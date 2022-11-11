#include <iostream>
#include <fstream>
#include "xsemi_ceid_Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/semi/gem/xsemi_gem_const.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/semi/gem/xsemi_ceid.h"
#include "xlib/semi/gem/xsemi_rpt.h"
#include "xlib/semi/gem/xsemi_v.h"
#include "xlib/semi/gem/xsemi_gem_sqlite_if.h"
#include "xlib/xeception.h"
#include <strstream>


XSEMI_CEID_Test::XSEMI_CEID_Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XSEMI_CEID_Test::setUp(void)
{
}


/** テスト後処理 */
void XSEMI_CEID_Test::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XSEMI_CEID_Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSEMI_CEID_Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_CEID_Test>("make_sqlstring", make_sqlstring) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_CEID_Test>("atom_access", atom_access) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_CEID_Test>("vector_test", vector_test) );

		
	return suite;
}

void XSEMI_CEID_Test::make_sqlstring()
{
	XSEMI_GemSQLiteIF objIF("xsecs_gem2.db");
	XSEMI_GemConst		construction;
	XSEMI_CEID	ceid(500, "EventName", "EventComment");
	
	
	XSEMI_CEID* pCEID;
	XSEMI_Rpt*  pRpt;
	XSEMI_V*	pV;
	XInflateStreamBuff	buff;
	std::ostream		stream( &buff );


	::unlink("xsecs_gem2.db");	
	objIF.createNewDatabase();
	construction.set_MasterData( &objIF );

	{
		pCEID = construction.get_CEIDs().add( &ceid );
		CPPUNIT_ASSERT( pCEID );
	}
	{
		XSEMI_Rpt	rpt(10, "RptName_10");
		pRpt  = construction.get_Rpts().add( &rpt );
		CPPUNIT_ASSERT( pRpt  );
	}
	{
		XSEMI_Rpt	rpt(11, "RptName_11");
		pRpt  = construction.get_Rpts().add( &rpt );
		CPPUNIT_ASSERT( pRpt  );
	}
	{
		XSEMI_ECV tmpV(13, "ECNAme13", XDATA_TYPE_U4);
		tmpV.put_DisplayName( "DisplayName" );
		pV = construction.get_Vs().add(&tmpV);
		CPPUNIT_ASSERT( pV  );
	}
	{
		XSEMI_ECV tmpV(14, "ECNAme14", XDATA_TYPE_U4);
		tmpV.put_DisplayName( "DisplayName" );
		pV = construction.get_Vs().add(&tmpV);
		CPPUNIT_ASSERT( pV  );
	}	
	
	construction.defineReport(10, 13);
	construction.defineReport(11, 14);
	construction.linkEventReport(500, 10);
	construction.linkEventReport(500, 11);



	try {
		buff.resetp();
		pCEID->makeInsertString(stream);
		//std::cout << buff.toString()  << std::endl;
		objIF.execute( buff.toString() );
	} catch(XExcept &e) {
		e.print( std::cerr );
		CPPUNIT_ASSERT( 0 );
	}

	try {
		objIF.updateObject( pCEID );
	} catch(XExcept &e) {
		e.print( std::cerr );
		CPPUNIT_ASSERT( 0 );
	}

	try {

		buff.resetp();
		pCEID->makeUpdateString(stream);
		//std::cout << buff.toString()  << std::endl;
		objIF.execute( buff.toString() );
	} catch(XExcept &e) {
		e.print( std::cerr );
		CPPUNIT_ASSERT( 0 );
	}
	try {

		buff.resetp();
		pCEID->makeDeleteString(stream);
		//std::cout << buff.toString()  << std::endl;
		objIF.execute( buff.toString() );
	} catch(XExcept &e) {
		e.print( std::cerr );
		CPPUNIT_ASSERT( 0 );
	}

}
void XSEMI_CEID_Test::atom_access()
{
	XSEMI_CEID ceid(500, "EventName", "EventComment");

	int cnt_atom = ceid.getColmnCount();
	int i;
	try {

		try {
			// カラム名の取得
			// エラーがあったらスローする
			for(i = 0; i < cnt_atom; i++) {
				ceid.getColmnName( i );
			}
		} catch(XExcept &e) {
			e.printStackTrace();
			CPPUNIT_ASSERT( 0 );
			return ;
		}
		ceid.put_Enable( true );

		// Atomアクセス
		// CeID
		CPPUNIT_ASSERT( ceid.getUint( XSEMI_DATA_CLMN_CeID ) == 500 );
		// EventName
		CPPUNIT_ASSERT( strcmp(ceid.getString(XSEMI_DATA_CLMN_EventName), "EventName") == 0 );
		// Comment
		CPPUNIT_ASSERT( strcmp(ceid.getString(XSEMI_DATA_CLMN_Comment), "EventComment") == 0 );
		// Enable
		CPPUNIT_ASSERT( ceid.getBOOL(XSEMI_DATA_CLMN_Enable) == true );

		// ObjectType
		CPPUNIT_ASSERT( ceid.getUint( XSEMI_DATA_CLMN_ObjectType ) == XSEMI_GemConstObject::CEID );
		CPPUNIT_ASSERT( ceid.getUint( 99 ) == XSEMI_GemConstObject::CEID );

		try {
			// インデックスオーバーフローアクセス
			ceid.getColmnName( XSEMI_CEID::ATOM_POS_ENABLE + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// インデックスオーバーフローアクセス
			ceid.getString( XSEMI_CEID::ATOM_POS_ENABLE + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// 存在しないキーのアクセス
			ceid.getString( "Unknown" );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void XSEMI_CEID_Test::vector_test()
{
	XSEMI_CEIDs ceids;
	XSEMI_CEID* pCEID = NULL;

	try {
		// CEID を追加
		{
			XSEMI_CEID tmp(600, "EventName_600", "EventComment_600");
			pCEID = ceids.add(&tmp);
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 600 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_600") == 0 );
		}
		{
			XSEMI_CEID tmp(601, "EventName_601", "EventComment_601");
			pCEID = ceids.add(&tmp);
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 601 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_601") == 0 );
		}
		{
			// 同じCEIDは追加できない
			XSEMI_CEID tmp(601, "EventName_601", "EventComment_601");
			pCEID = ceids.add(&tmp);
			CPPUNIT_ASSERT( pCEID == NULL);
		}
		
		// Count
		CPPUNIT_ASSERT( ceids.count() == 2 );

		// Index取得
		{
			pCEID = ceids.get(0);
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 600 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_600") == 0 );
		}
		{
			pCEID = ceids.get(1);
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 601 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_601") == 0 );
		}
		{
			pCEID = ceids.get(2);
			CPPUNIT_ASSERT( pCEID == NULL );
		}
		// ID取得
		{
			pCEID = ceids.get_byID(600);
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 600 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_600") == 0 );
		}
		{
			pCEID = ceids.get_byID(601);
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 601 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_601") == 0 );
		}
		{
			pCEID = ceids.get_byID(602);
			CPPUNIT_ASSERT( pCEID == NULL );
		}

		// Iteratorアクセス
		{
			// 最初のアイテムを取得
			pCEID = ceids.getFirstItem();
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 600 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_600") == 0 );
			// 次のアイテムを取得
			pCEID = ceids.getNextItem();
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 601 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_601") == 0 );
			// アイテムの最後に達している
			pCEID = ceids.getNextItem();
			CPPUNIT_ASSERT( pCEID == NULL );
			// 前のアイテムを取得
			pCEID = ceids.getPrevItem();
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 601 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_601") == 0 );
			// 前のアイテムを取得
			pCEID = ceids.getPrevItem();
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( pCEID->get_CEID() == 600 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pCEID->get_Comment(), "EventComment_600") == 0 );
			// アイテムの先頭に達している
			pCEID = ceids.getPrevItem();
			CPPUNIT_ASSERT( pCEID == NULL );
		}


		// 削除
		{
			pCEID = ceids.get_byID(600);
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( ceids.del(pCEID) );		
			CPPUNIT_ASSERT( ceids.count() == 1 );
			pCEID = ceids.get_byID(601);
			CPPUNIT_ASSERT( pCEID );
			CPPUNIT_ASSERT( ceids.del(pCEID) );		
			CPPUNIT_ASSERT( ceids.count() == 0 );
		}


	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}


