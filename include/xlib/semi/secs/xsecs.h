/**
 * @file xsecs.h
 * @brief 半導体製造SEMI規格 SECSデータ操作クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_SECS_HEASER_FILE__
#define __X_SECS_HEASER_FILE__

#include "xlib/xlib_head.h"

#include "xlib/xtime.h"
#include "xlib/xbuff.h"
#include "xlib/xdata.h"
#include "xlib/xdata_rich.h"
#include <vector>

#define XSECS_ERR_RCV				-1
#define XSECS_ERR_SND				-2
#define XSECS_ERR_SELECT_REQ		-3

#define XSECS_HEADER_SIZE			10

#define XSECS_DEVICE_ID_MAX			0x7FFF
#define XSECS_DEVICE_ID_MIN			0
#define XSECS_T3_TIMEOUT_MAX		120
#define XSECS_T3_TIMEOUT_MIN		1
#define XSECS_T5_TIMEOUT_MAX		240
#define XSECS_T5_TIMEOUT_MIN		1
#define XSECS_LINK_INTERVAL_MAX		240
#define XSECS_LINK_INTERVAL_MIN		1

#define XSECS_SECS1_MAX_BLOCK_SIZE	244
#define XSECS_SECS1_HANDSHAKE_ENQ	0x05	///< Line Bid (INQUIRE)
#define XSECS_SECS1_HANDSHAKE_EOT	0x04	///< Accept Bid (GRANT)
#define XSECS_SECS1_HANDSHAKE_ACK	0x06	///< Positive Acknowledge
#define XSECS_SECS1_HANDSHAKE_NAK	0x0F	///< Negative Acknowledge
#define XSECS_SECS1_MAX_BLOCK_COUNT	0x7FFF


/** @brief SECSメッセージヘッダ */
typedef struct XSECSHeader {
	/** @brief sessionID */
	unsigned short	sessionID;
	/** @brief header Byte 1 */
	unsigned char   byte2;
	/** @brief header Byte 2 */
	unsigned char   status;
	/** @brief PType */
	unsigned char   ptype;
	/** @brief SType */
	unsigned char	stype;
	/** @brief source id */
	unsigned char	src_id[2];
	/** @brief transaction id */
	unsigned char	tr_id[2];
	///// System byte */
	//unsigned char	systembyte[4];
}*LPXSECSHeader;

/** @brief SECSメッセージ構造体 */
typedef struct XSECSMSG
{
	/** @brief SECSメッセージヘッダ */
	XSECSHeader		header;
	/** @brief データ */
	unsigned char	*pData;
	/** @brief データ */
	unsigned char	*pDataWithHead;
	/** @brief データサイズ */
	unsigned long	ulDataSize;
	/** @brief データサイズ */
	unsigned long	ulDataSizeWithHead;
}*LPXSECSMSG;


/** @brief SECS Message type*/
enum XSECS_HSMS_TYPE {
	XSECS_HSMS_TYPE_DATA,		///<	DATA
	XSECS_HSMS_TYPE_HSMS,		///<	HSMS Control
	XSECS_HSMS_TYPE_UNKNOW		///<	Unknown
};
/** @brief 通信インターフェースのタイプ	*/
enum XSECS_COMMIF_TYPE
{
		XSECS_COMMIF_TYPE_UNKNOWN	///< 不明
	,	XSECS_COMMIF_TYPE_SECS		///< SECS
	,	XSECS_COMMIF_TYPE_HSMS		///< HSMS
	,	XSECS_COMMIF_TYPE_API		///< API
};

/** @brief value of SType */
enum XSECS_SType{
	XSECS_STYPE_DATA = 0,			///< Data Message
	XSECS_STYPE_SELECT_REQ = 1,		///< Select.Req
	XSECS_STYPE_SELECT_RSP = 2,		///< Select.Rep
	XSECS_STYPE_DELETE_REQ = 3,		///< Delete.Req
	XSECS_STYPE_DELETE_RSP = 4,		///< Delete.Rep
	XSECS_STYPE_LINK_REQ = 5,		///< Linktest.Req
	XSECS_STYPE_LINK_RSP = 6,		///< Linktest.Rep
	XSECS_STYPE_REJECT_REQ = 7,		///< Reject.Req
	XSECS_STYPE_SEPARATE_REQ = 9,	///< Separate.Req
};
enum XSECS_CONN_STS
{
	XSECS_CONN_STS_NOT_CONNECTED,		///<	未接続
	XSECS_CONN_STS_CONNECTING,			///<	接続中
	XSECS_CONN_STS_SELECT_ACCEPT,		///<	Select.req	を受け付けた
	XSECS_CONN_STS_CONNECTED,			///<	接続済み
	XSECS_CONN_STS_DISCONNECT,			///<	切断した
	XSECS_CONN_STS_ACCEPTING,			///<	クライアント接続町
};

/**	@brief	送信ステータス */
enum XSECS_SEND_STS {
	XSECS_SEND_STS_S = 0,				///< 成功：送信した
	XSECS_SEND_STS_S_AND_HAS_REP,		///< 成功：送信して、レスポンスを受信した
	XSECS_SEND_STS_S_SPOOL,				///< 成功：送信データはスプールされた
	XSECS_SEND_STS_F_NOT_CONNECTED = 10,///< 失敗：未接続だった
	XSECS_SEND_STS_F_TIMEOUT,			///< 失敗：タイムアウト
	XSECS_SEND_STS_F_DELIV,				///< 失敗：配信エラー
	XSECS_SEND_STS_F_DATA,				///< 失敗：データエラー
	XSECS_SEND_STS_F_UNINIT,			///< 失敗：未初期化
	XSECS_SEND_STS_F_NOT_FOUND_T,		///< 失敗：チケットが見つからない
	XSECS_SEND_STS_F_BUILD,				///< 失敗：送信メッセージ組み立て時にエラーが発生
	XSECS_SEND_STS_F_INQ_SREAM,			///< 失敗：マルチブロック問い合わせの2次メッセージのストリームが期待値でない
	XSECS_SEND_STS_F_INQ_FUNC,			///< 失敗：マルチブロック問い合わせの2次メッセージのファンクションが期待値でない
	XSECS_SEND_STS_F_INQ_DATA,			///< 失敗：マルチブロック問い合わせの2次メッセージのデータが期待値でない
	XSECS_SEND_STS_F_INQ_NOT_GRANT,		///< 失敗：マルチブロック問い合わせの2次メッセージのGRANTが拒否された
};



class XLIB_API XSecsData;
class XLIB_API XSecsHSMSLog;

DLL_EXTERN template class XLIB_API std::vector<XSecsData*>;
DLL_EXTERN template class XLIB_API std::vector<XSecsHSMSLog*>;

typedef XLIB_API std::vector<XSecsData*> XSecsDatas;
typedef XLIB_API std::vector<XSecsHSMSLog*> XSecsHSMSLogs;


/**	@brief	SECSデータクラス	*/
class XLIB_API XSecsData : public XDataCoder
{
public:
	/**	@brief	メンバー関数の引数に渡す、バイナリーモード */
	enum BIN_MODE {
		BODY,				///< body only
		HEAD_AND_BODY,		///< secs header + body
		SIZE_HEAD_AND_BODY,	///< dat_size + secs header + body
	};

	/**	@brief	construction */
	XSecsData();
	XSecsData(XD_UINT1 stream, XD_UINT1 function, bool bWait);
	/**	@brief	destruction */
	virtual ~XSecsData();
// operation
	/**	@brief	The clear function	 */
	virtual void clear();
	/**	@brief	add chiled data
	 *	@param	type	data type
	 *	@return	new item address. if error occourd, return NULL.
	 */
	virtual XRichData* addChiled(XDATA_TYPE type);
	/**	@brief	get chiled data
	 *	@param	index	index
	 *	@return	chiled data. if invalid index, return NULL.
	 */
	virtual XRichData* getChiled(XD_UINT4 index);


	/**	@brief	子リストにアイテムをセットします。
	 *
	 *	@param	pItem	セットするアイテム
	 *	@param	pPreItem	このアイテムで指定した位置の後に追加します。最後にアイテムを追加する場合は、NULLを指定します。
	 *
	 *	@return	true:成功/false:失敗
	 */
	virtual bool atachChild(XRichData* pItem, XRichData* pPreItem = NULL );
	/**	@brief	子リストからアイテムを解除します。子アイテムのメモリは削除しません。
	 *
	 *	@param	pItem	リストから解除するアイテム
	 *
	 *	@return	true:成功/false:失敗	
	 */
	virtual bool detachChild(XRichData* pItem);


	/**	@brief	remove chiled item
	 *	@param	pData	remove item
	 *	@retval	true	succeeded
	 *	@retval	false	failure
	 */
	virtual bool removeChiled(XRichData* pData);
	/**	@brief	get chile count
	 *	@return	chile count
	 */
	virtual XD_UINT4 getChiledCount();

	virtual void setResponseHeader( XSECSHeader *pHeader );
	virtual bool buildBinaryData(std::ostream &stream, BIN_MODE mode);
	virtual bool setFromBinary(XBIN* pRawData, XD_UINT4 len, BIN_MODE mode);
	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream,  XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));
// property
	XD_UINT2 get_session();
	void put_session(XD_UINT2 val);
	XD_UINT1 get_stream();
	void put_stream(XD_UINT1 val);
	XD_UINT1 get_function();
	void put_function(XD_UINT1 val);
	bool get_wait();
	void put_wait(bool val);
	
	XD_UINT2 getSourceId();
	void putSourceId(XD_UINT2 val);
	
	XD_UINT2 get_ticket();
	void put_ticket(XD_UINT2 val);

	
	XSECSHeader& get_header();


	static bool get_rbit(XSECSHeader& head);
	static void set_rbit(XSECSHeader& head, bool val);
	static bool get_ebit(XSECSHeader& head);
	static void set_ebit(XSECSHeader& head, bool val);
	static bool get_wait(XSECSHeader& head);
	static void set_wait(XSECSHeader& head, bool val);
	
	static XD_UINT2 get_deviceID(XSECSHeader& head);
	static void     set_deviceID(XSECSHeader& head, XD_UINT2 val);
	static XD_UINT1 get_stream(XSECSHeader& head);
	static void     set_stream(XSECSHeader& head, XD_UINT1 val);
	static XD_UINT1 get_function(XSECSHeader& head);
	static void     set_function(XSECSHeader& head, XD_UINT1 val);
	static XD_UINT2 get_blockNo(XSECSHeader& head);
	static void     set_blockNo(XSECSHeader& head, XD_UINT2 val);
	static XD_UINT2 get_source_id(XSECSHeader& head);
	static void     set_source_id(XSECSHeader& head, XD_UINT2 val);
	static XD_UINT2 get_ticket(XSECSHeader& head);
	static void     set_ticket(XSECSHeader& head, XD_UINT2 val);
	static void setSxFx(XSECSHeader& head, XD_UINT1 stream, XD_UINT1 function, bool bwait, XD_UINT2 thiket = 0);
	static void generateHostCode(XSECSHeader& head);
	static void generateNetWorkCode(XSECSHeader& head);
private:
	bool innerSetFromBinary(XDataReader &reader, XData &data, XRichData* pItem);
protected:
	/*	@brief	SECS Header */
	XSECSHeader					m_header;
	/*	@brief	SECS Data */
	XRichData					m_datas;
};




class XLIB_API XSecsHSMSLog : public XSecsData
{
public:
	XSecsHSMSLog();
	virtual ~XSecsHSMSLog();

	
protected:
	XTime			m_time;
//	Direction		m_directionSrc;
//	Direction		m_directionDst;
	XSECS_HSMS_TYPE	m_hsmsType;
};

#include "xlib/xlib_tail.h"
#endif /* __X_SECS_HEASER_FILE__ */
