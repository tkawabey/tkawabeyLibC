/**
 * @file xsemi_xml.h
 * @brief îºì±ëÃêªë¢SEMIãKäiÅ@XML
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_XML_H__
#define __XSEMI_XML_H__

#include "xlib/xstrbuff.h"
#include "xlib/xsocket.h"
#include "xlib/xlock.h"
#include "xlib/xarray.h"
#include "xlib/xtime.h"
#include "xlib/xbuff.h"
#include "xlib/xdata.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/xlib_head.h"


class XLIB_API XSEMI_XmlParser 
{
public:
	XSEMI_XmlParser();
	virtual ~XSEMI_XmlParser();

	virtual bool loadStyle(const TCHAR* pszStyleFile);
	virtual void unloadStyle();

	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream, XSECSHeader* pHeader, XBIN* pMsg, XD_UINT4 ulSize, const TCHAR* pFrom, const TCHAR* pTo, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));
	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream, XSecsData &data, const TCHAR* pFrom, const TCHAR* pTo, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));
private:
	virtual bool printInner(XLIB_STD_OUT_STR_STREAM &stream, XDataReader& reader, XData& data, void* pDomNode,  XStrBuff &blank, XD_INT4 nDepth, const TCHAR *pLF);
	virtual bool printInnerNoStyle(XLIB_STD_OUT_STR_STREAM &stream, XDataReader& reader, XData& data, XStrBuff &blank, XD_INT4 nDepth, const TCHAR *pLF);
	virtual bool printInnerValue(XLIB_STD_OUT_STR_STREAM &stream, XData& data, const TCHAR *pLF);
	virtual bool printInnerType(XLIB_STD_OUT_STR_STREAM &stream, XData& data);
protected:
	XMutex	m_lock;

	void*	m_pDomParser;
	void*	m_pItemTypeNode;
	void*	m_pMessageNode;


	struct MessageMap
	{
		XD_UINT4	sf;
		void*		pDomNode;
	};
	MessageMap* m_pMsgMap;
	XBuff	m_msgMapBuff;
	XD_UINT4 m_ulNumOfMsg;


};


#include "xlib/xlib_tail.h"
#endif /* __XSEMI_XML_H__ */
