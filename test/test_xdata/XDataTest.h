#ifndef __XDataTest_H__
#define __XDataTest_H__


#include <cppunit/TestCase.h>


class XDataTest : public CPPUNIT_NS::TestCase
{
public:
	XDataTest(){}
	/** コンストラクタ */
	explicit XDataTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	普通の書き込みテスト */
	void testNomalWrite(void);

	/**	@brief	普通の読み込みテスト */
	void testNormalRead(void);

	/**	@brief	リストのテスト */
	void testListValid();

	/**	@brief	BOOLのテスト */
	void testBoolValid();

	/**	@brief	BINARYのテスト */
	void testBinaryValid();

	/**	@brief	Asciiのテスト */
	void testAsciiValid();

	/**	@brief	JISのテスト */
	void testJISValid();

	/**	@brief	UTF16のテスト */
	void testUTF16Valid();

	/**	@brief	INT1のテスト */
	void testInt1Valid();

	/**	@brief	UINT1のテスト */
	void testUInt1Valid();

	/**	@brief	INT2のテスト */
	void testInt2Valid();

	/**	@brief	UINT2のテスト */
	void testUInt2Valid();


	/**	@brief	INT4のテスト */
	void testInt4Valid();

	/**	@brief	UINT4のテスト */
	void testUInt4Valid();

	/**	@brief	INT8のテスト */
	void testInt8Valid();

	/**	@brief	UINT8のテスト */
	void testUInt8Valid();

	/**	@brief	UFLOAT4のテスト */
	void testFloat4Valid();

	/**	@brief	UFLOAT8のテスト */
	void testFloat8Valid();
private:

};



#endif /* __XDataTest_H__ */

