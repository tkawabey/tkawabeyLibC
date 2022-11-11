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


#include "xDBSqlite3Test.h"



int main(int argc, char* argv[])
{
	setlocale( LC_ALL, "Japanese" );

	CPPUNIT_NS::TestResult controller;
	CPPUNIT_NS::TestRunner runner;
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );  

	runner.addTest(xDBSqlite3Test::suite());		//01


	runner.run(controller);


	XInflateStreamBuff	buff;
	std::iostream		stream(&buff);

    CPPUNIT_NS::CompilerOutputter outputter( &result, stream );
    outputter.write(); 
	XTRACE("%s\n", buff.toString());
	
	printf("%s\n", buff.toString());
	// èIóπèàóù
	return 0;
}

