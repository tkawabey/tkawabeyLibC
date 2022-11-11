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
#include "xsemi_gem_sqlite_if_Test.h"
#include "xsemi_gem_const_Test.h"
#include "xsemi_V_Test.h"
#include "xsemi_ceid_Test.h"
#include "xsemi_rpt_Test.h"
#include "xsemi_alarm_Test.h"
#include "xsemi_ackcode_Test.h"
#include "xsemi_spool_Test.h"


int main(int argc, char* argv[])
{
	setlocale( LC_ALL, "Japanese" );
	CPPUNIT_NS::TestResult controller;
	CPPUNIT_NS::TestRunner runner;
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );  

	runner.addTest(XSEMI_Spool_Test::suite());		//03
	runner.addTest(xsemi_gem_sqlite_if_Test::suite());		//03
	runner.addTest(XSEMI_Rpt_Test::suite());				//06
	runner.addTest(XSEMI_ACKCODE_Test::suite());			//06
	runner.addTest(XSEMI_ALARM_Test::suite());				//05
	runner.addTest(XSEMI_CEID_Test::suite());				//01
	runner.addTest(XSEMI_V_Test::suite());					//02

	runner.addTest(xsemi_gem_const_Test::suite());			//04



	runner.run(controller);


	XInflateStreamBuff	buff;
	std::iostream		stream(&buff);

    CPPUNIT_NS::CompilerOutputter outputter( &result, stream );
    outputter.write(); 
#ifdef _DEBUG
	XTRACE("%s\n", buff.toString());

#else
	std::cout << buff.toString() << std::endl;
#endif


	// I—¹ˆ—

	return 0;
}

