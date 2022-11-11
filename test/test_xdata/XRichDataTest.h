#ifndef __XRichDataTest_H__
#define __XRichDataTest_H__


#include <cppunit/TestCase.h>


class XRichDataTest : public CPPUNIT_NS::TestCase
{
public:
	XRichDataTest(){}
	/** �R���X�g���N�^ */
	explicit XRichDataTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	���ʂ̏������݃e�X�g */
	void testNomalWrite(void);
	/**	@brief	�A�T�C���e�X�g */
	void testAssign();
	/**	@brief	�^�ύX�e�X�g */
	void testChangeType();


	void testFind(void);
private:

};



#endif /* __XRichDataTest_H__ */

