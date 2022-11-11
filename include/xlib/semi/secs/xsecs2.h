/**
 * @file xsecs2.h
 * @brief 半導体製造SEMI規格　SECS-II
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_SECS2_HEASER_FILE__
#define __X_SECS2_HEASER_FILE__

#include "xlib/xlib_head.h"

#include "xlib/xeception.h"
#include "xlib/xbuff.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata.h"
#include "xlib/xdata_rich.h"
#include "xlib/xmlhelper.h"
#include "xlib/semi/secs/xsecs.h"


class XLIB_API XSecs2Obj;
class XLIB_API XSecs2Items;
class XLIB_API XSecs2Item;
class XLIB_API XSecs2ItemData;
class XLIB_API XSecs2ItemDataL;
class XLIB_API XSecs2ItemDataD;
class XLIB_API XSecs2ItemFormats;
class XLIB_API XSecs2ItemFormat;
class XLIB_API XSecs2ItemValue;
class XLIB_API XSecs2;
class XLIB_API XSecs2Messages;
class XLIB_API XSecs2Message;
class XLIB_API XSecs2MessageCats;
class XLIB_API XSecs2MessageCat;


/**	@brief	class of operate secs data	*/
class XLIB_API XSecs2Obj
{
public:
	enum TYPE {
			SECS2		///<	SECS
		,	ITEMS		///<	Items
		,	ITEM		///<	Item
		,	DATA		///<	DATA
		,	FORMATS		///<	Formats
		,	FORMAT		///<	Format
		,	VALUE		///<	Value

		,	MESSAGES	///<	Messages
		,	MESSAGE		///<	Message
		,	MSG_CATS	///<	MessagesCats
		,	MSG_CAT		///<	MessagesCat
	};
protected:
	/**	@brief	construction */
	XSecs2Obj(TYPE type, XSecs2Obj* pParent);
	/**	@brief	destruction */
	virtual ~XSecs2Obj();
	
	XSecs2Obj* getParentByType(TYPE type);

public:	
	TYPE getType() const;
protected:
	TYPE		m_type;
	XSecs2Obj*	m_pParent;
};




/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2Items : public XSecs2Obj
{
protected:
	XSecs2Items(XSecs2Obj *pParent);
	virtual ~XSecs2Items();
public:
	void clear();
	XSecs2Item* getHeadItem();
	XSecs2Item* getItemByName(XStrBuff &strName);
private:
	XSecs2Item* add(XmlHlpElementRawType* pElem);
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XList					m_items;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2;
friend class XSecs2Item;
};




/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2ItemData : public XSecs2Obj
{
protected:
	XSecs2ItemData(XmlHlpElementRawType* pElem, XSecs2Obj* pParent);
	virtual ~XSecs2ItemData();
public:
	virtual void clear() = 0;
	bool isDistribute() const;
	bool remove();
// children operation
	XSecs2ItemData* nextSibling();


	virtual bool validate(XRichData* pData, bool bThrow = true);
protected:
	virtual bool innerExpandRealMsg(XRichData* pRData);
	virtual bool innerSetRealMessage(XRichData* pSrcRData, XRichData* pDstRData);
	virtual bool loadXML(XmlHlpElementRawType* pElem) = 0;
protected:
	bool					m_bDistribute;
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2ItemFormat;
friend class XSecs2Message;
friend class XSecs2ItemDataL;
};

/**
 *	@brief	XSecs2ItemDataL
 */
class XLIB_API XSecs2ItemDataL : public XSecs2ItemData
{
protected:
	XSecs2ItemDataL(XmlHlpElementRawType* pElem, XSecs2Obj* pParent);
	virtual ~XSecs2ItemDataL();
public:
	virtual void clear();
	XSecs2ItemData* getHeadItem();


	int  getListCnt() const;
	void setListCnt(int val);
	XStrBuff getCaption() const;
	void setCaption(XStrBuff str);
	XSecs2ItemDataD* addDistribute( const TCHAR* pDistName);
	XSecs2ItemDataL* addList();

	XRichData* addReal(XRichData* pRParent);

	virtual bool validate(XRichData* pData, bool bThrow = true);
protected:
	virtual bool loadXML(XmlHlpElementRawType* pElem);
	virtual bool innerRemove(XSecs2ItemData* pItem);

	XList					m_items;

friend class XSecs2Message;
friend class XSecs2ItemData;
friend class XSecs2ItemFormat;
};

/**
 *	@brief	XSecs2ItemDataD
 */
class XLIB_API XSecs2ItemDataD : public XSecs2ItemData
{
protected:
	XSecs2ItemDataD(XmlHlpElementRawType* pElem, XSecs2Obj* pParent);
	virtual ~XSecs2ItemDataD();
public:
	virtual void clear();
	XStrBuff& getDistName();
	XSecs2Item* getDistItem();

	virtual bool validate(XRichData* pData, bool bThrow = true);
protected:
	virtual bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XStrBuff				m_strDistName;
	XSecs2Item*				m_pDistItem;
friend class XSecs2Message;
friend class XSecs2ItemData;
friend class XSecs2ItemDataL;
};
/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2ItemFormats : public XSecs2Obj
{
protected:
	XSecs2ItemFormats(XSecs2Obj* pParent);
	virtual ~XSecs2ItemFormats();
public:
	void clear();
	XSecs2ItemFormat* add(XmlHlpElementRawType* pElem);
	XSecs2ItemFormat* add(XDATA_TYPE type);
	bool remove(XSecs2ItemFormat* pItem);
	XSecs2ItemFormat* getHeadItem();
private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XList					m_items;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2Item;
friend class XSecs2ItemFormat;
};




/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2ItemFormat : public XSecs2Obj
{
protected:
	XSecs2ItemFormat(XDATA_TYPE type, XSecs2ItemFormats* pParent, XmlHlpElementRawType* pElem );
	virtual ~XSecs2ItemFormat();
public:
	void clear();
	XDATA_TYPE getDataType() const;
	bool isEqulDataType( XDATA_TYPE type );
	bool isSizeMin();
	bool isSizeMax();
	int  getSizeMin();
	int  getSizeMax();
	void setSizeMin(int val);
	void setSizeMax(int val);

	XRichData* getMaxData();
	XRichData* getMinData();


	bool remove();
	XSecs2ItemDataL*	getListItem() const;
	XSecs2ItemData*	getListHeadItem() const;
	XSecs2ItemFormat* nextSibling();
	


	bool removeValue(XSecs2ItemValue* pItem);
	XSecs2ItemValue*	getValueHeadItem() const;
	XSecs2ItemValue* addValue(XRichData* pRData);

private:
	bool loadXML(XmlHlpElementRawType* pElem);
	void innerSetVal(XStrBuff& str, XRichData* pData);
	XSecs2ItemValue* addValue(XmlHlpElementRawType* pElem);
protected:
	XDATA_TYPE				m_type;
	XRichData*				m_pMaxVal;
	XRichData*				m_pMinVal;
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
	XSecs2ItemDataL*		m_pListItem;
	XList					m_items;		// value
friend class XSecs2Items;
friend class XSecs2ItemFormats;
friend class XSecs2ItemData;
friend class XSecs2ItemValue;
};







/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2ItemValue : public XSecs2Obj, public XRichData
{
private:
	XSecs2ItemValue(XDATA_TYPE type, XSecs2ItemFormat* pParent, XmlHlpElementRawType* pElem );
	virtual ~XSecs2ItemValue();
public:
	XStrBuff getComment();
	void setComment(XStrBuff val);
	bool remove();
	XSecs2ItemValue* nextSibling();
	

private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2ItemFormat;
};


/**
 *	@brief	SECS2設定情報-メッセージカテゴリ一覧クラス
 */
class XLIB_API XSecs2MessageCats : public XSecs2Obj
{
protected:
	XSecs2MessageCats(XSecs2Obj *pParent);
	virtual ~XSecs2MessageCats();
public:
	/**	@brief	一覧情報を空にします。 */
	void clear();
	/**	@brief	SECS2設定情報-メッセージカテゴリを新規に追加します。
	 *
	 *	@param	strName	名前
	 *
	 *	@return	追加されたアイテム、失敗した場合はNULL。
	 *
	 *	@exception	XExcept	例外オブジェクト
	 */
	XSecs2MessageCat* add(XStrBuff strName);
	/**	@brief	SECS2設定情報-メッセージカテゴリを削除します。
	 *
	 *	@param	pItem	削除するアイテム
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool remove(XSecs2MessageCat* pItem);

	/**	@brief	先頭のアイテムを取得します。
	 *	@return	先頭のアイテム。空の場合は、NULL。
	 */
	XSecs2MessageCat* getHeadItem();
private:
	XSecs2MessageCat* add(XmlHlpElementRawType* pElem);
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XmlHlpElementRawType*	m_pElem;
	XList					m_items;
friend class XSecs2;
friend class XSecs2Messages;
friend class XSecs2MessageCat;
};




/**
 *	@brief	SECS2設定情報-メッセージカテゴリークラス
 */
class XLIB_API XSecs2MessageCat : public XSecs2Obj
{
private:
	XSecs2MessageCat(XSecs2MessageCats* pParent, XmlHlpElementRawType* pElem);
	virtual ~XSecs2MessageCat();
public:
	/**	@brief	説明文字を取得します。
	 *	@return	説明文字
	 */
	XStrBuff getName();
	/**	@brief	説明文字を設定します。
	 *	@param	str	設定値
	 *	@exception	XExcept	例外オブジェクト
	 */
	void setName(XStrBuff str);


	/**	@brief	次のインスタンスを返します。
	 *	@return	次のインスタンス。最後のインスタンスの場合は、NULL
	 */
	XSecs2MessageCat* nextSibling();


	/**	@brief	自インスタンスを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool remove();
private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
	XStrBuff				m_strName;
friend class XSecs2MessageCats;
};

/**
 *	@brief	SECS2設定情報-メッセージ一覧クラス
 */
class XLIB_API XSecs2Messages : public XSecs2Obj
{
protected:
	XSecs2Messages(XSecs2Obj *pParent);
	virtual ~XSecs2Messages();
public:
	/**	@brief	一覧情報を空にします。 */
	void clear();
	/**	@brief	SECS2設定情報-メッセージを新規に追加します。
	 *
	 *	@param	stream	ストリーム
	 *	@param	function	ファンクション
	 *
	 *	@return	追加されたアイテム、失敗した場合はNULL。
	 *
	 *	@exception	XExcept	例外オブジェクト
	 */
	XSecs2Message* add(XD_UINT1 stream, XD_UINT1 function);
	/**	@brief	SECS2設定情報-メッセージを削除します。
	 *
	 *	@param	pItem	削除するアイテム
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool remove(XSecs2Message* pItem);

	/**	@brief	先頭のアイテムを取得します。
	 *	@return	先頭のアイテム。空の場合は、NULL。
	 */
	XSecs2Message* getHeadItem();

	/**	@brief	SECS2設定情報-メッセージカテゴリ一覧クラスインスタンスを取得
	 *
	 *	@return	SECS2設定情報-メッセージカテゴリ一覧クラスインスタンス
	 */
	XSecs2MessageCats& getCategories();


	/**	@brief	The setRealMessage function
	 *
	 *	@param	pSECSData	a parameter of type XSecsData*
	 *	@param	bThrow	a parameter of type bool
	 *
	 *	@return	XSecs2Message*
	 *
	 *	
	 *	@exception	XExcept	例外オブジェクト
	 */
	XSecs2Message* setRealMessage(XSecsData* pSECSData, bool bThrow = true);
private:
	XSecs2Message* add(XmlHlpElementRawType* pElem);
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XmlHlpElementRawType*	m_pElem;
	XList					m_items;
	XSecs2MessageCats		m_categories;
friend class XSecs2;
friend class XSecs2Message;
};



/**
 *	@brief	SECS2設定情報-メッセージクラス
 */
class XLIB_API XSecs2Message : public XSecs2Obj
{
private:
	XSecs2Message(XSecs2Messages* pParent, XmlHlpElementRawType* pElem);
	virtual ~XSecs2Message();
public:
	enum DIRECTION {
			BOTH		///< 両方
		,	EtoH		///< ホスト→装置
		,	HtoE		///< 装置→ホスト
		,	INVALID		///< 不明
	};
	enum MLT_BLK {
			MLT_NO		///< マルチブロックメッセージ不可
		,	MLT_YES		///< マルチブロックメッセージ可能
		,	MLT_INQ		///< マルチブロックメッセージ確認メッセージ
		,	MLT_GRT		///< マルチブロックメッセージ受け入れ許可メッセージ
	};


	/**	@brief	ストリーム番号を取得します。
	 *	@return	ストリーム番号
	 */
	XD_UINT1 getStream() const;
	/**	@brief	ファンクション番号を取得します。
	 *	@return	ファンクション番号
	 */
	XD_UINT1 getFunction() const;
	/**	@brief	説明文字を取得します。
	 *	@return	説明文字
	 */
	XStrBuff getAction();
	/**	@brief	説明文字を設定します。
	 *	@param	str	設定値
	 *	@exception	XExcept	例外オブジェクト
	 */
	void setAction(XStrBuff str);

	/**	@brief	通信の向きの情報を取得します。
	 *	@return	通信の向きの情報
	 */
	DIRECTION getDirection() const;
	/**	@brief	通信の向きの情報を設定します。
	 *	@param	val	設定値
	 *	@exception	XExcept	例外オブジェクト
	 */
	void setDirection(DIRECTION val);

	/**	@brief	マルチブロックの設定情報を取得します。
	 *	@return	マルチブロックの設定情報
	 */
	MLT_BLK getMltCont() const;
	/**	@brief	マルチブロックの設定情報を設定します。
	 *	@param	val	設定値
	 *	@exception	XExcept	例外オブジェクト
	 */
	void setMltCont(MLT_BLK val);


	/**	@brief	リプライの設定情報を取得します。
	 *	@return	リプライの設定情報
	 */
	bool getReply() const;
	/**	@brief	リプライの設定情報を設定します。
	 *	@param	val	設定値
	 *	@exception	XExcept	例外オブジェクト
	 */
	void setReply(bool val);

	/**	@brief	ボディー情報を取得します。
	 *	@return	ボディー情報
	 */
	XSecs2ItemData* getBody();

	/**	@brief	ボディデータにリストアイテムを追加
	 *
	 *	@return	アイテム
	 */
	XSecs2ItemDataL* addList() ;
	/**	@brief	ボディデータにアイテムを追加
	 *	@param	str	アイテム名
	 *	@return	アイテム
	 *	@exception	XExcept	例外オブジェクト
	 */
	XSecs2ItemDataD* addDistribute(XStrBuff str) ;

	/**	@brief	ボディーを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *	@exception	XExcept	例外オブジェクト
	 */
	bool removeBody() ;


	/**	@brief	次のインスタンスを返します。
	 *	@return	次のインスタンス。最後のインスタンスの場合は、NULL
	 */
	XSecs2Message* nextSibling();


	/**	@brief	自インスタンスを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool remove();


	/**	@brief	カテゴリーを取得
	 *
	 *	@return	カテゴリー。指定されていない場合は、NULL
	 *
	 *	@exception	XExcept	例外オブジェクト
	 */
	XSecs2MessageCat* getCategory();

	/**	@brief	カテゴリーをセットする
	 *
	 *	@param	val	カテゴリー名
	 *
	 *	@exception	XExcept	例外オブジェクト
	 */
	void setCategory(XSecs2MessageCat* val);


	XRichData* newRealMessage();
	XRichData* getRealMessage();
	/**	@brief	The setRealMessage function
	 *
	 *	@param	pSECSData	a parameter of type XSecsData*
	 *	@param	bThrow	a parameter of type bool
	 *
	 *	@retval	-1	Streamが一致しない
	 *	@retval	-2	Functionが一致しない
	 *
	 *	
	 *	@exception	XExcept	例外オブジェクト
	 */
	int setRealMessage(XSecsData* pSECSData, bool bThrow = true);
	bool validateRealMessage(bool bThrow = true);

private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
	XD_UINT2				m_sf;
	XSecs2ItemData*			m_pBody;
	XSecs2MessageCat*		m_pCategory;
	XStrBuff				m_strCategoryName;
	XSecsData				m_msg;
friend class XSecs2Messages;
};




/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2Item : public XSecs2Obj
{
private:
	XSecs2Item(const TCHAR* pName, XDATA_TYPE type, XSecs2Items* pParent, XmlHlpElementRawType* pElem);
	virtual ~XSecs2Item();
public:
	
	XStrBuff& getName(){  return m_strName; }
	XDATA_TYPE getDataType(){ return m_type; }
	XStrBuff getComment();
	void setComment(XStrBuff val);

	bool getIsFixVals() const;
	void setIsFixVals(bool val);


	bool getIsMultiFmt() const;
	void setIsMultiFmt(bool val);


	XSecs2ItemFormat* getCurrentFormat();
	XSecs2ItemFormats& getFormats();

	XSecs2Item* nextSibling();



private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XStrBuff				m_strName;
	XDATA_TYPE				m_type;
	XPOSITION				m_pos;
	XSecs2ItemFormats		m_formats;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2Items;
};







/**	@brief	SECS-II定義クラス	*/
class XLIB_API XSecs2 : public XSecs2Obj
{
public:
	/**	@brief	construction */
	XSecs2();

	/**	@brief	destruction */
	virtual ~XSecs2();

	/**	@brief	情報を空にします。 */
	virtual void clear();

	/**	@brief	XMLファイルから情報をロードする。
	 *
	 *	@param	p_file_path	XMLファイルのパス
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual bool loadXML(const TCHAR* p_file_path);
	/**	@brief	XMLファイルに保存
	 *
	 *	@param	p_file_path	XMLファイルのパス
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual bool saveXML(const TCHAR* p_file_path);

	/**	@brief	アイテム一覧オブジェクトを取得
	 *	@return	アイテム一覧オブジェクト
	 */
	XSecs2Items& getItems();
	/**	@brief	メッセージ一覧オブジェクトを取得
	 *	@return	メッセージ一覧オブジェクト
	 */
	XSecs2Messages& getMessages();
protected:
	XSecs2Items						m_items;
	XSecs2Messages					m_messages;
	XmlHlpDocumentActl				m_xmlDoc;
};


#include "xlib/xlib_tail.h"
#endif /* __X_SECS2_HEASER_FILE__ */
