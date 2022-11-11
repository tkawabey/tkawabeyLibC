/**
 * @file xsemi_ackcode.h
 * @brief 半導体製造SEMI規格 ACKコードデータ管理クラス
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_ACKCODE_H__
#define __XSEMI_ACKCODE_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata_rich.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"


class XLIB_API XSEMI_GemConst;

class XLIB_API XSEMI_ACKCODE;
class XLIB_API XSEMI_ACKCODEs;

DLL_EXTERN template class XLIB_API std::vector<XSEMI_ACKCODE*>;


/**	@brief	ACKコード	*/
class XLIB_API XSEMI_ACKCODE : public XSEMI_GemConstObjectAtom	
{

public:
	enum ATOM_POS {
			ATOM_POS_ACKKIND = 0
		,	ATOM_POS_ACKCODE
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};
	enum KIND {
			KIND_ACKA = 0		// S5F14,F15,F18,S16F2,F4,F6,F12,F14,F16,F18,F24,F26,F30,S17F4,F8,F14
		,	KIND_ACKC3			// S3F6,F8,F10
		,	KIND_ACKC5			// S5F2,F4
		,	KIND_ACKC6			// S6F2,F4,F10,F12,F14
		,	KIND_ACKC7			// S7F4,F12,F14,F16,F18,F24,F32
		,	KIND_ACKC7A			// S7F27
		,	KIND_ACKC10			// S10F2,F4,F6,F10
		,	KIND_ACKC13			// S13F2,F4,F6,F8
		,	KIND_CAACK			// Carrier Action Acknowledge Code, S3F18,F20,F22,F24,F26
		,	KIND_CEPACK			// S2F50
		,	KIND_CPACK			// S2F42
		,	KIND_CSAACK			// S2F8
		,	KIND_DATAACK		// S14F22
		,	KIND_DRACK			// S2F34
		,	KIND_EAC			// S2F16
		,	KIND_ERACK			// S2F38
		,	KIND_ERRCODE		//
		,	KIND_GRANT			// S2F2,F40,S3F16,S4F26,S13F12,S14F24,S16F2,F12
		,	KIND_GRANT6			// S6F6
		,	KIND_GRANT1			// S12F6
		,	KIND_HCACK			// S2F42,F50
		,	KIND_HOHALTACK		// S4F41
		,	KIND_LIMITACK		// S2F46
		,	KIND_LRACK			// S2F36
		,	KIND_LVACK			// S2F46
		,	KIND_MAPER			// S12F19
		,	KIND_MAPFT			// S12F3,F5
		,	KIND_MDACK			// S12F8,F10,F12
		,	KIND_MIDAC			// S3F14
		,	KIND_MIDRA			// S3F12
		,	KIND_OBJACK			// S14F2,F6,F8,F10,F12,F14,F16,F18,F26,F28
		,	KIND_OFLACK			// S1F16
		,	KIND_ONLACK			// S1F18
		,	KIND_PPGNT			// S7F2,F30
		,	KIND_RAC			// S2F20
		,	KIND_RMACK			// S15F4,F6,F8,F10,F12,F14.....
		,	KIND_RMGRANT		// S15F2,F37,F46
		,	KIND_RRACK			// S4F18
		,	KIND_RSACK			// S4F2
		,	KIND_RSDA			// S6F24
		,	KIND_RSPACK			// S2F44
		,	KIND_SDACK			// S12F2
		,	KIND_SPAACK			// S2F4
		,	KIND_SSACK			// S18F2,F4,F6,F10,F12,F14
		,	KIND_SVACK			// S14F20
		,	KIND_TBLACK			// S13F14,F16
		,	KIND_TIAACK			// S2F24
		,	KIND_TIACK			// S2F32
		,	KIND_TRACK			// S4F20,F22,F23
		,	KIND_VLAACK			// S2F46
	};
// プロパティ
	/**	@brief	コンストラクタ
     *
	 *	@param	ackkind	ACKの種類
	 *	@param	code	コード
	*/
	XSEMI_ACKCODE(XD_UINT4 ackkind, XD_UINT1 code);
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_ACKCODE();

// property	
	/**	@brief	ACKの種類を取得します。
	 *
	 *	@return	XD_UINT4
	 */
	XD_UINT4 get_AckKind() const;

	/**	@brief	ACKコードを取得します。
	 *	@return	ACKコード
	 */
	XD_UINT1 get_AckCode() const;
	/**	@brief	ACKコードを設定します。
	 *
	 *	@param	val	ACKコード
	 */
	void set_AckCode(XD_UINT1 val);

	/**	@brief	コメントを取得します。
	 *	@return	コメント
	 */
	const TCHAR* get_Comment();
	/**	@brief	コメントを設定します。
	 *	@param	p_comment	コメント
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

// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT4					m_ulAckKind;	// Ack Kind
	XD_UINT1					m_ulAckCode;	// ACKCODE
	XStrBuff					m_strComment;	// Comment
	bool						m_bEnable;		// 有効/無効

	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_ACKCODEs;
};



/**	@brief	ACKコードの一覧管理クラス	 */
class XLIB_API XSEMI_ACKCODEs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	コンストラクタ */
	XSEMI_ACKCODEs();

	/**	@brief	デストラクタ */
	virtual ~XSEMI_ACKCODEs();

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
	XSEMI_ACKCODE* get(XD_UINT4 index);

	/**	@brief	指定したCEIDのアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_ACKCODE* get_byID(XD_UINT4 kind, XD_UINT4 code);

	/**	@brief	一覧の先頭アイテムを取得する。
	 *
	 *	@return	アイテム。見つからない場合はNULL。
	 */
	XSEMI_ACKCODE* getFirstItem();

	/**	@brief	現在の一覧のカレント位置の次のアイテムを取得する。
	 *
	 *	@return	アイテム。最後に達した場合は、NULLを返す。
	 */
	XSEMI_ACKCODE* getNextItem();

	/**	@brief	現在の一覧のカレント位置の前のアイテムを取得する。
	 *
	 *	@return	アイテム。先頭に達した場合は、NULLを返す。
	 */
	XSEMI_ACKCODE* getPrevItem();

	/**	@brief	アイテムを追加します。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_ACKCODE* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	アイテムを削除します。
	 *
	 *	@param	pCEID	アイテムを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool del(XSEMI_ACKCODE* pAckCode);
protected:
	std::vector<XSEMI_ACKCODE*>				m_array;
	std::vector<XSEMI_ACKCODE*>::iterator	m_ite;
	bool									m_bloaded;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};


#include "xlib/xlib_tail.h"
#endif /* __XSEMI_ACKCODE_H__ */
