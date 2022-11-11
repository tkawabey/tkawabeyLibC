/**
 * @file xsecs_text_parser.h
 * @brief 半導体製造SEMI規格 SECSデータ操作クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_SECS_TEXT_PARSER_HEASER_FILE__
#define __X_SECS_TEXT_PARSER_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"
#include "xlib/semi/secs/xsecs.h"

class XLIB_API XSecsTextParser
{
public:
	XSecsTextParser();
	virtual ~XSecsTextParser();

	virtual bool parseFromFile(const XStrBuff& strFilePath, XSecsData* pData);
	virtual bool parse(TCHAR* *pBuff, size_t &data_size, XSecsData* pData);
	bool parseBinaryFromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData);
	bool parseDigitFromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData);
	bool parseBoolromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData);
	bool parseStringFromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData);

	static bool parseFromString(const TCHAR* pData, size_t datalen, XRichData *pNewData);
	XStrBuff getErrorString();
protected:
	bool innerParseSmlText(TCHAR* *pData, size_t &data_size, XRichData* pObj);
	bool parseDigitOrBollFromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData);

	bool convertRealSMLString2RealString(TCHAR *pString, size_t &nSize);
	bool moveNextLine(TCHAR** p, size_t &size);
	bool moveNextValue(TCHAR** p, size_t &size);
	bool movePointer(TCHAR** p, size_t &size, size_t mv_size);
	bool movePointer(TCHAR* *ppData, size_t &size, const TCHAR* lpszC);
	bool movePointerUnSPACE(TCHAR* *ppData, size_t &size);
protected:
	XStrBuff			m_strLastErrorString;
	unsigned long		m_ulLine;
};

#include "xlib/xlib_tail.h"
#endif /* __X_SECS_TEXT_PARSER_HEASER_FILE__ */
