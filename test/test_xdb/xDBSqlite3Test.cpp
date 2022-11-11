#include <iostream>
#include <fstream>
#include "xlib/xfile.h"
#include "xlib/db/xdb_sqlite3.h"
#include "xlib/xdata_tbl.h"
#include "xDbSQLite3Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
xDBSqlite3Test::xDBSqlite3Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void xDBSqlite3Test::setUp(void)
{
}


/** �e�X�g�㏈�� */
void xDBSqlite3Test::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* xDBSqlite3Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("xDBSqlite3Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<xDBSqlite3Test>("testConnect", testConnect) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xDBSqlite3Test>("testConnect", testInsert) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xDBSqlite3Test>("testConnect", testSelect) );
	
	return suite;
}


void xDBSqlite3Test::testConnect()
{
	xDbSQLite3Connection	con;



	try {
		XFileName::Remove( _T("�e�X�g.db") );
		// DB�쐬
		CPPUNIT_ASSERT( con.open( _T("�e�X�g.db") , "user", "passwd") == true );
		// DB�͐V�K�쐬����Ă���͂�
		CPPUNIT_ASSERT( con.isNewDb() == true );
		// TEBLE�쐬
		CPPUNIT_ASSERT( con.execute(
			_T("CREATE TABLE TEST( \n")			
			_T("	STR_VAL			VARCHAR(32), \n")
			_T("	INT_VAL			INTEGER, \n")
			_T("	DOUBLE_VAL		double, \n")
			_T("	DATE_VAL		DATETIME, \n")
			_T("	BLOB_VAL		blob, \n")
			_T("	PRIMARY KEY (STR_VAL) \n")
			_T("); \n")
			) == true );

		// insert into TEST values('test_data_1', 1, 0.123, '2009/11/12 20:30:66')
		CPPUNIT_ASSERT( con.execute(
			_T(" insert into TEST values('test_data_1', 1, 0.123, '2009-11-12 20:30:66', '') \n")			
			) == true );
	} catch(XExcept &ex) {
		ex.print();
		CPPUNIT_ASSERT( 0 );
	}
	try {
		CPPUNIT_ASSERT( con.close() == true );
	} catch(XExcept &ex) {
		ex.print();
		CPPUNIT_ASSERT( 0 );
	}



	try {
		// DBOPEN
		CPPUNIT_ASSERT( con.open(_T("�e�X�g.db") , "user", "passwd") == true );
		// DB�͊��ɑ��݂���͂�
		CPPUNIT_ASSERT( con.isNewDb() == false );
	} catch(XExcept &ex) {
		ex.print();
		CPPUNIT_ASSERT( 0 );
	}
	try {
		CPPUNIT_ASSERT( con.close() == true );
	} catch(XExcept &ex) {
		ex.print();
		CPPUNIT_ASSERT( 0 );
	}

}

/** �}���e�X�g */
void xDBSqlite3Test::testInsert()
{
	xDbSQLite3Connection	con;
	xDbStmt* pSmt = NULL;


	try {
		// DBOPEN
		CPPUNIT_ASSERT( con.open(_T("�e�X�g.db"), _T("user"), _T("passwd")) == true );
		// DB�͊��ɑ��݂���͂�
		CPPUNIT_ASSERT( con.isNewDb() == false );

		// StateMent�̏���
		pSmt = con.getSmt(_T("insert into TEST values(?,?,?,?,?)"));
		CPPUNIT_ASSERT( pSmt != NULL );

		XTime tTime = XTime::getCurrentTime();
		char pBin[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
		// SQL�̃p�����[�^���Z�b�g
		CPPUNIT_ASSERT(  pSmt->setText(1, _T("������")) == true );
		CPPUNIT_ASSERT(  pSmt->setInt4(2, 123456) == true );
		CPPUNIT_ASSERT(  pSmt->setFloat8(3, 99.123456789) == true );
		CPPUNIT_ASSERT(  pSmt->setTimeStamp(4, &tTime) == true );
		CPPUNIT_ASSERT(  pSmt->setBlob(5, pBin, 10) == true );
		// Insert�̎��s
		CPPUNIT_ASSERT(  pSmt->step() == xDbStmt::DONE );


		// �p�����[�^�̃��Z�b�g
		CPPUNIT_ASSERT(  pSmt->clearBinds() == true );

		// SQL�̃p�����[�^���Z�b�g
		tTime = XTime::getCurrentTime();
		CPPUNIT_ASSERT(  pSmt->setText(1, _T("�������Q")) == true );
		CPPUNIT_ASSERT(  pSmt->setInt4(2, 78910) == true );
		CPPUNIT_ASSERT(  pSmt->setFloat8(3, 0.987654321) == true );
		CPPUNIT_ASSERT(  pSmt->setTimeStamp(4, &tTime) == true );
		CPPUNIT_ASSERT(  pSmt->setBlob(5, pBin, 10) == true );
		// Insert�̎��s
		CPPUNIT_ASSERT(  pSmt->step() == xDbStmt::DONE );


		CPPUNIT_ASSERT(  con.finalizeStmt( pSmt ) == true );




		CPPUNIT_ASSERT( con.close() == true );

	} catch(XExcept &ex) {
		ex.print();
		CPPUNIT_ASSERT( 0 );
	}
}

/** Select�e�X�g */
void xDBSqlite3Test::testSelect()
{
	xDbSQLite3Connection	con;
	xDbStmt* pSmt = NULL;
	XStrBuff strBuff;
	int columnCnt = 0;
	int i;
	XDATA_TYPE type;

	try {
		// DBOPEN
		CPPUNIT_ASSERT( con.open(_T("�e�X�g.db"), _T("user"), _T("passwd")) == true );
		// DB�͊��ɑ��݂���͂�
		CPPUNIT_ASSERT( con.isNewDb() == false );

		// StateMent�̏���
		pSmt = con.getSmt(_T("select * from TEST"));
		CPPUNIT_ASSERT( pSmt != NULL );
		
		// �J�������̎擾
		columnCnt = pSmt->getColumnCount();
		CPPUNIT_ASSERT( columnCnt == 5 );


		while( pSmt->step() == xDbStmt::ROW ) {
			XD_INT4 iVal;
			double dVal;
			XBuff blobVal;
			int iBlobSize = 0;
			XTime timeVal;


			// �J�������ƁA�f�[�^�^�C�v�̃`�F�b�N
			for( i = 0; i < columnCnt; i++ ) {

				// �f�[�^�^�C�v�擾
				type = pSmt->getColumnType( i );
				// �J���������擾
				CPPUNIT_ASSERT( pSmt->getColumnName(i, strBuff) );
				switch( i ) {
				case 0:
					CPPUNIT_ASSERT( type == XDATA_TYPE_A );
					CPPUNIT_ASSERT( strBuff == _T("STR_VAL") );
					break;
				case 1:
					CPPUNIT_ASSERT( type == XDATA_TYPE_I4 );
					CPPUNIT_ASSERT( strBuff == _T("INT_VAL") );
					break;
				case 2:
					CPPUNIT_ASSERT( type == XDATA_TYPE_F8 );
					CPPUNIT_ASSERT( strBuff == _T("DOUBLE_VAL") );
					break;
				case 3:
					// SQL Lite�ł́ADate�^���Ȃ��̂ŁAString�Ƃ��ď���
					//CPPUNIT_ASSERT( type == xDbStmt::SqlDT_DateTiem ); 
					CPPUNIT_ASSERT( type == XDATA_TYPE_A );
					CPPUNIT_ASSERT( strBuff == _T("DATE_VAL") );
					break;
				case 4:
					CPPUNIT_ASSERT( type == XDATA_TYPE_B || type == XDATA_TYPE_A );
					CPPUNIT_ASSERT( strBuff == _T("BLOB_VAL") );
					break;
				}
			}

			CPPUNIT_ASSERT( pSmt->getText(0, strBuff) );
			CPPUNIT_ASSERT( pSmt->getInt4(1, &iVal) );
			CPPUNIT_ASSERT( pSmt->getFloat8(2, &dVal) );
			CPPUNIT_ASSERT( pSmt->getTimeStamp(3, &timeVal) );
			CPPUNIT_ASSERT( pSmt->getBlob(4, blobVal, &iBlobSize) );
		}

		// �X�e�[�g�����g�̌�n��
		CPPUNIT_ASSERT(  con.finalizeStmt( pSmt ) == true );


		// DB�����
		CPPUNIT_ASSERT( con.close() == true );

	} catch(XExcept &ex) {
		ex.print();
		CPPUNIT_ASSERT( 0 );
	}
}


