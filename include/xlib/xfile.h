/**
 * @file xfile.h
 * @brief ファイルアクセス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_XFILE_HEASER_FILE__
#define __X_XFILE_HEASER_FILE__

#include "xlib/xlib_head.h"
#include <stdio.h>
#include <limits.h>
#ifdef WIN32

#else
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include <unistd.h>

  #ifndef _MAX_PATH
	#define _MAX_PATH	PATH_MAX
  #endif
#endif

#include "xtime.h"
#include "xio.h"


#ifdef WIN32
  /* @brief	デフォルトディレクトリ区切り文字 */
  #define X_FILE_DIRECTORY_SEP		_T('\\')
#elif __cygwin__
  /* @brief	デフォルトディレクトリ区切り文字 */
  #define X_FILE_DIRECTORY_SEP		_T('\\')
#elif __vms__
  /* @brief	デフォルトディレクトリ区切り文字 */
  #define X_FILE_DIRECTORY_SEP		_T('\\')
#else
  /* @brief	デフォルトディレクトリ区切り文字 */
  #define X_FILE_DIRECTORY_SEP		_T('/')
#endif //



/**
 * @brief	ファイル名
 */
class XLIB_API XFileName : public XStrBuff
{
public:
	/**
	 * @brief	コンストラクタ
	 */
	XFileName();
	/**
	 * @brief	コンストラクタ
	 * @param lpsz 初期化文字列
	 */
	XFileName(const char* lpsz);
	/**
	 * @brief	コンストラクタ
	 * @param lpsz 初期化文字列
	 */
	XFileName(const wchar_t* lpsz);
	/**
	 * @brief	コンストラクタ
	 * @param strSrc 初期化文字列
	 */
	XFileName(XFileName &strSrc);
	/** @brief	デストラクタ */
	virtual ~XFileName();
	/**
	 * @brief	文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XFileName& operator=(const char* lpsz);
	/**
	 * @brief	文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XFileName& operator=(const wchar_t* lpsz);

	/**
	 * @brief	フルパス名を作成します。
	 * @param lpszPath パス名を指定します。
	 * @param pDir パス名のディレクトリ部を返します。必要ない場合はNULLを指定します。
	 * @param pFile パス名のファイル部を返します。必要ない場合はNULLを指定します。
	 * @param pExp パス名の拡張子部を返します。必要ない場合はNULLを指定します。
	 * @param cDirSep ディレクトリの区切り文字を指定します。デフォルトはX_FILE_DIRECTORY_SEP。
	 * @return true 成功 / false 失敗
	 */
	bool MakeFullPath(const char* lpszPath, 
					  char* *pDir = NULL, 
					  char* *pFile = NULL, 
					  char* *pExp = NULL); 
	bool MakeFullPath(const wchar_t* lpszPath, 
					  wchar_t* *pDir = NULL, 
					  wchar_t* *pFile = NULL, 
					  wchar_t* *pExp = NULL); 
	/**
	 * @brief	ファイル名文字列を取得します。
	 * @return ファイル名文字列
	 */
	XStrBuff GetFileName() const;
	/**
	 * @brief	ファイル名の拡張子文字列を取得します。
	 * @return ファイル名の拡張子文字列
	 */
	XStrBuff GetFileExt() const;
	/**
	 * @brief	ファイルパス文字列を取得します。
	 * @return ファイルパス文字列
	 */
	XStrBuff GetPathName() const;
	/**
	 * @brief	ディレクトリ文字列を取得します。
	 * @return ディレクトリ文字列
	 */
	XStrBuff GetDirectory() const;


	/**
	 * @brief	カレントディレクトリを取得
	 * @param[in] Buff ディレクトリ文字を受け取るバッファ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	static bool Getcwd(XStrBuff& Buff);
	/**
	 * @brief	カレントディレクトリを変更します
	 * @param[in] pDir カレントディレクトリを変更します。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	static bool Cdir(const char* pDir);
	/** XFileName::chdir(const char*) */
	static bool Cdir(const wchar_t* pDir);
	/**
	 * @brief	ファイルを削除します。
	 * @param[in] pFile 削除するファイル
	 * @retval true 成功
	 * @retval false 失敗
	 */
	static bool Remove(const char* pFile);
	/** XFileName::remove(const char*) */
	static bool Remove(const wchar_t* pFile);

	/**	@brief	ファイルをリネームします。
	 *
	 *	@param	pOldFile	古い名前へのポインタ
	 *	@param	pNewFile	新しい名前へのポインタ
	 *	@retval true 成功
	 *	@retval false 失敗	
	 */
	static bool rename(const char* pOldFile, const char* pNewFile);
	/** XFileName::Rename(const char*) */
	static bool rename(const wchar_t* pOldFile, const wchar_t* pNewFile);
};

/**
 * @brief	ファイルステータスデータ
 */
struct XLIB_API XFileStatus
{
	/** @brief	作成日 */
	XTime         m_ctime;
	/** @brief	変更日 */
	XTime         m_mtime;
	/** @brief	アクセス日 */
	XTime         m_atime;
	/** @brief	サイズ */
	long          m_size;
	/** @brief	属性 */
	unsigned long m_attribute;
	/** @brief	ステータス */
	struct stat   m_dStat;
	/** @brief	ファイル名 */
	XFileName	  m_strFileName;
};

/**
 * @brief	ファイルアクセスクラス
 */
class XLIB_API XFile : public XIO
{
public:
	enum SeekPosition {
		begin   = SEEK_SET, 
		current = SEEK_CUR, 
		end     = SEEK_END  
	};

	/** @brief	コンストラクタ */
	XFile();
	/** @brief	デストラクタ */
	virtual ~XFile();

	/** @copydoc XIO::Close() */
	virtual bool Close();


	/**
	 * @brief	ファイルを作成します。
	 * @param[in] filename ファイル名文字列
	 * @param[in] bExistErr 作成するファイルが既に存在するとエラーとする場合trueを指定する。
	 * @param[in] bAppend 追加モードで作成する場合はtrueを指定する。
	 * @param[in] bShare 共有モードでオープンする場合trueを指定する。
	 * @param[in] pmode アクセス権。各OSのopenシステムコール参照されたし。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Create(const TCHAR *filename, bool bExistErr = true, bool bAppend = false, bool bShare = true, int pmode = 0666);
	/**
	 * @brief	ファイルをオープンします。
	 * @param[in] filename ファイル名文字列
	 * @param[in] bReadOnly 読み込み専用の場合はtrueを指定する。
	 * @param[in] bAppend 追加モードで作成する場合はtrueを指定する。
	 * @param[in] bTruc ファイルをオープンした時、ファイルの長さを０に切り詰める場合trueをし指定する。
	 * @param[in] bShare 共有モードでオープンする場合trueを指定する。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Open(const TCHAR *filename, bool bReadOnly = false, bool bAppend = true, bool bTruc = false, bool bShare = true);
	/** @copydoc XFile::Create(const TCHAR *, bool , bool , bool , int ) */
	virtual bool CreateA(const char *filename,    bool bExistErr, bool bAppend, bool bShare, int pmode);
	/** @copydoc XFile::Create(const TCHAR *, bool , bool , bool , int ) */
	virtual bool CreateW(const wchar_t *filename, bool bExistErr, bool bAppend, bool bShare, int pmode);
	/** @copydoc XFile::Open(const TCHAR *, bool , bool , bool , bool) */
	virtual bool OpenA(const char *filename,    bool bReadOnly, bool bAppend, bool bTruc,bool bShare);
	/** @copydoc XFile::Open(const TCHAR *, bool , bool , bool , bool) */
	virtual bool OpenW(const wchar_t *filename, bool bReadOnly, bool bAppend, bool bTruc,bool bShare);
	/**
	 * @brief	ファイルをオープンします。各OSのopenシステムコールを直接呼出します。
	 * @param[in] filename ファイルのパス文字列
	 * @param[in] flg オープンフラグ。各OSのopenシステムコール参照されたし。
	 * @param[in] bShare 共有モードでオープンする場合trueを指定する。
	 * @param[in] pmode アクセス権。各OSのopenシステムコール参照されたし。
	 */
	virtual bool OpenRawA(const char *filename,    int flg, bool bShare, int pmode);
	/** @copydoc XFile::OpenRawA(const char*,int,bool,int) */
	virtual bool OpenRawW(const wchar_t *filename, int flg, bool bShare, int pmode);


	/**
	 * @brief	ファイルインスタンスの複製を作成します。
	 * @return ファイル複製オブジェクト。失敗した場合NULLを返します。
	 */
	XFile* Duplicate() const;
	/**
	 * @brief	バッファをフラッシュします。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Flush();
	/**
	 * @brief	ファイルのサイズを取得します。
	 * @return ファイルの長さ。失敗した場合は0．
	 */
	virtual size_t GetLength();
	/**
	 * @brief	ファイル時間を取得します。
	 * @param[out] cTime 作成時間
	 * @param[out] mTime 変更時間
	 * @param[out] aTime アクセス時間
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetFileTime(XTime *cTime, XTime *mTime, XTime *aTime);
	/**
	 * @brief	ファイルのモードを取得します。
	 * @return ファイルのモード。失敗した場合は-１．
	 */
	unsigned short GetMode();

	/**
	 * @brief	指定された位置にファイルポインタを移動します。
	 * @param[in] offset originからのバイト数
	 * @param[in] origin  初期位置
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Seek(long offset, SeekPosition origin);
	/**
	 * @brief	ファイルの位置を先頭に移動します。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SeekFirst();
	/**
	 * @brief	ファイルの位置を最後に移動します。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SeekLast();
	/**
	 * @brief	ファイル ポインタの現在位置を得ます。
	 * @return 現在のファイル位置を返します
	 */
	long GetCurrentPos();
	
	/**
	 * @brief	テキストファイルとして読み込み、指定バッファにセットする。BOMがあった場合は、BOMのフォーマットから変換する。
	 * @param[out] buf  バッファ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool readAllText(XStrBuff& buf);
	

	/**
	 * @brief	ファイルのステータスを取得
	 * @param[in] lpszFileName ファイルのパス
	 * @param[out] rStatus ステータスを受け取るデータ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	static bool GetStatus(const char* lpszFileName, XFileStatus& rStatus);
	/**
	 * @brief	ファイルのステータスを取得
	 * @param[in] lpszFileName ファイルのパス
	 * @param[out] rStatus ステータスを受け取るデータ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	static bool GetStatus(const wchar_t* lpszFileName, XFileStatus& rStatus);
// Direcotry Operation
	/**	@brief	カレントディレクトリを取得します。
	 *	@return	カレントディレクトリの文字列
	 */
	static XFileName getCurrentDirectory();
	
	/**	@brief	カレントディレクトリを変更する。
	 *	@param	lpszDirectoryName	変更するディレクトリ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool changeCurrentDirectory(const char* lpszDirectoryName);
	/**	@brief	カレントディレクトリを変更する。
	 *	@param	lpszDirectoryName	変更するディレクトリ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool changeCurrentDirectory(const wchar_t* lpszDirectoryName);
	
	/**	@brief	ディレクトリを削除する。
	 *	@param	lpszDirectoryName	削除するディレクトリ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool removeDirectory(const char* lpszDirectoryName);
	/**	@brief	ディレクトリを削除する。
	 *	@param	lpszDirectoryName	削除するディレクトリ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool removeDirectory(const wchar_t* lpszDirectoryName);

	/**	@brief	ディレクトリを作成する。
	 *	@param	lpszDirectoryName	作成するディレクトリ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool makeDirectory(const char* lpszDirectoryName);
	/**	@brief	ディレクトリを作成する。
	 *	@param	lpszDirectoryName	作成するディレクトリ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	static bool makeDirectory(const wchar_t* lpszDirectoryName);


	static errno_t fopenA(FILE** ppFp, const char* p_file_path, const char* p_mode);
	static errno_t fopenW(FILE** ppFp, const wchar_t* p_file_path, const wchar_t* p_mode);
	static errno_t fopenT(FILE** ppFp, const TCHAR* p_file_path, const TCHAR* p_mode);

	static int     vfprintfA(FILE * fp, const char * _Format, va_list varg);
	static int     vfprintfW(FILE * fp, const wchar_t * _Format, va_list varg);
	static int     vfprintfT(FILE * fp, const TCHAR * _Format, va_list varg);
	static int     fprintfA(FILE* fp, const char* format, ...);
	static int     fprintfW(FILE* fp, const wchar_t* format, ...);
	static int     fprintfT(FILE* fp, const TCHAR* format, ...);

	static int     fputcA(int ch, FILE* fp);
	static int     fputcW(wchar_t ch, FILE* fp);
	static int     fputcT(int ch, FILE* fp);


	static int      fputsA(const char* p, FILE* fp);
	static int      fputsW(const wchar_t* p, FILE* fp);
	static int      fputsT(const TCHAR* p, FILE* fp);

	static char*    fgetsA(char* pbuff, int count, FILE* fp);
	static wchar_t* fgetsW(wchar_t* pbuff, int count, FILE* fp);
	static TCHAR*   fgetsT(TCHAR* pbuff, int count, FILE* fp);
	static int      fgetcA(FILE* fp);
	static wint_t   fgetcW(FILE* fp);
	static int      fgetcT(FILE* fp);


// Raq Member	
	/**  XIO::WriteRaw(const void* , size_t , int ) */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0);
	/**  XIO::ReadRaw(void* , size_t , int ) */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0);
protected:
	/* ファイル名 */
	XFileName	m_strFileName;
	/* ファイル */
	int			m_iFile;
};

/**
 * @brief	ファイルの検索クラス
 */
class XLIB_API XFileFind
{
public:
#ifndef WIN32
	/** @brief	ファイルデータクラス */
	typedef struct _XFindData
	{
		/** @brief	ステータスデータ */
		struct stat    m_stat;
		/** @brief	ディレクトリORファイルデータ */
		struct dirent  m_dirent;
	}XFindData, *LPXFindData;
#endif
	/** @brief	コンストラクタ */
	XFileFind();
	/** @brief	デストラクタ */
	virtual ~XFileFind();

	/**
	 * @brief	ファイル検索を開始します
	 * @param[in] pstrName 検索のスタートポイント
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool FindFile(const char* pstrName = NULL);
	/**
	 * @brief	ファイル検索を開始します
	 * @param[in] pstrName 検索のスタートポイント
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool FindFile(const wchar_t* pstrName = NULL);
	/**
	 * @brief	次のファイルを検索します。
	 * @retval true まだファイルがあります。
	 * @retval false これ以上ファイルはない。
	 */
	virtual bool NextFile();
	/**
	 * @brief	ファイルの検索を終了します。
	 */
	virtual void Close();

	/**
	 * ファイル名を取得します。
	 * @return ファイル名
	 */
	XFileName GetFileName() const;

	/**
	 * @brief	ファイルがドット(カレントまたは、親)であるか取得します。
	 * @retval true ドットである。
	 * @retval false ドットでない
	 */
	virtual bool IsDots();
	/**
	 * @brief	ファイルがディレクトリであるか取得します。
	 * @retval true ディレクトリである。
	 * @retval false ディレクトリでない
	 */
	virtual bool IsDirectory() const;
	/**
	 * @brief	ファイルが読み取り専用であるか取得します。
	 * @retval true 読み取り専用である。
	 * @retval false 読み取り専用でない
	 */
	virtual bool IsReadOnly();
#ifdef WIN32
	virtual bool IsCompressed() const;
	virtual bool IsSystem() const;
	virtual bool IsHidden() const;
	virtual bool IsTemporary() const;
	virtual bool IsNormal() const;
	virtual bool IsArchived() const;
#else
	virtual bool IsOwnerRead() const;
	virtual bool IsOwnerWrite() const;
	virtual bool IsOwnerExecute() const;
	virtual bool IsGroupRead() const;
	virtual bool IsGroupWrite() const;
	virtual bool IsGroupExecute() const;
	virtual bool IsOtherRead() const;
	virtual bool IsOtherWrite() const;
	virtual bool IsOtherExecute() const;
#endif
protected:
	/* @brief	コンテキストを閉じます */
	virtual void CloseContext();
	bool MatchesMask(int dwMask) const;
protected:
	/* @brief	ファイルデータ */
	void* m_pFoundInfo;
	/* @brief	次のファイルデータ */
	void* m_pNextInfo;
#ifdef WIN32
	/* @brief	コンテキスト */
	void *	m_hContext;
#else	/* for UNIX */
	/* @brief	コンテキスト */
	DIR*	m_hContext;
#endif
	/* @brief	検索の終了に達したかどうかのフラグ */
	bool	 m_bGotLast;
	/* @brief	ルートディレクトリ */
	XStrBuff m_strRoot;
	/* @brief	ディレクトリの区切り文字 */
	TCHAR	 m_chDirSeparator;
};


#include "xlib/xlib_tail.h"
#endif // __X_XFILE_HEASER_FILE__
