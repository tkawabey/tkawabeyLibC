#include <iostream>
#include <fstream>
#include "xsemi_gem_sqlite_if_Test.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "xlib/semi/gem/xsemi_gem_const.h"
#include "xlib/semi/gem/xsemi_alarm.h"
#include "xlib/semi/gem/xsemi_ackcode.h"
#include "xlib/semi/gem/xsemi_gem_sqlite_if.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/xeception.h"

#include <windows.h>
#include <strstream>
/**
コンストラクタ
@param name [in] テスト名
*/
xsemi_gem_sqlite_if_Test::xsemi_gem_sqlite_if_Test(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** テスト前処理 */
void xsemi_gem_sqlite_if_Test::setUp(void)
{
}


/** テスト後処理 */
void xsemi_gem_sqlite_if_Test::tearDown(void)
{
}

/**	@brief	テストスイート */
CPPUNIT_NS::Test* xsemi_gem_sqlite_if_Test::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("xsemi_gem_sqlite_if_Test");

	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_createNewDatabase", test_createNewDatabase) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("make_sql_string", make_sql_string) );
	
//	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_Import_All", test_Import_All) );

/*
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_updateCEIDs", test_updateCEIDs) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_updateRpts", test_updateRpts) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_updateVs", test_updateVs) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_linkEvent", test_linkEvent) );


	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_createNewDatabase", test_createNewDatabase) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_CEID", test_ImportCSV_CEID) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_REPTI", test_ImportCSV_REPTI) );
*/
//	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_VID", test_ImportCSV_VID) );
/*
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_CEID_RPTID", test_ImportCSV_CEID_RPTID) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_RPTID_DVID", test_ImportCSV_RPTID_DVID) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_KEYWORD", test_ImportCSV_KEYWORD) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_MODEL", test_ImportCSV_MODEL) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_MODEL_KEYWORD", test_ImportCSV_MODEL_KEYWORD) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_MODEL_TRANSIT", test_ImportCSV_MODEL_TRANSIT) );
	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_ImportCSV_MODEL_TRANSIT_VALS", test_ImportCSV_MODEL_TRANSIT_VALS) );
*/
//	suite->addTest( new CPPUNIT_NS::TestCaller<xsemi_gem_sqlite_if_Test>("test_load", test_load) );

/*
XTRACE("XDATA_TYPE_B:%u\n", XDATA_TYPE_B);
XTRACE("XDATA_TYPE_BOOL:%u\n", XDATA_TYPE_BOOL);
XTRACE("XDATA_TYPE_A:%u\n", XDATA_TYPE_A);
XTRACE("XDATA_TYPE_J:%u\n", XDATA_TYPE_J);
XTRACE("XDATA_TYPE_U1:%u\n", XDATA_TYPE_U1);
XTRACE("XDATA_TYPE_U2:%u\n", XDATA_TYPE_U2);
XTRACE("XDATA_TYPE_U4:%u\n", XDATA_TYPE_U4);
XTRACE("XDATA_TYPE_U8:%u\n", XDATA_TYPE_U8);
XTRACE("XDATA_TYPE_I1:%u\n", XDATA_TYPE_I1);
XTRACE("XDATA_TYPE_I2:%u\n", XDATA_TYPE_I2);
XTRACE("XDATA_TYPE_I4:%u\n", XDATA_TYPE_I4);
XTRACE("XDATA_TYPE_I8:%u\n", XDATA_TYPE_I8);
XTRACE("XDATA_TYPE_F4:%u\n", XDATA_TYPE_F4);
XTRACE("XDATA_TYPE_F8:%u\n", XDATA_TYPE_F8);
*/
	return suite;
}



void xsemi_gem_sqlite_if_Test::test_createNewDatabase()
{
	try {
		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.createNewDatabase() );
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::make_sql_string()
{
	try {
		XInflateStreamBuff	buff;
		std::ostream		stream( &buff );

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		XSEMI_GemConst		construction;
		
		construction.set_MasterData( &objIF );


		XSEMI_CEID*		pCEID;
		XSEMI_Rpt*		pRpt;
		XSEMI_V*		pV;
		XSEMI_ALARM*	pAlarm;
		XSEMI_VARIABLE_GRP* pVGrp;
		XSEMI_ACKCODE*	pAckCode;



		{
			// ContructionにCEIDを追加
			XSEMI_CEID	tmpdata(500, "EventName", "EventComment");
			pCEID = construction.get_CEIDs().add( &tmpdata );
			CPPUNIT_ASSERT( pCEID );
		}
		{
			XSEMI_Rpt	rpt(10, "RptName_10");
			rpt.set_Comment(_T("RptCommant_10"));
			pRpt  = construction.get_Rpts().add( &rpt );
			CPPUNIT_ASSERT( pRpt  );
		}
		{
			XSEMI_Rpt	rpt(11, "RptName_11");
			rpt.set_Comment(_T("RptCommant_11"));
			pRpt  = construction.get_Rpts().add( &rpt );
			CPPUNIT_ASSERT( pRpt  );
		}
		{
			// ECV
			XSEMI_ECV tmpV(900, "ECNAme13", XDATA_TYPE_U4);
			tmpV.put_DisplayName( "DisplayName" );
			tmpV.put_Unit("UNIT13");
			tmpV.get_InitialValue().putValue( (XD_UINT4)999999 );
			tmpV.put_Comment(_T("Comment 13"));
			tmpV.put_VariableGrpID( 10 );
			tmpV.put_Private( true );
			tmpV.put_GetEx( true );
			tmpV.get_EcMax().putValue( (XD_UINT4)9999999 );
			tmpV.get_EcMin().putValue( (XD_UINT4)9999 );
			tmpV.get_EcDef().putValue( (XD_UINT4)9999 );
			tmpV.put_SetEx(true);
			tmpV.put_Verify(true);
			pV = construction.get_Vs().add(&tmpV);
			CPPUNIT_ASSERT( pV  );
		}
		{
			// ECV
			XSEMI_ECV tmpV(901, "ECNAme14", XDATA_TYPE_A);
			tmpV.put_DisplayName( "DisplayName" );
			tmpV.put_Unit("UNIT14");
			tmpV.get_InitialValue().putValueString( "INIT_VALUE" );
			tmpV.put_Comment(_T("Comment 14"));
			tmpV.put_VariableGrpID( 11 );
			tmpV.put_Private( false );
			tmpV.put_GetEx( false );
			tmpV.put_SetEx(false);
			tmpV.put_Verify(false);
			pV = construction.get_Vs().add(&tmpV);
			CPPUNIT_ASSERT( pV  );
		}	
		{
			// SV
			XSEMI_SV tmpV(902, "ECNAme15", XDATA_TYPE_I4);
			tmpV.put_DisplayName( "DisplayName15" );
			tmpV.put_Unit("UNIT15");
			tmpV.get_InitialValue().putValueString( "INIT_VALUE" );
			tmpV.put_Comment(_T("Comment 15"));
			tmpV.put_VariableGrpID( 12 );
			tmpV.put_Private( false );
			tmpV.put_GetEx( false );
			tmpV.put_SizeMax(500);
			tmpV.put_SizeMin(-500);
			tmpV.get_limitMax().putValue( (XD_INT4)100 );
			tmpV.get_limitMin().putValue( (XD_INT4)-100 );
			tmpV.put_CeLimit(500);
			pV = construction.get_Vs().add(&tmpV);
			CPPUNIT_ASSERT( pV  );
		}	
		{
			// DV
			XSEMI_DV tmpV(903, "ECNAme16", XDATA_TYPE_I4);
			tmpV.put_DisplayName( "DisplayName16" );
			tmpV.put_Unit("UNIT16");
			tmpV.get_InitialValue().putValueString( "INIT_VALUE" );
			tmpV.put_Comment(_T("Comment 16"));
			tmpV.put_VariableGrpID( 16 );
			tmpV.put_Private( false );
			tmpV.put_GetEx( false );
			tmpV.put_SizeMax(500);
			tmpV.put_SizeMin(-500);
			pV = construction.get_Vs().add(&tmpV);
			CPPUNIT_ASSERT( pV  );
		}
		{
			XSEMI_ALARM	rpt(5000, "Alarm_01");
			rpt.set_ALCD(0xFF);
			rpt.set_Text(_T("AlarmText_01"));
			rpt.set_Comment(_T("AlarmComment_01"));
			rpt.set_OnCEID(500);
			pAlarm  = construction.get_Alarms().add( &rpt );
			CPPUNIT_ASSERT( pAlarm  );
		}
		{
			XSEMI_VARIABLE_GRP	rpt(500, "XSEMI_VARIABLE_GRP_10");
			rpt.put_Comment(_T("COMMENT_VARIABLE_GRP_10"));
			pVGrp  = construction.get_VARIABLE_GRPs().add( &rpt );
			CPPUNIT_ASSERT( pVGrp  );
		}
		{
			XSEMI_ACKCODE	rpt(10, 11);
			rpt.set_Comment(_T("AckCode_10_11"));
			pAckCode  = construction.get_AckCodes().add( &rpt );
			CPPUNIT_ASSERT( pAckCode  );
		}
		{
			XSEMI_ACKCODE	rpt(10, 12);
			rpt.set_Comment(_T("AckCode_10_12"));
			pAckCode  = construction.get_AckCodes().add( &rpt );
			CPPUNIT_ASSERT( pAckCode  );
		}




		// イベントリンクの設定
		construction.defineReport(10, 900);
		construction.defineReport(10, 902);
		construction.defineReport(11, 901);
		construction.defineReport(11, 903);
		construction.linkEventReport(500, 10);
		construction.linkEventReport(500, 11);




		//
		//	CEIDのMaster更新
		//
		try {
			pCEID = construction.get_CEIDs().get(0);
			CPPUNIT_ASSERT( pCEID  );
			// 追加
			buff.resetp();
			pCEID->makeInsertString(stream);
			//std::cout << buff.toString()  << std::endl;
			objIF.execute( buff.toString() );

			// Const経由の更新
			objIF.updateObject( pCEID );

			// 更新
			buff.resetp();
			pCEID->makeUpdateString(stream);
			//std::cout << buff.toString()  << std::endl;
			objIF.execute( buff.toString() );

			// 削除
			buff.resetp();
			pCEID->makeDeleteString(stream);
			//std::cout << buff.toString()  << std::endl;
			objIF.execute( buff.toString() );


			// 追加
			buff.resetp();
			pCEID->makeInsertString(stream);
			//std::cout << buff.toString()  << std::endl;
			objIF.execute( buff.toString() );


			// Const経由の削除
			objIF.deleteObject( pCEID );

		} catch(XExcept &e) {
			e.print( std::cerr );
			CPPUNIT_ASSERT( 0 );
		}


		//
		//	RptIDの更新
		//
		try {
			for(XD_UINT4 i = 0; i < construction.get_Rpts().count(); i++) {
				pRpt = construction.get_Rpts().get(i);
				CPPUNIT_ASSERT( pRpt  );

				// 追加
				buff.resetp();
				pRpt->makeInsertString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// Const経由の更新
				objIF.updateObject( pRpt );

				// 更新
				buff.resetp();
				pRpt->makeUpdateString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// 削除
				buff.resetp();
				pRpt->makeDeleteString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );



				// 追加
				buff.resetp();
				pRpt->makeInsertString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// Const経由の削除
				objIF.deleteObject( pRpt );
			}
		} catch(XExcept &e) {
			e.print( std::cerr );
			CPPUNIT_ASSERT( 0 );
		}


		//
		//	VIDの更新
		//
		try {
			for(XD_UINT4 i = 0; i < construction.get_Vs().count(); i++) {
				pV = construction.get_Vs().get(i);
				CPPUNIT_ASSERT( pV  );


				// 追加
				buff.resetp();
				pV->makeInsertString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// Const経由の更新
				objIF.updateObject( pV );

				// 更新
				buff.resetp();
				pV->makeUpdateString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// 削除
				buff.resetp();
				pV->makeDeleteString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );
			}

		} catch(XExcept &e) {
			e.print( std::cerr );
			CPPUNIT_ASSERT( 0 );
		}

		//
		//	Alarmの更新
		//
		try {
			for(XD_UINT4 i = 0; i < construction.get_Alarms().count(); i++) {
				pAlarm = construction.get_Alarms().get(i);
				CPPUNIT_ASSERT( pAlarm  );


				// 追加
				buff.resetp();
				pAlarm->makeInsertString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// Const経由の更新
				objIF.updateObject( pAlarm );

				// 更新
				buff.resetp();
				pAlarm->makeUpdateString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// 削除
				buff.resetp();
				pAlarm->makeDeleteString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );
			}

		} catch(XExcept &e) {
			e.print( std::cerr );
			CPPUNIT_ASSERT( 0 );
		}


		//
		//	VARIABLE_GRPの更新
		//
		try {
			for(XD_UINT4 i = 0; i < construction.get_VARIABLE_GRPs().count(); i++) {
				pVGrp = construction.get_VARIABLE_GRPs().get(i);
				CPPUNIT_ASSERT( pVGrp  );

				// 追加
				buff.resetp();
				pVGrp->makeInsertString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// Const経由の更新
				objIF.updateObject( pVGrp );

				// 更新
				buff.resetp();
				pVGrp->makeUpdateString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// 削除
				buff.resetp();
				pVGrp->makeDeleteString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );
			}
		} catch(XExcept &e) {
			e.print( std::cerr );
			CPPUNIT_ASSERT( 0 );
		}

		//
		//	ACKCODEの更新
		//
		try {
			for(XD_UINT4 i = 0; i < construction.get_AckCodes().count(); i++) {
				pAckCode = construction.get_AckCodes().get(i);
				CPPUNIT_ASSERT( pAckCode  );

				// 追加
				buff.resetp();
				pAckCode->makeInsertString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// Const経由の更新
				objIF.updateObject( pAckCode );

				// 更新
				buff.resetp();
				pAckCode->makeUpdateString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );

				// 削除
				buff.resetp();
				pAckCode->makeDeleteString(stream);
				//std::cout << buff.toString()  << std::endl;
				objIF.execute( buff.toString() );
			}
		} catch(XExcept &e) {
			e.print( std::cerr );
			CPPUNIT_ASSERT( 0 );
		}


		objIF.exportCSV(XSEMI_GemMstData::TABLE_TYPE_ALL, _T("ALLDATAS.CSV"), true);


	} catch(XExcept &e) {
		e.print( std::cerr );
		CPPUNIT_ASSERT( 0 );
	}
}
void xsemi_gem_sqlite_if_Test::test_Import_All()
{
	try {
		time_t	startTm = ::time(NULL);

		XSEMI_GemSQLiteIF objIF("xsecs_gemALL.db");
		CPPUNIT_ASSERT( objIF.createNewDatabase() );

		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_ALL, "ALLDATAS.csv", true ) );
		time_t	endtTm = ::time(NULL);
		XTRACE("test_ImportCSV_CEID ProcessingTime:%d\n", endtTm - startTm);


		// 
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_CEID, "UPD_CEID.csv", false ) );

	} catch(XExcept &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << " Hint:" << e.getHint()  << std::endl;
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_ImportCSV_CEID()
{
	try {
		time_t	startTm = ::time(NULL);

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_CEID, "CEID.csv", true ) );
		time_t	endtTm = ::time(NULL);
		XTRACE("test_ImportCSV_CEID ProcessingTime:%d\n", endtTm - startTm);


		// 
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_CEID, "UPD_CEID.csv", false ) );

	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}
void xsemi_gem_sqlite_if_Test::test_ImportCSV_REPTI()
{
	try {
		time_t	startTm = ::time(NULL);

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_RPTID, "RPTID.csv", true ) );

		time_t	endtTm = ::time(NULL);
		XTRACE("test_ImportCSV_REPTI ProcessingTime:%d\n", endtTm - startTm);
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}
void xsemi_gem_sqlite_if_Test::test_ImportCSV_VID()
{
	try {
		XTime startTime = XTime::getCurrentTime();

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_VID_EC, "EC.csv", true ) );
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_VID_SV, "SV.csv", true ) );
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_VID_DV, "DV.csv", true ) );


		XTime endtTime = XTime::getCurrentTime();
		XTRACE("test_ImportCSV_VID ProcessingTime:%f\n", endtTime.m_dt - startTime.m_dt);



	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_ImportCSV_CEID_RPTID()
{
	try {
		XTime startTime = XTime::getCurrentTime();

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_CEID_RPTID, "CEID_RPTID.csv", true ) );

		XTime endtTime = XTime::getCurrentTime();
		XTRACE("test_ImportCSV_CEID_RPTID ProcessingTime:%f\n", endtTime.m_dt - startTime.m_dt);

		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_CEID_RPTID, "UPD_CEID_RPTID.csv", false ) );
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_ImportCSV_RPTID_DVID()
{
	try {
		XTime startTime = XTime::getCurrentTime();

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_RPTID_DVID, "RPTID_DVID.csv", true ) );

		XTime endtTime = XTime::getCurrentTime();
		XTRACE("test_ImportCSV_RPTID_DVID ProcessingTime:%f\n", endtTime.m_dt - startTime.m_dt);

		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_RPTID_DVID, "UPD_RPTID_DVID.csv", false ) );
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_ImportCSV_KEYWORD()
{
	try {
		XTime startTime = XTime::getCurrentTime();

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_KEYWORD, "KEYWORD.csv", true ) );

		XTime endtTime = XTime::getCurrentTime();
		XTRACE("test_ImportCSV_KEYWORD ProcessingTime:%f\n", endtTime.m_dt - startTime.m_dt);


		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_KEYWORD, "UPF_KEYWORD.csv", false ) );
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_ImportCSV_MODEL()
{
	try {
		time_t	startTm = ::time(NULL);

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_MODEL, "MODEL.csv", true ) );
		time_t	endtTm = ::time(NULL);
		XTRACE("test_ImportCSV_CEID ProcessingTime:%d\n", endtTm - startTm);

		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_MODEL, "UPF_MODEL.csv", false ) );
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_ImportCSV_MODEL_KEYWORD()
{
	try {
		time_t	startTm = ::time(NULL);

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_MODEL_KEYWORD, "MODEL_KEYWORD.csv", true ) );
		time_t	endtTm = ::time(NULL);
		XTRACE("test_ImportCSV_MODEL_KEYWORD ProcessingTime:%d\n", endtTm - startTm);

	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_ImportCSV_MODEL_TRANSIT()
{
	try {
		time_t	startTm = ::time(NULL);

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_MODEL_TRANSIT, "MODEL_TRANSIT.csv", true ) );
		time_t	endtTm = ::time(NULL);
		XTRACE("test_ImportCSV_MODEL_TRANSIT ProcessingTime:%d\n", endtTm - startTm);

	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}
void xsemi_gem_sqlite_if_Test::test_ImportCSV_MODEL_TRANSIT_VALS()
{
	try {
		time_t	startTm = ::time(NULL);

		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		CPPUNIT_ASSERT( objIF.importCSV(XSEMI_GemSQLiteIF::TABLE_TYPE_MODEL_TRANSIT_VALS, "MODEL_TRANSIT_VALS.csv", true ) );
		time_t	endtTm = ::time(NULL);
		XTRACE("test_ImportCSV_MODEL_TRANSIT_VALS ProcessingTime:%d\n", endtTm - startTm);

	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}







void xsemi_gem_sqlite_if_Test::test_updateCEIDs()
{
	try {
/*
		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		XSEMI_GemConst    gemConstMap;

		XSEMI_CEID* pCEID = NULL;

		int i;
		for(i = 0; i < 100; i++) {
			char sz[100];
			sprintf(sz, "CEID %04u", i);
			pCEID = gemConstMap.addCEID(i, sz);
			CPPUNIT_ASSERT( pCEID );

			sprintf(sz, "Comment %04u", i);
			pCEID->set_Comment(sz);
		}
		// テーブルが空の時の更新はInsert
		CPPUNIT_ASSERT( objIF.updateCEIDs( gemConstMap ) );


		pCEID = gemConstMap.addCEID(100, "CEID 100");
		CPPUNIT_ASSERT( pCEID );
		pCEID->set_Comment("Comment 100");
		// 既に存在するCEIDは更新新しいアイテムは更新
		CPPUNIT_ASSERT( objIF.updateCEIDs( gemConstMap ) );
*/
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_updateRpts()
{
	try {
/*
		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		XSEMI_GemConst    gemConstMap;

		XSEMI_Rpt* pRpt = NULL;

		int i;
		for(i = 0; i < 100; i++) {
			char sz[100];
			sprintf(sz, "Rept %04u", i);
			pRpt = gemConstMap.addRpt(i, sz);
			CPPUNIT_ASSERT( pRpt );

			sprintf(sz, "Comment %04u", i);
			pRpt->set_Comment(sz);
		}
		// テーブルが空の時の更新はInsert
		CPPUNIT_ASSERT( objIF.updateRpts( gemConstMap ) );


		pRpt = gemConstMap.addRpt(100, "Rpt 100");
		CPPUNIT_ASSERT( pRpt );
		pRpt->set_Comment("Comment 100");
		// 既に存在するCEIDは更新新しいアイテムは更新
		CPPUNIT_ASSERT( objIF.updateRpts( gemConstMap ) );
*/
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_updateVs()
{
	try {
/*
		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		XSEMI_GemConst    gemConstMap;

		XSEMI_V* pV = NULL;

		int i;
		for(i = 0; i < 100; i++) {
			char sz[100];
//			XDATA_TYPE type;
			XSEMI_V::Class tyClas;

			tyClas = (XSEMI_V::Class)(i % 3);

			sprintf(sz, "V %04u", i);
			pV = gemConstMap.addV(i, sz, XDATA_TYPE_A, tyClas);
			CPPUNIT_ASSERT( pV );

			sprintf(sz, "Comment %04u", i);
			pV->put_Comment(sz);
		}
		// テーブルが空の時の更新はInsert
		CPPUNIT_ASSERT( objIF.updateVs( gemConstMap ) );


		pV = gemConstMap.addV(100, "V 100", XDATA_TYPE_A, XSEMI_V::SV);
		CPPUNIT_ASSERT( pV );
		pV->put_Comment("Comment 100");
		// 既に存在するCEIDは更新新しいアイテムは更新
		CPPUNIT_ASSERT( objIF.updateVs( gemConstMap ) );
*/
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}


void xsemi_gem_sqlite_if_Test::test_linkEvent()
{
	try {
		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		XSEMI_GemConst    gemConstMap;


		CPPUNIT_ASSERT( objIF.load(gemConstMap) );

//		gemConstMap.linkEvent(0, 0);
//		gemConstMap.linkEvent(0, 1);
//		gemConstMap.linkEvent(0, 2);
//		gemConstMap.unlinkEvent(0, 1);

		// 既に存在するCEIDは更新新しいアイテムは更新
		CPPUNIT_ASSERT( objIF.updateObject( &gemConstMap.get_CEIDs() ) );
	} catch(XExcept &e) {
		std::cerr << e.what();
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}

void xsemi_gem_sqlite_if_Test::test_load(void)
{
	try {
/*
		XSEMI_GemSQLiteIF objIF("xsecs_gem.db");
		XSEMI_GemConst    eventMap;

		objIF.load(eventMap);

		{

			XInflateStreamBuff buff;
			std::ostream stream(&buff);
			XDataWriter writer( &stream );
			
			eventMap.makeS2F33(writer, 1);

			XDataReader reader(buff.getBuff(), buff.size());
//			reader.print(std::cout);
		}

		{

			XInflateStreamBuff buff;
			std::ostream stream(&buff);
			XDataWriter writer( &stream );
			
			eventMap.makeS2F35(writer, 1);

			XSecsData data;	
			data.setFromBinary(buff.getBuff(), buff.size(), XSecsData::BODY);
			data.put_wait(true);
			data.put_stream(2);
			data.put_function(35);

//			data.print(std::cout);

			{
				XInflateStreamBuff	buff2(256);
				std::iostream		stream2(&buff2);
				if( data.buildBinaryData(stream2, XSecsData::SIZE_HEAD_AND_BODY) == false ) {

				}

				unsigned char* pMsg = buff2.getBuff();
				unsigned long ulSize = buff2.size();
				pMsg+=14;
				ulSize-=14;
			}


			{
				std::strstream		stream2;
				if( data.buildBinaryData(stream2, XSecsData::SIZE_HEAD_AND_BODY) == false ) {

				}
				stream2.freeze(0);
				unsigned char* pMsg = (unsigned char*)stream2.str();
				unsigned long ulSize = stream2.pcount();
				pMsg+=14;
				ulSize-=14;


				XDataReader reader(pMsg, ulSize);
//				reader.print(std::cout);
			}
		}

		{

			XInflateStreamBuff buff;
			std::ostream stream(&buff);
			XDataWriter writer( &stream );
			
			eventMap.makeS6F11(writer, 401, 1);

			XDataReader reader(buff.getBuff(), buff.size());
			reader.print(std::cout);
		}
*/
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}
}
