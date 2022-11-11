#ifndef __XExceptTest_H__
#define __XExceptTest_H__


#include <cppunit/TestCase.h>


class XExceptTest : public CPPUNIT_NS::TestCase
{
public:
	XExceptTest(){}
	/** コンストラクタ */
	explicit XExceptTest(const char* name);

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



#endif /* __XExceptTest_H__ */

