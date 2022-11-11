#ifndef __XSecsPcapParserTest_H__
#define __XSecsPcapParserTest_H__


#include <cppunit/TestCase.h>
#include <fstream>

class XSecsPcapParserTest : public CPPUNIT_NS::TestCase
{
public:
	XSecsPcapParserTest(){}
	/** コンストラクタ */
	explicit XSecsPcapParserTest(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	普通の書き込みテスト */
	void testNomalAccess(void);
	/** @brief パケットの再送 */
	void testReTransmited_Packet();
	/** @brief SACK応答の入ったTCPダンプファイルを読み込み */
	void testSACKMessageInclude();

	/** @brief シリアライズからのデータの読み込み */
	void testLoadSerialize();
public:
#ifdef _UNICODE
	std::wfstream m_fst;
	std::fstream m_serialize;
#else
	std::fstream m_fst;
	std::fstream m_serialize;
#endif
};



#endif /* __XSecsPcapParserTest_H__ */

