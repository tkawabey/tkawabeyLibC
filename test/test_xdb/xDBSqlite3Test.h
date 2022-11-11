#ifndef __xDBSqlite3Test_H__
#define __xDBSqlite3Test_H__


#include <cppunit/TestCase.h>


class xDBSqlite3Test : public CPPUNIT_NS::TestCase
{
public:
	xDBSqlite3Test(){}
	/** �R���X�g���N�^ */
	explicit xDBSqlite3Test(const char* name);

	/** �e�X�g�O���� */
	virtual void setUp(void);

	/** �e�X�g�㏈�� */
	virtual void tearDown(void);

	/** �e�X�g�X�C�[�g */
	static CPPUNIT_NS::Test* suite(void);

	/** �ڑ��e�X�g */
	void testConnect();


	/** �}���e�X�g */
	void testInsert();

	/** Select�e�X�g */
	void testSelect();
private:

};



#endif /* __xDBSqlite3Test_H__ */

