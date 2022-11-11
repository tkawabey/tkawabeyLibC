/**
 * @file xWinRegistry.h
 * @brief レジストリークラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_WIN_REGISTRY_HEASER_FILE__
#define __X_WIN_REGISTRY_HEASER_FILE__


#ifdef WIN32
#include <windows.h>
#include "xlib/xtime.h"
#include "xlib/xstrbuff.h"

/**
 * レジストリデータ情報構造体
 */
typedef struct _XRegInfo
{
	/** クラス名 */
	XStrBuff    strClassName;
	/** サブキーの数 */
	DWORD       dwSubKeys;
	/** キー名の最大文字数 */
	DWORD       dwMaxKeyNameSize;
	/** クラス名の最大文字数 */
	DWORD       dwMaxClassNameSize;
	/** 値の数 */
	DWORD       dwValues;
	/** 値の名前の最大文字数 */
	DWORD       dwMaxValueNameSize;
	/** 値のデータの最大サイズ */
	DWORD       dwMaxValueDataSize;
	/** セキュリティ記述子の最大サイズ */
	DWORD       dwMaxSecSize;
	/** 最後に更新のあった日付 */
	XTime       tmLastWrite;
}XRegInfo;
/**
 * レジストリの値情報構造体
 */
typedef struct _XRegValue
{
	/** 値の名前 */
	XStrBuff   strValueName;
	/** 値のデータ */
	XStrBuff   data;
	/** 値のデータのサイズ */
	DWORD      dwSize;
	/** 値のデータのタイプ */
	DWORD      dwType;
}XRegValue;

/**
 * レジストリアクセスクラス
 */
class XRegistry 
{
public:

public:
	XRegistry();
	~XRegistry();

// Operation

	bool Open(HKEY hKey, LPCTSTR lpszSubKey, REGSAM samDesired = KEY_READ);
	bool ReOpen(REGSAM samDesired = KEY_READ);
	bool OpenServerKey(LPCTSTR lpszSrv, HKEY hKey, LPCTSTR lpszSubKey, REGSAM samDesired = KEY_READ);


// Operation
	bool FillToKeys();
	bool FillToValues();
	int CountSubKeys();
	int CountValues();
	XRegistry *GetSubKey(int index);
	XRegValue *GetValue(int index);
	bool RegQueryInfoKey(XRegInfo &info);
	LPCTSTR GetSubKeyPath(){ return m_strSubKeyPath; }
	LPCTSTR GetKeyName(){ return m_lpszKeyName; }
	DWORD Size(LPCTSTR lpszVal);
	DWORD GetLastError() const;
	bool Read(LPCTSTR lpszVal, LPDWORD lpbData);
	bool Read(LPCTSTR lpszVal, XStrBuff* pStr);
	bool Read(LPCTSTR lpszVal, XRegValue* pVal);
	bool Write(LPCTSTR lpszVal, DWORD dwData);
	bool Write(LPCTSTR lpszVal, LPCTSTR lpszStr);
	bool Write(XRegValue* pVal);

// キー構築・消滅
	XRegistry *Create(LPCTSTR lpszSubKey, REGSAM samDesired = KEY_ALL_ACCESS, LPDWORD lpdwDisp = NULL, LPCTSTR lpszClassName = NULL, DWORD dwOption = REG_OPTION_NON_VOLATILE);
	bool Create(HKEY hKey, LPCTSTR lpszSubKey, REGSAM samDesired = KEY_ALL_ACCESS, LPDWORD lpdwDisp = NULL, LPCTSTR lpszClassName = NULL, DWORD dwOption = REG_OPTION_NON_VOLATILE);	
	bool DeleteKey(void);
	static bool DeleteKey(HKEY hKey, LPCTSTR lpszSubKey);
	bool Delete(LPCTSTR lpszVal);
	virtual bool Close(void);
protected:
	bool Open(REGSAM samDesired = KEY_READ);
	bool Create(REGSAM samDesired = KEY_ALL_ACCESS, LPDWORD lpdwDisp = NULL, LPCTSTR lpszClassName = NULL, DWORD dwOption = REG_OPTION_NON_VOLATILE);
	bool Read(LPCTSTR lpszVal, LPDWORD lpdwType, LPBYTE lpbData, LPDWORD lpcbData);
	bool Write(LPCTSTR lpszVal, DWORD fdwType, CONST BYTE* lpbData, DWORD cbData);
	bool FreeKeys();
	bool FreeValues();
	void _SetKeyName();
	bool IsValidObject();
protected:
	HKEY		m_hOenKey;
	HKEY		m_hDefKey;			// HKEY_CURRENT_USERなど
	XStrBuff	m_strSubKeyPath;	// Sub Key Strings
	bool		m_bOpenSrv;			// Flag to opend  Remote Server Key 
	LPCTSTR     m_lpszKeyName;		// Key name
	REGSAM      m_RegSam;			// Opend Access Mask
	XArray<XRegistry*> m_arrayKeys;   // Sub Key Datas
	XArray<XRegValue*> m_arrayValues; // Value Datas
	DWORD       m_dwLastError;
public:
};

#endif // WIN32

#endif // __X_WIN_REGISTRY_HEASER_FILE__
