#ifndef __xsecsHSMSPortTest_H__
#define __xsecsHSMSPortTest_H__


#include <cppunit/TestCase.h>


class XSecsHSMSPortTest : public CPPUNIT_NS::TestCase
{
public:
	XSecsHSMSPortTest(){}
	/** �R���X�g���N�^ */
	explicit XSecsHSMSPortTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	�A�N�e�B�u�̒ʐM�e�X�g */
	void testNomalActiveServer(void);


private:

};



#endif /* __xsecsHSMSPortTest_H__ */

