#ifndef __xsemi_ceid_Test_H__
#define __xsemi_ceid_Test_H__


#include <cppunit/TestCase.h>

class XSEMI_CEID_Test : public CPPUNIT_NS::TestCase
{
public:
	XSEMI_CEID_Test(){}
	/** �R���X�g���N�^ */
	explicit XSEMI_CEID_Test(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/** SQL������쐬 */
	void make_sqlstring();
	/** ATOM�A�N�Z�X�e�X�g */
	void atom_access();
	/** �ꗗ�Ǘ��e�X�g */
	void vector_test();

private:
};
#endif /* __xsemi_ceid_Test_H__ */


