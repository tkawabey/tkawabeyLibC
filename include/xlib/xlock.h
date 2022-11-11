/**
 * @file xlock.h
 * @brief ファイルアクセス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_LOCK_HEASER_FILE__
#define __X_LOCK_HEASER_FILE__

#include "xlib/xlib_head.h"


#ifdef WIN32
  #include <windows.h>
#else /* POSIX */
  #include <pthread.h>
  #define INFINITE 0
#endif /* WIN32 */


#define XSEMAPHORE_MAX 0x7fffffff

/**
 * ロックオブジェクト
 */
class XLIB_API XLock
{
public:
	/** コンストラクタ */
	XLock();
	/** デストラクタ */
	~XLock();
	/** ロックします。 */
	void lock();
	/** アンロックします。 */
	void unlock();
	friend class XLocker;
private:
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 */
	XLock(const XLock& l);
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 * @return 自身のインスタンス
	 */
	XLock& operator=(const XLock& l);	

#ifdef WIN32
	/** ロックオブジェクト */
	CRITICAL_SECTION rc;
#else /* POSIX */
	/** ロックオブジェクト */
	pthread_mutex_t m_hMutex;
#endif /* WIN32 */
};

/**
 * 自動ロック＆アンロックヘルパー
 */
class XLIB_API XLocker
{
protected:
	/** ロックオブジェクト参照値 */
	XLock	&critsync;
public:
	/**
	 * コンストラクタ
	 * @param m ロックオブジェクト
	 */
    XLocker(XLock& m) : critsync(m) { critsync.lock(); }
	/** デストラクタ */
    ~XLocker(void) { critsync.unlock(); }
private:
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 */
    XLocker(const XLock& l);
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 * @return 自身のインスタンス
	 */
    XLocker operator=(const XLock& l);
};




class XLIB_API XMutexer;
/**
 * 同期オブジェクトMutexをWrap
 */
class XLIB_API XMutex
{
public:
	/** コンストラクタ */
	XMutex();
	/** デストラクタ */
	~XMutex();
	/**
	 * ロックします。
	 */
	void lock(long lTimeOut = -1);
	/**
	 * アンロックします。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool ulock();

	friend class XMutexer;
#ifdef WIN32
	/** ロックオブジェクト */
	HANDLE			m_hMutex;
#else /* POSIX */
	/** ロックオブジェクト */
	pthread_mutex_t		m_hMutex;
	pthread_mutexattr_t m_mutexattr;
	pthread_mutex_t		m_sig_mutex;
	pthread_cond_t		m_sig_cond;
	int					m_ilocks;
#endif /* WIN32 */
};

/**
 * 自動ロック＆アンロックヘルパー
 */
class XLIB_API XMutexer
{
protected:
	/** ロックオブジェクト参照値 */
	XMutex &mutex;
public:
	/**
	 * コンストラクタ
	 * @param m ロックオブジェクト
	 */
	XMutexer(XMutex& m) : mutex(m) {mutex.lock();}
	/** デストラクタ */
	~XMutexer(){mutex.ulock();}
private:
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 */
    XMutexer(const XMutex& l);
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 * @return 自身のインスタンス
	 */
    XMutexer operator=(const XMutex& l);
};


#ifndef WIN32 // for POSIX implement pthread_cond...
/**
 * セマフォ用ヘルパークラス
 */
class XLIB_API XCondition
{
public:
	/**
	 * コンストラクタ
	 * @param pMutex ロックオブジェクト
	 */
	XCondition(XMutex *pMutex);
	/** デストラクタ */
	~XCondition();
	/**
	 * シグナルが発生するまでロックします。
	 */
    void Wait(void);
	/**
	 * 指定時間、シグナルが発生するまでロックします。
	 * @param[in] abs_sec 秒
	 * @param[in] abs_nsec ナノ秒
	 * @retval true 成功
	 * @retval false 失敗
	 */
    int TimedWait(unsigned long abs_sec, unsigned long abs_nsec = 0);

	// 指定された時間まで、WAIT
	/**
	 * シグナルを送信します。
	 */
    void Signal(void);
	/**
	 * 待機しているオブジェクト全てにシグナルを送信します。
	 */
    void BroadCast(void);
private:
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 */
    XCondition(const XCondition& l);
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 * @return 自身のインスタンス
	 */
    XCondition& operator=(const XCondition& l);
	/** ロックオブジェクト */
    XMutex         *m_pMutex;
	/** コンディションオブジェクト */
    pthread_cond_t m_Cond;
};
#endif


class XLIB_API XSemaphorer;
/**
 * 同期オブジェクトSemaphoreをWrap
 */
class XLIB_API XSemaphore
{
public:
	/**
	 * コンストラクタ
	 * @param nInitial 初期化数
	 */
	XSemaphore(unsigned int nInitial = 0, unsigned int nMaxCount = 1);
	/** デストラクタ */
	virtual ~XSemaphore();

	bool lock(long lTimeOut = -1);

	/** アンロックします。 */
	bool unlock();
	/**
	 * シグナルが発生するまでロックします。
	 */
	void Wait(void);
	/**
	 * ロックできるかそうか検査します。
	 * @retval 1 できる
	 * @retval 0 できない
	 */
	int Trywait(void);
	/**
	 * 指定されたセマフォオブジェクトのカウント値を、指定された数だけ増やします。
	 */
	void Post(void);
private:
#ifdef WIN32
	/** セマフォ用ハンドル */
	HANDLE			m_hMutex;
#else /* POSIX */
	/** セマフォ用ハンドル */
    XMutex         m_Mutex;
	/** コンディション値 */
    XCondition     m_Condition;
	/** カウンター */
    int            m_iValue;
#endif /* WIN32 */
	friend class XSemaphorer;
};

/**
 * 自動ロック＆アンロックヘルパー
 */
class XLIB_API XSemaphorer
{
protected:
	/** ロックオブジェクト参照値 */
	XSemaphore & m_sec;
public:
	/**
	 * コンストラクタ
	 * @param s ロックオブジェクト
	 */
	XSemaphorer(XSemaphore &s) : m_sec(s){ m_sec.Wait(); }
	/** デストラクタ */
	~XSemaphorer(void){ m_sec.Post(); }

private:
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 */
	XSemaphorer(const XSemaphore& l);
	/**
	 * コピー禁止のための対策
	 * @param l ロックオブジェクト
	 * @return 自身のインスタンス
	 */
	XSemaphorer& operator=(const XSemaphorer& l);
};


#ifdef WIN32
/**
 * 同期オブジェクトCEventをWrap
 */
class XLIB_API XEvent
{
public:
	/**
	 * コンストラクタ
	 * @param[in] bInitiallyOwn イベントオブジェクトの初期状態を指定します。
	 * @param[in] bManualReset 手動リセットオブジェクトを作成するか、自動リセットオブジェクトを作成するかを指定します。
	 * @param[in] lpszNAme 名前
	 * @param[in] lpsaAttribute セキュリティ記述子
	 */
	XEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE,
		LPCTSTR lpszNAme = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);

public:
	/**
	 * 指定されたオブジェクトをシグナル状態に設定します。
	 * @retval TRUE 成功
	 * @retval FALSE 失敗
	 */
	BOOL SetEvent();
	/**
	 * 指定されたイベントオブジェクトをシグナル状態に設定し、待機スレッドがあった場合はそのスレッドを解放し、イベントオブジェクトを非シグナル状態に再設定します。
	 * @retval TRUE 成功
	 * @retval FALSE 失敗
	 */
	BOOL PulseEvent();
	/**
	 * 指定されたイベントオブジェクトを非シグナル状態に設定します。
	 * @retval TRUE 成功
	 * @retval FALSE 失敗
	 */
	BOOL ResetEvent();
	/**
	 * アンロックします。
	 * @retval TRUE 成功
	 * @retval FALSE 失敗
	 */
	BOOL Unlock();
	/**
	 * 指定した時間ロックします
	 * @return WaitForSingleObjectの戻り値。ハンドルが無効な場合は、ERROR_INVALID_HANDLE。
	 */
	unsigned long Wait(unsigned long dwWaitTime = INFINITE);
protected:
	/** ロックオブジェクト */
	HANDLE			m_hMutex;
};
#endif /* WIN32 */

#include "xlib/xlib_tail.h"
#endif // __X_LOCK_HEASER_FILE__
