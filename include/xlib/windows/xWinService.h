/**
 * @file xWinService.h
 * @brief サービスコントロール
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_WIN_SERVICE_HEASER_FILE__
#define __X_WIN_SERVICE_HEASER_FILE__


#ifdef WIN32

  #ifdef __AFX_H__

  #else
    #include <windows.h>
  #endif
  #include <winsvc.h>
  #include "xlib/xstrbuff.h"
  #include "xlib/windows/xWinObject.h"

/**
 * SCマネージャ管理
 *
 */
class XWinSCManager : public XWinObject
{
public:
	/** コンストラクタ  */
	XWinSCManager();
	/** デストラクタ */
	virtual ~XWinSCManager();

	/**
	 * SCマネージャをオープンします
	 * @param[in] dwSCAccess オープンするアクセスマを指定します、
	 * @param[in] lpszMachine オープンする先がリモートマシーンの場合、マシーン名を指定します。ローカルコンピュータはNULLを指定します
	 * @param[in] lpszDataBase オープンするデータベースを指定します。デフォルトデータベースはNULLを指定します
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Open(DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const char* lpszMachine = NULL, 
				const char* lpszDataBase = NULL);
	/** Open::OpenSCManager(DWORD,const char*, const char*) */
	bool Open(DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const wchar_t* lpszMachine = NULL, 
				const wchar_t* lpszDataBase = NULL);
	/** @copydoc  XWinObject::Close() */
	virtual void Close();

protected:
	/** SCマネージャーをオープンしたマシーン */
	XStrBuff	m_strMachine;
	/** SCマネージャーをオープンしたDB */
	XStrBuff	m_strDataBase;
	/* アクセスモード */
	DWORD		m_dwAccessMode;
};

/**
 * レジストリアクセスクラス
 */
class XWinServiceCtrl
{
public:
	/** イベントログのタイプ */
	enum EVENT_KEY_PARENT { 
		/** アプリケーションログ */
		App, 
		/** セキュリティーログ */
		Sec, 
		/** システムログ */
		Sys
	};
	/** コンストラクタ */
	XWinServiceCtrl();
	/** デストラクタ */
	virtual ~XWinServiceCtrl();

	/**
	 * サービス名を取得
	 * @return サービス名
	 */
	XStrBuff GetServiceName() const;
	/**
	 * ユーザー定義のエラーコードを取得します
	 * @return ユーザー定義のエラーコード
	 */
	DWORD GetServiceSpecificExitCode() const;
	/**
	 * オープンされたSCマネージャのハンドルを取得します。
	 * @return オープンされたSCマネージャのハンドル
	 */
	HANDLE GetSCManagerHandle();

	/**
	 * SCマネージャをオープンします
	 * @param[in] dwSCAccess オープンするアクセスマを指定します、
	 * @param[in] lpszMachine オープンする先がリモートマシーンの場合、マシーン名を指定します。ローカルコンピュータはNULLを指定します
	 * @param[in] lpszDataBase オープンするデータベースを指定します。デフォルトデータベースはNULLを指定します
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool OpenSCManager(DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const char* lpszMachine = NULL, 
				const char* lpszDataBase = NULL);
	/** XWinServiceCtrl::OpenSCManager(DWORD,const char*, const char*) */
	bool OpenSCManager(DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const wchar_t* lpszMachine = NULL, 
				const wchar_t* lpszDataBase = NULL);

	/**
	 * サービスをオープンします。
	 * @param[in] lpszServiceName オープンするサービス名
	 * @param[in] dwAccess サービスのオープンするアクセスマを指定します、
	 * @param[in] dwSCAccess SCマネージャのオープンするアクセスマを指定します、
	 * @param[in] lpszMachine SCマネージャのオープンする先がリモートマシーンの場合、マシーン名を指定します。ローカルコンピュータはNULLを指定します
	 * @param[in] lpszDataBase SCマネージャのオープンするデータベースを指定します。デフォルトデータベースはNULLを指定します
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Open(	const char* lpszServiceName,
		        DWORD dwAccess = SERVICE_ALL_ACCESS ,
				DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const char* lpszMachine = NULL, 
				const char* lpszDataBase = NULL);
	/** XWinServiceCtrl::Open(const char*,DWORD,DWORD,const char*, const char*) */
	bool Open(	const wchar_t* lpszServiceName,
		        DWORD dwAccess = SERVICE_ALL_ACCESS ,
				DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const wchar_t* lpszMachine = NULL, 
				const wchar_t* lpszDataBase = NULL);
	/**
	 * サービを作成します。
	 * @note 作成する前にSCマネージャをオープンする必要が有ります
	 *       各詳細なパラメータの説明は、MSDN：CreateServiceを参照
	 * @param[in] lpszName サービス名
	 * @param[in] lpszDisplayName サービスの表示名
	 * @param[in] lpszImage サービスの実行イメージのフルパス
	 * @param[in] dwAccess サービスのオープンするアクセスマを指定します
	 * @param[in] dwServiceType サービスの種類
	 * @param[in] dwStart サービスの起動モード
	 * @param[in] dwErrorControl エラー時の処理
	 * @param[in] lpszOrderGroup オーダーグループ
	 * @param[in] lpszDependencies 依存するサービス
	 * @param[in] lpszServiceAccount 起動時のアカウント、NULLはローカルシステムアカウント
	 * @param[in] lpszPasswd lpszServiceAccountで指定したアカウントのパスワード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Create(const char* lpszName,
						const char* lpszDisplayName ,
						const char* lpszImage,
						DWORD dwAccess = SERVICE_ALL_ACCESS, 
						DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS,			
						DWORD dwStart = SERVICE_AUTO_START , 
						DWORD dwErrorControl = SERVICE_ERROR_NORMAL ,
						const char* lpszOrderGroup = NULL, 
						const char* lpszDependencies = NULL, 
						const char* lpszServiceAccount = NULL,
						const char* lpszPasswd = NULL);
	/** XWinServiceCtrl::Create(const char*,const char*,const char*,DWORD, DWORD ,DWORD ,DWORD ,const char* ,const char* ,const char* ,const char*) */
	bool Create(const wchar_t* lpszName,
						const wchar_t* lpszDisplayName ,
						const wchar_t* lpszImage,
						DWORD dwAccess = SERVICE_ALL_ACCESS, 
						DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS,			
						DWORD dwStart = SERVICE_AUTO_START , 
						DWORD dwErrorControl = SERVICE_ERROR_NORMAL ,
						const wchar_t* lpszOrderGroup = NULL, 
						const wchar_t* lpszDependencies = NULL, 
						const wchar_t* lpszServiceAccount = NULL,
						const wchar_t* lpszPasswd = NULL);
	/**
	 * サービを作成します。
	 * @note 作成する前にSCマネージャをオープンする必要が有ります
	 *       各詳細なパラメータの説明は、MSDN：CreateServiceを参照
	 * @param[in] lpszName サービス名
	 * @param[in] lpSorce サービスの設定情報構造体のアドレス
	 * @param[in] dwAccess サービスのオープンするアクセスマを指定します
	 * @param[in] lpszServiceAccount 起動時のアカウント、NULLはローカルシステムアカウント
	 * @param[in] lpszPasswd lpszServiceAccountで指定したアカウントのパスワード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Create(const char* lpszName,
						LPQUERY_SERVICE_CONFIGA lpSorce, 
						DWORD dwAccess = SERVICE_ALL_ACCESS,
						const char* lpszAccount = NULL, 
						const char* lpszPasswd = NULL);
	/** XWinServiceCtrl::Create(const char* ,LPQUERY_SERVICE_CONFIGA,DWORD ,const char* ,const char*) */
	bool Create(const wchar_t* lpszName,
						LPQUERY_SERVICE_CONFIGW lpSorce, 
						DWORD dwAccess = SERVICE_ALL_ACCESS,
						const wchar_t* lpszAccount = NULL, 
						const wchar_t* lpszPasswd = NULL);
	/**
	 * サービを削除します。
	 * @note サービスをオープンしておく必要があります。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Delete();

	/**
	 * サービスの設定を変更します。
	 * @note サービスをオープンしておく必要があります。
	 * @param[in] lpszDisplayName サービスの表示名
	 * @param[in] lpszImage サービスの実行イメージのフルパス
	 * @param[in] dwServiceType サービスの種類
	 * @param[in] dwStart サービスの起動モード
	 * @param[in] dwErrorControl エラー時の処理
	 * @param[in] lpszOrderGroup オーダーグループ
	 * @param[in] lpszDependencies 依存するサービス
	 * @param[in] lpszServiceAccount 起動時のアカウント、NULLはローカルシステムアカウント
	 * @param[in] lpszPasswd lpszServiceAccountで指定したアカウントのパスワード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool ChangeConfig( const char* lpszDisplayName = NULL,
						const char* lpszImage = NULL,
						DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS,			
						DWORD dwStart = SERVICE_AUTO_START , 
						DWORD dwErrorControl = SERVICE_ERROR_NORMAL ,
						const char* lpszOrderGroup = NULL, 
						const char* lpszDependencies = NULL, 
						const char* lpszServiceAccount = NULL,
						const char* lpszPasswd = NULL);
	/** XWinServiceCtrl::ChangeConfig(const char*, const char*, DWORD, DWORD, DWORD, const char*, const char*, const char*, const char*) */
	bool ChangeConfig( const wchar_t* lpszDisplayName = NULL,
						const wchar_t* lpszImage = NULL,
						DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS,			
						DWORD dwStart = SERVICE_AUTO_START , 
						DWORD dwErrorControl = SERVICE_ERROR_NORMAL ,
						const wchar_t* lpszOrderGroup = NULL, 
						const wchar_t* lpszDependencies = NULL, 
						const wchar_t* lpszServiceAccount = NULL,
						const wchar_t* lpszPasswd = NULL);
	/**
	 * サービスの設定を変更します。
	 * @note サービスをオープンしておく必要があります。
	 * @param[in] lpSorce サービスの設定情報構造体のアドレス
	 * @param[in] lpszAccount 起動時のアカウント、NULLはローカルシステムアカウント
	 * @param[in] lpszPasswd lpszAccountで指定したアカウントのパスワード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool ChangeConfig( LPQUERY_SERVICE_CONFIGA lpSorce, 
						const char* lpszAccount = NULL, 
						const char* lpszPasswd = NULL);
	/** XWinServiceCtrl::ChangeConfig(LPQUERY_SERVICE_CONFIGW, const wchar_t* , const wchar_t*) */
	bool ChangeConfig( LPQUERY_SERVICE_CONFIGW lpSorce, 
						const wchar_t* lpszAccount = NULL, 
						const wchar_t* lpszPasswd = NULL);
	/**
	 * オープンされたサービスに指定されたIDのコントロールハンドライベントを送信します。
	 * @note ユーザー定義のコントロールイベントIDは、１２８－２５５の間値になります
	 * @param[in] dwContorolID コントロールイベントID
	 * @param[out] pStatus ステータス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Cotrol(DWORD dwContorolID, SERVICE_STATUS *pStatus = NULL);
	/**
	 * オープンサービスを開始します。
     * @param[in] dwArgc サービスに引き渡すパラメータ数を指定します。
	 * @param[in] lpszArgv サービスに引き渡すパラメータの配列
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Start(DWORD dwArgc = 0, char** lpszArgv = NULL);
	/** XWinServiceCtrl::Start(DWORD,char**) */
	bool Start(DWORD dwArgc = 0, wchar_t** lpszArgv = NULL);
	/**
	 * サービスを停止します。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Stop();
	/**
	 * サービスを実行中あるか調べます。
	 * @retval true 実行中
	 * @retval false 停止中
	 */
	bool IsMove();
	/**
	 * サービスを停止中であるか調べます。
	 * @retval true 停止中
	 * @retval false 実行中
	 */
	bool IsStop();
	/**
	 * 指定したサービスインストールされているか調べます。
	 * @param[in] lpszServiceName サービス名
	 * @retval true インストールされている
	 * @retval false インストールされていない
	 */
	static bool IsInstall(LPCTSTR lpszServiceName);

	/**
	 * イベントログに依存するメッセージDLLを設定します。
	 */
//	DWORD SetEventKey(const char* lpszTsImage, 
//					 DWORD dwTsType = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE, 
//					 const char* lpszCaImange = NULL, 
//					 DWORD dwCaCount = 0, 
//					 DWORD dwSystemKey = XServiceCtrl::App,
//					 const char* lpszSorceName = NULL);

	/**
	 * サービスのステータスを取得します。
	 * @param[out] lpServiceStatus サービスのステータスを受け取るバッファ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool QueryServiceStatus(LPSERVICE_STATUS lpServiceStatus);
	/**
	 * サービスの設定情報を取得します。
	 * @param[out] lpServiceConfig サービスの設定情報を受け取るバッファ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool QueryServiceConfig(LPQUERY_SERVICE_CONFIGA *lpServiceConfig);
	/** XWinServiceCtrl::QueryServiceConfig(LPQUERY_SERVICE_CONFIGA*) */
	bool QueryServiceConfig(LPQUERY_SERVICE_CONFIGW *lpServiceConfig);
	/**
	 * サービスのステータス一覧を取得します。
	 * @param[out] lpServiceStatus サービスのステータスを受け取るバッファ
	 * @param[out] pDword ステータス数を受け取るバッファ
	 * @param[in] dwServiceType ステータスを取得するサービスタイプ
	 * @param[in] dwServiceState ステータスを取得するサービスステータス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool EnumServiceStatus(LPENUM_SERVICE_STATUS *lpServices, 
						   LPDWORD pDword, 
						   DWORD dwServiceType = SERVICE_WIN32, 
						   DWORD dwServiceState = SERVICE_STATE_ALL);
	/**
	 * オープンされたサービスをクローズします。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Close(void);
protected:
	SC_HANDLE		m_hSCManager;
	SC_HANDLE		m_hService;
	XStrBuff		m_strServiceName;
	DWORD			m_dwID;
	DWORD			m_dwAccess;
	DWORD			m_dwServiceSpecificExitCode;
};

#endif // WIN32
#endif //__X_WIN_SERVICE_HEASER_FILE__

