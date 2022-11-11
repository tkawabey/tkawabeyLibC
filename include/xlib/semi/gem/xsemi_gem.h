/**
 * @file xsemi_gem.h
 * @brief SEMI:GEM����
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

#define XSEMI_GEM_ERR_NON_TRANSACT_DEF			0	// �f�t�H���g���������s����
#define XSEMI_GEM_ERR_NON_TRANSACT_OV			1	// �I�[�o�[���C�g�Ŏ��������B
#define XSEMI_GEM_ERR_IN_DATA					2	// ���̓f�[�^���f�[�^���s���ł��B
#define XSEMI_GEM_ERR_OUT_BUFF					3	// �o�̓X�g���[���ɂ������߂܂���B
#define XSEMI_GEM_ERR_HEAD_STREAM				-2	// ���b�Z�[�W�E�u���b�N�E�w�b�_���̃X�g���[���E�^�C�v�����u���Ŗ���`�ł���B
#define XSEMI_GEM_ERR_HEAD_FUNCTION				-3	// ���b�Z�[�WID���̃t�@���N�V�����E�^�C�v�����u���Ŗ���`�ł���B
#define XSEMI_GEM_ERR_DATA						-4	// �X�g���[���ƃt�@���N�V�����͗����ł������A�f�[�^�E�t�H�[�}�b�g�����߂ł��Ȃ��B
#define XSEMI_GEM_ERR_TRANSACT_TIMEOUT			-5	// �g�����U�N�V�����^�C�}�E�^�C���A�E�g
#define XSEMI_GEM_ERR_OVER_LENGTH				-6	// �f�[�^����������
#define XSEMI_GEM_ERR_TIMEO						-7	// ��b�^�C���A�E
#define XSEMI_GEM_ERR_DEVAICE_ID				-9	// �f�o�C�XID������`
#define XSEMI_GEM_ERR_ABORT						-10	// Abort���b�Z�[�W�𑗐M����B


#define XSEMI_INVALID_VID						(XD_UINT4)-1
#define XSEMI_INVALID_CEID						(XD_UINT4)-1
#define XSEMI_INVALID_ALID						(XD_UINT4)-1



/**	@brief	GEM�N���X */
class XLIB_API XSEMI_Gem : public XSecsCommHandler, public XSEMI_GemConstObject
{
public:
	/**	@brief	�ʐM��ԃ��f���̃X�e�[�^�X */
	enum COMM_STS {
		COMM_STS_DISABLE			= 0,		//<	
		COMM_STS_ENABLE				= 1,		//<	
		COMM_STS_COMUNICATION		= 2,		//<	
		COMM_STS_NOT_COMUNICATION	= 3,		//<	
		COMM_STS_WAIT_CR_FROM_HOST	= 4,		//<	
		COMM_STS_WAIT_DELAY			= 5,		//<	
		COMM_STS_WAIT_CRA			= 6,		//<	
	};

	/**	@brief	getVValue�̃G���[�R�[�h */
	enum GETV_STS {
		GETV_STS_SUCCESS			= 0,		//<	�����B
		GETV_STS_NOT_FOUND_VID		= 1,		//<	�w�肳�ꂽVID��������Ȃ��B
		GETV_STS_NO_IMPLE			= 2,		//< �g���擾��`�Ɏ����������B
		GETV_STS_ERROR				= 100,		//< ���̑��G���[
	};


	/**	@brief	putVValue�̃G���[�R�[�h */
	enum PUTV_STS {
		PUTV_STS_SUCCESS			= 0,		//<	�����B
		PUTV_STS_NOT_FOUND_VID		= 1,		//<	�w�肳�ꂽVID��������Ȃ��B
		PUTV_STS_NO_IMPLE			= 2,		//< �g���擾��`�Ɏ����������B
		GETV_STS_DIFF_DATA_TYPE		= 3,		///< �w�肳�ꂽ�f�[�^�^�C�v�ƈقȂ�
		PUTV_STS_ERROR				= 100,		//< ���̑��G���[
	};

	enum WELLKNOWN_V_INDEX {
		EC_INDEX_MDLN		= 0,		///<	EC.���f��
		EC_INDEX_SOFTREV,				///<	EC.�\�t�g���r�W����
		EC_INDEX_CONFIG_CONNECT,		///<	EC.�ڑ��m����SxFx�B0:S1F1 1:S1F65 2:S1F13
		EC_INDEX_CONFIG_ESTABLISH_COMM_DELAY,	///<	EC.�ڑ��m����SxFx�̑��M����
		EC_INDEX_INIT_COMM_STS,			///<	EC.�����ʐM���(1: ENABLED/0: DISABLED)
		EC_INDEX_INIT_CTRL_STS,			///<	EC.�����R���g���[�����(1:Offline/2:Online)
		EC_INDEX_ONLINE_SUBSTS,			///<	EC.OnLine�T�u�X�e�[�g(4: online/local    5: online/remote)
		EC_INDEX_OFFLINE_SUBSTS,		///<	EC.OffLine�T�u�X�e�[�g	(1: offline/equipment offline   2: offline/attempt online    3: offline/host offline)
		EC_INDEX_ONLINE_FILED,			///<	EC.OnLine���s���̑J�ڃX�e�[�^�X(1: offline/equipment offline 3: offline/host offlin)
		EC_INDEX_POLL_DELAY,			///<	EC.S1F1�̑��M����
		EC_INDEX_BUFFER_SIZE,			///<	EC.�ő�o�b�t�@�T�C�Y
		// Format�n
		EC_INDEX_FORMAT_VID,			///<	EC.VID�̃t�H�[�}�b�g
		EC_INDEX_FORMAT_TRID,			///<	EC.TRID�̃t�H�[�}�b�g
		EC_INDEX_FORMAT_RPTID,			///<	EC.RPTID�̃t�H�[�}�b�g
		EC_INDEX_FORMAT_DSID,			///<	EC.DSID�̃t�H�[�}�b�g
		EC_INDEX_FORMAT_DATAID,			///<	EC.DATAID�̃t�H�[�}�b�g
		EC_INDEX_FORMAT_ALID,			///<	EC.ALID�̃t�H�[�}�b�g
		EC_INDEX_FORMAT_CEID,			///<	EC.CEID�̃t�H�[�}�b�g
		// ���I�C�x���g�ݒ�/�ύX
		EC_INDEX_CONFIG_EVENT,			///<	EC.�C�x���g���|�[�g�̗L��
		EC_INDEX_RP_TYPE,				///<	EC.�C�x���g���|�[�g�̃��|�[�g�^�^�C�v
		EC_INDEX_WBIT6,					///<	EC.�C�x���g���|�[�g��Wbit���Z�b�g���邩�ǂ���
		// Limit
		EC_INDEX_LIMIT_DELAY,			///<	EC.���~�b�g�Ď��f�[�^���W�Ԋu
		// Spool 
		EC_INDEX_CONFIG_SPOOL,			///<	EC.�X�v�[���̗L��
		EC_INDEX_MAX_SPOOL_SIZE,		///<	EC.�X�v�[������o�b�t�@�̗e��
		EC_INDEX_MAX_SPOOL_TRANMIT,		///<	EC.�ő�X�v�[����
		SV_INDEX_SPOOL_CNT_ACTUAL,		///<	SV.���X�v�[����
		SV_INDEX_SPOOL_CNT_TOTAL,		///<	SV.�X�v�[�����ꂽ��
		EC_INDEX_OV_WRITE_SPOOL,		///<	EC.SpoolFull���A�X�v���f�[�^���Â��f�[�^����㏑�����邩�ǂ���
		SV_INDEX_SOOL_FULLTIME,			///<	SV.SpoolFull���̎���
		SV_INDEX_SPOOL_STS,				///<	SV.Spool���
		SV_INDEX_SPOOL_LOAD_SUBSTS,		///<	SV.SpoolLoad�̏��
		SV_INDEX_SPOOL_UNLOAD_STS,		///<	SV.SpoolUnLoad�̏��
		SV_INDEX_SPOOL_START_TIME,		///<	SV.Spool���J�n��������
		SV_INDEX_SPOOL_FILE_NAME,		///<	EC.�X�v�[���t�@�C����

		EC_INDEX_OTH_ACTIVE_GEM_CONNECT,	///<	EC.�A�N�e�B�u��GEM�ڑ��iS1F13�𑗐M���邩�j��ݒ�B

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


	/**	@brief	API�R�}���h��M�̏��������܂��B
	 *
	 *	@param	pInitName	��������
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
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
	/**	@brief	�o�b�t�@�T�C�Y���擾���܂��B
	 *
	 *	@return	�o�b�t�@�T�C�Y���擾
	 */
	XD_UINT4 get_BufferSize();
// ����
	void getGemTime(XStrBuff &strTime);
	XTime getGemTime();
	void getTimeString(XTime& obTime, XStrBuff &strTime);
// �R���g���[���n
	XD_UINT4 get_ControlState() const;
	int chCtrlStateOffline();
	int chCtrlStateOnline();
	int chCtrlStateLocal();
	int chCtrlStateRemote();
// �ʐM��Ԍn
	XSEMI_Gem::COMM_STS get_CommunicationState() const;
	int chEnable();
	int chDisable();

// �ʐM�n
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
	/**	@brief	�`�P�b�g�����o�^����Ă��邩�ǂ����m�F���܂��B
	 *
	 *	@param	ulTicket	�`�P�b�g�i���o�[
	 *
	 *	@return	true	�`�P�b�g���o�^����Ă���
	 *	@return	false	�`�P�b�g���o�^����Ă��Ȃ�
	 */
	virtual bool isHaveTicket(XD_UINT4 ulTicket);
	/**	@brief	�`�P�b�g�����擾���܂��B
	 *
	 *	@param	ulTicket	�`�P�b�g�i���o�[
	 *	@param	pTicketInf	�`�P�b�g�����󂯎��o�b�t�@
	 *
	 *	@return	true	�`�P�b�g���o�^����Ă���
	 *	@return	false	�`�P�b�g���o�^����Ă��Ȃ�
	 */
	virtual bool getTicketInfo(XD_UINT4 ulTicket, XSeceCommItf::TICKET_INF* pTicketInf);
// �C�x���g
	/**	@brief	�w�肵��CEID�̃C�x���g�𑗐M���܂��B
	 *
	 *	@param	ulCEID	CEID
	 *
	 *	@retval	0	���M���܂���
	 *	@retval	-1	�w�肵��CEID��������Ȃ�
	 *	@retval	-2	���M���b�Z�[�W�̍쐬�Ɏ��s�����B
	 *	@retval	-3	���b�Z�[�W�𑗐M�ł��Ȃ�����
	 */
	int raiseEvent(XD_UINT4 ulCEID);

	/**	@brief	Const���ōX�V�����C�x���g�̃��|�[�g�����N�����R�~�b�g���܂��B	 */
	void commitLinkEventReport();
// �A���[��
	/**	@brief	�A���[���𑗐M���܂��B
	 *
	 *	@param	alid	�A���[��ID
	 *	@param	alcd	�A���[���R�[�h
	 *	@param	p_altx	�A���[���e�L�X�g
	 *
	 *	@retval	0	����
	 *	@retval	-1	�w�肳�ꂽ�A���[����������Ȃ�
	 *	@retval	-2	���M���b�Z�[�W�̍쐬�Ɏ��s�����B
	 *	@retval	-3	���b�Z�[�W�𑗐M�ł��Ȃ�����
	 *	@retval	-4	�w�肳�ꂽ�A���[���͖������
	 */
	int raiseAlarm(XD_UINT4 alid, XD_UINT1 alcd, const TCHAR* p_altx = NULL);

// �X�v�[��
	/**	@brief	�X�v�[����Ԃ��ω�����
	 *
	 *	@param	ulPreSatus	�O�̏��
	 *	@param	ulNewState	�V�������	
	 */
	virtual void notifySpoolState(XD_UINT4 ulPreSatus, XD_UINT4 ulNewState);

	/**	@brief	�X�v�[���ݒ肳�ꂽStream��Function�̐ݒ���擾����B
	 *
	 *	@param	stream	�ݒ���o�͂���X�g���[��
	 *
	 *	@return	int
	 *
	 *	@note	�擾�ł���A�f�[�^�\���́AS2,F43 Reset Spooling Streams and Functions (RSSF)
	 *			�Ɠ����`���B
	 *			�\���F L,m
	 *					1. L,2
	 *						1. <U1 STRID1> * �X�g���[������
	 *						2. L,n
	 *							1. <U1 FCNID1> * �t�@���N�V��������
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

// V�l�擾�E�X�V
	/**	@brief	WELLKNOWN_V_INDEX�Ŏw�肵��VID���擾
	 *
	 *	@param	index	VID�̎��
	 *
	 *	@return	VID
	 */
	XD_UINT4 getWellKnownVID(WELLKNOWN_V_INDEX index);

	/**	@brief	���O���w�肵�āAVID���擾
	 *
	 *	@param	p_name	V�̖��O
	 *
	 *	@return	VID	
	 */
	XD_UINT4 getVID_by_Name(const TCHAR* p_name);

	/**	@brief	�w�肵��V�̒l���擾���܂��B
	 *
	 *	@param	vid	�擾����VID���w�肵�܂��B
	 *	@param	type	�w�肵��VID�̌^���󂯎��o�b�t�@
	 *	@param	stream	�l���Z�b�g����X�g���[��
	 *
	 *	@return	�擾�X�e�[�^�X
	 */
	GETV_STS getVValue(XD_UINT4 vid, XDATA_TYPE *type, std::ostream *stream);

	/**	@brief	�w�肵��V�̒l���擾���܂��B
	 *
	 *	@param	vid	�擾����VID���w�肵�܂��B
	 *	@param	type	�w�肵��VID�̌^���󂯎��o�b�t�@
	 *	@param	writer	���C�^�[
	 *
	 *	@return	�擾�X�e�[�^�X
	 */
	GETV_STS getVValue(XD_UINT4 vid, XDATA_TYPE *type, XDataWriter& writer);

	/**	@brief	�w�肵��V�̒l���擾���܂��B
	 *
	 *	@param	vid	�擾����VID���w�肵�܂��B
	 *	@param	type	�w�肵��VID�̌^���󂯎��o�b�t�@
	 *	@param	pValue	�l���󂯎��o�b�t�@
	 *	@param	nsize	�o�b�t�@�̃T�C�Y�A
	 *
	 *	@return	�擾�X�e�[�^�X
	 */
	GETV_STS getVValue(XD_UINT4 vid, XDATA_TYPE *type, void* pValue, XD_UINT4 nsize);

	/**	@brief	�w�肵��V�̒l���擾���܂��B
	 *
	 *	@param	vid	�擾����VID���w�肵�܂��B
	 *	@param	rData	�l���󂯎��XRichData�B
	 *
	 *	@return	�擾�X�e�[�^�X
	 */
	GETV_STS getVValue(XD_UINT4 vid, XRichData& rData);

	/**	@brief	V�̒l���X�V����B
	 *
	 *	@param	vid	VID
	 *	@param	pValue	�l
	 *	@param	nsize	pValue�̃T�C�Y
	 *
	 *	@return	�ݒ�X�e�[�^�X
	 */
	PUTV_STS putVValue(XD_UINT4 vid, const void* pValue, XD_UINT4 nsize);

	/**	@brief	V�̒l���X�V����B
	 *
	 *	@param	vid	VID
	 *	@param	rData	�ݒ�f�[�^
	 *
	 *	@return	�ݒ�X�e�[�^�X
	 */
	PUTV_STS putVValue(XD_UINT4 vid, XRichData& rData);


// �v���Z�X�n
	/**	@brief	�w�肵��SxFx�̏��������s���A�o�̓X�g���[���̌��ʂ��������݂܂��B
	 *
	 *	@param	strem	STREAM
	 *	@param	function	FUNCTION
	 *	@param	inData	���̓f�[�^
	 *	@param	pOutStream	�o�̓X�g���[��
	 *
	 *	@return	0		�����ɂ̖���SxFx���w�肳�ꂽ�B
	 *	@return	1		���������s���܂����B
	 *	@return	2		���̓f�[�^���f�[�^���s���ł��B
	 *	@return	3		�o�̓X�g���[���ɂ������߂܂���B
	 *	@retval	-2		���b�Z�[�W�E�u���b�N�E�w�b�_���̃X�g���[���E�^�C�v�����u���Ŗ���`�ł���B
	 *	@retval	-3		���b�Z�[�WID���̃t�@���N�V�����E�^�C�v�����u���Ŗ���`�ł���B
	 *	@retval	-4		�X�g���[���ƃt�@���N�V�����͗����ł������A�f�[�^�E�t�H�[�}�b�g�����߂ł��Ȃ��B
	 *	@retval	-6		�f�[�^����������
	 *
	 *	@verbatim
	 *
	 *	@endverbatim
	 */
	int processGemMessage(XD_UINT1 strem, XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream);
	int processGemMessage(XSecsData& data);
	int processGemMessage(XSecsData& data, std::ostream& pOutStream);
// �R�[���o�b�N
	/**	@brief	�A���[���������ɒʒm�����
	 *
	 *	@param	alarmID	�A���[��ID
	 *	@param	alcd	�A���[���E�R�[�h
	 *	@param	pTxt	�A���[���E�e�L�X�g
	 *
	 *	@retval	0	��������
	 *	@retval	!0	�����o���Ȃ�
	 */
	virtual XD_UINT1 occurreAlarm(const XD_UINT4 alarmID, const XD_UINT1 alcd, const TCHAR* pTxt);
	/**	@brief	GetEx�o�R��V�l�擾�������s��
	 *
	 *	@param	ceid	CEID�A�C�x���g���M�o�R��V�l�擾�Ŗ����ꍇ�́A�O
	 *	@param	vid	VID
	 *	@param	type	�f�[�^�^�C�v
	 *	@param	stream	�������݃X�g���[��
	 *
	 *	@return	�擾�X�e�[�^�X
	 *
	 *	@note	type�����X�g�̏ꍇ�́A<L. .. >�@�`��
	 *			type�����X�g�̈ȊO�̏ꍇ�́A�o�C�i���[�f�[�^���̂��̂��ݒ肷��B
	 */
	virtual GETV_STS externGetVValue(const XD_UINT4 ceid, const XD_UINT4 vid, const XDATA_TYPE type, std::ostream *stream);

	/**	@brief	PutEx�o�R��V�l�ݒ�������s��
	 *
	 *	@param	vid	VID
	 *	@param	type	�f�[�^�^�C�v
	 *	@param	pValue	�������ރf�[�^
	 *	@param	ulValueSize	�f�[�^�T�C�Y
	 *
	 *	@return	�ݒ�X�e�[�^�X
	 *
	 *	@note	type�����X�g�̏ꍇ�́A<L. .. >�@�`��
	 *			type�����X�g�̈ȊO�̏ꍇ�́A�o�C�i���[�f�[�^���̂��̂��ݒ肷��B	
	 */
	virtual PUTV_STS externPutVValue(const XD_UINT4 vid, const XDATA_TYPE type, const void* pValue, XD_UINT4 ulValueSize);

	/**	@brief	�ʐM���肩��A��M�������b�Z�[�W���������܂��B
	 *
	 *	@param	pPort	�ʐM�I�u�W�F�N�g
	 *	@param	header	SECS���b�Z�[�W�w�b�_�[
	 *	@param	pMsgData	�f�[�^
	 *	@param	ulMsgSize	�f�[�^�T�C�Y
	 *
	 *	@retval	0		�f�t�H���g���������s����B
	 *	@retval	1		�I�[�o�[���C�g�Ŏ��������B
	 *	@retval	-2		���b�Z�[�W�E�u���b�N�E�w�b�_���̃X�g���[���E�^�C�v�����u���Ŗ���`�ł���B
	 *	@retval	-3		���b�Z�[�WID���̃t�@���N�V�����E�^�C�v�����u���Ŗ���`�ł���B
	 *	@retval	-4		�X�g���[���ƃt�@���N�V�����͗����ł������A�f�[�^�E�t�H�[�}�b�g�����߂ł��Ȃ��B
	 *	@retval	-5		�g�����U�N�V�����^�C�}�E�^�C���A�E�g
	 *	@retval	-6		�f�[�^����������
	 *	@retval	-7		��b�^�C���A�E
	 *	@retval	-8		�f�o�C�XID������`
	 *	@retval	���̑�	�G���[�����������B
	 */
	virtual int preReceiveDataMessage(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);

	/**	@brief	HSMS�R���g���[���f�[�^���擾���܂��B
	 *
	 *	@param	header	SECS���b�Z�[�W�w�b�_�[
	 *
	 *	@retval	true	1�����b�Z�[�W�̏ꍇ�Q�����b�Z�[�W��Ԃ��܂��B
	 *	@retval	false	1�����b�Z�[�W�̏ꍇ�Q�����b�Z�[�W��Ԃ��܂���B
	 */
	virtual bool receiveHSMSCtrlCallBack(XSECSHeader& header);

	virtual void sendDataMessageCallBack(XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);

	virtual void sendHSMSCtrlCallBack(XSECSHeader& header);

	virtual bool connectEventCallBack(XSeceCommItf* pPort, XSECS_CONN_STS sts);

	/**	@brief	�R���g���[����Ԃ̕ύX��ʒm���܂��B
	 *
	 *	@param	lTransitID	�w���ԍ��B�s���ȏꍇ��-1
	 *	@param	preStat	�O�̏��
	 *	@param	newStat	�V�������
	 */
	virtual void notifyContorolState(long lTransitID, XD_UINT4 preStat, XD_UINT4 newStat);
	/**	@brief	T3�^�C���A�E�g�������������Ƃ�ʒm���܂��B
	 *
	 *	@param	ulTicket	�`�P�b�g
	 *	@param	stream	�X�g���[��
	 *	@param	function	�t�@���N�V����
	 */
	virtual void t3TimeOutCallBack(XD_UINT4 ulTicket, XD_UINT1 stream, XD_UINT1 function);
	/**	@brief	T6�^�C���A�E�g�������������Ƃ�ʒm���܂��B	 */
	virtual void t6TimeOutCallBack();
	/**	@brief	�C���^�[�o���^�C�}�[�����B	 */
	virtual void intervalTimerCallBack();
	/**	@brief	�ʐM��Ԃ̕ύX��ʒm���܂��B
	 *
	 *	@param	lTransitID	�w���ԍ��B�s���ȏꍇ��-1
	 *	@param	preStat	�O�̏��
	 *	@param	newStat	�V�������
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
	// VID��CEID�̃t�H�[�}�b�g���擾���܂��B
	bool get_XXID_Format(WELLKNOWN_V_INDEX emConfigVID_Index, XDATA_TYPE* pType);
	// VID��writer�ɏ�������
	bool innerWriteVID(XDATA_TYPE vidFormatType, XSEMI_V* pV, XDataWriter& writer);

	/*	@brief	�f�[�^���b�Z�[�W���擾�����Ƃ��R�[������܂��B
	 *
	 *	@param	pPort	�ʐM�I�u�W�F�N�g
	 *	@param	header	SECS���b�Z�[�W�w�b�_�[
	 *	@param	pMsgData	�f�[�^
	 *	@param	ulMsgSize	�f�[�^�T�C�Y
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
	XSEMI_GemConst		m_constData;		// �ݒ�l�n���b�v
	XSeceCommItf*		m_pCommuIf;			// �ʐM�C���^�[�t�F�[�X

private:
	std::ostream		m_respStream;
	XInflateStreamBuff	m_repBuff;
	XMutex				m_lock;


	XRingBuffer			m_spoolBuff;		// �X�v�[���p�̃o�b�t�@

	bool				m_bInit;			// �������ς݃t���O
	bool				m_bHost;
	XD_UINT4			m_ulDeviceID;		//	�f�o�C�XID
	XD_UINT4			m_ulSequcialDATAID;	//
	XD_UINT4			m_ulControlStatus;	// Gem�R���g���[�����

	XTimeNonRTC			m_time;				// ����
//	Spool
	XD_UINT4			m_ulSpoolState;		// �X�v�[�����
	XSEMI_Spool			*m_pobjSpool;

	COMM_STS			m_emCommState;		// �ʐM���
	XTime				m_tmWaitDelay;		// WAIT_DELAY���^�C���A�E�g���鎞��
	double				m_dCommDelayTime;	// �O��ڑ��v���𑗐M��������
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
	_API*				m_pAPI;				// API�̃R�}���h����M����


	XD_UINT4			m_ulVIDs[V_INDEX_MAX];
	XD_UINT4			m_ulEVENTIndex[EV_INDEX_MAX];

friend class _API;
friend class XSEMI_Spool;
};


#include "xlib/xlib_tail.h"
#endif /* __XSECS_GEM_H__ */