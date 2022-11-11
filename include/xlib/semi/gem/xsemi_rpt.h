/**
 * @file xsemi_rpt.h
 * @brief 半導体製造SEMI規格　レポートID管理クラス
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_RPT_H__
#define __XSEMI_RPT_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/semi/gem/xsemi_v.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"
#include <vector>

class XLIB_API XSEMI_GemConst;
class XLIB_API XSEMI_CEID;

/**	@brief	レポートID管理クラス	 */
class XLIB_API XSEMI_Rpt : public XSEMI_GemConstObjectAtom
{
public:
	enum ATOM_POS {
			ATOM_POS_RPTID = 0
		,	ATOM_POS_NAME
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};


	/**	@brief	コンストラクタ
	 *
	 *	@param	rptid	レポートID
	 *	@param	p_name	レポート名
	 */
	XSEMI_Rpt(XD_UINT4 rptid, const TCHAR* p_name);
	/**	@brief	デストラクタ	 */
	virtual ~XSEMI_Rpt();

	/**	@brief	レポートIDを取得します。
	 *
	 *	@return	レポートID
	 */
	XD_UINT4 get_RptID();
	/**	@brief	レポート名を取得します。
	 *
	 *	@return	レポート名
	 */
	const TCHAR* get_Name();
	/**	@brief	レポート名を設定します。
	 *
	 *	@param	p_name	レポート名
	 */
	void set_Name(const TCHAR* p_name);
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
	void set_Enable(bool val);


	/**	@brief	レポートに、リンクしているVの数を取得します。
	 *
	 *	@return	レポートに、リンクしているVの数
	 */
	XD_UINT4 get_VsCount() const;
	/**	@brief	指定したインデックスのレポートに、リンクしているVを取得します。
	 *
	 *	@return	V
	 */
	XSEMI_V* get_V(XD_UINT4 index);
	/**	@brief	指定したIDのレポートに、リンクしているVを取得します。
	 *
	 *	@return	V
	 */
	XSEMI_V* get_VByID(XD_UINT4 vid);



// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();

	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	virtual void* getAtomValPtr(int index);
	virtual void makeDefineReportClearString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	XD_UINT4					m_rptid;		// Report ID
	XStrBuff					m_strName;		// Name
	XStrBuff					m_strComment;	// コメント
	bool						m_bEnable;		// Enable
	XSEMI_Vs					m_vs;
	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Rpts ;
};

/**	@brief	レポート一覧管理クラス	 */
class XLIB_API XSEMI_Rpts : public XSEMI_GemConstObject
{
public:
	/**	@brief	コンストラクタ
	 *  @param bLinkeDatas このインスタンスが、XSEMI_Rptのリンク情報のみ管理するかどうか？
	 */
	XSEMI_Rpts(bool bLinkeDatas);
	/**	@brief	デストラクタ */
	virtual ~XSEMI_Rpts();
	/**	@brief	レポートの一覧を解放します。 */
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
	XSEMI_Rpt* get(XD_UINT4 index);
	/**	@brief	指定したレポートのアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_Rpt* get_byID(XD_UINT4 id);

	/**	@brief	名前からレポートのアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_Rpt* get_byName(const TCHAR* p_name);

	/**	@brief	一覧の先頭アイテムを取得する。
	 *
	 *	@return	アイテム。見つからない場合はNULL。
	 */
	XSEMI_Rpt* getFirstItem();
	/**	@brief	現在の一覧のカレント位置の次のアイテムを取得する。
	 *
	 *	@return	アイテム。最後に達した場合は、NULLを返す。
	 */
	XSEMI_Rpt* getNextItem();
	/**	@brief	現在の一覧のカレント位置の前のアイテムを取得する。
	 *
	 *	@return	アイテム。先頭に達した場合は、NULLを返す。
	 */
	XSEMI_Rpt* getPrevItem();
	/**	@brief	アイテムを追加します。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_Rpt* add(XSEMI_GemDataAtom* pAtom);
	/**	@brief	アイテムを削除します。
	 *
	 *	@param	pRpt	アイテムを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool del(XSEMI_Rpt* pRpt);
protected:
	std::vector<XSEMI_Rpt*>				m_rpts;
	std::vector<XSEMI_Rpt*>::iterator	m_ite;
	bool								m_bLinkDatas;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_CEID;
};


#include "xlib/xlib_tail.h"
#endif /* __XSEMI_RPT_H__ */
