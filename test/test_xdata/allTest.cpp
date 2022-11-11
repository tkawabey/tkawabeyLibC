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
#include "XDataTest.h"
#include "XDataXLTest.h"
#include "XRichDataTest.h"


           /* ANSI 互換バージョン */
int average( int first, ... )
{
   int count = 0, sum = 0, i = first;
   va_list marker;

   va_start( marker, first );     /* 可変個の引数の初期化 */
   while( i != -1 )
   {
      sum += i;
      count++;
      i = va_arg( marker, int);
   }
   va_end( marker );              /* 可変個の引数のリセット */
   return( sum ? (sum / count) : 0 );
}



int main(int argc, char* argv[])
{
   /* 3 個の整数を渡して呼び出します (-1 は終端として使います) 。*/
//   printf( "平均: %d\n", average( 2, 3, 4, -1 ) );

   /* 4 個の整数を渡して呼び出します。*/
//   printf( "平均: %d\n", average( 5, 7, 9, 11, -1 ) );

   /* 終端 (-1) だけを渡します。*/
//   printf( "平均: %d\n", average( -1 ) );



	setlocale( LC_ALL, "Japanese" );

	CPPUNIT_NS::TestResult controller;
	CPPUNIT_NS::TestRunner runner;
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );  



	runner.addTest(XRichDataTest::suite());	//03
	runner.addTest(XDataXLTest::suite());	//02
	runner.addTest(XDataTest::suite());		//01




	runner.run(controller);


	XInflateStreamBuff	buff;
	std::iostream		stream(&buff);

    CPPUNIT_NS::CompilerOutputter outputter( &result, stream );
    outputter.write(); 
	XTRACE("%s\n", buff.toString());


	// 終了処理

	return 0;
}

