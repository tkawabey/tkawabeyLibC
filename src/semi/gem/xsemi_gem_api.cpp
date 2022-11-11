#include "xlib/semi/gem/xsemi_gem_api.h"

//
//	XSEMI_GemAPI::Q
//
int XSEMI_GemAPI::Q::popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize)
{
XEXCEPT_ENTRY
	return m_pGemAPI->popCallBack(ulMsgID, pData, ulDataSize);
XEXCEPT_EXIT
}

//
//	XSEMI_GemAPI
//
XSEMI_GemAPI::XSEMI_GemAPI()
{
XEXCEPT_ENTRY
	m_dev.m_pGemAPI = this;
XEXCEPT_EXIT
}
XSEMI_GemAPI::~XSEMI_GemAPI()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

bool XSEMI_GemAPI::open(const char* pName)
{
XEXCEPT_ENTRY
	return m_dev.open(pName) == XMsgQueueERR_SUCCESS;
XEXCEPT_EXIT
}
void XSEMI_GemAPI::close()
{
XEXCEPT_ENTRY
	m_dev.close();
XEXCEPT_EXIT
}
XSECS_SEND_STS XSEMI_GemAPI::sendRequest(XSECSHeader* pHeader, 
										 unsigned char *pData,	unsigned long ulDataSize, 
										 XBuff &buffResp,		XD_UINT4 *pulRspDataSize, 
										 XD_UINT4 ulTimeOut /*= 60*1000*/)
{
XEXCEPT_ENTRY
	//
	//	1次メッセージの送信
	//
	XInflateStreamBuff	buff(256);
	std::iostream		stream(&buff);
	XD_UINT2			cmd = 1;
	XD_UINT4			ulSECSSize = 0;
	XBuff				rcvData;
	size_t				rcvDataSize = 0;
	unsigned char*		pdata = NULL;
	XD_UINT4			ulRcvDataSize = 0;
	XD_UINT4			status;


	*pulRspDataSize = 0;
	// 送信メッセージのSECSとしてのサイズ
	ulSECSSize = sizeof(XSECSHeader) + ulDataSize;
	ulSECSSize = ::htonl(ulSECSSize);

	stream.write((char*)&cmd,        sizeof(XD_UINT2));		// コマンド
	stream.write((char*)&ulSECSSize, sizeof(XD_UINT4));		// 送信メッセージのSECSとしてのサイズ
	stream.write((char*)pHeader,     sizeof(XSECSHeader));	// メッセージヘッダー
	stream.write((char*)pData,       ulDataSize);			// データ

	// 1次メッセージを送信
	if( m_dev.sendRequset(buff.getBuff(), buff.size(), rcvData, &rcvDataSize, ulTimeOut) != XMsgQueueERR_SUCCESS ) {
		return XSECS_SEND_STS_F_TIMEOUT;
	}
	pdata = rcvData;

	// コマンド
	::memcpy(&cmd, pdata, sizeof(XD_UINT2));
	pdata       += sizeof(XD_UINT2);
	rcvDataSize -= sizeof(XD_UINT2);
	if( cmd != XSEMI_GEMAPI_CMD_RECEIVE_SECONDRY ) {
		return XSECS_SEND_STS_F_DATA;
	}
	// ステータス
	::memcpy(&status, pdata, sizeof(XD_UINT4));
	pdata       += sizeof(XD_UINT4);
	rcvDataSize -= sizeof(XD_UINT4);
	if( rcvDataSize < sizeof(XD_UINT4) ) {
		return XSECS_SEND_STS_F_DATA;
	}
	// 受信メッセージの、SECSとしてのサイズ
	::memcpy(&ulRcvDataSize, pdata, sizeof(XD_UINT4));
	ulRcvDataSize = ntohl(ulRcvDataSize);
	pdata       += sizeof(XD_UINT4);
	rcvDataSize -= sizeof(XD_UINT4);

	if( ulRcvDataSize != rcvDataSize ) {
		return XSECS_SEND_STS_F_DATA;
	}
	if( status == XSECS_SEND_STS_S_AND_HAS_REP ) {
		// 2次メッセージのデータがある
		*pulRspDataSize = ulRcvDataSize;
		buffResp.Alloc(ulRcvDataSize);
		memcpy(buffResp, pdata, ulRcvDataSize);
	}
	return (XSECS_SEND_STS)status;
XEXCEPT_EXIT
}

int XSEMI_GemAPI::receiveMessage(XSECSHeader* pHeader, unsigned char *pData, unsigned long ulDataSize)
{
XEXCEPT_ENTRY
	return XQueue_POP_CALLBCK_PROCESS;	
XEXCEPT_EXIT
}

int XSEMI_GemAPI::popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize)
{
XEXCEPT_ENTRY
	// cmd:2
	// msg_size:4
	// msg_data:n (SECSHeader+Data)
	//#define	XSEMI_GEMAPI_CMD_RECEIVE_MSG			3

	XD_UINT2	ulCommand;
	XD_UINT4	ulSECSRspDataSize;

	if( ulDataSize < 10 ) {
		XTRACE(_T("     Data Size Error.\n"));	
		return XQueue_POP_CALLBCK_PROCESS;
	}
	// Command
	::memcpy(&ulCommand, pData, sizeof(XD_UINT2));
	pData       += sizeof(XD_UINT2);
	ulDataSize -= sizeof(XD_UINT2);
	// SecsSize 
	::memcpy(&ulSECSRspDataSize, pData, sizeof(XD_UINT4));
	pData       += sizeof(XD_UINT4);
	ulDataSize -= sizeof(XD_UINT4);
	ulSECSRspDataSize = ntohl(ulSECSRspDataSize);

	return this->receiveMessage((XSECSHeader*)pData, (unsigned char*)&pData[10], ulDataSize-10);
XEXCEPT_EXIT
}

XMsgQueueERR XSEMI_GemAPI::pop(XBuff& dataBuff, size_t *pDataSize, XD_UINT4 ulMsgID /*= XQueue_MsgID_ANY*/, XD_UINT4 ulTimeout /*= XQueue_TIMEOUT_FOREVER*/)
{

XEXCEPT_ENTRY
	// cmd:2
	// msg_size:4
	// msg_data:n (SECSHeader+Data)
	//#define	XSEMI_GEMAPI_CMD_RECEIVE_MSG			3
	XD_UINT2		ulCommand;
	XD_UINT4		ulSECSRspDataSize;
	unsigned char*	pData;
	unsigned char*	pDst;


	XMsgQueueERR sts = m_dev.pop(dataBuff, pDataSize, ulMsgID, ulTimeout);
	if( sts != XMsgQueueERR_SUCCESS ) {
		return sts;
	}

	pData = dataBuff;
	// Command
	::memcpy(&ulCommand, pData, sizeof(XD_UINT2));
	pData       += sizeof(XD_UINT2);
	*pDataSize -= sizeof(XD_UINT2);
	// SecsSize 
	::memcpy(&ulSECSRspDataSize, pData, sizeof(XD_UINT4));
	pData       += sizeof(XD_UINT4);
	*pDataSize -= sizeof(XD_UINT4);
	ulSECSRspDataSize = ntohl(ulSECSRspDataSize);
	if( ulSECSRspDataSize != *pDataSize ) {
		return XMsgQueueERR_RCV_DATA_BROKEN;
	}
	pDst = dataBuff;

	::memmove(pDst ,pData, *pDataSize);
	return XMsgQueueERR_SUCCESS;
XEXCEPT_EXIT
}
