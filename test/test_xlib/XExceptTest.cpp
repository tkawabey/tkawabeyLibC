#include <iostream>
#include <fstream>
#include "XExceptTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xeception.h"


#include <windows.h>

/**
コンストラクタ
@param name [in] テスト名
*/
XExceptTest::XExceptTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XExceptTest::setUp(void)
{
}


/** テスト後処理 */
void XExceptTest::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XExceptTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XExceptTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XExceptTest>("XExceptTest", &testNomalWrite) );
	
	return suite;
}

void A_1()
{
	XEXCEPT_ENTRY
		XEXCEPT_THROW( XExcept_UNKNOWN, 0, 0);
	XEXCEPT_EXIT
}

void A()
{
	XEXCEPT_ENTRY
		A_1();
	XEXCEPT_EXIT
}

void XExceptTest::testNomalWrite(void)
{
	try {
		std::cout << "start A" << std::endl;
		A();
	} catch(XExcept &e) {
		e.printStackTrace();
	}
}
