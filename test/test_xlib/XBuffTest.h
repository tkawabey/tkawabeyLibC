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

	/**	@brief	XBuffのテスト */
	void testXBuff(void);


	/**	@brief	XInflateStreamBuffのテスト	*/
	void testXInflateStreamBuff(void);
	
	/**	@brief	XInflateStreamBuffのテスト	*/
	void testXInflateStreamBuff2(void);

	/**	@brief	XRingBufferのテスト	*/
	void testXRingBuffer(void);

	/**	@brief	XRingBufferBLのテスト	*/
	void testXRingBufferBL(void);

private:

};



#endif /* __XBuffTest_H__ */

