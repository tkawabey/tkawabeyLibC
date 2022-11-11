#ifndef __xsecsHSMSPortTest_H__
#define __xsecsHSMSPortTest_H__


#include <cppunit/TestCase.h>


class XSecsHSMSPortTest : public CPPUNIT_NS::TestCase
{
public:
	XSecsHSMSPortTest(){}
	/** コンストラクタ */
	explicit XSecsHSMSPortTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	アクティブの通信テスト */
	void testNomalActiveServer(void);


private:

};



#endif /* __xsecsHSMSPortTest_H__ */

