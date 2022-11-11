#include <iostream>
#include <fstream>
#include "XStrBuffCodeConvTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xstrbuff.h"
#include "xlib/xstrcordeconv.h"



/**
コンストラクタ
@param name [in] テスト名
*/
XStrBuffCodeConvTest::XStrBuffCodeConvTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XStrBuffCodeConvTest::setUp(void)
{
}


/** テスト後処理 */
void XStrBuffCodeConvTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XStrBuffCodeConvTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XStrBuffCodeConvTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffCodeConvTest>("XStrBuffCodeConvTest", &testConvUTF8) );
	
	return suite;
}


void XStrBuffCodeConvTest::testConvUTF8(void)
{
	XStrBuff	str;
	XStrBuff	str2;
	XStrBuff	str3;
	bool		bCesu = false;

	str = _T("あAいBうCえDおE");

	// UniCodeから、UTF8に変換
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertUnicodeToUTF8(str, str.GetLength(), str2, true) );
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertUTF8ToNative((const char*)(TCHAR*)str2, str3, &bCesu) );
	CPPUNIT_ASSERT( str.Compare( str3 ) == 0 );


	// Native=ASCII
	str2 = "";
	str3 = "";
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertNativeToUTF8("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", str2, true) );
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertUTF8ToNative((const char*)(TCHAR*)str2, str3, &bCesu) );
	CPPUNIT_ASSERT( str3.Compare( _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") ) == 0 );

	// Native=ASCII
	str2 = "";
	str3 = "";

	XStrBuffCodeConv::convertNativeToUTF8("かきくけこ", str2, true);
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertNativeToUTF8("かきくけこ", str2, true) );
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertUTF8ToNative((const char*)(TCHAR*)str2, str3, &bCesu) );
	CPPUNIT_ASSERT( str3.Compare( _T("かきくけこ") ) == 0 );

	// Native=Unicode
	str2 = "";
	str3 = "";
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertNativeToUTF8(L"指しス背祖", str2, true) );
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertUTF8ToNative((const char*)(TCHAR*)str2, str3, &bCesu) );
	CPPUNIT_ASSERT( str3.Compare( _T("指しス背祖") ) == 0 );

	
}
