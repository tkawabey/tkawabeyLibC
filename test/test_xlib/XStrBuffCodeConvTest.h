#ifndef __XStrBuffCodeConvTest_H__
#define __XStrBuffCodeConvTest_H__


#include <cppunit/TestCase.h>


class XStrBuffCodeConvTest : public CPPUNIT_NS::TestCase
{
public:
	XStrBuffCodeConvTest(){}
	/** コンストラクタ */
	explicit XStrBuffCodeConvTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	UTF8変換テスト */
	void testConvUTF8(void);
private:

};



#endif /* __XStrBuffCodeConvTest_H__ */

