#ifndef __XSEMI_ALARM_Test_H__
#define __XSEMI_ALARM_Test_H__


#include <cppunit/TestCase.h>


class XSEMI_ALARM_Test : public CPPUNIT_NS::TestCase
{
public:
	XSEMI_ALARM_Test(){}
	/** �R���X�g���N�^ */
	explicit XSEMI_ALARM_Test(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	void atom_access();
private:
};



#endif /* __XSEMI_ALARM_Test_H__ */

