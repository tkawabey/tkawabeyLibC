#include <iostream>
#include <fstream>
#include "xsemi_alarm_Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/semi/gem/xsemi_alarm.h"
#include "xlib/xeception.h"
#include <strstream>
/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XSEMI_ALARM_Test::XSEMI_ALARM_Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XSEMI_ALARM_Test::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XSEMI_ALARM_Test::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XSEMI_ALARM_Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSEMI_ALARM_Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_ALARM_Test>("atom_access", atom_access) );

	return suite;
}

void XSEMI_ALARM_Test::atom_access()
{
	try {
		XSEMI_ALARM	obj(5000, _T("ALARM_NAME"));
		int cnt_atom = obj.getColmnCount();
		int i;

		obj.set_ALCD(0xFF);
		obj.set_Text(_T("Alarm Text"));
		obj.set_Comment(_T("Comment"));
		obj.set_OnCEID(123);
		obj.set_OffCEID(456);

		try {
			// �J�������̎擾
			// �G���[����������X���[����
			for(i = 0; i < cnt_atom; i++) {
				obj.getColmnName( i );
			}
		} catch(XExcept &e) {
			e.printStackTrace();
			CPPUNIT_ASSERT( 0 );
			return ;
		}


		CPPUNIT_ASSERT( obj.getColmnCount() == (XSEMI_ALARM::ATOM_POS_ENABLE+1));


		// ALID
		CPPUNIT_ASSERT( obj.getUint(XSEMI_DATA_CLMN_ALID) == 5000 );
		// AlarmName
		CPPUNIT_ASSERT( strcmp(obj.getString(XSEMI_DATA_CLMN_AlarmName), "ALARM_NAME") == 0 );
		// ALCD
		CPPUNIT_ASSERT( obj.getUint(XSEMI_DATA_CLMN_ALCD) == 0xFF );
		// Comment
		CPPUNIT_ASSERT( strcmp(obj.getString(XSEMI_DATA_CLMN_AlarmText), "Alarm Text") == 0 );
		// Comment
		CPPUNIT_ASSERT( strcmp(obj.getString(XSEMI_DATA_CLMN_Comment), "Comment") == 0 );
		// ALCD
		CPPUNIT_ASSERT( obj.getUint(XSEMI_DATA_CLMN_OnCEID) == 123 );
		// ALCD
		CPPUNIT_ASSERT( obj.getUint(XSEMI_DATA_CLMN_OffCEID) == 456 );
		// ObjectType
		CPPUNIT_ASSERT( obj.getUint( XSEMI_DATA_CLMN_ObjectType ) == XSEMI_GemConstObject::ALARM );
		CPPUNIT_ASSERT( obj.getUint( 99 ) == XSEMI_GemConstObject::ALARM );


		try {
			// �C���f�b�N�X�I�[�o�[�t���[�A�N�Z�X
			obj.getColmnName( XSEMI_ALARM::ATOM_POS_ENABLE + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// �C���f�b�N�X�I�[�o�[�t���[�A�N�Z�X
			obj.getString( XSEMI_ALARM::ATOM_POS_ENABLE + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// ���݂��Ȃ��L�[�̃A�N�Z�X
			obj.getString( "Unknown" );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}

	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

