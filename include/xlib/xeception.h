/**
 * @file xeception.h
 * @brief ��O�I�u�W�F�N�g�̒�`
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_EXCEPTION_HEASER_FILE__
#define __X_EXCEPTION_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"
#include <vector>
#include <exception>
#include <iostream>
#ifdef WIN32
  #include <eh.h>
  #include <crtdbg.h>
#else
  #include <errno.h>
  #include <stdarg.h> 
  #include <assert.h>
  #include <stdio.h>
#endif

#ifdef WIN32
	#ifdef _DEBUG
		XLIB_API void XAssertFailedLine(LPCSTR lpszFileName, int nLine);
		#define _XASSERT(f) if (!(f)) { XAssertFailedLine(__FILE__, __LINE__); }
	#else
		#define _XASSERT(f)		((void)0)
	#endif
#else	/* WIN32 */
	#ifdef _DEBUG
		#define _XASSERT(f)		assert
	#else
		#define _XASSERT(f)		((void)0)
	#endif
#endif


#define XLIB_LOG_LEVEL_NORMAL		0x00000001
#define XLIB_LOG_LEVEL_ERROR		0x00000002
#define XLIB_LOG_LEVEL_WARNING		0x00000004
#define XLIB_LOG_LEVEL_DEBUG		0x00000008

/**
 * @brief	�f�o�b�O���j�^�[�ɕ�����𑗂�܂��B
 * @param[in] lpszFormat �t�H�[�}�b�g������
 * @param[in] ... �t�H�[�}�b�g�I�v�V����
 */
XLIB_API void XTRACE(const TCHAR* lpszFormat, ...);


#define XEXCEPT_THROW(category, osError, hint)		throw XExcept(category, __FILE__, __LINE__, osError, hint)
#define XEXCEPT_THROW_STR(category, what, osError, hint)		throw XExcept(category, what, __FILE__, __LINE__, osError, hint)
#define XEXCEPT_ADD_ERROR(pe, category) (pe)->addStackTrace((category), __FILE__, __LINE__,0,0)
#  define XEXCEPT_RETHROW(ex, category) \
	do { XEXCEPT_ADD_ERROR(&(ex), XExcept_RETHROW); throw; } while (0)


#define XEXCEPT_ENTRY	 try {

#define XEXCEPT_EXIT  }		\
     catch (XExcept& ex) { XEXCEPT_RETHROW(ex, XExcept_RETHROW); }	\
     catch (std::bad_alloc&) { XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0); }	\
     catch (std::exception&) { XEXCEPT_THROW(XExcept_NO_SUPPORT, 0, 0); }	\
     catch (...) { XEXCEPT_THROW(XExcept_UNKNOWN, 0, 0); }


#if _MSC_VER >= 1400 // VC2005(VC8.0)�ȍ~
	void XlibTranslateUt_StructuredException(unsigned int ExceptionCode, PEXCEPTION_POINTERS ExceptionPointers);
	void XlibTranslateInvalidParameterException(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
#	define XLIB_SET_SE_TRANSLATOR _set_se_translator(XlibTranslateUt_StructuredException);
#   define XLIB_SET_PE_TRANSLATOR _set_invalid_parameter_handler(XlibTranslateInvalidParameterException);
#else
#	define XLIB_SET_SE_TRANSLATOR 
#   define XLIB_SET_PE_TRANSLATOR 
#endif


class XLIB_API XExcept;


/**	@brief	��O�J�e�S���[ */
enum XExceptCATEGORY {
	XExcept_UNKNOWN,				///< ����`
	XExcept_STRUCTURED_EXCEPT,		///< �\������O
	XExcept_UNKNOWN_PARAM,			///< �s���ȃp�����[�^
	XExcept_OVER_INDEX,				///< �C���f�b�N�X�I�[�o�[�G���[
	XExcept_RETHROW,				///< ���X���[
	XExcept_MEMORY_ALLOC,			///< �������[�m��
	XExcept_NO_SUPPORT,				///< �T�|�[�g�O
	XExcept_RESOURCE,				///< ���\�[�X
	XExcept_ARCHIVE,				///< �A�[�J�C�u
	XExcept_FILE,					///< �t�@�C��
	XExcept_THREAD,					///< �X���b�h
	XExcept_SOCKET,					///< �\�P�b�g
	XExcept_XDATA,					///< XData�֘A
	XExcept_SECS,					///< SECS
	XExcept_SQLite,					///< SQLite
	XExcept_DB,						///< DB
	XExcept_APP,					///< Application
};


/**	@brief	�X�^�b�N�A�C�e�� */
class XLIB_API XExceptStackItem
{
public:
	XExceptStackItem(XExceptCATEGORY category, const char* file, int line, unsigned long ulOsErrorCode, unsigned long ulHint, const TCHAR* pErrMsg = NULL)
		:	m_catgegory(category)
		,	m_line(line)
		,	m_file(file)	
		,	m_ulOsErrorCode(ulOsErrorCode)
		,	m_ulHint(ulHint)
		,	m_what(_T("")){
		if( pErrMsg != NULL ) {
			m_what = pErrMsg;
		}
	}
	const char*			m_file;
	int					m_line;
	XExceptCATEGORY		m_catgegory;
	unsigned long		m_ulOsErrorCode;
	unsigned long		m_ulHint;
	XStrBuff			m_what;
};


DLL_EXTERN template class XLIB_API std::vector<XExceptStackItem*>;
typedef XLIB_API std::vector<XExceptStackItem*> XExceptStackItems;


/**
 * @brief	��O�x�[�X�N���X
 */
class XLIB_API XExcept : public std::exception
{
public:

	static void setThrowBadAlloc();
	/** strerror�֐��̑��� */
	static XStrBuff getSrdErrorStr(errno_t err);
public:
	/** �R���X�g���N�^ */
	XExcept(){ m_nOsErrorCode = 0; }
	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	category	�J�e�S���[
	 *	@param	file	�\�[�X�t�@�C��
	 *	@param	line	���C��
	 *	@param	ulOsErrorCode	OS�ˑ��̃G���[�R�[�h
	 *	@param	ulHint	�G���[�q���g
	 */
	XExcept(XExceptCATEGORY category, const char* file, int line, unsigned long ulOsErrorCode = 0, unsigned long ulHint = 0);
	
	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	category	�J�e�S���[
	 *	@param	file	�\�[�X�t�@�C��
	 *	@param	line	���C��
	 *	@param	p_what	����������
	 *	@param	ulOsErrorCode	OS�ˑ��̃G���[�R�[�h
	 *	@param	ulHint	�G���[�q���g
	 */
	XExcept(XExceptCATEGORY category,const char* p_what,  const char* file, int line, unsigned long ulOsErrorCode = 0, unsigned long ulHint = 0);
	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	category	�J�e�S���[
	 *	@param	file	�\�[�X�t�@�C��
	 *	@param	line	���C��
	 *	@param	p_what	����������
	 *	@param	ulOsErrorCode	OS�ˑ��̃G���[�R�[�h
	 *	@param	ulHint	�G���[�q���g
	 */
	XExcept(XExceptCATEGORY category,const wchar_t* p_what,  const char* file, int line, unsigned long ulOsErrorCode = 0, unsigned long ulHint = 0);
	
	/** �f�X�g���N�^ */
	virtual ~XExcept() throw ();

	/// ��O�̊T�v��Ԃ��B  std::exception�̃I�[�o�[���C�g
	virtual const char* what() const throw ();
	
	/// ��O�̊T�v��Ԃ��B
	const wchar_t* whatW() throw ();
	
	/// ��O�̊T�v��Ԃ��B
	const TCHAR* whatT() throw ();


	void addStackTrace(XExceptCATEGORY category, const char* file, int line, unsigned long ulOsErrorCode = 0, unsigned long ulHint = 0, const TCHAR* pErrMsg = 0);

#ifdef _UNICODE
	virtual void print(XLIB_STD_OUT_STR_STREAM& out = std::wcerr);
	virtual void printStackTrace(XLIB_STD_OUT_STR_STREAM& out = std::wcerr);
#else
	virtual void print(XLIB_STD_OUT_STR_STREAM& out = std::cerr);
	virtual void printStackTrace(XLIB_STD_OUT_STR_STREAM& out = std::cerr);
#endif
	/** @brief	OS�G���[�R�[�h���擾
	 *	@return OS�G���[�R�[�h
	 */
	unsigned long getOsErrorCode() const;
	/** @brief	�q���g�l���擾
	 *	@return �q���g�l
	 */
	unsigned long getHint() const;

	/**	@brief	�q���g�l��ݒ�
	 *	@param	ulHint	�q���g�l
	 */
	void setHist(const unsigned long ulHint);

	/** @brief	�G���[�J�e�S���[���擾
	 *	@return �G���[�J�e�S���[
	 */
	XExceptCATEGORY getCategory() const;

	/* @brief �G���[�R�[�h */
	unsigned long		m_nOsErrorCode;
private:
	XExcept& operator=(const XExcept& rhs);
private:
	/*	@brief	�G���[�J�e�S���[	*/
	XExceptCATEGORY		m_catgegory;
	/*	@brief	�G���[�q���g	*/
	unsigned long	m_ulHint;
	/*	@brief	�X�^�b�N�A�C�e��	*/
	XExceptStackItems			m_stacItms;
	/*	@brief	�G���[����������p�̃o�b�t�@	*/
	mutable XStrBuff	m_what;
	const char* p_strWat;
};



#define ___XEXCEPT_CHECK_VALID_TRUE(x) if( x == false ) {XExceptInvalidParam(RNAME, __FILE__, __LINE__); }
void XExceptInvalidParam(const char* p_func, const char* p_f_name, int line);

#include "xlib/xlib_tail.h"
#endif // __X_EXCEPTION_HEASER_FILE__



