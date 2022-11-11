#include <string.h>
#include "xlib/xsocket.h"




#ifdef WIN32
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocketInit
//
XSocketInit::XSocketInit(WORD wVersionRequested)
{
	int err;
	if(wVersionRequested == 0)
		wVersionRequested = MAKEWORD( 2, 0 );
 	err = ::WSAStartup( wVersionRequested, &m_wsaData );
#ifdef _DEBUG
	XTRACE(_T("XSocketInit\n"));
	XTRACE(_T("  Version      %d\n"), m_wsaData.wVersion);
	XTRACE(_T("  HighVersion  %d\n"), m_wsaData.wHighVersion);
	XTRACE(_T("  Description  %s\n"), (LPCTSTR)XStrBuff(m_wsaData.szDescription));
	XTRACE(_T("  SystemStatus %s\n"), (LPCTSTR)XStrBuff(m_wsaData.szSystemStatus));
	XTRACE(_T("  MaxSockets   %d\n"), m_wsaData.iMaxSockets);
	XTRACE(_T("  MaxUdpDg     %d\n"), m_wsaData.iMaxUdpDg);
#endif /* _DEBUG */
}
XSocketInit::~XSocketInit()
{
	::WSACleanup( );
}
#endif /* WIN32 */



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocketAddress
//
XSocketAddress::XSocketAddress()
{
XEXCEPT_ENTRY
	memset((struct sockaddr_in*)this, 0, sizeof(struct sockaddr_in));
XEXCEPT_EXIT
}
XSocketAddress::XSocketAddress(unsigned int nAddress, unsigned short iPort, short iFamily /*= AF_INET*/)
{
XEXCEPT_ENTRY
	SetAddress(nAddress, iPort, iFamily);
XEXCEPT_EXIT
}
XSocketAddress::XSocketAddress(const char* lpszHostName, unsigned short iPort, short iFamily /*= AF_INET*/)
{
XEXCEPT_ENTRY
	SetAddress(lpszHostName, iPort, iFamily);
XEXCEPT_EXIT
}
XSocketAddress::XSocketAddress(const struct sockaddr_in* pAddr)
{
XEXCEPT_ENTRY
	SetAddress(pAddr);
XEXCEPT_EXIT
}
XSocketAddress::XSocketAddress(const struct sockaddr* pAddr)
{
XEXCEPT_ENTRY
	SetAddress(pAddr);
XEXCEPT_EXIT
}
XSocketAddress& XSocketAddress::operator=(XSocketAddress& address)
{
XEXCEPT_ENTRY
	memset((struct sockaddr_in*)this, 0, sizeof(struct sockaddr_in));
	sin_family      = address.sin_family;
	sin_port        = address.sin_port;
	sin_addr.s_addr = address.sin_addr.s_addr;
	return *this;
XEXCEPT_EXIT
}
void XSocketAddress::SetAddress(unsigned int nAddress, unsigned short iPort, short iFamily/* = AF_INET*/)
{
XEXCEPT_ENTRY
	memset((struct sockaddr_in*)this, 0, sizeof(struct sockaddr_in));
	sin_family = iFamily;
	sin_port = htons(iPort);
	sin_addr.s_addr = htonl(nAddress);
XEXCEPT_EXIT
}
void XSocketAddress::SetAddress(const char* lpszHostName, unsigned short iPort, short iFamily/* = AF_INET*/)
{
XEXCEPT_ENTRY
	LPHOSTENT lpHostEntry;
	int       i;

	memset((struct sockaddr_in*)this, 0, sizeof(struct sockaddr_in));
	sin_family = iFamily;
	sin_port = htons(iPort);
	if( (lpHostEntry = ::gethostbyname(lpszHostName) ) == NULL) {
		return ;
	}

	
	i = 0;
	while(*(lpHostEntry->h_aliases+i) != NULL) {
		m_Aliases.append(*(lpHostEntry->h_aliases+i));
		i++;
	}
	i = 0;
	while(*(lpHostEntry->h_addr_list+i) != NULL) {
		struct in_addr sin_addr;

		memcpy(&sin_addr, *(lpHostEntry->h_addr_list+i), lpHostEntry->h_length);
		if(i == 0) {
			this->sin_addr.s_addr = sin_addr.s_addr;
		} else {
			XSocketAddress *pNewAddress = new XSocketAddress(sin_addr.s_addr, iPort, iFamily);
			m_IPAddresses.push_back(pNewAddress);
#ifdef _DEBUG
			char      *pTemp;
			pTemp = (char*)::inet_ntoa(sin_addr);
			XTRACE(_T("IP(%d):%s\n"), i,  pTemp);
#endif
		}
		i++;
	}
XEXCEPT_EXIT
}
void XSocketAddress::SetAddress(const struct sockaddr_in* pAddr)
{
XEXCEPT_ENTRY
	memcpy((struct sockaddr_in*)this, pAddr, sizeof(struct sockaddr_in));
XEXCEPT_EXIT
}
void XSocketAddress::SetAddress(const struct sockaddr* pAddr)
{
XEXCEPT_ENTRY
	memcpy((struct sockaddr_in*)this, pAddr, sizeof(struct sockaddr_in));
XEXCEPT_EXIT
}
XStrBuff XSocketAddress::GetFQDN() const
{
XEXCEPT_ENTRY
	XStrBuff str;
	LPHOSTENT lpHostEntry;
	unsigned int   nAddr = this->sin_addr.s_addr;


	lpHostEntry = gethostbyaddr( (char *)&nAddr, 4, sin_family);
	if(lpHostEntry == NULL)
	{
		return str;
	}
	str = lpHostEntry->h_name;
	return str;
XEXCEPT_EXIT
}
XStrBuff XSocketAddress::GetIPAddress() const
{
XEXCEPT_ENTRY
	XStrBuff str;

	str = (const char*)::inet_ntoa(this->sin_addr);
	return str;
XEXCEPT_EXIT
}
XSocketAddress::~XSocketAddress()
{
XEXCEPT_ENTRY
	std::vector<XSocketAddress*>::iterator ite;
	ite = m_IPAddresses.begin();
	for(; ite != m_IPAddresses.end(); ite++) {
		XSocketAddress *p = *ite;
		delete p;
	}
	m_IPAddresses.clear();

XEXCEPT_EXIT
}
XSocketAddress::operator sockaddr_in*()
{
XEXCEPT_ENTRY
	return (sockaddr_in*)this;
XEXCEPT_EXIT
}
XSocketAddress::operator sockaddr*()
{
XEXCEPT_ENTRY
	return (sockaddr*)(sockaddr_in*)this;
XEXCEPT_EXIT
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocket
//
XSocket::XSocket()
{
XEXCEPT_ENTRY
	m_Socket = INVALID_SOCKET;
	m_dwLastError = 0;
	m_dwOwnError  = 0;
XEXCEPT_EXIT
}
XSocket::XSocket(SOCKET Socket)
{
XEXCEPT_ENTRY
	Attach(Socket);
	m_dwLastError = 0;
	m_dwOwnError  = 0;
XEXCEPT_EXIT
}
void XSocket::Attach(SOCKET Socket)
{
XEXCEPT_ENTRY
	m_Socket = Socket;
XEXCEPT_EXIT
}
SOCKET XSocket::Detach()
{
XEXCEPT_ENTRY
	SOCKET temp  = m_Socket;
	m_Socket = INVALID_SOCKET;
	return temp;
XEXCEPT_EXIT
}
bool XSocket::Close()
{
XEXCEPT_ENTRY
	if(m_Socket != INVALID_SOCKET)
	{
		::closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
	return true;
XEXCEPT_EXIT
}
size_t XSocket::WriteRaw(const void* lpData, size_t nSize, int flg /*= 0*/)
{
XEXCEPT_ENTRY
	return ::send(m_Socket, (const char*)lpData, nSize, flg);
XEXCEPT_EXIT
}
size_t XSocket::ReadRaw(void* lpData, size_t nSize, int flg /*= 0*/)
{
XEXCEPT_ENTRY
	return ::recv(m_Socket, (char*)lpData, nSize, flg);
XEXCEPT_EXIT
}
bool XSocket::Socket(int type /*= SOCK_STREAM*/, 
				 int af /*= AF_INET*/, 
				 int protocol /*= IPPROTO_TCP*/)
{
XEXCEPT_ENTRY
	m_Socket = ::socket(af, type, protocol);
	if(m_Socket == INVALID_SOCKET)
	{
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XSocket::Bind(XSocketAddress& address)
{
XEXCEPT_ENTRY
	if( ::bind(m_Socket, (SOCKADDR*)address, sizeof(SOCKADDR)) == SOCKET_ERROR )
	{
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XSocket::Connect(XSocketAddress& address)
{
XEXCEPT_ENTRY
	if( (::connect(m_Socket, (LPSOCKADDR)address, sizeof(SOCKADDR_IN)))  == SOCKET_ERROR)
	{
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XSocket::ShutDown(int nHow)
{
XEXCEPT_ENTRY
	return ::shutdown(m_Socket, nHow) != -1 ? true : false;
XEXCEPT_EXIT
}
bool XSocket::ReadFrom(void* lpData, unsigned long nSize, XSocketAddress &address, int flags/* = 0*/)
{
XEXCEPT_ENTRY
	unsigned long nCnt;
	char* lpPtr = (char*)lpData;
	sockaddr_in   SenderAddr;
	socklen_t     nFromlen;
	
	nFromlen = sizeof(sockaddr_in);
	for(;;) {
		nCnt = recvfrom(m_Socket, lpPtr, nSize, flags, (struct sockaddr *)&SenderAddr, &nFromlen);
		if(nCnt == SOCKET_ERROR ) { 
			m_dwLastError = XSOKET_ERRORNO;
			m_dwOwnError  = 0;
			return false;
		} else
		if(nCnt == 0) {
			m_dwLastError = XSOCK_ERR_OWN;
			m_dwOwnError  = XSOCK_ERR_OWN_RCV_XERO;
			return false;
		} else
		if(nCnt < nSize) {
			nSize -= nCnt;
			lpPtr +=  nCnt;
			address.SetAddress(&SenderAddr);
			return true;
		} else {
			address.SetAddress(&SenderAddr);
			return true;
		}
	}
XEXCEPT_EXIT
}
bool XSocket::WriteTo(const void* lpData, unsigned long nSize, XSocketAddress &address, int flags/* = 0*/)
{
XEXCEPT_ENTRY
	unsigned long nCnt;
	socklen_t     nFromlen;
	const char* lpPtr = (const char*)lpData;


	nFromlen = sizeof(sockaddr_in);
	for(;;){
		nCnt = sendto(m_Socket, lpPtr, nSize, flags, address, nFromlen);
		if(nCnt == SOCKET_ERROR ) {
			m_dwLastError = XSOKET_ERRORNO;
			m_dwOwnError  = 0;
			return false;
		} else
		if(nCnt == 0) {
			m_dwLastError = XSOCK_ERR_OWN;
			m_dwOwnError  = XSOCK_ERR_OWN_WRT_XERO;
			return false;
		} else
		if(nCnt < nSize) {
			nSize -= nCnt;
			lpPtr +=  nCnt;
		} else {
			return true;
		}
	}
XEXCEPT_EXIT
}
bool XSocket::GetPeerName(XSocketAddress& address)
{
XEXCEPT_ENTRY
	sockaddr_in sockAddr;
	socklen_t   nSockAddrLen = sizeof(sockaddr_in);
	memset(&sockAddr, 0, sizeof(sockAddr));
	if( ::getpeername(m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen) == SOCKET_ERROR ) {
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	address.SetAddress(&sockAddr);
	return true;
XEXCEPT_EXIT
}
bool XSocket::GetSockName(XSocketAddress& address)
{
XEXCEPT_ENTRY
	sockaddr_in sockAddr;
	socklen_t   nSockAddrLen = sizeof(sockaddr_in);
	memset(&sockAddr, 0, sizeof(sockAddr));
	if( ::getsockname(m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen) == SOCKET_ERROR ) {
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	address.SetAddress(&sockAddr);
	return true;
XEXCEPT_EXIT
}
bool XSocket::IOCtrl(long cmd /*= FIONREAD*/, void* argp /*= 0*/ )
{
XEXCEPT_ENTRY
	unsigned long *argv = (unsigned long *)argp;
#ifdef WIN32
	if( (::ioctlsocket(m_Socket, cmd, argv)) == SOCKET_ERROR) {
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
#else
	if( (ioctl(m_Socket, cmd, argv)) == SOCKET_ERROR) {
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
#endif /* WIN32 */
	return true;
XEXCEPT_EXIT
}
bool XSocket::GetSocketOpt(void* optval1, socklen_t *optlen, int optname, int level /*= SOL_SOCKET*/)
{
XEXCEPT_ENTRY
	char* optval = (char*)optval1;
	if( (::getsockopt(m_Socket, level, optname, optval, optlen)) == SOCKET_ERROR) {
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XSocket::SetSocketOpt(void* optval1, int optlen , int optname, int level /*= SOL_SOCKET*/)
{
XEXCEPT_ENTRY
	char* optval = (char*)optval1;
	if( (::setsockopt(m_Socket, level, optname, optval, optlen)) == SOCKET_ERROR) {
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XSocket::IsValid()
{
XEXCEPT_ENTRY
	return m_Socket != INVALID_SOCKET;
XEXCEPT_EXIT
}

bool XSocket::Listen(int nBacklog)
{
XEXCEPT_ENTRY
	if( ::listen(m_Socket, nBacklog) == SOCKET_ERROR)
	{
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XSocket::Accept(XSocket *pClient)
{
XEXCEPT_ENTRY
	XSocketAddress address;
	socklen_t      nAddressLen;
	SOCKET         sock;
	nAddressLen = sizeof(SOCKADDR_IN);

#if !defined(_WIN32)
	fd_set readfds;
	int    n;


	FD_ZERO(&readfds);    /* fdsの初期化 */
	FD_SET(m_Socket, &readfds);

	XTRACE(_T("XSocket::Accept select waitting....\n"));
	if ((n = select(FD_SETSIZE, &readfds, NULL, NULL, NULL)) == -1) {
		XTRACE(_T("XSocket::Accept select err\n"));
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	XTRACE(_T("XSocket::Accept select occore\n"));
	if (FD_ISSET(m_Socket, &readfds) == 0) {
		XTRACE(_T("XSocket::Accept select NO FD_ISSET\n"));
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
#endif
	if( (sock = ::accept(m_Socket, address, &nAddressLen)) == SOCKET_ERROR) {
		m_dwLastError = XSOKET_ERRORNO;
		m_dwOwnError  = 0;
		return false;
	}
	pClient->Attach(sock);
	return true;
XEXCEPT_EXIT
}


// SO_BROADCAST
bool XSocket::get_Broadcast()
{
XEXCEPT_ENTRY
	bool val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_BROADCAST);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_Broadcast(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_BROADCAST);
XEXCEPT_EXIT
}
// SO_DEBUG
bool XSocket::get_Debug()
{
XEXCEPT_ENTRY
	bool val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_DEBUG);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_Debug(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_DEBUG);
XEXCEPT_EXIT
}
// SO_DONTROUTE
bool XSocket::get_DontRoute()
{
XEXCEPT_ENTRY
	bool val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_DONTROUTE);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_DontRoute(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_DONTROUTE);
XEXCEPT_EXIT
}

// SO_KEEPALIVE
bool XSocket::get_KeepAlive()
{
XEXCEPT_ENTRY
	bool val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_KEEPALIVE);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_KeepAlive(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_KEEPALIVE);
XEXCEPT_EXIT
}
// SO_LINGER
void XSocket::get_Linger(LINGER *pLinger)
{
XEXCEPT_ENTRY
	socklen_t  nSize = sizeof(LINGER);
	GetSocketOpt((void*)pLinger, &nSize, SO_LINGER);
XEXCEPT_EXIT
}
void XSocket::put_Linger(const LINGER *pLinger)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&pLinger, sizeof(LINGER), SO_LINGER);
XEXCEPT_EXIT
}
// SO_OOBINLINE
bool XSocket::get_Oobline()
{
XEXCEPT_ENTRY
	bool val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_OOBINLINE);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_Oobline(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_OOBINLINE);
XEXCEPT_EXIT
}
// SO_RCVBUF
int  XSocket::get_RcvBuf()
{
XEXCEPT_ENTRY
	int val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_RCVBUF);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_RcvBuf(int newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_RCVBUF);
XEXCEPT_EXIT
}
// SO_REUSEADDR
bool XSocket::get_ReuseAddr()
{
XEXCEPT_ENTRY
	bool val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_REUSEADDR);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_ReuseAddr(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_REUSEADDR);
XEXCEPT_EXIT
}
// SO_EXCLUSIVEADDRUSE
bool XSocket::get_ExclusiveAddr()
{
XEXCEPT_ENTRY
	bool val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, ~SO_REUSEADDR);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_ExclusiveAddr(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), ~SO_REUSEADDR);
XEXCEPT_EXIT
}
// SO_SNDBUF
int  XSocket::get_SndBuf()
{
XEXCEPT_ENTRY
	int val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_RCVBUF);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_SndBuf(int newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_RCVBUF);
XEXCEPT_EXIT
}


// SO_RCVTIMEO
int  XSocket::get_RcvTimeO()
{
XEXCEPT_ENTRY
	int val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_RCVTIMEO);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_RcvTimeO(int newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_RCVTIMEO);
XEXCEPT_EXIT
}
// SO_SNDTIMEO
int  XSocket::get_SndTimeO()
{
XEXCEPT_ENTRY
	int val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_SNDTIMEO);
	return val;
XEXCEPT_EXIT
}
#if defined(WIN32)
// SO_DONTLINGER
bool XSocket::get_DontLinger()
{
XEXCEPT_ENTRY
	bool val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_DONTLINGER);
	return val;
XEXCEPT_EXIT
}
void XSocket::put_DontLinger(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_DONTLINGER);
XEXCEPT_EXIT
}
// SO_GROUP_PRIORITY
int  XSocket::get_GroupPriority()
{
XEXCEPT_ENTRY
#ifdef _WINSOCK2API_
	int val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, SO_GROUP_PRIORITY);
	return val;
#else
	return 0;
#endif //_WINSOCK2API_
XEXCEPT_EXIT
}
void XSocket::put_GroupPriority(int newVal)
{
XEXCEPT_ENTRY
#ifdef _WINSOCK2API_
	SetSocketOpt((void*)&newVal, sizeof(newVal), SO_GROUP_PRIORITY);
#endif
XEXCEPT_EXIT
}
// TCP_NODELAY for IPPROTO_TCP
bool XSocket::get_NoDelay()
{
XEXCEPT_ENTRY
	int val;
	socklen_t  nSize = sizeof(val);
	GetSocketOpt((void*)&val, &nSize, TCP_NODELAY, IPPROTO_TCP);
	return val == 1 ? true : false;
XEXCEPT_EXIT
}
void XSocket::put_NoDelay(bool newVal)
{
XEXCEPT_ENTRY
	SetSocketOpt((void*)&newVal, sizeof(newVal), TCP_NODELAY, IPPROTO_TCP);
XEXCEPT_EXIT
}
#endif /*WIN32*/

double XSocket::htond(double val)
{
	double			d;
	int             i;
	unsigned char*	pucSrcData = (unsigned char*)&val;
	unsigned char*	pucDstData = (unsigned char*)&d;

	for(i = 0; i<sizeof(double); i++) {
		pucDstData[i] = pucSrcData[sizeof(double)-i-1];
	}
	return(d);
}
double XSocket::ntohd(double val)
{
	double			d;
	int             i;
	unsigned char*	pucSrcData = (unsigned char*)&val;
	unsigned char*	pucDstData = (unsigned char*)&d;

	for(i = 0; i<sizeof(double); i++) {
		pucDstData[i] = pucSrcData[sizeof(double)-i-1];
	}
	return(d);
}
unsigned long XSocket::_htonl(unsigned long val)
{
	return htonl(val);
}
unsigned long XSocket::_ntohl(unsigned long val)
{
	return ntohl(val);
}
unsigned short XSocket::_htons(unsigned short val)
{
	return htons(val);
}
unsigned short XSocket::_ntohs(unsigned short val)
{
	return ntohs(val);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocketClinet
//
XSocketClinet::XSocketClinet()
					:XSocket()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSocketClinet::XSocketClinet(SOCKET Socket)
					:XSocket(Socket)
{
}
XSocketClinet::~XSocketClinet()
{
XEXCEPT_ENTRY
	XSocket::Close();
XEXCEPT_EXIT
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocketServer
//
XSocketServer::XSocketServer()
					:XSocket()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSocketServer::XSocketServer(SOCKET Socket)
					:XSocket(Socket)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSocketServer::~XSocketServer()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocketTCPClinet
//
XSocketTCPClinet::XSocketTCPClinet()
					:XSocketClinet()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSocketTCPClinet::XSocketTCPClinet(SOCKET Socket)
					:XSocketClinet(Socket)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSocketTCPClinet::~XSocketTCPClinet()
{
XEXCEPT_ENTRY
	XSocket::Close();
XEXCEPT_EXIT
}
bool XSocketTCPClinet::Socket()
{
XEXCEPT_ENTRY
	return XSocket::Socket(SOCK_STREAM, AF_INET, IPPROTO_TCP);
XEXCEPT_EXIT
}
bool XSocketTCPClinet::Open(XSocketAddress& address)
{
XEXCEPT_ENTRY
	if( this->IsValid() == false ) {
		if( this->Socket() == false ) {
			return false;
		}
	}

	if( this->Connect(address) == false ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocketTCPServer
//
XSocketTCPServer::ClientData::ClientData(SOCKET ClientSocket, XSocketTCPServer* pServer)
{
XEXCEPT_ENTRY
	m_Client.Attach(ClientSocket);
	m_pServer = pServer;
XEXCEPT_EXIT
}

XSocketTCPServer::XSocketTCPServer()
	:	XSocketServer()
	,	m_lockAcceptThread(1, 1)
	,	m_pTerminateSync(NULL)
{
XEXCEPT_ENTRY
	m_pFnClient = NULL;
XEXCEPT_EXIT
}
XSocketTCPServer::XSocketTCPServer(SOCKET Socket)
	:	XSocketServer(Socket)
	,	m_lockAcceptThread(1, 1)
	,	m_pTerminateSync(NULL)
{
XEXCEPT_ENTRY
	m_pFnClient = NULL;
XEXCEPT_EXIT
}
XSocketTCPServer::~XSocketTCPServer()
{
XEXCEPT_ENTRY
	XPOSITION pos = m_arrayClientDatas.GetHeadPosition();
	while( pos != NULL ) {
		ClientData *p = (ClientData*)m_arrayClientDatas.GetNext( pos );
		delete p;
	}
	m_arrayClientDatas.RemoveAll();
XEXCEPT_EXIT
}
bool XSocketTCPServer::Socket()
{
XEXCEPT_ENTRY
	return XSocket::Socket(SOCK_STREAM, AF_INET, IPPROTO_TCP);
XEXCEPT_EXIT
}

void XSocketTCPServer::Open(XSocketAddress& address, XSOCK_CLIENTPROC pFnc, void *pUserData, int nBroking)
{
XEXCEPT_ENTRY
	XTRACE(_T("XSocketTCPServer::Open Enter\n"));
	if( this->IsValid() == false ) {
		if( this->Socket() == false ) {
			Close();
			XEXCEPT_THROW(XExcept_SOCKET, m_dwOwnError, m_dwLastError);
		}
	}
	if( this->Bind(address) == false ) {
		Close();
		XEXCEPT_THROW(XExcept_SOCKET, m_dwOwnError, m_dwLastError);
	}
	if( this->Listen(nBroking) == false ) {
		Close();
		XEXCEPT_THROW(XExcept_SOCKET, m_dwOwnError, m_dwLastError);
	}
	m_pFnClient = pFnc;
	m_pUserData = pUserData;
	try {
		this->StartUndetached(); 
		XThread::yield();
	} catch(XExcept &e) {
		Close();
		throw e;
	}
XEXCEPT_EXIT
}
void XSocketTCPServer::ClientDataAccess(void (*pFunc)(unsigned long, unsigned long, XSocketTCPServer::ClientData*, void*), void* pUserData)
{
XEXCEPT_ENTRY
	XMutexer locker(m_lockClientData);
	int i = 0;
	XPOSITION pos = m_arrayClientDatas.GetHeadPosition();
	while( pos != NULL ) {
		ClientData *p = (ClientData*)m_arrayClientDatas.GetNext( pos );
		pFunc(m_arrayClientDatas.GetCount(), i++, p, pUserData);
	}
XEXCEPT_EXIT
}

#include <iostream>
bool XSocketTCPServer::Close()
{
XEXCEPT_ENTRY
	XTRACE(_T("XSocketTCPServer::Close Enter\n"));
	
	unsigned long nAliveClient = 0;
	

	// ソケットを閉じる
	XSocket::Close();


	// スレッドが終了するのを待ちます。
	XTRACE(_T("XSocketTCPServer::Close  Waitting RunUndetached thread end ...\n"));
	{
		m_lockClientData.lock();
		nAliveClient = m_arrayClientDatas.GetCount();
		if( nAliveClient != 0 ) {
			m_pTerminateSync = new XSemaphore(0, nAliveClient);
		}
		m_lockClientData.ulock();
		if( m_pTerminateSync != NULL ) {
			m_pTerminateSync->Wait();
			delete m_pTerminateSync;
		}
	}
	XTRACE(_T("XSocketTCPServer::Close  Waitting RunUndetached thread Terminated\n"));

	XSemaphorer			locker(m_lockAcceptThread);
	
	XTRACE(_T("XSocketTCPServer::Close End\n"));
	return true;
XEXCEPT_EXIT
}
void* XSocketTCPServer::RunUndetached(void* pArg)
{
XEXCEPT_ENTRY
	XTRACE(_T("XSocketTCPServer::RunUndetached Enter\n"));
	XSemaphorer      locker(m_lockAcceptThread);
	XSocketTCPClinet client;
	while( this->Accept(&client ) ) {
		XTRACE(_T("XSocketTCPServer::RunUndetached Accept...\n"));
		ClientData *pClientData = new ClientData(client.Detach(), this);
		if(pClientData == NULL) {
			continue;
		}
		XThread *pThread = new XThread(XSocketTCPServer::RunClient, pClientData);
		if(pThread == NULL) {
			delete pClientData;
			continue;
		}
		pClientData->m_pThread = pThread;
		try {
			pThread->Start();
		} catch(XExcept &) {
			delete pClientData;
		}
	}
	XTRACE(_T("XSocketTCPServer::RunUndetached End\n"));
	return NULL;
XEXCEPT_EXIT
}
void* XSocketTCPServer::RunClient(void *pData)
{
XEXCEPT_ENTRY
	XTRACE(_T("XSocketTCPServer::RunClient Enter\n"));
	void *pRet = NULL;
	XSocketTCPServer::ClientData* pClientData = (XSocketTCPServer::ClientData*)pData;
	
	pClientData->m_pThread = XThread::Self();
	pClientData->m_pServer->AddClientData(pClientData);

	pRet = pClientData->m_pServer->m_pFnClient( &pClientData->m_Client, pClientData->m_pServer->m_pUserData );

	pClientData->m_Client.Close();

	XSocketTCPServer* pServer = pClientData->m_pServer; 
	pServer->DelClientData(pClientData);

	if( pServer->m_pTerminateSync ) {
		// サーバーのクライアント終了待ちを行っているばあい、
		// セマフォを、ポストする
		pServer->m_pTerminateSync->unlock();
	}
	
	XTRACE(_T("XSocketTCPServer::RunClient End\n"));
	return pRet;
XEXCEPT_EXIT
}
void XSocketTCPServer::AddClientData(XSocketTCPServer::ClientData* pData)
{
XEXCEPT_ENTRY
	XMutexer locker(m_lockClientData);
	m_arrayClientDatas.AddTail(pData);
XEXCEPT_EXIT
}
void XSocketTCPServer::DelClientData(XSocketTCPServer::ClientData* pData)
{
XEXCEPT_ENTRY
	XMutexer locker(m_lockClientData);
	int i = 0;

	XPOSITION pos1, pos2;
	for( pos1 = m_arrayClientDatas.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		if( (ClientData*)m_arrayClientDatas.GetNext( pos1 ) == pData )
		{
			ClientData* pa = (ClientData*)m_arrayClientDatas.GetAt( pos2 ); // Save the old pointer for
			m_arrayClientDatas.RemoveAt( pos2 );
			delete pa; 
			break;
		}
	}
XEXCEPT_EXIT
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocketUDPClinet
//
XSocketUDPClinet::XSocketUDPClinet()
					:XSocketClinet()
{
XEXCEPT_ENTRY
	m_bClose = true;
XEXCEPT_EXIT
}
XSocketUDPClinet::XSocketUDPClinet(SOCKET Socket)
					:XSocketClinet(Socket)
{
XEXCEPT_ENTRY
	m_bClose = true;
XEXCEPT_EXIT
}
XSocketUDPClinet::~XSocketUDPClinet()
{
XEXCEPT_ENTRY
	XSocket::Close();
XEXCEPT_EXIT
}
bool XSocketUDPClinet::Socket()
{
XEXCEPT_ENTRY
	return XSocket::Socket(SOCK_DGRAM, AF_INET, 0);
XEXCEPT_EXIT
}
bool XSocketUDPClinet::Open(XSocketAddress& address)
{
XEXCEPT_ENTRY
	if( this->IsValid() == false ) {
		if( this->Socket() == false ) {
			return false;
		}
	}

	if( this->Bind(address) == false ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XSocketUDPClinet::Close()
{
XEXCEPT_ENTRY
	if(m_bClose == false)
		return true;
	return XSocket::Close();
XEXCEPT_EXIT
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSocketUDPServer
//
XSocketUDPServer::XSocketUDPServer()
				:XSocketServer()
{
XEXCEPT_ENTRY
	m_pFnClient = NULL;
XEXCEPT_EXIT
}
XSocketUDPServer::XSocketUDPServer(SOCKET Socket)
				:XSocketServer(Socket)
{
XEXCEPT_ENTRY
	m_pFnClient = NULL;
XEXCEPT_EXIT
}
XSocketUDPServer::~XSocketUDPServer()
{
XEXCEPT_ENTRY
	Close();
XEXCEPT_EXIT
}
bool XSocketUDPServer::Socket()
{
XEXCEPT_ENTRY
	return XSocket::Socket(SOCK_DGRAM, AF_INET, 0);	
XEXCEPT_EXIT
}
void XSocketUDPServer::Open(XSocketAddress& address, XSOCK_CLIENTPROC pFnc, void *pUserData, int nBroking/* = 5*/)
{
XEXCEPT_ENTRY
	if( this->IsValid() == false ) {
		if( this->Socket() == false ) {
			Close();
			XEXCEPT_THROW(XExcept_SOCKET, m_dwOwnError, m_dwLastError);
		}
	}
	if( this->Bind(address) == false ) {
		Close();
		XEXCEPT_THROW(XExcept_SOCKET, m_dwOwnError, m_dwLastError);
	}
	m_pFnClient = pFnc;
	m_pUserData = pUserData;
	try {
		this->StartUndetached(); 
		XThread::yield();
	} catch(XExcept& e) {
		Close();
		throw e;
	}
XEXCEPT_EXIT
}
bool XSocketUDPServer::Close()
{
XEXCEPT_ENTRY
	XSocket::Close();

	// スレッドが終了するのを待ちます。
	XSemaphorer      locker(m_lockAcceptThread);
	XThread::yield();
//	XThread::Close();
	return true;
XEXCEPT_EXIT
}
void* XSocketUDPServer::RunUndetached(void* pArg)
{
XEXCEPT_ENTRY
	XSemaphorer      locker(m_lockAcceptThread);
	void             *pRet = NULL;
	XSocketUDPClinet sock(m_Socket);
	pRet = m_pFnClient(&sock, m_pUserData);
	sock.Detach();
	return pRet;
XEXCEPT_EXIT
}











