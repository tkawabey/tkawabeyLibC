/**
 * @file xsecs_comm.h
 * @brief 半導体製造SEMI規格 SECSデータ操作クラス
 *
 * @author T.Fujiha
 * @date 2007/06/29
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_SECS_COMMUNICATION_HEASER_FILE__
#define __X_SECS_COMMUNICATION_HEASER_FILE__

#include "xlib/xlib_head.h"

#include "xlib/xstrbuff.h"
#include "xlib/xsocket.h"
#include "xlib/xlock.h"
#include "xlib/xarray.h"
#include "xlib/xtime.h"
#include "xlib/semi/secs/xsecs.h"

class XLIB_API XSeceCommItf;
class XLIB_API XSecsCommHandler;


/**	@brief	SECS通信処理に対するコールバックインターフェースを定義します。	*/
class XLIB_API XSecsCommHandler
{
public:
	/**	@brief	コンストラクタ	*/
	XSecsCommHandler();
	/**	@brief	デストラクタ	*/
	virtual ~XSecsCommHandler();

	
	/**	@brief	現在のログレベルを取得します
	 *
	 *	@return	ログレベル
	 */
	XD_UINT4 getLogLevel() const;
	/**	@brief	ログレベルをセットします。
	 *
	 *	@param	level	ログレベル
	 */
	void     putLogLevel(const XD_UINT4 level);
	/**	@brief	指定されたログレベルをが設定されているか調べます。
	 *
	 *	@param	level	ログレベル
	 *
	 *	@retval	true	設定されている
	 *	@retval	false	設定されていない
	 */
	bool isSetLogLevel(long level);


// コールバック関数
	/**	@brief	ログメッセージコールバック
	 *
	 *	@param	loglevel	ログレベル
	 *	@param	pMessage	ログメッセージ
	 */
	virtual void writeLogCallBack(long loglevel, const TCHAR* pMessage);
	/**	@brief	データメッセージを取得したときコールされます。
	 *
	 *	@param	pPort	通信オブジェクト
	 *	@param	header	SECSメッセージヘッダー
	 *	@param	pMsgData	データ
	 *	@param	ulMsgSize	データサイズ
	 *
	 *	@return	bool
	 */
	virtual bool receiveDataMessageCallBack(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize) = 0;

	/**	@brief	コントロールデータを取得します。
	 *
	 *	@param	header	SECSメッセージヘッダー
	 *
	 *	@retval	true	1次メッセージの場合２次メッセージを返します。
	 *	@retval	false	1次メッセージの場合２次メッセージを返しません。
	 */
	virtual bool receiveHSMSCtrlCallBack(XSECSHeader& header) = 0;

	/**	@brief	データメッセージ送信前にコールされます。
	 *
	 *	@param	header	SECSメッセージヘッダー
	 *	@param	pMsgData	データ
	 *	@param	ulMsgSize	データサイズ
	 */
	virtual void sendDataMessageCallBack(XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);

	/**	@brief	2次メッセージ受信コールバック.sendRequestで、bWaitにfalseを指定した場合のみコールされる]
	 *
	 *	@param	pPort	通信オブジェクト
	 *	@param	header	SECSメッセージヘッダー
	 *	@param	pMsgData	データ
	 *	@param	ulMsgSize	データサイズ
	 *	 
	 *  @note	receiveDataMessageCallBack関数の後にコールされます。
	 */
	virtual void receiveResponseByNoWaitCallBack(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);
// hor HSMS
	/**	@brief	コントロールデータ送信前にコールされます。
	 *
	 *	@param	header	SECSメッセージヘッダー	
	 */
	virtual void sendHSMSCtrlCallBack(XSECSHeader& header);
	/**	@brief	ピアの接続状態が変化したことを知らせます。
	 *
	 *	@param	sts	接続ステータス
	 *
	 *	@return	virtual bool
	 */
	virtual bool connectEventCallBack(XSeceCommItf* pPort, XSECS_CONN_STS sts);


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

// SECS1
	/**	@brief	ハンドシェークコードを送信又は、受信した。
	 *	@param	bIsSend	true:送信/false:受信
	 *	@param	val		ハンドシェークコード
	 */
	virtual void SECS1_occurreHandShakeCode(bool bIsSend, XD_UINT1 val);
	/**	@brief	期待値でないハンドシェークコードを受信した
	 *	@param	val	ハンドシェークコード
	 */
	virtual void SECS1_unexpectHandShakeCode(XD_UINT1 val);
	/**	@brief	ブロックを送信･受信した。
	 *
	 *	@param	bIsSend	true:送信/false:受信
	 *	@param	header	ヘッダー
	 *	@param	pMsgData	データ
	 *	@param	ulMsgSize	データサイズ
	 */
	virtual void SECS1_occurreBlock(bool bIsSend, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);
protected:
	XD_UINT4			m_ulLogLevel;	// ログレベル
};


/**	@brief	通信インターフェースのベースクラス*/
class XLIB_API XSeceCommItf 
{
public:
	/**	@brief チケットステータス */
	enum TICKET_STATUS {
			NO_WAIT							///<	待っていない
		,	WAIT_RESERVE					///<	予約
		,	WAITTING						///<	待っている
		,	ALREADY_WAITING					///<	既に待っている
		,	TIMEO							///<	タイムアウトが発生
		,	CANCEL_WAIT						///<	waitをキャンセルした
		,	RCVED							///<	データを受信した
	};

	/**	@brief チケット情報 */
	typedef struct tagTICKET_INF
	{
		XD_UINT2		m_usTicket;			///<	チケット番号
		TICKET_STATUS	m_status;			///<	ステータス
		XD_UINT1		m_send_stream;		///<	送信ストリームー
		XD_UINT1		m_send_function;	///<	送信ファンクション
		XSECSHeader		m_resp_header;		///<	レスポンスメッセージヘッダー
		unsigned char*	m_resp_data;		///<	レスポンスメッセージデータ
		XD_UINT4		m_resp_data_size;	///<	レスポンスメッセージデータサイズ
		XTime			m_reg_time;			///<	登録された時間
		XTime			m_rcv_time;			///<	受信した時間
	}TICKET_INF;
public:
	XSeceCommItf();
	virtual ~XSeceCommItf();


	virtual bool openDevice() = 0;
	virtual bool closeDevice()= 0;
	virtual bool isOk() = 0;
	virtual XStrBuff getPeerName() = 0;
protected:
	virtual bool _readRaw(void *pData, size_t size) = 0;
	virtual bool _writeRaw(const void *pData, size_t size) = 0;
	virtual bool _writeMessage(const void *pData, size_t size) = 0;
	virtual bool innserInitilize() = 0;
	virtual void innerStateChange(XSECS_CONN_STS sts) = 0;
	virtual bool innerStartPassive() = 0;
	virtual bool innerStartActive() = 0;
	virtual void* mainProc() = 0;
	virtual void intervalProcess();

public:
// proprties
	bool get_connectDirection() const;
	XSECS_CONN_STS	 get_status() const;
	XSECS_COMMIF_TYPE getIfType() const;
	XD_UINT2 getDevaiceID() const;
	void     setDevaiceID(XD_UINT2 val);
	XD_UINT4 getOpt_T3() const ;
	void     setOpt_T3(XD_UINT4 val);
	XD_UINT4 getOpt_T6() const ;
	void     setOpt_T6(XD_UINT4 val);
	bool isEnable() const;

//member functions
	/**	@brief	1次メッセージの送信
	 *
	 *	@param	pData	送信データ
	 *	@param	bWait	2次メッセージを受信するまで待機するかどうか？
	 *					W-BITがＯＮで無い場合は無視される。
	 *	@param	pRep	2次メッセージ受け取りアドレス
	 *					W-BITがＯＮ、bWaitがtrueで無い場合は無視される。

	 *	@param	nTimeOut	タイムアウト値。-1を指定すると、Ｔ３タイムアウト値が指定される。
	 *					W-BITがＯＮ、bWaitがtrueで無い場合は無視される。
	 *
	 *  @note	1次メッセージの送信に成功して、W-BITがONの場合は、チケットが登録されます。
	 *	@return	送信ステータス。
	 */
	virtual	XSECS_SEND_STS sendRequest(XSecsData *pData, XSECSMSG* pRep = NULL, bool bWait = false, int nTimeOut = -1);
	/**	@brief	1次メッセージの送信
	 *
	 *	@param	pData	送信データ
	 *	@param	pRcvData	2次メッセージ受け取るデータのアドレス。
	 *	@param	bWait	2次メッセージを受信するまで待機するかどうか？
	 *					W-BITがＯＮで無い場合は無視される。
	 *	@param	bDropTicket	2次メッセージ受信後に、チケットを削除するかどうか？
	 *	@param	nTimeOut	タイムアウト値。-1を指定すると、Ｔ３タイムアウト値が指定される。
	 *					W-BITがＯＮ、bWaitがtrueで無い場合は無視される。
	 *
	 *  @note	1次メッセージの送信に成功して、W-BITがONの場合は、チケットが登録されます。
	 *	@return	送信ステータス。
	 */
	virtual	XSECS_SEND_STS sendRequest(XSecsData *pData, XSecsData *pRcvData, bool bWait = true, bool bDropTicket = true, int nTimeOut = -1);
	/**	@brief	1次メッセージの送信
	 *
	 *	@param	pHeader	送信メッセージのメッセージヘッダ
	 *	@param	pData	送信メッセージデータ
	 *	@param	ulDataSize	送信メッセージデータサイズ
	 *	@param	pRep	2次メッセージ受け取りアドレス
	 *					W-BITがＯＮ、bWaitがtrueで無い場合は無視される。
	 *	@param	bWait	2次メッセージを受信するまで待機するかどうか？
	 *					W-BITがＯＮで無い場合は無視される。
	 *	@param	nTimeOut	タイムアウト値。-1を指定すると、Ｔ３タイムアウト値が指定される。
	 *					W-BITがＯＮ、bWaitがtrueで無い場合は無視される。
	 *
	 *  @note	1次メッセージの送信に成功して、W-BITがONの場合は、チケットが登録されます。
	 *	@return	送信ステータス。
	 */
	virtual XSECS_SEND_STS sendRequest(XSECSHeader* pHeader, XBIN *pData, XD_UINT4 ulDataSize, XSECSMSG* pRep = NULL, bool bWait = false, int nTimeOut = -1);
	/**	@brief	2次メッセージの送信
	 *
	 *	@param	pHeader	送信メッセージのメッセージヘッダ
	 *	@param	pData	送信メッセージデータ
	 *	@param	ulDataSize	送信メッセージデータサイズ
	 *
	 *	@return	送信ステータス。	
	 */
	virtual XSECS_SEND_STS sendResponse(XSECSHeader* pHeader, XBIN *pData, XD_UINT4 ulDataSize);
	/**	@brief	2次メッセージの送信
	 *
	 *	@param	pData	送信メッセージデータ
	 *
	 *	@return	送信ステータス。	
	 */
	virtual	XSECS_SEND_STS sendResponse(XSecsData *pData);
	/**	@brief	2次メッセージのデータを取得
	 *
	 *	@param	ulTicket	チケットナンバー
	 *	@param	*ppResposeData	2次メッセージ受け取りアドレス
	 *	@param	pulResponseDataSize	2次メッセージのデータサイズを受け取るバッファ
	 *	@param	nTimeOut	タイムアウト値。-1を指定すると、Ｔ３タイムアウト値が指定される。
	 *
	 *	@return	送信ステータス。	
	 */
	virtual	XSECS_SEND_STS poll(XD_UINT4 ulTicket, XBIN **ppResposeData , XD_UINT4 *pulResponseDataSize, XD_UINT4 ulTimeO = (XD_UINT4)-1);
	/**	@brief	2次メッセージの受信をキャンセルする。
	 *
	 *	@param	ulTicket	チケットナンバー
	 *
	 *	@return	送信ステータス。		
	 */
	virtual XSECS_SEND_STS cancelWait(XD_UINT4 ulTicket);
	/**	@brief	指定したチケットのステータスを取得する。
	 *
	 *	@param	ulTicket	チケットナンバー
	 *
	 *	@return	チケットステータス。
	 */
	virtual int getTicketStatus(XD_UINT4 ulTicket);
	/**	@brief	チケットを削除する。
	 *
	 *	@param	ulTicket	チケットナンバー
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 */
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
	virtual bool getTicketInfo(XD_UINT4 ulTicket, TICKET_INF* pTicketInf);

	/**	@brief	インターフェース初期化します。
	 *
	 *	@param	bEnable	有効/無効
	 *	@param	bIsActive	Active/Pasive
	 *	@param	usDeviceID	デバイスID
	 *	@param	pHandler	ハンドラー
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 */
	bool initilize(bool bEnable, bool bIsActive, XD_UINT2 usDeviceID, XSecsCommHandler* pHandler);
	/**	@brief	インターフェースを閉じます。
	 *
	 *	@return	true	成功
	 *	@return	false	失敗	
	 */
	bool closeIf();
	/**	@brief	通信を有効にします。
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 */
	virtual bool enable();
	/**	@brief	通信を無効にします。
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 */
	virtual bool disable();
	virtual bool registT6Timer(bool bRegist);
protected:
	/*	@brief	TicketData	*/
	class TicketData {
	public:
		TicketData(XD_UINT2 usTicket, XD_UINT1 stream, XD_UINT1 function, bool bwait, XD_UINT4 ulT3);
		~TicketData();
	// property
		XD_UINT2 getTicket() const;
		bool     getWaitMode() const;
	// operation
		TICKET_STATUS wait(XD_UINT4 ulTimeOut);
		void cancelWait();
		void setReceived(unsigned char* pData, XD_UINT4 ulDataSize);



		XD_UINT2				m_usTicket;
		XD_UINT1				m_stream;
		XD_UINT1				m_function;
		bool					m_bwait;
		XBIN*					m_pRespData;
		XD_UINT4				m_ulRespDataSize;
		XTime					m_time;				// 登録された時間
		XTime					m_Rcvtime;			// 受信した時間
		XTime					m_T3TimeOutTime;	// T3タイムアウトが発生する時間
		XSemaphore				m_sync;
		XMutex					m_myLock;
		TICKET_STATUS			m_status;
		XSECSHeader				m_sendHeader;
	};
	class AnyThread : public XThread
	{
	public:
		AnyThread(XSeceCommItf* pMe, void* (*pFn)(void*))  :	m_pMe(pMe) ,	m_pFn(pFn) { }
		virtual ~AnyThread(){}
		void start() { XThread::StartUndetached(); }
	protected:
		virtual void* RunUndetached(void* pArg) { m_pFn(m_pMe); return NULL; }
		XSeceCommItf*	m_pMe;
		void*			(*m_pFn)(void*);
	};

protected:
	TicketData* addTicket(XD_UINT2 usTicket, XD_UINT1 stream, XD_UINT1 function, bool bwait);
	TicketData* findTicket(XD_UINT2 usTicket);
	TicketData* findTicketNoLock(XD_UINT2 usTicket);
private:
	// T3タイマースレッド
	static void* _T3Timer(void *pMe);
	void* T3Timer();
	static void* mainProc(void *pUserData);


// Active接続スレッド
	static void* _ActiveConnectTimer(void *pMe);
	void* ActiveConnectTimer();



protected:
	XSecsCommHandler*		m_pHandler;

	XSECS_CONN_STS			m_ststus;
	AnyThread*				m_pMainProcThread;
	AnyThread*				m_pT3TimerThread;
	XSemaphore				m_lockTimer;
	AnyThread*				m_pActiveConnetctThread;
	XSemaphore				m_lockActiveConnetct;


	bool					m_bIsActive;
	XD_UINT2				m_usCurTicket;
	XD_UINT2				m_usDeviceID;
	XD_UINT4				m_ulT3;
	XD_UINT4				m_ulT6;
	double					m_dtT6;
	XD_UINT4				m_ulActiveConnectTimeout;
	XD_UINT4				m_ulIntervalTimeout;

	bool					m_bOpend;	//  オープンしているかどうか
	bool					m_bEnable;	//	有効/無効


	XList					m_tickets;
	XMutex					m_lock;
	XMutex					m_lockTicket;
	XInflateStreamBuff		m_sendTempBuff;	//送信メッセージの一時的なバッファ

	XSECS_COMMIF_TYPE		m_ifType;

friend class TicketData;
};

#include "xlib/xlib_tail.h"
#endif /*__X_SECS_COMMUNICATION_HEASER_FILE__*/
