#include <iostream>
#include <fstream>
#include "xsemi_rpt_Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/semi/gem/xsemi_gem_const.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/semi/gem/xsemi_rpt.h"
#include "xlib/semi/gem/xsemi_rpt.h"
#include "xlib/semi/gem/xsemi_v.h"
#include "xlib/semi/gem/xsemi_gem_sqlite_if.h"
#include "xlib/xeception.h"
#include <strstream>


XSEMI_Rpt_Test::XSEMI_Rpt_Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XSEMI_Rpt_Test::setUp(void)
{
}


/** テスト後処理 */
void XSEMI_Rpt_Test::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XSEMI_Rpt_Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSEMI_Rpt_Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_Rpt_Test>("make_sqlstring", make_sqlstring) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_Rpt_Test>("atom_access", atom_access) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_Rpt_Test>("vector_test", vector_test) );

		
	return suite;
}

void XSEMI_Rpt_Test::make_sqlstring()
{

}

void XSEMI_Rpt_Test::atom_access()
{
	XSEMI_Rpt rpt(500, "Report Name");

	int cnt_atom = rpt.getColmnCount();
	int i;
	try {

		try {
			// カラム名の取得
			// エラーがあったらスローする
			for(i = 0; i < cnt_atom; i++) {
				rpt.getColmnName( i );
			}
		} catch(XExcept &e) {
			e.printStackTrace();
			CPPUNIT_ASSERT( 0 );
			return ;
		}
		rpt.set_Comment( _T("Comment") );
		rpt.set_Enable( true );

		// Atomアクセス
		// RptID
		CPPUNIT_ASSERT( rpt.getUint( XSEMI_DATA_CLMN_RptID ) == 500 );
		// RptName
		CPPUNIT_ASSERT( strcmp(rpt.getString(XSEMI_DATA_CLMN_RptName), "Report Name") == 0 );
		// Comment
		CPPUNIT_ASSERT( strcmp(rpt.getString(XSEMI_DATA_CLMN_Comment), "Comment") == 0 );
		// Enable
		CPPUNIT_ASSERT( rpt.getBOOL(XSEMI_DATA_CLMN_Enable) == true );

		// ObjectType
		CPPUNIT_ASSERT( rpt.getUint( XSEMI_DATA_CLMN_ObjectType ) == XSEMI_GemConstObject::RPTID );
		CPPUNIT_ASSERT( rpt.getUint( 99 ) == XSEMI_GemConstObject::RPTID );

		try {
			// インデックスオーバーフローアクセス
			rpt.getColmnName( cnt_atom );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// インデックスオーバーフローアクセス
			rpt.getString( cnt_atom  );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// 存在しないキーのアクセス
			rpt.getString( "Unknown" );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void XSEMI_Rpt_Test::vector_test()
{
	XSEMI_Rpts repts(false);
	XSEMI_Rpt* pRpts = NULL;

	try {
		// RptID を追加
		{
			XSEMI_Rpt tmp(600, "EventName_600");//, "EventComment_600");
			tmp.set_Comment("EventComment_600");
			pRpts = repts.add(&tmp);
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 600 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_600") == 0 );
		}
		{
			XSEMI_Rpt tmp(601, "EventName_601");//, "EventComment_601");
			tmp.set_Comment("EventComment_601");
			pRpts = repts.add(&tmp);
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 601 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_601") == 0 );
		}
		{
			// 同じCEIDは追加できない
			XSEMI_Rpt tmp(601, "EventName_601");//, "EventComment_601");
			tmp.set_Comment("EventComment_601");
			pRpts = repts.add(&tmp);
			CPPUNIT_ASSERT( pRpts == NULL);
		}
		
		// Count
		CPPUNIT_ASSERT( repts.count() == 2 );

		// Index取得
		{
			pRpts = repts.get(0);
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 600 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_600") == 0 );
		}
		{
			pRpts = repts.get(1);
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 601 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_601") == 0 );
		}
		{
			pRpts = repts.get(2);
			CPPUNIT_ASSERT( pRpts == NULL );
		}
		// ID取得
		{
			pRpts = repts.get_byID(600);
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 600 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_600") == 0 );
		}
		{
			pRpts = repts.get_byID(601);
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 601 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_601") == 0 );
		}
		{
			pRpts = repts.get_byID(602);
			CPPUNIT_ASSERT( pRpts == NULL );
		}

		// Iteratorアクセス
		{
			// 最初のアイテムを取得
			pRpts = repts.getFirstItem();
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 600 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_600") == 0 );
			// 次のアイテムを取得
			pRpts = repts.getNextItem();
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 601 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_601") == 0 );
			// アイテムの最後に達している
			pRpts = repts.getNextItem();
			CPPUNIT_ASSERT( pRpts == NULL );
			// 前のアイテムを取得
			pRpts = repts.getPrevItem();
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 601 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_601") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_601") == 0 );
			// 前のアイテムを取得
			pRpts = repts.getPrevItem();
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( pRpts->get_RptID() == 600 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Name(), "EventName_600") == 0 );
			CPPUNIT_ASSERT( strcmp(pRpts->get_Comment(), "EventComment_600") == 0 );
			// アイテムの先頭に達している
			pRpts = repts.getPrevItem();
			CPPUNIT_ASSERT( pRpts == NULL );
		}


		// 削除
		{
			pRpts = repts.get_byID(600);
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( repts.del(pRpts) );		
			CPPUNIT_ASSERT( repts.count() == 1 );
			pRpts = repts.get_byID(601);
			CPPUNIT_ASSERT( pRpts );
			CPPUNIT_ASSERT( repts.del(pRpts) );		
			CPPUNIT_ASSERT( repts.count() == 0 );
		}


	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}


