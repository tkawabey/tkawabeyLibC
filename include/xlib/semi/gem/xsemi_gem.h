/**
 * @file xsemi_gem.h
 * @brief SEMI:GEM実装
 *
 * @author T.Fujiha
 * @date 2007/07/15
 * @version 1.0
 *
 * Copyright (C) 2007 T.Fujiha,
 */
#ifndef __XSECS_GEM_H__
#define __XSECS_GEM_H__


#include "xlib/xlib_head.h"

#include "xlib/xeception.h"
#include "xlib/xlock.h"
#include "xlib/xmapfile.h"
#include "xlib/xtime.h"
#include "xlib/xmsgqueue.h"
#include "xlib/semi/gem/xsemi_gem_const.h"
#include "xlib/semi/secs/xsecs_comm.h"
#include "xlib/semi/secs/xsecsHSMSPort.h"
#include <vector>

class XSEMI_SV;

#define XSEMI_GEM_CTRL_STS_UNKNOWN				0	// 0
#define XSEMI_GEM_CTRL_STS_OFFLINE				1	// 1
#define XSEMI_GEM_CTRL_STS_ONLINE				2	// 2
#define XSEMI_GEM_CTRL_STS_OFFLINE_EQP			1	// 65
#define XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT		2	// 66
#define XSEMI_GEM_CTRL_STS_OFFLINE_HOST			3	// 67
#define XSEMI_GEM_CTRL_STS_ONLINE_LOCAL			4	// 129
#define XSEMI_GEM_CTRL_STS_ONLINE_REMOTE		5	// 130

#define XSEMI_GEM_SPOOL_INACTIVE                1	// notifyState:	
#define XSEMI_GEM_SPOOL_ACTIVE                  2	// notifyState:	
#define XSEMI_GEM_SPOOL_PURGE                   3	//	
#define XSEMI_GEM_SPOOL_TRANSMIT                4	//	
#define XSEMI_GEM_SPOOL_NOOUTPUT                5	//	
#define XSEMI_GEM_SPOOL_NOTFULL                 6	//	
#define XSEMI_GEM_SPOOL_FULL                    7	// notifyState:

#define XSEMI_GEM_ERR_NON_TRANSACT_DEF			0	// デフォルト処理を実行する
#define XSEMI_GEM_ERR_NON_TRANSACT_OV			1	// オーバーライトで実装した。
#define XSEMI_GEM_ERR_IN_DATA					2	// 入力データがデータが不正です。
#define XSEMI_GEM_ERR_OUT_BUFF					3	// 出力ストリームにかきこめません。
#define XSEMI_GEM_ERR_HEAD_STREAM				-2	// メッセージ・ブロック・ヘッダ内のストリーム・タイプが装置内で未定義である。
#define XSEMI_GEM_ERR_HEAD_FUNCTION				-3	// メッセージID内のファンクション・タイプが装置内で未定義である。
#define XSEMI_GEM_ERR_DATA						-4	// ストリームとファンクションは理解できたが、データ・フォーマットが解釈できない。
#define XSEMI_GEM_ERR_TRANSACT_TIMEOUT			-5	// トランザクションタイマ・タイムアウト
#define XSEMI_GEM_ERR_OVER_LENGTH				-6	// データが長すぎる
#define XSEMI_GEM_ERR_TIMEO						-7	// 会話タイムアウ
#define XSEMI_GEM_ERR_DEVAICE_ID				-9	// デバイスIDが未定義
#define XSEMI_GEM_ERR_ABORT						-10	// Abortメッセージを送信する。


#define XSEMI_INVALID_VID						(XD_UINT4)-1
#define XSEMI_INVALID_CEID						(XD_UINT4)-1
#define XSEMI_INVALID_ALID						(XD_UINT4)-1



/**	@brief	GEMクラス */
class XLIB_API XSEMI_Gem : public XSecsCommHandler, public XSEMI_GemConstObject
{
public:
	/**	@brief	通信状態モデルのステータス */
	enum COMM_STS {
		COMM_STS_DISABLE			= 0,		//<	
		COMM_STS_ENABLE				= 1,		//<	
		COMM_STS_COMUNICATION		= 2,		//<	
		COMM_STS_NOT_COMUNICATION	= 3,		//<	
		COMM_STS_WAIT_CR_FROM_HOST	= 4,		//<	
		COMM_STS_WAIT_DELAY			= 5,		//<	
		COMM_STS_WAIT_CRA			= 6,		//<	
	};

	/**	@brief	getVValueのエラーコード */
	enum GETV_STS {
		GETV_STS_SUCCESS			= 0,		//<	成功。
		GETV_STS_NOT_FOUND_VID		= 1,		//<	指定されたVIDが見つからない。
		GETV_STS_NO_IMPLE			= 2,		//< 拡張取得定義に実装が無い。
		GETV_STS_ERROR				= 100,		//< その他エラー
	};


	/**	@brief	putVValueのエラーコード */
	enum PUTV_STS {
		PUTV_STS_SUCCESS			= 0,		//<	成功。
		PUTV_STS_NOT_FOUND_VID		= 1,		//<	指定されたVIDが見つからない。
		PUTV_STS_NO_IMPLE			= 2,		//< 拡張取得定義に実装が無い。
		GETV_STS_DIFF_DATA_TYPE		= 3,		///< 指定されたデータタイプと異なる
		PUTV_STS_ERROR				= 100,		//< その他エラー
	};

	enum WELLKNOWN_V_INDEX {
		EC_INDEX_MDLN		= 0,		///<	EC.モデル
		EC_INDEX_SOFTREV,				///<	EC.ソフトリビジョン
		EC_INDEX_CONFIG_CONNECT,		///<	EC.接続確立のSxFx。0:S1F1 1:S1F65 2:S1F13
		EC_INDEX_CONFIG_ESTABLISH_COMM_DELAY,	///<	EC.接続確立のSxFxの送信周期
		EC_INDEX_INIT_COMM_STS,			///<	EC.初期通信状態(1: ENABLED/0: DISABLED)
		EC_INDEX_INIT_CTRL_STS,			///<	EC.初期コントロール状態(1:Offline/2:Online)
		EC_INDEX_ONLINE_SUBSTS,			///<	EC.OnLineサブステート(4: online/local    5: online/remote)
		EC_INDEX_OFFLINE_SUBSTS,		///<	EC.OffLineサブステート	(1: offline/equipment offline   2: offline/attempt online    3: offline/host offline)
		EC_INDEX_ONLINE_FILED,			///<	EC.OnLine失敗時の遷移ステータス(1: offline/equipment offline 3: offline/host offlin)
		EC_INDEX_POLL_DELAY,			///<	EC.S1F1の送信周期
		EC_INDEX_BUFFER_SIZE,			///<	EC.最大バッファサイズ
		// Format系
		EC_INDEX_FORMAT_VID,			///<	EC.VIDのフォーマット
		EC_INDEX_FORMAT_TRID,			///<	EC.TRIDのフォーマット
		EC_INDEX_FORMAT_RPTID,			///<	EC.RPTIDのフォーマット
		EC_INDEX_FORMAT_DSID,			///<	EC.DSIDのフォーマット
		EC_INDEX_FORMAT_DATAID,			///<	EC.DATAIDのフォーマット
		EC_INDEX_FORMAT_ALID,			///<	EC.ALIDのフォーマット
		EC_INDEX_FORMAT_CEID,			///<	EC.CEIDのフォーマット
		// 動的イベント設定/変更
		EC_INDEX_CONFIG_EVENT,			///<	EC.イベントレポートの有無
		EC_INDEX_RP_TYPE,				///<	EC.イベントレポートのレポートタタイプ
		EC_INDEX_WBIT6,					///<	EC.イベントレポートにWbitをセットするかどうか
		// Limit
		EC_INDEX_LIMIT_DELAY,			///<	EC.リミット監視データ収集間隔
		// Spool 
		EC_INDEX_CONFIG_SPOOL,			///<	EC.スプールの有無
		EC_INDEX_MAX_SPOOL_SIZE,		///<	EC.スプールするバッファの容量
		EC_INDEX_MAX_SPOOL_TRANMIT,		///<	EC.最大スプール数
		SV_INDEX_SPOOL_CNT_ACTUAL,		///<	SV.実スプール数
		SV_INDEX_SPOOL_CNT_TOTAL,		///<	SV.スプールされた数
		EC_INDEX_OV_WRITE_SPOOL,		///<	EC.SpoolFull時、スプルデータを古いデータから上書きするかどうか
		SV_INDEX_SOOL_FULLTIME,			///<	SV.SpoolFull時の時間
		SV_INDEX_SPOOL_STS,				///<	SV.Spool状態
		SV_INDEX_SPOOL_LOAD_SUBSTS,		///<	SV.SpoolLoadの状態
		SV_INDEX_SPOOL_UNLOAD_STS,		///<	SV.SpoolUnLoadの状態
		SV_INDEX_SPOOL_START_TIME,		///<	SV.Spoolを開始した時刻
		SV_INDEX_SPOOL_FILE_NAME,		///<	EC.スプールファイル名

		EC_INDEX_OTH_ACTIVE_GEM_CONNECT,	///<	EC.アクティブなGEM接続（S1F13を送信するか）を設定。

		V_INDEX_MAX
	};
	enum WELLKNOWN_EV_INDEX {
		EV_INDEX_SPOOL_ACTIVATE,
		EV_INDEX_SPOOL_DEACTIVE,
		EV_INDEX_SPOOL_TRANSMIT_FAIL,


		EV_INDEX_MAX

	};

	XSEMI_Gem();
	virtual ~XSEMI_Gem();


	bool isInitialize() const;
	bool initialize(XSEMI_GemMstData* pMasterData, bool bHost);
	XSEMI_GemConst& getConst(){ return m_constData; }


	/**	@brief	APIコマンド受信の準備をします。
	 *
	 *	@param	pInitName	初期化名
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool initAPI(const TCHAR* pInitName);
	bool release();
	bool openCommunication(XSeceCommItf* pInterface, bool bIsActive, XD_UINT2 usDeviceID);
	void closeCommunication();

	bool get_isHost() const;
	bool get_connectDirection() const;
	XD_UINT4 get_deviceID() const;
	XD_UINT4 get_SequenceDATAID();
	XD_UINT4 getOpt_T3() const;

	//
	/**	@brief	バッファサイズを取得します。
	 *
	 *	@return	バッファサイズを取得
	 */
	XD_UINT4 get_BufferSize();
// 時間
	void getGemTime(XStrBuff &strTime);
	XTime getGemTime();
	void getTimeString(XTime& obTime, XStrBuff &strTime);
// コントロール系
	XD_UINT4 get_ControlState() const;
	int chCtrlStateOffline();
	int chCtrlStateOnline();
	int chCtrlStateLocal();
	int chCtrlStateRemote();
// 通信状態系
	XSEMI_Gem::COMM_STS get_CommunicationState() const;
	int chEnable();
	int chDisable();

// 通信系
	XSECS_CONN_STS get_ConnectState() const ;
	virtual	XSECS_SEND_STS sendRequest(XSecsData *pData, XSECSMSG* pRep = NULL, bool bWait = false,  int nTimeOut = -1);
	virtual	XSECS_SEND_STS sendRequest(XSecsData *pData, XSecsData *pRcvData, bool bWait = true, bool bDropTicket = true, int nTimeOut = -1);
	virtual XSECS_SEND_STS sendRequest(XSECSHeader* pHeader, XBIN *pData, unsigned long ulDataSize, XSECSMSG* pRep = NULL, bool bWait = false, int nTimeOut = -1);

	virtual XSECS_SEND_STS sendInquire(XSECSHeader* pHeader, XBIN *pData, unsigned long ulDataSize, 
		XSECSHeader* inquireHead, unsigned long ulInqireSize = 0,		
		XBIN **ppResposeData = NULL, XD_UINT4 *pulResponseDataSize = NULL, bool bWait = false, int nTimeOut = -1);

	virtual XSECS_SEND_STS sendResponse(XSECSHeader* pHeader, XBIN *pData, unsigned long ulDataSize);
	virtual	XSECS_SEND_STS sendResponse(XSecsData *pData);
	virtual	XSECS_SEND_STS poll(XD_UINT4 ulTicket, XBIN **ppResposeData , XD_UINT4 *pulResponseDataSize, XD_UINT4 ulTimeO = (XD_UINT4)-1);
	virtual XSECS_SEND_STS cancelWait(XD_UINT4 ulTicket);
	virtual bool dropTicket(XD_UINT4 ulTicket);
	/**	@brief	チケットをが登録されているかどうか確認します。
	 *
	 *	@param	ulTicket	チケットナンバー
	 *
	 *	@return	true	チケットが登録されている
	 *	@return	false	チケットが登録されていない
	 */
	virtual bool isHaveTicket(XD_UINT4 ulTicket);
	/**	@brief	チケット情報を取得します。
	 *
	 *	@param	ulTicket	チケットナンバー
	 *	@param	pTicketInf	チケット情報を受け取るバッファ
	 *
	 *	@return	true	チケットが登録されている
	 *	@return	false	チケットが登録されていない
	 */
	virtual bool getTicketInfo(XD_UINT4 ulTicket, XSeceCommItf::TICKET_INF* pTicketInf);
// イベント
	/**	@brief	指定したCEIDのイベントを送信します。
	 *
	 *	@param	ulCEID	CEID
	 *
	 *	@retval	0	送信しました
	 *	@retval	-1	指定したCEIDが見つからない
	 *	@retval	-2	送信メッセージの作成に失敗した。
	 *	@retval	-3	メッセージを送信できなかった
	 */
	int raiseEvent(XD_UINT4 ulCEID);

	/**	@brief	Const情報で更新したイベントのレポートリンク情報をコミットします。	 */
	void commitLinkEventReport();
// アラーム
	/**	@brief	アラームを送信します。
	 *
	 *	@param	alid	アラームID
	 *	@param	alcd	アラームコード
	 *	@param	p_altx	アラームテキスト
	 *
	 *	@retval	0	成功
	 *	@retval	-1	指定されたアラームが見つからない
	 *	@retval	-2	送信メッセージの作成に失敗した。
	 *	@retval	-3	メッセージを送信できなかった
	 *	@retval	-4	指定されたアラームは無効状態
	 */
	int raiseAlarm(XD_UINT4 alid, XD_UINT1 alcd, const TCHAR* p_altx = NULL);

// スプール
	/**	@brief	スプール状態が変化した
	 *
	 *	@param	ulPreSatus	前の状態
	 *	@param	ulNewState	新しい状態	
	 */
	virtual void notifySpoolState(XD_UINT4 ulPreSatus, XD_UINT4 ulNewState);

	/**	@brief	スプール設定されたStream＆Functionの設定を取得する。
	 *
	 *	@param	stream	設定を出力するストリーム
	 *
	 *	@return	int
	 *
	 *	@note	取得できる、データ構造は、S2,F43 Reset Spooling Streams and Functions (RSSF)
	 *			と同じ形式。
	 *			構造： L,m
	 *					1. L,2
	 *						1. <U1 STRID1> * ストリーム識別
	 *						2. L,n
	 *							1. <U1 FCNID1> * ファンクション識別
	 *							:
	 *							n. <U1 FCNIDn>
	 *						:
	 *					m. L,2
	 *						1. <U1 STRIDm>
	 *						2. L,n
	 *							1. <U1 FCNID1>
	 *							:
	 *							n. <U1 FCNIDn>
	 */
	int getSpoolSettingSF(std::ostream *stream);

// V値取得・更新
	/**	@brief	WELLKNOWN_V_INDEXで指定したVIDを取得
	 *
	 *	@param	index	VIDの種類
	 *
	 *	@return	VID
	 */
	XD_UINT4 getWellKnownVID(WELLKNOWN_V_INDEX index);

	/**	@brief	名前を指定して、VIDを取得
	 *
	 *	@param	p_name	Vの名前
	 *
	 *	@return	VID	
	 */
	XD_UINT4 getVID_by_Name(const TCHAR* p_name);

	/**	@brief	指定したVの値を取得します。
	 *
	 *	@param	vid	取得するVIDを指定します。
	 *	@param	type	指定したVIDの型を受け取るバッファ
	 *	@param	stream	値をセットするストリーム
	 *
	 *	@return	取得ステータス
	 */
	GETV_STS getVValue(XD_UINT4 vid, XDATA_TYPE *type, std::ostream *stream);

	/**	@brief	指定したVの値を取得します。
	 *
	 *	@param	vid	取得するVIDを指定します。
	 *	@param	type	指定したVIDの型を受け取るバッファ
	 *	@param	writer	ライター
	 *
	 *	@return	取得ステータス
	 */
	GETV_STS getVValue(XD_UINT4 vid, XDATA_TYPE *type, XDataWriter& writer);

	/**	@brief	指定したVの値を取得します。
	 *
	 *	@param	vid	取得するVIDを指定します。
	 *	@param	type	指定したVIDの型を受け取るバッファ
	 *	@param	pValue	値を受け取るバッファ
	 *	@param	nsize	バッファのサイズ、
	 *
	 *	@return	取得ステータス
	 */
	GETV_STS getVValue(XD_UINT4 vid, XDATA_TYPE *type, void* pValue, XD_UINT4 nsize);

	/**	@brief	指定したVの値を取得します。
	 *
	 *	@param	vid	取得するVIDを指定します。
	 *	@param	rData	値を受け取るXRichData。
	 *
	 *	@return	取得ステータス
	 */
	GETV_STS getVValue(XD_UINT4 vid, XRichData& rData);

	/**	@brief	Vの値を更新する。
	 *
	 *	@param	vid	VID
	 *	@param	pValue	値
	 *	@param	nsize	pValueのサイズ
	 *
	 *	@return	設定ステータス
	 */
	PUTV_STS putVValue(XD_UINT4 vid, const void* pValue, XD_UINT4 nsize);

	/**	@brief	Vの値を更新する。
	 *
	 *	@param	vid	VID
	 *	@param	rData	設定データ
	 *
	 *	@return	設定ステータス
	 */
	PUTV_STS putVValue(XD_UINT4 vid, XRichData& rData);


// プロセス系
	/**	@brief	指定したSxFxの処理を実行し、出力ストリームの結果を書き込みます。
	 *
	 *	@param	strem	STREAM
	 *	@param	function	FUNCTION
	 *	@param	inData	入力データ
	 *	@param	pOutStream	出力ストリーム
	 *
	 *	@return	0		実装にの無いSxFxが指定された。
	 *	@return	1		処理を実行しました。
	 *	@return	2		入力データがデータが不正です。
	 *	@return	3		出力ストリームにかきこめません。
	 *	@retval	-2		メッセージ・ブロック・ヘッダ内のストリーム・タイプが装置内で未定義である。
	 *	@retval	-3		メッセージID内のファンクション・タイプが装置内で未定義である。
	 *	@retval	-4		ストリームとファンクションは理解できたが、データ・フォーマットが解釈できない。
	 *	@retval	-6		データが長すぎる
	 *
	 *	@verbatim
	 *
	 *	@endverbatim
	 */
	int processGemMessage(XD_UINT1 strem, XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream);
	int processGemMessage(XSecsData& data);
	int processGemMessage(XSecsData& data, std::ostream& pOutStream);
// コールバック
	/**	@brief	アラーム発生時に通知される
	 *
	 *	@param	alarmID	アラームID
	 *	@param	alcd	アラーム・コード
	 *	@param	pTxt	アラーム・テキスト
	 *
	 *	@retval	0	了解した
	 *	@retval	!0	了解出来ない
	 */
	virtual XD_UINT1 occurreAlarm(const XD_UINT4 alarmID, const XD_UINT1 alcd, const TCHAR* pTxt);
	/**	@brief	GetEx経由のV値取得実装を行う
	 *
	 *	@param	ceid	CEID、イベント送信経由でV値取得で無い場合は、０
	 *	@param	vid	VID
	 *	@param	type	データタイプ
	 *	@param	stream	書き込みストリーム
	 *
	 *	@return	取得ステータス
	 *
	 *	@note	typeがリストの場合は、<L. .. >　形式
	 *			typeがリストの以外の場合は、バイナリーデータそのものが設定する。
	 */
	virtual GETV_STS externGetVValue(const XD_UINT4 ceid, const XD_UINT4 vid, const XDATA_TYPE type, std::ostream *stream);

	/**	@brief	PutEx経由のV値設定実装を行う
	 *
	 *	@param	vid	VID
	 *	@param	type	データタイプ
	 *	@param	pValue	書き込むデータ
	 *	@param	ulValueSize	データサイズ
	 *
	 *	@return	設定ステータス
	 *
	 *	@note	typeがリストの場合は、<L. .. >　形式
	 *			typeがリストの以外の場合は、バイナリーデータそのものが設定する。	
	 */
	virtual PUTV_STS externPutVValue(const XD_UINT4 vid, const XDATA_TYPE type, const void* pValue, XD_UINT4 ulValueSize);

	/**	@brief	通信相手から、受信したメッセージを処理します。
	 *
	 *	@param	pPort	通信オブジェクト
	 *	@param	header	SECSメッセージヘッダー
	 *	@param	pMsgData	データ
	 *	@param	ulMsgSize	データサイズ
	 *
	 *	@retval	0		デフォルト処理を実行する。
	 *	@retval	1		オーバーライトで実装した。
	 *	@retval	-2		メッセージ・ブロック・ヘッダ内のストリーム・タイプが装置内で未定義である。
	 *	@retval	-3		メッセージID内のファンクション・タイプが装置内で未定義である。
	 *	@retval	-4		ストリームとファンクションは理解できたが、データ・フォーマットが解釈できない。
	 *	@retval	-5		トランザクションタイマ・タイムアウト
	 *	@retval	-6		データが長すぎる
	 *	@retval	-7		会話タイムアウ
	 *	@retval	-8		デバイスIDが未定義
	 *	@retval	その他	エラーが発生した。
	 */
	virtual int preReceiveDataMessage(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);

	/**	@brief	HSMSコントロールデータを取得します。
	 *
	 *	@param	header	SECSメッセージヘッダー
	 *
	 *	@retval	true	1次メッセージの場合２次メッセージを返します。
	 *	@retval	false	1次メッセージの場合２次メッセージを返しません。
	 */
	virtual bool receiveHSMSCtrlCallBack(XSECSHeader& header);

	virtual void sendDataMessageCallBack(XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);

	virtual void sendHSMSCtrlCallBack(XSECSHeader& header);

	virtual bool connectEventCallBack(XSeceCommItf* pPort, XSECS_CONN_STS sts);

	/**	@brief	コントロール状態の変更を通知します。
	 *
	 *	@param	lTransitID	指示番号。不明な場合は-1
	 *	@param	preStat	前の状態
	 *	@param	newStat	新しい状態
	 */
	virtual void notifyContorolState(long lTransitID, XD_UINT4 preStat, XD_UINT4 newStat);
	/**	@brief	T3タイムアウトが発生したことを通知します。
	 *
	 *	@param	ulTicket	チケット
	 *	@param	stream	ストリーム
	 *	@param	function	ファンクション
	 */
	virtual void t3TimeOutCallBack(XD_UINT4 ulTicket, XD_UINT1 stream, XD_UINT1 function);
	/**	@brief	T6タイムアウトが発生したことを通知します。	 */
	virtual void t6TimeOutCallBack();
	/**	@brief	インターバルタイマー処理。	 */
	virtual void intervalTimerCallBack();
	/**	@brief	通信状態の変更を通知します。
	 *
	 *	@param	lTransitID	指示番号。不明な場合は-1
	 *	@param	preStat	前の状態
	 *	@param	newStat	新しい状態
	 */
	virtual void notifyCommunicatonState(long lTransitID, COMM_STS preSts, COMM_STS newSts);
private:
	XSECS_SEND_STS innerSendRequest(XSECSHeader* pHeader, XBIN *pData, unsigned long ulDataSize, XSECSMSG* pRep = NULL, bool bWait = false, int nTimeOut = -1);
	bool innerCommunicateStateProcess(XSECS_CONN_STS sts);
	GETV_STS innerGetVValue(XD_UINT4 ceid, XD_UINT4 vid, XDATA_TYPE *type, std::ostream *stream);
	GETV_STS innerGetVValue(XD_UINT4 ceid, XD_UINT4 vid, XDATA_TYPE *type, XDataWriter& writer);
	GETV_STS innerGetVValue(XD_UINT4 ceid, XSEMI_V* pV, XDATA_TYPE *type, void* pValue, XD_UINT4 nsize);
	void innerCommunicationStateChange(COMM_STS state);
	void innerNotifyContorolState(XD_UINT4 sts);
	XD_UINT4 innerSyncroAlarm(XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);
	void innerWriteLog(XD_UINT4 level, const TCHAR* format, ...);
	// VIDやCEIDのフォーマットを取得します。
	bool get_XXID_Format(WELLKNOWN_V_INDEX emConfigVID_Index, XDATA_TYPE* pType);
	// VIDをwriterに書き込む
	bool innerWriteVID(XDATA_TYPE vidFormatType, XSEMI_V* pV, XDataWriter& writer);

	/*	@brief	データメッセージを取得したときコールされます。
	 *
	 *	@param	pPort	通信オブジェクト
	 *	@param	header	SECSメッセージヘッダー
	 *	@param	pMsgData	データ
	 *	@param	ulMsgSize	データサイズ
	 *
	 *	@return	bool
	 */
	virtual bool receiveDataMessageCallBack(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);

	/*	@brief	S1,F1 Are You There Request
	 *			S1,F3 Selected Equipment Status Request
	 *			S1,F11 Status Variable Namelist Request
	 *			S1,F13 Establish Communications Request
	 *			S1,F15 Request OFF-LINE
	 *			S1,F17 Request ON-LINE
	 */
	int processGemMessageS1(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream);
	/*	@brief	S1,F21
	 */
	int processGemMessageS1F21(XDataReader &inData, std::ostream& pOutStream);
	/*	@brief	S1,F22
	 */
	int processGemMessageS1F22(XDataReader &inData, std::ostream& pOutStream);
	/*	@brief	S1,F23
	 */
	int processGemMessageS1F23(XDataReader &inData, std::ostream& pOutStream);
	
	/*	@brief	S2,F13 Equipment Constant Request
	 *			S2,F15 New Equipment Constant Send
	 *			S2,F17 Date and Time Request
	 *			S2,F25 Loopback Diagnostic Request
	 *			S2,F29 Equipment Constant Namelist Request (ECNR)
	 *			S2,F31 Date and Time Set Request (DTS)
	 *			S2,F33 Define Report (DR)
	 *			S2,F37 Enable/Disable Event Report (EDER)
	 */
	int processGemMessageS2(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream);

	/**	@brief	S5,F1	Alarm Report Send (ARS)
	 *			S5,F3 Enable/Disable Alarm Send (EAS)
	 *			S5,F5 List Alarm Request (LAR)
	 *			S5,F7 List Enabled Alarm Request (LEAR)
	 */
	int processGemMessageS5(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream);

	/**	@brief	S6,F1 Trace Data Send (TDS)
	 *			S6,F5 Multi-block Data Send Inquire (MBI)
	 *			S6,F11 Event Report Send (ERS)
	 */
	int processGemMessageS6(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream);

	/**	@brief	S16
	 */
	int processGemMessageS16(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream);

	// S1,F65 Connect Request
	int processGemMessageS1F65(XDataReader &inData, std::ostream& pOutStream);

	// S2,F13 Equipment Constant Request
	int processGemMessageS2F13(XDataReader &inData, std::ostream& pOutStream);
	// S2,F15 New Equipment Constant Send
	int processGemMessageS2F15(XDataReader &inData, std::ostream& pOutStream);
	// S2,F17 Date and Time Request
	int processGemMessageS2F17(XDataReader &inData, std::ostream& pOutStream);
	// S2,F18 Date and Time Data (DTD)
	int processGemMessageS2F18(XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);
	// S2,F25 Loopback Diagnostic Request
	int processGemMessageS2F25(XDataReader &inData, std::ostream& pOutStream);
	// S2,F29 Equipment Constant Namelist Request (ECNR)
	int processGemMessageS2F29(XDataReader &inData, std::ostream& pOutStream);
	// S2,F31 Date and Time Set Request (DTS)
	int processGemMessageS2F31(XDataReader &inData, std::ostream& pOutStream);
	// S2,F33 Define Report (DR)
	int processGemMessageS2F33(XDataReader &inData, std::ostream& pOutStream);
	// S2,F35 Link Event Report (LER)
	int processGemMessageS2F35(XDataReader &inData, std::ostream& pOutStream);
	// S2,F37 Enable/Disable Event Report (EDER)
	int processGemMessageS2F37(XDataReader &inData, std::ostream& pOutStream);
	// S2,F39 Multi-block Inquire (DMBI)
	int processGemMessageS2F39(XDataReader &inData, std::ostream& pOutStream);
	// S2,F43 Reset Spooling Streams and Functions (RSSF)
	int processGemMessageS2F43(XDataReader &inData, std::ostream& pOutStream);

	// S5,F1	Alarm Report Send (ARS)
	int processGemMessageS5F1(XDataReader &inData, std::ostream& pOutStream);
	// S5,F3 Enable/Disable Alarm Send (EAS)
	int processGemMessageS5F3(XDataReader &inData, std::ostream& pOutStream);
	// S5,F5 List Alarm Request (LAR)
	int processGemMessageS5F5(XDataReader &inData, std::ostream& pOutStream);
	// S5,F7 List Enabled Alarm Request (LEAR)
	int processGemMessageS5F7(XDataReader &inData, std::ostream& pOutStream);


	// S6,F1 Trace Data Send (TDS)
	int processGemMessageS6F1(XDataReader &inData, std::ostream& pOutStream);
	// S6,F5 Multi-block Data Send Inquire (MBI)
	int processGemMessageS6F5(XDataReader &inData, std::ostream& pOutStream);
	// S6,F11 Event Report Send (ERS)
	int processGemMessageS6F11(XDataReader &inData, std::ostream& pOutStream);


protected:
	XSEMI_GemConst		m_constData;		// 設定値系ラップ
	XSeceCommItf*		m_pCommuIf;			// 通信インターフェース

private:
	std::ostream		m_respStream;
	XInflateStreamBuff	m_repBuff;
	XMutex				m_lock;


	XRingBuffer			m_spoolBuff;		// スプール用のバッファ

	bool				m_bInit;			// 初期化済みフラグ
	bool				m_bHost;
	XD_UINT4			m_ulDeviceID;		//	デバイスID
	XD_UINT4			m_ulSequcialDATAID;	//
	XD_UINT4			m_ulControlStatus;	// Gemコントロール状態

	XTimeNonRTC			m_time;				// 時間
//	Spool
	XD_UINT4			m_ulSpoolState;		// スプール状態
	XSEMI_Spool			*m_pobjSpool;

	COMM_STS			m_emCommState;		// 通信状態
	XTime				m_tmWaitDelay;		// WAIT_DELAYがタイムアウトする時間
	double				m_dCommDelayTime;	// 前回接続要求を送信した時間
	XD_UINT4			m_ulTiketWaitCRA;	// 

	class _API	: public XMsgQueueServer
				, public XSeceCommItf
	{
	public:
		_API(XSEMI_Gem* pGem);
		~_API();
		// overwrite XMsgQueueServer
		virtual void receiveRequest(ClinetData* pClient, XD_UINT4 ulMsgID, void *pData, size_t dataSize);
		virtual void ntfyConnection(XD_UINT8 ulClientID, bool bConnect);
		// overwrite XSeceCommItf
		virtual XStrBuff getPeerName();
		virtual bool openDevice(){ return true;}
		virtual bool closeDevice(){ return true;}
		virtual bool isOk(){ return true;}


		void receiveDataMessage(const XSECSHeader& header, const XBIN* pMsgData,const XD_UINT4 ulMsgSize);
	protected:
		virtual bool _readRaw(void *pData, size_t size){ return true;}
		virtual bool _writeRaw(const void *pData, size_t size){ return true;}
		virtual bool _writeMessage(const void *pData, size_t size);
		virtual bool innserInitilize(){ return true;}
		virtual void innerStateChange(XSECS_CONN_STS sts){}
		virtual bool innerStartPassive(){ return true;}
		virtual bool innerStartActive(){ return true;}
		virtual void* mainProc() {return 0;	}
		
		XD_UINT8			m_ulCurrentClientID;
		XD_UINT4			m_ulProccessingMsgID;
		ClinetData*			m_pProccessingClientData;
		XSEMI_Gem*			m_pGEM;
		XInflateStreamBuff	m_buffSendMsg;
	};
	_API*				m_pAPI;				// APIのコマンドを受信処理


	XD_UINT4			m_ulVIDs[V_INDEX_MAX];
	XD_UINT4			m_ulEVENTIndex[EV_INDEX_MAX];

friend class _API;
friend class XSEMI_Spool;
};


#include "xlib/xlib_tail.h"
#endif /* __XSECS_GEM_H__ */