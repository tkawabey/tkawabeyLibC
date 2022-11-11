#include <iostream>
#include <fstream>
#include "XSecsTextParserTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/semi/secs/xsecs.h"
#include "xlib/semi/secs/xsecs_text_parser.h"
#include "xlib/xeception.h"
#include "xlib/xbuff.h"
#include "xlib/xstrbuff.h"

#include <windows.h>
#include <strstream>
/**
コンストラクタ
@param name [in] テスト名
*/
XSecsTextParserTest::XSecsTextParserTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XSecsTextParserTest::setUp(void)
{
}


/** テスト後処理 */
void XSecsTextParserTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XSecsTextParserTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSecsTextParserTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testparseI4) );

	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testS2F3_Bin) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testParseBinary) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testparseU1) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testparseI1) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testparseU2) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testparseI2) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testparseU4) );

	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsTextParserTest>("XSecsTextParserTest", &testNomalAccess) );


	return suite;
}



void XSecsTextParserTest::testNomalAccess(void)
{
	try {
		XSecsTextParser parser;
		XSecsData data;

		CPPUNIT_ASSERT( parser.parseFromFile("XSecsDataTest_testBuildBinary.sml.unitest", &data) );

		TCHAR szBuff[500];
		TCHAR *p = szBuff;
		::memset(szBuff, 0, sizeof(szBuff));



		{
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff), _T("\"ABCDEFG\"\r\n")
						   _T("\"GGGGGGG\">"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData aaa(XDATA_TYPE_A);
			p = szBuff;
			parser.parseStringFromString(&p, len, &aaa);
		}

		parser.parseFromFile("XSecsDataTest_testBuildBinary.sml.unitest", &data);
	} catch(XExcept &e) {
		std::cerr << "testNomalAccess ERR" << std::endl;
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void XSecsTextParserTest::testParseBinary()
{
	try {
		

		TCHAR szBuff[500];
		TCHAR *p = szBuff;
		::memset(szBuff, 0, sizeof(szBuff));

		{
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("00 01 02 03 04 05 06\r\n")
							_T("  \t  \tAA BB CC DD EE FF 99>")
			);
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_B);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseBinaryFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 14 );
			CPPUNIT_ASSERT( data.getValueBinary(0)  == 0x00 );
			CPPUNIT_ASSERT( data.getValueBinary(1)  == 0x01 );
			CPPUNIT_ASSERT( data.getValueBinary(2)  == 0x02 );
			CPPUNIT_ASSERT( data.getValueBinary(3)  == 0x03 );
			CPPUNIT_ASSERT( data.getValueBinary(4)  == 0x04 );
			CPPUNIT_ASSERT( data.getValueBinary(5)  == 0x05 );
			CPPUNIT_ASSERT( data.getValueBinary(6)  == 0x06 );
			CPPUNIT_ASSERT( data.getValueBinary(7)  == 0xAA );
			CPPUNIT_ASSERT( data.getValueBinary(8)  == 0xBB );
			CPPUNIT_ASSERT( data.getValueBinary(9)  == 0xCC );
			CPPUNIT_ASSERT( data.getValueBinary(10) == 0xDD );
			CPPUNIT_ASSERT( data.getValueBinary(11) == 0xEE );
			CPPUNIT_ASSERT( data.getValueBinary(12) == 0xFF );
			CPPUNIT_ASSERT( data.getValueBinary(13) == 0x99 );
		}


		{
			// １６進表記以外の文字を指定
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("00 01 02 03 04 05 06\r\n")
							_T("AA BB CC DD EE FF GG>")
							);
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_B);
			p = szBuff;
			try {
				parser.parseBinaryFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"));	
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}



		{
			// １６進表記以外の文字を指定
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("00010203040506\r\n")
							_T("AABBCCDD EEFF99>")
							);
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_B);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseBinaryFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 14 );
			CPPUNIT_ASSERT( data.getValueBinary(0)  == 0x00 );
			CPPUNIT_ASSERT( data.getValueBinary(1)  == 0x01 );
			CPPUNIT_ASSERT( data.getValueBinary(2)  == 0x02 );
			CPPUNIT_ASSERT( data.getValueBinary(3)  == 0x03 );
			CPPUNIT_ASSERT( data.getValueBinary(4)  == 0x04 );
			CPPUNIT_ASSERT( data.getValueBinary(5)  == 0x05 );
			CPPUNIT_ASSERT( data.getValueBinary(6)  == 0x06 );
			CPPUNIT_ASSERT( data.getValueBinary(7)  == 0xAA );
			CPPUNIT_ASSERT( data.getValueBinary(8)  == 0xBB );
			CPPUNIT_ASSERT( data.getValueBinary(9)  == 0xCC );
			CPPUNIT_ASSERT( data.getValueBinary(10) == 0xDD );
			CPPUNIT_ASSERT( data.getValueBinary(11) == 0xEE );
			CPPUNIT_ASSERT( data.getValueBinary(12) == 0xFF );
			CPPUNIT_ASSERT( data.getValueBinary(13) == 0x99 );
		}

		{
			// 値の前に0xがある場合
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0x00 0x01 0x02 0x03 0x04 0x05 0x06\r\n")
							_T("0XAA 0XBB 0XCC 0XDD 0XEE 0XFF 0X99>")
							);
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_B);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseBinaryFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 14 );
			CPPUNIT_ASSERT( data.getValueBinary(0)  == 0x00 );
			CPPUNIT_ASSERT( data.getValueBinary(1)  == 0x01 );
			CPPUNIT_ASSERT( data.getValueBinary(2)  == 0x02 );
			CPPUNIT_ASSERT( data.getValueBinary(3)  == 0x03 );
			CPPUNIT_ASSERT( data.getValueBinary(4)  == 0x04 );
			CPPUNIT_ASSERT( data.getValueBinary(5)  == 0x05 );
			CPPUNIT_ASSERT( data.getValueBinary(6)  == 0x06 );
			CPPUNIT_ASSERT( data.getValueBinary(7)  == 0xAA );
			CPPUNIT_ASSERT( data.getValueBinary(8)  == 0xBB );
			CPPUNIT_ASSERT( data.getValueBinary(9)  == 0xCC );
			CPPUNIT_ASSERT( data.getValueBinary(10) == 0xDD );
			CPPUNIT_ASSERT( data.getValueBinary(11) == 0xEE );
			CPPUNIT_ASSERT( data.getValueBinary(12) == 0xFF );
			CPPUNIT_ASSERT( data.getValueBinary(13) == 0x99 );
		}

		{
			// エンドタグがない
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("00"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_B);
			p = szBuff;
			try {
				parser.parseBinaryFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}


		{
			// １６進表記としてデータがたらない
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("00 1 02>")
				);
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_B);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseBinaryFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 3 );
			CPPUNIT_ASSERT( data.getValueBinary(0)  == 0x00 );
			CPPUNIT_ASSERT( data.getValueBinary(1)  == 0x01 );
			CPPUNIT_ASSERT( data.getValueBinary(2)  == 0x02 );


		}

	} catch(XExcept &e) {

		std::cerr << "testParseBinary ERR" << std::endl;

		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void XSecsTextParserTest::testparseU1()
{
	try {
		

		TCHAR szBuff[500];
		TCHAR *p = szBuff;
		::memset(szBuff, 0, sizeof(szBuff));


		{
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("255>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U1);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueUInt1()  == 0xFF );
		}

		{
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0 1 2 3 4 5 6\r\n")
							_T("  \t  \t255>")
							);
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U1);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueUInt1(0)  == 0 );
			CPPUNIT_ASSERT( data.getValueUInt1(1)  == 1 );
			CPPUNIT_ASSERT( data.getValueUInt1(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueUInt1(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueUInt1(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueUInt1(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueUInt1(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueUInt1(7)  == 0xFF );
		}
		{

			// 16進数表記
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U1);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueUInt1()  == 0xFF );
		}
		{
			XSecsTextParser parser;

			// 16進数表記 複数
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0X00 0X01 0X02 0X03 0X04 0X05 0X06\r\n")
							_T("  \t  \t0xFF>")
							);
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U1);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueUInt1(0)  == 0 );
			CPPUNIT_ASSERT( data.getValueUInt1(1)  == 1 );
			CPPUNIT_ASSERT( data.getValueUInt1(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueUInt1(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueUInt1(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueUInt1(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueUInt1(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueUInt1(7)  == 0xFF );
		}

		{
			// オーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("256>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			// 16進数表記　オーバーフロー
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0x100>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			//　数値以外の値を指定
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xABCDFEGHI>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// エンドタグ無し
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
	} catch(XExcept &e) {

		std::cerr << "testparseU1 ERR" << std::endl;

		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}




void XSecsTextParserTest::testparseI1()
{
	try {
		

		TCHAR szBuff[500];
		TCHAR *p = szBuff;
		::memset(szBuff, 0, sizeof(szBuff));


		{
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-128>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueInt1()  == -128 );
		}

		{
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-128 -1 2 -3 4 -5 6\r\n")
							_T("  \t  \t127>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueInt1(0)  == -128 );
			CPPUNIT_ASSERT( data.getValueInt1(1)  == -1 );
			CPPUNIT_ASSERT( data.getValueInt1(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueInt1(3)  == -3 );
			CPPUNIT_ASSERT( data.getValueInt1(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueInt1(5)  == -5 );
			CPPUNIT_ASSERT( data.getValueInt1(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueInt1(7)  == 127 );
		}
		{

			// 16進数表記
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueInt1()  == -1 );
		}
		{
			XSecsTextParser parser;

			// 16進数表記 複数
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0X7F 0X80 0X02 0X03 0X04 0X05 0X06\r\n")
							_T("  \t  \t0xFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueInt1(0)  == 127 );
			CPPUNIT_ASSERT( data.getValueInt1(1)  == -128 );
			CPPUNIT_ASSERT( data.getValueInt1(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueInt1(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueInt1(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueInt1(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueInt1(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueInt1(7)  == -1 );
		}

		{
			// オーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("128>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// アンダーオーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-129>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			// 16進数表記　オーバーフロー
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0x100>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			//　数値以外の値を指定
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xABCDFEGHI>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// エンドタグ無し
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I1);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
	} catch(XExcept &e) {


		std::cerr << "testparseI1 ERR" << std::endl;


		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}



void XSecsTextParserTest::testparseU2()
{
	try {
		

		TCHAR szBuff[500];
		TCHAR *p = szBuff;
		::memset(szBuff, 0, sizeof(szBuff));


		{
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("65535>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U2);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueUInt2()  == 0xffff );
		}

		{
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0 1 2 3 4 5 6\r\n")
							_T("  \t  \t65535>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U2);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueUInt2(0)  == 0 );
			CPPUNIT_ASSERT( data.getValueUInt2(1)  == 1 );
			CPPUNIT_ASSERT( data.getValueUInt2(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueUInt2(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueUInt2(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueUInt2(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueUInt2(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueUInt2(7)  == 65535 );
		}
		{

			// 16進数表記
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xFFFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U2);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueUInt2()  == 65535 );
		}
		{
			XSecsTextParser parser;

			// 16進数表記 複数
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0X00 0X01 0X02 0X03 0X04 0X05 0X06\r\n")
							_T("  \t  \t0xFFFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U2);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueUInt2(0)  == 0 );
			CPPUNIT_ASSERT( data.getValueUInt2(1)  == 1 );
			CPPUNIT_ASSERT( data.getValueUInt2(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueUInt2(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueUInt2(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueUInt2(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueUInt2(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueUInt2(7)  == 65535 );
		}

		{
			// オーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("65536>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			// 16進数表記　オーバーフロー
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0x10000>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			//　数値以外の値を指定
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xABCDFEGHI>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// エンドタグ無し
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}





void XSecsTextParserTest::testparseI2()
{
	try {
		

		TCHAR szBuff[500];
		TCHAR *p = szBuff;
		::memset(szBuff, 0, sizeof(szBuff));


		{
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-32768>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueInt2()  == -32768 );
		}

		{
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-32768 -1 2 -3 4 -5 6\r\n")
							_T("  \t  \t32767>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueInt2(0)  == -32768 );
			CPPUNIT_ASSERT( data.getValueInt2(1)  == -1 );
			CPPUNIT_ASSERT( data.getValueInt2(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueInt2(3)  == -3 );
			CPPUNIT_ASSERT( data.getValueInt2(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueInt2(5)  == -5 );
			CPPUNIT_ASSERT( data.getValueInt2(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueInt2(7)  == 32767 );
		}
		{

			// 16進数表記
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xFFFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueInt2()  == -1 );
		}
		{
			XSecsTextParser parser;

			// 16進数表記 複数
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0X7FFF 0X8000 0X02 0X03 0X04 0X05 0X06\r\n")
							_T("  \t  \t0xFFFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueInt2(0)  == 32767 );
			CPPUNIT_ASSERT( data.getValueInt2(1)  == -32768 );
			CPPUNIT_ASSERT( data.getValueInt2(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueInt2(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueInt2(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueInt2(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueInt2(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueInt2(7)  == -1 );
		}

		{
			// オーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("32768>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// アンダーオーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-32769>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			// 16進数表記　オーバーフロー
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0x10000>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			//　数値以外の値を指定
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xABCDFEGHI>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// エンドタグ無し
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}



void XSecsTextParserTest::testparseU4()
{
	try {
		

		TCHAR szBuff[500];
		TCHAR *p = szBuff;
		::memset(szBuff, 0, sizeof(szBuff));

		{
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("4294967295>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U4);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueUInt4()  == 4294967295 );
		}

		{
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0 1 2 3 4 5 6\r\n")
							_T("  \t  \t4294967295>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U4);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueUInt4(0)  == 0 );
			CPPUNIT_ASSERT( data.getValueUInt4(1)  == 1 );
			CPPUNIT_ASSERT( data.getValueUInt4(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueUInt4(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueUInt4(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueUInt4(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueUInt4(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueUInt4(7)  == 4294967295 );
		}
		{

			// 16進数表記
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xFFFFFFFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U4);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueUInt4()  == 4294967295 );
		}
		{
			XSecsTextParser parser;

			// 16進数表記 複数
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0X00 0X01 0X02 0X03 0X04 0X05 0X06\r\n")
							_T("  \t  \t0xFFFFFFFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U4);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueUInt4(0)  == 0 );
			CPPUNIT_ASSERT( data.getValueUInt4(1)  == 1 );
			CPPUNIT_ASSERT( data.getValueUInt4(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueUInt4(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueUInt4(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueUInt4(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueUInt4(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueUInt4(7)  == 4294967295 );
		}

		{
			// オーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("4294967296>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U4);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			// 16進数表記　オーバーフロー
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0x100000000>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U4);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			//　数値以外の値を指定
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xABCDFEGHI>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U4);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// エンドタグ無し
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_U4);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}





void XSecsTextParserTest::testparseI4()
{
	try {
		

		TCHAR szBuff[500];
		TCHAR *p = szBuff;
		::memset(szBuff, 0, sizeof(szBuff));


		{
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-2147483647>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I4);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueInt4()  == -2147483647 );
		}

		{
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-2147483647 -1 2 -3 4 -5 6\r\n")
							_T("  \t  \t2147483647>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I4);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueInt4(0)  == -2147483647 );
			CPPUNIT_ASSERT( data.getValueInt4(1)  == -1 );
			CPPUNIT_ASSERT( data.getValueInt4(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueInt4(3)  == -3 );
			CPPUNIT_ASSERT( data.getValueInt4(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueInt4(5)  == -5 );
			CPPUNIT_ASSERT( data.getValueInt4(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueInt4(7)  == 2147483647 );
		}
		{

			// 16進数表記
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xFFFFFFFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I4);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 1 );
			CPPUNIT_ASSERT( data.getValueInt4()  == -1 );
		}
		{
			XSecsTextParser parser;

			// 16進数表記 複数
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0X7FFFFFFF 0X80000000 0X02 0X03 0X04 0X05 0X06\r\n")
							_T("  \t  \t0xFFFFFFFF>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I4);
			p = szBuff;
			CPPUNIT_ASSERT( parser.parseDigitFromString(&p, len, &data) == true );
			CPPUNIT_ASSERT( data.getDataCount() == 8 );
			CPPUNIT_ASSERT( data.getValueInt4(0)  == 2147483647 );
			CPPUNIT_ASSERT( data.getValueInt4(1)  == -2147483648 );
			CPPUNIT_ASSERT( data.getValueInt4(2)  == 2 );
			CPPUNIT_ASSERT( data.getValueInt4(3)  == 3 );
			CPPUNIT_ASSERT( data.getValueInt4(4)  == 4 );
			CPPUNIT_ASSERT( data.getValueInt4(5)  == 5 );
			CPPUNIT_ASSERT( data.getValueInt4(6)  == 6 );
			CPPUNIT_ASSERT( data.getValueInt4(7)  == -1 );
		}

		{
			// オーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("2147483648>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I2);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// アンダーオーバーフロー
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("-2147483649>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I4);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			// 16進数表記　オーバーフロー
			XSecsTextParser parser;
			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0x100000000>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I4);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}

		{
			//　数値以外の値を指定
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0xABCDFEGHI>"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I4);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
		{
			// エンドタグ無し
			XSecsTextParser parser;

			XStrBuff::StrCpy(szBuff, XCOUNTOF(szBuff),	_T("0"));
			size_t len = XStrBuff::StrLen(szBuff);
			XRichData data(XDATA_TYPE_I4);
			p = szBuff;
			try {
				parser.parseDigitFromString(&p, len, &data);
				CPPUNIT_ASSERT( 0 );
			} catch(XExcept &e) {
				XTRACE(_T("%s\n"), (const TCHAR*)e.what());	
			}
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void XSecsTextParserTest::testS2F3_Bin()
{


	try {
		XSecsTextParser parser;
		XSecsData data;
		bool b;

//		CPPUNIT_ASSERT( parser.parseFromFile("S7F3.SML.txt", &data) );
//		data.print( std::cout );
		b = parser.parseFromFile("S16F11_PJCREATE.sml", &data);
		CPPUNIT_ASSERT( b );
#ifdef _UNICODE
		data.print( std::wcout );
#else
		data.print( std::cout );
#endif

//		std::strstream          stream;
		XInflateStreamBuff	buff(256);
		std::iostream		stream(&buff);
		data.buildBinaryData(stream, XSecsData::SIZE_HEAD_AND_BODY);
		XBIN* pMsg = buff.getBuff();
		unsigned long ulSize = buff.size();

		pMsg += 14;
		ulSize-=14;

		XDataReader writer(pMsg, ulSize);
#ifdef _UNICODE
		writer.print(std::wcout);
#else
		writer.print(std::cout);
#endif


	} catch(XExcept &e) {
		std::cerr << e.what() << std::endl;
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}








