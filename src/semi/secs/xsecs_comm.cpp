#include "xlib/xeception.h"
#include "xlib/semi/secs/xsecs_comm.h"
#include <iostream>


//
//	XSecsCommHandler
//
XSecsCommHandler::XSecsCommHandler()
	:	m_ulLogLevel(XLIB_LOG_LEVEL_NORMAL | XLIB_LOG_LEVEL_ERROR)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSecsCommHandler::~XSecsCommHandler()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XD_UINT4 XSecsCommHandler::getLogLevel() const
{
XEXCEPT_ENTRY
	return m_ulLogLevel;
XEXCEPT_EXIT
}

void XSecsCommHandler::putLogLevel(const XD_UINT4 level)
{
XEXCEPT_ENTRY
	m_ulLogLevel = level;
XEXCEPT_EXIT
}
bool XSecsCommHandler::isSetLogLevel(long level)
{
XEXCEPT_ENTRY
	return (m_ulLogLevel & level) != 0 ? true : false;
XEXCEPT_EXIT
}

void XSecsCommHandler::writeLogCallBack(long loglevel, const TCHAR* pMessage)
{
XEXCEPT_ENTRY
	
XEXCEPT_EXIT
}

void XSecsCommHandler::sendDataMessageCallBack(XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

void XSecsCommHandler::sendHSMSCtrlCallBack(XSECSHeader& header)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
void XSecsCommHandler::receiveResponseByNoWaitCallBack(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
bool XSecsCommHandler::connectEventCallBack(XSeceCommItf* pPort, XSECS_CONN_STS sts)
{
XEXCEPT_ENTRY
	return true;
XEXCEPT_EXIT
}
void XSecsCommHandler::t3TimeOutCallBack(XD_UINT4 ulTicket, XD_UINT1 stream, XD_UINT1 function)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
void XSecsCommHandler::t6TimeOutCallBack()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

void XSecsCommHandler::intervalTimerCallBack()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

void XSecsCommHandler::SECS1_occurreHandShakeCode(bool bIsSend, XD_UINT1 val)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
void XSecsCommHandler::SECS1_unexpectHandShakeCode(XD_UINT1 val)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
void XSecsCommHandler::SECS1_occurreBlock(bool bIsSend, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}




//
//	XSeceCommItf::TicketData
//
XSeceCommItf::TicketData::TicketData(XD_UINT2 usTicket, 
									 XD_UINT1 stream, 
									 XD_UINT1 function, 
									 bool bwait, 
									 XD_UINT4 ulT3)
	:	m_usTicket(usTicket)
	,	m_stream(stream)
	,	m_function(function)
	,	m_bwait(bwait)
	,	m_pRespData(NULL)
	,	m_ulRespDataSize(0)
	,	m_status( XSeceCommItf::NO_WAIT )

{
XEXCEPT_ENTRY
	m_time = XTime::getCurrentTime();

	int iHour   = ulT3 / (60*60);
	int iMinute = ulT3 / (60);
	int iSecond = ulT3 % (60);

	double dblTime;
	dblTime = (((long)iHour * 3600L) +
		((long)iMinute * 60L) +
		((long)iSecond)) / 86400.;

	double setTime = dblTime + m_time.m_dt;
	m_T3TimeOutTime = setTime;

	if( m_bwait == true ) {
		m_status = WAIT_RESERVE;
	}

#ifdef _DEBUG
	XTRACE(_T("Reg Ticket Time:  %04u/%02u/%02u %02u:%02u:%02u - %04u/%02u/%02u %02u:%02u:%02u\n"),
		m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), 
		m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond(),

		m_T3TimeOutTime.GetYear(), m_T3TimeOutTime.GetMonth(), m_T3TimeOutTime.GetDay(), 
		m_T3TimeOutTime.GetHour(), m_T3TimeOutTime.GetMinute(), m_T3TimeOutTime.GetSecond()
		);	
#endif /* _DEBUG */
XEXCEPT_EXIT
}

XSeceCommItf::TicketData::~TicketData()
{
XEXCEPT_ENTRY
	if( m_pRespData != NULL ) {
		delete []m_pRespData;
	}
XEXCEPT_EXIT
}
XD_UINT2 XSeceCommItf::TicketData::getTicket() const 
{
XEXCEPT_ENTRY
	return m_usTicket;
XEXCEPT_EXIT
}
bool XSeceCommItf::TicketData::getWaitMode() const
{
XEXCEPT_ENTRY
	return m_bwait;
XEXCEPT_EXIT
}

XSeceCommItf::TICKET_STATUS XSeceCommItf::TicketData::wait(XD_UINT4 ulTimeOut)
{
XEXCEPT_ENTRY
	{
		XMutexer locker(m_myLock);
		if( WAITTING == m_status ) {
			// 既に待っている
			return ALREADY_WAITING;
		}
		if( m_status == RCVED) {
			// 既に受信している。
			return m_status;
		}
		m_status = WAITTING;
	}
	if( m_sync.lock( ulTimeOut ) != true ) {
		XMutexer locker(m_myLock);
		if( m_status != RCVED ) {
			m_status = TIMEO;
		}
	}
	return m_status;
XEXCEPT_EXIT
}
void XSeceCommItf::TicketData::cancelWait()
{
XEXCEPT_ENTRY
	bool bWaiting = false;
	{
		XMutexer locker(m_myLock);
		if( m_status == WAITTING ) {
			m_status = CANCEL_WAIT;
			bWaiting = true;
		}
	}
	if( bWaiting == true ) {
		if( m_sync.Trywait() == 0 ) {
			m_sync.unlock();
		}
	}
XEXCEPT_EXIT
}
void XSeceCommItf::TicketData::setReceived(unsigned char* pData, XD_UINT4 ulDataSize)
{
XEXCEPT_ENTRY
	{
		XMutexer locker(m_myLock);
		if( m_pRespData != NULL ) {
			delete []m_pRespData;
		}
		m_pRespData = new unsigned char[ulDataSize];
		if( m_pRespData == NULL ) {
			return;
		}
		m_ulRespDataSize = ulDataSize;
		::memcpy(m_pRespData, pData, ulDataSize);
		if( WAITTING == m_status ) {
			m_status = RCVED;
			if( m_sync.Trywait() == 0 ) {
				m_sync.unlock();
			}
		}
		m_Rcvtime = XTime::getCurrentTime();
		m_status = RCVED;
	}
XEXCEPT_EXIT
}

//
//	XSeceCommItf
//
XSeceCommItf::XSeceCommItf()
	:	m_pHandler( NULL )

	,	m_ststus( XSECS_CONN_STS_NOT_CONNECTED )
	,	m_pMainProcThread( NULL )
	,	m_pT3TimerThread( NULL )
	,	m_pActiveConnetctThread( NULL )

	,	m_bIsActive( false )
	,	m_usCurTicket( 0 )
	,	m_usDeviceID( 0 )

	,	m_ulT3(45)
	,	m_ulT6(5)
	,	m_dtT6(0.0)

	,	m_ulActiveConnectTimeout( 10 )

	,	m_bOpend( false )
	,	m_bEnable( true )

	,	m_ifType( XSECS_COMMIF_TYPE_UNKNOWN )

	,  m_sendTempBuff(20*1024*1024, 3*1024*1024)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XSeceCommItf::~XSeceCommItf()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

bool XSeceCommItf::initilize(bool bEnable, bool bIsActive, XD_UINT2 usDeviceID, XSecsCommHandler* pHandler)
{
XEXCEPT_ENTRY

	// 既にオープンしている場合はまずクローズ！！
	this->closeIf();

	XMutexer			locker(m_lock);


	m_bEnable    = bEnable;
	m_bIsActive  = bIsActive;
	m_usDeviceID = usDeviceID;
	m_pHandler   = pHandler;


	if( m_bIsActive == true ) {

		if( openDevice() == false ) {
			m_bIsActive = false;
			m_usDeviceID = 0;
			m_pHandler = NULL;

			return false;
		}


		// アクティブ接続タイマーを起動する。
		m_pActiveConnetctThread = new AnyThread(this, _ActiveConnectTimer);
	
		m_bOpend = true;
		m_pActiveConnetctThread->start();


	} else {
		innerStartPassive();
	}

	m_bOpend = true;
	
	// T3タイマーを起動する。
	m_pT3TimerThread = new AnyThread(this, _T3Timer);
	m_pT3TimerThread->start();

	return true;
XEXCEPT_EXIT
	m_bIsActive = false;
	m_usDeviceID = 0;
	m_pHandler = NULL;
	return false;
}

bool XSeceCommItf::closeIf()
{
XEXCEPT_ENTRY
	try {
		XMutexer			locker(m_lock);

		m_bOpend = false;

		// T3タイマーの停止を待つ
		if( m_pT3TimerThread != NULL ) {
			void *pRet = NULL;
			m_lockTimer.unlock();
			m_pT3TimerThread->Join(&pRet);
			m_pT3TimerThread = NULL;
		}

		// デバイスをクロースする。
		closeDevice();

		// アクティブ接続タイマーの停止を待つ
		if( m_pActiveConnetctThread != NULL ) {
			void *pRet = NULL;
			m_lockActiveConnetct.unlock();
			m_pActiveConnetctThread->Join(&pRet);
			m_pActiveConnetctThread = NULL;
		}
		innerStateChange( XSECS_CONN_STS_NOT_CONNECTED );
		if( m_pMainProcThread != NULL ) {

		}
	} catch(...) {
#ifdef _DEBUG
		XTRACE(_T("!! Invalid Exception. %s(%d)"), (const TCHAR*)XStrBuff(__FILE__), __LINE__);
		_XASSERT(0);
#endif
		return false;		
	}
	return true;
XEXCEPT_EXIT
}
bool XSeceCommItf::get_connectDirection() const
{
XEXCEPT_ENTRY
	return m_bIsActive;
XEXCEPT_EXIT
}

XSECS_CONN_STS XSeceCommItf::get_status() const
{
XEXCEPT_ENTRY
	return m_ststus;
XEXCEPT_EXIT
}

XSECS_COMMIF_TYPE XSeceCommItf::getIfType() const
{
XEXCEPT_ENTRY
	return m_ifType;
XEXCEPT_EXIT
}
XD_UINT2 XSeceCommItf::getDevaiceID() const 
{
XEXCEPT_ENTRY
	return m_usDeviceID;
XEXCEPT_EXIT
}
void     XSeceCommItf::setDevaiceID(XD_UINT2 val)
{
XEXCEPT_ENTRY
	m_usDeviceID = val;
XEXCEPT_EXIT
}
XD_UINT4 XSeceCommItf::getOpt_T3() const
{
XEXCEPT_ENTRY
	return m_ulT3;
XEXCEPT_EXIT
}
void     XSeceCommItf::setOpt_T3(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulT3 = val;
XEXCEPT_EXIT
}
XD_UINT4 XSeceCommItf::getOpt_T6() const
{
XEXCEPT_ENTRY
	return m_ulT6;
XEXCEPT_EXIT
}
void     XSeceCommItf::setOpt_T6(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulT6 = val;
XEXCEPT_EXIT
}
bool XSeceCommItf::isEnable() const 
{
XEXCEPT_ENTRY
	return m_bEnable;
XEXCEPT_EXIT
}



XSECS_SEND_STS XSeceCommItf::sendRequest(XSecsData *pData, 
								XSECSMSG* pRep  /*= NULL*/, 
								bool bWait /*= false */, 
								int nTimeOut /*= -1*/)
{
XEXCEPT_ENTRY
	XInflateStreamBuff	buff(256);
	std::iostream		stream(&buff);
	TicketData*			pTicket = NULL;
	bool				bWBit = false;
	
	{
		XMutexer			locker(m_lock);

		m_sendTempBuff.resetp();
		std::ostream		strm(&m_sendTempBuff);

		if( isOk() == false ) {
			return XSECS_SEND_STS_F_NOT_CONNECTED;
		}
		bWBit = pData->get_wait();

		// デバイスをセットする
		pData->put_session( m_usDeviceID );
		// チケットをセット
		pData->put_ticket( m_usCurTicket++ );
		
		if( pData->buildBinaryData(stream, XSecsData::SIZE_HEAD_AND_BODY) == false ) {
			return XSECS_SEND_STS_F_DATA;
		}

		if( bWBit == true ) {
			// チケット作成
			pTicket = addTicket( pData->get_ticket(), pData->get_stream(), pData->get_function(), bWait );
			if( pTicket == NULL ) {
				return XSECS_SEND_STS_F_DATA;
			}
		}


		// データ送信
		if( m_pHandler != NULL ) {
			unsigned char* pMsg = buff.getBuff();
			unsigned long ulSize = buff.size();
			m_pHandler->sendDataMessageCallBack(pData->get_header(), pMsg+4, ulSize-4);
		}
		if( _writeMessage(buff.getBuff(), buff.size()) == false) {
			// 送信失敗
			if( pTicket != NULL ) {
				pTicket->m_status = NO_WAIT;
				this->dropTicket( pData->get_ticket() );
			}
			return XSECS_SEND_STS_F_DELIV;
		}
		if( pTicket != NULL ) {
			unsigned char* pMsg = buff.getBuff();
			::memcpy(&(pTicket->m_sendHeader), pMsg, sizeof(XSECSHeader) );
		}
	}


	if( bWait == true && bWBit == true ) {
		// 待機モードの場合
		TICKET_STATUS stsTicket = pTicket->wait( nTimeOut== -1 ? m_ulT3*1000 : nTimeOut*1000 );
		if( stsTicket == TIMEO ) {
			return XSECS_SEND_STS_F_TIMEOUT;
		} else if( stsTicket != RCVED ) {
			return XSECS_SEND_STS_F_DELIV;
		}

		if( pRep != NULL ) {
			if( pTicket->m_ulRespDataSize >= 10 ) {
				::memcpy(&pRep->header, pTicket->m_pRespData, sizeof(XSECSHeader));
				pRep->pDataWithHead = pTicket->m_pRespData;
				pRep->pData			= &pTicket->m_pRespData[10];
				pRep->ulDataSize	= pTicket->m_ulRespDataSize-10;
				pRep->ulDataSizeWithHead= pTicket->m_ulRespDataSize;
			}
		}
/*
		if( ppResposeData != NULL ) {
			*ppResposeData = pTicket->m_pRespData;
		}
		if( pulResponseDataSize != NULL ) {
			*pulResponseDataSize = pTicket->m_ulRespDataSize;
		}
*/
		return XSECS_SEND_STS_S_AND_HAS_REP;
	} else 
	return XSECS_SEND_STS_S;
XEXCEPT_EXIT
}

XSECS_SEND_STS XSeceCommItf::sendRequest(XSecsData *pData, 
										 XSecsData *pRcvData, 
										 bool bWait /*= true*/, 
										 bool bDropTicket /*= true*/, 
										 int nTimeOut /*= -1*/)
{
XEXCEPT_ENTRY
	unsigned char*	pRespData = NULL;
	XD_UINT4		ulRespSize = 0;
	XSECSMSG		rspMsg;
	XSECS_SEND_STS	sts;

	sts = this->sendRequest(pData, &rspMsg, bWait, nTimeOut);
	if( sts == XSECS_SEND_STS_S_AND_HAS_REP && pRcvData != NULL) {
		if( pRcvData->setFromBinary(rspMsg.pDataWithHead, rspMsg.ulDataSizeWithHead, XSecsData::HEAD_AND_BODY) == false ) {
			XEXCEPT_THROW_STR(XExcept_SECS, _T("SECS Data has broken."), 0, 0);
		}
	}
	return sts;
XEXCEPT_EXIT
}

XSECS_SEND_STS XSeceCommItf::sendRequest(XSECSHeader* pHeader, 
								XBIN *pData, XD_UINT4 ulDataSize, 
								XSECSMSG* pRep /*= NULL*/, 
								bool bWait /*= false*/, 
								int nTimeOut /*= -1*/)
{
XEXCEPT_ENTRY

	XD_UINT4			ulSize = 0;
	bool				bWBit = pHeader->byte2 & 0x80 ? true : false;
	TicketData*			pTicket = NULL;
	{
		XMutexer			locker(m_lock);
		m_sendTempBuff.resetp();
		std::ostream		strm(&m_sendTempBuff);

		if( isOk() == false ) {
			return XSECS_SEND_STS_F_NOT_CONNECTED;
		}

		m_usCurTicket++;

		pHeader->stype = 0;
		pHeader->sessionID = XDataCoder::_hton2( m_usDeviceID );
		XD_UINT2	usTmpHostTicket = m_usCurTicket;
		XD_UINT2	usTmpTicket     = XDataCoder::_hton2( usTmpHostTicket);
		::memcpy(pHeader->tr_id, &usTmpTicket, sizeof(usTmpTicket));


		if( bWBit == true ) {
			// チケット作成
			pTicket = addTicket( usTmpHostTicket
											, 		XSecsData::get_stream( *pHeader )
											,		XSecsData::get_function( *pHeader )
											,		bWait	
				
				);
			if( pTicket == NULL ) {
				return XSECS_SEND_STS_F_DATA;
			}
		}



		ulSize = sizeof(XSECSHeader) + ulDataSize;
		ulSize = ::htonl(ulSize);
		// データ送信
		strm.write((const char*)&ulSize, 4);
		::memcpy(pHeader->tr_id, &usTmpTicket, sizeof(XD_UINT2));
		m_usCurTicket++;
		strm.write((const char*)pHeader, sizeof(XSECSHeader));
		strm.write((const char*)pData, ulDataSize);

		// データ送信
		if( m_pHandler != NULL ) {
			unsigned char* pMsg = m_sendTempBuff.getBuff();
			unsigned long ulSize = m_sendTempBuff.size();
				
			// コールバックに渡す、XSECSHeaderは、ホストオーダーコード
			XSECSHeader headLog;
			::memcpy(&headLog, pHeader, sizeof(XSECSHeader) );
			XSecsData::generateHostCode( headLog );


			m_pHandler->sendDataMessageCallBack(headLog, pMsg+4, ulSize-4);
		}
		if( _writeMessage(m_sendTempBuff.getBuff(), m_sendTempBuff.size()) == false) {
			// 送信失敗
			if( pTicket != NULL ) {
				pTicket->m_status = NO_WAIT;
				this->dropTicket( pTicket->m_usTicket);
			}
			return XSECS_SEND_STS_F_DELIV;
		}
		::memcpy(pHeader->tr_id, &usTmpHostTicket, sizeof(usTmpHostTicket));

		if( pTicket != NULL ) {
			// T3タイムアウトのS9F9を送信するためのヘッダーを保存
			::memcpy( &(pTicket->m_sendHeader), pHeader, sizeof(XSECSHeader) );
		}
	}

	if( bWait == true && bWBit == true ) {
		// 待機モードの場合
		TICKET_STATUS stsTicket = pTicket->wait( nTimeOut== -1 ? m_ulT3*1000 : nTimeOut*1000 );
		if( stsTicket == TIMEO ) {
			return XSECS_SEND_STS_F_TIMEOUT;
		} else if( stsTicket != RCVED ) {
			return XSECS_SEND_STS_F_DELIV;
		}
		if( pRep != NULL ) {
			if( pTicket->m_ulRespDataSize >= 10 ) {
				::memcpy(&pRep->header, pTicket->m_pRespData, sizeof(XSECSHeader));
				pRep->pDataWithHead = pTicket->m_pRespData;
				pRep->pData			= &pTicket->m_pRespData[10];
				pRep->ulDataSize	= pTicket->m_ulRespDataSize-10;
				pRep->ulDataSizeWithHead= pTicket->m_ulRespDataSize;
			}
		}
/*
		if( ppResposeData != NULL ) {
			*ppResposeData = pTicket->m_pRespData;
		}
		if( pulResponseDataSize != NULL ) {
			*pulResponseDataSize = pTicket->m_ulRespDataSize;
		}
*/
		return XSECS_SEND_STS_S_AND_HAS_REP;
	}

	return XSECS_SEND_STS_S;
XEXCEPT_EXIT
}
XSECS_SEND_STS XSeceCommItf::sendResponse(XSECSHeader* pHeader, XBIN *pData, XD_UINT4 ulDataSize)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	XD_UINT4			ulSize   = 0;
	XD_UINT4			ulTicket = 0;
	XD_UINT2			usData = 0;
	bool				bWbit = pHeader->byte2 & 0x80 ? true : false;
	m_sendTempBuff.resetp();
	std::ostream		strm(&m_sendTempBuff);


	if( isOk() == false ) {
		return XSECS_SEND_STS_F_NOT_CONNECTED;
	}

	ulSize = sizeof(XSECSHeader) + ulDataSize;
	ulSize = ::htonl(ulSize);
	// データ送信
	strm.write((const char*)&ulSize, 4);

	pHeader->sessionID = ::htons(m_usDeviceID);

	::memcpy(&usData, pHeader->src_id, sizeof(XD_UINT2));
	usData = ::htons(usData);
	::memcpy(pHeader->src_id, &usData, sizeof(XD_UINT2));

	::memcpy(&usData, pHeader->tr_id, sizeof(XD_UINT2));
	usData = ::htons(usData);
	::memcpy(pHeader->tr_id, &usData, sizeof(XD_UINT2));



	strm.write((const char*)pHeader, sizeof(XSECSHeader));

	if( ulDataSize != 0 ) {
		strm.write((const char*)pData, ulDataSize);
	}
	// データ送信
	if( m_pHandler != NULL ) {
		unsigned char* pMsg = m_sendTempBuff.getBuff();
		unsigned long ulSize = m_sendTempBuff.size();

		// コールバックに渡す、XSECSHeaderは、ホストオーダーコード
		XSECSHeader headLog;
		::memcpy(&headLog, pHeader, sizeof(XSECSHeader) );
		XSecsData::generateHostCode( headLog );


		m_pHandler->sendDataMessageCallBack(headLog, pMsg+4, ulSize-4);
	}
	if( _writeMessage(m_sendTempBuff.getBuff(), m_sendTempBuff.size()) == false) {
		// 送信失敗
		return XSECS_SEND_STS_F_DELIV;
	}
	return XSECS_SEND_STS_S;
XEXCEPT_EXIT
}
XSECS_SEND_STS XSeceCommItf::sendResponse(XSecsData *pData)
{
XEXCEPT_ENTRY
	XInflateStreamBuff	buff(256);
	std::iostream		stream(&buff);
	XMutexer			locker(m_lock);

	if( isOk() == false ) {
		return XSECS_SEND_STS_F_NOT_CONNECTED;
	}

	pData->put_session(m_usDeviceID);

	if( pData->buildBinaryData(stream, XSecsData::SIZE_HEAD_AND_BODY) == false ) {
		return XSECS_SEND_STS_F_DATA;
	}

	// データ送信
	if( m_pHandler != NULL ) {
		unsigned char *pMsg = buff.getBuff();
		XD_UINT4 ulSize = buff.size();
		m_pHandler->sendDataMessageCallBack(pData->get_header(), pMsg+4, ulSize-4);
	}
	if( _writeMessage(buff.getBuff(), buff.size()) == false) {
		// 送信失敗
		return XSECS_SEND_STS_F_DELIV;
	}

	return XSECS_SEND_STS_S;
XEXCEPT_EXIT
}
XSECS_SEND_STS XSeceCommItf::poll(XD_UINT4 ulTicket, 
					XBIN **ppResposeData , 
					XD_UINT4 *pulResponseDataSize, 
					XD_UINT4 ulTimeO /*= (XD_UINT4)-1*/)
{
XEXCEPT_ENTRY
	// チケットを探す
	TicketData *pTicket = this->findTicket(ulTicket);
	if( pTicket == NULL ) {
		return XSECS_SEND_STS_F_NOT_FOUND_T;
	}
	if( ulTimeO == (XD_UINT4)-1 ) {
		ulTimeO = m_ulT3*1000;
	}
	TICKET_STATUS stsTkt = pTicket->wait( ulTimeO );
	if( TIMEO == stsTkt ) {
		return XSECS_SEND_STS_F_TIMEOUT;
	} else
	if( RCVED != stsTkt ) {
		return XSECS_SEND_STS_F_DELIV;
	}

	*ppResposeData = pTicket->m_pRespData;
	*pulResponseDataSize = pTicket->m_ulRespDataSize;

	return XSECS_SEND_STS_S_AND_HAS_REP;
XEXCEPT_EXIT	
}
XSECS_SEND_STS XSeceCommItf::cancelWait(XD_UINT4 ulTicket)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lockTicket);
	// チケットを探す
	TicketData *pTicket = this->findTicketNoLock(ulTicket);
	if( pTicket == NULL ) {
		return XSECS_SEND_STS_F_NOT_FOUND_T;
	}
	pTicket->cancelWait();
	return XSECS_SEND_STS_S;
XEXCEPT_EXIT
}
int XSeceCommItf::getTicketStatus(XD_UINT4 ulTicket)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lockTicket);
	// チケットを探す
	TicketData *pTicket = this->findTicketNoLock(ulTicket);
	if( pTicket == NULL ) {
		return -1;
	}
	return pTicket->m_status;
XEXCEPT_EXIT
}


bool XSeceCommItf::enable()
{
XEXCEPT_ENTRY
	if( m_bEnable == true ) {
		// 既に有効
		return false;
	}
	if( m_bOpend == false ) {
		m_bEnable = true;
		// 通信がまだオープンされていない場合は、ここでお終い
		innerStateChange(XSECS_CONN_STS_NOT_CONNECTED);
		return true;	
	}
	if( m_bIsActive == false ) {
		if( openDevice() == true ) {
			// 通信状態通知
			innerStateChange(XSECS_CONN_STS_ACCEPTING);
		}
	}
	m_bEnable = true;
return true;
XEXCEPT_EXIT
}

bool XSeceCommItf::disable()
{
XEXCEPT_ENTRY
	if( m_bEnable == false ) {
		// 既に無効
		return false;
	}
	m_bEnable = false;	

	if( m_bOpend == false ) {
		// 通信がまだオープンされていない場合は、ここでお終い
		innerStateChange(XSECS_CONN_STS_NOT_CONNECTED);
		return true;	
	}

	//
	//	タイマー停止
	//
	m_dtT6 = 0.0;

	// 通信がオープンされている場合はオープンする。

	if( m_bIsActive == false && isOk() == true ) {
		// パッシブモードでかつ、アクティブが接続している場合は、セパレートを送信
//		sendHSMSCtrl( XSECS_STYPE_SEPARATE_REQ, m_usCurTicket++);
		XThread::Sleep(0, 100);

		closeDevice();
	}
	if( m_bIsActive == true && isOk() == true ) {
		closeDevice();
	}
XEXCEPT_EXIT
	return true;
}


bool XSeceCommItf::dropTicket(XD_UINT4 ulTicket)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lockTicket);

	XPOSITION pos1, pos2;
	for( pos1 = m_tickets.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		if( ((TicketData*)m_tickets.GetNext( pos1 ))->getTicket() == ulTicket ) {
			TicketData* pa = (TicketData*)m_tickets.GetAt( pos2 ); // Save the old pointer for
			if( pa->m_status == WAITTING || pa->m_status == WAIT_RESERVE ) {
				// waiting中
				XTRACE(_T("XSeceCommItf::dropTicket  Waiting....(%d)\n"), ulTicket);
				return false;
			}
			m_tickets.RemoveAt( pos2 );
//			delete[] pa->m_pRespData;
			delete pa;
			XTRACE(_T("XSeceCommItf::dropTicket  Delete....(%d)\n"), ulTicket);
			return true;
			break;
		}
	}
	return false;
XEXCEPT_EXIT
}

bool XSeceCommItf::isHaveTicket(XD_UINT4 ulTicket)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lockTicket);

	XPOSITION pos1, pos2;
	for( pos1 = m_tickets.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		if( ((TicketData*)m_tickets.GetNext( pos1 ))->getTicket() == ulTicket ) {
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}

bool XSeceCommItf::getTicketInfo(XD_UINT4 ulTicket, TICKET_INF* pTicketInf)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lockTicket);

	XPOSITION pos1, pos2;
	for( pos1 = m_tickets.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		if( ((TicketData*)m_tickets.GetNext( pos1 ))->getTicket() == ulTicket ) {
			TicketData* pa = (TicketData*)m_tickets.GetAt( pos2 ); // Save the old pointer for
			
			pTicketInf->m_usTicket = pa->m_usTicket;
			pTicketInf->m_status = pa->m_status;
			pTicketInf->m_send_stream = pa->m_stream;
			pTicketInf->m_send_function = pa->m_function;
			pTicketInf->m_reg_time = pa->m_time;
			pTicketInf->m_rcv_time = pa->m_Rcvtime;
			
			if( pTicketInf->m_status == RCVED ) {
				unsigned char* p = pa->m_pRespData;
				if( pa->m_ulRespDataSize >= sizeof(XSECSHeader) ) {
					::memcpy( &(pTicketInf->m_resp_header),
						p, sizeof(XSECSHeader));

					XSecsData::generateHostCode( pTicketInf->m_resp_header );

					p += sizeof(XSECSHeader);
					pTicketInf->m_resp_data = p;
					pTicketInf->m_resp_data_size = pa->m_ulRespDataSize - sizeof(XSECSHeader) ;
				} else {
					pTicketInf->m_resp_data = NULL;
					pTicketInf->m_resp_data_size = 0;
				}
			} else {
				pTicketInf->m_resp_data = NULL;
				pTicketInf->m_resp_data_size = 0;
			}
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}

bool XSeceCommItf::registT6Timer(bool bRegist)
{
XEXCEPT_ENTRY
	if( bRegist == false ) {
		// 登録解除の場合は、0.0 にリセット
		m_dtT6 = 0.0;
		return true;
	}
	if( m_dtT6 != 0.0 ) {
		// 既に登録されている。
		return false;
	}

	XTime dltTimeObj = XTime::getCurrentTime();
	XD_UINT4 ulT6 = m_ulT6;
	// デルタ値を計算
	double dblTime;
	dblTime = (((long)ulT6 / (60*60) * 3600L) +
		((long)ulT6 / (60) * 60L) +
		((long)ulT6 % (60))) / 86400.;
	m_dtT6 = dblTime + dltTimeObj.m_dt;
	return true;
XEXCEPT_EXIT
}






XSeceCommItf::TicketData* XSeceCommItf::addTicket(XD_UINT2 usTicket, 
													XD_UINT1 stream, 
													XD_UINT1 function,
													bool bwait)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lockTicket);
	TicketData			*pNewTicket = new TicketData(usTicket, stream, function, bwait, m_ulT3);

	if( pNewTicket == NULL ) {
		return NULL;
	}
	m_tickets.AddTail( pNewTicket );

	return pNewTicket;
XEXCEPT_EXIT
}


XSeceCommItf::TicketData* XSeceCommItf::findTicket(XD_UINT2 usTicket)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lockTicket);
	XPOSITION pos = m_tickets.GetHeadPosition();

	while( pos != NULL ) {
		TicketData *p = (TicketData*)m_tickets.GetNext( pos );
		if( p->getTicket() == usTicket ) {
			return p;
		}
	}
	return NULL;
XEXCEPT_EXIT
}
XSeceCommItf::TicketData* XSeceCommItf::findTicketNoLock(XD_UINT2 usTicket)
{
XEXCEPT_ENTRY
	XPOSITION pos = m_tickets.GetHeadPosition();

	while( pos != NULL ) {
		TicketData *p = (TicketData*)m_tickets.GetNext( pos );
		if( p->getTicket() == usTicket ) {
			return p;
		}
	}
	return NULL;
XEXCEPT_EXIT
}


void* XSeceCommItf::_T3Timer(void *pMe)
{
XEXCEPT_ENTRY
	return ((XSeceCommItf*)pMe)->T3Timer();
XEXCEPT_EXIT
}

void* XSeceCommItf::T3Timer()
{
	try {
		// T3タイマー監視
		
		while( m_bOpend == true ) {
			// 5秒スリープ
			m_lockTimer.lock(5*1000);
			if( m_bOpend == false ) {
				break;
			}

			// 継承先の、インターバル処理を行う
			this->intervalProcess();
			if( m_pHandler != NULL ) {
				// ハンドラー定義の、インターバル処理を行う
				m_pHandler->intervalTimerCallBack();
			}

			if( m_dtT6 != 0.0 ) {
				XTime nowTime = XTime::getCurrentTime();
				if( nowTime.m_dt > m_dtT6 ) {
					// T6タイムアウトをオーバーしている。
					if( m_pHandler != NULL ) {
						m_pHandler->t6TimeOutCallBack();
					}
					m_dtT6 = 0.0;
				}
			}

			XMutexer			locker(m_lockTicket);

			if( m_tickets.GetCount() == 0 ) {
				continue;
			}

			XTime nowTime = XTime::getCurrentTime();


			XPOSITION pos1, pos2;
			for( pos1 = m_tickets.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
			{
				TicketData *pTiket = (TicketData*)m_tickets.GetNext( pos1 );
				if( nowTime.m_dt > pTiket->m_T3TimeOutTime.m_dt 
					&&
					pTiket->m_status != RCVED	) {
					// T3タイムアウトをオーバーしている。
					if( m_pHandler != NULL ) {
						m_pHandler->t3TimeOutCallBack(pTiket->m_usTicket, pTiket->m_stream, pTiket->m_function);
					}


					// S9F9の送信
					XSECSHeader respHead;
					XInflateStreamBuff tmpStreamBuff;
					std::ostream  tmpStream( &tmpStreamBuff );
					XDataWriter	writer( &tmpStream );

					pTiket->m_sendHeader.status++;
					writer.putValAsBinary( 
						(const unsigned char *)&(pTiket->m_sendHeader), 
						sizeof(XSECSHeader) );
					XSecsData::setSxFx(respHead, 9, 9, false);
					this->sendRequest(&respHead, tmpStreamBuff.getBuff(), tmpStreamBuff.size());


					if( pTiket->m_status == WAITTING || pTiket->m_status == WAIT_RESERVE ) {
						pTiket->cancelWait();
					} else {
						m_tickets.RemoveAt( pos2 );
						delete pTiket;
					}
				}	
			}
		}

	} catch (XExcept& ex) { 
		if( m_pHandler != NULL ) {
			XLIB_STD_STREAM_BUFFER buff;
			// ストリームにセットする
			XLIB_STD_OUT_STR_STREAM strm(&buff);	
			ex.print( strm );
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, buff.toString() );
		}
	} catch (std::bad_alloc&) { 
		if( m_pHandler != NULL ) {
			XStrBuff strErr;
			strErr.Format(_T("catch std::bad_alloc. FILE:[%s] Line:[%u]"), (const TCHAR*)XStrBuff(__FILE__), __LINE__);
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr );
		}
	} catch (std::exception&) { 
		if( m_pHandler != NULL ) {
			XStrBuff strErr;
			strErr.Format(_T("catch std::exception. FILE:[%s] Line:[%u]"), (const TCHAR*)XStrBuff(__FILE__), __LINE__);
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr );
		}
	} catch (...) { 
		if( m_pHandler != NULL ) {
			XStrBuff strErr;
			strErr.Format(_T("catch unknown. FILE:[%s] Line:[%u]"), (const TCHAR*)XStrBuff(__FILE__), __LINE__);
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr );
		}
	}
	return &m_ulT3;
}

void* XSeceCommItf::mainProc(void *pUserData)
{
XEXCEPT_ENTRY
	((XSeceCommItf*)pUserData)->mainProc();
	return 0;
XEXCEPT_EXIT
}
void XSeceCommItf::intervalProcess()
{
XEXCEPT_ENTRY
	// non
XEXCEPT_EXIT
}


void* XSeceCommItf::_ActiveConnectTimer(void *pMe)
{
XEXCEPT_ENTRY
	return ((XSeceCommItf*)pMe)->ActiveConnectTimer();
XEXCEPT_EXIT
}
void* XSeceCommItf::ActiveConnectTimer()
{
	try {
		innerStateChange(XSECS_CONN_STS_NOT_CONNECTED);

		while( m_bOpend == true ) {
			if( (	m_ststus == XSECS_CONN_STS_NOT_CONNECTED
							||
					m_ststus == XSECS_CONN_STS_DISCONNECT
				)
				
				
				&& m_bIsActive == true		// 接続モードがActive
				&& m_bEnable == true ) {	// 有効状態

					if( innerStartActive() == false ) {
						m_lockActiveConnetct.lock(m_ulActiveConnectTimeout*1000);
						continue;
					}

					// 状態変化を通知
					innerStateChange(XSECS_CONN_STS_CONNECTED);
					this->mainProc();
					if( m_bOpend == false ) {
						return NULL;
					}
					{
						XMutexer			locker(m_lock);
						closeDevice();
						// 状態変化を通知
						innerStateChange(XSECS_CONN_STS_NOT_CONNECTED);
					}
			}
			// m_ulActiveConnectTimeout 秒スリープ
			m_lockActiveConnetct.lock(m_ulActiveConnectTimeout*1000);
		}
	} catch (XExcept& ex) { 
		if( m_pHandler != NULL ) {
			XLIB_STD_STREAM_BUFFER buff;
			// ストリームにセットする
			XLIB_STD_OUT_STR_STREAM strm(&buff);	
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, buff.toString() );
		}
	} catch (std::bad_alloc&) { 
		if( m_pHandler != NULL ) {
			XStrBuff strErr;
			XStrBuff strSourceFile = (const TCHAR*)XStrBuff(__FILE__);
			strErr.Format(_T("catch std::bad_alloc. FILE:[%s] Line:[%u]"), (const TCHAR*)strSourceFile, __LINE__);
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr );
		}
	} catch (std::exception&) { 
		if( m_pHandler != NULL ) {
			XStrBuff strErr;
			XStrBuff strSourceFile = (const TCHAR*)XStrBuff(__FILE__);
			strErr.Format(_T("catch std::exception. FILE:[%s] Line:[%u]"), (const TCHAR*)strSourceFile, __LINE__);
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr );
		}
	} catch (...) { 
		if( m_pHandler != NULL ) {
			XStrBuff strErr;
			XStrBuff strSourceFile = (const TCHAR*)XStrBuff(__FILE__);
			strErr.Format(_T("catch unknown. FILE:[%s] Line:[%u]"), (const TCHAR*)strSourceFile, __LINE__);
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr );
		}
	}
	return NULL;
}




