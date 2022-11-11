#ifndef __XSecsTextParserTest_H__
#define __XSecsTextParserTest_H__


#include <cppunit/TestCase.h>


class XSecsTextParserTest : public CPPUNIT_NS::TestCase
{
public:
	XSecsTextParserTest(){}
	/** �R���X�g���N�^ */
	explicit XSecsTextParserTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	���ʂ̏������݃e�X�g */
	void testNomalAccess(void);

	/**	@brief	�o�C�i���[���������� */
	void testParseBinary();
	
	/**	@brief	U1����� */
	void testparseU1();
	
	/**	@brief	I1����� */
	void testparseI1();
	
	/**	@brief	U2����� */
	void testparseU2();

	/**	@brief	I2����� */
	void testparseI2();

	/**	@brief	U4����� */
	void testparseU4();

	/**	@brief	I4����� */
	void testparseI4();

	void testS2F3_Bin();






private:

};



#endif /* __XSecsTextParserTest_H__ */

