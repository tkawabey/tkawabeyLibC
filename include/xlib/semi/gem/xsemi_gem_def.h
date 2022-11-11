#ifndef __XSECS_GEM_DEF_H__
#define __XSECS_GEM_DEF_H__

/*
EC
	EngTime
	EstablishCommunicationTimeOut
	InterruptionCtr
	MaxSpoolTransmit
	NSTime
	OverWriteSpool
	PrdRecovery
	PrdTime
	SbyRecovery
	SbyTime
	SDTime
	TimeFormat
	UDTime
SV
	AlarmsEnabled
	AlarmsSet
	ARAMSAccumReset
	ARAMSInfo
	ARAMSState
	ARAMSText
	ARAMSTimeStamp
	CLOCK
	ControlState
	CycleCtr
	DowntimeAlarm
	DowntimeText
	DowntimeDate
	EqpModel
	EqpName
	EqpSerialNum
	EventEnabled
	LastPowerdown
	PowerdownTime
	PowerupState
	PPExecName
	PPFormat
	PrdState
	PrevARAMSState
	PreviousProcessState
	ProcessState
	RcpExecName
	SpoolCountActual
	SpoolTotal
	SpoolFullTime
	SpoolStartTime
	SymptomID
	SymptomText
DV
	AlarmID
	EventLimit
	LimitVariable
	OperatorCommand
	PPChangeName
	PPChangeStates
	PPError
	RcpChangeName
	RcpChangeStates
	TrasitionType
	TRATID
	TRJOBIDENT
	TRJOBNM
	TRLNK

*/


// 固定CONFIG.ConfName の名前
#define XSEMI_GEM_CONF_MDLN					_T("GemMDLN")				// モデル名
#define XSEMI_GEM_CONF_SOFTREV				_T("GemSOFTREV")			// ソフトウェアリビジョン
#define XSEMI_GEM_CONF_CONF_CONNECT			_T("GemConfigConnect")		// 接続確立のSxFx。0:S1F1 1:S1F65 2:S1F13
#define XSEMI_GEM_CONF_CONST_FILE			_T("GemConstantFileName")	// 
#define XSEMI_GEM_CONF_ESTAB_COMM_DELAY		_T("GemEstabCommDelay")		// S1F13の送信周期
#define XSEMI_GEM_CONF_INIT_COMM_STATE		_T("GemInitCommState")		// 初期通信状態(1: ENABLED/0: DISABLED)
#define XSEMI_GEM_CONF_INIT_CTRL_STATE		_T("GemInitControlState")	// 初期コントロール状態
#define XSEMI_GEM_CONF_OFFLINE_SUB_STATE	_T("GemOfflineSubstate")	// OffLineサブステート
#define XSEMI_GEM_CONF_ONLINE_SUB_STATE		_T("GemOnlineSubstate")		// OnLineサブステート
#define XSEMI_GEM_CONF_ONLINE_FAILED		_T("GemOnlineFailed")		// OnLine失敗時の遷移ステータス
#define XSEMI_GEM_CONF_POOL_DELAY			_T("GemPollDelay")			// S1F1の送信周期
#define XSEMI_GEM_CONF_BUFFER_SIZE			_T("GemBufferSize")			// 最大バッファサイズ	U4
// Format系
#define XSEMI_GEM_CONF_FORMAT_VID			_T("GemVIDFormat")			// EC:VIDのフォーマット A, U*, I*の何れか
#define XSEMI_GEM_CONF_FORMAT_TRID			_T("GemTRIDFormat")			// EC:TRIDのフォーマット A, U*, I*の何れか
#define XSEMI_GEM_CONF_FORMAT_RPTID			_T("GemRPTIDFormat")		// EC:RPTIDのフォーマット A, U*, I*の何れか
#define XSEMI_GEM_CONF_FORMAT_DSID			_T("GemDSIDFormat")			// EC:DSIDのフォーマット A, U*, I*の何れか
#define XSEMI_GEM_CONF_FORMAT_DATAID		_T("GemDATAIDFormat")		// EC:DATAIDのフォーマット A, U*, I*の何れか
#define XSEMI_GEM_CONF_FORMAT_ALID			_T("GemALIDFormat")			// EC:ALIDのフォーマット A, U*, I*の何れか
#define XSEMI_GEM_CONF_FORMAT_CEID			_T("GemCEIDFormat")			// EC:CEIDのフォーマット A, U*, I*の何れか


#define XSEMI_GEM_CONF_CTRL_STS				_T("GemControlState")
#define XSEMI_GEM_CONF_PRE_CTRL_STS			_T("GemPreviousControlState")

// 動的イベント設定/変更
#define XSEMI_GEM_CONF_CONFIG_EVENT			_T("GemConfigEvents")			// EC:
#define XSEMI_GEM_CONF_RP_TYPE				_T("GemRpType")					// EC:
#define XSEMI_GEM_CONF_WBIT_6				_T("GemWBitS6")					// EC:

// Limit
#define XSEMI_GEM_CONF_LIMIT_DELAY			_T("GemLimitsDelay")			// EC:リミット監視データ収集間隔

// Spool
#define XSEMI_GEM_CONF_CONFIG_SPOOL				_T("GemConfigSpool")			// EC:スプーリング使用可否 0 = Disable 1 = Enable
#define XSEMI_GEM_CONF_MAX_SPOOL_SIZE			_T("GemMaxSpoolSize")			// EC:スプールファイルのサイズ
#define XSEMI_GEM_CONF_MAX_SPOOL_TRANSMIT		_T("GemMaxSpoolTransmit")		// EC:最大スプール転送メッセージ数 0 = No limit. Other = Number of messages.
#define XSEMI_GEM_CONF_SPPOL_CNT_ACTL			_T("GemSpoolCountActual")		// SV:
#define XSEMI_GEM_CONF_SPOOL_CNT_TOTAL			_T("GemSpoolCountTotal")		// SV:
#define XSEMI_GEM_CONF_OVR_WRITE_SPOOL			_T("GemOverWriteSpool")			// EC:オーバーライトスプール
#define XSEMI_GEM_CONF_SPOOL_FULL_TIME			_T("GemSpoolFullTime")			// SV:YYYYMMDDhhmmsscc
#define XSEMI_GEM_CONF_SPOOL_LOAD_SUBST			_T("GemSpoolLoadSubstate")		// SV:6 = Spool Not Full  7 = Spool Full
#define XSEMI_GEM_CONF_SPOOL_UNLOAD_SUBST		_T("GemSpoolUnloadSubstate")	// SV:3 = Purging Spool.  4 = Transmitting Spool.  5 = No Spool Output.
#define XSEMI_GEM_CONF_SPOOL_STAT				_T("GemSpoolState")				// SV:1 = Spool Inactive. 2 = Spool Active.
#define XSEMI_GEM_CONF_SPOOL_START_TIME			_T("GemSpoolStartTime")			// SV:YYYYMMDDhhmmsscc
#define XSEMI_GEM_CONF_SPOOL_ACTIVATE			_T("GemSpoolingActivated")		// Event:
#define XSEMI_GEM_CONF_SPOOL_DEACTIVATE			_T("GemSpoolingDeactivated")	// Event:
#define XSEMI_GEM_CONF_SPOOL_TRANS_FAIL			_T("GemSpoolTransmitFailure")	// Event:
#define XSEMI_GEM_CONF_SPOOL_FILENAME			_T("GemSpoolFileName")			// EC: スプールファイル名
#define XSEMI_GEM_CONF_OTH_ACTIVE_GEM_CONNECT	_T("GemActiveGemConnect")		// EC: アクティブなGEM接続（S1F13を送信するか）を設定。



/*
Variable Name Class Format Min Lth Max Lth
GemAbortLevel SV U1 1 1
GemAlarmFileName EC A 1 128
GemAlarmId SV U4 1 1
GemAlarmsEnabled SV+Loc L 0 0
GemAlarmsSet SV+Loc L 0 0
GemAlarmState SV U1 1 1
GemALIDFormat EC U1 1 1
GemASer SV U4 1 1
Table of GCD Keywords
149
GemCEIDFormat EC U1 1 1
GemClock SV+Loc A 16 16
GemConfigAlarms EC U1 1 1
GemConfigConnect EC U1 1 1
GemConfigEvents EC U1 1 1
GemConfigSpool EC U1 1 1
GemConstantFileName EC A 1 128
GemControlState SV+Loc U1 1 1
GemDataId SV U4 1 1
GemDATAIDFormat EC U1 1 1
GemDSIDFormat EC U1 1 1
GemDynamicDataLimit EC U4 1 1
GemEstabCommDelay EC U2 1 1
GemEventLimit DV B 1 1
GemEventsEnabled SV+Loc L 0 0
GemEventText DV A 5 24
GemHostId EC U2 1 1
GemHostMessageLimit EC U4 1 1
GemInitCommState EC U1 1 1
GemInitControlState EC U1 1 1
GemLimitsDelay EC U2 1 1
GemLimitsFileName EC A 1 128
GemLimitsVID DV U4 1 1
GemLinkState SV +Loc U1 1 1
GemMaxSpoolFileSize EC U4 1 1
Variable Name Class Format Min Lth Max Lth
2. GCD Data Requirements
150
GemMaxSpoolTransmit EC U4 1 1
GemMinimumInterval EC U4 1 1
GemMDLN SV A 0 6
GemOfflineSubstate EC U1 1 1
GemOnlineFailed EC U1 1 1
GemOnlineSubstate EC U1 1 1
GemOverWriteSpool EC BOOLEAN 1 1
GemPollDelay EC U2 1 1
GemPPChangeName DV A 0 80
GemPPChangeStatus DV U1 1 1
GemPPDir EC A 0 128
GemPPExecName SV A 0 80
GemPPExt EC A 0 40
GemPPKeepSecsHeader EC BOOLEAN 1 1
GemPPTempFileName EC A 1 128
GemPPTocName EC A 1 128
GemPreviousCeid SV U4 0 1
GemPreviousCommand SV A 0 20
GemPreviousControlState SV U1 0 1
GemPreviousProcessState
SV U1 0 1
GemProcessState SV U1 1 1
GemRecipeType EC U1 1 1
GemReportFileName EC A 1 128
GemRPTIDFormat EC U1 1 1
Variable Name Class Format Min Lth Max Lth
Table of GCD Keywords
151
GemRpType EC BOOLEAN 1 1
GemSharedMemorySize EC U4 1 1
GemSOFTREV SV A 0 6
GemSpoolCountActual SV+Loc U4 1 1
GemSpoolCountTotal SV+Loc U4 1 1
GemSpoolFileName EC A 1 128
GemSpoolFullTime SV+Loc A 0 16
GemSpoolLoadSubstate SV+Loc U1 1 1
GemSpoolState SV+Loc U1 1 1
GemSpoolStartTime SV+Loc A 0 16
GemSpoolUnloadSubstate
SV+Loc U1 1 1
GemTermReqSendMax EC I4 1 1
GemTermReqRecvMax EC I4 1 1
GemTime SV+Loc A 12 12
GemTimeFormat EC U1 1 1
GemTransType DV B 1 1
GemTRIDFormat EC U1 1 1
GemVIDFormat EC U1 1 1
GemWBitS5 EC U1 1 1
GemWBitS6 EC U1 1 1
GemWBitS10 EC U1 1 1*/

#define XSEMI_GEM_CONF_DEF_MDLN				"Model"
#define XSEMI_GEM_CONF_DEF_SOFTREV			"1.0"
#define XSEMI_GEM_CONF_DEF_CONF_CONNECT		2	// 接続確立のSxFx。デフォルト値：2:S1F13
#define XSEMI_GEM_CONF_DEF_CONF_CONNECT_MAX		2
#define XSEMI_GEM_CONF_DEF_CONF_CONNECT_MIN		0
#define XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY		30	//
#define XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY_MAX		120	//
#define XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY_MIN		1	//
#define XSEMI_GEM_CONF_DEF_INIT_COMM_STATE			1
#define XSEMI_GEM_CONF_DEF_INIT_COMM_STATE			1
#define XSEMI_GEM_CONF_DEF_INIT_COMM_STATE_MAX		1
#define XSEMI_GEM_CONF_DEF_INIT_COMM_STATE_MIN		0
#define XSEMI_GEM_CONF_DEF_INIT_CTRL_STATE			2
#define XSEMI_GEM_CONF_DEF_INIT_CTRL_STATE_MAX		2
#define XSEMI_GEM_CONF_DEF_INIT_CTRL_STATE_MIN		1
#define XSEMI_GEM_CONF_DEF_OFFLINE_SUB_STATE		1
#define XSEMI_GEM_CONF_DEF_OFFLINE_SUB_STATE_MAX	3
#define XSEMI_GEM_CONF_DEF_OFFLINE_SUB_STATE_MIN	1
#define XSEMI_GEM_CONF_DEF_ONLINE_SUB_STATE			4
#define XSEMI_GEM_CONF_DEF_ONLINE_SUB_STATE_MAX		5
#define XSEMI_GEM_CONF_DEF_ONLINE_SUB_STATE_MIN		4
#define XSEMI_GEM_CONF_DEF_ONLINE_FAILED			1
#define XSEMI_GEM_CONF_DEF_ONLINE_FAILED_MAX		3
#define XSEMI_GEM_CONF_DEF_ONLINE_FAILED_MIN		1
#define XSEMI_GEM_CONF_DEF_POOL_DELAY				0
#define XSEMI_GEM_CONF_DEF_POOL_DELAY_MAX			1800
#define XSEMI_GEM_CONF_DEF_POOL_DELAY_MIN			0
#define XSEMI_GEM_CONF_DEF_CONFIG_EVENT				1
#define XSEMI_GEM_CONF_DEF_CONFIG_EVENT_VS			0	// S6F3,F9
#define XSEMI_GEM_CONF_DEF_CONFIG_EVENT_EVENT		1	// S6F11,F13
#define XSEMI_GEM_CONF_DEF_CONFIG_EVENT_MAX			1
#define XSEMI_GEM_CONF_DEF_CONFIG_EVENT_MIN			0
#define XSEMI_GEM_CONF_DEF_RP_TYPE					false
#define XSEMI_GEM_CONF_DEF_WBIT_6					1
#define XSEMI_GEM_CONF_DEF_WBIT_6_MAX				1
#define XSEMI_GEM_CONF_DEF_WBIT_6_MIN				0
#define XSEMI_GEM_CONF_DEF_LIMITS_DELAY				5
#define XSEMI_GEM_CONF_DEF_LIMITS_DELAY_MAX			65535
#define XSEMI_GEM_CONF_DEF_LIMITS_DELAY_MIN			0
#define XSEMI_GEM_CONF_DEF_CONFIG_SPOOL				0
#define XSEMI_GEM_CONF_DEF_CONFIG_SPOOL_MAX			1
#define XSEMI_GEM_CONF_DEF_CONFIG_SPOOL_MIN			0
#define XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE			1024*6
#define XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE_MAX		99999
#define XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE_MIN		0
#define XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT		0
#define XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT_MAX	9999
#define XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT_MIN	0
#define XSEMI_GEM_CONF_DEF_OVR_WRITE_SPOOL			false
#define XSEMI_GEM_CONF_DEF_SPOOL_LOAD_SUBST			6
#define XSEMI_GEM_CONF_DEF_SPOOL_LOAD_SUBST_MAX		7
#define XSEMI_GEM_CONF_DEF_SPOOL_LOAD_SUBST_MIN		6
#define XSEMI_GEM_CONF_DEF_SPOOL_UNLOAD_SUBST		5
#define XSEMI_GEM_CONF_DEF_SPOOL_UNLOAD_SUBST_MAX	5
#define XSEMI_GEM_CONF_DEF_SPOOL_UNLOAD_SUBST_MIN	3
#define XSEMI_GEM_CONF_DEF_SPOOL_STAT				1
#define XSEMI_GEM_CONF_DEF_SPOOL_STAT_MAX			1
#define XSEMI_GEM_CONF_DEF_SPOOL_STAT_MIN			2
#define XSEMI_GEM_CONF_DEF_BUFFER_SIZE				0x00000FFF
#define XSEMI_GEM_CONF_DEF_BUFFER_SIZE_MAX			0x000FFFFF
#define XSEMI_GEM_CONF_DEF_BUFFER_SIZE_MIN			0x00000FFF

#endif /* __XSECS_GEM_DEF_H__ */
