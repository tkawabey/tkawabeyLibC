#ifndef __XSEMI_ACKCODES_Test_H__
#define __XSEMI_ACKCODES_Test_H__


#include <cppunit/TestCase.h>


class XSEMI_ACKCODE_Test : public CPPUNIT_NS::TestCase
{
public:
	XSEMI_ACKCODE_Test(){}
	/** �R���X�g���N�^ */
	explicit XSEMI_ACKCODE_Test(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	void atom_access();
	void vector_access();
private:
};



#endif /* __XSEMI_ACKCODES_Test_H__ */

