#ifndef __xmlhelperTest_H__
#define __xmlhelperTest_H__


#include <cppunit/TestCase.h>


class xmlhelperTest : public CPPUNIT_NS::TestCase
{
public:
	xmlhelperTest(){}
	/** �R���X�g���N�^ */
	explicit xmlhelperTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	���ʂ̏������݃e�X�g */
	void testRead(void);

	/**	@brief	���ʂ̏������݃e�X�g */
	void testRead2(void);

private:

};



#endif /* __xmlhelperTest_H__ */

