#include <iostream>
#include <fstream>
#include "XJSonTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xeception.h"
#include "xlib/json/xjson_array.h"
#include "xlib/json/xjson_obj.h"
#include "xlib/json/xjson_val.h"


#include <windows.h>
#ifdef _UNICODE
	#define XLIB_TEST_STD_ERR	std::wcerr
#else
	#define XLIB_TEST_STD_ERR	std::cerr
#endif



class MyXJsonValErrorReport : public XJsonValErrorReport
{
public:
	MyXJsonValErrorReport(){}
	virtual void error(const TCHAR* p_msg, size_t nCharPos, size_t nLineNumber, size_t nLinePos);
};

void MyXJsonValErrorReport::error(const TCHAR* p_msg, size_t nCharPos, size_t nLineNumber, size_t nLinePos)
{
	XLIB_TEST_STD_ERR << p_msg << std::endl;
	XLIB_TEST_STD_ERR << _T("  Char Pos      : ") << nCharPos << std::endl;
	XLIB_TEST_STD_ERR << _T("  Line Number   : ") << nLineNumber << std::endl;
	XLIB_TEST_STD_ERR << _T("  Line Char Pos : ") << nLinePos << std::endl;
}




/**
コンストラクタ
@param name [in] テスト名
*/
XJSonTest::XJSonTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XJSonTest::setUp(void)
{
}


/** テスト後処理 */
void XJSonTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XJSonTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XJSonTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonString) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonObject) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonArray) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonNumetric) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonPrimitive) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testXBuff) );
	
	return suite;
}


void XJSonTest::testParseJsonString(void)
{
	// 全てASCIIな文字を取得
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("\"abcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("abcd") );
	}
	// クォーテーションのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		p_ret = v.loadFromString(_T("\"ab\\\"cd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\"cd") );
	}
	// バックスラッシュのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		p_ret = v.loadFromString(_T("\"ab\\\\cd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\\cd") );
	}
	// スラッシュのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("\"ab\\/cd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab/cd") );
	}
	// バックスペースのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		XLIB_TEST_STD_ERR << _T("\"ab\\bcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\bcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\bcd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// フロムフィードのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		XLIB_TEST_STD_ERR << _T("\"ab\\fcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\fcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\fcd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// ラインフィードのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		XLIB_TEST_STD_ERR << _T("\"ab\\ncd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\ncd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\ncd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// キャリッジリターンのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		XLIB_TEST_STD_ERR << _T("\"ab\\rcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\rcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\rcd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// TABのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"ab\\tcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\tcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\tcd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// UNICODEのエスケープ文字が含まれる文字の解析
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"\\u5024\\u4F0D\"") << std::endl;
		p_ret = v.loadFromString(_T("\"\\u5024\\u4F0D\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("値伍") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}



	// 文字列の解析で、最後の"が無い。
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("\"abcd"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}

	// UNICODEのエスケープ文字で、データが足らない
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"\\u502") << std::endl;
		p_ret = v.loadFromString(_T("\"\\u502"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}

	// UNICODEのエスケープ文字で、16進数以外の文字がしようされている。
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"\\u5G24\\u4F0D\"") << std::endl;
		p_ret = v.loadFromString(_T("\"\\u5G24\\u4F0D\""), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}

	//エスケープ対象が不明
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"ab\\Gcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\Gcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
}

void XJSonTest::testParseJsonNumetric(void)
{
	//プラスの整数値
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("123456789"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isInteger() );
		CPPUNIT_ASSERT_EQUAL(true, v.getInteger() == 123456789 );
	}
	//マイナスの整数値
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("-123456789"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isInteger() );
		CPPUNIT_ASSERT_EQUAL(true, v.getInteger() == -123456789 );
	}
	//プラスの整数値
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("12345678.9"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isDouble() );
		CPPUNIT_ASSERT_EQUAL(true, v.getDouble() == 12345678.9 );
	}
	//マイナスの整数値
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("-12345678.9"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isDouble() );
		CPPUNIT_ASSERT_EQUAL(true, v.getDouble() == -12345678.9 );
	}
	//実数１
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.23e-003"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isDouble() );		
		CPPUNIT_ASSERT_EQUAL(true, v.getDouble() == 0.00123 );
	}
	//実数2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.23e+003"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isDouble() );		
		CPPUNIT_ASSERT_EQUAL(true, v.getDouble() == 1230.0 );
	}


	
	//先頭から0が続けて2つ出現
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("00"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//実数2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.23e003"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//実数2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.23e+003-003"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//実数2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.2300.3"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//実数2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("10-003"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//実数2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("ABC"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//実数2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("10ABC"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}


	

}
void XJSonTest::testParseJsonPrimitive(void)
{
	//true
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("true"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == true );
	}
	//TRUE
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("TRUE"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == true );
	}
	//TrUe
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("TrUe"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == true );
	}
	
	//false
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("false"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == false );
	}
	//FALSE
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("FALSE"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == false );
	}
	//FaLsE
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("FaLsE"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == false );
	}


	//null
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("null"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isNull() );
	}
	//NULL
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("NULL"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isNull() );
	}
	//NuLl
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("NuLl"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isNull() );
	}
}

void XJSonTest::testParseJsonObject(void)
{
	// プロパティの無いオブジェクト
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  { \t\r\n   }  \t\t\t\t\t\t\t\r\n\r\n "), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isObject() );
		CPPUNIT_ASSERT_EQUAL(true, v.getObject().size() == 0 );
	}
	// プロパティが1件オブジェクト
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \"aaa\"  \t : \t\r\n 123 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isObject() );
		CPPUNIT_ASSERT_EQUAL(true, v.getObject().size() == 1 );
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].isInteger() );		
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].getInteger() == 123 );
	}
	// プロパティが２件以上オブジェクト
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \"aaa\"  \t : \t\r\n 123 \t\r\n, \"bbb\"  \t : \t\r\n 456 \t\r\n, \"ccc\"  \t : \t\r\n 789 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isObject() );
		CPPUNIT_ASSERT_EQUAL(true, v.getObject().size() == 3 );
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].isInteger() );		
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].getInteger() == 123 );
		CPPUNIT_ASSERT_EQUAL(true, v[_T("bbb")].isInteger() );		
		CPPUNIT_ASSERT_EQUAL(true, v[_T("bbb")].getInteger() == 456 );
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].isInteger() );		
		CPPUNIT_ASSERT_EQUAL(true, v[_T("ccc")].getInteger() == 789 );
	}
	// プロパティー名が不正
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \t\r\n aaa  \t : \t\r\n 123 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	// プロパティの名前と値の区切り文字がコロンではない。
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \"aaa\"  \t ; \t\r\n 123 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	// 複数プロパティの区切り文字がカンマでない。
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \"aaa\"  \t : \t\r\n 123 \t\r\n @ \"bbb\"  \t : \t\r\n 456 \t\r\n, \"ccc\"  \t : \t\r\n 789 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
}
void XJSonTest::testParseJsonArray(void)
{
	// 空の配列定義
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n   ]  "), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isArray() );
		CPPUNIT_ASSERT_EQUAL(true, v.getArray().size() == 0 );
	}
	// 配列に１件のデータ
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n123   ]  "), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isArray() );
		CPPUNIT_ASSERT_EQUAL(true, v.getArray().size() == 1 );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)0].isInteger() );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)0].getInteger() == 123 );
	}
	// 配列に2件以上のデータ
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n123, \"ABC\", 1.23   ]  "), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isArray() );
		CPPUNIT_ASSERT_EQUAL(true, v.getArray().size() == 3 );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)0].isInteger() );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)0].getInteger() == 123 );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)1].isString() );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)1].getString() == _T("ABC") );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)2].isDouble() );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)2].getDouble() == 1.23 );
	}
	
	// 配列の括りの文字が無い
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n123     "), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	
	// 配列の区切り文字が不正
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n123 @ 123 ]    "), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
}
void XJSonTest::testXBuff(void)
{
	XJsonObj obj;
	XJsonArray ar;
	XJsonArray ar2;

	obj.add(_T("key1"), XJsonVal((int)10));
	obj.add(_T("キー2"), XJsonVal(_T("値１")));
	obj.add(_T("key3"), XJsonVal(10.123456));
	obj.add(_T("key4"), XJsonVal(true));
	obj.add(_T("key5"), XJsonVal(false));
	obj.add(_T("key6"), XJsonVal());

	XJsonObj obj2;
	obj2.add(_T("sub_key1"), XJsonVal((int)10));
	obj2.add(_T("サブキー2alphabet"), XJsonVal(_T("値１alphabet")));
	obj2.add(_T("sub_key3"), XJsonVal(10.123456));
	obj2.add(_T("sub_key4"), XJsonVal(true));
	obj2.add(_T("sub_key5"), XJsonVal(false));
	obj2.add(_T("sub_key6"), XJsonVal());
	ar2.push_back(XJsonVal((int)9999));
	ar2.push_back(XJsonVal(_T("値ログ")));
	ar2.push_back(XJsonVal(25.123456));
	ar2.push_back(XJsonVal(true));
	ar2.push_back(XJsonVal(false));
	ar2.push_back(XJsonVal());
	obj2.add(_T("sub_key7"), XJsonVal(ar2));





	ar.push_back(XJsonVal((int)10));
	ar.push_back(XJsonVal(_T("値伍")));
	ar.push_back(XJsonVal(20.123456));
	ar.push_back(XJsonVal(true));
	ar.push_back(XJsonVal(false));
	ar.push_back(XJsonVal());
	obj.add(_T("key7"), XJsonVal(ar));
	obj.add(_T("key8"), XJsonVal(obj2));


	
	obj[_T("key9")] = XJsonVal(456);
	obj[_T("key10")] = XJsonVal("ascii");
	obj[_T("key11")] = XJsonVal(L"unicode");

	XLIB_TEST_STD_ERR << _T("構築したデータの出力:") << std::endl;
	obj.print(XLIB_TEST_STD_ERR);


	XLIB_TEST_STD_ERR << std::endl;
	XLIB_TEST_STD_ERR << std::endl;

	XJsonVal v;
//	const TCHAR* p = v.loadFromString(_T("{\"city\":\"ota\",\"cityLatLong\":{\"lat\":35.569999,\"long\":139.799999},\"country\":\"JP\",\"region\":\"13\"}"));
	const TCHAR* p = v.loadFromString(_T("{\"\\u5024\\u4F0D\":10,\"key10\":\"ascii\",\"key11\":\"unicode\",\"key3\":10.1235,\"key4\":true,\"key5\":false,\"key6\":null,\"key7\":[10,\"\u5024\u4F0D\",20.1235,true,false,null],\"key8\":{\"sub_key1\":10,\"sub_key3\":10.1235,\"sub_key4\":true,\"sub_key5\":false,\"sub_key6\":null,\"sub_key7\":[9999,\"\u5024\u30ED\u30B0\",25.1235,true,false,null],\"\u30B5\u30D6\u30AD\u30FC2alphabet\":\"\u5024\uFF11alphabet\"},\"key9\":456,\"\u30AD\u30FC2\":\"\u5024\uFF11\"}"));
	XLIB_TEST_STD_ERR << _T("JSonStringを解析した内容の出力:") << std::endl;
	v.print(XLIB_TEST_STD_ERR, 0);
	XLIB_TEST_STD_ERR << std::endl;
	XLIB_TEST_STD_ERR << std::endl;

	if( v.isObject() ) {
		const XJsonObj& wkObj =  v.getObject();
		XJsonObj::Iterator ite = wkObj.getIterator();
		while( !ite.isEnd() ) {
			XLIB_TEST_STD_ERR << ite.getKey().c_str() << _T(" => ");
			ite.getValue().print(XLIB_TEST_STD_ERR, ~0, true);
			XLIB_TEST_STD_ERR << std::endl;
			ite++;
		}
	}
}
