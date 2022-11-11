/**
 * @file xWinRegistry.h
 * @brief ���W�X�g���[�N���X
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
 * ���W�X�g���f�[�^���\����
 */
typedef struct _XRegInfo
{
	/** �N���X�� */
	XStrBuff    strClassName;
	/** �T�u�L�[�̐� */
	DWORD       dwSubKeys;
	/** �L�[���̍ő啶���� */
	DWORD       dwMaxKeyNameSize;
	/** �N���X���̍ő啶���� */
	DWORD       dwMaxClassNameSize;
	/** �l�̐� */
	DWORD       dwValues;
	/** �l�̖��O�̍ő啶���� */
	DWORD       dwMaxValueNameSize;
	/** �l�̃f�[�^�̍ő�T�C�Y */
	DWORD       dwMaxValueDataSize;
	/** �Z�L�����e�B�L�q�q�̍ő�T�C�Y */
	DWORD       dwMaxSecSize;
	/** �Ō�ɍX�V�̂��������t */
	XTime       tmLastWrite;
}XRegInfo;
/**
 * ���W�X�g���̒l���\����
 */
typedef struct _XRegValue
{
	/** �l�̖��O */
	XStrBuff   strValueName;
	/** �l�̃f�[�^ */
	XStrBuff   data;
	/** �l�̃f�[�^�̃T�C�Y */
	DWORD      dwSize;
	/** �l�̃f�[�^�̃^�C�v */
	DWORD      dwType;
}XRegValue;

/**
 * ���W�X�g���A�N�Z�X�N���X
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

// �L�[�\�z�E����
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
	HKEY		m_hDefKey;			// HKEY_CURRENT_USER�Ȃ�
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
