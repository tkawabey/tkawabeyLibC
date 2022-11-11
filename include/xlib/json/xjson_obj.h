/**
 * @file xjson_obj.h
 * @brief JSON-Objectクラス
 *
 * @author T.Fujiha
 * @date 2012-02-15
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_JSON_OBJECT_HEADER_FILE__
#define __X_JSON_OBJECT_HEADER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"

class XLIB_API XJsonVal;

/**
 * @brief JSON-Objectクラス
 */
class XLIB_API XJsonObj
{
public:	
	
	/**
	 * @brief プロパティーにアクセスするためのイテレータークラス
	 */
	class Iterator {
	public:
		/**
		 * @brief コンストラクタ。
		 */
		Iterator();
		/**
		 * @brief コンストラクタ。
		 * @param	right	代入するイテレーター
		 */
		Iterator(const Iterator& right);
		/**
		 * @brief デストラクタ
		 */
		virtual ~Iterator();
		
		/**
		 * @brief 代入演算子
		 * @param	src	代入するイテレーター
		 */
		Iterator &operator=(const Iterator &src);

		/**
		 * @brief イテレーターの指し示すプロパティーの位置を次のプロパティーに移動させます。
		 */
		Iterator &operator++();
		/**
		 * @brief イテレーターの指し示すプロパティーの位置を前のプロパティーに移動させます。
		 */
		Iterator &operator--();
		
		/**
		 * @brief イテレーターの指し示すプロパティーの位置が先頭かどうかチェック
		 * @retval	true	先頭
		 * @retval	false	先頭でない
		 */
		bool isBegin();
		/**
		 * @brief イテレーターの指し示すプロパティーの位置が最後かどうかチェック
		 * @retval	true	最後
		 * @retval	false	最後でない
		 */
		bool isEnd();

		/**
		 * @brief イテレーターの指し示すプロパティーの名前を取得
		 * @return	プロパティー名
		 */
		const XStrBuff& getKey();
		/**
		 * @brief イテレーターの指し示すプロパティーの値を取得
		 * @return	プロパティー値
		 */
		XJsonVal getValue();

	protected:
		void*	m_pIte;
		void*	m_pMap;

		friend class XJsonObj;
	};


	/**
	 * @brief コンストラクタ。
	 */	
	XJsonObj();
	/**
	 * @brief コンストラクタ。
	 * @param	val	代入するオブジェクト
	 */	
	XJsonObj(const XJsonObj& val);
	/**
	 * @brief デストラクタ
	 */	
	virtual ~XJsonObj();
	
	/**
	 * @brief プロパティーをセットします。
	 * @param	key	プロパティー名
	 * @param	val	プロパティー値
	 * @retval	true	成功
	 * @retval	false	失敗
	 */	
	bool add(const XStrBuff& key, XJsonVal& val);
	/**
	 * @brief プロパティーを削除します。
	 * @param	strKey	プロパティー名
	 * @retval	true	成功
	 * @retval	false	失敗
	 */	
	bool del(const XStrBuff &strKey);
	/**
	 * @brief セットされてプロパティー数を返します。
	 * @return	プロパティー数
	 */	
	size_t	size() const;
	/**
	 * @brief イテレータを取得します。
	 * @return	イテレータ
	 */	
	Iterator getIterator() const;
	
	/**
	 * @brief 比較（一致）オペレータ。
	 * @param src	比較するオブジェクト
	 * @retval	true	一致
	 * @retval	false	不一致
	 */
	bool operator==(const XJsonObj &rhs) const;	
	/**
	 * @brief 比較（不一致）オペレータ。
	 * @param src	比較するオブジェクト
	 * @retval	true	不一致
	 * @retval	false	一致
	 */
	bool operator!=(const XJsonObj &rhs) const;
	/**
	 * @brief 比較（小也）オペレータ。
	 * @param src	比較するオブジェクト
	 * @retval	true	比較オブジェクトより小さい。
	 * @retval	false	比較オブジェクトより大きい。
	 */
	bool operator<(const XJsonObj &rhs) const;
	/**
	 * @brief 比較（小也。含む）オペレータ。
	 * @param src	比較するオブジェクト
	 * @retval	true	比較オブジェクトより小さい。または、同じ。
	 * @retval	false	比較オブジェクトより大きい。
	 */
	bool operator<=(const XJsonObj &rhs) const;
	/**
	 * @brief 比較（大也）オペレータ。
	 * @param src	比較するオブジェクト
	 * @retval	true	比較オブジェクトより大さい。
	 * @retval	false	比較オブジェクトより小きい。
	 */
	bool operator>(const XJsonObj &rhs) const;
	/**
	 * @brief 比較（大也。含む）オペレータ。
	 * @param src	比較するオブジェクト
	 * @retval	true	比較オブジェクトより大さい。または、同じ。
	 * @retval	false	比較オブジェクトより小きい。
	 */
	bool operator>=(const XJsonObj &rhs) const;

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
	 * @brief	JSON文字列をストリームに書き込む。
	 * @param	stream	ストリーム
	 * @param	nDepth	インデントの深さ。~0を指定すると、インデントを書き込まない。
	 * @param	bNonEscape	文字をエスケープする場合はfalse。
	 */
	void print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth = (size_t)~0, bool bNonEscape = false) const;
protected:
	void*								m_pObj;
};

#endif /*__X_JSON_OBJECT_HEADER_FILE__*/