#include "xlib/windows/xWinService.h"

#ifdef WIN32

/////////////////////////////////////////////////////////////////////////////////////////////
//
//	XWinSCManager
//
XWinSCManager::XWinSCManager()
	:	XWinObject(NULL)
{
	m_dwAccessMode = 0;
	m_strDataBase = _T("");
	m_strMachine = _T("");
}
XWinSCManager::~XWinSCManager()
{

}
void XWinSCManager::Close()
{
	if( m_hObject != m_hValidObject ) {
		::CloseServiceHandle( (SC_HANDLE)m_hObject );
		m_hObject = m_hValidObject;
	}
}
bool XWinSCManager::Open(DWORD dwSCAccess/* =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ */,
			const char* lpszMachine /*= NULL*/, 
			const char* lpszDataBase /*= NULL*/)
{
	this->Close();
	m_hObject = ::OpenSCManagerA(lpszMachine, lpszDataBase, dwSCAccess);
	if( this->IsValidate() == true ) {
		
		if( lpszMachine == NULL ) {			m_strMachine = _T("");
		} else {			m_strMachine = lpszMachine;		}
		
		if( lpszDataBase == NULL ) {			m_strDataBase = _T("");
		} else {			m_strDataBase = lpszDataBase;		}

		m_dwAccessMode = dwSCAccess;
	}
	return this->IsValidate();
}
bool XWinSCManager::Open(DWORD dwSCAccess/* =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ */,
			const wchar_t* lpszMachine /*= NULL*/, 
			const wchar_t* lpszDataBase /*= NULL*/)
{
	this->Close();
	m_hObject = ::OpenSCManagerW(lpszMachine, lpszDataBase, dwSCAccess);
	if( this->IsValidate() == true ) {
		
		if( lpszMachine == NULL ) {			m_strMachine = _T("");
		} else {			m_strMachine = lpszMachine;		}
		
		if( lpszDataBase == NULL ) {			m_strDataBase = _T("");
		} else {			m_strDataBase = lpszDataBase;		}

		m_dwAccessMode = dwSCAccess;
	}
	return this->IsValidate();
}
/////////////////////////////////////////////////////////////////////////////////////////////
//
//	XWinServiceCtrl
//
XWinServiceCtrl::XWinServiceCtrl()
{
	m_hSCManager     = NULL;
	m_hService		 = NULL;
	m_strServiceName = _T("");
	m_dwID           = 0;
}
XWinServiceCtrl::~XWinServiceCtrl()
{
	Close();
}
XStrBuff XWinServiceCtrl::GetServiceName() const
{
	XStrBuff str(m_strServiceName);
	return str;
}
DWORD XWinServiceCtrl::GetServiceSpecificExitCode() const
{
	return m_dwServiceSpecificExitCode;
}
HANDLE XWinServiceCtrl::GetSCManagerHandle()
{
	return m_hSCManager;
}

bool XWinServiceCtrl::OpenSCManager(DWORD dwSCAccess/* =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ */,
			const char* lpszMachine/* = NULL*/, 
			const char* lpszDataBase/* = NULL*/)
{
	if(m_hSCManager != NULL) {
		::CloseServiceHandle(m_hSCManager);
		m_hSCManager = NULL;
	}

	m_hSCManager = ::OpenSCManagerA(lpszMachine, lpszDataBase, dwSCAccess);
	if(!m_hSCManager) {
		return false;
	}
	return true;
}
bool XWinServiceCtrl::OpenSCManager(DWORD dwSCAccess /*=  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ */,
			const wchar_t* lpszMachine /*= NULL*/, 
			const wchar_t* lpszDataBase /*= NULL*/)
{
	if(m_hSCManager != NULL) {
		::CloseServiceHandle(m_hSCManager);
		m_hSCManager = NULL;
	}

	m_hSCManager = ::OpenSCManagerW(lpszMachine, lpszDataBase, dwSCAccess);
	if(!m_hSCManager) {
		return false;
	}
	return true;

}
bool XWinServiceCtrl::Open(	const char* lpszServiceName,
		    DWORD dwAccess /*= SERVICE_ALL_ACCESS */,
			DWORD dwSCAccess /*=  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ */,
			const char* lpszMachine /*= NULL*/, 
			const char* lpszDataBase /*= NULL*/)
{
	this->Close();
	if( this->OpenSCManager(dwSCAccess, lpszMachine, lpszDataBase) == false ) {
		return false;
	}
	m_hService = ::OpenServiceA(m_hSCManager, lpszServiceName, dwAccess);
	if( m_hService == NULL ) {
		return false;
	}
	m_dwAccess = dwAccess;
	m_strServiceName = lpszServiceName;
	return true;
}
bool XWinServiceCtrl::Open(	const wchar_t* lpszServiceName,
		    DWORD dwAccess /*= SERVICE_ALL_ACCESS */,
			DWORD dwSCAccess /*=  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ */,
			const wchar_t* lpszMachine /*= NULL*/, 
			const wchar_t* lpszDataBase /*= NULL*/)
{
	this->Close();
	if( this->OpenSCManager(dwSCAccess, lpszMachine, lpszDataBase) == false ) {
		return false;
	}
	m_hService = ::OpenServiceW(m_hSCManager, lpszServiceName, dwAccess);
	if( m_hService == NULL ) {
		return false;
	}
	m_dwAccess = dwAccess;
	m_strServiceName = lpszServiceName;
	return true;
}




#endif // WIN32

