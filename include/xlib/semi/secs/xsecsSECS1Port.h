/**
 * @file xsecsSECS1Port.h
 * @brief �����̐���SEMI�K�i �V���A���|�[�g�ʐM�C���^�[�t�F�[�X
 *
 * @author T.Fujiha
 * @date 2007/0710
 * @version 1.0
 *
 * Copyright (C) 2007 T.Fujiha,
 */
#ifndef __X_SECS_SECS1_PORT_HEASER_FILE__
#define __X_SECS_SECS1_PORT_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"
#include "xlib/xsocket.h"
#include "xlib/xlock.h"
#include "xlib/xarray.h"
#include "xlib/xtime.h"
#include "xlib/xserialport.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/semi/secs/xsecs_comm.h"
#include <deque>
#include <vector>



DLL_EXTERN template class XLIB_API std::vector<char*>;

/**	@brief	SEMI-HSMS�ʐM���̎���	*/
class XLIB_API XSEMI_SECS1 : public XSeceCommItf
{
public:
	/**	@brief	�R���X�g���N�^	*/
	XSEMI_SECS1();
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSEMI_SECS1();

	XSEMI_SECS1& setOpt_T1(XD_UINT4 val);
	XSEMI_SECS1& setOpt_T2(XD_UINT4 val);
	XSEMI_SECS1& setOpt_T4(XD_UINT4 val);
	XSEMI_SECS1& setOpt_RetryCount(XD_UINT4 val);
	XSEMI_SECS1& setOpt_PortName(XStrBuff& val);

	XSerialPort& getRawDev(){ return m_dev; }

	virtual XStrBuff getPeerName();
	virtual bool openDevice();
	virtual bool closeDevice();
	virtual bool isOk();
protected:
	virtual bool _readRaw(void *pData, size_t size);
	virtual bool _writeRaw(const void *pData, size_t size);
	virtual bool _writeMessage(const void *pData, size_t size);
	virtual bool innserInitilize();
	virtual void innerStateChange(XSECS_CONN_STS sts);
	virtual bool innerStartPassive();
	virtual bool innerStartActive();
	virtual void* mainProc();
private:
	static void* serverProc(void *pUserData);
	static XD_UINT2 getCheckSum(unsigned char* pVal, XD_UINT1 ucSize);
	/*	@brief	���M�L���[���`�F�b�N���āA�f�[�^������ꍇ�́A���M����B
	 *	@retval	0 ���M�f�[�^����������
	 *	@retval	1 ���M����
	 *	@retval	2 �������������B
	 *	@retval	-1 �G���[
	 */
	int checkSendQ();
	/*	@brief	���b�Z�[�W���M����
	 *
	 *	@param	pData	���M�f�[�^[ length(4)+Header(10)+Data(N)]
	 *	@param	size	���M�f�[�^�̃T�C�Y
	 *
	 *	@retval	1 ���M����
	 *	@retval	2 �������������B
	 *	@retval	-1 �G���[
	 */
	int innerwriteMessage(const void *pData, size_t size);
	bool genericReceive(XD_UINT2 &usCurrentBlockNo, 
						XD_UINT2 &usCurrentReadSeq,
						XInflateStreamBuff& buff,
						std::ostream &stream);
	void sendNak();
	bool sendHandShakeCode(XD_UINT1 val);
protected:
	class _tagSendDataQ{
	public:
		_tagSendDataQ(const void *pData, size_t size, XSemaphore& lock)
			:	m_lockNotifyChange(lock)
		{
			m_pData = new char[size];
			memcpy(m_pData, pData, size);
			m_size = size;
		}
		~_tagSendDataQ() {
			delete []m_pData;
		}
		size_t				m_size;
		char*				m_pData;
		XSemaphore&			m_lockNotifyChange;
	};
	XSerialPort		m_dev;
	XD_UINT4		m_ulT1;
	XD_UINT4		m_ulT2;
	XD_UINT4		m_ulT4;
	XD_UINT4		m_ulRetryCount;
	XStrBuff		m_strPortName;

	XSemaphore			m_lockNotifyChange;
	XMutex				m_send_data_q_lock;
	std::vector<char*>	m_send_data_q;
};




#include "xlib/xlib_tail.h"
#endif /* __X_SECS_HSMS_PORT_HEASER_FILE__ */
