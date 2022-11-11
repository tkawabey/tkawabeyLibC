#include <iostream>
#include <fstream>
#include "XBuffTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xeception.h"


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
XBuffTest::XBuffTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XBuffTest::setUp(void)
{
}


/** テスト後処理 */
void XBuffTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XBuffTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XBuffTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXBuff", &testXBuff) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXInflateStreamBuff", &testXInflateStreamBuff) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXInflateStreamBuff2", &testXInflateStreamBuff2) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXRingBuffer", &testXRingBuffer) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXRingBufferBL", &testXRingBufferBL) );
	
	return suite;
}


void XBuffTest::testXBuff(void)
{
	try {
		int i;
		unsigned char *p;
		unsigned char *p_end;
		XBuff buff;
		XBuff buff2;

		p = (unsigned char*)buff.Alloc(10);
		CPPUNIT_ASSERT( p );
		CPPUNIT_ASSERT( buff.GetBuffSize() == 10  );
		p_end = buff;
		CPPUNIT_ASSERT( p == p_end );
		p_end = (unsigned char*)buff.GetMaxBuffAddress();
		CPPUNIT_ASSERT( (p_end - p) == 10 );
		
		buff.MemSet( 0 );
		for( i = 0; i < 10; i++ ) {
			CPPUNIT_ASSERT( p[0] == 0);
		}

		p = (unsigned char*)buff.ReAlloc(20);
		CPPUNIT_ASSERT( buff.GetBuffSize() == 20  );
		p_end = buff;
		CPPUNIT_ASSERT( p == p_end );
		p_end = (unsigned char*)buff.GetMaxBuffAddress();
		CPPUNIT_ASSERT( (p_end - p) == 20 );
		
		buff.MemSet( 0xFA );
		for( i = 0; i < 20; i++ ) {
			CPPUNIT_ASSERT( p[0] == 0xFA);
		}

		// コピー
		buff2.DoCopy( &buff );

		// 元データを解放
		buff.Free();
		CPPUNIT_ASSERT( buff.GetBuffSize() == 0  );

		p = (unsigned char*)buff2;
		CPPUNIT_ASSERT( buff2.GetBuffSize() == 20  );
		p_end = buff2;
		CPPUNIT_ASSERT( p == p_end );
		p_end = (unsigned char*)buff2.GetMaxBuffAddress();
		CPPUNIT_ASSERT( (p_end - p) == 20 );
		// 同じデータであること
		for( i = 0; i < 20; i++ ) {
			CPPUNIT_ASSERT( p[0] == 0xFA);
		}
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XBuffTest::testXInflateStreamBuff(void)
{
	try {
		int i;
		XInflateStreamBuff buff(10, 6);

		
		CPPUNIT_ASSERT( buff.getBuffSize() == 10 );

		CPPUNIT_ASSERT( buff.sputn("0123456789", 10) == 10 );
		CPPUNIT_ASSERT( strcmp(buff.toString(), "0123456789") == 0 );

		CPPUNIT_ASSERT( buff.sputn("01234", 5) == 5 );

		// バッファサイズが拡張されているはず
		//	10+6
		CPPUNIT_ASSERT( buff.getBuffSize() == 16 );
		CPPUNIT_ASSERT( buff.size() == 15 );

		{
			const char *p = buff.getCurrentAddress();
			CPPUNIT_ASSERT( p );

			CPPUNIT_ASSERT( buff.sputn("5", 1) == 1 );
			CPPUNIT_ASSERT( *p == '5' );
		}
		{
			unsigned char* p = buff.getBuff();
			CPPUNIT_ASSERT( memcmp(p, "012345678901234", 15) == 0 );
		}

		// リセット
		buff.resetp();
		CPPUNIT_ASSERT( buff.sputn("ABCDEFGHIJ", 10) == 10 );
		CPPUNIT_ASSERT( strcmp(buff.toString(), "ABCDEFGHIJ") == 0 );
		CPPUNIT_ASSERT( buff.size() == 10 );

		// 大きなデータ書き込み
		int loop = 10000;
		buff.resetp();
		for( i = 0; i < loop; i++) {
			CPPUNIT_ASSERT( buff.sputn("1", 1) == 1 );
		}
		CPPUNIT_ASSERT( buff.size() == loop );
		{
			unsigned char* p = buff.getBuff();
			for( i = 0; i < loop; i++) {
				CPPUNIT_ASSERT( *p == '1' );
			}
		}
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		
		XInflateStreamBuff buff(500, 500);
		const TCHAR* pData = _T(" 0123456789/*-+!\"#$%&'()=\r\n\t~|\\^[]{}@`:*;+\\/?.>,<_");
		for(int i = 0; i < 100; i++ ) {
			std::ostream tmpStream(&buff);
			for(int j = 0; j < 100; j++ ) {
				tmpStream << pData;
			}
			buff.toString();
			buff.resetp();
		}
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

}


void XBuffTest::testXInflateStreamBuff2(void)
{
	try {
		// インフレートしない（6文字）
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << _T("012345");

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("012345")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	
	try {
		// インフレートしない（９文字）。NULLを含めてメモリー境界ギリギリ
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << _T("012345678");

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("012345678")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	
	
	try {
		// インフレートする（１４文字）。
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << _T("012345678");
		stream << _T("90125");

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("01234567890125")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	
	try {
		// マルチバイト文字を指定
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << _T("あいうえお");
		stream << _T("かきくけこ");

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("あいうえおかきくけこ")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	
	try {
		// char値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (char)'A';
		stream << (char)'B';

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("AB")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// unsigned char値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (unsigned char)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// short値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (short)123;
		stream << (short)-456;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123-456")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// unsigned short値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (unsigned short)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// int値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (int)123;
		stream << (int)-456;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123-456")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// unsigned int値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (unsigned int)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// long値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (long)123;
		stream << (long)-456;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123-456")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// unsigned long値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (unsigned long)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// XD_INT8値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (XD_INT8)123;
		stream << (XD_INT8)-456;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123-456")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// XD_UINT8値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (XD_UINT8)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// float値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (float)0.123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("0.123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// double値
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (double)0.123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("0.123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XBuffTest::testXRingBuffer(void)
{
	try {
		XRingBuffer buff;
		unsigned char sz[255];
		unsigned char sz2[255];
		unsigned char _attach_buff[500];

		for(int i = 0; i < 2; i++) {
			// データを順次書き込む
			memset(sz, 'a', sizeof(sz));
			if( i == 0 ) {
				CPPUNIT_ASSERT( buff.alloc(255) == true );
			} else {
				CPPUNIT_ASSERT( buff.attach( &_attach_buff[10], 255) == true );
			}

			CPPUNIT_ASSERT( buff.size() == 0 );	
			CPPUNIT_ASSERT( buff.free_size() == 255 );	
			
			CPPUNIT_ASSERT( buff.put(sz, 100) == true );
			CPPUNIT_ASSERT( buff.size() == 100 );	
			CPPUNIT_ASSERT( buff.free_size() == 155 );	

			memset(sz, 'b', sizeof(sz));
			CPPUNIT_ASSERT( buff.put(sz, 155) == true );
			CPPUNIT_ASSERT( buff.size() == 255 );	
			CPPUNIT_ASSERT( buff.free_size() == 0 );

			// もうこれ以上書き込めないはず
			CPPUNIT_ASSERT( buff.put(sz, 1) == false );

			// 読み取り位置を移動せず、読み取る
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.copy(sz, 100) == 100);
			memset(sz2, 'a', sizeof(sz2));
			CPPUNIT_ASSERT( memcmp(sz, sz2, 100) == 0);
			CPPUNIT_ASSERT( buff.size() == 255 );	
			CPPUNIT_ASSERT( buff.free_size() == 0 );

			// 書き込んだ順に取り出す
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.get(sz, 100) == 100);
			CPPUNIT_ASSERT( memcmp(sz, sz2, 100) == 0);
			CPPUNIT_ASSERT( buff.size() == 155 );	
			CPPUNIT_ASSERT( buff.free_size() == 100 );

			memset(sz2, 'b', sizeof(sz2));
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.get(sz, 100) == 100);
			CPPUNIT_ASSERT( memcmp(sz, sz2, 100) == 0);
			CPPUNIT_ASSERT( buff.size() == 55 );	
			CPPUNIT_ASSERT( buff.free_size() == 200 );	

			// 前に残っている状態で書き込み
			// Cを50　書き込み
			memset(sz, 'C', sizeof(sz));
			CPPUNIT_ASSERT( buff.put(sz, 50) == true );
			CPPUNIT_ASSERT( buff.size() == 105 );	
			CPPUNIT_ASSERT( buff.free_size() == 150 );

			// 読み取り
			// bを55　読み取り
			memset(sz2, 'b', sizeof(sz2));
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.get(sz, 55) == 55);
			CPPUNIT_ASSERT( memcmp(sz, sz2, 55) == 0);
			CPPUNIT_ASSERT( buff.size() == 50 );	
			CPPUNIT_ASSERT( buff.free_size() == 205 );	

			// 読み取り
			// Cを50 読み取り
			memset(sz2, 'C', sizeof(sz2));
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.get(sz, 50) == 50);
			CPPUNIT_ASSERT( memcmp(sz, sz2, 50) == 0);
			CPPUNIT_ASSERT( buff.size() == 0 );	
			CPPUNIT_ASSERT( buff.free_size() == 255 );	
		}
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XBuffTest::testXRingBufferBL(void)
{
	try {
		XRingBufferBL buff;
		unsigned char szBuff[1024];
		unsigned char szBuffCheck[1024];

		CPPUNIT_ASSERT( buff.alloc(1023) == true );
		CPPUNIT_ASSERT( buff.size() == 0 );				// 書き込まれたバッファのサイズ
		CPPUNIT_ASSERT( buff.free_size() == 1023-4 );	// 書き込み可能なバッファサイズ
		CPPUNIT_ASSERT( buff.cur_blk_size() == 0 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 0 );	// カレントのブロックカウント


		// ブロック書き込み
		::memset(szBuff, 'x', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 500) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 500 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 1 );		// カレントのブロックカウント

		// ブロック書き込み
		::memset(szBuff, 'y', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 200) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 500 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 2 );		// カレントのブロックカウント

		// ブロック書き込み
		::memset(szBuff, 'z', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 300) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 500 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 3 );		// カレントのブロックカウント

		// 破棄せずにカレントのブロック読み込み
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'x', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.copy(szBuff, sizeof(szBuff)) == 500 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 500) == 0);

		// カレントのブロック読み込み
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'x', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.get(szBuff, sizeof(szBuff)) == 500 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 500) == 0);
		CPPUNIT_ASSERT( buff.cur_blk_size() == 200 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 2 );		// カレントのブロックカウント

		// ブロック書き込み
		::memset(szBuff, 'A', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 300) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 200 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 3 );		// カレントのブロックカウント

		// ブロック書き込み.
		// バッファFullなので、POPで破棄して書き込まれる
		::memset(szBuff, 'B', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 300) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 300 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 3 );		// カレントのブロックカウント

		// カレントのブロック読み込み
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'z', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.get(szBuff, sizeof(szBuff)) == 300 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 300) == 0);
		CPPUNIT_ASSERT( buff.cur_blk_size() == 300 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 2 );		// カレントのブロックカウント

		// カレントのブロック読み込み
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'A', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.get(szBuff, sizeof(szBuff)) == 300 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 300) == 0);
		CPPUNIT_ASSERT( buff.cur_blk_size() == 300 );		// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 1 );		// カレントのブロックカウント

		// カレントのブロック読み込み
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'B', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.get(szBuff, sizeof(szBuff)) == 300 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 300) == 0);
		CPPUNIT_ASSERT( buff.cur_blk_size() == 0 );			// カレントのブロックサイズ
		CPPUNIT_ASSERT( buff.blk_count() == 0 );		// カレントのブロックカウント

	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

