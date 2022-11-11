#include <iostream>
#include <fstream>
#include "XStrBuffTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xeception.h"
#include "xlib/xstrbuff.h"
#include "xlib/xmap.h"

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
XStrBuffTest::XStrBuffTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XStrBuffTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XStrBuffTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XStrBuffTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XStrBuffTest");
	
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testXStrBuffSplit) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testConstructor) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testSet) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorEq) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorCast) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorComp) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorGetIndexAcc) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorSetIndexAcc) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorAdd) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testOperatorIns) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testCount) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testFormat) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XStrBuffTest>("XStrBuffTest", &testUtilFunctions) );



	return suite;
}


void XStrBuffTest::testConstructor(void)
{
	try {
		// �����A�]���o�b�t�@�̑����T�C�Y
		XStrBuff d(120);
		CPPUNIT_ASSERT_EQUAL((size_t)120, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByte�ŏ������B���̑��̓f�t�H���g
		XStrBuff d("0123456789");
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByte�ŏ������B�T�C�Y���w��B���̑��̓f�t�H���g
		XStrBuff d("0123456789", 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "012") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByte�ŏ������B�T�C�Y���w��B�����A�]���o�b�t�@�̑����T�C�Y���w��B
		XStrBuff d("0123456789", 4, 10);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123") );
		CPPUNIT_ASSERT_EQUAL((size_t)10, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByte�ŏ������B�����A�]���o�b�t�@�̑����T�C�Y���w��B�T�C�Y�̓f�t�H���g
		XStrBuff d("0123456789", -1, 10);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)10, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByte�ŏ������B���ۂ̃T�C�Y�ȏ�̃T�C�Y���w��B
		XStrBuff d("0123456789", 500);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	
	try {
		// Unicode�ŏ������B���̑��̓f�t�H���g
		XStrBuff d(L"0123456789");

		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicode�ŏ������B�T�C�Y���w��B���̑��̓f�t�H���g
		XStrBuff d(L"0123456789", 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"012") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicode�ŏ������B�T�C�Y���w��B�����A�]���o�b�t�@�̑����T�C�Y���w��B
		XStrBuff d(L"0123456789", 4, 10);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123") );
		CPPUNIT_ASSERT_EQUAL((size_t)10, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicode�ŏ������B�����A�]���o�b�t�@�̑����T�C�Y���w��B�T�C�Y�̓f�t�H���g
		XStrBuff d(L"0123456789", -1, 10);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)10, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicode�ŏ������B���ۂ̃T�C�Y�ȏ�̃T�C�Y���w��B
		XStrBuff d(L"0123456789", 500);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	
	try {
		// XStrBuff�ŏ������B���̑��̓f�t�H���g
		XStrBuff src(_T("0123456789"));
		XStrBuff d(src);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// XStrBuff�ŏ������B�]���o�b�t�@�̑����T�C�Y���w��
		XStrBuff src(_T("0123456789"));
		XStrBuff d(src, 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)3, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testSet(void)
{
	try {
		// MuitiByte�ŃZ�b�g�B���̑��̓f�t�H���g
		XStrBuff d;
		d.set("0123456789");
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByte�ŃZ�b�g�B�T�C�Y���w��
		XStrBuff d;
		d.set("0123456789", 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "012") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// MuitiByte�ŃZ�b�g�B���ۂ̃T�C�Y�ȏ�̃T�C�Y���w��B
		XStrBuff d;
		d.set("0123456789", 500);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(d, "0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	
	try {
		// Unicode�ŃZ�b�g�B���̑��̓f�t�H���g
		XStrBuff d;
		d.set(L"0123456789");
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicode�ŃZ�b�g�B�T�C�Y���w��B
		XStrBuff d;
		d.set(L"0123456789", 3);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"012") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

	try {
		// Unicode�ŃZ�b�g�B���ۂ̃T�C�Y�ȏ�̃T�C�Y���w��B
		XStrBuff d;
		d.set(L"0123456789", 500);
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(d, L"0123456789") );
		CPPUNIT_ASSERT_EQUAL((size_t)XSTRBUFF_DEF_INFLATE_SIZE, d.getInflateSize() );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorEq()
{

	try {
		// MultiByte�ő���B
		XStrBuff d;
		d = "����������";
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::StrCmp(d, _T("����������")) );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// Unicode�ő���B
		XStrBuff d;
		d = L"����������";
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::StrCmp(d, _T("����������")) );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// XStrBuff�ő���B
		XStrBuff src(_T("����������"));
		XStrBuff d;
		d = src;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::StrCmp(d, _T("����������")) );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}

}
void XStrBuffTest::testOperatorCast(void)
{
	try {
		// char*�ɕϊ�
		XStrBuff d(_T("����������"));
		char* p_addr = d;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(p_addr, "����������") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// const char*�ɕϊ�
		XStrBuff d(_T("����������"));
		const char* p_addr = d;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::strcmp(p_addr, "����������") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// char*�ɕϊ�
		XStrBuff d(_T("����������"));
		wchar_t* p_addr = d;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(p_addr, L"����������") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
	try {
		// const char*�ɕϊ�
		XStrBuff d(_T("����������"));
		const wchar_t* p_addr = d;
		CPPUNIT_ASSERT_EQUAL((int)0, XStrBuff::wcscmp(p_addr, L"����������") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorComp(void)
{
	try {
		// const char*�ɕϊ�
		XStrBuff d(_T("����������"));
		const wchar_t* p_addr = d;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("����������") );
		CPPUNIT_ASSERT_EQUAL(false, d == _T("����������") );
		CPPUNIT_ASSERT_EQUAL(false, d != _T("����������") );
		CPPUNIT_ASSERT_EQUAL(true,  d != _T("����������") );
	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorGetIndexAcc(void)
{
	try {
		XStrBuff d(_T("0123456789"));
		TCHAR c = 0;

		
		c = d[0];
		CPPUNIT_ASSERT_EQUAL((int)_T('0'),  (int)c );
		c = d.GetAt(0);
		CPPUNIT_ASSERT_EQUAL((int)_T('0'),  (int)c );

		c = d[2];
		CPPUNIT_ASSERT_EQUAL((int)_T('2'),  (int)c );
		c = d.GetAt(2);
		CPPUNIT_ASSERT_EQUAL((int)_T('2'),  (int)c );

		
		c = d[9];
		CPPUNIT_ASSERT_EQUAL((int)_T('9'),  (int)c );
		c = d.GetAt(9);
		CPPUNIT_ASSERT_EQUAL((int)_T('9'),  (int)c );
		
		// �w��C���f�b�N�X�I�[�o�[�B��O����������B
		try {
			c = d[10];
			CPPUNIT_ASSERT( false );
		} catch(XExcept &ex) {
			CPPUNIT_ASSERT_EQUAL(ex.getCategory(), XExcept_OVER_INDEX);
		}
		try {
			c = d.GetAt(10);
			CPPUNIT_ASSERT( false );
		} catch(XExcept &ex) {
			CPPUNIT_ASSERT_EQUAL(ex.getCategory(), XExcept_OVER_INDEX);
		}
		


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorSetIndexAcc(void)
{
	try {
		XStrBuff d(_T("0123456789"));
		
		d[0] = _T('A');
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A123456789") );
		d.SetAt(1, _T('B'));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("AB23456789") );

		d[2] = _T('C');
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("ABC3456789") );
		d.SetAt(3, _T('D'));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("ABCD456789") );

		
		d[8] = _T('X');
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("ABCD4567X9") );
		d.SetAt(9, _T('Y'));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("ABCD4567XY") );
		
		// �w��C���f�b�N�X�I�[�o�[�B��O����������B
		try {
			d[10] = _T('Z');
			CPPUNIT_ASSERT( false );
		} catch(XExcept &ex) {
			CPPUNIT_ASSERT_EQUAL(ex.getCategory(), XExcept_OVER_INDEX);
		}
		try {
			d.SetAt(10, _T('Z'));
			CPPUNIT_ASSERT( false );
		} catch(XExcept &ex) {
			CPPUNIT_ASSERT_EQUAL(ex.getCategory(), XExcept_OVER_INDEX);
		}
		


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testOperatorAdd(void)
{
	try {
		XStrBuff d(_T("0123456789"), 10);

		// MultiByte�ő���
		d += "ABC";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("0123456789ABC") );
		// UNICODE�ő���
		d += L"EFG";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("0123456789ABCEFG") );
		
		// MultiByte�ő���(���C�h����)
		d += "������";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("0123456789ABCEFG������") );
		// UNICODE�ő���(���C�h����)
		d += L"������";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("0123456789ABCEFG������������") );

		


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XStrBuffTest::testOperatorIns(void)
{
	try {
		XStrBuff d(_T("0123456789"), 10);

		// short(�v���X�l)����
		d << (short)30000;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000") );

		// short(�v���X�l)����
		d << (short)-30001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-30001") );
		
		
		// unsigned short����
		d << (unsigned short)60006;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-3000160006") );

		
		// int(�v���X�l)����
		d << (int)40000;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-300016000640000") );

		// short(�v���X�l)����
		d << (int)-40001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-300016000640000-40001") );

		// unsigned int����
		d << (unsigned int)100001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("012345678930000-300016000640000-40001100001") );


		d = _T("A");
		
		
		// long(�v���X�l)����
		d << (long)40000;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000") );

		// long(�v���X�l)����
		d << (long)-40001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000-40001") );

		// unsigned long����
		d << (unsigned long)100001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000-40001100001") );
		
		d = _T("A");
		
		// XD_INT8(�v���X�l)����
		d << (XD_INT8)40000;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000") );

		// XD_INT8(�v���X�l)����
		d << (XD_INT8)-40001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000-40001") );

		// XD_UINT8����
		d << (XD_UINT8)100001;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A40000-40001100001") );
		
		d = _T("A");
		// float����
		d << (float)0.1;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-001") );
		d = _T("A");
		// double����
		d << (double)0.01;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-002") );

		// MultiByte��������
		d << "����������";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-002����������") );
		// Unicode��������
		d << "����������";
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-002��������������������") );
		// XStrBuff����
		XStrBuff src(_T("����������"));
		d << src;
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("A1.000000e-002������������������������������") );

	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XStrBuffTest::testCount(void)
{
	try {
		static int INFLATE_SIZE = 10;
		XStrBuff d(INFLATE_SIZE);
		// 15�����̃o�b�t�@���m��
		CPPUNIT_ASSERT( d.Alloc(15 * sizeof(TCHAR)) );
		// 10�������
		d = _T("0123456789");
		CPPUNIT_ASSERT_EQUAL((size_t)10,  d.getLength() );
		CPPUNIT_ASSERT_EQUAL((size_t)15,  d.getCountOfBuff() );
		CPPUNIT_ASSERT_EQUAL((size_t)(15 * sizeof(TCHAR)),  d.GetBuffSize() );
		CPPUNIT_ASSERT_EQUAL((size_t)5,  d.getCountOfRestBuff() );
		
		// �X�� 2�����ǉ���� ���v12�����@�����m�ۂ�14����(NULL�܂܂��j�ɓ��B���Ă��Ȃ��B�������[��Realooc���ĂȂ��B
		d << _T("01");
		CPPUNIT_ASSERT_EQUAL((size_t)12,  d.getLength() );
		// �X�� 2�����ǉ���� ���v14�����@�����m�ۂ�14����(NULL�܂܂��j�Ɉ�v�B�������[��Realooc���ĂȂ��B
		d << _T("23");
		CPPUNIT_ASSERT_EQUAL((size_t)14,  d.getLength() );
		// �X�� 2�����ǉ���� ���v16�����@�����m�ۂ�14����(NULL�܂܂��j���I�[�o�[�B�������[��Realooc����B
		d << _T("45");
		CPPUNIT_ASSERT_EQUAL((size_t)16,  d.getLength() );


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testFormat(void)
{
	try {
		XStrBuff d;

		d.Format(_T("##%u,%s"), 10, _T("����������"));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("##10,����������") );

		d = XStrBuff::FormatS(_T("##%u,%s"), 10, _T("����������"));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("##10,����������") );

	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}
void XStrBuffTest::testUtilFunctions(void)
{
	try {
		XStrBuff d(_T(" abcdefg���������� "));
		int iFind = 0;

		d.MakeUpper();
		CPPUNIT_ASSERT_EQUAL(true,  d == _T(" ABCDEFG���������� ") );
		
		d.MakeLower();
		CPPUNIT_ASSERT_EQUAL(true,  d == _T(" abcdefg���������� ") );

		d.Trim(XStrBuff::TRIM_RIGHTONLY);
		CPPUNIT_ASSERT_EQUAL(true,  d == _T(" abcdefg����������") );
		
		d.Trim(XStrBuff::TRIM_LEFTONLY);
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("abcdefg����������") );

		d = _T(" abcdefg����������abc ");
		d.Trim(XStrBuff::TRIM_BOTH);
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("abcdefg����������abc") );

		
		d.Remove(_T('a'));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("bcdefg����������bc") );

		d.Replace(_T("bc"), _T("����������"));
		CPPUNIT_ASSERT_EQUAL(true,  d == _T("����������defg��������������������") );

		iFind = d.Find(_T("��"));
		CPPUNIT_ASSERT_EQUAL(true,  iFind != -1);


	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

void XStrBuffTest::testXStrBuffSplit(void)
{
	try {
		XStrBuffSplit split;

		split = _T("a01,b02,c03");
		CPPUNIT_ASSERT_EQUAL(true,  split.GetArgCouunt() == 3);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("a01"), split.GetArgv()[0]) == 0);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("b02"), split.GetArgv()[1]) == 0);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("c03"), split.GetArgv()[2]) == 0);

		split = _T("a01,\"b0,2\",c03");
		CPPUNIT_ASSERT_EQUAL(true,  split.GetArgCouunt() == 3);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("a01"), split.GetArgv()[0]) == 0);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("b0,2"), split.GetArgv()[1]) == 0);
		CPPUNIT_ASSERT_EQUAL(true,  XStrBuff::StrCmp(_T("c03"), split.GetArgv()[2]) == 0);

	} catch(XExcept &e) {
		e.print( XLIB_TEST_STD_ERR);
		CPPUNIT_ASSERT( 0  );
	}
}

