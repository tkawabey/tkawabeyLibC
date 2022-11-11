#include <stdio.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <iostream>
#include <locale.h>
#include "xlib/xbuff.h"
#include "xlib/xeception.h"
#include "XSecsDataTest.h"
#include "XSecsTextParserTest.h"
#include "XSecsPcapParserTest.h"
#include "XSecsHSMSPortTest.h"

int main(int argc, char* argv[])
{
	setlocale( LC_ALL, "Japanese" );

	CPPUNIT_NS::TestResult controller;
	CPPUNIT_NS::TestRunner runner;
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );  



	runner.addTest(XSecsDataTest::suite());		//01
	runner.addTest(XSecsTextParserTest::suite());		//01
//	runner.addTest(XSecsPcapParserTest::suite());		//01
	runner.addTest(XSecsHSMSPortTest::suite());		//01

	runner.run(controller);


	XInflateStreamBuff	buff;
	std::iostream		stream(&buff);

    CPPUNIT_NS::CompilerOutputter outputter( &result, stream );
    outputter.write(); 
#ifdef _DEBUG
	XTRACE(_T("%s\n"), buff.toString());
#endif
	std::cout << buff.toString() << std::endl;

	// I—¹ˆ—

	return 0;
}

