#include <iostream>
#include <fstream>
#include "XBuffTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xeception.h"


#include <windows.h>
#ifdef _UNICODE
	#define XLIB_TEST_STD_ERR	std::wcerr
#else
	#define XLIB_TEST_STD_ERR	std::cerr
#endif
/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XBuffTest::XBuffTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XBuffTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XBuffTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XBuffTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XBuffTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXBuff", &testXBuff) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXInflateStreamBuff", &testXInflateStreamBuff) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXInflateStreamBuff2", &testXInflateStreamBuff2) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXRingBuffer", &testXRingBuffer) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XBuffTest>("testXRingBufferBL", &testXRingBufferBL) );
	
	return suite;
}


void XBuffTest::testXBuff(void)
{
	try {
		int i;
		unsigned char *p;
		unsigned char *p_end;
		XBuff buff;
		XBuff buff2;

		p = (unsigned char*)buff.Alloc(10);
		CPPUNIT_ASSERT( p );
		CPPUNIT_ASSERT( buff.GetBuffSize() == 10  );
		p_end = buff;
		CPPUNIT_ASSERT( p == p_end );
		p_end = (unsigned char*)buff.GetMaxBuffAddress();
		CPPUNIT_ASSERT( (p_end - p) == 10 );
		
		buff.MemSet( 0 );
		for( i = 0; i < 10; i++ ) {
			CPPUNIT_ASSERT( p[0] == 0);
		}

		p = (unsigned char*)buff.ReAlloc(20);
		CPPUNIT_ASSERT( buff.GetBuffSize() == 20  );
		p_end = buff;
		CPPUNIT_ASSERT( p == p_end );
		p_end = (unsigned char*)buff.GetMaxBuffAddress();
		CPPUNIT_ASSERT( (p_end - p) == 20 );
		
		buff.MemSet( 0xFA );
		for( i = 0; i < 20; i++ ) {
			CPPUNIT_ASSERT( p[0] == 0xFA);
		}

		// �R�s�[
		buff2.DoCopy( &buff );

		// ���f�[�^�����
		buff.Free();
		CPPUNIT_ASSERT( buff.GetBuffSize() == 0  );

		p = (unsigned char*)buff2;
		CPPUNIT_ASSERT( buff2.GetBuffSize() == 20  );
		p_end = buff2;
		CPPUNIT_ASSERT( p == p_end );
		p_end = (unsigned char*)buff2.GetMaxBuffAddress();
		CPPUNIT_ASSERT( (p_end - p) == 20 );
		// �����f�[�^�ł��邱��
		for( i = 0; i < 20; i++ ) {
			CPPUNIT_ASSERT( p[0] == 0xFA);
		}
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XBuffTest::testXInflateStreamBuff(void)
{
	try {
		int i;
		XInflateStreamBuff buff(10, 6);

		
		CPPUNIT_ASSERT( buff.getBuffSize() == 10 );

		CPPUNIT_ASSERT( buff.sputn("0123456789", 10) == 10 );
		CPPUNIT_ASSERT( strcmp(buff.toString(), "0123456789") == 0 );

		CPPUNIT_ASSERT( buff.sputn("01234", 5) == 5 );

		// �o�b�t�@�T�C�Y���g������Ă���͂�
		//	10+6
		CPPUNIT_ASSERT( buff.getBuffSize() == 16 );
		CPPUNIT_ASSERT( buff.size() == 15 );

		{
			const char *p = buff.getCurrentAddress();
			CPPUNIT_ASSERT( p );

			CPPUNIT_ASSERT( buff.sputn("5", 1) == 1 );
			CPPUNIT_ASSERT( *p == '5' );
		}
		{
			unsigned char* p = buff.getBuff();
			CPPUNIT_ASSERT( memcmp(p, "012345678901234", 15) == 0 );
		}

		// ���Z�b�g
		buff.resetp();
		CPPUNIT_ASSERT( buff.sputn("ABCDEFGHIJ", 10) == 10 );
		CPPUNIT_ASSERT( strcmp(buff.toString(), "ABCDEFGHIJ") == 0 );
		CPPUNIT_ASSERT( buff.size() == 10 );

		// �傫�ȃf�[�^��������
		int loop = 10000;
		buff.resetp();
		for( i = 0; i < loop; i++) {
			CPPUNIT_ASSERT( buff.sputn("1", 1) == 1 );
		}
		CPPUNIT_ASSERT( buff.size() == loop );
		{
			unsigned char* p = buff.getBuff();
			for( i = 0; i < loop; i++) {
				CPPUNIT_ASSERT( *p == '1' );
			}
		}
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		
		XInflateStreamBuff buff(500, 500);
		const TCHAR* pData = _T(" 0123456789/*-+!\"#$%&'()=\r\n\t~|\\^[]{}@`:*;+\\/?.>,<_");
		for(int i = 0; i < 100; i++ ) {
			std::ostream tmpStream(&buff);
			for(int j = 0; j < 100; j++ ) {
				tmpStream << pData;
			}
			buff.toString();
			buff.resetp();
		}
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

}


void XBuffTest::testXInflateStreamBuff2(void)
{
	try {
		// �C���t���[�g���Ȃ��i6�����j
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << _T("012345");

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("012345")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	
	try {
		// �C���t���[�g���Ȃ��i�X�����j�BNULL���܂߂ă������[���E�M���M��
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << _T("012345678");

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("012345678")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	
	
	try {
		// �C���t���[�g����i�P�S�����j�B
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << _T("012345678");
		stream << _T("90125");

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("01234567890125")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	
	try {
		// �}���`�o�C�g�������w��
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << _T("����������");
		stream << _T("����������");

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("��������������������")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	
	try {
		// char�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (char)'A';
		stream << (char)'B';

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("AB")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// unsigned char�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (unsigned char)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// short�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (short)123;
		stream << (short)-456;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123-456")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// unsigned short�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (unsigned short)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// int�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (int)123;
		stream << (int)-456;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123-456")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// unsigned int�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (unsigned int)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// long�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (long)123;
		stream << (long)-456;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123-456")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// unsigned long�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (unsigned long)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// XD_INT8�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (XD_INT8)123;
		stream << (XD_INT8)-456;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123-456")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// XD_UINT8�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (XD_UINT8)123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// float�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (float)0.123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("0.123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// double�l
		XLIB_STD_STREAM_BUFFER buff(10, 6);
		XLIB_STD_OUT_STR_STREAM stream( &buff );

		stream << (double)0.123;

		CPPUNIT_ASSERT_EQUAL(0, XStrBuff::StrCmp(buff.toString(), _T("0.123")));
		
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XBuffTest::testXRingBuffer(void)
{
	try {
		XRingBuffer buff;
		unsigned char sz[255];
		unsigned char sz2[255];
		unsigned char _attach_buff[500];

		for(int i = 0; i < 2; i++) {
			// �f�[�^��������������
			memset(sz, 'a', sizeof(sz));
			if( i == 0 ) {
				CPPUNIT_ASSERT( buff.alloc(255) == true );
			} else {
				CPPUNIT_ASSERT( buff.attach( &_attach_buff[10], 255) == true );
			}

			CPPUNIT_ASSERT( buff.size() == 0 );	
			CPPUNIT_ASSERT( buff.free_size() == 255 );	
			
			CPPUNIT_ASSERT( buff.put(sz, 100) == true );
			CPPUNIT_ASSERT( buff.size() == 100 );	
			CPPUNIT_ASSERT( buff.free_size() == 155 );	

			memset(sz, 'b', sizeof(sz));
			CPPUNIT_ASSERT( buff.put(sz, 155) == true );
			CPPUNIT_ASSERT( buff.size() == 255 );	
			CPPUNIT_ASSERT( buff.free_size() == 0 );

			// ��������ȏ㏑�����߂Ȃ��͂�
			CPPUNIT_ASSERT( buff.put(sz, 1) == false );

			// �ǂݎ��ʒu���ړ������A�ǂݎ��
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.copy(sz, 100) == 100);
			memset(sz2, 'a', sizeof(sz2));
			CPPUNIT_ASSERT( memcmp(sz, sz2, 100) == 0);
			CPPUNIT_ASSERT( buff.size() == 255 );	
			CPPUNIT_ASSERT( buff.free_size() == 0 );

			// �������񂾏��Ɏ��o��
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.get(sz, 100) == 100);
			CPPUNIT_ASSERT( memcmp(sz, sz2, 100) == 0);
			CPPUNIT_ASSERT( buff.size() == 155 );	
			CPPUNIT_ASSERT( buff.free_size() == 100 );

			memset(sz2, 'b', sizeof(sz2));
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.get(sz, 100) == 100);
			CPPUNIT_ASSERT( memcmp(sz, sz2, 100) == 0);
			CPPUNIT_ASSERT( buff.size() == 55 );	
			CPPUNIT_ASSERT( buff.free_size() == 200 );	

			// �O�Ɏc���Ă����Ԃŏ�������
			// C��50�@��������
			memset(sz, 'C', sizeof(sz));
			CPPUNIT_ASSERT( buff.put(sz, 50) == true );
			CPPUNIT_ASSERT( buff.size() == 105 );	
			CPPUNIT_ASSERT( buff.free_size() == 150 );

			// �ǂݎ��
			// b��55�@�ǂݎ��
			memset(sz2, 'b', sizeof(sz2));
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.get(sz, 55) == 55);
			CPPUNIT_ASSERT( memcmp(sz, sz2, 55) == 0);
			CPPUNIT_ASSERT( buff.size() == 50 );	
			CPPUNIT_ASSERT( buff.free_size() == 205 );	

			// �ǂݎ��
			// C��50 �ǂݎ��
			memset(sz2, 'C', sizeof(sz2));
			memset(sz, 0, sizeof(sz));
			CPPUNIT_ASSERT( buff.get(sz, 50) == 50);
			CPPUNIT_ASSERT( memcmp(sz, sz2, 50) == 0);
			CPPUNIT_ASSERT( buff.size() == 0 );	
			CPPUNIT_ASSERT( buff.free_size() == 255 );	
		}
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XBuffTest::testXRingBufferBL(void)
{
	try {
		XRingBufferBL buff;
		unsigned char szBuff[1024];
		unsigned char szBuffCheck[1024];

		CPPUNIT_ASSERT( buff.alloc(1023) == true );
		CPPUNIT_ASSERT( buff.size() == 0 );				// �������܂ꂽ�o�b�t�@�̃T�C�Y
		CPPUNIT_ASSERT( buff.free_size() == 1023-4 );	// �������݉\�ȃo�b�t�@�T�C�Y
		CPPUNIT_ASSERT( buff.cur_blk_size() == 0 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 0 );	// �J�����g�̃u���b�N�J�E���g


		// �u���b�N��������
		::memset(szBuff, 'x', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 500) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 500 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 1 );		// �J�����g�̃u���b�N�J�E���g

		// �u���b�N��������
		::memset(szBuff, 'y', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 200) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 500 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 2 );		// �J�����g�̃u���b�N�J�E���g

		// �u���b�N��������
		::memset(szBuff, 'z', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 300) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 500 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 3 );		// �J�����g�̃u���b�N�J�E���g

		// �j�������ɃJ�����g�̃u���b�N�ǂݍ���
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'x', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.copy(szBuff, sizeof(szBuff)) == 500 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 500) == 0);

		// �J�����g�̃u���b�N�ǂݍ���
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'x', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.get(szBuff, sizeof(szBuff)) == 500 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 500) == 0);
		CPPUNIT_ASSERT( buff.cur_blk_size() == 200 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 2 );		// �J�����g�̃u���b�N�J�E���g

		// �u���b�N��������
		::memset(szBuff, 'A', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 300) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 200 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 3 );		// �J�����g�̃u���b�N�J�E���g

		// �u���b�N��������.
		// �o�b�t�@Full�Ȃ̂ŁAPOP�Ŕj�����ď������܂��
		::memset(szBuff, 'B', sizeof(szBuff));
		CPPUNIT_ASSERT( buff.put(szBuff, 300) == true );
		CPPUNIT_ASSERT( buff.cur_blk_size() == 300 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 3 );		// �J�����g�̃u���b�N�J�E���g

		// �J�����g�̃u���b�N�ǂݍ���
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'z', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.get(szBuff, sizeof(szBuff)) == 300 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 300) == 0);
		CPPUNIT_ASSERT( buff.cur_blk_size() == 300 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 2 );		// �J�����g�̃u���b�N�J�E���g

		// �J�����g�̃u���b�N�ǂݍ���
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'A', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.get(szBuff, sizeof(szBuff)) == 300 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 300) == 0);
		CPPUNIT_ASSERT( buff.cur_blk_size() == 300 );		// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 1 );		// �J�����g�̃u���b�N�J�E���g

		// �J�����g�̃u���b�N�ǂݍ���
		::memset(szBuff, 0, sizeof(szBuff));
		::memset(szBuffCheck, 'B', sizeof(szBuffCheck));
		CPPUNIT_ASSERT( buff.get(szBuff, sizeof(szBuff)) == 300 );
		CPPUNIT_ASSERT( memcmp(szBuff, szBuffCheck, 300) == 0);
		CPPUNIT_ASSERT( buff.cur_blk_size() == 0 );			// �J�����g�̃u���b�N�T�C�Y
		CPPUNIT_ASSERT( buff.blk_count() == 0 );		// �J�����g�̃u���b�N�J�E���g

	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

