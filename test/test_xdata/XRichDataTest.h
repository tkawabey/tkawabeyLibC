#ifndef __XRichDataTest_H__
#define __XRichDataTest_H__


#include <cppunit/TestCase.h>


class XRichDataTest : public CPPUNIT_NS::TestCase
{
public:
	XRichDataTest(){}
	/** コンストラクタ */
	explicit XRichDataTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	普通の書き込みテスト */
	void testNomalWrite(void);
	/**	@brief	アサインテスト */
	void testAssign();
	/**	@brief	型変更テスト */
	void testChangeType();


	void testFind(void);
private:

};



#endif /* __XRichDataTest_H__ */

