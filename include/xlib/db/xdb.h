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
 *	@brief	データコネクションクラスのベース
 */
class XLIB_API xDbConnection
{
public:
	xDbConnection();
	virtual ~xDbConnection();

	/**	@brief	コネクションをオープンします。
	 *
	 *	@param	p_con_str	接続もじれつ
	 *	@param	p_usr	ユーザー名
	 *	@param	p_pwd	パスワード
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool open(const TCHAR* p_con_str, 
					const TCHAR* p_usr,
					const TCHAR* p_pwd) = 0;


	/**	@brief	コネクションをクローズします。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool close() = 0;

	/**	@brief	SQL文を実行します。
	 *
	 *	@param	p_sql	SQL文
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool execute(const TCHAR* p_sql) = 0;



	/**	@brief	データステートメントを取得します。
	 *
	 *	@param	p_sql	SQL文
	 *
	 *	@retval	!NULL	成功
	 *	@retval	NULL	失敗
	 */
	virtual xDbStmt* getSmt(const TCHAR* p_sql) = 0;
	/**	@brief	データステートメントを解放します。
	 *
	 *	@param	pStmt	a parameter of type fileAccDataAccStmt*
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *	
	 */
	virtual bool finalizeStmt(xDbStmt* pStmt) = 0;


	/**	@brief	トランザクションを開始します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *	
	 */
	virtual bool beginTrans();

	/**	@brief	トランザクションをコミットします。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool commit();

	/**	@brief	トランザクションをロールバックします。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool rollback();
};

/**
 *	@brief	データコネクションクラスのベース
 */
class XLIB_API xDbStmt : public XDataTblBase
{
protected:
	xDbStmt();

public:
	/**	@brief	stepステータス	 */
	enum STS{
			ROW		///<	まだデータの続きがある
		,	DONE	///<	stepは成功した
		,	ERR		///<	エラーが発生した
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
