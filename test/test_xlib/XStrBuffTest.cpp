#include <iostream>
#include <fstream>
#include "XStrBuffTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xeception.h"
#include "xlib/xstrbuff.h"
#include "xlib/xmap.h"

#include <windows.h>
#ifdef _UNICODE
	#define XLIB_TEST_STD_ERR	std::wcerr
#else
	#define XLIB_TEST_STD_ERR	std::cerr
#endif
/**
コンストラクタ
@param name [in] テスト名
*/
XStrBuffTest::XStrBuffTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XStrBuffTest::setUp(void)
{
}


/** テスト後処理 */
void XStrBuffTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XStrBuffTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XStrBuffTest");
	
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testXStrBuffSplit) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testConstructor) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testSet) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorEq) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorCast) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorComp) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorGetIndexAcc) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorSetIndexAcc) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorAdd) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorIns) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testCount) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testFormat) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testUtilFunctions) );



	return suite;
}


void XStrBuffTest::testConstructor(void)
{
	try {
		// 初期、余分バッファの増加サイズ
		XStrBuff d(120);
		CPPUNIT_ASSERT_EQUAL((size_t)120, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByteで初期化。その他はデフォルト
		XStrBuff d("0123456789");
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByteで初期化。サイズを指定。その他はデフォルト
		XStrBuff d("0123456789", 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "012") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByteで初期化。サイズを指定。初期、余分バッファの増加サイズも指定。
		XStrBuff d("0123456789", 4, 10);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123") );
		CPPUNIT_ASSERT_EQUAL((size_t)10, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByteで初期化。初期、余分バッファの増加サイズも指定。サイズはデフォルト
		XStrBuff d("0123456789", -1, 10);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)10, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByteで初期化。実際のサイズ以上のサイズを指定。
		XStrBuff d("0123456789", 500);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	
	try {
		// Unicodeで初期化。その他はデフォルト
		XStrBuff d(L"0123456789");

		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicodeで初期化。サイズを指定。その他はデフォルト
		XStrBuff d(L"0123456789", 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"012") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicodeで初期化。サイズを指定。初期、余分バッファの増加サイズも指定。
		XStrBuff d(L"0123456789", 4, 10);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123") );
		CPPUNIT_ASSERT_EQUAL((size_t)10, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicodeで初期化。初期、余分バッファの増加サイズも指定。サイズはデフォルト
		XStrBuff d(L"0123456789", -1, 10);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)10, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicodeで初期化。実際のサイズ以上のサイズを指定。
		XStrBuff d(L"0123456789", 500);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	
	try {
		// XStrBuffで初期化。その他はデフォルト
		XStrBuff src(_T("0123456789"));
		XStrBuff d(src);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// XStrBuffで初期化。余分バッファの増加サイズも指定
		XStrBuff src(_T("0123456789"));
		XStrBuff d(src, 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)3, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testSet(void)
{
	try {
		// MuitiByteでセット。その他はデフォルト
		XStrBuff d;
		d.set("0123456789");
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByteでセット。サイズを指定
		XStrBuff d;
		d.set("0123456789", 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "012") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByteでセット。実際のサイズ以上のサイズを指定。
		XStrBuff d;
		d.set("0123456789", 500);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	
	try {
		// Unicodeでセット。その他はデフォルト
		XStrBuff d;
		d.set(L"0123456789");
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicodeでセット。サイズを指定。
		XStrBuff d;
		d.set(L"0123456789", 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"012") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	try {
		// Unicodeでセット。実際のサイズ以上のサイズを指定。
		XStrBuff d;
		d.set(L"0123456789", 500);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorEq()
{

	try {
		// MultiByteで代入。
		XStrBuff d;
		d = "あいうえお";
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::StrCmp(d, _T("あいうえお")) );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicodeで代入。
		XStrBuff d;
		d = L"あいうえお";
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::StrCmp(d, _T("あいうえお")) );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// XStrBuffで代入。
		XStrBuff src(_T("あいうえお"));
		XStrBuff d;
		d = src;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::StrCmp(d, _T("あいうえお")) );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

}
void XStrBuffTest::testOperatorCast(void)
{
	try {
		// char*に変換
		XStrBuff d(_T("あいうえお"));
		char* p_addr = d;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(p_addr, "あいうえお") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// const char*に変換
		XStrBuff d(_T("あいうえお"));
		const char* p_addr = d;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(p_addr, "あいうえお") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// char*に変換
		XStrBuff d(_T("あいうえお"));
		wchar_t* p_addr = d;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(p_addr, L"あいうえお") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// const char*に変換
		XStrBuff d(_T("あいうえお"));
		const wchar_t* p_addr = d;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(p_addr, L"あいうえお") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorComp(void)
{
	try {
		// const char*に変換
		XStrBuff d(_T("あいうえお"));
		const wchar_t* p_addr = d;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("あいうえお") );
		CPPUNIT_ASSERT_EQUAL(false, d == _T("あいうえか") );
		CPPUNIT_ASSERT_EQUAL(false, d != _T("あいうえお") );
		CPPUNIT_ASSERT_EQUAL(true,  d != _T("あいうえか") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorGetIndexAcc(void)
{
	try {
		XStrBuff d(_T("0123456789"));
		TCHAR c = 0;

		
		c = d[0];
		CPPUNIT_ASSERT_EQUAL((int)_T('0'),  (int)c );
		c = d.GetAt(0);
		CPPUNIT_ASSERT_EQUAL((int)_T('0'),  (int)c );

		c = d[2];
		CPPUNIT_ASSERT_EQUAL((int)_T('2'),  (int)c );
		c = d.GetAt(2);
		CPPUNIT_ASSERT_EQUAL((int)_T('2'),  (int)c );

		
		c = d[9];
		CPPUNIT_ASSERT_EQUAL((int)_T('9'),  (int)c );
		c = d.GetAt(9);
		CPPUNIT_ASSERT_EQUAL((int)_T('9'),  (int)c );
		
		// 指定インデックスオーバー。例外が発生する。
		try {
			c = d[10];
			CPPUNIT_ASSERT( false );
		} catch(XExcept &ex) {
			CPPUNIT_ASSERT_EQUAL(ex.getCategory(), XExcept_OVER_INDEX);
		}
		try {
			c = d.GetAt(10);
			CPPUNIT_ASSERT( false );
		} catch(XExcept &ex) {
			CPPUNIT_ASSERT_EQUAL(ex.getCategory(), XExcept_OVER_INDEX);
		}
		


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorSetIndexAcc(void)
{
	try {
		XStrBuff d(_T("0123456789"));
		
		d[0] = _T('A');
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A123456789") );
		d.SetAt(1, _T('B'));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("AB23456789") );

		d[2] = _T('C');
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("ABC3456789") );
		d.SetAt(3, _T('D'));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("ABCD456789") );

		
		d[8] = _T('X');
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("ABCD4567X9") );
		d.SetAt(9, _T('Y'));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("ABCD4567XY") );
		
		// 指定インデックスオーバー。例外が発生する。
		try {
			d[10] = _T('Z');
			CPPUNIT_ASSERT( false );
		} catch(XExcept &ex) {
			CPPUNIT_ASSERT_EQUAL(ex.getCategory(), XExcept_OVER_INDEX);
		}
		try {
			d.SetAt(10, _T('Z'));
			CPPUNIT_ASSERT( false );
		} catch(XExcept &ex) {
			CPPUNIT_ASSERT_EQUAL(ex.getCategory(), XExcept_OVER_INDEX);
		}
		


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorAdd(void)
{
	try {
		XStrBuff d(_T("0123456789"), 10);

		// MultiByteで足す
		d += "ABC";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("0123456789ABC") );
		// UNICODEで足す
		d += L"EFG";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("0123456789ABCEFG") );
		
		// MultiByteで足す(ワイド文字)
		d += "あいう";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("0123456789ABCEFGあいう") );
		// UNICODEで足す(ワイド文字)
		d += L"えおか";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("0123456789ABCEFGあいうえおか") );

		


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XStrBuffTest::testOperatorIns(void)
{
	try {
		XStrBuff d(_T("0123456789"), 10);

		// short(プラス値)を代入
		d << (short)30000;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000") );

		// short(プラス値)を代入
		d << (short)-30001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-30001") );
		
		
		// unsigned shortを代入
		d << (unsigned short)60006;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-3000160006") );

		
		// int(プラス値)を代入
		d << (int)40000;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-300016000640000") );

		// short(プラス値)を代入
		d << (int)-40001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-300016000640000-40001") );

		// unsigned intを代入
		d << (unsigned int)100001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-300016000640000-40001100001") );


		d = _T("A");
		
		
		// long(プラス値)を代入
		d << (long)40000;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000") );

		// long(プラス値)を代入
		d << (long)-40001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000-40001") );

		// unsigned longを代入
		d << (unsigned long)100001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000-40001100001") );
		
		d = _T("A");
		
		// XD_INT8(プラス値)を代入
		d << (XD_INT8)40000;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000") );

		// XD_INT8(プラス値)を代入
		d << (XD_INT8)-40001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000-40001") );

		// XD_UINT8を代入
		d << (XD_UINT8)100001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000-40001100001") );
		
		d = _T("A");
		// floatを代入
		d << (float)0.1;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-001") );
		d = _T("A");
		// doubleを代入
		d << (double)0.01;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-002") );

		// MultiByte文字を代入
		d << "あいうえお";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-002あいうえお") );
		// Unicode文字を代入
		d << "かきくけこ";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-002あいうえおかきくけこ") );
		// XStrBuffを代入
		XStrBuff src(_T("さしすせそ"));
		d << src;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-002あいうえおかきくけこさしすせそ") );

	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XStrBuffTest::testCount(void)
{
	try {
		static int INFLATE_SIZE = 10;
		XStrBuff d(INFLATE_SIZE);
		// 15文字のバッファを確保
		CPPUNIT_ASSERT( d.Alloc(15 * sizeof(TCHAR)) );
		// 10文字代入
		d = _T("0123456789");
		CPPUNIT_ASSERT_EQUAL((size_t)10,  d.getLength() );
		CPPUNIT_ASSERT_EQUAL((size_t)15,  d.getCountOfBuff() );
		CPPUNIT_ASSERT_EQUAL((size_t)(15 * sizeof(TCHAR)),  d.GetBuffSize() );
		CPPUNIT_ASSERT_EQUAL((size_t)5,  d.getCountOfRestBuff() );
		
		// 更に 2文字追加代入 合計12文字　初期確保の14文字(NULL含まず）に到達していない。メモリーはRealoocしてない。
		d << _T("01");
		CPPUNIT_ASSERT_EQUAL((size_t)12,  d.getLength() );
		// 更に 2文字追加代入 合計14文字　初期確保の14文字(NULL含まず）に一致。メモリーはRealoocしてない。
		d << _T("23");
		CPPUNIT_ASSERT_EQUAL((size_t)14,  d.getLength() );
		// 更に 2文字追加代入 合計16文字　初期確保の14文字(NULL含まず）をオーバー。メモリーはRealoocする。
		d << _T("45");
		CPPUNIT_ASSERT_EQUAL((size_t)16,  d.getLength() );


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testFormat(void)
{
	try {
		XStrBuff d;

		d.Format(_T("##%u,%s"), 10, _T("あいうえお"));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("##10,あいうえお") );

		d = XStrBuff::FormatS(_T("##%u,%s"), 10, _T("あいうえお"));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("##10,あいうえお") );

	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testUtilFunctions(void)
{
	try {
		XStrBuff d(_T(" abcdefgあいうえお "));
		int iFind = 0;

		d.MakeUpper();
		CPPUNIT_ASSERT_EQUAL(true,  d == _T(" ABCDEFGあいうえお ") );
		
		d.MakeLower();
		CPPUNIT_ASSERT_EQUAL(true,  d == _T(" abcdefgあいうえお ") );

		d.Trim(XStrBuff::TRIM_RIGHTONLY);
		CPPUNIT_ASSERT_EQUAL(true,  d == _T(" abcdefgあいうえお") );
		
		d.Trim(XStrBuff::TRIM_LEFTONLY);
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("abcdefgあいうえお") );

		d = _T(" abcdefgあいうえおabc ");
		d.Trim(XStrBuff::TRIM_BOTH);
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("abcdefgあいうえおabc") );

		
		d.Remove(_T('a'));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("bcdefgあいうえおbc") );

		d.Replace(_T("bc"), _T("かきくけこ"));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("かきくけこdefgあいうえおかきくけこ") );

		iFind = d.Find(_T("こ"));
		CPPUNIT_ASSERT_EQUAL(true,  iFind != -1);


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XStrBuffTest::testXStrBuffSplit(void)
{
	try {
		XStrBuffSplit split;

		split = _T("a01,b02,c03");
		CPPUNIT_ASSERT_EQUAL(true,  split.GetArgCouunt() == 3);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("a01"), split.GetArgv()[0]) == 0);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("b02"), split.GetArgv()[1]) == 0);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("c03"), split.GetArgv()[2]) == 0);

		split = _T("a01,\"b0,2\",c03");
		CPPUNIT_ASSERT_EQUAL(true,  split.GetArgCouunt() == 3);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("a01"), split.GetArgv()[0]) == 0);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("b0,2"), split.GetArgv()[1]) == 0);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("c03"), split.GetArgv()[2]) == 0);

	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

