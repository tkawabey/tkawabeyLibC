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

#include "XBuffTest.h"
#include "XStrBuffTest.h"
#include "XExceptTest.h"
#include "xmlhelperTest.h"
#include "xmsgqueueTest.h"
#include "xtimeTest.h"
#include "XStrBuffCodeConvTest.h"
#include "XJSonTest.h"



int main(int argc, char* argv[])
{
	setlocale( LC_ALL, "Japanese" );
	XLIB_SET_SE_TRANSLATOR;
	XLIB_SET_PE_TRANSLATOR;


	CPPUNIT_NS::TestResult controller;
	CPPUNIT_NS::TestRunner runner;
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );  

//	runner.addTest(xmlhelperTest::suite());		//01
//	runner.addTest(XBuffTest::suite());			//01
//	runner.addTest(XStrBuffCodeConvTest::suite());		//01
//	runner.addTest(XTimeTest::suite());		//01
//	runner.addTest(xmsgqueueTest::suite());		//01
//	runner.addTest(XStrBuffTest::suite());			//01
//	runner.addTest(XExceptTest::suite());		//01
	runner.addTest(XJSonTest::suite());		//01



	runner.run(controller);


	XInflateStreamBuff	buff;
	std::iostream		stream(&buff);

    CPPUNIT_NS::CompilerOutputter outputter( &result, stream );
    outputter.write(); 
	XTRACE(_T("%s\n"), (const TCHAR*)XStrBuff(buff.toString()));
	
	printf("%s\n", buff.toString());


	// èIóπèàóù


	return 0;
}

