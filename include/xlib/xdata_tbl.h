/**
 * @file xdata_tbl.h
 * @brief テーブル形式のデータを扱うヘルパークラスア
 *
 * @author T.Fujiha
 * @date 2010-04-15
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DATA_TBL_HEASER_FILE__
#define __X_DATA_TBL_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"
#include "xlib/xtime.h"
#include "xlib/xdata.h"
#include "xlib/xdata_rich.h"
#include "xlib/xeception.h"


#include <iostream>
#include <fstream>


/**
 *	@brief	テーブル形式のデータを扱うベースクラス
 */
class XLIB_API XDataTblBase
{
public:
	XDataTblBase();
	virtual ~XDataTblBase();

	virtual int getColumnCount() = 0;
	virtual XDATA_TYPE getColumnType(int index) = 0;
	virtual bool getColumnName(int index, XStrBuff &buf) = 0;

	virtual bool next() = 0;

	virtual bool setText(int index, const char* val) = 0;
	virtual bool setText(int index, const wchar_t* val) = 0;
	virtual bool setBool(int index, bool val) = 0;
	virtual bool setInt1(int index, XD_INT1 val) = 0;
	virtual bool setUInt1(int index, XD_UINT1 val) = 0;
	virtual bool setInt2(int index, XD_INT2 val) = 0;
	virtual bool setUInt2(int index, XD_UINT2 val) = 0;
	virtual bool setInt4(int index, XD_INT4 val) = 0;
	virtual bool setUInt4(int index, XD_UINT4 val) = 0;
	virtual bool setInt8(int index, XD_INT8 val) = 0;
	virtual bool setUInt8(int index, XD_UINT8 val) = 0;
	virtual bool setFloat4(int index, XD_FLOAT4 val) = 0;
	virtual bool setFloat8(int index, XD_FLOAT8 val) = 0;
	virtual bool setBlob(int index, void* val, int size) = 0;
	virtual bool setDate(int index, const XTime* val) = 0;
	virtual bool setTime(int index, const XTime* val) = 0;
	virtual bool setTimeStamp(int index, const XTime* val) = 0;
	virtual bool setNull(int index) = 0;
	virtual bool setRData(int index, XRichData* val) = 0;

	virtual bool getText(int index, XStrBuff &buf) = 0;
	virtual bool getBool(int index, bool* val) = 0;
	virtual bool getInt1(int index, XD_INT1* pVal) = 0;
	virtual bool getUInt1(int index, XD_UINT1* pVal) = 0;
	virtual bool getInt2(int index, XD_INT2* pVal) = 0;
	virtual bool getUInt2(int index, XD_UINT2* pVal) = 0;
	virtual bool getInt4(int index, XD_INT4* pVal) = 0;
	virtual bool getUInt4(int index, XD_UINT4* pVal) = 0;
	virtual bool getInt8(int index, XD_INT8* pVal) = 0;
	virtual bool getUInt8(int index, XD_UINT8* pVal) = 0;
	virtual bool getFloat4(int index, XD_FLOAT4* pVal) = 0;
	virtual bool getFloat8(int index, XD_FLOAT8* pVal) = 0;
	virtual bool getBlob(int index, XBuff& buf, int* p_size) = 0;
	virtual bool getDate(int index, XTime* val) = 0;
	virtual bool getTime(int index, XTime* val) = 0;
	virtual bool getTimeStamp(int index, XTime* val) = 0;
	virtual bool isNull(int index, bool* pVal) = 0;
	virtual bool getRData(int index, XRichData* val) = 0;


};




/**
 *	@brief	CSV,TAB区切りテキストファイルのテーブルを扱うクラス。
 */
class XLIB_API XDataTblFs : public XDataTblBase
{
public:
	/** @brief	フォーマット */
	enum FsFormat {
		FsFormat_CSV	,	///< CSVファイル
		FsFormat_TAB	,	///< TAB区切りのデータファイル
	};

	/** カラムのスキーマ構造体 */
	typedef struct tagClmnInf
	{
		XDATA_TYPE		dataType;		///<	データタイプ
		XStrBuff		strClmnName;	///<	カラム名
	}ClmnInf;


	XDataTblFs();
	virtual ~XDataTblFs();

	virtual bool open(const char* pFileName, bool bFirstClumnName, FsFormat format = FsFormat_CSV, bool bWriteMode = false);
	virtual bool close();
	virtual void setClmnsInf(int cnt, ClmnInf* pClmns);
	


// over write XDataTblBase
	virtual int getColumnCount();
	virtual XDATA_TYPE getColumnType(int index);
	virtual bool getColumnName(int index, XStrBuff &buf);

	virtual bool next();


	virtual bool setText(int index, const char* val);
	virtual bool setText(int index, const wchar_t* val);
	virtual bool setBool(int index, bool val);
	virtual bool setInt1(int index, XD_INT1 val);
	virtual bool setUInt1(int index, XD_UINT1 val);
	virtual bool setInt2(int index, XD_INT2 val);
	virtual bool setUInt2(int index, XD_UINT2 val);
	virtual bool setInt4(int index, XD_INT4 val);
	virtual bool setUInt4(int index, XD_UINT4 val);
	virtual bool setInt8(int index, XD_INT8 val);
	virtual bool setUInt8(int index, XD_UINT8 val);
	virtual bool setFloat4(int index, XD_FLOAT4 val);
	virtual bool setFloat8(int index, XD_FLOAT8 val);
	virtual bool setBlob(int index, void* val, int size);
	virtual bool setDate(int index, const XTime* val);
	virtual bool setTime(int index, const XTime* val);
	virtual bool setTimeStamp(int index, const XTime* val);
	virtual bool setNull(int index);
	virtual bool setRData(int index, XRichData* val);

	virtual bool getText(int index, XStrBuff &buf);
	virtual bool getBool(int index, bool* val);
	virtual bool getInt1(int index, XD_INT1* pVal);
	virtual bool getUInt1(int index, XD_UINT1* pVal);
	virtual bool getInt2(int index, XD_INT2* pVal);
	virtual bool getUInt2(int index, XD_UINT2* pVal);
	virtual bool getInt4(int index, XD_INT4* pVal);
	virtual bool getUInt4(int index, XD_UINT4* pVal);
	virtual bool getInt8(int index, XD_INT8* pVal);
	virtual bool getUInt8(int index, XD_UINT8* pVal);
	virtual bool getFloat4(int index, XD_FLOAT4* pVal);
	virtual bool getFloat8(int index, XD_FLOAT8* pVal);
	virtual bool getBlob(int index, XBuff& buf, int* p_size);
	virtual bool getDate(int index, XTime* val);
	virtual bool getTime(int index, XTime* val);
	virtual bool getTimeStamp(int index, XTime* val);
	virtual bool isNull(int index, bool* pVal);
	virtual bool getRData(int index, XRichData* val);

protected:
	XStrBuff *getWriteData(int index);
protected:
	FsFormat		m_format;
	XStrBuffSplit	*m_pClmns;
	std::iostream 	*m_pFs;
	bool			m_bNewFs;
	XStrBuffSplit	m_lineDatas;
	XD_UINT4		m_nLine;
	bool			m_bOnErrorThrow;
	XStrBuff		m_strSystem;
	bool			m_bWriteMode;
	XList			m_cntWriteDatas;
	XList			m_cntClmnSchemas;
};

#include "xlib/xlib_tail.h"

#endif /*__X_DATA_TBL_HEASER_FILE__*/