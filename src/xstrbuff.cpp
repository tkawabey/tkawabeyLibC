#ifdef _UNICODE
  #ifndef UNICODE
    #define UNICODE 1
  #endif
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#ifdef WIN32
	#include <windows.h>
	#include <winnls.h>
	#include <tchar.h>
 #else
	#include <wchar.h>
    #include <wctype.h>
#endif


#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"

#pragma warning( disable: 4307)
#pragma warning( disable: 4146)
#pragma warning( disable: 4293)
#pragma warning( disable: 4244)

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// XStrBuff
//
XStrBuff::XStrBuff(size_t iInflateSize /*= 256*/)
	:	m_iInflateSize(iInflateSize)
{
XEXCEPT_ENTRY
	m_pStrAddress = NULL;
	this->set(_T(""), -1);
XEXCEPT_EXIT
}
XStrBuff::XStrBuff(const char* lpsz, size_t size /*= -1*/, size_t iInflateSize /*= 256*/)
	:	m_iInflateSize(iInflateSize)
{
XEXCEPT_ENTRY
	m_pStrAddress = NULL;
	this->set(lpsz, size);
XEXCEPT_EXIT
}
XStrBuff::XStrBuff(const wchar_t* lpsz, size_t size /*= -1*/, size_t iInflateSize /*= 256*/)
	:	m_iInflateSize(iInflateSize)
{
XEXCEPT_ENTRY
	m_pStrAddress = NULL;
	this->set(lpsz, size);
XEXCEPT_EXIT
}
XStrBuff::XStrBuff(const XStrBuff& src, size_t iInflateSize /*= 256*/)
	:	m_iInflateSize(iInflateSize)
{
XEXCEPT_ENTRY
	m_pStrAddress = NULL;
	*this = (const TCHAR*)src.m_pStrAddress;
XEXCEPT_EXIT
}



/**	@brief	余分バッファの増加サイズを設定
	*	@param	iInflateSize	余分バッファの増加サイズ
	*/
bool XStrBuff::setInflateSize(size_t nInflateSize)
{
XEXCEPT_ENTRY
	if( nInflateSize == 0 ) {
		XEXCEPT_THROW(XExcept_UNKNOWN_PARAM, 0 , 0);
	}
	m_iInflateSize = nInflateSize;
XEXCEPT_EXIT
	return true;
}


/** @copycoc XBuff::Alloc(size_t) */
void *XStrBuff::Alloc(size_t nSize)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XStrBuff::Alloc"

	___XEXCEPT_CHECK_VALID_TRUE(nSize != 0);


	m_pStrAddress = (TCHAR*)XBuff::Alloc(nSize);
	return m_pStrAddress;
XEXCEPT_EXIT
}
/** @copycoc XBuff::ReAlloc(size_t) */
void *XStrBuff::ReAlloc(size_t nSize)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XStrBuff::ReAlloc"

	___XEXCEPT_CHECK_VALID_TRUE(nSize != 0);


	m_pStrAddress = (TCHAR*)XBuff::ReAlloc(nSize);
	return m_pStrAddress;
XEXCEPT_EXIT
}
/** @copycoc XBuff::Free() */
void XStrBuff::Free()
{
XEXCEPT_ENTRY
	XBuff::Free();
	m_pStrAddress = 0;
XEXCEPT_EXIT
}
XStrBuff::~XStrBuff()
{
XEXCEPT_ENTRY
	Free();
XEXCEPT_EXIT
}


const TCHAR* XStrBuff::c_str() const
{
XEXCEPT_ENTRY
	return m_pStrAddress;
XEXCEPT_EXIT
}

XStrBuff& XStrBuff::set(const char* lpsz, int size /*= -1*/)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XStrBuff::set"

	___XEXCEPT_CHECK_VALID_TRUE(lpsz != 0);

	if( size == -1 ) {
		*this = lpsz;
	} else {
		
		___XEXCEPT_CHECK_VALID_TRUE(size != 0);


		size_t reqSize = 0;
#ifdef _UNICODE
		reqSize = ( size + 1 ) * sizeof(wchar_t);
		if( reqSize > this->GetBuffSize() ) {
			Free();
			// allocate memory
			m_pStrAddress = (wchar_t*)this->Alloc( reqSize );	
		}
		this->MemSet( 0 );
		_XASSERT( m_pStrAddress );	
		if( lpsz == NULL ) {
			XStrBuff::__strcpyW(m_pStrAddress, this->getCountOfBuff(), L"");
		} else {
			XStrBuff::__mbstowcs(m_pStrAddress, this->getCountOfBuff(), lpsz, size);
		}
#else
		reqSize = ( size + 1 ) * sizeof(char);
		if( reqSize > this->GetBuffSize() ) {
			Free();
			// allocate memory
			m_pStrAddress = (char*)this->Alloc( reqSize );	
		}
		this->MemSet( 0 );
		_XASSERT( m_pStrAddress );	
		if( lpsz == NULL ) {
			XStrBuff::__strcpyA(m_pStrAddress,  this->GetBuffSize(), "");
		} else {
			XStrBuff::__strncpyA(m_pStrAddress,  this->GetBuffSize(), lpsz, size);
		}
#endif
	}
XEXCEPT_EXIT
	return *this;
}

XStrBuff& XStrBuff::set(const wchar_t* lpsz, int size /*= -1*/)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XStrBuff::set"

	___XEXCEPT_CHECK_VALID_TRUE(lpsz != 0);


	if( size == -1 ) {
		*this = lpsz;
	} else {		
		___XEXCEPT_CHECK_VALID_TRUE(size != 0);
		size_t reqSize = 0;
		reqSize = ( size + 1 ) * sizeof(TCHAR);
		if( reqSize > this->GetBuffSize() ) {
			Free();
			// allocate memory
			m_pStrAddress = (TCHAR*)this->Alloc( reqSize );	
		}
		_XASSERT( m_pStrAddress );
		this->MemSet( 0 );
#ifdef _UNICODE
		XStrBuff::__strncpyW(m_pStrAddress, this->getCountOfBuff()
			, lpsz == NULL ? L"" : lpsz
			, lpsz == NULL ? 0   : size);
#else
		if( lpsz == NULL ) {
			XStrBuff::__strcpyA(m_pStrAddress, this->getCountOfBuff(), "");
		} else {
			XStrBuff::__wcstombs(m_pStrAddress, this->getCountOfBuff(), lpsz, this->getCountOfBuff());
		}
#endif
	}
XEXCEPT_EXIT
	return *this;
}

const XStrBuff& XStrBuff::operator=(const char* lpsz)
{
XEXCEPT_ENTRY
	size_t i = 0;

	if( lpsz == NULL ) {
		if( m_pStrAddress != 0 ) {
			*m_pStrAddress = 0;
		}
		return *this;
	}
	
	i = ::strlen(lpsz) + 1;

#ifdef _UNICODE
	// clac Buff size
	i *= sizeof(wchar_t);
	if( i > this->GetBuffSize() ) {
		Free();
		// allocate memory
		m_pStrAddress = (wchar_t*)this->Alloc( i );	
	}
	_XASSERT( m_pStrAddress );
	if( lpsz == NULL ) {
		XStrBuff::__strcpyW(m_pStrAddress, this->getCountOfBuff(), L"");
	} else {
		XStrBuff::__mbstowcs(m_pStrAddress, this->getCountOfBuff(), lpsz, i);
	}
#else
	// clac Buff size
	i *= sizeof(char);
	if( i > this->GetBuffSize() ) {
		Free();
		// allocate memory
		m_pStrAddress = (char*)this->Alloc( i );
	}
	_XASSERT( m_pStrAddress );	
	if( lpsz == NULL ) {
		XStrBuff::__strcpyA(m_pStrAddress, this->GetBuffSize(), "");
	} else {
		XStrBuff::__strcpyA(m_pStrAddress, this->GetBuffSize(), lpsz);
	}
#endif	// _UNICODE
XEXCEPT_EXIT
	return *this;
}
const XStrBuff& XStrBuff::operator=(const wchar_t* lpsz)
{
XEXCEPT_ENTRY
	size_t nByteSize = 0;

	if( lpsz == NULL ) {
		if( m_pStrAddress != 0 ) {
			*m_pStrAddress = 0;
		}
		return *this;
	}
	// 必要なバッファサイズ
#ifdef _UNICODE
	nByteSize = ::wcslen(lpsz) + 1;	// Wide文字は、UNICODE＝１、MutileByte=2なので、とりあえずかけ２の値を確保する。
	nByteSize *= sizeof(TCHAR);
#else
	nByteSize = (::wcslen(lpsz)*2) + 1;	// Wide文字は、UNICODE＝１、MutileByte=2なので、とりあえずかけ２の値を確保する。
	nByteSize *= sizeof(TCHAR);
#endif	// _UNICODE
	
	if( nByteSize > this->GetBuffSize() ) {
		Free();
		// allocate memory
		m_pStrAddress = (TCHAR*)this->Alloc( nByteSize );
	}
	_XASSERT( m_pStrAddress );	
#ifdef _UNICODE
	// clac Buff size
	XStrBuff::__strcpyW(m_pStrAddress, this->getCountOfBuff()
		, lpsz == NULL ? L"" : lpsz);

#else
	XStrBuff::__wcstombs(m_pStrAddress, this->getCountOfBuff()
		, lpsz
		, this->getCountOfBuff()-1	/*m_pStrAddressバッファーに格納するワイド文字の最大数 (終端の null 文字は含まない)*/
	);
#endif	// _UNICODE

XEXCEPT_EXIT
	return *this;
}
const XStrBuff& XStrBuff::operator=(const XStrBuff& stringSrc)
{
XEXCEPT_ENTRY
	*this = stringSrc.m_pStrAddress;
XEXCEPT_EXIT
	return *this;
}
XStrBuff::operator const char*()
{
XEXCEPT_ENTRY
	char *pData = *this;
	return pData;
XEXCEPT_EXIT
}
XStrBuff::operator char*()
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	size_t  iSize = 0;
	char *pTemp = NULL;
	if( m_pStrAddress == NULL ) {
		return NULL;
	}
	
	iSize = ::wcslen(m_pStrAddress) + 1;
	iSize *= sizeof(wchar_t);

	if( m_ConvertBuff.GetBuffSize() < iSize ) {
		pTemp = (char*)m_ConvertBuff.Alloc( iSize );
	} else {
		pTemp = m_ConvertBuff;
	}
	XStrBuff::__wcstombs(pTemp, m_ConvertBuff.GetBuffSize() , m_pStrAddress
		, m_ConvertBuff.GetBuffSize() - 1 /*pTempバッファーに格納するワイド文字の最大数 (終端の null 文字は含まない)*/
	);
	return pTemp;
#else	
	return m_pStrAddress;
#endif	// _UNICODE
XEXCEPT_EXIT
}
XStrBuff::operator const wchar_t*()
{
XEXCEPT_ENTRY
	wchar_t *pData = *this;
	return pData;
XEXCEPT_EXIT
}
XStrBuff::operator wchar_t*()
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return m_pStrAddress;
#else	
	size_t  nArraySize = 0;
	size_t  nBuffSize = 0;
	wchar_t *pTemp = NULL;
	if( m_pStrAddress == NULL ) {
		return NULL;
	}
	
	nArraySize = ::strlen(m_pStrAddress) + 1;
	nBuffSize = nArraySize * sizeof(wchar_t);

	if( m_ConvertBuff.GetBuffSize() < nBuffSize ) {
		pTemp = (wchar_t*)m_ConvertBuff.Alloc( nBuffSize );
	} else {
		pTemp = (wchar_t*)(unsigned short*)m_ConvertBuff;
	}

	XStrBuff::__mbstowcs(pTemp, nArraySize, m_pStrAddress, ::strlen(m_pStrAddress) );
	return pTemp;
#endif	// _UNICODE
XEXCEPT_EXIT
}
const bool XStrBuff::operator==(const TCHAR* lpsz)  const
{
XEXCEPT_ENTRY
	return XStrBuff::StrCmp(m_pStrAddress, lpsz) == 0 ? true : false;
XEXCEPT_EXIT
}

const bool XStrBuff::operator==(const XStrBuff& right) const
{
XEXCEPT_ENTRY
	return XStrBuff::StrCmp(m_pStrAddress, right.c_str()) == 0 ? true : false;
XEXCEPT_EXIT
}



const bool XStrBuff::operator!=(const TCHAR* lpsz)  const
{
XEXCEPT_ENTRY
	return XStrBuff::StrCmp(m_pStrAddress, lpsz) != 0 ? true : false;
XEXCEPT_EXIT
}
TCHAR& XStrBuff::operator[](int nIndex)
{
XEXCEPT_ENTRY
	return this->GetAt(nIndex);
XEXCEPT_EXIT
}
const XStrBuff& XStrBuff::operator+=(const char* lpsz)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	XStrBuff strTmp(lpsz);
	*this += (const wchar_t*)strTmp;
	
#else
	size_t nBuffSize = 0;
	char *pTemp = NULL;

	if( lpsz == NULL ) {
		return *this;
	}
	
	nBuffSize  = this->getLengthOfByteSize();
	nBuffSize += XStrBuff::StrLenOfByteSize(lpsz);
	nBuffSize += sizeof(char);
	if( m_pStrAddress != NULL ) {
		if( nBuffSize <= this->GetBuffSize() ) {
			XStrBuff::StrCat(m_pStrAddress, this->getCountOfBuff(), lpsz);
			return *this;
		}
		pTemp = new char [XStrBuff::StrLen(m_pStrAddress) + 1];
		_XASSERT(pTemp);
		XStrBuff::StrCpy(pTemp, XStrBuff::StrLen(m_pStrAddress) + 1, m_pStrAddress);
	}
	if( nBuffSize > this->GetBuffSize() ) {

		nBuffSize += m_iInflateSize;	// 余分バッファのサイズを追加

		Free();
		// allocate memory
		m_pStrAddress = (char*)this->Alloc( nBuffSize );	
	}
	_XASSERT( m_pStrAddress );	
	if( pTemp != NULL ){
		XStrBuff::StrCpy(m_pStrAddress, this->getCountOfBuff(), pTemp);
		delete []pTemp;
		pTemp = m_pStrAddress;
		pTemp += XStrBuff::StrLen(m_pStrAddress);
		XStrBuff::StrCpy(pTemp, this->getCountOfRestBuff(), lpsz);
		pTemp = NULL;
	}else{
		XStrBuff::StrCpy(m_pStrAddress, this->getCountOfBuff(), lpsz);
	}
#endif
XEXCEPT_EXIT
	return *this;
}
const XStrBuff& XStrBuff::operator+=(const wchar_t* lpsz)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	size_t nBuffSize = 0;
	wchar_t *pTemp = NULL;

	if( lpsz == NULL ) {
		return *this;
	}

	// 必要なバッファサイズ。
	nBuffSize  = this->getLengthOfByteSize();
	nBuffSize += XStrBuff::StrLenOfByteSize(lpsz);
	nBuffSize += sizeof(wchar_t);
	
	if( m_pStrAddress != NULL ) {
		if( nBuffSize <= this->GetBuffSize() ) {
			XStrBuff::StrCat(m_pStrAddress, this->getCountOfBuff(), lpsz);
			return *this;
		}
		pTemp = new wchar_t [XStrBuff::StrLen(m_pStrAddress) + 1];
		_XASSERT(pTemp);
		XStrBuff::StrCpy(pTemp, XStrBuff::StrLen(m_pStrAddress) + 1, m_pStrAddress);
	}
	if( nBuffSize > this->GetBuffSize() ) {
		nBuffSize += m_iInflateSize;	// 余分バッファのサイズを追加
		Free();
		// allocate memory
		m_pStrAddress = (wchar_t*)this->Alloc( nBuffSize );	
	}
	_XASSERT( m_pStrAddress );	
	if( pTemp != NULL ){
		XStrBuff::StrCpy(m_pStrAddress, this->getCountOfBuff(), pTemp);
		delete []pTemp;
		pTemp = m_pStrAddress;
		pTemp += XStrBuff::StrLen(m_pStrAddress);
		XStrBuff::StrCpy(pTemp, this->getCountOfRestBuff(), lpsz);
		pTemp = NULL;
	}else{
		XStrBuff::StrCpy(m_pStrAddress, this->getCountOfRestBuff(), lpsz);
	}
#else
	XStrBuff strTmp(lpsz);
	*this += (const char*)strTmp;
#endif
XEXCEPT_EXIT
	return *this;
}
const XStrBuff& XStrBuff::operator+=(XStrBuff& val)
{
XEXCEPT_ENTRY
	*this += (const TCHAR*)val;
XEXCEPT_EXIT
	return *this;
}

XStrBuff& XStrBuff::operator<<(short val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[64];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%d"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(unsigned short val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[64];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff)-1, _T("%u"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(int val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[64];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%d"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(unsigned int val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[64];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%u"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(long val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[64];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%d"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(unsigned long val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[64];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%u"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(XD_INT8 val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[128];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%I64d"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(XD_UINT8 val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[128];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%I64u"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(float val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[64];
	::memset(szBuff, 0, sizeof(szBuff));
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%e"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(double val)
{
XEXCEPT_ENTRY
	TCHAR	szBuff[64];
	XStrBuff::printf(szBuff, XCOUNTOF(szBuff), _T("%e"), val);
	*this += szBuff;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(const char* val)
{
XEXCEPT_ENTRY
	*this += val;
XEXCEPT_EXIT
	return *this;
}
XStrBuff& XStrBuff::operator<<(const wchar_t* val)
{
XEXCEPT_ENTRY
	*this += val;
XEXCEPT_EXIT
	return *this;
}

XStrBuff& XStrBuff::operator<<(XStrBuff& val)
{
XEXCEPT_ENTRY
	*this += val;
XEXCEPT_EXIT
	return *this;
}


XStrBuff& XStrBuff::appendChar(char c)
{
XEXCEPT_ENTRY
	char sz[2] = {c, 0};
	*this += sz;
XEXCEPT_EXIT
	return *this;
}

XStrBuff& XStrBuff::appendChar(wchar_t c)
{
XEXCEPT_ENTRY
	wchar_t sz[2] = {c, 0};
	*this += sz;
XEXCEPT_EXIT
	return *this;
}



bool XStrBuff::operator < (const XStrBuff& rhight) const
{
XEXCEPT_ENTRY
	return XStrBuff::StrCmp(this->c_str(), rhight.c_str()) < 0;
XEXCEPT_EXIT
	return false;
}


size_t XStrBuff::GetLength() const
{
XEXCEPT_ENTRY
	return XStrBuff::StrLen(m_pStrAddress);
XEXCEPT_EXIT
}
TCHAR& XStrBuff::GetAt(size_t nIndex)
{
XEXCEPT_ENTRY
	if( !(nIndex < this->getLength() ) ) {
		XEXCEPT_THROW(XExcept_OVER_INDEX, 0, 0);
	}
	_XASSERT(nIndex >= 0);
	_XASSERT(nIndex < (int)XStrBuff::StrLen(m_pStrAddress));
	return m_pStrAddress[nIndex];
XEXCEPT_EXIT
}

void XStrBuff::SetAt(size_t nIndex, TCHAR ch)
{
XEXCEPT_ENTRY
	if( !(nIndex < this->getLength() ) ) {
		XEXCEPT_THROW(XExcept_OVER_INDEX, 0, 0);
	}
	_XASSERT(nIndex >= 0);
	_XASSERT(nIndex < (int)XStrBuff::StrLen(m_pStrAddress));
	m_pStrAddress[nIndex] = ch;
XEXCEPT_EXIT
}
void XStrBuff::FormatV(const TCHAR* lpszFormat, va_list argList)
{
XEXCEPT_ENTRY
	int nMaxLen = 0;
	const TCHAR* lpsz = lpszFormat;
	va_list argListSave = argList;
	enum FormatVender
	{
		FORCE_ANSI      = 0x10000,
		FORCE_UNICODE   = 0x20000,
		FORCE_INT64     = 0x40000
	};


	// 文字の拡張後の推定MAX文字列を求める
	for(;*lpsz != '\0'; lpsz = XStrBuff::StrInc(lpsz)) {

		//_tcsinc
		if (*lpsz != '%' || *(lpsz = XStrBuff::StrInc(lpsz)) == '%') {
			nMaxLen += XStrBuff::StrLen(lpsz);
			continue;
		}
		int nItemLen = 0;
		int nWidth = 0;
		for(; *lpsz != '\0'; lpsz = XStrBuff::StrInc(lpsz)) {
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' || *lpsz == ' ')
				;
			else
				break;
		}
		if (nWidth == 0) {
			nWidth = XStrBuff::AtoI(lpsz);
			for (; *lpsz != '\0' && XStrBuff::isDigit(*lpsz); lpsz = XStrBuff::StrInc(lpsz))
				;
		}


		int nPrecision = 0;
		if (*lpsz == '.') {
			lpsz = XStrBuff::StrInc(lpsz);

			if (*lpsz == '*') {
				nPrecision = va_arg(argList, int);
				lpsz = XStrBuff::StrInc(lpsz);
			} else {
				nPrecision = XStrBuff::AtoI(lpsz);
				for (; *lpsz != '\0' && XStrBuff::isDigit(*lpsz); lpsz = XStrBuff::StrInc(lpsz))
					;
			}
		}

		int nModifier = 0;
		if ( XStrBuff::StrNCmp(lpsz, _T("I64"), 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
			_XASSERT(0);
#endif
		} else {
			switch (*lpsz) {
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = XStrBuff::StrInc(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = XStrBuff::StrInc(lpsz);
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = XStrBuff::StrInc(lpsz);
				break;
			}
		}

		switch (*lpsz | nModifier)
		{
		// 1文字
		case 'c':
		case 'C':
			nItemLen = 2;
#ifdef WINDOWS
			va_arg(argList, TCHAR);
#else
			va_arg(argList, int);
#endif
			break;
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
#ifdef WINDOWS
			va_arg(argList, char);
#else
			va_arg(argList, int);
#endif
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:
			nItemLen = 2;
#ifdef WINDOWS
			va_arg(argList, WCHAR);
#else
			va_arg(argList, int);
#endif
			break;

		// 文字列
		case 's':
			{
				const TCHAR* pstrNextArg = va_arg(argList, const TCHAR*);
				if(pstrNextArg == NULL) {
				   nItemLen = 6;  // "(null)"
				} else {
				   nItemLen = XStrBuff::StrLen(pstrNextArg);
				   nItemLen = __max(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
#ifndef _UNICODE
				wchar_t* pstrNextArg = va_arg(argList, wchar_t*);
				if (pstrNextArg == NULL) {
				   nItemLen = 6;  // "(null)"
				} else {
				   nItemLen = ::wcslen(pstrNextArg);
				   nItemLen =__max(1, nItemLen);
				}
#else
				const char* pstrNextArg = va_arg(argList, const char*);
				if (pstrNextArg == NULL) {
				   nItemLen = 6; // "(null)"
				} else {
					nItemLen = ::strlen(pstrNextArg);
				   nItemLen = __max(1, nItemLen);
				}
#endif
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				const char*  pstrNextArg = va_arg(argList, const char*);
				if (pstrNextArg == NULL) {
				   nItemLen = 6; // "(null)"
				} else {
					nItemLen = ::strlen(pstrNextArg);
				   nItemLen = __max(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{
				wchar_t* pstrNextArg = va_arg(argList, wchar_t*);
				if (pstrNextArg == NULL) {
				   nItemLen = 6; // "(null)"
				} else {
					nItemLen = ::wcslen(pstrNextArg);
				   nItemLen = __max(1, nItemLen);
				}
			}
			break;
		}

		if (nItemLen != 0)
		{
			if (nPrecision != 0) {
				nItemLen = __min(nItemLen, nPrecision);
			}
			nItemLen = __max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// 数字
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
#ifdef WINDOWS
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
#endif /* WINDOWS */
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = __max(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 128;
				nItemLen = __max(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				{
					double f;
					XStrBuff   tmpBuff;
					TCHAR* pszTemp;
					pszTemp = (TCHAR*)tmpBuff.Alloc(__max(nWidth, 312+nPrecision+6));
					f = va_arg(argList, double);
#ifdef _UNICODE
					::XStrBuff::__swprintf(pszTemp, tmpBuff.getCountOfBuff(), L"%*.*f", nWidth, nPrecision+6, f );
#else
					::XStrBuff::__sprintf(pszTemp, tmpBuff.getCountOfBuff(), "%*.*f", nWidth, nPrecision+6, f );
#endif
					nItemLen = XStrBuff::StrLen(pszTemp);
				}
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = __max(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				_XASSERT(0);  // unknown formatting option
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	nMaxLen++;
	TCHAR* lpszDest = NULL;
	if( nMaxLen * sizeof(TCHAR) < this->m_nBuffSize ) {
		lpszDest = *this;
	} else {
		lpszDest = (TCHAR*)this->Alloc(nMaxLen * sizeof(TCHAR));
	}
	
	XStrBuff::VsPrintf(lpszDest, this->getCountOfBuff(), lpszFormat, argListSave);
	va_end(argListSave);
XEXCEPT_EXIT
}
void XStrBuff::Format(const TCHAR* lpszFormat, ...)
{
XEXCEPT_ENTRY
	va_list argList;
	va_start(argList, lpszFormat);
	XStrBuff::FormatV(lpszFormat, argList);
	va_end(argList);
XEXCEPT_EXIT
}
XStrBuff XStrBuff::FormatS(const TCHAR* lpszFormat, va_list argList)
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	str.FormatV(lpszFormat, argList);
	return str;
XEXCEPT_EXIT
}
XStrBuff XStrBuff::FormatS(const TCHAR* lpszFormat, ...)
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	va_list argList;
	va_start(argList, lpszFormat);
	str.FormatV(lpszFormat, argList);
	va_end(argList);
	return str;
XEXCEPT_EXIT
}
int XStrBuff::Compare(const TCHAR* lpsz) const
{
XEXCEPT_ENTRY
	return XStrBuff::StrCmp(m_pStrAddress, lpsz);
XEXCEPT_EXIT
}
int XStrBuff::CompareNoCase(const TCHAR* lpsz) const
{
XEXCEPT_ENTRY
	return XStrBuff::StrICmp(m_pStrAddress, lpsz);
XEXCEPT_EXIT
}
void XStrBuff::MakeUpper()
{
XEXCEPT_ENTRY
	this->StrUpr(m_pStrAddress);
XEXCEPT_EXIT
}
void XStrBuff::MakeLower()
{
XEXCEPT_ENTRY
	this->StrLwr(m_pStrAddress);
XEXCEPT_EXIT
}
const TCHAR* XStrBuff::Trim(TrimMode mode /*= TRIM_BOTH*/)
{
XEXCEPT_ENTRY
	int		i, len;
	TCHAR*  trim_str = _T(" \t\r\n\"");
	TCHAR*  lpszString;

	if( m_pStrAddress == NULL ) {
		return NULL;
	}

	lpszString = m_pStrAddress;

	if (mode & TRIM_LEFTONLY ) {
		len = XStrBuff::StrLen(lpszString);
		/* 先頭から space tab return を除去する */
		for (i = 0; i < len; i ++) {
			if (XStrBuff::StrChr(trim_str, lpszString[i]) == NULL) {
				::memmove(lpszString, lpszString+i, (len-i+1)*sizeof(TCHAR));
				break;
			}
		}
	}
	if (mode & TRIM_RIGHTONLY ) {
		len = XStrBuff::StrLen(lpszString);
		/* 最後から space tab return を除去する */
		for (i = len - 1; i >= 0; i--) {
			if (XStrBuff::StrChr(trim_str, lpszString[i]) == NULL) {
				break;
			} else {
				lpszString[i] = '\0';
			}
		}
	}
	return m_pStrAddress;
XEXCEPT_EXIT
}
const TCHAR* XStrBuff::Replace(const TCHAR* lpszFind, const TCHAR* lpszDest)
{
XEXCEPT_ENTRY
	XStrBuff tmpStr;
	int      nLength = this->GetLength();
	int      nFindCount = 0;
	TCHAR*   lpsz = m_pStrAddress;
	TCHAR*   lpszWork = NULL;
	TCHAR*	 lpszWork2 = NULL;
	TCHAR*	 pTmpStart = NULL;


	if( m_pStrAddress == NULL ) {
		return NULL;
	}


	_XASSERT(m_pStrAddress);
	_XASSERT(lpszFind);
	_XASSERT(lpszDest);
	_XASSERT( XStrBuff::StrLen(lpszFind) );

	// 文字列中にlpszFindがいくつあるか調べる
	while(1) {
		lpszWork = (TCHAR*)XStrBuff::StrStr(lpsz, lpszFind);
		if( lpszWork == NULL)
			break;
		nFindCount++;
		lpsz = lpszWork;
		lpsz += XStrBuff::StrLen(lpszFind);
	}
	// 見つからない場合はここで終了
	if( nFindCount == 0) {
		return m_pStrAddress;
	}

	// 変換後のサイズの計算
	nLength += ( XStrBuff::StrLen(lpszDest) *  (nFindCount + 1) );
	nLength += 1;	// NULL-term
	nLength *=  sizeof(TCHAR);
	// メモリの確保
	pTmpStart = lpszWork2 = (TCHAR*)tmpStr.Alloc( nLength );
	if( lpszWork2 == NULL ) {
		return NULL;
	}
	tmpStr.MemSet( 0 );



	// 変換
	lpsz = m_pStrAddress;
	lpszWork = NULL;
	while(1) {
		lpszWork = (TCHAR*)XStrBuff::StrStr(lpsz, lpszFind);
		if( lpszWork == NULL) {
			XStrBuff::StrCpy(lpszWork2, tmpStr.getCountOfBuff()-((size_t)(lpszWork2-pTmpStart)), lpsz);
			break;
		}
		*lpszWork = 0;
		XStrBuff::StrCpy(lpszWork2, tmpStr.getCountOfBuff()-((size_t)(lpszWork2-pTmpStart)), lpsz);
		lpszWork2 += XStrBuff::StrLen(lpsz);
		XStrBuff::StrCpy(lpszWork2, tmpStr.getCountOfBuff()-((size_t)(lpszWork2-pTmpStart)), lpszDest);
		lpszWork2 += XStrBuff::StrLen(lpszDest);
		lpsz = lpszWork;
		lpsz += XStrBuff::StrLen(lpszFind);
	}
	*this = (const TCHAR*)tmpStr;

	return m_pStrAddress;
XEXCEPT_EXIT
}
XStrBuff XStrBuff::Left(size_t nCount) const
{
XEXCEPT_ENTRY
	XStrBuff tmp;
	TCHAR c;
	
	_XASSERT(nCount >= 0);
	_XASSERT(nCount < (int)XStrBuff::StrLen(m_pStrAddress));
	
	c = m_pStrAddress[nCount];
	m_pStrAddress[nCount] = 0;
	tmp = m_pStrAddress;
	m_pStrAddress[nCount] = c;
	return tmp;
XEXCEPT_EXIT
}
XStrBuff XStrBuff::Right(size_t nCount) const
{
XEXCEPT_ENTRY
	XStrBuff tmp;
	TCHAR*  p = m_pStrAddress;
	_XASSERT(nCount >= 0);
	_XASSERT(nCount < (int)XStrBuff::StrLen(m_pStrAddress));

	p += ( XStrBuff::StrLen(m_pStrAddress) - nCount-1);
	tmp = p;
	return tmp;
XEXCEPT_EXIT
}
int XStrBuff::Remove(TCHAR chRemove)
{
XEXCEPT_ENTRY
	int				nCount = 0;
	XStrBuff		strRmp(*this);
	TCHAR*			pDest = NULL;
	const TCHAR*	pSrc;


	pDest = m_pStrAddress;
	pSrc = *this;

	while( *pSrc != 0 ) {
		if( *pSrc != chRemove ) {
			*pDest = *pSrc;
			pDest++;
		} else {
			nCount++;
		}
		pSrc++;
	}
	*(pDest++)=0;
	return nCount;
XEXCEPT_EXIT
}
int XStrBuff::Find( const TCHAR* pstr, int nStart ) const
{
XEXCEPT_ENTRY
	if (nStart > (int)XStrBuff::StrLen(m_pStrAddress))
		return -1;
	const TCHAR* lpsz = XStrBuff::StrStr(m_pStrAddress + nStart, pstr);

	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pStrAddress);
XEXCEPT_EXIT
}
int XStrBuff::Find( const TCHAR* lpszSub ) const
{
XEXCEPT_ENTRY
	return Find(lpszSub, 0);
XEXCEPT_EXIT
}
bool XStrBuff::MvPointerSPACE(TCHAR** pWork) 
{
XEXCEPT_ENTRY
	while(**pWork != _T(' ') && **pWork != _T('\t')) {
		if(**pWork == 0) {
			return false;
		}
		(*pWork)++;
	}
	return true;
XEXCEPT_EXIT
}
bool XStrBuff::MvPointerUnSPACE(TCHAR** pWork)
{
XEXCEPT_ENTRY
	while(**pWork == ' ' || **pWork == '\t') {
		if(**pWork == 0) {
			return false;
		}
		(*pWork)++;
	}
	return true;
XEXCEPT_EXIT
}
bool XStrBuff::MvPointerCHAR(TCHAR** pWork, TCHAR c) 
{
XEXCEPT_ENTRY
	while(**pWork != c) {
		if(**pWork == 0) {
			return false;
		}
		(*pWork)++;
	}
	return true;
XEXCEPT_EXIT
}
int XStrBuff::strmatchA(const char *ptn, const char *str)
{
XEXCEPT_ENTRY
	switch( *ptn )
	{
	  case '\0':
			return '\0' == *str;
	  case '*':
			return strmatchA( ptn+1, str ) || (('\0' != *str) && strmatchA( ptn, str+1 ));
	  case '?':
			return ('\0' != *str) && strmatchA( ptn+1, str+1 );
	  default:
			return ((unsigned char)*ptn==(unsigned char)*str) && strmatchA( ptn+1, str+1 );
	}
XEXCEPT_EXIT
}
int XStrBuff::strmatchW(const wchar_t *ptn, const wchar_t *str)
{
XEXCEPT_ENTRY
	switch( *ptn )
	{
	  case L'\0':
			return L'\0' == *str;
	  case '*':
			return strmatchW( ptn+1, str ) || ((L'\0' != *str) && strmatchW( ptn, str+1 ));
	  case '?':
			return (L'\0' != *str) && strmatchW( ptn+1, str+1 );
	  default:
			return ((unsigned char)*ptn==(unsigned char)*str) && strmatchW( ptn+1, str+1 );
	}
XEXCEPT_EXIT
}
int XStrBuff::strmatch(const TCHAR *ptn, const TCHAR *str)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XStrBuff::strmatchW(ptn, str);
#else
	return XStrBuff::strmatchA(ptn, str);
#endif
XEXCEPT_EXIT
}
size_t XStrBuff::StrLen(const TCHAR* lpsz)
{
XEXCEPT_ENTRY
	if( lpsz == NULL ) {
		return 0;
	}
#ifdef _UNICODE
	return ::wcslen(lpsz);
#else
	return ::strlen(lpsz);
#endif	// _UNICODE
XEXCEPT_EXIT
}
TCHAR* XStrBuff::StrCpy(TCHAR* lpszDst, size_t buff_count, const TCHAR* lpszSrc)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	XStrBuff::__strcpyW(lpszDst, buff_count, lpszSrc);
#else
	XStrBuff::__strcpyA(lpszDst, buff_count, lpszSrc);
#endif	// _UNICODE
	return lpszDst;
XEXCEPT_EXIT
}
TCHAR* XStrBuff::StrNCpy(TCHAR* lpszDst, size_t buff_count, const TCHAR* lpszSrc, size_t count)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	XStrBuff::__strncpyW(lpszDst, buff_count, lpszSrc, count);
#else
	XStrBuff::__strncpyA(lpszDst, buff_count, lpszSrc, count);
#endif	// _UNICODE
	return lpszDst;
XEXCEPT_EXIT
}
int XStrBuff::StrCmp(const TCHAR* lpsz1, const TCHAR* lpsz2)
{
XEXCEPT_ENTRY
	if( lpsz1 == NULL && lpsz2 == NULL ) {
		return 0;
	}
	if( lpsz1 == NULL  ) {
		return 1;
	}
	if( lpsz2 == NULL  ) {
		return -1;
	}
#ifdef _UNICODE
	return ::wcscmp(lpsz1, lpsz2);
#else
	return ::strcmp(lpsz1, lpsz2);
#endif	// _UNICODE
XEXCEPT_EXIT
}
int XStrBuff::StrICmp(const TCHAR* lpsz1, const TCHAR* lpsz2)
{
XEXCEPT_ENTRY
	if( lpsz1 == NULL && lpsz2 == NULL ) {
		return 0;
	}
	if( lpsz1 == NULL  ) {
		return 1;
	}
	if( lpsz2 == NULL  ) {
		return -1;
	}
#ifdef _UNICODE
	return XStrBuff::wcsicmp(lpsz1, lpsz2);
#else
	return XStrBuff::stricmp(lpsz1, lpsz2);
#endif	// _UNICODE
XEXCEPT_EXIT
}
int XStrBuff::StrNCmp(const TCHAR* lpsz1, const TCHAR* lpsz2, size_t count)
{
XEXCEPT_ENTRY
	if( lpsz1 == NULL && lpsz2 == NULL ) {
		return 0;
	}
	if( lpsz1 == NULL  ) {
		return 1;
	}
	if( lpsz2 == NULL  ) {
		return -1;
	}
#ifdef _UNICODE
	return ::wcsncmp(lpsz1, lpsz2, count);
#else
	return ::strncmp(lpsz1, lpsz2, count);
#endif	// _UNICODE
XEXCEPT_EXIT
}
TCHAR* XStrBuff::StrInc( const TCHAR *current)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XStrBuff::wcsinc(current);
#else
	return XStrBuff::strinc(current);
#endif	// _UNICODE
XEXCEPT_EXIT
}
TCHAR* XStrBuff::StrRChr( const TCHAR *string, int c)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return (TCHAR*)::wcsrchr(string, c);
#else
	return (TCHAR*)::strrchr(string, c);
#endif	// _UNICODE
XEXCEPT_EXIT
}
int XStrBuff::AtoI(const TCHAR* lpsz)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XStrBuff::wtoi(lpsz);
#else
	return XStrBuff::atoi(lpsz);
#endif	//
XEXCEPT_EXIT
}
XD_INT8 XStrBuff::AtoI64(const TCHAR* lpsz)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XStrBuff::wtoi(lpsz);
#else
	return XStrBuff::atoi(lpsz);
#endif	//
XEXCEPT_EXIT
}

XD_INT8 XStrBuff::strtollA(const char *nptr,char **endptr,int base)
{
XEXCEPT_ENTRY
	register XD_UINT8 cutoff,cutlim,value;
	register char *s=(char*)nptr;
	register unsigned char neg=0;
	register unsigned char any=0;
	register unsigned char ovf=0;
	register unsigned char c;

	value=0;

	if( endptr )
		*endptr=s;

	while((c=*s)!='\0' && (c== ' ' || c == '\t'))
		s++;

	if(*s=='-'){
		neg=1;
		s++;
	} else
	if(*s=='+'){
		s++;
	}

	if(base<2){
		base=10;
		if(s[0]=='0'){
			base=8;
			if(s[1]=='x' || s[1]=='X')
				base=16;
		}
	}

	if(base==16 && s[0]=='0' && (s[1]=='x' || s[1]=='X'))
		s+=2;

	cutoff= neg ? XD_LLONG_MAX + 1 : XD_LLONG_MAX;
	cutlim= cutoff % base;
	cutoff= cutoff / base;

	while((c=*s++)!='\0'){

		// Digit value
		if( '0' <= c && c <= '9' ){
			c = c-'0';
		} else
		if( 'a' <= c && c <= 'z' ){
			c = c-'a'+10;
		} else
		if( 'a' <= c && c <= 'z' ){
			c = c-'A'+10;
		} else
			break;

		if(c>=base)
			break;

		if(ovf || value>cutoff || (value==cutoff && c>cutlim)){
			ovf=1;
			continue;
		}

		value*=base;
		value+=c;
		any=1;
	}

	if(ovf){
		value = neg ? XD_LLONG_MIN : XD_LLONG_MAX;
		errno=ERANGE;
	}

	else if(neg){
		value=-value;
	}

	if(any){
		if(endptr)
			*endptr=s;
	}

	return value;
XEXCEPT_EXIT
}

XD_INT8 XStrBuff::strtollW(const wchar_t *nptr,wchar_t **endptr,int base)
{
XEXCEPT_ENTRY
	register XD_UINT8 cutoff,cutlim,value;
	register wchar_t *s=(wchar_t*)nptr;
	register wchar_t neg=0;
	register wchar_t any=0;
	register wchar_t ovf=0;
	register wchar_t c;

	value=0;

	if( endptr )
		*endptr=s;

	while((c=*s)!=L'\0' && (c== L' ' || c == L'\t'))
		s++;

	if(*s==L'-'){
		neg=1;
		s++;
	} else
	if(*s==L'+'){
		s++;
	}

	if(base<2){
		base=10;
		if(s[0]==L'0'){
			base=8;
			if(s[1]==L'x' || s[1]==L'X')
				base=16;
		}
	}

	if(base==16 && s[0]==L'0' && (s[1]==L'x' || s[1]==L'X'))
		s+=2;

	cutoff= neg ? XD_LLONG_MAX + 1 : XD_LLONG_MAX;
	cutlim= cutoff % base;
	cutoff= cutoff / base;

	while((c=*s)!=L'\0'){

		if( L'0' <= c && c <= L'9' ){
			c = c-L'0';
		} else
		if( L'a' <= c && c <= L'z' ){
			c = c-L'a'+10;
		} else
		if( L'a' <= c && c <= L'z' ){
			c = c-L'A'+10;
		} else
			break;

		if(c>=base)
			break;

		if(ovf || value>cutoff || (value==cutoff && c>cutlim)){
			ovf=1;
			continue;
		}

		value*=base;
		value+=c;
		any=1;
		s++;
	}

	if(ovf){
		value = neg ? XD_LLONG_MIN : XD_LLONG_MAX;
		errno=ERANGE;
	}

	else if(neg){
		value=-value;
	}

	if(any){
		if(endptr)
			*endptr=s;
	}

	return value;
XEXCEPT_EXIT
}
XD_UINT8 XStrBuff::strtoullA(const char *nptr,char **endptr,int base)
{
XEXCEPT_ENTRY
	register XD_UINT8 cutoff,cutlim,value;
	register char *s=(char*)nptr;
	register unsigned char neg=0;
	register unsigned char any=0;
	register unsigned char ovf=0;
	register unsigned char c;

	value=0;

	if(endptr) *endptr=s;

	while((c=*s)!='\0' && (c== ' ' || c == '\t'))
		s++;

	if(*s=='-'){
		neg=1;
		s++;
	} else
	if(*s=='+'){
		s++;
	}

	if(base<2){
		base=10;
		if(s[0]=='0'){
			base=8;
			if(s[1]=='x' || s[1]=='X')
				base=16;
		}
	}

	if(base==16 && s[0]=='0' && (s[1]=='x' || s[1]=='X'))
		s+=2;

	cutoff = XD_ULLONG_MAX;
	cutlim=cutoff%base;
	cutoff=cutoff/base;

	while((c=*s)!='\0'){

		if( '0' <= c && c << '9' ){
			c = c-'0';
		} else
		if( 'a' <= c && c << 'z' ){
			c = c-'a'+10;
		} else
		if( 'a' <= c && c << 'z' ){
			c = c-'A'+10;
		} else
			break;

		if(c>=base)
			break;

		if(ovf || value>cutoff || (value==cutoff && c>cutlim)){
			ovf=1;
			continue;
		}

		value*=base;
		value+=c;
		any=1;
		s++;
	}

	if(ovf){
		value=XD_ULLONG_MAX;
		errno=ERANGE;
	}

	else if(neg){
		value=-value;
	}

	if(any){
		if(endptr)
			*endptr=s;
	}

	return value;
XEXCEPT_EXIT
}

XD_UINT8 XStrBuff::strtoullW(const wchar_t *nptr,wchar_t **endptr,int base)
{
XEXCEPT_ENTRY
	register XD_UINT8 cutoff,cutlim,value;
	register wchar_t *s=(wchar_t*)nptr;
	register unsigned char neg=0;
	register unsigned char any=0;
	register unsigned char ovf=0;
	register unsigned char c;

	value=0;

	if(endptr) *endptr=s;

	while((c=*s++)!=L'\0' && (c== L' ' || c == L'\t'))
		s++;

	if(*s==L'-'){
		neg=1;
		s++;
	} else
	if(*s==L'+'){
		s++;
	}

	if(base<2){
		base=10;
		if(s[0]==L'0'){
			base=8;
			if(s[1]==L'x' || s[1]==L'X')
				base=16;
		}
	}

	if(base==16 && s[0]==L'0' && (s[1]==L'x' || s[1]==L'X'))
		s+=2;

	cutoff = XD_ULLONG_MAX;
	cutlim=cutoff%base;
	cutoff=cutoff/base;

	while((c=*s++)!=L'\0'){

		if( L'0' <= c && c << L'9' ){
			c = c-L'0';
		} else
		if( L'a' <= c && c << L'z' ){
			c = c-L'a'+10;
		} else
		if( L'a' <= c && c << L'z' ){
			c = c-L'A'+10;
		} else
			break;

		if(c>=base)
			break;

		if(ovf || value>cutoff || (value==cutoff && c>cutlim)){
			ovf=1;
			continue;
		}

		value*=base;
		value+=c;
		any=1;
	}

	if(ovf){
		value=XD_ULLONG_MAX;
		errno=ERANGE;
	}

	else if(neg){
		value=-value;
	}

	if(any){
		if(endptr)
			*endptr=s;
	}

	return value;
XEXCEPT_EXIT
}
XD_INT8 XStrBuff::strtoll(const TCHAR *nptr,TCHAR **endptr,int base)
{
XEXCEPT_ENTRY
	if( base == -1 ) {
		base = 10;
		// 規定値の自動判定
		if( XStrBuff::StrLen(nptr) > 2 ) {
			if( nptr[0] == _T('0') && ( nptr[1] == _T('x') || nptr[1] == _T('X') ) ) {
				base = 16;
				nptr+=2;	// ポインタを進める
			}
		}
	}
#ifdef _UNICODE
	return XStrBuff::strtollW(nptr,endptr,base);
#else
	return XStrBuff::strtollA(nptr,endptr,base);
#endif
XEXCEPT_EXIT
}
XD_INT4 XStrBuff::strtol(const TCHAR *nptr,TCHAR **endptr,int base)
{
XEXCEPT_ENTRY
	if( base == -1 ) {
		base = 10;
		// 規定値の自動判定
		if( XStrBuff::StrLen(nptr) > 2 ) {
			if( nptr[0] == _T('0') && ( nptr[1] == _T('x') || nptr[1] == _T('X') ) ) {
				base = 16;
				nptr+=2;	// ポインタを進める
			}
		}
	}
#ifdef _UNICODE
	return ::wcstol(nptr,endptr,base);
#else
	return ::strtol(nptr,endptr,base);
#endif
XEXCEPT_EXIT
}
XD_UINT8 XStrBuff::strtoull(const TCHAR *nptr,TCHAR **endptr,int base)
{
XEXCEPT_ENTRY
	if( base == -1 ) {
		base = 10;
		// 規定値の自動判定
		if( XStrBuff::StrLen(nptr) > 2 ) {
			if( nptr[0] == _T('0') && ( nptr[1] == _T('x') || nptr[1] == _T('X') ) ) {
				base = 16;
				nptr+=2;	// ポインタを進める
			}
		}
	}
#ifdef _UNICODE
	return XStrBuff::strtoullW(nptr,endptr,base);
#else
	return XStrBuff::strtoullA(nptr,endptr,base);
#endif
XEXCEPT_EXIT
}
XD_UINT4 XStrBuff::strtoul(const TCHAR *nptr,TCHAR **endptr,int base)
{
XEXCEPT_ENTRY
	if( base == -1 ) {
		base = 10;
		// 規定値の自動判定
		if( XStrBuff::StrLen(nptr) > 2 ) {
			if( nptr[0] == _T('0') && ( nptr[1] == _T('x') || nptr[1] == _T('X') ) ) {
				base = 16;
				nptr+=2;	// ポインタを進める
			}
		}
	}
#ifdef _UNICODE
	return ::wcstoul(nptr,endptr,base);
#else
	return ::strtoul(nptr,endptr,base);
#endif
XEXCEPT_EXIT
}


XD_FLOAT8 XStrBuff::strtodA( const char *nptr, char **endptr )
{
XEXCEPT_ENTRY
	return ::strtod(nptr, endptr);
XEXCEPT_EXIT
}
XD_FLOAT8 XStrBuff::strtodW( const wchar_t *nptr, wchar_t **endptr  )
{
XEXCEPT_ENTRY
	return ::wcstod(nptr, endptr);
XEXCEPT_EXIT
}

XD_FLOAT8 XStrBuff::strtod( const TCHAR *nptr, TCHAR **endptr  )
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XStrBuff::strtodW(nptr, endptr);
#else
	return XStrBuff::strtodA(nptr, endptr);
#endif	//
XEXCEPT_EXIT
}


int XStrBuff::isDigit(const TCHAR t)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return ::iswdigit(t);
#else
	return isdigit(t);
#endif	//
XEXCEPT_EXIT
}
int XStrBuff::isXDigit(const TCHAR t)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return ::iswxdigit(t);
#else
	return isxdigit(t);
#endif	//
XEXCEPT_EXIT
}
int XStrBuff::isDigitsEx(const TCHAR* pStr)
{
XEXCEPT_ENTRY
	int base = 10;

	// 規定値の自動判定
	if( XStrBuff::StrLen(pStr) > 2 ) {
		if( pStr[0] == _T('0') && ( pStr[1] == _T('x') || pStr[1] == _T('X') ) ) {
			base = 16;
			pStr+=2;	// ポインタを進める
		}
	}
	if( *pStr == _T('-') || *pStr == _T('+') ) {
		pStr++;
	}

	while( *pStr != 0 ) {
		if( base == 10 ) {
			if( isDigit( *pStr ) == 0 ) {
				return 0;
			}
		} else {
			if( isXDigit( *pStr ) == 0 ) {
				return 0;
			}
		}
		pStr++;
	}
	return 1;
XEXCEPT_EXIT
}
int XStrBuff::VsPrintf(TCHAR *buffer, size_t buff_count, const TCHAR *format, va_list argptr)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XStrBuff::__vswprintf(buffer, buff_count, format, argptr);
#else
	return XStrBuff::__vsprintf(buffer, buff_count, format, argptr);
#endif	//
XEXCEPT_EXIT
}

int XStrBuff::VsnPrintf(TCHAR *buffer, size_t buff_count, size_t maxlen, const TCHAR *format, va_list argptr)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	#ifdef _MSC_VER
		return XStrBuff::__vsnwprintf(buffer, buff_count, maxlen, format, argptr);
	#else
		return vsnwprintf(buffer, maxlen, format, argptr);
	#endif
#else
	#ifdef _MSC_VER
		return XStrBuff::__vsnprintf(buffer, buff_count, maxlen, format, argptr);
	#else
		return vsnprintf(buffer, maxlen, format, argptr);
	#endif
#endif	//
XEXCEPT_EXIT
}


int XStrBuff::printf(TCHAR *buffer, size_t buff_count, const TCHAR *format, ...)
{
XEXCEPT_ENTRY
	int iret = 0;
	va_list args;
	va_start(args, format);
	iret = XStrBuff::VsPrintf(buffer, buff_count, format, args);
	va_end(args);
	return iret;
XEXCEPT_EXIT
}


int XStrBuff::nprintf(TCHAR *buffer, size_t buff_count, size_t maxlen, const TCHAR *format, ...)
{
XEXCEPT_ENTRY
	int iret = 0;
	va_list args;
	va_start(args, format);
	iret = XStrBuff::VsPrintf(buffer, buff_count, format, args);
	va_end(args);
	return iret;
XEXCEPT_EXIT
}


TCHAR* XStrBuff::StrUpr(TCHAR *string)
{
XEXCEPT_ENTRY
#ifdef WIN32
	#ifdef _UNICODE
		return ::wcsupr(string);
	#else
		return XStrBuff::strupr(string);
	#endif	//
#else
	#ifdef _UNICODE
		wchar_t *cp; 
		for ( cp = wsrc ; *cp ; cp++ )
		{
			if ( (*cp >= (wchar_t)L'a') && (*cp <= (wchar_t)L'z') )
				*cp = *cp - (L'a' - L'A');
		}
		return(wsrc);
	#else
		char *cp;
		for ( cp = string ; *cp ; ++cp )
		{
			if ( ('a' <= *cp) && (*cp <= 'z') )
			{
				*cp -= 'a' - 'A';
			}
		}
		return(string);
	#endif	//
#endif
XEXCEPT_EXIT
}
TCHAR* XStrBuff::StrLwr(TCHAR *string)
{
XEXCEPT_ENTRY
#ifdef WIN32
	#ifdef _UNICODE
		return ::wcslwr(string);
	#else
		return XStrBuff::strlwr(string);
	#endif	//
#else
	#ifdef _UNICODE
		wchar_t *cp; 
		for ( cp = wsrc ; *cp ; cp++ )
		{
			if ( (*cp >= (wchar_t)L'A') && (*cp <= (wchar_t)L'Z') )
				*cp -= L'A' - L'a';
		}
		return(wsrc);
	#else
		char *cp;

		for ( cp = string ; *cp ; ++cp )
			if ( ('A' <= *cp) && (*cp <= 'Z') )
				*cp -= 'A' - 'a';
		return(string);
	#endif	//
#endif
XEXCEPT_EXIT
}
TCHAR* XStrBuff::StrChr(const TCHAR *lpsz, TCHAR c)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return (TCHAR*)::wcschr(lpsz, c);
#else
	return (TCHAR*)::strchr(lpsz, c);
#endif	//
XEXCEPT_EXIT
}
const TCHAR* XStrBuff::StrStr(const TCHAR *string1, const TCHAR *string2)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return ::wcsstr(string1, string2);
#else
	return ::strstr(string1, string2);
#endif	//
XEXCEPT_EXIT
}
TCHAR* XStrBuff::StrStr(TCHAR *string1, const TCHAR *string2)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return ::wcsstr(string1, string2);
#else
	return ::strstr(string1, string2);
#endif	//
XEXCEPT_EXIT
}
TCHAR* XStrBuff::StrCat(TCHAR *string1, size_t buff_count, const TCHAR *string2 )
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
#ifdef _UNICODE
	errno_t err = ::wcscat_s(string1, buff_count, string2);
#else
	errno_t err = ::strcat_s(string1, buff_count, string2);
#endif	//
	return string1;
#else 
#ifdef _UNICODE
	return ::wcscat(string1, string2);
#else
	return ::strcat(string1, string2);
#endif	//
#endif

XEXCEPT_EXIT
}

int XStrBuff::strcmp(const char *dst, const char *src)
{
XEXCEPT_ENTRY
	return ::strcmp(dst,src);
XEXCEPT_EXIT
}
int XStrBuff::wcscmp(const wchar_t * dst, const wchar_t * src)
{
XEXCEPT_ENTRY
	return ::wcscmp(dst,src);
XEXCEPT_EXIT
}


int XStrBuff::stricmp(const char *dst, const char *src)
{
XEXCEPT_ENTRY
#if defined(WIN32)
	return ::_stricmp(dst,src);
#else
	int f, l;
	do {
		if ( ((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z') ) {
			f -= 'A' - 'a';
		}
		if ( ((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z') ) {
			l -= 'A' - 'a';
		}
	} while ( f && (f == l) );
	return(f - l);
#endif
XEXCEPT_EXIT
}
int XStrBuff::wcsicmp(const wchar_t * dst, const wchar_t * src)
{
XEXCEPT_ENTRY
#if defined(WIN32)
	return ::_wcsicmp(dst,src);
#else
	while ((*src != L'\0') && (towlower(*dst) == towlower(*src))) {
		dst++;
		src++;
	}
	return towlower(*dst) - towlower(*src);
#endif
XEXCEPT_EXIT
}
char*    XStrBuff::strupr (char* string)
{
XEXCEPT_ENTRY
#if defined(WIN32)
	return ::_strupr(string);
#else
	char *cp;
	for ( cp = string ; *cp ; ++cp ) {
		if ( ('a' <= *cp) && (*cp <= 'z') ) {
			*cp -= 'a' - 'A';
		}
	}
	return(string);
#endif
XEXCEPT_EXIT
}
wchar_t* XStrBuff::wcsupr(wchar_t * wsrc)
{
XEXCEPT_ENTRY
#if defined(WIN32)
	return ::_wcsupr(wsrc);
#else
	wchar_t *cp; 
	for ( cp = wsrc ; *cp ; cp++ ) {
		if ( (*cp >= (wchar_t)L'a') && (*cp <= (wchar_t)L'z') ) {
			*cp = *cp - (L'a' - L'A');
		}
	}
	return(wsrc);
#endif
XEXCEPT_EXIT
}
char* XStrBuff::strlwr(char * string)
{
XEXCEPT_ENTRY
#if defined(WIN32)
	return ::_strlwr(string);
#else
	char *cp;

	for ( cp = string ; *cp ; ++cp ) {
		if ( ('A' <= *cp) && (*cp <= 'Z') ) {
			*cp -= 'A' - 'a';
		}
	}
	return(string);
#endif
XEXCEPT_EXIT
}
wchar_t* XStrBuff::wcslwr(wchar_t * wsrc)
{
XEXCEPT_ENTRY
#if defined(WIN32)
	return ::_wcslwr(wsrc);
#else
	wchar_t *cp; 
	for ( cp = wsrc ; *cp ; cp++ ) {
		if ( (*cp >= (wchar_t)L'A') && (*cp <= (wchar_t)L'Z') ) {
			*cp -= L'A' - L'a';
		}
	}
	return(wsrc);
#endif
XEXCEPT_EXIT
}
char* XStrBuff::strinc( const char *current)
{
XEXCEPT_ENTRY
	current++;
	return (char*)current;
XEXCEPT_EXIT
}
wchar_t* XStrBuff::wcsinc( const wchar_t *current)
{
XEXCEPT_ENTRY
	current++;
	return (wchar_t*)current;
XEXCEPT_EXIT
}
int XStrBuff::atoi(const char *string)
{
XEXCEPT_ENTRY
	return ::atoi(string);
XEXCEPT_EXIT
}
int XStrBuff::wtoi( const wchar_t *string )
{
XEXCEPT_ENTRY
	int iData = 0;

	while( *string != 0 ) {
		if( !('0' <= *string && *string  <= '9') ) {
			return iData;
		}

		iData *= 10;
		iData += *string - '0';
		string++;
	}

	return iData;
XEXCEPT_EXIT
}
errno_t XStrBuff::__strcpyA(char* p_dst, size_t buff_count, const char* p_src)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return ::strcpy_s(p_dst, buff_count, p_src);
#else
	strcpy(p_dst, p_src);
#endif
	return 0;
XEXCEPT_EXIT
}

errno_t XStrBuff::__strcpyW(wchar_t* p_dst, size_t buff_count, const wchar_t* p_src)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return ::wcscpy_s(p_dst, buff_count, p_src);
#else
	wcscpy(p_dst, p_src);
#endif
	return 0;
XEXCEPT_EXIT
}
errno_t XStrBuff::__strncpyA(char* p_dst, size_t buff_count, const char* p_src, size_t max_count)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return ::strncpy_s(p_dst, buff_count, p_src, max_count);
#else
	strncpy(p_dst, p_src, max_count);
#endif
	return 0;
XEXCEPT_EXIT
}
errno_t XStrBuff::__strncpyW(wchar_t* p_dst, size_t buff_count, const wchar_t* p_src, size_t max_count)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return ::wcsncpy_s(p_dst, buff_count, p_src, max_count);
#else
	wcsncpy(p_dst, p_src, max_count);
#endif
	return 0;
XEXCEPT_EXIT
}
errno_t XStrBuff::__wcstombs(char* p_dst, size_t buff_count, const wchar_t* p_src, size_t max_count)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	size_t convLen = 0;
	return wcstombs_s(&convLen, p_dst, buff_count, p_src, _TRUNCATE);
#else
	wcstombs(p_dst, p_src, max_count);
#endif
	return 0;
XEXCEPT_EXIT
}
errno_t XStrBuff::__mbstowcs(wchar_t* p_dst, size_t buff_count, const char* p_src, size_t src_count)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	size_t convLen = 0;
	::mbstowcs_s(&convLen, p_dst, buff_count, p_src, _TRUNCATE );	
#else
	::mbstowcs(p_dst, p_src, src_count );	
#endif
	return 0;
XEXCEPT_EXIT
}
errno_t XStrBuff::__sprintf(char* p_dst, size_t buff_count, const char* p_format, ...)
{
XEXCEPT_ENTRY
	errno_t iret = 0;
	va_list args;
	va_start(args, p_format);
	iret = XStrBuff::__vsprintf(p_dst, buff_count, p_format, args);
	va_end(args);
	return 0;
XEXCEPT_EXIT
}
errno_t XStrBuff::__swprintf(wchar_t* p_dst, size_t buff_count, const wchar_t* p_format, ...)
{
XEXCEPT_ENTRY
	errno_t iret = 0;
	va_list args;
	va_start(args, p_format);
	iret = XStrBuff::__vswprintf(p_dst, buff_count, p_format, args);
	va_end(args);
	return 0;
XEXCEPT_EXIT
}
errno_t XStrBuff::__vsprintf(char *buffer, size_t buff_count, const char *format, va_list argptr)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return vsprintf_s(buffer, buff_count, format, argptr);
#else
	vsprintf(buffer, format, argptr);
	return 0
#endif;
XEXCEPT_EXIT
}
errno_t XStrBuff::__vswprintf(wchar_t *buffer, size_t buff_count, const wchar_t *format, va_list argptr)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return vswprintf_s(buffer, buff_count, format, argptr);
#else
	vsprintf(buffer, format, argptr);
	return 0
#endif;
XEXCEPT_EXIT
}
errno_t XStrBuff::__vsnprintf(char *buffer, size_t buff_count, size_t max_count, const char *format, va_list argptr)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return _vsnprintf_s(buffer, buff_count, max_count, format, argptr);
#else
	_vsnprintf(buffer, max_count, format, argptr);
	return 0;
#endif
XEXCEPT_EXIT
}
errno_t XStrBuff::__vsnwprintf(wchar_t *buffer, size_t buff_count, size_t max_count, const wchar_t *format, va_list argptr)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return _vsnwprintf_s(buffer, buff_count, max_count, format, argptr);
#else
	_vsnprintf(buffer, max_count, format, argptr);
	return 0;
#endif
XEXCEPT_EXIT
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// XStrBuffList
//
XStrBuffList::XStrBuffList()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XStrBuffList::~XStrBuffList()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XStrBuffList::clear()
{
XEXCEPT_ENTRY
	XPOSITION pos;

	for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
		XStrBuff *pStr = (XStrBuff*)m_list.GetNext( pos );
		delete pStr;
	}
	m_list.RemoveAll();
XEXCEPT_EXIT
}
XStrBuffList& XStrBuffList::append(const char* lpszFormat, ...)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = new XStrBuff();
	m_list.AddTail(pStr);

	va_list args;
	va_start(args, lpszFormat);
#ifdef _UNICODE	
	XStrBuff strFormat(lpszFormat);
	pStr->FormatV(strFormat, args);
#else
	pStr->FormatV(lpszFormat, args);
#endif
	va_end(args);
	return *this;
XEXCEPT_EXIT
}
XStrBuffList& XStrBuffList::append(const wchar_t* lpszFormat, ...)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = new XStrBuff();
	m_list.AddTail(pStr);

	va_list args;
	va_start(args, lpszFormat);
#ifdef _UNICODE	
	pStr->FormatV(lpszFormat, args);
#else
	XStrBuff strFormat(lpszFormat);
	pStr->FormatV(strFormat, args);
#endif
	va_end(args);


	return *this;
XEXCEPT_EXIT
}
XStrBuffList& XStrBuffList::append(XStrBuff& str)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = new XStrBuff();
	m_list.AddTail(pStr);
	*pStr = str;
	return *this;
XEXCEPT_EXIT
}
XStrBuffList& XStrBuffList::insert(int iPos, const char* lpszFormat, ...)
{
XEXCEPT_ENTRY
	XPOSITION pos = m_list.FindIndex(iPos);	
	if( pos == NULL ) {
		return *this;
	}
	
	XStrBuff *pStr = new XStrBuff();
	m_list.InsertAfter(pos, pStr);

	va_list args;
	va_start(args, lpszFormat);
#ifdef _UNICODE	
	XStrBuff strFormat(lpszFormat);
	pStr->FormatV(strFormat, args);
#else
	pStr->FormatV(lpszFormat, args);
#endif
	va_end(args);

	return *this;
XEXCEPT_EXIT
}
XStrBuffList& XStrBuffList::insert(int iPos, const wchar_t* lpszFormat, ...)
{
XEXCEPT_ENTRY
	XPOSITION pos = m_list.FindIndex(iPos);	
	if( pos == NULL ) {
		return *this;
	}
	
	XStrBuff *pStr = new XStrBuff();
	m_list.InsertAfter(pos, pStr);

	va_list args;
	va_start(args, lpszFormat);
#ifdef _UNICODE	
	pStr->FormatV(lpszFormat, args);
#else
	XStrBuff strFormat(lpszFormat);
	pStr->FormatV(strFormat, args);
#endif
	va_end(args);

	return *this;
XEXCEPT_EXIT
}
XStrBuffList& XStrBuffList::insert(int iPos, XStrBuff& str)
{
XEXCEPT_ENTRY
	XPOSITION pos = m_list.FindIndex(iPos);	
	if( pos == NULL ) {
		return *this;
	}
	
	XStrBuff *pStr = new XStrBuff();
	m_list.InsertAfter(pos, pStr);

	*pStr = str;

	return *this;
XEXCEPT_EXIT
}
int XStrBuffList::GetCount() const
{
XEXCEPT_ENTRY
	return m_list.GetCount();
XEXCEPT_EXIT
}
XStrBuff* XStrBuffList::GetAt(int iPos)
{
XEXCEPT_ENTRY
	XPOSITION pos = m_list.FindIndex(iPos);	
	if( pos == NULL ) {
		return NULL;
	}
	XStrBuff* pStr = (XStrBuff*)m_list.GetAt(pos);
	return pStr;
XEXCEPT_EXIT
}
size_t XStrBuffList::capacityA(const char *pBefore /*= NULL*/, const char* pAfter /*= NULL*/)
{
XEXCEPT_ENTRY
	size_t iSize = 0;
	size_t iAppendSize = 0;
	XPOSITION pos;

	if( pBefore != NULL ) {
		iAppendSize += ::strlen(pBefore);
	}
	if( pAfter != NULL ) {
		iAppendSize += ::strlen(pAfter);
	}

	for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
		XStrBuff *pStr = (XStrBuff*)m_list.GetNext( pos );
		iSize += pStr->GetLength();
		iSize += iAppendSize;
	}
	iSize += 1;
	iSize *= sizeof( TCHAR );
	return iSize;
XEXCEPT_EXIT
}
size_t XStrBuffList::capacityW(const wchar_t *pBefore /*= NULL*/, const wchar_t* pAfter /*= NULL*/)
{
XEXCEPT_ENTRY
	size_t iSize = 0;
	size_t iAppendSize = 0;
	XPOSITION pos;

	if( pBefore != NULL ) {
		iAppendSize += ::wcslen(pBefore);
	}
	if( pAfter != NULL ) {
		iAppendSize += ::wcslen(pAfter);
	}

	for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
		XStrBuff *pStr = (XStrBuff*)m_list.GetNext( pos );
		iSize += pStr->GetLength();
		iSize += iAppendSize;
	}
	iSize += 1;
	iSize *= sizeof( TCHAR );
	return iSize;
XEXCEPT_EXIT
}
size_t XStrBuffList::capacity(const TCHAR *pBefore /*= NULL*/, const TCHAR* pAfter /*= NULL*/)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return capacityW(pBefore, pAfter);
#else
	return capacityA(pBefore, pAfter);
#endif
XEXCEPT_EXIT
}
XStrBuff XStrBuffList::toStringA(const char *pBefore/* = NULL*/, const char* pAfter/* = NULL*/)
{
XEXCEPT_ENTRY
	XStrBuff str;
	size_t iSize = capacityA(pBefore, pAfter) + 10;
	char *p = (char*)str.Alloc( iSize );
	char* p_start = p;
	XPOSITION pos;

	str.MemSet(0);
	if( p == NULL ) {
		return str;
	}
	for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
		XStrBuff *pStr = (XStrBuff*)m_list.GetNext( pos );
		
		if( pBefore != NULL ) {
			XStrBuff::__strcpyA(p, iSize-((size_t)(p-p_start)), pBefore);
			p += ::strlen(p);
		}

		
		XStrBuff::__strcpyA(p, iSize-((size_t)(p-p_start)), *pStr);
		p += ::strlen(p);


		if( pAfter != NULL ) {
			XStrBuff::__strcpyA(p, iSize-((size_t)(p-p_start)), pAfter);
			p += ::strlen(p);
		}
	}
	return str;
XEXCEPT_EXIT
}
XStrBuff XStrBuffList::toStringW(const wchar_t *pBefore/* = NULL*/, const wchar_t* pAfter/* = NULL*/)
{
XEXCEPT_ENTRY
	XStrBuff str;
	size_t iSize = capacityW(pBefore, pAfter) + 10;
	wchar_t *p = (wchar_t*)str.Alloc( iSize );
	wchar_t* p_start = p;
	XPOSITION pos;

	str.MemSet(0);
	if( p == NULL ) {
		return str;
	}
	for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
		XStrBuff *pStr = (XStrBuff*)m_list.GetNext( pos );
		
		if( pBefore != NULL ) {
			XStrBuff::__strcpyW(p, (iSize-((size_t)(p_start-p)))/sizeof(wchar_t), pBefore);
			p += ::wcslen(p);
		}

		
		XStrBuff::__strcpyW(p, (iSize-((size_t)(p_start-p)))/sizeof(wchar_t), *pStr);
		p += ::wcslen(p);


		if( pAfter != NULL ) {
			XStrBuff::__strcpyW(p, (iSize-((size_t)(p_start-p)))/sizeof(wchar_t), pAfter);
			p += ::wcslen(p);
		}
	}
	return str;
XEXCEPT_EXIT
}
XStrBuff XStrBuffList::toString(const TCHAR *pBefore/* = NULL*/, const TCHAR* pAfter/* = NULL*/)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return toStringW(pBefore, pAfter);
#else
	return toStringA(pBefore, pAfter);
#endif
XEXCEPT_EXIT
}
																					

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// XStrBuffCSV
//
XStrBuffCSV::XStrBuffCSV(TCHAR cDelimit/* = ','*/, TCHAR cIndeChar/* = '"\'*/)
	:XStrBuff()
{
XEXCEPT_ENTRY
	m_bConv = false;
	m_cDelimit = cDelimit;
	m_cIndeChar = cIndeChar;
XEXCEPT_EXIT
}
XStrBuffCSV::XStrBuffCSV(const char* p, TCHAR cDelimit/* = ','*/, TCHAR cIndeChar/* = '"\'*/)
	:XStrBuff(p)
{
XEXCEPT_ENTRY
	m_bConv = false;
	m_cDelimit = cDelimit;
	m_cIndeChar = cIndeChar;
	Conv();
	m_bConv = true;
XEXCEPT_EXIT
}
XStrBuffCSV::XStrBuffCSV(const wchar_t* p, TCHAR cDelimit/* = ','*/, TCHAR cIndeChar/* = '"\'*/)
	:XStrBuff(p)
{
XEXCEPT_ENTRY
	m_bConv = false;
	m_cDelimit = cDelimit;
	m_cIndeChar = cIndeChar;
	Conv();
	m_bConv = true;
XEXCEPT_EXIT
}

const XStrBuffCSV& XStrBuffCSV::operator=(const char* lpsz)
{
XEXCEPT_ENTRY
	XStrBuff::operator=(lpsz);
	m_bConv = false;
	Conv();
	return *this;
XEXCEPT_EXIT
}
const XStrBuffCSV& XStrBuffCSV::operator=(const wchar_t* lpsz)
{
XEXCEPT_ENTRY
	XStrBuff::operator=(lpsz);
	m_bConv = false;
	Conv();
	return *this;
XEXCEPT_EXIT
}

const TCHAR* XStrBuffCSV::Conv()
{
XEXCEPT_ENTRY
	int      nLength = this->GetLength();
	XStrBuff tmpStr;
	int      nFindCount = 0;
	int      nC = 0;
	int		 nLF = 0;
	TCHAR*   lpsz = m_pStrAddress;
	TCHAR*   lpszWork;
	TCHAR*	 lpszWork2;

	_XASSERT(m_pStrAddress);

	if( m_bConv == true)
		return m_pStrAddress;

	// 文字列中にlpszFindがいくつあるか調べる
	while(*lpsz != 0) {
		if( *lpsz == m_cIndeChar)
			nFindCount++;
		if( *lpsz == m_cDelimit)
			nC++;
		if( *lpsz == '\n')
			nLF++;
		lpsz++;
	}
	// 見つからない場合はここで終了
	if( nFindCount == 0) {
		m_bConv = true;
		if(nC || nLF) {
			tmpStr.Format(_T("\"%s\""), (const TCHAR*)m_pStrAddress);
			XStrBuff::operator=((const TCHAR*)tmpStr);
		}
		return m_pStrAddress;
	}

	// 変換後のサイズの計算
	nLength += (nFindCount  + 1);
	nLength *= sizeof(TCHAR);
	// メモリの確保
	lpszWork2 = (TCHAR*)tmpStr.Alloc( nLength );
	if( lpszWork2 == NULL ) {
		return NULL;
	}
	lpszWork = lpszWork2;

	lpsz = m_pStrAddress;
	// 文字列中にlpszFindがいくつあるか調べる
	while(*lpsz != 0) {
		if( *lpsz == m_cIndeChar) {
			*(lpszWork2++) = m_cIndeChar;
		}
		*(lpszWork2++) = *(lpsz++);
	}
	*(lpszWork2++) = 0;

	XStrBuff::operator=(lpszWork);
	if(nC || nLF) {
		tmpStr.Format(_T("\"%s\""), (const TCHAR*)m_pStrAddress);
		XStrBuff::operator=((const TCHAR*)tmpStr);
	}
	m_bConv = true;
	return m_pStrAddress;
XEXCEPT_EXIT
}



XStrBuffSQL::XStrBuffSQL(TCHAR cDelimit/* = '\''*/, TCHAR cIndeChar/* = '\''*/)
	:XStrBuff()
{
XEXCEPT_ENTRY
	m_bConv = false;
	m_cDelimit = cDelimit;
	m_cIndeChar = cIndeChar;
XEXCEPT_EXIT
}
XStrBuffSQL::XStrBuffSQL(const char* p, TCHAR cDelimit/* = '\''*/, TCHAR cIndeChar/* = '\''*/)
	:XStrBuff(p)
{
XEXCEPT_ENTRY
	m_bConv = false;
	m_cDelimit = cDelimit;
	m_cIndeChar = cIndeChar;
	Conv();
	m_bConv = true;
XEXCEPT_EXIT
}

XStrBuffSQL::XStrBuffSQL(const wchar_t* p, TCHAR cDelimit/* = '\''*/, TCHAR cIndeChar/* = '\''*/)
	:XStrBuff(p)
{
XEXCEPT_ENTRY
	m_bConv = false;
	m_cDelimit = cDelimit;
	m_cIndeChar = cIndeChar;
	Conv();
	m_bConv = true;
XEXCEPT_EXIT
}

const XStrBuffSQL& XStrBuffSQL::operator=(const char* lpsz)
{
XEXCEPT_ENTRY
	XStrBuff::operator=(lpsz);
	m_bConv = false;
	Conv();
	return *this;
XEXCEPT_EXIT
}
const XStrBuffSQL& XStrBuffSQL::operator=(const wchar_t* lpsz)
{
XEXCEPT_ENTRY
	XStrBuff::operator=(lpsz);
	m_bConv = false;
	Conv();
	return *this;
XEXCEPT_EXIT
}

const TCHAR* XStrBuffSQL::Conv()
{
XEXCEPT_ENTRY
	int      nLength = this->GetLength();
	XStrBuff tmpStr;
	int      nFindCount = 0;
	TCHAR*   lpsz = m_pStrAddress;
	TCHAR*   lpszWork;
	TCHAR*	 lpszWork2;

	_XASSERT(m_pStrAddress);

	if( m_bConv == true)
		return m_pStrAddress;

	// 文字列中にlpszFindがいくつあるか調べる
	while(*lpsz != 0) {
		if( *lpsz == m_cDelimit)
			nFindCount++;
		lpsz++;
	}
	// 見つからない場合はここで終了
	if( nFindCount == 0) {
		m_bConv = true;
		return m_pStrAddress;
	}

	// 変換後のサイズの計算
	nLength += (( sizeof(TCHAR) ) *  (nFindCount + 64));
	// メモリの確保
	lpszWork2 = (TCHAR*)tmpStr.Alloc( nLength );
	if( lpszWork2 == NULL ) {
		return NULL;
	}
	lpszWork = lpszWork2;

	lpsz = m_pStrAddress;
	// 文字列中にlpszFindがいくつあるか調べる
	while(*lpsz != 0) {
		if( *lpsz == m_cDelimit) {
			*(lpszWork2++) = m_cIndeChar;
		}
		*(lpszWork2++) = *(lpsz++);
	}
	*(lpszWork2++) = 0;

	XStrBuff::operator=(lpszWork);
	m_bConv = true;
	return m_pStrAddress;
XEXCEPT_EXIT
}




XStrBuffSplit::XStrBuffSplit(TCHAR cDelimiter, TCHAR cIndeChar)
	:	XStrBuff()
{
XEXCEPT_ENTRY
	CommounConstruct(cDelimiter, cIndeChar);
XEXCEPT_EXIT
}
XStrBuffSplit::XStrBuffSplit(const char* p, TCHAR cDelimiter, TCHAR cIndeChar)
	:	XStrBuff(p)
{
XEXCEPT_ENTRY
	CommounConstruct(cDelimiter, cIndeChar);
	if(m_cDelimiter == 0) {
		AllocNullNullTerminateA(p);
		SpritNullNullTerminate();
	} else {
		Sprit();
	}
XEXCEPT_EXIT
}

XStrBuffSplit::XStrBuffSplit(const wchar_t* p, TCHAR cDelimiter, TCHAR cIndeChar)
	:	XStrBuff(p)
{
XEXCEPT_ENTRY
	CommounConstruct(cDelimiter, cIndeChar);
	if(m_cDelimiter == 0) {
		AllocNullNullTerminateW(p);
		SpritNullNullTerminate();
	} else {
		Sprit();
	}
XEXCEPT_EXIT
}
XStrBuffSplit::~XStrBuffSplit()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

const XStrBuffSplit& XStrBuffSplit::operator=(const wchar_t* lpsz)
{
XEXCEPT_ENTRY
	if(m_cDelimiter == 0) {
		AllocNullNullTerminateW(lpsz);
		SpritNullNullTerminate();
	} else {
		XStrBuff::operator=(lpsz);
		Sprit();
	}
	return *this;
XEXCEPT_EXIT
}

const XStrBuffSplit& XStrBuffSplit::operator=(const char* lpsz)
{
XEXCEPT_ENTRY
	if(m_cDelimiter == 0) {
		AllocNullNullTerminateA(lpsz);
		SpritNullNullTerminate();
	} else {
		XStrBuff::operator=(lpsz);
		Sprit();
	}
	return *this;
XEXCEPT_EXIT
}
/**
 * スプリットされた文字配列の数を取得します。
 * @return スプリットされた文字配列の数
 */
int XStrBuffSplit::GetArgCouunt()  const
{
XEXCEPT_ENTRY
	return m_ArgCount;
XEXCEPT_EXIT
}
/**
 * スプリットされた文字配列を取得します。
 * @return スプリットされた文字配列
 */
const TCHAR** XStrBuffSplit::GetArgv()
{
XEXCEPT_ENTRY
	return (const TCHAR**)(char*)m_ArgBuff;
XEXCEPT_EXIT
}
const TCHAR* XStrBuffSplit::GetDivid(int nIndex)
{
XEXCEPT_ENTRY
	const TCHAR* *pData = GetArgv();
	return pData[nIndex];
XEXCEPT_EXIT
}
void XStrBuffSplit::SetIndentChar(TCHAR cIndeChar)
{
XEXCEPT_ENTRY
	m_cIndeChar = cIndeChar;
XEXCEPT_EXIT
}
void XStrBuffSplit::SetDelimite(TCHAR cDelimite)
{
	m_cDelimiter = cDelimite;
}
void XStrBuffSplit::SetNullCount(bool bIsNullCount)
{
XEXCEPT_ENTRY
	m_bIsNullCount = bIsNullCount;
XEXCEPT_EXIT
}
bool XStrBuffSplit::GetNullCount()
{
XEXCEPT_ENTRY
	return m_bIsNullCount;
XEXCEPT_EXIT
}

void XStrBuffSplit::CommounConstruct(TCHAR cDelimiter, TCHAR cIndeChar)
{
XEXCEPT_ENTRY
	m_cDelimiter = cDelimiter;
	m_cIndeChar    = cIndeChar;
	m_ArgCount   = -1;
	m_bIsNullCount = true;
XEXCEPT_EXIT
}

unsigned long XStrBuffSplit::AllocNullNullTerminateA(const char* lpszBuffOrg)
{
XEXCEPT_ENTRY
	unsigned long   dwSize = 0;
	const char* lpszBuff  = lpszBuffOrg;

	while( !(*lpszBuff == 0 && *(lpszBuff+1)== 0 ) ) {
		lpszBuff++;
		dwSize++;
	}
	dwSize += 2;
	
	m_pStrAddress = (TCHAR*)this->Alloc((dwSize+3) * sizeof(TCHAR));
	if( m_pStrAddress == NULL ) {
		return 0;
	}
	this->MemSet(0);
#ifdef _UNICODE
  #ifdef WIN32
	::MultiByteToWideChar(CP_ACP, 0, lpszBuffOrg,
		(dwSize+1) * sizeof(char) , m_pStrAddress, (dwSize+1)*sizeof(unsigned short) );
  #else
	mbstate_t ps;
    ::mbsnrtowcs(m_pStrAddress, &lpszBuffOrg, (dwSize+1) * sizeof(char), this->GetBuffSize(), &ps);
  #endif
#else
	::memcpy(m_pStrAddress, lpszBuffOrg, dwSize * sizeof(char));
#endif
	return dwSize;
XEXCEPT_EXIT
}


unsigned long XStrBuffSplit::AllocNullNullTerminateW(const wchar_t* lpszBuffOrg)
{
XEXCEPT_ENTRY
	unsigned long   dwSize = 0;
	const wchar_t* lpszBuff  = lpszBuffOrg;

	while( !(*lpszBuff == 0 && *(lpszBuff+1)== 0 ) ) {
		lpszBuff++;
		dwSize++;
	}
	dwSize += 2;
	
	m_pStrAddress = (TCHAR*)this->Alloc((dwSize+3) * sizeof(TCHAR));
	if( m_pStrAddress == NULL )  {
		return 0;
	}
	this->MemSet(0);
#ifdef _UNICODE
	::memcpy(m_pStrAddress, lpszBuffOrg, dwSize * sizeof(wchar_t));
#else
//  #ifdef WIN32
//	::WideCharToMultiByte(CP_ACP, 0, lpszBuffOrg,
//		(dwSize+1)*sizeof(wchar_t),	m_pStrAddress, (dwSize+1)*sizeof(char),	NULL, NULL);
//  #else
//	mbstate_t ps;
//	::wcsnrtombs(m_pStrAddress, &lpszBuffOrg, (dwSize+1) * sizeof(char), this->GetBuffSize(), &ps);
	size_t  real = this->GetBuffSize();
	char *pSrcString = m_pStrAddress;
	
	lpszBuff  = lpszBuffOrg;

	do {
		XStrBuff::__wcstombs(pSrcString, real, lpszBuff, this->GetBuffSize()-1);
		size_t nLen = ::wcslen(lpszBuff);
		lpszBuff += (nLen+1);
		pSrcString += (nLen+1);
		real -= (nLen+1);
		if( *lpszBuff== 0 ) {
			pSrcString++;
			*pSrcString = 0;
			break;
		}
	}while(1);
//  #endif
#endif
	return dwSize;
XEXCEPT_EXIT
}


int XStrBuffSplit::SpritNullNullTerminate()
{
XEXCEPT_ENTRY
	TCHAR* *pArgv;
	int    nSplitCount = 0;
	bool	bIndeChar = false;
	TCHAR* lpszBuff  = m_pStrAddress;
	TCHAR* lpszStart;

	m_ArgCount = 0;

	lpszStart = lpszBuff = (TCHAR*)*this;
	while( !(*lpszBuff == 0 && *(lpszBuff+1)== 0 ) )
	{
		if( *lpszBuff == 0 )
		{
			nSplitCount++;
		}
		lpszBuff++;
	}
	nSplitCount += 3;



	pArgv = (TCHAR**)m_ArgBuff.Alloc( sizeof(TCHAR*) * nSplitCount );
	if( pArgv == NULL )
		return 0;

	lpszStart = lpszBuff = (TCHAR*)*this;
	while( !(*lpszBuff == 0 && *(lpszBuff+1)== 0 ) )
	{
		if( *lpszBuff == 0 )
		{
			*pArgv++ = (TCHAR*)lpszStart;
			lpszStart = (lpszBuff+1);
			m_ArgCount++;
		}
		lpszBuff++;
	}
	if(*lpszStart != '\0')
	{
		*pArgv++ = (TCHAR*)lpszStart;
		m_ArgCount++;
	}
	*pArgv = NULL;
	return m_ArgCount;
XEXCEPT_EXIT
}
int XStrBuffSplit::Sprit()
{
XEXCEPT_ENTRY
	TCHAR*			lpszBuff = m_pStrAddress;
	TCHAR*			*pArgv;
	TCHAR*			p;
	int				nSplitCount = 0;
	unsigned int	nLength = this->GetLength();
	bool			bIndeChar = false;

	while( *lpszBuff ) {
		if( *lpszBuff == m_cDelimiter ) {
			nSplitCount++;
		}
		lpszBuff++;
	}
	nSplitCount += 10;


	pArgv = (TCHAR**)m_ArgBuff.Alloc( sizeof(TCHAR*) * nSplitCount );
	if( pArgv == NULL ) {
		return 0;
	}
	
	m_ArgCount = 0;
	p = lpszBuff = m_pStrAddress;

	if(*lpszBuff == m_cIndeChar) {
		bIndeChar = true;
	}

	while(*lpszBuff != '\0') {
		if( bIndeChar == true ) {
			int len = XStrBuff::StrLen(lpszBuff);
			::memmove(lpszBuff, lpszBuff+1, (len+1)*sizeof(TCHAR));
			while(*lpszBuff != '\0') {
				if(*lpszBuff == m_cIndeChar) {
					len = XStrBuff::StrLen(lpszBuff);
					::memmove(lpszBuff, lpszBuff+1, (len+1)*sizeof(TCHAR));
					if( *lpszBuff != m_cIndeChar ) {
						break;
					}
				}
				lpszBuff++;
			}
		}
		while( (*lpszBuff != m_cDelimiter) && (*lpszBuff != '\0')) {
			*lpszBuff++;
		}
		if(*lpszBuff != '\0') {
			*lpszBuff++ = '\0';
			if( !(*p == 0 && m_bIsNullCount == false) ) {
				*pArgv++ = p;
				m_ArgCount++;
			}
			
			p = lpszBuff;

			/* 最後がデミリタのみで終わっている時の対応 */
			if(*lpszBuff == '\0' && !(m_bIsNullCount == false) ) {
				*pArgv++ = NULL;
				m_ArgCount++;
			}
		} else {
			if( !(*p == 0 && m_bIsNullCount == false) ) {
				*pArgv++ = p;
				m_ArgCount++;
			}
			break;
		}
		bIndeChar = false;
		if(*lpszBuff == m_cIndeChar) {
			bIndeChar = true;
		}
	}
	*pArgv = NULL;
	return m_ArgCount;
XEXCEPT_EXIT
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// XInflateStrstream
//
using namespace std;
XInflateStrstream::XInflateStrstream()
#ifdef _UNICODE
	: wiostream(&m_buff)
#else
	: iostream(&m_buff)
#endif
	, m_buff(128, 256)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XInflateStrstream::XInflateStrstream(unsigned long nInitSize, unsigned long nIncrementSize /*= 1024*/)
#ifdef _UNICODE
	: wiostream(&m_buff)
#else
	: iostream(&m_buff)
#endif
	, m_buff(nInitSize, nIncrementSize)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XInflateStrstream::~XInflateStrstream()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
#ifdef _UNICODE
XInflateStreamBuffW *
#else
XInflateStreamBuff *	
#endif
	XInflateStrstream::rdbuf() const
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return (XInflateStreamBuffW*)&m_buff;
#else
	return (XInflateStreamBuff*)&m_buff;
#endif
XEXCEPT_EXIT
}
TCHAR *	XInflateStrstream::str()
{
XEXCEPT_ENTRY
	return (TCHAR*)m_buff.toString();
XEXCEPT_EXIT
}
streamsize XInflateStrstream::pcount()
{
XEXCEPT_ENTRY
	return (streamsize)m_buff.size();
XEXCEPT_EXIT
}
void XInflateStrstream::resetp()
{
XEXCEPT_ENTRY
	m_buff.resetp();
XEXCEPT_EXIT
}