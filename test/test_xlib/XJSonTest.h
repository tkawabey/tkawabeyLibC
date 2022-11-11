#ifndef __XJSonTest_H__
#define __XJSonTest_H__


#include <cppunit/TestCase.h>


class XJSonTest : public CPPUNIT_NS::TestCase
{
public:
	XJSonTest(){}
	/** コンストラクタ */
	explicit XJSonTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);


	void testParseJsonString(void);
	void testParseJsonObject(void);
	void testParseJsonArray(void);
	void testParseJsonNumetric(void);
	void testParseJsonPrimitive(void);
	/**	@brief	コンストラクターのテスト */
	void testXBuff(void);

private:

};



#endif /* __XJSonTest_H__ */

