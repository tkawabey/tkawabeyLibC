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

	/**	@brief	XBuff�̃e�X�g */
	void testXBuff(void);


	/**	@brief	XInflateStreamBuff�̃e�X�g	*/
	void testXInflateStreamBuff(void);
	
	/**	@brief	XInflateStreamBuff�̃e�X�g	*/
	void testXInflateStreamBuff2(void);

	/**	@brief	XRingBuffer�̃e�X�g	*/
	void testXRingBuffer(void);

	/**	@brief	XRingBufferBL�̃e�X�g	*/
	void testXRingBufferBL(void);

private:

};



#endif /* __XBuffTest_H__ */

