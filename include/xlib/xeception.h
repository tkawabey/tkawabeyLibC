/**
 * @file xeception.h
 * @brief 例外オブジェクトの定義
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
 * @brief	デバッグモニターに文字列を送ります。
 * @param[in] lpszFormat フォーマット文字列
 * @param[in] ... フォーマットオプション
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


#if _MSC_VER >= 1400 // VC2005(VC8.0)以降
	void XlibTranslateUt_StructuredException(unsigned int ExceptionCode, PEXCEPTION_POINTERS ExceptionPointers);
	void XlibTranslateInvalidParameterException(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
#	define XLIB_SET_SE_TRANSLATOR _set_se_translator(XlibTranslateUt_StructuredException);
#   define XLIB_SET_PE_TRANSLATOR _set_invalid_parameter_handler(XlibTranslateInvalidParameterException);
#else
#	define XLIB_SET_SE_TRANSLATOR 
#   define XLIB_SET_PE_TRANSLATOR 
#endif


class XLIB_API XExcept;


/**	@brief	例外カテゴリー */
enum XExceptCATEGORY {
	XExcept_UNKNOWN,				///< 未定義
	XExcept_STRUCTURED_EXCEPT,		///< 構造化例外
	XExcept_UNKNOWN_PARAM,			///< 不正なパラメータ
	XExcept_OVER_INDEX,				///< インデックスオーバーエラー
	XExcept_RETHROW,				///< リスロー
	XExcept_MEMORY_ALLOC,			///< メモリー確保
	XExcept_NO_SUPPORT,				///< サポート外
	XExcept_RESOURCE,				///< リソース
	XExcept_ARCHIVE,				///< アーカイブ
	XExcept_FILE,					///< ファイル
	XExcept_THREAD,					///< スレッド
	XExcept_SOCKET,					///< ソケット
	XExcept_XDATA,					///< XData関連
	XExcept_SECS,					///< SECS
	XExcept_SQLite,					///< SQLite
	XExcept_DB,						///< DB
	XExcept_APP,					///< Application
};


/**	@brief	スタックアイテム */
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
 * @brief	例外ベースクラス
 */
class XLIB_API XExcept : public std::exception
{
public:

	static void setThrowBadAlloc();
	/** strerror関数の代わり */
	static XStrBuff getSrdErrorStr(errno_t err);
public:
	/** コンストラクタ */
	XExcept(){ m_nOsErrorCode = 0; }
	/**	@brief	コンストラクタ
	 *
	 *	@param	category	カテゴリー
	 *	@param	file	ソースファイル
	 *	@param	line	ライン
	 *	@param	ulOsErrorCode	OS依存のエラーコード
	 *	@param	ulHint	エラーヒント
	 */
	XExcept(XExceptCATEGORY category, const char* file, int line, unsigned long ulOsErrorCode = 0, unsigned long ulHint = 0);
	
	/**	@brief	コンストラクタ
	 *
	 *	@param	category	カテゴリー
	 *	@param	file	ソースファイル
	 *	@param	line	ライン
	 *	@param	p_what	説明文字列
	 *	@param	ulOsErrorCode	OS依存のエラーコード
	 *	@param	ulHint	エラーヒント
	 */
	XExcept(XExceptCATEGORY category,const char* p_what,  const char* file, int line, unsigned long ulOsErrorCode = 0, unsigned long ulHint = 0);
	/**	@brief	コンストラクタ
	 *
	 *	@param	category	カテゴリー
	 *	@param	file	ソースファイル
	 *	@param	line	ライン
	 *	@param	p_what	説明文字列
	 *	@param	ulOsErrorCode	OS依存のエラーコード
	 *	@param	ulHint	エラーヒント
	 */
	XExcept(XExceptCATEGORY category,const wchar_t* p_what,  const char* file, int line, unsigned long ulOsErrorCode = 0, unsigned long ulHint = 0);
	
	/** デストラクタ */
	virtual ~XExcept() throw ();

	/// 例外の概要を返す。  std::exceptionのオーバーライト
	virtual const char* what() const throw ();
	
	/// 例外の概要を返す。
	const wchar_t* whatW() throw ();
	
	/// 例外の概要を返す。
	const TCHAR* whatT() throw ();


	void addStackTrace(XExceptCATEGORY category, const char* file, int line, unsigned long ulOsErrorCode = 0, unsigned long ulHint = 0, const TCHAR* pErrMsg = 0);

#ifdef _UNICODE
	virtual void print(XLIB_STD_OUT_STR_STREAM& out = std::wcerr);
	virtual void printStackTrace(XLIB_STD_OUT_STR_STREAM& out = std::wcerr);
#else
	virtual void print(XLIB_STD_OUT_STR_STREAM& out = std::cerr);
	virtual void printStackTrace(XLIB_STD_OUT_STR_STREAM& out = std::cerr);
#endif
	/** @brief	OSエラーコードを取得
	 *	@return OSエラーコード
	 */
	unsigned long getOsErrorCode() const;
	/** @brief	ヒント値を取得
	 *	@return ヒント値
	 */
	unsigned long getHint() const;

	/**	@brief	ヒント値を設定
	 *	@param	ulHint	ヒント値
	 */
	void setHist(const unsigned long ulHint);

	/** @brief	エラーカテゴリーを取得
	 *	@return エラーカテゴリー
	 */
	XExceptCATEGORY getCategory() const;

	/* @brief エラーコード */
	unsigned long		m_nOsErrorCode;
private:
	XExcept& operator=(const XExcept& rhs);
private:
	/*	@brief	エラーカテゴリー	*/
	XExceptCATEGORY		m_catgegory;
	/*	@brief	エラーヒント	*/
	unsigned long	m_ulHint;
	/*	@brief	スタックアイテム	*/
	XExceptStackItems			m_stacItms;
	/*	@brief	エラー説明文字列用のバッファ	*/
	mutable XStrBuff	m_what;
	const char* p_strWat;
};



#define ___XEXCEPT_CHECK_VALID_TRUE(x) if( x == false ) {XExceptInvalidParam(RNAME, __FILE__, __LINE__); }
void XExceptInvalidParam(const char* p_func, const char* p_f_name, int line);

#include "xlib/xlib_tail.h"
#endif // __X_EXCEPTION_HEASER_FILE__



