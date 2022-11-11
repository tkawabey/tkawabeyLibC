#ifndef __XStrBuffTest_H__
#define __XStrBuffTest_H__


#include <cppunit/TestCase.h>


class XStrBuffTest : public CPPUNIT_NS::TestCase
{
public:
	XStrBuffTest(){}
	/** コンストラクタ */
	explicit XStrBuffTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);



	/**	@brief	コンストラクターのテスト */
	void testConstructor(void);

	/**	@brief	set関数のテスト */
	void testSet(void);
	
	/**	@brief	operator=のテスト */
	void testOperatorEq(void);
	
	/**	@brief	operator 型*()のテスト */
	void testOperatorCast(void);

	/**	@brief	operator!= ot ==(const TCHAR* lpsz)のテスト */
	void testOperatorComp(void);

	/**	@brief	Getインデックスアクセスのテスト */
	void testOperatorGetIndexAcc(void);

	/**	@brief	Setインデックスアクセスのテスト */
	void testOperatorSetIndexAcc(void);

	/**	@brief	operator +=のテスト */
	void testOperatorAdd(void);

	/**	@brief	operator << のテスト */
	void testOperatorIns(void);

	/**	@brief	Count系 のテスト */
	void testCount(void);

	/**	@brief	Format系 のテスト */
	void testFormat(void);

	/**	@brief	その他雑多関数系のテスト */
	void testUtilFunctions(void);

	/** @brief	XStrBuffSplitのテスト*/
	void testXStrBuffSplit(void);

private:

};



#endif /* __XStrBuffTest_H__ */

