#ifndef __XSEMI_V_Test_H__
#define __XSEMI_V_Test_H__


#include <cppunit/TestCase.h>


class XSEMI_V_Test : public CPPUNIT_NS::TestCase
{
public:
	XSEMI_V_Test(){}
	/** コンストラクタ */
	explicit XSEMI_V_Test(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	void atom_ec_access();
private:
};



#endif /* __XSEMI_V_Test_H__ */

