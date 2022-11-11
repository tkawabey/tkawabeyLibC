#ifdef WIN32
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
 #include <winsock2.h>
 #ifndef _WINDEF_
  #include <windef.h>
 #endif //_WINDEF_
 #ifndef _WINERROR_
  #include <winerr.h>
 #endif //_WINERROR_
  #include <ws2tcpip.h>		// #define IP_TTL をインクルード
 
  #include <crtdbg.h>
  #include <tchar.h>
  #include <tchar.h>
  #include <stdio.h>

  #include "xlib/xeception.h"
  #include "xlib/windows/xWinIPHelp.h"



	/*
		以下に示す、IPヘッダー構造体などは、LINUXのソースから持ってきた
	*/

	typedef unsigned char u_int8_t;
	typedef signed char int8_t;
	typedef unsigned short u_int16_t;
	typedef signed short int16_t;
	typedef unsigned int u_int32_t;
	typedef signed int int32_t;

	#define ETHER_LEN 12
	#define ICMP_ECHO 8
	#define ICMP_REPLY 0
	#define ICMP_TTL 11
	#define ICMP_UNREACHED 3

	#define REQ_DATASIZE 32		// Echo Request Data size

	#define ICMP_ECHOREPLY	0
	#define ICMP_ECHOREQ	8

	/*-------------------- copy from windump\ethertype.h -----------------------------------*/
	#ifndef ETHERTYPE_IP
	#define	ETHERTYPE_IP		0x0800	/* IP protocol */
	#endif

	/*-------------------- copy from windump\ip.h -----------------------------------*/
	struct ip {
		u_int8_t	ip_vhl;		/* header length, version */
	#define IP_V(ip)	(((ip)->ip_vhl & 0xf0) >> 4)
	#define IP_HL(ip)	((ip)->ip_vhl & 0x0f)
		u_int8_t	ip_tos;		/* type of service */
		u_int16_t	ip_len;		/* total length */
		u_int16_t	ip_id;		/* identification */
		u_int16_t	ip_off;		/* fragment offset field */
	#define	IP_DF 0x4000			/* dont fragment flag */
	#define	IP_MF 0x2000			/* more fragments flag */
	#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
		u_int8_t	ip_ttl;		/* time to live */
		u_int8_t	ip_p;		/* protocol */
		u_int16_t	ip_sum;		/* checksum */
		struct	in_addr ip_src,ip_dst;	/* source and dest address */
	};

	/*-------------------- copy from windump\tcp.h -----------------------------------*/
	typedef	u_int32_t	tcp_seq;
	/*
	 * TCP header.
	 * Per RFC 793, September, 1981.
	 */
	struct tcphdr {
		u_int16_t	th_sport;		/* source port */
		u_int16_t	th_dport;		/* destination port */
		tcp_seq		th_seq;			/* sequence number */
		tcp_seq		th_ack;			/* acknowledgement number */
		u_int8_t	th_offx2;		/* data offset, rsvd */
	#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
		u_int8_t	th_flags;
	#define	TH_FIN	0x01
	#define	TH_SYN	0x02
	#define	TH_RST	0x04
	#define	TH_PUSH	0x08
	#define	TH_ACK	0x10
	#define	TH_URG	0x20
		u_int16_t	th_win;			/* window */
		u_int16_t	th_sum;			/* checksum */
		u_int16_t	th_urp;			/* urgent pointer */
	};




	// ICMP Header - RFC 792
	struct icmp_hdr
	{
		u_char	icmp_type;		// type
		u_char	icmp_code;		// code
		u_short	icmp_sum;		// icmp_sum
		u_short	icmp_id;		// Identification
		u_short	icmp_seq;		// Sequence
		unsigned char	icmp_data;			// Data
	};


	#define REQ_DATASIZE 32		// Echo Request Data size

	// ICMP Echo Request
	struct echo_req
	{
		struct icmp_hdr icmp_h;
		u_int32_t		echo_req_time;
		unsigned char	data[REQ_DATASIZE];
	};


	// ICMP Echo Reply
	struct echo_rsp
	{
		struct ip		ip_h;
		struct echo_req	echoRequest;
		unsigned char   cfilter[256];
	};


unsigned long xWinIPHelp::inetAddrW(wchar_t* pAddr)
{
XEXCEPT_ENTRY
	char sz[64];
	wcstombs(sz, pAddr, wcslen(pAddr)+1);
	return ::inet_addr(sz);
XEXCEPT_EXIT
}
unsigned long xWinIPHelp::inetAddrA(char* pAddr)
{
XEXCEPT_ENTRY
	return ::inet_addr(pAddr);
XEXCEPT_EXIT
}

XStrBuff xWinIPHelp::inetNtot(unsigned long ulIP)
{
XEXCEPT_ENTRY
	struct in_addr inAddr;
	inAddr.S_un.S_addr = ulIP;
#ifdef _UNICODE
	wchar_t sz[64];
	char *pIP = inet_ntoa(inAddr);

	mbstowcs(sz, pIP, strlen(pIP)+1);

	return XStrBuff(sz);
#else
	return XStrBuff( inet_ntoa(inAddr) );
#endif
XEXCEPT_EXIT
}
struct hostent* xWinIPHelp::getHostByNameW(wchar_t* pName)
{
XEXCEPT_ENTRY
	char sz[64];
	wcstombs(sz, pName, wcslen(pName)+1);
	return ::gethostbyname(sz);
XEXCEPT_EXIT
}
struct hostent* xWinIPHelp::getHostByNameA(char* pName)
{
XEXCEPT_ENTRY
return ::gethostbyname(pName);
XEXCEPT_EXIT
}


void xWinIPHelp::strToMAC_W(wchar_t *pMAC, unsigned char* pMACAddress)
{
	unsigned char tmp;
	wchar_t* pwk;
	int i;

	for(i = 0; i < 6; i++) {
		tmp = (unsigned char)::wcstoul(pMAC, &pwk, 16);
		if( pwk == NULL ) {
			return;
		}
		if(  *pwk != _T('-') && i != 5 ) {
			return;
		}
		pMAC = ++pwk;
		pMACAddress[i] = tmp;
	}
}
void xWinIPHelp::strToMAC_A(char *pMAC, unsigned char* pMACAddress)
{
	unsigned char tmp;
	char* pwk;
	int i;

	for(i = 0; i < 6; i++) {
		tmp = (unsigned char)strtoul(pMAC, &pwk, 16);
		if( pwk == NULL ) {
			return;
		}
		if(  *pwk != _T('-') && i != 5 ) {
			return;
		}
		pMAC = ++pwk;
		pMACAddress[i] = tmp;
	}
}


static u_short in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}

	if( nleft == 1 ) {
		u_short	u = 0;

		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

int xWinIPHelp::pingProccess(unsigned long ipaddress , int loop, int timeout, int ttl)
{
XEXCEPT_ENTRY
	int    sock;
	struct sockaddr_in addrin, rcvAddr;
	int		adderSize;
	struct echo_req	echoReq;
	clock_t		elapce_time;
	int i;
	unsigned char buff[1500];
	struct ip* pIP;
	struct echo_req* pEchoReq;
	int ret = 0;
	struct echo_rsp rs;

	// ログ 
	addrin.sin_addr.s_addr = ipaddress;
	addrin.sin_family = AF_INET;
	addrin.sin_port = 0;


#ifdef _UNICODE
//	my_printf(L"Pinging %S with 32 bytes of data:\n", inet_ntoa(addrin.sin_addr));
#else
//	my_printf("Pinging %s with 32 bytes of data:\n",  inet_ntoa(addrin.sin_addr));
#endif


	//
	//	ソケット作成
	//
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if( sock == ~0 ) {
//		my_printf(_T("socket error\n"));
		return -1;
	}

	//
	//	受信タイムアウトを設定
	//
	if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == ~0) {
//		my_printf( _T("setsockopt(SO_RCVTIMEO) error\n"));
		return -1;
	}
	//
	//	TTLを設定
	//
	if(setsockopt(sock, IPPROTO_IP, IP_TTL, (char *)&ttl, sizeof(ttl)) == ~0) {
//		my_printf(_T("setsockopt(SO_RCVTIMEO) error\n"));
		return -1;
	}


	for(i = 0; i < loop; i++) {

		// エコー要求に情報を設定
		echoReq.icmp_h.icmp_type	= ICMP_ECHOREQ;
		echoReq.icmp_h.icmp_code	= 0;
		echoReq.icmp_h.icmp_sum		= 0;
		echoReq.icmp_h.icmp_id		= i;
		echoReq.icmp_h.icmp_seq		= i;
		echoReq.echo_req_time       = clock();
		memset(echoReq.data, 'x', sizeof(echoReq.data));

		// パケット内にデータを入れ、チェックサムを計算
		echoReq.icmp_h.icmp_sum = in_cksum((u_short *)&echoReq, sizeof(struct echo_req));

		// エコー要求を送信
		if( sendto(sock, (const char*)&echoReq, sizeof(struct echo_req), 0, (struct sockaddr *) &addrin, sizeof(addrin)) == ~0 ) {
//			my_printf(_T("sendto error\n"));
			return -1;
		}

		// エコー応答を受信
		adderSize = sizeof(struct sockaddr_in);
		memset(buff, 0 , sizeof(buff));
		if( recvfrom(sock, (char*)buff, sizeof(buff), 0, (struct sockaddr *)&rcvAddr, &adderSize) == ~0 ) {
			if (WSAGetLastError() == WSAETIMEDOUT) {
//				my_printf(_T("Request timed out.\n"));
			} else {
//				my_printf(_T("recive error!!\n"));
			}
			continue;
		}

		
		memcpy(&rs, buff, sizeof(&rs));
		pIP = (struct ip*)buff;
		pEchoReq = (struct echo_req*)(buff + ( IP_HL(pIP) * 4));


   		switch( pEchoReq->icmp_h.icmp_type ) {
   	    case ICMP_TTL:
//			my_printf(_T("TTL\n"));
            break;
		case ICMP_UNREACHED:
//			my_printf(_T("Distnation Unreacheble.\n"));
			break;
		case ICMP_REPLY:
			if( pEchoReq->icmp_h.icmp_id == i ){
				// 経過時間
				elapce_time = clock() - pEchoReq->echo_req_time;
				// ログ 
#ifdef _UNICODE
//				my_printf(L"Reply from %S: bytes=%d time<%ums TTL=%d\n", inet_ntoa(rcvAddr.sin_addr), htons(pIP->ip_len) - ETHER_LEN, elapce_time, pIP->ip_ttl);
#else
//				my_printf( "Reply from %s: bytes=%d time<%ums TTL=%d\n", inet_ntoa(rcvAddr.sin_addr), htons(pIP->ip_len) - ETHER_LEN, elapce_time, pIP->ip_ttl);
#endif
			}
			break;
		default:
//			my_printf(_T("MTP TYPE=%d\n"), pEchoReq->icmp_h.icmp_type);
   		  	break;

        }
		ret = 1;
	}
	closesocket( sock );
	return ret;
XEXCEPT_EXIT
}
unsigned long xWinIPHelp::wakeUpLan(unsigned long uBroadCast, unsigned char* pMac)
{
XEXCEPT_ENTRY

	int i, j;
	int  msglen = 0;
	char		  msg[1024];
	int			  sock;
	int			  optval = 1;
	struct sockaddr_in bcast;
	int			  rc;

	// マジックパケットというのは、６個の FF(16進)のあと、MACアドレスを１６回以上繰り返した
	// UDPパケット。

    for (i = 0; i < 6; i++) {
		msg[msglen++] = (unsigned char)0xff;
    }
    for (i = 0; i < 16; i++) {
		for (j = 0; j < 6; j++) {
			msg[msglen++] = pMac[j];
		}
    }
    memset(&bcast, 0, sizeof(bcast));
    bcast.sin_family      = AF_INET;
    bcast.sin_addr.s_addr = uBroadCast;
    bcast.sin_port        = htons(29887);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
//		my_printf(_T("socket %d"), WSAGetLastError());
		return WSAGetLastError();
    }
    if((rc=setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char *)&optval, sizeof(optval))) < 0) {
//		my_printf(_T("setsockopt %d"), WSAGetLastError());
		return WSAGetLastError();
    }
	sendto(sock,(const char *)&msg, msglen, 0, (struct sockaddr *)&bcast, sizeof(bcast));
	closesocket(sock);
	return 0;	
XEXCEPT_EXIT
}

// プラットフォームSDKをインストールしていない場合
#define MAXLEN_PHYSADDR 8

typedef struct _MIB_IPNETROW
{
	DWORD		dwIndex;
	DWORD		dwPhysAddrLen;
	BYTE		bPhysAddr[MAXLEN_PHYSADDR];
	DWORD		dwAddr;
	DWORD		dwType;
} MIB_IPNETROW, *PMIB_IPNETROW;

typedef struct _MIB_IPNETTABLE
{
	DWORD             dwNumEntries;
	MIB_IPNETROW      table[1];
} MIB_IPNETTABLE, *PMIB_IPNETTABLE;

typedef struct _MIB_IPADDRROW
{
	DWORD		dwAddr;
	DWORD		dwIndex;
	DWORD		dwMask;
	DWORD		dwBCastAddr;
	DWORD		dwReasmSize;
	unsigned short	unused1;
	unsigned short	unused2;
} MIB_IPADDRROW, *PMIB_IPADDRROW;

typedef struct _MIB_IPADDRTABLE
{
	DWORD         dwNumEntries;
	MIB_IPADDRROW table[1];
} MIB_IPADDRTABLE, *PMIB_IPADDRTABLE;


// 関数の定義
typedef DWORD (WINAPI *defGetIpNetTable)(PMIB_IPNETTABLE , PULONG ,BOOL );
typedef DWORD (WINAPI *defGetIpAddrTable)(PMIB_IPADDRTABLE ,PULONG , BOOL);

xWinIPHelp::xWinIPHelp()
	:	m_module( NULL )
	,	m_pGetIpNetTable( NULL )
	,	m_pGetIpAddrTable( NULL )
{
XEXCEPT_ENTRY
	m_module = ::LoadLibrary(_T("iphlpapi.dll"));
	if( m_module == NULL ) {
		return;
	}
	m_pGetIpNetTable  = ::GetProcAddress(m_module, "GetIpNetTable");
	m_pGetIpAddrTable = ::GetProcAddress(m_module, "GetIpAddrTable");
XEXCEPT_EXIT
}

xWinIPHelp::~xWinIPHelp()
{
XEXCEPT_ENTRY
	if( m_module != NULL ) {
		::FreeLibrary( m_module );
	}
	m_module = NULL;
XEXCEPT_EXIT
}

bool xWinIPHelp::getIpNetTable(xWinIPNetRows &raws, BOOL bOrder)
{
XEXCEPT_ENTRY
    DWORD status = NO_ERROR;
    DWORD statusRetry = NO_ERROR;
    DWORD dwActualSize = 0, i;
	MIB_IPNETTABLE* pIpNetTabl = NULL;

	if( m_pGetIpNetTable == NULL ) {
		return false;
	}
    
    dwActualSize = 0;
    status = ((defGetIpNetTable)m_pGetIpNetTable)(pIpNetTabl, &dwActualSize, bOrder);
    if (status == NO_ERROR) {
        return false;
    }
    else if (status == ERROR_INSUFFICIENT_BUFFER) {
        pIpNetTabl = (PMIB_IPNETTABLE) malloc(dwActualSize);
		if( pIpNetTabl == NULL ) {
//			my_printf( _T("%s : %d\n"), _T("malloc"), GetLastError());		
			return false;
		}
        statusRetry = ((defGetIpNetTable)m_pGetIpNetTable)(pIpNetTabl, &dwActualSize, bOrder);

        if (statusRetry != NO_ERROR)  {
//			my_printf( _T("%s : %d\n"), _T("GetIpNetTable"), GetLastError());		
			free(pIpNetTabl);
            return false;
        } else {

			for(i = 0; i < pIpNetTabl->dwNumEntries; i++) {
				xWinIPHelpNetRow data;
				::memcpy(&data, &pIpNetTabl->table[i], sizeof(data));
				raws.push_back( data );
			}
			free(pIpNetTabl);
            return true;
        }
    }  else {
//		my_printf( _T("%s : %d\n"), _T("GetIpNetTable"), GetLastError());		
        return false;
    }
XEXCEPT_EXIT
	return false;
}
bool xWinIPHelp::getIpAddrTable(xWinIPAddrRows& rows, BOOL bOrder)
{
XEXCEPT_ENTRY
    DWORD status = NO_ERROR;
    DWORD statusRetry = NO_ERROR;
    DWORD dwActualSize = 0, i;
	MIB_IPADDRTABLE* pIpAddrTable = NULL;


	// APIをロードします。
	if( m_pGetIpAddrTable == NULL ) {
		return false;
	}

    status = ((defGetIpAddrTable)m_pGetIpAddrTable)(pIpAddrTable, &dwActualSize, bOrder);
    if (status == NO_ERROR) {
//        printf("No error\n");
        return false;
    } else if (status == ERROR_INSUFFICIENT_BUFFER)  {
        pIpAddrTable = (PMIB_IPADDRTABLE) malloc(dwActualSize);
        status = ((defGetIpAddrTable)m_pGetIpAddrTable)(pIpAddrTable, &dwActualSize, bOrder);
		if( status != 0 ) {
			free(pIpAddrTable);
			return false;
		}
		for(i = 0; i < pIpAddrTable->dwNumEntries; i++) {
			_xWinIPHelpAddrRow data;
			memcpy(&data, &pIpAddrTable->table[i], sizeof(data));
			rows.push_back( data );
		}
		free(pIpAddrTable);
		return true;
    } else {
       return true;
    }
XEXCEPT_EXIT
	return false;
}



#endif
