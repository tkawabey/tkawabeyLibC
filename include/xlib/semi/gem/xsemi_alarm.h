/**
 * @file xsemi_alarm.h
 * @brief 半導体製造SEMI規格　アラームデータ管理クラス
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_ALARAM_H__
#define __XSEMI_ALARAM_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata_rich.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"


class XLIB_API XSEMI_GemConst;


class XLIB_API XSEMI_ALARM;
class XLIB_API XSEMI_ALARMs;
DLL_EXTERN template class XLIB_API std::vector<XSEMI_ALARM*>;

//typedef XLIB_API std::vector<XSEMI_ALARM*> XSEMI_ALARMs;


/**	@brief	アラームデータ管理クラス	*/
class XLIB_API XSEMI_ALARM : public XSEMI_GemConstObjectAtom
{
public:
	enum ATOM_POS {
			ATOM_POS_ALID = 0
		,	ATOM_POS_ALCD
		,	ATOM_POS_NAME
		,	ATOM_POS_TEXT
		,	ATOM_POS_ON_CEID
		,	ATOM_POS_OFF_CEID
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};


// プロパティ
	/**	@brief	コンストラクタ
     *
	 *	@param	alarmID	ID
	 *	@param	p_name	名前
	*/
	XSEMI_ALARM(XD_UINT4 alarmID, const TCHAR* p_name);
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_ALARM();
	

	XD_UINT4 get_AlarmID();
	XD_UINT1 get_ALCD() const;
	void set_ALCD(XD_UINT1 val);
	const TCHAR* get_Name();
	void set_Name(const TCHAR* p_name);
	const TCHAR* get_Text();
	void set_Text(const TCHAR* p_text);
	const TCHAR* get_Comment();
	void set_Comment(const TCHAR* p_comment);

	XD_UINT4 get_OnCEID() const;
	void set_OnCEID(XD_UINT4 val);
	XD_UINT4 get_OffCEID() const;
	void set_OffCEID(XD_UINT4 val);

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


// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT4					m_ulAlarmId;	// ALARM ID
	XD_UINT1					m_alcd;			// ALCD
	XStrBuff					m_strName;		// Name
	XStrBuff					m_strText;		// Text
	XStrBuff					m_strComment;	// Comment
	XD_UINT4					m_ulOnEvent;	// On Event
	XD_UINT4					m_ulOffEvent;	// Off Event
	bool						m_bEnable;		// 有効/無効

	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_ALARMs;
};





/**	@brief	アラームデータの一覧管理クラス	 */
class XLIB_API XSEMI_ALARMs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	コンストラクタ */
	XSEMI_ALARMs();
	/**	@brief	デストラクタ */
	virtual ~XSEMI_ALARMs();
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
	XSEMI_ALARM* get(XD_UINT4 index);
	/**	@brief	指定したALARMのアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_ALARM* get_byID(XD_UINT4 id);

	/**	@brief	名前からALARMのアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_ALARM* get_byName(const TCHAR* p_name);

	/**	@brief	一覧の先頭アイテムを取得する。
	 *
	 *	@return	アイテム。見つからない場合はNULL。
	 */
	XSEMI_ALARM* getFirstItem();
	/**	@brief	現在の一覧のカレント位置の次のアイテムを取得する。
	 *
	 *	@return	アイテム。最後に達した場合は、NULLを返す。
	 */
	XSEMI_ALARM* getNextItem();
	/**	@brief	現在の一覧のカレント位置の前のアイテムを取得する。
	 *
	 *	@return	アイテム。先頭に達した場合は、NULLを返す。
	 */
	XSEMI_ALARM* getPrevItem();
	/**	@brief	アイテムを追加します。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_ALARM* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	アイテムを削除します。
	 *
	 *	@param	pCEID	アイテムを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool del(XSEMI_ALARM* pCEID);
protected:
	std::vector<XSEMI_ALARM*>			m_array;
	std::vector<XSEMI_ALARM*>::iterator	m_ite;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};


#include "xlib/xlib_tail.h"
#endif /* __XSEMI_ALARAM_H__ */
