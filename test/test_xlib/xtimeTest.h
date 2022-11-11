#ifndef __XTimeTest_H__
#define __XTimeTest_H__


#include <cppunit/TestCase.h>


class XTimeTest : public CPPUNIT_NS::TestCase
{
public:
	XTimeTest(){}
	/** コンストラクタ */
	explicit XTimeTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	XBuffのテスト */
	void testXBuff(void);


private:

};



#endif /* __XTimeTest_H__ */

