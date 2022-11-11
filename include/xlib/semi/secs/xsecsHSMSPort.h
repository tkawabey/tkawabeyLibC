/**
 * @file XSecsHSMSPort.h
 * @brief 半導体製造SEMI規格　HSMS通信インターフェース
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_SECS_HSMS_PORT_HEASER_FILE__
#define __X_SECS_HSMS_PORT_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"
#include "xlib/xsocket.h"
#include "xlib/xlock.h"
#include "xlib/xarray.h"
#include "xlib/xtime.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/semi/secs/xsecs_comm.h"
#include <deque>
#include <vector>

class XLIB_API XSecsHSMSPortHandler;
class XLIB_API XSecsHSMSPort;



DLL_EXTERN template class XLIB_API std::vector<int*>;
// std::dequeは、dll化でEXPORTSできないので、vectorで代用する
typedef XLIB_API std::vector<int*> XSecsHSMSPortQUEUE;



/**	@brief	SEMI-HSMS通信部の実装	*/
class XLIB_API XSEMI_HSMS : public XSeceCommItf
{
public:
	/**	@brief	コンストラクタ	*/
	XSEMI_HSMS();
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_HSMS();

	XSEMI_HSMS& setAddress(XSocketAddress &address);
	XSEMI_HSMS& setSockOptReuseAddr(bool val);

	XSocketAddress& getAddress();
	XSocketAddress& getSocketName();
	XSocketAddress& getPeerSocketName();
	/**	@brief	セパレートタイムアウトであるT5タイムアウトを指定します。TCP/IP接続要求を出す間隔を秒で取得。標準値は"10"秒です。
	 *	@return	T5タイムアウト
	 */
	XD_UINT4 getOpt_T5() const ;
	/**	@brief	セパレートタイムアウトであるT5タイムアウトを設定します。TCP/IP接続要求を出す間隔を秒で取得。標準値は"10"秒です。
	 *	@param	val	T5タイムアウト
	 */
	void     setOpt_T5(XD_UINT4 val);
	/**	@brief	NOT SELECTEDタイムアウトであるT7タイムアウトを指定します。TCP/IP接続を行ってからどれぐらいSelect.reqを待つかという待ち時間を秒で指定します。標準値は"10"秒です。
	 *	@return	T7タイムアウト
	 */
	XD_UINT4 getOpt_T7() const ;
	/**	@brief	NOT SELECTEDタイムアウトであるT7タイムアウトを設定します。TCP/IP接続を行ってからどれぐらいSelect.reqを待つかという待ち時間を秒で指定します。標準値は"10"秒です。
	 *	@param	val	T7タイムアウト
	 */
	void     setOpt_T7(XD_UINT4 val);
	/**	@brief	ネットワークキャラクタ間タイムアウトであるT8タイムアウトを指定します。１つのHSMSメッセージを受信し終わるまでの時間を秒で指定します。標準値は"5"秒です。
	*	@return	T8タイムアウト
	*/
	XD_UINT4 getOpt_T8() const ;
	/**	@brief	ネットワークキャラクタ間タイムアウトであるT8タイムアウトを設定します。１つのHSMSメッセージを受信し終わるまでの時間を秒で指定します。標準値は"5"秒です。
	 *	@param	val	T8タイムアウト
	 */
	void     setOpt_T8(XD_UINT4 val);
	/**	@brief	リンクテストの送信間隔を取得します。（秒）
	 *	@return	リンクテストの送信間隔（秒）
	 */
	XD_UINT4 getOpt_LinkTest() const ;
	/**	@brief	リンクテストの送信間隔を設定します。（秒）
	 *	@param	val	リンクテストの送信間隔（秒）
	 */
	void     setOpt_LinkTest(XD_UINT4 val);

	virtual XStrBuff getPeerName();
	virtual bool openDevice();
	virtual bool closeDevice();
	virtual bool isOk();


	XSocket& getCurSokecket() { return m_curSock; }
protected:
	virtual bool _readRaw(void *pData, size_t size);
	virtual bool _writeRaw(const void *pData, size_t size);
	virtual bool _writeMessage(const void *pData, size_t size);
	virtual bool innserInitilize();
	virtual void innerStateChange(XSECS_CONN_STS sts);
	virtual bool innerStartPassive();
	virtual bool innerStartActive();
	virtual void* mainProc();
	virtual void intervalProcess();

	virtual bool sendHSMSCtrl(XSECS_SType type, XD_UINT2 ulTicket);
private:
	static void* serverAcceptProc(XSocketClinet* pClientSock, void *pUserData);
	void* serverAcceptProc(XSocketClinet* pClientSock);
protected:
	bool			m_bSockOpt_ReUseAddr;


	XSocket*		m_pSocket;
	XSocket*		m_pCurSocket;
	XSocket			m_curSock;
	XSocketAddress	m_activeAddress;



	XD_UINT4		m_ulT7;
	XD_UINT4		m_ulT8;
	XD_UINT4		m_ulLinkTest;
	double			m_dtLinkTest;

	XSocketAddress			m_sockName;
	XSocketAddress			m_peerName;
	

};

//
//class XLIB_API XSecsHSMSPort 
//{
//public:
//	XSecsHSMSPort();
//	virtual ~XSecsHSMSPort();
//	virtual bool open(XSocketAddress &address, bool bIsActive, XD_UINT2 usDeviceID, XSecsCommHandler* pHandler);
//	virtual bool close();
//	bool enable();
//	bool disable();
//
//
//	virtual	XSECS_SEND_STS sendRequest(XSecsData *pData, bool bWait = false, unsigned char **ppResposeData = NULL, XD_UINT4 *pulResponseDataSize = NULL);
//	virtual	XSECS_SEND_STS sendRequest(XSecsData *pData, XSecsData *pRcvData, bool bWait = true, bool bDropTicket = true);
//	virtual XSECS_SEND_STS sendRequest(XSECSHeader* pHeader, unsigned char *pData, unsigned long ulDataSize, unsigned char **ppResposeData = NULL, XD_UINT4 *pulResponseDataSize = NULL);
//	virtual XSECS_SEND_STS sendResponse(XSECSHeader* pHeader, unsigned char *pData, unsigned long ulDataSize);
//	virtual	XSECS_SEND_STS sendResponse(XSecsData *pData);
//
//	virtual bool dropTicket(XD_UINT4 ulTicket);
//
//	virtual bool registT6Timer(bool bRegist);
//// proprty
//	bool get_connectDirection() const;
//	XSECS_CONN_STS	 get_status() const;
//
//
//// SocketOption
//	void setSocketOptReUseAddress(const bool bVal);
//	bool getSocketOptReUseAddress() const;
//
//
//
//protected:
//	enum TICKET_STATUS {
//		NO_WAIT,
//		WAIT,
//	};
//	class TicketData {
//	public:
//		TicketData(XD_UINT4 ulTicket, XD_UINT1 stream, XD_UINT1 function, bool bwait, XD_UINT4 ulT3);
//
//		XD_UINT4 getTicket(){ return m_ulTicket; }
//		XD_UINT4				m_ulTicket;
//		XD_UINT1				m_stream;
//		XD_UINT1				m_function;
//		bool					m_bwait;
//		unsigned char*			m_pRespData;
//		XD_UINT4				m_ulRespDataSize;
//		XTime					m_time;				// 登録された時間
//		XTime					m_T3TimeOutTime;	// T3タイムアウトが発生する時間
//		XSemaphore				m_sync;
//		TICKET_STATUS			m_status;
//	};
//	class AnyThread : public XThread
//	{
//	public:
//		AnyThread(XSecsHSMSPort* pMe, void* (*pFn)(void*)) 
//			:	m_pMe(pMe)
//			,	m_pFn(pFn)
//		{
//		}
//		virtual ~AnyThread(){}
//		void start() { XThread::StartUndetached(); }
//
//	protected:
//		virtual void* RunUndetached(void* pArg)
//		{
//			m_pFn(m_pMe); return NULL;
//		}
//		XSecsHSMSPort*	m_pMe;
//		void*			(*m_pFn)(void*);
//	};
//
//protected:
//	virtual void innerStateChange(XSECS_CONN_STS sts);
//
//private:
//
//	static void* serverAcceptProc(XSocketClinet* pClientSock, void *pUserData);
//	void* serverAcceptProc(XSocketClinet* pClientSock);
//	static void* mainProc(void *pUserData);
//	void* mainProc();
//// T3タイマースレッド
//	static void* _T3Timer(void *pMe);
//	void* T3Timer();
//// Active接続スレッド
//	static void* _ActiveConnectTimer(void *pMe);
//	void* ActiveConnectTimer();
//
//	virtual bool sendHSMSCtrl(XSECS_SType type, XD_UINT2 ulTicket);
//	TicketData* addTicket(XD_UINT4 ulTicket, XD_UINT1 stream, XD_UINT1 function, bool bwait);
//	TicketData* findTicket(XD_UINT4 ulTicket);
//
//
//protected:
//	bool			m_bSockOpt_ReUseAddr;
//
//
//	XSECS_CONN_STS			m_ststus;
//	AnyThread*		m_pMainProcThread;
//	AnyThread*		m_pT3TimerThread;
//	AnyThread*		m_pActiveConnetctThread;
//	XSocket*		m_pSocket;
//	XSocketAddress	m_activeAddress;
//
//	bool			m_bIsActive;
//	XD_UINT2		m_usCurTicket;
//	XD_UINT2		m_usDeviceID;
//
//	XD_UINT4		m_ulT3;
//	XD_UINT4		m_ulT5;
//	XD_UINT4		m_ulT6;
//	XD_UINT4		m_ulT7;
//	double			m_dtT6;
//
//
//	bool			m_bEnable;	//	有効/無効
//
//
//	XSecsHSMSPortQUEUE		m_msgQueue;
//	XSocketAddress			m_sockName;
//	XSocketAddress			m_peerName;
//	XList					m_tickets;
//	XMutex					m_lock;
//	XMutex					m_lockTicket;
//	XSecsCommHandler*		m_pHandler;
//friend class TicketData;
//};

#include "xlib/xlib_tail.h"
#endif /* __X_SECS_HSMS_PORT_HEASER_FILE__ */
