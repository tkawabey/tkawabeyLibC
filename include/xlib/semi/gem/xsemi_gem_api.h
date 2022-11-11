/**
 * @file xsemi_gem_api.h
 * @brief �N���ς݂́AXSEMI_Gem�̋@�\���O���v���Z�X����R�[�����邽�߂�API
 *
 * @author T.Fujiha
 * @date 2007/07/15
 * @version 1.0
 *
 * Copyright (C) 2007 T.Fujiha,
 */
#ifndef __XSECS_GEM_API_H__
#define __XSECS_GEM_API_H__

#include "xlib/xlib_head.h"
#include "xlib/xeception.h"
#include "xlib/xmsgqueue.h"
#include "xlib/semi/secs/xsecs.h"

// cmd:2
// msg_size:4
// msg_data:n (SECSHeader+Data)
#define	XSEMI_GEMAPI_CMD_SEND_PRIMARY			1
// cmd:2
// Status:4
// msg_size:4
// msg_data:n (SECSHeader+Data)
#define	XSEMI_GEMAPI_CMD_RECEIVE_SECONDRY		2
// cmd:2
// msg_size:4
// msg_data:n (SECSHeader+Data)
#define	XSEMI_GEMAPI_CMD_RECEIVE_MSG			3




/**	@brief	The XLIB_API class*/
class XLIB_API XSEMI_GemAPI
{
public:
	XSEMI_GemAPI();
	virtual ~XSEMI_GemAPI();

	bool open(const char* pName);
	void close();

	/**	@brief	The sendRequest function
	 *
	 *	@param	pHeader	a parameter of type XSECSHeader*
	 *	@param	pData	a parameter of type unsigned char *
	 *	@param	ulDataSize	a parameter of type unsigned long
	 *	@param	buffResp	a parameter of type XBuff&
	 *	@param	pulRspDataSize	a parameter of type XD_UINT4 *
	 *	@param	ulTimeOut	a parameter of type XD_UINT4
	 *
	 *	@return	���M�X�e�[�^�X
	 */
	XSECS_SEND_STS sendRequest(XSECSHeader* pHeader, 
								unsigned char *pData, unsigned long ulDataSize,
								XBuff& buffResp, XD_UINT4 *pulRspDataSize, 
								XD_UINT4 ulTimeOut = 60*1000);


	
	/**	@brief	�L���[���烁�b�Z�[�W�����o�����܂��B
	 *
	 *	@param	dataBuff	���b�Z�[�W���󂯎��o�b�t�@
	 *	@param	pDataSize	�󂯎�������b�Z�[�W�T�C�Y
	 *	@param	ulMsgID	���o�����b�Z�[�WID�B
	 *				- XQueue_MsgID_ANY���w�肷��ƁA�L���[�̐擪�ɂ��郁�b�Z�[�W�����o���B
	 *	@param	ulTimeout	���o���܂ł̃^�C���A�E�g�l���w��B
	 *				- XQueue_TIMEOUT_NOWAIT�́A�L���[�Ƀf�[�^���Ȃ��ꍇ�A�҂����ɒ����Ƀ��^�[������B
	 *				- XQueue_TIMEOUT_FOREVER�́A�����ɑ҂B
	 *
	 *	@return	���s�X�e�[�^�X
	 */
	XMsgQueueERR pop(XBuff& dataBuff, size_t *pDataSize, XD_UINT4 ulMsgID = XQueue_MsgID_ANY, XD_UINT4 ulTimeout = XQueue_TIMEOUT_FOREVER);
	/*{
		return m_dev.pop(dataBuff, pDataSize, ulMsgID, ulTimeout);
	}
	*/

	/**	@brief	The receiveMessage function
	 *
	 *	@param	pHeader	a parameter of type XSECSHeader*
	 *	@param	pData	a parameter of type unsigned char *
	 *	@param	ulDataSize	a parameter of type unsigned long
	 *
	 *	@retval	XQueue_POP_CALLBCK_IGNORE	�����𖳎������B�Ăяo�����́Apop�֐����R�[�����āA�f�[�^�̏������s��Ȃ���΍s���Ȃ�
	 *	@retval	XQueue_POP_CALLBCK_PROCESS	�f�[�^�����������B�B
	 */
	virtual int receiveMessage(XSECSHeader* pHeader, unsigned char *pData, unsigned long ulDataSize);

private:
	int popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);
protected:
	class Q : public XMsgQueueClient
	{
	public:
		Q(){}
		virtual ~Q(){}
		virtual int popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);
		XSEMI_GemAPI*	m_pGemAPI;
	friend class XSEMI_GemAPI;
	};
	Q	m_dev;
friend class Q;
};

#include "xlib/xlib_tail.h"
#endif /*__XSECS_GEM_API_H__*/


