/**
 * @file xdata_rich.h
 * @brief バイナリーデータアクセスクラス郡
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DATA_RICH_HEASER_FILE__
#define __X_DATA_RICH_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xeception.h"
#include <vector>


class XLIB_API XRichDataBase;
class XLIB_API XRichData;
class XLIB_API XRichDataXL;



DLL_EXTERN template class XLIB_API std::vector<XRichDataBase*>;
typedef XLIB_API std::vector<XRichDataBase*> XRichDataBases;

DLL_EXTERN template class XLIB_API std::vector<XRichData*>;
typedef XLIB_API std::vector<XRichData*> XRichDatas;

DLL_EXTERN template class XLIB_API std::vector<XRichDataXL*>;
typedef XLIB_API std::vector<XRichDataXL*> XRichDataXLs;


/**
 * @brief データアクセスクラス。追加、削除、編集が可能。
 *         XDataReaderやXDataWriterに比べて、メモリーアロケートが多発するので、パフォーマンスは劣る。
 */
class XLIB_API XRichDataBase : public XDataCoder 
{
public:
	/**	@brief	コンストラクタ
	 *	@param	format	データフォーマット
	 */
	XRichDataBase(XRichDataBase* pParent);
public:	
	/**	@brief	デストラクタ	*/
	virtual ~XRichDataBase();
	
	/**	@brief	データタイプを取得します
	 *	@return	データタイプ
	 */
	virtual XDATA_TYPE getType() const;
	/**	@brief	データタイプを文字列で取得します
	 *	@return	データタイプの文字列
	 */
	virtual const char* getTypeStringA() const;

	/**	@brief	指定した子リストアイテムを削除します。
	 *	@param	pItem	削除する子リストアイテム
	 *	@return	true:成功/false:失敗
	 */
	virtual bool removeChiled(XRichDataBase* pItem);

	/**	@brief	子リストにアイテムをセットします。
	 *
	 *	@param	pItem	セットするアイテム
	 *	@param	pPreItem	このアイテムで指定した位置の後に追加します。最後にアイテムを追加する場合は、NULLを指定します。
	 *
	 *	@return	true:成功/false:失敗
	 */
	virtual bool atachChild(XRichDataBase* pItem, XRichDataBase* pPreItem = NULL );
	/**	@brief	子リストからアイテムを解除します。子アイテムのメモリは削除しません。
	 *
	 *	@param	pItem	リストから解除するアイテム
	 *
	 *	@return	true:成功/false:失敗	
	 */
	virtual bool detachChild(XRichDataBase* pItem);

	/**	@brief	指定した子リストアイテム数を取得します。
	 *	@return	子リストアイテム数。
	 */
	virtual XD_UINT4 getChiledCount() const;

	/**	@brief	親アイテムのインスタンスアドレスを返します。
	 *	@return	親アイテムのインスタンスアドレス。親が無い場合はNULL。
	 */
	virtual XRichDataBase* getParent() const;

	/**	@brief	親アイテムから見た、このインスタンスの配列の位置を返します。
	 *
	 *	@return	親アイテムから見た、このインスタンスの配列の位置。親が無い場合は-1。
	 */
	virtual int getIndexFromParent() const;

	virtual void freeValue();

	virtual void clear();

	XStrBuff toString();

	virtual int compare(XRichDataBase* pDst);

	virtual bool isDirty() const;
	virtual void resetDirty();

	/**	@brief	レングスバイトを取得します。
	 *
	 *	@retval	-1	データサイズに一致させることを意味します。
	 *	@retval	1	1バイト
	 *	@retval	2	2バイト
	 *	@retval	3	3バイト
	 */
	virtual XD_INT1 getLengthByte();
	/**	@brief	レングスバイトを設定します。
	 *
	 *	@param	val	レングスバイト。指定できる値は、-1,1,2,3のいずれか
	 *
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual void    setLengthByte(XD_INT1 val) ;

	/**	@brief	ユーザーデータを取得します。
	 *
	 *	@return	ユーザーデータ
	 */
	virtual void*   getUserData() const;
	/**	@brief	ユーザーデータを設定します。
	 *
	 *	@param	val	ユーザーデータ
	 */
	virtual void    setUserData(void* val);
	
	virtual XD_UINT4 getDataCount() const;
	virtual XD_UINT4 getBinarySize() const;
	virtual bool assign(XD_UINT4 nValueCount) ;
	virtual bool putValueString(const char* pValue, int len = -1);
	virtual bool putValueString(const wchar_t* pValue, int len = -1);
	virtual bool putValueStringUTF16(const wchar_t* pValue, int len = -1);
	virtual bool putValue(const bool* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_INT1* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_UINT1* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_INT2* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_UINT2* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_INT4* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_UINT4* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_FLOAT4* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_INT8* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_UINT8* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_FLOAT8* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const bool value);
	virtual bool putValue(const XD_INT1 value);
	virtual bool putValue(const XD_UINT1 value);
	virtual bool putValue(const XD_INT2 value);
	virtual bool putValue(const XD_UINT2 value);
	virtual bool putValue(const XD_INT4 value);
	virtual bool putValue(const XD_UINT4 value);
	virtual bool putValue(const XD_FLOAT4 value);
	virtual bool putValue(const XD_INT8 value);
	virtual bool putValue(const XD_UINT8 value);
	virtual bool putValue(const XD_FLOAT8 value);


	virtual bool setValue(XD_UINT4 index, const bool value) ;
	virtual bool setValue(XD_UINT4 index, const XD_INT1 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_UINT1 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_INT2 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_UINT2 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_INT4 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_UINT4 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_FLOAT4 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_INT8 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_UINT8 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_FLOAT8 value) ;



	virtual void getValue(std::ostream *stream) const;
	virtual void getValue(XD_INT1 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_UINT1 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_INT2 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_UINT2 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_INT4 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_UINT4 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_FLOAT4 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_INT8 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_UINT8 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_FLOAT8 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValueBool(bool **pValue, XD_UINT4 &nValueCount) const;
	virtual const char* getValueString(int *err = NULL) const;
	virtual const unsigned char getValueBinary(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const bool getValueBoolean(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_INT1 getValueInt1(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_UINT1 getValueUInt1(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_INT2 getValueInt2(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_UINT2 getValueUInt2(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_INT4 getValueInt4(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_UINT4 getValueUInt4(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_FLOAT4 getValueFloat4(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_INT8 getValueInt8(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_UINT8 getValueUInt8(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_FLOAT8 getValueFloat8(XD_UINT4 index = 0, int *err = NULL) const;

	/**	@brief	データタイプを変更する。
	 *
	 *	@param	type	データタイプ
	 *
	 *	@return	true:成功/false:失敗
	 *
	 *	@exception	XExcept	例外オブジェクト	
	 */
	virtual bool changeType(XDATA_TYPE type) ;


	virtual bool buildBinaryData(std::ostream *stream) = 0;
	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream,  XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n")) = 0;
protected:
	virtual bool innerPutValue(const unsigned char* pValue, XD_UINT4 nValueSize, bool bHostOrder = true);
protected:
	XDATA_TYPE						m_type;
	XBIN*							m_pData;
	XD_UINT4						m_nDataSize;
	XRichDataBase*					m_pParent;
	std::vector<XRichDataBase*>		m_children;
	bool							m_bDirty;
	XD_INT1							m_nLengthByte;
	void*							m_pUserData;
};


/**
 * @brief	データアクセスクラス。追加、削除、編集が可能。
 *			XDataReaderやXDataWriterに比べて、メモリーアロケートが多発するので、パフォーマンスは劣る。
 */
class XLIB_API XRichData : public XRichDataBase
{
public:
	/**	@brief	コンストラクタ
	 *	@param	format	データフォーマット
	 */
	XRichData(XDATA_TYPE type);
	XRichData(XRichData &srcdata);
	
	/**	@brief	デストラクタ	*/
	virtual ~XRichData();

	/**	@brief	XDataから、値をセットする。
	 *
	 *	@param	reader	リーダーオブジェクト
	 *	@param	value	XDataオブジェクト
	 *	@param	bConvert	XDataの型と、現在の型が違う場合は、変換するかどうかのフラグ。
	 *						true  : 変換する。
	 *						false : 変換しない。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗	
	 */
	virtual bool putValueFromXData(XDataReader &reader, XData& value, bool bConvert = false);

	/**	@brief	リストデータの子データを追加します。
	 *	@param	type	追加するデータタイプ
	 *	@param	pPrePos	指定した子データアイテムの前に追加します。リストの最後に追加する場合はNULLを指定します。
	 *	@return	新規作成されたデータアイテム。新規作成に失敗した場合はNULL。
	 */
	virtual XRichData* addChiled(XDATA_TYPE type, XRichData* pPrePos = NULL);
	
	/**	@brief	指定したインデックスの子アイテムを取得します。
	 *	@param	index	取得する子アイテムのリストのインデックス位置。
	 *	@return	子アイテム。インデックスが不正ば場合はNULLを返します。、
	 */
	virtual XRichData* getChiled(XD_UINT4 index);

	/**	@brief	指定したデータをコピーします。
	 *
	 *	@param	pSrcData	コピー元のデータ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool copy(XRichData* pSrcData);

	/**	@brief	このインスタンスの複製を作成します。
	 *	@return	作成されたインスタンス
	 */
	virtual XRichData* clone();



	XRichData* find(const TCHAR* lpszFindpath, ...);

	virtual bool buildBinaryData(std::ostream *stream);

	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream,  XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));


	 bool operator==(XRichData& pDst);
	 bool operator!=(XRichData& pDst);
	 const XRichData& operator=(XRichData& pSrcData);
protected:
	/*	@brief	コンストラクタ
	 *	@param	pParentt	親データ。データリストタイプ。
	 */
	XRichData(XRichData* pParentt);
	virtual bool clone(XRichData *pTarget);
	virtual XRichData* find(const TCHAR* lpszFindpath, va_list argList);
	virtual bool buildBinaryData(XDataWriter &writer);
	virtual bool innerPutValueFromXData(XDataReader &reader, XData& value, bool bConvert);
private:
//	XRichDatas			m_list;
};




/**
 * @brief データアクセスクラス。追加、削除、編集が可能。
 *         XDataReaderやXDataWriterに比べて、メモリーアロケートが多発するので、パフォーマンスは劣る。
 */
class XLIB_API XRichDataXL : public XRichDataBase
{
public:
	XRichDataXL();
	virtual ~XRichDataXL();

	/**	@brief	タグ名を取得します。
	 *	@return	タグ名
	 */
	virtual const TCHAR* getTagName();
	/**	@brief	タグ名をセットします。
	 *	@param	pTagName	タグ名
	 */
	virtual void setTagName(const TCHAR* pTagName);
	/**	@brief	リストデータの子データを追加します。
	 *	@param	type	追加するデータタイプ
	 *	@param	pTagName	タグ名
	 *	@param	pPrePos	指定した子データアイテムの前に追加します。リストの最後に追加する場合はNULLを指定します。
	 *	@return	新規作成されたデータアイテム。新規作成に失敗した場合はNULL。
	 */
	virtual XRichDataXL* addChiled(XDATA_TYPE type, const TCHAR* pTagName, XRichDataXL* pPrePos = NULL);

	/**	@brief	指定したインデックスの子アイテムを取得します。
	 *	@param	index	取得する子アイテムのリストのインデックス位置。
	 *	@return	子アイテム。インデックスが不正ば場合はNULLを返します。、
	 */
	virtual XRichDataXL* getChiled(XD_UINT4 index);

	virtual bool copy(XRichDataXL* pSrcData);
	virtual XRichDataXL* clone();


	virtual bool buildBinaryData(std::ostream *stream);

	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream,  XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));

protected:
	/*	@brief	コンストラクタ
	 *	@param	pParentt	親データ。データリストタイプ。
	 */
	XRichDataXL(XRichDataXL* pParentt);
	virtual bool clone(XRichDataXL *pTarget);
	virtual bool buildBinaryData(XDataXLWriter &writer);
private:
	XRichDataXLs					m_list;
	XStrBuff						m_strTagName;
};


#include "xlib/xlib_tail.h"
#endif /* __X_DATA_RICH_HEASER_FILE__ */
