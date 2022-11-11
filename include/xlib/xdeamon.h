/**
 * @file xdeamon.h
 * @brief デーモンクラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DEAMON_HEASER_FILE__
#define __X_DEAMON_HEASER_FILE__

#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif
  #include <winsvc.h>
#endif

#include "xlib/xstrbuff.h"
#include "xlib/xarray.h"
#include "xlib/xlock.h"




class XDeamon;
 class XDeamonCompnent;

#ifdef _WIN32
	#define XDeamon_CA	__stdcall
#else
	#define XDeamon_CA	
#endif
/** デーモンのステータス */
enum XDEMON_STS {
	/** 停止 */
	XDEMON_STS_STOPPED = 1,
	/** 開始している途中 */
	XDEMON_STS_START_PENDING,
	/** 停止している途中 */
	XDEMON_STS_STOP_PENDING ,
	/** 実行中 */
	XDEMON_STS_RUNNING,
	/** 再開している途中 */
	XDEMON_STS_CONTINUE_PENDING,
	/** 一時停止している途中 */
	XDEMON_STS_PAUSE_PENDING,
	/** 一時停止 */
	XDEMON_STS_PAUSED,
};

/*
 * デーモンコンポーネントメイン関数の定義
 */
typedef void (XDeamon_CA *XDEAMON_MAIN)(unsigned long   dwNumDeamonsArgs,TCHAR  *lpDeamonArgVectors[]);
/*
 * デーモンイベントハンドラー関数の定義
 */
typedef void (XDeamon_CA *XDEAMON_FUNCTION)(unsigned long);

/**
 * デーモン管理テーブル
 */
typedef struct _XDEAMON_TABLE_ENTRY {
	/* コンポーネント名 */
    TCHAR*                      lpDeamonName;
	/* デーモンコンポーネントメイン関数 */
    XDEAMON_MAIN			    lpDeamonProc;
}XDEAMON_TABLE_ENTRY;


/**
 * コンポーネント定義マクロ
 * ME コンポーネントクラスの型
 * NAME コンポーネント名
 */
#define XDEAMON_MAINHANDLER(ME, NAME)                                              \
  TCHAR  *g_lpsz##ME##DeamonName = NAME;                                           \
  ME    g_Deamon##ME##Component;                                                   \
  void XDeamon_CA ME##DeamonMain(unsigned long dwArgc, TCHAR* lpszArgv[])  {        \
     g_Deamon##ME##Component.Main(dwArgc, lpszArgv);                               \
  }                                                                                \
  void XDeamon_CA ME##Handler(unsigned long fdwControl) {                           \
     g_Deamon##ME##Component.CtrlHandler(fdwControl);                              \
  }
/**
 * デーモン管理テーブルの定義開始
 */
#define XDEAMON_TABLSE_START()                                          \
  XDEAMON_TABLE_ENTRY g_DeamonTableEntry[] = {
/**
 * デーモン管理テーブルのコンポーネント定義
 * ME コンポーネントクラスの型
 */
#define XDEAMON_TABLSE_ENTRY(ME)                                        \
    {g_lpsz##ME##DeamonName, ME##DeamonMain},
/**
 * デーモン管理テーブルの定義終了
 */
#define XDEAMON_TABLSE_END()                                            \
    {NULL, NULL}                                                        \
  };                                                                    \
  XDeamon::g_pDemonTableEntry = g_DeamonTableEntry;
/**
 * デーモンのコンポーネントの初期化
 * ME コンポーネントクラスの型
 */
#define XDEAMON_COMPONENT_INIT(ME)                                      \
  g_Deamon##ME##Component.Initialize(g_lpsz##ME##DeamonName,            \
          ME##Handler);                                                 \
  _ASSERT(XDeamon::m_pGlobalDemon);                                     \
  XDeamon::m_pGlobalDemon->Add(&g_Deamon##ME##Component);


/* サンプル

// コンポーネントのインプリ
class Componet1 : public XDeamonCompnent, XThread 
{ 
public: 
	Componet1(); 
	virtual ~Componet1(); 
	virtual bool Setup(unsigned long argc, LPTSTR* argv){
        ...
    }
	virtual bool Stop(void){
        ...
    }
	virtual void* RunUndetached(void* pArg); 
}; 
// コンポーネントのインプリ
class Componet2 : public XDeamonCompnent, XThread 
{ 
public: 
	・・・
}; 

// グローバルのデーモンインスタンスを定義
XDeamon g_XDeamonInstance; 

// デーモンのコンポーネントの定義 
XDEAMON_MAINHANDLER(Componet1, "TesComponetName1") 
XDEAMON_MAINHANDLER(Componet2, "TesComponetName1") 

void main(int argc, char *argv[])
{
	// デーモンテーブルの定義 
	XDEAMON_TABLSE_START() 
	  XDEAMON_TABLSE_ENTRY(Componet1) 
	  XDEAMON_TABLSE_ENTRY(Componet2) 
	XDEAMON_TABLSE_END() 

	// デーモンの開始
	g_XDeamonInstance.Start(argc, argv);
}
*/

/**	エラー
*/
enum XDEMON_ERROR
{
	/** デーモンコンポーネント.登録エラー */
	XDEMON_ERROR_REGIST			= 1,
	/** デーモンコンポーネント.レポート報告エラー */
	XDEMON_ERROR_REPORT,
	/** デーモンコンポーネント.セットアップエラー */
	XDEMON_ERROR_SETUP,
};


/**
 * デーモンインスタンスクラス
 */
class XDeamon
{
public:
	/**
	 * コンポーネントスレッドデータ
	 */
	struct STARTUPINFO {
		/** デーモンコンポーネントメイン関数のアドレス */
		XDEAMON_MAIN			 lpDeamonMain;
		/** コンポーネントに渡すパラメータ数 */
		int                      argc;
		/** コンポーネントに渡すパラメータ */
		TCHAR                    **argv;
	};
	/** コンストラクタ */
	XDeamon();
	/** デストラクタ */
	virtual ~XDeamon();


	/**
	 * 指定したインデックスのコンポーネントを取得
	 *	@param	nIndes	インデックス
	 *	@return	コンポーネント
	*/
	XDeamonCompnent *GetComponent(int nIndes) const;
	/**	コンポーネント数を取得します。
	 *	@return	コンポーネント数
	*/
	int GetComponentCount() const;
	/**	コンポーネントを登録します。
	 *	@param	pConponent	登録するコンポーネント
	*/
	void Add(XDeamonCompnent *pConponent);


	/**	デーモンを開始します。
	 * @param	argc	コンポーネントに渡すパラメータ数
	 * @param	argv[]	コンポーネントに渡すパラメータ
	 * @param	bDebug	デバッグモードで開始するかどうか。
	 *					trueを指定した場合、DebugCallBakをオーバーライトし、デーモンの停止などを管理します。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	bool Start(int argc, TCHAR *argv[], bool bDebug = false);
	/**
	 * デバッグモード時の処理をオーバーライトしてください。
	 */
	virtual void DebugCallBak();

	/**	デバッグモードかどうか
	 * @retval true デバッグモードである
	 * @retval false デバッグモードでない
	*/
	bool IsDebugMode() const { return m_bDebug; }
	
	/**	@brief	シグナルを、全てのコンポーネントにイベント(CtrlHandler)にブロードキャストします。
		@param	signo	シグナルの値。OSに依存します。
	*/
	void SendBroadCastSignal(unsigned long signo);

// non scope

	/* デーモン登録テーブル */
	static XDEAMON_TABLE_ENTRY *g_pDemonTableEntry;
	/** プログラムで唯一のインスタンスのポインタ */
	static XDeamon*				m_pGlobalDemon;
protected:
	/* コンポーネント配列 */
	XArray<XDeamonCompnent*>   m_svcCompornent;
	/* デバッグフラグ */
	bool			m_bDebug;
	/* コンポーネント停止待ち */
	XSemaphore		m_semapho;
	/* 実行中のコンポーネント */
	unsigned long	m_nRunningCompornent;
};

/**
 * デーモンコンポーネントクラス
 */
class XDeamonCompnent
{
public:
	/* イベントハンドラー関数のポインタ */
	XDEAMON_FUNCTION			m_lpHandler;
	
	/**	
     * コンストラクタ
	 *	@param	dwStartWaitHint	Start時の必要とする時間をミリ秒単位で指定する。
	 *	@param	dwStopWaitHint	Stop時の必要とする時間をミリ秒単位で指定する。
	 */
	XDeamonCompnent(unsigned long dwStartWaitHint = 5000, 
				    unsigned long dwStopWaitHint = 15000);
	/** デストラクタ */
	virtual ~XDeamonCompnent();
	/**	コンポーネントの初期化
	 *	@param	lpszDeamonName	コンポーネント名
	 *	@param	pHandler	イベントハンドらー
	 */
	void Initialize(const TCHAR* lpszDeamonName, XDEAMON_FUNCTION pHandler);

	/**	@brief	コンポーネントを初期化します。
	 *  @note 派生クラスで必ずオーバーライトしてください。
	 *	@param	argc	コンポーネントに渡すパラメータ数
	 *	@param	argv[]	コンポーネントに渡すパラメータ
	 * @retval true 成功
	 * @retval false 失敗
	*/
	virtual bool Setup(unsigned long argc, TCHAR* argv[]) = 0;
	/**	@brief  コンポーネント終了時の後処理を実行します。
	*/
	virtual bool Cleanup(void) = 0;
	/**	@brief コンポーネントを停止します。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	bool Stop(void);
	/**	@brief コンポーネントを一時停止します。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	bool Pause(void);
	/**	@brief 一時停止されたコンポーネントを再開します。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	bool Continue(void);

	/**	@brief コンポーネントの停止を実装します。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	virtual bool StopImple(void);
	/**	@brief コンポーネントの一時停止を実装します。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	virtual bool PauseImple(void);
	/**	@brief コンポーネントの再開を実装します。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	virtual bool ContinueImple(void);



//	virtual unsigned long UserHandler(unsigned long cmd);
	/**	@brief	コンポーネントのメイン処理。レジスター、ステータス報告、コンポーネント終了待ちを行います
	 *	@param	dwArgc		コンポーネントに渡すパラメータ数
	 *	@param	lpszArgv	コンポーネントに渡すパラメータ
	*/
	virtual void Main(unsigned long dwArgc, TCHAR* *lpszArgv);
	/**	@brief	イベントハンドラー
	 *	@param	dwCommand	OSに依存
	 */
	virtual void CtrlHandler(unsigned long dwCommand);
	/**	@brief	コンポーネントで発生したエラーを報告します。
	 *	@param	nErrorNo	エラーNo
	 *	@param	nOsErrorNo	OS依存のエラーコード
	*/
	virtual void ErrorReport(XDEMON_ERROR nErrorNo, unsigned long nOsErrorNo);


	/**	@brief	コンポーネントの現在のステータスを取得します。
	 *	@return	ステータス
	 */
	const XDEMON_STS GetStatus(){ return m_nStatus; }
	/**	@brief	コンポーネント名を取得します。
	 *	@return	コンポーネント名
	 */
	const TCHAR* GetCompornentName(){  return m_lpszDeamonName; }
	/**	@brief	コンポーネントの開始中や停止などのステータスを報告します。
	 *	@param	dwCurrentState	現在のステータス
	 *	@param	dwWaitHint		次のステータスの変更するまでに必要な時間をミリ秒で指定します。Pendingのステータスで使用します。必要なければ０を指定します。
	 *	@param	dwSysExitCode	エラーコード。エラーがない場合は、０を指定します。
	 *	@param	dwProcessSpecificExitCode	システム依存のエラーコード。エラーがない場合は、０を指定します。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool ReportStatusChange(
					 unsigned long dwCurrentState,
					 unsigned long dwWaitHint      = 5000,
					 unsigned long dwSysExitCode = 0,
					 unsigned long dwProcessSpecificExitCode = 0);
private:
	/*	@brief	NTサービスのレジスター登録、または、シグナル登録を実行
	 * @retval true 成功
	 * @retval false 失敗
	*/
	bool RegisterHandler();

	/**	@brief	Peinding状態の途中経過を報告します。
	 *	@param	dwCurrentState	現在のステータス
	 *	@param	dwWaitHint	次のステータスの変更するまでに必要な時間をミリ秒で指定します。Pendingのステータスで使用します。必要なければ０を指定します。
	 *	@param	dwCheckPoint	チェックポイントの値
	 * @retval true 成功
	 * @retval false 失敗
	*/
	bool SetPendingStatus(unsigned long dwCurrentState,
						  unsigned long dwWaitHint = 5000,
						  unsigned long dwCheckPoint = 1);
	/*	@brief ステータスの更新
	 * @note NTサービスの場合、チェックポイントを１つインクリメントします。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	bool UpdateStatus(void);
	/*	@brief ステータスのセット。
	 * @note NTサービスの、SetServiceStatusを実行。
	 * @retval true 成功
	 * @retval false 失敗
	*/
	bool SetStatus(void);


protected:
	/* Start&Continue時の必要とする時間。ミリ秒単位。 */
	unsigned long	m_dwStartWaitHint;
	/* Stop&Pause時の必要とする時間。ミリ秒単位。 */
	unsigned long	m_dwStopWaitHint;
	/* コンポーネント名 */
	const TCHAR*    m_lpszDeamonName;
	/* 待機オブジェクト */
	XSemaphore		m_SvcWait;
	/* ステータス */
	XDEMON_STS		m_nStatus;
	/* 親インスタンス */
	XDeamon			*m_pParent;
#ifdef WIN32
	/* NTサービスのSCマネージャーハンドル */
	SC_HANDLE		m_hSCHandle;
	/* NTサービスのSCマネー通知するステータスデータ */
	SERVICE_STATUS	m_DeamonStatus;
#endif
	friend class XDeamon;
};

#endif //__X_DEAMON_HEASER_FILE__
