#ifndef __XSEMI_ALARM_Test_H__
#define __XSEMI_ALARM_Test_H__


#include <cppunit/TestCase.h>


class XSEMI_ALARM_Test : public CPPUNIT_NS::TestCase
{
public:
	XSEMI_ALARM_Test(){}
	/** コンストラクタ */
	explicit XSEMI_ALARM_Test(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	void atom_access();
private:
};



#endif /* __XSEMI_ALARM_Test_H__ */

