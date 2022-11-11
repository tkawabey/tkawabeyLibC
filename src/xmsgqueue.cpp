#include "xlib/xeception.h"
#include "xlib/xmsgqueue.h"
#include "xlib/xprocess.h"


//
//	XQueueMsg
//
XQueueMsg::XQueueMsg(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize)
	:	m_ulMsgID( ulMsgID )
	,	m_ulMsgDataSize(ulDataSize)
	,	m_pMsgData( NULL )
{
XEXCEPT_ENTRY
	if( m_ulMsgDataSize != 0 ) {
		m_pMsgData = new char[m_ulMsgDataSize];
		memcpy(m_pMsgData, pData, m_ulMsgDataSize);
	}
XEXCEPT_EXIT
}
XQueueMsg::~XQueueMsg()
{
XEXCEPT_ENTRY
	if( m_pMsgData ) {
		delete []m_pMsgData;
	}
	m_pMsgData = NULL;
	m_ulMsgDataSize = 0;
XEXCEPT_EXIT
}
void XQueueMsg::deleteMe()
{
XEXCEPT_ENTRY
	delete this;
XEXCEPT_EXIT
}
XD_UINT4 XQueueMsg::getMsgID() const
{
XEXCEPT_ENTRY
	return m_ulMsgID;
XEXCEPT_EXIT
}
XD_UINT4 XQueueMsg::getDataSize() const
{
XEXCEPT_ENTRY
	return m_ulMsgDataSize;
XEXCEPT_EXIT
}
const char* XQueueMsg::getData() const
{
XEXCEPT_ENTRY
	return m_pMsgData;
XEXCEPT_EXIT
}





//
//	XQueue
//
XQueue::popperEntry::popperEntry(XD_UINT4 ulMsgID, XQueueMsg** ppMsg)
	:	m_ulMsgID( ulMsgID )
	,	m_ppMsg( ppMsg )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XQueue::popperEntry::~popperEntry()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XQueue::XQueue()
	:	m_pPopCallBack( NULL )
	,	m_pPopCallBackUserData( NULL )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XQueue::~XQueue()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XQueue::clear()
{
XEXCEPT_ENTRY
	{
		// �z��̍Ō�Ƀ��b�Z�[�W��ǉ�
		XMutexer	locker(m_push_lock);
		XPOSITION pos;
		for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
			XQueueMsg* pMsg = (XQueueMsg*)m_list.GetNext( pos );
			pMsg->deleteMe();
		}
		m_list.RemoveAll();
	}
	{
		// ���ɑ҂��Ă���G���g���[�����邩����
		XMutexer	locker(m_popperEntry_lock);
		if( m_listPopperEntry.IsEmpty() == false ) {
			XPOSITION pos;
			for( pos = m_listPopperEntry.GetHeadPosition(); pos != NULL; ) {
				popperEntry* pDst = (popperEntry*)m_listPopperEntry.GetNext( pos );
				pDst->m_lock.unlock();
			}
			XThread::Sleep(1);
		}
	}
XEXCEPT_EXIT
}
bool XQueue::push(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize)
{
XEXCEPT_ENTRY

	// �V�������b�Z�[�W�̃C���X�^���X���쐬
	XQueueMsg *pMsg = new XQueueMsg(ulMsgID, pData, ulDataSize);

	{
		// ���ɑ҂��Ă���G���g���[�����邩����
		XMutexer	locker(m_popperEntry_lock);
		XPOSITION pos1, pos2;
		for( pos1 = m_listPopperEntry.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {

			popperEntry* pDst = (popperEntry*)m_listPopperEntry.GetNext( pos1 );
			if( pDst->m_ulMsgID == ulMsgID || pDst->m_ulMsgID == (XD_UINT4)-1 ) {
				*pDst->m_ppMsg = pMsg;
				pDst->m_lock.unlock();
				return true;
			}
		}

		if( m_pPopCallBack != NULL ) {
			// �R�[���o�b�N�֐��ŏ������ꂽ�ꍇ
			if( m_pPopCallBack(ulMsgID, pData, ulDataSize, m_pPopCallBackUserData) == XQueue_POP_CALLBCK_PROCESS ) {
				return true;
			}
		}
	}

	{
		// �z��̍Ō�Ƀ��b�Z�[�W��ǉ�
		XMutexer	locker(m_push_lock);
		m_list.AddTail( pMsg );
	}
XEXCEPT_EXIT
	return true;
}

XQueueMsg* XQueue::pop(XD_UINT4 ulMsgID /*= (XD_UINT4)-1*/, 
				  XD_UINT4 ulTimeout /*= (XD_UINT4)-1*/)
{
	popperEntry* pPopEntry = NULL;
	XQueueMsg*	pRetMsg = NULL;
XEXCEPT_ENTRY
	{
		XMutexer	locker( m_pop_lock );

		// �z��Ȃ��ɂ��邩�m�F����B
		{
			XMutexer	locker(m_push_lock);
			XPOSITION pos1, pos2;
			for( pos1 = m_list.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
				if( ((XQueueMsg*)m_list.GetNext( pos1 ))->m_ulMsgID == ulMsgID ||
							ulMsgID == (XD_UINT4)-1  ) {
			
					XQueueMsg* pa = (XQueueMsg*)m_list.GetAt( pos2 );
					m_list.RemoveAt( pos2 );
					return pa;
				}
			}
		}

		{
			XMutexer	locker(m_popperEntry_lock);
			// pop�G���g���[���쐬
			pPopEntry = new popperEntry(ulMsgID, &pRetMsg );
			m_listPopperEntry.AddTail( pPopEntry );
		}
	}

	pPopEntry->m_lock.lock( ulTimeout );

	{
		// pop�G���g���[���폜
		XMutexer	locker(m_popperEntry_lock);
		XPOSITION pos1, pos2;
		for( pos1 = m_listPopperEntry.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {

			popperEntry* pDst = (popperEntry*)m_listPopperEntry.GetNext( pos1 );

			if( pDst ==  pPopEntry ) {
				m_listPopperEntry.RemoveAt( pos2 );
				delete pPopEntry;
				break;
			}
		}
	}
XEXCEPT_EXIT
	return pRetMsg;
}
void XQueue::connectPopCallBack(int (*pPopCallBack)(XD_UINT4,char*,XD_UINT4,void*), void* pUserData)
{
XEXCEPT_ENTRY
	XMutexer	locker( m_popperEntry_lock );
	m_pPopCallBack = pPopCallBack;
	m_pPopCallBackUserData = pUserData;
XEXCEPT_EXIT
}

//
//	XIPCQueuePuller
//
XIPCQueuePuller::readerThread::readerThread(XIPCQueuePuller* puller)
	:	m_puller( puller )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XIPCQueuePuller::readerThread::~readerThread()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
void XIPCQueuePuller::readerThread::start()
{
XEXCEPT_ENTRY
	StartUndetached();
XEXCEPT_EXIT
}
void* XIPCQueuePuller::readerThread::RunUndetached(void* pArg)
{
XEXCEPT_ENTRY
	m_puller->innerReading();
	return NULL;
XEXCEPT_EXIT
}

XIPCQueuePuller::XIPCQueuePuller()
	:	m_iHandle( -1 )
	,	m_iMsgQueueID( -1 )
	,	m_ulCurrentTimeOut( 0 )
	,	m_bRunning( false )
	,	m_pReaderThread( NULL )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XIPCQueuePuller::~XIPCQueuePuller()
{
XEXCEPT_ENTRY
	close();
XEXCEPT_EXIT
}

XMsgQueueERR XIPCQueuePuller::open(const char* pName)
{
XEXCEPT_ENTRY
	return XIPCQueuePuller::open( (const wchar_t*)XStrBuff(pName) );
XEXCEPT_EXIT
}
XMsgQueueERR XIPCQueuePuller::open(const wchar_t* pName)
{
XEXCEPT_ENTRY

	XMutexer locker( m_lock );

#ifdef WIN32
	HANDLE h = NULL;
	XStrBuff strName(_T("\\\\.\\mailslot\\"));
	
	strName << pName;
	if( m_iHandle != -1 ) {
		h = (HANDLE)m_iHandle;
		m_iHandle = -1;
		::CloseHandle(h);
		h = NULL;
	}


	SECURITY_DESCRIPTOR secDesc;
	SECURITY_ATTRIBUTES secAttr;

	//MailBox�̃��[�U������݂��Ȃ��ݒ�
	::InitializeSecurityDescriptor (&secDesc, SECURITY_DESCRIPTOR_REVISION);
	::SetSecurityDescriptorDacl(&secDesc, true, 0, false);
	secAttr.nLength = sizeof (SECURITY_ATTRIBUTES);
	secAttr.bInheritHandle = FALSE;
	secAttr.lpSecurityDescriptor = &secDesc;

	// MailBox ���쐬����
	//XTRACE(_T("  CreateMailslot:[%s]\n"), strName.c_str());
	h = ::CreateMailslot(strName, 0, MAILSLOT_WAIT_FOREVER, &secAttr);
	if( h == INVALID_HANDLE_VALUE ) {
		return XMsgQueueERR_OPEN_DEVICE;
	}
	m_iHandle = (int)h;


	if( ::GetMailslotInfo(h, 0 , 0 , 0
			, &m_ulCurrentTimeOut) == FALSE ) {

		return XMsgQueueERR_GET_QUEUE_INFO;
	}
#else

	if( m_iHandle != -1 ) {
		::close(m_iHandle);
		m_iHandle = -1;
	}

	// �v���W�F�N�g���ʎq��ݒ�B�i�Œ�j
	const int PROJ_NUM = 0x55;

	// �t�@�C���� open ����
	m_iHandle = open(pName, O_CREAT|O_RDWR|O_EXCL, S_IRWXU|S_IRWXG|S_IRWXO);
	if( m_iHandle < 0 ) {
		m_iHandle = -1;
		return XMsgQueueERR_OPEN_DEVICE;
	}
	// �p�X���ƃv���W�F�N�g���ʎq�� System V IPC �ɕϊ�����
	key_t key = ::ftok(pName, PROJ_NUM);
	if(key < 0) {
		return XMsgQueueERR_OPEN_DEVICE;
	}
	// ���b�Z�[�W�̎��ʎq���擾����B
	/*
	 * IPC_CREAT(01000)
	 *	���݂̃L�[�ɑ΂���L�������݂��Ȃ���΁A �V�������b�Z�[�W�L�����쐬����B ������ IPC_EXCL �̎w�肪����΃G���[��Ԃ��B
	 * IPC_EXCL(02000)
	 *	�����L�[�ɑ΂�����̂����Ɋ��蓖�Ă��Ă���΃G���[��Ԃ��B
	 * IPC_NOWAIT(04000)
	 *	�҂��Ȃ���΂Ȃ�Ȃ��v�����A�G���[�Ƃ��܂��B
	*/
	if ((m_iMsgQueueID = msgget(key, IPC_CREAT|0666)) < 0) {
		m_iHandle = -1;
		return XMsgQueueERR_GET_QUEUE_INFO;
	}
#endif

	// �ǂݍ��݃X���b�h�J�n
	m_bRunning = true;
	m_pReaderThread = new readerThread(this);
	m_pReaderThread->start();

	return XMsgQueueERR_SUCCESS;
XEXCEPT_EXIT
}
XMsgQueueERR XIPCQueuePuller::close()
{
XEXCEPT_ENTRY
	XMutexer locker( m_lock );
	m_bRunning = false;
	if( m_iHandle != -1 ) {		
#ifdef WIN32
		HANDLE h = (HANDLE)m_iHandle;
		m_iHandle = -1;
		::CloseHandle(h);
#else
		int h = m_iHandle;
		m_iHandle = -1;
		::close(h);
#endif
	}
	if( m_pReaderThread != NULL ) {
		m_pReaderThread->Join(0);
		m_pReaderThread = NULL;
	}
	return XMsgQueueERR_SUCCESS;
XEXCEPT_EXIT
}
XMsgQueueERR XIPCQueuePuller::innerReading()
{
XEXCEPT_ENTRY
	XBuff buff;
	XD_UINT4 dwNextSize = 256;

	buff.Alloc(dwNextSize);

	if( m_iHandle == -1 ) {
		return XMsgQueueERR_NOT_OPEND;
	}
	while( m_bRunning == true ) {
		bool	bReadOK = false;
		XD_UINT4	ulDataSize = 0;
#ifdef WIN32
		DWORD	ulReceivedDataSize = 0;
		XD_UINT4	ulMsgID;
		HANDLE	h = (HANDLE)m_iHandle;


		// �ŏ��͂S�o�C�g�̃��b�Z�[�W��ID
		for(int l = 0; l < 2; l++) {
			bReadOK = false;
			//  ReadFilet�֐��̎��s�����s�̏ꍇ�͗�O�𑗏o
			if( ::ReadFile(h, buff, dwNextSize, &ulReceivedDataSize, NULL) == FALSE) {
				DWORD dwMaxMessageSize;  // �ő僁�b�Z�[�W�T�C�Y
				DWORD dwMessageCount=0;    // ���b�Z�[�W��
				DWORD dwReadTimeout;      // �ǂݎ��^�C���A�E�g�̊Ԋu

				if( ERROR_INSUFFICIENT_BUFFER == ::GetLastError() ) {
					// �o�b�t�@��������
					if( ::GetMailslotInfo(h
							, &dwMaxMessageSize , &dwNextSize
							, &dwMessageCount , &dwReadTimeout) == FALSE ) {
						break;
					}
					if( dwMessageCount == 0 || dwNextSize == MAILSLOT_NO_MESSAGE ) {
						break;
					}
					if( buff.GetBuffSize() < dwNextSize ) {
						buff.Alloc( dwNextSize+255);
					}
					if( ::ReadFile(h, buff, dwNextSize, &ulReceivedDataSize, NULL) == TRUE) {
						bReadOK = true;
					}
				}
			} else {
				bReadOK = true;
			}

			if( bReadOK == true ) {
				if( l == 0 ) {
					if( ulReceivedDataSize == sizeof(XD_UINT4) ) {
						::memcpy(&ulMsgID, buff, sizeof(XD_UINT4));
						bReadOK = false;
						continue;
					}
				} else {
					ulDataSize = ulReceivedDataSize;
				}
			}
		}
#else
		XMsgQueueBuff msgBuff;
		msgBuff.mtext = buff;

		if( msgrcv(m_iMsgQueueID, (struct msgbuf *)&msgBuff, dwNextSize, 0, 0) == -1) {

		}
#endif

		if( bReadOK == true ) {

#ifdef _DEBUG
			TCHAR szData[256] = _T("");
			char* pData = buff;
			if( ulDataSize > 20 ) {
				XStrBuff::printf(szData, XCOUNTOF(szData), _T("%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X..."),
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 0], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 1], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 2], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 3], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 4], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 5], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 6], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 7], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 8], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 9], 
					(XD_UINT1)pData[XMsgQueueCS_SIZE + 10]
					);
			}
			XTRACE(_T("  - XIPCQueuePuller::innerReading %s\n"), szData);
#endif

			m_q.push(ulMsgID, buff, ulDataSize);
		}
	}
XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}
XQueueMsg* XIPCQueuePuller::pop(XD_UINT4 ulMsgID /*= XQueue_MsgID_ANY*/, 
								XD_UINT4 ulTimeout /*= XQueue_TIMEOUT_FOREVER*/)
{
XEXCEPT_ENTRY
	return m_q.pop(ulMsgID, ulTimeout);
XEXCEPT_EXIT
}


XD_UINT4 XIPCQueuePuller::getMessageCount()
{
XEXCEPT_ENTRY
	XMutexer locker( m_lock );

	if( m_iHandle == -1 ) {
		return 0;
	}
#ifdef WIN32
	DWORD ulReceivedDataSize = 0;

	HANDLE h = (HANDLE)m_iHandle;
	DWORD dwMaxMessageSize;  // �ő僁�b�Z�[�W�T�C�Y
	DWORD dwNextSize;        // ���̃��b�Z�[�W�̃T�C�Y
	DWORD dwMessageCount=0;    // ���b�Z�[�W��
	DWORD dwReadTimeout;      // �ǂݎ��^�C���A�E�g

	// �o�b�t�@��������
	if( ::GetMailslotInfo(h
			, &dwMaxMessageSize , &dwNextSize
			, &dwMessageCount , &dwReadTimeout) == FALSE ) {
		return 0;
	}
	return dwMessageCount;
#else

#endif
XEXCEPT_EXIT
	return 0;
}
void XIPCQueuePuller::connectPopCallBack(int (*pPopCallBack)(XD_UINT4, char*, XD_UINT4,void*), 
										 void* pUserData)
{
XEXCEPT_ENTRY
	m_q.connectPopCallBack(pPopCallBack, pUserData);
XEXCEPT_EXIT
}



//
//	XIPCQueuePusher
//
XIPCQueuePusher::XIPCQueuePusher()
	:	m_iHandle( -1 )
	,	m_iMsgQueueID( -1 )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XIPCQueuePusher::~XIPCQueuePusher()
{
XEXCEPT_ENTRY
	close();
XEXCEPT_EXIT
}

XMsgQueueERR XIPCQueuePusher::open(const char* pName)
{
XEXCEPT_ENTRY
	return XIPCQueuePusher::open((const wchar_t*)XStrBuff(pName));
XEXCEPT_EXIT
}

XMsgQueueERR XIPCQueuePusher::open(const wchar_t* pName)
{
XEXCEPT_ENTRY
	

	XMutexer locker( m_lock );
#ifdef WIN32
	HANDLE h = NULL;
	XStrBuff strName(_T("\\\\.\\mailslot\\"));
	
	strName << pName;

	if( m_iHandle != -1 ) {
		h = (HANDLE)m_iHandle;
		m_iHandle = -1;
		::CloseHandle(h);
		h = NULL;
	}
	//XTRACE(_T("  Opening MailSlot:[%s]\n"), strName.c_str());
	h = ::CreateFile (strName, 
						GENERIC_WRITE, 
						FILE_SHARE_READ | FILE_SHARE_WRITE, 
						NULL, 
						OPEN_ALWAYS, 
						FILE_ATTRIBUTE_NORMAL, 
						NULL);
	if( h == INVALID_HANDLE_VALUE ) {
		return XMsgQueueERR_OPEN_DEVICE;
	}
	m_iHandle = (int)h;
#else

	if( m_iHandle != -1 ) {
		::close(m_iHandle);
		m_iHandle = -1;
	}

	// �v���W�F�N�g���ʎq��ݒ�B�i�Œ�j
	const int PROJ_NUM = 0x55;

	// �p�X���ƃv���W�F�N�g���ʎq�� System V IPC �ɕϊ�����
	key_t key = ::ftok(pName, PROJ_NUM);
	if(key < 0) {
		return XMsgQueueERR_OPEN_DEVICE;
	}
	// ���b�Z�[�W�̎��ʎq���擾����B
	if ((m_iMsgQueueID = msgget(key, 0666)) < 0) {
		m_iHandle = -1;
		return XMsgQueueERR_OPEN_DEVICE;
	}
#endif
	return XMsgQueueERR_SUCCESS;
XEXCEPT_EXIT
}
XMsgQueueERR XIPCQueuePusher::close()
{
XEXCEPT_ENTRY
	XMutexer locker( m_lock );
	if( m_iHandle != -1 ) {		
#ifdef WIN32
		HANDLE h = (HANDLE)m_iHandle;
		m_iHandle = -1;
		::CloseHandle(h);
#else
		int h = m_iHandle;
		m_iHandle = -1;
		::close(h);
#endif
	}
	return XMsgQueueERR_SUCCESS;
XEXCEPT_EXIT
}
XMsgQueueERR XIPCQueuePusher::push(XD_UINT4 ulMsgID, void *pData, size_t nDataSize)
{
XEXCEPT_ENTRY
	XMutexer locker( m_lock );
	if( m_iHandle == -1 ) {
		return XMsgQueueERR_NOT_OPEND;
	}
#ifdef WIN32
	// ���M�ς݃f�[�^�̈� 
	unsigned long ulTransmittedDataSize = 0;
	HANDLE h = (HANDLE)m_iHandle;

	// ���b�Z�[�WID���𑗐M����
	if( ::WriteFile(h, &ulMsgID, sizeof(XD_UINT4), &ulTransmittedDataSize, (LPOVERLAPPED)NULL) == 0) {
		return XMsgQueueERR_WRITE;
	}
	// �f�[�^�𑗐M����
	if( ::WriteFile(h, pData, nDataSize, &ulTransmittedDataSize, (LPOVERLAPPED)NULL) == 0) {
		return XMsgQueueERR_WRITE;
	}
#else

#endif
	return XMsgQueueERR_SUCCESS;
XEXCEPT_EXIT
}



//
//	XMsgQueueClient
//
XMsgQueueClient::XMsgQueueClient()
	:	m_id( 0 )
	,	m_ulSequenceNo( 0 )
{
XEXCEPT_ENTRY
	m_puller.connectPopCallBack(_popCallBack, this);
XEXCEPT_EXIT
}
XMsgQueueClient::~XMsgQueueClient()
{
XEXCEPT_ENTRY
	close();
XEXCEPT_EXIT
}
XMsgQueueERR XMsgQueueClient::open(const char* pName)
{
XEXCEPT_ENTRY
	return XMsgQueueClient::open((const wchar_t*)XStrBuff(pName));
XEXCEPT_EXIT
}
XMsgQueueERR XMsgQueueClient::open(const wchar_t* pName)
{
	XMsgQueueERR		status;
XEXCEPT_ENTRY
	XStrBuff			strMyname(pName);
	TCHAR				szUniq[256] = {0};
	unsigned long		pUniqID = (unsigned long)this;
	struct XMsgQueueCS	data;
	ClinentID			clientID_Uni;
	XD_UINT4			ulMsgID;

	close();

	// �N���C�A���gID���쐬
	clientID_Uni.m_id8 = (XD_UINT4)XProccess::getpid();
	clientID_Uni.m_id8 <<= 32;
	clientID_Uni.m_id8 |= (XD_UINT4)pUniqID;
	// Connect���b�Z�[�W
	data.type = XMsgQueueCS_CLINET_CONNECT;
	memcpy(data.clientid, &clientID_Uni.m_id8, 8);

	// Receive���I�[�v��
	XStrBuff::nprintf(szUniq, XCOUNTOF(szUniq), sizeof(szUniq), _T("_%08X_%08X"), 
		clientID_Uni.m_id4[0], clientID_Uni.m_id4[1]);
	strMyname << szUniq;
	status = m_puller.open((const wchar_t*)strMyname);
	if( status != XMsgQueueERR_SUCCESS ) {
		close();
		return status;
	}
	// PeerServer���I�[�v��
	status = m_pusher.open(pName);
	if( status != XMsgQueueERR_SUCCESS ) {
		close();
		return status;
	}

	{
		// ���b�Z�[�WID���̔�
		XMutexer locker( m_lockSequence );	
		++m_ulSequenceNo;
		ulMsgID = m_ulSequenceNo;
	}

	{
		XMutexer locker( m_lockpop );	
		m_popIDs.add(ulMsgID, 0);		//	���b�Z�[�W�h�c��o�^
		// �ڑ��v���𑗐M
		XTRACE(_T("  Open    Client :%08X_%08X\n"), clientID_Uni.m_id4[0], clientID_Uni.m_id4[1]);
		status = m_pusher.push(ulMsgID, &data, XMsgQueueCS_SIZE);
		if( status != XMsgQueueERR_SUCCESS ) {
			m_popIDs.del(ulMsgID);
			close();
			return status;
		}
	}
	// �ڑ����ʎ�M
	XQueueMsg *pMsg = m_puller.pop(ulMsgID, 10*1000);
	{
		XMutexer locker( m_lockpop );	
		m_popIDs.del(ulMsgID);		// �o�^����
	}
	if( pMsg == NULL ) {
		// �^�C���A�E�g
		close();
		return XMsgQueueERR_RCV_TIMEO;
	}
	struct XMsgQueueCS *pCS = (struct XMsgQueueCS *)pMsg->getData();
	if( pCS->type != XMsgQueueCS_CLINET_CONNECT_OK ) {
		XTRACE(_T("  Client Connect Failure(%d)\n"), pCS->type);

		status = XMsgQueueERR_NAK;

		close();
	} else {
		XTRACE(_T("  Client Connect Success\n"));


		status = XMsgQueueERR_SUCCESS;

		m_id = clientID_Uni.m_id8;
	}
	pMsg->deleteMe();
XEXCEPT_EXIT

	return status;
}
XMsgQueueERR XMsgQueueClient::close()
{
XEXCEPT_ENTRY
	XD_UINT8 id = m_id;
	m_id = 0;
	if( id != 0 ) {
		// Disconnect �𑗐M�B
		struct XMsgQueueCS data;
		data.type = XMsgQueueCS_CLINET_DISCONNECT;
		memcpy(data.clientid, &id, 8);
		m_pusher.push(0, &data, sizeof(data));
	}
	m_puller.close();
	m_pusher.close();
XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}
XMsgQueueERR XMsgQueueClient::sendRequset(void *pData, size_t nDataSize,
		XBuff& dataBuff, size_t *pDataSize, 
		XD_UINT4 ulTimeOut /*= XIPCQueuePuller::TIMEOUT_NON*/)
{
	XMsgQueueERR status;
XEXCEPT_ENTRY

	XD_UINT4	ulRespSize;
	XD_UINT4	ulMsgID = 0;
	XBuff buff;
	char *pTmp = NULL;
	XMsgQueueCS* pCSData = (XMsgQueueCS*)buff.Alloc(XMsgQueueCS_SIZE+nDataSize);
	
	// ���M�f�[�^�̏���
	pCSData->type = XMsgQueueCS_REQUEST;
	memcpy(pCSData->clientid, &m_id, 8);
	pTmp = (char*)pCSData;
	memcpy(&pTmp[XMsgQueueCS_SIZE], pData, nDataSize);

	{
		// ���b�Z�[�WID���̔�
		XMutexer locker( m_lockSequence );	
		++m_ulSequenceNo;
		ulMsgID = m_ulSequenceNo;
	}




	// ���N�G�X�g���M
	{
		XMutexer locker( m_lockpop );	
		m_popIDs.add(ulMsgID, 0);		//	���b�Z�[�W�h�c��o�^

		status = m_pusher.push(ulMsgID, pCSData, XMsgQueueCS_SIZE+nDataSize);
		if( status != XMsgQueueERR_SUCCESS ) {
			m_popIDs.del(ulMsgID);
			return status;
		}
	}


	// ���X�|���X��M
	XQueueMsg *pMsg = m_puller.pop(ulMsgID, ulTimeOut);
	{
		XMutexer locker( m_lockpop );	
		m_popIDs.del(ulMsgID);		// �o�^����
	}
	if( pMsg == NULL ) {
		// �^�C���A�E�g
		return XMsgQueueERR_RCV_TIMEO;
	}
	if( pMsg->getDataSize() < XMsgQueueCS_SIZE ) {
		return XMsgQueueERR_RCV_DATA_BROKEN;
	}

	ulRespSize = pMsg->getDataSize() - XMsgQueueCS_SIZE;
	dataBuff.Alloc(ulRespSize);
	if( pDataSize != NULL ) {
		*pDataSize = ulRespSize;
	}
	pTmp = (char*)pMsg->getData();
	memcpy(dataBuff, &pTmp[XMsgQueueCS_SIZE], ulRespSize);
	pMsg->deleteMe();
XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}

XMsgQueueERR XMsgQueueClient::pop(XBuff& dataBuff, 
								  size_t *pDataSize, 
								  XD_UINT4 ulMsgID /*= XQueue_MsgID_ANY*/, 
								  XD_UINT4 ulTimeout /*= XQueue_TIMEOUT_FOREVER*/)
{
XEXCEPT_ENTRY
	XD_UINT4	ulRespSize;
	char *pTmp = NULL;

	if( ulMsgID != XQueue_MsgID_ANY ) {
		XMutexer locker( m_lockpop );	
		m_popIDs.add(ulMsgID, 0);		//	���b�Z�[�W�h�c��o�^
	}
	// ���X�|���X��M
	XQueueMsg *pMsg = m_puller.pop(ulMsgID, ulTimeout);
	{
		XMutexer locker( m_lockpop );	
		m_popIDs.del(ulMsgID);		// �o�^����
	}
	if( pMsg == NULL ) {
		// �^�C���A�E�g
		return XMsgQueueERR_RCV_TIMEO;
	}
	if( pMsg->getDataSize() < XMsgQueueCS_SIZE ) {
		return XMsgQueueERR_RCV_DATA_BROKEN;
	}

	ulRespSize = pMsg->getDataSize() - XMsgQueueCS_SIZE;
	dataBuff.Alloc(ulRespSize);
	if( pDataSize != NULL ) {
		*pDataSize = ulRespSize;
	}
	pTmp = (char*)pMsg->getData();
	memcpy(dataBuff, &pTmp[XMsgQueueCS_SIZE], ulRespSize);
	pMsg->deleteMe();
XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}

int XMsgQueueClient::_popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize, void* pUserData)
{
XEXCEPT_ENTRY
	return ((XMsgQueueClient*)pUserData)->_popCallBack(ulMsgID, pData, ulDataSize);
XEXCEPT_EXIT
}

int XMsgQueueClient::_popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize)
{
XEXCEPT_ENTRY
	ClinentID			clientID_Uni;
	{
		XMutexer locker( m_lockpop );	
		if( m_popIDs.isExist(ulMsgID) == true ) {
			// ���N�G�X�g���������b�Z�[�W
			return XQueue_POP_CALLBCK_IGNORE;
		}
	}
	if( ulDataSize < XMsgQueueCS_SIZE ) {
		return XQueue_POP_CALLBCK_IGNORE;
	}

	clientID_Uni.m_id8 = *((XD_UINT8*)&pData[XMsgQueueCS_POS_CLINET_ID]);

#ifdef _DEBUG
	TCHAR szData[256] = _T("");
	if( ulDataSize > 10 ) {
		XStrBuff::printf(szData, XCOUNTOF(szData), _T("%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X..."),
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 0], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 1], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 2], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 3], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 4], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 5], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 6], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 7], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 8], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 9], 
			(XD_UINT1)pData[XMsgQueueCS_SIZE + 10]
			);
	}
	XTRACE(_T("  - XMsgQueueClient::_popCallBack :[%08X_%08X] MsgID:%u %s\n"), clientID_Uni.m_id4[0], clientID_Uni.m_id4[1], ulMsgID, szData);
#endif
	return popCallBack(ulMsgID, &pData[XMsgQueueCS_SIZE], ulDataSize-XMsgQueueCS_SIZE );
XEXCEPT_EXIT
	return XQueue_POP_CALLBCK_IGNORE;
}

int XMsgQueueClient::popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize)
{
XEXCEPT_ENTRY
	return XQueue_POP_CALLBCK_IGNORE;
XEXCEPT_EXIT
}

//
//	XMsgQueueServer
//
XMsgQueueServer::XMsgQueueServer()
	:	m_bRunning( false )
	,	m_ulSequenceNo( 0 )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XMsgQueueServer::~XMsgQueueServer()
{
XEXCEPT_ENTRY
	close();
XEXCEPT_EXIT
}
XMsgQueueERR XMsgQueueServer::open(const char* pName)
{
XEXCEPT_ENTRY
	XMsgQueueERR status;

	close();

	status = m_puller.open(pName);
	if( status != XMsgQueueERR_SUCCESS ) {
		return status;
	}
	m_strName = pName;

XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}
XMsgQueueERR XMsgQueueServer::open(const wchar_t* pName)
{
XEXCEPT_ENTRY
	XMsgQueueERR status;

	close();

	status = m_puller.open(pName);
	if( status != XMsgQueueERR_SUCCESS ) {
		return status;
	}
	m_strName = pName;

XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}
XMsgQueueERR XMsgQueueServer::close()
{
XEXCEPT_ENTRY
	bool b = m_bRunning;
	m_bRunning = false;
	if( b == true ) {
		struct XMsgQueueCS data;
		data.type = XMsgQueueCS_CLINET_TERMINATE;
		m_puller.m_q.push(0, (char*)&data, sizeof(data));
	}
	return	m_puller.close();
XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}
void XMsgQueueServer::startMainLoopThread()
{
XEXCEPT_ENTRY
	m_bRunning = true;
	StartUndetached();
XEXCEPT_EXIT
}

XMsgQueueERR XMsgQueueServer::sendEvent(void *pData, size_t dataSize, XD_UINT8 uClientID /*= (XD_UINT8)(-1)*/)
{
XEXCEPT_ENTRY
	XMutexer locker( m_lockClinets );
	XD_UINT4	ulMsgID = 0;
	XBuff buff;
	XMsgQueueCS* pCSData = (XMsgQueueCS*)buff.Alloc(XMsgQueueCS_SIZE+dataSize);

	// ���M���b�Z�[�W�̏���
	memset(pCSData, 0, XMsgQueueCS_SIZE);
	pCSData->type = XMsgQueueCS_EVENT;
	char *pTmp = (char*)pCSData;
	memcpy(&pTmp[XMsgQueueCS_SIZE], pData, dataSize);

	{
		// ���b�Z�[�WID���̔�
		XMutexer locker( m_lockSequence );	
		++m_ulSequenceNo;
		ulMsgID = m_ulSequenceNo;
	}


	XPOSITION pos1, pos2;
	for( pos1 = m_clients.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		m_clients.GetNext( pos1 );
		ClinetData* pa = (ClinetData*)m_clients.GetAt( pos2 );
		
		pa->m_pusher.push(ulMsgID, pTmp, XMsgQueueCS_SIZE+dataSize);
	}
XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}

int XMsgQueueServer::getClientsCount()
{
XEXCEPT_ENTRY
	XMutexer locker( m_lockClinets );
	return m_clients.GetCount();
XEXCEPT_EXIT
}

XD_UINT8 XMsgQueueServer::getClientID(int index)
{
XEXCEPT_ENTRY
	XMutexer locker( m_lockClinets );
	XPOSITION pos = m_clients.FindIndex( index );
	if( pos == NULL ) {
		return 0;
	}
	return ((ClinetData*)m_clients.GetAt(pos))->getClinetID();
XEXCEPT_EXIT
}

void*  XMsgQueueServer::RunUndetached(void* pArg)
{
XEXCEPT_ENTRY

	while( m_bRunning == true ) {
		XQueueMsg* pMsg = m_puller.pop();
		if( pMsg == NULL ) {
			// �����ɑ҂�ԂȂ̂ŁANULL�ŋA���Ă��邱�Ƃ́A
			// �f�o�C�X������ꂽ���
			break;
		}

		char* pTmp = (char*)pMsg->getData();
		struct XMsgQueueCS *pCS = (struct XMsgQueueCS *)pTmp;

		if( pCS->type == XMsgQueueCS_CLINET_TERMINATE ) {
			// �I������...
			pMsg->deleteMe();
			break;
		} else
		if( pCS->type == XMsgQueueCS_CLINET_CONNECT ) {
			//
			// �N���C�A���g���ڑ����Ă���
			//
			XMsgQueueClient::ClinentID	clientID_Uni;
			clientID_Uni.m_id8 = *((XD_UINT8*)&pTmp[XMsgQueueCS_POS_CLINET_ID]);
			XTRACE(_T("  Connect Client :[%08X_%08X] MsgID:%u\n"), clientID_Uni.m_id4[0], clientID_Uni.m_id4[1], pMsg->getMsgID());
			

			XStrBuff strMyname(m_strName);
			TCHAR	 szUniq[256];
			// Receive���I�[�v��
			XStrBuff::nprintf(szUniq, XCOUNTOF(szUniq), sizeof(szUniq), _T("_%08X_%08X"), clientID_Uni.m_id4[0], clientID_Uni.m_id4[1]);
			strMyname << szUniq;
			ClinetData *pClient = new ClinetData(clientID_Uni.m_id8 );
			if( pClient->m_pusher.open((const wchar_t*)strMyname) != XMsgQueueERR_SUCCESS ) {
				delete pClient;
				pMsg->deleteMe();
				continue;
			}
			{
				// �N���C�A���g���X�g�ɒǉ�
				XMutexer locker( m_lockClinets );
				m_clients.AddTail(pClient);
			}
			{
				// �ڑ�������ʒm
				struct XMsgQueueCS conOK;
				conOK.type = XMsgQueueCS_CLINET_CONNECT_OK;
				pClient->m_pusher.push(pMsg->getMsgID(), &conOK, sizeof(conOK));

				ntfyConnection( pClient->getClinetID(), true );
			}
		}else
		if( pCS->type == XMsgQueueCS_CLINET_DISCONNECT ) {
			//
			//	�N���C�A���g���ؒf����
			//
			XMsgQueueClient::ClinentID	clientID_Uni;
			clientID_Uni.m_id8 = *((XD_UINT8*)&pTmp[XMsgQueueCS_POS_CLINET_ID]);
			XTRACE(_T("  Disconnect Client :[%08X_%08X] MsgID:%u\n"), clientID_Uni.m_id4[0], clientID_Uni.m_id4[1], pMsg->getMsgID());

			XMutexer locker( m_lockClinets );	
			XPOSITION pos1, pos2;
			for( pos1 = m_clients.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
				if( ((ClinetData*)m_clients.GetNext( pos1 ))->m_clientID == clientID_Uni.m_id8  ) {
					ClinetData* pa = (ClinetData*)m_clients.GetAt( pos2 );
					ntfyConnection( pa->getClinetID(), false );
					m_clients.RemoveAt( pos2 );
					delete pa;
					break;
				}
			}
		} else
		if( pCS->type == XMsgQueueCS_REQUEST ) {
			//
			//	�N���C�A���g�����N�G�X�g�𑗂��Ă���
			//
			XMsgQueueClient::ClinentID	clientID_Uni;
			clientID_Uni.m_id8 = *((XD_UINT8*)&pTmp[XMsgQueueCS_POS_CLINET_ID]);
			XTRACE(_T("  Receive Request :[%08X_%08X] MsgID:%u\n"), clientID_Uni.m_id4[0], clientID_Uni.m_id4[1], pMsg->getMsgID());

			XMutexer locker( m_lockClinets );	
			bool bFindClient = false;
			XPOSITION pos1, pos2;
			for( pos1 = m_clients.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {

				if( ((ClinetData*)m_clients.GetNext( pos1 ))->m_clientID == clientID_Uni.m_id8  ) {
					ClinetData* pa = (ClinetData*)m_clients.GetAt( pos2 );
					bFindClient = true;

					receiveRequest(pa, 
						pMsg->getMsgID(), 
						&pTmp[XMsgQueueCS_SIZE], 
						pMsg->getDataSize() - XMsgQueueCS_SIZE);
				}
			}

			if( bFindClient == false ) {
				XTRACE(_T("  Lost ClientID :[%08X_%08X] MsgID:%u\n"), clientID_Uni.m_id4[0], clientID_Uni.m_id4[1], pMsg->getMsgID());	
			}
		}
		pMsg->deleteMe();
	}
	return NULL;
XEXCEPT_EXIT
	return 0;
}


XMsgQueueERR XMsgQueueServer::ClinetData::sendResponse(XD_UINT4 ulMsgID, const void *pData, size_t dataSize)
{
XEXCEPT_ENTRY
	XBuff buff;
	XMsgQueueCS* pCSData = (XMsgQueueCS*)buff.Alloc(XMsgQueueCS_SIZE+dataSize);

	pCSData->type = XMsgQueueCS_RESPONSE;
	memcpy(pCSData->clientid, &m_clientID, 8);
	char *pTmp = (char*)pCSData;
	memcpy(&pTmp[XMsgQueueCS_SIZE], pData, dataSize);
#ifdef _DEBUG
	XMsgQueueClient::ClinentID clientID_Uni;
	clientID_Uni.m_id8 = m_clientID;
	XTRACE(_T("  Send Response :[%08X_%08X] MsgID:%u\n"), clientID_Uni.m_id4[0], clientID_Uni.m_id4[1], ulMsgID);
#endif /*_DEBUG*/
	return m_pusher.push(ulMsgID, pCSData, XMsgQueueCS_SIZE+dataSize);
XEXCEPT_EXIT
	return XMsgQueueERR_SUCCESS;
}