#include <iostream>
#include <fstream>
#include "xmsgqueueTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/xmsgqueue.h"
#include "xlib/xthread.h"

/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
xmsgqueueTest::xmsgqueueTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void xmsgqueueTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void xmsgqueueTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* xmsgqueueTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("xmsgqueueTest");


	suite->addTest( new CPPUNIT_NS::TestCaller<xmsgqueueTest>("testXQueue", &testXQueue) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xmsgqueueTest>("testPull", &testPull) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xmsgqueueTest>("testClientServer", &testClientServer) );
	
	return suite;
}


class ThreadTestXQueue : public XThread
{
public:
	ThreadTestXQueue(XQueue& Q)
		:	queue( Q )
	{}
	~ThreadTestXQueue(){}
	void start(){ StartUndetached(); }
    virtual void* RunUndetached(void* pArg) {
		XD_UINT4 ulMsgId = 0;
		char	szData[5000];
		memset(szData, 'a', sizeof(szData));

		
		// 1�Ԃŏ�������
		queue.push(1, szData, sizeof(szData));

		XThread::Sleep(2, 0);

		// 2�Ԃŏ�������
		queue.push(2, szData, sizeof(szData));

		
		return NULL;
	
	}
	XQueue& queue;
};

// pop�R�[���o�b�N�֐�
int	xmsgqueueTest_testXQueue_PopCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize,void* pUserData)
{
	int* pAddr = (int*)pUserData;

	// ���[�U�[�f�[�^�ŁA�R�[���o�b�N���R�[�����ꂽ�񐔂��J�E���g�A�b�v
	(*pAddr)++;

	if( ulMsgID == 12 ) {
		// ���b�Z�[�W�h�c�F�P�Q�Ԃ͖�������B
		return XQueue_POP_CALLBCK_IGNORE;
	}

	return XQueue_POP_CALLBCK_PROCESS;
}


void xmsgqueueTest::testXQueue(void)
{
	XQueue	queue;
	XQueueMsg	*pMsg;
	XD_UINT4 ulMsgId = 0;
	char	szData[5000];
	int		iUserData = 0;

	memset(szData, 'a', sizeof(szData));

	

	// �����œ���āA
	ulMsgId++;
	queue.push(ulMsgId, szData, sizeof(szData));
	// �����Ŏ��o��
	pMsg = queue.pop(ulMsgId);
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == ulMsgId );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();


	ThreadTestXQueue*	thread = new ThreadTestXQueue(queue);
	thread->start();

	// ���b�Z�[�WID�F�Q�Ԃł܂�
	pMsg = queue.pop( 2 );
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 2 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();
	// �G���g���[�ɓ����Ă��郁�b�Z�[�W�̐擪���擾
	pMsg = queue.pop();
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 1 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();

	//  ���b�Z�[�WID�F3�Ԃł܂A�������݂��Ȃ��̂Ń^�C���A�E�g����͂�
	pMsg = queue.pop(3, 1000);
	CPPUNIT_ASSERT( pMsg == NULL );

	thread->Join(0);

	// pop�R�[���o�b�N�֐���o�^���܂��B
	queue.connectPopCallBack(xmsgqueueTest_testXQueue_PopCallBack, &iUserData);

	queue.push(10, szData, sizeof(szData));
	queue.push(11, szData, sizeof(szData));
	queue.push(12, szData, sizeof(szData));
	queue.push(13, szData, sizeof(szData));
	CPPUNIT_ASSERT( iUserData == 4 );	// ���[�U�[�f�[�^���X�V����Ă��邩�m�F
	// ���b�Z�[�W�h�c�F�P�Q�Ԃ����͖�������Ă���͂��Ȃ̂ŁApop�Ŏ擾�ł���B
	pMsg = queue.pop();
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 12 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();
}


void xmsgqueueTest::testPull(void)
{
	XIPCQueuePuller puller;
	XIPCQueuePusher	pusher;
//	size_t nDataSize;
	XBuff	buff;
	XQueueMsg	*pMsg;

	char data[5000];

	memset(data, 0 , sizeof(data));
	strcpy(data, "ABCDEFG");

	CPPUNIT_ASSERT( puller.open(_T("TEST.data")) == XMsgQueueERR_SUCCESS );
	CPPUNIT_ASSERT( pusher.open(_T("TEST.data")) == XMsgQueueERR_SUCCESS );

	// �^�C���A�E�g
	pMsg = puller.pop(XQueue_MsgID_ANY, 1000) ;
	CPPUNIT_ASSERT( pMsg == NULL );

	// Push
	CPPUNIT_ASSERT( pusher.push(0, data, sizeof(data)) == XMsgQueueERR_SUCCESS );
	// Push
	CPPUNIT_ASSERT( pusher.push(1, data, strlen(data)+1) == XMsgQueueERR_SUCCESS );

	// 1������o��
	pMsg = puller.pop(1, 1000);
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 1 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == strlen(data)+1 );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), data, pMsg->getDataSize()) == 0  );

	//�@�擪���珇�ɂ�����o��
	pMsg = puller.pop(XQueue_MsgID_ANY, 1000);
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 0 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(data) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), data, pMsg->getDataSize()) == 0  );

}



class xmsgqueueTestServer : public XMsgQueueServer
{
public:
	xmsgqueueTestServer(){	m_iCase = 0; }
	~xmsgqueueTestServer(){}
	virtual void receiveRequest(XMsgQueueServer::ClinetData* pClient, XD_UINT4 ulMsgID, void *pData, size_t dataSize)
	{
		if( m_iCase <= 3 ) {
			//
			//	���N�G�X�g��M�P�[�X1
			//
			char *pCData = (char*)pData;
			CPPUNIT_ASSERT(dataSize == 5000);
			for(int i = 0; i < dataSize; i++ ) {
				char c = i % 0xFF;
				CPPUNIT_ASSERT( pCData[i] == c );
			}
			
			// ���N�G�X�g�f�[�^�����̂܂܁A���X�|���X�ŕԂ�
			pClient->sendResponse(ulMsgID, pData, dataSize);
		}
		m_iCase++;
	}
	int m_iCase;
};

void xmsgqueueTest::testClientServer(void)
{
	XMsgQueueServer *pServer = new xmsgqueueTestServer;
	XMsgQueueClient	client;
	XMsgQueueClient	client2;
	XMsgQueueClient	client3;
	XMsgQueueClient	client4;
	char szData[5000];
	char *pData;
	int i;
	XBuff	buffRsp;
	size_t	sizeResp;

	memset(szData, 0, sizeof(szData));
	for(i = 0; i < sizeof(szData); i++ ) {
		szData[i] = i % 0xFF;
	}

	// �T�[�o�[�̃I�[�v��
	CPPUNIT_ASSERT( pServer->open(_T("TEST")) == XMsgQueueERR_SUCCESS );
	// �T�[�o�[�̃X���b�h�J�n
	pServer->startMainLoopThread();


	// �N���C�A���g�̐ڑ�
	CPPUNIT_ASSERT( client.open(_T("TEST")) == XMsgQueueERR_SUCCESS );
	CPPUNIT_ASSERT( client2.open(_T("TEST")) == XMsgQueueERR_SUCCESS );
	CPPUNIT_ASSERT( client3.open(_T("TEST")) == XMsgQueueERR_SUCCESS );
	CPPUNIT_ASSERT( client4.open(_T("TEST")) == XMsgQueueERR_SUCCESS );


	// ���N�G�X�g�A���X�|���X����M
	CPPUNIT_ASSERT( client.sendRequset(szData, sizeof(szData),
										buffRsp, &sizeResp, 3*1000) == XMsgQueueERR_SUCCESS );
	pData = buffRsp;
	CPPUNIT_ASSERT( sizeResp == sizeof(szData) );
	for(i = 0; i < sizeof(szData); i++ ) {
		char c = i % 0xFF;
		CPPUNIT_ASSERT( pData[i] == c );
	}
	// ���N�G�X�g�A���X�|���X����M
	CPPUNIT_ASSERT( client2.sendRequset(szData, sizeof(szData),
										buffRsp, &sizeResp, 3*1000) == XMsgQueueERR_SUCCESS );
	pData = buffRsp;
	CPPUNIT_ASSERT( sizeResp == sizeof(szData) );
	for(i = 0; i < sizeof(szData); i++ ) {
		char c = i % 0xFF;
		CPPUNIT_ASSERT( pData[i] == c );
	}
	// ���N�G�X�g�A���X�|���X����M
	CPPUNIT_ASSERT( client3.sendRequset(szData, sizeof(szData),
										buffRsp, &sizeResp, 3*1000) == XMsgQueueERR_SUCCESS );
	pData = buffRsp;
	CPPUNIT_ASSERT( sizeResp == sizeof(szData) );
	for(i = 0; i < sizeof(szData); i++ ) {
		char c = i % 0xFF;
		CPPUNIT_ASSERT( pData[i] == c );
	}
	// ���N�G�X�g�A���X�|���X����M
	CPPUNIT_ASSERT( client4.sendRequset(szData, sizeof(szData),
										buffRsp, &sizeResp, 3*1000) == XMsgQueueERR_SUCCESS );
	pData = buffRsp;
	CPPUNIT_ASSERT( sizeResp == sizeof(szData) );
	for(i = 0; i < sizeof(szData); i++ ) {
		char c = i % 0xFF;
		CPPUNIT_ASSERT( pData[i] == c );
	}

	// �N���C�A���g�̐ؒf
	client.close();
	client2.close();
	client3.close();
	client4.close();
	// �T�[�o�[�̃N���[�Y
	pServer->close();
	// �T�[�o�[�̃X���b�h�̒�~��҂�
	pServer->Join(0);
}

