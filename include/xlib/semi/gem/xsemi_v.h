/**
 * @file xsemi_v.h
 * @brief 半導体製造SEMI規格　変数データ（V）管理クラス
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_V_H__
#define __XSEMI_V_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata_rich.h"
#include "xlib/xarray.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"


class XLIB_API XSEMI_GemConst;

class XLIB_API XSEMI_Gem;
class XLIB_API XSEMI_Rpt;
class XLIB_API XSEMI_V;
class XLIB_API XSEMI_ECV;
class XLIB_API XSEMI_SV;
class XLIB_API XSEMI_DV;
class XLIB_API XSEMI_Vs;
class XLIB_API XSEMI_VVAL;
class XLIB_API XSEMI_VARIABLE_GRP;
class XLIB_API XSEMI_VARIABLE_GRPs;


DLL_EXTERN template class XLIB_API std::vector<XSEMI_V*>;
/**	@brief	V	*/
class XLIB_API XSEMI_V : public XSEMI_GemConstObjectAtom
						,public	XRichData
//						,public XSEMI_GemDataAtom	
{
public:
	/**	@brief	Vのクラス	*/
	enum Class {
		SV   = 0,	///< SV
		EC,			///< EC
		DV,			///< DV
		ALARM,		///< ALARM
	};
	/**	@brief	Private値	*/
	enum PRIVAL {
		PRIVAL_NON = 0,		// Privateでない
		PRIVAL_EQP,			// 装置側でPrivateである
		PRIVAL_HOST,		// HOST側でPrivateである
		PRIVAL_BOTH,		// 両方でPrivateである
	};

	enum ATOM_POS {
			ATOM_POS_VID = 0
		,	ATOM_POS_NAME
		,	ATOM_POS_DISPLAY_NAME
		,	ATOM_POS_DATA_UNIT
		,	ATOM_POS_DATA_TYPE
		,	ATOM_POS_MODE
		,	ATOM_POS_INIT_VALUE
		,	ATOM_POS_CATEGORY
		,	ATOM_POS_COMMENT
		,	ATOM_POS_VARIABLE_GRP
		,	ATOM_POS_VRIVATE
		,	ATOM_POS_GET_EX
		,	ATOM_POS_ENABLE
		,	ATOM_POS_VVALUE

		,	ATOM_POS_OBJECT_TYPE = 99
	};
// プロパティ
	/**	@brief	コンストラクタ
     *
	 *	@param	vid	ID
	 *	@param	p_name	名前
	 *	@param	type	データタイプ
	 *	@param	Class	クラス
	*/
	XSEMI_V(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, Class Class, bool bLoaded = false);
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_V();
	
	XD_UINT4 get_VID() const;

	const TCHAR* get_Name();
	void put_Name(const TCHAR* val);

	const TCHAR* get_DisplayName();
	void put_DisplayName(const TCHAR* val);

	XD_UINT1 get_Mode() const;
	Class get_Class() const;

	const TCHAR* get_Unit();
	void put_Unit(const TCHAR* val);


	XRichData& get_InitialValue();

	const TCHAR* get_Comment();
	void put_Comment(const TCHAR* val);
	bool get_Enable() const;
	void put_Enable(bool val);

	XD_UINT4 get_VariableGrpID();
	void put_VariableGrpID(XD_UINT4 val);

	PRIVAL get_Private();
	void put_Private(PRIVAL val);

	bool get_GetEx();
	void put_GetEx(bool val);
// メソッド
	/**	@brief	入力データの妥当性をチェックします。
	 *
	 *	@param	data	入力データ
	 *
	 *	@retval	0	妥当
	 *	@retval	-1	データタイプが違う
	 *	@retval	-2	最小値をオーバーしている
	 *	@retval	-3	最大値をオーバーしている
	 */
	int check(XData &data);

	/**	@brief	The fetch function
	 *
	 *	@param	pGem	a parameter of type XSEMI_Gem*
	 *
	 *	@return	bool
	 */
	bool fetch(XSEMI_Gem* pGem);
// overwrite	XSEMI_GemConstObject
	/**	@brief	オブジェクトのメンバーが変更されたかどうか調べます。
	 *	@copydoc XSEMI_GemConstObject::isDirtyMember()
	 */
	virtual bool isDirtyMember();	
	/**	@brief	オブジェクトのメンバーが変更状態をリセットします。
	 *  @copydoc XSEMI_GemConstObject::resetDirtyMember()
	 */
	virtual void resetDirtyMember();
	virtual void setDetail(XSEMI_GemDataAtom* pAtom);

	XSEMI_VVAL* getVValHeadItem();
	XSEMI_VVAL* addVVal(XRichData& data);
	XSEMI_VVAL* addVVal(XSEMI_GemDataAtom* pAtom);
	void removeVVal(XSEMI_VVAL* val);

// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
	virtual const TCHAR* getString(int index);

	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	virtual void* getAtomValPtr(int index);
	virtual void innerLoadDetail();
protected:
	// VID
	XD_UINT4	m_vid;
	// NAME
	XStrBuff	m_strName;
	// DisplayName
	XStrBuff	m_strDisplayName;
	// Comment
	XStrBuff	m_strComment;
	// Data Calss 
	Class		m_class;
	// Unit name
	XStrBuff	m_strUnit;
	// size
	XD_UINT4	m_size;
	// mode
	XD_UINT1	m_mode;
	// 初期値
	XRichData	m_InitValue;
	// Enable
	bool		m_bEnable;
	// VARIABLE_GRP
	XD_UINT4	m_variable_grpid;
	// Private
	PRIVAL		m_bPrivate;
	// get_ex
	bool		m_bGetEx;
	// VVALS一覧
	XList		m_list;
	// 
	XStrBuff	m_atomTmpBuff;

	enum LD_FLG {
			LD_FLG_DETAIL		= 0x0001
		,	LD_FLG_VALS			= 0x0002
	};
	unsigned long	m_loadedFlg;
	static XSEMI_CONST_SCHEMA	g_pVIDSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Vs; 
friend class XSEMI_ECV; 
friend class XSEMI_SV; 
friend class XSEMI_DV; 
friend class XSEMI_VVAL;
};


/**	@brief	ECV（装置定数）	*/
class XLIB_API XSEMI_ECV : public XSEMI_V
{
public:

	enum ATOM_POS_EC {
			ATOM_POS_EC_MAX = XSEMI_V::ATOM_POS_VVALUE+1
		,	ATOM_POS_EC_MIN
		,	ATOM_POS_EC_DEF
		,	ATOM_POS_EC_SET_EX
		,	ATOM_POS_EC_VERIFY
		,	ATOM_POS_EC_VALUE
	};


	/**	@brief	コンストラクタ
     *
	 *	@param	vid	ID
	 *	@param	p_name	名前
	 *	@param	type	データタイプ
	*/
	XSEMI_ECV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded = false);
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_ECV();



	/**	@brief	ecmax	*/
	XRichData& get_EcMax();
	/**	@brief	ecmin	*/
	XRichData& get_EcMin();
	/**	@brief	ecdef	*/
	XRichData& get_EcDef();


	bool get_SetEx() ;
	void put_SetEx(bool val);

	bool get_Verify() ;
	void put_Verify(bool val);
// overwrite	XSEMI_GemConstObject
	/**	@brief	オブジェクトのメンバーが変更されたかどうか調べます。
	 *	@copydoc XSEMI_GemConstObject::isDirtyMember()
	 */
	virtual bool isDirtyMember();	
	/**	@brief	オブジェクトのメンバーが変更状態をリセットします。
	 *  @copydoc XSEMI_GemConstObject::resetDirtyMember()
	 */
	virtual void resetDirtyMember();
	virtual void setDetail(XSEMI_GemDataAtom* pAtom);


// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();	
	virtual int getColmnCount();										
	virtual const TCHAR* getString(int index);

	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XRichData	m_EcMax;
	XRichData	m_EcMin;
	XRichData	m_EcDef;
	// get_ex
	bool		m_bSetEx;
	// get_ex
	bool		m_bVerify;
	static XSEMI_CONST_SCHEMA	g_pECVSchema[];
};

/**	@brief	SV（状態変数）	*/
class XLIB_API XSEMI_SV : public XSEMI_V
{
public:
	enum ATOM_POS_SV {
			ATOM_POS_SV_SIZE_MAX = XSEMI_V::ATOM_POS_VVALUE+1
		,	ATOM_POS_SV_SIZE_MIN
		,	ATOM_POS_SV_LIMIT_MAX
		,	ATOM_POS_SV_LIMIT_MIN
		,	ATOM_POS_SV_LIMIT_EVENT
	};


	/**	@brief	コンストラクタ
     *
	 *	@param	vid	ID
	 *	@param	p_name	名前
	 *	@param	type	データタイプ
	*/
	XSEMI_SV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded = false);
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_SV();




	/**	@brief	データサイズの最小値を取得
	 *
	 *	@return	データサイズの最小値
	 */
	XD_UINT4 get_SizeMax() ;
	/**	@brief	データサイズの最小値をセット
	 *
	 *	@param	val	データサイズの最小値
	 */
	void put_SizeMax(XD_UINT4 val);

	/**	@brief	データサイズの最大値を取得
	 *
	 *	@return	データサイズの最大値
	 */
	XD_UINT4 get_SizeMin() ;
	/**	@brief	データサイズの最大値をセット
	 *
	 *	@param	val	データサイズの最大値
	 */
	void put_SizeMin(XD_UINT4 val);


	/**	@brief	リミットオーバMAX値
	 *
	 *	@return	リミットオーバMAX値
	 */
	XRichData& get_limitMax();
	/**	@brief	リミットオーバMIN値
	 *
	 *	@return	リミットオーバMIN値
	 */
	XRichData& get_limitMin();

	/**	@brief	リミットオーバーイベントIDを取得
	 *
	 *	@return	リミットオーバーイベントID
	 */
	XD_UINT4 get_CeLimit() ;
	/**	@brief	リミットオーバーイベントIDをセット
	 *
	 *	@param	val	リミットオーバーイベントID
	 */
	void put_CeLimit(XD_UINT4 val);

// overwrite	XSEMI_GemConstObject
	/**	@brief	オブジェクトのメンバーが変更されたかどうか調べます。
	 *	@copydoc XSEMI_GemConstObject::isDirtyMember()
	 */
	virtual bool isDirtyMember();	
	/**	@brief	オブジェクトのメンバーが変更状態をリセットします。
	 *  @copydoc XSEMI_GemConstObject::resetDirtyMember()
	 */
	virtual void resetDirtyMember();
	virtual void setDetail(XSEMI_GemDataAtom* pAtom);


// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();	
	virtual int getColmnCount();										
	virtual const TCHAR* getString(int index);

	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT4	m_nSizeMax;
	XD_UINT4	m_nSizeMin;
	XRichData	m_limitMax;
	XRichData	m_limitMin;
	XD_UINT4	m_nCeLimit;
	static XSEMI_CONST_SCHEMA	g_pSVSchema[];
};

/**	@brief	DV（変数データ）	*/
class XLIB_API XSEMI_DV : public XSEMI_V
{
public:
	enum ATOM_POS_DV {
			ATOM_POS_DV_SIZE_MAX = XSEMI_V::ATOM_POS_VVALUE+1
		,	ATOM_POS_DV_SIZE_MIN
	};
	/**	@brief	コンストラクタ
     *
	 *	@param	vid	ID
	 *	@param	p_name	名前
	 *	@param	type	データタイプ
	*/
	XSEMI_DV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded = false);
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_DV();




	/**	@brief	データサイズの最小値を取得
	 *
	 *	@return	データサイズの最小値
	 */
	XD_UINT4 get_SizeMax() ;
	/**	@brief	データサイズの最小値をセット
	 *
	 *	@param	val	データサイズの最小値
	 */
	void put_SizeMax(XD_UINT4 val);

	/**	@brief	データサイズの最大値を取得
	 *
	 *	@return	データサイズの最大値
	 */
	XD_UINT4 get_SizeMin() ;
	/**	@brief	データサイズの最大値をセット
	 *
	 *	@param	val	データサイズの最大値
	 */
	void put_SizeMin(XD_UINT4 val);

	virtual void setDetail(XSEMI_GemDataAtom* pAtom);




// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();	
	virtual int getColmnCount();										
	virtual const TCHAR* getString(int index);

	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT4	m_nSizeMax;
	XD_UINT4	m_nSizeMin;
	static XSEMI_CONST_SCHEMA	g_pDVSchema[];
};


/**	@brief	VVAL	*/
class XLIB_API XSEMI_VVAL :	public XSEMI_GemConstObjectAtom
						,	public XRichData
{
public:
// Property
	const TCHAR* get_Comment();
	void put_Comment(const TCHAR* val);
	bool get_Enable();
	void put_Enable(bool val);	

	XSEMI_VVAL* nextSibling();
protected:
	XSEMI_VVAL(XSEMI_GemConstObject* pParent, XDATA_TYPE type);

public:
	enum ATOM_POS {
			ATOM_POS_VID = 0
		,	ATOM_POS_VALUE
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE
		,	ATOM_POS_OBJECT_TYPE = 99
	};
	
// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
	virtual const TCHAR* getString(int index);
	virtual XD_UINT4    getUint(int index);

protected:
// overwrite	XSEMI_GemConstObjectAtom
	virtual void* getAtomValPtr(int index);
protected:
	// Comment
	XStrBuff	m_strComment;
	// Enable
	bool		m_bEnable;
	// 
	XStrBuff	m_atomTmpBuff;
	XPOSITION	m_pos;
	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_V;
};



/**	@brief	V分類クラス	*/
class XLIB_API XSEMI_VARIABLE_GRP :		public XSEMI_GemConstObjectAtom	
{
public:
	enum ATOM_POS {
			ATOM_POS_ID = 0
		,	ATOM_POS_NAME
		,	ATOM_POS_COMMENT

		,	ATOM_POS_OBJECT_TYPE = 99
	};

	/**	@brief	コンストラクタ
	 *
	 *	@param	id	ID
	 *	@param	p_name	名前
	 */
	XSEMI_VARIABLE_GRP(XD_UINT4 id, const TCHAR* p_name);
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_VARIABLE_GRP();


	
	/**	@brief	IDを取得します。
	 *	@return	ID
	 */
	XD_UINT4 get_ID() const;
	/**	@brief	名前を取得します。
	 *	@return	名前
	 */
	const TCHAR* get_Name();
	/**	@brief	名前を設定します。
	 *	@param	val	名前
	 */
	void put_Name(const TCHAR* val);
	/**	@brief	コメントを取得します。
	 *	@return	コメント
	 */
	const TCHAR* get_Comment();
	/**	@brief	コメントを設定します。
	 *	@param	val	コメント
	 */
	void put_Comment(const TCHAR* val);


// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
protected:
// overwrite	XSEMI_GemConstObjectAtom
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT4	m_nID;
	XStrBuff	m_strName;
	XStrBuff	m_strComment;
	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XLIB_API XSEMI_VARIABLE_GRPs;
};



/**	@brief	VIDの一覧管理クラス	 */
class XLIB_API XSEMI_Vs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	コンストラクタ */
	XSEMI_Vs(bool bLinkeDatas);
	/**	@brief	デストラクタ */
	virtual ~XSEMI_Vs();
	/**	@brief	VIDの一覧を解放します。 */
	void clear();

	/**	@brief	一覧数を取得します
	 *	@return	一覧数
	 */
	XD_UINT4 count() const;
	/**	@brief	SV一覧数を取得します
	 *	@return	SV一覧数
	 */
	XD_UINT4 countSV() const;
	/**	@brief	ECV一覧数を取得します
	 *	@return	ECV一覧数
	 */
	XD_UINT4 countECV() const;
	/**	@brief	DV一覧数を取得します
	 *	@return	DV一覧数
	 */
	XD_UINT4 countDV() const;


	/**	@brief	指定したインデックスのアイテムを取得します。
	 *
	 *	@param	index	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_V* get(XD_UINT4 index);

	/**	@brief	指定したインデックスのECVアイテムを取得します。
	 *
	 *	@param	index	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_ECV* getECV(XD_UINT4 index);
	/**	@brief	指定したインデックスのSVアイテムを取得します。
	 *
	 *	@param	index	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_SV* getSV(XD_UINT4 index);
	/**	@brief	指定したインデックスのDVアイテムを取得します。
	 *
	 *	@param	index	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_DV* getDV(XD_UINT4 index);

	/**	@brief	指定したVIDのアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_V* get_byID(XD_UINT4 id);
	/**	@brief	名前からVIDのアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_V* get_byName(const TCHAR* p_name);

	/**	@brief	指定したVIDのECVアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_ECV* get_EC_byID(XD_UINT4 id);
	/**	@brief	指定したVIDのSVアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_SV*  get_SV_byID(XD_UINT4 id);
	/**	@brief	指定したVIDのDVアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_DV*  get_DV_byID(XD_UINT4 id);

	/**	@brief	一覧の先頭アイテムを取得する。
	 *
	 *	@return	アイテム。見つからない場合はNULL。
	 */
	XSEMI_V* getFirstItem();
	/**	@brief	現在の一覧のカレント位置の次のアイテムを取得する。
	 *
	 *	@return	アイテム。最後に達した場合は、NULLを返す。
	 */
	XSEMI_V* getNextItem();
	/**	@brief	現在の一覧のカレント位置の前のアイテムを取得する。
	 *
	 *	@return	アイテム。先頭に達した場合は、NULLを返す。
	 */
	XSEMI_V* getPrevItem();
	/**	@brief	アイテムを追加します。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_V* add(XSEMI_GemDataAtom* pAtom);
	/**	@brief	アイテムを追加します。pAtomから詳細項目は取り出し、設定しません。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_V* addNoDetail(XSEMI_GemDataAtom* pAtom);
	/**	@brief	アイテムを追加します。pAtomから詳細項目は取り出し、マスターデータに登録します。
     *
	 *	@param	vid	ID
	 *	@param	p_name	名前
	 *	@param	type	データタイプ
	 *	@param	Class	クラス
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	*/
	XSEMI_V* add(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, XSEMI_V::Class Class);

	/**	@brief	アイテムを削除します。
	 *
	 *	@param	pCEID	アイテムを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool del(XSEMI_V* pV);

private:
	static bool setRiDataFromString(XRichData* pRdata, const TCHAR* p_str);
protected:
	std::vector<XSEMI_V*>				m_vs;
	std::vector<XSEMI_SV*>				m_SV;
	std::vector<XSEMI_ECV*>				m_EC;
	std::vector<XSEMI_DV*>				m_DV;
	std::vector<XSEMI_V*>::iterator		m_ite;
	bool								m_bLinkDatas;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_Rpt;
};



/**	@brief	V分類クラス(一覧管理クラス)	 */
class XLIB_API XSEMI_VARIABLE_GRPs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	コンストラクタ */
	XSEMI_VARIABLE_GRPs();
	/**	@brief	デストラクタ */
	virtual ~XSEMI_VARIABLE_GRPs();


	/**	@brief	XSEMI_VARIABLE_GRPの一覧を解放します。 */
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
	XSEMI_VARIABLE_GRP* get(XD_UINT4 index);
	/**	@brief	指定したXSEMI_VARIABLE_GRPのアイテムを取得します。
	 *
	 *	@param	id	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_VARIABLE_GRP* get_byID(XD_UINT4 id);
	/**	@brief	一覧の先頭アイテムを取得する。
	 *
	 *	@return	アイテム。見つからない場合はNULL。
	 */
	XSEMI_VARIABLE_GRP* getFirstItem();
	/**	@brief	現在の一覧のカレント位置の次のアイテムを取得する。
	 *
	 *	@return	アイテム。最後に達した場合は、NULLを返す。
	 */
	XSEMI_VARIABLE_GRP* getNextItem();
	/**	@brief	現在の一覧のカレント位置の前のアイテムを取得する。
	 *
	 *	@return	アイテム。先頭に達した場合は、NULLを返す。
	 */
	XSEMI_VARIABLE_GRP* getPrevItem();
	/**	@brief	アイテムを追加します。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_VARIABLE_GRP* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	アイテムを削除します。
	 *
	 *	@param	pCEID	アイテムを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool del(XSEMI_VARIABLE_GRP* pGrp);
protected:
	std::vector<XSEMI_VARIABLE_GRP*>				m_vgrps;
	std::vector<XSEMI_VARIABLE_GRP*>::iterator		m_ite;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};

#include "xlib/xlib_tail.h"
#endif /* __XSEMI_V_H__ */
