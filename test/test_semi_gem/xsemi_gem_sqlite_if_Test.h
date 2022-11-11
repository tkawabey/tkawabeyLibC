#ifndef __xsemi_gem_sqlite_if_Test_H__
#define __xsemi_gem_sqlite_if_Test_H__


#include <cppunit/TestCase.h>


class xsemi_gem_sqlite_if_Test : public CPPUNIT_NS::TestCase
{
public:
	xsemi_gem_sqlite_if_Test(){}
	/** コンストラクタ */
	explicit xsemi_gem_sqlite_if_Test(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	新規DB作成テスト */
	void test_createNewDatabase();
	/**	@brief	XSEMI_GemDataAtom::makeXXXXStringのテスト */
	void make_sql_string();

	/**	@brief	全定義が記述されたCSVファイルからインポートする。 */
	void test_Import_All();
	/**	@brief	CEIDをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_CEID();
	/**	@brief	REPTIDをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_REPTI();
	/**	@brief	VIDをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_VID();
	/**	@brief	CEID_RPTIDをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_CEID_RPTID();
	/**	@brief	RPTID_DVIDをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_RPTID_DVID();
	/**	@brief	KEYWORDをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_KEYWORD();
	/**	@brief	MODELをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_MODEL();
	/**	@brief	MODEL_KEYWORDをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_MODEL_KEYWORD();
	/**	@brief	MODEL_TRANSITをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_MODEL_TRANSIT();
	/**	@brief	MODEL_TRANSIT_VALSをCSVファイルから読み込みインポートする。 */
	void test_ImportCSV_MODEL_TRANSIT_VALS();

	/**	@brief	CEIDを更新 */
	void test_updateCEIDs();
	/**	@brief	Rptを更新 */
	void test_updateRpts();
	/**	@brief	Rptを更新 */
	void test_updateVs();
	/**	@brief	イベントリンク */
	void test_linkEvent();
	/**	@brief	ロードテスト */
	void test_load(void);

private:

};



#endif /* __XSecsDataTest_H__ */

