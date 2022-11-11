#include "xlib/semi/gem/xsemi_xml.h"
#include "xlib/xeception.h"
#include "xlib/xmlhelper.h"


XSEMI_XmlParser::XSEMI_XmlParser()
	:		m_pDomParser( 0 )
	,		m_pMessageNode( 0 )
	,		m_pItemTypeNode( 0 )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XSEMI_XmlParser::~XSEMI_XmlParser()
{
XEXCEPT_ENTRY
	this->unloadStyle();
XEXCEPT_EXIT
}

bool XSEMI_XmlParser::loadStyle(const TCHAR* pszStyleFile)
{
XEXCEPT_ENTRY
	XmlHlpDocument<XmlHlpElementRawType>*	xmlDoc;
	XmlHlpErrReporter				errRepoter;
	XmlHlpNode<XmlHlpElementRawType>		pNodeTemp;
	MessageMap	*pMsgMapWk;
	XD_UINT4	ulMapCnt = 0;
	this->unloadStyle();

	xmlDoc = new XmlHlpDocument<XmlHlpElementRawType>;
	m_pDomParser = (void*)xmlDoc;


	if( xmlDoc->loadFromFile( XStrBuff(pszStyleFile), &errRepoter )  == false ) {
		return false;
	}	
	// メッセージノードを見つける
	m_pMessageNode = (void*)xmlDoc->findNode( XStrBuff("secs2/messages"));
	// アイテムノードを見つける
	m_pItemTypeNode = (void*)xmlDoc->findNode( XStrBuff("secs2/itemTypes"));
	if( m_pMessageNode == NULL || m_pItemTypeNode == NULL ) {
		return false;
	}

	pNodeTemp = (XmlHlpElementRawType*)m_pMessageNode;
	ulMapCnt = m_ulNumOfMsg = pNodeTemp.countElement();
	// メッセージマップ用メモリを確保
	pMsgMapWk = m_pMsgMap = (MessageMap*)m_msgMapBuff.Alloc( m_ulNumOfMsg * sizeof(MessageMap) );
	m_msgMapBuff.MemSet();

	pNodeTemp = pNodeTemp.firstChild();
	while( pNodeTemp != NULL ) {
		if( ulMapCnt == 0 ) {
			break;
		}

		XD_UINT4    usStream, usFunction;
		XStrBuff str;
		usStream = usFunction = 0;
		
		str = pNodeTemp.getAttribute("s");
		if( str != _T("") ) {
			usStream = atol(str);
		}
		str = pNodeTemp.getAttribute("f");
		if( str != _T("") ) {
			usFunction = atol(str);
		}


		if( usStream == 0 && usFunction == 0 ) {
			str = pNodeTemp.getAttribute("sf");
			if( str == _T("") ) { pNodeTemp = pNodeTemp.nextSiblingElement(); continue; }

			TCHAR* pSxFx = (TCHAR*)(const TCHAR*)str;
			TCHAR *pEndPtr;

			while( *pSxFx != NULL ) {
				if( '0' <= *pSxFx && *pSxFx <= '9' ) {
					break;
				}
				pSxFx++;
			}
			if( *pSxFx == NULL ) { pNodeTemp = pNodeTemp.nextSiblingElement(); continue; }

			usStream = (XD_UINT2)XStrBuff::strtoul(pSxFx, &pEndPtr, 10);

			if( pEndPtr == NULL ) { pNodeTemp = pNodeTemp.nextSiblingElement(); continue ; }
			pSxFx = pEndPtr;
			while( *pSxFx != NULL ) {
				if( '0' <= *pSxFx && *pSxFx <= '9' ) {
					break;
				}
				pSxFx++;
			}
			if( *pSxFx == NULL ) { pNodeTemp = pNodeTemp.nextSiblingElement(); continue; }
	
			usFunction = (XD_UINT2)XStrBuff::strtoul(pSxFx, &pEndPtr, 10);
		}
		pMsgMapWk->sf = ( usStream << 16 ) | usFunction;
		pMsgMapWk->pDomNode = pNodeTemp.m_pT;
		pMsgMapWk++;

		ulMapCnt--;

		pNodeTemp = pNodeTemp.nextSiblingElement();
	}
	return false;
XEXCEPT_EXIT
}

void XSEMI_XmlParser::unloadStyle()
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	XmlHlpDocument<XmlHlpElementRawType>*	pPserser = (XmlHlpDocument<XmlHlpElementRawType>*)m_pDomParser;
	m_pDomParser = NULL;
	if( pPserser != NULL ) {
		delete pPserser;
	}

	m_ulNumOfMsg = 0;
	m_pMsgMap = 0;
XEXCEPT_EXIT
}

bool XSEMI_XmlParser::print(XLIB_STD_OUT_STR_STREAM &stream, XSECSHeader* pHeader, XBIN* pMsg, XD_UINT4 ulSize,  const TCHAR* pFrom, const TCHAR* pTo, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XMutexer	locker(m_lock);
	XD_UINT4    ulSxFx, i;
	MessageMap	*pMsgMapWk = m_pMsgMap;
	MessageMap	*pMsgMapFound = NULL ;
	XStrBuff	strXerces(_T(""));
	XStrBuff blank;
	TCHAR *pBlak = NULL;

	pBlak = (TCHAR*)blank.Alloc( (nDepth+60) * sizeof(TCHAR) );
	if( pBlak == NULL ) {
		return false;
	}
	blank.memsetT(_T(' '));
	pBlak = (TCHAR*)blank;
	pBlak[nDepth] = 0;


	ulSxFx = MAKELONG( XSecsData::get_function(*pHeader), XSecsData::get_stream(*pHeader));
	for(i = 0; i < m_ulNumOfMsg; i++, pMsgMapWk++) {
		if( pMsgMapWk->sf == ulSxFx ) {
			pMsgMapFound = pMsgMapWk;
			break;
		}
	}
	if( pMsgMapFound == NULL ) {
		return false;
	}

	XmlHlpNode<XmlHlpElementRawType> pNodeMessage = (XmlHlpElementRawType*)pMsgMapFound->pDomNode;
	XmlHlpNode<XmlHlpElementRawType> pNodeBody    = pNodeMessage.findNode(XStrBuff("body"));
	if( pNodeBody.m_pT == NULL ) {
		return false;
	}

	XData       data;
	XDataReader reader(pMsg, ulSize);


	stream << (const TCHAR*)blank << _T("<Envelope>") << pLF;
	stream << (const TCHAR*)blank << _T(" <Header>") << pLF;
	if( pFrom != NULL ) {
		stream << (const TCHAR*)blank << _T("  <From>") << pFrom << _T("</From>") << pLF;
	}
	if( pTo != NULL ) {
		stream << (const TCHAR*)blank << _T("  <To>") << pTo << _T("</To>") << pLF;
	}

	stream << (const TCHAR*)blank << _T("  <MessageType>");
	int iFunction = (int)XSecsData::get_function(*pHeader);
	if( iFunction == 0 ) {
		stream << _T("fault");	
	} else 
	if( (iFunction %2) != 0 ) {
		stream << _T("Request");	
	} else {
		stream << _T("Reply");
	}
	stream << _T("</MessageType>") << pLF;
	

	if( iFunction == 0 ) {
	} else 
	if( (iFunction %2) != 0 ) {
		stream << (const TCHAR*)blank << _T("  <RequestId>") << 
			(int)XSecsData::get_ticket(*pHeader) << 
			_T("</RequestId>") << pLF;
	} else {
		stream << (const TCHAR*)blank << _T("  <CorrelationId>") << 
			(int)XSecsData::get_ticket(*pHeader) << 
			_T("</CorrelationId>") << pLF;
	}
	
	stream << (const TCHAR*)blank << _T("  <Action>");
	strXerces = pNodeMessage.getAttribute(_T("action"));
	if( strXerces != _T("") ) {
		stream << _T("S") << (int)XSecsData::get_stream(*pHeader) << 
				  _T("F") << (int)XSecsData::get_function(*pHeader);
	} else {
		stream << (const TCHAR*)strXerces;
	}
	stream << _T("</Action>") << pLF;


	stream << (const TCHAR*)blank << _T("  <ReplyExpected>") <<  (
		XSecsData::get_wait(*pHeader)== true ? _T("TRUE") : _T("FALSE")
		) << _T("</ReplyExpected>") << pLF;
//	stream << (const TCHAR*)blank << _T("  <EventIndex></EventIndex>") << pLF;
	stream << (const TCHAR*)blank << _T(" </Header>") << pLF;


	stream << (const TCHAR*)blank << _T(" <Body>") << pLF;
	stream << (const TCHAR*)blank << _T("  <data>") << pLF;

	// 最初の子ノード
	i = 0;
	pNodeBody = pNodeBody.firstChild();
	while( pNodeBody !=  NULL ) {
		if( reader.getValue( &data ) == false ) {
			break;
		}
		if( this->printInner(stream, reader, data, pNodeBody.m_pT, blank, nDepth+3, pLF) == false ) {
			break;
		}
		i++;
		pNodeBody = pNodeBody.nextSiblingElement();
	}

	blank.MemSet(' ');
	pBlak = (TCHAR*)blank;
	pBlak[nDepth] = 0;

	stream << (const TCHAR*)blank << _T("  </data>") << pLF;
	stream << (const TCHAR*)blank << _T(" </Body>") << pLF;
	stream << (const TCHAR*)blank << _T("</Envelope>") << pLF;
	return true;
XEXCEPT_EXIT
}

bool XSEMI_XmlParser::print(XLIB_STD_OUT_STR_STREAM &stream, XSecsData &data, const TCHAR* pFrom, const TCHAR* pTo, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY

	XInflateStreamBuff	buff;
	std::ostream		buffstream(&buff);

	if( data.buildBinaryData(buffstream, XSecsData::BODY) == false ) {
		return false;
	}

	return this->print(stream, &data.get_header(), buff.getBuff(), buff.size(), pFrom, pTo, nDepth, pLF);
XEXCEPT_EXIT
}


bool XSEMI_XmlParser::printInner(XLIB_STD_OUT_STR_STREAM &stream, XDataReader& reader, XData& data, void* pDomNode,  XStrBuff &blank, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XmlHlpNode<XmlHlpElementRawType> pNode = (XmlHlpElementRawType*)pDomNode;
	TCHAR *pBlak = NULL;

	if( blank.GetBuffSize() < (size_t)(nDepth+10) ) {
		pBlak = (TCHAR*)blank.Alloc( (nDepth+60) * sizeof(TCHAR) );
	} else {
		pBlak = (TCHAR*)blank;
	}
	if( pBlak == NULL ) {
		return false;
	}
	blank.MemSet(' ');
	pBlak = (TCHAR*)blank;
	pBlak[nDepth] = 0;


	if( pNode.CompareI_NodeName(L"list") == 0 ) {
		if( data.getType() != XDATA_TYPE_L ) {
			return false;
		}

		XStrBuff strNodeName(_T(""));
		strNodeName = pNode.getAttribute(_T("display"));
		if( strNodeName == _T("") ) {
			strNodeName = _T("list");
		}


		XD_UINT4 ulNumOfElem = pNode.countElement();

		if( ulNumOfElem == 0 ) {
			XTRACE(_T("pNode.countElement()=0\n"));
			return false;
		}

		XD_UINT4 ulLoop = data.getCount() / ulNumOfElem;



		if( ulLoop != 0 ) {
			


			stream << (const TCHAR*)blank << _T("<") << (const TCHAR*)strNodeName << _T(">") << pLF;

			for(XD_UINT4 i = 0, ulTotal = 0; i < ulLoop; i++){
				XmlHlpNode<XmlHlpElementRawType> pNodeChiled = pNode.firstChild();
				while( pNodeChiled != NULL ) {
					if( reader.getValue( &data ) == false ) {
						return false;
					}
					printInner(stream, reader, data, pNodeChiled.m_pT, blank, nDepth+1, pLF);

					pNodeChiled = pNodeChiled.nextSiblingElement();
					ulTotal++;
				}
			}

			blank.MemSet(' ');
			pBlak = (TCHAR*)blank;
			pBlak[nDepth] = 0;

			stream << (const TCHAR*)blank << _T("</") << (const TCHAR*)strNodeName << _T(">") << pLF;
		} else {
			stream << (const TCHAR*)blank << _T("<") << (const TCHAR*)strNodeName << _T("/>") << pLF;
		}
	} else {
		XStrBuff strNodeName(_T(""));
		strNodeName = pNode.getNodeName();

		stream << (const TCHAR*)blank << _T("<") << (const TCHAR*)strNodeName;
		stream << _T(" type=\"") ;
		printInnerType(stream, data);
		stream << _T("\"") ;
		stream << _T(">");

		if( data.getType() != XDATA_TYPE_L ) {
			printInnerValue(stream, data, pLF);
		} else {
			stream << pLF;
			printInnerNoStyle(stream, reader, data, blank, nDepth + 1, pLF);

			blank.MemSet(' ');
			pBlak = (TCHAR*)blank;
			pBlak[nDepth] = 0;

			stream << (const TCHAR*)blank;
		}
		stream <<  _T("</") << (const TCHAR*)strNodeName << _T(">") << pLF;
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_XmlParser::printInnerNoStyle(XLIB_STD_OUT_STR_STREAM &stream, XDataReader& reader, XData& data, XStrBuff &blank, XD_INT4 nDepth, const TCHAR *pLF)
{
XEXCEPT_ENTRY
	TCHAR *pBlak = NULL;

	if( blank.GetBuffSize() < (size_t)(nDepth+10) ) {
		pBlak = (TCHAR*)blank.Alloc( (nDepth+60) * sizeof(TCHAR) );
	} else {
		pBlak = (TCHAR*)blank;
	}
	if( pBlak == NULL ) {
		return false;
	}
	blank.MemSet(' ');
	pBlak = (TCHAR*)blank;
	pBlak[nDepth] = 0;





	switch(data.getType()){
	case XDATA_TYPE_L:
		{
			if( data.getSize() == 0 ) {
				stream << (const TCHAR*)blank << _T("<list/>") << pLF;
			} else {
				stream << (const TCHAR*)blank << _T("<list>") << pLF;
				for( XD_UINT4 i = 0; i < data.getSize(); i++) {
					
					if( reader.getValue(&data) == false ) {
						break;
					}

					printInnerNoStyle(stream, reader, data, blank, nDepth+1, pLF);
				}

				blank.MemSet(' ');
				pBlak = (TCHAR*)blank;
				pBlak[nDepth] = 0;

				stream << (const TCHAR*)blank << _T("</list>") << pLF;
			}
		}
		return true;
		break;
	default:
		{
			stream << (const TCHAR*)blank << _T("<item type=\"");
			printInnerType(stream, data);
			if( data.getSize() == 0 ) {
				stream  << _T("\"/>") << pLF;
			} else {
				stream  << _T("\">");

				printInnerValue(stream, data, pLF);

				stream << _T("</item>") << pLF;
			}
		}
		break;
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_XmlParser::printInnerValue(XLIB_STD_OUT_STR_STREAM &stream, XData& data, const TCHAR *pLF)
{
XEXCEPT_ENTRY
	switch(data.getType()){
	case XDATA_TYPE_L:
		break;
	case XDATA_TYPE_B:
		{
			TCHAR sz[5];
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << _T(" ");	}
				XStrBuff::printf(sz, XCOUNTOF(sz), _T("%02x"), data.getValAsUInt1(i) );
				stream << sz ;
			}
		}
		break;
	case XDATA_TYPE_BOOL:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << _T(" ");	}
				if( data.getValAsBool(i) == true ) {
					stream << _T("true");
				} else {
					stream << _T("false");
				}
			}
		}
		break;
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
		{
			TCHAR* pData = NULL;
			XD_UINT4 ulDataSize;


			ulDataSize = data.getValAsBinaryRef((XBIN**)&pData);
			for(XD_UINT4 i = 0; i < ulDataSize; i++, pData++) {
				if( *pData == '&' ) {
					stream << _T("&amp;");
				} else
				if( *pData == '<' ) {
					stream << _T("&lt;");
				} else
				if( *pData == '>' ) {
					stream << _T("&gt;");
				} else
				if( *pData == '\"' ) {
					stream << _T("&quot;");
				} else
				if( *pData == '\'' ) {
					stream << _T("&apos;");
				} else {
					stream << *pData;
				}
			}
				
		}
		break;
	case XDATA_TYPE_U1:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (unsigned int)data.getValAsUInt1(i);
			}
		}
		break;
	case XDATA_TYPE_U2:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (unsigned int)data.getValAsUInt2(i);
			}
		}
		break;
	case XDATA_TYPE_U4:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (unsigned int)data.getValAsUInt4(i);
			}
		}
		break;
	case XDATA_TYPE_U8:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (unsigned int)data.getValAsUInt8(i);
			}
		}
		break;
	case XDATA_TYPE_I1:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (int)data.getValAsInt1(i);
			}
		}
		break;
	case XDATA_TYPE_I2:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (int)data.getValAsInt2(i);
			}
		}
		break;
	case XDATA_TYPE_I4:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (int)data.getValAsInt4(i);
			}
		}
		break;
	case XDATA_TYPE_I8:
		{
			TCHAR sz[256];
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				XStrBuff::printf(sz, XCOUNTOF(sz), _T("%I64d"), data.getValAsInt8(i));
				stream << sz;
			}
		}
		break;
	case XDATA_TYPE_F4:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << data.getValAsFloat4();
			}
		}
		break;
	case XDATA_TYPE_F8:
		{
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << data.getValAsFloat8();
			}
		}
		break;
	}
	return true;
XEXCEPT_EXIT
}


bool XSEMI_XmlParser::printInnerType(XLIB_STD_OUT_STR_STREAM &stream, XData& data)
{
XEXCEPT_ENTRY
	stream << XData::getDataTypeString_From_TYPE( data.getType() );
	return true;
XEXCEPT_EXIT
}


