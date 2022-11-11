#include "xlib/windows/xWinRegistry.h"
#include "xlib/xeception.h"
#ifdef WIN32

XRegistry::XRegistry()
	:	m_hOenKey((HKEY)INVALID_HANDLE_VALUE)
	,	m_hDefKey((HKEY)INVALID_HANDLE_VALUE)
	,	m_bOpenSrv(false)
{
	m_dwLastError        = 0;
	m_RegSam             = 0;
}

XRegistry::~XRegistry()
{
	FreeKeys();
	FreeValues();
	Close();
}
bool XRegistry::IsValidObject()
{
	return m_hOenKey != INVALID_HANDLE_VALUE ? true : false;
}
bool XRegistry::Open(HKEY hKey, LPCTSTR lpszSubKey, REGSAM samDesired)
{
	if (IsValidObject()) {
		Close();
	}
	m_hDefKey = hKey;
	m_strSubKeyPath = lpszSubKey;
	_SetKeyName();
	return Open(samDesired);
}
bool XRegistry::ReOpen(REGSAM samDesired/* = KEY_READ*/)
{
	XStrBuff strSubKeyTmp = m_strSubKeyPath;
	return this->Open(m_hDefKey, (LPCTSTR)strSubKeyTmp, samDesired);
}
bool XRegistry::Open(REGSAM samDesired)
{
	_ASSERT(m_hDefKey != INVALID_HANDLE_VALUE );
	m_RegSam = samDesired;

	if(m_hDefKey != HKEY_PERFORMANCE_DATA ) {
		m_dwLastError = ::RegOpenKeyEx(m_hDefKey, m_strSubKeyPath, 0, samDesired, (HKEY*)&m_hOenKey);
		HANDLE h = m_hOenKey;
		if( ERROR_SUCCESS == m_dwLastError) {
			return true;
		}
	}
	else
	{
		m_hOenKey = (HKEY)HKEY_PERFORMANCE_DATA;
		return true;
	}
	return false;
}
bool XRegistry::OpenServerKey(LPCTSTR	lpszSrv, 
							  HKEY		hKey, 
							  LPCTSTR	lpszSubKey, 
							  REGSAM	samDesired)
{
	Close();
	m_dwLastError = ::RegConnectRegistry(lpszSrv, hKey , &m_hDefKey);
	if( ERROR_SUCCESS != m_dwLastError)
		return false;

	m_strSubKeyPath = lpszSubKey;
	if( Open(samDesired) )
		return (m_bOpenSrv = true);
	else
		return false;
}
bool XRegistry::Close(void)
{
	if (IsValidObject())
	{
		if(m_hDefKey != HKEY_PERFORMANCE_DATA )
		{
			::RegCloseKey((HKEY)m_hOenKey);
		}
		m_hOenKey = (HKEY)INVALID_HANDLE_VALUE;
	}
	if(m_bOpenSrv)
	{
		_ASSERT(m_hDefKey != INVALID_HANDLE_VALUE );
		::RegCloseKey((HKEY)m_hDefKey);
		m_hDefKey = (HKEY)INVALID_HANDLE_VALUE;
	}
	return true;
}

bool XRegistry::FreeKeys()
{
	int i;
	for(i = 0; i < m_arrayKeys.GetSize(); i++)
	{
		XRegistry *p = m_arrayKeys[i];
		delete p;
	}
	m_arrayKeys.RemoveAll();
	return true;
}
bool XRegistry::FreeValues()
{
	int i;
	for(i = 0; i < m_arrayValues.GetSize(); i++)
	{
		XRegValue *p = m_arrayValues[i];
		delete p;
	}
	m_arrayValues.RemoveAll();
	return true;
}
bool XRegistry::FillToKeys()
{
	XRegInfo  info;
	LPCTSTR   lpszSubKey = m_strSubKeyPath;
	XStrBuff  strSubKey;
	DWORD     dwResum = 0;
	DWORD     dwKeySize;
	LPTSTR    pKeyName;
	XStrBuff  buffKey;
	FILETIME  ftLastWriteTime;

	// オープンされているか調べます
	if( !this->IsValidObject() )
	{
		m_dwLastError = ERROR_INVALID_HANDLE;
		return false;
	}

	// オープンしたアクセス権があるか調べなければリオープンします
	if( ( m_RegSam & (KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE) ) == 0 )
	{
		REGSAM regSamTmp = m_RegSam | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE;
		if( this->ReOpen(regSamTmp) == false )
		{
			return false;
		}
	}
	// キー情報を取得します
	if( this->RegQueryInfoKey(info) == false)
	{
		return false;
	}

	// メモリを確保します
	pKeyName   = (LPTSTR)buffKey.Alloc(info.dwMaxKeyNameSize+100);
	if(pKeyName == NULL)
	{
		m_dwLastError = ERROR_NOT_ENOUGH_MEMORY;
		return false;
	}

	FreeKeys();

	for(dwResum = 0; dwResum < info.dwSubKeys; dwResum++)
	{
		dwKeySize   = info.dwMaxKeyNameSize+100;
		m_dwLastError = ::RegEnumKeyEx((HKEY)m_hOenKey, 
								 dwResum, 
								 pKeyName, 
								 &dwKeySize, 
								 NULL, 
								 NULL, 
								 NULL, 
								 &ftLastWriteTime);
		
		if(m_dwLastError != ERROR_SUCCESS) 
		{
			return false;
		}

		XRegistry *pNew = new XRegistry();
		if( pNew == NULL )
		{
			m_dwLastError = ERROR_NOT_ENOUGH_MEMORY;
			return false;
		}
		if( lstrlen(lpszSubKey))
		{
			strSubKey.Format(_T("%s\\%s"), lpszSubKey, (LPCTSTR)pKeyName);
		}
		else
		{
			strSubKey = (LPCTSTR)pKeyName;
		}
		if( pNew->Open(m_hDefKey, strSubKey) )
			m_arrayKeys.Add(pNew);
		else
			delete pNew;
	}
	return true;
}
bool XRegistry::FillToValues()
{
	XRegInfo  info;
	LPCTSTR   lpszSubKey = m_strSubKeyPath;
	XStrBuff  strSubKey;
	DWORD     dwResum = 0;
	DWORD     dwType;
	DWORD     dwValueNameSize;
	LPTSTR    pValueName;
	XStrBuff  buffValueName;
	DWORD     dwValueDataSize;
	LPTSTR    pValueData;
	XStrBuff  buffValueData;

	// オープンされているか調べます
	if( !this->IsValidObject() )
	{
		m_dwLastError = ERROR_INVALID_HANDLE;
		return false;
	}

	// オープンしたアクセス権があるか調べなければリオープンします
	if( ( m_RegSam & KEY_QUERY_VALUE ) == 0 )
	{
		REGSAM regSamTmp = m_RegSam | KEY_QUERY_VALUE;
		if( this->ReOpen(regSamTmp) == false )
		{
			return false;
		}
	}
	// キー情報を取得します
	if( this->RegQueryInfoKey(info) == false)
	{
		return false;
	}

	pValueName   = (LPTSTR)buffValueName.Alloc(info.dwMaxValueNameSize+100);
	pValueData   = (LPTSTR)buffValueData.Alloc(info.dwMaxValueDataSize+100);
	if(pValueName == NULL || pValueData == NULL)
	{
		m_dwLastError = ERROR_NOT_ENOUGH_MEMORY;
		return false;
	}

	FreeValues();

	for(dwResum = 0; dwResum < info.dwValues; dwResum++)
	{
		dwValueNameSize = info.dwMaxValueNameSize+100;
		dwValueDataSize = info.dwMaxValueDataSize+100;

		buffValueName.MemSet(0);
		buffValueData.MemSet(0);

		m_dwLastError = ::RegEnumValue((HKEY)m_hOenKey, 
								 dwResum, 
								 pValueName, 
								 &dwValueNameSize, 
								 NULL, 
								 &dwType, 
								 (LPBYTE)pValueData, 
								 &dwValueDataSize);
		if(m_dwLastError != ERROR_SUCCESS) 
		{
			return false;
		}

		XRegValue *pVal = new XRegValue();
		if( pVal == NULL )
		{
			m_dwLastError = ERROR_NOT_ENOUGH_MEMORY;
			return false;
		}
		LPTSTR pData = (LPTSTR)pVal->data.Alloc(dwValueDataSize+1);
		if(pData == NULL)
		{
			m_dwLastError = ERROR_NOT_ENOUGH_MEMORY;
			delete pVal;
			return false;
		}
		pVal->strValueName = pValueName;
		memcpy(pData, pValueData, dwValueDataSize);
		pVal->dwSize       = dwValueDataSize;
		pVal->dwType       = dwType;
		m_arrayValues.Add(pVal);
	}
	return true;
}
int XRegistry::CountSubKeys()
{
	return m_arrayKeys.GetSize();
}
int XRegistry::CountValues()
{
	return m_arrayValues.GetSize();
}
XRegistry *XRegistry::GetSubKey(int index)
{
	return m_arrayKeys[index];
}
XRegValue *XRegistry::GetValue(int index)
{
	return m_arrayValues[index];
}
bool XRegistry::RegQueryInfoKey(XRegInfo &info)
{
	TCHAR    ClassName[MAX_PATH] = _T("");
	DWORD    dwcClassLen = MAX_PATH;
	FILETIME ftm;

	// オープンしたアクセス権があるか調べなければリオープンします
	if( ( m_RegSam & KEY_QUERY_VALUE ) == 0 )
	{
		REGSAM regSamTmp = m_RegSam | KEY_QUERY_VALUE;
		if( this->ReOpen(regSamTmp) == false )
		{
			return false;
		}
	}

	m_dwLastError = ::RegQueryInfoKey((HKEY)m_hOenKey,
							      ClassName,
								  &dwcClassLen,
								  NULL,
								  &info.dwSubKeys,
								  &info.dwMaxKeyNameSize,
								  &info.dwMaxClassNameSize,
								  &info.dwValues,
								  &info.dwMaxValueNameSize,
								  &info.dwMaxValueDataSize,
								  &info.dwMaxSecSize,
								  &ftm);
	if(m_dwLastError == ERROR_SUCCESS) 
	{
		info.strClassName = ClassName;
		info.tmLastWrite  = ftm;
		return true;
	}
	return false;
}
DWORD XRegistry::Size(LPCTSTR lpszVal)
{
	DWORD dwSize;
	m_dwLastError = ::RegQueryValueEx((HKEY)m_hOenKey, lpszVal, NULL, NULL, NULL, &dwSize);
	if(m_dwLastError == ERROR_SUCCESS) 
	{
		return dwSize;
	} else {
		return -1;
	}
}
DWORD XRegistry::GetLastError() const
{
	return m_dwLastError;
}
bool XRegistry::Read(LPCTSTR lpszVal, LPDWORD lpbData)
{
	DWORD dwData = sizeof(DWORD);
	return Read(lpszVal, NULL, (LPBYTE)lpbData, &dwData);
}
bool XRegistry::Read(LPCTSTR lpszVal, XStrBuff* pStr)
{
	DWORD dwSize = Size(lpszVal);
	if (dwSize != -1)
	{
		LPBYTE pPtr;
		pPtr = (LPBYTE)pStr->Alloc(dwSize*sizeof(TCHAR));
		return  Read(lpszVal, NULL, pPtr, &dwSize);
	}
	else
	{
		return false;
	}
}
bool XRegistry::Read(LPCTSTR lpszVal, XRegValue* pVal)
{
	DWORD dwSize = this->Size(lpszVal);
	if(dwSize == -1)
	{
		return false;
	}
	LPBYTE pData = (LPBYTE)pVal->data.Alloc(dwSize);
	if(pData == NULL)
	{
		return false;
	}
	pVal->strValueName = lpszVal;
	pVal->dwSize = dwSize;

	return this->Read(lpszVal, &pVal->dwType, pData, &pVal->dwSize);
}
// protected:
bool XRegistry::Read(LPCTSTR lpszVal, LPDWORD lpdwType, LPBYTE lpbData, LPDWORD lpcbData)
{
	// オープンされているか調べます
	if( !this->IsValidObject() )
	{
		m_dwLastError = ERROR_INVALID_HANDLE;
		return false;
	}
	// オープンしたアクセス権があるか調べなければリオープンします
	if( ( m_RegSam & KEY_QUERY_VALUE ) == 0 )
	{
		REGSAM regSamTmp = m_RegSam | KEY_QUERY_VALUE;
		if( this->ReOpen(regSamTmp) == false )
		{
			return false;
		}
	}


	m_dwLastError = ::RegQueryValueEx((HKEY)m_hOenKey, lpszVal, NULL, lpdwType, lpbData, lpcbData);
	if( ERROR_SUCCESS == m_dwLastError)
		return true;
	else
		return false;
}


bool XRegistry::Write(LPCTSTR lpszVal, DWORD dwData)
{
	return Write(lpszVal, REG_DWORD, (CONST BYTE*)&dwData, sizeof(DWORD));
}
bool XRegistry::Write(LPCTSTR lpszVal, LPCTSTR lpszStr)
{
	return Write(lpszVal, REG_SZ, (CONST BYTE*)lpszStr, lstrlen(lpszStr)+1);
}
bool XRegistry::Write(XRegValue* pVal)
{
	DWORD dwSize;

	switch(pVal->dwType)
	{
	case REG_DWORD:
		dwSize = sizeof(DWORD);
		break;
	case REG_SZ:
	case REG_EXPAND_SZ:
	case REG_MULTI_SZ:
		dwSize = lstrlen(pVal->data)+sizeof(TCHAR);
		break;
	default:
		dwSize = pVal->dwSize;
	}
	return this->Write(pVal->strValueName, pVal->dwType, (CONST BYTE*)(void*)pVal->data, dwSize);
}
bool XRegistry::Write(LPCTSTR lpszVal, DWORD fdwType, CONST BYTE* lpbData, DWORD cbData)
{
	// オープンされているか調べます
	if( !this->IsValidObject() )
	{
		m_dwLastError = ERROR_INVALID_HANDLE;
		return false;
	}


	// オープンしたアクセス権があるか調べなければリオープンします
	if( ( m_RegSam & KEY_SET_VALUE ) == 0 )
	{
		REGSAM regSamTmp = m_RegSam | KEY_SET_VALUE;
		if( this->ReOpen(regSamTmp) == false )
		{
			return false;
		}
	}

	m_dwLastError = ::RegSetValueEx((HKEY)m_hOenKey, lpszVal, 0, fdwType, lpbData, cbData);
	if( ERROR_SUCCESS == m_dwLastError)
		return true;
	else
		return false;
}








bool XRegistry::Create(HKEY hKey, LPCTSTR lpszSubKey, REGSAM samDesired, LPDWORD lpdwDisp, LPCTSTR lpszClassName, DWORD dwOption)
{
	// オープンされているか調べます
	if (IsValidObject() )
	{
		Close();
	}
	DWORD dwDisp;
	if (lpdwDisp == NULL) 
	{
		lpdwDisp = &dwDisp;
	}
	m_hDefKey = hKey;
	m_strSubKeyPath = lpszSubKey;
	_SetKeyName();
	return Create(samDesired, lpdwDisp, lpszClassName, dwOption);
}
XRegistry *XRegistry::Create(LPCTSTR lpszSubKey, REGSAM samDesired , LPDWORD lpdwDisp , LPCTSTR lpszClassName, DWORD dwOption)
{
	XRegistry *pNew = new XRegistry();
	XStrBuff  strTmpSubKey;

	DWORD dwDisp;
	if (lpdwDisp == NULL) 
	{
		lpdwDisp = &dwDisp;
	}
	if(pNew == NULL)
	{
		return NULL;
	}

	if(m_strSubKeyPath.GetLength())
	{
		strTmpSubKey.Format(_T("%s\\%s"), (LPCTSTR)m_strSubKeyPath, lpszSubKey);
	}
	else
	{
		strTmpSubKey = lpszSubKey;
	}


	if( pNew->Create(m_hDefKey, strTmpSubKey,samDesired, lpdwDisp, lpszClassName, dwOption) == false )
	{
		m_dwLastError = pNew->GetLastError();
		delete pNew;
		return NULL;
	}
	m_arrayKeys.Add(pNew);
	return pNew;
}

bool XRegistry::Create(REGSAM samDesired, LPDWORD lpdwDisp, LPCTSTR lpszClassName, DWORD dwOption)
{
	_ASSERT(m_hDefKey != INVALID_HANDLE_VALUE );
	DWORD dwDisp;
	if (lpdwDisp == NULL)
	{
		lpdwDisp = &dwDisp;
	}

	m_RegSam = samDesired;

	m_dwLastError = ::RegCreateKeyEx(m_hDefKey, m_strSubKeyPath, 0, (LPTSTR)lpszClassName, dwOption, samDesired,
						NULL, (HKEY*)&m_hOenKey, lpdwDisp);
	if( ERROR_SUCCESS == m_dwLastError)
		return true;
	else
		return false;
}


bool XRegistry::DeleteKey(void)
{
	Close();
	m_dwLastError = ::RegDeleteKey(m_hDefKey, m_strSubKeyPath);
	if( ERROR_SUCCESS == m_dwLastError)
		return true;
	else
		return false;
}
bool XRegistry::DeleteKey(HKEY hKey, LPCTSTR lpszSubKey)
{
	DWORD dwLastError = ::RegDeleteKey(hKey, lpszSubKey);
	if( ERROR_SUCCESS == dwLastError)
		return true;
	else
		return false;
}
bool XRegistry::Delete(LPCTSTR lpszVal)
{
	// オープンしたアクセス権があるか調べなければリオープンします
	if( ( m_RegSam & KEY_SET_VALUE ) == 0 )
	{
		REGSAM regSamTmp = m_RegSam | KEY_SET_VALUE;
		if( this->ReOpen(regSamTmp) == false )
		{
			return false;
		}
	}

	m_dwLastError = ::RegDeleteValue((HKEY)m_hOenKey, lpszVal);
	if( ERROR_SUCCESS == m_dwLastError)
		return true;
	else
		return false;
}


void XRegistry::_SetKeyName()
{
	LPCTSTR lpszOrg;
	lpszOrg = m_lpszKeyName = m_strSubKeyPath;	
	if( lstrlen(m_lpszKeyName) )
	{
		m_lpszKeyName += (lstrlen(m_lpszKeyName) -1);
		while(m_lpszKeyName != lpszOrg)
		{
			if(*m_lpszKeyName == '\\')
			{
				m_lpszKeyName++;
				break;
			}
			m_lpszKeyName--;
		}
	}
}

#endif //WIN32
