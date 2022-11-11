#ifdef WIN32
  #include <windows.h>
#else
  #include <sys/time.h>
#endif
#include "xlib/xlock.h"
#include "xlib/xeception.h"

//////////////////////////////////////////////////////////////////////////////////////
//
//	XLock
//
XLock::XLock(void)
{
#ifdef WIN32
	::InitializeCriticalSection(&rc);
#else /* POSIX */
	#if (PthreadDraftVersion == 4)
	pthread_mutex_init(&m_hMutex, pthread_mutexattr_default);
	#else
	pthread_mutex_init(&m_hMutex, 0);
	#endif
#endif /* WIN32 */
}
XLock::~XLock(void)
{
#ifdef WIN32
    ::DeleteCriticalSection(&rc);
#else /* POSIX */
	pthread_mutex_destroy(&m_hMutex);
#endif /* WIN32 */
}
void XLock::lock(void)
{
#ifdef WIN32
    ::EnterCriticalSection(&rc);
#else /* POSIX */
	pthread_mutex_lock(&m_hMutex);
#endif /* WIN32 */
}
void XLock::unlock(void)
{
#ifdef WIN32
    ::LeaveCriticalSection(&rc);    
#else /* POSIX */
	pthread_mutex_unlock(&m_hMutex);
#endif /* WIN32 */
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	XMutex
//
XMutex::XMutex()
{
#ifdef WIN32
	m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
#else /* POSIX */

	m_ilocks = 0;
	pthread_mutexattr_init(&m_mutexattr);

	#if (PthreadDraftVersion == 4)
	pthread_mutex_init(&m_hMutex, pthread_mutexattr_default);
	#else
	pthread_mutex_init(&m_hMutex, &m_mutexattr);
	#endif

	pthread_mutex_init(&m_sig_mutex, 0);

	pthread_cond_init(&m_sig_cond, 0);


#endif /* WIN32 */
}
XMutex::~XMutex()
{
#ifdef WIN32

	::CloseHandle(m_hMutex);

#else /* POSIX */

    pthread_mutexattr_destroy(&m_mutexattr);
    pthread_mutex_destroy(&m_hMutex);
	pthread_mutex_destroy(&m_sig_mutex);
	pthread_cond_destroy (&m_sig_cond);

#endif /* WIN32 */
}
void XMutex::lock(long lTimeOut /*= -1*/)
{
#ifdef WIN32
	if(m_hMutex != NULL) {
		DWORD dw = ::WaitForSingleObject(m_hMutex, INFINITE);
		if( dw == WAIT_OBJECT_0 ) {
			return;
		} else
		if( dw == WAIT_TIMEOUT ) {
			return;
		}
		return;
	}
	return;
#else /* POSIX */
	int rc;
	int rTimeout = 0;

	pthread_mutex_lock(&m_sig_mutex);
	m_ilocks++;
	pthread_mutex_unlock(&m_sig_mutex);

	struct timespec timeout;
	struct timespec curTime;

	if( lTimeOut != -1 ) {
    	timeval tv;
		int iResult = gettimeofday(&tv, 0);
		curTime.tv_sec  = tv.tv_sec;
		curTime.tv_nsec = tv.tv_usec*1000;

		timeout.tv_sec  = curTime.tv_sec  +   lTimeOut  / 1000;
		timeout.tv_nsec = curTime.tv_nsec + ( lTimeOut  % 1000) * 1000000;
		if( timeout.tv_nsec >= 1000000000) {
			timeout.tv_nsec -= 1000000000;
			timeout.tv_sec  += 1;
		}
	}

	do {																		//	mutex取得待ちループ.
		pthread_mutex_lock(&m_sig_mutex);										//	タイムアウト用mutexのロック.
		if(m_ilocks == 1){
			rc = pthread_mutex_trylock(&m_hMutex);								//	ミューテックス　ロック.
			if (rc==0) {														//	lock成功			★ mutexループ脱出.
				pthread_mutex_unlock(&m_sig_mutex);
				break;
			}
			else if(rc==EINVAL){												//	EINVAL以外の値としてEBUSYがあるがこれはスルーさせる
				pthread_mutex_unlock(&m_sig_mutex);
				break;
			}
		}

		if( lTimeOut == -1 ){											//	タイムアウト無し.
			rTimeout = pthread_cond_wait(&m_sig_cond, &m_sig_mutex);			//	broadcast 待ち.
		} else {																//	タイムアウト有り.
			rTimeout = pthread_cond_timedwait(&m_sig_cond, &m_sig_mutex, &timeout);	//	broadcast or timeout待ち.
		}
		pthread_mutex_unlock(&m_sig_mutex);
	} while (rTimeout == 0);

	if( rTimeout !=0 ){
		pthread_mutex_lock(&m_sig_mutex);
		// m_ilocks
		pthread_mutex_unlock(&m_sig_mutex);
	}
	if (rTimeout != 0) {
		return ;
	}
#endif /* WIN32 */
}
bool XMutex::ulock()
{
#ifdef WIN32
	if(m_hMutex != NULL)
		return ::ReleaseMutex(m_hMutex) == TRUE ? true : false;
	return FALSE;
#else /* POSIX */
	int rc = 0;

	if(m_ilocks == 0 ) {
		pthread_mutex_unlock(&m_sig_mutex);
		return false;
	}
	pthread_mutex_lock(&m_sig_mutex);
	pthread_cond_broadcast(&m_sig_cond);
	rc = pthread_mutex_unlock(&m_hMutex);
	pthread_mutex_unlock(&m_sig_mutex);

	return (rc==0);	
#endif /* WIN32 */
}




//////////////////////////////////////////////////////////////////////////////////////
//
//	XCondition
//
#ifndef WIN32 /* for POSIX implement pthread_cond... */

#if (PthreadDraftVersion <= 6)
	#define ERRNO(x) (((x) != 0) ? (errno) : 0)
#else
	#define ERRNO(x) (x)
#endif


XCondition::XCondition(XMutex *pMutex)
		:m_pMutex(pMutex)
{
#if (PthreadDraftVersion == 4)
    pthread_cond_init(&m_Cond, pthread_condattr_default);
#else
    pthread_cond_init(&m_Cond, 0);
#endif
}
XCondition::~XCondition()
{
	pthread_cond_destroy(&m_Cond);
}
void XCondition::Wait(void)
{
	pthread_cond_wait(&m_Cond, &m_pMutex->m_hMutex);
}
int XCondition::TimedWait(unsigned long secs, unsigned long nanosecs)
{
	timespec rqts = { secs, nanosecs };
again:
	int rc = ERRNO(pthread_cond_timedwait(&m_Cond,
			  &m_pMutex->m_hMutex, &rqts));
	if (rc == 0)
		return 1;

#if (PthreadDraftVersion <= 6)
	if (rc == EAGAIN)
		return 0;
#endif

	if (rc == EINTR)
		goto again;

	if (rc == ETIMEDOUT)
		return 0;

	return 0;
}
void XCondition::Signal(void)
{
    pthread_cond_signal(&m_Cond);
}
void XCondition::BroadCast(void)
{
    pthread_cond_broadcast(&m_Cond);
}
#endif



//////////////////////////////////////////////////////////////////////////////////////
//
//	XSemaphore
//
XSemaphore::XSemaphore(unsigned int nInitial/* = 0*/, unsigned int nMaxCount /*= 1*/)
#ifdef WIN32

#else /* POSIX */
	:m_Condition(&m_Mutex)
#endif /* WIN32 */
{
#ifdef WIN32
	m_hMutex = ::CreateSemaphore(NULL, nInitial, nMaxCount, NULL);
#else /* POSIX */
	m_iValue = nInitial;
#endif /* WIN32 */
}
XSemaphore::~XSemaphore()
{

#ifdef WIN32
	if( m_hMutex != NULL ) {
		::CloseHandle(m_hMutex);
	}
#else /* POSIX */

#endif /* WIN32 */
}
bool XSemaphore::lock(long lTimeOut /*= -1*/)
{
#ifdef WIN32
	if( m_hMutex != NULL) {
		DWORD dwNumOfWait = ::WaitForSingleObject(m_hMutex, lTimeOut == -1 ? INFINITE : lTimeOut);
		if( dwNumOfWait == WAIT_OBJECT_0 ) {
			return true;
		}
#ifdef _DEBUG
		else
		if( dwNumOfWait == WAIT_TIMEOUT ) {
//			XTRACE(_T("XSemaphore::lock WaitForSingleObject Timeout(%d)\n"), lTimeOut);
		} else
		if( dwNumOfWait == WAIT_ABANDONED ) {
			XTRACE(_T("XSemaphore::lock WaitForSingleObject WAIT_ABANDONED(%d)\n"), ::GetLastError());
		} else
		if( dwNumOfWait == WAIT_FAILED ) {
			XTRACE(_T("XSemaphore::lock WaitForSingleObject WAIT_FAILED(%d)\n"), ::GetLastError());
		} else {
			XTRACE(_T("XSemaphore::lock WaitForSingleObject Unknown(%d)(%d)\n"), dwNumOfWait, ::GetLastError());
		}
#endif /*_DEBUG*/
	}
	return false;
#endif /* WIN32 */
}
bool XSemaphore::unlock()
{
#ifdef WIN32
    if (::ReleaseSemaphore(m_hMutex, 1, NULL) == FALSE) {

	}
	return true;
#endif /* WIN32 */
}
void XSemaphore::Wait(void)
{
#ifdef WIN32
	if( m_hMutex != NULL) {
		DWORD dwNumOfWait = ::WaitForSingleObject(m_hMutex, INFINITE);
		if( dwNumOfWait != WAIT_OBJECT_0 ) {

		}
	}
#else /* POSIX */
    XMutexer l(m_Mutex);
    while (m_iValue == 0) {
		m_Condition.Wait();
	}
    m_iValue--;
#endif /* WIN32 */
}
int XSemaphore::Trywait(void)
{
#ifdef WIN32
	DWORD dwNumOfWait = ::WaitForSingleObject(m_hMutex, 0);
	switch( dwNumOfWait )
	{
	case WAIT_OBJECT_0:
		return 1;
	case WAIT_TIMEOUT:
		return 0;
	}
    return 0;
#else /* POSIX */
	XMutexer l(m_Mutex);

	if (m_iValue == 0) {
		return 0;
	}
	m_iValue--;
	return 1;
#endif /* WIN32 */
}
void XSemaphore::Post(void)
{
#ifdef WIN32
    if (::ReleaseSemaphore(m_hMutex, 1, NULL) == FALSE) {

	}
#else /* POSIX */
    XMutexer l(m_Mutex);
    m_iValue++;
    m_Condition.Signal();
#endif /* WIN32 */
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	XEvent
//
#ifdef WIN32
XEvent::XEvent(BOOL bInitiallyOwn/* = FALSE*/, BOOL bManualReset/* = FALSE*/,
		LPCTSTR lpszName/* = NULL*/, LPSECURITY_ATTRIBUTES lpsaAttribute/* = NULL*/)
{
	m_hMutex = ::CreateEvent(lpsaAttribute, bManualReset,
		bInitiallyOwn, lpszName);
}
BOOL XEvent::SetEvent()
	{ _ASSERT(m_hMutex != NULL); return ::SetEvent(m_hMutex); }
BOOL XEvent::PulseEvent()
	{ _ASSERT(m_hMutex != NULL); return ::PulseEvent(m_hMutex); }
BOOL XEvent::ResetEvent()
	{ _ASSERT(m_hMutex != NULL); return ::ResetEvent(m_hMutex); }
BOOL XEvent::Unlock()
{
	return TRUE;
}
unsigned long XEvent::Wait(unsigned long dwWaitTime)
{
	if(m_hMutex == NULL)
	{
		::SetLastError(ERROR_INVALID_HANDLE);
		return ERROR_INVALID_HANDLE;
	}
	return ::WaitForSingleObject(m_hMutex, dwWaitTime);
}
#endif /* WIN32 */
