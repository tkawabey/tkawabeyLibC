#ifndef __XBuffTest_H__
#define __XBuffTest_H__


#include <cppunit/TestCase.h>


class XBuffTest : public CPPUNIT_NS::TestCase
{
public:
	XBuffTest(){}
	/** コンストラクタ */
	explicit XBuffTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	普通の書き込みテスト */
	void testNomalWrite(void);
private:

};



#endif /* __XBuffTest_H__ */

