/**
 * @file xsecs_pcap_parser.h
 * @brief 半導体製造SEMI規格 TCPDUMPからSECSデータを抽出するクラス
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


/**	@brief	PCAPデータクラス。ユーザーコールバック関数で渡される	*/
class XLIB_API XSecsPcapData
{
public:
	/**	@brief	コンストラクタ	 */
	XSecsPcapData();
	/**	@brief	デストラクタ
	 */
	virtual ~XSecsPcapData();


	/**	@brief	ストリームにインスタンス情報を文字列で出力
	 *
	 *	@param	stream	ストリーム
	 *	@param	nDepth	インデントの深さ
	 *	@param	pLF	改行コード
	 *	@param	bTCPIPheader	TCPIPヘッダーを表示するかどうか
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"), bool bTCPIPheader = false);

	bool printDump(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"), bool bTCPIPheader = false);
	
	/**	@brief	シリアライズされたデータを書き込みます。
	 *
	 *	@param	stream	ストリーム
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool serializeStore(std::ostream &stream);

	/**	@brief	シリアライズされたデータを読み込みます。
	 *
	 *	@param	stream	ストリーム
	 *	@param	buff	バッファ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool serializeLoad(std::istream &stream, XBuff &buff);
	
	
	/**	@brief	データ長
	 */
	void debugDump();
	/**	@brief	インスタンスのコピー
	 *
	 *	@param	right	コピー元インスタンス
	 */
	XSecsPcapData& operator = (const XSecsPcapData& right);

	/**	@brief	時間*/
	struct tm		m_Time;
	/**	@brief	時間．ナノ秒*/
	unsigned long	m_ulTimeUSec;
	/**	@brief	送信者IP*/
	struct in_addr	m_ip_src;
	/**	@brief	受信者IP*/
	struct in_addr	m_ip_dst;
	/**	@brief	送信者Port*/
	XD_UINT2		m_port_src;
	/**	@brief	受信者Port*/
	XD_UINT2		m_port_dst;
	/**	@brief	Sequenceナンバー*/
	XD_UINT4		m_th_seq;
	/**	@brief	Ackナンバー*/
	XD_UINT4		m_th_ack;
	/**	@brief	TCPフラグ*/
	XD_UINT1		m_th_flags;
	/**	@brief	ウインドウ*/
	XD_UINT2		m_th_win;
	/**	@brief	TCPチェックサム値*/
	XD_UINT2		m_th_sum;
	/**	@brief	TCP緊急データ*/
	XD_UINT2		m_th_urp;
	/**	@brief	データ長*/
	XD_UINT4		m_length;
	/**	@brief	前のパケットのデータ長さ*/
	XD_UINT4		m_lengthPrev;
	/**	@brief	データ*/
	XBIN*	m_pData;

	bool			m_bPrintHeaderOnly;

	/**	@brief	データの種類*/
	XSECS_HSMS_TYPE	m_type;
	/**	@brief	データの種類がSECSまたは、HSMSの場合、SECSヘッダー情報*/
	XSECSHeader		m_secsHeader;
	/**	@brief	データの種類がSECSの場合のデータ長*/
	XD_UINT4		m_secsDataLength;
	/**	@brief	パケットヘッダーの配列 */
	std::vector<XSecsPcapPacketHead*>	m_pcket_headers;

private:
	bool innerPrint(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"), bool bTCPIPheader = false, bool bDump = false);
};




/**	@brief	PCAPダンプ解析インターフェース */
class XLIB_API XSecsPcapParser
{
public:
	/**	@brief	コンストラクタ	 */
	XSecsPcapParser();
	/**	@brief	デストラクタ	 */
	virtual ~XSecsPcapParser();

	/**	@brief	ダンプファイルのパスを指定して、読み込み
	 *
	 *	@param	pFilePath	ダンプファイルのパス
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool parse(const char* pFilePath) = 0;

	/**	@brief	The parseLive function
	 *
	 *	@param	iDev	a parameter of type int
	 *	@param	pParam	a parameter of type const char*
	 *	@param	iSnapLen	a parameter of type int
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool parseLive(int iDev, const char* pParam, int iSnapLen) = 0;

	virtual void closeLive() = 0;

	/**	@brief	シリアライズされたデータを読み込みます。
	 *
	 *	@param	stream	ストリーム
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool serializeLoad(const char* pFilePath) = 0;

	/**	@brief	ユーザー定義コールバック関数をセットする。
	 *
	 *	@param	pCallBackFunction	コールバック関数
	 *	@param	pUserCaallBackData	コールバックに渡される、ユーザー定義のデータ
	 *	@param	pRawCallBack		Rawデータ解析用コールバック関数
	 */
	void setCallBackFunction(
		void (*pCallBackFunction)(XSecsPcapData*,void*), 
		void *pUserCaallBackData, 
		void (*pRawCallBack)(XSecsPcapData* , void *, void *, unsigned int ,void* , unsigned int ) = NULL);



	virtual void setDeltaTime(int iDeltaTime) = 0;
	/**	@brief	インスタンスを作成
	 *
	 *	@return	インスタンス
	 */
	static XSecsPcapParser* newInstance();
	static XSecsPcapParser* newInstanceNICap();
	/**	@brief	インスタンスを開放
	 *
	 *	@param	ppParser	インスタンスのアドレス。
	 */
	static void deleteInstance(XSecsPcapParser** ppParser);



protected:
	/*	@brief	解析ステータス	*/
	enum PACKET_STATUS {
		PACKET_STATUS_INIT,				//<	初期状態
		PACKET_STATUS_NEED_CONTINUE,	//<	マルチパケットの途中
		PACKET_STATUS_MULTI_SECS_MSG,	//<	１パケットに複数データあり
		PACKET_STATUS_SKIP,				//<	無視
	};

	/*	@brief	シーケンス確認データクラス	*/
	class packetSeq
	{
	public:
		packetSeq(XSecsPcapData &src) {
			m_comp = src;
			m_comp.m_length = 0;
			m_status = PACKET_STATUS_INIT;
			m_ulSackRequireCounter = 0;
		}
		/*	@brief	このインスタンスのデータにTrancでデータを書き込む
		 *
		 *	@param	pBuff	書き込むデータ
		 *	@param	iSize	書き込むデータのサイズ
		 *	@return	データの先頭アドレス。
		 */
		XBIN* newSetBuff(const XBIN* pBuff, int iSize);
		/*	@brief	このインスタンスのデータにAppendでデータを書き込む
		 *
		 *	@param	pBuff	書き込むデータ
		 *	@param	iSize	書き込むデータのサイズ
		 *	@return	データの先頭アドレス。
		 */
		XBIN* appendBuff(const XBIN* pBuff, int iSize);
		/*	@brief	データを指定したサイズ前へ移動する。
		 *	@param	iSize	移動するサイズ
		 *	@return	データの先頭アドレス。
		 */
		XBIN* moveOverBeforeBuff(size_t iSize);
		
		/*	@brief	データ長ｗ返す
		 *	@return	データ長
		 */
		XD_UINT4 size(){ return m_buffer.size(); }


		/*	@brief	SACKアイテムを過去に受信している場合、そのSACKアイテムを取得する。
		 *
		 *	@param	ulSeq	取得するSACKのSEQとSEQ+Lengthの収まるシーケンスナンバー
		 *
		 *	@retval	NULL	SACKアイテムがレンジ内に見つからなかった
		 *	@retval	NULL以外	見つかったSACKアイテム	
		 *	
		 */
		packetSeq* findSack(unsigned long ulSeq);

		/*	@brief	ヘッダーを追加する。
		 *
		 *	@param	pEt	Etherヘッダー
		 *	@param	pIP	IPヘッダー
		 *	@param	ulIPLen	IPヘッダー長
		 *	@param	pTCP	TCPヘッダーヘッダー
		 *	@param	ulTCPLen	TCPヘッダー長
		 *	@param	ulLen	データ長さ
		 *
		 *	@retval	true	成功
		 *	@retval	false	失敗
		 */
		bool addHeader(XBIN* pEt, XBIN* pIP, unsigned long ulIPLen, XBIN* pTCP, unsigned long ulTCPLen, unsigned long ulLen);

		/*	@brief	ヘッダー情報を空にする。 */
		void clearHeaders();

		/**	@brief	ヘッダー情報の先頭を取得
		 *
		 *	@return	ヘッダー情報。NULLの場合ヘッダー情報がない。
		 */
		XSecsPcapPacketHead* getTopHead();


		/* @brief パケット情報 */
		XSecsPcapData				m_comp;
		/* @brief バッファ */
		XInflateStreamBuff			m_buffer;
		/* @brief バッファ */
		XInflateStreamBuff			m_bufferSwap;
		/* @brief ステータス */
		PACKET_STATUS				m_status;
		/* @brief SACK発生時に、以前のパケットを貯めて置くためのデータ */
		std::vector<packetSeq*>		m_sacksDataItems;
		/* @brief マルチパケットの中間ACKを貯めて置くためのデータ */
		std::vector<packetSeq*>		m_acksDataItems;
		unsigned long m_ulSackRequireCounter;
	};




protected:
	virtual void liveLoopBack() = 0;

	void inner(XSecsPcapData& pcapData, void *pEtherHeader, void *pIP, unsigned int hlen_ip,
		void* pTCP, unsigned int hlen_tcp);




	/*	@brief	通信の向きが同じシーケンスアイテムを見つける
	 *
	 *	@param	pPcapData	通信の向き（IP&ポート）がセットされた、XSecsPcapDataインスタンス
	 *	@return	見つからない場合は、NULL。
	 */
	packetSeq* findPacketSeq(XSecsPcapData *pPcapData);
	/*	@brief	The 通信の向きが逆のシーケンスアイテムを見つける function
	 *
	 *	@param	pPcapData		通信の向き（IP&ポート）がセットされた、XSecsPcapDataインスタンス
	 *	@return	見つからない場合は、NULL。
	 */
	packetSeq* findDstPacketSeq(XSecsPcapData *pPcapData);



	/**	@brief	TCPオプションにSACKの値が入っているか調べます。
	 *
	 *	@param	pTCPHeader	TCPヘッダーの先頭アドレス
	 *	@param	ulTCPHeader	TCPヘッダー長さ
	 *	@param	pSacksValue	SACK値を受け取るアドレス
	 *	@param	pSacksCount	SACK値がある場合、Start:Endの組み合わせ数
	 *
	 *	@retval	true	有
	 *	@retval	false	無
	 */
	static bool foundSackValues(XBIN* pTCPHeader, unsigned long ulTCPHeader, unsigned long** pSacksValue, unsigned long *pSacksCount);

	/*	@brief	パケットのデータ部分を解析します。
	 *
	 *	@param	pcapData	解析結果を入れるバッファ
	 *	@param	pPack	解析元データ
	 *
	 *	@return	解析ステータス
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
	/*	@brief	ユーザー定義コールバック関数*/
	void		(*m_pCallBackFunction)(XSecsPcapData*,void*);
	/*	@brief	Rawデータ解析用コールバック*/
	void (*m_pRawCallBack)(XSecsPcapData* , void *, void *, unsigned int ,void* , unsigned int );

	/*	@brief	コールバックに渡される、ユーザー定義のデータ。*/
	void*		m_pUserCaallBackData;
	/*	@brief 時間表示時のデルタ時間	*/
	int						m_deltaTime;
	/* @brief	リアルタイム取得用スレッド */
	AnyThread*				m_pLinveThread;
	/* @breif シーケンス確認データ配列 */
	std::vector<packetSeq*>	m_packets;

friend class packetSeq;
};






// 以下実装クラス


/*	@brief	パケットのヘッダ情報*/
class XLIB_API XSecsPcapPacketHead
{
public:
	/*	@brief	コンストラクタ
	 *
	 *	@param	pEt	Etherヘッダー
	 *	@param	pIP	IPヘッダー
	 *	@param	ulIPLen	IPヘッダー長
	 *	@param	pTCP	TCPヘッダーヘッダー
	 *	@param	ulTCPLen	TCPヘッダー長
	 *	@param	ulLen	データ長さ
	 */
	XSecsPcapPacketHead(XBIN* pEt, XBIN* pIP, unsigned long ulIPLen, XBIN* pTCP, unsigned long ulTCPLen, unsigned long ulLen);
	/*	@brief	デストラクタ	 */
	virtual ~XSecsPcapPacketHead();
	/*	@brief	ストリームにインスタンス情報を文字列で出力
	 *
	 *	@param	stream	ストリーム
	 *	@param	nDepth	インデントの深さ
	 *	@param	pLF	改行コード
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));

	/*	@brief	Etherヘッダーの先頭アドレスを取得
	 *
	 *	@return	Etherヘッダーの先頭アドレス
	 */
	XBIN* get_EtherHead();
	/*	@brief	IPヘッダーの先頭アドレスを取得
	 *
	 *	@return	IPヘッダーの先頭アドレス
	 */
	XBIN* get_IPHead();
	/*	@brief	TCPヘッダーの先頭アドレスを取得
	 *
	 *	@return	TCPヘッダーの先頭アドレス
	 */
	XBIN* get_TCPHead();
	/*	@brief	IPヘッダー長を取得
	 *
	 *	@return	IPヘッダー長
	 */
	unsigned long get_IPHeadLength();
	/*	@brief	TCPヘッダー長を取得
	 *
	 *	@return	TCPヘッダー長
	 */
	unsigned long get_TCPHeadLength();
	/*	@brief	データ長を取得
	 *
	 *	@return	データ長
	 */
	unsigned long get_DataLength();
private:
	/*	@brief	Etherヘッダー*/
	XBIN*	m_pEtherHead;
	/*	@brief	IPヘッダー*/
	XBIN*	m_pIPHead;
	/*	@brief	IPヘッダー長*/
	unsigned long	m_ulIPHeader;
	/*	@brief	TCPヘッダーヘッダー*/
	XBIN*	m_pTCPHead;
	/*	@brief	TCPヘッダー長*/
	unsigned long	m_ulTCPHeader;
	/*	@brief	データ長さ*/
	unsigned long	m_ulLen;
	/*	@brief	バッファ*/
	XBIN*	m_pBuff;
};




/*	@brief	Libpcapダンプ解析実装 */
class XSecsPcapParserImple : public XSecsPcapParser
{
public:
	/*	@brief	コンストラクタ	 */
	XSecsPcapParserImple();
	/**	@brief	デストラクタ	 */
	virtual ~XSecsPcapParserImple();
	/*	@brief	ダンプファイルのパスを指定して、読み込み
	 *
	 *	@param	pFilePath	ダンプファイルのパス
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool parse(const char* pFilePath);

	virtual bool parseLive(int iDev, const char* pParam, int iSnapLen);


	virtual void closeLive();

	/**	@brief	シリアライズされたデータを読み込みます。
	 *
	 *	@param	stream	ストリーム
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool serializeLoad(const char* pFilePath);

	virtual void setDeltaTime(int iDeltaTime);

	/*	@brief	解析内部処理
	 *
	 *	@param	pH	パケットヘッダー
	 *	@param	pkt_data	パケットデータ
	 */
	void innerParse(const void* pH, const u_char *pkt_data);


protected:
	virtual void liveLoopBack();

private:
	/* @breif libpcapをオープンした時のハンドル */
	void					*m_pd;
	/* @breif libpcap */
	XDlLorder				m_libPcap;
	// 関数のアドレス
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





/*	@brief	NICインターフェースから、受信データのみキャプチャーする
 *			libpcap(winpcap)を必要としない
 */
class XSecsNICapParserImple : public XSecsPcapParser
{
public:
	/*	@brief	コンストラクタ	 */
	XSecsNICapParserImple();
	/**	@brief	デストラクタ	 */
	virtual ~XSecsNICapParserImple();
	/*	@brief	ダンプファイルのパスを指定して、読み込み
	 *
	 *	@param	pFilePath	ダンプファイルのパス
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool parse(const char* pFilePath);

	virtual bool parseLive(int iDev, const char* pParam, int iSnapLen);


	virtual void closeLive();

	/**	@brief	シリアライズされたデータを読み込みます。
	 *
	 *	@param	stream	ストリーム
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
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
