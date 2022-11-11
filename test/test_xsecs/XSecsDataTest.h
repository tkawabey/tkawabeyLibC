#ifndef __XSecsDataTest_H__
#define __XSecsDataTest_H__


#include <cppunit/TestCase.h>


class XSecsDataTest : public CPPUNIT_NS::TestCase
{
public:
	XSecsDataTest(){}
	/** コンストラクタ */
	explicit XSecsDataTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	普通の書き込みテスト */
	void testNomalAccess(void);

	/**	@brief	バイナリーデータ作成 */
	void testBuildBinary(void);
private:

};



#endif /* __XSecsDataTest_H__ */

