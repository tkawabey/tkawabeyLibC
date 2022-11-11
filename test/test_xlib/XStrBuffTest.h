#ifndef __XStrBuffTest_H__
#define __XStrBuffTest_H__


#include <cppunit/TestCase.h>


class XStrBuffTest : public CPPUNIT_NS::TestCase
{
public:
	XStrBuffTest(){}
	/** �R���X�g���N�^ */
	explicit XStrBuffTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);



	/**	@brief	�R���X�g���N�^�[�̃e�X�g */
	void testConstructor(void);

	/**	@brief	set�֐��̃e�X�g */
	void testSet(void);
	
	/**	@brief	operator=�̃e�X�g */
	void testOperatorEq(void);
	
	/**	@brief	operator �^*()�̃e�X�g */
	void testOperatorCast(void);

	/**	@brief	operator!= ot ==(const TCHAR* lpsz)�̃e�X�g */
	void testOperatorComp(void);

	/**	@brief	Get�C���f�b�N�X�A�N�Z�X�̃e�X�g */
	void testOperatorGetIndexAcc(void);

	/**	@brief	Set�C���f�b�N�X�A�N�Z�X�̃e�X�g */
	void testOperatorSetIndexAcc(void);

	/**	@brief	operator +=�̃e�X�g */
	void testOperatorAdd(void);

	/**	@brief	operator << �̃e�X�g */
	void testOperatorIns(void);

	/**	@brief	Count�n �̃e�X�g */
	void testCount(void);

	/**	@brief	Format�n �̃e�X�g */
	void testFormat(void);

	/**	@brief	���̑��G���֐��n�̃e�X�g */
	void testUtilFunctions(void);

	/** @brief	XStrBuffSplit�̃e�X�g*/
	void testXStrBuffSplit(void);

private:

};



#endif /* __XStrBuffTest_H__ */

