/**
 * @file XSecsHSMSPort.h
 * @brief �����̐���SEMI�K�i�@HSMS�ʐM�C���^�[�t�F�[�X
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
// std::deque�́Adll����EXPORTS�ł��Ȃ��̂ŁAvector�ő�p����
typedef XLIB_API std::vector<int*> XSecsHSMSPortQUEUE;



/**	@brief	SEMI-HSMS�ʐM���̎���	*/
class XLIB_API XSEMI_HSMS : public XSeceCommItf
{
public:
	/**	@brief	�R���X�g���N�^	*/
	XSEMI_HSMS();
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSEMI_HSMS();

	XSEMI_HSMS& setAddress(XSocketAddress &address);
	XSEMI_HSMS& setSockOptReuseAddr(bool val);

	XSocketAddress& getAddress();
	XSocketAddress& getSocketName();
	XSocketAddress& getPeerSocketName();
	/**	@brief	�Z�p���[�g�^�C���A�E�g�ł���T5�^�C���A�E�g���w�肵�܂��BTCP/IP�ڑ��v�����o���Ԋu��b�Ŏ擾�B�W���l��"10"�b�ł��B
	 *	@return	T5�^�C���A�E�g
	 */
	XD_UINT4 getOpt_T5() const ;
	/**	@brief	�Z�p���[�g�^�C���A�E�g�ł���T5�^�C���A�E�g��ݒ肵�܂��BTCP/IP�ڑ��v�����o���Ԋu��b�Ŏ擾�B�W���l��"10"�b�ł��B
	 *	@param	val	T5�^�C���A�E�g
	 */
	void     setOpt_T5(XD_UINT4 val);
	/**	@brief	NOT SELECTED�^�C���A�E�g�ł���T7�^�C���A�E�g���w�肵�܂��BTCP/IP�ڑ����s���Ă���ǂꂮ�炢Select.req��҂��Ƃ����҂����Ԃ�b�Ŏw�肵�܂��B�W���l��"10"�b�ł��B
	 *	@return	T7�^�C���A�E�g
	 */
	XD_UINT4 getOpt_T7() const ;
	/**	@brief	NOT SELECTED�^�C���A�E�g�ł���T7�^�C���A�E�g��ݒ肵�܂��BTCP/IP�ڑ����s���Ă���ǂꂮ�炢Select.req��҂��Ƃ����҂����Ԃ�b�Ŏw�肵�܂��B�W���l��"10"�b�ł��B
	 *	@param	val	T7�^�C���A�E�g
	 */
	void     setOpt_T7(XD_UINT4 val);
	/**	@brief	�l�b�g���[�N�L�����N�^�ԃ^�C���A�E�g�ł���T8�^�C���A�E�g���w�肵�܂��B�P��HSMS���b�Z�[�W����M���I���܂ł̎��Ԃ�b�Ŏw�肵�܂��B�W���l��"5"�b�ł��B
	*	@return	T8�^�C���A�E�g
	*/
	XD_UINT4 getOpt_T8() const ;
	/**	@brief	�l�b�g���[�N�L�����N�^�ԃ^�C���A�E�g�ł���T8�^�C���A�E�g��ݒ肵�܂��B�P��HSMS���b�Z�[�W����M���I���܂ł̎��Ԃ�b�Ŏw�肵�܂��B�W���l��"5"�b�ł��B
	 *	@param	val	T8�^�C���A�E�g
	 */
	void     setOpt_T8(XD_UINT4 val);
	/**	@brief	�����N�e�X�g�̑��M�Ԋu���擾���܂��B�i�b�j
	 *	@return	�����N�e�X�g�̑��M�Ԋu�i�b�j
	 */
	XD_UINT4 getOpt_LinkTest() const ;
	/**	@brief	�����N�e�X�g�̑��M�Ԋu��ݒ肵�܂��B�i�b�j
	 *	@param	val	�����N�e�X�g�̑��M�Ԋu�i�b�j
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
//		XTime					m_time;				// �o�^���ꂽ����
//		XTime					m_T3TimeOutTime;	// T3�^�C���A�E�g���������鎞��
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
//// T3�^�C�}�[�X���b�h
//	static void* _T3Timer(void *pMe);
//	void* T3Timer();
//// Active�ڑ��X���b�h
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
//	bool			m_bEnable;	//	�L��/����
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
