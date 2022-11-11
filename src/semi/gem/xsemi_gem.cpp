#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/semi/gem/xsemi_gem_def.h"
#include "xlib/semi/gem/xsemi_gem_api.h"
#include "xlib/xfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <strstream>




XSEMI_Gem::XSEMI_Gem()
	:	XSEMI_GemConstObject( XSEMI_GemConstObject::GEM )
	,	m_respStream(&m_repBuff)
	,	m_pCommuIf( NULL )


	,	m_bInit(false)
	,	m_bHost(false)
	,	m_ulDeviceID(0)
	,	m_ulSequcialDATAID( 1 )
	,	m_ulControlStatus(  XSEMI_GEM_CTRL_STS_UNKNOWN )

	,	m_emCommState(	XSEMI_Gem::COMM_STS_NOT_COMUNICATION	)
	,	m_dCommDelayTime( 0.0 )
// Spool
	,	m_ulSpoolState( XSEMI_GEM_SPOOL_INACTIVE )
	,	m_pobjSpool(NULL)

	,	m_pAPI( NULL )
{
XEXCEPT_ENTRY
	m_constData.m_pParentObject = this;

	memset(m_ulVIDs, 0xFF, sizeof(m_ulVIDs));
	memset(m_ulEVENTIndex, 0xFF, sizeof(m_ulEVENTIndex));

	m_pobjSpool = new XSEMI_Spool(m_constData.get_SpoolSFs());
	m_pobjSpool->setGem( this );

XEXCEPT_EXIT
}


XSEMI_Gem::~XSEMI_Gem()
{
XEXCEPT_ENTRY
	if( m_pobjSpool != NULL ) {
		delete m_pobjSpool;
	}
	if( m_pAPI != NULL ) {
		// APIを閉じる
		m_pAPI->close();
		m_pAPI->Join(0);
		m_pAPI = NULL;
	}
XEXCEPT_EXIT
}

bool XSEMI_Gem::isInitialize() const
{
XEXCEPT_ENTRY
	return m_bInit;
XEXCEPT_EXIT
}

bool XSEMI_Gem::initialize(XSEMI_GemMstData* pMasterData, bool bHost)
{
XEXCEPT_ENTRY
	XStrBuff	strErrMsg;
	XFileName	fileName;
	XFileStatus fileSts;
	XRichData	confValue(XDATA_TYPE_L);
	bool		bExistFile = false;
	int			err = 0;
	// 自身がHOSTとして動作するかどうか？
	m_bHost = bHost;

	try {

		// メタデータのセット
		if( m_constData.initialize( pMasterData ) == false ) {
			return false;
		}

		//-----------------------------------------------------------------
		// 以下、設定情報の確定
		//-----------------------------------------------------------------
		XSEMI_V *pV = NULL;
		struct ___IDS__ {
			TCHAR*		p_name;
			XD_UINT4	index;
		} ___vindex___[] = {
			// Comm
			{XSEMI_GEM_CONF_MDLN,					EC_INDEX_MDLN},
			{XSEMI_GEM_CONF_SOFTREV,				EC_INDEX_SOFTREV},
			{XSEMI_GEM_CONF_CONF_CONNECT,			EC_INDEX_CONFIG_CONNECT},
			{XSEMI_GEM_CONF_ESTAB_COMM_DELAY,		EC_INDEX_CONFIG_ESTABLISH_COMM_DELAY},
			{XSEMI_GEM_CONF_INIT_COMM_STATE,		EC_INDEX_INIT_COMM_STS},
			{XSEMI_GEM_CONF_INIT_CTRL_STATE,		EC_INDEX_INIT_CTRL_STS},
			{XSEMI_GEM_CONF_ONLINE_SUB_STATE,		EC_INDEX_ONLINE_SUBSTS},
			{XSEMI_GEM_CONF_OFFLINE_SUB_STATE,		EC_INDEX_OFFLINE_SUBSTS},
			{XSEMI_GEM_CONF_ONLINE_FAILED,			EC_INDEX_ONLINE_FILED},
			{XSEMI_GEM_CONF_POOL_DELAY,				EC_INDEX_POLL_DELAY},
			{XSEMI_GEM_CONF_BUFFER_SIZE,			EC_INDEX_BUFFER_SIZE},
			// Format系
			{XSEMI_GEM_CONF_FORMAT_VID,				EC_INDEX_FORMAT_VID},
			{XSEMI_GEM_CONF_FORMAT_TRID,			EC_INDEX_FORMAT_TRID},
			{XSEMI_GEM_CONF_FORMAT_RPTID,			EC_INDEX_FORMAT_RPTID},
			{XSEMI_GEM_CONF_FORMAT_DSID,			EC_INDEX_FORMAT_DSID},
			{XSEMI_GEM_CONF_FORMAT_DATAID,			EC_INDEX_FORMAT_DATAID},
			{XSEMI_GEM_CONF_FORMAT_ALID,			EC_INDEX_FORMAT_ALID},
			{XSEMI_GEM_CONF_FORMAT_CEID,			EC_INDEX_FORMAT_CEID},
			// Events
			{XSEMI_GEM_CONF_CONFIG_EVENT,			EC_INDEX_CONFIG_EVENT},
			{XSEMI_GEM_CONF_RP_TYPE,				EC_INDEX_RP_TYPE},
			{XSEMI_GEM_CONF_WBIT_6,					EC_INDEX_WBIT6},
			// Limit
			{XSEMI_GEM_CONF_LIMIT_DELAY,			EC_INDEX_LIMIT_DELAY},
			// Spool
			{XSEMI_GEM_CONF_CONFIG_SPOOL,			EC_INDEX_CONFIG_SPOOL},
			{XSEMI_GEM_CONF_MAX_SPOOL_SIZE,			EC_INDEX_MAX_SPOOL_SIZE},
			{XSEMI_GEM_CONF_MAX_SPOOL_TRANSMIT,		EC_INDEX_MAX_SPOOL_TRANMIT},
			{XSEMI_GEM_CONF_SPPOL_CNT_ACTL,			SV_INDEX_SPOOL_CNT_ACTUAL},
			{XSEMI_GEM_CONF_SPOOL_CNT_TOTAL,		SV_INDEX_SPOOL_CNT_TOTAL},
			{XSEMI_GEM_CONF_OVR_WRITE_SPOOL,		EC_INDEX_OV_WRITE_SPOOL},
			{XSEMI_GEM_CONF_SPOOL_FULL_TIME,		SV_INDEX_SOOL_FULLTIME},
			{XSEMI_GEM_CONF_SPOOL_STAT,				SV_INDEX_SPOOL_STS},
			{XSEMI_GEM_CONF_SPOOL_LOAD_SUBST,		SV_INDEX_SPOOL_LOAD_SUBSTS},
			{XSEMI_GEM_CONF_SPOOL_UNLOAD_SUBST,		SV_INDEX_SPOOL_UNLOAD_STS},
			{XSEMI_GEM_CONF_SPOOL_START_TIME,		SV_INDEX_SPOOL_START_TIME},
			{XSEMI_GEM_CONF_SPOOL_FILENAME,			SV_INDEX_SPOOL_FILE_NAME},
			// Other
			{XSEMI_GEM_CONF_OTH_ACTIVE_GEM_CONNECT,	EC_INDEX_OTH_ACTIVE_GEM_CONNECT},

			{NULL, 0}
		};

		for(int ii = 0; ___vindex___[ii].p_name != NULL;ii++) {
			pV = m_constData.get_Vs().get_byName( ___vindex___[ii].p_name );
			if( pV != NULL ) {
				m_ulVIDs[___vindex___[ii].index] = pV->get_VID();
			} else {
				writeLogCallBack(XLIB_LOG_LEVEL_WARNING, 
					(const TCHAR*)XStrBuff::FormatS(_T("V_NAME : [%s] not defined"), ___vindex___[ii].p_name )
				);
			}
		}

		// モデルとSoftVerが空の場合は、デフォルト値を設定する。
		if( m_ulVIDs[EC_INDEX_MDLN] != (XD_UINT4)-1 ) {
			char szModel[64] = {0};
			this->getVValue(m_ulVIDs[EC_INDEX_MDLN], NULL, (void*)szModel, sizeof(szModel));
			if( szModel[0] == 0 ) {
				this->putVValue(m_ulVIDs[EC_INDEX_MDLN], "MODEL ", 6);
			}		
		}
		if( m_ulVIDs[EC_INDEX_SOFTREV] != (XD_UINT4)-1 ) {
			char szModel[64] = {0};
			this->getVValue(m_ulVIDs[EC_INDEX_SOFTREV], NULL, (void*)szModel, sizeof(szModel));
			if( szModel[0] == 0 ) {
				this->putVValue(m_ulVIDs[EC_INDEX_SOFTREV], "Ver.01", 6);
			}		
		}



		// 初期コントロール状態
		if( m_ulVIDs[EC_INDEX_INIT_CTRL_STS] !=  XSEMI_INVALID_VID ) {
			XBIN uc = XSEMI_GEM_CTRL_STS_UNKNOWN;
			this->getVValue(m_ulVIDs[EC_INDEX_INIT_CTRL_STS], NULL, &uc, sizeof(uc));
			m_ulControlStatus = (XD_UINT4)uc;
		}
		// 初期通信状態
		m_emCommState = XSEMI_Gem::COMM_STS_NOT_COMUNICATION;
		if( m_ulVIDs[EC_INDEX_INIT_COMM_STS] !=  XSEMI_INVALID_VID ) {
			XBIN uc = XSEMI_GEM_CTRL_STS_ONLINE_REMOTE;
			this->getVValue(m_ulVIDs[EC_INDEX_INIT_COMM_STS], NULL, &uc, sizeof(uc));
			if( uc == 0 ) {
				m_emCommState = XSEMI_Gem::COMM_STS_DISABLE;
			}
		}

		// スプールの設定を初期化
		if( getWellKnownVID(EC_INDEX_CONFIG_SPOOL) !=  XSEMI_INVALID_VID ) {
			XBIN uc = XSEMI_GEM_CONF_DEF_CONFIG_SPOOL;
			this->getVValue(getWellKnownVID(EC_INDEX_CONFIG_SPOOL), NULL, &uc, sizeof(uc));
			if( uc == 1 ) {
				TCHAR	szSpoolFile[260] = _T("SPOOL.LOG");
				XD_UINT1 ucOverwriteSpool = (XBIN)XSEMI_GEM_CONF_DEF_OVR_WRITE_SPOOL;
				XD_UINT4 ulSpoolTransmit = XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT;
				XD_UINT4 ulSpoolSize     = XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE;
				// EC:スプールファイル名
				if( m_ulVIDs[SV_INDEX_SPOOL_FILE_NAME] !=  XSEMI_INVALID_VID ) {
					this->getVValue(m_ulVIDs[SV_INDEX_SPOOL_FILE_NAME], NULL, szSpoolFile, sizeof(szSpoolFile));
				}
				// EC:オーバーライト？
				if( m_ulVIDs[EC_INDEX_OV_WRITE_SPOOL] !=  XSEMI_INVALID_VID ) {
					this->getVValue(m_ulVIDs[EC_INDEX_OV_WRITE_SPOOL], NULL, &ucOverwriteSpool, sizeof(ucOverwriteSpool));
				}
				// EC:最大Spool数
				if( m_ulVIDs[EC_INDEX_MAX_SPOOL_TRANMIT] !=  XSEMI_INVALID_VID ) {
					this->getVValue(m_ulVIDs[EC_INDEX_MAX_SPOOL_TRANMIT], NULL, &ulSpoolTransmit, sizeof(ulSpoolTransmit));
				}
				// EC:最大Spoolサイズ
				if( m_ulVIDs[EC_INDEX_MAX_SPOOL_SIZE] !=  XSEMI_INVALID_VID ) {
					this->getVValue(m_ulVIDs[EC_INDEX_MAX_SPOOL_SIZE], NULL, &ulSpoolTransmit, sizeof(ulSpoolTransmit));
				}

				m_pobjSpool->open(szSpoolFile, ulSpoolSize, ulSpoolTransmit, ucOverwriteSpool == 1 ? true : false);
			}
		}

		m_bInit = true;
	} catch( XExcept &e ) {
		// 後処理
		XEXCEPT_RETHROW(e, XExcept_SECS);
	}
	return m_bInit;
XEXCEPT_EXIT
}

bool XSEMI_Gem::initAPI(const TCHAR* pInitName)
{
XEXCEPT_ENTRY
	if( m_pAPI != NULL ) {
		// APIを閉じる
		m_pAPI->close();
		m_pAPI->Join(0);
		m_pAPI = NULL;
	}
	m_pAPI = new _API( this );
	if( m_pAPI->open(pInitName) != XMsgQueueERR_SUCCESS  ) {
		return false;
	}
	m_pAPI->startMainLoopThread();
	return true;
XEXCEPT_EXIT
}
bool XSEMI_Gem::release()
{
XEXCEPT_ENTRY
	XStrBuff	strErrMsg;
	
	if( m_pAPI != NULL ) {

		try {
			// APIを閉じる
			m_pAPI->close();
			m_pAPI->Join(0);
		} catch (XExcept& ex) {
			m_pAPI = NULL;	
			// Joinはスローする事がある
			if( !( ex.getHint() == XThredERROR_UN_STS_RUNNING_TERMINATED ||
				   ex.getHint() == XThredERROR_JOIN )  ) {
				
				XEXCEPT_RETHROW(ex, XExcept_RETHROW);
			}
		}
		m_pAPI = NULL;
	}
	if( m_bInit == false ) {
		return true;
	}
	
	// ECの値をセーブする。
	XEXCEPT_ENTRY
		m_constData.saveECValues();
		m_constData.clear();
	XEXCEPT_EXIT
	m_bInit = false;
	return true;
XEXCEPT_EXIT
}

bool XSEMI_Gem::openCommunication(XSeceCommItf* pInterface, bool bIsActive, XD_UINT2 usDeviceID)
{
XEXCEPT_ENTRY
	m_pCommuIf = pInterface;

	return m_pCommuIf->initilize(m_emCommState == COMM_STS_DISABLE ? false : true, 
								bIsActive, 
								usDeviceID, 
								this);
XEXCEPT_EXIT
	m_pCommuIf = NULL;
	return false;
}

void XSEMI_Gem::closeCommunication()
{
XEXCEPT_ENTRY
	XSeceCommItf* pTmp = m_pCommuIf;
	m_pCommuIf = NULL;
	if( pTmp != NULL ) {
		pTmp->closeIf();
	}
XEXCEPT_EXIT
}

//  コントロール系
XD_UINT4 XSEMI_Gem::get_ControlState() const
{
XEXCEPT_ENTRY
	return m_ulControlStatus;
XEXCEPT_EXIT
}

int XSEMI_Gem::chCtrlStateOffline()
{
XEXCEPT_ENTRY

	XD_UINT4 ulPreControlStatus = 0;

	if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_LOCAL ||
		m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_REMOTE ) {
		// オンライン状態の場合、
		// 通知
		// -> 装置オフライン
		innerNotifyContorolState(XSEMI_GEM_CTRL_STS_OFFLINE_EQP);

		return 1;
	}
	return 0;
XEXCEPT_EXIT
}
int XSEMI_Gem::chCtrlStateOnline()
{
XEXCEPT_ENTRY
	XD_UINT4 ulPreControlStatus = 0;

	if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_OFFLINE_EQP ) {
		// 装置オフライン

		ulPreControlStatus = m_ulControlStatus;


		// S1F1 を送信
		XSecsData data;
		data.put_stream( 1 );
		data.put_function( 1 );
		data.put_wait( true );

		if( this->sendRequest(&data) == XSECS_SEND_STS_S ) {
			// 通知
			// -> Attempt オフライン
			innerNotifyContorolState(XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT);

			return 1;
		} else {
			// 強制的に装置OffLineへ
		}
	} 

	return 0;
XEXCEPT_EXIT
}
int XSEMI_Gem::chCtrlStateLocal()
{
XEXCEPT_ENTRY
	XD_UINT4 ulPreControlStatus = 0;

	if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_REMOTE ) {
		// 通知
		// -> Online LOCAL
		innerNotifyContorolState(XSEMI_GEM_CTRL_STS_ONLINE_LOCAL);
		return 1;
	}
	return 0;
XEXCEPT_EXIT
}
int XSEMI_Gem::chCtrlStateRemote()
{
XEXCEPT_ENTRY
	XD_UINT4 ulPreControlStatus = 0;

	if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_LOCAL ) {
		// 通知
		// -> Online LOCAL
		innerNotifyContorolState(XSEMI_GEM_CTRL_STS_ONLINE_REMOTE);
		return 1;
	}
	return 0;
XEXCEPT_EXIT
}

XSEMI_Gem::COMM_STS XSEMI_Gem::get_CommunicationState() const
{
XEXCEPT_ENTRY
	return m_emCommState;
XEXCEPT_EXIT
}

int XSEMI_Gem::chEnable()
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		// 通信インターフェースが初期化されていない
		innerCommunicationStateChange(COMM_STS_DISABLE);
		return -1;
	}
	if( m_pCommuIf->isEnable() ==  true ) {
		// 既に、有効
		return 1;
	}
	if( m_pCommuIf->enable() == false ) {
		return 1;
	}
	innerCommunicationStateChange(COMM_STS_NOT_COMUNICATION);
	return 0;
XEXCEPT_EXIT
}

int XSEMI_Gem::chDisable()
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		// 通信インターフェースが初期化されていない
		innerCommunicationStateChange(COMM_STS_DISABLE);
		return -1;
	}
	if( m_pCommuIf->isEnable() ==  false ) {
		// 既に、無効
		innerCommunicationStateChange(COMM_STS_DISABLE);
		return 1;
	} else {
		if( m_pCommuIf->disable() == false ) {
			return 1;
		}
	}
	innerCommunicationStateChange(COMM_STS_DISABLE);
	return 0;
XEXCEPT_EXIT
}

bool XSEMI_Gem::get_isHost() const
{
XEXCEPT_ENTRY
	return m_bHost;
XEXCEPT_EXIT
}

bool XSEMI_Gem::get_connectDirection() const
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return false;
	}
	return m_pCommuIf->get_connectDirection();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_Gem::get_deviceID() const
{
XEXCEPT_ENTRY
	return m_ulDeviceID;
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_Gem::get_SequenceDATAID()
{
XEXCEPT_ENTRY
	m_ulSequcialDATAID++;
	return m_ulSequcialDATAID;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_Gem::get_BufferSize()
{
XEXCEPT_ENTRY
	XD_UINT4 val = XSEMI_GEM_CONF_DEF_BUFFER_SIZE;
	if( m_ulVIDs[EC_INDEX_BUFFER_SIZE] !=  XSEMI_INVALID_VID ) {
		this->getVValue(
				
				m_ulVIDs[EC_INDEX_BUFFER_SIZE], 
				NULL, 
				&val, 
				sizeof(val) 
			
			
			);
	}
	return val;
XEXCEPT_EXIT
}

void XSEMI_Gem::getGemTime(XStrBuff &strTime)
{
XEXCEPT_ENTRY

	getTimeString(m_time.getCurrentTime(), strTime);

XEXCEPT_EXIT
}

XTime XSEMI_Gem::getGemTime()
{
XEXCEPT_ENTRY
	return m_time.getCurrentTime();
XEXCEPT_EXIT
}

void XSEMI_Gem::getTimeString(XTime& obTime, XStrBuff &strTime)
{
XEXCEPT_ENTRY
	TCHAR	szMSEC[64];


	XStrBuff::printf(szMSEC, XCOUNTOF(szMSEC), _T("%u"), obTime.GetMSecond());
	szMSEC[2] = 0;
	
//	if( obTime.GetStatus() == XTime::valid ) {
//		strTime = _T("0000000000000000");
//	} else {
		strTime.Format(_T("%04u%02u%02u%02u%02u%02u%s"), 
				obTime.GetYear()
			,	obTime.GetMonth()
			,	obTime.GetDay()
			,	obTime.GetHour()
			,	obTime.GetMinute()
			,	obTime.GetSecond()
			,	szMSEC);
//	}
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_Gem::getOpt_T3() const
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return 0;
	}
	return m_pCommuIf->getOpt_T3();
XEXCEPT_EXIT
}

XSECS_CONN_STS XSEMI_Gem::get_ConnectState() const 
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return XSECS_CONN_STS_NOT_CONNECTED;
	}
	return m_pCommuIf->get_status();
XEXCEPT_EXIT
}


XSECS_SEND_STS XSEMI_Gem::innerSendRequest(XSECSHeader* pHeader, 
										   XBIN *pData, 
										   unsigned long ulDataSize, 
										   XSECSMSG* pRep /*= NULL*/, 
										   bool bWait /*= false*/, 
										   int nTimeOut /*= -1*/)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return XSECS_SEND_STS_F_UNINIT;
	}
	return m_pCommuIf->sendRequest(pHeader, pData, ulDataSize, pRep, bWait, nTimeOut);
XEXCEPT_EXIT
}


XSECS_SEND_STS XSEMI_Gem::sendRequest(XSecsData *pData, 
									  XSECSMSG* pRep /*= NULL*/, 
									  bool bWait /*= false*/, 
									  int nTimeOut /*= -1*/)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return XSECS_SEND_STS_F_UNINIT;
	}
	return m_pCommuIf->sendRequest(pData, pRep, bWait, nTimeOut);
XEXCEPT_EXIT
}
XSECS_SEND_STS XSEMI_Gem::sendRequest(XSecsData *pData, 
									  XSecsData *pRcvData, 
									  bool bWait /*= true*/, 
									  bool bDropTicket /*= true*/, 
									  int nTimeOut /*= -1*/)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return XSECS_SEND_STS_F_UNINIT;
	}
	return m_pCommuIf->sendRequest(pData, pRcvData, bWait, bDropTicket, nTimeOut);
XEXCEPT_EXIT
}
XSECS_SEND_STS XSEMI_Gem::sendRequest(XSECSHeader* pHeader, 
									  XBIN *pData, 
									  unsigned long ulDataSize, 
									  XSECSMSG* pRep /*= NULL*/, 
									  bool bWait /*= false*/, 
									  int nTimeOut /*= -1*/)
{
XEXCEPT_ENTRY
	return innerSendRequest(pHeader, pData, ulDataSize, pRep, bWait, nTimeOut);
XEXCEPT_EXIT
}

XSECS_SEND_STS XSEMI_Gem::sendInquire(XSECSHeader* pHeader, 
										XBIN *pData, 
										unsigned long ulDataSize, 
										XSECSHeader* inquireHead, 
										unsigned long ulInqireSize /*= 0*/,
										XBIN **ppResposeData /*= NULL*/, 
										XD_UINT4 *pulResponseDataSize /*= NULL*/, 
										bool bWait /*= false*/, 
										int nTimeOut /*= -1*/)
{
XEXCEPT_ENTRY

	if( m_pCommuIf == NULL ) {
		return XSECS_SEND_STS_F_UNINIT;
	}

	XBIN *pInqRspBuff = NULL;
	XD_UINT4       ulInqRspSize = 0;
	XSECS_SEND_STS	stsInq;
	XSECSHeader		inqReqHead;
	XSECSMSG		inqResp;

	::memset(&inqReqHead, 0, sizeof(inqReqHead));
	if( inquireHead != NULL ) {
		::memcpy(&inqReqHead, inquireHead, sizeof(inqReqHead));
	} else {
		
	}


	// Inqireメッセージの作成
	XInflateStreamBuff  strmBuf;
	std::ostream		oStream(&strmBuf);
	XDataWriter			writer( &oStream );	
	if( writer.putValAsList( 2 ) == false ||
		writer.putValAsUInt4( this->get_SequenceDATAID() ) == false ||
		writer.putValAsUInt4( ulInqireSize == 0 ? ulDataSize+sizeof(XSECSHeader) : ulInqireSize ) == false ) {
		// 失敗：送信メッセージ組み立て時にエラーが発生
		return XSECS_SEND_STS_F_BUILD;
	}
	// Inqireメッセージの送信＆2次メッセージの受信
	stsInq = innerSendRequest(&inqReqHead, strmBuf.getBuff(), strmBuf.size(), 
		&inqResp, true, -1);
	if( XSECS_SEND_STS_S_AND_HAS_REP != stsInq ) {
		return stsInq;
	}
/*
	::memcpy(&inqRspHead, pInqRspBuff, sizeof(inqRspHead));
	XDataReader reader(pInqRspBuff+10, ulInqRspSize-10);
	XData		xdata;
	if( reader.getValue( &xdata ) == false || 
		xdata.getType() != XDATA_TYPE_B ||
		xdata.getSize() != 1 ) {
		// 失敗：マルチブロック問い合わせの2次メッセージのデータが期待値でない
		this->dropTicket( XSecsData::get_ticket(*inquireHead) );
		return XSECS_SEND_STS_F_INQ_DATA;
	}
	if( xdata.getValAsUInt1() != 0 ) {
		// 失敗：マルチブロック問い合わせの2次メッセージのGRANTが拒否された
		this->dropTicket( XSecsData::get_ticket(*inquireHead) );
		return XSECS_SEND_STS_F_INQ_DATA;
	}
*/
	this->dropTicket( XSecsData::get_ticket(*inquireHead) );

	// メッセージの送信。
	return innerSendRequest(pHeader, pData, ulDataSize, &inqResp, bWait, nTimeOut);
XEXCEPT_EXIT
}






XSECS_SEND_STS XSEMI_Gem::sendResponse(XSECSHeader* pHeader, XBIN *pData, unsigned long ulDataSize)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return XSECS_SEND_STS_F_UNINIT;
	}
	return m_pCommuIf->sendResponse(pHeader, pData, ulDataSize);
XEXCEPT_EXIT
}
XSECS_SEND_STS XSEMI_Gem::sendResponse(XSecsData *pData)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return XSECS_SEND_STS_F_UNINIT;
	}
	return  m_pCommuIf->sendResponse(pData);
XEXCEPT_EXIT
}
XSECS_SEND_STS XSEMI_Gem::poll(XD_UINT4 ulTicket, XBIN **ppResposeData , XD_UINT4 *pulResponseDataSize, XD_UINT4 ulTimeO /*= (XD_UINT4)-1*/)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return XSECS_SEND_STS_F_UNINIT;
	}
	return  m_pCommuIf->poll(ulTicket, ppResposeData, pulResponseDataSize, ulTimeO);
XEXCEPT_EXIT
}
XSECS_SEND_STS XSEMI_Gem::cancelWait(XD_UINT4 ulTicket)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return XSECS_SEND_STS_F_UNINIT;
	}
	return  m_pCommuIf->cancelWait(ulTicket);
XEXCEPT_EXIT
}
bool XSEMI_Gem::dropTicket(XD_UINT4 ulTicket)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return false;
	}
	if( m_pCommuIf->dropTicket(ulTicket) == false ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_Gem::isHaveTicket(XD_UINT4 ulTicket)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return false;
	}
	if( m_pCommuIf->isHaveTicket(ulTicket) == false ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_Gem::getTicketInfo(XD_UINT4 ulTicket, XSeceCommItf::TICKET_INF* pTicketInf)
{
XEXCEPT_ENTRY
	if( m_pCommuIf == NULL ) {
		return false;
	}
	if( m_pCommuIf->getTicketInfo(ulTicket, pTicketInf) == false ) {
		return false;
	}

	// m_pCommuIfで取得する時間はクロックタイムなので、GEMタイムに合わせる様に修正する。
	XTime	timeGem = this->getGemTime();
	XTime	timeNow = XTime::getCurrentTime();
	double diffTime = timeNow.m_dt - timeGem.m_dt;

	pTicketInf->m_reg_time.m_dt -= diffTime;
	pTicketInf->m_rcv_time.m_dt -= diffTime;

	return true;
XEXCEPT_EXIT
}

int XSEMI_Gem::raiseEvent(XD_UINT4 ulCEID)
{
XEXCEPT_ENTRY
	XInflateStreamBuff	buff;
	std::ostream		stream(&buff);
	XDataWriter			writer( &stream );
	XD_UINT1			ucConfigEvents		= XSEMI_GEM_CONF_DEF_CONFIG_EVENT;
	XD_UINT1			ucWbitS6			= XSEMI_GEM_CONF_DEF_WBIT_6;
	bool				ucAnnotatedReport	= XSEMI_GEM_CONF_DEF_RP_TYPE;
	int					function = 11;



	if( m_ulVIDs[EC_INDEX_CONFIG_EVENT] !=  XSEMI_INVALID_VID ) {
		this->getVValue(m_ulVIDs[EC_INDEX_CONFIG_EVENT], NULL, &ucConfigEvents, sizeof(ucConfigEvents));
	}
	if( m_ulVIDs[EC_INDEX_WBIT6] !=  XSEMI_INVALID_VID ) {
		this->getVValue(m_ulVIDs[EC_INDEX_WBIT6], NULL, &ucWbitS6, sizeof(ucWbitS6));
	}
	if( m_ulVIDs[EC_INDEX_RP_TYPE] !=  XSEMI_INVALID_VID ) {
		this->getVValue(m_ulVIDs[EC_INDEX_RP_TYPE], NULL, &ucAnnotatedReport, sizeof(ucAnnotatedReport));
	}

	if( ucConfigEvents == XSEMI_GEM_CONF_DEF_CONFIG_EVENT_VS && 
		ucAnnotatedReport == 1 ) {
		function = 3;
	} else
	if( ucConfigEvents == XSEMI_GEM_CONF_DEF_CONFIG_EVENT_VS && 
		ucAnnotatedReport == 0 ) {
		function = 9;
	} else
	if( ucConfigEvents == XSEMI_GEM_CONF_DEF_CONFIG_EVENT_EVENT && 
		ucAnnotatedReport == 1 ) {
		function = 13;
	} else
	if( ucConfigEvents == XSEMI_GEM_CONF_DEF_CONFIG_EVENT_EVENT && 
		ucAnnotatedReport == 0 ) {
		function = 11;
	}


	// CEIDを見つける
	XSEMI_CEID* pCEID = m_constData.get_CEIDs( ).get_byID( ulCEID );
	if( pCEID == NULL ) {
		return -1;
	}

	// L.3 or 4
	if( writer.putValAsList(
		ucConfigEvents == XSEMI_GEM_CONF_DEF_CONFIG_EVENT_VS ? 4 : 3
		) == false ) {
		return -2;
	}
	if( ucConfigEvents == XSEMI_GEM_CONF_DEF_CONFIG_EVENT_VS ) {
		// 1. <U4 PFCD>
		XBIN bin = 0;
		if( writer.putValAsBinary(&bin, sizeof(bin) ) == false ) {
			return -2;
		}
	}
	// 1. <U4 DATAID>
	if( writer.putValAsUInt4( m_ulSequcialDATAID++ ) == false ) {
		return -2;
	}
	// 2. <U4 CEID>
	if( writer.putValAsUInt4( ulCEID ) == false ) {
		return -2;
	}

	// L.3
	if( writer.putValAsList( pCEID->get_RptsCount() ) == false ) {
		return -2;
	}

	for(XD_UINT4 i = 0; i < pCEID->get_RptsCount(); i++) {
		XSEMI_Rpt* pRpt = pCEID->get_Rpt( i );
		_XASSERT( pRpt );
		
		// L.2
		if( writer.putValAsList( 2 ) == false ) {
			return -2;
		}
		// 1. <U4 RPTID>
		if( writer.putValAsUInt4( pRpt->get_RptID() ) == false ) {
			return -2;
		}
		// 2. L,b
		if( writer.putValAsList( pRpt->get_VsCount() ) == false ) {
			return -2;
		}
		for(XD_UINT4 j = 0; j < pRpt->get_VsCount(); j++) {
			XSEMI_V* pV = pRpt->get_V( j );
			_XASSERT( pV );


			if( ucAnnotatedReport == 1 ) {
				// L.2
				if( writer.putValAsList( 2 ) == false ) {
					return -2;
				}
				if( writer.putValAsUInt4( pV->get_VID() ) == false ) {
					return -2;
				}
			}
			XDATA_TYPE type;
			XDataWriter dataWriter( &stream );
			if( this->innerGetVValue(ulCEID, pV->get_VID(), &type, dataWriter) != GETV_STS_SUCCESS ) {
				return -2;
			}
//			if( pV->buildBinaryData( &stream ) == false ) {
//				return -2;
//			}
		}
	}


	// メッセージを送信。
	XSECSHeader header;
	XSecsData::setSxFx(header, 6, function, ucWbitS6 == 1 ? true : false);
	if( sendRequest(&header, buff.getBuff(), buff.size()) != XSECS_SEND_STS_S ) {
		return -3;
	}
	return 0;
XEXCEPT_EXIT
}

void XSEMI_Gem::commitLinkEventReport()
{
XEXCEPT_ENTRY
	XD_UINT4 i, j, ope_cnt;
	XD_UINT4 cntCEID = 0;
	XD_UINT4 cntCEID_True = 0;
	
	// リンク情報が変更されtあアイテムがあるか調べる
	for(i = 0; i < m_constData.get_CEIDs().count(); i++) {
		XSEMI_CEID* pCEID = m_constData.get_CEIDs().get( i );
		if( pCEID->m_bDirtyLinked == true ) { 
			cntCEID++;
			if( pCEID->get_Enable() == true ) { cntCEID_True++; }
		}
	}
	if( cntCEID == 0 ) {
		// 更新すべきアイテムが見つからなかった
		return ;
	}

	if( this->get_isHost() == true ) {
		// ホストの場合、
		if( this->get_CommunicationState() != XSEMI_Gem::COMM_STS_COMUNICATION ) {
			// 装置にまだ接続できていない。
			XEXCEPT_THROW_STR(XExcept_SECS, 
				_T("not connected to equipment."), 0, 1);	
		}
		//
//		if( this->get_ControlState() != XSEMI_GEM_CTRL_STS_ONLINE_REMOTE) {
//			// OnlineRemoteでないと設定できない。
//			XEXCEPT_THROW_STR(XExcept_SECS, 
//				_T("access deny. not allow ctrol state."), 0, 2);	
//		}
	}

	try {




		if( this->get_isHost() == true ) {
			// S2,F35 Link Event Report (LER)を送信して、装置側を更新する。

			for( ope_cnt =0; ope_cnt < 2; ope_cnt++) {
				
				XInflateStreamBuff	buff;
				XSECSHeader			header;
				std::ostream		stream(&buff);
				XDataWriter			writer( &stream );
				XSECSMSG			rspmsg;
			
				XSecsData::setSxFx(header, 2, 35, true);

				// L.2
				if( writer.putValAsList(2) == false ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("message build."), 0, 4);	
				}
				// 1. <U4 DATAID>
				if( writer.putValAsUInt4( m_ulSequcialDATAID++ ) == false ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("message build."), 0, 4);	
				}
				// 2. L,a
				if( writer.putValAsList( cntCEID ) == false ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("message build."), 0, 4);	
				}
				for(i = 0; i < m_constData.get_CEIDs().count(); i++) {
					XSEMI_CEID* pCEID = m_constData.get_CEIDs().get( i );
					if( pCEID->m_bDirtyLinked != true ) { continue; }

					// 1. L,2
					if( writer.putValAsList(2) == false ) {
						XEXCEPT_THROW_STR(XExcept_SECS, 
							_T("message build."), 0, 4);	
					}
					// 1. <U4 CEID1> * 通知イベントID
					if( writer.putValAsUInt4( pCEID->get_CEID() ) == false ) {
						XEXCEPT_THROW_STR(XExcept_SECS, 
							_T("message build."), 0, 4);	
					}
					if( ope_cnt == 0 ) {
						// リンク状態を一旦解除する。
						// L.b
						if( writer.putValAsList( 0 ) == false ) {
							XEXCEPT_THROW_STR(XExcept_SECS, 
								_T("message build."), 0, 4);	
						}
					} else {
						// リンク状態を設定する。
						// L.b
						if( writer.putValAsList( pCEID->get_RptsCount() ) == false ) {
							XEXCEPT_THROW_STR(XExcept_SECS, 
								_T("message build."), 0, 4);	
						}
						for(j = 0; j < pCEID->get_RptsCount(); j++) {
							XSEMI_Rpt* pRpt = pCEID->get_Rpt( j );

							if( writer.putValAsUInt4( pRpt->get_RptID() ) == false ) {
								XEXCEPT_THROW_STR(XExcept_SECS, 
									_T("message build."), 0, 4);	
							}
						}
					}
				}

				// メッセージ送信
				if( sendRequest(&header, buff.getBuff(), buff.size(), &rspmsg) != XSECS_SEND_STS_S_AND_HAS_REP ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("fail to deivery message to equipment."), 0, 5);	
				}
				if( rspmsg.ulDataSize <= 0 ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("receive data broken."), 0, 5);	
				}
				XDataReader reader(rspmsg.pData, rspmsg.ulDataSize);
				XData       data;
				if( reader.getValue(&data) != true ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("receive data broken."), 0, 5);	
				}
				if( data.getType() != XDATA_TYPE_B || data.getCount() != 1 ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("receive data broken."), 0, 5);	
				}
				XD_UINT1 LRACK = data.getValAsUInt1();
				if( LRACK != 0 ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("LRACK is not Zero."), 0, LRACK);	
				}

				this->dropTicket( XSecsData::get_ticket(header) );
			}


			if( cntCEID_True != 0 ) {
				//	S2,F37 Enable/Disable Event Report (EDER)を送信して、
				//	CEIDを有効に設定する。

				XInflateStreamBuff	buff;
				XSECSHeader			header;
				std::ostream		stream(&buff);
				XDataWriter			writer( &stream );
			
				XSecsData::setSxFx(header, 2, 35, true);

				// L.2
				if( writer.putValAsList(2) == false ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("message build."), 0, 4);	
				}
				// 1. <BOOL CEED>
				if( writer.putValAsBool( true ) == false ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("message build."), 0, 4);	
				}
				// 2. L,a
				if( writer.putValAsList( cntCEID_True ) == false ) {
					XEXCEPT_THROW_STR(XExcept_SECS, 
						_T("message build."), 0, 4);	
				}
				for(i = 0; i < m_constData.get_CEIDs().count(); i++) {
					XSEMI_CEID* pCEID = m_constData.get_CEIDs().get( i );
					if( pCEID->m_bDirtyLinked != true ) { continue; }
					if( pCEID->get_Enable() == true ) {
						if( writer.putValAsUInt4( pCEID->get_CEID() ) == false ) {
							XEXCEPT_THROW_STR(XExcept_SECS, 
								_T("message build."), 0, 4);	
						}
					}
				}
			}
		}


		// マスターを更新
		m_constData.updateMaster( &(m_constData.get_CEIDs()) );

	} catch ( XExcept &ex ) {
		XEXCEPT_RETHROW(ex, XExcept_RETHROW);
	}
//	return true;
XEXCEPT_EXIT
}


int XSEMI_Gem::raiseAlarm(XD_UINT4 alid, XD_UINT1 alcd, const TCHAR* p_altx /*= NULL*/)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	
	// CEIDを見つける
	XSEMI_ALARM* pAlarm = m_constData.get_Alarms().get_byID( alid );
	if( pAlarm == NULL ) {
		return -1;
	}
	if( pAlarm->get_Enable() == false ) {
		return -4;
	}
	XInflateStreamBuff	buff;
	std::ostream		stream(&buff);
	XDataWriter			writer( &stream );

	// L.3
	if( writer.putValAsList(3) == false ) {
		return -2;
	}
	if( writer.putValAsBinary(&alcd, 1) == false ) {
		return -2;
	}
	if( writer.putValAsUInt4(alid) == false ) {
		return -2;
	}
	if( p_altx != NULL ) {
		if( writer.putValAsStringA(p_altx) == false ) {
			return -2;
		}
	} else {
		if( writer.putValAsStringA( pAlarm->get_Text() ) == false ) {
			return -2;
		}
	}

	// メッセージを送信。
	XSECSHeader header;
	XSecsData::setSxFx(header, 5, 1, true);
	if( sendRequest(&header, buff.getBuff(), buff.size()) != XSECS_SEND_STS_S ) {
		return -3;
	}
	pAlarm->set_ALCD( alcd );
	if( p_altx != NULL ) {
		pAlarm->set_Name( p_altx );
	}

	// 関連イベントを送信します。
	if( alcd & 0x80 ) {
		if( pAlarm->get_OnCEID() != 0 ) {
			this->raiseEvent( pAlarm->get_OnCEID() );
		}
	} else {
		if( pAlarm->get_OffCEID() != 0 ) {
			this->raiseEvent( pAlarm->get_OffCEID() );
		}
	}
	return 0;
XEXCEPT_EXIT
}


// スプール

void XSEMI_Gem::notifySpoolState(XD_UINT4 ulPreSatus, XD_UINT4 ulNewState)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

int XSEMI_Gem::getSpoolSettingSF(std::ostream *stream)
{
XEXCEPT_ENTRY

	XSEMI_SpoolSFs&		obSFs = m_constData.get_SpoolSFs();
	XSEMI_SpoolSF*		pSF = NULL;
	XD_UINT4			i = 0;
	XRichData			rData(XDATA_TYPE_L);

	for( i = 0; i < obSFs.count(); i++ ) {
		pSF = obSFs.get( i );
		_XASSERT( pSF );
		XRichData* pRStream = NULL;
		XRichData* pRL = NULL;
		// 既に設定されているStreamを探す。
		for(XD_UINT4 j = 0; j < rData.getChiledCount(); j++ ) {
			XRichData* pRTemp = rData.getChiled( j );
			_XASSERT( pRTemp );

			if( pRTemp->getChiledCount() != 2 ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			XRichData* pRTemp2 = pRTemp->getChiled( 0 );
			_XASSERT( pRTemp2 );
			if( pRTemp2->getValueUInt1() == pSF->get_Stream() ) {
				pRL = pRTemp->getChiled( 1 );
				_XASSERT( pRL );
				pRStream = pRTemp;
				break;
			}
		}
		
		if( pRStream == NULL ) {
			// 新規Stream
			pRStream = rData.addChiled(XDATA_TYPE_L);
			if( pRStream == NULL ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			XRichData* pRTemp = pRStream->addChiled( XDATA_TYPE_U1 );
			if( pRTemp == NULL ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			pRTemp->putValue( pSF->get_Stream() );

			pRL = pRStream->addChiled( XDATA_TYPE_L );
		}

		_XASSERT( pRStream );
		_XASSERT( pRL );

		if( pSF->get_Function() == XSEMI_SPOOL_ALL_FUNCTIONS ) {
			// 全てのFunctionが対象
			pRStream->setUserData( pSF );
		} else {
			// 指定Function
			XD_UINT1 function = (XD_UINT1)pSF->get_Function();

			XRichData* pRTemp = pRL->addChiled( XDATA_TYPE_U1 );
			if( pRTemp == NULL ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			pRTemp->putValue( function );
		}
	}

	// バイナリーデータに組み立てる。
	if( rData.buildBinaryData( stream ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}


XD_UINT1 XSEMI_Gem::occurreAlarm(const XD_UINT4 alarmID, const XD_UINT1 alcd, const TCHAR* pTxt)
{
XEXCEPT_ENTRY
	return 0;
XEXCEPT_EXIT
}


bool XSEMI_Gem::innerCommunicateStateProcess(XSECS_CONN_STS sts)
{
XEXCEPT_ENTRY
	// ホストが接続して、通信確立の処理を実行する
	XDATA_TYPE	type;
	XD_UINT1 configConnect = XSEMI_GEM_CONF_DEF_CONF_CONNECT;
	XD_UINT1 configActiveGemConnect = 1;
	XD_UINT2 delayTimeOut =  XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY;	
	XTime nowTime = XTime::getCurrentTime();


	
	if( m_ulVIDs[EC_INDEX_OTH_ACTIVE_GEM_CONNECT] != (XD_UINT4)-1 ) {
		this->getVValue(m_ulVIDs[EC_INDEX_OTH_ACTIVE_GEM_CONNECT], &type, &configActiveGemConnect, sizeof(configActiveGemConnect));
		if( configActiveGemConnect == 0 ) {

			writeLogCallBack(XLIB_LOG_LEVEL_WARNING, _T("skip gem connect"));

			return true;
		}
	}



	if( m_ulVIDs[EC_INDEX_CONFIG_CONNECT] != (XD_UINT4)-1 ) {
		this->getVValue(m_ulVIDs[EC_INDEX_CONFIG_CONNECT], &type, &configConnect, sizeof(configConnect));
	}

	// 接続確立の送信周期内かチェックする。
	if( m_ulVIDs[EC_INDEX_CONFIG_ESTABLISH_COMM_DELAY] != (XD_UINT4)-1 ) {
		this->getVValue(m_ulVIDs[EC_INDEX_CONFIG_ESTABLISH_COMM_DELAY], &type, &delayTimeOut, sizeof(delayTimeOut));
	}
	if( nowTime.m_dt < m_dCommDelayTime ) {
		// 
		return false;
	}


	// 0:S1F1 1:S1F65 2:S1F13 
	XInflateBinStrstream	stream;
	XSECSHeader			header;
	XDataWriter			writer(&stream);
	XDATA_TYPE			dataType;


	switch( configConnect ) {
	case 0:
		XSecsData::setSxFx(header, 1, 1, true);
		break;
	case 1:
		XSecsData::setSxFx(header, 1, 65, true);
		if( this->get_isHost() == true ) {
			/*
				S1F65 W * H->E
				<L> .
			*/
			writer.putValAsList(0);
		} else {
			/*
				S1F65 W * H<-E
				<L
					<A MDLN>
					<A SOFTREV>
				> .
			*/
			writer.putValAsList(2);
			if( m_ulVIDs[EC_INDEX_MDLN] != (XD_UINT4)-1 ) {
				this->getVValue(m_ulVIDs[EC_INDEX_MDLN], &dataType, writer);
			} else {
				writer.putValAsStringA("");
			}

			if( m_ulVIDs[EC_INDEX_SOFTREV] != (XD_UINT4)-1 ) {
				this->getVValue(m_ulVIDs[EC_INDEX_SOFTREV], &dataType, writer);
			} else {
				writer.putValAsStringA("");
			}
		}
		break;
	case 2:
		XSecsData::setSxFx(header, 1, 13, true);
		if( this->get_isHost() == true ) {
			/*
				S1F13 W * H->E
				<L> .
			*/
			writer.putValAsList(0);
		} else {
			/*
				S1F13 W * H<-E
				<L
					<A MDLN>
					<A SOFTREV>
				> .
			*/
			writer.putValAsList(2);
			if( m_ulVIDs[EC_INDEX_MDLN] != (XD_UINT4)-1 ) {
				this->getVValue(m_ulVIDs[EC_INDEX_MDLN], &dataType, writer);
			} else {
				writer.putValAsStringA("");
			}

			if( m_ulVIDs[EC_INDEX_SOFTREV] != (XD_UINT4)-1 ) {
				this->getVValue(m_ulVIDs[EC_INDEX_SOFTREV], &dataType, writer);
			} else {
				writer.putValAsStringA("");
			}
		}
		break;
	}

	if( sendRequest(&header, stream.rdbuf()->getBuff(), (unsigned long)stream.pcount()) == XSECS_SEND_STS_S ) {

		m_ulTiketWaitCRA = XSecsData::get_ticket(header);

		// 通信確立送信時間を保存しておく
		double dblTime;
		dblTime = (((long)delayTimeOut / (60*60) * 3600L) +
			((long)delayTimeOut / (60) * 60L) +
			((long)delayTimeOut % (60))) / 86400.;
		m_dCommDelayTime = nowTime.m_dt + dblTime;


		// 通信状態の変更を通知
		innerCommunicationStateChange(COMM_STS_WAIT_CRA);
		return true;
	}
	return true;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessage(XD_UINT1 strem, XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	switch( strem ) {
	case 1:
		return processGemMessageS1(function, inData, pOutStream);
		break;
	case 2:
		return processGemMessageS2(function, inData, pOutStream);
		break;
	case 5:
		return processGemMessageS5(function, inData, pOutStream);
		break;
	case 6:
		return processGemMessageS6(function, inData, pOutStream);
		break;
	case 16:
		return processGemMessageS16(function, inData, pOutStream);
		break;
	case 9:
		return XSEMI_GEM_ERR_NON_TRANSACT_DEF;
	}
	return XSEMI_GEM_ERR_HEAD_STREAM;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessage(XSecsData& data, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XInflateStreamBuff	inBuff(255, 2048);
	std::ostream		inStream(&inBuff);
	int					iRetVal;


	if( data.buildBinaryData(inStream, XSecsData::BODY) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	XDataReader	reader(inBuff.getBuff(), inBuff.size());
	iRetVal = this->processGemMessage(data.get_stream(), data.get_function(), reader, pOutStream);
	return iRetVal;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessage(XSecsData& data)
{
XEXCEPT_ENTRY
	XInflateStreamBuff	inBuff(255, 2048);
	std::ostream		inStream(&inBuff);
	XInflateStreamBuff	outBuff(255, 2048);
	std::ostream		outStream(&outBuff);
	
	if( data.buildBinaryData(inStream, XSecsData::BODY) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}

	XDataReader	reader(inBuff.getBuff(), inBuff.size());
	int iRetVal = this->processGemMessage(data.get_stream(), data.get_function(), reader, outStream);

	if( iRetVal != XSEMI_GEM_ERR_NON_TRANSACT_OV ) {
		return iRetVal;
	}
	return 0;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS16(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XData data;
	XDataWriter	writer( &pOutStream );
	switch( function ) {
	case 7:
	case 9:
//		return XSEMI_GEM_ERR_NON_TRANSACT_OV;		
		break;

	}
	return XSEMI_GEM_ERR_HEAD_FUNCTION;
XEXCEPT_EXIT
}
int XSEMI_Gem::preReceiveDataMessage(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
	return 1;
XEXCEPT_EXIT
}


void XSEMI_Gem::innerCommunicationStateChange(XSEMI_Gem::COMM_STS state)
{
XEXCEPT_ENTRY
	if( m_emCommState != state ) {
		// 無効状態から、有効関数がコールされていないのに、ステータスが河路事への対応
		if( m_emCommState == COMM_STS_DISABLE && m_pCommuIf == NULL	 ) {
			return ;
		}
		if( m_emCommState == COMM_STS_DISABLE && m_pCommuIf->isEnable() == false	 ) {
			return ;
		}



		COMM_STS preState = m_emCommState;
		m_emCommState = state;
		if(m_emCommState == COMM_STS_WAIT_DELAY && m_pCommuIf != NULL) {
			m_pCommuIf->registT6Timer( true );
		}

		long lTransitNo = -1;

		// 状態遷移のチェック
		if( preState == COMM_STS_DISABLE && m_emCommState == COMM_STS_NOT_COMUNICATION ) {
			//  2 の遷移
			lTransitNo = 2;
		} else
		if( ( preState == COMM_STS_COMUNICATION ||
			  preState == COMM_STS_NOT_COMUNICATION ||
			  preState == COMM_STS_WAIT_CR_FROM_HOST ||
			  preState == COMM_STS_WAIT_DELAY ||
			  preState == COMM_STS_WAIT_CRA  )
									&&
			m_emCommState == COMM_STS_DISABLE ) {
			//  3 の遷移
			lTransitNo = 3;
		} else
		if( preState == COMM_STS_WAIT_CR_FROM_HOST && m_emCommState == COMM_STS_NOT_COMUNICATION ) {
			//  4 の遷移
			lTransitNo = 4;
		} else
		if( preState == COMM_STS_NOT_COMUNICATION && m_emCommState == COMM_STS_WAIT_CRA ) {
			//  5 の遷移
			lTransitNo = 5;
		} else
		if( preState == COMM_STS_WAIT_CRA && m_emCommState == COMM_STS_WAIT_DELAY ) {
			//  6 の遷移
			lTransitNo = 6;
		} else
		if( preState == COMM_STS_WAIT_DELAY && m_emCommState == COMM_STS_WAIT_CRA ) {
			//  7 の遷移
			lTransitNo = 7;
		} else
		if( preState == COMM_STS_WAIT_CRA && m_emCommState == COMM_STS_COMUNICATION ) {
			//  9 の遷移
			lTransitNo = 9;
		} else
		if( preState == COMM_STS_NOT_COMUNICATION && m_emCommState == COMM_STS_WAIT_CR_FROM_HOST ) {
			// 10 の遷移
			lTransitNo = 10;
		} else
		if( preState == COMM_STS_COMUNICATION && m_emCommState == COMM_STS_NOT_COMUNICATION ) {
			// 14 の遷移
			lTransitNo = 14;
		} else
		if( preState == COMM_STS_WAIT_CR_FROM_HOST && m_emCommState == COMM_STS_COMUNICATION ) {
			// 15 の遷移
			lTransitNo = 15;
		} else
		if( preState == COMM_STS_WAIT_CR_FROM_HOST && m_emCommState == COMM_STS_WAIT_CRA ) {
			// 5 の遷移
			lTransitNo = 5;
		} else {
			XStrBuff strError;
			strError.Format(_T("Invalid Communication state Changed. Pre%d New:%d"),
					preState,
					m_emCommState
				);
			writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strError);
		}

		notifyCommunicatonState(lTransitNo, preState, m_emCommState);

		if( lTransitNo == 9 || lTransitNo == 15 ) {
			XBIN uc = (XBIN)XSEMI_GEM_CTRL_STS_ONLINE_REMOTE;
			if( m_ulVIDs[EC_INDEX_ONLINE_SUBSTS] != XSEMI_INVALID_VID) {
				XDATA_TYPE type;
				this->getVValue( m_ulVIDs[EC_INDEX_ONLINE_SUBSTS], &type, &uc, sizeof(uc));
			}
			innerNotifyContorolState((XD_UINT4)uc);
		}
	}
XEXCEPT_EXIT
}

void XSEMI_Gem::innerNotifyContorolState(XD_UINT4 sts)
{
XEXCEPT_ENTRY
	if( m_ulControlStatus != sts ) {
		// 状態変化があった
		XD_UINT4 preSts = m_ulControlStatus;
		m_ulControlStatus = sts;


		// Transit:
		//	1:	(non)	->	(any)
		//	2:	(non)	->	XSEMI_GEM_CTRL_STS_OFFLINE_EQP
		//		(non)	->	XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT
		//		(non)	->	XSEMI_GEM_CTRL_STS_OFFLINE_HOST
		//	3: XSEMI_GEM_CTRL_STS_OFFLINE_EQP		->	XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT
		//	4: XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT	->	XSEMI_GEM_CTRL_STS_OFFLINE_EQP
		//	   XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT	->	XSEMI_GEM_CTRL_STS_OFFLINE_HOST
		//	5: XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT	->	XSEMI_GEM_CTRL_STS_ONLINE_LOCAL
		//	   XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT	->	XSEMI_GEM_CTRL_STS_ONLINE_REMOTE
		//	6: XSEMI_GEM_CTRL_STS_ONLINE_LOCAL		->	XSEMI_GEM_CTRL_STS_OFFLINE_EQP
		//	   XSEMI_GEM_CTRL_STS_ONLINE_REMOTE		->	XSEMI_GEM_CTRL_STS_OFFLINE_EQP
		//	7: (null)	->	XSEMI_GEM_CTRL_STS_ONLINE_LOCAL
		//	   (null)	->	XSEMI_GEM_CTRL_STS_ONLINE_REMOTE
		//	8: XSEMI_GEM_CTRL_STS_ONLINE_LOCAL		-> XSEMI_GEM_CTRL_STS_ONLINE_REMOTE
		//	9: XSEMI_GEM_CTRL_STS_ONLINE_REMOTE		-> XSEMI_GEM_CTRL_STS_ONLINE_LOCAL
		//	10:XSEMI_GEM_CTRL_STS_ONLINE_LOCAL		->	XSEMI_GEM_CTRL_STS_OFFLINE_HOST
		//	   XSEMI_GEM_CTRL_STS_ONLINE_REMOTE		->	XSEMI_GEM_CTRL_STS_OFFLINE_HOST
		//	11:XSEMI_GEM_CTRL_STS_OFFLINE_HOST		->	XSEMI_GEM_CTRL_STS_ONLINE_LOCAL
		//	   XSEMI_GEM_CTRL_STS_OFFLINE_HOST		->	XSEMI_GEM_CTRL_STS_ONLINE_REMOTE
		//	12:XSEMI_GEM_CTRL_STS_OFFLINE_HOST		->	XSEMI_GEM_CTRL_STS_OFFLINE_EQP
		long	lTransitID = -1;
		switch( preSts ) {
		case XSEMI_GEM_CTRL_STS_OFFLINE_EQP:
			switch( m_ulControlStatus ) {
			case XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT:
				lTransitID = 3;
				break;
			}
			break;
		case XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT:
			switch( m_ulControlStatus ) {
			case XSEMI_GEM_CTRL_STS_OFFLINE_EQP:
			case XSEMI_GEM_CTRL_STS_OFFLINE_HOST:
				lTransitID = 4;
				break;
			case XSEMI_GEM_CTRL_STS_ONLINE_LOCAL:
			case XSEMI_GEM_CTRL_STS_ONLINE_REMOTE:
				lTransitID = 5;
				break;
			}
			break;
		case XSEMI_GEM_CTRL_STS_ONLINE_LOCAL:
			switch( m_ulControlStatus ) {
			case XSEMI_GEM_CTRL_STS_OFFLINE_EQP:
				lTransitID = 6;
				break;
			case XSEMI_GEM_CTRL_STS_ONLINE_REMOTE:
				lTransitID = 8;
				break;
			case XSEMI_GEM_CTRL_STS_OFFLINE_HOST:
				lTransitID = 10;
				break;
			}
			break;

		case XSEMI_GEM_CTRL_STS_ONLINE_REMOTE:
			switch( m_ulControlStatus ) {
			case XSEMI_GEM_CTRL_STS_OFFLINE_EQP:
				lTransitID = 6;
				break;
			case XSEMI_GEM_CTRL_STS_ONLINE_LOCAL:
				lTransitID = 9;
				break;
			case XSEMI_GEM_CTRL_STS_OFFLINE_HOST:
				lTransitID = 10;
				break;
			}
			break;
		case XSEMI_GEM_CTRL_STS_OFFLINE_HOST:
			switch( m_ulControlStatus ) {
			case XSEMI_GEM_CTRL_STS_ONLINE_LOCAL:
			case XSEMI_GEM_CTRL_STS_ONLINE_REMOTE:
				lTransitID = 11;
				break;
			case XSEMI_GEM_CTRL_STS_OFFLINE_EQP:
				lTransitID = 12;
				break;
			}
			break;
		default:
			switch( m_ulControlStatus ) {
			case XSEMI_GEM_CTRL_STS_ONLINE_LOCAL:
			case XSEMI_GEM_CTRL_STS_ONLINE_REMOTE:
				lTransitID = 7;
				break;
			default:
				lTransitID = 2;
				break;
			}
		}

		// 通知
		notifyContorolState(lTransitID, preSts, m_ulControlStatus);
	}
XEXCEPT_EXIT
}


void XSEMI_Gem::innerWriteLog(XD_UINT4 level, const TCHAR* format, ...)
{
XEXCEPT_ENTRY
	XStrBuff strMsg;
	va_list argList;
	va_start(argList, format);
	strMsg.FormatV(format, argList);
	va_end(argList);

	writeLogCallBack(level, strMsg);
XEXCEPT_EXIT
}
bool XSEMI_Gem::get_XXID_Format(WELLKNOWN_V_INDEX emConfigVID_Index, XDATA_TYPE* pType)
{
XEXCEPT_ENTRY
	XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
	if( m_ulVIDs[emConfigVID_Index] !=  XSEMI_INVALID_VID ) {
		unsigned char ucVidFormatType = (unsigned char)XDATA_TYPE_U4;
		if( this->getVValue(m_ulVIDs[emConfigVID_Index], NULL, &ucVidFormatType, sizeof(ucVidFormatType)) == GETV_STS_SUCCESS ) {
			vidFormatType = (XDATA_TYPE)ucVidFormatType;
		}
	}
	switch (vidFormatType)
	{
	case XDATA_TYPE_A:
	case XDATA_TYPE_U1:
	case XDATA_TYPE_U2:
	case XDATA_TYPE_U4:
	case XDATA_TYPE_U8:
	case XDATA_TYPE_I1:
	case XDATA_TYPE_I2:
	case XDATA_TYPE_I4:
	case XDATA_TYPE_I8:
		break;
	default:
		// フォーマットが不正
		innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("invalid format type. Format:(0x%X) VID:(%d)"),
			vidFormatType, m_ulVIDs[emConfigVID_Index]);
		return false;
		break;
	}
	if( emConfigVID_Index == EC_INDEX_FORMAT_ALID ) {
		// ALIDは、フォーマット：Aは指定できない。
		if( vidFormatType == XDATA_TYPE_A ) {
			// フォーマットが不正
			innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("invalid format type. Format:(0x%X) VID:(%d)"),
				vidFormatType, m_ulVIDs[emConfigVID_Index]);
			return false;
		}
	}
	*pType = vidFormatType;
	return true;
XEXCEPT_EXIT
	return false;
}
// VIDをwriterに書き込む
bool XSEMI_Gem::innerWriteVID(XDATA_TYPE vidFormatType, XSEMI_V* pV, XDataWriter& writer)
{
XEXCEPT_ENTRY
	//   ** SVID
	switch (vidFormatType)
	{
	case XDATA_TYPE_A:
		if( writer.putValAsStringA( pV->get_Name() )  == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_U1:
		if( writer.putValAsUInt1( (XD_UINT1)pV->get_VID() )  == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_U2:
		if( writer.putValAsUInt2( (XD_UINT2)pV->get_VID() )  == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_U4:
		if( writer.putValAsUInt4( (XD_UINT4)pV->get_VID() )  == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_U8:
		if( writer.putValAsUInt8( (XD_UINT8)pV->get_VID() )  == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_I1:
		if( writer.putValAsInt1( (XD_INT1)pV->get_VID() )  == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_I2:
		if( writer.putValAsInt2( (XD_INT2)pV->get_VID() )  == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_I4:
		if( writer.putValAsInt4( (XD_INT4)pV->get_VID() )  == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_I8:
		if( writer.putValAsInt8( (XD_INT8)pV->get_VID() )  == false ) {
			return false;
		}
		break;
	default:
		// フォーマットが不正
		return false;
		break;
	}
	return true;
XEXCEPT_EXIT
	return false;
}

bool XSEMI_Gem::receiveDataMessageCallBack(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
	XDATA_TYPE type;

	//
	//	API
	//
	if( m_pAPI != NULL ) {
		if( ulMsgSize < 10*1024*1024) {
			m_pAPI->receiveDataMessage(header, pMsgData, ulMsgSize);
		}
	}
	//
	// ユーザー定義の処理を実行。
	//
	int iPreStatus = preReceiveDataMessage(pPort, header, pMsgData, ulMsgSize);



	//
	//	接続処理関連
	//
	if( (int)(header.byte2 & ~0x80) == 1  ) {
		switch( header.status ) {
		case 1:
			{
				XD_UINT1 configConnect = XSEMI_GEM_CONF_DEF_CONF_CONNECT;
				if( m_ulVIDs[EC_INDEX_CONFIG_CONNECT] != (XD_UINT4)-1 ) {
					this->getVValue(m_ulVIDs[EC_INDEX_CONFIG_CONNECT], &type, &configConnect, sizeof(configConnect));
				}

				if( m_emCommState == COMM_STS_WAIT_CR_FROM_HOST && 
					this->get_connectDirection() == true && configConnect == 0 ) {	// S1F1 で接続確立

					//   モデル図の、COMMUNICATIONSの遷移  15  に遷移。
					innerCommunicationStateChange(COMM_STS_COMUNICATION);
				}
			}
			break;
		case 2:
			{
				XD_UINT1 configConnect = XSEMI_GEM_CONF_DEF_CONF_CONNECT;
				if( m_ulVIDs[EC_INDEX_CONFIG_CONNECT] != (XD_UINT4)-1 ) {
					this->getVValue(m_ulVIDs[EC_INDEX_CONFIG_CONNECT], &type, &configConnect, sizeof(configConnect));
				}
				if( configConnect == 0 ) {

					//	S1,F2 On Line Data (D) S,H⇔E
					if( ( m_emCommState == COMM_STS_WAIT_CRA )
								||
						( m_emCommState == COMM_STS_WAIT_CR_FROM_HOST   ) ) {

						do {
							COMM_STS preSts = m_emCommState;	// 前の状態を保存

							XDataReader reader(pMsgData+10, ulMsgSize-10);
							XData       data;

							if( reader.getValue( &data ) == false ) {
								// SECSメッセージとおかしい
								//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
								if( m_emCommState == COMM_STS_WAIT_CRA ) {
									innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
								}
								break;
							}
							if(m_bHost == true) {
								/*
									// ホストの場合
									構造：	i） ホストから送信するメッセージのフォーマット
												L,0
											ii） 装置から送信するメッセージのフォーマット
												L,2
													1. <A[6] MDLN> * 装置の形式
													2. <A[6] SOFTREV> * ソフトウェアのレビジョン・コード
								*/
								if( data.getType() != XDATA_TYPE_L && data.getCount() != 2 ) {
									// SECSメッセージとおかしい
									//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
									if( m_emCommState == COMM_STS_WAIT_CRA ) {
										innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
									}
									break;
								}
								if( reader.getValue( &data ) == false || data.getType() != XDATA_TYPE_A ) {
									// SECSメッセージとおかしい
									//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
									if( m_emCommState == COMM_STS_WAIT_CRA ) {
										innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
									}
									break;
								}
								if( reader.getValue( &data ) == false || data.getType() != XDATA_TYPE_A ) {
									// SECSメッセージとおかしい
									//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
									if( m_emCommState == COMM_STS_WAIT_CRA ) {
										innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
									}
									break;
								}
							} else {
								// 装置の場合
								if( data.getType() != XDATA_TYPE_L && data.getCount() != 0 ) {
									// SECSメッセージとおかしい
									//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
									if( m_emCommState == COMM_STS_WAIT_CRA ) {
										innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
									}
									break;
								}
							}
							//   モデル図の、COMMUNICATIONSの遷移  9  に遷移。
							innerCommunicationStateChange(COMM_STS_COMUNICATION);

						} while( 0 ) ;

						if( m_ulTiketWaitCRA != 0 ) {
							this->dropTicket( m_ulTiketWaitCRA );
						}
						m_ulTiketWaitCRA = 0;
					}


					if(m_bHost == false) {
						// S1,F2 On Line Data (D)
						// 装置の場合コントロール状態変更
						// コントロール状態モデル 5 の遷移
						if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_OFFLINE_ATTEMPT ) {
							// コントロール状態の変化を通知
							innerNotifyContorolState(XSEMI_GEM_CTRL_STS_ONLINE_REMOTE);
						}
					} else {
						// 装置の場合。
					}
				}
			}
			break;
		case 13:
			{
				XD_UINT1 configConnect = XSEMI_GEM_CONF_DEF_CONF_CONNECT;
				if( m_ulVIDs[EC_INDEX_CONFIG_CONNECT] != (XD_UINT4)-1 ) {
					this->getVValue(m_ulVIDs[EC_INDEX_CONFIG_CONNECT], &type, &configConnect, sizeof(configConnect));
				}

				//	S1,F13 Establish Communications Request (CR)
				if( m_emCommState == COMM_STS_WAIT_CR_FROM_HOST && 
					this->get_connectDirection() == true &&
					configConnect == 2 ) {	// S1F13 で接続確立
					//   モデル図の、COMMUNICATIONSの遷移  15  に遷移。
					innerCommunicationStateChange(COMM_STS_COMUNICATION);
				}
			}
			break;
		case 14:
			{
				XD_UINT1 configConnect = XSEMI_GEM_CONF_DEF_CONF_CONNECT;
				if( m_ulVIDs[EC_INDEX_CONFIG_CONNECT] != (XD_UINT4)-1 ) {
					this->getVValue(m_ulVIDs[EC_INDEX_CONFIG_CONNECT], &type, &configConnect, sizeof(configConnect));
				}

				//	S1,F14 Establish Communications Request Acknowledge (CRA)
				if( ( m_emCommState == COMM_STS_WAIT_CRA &&  configConnect == 2 )
							||
					( m_emCommState == COMM_STS_WAIT_CR_FROM_HOST && configConnect == 2 ) ) {
					
					do {
						// S1F13 で接続確立
						
						COMM_STS preSts = m_emCommState;	// 前の状態を保存

						// COMMACK を確認
						XDataReader reader(pMsgData+10, ulMsgSize-10);
						XData       data;

						if( reader.getValue( &data ) == false ) {
							// SECSメッセージとおかしい
							//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
							if( m_emCommState == COMM_STS_WAIT_CRA ) {
								innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
							}
							break;
						}
						if( data.getType() != XDATA_TYPE_L && data.getCount() != 2 ) {
							// SECSメッセージとおかしい
							//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
							if( m_emCommState == COMM_STS_WAIT_CRA ) {
								innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
							}
							break;
						}
						if( reader.getValue( &data ) == false ) {
							// SECSメッセージとおかしい
							//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
							if( m_emCommState == COMM_STS_WAIT_CRA ) {
								innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
							}
							break;
						}
						if( data.getType() != XDATA_TYPE_B && data.getSize() != 1 ) {
							// SECSメッセージとおかしい
							//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
							if( m_emCommState == COMM_STS_WAIT_CRA ) {
								innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
							}
							break;
						}
						if( data.getValAsUInt1() != 0 ) {
							// SECSメッセージとおかしい
							//   モデル図の、COMMUNICATIONSの遷移  6  に遷移。
							if( m_emCommState == COMM_STS_WAIT_CRA ) {
								innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
							}
							break;
						}
						//   モデル図の、COMMUNICATIONSの遷移  9  に遷移。
						innerCommunicationStateChange(COMM_STS_COMUNICATION);
					} while( 0 );
					if( m_ulTiketWaitCRA != 0 ) {
						this->dropTicket( m_ulTiketWaitCRA );
					}
					m_ulTiketWaitCRA = 0;
				}
			}
			break;
		case 16:
			// S1,F16 OFF-LINE Acknowledge (OFLA)
			if( m_bHost == true ) {
				// ACK を確認
				XDataReader reader(pMsgData+10, ulMsgSize-10);
				XData       data;

				// <B[1] OFLACK> * オフライン要求に対する確認コード
				if( reader.getValue( &data ) == true ) {
					if( data.getType() == XDATA_TYPE_B &&
						data.getCount() == 1 && 
						data.getValAsUInt1() == 0) {
						// コントロール状態の変化を通知
						innerNotifyContorolState( XSEMI_GEM_CTRL_STS_OFFLINE_HOST );
					}
				}
			}
			break;
		case 18:
			// S1,F18 ON-LINE Acknowledge (ONLA)
			if( m_bHost == true ) {
				XDataReader reader(pMsgData+10, ulMsgSize-10);
				XData       data;

				// <B[1] OFLACK> * オフライン要求に対する確認コード
				if( reader.getValue( &data ) == true ) {
					if( data.getType() == XDATA_TYPE_B &&
						data.getCount() == 1 && 
						data.getValAsUInt1() == 0) {
						// コントロール状態の変化を通知
						
						XBIN uc = (XBIN)XSEMI_GEM_CTRL_STS_ONLINE_REMOTE;
						if( m_ulVIDs[EC_INDEX_ONLINE_SUBSTS] != XSEMI_INVALID_VID) {
							XDATA_TYPE type;
							this->getVValue( m_ulVIDs[EC_INDEX_ONLINE_SUBSTS], &type, &uc, sizeof(uc));
						}
						innerNotifyContorolState((XD_UINT4)uc);
					}
				}
			}
			break;
		}
	}
	if( (int)(header.byte2 & ~0x80) == 2  && this->get_isHost() == true ) {		//

		switch( header.status ) {
		case 18:
			//	時間を同期する。
			iPreStatus = processGemMessageS2F18(header, pMsgData, ulMsgSize);
			break;
		}
	}
	if( (int)(header.byte2 & ~0x80) == 5  && this->get_isHost() == true ) {
		//
		//	Host で且つアラーム・リスト・データを受信した時は、ローカルデータと同期する。
		//
		switch( header.status ) {
		case 6:
		case 8:
			innerSyncroAlarm(header, pMsgData, ulMsgSize);
			break;
		}

	}
	//
	//	ユーザー定義の処理で実装された場合はココでおしまい。
	//
	if( iPreStatus == XSEMI_GEM_ERR_NON_TRANSACT_OV )	 {
		// ユーザーサイドで処理された
		return true;
	}





	XSECSHeader respHead;
	m_repBuff.resetp();
	XDataWriter	writer( &m_respStream );

	//
	//	その他特別な処理
	//
	if( (int)(header.byte2 & ~0x80) == 2  ) {
		switch( header.status ) {
		case 25:
			// LooBackの場合は、受信メッセージをそのまま、レスポンスにセット
			m_respStream.write((const char*)pMsgData+10, ulMsgSize-10);
			break;
		}
	}


	if( (header.status % 2) != 0  && (int)(header.byte2 & ~0x80) != 9 ) {	// プライマリー、メッセージ
		int sts = iPreStatus;
		if( iPreStatus == XSEMI_GEM_ERR_NON_TRANSACT_DEF ) {
			// WBITがONの場合、規定処理を実行し
			XDataReader reader(pMsgData+10, ulMsgSize-10);
			sts = processGemMessage((header.byte2 & ~0x80), header.status, reader, m_respStream);
		}
		switch( sts ) {
		case XSEMI_GEM_ERR_NON_TRANSACT_OV:
			{

				if( XSecsData::get_wait(header) == true  ) {
					::memcpy(&respHead, &header, sizeof(respHead));
					respHead.byte2 &= ~0x80;
					respHead.status++;
					pPort->sendResponse(&respHead, m_repBuff.getBuff(), m_repBuff.size());
				}
				return true;
			}
			break;
		case XSEMI_GEM_ERR_DATA:
			{
				// S9,F7 Illegal Data (IDN)
				::memcpy(&respHead, &header, sizeof(respHead));
				writer.putValAsBinary( (const XBIN *)&respHead, sizeof(respHead) );

				XSecsData::setSxFx(respHead, 9, 7, false);
				pPort->sendRequest(&respHead, m_repBuff.getBuff(), m_repBuff.size());
			}
			break;
		case XSEMI_GEM_ERR_HEAD_STREAM:
			{
				// S9,F3 Unrecognized Stream Type (USN)
				::memcpy(&respHead, &header, sizeof(respHead));
				writer.putValAsBinary( (const XBIN *)&respHead, sizeof(respHead) );

				XSecsData::setSxFx(respHead, 9, 3, false);
				pPort->sendRequest(&respHead, m_repBuff.getBuff(), m_repBuff.size());
			}
			break;
		case XSEMI_GEM_ERR_HEAD_FUNCTION:
			{
				// S9,F5 Unrecognized Function Type (UFN)
				::memcpy(&respHead, &header, sizeof(respHead));
				writer.putValAsBinary( (const XBIN *)&respHead, sizeof(respHead) );

				XSecsData::setSxFx(respHead, 9, 5, false);
				pPort->sendRequest(&respHead, m_repBuff.getBuff(), m_repBuff.size());
			}
			break;
		case XSEMI_GEM_ERR_ABORT:
			{
				// Abort Transaction
				::memcpy(&respHead, &header, sizeof(respHead));
				XSecsData::set_function(respHead, 0 );
				XSecsData::set_wait(respHead, false );
				pPort->sendRequest(&respHead, NULL, 0);

			}
		}
	}




	if( (int)(header.byte2 & ~0x80) == 7 && (header.byte2 & 0x80) != 0 ) {
		switch( header.status ) {
		case 27:
			// S7,F27 Process Program Verification Send (PVS)
			{
				::memcpy(&respHead, &header, sizeof(respHead));
				respHead.byte2 &= ~0x80;
				respHead.status++;
				pPort->sendResponse(&respHead, m_repBuff.getBuff(), m_repBuff.size());
			}
			break;
		case 29:
			// S7,F29 Process Program Verification Inquire (PVI)
			{
				XBIN PPGNT = 0;
				writer.putValAsBinary( &PPGNT, 1 );
				::memcpy(&respHead, &header, sizeof(respHead));
				respHead.byte2 &= ~0x80;
				respHead.status++;
				pPort->sendResponse(&respHead, m_repBuff.getBuff(), m_repBuff.size());
			}
			break;
		}	
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_Gem::receiveHSMSCtrlCallBack(XSECSHeader& header)
{
XEXCEPT_ENTRY
	return true;
XEXCEPT_EXIT
}
void XSEMI_Gem::sendDataMessageCallBack(XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
void XSEMI_Gem::sendHSMSCtrlCallBack(XSECSHeader& header)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
bool XSEMI_Gem::connectEventCallBack(XSeceCommItf* pPort, XSECS_CONN_STS sts)
{
XEXCEPT_ENTRY

	if( sts == XSECS_CONN_STS_CONNECTED && 
		this->get_connectDirection() == true  ) {
		// Active & 接続したとき
		
		// 相手からS1F13を待つ状態
		innerCommunicationStateChange(COMM_STS_WAIT_CR_FROM_HOST);
	} else	
	if( sts == XSECS_CONN_STS_NOT_CONNECTED ) {
		// 切断したとき
		
		innerCommunicationStateChange(COMM_STS_NOT_COMUNICATION);
	} else	
	if( sts == XSECS_CONN_STS_SELECT_ACCEPT && 
		this->get_connectDirection() == false  ) {
		// Select.req	を受け付けた, かつ、Passiveの場合は、S1F13を送信する。
		innerCommunicateStateProcess(sts);
	}
	return true;
XEXCEPT_EXIT
}
void XSEMI_Gem::notifyContorolState(long lTransitID, XD_UINT4 preStat, XD_UINT4 newStat)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

void XSEMI_Gem::t3TimeOutCallBack(XD_UINT4 ulTicket, XD_UINT1 stream, XD_UINT1 function)
{
XEXCEPT_ENTRY
	if( stream == 1 && function == 13 && m_emCommState == COMM_STS_WAIT_CRA ) {
		// 通信確立でタイムアウトした。
		innerCommunicationStateChange(COMM_STS_WAIT_DELAY);
	}

XEXCEPT_EXIT
}

void XSEMI_Gem::t6TimeOutCallBack()
{
XEXCEPT_ENTRY

	innerCommunicationStateChange(COMM_STS_WAIT_CRA);
	// 
	if( m_pCommuIf != NULL ) {
		innerCommunicateStateProcess( m_pCommuIf->get_status() );
	}
XEXCEPT_EXIT
}
void XSEMI_Gem::intervalTimerCallBack()
{
XEXCEPT_ENTRY
	if( get_CommunicationState() != COMM_STS_COMUNICATION &&
		get_ConnectState()       == XSECS_CONN_STS_CONNECTED) {
		// 通信確立を送信
		innerCommunicateStateProcess( XSECS_CONN_STS_CONNECTED );
	}
XEXCEPT_EXIT
}

void XSEMI_Gem::notifyCommunicatonState(long lTransitID, COMM_STS preSts, COMM_STS newSts)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XSEMI_Gem::_API::_API(XSEMI_Gem* pGem)
	:	m_pGEM( pGem )
{
XEXCEPT_ENTRY
	m_ifType = XSECS_COMMIF_TYPE_API;
	m_pProccessingClientData = 0;
	m_ulProccessingMsgID = 0;
	m_ulCurrentClientID = 0;
	m_pHandler = pGem;
XEXCEPT_EXIT
}
XSEMI_Gem::_API::~_API()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT

}
void XSEMI_Gem::_API::receiveRequest(ClinetData* pClient, XD_UINT4 ulMsgID, void *pvData, size_t dataSize)
{
XEXCEPT_ENTRY
	XBIN *pData = (XBIN*)pvData;
	XD_UINT2	ulCommand;

	m_pProccessingClientData = 0;
	m_ulProccessingMsgID = 0;
	m_ulCurrentClientID = 0;

	::memcpy(&ulCommand, pData, sizeof(XD_UINT2));
	pData   += sizeof(XD_UINT2);
	dataSize -= sizeof(XD_UINT2);
	if( ulCommand == XSEMI_GEMAPI_CMD_SEND_PRIMARY ) {
		//
		//	プライマリーメッセージ送信コマンド
		//
		m_pProccessingClientData = pClient;
		m_ulProccessingMsgID = ulMsgID;
		m_ulCurrentClientID = pClient->getClinetID();

		// Primary
		XD_UINT4 ulSize;
		
		::memcpy(&ulSize, pData, sizeof(XD_UINT4));
		ulSize = ::ntohl(ulSize);
		pData    += sizeof(XD_UINT4);
		dataSize -= sizeof(XD_UINT4);

		XSECSHeader* pHeader = (XSECSHeader*)pData;
		if( ulSize == dataSize ) {
			XSECSMSG		rspmsg;
			XD_UINT4		ulSECSRspDataSize = 0;
			XSECS_SEND_STS sts = m_pGEM->sendRequest( (XSECSHeader*)pData, &pData[10], dataSize-10 , &rspmsg);
			if( sts == XSECS_SEND_STS_S_AND_HAS_REP ) {
				rspmsg.ulDataSizeWithHead = htonl(rspmsg.ulDataSizeWithHead);
			} else {
				rspmsg.ulDataSizeWithHead = 0;
			}
			// 返信伝聞を作成
			ulCommand = XSEMI_GEMAPI_CMD_RECEIVE_SECONDRY;
			XInflateStreamBuff	buff(256);
			std::iostream		stream(&buff);
			stream.write( (char*)&ulCommand, sizeof(XD_UINT2) );
			stream.write( (char*)&sts, sizeof(XD_UINT4) );
			stream.write( (char*)&ulSECSRspDataSize, sizeof(XD_UINT4) );
			stream.write( (char*)rspmsg.pDataWithHead, rspmsg.ulDataSizeWithHead );

			// 返信
			pClient->sendResponse(ulMsgID, buff.getBuff(), buff.size());

			m_pGEM->dropTicket( XSecsData::get_ticket(*pHeader) );
		}
	}
XEXCEPT_EXIT
}

void XSEMI_Gem::_API::ntfyConnection(XD_UINT8 ulClientID, bool bConnect)
{
XEXCEPT_ENTRY
	m_ulCurrentClientID = ulClientID;
	m_pGEM->connectEventCallBack( this, bConnect == true ? XSECS_CONN_STS_CONNECTED : XSECS_CONN_STS_DISCONNECT);
XEXCEPT_EXIT
}
XStrBuff XSEMI_Gem::_API::getPeerName()
{
XEXCEPT_ENTRY
	XStrBuff str;
	XMsgQueueClient::ClinentID clientID;
	clientID.m_id8 = m_ulCurrentClientID;

	str.Format(_T("%08X_%08X"), clientID.m_id4[0], clientID.m_id4[1] );

	return str;
XEXCEPT_EXIT
}
bool XSEMI_Gem::_API::_writeMessage(const void *pData, size_t size)
{
XEXCEPT_ENTRY
	if( m_pProccessingClientData == NULL ) {
		return false;
	}
	m_pProccessingClientData->sendResponse(m_ulProccessingMsgID, pData, size);
	
	return true;
XEXCEPT_EXIT
}

void XSEMI_Gem::_API::receiveDataMessage(const XSECSHeader& header, const XBIN* pMsgData,const XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
	// cmd:2
	// msg_size:4
	// msg_data:n (SECSHeader+Data)
	//#define	XSEMI_GEMAPI_CMD_RECEIVE_MSG			3

	m_buffSendMsg.resetp();

	XD_UINT2	ulCommand = XSEMI_GEMAPI_CMD_RECEIVE_MSG;
	XD_UINT4	ulSECSRspDataSize;
	std::iostream		stream(&m_buffSendMsg);
	ulSECSRspDataSize = htonl(ulMsgSize);
	stream.write( (char*)&ulCommand, sizeof(XD_UINT2) );
	stream.write( (char*)&ulSECSRspDataSize, sizeof(XD_UINT4) );

	stream.write( (char*)pMsgData, ulMsgSize );

	XBIN * pSendBuff = m_buffSendMsg.getBuff();
	this->sendEvent(pSendBuff, m_buffSendMsg.size());
XEXCEPT_EXIT
}

