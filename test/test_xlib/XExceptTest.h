#ifndef __XExceptTest_H__
#define __XExceptTest_H__


#include <cppunit/TestCase.h>


class XExceptTest : public CPPUNIT_NS::TestCase
{
public:
	XExceptTest(){}
	/** �R���X�g���N�^ */
	explicit XExceptTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	���ʂ̏������݃e�X�g */
	void testNomalWrite(void);
private:

};



#endif /* __XExceptTest_H__ */

