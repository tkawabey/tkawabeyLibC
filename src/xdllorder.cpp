#include "xlib/xeception.h"
#include "xlib/xdllorder.h"
#ifdef WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

XDlLorder::XDlLorder()
	:	m_pHandle( NULL )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XDlLorder::~XDlLorder()
{
XEXCEPT_ENTRY
	close();
XEXCEPT_EXIT
}

bool XDlLorder::open(const char* filename, int flag /*= 0*/)
{
XEXCEPT_ENTRY

	// 既にオープンしていた場合の対策
	close();

#ifdef WIN32
	m_pHandle = (void*)LoadLibraryExA(filename, NULL, flag);
	if( m_pHandle == NULL ) {
		return false;
	}
	return true;
#else
	if( flag == 0 ) {
		flag = RTLD_LAZY;
	}
	m_pHandle = dlopen(filename, flag);
	if( m_pHandle == NULL ) {
		return false;
	}
	return true;
#endif
XEXCEPT_EXIT
}

bool XDlLorder::open(const wchar_t* filename, int flag /*= 0*/)
{
XEXCEPT_ENTRY

	// 既にオープンしていた場合の対策
	close();

#ifdef WIN32
	m_pHandle = (void*)LoadLibraryExW(filename, NULL, flag);
	if( m_pHandle == NULL ) {
		return false;
	}
	return true;
#else
	if( flag == 0 ) {
		flag = RTLD_LAZY;
	}
	m_pHandle = dlopen(XStrBuff(filename), flag);
	if( m_pHandle == NULL ) {
		return false;
	}
	return true;
#endif
XEXCEPT_EXIT
}

bool XDlLorder::close()
{
XEXCEPT_ENTRY
#ifdef WIN32
	if( m_pHandle != NULL ) {
		::FreeLibrary((HMODULE)m_pHandle);
	}
#else
	if( m_pHandle != NULL ) {
		::dlclose(m_pHandle);
	}
#endif
	m_pHandle = NULL;
	return true;
XEXCEPT_EXIT
}


void* XDlLorder::loadSymbol(const char* symbol)
{
XEXCEPT_ENTRY
	void* pFunc = NULL;
	if( m_pHandle == NULL ) {
		return pFunc;
	}
#ifdef WIN32
	pFunc = ::GetProcAddress((HMODULE)m_pHandle, symbol);
#else
	pFunc = dlsym(m_pHandle, symbol);
#endif
	return pFunc;
XEXCEPT_EXIT
}


void* XDlLorder::loadSymbol(const wchar_t* symbol)
{
XEXCEPT_ENTRY
	void* pFunc = NULL;
	if( m_pHandle == NULL ) {
		return pFunc;
	}
#ifdef WIN32
	pFunc = ::GetProcAddress((HMODULE)m_pHandle, XStrBuff(symbol));
#else
	pFunc = dlsym(m_pHandle, symbol);
#endif
	return pFunc;
XEXCEPT_EXIT
}












