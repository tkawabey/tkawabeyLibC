#ifndef __XSecsTextParserTest_H__
#define __XSecsTextParserTest_H__


#include <cppunit/TestCase.h>


class XSecsTextParserTest : public CPPUNIT_NS::TestCase
{
public:
	XSecsTextParserTest(){}
	/** コンストラクタ */
	explicit XSecsTextParserTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	普通の書き込みテスト */
	void testNomalAccess(void);

	/**	@brief	バイナリー文字列を解析 */
	void testParseBinary();
	
	/**	@brief	U1を解析 */
	void testparseU1();
	
	/**	@brief	I1を解析 */
	void testparseI1();
	
	/**	@brief	U2を解析 */
	void testparseU2();

	/**	@brief	I2を解析 */
	void testparseI2();

	/**	@brief	U4を解析 */
	void testparseU4();

	/**	@brief	I4を解析 */
	void testparseI4();

	void testS2F3_Bin();






private:

};



#endif /* __XSecsTextParserTest_H__ */

