
#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"
#include "xlib/db/xdb_sqlite3.h"
#include "xlib/xstrcordeconv.h"
#include <sqlite/sqlite3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <tchar.h>
#include <stdio.h>

	typedef int (*def_sqlite3_open16)(const void *, sqlite3 **);
	typedef const void* (*def_sqlite3_errmsg16)(sqlite3*);
	typedef int (*def_sqlite3_prepare16)(sqlite3 *,const void *,int ,sqlite3_stmt **,const void **);
	typedef int (*def_sqlite3_bind_text16)(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
	typedef const void* (*def_sqlite3_column_text16)(sqlite3_stmt*, int);
	typedef const void* (*def_sqlite3_column_name16)(sqlite3_stmt*,int);


	typedef int (*def_sqlite3_open)(const char *, sqlite3 **);
	typedef const char* (*def_sqlite3_errmsg)(sqlite3*);
	typedef int (*def_sqlite3_prepare)(sqlite3 *, const char *,int,sqlite3_stmt **,const char **);
	typedef int (*def_sqlite3_bind_text)(sqlite3_stmt*, int, const char*, int n, void(*)(void*));
	typedef const unsigned char* (*def_sqlite3_column_text)(sqlite3_stmt*, int iCol);
	typedef const char* (*def_sqlite3_column_name)(sqlite3_stmt*,int);

	typedef int (*def_sqlite3_close)(sqlite3 *);
	typedef int (*def_sqlite3_exec)(sqlite3*,const char *,
				 sqlite3_callback,void *,char **errmsg);
	typedef int (*def_sqlite3_finalize)(sqlite3_stmt *);
	typedef int (*def_sqlite3_column_count)(sqlite3_stmt *);
	typedef int (*def_sqlite3_column_type)(sqlite3_stmt*, int );
	typedef int (*def_sqlite3_bind_int)(sqlite3_stmt*, int, int);
	typedef int (*def_sqlite3_bind_double)(sqlite3_stmt*, int, double);
	typedef int (*def_sqlite3_bind_blob)(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
	typedef int (*def_sqlite3_bind_null)(sqlite3_stmt*, int);
	typedef int (*def_sqlite3_column_int)(sqlite3_stmt*, int iCol);
	typedef double (*def_sqlite3_column_double)(sqlite3_stmt*, int iCol);
	typedef const void* (*def_sqlite3_column_blob)(sqlite3_stmt*, int iCol);
	typedef int (*def_sqlite3_column_bytes)(sqlite3_stmt*, int iCol);
	typedef int (*def_sqlite3_step)(sqlite3_stmt*);
	typedef int (*def_sqlite3_clear_bindings)(sqlite3_stmt*);
	typedef int (*def_sqlite3_reset)(sqlite3_stmt *pStmt);


#define XSQLITE3_VALID_OPEN(a) 	if( a== NULL ) { \
		XEXCEPT_THROW_STR(XExcept_DB, _T("dose not opend."), 0, 0);	\
	}

#define XSQLITE3_VALID_API(a) 	if( m_fn_ ## a == NULL ) { \
		XEXCEPT_THROW_STR(XExcept_DB, _T("not found address '#a' in 'sqlite3'."), 0, 0);	\
		return false;\
	}


#ifdef UNICODE
#define XSQLITE3_OPEN(a,b) \
	((def_sqlite3_open16)m_fn_sqlite3_open16)(a, b)
#else
#define XSQLITE3_OPEN(a,b) \
	((def_sqlite3_open)m_fn_sqlite3_open)(a, b)
#endif


#ifdef UNICODE
#define XSQLITE3_EXEC(a,b,c,d,e)							\
	((def_sqlite3_exec)m_fn_sqlite3_exec)(a,XStrBuff(b),c,d,e)
#else
#define XSQLITE3_EXEC(a,b,c,d,e)							\
	((def_sqlite3_exec)m_fn_sqlite3_exec)(a,b,c,d,e)
#endif



#ifdef UNICODE
#define XSQLITE3_ERRMSG(a) \
	((def_sqlite3_errmsg16)m_fn_sqlite3_errmsg16)(a)
#else
#define XSQLITE3_ERRMSG(a) \
	((def_sqlite3_errmsg)m_fn_sqlite3_errmsg)(a)
#endif


#ifdef UNICODE
#define XSQLITE3_ERRMSG2(a) \
	(const wchar_t*)((def_sqlite3_errmsg16)m_pCon->m_fn_sqlite3_errmsg16)(a)
#else
#define XSQLITE3_ERRMSG2(a) \
	((def_sqlite3_errmsg)m_pCon->m_fn_sqlite3_errmsg)(a)
#endif


#ifdef UNICODE
#define XSQLITE3_PREPARE(a,b,c,d) \
	((def_sqlite3_prepare16)m_fn_sqlite3_prepare16)(a,b,wcslen(b), c, d)
#else
#define XSQLITE3_PREPARE(a,b,c,d) \
	((def_sqlite3_prepare)m_fn_sqlite3_prepare)(a,b,strlen(b), c, d)
#endif

#ifdef UNICODE
#define XSQLITE3_COLMN_NAME(a,b) \
	(const wchar_t*)((def_sqlite3_column_name16)m_pCon->m_fn_sqlite3_column_name16)(a,b)
#else
#define XSQLITE3_COLMN_NAME(a,b) \
	((def_sqlite3_column_name)m_pCon->m_fn_sqlite3_column_name)(a,b)
#endif


#ifdef UNICODE
#define XSQLITE3_BIND_TEXT(a,b,c,d) \
	((def_sqlite3_bind_text16)m_pCon->m_fn_sqlite3_bind_text16)(a,b, c, wcslen(c), d)
#else
#define XSQLITE3_BIND_TEXT(a,b,c,d) \
	((def_sqlite3_bind_text)m_pCon->m_fn_sqlite3_bind_text)(a,b, c, strlen(c), d)
#endif


//
//	xDbSQLite3Connection
//
xDbSQLite3Connection::xDbSQLite3Connection()
	:	m_pdbHandle( NULL ) 
	,	m_bUseUTF16( false )
	,	m_bNewDB( false )

	,	m_fn_sqlite3_open(NULL)
	,	m_fn_sqlite3_open16(NULL)
	,	m_fn_sqlite3_close(NULL)
	,	m_fn_sqlite3_exec(NULL)
	,	m_fn_sqlite3_errmsg(NULL)
	,	m_fn_sqlite3_errmsg16(NULL)
	,	m_fn_sqlite3_prepare(NULL)
	,	m_fn_sqlite3_prepare16(NULL)
	,	m_fn_sqlite3_finalize(NULL)
	,	m_fn_sqlite3_column_count(NULL)
	,	m_fn_sqlite3_column_type(NULL)
	,	m_fn_sqlite3_column_name(NULL)
	,	m_fn_sqlite3_column_name16(NULL)
	,	m_fn_sqlite3_bind_text(NULL)
	,	m_fn_sqlite3_bind_text16(NULL)
	,	m_fn_sqlite3_bind_int(NULL)
	,	m_fn_sqlite3_bind_double(NULL)
	,	m_fn_sqlite3_bind_blob(NULL)
	,	m_fn_sqlite3_bind_null(NULL)
	,	m_fn_sqlite3_column_text(NULL)
	,	m_fn_sqlite3_column_text16(NULL)
	,	m_fn_sqlite3_column_int(NULL)
	,	m_fn_sqlite3_column_double(NULL)
	,	m_fn_sqlite3_column_blob(NULL)
	,	m_fn_sqlite3_column_bytes(NULL)
	,	m_fn_sqlite3_step(NULL)
	,	m_fn_sqlite3_clear_bindings(NULL)
	,	m_fn_sqlite3_reset(NULL)


{
}


xDbSQLite3Connection::~xDbSQLite3Connection()
{
}
bool xDbSQLite3Connection::isNewDb() const
{
	return m_bNewDB;
}
bool xDbSQLite3Connection::open(const TCHAR* p_con_str, 
					const TCHAR* p_usr,
					const TCHAR* p_pwd)
{
XEXCEPT_ENTRY

	sqlite3*		db = NULL;
	int				err = 0;
	int				iExistDB = 0;
	struct _stat	buf_stat;
	XStrBuff		strUTF8Constr(_T(""));

	m_bNewDB = false;

	// DLLのロード
#ifdef WIN32
	if( m_mdl.open(_T("sqlite3.dll")) == false ) {
#else
	if( m_mdl.open(_T("sqlite3.so")) == false ) {
#endif
		XEXCEPT_THROW_STR(XExcept_DB, _T("fail to load 'sqlite3'."), 0, 0);
		return false;
	}
	
#define SQLITE_LOAD_SYMBOL(a)	m_fn_ ## a = ( def_ ## a ) m_mdl.loadSymbol( _T(#a) );\
	if( m_fn_ ## a == NULL ) { \
		TCHAR* pFuncName = _T(#a);	\
		XStrBuff strErrMsg;			\
		strErrMsg.Format(_T("not found address '%s' in 'sqlite3'."), pFuncName);\
		XEXCEPT_THROW_STR(XExcept_DB, (const TCHAR*)strErrMsg, 0, 0);	\
		return false;\
	}
#define SQLITE_LOAD_SYMBOL_IGNORE(a)	m_fn_ ## a = ( def_ ## a ) m_mdl.loadSymbol( _T(#a) );


	SQLITE_LOAD_SYMBOL(sqlite3_open);
	SQLITE_LOAD_SYMBOL(sqlite3_open16);
	SQLITE_LOAD_SYMBOL(sqlite3_errmsg);
	SQLITE_LOAD_SYMBOL(sqlite3_errmsg16);
	SQLITE_LOAD_SYMBOL(sqlite3_prepare);
	SQLITE_LOAD_SYMBOL(sqlite3_prepare16);
	SQLITE_LOAD_SYMBOL(sqlite3_bind_text);
	SQLITE_LOAD_SYMBOL(sqlite3_bind_text16);
	SQLITE_LOAD_SYMBOL(sqlite3_column_text);
	SQLITE_LOAD_SYMBOL(sqlite3_column_text16);
	SQLITE_LOAD_SYMBOL(sqlite3_column_name);
	SQLITE_LOAD_SYMBOL(sqlite3_column_name16);

	SQLITE_LOAD_SYMBOL(sqlite3_close);
	SQLITE_LOAD_SYMBOL(sqlite3_exec);
	SQLITE_LOAD_SYMBOL(sqlite3_finalize);
	SQLITE_LOAD_SYMBOL(sqlite3_column_count);
	SQLITE_LOAD_SYMBOL(sqlite3_column_type);
	SQLITE_LOAD_SYMBOL(sqlite3_bind_int);
	SQLITE_LOAD_SYMBOL(sqlite3_bind_blob);
	SQLITE_LOAD_SYMBOL(sqlite3_bind_double);
	SQLITE_LOAD_SYMBOL(sqlite3_bind_null);
	SQLITE_LOAD_SYMBOL(sqlite3_column_int);
	SQLITE_LOAD_SYMBOL(sqlite3_column_double);
	SQLITE_LOAD_SYMBOL(sqlite3_column_blob);
	SQLITE_LOAD_SYMBOL(sqlite3_column_bytes);
	SQLITE_LOAD_SYMBOL(sqlite3_step);
	SQLITE_LOAD_SYMBOL_IGNORE(sqlite3_clear_bindings);
	SQLITE_LOAD_SYMBOL(sqlite3_reset);



	// DBの存在確認
	if( _tcscmp(p_con_str, _T(":memory:")) != 0 ) {
		if( _tstat(p_con_str, &buf_stat ) == 0 ) {
			iExistDB = 1;
		}
	}


	// ファイル名、ディレクトリ名にUNICODE文字がある場合は、UTF8形式
	if( XStrBuffCodeConv::convertNativeToUTF8(p_con_str, strUTF8Constr, true) == false ) {
		return false;
	}

	err = XSQLITE3_OPEN(strUTF8Constr, &db);
	if (err != SQLITE_OK) {
		return false;
	}
	m_pdbHandle = db;
	if( iExistDB == 1 ) {
		return true;
	}

	m_bNewDB = true;

	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Connection::close()
{
XEXCEPT_ENTRY
	if( m_pdbHandle != NULL && m_fn_sqlite3_close != NULL) {
		int err = ((def_sqlite3_close)m_fn_sqlite3_close)((sqlite3*)m_pdbHandle);
	}
	m_pdbHandle = NULL;

	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Connection::execute(const TCHAR* p_sql)
{
XEXCEPT_ENTRY
	int err;
	sqlite3* db = (sqlite3*)m_pdbHandle;

	XSQLITE3_VALID_OPEN(m_pdbHandle);

	if( p_sql == NULL ) {
		return false;
	}


	err = XSQLITE3_EXEC(db, p_sql, NULL, NULL, NULL);
	if (err != SQLITE_OK) {
		XStrBuff strMsgBuff;

		strMsgBuff.Format(_T("%s. SQL:[%s]"),
			XSQLITE3_ERRMSG(db), p_sql);

		XEXCEPT_THROW_STR(XExcept_DB, (const TCHAR*)strMsgBuff, 0, 0);

		return false;
	}
	return true;
XEXCEPT_EXIT
	return false;
}

xDbStmt* xDbSQLite3Connection::getSmt(const TCHAR* p_sql)
{
XEXCEPT_ENTRY
	int err;
	sqlite3_stmt* psmt = NULL;
	sqlite3* db = (sqlite3*)m_pdbHandle;
	xDbSQLite3Stmt* pRet = NULL;

	XSQLITE3_VALID_OPEN(m_pdbHandle);

	err = XSQLITE3_PREPARE(db, p_sql, &psmt, NULL);
	if (err != SQLITE_OK) {
		XStrBuff strMsgBuff;

		strMsgBuff.Format(_T("%s. SQL:[%s]"),
			XSQLITE3_ERRMSG(db), p_sql);

		XEXCEPT_THROW_STR(XExcept_DB, (const TCHAR*)strMsgBuff, 0, 0);

		return NULL;
	}


	pRet = new xDbSQLite3Stmt();
	if( pRet == NULL ) {
		return NULL;
	}
	pRet->m_pstmt = psmt;
	pRet->m_pdb   = db;
	pRet->m_pCon  = this;

	return pRet;
XEXCEPT_EXIT
	return NULL;
}


bool xDbSQLite3Connection::finalizeStmt(xDbStmt* pStmt)
{
XEXCEPT_ENTRY
	xDbSQLite3Stmt* p = (xDbSQLite3Stmt*)pStmt;
	delete p;
	return true;
XEXCEPT_EXIT
	return false;
}



//
//
//
xDbSQLite3Stmt::xDbSQLite3Stmt()
	:	m_pstmt( NULL )
	,	m_pdb( NULL )
	,	m_pCon( NULL )
{

}

xDbSQLite3Stmt::~xDbSQLite3Stmt()
{
	if( m_pstmt != NULL ) {
		((def_sqlite3_finalize)m_pCon->m_fn_sqlite3_finalize)((sqlite3_stmt*)m_pstmt);
	}
}

int xDbSQLite3Stmt::getColumnCount()
{
XEXCEPT_ENTRY
	return ((def_sqlite3_column_count)m_pCon->m_fn_sqlite3_column_count)((sqlite3_stmt*)m_pstmt);
XEXCEPT_EXIT
}

XDATA_TYPE xDbSQLite3Stmt::getColumnType(int index)
{
XEXCEPT_ENTRY
	int type = ((def_sqlite3_column_type)m_pCon->m_fn_sqlite3_column_type)((sqlite3_stmt*)m_pstmt, index);
	switch( type ) {
	case SQLITE_INTEGER:
		return XDATA_TYPE_I4;
	case SQLITE3_TEXT:
		return XDATA_TYPE_A;
	case SQLITE_BLOB:
		return XDATA_TYPE_B;
	case SQLITE_FLOAT:
		return XDATA_TYPE_F8;
	case SQLITE_NULL:
		return XDATA_TYPE_NULL;
	default:
		break;
	}
	return XDATA_TYPE_END;
XEXCEPT_EXIT
}

bool xDbSQLite3Stmt::getColumnName(int index, XStrBuff &buf)
{
XEXCEPT_ENTRY
	const TCHAR* pret = XSQLITE3_COLMN_NAME((sqlite3_stmt*)m_pstmt, index);
	if( pret == NULL ) {
		XEXCEPT_THROW_STR(XExcept_DB, _T("SQL Access Error. column name index is invalid."), 0, 0);
	}
	buf = pret;
	return true;
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setText(int index, const char* val)
{
XEXCEPT_ENTRY
	int err = 0;

	err = XSQLITE3_BIND_TEXT((sqlite3_stmt*)m_pstmt, index, XStrBuff(val), SQLITE_TRANSIENT );
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setText(int index, const wchar_t* val)
{
XEXCEPT_ENTRY
	int err = 0;
	XStrBuff str(val);
	err = XSQLITE3_BIND_TEXT((sqlite3_stmt*)m_pstmt, index, str, SQLITE_TRANSIENT );
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setBool(int index, bool val)
{
XEXCEPT_ENTRY
	int err = 0;
	int iVal = (int)val;

	err = ((def_sqlite3_bind_int)m_pCon->m_fn_sqlite3_bind_int)((sqlite3_stmt*)m_pstmt, index, iVal );
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setInt1(int index, XD_INT1 val)
{
XEXCEPT_ENTRY
	return this->setInt4(index, (XD_INT4)val);
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setUInt1(int index, XD_UINT1 val)
{
XEXCEPT_ENTRY
	return this->setInt4(index, (XD_INT4)val);
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setInt2(int index, XD_INT2 val)
{
XEXCEPT_ENTRY
	return this->setInt4(index, (XD_INT4)val);
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setUInt2(int index, XD_UINT2 val)
{
XEXCEPT_ENTRY
	return this->setInt4(index, (XD_INT4)val);
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setInt4(int index, XD_INT4 val)
{
XEXCEPT_ENTRY
	int err = 0;

	err = ((def_sqlite3_bind_int)m_pCon->m_fn_sqlite3_bind_int)((sqlite3_stmt*)m_pstmt, index, val );
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setUInt4(int index, XD_UINT4 val)
{
XEXCEPT_ENTRY
	return this->setInt4(index, (XD_INT4)val);
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setInt8(int index, XD_INT8 val)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::setUInt8(int index, XD_UINT8 val)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
	return false;
}



bool xDbSQLite3Stmt::setFloat4(int index, XD_FLOAT4 val)
{
XEXCEPT_ENTRY
	return this->setFloat8(index, (XD_FLOAT8)val);
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::setFloat8(int index, XD_FLOAT8 val)
{
XEXCEPT_ENTRY
	int err = 0;

	err = ((def_sqlite3_bind_double)m_pCon->m_fn_sqlite3_bind_double)((sqlite3_stmt*)m_pstmt, index, val);
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}


	return true;
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::setBlob(int index, void* val, int size)
{
XEXCEPT_ENTRY
	int err = 0;

	err = ((def_sqlite3_bind_blob)m_pCon->m_fn_sqlite3_bind_blob)((sqlite3_stmt*)m_pstmt, index, val, size, SQLITE_TRANSIENT);
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}


	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::setDate(int index, const XTime* val)
{
XEXCEPT_ENTRY
	int err = 0;
	char	szTime[64];
	
	XStrBuff::__sprintf(szTime, XCOUNTOF(szTime), "%04d-%02d-%02d %02d:%02d:%02d",
		val->GetYear(), val->GetMonth(), val->GetDay(), 
		val->GetHour(), val->GetMinute(), val->GetSecond());
	err = ((def_sqlite3_bind_text)m_pCon->m_fn_sqlite3_bind_text)((sqlite3_stmt*)m_pstmt, index, szTime, strlen(szTime), SQLITE_TRANSIENT );
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}


	return true;
XEXCEPT_EXIT
	return false;
}



bool xDbSQLite3Stmt::setTime(int index, const XTime* val)
{
XEXCEPT_ENTRY
	int err = 0;
	char	szTime[64];

	XStrBuff::__sprintf(szTime, XCOUNTOF(szTime), "%04d-%02d-%02d %02d:%02d:%02d",
		val->GetYear(), val->GetMonth(), val->GetDay(), 
		val->GetHour(), val->GetMinute(), val->GetSecond());
	err = ((def_sqlite3_bind_text)m_pCon->m_fn_sqlite3_bind_text)((sqlite3_stmt*)m_pstmt, index, szTime, strlen(szTime), SQLITE_TRANSIENT );
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}


	return true;
XEXCEPT_EXIT
	return false;
}





bool xDbSQLite3Stmt::setTimeStamp(int index, const XTime* val)
{
XEXCEPT_ENTRY
	int err = 0;
	char	szTime[64];

	XStrBuff::__sprintf(szTime, XCOUNTOF(szTime), "%04d-%02d-%02d %02d:%02d:%02d",
		val->GetYear(), val->GetMonth(), val->GetDay(), 
		val->GetHour(), val->GetMinute(), val->GetSecond());
	err = ((def_sqlite3_bind_text)m_pCon->m_fn_sqlite3_bind_text)((sqlite3_stmt*)m_pstmt, index, szTime, strlen(szTime), SQLITE_TRANSIENT );
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}


	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::setNull(int index)
{
XEXCEPT_ENTRY
	int err = 0;

	err = ((def_sqlite3_bind_null)m_pCon->m_fn_sqlite3_bind_null)((sqlite3_stmt*)m_pstmt, index);
	if( err != SQLITE_OK ) {
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, index);
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::setRData(int index, XRichData* val)
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getText(int index, XStrBuff &buf)
{
XEXCEPT_ENTRY
	const char* pText = (const char*)((def_sqlite3_column_text)m_pCon->m_fn_sqlite3_column_text)((sqlite3_stmt*)m_pstmt, index);
	if( pText == NULL ) {
		return false;
	}
	buf = pText;
	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getBool(int index, bool* pVal)
{
XEXCEPT_ENTRY
	XD_INT4 iVal = 0;
	iVal = ((def_sqlite3_column_int)m_pCon->m_fn_sqlite3_column_int)((sqlite3_stmt*)m_pstmt, index);
	if( iVal == 0 ) {
		*pVal = false;
	} else {
		*pVal = true;
	}
	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getInt1(int index, XD_INT1* pVal)
{
XEXCEPT_ENTRY
	XD_INT4 iVal = 0;
	if( this->getInt4(index, &iVal) == false ) {
		return false;
	}
	*pVal = (XD_INT1)iVal;
	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getUInt1(int index, XD_UINT1* pVal)
{
XEXCEPT_ENTRY
	XD_INT4 iVal = 0;
	if( this->getInt4(index, &iVal) == false ) {
		return false;
	}
	*pVal = (XD_UINT1)iVal;
	return true;
XEXCEPT_EXIT
	return false;
}




bool xDbSQLite3Stmt::getInt2(int index, XD_INT2* pVal)
{
XEXCEPT_ENTRY
	XD_INT4 iVal = 0;
	if( this->getInt4(index, &iVal) == false ) {
		return false;
	}
	*pVal = (XD_INT2)iVal;
	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getUInt2(int index, XD_UINT2* pVal)
{
XEXCEPT_ENTRY
	XD_INT4 iVal = 0;
	if( this->getInt4(index, &iVal) == false ) {
		return false;
	}
	*pVal = (XD_UINT2)iVal;
	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getInt4(int index, XD_INT4* pVal)
{
XEXCEPT_ENTRY
	*pVal = ((def_sqlite3_column_int)m_pCon->m_fn_sqlite3_column_int)((sqlite3_stmt*)m_pstmt, index);
	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getUInt4(int index, XD_UINT4* pVal)
{
XEXCEPT_ENTRY
	XD_INT4 iVal = 0;
	if( this->getInt4(index, &iVal) == false ) {
		return false;
	}
	*pVal = (XD_UINT2)iVal;
	return true;
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::getInt8(int index, XD_INT8* pVal)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getUInt8(int index, XD_UINT8* pVal)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::getFloat4(int index, XD_FLOAT4* pVal)
{
XEXCEPT_ENTRY
	XD_FLOAT8 dVal = 0;
	if( this->getFloat8(index, &dVal) == false ) {
		return false;
	}
	*pVal = (XD_FLOAT4)dVal;
	return true;
XEXCEPT_EXIT
	return false;
}


bool xDbSQLite3Stmt::getFloat8(int index, XD_FLOAT8* pVal)
{
XEXCEPT_ENTRY
	*pVal = ((def_sqlite3_column_double)m_pCon->m_fn_sqlite3_column_double)((sqlite3_stmt*)m_pstmt, index);
	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getBlob(int index, XBuff& buf, int* p_size)
{
XEXCEPT_ENTRY
	size_t iSize = 0;
	void *pData = NULL;

	iSize = ((def_sqlite3_column_bytes)m_pCon->m_fn_sqlite3_column_bytes)((sqlite3_stmt*)m_pstmt, index);
	if( buf.GetBuffSize() < (size_t)iSize ) {
		pData = buf.Alloc( iSize );
	} else {
		pData = buf;
	}
	memcpy(pData, 
		((def_sqlite3_column_blob)m_pCon->m_fn_sqlite3_column_blob)((sqlite3_stmt*)m_pstmt, index),
		iSize);
	*p_size = iSize;
	return true;
XEXCEPT_EXIT
	return false;
}
bool xDbSQLite3Stmt::getDate(int index, XTime* val)
{
XEXCEPT_ENTRY
	const char* pTime = (const char*)((def_sqlite3_column_text)m_pCon->m_fn_sqlite3_column_text)((sqlite3_stmt*)m_pstmt, index);
	char* pEndPtr = NULL;
	char* pWorkPtr = NULL;
	int year, month, day, hour, minute, second;

	if( pTime == NULL ) {
		return false;
	}
	pWorkPtr = (char*)pTime;
	year = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	month = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	day = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ' ') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;

	
	hour = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	
	minute = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	

	second = (int)strtoul(pWorkPtr, &pEndPtr, 10);

	val->SetDateTime(year, month, day, hour, minute, second);
	return true;
XEXCEPT_EXIT
	return false;
}
bool xDbSQLite3Stmt::getTime(int index, XTime* val)
{
XEXCEPT_ENTRY
	const char* pTime = (const char*)((def_sqlite3_column_text)m_pCon->m_fn_sqlite3_column_text)((sqlite3_stmt*)m_pstmt, index);
	char* pEndPtr = NULL;
	char* pWorkPtr = NULL;
	int year, month, day, hour, minute, second;

	if( pTime == NULL ) {
		return false;
	}
	pWorkPtr = (char*)pTime;
	year = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	month = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	day = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ' ') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;

	
	hour = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	
	minute = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	

	second = (int)strtoul(pWorkPtr, &pEndPtr, 10);

	val->SetDateTime(year, month, day, hour, minute, second);
	return true;
XEXCEPT_EXIT
	return false;
}
bool xDbSQLite3Stmt::getTimeStamp(int index, XTime* val)
{
XEXCEPT_ENTRY
	const char* pTime = (const char*)((def_sqlite3_column_text)m_pCon->m_fn_sqlite3_column_text)((sqlite3_stmt*)m_pstmt, index);
	char* pEndPtr = NULL;
	char* pWorkPtr = NULL;
	int year, month, day, hour, minute, second;

	if( pTime == NULL ) {
		return false;
	}
	pWorkPtr = (char*)pTime;
	year = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	month = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	day = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ' ') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;

	
	hour = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	
	minute = (int)strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (char*)++pEndPtr;
	

	second = (int)strtoul(pWorkPtr, &pEndPtr, 10);

	val->SetDateTime(year, month, day, hour, minute, second);
	return true;
XEXCEPT_EXIT
	return false;
}
bool xDbSQLite3Stmt::isNull(int index, bool* pVal)
{
XEXCEPT_ENTRY
	*pVal = false;
	if( this->getColumnType(index) == XDATA_TYPE_NULL ) {
		*pVal = true;
	}
	return true;
XEXCEPT_EXIT
	return false;
}

bool xDbSQLite3Stmt::getRData(int index, XRichData* val)
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
	return false;
}

xDbStmt::STS xDbSQLite3Stmt::step()
{
XEXCEPT_ENTRY
	int err;

	err = ((def_sqlite3_step)m_pCon->m_fn_sqlite3_step)((sqlite3_stmt*)m_pstmt);
	switch( err ) {
	case SQLITE_ROW:
		return xDbStmt::ROW;
		break;
	case SQLITE_DONE:
		return xDbStmt::DONE;
		break;
	default:
		XEXCEPT_THROW_STR(XExcept_DB, XSQLITE3_ERRMSG2((sqlite3*)m_pdb), 0, 0);

		break;
	}
	return xDbStmt::ERR;
XEXCEPT_EXIT
}
bool xDbSQLite3Stmt::clearBinds()
{
XEXCEPT_ENTRY
	if( m_pCon->m_fn_sqlite3_clear_bindings != NULL ) {
		((def_sqlite3_clear_bindings)m_pCon->m_fn_sqlite3_clear_bindings)((sqlite3_stmt*)m_pstmt);
	}
	((def_sqlite3_reset)m_pCon->m_fn_sqlite3_reset)((sqlite3_stmt*)m_pstmt);
	return true;
XEXCEPT_EXIT
	return false;
}

