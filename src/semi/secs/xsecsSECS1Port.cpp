#include "xlib/xeception.h"
#include "xlib/semi/secs/xsecsSECS1Port.h"
#include <iostream>



XSEMI_SECS1::XSEMI_SECS1()
	:	m_ulT1( 5*100 )
	,	m_ulT2( 10*1000 )
	,	m_ulT4(45)
	,	m_ulRetryCount( 3 )
{
XEXCEPT_ENTRY
	m_ifType = XSECS_COMMIF_TYPE_SECS;
XEXCEPT_EXIT
}


XSEMI_SECS1::~XSEMI_SECS1()
{
XEXCEPT_ENTRY
	closeIf();
XEXCEPT_EXIT
}

XSEMI_SECS1& XSEMI_SECS1::setOpt_T1(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulT1 = val;
XEXCEPT_EXIT
	return *this;
}
XSEMI_SECS1& XSEMI_SECS1::setOpt_T2(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulT2 = val;
XEXCEPT_EXIT
	return *this;
}
XSEMI_SECS1& XSEMI_SECS1::setOpt_T4(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulT4 = val;
XEXCEPT_EXIT
	return *this;
}
XSEMI_SECS1& XSEMI_SECS1::setOpt_RetryCount(XD_UINT4 val)
{
XEXCEPT_ENTRY
	if( val == 0 ) { val = 1; }
	m_ulRetryCount = val;
XEXCEPT_EXIT
	return *this;
}
XSEMI_SECS1& XSEMI_SECS1::setOpt_PortName(XStrBuff& val)
{
XEXCEPT_ENTRY
	m_strPortName = val;
XEXCEPT_EXIT
	return *this;
}
XStrBuff XSEMI_SECS1::getPeerName()
{
XEXCEPT_ENTRY
	XStrBuff str = m_strPortName;
	return str;
XEXCEPT_EXIT
}
bool XSEMI_SECS1::openDevice()
{
XEXCEPT_ENTRY
	if( isOk() == true ) {
		return true;
	}
	m_dev.Close();
	if( m_dev.open(m_strPortName, XSerialPort::Opt().BaudRate(9600)) == false ) {
		return false;
	}
XEXCEPT_EXIT
	return true;
}
bool XSEMI_SECS1::closeDevice()
{
XEXCEPT_ENTRY
	m_dev.Close();

	//送信データを削除
	XMutexer	locker(m_send_data_q_lock);
	std::vector<char*>::iterator ite = m_send_data_q.begin();
	for(; ite != m_send_data_q.end(); ite++) {
		char* p = *ite;
		_tagSendDataQ* psendData = (_tagSendDataQ*)p;
		delete psendData;
	}
	m_send_data_q.clear();	
XEXCEPT_EXIT
	return true;
}
bool XSEMI_SECS1::isOk()
{
XEXCEPT_ENTRY
	return m_dev.isValid();
XEXCEPT_EXIT
	return true;
}
bool XSEMI_SECS1::_readRaw(void *pData, size_t size)
{
XEXCEPT_ENTRY
	if( m_dev.isValid() == false ) {
		return false;
	}
	return m_dev.Read(pData, size);
XEXCEPT_EXIT
	return true;
}
bool XSEMI_SECS1::_writeRaw(const void *pData, size_t size)
{
XEXCEPT_ENTRY
	if( m_dev.isValid() == false ) {
		return false;
	}
	return m_dev.Write(pData, size);
XEXCEPT_EXIT
	return true;
}
bool XSEMI_SECS1::_writeMessage(const void *pData, size_t size)
{
XEXCEPT_ENTRY
	// 送信キューにセットするのみ
	XSemaphore		lock;
	_tagSendDataQ *pNewSendData = NULL;
	{
		XMutexer	locker(m_send_data_q_lock);
		pNewSendData = new _tagSendDataQ(pData, size, lock);
		if( pNewSendData == NULL ) {
			return false;
		}
		m_send_data_q.push_back((char*)pNewSendData);
	}
XEXCEPT_EXIT
	return true;
}
void XSEMI_SECS1::innerStateChange(XSECS_CONN_STS sts)
{
XEXCEPT_ENTRY
	if( m_ststus != sts ) {
		m_ststus = sts;
		if( m_pHandler != NULL ) {
			m_pHandler->connectEventCallBack(this, m_ststus);
		}
	}
XEXCEPT_EXIT
}
bool XSEMI_SECS1::innserInitilize()
{
XEXCEPT_ENTRY
	return true;
XEXCEPT_EXIT
	return false;
}
bool XSEMI_SECS1::innerStartPassive()
{
XEXCEPT_ENTRY
	openDevice();
	XThread::Create(serverProc, this);
XEXCEPT_EXIT
	return true;
}

bool XSEMI_SECS1::innerStartActive()
{
XEXCEPT_ENTRY
	openDevice();
XEXCEPT_EXIT
	return true;
}


void* XSEMI_SECS1::mainProc()
{
XEXCEPT_ENTRY

	XD_UINT2			usCurrentBlockNo = 0;
	XD_UINT2			usCurrentReadSeq = 0;
	XInflateStreamBuff	buff;
	std::ostream		stream(&buff);

	while( 1 ) {
		XD_UINT1 ucBlock;

		if( isOk() == false || m_bEnable == false ) {
			return 0;
		}
		{
			XSerialPort::Opt opt = m_dev.getOpt();
			opt.ReadIntervalTimeout( 0 );		// 受信キャラクタ間のタイムアウト
			opt.ReadTotalTimeoutConstant( 100 );	// プロトコルタイムアウト
			opt.ReadTotalTimeoutMultiplier( 0 );
			opt.WriteTotalTimeoutMultiplier( 0 );
			opt.WriteTotalTimeoutConstant( 0 );	// プロトコルタイムアウト
			m_dev.setOpt(opt);
		}
		// 送信キューのチェック
		int ret = checkSendQ();	// ret = 2の場合は、競合が発生して、ENQを受信済み
		if( ret != 2 ) {
			// ENQ を受信する。
			if( _readRaw(&ucBlock, 1) == false ) {
				if( isOk() == false ) {
					break;
				}
				continue;
			}
			if( m_pHandler != NULL ) {
				m_pHandler->SECS1_occurreHandShakeCode(false, ucBlock);
			}

			if( ucBlock != XSECS_SECS1_HANDSHAKE_ENQ ) {
				if( m_pHandler != NULL ) {
					m_pHandler->SECS1_unexpectHandShakeCode(ucBlock);
				}
				break;
			}
		}
		if( genericReceive( usCurrentBlockNo, usCurrentReadSeq, buff, stream) == false ) {
			break;
		}		
	}
	innerStateChange(XSECS_CONN_STS_NOT_CONNECTED);
	return 0;
XEXCEPT_EXIT
}

XD_UINT2 XSEMI_SECS1::getCheckSum(unsigned char* pVal, XD_UINT1 ucSize)
{
XEXCEPT_ENTRY
	XD_UINT2 val = 0;
	for(XD_UINT1 i = 0; i < ucSize; i++) {
		val += *pVal;
		pVal++;
	}
	return val;
XEXCEPT_EXIT
}


int XSEMI_SECS1::checkSendQ()
{
XEXCEPT_ENTRY
	XMutexer	locker(m_send_data_q_lock);
	std::vector<char*>::iterator ite = m_send_data_q.begin();
	
	for(; ite != m_send_data_q.end(); ite++) {
		char* p = *ite;
		_tagSendDataQ* psendData = (_tagSendDataQ*)p;
		XSECSHeader* pHeader = (XSECSHeader*)&(psendData->m_pData[4]);
		
		int ret = -1;
		for( XD_UINT4 i = 0; i < m_ulRetryCount; i++ ) {
			ret = innerwriteMessage(psendData->m_pData, psendData->m_size);
			if( ret == 2 ) {	// ret = 2の場合は、競合が発生して、ENQを受信
				return 2;
			}
			if( ret == 1 ) {
				break;
			}
		}
		if( ret == -1 ) {
			if( m_pHandler != NULL ) {
				XStrBuff strErr;
				
				strErr.Format(_T("Real SendData Faild. S%dF%d "), XSecsData::get_stream(*pHeader)
					, XSecsData::get_function(*pHeader));
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, strErr);
			}
		}
//		psendData->m_lockNotifyChange.unlock();
		m_send_data_q.erase(ite);
		delete psendData;
		return ret;
	}		
XEXCEPT_EXIT
	return 0;
}



int XSEMI_SECS1::innerwriteMessage(const void *pData, size_t size)
{
XEXCEPT_ENTRY
	if( size < 14 ) {
		return -1;
	}


	{
		XSerialPort::Opt opt = m_dev.getOpt();
		opt.ReadIntervalTimeout( 10 );		// 受信キャラクタ間のタイムアウト
		opt.ReadTotalTimeoutConstant( m_ulT2 );	// プロトコルタイムアウト
		opt.ReadTotalTimeoutMultiplier( 1 );
		opt.WriteTotalTimeoutMultiplier( 10 );
		opt.WriteTotalTimeoutConstant( m_ulT2 );	// プロトコルタイムアウト
		m_dev.setOpt(opt);
	}

	XD_UINT1 ucBlock;
	XD_UINT1 ucBlockSize;
	XD_UINT2 usCheckSum = 0;

	XD_UINT4 ulSize = 0;
	char*	pWork = (char*)pData;
	char	block[XSECS_SECS1_MAX_BLOCK_SIZE+XSECS_HEADER_SIZE ];

	::memcpy(&ulSize, pWork, 4);
	ulSize = ntohl(ulSize);

	pWork += 4;
	memcpy(block, pWork, XSECS_HEADER_SIZE );
	pWork += XSECS_HEADER_SIZE;


	size_t sizeTmp = size - 14;
	XD_UINT2 usNumOfBlock = (sizeTmp / XSECS_SECS1_MAX_BLOCK_SIZE)+1;
	XD_UINT1 ucEndBlockSize = sizeTmp - ( (usNumOfBlock-1)*XSECS_SECS1_MAX_BLOCK_SIZE);


	for(XD_UINT2 i = 0; i < usNumOfBlock; i++) {

		XSECSHeader* pHeader = (XSECSHeader*)block;
		XSecsData::generateHostCode(*pHeader);

		XSecsData::set_ebit(   *pHeader, i+1 >= usNumOfBlock ? true : false);
		XSecsData::set_blockNo(*pHeader, i+1);

		XSecsData::generateNetWorkCode(*pHeader);

		if( i+1 >= usNumOfBlock ) {
			ucBlockSize = ucEndBlockSize+XSECS_HEADER_SIZE;
		} else {
			ucBlockSize = XSECS_SECS1_MAX_BLOCK_SIZE/*+XSECS_HEADER_SIZE*/;
		}
		memcpy(&block[XSECS_HEADER_SIZE], pWork, ucBlockSize );
		usCheckSum = htons( getCheckSum((unsigned char*)block, ucBlockSize) );


#ifdef _DEBUG
XEXCEPT_ENTRY
		XTRACE(_T("Num Of Block:(%d)%d  EndSize:%d\n"), i+1, usNumOfBlock, ucEndBlockSize);
		XSECSHeader &header = *pHeader;
		XD_UINT4 ulBlockNo = (header.ptype & ~0x80);
		ulBlockNo <<= 4;
		ulBlockNo |= header.stype;

		unsigned char *pC = (unsigned char*)&header;
		XTRACE(_T("SEND:"));
		for(int ii = 0; ii < 10; ii++) {
			XTRACE(_T("%02X "), *pC);
			pC++;
		}
		XTRACE(_T("\n"));
		XTRACE(_T("  Length: %d sessionID:%04X\n"), ucBlockSize, XSecsData::get_deviceID(header));
		XTRACE(_T("  S%dF%d %s\n")
					, 		(int)(header.byte2 & ~0x80)
					, 		header.status 
					,		header.byte2 & 0x80 ? "W" : "");
		XTRACE(_T("  block:No(%d)   %s\n")
					, 		XSecsData::get_blockNo(header)
					,		XSecsData::get_ebit(header) ? "E" : "" );
		XTRACE(_T("  systembyte:%08X(%d)\n"), XSecsData::get_ticket(header), XSecsData::get_ticket(header));
		XTRACE(_T("  ChkSum:%04X\n"), usCheckSum);
XEXCEPT_EXIT
#endif /*_DEBUG*/

		// ENQ 
		if( sendHandShakeCode(XSECS_SECS1_HANDSHAKE_ENQ)  == false ) {
			if( m_pHandler != NULL ) {
				XStrBuff strErr;
				strErr.Format(_T("write ENQ Error."));
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr);
			}
			return -1;
		}

		// wait for EOT
		if( this->_readRaw(&ucBlock, 1) == false ) {
			if( m_pHandler != NULL ) {
				XStrBuff strErr;
				strErr.Format(_T("read EOT Error."));
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr);
			}
			return -1;
		}
		if( m_pHandler != NULL ) {
			m_pHandler->SECS1_occurreHandShakeCode(false, ucBlock);
		}

		if( ucBlock != XSECS_SECS1_HANDSHAKE_EOT ) {
			if( ucBlock == XSECS_SECS1_HANDSHAKE_ENQ && m_bIsActive == true) {
				// 競合が発生し
				// スレーブの場合は、受信に変わる
				return 2;
			}
			if( m_pHandler != NULL ) {
				m_pHandler->SECS1_unexpectHandShakeCode(ucBlock);
			}
			if( m_pHandler != NULL ) {
				XStrBuff strErr;
				strErr.Format(_T("invalid block transfer Code(%02X)."), ucBlock);
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr);
			}
			return -1;
		}

		bool bWriteError = false;
		// send block size
		if( this->_writeRaw(&ucBlockSize, 1) == false ) {
			bWriteError = true;
		}
		// send data
		if( m_pHandler != NULL ) {
			XSECSHeader hederTmp;
			::memcpy(&hederTmp, pHeader, XSECS_HEADER_SIZE);
			XSecsData::generateHostCode(hederTmp);
			m_pHandler->SECS1_occurreBlock(true, *pHeader, (unsigned char*)block, ucBlockSize);
		}
		if( this->_writeRaw(&block, ucBlockSize) == false ) {
			bWriteError = true;
		}
		// send cheksum
		if( this->_writeRaw(&usCheckSum, sizeof(usCheckSum)) == false ) {
			bWriteError = true;
		}

		// wait for ack
		if( this->_readRaw(&ucBlock, 1) == false ) {

			if( m_pHandler != NULL ) {
				XStrBuff strErr;
				strErr.Format(_T("read ACK Error."));
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr);
			}

			return -1;
		}

		if( m_pHandler != NULL ) {
			m_pHandler->SECS1_occurreHandShakeCode(false, ucBlock);
		}
		if( ucBlock != XSECS_SECS1_HANDSHAKE_ACK ) {
			if( m_pHandler != NULL ) {
				m_pHandler->SECS1_unexpectHandShakeCode(ucBlock);
			}
			return -1;
		}
		if( bWriteError == true ) {
			return -1;
		}
		
		pWork += ucBlockSize;
	}
XEXCEPT_EXIT
	return 1;
}

bool XSEMI_SECS1::genericReceive(XD_UINT2 &usCurrentBlockNo, 
						XD_UINT2 &usCurrentReadSeq,
						XInflateStreamBuff& buff,
						std::ostream &stream)
{
XEXCEPT_ENTRY
	XD_UINT2 usData2;
	XD_UINT1 ucBlockSize;
	XSECSHeader header;
	unsigned char  block[XSECS_SECS1_MAX_BLOCK_SIZE+XSECS_HEADER_SIZE];
	bool bHasSecondaryWithWait = false;


	
	XSerialPort::Opt opt = m_dev.getOpt();
	opt.ReadIntervalTimeout( 10 );
	opt.ReadTotalTimeoutConstant( m_ulT2 );
	opt.ReadTotalTimeoutMultiplier( 10 );
	opt.WriteTotalTimeoutMultiplier( 10 );
	opt.WriteTotalTimeoutConstant( m_ulT2 );
	m_dev.setOpt(opt);


	// EOT を送信する。
	if( sendHandShakeCode(XSECS_SECS1_HANDSHAKE_EOT) == false ) {
		return false;
	}
	// ブロックサイズの受信
	if( _readRaw(&ucBlockSize, 1) == false ) {
		// NAK送信
		sendHandShakeCode(XSECS_SECS1_HANDSHAKE_NAK);
		return isOk();	// デバイスが閉じたことぐらいしか考えられないので、ブレークする。
	}
	if( !( 10 <= ucBlockSize && ucBlockSize <= 254 ) ) {
		// T1タイムアウトが発生するまで読み捨て、NAK送信
		sendNak();
		return true;
	}

	


	opt.ReadTotalTimeoutConstant( m_ulT1 );
	opt.WriteTotalTimeoutConstant( m_ulT1 );
	m_dev.setOpt(opt);

	unsigned short checkSum;
	// ブロックデータの読み込み
	if( _readRaw(block, ucBlockSize) == false ||
		_readRaw(&checkSum, 2) == false ) {
		// NAK送信
		sendHandShakeCode(XSECS_SECS1_HANDSHAKE_NAK);
		return isOk();	// デバイスが閉じたことぐらいしか考えられないので、ブレークする。
	}
	::memcpy(&header, block, XSECS_HEADER_SIZE);
	if( m_pHandler != NULL ) {
		XSECSHeader hederTmp;
		::memcpy(&hederTmp, &header, XSECS_HEADER_SIZE);
		XSecsData::generateHostCode(hederTmp);
		m_pHandler->SECS1_occurreBlock(false, hederTmp, (unsigned char*)block, ucBlockSize);
	}
	checkSum = ::htons(checkSum);
	if( getCheckSum(block, ucBlockSize) != checkSum ) {
		// チェックサムエラー
		// Nackを送信
		sendNak();
		return isOk();
	}		


	// ヘッダーのデータをホストオーダーに変換
	::memcpy(&header, block, 10);
	header.sessionID = ::ntohs(header.sessionID);
	::memcpy(&usData2, header.tr_id, sizeof(usData2));
	usData2 = ::ntohs(usData2);
	::memcpy(header.tr_id, &usData2, sizeof(usData2));

	// カレントシーケンスのチェック
	if( usCurrentReadSeq == 0 ) {
		usCurrentReadSeq = XSecsData::get_ticket(header);
	}

	if( usCurrentBlockNo != 0 ) {
		// マルチブロックの終了だった場合
		if( ( usCurrentBlockNo+1) != XSecsData::get_blockNo(header) ) {	// 連番でない
			// 期待するブロックNoでなかった
			if( m_pHandler != NULL ) {
				XStrBuff strErr;
				strErr.Format(_T("SECS1 Communication Error. invalid BlockNo Received. Pre (%d) Cur(%d)"),
					usCurrentBlockNo, XSecsData::get_blockNo(header) );
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr);
			}
			return true;
		}
	}
	if( XSecsData::get_ebit(header) == true ) {
		usCurrentBlockNo = 0;
	} else {

	}
	// Ackを送信
	if( sendHandShakeCode(XSECS_SECS1_HANDSHAKE_ACK) == false ) {
		return false;
	}
#ifdef _DEBUG
	XD_UINT4 ulBlockNo = (header.ptype & ~0x80);
	ulBlockNo <<= 4;
	ulBlockNo |= header.stype;

	unsigned char *pC = (unsigned char*)&header;
	XTRACE(_T("RECV:"));
	for(int ii = 0; ii < 10; ii++) {
		XTRACE(_T("%02X "), *pC);
		pC++;
	}
	XTRACE(_T("\n"));
	XTRACE(_T("  Length: %d sessionID:%04X\n"), ucBlockSize, XSecsData::get_deviceID(header));
	XTRACE(_T("  S%dF%d %s\n")
				, 		(int)(header.byte2 & ~0x80)
				, 		header.status 
				,		header.byte2 & 0x80 ? "W" : "");
	XTRACE(_T("  block:No(%d)   %s\n")
				, 		XSecsData::get_blockNo(header)
				,		XSecsData::get_ebit(header) ? "E" : "" );
	XTRACE(_T("  systembyte:%08X(%d)\n"), XSecsData::get_ticket(header), XSecsData::get_ticket(header));
	XTRACE(_T("  ChkSum:%04X\n"), checkSum);
#endif /*_DEBUG*/

	// データ部の書き込み
	stream.write((const char *)&block[XSECS_HEADER_SIZE], ucBlockSize-XSECS_HEADER_SIZE);

	if( XSecsData::get_ebit(header) == false ) {
		return true;
	}

	long nSize = buff.size() + 10 ;
	unsigned char *pData = new unsigned char[ nSize ];
	::memcpy(pData, &header, XSECS_HEADER_SIZE);
	::memcpy(&pData[XSECS_HEADER_SIZE], buff.getBuff(), buff.size());
	buff.resetp();



	{
		XMutexer			locker(m_lockTicket);
		// チケットを探す
		TicketData* pTicket = NULL;
		if( header.sessionID != 0xFFFF ) {
			pTicket = findTicket(usData2);
		}
		if( pTicket != NULL ) {
			pTicket->setReceived(pData, nSize);

			if( m_pHandler != NULL ) {
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_DEBUG, _T("unlock Ticket"));
			}
		}
		// S9F*のメッセージを受信した場合は、同期をブレークさせる
		if( (int)(header.byte2 & ~0x80) == 9 ) {
			XSECSHeader header2;
			XDataReader reader(pData+10, nSize-10);
			XData       data;
			
			reader.getValue( &data );
			data.getValAsBinary((unsigned char *)&header2, sizeof(header2));


			header2.sessionID = ::ntohs(header2.sessionID);
			::memcpy(&usData2, header2.tr_id, sizeof(usData2));
			usData2 = ::ntohs(usData2);
			::memcpy(header2.tr_id, &usData2, sizeof(usData2));

			if( header2.sessionID != 0xFFFF ) {
				pTicket = findTicket(usData2);
			}
			if( pTicket != NULL ) {
				pTicket->setReceived(pData, nSize);
			}
		}

	}

	if( m_pHandler != NULL ) {
		XSECSHeader reportHeader;
		::memcpy(&reportHeader, pData, sizeof(XSECSHeader));
		m_pHandler->receiveDataMessageCallBack(this, header, pData, nSize);

		// 2次メッセージを受信した？
		if( bHasSecondaryWithWait == false ) {
			m_pHandler->receiveResponseByNoWaitCallBack(this, header, pData, nSize);
		}
	}
XEXCEPT_EXIT
	return true;
}

void XSEMI_SECS1::sendNak()
{
XEXCEPT_ENTRY
	XD_UINT1 ucBlock;
	XSerialPort::Opt opt = m_dev.getOpt();
	opt.ReadIntervalTimeout( 10 );
	opt.ReadTotalTimeoutConstant( m_ulT1 );
	opt.ReadTotalTimeoutMultiplier( 10 );
	opt.WriteTotalTimeoutMultiplier( 10 );
	opt.WriteTotalTimeoutConstant( m_ulT1 );
	m_dev.setOpt(opt);

	while( _readRaw(&ucBlock, 1) == true ) {
		;
	}
	// NAK送信
	if( isOk() == true ) {
		sendHandShakeCode(XSECS_SECS1_HANDSHAKE_NAK);
	}
XEXCEPT_EXIT
}
void* XSEMI_SECS1::serverProc(void *pUserData)
{
XEXCEPT_ENTRY
	return ((XSEMI_SECS1*)pUserData)->mainProc();
XEXCEPT_EXIT
}

bool XSEMI_SECS1::sendHandShakeCode(XD_UINT1 val)
{
XEXCEPT_ENTRY
	if( m_pHandler != NULL ) {
		m_pHandler->SECS1_occurreHandShakeCode(true, val);
	}
	if( _writeRaw(&val, 1) == false ) {
		return false;
	}
XEXCEPT_EXIT
	return true;
}