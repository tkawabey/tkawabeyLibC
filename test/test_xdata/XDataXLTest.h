#ifndef __XDataXLTest_H__
#define __XDataXLTest_H__


#include <cppunit/TestCase.h>


class XDataXLTest : public CPPUNIT_NS::TestCase
{
public:
	XDataXLTest(){}
	/** コンストラクタ */
	explicit XDataXLTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	普通の書き込みテスト
	 */
	void testNomalWrite(void);
	/**	@brief	普通の読み込みテスト
	 */
	void testNormalRead(void);
private:

};



#endif /* __XDataXLTest_H__ */

