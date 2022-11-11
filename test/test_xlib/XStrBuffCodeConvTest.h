#ifndef __XStrBuffCodeConvTest_H__
#define __XStrBuffCodeConvTest_H__


#include <cppunit/TestCase.h>


class XStrBuffCodeConvTest : public CPPUNIT_NS::TestCase
{
public:
	XStrBuffCodeConvTest(){}
	/** �R���X�g���N�^ */
	explicit XStrBuffCodeConvTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	UTF8�ϊ��e�X�g */
	void testConvUTF8(void);
private:

};



#endif /* __XStrBuffCodeConvTest_H__ */

