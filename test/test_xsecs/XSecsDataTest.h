#ifndef __XSecsDataTest_H__
#define __XSecsDataTest_H__


#include <cppunit/TestCase.h>


class XSecsDataTest : public CPPUNIT_NS::TestCase
{
public:
	XSecsDataTest(){}
	/** �R���X�g���N�^ */
	explicit XSecsDataTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	���ʂ̏������݃e�X�g */
	void testNomalAccess(void);

	/**	@brief	�o�C�i���[�f�[�^�쐬 */
	void testBuildBinary(void);
private:

};



#endif /* __XSecsDataTest_H__ */

