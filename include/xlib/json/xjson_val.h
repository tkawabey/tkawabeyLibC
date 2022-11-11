/**
 * @file xjson_val.h
 * @brief JSON-Valueクラス
 *
 * @author T.Fujiha
 * @date 2012-02-15
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_JSON_VALUE_HEADER_FILE__
#define __X_JSON_VALUE_HEADER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xeception.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"
#include "xlib/xmap.h"

class XLIB_API XJsonObj;
class XLIB_API XJsonArray;


/**
 * @brief JSON解析時にエラー報告するインターフェース
 */
class XLIB_API XJsonValErrorReport
{
public:
	XJsonValErrorReport();
	void setStartJsonStrings(const TCHAR* pStart) { if( m_pStart != NULL) return ; m_pStart = pStart; }
	void getPositionInfo(const TCHAR* pCheckStr, size_t* pCharPos = NULL, size_t *pLineNumber = NULL, size_t *pLineCharPos = NULL);


	virtual void error(const TCHAR* p_msg, size_t nCharPos, size_t nLineNumber, size_t nLinePos) = 0;


private:
	const TCHAR* m_pStart;
};

/**
 * @brief 通常フォーマット用データクラス
 */
class XLIB_API XJsonVal
{
public:
	/**
	 * @brief	データタイプ
	 */
	enum TYPE {
		STRING,		///<	文字列
		INTEGER,	///<	整数
		DOUBLE,		///<	実数
		OBJECT,		///<	オブジェクト
		ARRAY,		///<	配列
		BOOLEAN,	///<	真偽値
		NULL_VALUE,	///<	NULL
		UNKNOWN		///<	不明
	};


	/* 空データ*/
	static const XStrBuff	EMPTY_STRING;
	static const int		EMPTY_INT;
	static const double		EMPTY_DOUBLE;
	static const XJsonObj	EMPTY_OBJECT;
	static const XJsonArray	EMPTY_ARRAY;
	static const bool		EMPTY_BOOL;

	/**
	 * @brief コンストラクタ。NULL値で初期化
	 */
	XJsonVal();

	/**
	 * @brief コンストラクタ。文字列で初期化
	 * @param newCString	C-style文字列を使用して、文字列で初期化
	 */
	XJsonVal(const char *newCString);
	
	/**
	 * @brief コンストラクタ。文字列で初期化
	 * @param newCString	C-style文字列を使用して、文字列で初期化
	 */
	XJsonVal(const wchar_t *newCString);


	/**
	 * @brief コンストラクタ。整数で初期化
	 * @param newInt	初期化する整数の値
	 */
	XJsonVal(int newInt);

	/**
	 * @brief コンストラクタ。実数で初期化
	 * @param newDouble	初期化する実数の値
	 */
	XJsonVal(double newDouble);
	
	/**
	 * @brief コンストラクタ。オブジェクトで初期化
	 * @param newObject	初期化するオブジェクトの値
	 */
	XJsonVal(const XJsonObj &newObject);

	/**
	 * @brief コンストラクタ。配列で初期化
	 * @param XJsonArray	初期化する配列の値
	 */
	XJsonVal(const XJsonArray &XJsonArray);

	/**
	 * @brief コンストラクタ。真偽値で初期化
	 * @param newBoolean	初期化する真偽値の値
	 */
	XJsonVal(bool newBoolean);

	/**
	 * @brief コンストラクタ。引数で指定したコピー元のXJsonValと同じ内容で初期化
	 * @param src	コピー元のXJsonVal
	 */
	XJsonVal(const XJsonVal &src);

	/**
	 * @brief	デストラクタ
	 */
	virtual ~XJsonVal();



	/**
	 * @brief 代入オペレータ。
	 * @param src	コピー元のXJsonVal
	 * @return 自身
	 */
	XJsonVal &operator=(const XJsonVal &src);

	/**
	 * @brief 文字列代入オペレータ。
	 * @param src	コピー元の文字列
	 * @return 自身
	 */
	XJsonVal &operator=(const char *src);

	/**
	 * @brief 文字列代入オペレータ。
	 * @param src	コピー元の文字列
	 * @return 自身
	 */
	XJsonVal &operator=(const wchar_t *src);

	/**
	 * @brief 整数代入オペレータ。
	 * @param src	コピー元の整数
	 * @return 自身
	 */
	XJsonVal &operator=(int src);

	/**
	 * @brief 実数代入オペレータ。
	 * @param src	コピー元の実数
	 * @return 自身
	 */
	XJsonVal &operator=(double src);

	/**
	 * @brief オブジェクト代入オペレータ。
	 * @param src	コピー元のオブジェクト
	 * @return 自身
	 */
	XJsonVal &operator=(const XJsonObj &src);

	/**
	 * @brief 配列代入オペレータ。
	 * @param src	コピー元の配列
	 * @return 自身
	 */
	XJsonVal &operator=(const XJsonArray &src);

	/**
	 * @brief 真偽値代入オペレータ。
	 * @param src	コピー元の真偽値
	 * @return 自身
	 */
	XJsonVal &operator=(bool src);

	/**
	 * @brief 比較（一致）オペレータ。
	 * @param src	比較するXJsonVal
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool operator==(const XJsonVal &rhs) const;


	/**
	 * @brief 比較（不一致）オペレータ。
	 * @param src	比較するXJsonVal
	 * @retval	true	不一致
	 * @retval	false	一致
	 */
	bool operator!=(const XJsonVal &rhs) const;

	/**
	 * @brief 比較（小也）オペレータ。
	 * @param src	比較するXJsonVal
	 * @retval	true	比較オブジェクトより小さい。
	 * @retval	false	比較オブジェクトより大きい。
	 */
	bool operator <(const XJsonVal &rhs) const;

	/**
	 * @brief 比較（小也。含む）オペレータ。
	 * @param src	比較するXJsonVal
	 * @retval	true	比較オブジェクトより小さい。または、同じ。
	 * @retval	false	比較オブジェクトより大きい。
	 */
	bool operator <=(const XJsonVal &rhs) const;

	/**
	 * @brief 比較（大也）オペレータ。
	 * @param src	比較するXJsonVal
	 * @retval	true	比較オブジェクトより大さい。
	 * @retval	false	比較オブジェクトより小きい。
	 */
	bool operator >(const XJsonVal &rhs) const;

	/**
	 * @brief 比較（大也。含む）オペレータ。
	 * @param src	比較するXJsonVal
	 * @retval	true	比較オブジェクトより大さい。または、同じ。
	 * @retval	false	比較オブジェクトより小きい。
	 */
	bool operator >=(const XJsonVal &rhs) const;
	

	/**
	 * @brief 指定したプロパティー名の値を取得
	 *        プロパティー名が存在しない場合は、作成する。
	 * @param key	プロパティー名
	 * @return	プロパティー値
	 */
	XJsonVal &operator[](const char *key);
	
	
	/**
	 * @brief 指定したプロパティー名の値を取得
	 *        プロパティー名が存在しない場合は、作成する。
	 * @param key	プロパティー名
	 * @return	プロパティー値
	 */
	XJsonVal &operator[](const wchar_t *key);

	/**
	 * @brief 指定したインデックスの値を取得
	 *        オーバーフローしたインデックスを指定した場合は、配列を指定したインデックス分拡張して、作成したオブジェクトを返す。
	 * @param index	インデックス
	 * @return	プロパティー値
	 */
	XJsonVal &operator[](size_t index);

	/**
	 * @brief データタイプを取得。
	 * @return	データタイプ
	 */
	TYPE getType() const;


	/**
	 * @brief 文字データかどうかをチェックします。
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool isString() const;

	/**
	 * @brief 文字データに変換可能かどうかチェックします。
	 * @retval	true	可能
	 * @retval	false	不可能
	 */
	bool isStringable() const;

	/**
	 * @brief 整数データかどうかをチェックします。
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool isInteger() const;

	/**
	 * @brief 実数データかどうかをチェックします。
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool isDouble() const;

	/**
	 * @brief 数値（整数・実数）データかどうかをチェックします。
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool isNumeric() const;

	/**
	 * @brief オブジェクトデータかどうかをチェックします。
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool isObject() const;

	/**
	 * @brief 配列データかどうかをチェックします。
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool isArray() const;

	/**
	 * @brief 真偽値データかどうかをチェックします。
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool isBoolean() const;

	/**
	 * @brief NULLデータかどうかをチェックします。
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool isNull() const;


	/**
	 * @brief 文字データを取得します。
	 * @return	文字データ
	 */
	const XStrBuff &getString() const;

	/**
	 * @brief 文字データを取得します。文字データで無い場合は、引数の値を返します。
	 * @param	defaultValue	文字データで無い場合のデフォルト値。
	 * @return	文字データ
	 */
	const XStrBuff &tryGetString(const XStrBuff &defaultValue) const;


	/**
	 * @brief 文字列に変換した値を取得します。変換可能なデータタイプは、（文字、整数、実数、真偽値、NULL）
	 * @return	文字データ
	 */
	const XStrBuff getToString() const;

	/**
	 * @brief 文字データをセットします。
	 * @param	newString	セットするC-Style文字
	 */
	void setString(const char* newString);

	/**
	 * @brief 文字データをセットします。
	 * @param	newString	セットするC-Style文字
	 */
	void setString(const wchar_t* newString);


	/**
	 * @brief 整数データを取得します。
	 * @return	整数データ
	 */
	int getInteger() const;

	/**
	 * @brief	整数データを取得します。整数データで無い場合は、引数の値を返します。
	 * @param	defaultValue	整数データで無い場合のデフォルト値。
	 * @return	整数データ
	 */
	int tryGetInteger(int defaultValue) const;

	/**
	 * @brief 整数データをセットします。
	 * @param	newInt	セットする整数
	 */
	void setInteger(int newInt);

	/**
	 * @brief 実数データを取得します。
	 * @return	整数データ
	 */
	double getDouble() const;

	/**
	 * @brief	整数データを取得します。整数データで無い場合は、引数の値を返します。
	 * @param	defaultValue	整数データで無い場合のデフォルト値。
	 * @return	整数データ
	 */
	double tryGetDouble(double defaultValue) const;


	/**
	 * @brief 実数(16Bye)データを取得します。
	 * @return	実数(16Bye)データ
	 */
	float getFloat() const;

	/**
	 * @brief	実数(16Bye)データを取得します。実数データで無い場合は、引数の値を返します。
	 * @param	defaultValue	実数データで無い場合のデフォルト値。
	 * @return	実数(16Bye)データ
	 */
	float tryGetFloat(float defaultValue) const;


	/**
	 * @brief 実数データをセットします。
	 * @param	newDouble	セットする実数
	 */
	void setDouble(double newDouble);

	/**
	 * @brief オブジェクトデータを取得します。
	 * @return	オブジェクトデータ
	 */
	const XJsonObj &getObject() const;

	/**
	 * @brief オブジェクトデータをセットします。
	 * @param	newObject	セットするオブジェクト
	 */
	void setObject(const XJsonObj &newObject);

	/**
	 * @brief 配列データを取得します。
	 * @return	配列データ
	 */
	const XJsonArray &getArray() const;

	/**
	 * @brief 配列データをセットします。
	 * @param	newArray	セットする配列
	 */
	void setArray(const XJsonArray &newArray);

	/**
	 * @brief 真偽値データを取得します。
	 * @return	真偽値データ
	 */
	bool getBoolean() const;

	/**
	 * @brief	真偽値データを取得します。真偽値データで無い場合は、引数の値を返します。
	 * @param	defaultValue	真偽値データで無い場合のデフォルト値。
	 * @return	真偽値データ
	 */
	bool tryGetBoolean(bool defaultValue) const;

	/**
	 * @brief 真偽値データをセットします。
	 * @param	newBoolean	セットする真偽値
	 */
	void setBoolean(bool newBoolean);

	/**
	 * @brief NULLをセットします。
	 */
	void setNull();


	/**
	 * @brief	引数で指定したJson文字列を解析して、値を構築します。
	 * @param	json	Json文字列
	 * @param	pErrorRepoter	Json文字列解析エラー時に、エラーを報告するインターフェース。NULLを指定すると報告されない。
	 * @retval	!=NULL	解析が終了したJson文字の位置。
 	 * @retval	==NULL	解析エラーが発生した。
	 */
	const TCHAR* loadFromString(const TCHAR* json, XJsonValErrorReport* pErrorRepoter = NULL);
	


	/**
	 * @brief	JSON文字列をストリームに書き込む。
	 * @param	stream	ストリーム
	 * @param	nDepth	インデントの深さ。~0を指定すると、インデントを書き込まない。
	 * @param	bNonEscape	文字をエスケープする場合はfalse。
	 */
	void print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth = (size_t)~0, bool bNonEscape = false) const;

	/**
	 * @brief	指定した文字をエスケープして、ストリームに書き込み。
	 * @param	stream	ストリーム
	 * @param	str	文字
	 */
	static void printEscapeString(XLIB_STD_OUT_STR_STREAM &stream, const XStrBuff& str);

private:
	union ValueDataPointer {
		XStrBuff		*m_pStringValue;
		int				*m_pIntValue;
		double			*m_pDoubleValue;
		XJsonObj		*m_pObjectValue;
		XJsonArray		*m_pArrayValue;
		bool			*m_pBoolValue;

		ValueDataPointer();
		ValueDataPointer(XStrBuff *newStringValue);
		ValueDataPointer(int *newIntValue);
		ValueDataPointer(double *newDoubleValue);
		ValueDataPointer(XJsonObj *newObjectValue);
		ValueDataPointer(XJsonArray *newArrayValue);
		ValueDataPointer(bool *newBoolValue);
	};

	static bool isHexDigit(TCHAR digit);
	static bool isWhiteSpace(TCHAR whiteSpace);
	static const TCHAR* readString(const TCHAR* p_str, XStrBuff &result, XJsonValErrorReport* pErrorRepoter);
	static const TCHAR* readObject(const TCHAR* p_str, XJsonObj &result, XJsonValErrorReport* pErrorRepoter);
	static const TCHAR* readArray(const TCHAR* p_str, XJsonArray &result, XJsonValErrorReport* pErrorRepoter);
	static const TCHAR* readNumber(const TCHAR* p_str, XJsonVal &result, XJsonValErrorReport* pErrorRepoter);
	static const TCHAR* readToNonWhiteSpace(const TCHAR* p_str,
		                            TCHAR &currentCharacter);
	void clear();


	TYPE					m_type;
	ValueDataPointer		m_data;



public:
};


#endif /*__X_JSON_VALUE_HEADER_FILE__*/