#ifndef __XML_FELPER__H__
#define __XML_FELPER__H__

#ifdef _USE_TI_XML__
	// TiXmlNode
	#include "tinyxml/tinyxml.h"
#elif _USE_TI2_XML__
	#include "tinyxml2/tinyxml2.h"
#elif _USE_XERCESC_XML_
	// xercesc
	#include <xercesc/util/PlatformUtils.hpp>
	#include <xercesc/dom/DOM.hpp>
	#include <xercesc/dom/DOMImplementation.hpp>
	#include <xercesc/dom/DOMImplementationLS.hpp>
	#include <xercesc/dom/DOMWriter.hpp>
	#include <xercesc/framework/StdOutFormatTarget.hpp>
	#include <xercesc/framework/LocalFileFormatTarget.hpp>
	#include <xercesc/framework/MemBufFormatTarget.hpp>
	#include <xercesc/framework/MemBufInputSource.hpp>
	#include <xercesc/parsers/XercesDOMParser.hpp>
	#include <xercesc/util/XMLUni.hpp>
	#include <xercesc/util/XercesDefs.hpp>
	#include <xercesc/sax/ErrorHandler.hpp>
	#include <xercesc/sax/SAXParseException.hpp>
#endif /*_USE_TI_XML__*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"



enum XmlHlpNode_NodeType
{
	XmlHlpNode_NodeType_DOCUMENT,
	XmlHlpNode_NodeType_ELEMENT,
	XmlHlpNode_NodeType_COMMENT,
	XmlHlpNode_NodeType_UNKNOWN,
	XmlHlpNode_NodeType_ATTRIBUTE,
	XmlHlpNode_NodeType_TEXT,
	XmlHlpNode_NodeType_CDATA,
	XmlHlpNode_NodeType_DECLARATION,
	XmlHlpNode_NodeType_TYPECOUNT
};

class XmlHlpErrReporter;


#ifdef _USE_TI_XML__
	#define XmlHlpNodeRawType		TiXmlNode
	#define XmlHlpElementRawType	TiXmlElement
	#define XmlT(x)					_T( ## x ## )
#elif _USE_TI2_XML__
	#define XmlHlpNodeRawType		tinyxml2::XMLNode
	#define XmlHlpElementRawType	tinyxml2::XMLElement
	#define XmlT(x)					_T( ## x ## )
#elif _USE_XERCESC_XML_
	// xercesc
	XERCES_CPP_NAMESPACE_USE
	#define XmlHlpNodeRawType		DOMNode
	#define XmlHlpElementRawType	DOMElement
	#define XmlT(x)					L ## x
#endif /*_USE_TI_XML__*/

#define XmlHlpDocumentActl		XmlHlpDocument<XmlHlpNodeRawType>
#define XmlHlpElementActl		XmlHlpNode<XmlHlpElementRawType>
struct XmlHlpInitilizer;



/** 
 * XMLパーサーの初期化
 */
struct XmlHlpInitilizer {
	XmlHlpInitilizer(){
		#ifdef _USE_XERCESC_XML_
			try { XMLPlatformUtils::Initialize(); } catch(...){}
		#endif /*_USE_XERCESC_XML_*/
	}
	~XmlHlpInitilizer(){
		Release();
	}
	void Release(){
		#ifdef _USE_XERCESC_XML_
			try { XMLPlatformUtils::Terminate(); } catch(...){}
		#endif /*_USE_XERCESC_XML_*/
	}
};
template <class T>
class XmlHlpNode
{
public:
// construction
	XmlHlpNode(){ m_pT = NULL; }
	XmlHlpNode(T* p){ m_pT = p; }
	virtual ~XmlHlpNode(){}
	operator T*(){ return m_pT;}
	T& operator*() const{return *m_pT;}
	T** operator&(){return &m_pT;}
	T* operator->() const{return m_pT;}
	bool operator!() const{	return (m_pT == NULL); }
	bool operator<(T* pT) const{ return m_pT < pT; }
	bool operator==(T* pT) const { return m_pT == pT; }
	bool operator!=(T* pT) const { return m_pT != pT; }
	T* operator=(const XmlHlpNode<T>& lp){m_pT = lp.m_pT;return m_pT;}
	const XmlHlpNode<T>& operator=(T* p){m_pT = p; return *this;}

	XmlHlpNode_NodeType getType();
	XStrBuff getNodeName();
	XStrBuff getValue();
	/**	@brief	テキストを設定
	 *
	 *	@param	src	テキスト
	 *	@param	nMd	0:TEXT_NODE	1:CDATA_SECTION_NODE	2:0 or 1
	 *
	 *	@return	bool
	 */
	bool setValue(const XStrBuff& src,  int nMd = 0);

	int cntAttribute();
	bool isAttribute(const char* attrName);
	bool isAttribute(const wchar_t* attrName);
	XStrBuff getAttribute(const int index, XStrBuff& name, XStrBuff& value);
	XStrBuff getAttribute(const char* attrName);
	XStrBuff getAttribute(const wchar_t* attrName);
	int getAttributeInt(const char* attrName);
	int getAttributeInt(const wchar_t* attrName);
	unsigned int getAttributeUInt(const char* attrName);
	unsigned int getAttributeUInt(const wchar_t* attrName);
	float getAttributeFloat(const char* attrName);
	float getAttributeFloat(const wchar_t* attrName);
	double getAttributeDouble(const char* attrName);
	double getAttributeDouble(const wchar_t* attrName);

	void setAttribute(const char* attrName, const char* pValue);
	void setAttribute(const char* attrName, unsigned int val);
	void setAttribute(const char* attrName, int val);
	void setAttribute(const char* attrName, float val);
	void setAttribute(const char* attrName, double val);

	
	void setAttribute(const wchar_t* attrName, const wchar_t* pValue);
	void setAttribute(const wchar_t* attrName, unsigned int val);
	void setAttribute(const wchar_t* attrName, int val);
	void setAttribute(const wchar_t* attrName, float val);
	void setAttribute(const wchar_t* attrName, double val);

	void removeAttr(const char* attrName);
	void removeAttr(const wchar_t* attrName);


	int CompareI_NodeName(const char* val);
	int CompareI_NodeName(const wchar_t* val);
	XmlHlpElementRawType* addElement(const char* p_tag);
	XmlHlpElementRawType* addElement(const wchar_t* p_tag);
	XmlHlpElementRawType* insert(T* pInsItem, T* pPrev);
	bool removeChild(T* removeThis);
	bool removeChildren();
	unsigned long countElement();
	void getPathString(XStrBuff& str);
	void clear();

	T* parent();
	T* firstChild();
	T* nextSibling();
	T* firstChildElement();
	T* nextSiblingElement();
	T* prevSiblingElement();
	T* findNode(XStrBuff& strFile);
	T* clone();

// member
	T* m_pT;
};

template <class T>
class XmlHlpDocument : public XmlHlpNode<T>
{
public:
	XmlHlpDocument(){ 
		#ifdef _USE_TI_XML__
			m_pT = (TiXmlElement*)&m_doc; 
		#elif _USE_TI2_XML__
			m_pT = (tinyxml2::XMLElement*)m_doc.RootElement(); 
		#elif _USE_XERCESC_XML_
			m_pPaerse = NULL;
			m_pDoc = NULL;
		#endif /*_USE_TI_XML__*/
		m_bOpend = false;
	}
	virtual ~XmlHlpDocument(){
		#ifdef _USE_TI_XML__
		#elif _USE_TI2_XML__

		#elif _USE_XERCESC_XML_
			if( m_pPaerse != NULL ) delete m_pPaerse;
			m_pPaerse = NULL;
		#endif /*_USE_TI_XML__*/
	}
	bool loadFromFile(const XStrBuff& strFile, XmlHlpErrReporter *pErrorReportergd);
	bool saveFromFile(const XStrBuff& strFile, XmlHlpErrReporter *pErrorReportergd);
private:
	#ifdef _USE_TI_XML__
		TiXmlDocument	m_doc;
	#elif _USE_TI2_XML__
		tinyxml2::XMLDocument	m_doc;
	#elif _USE_XERCESC_XML_
		//
		DOMDocument*	m_pDoc;
		XercesDOMParser*	m_pPaerse;
	#endif /*_USE_TI_XML__*/
	bool	m_bOpend;
};


class XmlHlpErrReporter
		#ifdef _USE_XERCESC_XML_
			: public ErrorHandler
		#endif /*_USE_XERCESC_XML_*/
{
public:
	XmlHlpErrReporter();
	virtual ~XmlHlpErrReporter();

	XStrBuff& getErrorString(){ return m_strErrString; }

	unsigned long m_ulRow;
	unsigned long m_ulColum;
	XStrBuff      m_strErrString;

#ifdef _USE_XERCESC_XML_
	virtual void warning(const SAXParseException& toCatch){}
	virtual void error(const SAXParseException& toCatch){
		if(fSawErrors == false) {
			//m_strSystemID = toCatch.getSystemId();
			m_strErrString  = toCatch.getMessage();
			m_ulRow = toCatch.getLineNumber();
			m_ulColum = toCatch.getColumnNumber();
			fSawErrors = true;
		}
	}
	virtual void fatalError(const SAXParseException& toCatch) {
		this->error(toCatch);
	}
	virtual void resetErrors(){ fSawErrors = false;}
	virtual bool getSawErrors() const{ return fSawErrors;}
	bool    fSawErrors;
#endif /*_USE_XERCESC_XML_*/
};

//
//	XmlHlpNode
//
template<class T>
unsigned long XmlHlpNode<T>::countElement()
{
	_XASSERT(m_pT);
	#ifdef _USE_TI_XML__
		TiXmlNode* pNode = m_pT->FirstChildElement();
		TiXmlElement* itemElement = NULL;
		unsigned long ulCount = 0;

		if( pNode == NULL ) {
			return ulCount;
		}
		itemElement = pNode->ToElement();
		if( itemElement == NULL ) {
			return ulCount;
		}
		while( itemElement != NULL ) {
			ulCount++;
			itemElement = itemElement->NextSiblingElement();
		}
		return ulCount;
	#elif _USE_TI2_XML__
		tinyxml2::XMLNode* pNode = m_pT->FirstChildElement();
		tinyxml2::XMLElement* itemElement = NULL;
		unsigned long ulCount = 0;

		if( pNode == NULL ) {
			return ulCount;
		}
		itemElement = pNode->ToElement();
		if( itemElement == NULL ) {
			return ulCount;
		}
		while( itemElement != NULL ) {
			ulCount++;
			itemElement = itemElement->NextSiblingElement();
		}
		return ulCount;
	#elif _USE_XERCESC_XML_
		register unsigned long cnt = 0;
		register DOMNodeList *plist = m_pT->getChildNodes();
		register XMLSize_t nLen = plist->getLength();

		
		DOMNode *pFirst = m_pT->getFirstChild();
		if( pFirst == NULL ) {
			return 0;
		}
		DOMNode *pNodeTemp = pFirst;
		for(;;) {
			if( pNodeTemp->getNodeType() == DOMNode::ELEMENT_NODE ) cnt++;
			DOMNode *p = pNodeTemp->getNextSibling();
			if( p == NULL ) {
				break;
			}
			pNodeTemp = p;
		}
		return cnt;
	#endif /*_USE_TI_XML__*/
}

template<class T>
XmlHlpNode_NodeType XmlHlpNode<T>::getType()
{
	_XASSERT(m_pT);
	XmlHlpNode_NodeType type = XmlHlpNode_NodeType_UNKNOWN;
#ifdef _USE_TI_XML__
	switch( m_pT->Type() ){
	case TiXmlNode::TINYXML_DOCUMENT:
		type = XmlHlpNode_NodeType_DOCUMENT;
		break;
	case TiXmlNode::TINYXML_ELEMENT:
		type = XmlHlpNode_NodeType_ELEMENT;
		break;
	case TiXmlNode::TINYXML_COMMENT:
		type = XmlHlpNode_NodeType_COMMENT;
		break;
	case TiXmlNode::TINYXML_UNKNOWN:
		type = XmlHlpNode_NodeType_UNKNOWN;
		break;
	case TiXmlNode::TINYXML_TEXT:
		type = XmlHlpNode_NodeType_TEXT;
		break;
	case TiXmlNode::TINYXML_DECLARATION:
		type = XmlHlpNode_NodeType_DECLARATION;
		break;
	case TiXmlNode::TINYXML_TYPECOUNT:
		type = XmlHlpNode_NodeType_TYPECOUNT;
		break;
	}
#elif _USE_TI2_XML__
	if( m_pT->ToDocument() != NULL ) {
		type = XmlHlpNode_NodeType_DOCUMENT;
	} else
	if( m_pT->ToElement() != NULL ) {
		type = XmlHlpNode_NodeType_ELEMENT;
	} else
	if( m_pT->ToComment() != NULL ) {
		type = XmlHlpNode_NodeType_COMMENT;
	} else
	if( m_pT->ToText() != NULL ) {
		type = XmlHlpNode_NodeType_TEXT;
	} else
	if( m_pT->ToText() != NULL ) {
		type = XmlHlpNode_NodeType_TEXT;
	} else
	if( m_pT->ToDeclaration() != NULL ) {
		type = XmlHlpNode_NodeType_DECLARATION;
	} else
	if( m_pT->ToUnknown() != NULL ) {
		type = XmlHlpNode_NodeType_UNKNOWN;
	}
#elif _USE_XERCESC_XML_
	/*
		enum NodeType {
			ELEMENT_NODE                = 1,
			ATTRIBUTE_NODE              = 2,
			TEXT_NODE                   = 3,
			CDATA_SECTION_NODE          = 4,
			ENTITY_REFERENCE_NODE       = 5,
			ENTITY_NODE                 = 6,
			PROCESSING_INSTRUCTION_NODE = 7,
			COMMENT_NODE                = 8,
			DOCUMENT_NODE               = 9,
			DOCUMENT_TYPE_NODE          = 10,
			DOCUMENT_FRAGMENT_NODE      = 11,
			NOTATION_NODE               = 12
		};
	*/
	switch( m_pT->getNodeType() ){
	case DOMNode::ELEMENT_NODE:
		type = XmlHlpNode_NodeType_ELEMENT;
		break;
	case DOMNode::ATTRIBUTE_NODE:
		type = XmlHlpNode_NodeType_ATTRIBUTE;
		break;
	case DOMNode::TEXT_NODE:
		type = XmlHlpNode_NodeType_TEXT;
		break;
	case DOMNode::CDATA_SECTION_NODE:
		type = XmlHlpNode_NodeType_CDATA;
		break;

	case DOMNode::COMMENT_NODE:
		type = XmlHlpNode_NodeType_COMMENT;
		break;


	}
#endif /*_USE_TI_XML__*/
	return type;
}

template<class T>
void XmlHlpNode<T>::getPathString(XStrBuff& str)
{
	_XASSERT(m_pT);

	T* pParent = parent();
	if( pParent != NULL ) {
		XmlHlpNode<T>	obParent = pParent;
		obParent.getPathString( str );
	}
	if( getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	str << _T("/") << (const TCHAR*)this->getNodeName();
#ifdef _USE_TI_XML__
	TiXmlAttribute* pAttr = m_pT->FirstAttribute();
	if( pAttr != NULL ) {
		str << _T("[");
	}
	for(; pAttr != NULL;  ) {
		str << _T("@") << pAttr->Name() << _T("=") << pAttr->Value();
		pAttr = pAttr->Next();
		if( pAttr == NULL ) {
			str << _T("]");
		}
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	const tinyxml2::XMLAttribute* pAttr = m_pT->FirstAttribute();
	if( pAttr != NULL ) {
		str << _T("[");
	}
	for(; pAttr != NULL;  ) {
		str << _T("@") << pAttr->Name() << _T("=") << pAttr->Value();
		pAttr = pAttr->Next();
		if( pAttr == NULL ) {
			str << _T("]");
		}
	}
#endif /*_USE_TI_XML__*/
}

template<class T>
void XmlHlpNode<T>::clear()
{
#ifdef _USE_TI_XML__
	m_pT->Clear();
#elif _USE_TI2_XML__
	m_pT->Clear();
#endif /*_USE_TI_XML__*/
}


template<class T>
XStrBuff XmlHlpNode<T>::getNodeName()
{
	_XASSERT(m_pT);

	XStrBuff str(_T(""));
#ifdef _USE_TI_XML__
		str = m_pT->Value();
#elif _USE_TI2_XML__
		str = m_pT->Value();
#elif _USE_XERCESC_XML_
		str = m_pT->getNodeName();
#endif /*_USE_TI_XML__*/
	return str;
}
template<class T>
int XmlHlpNode<T>::CompareI_NodeName(const char* val)
{
	_XASSERT(m_pT);

#ifdef _USE_TI_XML__
		const char* pDst = m_pT->Value();
		if( pDst == NULL ) {
			return -1;
		}
		return _stricmp(val, pDst);
#elif _USE_TI2_XML__
		const char* pDst = m_pT->Value();
		if( pDst == NULL ) {
			return -1;
		}
		return _stricmp(val, pDst);
#elif _USE_XERCESC_XML_
		XStrBuff str = m_pT->getNodeName();
		return stricmp(val, str);
#endif /*_USE_TI_XML__*/
}

template<class T>
int XmlHlpNode<T>::CompareI_NodeName(const wchar_t* val)
{
	_XASSERT(m_pT);
#ifdef _USE_TI_XML__
		const char* pDst = m_pT->Value();
		if( pDst == NULL ) {
			return -1;
		}
		XStrBuff strDst(pDst);
		return _wcsicmp(val, strDst);
#elif _USE_TI2_XML__
		const char* pDst = m_pT->Value();
		if( pDst == NULL ) {
			return -1;
		}
		XStrBuff strDst(pDst);
		return _wcsicmp(val, strDst);
#elif _USE_XERCESC_XML_
		return wcsicmp(val, m_pT->getNodeName());
#endif /*_USE_TI_XML__*/
}


template<class T>
XStrBuff XmlHlpNode<T>::getValue()
{
	_XASSERT(m_pT);

	XStrBuff str(_T(""));
#ifdef _USE_TI_XML__
		str = m_pT->GetText();
#elif _USE_TI2_XML__
		str = m_pT->GetText();
#elif _USE_XERCESC_XML_
		if( m_pT->getNodeType() == DOMNode::ELEMENT_NODE ) {
			if( m_pT->hasChildNodes() == false ) {
				return str;
			}
			DOMNode* p = m_pT->getFirstChild();
			while( p != NULL ) {
				if(  p->getNodeType() == DOMNode::TEXT_NODE ){
					str = ((DOMText*)p)->getNodeValue();
					break;
				} else
				if(  p->getNodeType() == DOMNode::CDATA_SECTION_NODE ){
					str = ((DOMText*)p)->getNodeValue();
					break;
				}
				p = p->getNextSibling();
			}
		} else {
			str = m_pT->getNodeValue();
		}
#endif /*_USE_TI_XML__*/
	return str;
}

template<class T>
bool XmlHlpNode<T>::setValue(const XStrBuff& src,  int nMd /*= 0*/)
{
	_XASSERT(m_pT);

	#ifdef _USE_TI_XML__	

		TiXmlText* pText = NULL;
		TiXmlNode* child = m_pT->FirstChild();
		if ( child ) {
			pText = child->ToText();
		}

		if( pText == NULL ) {
			TiXmlText txt( (const char*)XStrBuff(src.c_str()) );
			pText =  (TiXmlText*)m_pT->InsertEndChild( txt );
			if( pText == NULL ) {
				return false;
			}
		}

		pText->SetCDATA( nMd == 1 );
		pText->SetValue( (const char*)XStrBuff(src.c_str()) );
	#elif _USE_TI2_XML__

		tinyxml2::XMLText* pText = NULL;
		tinyxml2::XMLNode* child = m_pT->FirstChild();
		if ( child ) {
			pText = child->ToText();
		}

		if( pText == NULL ) {
			tinyxml2::XMLText* p = m_pT->GetDocument()->NewText( (const char*)XStrBuff(src.c_str()) );
			pText =  (tinyxml2::XMLText*)m_pT->InsertEndChild( p );
			if( pText == NULL ) {
				return false;
			}
		}

		pText->SetCData( nMd == 1 );
		pText->SetValue( (const char*)XStrBuff(src.c_str()) );
	#elif _USE_XERCESC_XML_


		DOMNodeList *plist = m_pT->getChildNodes();
		DOMNode *pExist = NULL;
		for(XMLSize_t i = 0; i < plist->getLength(); i++) {
			DOMNode *p = plist->item(i);
			if( p->getNodeType() == DOMNode::TEXT_NODE ) {
				pExist = p;
				break;
			}
			if( p->getNodeType() == DOMNode::CDATA_SECTION_NODE ) {
				pExist = p;
				break;
			}
		}	
		if( pExist == NULL ) {
			if( nMd == 0 ) {
				DOMText*     pDataVal;
				pDataVal = m_pT->getOwnerDocument()->createTextNode(src);
				((DOMElement*)m_pT)->appendChild(pDataVal);
			}else {
				DOMCDATASection* pDataVal;
				pDataVal = m_pT->getOwnerDocument()->createCDATASection(src);
				((DOMElement*)m_pT)->appendChild(pDataVal);
			}
		} else {
			((DOMCharacterData*)pExist)->setData(src);
		}


	#endif /*_USE_TI_XML__*/
	return true;
}

template<class T>
int XmlHlpNode<T>::cntAttribute()
{
	_XASSERT(m_pT);
	
	bool b = false;
#ifdef _USE_TI_XML__
//	TiXmlAttribute* pAttr = m_pT->FirstAttribute();
#elif _USE_TI2_XML__

#elif _USE_XERCESC_XML_
	if( m_pT->hasAttributes() == false ) {
		return 0;
	}
	DOMNamedNodeMap *pAttrs = m_pT->getAttributes();
	_XASSERT(pAttrs);	
	return pAttrs->getLength();
#endif /*_USE_TI_XML__*/
	return 0;
}

template<class T>
XStrBuff XmlHlpNode<T>::getAttribute(const int index, XStrBuff& name, XStrBuff& value)
{
	_XASSERT(m_pT);
	
	XStrBuff str(_T(""));
#ifdef _USE_TI_XML__
	
#elif _USE_TI2_XML__

#elif _USE_XERCESC_XML_
	if( m_pT->hasAttributes() == false ) {
		return str;
	}
	DOMNamedNodeMap *pAttrs = m_pT->getAttributes();
	_XASSERT(pAttrs);	
	DOMNode* pNode = pAttrs->item( index );

	if( pNode == NULL ) {
		return str;
	}
	if( pNode->getNodeType() != DOMNode::ATTRIBUTE_NODE ) {
		return str;
	}
	name = pNode->getNodeName();
	value = pNode->getNodeValue();
#endif /*_USE_TI_XML__*/
	return str;
}

template<class T>
bool XmlHlpNode<T>::isAttribute(const char* attrName)
{
	_XASSERT(m_pT);

	bool b = false;
#ifdef _USE_TI_XML__
	TiXmlAttribute* pAttr = m_pT->FirstAttribute();
	while( pAttr != NULL ) {
		if( XStrBuff::stricmp(pAttr->Name(), attrName ) == 0 ) {
			return true;
		}
		pAttr = pAttr->Next();
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return false;
	}
	return m_pT->Attribute(attrName) != NULL;
#elif _USE_XERCESC_XML_
	if( m_pT->hasAttributes() == false ) {
		return false;
	}
	DOMNamedNodeMap *pAttrs = m_pT->getAttributes();
	_XASSERT(pAttrs);	
	DOMNode   *pAttr = pAttrs->getNamedItem( XStrBuff(attrName) );
	b = pAttr != NULL ? true : false;
#endif /*_USE_TI_XML__*/
	return b;
}

template<class T>
bool XmlHlpNode<T>::isAttribute(const wchar_t* attrName)
{
	_XASSERT(m_pT);

	bool b = false;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return false;
	}
	XStrBuff	strAttr(attrName);
	TiXmlAttribute* pAttr = m_pT->FirstAttribute();
	while( pAttr != NULL ) {
		if( XStrBuff::stricmp(pAttr->Name(), strAttr ) == 0 ) {
			return true;
		}
		pAttr = pAttr->Next();
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return false;
	}
	XStrBuff	strAttr(attrName);
	const tinyxml2::XMLAttribute* pAttr = m_pT->FirstAttribute();
	while( pAttr != NULL ) {
		if( XStrBuff::stricmp(pAttr->Name(), strAttr ) == 0 ) {
			return true;
		}
		pAttr = pAttr->Next();
	}
#elif _USE_XERCESC_XML_
	if( m_pT->hasAttributes() == false ) {
		return false;
	}
	DOMNamedNodeMap *pAttrs = m_pT->getAttributes();
	_XASSERT(pAttrs);	
	DOMNode   *pAttr = pAttrs->getNamedItem( attrName );
	b = pAttr != NULL ? true : false;
#endif /*_USE_TI_XML__*/
	return b;
}


template<class T>
XStrBuff XmlHlpNode<T>::getAttribute(const char* attrName)
{
	XStrBuff str(_T(""));
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return _T("");
	}
	const char* pAttr = m_pT->Attribute( attrName );
	if( pAttr != NULL ) {
		str = pAttr;
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return _T("");
	}
	const char* pAttr = m_pT->Attribute( attrName );
	if( pAttr != NULL ) {
		str = pAttr;
	}
#elif _USE_XERCESC_XML_
	if( m_pT->hasAttributes() == false ) {
		return str;
	}
	DOMNamedNodeMap *pAttrs = m_pT->getAttributes();
	_XASSERT(pAttrs);	
	DOMNode   *pAttr = pAttrs->getNamedItem( XStrBuff(attrName) );
	if( pAttr != NULL ) {
		str = pAttr->getNodeValue();
	}
#endif /*_USE_TI_XML__*/
	return str;
}
template<class T>
XStrBuff XmlHlpNode<T>::getAttribute(const wchar_t* attrName)
{
	XStrBuff str(_T(""));
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return _T("");
	}
	const char* pAttr = m_pT->Attribute( XStrBuff(attrName) );
	if( pAttr != NULL ) {
		str = pAttr;
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return _T("");
	}
	const char* pAttr = m_pT->Attribute( XStrBuff(attrName) );
	if( pAttr != NULL ) {
		str = pAttr;
	}
#elif _USE_XERCESC_XML_
	if( m_pT->hasAttributes() == false ) {
		return str;
	}
	DOMNamedNodeMap *pAttrs = m_pT->getAttributes();
	_XASSERT(pAttrs);	
	DOMNode   *pAttr = pAttrs->getNamedItem( attrName );
	if( pAttr != NULL ) {
		str = pAttr->getNodeValue();
	}


#endif /*_USE_TI_XML__*/
	return str;
}




template<class T>
int XmlHlpNode<T>::getAttributeInt(const char* attrName)
{
	int val = 0;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	const char* pAttr = m_pT->Attribute( attrName );
	if( pAttr != NULL ) {
		if( strlen( pAttr ) >= 3 ) {
			if( pAttr[0] == '0' && (pAttr[1] == 'x' || pAttr[1] == 'X') ) {
				val = ::strtol(&pAttr[2], 0, 16);
			} else {
				val = ::strtol(&pAttr[0], 0, 10);
			}
		} else {
			val = ::strtol(&pAttr[0], 0, 10);
		}
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	const char* pAttr = m_pT->Attribute( attrName );
	if( pAttr != NULL ) {
		if( strlen( pAttr ) >= 3 ) {
			if( pAttr[0] == '0' && (pAttr[1] == 'x' || pAttr[1] == 'X') ) {
				val = ::strtol(&pAttr[2], 0, 16);
			} else {
				val = ::strtol(&pAttr[0], 0, 10);
			}
		} else {
			val = ::strtol(&pAttr[0], 0, 10);
		}
	}
#elif _USE_XERCESC_XML_
	XStrBuff strVal = this->getAttribute(attrName);
	val = atol(strVal);
#endif /*_USE_TI_XML__*/
	return val;
}
template<class T>
int XmlHlpNode<T>::getAttributeInt(const wchar_t* attrName)
{
	int val = 0;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	const char* pAttr = m_pT->Attribute( XStrBuff( attrName ) );
	if( pAttr != NULL ) {
		if( strlen( pAttr ) >= 3 ) {
			if( pAttr[0] == '0' && (pAttr[1] == 'x' || pAttr[1] == 'X') ) {
				val = ::strtol(&pAttr[2], 0, 16);
			} else {
				val = ::strtol(&pAttr[0], 0, 10);
			}
		} else {
			val = ::strtol(&pAttr[0], 0, 10);
		}
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	const char* pAttr = m_pT->Attribute( XStrBuff( attrName ) );
	if( pAttr != NULL ) {
		if( strlen( pAttr ) >= 3 ) {
			if( pAttr[0] == '0' && (pAttr[1] == 'x' || pAttr[1] == 'X') ) {
				val = ::strtol(&pAttr[2], 0, 16);
			} else {
				val = ::strtol(&pAttr[0], 0, 10);
			}
		} else {
			val = ::strtol(&pAttr[0], 0, 10);
		}
	}
#elif _USE_XERCESC_XML_
	XStrBuff strVal = this->getAttribute(attrName);
	val = atol(strVal);
#endif /*_USE_TI_XML__*/
	return val;
}
template<class T>
unsigned int XmlHlpNode<T>::getAttributeUInt(const char* attrName)
{
	int val = 0;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	const char* pAttr = m_pT->Attribute( attrName );
	if( pAttr != NULL ) {
		if( strlen( pAttr ) >= 3 ) {
			if( pAttr[0] == '0' && (pAttr[1] == 'x' || pAttr[1] == 'X') ) {
				val = ::strtoul(&pAttr[2], 0, 16);
			} else {
				val = ::strtoul(&pAttr[0], 0, 10);
			}
		} else {
			val = ::strtoul(&pAttr[0], 0, 10);
		}
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	const char* pAttr = m_pT->Attribute( attrName );
	if( pAttr != NULL ) {
		if( strlen( pAttr ) >= 3 ) {
			if( pAttr[0] == '0' && (pAttr[1] == 'x' || pAttr[1] == 'X') ) {
				val = ::strtoul(&pAttr[2], 0, 16);
			} else {
				val = ::strtoul(&pAttr[0], 0, 10);
			}
		} else {
			val = ::strtoul(&pAttr[0], 0, 10);
		}
	}
#elif _USE_XERCESC_XML_
	XStrBuff strVal = this->getAttribute(attrName);
	val = atol(strVal);
#endif /*_USE_TI_XML__*/
	return val;
}
template<class T>
unsigned int  XmlHlpNode<T>::getAttributeUInt(const wchar_t* attrName)
{
	int val = 0;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	const char* pAttr = m_pT->Attribute( XStrBuff( attrName ) );
	if( pAttr != NULL ) {
		if( strlen( pAttr ) >= 3 ) {
			if( pAttr[0] == '0' && (pAttr[1] == 'x' || pAttr[1] == 'X') ) {
				val = ::strtoul(&pAttr[2], 0, 16);
			} else {
				val = ::strtoul(&pAttr[0], 0, 10);
			}
		} else {
			val = ::strtoul(&pAttr[0], 0, 10);
		}
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	const char* pAttr = m_pT->Attribute( XStrBuff( attrName ) );
	if( pAttr != NULL ) {
		if( strlen( pAttr ) >= 3 ) {
			if( pAttr[0] == '0' && (pAttr[1] == 'x' || pAttr[1] == 'X') ) {
				val = ::strtoul(&pAttr[2], 0, 16);
			} else {
				val = ::strtoul(&pAttr[0], 0, 10);
			}
		} else {
			val = ::strtoul(&pAttr[0], 0, 10);
		}
	}
#elif _USE_XERCESC_XML_
	XStrBuff strVal = this->getAttribute(attrName);
	val = atol(strVal);
#endif /*_USE_TI_XML__*/
	return val;
}
template<class T>
float XmlHlpNode<T>::getAttributeFloat(const char* attrName)
{
	float val = 0.0;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	m_pT->QueryFloatAttribute( attrName, &val );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return false;
	}
	m_pT->QueryFloatAttribute( attrName, &val );
#endif /*_USE_TI_XML__*/
	return val;
}
template<class T>
float XmlHlpNode<T>::getAttributeFloat(const wchar_t* attrName)
{
	float val = 0.0;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	return  m_pT->QueryFloatAttribute( XStrBuff( attrName ), &val );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	return  m_pT->QueryFloatAttribute( XStrBuff( attrName ), &val );
#endif /*_USE_TI_XML__*/
	return val;
}
template<class T>
double XmlHlpNode<T>::getAttributeDouble(const char* attrName)
{
	double val = 0.0;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return 0.0;
	}
	if( m_pT->QueryDoubleAttribute( attrName, &val ) != TIXML_SUCCESS ) {
		return 0.0;
	}
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return false;
	}
	if( m_pT->QueryDoubleAttribute( attrName, &val ) != tinyxml2::XML_SUCCESS ) {
		return 0.0;
	}
#endif /*_USE_TI_XML__*/
	return val;
}
template<class T>
double XmlHlpNode<T>::getAttributeDouble(const wchar_t* attrName)
{
	double val = 0.0;
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	m_pT->QueryDoubleAttribute(  XStrBuff( attrName ), &val );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return val;
	}
	m_pT->QueryDoubleAttribute(  XStrBuff( attrName ), &val );
#endif /*_USE_TI_XML__*/
	return val;
}
template<class T>
void XmlHlpNode<T>::setAttribute(const char* attrName, const char* pValue)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	m_pT->SetAttribute(attrName, pValue );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	m_pT->SetAttribute(attrName, pValue );
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::setAttribute(const char* attrName, int val)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%d", val);
	m_pT->SetAttribute(attrName, szValue );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%d", val);
	m_pT->SetAttribute(attrName, szValue );
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::setAttribute(const char* attrName, unsigned int val)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%u", val);
	m_pT->SetAttribute(attrName, szValue );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%u", val);
	m_pT->SetAttribute(attrName, szValue );
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::setAttribute(const char* attrName, float val)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%e", val);
	m_pT->SetAttribute(attrName, szValue );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%e", val);
	m_pT->SetAttribute(attrName, szValue );
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::setAttribute(const char* attrName, double val)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%e", val);
	m_pT->SetAttribute(attrName, szValue );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%e", val);
	m_pT->SetAttribute(attrName, szValue );
#endif /*_USE_TI_XML__*/
}



template<class T>
void XmlHlpNode<T>::setAttribute(const wchar_t* attrName, const wchar_t* pValue)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	m_pT->SetAttribute((const char*)XStrBuff(attrName), (const char*)XStrBuff(pValue) );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	m_pT->SetAttribute((const char*)XStrBuff(attrName), (const char*)XStrBuff(pValue) );
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::setAttribute(const wchar_t* attrName, int val)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%d", val);
	m_pT->SetAttribute((const char*)XStrBuff(attrName), szValue );;
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue), "%d", val);
	m_pT->SetAttribute((const char*)XStrBuff(attrName), szValue );;
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::setAttribute(const wchar_t* attrName, unsigned int val)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%u", val);
	m_pT->SetAttribute((const char*)XStrBuff(attrName), szValue );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%u", val);
	m_pT->SetAttribute((const char*)XStrBuff(attrName), szValue );
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::setAttribute(const wchar_t* attrName, float val)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%e", val);
	m_pT->SetAttribute((const char*)XStrBuff(attrName), szValue );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%e", val);
	m_pT->SetAttribute((const char*)XStrBuff(attrName), szValue );
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::setAttribute(const wchar_t* attrName, double val)
{
#ifdef _USE_TI_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%e", val);
	m_pT->SetAttribute((const char*)XStrBuff(attrName), szValue );
#elif _USE_TI2_XML__
	if( this->getType() != XmlHlpNode_NodeType_ELEMENT ) {
		return ;
	}
	char szValue[256] = {0};
	XStrBuff::__sprintf(szValue, sizeof(szValue),  "%e", val);
	m_pT->SetAttribute((const char*)XStrBuff(attrName), szValue );
#endif /*_USE_TI_XML__*/
}

template<class T>
void XmlHlpNode<T>::removeAttr(const char* attrName)
{
#ifdef _USE_TI_XML__
	if( this->getType() == XmlHlpNode_NodeType_ELEMENT ) {
		((TiXmlElement*)m_pT)->RemoveAttribute( attrName );
	}
#elif _USE_TI2_XML__
	if( this->getType() == XmlHlpNode_NodeType_ELEMENT ) {
		((XmlHlpElementRawType*)m_pT)->DeleteAttribute( attrName );
	}
#endif /*_USE_TI_XML__*/
}
template<class T>
void XmlHlpNode<T>::removeAttr(const wchar_t* attrName)
{
#ifdef _USE_TI_XML__
	if( this->getType() == XmlHlpNode_NodeType_ELEMENT ) {
		((TiXmlElement*)m_pT)->RemoveAttribute( (const char*)XStrBuff(attrName) );
	}
#elif _USE_TI2_XML__
	if( this->getType() == XmlHlpNode_NodeType_ELEMENT ) {
		((XmlHlpElementRawType*)m_pT)->DeleteAttribute( (const char*)XStrBuff(attrName)  );
	}
#endif /*_USE_TI_XML__*/
}









template<class T>
T* XmlHlpNode<T>::parent()
{
	_XASSERT(m_pT);
#ifdef _USE_TI_XML__
	return (T*)m_pT->Parent();
#elif _USE_TI2_XML__
	return (T*)m_pT->Parent();
#elif _USE_XERCESC_XML_
	return (T*)m_pT->getParentNode();	
#endif /*_USE_TI_XML__*/
}


template<class T>
T* XmlHlpNode<T>::firstChild()
{
	_XASSERT(m_pT);
#ifdef _USE_TI_XML__
	return (XmlHlpElementRawType*)m_pT->FirstChild();
#elif _USE_TI2_XML__
	return (XmlHlpElementRawType*)m_pT->FirstChild();
#elif _USE_XERCESC_XML_
	return (XmlHlpElementRawType*)m_pT->getFirstChild();
#endif /*_USE_TI_XML__*/
}

template<class T>
T* XmlHlpNode<T>::nextSibling()
{
	_XASSERT(m_pT);
#ifdef _USE_TI_XML__
	return (XmlHlpElementRawType*)m_pT->NextSibling();
#elif _USE_TI2_XML__
	return (XmlHlpElementRawType*)m_pT->NextSibling();
#elif _USE_XERCESC_XML_
	return (XmlHlpElementRawType*)m_pT->getNextSibling();
#endif /*_USE_TI_XML__*/
}

template<class T>
T* XmlHlpNode<T>::firstChildElement()
{
	T* p = 	firstChild();
	if( p == NULL) {
		return NULL;
	}
	XmlHlpElementActl elm((XmlHlpElementRawType*)p);
	if( elm.getType() == XmlHlpNode_NodeType_ELEMENT ) {
		return p;
	}
	return elm.nextSiblingElement();
}

template<class T>
T* XmlHlpNode<T>::nextSiblingElement()
{
	_XASSERT(m_pT);
#ifdef _USE_TI_XML__
	return m_pT->NextSiblingElement();
#elif _USE_TI2_XML__
	return m_pT->NextSiblingElement();
#elif _USE_XERCESC_XML_
	XmlHlpElementRawType* p = (XmlHlpElementRawType*)m_pT->getNextSibling();
	while( p != NULL ) {
		if( p->getNodeType() == DOMNode::ELEMENT_NODE ) {
			return p;	
		}
		p = (XmlHlpElementRawType*)p->getNextSibling();
	}
	return NULL;
#endif /*_USE_TI_XML__*/
}

template<class T>
T* XmlHlpNode<T>::prevSiblingElement()
{
	_XASSERT(m_pT);
#ifdef PreviousSibling
	return m_pT->NextSiblingElement();
#elif _USE_XERCESC_XML_
	XmlHlpElementRawType* p = (XmlHlpElementRawType*)m_pT->getPrevSibling();
	while( p != NULL ) {
		if( p->getNodeType() == DOMNode::ELEMENT_NODE ) {
			return p;	
		}
		p = (XmlHlpElementRawType*)p->getPrevSibling();
	}
	return NULL;
#endif /*_USE_TI_XML__*/
}


typedef struct XmlHlpAtterData {
	XmlHlpAtterData(){ pAttrName = pAttrData = NULL;}
	char *pAttrName;
	char *pAttrData;
}*PXmlHlpAtterData;

template<class T>
T* XmlHlpNode<T>::findNode(XStrBuff& strPAth)
{
	char *pPath				= strPAth;
	char *pStart			= pPath;
	XStrBuff				pSerchNodeName;
	char *pWk				= NULL;
	bool  bAttr				= false;
	bool  bMore				= false;
	XmlHlpNode<T>			pfindNode;
	std::vector<XmlHlpAtterData*> attrList;

	pSerchNodeName = this->getNodeName();
	/*
		Path
		Node1[@attr1=1@attr2=2]/Node2/Node3
	*/
	while( *pPath != NULL ) {
		if((int)*pPath == (int)'/') {
			break;
		}
		pPath ++;
	}
	if((int)*pPath == (int)'/') {
		*pPath = 0;
		pPath++;
		/*
			pPath = Node2/Node3
			pStart = Node1[@attr1=1@attr2=2]
		*/
		bMore = true;
	}
	if( *pPath  == NULL ) {
		pPath = pStart;
	}
	pWk = pStart;
	while( *pWk != NULL ) {
		if((int)*pWk == (int)'[') {
			break;
		}
		pWk ++;
	}
	if((int)*pWk == (int)'[') {
		*pWk = 0;
		pWk++;
		/*
			pWk = @attr1=1@attr2=2]
			pStart = Node1
		*/
	}
	pSerchNodeName = pStart;

	// Analyze Attribute
	pStart = pWk;
	PXmlHlpAtterData pAttr = NULL;
	while( *pWk != NULL ) {
		if( *pWk == (int)'@') {
			if( pAttr != NULL ) {
				attrList.push_back(pAttr);	
			}
			pAttr = NULL;
			pAttr = new XmlHlpAtterData;
			*pWk = 0;
			pWk++;
			pAttr->pAttrName = pWk;
			continue;
		}
		if( *pWk == (int)']') {
			if( pAttr != NULL ) {
				*pWk = 0;
				pWk++;
				attrList.push_back(pAttr);
				pAttr = NULL;
				continue;
			}
			pAttr = NULL;
		}
		if( *pWk == (int)'=') {
			if( pAttr != NULL ) {
				*pWk = 0;
				pWk++;
				pAttr->pAttrData = pWk;
				continue;
			}
		}
		pWk ++;
	}
	if( pAttr != NULL ) {
		delete pAttr;
	}
	// Trim Attribute Data's
	{
		std::vector<XmlHlpAtterData*>::iterator ite;
		for (ite = attrList.begin();ite != attrList.end(); ++ite) {
			XmlHlpAtterData *pAttrInfo = *ite;
			if( *pAttrInfo->pAttrData == (int)'\"') {
				pAttrInfo->pAttrData++;
				if( pAttrInfo->pAttrData[strlen(pAttrInfo->pAttrData)-1] == (int)'\"' ) {
					pAttrInfo->pAttrData[strlen(pAttrInfo->pAttrData)-1] = 0;
				}
			}
			else if( *pAttrInfo->pAttrData == (int)'\'') {
				pAttrInfo->pAttrData++;
				if( pAttrInfo->pAttrData[strlen(pAttrInfo->pAttrData)-1] == (int)'\'' ) {
					pAttrInfo->pAttrData[strlen(pAttrInfo->pAttrData)-1] = 0;
				}
			}
		}
	}

	XmlHlpNode<T>	p;
	p = this->firstChild();
	while( p != NULL ) {
		if( p.getNodeName() == pSerchNodeName ) {
			std::vector<XmlHlpAtterData*>::iterator ite;
			ite = attrList.begin();
			if( ite == attrList.end() ) {
				pfindNode = p;
				break;
			} else {
				bool bEq = true;
				for( ; ite != attrList.end(); ite++) {
					XStrBuff attrDstVal;
					attrDstVal = p.getAttribute( (*ite)->pAttrName );
					if( strcmp(attrDstVal, (*ite)->pAttrData) != 0 ) {
						bEq = false;
						break;
					}
				}
				if( bEq == true ) {
					pfindNode = p;
					break;
				}
			}
		}
		p = p.nextSiblingElement();
	}
	// Delete AttrData
	{
		std::vector<XmlHlpAtterData*>::iterator ite;
		for (ite = attrList.begin();ite != attrList.end(); ++ite) {
			XmlHlpAtterData *pAttrInfo = *ite;
			delete pAttrInfo;
		}
		attrList.clear();
	}
	if( pfindNode == NULL ) {
		return NULL;
	}
	if(bMore == true) {
		XStrBuff strFindPath;
		strFindPath = pPath;
		return pfindNode.findNode(strFindPath);
	}
	return pfindNode;
}

template<class T>
T* XmlHlpNode<T>::clone()
{
	_XASSERT(m_pT);
	T* pNewVal = NULL;
#ifdef _USE_XERCESC_XML_
	pNewVal = m_pT->cloneNode( true );
#elif _USE_TI_XML__
	pNewVal = (T*)m_pT->Clone( );
#elif _USE_TI2_XML__
	pNewVal = (T*)m_pT->ShallowClone( m_pT->GetDocument() );
#endif /*_USE_TI_XML__*/
	return pNewVal;
}

template<class T>
XmlHlpElementRawType* XmlHlpNode<T>::addElement(const char* p_tag)
{
	_XASSERT(m_pT);
#ifdef _USE_XERCESC_XML_
	DOMDocument  *pDoc = m_pT->getOwnerDocument();
	DOMElement   *pElem = NULL;
	if( pDoc == NULL){
		if( m_pT->getNodeType() != DOMNode::DOCUMENT_NODE) {
			return NULL;
		}
		pDoc = (DOMDocument*)m_pT;
	}
	pElem = pDoc->createElement( XStrBuff(p_tag) );
	if( pElem == NULL)	return NULL;
	m_pT->appendChild(pElem);
	return pElem;
#elif _USE_TI_XML__
	TiXmlElement element( p_tag );
	return (XmlHlpElementRawType*) m_pT->InsertEndChild( element );
#elif _USE_TI2_XML__
	tinyxml2::XMLElement* pElm = m_pT->GetDocument()->NewElement( p_tag );
	return (XmlHlpElementRawType*) m_pT->InsertEndChild( pElm );
#endif /*_USE_TI_XML__*/
	return NULL;
}

template<class T>
XmlHlpElementRawType* XmlHlpNode<T>::addElement(const wchar_t* p_tag)
{
	_XASSERT(m_pT);
#ifdef _USE_XERCESC_XML_
	DOMDocument  *pDoc = m_pT->getOwnerDocument();
	DOMElement   *pElem = NULL;
	if( pDoc == NULL){
		if( m_pT->getNodeType() != DOMNode::DOCUMENT_NODE) {
			return NULL;
		}
		pDoc = (DOMDocument*)m_pT;
	}
	pElem = pDoc->createElement( p_tag );
	if( pElem == NULL)	return NULL;
	m_pT->appendChild(pElem);
	return pElem;
#elif _USE_TI_XML__
	TiXmlElement element( (const char*)XStrBuff(p_tag) );
	return (XmlHlpElementRawType*) m_pT->InsertEndChild( element );
#elif _USE_TI2_XML__
	tinyxml2::XMLElement* pElm = m_pT->GetDocument()->NewElement( (const char*)XStrBuff(p_tag) );
	return (XmlHlpElementRawType*) m_pT->InsertEndChild( pElm );
#endif /*_USE_TI_XML__*/
	return NULL;
}
template<class T>
XmlHlpElementRawType* XmlHlpNode<T>::insert(T* pInsItem, T* pPrev)
{
	_XASSERT(m_pT);
	XmlHlpElementActl elm = (XmlHlpElementRawType*)pInsItem;
#ifdef _USE_XERCESC_XML_
	T* pElm = elm.clone();
	if( pElm == NULL ) {
		return ;
	}
	return (XmlHlpElementRawType*) m_pT->insertBefore(pElm, pPrev);
#elif  _USE_TI_XML__
	if( pPrev == NULL ) {
		XmlHlpNode<T> apNode = pInsItem;
		T* pClone = apNode.clone();
		return (XmlHlpElementRawType*) m_pT->LinkEndChild( pClone );
	} else {
		return (XmlHlpElementRawType*) m_pT->InsertBeforeChild(pPrev, *pInsItem);
	}
#elif _USE_TI2_XML__
	if( pPrev == NULL ) {
		XmlHlpNode<T> apNode = pInsItem;
		T* pClone = apNode.clone();
		return (XmlHlpElementRawType*) m_pT->LinkEndChild( pClone );
	} else {
		return (XmlHlpElementRawType*) m_pT->InsertAfterChild(pPrev, pInsItem);
	}
#endif /*_USE_TI_XML__*/
	return NULL;
}


template<class T>
bool XmlHlpNode<T>::removeChild(T* removeThis)
{
#ifdef _USE_TI_XML__
	return m_pT->RemoveChild( (XmlHlpNodeRawType*)removeThis );
#elif _USE_TI2_XML__
	m_pT->DeleteChild( (XmlHlpNodeRawType*)removeThis );
	return true;
#elif _USE_XERCESC_XML_
	return m_pT->removeChild( (XmlHlpNodeRawType*)removeThis ) != NULL;
#endif /*_USE_TI_XML__*/
}
template<class T>
bool  XmlHlpNode<T>::removeChildren()
{
#ifdef _USE_TI_XML__
	TiXmlNode* p = 	(TiXmlNode*)this->firstChild();
	for(; p != NULL; ) {
		TiXmlNode* pPre = p;
		p = p->NextSibling();
		m_pT->RemoveChild( pPre );
	}
	return true;
#elif _USE_TI2_XML__
	TiXmlNode* p = 	(TiXmlNode*)this->firstChild();
	for(; p != NULL; ) {
		TiXmlNode* pPre = p;
		p = p->NextSibling();
		m_pT->RemoveChild( pPre );
	}
	return true;
#elif _USE_XERCESC_XML_
	DOMNode* p = 	(DOMNode*)this->getFirstChild();
	for(; p != NULL; ) {
		DOMNode* pPre = p;
		p = p->getNextSibling();
		m_pT->removeChild( pPre );
	}
	return true;
#endif /*_USE_TI_XML__*/
	return false;
}




//
//	XmlHlpDocument
//
template<class T>
bool XmlHlpDocument<T>::loadFromFile(const XStrBuff& strFile, 
									 XmlHlpErrReporter *pErrorReportergd)
{
#ifdef _USE_TI_XML__
	XStrBuff strFileName = strFile.c_str();
	if( m_doc.LoadFile( (const char*)strFileName ) == false ) {
		if( pErrorReportergd != NULL ) {
			pErrorReportergd->m_strErrString =  m_doc.ErrorDesc();
			pErrorReportergd->m_ulRow        =  m_doc.ErrorRow();
			pErrorReportergd->m_ulColum      =  m_doc.ErrorCol();
		}
		return false;
	}
	m_bOpend = true;
	return true;
#elif _USE_TI2_XML__
	XStrBuff strFileName = strFile.c_str();
	if( m_doc.LoadFile( (const char*)strFileName ) != tinyxml2::XML_SUCCESS ) {
		if( pErrorReportergd != NULL ) {
			pErrorReportergd->m_strErrString =  m_doc.GetErrorStr1();
			pErrorReportergd->m_ulRow        =  0;//m_doc.ErrorRow();
			pErrorReportergd->m_ulColum      =  0;//m_doc.ErrorCol();
		}
		return false;
	}
	m_bOpend = true;
	return true;
#elif _USE_XERCESC_XML_
	try {
		m_pDoc = NULL;
		m_pT =  NULL;
		if( m_pPaerse != NULL ) delete m_pPaerse;
		m_pPaerse = NULL;

		if( (m_pPaerse = new XercesDOMParser) == NULL ) {
			if(pErrorReportergd) {pErrorReportergd->m_strErrString = "Malloc";}
			return false;
		}
		m_pPaerse->setValidationScheme(XercesDOMParser::Val_Never);
		m_pPaerse->setDoNamespaces(false);
		m_pPaerse->setDoSchema(false);
		m_pPaerse->setValidationSchemaFullChecking(false);
		m_pPaerse->setCreateEntityReferenceNodes(false);
		if( pErrorReportergd ) {
			m_pPaerse->setErrorHandler(pErrorReportergd);
		}
		m_pPaerse->parse( (const XMLCh*)strFile.c_str() );
		
		if( pErrorReportergd ) {
			if( pErrorReportergd->getSawErrors() ) {
				if( m_pPaerse != NULL ) delete m_pPaerse;
				m_pPaerse = NULL;
				return false;
			} else {
				m_pDoc = m_pPaerse->getDocument();
				m_pT = (T*)m_pDoc;
			}
		} else {
			m_pDoc = m_pPaerse->getDocument();
			m_pT = (T*)m_pDoc;
		}
	}catch (const XMLException& e) {
		if(pErrorReportergd) {pErrorReportergd->m_strErrString = e.getMessage();}
		if( m_pPaerse != NULL ) delete m_pPaerse;
		m_pPaerse = NULL;
		return false;
	} catch (const DOMException& e) {
		if(pErrorReportergd) {pErrorReportergd->m_strErrString = e.msg;}
		if( m_pPaerse != NULL ) delete m_pPaerse;
		m_pPaerse = NULL;
		return false;
	} catch(...) {
		if(pErrorReportergd) {pErrorReportergd->m_strErrString = "Invalid Exception Occored";}
		if( m_pPaerse != NULL ) delete m_pPaerse;
		m_pPaerse = NULL;
		return false;
	}
	m_bOpend = true;
	return  true;
#endif /*_USE_TI_XML__*/
}


template<class T>
bool XmlHlpDocument<T>::saveFromFile(const  XStrBuff& strFile, XmlHlpErrReporter *pErrorReportergd)
{
//	if( m_bOpend == false ) {
//		if( pErrorReportergd != NULL ) {
//			pErrorReportergd->m_strErrString = _T("not opend.");
//		}
//		return false;
//	}
#ifdef _USE_TI_XML__
	XStrBuff strFileName = strFile.c_str();
	if( m_doc.SaveFile( strFileName ) == false ) {
		if( pErrorReportergd != NULL ) {
			pErrorReportergd->m_strErrString =  m_doc.ErrorDesc();
			pErrorReportergd->m_ulRow        =  m_doc.ErrorRow();
			pErrorReportergd->m_ulColum      =  m_doc.ErrorCol();
		}
		return false;
	}
	return true;
#elif _USE_TI2_XML__
	XStrBuff strFileName = strFile.c_str();
	if( m_doc.SaveFile( strFileName ) == false ) {
		if( pErrorReportergd != NULL ) {
			pErrorReportergd->m_strErrString =  m_doc.GetErrorStr1();
			pErrorReportergd->m_ulRow        =  0;//m_doc.ErrorRow();
			pErrorReportergd->m_ulColum      =  0;//m_doc.ErrorCol();
		}
		return false;
	}
	return true;
#elif _USE_XERCESC_XML_
	DOMWriter			*theSerializer = NULL;
	XMLFormatTarget		*myFormTarget = NULL;
	XStrBuff strTmpFileName = strFile;
	try {
		DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(L"Core");
		
		theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
		if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
		myFormTarget = new LocalFileFormatTarget( (const XMLCh*)strTmpFileName );
#ifdef WIN32
		theSerializer->setNewLine(L"\r\n");
		theSerializer->setEncoding(L"Shift-JIS");
#else
		theSerializer->setNewLine(L"\n");
#endif 
		theSerializer->writeNode(myFormTarget, *m_pDoc);

		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;

	}catch (const XMLException& e) {
		if(pErrorReportergd) {pErrorReportergd->m_strErrString = e.getMessage();}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	} catch (const DOMException& e) {
		if(pErrorReportergd) {pErrorReportergd->m_strErrString = e.msg;}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	} catch(...) {
		if(pErrorReportergd) {pErrorReportergd->m_strErrString = "Invalid Exception Occored";}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	}
	return  true;
#endif /*_USE_TI_XML__*/
	return false;
}

//
//	XmlHlpErrReporter
//
inline XmlHlpErrReporter::XmlHlpErrReporter()
	:	m_strErrString(_T(""))
	,	m_ulRow(0)
	,	m_ulColum(0)
	#ifdef _USE_XERCESC_XML_
	,	fSawErrors( false )
	#endif /*_USE_XERCESC_XML_*/
{


}
inline XmlHlpErrReporter::~XmlHlpErrReporter()
{
}



#endif /*__XML_FELPER__H__*/
