#include "xlib/xstrbuff.h"
#include "xlib/xdata.h"
#include "xlib/xdata_rich.h"
#include "xlib/xeception.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"
#include "xlib/semi/secs/xsecs_text_parser.h"


XSEMI_GemConstObject::XSEMI_GemConstObject(TYPE type)
	:	m_type(type)
	,	m_bDirtyMember( false )
	,	m_pParentObject( NULL )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSEMI_GemConstObject::~XSEMI_GemConstObject()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSEMI_GemConstObject::TYPE XSEMI_GemConstObject::get_ObjectType() const
{
XEXCEPT_ENTRY
	return m_type;
XEXCEPT_EXIT
}
bool XSEMI_GemConstObject::isDirtyMember()
{
XEXCEPT_ENTRY
	return m_bDirtyMember;
XEXCEPT_EXIT
}
void XSEMI_GemConstObject::resetDirtyMember()
{
XEXCEPT_ENTRY
	m_bDirtyMember = false;
XEXCEPT_EXIT
}
XSEMI_GemConstObject* XSEMI_GemConstObject::getParentByType( XSEMI_GemConstObject::TYPE type )
{
XEXCEPT_ENTRY

	if( m_type == type ) {
		return this;
	}
	if( m_pParentObject != NULL ) {
		return m_pParentObject->getParentByType( type );
	}	
	return NULL;
XEXCEPT_EXIT
}


//---------------------------------------------------------------------------------
//
//	XSEMI_GemDataAtom
//
//---------------------------------------------------------------------------------
const TCHAR* XSEMI_GemDataAtom::getString(XSEMI_CONST_SCHEMA* pSchema, int index, void* pVal, XStrBuff* pBuff)
{
XEXCEPT_ENTRY

	switch( pSchema[index].real_type ) {
	case XSEMI_CONST_SCHEMA_RTYPE_NULL:
		return 0;
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_CHAR_PTR:
		if( pVal == NULL ) {
			NULL;
		}
		if( pBuff != NULL ) {
			*pBuff = (const TCHAR*)pVal;
		}
		return (const TCHAR*)pVal;
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_XSTR:
		if( pVal == NULL ) {
			NULL;
		}
		{	XStrBuff* pStr = (XStrBuff*)pVal;
			if( pBuff != NULL ) {
				*pBuff = (*pStr);
			}
			return (const TCHAR*)(*pStr);
		}
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_RDATA:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		{
			XRichData* pRData = (XRichData*)pVal;
			if( pBuff == NULL ) {
				XEXCEPT_THROW_STR(XExcept_XDATA, _T("pBuff is not specieded."), 0, index);
			}
			*pBuff = pRData->toString();
			return (const TCHAR*)(*pBuff);
		}
		break;
	default:
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("data type error."), 0, index);
		break;
	}
	return 0;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_GemDataAtom::getUint(XSEMI_CONST_SCHEMA* pSchema, int index, void* pVal)
{
XEXCEPT_ENTRY
	switch( pSchema[index].real_type ) {
	case XSEMI_CONST_SCHEMA_RTYPE_NULL:
		return 0;
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_UINT1:
	case XSEMI_CONST_SCHEMA_RTYPE_INT1:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		return  (XD_UINT4) *((XD_UINT1*)pVal);
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_UINT2:
	case XSEMI_CONST_SCHEMA_RTYPE_INT2:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		return  (XD_UINT4) *((XD_UINT2*)pVal);
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_UINT4:
	case XSEMI_CONST_SCHEMA_RTYPE_INT4:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		return  (XD_UINT4) *((XD_UINT4*)pVal);
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_UINT8:
	case XSEMI_CONST_SCHEMA_RTYPE_INT8:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		return  (XD_UINT4) *((XD_UINT8*)pVal);
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_BOOL:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		return  *((bool*)pVal) == true ? 1 : 0;
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_RDATA:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		{
			XRichData* pRData = (XRichData*)pVal;
			XD_UINT4 val = 0;
			switch( pRData->getType() ) {
			case XDATA_TYPE_U1:
				val = (XD_UINT4)pRData->getValueUInt1();
				break;
			case XDATA_TYPE_U2:
				val = (XD_UINT4)pRData->getValueUInt2();
				break;
			case XDATA_TYPE_U4:
				val = (XD_UINT4)pRData->getValueUInt4();
				break;
			case XDATA_TYPE_I1:
				val = (XD_UINT4)pRData->getValueInt1();
				break;
			case XDATA_TYPE_I2:
				val = (XD_UINT4)pRData->getValueInt2();
				break;
			case XDATA_TYPE_I4:
				val = (XD_UINT4)pRData->getValueInt4();
				break;
			default:
				XEXCEPT_THROW_STR(XExcept_XDATA, _T("data type error."), 0, index);
				break;
			}
			return val;
		}
		break;
	default:
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("data type error."), 0, index);
		break;
	}

	return 0;
XEXCEPT_EXIT
}
XD_FLOAT8 XSEMI_GemDataAtom::getDouble(XSEMI_CONST_SCHEMA* pSchema, int index, void* pVal)
{
XEXCEPT_ENTRY
	switch( pSchema[index].real_type ) {
	case XDATA_TYPE_F4:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		return  (XD_FLOAT8) *((XD_FLOAT4*)pVal);
		break;
	case XDATA_TYPE_F8:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		return  (XD_FLOAT8) *((XD_FLOAT8*)pVal);
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_RDATA:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		{
			XRichData* pRData = (XRichData*)pVal;
			XD_FLOAT8 val = 0;
			switch( pRData->getType() ) {
			case XDATA_TYPE_F4:
				val = (XD_FLOAT8)pRData->getValueFloat4();
				break;
			case XDATA_TYPE_F8:
				val = (XD_FLOAT8)pRData->getValueFloat8();
				break;
			default:
				XEXCEPT_THROW_STR(XExcept_XDATA, _T("data type error."), 0, index);
				break;
			}
			return val;
		}
		break;
	default:
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("data type error."), 0, index);
		break;
	}
	return 0;
XEXCEPT_EXIT
}
void XSEMI_GemDataAtom::convClmnSQLString(int index, XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	switch( getColmnType( index ) ) {
	case XDATA_TYPE_A:
		convSQLString( getString(index) , stream);
		break;
	case XDATA_TYPE_U1:
		convSQLString( getUint(index) , stream);
		break;
	case XDATA_TYPE_U2:
		convSQLString( getUint(index) , stream);
		break;
	case XDATA_TYPE_U4:
		convSQLString( getUint(index) , stream);
		break;
	case XDATA_TYPE_BOOL:
		convSQLString( getBOOL(index) , stream);
		break;
	case XDATA_TYPE_F8:
		convSQLString( getDouble(index) , stream);
		break;
	case XDATA_TYPE_END:
		stream << _T("?");
		break;
	default:
		_XASSERT(0 );
		break;
	}
XEXCEPT_EXIT
}
void XSEMI_GemDataAtom::makeInsertString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	int	i;

	stream << _T("insert into ") << getAtomName() << _T("(");

	for(i = 0; i < getColmnCount(); i++) {
		if( i != 0 ) {
			stream << _T(", ");
		}
		stream << getColmnName( i );
	}
	stream << _T(") values(");
	for(i = 0; i < getColmnCount(); i++) {
		if( i != 0 ) {
			stream << _T(", ");
		}
		convClmnSQLString(i, stream);
	}
	stream << _T(");");
XEXCEPT_EXIT
}



void XSEMI_GemDataAtom::makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	int	i;
	int iSetCnt = 0;
	int iWhereCnt = 0;

	stream << _T("update ") << getAtomName() << _T(" set	 ");

	for(i = 0; i < getColmnCount(); i++) {
		if( isColumnKey(i) == false ) {
			if( iSetCnt != 0 ) {
				stream << _T(", ");
			}
			stream << getColmnName( i ) << _T("=");

			convClmnSQLString(i, stream);

			iSetCnt ++;
		} else {
			iWhereCnt++;
		}
	}
	if( iWhereCnt != 0 ) {
		iWhereCnt = 0;

		stream << _T(" where ");


		for(i = 0; i < getColmnCount(); i++) {

			if( isColumnKey(i) == true ) {
				if( iWhereCnt != 0 ) {
					stream << _T(" and ");
				}
				stream << getColmnName( i ) << _T("=");
				convClmnSQLString(i, stream);
				iWhereCnt++;
			}
		}
	}
	stream << _T(";");
XEXCEPT_EXIT
}

void XSEMI_GemDataAtom::makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	int	i;
	int iWhereCnt = 0;


	stream << _T("delete from ") << getAtomName();

	for(i = 0; i < getColmnCount(); i++) {
		if( isColumnKey(i) == true ) {
			iWhereCnt++;
		}
	}
	if( iWhereCnt != 0 ) {
		iWhereCnt = 0;
		stream << _T(" where ");
		for(i = 0; i < getColmnCount(); i++) {

			if( isColumnKey(i) == true ) {
				if( iWhereCnt != 0 ) {
					stream << _T(" and ");
				}
				stream << getColmnName( i ) << _T("=");
				convClmnSQLString(i, stream);
				iWhereCnt++;
			}
		}
	}
	stream << _T(";");
XEXCEPT_EXIT
}


void XSEMI_GemDataAtom::convSQLString(XD_UINT4 val, XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	TCHAR szBuff[256];	
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%u"), val);
	stream << szBuff;
XEXCEPT_EXIT
}
void XSEMI_GemDataAtom::convSQLString(XD_FLOAT8 val, XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	TCHAR szBuff[256];	
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%f"), val);
	stream << szBuff;
XEXCEPT_EXIT
}
void XSEMI_GemDataAtom::convSQLString(bool val, XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	TCHAR szBuff[256];	
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%u"), val);
	stream << szBuff;
XEXCEPT_EXIT
}
void XSEMI_GemDataAtom::convSQLString(const char* val, XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	stream << _T("'") << (const TCHAR*)XStrBuffSQL( val ) << _T("'");
XEXCEPT_EXIT
}
void XSEMI_GemDataAtom::convSQLString(const wchar_t* val, XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	stream << _T("'") << (const TCHAR*)XStrBuffSQL( val ) << _T("'");
XEXCEPT_EXIT
}


//---------------------------------------------------------------------------------
//
//	XSEMI_GemConstObjectAtom
//
//---------------------------------------------------------------------------------
XSEMI_GemConstObjectAtom::XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::TYPE type)
	:	XSEMI_GemConstObject(type)
	,	m_pBaseSchema( NULL )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

XSEMI_GemConstObjectAtom::~XSEMI_GemConstObjectAtom()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemConstObjectAtom::getColmnName(int index)
{
XEXCEPT_ENTRY
	if( 99 == index ) {
		// オブジェクトタイプ
		return XSEMI_DATA_CLMN_ObjectType;
	}
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return m_pBaseSchema[ index ].clm_name;
XEXCEPT_EXIT
}

XDATA_TYPE XSEMI_GemConstObjectAtom::getColmnType(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return m_pBaseSchema[ index ].data_type;
XEXCEPT_EXIT
}

bool XSEMI_GemConstObjectAtom::isColumnKey(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return m_pBaseSchema[index].bKey;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemConstObjectAtom::getString(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return getString( getAtomIndex(pKey) );
XEXCEPT_EXIT
}
XD_UINT4    XSEMI_GemConstObjectAtom::getUint(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return getUint( getAtomIndex(pKey) );
XEXCEPT_EXIT
}
XD_INT4     XSEMI_GemConstObjectAtom::getInt(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return getInt( getAtomIndex(pKey) );
XEXCEPT_EXIT
}
XD_FLOAT8   XSEMI_GemConstObjectAtom::getDouble(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return getDouble( getAtomIndex(pKey) );
XEXCEPT_EXIT
}
bool        XSEMI_GemConstObjectAtom::getBOOL(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	return getBOOL( getAtomIndex(pKey) );
XEXCEPT_EXIT
}
void        XSEMI_GemConstObjectAtom::getRData(const TCHAR* pKey, XRichData& data)
{
XEXCEPT_ENTRY
	XSEMI_GemConstObjectAtom::getRData( getAtomIndex(pKey), data );
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemConstObjectAtom::getString(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XSEMI_GemDataAtom::getString(m_pBaseSchema, index, getAtomValPtr( index ), NULL);
XEXCEPT_EXIT
}
XD_UINT4    XSEMI_GemConstObjectAtom::getUint(int index)
{
XEXCEPT_ENTRY
	if( 99 == index ) {
		// オブジェクトタイプ
		return this->get_ObjectType();
	}
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XSEMI_GemDataAtom::getUint(m_pBaseSchema, index, getAtomValPtr( index ));
XEXCEPT_EXIT
}
XD_INT4     XSEMI_GemConstObjectAtom::getInt(int index)
{
XEXCEPT_ENTRY
	return (XD_INT4)getUint(index);
XEXCEPT_EXIT
}
XD_FLOAT8   XSEMI_GemConstObjectAtom::getDouble(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XSEMI_GemDataAtom::getDouble(m_pBaseSchema, index, getAtomValPtr( index ));
XEXCEPT_EXIT
}
bool        XSEMI_GemConstObjectAtom::getBOOL(int index)
{

XEXCEPT_ENTRY
	return getUint(index) == 0 ? false : true;
XEXCEPT_EXIT
}
void        XSEMI_GemConstObjectAtom::getRData(int index, XRichData& data)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	void *pVal = getAtomValPtr( index );
	switch( m_pBaseSchema[index].real_type ) {
	case XDATA_TYPE_F4:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		data.putValue( (XD_FLOAT8) *((XD_FLOAT4*)pVal) );
		break;
	case XDATA_TYPE_F8:
		if( pVal == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("value ptr is null."), 0, index);
		}
		data.putValue( (XD_FLOAT8) *((XD_FLOAT8*)pVal) );
		break;
	case XSEMI_CONST_SCHEMA_RTYPE_RDATA:
		{
			XRichData* pRData = (XRichData*)pVal;
			data.copy( pRData );
		}
		break;
	default:
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("data type error."), 0, index);
		break;
	}


XEXCEPT_EXIT
}
int XSEMI_GemConstObjectAtom::getAtomIndex(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	for(int i = 0; ; i++) {
		if( XStrBuff::StrCmp(m_pBaseSchema[i].clm_name, pKey) == 0 ) {
			return i;
		}
		if( XStrBuff::StrCmp(m_pBaseSchema[i].clm_name, _T("")) == 0 ) {
			break;
		}
	}
	XEXCEPT_THROW_STR(XExcept_XDATA, (const TCHAR*)XStrBuff::FormatS(_T("invalid key [%s]"), (const TCHAR*)XStrBuff(pKey)), 0, 0);
	return -1;
XEXCEPT_EXIT
}


//-----------------------------------------------------------------------------------------
//
//	XSEMI_GemDataAtomCSV
//
//-----------------------------------------------------------------------------------------
XSEMI_GemDataAtomCSV::XSEMI_GemDataAtomCSV(const TCHAR* pKeys, const TCHAR* pAtomName) 
{
XEXCEPT_ENTRY
	m_keys = pKeys;
	m_strAtom = pAtomName;
XEXCEPT_EXIT
}

XSEMI_GemDataAtomCSV::~XSEMI_GemDataAtomCSV()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemDataAtomCSV::getAtomName()
{
XEXCEPT_ENTRY
	return m_strAtom;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_GemDataAtomCSV::getColmnName(int index)
{
XEXCEPT_ENTRY
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return m_keys.GetArgv()[index];
XEXCEPT_EXIT
}
XDATA_TYPE  XSEMI_GemDataAtomCSV::getColmnType(int index)
{
XEXCEPT_ENTRY
	if( index >= m_keys.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XDATA_TYPE_END;
XEXCEPT_EXIT
}
int XSEMI_GemDataAtomCSV::getColmnCount()
{
XEXCEPT_ENTRY
	return m_keys.GetArgCouunt();
XEXCEPT_EXIT
}

void XSEMI_GemDataAtomCSV::setValues(const TCHAR* pValues) 
{
XEXCEPT_ENTRY
	m_values = pValues;
	if( m_keys.GetArgCouunt() != m_values.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("different count of column."), 0, 0);
	}
XEXCEPT_EXIT
}

const TCHAR* XSEMI_GemDataAtomCSV::getString(const TCHAR* pKey) 
{
XEXCEPT_ENTRY
	return this->getString( getIndex( pKey ) );
XEXCEPT_EXIT
}
XD_UINT4    XSEMI_GemDataAtomCSV::getUint(const TCHAR* pKey) 
{
XEXCEPT_ENTRY
	if( XStrBuff::StrCmp(pKey, XSEMI_DATA_CLMN_DataType) == 0 ) {
		XStrBuff strErr;
		const TCHAR* pType = this->getString( getIndex( pKey ) );
		XDATA_TYPE  type = XDATA_TYPE_END;
		if( XStrBuff::StrCmp(pType, _T("L")) == 0 ) {		type = XDATA_TYPE_L;	} else
		if( XStrBuff::StrCmp(pType, _T("B")) == 0 ) {		type = XDATA_TYPE_B;	} else
		if( XStrBuff::StrCmp(pType, _T("BOOL")) == 0 ) {	type = XDATA_TYPE_BOOL;	} else
		if( XStrBuff::StrCmp(pType, _T("A")) == 0 ) {		type = XDATA_TYPE_A;	} else
		if( XStrBuff::StrCmp(pType, _T("J")) == 0 ) {		type = XDATA_TYPE_J;	} else
		if( XStrBuff::StrCmp(pType, _T("UTF16")) == 0 ) {	type = XDATA_TYPE_UTF16;	} else
		if( XStrBuff::StrCmp(pType, _T("U1")) == 0 ) {	type = XDATA_TYPE_U1;	} else
		if( XStrBuff::StrCmp(pType, _T("U2")) == 0 ) {	type = XDATA_TYPE_U2;	} else
		if( XStrBuff::StrCmp(pType, _T("U4")) == 0 ) {	type = XDATA_TYPE_U4;	} else
		if( XStrBuff::StrCmp(pType, _T("U8")) == 0 ) {	type = XDATA_TYPE_U8;	} else
		if( XStrBuff::StrCmp(pType, _T("I1")) == 0 ) {	type = XDATA_TYPE_I1;	} else
		if( XStrBuff::StrCmp(pType, _T("I2")) == 0 ) {	type = XDATA_TYPE_I2;	} else
		if( XStrBuff::StrCmp(pType, _T("I4")) == 0 ) {	type = XDATA_TYPE_I4;	} else
		if( XStrBuff::StrCmp(pType, _T("I8")) == 0 ) {	type = XDATA_TYPE_I8;	} else
		if( XStrBuff::StrCmp(pType, _T("F4")) == 0 ) {	type = XDATA_TYPE_F4;	} else
		if( XStrBuff::StrCmp(pType, _T("F8")) == 0 ) {	type = XDATA_TYPE_F8;	}
		else {
			strErr.Format(_T("invalid %s. [%s]"), XSEMI_DATA_CLMN_DataType, pType);
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
		}
		return (XD_INT4)type;
	}
	return this->getUint( getIndex( pKey ) );
XEXCEPT_EXIT
}
XD_INT4     XSEMI_GemDataAtomCSV::getInt(const TCHAR* pKey) 
{
XEXCEPT_ENTRY
	if( XStrBuff::StrCmp(pKey, XSEMI_DATA_CLMN_DataType) == 0 ) {
		return (XD_INT4)XSEMI_GemDataAtomCSV::getUint(pKey);
	}

	return this->getInt( getIndex( pKey ) );
XEXCEPT_EXIT
}
XD_FLOAT8   XSEMI_GemDataAtomCSV::getDouble(const TCHAR* pKey) 
{
XEXCEPT_ENTRY
	return this->getDouble( getIndex( pKey ) );
XEXCEPT_EXIT
}
bool        XSEMI_GemDataAtomCSV::getBOOL(const TCHAR* pKey) 
{
XEXCEPT_ENTRY
	return this->getBOOL( getIndex( pKey ) );
XEXCEPT_EXIT
}
void        XSEMI_GemDataAtomCSV::getRData(const TCHAR* pKey, XRichData& data)
{
XEXCEPT_ENTRY
	this->getRData( getIndex( pKey ), data );
XEXCEPT_EXIT
}

const TCHAR* XSEMI_GemDataAtomCSV::getString(int index)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_values.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return m_values.GetArgv()[index];
XEXCEPT_EXIT
}

XD_UINT4    XSEMI_GemDataAtomCSV::getUint(int index)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_values.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	const TCHAR *pVal = m_values.GetArgv()[index];
	XD_UINT4 val = 0;
	if( pVal != NULL ) {
		val = XStrBuff::strtoul(pVal, 0, -1);
	}
	return val;
XEXCEPT_EXIT
}

XD_INT4     XSEMI_GemDataAtomCSV::getInt(int index)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_values.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	
	XD_INT4 val = XStrBuff::strtol(m_values.GetArgv()[index], NULL, -1);
	return val;
XEXCEPT_EXIT
}


XD_FLOAT8   XSEMI_GemDataAtomCSV::getDouble(int index)
{
XEXCEPT_ENTRY
	TCHAR* pEndPtr = NULL;
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_values.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	
	XD_FLOAT8 val = XStrBuff::strtod(m_values.GetArgv()[index], &pEndPtr);
	return val;
XEXCEPT_EXIT
}



bool        XSEMI_GemDataAtomCSV::getBOOL(int index)
{
XEXCEPT_ENTRY
	TCHAR* pEndPtr = NULL;
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_values.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	
	XD_UINT4 val = XStrBuff::strtol(m_values.GetArgv()[index], &pEndPtr, 10);
	return val == 0 ? false : true;
XEXCEPT_EXIT
}
void        XSEMI_GemDataAtomCSV::getRData(int index, XRichData& data)
{
XEXCEPT_ENTRY
	if( index < 0 ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	if( index >= m_values.GetArgCouunt() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	
	XSecsTextParser::parseFromString(m_values.GetArgv()[index], XStrBuff::StrLen(m_values.GetArgv()[index]), &data);


XEXCEPT_EXIT
}

int XSEMI_GemDataAtomCSV::getIndex(const TCHAR* pKey)
{
XEXCEPT_ENTRY
	int iCnt            = m_keys.GetArgCouunt();
	const TCHAR** pArgv = m_keys.GetArgv();
	int i;
	XStrBuff strErr;
#ifdef _UNICODE
	XStrBuff strBuff(pKey);
	const wchar_t* pKeyDst = strBuff;
#else
	const char* pKeyDst = pKey;
#endif


	for(i = 0; i < iCnt; i++) {
		if( XStrBuff::StrCmp(pKeyDst, pArgv[i]) == 0 ) {
			return i;
		}
	}
	

	strErr.Format(_T("not found key:[%s]. def["), pKeyDst);

	for(i = 0; i < iCnt; i++) {
		if( i != 0 ) {
			strErr += _T(", ");
		}
		strErr += pArgv[i];
	}
	strErr += _T("]");
	XEXCEPT_THROW_STR(XExcept_XDATA, (const TCHAR*)strErr, 0, 0);
	return -1;
XEXCEPT_EXIT
}
