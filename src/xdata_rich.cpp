#include "xlib/xdata_rich.h"
#include "xlib/xeception.h"
#include <assert.h>

XRichDataBase::XRichDataBase(XRichDataBase* pParent)
	:	m_pData(NULL)
	,	m_nDataSize(0)
	,	m_pParent(pParent)
	,	m_bDirty( false )
	,	m_nLengthByte( -1 )
	,	m_pUserData( NULL )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XRichDataBase::~XRichDataBase()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
XDATA_TYPE XRichDataBase::getType() const
{
XEXCEPT_ENTRY
	return m_type;
XEXCEPT_EXIT
}
const char* XRichDataBase::getTypeStringA() const
{
XEXCEPT_ENTRY
	return XData::getDataTypeStringA_From_TYPE( m_type );
/*
	static const char *pTypeString[] = { ""
						, "L"
						, "B"
						, "BOOLEAN"
						, "A"
						, "J"
						, "UTF16"
						, "U1"
						, "U2"
						, "U4"
						, "U8"
						, "I1"
						, "I2"
						, "I4"
						, "I8"
						, "F4"
						, "F8"
		};

	switch(m_type) {
	case XDATA_TYPE_L:
		return pTypeString[1];
	case XDATA_TYPE_B:
		return pTypeString[2];
	case XDATA_TYPE_BOOL:
		return pTypeString[3];
	case XDATA_TYPE_A:
		return pTypeString[4];
	case XDATA_TYPE_J:
		return pTypeString[5];
	case XDATA_TYPE_UTF16:
		return pTypeString[6];
	case XDATA_TYPE_U1:
		return pTypeString[7];
	case XDATA_TYPE_U2:
		return pTypeString[8];
	case XDATA_TYPE_U4:
		return pTypeString[9];
	case XDATA_TYPE_U8:
		return pTypeString[10];
	case XDATA_TYPE_I1:
		return pTypeString[11];
	case XDATA_TYPE_I2:
		return pTypeString[12];
	case XDATA_TYPE_I4:
		return pTypeString[13];
	case XDATA_TYPE_I8:
		return pTypeString[14];
	case XDATA_TYPE_F4:
		return pTypeString[15];
	case XDATA_TYPE_F8:
		return pTypeString[16];
	}

	return pTypeString[0];
*/
XEXCEPT_EXIT
}
bool XRichDataBase::removeChiled(XRichDataBase* pItem)
{
XEXCEPT_ENTRY
	std::vector<XRichDataBase*>::iterator ite;
	ite = m_children.begin();
	for( ; ite != m_children.end(); ite++ ) {
		XRichDataBase *p = (*ite);
		if( p == pItem) {
			delete p;
			m_children.erase(ite);
			m_bDirty = true;
			return true;
		}
	}
//	XPOSITION pos = m_children.GetHeadPosition();
//	for( ; pos != NULL;){
//		XRichDataBase *p = (XRichDataBase*)m_children.GetNext(pos);
//		if( p == pItem) {
//			delete p;
//			m_children.RemoveAt( pos );
//			return true;
//		}
//	}
	m_error = XD2_ERRIT;
	if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
	return false;
XEXCEPT_EXIT
}



bool XRichDataBase::atachChild(XRichDataBase* pItem, XRichDataBase* pPreItem /*= NULL*/ )
{
XEXCEPT_ENTRY
	if( pPreItem == NULL ) {
		pItem->m_pParent = this;	//親の書き換え
		m_children.push_back(pItem);
		m_bDirty = true;
		return true;
	}

	std::vector<XRichDataBase*>::iterator ite;
	ite = m_children.begin();
	for( ; ite != m_children.end(); ite++ ) {
		XRichDataBase *p = (*ite);
		if( p == pPreItem) {
			ite++;
			m_children.insert(ite, pItem);
			pItem->m_pParent = this;	//親の書き換え
			m_bDirty = true;
			return true;
		}
	}
	m_error = XD2_ERRIT;
	if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
	return false;
XEXCEPT_EXIT
}
bool XRichDataBase::detachChild(XRichDataBase* pItem)
{
XEXCEPT_ENTRY
	std::vector<XRichDataBase*>::iterator ite;
	ite = m_children.begin();
	for( ; ite != m_children.end(); ite++ ) {
		XRichDataBase *p = (*ite);
		if( p == pItem) {
			m_children.erase(ite);
			pItem->m_pParent = NULL;	//親の書き換え
			m_bDirty = true;
			return true;
		}
	}
	m_error = XD2_ERRIT;
	if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
	return false;
XEXCEPT_EXIT
}



XD_UINT4 XRichDataBase::getChiledCount() const
{
XEXCEPT_ENTRY
	return m_children.size();
//	return m_children.GetCount();
XEXCEPT_EXIT
}
XRichDataBase* XRichDataBase::getParent() const
{
XEXCEPT_ENTRY
	return m_pParent;
XEXCEPT_EXIT
}
int XRichDataBase::getIndexFromParent() const
{
	int index = -1;	
XEXCEPT_ENTRY
	if( m_pParent== NULL ) {
		return -1;
	}
	std::vector<XRichDataBase*>& array = m_pParent->m_children;
	std::vector<XRichDataBase*>::iterator ite = array.begin();
	for(int i = 0 ; ite != array.end(); ite++, i++) {
		if( *ite == this ) {
			index = i;
			break;
		}
	}
XEXCEPT_EXIT
	return index;
}

void XRichDataBase::freeValue()
{
XEXCEPT_ENTRY
	if( m_pData != NULL ) {
		delete []m_pData;
	}
	m_pData = NULL;
	m_nDataSize = 0;
	m_bDirty = true;
XEXCEPT_EXIT
}

void XRichDataBase::clear()
{
XEXCEPT_ENTRY
	std::vector<XRichDataBase*>::iterator ite;
	ite = m_children.begin();
	for( ; ite != m_children.end(); ite++ ) {
		XRichDataBase *p = (*ite);
		delete p;
	}
	m_children.clear();
//	XPOSITION pos = m_children.GetHeadPosition();
//	for( ; pos != NULL;){
//		XRichDataBase *p = (XRichDataBase*)m_children.GetNext(pos);
//		delete p;
//	}
//	m_children.RemoveAll();
	this->freeValue();
XEXCEPT_EXIT
}

XStrBuff XRichDataBase::toString()
{
XEXCEPT_ENTRY
	XInflateStreamBuff buff(256, 512);
	std::ostream stream(&buff);

	XStrBuff str( _T("") );
	switch( m_type ) {
	case XDATA_TYPE_B:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				char sz[50] = {0};
				if( i != 0 ) {
					stream << " ";
				}
				XStrBuff::__sprintf(sz, XCOUNTOF(sz), "0x%02x", getValueUInt1( i ));
				stream << sz;
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_BOOL:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				if( getValueUInt1(i) == 0 ) {
					stream << "false";
				} else {
					stream << "true";
				}
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
		str = getValueString();
		break;
	case XDATA_TYPE_UTF16:
		str = getValueString();
		break;
	case XDATA_TYPE_U1:
	case XDATA_TYPE_UX:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				stream << (unsigned int)getValueUInt1( i );
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_I1:
	case XDATA_TYPE_IX:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				stream << (int)getValueInt1( i );
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_U2:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				stream << getValueUInt2( i );
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_I2:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				stream << getValueInt2( i );
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_U4:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				stream << getValueUInt4( i );
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_I4:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				stream << getValueInt4( i );
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_F4:
	case XDATA_TYPE_FX:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				stream << getValueFloat4( i );
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_U8:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				char sz[50] = {0};
				if( i != 0 ) {
					stream << " ";
				}
				XStrBuff::__sprintf(sz, XCOUNTOF(sz), "%I64u", getValueUInt8( i ));
				stream << sz;
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_I8:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				char sz[50] = {0};
				if( i != 0 ) {
					stream << " ";
				}
				XStrBuff::__sprintf(sz, XCOUNTOF(sz),  "%I64d", getValueInt8( i ));
				stream << sz;
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_F8:
		{
			XD_UINT4 i = 0;
			for(i = 0; i < this->getDataCount(); i++) {
				if( i != 0 ) {
					stream << " ";
				}
				stream << getValueFloat8( i );
			}
			str = buff.toString();
		}
		break;
	case XDATA_TYPE_L:
		break;
	default:
		XEXCEPT_THROW_STR(XExcept_XDATA, (const TCHAR*)XStrBuff::FormatS(_T("unknown data type:(%X)"),m_type), 0, m_type);
		break;
	}
	return str;
XEXCEPT_EXIT
}

int XRichDataBase::compare(XRichDataBase* pDst)
{
XEXCEPT_ENTRY
	XD_UINT4	ulArrayCnt = 0;
	XD_UINT4	i = 0;
	if( pDst->getType() != this->getType() ) {
		// データタイプが異なる
		if( this->getType() == XDATA_TYPE_UX ) {
			switch( pDst->getType() ) {
			case XDATA_TYPE_U1:
			case XDATA_TYPE_U2:
			case XDATA_TYPE_U4:
			case XDATA_TYPE_U8:
			case XDATA_TYPE_UX:
				break;
			default:
				return  -2;
			}
		} else
		if( this->getType() == XDATA_TYPE_IX ) {
			switch( pDst->getType() ) {
			case XDATA_TYPE_I1:
			case XDATA_TYPE_I2:
			case XDATA_TYPE_I4:
			case XDATA_TYPE_I8:
			case XDATA_TYPE_IX:
				break;
			default:
				return  -2;
			}
		} else
		if( this->getType() == XDATA_TYPE_FX ) {
			switch( this->getType() ) {
			case XDATA_TYPE_F4:
			case XDATA_TYPE_F8:
			case XDATA_TYPE_FX:
				break;
			default:
				return  -2;
			}
		}if( pDst->getType() == XDATA_TYPE_UX ) {
			switch( this->getType() ) {
			case XDATA_TYPE_U1:
			case XDATA_TYPE_U2:
			case XDATA_TYPE_U4:
			case XDATA_TYPE_U8:
			case XDATA_TYPE_UX:
				break;
			default:
				return  -2;
			}
		} else
		if( pDst->getType() == XDATA_TYPE_IX ) {
			switch( this->getType() ) {
			case XDATA_TYPE_I1:
			case XDATA_TYPE_I2:
			case XDATA_TYPE_I4:
			case XDATA_TYPE_I8:
			case XDATA_TYPE_IX:
				break;
			default:
				return  -2;
			}
		} else
		if( pDst->getType() == XDATA_TYPE_FX ) {
			switch( this->getType() ) {
			case XDATA_TYPE_F4:
			case XDATA_TYPE_F8:
			case XDATA_TYPE_FX:
				break;
			default:
				return  -2;
			}
		} else {
			return -2;
		}
	}
	switch( this->getType() ) {
	case XDATA_TYPE_END:
	case XDATA_TYPE_L:
		// 比較できないデータタイプ
		return -3;
	}
	if( pDst->getDataCount() != this->getDataCount() ) {
		// 配列数が異なる
		return -4;
	}

	ulArrayCnt = pDst->getDataCount();
	for( i = 0; i < ulArrayCnt; i++) {
		switch( this->getType() ) {
		case XDATA_TYPE_U1:
		case XDATA_TYPE_UX:
			{
				XD_UINT1 src = this->getValueUInt1(i);
				XD_UINT1 dst = pDst->getValueUInt1(i);
				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_I1:
		case XDATA_TYPE_IX:
			{
				XD_INT1 src = this->getValueInt1(i);
				XD_INT1 dst = pDst->getValueInt1(i);
				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_U2:
		case XDATA_TYPE_UTF16:
			{
				XD_UINT2 src = this->getValueUInt2(i);
				XD_UINT2 dst = 0;
				if( pDst->getType() == XDATA_TYPE_UX ) {
					dst = (XD_UINT2)pDst->getValueUInt1(i);
				} else {
					dst = pDst->getValueUInt2(i);
				}


				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_I2:
			{
				XD_INT2 src = this->getValueInt2(i);
				XD_INT2 dst = 0;
				if( pDst->getType() == XDATA_TYPE_IX ) {
					dst = (XD_INT2)pDst->getValueInt1(i);
				} else {
					dst = pDst->getValueInt2(i);
				}

				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_U4:
			{
				XD_UINT4 src = this->getValueUInt4(i);
				XD_UINT4 dst = 0;
				if( pDst->getType() == XDATA_TYPE_UX ) {
					dst = (XD_UINT4)pDst->getValueUInt1(i);
				} else {
					dst = pDst->getValueUInt4(i);
				}

				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_I4:
			{
				XD_INT4 src = this->getValueInt4(i);
				XD_INT4 dst = 0;
				if( pDst->getType() == XDATA_TYPE_IX ) {
					dst = (XD_INT4)pDst->getValueInt1(i);
				} else {
					dst = pDst->getValueInt4(i);
				}

				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_U8:
			{
				XD_UINT8 src = this->getValueUInt8(i);
				XD_UINT8 dst = pDst->getValueUInt8(i);
				if( pDst->getType() == XDATA_TYPE_UX ) {
					dst = (XD_UINT8)pDst->getValueUInt1(i);
				} else {
					dst = pDst->getValueUInt8(i);
				}

				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_I8:
			{
				XD_INT8 src = this->getValueInt8(i);
				XD_INT8 dst = pDst->getValueInt8(i);
				if( pDst->getType() == XDATA_TYPE_IX ) {
					dst = (XD_INT8)pDst->getValueInt1(i);
				} else {
					dst = pDst->getValueInt8(i);
				}

				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_F4:
			{
				XD_FLOAT4 src = this->getValueFloat4(i);
				XD_FLOAT4 dst = pDst->getValueFloat4(i);
				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		case XDATA_TYPE_F8:
			{
				XD_FLOAT8 src = this->getValueFloat8(i);
				XD_FLOAT8 dst = pDst->getValueFloat8(i);
				if( pDst->getType() == XDATA_TYPE_FX ) {
					dst = (XD_FLOAT8)pDst->getValueFloat4(i);
				} else {
					dst = pDst->getValueFloat8(i);
				}
				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		default:
			{
				XD_UINT1 src = this->getValueUInt1(i);
				XD_UINT1 dst = pDst->getValueUInt1(i);
				if( src < dst ) {
					return -1;
				} else 
				if( src > dst ) {
					return 1;
				}
			}
			break;
		}
	}
	return 0;
XEXCEPT_EXIT
}

bool XRichDataBase::isDirty() const
{
XEXCEPT_ENTRY
	return m_bDirty;
XEXCEPT_EXIT
}
void XRichDataBase::resetDirty()
{
XEXCEPT_ENTRY
	m_bDirty = false;
XEXCEPT_EXIT
}

XD_INT1 XRichDataBase::getLengthByte()
{
XEXCEPT_ENTRY
	return m_nLengthByte;
XEXCEPT_EXIT
}
void XRichDataBase::setLengthByte(XD_INT1 val) 
{
XEXCEPT_ENTRY
	switch( val ) {
	case -1:
	case 1:
	case 2:
	case 3:
		break;
	default:
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS( _T("invalid length byte") ), 0, val);
		return ;
	}
	m_nLengthByte = val;
XEXCEPT_EXIT
}

void* XRichDataBase::getUserData() const
{
XEXCEPT_ENTRY
	return m_pUserData;
XEXCEPT_EXIT
}
void XRichDataBase::setUserData(void* val)
{
XEXCEPT_ENTRY
	m_pUserData = val;
XEXCEPT_EXIT
}



XD_UINT4 XRichDataBase::getDataCount() const
{
XEXCEPT_ENTRY
	switch( m_type ) {
	case XDATA_TYPE_B:
	case XDATA_TYPE_BOOL:
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
	case XDATA_TYPE_U1:
	case XDATA_TYPE_I1:
	case XDATA_TYPE_UX:
	case XDATA_TYPE_IX:
		return m_nDataSize;
		break;
	case XDATA_TYPE_U2:
	case XDATA_TYPE_I2:
	case XDATA_TYPE_UTF16:
		return m_nDataSize / sizeof(XD_INT2);
		break;
	case XDATA_TYPE_U4:
	case XDATA_TYPE_I4:
	case XDATA_TYPE_F4:
	case XDATA_TYPE_FX:
		return m_nDataSize / sizeof(XD_INT4);
		break;
	case XDATA_TYPE_U8:
	case XDATA_TYPE_I8:
	case XDATA_TYPE_F8:
		return m_nDataSize / sizeof(XD_FLOAT8);
		break;
	}
//	m_error = XD2_ERRTY;
	if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
	return 0;
XEXCEPT_EXIT
}
XD_UINT4 XRichDataBase::getBinarySize() const
{
XEXCEPT_ENTRY
	return m_nDataSize;
XEXCEPT_EXIT
}


bool XRichDataBase::assign(XD_UINT4 nValueCount) 
{
XEXCEPT_ENTRY
	XD_UINT4 ulValusSize = 0;
	switch( m_type ) {
	case XDATA_TYPE_L:
		freeValue();
		return true;
	case XDATA_TYPE_B:
	case XDATA_TYPE_BOOL:
	case XDATA_TYPE_U1:
	case XDATA_TYPE_I1:
		ulValusSize = nValueCount;
		break;
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
		ulValusSize = nValueCount+sizeof(char);
		break;
	case XDATA_TYPE_UTF16:
		ulValusSize = nValueCount*sizeof(wchar_t)+sizeof(wchar_t);
		break;
	case XDATA_TYPE_U2:
	case XDATA_TYPE_I2:
		ulValusSize = nValueCount*2;
		break;
	case XDATA_TYPE_U4:
	case XDATA_TYPE_I4:
	case XDATA_TYPE_F4:
		ulValusSize = nValueCount*4;
		break;
	case XDATA_TYPE_U8:
	case XDATA_TYPE_I8:
	case XDATA_TYPE_F8:
		ulValusSize = nValueCount*8;
		break;
	default:
		m_error = XD2_ERRTY;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	freeValue();

	m_pData = new unsigned char[ulValusSize];
	if( m_pData == NULL ) {
		m_error = XD2_ERRMA;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
		return false;
	}
	m_nDataSize = ulValusSize;
	::memset(m_pData, 0 , ulValusSize);
XEXCEPT_EXIT
	return true;
}


bool XRichDataBase::putValueString(const char* pValue, int len /*= -1*/)
{
XEXCEPT_ENTRY
	freeValue();
	if( pValue == NULL ) {
		return true;
	}
	if( len == -1 ) {
		len = ::strlen(pValue);
	}
	return innerPutValue((unsigned char*)pValue, len);
/*
	m_pData = new unsigned char[len+1];
	if( m_pData == NULL ) {
		m_error = XD2_ERRMA;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
		return false;
	}
	::memset(m_pData, 0, len+1);
	::memcpy(m_pData, pValue, len);
	m_nDataSize = len;
	m_bDirty = true;
	return true;
*/
XEXCEPT_EXIT
}
bool XRichDataBase::putValueString(const wchar_t* pValue, int len /*= -1*/)
{
XEXCEPT_ENTRY
	freeValue();
	if( pValue == NULL ) {
		return true;
	}
	XStrBuff strWork(pValue);
	if( len == -1 ) {
		len = ::wcslen(pValue);
	}
	return XRichDataBase::putValueString((const char*)strWork, len);

XEXCEPT_EXIT
}
bool XRichDataBase::putValueStringUTF16(const wchar_t* pValue, int len /*= -1*/)
{
XEXCEPT_ENTRY
	freeValue();
	if( pValue == NULL ) {
		return true;
	}
	if( len == -1 ) {
		len = ::wcslen((const wchar_t*)pValue);
	}

	return innerPutValue((unsigned char*)pValue, len*sizeof(wchar_t));
/*
	unsigned short *pUniData = new unsigned short[len+1];
	m_pData = (unsigned char*)pUniData;
	if( m_pData == NULL ) {
		m_error = XD2_ERRMA;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
		return false;
	}
	::wcscpy((wchar_t*)m_pData, (const wchar_t*)pValue);
	m_nDataSize = len*sizeof(unsigned short);
	m_bDirty = true;
	return true;
*/
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const bool* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount, bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const char* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount, bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_UINT1* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount, bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_INT2* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount*sizeof(XD_INT2), bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_UINT2* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount*sizeof(XD_INT2), bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_INT4* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount*sizeof(XD_INT4), bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_UINT4* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount*sizeof(XD_INT4), bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_FLOAT4* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount*sizeof(XD_INT4), bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_INT8* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount*sizeof(XD_FLOAT8), bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_UINT8* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount*sizeof(XD_FLOAT8), bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_FLOAT8* pValue, XD_UINT4 nValueCount, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	return innerPutValue((unsigned char*)pValue, nValueCount*sizeof(XD_FLOAT8), bHostOrder);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const bool value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const char value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_UINT1 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_INT2 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_UINT2 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_INT4 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_UINT4 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_FLOAT4 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_INT8 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_UINT8 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}
bool XRichDataBase::putValue(const XD_FLOAT8 value)
{
XEXCEPT_ENTRY
	return putValue(&value, 1);
XEXCEPT_EXIT
}

bool XRichDataBase::setValue(XD_UINT4 index, const bool value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((bool*)m_pData)[index] = value;
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_INT1 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_INT1*)m_pData)[index] = value;
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_UINT1 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_UINT1*)m_pData)[index] = value;
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_INT2 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_INT2*)m_pData)[index] = value;	
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_UINT2 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_UINT2*)m_pData)[index] = value;		
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_INT4 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_INT4*)m_pData)[index] = value;
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_UINT4 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_UINT4*)m_pData)[index] = value;
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_FLOAT4 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_FLOAT4*)m_pData)[index] = value;
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_INT8 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_INT8*)m_pData)[index] = value;
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_UINT8 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_UINT8*)m_pData)[index] = value;
XEXCEPT_EXIT
	return true;
}
bool XRichDataBase::setValue(XD_UINT4 index, const XD_FLOAT8 value) 
{
XEXCEPT_ENTRY
	if( getDataCount() <= index ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	((XD_FLOAT8*)m_pData)[index] = value;	
XEXCEPT_EXIT
	return true;
}





void XRichDataBase::getValueBool(bool **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (bool*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(std::ostream *stream) const
{
XEXCEPT_ENTRY
	XD_UINT4 nValueCount = getDataCount();
	XD_UINT4 i;

	switch( m_type ) {
	case	XDATA_TYPE_B:
	case	XDATA_TYPE_BOOL:
	case	XDATA_TYPE_A:
	case	XDATA_TYPE_J:
	case	XDATA_TYPE_U1:
	case	XDATA_TYPE_I1:
		stream->write((const char*)m_pData, m_nDataSize);
		break;
	case XDATA_TYPE_U2:
	case XDATA_TYPE_I2:
		for(i = 0; i < nValueCount; i++ ){ 
			XD_UINT2 val = getValueUInt2( i );
			stream->write((const char*)&val, sizeof(val));
		}
		break;
	case XDATA_TYPE_U4:
	case XDATA_TYPE_I4:
	case XDATA_TYPE_F4:
		for(i = 0; i < nValueCount; i++ ){ 
			XD_UINT4 val = getValueUInt4( i );
			stream->write((const char*)&val, sizeof(val));
		}
		break;
	case XDATA_TYPE_U8:
	case XDATA_TYPE_I8:
	case XDATA_TYPE_F8:
		for(i = 0; i < nValueCount; i++ ){ 
			XD_UINT8 val = getValueUInt8( i );
			stream->write((const char*)&val, sizeof(val));
		}
		break;
	}
XEXCEPT_EXIT
}
void XRichDataBase::getValue(char **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (char*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_UINT1 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (unsigned char*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_INT2 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (XD_INT2*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_UINT2 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (XD_UINT2*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_INT4 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (XD_INT4*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_UINT4 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (XD_UINT4*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_FLOAT4 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (XD_FLOAT4*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_INT8 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (XD_INT8*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_UINT8 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (XD_UINT8*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}
void XRichDataBase::getValue(XD_FLOAT8 **pValue, XD_UINT4 &nValueCount) const
{
XEXCEPT_ENTRY
	*pValue = (XD_FLOAT8*)m_pData;
	nValueCount = getDataCount();
XEXCEPT_EXIT
}

const char* XRichDataBase::getValueString(int *err) const
{
XEXCEPT_ENTRY
	if( m_nDataSize == 0 ) {
		return "";
	}
	return (const char*)m_pData;
XEXCEPT_EXIT
}
const unsigned char XRichDataBase::getValueBinary(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	unsigned char *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}

const bool XRichDataBase::getValueBoolean(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	bool *value;
	XD_UINT4 nItemCout;
	
	getValueBool(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	return value[index];
XEXCEPT_EXIT
}
const char XRichDataBase::getValueInt1(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	char *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}

const XD_UINT1 XRichDataBase::getValueUInt1(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_UINT1 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}


const XD_INT2 XRichDataBase::getValueInt2(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_INT2 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}

const XD_UINT2 XRichDataBase::getValueUInt2(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_UINT2 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}

const XD_INT4 XRichDataBase::getValueInt4(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_INT4 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}

const XD_UINT4 XRichDataBase::getValueUInt4(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_UINT4 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}


const XD_FLOAT4 XRichDataBase::getValueFloat4(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_FLOAT4 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}




const XD_INT8 XRichDataBase::getValueInt8(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_INT8 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}

const XD_UINT8 XRichDataBase::getValueUInt8(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_UINT8 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}


const XD_FLOAT8 XRichDataBase::getValueFloat8(XD_UINT4 index /*= 0*/, int *err /*= NULL*/) const
{
XEXCEPT_ENTRY
	XD_FLOAT8 *value;
	XD_UINT4 nItemCout;
	
	getValue(&value, nItemCout);
	if( nItemCout == 0 ) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	if( nItemCout <= index) {
		if( err ) {	*err = XD2_ERRAS;}
//		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	return value[index];
XEXCEPT_EXIT
}

bool XRichDataBase::changeType(XDATA_TYPE type) 
{
XEXCEPT_ENTRY
	int err = 0;
	XDATA_TYPE	type_old = m_type;
	if( type_old == type ) {
		// 同じデータタイプ
		return true;
	}

	// 元データを保存
	XRichData	rData(type);
	XD_UINT4	count = this->getDataCount();
	if( rData.assign( this->getDataCount() ) == false ) {
		return false;
	}
	

#define changeType_COMMON_ALG(x)				switch( type ) {							\
	case XDATA_TYPE_BOOL:																	\
		if( rData.setValue(i, (const bool)(x!=0)) == false ) {return false;}		break;	\
	case XDATA_TYPE_U1:			case XDATA_TYPE_A:			case XDATA_TYPE_J:				\
		if( rData.setValue(i, (const XD_UINT1)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_I1:	case XDATA_TYPE_B:													\
		if( rData.setValue(i, (const XD_INT1)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_U2:			case XDATA_TYPE_UTF16:										\
		if( rData.setValue(i, (const XD_UINT2)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_I2:																		\
		if( rData.setValue(i, (const XD_INT2)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_U4:																		\
		if( rData.setValue(i, (const XD_UINT4)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_I4:																		\
		if( rData.setValue(i, (const XD_INT4)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_F4:																		\
		if( rData.setValue(i, (const XD_FLOAT4)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_U8:																		\
		if( rData.setValue(i, (const XD_UINT8)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_I8:																		\
		if( rData.setValue(i, (const XD_INT8)x ) == false ) {return false;}		break;	\
	case XDATA_TYPE_F8:																		\
		if( rData.setValue(i, (const XD_FLOAT8)x ) == false ) {return false;}		break;	\
	default:																				\
		m_error = XD2_ERRTY;																\
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}\
		return false;																		\
	}

	for(XD_UINT4 i = 0; i < count; i++) {

		switch( type_old ) {
		
		case XDATA_TYPE_BOOL:
			{
				bool val = this->getValueBoolean(i, &err);
				switch( type ) {
				case XDATA_TYPE_BOOL:
					if( rData.setValue(i, (const bool)val) == false ) {return false;}
					break;
				case XDATA_TYPE_U1:			case XDATA_TYPE_A:			case XDATA_TYPE_J:
					if( rData.setValue(i, (const XD_UINT1)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_I1:	case XDATA_TYPE_B:
					if( rData.setValue(i, (const XD_INT1)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_U2:			case XDATA_TYPE_UTF16:
					if( rData.setValue(i, (const XD_UINT2)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_I2:
					if( rData.setValue(i, (const XD_INT2)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_U4:
					if( rData.setValue(i, (const XD_UINT4)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_I4:
					if( rData.setValue(i, (const XD_INT4)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_F4:
					if( rData.setValue(i, (const XD_FLOAT4)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_U8:
					if( rData.setValue(i, (const XD_UINT8)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_I8:
					if( rData.setValue(i, (const XD_INT8)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_F8:
					if( rData.setValue(i, (const XD_FLOAT8)val ) == false ) {return false;}
					break;
				default:
					m_error = XD2_ERRTY;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
					return false;
				}
			}
			break;
		case XDATA_TYPE_U1:			case XDATA_TYPE_A:			case XDATA_TYPE_J:
			{
				changeType_COMMON_ALG( this->getValueUInt1(i, &err) )
			}
			break;
		case XDATA_TYPE_I1:	case XDATA_TYPE_B:
			{
				changeType_COMMON_ALG( this->getValueInt1(i, &err) );
			}
			break;
		case XDATA_TYPE_U2:			case XDATA_TYPE_UTF16:
			{
				changeType_COMMON_ALG( this->getValueUInt2(i, &err) );
			}
			break;
		case XDATA_TYPE_I2:
			{
				changeType_COMMON_ALG( this->getValueInt2(i, &err) );
			}
			break;
		case XDATA_TYPE_U4:
			{
				changeType_COMMON_ALG( this->getValueUInt4(i, &err) );
			}
			break;
		case XDATA_TYPE_I4:
			{
				changeType_COMMON_ALG( this->getValueInt4(i, &err) );
			}
			break;
		case XDATA_TYPE_F4:
			{
				changeType_COMMON_ALG( this->getValueFloat4(i, &err) );
			}
			break;
		case XDATA_TYPE_U8:
			{
				XD_UINT8 val = this->getValueUInt8(i, &err);
				switch( type ) {
				case XDATA_TYPE_BOOL:
					if( rData.setValue(i, (val!=0)) == false ) {return false;}
					break;
				case XDATA_TYPE_U1:			case XDATA_TYPE_A:			case XDATA_TYPE_J:
					if( rData.setValue(i, (const XD_UINT1)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_I1:	case XDATA_TYPE_B:
					if( rData.setValue(i, (const XD_INT1)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_U2:			case XDATA_TYPE_UTF16:
					if( rData.setValue(i, (const XD_UINT2)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_I2:
					if( rData.setValue(i, (const XD_INT2)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_U4:
					if( rData.setValue(i, (const XD_UINT4)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_I4:
					if( rData.setValue(i, (const XD_INT4)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_F4:
					if( rData.setValue(i, (const XD_FLOAT4)(XD_INT8)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_U8:
					if( rData.setValue(i, (const XD_UINT8)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_I8:
					if( rData.setValue(i, (const XD_INT8)val ) == false ) {return false;}
					break;
				case XDATA_TYPE_F8:
					if( rData.setValue(i, (const XD_FLOAT8)(XD_INT8)val ) == false ) {return false;}
					break;
				default:
					m_error = XD2_ERRTY;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
					return false;
				}
			}
			break;
		case XDATA_TYPE_I8:
			{
				changeType_COMMON_ALG( this->getValueInt8(i, &err) );
			}
			break;
		case XDATA_TYPE_F8:
			{
				changeType_COMMON_ALG( this->getValueFloat8(i, &err) );
			}
			break;
		default:
			m_error = XD2_ERRTY;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
			return false;
		}
		if( err != 0 ) {
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, err);	}
			return false;
		}
	}


	m_type = type;
	return innerPutValue(rData.m_pData, rData.m_nDataSize);
#undef changeType_COMMON_ALG
XEXCEPT_EXIT
	return true;
}




bool XRichDataBase::innerPutValue(const unsigned char* pValue, XD_UINT4 nValueSize, bool bHostOrder /*= true*/)
{
XEXCEPT_ENTRY
	freeValue();
	if( nValueSize == 0 ) {
		return true;
	}
	if( m_type == XDATA_TYPE_A || m_type == XDATA_TYPE_J ) {
		m_pData = new unsigned char[nValueSize+sizeof(char)];
	} 
	else if( m_type == XDATA_TYPE_UTF16 ) {
		m_pData = new unsigned char[nValueSize+sizeof(wchar_t)];
	} else {
		m_pData = new unsigned char[nValueSize];
	}
	if( m_pData == NULL ) {
		m_error = XD2_ERRMA;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
		return false;
	}
	if( m_type == XDATA_TYPE_A ) {
		::memset(m_pData, 0, nValueSize+sizeof(char));
	} 
	else if( m_type == XDATA_TYPE_UTF16 ) {
		::memset(m_pData, 0, nValueSize+sizeof(wchar_t));
	}
	::memcpy(m_pData, pValue, nValueSize);
	m_nDataSize = nValueSize;

	if( bHostOrder == false ) {
		// ネットワークバイトオーダーの場合は、ホストオーダーに変更する。
		switch( m_type ) {
		case XDATA_TYPE_UTF16:
		case XDATA_TYPE_U2:
		case XDATA_TYPE_I2:
			{
				XD_UINT2 *pU2Val   = (XD_UINT2*)m_pData;
				XD_UINT4 ulLoopCnt = this->getDataCount();
				for( XD_UINT4 i = 0; i < ulLoopCnt; i++) {
					*pU2Val = XDataCoder::_ntoh2(*pU2Val);
					pU2Val++;
				}
			}
			break;
		case XDATA_TYPE_U4:
		case XDATA_TYPE_I4:
		case XDATA_TYPE_F4:
			{
				XD_UINT4 *pU4Val   = (XD_UINT4*)m_pData;
				XD_UINT4 ulLoopCnt = this->getDataCount();
				for( XD_UINT4 i = 0; i < ulLoopCnt; i++) {
					*pU4Val = XDataCoder::_ntoh4(*pU4Val);
					pU4Val++;
				}
			}
			break;
		case XDATA_TYPE_U8:
		case XDATA_TYPE_I8:
		case XDATA_TYPE_F8:
			{
				XD_UINT8 *pU8Val   = (XD_UINT8*)m_pData;
				XD_UINT4 ulLoopCnt = this->getDataCount();
				for( XD_UINT4 i = 0; i < ulLoopCnt; i++) {
					*pU8Val = XDataCoder::_ntoh8(*pU8Val);
					pU8Val++;
				}
			}
			break;
		default:
			break;
		}
	}
	m_bDirty = true;
	return true;
XEXCEPT_EXIT
}








XRichData::XRichData(XDATA_TYPE type)
	:	XRichDataBase(NULL)
{
XEXCEPT_ENTRY

	m_type = type;
XEXCEPT_EXIT
}
XRichData::XRichData(XRichData &srcdata)
	:	XRichDataBase(NULL)

{
XEXCEPT_ENTRY
	*this = srcdata;
XEXCEPT_EXIT
}

XRichData::XRichData(XRichData* pParentt)
	:	XRichDataBase(pParentt)
{
}
XRichData::~XRichData()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XRichData* XRichData::addChiled(XDATA_TYPE type, XRichData* pPrePos/* = NULL*/)
{
XEXCEPT_ENTRY
	if( m_type != XDATA_TYPE_L ) {
		return NULL;
	}
	std::vector<XRichDataBase*>::iterator ite;
	if( pPrePos != NULL ) {
		ite = m_children.begin();
		for( ; ite != m_children.end(); ite++ ) {
			XRichData *p = (XRichData*)(*ite);
			if( p == pPrePos ) {
				break;
			}
		}
		if( ite == m_children.end() ) {
			m_error = XD2_ERRIT;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
			return NULL;
		}
	}
/*
	XPOSITION pos = m_children.GetHeadPosition();
	if( pPrePos != NULL ) {
		for( ; pos != NULL;) {
			if( m_children.GetNext(pos) == pPrePos )  {
				break;
			}
		}
		if( pos == NULL ) {
			return NULL;
		}
	}
*/
	XRichData *pNew = new XRichData(this);
	if( pNew == NULL ) {
		m_error = XD2_ERRMA;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
		return NULL;
	}
	pNew->m_type = type;
	
	if( pPrePos != NULL ) {
		//m_children.InsertBefore(pos, pNew);
		m_children.insert(ite, pNew);
	} else {
		//m_children.AddTail( pNew );
		m_children.push_back(pNew);
	}
	m_bDirty = true;
	return pNew;
XEXCEPT_EXIT
}

XRichData* XRichData::getChiled(XD_UINT4 index)
{
XEXCEPT_ENTRY
	XD_UINT4 i = 0;
	std::vector<XRichDataBase*>::iterator ite;
	ite = m_children.begin();
	for( ; ite != m_children.end(); ite++ ) {
		if( i== index ) {
			return (XRichData*)(*ite);
		}
		i++;
	}
	m_error = XD2_ERRIT;
	if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
	return NULL;
//	XPOSITION pos = m_children.FindIndex(index);
//	if( pos == NULL ) {
//		return NULL;
//	}
//	return (XRichData*)m_children.GetAt( pos );
XEXCEPT_EXIT
}


bool XRichData::buildBinaryData(std::ostream *stream)
{
XEXCEPT_ENTRY
	XDataWriter writer(stream);
	return buildBinaryData(writer);
XEXCEPT_EXIT
}
bool XRichData::copy(XRichData* pSrcData)
{
XEXCEPT_ENTRY
	std::vector<XRichDataBase*>::iterator ite;
	ite = m_children.begin();
	for( ; ite != m_children.end(); ite++ ) {
		XRichData *p = (XRichData*)(*ite);
		delete p;
	}
	m_children.clear();
	this->freeValue();

	m_type = pSrcData->m_type;

	if( m_type == XDATA_TYPE_L ) {
		if( pSrcData->clone(this) == false ) {
			m_error = pSrcData->getError();
			return false;
		}
	} else if( m_type == XDATA_TYPE_A || m_type == XDATA_TYPE_J ) {
		if( putValueString((const char*)pSrcData->m_pData) == false ) {
			return false;
		}
	} else {
		if( innerPutValue(pSrcData->m_pData, pSrcData->m_nDataSize) == false ) {
			return false;
		}
	}
	m_bDirty = true;
	return true;
XEXCEPT_EXIT
}
XRichData* XRichData::clone()
{
XEXCEPT_ENTRY
	XRichData *pNew = new XRichData(m_type);
	if( pNew == NULL ) { 
		m_error = XD2_ERRMA;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
		return NULL;
	}
	pNew->m_nLengthByte = this->m_nLengthByte;
	if( m_type == XDATA_TYPE_L ) {
		if( clone(pNew) == false ) {
			delete pNew;
			pNew = NULL;
		}
	} else if( m_type == XDATA_TYPE_A || m_type == XDATA_TYPE_J ) {
		if( pNew->putValueString((const char*)m_pData) == false ) {
			m_error = pNew->getError();
			return false;
		}
	} else {
		if( pNew->innerPutValue(m_pData, m_nDataSize) == false ) {
			m_error = pNew->getError();
			return false;
		}
	}
	return pNew;
XEXCEPT_EXIT
}

XRichData* XRichData::find(const TCHAR* lpszFindpath, ...)
{
XEXCEPT_ENTRY
	XRichData* pRetVal = NULL;
	va_list marker;
	va_start( marker, lpszFindpath );
	
	pRetVal = find(lpszFindpath, marker);

	va_end( marker );
	return pRetVal;
XEXCEPT_EXIT
}



XRichData* XRichData::find(const TCHAR* lpszFindpath, va_list argList)
{
XEXCEPT_ENTRY
	XRichData* pRetVal = NULL;
	XD_UINT4 pos = 0;
	if( *lpszFindpath == 0 ) {
		return NULL;
	}
	pos = (XD_UINT4)va_arg( argList, int);
	if( getChiledCount() < pos ) {
		return NULL;
	}
	pRetVal = getChiled(pos);
	if( pRetVal == NULL ) {
		return NULL;
	}
	lpszFindpath++;
	if( *lpszFindpath == 0 ) {
		return pRetVal;
	}
	return pRetVal->find(lpszFindpath, argList);
XEXCEPT_EXIT
}

bool XRichData::clone(XRichData *pTarget)
{
XEXCEPT_ENTRY
	XD_UINT4 i;

	assert(pTarget->m_type == XDATA_TYPE_L);

	for(i = 0; i < getChiledCount(); i++) {
		XRichData *pSrc = getChiled(i);
		assert( pSrc );
		XRichData* pDst = pTarget->addChiled(pSrc->m_type);
		pDst->m_nLengthByte = pSrc->m_nLengthByte;
		if( pSrc->m_type == XDATA_TYPE_L ) {
			if( pSrc->clone(pDst) == false ) {
				m_error = pSrc->getError();
				return false;
			}
		}  else if( pSrc->m_type == XDATA_TYPE_A || pSrc->m_type == XDATA_TYPE_J ) {
			if( pDst->putValueString((const char*)pSrc->m_pData) == false ) {
				m_error = pDst->getError();
				return false;
			}
		} else {
			pDst->innerPutValue(pSrc->m_pData, pSrc->m_nDataSize);
		}
	}
	return true;
XEXCEPT_EXIT
}
bool XRichData::buildBinaryData(XDataWriter &writer)
{
XEXCEPT_ENTRY
	switch( m_type ) {
	case XDATA_TYPE_L:
		{
			//if( writer.putValAsList(getChiledCount()) == false ) {
			if( writer.putValue(XDATA_TYPE_L, NULL, getChiledCount() ,m_nLengthByte) == false ) {
				return false;
			}
			XD_UINT4 i = 0;
			for( ; i < getChiledCount(); i++) {
				XRichData *pData = getChiled(i);

				if( pData->buildBinaryData(writer) == false ) {
					return false;
				}
			}
		}
		break;
	case XDATA_TYPE_B:
	case XDATA_TYPE_BOOL:
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
	case XDATA_TYPE_UTF16:
	case XDATA_TYPE_U1:
	case XDATA_TYPE_I1:
	case XDATA_TYPE_U2:
	case XDATA_TYPE_I2:
	case XDATA_TYPE_U4:
	case XDATA_TYPE_I4:
	case XDATA_TYPE_F4:
	case XDATA_TYPE_U8:
	case XDATA_TYPE_I8:
	case XDATA_TYPE_F8:
		if( writer.putValue(m_type, m_pData, getDataCount() ,m_nLengthByte ) == false ) {
			return false;
		}
		break;
	default:
		m_error = XD2_ERRTY;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
		break;
	}
	m_bDirty = true;
	return true;
XEXCEPT_EXIT
}
bool XRichData::putValueFromXData(XDataReader &reader, XData& value, bool bConvert /*= false*/)
{
XEXCEPT_ENTRY
	// データタイプのチェック
	if( m_type != value.getType() ) {
		if( bConvert == false ) {
			m_error = XD2_ERRTY;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
			return false;
		}
	}

	this->clear();
	return innerPutValueFromXData(reader, value, bConvert);
XEXCEPT_EXIT
}
bool XRichData::innerPutValueFromXData(XDataReader &reader, XData& value, bool bConvert)
{
XEXCEPT_ENTRY
	unsigned char *pRawData = NULL;
	XD_UINT4	  ulRawSize = 0;

	m_type = value.getType();
	if( value.isFitLengthByte() == false ) {
		m_nLengthByte = XData::getLengthByte(m_type, value.getCount() );
	} else {
		m_nLengthByte = -1;
	}
	ulRawSize = value.getValAsBinaryRef( &pRawData );
	

	switch( value.getType() ) {
	case XDATA_TYPE_L:
		{
			XD_UINT4 ulCnt = value.getCount();
			for(XD_UINT4 i = 0; i < ulCnt; i++) {
				if( reader.getValue( &value ) == false ) {
					return false;
				}
				XRichData* pChiled = this->addChiled( value.getType() );
				if( pChiled == NULL ) {
					return false;
				}
				if( pChiled->innerPutValueFromXData(reader, value, bConvert ) == false ) {
					return false;
				}
			}
		}
		break;
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
		if( putValueString((const char*)pRawData, ulRawSize) == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_B:
	case XDATA_TYPE_BOOL:
	case XDATA_TYPE_U1:
	case XDATA_TYPE_I1:
	case XDATA_TYPE_UTF16:
	case XDATA_TYPE_U2:
	case XDATA_TYPE_I2:
	case XDATA_TYPE_U4:
	case XDATA_TYPE_I4:
	case XDATA_TYPE_F4:
	case XDATA_TYPE_U8:
	case XDATA_TYPE_I8:
	case XDATA_TYPE_F8:
		return innerPutValue(pRawData, ulRawSize, false);
		break;
	default:
		m_error = XD2_ERRTY;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
		break;
	}
	return true;
XEXCEPT_EXIT
}
bool XRichData::print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XInflateStreamBuff	buff;
	std::ostream		buffstream(&buff);

	if( buildBinaryData(&buffstream) == false ) {
		return false;
	}

	XDataReader reader(buff.getBuff(), buff.size());
	return reader.print(stream, nDepth, pLF);
XEXCEPT_EXIT
}

bool XRichData::operator==(XRichData& pDst)
{
XEXCEPT_ENTRY
	return this->compare(&pDst) == 0 ? true : false;
XEXCEPT_EXIT
}

bool XRichData::operator!=(XRichData& pDst)
{
XEXCEPT_ENTRY
	return this->compare(&pDst) != 0 ? true : false;
XEXCEPT_EXIT
}

const XRichData& XRichData::operator=(XRichData& pSrcData)
{
XEXCEPT_ENTRY
	this->copy(&pSrcData);
	return *this;
XEXCEPT_EXIT
}



//
//	XRichDataXL
//
XRichDataXL::XRichDataXL()
	:	m_strTagName(_T(""))
	,	XRichDataBase(NULL)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XRichDataXL::XRichDataXL(XRichDataXL* pParentt)
	:	m_strTagName(_T(""))
	,	XRichDataBase(pParentt)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XRichDataXL::~XRichDataXL()
{
}


const TCHAR* XRichDataXL::getTagName()
{
XEXCEPT_ENTRY
	return m_strTagName;
XEXCEPT_EXIT
}
void XRichDataXL::setTagName(const TCHAR* pTagName)
{
XEXCEPT_ENTRY
	m_strTagName = pTagName;
XEXCEPT_EXIT
}

XRichDataXL* XRichDataXL::addChiled(XDATA_TYPE type, const TCHAR* pTagName, XRichDataXL* pPrePos/* = NULL*/)
{
XEXCEPT_ENTRY
	if( m_type != XDATA_TYPE_L ) {
		m_error = XD2_ERRTY;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return NULL;
	}
	std::vector<XRichDataBase*>::iterator ite;
	if( pPrePos != NULL ) {
		ite = m_children.begin();
		for( ; ite != m_children.end(); ite++ ) {
			XRichDataXL *p = (XRichDataXL*)(*ite);
			if( p == pPrePos ) {
				break;
			}
		}
		if( ite == m_children.end() ) {
			m_error = XD2_ERRIT;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
			return NULL;
		}
	}
/*
	XPOSITION pos = m_children.GetHeadPosition();
	if( pPrePos != NULL ) {
		for( ; pos != NULL;) {
			if( m_children.GetNext(pos) == pPrePos )  {
				break;
			}
		}
		if( pos == NULL ) {
			return NULL;
		}
	}
*/
	XRichDataXL *pNew = new XRichDataXL(this);
	if( pNew == NULL ) {
		m_error = XD2_ERRMA;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
		return NULL;
	}
	pNew->m_type = type;
	if( pTagName ) {
		pNew->setTagName( pTagName );
	}
	
	if( pPrePos != NULL ) {
		//m_children.InsertBefore(pos, pNew);
		m_children.insert(ite, pNew);
	} else {
		//m_children.AddTail( pNew );
		m_children.push_back(pNew);
	}
	m_bDirty = true;
	return pNew;
XEXCEPT_EXIT
}
XRichDataXL* XRichDataXL::getChiled(XD_UINT4 index)
{
XEXCEPT_ENTRY
	XD_UINT4 i = 0;
	std::vector<XRichDataBase*>::iterator ite;
	ite = m_children.begin();
	for( ; ite != m_children.end(); ite++ ) {
		if( i== index ) {
			return (XRichDataXL*)(*ite);
		}
		i++;
	}
	m_error = XD2_ERRIT;
	if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
	return NULL;
//	XPOSITION pos = m_children.FindIndex(index);
//	if( pos == NULL ) {
//		return NULL;
//	}
//	return (XRichDataXL*)m_children.GetAt( pos );
XEXCEPT_EXIT
}
bool XRichDataXL::copy(XRichDataXL* pSrcData)
{
XEXCEPT_ENTRY
	assert(pSrcData);

	std::vector<XRichDataBase*>::iterator ite;
	ite = m_children.begin();
	for( ; ite != m_children.end(); ite++ ) {
		XRichDataXL *p = (XRichDataXL*)(*ite);
		delete p;
	}
	m_children.clear();
	this->freeValue();

	m_type = pSrcData->m_type;
	m_strTagName = pSrcData->m_strTagName;

	if( m_type == XDATA_TYPE_L ) {
		if( pSrcData->clone(this) == false ) {
			m_error = pSrcData->getError();
			return false;
		}
	} else if( m_type == XDATA_TYPE_A || m_type == XDATA_TYPE_J ) {
		if( putValueString((const char*)pSrcData->m_pData) == false ) {
			return false;
		}
	} else {
		if( innerPutValue(pSrcData->m_pData, pSrcData->m_nDataSize) == false ) {
			return false;
		}
	}
	m_bDirty = true;
	return true;
XEXCEPT_EXIT
}
XRichDataXL* XRichDataXL::clone()
{
XEXCEPT_ENTRY
	XRichDataXL *pNew = new XRichDataXL();
	if( pNew == NULL ) { 
		m_error = XD2_ERRMA;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
		return NULL;
	}
	pNew->m_type = m_type;
	pNew->m_strTagName = m_strTagName;
	if( m_type == XDATA_TYPE_L ) {
		if( clone(pNew) == false ) {
			delete pNew;
			pNew = NULL;
		}
	} else if( m_type == XDATA_TYPE_A || m_type == XDATA_TYPE_J ) {
		if( pNew->putValueString((const char*)m_pData) == false ) {
			m_error = pNew->getError();
			return false;
		}
	} else {
		pNew->innerPutValue(m_pData, m_nDataSize);
	}
	return pNew;
XEXCEPT_EXIT
}
bool XRichDataXL::clone(XRichDataXL *pTarget)
{
XEXCEPT_ENTRY
	XD_UINT4 i;

	assert(pTarget->m_type == XDATA_TYPE_L);

	for(i = 0; i < getChiledCount(); i++) {
		XRichDataXL *pSrc = getChiled(i);
		assert( pSrc );
		XRichDataXL* pDst = pTarget->addChiled(pSrc->m_type, pSrc->m_strTagName);
		if( pSrc->m_type == XDATA_TYPE_L ) {
			if( pSrc->clone(pDst) == false ) {
				m_error = pSrc->getError();
				return false;
			}
		}  else if( pSrc->m_type == XDATA_TYPE_A || pSrc->m_type == XDATA_TYPE_J ) {
			if( pDst->putValueString((const char*)pSrc->m_pData) == false ) {
				m_error = pDst->getError();
				return false;
			}
		} else {
			if( pDst->innerPutValue(pSrc->m_pData, pSrc->m_nDataSize) == false ) {
				m_error = pDst->getError();
				return false;
			}
		}
	}
	return true;
XEXCEPT_EXIT
}

bool XRichDataXL::buildBinaryData(std::ostream *stream)
{
XEXCEPT_ENTRY
	XDataXLWriter writer(stream);
	return buildBinaryData(writer);
XEXCEPT_EXIT
}
bool XRichDataXL::buildBinaryData(XDataXLWriter &writer)
{
XEXCEPT_ENTRY	
	assert(0);

	switch( m_type ) {
	case XDATA_TYPE_L:
		{

			XD_UINT4 i = 0;
			for( ; i < getChiledCount(); i++) {
				XRichDataXL *pData = getChiled(i);
			}
		}
		break;
	case XDATA_TYPE_B:
		break;
	case XDATA_TYPE_BOOL:
		break;
	case XDATA_TYPE_A:
		break;
	case XDATA_TYPE_J:
		break;
	case XDATA_TYPE_U1:
		break;
	case XDATA_TYPE_I1:

		break;
	case XDATA_TYPE_U2:
		break;
	case XDATA_TYPE_I2:
		break;
	case XDATA_TYPE_U4:
		break;
	case XDATA_TYPE_I4:
		break;
	case XDATA_TYPE_F4:
		break;
	case XDATA_TYPE_U8:
		break;
	case XDATA_TYPE_I8:
		break;
	case XDATA_TYPE_F8:

		break;
	}
	m_bDirty = true;
	return true;
XEXCEPT_EXIT
}

bool XRichDataXL::print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XInflateStreamBuff	buff;
	std::ostream		buffstream(&buff);

	if( buildBinaryData(&buffstream) == false ) {
		return false;
	}

	XDataXLReader reader(buff.getBuff(), buff.size());
	return reader.print(stream, nDepth, pLF);
XEXCEPT_EXIT
}





