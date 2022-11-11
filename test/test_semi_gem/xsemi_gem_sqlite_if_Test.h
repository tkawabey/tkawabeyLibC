#ifndef __xsemi_gem_sqlite_if_Test_H__
#define __xsemi_gem_sqlite_if_Test_H__


#include <cppunit/TestCase.h>


class xsemi_gem_sqlite_if_Test : public CPPUNIT_NS::TestCase
{
public:
	xsemi_gem_sqlite_if_Test(){}
	/** �R���X�g���N�^ */
	explicit xsemi_gem_sqlite_if_Test(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/**	@brief	�V�KDB�쐬�e�X�g */
	void test_createNewDatabase();
	/**	@brief	XSEMI_GemDataAtom::makeXXXXString�̃e�X�g */
	void make_sql_string();

	/**	@brief	�S��`���L�q���ꂽCSV�t�@�C������C���|�[�g����B */
	void test_Import_All();
	/**	@brief	CEID��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_CEID();
	/**	@brief	REPTID��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_REPTI();
	/**	@brief	VID��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_VID();
	/**	@brief	CEID_RPTID��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_CEID_RPTID();
	/**	@brief	RPTID_DVID��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_RPTID_DVID();
	/**	@brief	KEYWORD��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_KEYWORD();
	/**	@brief	MODEL��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_MODEL();
	/**	@brief	MODEL_KEYWORD��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_MODEL_KEYWORD();
	/**	@brief	MODEL_TRANSIT��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_MODEL_TRANSIT();
	/**	@brief	MODEL_TRANSIT_VALS��CSV�t�@�C������ǂݍ��݃C���|�[�g����B */
	void test_ImportCSV_MODEL_TRANSIT_VALS();

	/**	@brief	CEID���X�V */
	void test_updateCEIDs();
	/**	@brief	Rpt���X�V */
	void test_updateRpts();
	/**	@brief	Rpt���X�V */
	void test_updateVs();
	/**	@brief	�C�x���g�����N */
	void test_linkEvent();
	/**	@brief	���[�h�e�X�g */
	void test_load(void);

private:

};



#endif /* __XSecsDataTest_H__ */

