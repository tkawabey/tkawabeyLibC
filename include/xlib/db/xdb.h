/**
 * @file xdb.h
 * @brief 
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DB_HEASER_FILE__
#define __X_DB_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xtime.h"
#include "xlib/xdata.h"
#include "xlib/xdata_rich.h"
#include "xlib/xdata_tbl.h"
#include "xlib/xeception.h"


class xDbConnection;
class xDbStmt;

/**
 *	@brief	�f�[�^�R�l�N�V�����N���X�̃x�[�X
 */
class XLIB_API xDbConnection
{
public:
	xDbConnection();
	virtual ~xDbConnection();

	/**	@brief	�R�l�N�V�������I�[�v�����܂��B
	 *
	 *	@param	p_con_str	�ڑ��������
	 *	@param	p_usr	���[�U�[��
	 *	@param	p_pwd	�p�X���[�h
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool open(const TCHAR* p_con_str, 
					const TCHAR* p_usr,
					const TCHAR* p_pwd) = 0;


	/**	@brief	�R�l�N�V�������N���[�Y���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool close() = 0;

	/**	@brief	SQL�������s���܂��B
	 *
	 *	@param	p_sql	SQL��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool execute(const TCHAR* p_sql) = 0;



	/**	@brief	�f�[�^�X�e�[�g�����g���擾���܂��B
	 *
	 *	@param	p_sql	SQL��
	 *
	 *	@retval	!NULL	����
	 *	@retval	NULL	���s
	 */
	virtual xDbStmt* getSmt(const TCHAR* p_sql) = 0;
	/**	@brief	�f�[�^�X�e�[�g�����g��������܂��B
	 *
	 *	@param	pStmt	a parameter of type fileAccDataAccStmt*
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *	
	 */
	virtual bool finalizeStmt(xDbStmt* pStmt) = 0;


	/**	@brief	�g�����U�N�V�������J�n���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *	
	 */
	virtual bool beginTrans();

	/**	@brief	�g�����U�N�V�������R�~�b�g���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool commit();

	/**	@brief	�g�����U�N�V���������[���o�b�N���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool rollback();
};

/**
 *	@brief	�f�[�^�R�l�N�V�����N���X�̃x�[�X
 */
class XLIB_API xDbStmt : public XDataTblBase
{
protected:
	xDbStmt();

public:
	/**	@brief	step�X�e�[�^�X	 */
	enum STS{
			ROW		///<	�܂��f�[�^�̑���������
		,	DONE	///<	step�͐�������
		,	ERR		///<	�G���[����������
	} ;
	virtual ~xDbStmt();


	virtual bool next();
	virtual STS	step() = 0;
	virtual bool clearBinds() = 0;

private:
friend class xDbConnection;
};




#include "xlib/xlib_tail.h"
#endif /* __X_DB_HEASER_FILE__ */
