#include "xlib/xeception.h"
#include "xlib/semi/secs/xsecsHSMSPort.h"
#include <iostream>




XSEMI_HSMS::XSEMI_HSMS()
	:	m_pSocket( NULL )
	,	m_ulT7(5)
	,	m_ulT8( 5 )
	,	m_ulLinkTest( 0 ) 
	,	m_dtLinkTest( 0.0 )
{
XEXCEPT_ENTRY
	m_ifType = XSECS_COMMIF_TYPE_HSMS;
XEXCEPT_EXIT
}


XSEMI_HSMS::~XSEMI_HSMS()
{
XEXCEPT_ENTRY
	closeIf();
XEXCEPT_EXIT
}


XSEMI_HSMS& XSEMI_HSMS::setAddress(XSocketAddress &address)
{
XEXCEPT_ENTRY
	m_activeAddress = address;
	return *this;
XEXCEPT_EXIT
}
XSEMI_HSMS& XSEMI_HSMS::setSockOptReuseAddr(bool val)
{
XEXCEPT_ENTRY
	m_bSockOpt_ReUseAddr = val;
	return *this;
XEXCEPT_EXIT
}

XSocketAddress& XSEMI_HSMS::getAddress()
{
XEXCEPT_ENTRY
	return m_activeAddress;
XEXCEPT_EXIT
}
XSocketAddress& XSEMI_HSMS::getSocketName()
{
XEXCEPT_ENTRY
	return m_sockName;
XEXCEPT_EXIT
}
XSocketAddress& XSEMI_HSMS::getPeerSocketName()
{
XEXCEPT_ENTRY
	return m_peerName;
XEXCEPT_EXIT
}
XStrBuff XSEMI_HSMS::getPeerName()
{
XEXCEPT_ENTRY
	XStrBuff str;
	str = m_peerName.GetIPAddress();
	return str;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_HSMS::getOpt_T5() const
{
XEXCEPT_ENTRY
	return m_ulActiveConnectTimeout;
XEXCEPT_EXIT
}
void     XSEMI_HSMS::setOpt_T5(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulActiveConnectTimeout = val;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_HSMS::getOpt_T7() const
{
XEXCEPT_ENTRY
	return m_ulT7;
XEXCEPT_EXIT
}
void     XSEMI_HSMS::setOpt_T7(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulT7 = val;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_HSMS::getOpt_T8() const
{
XEXCEPT_ENTRY
	return m_ulT8;
XEXCEPT_EXIT
}
void     XSEMI_HSMS::setOpt_T8(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulT8 = val;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_HSMS::getOpt_LinkTest() const
{
XEXCEPT_ENTRY
	return m_ulLinkTest;
XEXCEPT_EXIT
}
void     XSEMI_HSMS::setOpt_LinkTest(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulLinkTest = val;
XEXCEPT_EXIT
}

bool XSEMI_HSMS::openDevice()
{
XEXCEPT_ENTRY
	if( m_bIsActive == true ) {
		// �A�N�e�B�u�ڑ��^�C�}�[���N������B
	} else {
		// �A�N�e�B�u��TCP�I�[�v��
/*
		m_pSocket = new XSocketTCPServer();

		if( m_pSocket->Socket() == false ) {
			return false;
		}
		m_pSocket->put_ReuseAddr( m_bSockOpt_ReUseAddr );
		//m_pSocket->put_ExclusiveAddr( m_bSockOpt_ReUseAddr );

		((XSocketTCPServer*)m_pSocket)->Open(m_activeAddress, serverAcceptProc, this);

		m_pSocket->GetSockName(m_sockName);
		m_pSocket->GetPeerName(m_peerName);

		// �ʐM��Ԓʒm
		innerStateChange( XSECS_CONN_STS_ACCEPTING );
*/
	}
XEXCEPT_EXIT
	return true;
}
bool XSEMI_HSMS::closeDevice()
{
XEXCEPT_ENTRY

	if( m_bIsActive == true ) {
		// �p�b�V�u���[�h�ł��A�A�N�e�B�u���ڑ����Ă���ꍇ�́A�Z�p���[�g�𑗐M
		sendHSMSCtrl( XSECS_STYPE_SEPARATE_REQ, m_usCurTicket++);
		XThread::Sleep(0, 100);
	}
	m_curSock.Close();

	if( m_bIsActive == false && m_pSocket != NULL) {
		// �p�b�V�u���[�h�ł��A�A�N�e�B�u���ڑ����Ă���ꍇ�́A�Z�p���[�g�𑗐M
		sendHSMSCtrl( XSECS_STYPE_SEPARATE_REQ, m_usCurTicket++);
		XThread::Sleep(0, 100);
	}
	m_curSock.Close();
	if( m_pSocket != NULL ) {
		m_pSocket->Close();
		XThread::Sleep(0, 100);
		delete m_pSocket;
	}
	m_pSocket = NULL;
	XThread::Sleep(0, 100);
XEXCEPT_EXIT
	return true;
}
bool XSEMI_HSMS::isOk()
{
XEXCEPT_ENTRY
	return  m_curSock.IsValid();
XEXCEPT_EXIT
	return true;
}
bool XSEMI_HSMS::_readRaw(void *pData, size_t size)
{
XEXCEPT_ENTRY
	if( isOk() == false ) {
		return false;
	}
	return m_curSock.Read(pData, size);
XEXCEPT_EXIT
	return true;
}
bool XSEMI_HSMS::_writeRaw(const void *pData, size_t size)
{
XEXCEPT_ENTRY
	if( isOk() == false ) {
		return false;
	}
	return m_curSock.Write(pData, size);
XEXCEPT_EXIT
	return true;
}
bool XSEMI_HSMS::_writeMessage(const void *pData, size_t size)
{
XEXCEPT_ENTRY
	return _writeRaw(pData, size);
XEXCEPT_EXIT
	return true;
}
void XSEMI_HSMS::innerStateChange(XSECS_CONN_STS sts)
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
bool XSEMI_HSMS::innserInitilize()
{
XEXCEPT_ENTRY
	return true;
XEXCEPT_EXIT
	return false;
}
bool XSEMI_HSMS::innerStartPassive()
{
XEXCEPT_ENTRY
	try {
		// �A�N�e�B�u��TCP�I�[�v��
		m_pSocket = new XSocketTCPServer();

		if( m_pSocket->Socket() == false ) {
			return false;
		}
		m_pSocket->put_ReuseAddr( m_bSockOpt_ReUseAddr );

		((XSocketTCPServer*)m_pSocket)->Open(m_activeAddress, serverAcceptProc, this);

		m_pSocket->GetSockName(m_sockName);
		m_pSocket->GetPeerName(m_peerName);

		// �ʐM��Ԓʒm
		innerStateChange( XSECS_CONN_STS_ACCEPTING );
	} catch(XExcept &e) {
		if( m_pSocket != NULL ) {
			delete m_pSocket;
			m_pSocket = NULL;
		}
		if( m_pHandler != NULL ) {
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, e.whatT());
		}
//		m_ststus = XSecsHSMSPortHandler::NOT_CONNECTED;
		return false;
	}

XEXCEPT_EXIT
	return true;
}
bool XSEMI_HSMS::innerStartActive()
{
XEXCEPT_ENTRY
	// �p�b�V�u��TCP�I�[�v��
	XSocketTCPClinet* pTempSock = new XSocketTCPClinet();
	pTempSock->Socket();
	pTempSock->put_ReuseAddr( m_bSockOpt_ReUseAddr );
	if( ((XSocketTCPClinet*)pTempSock)->Open(m_activeAddress) == false ) {
		delete pTempSock;
		pTempSock = NULL;
		return false;
	}
	m_curSock.Attach( pTempSock->Detach() );
	delete pTempSock;

	// SELECT_REQ �𑗐M
	if( sendHSMSCtrl(XSECS_STYPE_SELECT_REQ, m_usCurTicket++) ==false ) {
		m_curSock.Close();
		return false;
	}


	// SELECT_RSP ����M
	m_curSock.put_RcvTimeO( m_ulT6 * 1000 );
	XSECSHeader header;
	long nSize = 0;
	if( m_curSock.Read(&nSize, 4) == false || 
		m_curSock.Read(&header, sizeof(header)) == false) {

		m_curSock.Close();
		
		return false;
	}
	m_curSock.put_RcvTimeO(0);



	if( m_pHandler != NULL ) {
		m_pHandler->receiveHSMSCtrlCallBack(header);
	}

	if( !( header.ptype == 0 && 
		  header.stype == XSECS_STYPE_SELECT_RSP ) ) {

		m_curSock.Close();
		return false;
	}


	// �A�h���X���擾
	m_curSock.GetSockName(m_sockName);
	m_curSock.GetPeerName(m_peerName);
XEXCEPT_EXIT
	return true;
}

void* XSEMI_HSMS::mainProc()
{
XEXCEPT_ENTRY

	XBuff	buff;
	buff.Alloc( 4029 ); 



	while( 1 ) {
		long nSize = 0;
		bool bHasSecondaryWithWait = false;



		if( isOk() == false || m_bEnable == false ) {
			return 0;
		}
		m_curSock.put_RcvTimeO( 0 );
		if( _readRaw(&nSize, 4) == false ) {
			break;
		}
		nSize = ::ntohl(nSize);
		if( !(nSize >= 10 ) ) {
			continue;
		}
		// �o�b�t�@�̏���
		unsigned char *pData = NULL;
		if( buff.GetBuffSize() < nSize ) {
			pData = (unsigned char*)buff.Alloc( nSize + 1024 ); 
		} else {
			pData = buff;
		}

		// T8�^�C���A�E�g��ݒ�
		//	�l�b�g���[�N�L�����N�^�ԃ^�C���A�E�g�ł���T8�^�C���A�E�g��ݒ肵�܂��B
		//	�P��HSMS���b�Z�[�W����M���I���܂ł̎��Ԃ�b�Ŏw�肵�܂��B�W���l��
		m_curSock.put_RcvTimeO( m_ulT8 * 1000 );

		if( _readRaw(pData, nSize) == false ) {
			break;
		}
		XSECSHeader header;
		::memcpy(&header, pData, sizeof(XSECSHeader));
		XSecsData::generateHostCode( header );

		{
			XMutexer			locker(m_lockTicket);
			// �`�P�b�g��T��
			TicketData* pTicket = NULL;
			if( header.sessionID != 0xFFFF ) {
				pTicket = findTicketNoLock( XSecsData::get_ticket(header) );
			}
			if( pTicket != NULL ) {
				pTicket->setReceived(pData, nSize);
			}

			// S9F*�̃��b�Z�[�W����M�����ꍇ�́A�������u���[�N������
			if( (int)(header.byte2 & ~0x80) == 9 ) {
				XSECSHeader header2;
				XDataReader reader(pData+10, nSize-10);
				XData       data;
				
				reader.getValue( &data );
				data.getValAsBinary((unsigned char *)&header2, sizeof(header2));
				XSecsData::generateHostCode( header2 );

				if( header2.sessionID != 0xFFFF ) {
					pTicket = findTicketNoLock( XSecsData::get_ticket(header2) );
				}
				if( pTicket != NULL ) {
					pTicket->setReceived(pData, nSize);
				}
			}


			// bHasSecondaryWithNoWait
			if( pTicket != NULL ) {
				bHasSecondaryWithWait = pTicket->getWaitMode();
			}
		}

		bool bRet = true;
		//
		// HSMS Ctrol
		//
		if( header.sessionID == 0xFFFF && 
			header.ptype == 0 && 
			header.stype == XSECS_STYPE_SELECT_REQ ) {

			if( m_pHandler != NULL ) {
				bRet = m_pHandler->receiveHSMSCtrlCallBack(header);
			}
			// Selcte.req
			if( bRet == true ) {
				sendHSMSCtrl(XSECS_STYPE_SELECT_RSP, *((XD_UINT2*)&header.tr_id));
			}

			innerStateChange(XSECS_CONN_STS_SELECT_ACCEPT);
		} else 
		if( header.sessionID == 0xFFFF && 
			header.ptype == 0 && 
			header.stype == XSECS_STYPE_SELECT_RSP ) {

			if( m_pHandler != NULL ) {

				bRet = m_pHandler->receiveHSMSCtrlCallBack(header);
			}
		} else 
		if( header.sessionID == 0xFFFF && 
			header.ptype == 0 && 
			header.stype == XSECS_STYPE_LINK_REQ ) {
			
			if( m_pHandler != NULL ) {
				bRet = m_pHandler->receiveHSMSCtrlCallBack(header);
			}
			// Link.req
			sendHSMSCtrl(XSECS_STYPE_LINK_RSP, *((XD_UINT2*)&header.tr_id));
		} else 
		if( header.sessionID == 0xFFFF && 
			header.ptype == 0 && 
			header.stype == XSECS_STYPE_LINK_RSP ) {

			if( m_pHandler != NULL ) {

				bRet = m_pHandler->receiveHSMSCtrlCallBack(header);
			}
		} else 
		if( header.sessionID == 0xFFFF && 
			header.ptype == 0 && 
			header.stype == XSECS_STYPE_SEPARATE_REQ ) {
			
			if( m_pHandler != NULL ) {

				bRet = m_pHandler->receiveHSMSCtrlCallBack(header);
			}
			// Peer���ؒf�v����������
			break;
		} else 
		if( /*header.ptype == 0 && */
			header.stype == XSECS_STYPE_DATA ) {
			// Data
			if( m_pHandler != NULL ) {
				bRet = m_pHandler->receiveDataMessageCallBack(this, header, pData, nSize);

				// 2�����b�Z�[�W����M�����H
				if( bHasSecondaryWithWait == false ) {
					m_pHandler->receiveResponseByNoWaitCallBack(this, header, pData, nSize);
				}
			}
		}
	}



	innerStateChange(XSECS_CONN_STS_NOT_CONNECTED);
	return 0;
XEXCEPT_EXIT
}
void XSEMI_HSMS::intervalProcess()
{
XEXCEPT_ENTRY
	if( m_ulLinkTest != 0 ) {
		// �����N�e�X�g�̑��M
		XTime nowTime = XTime::getCurrentTime();
		if( nowTime.m_dt > m_dtLinkTest ) {
			
			sendHSMSCtrl(XSECS_STYPE_LINK_REQ, m_usCurTicket++);

			// �f���^�l���v�Z
			double dblTime;
			dblTime = (((long)m_ulLinkTest / (60*60) * 3600L) +
				((long)m_ulLinkTest / (60) * 60L) +
				((long)m_ulLinkTest % (60))) / 86400.;
			m_dtLinkTest = dblTime + nowTime.m_dt;
		}
	}
XEXCEPT_EXIT
}

bool XSEMI_HSMS::sendHSMSCtrl(XSECS_SType type, XD_UINT2 usTicket)
{
XEXCEPT_ENTRY

	XInflateStreamBuff buff(15);
	std::ostream stream(&buff);
	XSecsData secsData;
	XMutexer			locker(m_lock);

	if( isOk() == false  || m_bEnable == false) {
		return false;
	}

	secsData.put_session(0xffff);
	secsData.get_header().stype = (unsigned char)type;
	secsData.put_ticket(usTicket);

	secsData.buildBinaryData(stream, XSecsData::SIZE_HEAD_AND_BODY);
	_XASSERT(buff.size() == 14);
	if( _writeRaw(buff.getBuff(), buff.size()) == false) {
		// ���M���s
		return false;
	}
	if( m_pHandler != NULL ) {
		m_pHandler->sendHSMSCtrlCallBack(secsData.get_header());
	}
	return true;
XEXCEPT_EXIT
}


void* XSEMI_HSMS::serverAcceptProc(XSocketClinet* pClientSock, void *pUserData)
{
XEXCEPT_ENTRY
	return ((XSEMI_HSMS*)pUserData)->serverAcceptProc(pClientSock);
XEXCEPT_EXIT
}
void* XSEMI_HSMS::serverAcceptProc(XSocketClinet* pClientSock)
{
XEXCEPT_ENTRY
	try {
		long		nSize = 0;
		XD_UINT4	ulData4 = 0;
		XSECSHeader *pHeader;
		char		szWorkBuff[512] = "";
		unsigned char rsp_sts = 0;
		/*
			�Ή�����Select.req�̃V�X�e���E�o�C�g�̒l�Ɠ������B
			----------------------------------------------------------------
			�l	����
			----------------------------------------------------------------
			0	�ʐM���m�����ꂽ�B�Z���N�g�͐������Ċ��������B
			1	�ʐM�͂��łɃA�N�e�B�u�ł���BNSR�́A�O��̃Z���N�g�ł��ł�
				�ʐM���m����	��Ă���B
			2	�ڑ��̏����������Ă��Ȃ��BNSR�́A�Z���N�g�v����������鏀��
				���܂��ł��Ă��Ȃ��B
			3	�ڑ����悤�Ƃ���HOST�i�����[�g�E�G���e�B�e�B�E�|�[�g�ԍ��Ɏw
				�肳�ꂽ�h�o�A�h���X��HOST�j�ȊO��HOST�����Select.req����M
				�����B�Z���N�g�葱�����I������B�����āATCP/IP�ڑ����N���[�Y
				���ANOT CONNECTED��Ԃɓ���B  
			-----------------------------------------------------------------
		*/
		// Peer���ؒf����܂ŁA���̃N���C�A���g�͎󂯓���Ȃ��B
		//  ��NG�D�@Accept�͎󂯓���邪�ASelect�Rsp��NG��Ԃ��B
	//	((XSocketTCPServer*)m_pSocket)->ShutDown(SD_BOTH);
		
		XThread *pSelf = XThread::Self();

		// �A�h���X���擾
		pClientSock->GetSockName(m_sockName);
		pClientSock->GetPeerName(m_peerName);

		// Select.Rsp���󂯎��܂ł́A�ڑ���...
		// T7�^�C�}�[�J�n
		//	NOT SELECTED�^�C���A�E�g�ł���T7�^�C���A�E�g���w�肵�܂��B
		//	TCP/IP�ڑ����s���Ă���ǂꂮ�炢Select.req��҂��Ƃ����҂����Ԃ�b�Ŏw�肵�܂��B�W���l��"10"�b�ł��B
		pClientSock->put_RcvTimeO( m_ulT7 * 1000 );
		if( pClientSock->Read(szWorkBuff, 14) == false ) {
			if( m_pHandler != NULL ) {
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, 
					(const TCHAR*)XStrBuff::FormatS(_T("fail to recv. Select.Req. peer:%s:%u"), 
						(const TCHAR*)m_peerName.GetIPAddress(), m_peerName.sin_port)
					);
			}
			return 0;
		}
		pClientSock->put_RcvTimeO( 0 );
		nSize = ::ntohl( *((long*)szWorkBuff) );
		pHeader = (XSECSHeader*)&szWorkBuff[4];


		// HSMS�̃w�b�_�[�`�F�b�N
		if( !(	pHeader->sessionID == 0xFFFF && 
				pHeader->ptype == 0 && 
				pHeader->stype == XSECS_STYPE_SELECT_REQ &&
				nSize == sizeof(XSECSHeader) ) ) {
			if( m_pHandler != NULL ) {
				
				m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR,
					(const TCHAR*)XStrBuff::FormatS(_T("invalid recv data. Select.Req. peer:%s:%u"), 
						(const TCHAR*)m_peerName.GetIPAddress(), m_peerName.sin_port)
				);
			}
			return 0;
		}

		// Select.Rsp ��ԐM
		if( m_bEnable == false ) {
			rsp_sts = 2;
		} else
		if( isOk() == true ) {
			rsp_sts = 3;
		} else {
			rsp_sts = 0;
		}
		m_curSock.Attach( pClientSock->Detach() );
		pHeader->status = rsp_sts;
		pHeader->stype = XSECS_STYPE_SELECT_RSP;
		if( m_curSock.Write(szWorkBuff, 14) == false ) {

			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR,
				(const TCHAR*)XStrBuff::FormatS(_T("fail to send. Select.Rsp peer:%s:%u"), 
					(const TCHAR*)m_peerName.GetIPAddress(), m_peerName.sin_port)
			);
		}

		if( rsp_sts != 0 ) {
			return 0;
		}

		bool bret = true;
		// Select.req	���󂯕t����
		innerStateChange(XSECS_CONN_STS_SELECT_ACCEPT);
		if( bret == false ) {
			m_curSock.Close();
			return 0;
		}

		//
		//	���C�����[�v���������s
		//
		this->mainProc();

		m_curSock.Detach();

		// ���ڑ�
		innerStateChange(XSECS_CONN_STS_NOT_CONNECTED);

		XTRACE(_T("Passive main trm \n"));

	} catch (XExcept& ex) { 
		if( m_pHandler != NULL ) {
			XLIB_STD_STREAM_BUFFER buff;
			// �X�g���[���ɃZ�b�g����
			XLIB_STD_OUT_STR_STREAM strm(&buff);						
			ex.print( strm );
			m_pHandler->writeLogCallBack(XLIB_LOG_LEVEL_ERROR, (const TCHAR*)XStrBuff( buff.toString() ) );
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
	return 0;
XEXCEPT_EXIT
}






