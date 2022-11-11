#include <iostream>
#include <fstream>
#include "XJSonTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/xdata.h"
#include "xlib/xbuff.h"
#include "xlib/xeception.h"
#include "xlib/json/xjson_array.h"
#include "xlib/json/xjson_obj.h"
#include "xlib/json/xjson_val.h"


#include <windows.h>
#ifdef _UNICODE
	#define XLIB_TEST_STD_ERR	std::wcerr
#else
	#define XLIB_TEST_STD_ERR	std::cerr
#endif



class MyXJsonValErrorReport : public XJsonValErrorReport
{
public:
	MyXJsonValErrorReport(){}
	virtual void error(const TCHAR* p_msg, size_t nCharPos, size_t nLineNumber, size_t nLinePos);
};

void MyXJsonValErrorReport::error(const TCHAR* p_msg, size_t nCharPos, size_t nLineNumber, size_t nLinePos)
{
	XLIB_TEST_STD_ERR << p_msg << std::endl;
	XLIB_TEST_STD_ERR << _T("  Char Pos      : ") << nCharPos << std::endl;
	XLIB_TEST_STD_ERR << _T("  Line Number   : ") << nLineNumber << std::endl;
	XLIB_TEST_STD_ERR << _T("  Line Char Pos : ") << nLinePos << std::endl;
}




/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XJSonTest::XJSonTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XJSonTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XJSonTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XJSonTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XJSonTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonString) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonObject) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonArray) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonNumetric) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testParseJsonPrimitive) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XJSonTest>("XJSonTest", &testXBuff) );
	
	return suite;
}


void XJSonTest::testParseJsonString(void)
{
	// �S��ASCII�ȕ������擾
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("\"abcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("abcd") );
	}
	// �N�H�[�e�[�V�����̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		p_ret = v.loadFromString(_T("\"ab\\\"cd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\"cd") );
	}
	// �o�b�N�X���b�V���̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		p_ret = v.loadFromString(_T("\"ab\\\\cd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\\cd") );
	}
	// �X���b�V���̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("\"ab\\/cd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab/cd") );
	}
	// �o�b�N�X�y�[�X�̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		XLIB_TEST_STD_ERR << _T("\"ab\\bcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\bcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\bcd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// �t�����t�B�[�h�̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		XLIB_TEST_STD_ERR << _T("\"ab\\fcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\fcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\fcd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// ���C���t�B�[�h�̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		XLIB_TEST_STD_ERR << _T("\"ab\\ncd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\ncd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\ncd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// �L�����b�W���^�[���̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		XLIB_TEST_STD_ERR << _T("\"ab\\rcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\rcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\rcd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// TAB�̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"ab\\tcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\tcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("ab\tcd") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}
	// UNICODE�̃G�X�P�[�v�������܂܂�镶���̉��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"\\u5024\\u4F0D\"") << std::endl;
		p_ret = v.loadFromString(_T("\"\\u5024\\u4F0D\""), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isString() );
		CPPUNIT_ASSERT_EQUAL(true, v.getString() == _T("�l��") );
		XLIB_TEST_STD_ERR << v.getString().c_str() << std::endl;
	}



	// ������̉�͂ŁA�Ō��"�������B
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("\"abcd"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}

	// UNICODE�̃G�X�P�[�v�����ŁA�f�[�^������Ȃ�
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"\\u502") << std::endl;
		p_ret = v.loadFromString(_T("\"\\u502"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}

	// UNICODE�̃G�X�P�[�v�����ŁA16�i���ȊO�̕��������悤����Ă���B
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"\\u5G24\\u4F0D\"") << std::endl;
		p_ret = v.loadFromString(_T("\"\\u5G24\\u4F0D\""), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}

	//�G�X�P�[�v�Ώۂ��s��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		XLIB_TEST_STD_ERR << _T("\"ab\\Gcd\"") << std::endl;
		p_ret = v.loadFromString(_T("\"ab\\Gcd\""), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
}

void XJSonTest::testParseJsonNumetric(void)
{
	//�v���X�̐����l
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("123456789"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isInteger() );
		CPPUNIT_ASSERT_EQUAL(true, v.getInteger() == 123456789 );
	}
	//�}�C�i�X�̐����l
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("-123456789"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isInteger() );
		CPPUNIT_ASSERT_EQUAL(true, v.getInteger() == -123456789 );
	}
	//�v���X�̐����l
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("12345678.9"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isDouble() );
		CPPUNIT_ASSERT_EQUAL(true, v.getDouble() == 12345678.9 );
	}
	//�}�C�i�X�̐����l
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("-12345678.9"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isDouble() );
		CPPUNIT_ASSERT_EQUAL(true, v.getDouble() == -12345678.9 );
	}
	//�����P
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.23e-003"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isDouble() );		
		CPPUNIT_ASSERT_EQUAL(true, v.getDouble() == 0.00123 );
	}
	//����2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.23e+003"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isDouble() );		
		CPPUNIT_ASSERT_EQUAL(true, v.getDouble() == 1230.0 );
	}


	
	//�擪����0��������2�o��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("00"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//����2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.23e003"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//����2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.23e+003-003"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//����2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("1.2300.3"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//����2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("10-003"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//����2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("ABC"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	//����2
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("10ABC"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}


	

}
void XJSonTest::testParseJsonPrimitive(void)
{
	//true
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("true"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == true );
	}
	//TRUE
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("TRUE"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == true );
	}
	//TrUe
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("TrUe"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == true );
	}
	
	//false
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("false"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == false );
	}
	//FALSE
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("FALSE"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == false );
	}
	//FaLsE
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("FaLsE"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isBoolean() );
		CPPUNIT_ASSERT_EQUAL(true, v.getBoolean() == false );
	}


	//null
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("null"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isNull() );
	}
	//NULL
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("NULL"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isNull() );
	}
	//NuLl
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;
		
		p_ret = v.loadFromString(_T("NuLl"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isNull() );
	}
}

void XJSonTest::testParseJsonObject(void)
{
	// �v���p�e�B�̖����I�u�W�F�N�g
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  { \t\r\n   }  \t\t\t\t\t\t\t\r\n\r\n "), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isObject() );
		CPPUNIT_ASSERT_EQUAL(true, v.getObject().size() == 0 );
	}
	// �v���p�e�B��1���I�u�W�F�N�g
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \"aaa\"  \t : \t\r\n 123 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isObject() );
		CPPUNIT_ASSERT_EQUAL(true, v.getObject().size() == 1 );
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].isInteger() );		
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].getInteger() == 123 );
	}
	// �v���p�e�B���Q���ȏ�I�u�W�F�N�g
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \"aaa\"  \t : \t\r\n 123 \t\r\n, \"bbb\"  \t : \t\r\n 456 \t\r\n, \"ccc\"  \t : \t\r\n 789 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isObject() );
		CPPUNIT_ASSERT_EQUAL(true, v.getObject().size() == 3 );
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].isInteger() );		
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].getInteger() == 123 );
		CPPUNIT_ASSERT_EQUAL(true, v[_T("bbb")].isInteger() );		
		CPPUNIT_ASSERT_EQUAL(true, v[_T("bbb")].getInteger() == 456 );
		CPPUNIT_ASSERT_EQUAL(true, v[_T("aaa")].isInteger() );		
		CPPUNIT_ASSERT_EQUAL(true, v[_T("ccc")].getInteger() == 789 );
	}
	// �v���p�e�B�[�����s��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \t\r\n aaa  \t : \t\r\n 123 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	// �v���p�e�B�̖��O�ƒl�̋�؂蕶�����R�����ł͂Ȃ��B
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \"aaa\"  \t ; \t\r\n 123 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	// �����v���p�e�B�̋�؂蕶�����J���}�łȂ��B
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("{ \"aaa\"  \t : \t\r\n 123 \t\r\n @ \"bbb\"  \t : \t\r\n 456 \t\r\n, \"ccc\"  \t : \t\r\n 789 \t\r\n   }"), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
}
void XJSonTest::testParseJsonArray(void)
{
	// ��̔z���`
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n   ]  "), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isArray() );
		CPPUNIT_ASSERT_EQUAL(true, v.getArray().size() == 0 );
	}
	// �z��ɂP���̃f�[�^
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n123   ]  "), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isArray() );
		CPPUNIT_ASSERT_EQUAL(true, v.getArray().size() == 1 );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)0].isInteger() );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)0].getInteger() == 123 );
	}
	// �z���2���ȏ�̃f�[�^
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n123, \"ABC\", 1.23   ]  "), &errR );
		CPPUNIT_ASSERT_EQUAL(true, p_ret != NULL );
		CPPUNIT_ASSERT_EQUAL(true, v.isArray() );
		CPPUNIT_ASSERT_EQUAL(true, v.getArray().size() == 3 );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)0].isInteger() );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)0].getInteger() == 123 );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)1].isString() );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)1].getString() == _T("ABC") );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)2].isDouble() );
		CPPUNIT_ASSERT_EQUAL(true, v[(size_t)2].getDouble() == 1.23 );
	}
	
	// �z��̊���̕���������
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n123     "), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
	
	// �z��̋�؂蕶�����s��
	{
		MyXJsonValErrorReport	errR;
		XJsonVal v;
		const TCHAR* p_ret = NULL;

		p_ret = v.loadFromString(_T("  [ \t\r\n123 @ 123 ]    "), &errR );
		CPPUNIT_ASSERT_EQUAL(false, p_ret != NULL );
	}
}
void XJSonTest::testXBuff(void)
{
	XJsonObj obj;
	XJsonArray ar;
	XJsonArray ar2;

	obj.add(_T("key1"), XJsonVal((int)10));
	obj.add(_T("�L�[2"), XJsonVal(_T("�l�P")));
	obj.add(_T("key3"), XJsonVal(10.123456));
	obj.add(_T("key4"), XJsonVal(true));
	obj.add(_T("key5"), XJsonVal(false));
	obj.add(_T("key6"), XJsonVal());

	XJsonObj obj2;
	obj2.add(_T("sub_key1"), XJsonVal((int)10));
	obj2.add(_T("�T�u�L�[2alphabet"), XJsonVal(_T("�l�Palphabet")));
	obj2.add(_T("sub_key3"), XJsonVal(10.123456));
	obj2.add(_T("sub_key4"), XJsonVal(true));
	obj2.add(_T("sub_key5"), XJsonVal(false));
	obj2.add(_T("sub_key6"), XJsonVal());
	ar2.push_back(XJsonVal((int)9999));
	ar2.push_back(XJsonVal(_T("�l���O")));
	ar2.push_back(XJsonVal(25.123456));
	ar2.push_back(XJsonVal(true));
	ar2.push_back(XJsonVal(false));
	ar2.push_back(XJsonVal());
	obj2.add(_T("sub_key7"), XJsonVal(ar2));





	ar.push_back(XJsonVal((int)10));
	ar.push_back(XJsonVal(_T("�l��")));
	ar.push_back(XJsonVal(20.123456));
	ar.push_back(XJsonVal(true));
	ar.push_back(XJsonVal(false));
	ar.push_back(XJsonVal());
	obj.add(_T("key7"), XJsonVal(ar));
	obj.add(_T("key8"), XJsonVal(obj2));


	
	obj[_T("key9")] = XJsonVal(456);
	obj[_T("key10")] = XJsonVal("ascii");
	obj[_T("key11")] = XJsonVal(L"unicode");

	XLIB_TEST_STD_ERR << _T("�\�z�����f�[�^�̏o��:") << std::endl;
	obj.print(XLIB_TEST_STD_ERR);


	XLIB_TEST_STD_ERR << std::endl;
	XLIB_TEST_STD_ERR << std::endl;

	XJsonVal v;
//	const TCHAR* p = v.loadFromString(_T("{\"city\":\"ota\",\"cityLatLong\":{\"lat\":35.569999,\"long\":139.799999},\"country\":\"JP\",\"region\":\"13\"}"));
	const TCHAR* p = v.loadFromString(_T("{\"\\u5024\\u4F0D\":10,\"key10\":\"ascii\",\"key11\":\"unicode\",\"key3\":10.1235,\"key4\":true,\"key5\":false,\"key6\":null,\"key7\":[10,\"\u5024\u4F0D\",20.1235,true,false,null],\"key8\":{\"sub_key1\":10,\"sub_key3\":10.1235,\"sub_key4\":true,\"sub_key5\":false,\"sub_key6\":null,\"sub_key7\":[9999,\"\u5024\u30ED\u30B0\",25.1235,true,false,null],\"\u30B5\u30D6\u30AD\u30FC2alphabet\":\"\u5024\uFF11alphabet\"},\"key9\":456,\"\u30AD\u30FC2\":\"\u5024\uFF11\"}"));
	XLIB_TEST_STD_ERR << _T("JSonString����͂������e�̏o��:") << std::endl;
	v.print(XLIB_TEST_STD_ERR, 0);
	XLIB_TEST_STD_ERR << std::endl;
	XLIB_TEST_STD_ERR << std::endl;

	if( v.isObject() ) {
		const XJsonObj& wkObj =  v.getObject();
		XJsonObj::Iterator ite = wkObj.getIterator();
		while( !ite.isEnd() ) {
			XLIB_TEST_STD_ERR << ite.getKey().c_str() << _T(" => ");
			ite.getValue().print(XLIB_TEST_STD_ERR, ~0, true);
			XLIB_TEST_STD_ERR << std::endl;
			ite++;
		}
	}
}
