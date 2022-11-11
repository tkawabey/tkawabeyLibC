#include "xlib/semi/secs/xsecs_pcap_parser.h"
#include "xlib/xfile.h"
#include "xlib/xeception.h"

#include <pcap.h>
#include <memory>

#ifdef _WIN32

/*-------------------- copy from windump\ether.h -----------------------------------*/
#define ETHER_HDRLEN		14
#define	ETHER_ADDR_LEN		6
/*
 * Structure of a DEC/Intel/Xerox or 802.3 Ethernet header.
 */
struct	ether_header {
	u_int8_t	ether_dhost[ETHER_ADDR_LEN];
	u_int8_t	ether_shost[ETHER_ADDR_LEN];
	u_int16_t	ether_type;
};
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

#else /* linux . unix */
#include <sys/socket.h>
#include <socketbits.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif



//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
static void XSecsPcapParser_catptureHandler(u_char *pParam,
						const struct pcap_pkthdr *header, 
						const u_char *pkt_data)
{
XEXCEPT_ENTRY
	((XSecsPcapParserImple*)pParam)->innerParse(header, pkt_data);
XEXCEPT_EXIT
}

static void XSecsPcapParser_PrintBlank(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth)
{
	XD_INT4 i = 0;
	for(i = 0; i < nDepth; i++) {
		stream << _T(" ");
	}
}
static void XSecsPcapParser_PrintDump(XLIB_STD_OUT_STR_STREAM &stream, 
									  XD_INT4 nDepth, const TCHAR *pLF,
									  XBIN *pBin, unsigned long ulBunLen)
{
	//  0000 : 00 00 00 49 00 00 00 49 00 00 00 49 00 00 00 49 : 0123456789123456
	u_int nOneLineSize = 80;
	u_int nLine = ulBunLen / 16 + ((ulBunLen%16) != 0 ? 1 : 0);
	TCHAR  szASCII[20];
	TCHAR  szBuff[1024];
	unsigned long i, iAsciiPos, iTmp;
	XLIB_STD_STREAM_BUFFER  workerBuff;
	XLIB_STD_OUT_STR_STREAM workerStream( &workerBuff );
	
	::memset(szASCII, 0, sizeof(szASCII));
	::memset(szBuff, 0, sizeof(szBuff));

	for(i = 1, iAsciiPos = 0; i < ulBunLen+1; i++) {
		if( (i % 16) == 1 ) {
			XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("  %04X : "), i-1);
			workerStream << szBuff;
		}


		XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%02X "), (u_char)*pBin);
		workerStream << szBuff;


		szASCII[iAsciiPos] =  (0x20 <= *pBin && *pBin <= 0x7e) ? *pBin : '.';

		if( (i % 16) == 0 ) {
			XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T(": %s"), szASCII);
			workerStream << szBuff;

			XSecsPcapParser_PrintBlank(stream, nDepth);
			stream << workerBuff.toString() <<  pLF;
			workerBuff.resetp();
			
			::memset(szASCII, 0, sizeof(szASCII));
			::memset(szBuff, 0, sizeof(szBuff));

			iAsciiPos = 0;
		}else {
			iAsciiPos++;
		}
		pBin++;
	}
	if( ( iTmp = ((i-1) % 16)) != 0 ) {
		if( iAsciiPos != 0 ) {
			iTmp = 16 - iTmp +1;
			while( iTmp != 1 ) {
				workerStream << _T("   ");
				iTmp--;
			}
			XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T(": %s"), szASCII);

			XSecsPcapParser_PrintBlank(stream, nDepth);
			stream << workerBuff.toString() <<  pLF;
		}
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSecsPcapData imple
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
XSecsPcapData::XSecsPcapData()
	:	m_bPrintHeaderOnly( false )
	,	m_type( XSECS_HSMS_TYPE_UNKNOW )
{
XEXCEPT_ENTRY
	m_lengthPrev = 0;
XEXCEPT_EXIT
}
XSecsPcapData::~XSecsPcapData()
{
XEXCEPT_ENTRY
	// パケットヘッダ情報を削除
	std::vector<XSecsPcapPacketHead*>::iterator ite = m_pcket_headers.begin();
	for(; ite != m_pcket_headers.end(); ite++) {
		XSecsPcapPacketHead* p = *ite;
		delete p;
	}
	m_pcket_headers.clear();
XEXCEPT_EXIT
}
XSecsPcapData& XSecsPcapData::operator = (const XSecsPcapData& right)
{
XEXCEPT_ENTRY

	::memcpy(&m_Time, &right.m_Time, sizeof(m_Time));
	m_ulTimeUSec = right.m_ulTimeUSec;
	m_ip_src.S_un.S_addr = right.m_ip_src.S_un.S_addr;
	m_ip_dst.S_un.S_addr = right.m_ip_dst.S_un.S_addr;
	m_port_src = right.m_port_src;
	m_port_dst = right.m_port_dst;
	m_th_seq = right.m_th_seq;
	m_th_ack = right.m_th_ack;
	m_th_flags = right.m_th_flags;
	m_th_win = right.m_th_win;
	m_th_sum = right.m_th_sum;
	m_th_urp = right.m_th_urp;
	m_length = right.m_length;

	m_lengthPrev = right.m_length;

	return *this;
XEXCEPT_EXIT
}
bool XSecsPcapData::innerPrint(		XLIB_STD_OUT_STR_STREAM &stream
							   ,	XD_INT4 nDepth /*= 0*/
							   ,	const TCHAR *pLF /*= "\n"*/
							   ,	bool bTCPIPheader /*= false*/
							   ,	bool bDump /*= false*/)
{
XEXCEPT_ENTRY
	XStrBuff strText;
	TCHAR	szHexBuff[1024], msec[64];;
	XSecsPcapParser_PrintBlank(stream, nDepth);



	strText.Format(_T("%04u/%02u/%02u %02u:%02u:%02u.%10u   "), 
			m_Time.tm_year + 1900, m_Time.tm_mon+1, m_Time.tm_mday,
			m_Time.tm_hour, m_Time.tm_min, m_Time.tm_sec, m_ulTimeUSec);
	stream << (const TCHAR*)strText;

	if(m_type == XSECS_HSMS_TYPE_DATA) {
		stream << _T("  ");
/*
2007/08/09 16:55:58.59   <Receive>       ( S:35, F:37, W:1, T:0019, length:21 )
 16.163.65.177.4002 < 16.148.52.116.1161   ( SEQ:857393162, ACK:1661146632, CTL=ACK,PSH )
  header:    0000 A325 0000 0000  0019  /  length: 0000 0011
  <S35F37 W
    <L[2/1]
      <BOOL[1/1] 0x00>
      <L[0/1]>
    >
  >
*/
		// SECS-II 
		XSecsPcapParser_PrintBlank(stream, nDepth+1);
		XBIN iStream = m_secsHeader.byte2 & 0x80 ? m_secsHeader.byte2 - 0x80 : m_secsHeader.byte2;
		XD_UINT2 usTicket;
		::memcpy(&usTicket, m_secsHeader.tr_id, sizeof(usTicket));
		XStrBuff::printf(szHexBuff, XCOUNTOF(szHexBuff), _T("( S:%u, F:%u, W:%s, T:%04X, length:%u )"),
				(int)iStream
			,	(int)m_secsHeader.status
			,	m_secsHeader.byte2 & 0x80 ? _T("1"): _T("0")
			,	::ntohs(usTicket)
			,	m_secsDataLength
			);
		stream << szHexBuff;
	} else
	if(m_type == XSECS_HSMS_TYPE_HSMS) {


/*
2007/08/09 16:54:44.07   <Receive>       Select.req
 16.163.65.177.4002 < 16.148.52.116.1153   ( SEQ:2915498878, ACK:1053879184, CTL=ACK,PSH )
  header:    FFFF 0000 0001 0000  0010  /  length: 0000 000A

2007/08/09 16:54:44.60   <Send>          Select.rsp ( Status 2 )
 16.163.65.177.4002 > 16.148.52.116.1153   ( SEQ:1053879184, ACK:2915498892, CTL=ACK,PSH )
  header:    FFFF 0002 0002 0000  0010  /  length: 0000 000A
*/

		stream << _T("  ");

		XD_UINT2 usTicket;
		::memcpy(&usTicket, m_secsHeader.tr_id, sizeof(usTicket));

		if( m_secsHeader.stype == (XBIN)XSECS_STYPE_SELECT_REQ ) {
			stream << _T("Select.Req");
		} else
		if( m_secsHeader.stype == (XBIN)XSECS_STYPE_SELECT_RSP ) {
			stream << _T("Select.Rsp ") << _T("( Status ") << (int)m_secsHeader.status << _T(" )");
		} else
		if( m_secsHeader.stype == (XBIN)XSECS_STYPE_DELETE_REQ ) {
			stream << _T("Delete.Req");
		} else
		if( m_secsHeader.stype == (XBIN)XSECS_STYPE_DELETE_RSP ) {
			stream << _T("Delete.Rsp");
		} else
		if( m_secsHeader.stype == (XBIN)XSECS_STYPE_LINK_REQ ) {
			stream << _T("Linktest.Req");
		} else
		if( m_secsHeader.stype == (XBIN)XSECS_STYPE_LINK_RSP ) {
			stream << _T("Linktest.Rsp");
		} else
		if( m_secsHeader.stype == (XBIN)XSECS_STYPE_REJECT_REQ ) {
			stream << _T("Reject.Req");
		} else
		if( m_secsHeader.stype == (XBIN)XSECS_STYPE_SEPARATE_REQ ) {
			stream << _T("Separate.Req");
		}
	}

	stream << pLF;

	/*
	 16.163.65.177.4002 < 16.148.52.116.1153   ( SEQ:2915498878, ACK:1053879184, CTL=ACK,PSH )
	*/
	XSecsPcapParser_PrintBlank(stream, nDepth+1);
	strText.Format(_T("%d.%d.%d.%d:%d"), m_ip_src.S_un.S_un_b.s_b1,
		m_ip_src.S_un.S_un_b.s_b2,
		m_ip_src.S_un.S_un_b.s_b3,
		m_ip_src.S_un.S_un_b.s_b4,
		m_port_src);
	stream << (const TCHAR*)strText << _T(" > ");
	strText.Format(_T("%d.%d.%d.%d:%d"), m_ip_dst.S_un.S_un_b.s_b1,
		m_ip_dst.S_un.S_un_b.s_b2,
		m_ip_dst.S_un.S_un_b.s_b3,
		m_ip_dst.S_un.S_un_b.s_b4,
		m_port_dst);
	stream << (const XBIN*)strText << _T(" ");

	strText.Format(_T("( SEQ:%u, ACK:%u LEN:%u flg:"), m_th_seq, m_th_ack, m_length);
	stream << (const XBIN*)strText;

	bool bFind = false;
	if( m_th_flags & TH_FIN) {
		stream << _T("FIN");
		bFind = true;
	}
	if( m_th_flags & TH_SYN) {
		if( bFind )	stream << _T("|");
		stream << _T("SYN");
		bFind = true;
	}
	if( m_th_flags & TH_RST) {
		if( bFind )	stream << _T("|");
		stream << _T("RST");
		bFind = true;
	}
	if( m_th_flags & TH_PUSH) {
		if( bFind )	stream << _T("|");
		stream << _T("PUSH");
		bFind = true;
	}
	if( m_th_flags & TH_ACK) {
		if( bFind )	stream << _T("|");
		stream << _T("ACK");
		bFind = true;
	}
	if( m_th_flags & TH_URG) {
		if( bFind )	stream << _T("|");
		stream << _T("URG");
		bFind = true;
	}
	if( m_th_flags & 0x40) {
		if( bFind )	stream << _T("|");
		stream << _T("WHAT1");
		bFind = true;
	}
	if( m_th_flags & 0x80) {
		if( bFind )	stream << _T("|");
		stream << _T("WHAT2");
		bFind = true;
	}
	stream << _T(")") << pLF;





	if( bTCPIPheader == true ) {
		std::vector<XSecsPcapPacketHead*>::iterator iteHeader = m_pcket_headers.begin();
		for(; iteHeader != m_pcket_headers.end(); iteHeader++) {
			(*iteHeader)->print(stream, nDepth+1, pLF);
			break;
		}
	}

	if( bDump == true ) {
		if( m_length != 0 ) {
			XSecsPcapParser_PrintBlank(stream, nDepth+2);
			stream << _T("Data:") << pLF;
			XSecsPcapParser_PrintDump(stream, nDepth+1, pLF, m_pData, m_length);
		}
		return true;
	}

	if(m_type == XSECS_HSMS_TYPE_DATA) {
		// HSMS Header
		int i;
		XBIN* pSECSHeaderAddr = (XBIN*)&m_secsHeader;
		stream << _T("  header:    ");
		for(i = 0; i < 10; i++) {
			XStrBuff::printf(msec, XCOUNTOF(msec), _T("%02X"), pSECSHeaderAddr[i]);
			if( i != 0 && (i % 2) == 0 ) { stream << _T(" ");}
			if( i != 0 && (i % 8) == 0 ) { stream << _T(" ");}
			stream << msec;
		}
		pSECSHeaderAddr = (XBIN*)&m_secsDataLength;
		stream << _T("  /  length: ");
		for(i = 0; i < 4; i++) {
			XStrBuff::printf(msec, XCOUNTOF(msec), _T("%02X"), pSECSHeaderAddr[3-i]);
			if( i != 0 && (i % 2) == 0 ) { stream << _T(" ");}
			if( i != 0 && (i % 8) == 0 ) { stream << _T(" ");}
			stream << msec;
		}
		stream << pLF;

		if( m_bPrintHeaderOnly == false ) {
			// SECS-II 
			XSecsPcapParser_PrintBlank(stream, nDepth+2);
			XBIN iStream = m_secsHeader.byte2 & 0x80 ? m_secsHeader.byte2 - 0x80 : m_secsHeader.byte2;
			stream << _T("<S") << (int)iStream << _T("F") << (int)m_secsHeader.status;
			if( m_secsHeader.byte2 & 0x80 ) {
				stream << _T(" W");
			}
			XD_UINT2 usTicket;
			::memcpy(&usTicket, m_secsHeader.tr_id, sizeof(usTicket));
			stream << _T("  (T:") << ::ntohs(usTicket) << _T(")") << pLF;

			XDataReader reader(m_pData+14,  m_length-14 );
			reader.print(stream, nDepth+3, pLF);
			XSecsPcapParser_PrintBlank(stream, nDepth+2);
			stream << _T(">") << pLF;
		}
	} else
	if(m_type == XSECS_HSMS_TYPE_HSMS) {

		int i;
		XBIN* pSECSHeaderAddr = (XBIN*)&m_secsHeader;
		stream << _T("  header:    ");
		for(i = 0; i < 10; i++) {
			XStrBuff::printf(msec, XCOUNTOF(msec), _T("%02X"), pSECSHeaderAddr[i]);
			if( i != 0 && (i % 2) == 0 ) { stream << _T(" ");}
			if( i != 0 && (i % 8) == 0 ) { stream << _T(" ");}
			stream << msec;
		}
		pSECSHeaderAddr = (XBIN*)&m_secsDataLength;
		stream << _T("  /  length: ");
		for(i = 0; i < 4; i++) {
			XStrBuff::printf(msec, XCOUNTOF(msec), _T("%02X"), pSECSHeaderAddr[3-i]);
			if( i != 0 && (i % 2) == 0 ) { stream << _T(" ");}
			if( i != 0 && (i % 8) == 0 ) { stream << _T(" ");}
			stream << msec;
		}
		stream << pLF;

	} else {
		if( m_length != 0 && m_bPrintHeaderOnly == false ) {
			XSecsPcapParser_PrintBlank(stream, nDepth+2);
			stream << _T("Data:") << pLF;
			XSecsPcapParser_PrintDump(stream, nDepth+1, pLF, m_pData, m_length);
		}
	}
	if( m_bPrintHeaderOnly == false ) {
		stream << pLF;
	}
	return true;
XEXCEPT_EXIT
}
bool XSecsPcapData::print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/, bool bTCPIPheader /*= false*/)
{
XEXCEPT_ENTRY
	return innerPrint(stream, nDepth, pLF, bTCPIPheader, false);
XEXCEPT_EXIT
}
bool XSecsPcapData::printDump(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/, bool bTCPIPheader /*= false*/)
{
XEXCEPT_ENTRY
	return innerPrint(stream, nDepth, pLF, bTCPIPheader, true);
XEXCEPT_EXIT
}

bool XSecsPcapData::serializeStore(std::ostream &stream)
{
XEXCEPT_ENTRY
	XD_UINT4 ul4Data = 0;
	XD_UINT2 ul2Data = 0;
	XD_UINT1 ul1Data = 0;

	// 時間
	ul4Data = ::htonl(::mktime(&m_Time));
	stream.write((char*)&ul4Data, sizeof(ul4Data));
	ul4Data = ::htonl(m_ulTimeUSec);
	stream.write((char*)&ul4Data, sizeof(ul4Data));

	// IP&Port
	ul4Data = ::htonl(m_ip_src.S_un.S_addr);
	stream.write((char*)&ul4Data, sizeof(ul4Data));
	ul2Data = ::htons(m_port_src);
	stream.write((char*)&ul2Data, sizeof(ul2Data));

	ul4Data = ::htonl(m_ip_dst.S_un.S_addr);
	stream.write((char*)&ul4Data, sizeof(ul4Data));
	ul2Data = ::htons(m_port_dst);
	stream.write((char*)&ul2Data, sizeof(ul2Data));

	// Flags
	ul4Data = ::htonl(m_th_seq);
	stream.write((char*)&ul4Data, sizeof(ul4Data));
	ul4Data = ::htonl(m_th_ack);
	stream.write((char*)&ul4Data, sizeof(ul4Data));
	stream.write((char*)&m_th_flags, sizeof(m_th_flags));
	ul2Data = ::htons(m_th_win);
	stream.write((char*)&ul2Data, sizeof(ul2Data));
	ul2Data = ::htons(m_th_sum);
	stream.write((char*)&ul2Data, sizeof(ul2Data));
	ul2Data = ::htons(m_th_urp);
	stream.write((char*)&ul2Data, sizeof(ul2Data));

	// ヘッダー
	ul4Data = ::htonl(m_pcket_headers.size());
	stream.write((char*)&ul4Data, sizeof(ul4Data));
	std::vector<XSecsPcapPacketHead*>::iterator ite = m_pcket_headers.begin();
	for(; ite != m_pcket_headers.end(); ite++) {


		// IP Header, length
		ul4Data = ::htonl((*ite)->get_IPHeadLength());
		stream.write((char*)&ul4Data, sizeof(ul4Data));
		// TCP Header, length
		ul4Data = ::htonl((*ite)->get_TCPHeadLength());
		stream.write((char*)&ul4Data, sizeof(ul4Data));
		// Data length
		ul4Data = ::htonl((*ite)->get_DataLength());
		stream.write((char*)&ul4Data, sizeof(ul4Data));


		stream.write((const char*)(*ite)->get_EtherHead(), ETHER_HDRLEN);
		stream.write((const char*)(*ite)->get_IPHead(), (*ite)->get_IPHeadLength());
		stream.write((const char*)(*ite)->get_TCPHead(), (*ite)->get_TCPHeadLength());
	}

	// データタイプ
	ul1Data = (XD_UINT1)m_type;
	stream.write((char*)&ul1Data, sizeof(ul1Data));
	// データ
	ul4Data = ::htonl(m_length);
	stream.write((char*)&ul4Data, sizeof(ul4Data));
	stream.write((char*)m_pData, m_length);	


	return true;
XEXCEPT_EXIT
}

bool XSecsPcapData::serializeLoad(std::istream &stream, XBuff &buff)
{
XEXCEPT_ENTRY
	XD_UINT4 ul4Data = 0;
	XD_UINT2 ul2Data = 0;
	XD_UINT1 ul1Data = 0;

	// 時間
	stream.read((char*)&ul4Data, sizeof(ul4Data));	if( stream.gcount() != sizeof(ul4Data))	return false;
	ul4Data = ::ntohl(ul4Data);
	struct tm *p_tm = ::localtime((time_t*)&ul4Data);
	if( p_tm == NULL )	return false;
	::memcpy(&m_Time, p_tm, sizeof(struct tm));
	stream.read((char*)&ul4Data, sizeof(ul4Data));	if( stream.gcount() != sizeof(ul4Data))	return false;
	
	m_ulTimeUSec = ::ntohl(ul4Data);

	// IP&Port
	stream.read((char*)&ul4Data, sizeof(ul4Data));	if( stream.gcount() != sizeof(ul4Data))	return false;
	m_ip_src.S_un.S_addr = ::ntohl(ul4Data);
	stream.read((char*)&ul2Data, sizeof(ul2Data));	if( stream.gcount() != sizeof(ul2Data))	return false;
	m_port_src = ::ntohs(ul2Data);

	stream.read((char*)&ul4Data, sizeof(ul4Data));	if( stream.gcount() != sizeof(ul4Data))	return false;
	m_ip_dst.S_un.S_addr = ::ntohl(ul4Data);
	stream.read((char*)&ul2Data, sizeof(ul2Data));	if( stream.gcount() != sizeof(ul2Data))	return false;
	m_port_dst = ::ntohs(ul2Data);


	// Flags
	stream.read((char*)&ul4Data, sizeof(ul4Data));	if( stream.gcount() != sizeof(ul4Data))	return false;
	m_th_seq = ::ntohl(ul4Data);
	stream.read((char*)&ul4Data, sizeof(ul4Data));	if( stream.gcount() != sizeof(ul4Data))	return false;
	m_th_ack = ::ntohl(ul4Data);
	stream.read((char*)&m_th_flags, sizeof(m_th_flags));	if( stream.gcount() != sizeof(m_th_flags))	return false;
	stream.read((char*)&ul2Data, sizeof(ul2Data));	if( stream.gcount() != sizeof(ul2Data))	return false;
	m_th_win = ::ntohs(ul2Data);
	stream.read((char*)&ul2Data, sizeof(ul2Data));	if( stream.gcount() != sizeof(ul2Data))	return false;
	m_th_sum = ::ntohs(ul2Data);
	stream.read((char*)&ul2Data, sizeof(ul2Data));	if( stream.gcount() != sizeof(ul2Data))	return false;
	m_th_urp = ::ntohs(ul2Data);

	// ヘッダー
	XD_UINT4 hCount = 0;
	stream.read((char*)&hCount, sizeof(hCount));	if( stream.gcount() != sizeof(hCount))	return false;
	hCount = ::ntohl(hCount);
	for(ul4Data=0; ul4Data < hCount; ul4Data++) {
		XBIN szDefBuff[ETHER_HDRLEN + 20 + 20];
		XD_UINT4 ulIPLen, ulTCPLen, ulDataLen;


		stream.read((char*)&ulIPLen, sizeof(ulIPLen));	if( stream.gcount() != sizeof(ulIPLen))	return false;
		ulIPLen = ::ntohl(ulIPLen);
		stream.read((char*)&ulTCPLen, sizeof(ulTCPLen));	if( stream.gcount() != sizeof(ulTCPLen))	return false;
		ulTCPLen = ::ntohl(ulTCPLen);
		stream.read((char*)&ulDataLen, sizeof(ulDataLen));	if( stream.gcount() != sizeof(ulDataLen))	return false;
		ulDataLen = ::ntohl(ulDataLen);


		if( 20 + 20 == ulIPLen + ulTCPLen ) {
			stream.read((char*)&szDefBuff, sizeof(szDefBuff));	if( stream.gcount() != sizeof(szDefBuff))	return false;
		} else {
			std::auto_ptr<char> apBuff( new char[ulIPLen + ulTCPLen + ETHER_HDRLEN + 10] );
			stream.read((char*)apBuff.get(), sizeof(ulIPLen + ulTCPLen + ETHER_HDRLEN));	if( stream.gcount() != (ulIPLen + ulTCPLen + ETHER_HDRLEN))	return false;
		}
	}

	// データタイプ
	stream.read((char*)&ul1Data, sizeof(ul1Data));	if( stream.gcount() != sizeof(ul1Data))	return false;
	m_type = (XSECS_HSMS_TYPE)ul1Data;
	// データ
	stream.read((char*)&ul4Data, sizeof(ul4Data));	if( stream.gcount() != sizeof(ul4Data))	return false;
	m_length = ::ntohl(ul4Data);


	if( m_length > buff.GetBuffSize() ) {
		m_pData = (XBIN*)buff.Alloc(m_length+10);
	} else {
		m_pData = buff;
	}
	stream.read((char*)m_pData, m_length);	if( stream.gcount() != m_length)	return false;

	
	
	if( m_type == XSECS_HSMS_TYPE_DATA || m_type == XSECS_HSMS_TYPE_HSMS ) {
		if( m_length >= 14 ) {
			::memcpy(&m_secsDataLength, &m_pData[0], sizeof(m_secsDataLength));
			::memcpy(&m_secsHeader,     &m_pData[4], sizeof(m_secsHeader));
		}
	}
	return true;
XEXCEPT_EXIT
}

void XSecsPcapData::debugDump()
{
XEXCEPT_ENTRY
	XTRACE(_T("%04u/%02u/%02u %02u:%02u:%02u.%10u"), m_Time.tm_year + 1900, m_Time.tm_mon+1, m_Time.tm_mday,
			m_Time.tm_hour, m_Time.tm_min, m_Time.tm_sec, m_ulTimeUSec);
	XTRACE(_T(" [%d.%d.%d.%d(%d) - "), m_ip_src.S_un.S_un_b.s_b1,
		m_ip_src.S_un.S_un_b.s_b2,
		m_ip_src.S_un.S_un_b.s_b3,
		m_ip_src.S_un.S_un_b.s_b4,
		m_port_src);
	XTRACE(_T("%d.%d.%d.%d(%d) ]"), m_ip_dst.S_un.S_un_b.s_b1,
		m_ip_dst.S_un.S_un_b.s_b2,
		m_ip_dst.S_un.S_un_b.s_b3,
		m_ip_dst.S_un.S_un_b.s_b4,
		m_port_dst);

	XTRACE(_T("seq:%u:%u(%u) ack:%u win(%d)"), m_th_seq, m_th_seq+m_length, m_length, m_th_ack, this->m_th_win);
	bool bFind = false;
	if( m_th_flags & TH_FIN) {
		XTRACE(_T("FIN"));
		bFind = true;
	}
	if( m_th_flags & TH_SYN) {
		if( bFind )	XTRACE(_T(" |"));
		XTRACE(_T("SYN"));
		bFind = true;
	}
	if( m_th_flags & TH_RST) {
		if( bFind )	XTRACE(_T(" |"));
		XTRACE(_T("RST"));
		bFind = true;
	}
	if( m_th_flags & TH_PUSH) {
		if( bFind )	XTRACE(_T(" |"));
		XTRACE(_T("PUSH"));
		bFind = true;
	}
	if( m_th_flags & TH_ACK) {
		if( bFind )	XTRACE(_T(" |"));
		XTRACE(_T("ACK"));
		bFind = true;
	}
	if( m_th_flags & TH_URG) {
		if( bFind )	XTRACE(_T(" |"));
		XTRACE(_T("URG"));
		bFind = true;
	}
	if( m_th_flags & 0x40) {
		if( bFind )	XTRACE(_T(" |"));
		XTRACE(_T("WHAT1"));
		bFind = true;
	}
	if( m_th_flags & 0x80) {
		if( bFind )	XTRACE(_T(" |"));
		XTRACE(_T("WHAT2"));
		bFind = true;
	}


	XTRACE(_T("\n"));
XEXCEPT_EXIT
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSecsPcapPacketHead
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
XSecsPcapPacketHead::XSecsPcapPacketHead(XBIN* pEt, 
										 XBIN* pIP, 
										 unsigned long ulIPLen, 
										 XBIN* pTCP, 
										 unsigned long ulTCPLen,
										 unsigned long ulLen)
{
XEXCEPT_ENTRY
	unsigned long ulDataLen = ETHER_HDRLEN + ulIPLen + ulTCPLen;
	m_pBuff = new XBIN[ulDataLen + 10];
	if( m_pBuff == NULL ) {
		return ;
	}
	if( pEt != NULL ) {
		::memcpy(&m_pBuff[0], pEt, ETHER_HDRLEN);
	} else {
		::memset(&m_pBuff[0], 0, ETHER_HDRLEN);
	}
	::memcpy(&m_pBuff[ETHER_HDRLEN], pIP, ulIPLen);
	::memcpy(&m_pBuff[ETHER_HDRLEN + ulIPLen], pTCP, ulTCPLen);

	m_pEtherHead = &m_pBuff[0];
	m_pIPHead    = &m_pBuff[ETHER_HDRLEN];
	m_pTCPHead	 = &m_pBuff[ETHER_HDRLEN + ulIPLen];

	m_ulIPHeader  = ulIPLen;
	m_ulTCPHeader = ulTCPLen;

	m_ulLen		  = ulLen;
XEXCEPT_EXIT
}
XSecsPcapPacketHead::~XSecsPcapPacketHead()
{
XEXCEPT_ENTRY
	if( m_pBuff != NULL ) {
		delete m_pBuff;
	}
XEXCEPT_EXIT
}
bool XSecsPcapPacketHead::print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	unsigned long i;
	TCHAR szBuff10[10];


	XSecsPcapParser_PrintBlank(stream, nDepth+1);
	stream << _T("EtherHeader:  ");
	for(i = 0; i < ETHER_HDRLEN; i++) {
		XStrBuff::printf(szBuff10, XCOUNTOF(szBuff10), _T("%02X"), m_pEtherHead[i]);
		if( (i % 2) == 0 ) { stream << _T(" ");}
		stream << szBuff10;
	}
	stream << pLF;
	XSecsPcapParser_PrintBlank(stream, nDepth+1);
	stream << _T("IPHeader(") << m_ulIPHeader << "): ";
	for(i = 0; i < m_ulIPHeader; i++) {
		XStrBuff::printf(szBuff10, XCOUNTOF(szBuff10), _T("%02X"), m_pIPHead[i]);
		if( (i % 2) == 0 ) { stream << _T(" ");}
		stream << szBuff10;
	}
	stream << pLF;
	XSecsPcapParser_PrintBlank(stream, nDepth+1);
    stream << _T("TCPHeader(") << m_ulTCPHeader << _T("):");
	for(i = 0; i < m_ulTCPHeader; i++) {
		XStrBuff::printf(szBuff10, XCOUNTOF(szBuff10), _T("%02X"), m_pTCPHead[i]);
		if( (i % 2) == 0 ) { stream << _T(" ");}
		stream << szBuff10;
	}
	stream << pLF;


	return true;

	struct tcphdr* pTCP = (struct tcphdr*)m_pTCPHead;
	XSecsPcapParser_PrintBlank(stream, nDepth+2);
	stream << _T("seq:") << ::ntohl(pTCP->th_seq) << _T(" ") <<  
		::ntohl(pTCP->th_seq) + m_ulLen << _T("(") << m_ulLen << _T(")") << _T(" ack:") << ::ntohl(pTCP->th_ack) << 
		_T(" win:") << ::ntohl(pTCP->th_win) << _T(" len:") << m_ulLen << _T(" flg:");
	bool bFind = false;
	if( pTCP->th_flags & TH_FIN ) {
		stream << _T("FIN");
		bFind = true;
	}
	if( pTCP->th_flags & TH_SYN) {
		if( bFind )	stream << _T("|");
		stream << _T("SYN");
		bFind = true;
	}
	if( pTCP->th_flags & TH_RST) {
		if( bFind )	stream << _T("|");
		stream << _T("RST");
		bFind = true;
	}
	if( pTCP->th_flags & TH_PUSH) {
		if( bFind )	stream << _T("|");
		stream << _T("PUSH");
		bFind = true;
	}
	if( pTCP->th_flags & TH_ACK) {
		if( bFind )	stream << _T("|");
		stream << _T("ACK");
		bFind = true;
	}
	if( pTCP->th_flags & TH_URG) {
		if( bFind )	stream << _T("|");
		stream << _T("URG");
		bFind = true;
	}
	if( pTCP->th_flags & 0x40) {
		if( bFind )	stream << _T("|");
		stream << _T("WHAT1");
		bFind = true;
	}
	if( pTCP->th_flags & 0x80) {
		if( bFind )	stream << _T("|");
		stream << _T("WHAT2");
		bFind = true;
	}
	stream << pLF;


	if( m_ulTCPHeader > sizeof(tcphdr) ) {
		XSecsPcapParser_PrintBlank(stream, nDepth+2);
		stream << _T("TCP_Option: ");
		XBIN ch = ' ';

		unsigned long ulLen = m_ulTCPHeader - sizeof(tcphdr);
		XBIN *pTCPOpt = m_pTCPHead + sizeof(tcphdr);
		register int iTCPOpt, iTCPOptLen, iTCPOptDataLen;

		while (ulLen > 0) {
			stream << ch;
			iTCPOpt = *pTCPOpt++;
			if( iTCPOpt == 0 || iTCPOpt == 1) {
				iTCPOptLen = 1;
			} else {
				//TCHECK(*pTCPOpt);
				iTCPOptLen = *pTCPOpt++;
				if (iTCPOptLen < 2 || iTCPOptLen > ulLen) {
					stream << _T("Bad TCP Option");
					break;
				}
				--ulLen;
			}			
			ulLen--;
			iTCPOptDataLen = 0;

			switch( iTCPOpt ) {
			case 0:	// TCPOPT_EOL
				stream << _T("eol");
				break;
			case 1:	//	TCPOPT_NOP
				stream << _T("nop");
				break;
			case 2:	// TCPOPT_MAXSEG
				stream << _T("mss");
				iTCPOptDataLen = 2;
				{
					unsigned short sData = 0;
					::memcpy(&sData, pTCPOpt, iTCPOptDataLen);
					stream << _T(" ") << ::ntohs(sData);
				}
				break;
			case 3:	//	TCPOPT_WSCALE window scale factor (rfc1323)
				stream << _T("wscale");
				iTCPOptDataLen = 1;
				stream << _T(" ") << (XBIN)*pTCPOpt;
				break;
			case 4:	//	TCPOPT_SACKOK selective ack ok (rfc2018)
				stream << _T("sackOK");
				break;
			case 5:	//	TCPOPT_SACK selective ack (rfc2018)
				stream << _T("sack");
				iTCPOptDataLen = iTCPOptLen - 2;
				if( (iTCPOptDataLen % 8) != 0 ) {
					stream << _T("malformed sack");
				} else {
					stream << _T(" sack ") << iTCPOptDataLen / 8 << _T(" ");

					for (i = 0; i < iTCPOptDataLen; i += 8) {
						unsigned long ulS;
						unsigned long ulE;
						::memcpy(&ulS, &pTCPOpt[i + 0], 4);
						::memcpy(&ulE, &pTCPOpt[i + 4], 4);

						ulS = ::ntohl(ulS);
						ulE = ::ntohl(ulE);

						stream << _T("{") << ulS << _T(":") << ulE << _T("}");
					}
					stream << _T(" ");
				}
				break;
			case 6:	//	TCPOPT_ECHO echo (rfc1072)
				stream << _T("echo");
				iTCPOptDataLen = 4;
				{
					unsigned long ulData = 0;
					::memcpy(&ulData, pTCPOpt, iTCPOptDataLen);
					stream << _T(" ") << (unsigned long)::ntohl(ulData);
				}
				break;
			case 7:	//	TCPOPT_ECHOREPLY echo (rfc1072)
				stream << _T("echoreply");
				iTCPOptDataLen = 4;
				{
					unsigned long ulData = 0;
					::memcpy(&ulData, pTCPOpt, iTCPOptDataLen);
					stream << _T(" ") << (unsigned long)::ntohl(ulData);
				}
				break;
			case 8:	//	TCPOLEN_TIMESTAMP	timestamp (rfc1323)
				stream << _T("timestamp");
				iTCPOptDataLen = 8;
				{
					unsigned long ulData = 0;
					::memcpy(&ulData, pTCPOpt, iTCPOptDataLen);
					stream << _T(" ") << (unsigned long)::ntohl(ulData);
					::memcpy(&ulData, pTCPOpt+4, iTCPOptDataLen);
					stream << _T(" ") << (unsigned long)::ntohl(ulData);
				}
				break;

			case 11:	//	TCPOPT_CC	T/TCP CC options (rfc1644)
				stream << _T("cc");
				iTCPOptDataLen = 4;
				{
					unsigned long ulData = 0;
					::memcpy(&ulData, pTCPOpt, iTCPOptDataLen);
					stream << _T(" ") << (unsigned long)::ntohl(ulData);
				}
				break;
			case 12:	//	TCPOPT_CCNEW	T/TCP CC options (rfc1644)
				stream << _T("ccnew");
				iTCPOptDataLen = 4;
				{
					unsigned long ulData = 0;
					::memcpy(&ulData, pTCPOpt, iTCPOptDataLen);
					stream << _T(" ") << (unsigned long)::ntohl(ulData);
				}
				break;
			case 13:	//	TCPOPT_CCECHO	T/TCP CC options (rfc1644)
				stream << _T("ccecho");
				iTCPOptDataLen = 4;
				{
					unsigned long ulData = 0;
					::memcpy(&ulData, pTCPOpt, iTCPOptDataLen);
					stream << _T(" ") << (unsigned long)::ntohl(ulData);
				}
				break;
			default:
				stream << _T("opt-") << iTCPOpt << _T(":");
				iTCPOptDataLen = iTCPOptLen - 2;
				for (i = 0; i < iTCPOptDataLen; i++) {
					stream << std::hex << (XBIN)pTCPOpt[i];
				}
				break;
			}
			pTCPOpt += iTCPOptDataLen;
			ulLen -= iTCPOptDataLen;

			++iTCPOptDataLen;
			if( !(iTCPOpt == 0 || iTCPOpt == 1) ) { // !( TCPOPT_EOL || TCPOPT_NOP)
				++iTCPOptDataLen;
			}
			if( iTCPOptDataLen != iTCPOptLen ) {
				stream << _T("[len ") << iTCPOptLen << _T("]");
			}

			ch = ',';
			if( iTCPOpt == 0 ) {	// TCPOPT_EOL
				break;
			}
		}

		stream << pLF;
	}


	return true;
XEXCEPT_EXIT
}
XBIN* XSecsPcapPacketHead::get_EtherHead()
{
XEXCEPT_ENTRY
	return m_pEtherHead;
XEXCEPT_EXIT
}
XBIN* XSecsPcapPacketHead::get_IPHead()
{
XEXCEPT_ENTRY
	return m_pIPHead;
XEXCEPT_EXIT
}
XBIN* XSecsPcapPacketHead::get_TCPHead()
{
XEXCEPT_ENTRY
	return m_pTCPHead;
XEXCEPT_EXIT
}
unsigned long XSecsPcapPacketHead::get_IPHeadLength()
{
XEXCEPT_ENTRY
	return m_ulIPHeader;
XEXCEPT_EXIT
}
unsigned long XSecsPcapPacketHead::get_TCPHeadLength()
{
XEXCEPT_ENTRY
	return m_ulTCPHeader;
XEXCEPT_EXIT
}
unsigned long XSecsPcapPacketHead::get_DataLength()
{
XEXCEPT_ENTRY
	return m_ulLen;
XEXCEPT_EXIT
}






//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSecsPcapParser interface
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
XSecsPcapParser::XSecsPcapParser()
	:	m_pCallBackFunction(NULL)
	,	m_pUserCaallBackData(0)
	,	m_pRawCallBack( NULL )
	,	m_pLinveThread( NULL )
	,	m_deltaTime( 0 )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSecsPcapParser::~XSecsPcapParser()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
void XSecsPcapParser::setCallBackFunction(
									void (*pCallBackFunction)(XSecsPcapData*,void*)
								,	void *pUserCaallBackData
								,	void (*pRawCallBack)(XSecsPcapData* , void *, void *, unsigned int ,void* , unsigned int ) /*= NULL*/)
{
XEXCEPT_ENTRY
	m_pCallBackFunction  = pCallBackFunction;
	m_pUserCaallBackData = pUserCaallBackData;
	m_pRawCallBack       = pRawCallBack;
XEXCEPT_EXIT
}
XSecsPcapParser* XSecsPcapParser::newInstance()
{
XEXCEPT_ENTRY
	XSecsPcapParser *pnewInstance = new XSecsPcapParserImple();
	return pnewInstance;
XEXCEPT_EXIT
}
XSecsPcapParser* XSecsPcapParser::newInstanceNICap()
{
XEXCEPT_ENTRY
	XSecsPcapParser *pnewInstance = new XSecsNICapParserImple();
	return pnewInstance;
XEXCEPT_EXIT
}
void XSecsPcapParser::deleteInstance(XSecsPcapParser** ppParser)
{
XEXCEPT_ENTRY
	if(*ppParser) {
		delete *ppParser;
		*ppParser = NULL;
	}
XEXCEPT_EXIT
}


void XSecsPcapParser::inner(XSecsPcapData& pcapData, 
							void *pEtherHeader, 
							void *pIP, unsigned int hlen_ip,
							void* pTCP, unsigned int hlen_tcp)
{
XEXCEPT_ENTRY
	unsigned long			ulSaveLen = 0;
	bool bNewSequence = false;
	register const u_char	*pWk;
	u_int					length = 0;
#ifdef _DEBUG
	static char szMySeqLog[512];
#endif

	if( m_pRawCallBack != NULL ) {
		// 生データ解析
		m_pRawCallBack(&pcapData, pEtherHeader, pIP, hlen_ip, pTCP, hlen_tcp);
	}

	length = ulSaveLen = pcapData.m_length;
	pWk = pcapData.m_pData;

	packetSeq* pPacket = findPacketSeq( &pcapData );
	if( pPacket == NULL ) {
		// 新しいシーケンスが発生
		bNewSequence = true;
		pPacket = new packetSeq(pcapData);
		if( pPacket == NULL ) {
			return ;
		}
		m_packets.push_back( pPacket );
	}
	// 通信の向きが逆のシーケンスアイテムを見つける
	packetSeq* pPacketDst = findDstPacketSeq( &pcapData );


	if( pcapData.m_th_flags == TH_SYN && pPacket != NULL) {
		// 前回の切断のPORT/IPのペアと同一である場合、同じシーケンスアイテムが取得できるが、
		// シーケンスとしては、異なるためリセットする。
		{
			std::vector<packetSeq*>::iterator ite = m_packets.begin();
			for(; ite != m_packets.end(); ite++) {
				packetSeq* p = (*ite);
				if( pPacket == p ) {
					m_packets.erase( ite );
					delete p;
					break;
				}
			}
			bNewSequence = true;
			pPacket = new packetSeq(pcapData);
			if( pPacket == NULL ) {
				return ;
			}
			m_packets.push_back( pPacket );
		}

		if( pPacketDst != NULL ) {
			std::vector<packetSeq*>::iterator ite = m_packets.begin();
			for(; ite != m_packets.end(); ite++) {
				packetSeq* p = (*ite);
				if( pPacketDst == p ) {
					m_packets.erase( ite );
					delete p;
					break;
				}
			}
		}
	}


	XBIN cSequenceStatus = 0;
	if( bNewSequence == false ) {


		//
		// 連続したシーケンスで、順序正しいシーケンスであるか確認。
		//
		XD_UINT4 lenInflate = ( pPacket->m_comp.m_th_flags & TH_SYN || 
								pPacket->m_comp.m_th_flags & TH_FIN ) ? 1 : 0;

		if( pcapData.m_th_seq <= pPacket->m_comp.m_th_seq && pPacket->m_comp.m_lengthPrev != 0 ) {
			// 前のシーケンス番号がの方が大きい場合、
			if( pcapData.m_th_seq == pPacket->m_comp.m_th_seq ) {
				int diffLen = pcapData.m_length - pPacket->m_comp.m_lengthPrev;
#ifdef _DEBUG
				sprintf(szMySeqLog, "MySEQ,Prev Equal...,,,PoolCnt:%u,%u,diff:%u,oldack:%u,0x%X", 
					pPacket->m_sacksDataItems.size(),
					0,
					diffLen,
					pPacket->m_comp.m_th_ack,
					0
					);
				std::cout << szMySeqLog << std::endl;
#endif
				if( diffLen == 0 ) {
					// 同じパケット
					
					return ;
				}
#ifdef _DEBUG
				if( diffLen > 0 ) {	std::cout << "MySEQ,inflate" << std::endl;	} 
				else {				std::cout << "MySEQ,deflate" << std::endl;	}
#endif

				pPacket->m_comp.m_th_seq = pcapData.m_th_seq;
				pPacket->m_comp.m_lengthPrev = pcapData.m_length;
				pPacket->m_comp.m_th_ack = pcapData.m_th_ack;
			} else {
#ifdef _DEBUG
				sprintf(szMySeqLog, "MySEQ,Prev Less than...,,,PoolCnt:%u,%u,%u,OLDACK:%u,0x%X", 
					pPacket->m_sacksDataItems.size(),
					0,
					0,
					pPacket->m_comp.m_th_ack,
					0
					);
				std::cout << szMySeqLog << std::endl;
#endif
			}

		}  
		if( (pPacket->m_comp.m_th_seq + pPacket->m_comp.m_lengthPrev + lenInflate) != (pcapData.m_th_seq) ) {
			// シーケンス順序が正しくない
			cSequenceStatus = 0xFE;		
			
			// パケットの再送 ? か確認。			
			if( pPacket->m_comp.m_th_seq == pcapData.m_th_seq 
						&&
				pPacket->m_comp.m_th_ack == pcapData.m_th_ack ) {

				if( pcapData.m_length > pPacket->m_comp.m_lengthPrev ) {
					// 前のパケットよりデータが大きい場合
					XBIN* pPrevBuff = pPacket->m_buffer.getBuff();
					pPrevBuff += pPacket->m_buffer.size() - pPacket->m_comp.m_lengthPrev;

					if( ::memcmp(pPrevBuff, pcapData.m_pData, pPacket->m_comp.m_lengthPrev) == 0 ) {
						cSequenceStatus = 0x0E;
						pcapData.m_pData		= (XBIN*)pWk+pPacket->m_comp.m_lengthPrev;
						pcapData.m_length       = pcapData.m_length - pPacket->m_comp.m_lengthPrev;
						
						pPacket->m_comp.m_lengthPrev = length;
						length = pcapData.m_length;
						pWk = pcapData.m_pData;
					}
				} else {
					// 前のパケットよりデータが小さい場合

					// 再送パケットは破棄する

					cSequenceStatus = 0x0E;	

				}
			}
		}

		if( cSequenceStatus == 0xFE ) {

//	XTRACE(_T("    Cur:"));	pcapData.debugDump();
//	XTRACE(_T("    Pre:"));	pPacket->m_comp.debugDump();
//	XTRACE(_T("    Dst:"));	pPacketDst->m_comp.debugDump();
#ifdef _DEBUG
			sprintf(szMySeqLog, "MySEQ,May be SACK_DATA,,,PoolCnt:%u,%u,%u,%u,0x%X", 
					pPacket->m_sacksDataItems.size(),
					0,
					0,
					pPacket->m_comp.m_th_ack,
					0
					);
				std::cout << szMySeqLog << std::endl;
#endif
				

			if( pPacket->m_comp.m_th_ack == pcapData.m_th_ack ) {

				// シーケンスが正しくない場合、SACKによるパケットの再送があるかも知れないので、貯めておく。
				packetSeq* pSack = new packetSeq(pcapData);
				pPacket->m_sacksDataItems.push_back( pSack );
				if( pSack->newSetBuff(pWk, length) == NULL ) {
					XTRACE(_T("  ERR : 追加\n"));
				}
#ifdef _DEBUG
				sprintf(szMySeqLog, "MySEQ,SACK_DATA,,,PoolCnt:%u,%u,%u,%u,0x%X", 
					pPacket->m_sacksDataItems.size(),
					0,
					0,
					pPacket->m_comp.m_th_ack,
					0
					);
				std::cout << szMySeqLog << std::endl;
#endif
				// パケットヘッダ情報の追加
				pSack->addHeader(
					(XBIN*)pEtherHeader, 
					(XBIN*)pIP, hlen_ip,
					(XBIN*)pTCP, hlen_tcp,
					length);
				return ;
			} else {

				// 最後のACKと連続しているか？
				{
					std::vector<packetSeq*>::iterator iteSACK = pPacket->m_sacksDataItems.end();
					if( iteSACK != pPacket->m_sacksDataItems.begin() ) {
						iteSACK--;

						if( (*iteSACK)->m_comp.m_th_seq + (*iteSACK)->m_comp.m_length == pcapData.m_th_seq ) {
							// シーケンス番号をデコレートしておく
							pPacket->m_comp.m_th_seq     = (*iteSACK)->m_comp.m_th_seq;
							pPacket->m_comp.m_th_ack     = (*iteSACK)->m_comp.m_th_ack;
							pPacket->m_comp.m_lengthPrev = (*iteSACK)->m_comp.m_length;	
							cSequenceStatus = 0;
						}
					}
				}

				if( cSequenceStatus != 0 ) {
#ifdef _DEBUG
					sprintf(szMySeqLog, "MySEQ,!!ERR_Invalid_SEQ!!,,,PoolCnt:%u,seq:%u,len:%u,ack%u,0x%X", 
						pPacket->m_sacksDataItems.size(),
						pPacket->m_comp.m_th_seq,
						pPacket->m_comp.m_lengthPrev,
						pPacket->m_comp.m_th_ack,
						0
						);
					std::cout << szMySeqLog << std::endl;
#endif

					// SACKデータを破棄する。
					if( pPacket->m_sacksDataItems.size() ) {
						// SACKを吐き出す。
						std::vector<packetSeq*>::iterator iteSACK = pPacket->m_sacksDataItems.begin();
						for(; iteSACK != pPacket->m_sacksDataItems.end(); iteSACK++) {
							m_pCallBackFunction(&(*iteSACK)->m_comp, m_pUserCaallBackData);
#ifdef _DEBUG
							std::cout << "MySEQ, SACK Destory:" << std::endl;
#endif
							delete *iteSACK;
						}
						pPacket->m_sacksDataItems.clear();
					}
					
					
					
					
					packetSeq* pSack = new packetSeq(pcapData);
					pPacket->m_sacksDataItems.push_back( pSack );
					if( pSack->newSetBuff(pWk, length) == NULL ) {
						XTRACE(_T("  ERR : 追加\n"));
					}
					// パケットヘッダ情報の追加
					pSack->addHeader(
						(XBIN*)pEtherHeader, 
						(XBIN*)pIP, hlen_ip,
						(XBIN*)pTCP, hlen_tcp,
						length);
					return ;
				} else {
#ifdef _DEBUG
					sprintf(szMySeqLog, "MySEQ,Continue SACK_DATA,,,PoolCnt:%u,seq:%u,len:%u,ack%u,0x%X", 
						pPacket->m_sacksDataItems.size(),
						pPacket->m_comp.m_th_seq,
						pPacket->m_comp.m_lengthPrev,
						pPacket->m_comp.m_th_ack,
						0
						);
					std::cout << szMySeqLog << std::endl;

					packetSeq* pSack = new packetSeq(pcapData);
					pPacket->m_sacksDataItems.push_back( pSack );
					if( pSack->newSetBuff(pWk, length) == NULL ) {
						XTRACE(_T("  ERR : 追加\n"));
					}
					// パケットヘッダ情報の追加
					pSack->addHeader(
						(XBIN*)pEtherHeader, 
						(XBIN*)pIP, hlen_ip,
						(XBIN*)pTCP, hlen_tcp,
						length);
					return ;
#endif
				}

				return ;
			}
#ifdef _DEBUG
					sprintf(szMySeqLog, "MySEQ,WHAT SACK_DATA,,,PoolCnt:%u,seq:%u,len:%u,ack%u,0x%X", 
						pPacket->m_sacksDataItems.size(),
						pPacket->m_comp.m_th_seq,
						pPacket->m_comp.m_lengthPrev,
						pPacket->m_comp.m_th_ack,
						0
						);
					std::cout << szMySeqLog << std::endl;
#endif
			return ;
		}
	}



	// シーケンスをチェックする
	if (pcapData.m_th_flags & TH_ACK && !(pcapData.m_th_flags & TH_RST)) {
		// ACK送信の場合。
		if( pPacketDst ) {
			// ACK送信時、TCPオプションにSACK値がある場合、DSTに(pPacketDst)貯めたSACKアイテム、
			// にマークをつける
			unsigned long *pSacksValue = NULL;
			unsigned long ulSacksCount = 0;
			// TCP optionにSACK値があるか調べる。
			if( foundSackValues((XBIN*)pTCP, hlen_tcp, &pSacksValue, &ulSacksCount) == true ) {
#ifdef _DEBUG
				sprintf(szMySeqLog, "MySEQ,SACK opt found...,,,PoolCnt:%u,%u,%u,OLDACK:%u,0x%X", 
					pPacket->m_sacksDataItems.size(),
					0,
					0,
					ulSacksCount,
					0
					);
				std::cout << szMySeqLog << std::endl;
#endif

				for(unsigned long i = 0; i < ulSacksCount; i+=2) {
					unsigned long s = ::ntohl(pSacksValue[i]);
					unsigned long e = ::ntohl(pSacksValue[i+1]);
					XBIN valid = 0;
//					XTRACE(_T(" sack_P{%u:%u}\n"), s, e);
					// 同じレンジのアイテムを見つける。
					std::vector<packetSeq*>::iterator iteSackItem = pPacketDst->m_sacksDataItems.begin();
					for(; iteSackItem != pPacketDst->m_sacksDataItems.end(); iteSackItem++) {
						if( s <= (*iteSackItem)->m_comp.m_th_seq && 
							(*iteSackItem)->m_comp.m_th_seq + (*iteSackItem)->m_comp.m_length <= e ) {

							if( s == (*iteSackItem)->m_comp.m_th_seq)  {
								valid |= 0x01;
							}
							if( (*iteSackItem)->m_comp.m_th_seq + (*iteSackItem)->m_comp.m_length == e)  {
								valid |= 0x02;
							}
							// SACK要求された送信者のSACKアイテムの参照値をインクリメントする
							(*iteSackItem)->m_ulSackRequireCounter ++ ;
						}
					}

					if( valid != 0x03 ) {
						XTRACE(_T("value of 'sack' not received was required. sack{%u:%u}"), s, e);
					} else {

						// マルチパケット中の、中間ACK

						packetSeq* pAck = new packetSeq(pcapData);
						pPacketDst->m_acksDataItems.push_back( pAck );
						if( pAck->newSetBuff(pWk, length) == NULL ) {
							XTRACE(_T("  ERR : 追加\n"));
						}
						// パケットヘッダ情報の追加
						pAck->addHeader(
							(XBIN*)pEtherHeader, 
							(XBIN*)pIP, hlen_ip,
							(XBIN*)pTCP, hlen_tcp,
							length);

						pPacket->m_comp = pcapData;
						return ;
					}
				}
			}


			// DSTのパケットを受信した。
			XD_UINT4 ack = pcapData.m_th_ack;
			if( pPacketDst->m_comp.m_th_flags  & (TH_SYN | TH_FIN) ) {
				// SYN | FIN に対するACKの場合は、m_th_ack + 1となっているので、
				// DSTのSEQ-1となるべき。
				ack--;
			}
			if( pPacketDst->m_comp.m_th_seq + pPacketDst->m_comp.m_lengthPrev == ack ) {
				// 送信者が送って来たSEQ＋データ長がACKの値になっていることが基本。
				cSequenceStatus = 3;
			} else {
				bool bFound = false;
				if( pPacketDst->m_comp.m_pcket_headers.size() != 0 ) {
					// マルチパケット中の中間ACK
					std::vector<XSecsPcapPacketHead*>::iterator iteHead = pPacketDst->m_comp.m_pcket_headers.begin();
					for(; iteHead != pPacketDst->m_comp.m_pcket_headers.end(); iteHead++) {
						XSecsPcapPacketHead* pHead = *iteHead;
						tcphdr* pTCPHeader = (tcphdr*)pHead->get_TCPHead();
						if( ::ntohl(pTCPHeader->th_seq) + pHead->get_DataLength() == ack ) {
							bFound = true;
						}
					}
				}
				if( bFound == false && pPacketDst->m_sacksDataItems.size() != 0 ) {
					std::vector<packetSeq*>::iterator iteSackItem = pPacketDst->m_sacksDataItems.end();
					iteSackItem--;
					if( iteSackItem != pPacketDst->m_sacksDataItems.begin() ) {
						ack = pcapData.m_th_ack;
						if( (*iteSackItem)->m_comp.m_th_flags  & (TH_SYN | TH_FIN) ) {
							ack--;
						}
						if( (*iteSackItem)->m_comp.m_th_seq + (*iteSackItem)->m_comp.m_length == ack ) {
							bFound = true;
						}
					}
				}
				if( bFound == false ) {
					XTRACE(_T("  ERR : INVALID ACK %d\n"), pcapData.m_th_ack);
					cSequenceStatus = 0xFF;
//					XTRACE(_T("    Cur:"));	pcapData.debugDump();
//					XTRACE(_T("    Pre:"));	pPacket->m_comp.debugDump();
//					XTRACE(_T("    Dst:"));	pPacketDst->m_comp.debugDump();
				}
			}


			if( pPacketDst->m_status == PACKET_STATUS_NEED_CONTINUE 
				// マルチパケット中の、中間ACK
					&&
				!(pcapData.m_th_flags == TH_PUSH)) {
				// ３ハンドシェークで、PUSHがある場合は除く


				packetSeq* pAck = new packetSeq(pcapData);
				pPacketDst->m_acksDataItems.push_back( pAck );
				if( pAck->newSetBuff(pWk, length) == NULL ) {
					XTRACE(_T("  ERR : 追加\n"));
				}
				// パケットヘッダ情報の追加
				pAck->addHeader(
					(XBIN*)pEtherHeader, 
					(XBIN*)pIP, hlen_ip,
					(XBIN*)pTCP, hlen_tcp,
					length);


				pPacket->m_comp = pcapData;
				return ;
			}
		}
	}

	
	pPacket->m_comp = pcapData;
	if( pPacket->m_status == PACKET_STATUS_NEED_CONTINUE ) {
		// 以前のデータの続きの場合は、追記モード
		pPacket->m_comp.m_pData = pPacket->appendBuff(pWk, length);
	} else {
		// 新しいデータの開始。
		pPacket->m_comp.m_pData = pPacket->newSetBuff(pWk, length);
		_XASSERT(::memcmp(pcapData.m_pData, pWk, length) == 0);
		_XASSERT(pPacket->size() == length);

		// パケットヘッダ情報を削除
		pPacket->clearHeaders();
	}
	// パケットヘッダ情報の追加
	pPacket->addHeader((XBIN*)pEtherHeader, 
		(XBIN*)pIP, hlen_ip,
		(XBIN*)pTCP, hlen_tcp,
		length);

	// 前パケットのデータサイズとして保存
	pPacket->m_comp.m_lengthPrev = ulSaveLen;


	if( cSequenceStatus == 3 ) {
		if( pPacket->m_sacksDataItems.size() != 0 ) {
			// SACKされている場合、パケットを組み立てる
			packetSeq* pSack = NULL;
			unsigned long ulFindSeq = 0;
			unsigned long ulFindLength = 0;

			ulFindSeq = pPacket->m_comp.m_th_seq;
			ulFindLength = length;
			while(1) {
//				XTRACE(" -- %d\n", ulFindSeq + ulFindLength);
				pSack = pPacket->findSack(ulFindSeq + ulFindLength);
				if( pSack ) {
					if( pSack->m_ulSackRequireCounter == 0 ) {
						// 要求されたアイテムは、ACKのTCPオプションのSACKのデータとして要求されていない
						pPacket->m_sacksDataItems.push_back(pSack);
						break;
					}

					/*
                         = がulDistanceの値。
						pPackt  pSack
						*|
						 |---------|*
						 |         |
						*|         |
						 ==========|*

						 プラスの値の時は、pSackに残りの値を持つ

  						pPackt  pSack
						*|
						 |---------|*
						 |         |
						 |=========|*
						*|
						 マイナスの時は、pSackは意味を持たない。
					*/
					long lDistance = 
						(ulFindSeq + ulFindLength )
									-
						(pSack->m_comp.m_th_seq);
					if( 0 <= lDistance && lDistance <= pSack->m_comp.m_length ) {
						XBIN *pBuffDistance = pSack->m_comp.m_pData;
						pBuffDistance += (lDistance);
						if( pPacket->appendBuff(pBuffDistance, pSack->m_comp.m_length-lDistance) == NULL ) {
							XTRACE(_T("Append Error\n"));
						}
					}
					ulFindSeq    = pSack->m_comp.m_th_seq;
					ulFindLength = pSack->m_comp.m_length;

					// ヘッダー情報を足す
					XSecsPcapPacketHead* pSackHeader = pSack->getTopHead();
					if( pSackHeader != NULL ) {
						pPacket->addHeader(pSackHeader->get_EtherHead(),
							pSackHeader->get_IPHead(), pSackHeader->get_IPHeadLength(),
							pSackHeader->get_TCPHead(), pSackHeader->get_TCPHeadLength(),
							pSackHeader->get_DataLength());
					}

					// シーケンス番号をデコレートしておく
					pPacket->m_comp.m_th_seq = ulFindSeq;
					pPacket->m_comp.m_th_ack = pSack->m_comp.m_th_ack;
					pPacket->m_comp.m_lengthPrev = ulFindLength;

					delete pSack;
				} else {
					break;
				}
			}
		}
	}



	PACKET_STATUS sts;
	while(1) {
		//
		// パケットがSECSデータか判定します
		//
		pcapData.m_pData  = pPacket->m_comp.m_pData;
		pcapData.m_length = pPacket->m_comp.m_length;
		sts = judge(pcapData, pPacket);
		if( sts != PACKET_STATUS_NEED_CONTINUE && m_pCallBackFunction ) {
			if( sts == PACKET_STATUS_INIT ) {

				if( pPacket->m_sacksDataItems.size() ) {
					// SACKを吐き出す。
					std::vector<packetSeq*>::iterator iteSACK = pPacket->m_sacksDataItems.begin();
					for(; iteSACK != pPacket->m_sacksDataItems.end(); iteSACK++) {
#ifdef _DEBUG
						std::cout << "MySEQ, SACK Build:" << std::endl;
//						(*iteSACK)->m_comp.debugDump();
#endif
						pPacket->appendBuff((*iteSACK)->m_comp.m_pData, (*iteSACK)->m_comp.m_length  );
						delete *iteSACK;
					}
					pPacket->m_sacksDataItems.clear();
					continue;
				}
			}

			// ヘッダーのアドレスをコピー
			pcapData.m_pcket_headers.clear();
			pcapData.m_pcket_headers = pPacket->m_comp.m_pcket_headers;

			// ユーザー定義コールバック関数を呼び出す
			m_pCallBackFunction(&pcapData, m_pUserCaallBackData);

			pcapData.m_pcket_headers.clear();

			if( sts == PACKET_STATUS_INIT ) {
				if( pPacket->m_acksDataItems.size() ) {
					// マルチパケットの中間ACKを貯めていた場合は、ACKを吐き出す。
					bool bFirst = true;
					std::vector<packetSeq*>::iterator iteACK = pPacket->m_acksDataItems.begin();
					for(; iteACK != pPacket->m_acksDataItems.end(); iteACK++) {
#ifdef _DEBUG
						std::cout << "MySEQ, MPack Destory:" << std::endl;
#endif
//						m_pCallBackFunction(&(*iteACK)->m_comp, m_pUserCaallBackData);
//						delete *iteACK;

						if( bFirst == true ) {
							XSecsPcapData& ackPCAPData = (*iteACK)->m_comp;
							pcapData = ackPCAPData;
							pPacket->m_comp.m_pData = pPacket->newSetBuff((*iteACK)->m_comp.m_pData, (*iteACK)->m_comp.m_length);
						} else {
							pPacket->appendBuff((*iteACK)->m_comp.m_pData, (*iteACK)->m_comp.m_length  );
						}
						delete *iteACK;
						bFirst = false;
					}
					pPacket->m_acksDataItems.clear();

					continue;
				}
			}
		}
		if( sts != PACKET_STATUS_MULTI_SECS_MSG ) {
			break;
		}

		// バッファを前に詰める
		pcapData.m_pData  = pPacket->moveOverBeforeBuff( pcapData.m_length );
	}
	return;
XEXCEPT_EXIT
}



XSecsPcapParser::PACKET_STATUS XSecsPcapParser::judge(XSecsPcapData &pcapData, packetSeq* pPack)
{
XEXCEPT_ENTRY
	u_char     *pWk;
	XD_UINT4	ulSECSData = 0;


	if( pPack->m_comp.m_length == 0 ) {
		pcapData.m_type = XSECS_HSMS_TYPE_UNKNOW;
		return pPack->m_status;
	}
	if( pPack->m_comp.m_length < 14 ) {
		// データの判定不可能なので、スキップする
		pcapData.m_type = XSECS_HSMS_TYPE_UNKNOW;
		pPack->m_status = PACKET_STATUS_NEED_CONTINUE;
		return pPack->m_status;
	}

	//------------------------------------------------------------------------
	//  ####  HSMS Message ####
	//  4		Secs Len（ヘッダー＋データ）
	//  0-10	ヘッダー情報
	//	0-n		SECSデータ部
	//------------------------------------------------------------------------
	
	// SECSのデータ長さを取得
	pWk = pPack->m_comp.m_pData;
	::memcpy(&pcapData.m_secsDataLength, pWk, sizeof(ulSECSData));
	pcapData.m_secsDataLength = ::ntohl(pcapData.m_secsDataLength);
	pWk += 4;



	// Set Header
	//------------------------------------------------------------------------
	//  #### ヘッダー情報 ####
	//	0-1		セッションID
	//	2		ヘッダーバイト２
	//	3		ヘッダーバイト３ (Pタイプ =0の時、SECSファンクション、Pタイプ !=0の時,)
	//	4		Pタイプ 
	//	5		Sタイプ
	//	6-9		システムバイト
	//------------------------------------------------------------------------
	::memcpy(&pcapData.m_secsHeader, pWk, sizeof(pcapData.m_secsHeader));
	if( ( pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_SELECT_REQ ||
		  pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_SELECT_RSP ||
		  pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_DELETE_REQ ||
		  pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_DELETE_RSP ||
		  pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_LINK_REQ ||
		  pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_LINK_RSP ||
		  pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_REJECT_REQ ||
		  pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_SEPARATE_REQ) &&
		  pcapData.m_secsHeader.ptype == 0 ) {
		
		pcapData.m_type = XSECS_HSMS_TYPE_HSMS;

		if(pcapData.m_secsDataLength < pPack->m_comp.m_length - 4 ) {
			// 1パケットに複数のSECSデータがある場合アリ！！
			long nDiffSize = (pPack->m_comp.m_length - 4) - pcapData.m_secsDataLength;
			pWk += pcapData.m_secsDataLength;

			// バッファを前に詰める
//			pcapData.m_pData = pPack->moveOverBeforeBuff( pcapData.m_secsDataLength + 4 );
			pcapData.m_length = pcapData.m_secsDataLength + 4;

			// 続けて解析する
			pPack->m_status = PACKET_STATUS_MULTI_SECS_MSG;
		} else {
			pPack->m_status = PACKET_STATUS_INIT;
		}
	} else 
	if( pcapData.m_secsHeader.stype == (XBIN)XSECS_STYPE_DATA && 
		pcapData.m_secsHeader.ptype == (XBIN)0 ) {
		
		if(pcapData.m_secsDataLength <= pPack->m_comp.m_length - 4 ) {
			// SECS データメッセージとして、データを揃っている
			XDataReader reader(pWk+sizeof(pcapData.m_secsHeader), pcapData.m_secsDataLength-sizeof(pcapData.m_secsHeader));
			if( 0 == reader.test() ) {
				pcapData.m_type = XSECS_HSMS_TYPE_DATA;

				if(pcapData.m_secsDataLength < pPack->m_comp.m_length - 4 ) {
					// 1パケットに複数のSECSデータがある場合アリ！！
					long nDiffSize = (pPack->m_comp.m_length - 4) - pcapData.m_secsDataLength;
					pWk += pcapData.m_secsDataLength;

					// バッファを前に詰める
//					pcapData.m_pData  = pPack->moveOverBeforeBuff( pcapData.m_secsDataLength + 4 );
					pcapData.m_length = pcapData.m_secsDataLength + 4;

					// 続けて解析する
					pPack->m_status = PACKET_STATUS_MULTI_SECS_MSG;
				} else {
					pPack->m_status = PACKET_STATUS_INIT;
				}
			} else {
				pcapData.m_type = XSECS_HSMS_TYPE_UNKNOW;
				pPack->m_status = PACKET_STATUS_INIT;
			}		
		} else {
			pcapData.m_type = XSECS_HSMS_TYPE_UNKNOW;
			// SECSデータに足りていない場合は、途中まで、SECSデータであるか確認する
			XDataReader reader(pWk+sizeof(pcapData.m_secsHeader), pPack->m_comp.m_length - 4-sizeof(pcapData.m_secsHeader));
			if( XD2_ERRSS != reader.test() ) {
				// SECS データではない
				pPack->m_status = PACKET_STATUS_INIT;
			} else {
				// データ部のSECSデータ長が足りていない場合は、次パケットにデータがあるかもしれない
				pPack->m_status = PACKET_STATUS_NEED_CONTINUE;
			}
		}
	} else {
		// 不明
		pcapData.m_type = XSECS_HSMS_TYPE_UNKNOW;
		pPack->m_status = PACKET_STATUS_INIT;
	}
	return pPack->m_status;
XEXCEPT_EXIT
}

XSecsPcapParser::packetSeq* XSecsPcapParser::findPacketSeq(XSecsPcapData *pPcapData)
{
XEXCEPT_ENTRY
	// 通信の向きが同じシーケンスアイテムを見つける
	std::vector<packetSeq*>::iterator ite;
	for( ite = m_packets.begin(); ite != m_packets.end(); ite++) {
		packetSeq *pDst = *ite;
		if( pDst->m_comp.m_ip_src.S_un.S_addr == pPcapData->m_ip_src.S_un.S_addr &&
			pDst->m_comp.m_port_src== pPcapData->m_port_src                      &&
			pDst->m_comp.m_ip_dst.S_un.S_addr == pPcapData->m_ip_dst.S_un.S_addr &&
			pDst->m_comp.m_port_dst== pPcapData->m_port_dst                      ) {
			return *ite;
		}
	}
	return NULL;
XEXCEPT_EXIT
}
XSecsPcapParser::packetSeq* XSecsPcapParser::findDstPacketSeq(XSecsPcapData *pPcapData)
{
XEXCEPT_ENTRY
	// 通信の向きが逆のシーケンスアイテムを見つける
	std::vector<packetSeq*>::iterator ite;
	for( ite = m_packets.begin(); ite != m_packets.end(); ite++) {
		packetSeq *pDst = *ite;
		if( pDst->m_comp.m_ip_src.S_un.S_addr == pPcapData->m_ip_dst.S_un.S_addr &&
			pDst->m_comp.m_port_src== pPcapData->m_port_dst                      &&
			pDst->m_comp.m_ip_dst.S_un.S_addr == pPcapData->m_ip_src.S_un.S_addr &&
			pDst->m_comp.m_port_dst== pPcapData->m_port_src                      ) {
			return *ite;
		}
	}
	return NULL;
XEXCEPT_EXIT
}
bool XSecsPcapParser::foundSackValues(XBIN* pTCPHeader, unsigned long ulTCPHeader, 
										  unsigned long** pSacksValue, unsigned long *pSacksCount)
{
XEXCEPT_ENTRY
	
	if( !(ulTCPHeader > sizeof(tcphdr)) ) {
		return false;
	}
	
	long ulLen = ulTCPHeader - sizeof(tcphdr);
	XBIN *pTCPOpt = pTCPHeader + sizeof(tcphdr);
	register int iTCPOpt, iTCPOptLen, iTCPOptDataLen;

	*pSacksCount = 0;

	while (ulLen > 0) {
		iTCPOpt = *pTCPOpt++;
		if( iTCPOpt == 0 || iTCPOpt == 1) {
			iTCPOptLen = 1;
		} else {
			//TCHECK(*pTCPOpt);
			iTCPOptLen = *pTCPOpt++;
			if (iTCPOptLen < 2 || iTCPOptLen > ulLen) {
				break;
			}
			--ulLen;
		}			
		ulLen--;
		iTCPOptDataLen = 0;

		switch( iTCPOpt ) {
		case 0:	// TCPOPT_EOL
		case 1:	//	TCPOPT_NOP
		case 4:	//	TCPOPT_SACKOK selective ack ok (rfc2018)
			break;
		case 2:	// TCPOPT_MAXSEG
			iTCPOptDataLen = 2;
			break;
		case 3:	//	TCPOPT_WSCALE window scale factor (rfc1323)
			iTCPOptDataLen = 1;
			break;
		case 5:	//	TCPOPT_SACK selective ack (rfc2018)
			iTCPOptDataLen = iTCPOptLen - 2;
			if( (iTCPOptDataLen % 8) != 0 ) {
				return false;
			} else {
				*pSacksCount = iTCPOptDataLen / 8;
				*pSacksValue = (unsigned long*)pTCPOpt;
			}
			break;
		case 8:	//	TCPOLEN_TIMESTAMP	timestamp (rfc1323)
			iTCPOptDataLen = 8;
			break;
		case 6:	//	TCPOPT_ECHO echo (rfc1072)
		case 7:	//	TCPOPT_ECHOREPLY echo (rfc1072)
		case 11://	TCPOPT_CC	T/TCP CC options (rfc1644)
		case 12://	TCPOPT_CCNEW	T/TCP CC options (rfc1644)
		case 13://	TCPOPT_CCECHO	T/TCP CC options (rfc1644)
			iTCPOptDataLen = 4;
			break;
		default:
			iTCPOptDataLen = iTCPOptLen - 2;
			break;
		}
		pTCPOpt += iTCPOptDataLen;
		ulLen -= iTCPOptDataLen;

		++iTCPOptDataLen;
		if( !(iTCPOpt == 0 || iTCPOpt == 1) ) { // !( TCPOPT_EOL || TCPOPT_NOP)
			++iTCPOptDataLen;
		}
		if( iTCPOpt == 0 ) {	// TCPOPT_EOL
			break;
		}
	}
	return *pSacksCount != 0;
XEXCEPT_EXIT
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSecsPcapParserImple imple
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
XSecsPcapParserImple::XSecsPcapParserImple()
{
XEXCEPT_ENTRY

	if( m_libPcap.open("wpcap.dll", 0) == true ) {
		m_p_pcap_open_offline = m_libPcap.loadSymbol("pcap_open_offline");
		m_p_pcap_loop = m_libPcap.loadSymbol("pcap_loop");
		m_p_pcap_close = m_libPcap.loadSymbol("pcap_close");

		m_p_pcap_lookupdev		= m_libPcap.loadSymbol("pcap_lookupdev");
		m_p_pcap_open_live		= m_libPcap.loadSymbol("pcap_open_live");
		m_p_pcap_dump_open		= m_libPcap.loadSymbol("pcap_dump_open");
		m_p_pcap_snapshot		= m_libPcap.loadSymbol("pcap_snapshot");
		m_p_pcap_lookupnet		= m_libPcap.loadSymbol("pcap_lookupnet");
		m_p_pcap_compile		= m_libPcap.loadSymbol("pcap_compile");
		m_p_pcap_setfilter		= m_libPcap.loadSymbol("pcap_setfilter");
		m_p_pcap_geterr			= m_libPcap.loadSymbol("pcap_geterr");
		m_p_pcap_dump_close		= m_libPcap.loadSymbol("pcap_dump_close");
		m_p_pcap_findalldevs	= m_libPcap.loadSymbol("pcap_findalldevs");

		if( m_p_pcap_open_offline == NULL ||
			m_p_pcap_loop == NULL ||
			m_p_pcap_close == NULL ) {

			m_p_pcap_open_offline = NULL;
			m_p_pcap_loop = NULL;
			m_p_pcap_close = NULL;
		}
	}

XEXCEPT_EXIT
}


XSecsPcapParserImple::~XSecsPcapParserImple()
{
XEXCEPT_ENTRY
	std::vector<packetSeq*>::iterator ite = m_packets.begin();
	for(; ite != m_packets.end(); ite++) {
		packetSeq* pSEQ = (*ite);
		delete pSEQ;
	}
	m_packets.clear();

	closeLive();

	m_libPcap.close();
XEXCEPT_EXIT
}

void XSecsPcapParserImple::setDeltaTime(int iDeltaTime)
{
XEXCEPT_ENTRY
	m_deltaTime = iDeltaTime;
XEXCEPT_EXIT
}

bool XSecsPcapParserImple::parse(const char* pFilePath)
{
XEXCEPT_ENTRY
	char ebuf[1000] = "";
	register int nChekPoint = 0;

	try {
		void*	(*p_open_f)(const char *, char *);
		int		(*p_loop_f)(pcap_t *, int, pcap_handler, u_char *);
		void	(*p_close_f)(pcap_t *);

		p_open_f = (void* (*)(const char *, char *))m_p_pcap_open_offline;
		if( p_open_f == NULL ) {
			return false;
		}
		p_loop_f = (int (*)(pcap_t *, int, pcap_handler, u_char *))m_p_pcap_loop;
		if( p_loop_f == NULL ) {
			return false;
		}
		p_close_f = (void	(*)(pcap_t *))m_p_pcap_close;
		if( p_close_f == NULL ) {
			return false;
		}


		m_pd = (void*)p_open_f(pFilePath, ebuf);
		if ( m_pd == NULL ) {
			throw (0);
		}
		if (p_loop_f((pcap_t*)m_pd, -1, XSecsPcapParser_catptureHandler, (XBIN*)this) < 0) {
		}
		p_close_f( (pcap_t*)m_pd );
		m_pd = NULL;
	} catch(...) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}



bool XSecsPcapParserImple::parseLive(int iDev, const char* pParam, int iSnapLen)
{
XEXCEPT_ENTRY

	char ebuf[1000] = "";
	pcap_if_t* devpointer;

	register char *pDevice;
	char *pTargetDevice = NULL;
	wchar_t *pTargetDeviceW = NULL;
	bpf_u_int32   localnet, netmask;
	int           i;
	struct bpf_program fcode;



	// デバイス文字を取得します。
	pDevice = ((char*(*)(char *))m_p_pcap_lookupdev)(ebuf);
	if( pDevice == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, ebuf, 0, 0);
	}


	if( ((int(*)(pcap_if_t **, char *))m_p_pcap_findalldevs)(&devpointer, ebuf) < 0 ) {
		XEXCEPT_THROW_STR(XExcept_SECS, ebuf, 0, 0);
	}
	for (i = 0; i < iDev-1; i++){
		devpointer = devpointer->next;
		if (devpointer == NULL) {
			XEXCEPT_THROW_STR(XExcept_SECS, _T("Invalid adapter index"), 0, 0);
		}
	}
	pTargetDevice = devpointer->name;


#ifdef WIN32
//	pTargetDevice = (char*)XSecsPcapParserImple_getAdapterFromList(pDevice, iDev);
//	pTargetDeviceW = (wchar_t *)pTargetDevice;
//	if( pTargetDevice == NULL ) {
//		XEXCEPT_THROW_STR(XExcept_SECS, "invalid interface", 0, 0);
//	}
#endif
//	// ダンプファイルをオープン
//		if( lstrlen(m_strDumperFileName) != 0) {
#ifdef _UNICODE
//			m_pDumper = pcap_dump_open(m_pd, _bstr_t((LPCWSTR)m_strDumperFileName));
#else 
//			m_pDumper = pcap_dump_open(m_pd, m_strDumperFileName);
#endif /* _UNICODE */
//		}


	// デバイスのオープン
	m_pd = (void*)((pcap_t*(*)(const char *, int, int, int, char *))m_p_pcap_open_live)(
				pTargetDevice, 
				iSnapLen, 
				1, 
				1000, 
				ebuf);
	if( m_pd == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, ebuf, 0, 0);
	}

	// (int(*)(pcap_t *));
	i = ((int(*)(pcap_t *))m_p_pcap_snapshot)((pcap_t*)m_pd);

	// m_p_pcap_lookupnet
	if( ((int (*)(const char *, bpf_u_int32 *, bpf_u_int32 *, char *))m_p_pcap_lookupnet)(
		pTargetDevice, &localnet, &netmask, ebuf) < 0 ) {
		XEXCEPT_THROW_STR(XExcept_SECS, ebuf, 0, 0);
	}

	// m_p_pcap_compile
	i = ((int (*)(pcap_t *, struct bpf_program *, char *, int, bpf_u_int32)) m_p_pcap_compile)(
					(pcap_t*)m_pd, &fcode, (char*)pParam, 1, netmask);
	if ( i < 0) {
		sprintf(ebuf, "pcap_compile failur. %d, param:[%s]", i, pParam);
		XEXCEPT_THROW_STR(XExcept_SECS, ebuf, 0, 0);
	}

	// pcap_setfilter
	if (((int (*)(pcap_t *, struct bpf_program *)) m_p_pcap_setfilter)((pcap_t *)m_pd, &fcode) < 0) {
		XEXCEPT_THROW_STR(XExcept_SECS, "pcap_setfilter", 0, 0);
	}

	m_pLinveThread = new AnyThread(this);
	m_pLinveThread->start();

	return true;
XEXCEPT_EXIT
}


void XSecsPcapParserImple::closeLive()
{
XEXCEPT_ENTRY
	if( m_pd != 0 ) {
		((void(*)(pcap_t *))m_p_pcap_close)( (pcap_t*)m_pd );
	}
	if( m_pLinveThread != NULL ) {
		m_pLinveThread->Join(0);
		m_pLinveThread = 0;
	}
	if( m_pd != 0 ) {
		m_pd = 0;
	}
XEXCEPT_EXIT
}


void XSecsPcapParserImple::liveLoopBack()
{
	try {
		// pcap_loop
		int i = ((int (*)(pcap_t *, int, pcap_handler, u_char *))m_p_pcap_loop)(
			(pcap_t *)m_pd, -1, XSecsPcapParser_catptureHandler, (PUCHAR)this);
	} catch(...) {
		
	}
}


bool XSecsPcapParserImple::serializeLoad(const char* pFilePath)
{
XEXCEPT_ENTRY
	std::fstream file;
	XBuff data_buff;


	file.open(pFilePath, std::ios::in | std::ios::binary);
	if( file.is_open() == 0 ) {
		return false;
	}

	while(1) {
		XSecsPcapData data;
		if( data.serializeLoad(file, data_buff) == false ) {
			break;
		}
		
		
		if( m_pCallBackFunction ) {
			m_pCallBackFunction(&data, m_pUserCaallBackData);
		}
	}
	file.close();
	return true;
XEXCEPT_EXIT
}


void XSecsPcapParserImple::innerParse(const void *pH, const u_char *pkt_data)
{
XEXCEPT_ENTRY
	const struct pcap_pkthdr *header = (const struct pcap_pkthdr *)pH;
	struct pcap_pkthdr		lPacket;
	u_int					caplen = header->caplen;
	u_int					length = header->len;
	struct timeval			timeVal;
	register const u_char	*pWk, *packetp;
	const u_char			*pChunkValid;
	u_short					ether_type;
	struct	ether_header	*pEtherHeader;
	struct ip				*pIP;
	struct tcphdr			*pTCP;
	register u_int			hlen_ip, hlen_tcp, len, off;
	XSecsPcapData			pcapData;
	unsigned long			ulSaveLen = 0;


	//struct pcap_pkthdr {
	//	struct timeval ts;	/* time stamp */
	//	bpf_u_int32 caplen;	/* length of portion present */
	//	bpf_u_int32 len;	/* length this packet (off wire) */
	//};
	::memcpy(&lPacket, header, sizeof(struct pcap_pkthdr));

	/*
	|----------------------------------|
	|  ether_header                    |
	|----------------------------------|	
	|  IP DATA                         |
	|----------------------------------|
	|  TCP DATA                        |
	|----------------------------------|
	*/
	// 時間の保存
	::memcpy(&timeVal, &lPacket.ts, sizeof(timeVal));
	{
		// デルタ時間を加算する
		timeVal.tv_sec += m_deltaTime;

		struct tm* pTime = _gmtime32((const long *) &timeVal.tv_sec);
		if( pTime ) {
			::memcpy(&pcapData.m_Time, pTime,  sizeof(pcapData.m_Time));
		}
		pcapData.m_ulTimeUSec = ::ntohl( timeVal.tv_usec );
	}

	if (caplen < ETHER_HDRLEN) {
		return ;
	}
	packetp = pkt_data;
	pChunkValid = pkt_data + caplen;
	// ETHER 部の解析
	length -= ETHER_HDRLEN;
	caplen -= ETHER_HDRLEN;
	pEtherHeader = (struct ether_header *)pkt_data;
	pkt_data += ETHER_HDRLEN;

	// ETHER のタイプ
	ether_type = ::ntohs(pEtherHeader->ether_type);

	// IP 部の解析
	{
		// IP のデータ以外は無視
		if( ether_type != ETHERTYPE_IP ) {
			return;
		}
		// チェック
		if ((pkt_data + 1) > pChunkValid) {
			return;
		}
		if (length < sizeof (struct ip)) {
//			TRACE(_T("truncated-ip %d\n"), length);
			return;
		}
		pIP = (struct ip*)pkt_data;
		::memcpy(&pcapData.m_ip_src, &pIP->ip_src, sizeof(struct	in_addr));
		::memcpy(&pcapData.m_ip_dst, &pIP->ip_dst, sizeof(struct	in_addr));

		hlen_ip = IP_HL(pIP) * 4;
		len = ::ntohs(pIP->ip_len);
		if (length < len) {
//			TRACE(_T("truncated-ip - %d bytes missing!"), len - length) ;
			return;
		}
		len -= hlen_ip;
		off = ntohs(pIP->ip_off);
		if ((off & 0x1fff) != 0) {
//			TRACE(_T("Valid OFF\n")) ;
			return;
		}
		//length -= hlen_ip;
		length = len;
	}
	// TCP 部の解析
	{
		pWk = (const u_char *)pIP + hlen_ip;
		pTCP = (tcphdr *)pWk;
		if( pIP->ip_p != IPPROTO_TCP ) {
//			TRACE(_T("Not TCP  (%d)\n"), tcpip.m_IP->ip_p) ;
			return ;
		}

		hlen_tcp = TH_OFF(pTCP) * 4;
		if (hlen_tcp > len) {
//			TRACE(_T(" [bad hdr length]\n"));
			return ;
		}
		length -= hlen_tcp;
		pWk    += hlen_tcp;

		pcapData.m_port_src		= ::ntohs(pTCP->th_sport);
		pcapData.m_port_dst		= ::ntohs(pTCP->th_dport);
		pcapData.m_th_seq		= ::ntohl(pTCP->th_seq);
		pcapData.m_th_ack		= ::ntohl(pTCP->th_ack);
		pcapData.m_th_flags		= pTCP->th_flags;
		pcapData.m_th_win		= ::ntohs(pTCP->th_win);
		pcapData.m_th_sum		= ::ntohs(pTCP->th_sum);
		pcapData.m_th_urp		= ::ntohs(pTCP->th_urp);
		ulSaveLen = pcapData.m_length		= length;
		pcapData.m_pData		= (XBIN*)pWk;
	} 


	inner(pcapData, pEtherHeader, pIP, hlen_ip, pTCP, hlen_tcp);
XEXCEPT_EXIT
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSecsPcapParserImple::packetSeq
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
XSecsNICapParserImple::XSecsNICapParserImple()
	:	m_sock(~0)
	,	m_iSnapLen( 10000 )
	,	m_pOpts( NULL )
	,	n_nNumOpt( 0 )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XSecsNICapParserImple::~XSecsNICapParserImple()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
bool XSecsNICapParserImple::parse(const char* pFilePath)
{
XEXCEPT_ENTRY
	return true;
XEXCEPT_EXIT
}

bool XSecsNICapParserImple::parseLive(int iDev, const char* pParam, int iSnapLen)
{
XEXCEPT_ENTRY
	struct sockaddr_in* pTargtAddr = NULL;


#ifdef _WIN32
	// Windows 用の実装
	SOCKET_ADDRESS_LIST* pAddrList;
	XBIN buf[4029];
	DWORD dwTmp;
	unsigned int optval;

	closeLive();


	//コマンドラインを解析
	parseOption(pParam);

	
	// リードサイズ
	m_iSnapLen = iSnapLen;

	// Rawソケット作成
	m_sock = ::WSASocket(AF_INET, SOCK_RAW, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if( m_sock == ~0 ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("WSASocket"), ::WSAGetLastError(), 0);
		return false;
	}
    // アダプタのリスト取得
    if(WSAIoctl(m_sock,SIO_ADDRESS_LIST_QUERY,NULL,0,buf,sizeof(buf),&dwTmp,NULL,NULL) == SOCKET_ERROR){
		XEXCEPT_THROW_STR(XExcept_SECS, _T("WSAIoctl(SIO_ADDRESS_LIST_QUERY)"), ::WSAGetLastError(), 0);
        return false;
    }
    pAddrList = (SOCKET_ADDRESS_LIST *)buf;

	if( iDev < pAddrList->iAddressCount || iDev == 0 ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("Devaice ID Error"), 0, 0);
		return false;
	}
	pTargtAddr = (struct sockaddr_in*)pAddrList->Address[ iDev - 1 ].lpSockaddr;

#ifdef _DEBUG
	for(int  i = 0; i < pAddrList->iAddressCount; i++) {
		struct sockaddr_in* pAddr = (struct sockaddr_in*)pAddrList->Address[i].lpSockaddr;
		XTRACE(_T("IP:%s\n"), (const TCHAR*)XStrBuff( inet_ntoa( pAddr->sin_addr ) ) );
	}
#endif

	// デバイスに接続
	pTargtAddr->sin_family = AF_INET;
	pTargtAddr->sin_port = 0;
	if( ::bind(m_sock,(SOCKADDR *)pTargtAddr,sizeof(struct sockaddr_in)) == ~0 ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("bind"), ::WSAGetLastError(), 0);
        return false;
	}


#ifndef SIO_RCVALL
#define SIO_RCVALL _WSAIOW(IOC_VENDOR, 2)
#endif
	// 全て受信のソケットオプションをセット
	optval = 1;
	if( ::WSAIoctl(m_sock, SIO_RCVALL, &optval, sizeof(optval),NULL,0, &dwTmp,NULL,NULL) == ~0 ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("WSAIoctl(SIO_RCVALL)"), ::WSAGetLastError(), 0);
        return false;
	}
#else

	closeLive();

#endif /*_WIN32*/

	m_pLinveThread = new AnyThread(this);
	if( m_pLinveThread == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("new AnyThread"), ::WSAGetLastError(), 0);
		return false;
	}
	m_pLinveThread->start();

	return true;
XEXCEPT_EXIT
}
void XSecsNICapParserImple::closeLive()
{
XEXCEPT_ENTRY

	if( m_sock != ~0 ) {
#ifdef _WIN32
		::closesocket(m_sock);
#else
		::close(m_sock);
#endif /*_WIN32*/
		m_sock = ~0;
	}

	if( m_pLinveThread != NULL ) {
		m_pLinveThread->Join(0);
		m_pLinveThread = 0;
	}


	if( m_pOpts != NULL ) {
		delete []m_pOpts;
		m_pOpts = NULL;
	}
	n_nNumOpt = 0;
	m_pOpts = NULL;

	return;
XEXCEPT_EXIT
}
bool XSecsNICapParserImple::serializeLoad(const char* pFilePath)
{
XEXCEPT_ENTRY
	return true;
XEXCEPT_EXIT
}

void XSecsNICapParserImple::setDeltaTime(int iDeltaTime)
{
XEXCEPT_ENTRY
	return;
XEXCEPT_EXIT
}

void XSecsNICapParserImple::liveLoopBack()
{
XEXCEPT_ENTRY

	std::auto_ptr<XBIN> apBuff( new XBIN[ m_iSnapLen ] );
	XBIN* pBuff = apBuff.get();
	
	
	if( pBuff == NULL ) {
		return ;
	}
	

#ifdef _WIN32
	// Windows 用の実装

	WSABUF wsb;

	while( 1 ) {
		unsigned long length = 0;
		unsigned long Flags = 0;

		Sleep(1);

		wsb.buf=(char*)pBuff;
		wsb.len=m_iSnapLen;
		memset(pBuff,0x0, m_iSnapLen);			

		if( SOCKET_ERROR == ::WSARecv(m_sock,&wsb,1,&length,&Flags,NULL,NULL)){
			break;
		}
		if( length == 0 ) {
			continue;
		}

		/*
		|----------------------------------|	
		|  IP DATA                         |
		|----------------------------------|
		|  TCP DATA                        |
		|----------------------------------|
		*/
		register const u_char	*pWk;
		struct ip				*pIP;
		struct tcphdr			*pTCP;
		register u_int			hlen_ip, hlen_tcp, len, off;
		XSecsPcapData			pcapData;
		unsigned long			ulSaveLen = 0;

		pIP = (struct ip*)pBuff;
		::memcpy(&pcapData.m_ip_src, &pIP->ip_src, sizeof(struct	in_addr));
		::memcpy(&pcapData.m_ip_dst, &pIP->ip_dst, sizeof(struct	in_addr));

		if( pIP->ip_p != IPPROTO_TCP ) {
			// TCPのみ対象とする
			continue;
		}

		hlen_ip = IP_HL(pIP) * 4;
		len = ::ntohs(pIP->ip_len);
		if (length < len) {
//			XTRACE(_T("truncated-ip - %d bytes missing!"), len - length) ;
			continue;
		}
		len -= hlen_ip;
		off = ntohs(pIP->ip_off);
		if ((off & 0x1fff) != 0) {
//			XTRACE(_T("Valid OFF\n")) ;
			continue;
		}
		length = len;

		// TCP 部の解析
		pWk = (const u_char *)pIP + hlen_ip;
		pTCP = (tcphdr *)pWk;

		hlen_tcp = TH_OFF(pTCP) * 4;

		length -= hlen_tcp;
		pWk    += hlen_tcp;

		pcapData.m_port_src		= ::ntohs(pTCP->th_sport);
		pcapData.m_port_dst		= ::ntohs(pTCP->th_dport);
		pcapData.m_th_seq		= ::ntohl(pTCP->th_seq);
		pcapData.m_th_ack		= ::ntohl(pTCP->th_ack);
		pcapData.m_th_flags		= pTCP->th_flags;
		pcapData.m_th_win		= ::ntohs(pTCP->th_win);
		pcapData.m_th_sum		= ::ntohs(pTCP->th_sum);
		pcapData.m_th_urp		= ::ntohs(pTCP->th_urp);
		ulSaveLen = pcapData.m_length		= length;
		pcapData.m_pData		= (XBIN*)pWk;

		bool bFind = false;
		for(int i = 0; i < n_nNumOpt; i++ ) {
			if( m_pOpts[i].m_mode & 0x01 ) {
				if( pcapData.m_ip_src.S_un.S_addr == m_pOpts[i].m_filterIP ||
					pcapData.m_ip_dst.S_un.S_addr == m_pOpts[i].m_filterIP ) {
					bFind = true;
				}
			} else
			if( m_pOpts[i].m_mode & 0x02 ) {
				if( pcapData.m_port_src == m_pOpts[i].m_filterPort ||
					pcapData.m_port_dst == m_pOpts[i].m_filterPort ) {
					bFind = true;
				}
			}
		}
		if( bFind == true || n_nNumOpt == 0 ) {
			time_t ti = ::time(0);
			struct tm* pTime = ::localtime(&ti);
			if( pTime ) {
				::memcpy(&pcapData.m_Time, pTime,  sizeof(pcapData.m_Time));
			}
			pcapData.m_ulTimeUSec = 0;


			inner(pcapData, 0, pIP, hlen_ip, pTCP, hlen_tcp);
		}
	}	
#else


#endif /*_WIN32*/

	return;
XEXCEPT_EXIT
}

bool XSecsNICapParserImple::parseOption(const char* pParam)
{
XEXCEPT_ENTRY
	XStrBuffSplit argBuff(_T(' ') , _T('\"') );
	argBuff = pParam;
	int argc            = argBuff.GetArgCouunt();
	const TCHAR** pArgv = argBuff.GetArgv();
	int		cntOpt = 0;
	int		i;

	if( m_pOpts != NULL ) {
		delete []m_pOpts;
		m_pOpts = NULL;
	}
	n_nNumOpt = 0;
	m_pOpts = NULL;

	for(i = 0; i < argc; i++) {
		if( XStrBuff::StrICmp(_T("host"), pArgv[i]) == 0 ) {
			if( i >= (argc-1) ) {
				XEXCEPT_THROW_STR(XExcept_SECS, _T("invalid parameter. hostname not specify."), ::WSAGetLastError(), 0);
				return false;
			}
			cntOpt++;
			i++;
		} else 
		if( XStrBuff::StrICmp(_T("port"), pArgv[i]) == 0 ) {
			if( i >= (argc-1) ) {
				XEXCEPT_THROW_STR(XExcept_SECS, _T("invalid parameter. portnumber not specify."), ::WSAGetLastError(), 0);
				return false;
			}
			cntOpt++;
			i++;
		}
	}



	if( cntOpt == 0 ) {
		return true;
	}
	m_pOpts = new  option[cntOpt];
	if( m_pOpts == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("malloc"), 0, cntOpt*sizeof(option));
		return false;
	}
	n_nNumOpt = cntOpt;
	cntOpt = 0;
	for(i = 0; i < argc; i++) {
		if( XStrBuff::StrICmp(_T("host"), pArgv[i]) == 0 ) {
			m_pOpts[cntOpt].m_mode = 0x01;
			m_pOpts[cntOpt].m_filterIP = ::inet_addr( XStrBuff(pArgv[i+1]) );
			cntOpt++;
			i++;
		} else 
		if( XStrBuff::StrICmp(_T("port"), pArgv[i]) == 0 ) {
			m_pOpts[cntOpt].m_mode = 0x02;
			m_pOpts[cntOpt].m_filterPort = (unsigned short)XStrBuff::AtoI(pArgv[i+1]);

			cntOpt++;
			i++;
		}
	}

	return true;
XEXCEPT_EXIT
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XSecsPcapParser::packetSeq
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
XBIN* XSecsPcapParser::packetSeq::newSetBuff(const XBIN* pBuff, int iSize)
{
XEXCEPT_ENTRY
	m_buffer.resetp();
	m_buffer.sputn((char*)pBuff, iSize);

	m_comp.m_pData = m_buffer.getBuff();
	m_comp.m_length = this->size();

	return m_buffer.getBuff();
XEXCEPT_EXIT
}
XBIN* XSecsPcapParser::packetSeq::appendBuff(const XBIN* pBuff, int iSize)
{
XEXCEPT_ENTRY
	m_buffer.sputn((char*)pBuff, iSize);
	

	m_comp.m_pData = m_buffer.getBuff();
	m_comp.m_length = this->size();


	return m_buffer.getBuff();
XEXCEPT_EXIT
}
XBIN* XSecsPcapParser::packetSeq::moveOverBeforeBuff(size_t iSize)
{
XEXCEPT_ENTRY
	XBIN* pBuff = 0;
	XD_UINT4       ulSize = 0;
	if( m_buffer.size() < iSize ) {
		return m_buffer.getBuff();
	}

	// スワップに保存
	m_bufferSwap.resetp();
	pBuff  = m_buffer.getBuff();
	ulSize = m_buffer.size();
	pBuff += iSize;
	ulSize -= iSize;
	m_bufferSwap.sputn((char*)pBuff, ulSize);

	// 
	m_buffer.resetp();
	m_buffer.sputn((char*)m_bufferSwap.getBuff(), m_bufferSwap.size());


	m_comp.m_pData = m_buffer.getBuff();
	m_comp.m_length = this->size();

	return m_buffer.getBuff();
XEXCEPT_EXIT
}

XSecsPcapParser::packetSeq* XSecsPcapParser::packetSeq::findSack(unsigned long ulSeq)
{
XEXCEPT_ENTRY

	std::vector<packetSeq*>::iterator iteSack = m_sacksDataItems.begin();
	for(; iteSack != m_sacksDataItems.end(); iteSack++) {
		packetSeq* p = *iteSack;

		if( p->m_comp.m_th_seq <= ulSeq
			
				&&

			ulSeq <= p->m_comp.m_th_seq + p->m_comp.m_length ) {

			m_sacksDataItems.erase( iteSack );
			return p;
		}
	}
	return NULL;
XEXCEPT_EXIT
}
bool XSecsPcapParser::packetSeq::addHeader(XBIN* pEt, 
												XBIN* pIP, unsigned long ulIPLen, 
												XBIN* pTCP, unsigned long ulTCPLen, 
												unsigned long ulLen)
{
XEXCEPT_ENTRY
	// パケットヘッダ情報の追加
	XSecsPcapPacketHead* pNewPcaHead = new XSecsPcapPacketHead(
		(XBIN*)pEt, 
		(XBIN*)pIP, ulIPLen,
		(XBIN*)pTCP, ulTCPLen,
		ulLen);
	if( pNewPcaHead == NULL ) {
		return false;
	}
	m_comp.m_pcket_headers.push_back(pNewPcaHead);
	return true;
XEXCEPT_EXIT
}
void XSecsPcapParser::packetSeq::clearHeaders()
{
XEXCEPT_ENTRY
	// パケットヘッダ情報を削除
	std::vector<XSecsPcapPacketHead*>::iterator ite = m_comp.m_pcket_headers.begin();
	for(; ite != m_comp.m_pcket_headers.end(); ite++) {
		XSecsPcapPacketHead* p = *ite;
		delete p;
	}
	m_comp.m_pcket_headers.clear();
XEXCEPT_EXIT
}
XSecsPcapPacketHead* XSecsPcapParser::packetSeq::getTopHead()
{
XEXCEPT_ENTRY
	if( m_comp.m_pcket_headers.size() == 0 ) {
		return NULL;
	}
	return m_comp.m_pcket_headers[0];
XEXCEPT_EXIT
}

