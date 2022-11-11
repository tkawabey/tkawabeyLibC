#ifndef __XJSonTest_H__
#define __XJSonTest_H__


#include <cppunit/TestCase.h>


class XJSonTest : public CPPUNIT_NS::TestCase
{
public:
	XJSonTest(){}
	/** �R���X�g���N�^ */
	explicit XJSonTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);


	void testParseJsonString(void);
	void testParseJsonObject(void);
	void testParseJsonArray(void);
	void testParseJsonNumetric(void);
	void testParseJsonPrimitive(void);
	/**	@brief	�R���X�g���N�^�[�̃e�X�g */
	void testXBuff(void);

private:

};



#endif /* __XJSonTest_H__ */

