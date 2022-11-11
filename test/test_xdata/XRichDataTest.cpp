#include <iostream>
#include <fstream>
#include "XRichDataTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata_rich.h"
#include "xlib/xbuff.h"


/**
コンストラクタ
@param name [in] テスト名
*/
XRichDataTest::XRichDataTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XRichDataTest::setUp(void)
{
}


/** テスト後処理 */
void XRichDataTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XRichDataTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XRichDataTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XRichDataTest>("testNomalWrite", testNomalWrite) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XRichDataTest>("testFind", testFind) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XRichDataTest>("testAssign", testAssign) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XRichDataTest>("testChangeType", testChangeType) );
	return suite;
}


void XRichDataTest::testNomalWrite(void)
{
	XRichData	data(XDATA_TYPE_L);
	XRichData	*pParent;
	XRichData	*pData;
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





	pParent = data.addChiled(XDATA_TYPE_L);			CPPUNIT_ASSERT( pParent != NULL );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
	pData = pParent->addChiled(XDATA_TYPE_J);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
	pData = pParent->addChiled(XDATA_TYPE_UTF16);	CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueStringUTF16(strUnicode) );	
	pData = pParent->addChiled(XDATA_TYPE_B);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(szbi1, 1) );
	pData = pParent->addChiled(XDATA_TYPE_BOOL);	CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(true) );
	pData = pParent->addChiled(XDATA_TYPE_U1);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_UINT1)0xFF) );
	pData = pParent->addChiled(XDATA_TYPE_I1);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_INT1)0xFF) );
	pData = pParent->addChiled(XDATA_TYPE_U2);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_UINT2)0xFFFF) );
	pData = pParent->addChiled(XDATA_TYPE_I2);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_INT2)0xFFFF) );
	pData = pParent->addChiled(XDATA_TYPE_U4);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_UINT4)0xFFFFFFFF) );
	pData = pParent->addChiled(XDATA_TYPE_I4);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_INT4)0xFFFFFFFF) );
	pData = pParent->addChiled(XDATA_TYPE_U8);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_UINT8)0xFFFFFFFFFFFFFFFF) );
	pData = pParent->addChiled(XDATA_TYPE_I8);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_INT8)0xFFFFFFFFFFFFFFFF) );
	pData = pParent->addChiled(XDATA_TYPE_F4);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_FLOAT4)999.1234) );
	pData = pParent->addChiled(XDATA_TYPE_F8);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue((XD_FLOAT8)999.1234) );


	pParent = data.addChiled(XDATA_TYPE_L);			CPPUNIT_ASSERT( pParent != NULL );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_J);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_UTF16);	CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_B);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_BOOL);	CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_U1);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_I1);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_U2);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_I2);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_U4);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_I4);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_U8);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_I8);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_F4);		CPPUNIT_ASSERT( pData != NULL );
	pData = pParent->addChiled(XDATA_TYPE_F8);		CPPUNIT_ASSERT( pData != NULL );



	pParent = data.addChiled(XDATA_TYPE_L);			CPPUNIT_ASSERT( pParent != NULL );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
	pData = pParent->addChiled(XDATA_TYPE_J);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
	pData = pParent->addChiled(XDATA_TYPE_UTF16);	CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueStringUTF16(strUnicode) );	
	pData = pParent->addChiled(XDATA_TYPE_B);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(szbi1, sizeof(szbi1) / sizeof(unsigned char)) );
	pData = pParent->addChiled(XDATA_TYPE_BOOL);	CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valBoll, sizeof(valBoll) / sizeof(bool)) );
	pData = pParent->addChiled(XDATA_TYPE_U1);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valU1, sizeof(valU1) / sizeof(XD_UINT1)) );
	pData = pParent->addChiled(XDATA_TYPE_I1);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valI1, sizeof(valI1) / sizeof(XD_INT1)) );
	pData = pParent->addChiled(XDATA_TYPE_U2);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valU2, sizeof(valU2) / sizeof(XD_UINT2)) );
	pData = pParent->addChiled(XDATA_TYPE_I2);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valI2, sizeof(valI2) / sizeof(XD_INT2)) );
	pData = pParent->addChiled(XDATA_TYPE_U4);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valU4, sizeof(valU4) / sizeof(XD_UINT4)) );
	pData = pParent->addChiled(XDATA_TYPE_I4);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valI4, sizeof(valI4) / sizeof(XD_INT4)) );
	pData = pParent->addChiled(XDATA_TYPE_U8);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valU8, sizeof(valU8) / sizeof(XD_UINT8)) );
	pData = pParent->addChiled(XDATA_TYPE_I8);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valI8, sizeof(valI8) / sizeof(XD_INT8)) );
	pData = pParent->addChiled(XDATA_TYPE_F4);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valF4, sizeof(valF4) / sizeof(XD_FLOAT4)) );
	pData = pParent->addChiled(XDATA_TYPE_F8);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValue(valF8, sizeof(valF8) / sizeof(XD_FLOAT8)) );

	{
		std::fstream fst;
		fst.open("XRichDataTest_testNomalWrite.xml.unitest", std::ios::out);
		CPPUNIT_ASSERT( data.print(fst, 10, "\n") );
		fst.close();
	}
	{
		std::fstream fst;
		fst.open("XRichDataTest_testNomalWrite.bin.unitest", std::ios::out);
		CPPUNIT_ASSERT( data.buildBinaryData(&fst) );
		fst.close();
	}
}
void XRichDataTest::testFind(void)
{
	XRichData	data(XDATA_TYPE_L);
	XRichData	*pParent;
	XRichData	*pData;


	/*
	 L.3
	  L.3
	   A.
	   A.
	   A.
	  L.3
	   A.
	   A.
	   A.
	  L.3
	   A.
	   L.2
	     L.3
		  A.
		  A.
		  A.
		 L.2
		  A.
		  A.
	   A.
	*/
	pParent = data.addChiled(XDATA_TYPE_L);			CPPUNIT_ASSERT( pParent != NULL );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );

	pParent = data.addChiled(XDATA_TYPE_L);			CPPUNIT_ASSERT( pParent != NULL );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("あいうえお") );
	pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );

	XRichData	*pParent2;
	XRichData	*pParent3;
	pParent = data.addChiled(XDATA_TYPE_L);			CPPUNIT_ASSERT( pParent != NULL );
		pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
		CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
		pParent2 = pParent->addChiled(XDATA_TYPE_L);		CPPUNIT_ASSERT( pParent2 != NULL );
			pParent3 = pParent2->addChiled(XDATA_TYPE_L);		CPPUNIT_ASSERT( pParent3 != NULL );
				pData = pParent3->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
				CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
				pData = pParent3->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
				CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );	
				pData = pParent3->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
				CPPUNIT_ASSERT( pData->putValueString("ほげほげ") );	
			pParent3 = pParent2->addChiled(XDATA_TYPE_L);		CPPUNIT_ASSERT( pParent3 != NULL );
				pData = pParent3->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
				CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
				pData = pParent3->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
				CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );	
		pData = pParent->addChiled(XDATA_TYPE_A);		CPPUNIT_ASSERT( pData != NULL );
		CPPUNIT_ASSERT( pData->putValueString("abcdefghijklmnopqrstuvwxyz[]@;:_-=;*") );
	pData = data.find("##", 1, 1);	// ルートから、"A.あいうえお"を取得
	CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( ::strcmp( pData->getValueString(), "あいうえお") == 0  );
	pData = data.find("####", 2, 1, 0, 2);	// ルートから、"A.ほげほげ"を取得
	CPPUNIT_ASSERT( pData != NULL );
	CPPUNIT_ASSERT( ::strcmp( pData->getValueString(), "ほげほげ") == 0  );
}

void XRichDataTest::testAssign()
{
	const int loop_cnt = 500;
	int i;
	// Bin
	try {
		XRichData rdata(XDATA_TYPE_B);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_INT1)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueInt1(i) == (XD_INT1)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}

	// Boolean
	try {
		XRichData rdata(XDATA_TYPE_BOOL);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (bool) ( (i%2) == 0 )) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueBoolean(i) == (bool) ( (i%2) == 0 )  );
		}
		
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}

	// Ascii
	try {
		XRichData rdata(XDATA_TYPE_A);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_UINT1)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueUInt1(i) == (XD_UINT1)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}

	// Jis
	try {
		XRichData rdata(XDATA_TYPE_J);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_UINT1)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueUInt1(i) == (XD_UINT1)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}
	// UTF16
	try {
		XRichData rdata(XDATA_TYPE_UTF16);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_UINT2)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueUInt2(i) == (XD_UINT2)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}


	// UINT1
	try {
		XRichData rdata(XDATA_TYPE_U1);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_UINT1)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueUInt1(i) == (XD_UINT1)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}
	// INT1
	try {
		XRichData rdata(XDATA_TYPE_I1);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_INT1)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueInt1(i) == (XD_INT1)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}


	// UINT2
	try {
		XRichData rdata(XDATA_TYPE_U2);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_UINT2)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueUInt2(i) == (XD_UINT2)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}
	// INT2
	try {
		XRichData rdata(XDATA_TYPE_I2);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_INT2)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueInt2(i) == (XD_INT2)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}


	// UINT4
	try {
		XRichData rdata(XDATA_TYPE_U4);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_UINT4)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueUInt4(i) == (XD_UINT4)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}
	// INT4
	try {
		XRichData rdata(XDATA_TYPE_I4);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_INT4)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueInt4(i) == (XD_INT4)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}
	// Float4
	try {
		XRichData rdata(XDATA_TYPE_F4);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_FLOAT4)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueFloat4(i) == (XD_FLOAT4)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}


	// UINT8
	try {
		XRichData rdata(XDATA_TYPE_U8);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_UINT8)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueUInt8(i) == (XD_UINT8)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}
	// INT8
	try {
		XRichData rdata(XDATA_TYPE_I8);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_INT8)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueInt8(i) == (XD_INT8)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}
	// Float8
	try {
		XRichData rdata(XDATA_TYPE_F8);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_FLOAT8)i) );
		}
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT(  rdata.getValueFloat8(i) == (XD_FLOAT8)i );
		}
	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}

}

void XRichDataTest::testChangeType()
{
	const int loop_cnt = 500;
	int i;
	// Bin
	try {
		XRichData rdata(XDATA_TYPE_U1);
		CPPUNIT_ASSERT( rdata.assign( loop_cnt ) );
		for( i = 0; i < loop_cnt; i++ ) {
			CPPUNIT_ASSERT( rdata.setValue( i, (XD_INT1)i) );
		}


		CPPUNIT_ASSERT( rdata.changeType( XDATA_TYPE_U8 ) );
		CPPUNIT_ASSERT( rdata.getDataCount() == (XD_UINT4)loop_cnt );
		for( i = 0; i < loop_cnt; i++ ) {
			printf("%d\n", i);
			if( i == 256 ) {
				int fjksdio = 0;
			}
			CPPUNIT_ASSERT(  rdata.getValueUInt8(i) == (XD_UINT8)i );
		}

	} catch(XExcept& e) {
		CPPUNIT_ASSERT( false );
		e.print();
	}
}
