/**
 * @file xjson_array.h
 * @brief JSON-Array（配列）クラス
 *
 * @author T.Fujiha
 * @date 2012-02-15
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_JSON_ARRAY_HEADER_FILE__
#define __X_JSON_ARRAY_HEADER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"
#include "xlib/xmap.h"


class XLIB_API XJsonVal;

/**
 * @brief JSON-Array（配列）クラス
 */
class XLIB_API XJsonArray
{
public:	
	/**
	 * @brief コンストラクタ。
	 */	
	XJsonArray();
	/**
	 * @brief コンストラクタ。
	 * @param	val	代入するオブジェクト
	 */	
	XJsonArray(const XJsonArray& val);
	/**
	 * @brief デストラクタ
	 */	
	virtual ~XJsonArray();
	
	/**
	 * @brief 比較（一致）オペレータ。
	 * @param src	比較する配列
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool operator==(const XJsonArray &rhs) const;
	/**
	 * @brief 比較（不一致）オペレータ。
	 * @param src	比較する配列
	 * @retval	true	不一致
	 * @retval	false	一致
	 */
	bool operator!=(const XJsonArray &rhs) const;
	/**
	 * @brief 比較（小也）オペレータ。
	 * @param src	比較する配列
	 * @retval	true	比較配列より小さい。
	 * @retval	false	比較配列より大きい。
	 */
	bool operator<(const XJsonArray &rhs) const;
	/**
	 * @brief 比較（小也。含む）オペレータ。
	 * @param src	比較する配列
	 * @retval	true	比較配列より小さい。または、同じ。
	 * @retval	false	比較配列より大きい。
	 */
	bool operator<=(const XJsonArray &rhs) const;
	/**
	 * @brief 比較（大也）オペレータ。
	 * @param src	比較する配列
	 * @retval	true	比較配列より大さい。
	 * @retval	false	比較配列より小きい。
	 */
	bool operator>(const XJsonArray &rhs) const;
	/**
	 * @brief 比較（大也。含む）オペレータ。
	 * @param src	比較する配列
	 * @retval	true	比較配列より大さい。または、同じ。
	 * @retval	false	比較配列より小きい。
	 */
	bool operator>=(const XJsonArray &rhs) const;

	/**
	 * @brief 指定したインデックスの値を取得
	 *        オーバーフローしたインデックスを指定した場合は、配列を指定したインデックス分拡張して、作成したオブジェクトを返す。
	 * @param index	インデックス
	 * @return	プロパティー値
	 */
	XJsonVal &operator[](size_t index);
	
	/**
	 * @brief 指定したインデックスの値を取得
	 *        オーバーフローしたインデックスを指定した場合は、配列を指定したインデックス分拡張して、作成したオブジェクトを返す。
	 * @param index	インデックス
	 * @return	プロパティー値
	 */
	XJsonVal &at(size_t index){ return (*this)[index];}

	/**
	 * @brief 配列数を取得します。
	 * @return	配列数
	 */
	size_t size() const;
	
	/**
	 * @brief 配列の数を拡張します。
	 * @param n		配列数
	 */
	void resize(size_t n);

	/**
	 * @brief 配列の最後にXJsonValを追加します。
	 */
	void push_back(XJsonVal& newVal);
	/**
	 * @brief 配列の最後の要素を削除します。
	 */
	void pop_back();
	/**
	 * @brief 配列の最後の要素を取得します。
	 */
	XJsonVal& back();

	/**
	 * @brief	JSON文字列をストリームに書き込む。
	 * @param	stream	ストリーム
	 * @param	nDepth	インデントの深さ。~0を指定すると、インデントを書き込まない。
	 * @param	bNonEscape	文字をエスケープする場合はfalse。
	 */
	void print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth = (size_t)~0, bool bNonEscape = false) const;
protected:
	void*								m_pObj;
};

#endif /*__X_JSON_ARRAY_HEADER_FILE__*/
