#ifndef __XBuffTest_H__
#define __XBuffTest_H__


#include <cppunit/TestCase.h>


class XBuffTest : public CPPUNIT_NS::TestCase
{
public:
	XBuffTest(){}
	/** �R���X�g���N�^ */
	explicit XBuffTest(const char* name);

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



#endif /* __XBuffTest_H__ */

