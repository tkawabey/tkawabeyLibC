#ifndef __XTimeTest_H__
#define __XTimeTest_H__


#include <cppunit/TestCase.h>


class XTimeTest : public CPPUNIT_NS::TestCase
{
public:
	XTimeTest(){}
	/** �R���X�g���N�^ */
	explicit XTimeTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	XBuff�̃e�X�g */
	void testXBuff(void);


private:

};



#endif /* __XTimeTest_H__ */

