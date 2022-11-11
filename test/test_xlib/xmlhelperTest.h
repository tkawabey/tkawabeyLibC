#ifndef __xmlhelperTest_H__
#define __xmlhelperTest_H__


#include <cppunit/TestCase.h>


class xmlhelperTest : public CPPUNIT_NS::TestCase
{
public:
	xmlhelperTest(){}
	/** コンストラクタ */
	explicit xmlhelperTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	普通の書き込みテスト */
	void testRead(void);

	/**	@brief	普通の書き込みテスト */
	void testRead2(void);

private:

};



#endif /* __xmlhelperTest_H__ */

