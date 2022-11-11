#ifndef __xsemi_ceid_Test_H__
#define __xsemi_ceid_Test_H__


#include <cppunit/TestCase.h>

class XSEMI_CEID_Test : public CPPUNIT_NS::TestCase
{
public:
	XSEMI_CEID_Test(){}
	/** コンストラクタ */
	explicit XSEMI_CEID_Test(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/** SQL文字列作成 */
	void make_sqlstring();
	/** ATOMアクセステスト */
	void atom_access();
	/** 一覧管理テスト */
	void vector_test();

private:
};
#endif /* __xsemi_ceid_Test_H__ */


