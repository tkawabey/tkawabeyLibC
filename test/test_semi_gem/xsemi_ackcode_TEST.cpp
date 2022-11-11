#include <iostream>
#include <fstream>
#include "xsemi_ackcode_Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/semi/gem/xsemi_ackcode.h"
#include "xlib/xeception.h"
#include <strstream>
/**
コンストラクタ
@param name [in] テスト名
*/
XSEMI_ACKCODE_Test::XSEMI_ACKCODE_Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XSEMI_ACKCODE_Test::setUp(void)
{
}


/** テスト後処理 */
void XSEMI_ACKCODE_Test::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XSEMI_ACKCODE_Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSEMI_ACKCODE_Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_ACKCODE_Test>("atom_access", atom_access) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_ACKCODE_Test>("vector_access", vector_access) );

	return suite;
}

void XSEMI_ACKCODE_Test::atom_access()
{
	try {
		XSEMI_ACKCODE	obj(5000, 0xFF);
		int cnt_atom = obj.getColmnCount();
		int i;

		obj.set_Comment(_T("Comment"));

		try {
			// カラム名の取得
			// エラーがあったらスローする
			for(i = 0; i < cnt_atom; i++) {
				obj.getColmnName( i );
			}
		} catch(XExcept &e) {
			e.printStackTrace();
			CPPUNIT_ASSERT( 0 );
			return ;
		}


		CPPUNIT_ASSERT( obj.getColmnCount() == (XSEMI_ACKCODE::ATOM_POS_ENABLE+1));


		// ACKKIND
		CPPUNIT_ASSERT( obj.getUint(XSEMI_DATA_CLMN_ACKKIND) == 5000 );
		// ACKCODE
		CPPUNIT_ASSERT( obj.getUint(XSEMI_DATA_CLMN_ACKCODE) == 0xFF );
		// Comment
		CPPUNIT_ASSERT( strcmp(obj.getString(XSEMI_DATA_CLMN_Comment), "Comment") == 0 );
		// ObjectType
		CPPUNIT_ASSERT( obj.getUint( XSEMI_DATA_CLMN_ObjectType ) == XSEMI_GemConstObject::ACKCODE );
		CPPUNIT_ASSERT( obj.getUint( 99 ) == XSEMI_GemConstObject::ACKCODE );


		try {
			// インデックスオーバーフローアクセス
			obj.getColmnName( XSEMI_ACKCODE::ATOM_POS_ENABLE + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// インデックスオーバーフローアクセス
			obj.getString( XSEMI_ACKCODE::ATOM_POS_ENABLE + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		try {
			// 存在しないキーのアクセス
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

void XSEMI_ACKCODE_Test::vector_access()
{
	try {
		XSEMI_ACKCODEs	array;
		int i, k, index;
		int loopI = 5;
		int loopK = 3;

		for(k = 0; k < loopK; k++ ) {
			for(i = 0; i < loopI; i++ ) {
				XSEMI_ACKCODE obj(k, i);
				XSEMI_ACKCODE* pNewItem = array.add( &obj );

				CPPUNIT_ASSERT(pNewItem);

				// ACKKIND
				CPPUNIT_ASSERT( pNewItem->getUint(XSEMI_DATA_CLMN_ACKKIND) == k );
				// ACKCODE
				CPPUNIT_ASSERT( pNewItem->getUint(XSEMI_DATA_CLMN_ACKCODE) == i );
			}
		}



		XD_UINT4 objCount = array.count();
		CPPUNIT_ASSERT(  objCount == (loopK*loopI) );

		// インデックスアクセス
		for(k = 0, index = 0; k < loopK; k++ ) {
			for(i = 0; i < loopI; i++ ) {

				XSEMI_ACKCODE* pObj = array.get( index );
				CPPUNIT_ASSERT(pObj);
				// ACKKIND
				CPPUNIT_ASSERT( pObj->get_AckKind() == k );
				// ACKCODE
				CPPUNIT_ASSERT( pObj->get_AckCode() == i );


				index++;
			}
		}


		// ID指定アクセス
		for(k = 0, index = 0; k < loopK; k++ ) {
			for(i = 0; i < loopI; i++ ) {

				XSEMI_ACKCODE* pObj = array.get_byID( k, i  );
				CPPUNIT_ASSERT(pObj);
			}
		}


		// For-Eachアクセス
		XSEMI_ACKCODE* pCurObj = array.getFirstItem();
		CPPUNIT_ASSERT(pCurObj);
		for(k = 0, index = 0; k < loopK; k++ ) {
			for(i = 0; i < loopI; i++ ) {

				CPPUNIT_ASSERT(pCurObj);


				// ACKKIND
				CPPUNIT_ASSERT( pCurObj->get_AckKind() == k );
				// ACKCODE
				CPPUNIT_ASSERT( pCurObj->get_AckCode() == i );
			
				pCurObj = array.getNextItem();

			}
		}

		// 削除
		pCurObj = array.get_byID(1, 1);
		CPPUNIT_ASSERT(pCurObj);
		CPPUNIT_ASSERT( array.del(pCurObj) );
		// 削除したので、存在しないはず
		pCurObj = array.get_byID(1, 1);
		CPPUNIT_ASSERT( pCurObj == NULL );

		objCount = array.count();
		CPPUNIT_ASSERT(  objCount == ( (loopK*loopI) - 1 ) );




		{
			// 既に存在するアイテムを追加
			XSEMI_ACKCODE obj(1, 2);
			XSEMI_ACKCODE* pNewItem = array.add( &obj );
			CPPUNIT_ASSERT(pNewItem == NULL );
		}





	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}
