#include "xlib/xdeamon.h"
#include "xlib/xthread.h"
#include "xlib/xbuff.h"
#ifndef WIN32
  #include <strings.h>
  #include <signal.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////
//
//	gloabal value initialize
//
XDEAMON_TABLE_ENTRY* XDeamon::g_pDemonTableEntry  = NULL;
XDeamon*			 XDeamon::m_pGlobalDemon = NULL;
//////////////////////////////////////////////////////////////////////////////////////
//
//	Local Defined
//
/**
 * �f�[�����R���|�[�l���g�X���b�h�Ǘ��f�[�^�[
 */
typedef struct _XDemonMgrData
{
	/** �Z�}�t�H */
	XSemaphore		*m_pSemaphore;
	/** �ғ����̃R���|�[�l���g�� */
	unsigned long	*m_pNumPomo;
	/** �ғ����̃R���|�[�l���g�����f�N�������g���邽�߂̃��b�N�I�u�W�F�N�g */
	XLock			*m_pLock;
	/** �X�^�[�g�A�b�v��� */
	XDeamon::STARTUPINFO pStartUpInfo;
}XDemonMgrData;

void* XDemonMgrThread(void *pData)
{
	XDemonMgrData *pMgrData = (XDemonMgrData*)pData;
	

	pMgrData->pStartUpInfo.lpDeamonMain(pMgrData->pStartUpInfo.argc, pMgrData->pStartUpInfo.argv);

	
	XLocker locker( *pMgrData->m_pLock );
	int iData = *pMgrData->m_pNumPomo;
	
	iData--;

	*pMgrData->m_pNumPomo = iData;
	pMgrData->m_pSemaphore->Post();
	return 0;
}
#ifdef WIN32
BOOL WINAPI __XDEMON_HandlerRoutine(DWORD dwCtrlType)
{
	_XASSERT( XDeamon::m_pGlobalDemon );
	XDeamon::m_pGlobalDemon->SendBroadCastSignal(dwCtrlType);
	return TRUE;
}
#else /* POSIX UNIX */
void __XDEMON_SIG_TERM(int signo)
{
	_XASSERT( XDeamon::m_pGlobalDemon );
	XDeamon::m_pGlobalDemon->SendBroadCastSignal(signo);
}
void __XDEMON_SIG_HUP(int signo)
{
	_XASSERT( XDeamon::m_pGlobalDemon );
	XDeamon::m_pGlobalDemon->SendBroadCastSignal(signo);
}
void __XDEMON_SIG_CHILED(int signo)
{
	_XASSERT( XDeamon::m_pGlobalDemon );
	XDeamon::m_pGlobalDemon->SendBroadCastSignal(signo);
}
void* __XDEMON_Signal(int signo, void (*func)(int))
{
    struct sigaction    act;
    struct sigaction    oact;

	::memset(&act, 0, sizeof(act));
	::memset(&oact, 0, sizeof(oact));

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if( sigaction(signo, &act, &oact) < 0 ){
        return (void*)SIG_ERR;
    }
    return (void*)oact.sa_handler;
}
#endif

//////////////////////////////////////////////////////////////////////////////////////
//
//	XDeamon
//
XDeamon::XDeamon()
{
	m_bDebug = false;
	m_nRunningCompornent = 0;

	m_pGlobalDemon = this;
}
XDeamon::~XDeamon()
{
	m_svcCompornent.RemoveAll();
}
XDeamonCompnent *XDeamon::GetComponent(int nIndes)  const
{
	return m_svcCompornent[nIndes];
}
int XDeamon::GetComponentCount()  const
{
	return  m_svcCompornent.GetSize();
}
void XDeamon::Add(XDeamonCompnent *pConponent)
{
	pConponent->m_pParent = this;
	m_svcCompornent.Add(pConponent);
}
bool XDeamon::Start(int argc, TCHAR *argv[], bool bDebug /*= false*/)
{
	unsigned long dwDeamonCount = 0;
	XDEAMON_TABLE_ENTRY *pTbl = g_pDemonTableEntry;
	bool bRetcode = false;

	m_bDebug = bDebug;

	// �f�[�����̐����J�E���g
	while(1) {
		if(pTbl->lpDeamonName == NULL)
			break;
		dwDeamonCount++;
		pTbl++;
	}

#ifdef WIN32
	OSVERSIONINFO OsVersion;
	OsVersion.dwOSVersionInfoSize = sizeof (OsVersion);

	::GetVersionEx(&OsVersion);
	BOOL bIsNT = (OsVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT);
	if( bIsNT == TRUE && m_bDebug == false ) {
		bRetcode = 
			::StartServiceCtrlDispatcher((SERVICE_TABLE_ENTRY*)g_pDemonTableEntry) == TRUE ? true : false;
	} else {
#endif // WIN32
		XLock		lock;
		unsigned long  i;
		XDemonMgrData *pThreadData = new XDemonMgrData[ dwDeamonCount ];

		pTbl = g_pDemonTableEntry;

		if( pThreadData == NULL ) {
			return false;
		}

		for( i = 0; i < dwDeamonCount; i++) {
			
			m_nRunningCompornent++;

			pThreadData[i].m_pSemaphore					= &m_semapho;
			pThreadData[i].m_pNumPomo					= &m_nRunningCompornent;
			pThreadData[i].m_pLock						= &lock;
			pThreadData[i].pStartUpInfo.lpDeamonMain	= pTbl[i].lpDeamonProc;
			pThreadData[i].pStartUpInfo.argc			= argc;
			pThreadData[i].pStartUpInfo.argv			= argv;

			XThread *pThread = XThread::Create(XDemonMgrThread, &pThreadData[i]);
		}

		if( m_bDebug == false ) {
			for(;;) {
				m_semapho.Wait();
				if( m_nRunningCompornent == 0 ) {
					break;
				}
			}
		} else {
			DebugCallBak();
		}
		delete pThreadData;
#ifdef WIN32
	}
#endif // WIN32
	return true;
}
void XDeamon::SendBroadCastSignal(unsigned long signo)
{
	for( int i = 0; i < GetComponentCount() ; i++ ) {
		XDeamonCompnent *p = GetComponent( i );
		p->m_lpHandler(signo);
	}
	return ;

}
void XDeamon::DebugCallBak()
{
	char sz[512];
	int i;

	for(;;) {
		// �R���|�[�l���g�X�e�[�^�X�ꗗ�\��
		printf("Deamon Compornents Status\n");
		for( i = 0; i < m_svcCompornent.GetSize(); i++) {
			XDeamonCompnent *pComp = m_svcCompornent[i];
			XStrBuff strCompName = pComp->GetCompornentName(); 

			printf("  %d- %s : ", i+1, (const char*)strCompName);
			switch( pComp->GetStatus() ) {
			case XDEMON_STS_STOPPED:
				printf("STOP");
				break;
			case XDEMON_STS_START_PENDING:
				printf("START Pending...");
				break;
			case XDEMON_STS_STOP_PENDING:
				printf("STOP Pending...");
				break;
			case XDEMON_STS_RUNNING:
				printf("RUNNING");
				break;
			case XDEMON_STS_CONTINUE_PENDING:
				printf("CONTINUE Pending...");
				break;
			case XDEMON_STS_PAUSE_PENDING:
				printf("PAUSE Pending...");
				break;
			case XDEMON_STS_PAUSED:
				printf("PAUSED");
				break;
			default:
				printf("????");
				break;
			}
			printf("\n");
		}

		printf("Please Enter Command\n"
			   "  stop:\n");
		char *pp;
		::fgets(sz, sizeof(sz), stdin);
		pp = ::strrchr(sz, '\n');
		if( pp != NULL ) { *pp = 0;}


		if( ::strcmp(sz, "stop") == 0 ) {
			// �w�肵���R���|�[�l���g���~
			printf("plase enter compornent number:");
			::fgets(sz, sizeof(sz), stdin);
			pp = ::strrchr(sz, '\n');
			if( pp != NULL ) { *pp = 0;}

			int iCmp = ::atoi(sz);
			if( iCmp > m_svcCompornent.GetSize() || iCmp == 0 ) {
				fprintf(stderr, "invalid compornent numver - %d\n", iCmp);
			} else {
				XDeamonCompnent *pComp = m_svcCompornent[iCmp - 1];
				pComp->Stop();
			}
		}

		if( ::strcmp(sz, "exit") == 0 ) {
			// �I��
			for( i = 0; i < m_svcCompornent.GetSize(); i++) {
				XDeamonCompnent *pComp = m_svcCompornent[i];
				XStrBuff strCompName = pComp->GetCompornentName(); 

				switch( pComp->GetStatus() ) {
				case XDEMON_STS_RUNNING:
					// �I�����Ă��Ȃ��R���|�[�l���g���~
					pComp->Stop();
					break;
				}
			}


			for(;;) {
				// �S�R���|�[�l���g���I������܂Ń��[�v
				XThread::Sleep(1);
				bool bAllStop = true;
				for( i = 0; i < m_svcCompornent.GetSize(); i++) {
					XDeamonCompnent *pComp = m_svcCompornent[i];
					XStrBuff strCompName = pComp->GetCompornentName(); 

					if( pComp->GetStatus() != XDEMON_STS_STOPPED ) {
						bAllStop = false;
					}
				}
				if( bAllStop == true ) {
					return ;
				}
			}

		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	XDeamonCompnent
//
XDeamonCompnent::XDeamonCompnent(
				unsigned long dwStartWaitHint /*= 5000*/, 
				unsigned long dwStopWaitHint /*= 15000*/)
	:	m_pParent(NULL)
{
#ifdef WIN32
	m_DeamonStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_hSCHandle					  = NULL;
#endif
	m_dwStartWaitHint	= dwStartWaitHint;
	m_dwStopWaitHint    = dwStopWaitHint;
	m_lpHandler         = NULL;
	m_nStatus			= XDEMON_STS_STOPPED;
}
XDeamonCompnent::~XDeamonCompnent()
{

}
void XDeamonCompnent::Initialize(const TCHAR* lpszDeamonName, XDEAMON_FUNCTION pHandler)
{
	m_lpszDeamonName	= lpszDeamonName;
	m_lpHandler		= pHandler;
}
bool XDeamonCompnent::Stop(void)
{
	this->ReportStatusChange(XDEMON_STS_STOP_PENDING, m_dwStopWaitHint);
	bool bRet = StopImple();
	m_SvcWait.Post();
	return bRet;
}
bool XDeamonCompnent::Pause(void)
{
	this->ReportStatusChange(XDEMON_STS_PAUSE_PENDING, m_dwStopWaitHint);
	bool bRet = PauseImple();
	if( bRet == true ) {
		this->ReportStatusChange(XDEMON_STS_PAUSED, 0);
	}
	return bRet;
}
bool XDeamonCompnent::Continue(void)
{
	this->ReportStatusChange(XDEMON_STS_CONTINUE_PENDING, m_dwStartWaitHint);
	bool bRet = ContinueImple();
	if( bRet == true ) {
		this->ReportStatusChange(XDEMON_STS_RUNNING, 0);
	}
	return bRet;
}
bool XDeamonCompnent::StopImple(void)
{
	return true;
}
bool XDeamonCompnent::PauseImple(void)
{
	return true;
}
bool XDeamonCompnent::ContinueImple(void)
{
	return true;
}
//unsigned long XDeamonCompnent::UserHandler(unsigned long cmd)
//{
//	return 0;
//}
void XDeamonCompnent::Main(unsigned long dwArgc, TCHAR* *lpszArgv)
{
	// �f�[�����G���g���[��o�^
	if( this->RegisterHandler() == false ) {
		this->Cleanup();
		return ;
	}
	// �J�n���Ă���r�����
	if( this->ReportStatusChange(XDEMON_STS_START_PENDING, m_dwStartWaitHint) == false) {
		this->Cleanup();
		return ;
	}
	// �R���|�[�l���g���Z�b�g�A�b�v
	if( this->Setup(dwArgc, lpszArgv) == true ) {
		// ���s�����
		this->ReportStatusChange(XDEMON_STS_RUNNING);
		m_SvcWait.Wait();
//		this->Wait();
		m_SvcWait.Wait();
	} else {
		this->ErrorReport(XDEMON_ERROR_SETUP, 0);
	}
	// �㏈��
	this->Cleanup();
	// ���s�����
	this->ReportStatusChange(XDEMON_STS_STOPPED, 0);
}
void XDeamonCompnent::CtrlHandler(unsigned long dwCommand)
{
#ifdef WIN32
	if( dwCommand == CTRL_SHUTDOWN_EVENT ||
		dwCommand == CTRL_C_EVENT        ||
		dwCommand == CTRL_BREAK_EVENT    ||
		dwCommand == CTRL_CLOSE_EVENT    ) {
		this->Stop();
	}
#else
	if( dwCommand == SIGTERM ) {
		this->Stop();
	}
#endif
}
void XDeamonCompnent::ErrorReport(XDEMON_ERROR nErrorNo, unsigned long nOsErrorNo)
{

}

bool XDeamonCompnent::RegisterHandler()
{
#ifdef WIN32
	if( m_pParent->IsDebugMode() == false ) {
		OSVERSIONINFO OsVersion;
		OsVersion.dwOSVersionInfoSize = sizeof (OsVersion);

		::GetVersionEx(&OsVersion);

		BOOL bIsNT = (OsVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT);
		if( bIsNT == TRUE ) {
			m_hSCHandle = (SC_HANDLE)::RegisterServiceCtrlHandler( m_lpszDeamonName,  m_lpHandler);
			if( m_hSCHandle == NULL ) {
				this->ErrorReport(XDEMON_ERROR_REGIST, ::GetLastError());
			}
			return m_hSCHandle != NULL ? true : false;
		}
	}
	if( ::SetConsoleCtrlHandler(__XDEMON_HandlerRoutine, TRUE) == FALSE ) {
		this->ErrorReport(XDEMON_ERROR_REGIST, ::GetLastError());
		return false;
	}
	return true;
#else
	if( __XDEMON_Signal(SIGCHLD, __XDEMON_SIG_CHILED) == SIG_ERR ) {
		this->ErrorReport(XDEMON_ERROR_REGIST, errno);
		return false;
	}
	if( __XDEMON_Signal(SIGTERM, __XDEMON_SIG_TERM) == SIG_ERR ) {
		this->ErrorReport(XDEMON_ERROR_REGIST, errno);
		return false;
	}
	if( __XDEMON_Signal(SIGHUP, __XDEMON_SIG_HUP) == SIG_ERR ) {
		this->ErrorReport(XDEMON_ERROR_REGIST, errno);
		return false;
	}
	return true;
#endif
}

bool XDeamonCompnent::ReportStatusChange(
				 unsigned long dwCurrentState,
				 unsigned long dwWaitHint /*     = 5000*/,
				 unsigned long dwSysExitCode/* = NO_ERROR*/,
				 unsigned long dwProcessSpecificExitCode/* = 0*/)
{
	m_nStatus = (XDEMON_STS)dwCurrentState;
#ifdef WIN32
	// �X�e�[�^�X
	m_DeamonStatus.dwCurrentState    = dwCurrentState;

	//	dwControlsAccepted �󂯕t���鐧��R�}���h�̎��
	//	(�ꎞ��~  �ĊJ  �I��)
	switch( dwCurrentState ) {
	case XDEMON_STS_RUNNING:
		m_DeamonStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
		break;
	default:
		m_DeamonStatus.dwControlsAccepted = 0;
		break;
	}

	// dwWin32ExitCode �� dwServiceSpecificExitCode �͊J�n���ƏI�����ɔ��������G���[��ʒm 
	// GetLastError WnetGetlastError()�Ȃǂ�ݒ肷��	
	// �A�v���P�[�V�����G���[�̏ꍇ�� dwWin32ExitCode �� ERROR_SERVICE_SPECIFIC_ERROR��ݒ肵��
	// dwServiceSpecificExitCode�ɃG���[�R�[�h��ݒ肷��
	m_DeamonStatus.dwWin32ExitCode   = dwSysExitCode;
	m_DeamonStatus.dwServiceSpecificExitCode = dwProcessSpecificExitCode;

	switch (dwCurrentState) {
	  case SERVICE_START_PENDING:
	  case SERVICE_STOP_PENDING:
	  case SERVICE_PAUSE_PENDING:
	  case SERVICE_CONTINUE_PENDING:
		 // dwCheckPoint ��  dwWaitHint���g������I�ɊJ�n���ƏI�����ɔ��������G���[��ʒm���� 
		 // dwWaitHint �͑���ɕK�v���Ɨ\������鎞�ԁi�~���b�Ŏw�肳���j
		 // ���̎��Ԃ��o�߂���ƁA�T�[�r�X�}�l�[�W���[�́A�T�[�r�X�̏󋵂��m�F����
		 // ����܂ł� dwCheckPoint �̒l���X�V����Ȃ��ꍇ�T�[�r�X���ȏ�I���܂��͒��f�����Ƃ݂Ȃ����
		 m_DeamonStatus.dwCheckPoint = 1;
		 m_DeamonStatus.dwWaitHint   = dwWaitHint;
		 break;
	  default:
		 m_DeamonStatus.dwCheckPoint = 0;
		 m_DeamonStatus.dwWaitHint   = 0;
		 break;
	}
#endif
	bool bRetcode = SetStatus();
	return bRetcode;
}
bool XDeamonCompnent::SetPendingStatus(unsigned long dwCurrentState,
					  unsigned long dwWaitHint /*= 5000*/,
					  unsigned long dwCheckPoint /*= 1*/)
{
#ifdef WIN32
   m_DeamonStatus.dwCurrentState            = dwCurrentState;

   // dwControlsAccepted ��0���w�肷��Ə�Ԃ��ڍs���Ă��邢�Ƃ��ɐ���̗v���𖳌��ɂ���
   m_DeamonStatus.dwControlsAccepted        = 0;

   m_DeamonStatus.dwWin32ExitCode           = 0;
   m_DeamonStatus.dwServiceSpecificExitCode = 0;

   m_DeamonStatus.dwWaitHint   = dwWaitHint;
   m_DeamonStatus.dwCheckPoint = dwCheckPoint;
#endif
   bool bRetcode = SetStatus();

   return bRetcode;
}
bool XDeamonCompnent::UpdateStatus(void)
{
#ifdef WIN32
	if (m_DeamonStatus.dwCheckPoint > 0) {
		++m_DeamonStatus.dwCheckPoint;
	}
#endif
	bool bRetcode = SetStatus();
	return bRetcode;
}
bool XDeamonCompnent::SetStatus(void)
{
#ifdef WIN32
	bool bRetcode;

	OSVERSIONINFO OsVersion;
	OsVersion.dwOSVersionInfoSize = sizeof (OsVersion);

	::GetVersionEx(&OsVersion);

	BOOL bIsNT = (OsVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT);
	if( bIsNT ) {
		if( m_pParent->IsDebugMode() == false ) {
			bRetcode = (::SetServiceStatus((SERVICE_STATUS_HANDLE)m_hSCHandle, &m_DeamonStatus) == TRUE ? true : false );
			if( bRetcode == false ) {
				this->ErrorReport(XDEMON_ERROR_REPORT, ::GetLastError() );
			}
		} else {
			bRetcode = true;
		}
	} else {
		bRetcode = true;
	}
	return bRetcode;
#else
	return true;
#endif
}


