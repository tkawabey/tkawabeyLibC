#include <iostream>
#include <fstream>
#include "xsemi_spool_Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/semi/gem/xsemi_spool.h"
#include "xlib/xeception.h"
#include <strstream>
/**
コンストラクタ
@param name [in] テスト名
*/
XSEMI_Spool_Test::XSEMI_Spool_Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void XSEMI_Spool_Test::setUp(void)
{
}


/** テスト後処理 */
void XSEMI_Spool_Test::tearDown(void)
{
	XFileName::Remove(_T("SPOOL.log"));


}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* XSEMI_Spool_Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSEMI_Spool_Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_Spool_Test>("atom_access",			atom_access) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_Spool_Test>("vector_test",			vector_test) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_Spool_Test>("spool_cls",			spool_cls) );
	suite->addTest( new CPPUNIT_NS::TestCaller<XSEMI_Spool_Test>("spool_stateModel",	spool_stateModel) );

	return suite;
}

void XSEMI_Spool_Test::atom_access()
{
	try {
		XSEMI_SpoolSF	obj(1, 11);
		int cnt_atom = obj.getColmnCount();
		int i;
		int maxIndex = (int)XSEMI_SpoolSF::ATOM_POS_FUNCTION;
		CPPUNIT_ASSERT( cnt_atom == 2 );
		try {
			// カラム名の取得
			// エラーがあったらスローする
			for(i = 0; i < cnt_atom; i++) {
				obj.getColmnName( i );
				obj.getColmnType( i );
				obj.isColumnKey( i );
			}
		} catch(XExcept &e) {
			e.printStackTrace();
			CPPUNIT_ASSERT( 0 );
			return ;
		}


		CPPUNIT_ASSERT( obj.getColmnCount() == (maxIndex+1));


		// STREAM
		CPPUNIT_ASSERT( obj.getUint(XSEMI_DATA_CLMN_STREAM) == 1 );
		// FUNCTION
		CPPUNIT_ASSERT( obj.getUint(XSEMI_DATA_CLMN_FUNCTION) == 11 );

		try {
			// インデックスオーバーフローアクセス
			obj.getColmnName( maxIndex + 1 );
			CPPUNIT_ASSERT( 0 );
		} catch(XExcept &e) {
			e.what();
		}
		

		try {
			// インデックスオーバーフローアクセス
			obj.getString( maxIndex + 1 );
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


void XSEMI_Spool_Test::vector_test()
{
	XSEMI_SpoolSFs ovjVector;
	XSEMI_SpoolSF* pObj = NULL;

	try {
		// 追加
		{
			XSEMI_SpoolSF tmp(1, 11);
			pObj = ovjVector.add(&tmp);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 1 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		{
			XSEMI_SpoolSF tmp(2, 11);
			pObj = ovjVector.add(&tmp);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 2 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		{
			// 同じアイテムは追加できない
			XSEMI_SpoolSF tmp(1, 11);
			pObj = ovjVector.add(&tmp);
			CPPUNIT_ASSERT( pObj == NULL);
		}
		
		// Count
		CPPUNIT_ASSERT( ovjVector.count() == 2 );

		// Index取得
		{
			pObj = ovjVector.get(0);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 1 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		{
			pObj = ovjVector.get(1);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 2 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		{
			pObj = ovjVector.get(2);
			CPPUNIT_ASSERT( pObj == NULL );
		}

		// ID取得
		{
			pObj = ovjVector.get_byID(1, 11);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 1 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		{
			pObj = ovjVector.get_byID(2, 11);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 2 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		{
			pObj = ovjVector.get_byID(3, 11);
			CPPUNIT_ASSERT( pObj == NULL );
		}

		// Iteratorアクセス
		{
			// 最初のアイテムを取得
			pObj = ovjVector.getFirstItem();
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 1 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
			// 次のアイテムを取得
			pObj = ovjVector.getNextItem();
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 2 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
			// アイテムの最後に達している
			pObj = ovjVector.getNextItem();
			CPPUNIT_ASSERT( pObj == NULL );
			// 前のアイテムを取得
			pObj = ovjVector.getPrevItem();
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 2 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
			// 前のアイテムを取得
			pObj = ovjVector.getPrevItem();
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 1 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
			// アイテムの先頭に達している
			pObj = ovjVector.getPrevItem();
			CPPUNIT_ASSERT( pObj == NULL );
		}


		// 削除
		{
			pObj = ovjVector.get_byID(1, 11);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( ovjVector.del(pObj) );		
			CPPUNIT_ASSERT( ovjVector.count() == 1 );
			pObj = ovjVector.get_byID(2, 11);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( ovjVector.del(pObj) );		
			CPPUNIT_ASSERT( ovjVector.count() == 0 );
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}



void XSEMI_Spool_Test::spool_cls()
{
	const char *pStringDATA= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

	try {
		XSEMI_SpoolSFs	ovjVector;
		XSEMI_Spool		objSpool( ovjVector );
		XSEMI_SpoolSF*	pObj = NULL;
		int iSts;


		// 追加
		{
			XSEMI_SpoolSF tmp(1, 11);
			pObj = ovjVector.add(&tmp);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 1 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		// open
		CPPUNIT_ASSERT( objSpool.open(_T("SPOOL.log"), 
										2000,		// サイズ
										3,			// トランスミット数
										false) );	// オーバーライト

		// データ作成
		XSECSHeader header;
		XInflateStreamBuff buff;
		std::ostream buffStream(&buff);
		XDataWriter writer(&buffStream);
		int i;
		XD_UINT4	ulChkSize = 0;
		XD_UINT4	ulChkSize20 = 0;


		// 
		{
			//pushトランスミットオーバー確認
			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(0);

			XTime obTime(2000, 1, 1, 0, 0, 0, 50);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_NG_TRANSMIT ); // トランスミットオーバーエラー
			CPPUNIT_ASSERT( 3 == objSpool.count() );

			// pop
			XD_UINT4	ulTimeSec, ulTimeMSec, ulDataSize;
			XBuff		buffO2;
			iSts = objSpool.pop(ulTimeSec, ulTimeMSec, header, buffO2, ulDataSize);
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 2 == objSpool.count() );
			iSts = objSpool.pop(ulTimeSec, ulTimeMSec, header, buffO2, ulDataSize);
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 1 == objSpool.count() );
			iSts = objSpool.pop(ulTimeSec, ulTimeMSec, header, buffO2, ulDataSize);
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 0 == objSpool.count() );
			iSts = objSpool.pop(ulTimeSec, ulTimeMSec, header, buffO2, ulDataSize);
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_NG_NON );	// データなし
		}


		// push
		{
			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(10);
			for(i = 0; i < 10; i++ ) {
				writer.putValAsStringA(pStringDATA);
			}

			ulChkSize = buff.size();
			XTime obTime(2000, 1, 1, 0, 0, 0, 50);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 1 == objSpool.count() );
		}
		// push
		{
			// Spool対象外のSxFxを指定
			XSecsData::setSxFx(header, 2, 13, true);
			buff.resetp();
			writer.putValAsList(10);
			for(i = 0; i < 10; i++ ) {
				writer.putValAsStringA(pStringDATA);
			}

			XTime obTime(2000, 1, 1, 0, 0, 0, 50);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_NA_SF );
			CPPUNIT_ASSERT( 1 == objSpool.count() );
		}
		// push
		{
			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(20);
			for(i = 0; i < 20; i++ ) {
				writer.putValAsStringA(pStringDATA);
			}
			ulChkSize20 = buff.size();
			XTime obTime(2000, 1, 1, 1, 1, 1, 50);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 2 == objSpool.count() );
		}
		// push
		{
			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(0);
			XTime obTime(2000, 1, 1, 2, 2, 2, 60);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 3 == objSpool.count() );
		}
		// push
		{
			// スプールが一杯になっているはず

			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(10);
			for(i = 0; i < 10; i++ ) {
				writer.putValAsStringA(pStringDATA);
			}
			XTime obTime(2000, 1, 1, 3, 3, 3, 50);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_NG_FULL );
			CPPUNIT_ASSERT( 3 == objSpool.count() );
		}

		
		// pop
		XD_UINT4	ulTimeSec, ulTimeMSec, ulDataSize;
		XBuff		buffO;
		iSts = objSpool.pop(ulTimeSec, ulTimeMSec, header, buffO, ulDataSize);
		CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
		CPPUNIT_ASSERT( 2 == objSpool.count() );
		{
			// Check TIME
			XTime obTime((time_t)ulTimeSec, ulTimeMSec);
			CPPUNIT_ASSERT(	obTime.GetYear() == 2000 );
			CPPUNIT_ASSERT(	obTime.GetMonth() == 1 );
			CPPUNIT_ASSERT(	obTime.GetDay() == 1 );
			CPPUNIT_ASSERT(	obTime.GetHour() == 0 );
			CPPUNIT_ASSERT(	obTime.GetMinute() == 0 );
			CPPUNIT_ASSERT(	obTime.GetSecond() == 0 );
			CPPUNIT_ASSERT(	obTime.m_ulMSec == 50 );

			// Check Header
			CPPUNIT_ASSERT(	XSecsData::get_stream(header) == 1 );
			CPPUNIT_ASSERT(	XSecsData::get_function(header) == 11 );
			CPPUNIT_ASSERT(	XSecsData::get_wait(header) == true );
			CPPUNIT_ASSERT(	ulChkSize == ulDataSize );

			// Check Data
			XData	data;
			XDataReader	reader(buffO, ulDataSize);

			CPPUNIT_ASSERT(	reader.getValue(&data) == true);
			CPPUNIT_ASSERT(	data.getType()         == XDATA_TYPE_L );
			CPPUNIT_ASSERT(	data.getCount()        == 10 );
			for(i = 0; i < 10; i++ ) {
				CPPUNIT_ASSERT(	reader.getValue(&data) == true);
				CPPUNIT_ASSERT(	data.getType()         == XDATA_TYPE_A );
				unsigned char *pRefData;
				ulDataSize = data.getValAsBinaryRef(&pRefData);
				CPPUNIT_ASSERT(	strlen(pStringDATA)        == ulDataSize );
				CPPUNIT_ASSERT(	memcmp(pStringDATA, pRefData, ulDataSize) == 0 );
			}
		}
		objSpool.close();
		

	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}

	try {

		// 再度オープン、SPOOL.logはマップファイルになっているので、前回のデータが引き継がれているはず
		XSECSHeader header;
		int i;

		XSEMI_SpoolSFs	ovjVector;
		XSEMI_Spool		objSpool( ovjVector );
		XSEMI_SpoolSF*	pObj = NULL;
		int iSts;


		// 追加
		{
			XSEMI_SpoolSF tmp(1, 11);
			pObj = ovjVector.add(&tmp);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 1 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		// open
		CPPUNIT_ASSERT( objSpool.open(_T("SPOOL.log"), 
										2000,		// サイズ
										3,			// トランスミット数
										false) );	// オーバーライト

		CPPUNIT_ASSERT( 2 == objSpool.count() );

		
		// pop
		XD_UINT4	ulTimeSec, ulTimeMSec, ulDataSize;
		XBuff		buffO;
		iSts = objSpool.pop(ulTimeSec, ulTimeMSec, header, buffO, ulDataSize);
		CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
		CPPUNIT_ASSERT( 1 == objSpool.count() );
		{
			// Check TIME
			XTime obTime((time_t)ulTimeSec, ulTimeMSec);
			CPPUNIT_ASSERT(	obTime.GetYear() == 2000 );
			CPPUNIT_ASSERT(	obTime.GetMonth() == 1 );
			CPPUNIT_ASSERT(	obTime.GetDay() == 1 );
			CPPUNIT_ASSERT(	obTime.GetHour() == 1 );
			CPPUNIT_ASSERT(	obTime.GetMinute() == 1 );
			CPPUNIT_ASSERT(	obTime.GetSecond() == 1 );
			CPPUNIT_ASSERT(	obTime.m_ulMSec == 50 );

			// Check Header
			CPPUNIT_ASSERT(	XSecsData::get_stream(header) == 1 );
			CPPUNIT_ASSERT(	XSecsData::get_function(header) == 11 );
			CPPUNIT_ASSERT(	XSecsData::get_wait(header) == true );

			// Check Data
			XData	data;
			XDataReader	reader(buffO, ulDataSize);

			CPPUNIT_ASSERT(	reader.getValue(&data) == true);
			CPPUNIT_ASSERT(	data.getType()         == XDATA_TYPE_L );
			CPPUNIT_ASSERT(	data.getCount()        == 20 );
			for(i = 0; i < 20; i++ ) {
				CPPUNIT_ASSERT(	reader.getValue(&data) == true);
				CPPUNIT_ASSERT(	data.getType()         == XDATA_TYPE_A );
				unsigned char *pRefData;
				ulDataSize = data.getValAsBinaryRef(&pRefData);
				CPPUNIT_ASSERT(	strlen(pStringDATA)        == ulDataSize );
				CPPUNIT_ASSERT(	memcmp(pStringDATA, pRefData, ulDataSize) == 0 );
			}
		}
		iSts = objSpool.pop(ulTimeSec, ulTimeMSec, header, buffO, ulDataSize);
		CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
		CPPUNIT_ASSERT( 0 == objSpool.count() );
		{
			// Check TIME
			XTime obTime((time_t)ulTimeSec, ulTimeMSec);
			CPPUNIT_ASSERT(	obTime.GetYear() == 2000 );
			CPPUNIT_ASSERT(	obTime.GetMonth() == 1 );
			CPPUNIT_ASSERT(	obTime.GetDay() == 1 );
			CPPUNIT_ASSERT(	obTime.GetHour() == 2 );
			CPPUNIT_ASSERT(	obTime.GetMinute() == 2 );
			CPPUNIT_ASSERT(	obTime.GetSecond() == 2 );
			CPPUNIT_ASSERT(	obTime.m_ulMSec == 60 );

			// Check Header
			CPPUNIT_ASSERT(	XSecsData::get_stream(header) == 1 );
			CPPUNIT_ASSERT(	XSecsData::get_function(header) == 11 );
			CPPUNIT_ASSERT(	XSecsData::get_wait(header) == true );

			// Check Data
			XData	data;
			XDataReader	reader(buffO, ulDataSize);

			CPPUNIT_ASSERT(	reader.getValue(&data) == true);
			CPPUNIT_ASSERT(	data.getType()         == XDATA_TYPE_L );
			CPPUNIT_ASSERT(	data.getCount()        == 0 );
		}
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void XSEMI_Spool_Test::spool_stateModel()
{
	const char *pStringDATA= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

	try {
		XSEMI_SpoolSFs	ovjVector;
		XSEMI_Spool		objSpool( ovjVector );
		XSEMI_SpoolSF*	pObj = NULL;
		int iSts;


		// 追加
		{
			XSEMI_SpoolSF tmp(1, 11);
			pObj = ovjVector.add(&tmp);
			CPPUNIT_ASSERT( pObj );
			CPPUNIT_ASSERT( pObj->get_Stream() == 1 );
			CPPUNIT_ASSERT( pObj->get_Function() == 11 );
		}
		// open
		CPPUNIT_ASSERT( objSpool.open(_T("SPOOL.log"), 
										2000,		// サイズ
										3,			// トランスミット数
										false) );	// オーバーライト

		// データ作成
		XSECSHeader header;
		XInflateStreamBuff buff;
		std::ostream buffStream(&buff);
		XDataWriter writer(&buffStream);
		int i;
		XD_UINT4	ulChkSize = 0;
		XD_UINT4	ulChkSize20 = 0;


		// スプール状態チェック
		CPPUNIT_ASSERT( 0 == objSpool.getActualCnt() );
		CPPUNIT_ASSERT( 0 == objSpool.getTotalCnt() );
		CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_INACTIVE == objSpool.getSpoolState() );
		CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOTFULL == objSpool.getSpoolLoadState() );
		CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOOUTPUT == objSpool.getSpoolUnLoadState() );


		// push
		{
			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(10);
			for(i = 0; i < 10; i++ ) {				writer.putValAsStringA(pStringDATA);			}

			ulChkSize = buff.size();
			XTime obTime(2000, 1, 1, 0, 0, 0, 50);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 1 == objSpool.count() );
			// スプール状態チェック
			CPPUNIT_ASSERT( 1 == objSpool.getActualCnt() );
			CPPUNIT_ASSERT( 1 == objSpool.getTotalCnt() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_ACTIVE == objSpool.getSpoolState() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOTFULL == objSpool.getSpoolLoadState() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOOUTPUT == objSpool.getSpoolUnLoadState() );
		}
		// push
		{
			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(20);
			for(i = 0; i < 20; i++ ) {
				writer.putValAsStringA(pStringDATA);
			}
			ulChkSize20 = buff.size();
			XTime obTime(2000, 1, 1, 1, 1, 1, 50);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 2 == objSpool.count() );
			// スプール状態チェック
			CPPUNIT_ASSERT( 2 == objSpool.getActualCnt() );
			CPPUNIT_ASSERT( 2 == objSpool.getTotalCnt() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_ACTIVE == objSpool.getSpoolState() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOTFULL == objSpool.getSpoolLoadState() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOOUTPUT == objSpool.getSpoolUnLoadState() );

		}
		// push
		{
			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(0);
			XTime obTime(2000, 1, 1, 2, 2, 2, 60);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
			CPPUNIT_ASSERT( 3 == objSpool.count() );
			// スプール状態チェック
			CPPUNIT_ASSERT( 3 == objSpool.getActualCnt() );
			CPPUNIT_ASSERT( 3 == objSpool.getTotalCnt() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_ACTIVE == objSpool.getSpoolState() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOTFULL == objSpool.getSpoolLoadState() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOOUTPUT == objSpool.getSpoolUnLoadState() );

		}
		// push
		{
			// スプールが一杯になっているはず

			XSecsData::setSxFx(header, 1, 11, true);
			buff.resetp();
			writer.putValAsList(10);
			for(i = 0; i < 10; i++ ) {
				writer.putValAsStringA(pStringDATA);
			}
			XTime obTime(2000, 1, 1, 3, 3, 3, 50);
			iSts = objSpool.push(obTime.mktime(), obTime.m_ulMSec, &header, 
				buff.getBuff(),
				buff.size());
			CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_NG_FULL );
			CPPUNIT_ASSERT( 3 == objSpool.count() );
			// スプール状態チェック
			CPPUNIT_ASSERT( 3 == objSpool.getActualCnt() );
			CPPUNIT_ASSERT( 4 == objSpool.getTotalCnt() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_FULL == objSpool.getSpoolState() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_FULL == objSpool.getSpoolLoadState() );
			CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOOUTPUT == objSpool.getSpoolUnLoadState() );
		}

		// パージ
		iSts = objSpool.purge();
		CPPUNIT_ASSERT( iSts == XSEMI_SPOOL_STS_OK );
		CPPUNIT_ASSERT( 0 == objSpool.getActualCnt() );
		CPPUNIT_ASSERT( 4 == objSpool.getTotalCnt() );
		CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_INACTIVE == objSpool.getSpoolState() );
		CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOTFULL == objSpool.getSpoolLoadState() );
		CPPUNIT_ASSERT( XSEMI_GEM_SPOOL_NOOUTPUT == objSpool.getSpoolUnLoadState() );

		
		objSpool.close();
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}