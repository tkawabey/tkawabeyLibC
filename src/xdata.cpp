#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"

bool XData::g_bErrorRise = false;

XData::XData()
	:	m_nSize(0)
	,	m_nSizeOfSize(0)
	,	m_nType(XDATA_TYPE_END)
	,	m_pValue(NULL)
	,	m_error(0)
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

XData::~XData()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XD_UINT4 XData::getCount() const
{
XEXCEPT_ENTRY
	return ( m_nSize / (1 << (m_nType & 0x03))  );
XEXCEPT_EXIT
}
XD_UINT4 XData::getSize() const
{
XEXCEPT_ENTRY
	return m_nSize;
XEXCEPT_EXIT
}
XD_UINT1 XData::getLengthByte() const
{
XEXCEPT_ENTRY
	return m_nSizeOfSize;
XEXCEPT_EXIT
}
XD_UINT4 XData::getCountByByteSize(XDATA_TYPE type, XD_UINT4 byteSize)
{
XEXCEPT_ENTRY
	XD_UINT4 sc = (XD_UINT4)(type & 0x03);
	if( sc == 0 ) {
		return sc;
	}
	return byteSize / sc;
XEXCEPT_EXIT
	return 0;
}
bool XData::isFitLengthByte() const
{
XEXCEPT_ENTRY
	return m_nSizeOfSize == XData::getLengthByte(m_nType, this->getCount());
XEXCEPT_EXIT
}
XDATA_TYPE XData::getType() const
{
XEXCEPT_ENTRY
	return m_nType;
XEXCEPT_EXIT
}
int XData::getError()
{
XEXCEPT_ENTRY
	return m_error;
XEXCEPT_EXIT
}
void XData::resetError()
{
XEXCEPT_ENTRY
	m_error = 0;
XEXCEPT_EXIT
}
bool XData::getValue(std::ostream *stream)
{
XEXCEPT_ENTRY
	
	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	XD_UINT4 count = this->getCount();
	XD_UINT4 i = 0;

	if( m_nType == XDATA_TYPE_B || 
		m_nType == XDATA_TYPE_BOOL ||
		m_nType == XDATA_TYPE_A ||
		m_nType == XDATA_TYPE_J ||
		m_nType == XDATA_TYPE_U1 ||
		m_nType == XDATA_TYPE_U2 ) {
		XBIN* pValue = NULL;
		size_t nSize = this->getValAsBinaryRef( &pValue );
		stream->write( (char*)pValue, nSize);
		return true;
	} else 
	if( m_nType == XD2_UTF16 ) {
		XStrBuff str(_T(""));
		getValAsStringUTF16(str);
		stream->write( (char*)(const wchar_t*)str, str.getLength() * sizeof(wchar_t));
		return true;
	}

	for( i = 0; i < count; i++ ) {
		switch( m_nType ) {
		case XDATA_TYPE_I2: 
			{
				XD_INT2 val = this->getValAsInt2( i );
				stream->write( (char*)&val, sizeof(val));
			}
			break;
		case XDATA_TYPE_U2: 
			{
				XD_UINT2 val = this->getValAsUInt2( i );
				stream->write( (char*)&val, sizeof(val));
			}
			break;
		case XDATA_TYPE_I4: 
			{
				XD_INT4 val = this->getValAsInt4( i );
				stream->write( (char*)&val, sizeof(val));
			}
			break;
		case XDATA_TYPE_U4: 
			{
				XD_UINT4 val = this->getValAsUInt4( i );
				stream->write( (char*)&val, sizeof(val));
			}
			break;
		case XDATA_TYPE_I8: 
			{
				XD_INT8 val = this->getValAsInt8( i );
				stream->write( (char*)&val, sizeof(val));
			}
			break;
		case XDATA_TYPE_U8: 
			{
				XD_UINT8 val = this->getValAsUInt8( i );
				stream->write( (char*)&val, sizeof(val));
			}
			break;
		case XDATA_TYPE_F4: 
			{
				XD_FLOAT4 val = this->getValAsFloat4( i );
				stream->write( (char*)&val, sizeof(val));
			}
			break;
		case XDATA_TYPE_F8: 
			{
				XD_FLOAT8 val = this->getValAsFloat8( i );
				stream->write( (char*)&val, sizeof(val));
			}
			break;
		default:
			m_error = XD2_ERRCV;
			if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_nType);	}
			return false;
			break;
		}

	}
	return true;
XEXCEPT_EXIT
	return false;
}
size_t XData::getValAsBinaryRef(XBIN **ppValue)
{
XEXCEPT_ENTRY
	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return 0;
	}
	*ppValue = m_pValue;
	return m_nSize;
XEXCEPT_EXIT
}
void XData::getValAsBinary(XBIN *pValue, size_t nValueSize)
{
XEXCEPT_ENTRY
	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return;
	}
	if( m_nSize > nValueSize ) {
		m_error = XD2_ERRSS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ;
	}
	::memcpy(pValue, m_pValue, m_nSize);
XEXCEPT_EXIT
}
bool XData::getValAsBool(XD_UINT4 nIndex /*= 0*/)
{	
XEXCEPT_ENTRY
	bool ret = false;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	bool *pWork = (bool*)m_pValue;
	ret = pWork[nIndex];
	return ret;
XEXCEPT_EXIT
}
void XData::getValAsStringA(XStrBuff& str)
{
XEXCEPT_ENTRY
	// 文字列は変換禁止
	if( !(m_nType == XDATA_TYPE_A || m_nType == XDATA_TYPE_J) ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ;
	}
	str.MemSet(0);
	if( m_nSize != 0 ) {
		char* p = (char*)malloc(m_nSize+1);
		::memset(p, 0, m_nSize+1);
		::memcpy(p, m_pValue, m_nSize);
		str.set((const char*)p, m_nSize);
		free(p);
	}
/*
	if( str.GetBuffSize() < ( (m_nSize+1) * sizeof(TCHAR) ) ) {
		// バッファが満たない場合に確保する。
		if( str.Alloc(m_nSize+1) == NULL ) {
			m_error = XD2_ERRMA;
			if( XData::g_bErrorRise == true ) {
				XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
			}
			return ;
		}
	}
	str.MemSet(0);
	::memcpy(str, m_pValue, m_nSize);
*/
XEXCEPT_EXIT
}

void XData::getValAsStringJ(XStrBuff& str)
{
XEXCEPT_ENTRY
	getValAsStringA( str );
XEXCEPT_EXIT
}

void XData::getValAsStringUTF16(XStrBuff& str)
{
XEXCEPT_ENTRY
	XBuff buff;
	wchar_t *pBuff = NULL;
	size_t ulBuffSize = (m_nSize+1) * sizeof(wchar_t);
	
	// 文字列は変換禁止
	if( m_nType != XDATA_TYPE_UTF16 ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ;
	}
	str.MemSet(0);
	if( m_nSize != 0 ) {
		str.set((const wchar_t*)m_pValue, m_nSize/sizeof(wchar_t));
	}
/*
	if( ulBuffSize > buff.GetBuffSize() ) {
		pBuff = (wchar_t *)buff.Alloc( ulBuffSize );
		if( pBuff == NULL ) {
			m_error = XD2_ERRMA;
			if( XData::g_bErrorRise == true ) {
				XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
			}
			return ;
		}
	} else {
		pBuff = (wchar_t *)buff;		
	}
	buff.MemSet(0);
	
	for(XD_UINT4 i = 0; i < m_nSize/2; i++) {
		wchar_t c = this->getValAsUInt2(i);
		pBuff[i] = c;
	}
	str = (wchar_t*)pBuff;
*/
XEXCEPT_EXIT
}

XStrBuff XData::getValAsStringA()
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	getValAsStringA( str );
	return str;
XEXCEPT_EXIT
}
XStrBuff XData::getValAsStringJ()
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	getValAsStringA( str );
	return str;
XEXCEPT_EXIT
}
XStrBuff XData::getValAsStringUTF16()
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	getValAsStringUTF16( str );
	return str;
XEXCEPT_EXIT
}
XD_INT1 XData::getValAsInt1(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_INT1 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_INT1 *pWork = (XD_INT1*)m_pValue;
	ret = pWork[nIndex];
	return ret;
XEXCEPT_EXIT
}
XD_INT2 XData::getValAsInt2(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_INT2 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_INT2 *pWork = (XD_INT2*)m_pValue;
	ret = XDataCoder::_ntoh2(pWork[nIndex]);
	return ret;
XEXCEPT_EXIT
}
XD_INT4 XData::getValAsInt4(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_INT4 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_INT4 *pWork = (XD_INT4*)m_pValue;
	ret = XDataCoder::_ntoh4(pWork[nIndex]);
	return ret;
XEXCEPT_EXIT
}
XD_UINT8 XData::getValAsInt8(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT8 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_UINT8 *pWork = (XD_UINT8*)m_pValue;
	ret = XDataCoder::_ntoh8(pWork[nIndex]);
	return ret;
XEXCEPT_EXIT
}

XD_UINT1 XData::getValAsUInt1(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT1 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_UINT1 *pWork = (XD_UINT1*)m_pValue;
	ret = pWork[nIndex];
	return ret;
XEXCEPT_EXIT
}
XD_UINT2 XData::getValAsUInt2(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT2 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_UINT2 *pWork = (XD_UINT2*)m_pValue;
	ret = XDataCoder::_ntoh2(pWork[nIndex]);
	return ret;
XEXCEPT_EXIT
}
XD_UINT4 XData::getValAsUInt4(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT4 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_UINT4 *pWork = (XD_UINT4*)m_pValue;
	ret = XDataCoder::_ntoh4(pWork[nIndex]);
	return ret;
XEXCEPT_EXIT
}
XD_UINT8 XData::getValAsUInt8(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT8 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_UINT8 *pWork = (XD_UINT8*)m_pValue;
	ret = XDataCoder::_ntoh8(pWork[nIndex]);
	return ret;
XEXCEPT_EXIT
}
XD_FLOAT4 XData::getValAsFloat4(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_FLOAT4 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_INT4 *pWork = (XD_INT4*)m_pValue;
	XD_INT4 lTmp = XDataCoder::_ntoh4(pWork[nIndex]);
	::memcpy(&ret, &lTmp, 4);
	return ret;
XEXCEPT_EXIT
}
XD_FLOAT8 XData::getValAsFloat8(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_FLOAT8 ret = 0;

	if( m_nType == XDATA_TYPE_L ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	XD_UINT8 *pWork = (XD_UINT8*)m_pValue;
	XD_UINT8 lTmp = XDataCoder::_ntoh8(pWork[nIndex]);
	::memcpy(&ret, &lTmp, 8);
	return ret;
XEXCEPT_EXIT
}

void XData::getCastValAsStringA(XStrBuff& str)
{
XEXCEPT_ENTRY
	// 文字列は変換禁止
	if( !(m_nType == XDATA_TYPE_A || m_nType == XDATA_TYPE_B) ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ;
	}
	getValAsStringA( str );
XEXCEPT_EXIT
}

void XData::getCastValAsStringJ(XStrBuff& str)
{
XEXCEPT_ENTRY
	// 文字列は変換禁止
	if( !(m_nType == XDATA_TYPE_A || m_nType == XDATA_TYPE_B || m_nType == XDATA_TYPE_J) ) {
		m_error = XD2_ERRCV;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ;
	}
	str.MemSet(0);
	if( m_nSize != 0 ) {
		str.set((const char*)m_pValue, m_nSize);
	}
	getValAsStringA( str );
XEXCEPT_EXIT
}

XStrBuff XData::getCastValAsStringA()
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	getCastValAsStringA( str );
	return str;
XEXCEPT_EXIT
}
XStrBuff XData::getCastValAsStringJ()
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	getCastValAsStringJ( str );
	return str;
XEXCEPT_EXIT
}

XD_INT1 XData::getCastValAsInt1(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_INT1 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

#define CAST_SET(T, DT, TYPE) case TYPE:			\
		{											\
			DT *pWork = (DT*)m_pValue;				\
			ret = (T)pWork[nIndex];					\
		}											\
		break;
#define CAST_SET_N2F(T, DT, TYPE, N2F) case TYPE:	\
		{											\
			DT *pWork = (DT*)m_pValue;				\
			ret = N2F((T)pWork[nIndex]);			\
		}											\
		break;

#define CAST_SET_ALL(T)	CAST_SET(T, XD_INT1, XDATA_TYPE_I1)							\
	CAST_SET_N2F(T, XD_INT2, XDATA_TYPE_I2, XDataCoder::_ntoh2)						\
	CAST_SET_N2F(T, XD_INT4, XDATA_TYPE_I4, XDataCoder::_ntoh4)						\
	CAST_SET_N2F(T, XD_INT8, XDATA_TYPE_I8, XDataCoder::_ntoh8)						\
	CAST_SET(T, XD_UINT1, XDATA_TYPE_U1)											\
	CAST_SET_N2F(T, XD_UINT2, XDATA_TYPE_U2, XDataCoder::_ntoh2)					\
	CAST_SET_N2F(T, XD_UINT4, XDATA_TYPE_U4, XDataCoder::_ntoh4)					\
	CAST_SET_N2F(T, XD_UINT8, XDATA_TYPE_U8, XDataCoder::_ntoh8)					\
	CAST_SET(T, XBIN, XDATA_TYPE_B)

	switch( m_nType ) {
	CAST_SET_ALL(XD_INT1)
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_INT2   XData::getCastValAsInt2(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_INT2 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	CAST_SET_ALL(XD_INT2)
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_INT4 XData::getCastValAsInt4(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_INT4 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	CAST_SET_ALL(XD_INT4)
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_UINT8 XData::getCastValAsInt8(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT8 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	CAST_SET_ALL(XD_UINT8)
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_UINT1 XData::getCastValAsUInt1(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT1 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	CAST_SET_ALL(XD_UINT1)
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_UINT2 XData::getCastValAsUInt2(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT2 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	CAST_SET_ALL(XD_UINT2)
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_UINT4 XData::getCastValAsUInt4(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT4 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	CAST_SET_ALL(XD_UINT4)
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_UINT8 XData::getCastValAsUInt8(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_UINT8 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	CAST_SET_ALL(XD_UINT8)
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_FLOAT4 XData::getCastValAsFloat4(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_FLOAT4 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	case XDATA_TYPE_F4:
		{
			XD_FLOAT4* pWork = (XD_FLOAT4*)m_pValue;
			ret = (XD_FLOAT4)pWork[nIndex];
		}
		break;
	case XDATA_TYPE_F8:
		{
			XD_FLOAT8* pWork = (XD_FLOAT8*)m_pValue;
			ret = (XD_FLOAT4)pWork[nIndex];
		}
		break;
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

XD_FLOAT8 XData::getCastValAsFloat8(XD_UINT4 nIndex /*= 0*/)
{
XEXCEPT_ENTRY
	XD_FLOAT8 ret = 0;

	if( getCount() <= nIndex ) {
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}

	switch( m_nType ) {
	case XDATA_TYPE_F4:
		{
			XD_FLOAT4* pWork = (XD_FLOAT4*)m_pValue;
			ret = (XD_FLOAT8)pWork[nIndex];
		}
		break;
	case XDATA_TYPE_F8:
		{
			XD_FLOAT8* pWork = (XD_FLOAT8*)m_pValue;
			ret = (XD_FLOAT8)pWork[nIndex];
		}
		break;
	default:
		m_error = XD2_ERRAS;
		if( XData::g_bErrorRise == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return ret;
	}
	return ret;
XEXCEPT_EXIT
}

void XData::setErrorRise(bool val)
{
XEXCEPT_ENTRY
	g_bErrorRise = val;
XEXCEPT_EXIT
}
bool XData::getErrorRise()
{
XEXCEPT_ENTRY
	return g_bErrorRise;
XEXCEPT_EXIT
}
XDATA_TYPE XData::parseDataType_From_String(const XStrBuff& str)
{
XEXCEPT_ENTRY
	if( str == _T("L") || str.CompareNoCase(_T("list")) == 0 ) {
		return XDATA_TYPE_L;
	} else
	if( str == _T("B") || str.CompareNoCase(_T("binary")) == 0 ) {
		return XDATA_TYPE_B;
	} else
	if( str.CompareNoCase(_T("bool")) == 0 ||
		str.CompareNoCase(_T("boolean")) == 0) {
		return XDATA_TYPE_BOOL;
	} else
	if( str == _T("A") || str.CompareNoCase(_T("ascii")) == 0 ) {
		return XDATA_TYPE_A;
	} else
	if( str == _T("UTF8") || str.CompareNoCase(_T("UTF8")) == 0 ) {
		return XDATA_TYPE_UTF16;
	} else
	if( str == _T("J") || str.CompareNoCase(_T("jis")) == 0 ) {
		return XDATA_TYPE_J;
	} else
	if( str.CompareNoCase(_T("u1")) == 0 || str.CompareNoCase(_T("unsigned int 1")) == 0 ) {
		return XDATA_TYPE_U1;
	} else
	if( str.CompareNoCase(_T("u2")) == 0 || str.CompareNoCase(_T("unsigned int 2")) == 0 ) {
		return XDATA_TYPE_U2;
	} else
	if( str.CompareNoCase(_T("u4")) == 0 || str.CompareNoCase(_T("unsigned int 4")) == 0 ) {
		return XDATA_TYPE_U4;
	} else
	if( str.CompareNoCase(_T("u8")) == 0 || str.CompareNoCase(_T("unsigned int 8")) == 0 ) {
		return XDATA_TYPE_U8;
	} else
	if( str.CompareNoCase(_T("i1")) == 0 || str.CompareNoCase(_T("int 1")) == 0 ) {
		return XDATA_TYPE_I1;
	} else
	if( str.CompareNoCase(_T("i2")) == 0 || str.CompareNoCase(_T("int 2")) == 0 ) {
		return XDATA_TYPE_I2;
	} else
	if( str.CompareNoCase(_T("i4")) == 0 || str.CompareNoCase(_T("int 4")) == 0 ) {
		return XDATA_TYPE_I4;
	} else
	if( str.CompareNoCase(_T("i8")) == 0 || str.CompareNoCase(_T("int 8")) == 0 ) {
		return XDATA_TYPE_I8;
	} else
	if( str.CompareNoCase(_T("f4")) == 0 || str.CompareNoCase(_T("float 4")) == 0 ) {
		return XDATA_TYPE_F4;
	} else
	if( str.CompareNoCase(_T("f8")) == 0 || str.CompareNoCase(_T("float 8")) == 0 ) {
		return XDATA_TYPE_F8;
	} else
	if( str.CompareNoCase(_T("ux")) == 0 || str.CompareNoCase(_T("unsigned int")) == 0 ) {
		return XDATA_TYPE_UX;
	} else
	if( str.CompareNoCase(_T("ix")) == 0 || str.CompareNoCase(_T("int")) == 0 ) {
		return XDATA_TYPE_IX;
	} else
	if( str.CompareNoCase(_T("fx")) == 0 || str.CompareNoCase(_T("float")) == 0 ) {
		return XDATA_TYPE_FX;
	}
XEXCEPT_EXIT
	return XDATA_TYPE_END;
}

const char* XData::getDataTypeStringA_From_TYPE(const XDATA_TYPE type)
{
XEXCEPT_ENTRY
	switch( type ) {
	case XDATA_TYPE_L:
		return "L";
	case XDATA_TYPE_B:
		return "B";
	case XDATA_TYPE_BOOL:
		return "BOOL";
	case XDATA_TYPE_A:
		return "A";
	case XDATA_TYPE_UTF16:
		return "UTF16";
	case XDATA_TYPE_J:
		return "J";
	case XDATA_TYPE_U1:
		return "U1";
	case XDATA_TYPE_U2:
		return "U2";
	case XDATA_TYPE_U4:
		return "U4";
	case XDATA_TYPE_U8:
		return "U8";
	case XDATA_TYPE_I1:
		return "I1";
	case XDATA_TYPE_I2:
		return "I2";
	case XDATA_TYPE_I4:
		return "I4";
	case XDATA_TYPE_I8:
		return "I8";
	case XDATA_TYPE_F4:
		return "F4";
	case XDATA_TYPE_F8:
		return "F8";
	case XDATA_TYPE_UX:
		return "UX";
	case XDATA_TYPE_IX:
		return "IX";
	case XDATA_TYPE_FX:
		return "FX";
	}
XEXCEPT_EXIT
	return "unknown";
}
const wchar_t* XData::getDataTypeStringW_From_TYPE(const XDATA_TYPE type)
{
XEXCEPT_ENTRY
	switch( type ) {
	case XDATA_TYPE_L:
		return L"L";
	case XDATA_TYPE_B:
		return L"B";
	case XDATA_TYPE_BOOL:
		return L"BOOL";
	case XDATA_TYPE_A:
		return L"A";
	case XDATA_TYPE_UTF16:
		return L"UTF16";
	case XDATA_TYPE_J:
		return L"J";
	case XDATA_TYPE_U1:
		return L"U1";
	case XDATA_TYPE_U2:
		return L"U2";
	case XDATA_TYPE_U4:
		return L"U4";
	case XDATA_TYPE_U8:
		return L"U8";
	case XDATA_TYPE_I1:
		return L"I1";
	case XDATA_TYPE_I2:
		return L"I2";
	case XDATA_TYPE_I4:
		return L"I4";
	case XDATA_TYPE_I8:
		return L"I8";
	case XDATA_TYPE_F4:
		return L"F4";
	case XDATA_TYPE_F8:
		return L"F8";
	case XDATA_TYPE_UX:
		return L"UX";
	case XDATA_TYPE_IX:
		return L"IX";
	case XDATA_TYPE_FX:
		return L"FX";
	}
XEXCEPT_EXIT
	return L"unknown";
}
const TCHAR* XData::getDataTypeString_From_TYPE(const XDATA_TYPE type)
{
#ifdef _UNICODE
	return getDataTypeStringW_From_TYPE( type );
#else
	return getDataTypeStringA_From_TYPE( type );
#endif
}
XD_UINT1 XData::getLengthByte(XD_UINT1 type, XD_UINT4 count)
{
	XD_UINT4 nSizeOfSize = 1;
	XD_UINT4 nReal = 1;
	XBIN nType = XD2_END;
	XBIN nRels = 0;
	XBIN nTy;


	nReal <<= (type & 0x3);
	nTy = (type & ~0x3) >> 2; 
	nRels = type & 0x3; 

XEXCEPT_ENTRY
	nSizeOfSize = 0x01;
	if( count > 0x000000FF ) {
		nSizeOfSize = 0x02;
	}
	if( count > 0x0000FFFF ) {
		nSizeOfSize = 0x03;
	}
XEXCEPT_EXIT
	return (XD_UINT1)nSizeOfSize;
}






//-------------------------------------------------------------------------------------
//
//	XDataXL
//
//-------------------------------------------------------------------------------------
XDataXL::XDataXL()
	:	m_ptagName(NULL)
	,	m_ntagNameSize(0)
	,	m_pAttrList(NULL)
	,	m_nAttrListSize(0)
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XDataXL::~XDataXL()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XStrBuff XDataXL::getTagName()
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	XDataReader reader(m_ptagName, m_ntagNameSize);
	XData data;


	if( m_ptagName == NULL && m_ntagNameSize ) {
		m_error = XD2_ERRDI;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return str;
	}
	if( reader.getValue(&data) == false ) {
		m_error = XD2_ERRDI;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return str;
	}
	if( data.getType() != XD2_A ) {
		m_error = XD2_ERRTY;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return str;
	}
	return data.getValAsStringA();
XEXCEPT_EXIT
}










//-------------------------------------------------------------------------------------
//
//	XDataCoder
//
//-------------------------------------------------------------------------------------
XDataCoder::XDataCoder()
	:	m_error(0)
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XDataCoder::~XDataCoder()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
unsigned char XDataCoder::biCS2pgCS(XBIN cs)
{
XEXCEPT_ENTRY
	unsigned char ret = 0xFF;
	switch ( cs ) {
	case 0x01:
		ret = 0;		
		break;
	case 0x02:
		ret = 1;
		break;
	case 0x04:
		ret = 2;
		break;
	case 0x00:
		ret = 3;
		break;
	}
	return ret;
XEXCEPT_EXIT
}
XD_INT2 XDataCoder::_hton2(XD_INT2 h) 
{
XEXCEPT_ENTRY
	XD_INT2			d;
	int             i;
	XBIN*			pucSrcData = (XBIN*)&h;
	XBIN*			pucDstData = (XBIN*)&d;

	for( i = 0; i<sizeof(XD_INT2); i++) {
		pucDstData[i] = pucSrcData[sizeof(XD_INT2)-i-1];
	}
	return(d);
XEXCEPT_EXIT
}
XD_INT4 XDataCoder::_hton4(XD_INT4 h) 
{
XEXCEPT_ENTRY
	XD_INT4			d;
	int             i;
	XBIN*	pucSrcData = (XBIN*)&h;
	XBIN*	pucDstData = (XBIN*)&d;

	for(i = 0; i<sizeof(XD_INT4); i++) {
		pucDstData[i] = pucSrcData[sizeof(XD_INT4)-i-1];
	}
	return(d);
XEXCEPT_EXIT
}
XD_UINT8 XDataCoder::_hton8(XD_UINT8 h) 
{
XEXCEPT_ENTRY
	XD_UINT8		d;
	int             i;
	XBIN*	pucSrcData = (XBIN*)&h;
	XBIN*	pucDstData = (XBIN*)&d;

	for(i = 0; i<sizeof(XD_UINT8); i++) {
		pucDstData[i] = pucSrcData[sizeof(XD_UINT8)-i-1];
	}
	return(d);
XEXCEPT_EXIT
}
XD_INT2 XDataCoder::_ntoh2(XD_INT2 h) 
{
XEXCEPT_ENTRY
	XD_INT2			d;
	int             i;
	XBIN*	pucSrcData = (XBIN*)&h;
	XBIN*	pucDstData = (XBIN*)&d;

	for(i = 0; i<sizeof(XD_INT2); i++) {
		pucDstData[i] = pucSrcData[sizeof(XD_INT2)-i-1];
	}
	return(d);
XEXCEPT_EXIT
}
XD_INT4 XDataCoder::_ntoh4(XD_INT4 h)
{
XEXCEPT_ENTRY
	XD_INT4			d;
	int             i;
	XBIN*	pucSrcData = (XBIN*)&h;
	XBIN*	pucDstData = (XBIN*)&d;

	for(i = 0; i<sizeof(XD_INT4); i++) {
		pucDstData[i] = pucSrcData[sizeof(XD_INT4)-i-1];
	}
	return(d);
XEXCEPT_EXIT
}
XD_UINT8 XDataCoder::_ntoh8(XD_UINT8 h) 
{
XEXCEPT_ENTRY
	XD_UINT8		d;
	int             i;
	XBIN*	pucSrcData = (XBIN*)&h;
	XBIN*	pucDstData = (XBIN*)&d;

	for(i = 0; i<sizeof(XD_UINT8); i++) {
		pucDstData[i] = pucSrcData[sizeof(XD_UINT8)-i-1];
	}
	return(d);
XEXCEPT_EXIT
}
int XDataCoder::getError()
{
XEXCEPT_ENTRY
	return m_error;
XEXCEPT_EXIT
}
void XDataCoder::resetError()
{
XEXCEPT_ENTRY
	m_error = 0;
XEXCEPT_EXIT
}





//-------------------------------------------------------------------------------------
//
//	XDataReader
//
//-------------------------------------------------------------------------------------
XDataReader::XDataReader(XBIN *pData, XD_UINT4 nSize)
	:	m_pDataFirst(pData)
	,	m_pCurrentData(pData)
	,	m_nDataSize(nSize)
	,	m_nCurrentDataSize(nSize)
{
XEXCEPT_ENTRY
	m_cCurrentType = (XDATA_TYPE)m_pDataFirst[0];
XEXCEPT_EXIT
}

XDataReader::~XDataReader()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

bool XDataReader::getValue(XData *pData)
{
XEXCEPT_ENTRY
	XBIN cn = 0;			/* SECS BIN Type */
	XBIN ct = 0;			/* Conversion Type */
	XBIN cs = 0;			/* Real of Type */
	XBIN sc = 0;			/* Size of Size */
	int iSize = 0;

	if( m_nCurrentDataSize == 0 ) {
		return false;
	}
	cn = m_pCurrentData[0];
	ct = (cn & 0xE0);
	cs = (cn & (0x07 << 2 ))>>2;
	sc = (cn & 0x03);

	m_pCurrentData++;
	m_nCurrentDataSize--;

	if( m_nCurrentDataSize - sc < 0 ) {
		m_error = XD2_ERRSS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}

	iSize = GetDataSize( sc );
	if( iSize == -1) {
		m_error = XD2_ERRSS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}

	m_pCurrentData     += sc;
	m_nCurrentDataSize -= sc;

	pData->m_nSizeOfSize = sc;

	
	switch( ct ) {
	case 0:
		{
			m_cCurrentType = (XDATA_TYPE)XD2_L;
			// List.
			pData->m_nSize = iSize;
			pData->m_nType = m_cCurrentType;
			pData->m_pValue = NULL;

			/* Move Next  */
			if( SetNext() < 0 ) {
				m_error = XD2_ERRSS;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}			
				return false;
			}
			
			return true;
		}
		break;
	case 0x20:
		{
			if( cs == 0 ) {
				m_cCurrentType = (XDATA_TYPE)XD2_B;
			} else
			if( cs == 1 ) {
				m_cCurrentType = (XDATA_TYPE)XD2_BOOL;
			} else {
				m_error = XD2_ERRTY;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
				return false;
			}
		}
		break;
	case 0x40:
		{
			if( cs == 0 ) {
				m_cCurrentType = (XDATA_TYPE)XD2_A;
			} else
			if( cs == 1 ) {
				m_cCurrentType = (XDATA_TYPE)XD2_J;
			} else
			if( cs == 2 ) {
				m_cCurrentType = (XDATA_TYPE)XDATA_TYPE_UTF16;
			} else {
				m_error = XD2_ERRTY;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
				return false;
			}
		}
		break;
	case 0XA0:
		// Ux.
		m_cCurrentType = (XDATA_TYPE)XD2_TYP(6, biCS2pgCS(cs));
		break;
	case 0x60:
		// Ix.
		m_cCurrentType = (XDATA_TYPE)XD2_TYP(7, biCS2pgCS(cs));
		break;
	case 0x80:
		// Fx
		m_cCurrentType = (XDATA_TYPE)XD2_TYP(8, biCS2pgCS(cs));
		break;
	default:
		m_error = XD2_ERRTY;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}

	if( m_nCurrentDataSize - iSize < 0 ) {
		m_error = XD2_ERRSS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	pData->m_nSize = iSize;
	pData->m_nType = m_cCurrentType;
	pData->m_pValue = m_pCurrentData;

	m_pCurrentData     += iSize;
	m_nCurrentDataSize -= iSize;

	/* Move Next  */
	if( SetNext() < 0 ) {
		m_error = XD2_ERRSS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}

	return true;
XEXCEPT_EXIT
}
bool XDataReader::skipList(XData *pData)
{
	if( pData->getType() != XDATA_TYPE_L ) {
		return false;
	}
	XD_UINT4 cnt = pData->getCount();
	for(XD_UINT4 i = 0; i < cnt; i++) {

		if( this->getValue(pData) == false ) { return false; }
		if( pData->getType() == XDATA_TYPE_L ) {
			if( this->skipList(pData) == false ) { return false; }
		}
	}
	return true;
}
bool XDataReader::print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XStrBuff blank;
	return printInner(stream, nDepth, blank, pLF);
XEXCEPT_EXIT
}


XD_INT4 XDataReader::test()
{
XEXCEPT_ENTRY
	XBIN*				SAVE_pDataFirst			= m_pDataFirst;
	XBIN*				SAVE_pCurrentData		= m_pCurrentData;
	int					SAVE_nDataSize			= m_nDataSize;
	int					SAVE_nCurrentDataSize	= m_nCurrentDataSize;
	XDATA_TYPE			SAVE_cCurrentType		= m_cCurrentType;	
	XData				data;
	bool				bResultOK = true;

	if( m_nDataSize == 0 ) {
		return XD2_ERRSS;
	}

	if( this->getValue(&data) == false ) {
		bResultOK = false;
	}
	if( testInner(data) == false) {
		bResultOK = false;
	}
	m_pDataFirst		= SAVE_pDataFirst;
	m_pCurrentData		= SAVE_pCurrentData;
	m_nDataSize			= SAVE_nDataSize;
	m_nCurrentDataSize	= SAVE_nCurrentDataSize;
	m_cCurrentType		= SAVE_cCurrentType;
	return bResultOK ? 0 : m_error;
XEXCEPT_EXIT
}

void XDataReader::getRaw(XBIN** pAdder, int* pSize)
{
XEXCEPT_ENTRY
	*pAdder		= m_pDataFirst;
	*pSize		= m_nDataSize;
XEXCEPT_EXIT
}
void XDataReader::getCurrentRaw(XBIN** pAdder, int* pSize)
{
XEXCEPT_ENTRY
	*pAdder		= m_pCurrentData;
	*pSize		= m_nCurrentDataSize;
XEXCEPT_EXIT
}

bool XDataReader::testInner(XData &data)
{
XEXCEPT_ENTRY
	if( data.getType() == XDATA_TYPE_L ) {
		XD_UINT4 iCount = data.getCount();
		for(XD_UINT4 i = 0; i < iCount; i++) {
			if( m_nCurrentDataSize == 0 ) {
				m_error = XD2_ERRSS;
				return false;
			}
			if( this->getValue(&data) == false ) {
				return false;
			}
			if( testInner(data) == false ) {
				return false;
			}
		}
	}
	return true;
XEXCEPT_EXIT
}


bool XDataReader::printInner(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth,  XStrBuff &blank, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XData data;
	TCHAR *pBlak = NULL;


	if( blank.getCountOfBuff() < nDepth+10 ) {
		pBlak = (TCHAR*)blank.Alloc( (nDepth+60) * sizeof(TCHAR) );
	} else {
		pBlak = (TCHAR*)blank;
	}
	if( pBlak == NULL ) {
		return false;
	}
	pBlak = (TCHAR*)blank;
	blank.memsetT(_T(' '));
	pBlak[nDepth] = 0;

	do {
		if( this->getValue(&data) == false ) {
			break;
		}
		if( data.getType() == XD2_L ) {
			stream << (const TCHAR*)blank << _T("<L[") << data.getSize() << _T("/1]");
			if( data.getSize() == 0 ) {
				stream << _T(">") << pLF;
				return true;				
			}
			stream << pLF;
			for( size_t i = 0; i < data.getSize(); i++) {
				if( i > 10 )
				{
					stream << "And More......." << pLF;
					break;
				}
				if( printInner(stream, nDepth+1, blank, pLF) == false ) {
					return false;
				}
			}

			pBlak = (TCHAR*)blank;
			blank.memsetT(_T(' '));
			pBlak[nDepth] = 0;

			stream << (const TCHAR*)blank << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_A ) {
#define XDataReader_PRINT_SML_START	stream << (const TCHAR*)blank << _T("<") << XData::getDataTypeString_From_TYPE(data.getType()) << _T("[") << data.getSize() << _T("/") << (int)data.getLengthByte() << _T("] ");
			XDataReader_PRINT_SML_START;
			// 長過ぎる文字列はカットする
			stream << _T("\"") << (const TCHAR*)data.getValAsStringA() << _T("\">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_J ) {
			XDataReader_PRINT_SML_START;
			stream << _T("\"") << (const TCHAR*)data.getValAsStringJ() << _T("\">") << pLF;
			return true;
		} else 
		if( data.getType() == XDATA_TYPE_UTF16 ) {
			XDataReader_PRINT_SML_START;
			stream << _T("\"") << (const TCHAR*)data.getValAsStringUTF16() << _T("\">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_BOOL ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				if( data.getValAsBool(i) == true ) {
					stream << _T("true");
				} else {
					stream << _T("false");
				}
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_B ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				TCHAR sz[5] ={0};
				if( i != 0 ) {	stream << _T(" ");	}
				XStrBuff::printf(sz, XCOUNTOF(sz), _T("0x%02x"), data.getValAsUInt1(i) );
				stream << sz ;

				if( i != 0 &&  ( i % 30 ) == 0 ) {
					stream << pLF << (const TCHAR*)blank << _T("   ");
				}
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_I1 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (int)data.getValAsInt1(i);
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_I2 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << data.getValAsInt2(i);
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_I4 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << data.getValAsInt4(i);
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_I8 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				TCHAR sz[256] = {0};
				XStrBuff::printf(sz, XCOUNTOF(sz), _T("%I64d"), data.getValAsInt8(i));
				stream << sz;
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_U1 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << (unsigned int)data.getValAsUInt1(i);
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_U2 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << data.getValAsUInt2(i);
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_U4 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << data.getValAsUInt4(i);
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_U8 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				TCHAR sz[256] = {0};
				XStrBuff::printf(sz, XCOUNTOF(sz), _T("%I64u"), data.getValAsUInt8(i));
				stream << sz;
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_F4 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}
				stream << data.getValAsFloat4();
			}
			stream << _T(">") << pLF;
			return true;
		} else 
		if( data.getType() == XD2_F8 ) {
			XDataReader_PRINT_SML_START;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {
					stream << _T(" ");
				}

				stream << data.getValAsFloat8();
			}
			stream << _T(">") << pLF;
			return true;
		} else {
			m_error = XD2_ERRTY;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		}
	} while(1);
	return true;
XEXCEPT_EXIT
}

XD_INT4 XDataReader::SetNext()
{
XEXCEPT_ENTRY
	XBIN cn = *m_pCurrentData;
	XBIN cs = 0;	/* Real of Type */
	XBIN sc = 0;	/* Size of Size */

	m_cCurrentType = (XDATA_TYPE)((cn & 0x1C) >> 2);
	sc = cn & 0x03;

	return (XD_INT4)sc;
XEXCEPT_EXIT
}


XD_INT4 XDataReader::GetDataSize(XD_UINT1 nSizeOfSize)
{
XEXCEPT_ENTRY
	XD_INT4 nSize = -1;
	XBIN  cData = 0;
	unsigned short sData = 0;
	unsigned long  lData = 0;
	
	
	if( nSizeOfSize == 1 ) {
		memcpy(&cData, m_pCurrentData, nSizeOfSize);
		nSize = (int)cData;
	} else if( nSizeOfSize == 2 ) {
		memcpy(&sData, m_pCurrentData, nSizeOfSize);
		sData = _hton2(sData);
		nSize = (int)sData;
	} else if( nSizeOfSize == 3 ) {
		memcpy(&lData, m_pCurrentData, nSizeOfSize);
		lData = (int)_hton4(lData);
		nSize = (int)lData;
		nSize >>= 8;
	} else {
		m_error = XD2_ERRDI;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
	}
	return nSize;
XEXCEPT_EXIT
}




//-------------------------------------------------------------------------------------
//
//	XDataWriter
//
//-------------------------------------------------------------------------------------
XDataWriter::XDataWriter(std::ostream *stream)
	:	m_stream(stream)
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XDataWriter::~XDataWriter()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

std::ostream* XDataWriter::get_stream()
{
XEXCEPT_ENTRY
	return m_stream;
XEXCEPT_EXIT
}

bool XDataWriter::putValAsEnd()
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_END, NULL, 0);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsList(const XD_UINT4 nListCout)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_L, NULL, nListCout);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsBool(const bool value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_BOOL, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsBinary(const XBIN *pValue, int nSize)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_B, (void*)pValue, nSize);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsStringA(const char *pValue)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_A, (void*)pValue, ::strlen(pValue));
XEXCEPT_EXIT
}
bool XDataWriter::putValAsStringA(const wchar_t *pValue)
{
XEXCEPT_ENTRY
	XStrBuff strTmp(pValue);
	return putValAsStringA((const char*)strTmp);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsStringW(const wchar_t *pValue)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_UTF16, (void*)pValue, ::wcslen(pValue));
XEXCEPT_EXIT
}
bool XDataWriter::putValAsStringJ(const char *pValue)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_J, (void*)pValue, ::strlen(pValue));
XEXCEPT_EXIT
}
bool XDataWriter::putValAsStringJ(const wchar_t *pValue)
{
XEXCEPT_ENTRY
	XStrBuff strTmp(pValue);
	return putValAsStringJ((const char*)strTmp);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsInt1(const char value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_I1, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsInt2(const XD_INT2 value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_I2, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsInt4(const XD_INT4 value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_I4, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsInt8(const XD_INT8 value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_I8, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsUInt1(const unsigned char value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_U1, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsUInt2(const XD_UINT2 value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_U2, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsUInt4(const XD_UINT4 value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_U4, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsUInt8(const XD_UINT8 value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_U8, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsFloat4(const XD_FLOAT4 value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_F4, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValAsFloat8(const XD_FLOAT8 value)
{
XEXCEPT_ENTRY
	return putValue(XDATA_TYPE_F8, (void*)&value, 1);
XEXCEPT_EXIT
}
bool XDataWriter::putValue(XDATA_TYPE datatype, void *pValue, XD_UINT4 count, XD_INT1 lengthByte /*= -1*/)
{
XEXCEPT_ENTRY
	XBIN nRels = 0;
	XBIN nTy;
	XBIN type = (XBIN)datatype;

	nTy = (type & ~0x3) >> 2; 
	nRels = type & 0x3; 

	if( type == XD2_END ) {
		m_stream->rdbuf()->sputn((char*)&type, 1);
		return true;
	}

	if( writeType(type, count, lengthByte) < 0 ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
	}
	if( type == XD2_L ) {
		return true;
	}

	switch( nRels ) {
	case 0:
		{
			m_stream->rdbuf()->sputn((char*)pValue, count);		
		}
		break;
	case 1:
		{
			XD_INT2 data;
			XD_INT2 *pWork = (XD_INT2*)pValue;

			for(XD_UINT4 i = 0; i < count; i++) {
				data = this->_hton2(*pWork);
				m_stream->rdbuf()->sputn((char*)&data, 2);	
				pWork++;
			}
		}
		break;
	case 2:
		{
			XD_INT4 data;
			XD_INT4 *pWork = (XD_INT4*)pValue;

			for(XD_UINT4 i = 0; i < count; i++) {
				data = this->_hton4(*pWork);
				m_stream->rdbuf()->sputn((char*)&data, 4);	
				pWork++;
			}
		}
		break;
	case 3:
		{
			XD_INT8 data;
			XD_INT8 *pWork = (XD_INT8*)pValue;

			for(XD_UINT4 i = 0; i < count; i++) {
				data = this->_hton8(*pWork);
				m_stream->rdbuf()->sputn((char*)&data, 8);	
				pWork++;
			}
		}
		break;
	}



	return true;
XEXCEPT_EXIT
}

bool XDataWriter::putCastValue(XDATA_TYPE castType, XDATA_TYPE inputDatatype, void *pValue, XD_UINT4 count, XD_INT1 lengthByte /*= -1*/)
{
XEXCEPT_ENTRY
	XD_UINT4 i = 0;

	if( castType == inputDatatype ) {
		// 変換タイプが同じ場合は、変換の必要がないので、putValueをコールする。
		return this->putValue(inputDatatype, pValue, count, lengthByte);
	}
	if( count == 0 ) {
		// データカウントが０の場合は、値を変換する必要がないので、putValueをコールする。
		// タグのみ書き込む
		return this->putValue(castType, pValue, count, lengthByte);
	}


	if( castType == XDATA_TYPE_U1 ||
		castType == XDATA_TYPE_B ||
		castType == XDATA_TYPE_BOOL) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_UINT1 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_UINT1)XStrBuff::strtoullW((const wchar_t*)pValue, NULL, 0);
			} else {
				val = (XD_UINT1)XStrBuff::strtoullA((const char*)pValue, NULL, 0);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_UINT1* pValue = (XD_UINT1*)buf.Alloc(sizeof(XD_UINT1)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_UINT1)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_UINT1)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_UINT1)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_UINT1)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_UINT1)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_UINT1)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_UINT1)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_UINT1)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_UINT1)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_UINT1)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_UINT1)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_UINT1)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_U2 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_UINT2 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_UINT2)XStrBuff::strtoullW((const wchar_t*)pValue, NULL, 0);
			} else {
				val = (XD_UINT2)XStrBuff::strtoullA((const char*)pValue, NULL, 0);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_UINT2* pValue = (XD_UINT2*)buf.Alloc(sizeof(XD_UINT2)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_UINT2)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_UINT2)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_UINT2)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_UINT2)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_UINT2)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_UINT2)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_UINT2)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_UINT2)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_UINT2)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_UINT2)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_UINT2)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_UINT2)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_U4 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_UINT4 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_UINT4)XStrBuff::strtoullW((const wchar_t*)pValue, NULL, 0);
			} else {
				val = (XD_UINT4)XStrBuff::strtoullA((const char*)pValue, NULL, 0);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_UINT4* pValue = (XD_UINT4*)buf.Alloc(sizeof(XD_UINT4)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_UINT4)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_UINT4)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_UINT4)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_UINT4)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_UINT4)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_UINT4)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_UINT4)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_UINT4)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_UINT4)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_UINT4)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_UINT4)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_UINT4)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_U8 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_UINT8 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_UINT8)XStrBuff::strtoullW((const wchar_t*)pValue, NULL, 0);
			} else {
				val = (XD_UINT8)XStrBuff::strtoullA((const char*)pValue, NULL, 0);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_UINT8* pValue = (XD_UINT8*)buf.Alloc(sizeof(XD_UINT8)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_UINT8)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_UINT8)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_UINT8)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_UINT8)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_UINT8)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_UINT8)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_UINT8)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_UINT8)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_UINT8)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_UINT8)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_UINT8)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_UINT8)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_I1 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_INT1 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_INT1)XStrBuff::strtollW((const wchar_t*)pValue, NULL, 0);
			} else {
				val = (XD_INT1)XStrBuff::strtollA((const char*)pValue, NULL, 0);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_INT1* pValue = (XD_INT1*)buf.Alloc(sizeof(XD_INT1)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_INT1)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_INT1)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_INT1)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_INT1)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_INT1)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_INT1)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_INT1)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_INT1)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_INT1)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_INT1)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_INT1)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_INT1)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_I2 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_INT2 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_INT2)XStrBuff::strtollW((const wchar_t*)pValue, NULL, 0);
			} else {
				val = (XD_INT2)XStrBuff::strtollA((const char*)pValue, NULL, 0);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_INT2* pValue = (XD_INT2*)buf.Alloc(sizeof(XD_INT2)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_INT2)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_INT2)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_INT2)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_INT2)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_INT2)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_INT2)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_INT2)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_INT2)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_INT2)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_INT2)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_INT2)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_INT2)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_I4 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_INT4 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_INT4)XStrBuff::strtollW((const wchar_t*)pValue, NULL, 0);
			} else {
				val = (XD_INT4)XStrBuff::strtollA((const char*)pValue, NULL, 0);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_INT4* pValue = (XD_INT4*)buf.Alloc(sizeof(XD_INT4)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_INT4)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_INT4)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_INT4)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_INT4)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_INT4)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_INT4)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_INT4)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_INT4)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_INT4)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_INT4)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_INT4)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_INT4)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_I8 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_INT8 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_INT8)XStrBuff::strtollW((const wchar_t*)pValue, NULL, 0);
			} else {
				val = (XD_INT8)XStrBuff::strtollA((const char*)pValue, NULL, 0);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_INT8* pValue = (XD_INT8*)buf.Alloc(sizeof(XD_INT8)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_INT8)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_INT8)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_INT8)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_INT8)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_INT8)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_INT8)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_INT8)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_INT8)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_INT8)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_INT8)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_INT8)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_INT8)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_F4 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_FLOAT4 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_FLOAT4)XStrBuff::strtodW((const wchar_t*)pValue, NULL);
			} else {
				val = (XD_FLOAT4)XStrBuff::strtodA((const char*)pValue, NULL);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}

		XBuff buf;
		XD_FLOAT4* pValue = (XD_FLOAT4*)buf.Alloc(sizeof(XD_FLOAT4)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_FLOAT4)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_FLOAT4)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_FLOAT4)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_FLOAT4)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_FLOAT4)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_FLOAT4)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_FLOAT4)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_FLOAT4)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_FLOAT4)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_FLOAT4)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_FLOAT4)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_FLOAT4)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_F8 ) {
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_UTF16 ||
			inputDatatype == XDATA_TYPE_J) {
			// 文字列の場合。
			XD_FLOAT8 val = 0;

			if( inputDatatype == XDATA_TYPE_UTF16 ) {
				val = (XD_FLOAT8)XStrBuff::strtodW((const wchar_t*)pValue, NULL);
			} else {
				val = (XD_FLOAT8)XStrBuff::strtodA((const char*)pValue, NULL);
			}
			return this->putValue(castType, &val, 1, lengthByte);
		}
		
		XBuff buf;
		XD_FLOAT8* pValue = (XD_FLOAT8*)buf.Alloc(sizeof(XD_FLOAT8)  * count);
		if( pValue == NULL ) {
			return false;
		}
		for(i = 0; i < count; i++ ) {
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				pValue[i] = (XD_FLOAT8)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_BOOL:
				pValue[i] = (XD_FLOAT8)((XBIN*)pValue)[i];
				break;
			case XDATA_TYPE_U1:
				pValue[i] = (XD_FLOAT8)((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				pValue[i] = (XD_FLOAT8)((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				pValue[i] = (XD_FLOAT8)((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				pValue[i] = (XD_FLOAT8)((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				pValue[i] = (XD_FLOAT8)((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				pValue[i] = (XD_FLOAT8)((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				pValue[i] = (XD_FLOAT8)((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				pValue[i] = (XD_FLOAT8)((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				pValue[i] = (XD_FLOAT8)((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				pValue[i] = (XD_FLOAT8)((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		return this->putValue(castType, pValue, count, lengthByte);
	} else
	if( castType == XDATA_TYPE_A ||
		castType == XDATA_TYPE_UTF16 ||
		castType == XDATA_TYPE_J ) {
		if( inputDatatype == XDATA_TYPE_UTF16 ) {
			XStrBuff str((const wchar_t*)pValue, count);
			if( castType == XDATA_TYPE_A ) {
				return this->putValAsStringA( (const TCHAR*)str );
			} else
			if( castType == XDATA_TYPE_UTF16 ) {
				return this->putValAsStringW( (const wchar_t*)str );
			} else
			if( castType == XDATA_TYPE_J ) {
				return this->putValAsStringJ( (const TCHAR*)str );
			}
		} else
		if( inputDatatype == XDATA_TYPE_A ||
			inputDatatype == XDATA_TYPE_J) {
			XStrBuff str((const char*)pValue, count);
			if( castType == XDATA_TYPE_A ) {
				return this->putValAsStringA( (const TCHAR*)str );
			} else
			if( castType == XDATA_TYPE_UTF16 ) {
				return this->putValAsStringW( (const wchar_t*)str );
			} else
			if( castType == XDATA_TYPE_J ) {
				return this->putValAsStringJ( (const TCHAR*)str );
			}
		}


		XStrBuff  str(_T(""));
		for(i = 0; i < count; i++ ) {
			
			if( i != 0 ) {
				str << _T(" ");
			}
			switch( inputDatatype ) {
			case XDATA_TYPE_B:
				{
					TCHAR szBufHex[6] = {0};
					XStrBuff::printf(szBufHex, XCOUNTOF(szBufHex), _T("%X"), 
						((XBIN*)pValue)[i]);
					str << szBufHex;
				}
				break;
			case XDATA_TYPE_BOOL:
				if( ((XBIN*)pValue)[i] == 0 ) {
					str << _T("false");
				} else {
					str << _T("true");
				}
				break;
			case XDATA_TYPE_U1:
				str << ((XD_UINT1*)pValue)[i];
				break;
			case XDATA_TYPE_U2:
				str << ((XD_UINT2*)pValue)[i];
				break;
			case XDATA_TYPE_U4:
				str << ((XD_UINT4*)pValue)[i];
				break;
			case XDATA_TYPE_U8:
				str << ((XD_UINT8*)pValue)[i];
				break;
			case XDATA_TYPE_I1:
				str << ((XD_INT1*)pValue)[i];
				break;
			case XDATA_TYPE_I2:
				str << ((XD_INT2*)pValue)[i];
				break;
			case XDATA_TYPE_I4:
				str << ((XD_INT4*)pValue)[i];
				break;
			case XDATA_TYPE_I8:
				str << ((XD_INT8*)pValue)[i];
				break;
			case XDATA_TYPE_F4:
				str << ((XD_FLOAT4*)pValue)[i];
				break;
			case XDATA_TYPE_F8:
				str << ((XD_FLOAT8*)pValue)[i];
				break;
			default:
				return false;
			}
		}
		if( castType == XDATA_TYPE_A ) {
			return this->putValAsStringA( (const TCHAR*)str );
		} else
		if( castType == XDATA_TYPE_UTF16 ) {
			return this->putValAsStringW( (const wchar_t*)str );
		} else
		if( castType == XDATA_TYPE_J ) {
			return this->putValAsStringJ( (const TCHAR*)str );
		}
	} else {
		// 変換不可能
		return false;
	}

	return true;
XEXCEPT_EXIT
}

XD_INT4 XDataWriter::writeType(unsigned char type, int count, XD_INT1 lengthByte /*= -1*/)
{
XEXCEPT_ENTRY
	int nSizeOfSize = 1;
	int nReal = 1;
	unsigned char nType = XD2_END;
	unsigned char nType2 = 0;
	unsigned char nRels = 0;
	unsigned char nTy;


	nReal <<= (type & 0x3);
	nTy = (type & ~0x3) >> 2; 
	nRels = type & 0x3; 

	nSizeOfSize = XData::getLengthByte(type, count );
	count *= nReal;
/*	
	nSizeOfSize = 0x01;
	if( count > 0x000000FF ) {
		nSizeOfSize = 0x02;
	}
	if( count > 0x0000FFFF ) {
		nSizeOfSize = 0x03;
	}
*/
	/* Over Flow */
	if( (0x80000000 & count) != 0 ) {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return -1;
	}

	// decorate size of size
	if( lengthByte != -1 ) {
		if( !(1 <= lengthByte &&  lengthByte <= 3) ) {
			m_error = XD2_ERRAS;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
			return -1;
		}
		if( lengthByte > nSizeOfSize ) {
			nSizeOfSize = lengthByte;
		}
	}

	if( nTy == 1 ) {
		// LIST
		nType = 0;
	} else if(nTy == 2) {					/* BI */
		nType = 0x20;
	} else if(nTy == 3) {					/* BOOLENAN */
		nType = 0x20;
		nType2 = 0x01;
	} else if(nTy == 4 || nTy == 9) {		/* AS */
		nType = 0x40;
		if(nRels == 0x0) {nType2 = 0x00;}
		if(nRels == 0x1) {nType2 = 0x02;}
	} else if(nTy == 5 || nTy == 10) {		/* J */
		nType = 0x40;
		nType2 = 0x01;
	} else if(nTy == 6 || nTy == 11) {		/* U */
		if(nRels == 0x0) {nType2 = 0x01;}
		if(nRels == 0x1) {nType2 = 0x02;}
		if(nRels == 0x2) {nType2 = 0x04;}
		if(nRels == 0x3) {nType2 = 0x00;}  /* ?? */

		
		nType = 0XA0;
	} else if(nTy == 7 || nTy == 12) {		/* I */
		if(nRels == 0x0) {nType2 = 0x01;}
		if(nRels == 0x1) {nType2 = 0x02;}
		if(nRels == 0x2) {nType2 = 0x04;}
		if(nRels == 0x3) {nType2 = 0x00;}  /* ?? */

		nType = 0x60;
	} else if(nTy == 8 || nTy == 13) {		/* F */
		if(nRels == 0x0) {nType2 = 0x01;}
		if(nRels == 0x1) {nType2 = 0x02;}
		if(nRels == 0x2) {nType2 = 0x04;}
		if(nRels == 0x3) {nType2 = 0x00;}  /* ?? */

		nType = 0X80;
	} else {
		m_error = XD2_ERRAS;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return -1;
	}
	nType2 <<= 2;
	nType |= (unsigned char)nSizeOfSize;
	nType |= nType2;
	m_stream->write((char*)&nType, 1);

	
	/* Set Size DATA */
	if( nSizeOfSize == 1 ) {
		{
			unsigned char nSize = (unsigned char)(count);
			m_stream->write((char*)&nSize, 1);
		}
	} else if( nSizeOfSize == 2 ) { 
		{
			XD_UINT2 nSize = (XD_UINT2)(count);
			nSize = this->_hton2(nSize);
			m_stream->write((char*)&nSize, 2);
		}
	} else /* if( nSizeOfSize == 3 ) */ { 
		{
			XD_UINT4 nSize = (XD_UINT4)(count);
			nSize = this->_hton4(nSize);
			nSize >>=8;
			m_stream->write((char*)&nSize, 3);
		}
	} 
	return nSizeOfSize;
XEXCEPT_EXIT
}




//-------------------------------------------------------------------------------------
//
//	XDataAttribute
//
//-------------------------------------------------------------------------------------
XDataAttribute::XDataAttribute()
	:	m_buf(0x7F, 0xFF)
	,	m_strTagName(_T(""))
	,	m_stream( &m_buf )
	,	XDataWriter( &m_stream )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XDataAttribute::~XDataAttribute()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
const TCHAR* XDataAttribute::getTagName()
{
XEXCEPT_ENTRY
	return m_strTagName;
XEXCEPT_EXIT
}
bool XDataAttribute::putValue(XDATA_TYPE datatype, XBIN *pValue, int count)
{
XEXCEPT_ENTRY
	// バッファをリセット
	m_buf.resetp();
	bool b =  XDataWriter::putValue(datatype, pValue, count);
	if( b == true ) {
		m_nSize = count * (1 << (m_nType & 0x03));
		XBIN * p = m_buf.getBuff();
		m_pValue = &p[m_buf.size() - m_nSize];
	} else {
		m_nSize = 0;
		pValue  = NULL;
	}
	return b;
XEXCEPT_EXIT
}








//-------------------------------------------------------------------------------------
//
//	XDataAttributeList
//
//-------------------------------------------------------------------------------------
XDataAttributeList::XDataAttributeList()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XDataAttributeList::~XDataAttributeList()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
XDataAttribute* XDataAttributeList::add(XDATA_TYPE type, const TCHAR* pTagName)
{
XEXCEPT_ENTRY
	XDataAttribute *pNew = new XDataAttribute();
	
	if( pNew == NULL ) {
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);	}
		return NULL;
	}
	pNew->m_strTagName	= pTagName;
	pNew->m_nType		= (XDATA_TYPE)type;
	m_attrs.Add( pNew );
	return pNew;
XEXCEPT_EXIT
}
bool XDataAttributeList::remove(const TCHAR* pTagName)
{
XEXCEPT_ENTRY
	for(int i = 0; i < m_attrs.GetSize(); i++) {
		XDataAttribute *p = m_attrs[i];
		if( p->m_strTagName.Compare( pTagName ) == 0 ) {
			delete p;
			m_attrs.RemoveAt( i );
		}		
	}
	if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, (unsigned long)XD2_ERRIT);	}
	return false;
XEXCEPT_EXIT
}
void XDataAttributeList::clear()
{
XEXCEPT_ENTRY
	for(int i = 0; i < m_attrs.GetSize(); i++) {
		XDataAttribute *p = m_attrs[i];
		delete p;
	}
	m_attrs.RemoveAll();
XEXCEPT_EXIT
}
int XDataAttributeList::size()
{
XEXCEPT_ENTRY
	return m_attrs.GetSize();
XEXCEPT_EXIT
}
XDataAttribute* XDataAttributeList::at(int nIndex)
{
XEXCEPT_ENTRY
	return m_attrs[ nIndex ];
XEXCEPT_EXIT
}






//-------------------------------------------------------------------------------------
//
//	XDataXLReader
//
//-------------------------------------------------------------------------------------
XDataXLReader::XDataXLReader(XBIN *pData, XD_UINT4 nSize)
	:	m_reader(pData, nSize)
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XDataXLReader::~XDataXLReader()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
bool XDataXLReader::getValue(XDataXL *pData)
{
XEXCEPT_ENTRY
	XData dataFirst;
	XData dataDummy;
	XData dataTagName;
	XData dataAttribute;
	XData dataData;

	if( m_reader.getValue(&dataFirst) == false ) {
		pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
		return false;
	}
	
	if( dataFirst.getType() == XD2_L ) {
		//
		// get TAG Name
		pData->m_ptagName = m_reader.m_pCurrentData;
		if( m_reader.getValue(&dataTagName) == false ) {
			pData->m_nType =(XDATA_TYPE) XD2_ERRDI;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
			return false;
		}
		if( dataTagName.getType() != XD2_A ) {
			pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
			return false;
		}
		pData->m_ntagNameSize = m_reader.m_pCurrentData - pData->m_ptagName;

		//
		// get Attribte
		pData->m_pAttrList = m_reader.m_pCurrentData;
		if( m_reader.getValue(&dataAttribute) == false ) {
			pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
			return false;
		}
		if( dataAttribute.getType() != XD2_L ) {
			pData->m_nType =(XDATA_TYPE) XD2_ERRTY;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
			return false;
		}
		for(XD_UINT4 i = 0; i < dataAttribute.getSize(); i++) {
			// Skip AttrData 
			if( m_reader.getValue(&dataDummy) == false ) {
				pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
				return false;
			}
			if( dataDummy.getType() != XD2_L ) {
				pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
				return false;
			}
			if( dataDummy.getSize() != 2 ) {
				pData->m_nType = (XDATA_TYPE)XD2_ERRAS;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
				return false;
			}
			{
				XData dataDummy2;
				if( m_reader.getValue(&dataDummy2) == false ) {
					pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
					return false;
				}
				if( dataDummy2.getType() != XD2_A ) {
					pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
					return false;
				}
				if( m_reader.getValue(&dataDummy2) == false ) {
					pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
					return false;
				}
			}
		}
		pData->m_nAttrListSize = m_reader.m_pCurrentData - pData->m_pAttrList;


		pData->m_nType       = (XDATA_TYPE)XD2_L;
		pData->m_nSize       = dataFirst.getSize()-2;
		pData->m_nSizeOfSize = (XD_UINT1)dataFirst.getLengthByte();
	} else 
	if( dataFirst.getType() == XD2_B ) {
		XDataReader reader(dataFirst.m_pValue, dataFirst.getSize());

		if( reader.getValue(&dataDummy) == false ) {
			pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
			return false;
		}
		if( dataDummy.getType() != XD2_L ) {
			pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
			return false;
		}
		if( dataDummy.getSize() != 3 ) {
			pData->m_nType = (XDATA_TYPE)XD2_ERRAS;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
			return false;
		}
		{
			//
			// get TAG Name
			pData->m_ptagName = reader.m_pCurrentData;
			if( reader.getValue(&dataTagName) == false ) {
				pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
				return false;
			}
			if( dataTagName.getType() != XD2_A ) {
				pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
				return false;
			}
			pData->m_ntagNameSize = reader.m_pCurrentData - pData->m_ptagName;

			//
			// get Attribte
			pData->m_pAttrList = reader.m_pCurrentData;
			if( reader.getValue(&dataAttribute) == false ) {
				pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
				return false;
			}
			if( dataAttribute.getType() != XD2_L ) {
				pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
				return false;
			}
			for(XD_UINT4 i = 0; i < dataAttribute.getSize(); i++) {
				// Skip AttrData 
				if( reader.getValue(&dataDummy) == false ) {
					pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
					return false;
				}
				if( dataDummy.getType() != XD2_L ) {
					pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
					return false;
				}
				if( dataDummy.getSize() != 2 ) {
					pData->m_nType = (XDATA_TYPE)XD2_ERRAS;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
					return false;
				}
				{
					XData dataDummy2;
					if( reader.getValue(&dataDummy2) == false ) {
						pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
						if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
						return false;
					}
					if( dataDummy2.getType() != XD2_A ) {
						pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
						if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
						return false;
					}
					if( reader.getValue(&dataDummy2) == false ) {
						pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
						if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
						return false;
					}
				}
			}
			pData->m_nAttrListSize = reader.m_pCurrentData - pData->m_pAttrList;

			//
			// get Data
			if( reader.getValue(&dataData) == false ) {
				pData->m_nType = (XDATA_TYPE)XD2_ERRDI;
				if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
				return false;
			}
			pData->m_nType       = dataData.getType();
			pData->m_nSize       = dataData.getSize();
			pData->m_nSizeOfSize = dataData.getLengthByte();
			pData->m_pValue      = dataData.m_pValue;
		}
	} else {
		pData->m_nType = (XDATA_TYPE)XD2_ERRTY;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, pData->m_nType);	}
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XDataXLReader::print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XStrBuff brank;
	return printInner(stream, nDepth, brank, pLF);
XEXCEPT_EXIT
}
bool XDataXLReader::printInner(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth,  XStrBuff &brank, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XDataXL data;
	TCHAR *pBlak = NULL;


	if( brank.getCountOfBuff() < nDepth+10 ) {
		pBlak = (TCHAR*)brank.Alloc( (nDepth+60) * sizeof(TCHAR) );
	} else {
		pBlak = (TCHAR*)brank;
	}
	if( pBlak == NULL ) {
		return false;
	}
	pBlak = (TCHAR*)brank;
	brank.memsetT(_T(' '));
	pBlak[nDepth] = 0;

	do {
		if( this->getValue(&data) == false ) {
			break;
		}
		stream << (const TCHAR*)brank << _T("<") << (const TCHAR*)data.getTagName();
		stream << _T(" type=\"") << XData::getDataTypeString_From_TYPE(data.getType()) << _T("\"");

		stream << _T(" size=\"");
		stream << data.getSize();
		stream << _T("\"");
		stream << _T(" sizesize=\"");
		stream << (int)data.getLengthByte();
		stream << _T("\"");
		stream << _T(" count=\"");
		stream << data.getCount();
		stream << _T("\"");



		// print attribute
		
		stream << _T(">");


		if( data.getType() == XD2_L ) {
			stream << pLF;
			for( XD_UINT4 i = 0; i < data.getSize(); i++) {
				printInner(stream, nDepth+1, brank, pLF);
			}

			pBlak = (TCHAR*)brank;
			brank.memsetT(_T(' '));
			pBlak[nDepth] = 0;


			stream << (const TCHAR*)brank << _T("</") << (const TCHAR*)data.getTagName() << _T(">");
			stream << pLF;
			return true;
		} else 
		if( data.getType() == XD2_A ) {
			stream << (const TCHAR*)data.getValAsStringA();
		} else 
		if( data.getType() == XD2_J ) {
			stream << (const TCHAR*)data.getValAsStringJ();
		} else 
		if( data.getType() == XD2_UTF16 ) {
			stream << (const TCHAR*)data.getValAsStringUTF16();
		} else 
		if( data.getType() == XD2_BOOL ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << _T(" ");	}
				if( data.getValAsBool(i) == true ) {
					stream << _T("true");
				} else {
					stream << _T("false");
				}
			}
		} else 
		if( data.getType() == XD2_B ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				TCHAR sz[5] = {0};
				if( i != 0 ) {	stream << " ";	}
				XStrBuff::printf(sz, XCOUNTOF(sz), _T("0x%02x"), data.getValAsUInt1(i) );
				stream << sz ;
			}
		} else 
		if( data.getType() == XD2_I1 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				stream << (int)data.getValAsInt1(i);
			}
		} else 
		if( data.getType() == XD2_I2 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				stream << data.getValAsInt2(i);
			}
		} else 
		if( data.getType() == XD2_I4 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				stream << data.getValAsInt4(i);
			}
		} else 
		if( data.getType() == XD2_I8 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				TCHAR sz[256] = {0};
				XStrBuff::printf(sz, XCOUNTOF(sz), _T("%I64d"), data.getValAsInt8(i));
				stream << sz;
			}
		} else 
		if( data.getType() == XD2_U1 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				stream << (unsigned int)data.getValAsUInt1(i);
			}
		} else 
		if( data.getType() == XD2_U2 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				stream << data.getValAsUInt2(i);
			}
		} else 
		if( data.getType() == XD2_U4 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				stream << data.getValAsUInt4(i);
			}
		} else 
		if( data.getType() == XD2_U8 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				TCHAR sz[256] = {0};
				XStrBuff::printf(sz, XCOUNTOF(sz), _T("%I64u"), data.getValAsUInt8(i));
				stream << sz;
			}
		} else 
		if( data.getType() == XD2_F4 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				stream << data.getValAsFloat4();
			}
		} else 
		if( data.getType() == XD2_F8 ) {
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( i != 0 ) {	stream << " ";	}
				stream << data.getValAsFloat8();
			}
		}
		stream << "</" << (const TCHAR*)data.getTagName() << _T(">");
		stream << pLF;
		return true;
	} while(1);
	return true;
XEXCEPT_EXIT
}




//-------------------------------------------------------------------------------------
//
//	XDataXLWriter
//
//-------------------------------------------------------------------------------------
XDataXLWriter::XDataXLWriter(std::ostream *stream)
	:	m_writer(stream)
{
XEXCEPT_ENTRY


XEXCEPT_EXIT
}
XDataXLWriter::~XDataXLWriter()
{
XEXCEPT_ENTRY


XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsList(const char* pTagName, int nListCout, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	if( m_writer.putValAsList( nListCout + 2 ) == false ) {	return false; }
	if( m_writer.putValAsStringA(pTagName) == false ) {	return false;	}
	if( putAttributes(m_writer, pAttrList) == false ) {	return false;	}	
	return true;
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsBool(const char* pTagName, const bool value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_BOOL, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsBinary(const char* pTagName, const unsigned char *pValue, int nSize, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_B, (unsigned char*)pValue, nSize, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsStringA(const char* pTagName, const char *pValue, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_A, (unsigned char*)pValue, ::strlen(pValue), pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsStringW(const char* pTagName, const wchar_t *pValue, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_UTF16, (unsigned char*)pValue, ::wcslen(pValue), pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsStringJ(const char* pTagName, const char *pValue, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_J, (unsigned char*)pValue, ::strlen(pValue), pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsInt1(const char* pTagName, const char value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_I1, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsInt2(const char* pTagName, const XD_INT2 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_I2, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsInt4(const char* pTagName, const XD_INT4 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_I4, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsInt8(const char* pTagName, const XD_INT8 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_I8, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsUInt1(const char* pTagName, const XD_UINT1 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_U1, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsUInt2(const char* pTagName, const XD_UINT2 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_U2, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsUInt4(const char* pTagName, const XD_UINT4 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_U4, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsUInt8(const char* pTagName, const XD_UINT8 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_U8, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsFloat4(const char* pTagName, const XD_FLOAT4 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_F4, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsFloat8(const char* pTagName, const XD_FLOAT8 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_F8, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValue(const char* pTagName, XDATA_TYPE type, void *pValue, int count, XDataAttributeList *pAttrList /*= NULL*/)
{
XEXCEPT_ENTRY

	XInflateStreamBuff		buf(0xFF, 0xFF);
	std::ostream			stream( &buf );
	XDataWriter				writer( &stream );
	if( writer.putValAsList( 3 ) == false ) {	return false; }
	if( writer.putValAsStringA( pTagName ) == false ) {	return false; }
	if( putAttributes(writer, pAttrList) == false ) {	return false; }	
	if( writer.putValue(type, pValue, count ) == false ) {	return false; }
	if( m_writer.putValAsBinary(buf.getBuff(), buf.size() ) == false ) {	return false; }
/*
	if( m_writer.putValAsList( 3 ) == false ) {	return false; }
	if( m_writer.putValAsStringA( pTagName ) == false ) {	return false; }
	if( putAttributes(m_writer, pAttrList) == false ) {	return false; }	
	if( m_writer.putValAsValue(type, pValue, count ) == false ) {	return false; }
*/	
	return true;
XEXCEPT_EXIT
}

bool XDataXLWriter::putAttributes(XDataWriter &writer, XDataAttributeList *pAttrList)
{
XEXCEPT_ENTRY
	if( pAttrList ) {
		if( writer.putValAsList( pAttrList->size() ) == false ) {	return false; }
		for(int i = 0; i < pAttrList->size(); i++) {
			XDataAttribute *pAttr = pAttrList->at( i );
			if( writer.putValAsList( 2 ) == false ) {	return false; }
			if( writer.putValAsStringA( pAttr->getTagName() ) == false ) {	return false; }
			if( writer.putValue( pAttr->getType(), pAttr->m_pValue, pAttr->getCount() ) == false ) {	return false; }
		}
	} else {
		if( writer.putValAsList( 0 ) == false ) {	return false;	}
	}
	return true;
XEXCEPT_EXIT
}

bool XDataXLWriter::putValAsList(const wchar_t* pTagName, int nListCout, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	if( m_writer.putValAsList( nListCout + 2 ) == false ) {	return false; }
	if( m_writer.putValAsStringA(  pTagName  ) == false ) {	return false;	}
	if( putAttributes(m_writer, pAttrList) == false ) {	return false;	}	
	return true;
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsBool(const wchar_t* pTagName, const bool value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_BOOL, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsBinary(const wchar_t* pTagName, const unsigned char *pValue, int nSize, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_B, (unsigned char*)pValue, nSize, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsStringA(const wchar_t* pTagName, const char *pValue, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_A, (unsigned char*)pValue, ::strlen(pValue), pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsStringW(const wchar_t* pTagName, const wchar_t *pValue, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_UTF16, (unsigned char*)pValue, ::wcslen(pValue), pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsStringJ(const wchar_t* pTagName, const char *pValue, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_J, (unsigned char*)pValue, ::strlen(pValue), pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsInt1(const wchar_t* pTagName, const char value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_I1, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsInt2(const wchar_t* pTagName, const XD_INT2 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_I2, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsInt4(const wchar_t* pTagName, const XD_INT4 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_I4, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsInt8(const wchar_t* pTagName, const XD_INT8 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_I8, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsUInt1(const wchar_t* pTagName, const XD_UINT1 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_U1, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsUInt2(const wchar_t* pTagName, const XD_UINT2 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_U2, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsUInt4(const wchar_t* pTagName, const XD_UINT4 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_U4, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsUInt8(const wchar_t* pTagName, const XD_UINT8 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_U8, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsFloat4(const wchar_t* pTagName, const XD_FLOAT4 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_F4, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValAsFloat8(const wchar_t* pTagName, const XD_FLOAT8 value, XDataAttributeList *pAttrList/* = NULL*/)
{
XEXCEPT_ENTRY
	return putValue(pTagName, XDATA_TYPE_F8, (unsigned char*)&value, 1, pAttrList);
XEXCEPT_EXIT
}
bool XDataXLWriter::putValue(const wchar_t* pTagName, XDATA_TYPE type, void *pValue, int count, XDataAttributeList *pAttrList /*= NULL*/)
{
XEXCEPT_ENTRY

	XInflateStreamBuff		buf(0xFF, 0xFF);
	std::ostream			stream( &buf );
	XDataWriter				writer( &stream );
	if( writer.putValAsList( 3 ) == false ) {	return false; }
	if( writer.putValAsStringA( pTagName ) == false ) {	return false; }
	if( putAttributes(writer, pAttrList) == false ) {	return false; }	
	if( writer.putValue(type, pValue, count ) == false ) {	return false; }
	if( m_writer.putValAsBinary(buf.getBuff(), buf.size() ) == false ) {	return false; }

	return true;
XEXCEPT_EXIT
}



