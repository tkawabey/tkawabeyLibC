#include <iostream>
#include <fstream>
#include "XDataXLTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"


/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XDataXLTest::XDataXLTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XDataXLTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XDataXLTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XDataXLTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XDataXLTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XDataXLTest>("XDataXLTest", testNomalWrite) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XDataXLTest>("XDataXLTest", testNormalRead) );
	return suite;
}


void XDataXLTest::testNomalWrite(void)
{
	XInflateStreamBuff	buff;
	std::iostream		stream(&buff);
	unsigned char		szbi1[10] = "";
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
	XStrBuff			strUnicode("����������");

	::memset(szbi1, 'A', sizeof(szbi1));
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

	// �f�[�^��������
	XDataXLWriter	writer(&stream);
	CPPUNIT_ASSERT( writer.putValAsList("list", 14) );
	CPPUNIT_ASSERT( writer.putValAsBool("bool1", true) );
	::memset(szbi1, 0xFF, sizeof(szbi1));
	CPPUNIT_ASSERT( writer.putValAsBinary("binary1", szbi1, sizeof(szbi1)) );
	::memset(szbi1, 'A', sizeof(szbi1));
	szbi1[ sizeof(szbi1) -1 ] = 0;
	CPPUNIT_ASSERT( writer.putValAsStringA("ascii2", (char*)szbi1) );
	::memset(szbi1, 'J', sizeof(szbi1));
	szbi1[ sizeof(szbi1) -1 ] = 0;
	CPPUNIT_ASSERT( writer.putValAsStringJ("jis", (char*)szbi1) );
	CPPUNIT_ASSERT( writer.putValAsStringW("unicode", strUnicode) );
	CPPUNIT_ASSERT( writer.putValAsInt1("int1", (XD_INT1)0xFF) );
	CPPUNIT_ASSERT( writer.putValAsInt2("int2", (XD_INT2)0xFFFF) );
	CPPUNIT_ASSERT( writer.putValAsInt4("int4", (XD_INT4)0xFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt1("uint1", 0xFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt2("uint2", 0xFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt4("uint4", 0xFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsFloat4("float4", (XD_FLOAT4)123456789.123456789) );
	CPPUNIT_ASSERT( writer.putValAsFloat8("float8", 123456789.123456789) );

	CPPUNIT_ASSERT( writer.putValAsList("list", 13) );
	{
		// �f�[�^��z��Œǉ�
		CPPUNIT_ASSERT( writer.putValue("bool1", XDATA_TYPE_BOOL, valBoll, sizeof(valBoll)) );
		::memset(szbi1, 0xFF, sizeof(szbi1));
		CPPUNIT_ASSERT( writer.putValAsBinary("binary1", szbi1, sizeof(szbi1)) );
		::memset(szbi1, 'A', sizeof(szbi1));
		szbi1[ sizeof(szbi1) -1 ] = 0;
		CPPUNIT_ASSERT( writer.putValAsStringA("ascii2", (char*)szbi1) );
		::memset(szbi1, 'J', sizeof(szbi1));
		szbi1[ sizeof(szbi1) -1 ] = 0;
		CPPUNIT_ASSERT( writer.putValAsStringJ("jis", (char*)szbi1) );
		strUnicode = "�����������������������������������Ă�abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()=~|\\^~=-[]{}`:*+;/.<>,���ӉK�G���@�@";
		CPPUNIT_ASSERT( writer.putValAsStringW("unicode", strUnicode) );
		CPPUNIT_ASSERT( writer.putValue("int1", XDATA_TYPE_I1, valI1, 10) );
		CPPUNIT_ASSERT( writer.putValue("int2", XDATA_TYPE_I2, valI2, 10) );
		CPPUNIT_ASSERT( writer.putValue("int4", XDATA_TYPE_I4, valI4, 10) );
		CPPUNIT_ASSERT( writer.putValue("uint1", XDATA_TYPE_U1, valU1, 10) );
		CPPUNIT_ASSERT( writer.putValue("uint2", XDATA_TYPE_U2, valU2, 10) );
		CPPUNIT_ASSERT( writer.putValue("uint4", XDATA_TYPE_U4, valU4, 10) );
		CPPUNIT_ASSERT( writer.putValue("float4", XDATA_TYPE_F4, valF4, 10) );
		CPPUNIT_ASSERT( writer.putValue("float8", XDATA_TYPE_F8, valF8, 10) );
	}

	{
		std::fstream fst;

		fst.open("XDataXLTest_testNomalWrite.xml.unitest", std::ios::out);

		XDataXLReader reader(buff.getBuff(), buff.size()  );
		CPPUNIT_ASSERT( reader.print(fst) );
		fst.close();
	}
}

void XDataXLTest::testNormalRead(void)
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
	XStrBuff			strUnicode("����������");
	XDataAttributeList	attr;
	XDataAttribute		*pattr;

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

	// �����f�[�^�쐬
	pattr = attr.add(XDATA_TYPE_B, "attr_bi");
	CPPUNIT_ASSERT( pattr );
	CPPUNIT_ASSERT( pattr->putValAsBinary(szbi1, sizeof(szbi1)) );
	pattr = attr.add(XDATA_TYPE_BOOL, "attr_bool");
	CPPUNIT_ASSERT( pattr );
	CPPUNIT_ASSERT( pattr->putValAsBool(true) );
	pattr = attr.add(XDATA_TYPE_A, "attr_ascii");
	CPPUNIT_ASSERT( pattr );
	CPPUNIT_ASSERT( pattr->putValAsStringA("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") );
	pattr = attr.add(XDATA_TYPE_J, "attr_jis");
	CPPUNIT_ASSERT( pattr );
	CPPUNIT_ASSERT( pattr->putValAsStringJ("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") );
	pattr = attr.add(XDATA_TYPE_UTF16, "attr_utf16");
	CPPUNIT_ASSERT( pattr );
	CPPUNIT_ASSERT( pattr->putValAsStringW(XStrBuff("�ӂ����ӂ���")) );





	// �f�[�^��������
	XDataXLWriter	writer(&stream);
	CPPUNIT_ASSERT( writer.putValAsList("list", 16, &attr) );
	CPPUNIT_ASSERT( writer.putValAsBool("bool1", true) );
	CPPUNIT_ASSERT( writer.putValAsBinary("binary1", szbi1, sizeof(szbi1)) );
	CPPUNIT_ASSERT( writer.putValAsStringA("ascii2", (char*)szA1) );
	CPPUNIT_ASSERT( writer.putValAsStringJ("jis", (char*)szJ1) );
	CPPUNIT_ASSERT( writer.putValAsStringW("unicode", strUnicode) );
	CPPUNIT_ASSERT( writer.putValAsInt1("int1", (XD_INT1)0xFF) );
	CPPUNIT_ASSERT( writer.putValAsInt2("int2", (XD_INT2)0xFFFF) );
	CPPUNIT_ASSERT( writer.putValAsInt4("int4", (XD_INT4)0xFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsInt8("int8", (XD_INT8)0xFFFFFFFFFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt1("uint1", 0xFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt2("uint2", 0xFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt4("uint4", 0xFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsUInt8("uint8", 0xFFFFFFFFFFFFFFFF) );
	CPPUNIT_ASSERT( writer.putValAsFloat4("float4", (XD_FLOAT4)123456789.123456789) );
	CPPUNIT_ASSERT( writer.putValAsFloat8("float8", 123456789.123456789) );

	CPPUNIT_ASSERT( writer.putValAsList("chiled_list", 15) );
	{
		// �f�[�^��z��Œǉ�
		CPPUNIT_ASSERT( writer.putValue("bool1", XDATA_TYPE_BOOL, valBoll, sizeof(valBoll)) );
		CPPUNIT_ASSERT( writer.putValAsBinary("binary1", szbi1, sizeof(szbi1)) );
		CPPUNIT_ASSERT( writer.putValAsStringA("ascii2", (char*)szA1) );
		CPPUNIT_ASSERT( writer.putValAsStringJ("jis", (char*)szJ1) );
		strUnicode = "�����������������������������������Ă�";
		CPPUNIT_ASSERT( writer.putValAsStringW("unicode", strUnicode) );
		CPPUNIT_ASSERT( writer.putValue("int1", XDATA_TYPE_I1, valI1, 10) );
		CPPUNIT_ASSERT( writer.putValue("int2", XDATA_TYPE_I2, valI2, 10) );
		CPPUNIT_ASSERT( writer.putValue("int4", XDATA_TYPE_I4, valI4, 10) );
		CPPUNIT_ASSERT( writer.putValue("int8", XDATA_TYPE_I8, valI8, 10) );
		CPPUNIT_ASSERT( writer.putValue("uint1", XDATA_TYPE_U1, valU1, 10) );
		CPPUNIT_ASSERT( writer.putValue("uint2", XDATA_TYPE_U2, valU2, 10) );
		CPPUNIT_ASSERT( writer.putValue("uint4", XDATA_TYPE_U4, valU4, 10) );
		CPPUNIT_ASSERT( writer.putValue("uint8", XDATA_TYPE_U8, valU8, 10) );
		CPPUNIT_ASSERT( writer.putValue("float4", XDATA_TYPE_F4, valF4, 10) );
		CPPUNIT_ASSERT( writer.putValue("float8", XDATA_TYPE_F8, valF8, 10) );
	}


	{
		XDataXLReader reader(buff.getBuff(), buff.size()  );
		
		XDataXL dataXL;
		// list
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "list" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_L );	
		CPPUNIT_ASSERT( dataXL.getCount() == 16 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 16 );
		// ���X�g�͕ϊ��s�\
		unsigned char *pref = NULL;
		CPPUNIT_ASSERT( dataXL.getValAsBinaryRef(&pref) == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		dataXL.getValAsBinary(szbi1, sizeof(szbi1));
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsBool() == false );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsStringA() == "" );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsStringJ() == "" );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsStringUTF16() == "" );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsInt1() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsInt2() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsInt4() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsInt8() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsUInt1() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsUInt1() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsUInt2() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsUInt4() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsUInt8() == 0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsFloat4() == 0.0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsFloat8() == 0.0 );
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRCV );	// �ϊ��G���[
		dataXL.resetError();								// �G���[�N���A�[



		// bool  
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "bool1" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_BOOL );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 1 );
		CPPUNIT_ASSERT( dataXL.getValAsBool() == true );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
	

		// binary1
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "binary1" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_B );	
		CPPUNIT_ASSERT( dataXL.getCount() == sizeof(szbi1) );	
		CPPUNIT_ASSERT( dataXL.getSize() == sizeof(szbi1) );
		dataXL.getValAsBinary(szbi2, sizeof(szbi2));
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( ::memcmp(szbi2, szbi1, sizeof(szbi1)) == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsBinaryRef(&pref) == sizeof(szbi1) );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( ::memcmp(pref, szbi1, sizeof(szbi1)) == 0 );
		
		// ascii2
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "ascii2" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_A );	
		CPPUNIT_ASSERT( dataXL.getCount() == strlen(szA1) );	
		CPPUNIT_ASSERT( dataXL.getSize() == strlen(szA1) );
		CPPUNIT_ASSERT( dataXL.getValAsStringA() == szA1 );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// jis
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "jis" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_J );	
		CPPUNIT_ASSERT( dataXL.getCount() == strlen(szJ1) );	
		CPPUNIT_ASSERT( dataXL.getSize() == strlen(szJ1) );
		CPPUNIT_ASSERT( dataXL.getValAsStringJ() == szJ1 );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// unicode
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "unicode" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_UTF16 );	
		CPPUNIT_ASSERT( dataXL.getValAsStringUTF16() == "����������" );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// int1
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "int1" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_I1 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 1 );
		CPPUNIT_ASSERT( dataXL.getValAsInt1() == (XD_INT1)0xFF );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// int2
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "int2" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_I2 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 2 );
		CPPUNIT_ASSERT( dataXL.getValAsInt1() == (XD_INT2)0xFFFF );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// int4
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "int4" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_I4 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 4 );
		CPPUNIT_ASSERT( dataXL.getValAsInt4() == (XD_INT4)0xFFFFFFFF );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// int8
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "int8" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_I8 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 8 );
		CPPUNIT_ASSERT( dataXL.getValAsInt8() == (XD_INT8)0xFFFFFFFFFFFFFFFF );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// uint1
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "uint1" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_U1 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 1 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt1() == 0xFF );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// uint2
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "uint2" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_U2 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 2 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt2() == 0xFFFF );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// uint4
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "uint4" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_U4 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 4 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt4() == 0xFFFFFFFF );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// uint8
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "uint8" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_U8 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 8 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt8() == 0xFFFFFFFFFFFFFFFF );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );

		// float4
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "float4" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_F4 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 4 );
//		CPPUNIT_ASSERT( dataXL.getValAsFloat4() == (XD_FLOAT4)1.23457e+008 );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );


		// float8
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "float8" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_F8 );	
		CPPUNIT_ASSERT( dataXL.getCount() == 1 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 8 );
//		CPPUNIT_ASSERT( dataXL.getValAsFloat8() == 1.23457e+008 );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );





		// list
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "chiled_list" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_L );	
		CPPUNIT_ASSERT( dataXL.getCount() == 15 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 15 );

		// bool  
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "bool1" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_BOOL );	
		CPPUNIT_ASSERT( dataXL.getCount() == 10 );	
		CPPUNIT_ASSERT( dataXL.getSize() == 10 );
		CPPUNIT_ASSERT( dataXL.getValAsBool() == true );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsBool(1) == true );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsBool(9) == true );		
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsBool(10) == false );	// �w��z��ȏエ�w��		
		CPPUNIT_ASSERT( dataXL.getError() == XD2_ERRAS );
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsInt1() == 1 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt2() == (XD_INT2)0x0101 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt4() == (XD_INT4)0x01010101 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt8() == (XD_INT8)0x0101010101010101 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt1() == 0x01 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt2() == 0x0101 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt4() == 0x01010101 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt8() == 0x0101010101010101 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.getValAsFloat4();		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.getValAsFloat8();		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );



		// binary1
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "binary1" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_B );	
		CPPUNIT_ASSERT( dataXL.getCount() == sizeof(szbi1) );	
		CPPUNIT_ASSERT( dataXL.getSize() == sizeof(szbi1) );
		dataXL.getValAsBinary(szbi2, sizeof(szbi2));
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( ::memcmp(szbi2, szbi1, sizeof(szbi1)) == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsBinaryRef(&pref) == sizeof(szbi1) );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( ::memcmp(pref, szbi1, sizeof(szbi1)) == 0 );
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsInt1() == (XD_INT1)0xAB );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt2() == (XD_INT2)0xABAB );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt4() == (XD_INT4)0xABABABAB );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt8() == (XD_INT8)0xABABABABABABABAB );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt1() == 0xAB );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt2() == 0xABAB );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt4() == 0xABABABAB );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt8() == 0xABABABABABABABAB );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.getValAsFloat4();		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.getValAsFloat8();		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );


		// ascii2
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "ascii2" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_A );	
		CPPUNIT_ASSERT( dataXL.getCount() == strlen(szA1) );	
		CPPUNIT_ASSERT( dataXL.getSize() == strlen(szA1) );
		CPPUNIT_ASSERT( dataXL.getValAsStringA() == szA1 );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsInt1() == (XD_INT1)0x41 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt2() == (XD_INT2)0x4141 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt4() == (XD_INT4)0x41414141 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt8() == (XD_INT8)0x4141414141414141 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt1() == 0x41 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt2() == 0x4141 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt4() == 0x41414141 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt8() == 0x4141414141414141 );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.getValAsFloat4();		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.getValAsFloat8();		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );



		// jis
		CPPUNIT_ASSERT( reader.getValue(&dataXL) );
		CPPUNIT_ASSERT( dataXL.getTagName() == "jis" );
		CPPUNIT_ASSERT( dataXL.getType() == XDATA_TYPE_J );	
		CPPUNIT_ASSERT( dataXL.getCount() == strlen(szJ1) );	
		CPPUNIT_ASSERT( dataXL.getSize() == strlen(szJ1) );
		CPPUNIT_ASSERT( dataXL.getValAsStringJ() == szJ1 );
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.resetError();								// �G���[�N���A�[
		CPPUNIT_ASSERT( dataXL.getValAsInt1() == (XD_INT1)0x6A );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt2() == (XD_INT2)0x6A6A );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt4() == (XD_INT4)0x6A6A6A6A );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsInt8() == (XD_INT8)0x6A6A6A6A6A6A6A6A );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt1() == 0x6A );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt2() == 0x6A6A );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt4() == 0x6A6A6A6A );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		CPPUNIT_ASSERT( dataXL.getValAsUInt8() == 0x6A6A6A6A6A6A6A6A );		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.getValAsFloat4();		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );
		dataXL.getValAsFloat8();		// �ϊ�
		CPPUNIT_ASSERT( dataXL.getError() == 0 );


	}
	{
		std::fstream fst;

		fst.open("XDataXLTest_testNormalRead.xml.unitest", std::ios::out);

		XDataXLReader reader(buff.getBuff(), buff.size()  );
		CPPUNIT_ASSERT( reader.print(fst) );
		fst.close();
	}
}






