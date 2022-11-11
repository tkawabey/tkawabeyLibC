#include <iostream>
#include <fstream>
#include "XSecsDataTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/semi/secs/xsecs.h"
#include "xlib/xeception.h"
#include "xlib/xbuff.h"

#include <windows.h>
#include <strstream>
/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XSecsDataTest::XSecsDataTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XSecsDataTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XSecsDataTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XSecsDataTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSecsDataTest");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsDataTest>("XSecsDataTest", &testNomalAccess) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsDataTest>("XSecsDataTest", &testBuildBinary) );



	return suite;
}



void XSecsDataTest::testNomalAccess(void)
{
	XSecsData data;
	XRichData *pR;
	XRichData *pR2;
	try {
		// �w�b�_�[�̃Z�b�g
		data.put_stream(6);
		data.put_function(12);
		data.put_wait(true);
		data.put_ticket(0x11EE);
		CPPUNIT_ASSERT( data.get_stream() == 6 );
		CPPUNIT_ASSERT( data.get_function() == 12 );
		CPPUNIT_ASSERT( data.get_wait() == true );
		CPPUNIT_ASSERT( data.get_ticket() == 0x11EE );
		// �q�A�C�e���ǉ�
		pR = data.addChiled(XDATA_TYPE_L);
		CPPUNIT_ASSERT( pR );
		// �q�̐�
		CPPUNIT_ASSERT( data.getChiledCount() == 1 );
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_A)->putValueString("HOGEHOGE") );
		// �w�肵���C���f�b�N�X�̎q���擾
		pR2 = data.getChiled(0);
		CPPUNIT_ASSERT( pR == pR2 );
		CPPUNIT_ASSERT( pR->getType() == XDATA_TYPE_L  );
		CPPUNIT_ASSERT( pR->getChiledCount() == 1  );
		// �s���C���f�b�N�X�̎q���擾
		pR2 = data.getChiled(1);
		CPPUNIT_ASSERT( pR2 == NULL );
		// �w�肵���q���폜
		CPPUNIT_ASSERT( data.removeChiled(data.getChiled(0)) == 1 );		
		// �q�̐�
		CPPUNIT_ASSERT( data.getChiledCount() == 0 );

		{
			std::strstream		stream;
			CPPUNIT_ASSERT( data.buildBinaryData(stream, XSecsData::SIZE_HEAD_AND_BODY) );
		}
		{
			XInflateStreamBuff	buff(10);
			std::iostream		stream(&buff);
			CPPUNIT_ASSERT( data.buildBinaryData(stream, XSecsData::SIZE_HEAD_AND_BODY) );
		}

	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}
void XSecsDataTest::testBuildBinary(void)
{
	XSecsData data;
	XRichData *pR;
	try {
		// �w�b�_�[�̃Z�b�g
		data.put_stream(6);
		data.put_function(12);
		data.put_wait(true);
		data.put_ticket(0x11EE);
		CPPUNIT_ASSERT( data.get_stream() == 6 );
		CPPUNIT_ASSERT( data.get_function() == 12 );
		CPPUNIT_ASSERT( data.get_wait() == true );
		CPPUNIT_ASSERT( data.get_ticket() == 0x11EE );
		// �q�A�C�e���ǉ�
		pR = data.addChiled(XDATA_TYPE_L);
		CPPUNIT_ASSERT( pR );
		// �q�̐�
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_BOOL)->putValue(true) );
		pR = pR->addChiled(XDATA_TYPE_L);
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_A)->putValueString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") );
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_J)->putValueString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") );
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_I1)->putValue((XD_INT1)0xFF) );
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_I2)->putValue((XD_INT2)0xFFFF) );
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_I4)->putValue((XD_INT4)0xFFFFFFFF) );		
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_I8)->putValue((XD_INT8)0xFFFFFFFFFFFFFFFF) );	
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_U1)->putValue((XD_UINT1)0xFF) );
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_U2)->putValue((XD_UINT2)0xFFFF) );
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_U4)->putValue((XD_UINT4)0xFFFFFFFF) );		
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_U8)->putValue((XD_UINT8)0xFFFFFFFFFFFFFFFF) );	
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_F4)->putValue((XD_FLOAT4)1.23456789) );		
		CPPUNIT_ASSERT( pR->addChiled(XDATA_TYPE_F8)->putValue((XD_FLOAT8)1.23456789) );	
		
		{
			// �o�C�i���[�f�[�^�쐬
			XInflateStreamBuff	buff;
			std::iostream		stream(&buff);
			CPPUNIT_ASSERT( data.buildBinaryData(stream, XSecsData::SIZE_HEAD_AND_BODY) );


			XSecsData data2;
			CPPUNIT_ASSERT( data2.setFromBinary(buff.getBuff(), buff.size(), XSecsData::SIZE_HEAD_AND_BODY) );

			{
				// �o�C�i���[�o��
				std::fstream fst;
				fst.open("XSecsDataTest_testBuildBinary.bin.unitest", std::ios::out);
				CPPUNIT_ASSERT( data2.buildBinaryData(fst, XSecsData::HEAD_AND_BODY) );
				fst.close();
			}
			{
				// SML �\��
#ifdef _UNICODE
				std::wfstream fst;
#else
				std::fstream fst;
#endif
				fst.open(_T("XSecsDataTest_testBuildBinary.sml.unitest"), std::ios::out);
				data2.print( fst );
				fst.close();
			}
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}



