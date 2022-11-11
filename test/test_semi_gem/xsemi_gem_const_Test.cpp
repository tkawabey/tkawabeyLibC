#include <iostream>
#include <fstream>
#include "xsemi_gem_const_Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/semi/gem/xsemi_gem_const.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/xeception.h"

#include <windows.h>
#include <strstream>
/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
xsemi_gem_const_Test::xsemi_gem_const_Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void xsemi_gem_const_Test::setUp(void)
{
}


/** �e�X�g�㏈�� */
void xsemi_gem_const_Test::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* xsemi_gem_const_Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("xsemi_gem_const_Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_const_Test>("test_XSEMI_GemDataAtomCSV", test_XSEMI_GemDataAtomCSV) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_const_Test>("test_updateV_from_S1F12", test_updateV_from_S1F12) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_const_Test>("test_updateV_from_S2F29", test_updateV_from_S2F29) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_const_Test>("test_makeS6F11", test_makeS6F11) );


	return suite;
}

void xsemi_gem_const_Test::test_XSEMI_GemDataAtomCSV(void)
{
	try {
		const char* pStrVal;
		XD_UINT4	uval;
		XD_UINT4	ival;
		XD_FLOAT8	fval;
		bool		bVal;

		// �ȉ�����n�̃e�X�g
		XSEMI_GemDataAtomCSV obj("Key1,Key2,Key3,Key4,Key5", "TEST");
		obj.setValues("ASCII,100,-10,10.5,1");
		pStrVal = obj.getString("Key1");
		CPPUNIT_ASSERT( strcmp(pStrVal,"ASCII") == 0  );
		uval    = obj.getUint("Key2");
		CPPUNIT_ASSERT( uval == 100  );
		ival    = obj.getInt("Key3");
		CPPUNIT_ASSERT( ival == -10  );
		fval    = obj.getDouble("Key4");
		bVal    = obj.getBOOL("Key5");
		CPPUNIT_ASSERT( bVal == true  );

		pStrVal = obj.getString( 0 );
		CPPUNIT_ASSERT( strcmp(pStrVal,"ASCII") == 0  );
		uval    = obj.getUint( 1 );
		CPPUNIT_ASSERT( uval == 100  );
		ival    = obj.getInt( 2 );
		CPPUNIT_ASSERT( ival == -10  );
		fval    = obj.getDouble( 3 );
		bVal    = obj.getBOOL( 4 );
		CPPUNIT_ASSERT( bVal == true  );

		try {
			// ���݂��Ȃ��L�[���w��
			pStrVal = obj.getString("Key9");

			CPPUNIT_ASSERT( 0 );	// �X���[�����͂�
		} catch(...) {}
		try {
			// ���݂��Ȃ��C���f�b�N�X���w��
			pStrVal = obj.getString( 5 );

			CPPUNIT_ASSERT( 0 );	// �X���[�����͂�
		} catch(...) {}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}


void xsemi_gem_const_Test::test_updateV_from_S1F12(void)
{
	try {
		XSEMI_GemConst	eventMap;
		XSecsData data;
		data.put_stream(1);
		data.put_function(11);

		XRichData* pRootData = data.addChiled(XDATA_TYPE_L);
		CPPUNIT_ASSERT( pRootData );
		{
			XRichData* pSVIDRoot = pRootData->addChiled(XDATA_TYPE_L);
			CPPUNIT_ASSERT( pSVIDRoot );

			pSVIDRoot->addChiled(XDATA_TYPE_U4)->putValue((XD_UINT4)100);					// SVID
			pSVIDRoot->addChiled(XDATA_TYPE_A)->putValueString( "Test SVID NAME 001" );		// SVNAME1
			pSVIDRoot->addChiled(XDATA_TYPE_A)->putValueString( "Test UNIT 001" );			// UNITS1
		}
		{
			XRichData* pSVIDRoot = pRootData->addChiled(XDATA_TYPE_L);
			CPPUNIT_ASSERT( pSVIDRoot );

			pSVIDRoot->addChiled(XDATA_TYPE_U4)->putValue((XD_UINT4)101);					// SVID
			pSVIDRoot->addChiled(XDATA_TYPE_A)->putValueString( "Test SVID NAME 002" );		// SVNAME1
			pSVIDRoot->addChiled(XDATA_TYPE_A)->putValueString( "Test UNIT 002" );			// UNITS1
		}


//		CPPUNIT_ASSERT( eventMap.updateV_from_S1F12(data) );
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}
void xsemi_gem_const_Test::test_updateV_from_S2F29(void)
{
	try {
		XSEMI_GemConst	eventMap;
		XSecsData data;
		data.put_stream(1);
		data.put_function(11);

		XRichData* pRootData = data.addChiled(XDATA_TYPE_L);
		CPPUNIT_ASSERT( pRootData );
		{
			XRichData* pSVIDRoot = pRootData->addChiled(XDATA_TYPE_L);
			CPPUNIT_ASSERT( pSVIDRoot );

			pSVIDRoot->addChiled(XDATA_TYPE_U4)->putValue((XD_UINT4)100);					// ECID1
			pSVIDRoot->addChiled(XDATA_TYPE_A)->putValueString( "Test EC NAME 001" );		// ECNAME
			pSVIDRoot->addChiled(XDATA_TYPE_U4)->putValue((XD_UINT4)0);						// ECMIN
			pSVIDRoot->addChiled(XDATA_TYPE_U4)->putValue((XD_UINT4)1000);					// ECMAX
			pSVIDRoot->addChiled(XDATA_TYPE_U4)->putValue((XD_UINT4)50);					// ECDEF
			pSVIDRoot->addChiled(XDATA_TYPE_A)->putValueString( "Test UNIT 001" );			// UNITS
		}


//		CPPUNIT_ASSERT( eventMap.updateV_from_S2F30(data) );
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_const_Test::test_makeS6F11(void)
{
	try {
		XSEMI_GemConst	gemConst;


	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}