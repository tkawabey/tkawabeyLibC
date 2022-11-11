#ifndef __X_XERCES_HELPER__
#define __X_XERCES_HELPER__

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
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#if _DEBUG
 #if WIN32
  #include <crtdbg.h>
  #ifdef _MT
   #ifdef _DLL
//      #pragma comment(lib, "MSVCRTD.LIB")
   #else
//      #pragma comment(lib, "LIBCMTD.LIB")
   #endif
  #else
//   #ifdef _DLL
//      #pragma comment(lib, "MSVCRT.LIB")
//   #else
//      #pragma comment(lib, "LIBCMT.LIB")
//   #endif
  #endif
  #define __OC_ASSERT(expr) \
        do { if (!(expr) && \
                (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))) \
             _CrtDbgBreak(); } while (0)

  #define __OC_TRACE(a)	::OutputDebugStringA(a);

 #else
   #include <assert.h>

   #define __OC_ASSERT(expr)  assert(expr);
  #define __OC_TRACE(a)	fprintf(stderr, "%s", a);
 #endif /*WIN32*/
#else 
  #define __OC_ASSERT(expr) ((void)0)
  #define __OC_TRACE(a) ((void)0)
#endif /*_DEBUG*/


XERCES_CPP_NAMESPACE_USE
///////////////////////////////////////////////////////////////////////////////////
//
// Class defained
//
///////////////////////////////////////////////////////////////////////////////////

/** 
 * Xerces'DLL 初期化
 */
struct XercesInitilizer {
	XercesInitilizer(){
		try { XMLPlatformUtils::Initialize(); } catch(...){}
	}
	~XercesInitilizer(){
		Release();
	}
	void Release(){
		try { XMLPlatformUtils::Terminate(); } catch(...){}
	}
};

/** 
 * 文字列バッファ(文字コードに違いを吸収する薄いラッパー)
 */
class XercesString {
public:
	XercesString(){
		m_pUTF8Str = NULL;m_pMultiStr = NULL;
	}
	XercesString(XMLCh* p){
		m_pUTF8Str = NULL;m_pMultiStr = NULL;*this = p;
	}
	XercesString(char* p){
		m_pUTF8Str = NULL;m_pMultiStr = NULL;*this = p;
	}
	XercesString(const XMLCh* p){
		m_pUTF8Str = NULL;m_pMultiStr = NULL;*this = p;
	}
	XercesString(const char* p){
		m_pUTF8Str = NULL;m_pMultiStr = NULL;*this = p;
	}
	XercesString(const XercesString &src)
	{
		const XMLCh* pU = src.m_pUTF8Str;
		const char*  pM = src.m_pMultiStr;
		m_pUTF8Str = NULL;m_pMultiStr = NULL;
		if(pU != NULL) { *this = pU; }
		if(pM != NULL) { *this = pM; }
	}
	~XercesString(){
		Clear();
	}
	void Clear() {
		if(m_pUTF8Str) XMLString::release(&m_pUTF8Str);
		m_pUTF8Str = NULL;
		if(m_pMultiStr) XMLString::release(&m_pMultiStr);
		m_pMultiStr = NULL;
	}
	int length(){ 
		if(m_pUTF8Str == NULL && m_pMultiStr == NULL ){return 0;}
		return ::strlen(*this);
	}
	operator XMLCh*(){
		if( m_pUTF8Str != NULL ) return m_pUTF8Str;
		if( m_pMultiStr == NULL ) return L"";
		m_pUTF8Str = XMLString::transcode(m_pMultiStr);
		return m_pUTF8Str;
	}
	operator const XMLCh*(){ return (XMLCh*)*this; }
	operator char*(){
		if( m_pMultiStr != NULL ) return m_pMultiStr;
		if( m_pUTF8Str == NULL ) return "";
		m_pMultiStr = XMLString::transcode(m_pUTF8Str);
		return m_pMultiStr;
	}
	operator const char*(){ return (char*)*this; }

	const XMLCh *getUTF8Str(){ return (const XMLCh*)*this;}
	const char *getMultiStr(){ return (const char*)*this;}
	const XercesString& operator=(const long n){
		Clear();
		XercesString tmp;
		tmp.Format("%d", n);
		*this = tmp.getUTF8Str();
		return *this;
	}
	const XercesString& operator=(const char* const pStr){
		Clear();
		m_pUTF8Str = XMLString::transcode(pStr);
		return *this;
	}
	const XercesString& operator=(const XMLCh* const pStr){
		Clear();
		m_pMultiStr = XMLString::transcode(pStr);
		return *this;
	}
	const XercesString& operator=(XercesString &pStr){
		*this = pStr.getUTF8Str();
		return *this;
	}
	int Format(const char *lpszFormat, ...){
		char sz[1024];
		va_list argList;
		va_start(argList, lpszFormat);
		vsprintf(sz, lpszFormat, argList);
		va_end(argList);
		*this = sz;
		return ::strlen(sz);
	}
	int SeqF(const char *p, unsigned long  n) {
		return Format("%s%d", p, n);
	}
private:
	XMLCh *m_pUTF8Str;
	char  *m_pMultiStr;
	int	  m_nRef;
};

/** 
 * DOMNodeインスタンスの派生クラスを操作するヘルパー関数
 */
template <class T>
class DOMNodeHelper {
public:
	T* m_pT;
	
	DOMNodeHelper(){m_pT = NULL;}
	DOMNodeHelper(T* p){ m_pT = p;}
	operator T*(){ return m_pT;}
	T& operator*() const{return *m_pT;}
	T** operator&(){return &m_pT;}
	T* operator->() const{return m_pT;}
	bool operator!() const{	return (m_pT == NULL); }
	bool operator<(T* pT) const{ return m_pT < pT; }
	bool operator==(T* pT) const { return m_pT == pT; }
	bool operator!=(T* pT) const { return m_pT != pT; }
	T* operator=(const DOMNodeHelper<T>& lp){m_pT = lp.m_pT;return m_pT;}
	const DOMNodeHelper<T>& operator=(T* p){m_pT = p; return *this;}

	unsigned long CountElement();
	bool isLeaf(){	return CountElement() == 0;	}
	DOMNode* findNode(XercesString &strPAth, XMLSize_t *puResum = NULL);
	bool Vaidata(){ return m_pT != NULL; }
	/**
	 * GetNodeText
	 * @param buff buffer
	 * @param nMd	0:TEXT_NODE	1:CDATA_SECTION_NODE	2:0 or 1
	 */
	bool GetNodeText(XercesString &buff, int nMd = 0);
	DOMText *GetNodeText(int nMd = 0);
	/**
	 * GetNodeText
	 * @param buff buffer
	 * @param nMd	0:TEXT_NODE	1:CDATA_SECTION_NODE	2:0 or 1
	 */
	bool SetNodeText(const XMLCh*  src,  int nMd = 0);
	int CompareNodeName(const XMLCh*  src)
	{
		__OC_ASSERT(m_pT);
		const XMLCh* p1 = m_pT->getNodeName();
		const XMLCh* p2 = src;
		return ::wcscmp(p1, p2);
	}
// ↓	TK034
	int CompareI_NodeName(const XMLCh*  src)
	{
		__OC_ASSERT(m_pT);
		const XMLCh* p1 = m_pT->getNodeName();
		const XMLCh* p2 = src;
		return ::wcsicmp(p1, p2);
	}
// ↑	TK034
	int CompareNodeName(const char*  src){XercesString p2 = src;return this->CompareNodeName((const XMLCh*)p2);}
	bool GetAttribute(const XMLCh*  src, XercesString &buff);
	bool GetAttribute(const char*  src, XercesString &buff){XercesString p2 = src;return this->GetAttribute((const XMLCh*)p2, buff);}
	DOMAttr* GetAttribute(const XMLCh* src); 
	DOMAttr* GetAttribute(const char* src){XercesString p2 = src;return this->GetAttribute((const XMLCh*)p2);}
	bool SetAttribute(XercesString attrName, XercesString attrVariavle) {
		__OC_ASSERT(m_pT);
		try {
			if( m_pT->getAttributes() == NULL ) {
				return false;
			}
			DOMAttr *pAttr = (DOMAttr*)m_pT->getAttributes()->getNamedItem(attrName);
			if( pAttr != NULL ) {
				pAttr->setValue(attrVariavle);
				return true;
			}
			DOMDocument *pDoc = m_pT->getOwnerDocument();
			if(pDoc == NULL) {
				return false;
			}
			pAttr = pDoc->createAttribute(attrName);
			if(pAttr == NULL) {
				return false;
			}
			pAttr->setValue(attrVariavle);
			m_pT->getAttributes()->setNamedItem(pAttr);
		}catch(...){return false;}
		return true;
	}
	
	
	/**
	 * 子ノード数を取得します。
	 * @return 子ノード数
	 */
	XMLSize_t getChildCount();
	/**
	 * 指定したインデックスの子ノードを取得
	 * @param i 取得する子ノードのインデックス
	 * @return 子ノード。インデックスが間違っている場合はNULL
	 */
	DOMNode* getChileItem(XMLSize_t i);
	void Release();

	DOMElement *AddElement(XercesString tagName, bool bAppendChiled = true);



};


/** 
 * DOMNodeインスタンス自動開放
 */
template <class T>
class DOMDocReleaser : public DOMNodeHelper<T>{
public:
	DOMDocReleaser(T *p){
		DOMNodeHelper<T> *pp = (DOMNodeHelper<T>*)this;
		*pp = p;
	}
	~DOMDocReleaser(){
		if( m_pT != NULL ) {m_pT->release();}
		m_pT = NULL;
	}
	T *m_pDoc;
};

/** 
 * XMLデータ読込エラー報告
 */
class XercesReaderReporter : public ErrorHandler
{
public:
	XercesReaderReporter(){
		fSawErrors = false;
		m_strSystemID = "";
		m_strFastErrMsg = "";
		m_nFastErrLine = -1;
		m_nFastErrColumn = -1;
	}
	~XercesReaderReporter(){}
	virtual void warning(const SAXParseException& toCatch){}
	virtual void error(const SAXParseException& toCatch){
		if(fSawErrors == false) {
			m_strSystemID = toCatch.getSystemId();
			m_strFastErrMsg  = toCatch.getMessage();
			m_nFastErrLine = toCatch.getLineNumber();
			m_nFastErrColumn = toCatch.getColumnNumber();
			fSawErrors = true;
		}
	}
	virtual void fatalError(const SAXParseException& toCatch) {
		this->error(toCatch);
	}
	virtual void resetErrors(){ fSawErrors = false;}
	virtual bool getSawErrors() const{ return fSawErrors;}
	
	bool    fSawErrors;
	XercesString  m_strSystemID;
	XercesString  m_strFastErrMsg;
	unsigned long m_nFastErrLine;
	unsigned long m_nFastErrColumn;
};



///////////////////////////////////////////////////////////////////////////////////
//
// Function's
//
///////////////////////////////////////////////////////////////////////////////////
inline bool XercesRead(XercesString fileName, 
					   XercesReaderReporter *pErrHndler, 
					   XercesDOMParser **ppPaeser) {
	try {
		if( (*ppPaeser = new XercesDOMParser) == NULL ) {
			if(pErrHndler) {pErrHndler->m_strFastErrMsg = "Malloc";}
			return false;
		}
		(*ppPaeser)->setValidationScheme(XercesDOMParser::Val_Never);
		(*ppPaeser)->setDoNamespaces(false);
		(*ppPaeser)->setDoSchema(false);
		(*ppPaeser)->setValidationSchemaFullChecking(false);
		(*ppPaeser)->setCreateEntityReferenceNodes(false);
		if( pErrHndler ) {
			(*ppPaeser)->setErrorHandler(pErrHndler);
		}
		(*ppPaeser)->parse( (const XMLCh*)fileName );
		if( pErrHndler ) {
			if( pErrHndler->getSawErrors() ) {
				if( *ppPaeser != NULL ) delete *ppPaeser;
				return false;
			}
		}

	}catch (const XMLException& e) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = e.getMessage();}
		if( *ppPaeser != NULL ) delete *ppPaeser;
		return false;
	} catch (const DOMException& e) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = e.msg;}
		if( *ppPaeser != NULL ) delete *ppPaeser;
		return false;
	} catch(...) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = "Invalid Exception Occored";}
		if( *ppPaeser != NULL ) delete *ppPaeser;
		return false;
	}
	return  true;
}
inline bool XercesReadFromMomory(char *pDocMemory, 
					   XercesReaderReporter *pErrHndler, 
					   XercesDOMParser **ppPaeser) {
	try {
		if( (*ppPaeser = new XercesDOMParser) == NULL ) {
			if(pErrHndler) {pErrHndler->m_strFastErrMsg = "Malloc";}
			return false;
		}
		(*ppPaeser)->setValidationScheme(XercesDOMParser::Val_Never);
		(*ppPaeser)->setDoNamespaces(false);
		(*ppPaeser)->setDoSchema(false);
		(*ppPaeser)->setValidationSchemaFullChecking(false);
		(*ppPaeser)->setCreateEntityReferenceNodes(false);
		if( pErrHndler ) {
			(*ppPaeser)->setErrorHandler(pErrHndler);
		}
	    MemBufInputSource memoryData((const XMLByte*)pDocMemory,
									 strlen(pDocMemory),
									 "MemoryData", false);

		(*ppPaeser)->parse( memoryData );
		if( pErrHndler ) {
			if( pErrHndler->getSawErrors() ) {
				if( *ppPaeser != NULL ) delete *ppPaeser;
				return false;
			}
		}

	}catch (const XMLException& e) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = e.getMessage();}
		if( *ppPaeser != NULL ) delete *ppPaeser;
		return false;
	} catch (const DOMException& e) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = e.msg;}
		if( *ppPaeser != NULL ) delete *ppPaeser;
		return false;
	} catch(...) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = "Invalid Exception Occored";}
		if( *ppPaeser != NULL ) delete *ppPaeser;
		return false;
	}
	return  true;
}
inline bool XercesWrite(XercesString fileName, 
					   XercesReaderReporter *pErrHndler, 
					   DOMDocument* pDomDoc) {
	DOMWriter			*theSerializer = NULL;
	XMLFormatTarget		*myFormTarget = NULL;
	try {
		DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(L"Core");
		
		theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
		if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
		myFormTarget = new LocalFileFormatTarget( (const XMLCh*)fileName );
#ifdef WIN32
		theSerializer->setNewLine(L"\r\n");
#else
		theSerializer->setNewLine(L"\n");
#endif 
		theSerializer->writeNode(myFormTarget, *pDomDoc);

		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;

	}catch (const XMLException& e) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = e.getMessage();}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	} catch (const DOMException& e) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = e.msg;}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	} catch(...) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = "Invalid Exception Occored";}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	}
	return  true;
}
inline bool XercesWriteToMemory(XercesReaderReporter *pErrHndler, 
					   DOMDocument* pDomDoc,
					   XercesString &outBuff,
						int initCapacity = 4023
					   ) {
	DOMWriter			*theSerializer = NULL;
	XMLFormatTarget		*myFormTarget = NULL;
	try {
		DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(L"Core");
		
		theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
		if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
			theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
		myFormTarget = new MemBufFormatTarget( initCapacity );

		theSerializer->writeNode(myFormTarget, *pDomDoc);

		outBuff = (const char*)((MemBufFormatTarget*)myFormTarget)->getRawBuffer();
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;

	}catch (const XMLException& e) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = e.getMessage();}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	} catch (const DOMException& e) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = e.msg;}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	} catch(...) {
		if(pErrHndler) {pErrHndler->m_strFastErrMsg = "Invalid Exception Occored";}
		if( theSerializer != NULL )
			delete theSerializer;
		if( myFormTarget != NULL )
			delete myFormTarget;
		return false;
	}
	return  true;
}
inline DOMDocument* XercesNewDocument(XercesString strRoot,const XMLCh* pCore = L"Core" ) {
	DOMDocument *pDomDoc = NULL;
	try {
		DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(pCore);
		pDomDoc = impl->createDocument(0,strRoot,0);
	} catch(...) {
		return NULL;
	}
	return pDomDoc;
}

///////////////////////////////////////////////////////////////////////////////////
//
// implementation
//
///////////////////////////////////////////////////////////////////////////////////
template<class T>
unsigned long DOMNodeHelper<T>::CountElement()
{
	register unsigned long cnt = 0;
	register DOMNodeList *plist = m_pT->getChildNodes();
	register XMLSize_t nLen = plist->getLength();

	
	DOMNode *pFirst = m_pT->getFirstChild();
	if( pFirst == NULL ) {
		return 0;
	}
#if 1
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
#else
// Kawabuchiさん最新のチェックインは↑のコード（ただしfindNodeのResume無い）
	DOMNode *pNodeTemp = pFirst;
	for(;;) {
		DOMNode *p = pNodeTemp->getNextSibling();
		if( p == NULL ) {
			break;
		}
		if( p->getNodeType() == DOMNode::ELEMENT_NODE ) cnt++;
		pNodeTemp = p;
	}
	return cnt;
#endif
//********/
}
typedef struct AtterData {
	AtterData(){ pAttrName = pAttrData = NULL;}
	char *pAttrName;
	char *pAttrData;
}*PAtterData;
template<class T>
DOMNode* DOMNodeHelper<T>::findNode(XercesString &strPAth, XMLSize_t *puResum /* = NULL */) {
	char *pPath		  = strPAth;
	char *pStart		  = pPath;
	XercesString pSerchNodeName;
	char *pWk			  = NULL;
	bool  bAttr = false;
	bool  bMore = false;
	DOMNodeHelper<DOMNode> findNode;
	std::vector<AtterData*> attrList;

	__OC_ASSERT(m_pT);
	
	pSerchNodeName = m_pT->getNodeName();
	// Not Have Child
	if( m_pT->hasChildNodes() == false ) {
		return NULL;
	}

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
	PAtterData pAttr = NULL;
	while( *pWk != NULL ) {
		if( *pWk == (int)'@') {
			if( pAttr != NULL ) {
				attrList.push_back(pAttr);	
			}
			pAttr = NULL;
			pAttr = new AtterData;
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
		std::vector<AtterData*>::iterator ite;
		for (ite = attrList.begin();ite != attrList.end(); ++ite) {
			AtterData *pAttrInfo = *ite;
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



	// Find Node
	DOMNodeList *plist = m_pT->getChildNodes();
	XMLSize_t i = 0;
	int	length = plist->getLength();

	if( puResum != NULL ) {
		i = *puResum;

		if( i > length ) {
			return NULL;
		}
	}

	DOMNodeHelper<DOMNode> p;

	for(; i < length; i++){
		p = plist->item(i);

		if( p->getNodeType() != DOMNode::ELEMENT_NODE ) continue;
//	↓	TK034
		if( wcsicmp(p->getNodeName(), pSerchNodeName) == 0 ) {
//	↑	TK034

			std::vector<AtterData*>::iterator ite;
			ite = attrList.begin();
			if( ite == attrList.end() ) {
				findNode = p;
				break;
			} else {
				bool bEq = true;
				for (ite = attrList.begin();ite != attrList.end(); ++ite) {
					// Agree Attribute
					XercesString attrDstVal;
					AtterData *pAttrInfo = *ite;
					if( p.GetAttribute(pAttrInfo->pAttrName, attrDstVal) == false ) {
						bEq = false;
						continue;
					}
//	↓	TK034
					if( wcsicmp(attrDstVal, XercesString(pAttrInfo->pAttrData)) != 0 ) {
//	↑	TK034
						bEq = false;
					}
				}
				if( bEq == true ) {
					findNode = p;
					break;
				}
			}
		}
	}
	// Delete AttrData
	{
		std::vector<AtterData*>::iterator ite;
		for (ite = attrList.begin();ite != attrList.end(); ++ite) {
			AtterData *pAttrInfo = *ite;
			delete pAttrInfo;
		}
	}

	
	if( (T*)findNode == NULL ) {
		return NULL;
	}

	if( puResum != NULL ) {
		*puResum = i;
	}
	if(bMore == true) {
		XercesString strFindPath;
		strFindPath = pPath;
		return findNode.findNode(strFindPath);
	}
	return findNode.m_pT;
}

// implement
template<class T>
DOMText* DOMNodeHelper<T>::GetNodeText(int nMd /*= 0*/) {
	// Not Have Child
	if( m_pT->hasChildNodes() == false ) {
		return NULL;
	}
	DOMNodeList *plist = m_pT->getChildNodes();
	for(XMLSize_t i = 0; i < plist->getLength(); i++){
		DOMNode *p = plist->item(i);
		if( p->getNodeType() == DOMNode::TEXT_NODE  && 
			( nMd == 0 || nMd == 2 )) {
			return static_cast<DOMText*>(p);
		}
		if( p->getNodeType() == DOMNode::CDATA_SECTION_NODE  && 
			( nMd == 1 || nMd == 2 )) {
			return static_cast<DOMText*>(p);
		}
	}
	return NULL;
}
template<class T>
bool DOMNodeHelper<T>::GetNodeText(XercesString &buff, int nMd /*= 0*/) {
	
	DOMText* pTxt = this->GetNodeText(nMd);
	if(pTxt == NULL ) {
		return false;
	}
	buff = pTxt->getNodeValue();
	return true;
}
template<class T>
bool DOMNodeHelper<T>::SetNodeText(const XMLCh*  src,  int nMd/*= 0*/)
{
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
	return true;
}
template<class T>
DOMAttr* DOMNodeHelper<T>::GetAttribute(const XMLCh* src)
{
	__OC_ASSERT(m_pT);	
	if( m_pT->hasAttributes() == false ) {
		return false;
	}
	DOMNamedNodeMap *pAttrs = m_pT->getAttributes();
	__OC_ASSERT(pAttrs);	
	DOMNode   *pAttr = pAttrs->getNamedItem(src);
	return (DOMAttr*)pAttr;
}
template<class T>
bool DOMNodeHelper<T>::GetAttribute(const XMLCh*  src, XercesString &buff)
{
	__OC_ASSERT(m_pT);	
	DOMAttr *p = GetAttribute(src);
	if( p == NULL ) {
		return false;
	}
	buff = p->getNodeValue();
	return true;
}
template<class T>
XMLSize_t DOMNodeHelper<T>::getChildCount(){
	__OC_ASSERT(m_pT);
	if( m_pT->hasChildNodes() == false ) {
		return 0;
	}
	DOMNodeList *list = m_pT->getChildNodes();
	__OC_ASSERT(list);
	return list->getLength();
}
template<class T>
DOMNode* DOMNodeHelper<T>::getChileItem(XMLSize_t i) {
	__OC_ASSERT(m_pT);
	if( m_pT->hasChildNodes() == false ) {
		return NULL;
	}
	DOMNodeList *list = m_pT->getChildNodes();
	__OC_ASSERT(list);
	return list->item(i);
}
template<class T>
void DOMNodeHelper<T>::Release() {
	__OC_ASSERT(m_pT);
	m_pT->release();
	m_pT = NULL;
}
template<class T>
DOMElement* DOMNodeHelper<T>::AddElement(XercesString tagName, bool bAppendChiled/* = true*/) {
	__OC_ASSERT(m_pT);
	DOMDocument  *pDoc = m_pT->getOwnerDocument();
	DOMElement   *pElem = NULL;
	if( pDoc == NULL)	return NULL;
	pElem = pDoc->createElement(tagName);
	if( pElem == NULL)	return NULL;
	m_pT->appendChild(pElem);
	return pElem;
}
#endif /*__X_XERCES_HELPER__*/
