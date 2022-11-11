/**
 * @file xfile.h
 * @brief �t�@�C���A�N�Z�X
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
  /* @brief	�f�t�H���g�f�B���N�g����؂蕶�� */
  #define X_FILE_DIRECTORY_SEP		_T('\\')
#elif __cygwin__
  /* @brief	�f�t�H���g�f�B���N�g����؂蕶�� */
  #define X_FILE_DIRECTORY_SEP		_T('\\')
#elif __vms__
  /* @brief	�f�t�H���g�f�B���N�g����؂蕶�� */
  #define X_FILE_DIRECTORY_SEP		_T('\\')
#else
  /* @brief	�f�t�H���g�f�B���N�g����؂蕶�� */
  #define X_FILE_DIRECTORY_SEP		_T('/')
#endif //



/**
 * @brief	�t�@�C����
 */
class XLIB_API XFileName : public XStrBuff
{
public:
	/**
	 * @brief	�R���X�g���N�^
	 */
	XFileName();
	/**
	 * @brief	�R���X�g���N�^
	 * @param lpsz ������������
	 */
	XFileName(const char* lpsz);
	/**
	 * @brief	�R���X�g���N�^
	 * @param lpsz ������������
	 */
	XFileName(const wchar_t* lpsz);
	/**
	 * @brief	�R���X�g���N�^
	 * @param strSrc ������������
	 */
	XFileName(XFileName &strSrc);
	/** @brief	�f�X�g���N�^ */
	virtual ~XFileName();
	/**
	 * @brief	�������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XFileName& operator=(const char* lpsz);
	/**
	 * @brief	�������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XFileName& operator=(const wchar_t* lpsz);

	/**
	 * @brief	�t���p�X�����쐬���܂��B
	 * @param lpszPath �p�X�����w�肵�܂��B
	 * @param pDir �p�X���̃f�B���N�g������Ԃ��܂��B�K�v�Ȃ��ꍇ��NULL���w�肵�܂��B
	 * @param pFile �p�X���̃t�@�C������Ԃ��܂��B�K�v�Ȃ��ꍇ��NULL���w�肵�܂��B
	 * @param pExp �p�X���̊g���q����Ԃ��܂��B�K�v�Ȃ��ꍇ��NULL���w�肵�܂��B
	 * @param cDirSep �f�B���N�g���̋�؂蕶�����w�肵�܂��B�f�t�H���g��X_FILE_DIRECTORY_SEP�B
	 * @return true ���� / false ���s
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
	 * @brief	�t�@�C������������擾���܂��B
	 * @return �t�@�C����������
	 */
	XStrBuff GetFileName() const;
	/**
	 * @brief	�t�@�C�����̊g���q��������擾���܂��B
	 * @return �t�@�C�����̊g���q������
	 */
	XStrBuff GetFileExt() const;
	/**
	 * @brief	�t�@�C���p�X��������擾���܂��B
	 * @return �t�@�C���p�X������
	 */
	XStrBuff GetPathName() const;
	/**
	 * @brief	�f�B���N�g����������擾���܂��B
	 * @return �f�B���N�g��������
	 */
	XStrBuff GetDirectory() const;


	/**
	 * @brief	�J�����g�f�B���N�g�����擾
	 * @param[in] Buff �f�B���N�g���������󂯎��o�b�t�@
	 * @retval true ����
	 * @retval false ���s
	 */
	static bool Getcwd(XStrBuff& Buff);
	/**
	 * @brief	�J�����g�f�B���N�g����ύX���܂�
	 * @param[in] pDir �J�����g�f�B���N�g����ύX���܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	static bool Cdir(const char* pDir);
	/** XFileName::chdir(const char*) */
	static bool Cdir(const wchar_t* pDir);
	/**
	 * @brief	�t�@�C�����폜���܂��B
	 * @param[in] pFile �폜����t�@�C��
	 * @retval true ����
	 * @retval false ���s
	 */
	static bool Remove(const char* pFile);
	/** XFileName::remove(const char*) */
	static bool Remove(const wchar_t* pFile);

	/**	@brief	�t�@�C�������l�[�����܂��B
	 *
	 *	@param	pOldFile	�Â����O�ւ̃|�C���^
	 *	@param	pNewFile	�V�������O�ւ̃|�C���^
	 *	@retval true ����
	 *	@retval false ���s	
	 */
	static bool rename(const char* pOldFile, const char* pNewFile);
	/** XFileName::Rename(const char*) */
	static bool rename(const wchar_t* pOldFile, const wchar_t* pNewFile);
};

/**
 * @brief	�t�@�C���X�e�[�^�X�f�[�^
 */
struct XLIB_API XFileStatus
{
	/** @brief	�쐬�� */
	XTime         m_ctime;
	/** @brief	�ύX�� */
	XTime         m_mtime;
	/** @brief	�A�N�Z�X�� */
	XTime         m_atime;
	/** @brief	�T�C�Y */
	long          m_size;
	/** @brief	���� */
	unsigned long m_attribute;
	/** @brief	�X�e�[�^�X */
	struct stat   m_dStat;
	/** @brief	�t�@�C���� */
	XFileName	  m_strFileName;
};

/**
 * @brief	�t�@�C���A�N�Z�X�N���X
 */
class XLIB_API XFile : public XIO
{
public:
	enum SeekPosition {
		begin   = SEEK_SET, 
		current = SEEK_CUR, 
		end     = SEEK_END  
	};

	/** @brief	�R���X�g���N�^ */
	XFile();
	/** @brief	�f�X�g���N�^ */
	virtual ~XFile();

	/** @copydoc XIO::Close() */
	virtual bool Close();


	/**
	 * @brief	�t�@�C�����쐬���܂��B
	 * @param[in] filename �t�@�C����������
	 * @param[in] bExistErr �쐬����t�@�C�������ɑ��݂���ƃG���[�Ƃ���ꍇtrue���w�肷��B
	 * @param[in] bAppend �ǉ����[�h�ō쐬����ꍇ��true���w�肷��B
	 * @param[in] bShare ���L���[�h�ŃI�[�v������ꍇtrue���w�肷��B
	 * @param[in] pmode �A�N�Z�X���B�eOS��open�V�X�e���R�[���Q�Ƃ��ꂽ���B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Create(const TCHAR *filename, bool bExistErr = true, bool bAppend = false, bool bShare = true, int pmode = 0666);
	/**
	 * @brief	�t�@�C�����I�[�v�����܂��B
	 * @param[in] filename �t�@�C����������
	 * @param[in] bReadOnly �ǂݍ��ݐ�p�̏ꍇ��true���w�肷��B
	 * @param[in] bAppend �ǉ����[�h�ō쐬����ꍇ��true���w�肷��B
	 * @param[in] bTruc �t�@�C�����I�[�v���������A�t�@�C���̒������O�ɐ؂�l�߂�ꍇtrue�����w�肷��B
	 * @param[in] bShare ���L���[�h�ŃI�[�v������ꍇtrue���w�肷��B
	 * @retval true ����
	 * @retval false ���s
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
	 * @brief	�t�@�C�����I�[�v�����܂��B�eOS��open�V�X�e���R�[���𒼐ڌďo���܂��B
	 * @param[in] filename �t�@�C���̃p�X������
	 * @param[in] flg �I�[�v���t���O�B�eOS��open�V�X�e���R�[���Q�Ƃ��ꂽ���B
	 * @param[in] bShare ���L���[�h�ŃI�[�v������ꍇtrue���w�肷��B
	 * @param[in] pmode �A�N�Z�X���B�eOS��open�V�X�e���R�[���Q�Ƃ��ꂽ���B
	 */
	virtual bool OpenRawA(const char *filename,    int flg, bool bShare, int pmode);
	/** @copydoc XFile::OpenRawA(const char*,int,bool,int) */
	virtual bool OpenRawW(const wchar_t *filename, int flg, bool bShare, int pmode);


	/**
	 * @brief	�t�@�C���C���X�^���X�̕������쐬���܂��B
	 * @return �t�@�C�������I�u�W�F�N�g�B���s�����ꍇNULL��Ԃ��܂��B
	 */
	XFile* Duplicate() const;
	/**
	 * @brief	�o�b�t�@���t���b�V�����܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Flush();
	/**
	 * @brief	�t�@�C���̃T�C�Y���擾���܂��B
	 * @return �t�@�C���̒����B���s�����ꍇ��0�D
	 */
	virtual size_t GetLength();
	/**
	 * @brief	�t�@�C�����Ԃ��擾���܂��B
	 * @param[out] cTime �쐬����
	 * @param[out] mTime �ύX����
	 * @param[out] aTime �A�N�Z�X����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetFileTime(XTime *cTime, XTime *mTime, XTime *aTime);
	/**
	 * @brief	�t�@�C���̃��[�h���擾���܂��B
	 * @return �t�@�C���̃��[�h�B���s�����ꍇ��-�P�D
	 */
	unsigned short GetMode();

	/**
	 * @brief	�w�肳�ꂽ�ʒu�Ƀt�@�C���|�C���^���ړ����܂��B
	 * @param[in] offset origin����̃o�C�g��
	 * @param[in] origin  �����ʒu
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Seek(long offset, SeekPosition origin);
	/**
	 * @brief	�t�@�C���̈ʒu��擪�Ɉړ����܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SeekFirst();
	/**
	 * @brief	�t�@�C���̈ʒu���Ō�Ɉړ����܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SeekLast();
	/**
	 * @brief	�t�@�C�� �|�C���^�̌��݈ʒu�𓾂܂��B
	 * @return ���݂̃t�@�C���ʒu��Ԃ��܂�
	 */
	long GetCurrentPos();
	
	/**
	 * @brief	�e�L�X�g�t�@�C���Ƃ��ēǂݍ��݁A�w��o�b�t�@�ɃZ�b�g����BBOM���������ꍇ�́ABOM�̃t�H�[�}�b�g����ϊ�����B
	 * @param[out] buf  �o�b�t�@
	 * @retval true ����
	 * @retval false ���s
	 */
	bool readAllText(XStrBuff& buf);
	

	/**
	 * @brief	�t�@�C���̃X�e�[�^�X���擾
	 * @param[in] lpszFileName �t�@�C���̃p�X
	 * @param[out] rStatus �X�e�[�^�X���󂯎��f�[�^
	 * @retval true ����
	 * @retval false ���s
	 */
	static bool GetStatus(const char* lpszFileName, XFileStatus& rStatus);
	/**
	 * @brief	�t�@�C���̃X�e�[�^�X���擾
	 * @param[in] lpszFileName �t�@�C���̃p�X
	 * @param[out] rStatus �X�e�[�^�X���󂯎��f�[�^
	 * @retval true ����
	 * @retval false ���s
	 */
	static bool GetStatus(const wchar_t* lpszFileName, XFileStatus& rStatus);
// Direcotry Operation
	/**	@brief	�J�����g�f�B���N�g�����擾���܂��B
	 *	@return	�J�����g�f�B���N�g���̕�����
	 */
	static XFileName getCurrentDirectory();
	
	/**	@brief	�J�����g�f�B���N�g����ύX����B
	 *	@param	lpszDirectoryName	�ύX����f�B���N�g��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool changeCurrentDirectory(const char* lpszDirectoryName);
	/**	@brief	�J�����g�f�B���N�g����ύX����B
	 *	@param	lpszDirectoryName	�ύX����f�B���N�g��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool changeCurrentDirectory(const wchar_t* lpszDirectoryName);
	
	/**	@brief	�f�B���N�g�����폜����B
	 *	@param	lpszDirectoryName	�폜����f�B���N�g��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool removeDirectory(const char* lpszDirectoryName);
	/**	@brief	�f�B���N�g�����폜����B
	 *	@param	lpszDirectoryName	�폜����f�B���N�g��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool removeDirectory(const wchar_t* lpszDirectoryName);

	/**	@brief	�f�B���N�g�����쐬����B
	 *	@param	lpszDirectoryName	�쐬����f�B���N�g��
	 *	@retval true ����
	 *	@retval false ���s
	 */
	static bool makeDirectory(const char* lpszDirectoryName);
	/**	@brief	�f�B���N�g�����쐬����B
	 *	@param	lpszDirectoryName	�쐬����f�B���N�g��
	 *	@retval true ����
	 *	@retval false ���s
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
	/* �t�@�C���� */
	XFileName	m_strFileName;
	/* �t�@�C�� */
	int			m_iFile;
};

/**
 * @brief	�t�@�C���̌����N���X
 */
class XLIB_API XFileFind
{
public:
#ifndef WIN32
	/** @brief	�t�@�C���f�[�^�N���X */
	typedef struct _XFindData
	{
		/** @brief	�X�e�[�^�X�f�[�^ */
		struct stat    m_stat;
		/** @brief	�f�B���N�g��OR�t�@�C���f�[�^ */
		struct dirent  m_dirent;
	}XFindData, *LPXFindData;
#endif
	/** @brief	�R���X�g���N�^ */
	XFileFind();
	/** @brief	�f�X�g���N�^ */
	virtual ~XFileFind();

	/**
	 * @brief	�t�@�C���������J�n���܂�
	 * @param[in] pstrName �����̃X�^�[�g�|�C���g
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool FindFile(const char* pstrName = NULL);
	/**
	 * @brief	�t�@�C���������J�n���܂�
	 * @param[in] pstrName �����̃X�^�[�g�|�C���g
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool FindFile(const wchar_t* pstrName = NULL);
	/**
	 * @brief	���̃t�@�C�����������܂��B
	 * @retval true �܂��t�@�C��������܂��B
	 * @retval false ����ȏ�t�@�C���͂Ȃ��B
	 */
	virtual bool NextFile();
	/**
	 * @brief	�t�@�C���̌������I�����܂��B
	 */
	virtual void Close();

	/**
	 * �t�@�C�������擾���܂��B
	 * @return �t�@�C����
	 */
	XFileName GetFileName() const;

	/**
	 * @brief	�t�@�C�����h�b�g(�J�����g�܂��́A�e)�ł��邩�擾���܂��B
	 * @retval true �h�b�g�ł���B
	 * @retval false �h�b�g�łȂ�
	 */
	virtual bool IsDots();
	/**
	 * @brief	�t�@�C�����f�B���N�g���ł��邩�擾���܂��B
	 * @retval true �f�B���N�g���ł���B
	 * @retval false �f�B���N�g���łȂ�
	 */
	virtual bool IsDirectory() const;
	/**
	 * @brief	�t�@�C�����ǂݎ���p�ł��邩�擾���܂��B
	 * @retval true �ǂݎ���p�ł���B
	 * @retval false �ǂݎ���p�łȂ�
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
	/* @brief	�R���e�L�X�g����܂� */
	virtual void CloseContext();
	bool MatchesMask(int dwMask) const;
protected:
	/* @brief	�t�@�C���f�[�^ */
	void* m_pFoundInfo;
	/* @brief	���̃t�@�C���f�[�^ */
	void* m_pNextInfo;
#ifdef WIN32
	/* @brief	�R���e�L�X�g */
	void *	m_hContext;
#else	/* for UNIX */
	/* @brief	�R���e�L�X�g */
	DIR*	m_hContext;
#endif
	/* @brief	�����̏I���ɒB�������ǂ����̃t���O */
	bool	 m_bGotLast;
	/* @brief	���[�g�f�B���N�g�� */
	XStrBuff m_strRoot;
	/* @brief	�f�B���N�g���̋�؂蕶�� */
	TCHAR	 m_chDirSeparator;
};


#include "xlib/xlib_tail.h"
#endif // __X_XFILE_HEASER_FILE__
