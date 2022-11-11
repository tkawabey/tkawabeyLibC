#include <iostream>
#include <fstream>
#include "XDataTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"


/**
コンストラクタ
@param name [in] テスト名
*/
XDataTest::XDataTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XDataTest::setUp(void)
{
}


/** テスト後処理 */
void XDataTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XDataTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XDataTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testNomalWrite) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testNormalRead) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testBoolValid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testBinaryValid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testAsciiValid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testJISValid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testUTF16Valid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testInt1Valid) );	
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testUInt1Valid) );	
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testInt2Valid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testUInt2Valid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testInt4Valid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testUInt4Valid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testInt8Valid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testUInt8Valid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testFloat4Valid) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testFloat8Valid) );
	
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataTest>("XDataTest", testListValid) );
	
	
	return suite;
}


void XDataTest::testNomalWrite(void)
{
	XInflateStreamBuff	buff;
	std::iostream		stream(&buff);
	unsigned char		szbi1[10] = "";
	char				szA1[100];
	char				szJ1[100];
	bool                valBoll[10];
	XD_INT1				valI1[10];
	XD_INT2				valI2[10];
	XD_INT4				valI4[10];
	XD_INT8				valI8[10];
	XD_UINT1			valU1[10];
	XD_UINT2			valU2[10];
	XD_UINT4			valU4[10];
	XD_UINT8			valU8[10];
	XD_FLOAT4			valF4[10];
	XD_FLOAT8			valF8[10];
	XStrBuff			strUnicode("あいうえお");

	::memset(szbi1, 'A', sizeof(szbi1));
	::memset(szbi1, 0xAB, sizeof(szbi1));
	::memset(szA1, 'A', sizeof(szA1));
	szA1[ sizeof(szA1) -1 ] = 0;
	::memset(szJ1, 'j', sizeof(szJ1));
	szJ1[ sizeof(szJ1) -1 ] = 0;
	::memset(valBoll, true, sizeof(valBoll));
	::memset(valI1, 0x88, sizeof(valI1));
	::memset(valI2, 0x77, sizeof(valI2));
	::memset(valI4, 0xAA, sizeof(valI4));
	::memset(valI8, 0xBB, sizeof(valI8));
	::memset(valU1, 0x88, sizeof(valU1));
	::memset(valU2, 0x77, sizeof(valU2));
	::memset(valU4, 0xAA, sizeof(valU4));
	::memset(valU8, 0xBB, sizeof(valU8));
	::memset(valF4, 0xCC, sizeof(valF4));
	::memset(valF8, 0xDD, sizeof(valF8));

	// データ書き込み
	XDataWriter	writer(&stream);
	CPPUNIT_ASSERT( writer.putValAsList(16) );
	CPPUNIT_ASSERT( writer.putValAsBool(true) );
	::memset(szbi1, 0xFF, sizeof(szbi1));
	CPPUNIT_ASSERT( writer.putValAsBinary( szbi1, sizeof(szbi1)) );
	CPPUNIT_ASSERT( writer.putValAsStringA(szA1) );
	CPPUNIT_ASSERT( writer.putValAsStringJ(szJ1) );
	CPPUNIT_ASSERT( writer.putValAsStringW(strUnicode) );
	CPPUNIT_ASSERT( writer.putValAsInt1((XD_INT1)0xFF) );
	CPPUNIT_ASSERT( writer.putValAsInt2((XD_INT2)0xFFFF) );
	CPPUNIT_ASSERT( writer.putValAsInt4((XD_INT4)0xFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsInt8((XD_INT8)0xFFFFFFFFFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt1(0xFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt2(0xFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt4(0xFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt8(0xFFFFFFFFFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsFloat4((XD_FLOAT4)123456789.123456789) );
	CPPUNIT_ASSERT( writer.putValAsFloat8(123456789.123456789) );

	CPPUNIT_ASSERT( writer.putValAsList(15) );
	{
		// データを配列で追加
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_BOOL, valBoll, sizeof(valBoll)) );
		CPPUNIT_ASSERT( writer.putValAsBinary(szbi1, sizeof(szbi1)) );
		CPPUNIT_ASSERT( writer.putValAsStringA(szA1) );
		CPPUNIT_ASSERT( writer.putValAsStringJ(szJ1) );
		strUnicode = "あいうえおかきくけこさしすせそたちつてとabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()=~|\\^~=-[]{}`:*+;/.<>,亜意卯絵尾　　";
		CPPUNIT_ASSERT( writer.putValAsStringW(strUnicode) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I1, valI1, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I2, valI2, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I4, valI4, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I8, valI8, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U1, valU1, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U2, valU2, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U4, valU4, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U8, valU8, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F4, valF4, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F8, valF8, 10) );
	}

	{
		std::fstream fst;

		fst.open("XDataTest_testNomalWrite.xml.unitest", std::ios::out);

		XDataReader reader(buff.getBuff(), buff.size()  );
		CPPUNIT_ASSERT( reader.print(fst) );
		fst.close();
	}
}

void XDataTest::testNormalRead(void)
{
	XInflateStreamBuff	buff;
	std::iostream		stream(&buff);
	unsigned char		szbi1[0xff] = "";
	unsigned char		szbi2[0xff] = "";
	char				szA1[100];
	char				szJ1[100];
	bool                valBoll[10];
	XD_INT1				valI1[10];
	XD_INT2				valI2[10];
	XD_INT4				valI4[10];
	XD_INT8				valI8[10];
	XD_UINT1			valU1[10];
	XD_UINT2			valU2[10];
	XD_UINT4			valU4[10];
	XD_UINT8			valU8[10];
	XD_FLOAT4			valF4[10];
	XD_FLOAT8			valF8[10];
	XStrBuff			strUnicode("あいうえお");


	::memset(szbi1, 0xAB, sizeof(szbi1));
	::memset(szA1, 'A', sizeof(szA1));
	szA1[ sizeof(szA1) -1 ] = 0;
	::memset(szJ1, 'j', sizeof(szJ1));
	szJ1[ sizeof(szJ1) -1 ] = 0;
	::memset(valBoll, true, sizeof(valBoll));
	::memset(valI1, 0x88, sizeof(valI1));
	::memset(valI2, 0x77, sizeof(valI2));
	::memset(valI4, 0xAA, sizeof(valI4));
	::memset(valI8, 0xBB, sizeof(valI8));
	::memset(valU1, 0x88, sizeof(valU1));
	::memset(valU2, 0x77, sizeof(valU2));
	::memset(valU4, 0xAA, sizeof(valU4));
	::memset(valU8, 0xBB, sizeof(valU8));
	::memset(valF4, 0xCC, sizeof(valF4));
	::memset(valF8, 0xDD, sizeof(valF8));



	// データ書き込み
	XDataWriter	writer(&stream);
	CPPUNIT_ASSERT( writer.putValAsList(16) );
	CPPUNIT_ASSERT( writer.putValAsBool(true) );
	CPPUNIT_ASSERT( writer.putValAsBinary( szbi1, sizeof(szbi1)) );
	CPPUNIT_ASSERT( writer.putValAsStringA(szA1) );
	CPPUNIT_ASSERT( writer.putValAsStringJ(szJ1) );
	CPPUNIT_ASSERT( writer.putValAsStringW(strUnicode) );
	CPPUNIT_ASSERT( writer.putValAsInt1((XD_INT1)0xFF) );
	CPPUNIT_ASSERT( writer.putValAsInt2((XD_INT2)0xFFFF) );
	CPPUNIT_ASSERT( writer.putValAsInt4((XD_INT4)0xFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsInt8((XD_INT8)0xFFFFFFFFFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt1(0xFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt2(0xFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt4(0xFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt8(0xFFFFFFFFFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsFloat4((XD_FLOAT4)123456789.123456789) );
	CPPUNIT_ASSERT( writer.putValAsFloat8(123456789.123456789) );

	CPPUNIT_ASSERT( writer.putValAsList(15) );
	{
		// データを配列で追加
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_BOOL, valBoll, sizeof(valBoll)) );
		CPPUNIT_ASSERT( writer.putValAsBinary(szbi1, sizeof(szbi1)) );
		CPPUNIT_ASSERT( writer.putValAsStringA(szA1) );
		CPPUNIT_ASSERT( writer.putValAsStringJ(szJ1) );
		strUnicode = "あいうえおかきくけこさしすせそたちつてとabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()=~|\\^~=-[]{}`:*+;/.<>,亜意卯絵尾　　";
		CPPUNIT_ASSERT( writer.putValAsStringW(strUnicode) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I1, valI1, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I2, valI2, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I4, valI4, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I8, valI8, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U1, valU1, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U2, valU2, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U4, valU4, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U8, valU8, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F4, valF4, 10) );
		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F8, valF8, 10) );
	}


	{
		XDataReader reader(buff.getBuff(), buff.size()  );
		
		XData data;
		// list
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_L );	
		CPPUNIT_ASSERT( data.getCount() == 16 );	
		CPPUNIT_ASSERT( data.getSize() == 16 );
		// リストは変換不可能
		unsigned char *pref = NULL;
		CPPUNIT_ASSERT( data.getValAsBinaryRef(&pref) == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		data.getValAsBinary(szbi1, sizeof(szbi1));
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsBool() == false );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsStringA() == "" );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsStringJ() == "" );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsStringUTF16() == "" );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsInt1() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsInt2() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsInt4() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsInt8() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsUInt1() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsUInt1() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsUInt2() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsUInt4() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsUInt8() == 0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsFloat4() == 0.0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsFloat8() == 0.0 );
		CPPUNIT_ASSERT( data.getError() == XD2_ERRCV );	// 変換エラー
		data.resetError();								// エラークリアー



		// bool  
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_BOOL );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 1 );
		CPPUNIT_ASSERT( data.getValAsBool() == true );
		CPPUNIT_ASSERT( data.getError() == 0 );
	

		// binary1
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_B );	
		CPPUNIT_ASSERT( data.getCount() == sizeof(szbi1) );	
		CPPUNIT_ASSERT( data.getSize() == sizeof(szbi1) );
		data.getValAsBinary(szbi2, sizeof(szbi2));
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( ::memcmp(szbi2, szbi1, sizeof(szbi1)) == 0 );
		CPPUNIT_ASSERT( data.getValAsBinaryRef(&pref) == sizeof(szbi1) );
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( ::memcmp(pref, szbi1, sizeof(szbi1)) == 0 );
		
		// ascii2
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_A );	
		CPPUNIT_ASSERT( data.getCount() == strlen(szA1) );	
		CPPUNIT_ASSERT( data.getSize() == strlen(szA1) );
		CPPUNIT_ASSERT( data.getValAsStringA() == szA1 );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// jis
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_J );	
		CPPUNIT_ASSERT( data.getCount() == strlen(szJ1) );	
		CPPUNIT_ASSERT( data.getSize() == strlen(szJ1) );
		CPPUNIT_ASSERT( data.getValAsStringJ() == szJ1 );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// unicode
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_UTF16 );	
		CPPUNIT_ASSERT( data.getValAsStringUTF16() == "あいうえお" );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// int1
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I1 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 1 );
		CPPUNIT_ASSERT( data.getValAsInt1() == (XD_INT1)0xFF );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// int2
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I2 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 2 );
		CPPUNIT_ASSERT( data.getValAsInt1() == (XD_INT2)0xFFFF );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// int4
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I4 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 4 );
		CPPUNIT_ASSERT( data.getValAsInt4() == (XD_INT4)0xFFFFFFFF );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// int8
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I8 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 8 );
		CPPUNIT_ASSERT( data.getValAsInt8() == (XD_INT8)0xFFFFFFFFFFFFFFFF );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// uint1
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U1 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 1 );
		CPPUNIT_ASSERT( data.getValAsUInt1() == 0xFF );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// uint2
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U2 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 2 );
		CPPUNIT_ASSERT( data.getValAsUInt2() == 0xFFFF );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// uint4
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U4 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 4 );
		CPPUNIT_ASSERT( data.getValAsUInt4() == 0xFFFFFFFF );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// uint8
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U8 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 8 );
		CPPUNIT_ASSERT( data.getValAsUInt8() == 0xFFFFFFFFFFFFFFFF );
		CPPUNIT_ASSERT( data.getError() == 0 );

		// float4
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_F4 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 4 );
//		CPPUNIT_ASSERT( data.getValAsFloat4() == (XD_FLOAT4)1.23457e+008 );
		CPPUNIT_ASSERT( data.getError() == 0 );


		// float8
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_F8 );	
		CPPUNIT_ASSERT( data.getCount() == 1 );	
		CPPUNIT_ASSERT( data.getSize() == 8 );
//		CPPUNIT_ASSERT( data.getValAsFloat8() == 1.23457e+008 );
		CPPUNIT_ASSERT( data.getError() == 0 );





		// list
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_L );	
		CPPUNIT_ASSERT( data.getCount() == 15 );	
		CPPUNIT_ASSERT( data.getSize() == 15 );

		// bool  
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_BOOL );	
		CPPUNIT_ASSERT( data.getCount() == 10 );	
		CPPUNIT_ASSERT( data.getSize() == 10 );
		CPPUNIT_ASSERT( data.getValAsBool() == true );
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsBool(1) == true );
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsBool(9) == true );		
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsBool(10) == false );	// 指定配列以上お指定		
		CPPUNIT_ASSERT( data.getError() == XD2_ERRAS );
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsInt1() == 1 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt2() == (XD_INT2)0x0101 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt4() == (XD_INT4)0x01010101 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt8() == (XD_INT8)0x0101010101010101 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt1() == 0x01 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt2() == 0x0101 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt4() == 0x01010101 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt8() == 0x0101010101010101 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.getValAsFloat4();		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.getValAsFloat8();		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );



		// binary1
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_B );	
		CPPUNIT_ASSERT( data.getCount() == sizeof(szbi1) );	
		CPPUNIT_ASSERT( data.getSize() == sizeof(szbi1) );
		data.getValAsBinary(szbi2, sizeof(szbi2));
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( ::memcmp(szbi2, szbi1, sizeof(szbi1)) == 0 );
		CPPUNIT_ASSERT( data.getValAsBinaryRef(&pref) == sizeof(szbi1) );
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( ::memcmp(pref, szbi1, sizeof(szbi1)) == 0 );
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsInt1() == (XD_INT1)0xAB );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt2() == (XD_INT2)0xABAB );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt4() == (XD_INT4)0xABABABAB );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt8() == (XD_INT8)0xABABABABABABABAB );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt1() == 0xAB );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt2() == 0xABAB );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt4() == 0xABABABAB );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt8() == 0xABABABABABABABAB );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.getValAsFloat4();		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.getValAsFloat8();		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );


		// ascii2
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_A );	
		CPPUNIT_ASSERT( data.getCount() == strlen(szA1) );	
		CPPUNIT_ASSERT( data.getSize() == strlen(szA1) );
		CPPUNIT_ASSERT( data.getValAsStringA() == szA1 );
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsInt1() == (XD_INT1)0x41 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt2() == (XD_INT2)0x4141 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt4() == (XD_INT4)0x41414141 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt8() == (XD_INT8)0x4141414141414141 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt1() == 0x41 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt2() == 0x4141 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt4() == 0x41414141 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt8() == 0x4141414141414141 );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.getValAsFloat4();		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.getValAsFloat8();		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );



		// jis
		CPPUNIT_ASSERT( reader.getValue(&data) );
		CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_J );	
		CPPUNIT_ASSERT( data.getCount() == strlen(szJ1) );	
		CPPUNIT_ASSERT( data.getSize() == strlen(szJ1) );
		CPPUNIT_ASSERT( data.getValAsStringJ() == szJ1 );
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.resetError();								// エラークリアー
		CPPUNIT_ASSERT( data.getValAsInt1() == (XD_INT1)0x6A );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt2() == (XD_INT2)0x6A6A );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt4() == (XD_INT4)0x6A6A6A6A );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsInt8() == (XD_INT8)0x6A6A6A6A6A6A6A6A );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt1() == 0x6A );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt2() == 0x6A6A );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt4() == 0x6A6A6A6A );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		CPPUNIT_ASSERT( data.getValAsUInt8() == 0x6A6A6A6A6A6A6A6A );		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.getValAsFloat4();		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );
		data.getValAsFloat8();		// 変換
		CPPUNIT_ASSERT( data.getError() == 0 );


	}
	{
		std::fstream fst;

		fst.open("XDataTest_testNormalRead.xml.unitest", std::ios::out);

		XDataReader reader(buff.getBuff(), buff.size()  );
		CPPUNIT_ASSERT( reader.print(fst) );
		fst.close();
	}
}

void XDataTest::testListValid()
{
	{
		// test 0 list.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValAsList(0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_L );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testListValid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int1 of sizeOfsize list.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValAsList(0xFF) );
		{
			for(XD_UINT4 i = 0; i < 0xFF; i++) {
				CPPUNIT_ASSERT( writer.putValAsStringA("ABCD") );
			}
		}

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_L );	
			CPPUNIT_ASSERT( data.getCount() == 0xFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFF );
			for(XD_UINT4 i = 0; i < 0xFF; i++) {
				CPPUNIT_ASSERT( reader.getValue(&data) );
				CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_A );	
				CPPUNIT_ASSERT( data.getCount() == 4 );	
				CPPUNIT_ASSERT( data.getSize() == 4 );
			}
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testListValid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize list.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValAsList(0xFFFF) );
		{
			for(XD_UINT4 i = 0; i < 0xFFFF; i++) {
				CPPUNIT_ASSERT( writer.putValAsStringA("ABCD") );
			}
		}

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_L );	
			CPPUNIT_ASSERT( data.getCount() == 0xFFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFF );
			for(XD_UINT4 i = 0; i < 0xFFFF; i++) {
				CPPUNIT_ASSERT( reader.getValue(&data) );
				CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_A );	
				CPPUNIT_ASSERT( data.getCount() == 4 );	
				CPPUNIT_ASSERT( data.getSize() == 4 );
			}
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testListValid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
	{
		// test over_int2 of sizeOfsize list.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValAsList(0x0001FFFF) );
		{
			for(XD_UINT4 i = 0; i < 0x0001FFFF; i++) {
				CPPUNIT_ASSERT( writer.putValAsStringA("ABCD") );
			}
		}

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_L );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFF );
			for(XD_UINT4 i = 0; i < 0x0001FFFF; i++) {
				CPPUNIT_ASSERT( reader.getValue(&data) );
				CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_A );	
				CPPUNIT_ASSERT( data.getCount() == 4 );	
				CPPUNIT_ASSERT( data.getSize() == 4 );
			}
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testListValid_03.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}


}

void XDataTest::testBoolValid()
{
	{
		// test 0 bool.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_BOOL, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_BOOL );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testBoolValid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize bool.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		bool valBool[0xFFFF];

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_BOOL, valBool, 0xFFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_BOOL );	
			CPPUNIT_ASSERT( data.getCount() == 0xFFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testBoolValid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize bool.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		bool valBool[0x0001FFFF];

		::memset(valBool, true, sizeof(valBool));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_BOOL, valBool, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_BOOL );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testBoolValid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}

void XDataTest::testBinaryValid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_B, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_B );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testBinaryValid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		unsigned char valBool[0xFFFF];

		::memset(valBool, -1, sizeof(valBool));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_B, valBool, 0xFFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_B );	
			CPPUNIT_ASSERT( data.getCount() == 0xFFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testBinaryValid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		unsigned char valBool[0x0001FFFF];

		::memset(valBool, -1, sizeof(valBool));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_B, valBool, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_B );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testBinaryValid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}


void XDataTest::testAsciiValid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_A, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_A );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testAsciiValid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		char val[0xFFFF];

		::memset(val, 'A', sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_A, val, 0xFFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_A );	
			CPPUNIT_ASSERT( data.getCount() == 0xFFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testAsciiValid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		unsigned char val[0x0001FFFF];

		::memset(val, 'A', sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_A, val, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_A );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testAsciiValid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}

void XDataTest::testJISValid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_J, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_J );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testJISValid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		char val[0xFFFF];

		::memset(val, 'J', sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_J, val, 0xFFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_J );	
			CPPUNIT_ASSERT( data.getCount() == 0xFFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testJISValid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		unsigned char val[0x0001FFFF];

		::memset(val, 'J', sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_J, val, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_J );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testJISValid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}


void XDataTest::testUTF16Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_UTF16, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_UTF16 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUTF16Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		unsigned short val[0x7FFF];


		{
			for(XD_UINT4 i = 0; i < 0x7FFF; i++) {
				val[i] = 0x0041;
			}
		}

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_UTF16, val, 0x7FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_UTF16 );	
			CPPUNIT_ASSERT( data.getCount() == 0x7FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFE );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUTF16Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		unsigned short val[0x0001FFFF];

		{
			for(XD_UINT4 i = 0; i < 0x0001FFFF; i++) {
				val[i] = 0x0041;
			}
		}

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_UTF16, val, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_UTF16 );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0003FFFE );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUTF16Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}

void XDataTest::testInt1Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I1, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I1 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt1Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_INT1 val[0xFFFF];

		::memset(val, 'J', sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I1, val, 0xFFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I1 );	
			CPPUNIT_ASSERT( data.getCount() == 0xFFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt1Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_INT1 val[0x0001FFFF];

		::memset(val, 'J', sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I1, val, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I1 );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt1Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}

void XDataTest::testUInt1Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U1, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U1 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt1Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_UINT1 val[0xFFFF];

		::memset(val, 0xff, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U1, val, 0xFFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U1 );	
			CPPUNIT_ASSERT( data.getCount() == 0xFFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt1Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_UINT1 val[0x0001FFFF];

		::memset(val, 0xFF, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U1, val, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U1 );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFF );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt1Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}





void XDataTest::testInt2Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I2, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I2 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt2Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_INT2 val[0x7FFF];



		::memset(val, 0xFF, sizeof(val));


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I2, val, 0x7FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I2 );	
			CPPUNIT_ASSERT( data.getCount() == 0x7FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFE );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt2Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_INT2 val[0x0001FFFF];


		::memset(val, 0xFF, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I2, val, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I2 );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0003FFFE );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt2Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}



void XDataTest::testUInt2Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U2, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U2 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt2Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_UINT2 val[0x7FFF];



		::memset(val, 0xFF, sizeof(val));


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U2, val, 0x7FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U2 );	
			CPPUNIT_ASSERT( data.getCount() == 0x7FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFE );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt2Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_UINT2 val[0x0001FFFF];


		::memset(val, 0xFF, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U2, val, 0x0001FFFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U2 );	
			CPPUNIT_ASSERT( data.getCount() == 0x0001FFFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0003FFFE );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt2Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}




void XDataTest::testInt4Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I4, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I4 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt4Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_INT4 val[0x3FFF];



		::memset(val, 0xFF, sizeof(val));


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I4, val, 0x3FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I4 );	
			CPPUNIT_ASSERT( data.getCount() == 0x3FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFC );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt4Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_INT4 val[0x7FFF];


		::memset(val, 0xFF, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I4, val, 0x7FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I4 );	
			CPPUNIT_ASSERT( data.getCount() == 0x7FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFC );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt4Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}

void XDataTest::testUInt4Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U4, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U4 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt4Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_UINT4 val[0x3FFF];



		::memset(val, 0xFF, sizeof(val));


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U4, val, 0x3FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U4 );	
			CPPUNIT_ASSERT( data.getCount() == 0x3FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFC );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt4Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_UINT4 val[0x7FFF];


		::memset(val, 0xFF, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U4, val, 0x7FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U4 );	
			CPPUNIT_ASSERT( data.getCount() == 0x7FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFC );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt4Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}

void XDataTest::testInt8Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I8, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I8 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt8Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_INT8 val[0x1FFF];



		::memset(val, 0xFF, sizeof(val));


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I8, val, 0x1FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I8 );	
			CPPUNIT_ASSERT( data.getCount() == 0x1FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFF8 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt8Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_INT8 val[0x3FFF];


		::memset(val, 0xFF, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_I8, val, 0x3FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_I8 );	
			CPPUNIT_ASSERT( data.getCount() == 0x3FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFF8 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testInt8Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}


void XDataTest::testUInt8Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U8, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U8 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt8Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_UINT8 val[0x1FFF];



		::memset(val, 0xFF, sizeof(val));


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U8, val, 0x1FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U8 );	
			CPPUNIT_ASSERT( data.getCount() == 0x1FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFF8 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt8Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_UINT8 val[0x3FFF];


		::memset(val, 0xFF, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_U8, val, 0x3FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_U8 );	
			CPPUNIT_ASSERT( data.getCount() == 0x3FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFF8 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testUInt8Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}

void XDataTest::testFloat4Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F4, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_F4 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testFloat4Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_FLOAT4 val[0x3FFF];



		::memset(val, 0x01, sizeof(val));


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F4, val, 0x3FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_F4 );	
			CPPUNIT_ASSERT( data.getCount() == 0x3FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFFC );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testFloat4Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_FLOAT4 val[0x7FFF];


		::memset(val, 0x01, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F4, val, 0x7FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_F4 );	
			CPPUNIT_ASSERT( data.getCount() == 0x7FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFFC );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testFloat4Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}

void XDataTest::testFloat8Valid()
{
	{
		// test 0 binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F8, 0, 0) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_F8 );	
			CPPUNIT_ASSERT( data.getCount() == 0 );	
			CPPUNIT_ASSERT( data.getSize() == 0 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testFloat8Valid_00.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test max_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_FLOAT8 val[0x1FFF];



		::memset(val, 0x01, sizeof(val));


		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F8, val, 0x1FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_F8 );	
			CPPUNIT_ASSERT( data.getCount() == 0x1FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0xFFF8 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testFloat8Valid_01.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}

	{
		// test over_int2 of sizeOfsize binary.
		XInflateStreamBuff	buff;
		std::iostream		stream(&buff);
		XDataWriter	writer(&stream);
		XD_FLOAT8 val[0x3FFF];


		::memset(val, 0x01, sizeof(val));

		CPPUNIT_ASSERT( writer.putValue(XDATA_TYPE_F8, val, 0x3FFF) );

		{
			XData data;
			XDataReader reader(buff.getBuff(), buff.size()  );
			// list
			CPPUNIT_ASSERT( reader.getValue(&data) );
			CPPUNIT_ASSERT( data.getType() == XDATA_TYPE_F8 );	
			CPPUNIT_ASSERT( data.getCount() == 0x3FFF );	
			CPPUNIT_ASSERT( data.getSize() == 0x0001FFF8 );
			CPPUNIT_ASSERT( reader.getValue(&data) == false );
		}
		{
			std::fstream fst;
			fst.open("XDataTest_testFloat8Valid_02.xml.unitest", std::ios::out);
			XDataReader reader(buff.getBuff(), buff.size()  );
			CPPUNIT_ASSERT( reader.print(fst) );
			fst.close();
		}
	}
}
