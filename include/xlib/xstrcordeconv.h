/**
 * @file xstrcordeconv.h
 * @brief 文字コード変換クラス群
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_STRCORDCONV_HEASER_FILE__
#define __X_STRCORDCONV_HEASER_FILE__
#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"


/**
 * 文字コード変換クラス
 */
class XLIB_API XStrBuffCodeConv : public XStrBuff
{
public:
	/**
	 * コードの種類
	 */
	enum  CodeType { 
		UNKNOWN,		/** Unkown */
		SJIS,			/** SJIS */
		EUC,			/** EUC */
		JIS,			/** JIS */
		SMB_HEX,		/** Samba-HEX */
		SMB_CAP			/** Samba-CAP */
	};
public:
	/** @copydoc XStrBuff::XStrBuff() */
	XStrBuffCodeConv();
	/** @copydoc XStrBuff::XStrBuff(const char* ) */
	XStrBuffCodeConv(const char* lpsz);
	/** @copydoc XStrBuff::XStrBuff(const wchar_t* ) */
	XStrBuffCodeConv(const wchar_t* lpsz);
	/** @copydoc XStrBuff::operator=(const wchar_t* ) */
	const XStrBuffCodeConv& operator=(const wchar_t* lpsz);
	/** @copydoc XStrBuff::operator=(const char* ) */
	const XStrBuffCodeConv& operator=(const char* lpsz);

	/**
	 * 文字コードを変換します。
	 * @param incode 変換前の文字コード
	 * @param outcode 変換後の文字コード
	 * @param dwSrcSize ソースのサイズ。 -1 を指定すると、自身のインスタンスを変化文字にセットします。それ以外は、pOutBuf,に変換文字をセットします。
	 * @param pOutBuf 変換後のバッファ。NULL を指定すると、自身のインスタンスを変化文字にセットします。それ以外は、pOutBuf,に変換文字をセットします。dwSrcSize,pdwOutSize も同時に指定してください。
	 * @param pdwOutSize 変換後のバッファのサイズ。NULL を指定すると、自身のインスタンスを変化文字にセットします。それ以外は、pOutBuf,に変換文字をセットします。dwSrcSize、pOutBufも同時に指定してください。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Convert(CodeType incode , CodeType outcode, unsigned long dwSrcSize = -1, XBuff *pOutBuf = NULL, unsigned long *pdwOutSize = NULL);

	/**	@brief	UNICODE文字をUTF8文字に変換
	 *
	 *	@param	pszSrc	UNICODE文字
	 *	@param	srcLen	UNICODE文字の長さ
	 *	@param	dst	UTF8文字に変換した値を受け取るバッファ
	 *	@param	bCESU8	a parameter of type bool
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool convertUnicodeToUTF8(const wchar_t* pszSrc, size_t srcLen, XStrBuff& dst, bool bCESU8 );

	/**	@brief	UTF8文字をUNICODE文字に変換
	 *
	 *	@param	pszSrc	UTF8文字
	 *	@param	srcLen	UTF8文字の長さ
	 *	@param	dst	UNICODE文字に変換した値を受け取るバッファ
	 *	@param	pfCESU	a parameter of type bool *
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool convertUTF8ToUnicode(const char* size_t, int srcLen, XBuff& dst, bool *pfCESU );


	/**	@brief	Native文字をUTF8文字に変換
	 *
	 *	@param	pszSrc	Native文字
	 *	@param	srcLen	Native文字の長さ
	 *	@param	dst	UTF8文字を受け取るバッファ
	 *	@param	pfCESU	a parameter of type bool *
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool convertNativeToUTF8(const char* pszSrc, XStrBuff& dst, bool bCESU8 );

	/**	@brief	Native文字をUTF8文字に変換
	 *
	 *	@param	pszSrc	Native文字
	 *	@param	srcLen	Native文字の長さ
	 *	@param	dst	UTF8文字を受け取るバッファ
	 *	@param	pfCESU	a parameter of type bool *
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool convertNativeToUTF8(const wchar_t* pszSrc, XStrBuff& dst, bool bCESU8 );


	/**	@brief	UTF8文字をNativeに変換
	 *
	 *	@param	pszSrc	UTF8文字
	 *	@param	srcLen	UTF8文字の長さ
	 *	@param	dstVaitive文字に変換した値を受け取るバッファ
	 *	@param	pfCESU	a parameter of type bool *
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool convertUTF8ToNative(const char* pszSrc, XStrBuff& dst, bool *pfCESU );

protected:
	/** 初期化 */
	void CommonConstruct();
	/**
	 * EUC->SJIS 変換
	 * @param pOutBuf 出力先バッファ
	 * @param dwSrcSize バッファサイズ
	 * @return 変換されたバイト数
	 */
	unsigned long ConvEUCtoSJIS(XBuff *pOutBuf, unsigned long dwSrcSize);
	/**
	 * JIS->SJIS 変換
	 * @param pOutBuf 出力先バッファ
	 * @param dwSrcSize バッファサイズ
	 * @return 変換されたバイト数
	 */
	unsigned long ConvJIStoSJIS(XBuff *pOutBuf, unsigned long dwSrcSize);
	/**
	 * SJIS->EUC 変換
	 * @param pOutBuf 出力先バッファ
	 * @param dwSrcSize バッファサイズ
	 * @return 変換されたバイト数
	 */
	unsigned long ConvSJIStoEUC(XBuff *pOutBuf, unsigned long dwSrcSize);
	/**
	 * SJIS->JIS 変換
	 * @param pOutBuf 出力先バッファ
	 * @param dwSrcSize バッファサイズ
	 * @return 変換されたバイト数
	 */
	unsigned long ConvSJIStoJIS(XBuff *pOutBuf, unsigned long dwSrcSize);

	/**
	 * EUC->SJIS カナ文字変換
	 * @param Dt かな文字の値
	 * @param pOut 出力先バッファ
	 * @return 変換後の文字列
	 */
	char *ConvEUCtoSJISkanaProc(char Dt, char *pOut);
	/**
	 * JIS->SJIS カナ文字変換
	 * @param Dt かな文字の値
	 * @param pOut 出力先バッファ
	 * @return 変換後の文字列
	 */
	char *ConvJIStoSJISkanaProc(char Dt, char *pOut);
	/**
	 * SJIS->EUC カナ文字変換
	 * @param Dt かな文字の値
	 * @param pOut 出力先バッファ
	 * @return 変換後の文字列
	 */
	char *ConvSJIStoEUCkanaProc(char Dt, char *pOut);
	/**
	 * SJIS->JIS カナ文字変換
	 * @param Dt かな文字の値
	 * @param pOut 出力先バッファ
	 * @return 変換後の文字列
	 */
	char *ConvSJIStoJISkanaProc(char Dt, char *pOut);

// static
	/**
	 * 濁音／半濁音になる文字かチェック
	 * @param Ch １バイトカタカナコード
	 * @param nDaku 濁点／半濁点
	 * @retval 0 濁点／半濁点
	 * @retval 0以外 文字コードに加える値
	 */
	static int AskDakuon(char Ch, char nDaku);
	/**
	 * １バイトカタカナをJIS漢字コードに変換
	 * @param Ch １バイトカタカナコード
	 * @return JIS漢字コード
	 */
	static int HanKataToZen(char Ch);
protected:
	/** 漢字モード */
	enum KanaKanji {
		CONV_ASCII =		0,		/** ASCII文字処理中 */
		CONV_KANJI =		1,		/** 漢字処理中 */
		CONV_KANA	 =		2		/** 半角カタカナ処理中 */
	};
	/** 漢字モード */
	int         m_nKanjiMode;
	/** 漢字モード */
	char        m_cKanjiFst;
	/** 漢字モード */
	char        m_cKanjiPrev;
	/** エスケープ数r */
	int         m_nEscNum;
	/** エスケープコード */
	char        m_cEscCode[2];
	/** かな変換モード */
	bool        m_nKanaConv;
	/** 勧業変換モード */
	bool        m_bConvCrLf;
	/** 終端コード */
	char        m_cTermCode;
};

/**
 * Base64のコーダークラス
 */
class XLIB_API XStrBuffBase64Coder :  public XStrBuff
{
public:
	/** @copydoc XStrBuff::XStrBuff() */
	XStrBuffBase64Coder();
	/** @copydoc XStrBuff::XStrBuff(const char* ) */
	XStrBuffBase64Coder(const char* lpsz);
	/** @copydoc XStrBuff::XStrBuff(const wchar_t* ) */
	XStrBuffBase64Coder(const wchar_t* lpsz);
	/** デストラクタ */
	virtual ~XStrBuffBase64Coder();
	/** @copydoc XStrBuff::operator=(const char* ) */
	const XStrBuffBase64Coder& operator=(const char* lpsz);
	/** @copydoc XStrBuff::operator=(const wchar_t* ) */
	const XStrBuffBase64Coder& operator=(const wchar_t* lpsz);

	
	/**
	 * エンコード
	 * @return エンコードされた文字列
	 */
	const TCHAR* Encode();
	/**
	 * デコード
	 * @return デコードされた文字列
	 */
	const TCHAR* Decode();

	/**	@brief	バッファをエンコードします。
	 *
	 *	@param	src	変換するバッファ
	 *	@param	dst	変換後のデータを受け取るバッファ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool encode(XBuff& src, XStrBuff& dst);
	/**	@brief	バッファをデコードします。
	 *
	 *	@param	src	変換するバッファ
	 *	@param	dst	変換後のデータを受け取るバッファ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool decode(const char* psrc, XBuff& dst);
	/**	@brief	バッファをデコードします。
	 *
	 *	@param	src	変換するバッファ
	 *	@param	dst	変換後のデータを受け取るバッファ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool decode(const wchar_t* psrc, XBuff& dst);
protected:
	/**
	 * エンコード
	 * @param lpszSrc 変換元データ
	 * @param lpszDest 変換先データバッファ
	 * @param nSrcLen 変換元データサイズ
	 * @return エンコード後のサイズ
	 */
	static int encode(const char* lpszSrc, char* lpszDest, int nSrcLen);
	/**
	 * デコード
	 * @param lpszSrc 変換元データ
	 * @param lpszDest 変換先データバッファ
	 * @param nSrcLen 変換元データサイズ
	 * @return デコード後のサイズ
	 */
	static int decode(const char* lpszSrc, char* lpszDest, int nSrcLen);
};




#include "xlib/xlib_tail.h"
#endif // __X_STRCORDCONV_HEASER_FILE__
