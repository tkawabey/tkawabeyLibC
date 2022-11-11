/**
 * @file xstrbuff.h
 * @brief 文字列管理クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_STRBUFF_HEASER_FILE__
#define __X_STRBUFF_HEASER_FILE__
#pragma warning( disable: 4250)
#include "xlib/xlib_head.h"
#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif
#endif //
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>
#include "xlib/xbuff.h"
#include "xlib/xarray.h"



#ifdef _UNICODE
  #ifndef UNICODE
    #define UNICODE 1
  #endif
#endif

#ifdef WIN32
	#include <tchar.h>
#endif // WIN32


#ifndef _TCHAR_DEFINED
 #ifdef _UNICODE
  #define TCHAR	wchar_t
 #else
  #define TCHAR	char
 #endif	// _UNICODE
#endif // TCHAR


#ifndef _T
 #ifdef _UNICODE
	#define _T(x)       L ## x
 #else
	#define _T(x)       x
 #endif	// _UNICODE
#endif /* _T */ 

#define XCOUNTOF(A) (sizeof(A)/sizeof(A[1]))

#define XSTRBUFF_DEF_INFLATE_SIZE	((size_t)256)

/** 
 * 文字列管理クラス
 */
class XLIB_API XStrBuff : public XBuff
{
public:
	/** 文字列のトリミングモード */
	enum TrimMode {
		TRIM_RIGHTONLY = 1,	/** 右端のみ */
		TRIM_LEFTONLY  = 2,	/** 左端のみ */
		TRIM_BOTH      = 3	/** 両方 */
	};

	/**	@brief	コンストラクタ
	 *	@param	iInflateSize	余分バッファの増加サイズ
	 */
	XStrBuff(size_t iInflateSize = XSTRBUFF_DEF_INFLATE_SIZE);
	/**
	 * コンストラクタ
	 *	@param lpsz 初期化文字列
	 *	@param	iInflateSize	余分バッファの増加サイズ
	 */
	XStrBuff(const char* lpsz, size_t size = -1, size_t iInflateSize = XSTRBUFF_DEF_INFLATE_SIZE);
	/**
	 * コンストラクタ
	 *	@param lpsz 初期化文字列
	 *	@param	iInflateSize	余分バッファの増加サイズ
	 */
	XStrBuff(const wchar_t* lpsz, size_t size = -1, size_t iInflateSize = XSTRBUFF_DEF_INFLATE_SIZE);
	/**
	 * コンストラクタ
	 *	@param src コピー元ソース
	 *	@param	iInflateSize	余分バッファの増加サイズ
	 */
	XStrBuff(const XStrBuff& src, size_t iInflateSize = XSTRBUFF_DEF_INFLATE_SIZE);
	/** @brief	デストラクタ */
	virtual ~XStrBuff();

	

	/**	@brief	余分バッファの増加サイズを設定
	 *	@param	iInflateSize	余分バッファの増加サイズ
	 *　@exception インデックスオーバーの場合、XExcept(XExcept_UNKNOWN_PARAM)が発生
	 */
	bool setInflateSize(size_t nInflateSize);
	/**	@brief	余分バッファの増加サイズを取得
	 *	@return	余分バッファの増加サイズ
	 */
	size_t getInflateSize() const{ return m_iInflateSize; }

	/** @copydoc XBuff::Alloc(size_t) */
	virtual void *Alloc(size_t nSize);
	/** @copydoc XBuff::ReAlloc(size_t) */
	virtual void *ReAlloc(size_t nSize);
	/** @copydoc XBuff::Free() */
	virtual void Free();

	/**	@brief	文字列を返します。
	 *
	 *	@return	文字列
	 */
	virtual const TCHAR* c_str() const;

	virtual XStrBuff& set(const char* lpsz, int size = -1);
	virtual XStrBuff& set(const wchar_t* lpsz, int size = -1);

	/**
	 * @brief	文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XStrBuff& operator=(const char* lpsz);
	/**
	 * @brief	文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */
	const XStrBuff& operator=(const wchar_t* lpsz);
	/**
	 * @brief	文字列を挿入します。
	 * @param stringSrc コピー元インスタンス
	 * @return 自身のインスタンス
	 */
	const XStrBuff& operator=(const XStrBuff& stringSrc);
	/**
	 * @brief	文字列を取得します。
	 * @return 文字列
	 */
	operator const char*();
	/**
	 * @brief	文字列を取得します。
	 * @return 文字列
	 */
	operator char*();
	/**
	 * @brief	文字列を取得します。
	 * @return 文字列
	 */
	operator const wchar_t*();
	/**
	 * @brief	文字列を取得します。
	 * @return 文字列
	 */
	operator wchar_t*();
	/**
	 * @brief	指定した文字列と比較します。
	 * @param lpsz 比較する文字列
	 * @retval true 等しい
	 * @retval false 等しくない
	 */
	const bool operator==(const TCHAR* lpsz) const;

	/**
	 * @brief	指定した文字列と比較します。
	 * @param lpsz 比較する文字列
	 * @retval true 等しい
	 * @retval false 等しくない
	 */
	const bool operator==(const XStrBuff& right) const;

	/**
	 * @brief	指定した文字列と比較します。
	 * @param lpsz 比較する文字列
	 * @retval true 等しくない
	 * @retval false 等しい
	 */
	const bool operator!=(const TCHAR* lpsz) const;

	bool operator < (const XStrBuff& rhight) const;

	/**
	 * @brief	指定したインデックスのキャラクターを取得します。
	 * @param nIndex キャラクターを取得するインデックス
	 * @return キャラクター
	 */
	TCHAR& operator[](int nIndex);
	/**
	 * @brief	現在のインスタンスの文字列に指定した文字列を連結します。
	 * @param lpsz 連結する文字列
	 * @return 自身
	 */
	const XStrBuff& operator+=(const char* lpsz);
	/**
	 * @brief	現在のインスタンスの文字列に指定した文字列を連結します。
	 * @param lpsz 連結する文字列
	 * @return 自身
	 */
	const XStrBuff& operator+=(const wchar_t* lpsz);
	/**
	 * @brief	現在のインスタンスの文字列に指定した文字列を連結します。
	 * @param lpsz 連結する文字列
	 * @return 自身
	 */
	const XStrBuff& operator+=(XStrBuff& val);

	XStrBuff& operator<<(short val);
	XStrBuff& operator<<(unsigned short val);
	XStrBuff& operator<<(int val);
	XStrBuff& operator<<(unsigned int val);
	XStrBuff& operator<<(long val);
	XStrBuff& operator<<(unsigned long val);
	XStrBuff& operator<<(XD_INT8 val);
	XStrBuff& operator<<(XD_UINT8 val);
	XStrBuff& operator<<(float val);
	XStrBuff& operator<<(double val);
	XStrBuff& operator<<(const char* val);
	XStrBuff& operator<<(const wchar_t* val);
	XStrBuff& operator<<(XStrBuff& val);

	XStrBuff& appendChar(char c);
	XStrBuff& appendChar(wchar_t c);

	/**
	 * 文字列の長さを取得します。
	 * @return 文字列の長
	 */
	size_t GetLength() const;
	/**
	 * 文字列の長さを取得します。
	 * @return 文字列の長
	 */
	size_t getLength() const { return this->GetLength(); }
	
	/**
	 * 文字列の長さ分のバッファサイズを取得します。
	 * @return 文字列の長さ分のバッファサイズ
	 */
	size_t getLengthOfByteSize() const { return this->GetLength()*sizeof(TCHAR); }

	
	/**
	 * バッファカウント（文字数）を取得
	 * @return バッファカウント（文字数）を取得
	 */
	size_t getCountOfBuff()  const{  return this->GetBuffSize() / sizeof(TCHAR); }
	
	/**
	 * 余りバッファカウント（文字数）のを取得
	 * @return 余りバッファカウント（文字数）を取得
	 */
	size_t getCountOfRestBuff()  const { return this->getCountOfBuff() - this->getLength(); }

	/**
	 * 指定したインデックスのキャラクターを取得します。
	 * @param nIndex キャラクターを取得するインデックス
	 * @return キャラクター
	 * @exception インデックスオーバーの場合、XExcept(XExcept_OVER_INDEX)が発生
	 */
	TCHAR& GetAt(size_t nIndex);

	/**
	 * 指定したインデックスの位置のキャラクターをchと置き換えます。
	 * @param nIndex 置き換えるインデックス
	 * @param ch 置き換えるキャラクター
	 * @exception インデックスオーバーの場合、XExcept(XExcept_OVER_INDEX)が発生
	 */
	void SetAt(size_t nIndex, TCHAR ch);
	/**
	 * 書式付データで、文字列を作成します。
	 * @param lpszFormat 書式制御文字列
	 * @param argList  引数リストへのポインタ
	 */
	void FormatV(const TCHAR* lpszFormat, va_list argList);
	/**
	 * 書式付データで、文字列を作成します。
	 * @param lpszFormat 書式制御文字列
	 * @param ...  オプションの引数
	 */
	void Format(const TCHAR* lpszFormat, ...);

	/**
	 * 書式付データで、文字列を作成します。
	 * @param lpszFormat 書式制御文字列
	 * @param argList  引数リストへのポインタ
	 * @return 書式化された文字列のXStrBuff
	 */
	static XStrBuff FormatS(const TCHAR* lpszFormat, va_list argList);
	/**
	 * 書式付データで、文字列を作成します。
	 * @param lpszFormat 書式制御文字列
	 * @param ...  オプションの引数
	 * @return 書式化された文字列のXStrBuff
	 */
	static XStrBuff FormatS(const TCHAR* lpszFormat, ...);
	/**
	 * 指定した文字列と比較します。
	 * @param lpsz 比較する文字列
	 * @retval 0 同じ文字列です。
	 * @retval 正の値 lpszの方が小さい
	 * @retval 負の値 lpszの方が大きい
	 */
	int Compare(const TCHAR* lpsz) const;
	/**
	 * 指定した文字列を文字の大文字小文字を口説せず比較します。
	 * @param lpsz 比較する文字列
	 * @retval 0 同じ文字列です。
	 * @retval 正の値 lpszの方が小さい
	 * @retval 負の値 lpszの方が大きい
	 */
	int CompareNoCase(const TCHAR* lpsz) const;
	/** 文字列を大文字に変換します。 */
	void MakeUpper();
	/** 文字列を小文字に変換します。 */
	void MakeLower();
	/**
	 * 文字列の前後をトリミングします。
	 * @param mode トリムするモード
	 * @return トリム後の文字列
	 */
	const TCHAR* Trim(TrimMode mode = TRIM_BOTH);
	/**
	 * 指定された文字を他の文字に置き換えます。
	 * @param lpszFind lpszDestに置き換える文字。
	 * @param lpszDest lpszFindと置き換える文字。
	 * @return 置き換え後の文字列
	 */
	const TCHAR* Replace(const TCHAR* lpszFind, const TCHAR* lpszDest);
	/**
	 * 文字列の左先頭から、nCount 文字を抽出し、抽出した部分文字列のコピーを返します
	 * @param nCount 抽出する文字数
	 * @return コピーインスタンス
	 */
	XStrBuff Left(size_t nCount) const;
	/**
	 * 文字列の右端から、nCount 文字を抽出し、抽出した部分文字列のコピーを返します
	 * @param nCount 抽出する文字数
	 * @return コピーインスタンス
	 */
	XStrBuff Right(size_t nCount) const;
	/**
	 * 指定された文字を文字列から削除します。
	 * @param chRemove 文字列から削除する文字。
	 * @return 文字列から削除した文字数を返します。
	 */
	int Remove(TCHAR chRemove);
	/**
	 * 大きな文字列内の文字または部分文字列を検索します。
	 * @param pstr 検索する部分文字列を指定します。
	 * @param nStart 文字列内の検索開始文字のインデックス
	 * @return 指定した部分文字列または文字と一致した位置。
	 */
	int Find( const TCHAR* pstr, int nStart ) const;
	/**
	 * 大きな文字列内の文字または部分文字列を検索します。
	 * @param lpszSub 検索する部分文字列を指定します。
	 * @return 指定した部分文字列または文字と一致した位置。
	 */
	int Find( const TCHAR* lpszSub ) const;


	/**
	 * 文字列の長さを取得します。
	 * @param lpsz 文字列
	 * @return 文字列の長
	 */
	static size_t StrLen(const TCHAR* lpsz);
	/**
	 * 文字列の長さ分のバッファサイズを取得します。
	 * @param lpsz 文字列
	 * @return 文字列の長分のバッファサイズ
	 */
	static size_t StrLenOfByteSize(const TCHAR* lpsz){ return StrLen(lpsz)*sizeof(TCHAR); }
	/**
	 * 文字列をコピーします。
	 * @param lpszDst コピー先の文字列
	 * @param lpszSrc コピー元の NULL 終端文字列
	 * @return コピー先の文字列を返します。
	 */
	static TCHAR* StrCpy(TCHAR* lpszDst, size_t buff_count, const TCHAR* lpszSrc);
	/**
	 * 文字列から別の文字列に文字をコピーします。
	 * @param lpszDst コピー先の文字列
	 * @param lpszSrc コピー元の NULL 終端文字列
	 * @param count コピーする文字数
	 * @return コピー先の文字列を返します。
	 */
	static TCHAR* StrNCpy(TCHAR* lpszDst, size_t buff_count, const TCHAR* lpszSrc, size_t count);
	/**
	 * 文字列を比較します。
	 * @param lpsz1 比較する NULL 終端文字列
	 * @param lpsz2 比較する NULL 終端文字列
	 * @retval 0 同じ文字列です。
	 * @retval 正の値 psz2の方が小さい
	 * @retval 負の値 psz1の方が大きい
	 */
	static int    StrCmp(const TCHAR* lpsz1, const TCHAR* lpsz2);
	/**
	 * 文字列中の文字を小文字として比較します。
	 * @param lpsz1 比較する NULL 終端文字列
	 * @param lpsz2 比較する NULL 終端文字列
	 * @retval 0 同じ文字列です。
	 * @retval 正の値 psz2の方が小さい
	 * @retval 負の値 psz1の方が大きい
	 */
	static int    StrICmp(const TCHAR* lpsz1, const TCHAR* lpsz2);
	/**
	 * 2 つの文字列の文字を比較します。
	 * @param lpsz1 比較する NULL 終端文字列
	 * @param lpsz2 比較する NULL 終端文字列
	 * @param count 比較する文字数
	 * @retval 0 同じ文字列です。
	 * @retval 正の値 psz2の方が小さい
	 * @retval 負の値 psz1の方が大きい
	 */
	static int    StrNCmp(const TCHAR* lpsz1, const TCHAR* lpsz2, size_t count);
	/**
	 * マルチバイト文字の第1バイトへのポインタを返します。
	 * @param lpsz 字列のポインタ
	 * @return lpsz直後のマルチバイト文字の第1バイトへのポインタ
	 */
	static TCHAR* StrInc( const TCHAR *lpsz);
	/**
	 * 文字列を走査し、指定された文字の最後の出現箇所を見つけます。
	 * @param string 走査対象の NULL 終端文字列
	 * @param c 見つける文字
	 * @return 文字 c が string 内に最後に現れた位置へのポインタを返します。指定された文字が見つからない場合は NULL ポインタを返します。
	 */	
	static TCHAR* StrRChr( const TCHAR *string, int c);

	/**
	 * 文字列を数値に変換します。
	 * @param string 文字列
	 * @return　変換後の数値
	 */
	static int    AtoI(const TCHAR* string);

	/**
	 * 文字列を数値に変換します。
	 * @param string 文字列
	 * @return　変換後の数値
	 */
	static XD_INT8 AtoI64(const TCHAR* string);

	/**	@brief	文字列を数値に変換する。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *	@param	base	数値の基数
	 *
	 *	@return	オーバーフローが発生すると LONG_MAX または LONG_MIN を返します。変換できなかった場合は 0 を返します。
	 */
	static XD_INT8 strtollA(const char *nptr,char **endptr,int base);
	/**	@brief	文字列を数値に変換する。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *	@param	base	数値の基数
	 *
	 *	@return	オーバーフローが発生すると LONG_MAX または LONG_MIN を返します。変換できなかった場合は 0 を返します。
	 */
	static XD_INT8 strtollW(const wchar_t *nptr,wchar_t **endptr,int base);


	/**	@brief	文字列を数値に変換する。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *	@param	base	数値の基数
	 *
	 *	@return	オーバーフローが発生すると LONG_MAX または LONG_MIN を返します。変換できなかった場合は 0 を返します。
	 */
	static XD_UINT8 strtoullA(const char *nptr,char **endptr,int base);
	/**	@brief	文字列を数値に変換する。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *	@param	base	数値の基数
	 *
	 *	@return	オーバーフローが発生すると LONG_MAX または LONG_MIN を返します。変換できなかった場合は 0 を返します。
	 */
	static XD_UINT8 strtoullW(const wchar_t *nptr,wchar_t **endptr,int base);
	/**	@brief	文字列を数値に変換する。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *	@param	base	数値の基数。－１を指定すると、基数の自動判定。OXFFFF(16進数指定可能)。デフォルトは１０進数
	 *
	 *	@return	オーバーフローが発生すると LONG_MAX または LONG_MIN を返します。変換できなかった場合は 0 を返します。
	 */
	static XD_INT8 strtoll(const TCHAR *nptr,TCHAR **endptr,int base);
	/**	@brief	文字列を数値に変換する。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *	@param	base	数値の基数。－１を指定すると、基数の自動判定。OXFFFF(16進数指定可能)。デフォルトは１０進数	 *
	 *
	 *	@return	オーバーフローが発生すると LONG_MAX または LONG_MIN を返します。変換できなかった場合は 0 を返します。
	 */
	static XD_INT4 strtol(const TCHAR *nptr,TCHAR **endptr,int base);
	/**	@brief	文字列を数値に変換する。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *	@param	base	数値の基数。－１を指定すると、基数の自動判定。OXFFFF(16進数指定可能)。デフォルトは１０進数	 *
	 *
	 *	@return	オーバーフローが発生すると LONG_MAX または LONG_MIN を返します。変換できなかった場合は 0 を返します。
	 */
	static XD_UINT8 strtoull(const TCHAR *nptr,TCHAR **endptr,int base);
	/**	@brief	文字列を数値に変換する。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *	@param	base	数値の基数。－１を指定すると、基数の自動判定。OXFFFF(16進数指定可能)。デフォルトは１０進数
	 *
	 *	@return	オーバーフローが発生すると LONG_MAX または LONG_MIN を返します。変換できなかった場合は 0 を返します。
	 */
	static XD_UINT4 strtoul(const TCHAR *nptr,TCHAR **endptr,int base);


	/**	@brief	文字列を実数に変換します。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *
	 *	@return	変換された値
	 */
	static XD_FLOAT8 strtodA( const char *nptr, char **endptr );

	/**	@brief	文字列を実数に変換します。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *
	 *	@return	変換された値
	 */
	static XD_FLOAT8 strtodW( const wchar_t *nptr, wchar_t **endptr  );

	/**	@brief	文字列を実数に変換します。
	 *
	 *	@param	nptr	変換する NULL 終端文字列
	 *	@param	*endptr	走査の終了位置を示す文字へのポインタ
	 *
	 *	@return	変換された値
	 */
	static XD_FLOAT8 strtod( const TCHAR *nptr, TCHAR **endptr  );

	/** 
	 * tが特定の10進数字表現であると、真を返します。
	 * @param t テストする整数値
	 * @return tが特定の10進数字表現であると、真を返します。
	 */
	static int    isDigit(const TCHAR t);
	/** 
	 * tが特定の16進数字表現であると、真を返します。
	 * @param t テストする整数値
	 * @return tが特定の16進数字表現であると、真を返します。
	 */
	static int    isXDigit(const TCHAR t);
	/**	@brief	指定文字列が数値字表現であると、真を返します.
	 *			16真数表記は、文字列の先頭に、0xを付加して文字列を指定します。
	 *	@param	pStr	判定する文字列
	 *
	 *	@return tが特定の10進数字表現であると、真を返します。
	 */
	static int    isDigitsEx(const TCHAR* pStr);
	/**	@brief	引数リストへのポインタを使って、書式付き出力を書き込みます。
	 *	@param buffer 出力先の記憶領域
	 *	@param format 書式制御
	 *	@param argptr 引数リストへのポインタ
	 *	@retval	1	数値である
	 *	@retval	0	数値でない
	 */
	static int    VsPrintf(TCHAR *buffer, size_t buff_count, const TCHAR *format, va_list argptr);
	static int    VsnPrintf(TCHAR *buffer, size_t buff_count, size_t maxlen, const TCHAR *format, va_list argptr);
	static int  printf(TCHAR *buffer, size_t buff_count, const TCHAR *format, ...);
	static int  nprintf(TCHAR *buffer, size_t buff_count, size_t maxlen, const TCHAR *format, ...);
	/**
	 * 文字列中の文字を大文字に変換します。
	 * @param string 文字列
	 * @return 変換された文字列へのポインタ
	 */
	static TCHAR* StrUpr(TCHAR *string);
	/**
	 * 文字列中の文字を小文字に変換します。
	 * @param string 文字列
	 * @return 変換された小字列へのポインタ
	 */
	static TCHAR* StrLwr(TCHAR *string);
	/**
	 * 部分文字列を見つけます。
	 * @param string1 (null)
	 * @param string2 (null)
	 * @return string2 が string1 内に最初に現れた位置へのポインタ。string2 がstring1 内に見つからない場合は NULL ポインタを返します。string2 が長さ 0 の文字列を指している場合は、string1 を返します。
	 */
	static const TCHAR* StrStr(const TCHAR *string1, const TCHAR *string2);
	/**
	 * 部分文字列を見つけます。
	 * @param string1 (null)
	 * @param string2 (null)
	 * @return string2 が string1 内に最初に現れた位置へのポインタ。string2 がstring1 内に見つからない場合は NULL ポインタを返します。string2 が長さ 0 の文字列を指している場合は、string1 を返します。
	 */
	static TCHAR* StrStr(TCHAR *string1, const TCHAR *string2);
	/**
	 * 文字列を検索して文字を見つけます。
	 * @param lpsz NULL 終端文字列
	 * @param c 検索する文字
	 * @return string 中で c が最初に現れた位置へのポインタを返します。文字が見つからないと NULL を返します。
	 */
	static TCHAR* StrChr(const TCHAR *lpsz, TCHAR c);
	/**
	 * 文字列を追加します。
	 * @param string1 追加先の NULL 終端文字列
	 * @param string2 追加する NULL 終端文字列
	 * @return 加先の文字列 (string1) を返します
	 * @note 2つ目のパラメーター(buff_count)は、バッファーの残りのサイズではなく、合計サイズであることに注意してください。
	 */
	static TCHAR* StrCat(TCHAR *string1, size_t buff_count, const TCHAR *string2 );
	/**
	 * 指定した文字列のアドレスを、文字列の左から、スペースまたは、タブが見付かるまでポインターを進めます。
	 * @param pWork 文字列のアドレス
	 * @retval true 見付かった
	 * @retval false 見付からない
	 */
	static bool MvPointerSPACE(TCHAR* *pWork);
	/**
	 * 指定した文字列のアドレスを、文字列の左から、スペースまたは、タブでない文字が見付かるまで、ポインターを進めます。
	 * @param pWork 文字列のアドレス
	 * @retval true 見付かった
	 * @retval false 見付からない
	 */
	static bool MvPointerUnSPACE(TCHAR* *pWork);
	/**
	 * 指定した文字列のアドレスを、文字列の左から、c 文字が見付かるまで、ポインターを進めます。
	 * @param pWork 文字列のアドレス
	 * @param c サーチする文字
	 * @retval true 見付かった
	 * @retval false 見付からない
	 */
	static bool MvPointerCHAR(TCHAR* *pWork, TCHAR c);

	/**	@brief	ワイルドカード指定可能な文字列比較
	 *
	 *	@param	ptn	パターン文字
	 *	@param	str	比較対照の文字
	 *
	 *	@retval	真：一致
	 *	@retval	偽：不一致
	 */
	static int strmatchA(const char *ptn, const char *str);
	/**	@brief	ワイルドカード指定可能な文字列比較
	 *
	 *	@param	ptn	パターン文字
	 *	@param	str	比較対照の文字
	 *
	 *	@retval	真：一致
	 *	@retval	偽：不一致
	 */
	static int strmatchW(const wchar_t *ptn, const wchar_t *str);
	/**	@brief	ワイルドカード指定可能な文字列比較
	 *
	 *	@param	ptn	パターン文字
	 *	@param	str	比較対照の文字
	 *
	 *	@retval	真：一致
	 *	@retval	偽：不一致
	 */
	static int strmatch(const TCHAR *ptn, const TCHAR *str);


	static errno_t __strcpyA(char* p_dst, size_t buff_count, const char* p_src);
	static errno_t __strcpyW(wchar_t* p_dst, size_t buff_count, const wchar_t* p_src);
	static errno_t __strncpyA(char* p_dst, size_t buff_count, const char* p_src, size_t max_count);
	static errno_t __strncpyW(wchar_t* p_dst, size_t buff_count, const wchar_t* p_src, size_t max_count);
	static errno_t __wcstombs(char* p_dst, size_t buff_count, const wchar_t* p_src, size_t max_count);
	static errno_t __mbstowcs(wchar_t* p_dst, size_t buff_count, const char* p_src, size_t src_count);

	static errno_t __sprintf(char* p_dst, size_t buff_count, const char* p_format, ...);
	static errno_t __swprintf(wchar_t* p_dst, size_t buff_count, const wchar_t* p_format, ...);
	static errno_t __vsprintf(char *buffer, size_t buff_count, const char *format, va_list argptr);
	static errno_t __vswprintf(wchar_t *buffer, size_t buff_count, const wchar_t *format, va_list argptr);
	static errno_t __vsnprintf(char *buffer, size_t buff_count, size_t max_count, const char *format, va_list argptr);
	static errno_t __vsnwprintf(wchar_t *buffer, size_t buff_count, size_t max_count, const wchar_t *format, va_list argptr);
	
	/** @copydoc XStrBuff::StrCmp(const TCHAR*, const TCHAR*) */
	static int strcmp(const char *lpsz1, const char *lpsz2);
	/** @copydoc XStrBuff::StrCmp(const TCHAR*, const TCHAR*) */
	static int wcscmp (const wchar_t * lpsz1, const wchar_t * lpsz2);

	/** @copydoc XStrBuff::StrICmp(const TCHAR*, const TCHAR*) */
	static int stricmp(const char *lpsz1, const char *lpsz2);
	/** @copydoc XStrBuff::StrICmp(const TCHAR*, const TCHAR*) */
	static int wcsicmp (const wchar_t * lpsz1, const wchar_t * lpsz2);
	/** @copydoc XStrBuff::StrUpr(TCHAR*) */
	static char* strupr (char * string);
	/** @copydoc XStrBuff::StrUpr(TCHAR*) */
	static wchar_t* wcsupr(wchar_t * string);
	/** @copydoc XStrBuff::StrLwr(TCHAR*) */
	static char* strlwr(char * string);
	/** @copydoc XStrBuff::StrLwr(TCHAR*) */
	static wchar_t* wcslwr (wchar_t * string);
	/** @copydoc XStrBuff::StrInc( const TCHAR*) */
	static char* strinc( const char *lpsz );
	/** @copydoc XStrBuff::StrInc( const TCHAR*) */
	static wchar_t* wcsinc( const wchar_t *lpsz );
	/** @copydoc XStrBuff::AtoI(const TCHAR*) */
	static int atoi(const char *string);
	/** @copydoc XStrBuff::AtoI(const TCHAR*) */
	static int wtoi( const wchar_t *string );
protected:
	/* 文字列のバッファ */
	TCHAR*	m_pStrAddress;
	/* 文字コード変更用バッファ */
	XBuff	m_ConvertBuff;
	/* バッファの増可サイズ */
	size_t	m_iInflateSize;
};


/**
 * 文字列配列クラス(JAVAのjava.lang.StringBufferのような動きをする)
 */
class XLIB_API XStrBuffList
{
public:
	/** コンストラクタ */
	XStrBuffList();
	/** デストラクタ */
	virtual ~XStrBuffList();

	/** リストを空にします */
	void clear();

	/**
	 * 文字列をバッファの最後に追加する。
	 * @param[in] lpszFormat 追加する文字列の書式フォーマット
	 * @param[in] ... 書式フォーマットのオプション
	 * @return 自身
	 */
	XStrBuffList& append(const char* lpszFormat, ...);
	/** @copydoc XStrBuffList::append(const char* , ...) */
	XStrBuffList& append(const wchar_t* lpszFormat, ...);
	/**
	 * 文字列をバッファの最後に追加する。
	 * @param[in] str 文字列
	 * @return 自身
	 */
	XStrBuffList& append(XStrBuff& str);

	/**
	 *  文字を文字列バッファに挿入する。
	 * @param[in] iPos 挿入するオフセット
	 * @param[in] lpszFormat 挿入する文字列の書式フォーマット
	 * @param[in] ... 書式フォーマットのオプション
	 * @return 自身
	 */
	XStrBuffList& insert(int iPos, const char* lpszFormat, ...);
	/** @copydoc XStrBuffList::insert(int, const char* , ...) */
	XStrBuffList& insert(int iPos, const wchar_t* lpszFormat, ...);
	/**
	 *  文字を文字列バッファに挿入する。
	 * @param[in] iPos 挿入するオフセット
	 * @param[in] str 挿入する文字列
	 * @return 自身
	 */
	XStrBuffList& insert(int iPos, XStrBuff& str);
	/**
	 * 配列数を返します。
	 * @return 配列数
	 */ 
	int GetCount() const;
	/**
	 * 指定したインデックスのXStrBuffインスタンスのアドレスを返します。
	 * @param[in] iPos 
	 * @return XStrBuffインスタンスのアドレス。インデックスが不正な場合は、NULL。
	 */ 
	XStrBuff* GetAt(int iPos);

	/**
	 * toStringで返される文字数を求めます。
	 * @param[in] pBefore 配列ごとの前に追加される文字
	 * @param[in] pAfter 配列ごとの後に追加される文字
	 * @return 文字数
	 */
	size_t capacityA(const char *pBefore = NULL, const char* pAfter = NULL);
	/** XStrBuffList::capacityA(const char *pBefore , const char*)  */
	size_t capacityW(const wchar_t *pBefore = NULL, const wchar_t* pAfter = NULL);
	/** XStrBuffList::capacityA(const char *pBefore , const char*)  */
	size_t capacity(const TCHAR *pBefore = NULL, const TCHAR* pAfter = NULL);

	/**
	 * 文字列を作成します。
	 * @param[in] pBefore 配列ごとの前に追加される文字
	 * @param[in] pAfter 配列ごとの後に追加される文字
	 * @return 文字
	 */
	XStrBuff toStringA(const char *pBefore = NULL, const char* pAfter = NULL);
	/** XStrBuffList::toStringA(const char *pBefore , const char*)  */
	XStrBuff toStringW(const wchar_t *pBefore = NULL, const wchar_t* pAfter = NULL);
	/** XStrBuffList::toStringA(const char *pBefore , const char*)  */
	XStrBuff toString(const TCHAR *pBefore = NULL, const TCHAR* pAfter = NULL);
protected:
	/** リスト */
	XList		m_list;
};


/**
 * 文字列を、Excelで読み込めるCSV形式に整形します
 */
class XStrBuffCSV : public XStrBuff
{
public:
// Construction
	/**
	 * コンストラクタ
	 * @param cDelimit デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffCSV(TCHAR cDelimit = ',', TCHAR cIndeChar = '\"');
	/**
	 * コンストラクタ
	 * @param p 代入する文字列
	 * @param cDelimit デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffCSV(const char* p, TCHAR cDelimit = ',', TCHAR cIndeChar = '\"');
	/**
	 * コンストラクタ
	 * @param p 代入する文字列
	 * @param cDelimit デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffCSV(const wchar_t* p, TCHAR cDelimit = ',', TCHAR cIndeChar = '\"');
	/**
	 * 文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XStrBuffCSV& operator=(const char* lpsz);
	/**
	 * 文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XStrBuffCSV& operator=(const wchar_t* lpsz);
protected:
	/**
	 * バッファ内の文字列をExcelで読み込めるCSV形式に整形します。
	 * @return 変換後の文字列
	 */
	const TCHAR* Conv();
	/** デリミタ */
	TCHAR m_cDelimit;
	/** インデント */
	TCHAR m_cIndeChar;
	/** 変換済みフラグ */
	bool m_bConv;
};

/**
 * 文字列を、SQL文字用に、' があると、もうひとつ' のついた文字に整形します
 */
class XStrBuffSQL : public XStrBuff
{
public:
	/**
	 * コンストラクタ
	 * @param cDelimit デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffSQL(TCHAR cDelimit = '\'', TCHAR cIndeChar = '\'');
	/**
	 * コンストラクタ
	 * @param p 代入する文字列
	 * @param cDelimit デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffSQL(const char* p, TCHAR cDelimit = '\'', TCHAR cIndeChar = '\'');
	/**
	 * コンストラクタ
	 * @param p 代入する文字列
	 * @param cDelimit デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffSQL(const wchar_t* p, TCHAR cDelimit = '\'', TCHAR cIndeChar = '\'');
	/**
	 * 文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XStrBuffSQL& operator=(const char* lpsz);
	/**
	 * 文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XStrBuffSQL& operator=(const wchar_t* lpsz);
protected:
	/**
	 * 文字列を、SQL文字用に、' があると、もうひとつ' のついた文字に整形します
	 * @return 変換後の文字列
	 */
	const TCHAR* Conv();
	/** デリミタ */
	TCHAR m_cDelimit;
	/** インデント */
	TCHAR m_cIndeChar;
	/** 変換済みフラグ */
	bool m_bConv;
};

/** 
 * 文字列をスプリットするクラス
 */
class XStrBuffSplit :  public XStrBuff
{
public:
	/**
	 * コンストラクタ
	 * @param cDelimiter デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffSplit(TCHAR cDelimiter = ',', TCHAR cIndeChar = '\"');
	/**
	 * コンストラクタ
	 * @param p 代入する文字列
	 * @param cDelimiter デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffSplit(const char* p, TCHAR cDelimiter = ',', TCHAR cIndeChar = '\"');
	/**
	 * コンストラクタ
	 * @param p 代入する文字列
	 * @param cDelimiter デリミタ
	 * @param cIndeChar インデント
	 */
	XStrBuffSplit(const wchar_t* p, TCHAR cDelimiter = ',', TCHAR cIndeChar = '\"');
	/** デストラクタ */
	virtual ~XStrBuffSplit();

	/**
	 * 文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XStrBuffSplit& operator=(const wchar_t* lpsz);
	/**
	 * 文字列を挿入します。
	 * @param lpsz 文字列
	 * @return 自身のインスタンス
	 */	
	const XStrBuffSplit& operator=(const char* lpsz);



	/**
	 * スプリットされた文字配列の数を取得します。
	 * @return スプリットされた文字配列の数
	 */
	int GetArgCouunt() const;
	/**
	 * スプリットされた文字配列を取得します。
	 * @return スプリットされた文字配列
	 */
	const TCHAR** GetArgv();
	/**
	 * スプリットされた文字配列の指定したインデックスの値を取得します。
	 * @param nIndex インデックス
	 * @return 文字列
	 */
	const TCHAR* GetDivid(int nIndex);
	/**
	 * インデント文字をセットします。
	 * @param cIndeChar インデント文字
	 */
	void SetIndentChar(TCHAR cIndeChar);
	/**
	 * デリミタ文字をセットします。
	 * @param cDelimite インデント文字
	 */
	void SetDelimite(TCHAR cDelimite);
	/**
	 * Splitした結果、NULL文字のデータがある場合配列に加えるか判断するフラグを更新します
	 * @param bIsNullCount true NULL文字のデータを配列に加える。<br>false 加えない
	 */
	void SetNullCount(bool bIsNullCount);
	/**
	 * Splitした結果、NULL文字のデータがある場合配列に加えるか判断するフラグを取得します
	 * @retval true NULL文字のデータを配列に加える。
	 * @retval false NULL文字のデータを配列に加えない。
	 */
	bool GetNullCount();
protected:
	/**
	 * インスタンスの初期化
	 * @param cDelimiter デリミタ
	 * @param cIndeChar インデント
	 */
	void CommounConstruct(TCHAR cDelimiter, TCHAR cIndeChar);

	/**
	 * lpszBuffOrgの終端がNULLが2つセットされいるデータのメモリ確保
	 * @param lpszBuffOrg 終端がNULLが2つセットされいるデータ
	 * @return 確保したメモリサイズ
	 */
	unsigned long AllocNullNullTerminateA(const char*  lpszBuffOrg);
	/**
	 * lpszBuffOrgの終端がNULLが2つセットされいるデータのメモリ確保
	 * @param lpszBuffOrg 終端がNULLが2つセットされいるデータ
	 * @return 確保したメモリサイズ
	 */
	unsigned long AllocNullNullTerminateW(const wchar_t* lpszBuffOrg);
	/**
	 * 文字データが、NULL区切り、データの終端がNULL2つのデータのスプリット処理を行います。
	 * @return スプリットされた文字配列の数
	 */
	int SpritNullNullTerminate();
	/**
	 * 内部文字列をスプリットします。
	 * @return スプリットされた文字配列の数
	 */
	int Sprit();
	/** デリミタ */
	TCHAR m_cDelimiter;
	/** インデント */
	TCHAR m_cIndeChar;
	/** 配列用のバッファ */
	XBuff m_ArgBuff;
	/** 配列数 */
	int   m_ArgCount;
	/** Splitした結果、NULL文字のデータがある場合配列に加えるか判断するフラグ */
	bool  m_bIsNullCount;

};


/**	@brief	自動メモリ拡張バッファストリーム	*/
class XLIB_API XInflateStrstream :public 
#ifdef _UNICODE
	std::wiostream 
#else
	std::iostream 
#endif
{
public:
	/**	@brief	コンストラクタ	*/
	XInflateStrstream();
	/**	@brief	コンストラクタ
	 *	@param	nInitSize	初期確保するバッファサイズ
	 *	@param	nIncrementSize	バッファが足りなくなったら、増加するサイズ
	 */
    XInflateStrstream(unsigned long nInitSize, unsigned long nIncrementSize = 1024);
	/**	@brief	デストラクタ	*/
	virtual ~XInflateStrstream();
	/**	@brief	ストリームバッファを取得
	 *	@return	ストリームバッファ
	 */
#ifdef _UNICODE
    XInflateStreamBuffW *rdbuf() const;
#else
    XInflateStreamBuff *rdbuf() const;
#endif
	/**	@brief	バッファに書き込まれた文字列を読み込む
	 *	@return	バッファに書き込まれた文字列
	 */
	TCHAR *str();
	/**	@brief	書き込んだサイズを取得
	 *	@return	書き込んだサイズ
	 */
	std::streamsize pcount();
	/**	@brief	入力ポインタを先頭に戻します。	*/
	void resetp();
protected:
#ifdef _UNICODE
	XInflateStreamBuffW	m_buff;
#else
	XInflateStreamBuff	m_buff;
#endif
};

#include "xlib/xlib_tail.h"
#endif //__X_STRBUFF_HEASER_FILE__
