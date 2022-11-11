/**
 * @file xsecs_comm.h
 * @brief �����̐���SEMI�K�i SECS�f�[�^����N���X
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


/**	@brief	SECS�ʐM�����ɑ΂���R�[���o�b�N�C���^�[�t�F�[�X���`���܂��B	*/
class XLIB_API XSecsCommHandler
{
public:
	/**	@brief	�R���X�g���N�^	*/
	XSecsCommHandler();
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSecsCommHandler();

	
	/**	@brief	���݂̃��O���x�����擾���܂�
	 *
	 *	@return	���O���x��
	 */
	XD_UINT4 getLogLevel() const;
	/**	@brief	���O���x�����Z�b�g���܂��B
	 *
	 *	@param	level	���O���x��
	 */
	void     putLogLevel(const XD_UINT4 level);
	/**	@brief	�w�肳�ꂽ���O���x�������ݒ肳��Ă��邩���ׂ܂��B
	 *
	 *	@param	level	���O���x��
	 *
	 *	@retval	true	�ݒ肳��Ă���
	 *	@retval	false	�ݒ肳��Ă��Ȃ�
	 */
	bool isSetLogLevel(long level);


// �R�[���o�b�N�֐�
	/**	@brief	���O���b�Z�[�W�R�[���o�b�N
	 *
	 *	@param	loglevel	���O���x��
	 *	@param	pMessage	���O���b�Z�[�W
	 */
	virtual void writeLogCallBack(long loglevel, const TCHAR* pMessage);
	/**	@brief	�f�[�^���b�Z�[�W���擾�����Ƃ��R�[������܂��B
	 *
	 *	@param	pPort	�ʐM�I�u�W�F�N�g
	 *	@param	header	SECS���b�Z�[�W�w�b�_�[
	 *	@param	pMsgData	�f�[�^
	 *	@param	ulMsgSize	�f�[�^�T�C�Y
	 *
	 *	@return	bool
	 */
	virtual bool receiveDataMessageCallBack(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize) = 0;

	/**	@brief	�R���g���[���f�[�^���擾���܂��B
	 *
	 *	@param	header	SECS���b�Z�[�W�w�b�_�[
	 *
	 *	@retval	true	1�����b�Z�[�W�̏ꍇ�Q�����b�Z�[�W��Ԃ��܂��B
	 *	@retval	false	1�����b�Z�[�W�̏ꍇ�Q�����b�Z�[�W��Ԃ��܂���B
	 */
	virtual bool receiveHSMSCtrlCallBack(XSECSHeader& header) = 0;

	/**	@brief	�f�[�^���b�Z�[�W���M�O�ɃR�[������܂��B
	 *
	 *	@param	header	SECS���b�Z�[�W�w�b�_�[
	 *	@param	pMsgData	�f�[�^
	 *	@param	ulMsgSize	�f�[�^�T�C�Y
	 */
	virtual void sendDataMessageCallBack(XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);

	/**	@brief	2�����b�Z�[�W��M�R�[���o�b�N.sendRequest�ŁAbWait��false���w�肵���ꍇ�̂݃R�[�������]
	 *
	 *	@param	pPort	�ʐM�I�u�W�F�N�g
	 *	@param	header	SECS���b�Z�[�W�w�b�_�[
	 *	@param	pMsgData	�f�[�^
	 *	@param	ulMsgSize	�f�[�^�T�C�Y
	 *	 
	 *  @note	receiveDataMessageCallBack�֐��̌�ɃR�[������܂��B
	 */
	virtual void receiveResponseByNoWaitCallBack(XSeceCommItf* pPort, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);
// hor HSMS
	/**	@brief	�R���g���[���f�[�^���M�O�ɃR�[������܂��B
	 *
	 *	@param	header	SECS���b�Z�[�W�w�b�_�[	
	 */
	virtual void sendHSMSCtrlCallBack(XSECSHeader& header);
	/**	@brief	�s�A�̐ڑ���Ԃ��ω��������Ƃ�m�点�܂��B
	 *
	 *	@param	sts	�ڑ��X�e�[�^�X
	 *
	 *	@return	virtual bool
	 */
	virtual bool connectEventCallBack(XSeceCommItf* pPort, XSECS_CONN_STS sts);


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

// SECS1
	/**	@brief	�n���h�V�F�[�N�R�[�h�𑗐M���́A��M�����B
	 *	@param	bIsSend	true:���M/false:��M
	 *	@param	val		�n���h�V�F�[�N�R�[�h
	 */
	virtual void SECS1_occurreHandShakeCode(bool bIsSend, XD_UINT1 val);
	/**	@brief	���Ғl�łȂ��n���h�V�F�[�N�R�[�h����M����
	 *	@param	val	�n���h�V�F�[�N�R�[�h
	 */
	virtual void SECS1_unexpectHandShakeCode(XD_UINT1 val);
	/**	@brief	�u���b�N�𑗐M���M�����B
	 *
	 *	@param	bIsSend	true:���M/false:��M
	 *	@param	header	�w�b�_�[
	 *	@param	pMsgData	�f�[�^
	 *	@param	ulMsgSize	�f�[�^�T�C�Y
	 */
	virtual void SECS1_occurreBlock(bool bIsSend, XSECSHeader& header, XBIN* pMsgData,XD_UINT4 ulMsgSize);
protected:
	XD_UINT4			m_ulLogLevel;	// ���O���x��
};


/**	@brief	�ʐM�C���^�[�t�F�[�X�̃x�[�X�N���X*/
class XLIB_API XSeceCommItf 
{
public:
	/**	@brief �`�P�b�g�X�e�[�^�X */
	enum TICKET_STATUS {
			NO_WAIT							///<	�҂��Ă��Ȃ�
		,	WAIT_RESERVE					///<	�\��
		,	WAITTING						///<	�҂��Ă���
		,	ALREADY_WAITING					///<	���ɑ҂��Ă���
		,	TIMEO							///<	�^�C���A�E�g������
		,	CANCEL_WAIT						///<	wait���L�����Z������
		,	RCVED							///<	�f�[�^����M����
	};

	/**	@brief �`�P�b�g��� */
	typedef struct tagTICKET_INF
	{
		XD_UINT2		m_usTicket;			///<	�`�P�b�g�ԍ�
		TICKET_STATUS	m_status;			///<	�X�e�[�^�X
		XD_UINT1		m_send_stream;		///<	���M�X�g���[���[
		XD_UINT1		m_send_function;	///<	���M�t�@���N�V����
		XSECSHeader		m_resp_header;		///<	���X�|���X���b�Z�[�W�w�b�_�[
		unsigned char*	m_resp_data;		///<	���X�|���X���b�Z�[�W�f�[�^
		XD_UINT4		m_resp_data_size;	///<	���X�|���X���b�Z�[�W�f�[�^�T�C�Y
		XTime			m_reg_time;			///<	�o�^���ꂽ����
		XTime			m_rcv_time;			///<	��M��������
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
	/**	@brief	1�����b�Z�[�W�̑��M
	 *
	 *	@param	pData	���M�f�[�^
	 *	@param	bWait	2�����b�Z�[�W����M����܂őҋ@���邩�ǂ����H
	 *					W-BIT���n�m�Ŗ����ꍇ�͖��������B
	 *	@param	pRep	2�����b�Z�[�W�󂯎��A�h���X
	 *					W-BIT���n�m�AbWait��true�Ŗ����ꍇ�͖��������B

	 *	@param	nTimeOut	�^�C���A�E�g�l�B-1���w�肷��ƁA�s�R�^�C���A�E�g�l���w�肳���B
	 *					W-BIT���n�m�AbWait��true�Ŗ����ꍇ�͖��������B
	 *
	 *  @note	1�����b�Z�[�W�̑��M�ɐ������āAW-BIT��ON�̏ꍇ�́A�`�P�b�g���o�^����܂��B
	 *	@return	���M�X�e�[�^�X�B
	 */
	virtual	XSECS_SEND_STS sendRequest(XSecsData *pData, XSECSMSG* pRep = NULL, bool bWait = false, int nTimeOut = -1);
	/**	@brief	1�����b�Z�[�W�̑��M
	 *
	 *	@param	pData	���M�f�[�^
	 *	@param	pRcvData	2�����b�Z�[�W�󂯎��f�[�^�̃A�h���X�B
	 *	@param	bWait	2�����b�Z�[�W����M����܂őҋ@���邩�ǂ����H
	 *					W-BIT���n�m�Ŗ����ꍇ�͖��������B
	 *	@param	bDropTicket	2�����b�Z�[�W��M��ɁA�`�P�b�g���폜���邩�ǂ����H
	 *	@param	nTimeOut	�^�C���A�E�g�l�B-1���w�肷��ƁA�s�R�^�C���A�E�g�l���w�肳���B
	 *					W-BIT���n�m�AbWait��true�Ŗ����ꍇ�͖��������B
	 *
	 *  @note	1�����b�Z�[�W�̑��M�ɐ������āAW-BIT��ON�̏ꍇ�́A�`�P�b�g���o�^����܂��B
	 *	@return	���M�X�e�[�^�X�B
	 */
	virtual	XSECS_SEND_STS sendRequest(XSecsData *pData, XSecsData *pRcvData, bool bWait = true, bool bDropTicket = true, int nTimeOut = -1);
	/**	@brief	1�����b�Z�[�W�̑��M
	 *
	 *	@param	pHeader	���M���b�Z�[�W�̃��b�Z�[�W�w�b�_
	 *	@param	pData	���M���b�Z�[�W�f�[�^
	 *	@param	ulDataSize	���M���b�Z�[�W�f�[�^�T�C�Y
	 *	@param	pRep	2�����b�Z�[�W�󂯎��A�h���X
	 *					W-BIT���n�m�AbWait��true�Ŗ����ꍇ�͖��������B
	 *	@param	bWait	2�����b�Z�[�W����M����܂őҋ@���邩�ǂ����H
	 *					W-BIT���n�m�Ŗ����ꍇ�͖��������B
	 *	@param	nTimeOut	�^�C���A�E�g�l�B-1���w�肷��ƁA�s�R�^�C���A�E�g�l���w�肳���B
	 *					W-BIT���n�m�AbWait��true�Ŗ����ꍇ�͖��������B
	 *
	 *  @note	1�����b�Z�[�W�̑��M�ɐ������āAW-BIT��ON�̏ꍇ�́A�`�P�b�g���o�^����܂��B
	 *	@return	���M�X�e�[�^�X�B
	 */
	virtual XSECS_SEND_STS sendRequest(XSECSHeader* pHeader, XBIN *pData, XD_UINT4 ulDataSize, XSECSMSG* pRep = NULL, bool bWait = false, int nTimeOut = -1);
	/**	@brief	2�����b�Z�[�W�̑��M
	 *
	 *	@param	pHeader	���M���b�Z�[�W�̃��b�Z�[�W�w�b�_
	 *	@param	pData	���M���b�Z�[�W�f�[�^
	 *	@param	ulDataSize	���M���b�Z�[�W�f�[�^�T�C�Y
	 *
	 *	@return	���M�X�e�[�^�X�B	
	 */
	virtual XSECS_SEND_STS sendResponse(XSECSHeader* pHeader, XBIN *pData, XD_UINT4 ulDataSize);
	/**	@brief	2�����b�Z�[�W�̑��M
	 *
	 *	@param	pData	���M���b�Z�[�W�f�[�^
	 *
	 *	@return	���M�X�e�[�^�X�B	
	 */
	virtual	XSECS_SEND_STS sendResponse(XSecsData *pData);
	/**	@brief	2�����b�Z�[�W�̃f�[�^���擾
	 *
	 *	@param	ulTicket	�`�P�b�g�i���o�[
	 *	@param	*ppResposeData	2�����b�Z�[�W�󂯎��A�h���X
	 *	@param	pulResponseDataSize	2�����b�Z�[�W�̃f�[�^�T�C�Y���󂯎��o�b�t�@
	 *	@param	nTimeOut	�^�C���A�E�g�l�B-1���w�肷��ƁA�s�R�^�C���A�E�g�l���w�肳���B
	 *
	 *	@return	���M�X�e�[�^�X�B	
	 */
	virtual	XSECS_SEND_STS poll(XD_UINT4 ulTicket, XBIN **ppResposeData , XD_UINT4 *pulResponseDataSize, XD_UINT4 ulTimeO = (XD_UINT4)-1);
	/**	@brief	2�����b�Z�[�W�̎�M���L�����Z������B
	 *
	 *	@param	ulTicket	�`�P�b�g�i���o�[
	 *
	 *	@return	���M�X�e�[�^�X�B		
	 */
	virtual XSECS_SEND_STS cancelWait(XD_UINT4 ulTicket);
	/**	@brief	�w�肵���`�P�b�g�̃X�e�[�^�X���擾����B
	 *
	 *	@param	ulTicket	�`�P�b�g�i���o�[
	 *
	 *	@return	�`�P�b�g�X�e�[�^�X�B
	 */
	virtual int getTicketStatus(XD_UINT4 ulTicket);
	/**	@brief	�`�P�b�g���폜����B
	 *
	 *	@param	ulTicket	�`�P�b�g�i���o�[
	 *
	 *	@return	true	����
	 *	@return	false	���s
	 */
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
	virtual bool getTicketInfo(XD_UINT4 ulTicket, TICKET_INF* pTicketInf);

	/**	@brief	�C���^�[�t�F�[�X���������܂��B
	 *
	 *	@param	bEnable	�L��/����
	 *	@param	bIsActive	Active/Pasive
	 *	@param	usDeviceID	�f�o�C�XID
	 *	@param	pHandler	�n���h���[
	 *
	 *	@return	true	����
	 *	@return	false	���s
	 */
	bool initilize(bool bEnable, bool bIsActive, XD_UINT2 usDeviceID, XSecsCommHandler* pHandler);
	/**	@brief	�C���^�[�t�F�[�X����܂��B
	 *
	 *	@return	true	����
	 *	@return	false	���s	
	 */
	bool closeIf();
	/**	@brief	�ʐM��L���ɂ��܂��B
	 *
	 *	@return	true	����
	 *	@return	false	���s
	 */
	virtual bool enable();
	/**	@brief	�ʐM�𖳌��ɂ��܂��B
	 *
	 *	@return	true	����
	 *	@return	false	���s
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
		XTime					m_time;				// �o�^���ꂽ����
		XTime					m_Rcvtime;			// ��M��������
		XTime					m_T3TimeOutTime;	// T3�^�C���A�E�g���������鎞��
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
	// T3�^�C�}�[�X���b�h
	static void* _T3Timer(void *pMe);
	void* T3Timer();
	static void* mainProc(void *pUserData);


// Active�ڑ��X���b�h
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

	bool					m_bOpend;	//  �I�[�v�����Ă��邩�ǂ���
	bool					m_bEnable;	//	�L��/����


	XList					m_tickets;
	XMutex					m_lock;
	XMutex					m_lockTicket;
	XInflateStreamBuff		m_sendTempBuff;	//���M���b�Z�[�W�̈ꎞ�I�ȃo�b�t�@

	XSECS_COMMIF_TYPE		m_ifType;

friend class TicketData;
};

#include "xlib/xlib_tail.h"
#endif /*__X_SECS_COMMUNICATION_HEASER_FILE__*/
