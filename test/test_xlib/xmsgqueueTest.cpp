#include <iostream>
#include <fstream>
#include "xmsgqueueTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/xmsgqueue.h"
#include "xlib/xthread.h"

/**
コンストラクタ
@param name [in] テスト名
*/
xmsgqueueTest::xmsgqueueTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void xmsgqueueTest::setUp(void)
{
}


/** テスト後処理 */
void xmsgqueueTest::tearDown(void)
{
}

/**	@brief	テストスイート */
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

		
		// 1番で書き込み
		queue.push(1, szData, sizeof(szData));

		XThread::Sleep(2, 0);

		// 2番で書き込み
		queue.push(2, szData, sizeof(szData));

		
		return NULL;
	
	}
	XQueue& queue;
};

// popコールバック関数
int	xmsgqueueTest_testXQueue_PopCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize,void* pUserData)
{
	int* pAddr = (int*)pUserData;

	// ユーザーデータで、コールバックがコールされた回数をカウントアップ
	(*pAddr)++;

	if( ulMsgID == 12 ) {
		// メッセージＩＤ：１２番は無視する。
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

	

	// 自分で入れて、
	ulMsgId++;
	queue.push(ulMsgId, szData, sizeof(szData));
	// 自分で取り出す
	pMsg = queue.pop(ulMsgId);
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == ulMsgId );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();


	ThreadTestXQueue*	thread = new ThreadTestXQueue(queue);
	thread->start();

	// メッセージID：２番でまつ
	pMsg = queue.pop( 2 );
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 2 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();
	// エントリーに入っているメッセージの先頭を取得
	pMsg = queue.pop();
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 1 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();

	//  メッセージID：3番でまつ、書き込みがないのでタイムアウトするはず
	pMsg = queue.pop(3, 1000);
	CPPUNIT_ASSERT( pMsg == NULL );

	thread->Join(0);

	// popコールバック関数を登録します。
	queue.connectPopCallBack(xmsgqueueTest_testXQueue_PopCallBack, &iUserData);

	queue.push(10, szData, sizeof(szData));
	queue.push(11, szData, sizeof(szData));
	queue.push(12, szData, sizeof(szData));
	queue.push(13, szData, sizeof(szData));
	CPPUNIT_ASSERT( iUserData == 4 );	// ユーザーデータが更新されているか確認
	// メッセージＩＤ：１２番だけは無視されているはずなので、popで取得できる。
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

	// タイムアウト
	pMsg = puller.pop(XQueue_MsgID_ANY, 1000) ;
	CPPUNIT_ASSERT( pMsg == NULL );

	// Push
	CPPUNIT_ASSERT( pusher.push(0, data, sizeof(data)) == XMsgQueueERR_SUCCESS );
	// Push
	CPPUNIT_ASSERT( pusher.push(1, data, strlen(data)+1) == XMsgQueueERR_SUCCESS );

	// 1から取り出し
	pMsg = puller.pop(1, 1000);
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 1 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == strlen(data)+1 );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), data, pMsg->getDataSize()) == 0  );

	//　先頭から順にから取り出し
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
			//	リクエスト受信ケース1
			//
			char *pCData = (char*)pData;
			CPPUNIT_ASSERT(dataSize == 5000);
			for(int i = 0; i < dataSize; i++ ) {
				char c = i % 0xFF;
				CPPUNIT_ASSERT( pCData[i] == c );
			}
			
			// リクエストデータをそのまま、レスポンスで返す
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

	// サーバーのオープン
	CPPUNIT_ASSERT( pServer->open(_T("TEST")) == XMsgQueueERR_SUCCESS );
	// サーバーのスレッド開始
	pServer->startMainLoopThread();


	// クライアントの接続
	CPPUNIT_ASSERT( client.open(_T("TEST")) == XMsgQueueERR_SUCCESS );
	CPPUNIT_ASSERT( client2.open(_T("TEST")) == XMsgQueueERR_SUCCESS );
	CPPUNIT_ASSERT( client3.open(_T("TEST")) == XMsgQueueERR_SUCCESS );
	CPPUNIT_ASSERT( client4.open(_T("TEST")) == XMsgQueueERR_SUCCESS );


	// リクエスト、レスポンスを受信
	CPPUNIT_ASSERT( client.sendRequset(szData, sizeof(szData),
										buffRsp, &sizeResp, 3*1000) == XMsgQueueERR_SUCCESS );
	pData = buffRsp;
	CPPUNIT_ASSERT( sizeResp == sizeof(szData) );
	for(i = 0; i < sizeof(szData); i++ ) {
		char c = i % 0xFF;
		CPPUNIT_ASSERT( pData[i] == c );
	}
	// リクエスト、レスポンスを受信
	CPPUNIT_ASSERT( client2.sendRequset(szData, sizeof(szData),
										buffRsp, &sizeResp, 3*1000) == XMsgQueueERR_SUCCESS );
	pData = buffRsp;
	CPPUNIT_ASSERT( sizeResp == sizeof(szData) );
	for(i = 0; i < sizeof(szData); i++ ) {
		char c = i % 0xFF;
		CPPUNIT_ASSERT( pData[i] == c );
	}
	// リクエスト、レスポンスを受信
	CPPUNIT_ASSERT( client3.sendRequset(szData, sizeof(szData),
										buffRsp, &sizeResp, 3*1000) == XMsgQueueERR_SUCCESS );
	pData = buffRsp;
	CPPUNIT_ASSERT( sizeResp == sizeof(szData) );
	for(i = 0; i < sizeof(szData); i++ ) {
		char c = i % 0xFF;
		CPPUNIT_ASSERT( pData[i] == c );
	}
	// リクエスト、レスポンスを受信
	CPPUNIT_ASSERT( client4.sendRequset(szData, sizeof(szData),
										buffRsp, &sizeResp, 3*1000) == XMsgQueueERR_SUCCESS );
	pData = buffRsp;
	CPPUNIT_ASSERT( sizeResp == sizeof(szData) );
	for(i = 0; i < sizeof(szData); i++ ) {
		char c = i % 0xFF;
		CPPUNIT_ASSERT( pData[i] == c );
	}

	// クライアントの切断
	client.close();
	client2.close();
	client3.close();
	client4.close();
	// サーバーのクローズ
	pServer->close();
	// サーバーのスレッドの停止を待つ
	pServer->Join(0);
}

