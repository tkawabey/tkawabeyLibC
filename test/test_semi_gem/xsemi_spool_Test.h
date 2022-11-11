#ifndef __XSEMI_SPOOL_Test_H__
#define __XSEMI_SPOOL_Test_H__


#include <cppunit/TestCase.h>


class XSEMI_Spool_Test : public CPPUNIT_NS::TestCase
{
public:
	XSEMI_Spool_Test(){}
	/** コンストラクタ */
	explicit XSEMI_Spool_Test(const char* name);

	/** テスト前処理 */
	virtual void setUp(void);

	/** テスト後処理 */
	virtual void tearDown(void);

	/** テストスイート */
	static CPPUNIT_NS::Test* suite(void);

	void atom_access();
	void vector_test();
	void spool_cls();
	void spool_stateModel();
private:
};



#endif /* __XSEMI_SPOOL_Test_H__ */

