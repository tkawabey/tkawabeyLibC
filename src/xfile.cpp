#ifdef WIN32
  #include <windows.h>
  #include <direct.h>
  #include <wchar.h>
  #include <io.h>
  #include <share.h>
#else
  #include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>


#include "xlib/xfile.h"
#include "xlib/xdata.h"
#include "xlib/xstrcordeconv.h"
#include "xlib/xeception.h"


/////////////////////////////////////////////////////////////////////////////////////
//
// XFileName
//
XFileName::XFileName()
	:XStrBuff()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XFileName::XFileName(const char* lpsz)
	:XStrBuff(lpsz)
{
XEXCEPT_ENTRY
	if( this->GetLength() > 1 ) {
		if( m_pStrAddress[this->GetLength()-1] == X_FILE_DIRECTORY_SEP ) {
			m_pStrAddress[this->GetLength()-1] = 0;
		}
	}
XEXCEPT_EXIT
}
XFileName::XFileName(const wchar_t* lpsz)
	:XStrBuff(lpsz)
{
XEXCEPT_ENTRY
	if( this->GetLength() > 1 ) {
		if( m_pStrAddress[this->GetLength()-1] == X_FILE_DIRECTORY_SEP ) {
			m_pStrAddress[this->GetLength()-1] = 0;
		}
	}
XEXCEPT_EXIT
}
XFileName::XFileName(XFileName &strSrc)
{
XEXCEPT_ENTRY
	*this = (const TCHAR*)strSrc.m_pStrAddress;
XEXCEPT_EXIT
}
XFileName::~XFileName()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
const XFileName& XFileName::operator=(const char* lpsz)
{
XEXCEPT_ENTRY
	XStrBuff::operator=(lpsz);
	return *this;
XEXCEPT_EXIT
}
const XFileName& XFileName::operator=(const wchar_t* lpsz)
{
XEXCEPT_ENTRY
	XStrBuff::operator=(lpsz);
	return *this;
XEXCEPT_EXIT
}
bool XFileName::MakeFullPath(const char* lpszPath, 
				  char* *pDir /*= NULL*/, 
				  char* *pFile /*= NULL*/, 
				  char* *pExp /*= NULL*/)
{
XEXCEPT_ENTRY

	char				szPath[_MAX_PATH] = "";
	char*				pWk = NULL;
	char*				pStart = NULL;
#ifdef WIN32
	char *pFilePart = NULL;
	if( ::GetFullPathNameA(lpszPath, sizeof(szPath), szPath, &pFilePart)	== FALSE ) {
		return false;
	}
#else

 #ifdef _USING_REAL_PATH
	if( ::realpath(lpszPath, szPath) == NULL ) {
		return false;
	}
 #else 
	XStrBuffSplit spitter(_T('/'));
	const char		**pArgv = NULL;
	const char		*pPathTemp = NULL;
	int				nArgc, i;
	int             iUp = 0;
	int             iThere = 0;
	char			szPwd[260*2] = "";
	char			szSep[2] = "";
	XStrBuff		strBuff( _T("") );
	XStrBuff		strSwap( _T("") );

	szSep[0] = _T('/');
	szSep[1] = 0;


	if( *lpszPath == _T('.') ) {
		XStrBuff strPwd;
		// 現在のディレクトリ名を取得
		if( XFileName::Getcwd(strPwd) == false ) {
			return false;
		}
		::strcpy(szPwd, strPwd);
		XStrBuff::StrCat(szPwd, szSep);
		XStrBuff::StrCat(szPwd, lpszPath);
	} else {
		// ファイル名のみかチェック
		pPathTemp = lpszPath;
		while( *pPathTemp != 0 ) {
			if( *pPathTemp == szSep[0] ) {
				iThere = 1;
				break;
			}
			pPathTemp++;
		}
		// ファイル名のみ
		if( iThere == 0 ) {
			XStrBuff strPwd;
			// 現在のディレクトリ名を取得
			if( XFileName::Getcwd(strPwd) == false ) {
				return false;
			}
			::strcpy(szPwd, strPwd);
			XStrBuff::StrCat(szPwd, szSep);
			XStrBuff::StrCat(szPwd, lpszPath);
		} else {
			strcpy(szPwd, lpszPath);
		}
		iThere = 0;
	}
	spitter = szPwd;
	pArgv = spitter.GetArgv();
	nArgc = spitter.GetArgCouunt();

	i = spitter.GetArgCouunt()-1;
	for(;;) {
		if( ::strcmp(pArgv[i], "..")  == 0 ) {
			iUp++;
		} else
		if( strcmp(pArgv[i], ".")  == 0 ) {
			iThere++;
		} else
		if( iUp != 0 ) {
			if(iUp > 0) {
				iUp--;
			}				 
		} else {
			if( ::strlen(strBuff) == 0 ) {
				strSwap.Format("%s%s", (const char*)pArgv[i], (const char*)strBuff);
			} else {
				strSwap.Format("%s%s%s", (const char*)pArgv[i], szSep, (const char*)strBuff);
			}
			strBuff = strSwap;
		}
		if( i == 0 ) {
			break;
		}
		i--;
	}
	::strcpy(szPath, strSwap);

	pWk = NULL;
	pStart = NULL;
 #endif // _USING_REAL_PATH
#endif // WIN32
	*this = (char*)szPath;
	pWk = pStart = *this;

	if( pDir != NULL ) {
		*pDir = pStart;
	}
	pWk += ::strlen(pWk);
	while( pWk != pStart) {
		if( *pWk == X_FILE_DIRECTORY_SEP ) {
			pWk++;
			break;
		}
		pWk--;
	}
	if( pFile != NULL ) {
		*pFile = pWk;
	}

	pStart = pWk;
	pWk += ::strlen(pWk);
	while( pWk != pStart) {
		if( *pWk == '.' ) {
			pWk++;
			break;
		}
		pWk--;
	}
	if( pWk != pStart && pExp != NULL ) {
		*pExp = pWk;
	}
XEXCEPT_EXIT
	return true;
}
bool XFileName::MakeFullPath(const wchar_t* lpszPath, 
				  wchar_t* *pDir /*= NULL*/, 
				  wchar_t* *pFile /*= NULL*/, 
				  wchar_t* *pExp /*= NULL*/)
{
XEXCEPT_ENTRY
	if( this->MakeFullPath( (const char*)XStrBuff(lpszPath) ) == false ) {
		return false;
	}
	
	wchar_t*	pPath = *this;
	wchar_t*	pWk = NULL;
	wchar_t*	pStart = NULL;
	pWk = pStart = *this;

	if( pDir != NULL ) {
		*pDir = pStart;
	}
	pWk += ::wcslen(pWk);
	while( pWk != pStart) {
		if( *pWk == X_FILE_DIRECTORY_SEP ) {
			pWk++;
			break;
		}
		pWk--;
	}
	if( pFile != NULL ) {
		*pFile = pWk;
	}

	pStart = pWk;
	pWk += ::wcslen(pWk);
	while( pWk != pStart) {
		if( *pWk == L'.' ) {
			pWk++;
			break;
		}
		pWk--;
	}
	if( pWk != pStart && pExp != NULL ) {
		*pExp = pWk;
	}
XEXCEPT_EXIT
	return true;
}

XStrBuff XFileName::GetFileName() const
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	XFileName fileName;
	TCHAR* pDir = NULL; 
	TCHAR* pFile = NULL; 
	TCHAR* pExp = NULL;
	
XTRACE(_T("XFileName::GetFileName()　 m_pStrAddress:%s\n"), m_pStrAddress);
	if( m_pStrAddress == NULL ) {
		return str;
	}

	fileName.MakeFullPath(m_pStrAddress, &pDir, &pFile, &pExp);
	if( pDir ) {
		str = (const TCHAR*)pFile;
	}
	return str;
XEXCEPT_EXIT
}

XStrBuff XFileName::GetFileExt() const
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	XFileName fileName;
	TCHAR* pDir = NULL; 
	TCHAR* pFile = NULL; 
	TCHAR* pExp = NULL;

XTRACE(_T("XFileName::GetFileExt()　 m_pStrAddress:%s\n"), m_pStrAddress);	
	if( m_pStrAddress == NULL ) {
		return str;
	}

	fileName.MakeFullPath(m_pStrAddress, &pDir, &pFile, &pExp);
	if( pExp ) {
		str = (const TCHAR*)pExp;
	}
	return str;
XEXCEPT_EXIT
}


XStrBuff XFileName::GetPathName() const
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	XFileName fileName;
	TCHAR* pDir = NULL; 
	TCHAR* pFile = NULL; 
	TCHAR* pExp = NULL;
	
//XTRACE(_T("XFileName::GetPathName()　 m_pStrAddress:%s\n"), m_pStrAddress);	
	if( m_pStrAddress == NULL ) {
		return str;
	}

	fileName.MakeFullPath(m_pStrAddress);
	str = (const TCHAR*)fileName;
	return str;
XEXCEPT_EXIT
}
/**
 * ディレクトリ文字列を取得します。
 * @return ディレクトリ文字列
 */
XStrBuff XFileName::GetDirectory() const
{
XEXCEPT_ENTRY
	XStrBuff str(_T(""));
	XFileName fileName;
	TCHAR* pDir = NULL; 
	TCHAR* pFile = NULL; 
	TCHAR* pExp = NULL;
	TCHAR	cSave = 0;
	

	if( m_pStrAddress == NULL ) {
		return str;
	}

	fileName.MakeFullPath(m_pStrAddress, &pDir, &pFile, &pExp);
	if( pFile ) {
		pFile--;	// ディレクトリの区切り分
		if( *pFile == X_FILE_DIRECTORY_SEP ) {
			*pFile = 0;
		}
		str = pDir;
	} else if( pDir != NULL ) {
		str = pDir;
	}
	return str;
XEXCEPT_EXIT
}
bool XFileName::Getcwd(XStrBuff& Buff)
{
XEXCEPT_ENTRY
	char sz[260] = {0};
#if _MSC_VER >= 1310
	if( ::_getcwd(sz, sizeof(sz)) == NULL ) {
#else
	if( ::getcwd(sz, sizeof(sz)) == NULL ) {
#endif
		return false;
	}
	Buff = sz;
	return true;
XEXCEPT_EXIT
}
bool XFileName::Cdir(const char* pDir)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return ::_chdir(pDir) == 0 ? true : false;
#else
	return ::chdir(pDir) == 0 ? true : false;
#endif
XEXCEPT_EXIT
}
bool XFileName::Cdir(const wchar_t* pDir)
{
XEXCEPT_ENTRY
	return XFileName::Cdir( (const char* )XStrBuff(pDir) );
XEXCEPT_EXIT
}
bool XFileName::Remove(const char* pFile)
{
XEXCEPT_ENTRY
#if _MSC_VER >= 1310
	return ::_unlink(pFile) == 0 ? true : false;
#else
	return ::unlink(pFile) == 0 ? true : false;
#endif
XEXCEPT_EXIT
}
bool XFileName::Remove(const wchar_t* pFile)
{
XEXCEPT_ENTRY
	return XFileName::Remove( (const char* )XStrBuff(pFile) );
XEXCEPT_EXIT
}
bool XFileName::rename(const char* pOldFile, const char* pNewFile)
{
XEXCEPT_ENTRY
	return ::rename( pOldFile, pNewFile) == 0 ? true : false;
XEXCEPT_EXIT
}
bool XFileName::rename(const wchar_t* pOldFile, const wchar_t* pNewFile)
{
XEXCEPT_ENTRY
	return ::_wrename( pOldFile, pNewFile ) == 0 ? true : false;
XEXCEPT_EXIT
}

/////////////////////////////////////////////////////////////////////////////////////
//
// XFile
//
XFile::XFile()
{
XEXCEPT_ENTRY
	m_iFile = -1;
XEXCEPT_EXIT
}
XFile::~XFile()
{
XEXCEPT_ENTRY
	Close();
XEXCEPT_EXIT
}
bool XFile::Close()
{
XEXCEPT_ENTRY
	if( m_iFile != -1 ) {
#ifdef WIN32
		_close(m_iFile);
#else
		::close(m_iFile);
#endif
		m_iFile = -1;
	}
	return true;
XEXCEPT_EXIT
}
bool XFile:: Create(const TCHAR *filename, bool bExistErr, bool bAppend, bool bShare, int pmode)
{
XEXCEPT_ENTRY
#ifdef UNICODE
	return CreateW(filename, bExistErr, bAppend, bShare, pmode);
#else
	return CreateA(filename, bExistErr, bAppend, bShare, pmode);
#endif
XEXCEPT_EXIT
}
bool XFile::Open(const TCHAR *filename, bool bReadOnly, bool bAppend, bool bTruc,bool bShare)
{
XEXCEPT_ENTRY
#ifdef UNICODE
	return OpenW(filename, bReadOnly, bAppend, bTruc, bShare);
#else
	return OpenA(filename, bReadOnly, bAppend, bTruc, bShare);
#endif
XEXCEPT_EXIT
}
bool XFile::CreateA(const char *filename, bool bExistErr, bool bAppend, bool bShare, int pmode)
{
XEXCEPT_ENTRY
	int flg = O_CREAT | O_RDWR;
#ifdef WIN32
	flg |= O_BINARY;
#endif
	if( bExistErr == true ) { flg |= O_EXCL; }
	if( bAppend == true ) {   flg |= O_APPEND; }
	return OpenRawA(filename, flg, bShare, pmode);
XEXCEPT_EXIT
}
bool XFile::CreateW(const wchar_t *filename, bool bExistErr, bool bAppend, bool bShare, int pmode)
{
XEXCEPT_ENTRY
	int flg = O_CREAT | O_RDWR;
#ifdef WIN32
	flg |= O_BINARY;
#endif
	if( bExistErr == true ) { flg |= O_EXCL; }
	if( bAppend == true ) {   flg |= O_APPEND; }
	return OpenRawW(filename, flg, bShare, pmode);
XEXCEPT_EXIT
}

bool XFile::OpenA(const char *filename, bool bReadOnly, bool bAppend, bool bTruc, bool bShare)
{
XEXCEPT_ENTRY
	int flg = 0;
#ifdef WIN32
	flg |= O_BINARY;
#endif	
	if( bReadOnly) {
		flg |= O_RDONLY;
	} else {
		flg |= O_RDWR;
	}
	if( bAppend == true ) { flg |= O_APPEND; }
	if( bTruc == true ) {   flg |= O_TRUNC; }

	return OpenRawA(filename, flg, bShare, 0x0666);
XEXCEPT_EXIT
}
bool XFile::OpenW(const wchar_t *filename, bool bReadOnly, bool bAppend, bool bTruc,bool bShare)
{
XEXCEPT_ENTRY
	int flg = 0;
#ifdef WIN32
	flg |= O_BINARY;
#endif
	if( bReadOnly) {
		flg |= O_RDONLY;
	} else {
		flg |= O_RDWR;
	}
	if( bAppend == true ) { flg |= O_APPEND; }
	if( bTruc == true ) {   flg |= O_TRUNC; }

	return OpenRawW(filename, flg, bShare, 0x0666);
XEXCEPT_EXIT
}

bool XFile::OpenRawA(const char *filename, int flg, bool bShare, int pmode)
{
XEXCEPT_ENTRY
	this->Close();

#ifdef WIN32
	pmode = _SH_DENYNO;
	if( bShare == true ) {
#if _MSC_VER >= 1310
		errno_t errNo = _sopen_s(&m_iFile, filename, flg, pmode, _S_IREAD | _S_IWRITE);
#else
	m_iFile = _sopen(filename, flg, pmode, _S_IWRITE);
#endif
	} else {
#if _MSC_VER >= 1310
		errno_t errNo = _sopen_s(&m_iFile, filename, flg, pmode, _S_IREAD | _S_IWRITE);
#else
	m_iFile = _sopen(filename, flg, pmode, _S_IWRITE);
#endif
	}
#else
	if( bShare == true ) {
		flg |= O_SYNC;
	}
	m_iFile = ::open(filename, flg, pmode);
#endif


#ifdef _DEBUG
	if( m_iFile == -1) {
#if _MSC_VER >= 1310
		char szErrorMsg[256] = {0};
		::strerror_s(szErrorMsg, XCOUNTOF(szErrorMsg), errno);
		XStrBuff strError = szErrorMsg;
#else
		XStrBuff strError = ::strerror( errno );
#endif
		XStrBuff strFile = filename;
		XTRACE(_T("Fail to open file. [%s] / %s\n"), (const TCHAR*)strFile, (const TCHAR*)strError);
	}
#endif
	return m_iFile != -1 ? true : false;
XEXCEPT_EXIT
}
bool XFile::OpenRawW(const wchar_t *filename, int flg, bool bShare, int pmode)
{
XEXCEPT_ENTRY
//	return OpenRawA( XStrBuff(filename), flg, bShare, pmode);
	this->Close();

#ifdef WIN32
	pmode = _SH_DENYNO;
	if( bShare == true ) {
#if _MSC_VER >= 1310
		errno_t errNo = _wsopen_s(&m_iFile, filename, flg, pmode, _S_IREAD | _S_IWRITE);
#else
	m_iFile = _sopen(filename, flg, pmode, _S_IWRITE);
#endif
	} else {
#if _MSC_VER >= 1310
		errno_t errNo = _wsopen_s(&m_iFile, filename, flg, pmode, _S_IREAD | _S_IWRITE);
#else
	m_iFile = _sopen(filename, flg, pmode, _S_IWRITE);
#endif
	}
#else
	if( bShare == true ) {
		flg |= O_SYNC;
	}
	m_iFile = ::open(XStrBuff(filename), flg, pmode);
#endif


#ifdef _DEBUG
	if( m_iFile == -1) {
#if _MSC_VER >= 1310
		char szErrorMsg[256] = {0};
		::strerror_s(szErrorMsg, XCOUNTOF(szErrorMsg), errno);
		XStrBuff strError = szErrorMsg;
#else
		XStrBuff strError = ::strerror( errno );
#endif
		XStrBuff strFile = filename;
		XTRACE(_T("Fail to open file. [%s] / %s\n"), (const TCHAR*)strFile, (const TCHAR*)strError);
	}
#endif
	return m_iFile != -1 ? true : false;
XEXCEPT_EXIT
}

XFile* XFile::Duplicate() const
{
XEXCEPT_ENTRY
	if( m_iFile == -1) {
		return NULL;
	}
	int iDup  = ::_dup((int)m_iFile);

	if( iDup == -1 ) {
		return NULL;
	}
	XFile *pFile = new XFile();
	pFile->m_iFile = iDup;
	return pFile;
XEXCEPT_EXIT
}
bool XFile::Flush()
{
XEXCEPT_ENTRY
//	_XASSERT( m_pFp );
//	if( ::fflush(m_pFp) != 0 ) {
//		return false;
//	}
	return true;
XEXCEPT_EXIT
}
size_t XFile::GetLength()
{
XEXCEPT_ENTRY
	if( m_iFile == -1 ) {
		return 0;
	}
	struct stat  sts;
	if( ::fstat(m_iFile, &sts) == -1 ) {
		return (size_t)0;
	}
	return sts.st_size;
XEXCEPT_EXIT
}
bool XFile::GetFileTime(XTime *cTime, XTime *mTime, XTime *aTime)
{
XEXCEPT_ENTRY
	if( m_iFile == -1 ) {
		return false;
	}
	struct stat  sts;
	if( ::fstat(m_iFile, &sts) == -1 ) {
		return true;
	}
	if( cTime != NULL ) {
		*cTime = sts.st_ctime;
	}
	if( mTime != NULL ) {
		*mTime = sts.st_mtime;
	}
	if( aTime != NULL ) {
		*aTime = sts.st_atime;
	}
	return true;
XEXCEPT_EXIT
}
unsigned short XFile::GetMode()
{
XEXCEPT_ENTRY
	struct stat  sts;

	if( m_iFile == -1 ) {
		return 0;
	}
	if( ::fstat(m_iFile, &sts) == -1 ) {
		return 0;
	}
	return sts.st_mode;
XEXCEPT_EXIT
}
bool XFile::Seek(long offset, SeekPosition origin)
{
XEXCEPT_ENTRY
	if( m_iFile == -1 ) {
		return false;
	}

	if( ::_lseek(m_iFile, offset, origin) != 0 ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XFile::SeekFirst()
{
XEXCEPT_ENTRY
	return this->Seek(0, begin);
XEXCEPT_EXIT
}
bool XFile::SeekLast()
{
XEXCEPT_ENTRY
	return Seek(0, end);
XEXCEPT_EXIT
}
long XFile::GetCurrentPos()
{
XEXCEPT_ENTRY
	if( m_iFile == -1 ) {
		return 0;
	}
	return Seek(0, current);
XEXCEPT_EXIT
}
bool XFile::readAllText(XStrBuff& buf)
{
XEXCEPT_ENTRY
	size_t	sizeSize = GetLength();
	size_t	i = 0;
	XBuff	workBuff;
	XBIN*	pWk = NULL;
	int		iCode = 0;	// 0:multi byte: 1:UTF16 2:Unicode(Big endan) 3:UTF8

	if( sizeSize == 0 ) {
		buf = _T("");
		return true;
	}
	pWk = (XBIN*)workBuff.Alloc( sizeSize+1 );
	if( pWk == NULL ) {
		return false;
	}
	workBuff.MemSet();

	if( this->Read(pWk, sizeSize) == false ) {
		return false;
	}
	if( sizeSize > 3 ) {
		if( pWk[0] == 0xFF && pWk[1] == 0xFE ) {
			iCode = 1;
			pWk += 2;
			sizeSize -= 2;
		} else
		if( pWk[0] == 0xFE && pWk[1] == 0xFF ) {
			iCode = 2;
			pWk += 2;
			sizeSize -= 2;
		} else
		if( pWk[0] == 0xEF && pWk[1] == 0xBB && pWk[1] == 0xBF ) {
			iCode = 3;
			pWk += 3;
			sizeSize -= 3;
		}
	}
	
	if( iCode == 0 ) {
		buf = (const char*)pWk;
	} else
	if( iCode == 1 ) {
		buf = (const wchar_t*)pWk;
	} else
	if( iCode == 2 ) {
		size_t len = sizeSize / sizeof(wchar_t);
		wchar_t* p_w = (wchar_t*)pWk;
		for(i = 0; i < len; i++ ) {
			*p_w = XDataCoder::_ntoh2(*p_w);
			p_w++;
		}
		buf = (const wchar_t*)pWk;
	} else
	if( iCode == 3 ) {
		XStrBuffCodeConv::convertNativeToUTF8((const char*)pWk, buf, true);
	}
	return true;
XEXCEPT_EXIT
	return false;
}
bool XFile::GetStatus(const char* lpszFileNsme, XFileStatus& rStatus)
{
XEXCEPT_ENTRY
	struct stat dStat;
	if( ::stat(lpszFileNsme, &dStat) == -1 ) {
		return false;
	}
	rStatus.m_ctime      = dStat.st_ctime;
	rStatus.m_mtime      = dStat.st_mtime;
	rStatus.m_atime      = dStat.st_atime;
	rStatus.m_size       = dStat.st_size;
	rStatus.m_attribute  = dStat.st_mode;
	memcpy(&rStatus.m_dStat, &dStat, sizeof(dStat));
	rStatus.m_strFileName = lpszFileNsme;
	return true;
XEXCEPT_EXIT
}
bool XFile::GetStatus(const wchar_t* lpszFileNsme, XFileStatus& rStatus)
{
XEXCEPT_ENTRY
	return XFile::GetStatus( (const char*)XStrBuff(lpszFileNsme), rStatus);
XEXCEPT_EXIT
}
XFileName XFile::getCurrentDirectory()
{
XEXCEPT_ENTRY
	XFileName fileName;
#ifdef _UNICODE
	wchar_t szBuff[260] = L"";

	_wgetcwd(szBuff, sizeof(szBuff));
	fileName = szBuff;
#else
	char szBuff[260] = "";


	_getcwd(szBuff, sizeof(szBuff));
	fileName = szBuff;
#endif	// _UNICODE
	return fileName;
XEXCEPT_EXIT
}

bool XFile::changeCurrentDirectory(const char* lpszDirectoryName)
{
XEXCEPT_ENTRY
	if( _chdir(lpszDirectoryName) == -1 ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XFile::changeCurrentDirectory(const wchar_t* lpszDirectoryName)
{
XEXCEPT_ENTRY
#ifdef _MSC_VER 
	if( _wchdir(lpszDirectoryName) == -1 ) {
		return false;
	}
	return true;
#else
	return XFile::changeCurrentDirectory((const char*)XStrBuff(lpszDirectoryName));
#endif
XEXCEPT_EXIT
}



bool XFile::removeDirectory(const char* lpszDirectoryName)
{
XEXCEPT_ENTRY

	if( _rmdir(lpszDirectoryName) == -1 ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}
bool XFile::removeDirectory(const wchar_t* lpszDirectoryName)
{
XEXCEPT_ENTRY
#ifdef _MSC_VER 
	if( _wrmdir(lpszDirectoryName) == -1 ) {
		return false;
	}
	return true;
#else
	return XFile::removeDirectory((const char*)XStrBuff(lpszDirectoryName));
#endif
XEXCEPT_EXIT
}


bool XFile::makeDirectory(const char* lpszDirectoryName)
{
XEXCEPT_ENTRY
#ifdef _MSC_VER 
	if( _mkdir(lpszDirectoryName) == -1 ) {
		return false;
	}
	return true;
#else
	if( mkdir(lpszDirectoryName, 0777) == -1 ) {
		return false;
	}
	return true;
#endif
XEXCEPT_EXIT
}
bool XFile::makeDirectory(const wchar_t* lpszDirectoryName)
{
XEXCEPT_ENTRY
#ifdef _MSC_VER 
	if( _wmkdir(lpszDirectoryName) == -1 ) {
		return false;
	}
	return true;
#else
	return XFile::makeDirectory( (const char*)XStrBuff(lpszDirectoryName) );
#endif
XEXCEPT_EXIT
}


size_t XFile::WriteRaw(const void* lpData, size_t nSize, int flg /*= 0*/)
{
XEXCEPT_ENTRY
	if( m_iFile == -1 ) {
		return (size_t)-1;
	}
	size_t iRet = 0;
#ifdef WIN32
	iRet = _write(m_iFile, lpData, nSize);
#else
	iRet = ::write(m_iFile, lpData, nSize);
#endif
#ifdef _DEBUG
	if( iRet == -1 ) {
		XTRACE(_T("XFile::WriteRaw   ::write error (%u)\n"), errno);
	}
#endif
	return iRet;
XEXCEPT_EXIT
}
size_t XFile::ReadRaw(void* lpData, size_t nSize, int flg /*= 0*/)
{
XEXCEPT_ENTRY
	if( m_iFile == -1 ) {
		return (size_t)-1;
	}
	size_t iRet = 0;
#ifdef WIN32
	iRet = _read(m_iFile, lpData, nSize);
#else
	iRet = ::read(m_iFile, lpData, nSize);
#endif
	return iRet;
XEXCEPT_EXIT
}


errno_t XFile::fopenA(FILE** ppFp, const char* p_file_path, const char* p_mode)
{
	errno_t errNo = 0;
XEXCEPT_ENTRY

#if _MSC_VER >= 1310
	errNo = ::fopen_s(ppFp, p_file_path, p_mode);
#else
	*ppFp = ::fopen(p_file_path, p_mode);
	errNo = errno;	
#endif

XEXCEPT_EXIT
	return errNo;
}
errno_t XFile::fopenW(FILE** ppFp, const wchar_t* p_file_path, const wchar_t* p_mode)
{
	errno_t errNo = 0;
XEXCEPT_ENTRY

#if _MSC_VER >= 1310
	errNo = ::_wfopen_s(ppFp, p_file_path, p_mode);
#else
	*ppFp = ::_wfopen(p_file_path, p_mode);
	errNo = errno;	
#endif

XEXCEPT_EXIT
	return errNo;
}
errno_t XFile::fopenT(FILE** ppFp, const TCHAR* p_file_path, const TCHAR* p_mode)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XFile::fopenW(ppFp, p_file_path, p_mode);
#else
	return XFile::fopenA(ppFp, p_file_path, p_mode);
#endif
XEXCEPT_EXIT
}


int     XFile::vfprintfA(FILE * fp, const char * _Format, va_list varg)
{
XEXCEPT_ENTRY
	return ::vfprintf(fp, _Format, varg);
XEXCEPT_EXIT
}
int     XFile::vfprintfW(FILE * fp, const wchar_t * _Format, va_list varg)
{
XEXCEPT_ENTRY
	return ::vfwprintf(fp, _Format, varg);
XEXCEPT_EXIT
}
int     XFile::vfprintfT(FILE * fp, const TCHAR * _Format, va_list varg)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XFile::vfprintfW(fp, _Format, varg);
#else
	return XFile::vfprintfA(fp, _Format, varg);
#endif
XEXCEPT_EXIT
}


int XFile::fprintfA(FILE* fp, const char* format, ...)
{
	int ret = 0;
XEXCEPT_ENTRY
	va_list args;
	va_start(args, format);
	ret = XFile::vfprintfA(fp, format, args);
	va_end(args);
XEXCEPT_EXIT
	return ret;
}
int XFile::fprintfW(FILE* fp, const wchar_t* format, ...)
{
	int ret = 0;
XEXCEPT_ENTRY
	va_list args;
	va_start(args, format);
	ret = XFile::vfprintfW(fp, format, args);
	va_end(args);
XEXCEPT_EXIT
	return ret;
}
int XFile::fprintfT(FILE* fp, const TCHAR* format, ...)
{
	int ret = 0;
XEXCEPT_ENTRY
	va_list args;
	va_start(args, format);
#ifdef _UNICODE
	ret = XFile::vfprintfW(fp, format, args);
#else
	ret = XFile::vfprintfA(fp, format, args);
#endif
	va_end(args);
XEXCEPT_EXIT
	return ret;
}

int XFile::fputcA(int ch, FILE* fp)
{
XEXCEPT_ENTRY
	return ::fputc(ch, fp);
XEXCEPT_EXIT
}
int XFile::fputcW(wchar_t ch, FILE* fp)
{
XEXCEPT_ENTRY
	return ::fputwc(ch, fp);
XEXCEPT_EXIT
}
int XFile::fputcT(int ch, FILE* fp)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XFile::fputcW(ch, fp);
#else
	return XFile::fputcA(ch, fp);
#endif
XEXCEPT_EXIT
}




int XFile::fputsA(const char* p, FILE* fp)
{
XEXCEPT_ENTRY
	return ::fputs(p, fp);
XEXCEPT_EXIT
}
int XFile::fputsW(const wchar_t* p, FILE* fp)
{
XEXCEPT_ENTRY
	return ::fputws(p, fp);
XEXCEPT_EXIT
}
int XFile::fputsT(const TCHAR* p, FILE* fp)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XFile::fputsW(p, fp);
#else
	return XFile::fputsA(p, fp);
#endif
XEXCEPT_EXIT
}

char*    XFile::fgetsA(char* pbuff, int count, FILE* fp)
{
XEXCEPT_ENTRY
	return ::fgets(pbuff, count, fp);
XEXCEPT_EXIT
}
wchar_t* XFile::fgetsW(wchar_t* pbuff, int count, FILE* fp)
{
XEXCEPT_ENTRY
	return ::fgetws(pbuff, count, fp);
XEXCEPT_EXIT
}
TCHAR*    XFile::fgetsT(TCHAR* pbuff, int count, FILE* fp)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XFile::fgetsW(pbuff, count, fp);
#else
	return XFile::fgetsA(pbuff, count, fp);
#endif
XEXCEPT_EXIT
}
int      XFile::fgetcA(FILE* fp)
{
XEXCEPT_ENTRY
	return ::fgetc(fp);
XEXCEPT_EXIT
}
wint_t   XFile::fgetcW(FILE* fp)
{
XEXCEPT_ENTRY
	return ::fgetwc(fp);
XEXCEPT_EXIT
}
int      XFile::fgetcT(FILE* fp)
{
XEXCEPT_ENTRY
#ifdef _UNICODE
	return XFile::fgetcW(fp);
#else
	return XFile::fgetcA(fp);
#endif
XEXCEPT_EXIT
}


//
//	XFileFind
//
XFileFind::XFileFind()
{
XEXCEPT_ENTRY
	m_pFoundInfo = NULL;
	m_pNextInfo = NULL;
	m_hContext = NULL;
#ifdef WIN32
	m_chDirSeparator = '\\';
#else	/* for UNIX */
	m_chDirSeparator = '/';
#endif
XEXCEPT_EXIT
}

XFileFind::~XFileFind()
{
XEXCEPT_ENTRY
	Close();
XEXCEPT_EXIT
}


bool XFileFind::FindFile(const char* pstrName /*= NULL*/)
{
XEXCEPT_ENTRY
	Close();
#ifdef WIN32
#ifdef _UNICODE
	m_pNextInfo = new WIN32_FIND_DATAW;
#else
	m_pNextInfo = new WIN32_FIND_DATAA;
#endif
	m_bGotLast = false;
	XFileName    fileName;

	if (pstrName == NULL) {
		pstrName = "*.*";
	}
#ifdef _UNICODE
	XStrBuff::StrCpy(((WIN32_FIND_DATAW*) m_pNextInfo)->cFileName
		, sizeof(((WIN32_FIND_DATAW*) m_pNextInfo)->cFileName)/sizeof(wchar_t)
		, XStrBuff(pstrName) );
#else
	XStrBuff::StrCpy(((WIN32_FIND_DATAA*) m_pNextInfo)->cFileName
		, sizeof(((WIN32_FIND_DATAA*) m_pNextInfo)->cFileName)/sizeof(char)
		, pstrName);
#endif

  #ifdef _UNICODE
	m_hContext = ::FindFirstFileW(XStrBuff(pstrName), (WIN32_FIND_DATAW*)m_pNextInfo);
  #else
	m_hContext = ::FindFirstFileA(pstrName, (WIN32_FIND_DATAA*)m_pNextInfo);
  #endif

	if (m_hContext == INVALID_HANDLE_VALUE) {
		DWORD dwTemp = ::GetLastError();
		Close();
		::SetLastError(dwTemp);
		return false;
	}
	// ルート情報の取得
	TCHAR* pstrRoot = (TCHAR*)m_strRoot.Alloc(_MAX_PATH * sizeof(TCHAR));
	fileName.MakeFullPath(pstrName);
	XStrBuff::StrCpy(pstrRoot, m_strRoot.getCountOfBuff(), fileName);
	const TCHAR* pstr = pstrRoot;

#else	/* for UNIX */
	m_pNextInfo = new XFindData;
	m_bGotLast = false;

	if (pstrName == NULL)
		pstrName = ".";

	// ルート情報の取得
	TCHAR* pstrRoot = (TCHAR*)m_strRoot.Alloc(260);
	const TCHAR* pstr = realpath(pstrName, pstrRoot);

	_XASSERT(pstr != NULL);
	if (pstr == NULL) {
		Close();
		return false;
	}

	// ファイル情報の取得
	m_hContext = ::opendir(pstrName);
	if(m_hContext == NULL) {
		Close();
		return false;
	}
	struct dirent *pDirent = ::readdir(m_hContext);
	if(pDirent == NULL)
	{
		Close();
		return false;
	}
	::memcpy( &((XFindData*)m_pNextInfo)->m_dirent, pDirent, sizeof(struct dirent));

	XFileStatus rStatus;
	if( XFile::GetStatus(pstr, rStatus) == false ) {
		Close();
		return false;
	}
	::memcpy( &((XFindData*)m_pNextInfo)->m_stat, &rStatus.m_dStat, sizeof(struct stat));	
#endif


#ifdef WIN32
	_XASSERT(pstr != NULL);
	if (pstr == NULL) {
		Close();
		::SetLastError(ERROR_INVALID_NAME);
		return false;
	} else {
		TCHAR* pstrBack  = XStrBuff::StrRChr(pstrRoot, '\\');
		TCHAR* pstrFront = XStrBuff::StrRChr(pstrRoot, '/');

		if (pstrFront != NULL || pstrBack != NULL) {
			if (pstrFront == NULL)
				pstrFront = pstrRoot;
			if (pstrBack == NULL)
				pstrBack = pstrRoot;

			if (pstrFront >= pstrBack)
				*pstrFront = '\0';
			else
				*pstrBack = '\0';
		}
	}
#endif
	return true;
XEXCEPT_EXIT
}
bool XFileFind::FindFile(const wchar_t* pstrName /*= NULL*/)
{
XEXCEPT_ENTRY
	if( pstrName == NULL ) {
		return FindFile( (const char*)NULL );
	} else {
		return FindFile( (const char*)XStrBuff(pstrName) );
	}
XEXCEPT_EXIT
}
bool XFileFind::NextFile()
{
XEXCEPT_ENTRY
	_XASSERT(m_hContext != NULL);
	if (m_hContext == NULL) {
		return false;
	}
	if (m_pFoundInfo == NULL) {
#ifdef WIN32
  #ifdef _UNICODE
		m_pFoundInfo = new WIN32_FIND_DATAW;
  #else
		m_pFoundInfo = new WIN32_FIND_DATAA;
  #endif
#else
		m_pFoundInfo = new XFindData;
#endif
	}

	void* pTemp = m_pFoundInfo;
	m_pFoundInfo = m_pNextInfo;
	m_pNextInfo = pTemp;
#ifdef WIN32
  #ifdef _UNICODE
	return ::FindNextFileW(m_hContext, (LPWIN32_FIND_DATAW) m_pNextInfo) == TRUE ? true : false;
  #else
	return ::FindNextFileA(m_hContext, (LPWIN32_FIND_DATAA) m_pNextInfo) == TRUE ? true : false;
  #endif	
#else
	XStrBuff strRoot;
	strRoot = m_strRoot;
	struct dirent *pDirent = ::readdir(m_hContext);
	if(pDirent == NULL) {
		return false;
	}
	::memcpy( &((XFindData*)m_pNextInfo)->m_dirent, pDirent, sizeof(struct dirent));

	strRoot += _T("/");
	strRoot += pDirent->d_name;

	XFileStatus rStatus;
	if( XFile::GetStatus((const TCHAR*)strRoot, rStatus) == false ) {
		return false;
	}
	::memcpy( &((XFindData*)m_pNextInfo)->m_stat, &rStatus.m_dStat, sizeof(struct stat));	
	
	return true;
#endif
XEXCEPT_EXIT
}
void XFileFind::Close()
{
XEXCEPT_ENTRY
	if (m_pFoundInfo != NULL) {
#ifdef WIN32
		delete m_pFoundInfo;
#else
		LPXFindData pTmp = (LPXFindData)m_pFoundInfo;
		delete pTmp;
#endif
		m_pFoundInfo = NULL;
	}

	if (m_pNextInfo != NULL) {
#ifdef WIN32
		delete m_pNextInfo;
#else
		LPXFindData pTmp = (LPXFindData)m_pNextInfo;
		delete pTmp;
#endif
		m_pNextInfo = NULL;
	}
#ifdef WIN32
	if (m_hContext != NULL && m_hContext != INVALID_HANDLE_VALUE)
#else
	if (m_hContext != NULL)
#endif
	{
		CloseContext();
		m_hContext = NULL;
	}
XEXCEPT_EXIT
}

void XFileFind::CloseContext()
{
XEXCEPT_ENTRY
#ifdef WIN32
	::FindClose(m_hContext);
#else
	::closedir(m_hContext);
#endif
	return;
XEXCEPT_EXIT
}
XFileName XFileFind::GetFileName() const
{
XEXCEPT_ENTRY
	XFileName strFileName("");
	XStrBuff strFName;
	XStrBuff strResult = m_strRoot;
	
	if (m_pFoundInfo != NULL) {
#ifdef WIN32
  #ifdef _UNICODE
		strFName = ((LPWIN32_FIND_DATAW) m_pFoundInfo)->cFileName;
  #else
		strFName = ((LPWIN32_FIND_DATAA) m_pFoundInfo)->cFileName;
  #endif
#else	
		strFName = ((LPXFindData) m_pFoundInfo)->m_dirent.d_name;
#endif
	}
	strFileName.Format(_T("%s%c%s"), 
		(const TCHAR*)strResult, X_FILE_DIRECTORY_SEP, (const TCHAR*)strFName);
	return strFileName;
XEXCEPT_EXIT
}
bool XFileFind::IsDots()
{
XEXCEPT_ENTRY
	_XASSERT(m_hContext != NULL);

	// return TRUE if the file name is "." or ".." and
	// the file is a directory

	bool bResult = false;
	if (m_pFoundInfo != NULL && IsDirectory()) {
#ifdef WIN32
  #ifdef _UNICODE
		LPWIN32_FIND_DATAW pFindData = (LPWIN32_FIND_DATAW) m_pFoundInfo;
  #else
		LPWIN32_FIND_DATAA pFindData = (LPWIN32_FIND_DATAA) m_pFoundInfo;
  #endif
		if (pFindData->cFileName[0] == '.')
		{
			if (pFindData->cFileName[1] == '\0' ||
				(pFindData->cFileName[1] == '.' &&
				 pFindData->cFileName[2] == '\0'))
			{
				bResult = true;
			}
		}
#else
		struct dirent* pFindData = &((LPXFindData)m_pFoundInfo)->m_dirent;
		if (pFindData->d_name[0] == '.')
		{
			if (pFindData->d_name[1] == '\0' ||
				(pFindData->d_name[1] == '.' &&
				 pFindData->d_name[2] == '\0'))
			{
				bResult = true;
			}
		}
#endif

	}
	return bResult;
XEXCEPT_EXIT
}
bool XFileFind::IsDirectory() const
{
XEXCEPT_ENTRY
#ifdef WIN32
	return MatchesMask(FILE_ATTRIBUTE_DIRECTORY);
#else	/* for UNIX */
	return MatchesMask(S_IFDIR);
#endif
XEXCEPT_EXIT
}
bool XFileFind::IsReadOnly()
{
XEXCEPT_ENTRY
#ifdef WIN32
	return MatchesMask(FILE_ATTRIBUTE_READONLY);
#else	/* for UNIX */
	return MatchesMask(S_IREAD) == true ? false : true;
#endif
XEXCEPT_EXIT
}

#ifdef WIN32
bool XFileFind::IsCompressed() const
{
XEXCEPT_ENTRY
	return MatchesMask(FILE_ATTRIBUTE_COMPRESSED);
XEXCEPT_EXIT
}
bool XFileFind::IsSystem() const
{
XEXCEPT_ENTRY
	return MatchesMask(FILE_ATTRIBUTE_SYSTEM);
XEXCEPT_EXIT
}
bool XFileFind::IsHidden() const
{
XEXCEPT_ENTRY
	return MatchesMask(FILE_ATTRIBUTE_HIDDEN);
XEXCEPT_EXIT
}
bool XFileFind::IsTemporary() const
{
XEXCEPT_ENTRY
	return MatchesMask(FILE_ATTRIBUTE_TEMPORARY);
XEXCEPT_EXIT
}
bool XFileFind::IsNormal() const
{
XEXCEPT_ENTRY
	return MatchesMask(FILE_ATTRIBUTE_NORMAL);
XEXCEPT_EXIT
}
bool XFileFind::IsArchived() const
{
XEXCEPT_ENTRY
	return MatchesMask(FILE_ATTRIBUTE_ARCHIVE);
XEXCEPT_EXIT
}
#else

bool XFileFind::IsOwnerRead() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IRUSR);
XEXCEPT_EXIT
}
bool XFileFind::IsOwnerWrite() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IWUSR);
XEXCEPT_EXIT
}
bool XFileFind::IsOwnerExecute() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IXUSR);
XEXCEPT_EXIT
}
bool XFileFind::IsGroupRead() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IRGRP);
XEXCEPT_EXIT
}
bool XFileFind::IsGroupWrite() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IWGRP);
XEXCEPT_EXIT
}
bool XFileFind::IsGroupExecute() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IXGRP);
XEXCEPT_EXIT
}
bool XFileFind::IsOtherRead() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IROTH);
XEXCEPT_EXIT
}
bool XFileFind::IsOtherWrite() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IWOTH);
XEXCEPT_EXIT
}
bool XFileFind::IsOtherExecute() const
{
XEXCEPT_ENTRY
	return MatchesMask(S_IXOTH);
XEXCEPT_EXIT
}
#endif




bool XFileFind::MatchesMask(int dwMask) const
{
XEXCEPT_ENTRY
	_XASSERT(m_hContext != NULL);

	if (m_pFoundInfo != NULL) {
#ifdef WIN32
		return (!!(((LPWIN32_FIND_DATA) m_pFoundInfo)->dwFileAttributes & dwMask));
#else
		return (!!(((LPXFindData)m_pFoundInfo)->m_stat.st_mode & dwMask));
#endif
	} else {
		return false;
	}
XEXCEPT_EXIT
}







