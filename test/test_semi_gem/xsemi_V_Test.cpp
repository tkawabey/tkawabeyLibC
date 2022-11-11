#include <iostream>
#include <fstream>
#include "xsemi_V_Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/semi/gem/xsemi_v.h"
#include "xlib/xeception.h"
#include <strstream>
/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XSEMI_V_Test::XSEMI_V_Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XSEMI_V_Test::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XSEMI_V_Test::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XSEMI_V_Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSEMI_V_Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_V_Test>("atom_ec_access", atom_ec_access) );

	return suite;
}

void XSEMI_V_Test::atom_ec_access()
{
	XSEMI_ECV v(500, "Test EC Name", XDATA_TYPE_A);
	int cnt_atom = v.getColmnCount();
	int i;
	try {
		// �e�X�g�f�[�^�̃Z�b�g
		v.put_DisplayName("EC Display Name");
		v.put_Unit("UNIT");
		v.get_InitialValue().putValueString("init value");
		v.put_Comment("Comment");
		v.put_Enable(true);
		v.put_VariableGrpID(501);
		v.put_Private(true);
		v.put_GetEx(true);
		v.get_EcMax().putValueString("EcMax");
		v.get_EcMin().putValueString("EcMin");
		v.get_EcDef().putValueString("EcDef");
		v.put_SetEx(true);
		v.put_Verify(true);


		try {
			// �J�������̎擾
			// �G���[����������X���[����
			for(i = 0; i < cnt_atom; i++) {
				v.getColmnName( i );
			}
		} catch(XExcept &e) {
			e.printStackTrace();
			CPPUNIT_ASSERT( 0 );
			return ;
		}
		// Atom�A�N�Z�X
		// VID
		CPPUNIT_ASSERT( v.getUint( XSEMI_DATA_CLMN_VID ) == 500 );
		// Name
		CPPUNIT_ASSERT( strcmp(v.getString(XSEMI_DATA_CLMN_Name), "Test EC Name") == 0 );
		// DisplayName
		CPPUNIT_ASSERT( strcmp(v.getString(XSEMI_DATA_CLMN_DisplayName), "EC Display Name") == 0 );
		// DataUnit
		CPPUNIT_ASSERT( strcmp(v.getString(XSEMI_DATA_CLMN_DataUnit), "UNIT") == 0 );
		// DataType
		CPPUNIT_ASSERT( v.getUint( XSEMI_DATA_CLMN_DataType ) == XDATA_TYPE_A );
		// Mode
		CPPUNIT_ASSERT( v.getUint( XSEMI_DATA_CLMN_Mode ) == 0 );
		// InitValue
		CPPUNIT_ASSERT( strcmp(v.getString(XSEMI_DATA_CLMN_InitValue), "init value") == 0 );
		// Category
		CPPUNIT_ASSERT( v.getUint( XSEMI_DATA_CLMN_Category ) == XSEMI_V::EC );
		// Comment
		CPPUNIT_ASSERT( strcmp(v.getString(XSEMI_DATA_CLMN_Comment), "Comment") == 0 );
		// VARIABLE_GRP
		CPPUNIT_ASSERT( v.getUint( XSEMI_DATA_CLMN_VARIABLE_GRP ) == 501 );
		// VPRIVATE 
		CPPUNIT_ASSERT( v.getBOOL( XSEMI_DATA_CLMN_VPRIVATE ) == true );
		// GET_EX 
		CPPUNIT_ASSERT( v.getBOOL( XSEMI_DATA_CLMN_GET_EX ) == true );
		// EcMax
		CPPUNIT_ASSERT( strcmp(v.getString(XSEMI_DATA_CLMN_EcMax), "EcMax") == 0 );
		// EcMin
		CPPUNIT_ASSERT( strcmp(v.getString(XSEMI_DATA_CLMN_EcMin), "EcMin") == 0 );
		// EcDef
		CPPUNIT_ASSERT( strcmp(v.getString(XSEMI_DATA_CLMN_EcDef), "EcDef") == 0 );
		// SET_EX 
		CPPUNIT_ASSERT( v.getBOOL( XSEMI_DATA_CLMN_SET_EX ) == true );
		// ECVerify 
		CPPUNIT_ASSERT( v.getBOOL( XSEMI_DATA_CLMN_ECVerify ) == true );
		// Enable
		CPPUNIT_ASSERT( v.getBOOL(XSEMI_DATA_CLMN_Enable) == true );
		// ObjectType
		CPPUNIT_ASSERT( v.getUint( XSEMI_DATA_CLMN_ObjectType ) == XSEMI_GemConstObject::VID );
		CPPUNIT_ASSERT( v.getUint( 99 ) == XSEMI_GemConstObject::VID );
		try {
			// �C���f�b�N�X�I�[�o�[�t���[�A�N�Z�X
			v.getColmnName( XSEMI_ECV::ATOM_POS_EC_VALUE + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// �C���f�b�N�X�I�[�o�[�t���[�A�N�Z�X
			v.getString( XSEMI_ECV::ATOM_POS_EC_VALUE + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// ���݂��Ȃ��L�[�̃A�N�Z�X
			v.getString( "Unknown" );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}


	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

