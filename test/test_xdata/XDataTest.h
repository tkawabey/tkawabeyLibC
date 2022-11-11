#ifndef __XDataTest_H__
#define __XDataTest_H__


#include <cppunit/TestCase.h>


class XDataTest : public CPPUNIT_NS::TestCase
{
public:
	XDataTest(){}
	/** �R���X�g���N�^ */
	explicit XDataTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	���ʂ̏������݃e�X�g */
	void testNomalWrite(void);

	/**	@brief	���ʂ̓ǂݍ��݃e�X�g */
	void testNormalRead(void);

	/**	@brief	���X�g�̃e�X�g */
	void testListValid();

	/**	@brief	BOOL�̃e�X�g */
	void testBoolValid();

	/**	@brief	BINARY�̃e�X�g */
	void testBinaryValid();

	/**	@brief	Ascii�̃e�X�g */
	void testAsciiValid();

	/**	@brief	JIS�̃e�X�g */
	void testJISValid();

	/**	@brief	UTF16�̃e�X�g */
	void testUTF16Valid();

	/**	@brief	INT1�̃e�X�g */
	void testInt1Valid();

	/**	@brief	UINT1�̃e�X�g */
	void testUInt1Valid();

	/**	@brief	INT2�̃e�X�g */
	void testInt2Valid();

	/**	@brief	UINT2�̃e�X�g */
	void testUInt2Valid();


	/**	@brief	INT4�̃e�X�g */
	void testInt4Valid();

	/**	@brief	UINT4�̃e�X�g */
	void testUInt4Valid();

	/**	@brief	INT8�̃e�X�g */
	void testInt8Valid();

	/**	@brief	UINT8�̃e�X�g */
	void testUInt8Valid();

	/**	@brief	UFLOAT4�̃e�X�g */
	void testFloat4Valid();

	/**	@brief	UFLOAT8�̃e�X�g */
	void testFloat8Valid();
private:

};



#endif /* __XDataTest_H__ */

