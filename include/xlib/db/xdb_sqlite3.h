/**
 * @file xdb_sqlite3.h
 * @brief 
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DB_SQLITE3_HEASER_FILE__
#define __X_DB_SQLITE3_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xtime.h"
#include "xlib//db/xdb.h"
#include "xlib/xdllorder.h"

class XLIB_API xDbConnection;
class XLIB_API xDbStmt;
class XLIB_API xDbSQLite3Stmt;

/**
 *	@brief	データコネクションクラスのベース
 */
class XLIB_API xDbSQLite3Connection : public xDbConnection
{
public:
	xDbSQLite3Connection();
	virtual ~xDbSQLite3Connection();
// properties
	/**	@brief	指定したDBが存在せず、新規に作成したかどうか
	 *
	 *	@retval	true	作成した
	 *	@retval	false	既に存在している
	 */
	bool isNewDb() const;
// methods
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
					const TCHAR* p_pwd);


	/**	@brief	コネクションをクローズします。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool close();

	/**	@brief	SQL文を実行します。
	 *
	 *	@param	p_sql	SQL文
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool execute(const TCHAR* p_sql);

	/**	@brief	データステートメントを取得します。
	 *
	 *	@param	p_sql	SQL文
	 *
	 *	@retval	!NULL	成功
	 *	@retval	NULL	失敗
	 */
	virtual xDbStmt* getSmt(const TCHAR* p_sql);
	/**	@brief	データステートメントを解放します。
	 *
	 *	@param	pStmt	a parameter of type fileAccDataAccStmt*
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *	
	 */
	virtual bool finalizeStmt(xDbStmt* pStmt);


private:
	XDlLorder		m_mdl;
	void*			m_pdbHandle;
	bool			m_bUseUTF16;
	bool			m_bNewDB;

	void*	m_fn_sqlite3_open;
	void*	m_fn_sqlite3_open16;
	void*	m_fn_sqlite3_close;
	void*	m_fn_sqlite3_exec;
	void*	m_fn_sqlite3_errmsg;
	void*	m_fn_sqlite3_errmsg16;
	void*	m_fn_sqlite3_prepare;
	void*	m_fn_sqlite3_prepare16;
	void*	m_fn_sqlite3_finalize;
	void*	m_fn_sqlite3_column_count;
	void*	m_fn_sqlite3_column_type;
	void*	m_fn_sqlite3_column_name;
	void*	m_fn_sqlite3_column_name16;
	void*	m_fn_sqlite3_bind_text;
	void*	m_fn_sqlite3_bind_text16;
	void*	m_fn_sqlite3_bind_int;
	void*	m_fn_sqlite3_bind_double;
	void*	m_fn_sqlite3_bind_blob;
	void*	m_fn_sqlite3_bind_null;
	void*	m_fn_sqlite3_column_text;
	void*	m_fn_sqlite3_column_text16;
	void*	m_fn_sqlite3_column_int;
	void*	m_fn_sqlite3_column_double;
	void*	m_fn_sqlite3_column_blob;
	void*	m_fn_sqlite3_column_bytes;
	void*	m_fn_sqlite3_step;
	void*	m_fn_sqlite3_clear_bindings;
	void*	m_fn_sqlite3_reset;
friend class xDbSQLite3Stmt;
};

/**
 *	@brief	データコネクションクラスのベース
 */
class XLIB_API xDbSQLite3Stmt : public xDbStmt
{
protected:
	xDbSQLite3Stmt();

public:
	virtual ~xDbSQLite3Stmt();


	virtual int getColumnCount();
	virtual XDATA_TYPE getColumnType(int index);
	virtual bool getColumnName(int index, XStrBuff &buf);

	virtual bool setText(int index, const char* val);
	virtual bool setText(int index, const wchar_t* val);
	virtual bool setBool(int index, bool val);
	virtual bool setInt1(int index, XD_INT1 val);
	virtual bool setUInt1(int index, XD_UINT1 val);
	virtual bool setInt2(int index, XD_INT2 val);
	virtual bool setUInt2(int index, XD_UINT2 val);
	virtual bool setInt4(int index, XD_INT4 val);
	virtual bool setUInt4(int index, XD_UINT4 val);
	virtual bool setInt8(int index, XD_INT8 val);
	virtual bool setUInt8(int index, XD_UINT8 val);
	virtual bool setFloat4(int index, XD_FLOAT4 val);
	virtual bool setFloat8(int index, XD_FLOAT8 val);
	virtual bool setBlob(int index, void* val, int size);
	virtual bool setDate(int index, const XTime* val);
	virtual bool setTime(int index, const XTime* val);
	virtual bool setTimeStamp(int index, const XTime* val);
	virtual bool setNull(int index);
	virtual bool setRData(int index, XRichData* val);

	virtual bool getText(int index, XStrBuff &buf);
	virtual bool getBool(int index, bool* val);
	virtual bool getInt1(int index, XD_INT1* pVal);
	virtual bool getUInt1(int index, XD_UINT1* pVal);
	virtual bool getInt2(int index, XD_INT2* pVal);
	virtual bool getUInt2(int index, XD_UINT2* pVal);
	virtual bool getInt4(int index, XD_INT4* pVal);
	virtual bool getUInt4(int index, XD_UINT4* pVal);
	virtual bool getInt8(int index, XD_INT8* pVal);
	virtual bool getUInt8(int index, XD_UINT8* pVal);
	virtual bool getFloat4(int index, XD_FLOAT4* pVal);
	virtual bool getFloat8(int index, XD_FLOAT8* pVal);
	virtual bool getBlob(int index, XBuff& buf, int* p_size);
	virtual bool getDate(int index, XTime* val);
	virtual bool getTime(int index, XTime* val);
	virtual bool getTimeStamp(int index, XTime* val);
	virtual bool isNull(int index, bool* pVal);
	virtual bool getRData(int index, XRichData* val);

	virtual xDbStmt::STS	step();
	virtual bool clearBinds();


private:
	void*	m_pstmt;
	void*	m_pdb;
	xDbSQLite3Connection* m_pCon;
friend class xDbSQLite3Connection;
};




#include "xlib/xlib_tail.h"
#endif /* __X_DB_SQLITE3_HEASER_FILE__ */
