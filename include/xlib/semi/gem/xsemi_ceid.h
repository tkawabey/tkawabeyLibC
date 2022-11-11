/**
 * @file xsemi_ceid.h
 * @brief 半導体製造SEMI規格　収集イベント(CEID)管理クラス
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2006 T.Fujiha,
 */
#ifndef __XSEMI_CEID_H__
#define __XSEMI_CEID_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"

class XLIB_API XSEMI_Gem;
class XLIB_API XSEMI_GemConst;
class XLIB_API XSEMI_Rpt;
class XLIB_API XSEMI_Rpts;
class XLIB_API XSEMI_CEID;


#include "xlib/semi/gem/xsemi_gem_const_obj.h"
#include "xlib/semi/gem/xsemi_rpt.h"
#include <vector>

class XLIB_API XSEMI_CEIDs;

//typedef XLIB_API std::vector<XSEMI_Rpt*> XSEMI_Rpts;

DLL_EXTERN template class XLIB_API std::vector<XSEMI_CEID*>;


/**	@brief	収集イベント(CEID)管理クラス	 */

class XLIB_API XSEMI_CEID :  public XSEMI_GemConstObjectAtom
{
public:
	enum ATOM_POS {
			ATOM_POS_CEID = 0
		,	ATOM_POS_NAME
		,	ATOM_POS_DISPLAY_NAME
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};


	/**	@brief	コンストラクタ
	 *
	 *	@param	ceid	CEID
	 *	@param	p_name	イベント名
	 *	@param	p_comment	コメント
	 */
	XSEMI_CEID(XD_UINT4 ceid, const TCHAR* p_name, const TCHAR* p_comment);
	/**	@brief	デストラクタ	 */
	virtual ~XSEMI_CEID();


	/**	@brief	CEIDを取得します。
	 *
	 *	@return	CEID
	 */
	XD_UINT4 get_CEID() const ;
	/**	@brief	イベント名を取得します。
	 *
	 *	@return	イベント名
	 */
	const TCHAR* get_Name();
	/**	@brief	イベント名を設定します。
	 *
	 *	@param	p_name	イベント名
	 */
	void set_Name(const TCHAR* p_name);


	/**	@brief	イベント表示名を取得します。
	 *
	 *	@return	イベント表示名
	 */
	const TCHAR* get_DisplayName();
	/**	@brief	イベント表示名を設定します。
	 *
	 *	@param	p_name	イベント表示名
	 */
	void set_DisplayName(const TCHAR* p_name);


		
	/**	@brief	コメントを取得します。
	 *
	 *	@return	コメント
	 */
	const TCHAR* get_Comment();
	/**	@brief	コメントを設定します。
	 *
	 *	@param	p_comment	コメント
	 *	
	 */
	void set_Comment(const TCHAR* p_comment);

	/**	@brief	有効/無効フラグを取得します。
	 *
	 *	@retval	true	有効
	 *	@retval	false	無効
	 */
	bool get_Enable() const;
	/**	@brief	有効/無効フラグを取得します。
	 *
	 *	@param	val	true:有効/false:無効
	 */
	void put_Enable(bool val);


	/**	@brief	ユーザーデータを取得します。
	 *
	 *	@return	ユーザーデータ
	 */
	XD_UINT4 get_UserData() const;
	/**	@brief	ユーザーデータを設定します。
	 *
	 *	@param	val	ユーザーデータ
	 */
	void set_UserData(XD_UINT4 val);

	/**	@brief	CEIDに緋も付くレポートIDの数を帰します。
	 *
	 *	@return	CEIDに緋も付くCEIDの数
	 */
	XD_UINT4 get_RptsCount();
	/**	@brief	指定したインデックスのCEIDに緋も付くレポートIDオブジェクトを取得する。
	 *
	 *	@param	index	０から始まるインデックス
	 *
	 *	@return	レポートIDオブジェクト
	 */
	XSEMI_Rpt* get_Rpt(XD_UINT4 index);
	/**	@brief	レポートIDのCEIDに緋も付くレポートIDオブジェクトを取得する。
	 *
	 *	@param	rptid	レポートID
	 *
	 *	@return	レポートIDオブジェクト
	 */
	XSEMI_Rpt* get_RptByID(XD_UINT4 rptid);

// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();

	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
// overwrite	XSEMI_GemConstObjectAtom
	virtual void* getAtomValPtr(int index);
	virtual void makeEventLinkClearString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	XD_UINT4					m_ceid;				// CEID
	XStrBuff					m_strName;			// Name
	XStrBuff					m_strDisplayName;	// DisplayName
	XStrBuff					m_strComment;		// Comment
	bool						m_bEnable;			// Enable
	XD_UINT4					m_ulUserData;		// UserData
	XSEMI_Rpts					m_rpts;				// RportIDs
	bool						m_bDirtyLinked;		// リンク情報が変更されたかどうかのフラグ


	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_CEIDs;
};

/**	@brief	収集イベント(CEID)の一覧管理クラス	 */
class XLIB_API XSEMI_CEIDs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	コンストラクタ */
	XSEMI_CEIDs();
	/**	@brief	デストラクタ */
	virtual ~XSEMI_CEIDs();
	/**	@brief	CEIDの一覧を解放します。 */
	void clear();

	/**	@brief	一覧数を取得します
	 *	@return	一覧数
	 */
	XD_UINT4 count();
	/**	@brief	指定したインデックスのアイテムを取得します。
	 *
	 *	@param	index	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_CEID* get(XD_UINT4 index);

	/**	@brief	指定したCEIDクラスのインスタンスを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	CEIDクラスのインスタンス
	 */
	XSEMI_CEID* get_byID(XD_UINT4 id);

	/**	@brief	名前からCEIDクラスのインスタンスを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	CEIDクラスのインスタンス
	 */
	XSEMI_CEID* get_byName(const TCHAR* p_name);

	/**	@brief	一覧の先頭CEIDクラスのインスタンスを取得する。
	 *
	 *	@return	CEIDクラスのインスタンス。見つからない場合はNULL。
	 */
	XSEMI_CEID* getFirstItem();

	/**	@brief	現在の一覧のカレント位置の次のCEIDクラスのインスタンスを取得する。
	 *
	 *	@return	CEIDクラスのインスタンス。最後に達した場合は、NULLを返す。
	 */
	XSEMI_CEID* getNextItem();

	/**	@brief	現在の一覧のカレント位置の前のCEIDクラスのインスタンスを取得する。
	 *
	 *	@return	CEIDクラスのインスタンス。先頭に達した場合は、NULLを返す。
	 */
	XSEMI_CEID* getPrevItem();

	/**	@brief	CEIDクラスのインスタンスを追加します。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_CEID* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	アイテムを追加します。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_CEID* add(XD_UINT4 ceid, const TCHAR* p_name);

	/**	@brief	CEIDクラスのインスタンスを削除します。
	 *
	 *	@param	pCEID	CEIDクラスのインスタンスを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool del(XSEMI_CEID* pCEID);
protected:
	std::vector<XSEMI_CEID*>			m_ceid;
	std::vector<XSEMI_CEID*>::iterator	m_ite;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};

#include "xlib/xlib_tail.h"
#endif /* __XSEMI_CEID_H__ */