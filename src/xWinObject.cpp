#include "xlib/windows/xWinObject.h"
#ifdef WIN32

XWinObject::XWinObject(HANDLE hInitObject /*= NULL*/)
{
	m_hObject      = hInitObject;
	m_hValidObject = hInitObject;
}
XWinObject::~XWinObject()
{
	Close();
}
void XWinObject::Close()
{
	if( m_hValidObject != m_hObject ) {
		::CloseHandle(m_hObject);
	}
	m_hObject = m_hValidObject;
}
bool XWinObject::IsValidate()
{
	return m_hObject == m_hValidObject ? false : true;
}

#endif

