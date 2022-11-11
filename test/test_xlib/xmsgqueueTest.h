#ifndef __xmsgqueueTest_H__
#define __xmsgqueueTest_H__


#include <cppunit/TestCase.h>


class xmsgqueueTest : public CPPUNIT_NS::TestCase
{
public:
	xmsgqueueTest(){}
	/** コンストラクタ */
	explicit xmsgqueueTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	XQueue */
	void testXQueue(void);

	/**	@brief	Pull */
	void testPull(void);

	/**	@brief	ClientServer */
	void testClientServer(void);
private:

};



#endif /* __xmsgqueueTest_H__ */

