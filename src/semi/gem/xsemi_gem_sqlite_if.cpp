#include "xlib/semi/gem/xsemi_gem_sqlite_if.h"
#include "xlib/semi/gem/xsemi_gem_sqlite_grammer.c"
#include "xlib/semi/secs/xsecs_text_parser.h"
#include "xlib/semi/gem/xsemi_gem_def.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/xfile.h"
#include <sqlite/sqlite3.h>
#include <fstream>
#include <string>


class XLIB_API XSEMI_GemDataAtomSQLite: public XSEMI_GemDataAtom
{
public:
	XSEMI_GemDataAtomSQLite(sqlite3_stmt* select_sql);
	virtual ~XSEMI_GemDataAtomSQLite();

	virtual const TCHAR* getAtomName();
	virtual const TCHAR* getColmnName(int index);
	virtual XDATA_TYPE  getColmnType(int index);
	virtual bool        isColumnKey(int index){ _XASSERT(0);	return false; }
	virtual int getColmnCount();

	virtual const TCHAR* getString(const TCHAR* pKey);
	virtual XD_UINT4    getUint(const TCHAR* pKey);
	virtual XD_INT4     getInt(const TCHAR* pKey);
	virtual XD_FLOAT8   getDouble(const TCHAR* pKey);
	virtual bool        getBOOL(const TCHAR* pKey);
	virtual void        getRData(const TCHAR* pKey, XRichData& data);

	virtual const TCHAR* getString(int index);
	virtual XD_UINT4    getUint(int index);
	virtual XD_INT4     getInt(int index);
	virtual XD_FLOAT8   getDouble(int index);
	virtual bool        getBOOL(int index);
	virtual void        getRData(int index, XRichData& data);
private:
	int getIndex(const TCHAR* pKey);

	XStrBuffSplit	m_keys;
	sqlite3_stmt*	m_select_sql;
};

static const TCHAR* XSEMI_GemDataAtomSQLite_get_column_name(sqlite3_stmt* select_sql, int index)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
		return (const wchar_t*)sqlite3_column_name16(select_sql, index);
#else
		return sqlite3_column_name(select_sql, index);
#endif
XEXCEPT_EXIT
}

static const TCHAR* XSEMI_GemDataAtomSQLite_get_column_text(sqlite3_stmt* select_sql, int index)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
		return (const wchar_t*)sqlite3_column_text16(select_sql, index);
#else
		return (const char*)sqlite3_column_text(select_sql, index);
#endif
XEXCEPT_EXIT
}

static const TCHAR* XSEMI_GemDataAtomSQLite_sqlite3_errmsg(sqlite3* db) 
{
XEXCEPT_ENTRY
#ifdef _UNICODE
		return (const TCHAR*)sqlite3_errmsg16(db);
#else
		return sqlite3_errmsg(db);
#endif
XEXCEPT_EXIT
}

static int XSEMI_GemDataAtomSQLite_prepare(
  sqlite3 *db,            /* Database handle */
  const TCHAR *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const TCHAR **pzTail     /* OUT: Pointer to unused portion of zSql */
)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	int ret = sqlite3_prepare16(db, zSql, -1, ppStmt, (const void**)pzTail);
	if( *ppStmt == NULL ) {
		XTRACE(_T("SQL:%s\n"), zSql);
		XTRACE(_T("ERR:%d.  %s\n"), ret, XSEMI_GemDataAtomSQLite_sqlite3_errmsg(db));
	}
	return ret;
#else
		return sqlite3_prepare(db, zSql, nByte, ppStmt, pzTail);
#endif
XEXCEPT_EXIT
}


XSEMI_GemDataAtomSQLite::XSEMI_GemDataAtomSQLite(sqlite3_stmt* select_sql)
{
XEXCEPT_ENTRY
	m_select_sql = select_sql;
	int i;
	XStrBuff		strColumns(_T(""));
	int numOfColumn = sqlite3_column_count(m_select_sql);	// カラム数
	
	for(i = 0; i < numOfColumn; i++) {
		if( i != 0 ) {
			strColumns += _T(",");
		}
		strColumns += XSEMI_GemDataAtomSQLite_get_column_name(m_select_sql, i);
	}
	m_keys = (const TCHAR*)strColumns;
#if 0
	int iCnt            = m_keys.GetArgCouunt();
	const TCHAR** pArgv = m_keys.GetArgv();

	XTRACE(_T("GemDataAtomSQLite::XSEMI_GemDataAtomSQLite\n"));
	for(i = 0; i < iCnt; i++) {
		XTRACE(_T("  %02d:%s"), i, (const TCHAR*)XStrBuff(pArgv[i]));
		XTRACE(_T("\n"));
	}
#endif
XEXCEPT_EXIT
}
XSEMI_GemDataAtomSQLite::~XSEMI_GemDataAtomSQLite()
{
}
const TCHAR* XSEMI_GemDataAtomSQLite::getAtomName()
{
XEXCEPT_ENTRY
	return XSEMI_GemDataAtomSQLite_get_column_name(m_select_sql, 0);
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemDataAtomSQLite::getColmnName(int index)
{
XEXCEPT_ENTRY
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return m_keys.GetArgv()[index];
XEXCEPT_EXIT
}
XDATA_TYPE XSEMI_GemDataAtomSQLite::getColmnType(int index)
{
XEXCEPT_ENTRY
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	int type = sqlite3_column_type(m_select_sql, index);
	switch( type ) {
	case SQLITE_INTEGER:
		return XDATA_TYPE_U4;
		break;
	case SQLITE_FLOAT:
		return XDATA_TYPE_F8;
		break;
	case SQLITE3_TEXT:
		return XDATA_TYPE_A;
		break;
	case SQLITE_BLOB:
		return XDATA_TYPE_END;
		break;
	default:
		_XASSERT( 0 );
		break;
	}
	return XDATA_TYPE_END;
XEXCEPT_EXIT
}
int XSEMI_GemDataAtomSQLite::getColmnCount()
{
XEXCEPT_ENTRY
	return m_keys.GetArgCouunt();
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemDataAtomSQLite::getString(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return this->getString( getIndex( pKey ) );
XEXCEPT_EXIT
}
XD_UINT4    XSEMI_GemDataAtomSQLite::getUint(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return this->getUint( getIndex( pKey ) );
XEXCEPT_EXIT
}
XD_INT4     XSEMI_GemDataAtomSQLite::getInt(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return this->getInt( getIndex( pKey ) );
XEXCEPT_EXIT
}
XD_FLOAT8   XSEMI_GemDataAtomSQLite::getDouble(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return this->getDouble( getIndex( pKey ) );
XEXCEPT_EXIT
}
bool        XSEMI_GemDataAtomSQLite::getBOOL(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return this->getBOOL( getIndex( pKey ) );
XEXCEPT_EXIT
}
void        XSEMI_GemDataAtomSQLite::getRData(const TCHAR* pKey, XRichData& data)
{
XEXCEPT_ENTRY
	this->getRData( getIndex( pKey ), data );
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemDataAtomSQLite::getString(int index)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	int type = sqlite3_column_type(m_select_sql, index);
	if( type != SQLITE3_TEXT ) {
		XStrBuff strErr;
		strErr.Format(_T("different data_type. require txt, table column (%d). "), type);
		XEXCEPT_THROW_STR(XExcept_SQLite, (const TCHAR*)strErr, 0, index);
	}
	return XSEMI_GemDataAtomSQLite_get_column_text(m_select_sql, index);
XEXCEPT_EXIT
}
XD_UINT4    XSEMI_GemDataAtomSQLite::getUint(int index)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	int type = sqlite3_column_type(m_select_sql, index);
	if( type != SQLITE_INTEGER ) {
		XStrBuff strErr;
		strErr.Format(_T("different data_type. require txt, table column (%d). "), type);
		XEXCEPT_THROW_STR(XExcept_SQLite, (const TCHAR*)strErr, 0, index);
	}
	return sqlite3_column_int(m_select_sql, index);
XEXCEPT_EXIT
}
XD_INT4     XSEMI_GemDataAtomSQLite::getInt(int index)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	int type = sqlite3_column_type(m_select_sql, index);
	if( type != SQLITE_INTEGER ) {
		XStrBuff strErr;
		strErr.Format(_T("different data_type. require txt, table column (%d). "), type);
		XEXCEPT_THROW_STR(XExcept_SQLite, (const TCHAR*)strErr, 0, index);
	}
	return sqlite3_column_int(m_select_sql, index);
XEXCEPT_EXIT
}
XD_FLOAT8   XSEMI_GemDataAtomSQLite::getDouble(int index)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	int type = sqlite3_column_type(m_select_sql, index);
	if( type != SQLITE_INTEGER ) {
		XStrBuff strErr;
		strErr.Format(_T("different data_type. require txt, table column (%d). "), type);
		XEXCEPT_THROW_STR(XExcept_SQLite, (const TCHAR*)strErr, 0, index);
	}
	return sqlite3_column_double(m_select_sql, index);
XEXCEPT_EXIT
}
bool        XSEMI_GemDataAtomSQLite::getBOOL(int index)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	int type = sqlite3_column_type(m_select_sql, index);
	if( type != SQLITE_INTEGER ) {
		XStrBuff strErr;
		strErr.Format(_T("different data_type. require txt, table column (%d). "), type);
		XEXCEPT_THROW_STR(XExcept_SQLite, (const TCHAR*)strErr, 0, index);
	}
	return sqlite3_column_int(m_select_sql, index) == 0 ? false : true;
XEXCEPT_EXIT
}
void        XSEMI_GemDataAtomSQLite::getRData(int index, XRichData& data)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	int type = sqlite3_column_type(m_select_sql, index);

	switch( type ) {
	case SQLITE_INTEGER:
		switch( data.getType() ) {
		case XDATA_TYPE_U1:
			data.putValue( (XD_UINT1)sqlite3_column_int(m_select_sql, index) );
			break;
		case XDATA_TYPE_U2:
			data.putValue( (XD_UINT2)sqlite3_column_int(m_select_sql, index) );
			break;
		case XDATA_TYPE_U4:
			data.putValue( (XD_UINT4)sqlite3_column_int(m_select_sql, index) );
			break;
		case XDATA_TYPE_U8:
			data.putValue( (XD_UINT8)sqlite3_column_int(m_select_sql, index) );
			break;
		case XDATA_TYPE_I1:
			data.putValue( (XD_INT1)sqlite3_column_int(m_select_sql, index) );
			break;
		case XDATA_TYPE_I2:
			data.putValue( (XD_INT2)sqlite3_column_int(m_select_sql, index) );
			break;
		case XDATA_TYPE_I4:
			data.putValue( (XD_INT4)sqlite3_column_int(m_select_sql, index) );
			break;
		case XDATA_TYPE_I8:
			data.putValue( (XD_INT8)sqlite3_column_int(m_select_sql, index) );
			break;
		}
		break;
	case SQLITE_FLOAT:
		switch( data.getType() ) {
		case XDATA_TYPE_F4:
			data.putValue( (float)sqlite3_column_double(m_select_sql, index) );
			break;
		case XDATA_TYPE_F8:
			data.putValue( sqlite3_column_double(m_select_sql, index) );
			break;
		default:
			XEXCEPT_THROW_STR(XExcept_SQLite, 
				(const TCHAR*)XStrBuff::FormatS(_T("specified XRichData type incorrect. ColumnName(%s:SQLITE_FLOAT) Index(%d) Specified(%s)")
					,	(const TCHAR*)XSEMI_GemDataAtomSQLite_get_column_name(m_select_sql, index)
					,	index
					,	(const TCHAR*)XStrBuff( XData::getDataTypeStringA_From_TYPE( data.getType() ) )
				), 0, index);
			break;
		}
		break;
	case SQLITE3_TEXT:
		switch( data.getType() ) {
		case XDATA_TYPE_A:
		case XDATA_TYPE_UTF16:
		case XDATA_TYPE_J:
			data.putValueString( (const TCHAR*)XSEMI_GemDataAtomSQLite_get_column_text(m_select_sql, index) );
			break;
		default:
#ifdef _DEBUG
			XTRACE(_T("XSEMI_GemDataAtomSQLite::getRData\n"));
			for(int i = 0; i < sqlite3_column_count(m_select_sql); i++) {
				XTRACE(_T("  %02d:%s"), i, (const TCHAR*)XSEMI_GemDataAtomSQLite_get_column_name(m_select_sql, i));
				type = sqlite3_column_type(m_select_sql, i);

				switch( type ) {
				case SQLITE_INTEGER:
					XTRACE(_T("SQLITE_INTEGER"));
					break;
				case SQLITE_FLOAT:
					XTRACE(_T("SQLITE_FLOAT"));
					break;
				case SQLITE3_TEXT:
					XTRACE(_T("SQLITE3_TEXT"));
					break;
				case SQLITE_BLOB:
					XTRACE(_T("SQLITE_BLOB"));
					break;
				}
				XTRACE(_T("\n"));
			}
#endif

			XEXCEPT_THROW_STR(XExcept_SQLite, 
				(const TCHAR*)XStrBuff::FormatS(_T("specified XRichData type incorrect. ColumnName(%s:SQLITE3_TEXT) Index(%d) Specified(%s)")
					,	(const TCHAR*)XStrBuff( XSEMI_GemDataAtomSQLite_get_column_name(m_select_sql, index) )
					,	index
					,	(const TCHAR*)XStrBuff( XData::getDataTypeStringA_From_TYPE( data.getType() ) )
				), 0, index);
			break;
		}
		break;
	case SQLITE_BLOB:
		{
			XBIN* pVal  = (XBIN*)sqlite3_column_blob(m_select_sql, index);
			XD_UINT4       nSize = sqlite3_column_bytes(m_select_sql, index);
			if( nSize == 0 || pVal == NULL) {
				return ;
			}
			XData		dataB;
			XDataReader	reader(pVal, nSize);
			if( reader.getValue( &dataB ) == false ) {
				XEXCEPT_THROW_STR(XExcept_SQLite, 
					(const TCHAR*)XStrBuff::FormatS(_T("binary data has broken. ColumnName(%s:SQLITE_BLOB)  Index(%d)")
						,	(const TCHAR*)XStrBuff( XSEMI_GemDataAtomSQLite_get_column_name(m_select_sql, index) )
						,	index
					), 0, index);
				return ;
			}
			if( data.putValueFromXData(reader, dataB) == false ) {
				XEXCEPT_THROW_STR(XExcept_SQLite, 
					(const TCHAR*)XStrBuff::FormatS(_T("binary data has broken. ColumnName(%s:SQLITE_BLOB)  Index(%d)")
						,	(const TCHAR*)XStrBuff( XSEMI_GemDataAtomSQLite_get_column_name(m_select_sql, index) )
						,	index
					), 0, index);
				return ;
			}
		}
		break;
	case SQLITE_NULL:
		{
			data.clear();
		}
		break;
	default:
#ifdef _DEBUG
			XTRACE(_T("XSEMI_GemDataAtomSQLite::getRData\n"));
			for(int i = 0; i < sqlite3_column_count(m_select_sql); i++) {
				XTRACE(_T("  %02d:%s"), i, (const TCHAR*)XStrBuff(XSEMI_GemDataAtomSQLite_get_column_name(m_select_sql, i)));
				type = sqlite3_column_type(m_select_sql, i);

				switch( type ) {
				case SQLITE_INTEGER:
					XTRACE(_T("SQLITE_INTEGER"));
					break;
				case SQLITE_FLOAT:
					XTRACE(_T("SQLITE_FLOAT"));
					break;
				case SQLITE3_TEXT:
					XTRACE(_T("SQLITE3_TEXT"));
					break;
				case SQLITE_BLOB:
					XTRACE(_T("SQLITE_BLOB"));
					break;
				}
				XTRACE(_T("\n"));
			}
#endif
		_XASSERT( 0 );
		break;
	}
XEXCEPT_EXIT
}
int XSEMI_GemDataAtomSQLite::getIndex(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	int iCnt            = m_keys.GetArgCouunt();
	const TCHAR** pArgv = m_keys.GetArgv();
#ifdef _UNICODE
	XStrBuff strBuff(pKey);
	const wchar_t* pKeyDst = strBuff;
#else
	const char* pKeyDst = pKey;
#endif
	XStrBuff strErr;


	for(int i = 0; i < iCnt; i++) {
		if( XStrBuff::StrCmp(pKeyDst, pArgv[i]) == 0 ) {
			return i;
		}
	}
	strErr.Format(_T("not found key:[%s] AtomName:[%s]"), pKeyDst, this->getAtomName());
	XEXCEPT_THROW_STR(XExcept_XDATA, (const TCHAR*)strErr, 0, 0);
	return -1;
XEXCEPT_EXIT
}

/////////////////////////////////////////////////////////////////////////
//
//	XSEMI_GemSQLiteIF
//
/////////////////////////////////////////////////////////////////////////
XSEMI_GemSQLiteIF::XSEMI_GemSQLiteIF(const TCHAR* p_file_path)
	:	m_strFilePath(p_file_path)
	,	m_db(0)
	,	m_iTansactin( 0 )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XSEMI_GemSQLiteIF::~XSEMI_GemSQLiteIF()
{
XEXCEPT_ENTRY
	closeDB();
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemSQLiteIF::get_DBFilePath()
{
XEXCEPT_ENTRY
	return m_strFilePath;
XEXCEPT_EXIT
}


void XSEMI_GemSQLiteIF::openDB() 
{
XEXCEPT_ENTRY
	if( m_db != 0 ) {
		return ;
	}
	{
		sqlite3*		db;
		int				err;
#ifdef _UNICODE
		err = sqlite3_open16((const void*)(wchar_t*)m_strFilePath, &db);
#else
		err = sqlite3_open(m_strFilePath, &db);
#endif
		if (err != SQLITE_OK) {
			XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
		}
		m_db = (int)db;
	}
	
XEXCEPT_EXIT
}
void XSEMI_GemSQLiteIF::closeDB()
{
XEXCEPT_ENTRY
	if( m_db == 0 ) {
		return ;
	}
	{
		sqlite3_close((sqlite3*)m_db);
		m_db = 0;
	}
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::createNewDatabase()
{
XEXCEPT_ENTRY
	// データベースのオープン
	openDB();
	//
	//	トランザクション開始
	//
	beginTansction();

	// CONFIG
	this->execute(_T("drop table CONFIG;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_CONFIG);

	// CEID
	this->execute(_T("drop table CEID;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_CEID);

	// RPTID
	this->execute(_T("drop table RPTID;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_RPTID);

	// VARIABLE_GRP
	this->execute(_T("drop table VARIABLE_GRP;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_VARIABLE_GRP);

	// VID
	this->execute(_T("drop table VID;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_VID);

	// EC
	this->execute(_T("drop table EC;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_EC);

	// SV
	this->execute(_T("drop table SV;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_SV);

	// DV
	this->execute(_T("drop table DV;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_DV);

	// VVALS
	this->execute(_T("drop table VVALS;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_VVALS);
	// alter table VID add VValue blob;

	// CEID_RPTID
	this->execute(_T("drop table CEID_RPTID;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_CEID_RPTID);

	// RPTID_DVID
	this->execute(_T("drop table RPTID_DVID;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_RPTID_DVID);

	// ALARM
	this->execute(_T("drop table ALARM;"), false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_ALARM);

	// ACKCODES
	this->execute(XSEMI_SQL_GRM_DROP_TABLE_ACKCODES, false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_ACKCODES);


	// SPOOL_S
	this->execute(XSEMI_SQL_GRM_DROP_TABLE_SPOOL_SF, false );
	this->execute(XSEMI_SQL_GRM_CREATE_TABLE_SPOOL_SF);



	// デフォルトデータをセットする。
	setDefaultValues();

	endTansction();
	return true;
XEXCEPT_EXIT
}
bool XSEMI_GemSQLiteIF::load(XSEMI_GemConst& eventmap)
{
XEXCEPT_ENTRY

	int				err;
	TCHAR*			p_sql;
	sqlite3_stmt*	select_sql = NULL;
	XStrBuff		strSql;
	XSEMI_Gem*		pGem = NULL;
	XFileStatus		fStat;

	if( m_pConst == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object is not specified."), 0, 0);	
		return false;
	}

//	FILE *fp = NULL;
	if( XFile::GetStatus((const TCHAR*)m_strFilePath, fStat) == false ) {
//	if( XFile::fopenT(&fp, m_strFilePath, _T("r")) != 0 ) {
		// 存在しない場合は新規作成
		this->createNewDatabase();
	} else {
//		fclose(fp);
	}

	// データベースのオープン
	openDB();

	{
		XD_UINT4 ulVersion = 0;
		XRichData version(XDATA_TYPE_U4);
		if( this->get_ConfigValue(_T("version"), version) == true ) {
			ulVersion = version.getValueUInt4();
		}
		if( ulVersion < 2 ) {
			try {
				execute(_T("alter table VID add VValue blob"));
			} catch(...) {
			}
			version.putValue((XD_UINT4)2);
			this->set_ConfigValue(_T("version"), version);
		}
	}
	
	pGem = (XSEMI_Gem*)m_pConst->getParentByType( XSEMI_GemConstObject::GEM );


	
	/*
		CREATE TABLE CEID (	CeID INTEGER primary key, 
					EventName varchar(50), 
					Comment varchar(255), 
					Enable TINYINT);
	*/
	// CEIDの一覧取得
	{
		p_sql = (TCHAR*)XSEMI_SQL_GRM_SELECT_CEID;
		err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
		if (err != SQLITE_OK) {
			XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
		}
		XSEMI_GemDataAtomSQLite atomCEID(select_sql);
		while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
			XSEMI_CEID* pCEID = eventmap.get_CEIDs().add(&atomCEID);
			if( pCEID != NULL ) {
				pCEID->resetDirtyMember();
			}			
		}
		sqlite3_finalize(select_sql);
		select_sql = NULL;
	}

	/*
		CREATE TABLE RPTID (	RptID INTEGER primary key, 
					RptName varchar(255),
					Comment varchar(255), 
					Enable TINYINT);
	*/
	{
		p_sql = (TCHAR*)XSEMI_SQL_GRM_SELECT_RPTID;
		err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
		if (err != SQLITE_OK) {
			XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
		}
		XSEMI_GemDataAtomSQLite atomRpt(select_sql);
		while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
			XSEMI_Rpt* pRpt = eventmap.get_Rpts().add(&atomRpt);
			if( pRpt != NULL ) {
				pRpt->resetDirtyMember();
			}			
		}
		sqlite3_finalize(select_sql);
		select_sql = NULL;
	}


	//-----------------------------------------------------------------------
	// VariableGrpの取得
	//-----------------------------------------------------------------------
	{
		p_sql =	(TCHAR*)XSEMI_SQL_GRM_SELECT_VARIABLE_GRP;
		err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
		if (err != SQLITE_OK) {
			XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
		}
		XSEMI_GemDataAtomSQLite atomVG(select_sql);
		while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
			XSEMI_VARIABLE_GRP *pVg = eventmap.get_VARIABLE_GRPs().add( &atomVG );
			if( pVg != NULL ) {
				pVg->resetDirtyMember();
			}			
		}
		sqlite3_finalize(select_sql);
		select_sql = NULL;
	}

	/*
	CREATE TABLE VID (	VID INTEGER primary key, 
				Name		varchar(50),
				DisplayName	varchar(50),
				DataUnit	varchar(50),
				DataType	INTEGER,
				Mode		INTEGER,
				InitValue	varchar(256),
				Category	INTEGER,
				Comment		varchar(255),			
				Enable		TINYINT);
	*/


	//-----------------------------------------------------------------------
	// ECの取得
	//-----------------------------------------------------------------------
	/*
	CREATE TABLE EC (	ECID INTEGER primary key, 
				EcMax		varchar(256),
				EcMin		varchar(256),
				EcDef		varchar(256),
				EcVal		varchar(256));

	select		VID.VID			, VID.Name		, VID.DisplayName 
			,	VID.DataUnit	, VID.DataType	, VID.Mode 
			,	VID.InitValue	, VID.Category	, VID.Comment 
			,	VID.Enable		
			,	EC.EcMax		, EC.EcMin		, EC.EcDef			, EC.EcVal
	from VID,EC 
	where	VID.Category = 1 
		and	VID.VID = EC.ECID;
	*/
	strSql = XSEMI_SQL_GRM_SELECT_EC;
	if( pGem ) {
		if( pGem->get_isHost() == true ) {
			strSql += _T(" and VID.VPRIVATE != 1");
		} else {
			strSql += _T(" and VID.VPRIVATE != 2");
		}
	}
	strSql += _T(";");
	p_sql =	(TCHAR*)(const TCHAR*)strSql;
	err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	XSEMI_GemDataAtomSQLite atomEC(select_sql);
	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		XSEMI_V *pV = eventmap.get_Vs().addNoDetail( &atomEC );
		if( pV != NULL ) {
			pV->resetDirtyMember();
		}
		
	}
	sqlite3_finalize(select_sql);
	select_sql = NULL;

	//-----------------------------------------------------------------------
	// SVの取得
	//-----------------------------------------------------------------------
	/*
	CREATE TABLE SV (	SVID INTEGER primary key, 
				SizeMax		INTEGER,
				SizeMin		INTEGER,
				LimitMax	varchar(256),
				LimitMin	varchar(256),
				CeLimits	INTEGER);

	select		VID.VID			, VID.Name		, VID.DisplayName 
			,	VID.DataUnit	, VID.DataType	, VID.Mode 
			,	VID.InitValue	, VID.Category	, VID.Comment 
			,	VID.Enable		
			,	SV.SizeMax		, SV.SizeMin	, SV.LimitMax 
			,	SV.LimitMin		, SV.CeLimits
	from VID,SV 
	where	VID.Category = 0
		and	VID.VID = SV.SVID;
	*/
	strSql = XSEMI_SQL_GRM_SELECT_SV;
	if( pGem ) {
		if( pGem->get_isHost() == true ) {
			strSql += _T(" and VID.VPRIVATE != 1");
		} else {
			strSql += _T(" and VID.VPRIVATE != 2");
		}
	}
	strSql += _T(";");
	p_sql =	(TCHAR*)(const TCHAR*)strSql;
	err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	XSEMI_GemDataAtomSQLite atomSV(select_sql);
	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		XSEMI_V *pV = eventmap.get_Vs().addNoDetail( &atomSV );
		if( pV != NULL ) {
			pV->resetDirtyMember();
		}
		
	}
	sqlite3_finalize(select_sql);
	select_sql = NULL;



	//-----------------------------------------------------------------------
	// DVの取得
	//-----------------------------------------------------------------------
	/*
	CREATE TABLE DV (	DVID INTEGER primary key, 
				SizeMax		INTEGER,
				SizeMin		INTEGER);

	select		VID.VID			, VID.Name		, VID.DisplayName 
			,	VID.DataUnit	, VID.DataType	, VID.Mode 
			,	VID.InitValue	, VID.Category	, VID.Comment 
			,	VID.Enable		
			,	DV.SizeMax		, DV.SizeMin
	from VID,DV 
	where	VID.Category = 2
		and	VID.VID = DV.DVID;
	*/
	strSql = XSEMI_SQL_GRM_SELECT_DV;
	if( pGem ) {
		if( pGem->get_isHost() == true ) {
			strSql += _T(" and VID.VPRIVATE != 1");
		} else {
			strSql += _T(" and VID.VPRIVATE != 2");
		}
	}
	strSql += _T(";");
	p_sql =	(TCHAR*)(const TCHAR*)strSql;
	err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	XSEMI_GemDataAtomSQLite atomDV(select_sql);
	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		XSEMI_V *pV = eventmap.get_Vs().addNoDetail( &atomDV );
		if( pV != NULL ) {
			pV->resetDirtyMember();
		}
		
	}
	sqlite3_finalize(select_sql);
	select_sql = NULL;


	/*
		CREATE TABLE CEID_RPTID (	CeID INTEGER primary key, 
					RptIdSeq INTEGER primary key,
					RptID INTEGER,
					Option TINYINT,
					Comment varchar(255),			
					Enable TINYINT,
					primary key(RptID, DvIdSeq));
	*/
	p_sql =	(TCHAR*)XSEMI_SQL_GRM_SELECT_CEID_RPTID;
	err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		XD_UINT4 ceid   = (XD_UINT4)sqlite3_column_int(select_sql, 0);
		XD_UINT4 rptSeq = (XD_UINT4)sqlite3_column_int(select_sql, 1);
		XD_UINT4 rptID  = (XD_UINT4)sqlite3_column_int(select_sql, 2);

		if( eventmap.setRpt(ceid, rptID) == NULL ) {
//			return false;
		}
	}
	sqlite3_finalize(select_sql);
	select_sql = NULL;


	/*
		CREATE TABLE RPTID_DVID (	RptID INTEGER ,
					DvIdSeq INTEGER,
					DvID INTEGER,
					Enable TINYINT,
					primary key(RptID, DvIdSeq));
	*/
	p_sql =	(TCHAR*)XSEMI_SQL_GRM_SELECT_RPTID_DVID;
	err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		XD_UINT4 ulRptID     = (XD_UINT4)sqlite3_column_int(select_sql, 0);
		XD_UINT4 ulDvIdSeq   = (XD_UINT4)sqlite3_column_int(select_sql, 1);
		XD_UINT4 ulDvID   = (XD_UINT4)sqlite3_column_int(select_sql, 2);

		if( eventmap.defineReport(ulRptID, ulDvID) == false ) {
//			return false;
		}
	}
	sqlite3_finalize(select_sql);
	select_sql = NULL;


	/*
		CREATE TABLE ALARM (	ALID INTEGER, 
						ALCD INTEGER, 
						AlarmName varchar(255),
						AlarmText varchar(255),
						OnCEID INTEGER, 
						OffCEID INTEGER, 
						Comment varchar(255),
						Enable TINYINT,
						primary key(ALID)
					);
	*/
	p_sql =	(TCHAR*)XSEMI_SQL_GRM_SELECT_ALARM;
	err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	XSEMI_GemDataAtomSQLite atomAlarm(select_sql);
	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		XSEMI_ALARM* pAlarm = eventmap.get_Alarms().add( &atomAlarm );//(alid, p_name);
		if( pAlarm != NULL ) {
			pAlarm->resetDirtyMember();
		}
	}
	sqlite3_finalize(select_sql);
	select_sql = NULL;


	// Spool SF
	p_sql =	(TCHAR*)XSEMI_SQL_GRM_SELECT_SPOOL_SF;
	err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	XSEMI_GemDataAtomSQLite atomSpoolSF(select_sql);
	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		XSEMI_SpoolSF* pSpool = eventmap.get_SpoolSFs().add( &atomSpoolSF );
		if( pSpool != NULL ) {
			pSpool->resetDirtyMember();
		}
	}
	sqlite3_finalize(select_sql);
	select_sql = NULL;


	// 初期取得時は、Dirtyフラグを全てリセットする。
	eventmap.resetDirtyAllItem();

	return true;
XEXCEPT_EXIT
}


bool XSEMI_GemSQLiteIF::get_ConfigValue(const TCHAR* pszConfName, XRichData &data, XStrBuff* pComment /*= NULL*/)
{
XEXCEPT_ENTRY
	int				err;
	sqlite3_stmt*	select_sql = NULL;
	TCHAR			szSQL[256];
	bool			bFind = false;
	XStrBuff		strError;
	XStrBuffSQL		strConfName(pszConfName);



	// データベースのオープン
	openDB();

	// Select...
	XStrBuff::printf(szSQL, XCOUNTOF(szSQL), _T("select DataType,ConfData,Comment from CONFIG where ConfName='%s'")
		, (const TCHAR*)strConfName);
	err = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, szSQL, XStrBuff::StrLen(szSQL), &select_sql, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		XDATA_TYPE emDataType   = (XDATA_TYPE)  sqlite3_column_int(select_sql, 0);
		const TCHAR* pValue      = (const TCHAR *)XSEMI_GemDataAtomSQLite_get_column_text(select_sql, 1);
		const TCHAR* pStrComment = (const TCHAR *)XSEMI_GemDataAtomSQLite_get_column_text(select_sql, 2);
		XStrBuff    buff(pValue);
		TCHAR*       pBuff;
		size_t      iSize;




		// 文字列の値を変換する。
		XSecsTextParser textParser;
		XRichData srcData(emDataType);

		try {
			// データタイプをチェックする。
			switch( emDataType ) {
			case XDATA_TYPE_A:
			case XDATA_TYPE_J:
				srcData.putValueString((const char*)buff);
				break;
			case XDATA_TYPE_UTF16:
				srcData.putValueStringUTF16(buff);
				break;
			case XDATA_TYPE_B:
				buff += _T(">");
				iSize = buff.GetLength();
				pBuff = buff;
				textParser.parseBinaryFromString(&pBuff, iSize, &srcData);
				break;
			case XDATA_TYPE_BOOL:
				buff += _T(">");
				iSize = buff.GetLength();
				pBuff = buff;
				textParser.parseBoolromString(&pBuff, iSize, &srcData);
				break;
			case XDATA_TYPE_U1:
			case XDATA_TYPE_U2:
			case XDATA_TYPE_U4:
			case XDATA_TYPE_U8:
			case XDATA_TYPE_I1:
			case XDATA_TYPE_I2:
			case XDATA_TYPE_I4:
			case XDATA_TYPE_I8:
			case XDATA_TYPE_F4:
			case XDATA_TYPE_F8:
				buff += _T(">");
				iSize = buff.GetLength();
				pBuff = buff;
				textParser.parseDigitFromString(&pBuff, iSize, &srcData);
				break;
			default:
				strError.Format(_T("invalid DataType[%u], ConfName='%s'"), emDataType, XStrBuff(pszConfName));
				XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strError, 0, err);
				break;
			}
		} catch(XExcept &e) {
			sqlite3_finalize(select_sql);
			select_sql = NULL;
			XEXCEPT_RETHROW(e, XExcept_RETHROW);
		}

		data.copy( &srcData );
		if( pComment != NULL && pStrComment != NULL ) {
			*pComment = pStrComment;
		}
		bFind = true;
	}
	sqlite3_finalize(select_sql);
	select_sql = NULL;
	return bFind;
XEXCEPT_EXIT
}
bool XSEMI_GemSQLiteIF::set_ConfigValue(const TCHAR* pszConfName, XRichData &data, const TCHAR* lpszComment /*= NULL*/)
{
XEXCEPT_ENTRY
	XRichData exitData(XDATA_TYPE_L);
	bool		bFind = false;
	XStrBuff	strSQL;
	int			err;
	// データベースのオープン
	openDB();

	// 既に存在しているか確認。
	try {
		bFind = get_ConfigValue(pszConfName, exitData);
	} catch(XExcept &e) {
		if( e.getCategory() != XExcept_SECS ) {
			// カテゴリーがSECSレベルでない場合は、リスロー
			XEXCEPT_RETHROW(e, XExcept_RETHROW);
		}
	}

	XStrBuff strValue = data.toString();
	if( bFind == false ) {
		strSQL.Format(_T("insert into CONFIG(ConfName,ConfData,DataType,Comment) values('%s','%s',%d,'%s');")
			,			(const TCHAR*)XStrBuffSQL(pszConfName)
			,			(const TCHAR*)XStrBuffSQL((const TCHAR*)strValue)
			,			data.getType()
			,			lpszComment == NULL ? (const TCHAR*)_T("") : (const TCHAR*)XStrBuffSQL(lpszComment)
			);
	} else {
		strSQL.Format(_T("update CONFIG set ConfData='%s',DataType=%d,Comment='%s' where ConfName='%s';")
			,			(const TCHAR*)XStrBuffSQL((const TCHAR*)strValue)
			,			data.getType()
			,			lpszComment == NULL ? (const TCHAR*)_T("") : (const TCHAR*)XStrBuffSQL(lpszComment)
			,			(const TCHAR*)XStrBuffSQL(pszConfName)
			);
	}

	err = sqlite3_exec((sqlite3*)m_db,	strSQL, NULL, NULL, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	return true;
XEXCEPT_EXIT
}




bool XSEMI_GemSQLiteIF::deleteCEID_RptID(XD_UINT4 ceid, XD_UINT4 rptid)
{
XEXCEPT_ENTRY
	// データベースのオープン
	openDB();
	//	トランザクション開始
	beginTansction();
	try {
		// CEIDに緋も付くReptIDを削除
		innerdeleteCEID_RptID(ceid, rptid);

		//	コミット
		endTansction();
	} catch ( XExcept &ex ) {
		rollBackTansction();
		XEXCEPT_RETHROW(ex, XExcept_RETHROW); 
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::loadVVAlS(XSEMI_V* pVID)
{
XEXCEPT_ENTRY
	int				err;
	TCHAR*			p_sql;
	sqlite3_stmt*	select_sql = NULL;
	XStrBuff		strSql;

	// データベースのオープン
	openDB();
	try {
		strSql.Format(_T("select * from VVALS where VID=%d;"), pVID->get_VID());
		


		p_sql = (TCHAR*)(const TCHAR*)strSql;
		XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
		XSEMI_GemDataAtomSQLite atom(select_sql);
		while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
			XSEMI_VVAL* pVVal = pVID->addVVal( &atom );
			//pVVal->resetDirtyMember();
		}
		sqlite3_finalize(select_sql);
		select_sql = NULL;
	} catch ( XExcept &ex ) {
		XEXCEPT_RETHROW(ex, XExcept_RETHROW); 
	}
	return true;
XEXCEPT_EXIT
	return false;
}





//
//	CSVファイルからデータをインポートする。
//
bool XSEMI_GemSQLiteIF::importCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader)
{
XEXCEPT_ENTRY
	XFile		 file;
	int iLineCount = 0;
	XStrBuff		strErrMsg;
	bool			bAll = (tableType == XSEMI_GemMstData::TABLE_TYPE_ALL) ? true : false;
	XSEMI_GemDataAtomCSV* pCVSData = NULL;
	XExcept			errEx(XExcept_SECS, _T("import from CSV error."), __FILE__, __LINE__);
	bool			bError = false;
	XSEMI_Gem*		pGem;
	if( m_pConst == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object is not specified."), 0, 0);	
		return false;
	}
	pGem = (XSEMI_Gem*)m_pConst->getParentByType( XSEMI_GemConstObject::GEM );


	// データベースのオープン
	openDB();
	//	トランザクション開始
	beginTansction();

	try {
		if( file.Open(p_file, true, false, false, true) == false ) {
			strErrMsg.Format(_T("fail to opne file(%s)."), p_file);
			XEXCEPT_THROW_STR(XExcept_FILE, (const TCHAR*)strErrMsg, 0, 0);
			return false;
		}
		XStrBuff	 strLine;
		while( file.ReadLine( strLine ) == true ) {
			iLineCount++;
			if( iLineCount == 0 && bExitHeader == true && bAll == false) {
				continue;
			}

			strLine.Trim( XStrBuff::TRIM_RIGHTONLY );
			if( strLine.GetLength() == 0 ) {
				continue;
			}

			if( bAll == true ) {
				const TCHAR* pClmn = strLine;
				if( *pClmn == _T('\"') ) {
					pClmn++;
				}
				if( *pClmn == _T('[') ) {
					XStrBuff strAtomName;
					if(  XStrBuff::StrNCmp(pClmn, _T("[CEID]"), XStrBuff::StrLen(_T("[CEID]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_CEID;
						strLine.Replace(_T("[CEID]"), _T("CeID"));

						strAtomName = _T("CEID");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[RPTID]"), XStrBuff::StrLen(_T("[RPTID]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_RPTID;
						strLine.Replace(_T("[RPTID]"), _T("RptID"));

						strAtomName = _T("RPTID");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[EC]"), XStrBuff::StrLen(_T("[EC]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_VID_EC;
						strLine.Replace(_T("[EC]"), XSEMI_DATA_CLMN_VID);

						strAtomName = _T("EC");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[SV]"), XStrBuff::StrLen(_T("[SV]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_VID_SV;
						strLine.Replace(_T("[SV]"), XSEMI_DATA_CLMN_VID);

						strAtomName = _T("SV");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[DV]"), XStrBuff::StrLen(_T("[DV]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_VID_DV;
						strLine.Replace(_T("[DV]"), XSEMI_DATA_CLMN_VID);

						strAtomName = _T("DV");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[CEID_RPTID]"), XStrBuff::StrLen(_T("[CEID_RPTID]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_CEID_RPTID;
						strLine.Replace(_T("[CEID_RPTID]"), XSEMI_DATA_CLMN_CeID);


						strAtomName = _T("CEID_RPTID");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[RPTID_DVID]"), XStrBuff::StrLen(_T("[RPTID_DVID]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_RPTID_DVID;
						strLine.Replace(_T("[RPTID_DVID]"), XSEMI_DATA_CLMN_RptID);

						strAtomName = _T("RPTID_DVID");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[ALARM]"), XStrBuff::StrLen(_T("[ALARM]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_ALARM;
						strLine.Replace(_T("[ALARM]"), _T("ALID"));

						strAtomName = _T("ALARM");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[VARIABLE_GRP]"), XStrBuff::StrLen(_T("[VARIABLE_GRP]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_VARIABLE_GRP;
						strLine.Replace(_T("[VARIABLE_GRP]"), _T("GRPID"));

						strAtomName = _T("VARIABLE_GRP");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[ACKCODES]"), XStrBuff::StrLen(_T("[ACKCODES]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_ACKCODES;
						strLine.Replace(_T("[ACKCODES]"), _T("ACKKIND"));

						strAtomName = _T("ACKCODES");
					} else
					if(  XStrBuff::StrNCmp(pClmn, _T("[VVALS]"), XStrBuff::StrLen(_T("[VVALS]"))) == 0 ) {
						tableType = XSEMI_GemMstData::TABLE_TYPE_VVAL;
						strLine.Replace(_T("[VVALS]"), _T("VID"));

						strAtomName = _T("VVALS");
					} else {
						XEXCEPT_THROW_STR(XExcept_SECS, _T("invalid column."), 0, iLineCount);
					}


					if( pCVSData ) {
						delete pCVSData;
					}
					pCVSData = new XSEMI_GemDataAtomCSV( strLine, strAtomName );
					continue;
				}
			}

			if( pCVSData != NULL ) {
				pCVSData->setValues( strLine ); 

				if( pCVSData->getColmnCount() == 0 ) {
					// 空行
					continue;
				}
				if( XStrBuff::StrCmp( pCVSData->getString(0), _T("")) == 0 ) {
					// 第一カラムにはデータ必須
					continue;
				}


				switch( tableType ) {
				case XSEMI_GemMstData::TABLE_TYPE_CEID:
					{
						XSEMI_CEID *pNewInstance = m_pConst->get_CEIDs().add( pCVSData );
						if( pNewInstance == NULL ) {
							errEx.addStackTrace(XExcept_SECS, __FILE__, __LINE__, 0, 0, (const TCHAR*)
								XStrBuff::FormatS(_T("fail to add CEID:%d"), pCVSData->getUint( 0 ) ) );
							bError = true;
						} else {
							innerUpdateCEID((void*)m_db, pNewInstance);
						}
					}
					break;
				case XSEMI_GemMstData::TABLE_TYPE_RPTID:
					{
						XSEMI_Rpt *pNewInstance = m_pConst->get_Rpts().add( pCVSData );
						if( pNewInstance == NULL ) {
							errEx.addStackTrace(XExcept_SECS, __FILE__, __LINE__, 0, 0, (const TCHAR*)
								XStrBuff::FormatS(_T("fail to add RPTID:%d"), pCVSData->getUint( 0 ) ) );
							bError = true;
						} else {
							innerUpdateRpt((void*)m_db, pNewInstance);
						}
					}
					break;
				case XSEMI_GemMstData::TABLE_TYPE_VID_EC:
				case XSEMI_GemMstData::TABLE_TYPE_VID_SV:
				case XSEMI_GemMstData::TABLE_TYPE_VID_DV:
					{
						XSEMI_V *pNewInstance = m_pConst->get_Vs().add( pCVSData );
						if( pNewInstance == NULL ) {
							errEx.addStackTrace(XExcept_SECS, __FILE__, __LINE__, 0, 0, (const TCHAR*)
								XStrBuff::FormatS(_T("fail to add VID:%d"), pCVSData->getUint( 0 ) ) );
							bError = true;
						} else {
							innerUpdateV((void*)m_db, pNewInstance);
						}
					}
					break;
				case XSEMI_GemMstData::TABLE_TYPE_CEID_RPTID:
					innerUpdateCEID_RPTID((sqlite3*)m_db, pCVSData);
					break;
				case XSEMI_GemMstData::TABLE_TYPE_RPTID_DVID:
					innerUpdateRPTID_DVID((sqlite3*)m_db, pCVSData);
					break;
				case XSEMI_GemMstData::TABLE_TYPE_ALARM:
					{

						XSEMI_ALARM *pNewInstance = m_pConst->get_Alarms().add( pCVSData );
						if( pNewInstance == NULL ) {
							errEx.addStackTrace(XExcept_SECS, __FILE__, __LINE__, 0, 0, (const TCHAR*)
								XStrBuff::FormatS(_T("fail to add ALARM:%d"), pCVSData->getUint( 0 ) ) );
							bError = true;
						} else {
							innerUpdateAlaram((void*)m_db, pNewInstance);
						}
					}
					break;
				case XSEMI_GemMstData::TABLE_TYPE_VARIABLE_GRP:
					{
						XSEMI_VARIABLE_GRP* pNewInstance = m_pConst->get_VARIABLE_GRPs().add( pCVSData );
						if( pNewInstance == NULL ) {
							errEx.addStackTrace(XExcept_SECS, __FILE__, __LINE__, 0, 0, (const TCHAR*)
								XStrBuff::FormatS(_T("fail to add Variable Group:%d"), pCVSData->getUint( 0 ) ) );
							bError = true;
						} else {
							innerUpdateVariableGRP((void*)m_db, pNewInstance);
						}
					}
					break;
				case XSEMI_GemMstData::TABLE_TYPE_ACKCODES:
					{
						XSEMI_ACKCODE* pNewInstance = m_pConst->get_AckCodes().add( pCVSData );
						if( pNewInstance == NULL ) {
							errEx.addStackTrace(XExcept_SECS, __FILE__, __LINE__, 0, 0, (const TCHAR*)
								XStrBuff::FormatS(_T("fail to add ACKCODE:%d"), pCVSData->getUint( 0 ) ) );
							bError = true;
						} else {
							this->updateObject( pNewInstance );
						}
					}
					break;
				case XSEMI_GemMstData::TABLE_TYPE_VVAL:
					{
						XSEMI_V *pVID = NULL;
						XStrBuff strID = pCVSData->getString(0);
						if( XStrBuff::isDigitsEx( strID ) == 0 ) {
							pVID = m_pConst->get_Vs().get_byName( strID );
						} else {
							pVID = m_pConst->get_Vs().get_byID( pCVSData->getUint(0) );
						}						
						if( pVID != NULL ) {
							XSEMI_VVAL* pNewInstance = pVID->addVVal( pCVSData );
							if( pNewInstance == NULL ) {
								errEx.addStackTrace(XExcept_SECS, __FILE__, __LINE__, 0, 0, (const TCHAR*)
									XStrBuff::FormatS(_T("fail to add VVAL:%d"), pCVSData->getUint( 0 ) ) );
								bError = true;
							} else {
								this->updateObject( pNewInstance );
							}
						}
					}
					break;
				}
			}
		}
		if( pCVSData ) {
			delete pCVSData;
		}
		file.Close();

		//	コミット
		endTansction();
	} catch ( XExcept &ex ) {
		rollBackTansction();
		strErrMsg.Format(_T("File:[%s] Line:%d"), p_file, iLineCount);
		do { 
			ex.addStackTrace(XExcept_RETHROW, __FILE__, __LINE__,0,iLineCount, strErrMsg); 
			throw; 
		} while (0);
	}
//	if( bError == true ) {
//		throw errEx;
//	}
	return true;
XEXCEPT_EXIT
}


bool XSEMI_GemSQLiteIF::innerExportCSV(FILE* fp, const TCHAR* pSQL, const TCHAR* pHeaderFirstClmn /*= NULL*/)
{
XEXCEPT_ENTRY
	int				err = 0;
	sqlite3_stmt*	select_sql = NULL;
	XStrBuff		strErrMsg;
	int				iDataTypePos = -1;

	XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, pSQL, XStrBuff::StrLen(pSQL), &select_sql, NULL);
	
	int numOfColumn = sqlite3_column_count(select_sql);
	
	if( pHeaderFirstClmn != NULL ) {
		// ヘッダー書き込み
		for(int i = 0; i < numOfColumn; i++) {
			if( i == 0 ) { 
				XFile::fprintfT(fp, _T("[%s]"), pHeaderFirstClmn);
			} else {
				const TCHAR* pCplumName = XSEMI_GemDataAtomSQLite_get_column_name(select_sql, i);
				if( XStrBuff::StrCmp(pCplumName, _T("DataType")) == 0 ) {
					// データタイプは文字列に変換して出力する。
					iDataTypePos = i;
				}
				XFile::fprintfT(fp, _T(",%s"), pCplumName);
			}
		}
		XFile::fprintfT(fp, _T("\n"));
	}


	while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
		for(int i = 0; i < numOfColumn; i++) {
			int type = sqlite3_column_type(select_sql, i);

			if( i != 0 ) {
				XFile::fprintfT(fp, _T(","));
			}

			switch( type ) {
			case SQLITE_INTEGER:
				{
					XD_UINT4 val              = (XD_UINT4)sqlite3_column_int(select_sql,     i);
					if( iDataTypePos == i ) {
						XFile::fprintfT(fp, XData::getDataTypeString_From_TYPE((XDATA_TYPE)val));
					} else {
						XFile::fprintfT(fp, _T("%u"), val);
					}
				}
				break;
			case SQLITE_FLOAT:
				{
					XD_FLOAT8 val              = (XD_FLOAT8)sqlite3_column_double(select_sql,     i);
					XFile::fprintfT(fp, _T("%e"), val);
				}
				break;
			case SQLITE3_TEXT:
				{
					const TCHAR *val      = (const TCHAR*)XSEMI_GemDataAtomSQLite_get_column_text(select_sql, i);
					if( val != NULL ) {
						XStrBuffCSV strRptName(val);
						XFile::fprintfT(fp, _T("\"%s\""), (const TCHAR*)val);
					}
				}
				break;
			case SQLITE_NULL:
				XFile::fprintfT(fp, _T(""));
				break;
			case SQLITE_BLOB:
				{
					XBIN* pVal  = (XBIN*)sqlite3_column_blob(select_sql, i);
					XD_UINT4       nSize = sqlite3_column_bytes(select_sql, i);
					if( pVal != NULL && nSize != 0 ) {
						XDataReader reader(pVal, nSize);
						XData data;
						if( reader.getValue( &data ) == true ) {
							XRichData rData( data.getType() );
							rData.putValueFromXData(reader, data);
							XFile::fprintfT(fp, _T("\"%s\""), (const TCHAR*)rData.toString());
						}

					}
				}
				break;
			default:
				// サポートされていないタイプ
				strErrMsg.Format(_T("invalid table colmn type(%u)."), type);
				XEXCEPT_THROW_STR(XExcept_SQLite, (const TCHAR*)strErrMsg, 0, 0);
				break;
			}
		}
		XFile::fprintfT(fp, _T("\n"));
	}
	sqlite3_finalize(select_sql);
	if (err != SQLITE_DONE) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}

	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::exportCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader)
{
XEXCEPT_ENTRY

	int				err = 0;
	XStrBuff		strErrMsg;
	sqlite3_stmt*	select_sql = NULL;
	FILE*			fp = NULL;
	TCHAR*			p_sql = NULL;
	bool			bAll = (tableType == XSEMI_GemMstData::TABLE_TYPE_ALL) ? true : false;
	XSEMI_GemMstData::TABLE_TYPE	tblTypes[XSEMI_GemConstObject::UNKOWN];
	int								numOfTbleType = 1;
	int								i;
	// データベースのオープン
	openDB();
	try {
		if( bAll == false ) {
			tblTypes[0] = tableType;
			switch( tableType ) {
			case XSEMI_GemMstData::TABLE_TYPE_CEID:
			case XSEMI_GemMstData::TABLE_TYPE_RPTID:
			case XSEMI_GemMstData::TABLE_TYPE_VARIABLE_GRP:
			case XSEMI_GemMstData::TABLE_TYPE_VID_EC:
			case XSEMI_GemMstData::TABLE_TYPE_VID_SV:
			case XSEMI_GemMstData::TABLE_TYPE_VID_DV:
			case XSEMI_GemMstData::TABLE_TYPE_CEID_RPTID:
			case XSEMI_GemMstData::TABLE_TYPE_RPTID_DVID:
			case XSEMI_GemMstData::TABLE_TYPE_ALARM:
			case XSEMI_GemMstData::TABLE_TYPE_ACKCODES:
			case XSEMI_GemMstData::TABLE_TYPE_SPOOL_SF:
				break;
			default:
				// サポートされていないタイプ
				strErrMsg.Format(_T("exportCSV. non support type(%u)."), tableType);
				XEXCEPT_THROW_STR(XExcept_FILE, (const TCHAR*)strErrMsg, 0, 0);
				break;
			}
		} else {
			numOfTbleType = 0;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_CEID;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_RPTID;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_VARIABLE_GRP;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_VID_EC;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_VID_SV;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_VID_DV;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_CEID_RPTID;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_RPTID_DVID;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_ALARM;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_ACKCODES;
			tblTypes[numOfTbleType++] = XSEMI_GemMstData::TABLE_TYPE_SPOOL_SF;
		}

		// ファイル作成
		if( XFile::fopenT(&fp, p_file, 
#ifdef _WIN32
			_T("wt")
#else
			_T("w")
#endif			
			) != 0 ) {
			strErrMsg.Format(_T("fail to opne file(%s)."), p_file);
			XEXCEPT_THROW_STR(XExcept_FILE, (const TCHAR*)strErrMsg, 0, 0);
			return false;
		}


		for(i = 0; i < numOfTbleType; i++) {
			switch( tblTypes[i] ) {
			case XSEMI_GemMstData::TABLE_TYPE_CEID:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_CEID, bExitHeader == true ? _T("CEID"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_RPTID:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_RPTID, bExitHeader == true ? _T("RPTID"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_VARIABLE_GRP:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_VARIABLE_GRP, bExitHeader == true ? _T("VARIABLE_GRP"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_VID_EC:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_EC, bExitHeader == true ? _T("EC"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_VID_SV:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_SV, bExitHeader == true ? _T("SV"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_VID_DV:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_DV, bExitHeader == true ? _T("DV"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_CEID_RPTID:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_CEID_RPTID, bExitHeader == true ? _T("CEID_RPTID"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_RPTID_DVID:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_RPTID_DVID, bExitHeader == true ? _T("RPTID_DVID"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_ALARM:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_ALARM, bExitHeader == true ? _T("ALARM"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_ACKCODES:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_ACKCODES, bExitHeader == true ? _T("ACKCODES"): NULL);
				break;
			case XSEMI_GemMstData::TABLE_TYPE_SPOOL_SF:
				innerExportCSV(fp, XSEMI_SQL_GRM_SELECT_SPOOL_SF, bExitHeader == true ? _T("ACKCODES"): NULL);
				break;
			default:
				// サポートされていないタイプ
				strErrMsg.Format(_T("non support type(%u)."), tableType);
				XEXCEPT_THROW_STR(XExcept_FILE, (const TCHAR*)strErrMsg, 0, 0);
				break;
			}
		}
		fclose(fp);
		fp = NULL;
	} catch ( XExcept &ex ) {
		if( fp != NULL ) {
			fclose(fp);
			fp = NULL;
		}
		XEXCEPT_RETHROW(ex, XExcept_RETHROW); 
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::reloadObject(XSEMI_GemConstObject* pObject)
{
XEXCEPT_ENTRY
	XStrBuff strErrMsg;
	XStrBuff		strSQL;

	if( pObject == NULL ) {
		strErrMsg.Format(_T("object is null."));
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrMsg, 0, 0);	
	}

	// データベースのオープン
	openDB();

	//	トランザクション開始
	beginTansction();

	try {
		switch( pObject->get_ObjectType() ) {
		case XSEMI_GemConstObject::VID:
			{
				innerReloadV((sqlite3*)m_db, (XSEMI_V*)pObject);
			}
			break;
		}
		endTansction();
	} catch ( XExcept &ex ) {
		rollBackTansction();
		XEXCEPT_RETHROW(ex, XExcept_RETHROW); 
	}
	return true;
XEXCEPT_EXIT
}
bool XSEMI_GemSQLiteIF::updateObject(XSEMI_GemConstObject* pObject)
{
XEXCEPT_ENTRY
	XStrBuff strErrMsg;
	XStrBuff		strSQL;




	if( pObject == NULL ) {
		strErrMsg.Format(_T("object is null."));
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrMsg, 0, 0);	
	}


	// データベースのオープン
	openDB();

	//	トランザクション開始
	beginTansction();

	try {
		switch( pObject->get_ObjectType() ) {
		case XSEMI_GemConstObject::CEID:
			{
				XSEMI_CEID* pCEID = (XSEMI_CEID*)pObject;
				innerUpdateCEID((sqlite3*)m_db, pCEID);
			}
			break;
		case XSEMI_GemConstObject::CEIDs:
			{
				XSEMI_CEIDs* pCEIDs = (XSEMI_CEIDs*)pObject;
				XSEMI_CEID* pCEID = pCEIDs->getFirstItem();
				while( pCEID != NULL ) {
					if( pCEID->isDirtyMember() == true ) {
						innerUpdateCEID((sqlite3*)m_db, pCEID);
					}
					pCEID = pCEIDs->getNextItem();
				}
			}
			break;
		case XSEMI_GemConstObject::RPTID:
			{
				XSEMI_Rpt* pRpt = (XSEMI_Rpt*)pObject;
				innerUpdateRpt((sqlite3*)m_db, pRpt);
			}
			break;
		case XSEMI_GemConstObject::RPTIDs:
			{
				XSEMI_Rpts* pRpts = (XSEMI_Rpts*)pObject;
				XSEMI_Rpt* pRpt = pRpts->getFirstItem();
				while( pRpt != NULL ) {
					if( pRpt->isDirtyMember() == true ) {
						innerUpdateRpt((sqlite3*)m_db, pRpt);
					}
					pRpt = pRpts->getNextItem();
				}
			}
			break;
		case XSEMI_GemConstObject::VID:
			{
				XSEMI_V* pV = (XSEMI_V*)pObject;
				innerUpdateV((sqlite3*)m_db, pV);
			}
			break;
		case XSEMI_GemConstObject::VIDs:
			{
				XSEMI_Vs* pVs = (XSEMI_Vs*)pObject;
				XSEMI_V* pV = pVs->getFirstItem();
				while( pV != NULL ) {
					if( pV->isDirtyMember() == true ) {
						innerUpdateV((sqlite3*)m_db, pV);
					}
					pV = pVs->getNextItem();
				}
			}
			break;
		case XSEMI_GemConstObject::ALARM:
			{
				XSEMI_ALARM* pAlarm = (XSEMI_ALARM*)pObject;
				innerUpdateAlaram((sqlite3*)m_db, pAlarm);
			}
			break;
		case XSEMI_GemConstObject::ALARMs:
			{
				XSEMI_ALARMs* pAlarms = (XSEMI_ALARMs*)pObject;
				XSEMI_ALARM* pAlarm = pAlarms->getFirstItem();
				while( pAlarm != NULL ) {
					if( pAlarm->isDirtyMember() == true ) {
						innerUpdateAlaram((sqlite3*)m_db, pAlarm);
					}
					pAlarm = pAlarms->getNextItem();
				}
			}
			break;
		case XSEMI_GemConstObject::VARIABLE_GRP:
			{
				XLIB_STD_STREAM_BUFFER		buff;
				XLIB_STD_OUT_STR_STREAM		stream( &buff );
				XSEMI_VARIABLE_GRP* pAtom = (XSEMI_VARIABLE_GRP*)pObject;
				// 追加に失敗したら更新する。
				buff.resetp();
				pAtom->makeInsertString(stream);
				if( this->execute( buff.toString(), false )  == false ) {
					buff.resetp();
					pAtom->makeUpdateString(stream);
					this->execute( buff.toString() );
				}
			}
			break;

		case XSEMI_GemConstObject::ACKCODE:
			{
				XLIB_STD_STREAM_BUFFER		buff;
				XLIB_STD_OUT_STR_STREAM		stream( &buff );
				XSEMI_ACKCODE* pAtom = (XSEMI_ACKCODE*)pObject;
				// 追加に失敗したら更新する。
				buff.resetp();
				pAtom->makeInsertString(stream);
				if( this->execute( buff.toString(), false )  == false ) {
					buff.resetp();
					pAtom->makeUpdateString(stream);
					this->execute( buff.toString() );
				}
			}
			break;
		case XSEMI_GemConstObject::SPOOL_SFs:
			{
				// 全件削除
				this->execute( XSEMI_SQL_GRM_DELETE_SPOOL_SF );
				
				XSEMI_SpoolSFs* pSFs = (XSEMI_SpoolSFs*)pObject;
				XSEMI_SpoolSF* pSF = pSFs->getFirstItem();
				while( pSF != NULL ) {
					this->updateObject( pSF );
					pSF = pSFs->getNextItem();
				}
			}
			break;
		case XSEMI_GemConstObject::SPOOL_SF:
			{
				XLIB_STD_STREAM_BUFFER		buff;
				XLIB_STD_OUT_STR_STREAM		stream( &buff );
				XSEMI_SpoolSF* pAtom = (XSEMI_SpoolSF*)pObject;
				// 追加に失敗したら更新する。
				buff.resetp();
				pAtom->makeInsertString(stream);
				if( this->execute( buff.toString(), false )  == false ) {
					buff.resetp();
					pAtom->makeUpdateString(stream);
					this->execute( buff.toString() );
				}
			}
			break;
		case XSEMI_GemConstObject::VVAL:
			{
				XLIB_STD_STREAM_BUFFER		buff;
				XLIB_STD_OUT_STR_STREAM		stream( &buff );
				XSEMI_VVAL* pAtom = (XSEMI_VVAL*)pObject;
				// 追加に失敗したら更新する。
				buff.resetp();
				pAtom->makeInsertString(stream);
				if( this->execute( buff.toString(), false )  == false ) {
					buff.resetp();
					pAtom->makeUpdateString(stream);
					this->execute( buff.toString() );
				}
			}
			break;
		default:
			strErrMsg.Format(_T("cannot updateObject. non suport objecttype (%d)."), pObject->get_ObjectType());
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrMsg, 0, 0);	
			break;
		}

		endTansction();
	} catch ( XExcept &ex ) {
		rollBackTansction();
		XEXCEPT_RETHROW(ex, XExcept_RETHROW); 
	}
	return true;
XEXCEPT_EXIT
}


bool XSEMI_GemSQLiteIF::deleteObject(XSEMI_GemConstObject* pObject)
{
XEXCEPT_ENTRY
	XStrBuff strErrMsg;
	XLIB_STD_STREAM_BUFFER		buff;
	XLIB_STD_OUT_STR_STREAM		stream( &buff );

	if( pObject == NULL ) {
		strErrMsg.Format(_T("object is null."));
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrMsg, 0, 0);	
	}

	// データベースのオープン
	openDB();

	//	トランザクション開始
	beginTansction();

	try {
		switch( pObject->get_ObjectType() ) {
		case XSEMI_GemConstObject::CEID:
			{
				XSEMI_CEID* pCEID = (XSEMI_CEID*)pObject;
				buff.resetp();
				pCEID->makeDeleteString(stream);
				this->execute( buff.toString() );
			}
			break;
		case XSEMI_GemConstObject::RPTID:
			{
				XSEMI_Rpt* pRpt = (XSEMI_Rpt*)pObject;
				buff.resetp();
				pRpt->makeDeleteString(stream);
				this->execute( buff.toString() );
			}
			break;
		case XSEMI_GemConstObject::VID:
			{
				XSEMI_V* pV = (XSEMI_V*)pObject;
				buff.resetp();
				pV->makeDeleteString(stream);
				this->execute( buff.toString() );
			}
			break;
		case XSEMI_GemConstObject::ALARM:
			{
				XSEMI_ALARM* pAlarm = (XSEMI_ALARM*)pObject;
				buff.resetp();
				pAlarm->makeDeleteString(stream);
				this->execute( buff.toString() );
			}
			break;
		case XSEMI_GemConstObject::VARIABLE_GRP:
			{
				XSEMI_VARIABLE_GRP* pAtom = (XSEMI_VARIABLE_GRP*)pObject;
				buff.resetp();
				pAtom->makeDeleteString(stream);
				this->execute( buff.toString() );
			}
			break;
		case XSEMI_GemConstObject::ACKCODE:
			{
				XSEMI_ACKCODE* pAtom = (XSEMI_ACKCODE*)pObject;
				buff.resetp();
				pAtom->makeDeleteString(stream);
				this->execute( buff.toString() );
			}
			break;
		case XSEMI_GemConstObject::VVAL:
			{
				XSEMI_VVAL* pAtom = (XSEMI_VVAL*)pObject;
				buff.resetp();
				pAtom->makeDeleteString(stream);
				this->execute( buff.toString() );
			}
			break;
		default:
			strErrMsg.Format(_T("non suport objecttype (%d)."), pObject->get_ObjectType());
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrMsg, 0, 0);	
			break;
		}

		endTansction();
	} catch ( XExcept &ex ) {
		rollBackTansction();
		XEXCEPT_RETHROW(ex, XExcept_RETHROW); 
	}
	return true;
XEXCEPT_EXIT
}


#include "xlib/xstrcordeconv.h"

#ifdef _UNICODE
const char* XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(const wchar_t* p_str)
#else
const char* XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(const char* p_str)
#endif
{
	static XStrBuff buf;
	bool bfCESU = false;
#ifdef _UNICODE
	XStrBuffCodeConv::convertUnicodeToUTF8(p_str, wcslen(p_str), buf, true);
	return (const char*)(const wchar_t*)buf;
#else
	XStrBuffCodeConv::convertNativeToUTF8(p_str, buf, true);
	return buf;
#endif
}




bool XSEMI_GemSQLiteIF::execute(const TCHAR* p_strSQL, bool throwError /*= true*/)
{
XEXCEPT_ENTRY
	int				err;

XTRACE(_T("IN XSEMI_GemSQLiteIF::execute\n"));	


	// データベースのオープン
	openDB();

	if( XStrBuff::StrStr(p_strSQL, _T("@@@"))  == 0 ) {


XTRACE(_T("  %s\n"), p_strSQL);
/*
#ifdef _UNICODE
#define XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(A) (const char*)XStrBuff(A)
#else
#define XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(A) (A)
#endif
*/


		err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(p_strSQL), NULL, NULL, NULL);
		if (err != SQLITE_OK) {
			XStrBuff strErrSQL = XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db);
			XStrBuff strErr;


XTRACE(_T("SQL_EXECUTE Error !!! [%s]\n"), (const TCHAR*)strErrSQL);

			strErr.Format(_T("SQL:[%s] ErrorStr:[%s]"), p_strSQL, (const TCHAR*)strErrSQL);
			if( throwError == true ) {
				XEXCEPT_THROW_STR(XExcept_SQLite, (const TCHAR*)strErr, 0, err);
			}
			return false;
		}
	} else {
		XStrBuff		strSQL(p_strSQL);
		TCHAR*			pTmp;
		TCHAR*			pWrk;

		pWrk = pTmp = strSQL;

		while( pWrk != 0 ) {
			pWrk = XStrBuff::StrStr(pTmp, _T("@@@"));
			if( pWrk != 0 ) {
				*pWrk = 0;
				pWrk += 3;
				if( *pWrk == '\n' ) {
					pWrk++;
				}
			}


XTRACE(_T("  %s\n"), pTmp);


			err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(pTmp), NULL, NULL, NULL);
			if (err != SQLITE_OK) {
				XStrBuff strErrSQL = XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db);
				XStrBuff strErr;

XTRACE(_T("SQL_EXECUTE Error !!! [%s]\n"), (const TCHAR*)strErrSQL);


				strErr.Format(_T("SQL:[%s] ErrorStr:[%s]"), pTmp, (const TCHAR*)strErrSQL);
				if( throwError == true ) {
					XEXCEPT_THROW_STR(XExcept_SQLite, (const TCHAR*)strErr, 0, err);
				}
				return false;
			}
			pTmp = pWrk;
		}
	}
	return true;
XEXCEPT_EXIT
}
bool XSEMI_GemSQLiteIF::innerUpdateCEID(void* pdb, XSEMI_CEID *pCEID)
{
XEXCEPT_ENTRY
	XLIB_STD_STREAM_BUFFER		buff;
	XLIB_STD_OUT_STR_STREAM		stream( &buff );

	// 追加に失敗したら更新する。
	buff.resetp();
	pCEID->makeInsertString(stream);
	if( this->execute( buff.toString(), false )  == false ) {
		buff.resetp();
		pCEID->makeUpdateString(stream);
		this->execute( buff.toString() );
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::innerdeleteCEID_RptID(XD_UINT4 ceid, XD_UINT4 rptid)
{
XEXCEPT_ENTRY
	int err;

	// 追加に失敗したら更新する。
	XStrBuff strSQL;
	
	//
	// リンクイベント情報削除
	//
	if( ceid == 0xFFFFFFFF ) {
		strSQL.Format(_T("delete from CEID_RPTID;"));
	} else {
		if( rptid == 0xFFFFFFFF ) {
			strSQL.Format(_T("delete from CEID_RPTID where CeID=%d;")
							, 	ceid
						);
		} else {
			strSQL.Format(_T("delete from CEID_RPTID where CeID=%d and RptID=%u;")
							, 	ceid
							,	rptid
						);
		}
	}
	err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(strSQL), NULL, NULL, NULL);
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::innerUpdateRpt(void* pdb, XSEMI_Rpt *pRpt)
{
XEXCEPT_ENTRY
	XLIB_STD_STREAM_BUFFER		buff;
	XLIB_STD_OUT_STR_STREAM		stream( &buff );

	// 追加に失敗したら更新する。
	buff.resetp();
	pRpt->makeInsertString(stream);
	if( this->execute( buff.toString(), false )  == false ) {
		buff.resetp();
		pRpt->makeUpdateString(stream);
		this->execute( buff.toString() );
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::innerUpdateV(void* pdb, XSEMI_V *pV)
{
XEXCEPT_ENTRY
	XLIB_STD_STREAM_BUFFER		buff;
	XLIB_STD_OUT_STR_STREAM		stream( &buff );
	int i;
	sqlite3_stmt*		sql_Smt = 0;
	int					rc;

	// データベースのオープン
	openDB();

	XStrBuff		strSQL(_T(""));
	XStrBuff		strWorkerSQL(_T(""));
	TCHAR*			pTmp;
	TCHAR*			pWrk;

	// INSERT, SQL文を作成する
	i = 0;
	buff.resetp();
	pV->makeInsertString(stream);
	strSQL = buff.toString();
	pWrk = pTmp = strSQL;
	while( pWrk != 0 ) {
		pWrk = XStrBuff::StrStr(pTmp, _T("@@@"));
		if( pWrk != 0 ) {
			*pWrk = 0;
			pWrk += 3;
			if( *pWrk == '\n' ) { pWrk++;}
		}
XTRACE(_T("SQL:\n   %s\n"), pTmp);
		strWorkerSQL = pTmp;
		rc = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, pTmp, XStrBuff::StrLen(pTmp), &sql_Smt, 0);
		if (rc == SQLITE_OK) {
			if( i == 0 ) {
				XInflateBinStrstream	stremBuff(256);
				// InitValue
				pV->get_InitialValue().buildBinaryData( &stremBuff );
				sqlite3_bind_blob(sql_Smt, 1, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
				stremBuff.rdbuf()->resetp();

				// VValue
				pV->buildBinaryData( &stremBuff );
				sqlite3_bind_blob(sql_Smt, 2, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
				stremBuff.rdbuf()->resetp();

			} else
			if( i == 1 ) {
				if( pV->get_Class() == XSEMI_V::EC ) {
					XInflateBinStrstream	stremBuff(256);
					XSEMI_ECV* pECV = (XSEMI_ECV*)pV;

					// Max
					pECV->get_EcMax().buildBinaryData( &stremBuff );
					sqlite3_bind_blob(sql_Smt, 1, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
					stremBuff.rdbuf()->resetp();

					// Min
					pECV->get_EcMin().buildBinaryData( &stremBuff );
					sqlite3_bind_blob(sql_Smt, 2, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
					stremBuff.rdbuf()->resetp();

					// Def
					pECV->get_EcDef().buildBinaryData( &stremBuff );
					sqlite3_bind_blob(sql_Smt, 3, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
					stremBuff.rdbuf()->resetp();

					// Value
					pECV->buildBinaryData( &stremBuff );
					sqlite3_bind_blob(sql_Smt, 4, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
				} else
				if( pV->get_Class() == XSEMI_V::SV ) {
					XInflateBinStrstream	stremBuff(256);
					XSEMI_SV* pSV = (XSEMI_SV*)pV;

					// LimitMax
					pSV->get_limitMax().buildBinaryData( &stremBuff );
					sqlite3_bind_blob(sql_Smt, 1, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
					stremBuff.rdbuf()->resetp();

					// LimitMin
					pSV->get_limitMin().buildBinaryData( &stremBuff );
					sqlite3_bind_blob(sql_Smt, 2, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
					stremBuff.rdbuf()->resetp();
				}
			}

			rc = sqlite3_step(sql_Smt);
			if (rc != SQLITE_DONE) {
				XTRACE(_T("SQL ERR:%s\n"), XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db));
			}
			rc = sqlite3_finalize(sql_Smt);
			if (rc != SQLITE_OK) {
				XTRACE(_T("SQL ERR:%s\n"), XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db));
				break;
			}
		}
		i++;
		pTmp = pWrk;
	}
	if (rc != SQLITE_OK) {
		// 追加に失敗したら、更新
		i = 0;
		buff.resetp();
		pV->makeUpdateString(stream);

		strSQL = buff.toString();
		pWrk = pTmp = strSQL;
		while( pWrk != 0 ) {
			pWrk = XStrBuff::StrStr(pTmp, _T("@@@"));
			if( pWrk != 0 ) {
				*pWrk = 0;
				pWrk += 3;
				if( *pWrk == '\n' ) { pWrk++;}
			}
XTRACE(_T("SQL:\n   %s\n"), pTmp);
			strWorkerSQL = pTmp;
			rc = XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, pTmp, XStrBuff::StrLen(pTmp), &sql_Smt, 0);
			if (rc == SQLITE_OK) {

				if( i == 0 ) {
					XInflateBinStrstream	stremBuff(256);
				
					// InitValue
					pV->get_InitialValue().buildBinaryData( &stremBuff );
					sqlite3_bind_blob(sql_Smt, 1, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
					stremBuff.rdbuf()->resetp();

					// VValue
					pV->buildBinaryData( &stremBuff );
					sqlite3_bind_blob(sql_Smt, 2, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
					stremBuff.rdbuf()->resetp();
				} else
				if( i == 1 ) {
					if( pV->get_Class() == XSEMI_V::EC ) {
						XInflateBinStrstream	stremBuff(256);
						XSEMI_ECV* pECV = (XSEMI_ECV*)pV;

						// Max
						pECV->get_EcMax().buildBinaryData( &stremBuff );
						sqlite3_bind_blob(sql_Smt, 1, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
						stremBuff.rdbuf()->resetp();

						// Min
						pECV->get_EcMin().buildBinaryData( &stremBuff );
						sqlite3_bind_blob(sql_Smt, 2, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
						stremBuff.rdbuf()->resetp();

						// Def
						pECV->get_EcDef().buildBinaryData( &stremBuff );
						sqlite3_bind_blob(sql_Smt, 3, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
						stremBuff.rdbuf()->resetp();

						// Value
						pECV->buildBinaryData( &stremBuff );
#ifdef _DEBUG
						{
							XTRACE(_T("ECVal "));
							XBIN* pVal = (XBIN*)stremBuff.rdbuf()->getBuff();
							for(size_t ii = 0; ii < stremBuff.rdbuf()->size(); ii++) {
								XTRACE(_T("%02X "), pVal[ii]);
							}
							XTRACE(_T("\n"));
						}
#endif /*_DEBUG*/
						sqlite3_bind_blob(sql_Smt, 4, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
						stremBuff.rdbuf()->resetp();
					} else
					if( pV->get_Class() == XSEMI_V::SV ) {
						XInflateBinStrstream	stremBuff(256);
						XSEMI_SV* pSV = (XSEMI_SV*)pV;

						// LimitMax
						pSV->get_limitMax().buildBinaryData( &stremBuff );
						sqlite3_bind_blob(sql_Smt, 1, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
						stremBuff.rdbuf()->resetp();

						// LimitMin
						pSV->get_limitMin().buildBinaryData( &stremBuff );
						sqlite3_bind_blob(sql_Smt, 2, stremBuff.rdbuf()->getBuff(), stremBuff.rdbuf()->size(), SQLITE_TRANSIENT);
						stremBuff.rdbuf()->resetp();
					}
				}

				rc = sqlite3_step(sql_Smt);
				if (rc != SQLITE_DONE) {
					XTRACE(_T("SQL ERR:%s\n"), XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db));
				}
				rc = sqlite3_finalize(sql_Smt);
				if (rc != SQLITE_OK) {
					XTRACE(_T("SQL ERR:%s\n"), XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db));
					break;
				}
			}
			i++;
			pTmp = pWrk;
		}
	}
	if (rc != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, 
			(const TCHAR*)XStrBuff::FormatS(_T("SQL:[%s] ErrorStr:[%s]")
				,	(const TCHAR*)strWorkerSQL
				,	(const TCHAR*)XStrBuff( XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db) )
			), 0, rc);
	}

	XSEMI_VVAL* pVal = pV->getVValHeadItem();
	while( pVal != NULL ) {
		if( pVal->isDirtyMember() == true ) {
			updateObject( pVal );
		}
		pVal = pVal->nextSibling();
	}



	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::innerUpdateAlaram(void* pdb, XSEMI_ALARM *pAlarm)
{
XEXCEPT_ENTRY
	XLIB_STD_STREAM_BUFFER		buff;
	XLIB_STD_OUT_STR_STREAM		stream( &buff );
	// 追加に失敗したら更新する。
	buff.resetp();
	pAlarm->makeInsertString(stream);
	if( this->execute( buff.toString(), false )  == false ) {
		buff.resetp();
		pAlarm->makeUpdateString(stream);
		this->execute( buff.toString() );
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::innerUpdateVariableGRP(void* pdb, XSEMI_VARIABLE_GRP *pVG)
{
XEXCEPT_ENTRY
	sqlite3* db = (sqlite3*)pdb;
	XStrBuff strSQL;
	int err  = SQLITE_OK;
	// 追加に失敗したら更新する。
	strSQL.Format(XSEMI_SQL_GRM_INSERT_VARIABLE_GRP
						, 	                          pVG->get_ID()
						,	(const TCHAR*)XStrBuffSQL( pVG->get_Name() == NULL ? _T("") : pVG->get_Name()  )
						,	(const TCHAR*)XStrBuffSQL( pVG->get_Comment()  == NULL ? _T("") : pVG->get_Comment()  )
					);
	err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(strSQL), NULL, NULL, NULL);
	if( err == SQLITE_CONSTRAINT ) {

		strSQL.Format(XSEMI_SQL_GRM_UPDATE_VARIABLE_GRP
						,	(const TCHAR*)XStrBuffSQL( pVG->get_Name()  == NULL ? _T("") : pVG->get_Name()  )
						,	(const TCHAR*)XStrBuffSQL( pVG->get_Comment()  == NULL ? _T("") : pVG->get_Comment()  )
						, 	                          pVG->get_ID()
					);
		err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(strSQL), NULL, NULL, NULL);
	}
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	return true;
XEXCEPT_EXIT
}
bool XSEMI_GemSQLiteIF::innerUpdateCEID_RPTID(void* pdb, XSEMI_GemDataAtom* pAtmData)
{
XEXCEPT_ENTRY
	sqlite3*		db = (sqlite3*)pdb;
	int				err;
	XStrBuff		strSQL;
	// 追加に失敗したら更新する。
	strSQL.Format(XSEMI_SQL_GRM_INSERT_CEID_RPTID
						,								pAtmData->getUint(XSEMI_DATA_CLMN_CeID)
						,								pAtmData->getUint(XSEMI_DATA_CLMN_RptIdSeq)
						,								pAtmData->getUint(XSEMI_DATA_CLMN_RptID)
						,								pAtmData->getUint(XSEMI_DATA_CLMN_Option)
						,	(const TCHAR*)XStrBuffSQL(	pAtmData->getString(XSEMI_DATA_CLMN_Comment) == NULL ? _T("") : pAtmData->getString(XSEMI_DATA_CLMN_Comment)  )
					);
	err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(strSQL), NULL, NULL, NULL);
	if( err == SQLITE_CONSTRAINT ) {
		strSQL.Format(XSEMI_SQL_GRM_UPDATE_CEID_RPTID
							,								pAtmData->getUint(XSEMI_DATA_CLMN_Option)
							,	(const TCHAR*)XStrBuffSQL(	pAtmData->getString(XSEMI_DATA_CLMN_Comment) == NULL ? _T("") : pAtmData->getString(XSEMI_DATA_CLMN_Comment) )
							,								pAtmData->getUint(XSEMI_DATA_CLMN_CeID)
							,								pAtmData->getUint(XSEMI_DATA_CLMN_RptIdSeq)
							,								pAtmData->getUint(XSEMI_DATA_CLMN_RptID)
						);
		err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(strSQL), NULL, NULL, NULL);
	}
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::innerUpdateRPTID_DVID(void* pdb, XSEMI_GemDataAtom* pAtmData)
{
XEXCEPT_ENTRY
	sqlite3*		db = (sqlite3*)pdb;
	int				err;
	XStrBuff		strSQL;


	// 追加に失敗したら更新する。
	strSQL.Format(XSEMI_SQL_GRM_INSERT_RPTID_DVID
						,								pAtmData->getUint(XSEMI_DATA_CLMN_RptID)
						,								pAtmData->getUint(XSEMI_DATA_CLMN_DvIdSeq)
						,								pAtmData->getUint(XSEMI_DATA_CLMN_DvID)
					);
	err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(strSQL), NULL, NULL, NULL);
	if( err == SQLITE_CONSTRAINT ) {

		strSQL.Format(XSEMI_SQL_GRM_UPDATE_RPTID_DVID
						,								pAtmData->getUint(XSEMI_DATA_CLMN_DvID)
						,								pAtmData->getUint(XSEMI_DATA_CLMN_RptID)
						,								pAtmData->getUint(XSEMI_DATA_CLMN_DvIdSeq)
						);
		err = sqlite3_exec((sqlite3*)m_db, XSEMI_GemSQLiteIF_sqlite3_exec_WRAP(strSQL), NULL, NULL, NULL);
	}
	if (err != SQLITE_OK) {
		XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemSQLiteIF::innerReloadV(void* pdb, XSEMI_V *pV)
{
XEXCEPT_ENTRY
	try {
		int				err;
		TCHAR*			p_sql;
		sqlite3_stmt*	select_sql = NULL;
		XStrBuff		strSql;
	
		switch( pV->get_Class() ) {
		case XSEMI_V::SV:
			strSql = XSEMI_SQL_GRM_SELECT_SV;
			break;
		case XSEMI_V::EC:
			strSql = XSEMI_SQL_GRM_SELECT_EC;
			break;
		case XSEMI_V::DV:
			strSql = XSEMI_SQL_GRM_SELECT_DV;
			break;
		default:
			break;
		}
		strSql += (const TCHAR*)XStrBuff::FormatS(_T("	and	VID.VID = %d;"), pV->get_VID() );
		p_sql =	(TCHAR*)(const TCHAR*)strSql;
		XSEMI_GemDataAtomSQLite_prepare((sqlite3*)m_db, p_sql, XStrBuff::StrLen(p_sql), &select_sql, NULL);
		XSEMI_GemDataAtomSQLite atomV(select_sql);
		while ((err = sqlite3_step(select_sql)) == SQLITE_ROW) {
			pV->setDetail( &atomV );
			pV->resetDirtyMember();
		}
		sqlite3_finalize(select_sql);
	} catch ( XExcept &ex ) {
		XStrBuff strMsg(_T(""));
		strMsg.Format(_T("VID=%u"), pV->get_VID());
		ex.addStackTrace(XExcept_SECS, __FILE__, __LINE__, 0, 0, strMsg);
		XEXCEPT_RETHROW(ex, XExcept_RETHROW); 
	}
	return true;
XEXCEPT_EXIT
}
void XSEMI_GemSQLiteIF::setRDataFromString(const TCHAR* p_str, XRichData& val)
{
XEXCEPT_ENTRY
	XStrBuff buff;
	TCHAR* pBuff;
	unsigned int len = 0;
	XSecsTextParser parser;

	pBuff = (TCHAR*)buff.Alloc(::XStrBuff::StrLen( p_str ) + 10);
	XStrBuff::StrCpy(pBuff, buff.getCountOfBuff(), p_str);
	XStrBuff::StrCat(pBuff, buff.getCountOfBuff(),  _T(">"));
	len = XStrBuff::StrLen(pBuff);

	switch( val.getType() ) {
	case XDATA_TYPE_L:case XDATA_TYPE_END:
		break;
	case XDATA_TYPE_A:case XDATA_TYPE_J:case XDATA_TYPE_UTF16:
		parser.parseStringFromString(&pBuff, len, &val);
		break;
	case XDATA_TYPE_B:
		parser.parseBinaryFromString(&pBuff, len, &val);
		break;
	case XDATA_TYPE_BOOL:
		parser.parseBoolromString(&pBuff, len, &val);
		break;
	default:
		parser.parseDigitFromString(&pBuff, len, &val);
		break;
	}
XEXCEPT_EXIT
}
bool XSEMI_GemSQLiteIF::setRiDataFromString(XRichData* pRdata, const TCHAR* p_str)
{
XEXCEPT_ENTRY
	try {
		XSecsTextParser::parseFromString(p_str, XStrBuff::StrLen(p_str), pRdata);
	} catch ( XExcept &e ) {
		// eが未使用になるため、コンパイルのwaningを出力しないための処理
		unsigned long err_code = e.getOsErrorCode();
	}
	return true;
XEXCEPT_EXIT
}

void XSEMI_GemSQLiteIF::beginTansction()
{
XEXCEPT_ENTRY
	if( m_iTansactin == 0 ) {
		// データベースのオープン
		openDB();
		//
		//	トランザクション開始
		//
		sqlite3_exec((sqlite3*)m_db, "BEGIN;", NULL, NULL, NULL);
	}
	m_iTansactin++;
XEXCEPT_EXIT
}
void XSEMI_GemSQLiteIF::endTansction()
{
XEXCEPT_ENTRY
	if( m_iTansactin == 0 ) {
		return ;
	}
	m_iTansactin--;
	if( m_iTansactin == 0 ) {
		// データベースのオープン
		openDB();

		int err = sqlite3_exec((sqlite3*)m_db, "COMMIT;", NULL, NULL, NULL);
		if (err != SQLITE_OK) {
			XEXCEPT_THROW_STR(XExcept_SQLite, XSEMI_GemDataAtomSQLite_sqlite3_errmsg((sqlite3*)m_db), 0, err);
		}
	}
XEXCEPT_EXIT
}
void XSEMI_GemSQLiteIF::rollBackTansction()
{
XEXCEPT_ENTRY
	sqlite3_exec((sqlite3*)m_db, "rollback;", NULL, NULL, NULL);
	m_iTansactin = 0;
XEXCEPT_EXIT
}