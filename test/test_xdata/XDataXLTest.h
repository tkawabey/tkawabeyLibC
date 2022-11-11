#ifndef __XDataXLTest_H__
#define __XDataXLTest_H__


#include <cppunit/TestCase.h>


class XDataXLTest : public CPPUNIT_NS::TestCase
{
public:
	XDataXLTest(){}
	/** �R���X�g���N�^ */
	explicit XDataXLTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	���ʂ̏������݃e�X�g
	 */
	void testNomalWrite(void);
	/**	@brief	���ʂ̓ǂݍ��݃e�X�g
	 */
	void testNormalRead(void);
private:

};



#endif /* __XDataXLTest_H__ */

