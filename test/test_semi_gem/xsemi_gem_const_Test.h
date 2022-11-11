#ifndef __xsemi_gem_const_Test_H__
#define __xsemi_gem_const_Test_H__


#include <cppunit/TestCase.h>


class xsemi_gem_const_Test : public CPPUNIT_NS::TestCase
{
public:
	xsemi_gem_const_Test(){}
	/** �R���X�g���N�^ */
	explicit xsemi_gem_const_Test(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	XSEMI_GemDataAtomCSV�̃e�X�g */
	void test_XSEMI_GemDataAtomCSV(void);

	/**	@brief	S1F12����AV���X�V */
	void test_updateV_from_S1F12(void);
	/**	@brief	S2F29����AV���X�V */
	void test_updateV_from_S2F29(void);
	/**	@brief	S6F11�쐬 */
	void test_makeS6F11(void);
private:

};



#endif /* __xsemi_gem_const_Test_H__ */

