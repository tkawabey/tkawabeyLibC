
#ifdef WIN32

 #ifndef _WINDOWS_
  #include <windows.h>
 #endif //_WINDOWS_
 #ifndef _WINDEF_
  #include <windef.h>
 #endif //_WINDEF_
 #ifndef _WINERROR_
  #include <winerr.h>
 #endif //_WINERROR_

 #include <crtdbg.h>
 #include <tchar.h>
 #include <process.h>
#ifndef _MT
	#error('undefine MT. do point [menu]->[project]. and open project setting property, select C/C++ tab. set /MT on project options')
#endif

#else /* For POSIX */

  #include <stdlib.h>
  #include <string.h>
  #include <stdio.h>
  #include <stdarg.h>
  #include <assert.h>
  #include <errno.h>
  #include <ctype.h>

	#if (defined(__GLIBC__) && __GLIBC__ >= 2) || defined(__SCO_VERSION__) || defined(__aix__) || defined (__cygwin__)
      // typedef of struct timeval and gettimeofday();
      #include <sys/time.h>
      #include <unistd.h>
	#endif

	#if defined(__linux__) && defined(_MIT_POSIX_THREADS)
      #include <pthread/mit/sys/timers.h>
	#endif

	#if defined(__irix__) && defined(PthreadSupportThreadPriority)
	 #if _POSIX_THREAD_PRIORITY_SCHEDULING
	  #include <sched.h>
	 #endif
	#endif

     #ifdef __CYGWIN__
       #include <unistd.h>		// for sleep
       #include <time.h> 
       #include <sys/time.h>	// for struct timeval 
	  #ifndef pthread_yield
		void pthread_yield()
		{
			::sleep(0);
		}
	  #endif
	#endif



	#if (PthreadDraftVersion <= 6)
		#define ERRNO(x) (((x) != 0) ? (errno) : 0)
		#ifdef __VMS
		// pthread_setprio returns old priority on success (draft version 4:
		// OpenVms version < 7)
#define THROW_ERRORS_XTHREAD(x, y) { if ((x) == -1) XEXCEPT_THROW(XExcept_THREAD, errno, y);}
		#else
			#define THROW_ERRORS_XTHREAD(x, y) { if ((x) == -1) XEXCEPT_THROW(XExcept_THREAD, errno, y);}
		#endif
	#else
		#define ERRNO(x) (x)
		#define THROW_ERRORS_XTHREAD(x, y) { int rc = (x); \
				  if (rc != 0) XEXCEPT_THROW(XExcept_THREAD, rc, y);}
	#endif



#endif //WIN32

#include "xlib/xthread.h"

static XThread::InitT x_thread_init;

/**
 * スリープ値
 */
#define XTREAD_MAX_SLEEP_SECONDS (DWORD)4294966	// (2**32-2)/1000



#ifdef WIN32
///////////////////////////////////////////////////////////////////////////
//
// INNER CLASS DEFINED
//
class _InternalXthreadDummy : public XThread 
{
public:
	_InternalXthreadDummy();
	~_InternalXthreadDummy();
	friend class _InternalXthreadHelper;
private:
	_InternalXthreadDummy*     m_pNext;
};

class _InternalXthreadHelper 
{
public:  
	_InternalXthreadHelper();
	~_InternalXthreadHelper();
	operator XThread*() { return m_pThread; };
	XThread* operator->() { return m_pThread; };

	static _InternalXthreadDummy* m_pCache;
	static XLock                  m_CacheLock;
private:
	_InternalXthreadDummy*   m_pDummy;
	XThread*                 m_pThread;
};
///////////////////////////////////////////////////////////////////////////
//
// Global value initialize
//
_InternalXthreadDummy* _InternalXthreadHelper::m_pCache = 0;
XLock                  _InternalXthreadHelper::m_CacheLock;
int                    XThread::InitT::m_count = 0;
XLock*                 XThread::m_NextIdMutex = NULL;
int                    XThread::m_NextId = 0;
static DWORD          g_dwSelfTlsIndex;


///////////////////////////////////////////////////////////////////////////
//
// Static function
//
static void GetTimeNow(unsigned long* abs_sec, unsigned long* abs_nsec)
{
    static int days_in_preceding_months[12]
	= { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
    static int days_in_preceding_months_leap[12]
	= { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

    SYSTEMTIME st;

    ::GetSystemTime(&st);
    *abs_nsec = st.wMilliseconds * 1000000;

    DWORD days = ((st.wYear - 1970) * 365 + (st.wYear - 1969) / 4
		  + ((st.wYear % 4)
		     ? days_in_preceding_months[st.wMonth - 1]
		     : days_in_preceding_months_leap[st.wMonth - 1])
		  + st.wDay - 1);

    *abs_sec = st.wSecond + 60 * (st.wMinute + 60 * (st.wHour + 24 * days));
}
///////////////////////////////////////////////////////////////////////////
//
// XThread
//
XThread::XThread(void (*pFn)(void*), 
				 void* pArg ,
				 priority_t pri )
{
    CommonConstructor(pArg, pri, 1);
    m_pFnVoid = pFn;
    m_pFnRet = NULL;
}
XThread::XThread(void* (*pFn)(void*), 
				 void* pArg ,
				 priority_t pri )
{
    CommonConstructor(pArg, pri, 1);
    m_pFnVoid = NULL;
    m_pFnRet = pFn;
}
XThread::XThread(void* pArg, priority_t pri)
{
    CommonConstructor(pArg, pri, 1);
    m_pFnVoid = NULL;
    m_pFnRet = NULL;
}

void XThread::CommonConstructor(void* pArg, priority_t pri, int det)
{
	m_state = STATE_NEW;
	m_priority = pri;

	m_NextIdMutex->lock();
	// ｽﾚｯﾄﾞObjectの数をｲﾝｸﾘﾒﾝﾄ
	m_nId = m_NextId++;
	m_NextIdMutex->unlock();

	m_pThreadArg = pArg;
	m_nDetached = det;	

	m_hCondSemaphore = ::CreateSemaphore(NULL, 0, XSEMAPHORE_MAX, NULL);
	if (m_hCondSemaphore == NULL) {
		XEXCEPT_THROW(XExcept_THREAD, ::GetLastError(), XThredERROR_CREATE_SEM);
	}

	m_pCondNext = m_pCcondPrev = NULL;
	m_bCondWaiting = FALSE;
}

XThread::~XThread(void)
{
	if( m_hCondSemaphore != NULL ) {
		::CloseHandle(m_hCondSemaphore);
		m_hCondSemaphore = NULL;
	}
}

// ｽﾚｯﾄﾞの開始
void XThread::Start(void)
{
	XLocker l(m_mutex);

	if (m_state != STATE_NEW) {
		XEXCEPT_THROW(XExcept_THREAD, 0, XThredERROR_UN_STS_NEW);
	}
	unsigned int t;
	m_hThread = (HANDLE)::_beginthreadex(
					NULL,
					0,	
					XThreadWrapper, 
					(LPVOID)this,
					CREATE_SUSPENDED, 
					&t);
	m_nId = t;
	if(m_hThread == NULL) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_CREATE_THREAD);
	}
	// プライオリティのセット
	if(!::SetThreadPriority(m_hThread, m_priority)) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_CREATE_THREAD);
	}
	// スレッドの再開
	if(::ResumeThread(m_hThread) == 0xffffffff) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_CREATE_THREAD);
	}
	m_state = STATE_RUNNING;
}

void XThread::StartUndetached(void)
{
    if ((m_pFnVoid != NULL) || (m_pFnRet != NULL)) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_UN_INIT);
	}
    m_nDetached = 0;
    Start();
}

void XThread::Join(void** status)
{
	m_mutex.lock();

	if( (m_state != STATE_RUNNING) && 
		(m_state != STATE_TERMINATED) ) {
		m_mutex.unlock();
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_UN_STS_RUNNING_TERMINATED);
	}

	m_mutex.unlock();
	if (this == Self() ) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_JOIN);
	}
	if (m_nDetached) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_JOIN);
	}
	if( ::WaitForSingleObject(m_hThread, INFINITE) != WAIT_OBJECT_0) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_JOIN);
	}

	if(status) {
		*status = m_pReturnVal;
	}
	delete this;
}

void XThread::SetPriority(priority_t pri)
{
    XLocker l(m_mutex);

    if (m_state != STATE_RUNNING) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_UN_STS_RUNNING);
	}

    m_priority = pri;
    if ( ::SetThreadPriority(m_hThread, NTpriority(pri)) == FALSE ) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_UN_STS_RUNNING);
	}
}

XThread* XThread::Create(void (*pFn)(void*), void* pArg, priority_t pri)
{
    XThread* pThread = new XThread(pFn, pArg, pri);
    pThread->Start();
    return pThread;
}

XThread* XThread::Create(void* (*pFn)(void*), void* pArg, priority_t pri)
{
    XThread* pThread = new XThread(pFn, pArg, pri);
    pThread->Start();
    return pThread;
}
void XThread::Exit(void* pRetVal, bool bCallToEndThread)
{
	XThread* pMe = Self();

	if (pMe) {
		pMe->m_mutex.lock();
		pMe->m_state = STATE_TERMINATED;
		pMe->m_mutex.unlock();
		if (pMe->m_nDetached) {
			delete pMe;
		} else {
			pMe->m_pReturnVal = pRetVal;
		}
	}
	if(bCallToEndThread) {
		::_endthreadex(0);
	}
}
//////////////////////////////////////////////////////
//
// ｽﾚｯﾄﾞﾛｰｶﾙｽﾄﾚｰｼﾞ のg_dwSelfTlsIndexで参照される XThread
//	を取り出す
//
/////////////////////////////////////////////////////
XThread* XThread::Self(void)
{
	LPVOID pMe = NULL;
	pMe = ::TlsGetValue(g_dwSelfTlsIndex);

	if (pMe == NULL) {
	}
	return (XThread*)pMe;
}

void XThread::yield(void)
{
    ::Sleep(0);
}

void XThread::Sleep(unsigned long secs, 
					unsigned long nanosecs)
{
    if (secs <= XTREAD_MAX_SLEEP_SECONDS) {
		::Sleep(secs * 1000 + nanosecs);
		return;
    }

    DWORD no_of_max_sleeps = secs / XTREAD_MAX_SLEEP_SECONDS;

    for (DWORD i = 0; i < no_of_max_sleeps; i++) {
		::Sleep(XTREAD_MAX_SLEEP_SECONDS * 1000);
	}
    ::Sleep((secs % XTREAD_MAX_SLEEP_SECONDS) * 1000 + nanosecs / 1000000);
}

void XThread::GetTime(unsigned long* abs_sec, 
					   unsigned long* abs_nsec,		      
					   unsigned long rel_sec, 
					   unsigned long rel_nsec)
{
    GetTimeNow(abs_sec, abs_nsec);
    *abs_nsec += rel_nsec;
    *abs_sec += rel_sec + *abs_nsec / 1000000000;
    *abs_nsec = *abs_nsec % 1000000000;
}

int XThread::NTpriority(priority_t pri)
{
    switch (pri)
	{
    case PRIORITY_LOW:
		return THREAD_PRIORITY_LOWEST;
    case PRIORITY_NORMAL:
		return THREAD_PRIORITY_NORMAL;
    case PRIORITY_HIGH:
		return THREAD_PRIORITY_HIGHEST;
    }
    return 0; 
}
///////////////////////////////////////////////////////////////////////////
//
// _InternalXthreadDummy
//
_InternalXthreadDummy::_InternalXthreadDummy()
	: m_pNext(NULL)
{	
}
_InternalXthreadDummy::~_InternalXthreadDummy()
{
}
///////////////////////////////////////////////////////////////////////////
//
// _InternalXthreadHelper
//
_InternalXthreadHelper::_InternalXthreadHelper()
{
	m_pDummy = 0;
	// ｽﾚｯﾄﾞﾛｰｶﾙｽﾄﾚｰｼﾞ のg_dwSelfTlsIndexで参照される XThread
	//	を取り出す
	m_pThread = XThread::Self();
	// XThread が取得できない場合、ﾀﾞﾐｰを作成する

	if (!m_pThread) {
		XLocker sync(m_CacheLock);
		if (m_pCache) {
			m_pDummy = m_pCache;
			m_pCache = m_pCache->m_pNext;
		} else  {
			m_pDummy = new _InternalXthreadDummy;
		}
		m_pThread = m_pDummy;
	}
}

_InternalXthreadHelper::~_InternalXthreadHelper()
{
	if ( m_pDummy ) {
		XLocker sync(m_CacheLock);
		m_pDummy->m_pNext = m_pCache;
		m_pCache = m_pDummy;
	}
}


XThreadCondition::XThreadCondition(XLock* pLock)
				:m_pLock(pLock)
{
	::InitializeCriticalSection(&m_crit);
	m_pWaitingHead = m_pWaitingHead = NULL;
}
XThreadCondition::~XThreadCondition(void)
{
	::DeleteCriticalSection(&m_crit);
}
void XThreadCondition::Wait(void)
{
	_InternalXthreadHelper pMe;

	::EnterCriticalSection(&m_crit);


	pMe->m_pCondNext = NULL;
	pMe->m_pCcondPrev = m_pWaitingHead;
	if (m_pWaitingHead == NULL) {
		m_pWaitingHead = pMe;
	} else {
		m_pWaitingHead->m_pCondNext = pMe;
	}
	m_pWaitingHead = pMe;
	pMe->m_bCondWaiting = TRUE;

	::LeaveCriticalSection(&m_crit);

	m_pLock->unlock();

	DWORD result = ::WaitForSingleObject(pMe->m_hCondSemaphore, INFINITE);
	m_pLock->lock();
	if (result != WAIT_OBJECT_0) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_WAIT);
	}
}
int XThreadCondition::TimedWait(unsigned long abs_sec, unsigned long abs_nsec)
{
	_InternalXthreadHelper pMe;

	::EnterCriticalSection(&m_crit);

	pMe->m_pCondNext = NULL;
	pMe->m_pCcondPrev = m_pWaitingHead;
	if (m_pWaitingHead == NULL) {
		m_pWaitingHead = pMe;
	} else {
		m_pWaitingHead->m_pCondNext = pMe;
	}
	m_pWaitingHead = pMe;
	pMe->m_bCondWaiting = TRUE;

	::LeaveCriticalSection(&m_crit);

	m_pLock->unlock();

	unsigned long now_sec, now_nsec;

	GetTimeNow(&now_sec, &now_nsec);

	DWORD timeout = (abs_sec-now_sec) * 1000 + (abs_nsec-now_nsec) / 1000000;

	if ((abs_sec <= now_sec) && ((abs_sec < now_sec) || (abs_nsec < abs_nsec))) {
		timeout = 0;
	}

	DWORD result = ::WaitForSingleObject(pMe->m_hCondSemaphore, timeout);
	if (result == WAIT_TIMEOUT) {
		::EnterCriticalSection(&m_crit);

		if (pMe->m_bCondWaiting) {
			if (pMe->m_pCcondPrev != NULL)
				pMe->m_pCcondPrev->m_pCondNext = pMe->m_pCondNext;
			else
				m_pWaitingHead = pMe->m_pCondNext;
			if (pMe->m_pCondNext != NULL)
				pMe->m_pCondNext->m_pCcondPrev = pMe->m_pCcondPrev;
			else
				m_pWaitingHead = pMe->m_pCcondPrev;
			pMe->m_bCondWaiting = FALSE;

			::LeaveCriticalSection(&m_crit);

			m_pLock->lock();
			return 0;
		}
		::LeaveCriticalSection(&m_crit);

		result = WaitForSingleObject(pMe->m_hCondSemaphore, INFINITE);
	}

	if (result != WAIT_OBJECT_0) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_WAIT);
	}

	m_pLock->lock();
	return 1;
}
void XThreadCondition::Signal(void)
{
	::EnterCriticalSection(&m_crit);

	if (m_pWaitingHead != NULL) {
		XThread* t = m_pWaitingHead;
		m_pWaitingHead = t->m_pCondNext;
		if (m_pWaitingHead == NULL) {
			m_pWaitingHead = NULL;
		} else {
			m_pWaitingHead->m_pCcondPrev = NULL;
		}
		t->m_bCondWaiting = FALSE;

		if (!ReleaseSemaphore(t->m_hCondSemaphore, 1, NULL)) {
			int rc = GetLastError();
			::LeaveCriticalSection(&m_crit);
			XEXCEPT_THROW(XExcept_THREAD,  rc, XThredERROR_SIGNAL);
		}
	}
	::LeaveCriticalSection(&m_crit);
}

void XThreadCondition::BroadCast(void)
{
	::EnterCriticalSection(&m_crit);

	while (m_pWaitingHead != NULL) {
		XThread* t = m_pWaitingHead;
		m_pWaitingHead = t->m_pCondNext;
		if (m_pWaitingHead == NULL) {
			m_pWaitingHead = NULL;
		} else {
			m_pWaitingHead->m_pCcondPrev = NULL;
		}
		t->m_bCondWaiting = FALSE;

		if (!ReleaseSemaphore(t->m_hCondSemaphore, 1, NULL)) {
			int rc = GetLastError();
			::LeaveCriticalSection(&m_crit);
			XEXCEPT_THROW(XExcept_THREAD,  rc, XThredERROR_BROAD_CAST);
		}
	}
	::LeaveCriticalSection(&m_crit);
}

///////////////////////////////////////////////////////////////////////////
//
// XThread::InitT
//
XThread::InitT::InitT(void)
{
#ifdef _DEBUG
	XTRACE(_T("enter XThread::InitT::InitT - count=(%d)\n"), m_count);
#endif
    if (m_count++ != 0){
		return;
	}

	g_dwSelfTlsIndex = ::TlsAlloc();
	if (g_dwSelfTlsIndex == 0xffffffff) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_INIT);
	}
	m_NextIdMutex = NULL;
	m_NextIdMutex = new XLock;

	XThread* pThread = new XThread;

	pThread->m_state = STATE_RUNNING;

	if (!::DuplicateHandle(::GetCurrentProcess(),
			::GetCurrentThread(),
			::GetCurrentProcess(), &pThread->m_hThread,
			0, FALSE, DUPLICATE_SAME_ACCESS))
	{
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_INIT);
	}

	pThread->m_dwNtId = ::GetCurrentThreadId();
	if (!::TlsSetValue(g_dwSelfTlsIndex, (LPVOID)pThread)) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_INIT);
	}
	if (!::SetThreadPriority(pThread->m_hThread, NTpriority(PRIORITY_NORMAL))) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_INIT);
	}
}

XThread::InitT::~InitT(void)
{
#ifdef _DEBUG
	XTRACE(_T("enter XThread::InitT::~InitT - count=(%d)\n"), m_count);
#endif /* _DEBUG  */
	XThread* pThread = XThread::Self();

	if(m_NextIdMutex) {
		delete m_NextIdMutex;
		m_NextIdMutex = NULL;
	}

	if(pThread) {
		pThread->Exit(NULL, FALSE);
	}
	::TlsFree(g_dwSelfTlsIndex);
}

unsigned __stdcall XThreadWrapper(void* ptr)
{
    XThread* pMe = (XThread*)ptr;
	// ｽﾚｯﾄﾞ開始と動じにTLSに自分を追加
    if ( ::TlsSetValue(g_dwSelfTlsIndex, (LPVOID)pMe) == FALSE) {
		XEXCEPT_THROW(XExcept_THREAD,  ::GetLastError(), XThredERROR_INIT);
	}
    if (pMe->m_pFnVoid != NULL) {
		(*pMe->m_pFnVoid)(pMe->m_pThreadArg);
		XThread::Exit();
    }

    if (pMe->m_pFnRet != NULL) {
		void* pRetVal = (*pMe->m_pFnRet)(pMe->m_pThreadArg);
		XThread::Exit(pRetVal);
    }

    if (pMe->m_nDetached) {
		pMe->Run(pMe->m_pThreadArg);
		XThread::Exit();
    } else {
		// StartUndetached を実行した場合
		void* pRetVal = pMe->RunUndetached(pMe->m_pThreadArg);
		XThread::Exit(pRetVal);
    }
    return 0;
}


#else /* For POSIX */




///////////////////////////////////////////////////////////////////////////
//
// Global value initialize
//
XLock*               XThread::m_NextIdMutex;
int                  XThread::m_NextId = 0;
static pthread_key_t g_dwSelfTlsIndex = 0;

#ifdef PthreadSupportThreadPriority
	static int g_LowestPriority;
	static int g_NormalPriority;
	static int g_HighestPriority;
#endif

#if defined(__osf1__) && defined(__alpha__) || defined(__VMS)
  static size_t g_StackSize = 32768;
#elif defined(__rtems__)
  static size_t g_StackSize = ThreadStackSize;
#elif defined(__aix__)
  static size_t g_StackSize = 262144;
#else
  static size_t g_StackSize = 0;
#endif

static int& count() {
  static int the_count = 0;
  return the_count;
}


///////////////////////////////////////////////////////////////////////////
//
// XThread::InitT
//
XThread::InitT::InitT(void)
{
	XTRACE(_T("XThread::InitT::InitT enter\n"));
	if (count()++ != 0) {
		return;
	}
	int ret;
#ifdef NeedPthreadInit
	XTRACE(_T("   NeedPthreadInit\n"));
	pthread_init();
#endif

#ifdef _DEBUG
	int nPthreadDraftVersion = PthreadDraftVersion;
	XTRACE(_T("   PthreadDraftVersion = %d\n"), nPthreadDraftVersion);
#endif /* _DEBUG */

#if (PthreadDraftVersion == 4)
	THROW_ERRORS_XTHREAD(pthread_keycreate(&g_dwSelfTlsIndex, NULL), XThredERROR_INIT);
#else
	THROW_ERRORS_XTHREAD(pthread_key_create(&g_dwSelfTlsIndex, NULL), XThredERROR_INIT);
#endif

#ifdef PthreadSupportThreadPriority

	#if defined(__osf1__) && defined(__alpha__) || defined(__VMS)
		g_LowestPriority = PRI_OTHER_MIN;
		g_HighestPriority = PRI_OTHER_MAX;
	#elif defined(__hpux__)
		g_LowestPriority = PRI_OTHER_MIN;
		g_HighestPriority = PRI_OTHER_MAX;
	#elif defined(__sunos__) && (__OSVERSION__ == 5)
		// a bug in pthread_attr_setschedparam means lowest priority is 1 not 0
		g_LowestPriority  = 1;
		g_HighestPriority = 3;
	#else
		g_LowestPriority = sched_get_priority_min(SCHED_FIFO);
		g_HighestPriority = sched_get_priority_max(SCHED_FIFO);
	#endif

	switch (g_HighestPriority - g_LowestPriority) 
	{
	case 0:
	case 1:
		g_NormalPriority = g_LowestPriority;
		break;
	default:
		g_NormalPriority = g_LowestPriority + 1;
		break;
	}
#endif   /* PthreadSupportThreadPriority */

	m_NextIdMutex = new XLock;
	//
	// Create object for this (i.e. initial) thread.
	//
	XThread* t = new XThread;

	t->m_state = STATE_RUNNING;

	t->m_hThread = pthread_self();

	ret = pthread_setspecific(g_dwSelfTlsIndex, (void*)t);
	THROW_ERRORS_XTHREAD(ret, XThredERROR_INIT);
#ifdef PthreadSupportThreadPriority
	#if (PthreadDraftVersion == 4)
		THROW_ERRORS_XTHREAD(pthread_setprio(t->m_hThread,
			 posix_priority(PRIORITY_NORMAL))   , XThredERROR_INIT);
	#elif (PthreadDraftVersion == 6)
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		THROW_ERRORS_XTHREAD(pthread_attr_setprio(&attr, PosixPriority(PRIORITY_NORMAL))  , XThredERROR_INIT);
		THROW_ERRORS_XTHREAD(pthread_setschedattr(t->m_hThread, attr)   , XThredERROR_INIT);
	#else
		struct sched_param sparam;
		sparam.sched_priority = PosixPriority(PRIORITY_NORMAL);
//		THROW_ERRORS_XTHREAD(pthread_setschedparam(t->m_hThread, SCHED_OTHER, &sparam));

//	  #ifndef __linux__
//		THROW_ERRORS_XTHREAD(pthread_setschedparam(t->m_hThread, SCHED_OTHER, &sparam)  , XThredERROR_INIT);
//	  #else
		int nErr = pthread_setschedparam(t->m_hThread, SCHED_OTHER, &sparam);
		if( nErr != 0 ) {
			switch( nErr ) {
			case ENOTSUP: // Not support
				break;
			default:
				XTRACE(_T(" pthread_setschedparam Error %s \n"), strerror(nErr) );
//				THROW_ERRORS_XTHREAD(nErr  , XThredERROR_INIT);
				break;
			}
		}
//	  #endif /* __linux__ */

	#endif   /* PthreadDraftVersion */

#endif   /* PthreadSupportThreadPriority */
	XTRACE(_T("XThread::InitT::InitT end\n"));
}

XThread::InitT::~InitT(void)
{
	XTRACE(_T("XThread::InitT::~InitT enter\n"));
	if (--count() != 0)
		return;

	XThread* self = XThread::Self();
	if (!self)
		return;

	pthread_setspecific(g_dwSelfTlsIndex, 0);
	delete self;

	if(m_NextIdMutex != NULL)
		delete m_NextIdMutex;
	XTRACE(_T("XThread::InitT::~InitT end\n"));
}

extern "C" void* XThreadWrapper(void* ptr)
{
	XThread* me = (XThread*)ptr;

	XTRACE(_T("XThreadWrapper enter\n"));

	THROW_ERRORS_XTHREAD(pthread_setspecific(g_dwSelfTlsIndex, me) , XThredERROR_INIT);

	//
	// Now invoke the thread function with the given argument.
	//
	if (me->m_pFnVoid != NULL) {
		(*me->m_pFnVoid)(me->m_pThreadArg);
		XThread::Exit();
	}

	if (me->m_pFnRet != NULL) {
		void* return_value = (*me->m_pFnRet)(me->m_pThreadArg);
		XThread::Exit(return_value);
	}

	if (me->m_nDetached) {
		me->Run(me->m_pThreadArg);
		XThread::Exit();
	} else {
		void* return_value = me->RunUndetached(me->m_pThreadArg);
		XThread::Exit(return_value);
	}
	XTRACE(_T("XThreadWrapper end\n"));
	// should never get here.
	return NULL;
}

///////////////////////////////////////////////////////////////////////////
//
// XThread
//
XThread::XThread(void (*pFn)(void*), 
				 void* pArg ,
				 priority_t pri )
{
    CommonConstructor(pArg, pri, 1);
    m_pFnVoid = pFn;
    m_pFnRet = NULL;
}
XThread::XThread(void* (*pFn)(void*), 
				 void* pArg ,
				 priority_t pri )
{
    CommonConstructor(pArg, pri, 1);
    m_pFnVoid = NULL;
    m_pFnRet = pFn;
}
XThread::XThread(void* pArg, priority_t pri)
{
    CommonConstructor(pArg, pri, 1);
    m_pFnVoid = NULL;
    m_pFnRet = NULL;
}

void XThread::CommonConstructor(void* pArg, priority_t pri, int det)
{
	m_state = STATE_NEW;
	m_priority = pri;

	m_NextIdMutex->lock();
	// ｽﾚｯﾄﾞObjectの数をｲﾝｸﾘﾒﾝﾄ
	m_nId = m_NextId++;
	m_NextIdMutex->unlock();

	m_pThreadArg = pArg;
	m_nDetached = det;	
}
XThread::~XThread(void)
{
}

// ｽﾚｯﾄﾞの開始
void XThread::Start(void)
{
	XTRACE(_T("XThread::Start enter\n"));
	XLocker l(m_mutex);

	if (m_state != STATE_NEW) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_UN_STS_NEW);
	}

	pthread_attr_t attr;
#if (PthreadDraftVersion == 4)
	pthread_attr_create(&attr);
#else
	pthread_attr_init(&attr);
#endif

#if (PthreadDraftVersion == 8)
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_UNDETACHED);
#endif

#ifdef PthreadSupportThreadPriority
	#if (PthreadDraftVersion <= 6)
		THROW_ERRORS_XTHREAD(pthread_attr_setprio(&attr, PosixPriority(_priority)), XThredERROR_SET_PRIORITY);
	#else
		struct sched_param sparam;
		sparam.sched_priority = PosixPriority(m_priority);
		int nErr = pthread_attr_setschedparam(&attr, &sparam);
		if( nErr != 0 ) {
			switch( nErr ) {
			case ENOTSUP: // Not support
				break;
			default:
				XTRACE(_T(" pthread_setschedparam Error %s \n"), strerror(nErr) );
//				THROW_ERRORS_XTHREAD(nErr  , XThredERROR_INIT);
				break;
			}
		}
	#endif	/* PthreadDraftVersion */
#endif	/* PthreadSupportThreadPriority */

#if !defined(__linux__)
	if (g_StackSize) {
		THROW_ERRORS_XTHREAD(pthread_attr_setstacksize(&attr, g_StackSize), XThredERROR_CREATE_THREAD);
	}
#endif

#if (PthreadDraftVersion == 4)
    THROW_ERRORS_XTHREAD(pthread_create(&m_hThread, attr, XThreadWrapper, (void*)this), XThredERROR_CREATE_THREAD);
    pthread_attr_delete(&attr);
#else
    THROW_ERRORS_XTHREAD(pthread_create(&m_hThread, &attr, XThreadWrapper, (void*)this), XThredERROR_CREATE_THREAD);
    pthread_attr_destroy(&attr);
#endif
    m_state = STATE_RUNNING;
    if (m_nDetached) {
#if (PthreadDraftVersion <= 6)
		THROW_ERRORS_XTHREAD(pthread_detach(&m_hThread), XThredERROR_CREATE_THREAD);
#else
		THROW_ERRORS_XTHREAD(pthread_detach(m_hThread), XThredERROR_CREATE_THREAD);
#endif
    }
	XTRACE(_T("XThread::Start end\n"));
}


void XThread::StartUndetached(void)
{
	XTRACE(_T("XThread::StartUndetached enter\n"));
    if ((m_pFnVoid != NULL) || (m_pFnRet != NULL)) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_UN_INIT);
	}
    m_nDetached = 0;
    Start();
	XTRACE(_T("XThread::StartUndetached end\n"));
}

void XThread::Join(void** status)
{
	m_mutex.lock();

	if ((m_state != STATE_RUNNING) && 
		(m_state != STATE_TERMINATED)) {
		m_mutex.unlock();
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_UN_STS_RUNNING_TERMINATED);
	}

	m_mutex.unlock();
	if (this == Self()) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_JOIN);
	}
	if (m_nDetached) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_JOIN);
	}

	THROW_ERRORS_XTHREAD(pthread_join(m_hThread, status), XThredERROR_JOIN);
#if (PthreadDraftVersion == 4)
	THROW_ERRORS_XTHREAD(pthread_detach(&posix_thread));
#endif
	delete this;
}


void XThread::SetPriority(priority_t pri)
{
    XLocker l(m_mutex);

	if (m_state != STATE_RUNNING) {
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_UN_STS_RUNNING);
	}

    m_priority = pri;
#ifdef PthreadSupportThreadPriority
	#if (PthreadDraftVersion == 4)
		THROW_ERRORS_XTHREAD(pthread_setprio(m_hThread, PosixPriority(pri)), XThredERROR_SET_PRIORITY);
	#elif (PthreadDraftVersion == 6)
		pthread_attr_t attr;
		pthread_attr_init(&attr);

		THROW_ERRORS_XTHREAD(pthread_attr_setprio(&attr, PosixPriority(pri)), XThredERROR_SET_PRIORITY);

		THROW_ERRORS_XTHREAD(pthread_setschedattr(m_hThread, attr),  XThredERROR_SET_PRIORITY);
	#else
		struct sched_param sparam;
		sparam.sched_priority = PosixPriority(pri);
		THROW_ERRORS_XTHREAD(pthread_setschedparam(m_hThread, SCHED_OTHER, &sparam), XThredERROR_SET_PRIORITY);
	#endif   /* PthreadDraftVersion */
#endif   /* PthreadSupportThreadPriority */
}


XThread* XThread::Create(void (*pFn)(void*), void* pArg, priority_t pri)
{
    XThread* pThread = new XThread(pFn, pArg, pri);
    pThread->Start();
    return pThread;
}

XThread* XThread::Create(void* (*pFn)(void*), void* pArg, priority_t pri)
{
    XThread* pThread = new XThread(pFn, pArg, pri);
    pThread->Start();
    return pThread;
}
void XThread::Exit(void* pRetVal, bool bCallToEndThread)
{
	XThread* pMe = Self();

	if (pMe) {
		pMe->m_mutex.lock();
		pMe->m_state = STATE_TERMINATED;
		pMe->m_mutex.unlock();

		if (pMe->m_nDetached) {
			delete pMe;
		}
	}
	pthread_exit(pRetVal);
}
XThread* XThread::Self(void)
{
    XThread* me;

#if (PthreadDraftVersion <= 6)
    THROW_ERRORS_XTHREAD(pthread_getspecific(g_dwSelfTlsIndex, (void**)&me), XThredERROR_SELF);
#else
    me = (XThread *)pthread_getspecific(g_dwSelfTlsIndex);
#endif

	if (!me) {
	}
	return me;
}

void XThread::yield(void)
{
#if !defined(__osf1__) /* True64 はノーサポート*/
#if (PthreadDraftVersion == 6)
    pthread_yield(NULL);
#elif (PthreadDraftVersion < 9)
//    pthread_yield();
#else
    THROW_ERRORS_XTHREAD(sched_yield(), XThredERROR_YIELD);
#endif
#else
	pthread_yield_np();
#endif
}

void XThread::Sleep(unsigned long secs, 
					unsigned long nanosecs)
{
	timespec rqts = { secs, nanosecs };

#ifndef NoNanoSleep
	timespec remain;
	while (nanosleep(&rqts, &remain)) {
		if (errno == EINTR) {
			rqts.tv_sec  = remain.tv_sec;
			rqts.tv_nsec = remain.tv_nsec;
			continue;
		} else {
			XEXCEPT_THROW(XExcept_THREAD,  errno, XThredERROR_SLEEP);
		}
	}
#else
	#if defined(__osf1__) && defined(__alpha__) || defined(__hpux__) && (__OSVERSION__ == 10) || defined(__VMS) || defined(__SINIX__) || defined (__POSIX_NT__)
		if (pthread_delay_np(&rqts) != 0)
			XEXCEPT_THROW(XExcept_THREAD,  errno, XThredERROR_SLEEP);

	#elif defined(__linux__) || defined(__aix__)
		if (secs > 2000)
		{
			while ((secs = ::sleep(secs))) ;
		}
		else
		{
			usleep(secs * 1000000 + (nanosecs / 1000));
		}
	#elif defined(__darwin__) || defined(__macos__)
		// Single UNIX Specification says argument of usleep() must be
		// less than 1,000,000.
		secs += nanosecs / 1000000000;
		nanosecs %= 1000000000;
		while ((secs = ::sleep(secs))) ;
			usleep(nanosecs / 1000);
	#else
		XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_SLEEP);
	#endif
#endif	/* NoNanoSleep */
}
void XThread::GetTime(unsigned long* abs_sec, 
					   unsigned long* abs_nsec,		      
					   unsigned long rel_sec, 
					   unsigned long rel_nsec)
{
	timespec abs;

#if defined(__osf1__) && defined(__alpha__) || defined(__hpux__) && (__OSVERSION__ == 10) || defined(__VMS) || defined(__SINIX__) || defined(__POSIX_NT__)

	timespec rel;
	rel.tv_sec = rel_sec;
	rel.tv_nsec = rel_nsec;
	THROW_ERRORS_XTHREAD(pthread_get_expiration_np(&rel, &abs), XThredERROR_GETTIME);

#else

#if defined(__linux__) || defined(__aix__) || defined(__SCO_VERSION__) || defined(__darwin__) || defined(__macos__) || defined(__CYGWIN__)

	struct timeval tv;
	gettimeofday(&tv, NULL); 
	abs.tv_sec = tv.tv_sec;
	abs.tv_nsec = tv.tv_usec * 1000;

#else	/* __linux__ || __aix__ */

	clock_gettime(CLOCK_REALTIME, &abs);

#endif	/* __linux__ || __aix__ */

	abs.tv_nsec += rel_nsec;
	abs.tv_sec += rel_sec + abs.tv_nsec / 1000000000;
	abs.tv_nsec = abs.tv_nsec % 1000000000;

#endif	/* __osf1__ && __alpha__ */

	*abs_sec = abs.tv_sec;
	*abs_nsec = abs.tv_nsec;
}

int XThread::PosixPriority(priority_t pri)
{
#ifdef PthreadSupportThreadPriority
	switch (pri)
	{
	case PRIORITY_LOW:
		return g_LowestPriority;

	case PRIORITY_NORMAL:
		return g_NormalPriority;

	case PRIORITY_HIGH:
		return g_HighestPriority;

	}
#endif
	XEXCEPT_THROW(XExcept_THREAD,  0, XThredERROR_NO_SUPPORT);
#ifdef _MSC_VER
	return 0;
#endif
}

#endif //WIN32
