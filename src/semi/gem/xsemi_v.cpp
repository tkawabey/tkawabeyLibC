#include "xlib/semi/gem/xsemi_v.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/xeception.h"
#include "xlib/semi/secs/xsecs_text_parser.h"

//-----------------------------------------------------------------------------------------
//
//	XSEMI_V
//
//-----------------------------------------------------------------------------------------
XSEMI_CONST_SCHEMA	XSEMI_V::g_pVIDSchema[] = {
		{XSEMI_DATA_CLMN_VID,			XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_Name,			XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DisplayName,	XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DataUnit,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DataType,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Mode,			XDATA_TYPE_U1,		XSEMI_CONST_SCHEMA_RTYPE_UINT1, false}
	,	{XSEMI_DATA_CLMN_InitValue,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_Category,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Comment,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_VARIABLE_GRP,	XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_VPRIVATE,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_GET_EX,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_Enable,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_VValue,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{_T(""),						XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};

XSEMI_V::XSEMI_V(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, Class tyClass, bool bLoaded /*= false*/)
	:	XSEMI_GemConstObjectAtom( XSEMI_GemConstObject::VID )
	,	XRichData(type)
	,	m_vid(vid)
	,	m_strName(p_name)
	,	m_strDisplayName(_T(""))
	,	m_class(tyClass)
	,	m_strUnit("")
	,	m_size( 0 )
	,	m_mode( 0 )
	,	m_InitValue( type )
	,	m_bEnable(true)
	,	m_variable_grpid( 0 )
	,	m_bPrivate( XSEMI_V::PRIVAL_NON )
	,	m_bGetEx( false )
	,	m_strComment("")

	,	m_loadedFlg( 0 )
{
XEXCEPT_ENTRY
	if( bLoaded == true ) {
		m_loadedFlg = ~0;
	}
XEXCEPT_EXIT
}


XSEMI_V::~XSEMI_V()
{
XEXCEPT_ENTRY
	XPOSITION pos1, pos2;
	for( pos1 = m_list.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		XSEMI_VVAL *p = (XSEMI_VVAL*)m_list.GetNext( pos1 );
		delete p;
		m_list.RemoveAt( pos2 );
	}
	m_list.RemoveAll();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_V::get_VID() const
{
XEXCEPT_ENTRY
	return m_vid;
XEXCEPT_EXIT
}

const TCHAR* XSEMI_V::get_Name()
{
XEXCEPT_ENTRY
	return m_strName;
XEXCEPT_EXIT
}
void XSEMI_V::put_Name(const TCHAR* val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strName = val;
XEXCEPT_EXIT
}

const TCHAR* XSEMI_V::get_DisplayName()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_strDisplayName;
XEXCEPT_EXIT
}
void XSEMI_V::put_DisplayName(const TCHAR* val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strDisplayName = val;
XEXCEPT_EXIT
}

XD_UINT1 XSEMI_V::get_Mode() const
{
XEXCEPT_ENTRY
	return m_mode;
XEXCEPT_EXIT
}


const TCHAR* XSEMI_V::get_Comment()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_strComment;
XEXCEPT_EXIT
}

void XSEMI_V::put_Comment(const TCHAR* val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strComment = val;
XEXCEPT_EXIT
}

XSEMI_V::Class XSEMI_V::get_Class() const
{
XEXCEPT_ENTRY
	return m_class;
XEXCEPT_EXIT
}

const TCHAR* XSEMI_V::get_Unit()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_strUnit;
XEXCEPT_EXIT
}

void XSEMI_V::put_Unit(const TCHAR* val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strUnit = val;
XEXCEPT_EXIT
}

XRichData& XSEMI_V::get_InitialValue()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_InitValue;
XEXCEPT_EXIT
}

bool XSEMI_V::get_Enable() const
{
XEXCEPT_ENTRY
	return m_bEnable;
XEXCEPT_EXIT
}
void XSEMI_V::put_Enable(bool val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_bEnable = val;
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_V::get_VariableGrpID()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_variable_grpid;
XEXCEPT_EXIT
}
void XSEMI_V::put_VariableGrpID(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_variable_grpid = val;
XEXCEPT_EXIT
}

XSEMI_V::PRIVAL XSEMI_V::get_Private()
{
XEXCEPT_ENTRY
	return m_bPrivate;
XEXCEPT_EXIT
}
void XSEMI_V::put_Private(XSEMI_V::PRIVAL val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_bPrivate = val;
XEXCEPT_EXIT
}

bool XSEMI_V::get_GetEx()
{
XEXCEPT_ENTRY
	return m_bGetEx;
XEXCEPT_EXIT
}
void XSEMI_V::put_GetEx(bool val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_bGetEx = val;
XEXCEPT_EXIT
}

int XSEMI_V::check(XData &data)
{
XEXCEPT_ENTRY

	XD_UINT4	i = 0;
	int iRetVal = -1;

	// データタイプが違う
	if( this->getType() != data.getType() ) {
		return iRetVal;
	}

	// 範囲のチェック
	XRichData	*pCheck = NULL;

/*
	iRetVal--;

	// MAX値の確認
	pCheck = get_Max();
	if( pCheck != NULL ) {
		if( pCheck->getDataCount() != 0 ) {
			switch( data.getType() ) {
			case XDATA_TYPE_A:
			case XDATA_TYPE_J:

				break;
			case XDATA_TYPE_U1:
				{
					XD_UINT1 val;
					XD_UINT1 chkVal = pCheck->getValueUInt1();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsUInt1( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_U2:
				{
					XD_UINT2 val;
					XD_UINT2 chkVal = pCheck->getValueUInt2();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsUInt2( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_U4:
				{
					XD_UINT4 val;
					XD_UINT4 chkVal = pCheck->getValueUInt4();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsUInt4( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_U8:
				{
					XD_UINT8 val;
					XD_UINT8 chkVal = pCheck->getValueUInt8();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsUInt8( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;

			case XDATA_TYPE_I1:
				{
					XD_INT1 val;
					XD_INT1 chkVal = pCheck->getValueInt1();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsInt1( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_I2:
				{
					XD_INT2 val;
					XD_INT2 chkVal = pCheck->getValueInt2();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsInt2( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_I4:
				{
					XD_INT4 val;
					XD_INT4 chkVal = pCheck->getValueInt4();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsInt4( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_I8:
				{
					XD_INT8 val;
					XD_INT8 chkVal = pCheck->getValueInt8();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsInt8( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;

			case XDATA_TYPE_F4:
				{
					XD_FLOAT4 val;
					XD_FLOAT4 chkVal = pCheck->getValueFloat4();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsFloat4( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_F8:
				{
					XD_FLOAT8 val;
					XD_FLOAT8 chkVal = pCheck->getValueFloat8();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsFloat8( i );
						if( val < chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			}
		}
	}


	iRetVal--;

	// MIN値の確認
	pCheck = get_Min();
	if( pCheck != NULL ) {
		if( pCheck->getDataCount() != 0 ) {
			switch( data.getType() ) {
			case XDATA_TYPE_A:
			case XDATA_TYPE_J:

				break;
			case XDATA_TYPE_U1:
				{
					XD_UINT1 val;
					XD_UINT1 chkVal = pCheck->getValueUInt1();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsUInt1( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_U2:
				{
					XD_UINT2 val;
					XD_UINT2 chkVal = pCheck->getValueUInt2();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsUInt2( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_U4:
				{
					XD_UINT4 val;
					XD_UINT4 chkVal = pCheck->getValueUInt4();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsUInt4( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_U8:
				{
					XD_UINT8 val;
					XD_UINT8 chkVal = pCheck->getValueUInt8();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsUInt8( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;

			case XDATA_TYPE_I1:
				{
					XD_INT1 val;
					XD_INT1 chkVal = pCheck->getValueInt1();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsInt1( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_I2:
				{
					XD_INT2 val;
					XD_INT2 chkVal = pCheck->getValueInt2();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsInt2( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_I4:
				{
					XD_INT4 val;
					XD_INT4 chkVal = pCheck->getValueInt4();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsInt4( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_I8:
				{
					XD_INT8 val;
					XD_INT8 chkVal = pCheck->getValueInt8();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsInt8( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;

			case XDATA_TYPE_F4:
				{
					XD_FLOAT4 val;
					XD_FLOAT4 chkVal = pCheck->getValueFloat4();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsFloat4( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			case XDATA_TYPE_F8:
				{
					XD_FLOAT8 val;
					XD_FLOAT8 chkVal = pCheck->getValueFloat8();
					for(i = 0; i < data.getCount(); i++) {
						val = data.getValAsFloat8( i );
						if( val > chkVal ) {
							return iRetVal;
						}
					}
				}
				break;
			}
		}
	}	
*/
	return 0;
XEXCEPT_EXIT
}

bool XSEMI_V::fetch(XSEMI_Gem* pGem)
{
XEXCEPT_ENTRY
	bool bGetEx = this->get_GetEx();

	if( bGetEx == false ) {
		// GetEx経由以外は、XSEMI_Gem::getVValueを呼び出す必要なし
		return true;
	} else {
		XInflateStreamBuff	buff;
		std::ostream		stream( &buff );
		XDataWriter			dataWriter( &stream );
		XDATA_TYPE			type;


		pGem->getVValue(this->get_VID(), &type, dataWriter);

		XData			xdata;
		XDataReader		dataReader( buff.getBuff(), buff.size() );

		dataReader.getValue( &xdata );
		return this->putValueFromXData(dataReader, xdata);
	}
	return true;
XEXCEPT_EXIT
}

bool XSEMI_V::isDirtyMember()
{
XEXCEPT_ENTRY
	if( XSEMI_GemConstObject::isDirtyMember() == true ) {
		return true;
	}
	if( m_InitValue.isDirty() == true ) {
		return true;
	}
	return this->isDirty();
XEXCEPT_EXIT
}

void XSEMI_V::resetDirtyMember()
{
XEXCEPT_ENTRY
	XSEMI_GemConstObject::resetDirtyMember();
	m_InitValue.resetDirty();
	this->resetDirty();
XEXCEPT_EXIT
}

// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_V::getAtomName()
{
XEXCEPT_ENTRY
	return _T("VID");
XEXCEPT_EXIT
}
int XSEMI_V::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_VVALUE+1;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_V::getString(int index)
{
XEXCEPT_ENTRY
	if( index > (getColmnCount()-1) ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XSEMI_GemDataAtom::getString(g_pVIDSchema, index, getAtomValPtr( index ), &m_atomTmpBuff);
XEXCEPT_EXIT
}
void* XSEMI_V::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( index > (getColmnCount()-1) ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	switch( index ) {
	case ATOM_POS_VID:
		return &m_vid;
		break;
	case ATOM_POS_NAME:
		return &m_strName;
		break;
	case ATOM_POS_DISPLAY_NAME:
		if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
			this->innerLoadDetail();
		}
		return &m_strDisplayName;
		break;
	case ATOM_POS_DATA_UNIT:
		if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
			this->innerLoadDetail();
		}
		return &m_strUnit;
		break;
	case ATOM_POS_DATA_TYPE:
		return &(XRichDataBase::m_type);
		break;
	case ATOM_POS_MODE:
		return &m_mode;
		break;
	case ATOM_POS_INIT_VALUE:
		if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
			this->innerLoadDetail();
		}
		return &m_InitValue;
		break;
	case ATOM_POS_CATEGORY:
		return &m_class;
		break;
	case ATOM_POS_COMMENT:
		if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
			this->innerLoadDetail();
		}
		return &m_strComment;
		break;
	case ATOM_POS_VARIABLE_GRP:
		if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
			this->innerLoadDetail();
		}
		return &m_variable_grpid;
		break;
	case ATOM_POS_VRIVATE:
		if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
			this->innerLoadDetail();
		}
		return &m_bPrivate;
		break;
	case ATOM_POS_GET_EX:
		if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
			this->innerLoadDetail();
		}
		return &m_bGetEx;
		break;
	case ATOM_POS_ENABLE:
		return &m_bEnable;
		break;
	case ATOM_POS_VVALUE:
		return (XRichData*)this;
		break;
	}	
	return NULL;
XEXCEPT_EXIT
}
void XSEMI_V::makeInsertString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	int	i;
	stream << _T("insert into ") << XSEMI_V::getAtomName() << _T("(");

	for(i = 0; i < XSEMI_V::getColmnCount(); i++) {
		if( i != 0 ) {
			stream << _T(", ");
		}
		stream << XSEMI_V::getColmnName( i );
	}
	stream << _T(") values(");
	for(i = 0; i < XSEMI_V::getColmnCount(); i++) {
		if( i != 0 ) {
			stream << _T(", ");
		}
		convClmnSQLString(i, stream);
	}
	stream << _T(");");
XEXCEPT_EXIT
}
void XSEMI_V::makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	int	i;
	int iSetCnt = 0;
	int iWhereCnt = 0;

	stream << _T("update ") << XSEMI_V::getAtomName() << _T(" set	 ");

	for(i = 0; i < XSEMI_V::getColmnCount(); i++) {
		if( XSEMI_V::isColumnKey(i) == false ) {
			if( iSetCnt != 0 ) {
				stream << _T(", ");
			}
			stream << XSEMI_V::getColmnName( i ) << _T("=");

			convClmnSQLString(i, stream);
			iSetCnt ++;
		} else {
			iWhereCnt++;
		}
	}
	if( iWhereCnt != 0 ) {
		iWhereCnt = 0;

		stream << _T(" where ");


		for(i = 0; i < XSEMI_V::getColmnCount(); i++) {

			if( XSEMI_V::isColumnKey(i) == true ) {
				if( iWhereCnt != 0 ) {
					stream << _T(" and ");
				}
				stream << XSEMI_V::getColmnName( i ) << _T("=");
				convClmnSQLString(i, stream);
				iWhereCnt++;
			}
		}
	}
	stream << _T(";");
XEXCEPT_EXIT
}
void XSEMI_V::makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	int	i;
	int iWhereCnt = 0;


	stream << _T("delete from ") << XSEMI_V::getAtomName();

	for(i = 0; i < XSEMI_V::getColmnCount(); i++) {
		if( XSEMI_V::isColumnKey(i) == true ) {
			iWhereCnt++;
		}
	}
	if( iWhereCnt != 0 ) {
		iWhereCnt = 0;
		stream << _T(" where ");
		for(i = 0; i < XSEMI_V::getColmnCount(); i++) {

			if( isColumnKey(i) == true ) {
				if( iWhereCnt != 0 ) {
					stream << _T(" and ");
				}
				stream << XSEMI_V::getColmnName( i ) << _T("=");
				convClmnSQLString(i, stream);
				iWhereCnt++;
			}
		}
	}
	stream << _T(";");
XEXCEPT_EXIT
}

void XSEMI_V::innerLoadDetail()
{
XEXCEPT_ENTRY
	XSEMI_GemConst* pConst = (XSEMI_GemConst*)this->getParentByType( XSEMI_GemConstObject::CONSTRUCTION );
	if( pConst == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object not found. on parent"), 0, 0);
		return ;
	}
	XSEMI_GemMstData* pMater = pConst->get_MasterData();
	if( pMater == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Master Object not found. on parent"), 0, 0);
		return ;
	}
	pMater->reloadObject( this );

	m_loadedFlg |= LD_FLG_DETAIL;
XEXCEPT_EXIT
}
void XSEMI_V::setDetail(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	m_strDisplayName = pAtom->getString(XSEMI_DATA_CLMN_DisplayName);
	m_strUnit =        pAtom->getString(XSEMI_DATA_CLMN_DataUnit);
	m_strComment =     pAtom->getString(XSEMI_DATA_CLMN_Comment);
	m_variable_grpid = pAtom->getUint(XSEMI_DATA_CLMN_VARIABLE_GRP);
	m_bPrivate       = (XSEMI_V::PRIVAL)pAtom->getUint(XSEMI_DATA_CLMN_VPRIVATE);
	m_bGetEx         = pAtom->getUint(XSEMI_DATA_CLMN_GET_EX) == 1;
	pAtom->getRData( XSEMI_DATA_CLMN_InitValue, this->m_InitValue );
	pAtom->getRData( XSEMI_DATA_CLMN_VValue, *this );
	m_loadedFlg |= LD_FLG_DETAIL;
XEXCEPT_EXIT
}

XSEMI_VVAL* XSEMI_V::getVValHeadItem()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_VALS ) == 0 ) {
		// まだロードしていない
		XSEMI_GemConst* pConst = (XSEMI_GemConst*)this->getParentByType(XSEMI_GemConstObject::CONSTRUCTION);
		if( pConst == NULL ) {
			XEXCEPT_THROW_STR(XExcept_APP, _T("Const is not found."), 0, 0);	
			return NULL;
		}
		XSEMI_GemMstData* pMst = pConst->get_MasterData();
		if( pMst == NULL ) {
			XEXCEPT_THROW_STR(XExcept_APP, _T("Master Object is not found."), 0, 0);	
			return NULL;
		}
		pMst->loadVVAlS( this );
		m_loadedFlg |= LD_FLG_VALS;
	}
	if( m_list.IsEmpty() ) {
		return NULL;
	}
	return (XSEMI_VVAL*)m_list.GetHead();
XEXCEPT_EXIT
	return NULL;
}

XSEMI_VVAL* XSEMI_V::addVVal(XRichData& data)
{
XEXCEPT_ENTRY
	XSEMI_GemConst* pConst = (XSEMI_GemConst*)this->getParentByType( XSEMI_GemConstObject::CONSTRUCTION );
	if( pConst == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object not found. on parent"), 0, 0);
		return NULL;
	}
	XSEMI_GemMstData* pMater = pConst->get_MasterData();
	if( pMater == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Master Object not found. on parent"), 0, 0);
		return NULL;
	}


	XSEMI_VVAL* pVVal = NULL;
	pVVal = new XSEMI_VVAL( this, XRichData::getType() );
	if( pVVal == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, sizeof(XSEMI_VVAL));
	}
	pMater->updateObject(pVVal);

	pVVal->m_pos = m_list.AddTail(pVVal);
	pVVal->copy( &data );

	m_bDirtyMember = true;
	return pVVal;
XEXCEPT_EXIT
	return NULL;
}

XSEMI_VVAL* XSEMI_V::addVVal(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_VVAL* pVVal = NULL;
	XStrBuff strValue           = pAtom->getString(XSEMI_DATA_CLMN_Vvalue);
	XStrBuff strCommnet         = pAtom->getString(XSEMI_DATA_CLMN_Comment);
	bool bEnable                = pAtom->getUint(XSEMI_DATA_CLMN_Enable) == 1;

	pVVal = new XSEMI_VVAL( this, XRichData::getType() );
	if( pVVal == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, sizeof(XSEMI_VVAL));
	}
	pVVal->m_pos = m_list.AddTail(pVVal);

	try {
		XSecsTextParser::parseFromString( (TCHAR*)strValue, strValue.getLength(),  pVVal);
	} catch ( XExcept& ) {
	}
	pVVal->put_Comment( strCommnet );
	pVVal->put_Enable( bEnable );
	pVVal->m_bDirtyMember = false;
	return pVVal;
XEXCEPT_EXIT
	return NULL;
}

void XSEMI_V::removeVVal(XSEMI_VVAL* val)
{
XEXCEPT_ENTRY

	XSEMI_GemConst* pConst = (XSEMI_GemConst*)this->getParentByType( XSEMI_GemConstObject::CONSTRUCTION );
	if( pConst == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object not found. on parent"), 0, 0);
		return ;
	}
	XSEMI_GemMstData* pMater = pConst->get_MasterData();
	if( pMater == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Master Object not found. on parent"), 0, 0);
		return ;
	}

	XPOSITION pos1, pos2;
	for( pos1 = m_list.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		XSEMI_VVAL *pItem = (XSEMI_VVAL*)m_list.GetNext( pos1 );
		if( val == pItem ) {
			m_list.RemoveAt( pos2 );
			pMater->deleteObject( val );
			m_bDirtyMember = true;
			delete val;
			return ;
		}
	}
XEXCEPT_EXIT
}




//-----------------------------------------------------------------------------------------
//
//	XSEMI_ECV
//
//-----------------------------------------------------------------------------------------
XSEMI_CONST_SCHEMA	XSEMI_ECV::g_pECVSchema[] = {
		{XSEMI_DATA_CLMN_VID,			XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_Name,			XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DisplayName,	XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DataUnit,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DataType,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Mode,			XDATA_TYPE_U1,		XSEMI_CONST_SCHEMA_RTYPE_UINT1, false}
	,	{XSEMI_DATA_CLMN_InitValue,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_Category,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Comment,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_VARIABLE_GRP,	XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_VPRIVATE,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_GET_EX,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_Enable,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_VValue,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}

	,	{XSEMI_DATA_CLMN_EcMax,			XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_EcMin,			XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_EcDef,			XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_SET_EX,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_ECVerify,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_EcVal,			XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}

	,	{_T(""),						XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_ECV::XSEMI_ECV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded /*= false*/)
	:	XSEMI_V(vid, p_name, type, XSEMI_V::EC, bLoaded)
	,	m_EcMax( type )
	,	m_EcMin( type )
	,	m_EcDef( type )
	,	m_bSetEx( false )
	,	m_bVerify( false )
{
XEXCEPT_ENTRY
	m_pBaseSchema = g_pECVSchema;
XEXCEPT_EXIT
}
XSEMI_ECV::~XSEMI_ECV()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

XRichData& XSEMI_ECV::get_EcMax()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_EcMax;
XEXCEPT_EXIT
}
XRichData& XSEMI_ECV::get_EcMin()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_EcMin;
XEXCEPT_EXIT
}
XRichData& XSEMI_ECV::get_EcDef()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_EcDef;
XEXCEPT_EXIT
}

bool XSEMI_ECV::get_SetEx() 
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_bSetEx;
XEXCEPT_EXIT
}
void XSEMI_ECV::put_SetEx(bool val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_bSetEx = val;
XEXCEPT_EXIT
}

bool XSEMI_ECV::get_Verify() 
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_bVerify;
XEXCEPT_EXIT
}
void XSEMI_ECV::put_Verify(bool val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_bVerify = val;
XEXCEPT_EXIT
}

bool XSEMI_ECV::isDirtyMember()
{
XEXCEPT_ENTRY
	if( XSEMI_V::isDirtyMember() == true ) {
		return true;
	}
	if( m_EcMax.isDirty() == true ) {
		return true;
	}
	if( m_EcMin.isDirty() == true ) {
		return true;
	}
	if( m_EcDef.isDirty() == true ) {
		return true;
	}
	return this->isDirty();
XEXCEPT_EXIT
}

void XSEMI_ECV::resetDirtyMember()
{
XEXCEPT_ENTRY
	XSEMI_V::resetDirtyMember();
	m_EcMax.resetDirty();
	m_EcMin.resetDirty();
	m_EcDef.resetDirty();
	this->resetDirty();
XEXCEPT_EXIT
}
void XSEMI_ECV::setDetail(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_V::setDetail(pAtom);
	pAtom->getRData( XSEMI_DATA_CLMN_EcMax, this->m_EcMax );
	pAtom->getRData( XSEMI_DATA_CLMN_EcMin, this->m_EcMin );
	pAtom->getRData( XSEMI_DATA_CLMN_EcDef, this->m_EcDef );
	m_bSetEx  = pAtom->getUint(XSEMI_DATA_CLMN_SET_EX) == 1;
	m_bVerify = pAtom->getUint(XSEMI_DATA_CLMN_ECVerify) == 1;
XEXCEPT_EXIT
}
// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_ECV::getAtomName()
{
XEXCEPT_ENTRY
	return _T("EC");
XEXCEPT_EXIT
}
int XSEMI_ECV::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_EC_VALUE+1;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_ECV::getString(int index)
{
XEXCEPT_ENTRY
	if( index > (getColmnCount()-1) ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XSEMI_GemDataAtom::getString(g_pECVSchema, index, getAtomValPtr( index ), &m_atomTmpBuff);
XEXCEPT_EXIT
}
void* XSEMI_ECV::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	if( index <= XSEMI_V::ATOM_POS_VVALUE ) {
		return XSEMI_V::getAtomValPtr( index );
	}
	switch( index ) {
	case ATOM_POS_EC_MAX:
		return &m_EcMax;
		break;
	case ATOM_POS_EC_MIN:
		return &m_EcMin;
		break;
	case ATOM_POS_EC_DEF:
		return &m_EcDef;
		break;
	case ATOM_POS_EC_SET_EX:
		return &m_bSetEx;
		break;
	case ATOM_POS_EC_VERIFY:
		return &m_bVerify;
		break;
	case ATOM_POS_EC_VALUE:
		return (XRichData*)this;;
		break;
	}
	return NULL;
XEXCEPT_EXIT
}
void XSEMI_ECV::makeInsertString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeInsertString( stream );

	int	i;
	stream << _T("@@@") << std::endl;
	stream << _T("insert into ") << getAtomName() << _T("(ECID ");
	for(i = ATOM_POS_EC_MAX; i < getColmnCount(); i++) {
		stream << _T(", ");
		stream << getColmnName( i );
	}
	stream << _T(") values(");
	convSQLString( this->get_VID(), stream );
	for(i = ATOM_POS_EC_MAX; i < getColmnCount(); i++) {
		stream << _T(", ");
		convClmnSQLString(i, stream);
	}
	stream << _T(");");
XEXCEPT_EXIT
}
void XSEMI_ECV::makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeUpdateString( stream );

	int	i;
	int iSetCnt = 0;

	stream << _T("@@@") << std::endl;
	stream << _T("update ") << getAtomName() << _T(" set	 ");

	for(i = ATOM_POS_EC_MAX; i < getColmnCount(); i++) {
		if( isColumnKey(i) == false ) {
			if( iSetCnt != 0 ) {
				stream << _T(", ");
			}
			stream << getColmnName( i ) << _T("=");

			convClmnSQLString(i, stream);

			iSetCnt ++;
		}
	}
	stream << _T(" where ECID=");
	convSQLString( this->get_VID(), stream );
	stream << _T(";");
XEXCEPT_EXIT
}
void XSEMI_ECV::makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeDeleteString( stream );

	stream << _T("@@@") << std::endl;

	stream << _T("delete from ") << getAtomName() << _T(" where ECID=");
	convSQLString( this->get_VID(), stream );
	stream << _T(";");
XEXCEPT_EXIT
}

//-----------------------------------------------------------------------------------------
//
//	XSEMI_SV
//
//-----------------------------------------------------------------------------------------
XSEMI_CONST_SCHEMA	XSEMI_SV::g_pSVSchema[] = {
		{XSEMI_DATA_CLMN_VID,			XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_Name,			XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DisplayName,	XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DataUnit,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DataType,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Mode,			XDATA_TYPE_U1,		XSEMI_CONST_SCHEMA_RTYPE_UINT1, false}
	,	{XSEMI_DATA_CLMN_InitValue,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_Category,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Comment,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_VARIABLE_GRP,	XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_VPRIVATE,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_GET_EX,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_Enable,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_VValue,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}

	,	{XSEMI_DATA_CLMN_SizeMax,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_SizeMin,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_LimitMax,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_LimitMin,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_CeLimits,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}

	,	{_T(""),						XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_SV::XSEMI_SV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded /*= false*/)
	:	XSEMI_V(vid, p_name, type, XSEMI_V::SV, bLoaded)
	,	m_nSizeMax( 0 )
	,	m_nSizeMin( 0 )
	,	m_limitMax( type )
	,	m_limitMin( type )
	,	m_nCeLimit( 0 )
{
XEXCEPT_ENTRY
	m_pBaseSchema = g_pSVSchema;
XEXCEPT_EXIT
}
XSEMI_SV::~XSEMI_SV()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

XD_UINT4 XSEMI_SV::get_SizeMax() 
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_nSizeMax;
XEXCEPT_EXIT
}
void XSEMI_SV::put_SizeMax(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_nSizeMax = val;
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_SV::get_SizeMin() 
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_nSizeMin;
XEXCEPT_EXIT
}
void XSEMI_SV::put_SizeMin(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_nSizeMin = val;
XEXCEPT_EXIT
}


XRichData& XSEMI_SV::get_limitMax()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_limitMax;
XEXCEPT_EXIT
}
XRichData& XSEMI_SV::get_limitMin()
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_limitMin;
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_SV::get_CeLimit() 
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_nCeLimit;
XEXCEPT_EXIT
}
void XSEMI_SV::put_CeLimit(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_nCeLimit = val;
XEXCEPT_EXIT
}

bool XSEMI_SV::isDirtyMember()
{
XEXCEPT_ENTRY
	if( XSEMI_V::isDirtyMember() == true ) {
		return true;
	}
	if( m_limitMax.isDirty() == true ) {
		return true;
	}
	if( m_limitMin.isDirty() == true ) {
		return true;
	}
	return this->isDirty();
XEXCEPT_EXIT
}

void XSEMI_SV::resetDirtyMember()
{
XEXCEPT_ENTRY
	XSEMI_V::resetDirtyMember();
	m_limitMax.resetDirty();
	m_limitMin.resetDirty();
	this->resetDirty();
XEXCEPT_EXIT
}

void XSEMI_SV::setDetail(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_V::setDetail(pAtom);

	pAtom->getRData( XSEMI_DATA_CLMN_LimitMax, this->m_limitMax );
	pAtom->getRData( XSEMI_DATA_CLMN_LimitMin, this->m_limitMin );
	m_nSizeMax = pAtom->getUint(XSEMI_DATA_CLMN_SizeMax);
	m_nSizeMin = pAtom->getUint(XSEMI_DATA_CLMN_SizeMin);
	m_nCeLimit = pAtom->getUint(XSEMI_DATA_CLMN_CeLimits) ;

XEXCEPT_EXIT
}
// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_SV::getAtomName()
{
XEXCEPT_ENTRY
	return _T("SV");
XEXCEPT_EXIT
}
int XSEMI_SV::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_SV_LIMIT_EVENT+1;
XEXCEPT_EXIT
}

const TCHAR* XSEMI_SV::getString(int index)
{
XEXCEPT_ENTRY
	if( index > (getColmnCount()-1) ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XSEMI_GemDataAtom::getString(m_pBaseSchema, index, getAtomValPtr( index ), &m_atomTmpBuff);
XEXCEPT_EXIT
}
void* XSEMI_SV::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	if( index <= XSEMI_V::ATOM_POS_VVALUE ) {
		return XSEMI_V::getAtomValPtr( index );
	}
	switch( index ) {
	case ATOM_POS_SV_SIZE_MAX:
		return &m_nSizeMax;
		break;
	case ATOM_POS_SV_SIZE_MIN:
		return &m_nSizeMin;
		break;
	case ATOM_POS_SV_LIMIT_MAX:
		return &m_limitMax;
		break;
	case ATOM_POS_SV_LIMIT_MIN:
		return &m_limitMin;
		break;
	case ATOM_POS_SV_LIMIT_EVENT:
		return &m_nCeLimit;
		break;
	}
	return NULL;
XEXCEPT_EXIT
}
void XSEMI_SV::makeInsertString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeInsertString( stream );

	int	i;
	stream << _T("@@@") << std::endl;
	stream << _T("insert into ") << getAtomName() << _T("(SVID ");
	for(i = ATOM_POS_SV_SIZE_MAX; i < getColmnCount(); i++) {
		stream << _T(", ");
		stream << getColmnName( i );
	}
	stream << _T(") values(");
	convSQLString( this->get_VID(), stream );
	for(i = ATOM_POS_SV_SIZE_MAX; i < getColmnCount(); i++) {
		stream << _T(", ");
		convClmnSQLString(i, stream);
	}
	stream << _T(");");
XEXCEPT_EXIT
}
void XSEMI_SV::makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeUpdateString( stream );



	int	i;
	int iSetCnt = 0;

	stream << _T("@@@") << std::endl;
	stream << _T("update ") << getAtomName() << _T(" set	 ");

	for(i = ATOM_POS_SV_SIZE_MAX; i < getColmnCount(); i++) {
		if( isColumnKey(i) == false ) {
			if( iSetCnt != 0 ) {
				stream << _T(", ");
			}
			stream << getColmnName( i ) << _T("=");

			convClmnSQLString(i, stream);

			iSetCnt ++;
		}
	}
	stream << _T(" where SVID=");
	convSQLString( this->get_VID(), stream );
	stream << _T(";");
XEXCEPT_EXIT
}
void XSEMI_SV::makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeDeleteString( stream );

	stream << _T("@@@") << std::endl;
	stream << _T("delete from ") << getAtomName() << _T(" where SVID=");
	convSQLString( this->get_VID(), stream );
	stream << _T(";");
XEXCEPT_EXIT
}




//
//	XSEMI_DV
//
XSEMI_CONST_SCHEMA	XSEMI_DV::g_pDVSchema[] = {
		{XSEMI_DATA_CLMN_VID,			XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_Name,			XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DisplayName,	XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DataUnit,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DataType,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Mode,			XDATA_TYPE_U1,		XSEMI_CONST_SCHEMA_RTYPE_UINT1, false}
	,	{XSEMI_DATA_CLMN_InitValue,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}
	,	{XSEMI_DATA_CLMN_Category,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Comment,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_VARIABLE_GRP,	XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_VPRIVATE,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_GET_EX,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_Enable,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{XSEMI_DATA_CLMN_VValue,		XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, false}

	,	{XSEMI_DATA_CLMN_SizeMax,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_SizeMin,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}

	,	{_T(""),							XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_DV::XSEMI_DV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded /*= false*/)
	:	XSEMI_V(vid, p_name, type, XSEMI_V::DV, bLoaded)
	,	m_nSizeMax( 0 )
	,	m_nSizeMin( 0 )
{
XEXCEPT_ENTRY
	m_pBaseSchema = g_pDVSchema;
XEXCEPT_EXIT
}
XSEMI_DV::~XSEMI_DV()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

XD_UINT4 XSEMI_DV::get_SizeMax() 
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_nSizeMax;
XEXCEPT_EXIT
}
void XSEMI_DV::put_SizeMax(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_nSizeMax = val;
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_DV::get_SizeMin() 
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	return m_nSizeMin;
XEXCEPT_EXIT
}
void XSEMI_DV::put_SizeMin(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_nSizeMin = val;
XEXCEPT_EXIT
}

void XSEMI_DV::setDetail(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_V::setDetail(pAtom);

	m_nSizeMax = pAtom->getUint(XSEMI_DATA_CLMN_SizeMax);
	m_nSizeMin = pAtom->getUint(XSEMI_DATA_CLMN_SizeMin);
XEXCEPT_EXIT
}
// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_DV::getAtomName()
{
XEXCEPT_ENTRY
	return _T("DV");
XEXCEPT_EXIT
}
int XSEMI_DV::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_DV_SIZE_MIN+1;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_DV::getString(int index)
{
XEXCEPT_ENTRY
	if( index > (getColmnCount()-1) ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XSEMI_GemDataAtom::getString(m_pBaseSchema, index, getAtomValPtr( index ), &m_atomTmpBuff);
XEXCEPT_EXIT
}
void* XSEMI_DV::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( (m_loadedFlg & LD_FLG_DETAIL) == 0 ) {
		this->innerLoadDetail();
	}
	if( index <= XSEMI_V::ATOM_POS_VVALUE ) {
		return XSEMI_V::getAtomValPtr( index );
	}
	switch( index ) {
	case ATOM_POS_DV_SIZE_MAX:
		return &m_nSizeMax;
		break;
	case ATOM_POS_DV_SIZE_MIN:
		return &m_nSizeMin;
		break;
	}
	return NULL;
XEXCEPT_EXIT
}
void XSEMI_DV::makeInsertString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeInsertString( stream );

	int	i;
	stream << _T("@@@") << std::endl;
	stream << _T("insert into ") << getAtomName() << _T("(DVID ");
	for(i = ATOM_POS_DV_SIZE_MAX; i < getColmnCount(); i++) {
		stream << _T(", ");
		stream << getColmnName( i );
	}
	stream << _T(") values(");
	convSQLString( this->get_VID(), stream );
	for(i = ATOM_POS_DV_SIZE_MAX; i < getColmnCount(); i++) {
		stream << _T(", ");
		convClmnSQLString(i, stream);
	}
	stream << _T(");");
XEXCEPT_EXIT
}
void XSEMI_DV::makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeUpdateString( stream );



	int	i;
	int iSetCnt = 0;
	int iWhereCnt = 0;

	stream << _T("@@@") << std::endl;
	stream << _T("update ") << getAtomName() << _T(" set	 ");

	for(i = ATOM_POS_DV_SIZE_MAX; i < getColmnCount(); i++) {
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
	stream << _T(" where DVID=");
	convSQLString( this->get_VID(), stream );
	stream << _T(";");
XEXCEPT_EXIT
}
void XSEMI_DV::makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_V::makeDeleteString( stream );

	stream << _T("@@@") << std::endl;
	stream << _T("delete from ") << getAtomName();
	stream << _T(" where DVID=");
	convSQLString( this->get_VID(), stream );
	stream << _T(";");
XEXCEPT_EXIT
}



//////////////////////////////////////////////////////////////////////////////////////////
//
//	XSEMI_VVAL
//
//////////////////////////////////////////////////////////////////////////////////////////
XSEMI_CONST_SCHEMA	XSEMI_VVAL::g_pSchema[] = {
		{XSEMI_DATA_CLMN_VID,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_NULL, true}
	,	{XSEMI_DATA_CLMN_VValue,	XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_RDATA, true}
	,	{XSEMI_DATA_CLMN_Comment,	XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_Enable,	XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{_T(""),					XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_VVAL::XSEMI_VVAL(XSEMI_GemConstObject* pParent, XDATA_TYPE type)
	:	XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::VVAL)
	,	XRichData(type)
	,	m_bEnable(true)
	,	m_strComment("")
	,	m_pos( NULL )
{
XEXCEPT_ENTRY
	m_pBaseSchema = g_pSchema;
	m_pParentObject = pParent;
XEXCEPT_EXIT
}


const TCHAR* XSEMI_VVAL::get_Comment()
{
XEXCEPT_ENTRY
	return m_strComment;
XEXCEPT_EXIT
}
void XSEMI_VVAL::put_Comment(const TCHAR* val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strComment = val;
XEXCEPT_EXIT
}
bool XSEMI_VVAL::get_Enable()
{
XEXCEPT_ENTRY
	return m_bEnable;
XEXCEPT_EXIT
}
void XSEMI_VVAL::put_Enable(bool val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_bEnable = val;
XEXCEPT_EXIT
}
XSEMI_VVAL* XSEMI_VVAL::nextSibling()
{
XEXCEPT_ENTRY
	XPOSITION pos = m_pos;
	if( m_pParentObject == NULL || pos == NULL ) {
		return NULL;
	}
	if( m_pParentObject->get_ObjectType() != XSEMI_GemConstObject::VID ) {
		return NULL;
	}
	XSEMI_VVAL *pItem = (XSEMI_VVAL*)((XSEMI_V*)m_pParentObject)->m_list.GetNext( pos );
	if( pos == NULL ) {
		return NULL;
	}
	return (XSEMI_VVAL*)((XSEMI_V*)m_pParentObject)->m_list.GetNext( pos );
XEXCEPT_EXIT
}
const TCHAR* XSEMI_VVAL::getAtomName()
{
XEXCEPT_ENTRY
	return _T("VVALS");
XEXCEPT_EXIT
}
int XSEMI_VVAL::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_ENABLE+1;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_VVAL::getString(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	return XSEMI_GemDataAtom::getString(g_pSchema, index, getAtomValPtr( index ), &m_atomTmpBuff);
XEXCEPT_EXIT
}
XD_UINT4    XSEMI_VVAL::getUint(int index)
{
XEXCEPT_ENTRY
	if( XSEMI_V::ATOM_POS_OBJECT_TYPE == index ) {
		// オブジェクトタイプ
		return this->get_ObjectType();
	}
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	if( ATOM_POS_VID == index ) {
		XSEMI_V* pVID = (XSEMI_V*)this->getParentByType( XSEMI_GemConstObject::VID );
		if( pVID == NULL ) {
			XEXCEPT_THROW_STR(XExcept_XDATA, _T("VID object is not found. on parent."), 0, index);
		}
		return pVID->get_VID();
	}
	return XSEMI_GemConstObjectAtom::getUint(index);
XEXCEPT_EXIT
}
void* XSEMI_VVAL::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	switch( index ) {
	case ATOM_POS_VID:
		return NULL;
		break;
	case ATOM_POS_VALUE:
		return (XRichData*)this;;
		break;
	case ATOM_POS_COMMENT:
		return &m_strComment;
		break;
	case ATOM_POS_ENABLE:
		return &m_bEnable;
		break;
	}
	return NULL;
XEXCEPT_EXIT
}



//-----------------------------------------------------------------------------------
//
//	XSEMI_VARIABLE_GRP
//
//-----------------------------------------------------------------------------------
XSEMI_CONST_SCHEMA	XSEMI_VARIABLE_GRP::g_pSchema[] = {
		{XSEMI_DATA_CLMN_GRPID,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_Name,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, true}
	,	{XSEMI_DATA_CLMN_Comment,	XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{_T(""),					XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_VARIABLE_GRP::XSEMI_VARIABLE_GRP(XD_UINT4 id, const TCHAR* p_name)
	:	XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::VARIABLE_GRP)
	,	m_nID(id)
	,	m_strName(p_name)
	,	m_strComment("")
{
XEXCEPT_ENTRY
	m_pBaseSchema = g_pSchema;
XEXCEPT_EXIT
}

XSEMI_VARIABLE_GRP::~XSEMI_VARIABLE_GRP()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_VARIABLE_GRP::get_ID() const
{
XEXCEPT_ENTRY
	return m_nID;
XEXCEPT_EXIT
}

const TCHAR* XSEMI_VARIABLE_GRP::get_Name()
{
XEXCEPT_ENTRY
	return m_strName;
XEXCEPT_EXIT
}

void XSEMI_VARIABLE_GRP::put_Name(const TCHAR* val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strName = val;
XEXCEPT_EXIT
}

const TCHAR* XSEMI_VARIABLE_GRP::get_Comment()
{
XEXCEPT_ENTRY
	return m_strComment;
XEXCEPT_EXIT
}
void XSEMI_VARIABLE_GRP::put_Comment(const TCHAR* val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strComment = val;
XEXCEPT_EXIT
}

// overwrite	XSEMI_GemConstObjectAtom
const TCHAR* XSEMI_VARIABLE_GRP::getAtomName()
{
XEXCEPT_ENTRY
	return _T("VARIABLE_GRP");
XEXCEPT_EXIT
}
int XSEMI_VARIABLE_GRP::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_COMMENT+1;
XEXCEPT_EXIT
}
void* XSEMI_VARIABLE_GRP::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}
	switch( index ) {
	case ATOM_POS_ID:
		return &m_nID;
		break;
	case ATOM_POS_NAME:
		return &m_strName;
		break;
	case ATOM_POS_COMMENT:
		return &m_strComment;
		break;
	}
	return NULL;
XEXCEPT_EXIT
}

//-----------------------------------------------------------------------------------
//
//	XSEMI_Vs
//
//-----------------------------------------------------------------------------------
XSEMI_Vs::XSEMI_Vs(bool bLinkeDatas)
	:	XSEMI_GemConstObject(XSEMI_GemConstObject::VIDs)
	,	m_bLinkDatas( bLinkeDatas )
{
XEXCEPT_ENTRY
	m_ite = m_vs.begin();
XEXCEPT_EXIT
}
XSEMI_Vs::~XSEMI_Vs()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSEMI_Vs::clear()
{
XEXCEPT_ENTRY
	if( m_bLinkDatas == false ) {
		std::vector<XSEMI_V*>::iterator iteV = m_vs.begin();	
		for( ; iteV != m_vs.end(); iteV++) {
			delete (*iteV);
		}
	}
	m_vs.clear();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_Vs::count() const
{
XEXCEPT_ENTRY
	return m_vs.size();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_Vs::countSV() const
{
XEXCEPT_ENTRY
	return m_SV.size();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_Vs::countECV() const
{
XEXCEPT_ENTRY
	return m_EC.size();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_Vs::countDV() const
{
XEXCEPT_ENTRY
	return m_DV.size();
XEXCEPT_EXIT
}


XSEMI_V* XSEMI_Vs::get(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_vs.size() ) {
		return NULL;
	}
	return m_vs[index];
XEXCEPT_EXIT	
}

XSEMI_ECV* XSEMI_Vs::getECV(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_EC.size() ) {
		return NULL;
	}
	return m_EC[index];
XEXCEPT_EXIT	
}

XSEMI_SV* XSEMI_Vs::getSV(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_SV.size() ) {
		return NULL;
	}
	return m_SV[index];
XEXCEPT_EXIT	
}

XSEMI_DV* XSEMI_Vs::getDV(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_DV.size() ) {
		return NULL;
	}
	return m_DV[index];
XEXCEPT_EXIT	
}


XSEMI_V* XSEMI_Vs::get_byID(XD_UINT4 id)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_V*>::iterator iteV = m_vs.begin();	
	for( ; iteV != m_vs.end(); iteV++) {
		if( (*iteV)->get_VID() == id ) {
			return (*iteV);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}

XSEMI_V* XSEMI_Vs::get_byName(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_V*>::iterator iteV = m_vs.begin();	
	for( ; iteV != m_vs.end(); iteV++) {
		if( XStrBuff::StrCmp((*iteV)->get_Name(), p_name) == 0 ) {
			return (*iteV);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}


XSEMI_SV* XSEMI_Vs::get_SV_byID(XD_UINT4 id)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_SV*>::iterator iteV = m_SV.begin();	
	for( ; iteV != m_SV.end(); iteV++) {
		if( (*iteV)->get_VID() == id ) {
			return (*iteV);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}

XSEMI_ECV* XSEMI_Vs::get_EC_byID(XD_UINT4 id)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_ECV*>::iterator iteV = m_EC.begin();	
	for( ; iteV != m_EC.end(); iteV++) {
		if( (*iteV)->get_VID() == id ) {
			return (*iteV);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}

XSEMI_DV*  XSEMI_Vs::get_DV_byID(XD_UINT4 id)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_DV*>::iterator iteV = m_DV.begin();	
	for( ; iteV != m_DV.end(); iteV++) {
		if( (*iteV)->get_VID() == id ) {
			return (*iteV);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}


XSEMI_V* XSEMI_Vs::getFirstItem()
{
XEXCEPT_ENTRY
	m_ite = m_vs.begin();
	if( m_ite == m_vs.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT	
}
XSEMI_V* XSEMI_Vs::getNextItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_vs.end() ) {
		return NULL;
	}
	m_ite++;
	if( m_ite == m_vs.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_V* XSEMI_Vs::getPrevItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_vs.begin() ) {
		return NULL;
	}
	m_ite--;
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_V* XSEMI_Vs::add(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	if( m_bLinkDatas == true ) {
		return NULL;
	}


	XSEMI_V* pNewItem = NULL;
	XD_UINT4 vid				= pAtom->getUint(XSEMI_DATA_CLMN_VID);
	XStrBuff strName            = pAtom->getString(XSEMI_DATA_CLMN_Name);
	XDATA_TYPE type             = (XDATA_TYPE)pAtom->getUint(XSEMI_DATA_CLMN_DataType);
	XD_UINT4 mode				= pAtom->getUint(XSEMI_DATA_CLMN_Mode);
	XSEMI_V::Class tyClas       = (XSEMI_V::Class)pAtom->getUint(XSEMI_DATA_CLMN_Category);


	pNewItem = get_byID( vid );
	if( pNewItem != NULL ) {
		// 既に同じCEIDがある
		return NULL;
	}

	switch( tyClas ) {
	case XSEMI_V::EC:
		pNewItem = new XSEMI_ECV(vid, strName, type);
		break;
	case XSEMI_V::SV:
		pNewItem = new XSEMI_SV(vid, strName, type);
		break;
	case XSEMI_V::DV:
		pNewItem = new XSEMI_DV(vid, strName, type);
		break;
	}
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->m_pParentObject = this;
	m_vs.push_back( pNewItem );

	// クラス別配列に追加する。
	switch( tyClas )
	{
	case XSEMI_V::SV:
		m_SV.push_back( (XSEMI_SV*)pNewItem );
		break;
	case XSEMI_V::EC:
		m_EC.push_back( (XSEMI_ECV*)pNewItem );
		break;
	case XSEMI_V::DV:
		m_DV.push_back( (XSEMI_DV*)pNewItem );
		break;
	}




	pNewItem->put_Enable( pAtom->getUint(XSEMI_DATA_CLMN_Enable) == 1 );
	pNewItem->put_DisplayName( pAtom->getString(XSEMI_DATA_CLMN_DisplayName) );
	pNewItem->put_Unit( pAtom->getString(XSEMI_DATA_CLMN_DataUnit) );
	pNewItem->put_Comment( pAtom->getString(XSEMI_DATA_CLMN_Comment) );
	pNewItem->put_VariableGrpID( pAtom->getUint(XSEMI_DATA_CLMN_VARIABLE_GRP) );
	pNewItem->put_Private( (XSEMI_V::PRIVAL)pAtom->getUint(XSEMI_DATA_CLMN_VPRIVATE) );
	pNewItem->put_GetEx( pAtom->getUint(XSEMI_DATA_CLMN_GET_EX) == 1 );
	setRiDataFromString( &(pNewItem->get_InitialValue()), pAtom->getString(XSEMI_DATA_CLMN_InitValue));
	pNewItem->setDetail( pAtom );
	return pNewItem;
XEXCEPT_EXIT	
}

XSEMI_V* XSEMI_Vs::addNoDetail(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	if( m_bLinkDatas == true ) {
		return NULL;
	}

	XSEMI_V* pNewItem = NULL;
	XD_UINT4 vid				= pAtom->getUint(XSEMI_DATA_CLMN_VID);
	XStrBuff strName            = pAtom->getString(XSEMI_DATA_CLMN_Name);
	XDATA_TYPE type             = (XDATA_TYPE)pAtom->getUint(XSEMI_DATA_CLMN_DataType);
	XD_UINT4 mode				= pAtom->getUint(XSEMI_DATA_CLMN_Mode);
	XSEMI_V::Class tyClas       = (XSEMI_V::Class)pAtom->getUint(XSEMI_DATA_CLMN_Category);


	pNewItem = get_byID( vid );
	if( pNewItem != NULL ) {
		// 既に同じCEIDがある
		return NULL;
	}

	switch( tyClas ) {
	case XSEMI_V::EC:
		pNewItem = new XSEMI_ECV(vid, strName, type);
		break;
	case XSEMI_V::SV:
		pNewItem = new XSEMI_SV(vid, strName, type);
		break;
	case XSEMI_V::DV:
		pNewItem = new XSEMI_DV(vid, strName, type);
		break;
	}
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->m_pParentObject = this;
	m_vs.push_back( pNewItem );

	// クラス別配列に追加する。
	switch( tyClas )
	{
	case XSEMI_V::SV:
		m_SV.push_back( (XSEMI_SV*)pNewItem );
		break;
	case XSEMI_V::EC:
		m_EC.push_back( (XSEMI_ECV*)pNewItem );
		break;
	case XSEMI_V::DV:
		m_DV.push_back( (XSEMI_DV*)pNewItem );
		break;
	}
	pNewItem->put_Enable( pAtom->getUint(XSEMI_DATA_CLMN_Enable) == 1 );
	pNewItem->put_Private( (XSEMI_V::PRIVAL)pAtom->getUint(XSEMI_DATA_CLMN_VPRIVATE) );
	pNewItem->put_GetEx( pAtom->getUint(XSEMI_DATA_CLMN_GET_EX) == 1 );
	pAtom->getRData( XSEMI_DATA_CLMN_VValue, *((XRichData*)pNewItem));
	if( tyClas == XSEMI_V::EC ) {
		XSEMI_ECV* pEC = (XSEMI_ECV*)pNewItem;
		pAtom->getRData( XSEMI_DATA_CLMN_EcVal, *((XRichData*)pEC));
	}
	return pNewItem;
XEXCEPT_EXIT	
}

XSEMI_V* XSEMI_Vs::add(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, XSEMI_V::Class Class)
{
XEXCEPT_ENTRY
	XSEMI_GemConst* pConst = (XSEMI_GemConst*)this->getParentByType( XSEMI_GemConstObject::CONSTRUCTION );
	if( pConst == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object not found. on parent"), 0, 0);
		return NULL;
	}
	XSEMI_GemMstData* pMater = pConst->get_MasterData();
	if( pMater == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Master Object not found. on parent"), 0, 0);
		return NULL;
	}
	

	XSEMI_V* pNewItem = NULL;
	switch( Class ) {
	case XSEMI_V::EC:
		pNewItem = new XSEMI_ECV(vid, p_name, type);
		break;
	case XSEMI_V::SV:
		pNewItem = new XSEMI_SV(vid, p_name, type);
		break;
	case XSEMI_V::DV:
		pNewItem = new XSEMI_DV(vid, p_name, type);
		break;
	}
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->m_pParentObject = this;

	pMater->updateObject( pNewItem );
	
	m_vs.push_back( pNewItem );

	return pNewItem;
XEXCEPT_EXIT
	return NULL;
}


bool XSEMI_Vs::del(XSEMI_V* pV)
{
XEXCEPT_ENTRY

	XSEMI_GemConst* pConst = (XSEMI_GemConst*)this->getParentByType( XSEMI_GemConstObject::CONSTRUCTION );
	if( pConst == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object not found. on parent"), 0, 0);
		return false;
	}
	XSEMI_GemMstData* pMater = pConst->get_MasterData();
	if( pMater == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Master Object not found. on parent"), 0, 0);
		return false;
	}
	if( pMater->deleteObject(pV) == false ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("fail to delete object from store."), 0, 0);
		return false;
	}

	std::vector<XSEMI_V*>::iterator iteV = m_vs.begin();	
	for( ; iteV != m_vs.end(); iteV++) {
		XSEMI_V* pVDef = ( *iteV );
		if( pVDef == pV ) {

			// クラス別配列から、削除する。
			switch( pVDef->get_Class() )
			{
			case XSEMI_V::SV:
				{
					std::vector<XSEMI_SV*>::iterator iteMapClass;
					iteMapClass = m_SV.begin();
					for( ; iteMapClass != m_SV.end(); iteMapClass++) {
						if( (*iteMapClass) == pV ) {
							m_SV.erase( iteMapClass );
							break;
						}
					}
				}
				break;
			case XSEMI_V::EC:
				{
					std::vector<XSEMI_ECV*>::iterator iteMapClass;
					iteMapClass = m_EC.begin();
					for( ; iteMapClass != m_EC.end(); iteMapClass++) {
						if( (*iteMapClass) == pV ) {
							m_EC.erase( iteMapClass );
							break;
						}
					}
				}
				break;
			case XSEMI_V::DV:
				{
					std::vector<XSEMI_DV*>::iterator iteMapClass;
					iteMapClass = m_DV.begin();
					for( ; iteMapClass != m_DV.end(); iteMapClass++) {
						if( (*iteMapClass) == pV ) {
							m_DV.erase( iteMapClass );
							break;
						}
					}
				}
				break;
			}

			if( m_bLinkDatas == false ) {
				delete pVDef;
			}
			m_vs.erase( iteV );
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}

bool XSEMI_Vs::setRiDataFromString(XRichData* pRdata, const TCHAR* p_str)
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


//
//	XSEMI_VARIABLE_GRPs
//
XSEMI_VARIABLE_GRPs::XSEMI_VARIABLE_GRPs()
	:	XSEMI_GemConstObject(XSEMI_GemConstObject::VARIABLE_GRPs)
{
XEXCEPT_ENTRY
	m_ite = m_vgrps.begin();
XEXCEPT_EXIT
}
XSEMI_VARIABLE_GRPs::~XSEMI_VARIABLE_GRPs()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSEMI_VARIABLE_GRPs::clear()
{
XEXCEPT_ENTRY
	std::vector<XSEMI_VARIABLE_GRP*>::iterator ite = m_vgrps.begin();	
	for( ; ite != m_vgrps.end(); ite++) {
		delete (*ite);
	}
	m_vgrps.clear();
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_VARIABLE_GRPs::count()
{
XEXCEPT_ENTRY
	return m_vgrps.size();
XEXCEPT_EXIT
}
XSEMI_VARIABLE_GRP* XSEMI_VARIABLE_GRPs::get(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_vgrps.size() ) {
		return NULL;
	}
	return m_vgrps[index];
XEXCEPT_EXIT
}
XSEMI_VARIABLE_GRP* XSEMI_VARIABLE_GRPs::get_byID(XD_UINT4 id)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_VARIABLE_GRP*>::iterator ite = m_vgrps.begin();	
	for( ; ite != m_vgrps.end(); ite++) {
		if( (*ite)->get_ID() == id ) {
			return (*ite);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}

XSEMI_VARIABLE_GRP* XSEMI_VARIABLE_GRPs::getFirstItem()
{
XEXCEPT_ENTRY
	m_ite = m_vgrps.begin();
	if( m_ite == m_vgrps.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT	
}
XSEMI_VARIABLE_GRP* XSEMI_VARIABLE_GRPs::getNextItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_vgrps.end() ) {
		return NULL;
	}
	m_ite++;
	if( m_ite == m_vgrps.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_VARIABLE_GRP* XSEMI_VARIABLE_GRPs::getPrevItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_vgrps.begin() ) {
		return NULL;
	}
	m_ite--;
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_VARIABLE_GRP* XSEMI_VARIABLE_GRPs::add(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_VARIABLE_GRP* pNewItem = NULL;
	XD_UINT4 id       = pAtom->getUint(XSEMI_DATA_CLMN_GRPID);
	XStrBuff strName    = pAtom->getString(XSEMI_DATA_CLMN_Name);
	XStrBuff strComment = pAtom->getString(XSEMI_DATA_CLMN_Comment);



	pNewItem = get_byID( id );
	if( pNewItem != NULL ) {
		// 既に同じCEIDがある
		return NULL;
	}
	pNewItem = new XSEMI_VARIABLE_GRP(id, strName);
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->put_Comment(strComment);
	pNewItem->m_pParentObject = this;
	m_vgrps.push_back( pNewItem );
	return pNewItem;
XEXCEPT_EXIT
}


bool XSEMI_VARIABLE_GRPs::del(XSEMI_VARIABLE_GRP* pGrp)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_VARIABLE_GRP*>::iterator ite = m_vgrps.begin();	
	for( ; ite != m_vgrps.end(); ite++) {
		XSEMI_VARIABLE_GRP* pGrpDef = ( *ite );
		if( pGrpDef == pGrp ) {
			delete pGrpDef;
			m_vgrps.erase( ite );
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}




