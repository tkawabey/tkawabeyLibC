#ifndef __XSecsPcapParserTest_H__
#define __XSecsPcapParserTest_H__


#include <cppunit/TestCase.h>
#include <fstream>

class XSecsPcapParserTest : public CPPUNIT_NS::TestCase
{
public:
	XSecsPcapParserTest(){}
	/** �R���X�g���N�^ */
	explicit XSecsPcapParserTest(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	���ʂ̏������݃e�X�g */
	void testNomalAccess(void);
	/** @brief �p�P�b�g�̍đ� */
	void testReTransmited_Packet();
	/** @brief SACK�����̓�����TCP�_���v�t�@�C����ǂݍ��� */
	void testSACKMessageInclude();

	/** @brief �V���A���C�Y����̃f�[�^�̓ǂݍ��� */
	void testLoadSerialize();
public:
#ifdef _UNICODE
	std::wfstream m_fst;
	std::fstream m_serialize;
#else
	std::fstream m_fst;
	std::fstream m_serialize;
#endif
};



#endif /* __XSecsPcapParserTest_H__ */

