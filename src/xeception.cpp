#ifdef WIN32
   #include <windows.h>
   #include <winbase.h>
   #include <new.h>
#endif
#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"

void XTRACE(const TCHAR* lpszFormat, ...)
{
#ifdef _DEBUG
	XStrBuff str;
	va_list args;
	va_start(args, lpszFormat);

	str.FormatV(lpszFormat, args);
	va_end(args);

 #ifdef WIN32
#ifdef UNICODE
	::OutputDebugStringW((const wchar_t*)str);
#else
	::OutputDebugStringA((const char*)str);
#endif // UNICODE	
 #else
	printf("%s", (const char*)str);
 #endif
#endif // _DEBUG	
}


#if _MSC_VER >= 1400 // VC2005(VC8.0)以降
	void XlibTranslateUt_StructuredException(unsigned int ExceptionCode, PEXCEPTION_POINTERS ExceptionPointers)
	{
		XEXCEPT_THROW(XExcept_STRUCTURED_EXCEPT, 0, 0);
	}
	void XlibTranslateInvalidParameterException(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
	{
		XEXCEPT_THROW(XExcept_UNKNOWN_PARAM, 0, 0);
	}
#endif



#ifdef WIN32
#ifdef _DEBUG
void XAssertFailedLine(LPCSTR lpszFileName, int nLine)
{
	_CrtDbgReport(_CRT_ASSERT, lpszFileName, nLine, NULL, NULL);
}
#endif // _DEBUG	
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1200
	int __XExcept__ThrowBadAlloc( size_t size )
	{
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, size);
	}
#endif
void XExcept::setThrowBadAlloc()
{
#if defined(_MSC_VER) && _MSC_VER <= 1200
	/*mallocもnewと同じハンドラを呼ぶよ*/
	_set_new_mode(1);

	/*メモリが確保できなかったらThrowBadAllocハンドラを呼ぶよ*/
	_set_new_handler(__XExcept__ThrowBadAlloc);
#endif
}
XStrBuff XExcept::getSrdErrorStr(errno_t err)
{
	XStrBuff str;
	
#if _MSC_VER >= 1310
	char szBuff[512] = {0};
	strerror_s(szBuff, sizeof(szBuff), err);
	str = szBuff;
#else
	str = ::strerror(err);
#endif
	return str;
}


XExcept::XExcept(XExceptCATEGORY category, const char* file, int line, unsigned long ulOsErrorCode /*= 0*/, unsigned long ulHint  /*= 0*/)
{
	m_what = "";
	p_strWat = m_what;
	addStackTrace(category, file, line, ulOsErrorCode, ulHint);
}
XExcept::XExcept(XExceptCATEGORY category, const char* p_what, const char* file, int line, unsigned long ulOsErrorCode /*= 0*/, unsigned long ulHint  /*= 0*/)
{
	m_what = p_what;
	p_strWat = m_what;
	addStackTrace(category, file, line, ulOsErrorCode, ulHint);
}
XExcept::XExcept(XExceptCATEGORY category, const wchar_t* p_what, const char* file, int line, unsigned long ulOsErrorCode /*= 0*/, unsigned long ulHint  /*= 0*/)
{
	m_what = p_what;
	p_strWat = m_what;
	addStackTrace(category, file, line, ulOsErrorCode, ulHint);
}
XExcept::~XExcept() throw ()
{
	try {
		std::vector<XExceptStackItem*>::iterator ite = m_stacItms.begin();
		for(; ite != m_stacItms.end(); ite++) {
			delete *ite;
		}
		m_stacItms.clear();
	} catch(...) {
		// ignore
	}
}
const char* XExcept::what() const throw ()
{
	return p_strWat;
}
const wchar_t* XExcept::whatW() throw ()
{
	return m_what;
}
const TCHAR* XExcept::whatT() throw ()
{
#ifdef _UNICODE
	return whatW();
#else
	return what();
#endif
}
void XExcept::addStackTrace(XExceptCATEGORY category, const char* file, int line, unsigned long ulOsErrorCode /*= 0*/, unsigned long ulHint  /*= 0*/, const TCHAR* pErrMsg /*= NULL*/)
{
	if( m_stacItms.size() == 0 ) {
		m_catgegory = category;
		m_nOsErrorCode = ulOsErrorCode;
		m_ulHint = ulHint;
	}
	XExceptStackItem *pNewItem = NULL;
	
	pNewItem =new XExceptStackItem(category, file, line, ulOsErrorCode, ulHint, pErrMsg); 

	m_stacItms.push_back( pNewItem );
}
void XExcept::print(XLIB_STD_OUT_STR_STREAM& out /*= std::cerr*/)
{
	if( m_what.GetLength() != 0 ) {
		out << (const TCHAR*)this->whatT() << std::endl;
	}
	if( m_nOsErrorCode != 0 ) {
		out << _T("OSErrorCode:") <<  m_nOsErrorCode << std::endl;
	}
	if( m_ulHint != 0 ) {
		out << _T("Hint:") <<  m_ulHint << std::endl;
	}
//	this->printStackTrace(out);
}
void XExcept::printStackTrace(XLIB_STD_OUT_STR_STREAM& out /*= std::cerr*/)
{
	XExceptStackItems::iterator	ite	= m_stacItms.begin();

	for(; ite != m_stacItms.end(); ite++) {
		switch( (*ite)->m_catgegory ) {
		case XExcept_UNKNOWN:
			out << _T("UNKNOWN  ");
			break;
		case XExcept_STRUCTURED_EXCEPT:
			out << _T("STRUCTURED_EXCEPT  ");
			break;
		case XExcept_UNKNOWN_PARAM:
			out << _T("UNKNOWN_PARAM  ");
			break;
		case XExcept_OVER_INDEX:
			out << _T("OVER_INDEX  ");
			break;
		case XExcept_RETHROW:
			out << _T("RETHROW  ");
			break;
		case XExcept_MEMORY_ALLOC:
			out << _T("MEMORY_ALLOC  ");
			break;
		case XExcept_NO_SUPPORT:
			out << _T("NO_SUPPORT  ");
			break;
		case XExcept_RESOURCE:
			out << _T("RESOURCE  ");
			break;
		case XExcept_ARCHIVE:
			out << _T("ARCHIVE  ");
			break;
		case XExcept_FILE:
			out << _T("FILE  ");
			break;
		case XExcept_XDATA:
			out << _T("XDATA  ");
			break;
		case XExcept_SECS:
			out << _T("SECS  ");
			break;
		case XExcept_SQLite:
			out << _T("SQL  ");
			break;
		case XExcept_APP:
			out << _T("Application  ");
			break;
		default:
			out << _T("???????  ");
			break;
		}
		bool bOtherMsg = false;
		XStrBuff strSourceFile((*ite)->m_file);
		out << (const TCHAR*)strSourceFile << _T("(") << std::dec << (*ite)->m_line << _T(")") << std::endl;
		if( (*ite)->m_what.GetLength() != 0 ) {
			out << _T("  [") << (const TCHAR*)(*ite)->m_what << _T("]");
		}
		if( (*ite)->m_ulHint != 0 ) {
			out << _T("  Hint:[") << (*ite)->m_ulHint << _T("]");
		}
		if( (*ite)->m_ulOsErrorCode != 0 ) {
			out << _T("  OSerr:[") << (*ite)->m_ulOsErrorCode << _T("]");
		}
		if( bOtherMsg == true ) {
			out << std::endl;
		}
	}
}	

unsigned long XExcept::getOsErrorCode() const
{
	return m_nOsErrorCode;
}
unsigned long XExcept::getHint() const
{
	return m_ulHint;
}
void XExcept::setHist(const unsigned long ulHint)
{
	m_ulHint = ulHint;
}
XExceptCATEGORY XExcept::getCategory() const
{
	return m_catgegory;
}

void XExceptInvalidParam(const char* p_func, const char* p_f_name, int line)
{
	throw XExcept(XExcept_UNKNOWN_PARAM, p_func, p_f_name, line, 0, 0);
}