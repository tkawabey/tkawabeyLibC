#ifndef __xDBSqlite3Test_H__
#define __xDBSqlite3Test_H__


#include <cppunit/TestCase.h>


class xDBSqlite3Test : public CPPUNIT_NS::TestCase
{
public:
	xDBSqlite3Test(){}
	/** コンストラクタ */
	explicit xDBSqlite3Test(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/** 接続テスト */
	void testConnect();


	/** 挿入テスト */
	void testInsert();

	/** Selectテスト */
	void testSelect();
private:

};



#endif /* __xDBSqlite3Test_H__ */

