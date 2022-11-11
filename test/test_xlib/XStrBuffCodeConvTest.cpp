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
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XStrBuffCodeConvTest::XStrBuffCodeConvTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XStrBuffCodeConvTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XStrBuffCodeConvTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
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

	str = _T("��A��B��C��D��E");

	// UniCode����AUTF8�ɕϊ�
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

	XStrBuffCodeConv::convertNativeToUTF8("����������", str2, true);
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertNativeToUTF8("����������", str2, true) );
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertUTF8ToNative((const char*)(TCHAR*)str2, str3, &bCesu) );
	CPPUNIT_ASSERT( str3.Compare( _T("����������") ) == 0 );

	// Native=Unicode
	str2 = "";
	str3 = "";
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertNativeToUTF8(L"�w���X�w�c", str2, true) );
	CPPUNIT_ASSERT( XStrBuffCodeConv::convertUTF8ToNative((const char*)(TCHAR*)str2, str3, &bCesu) );
	CPPUNIT_ASSERT( str3.Compare( _T("�w���X�w�c") ) == 0 );

	
}
