#include <iostream>
#include <fstream>
#include "xmlhelperTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xmlhelper.h"
#include "xlib/xeception.h"




/**
コンストラクタ
@param name [in] テスト名
*/
xmlhelperTest::xmlhelperTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void xmlhelperTest::setUp(void)
{
}


/** テスト後処理 */
void xmlhelperTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* xmlhelperTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("xmlhelperTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<xmlhelperTest>("xmlhelperTest2", &testRead2) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xmlhelperTest>("xmlhelperTest", &testRead) );
	
	return suite;
}
void xmlhelperTest::testRead(void)
{
	XmlHlpDocumentActl	doc;
	XmlHlpErrReporter	err;
	XmlHlpElementActl	elm1;
	XmlHlpElementActl	elm2;

	// ドキュメントをオープン
	CPPUNIT_ASSERT( doc.loadFromFile(XStrBuff("XSEMIModels.xml"), &err) );
	// 子ノードの数
	CPPUNIT_ASSERT( doc.countElement()==1 );
	// 最初の子ノードを取得
	elm1 = (XmlHlpElementRawType*)doc.firstChild();
	CPPUNIT_ASSERT( elm1 != NULL );
	// 子ノードのタグ名チェック
	CPPUNIT_ASSERT( elm1.getNodeName() == _T("XSEMIModels") );

	// 
	for(	elm2 = elm1.firstChild(); 
			elm2 != NULL; 
			elm2 = elm2.nextSiblingElement() ) {

		std::cout << (const TCHAR*)elm2.getNodeName() << "  name="
				  << (const TCHAR*)elm2.getAttribute("name") << std::endl;
	}
}


const TCHAR* getConvertType(const TCHAR* pVal) 
{
		 if( XStrBuff::StrCmp(pVal, _T("binary")) == 0 ) { return _T("B"); }
	else if( XStrBuff::StrCmp(pVal, _T("boolean")) == 0 ) { return _T("BOOL"); }
	else if( XStrBuff::StrCmp(pVal, _T("ascii")) == 0 ) { return _T("A"); }
	else if( XStrBuff::StrCmp(pVal, _T("uint1")) == 0 ) { return _T("U1"); }
	else if( XStrBuff::StrCmp(pVal, _T("uint2")) == 0 ) { return _T("U2"); }
	else if( XStrBuff::StrCmp(pVal, _T("uint4")) == 0 ) { return _T("U4"); }
	else if( XStrBuff::StrCmp(pVal, _T("uint8")) == 0 ) { return _T("U8"); }
	else if( XStrBuff::StrCmp(pVal, _T("uintx")) == 0 ) { return _T("UX"); }
	else if( XStrBuff::StrCmp(pVal, _T("int1")) == 0 ) { return _T("I1"); }
	else if( XStrBuff::StrCmp(pVal, _T("int2")) == 0 ) { return _T("I2"); }
	else if( XStrBuff::StrCmp(pVal, _T("int4")) == 0 ) { return _T("I4"); }
	else if( XStrBuff::StrCmp(pVal, _T("int8")) == 0 ) { return _T("I8"); }
	else if( XStrBuff::StrCmp(pVal, _T("intx")) == 0 ) { return _T("IX"); }
	else if( XStrBuff::StrCmp(pVal, _T("float4")) == 0 ) { return _T("F4"); }
	else if( XStrBuff::StrCmp(pVal, _T("float8")) == 0 ) { return _T("F8"); }
	else if( XStrBuff::StrCmp(pVal, _T("floatx")) == 0 ) { return _T("FX"); }
	else if( XStrBuff::StrCmp(pVal, _T("list")) == 0 ) { return _T("L"); }

//	_ASSERT(0);

	return _T("");
}

void loadList(XmlHlpElementActl& org, XmlHlpElementActl dst)
{
	XmlHlpElementActl	elm;
	for(	elm = org.firstChild(); 
			elm != NULL; 
			elm = elm.nextSiblingElement() ) {

		if( elm.getNodeName() == _T("list") ) {




			XmlHlpElementActl elemList = dst.addElement(_T("list"));

			if( elm.isAttribute( _T("cnt") ) ) {
				elemList.setAttribute( _T("cnt") , elm.getAttribute( _T("cnt") ) );
			}

			loadList( elm, elemList);
		} else 
		if( elm.getNodeName() == _T("item") ) {
			XmlHlpElementActl elemLItem = dst.addElement(_T("item"));
			elemLItem.setAttribute( _T("dstItem") , elm.getValue() );
		} else {

			_ASSERT(0);

		}
	}
}

void xmlhelperTest::testRead2(void)
{
	XmlHlpDocumentActl	doc;
	XmlHlpDocumentActl	docWrit;
	XmlHlpErrReporter	err;
	XmlHlpElementActl	elm1;
	XmlHlpElementActl	elm2;


	XmlHlpElementActl writeRoot = docWrit.addElement(_T("secs"));
	XmlHlpElementActl itemTypes = writeRoot.addElement(_T("itemTypes"));
	XmlHlpElementActl messages  = writeRoot.addElement(_T("messages"));


	// ドキュメントをオープン
	CPPUNIT_ASSERT( doc.loadFromFile(XStrBuff("xsecs.xml"), &err) );
	// 子ノードの数
	CPPUNIT_ASSERT( doc.countElement()==1 );
	// 最初の子ノードを取得
	elm1 = (XmlHlpElementRawType*)doc.firstChildElement();
	CPPUNIT_ASSERT( elm1 != NULL );
	// 子ノードのタグ名チェック
	CPPUNIT_ASSERT( elm1.getNodeName() == _T("secs") );

	// 
	for(	elm2 = elm1.firstChild(); 
			elm2 != NULL; 
			elm2 = elm2.nextSiblingElement() ) {

		if( elm2.getNodeName() == _T("itemTypes") ) {

			XmlHlpElementActl	elm3;
			elm3 = elm2.firstChild(); 
			for(	; 
					elm3 != NULL; 
					elm3 = elm3.nextSiblingElement() ) {

				XmlHlpElementActl itemType = itemTypes.addElement((const TCHAR*)elm3.getNodeName());
				XStrBuff strType1 = elm3.getAttribute("type");
				XStrBuff strType2 = getConvertType( strType1 );
				itemType.setAttribute( _T("name") , elm3.getAttribute("name") );
				itemType.setAttribute( _T("type") , strType2 );

				XmlHlpElementActl elemFormats = itemType.addElement(_T("formats"));
				XmlHlpElementActl elemValues  = itemType.addElement(_T("values"));


				XmlHlpElementActl	elm4;
				elm4 = elm3.firstChild(); 
				for(;elm4 != NULL; elm4 = elm4.nextSiblingElement() ) {

					if( elm4.getNodeName() == _T("format") ) {

						strType1 = elm4.getValue();
						XmlHlpElementActl elemFormat = elemFormats.addElement(_T("format"));


						strType2 = getConvertType( strType1 );
						elemFormat.setAttribute( _T("type") , strType2 );
						if( strType2 == _T("L") ) {
							loadList(elm4, elemFormat);
						} 

						if( elm4.isAttribute( _T("size_min") ) ) {
							elemFormat.setAttribute( _T("size_min") , elm4.getAttribute( _T("size_min") ) );
						}
						if( elm4.isAttribute( _T("size_max") ) ) {
							elemFormat.setAttribute( _T("size_max") , elm4.getAttribute( _T("size_max") ) );
						}


					}
				}
			}
		} else 
		if( elm2.getNodeName() == _T("messages") ) {
			XmlHlpElementActl	elm3;
			elm3 = elm2.firstChild(); 
			for(	; 
					elm3 != NULL; 
					elm3 = elm3.nextSiblingElement() ) {

				XmlHlpElementActl elemMsg = messages.addElement((const TCHAR*)elm3.getNodeName());

				XStrBuff strS = elm3.getAttribute("s");
				XStrBuff strF = elm3.getAttribute("f");
				XStrBuff strdir = elm3.getAttribute("dir");
				XStrBuff strreply = elm3.getAttribute("reply");
				XStrBuff strmulti = elm3.getAttribute("multi");
				XStrBuff strcategory = elm3.getAttribute("category");
				XStrBuff straction = elm3.getAttribute("action");

				if( strS == _T("") ) {
					XStrBuff strSF = elm3.getAttribute("sf");
					TCHAR* p = strSF;
					TCHAR* pFirst;
					if( *p != _T('S') ) {
						_XASSERT(0);
					}
					p++;
					pFirst = p;
					while( ('0' <= *p && *p <= '9' )) {

						p++;
					}
					if( *p != _T('F') ) {
						_XASSERT(0);
					}
					*p = 0;	p++;

					strS = pFirst;
					strF = p;
				}
				
				elemMsg.setAttribute( _T("s") , strS );
				elemMsg.setAttribute( _T("f") , strF );
				if( XStrBuff::StrLen((const TCHAR*)straction) != 0 ) {
//				if( straction != _T("") ){
					elemMsg.setAttribute( _T("action") , straction );
				}
				if( XStrBuff::StrLen((const TCHAR*)strdir) != 0 ) {
//				if( strdir != _T("") ){
					elemMsg.setAttribute( _T("dir") , strdir );
				}
				if( XStrBuff::StrLen((const TCHAR*)strreply) != 0 ) {
//				if( strreply != _T("") ){
					elemMsg.setAttribute( _T("reply") , strreply );
				}
				if( XStrBuff::StrLen((const TCHAR*)strmulti) != 0 ) {
//				if( strmulti != _T("") ){
					elemMsg.setAttribute( _T("multi") , strmulti );
				}
				if( XStrBuff::StrLen((const TCHAR*)strcategory) != 0 ) {
//				if( strcategory != _T("") ){
					elemMsg.setAttribute( _T("category") , strcategory );
				}


				XmlHlpElementActl	elm4;
				elm4 = elm3.firstChild(); 
				for(	; 
						elm4 != NULL; 
						elm4 = elm4.nextSiblingElement() ) {

					if( elm4.getNodeName() == _T("body") ) {

						XmlHlpElementActl elemBody = elemMsg.addElement(_T("body"));

						loadList(elm4, elemBody);
					}
				}
			}
		}
	}


	//writeRoot.insert(messages, itemTypes);
	//writeRoot.removeChild(messages);

	docWrit.saveFromFile(XStrBuff("xsecs2.xml"), &err) ;
}
