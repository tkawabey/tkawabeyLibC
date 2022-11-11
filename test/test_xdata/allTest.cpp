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


           /* ANSI �݊��o�[�W���� */
int average( int first, ... )
{
   int count = 0, sum = 0, i = first;
   va_list marker;

   va_start( marker, first );     /* �ό̈����̏����� */
   while( i != -1 )
   {
      sum += i;
      count++;
      i = va_arg( marker, int);
   }
   va_end( marker );              /* �ό̈����̃��Z�b�g */
   return( sum ? (sum / count) : 0 );
}



int main(int argc, char* argv[])
{
   /* 3 �̐�����n���ČĂяo���܂� (-1 �͏I�[�Ƃ��Ďg���܂�) �B*/
//   printf( "����: %d\n", average( 2, 3, 4, -1 ) );

   /* 4 �̐�����n���ČĂяo���܂��B*/
//   printf( "����: %d\n", average( 5, 7, 9, 11, -1 ) );

   /* �I�[ (-1) ������n���܂��B*/
//   printf( "����: %d\n", average( -1 ) );



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


	// �I������

	return 0;
}

