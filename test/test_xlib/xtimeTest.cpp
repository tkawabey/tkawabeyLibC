#include <iostream>
#include <fstream>
#include "xtimeTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xeception.h"
#include "xlib/xtime.h"

XTimeTest::XTimeTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


void XTimeTest::setUp(void)
{
}


void XTimeTest::tearDown(void)
{
}


CPPUNIT_NS::Test* XTimeTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XTimeTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XTimeTest>("testXBuff", &testXBuff) );
	
	return suite;
}


void XTimeTest::testXBuff(void)
{
	try {
		int i = 0;
		int iLoop = 300000;
		XTimeNonRTC	rtcTime;
		clock_t		start, stop;
		clock_t		diff1, diff2;


		start = clock();
		for( i = 0; i < iLoop; i++) {
			XTime t = XTime::getCurrentTime();
		}
		stop  = clock();
		diff1 = stop - start;


		start = clock();
		for( i = 0; i < iLoop; i++) {
			time_t msec;
			time_t sec = rtcTime.getTime(&msec);
			XTime t = sec;
		}
		stop  = clock();
		diff2 = stop - start;

		
		fprintf(stderr, "XTime       Retrive time. %u(msc)\n", diff1);
		fprintf(stderr, "XTimeNonRTC Retrive time. %u(msc)\n", diff2);

	} catch(XExcept &e) {
#ifdef _UNICODE
		e.print( std::wcerr);
#else
		e.print( std::cerr);
#endif
		CPPUNIT_ASSERT( 0  );
	}
}
