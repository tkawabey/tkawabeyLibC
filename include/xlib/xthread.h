/**
 * @file xthread.h
 * @brief スレッド
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_THREAD_HEADER_FILE__
#define __X_THREAD_HEADER_FILE__
#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"
#include "xlib/xlock.h"
#include "xlib/xeception.h"

#ifdef WIN32

#else /* POSIX */
   #include <pthread.h>
	extern "C" void* XThreadWrapper(void* ptr);
#endif /* WIN32 */


/**
 * @brief	XThredExceptのgetErrorCodeメンバで受け取るエラーコード
 */
enum XThredERROR
{
	XThredERROR_SUCCESS = 0,					/** 成功 */
	XThredERROR_CREATE_SEM,						/** セマフォの作成に失敗 */
	XThredERROR_UN_STS_NEW,						/** このオペレーションは、ステータスがSTATE_NEWでなければいけない */
	XThredERROR_UN_STS_RUNNING,					/** このオペレーションは、ステータスがSTATE_RUNNINGでなければいけない */
	XThredERROR_UN_STS_RUNNING_TERMINATED,		/** このオペレーションは、ステータスがSTATE_RUNNINGまたは、TERMINATEDでなければいけない */

	XThredERROR_CREATE_THREAD,					/** スレッドの作成に失敗 */
	XThredERROR_SET_PRIORITY,					/** プライオリティを完了できません */
	XThredERROR_UN_INIT,						/** インスタンスの初期化が完了してません */
	XThredERROR_JOIN,							/** JOINを完了できません */
	XThredERROR_WAIT,							/** WAITを完了できません */
	XThredERROR_SIGNAL,							/** SIGNALを完了できません */
	XThredERROR_BROAD_CAST,						/** BroadCastを完了できません */
	XThredERROR_YIELD,							/** YIELDを完了できません */
	XThredERROR_SLEEP,							/** Sleepを完了できません */
	XThredERROR_SELF,							/** Selfを完了できません */
	XThredERROR_GETTIME,						/** Selfを完了できません */
	XThredERROR_INIT,							/** XThread::InitTを完了できません */
	XThredERROR_NO_SUPPORT,						/** システムでこの機能をサポートしていません */

};

class XLIB_API XThreadCondition;
class XLIB_API XThread;



/**
 * @brief	スレッドクラス
 */
class XLIB_API XThread
{
public:
	/** @brief	スレッドのプライオリティ */
    enum priority_t {
		PRIORITY_LOW,		/** LOW */
		PRIORITY_NORMAL,	/** NORMAL */
		PRIORITY_HIGH		/** HIGH */
    };

	/** @brief	スレッドのステータス */
    enum state_t {
		STATE_NEW,			/** スレッドは開始されていない */
		STATE_RUNNING,		/** スレッド実行中 */
		STATE_TERMINATED	/** スレッド終了 */
    };

	/**
	 *  @brief	コンストラクタ
	 * @param pFnc  スレッドがコールされる関数へのポインタ
	 * @param pArgs スレッドに渡すパラメータ
	 * @param pri プライオリティ
	 */
    XThread(void (*pFnc)(void*), void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/**
	 *  @brief	コンストラクタ
	 * @param pFnc  スレッドがコールされる関数へのポインタ
	 * @param pArgs スレッドに渡すパラメータ
	 * @param pri プライオリティ
	 */
    XThread(void* (*pFnc)(void*), void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/**
     * @brief	スレッドを開始します
	 */
    void Start();
protected:
	/**
	 * @brief	スレッドのプライオリティ
	 * @param pArgs スレッドに渡すパラメータ
	 * @param pri スレッドプライオリティー
	 */
    XThread(void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/** @brief	デストラクタ */
    virtual ~XThread(void);
	/**
	 * @brief	RunUndetachedがコールされます。
	 */
    void StartUndetached(void);
public:
	/**
	 * @brief	ジョインします。
	 * @param status 渡す値
	 */
    void Join(void** status);
	/**
	 * @brief	プライオリティをセットします
	 * @param pti プライオリティ
	 */
    void SetPriority(priority_t pti);
	/**
     * @brief	スレッドを作成します。
	 * @param pFn  スレッドがコールされる関数へのポインタ
	 * @param pArgs スレッドに渡すパラメータ
	 * @param pri プライオリティ
	 * @return 新しいスレッドオブジェクトインスタンスアドレス
	 */
    static XThread* Create(void (*pFn)(void*), void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/**
     * @brief	スレッドを作成します。
	 * @param pFn  スレッドがコールされる関数へのポインタ
	 * @param pArgs スレッドに渡すパラメータ
	 * @param pri プライオリティ
	 * @return 新しいスレッドオブジェクトインスタンスアドレス
	 */
    static XThread* Create(void* (*pFn)(void*), void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/**
	 * @brief	スレッドローカルストレージにセットされたスレッドを終了します
	 * @param RetVale スレッドの終了コード
	 * @param bCallToEndThread EndThreadをコールするかどうか
	 */
    static void Exit(void* RetVale = NULL, bool bCallToEndThread = true);
	/**
	 * @brief	スレッドローカルストレージにセットされたスレッドを取得します。
	 * @return 現在のスレッドから、別スレッドにCPUを割り当てます
	 */
    static XThread* Self(void);
	/**
	 * @brief	現在のスレッドから、別スレッドにCPUを割り当てます
	 */
	static void yield(void);
	/**
	 * @brief	指定した時間待機します
	 * @param secs ミリ秒を指定します
	 * @param nanosecs ナノ秒を指定します
	 */
    static void Sleep(unsigned long secs, unsigned long nanosecs = 0);
	/**
	 * @brief	ミリ秒とナノ秒の時間を計算した値を取得しまう。カレント時間を比較したオフセットが与えられます。
	 * @param abs_sec オフセットを取る秒
	 * @param abs_nsec オフセットを取るナノ秒
	 * @param rel_sec 実際の秒
	 * @param rel_nsec 実際のナノ秒
	 */
    static void GetTime(unsigned long* abs_sec, unsigned long* abs_nsec,
			 unsigned long rel_sec = 0, unsigned long rel_nsec=0);
protected:
	/**
	 * @brief	派生クラスで、スレッドに実行させる内容をオーバライドします。Startをコールすることによって呼び出されます。
	 * @param pArg コンストラクタで渡されたデータ
	 */
    virtual void Run(void* pArg) {}
	/**
	 * @brief	派生クラスで、スレッドに実行させる内容をオーバライドします。StartUnterachedをコールすることによって呼び出されます。
	 * @param pArg コンストラクタで渡されたデータ
	 * @return スレッドの戻り値
	 */
    virtual void* RunUndetached(void* pArg) { return NULL; }
	/*
	 * @brief	コンストラクタで渡されるデータ
	 * @param pArg スレッドに渡すパラメータ
	 * @param pri プライオリティ
	 * @param nDet プライオリティ
	 */
    void CommonConstructor(void* pArg, priority_t pri, int nDet);

	/* @brief	内部メンバアクセス用オブジェクト */
	XLock			m_mutex;
	/* @brief	スレッドステータス */
    state_t			m_state;
	/* @brief	スレッドプライオリティー */
    priority_t		m_priority;
 
	static XLock* m_NextIdMutex;
    static int		m_NextId;
    int				m_nId;
    void			(*m_pFnVoid)(void*);
    void*			(*m_pFnRet)(void*);
    void*			m_pThreadArg;
    int				m_nDetached;
public:
	/**
	 * @brief	プライオリティを取得します
	 * @return プライオリティ
	 */
    priority_t priority(void) {
		XLocker l(m_mutex);
		return m_priority;
    }
	/**
	 * @brief	ステータスを取得します
	 * @return ステータス
	 */
    state_t state(void)  {
		XLocker l(m_mutex);
		return m_state;
    }
	/**
	 * @brief	IDを取得します
	 * @return スレッドID
	 */
    int id(void) { return m_nId; }

	/*
	 * @brief	XThreadで使用すするTLSを初期化します
	 */
    class InitT {
		/* @brief	カウンター */
		static int m_count;
	public:
		/* @brief	コンストラクター */
		InitT(void);
		/* @brief	ですトラクター */
		~InitT(void);
    };
    friend class InitT;
private:
#ifdef WIN32
	/* @brief	スレッドハンドル */
	HANDLE		m_hThread;
	/* @brief	NTスレッドID */
    DWORD      m_dwNtId;
	/* @brief	戻り値 */
    void*      m_pReturnVal;
	/* @brief	セマフォ用ハンドル */
    HANDLE     m_hCondSemaphore;
	/* @brief	次のスレッド */
    XThread*   m_pCondNext;
	/* @brief	前のスレッド */
    XThread*   m_pCcondPrev;
	/* @brief	スレッド配列ロック */
    BOOL       m_bCondWaiting;
	
    static int NTpriority(priority_t);
    friend class XThreadCondition;
    friend unsigned __stdcall XThreadWrapper(void* ptr);
#else /* POSIX */
	pthread_t		m_hThread;
	static int PosixPriority(priority_t npriority);
	friend void* XThreadWrapper(void* ptr);
#endif /* WIN32 */

};



class XLIB_API XThreadCondition
{
	XLock   *m_pLock;
public:
	XThreadCondition(XLock *pLock);
	~XThreadCondition();

    void Wait(void);
	// ｺﾝﾃﾞｨｼｮﾝの値がｼｸﾞﾅﾙになるまでWAIT。
    int TimedWait(unsigned long abs_sec, unsigned long abs_nsec = 0);
	// 指定された時間まで、WAIT
    void Signal(void);

    void BroadCast(void);
private:
    XThreadCondition(const XThreadCondition&);
    XThreadCondition& operator=(const XThreadCondition&);

private:
#ifdef _WIN32
	CRITICAL_SECTION      m_crit;
	XThread*              m_pWaitingHead;
	XThread*              m_pWaitingTail;
#else /* POSIX */
    pthread_cond_t posix_cond;
#endif /* WIN32 */
};


#include "xlib/xlib_tail.h"
#endif // __X_THREAD_HEADER_FILE__
