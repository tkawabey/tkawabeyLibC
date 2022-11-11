#ifndef __xmsgqueueTest_H__
#define __xmsgqueueTest_H__


#include <cppunit/TestCase.h>


class xmsgqueueTest : public CPPUNIT_NS::TestCase
{
public:
	xmsgqueueTest(){}
	/** �R���X�g���N�^ */
	explicit xmsgqueueTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	XQueue */
	void testXQueue(void);

	/**	@brief	Pull */
	void testPull(void);

	/**	@brief	ClientServer */
	void testClientServer(void);
private:

};



#endif /* __xmsgqueueTest_H__ */

