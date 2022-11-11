/**
 * @file xsecs_pcap_parser.h
 * @brief �����̐���SEMI�K�i TCPDUMP����SECS�f�[�^�𒊏o����N���X
 *
 * @author T.Kawabuchi
 * @date 2005-04/04
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_SECS_PCAP_PARSER_HEASER_FILE__
#define __X_SECS_PCAP_PARSER_HEASER_FILE__

#ifdef WIN32
#include "xlib/xsocket.h"
#endif
#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"		// include socket definition.
#include "xlib/xdllorder.h"
#include "xlib/semi/secs/xsecs.h"

#include <fstream>
#include <vector>



class XSecsPcapParser;
class XSecsPcapData;
class XSecsPcapParserImple;
class XSecsPcapPacketHead;


/**	@brief	PCAP�f�[�^�N���X�B���[�U�[�R�[���o�b�N�֐��œn�����	*/
class XLIB_API XSecsPcapData
{
public:
	/**	@brief	�R���X�g���N�^	 */
	XSecsPcapData();
	/**	@brief	�f�X�g���N�^
	 */
	virtual ~XSecsPcapData();


	/**	@brief	�X�g���[���ɃC���X�^���X���𕶎���ŏo��
	 *
	 *	@param	stream	�X�g���[��
	 *	@param	nDepth	�C���f���g�̐[��
	 *	@param	pLF	���s�R�[�h
	 *	@param	bTCPIPheader	TCPIP�w�b�_�[��\�����邩�ǂ���
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"), bool bTCPIPheader = false);

	bool printDump(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"), bool bTCPIPheader = false);
	
	/**	@brief	�V���A���C�Y���ꂽ�f�[�^���������݂܂��B
	 *
	 *	@param	stream	�X�g���[��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool serializeStore(std::ostream &stream);

	/**	@brief	�V���A���C�Y���ꂽ�f�[�^��ǂݍ��݂܂��B
	 *
	 *	@param	stream	�X�g���[��
	 *	@param	buff	�o�b�t�@
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool serializeLoad(std::istream &stream, XBuff &buff);
	
	
	/**	@brief	�f�[�^��
	 */
	void debugDump();
	/**	@brief	�C���X�^���X�̃R�s�[
	 *
	 *	@param	right	�R�s�[���C���X�^���X
	 */
	XSecsPcapData& operator = (const XSecsPcapData& right);

	/**	@brief	����*/
	struct tm		m_Time;
	/**	@brief	���ԁD�i�m�b*/
	unsigned long	m_ulTimeUSec;
	/**	@brief	���M��IP*/
	struct in_addr	m_ip_src;
	/**	@brief	��M��IP*/
	struct in_addr	m_ip_dst;
	/**	@brief	���M��Port*/
	XD_UINT2		m_port_src;
	/**	@brief	��M��Port*/
	XD_UINT2		m_port_dst;
	/**	@brief	Sequence�i���o�[*/
	XD_UINT4		m_th_seq;
	/**	@brief	Ack�i���o�[*/
	XD_UINT4		m_th_ack;
	/**	@brief	TCP�t���O*/
	XD_UINT1		m_th_flags;
	/**	@brief	�E�C���h�E*/
	XD_UINT2		m_th_win;
	/**	@brief	TCP�`�F�b�N�T���l*/
	XD_UINT2		m_th_sum;
	/**	@brief	TCP�ً}�f�[�^*/
	XD_UINT2		m_th_urp;
	/**	@brief	�f�[�^��*/
	XD_UINT4		m_length;
	/**	@brief	�O�̃p�P�b�g�̃f�[�^����*/
	XD_UINT4		m_lengthPrev;
	/**	@brief	�f�[�^*/
	XBIN*	m_pData;

	bool			m_bPrintHeaderOnly;

	/**	@brief	�f�[�^�̎��*/
	XSECS_HSMS_TYPE	m_type;
	/**	@brief	�f�[�^�̎�ނ�SECS�܂��́AHSMS�̏ꍇ�ASECS�w�b�_�[���*/
	XSECSHeader		m_secsHeader;
	/**	@brief	�f�[�^�̎�ނ�SECS�̏ꍇ�̃f�[�^��*/
	XD_UINT4		m_secsDataLength;
	/**	@brief	�p�P�b�g�w�b�_�[�̔z�� */
	std::vector<XSecsPcapPacketHead*>	m_pcket_headers;

private:
	bool innerPrint(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"), bool bTCPIPheader = false, bool bDump = false);
};




/**	@brief	PCAP�_���v��̓C���^�[�t�F�[�X */
class XLIB_API XSecsPcapParser
{
public:
	/**	@brief	�R���X�g���N�^	 */
	XSecsPcapParser();
	/**	@brief	�f�X�g���N�^	 */
	virtual ~XSecsPcapParser();

	/**	@brief	�_���v�t�@�C���̃p�X���w�肵�āA�ǂݍ���
	 *
	 *	@param	pFilePath	�_���v�t�@�C���̃p�X
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool parse(const char* pFilePath) = 0;

	/**	@brief	The parseLive function
	 *
	 *	@param	iDev	a parameter of type int
	 *	@param	pParam	a parameter of type const char*
	 *	@param	iSnapLen	a parameter of type int
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool parseLive(int iDev, const char* pParam, int iSnapLen) = 0;

	virtual void closeLive() = 0;

	/**	@brief	�V���A���C�Y���ꂽ�f�[�^��ǂݍ��݂܂��B
	 *
	 *	@param	stream	�X�g���[��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool serializeLoad(const char* pFilePath) = 0;

	/**	@brief	���[�U�[��`�R�[���o�b�N�֐����Z�b�g����B
	 *
	 *	@param	pCallBackFunction	�R�[���o�b�N�֐�
	 *	@param	pUserCaallBackData	�R�[���o�b�N�ɓn�����A���[�U�[��`�̃f�[�^
	 *	@param	pRawCallBack		Raw�f�[�^��͗p�R�[���o�b�N�֐�
	 */
	void setCallBackFunction(
		void (*pCallBackFunction)(XSecsPcapData*,void*), 
		void *pUserCaallBackData, 
		void (*pRawCallBack)(XSecsPcapData* , void *, void *, unsigned int ,void* , unsigned int ) = NULL);



	virtual void setDeltaTime(int iDeltaTime) = 0;
	/**	@brief	�C���X�^���X���쐬
	 *
	 *	@return	�C���X�^���X
	 */
	static XSecsPcapParser* newInstance();
	static XSecsPcapParser* newInstanceNICap();
	/**	@brief	�C���X�^���X���J��
	 *
	 *	@param	ppParser	�C���X�^���X�̃A�h���X�B
	 */
	static void deleteInstance(XSecsPcapParser** ppParser);



protected:
	/*	@brief	��̓X�e�[�^�X	*/
	enum PACKET_STATUS {
		PACKET_STATUS_INIT,				//<	�������
		PACKET_STATUS_NEED_CONTINUE,	//<	�}���`�p�P�b�g�̓r��
		PACKET_STATUS_MULTI_SECS_MSG,	//<	�P�p�P�b�g�ɕ����f�[�^����
		PACKET_STATUS_SKIP,				//<	����
	};

	/*	@brief	�V�[�P���X�m�F�f�[�^�N���X	*/
	class packetSeq
	{
	public:
		packetSeq(XSecsPcapData &src) {
			m_comp = src;
			m_comp.m_length = 0;
			m_status = PACKET_STATUS_INIT;
			m_ulSackRequireCounter = 0;
		}
		/*	@brief	���̃C���X�^���X�̃f�[�^��Tranc�Ńf�[�^����������
		 *
		 *	@param	pBuff	�������ރf�[�^
		 *	@param	iSize	�������ރf�[�^�̃T�C�Y
		 *	@return	�f�[�^�̐擪�A�h���X�B
		 */
		XBIN* newSetBuff(const XBIN* pBuff, int iSize);
		/*	@brief	���̃C���X�^���X�̃f�[�^��Append�Ńf�[�^����������
		 *
		 *	@param	pBuff	�������ރf�[�^
		 *	@param	iSize	�������ރf�[�^�̃T�C�Y
		 *	@return	�f�[�^�̐擪�A�h���X�B
		 */
		XBIN* appendBuff(const XBIN* pBuff, int iSize);
		/*	@brief	�f�[�^���w�肵���T�C�Y�O�ֈړ�����B
		 *	@param	iSize	�ړ�����T�C�Y
		 *	@return	�f�[�^�̐擪�A�h���X�B
		 */
		XBIN* moveOverBeforeBuff(size_t iSize);
		
		/*	@brief	�f�[�^�����Ԃ�
		 *	@return	�f�[�^��
		 */
		XD_UINT4 size(){ return m_buffer.size(); }


		/*	@brief	SACK�A�C�e�����ߋ��Ɏ�M���Ă���ꍇ�A����SACK�A�C�e�����擾����B
		 *
		 *	@param	ulSeq	�擾����SACK��SEQ��SEQ+Length�̎��܂�V�[�P���X�i���o�[
		 *
		 *	@retval	NULL	SACK�A�C�e���������W���Ɍ�����Ȃ�����
		 *	@retval	NULL�ȊO	��������SACK�A�C�e��	
		 *	
		 */
		packetSeq* findSack(unsigned long ulSeq);

		/*	@brief	�w�b�_�[��ǉ�����B
		 *
		 *	@param	pEt	Ether�w�b�_�[
		 *	@param	pIP	IP�w�b�_�[
		 *	@param	ulIPLen	IP�w�b�_�[��
		 *	@param	pTCP	TCP�w�b�_�[�w�b�_�[
		 *	@param	ulTCPLen	TCP�w�b�_�[��
		 *	@param	ulLen	�f�[�^����
		 *
		 *	@retval	true	����
		 *	@retval	false	���s
		 */
		bool addHeader(XBIN* pEt, XBIN* pIP, unsigned long ulIPLen, XBIN* pTCP, unsigned long ulTCPLen, unsigned long ulLen);

		/*	@brief	�w�b�_�[������ɂ���B */
		void clearHeaders();

		/**	@brief	�w�b�_�[���̐擪���擾
		 *
		 *	@return	�w�b�_�[���BNULL�̏ꍇ�w�b�_�[��񂪂Ȃ��B
		 */
		XSecsPcapPacketHead* getTopHead();


		/* @brief �p�P�b�g��� */
		XSecsPcapData				m_comp;
		/* @brief �o�b�t�@ */
		XInflateStreamBuff			m_buffer;
		/* @brief �o�b�t�@ */
		XInflateStreamBuff			m_bufferSwap;
		/* @brief �X�e�[�^�X */
		PACKET_STATUS				m_status;
		/* @brief SACK�������ɁA�ȑO�̃p�P�b�g�𒙂߂Ēu�����߂̃f�[�^ */
		std::vector<packetSeq*>		m_sacksDataItems;
		/* @brief �}���`�p�P�b�g�̒���ACK�𒙂߂Ēu�����߂̃f�[�^ */
		std::vector<packetSeq*>		m_acksDataItems;
		unsigned long m_ulSackRequireCounter;
	};




protected:
	virtual void liveLoopBack() = 0;

	void inner(XSecsPcapData& pcapData, void *pEtherHeader, void *pIP, unsigned int hlen_ip,
		void* pTCP, unsigned int hlen_tcp);




	/*	@brief	�ʐM�̌����������V�[�P���X�A�C�e����������
	 *
	 *	@param	pPcapData	�ʐM�̌����iIP&�|�[�g�j���Z�b�g���ꂽ�AXSecsPcapData�C���X�^���X
	 *	@return	������Ȃ��ꍇ�́ANULL�B
	 */
	packetSeq* findPacketSeq(XSecsPcapData *pPcapData);
	/*	@brief	The �ʐM�̌������t�̃V�[�P���X�A�C�e���������� function
	 *
	 *	@param	pPcapData		�ʐM�̌����iIP&�|�[�g�j���Z�b�g���ꂽ�AXSecsPcapData�C���X�^���X
	 *	@return	������Ȃ��ꍇ�́ANULL�B
	 */
	packetSeq* findDstPacketSeq(XSecsPcapData *pPcapData);



	/**	@brief	TCP�I�v�V������SACK�̒l�������Ă��邩���ׂ܂��B
	 *
	 *	@param	pTCPHeader	TCP�w�b�_�[�̐擪�A�h���X
	 *	@param	ulTCPHeader	TCP�w�b�_�[����
	 *	@param	pSacksValue	SACK�l���󂯎��A�h���X
	 *	@param	pSacksCount	SACK�l������ꍇ�AStart:End�̑g�ݍ��킹��
	 *
	 *	@retval	true	�L
	 *	@retval	false	��
	 */
	static bool foundSackValues(XBIN* pTCPHeader, unsigned long ulTCPHeader, unsigned long** pSacksValue, unsigned long *pSacksCount);

	/*	@brief	�p�P�b�g�̃f�[�^��������͂��܂��B
	 *
	 *	@param	pcapData	��͌��ʂ�����o�b�t�@
	 *	@param	pPack	��͌��f�[�^
	 *
	 *	@return	��̓X�e�[�^�X
	 */
	PACKET_STATUS judge(XSecsPcapData &pcapData, packetSeq* pPack);

	class AnyThread : public XThread
	{
	public:
		AnyThread(void* pMe) 
			:	m_pMe(pMe)
		{
		}
		virtual ~AnyThread(){}
		void start() { XThread::StartUndetached(); }

	protected:
		virtual void* RunUndetached(void* pArg) { 
			((XSecsPcapParser*)m_pMe)->liveLoopBack();
			return NULL;
		}
		void*			m_pMe;
	};
friend class AnyThread;

protected:
	/*	@brief	���[�U�[��`�R�[���o�b�N�֐�*/
	void		(*m_pCallBackFunction)(XSecsPcapData*,void*);
	/*	@brief	Raw�f�[�^��͗p�R�[���o�b�N*/
	void (*m_pRawCallBack)(XSecsPcapData* , void *, void *, unsigned int ,void* , unsigned int );

	/*	@brief	�R�[���o�b�N�ɓn�����A���[�U�[��`�̃f�[�^�B*/
	void*		m_pUserCaallBackData;
	/*	@brief ���ԕ\�����̃f���^����	*/
	int						m_deltaTime;
	/* @brief	���A���^�C���擾�p�X���b�h */
	AnyThread*				m_pLinveThread;
	/* @breif �V�[�P���X�m�F�f�[�^�z�� */
	std::vector<packetSeq*>	m_packets;

friend class packetSeq;
};






// �ȉ������N���X


/*	@brief	�p�P�b�g�̃w�b�_���*/
class XLIB_API XSecsPcapPacketHead
{
public:
	/*	@brief	�R���X�g���N�^
	 *
	 *	@param	pEt	Ether�w�b�_�[
	 *	@param	pIP	IP�w�b�_�[
	 *	@param	ulIPLen	IP�w�b�_�[��
	 *	@param	pTCP	TCP�w�b�_�[�w�b�_�[
	 *	@param	ulTCPLen	TCP�w�b�_�[��
	 *	@param	ulLen	�f�[�^����
	 */
	XSecsPcapPacketHead(XBIN* pEt, XBIN* pIP, unsigned long ulIPLen, XBIN* pTCP, unsigned long ulTCPLen, unsigned long ulLen);
	/*	@brief	�f�X�g���N�^	 */
	virtual ~XSecsPcapPacketHead();
	/*	@brief	�X�g���[���ɃC���X�^���X���𕶎���ŏo��
	 *
	 *	@param	stream	�X�g���[��
	 *	@param	nDepth	�C���f���g�̐[��
	 *	@param	pLF	���s�R�[�h
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));

	/*	@brief	Ether�w�b�_�[�̐擪�A�h���X���擾
	 *
	 *	@return	Ether�w�b�_�[�̐擪�A�h���X
	 */
	XBIN* get_EtherHead();
	/*	@brief	IP�w�b�_�[�̐擪�A�h���X���擾
	 *
	 *	@return	IP�w�b�_�[�̐擪�A�h���X
	 */
	XBIN* get_IPHead();
	/*	@brief	TCP�w�b�_�[�̐擪�A�h���X���擾
	 *
	 *	@return	TCP�w�b�_�[�̐擪�A�h���X
	 */
	XBIN* get_TCPHead();
	/*	@brief	IP�w�b�_�[�����擾
	 *
	 *	@return	IP�w�b�_�[��
	 */
	unsigned long get_IPHeadLength();
	/*	@brief	TCP�w�b�_�[�����擾
	 *
	 *	@return	TCP�w�b�_�[��
	 */
	unsigned long get_TCPHeadLength();
	/*	@brief	�f�[�^�����擾
	 *
	 *	@return	�f�[�^��
	 */
	unsigned long get_DataLength();
private:
	/*	@brief	Ether�w�b�_�[*/
	XBIN*	m_pEtherHead;
	/*	@brief	IP�w�b�_�[*/
	XBIN*	m_pIPHead;
	/*	@brief	IP�w�b�_�[��*/
	unsigned long	m_ulIPHeader;
	/*	@brief	TCP�w�b�_�[�w�b�_�[*/
	XBIN*	m_pTCPHead;
	/*	@brief	TCP�w�b�_�[��*/
	unsigned long	m_ulTCPHeader;
	/*	@brief	�f�[�^����*/
	unsigned long	m_ulLen;
	/*	@brief	�o�b�t�@*/
	XBIN*	m_pBuff;
};




/*	@brief	Libpcap�_���v��͎��� */
class XSecsPcapParserImple : public XSecsPcapParser
{
public:
	/*	@brief	�R���X�g���N�^	 */
	XSecsPcapParserImple();
	/**	@brief	�f�X�g���N�^	 */
	virtual ~XSecsPcapParserImple();
	/*	@brief	�_���v�t�@�C���̃p�X���w�肵�āA�ǂݍ���
	 *
	 *	@param	pFilePath	�_���v�t�@�C���̃p�X
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool parse(const char* pFilePath);

	virtual bool parseLive(int iDev, const char* pParam, int iSnapLen);


	virtual void closeLive();

	/**	@brief	�V���A���C�Y���ꂽ�f�[�^��ǂݍ��݂܂��B
	 *
	 *	@param	stream	�X�g���[��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool serializeLoad(const char* pFilePath);

	virtual void setDeltaTime(int iDeltaTime);

	/*	@brief	��͓�������
	 *
	 *	@param	pH	�p�P�b�g�w�b�_�[
	 *	@param	pkt_data	�p�P�b�g�f�[�^
	 */
	void innerParse(const void* pH, const u_char *pkt_data);


protected:
	virtual void liveLoopBack();

private:
	/* @breif libpcap���I�[�v���������̃n���h�� */
	void					*m_pd;
	/* @breif libpcap */
	XDlLorder				m_libPcap;
	// �֐��̃A�h���X
	void*					m_p_pcap_open_offline;
	void*					m_p_pcap_loop;
	void*					m_p_pcap_close;

	void*					m_p_pcap_lookupdev;
	void*					m_p_pcap_open_live;
	void*					m_p_pcap_dump_open;
	void*					m_p_pcap_snapshot;
	void*					m_p_pcap_lookupnet;
	void*					m_p_pcap_compile;
	void*					m_p_pcap_setfilter;
	void*					m_p_pcap_geterr;
	void*					m_p_pcap_dump_close;
	void*					m_p_pcap_findalldevs;
friend class packetSeq;
};





/*	@brief	NIC�C���^�[�t�F�[�X����A��M�f�[�^�̂݃L���v�`���[����
 *			libpcap(winpcap)��K�v�Ƃ��Ȃ�
 */
class XSecsNICapParserImple : public XSecsPcapParser
{
public:
	/*	@brief	�R���X�g���N�^	 */
	XSecsNICapParserImple();
	/**	@brief	�f�X�g���N�^	 */
	virtual ~XSecsNICapParserImple();
	/*	@brief	�_���v�t�@�C���̃p�X���w�肵�āA�ǂݍ���
	 *
	 *	@param	pFilePath	�_���v�t�@�C���̃p�X
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool parse(const char* pFilePath);

	virtual bool parseLive(int iDev, const char* pParam, int iSnapLen);


	virtual void closeLive();

	/**	@brief	�V���A���C�Y���ꂽ�f�[�^��ǂݍ��݂܂��B
	 *
	 *	@param	stream	�X�g���[��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool serializeLoad(const char* pFilePath);

	virtual void setDeltaTime(int iDeltaTime);
protected:
	virtual void liveLoopBack();
private:

	bool parseOption(const char* pParam);

	typedef struct _option {
		XBIN	m_mode;	// 0x01 = Filter IP. 0x02 = Filter Port
		unsigned long	m_filterIP;
		unsigned short	m_filterPort;
	}option;
	option*				m_pOpts;
	int					n_nNumOpt;
	int	m_iSnapLen;
	int	m_sock;
};



#include "xlib/xlib_tail.h"
#endif /* __X_SECS_PCAP_PARSER_HEASER_FILE__ */
