#ifndef __xsemi_gem_const_Test_H__
#define __xsemi_gem_const_Test_H__


#include <cppunit/TestCase.h>


class xsemi_gem_const_Test : public CPPUNIT_NS::TestCase
{
public:
	xsemi_gem_const_Test(){}
	/** コンストラクタ */
	explicit xsemi_gem_const_Test(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	XSEMI_GemDataAtomCSVのテスト */
	void test_XSEMI_GemDataAtomCSV(void);

	/**	@brief	S1F12から、Vを更新 */
	void test_updateV_from_S1F12(void);
	/**	@brief	S2F29から、Vを更新 */
	void test_updateV_from_S2F29(void);
	/**	@brief	S6F11作成 */
	void test_makeS6F11(void);
private:

};



#endif /* __xsemi_gem_const_Test_H__ */

