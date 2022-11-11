/**
 * @file xmsgqueue.h
 * @brief ���b�Z�[�W�L���[�N���X
 *
 * @author T.Fujiha
 * @date 2007/07/15
 * @version 1.0
 *
 * Copyright (C) 2007 T.Fujiha,
 */
#ifndef __X_MESSAGE_QUEUE_FILE__
#define __X_MESSAGE_QUEUE_FILE__


#include "xlib/xlib_head.h"
#include "xlib/xio.h"
#include "xlib/xlock.h"
#include "xlib/xbuff.h"
#include "xlib/xthread.h"
#include "xlib/xmap.h"

class XLIB_API XQueueMsg;
class XLIB_API XQueueMsgCS;
class XLIB_API XQueue;
struct XMsgQueueBuff;
class XLIB_API XIPCQueuePuller;
class XLIB_API XIPCQueuePusher;



/**	@brief	���s�X�e�[�^�X */
enum XMsgQueueERR
{
		XMsgQueueERR_SUCCESS			///<	�G���[�͔������Ă��Ȃ�
	,	XMsgQueueERR_OPEN_DEVICE		///<	�f�o�C�X�̃I�[�v���Ɏ��s
	,	XMsgQueueERR_GET_QUEUE_INFO		///<	�f�o�C�X��������擾�ł��Ȃ�
	,	XMsgQueueERR_SET_QUEUE_INFO		///<	Queue���̐ݒ�Ɏ��s����
	,	XMsgQueueERR_NOT_OPEND			///<	open���������Ă��Ȃ�
	,	XMsgQueueERR_PULL				///<	�L���[���烁�b�Z�[�W�̓ǂݎ��ŃG���[�����������B
	,	XMsgQueueERR_WRITE				///<	�f�o�C�X�ւ̏������݂ɂŃG���[������
	,	XMsgQueueERR_NAK				///<	Peer�ɂ���đ��삪���ۂ���܂����B
	,	XMsgQueueERR_RCV_DATA_BROKEN	///<	��M�f�[�^�����Ă���
	,	XMsgQueueERR_RCV_TIMEO			///<	��M�^�C���A�E�g
};

#define XMsgQueueCS_REQUEST				0
#define XMsgQueueCS_RESPONSE			1
#define XMsgQueueCS_EVENT				2
#define XMsgQueueCS_CLINET_CONNECT		50
#define XMsgQueueCS_CLINET_CONNECT_OK	51
#define XMsgQueueCS_CLINET_CONNECT_NG	52
#define XMsgQueueCS_CLINET_DISCONNECT	60
#define XMsgQueueCS_CLINET_TERMINATE	(XD_UINT2)-1

#define XQueue_MsgID_ANY				(XD_UINT4)-1

#define XQueue_TIMEOUT_NOWAIT			0
#define XQueue_TIMEOUT_FOREVER			(XD_UINT4)-1

#define XQueue_POP_CALLBCK_IGNORE		0	///<	pop�R�[���o�b�N�֐��ŏ������Ȃ�����
#define XQueue_POP_CALLBCK_PROCESS		1	///<	pop�R�[���o�b�N�֐��ŏ�������


#define XMsgQueueCS_SIZE				10
#define XMsgQueueCS_POS_CLINET_ID		2

struct XMsgQueueCS {
    unsigned short	type;			// ���b�Z�[�W�^�C�v
	XD_UINT1		clientid[8];	// �N���C�A���gID
};




/**	@brief	�L���[���b�Z�[�W	*/
class XLIB_API XQueueMsg 
{
protected:
	XQueueMsg(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);
	virtual ~XQueueMsg();
public:
	/**	@brief	���̃C���X�^���X���폜���܂��B */
	virtual void deleteMe();

	/**	@brief	���b�Z�[�WID���擾���܂��B
	 *
	 *	@return	���b�Z�[�WID
	 */
	XD_UINT4 getMsgID() const;

	/**	@brief	�f�[�^�T�C�Y���擾���܂��B
	 *
	 *	@return	�f�[�^�T�C�Y
	 */
	XD_UINT4 getDataSize() const;

	/**	@brief	�f�[�^�̃A�h���X���擾���܂��B
	 *
	 *	@return	�f�[�^�̃A�h���X
	 */
	const char* getData() const;
protected:
	XD_UINT4	m_ulMsgID;
	XD_UINT4	m_ulMsgDataSize;
	char*		m_pMsgData;
friend class XLIB_API XQueue ;
friend class XLIB_API XQueueMsgCS;
};



/**	@brief	�L���[
@verbatim
{
	XQueue	queue;
	XQueueMsg	*pMsg;
	XD_UINT4 ulMsgId = 0;
	char	szData[5000];

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
	// ����Ȃ��Ȃ�����폜����B
	pMsg->deleteMe();

	// �L���[�Ƀf�[�^��push����X���b�h���J�n
	ThreadTestXQueue*	thread = new ThreadTestXQueue(queue);
	thread->start();

	// �X���b�h���A���b�Z�[�WID�F�Q�Ԃŏ������݁A������擾
	pMsg = queue.pop( 2 );
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 2 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	// ����Ȃ��Ȃ�����폜����B
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
	queue.connectPopCallBack(_PopCallBack, &iUserData);

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


// pop�R�[���o�b�N�֐�
int	_PopCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize,void* pUserData)
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
	...

// �L���[�Ƀf�[�^��push����X���b�h
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

		
		// ���b�Z�[�WID�@1�Ԃŏ�������
		queue.push(1, szData, sizeof(szData));

		XThread::Sleep(2, 0);

		// ���b�Z�[�WID�@2�Ԃŏ�������
		queue.push(2, szData, sizeof(szData));

		
		return NULL;
	
	}
	XQueue& queue;
};
@endverbatim
*/
class XLIB_API XQueue 
{
public:
	XQueue();
	virtual ~XQueue();

	/**	@brief	�L���[�G���g���[����ɂ��܂��B */
	void clear();
	
	/**	@brief	�L���[�Ƀ��b�Z�[�W���Z�b�g���܂��B
	 *
	 *	@param	ulMsgID	���b�Z�[�WID
	 *	@param	pData	�f�[�^
	 *	@param	ulDataSize	�f�[�^�̃T�C�Y
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool push(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);

	/**	@brief	�L���[���烁�b�Z�[�W�����o�����܂��B
	 *
	 *	@param	ulMsgID	���o�����b�Z�[�WID�B
	 *				- XQueue_MsgID_ANY���w�肷��ƁA�L���[�̐擪�ɂ��郁�b�Z�[�W�����o���B
	 *	@param	ulTimeout	���o���܂ł̃^�C���A�E�g�l(�~���b)���w��B
	 *				- XQueue_TIMEOUT_NOWAIT�́A�L���[�Ƀf�[�^���Ȃ��ꍇ�A�҂����ɒ����Ƀ��^�[������B
	 *				- XQueue_TIMEOUT_FOREVER�́A�����ɑ҂B
	 *
	 *	@return	���b�Z�[�W�B�^�C���A�E�g�����ꍇ�́ANULL�B
	 */
	XQueueMsg* pop(XD_UINT4 ulMsgID = XQueue_MsgID_ANY, XD_UINT4 ulTimeout = XQueue_TIMEOUT_FOREVER);

	/**	@brief	pop�R�[���o�b�N�֐���o�^���܂��B
	 *	@note	pop�R�[���o�b�N�֐���o�^����ƁA�Ăяo�����́Apop�֐����R�[�����Ȃ��Ă��A
	 *			�R�[���o�b�N����f�[�^�𓾂邱�Ƃ΂ł��܂��B�R�[���o�b�N�֐��̌^�͈ȉ��̒ʂ�
	 *			- int	PopCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize, void* pUserData);
	 *			 - �p�����[�^:
	 *			  - ulMsgID ���b�Z�[�WID
	 *			  - pData �f�[�^
	 *			  - ulDataSize �f�[�^�T�C�Y
	 *			  - pUserData ���[�U�[�f�[�^
	 *			 - �߂�l:
	 *			  - XQueue_POP_CALLBCK_IGNORE	�����𖳎������B�Ăяo�����́Apop�֐����R�[�����āA
	 *													�f�[�^�̏������s��Ȃ���΍s���Ȃ�
	 *			  - XQueue_POP_CALLBCK_PROCESS	�f�[�^�����������B
	 *	@param	pPopCallBack	pop�R�[���o�b�N�֐��B�o�^��������ꍇ�́ANULL���w�肵�܂��B	
	 *	@param	pUserData	pop�R�[���o�b�N�֐��ɓn����郆�[�U�[�f�[�^���w�肵�܂��B
	 */
	void connectPopCallBack(int (*pPopCallBack)(XD_UINT4, char*, XD_UINT4,void*), void* pUserData);
private:
	class popperEntry{
	public:
		popperEntry(XD_UINT4 ulMsgID, XQueueMsg** ppMsg);
		~popperEntry();
		XD_UINT4	m_ulMsgID;
		XQueueMsg**		m_ppMsg;
		XSemaphore	m_lock;
	};
	XMutex		m_push_lock;
	XMutex		m_pop_lock;
	XMutex		m_popperEntry_lock;
	XList		m_list;
	XList		m_listPopperEntry;
	int			(*m_pPopCallBack)(XD_UINT4, char*, XD_UINT4,void*);
	void*		m_pPopCallBackUserData;
friend class XQueueMsg;
};





/** @brief �L���[���烁�b�Z�[�W���擾����N���X(IPC:InterProcess Communication)	*/
class XLIB_API XIPCQueuePuller
{
public:
	/** @brief �R���X�g���N�^*/
	XIPCQueuePuller();
	virtual ~XIPCQueuePuller();

	
	/**	@brief	�L���[���I�[�v�����܂��B
	 *
	 *	@param	pName	��ӂɎ��ʂ��閼�O���w�肵�܂��B
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR open(const char* pName);
	/**	@brief	�L���[���I�[�v�����܂��B
	 *
	 *	@param	pName	��ӂɎ��ʂ��閼�O���w�肵�܂��B
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR open(const wchar_t* pName);
	/**	@brief	�L���[����܂�
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR close();
	/**	@brief	�L���[���烁�b�Z�[�W�����o�����܂��B
	 *
	 *	@param	ulMsgID	���o�����b�Z�[�WID�B
	 *				- XQueue_MsgID_ANY���w�肷��ƁA�L���[�̐擪�ɂ��郁�b�Z�[�W�����o���B
	 *	@param	ulTimeout	���o���܂ł̃^�C���A�E�g�l���w��B
	 *				- XQueue_TIMEOUT_NOWAIT�́A�L���[�Ƀf�[�^���Ȃ��ꍇ�A�҂����ɒ����Ƀ��^�[������B
	 *				- XQueue_TIMEOUT_FOREVER�́A�����ɑ҂B
	 *
	 *	@return	���b�Z�[�W�B�^�C���A�E�g�����ꍇ�́ANULL�B
	 */
	XQueueMsg* pop(XD_UINT4 ulMsgID = XQueue_MsgID_ANY, XD_UINT4 ulTimeout = XQueue_TIMEOUT_FOREVER);

	/**	@brief	�L���[�ɂ��郁�b�Z�[�W�̐���Ԃ��܂��B
	 *	@return	���b�Z�[�W��
	 */
	XD_UINT4 getMessageCount();

	/**	@brief	pop�R�[���o�b�N�֐���o�^���܂��B
	 *	@note	pop�R�[���o�b�N�֐���o�^����ƁA�Ăяo�����́Apop�֐����R�[�����Ȃ��Ă��A
	 *			�R�[���o�b�N����f�[�^�𓾂邱�Ƃ΂ł��܂��B�R�[���o�b�N�֐��̌^�͈ȉ��̒ʂ�
	 *			- int	PopCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize, void* pUserData);
	 *			 - �p�����[�^:
	 *			  - ulMsgID ���b�Z�[�WID
	 *			  - pData �f�[�^
	 *			  - ulDataSize �f�[�^�T�C�Y
	 *			  - pUserData ���[�U�[�f�[�^
	 *			 - �߂�l:
	 *			  - XQueue_POP_CALLBCK_IGNORE	�����𖳎������B�Ăяo�����́Apop�֐����R�[�����āA
	 *													�f�[�^�̏������s��Ȃ���΍s���Ȃ�
	 *			  - XQueue_POP_CALLBCK_PROCESS	�f�[�^�����������B
	 *	@param	pPopCallBack	pop�R�[���o�b�N�֐��B�o�^��������ꍇ�́ANULL���w�肵�܂��B	
	 *	@param	pUserData	pop�R�[���o�b�N�֐��ɓn����郆�[�U�[�f�[�^���w�肵�܂��B
	 */
	void connectPopCallBack(int (*pPopCallBack)(XD_UINT4, char*, XD_UINT4,void*), void* pUserData);
protected:

	XMsgQueueERR innerReading();

	class readerThread : public XThread
	{
	public:
		readerThread(XIPCQueuePuller* puller);
		~readerThread();
		void start();
		virtual void* RunUndetached(void* pArg);
		XIPCQueuePuller*	m_puller;
	};


	XMutex			m_lock;
	int				m_iHandle;
	int				m_iMsgQueueID;
	XD_UINT4		m_ulCurrentTimeOut;
	bool			m_bRunning;
	readerThread*	m_pReaderThread;
	XQueue			m_q;
friend class readerThread;
friend class XLIB_API XMsgQueueServer;
};


/** @brief �L���[�փf�[�^���Z�b�g����N���X(IPC:InterProcess Communication)�@*/
class XLIB_API XIPCQueuePusher
{
public:
	/** @brief �R���X�g���N�^*/
	XIPCQueuePusher();

	virtual ~XIPCQueuePusher();

	/**	@brief	�L���[���I�[�v�����܂��B
	 *
	 *	@param	pName	���ɁA�I�[�v������Ă���Puller�̖��O���w�肵�܂��B
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR open(const char* pName);

	/**	@brief	�L���[���I�[�v�����܂��B
	 *
	 *	@param	pName	���ɁA�I�[�v������Ă���Puller�̖��O���w�肵�܂��B
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR open(const wchar_t* pName);
	/**	@brief	�L���[����܂�
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR close();
	/**	@brief	�L���[�փf�[�^���Z�b�g���܂��B
	 *
	 *	@param	pData	�Z�b�g����f�[�^
	 *	@param	nDataSize	�f�[�^�T�C�Y
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR push(XD_UINT4 ulMsgID, void *pData, size_t nDataSize);

protected:
	XMutex	m_lock;
	int		m_iHandle;
	int		m_iMsgQueueID;
};



/** @brief �L���[���g�p���ĒʐM���s���N���C�A���g�N���X�@*/
class XLIB_API XMsgQueueClient
{
public:
	/** @brief �R���X�g���N�^*/
	XMsgQueueClient();
	virtual ~XMsgQueueClient();

	/**	@brief	�L���[�T�[�o�[�̂Ƃ̐ڑ����m�����܂��B
	 *
	 *	@param	pName	�T�[�o�[���I�[�v��s�������O
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR open(const char* pName);
	/**	@brief	�L���[�T�[�o�[�̂Ƃ̐ڑ����m�����܂��B
	 *
	 *	@param	pName	�T�[�o�[���I�[�v��s�������O
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR open(const wchar_t* pName);
	
	/**	@brief	�L���[�T�[�o�[�̂Ƃ̐ڑ�����܂��B
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR close();

	/**	@brief	�N���C�A���gID���擾���܂��B
	 *	@return	�N���C�A���gID
	 */
	XD_UINT8 getID() const { return m_id; }
	
	/**	@brief	���N�G�X�g�𑗐M���āA���X�|���X����M���܂��B
	 *
	 *	@param	pData	���M�f�[�^
	 *	@param	nDataSize	���M�f�[�^�̃T�C�Y
	 *	@param	dataBuff	���X�|���X���󂯎��o�b�t�@
	 *	@param	pDataSize	���X�|���X�̃T�C�Y
	 *	@param	ulTimeOut	�^�C���A�E�g�l�B
	 *				- XQueue_TIMEOUT_NOWAIT�́A�L���[�Ƀf�[�^���Ȃ��ꍇ�A�҂����ɒ����Ƀ��^�[������B
	 *				- XQueue_TIMEOUT_FOREVER�́A�����ɑ҂B
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR sendRequset(void *pData, size_t nDataSize, XBuff& dataBuff, size_t *pDataSize, XD_UINT4 ulTimeOut = XQueue_TIMEOUT_NOWAIT);

	/**	@brief	�L���[���烁�b�Z�[�W�����o�����܂��B
	 *
	 *	@param	dataBuff	���b�Z�[�W���󂯎��o�b�t�@
	 *	@param	pDataSize	�󂯎�������b�Z�[�W�T�C�Y
	 *	@param	ulMsgID	���o�����b�Z�[�WID�B
	 *				- XQueue_MsgID_ANY���w�肷��ƁA�L���[�̐擪�ɂ��郁�b�Z�[�W�����o���B
	 *	@param	ulTimeout	���o���܂ł̃^�C���A�E�g�l���w��B
	 *				- XQueue_TIMEOUT_NOWAIT�́A�L���[�Ƀf�[�^���Ȃ��ꍇ�A�҂����ɒ����Ƀ��^�[������B
	 *				- XQueue_TIMEOUT_FOREVER�́A�����ɑ҂B
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR pop(XBuff& dataBuff, size_t *pDataSize, XD_UINT4 ulMsgID = XQueue_MsgID_ANY, XD_UINT4 ulTimeout = XQueue_TIMEOUT_FOREVER);

	/**	@brief	�h���N���X�ŁApop�R�[���o�b�N���������܂��B�f�t�H���g�́A���������A�L���[�ɃZ�b�g����܂��B
	 *
	 *	@param	ulMsgID	���b�Z�[�W�h�c
	 *	@param	pData	�f�[�^
	 *	@param	ulDataSize	�f�[�^�T�C�Y
	 *
	 *	@retval	XQueue_POP_CALLBCK_IGNORE	�����𖳎������B�Ăяo�����́Apop�֐����R�[�����āA�f�[�^�̏������s��Ȃ���΍s���Ȃ�
	 *	@retval	XQueue_POP_CALLBCK_PROCESS	�f�[�^�����������B
	 */
	virtual int popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);
	

	/*	@brief	�N���C�A���gID */
	union ClinentID {
		XD_UINT8			m_id8;
		XD_UINT4			m_id4[2];	// 0:�v���Z�XID 1:���̃C���X�^���X��ID
		XD_UINT2			m_id2[4];
		XD_UINT1			m_id1[8];
	};
private:
	static int _popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize, void* pUserData);
	int _popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);

protected:
	XIPCQueuePuller		m_puller;
	XIPCQueuePusher		m_pusher;
	XD_UINT8			m_id;

	XMutex				m_lockpop;		//  POP,���b�N�I�u�W�F�N�g
	XMapAddr			m_popIDs;		//  ���M���b�Z�[�W�h�c�Ǘ��f�[�^
	XMutex				m_lockSequence;	//  �V�[�P���X�i���o�[,���b�N�I�u�W�F�N�g
	XD_UINT4			m_ulSequenceNo;	//	�V�[�P���X�i���o�[
};


/** @brief �L���[���g�p���ĒʐM���s���T�[�o�[�N���X�@*/
class XLIB_API XMsgQueueServer : public XThread
{
protected:
	class ClinetData;	// �T�[�o�[�ɐڑ������N���C�A���g�̏��
public:
	XMsgQueueServer();
	virtual ~XMsgQueueServer();


	/**	@brief	�T�[�o�[���I�[�v�����܂��B
	 *
	 *	@param	pName	���O
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR open(const char* pName);
	/**	@brief	�T�[�o�[���I�[�v�����܂��B
	 *
	 *	@param	pName	���O
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR open(const wchar_t* pName);
	/**	@brief	�T�[�o�[����܂��B
	 *
	 *	@return	���s�X�e�[�^�X	
	 */
	XMsgQueueERR close();

	/**	@brief	�T�[�o�[�̃��C�����[�v�X���b�h���J�n���܂��B
	 */
	void startMainLoopThread();

	/**	@brief	�C�x���g�𑗐M���܂��B
	 *
	 *	@param	pData	���M�f�[�^
	 *	@param	dataSize	�f�[�^�̃T�C�Y
	 *	@param	uClientID	�N���C�A���gID�B
	 *
	 *	@return	���s�X�e�[�^�X		
	 */
	XMsgQueueERR sendEvent(void *pData, size_t dataSize, XD_UINT8 uClientID = (XD_UINT8)(-1));
	
// property
	int getClientsCount();
	XD_UINT8 getClientID(int index);


// must Overwrite
	/**	@brief	�h���N���X�ŁA�N���C�A���g���烊�N�G�X�g����M�������̓�����L�q���܂��B
	 *
	 *	@param	pClient	�N���C�A���g���
	 *	@param	pData	���N�G�X�g�̃f�[�^
	 *	@param	dataSize	���N�G�X�g�f�[�^�T�C�Y
	 */
	virtual void receiveRequest(ClinetData* pClient, XD_UINT4 ulMsgID, void *pData, size_t dataSize) = 0;

	/**	@brief	�N���C�A���g�̐ڑ��̏󋵕ω���ʒm���܂��B
	 *	@param	ulClientID	�N���C�A���gID
	 *	@param	bConnect	true:�ڑ�/false:�ؒf
	 */
	virtual void ntfyConnection(XD_UINT8 ulClientID, bool bConnect){};
protected:
	virtual void* RunUndetached(void* pArg);

	/**	@brief	�T�[�o�[�ɐڑ������N���C�A���g�̏��*/
	class ClinetData {
	public:
		/**	@brief	�N���C�A���gID���擾���܂��B
		 *	@return	�N���C�A���gID
		 */
		XD_UINT8 getClinetID(){ return m_clientID; }
		/**	@brief	���X�|���X�𑗐M���܂��B
		 *
		 *	@param	pData	���X�|���X�f�[�^
		 *	@param	dataSize	���X�|���X�f�[�^�̃T�C�Y
		 *
		 *	@return	���s�X�e�[�^�X	
		 */
		XMsgQueueERR sendResponse(XD_UINT4 ulMsgID, const void *pData, size_t dataSize);
	private:
		ClinetData(XD_UINT8 clientID):m_clientID(clientID){}
		~ClinetData(){ m_pusher.close(); }
		XIPCQueuePusher		m_pusher;
		XD_UINT8			m_clientID;

	friend class XMsgQueueServer;
	};
protected:
	XIPCQueuePuller		m_puller;
	XStrBuff			m_strName;
	XList				m_clients;
	XMutex				m_lockClinets;
	bool				m_bRunning;
	XMutex				m_lockSequence;	//  �V�[�P���X�i���o�[,���b�N�I�u�W�F�N�g
	XD_UINT4			m_ulSequenceNo;	//	�V�[�P���X�i���o�[
};


#include "xlib/xlib_tail.h"
#endif /*__X_MESSAGE_QUEUE_FILE__*/


