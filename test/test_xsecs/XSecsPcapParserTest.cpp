#include <iostream>
#include <fstream>
#include "XSecsPcapParserTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/semi/secs/xsecs_pcap_parser.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/xeception.h"
#include "xlib/xbuff.h"

/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XSecsPcapParserTest::XSecsPcapParserTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XSecsPcapParserTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XSecsPcapParserTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XSecsPcapParserTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSecsPcapParserTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsPcapParserTest>("XSecsPcapParserTest", &testNomalAccess) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsPcapParserTest>("XSecsPcapParserTest", &testReTransmited_Packet) );
	
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsPcapParserTest>("XSecsPcapParserTest", &testSACKMessageInclude) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsPcapParserTest>("XSecsPcapParserTest", &testLoadSerialize) );


	return suite;
}



void normalCallback(XSecsPcapData* pPcapData,void* pUserDara)
{
	XSecsPcapParserTest *pUser = (XSecsPcapParserTest*)pUserDara;
	if( pUser->m_fst.is_open() != 0 ) {
		pPcapData->print(pUser->m_fst, 2);
	}

	if( pPcapData->m_length > 1000 && pUser->m_serialize.is_open() != 0 ) {
		pPcapData->serializeStore(pUser->m_serialize);
	}
}

void XSecsPcapParserTest::testNomalAccess(void)
{
	try {
		CPPUNIT_ASSERT( (int)sizeof(XSECSHeader) == 10 );
		// �C���X�^���X�쐬
		XSecsPcapParser* parser = XSecsPcapParser::newInstance();
		CPPUNIT_ASSERT( parser );
		XSecsData data;

		m_fst.open("XSecsPcapParserTest_testNomalAccess.SML.unitest", std::ios::out);

		// �R�[���o�b�N�֐����Z�b�g
		parser->setCallBackFunction(normalCallback, this);
		// �o�C�i���[���O�����
		CPPUNIT_ASSERT( parser->parse("MULTI_PACKET_2005110413433340.blog") );
//		CPPUNIT_ASSERT( parser->parse("OcswCommHSMS_2006041017113228.blog") );
//		CPPUNIT_ASSERT( parser->parse("OcswCommHSMS_2006020923172976.blog") );
		XSecsPcapParser::deleteInstance(&parser);

		m_fst.close();

	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}

}

void XSecsPcapParserTest::testReTransmited_Packet()
{
	try {
		CPPUNIT_ASSERT( (int)sizeof(XSECSHeader) == 10 );
		// �C���X�^���X�쐬
		XSecsPcapParser* parser = XSecsPcapParser::newInstance();
		CPPUNIT_ASSERT( parser );
		XSecsData data;

		m_fst.open("XSecsPcapParserTest_testNomalAccess.SML.unitest", std::ios::out);

		// �R�[���o�b�N�֐����Z�b�g
		parser->setCallBackFunction(normalCallback, this);
		// �o�C�i���[���O�����
		CPPUNIT_ASSERT( parser->parse("ReTransmited_Packet.blog") );
		XSecsPcapParser::deleteInstance(&parser);

		m_fst.close();

	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}

}

void XSecsPcapParserTest::testSACKMessageInclude()
{
	try {
		CPPUNIT_ASSERT( (int)sizeof(XSECSHeader) == 10 );
		// �C���X�^���X�쐬
		XSecsPcapParser* parser = XSecsPcapParser::newInstance();
		CPPUNIT_ASSERT( parser );
		XSecsData data;

		m_fst.open("XSecsPcapParserTest_testSACKMessageInclude.SML.unitest", std::ios::out);

		// �R�[���o�b�N�֐����Z�b�g
		parser->setCallBackFunction(normalCallback, this);
		// �o�C�i���[���O�����
		CPPUNIT_ASSERT( parser->parse("Include_SACK_Message.blog") );

		XSecsPcapParser::deleteInstance(&parser);

		m_fst.close();

	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}

}

void testLoadSerialize_Callback(XSecsPcapData* pPcapData,void* pUserDara)
{
	XSecsPcapParserTest *pUser = (XSecsPcapParserTest*)pUserDara;
	pPcapData->print(pUser->m_fst, 2);
}

void XSecsPcapParserTest::testLoadSerialize()
{
	try {
		CPPUNIT_ASSERT( (int)sizeof(XSECSHeader) == 10 );
		// �C���X�^���X�쐬
		XSecsPcapParser* parser = XSecsPcapParser::newInstance();
		CPPUNIT_ASSERT( parser );
		// �R�[���o�b�N�֐����Z�b�g
		parser->setCallBackFunction(normalCallback, this);

		const char *pSerialize_file = "XSecsPcapParserTest_Serialize.data";


		//
		//	�V���A���C�Y�f�[�^���쐬
		//
		m_serialize.open(pSerialize_file, std::ios::out | std::ios::binary);
		// �o�C�i���[���O�����
		CPPUNIT_ASSERT( parser->parse("Include_SACK_Message.blog") );
		m_serialize.close();

		//
		//	�V���A���C�Y�f�[�^��ǂݍ���
		//
		m_fst.open("XSecsPcapParserTest_testLoadSerialize.SML.unitest", std::ios::out);
		// �o�C�i���[���O�����
		CPPUNIT_ASSERT( parser->serializeLoad(pSerialize_file) );
		m_fst.close();




		XSecsPcapParser::deleteInstance(&parser);
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}


