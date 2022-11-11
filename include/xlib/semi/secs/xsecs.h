/**
 * @file xsecs.h
 * @brief �����̐���SEMI�K�i SECS�f�[�^����N���X
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


/** @brief SECS���b�Z�[�W�w�b�_ */
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

/** @brief SECS���b�Z�[�W�\���� */
typedef struct XSECSMSG
{
	/** @brief SECS���b�Z�[�W�w�b�_ */
	XSECSHeader		header;
	/** @brief �f�[�^ */
	unsigned char	*pData;
	/** @brief �f�[�^ */
	unsigned char	*pDataWithHead;
	/** @brief �f�[�^�T�C�Y */
	unsigned long	ulDataSize;
	/** @brief �f�[�^�T�C�Y */
	unsigned long	ulDataSizeWithHead;
}*LPXSECSMSG;


/** @brief SECS Message type*/
enum XSECS_HSMS_TYPE {
	XSECS_HSMS_TYPE_DATA,		///<	DATA
	XSECS_HSMS_TYPE_HSMS,		///<	HSMS Control
	XSECS_HSMS_TYPE_UNKNOW		///<	Unknown
};
/** @brief �ʐM�C���^�[�t�F�[�X�̃^�C�v	*/
enum XSECS_COMMIF_TYPE
{
		XSECS_COMMIF_TYPE_UNKNOWN	///< �s��
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
	XSECS_CONN_STS_NOT_CONNECTED,		///<	���ڑ�
	XSECS_CONN_STS_CONNECTING,			///<	�ڑ���
	XSECS_CONN_STS_SELECT_ACCEPT,		///<	Select.req	���󂯕t����
	XSECS_CONN_STS_CONNECTED,			///<	�ڑ��ς�
	XSECS_CONN_STS_DISCONNECT,			///<	�ؒf����
	XSECS_CONN_STS_ACCEPTING,			///<	�N���C�A���g�ڑ���
};

/**	@brief	���M�X�e�[�^�X */
enum XSECS_SEND_STS {
	XSECS_SEND_STS_S = 0,				///< �����F���M����
	XSECS_SEND_STS_S_AND_HAS_REP,		///< �����F���M���āA���X�|���X����M����
	XSECS_SEND_STS_S_SPOOL,				///< �����F���M�f�[�^�̓X�v�[�����ꂽ
	XSECS_SEND_STS_F_NOT_CONNECTED = 10,///< ���s�F���ڑ�������
	XSECS_SEND_STS_F_TIMEOUT,			///< ���s�F�^�C���A�E�g
	XSECS_SEND_STS_F_DELIV,				///< ���s�F�z�M�G���[
	XSECS_SEND_STS_F_DATA,				///< ���s�F�f�[�^�G���[
	XSECS_SEND_STS_F_UNINIT,			///< ���s�F��������
	XSECS_SEND_STS_F_NOT_FOUND_T,		///< ���s�F�`�P�b�g��������Ȃ�
	XSECS_SEND_STS_F_BUILD,				///< ���s�F���M���b�Z�[�W�g�ݗ��Ď��ɃG���[������
	XSECS_SEND_STS_F_INQ_SREAM,			///< ���s�F�}���`�u���b�N�₢���킹��2�����b�Z�[�W�̃X�g���[�������Ғl�łȂ�
	XSECS_SEND_STS_F_INQ_FUNC,			///< ���s�F�}���`�u���b�N�₢���킹��2�����b�Z�[�W�̃t�@���N�V���������Ғl�łȂ�
	XSECS_SEND_STS_F_INQ_DATA,			///< ���s�F�}���`�u���b�N�₢���킹��2�����b�Z�[�W�̃f�[�^�����Ғl�łȂ�
	XSECS_SEND_STS_F_INQ_NOT_GRANT,		///< ���s�F�}���`�u���b�N�₢���킹��2�����b�Z�[�W��GRANT�����ۂ��ꂽ
};



class XLIB_API XSecsData;
class XLIB_API XSecsHSMSLog;

DLL_EXTERN template class XLIB_API std::vector<XSecsData*>;
DLL_EXTERN template class XLIB_API std::vector<XSecsHSMSLog*>;

typedef XLIB_API std::vector<XSecsData*> XSecsDatas;
typedef XLIB_API std::vector<XSecsHSMSLog*> XSecsHSMSLogs;


/**	@brief	SECS�f�[�^�N���X	*/
class XLIB_API XSecsData : public XDataCoder
{
public:
	/**	@brief	�����o�[�֐��̈����ɓn���A�o�C�i���[���[�h */
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


	/**	@brief	�q���X�g�ɃA�C�e�����Z�b�g���܂��B
	 *
	 *	@param	pItem	�Z�b�g����A�C�e��
	 *	@param	pPreItem	���̃A�C�e���Ŏw�肵���ʒu�̌�ɒǉ����܂��B�Ō�ɃA�C�e����ǉ�����ꍇ�́ANULL���w�肵�܂��B
	 *
	 *	@return	true:����/false:���s
	 */
	virtual bool atachChild(XRichData* pItem, XRichData* pPreItem = NULL );
	/**	@brief	�q���X�g����A�C�e�����������܂��B�q�A�C�e���̃������͍폜���܂���B
	 *
	 *	@param	pItem	���X�g�����������A�C�e��
	 *
	 *	@return	true:����/false:���s	
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
