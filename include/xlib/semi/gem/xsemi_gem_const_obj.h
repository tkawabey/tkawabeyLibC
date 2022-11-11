#ifndef __XSECS_GEM_CONST__OBJ_H__
#define __XSECS_GEM_CONST__OBJ_H__

#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"
#include "xlib/xdata_rich.h"

#define XSEMI_DATA_CLMN_ObjectType		_T("ObjectType")
#define XSEMI_DATA_CLMN_Name			_T("Name")
#define XSEMI_DATA_CLMN_Comment			_T("Comment")
#define XSEMI_DATA_CLMN_Enable			_T("Enable")
#define XSEMI_DATA_CLMN_VValue			_T("VValue")
#define XSEMI_DATA_CLMN_Vvalue			_T("Vvalue")

 #define XSEMI_DATA_CLMN_CEID			_T("CEID")
 #define XSEMI_DATA_CLMN_CeID			_T("CeID")
 #define XSEMI_DATA_CLMN_EventName		_T("EventName")

 #define XSEMI_DATA_CLMN_RPTID			_T("RPTID")
 #define XSEMI_DATA_CLMN_RptID			_T("RptID")
 #define XSEMI_DATA_CLMN_RptName		_T("RptName")

 #define XSEMI_DATA_CLMN_VID			_T("VID")
 #define XSEMI_DATA_CLMN_DisplayName	_T("DisplayName")
 #define XSEMI_DATA_CLMN_DataUnit		_T("DataUnit")
 #define XSEMI_DATA_CLMN_DataType		_T("DataType")
 #define XSEMI_DATA_CLMN_Mode			_T("Mode")
 #define XSEMI_DATA_CLMN_InitValue		_T("InitValue")
 #define XSEMI_DATA_CLMN_Category		_T("Category")
 #define XSEMI_DATA_CLMN_VARIABLE_GRP	_T("VARIABLE_GRP")
 #define XSEMI_DATA_CLMN_VPRIVATE		_T("VPRIVATE")
 #define XSEMI_DATA_CLMN_GET_EX			_T("GET_EX")


 #define XSEMI_DATA_CLMN_ECID			_T("ECID")
 #define XSEMI_DATA_CLMN_EcMax			_T("EcMax")
 #define XSEMI_DATA_CLMN_EcMin			_T("EcMin")
 #define XSEMI_DATA_CLMN_EcDef			_T("EcDef")
 #define XSEMI_DATA_CLMN_EcVal			_T("EcVal")
 #define XSEMI_DATA_CLMN_SET_EX			_T("SET_EX")
 #define XSEMI_DATA_CLMN_ECVerify		_T("ECVerify")

 #define XSEMI_DATA_CLMN_SizeMax		_T("SizeMax")
 #define XSEMI_DATA_CLMN_SizeMin		_T("SizeMin")

 #define XSEMI_DATA_CLMN_LimitMax		_T("LimitMax")
 #define XSEMI_DATA_CLMN_LimitMin		_T("LimitMin")
 #define XSEMI_DATA_CLMN_CeLimits		_T("CeLimits")

 #define XSEMI_DATA_CLMN_RptIdSeq		_T("RptIdSeq")
 #define XSEMI_DATA_CLMN_Option			_T("Option")

 #define XSEMI_DATA_CLMN_DvIdSeq		_T("DvIdSeq")
 #define XSEMI_DATA_CLMN_DvID			_T("DvID")

 #define XSEMI_DATA_CLMN_ALID			_T("ALID")
 #define XSEMI_DATA_CLMN_ALCD			_T("ALCD")
 #define XSEMI_DATA_CLMN_AlarmName		_T("AlarmName")
 #define XSEMI_DATA_CLMN_AlarmText		_T("AlarmText")
 #define XSEMI_DATA_CLMN_OnCEID			_T("OnCEID")
 #define XSEMI_DATA_CLMN_OffCEID		_T("OffCEID")

 #define XSEMI_DATA_CLMN_ACKKIND		_T("ACKKIND")
 #define XSEMI_DATA_CLMN_ACKCODE		_T("ACKCODE")

 #define XSEMI_DATA_CLMN_STREAM			_T("STREAM")
 #define XSEMI_DATA_CLMN_FUNCTION		_T("FUNCTION")


 #define XSEMI_DATA_CLMN_GRPID			_T("GRPID")






/**	@brief	CEID,V,ALARMなどのデータのベースクラス	*/
class XLIB_API XSEMI_GemConstObject
{
public:
	enum TYPE
	{
		GEM,				///< Gem
		CONSTRUCTION,		///< Construction
		CEID,				///< CEID
		CEIDs,				///< CEIDs(CEIDの一覧管理)
		RPTID,				///< RPTID
		RPTIDs,				///< RPTIDs(RPTIDの一覧管理)
		VID,				///< VID
		VIDs,				///< VIDs(VIDの一覧管理)
		VVAL,				///< VVAL Vの取りうる値
		ALARM,				///< ALARM
		ALARMs,				///< ALARMs
		VARIABLE_GRP,		///< V分類クラス
		VARIABLE_GRPs,		///< V分類クラス(一覧管理クラス)
		ACKCODE,			///< ACKコード
		ACKCODEs,			///< ACKコード(一覧管理クラス)
		SPOOL_SF,			///< スプールするSxFx
		SPOOL_SFs,			///< スプールするSxFx(一覧管理クラス)


		UNKOWN,				///< UNKOWN
	};

	/**	@brief	コンストラクタ
	 *
	 *	@param	type	Objectタイプ
	 */
	XSEMI_GemConstObject(TYPE type);
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_GemConstObject();

	/**	@brief	Objectタイプを取得します。
	 *
	 *	@return	Objectタイプ
	 */
	TYPE get_ObjectType() const;

	/**	@brief	オブジェクトのメンバーが変更されたかどうか調べます。
	 *
	 *	@retval	true	変更された
	 *	@retval	false	変更されていない
	 */
	virtual bool isDirtyMember();	
	/**	@brief	オブジェクトのメンバーが変更状態をリセットします。	 */
	virtual void resetDirtyMember();
	
	XSEMI_GemConstObject* getParentByType( TYPE type );


protected:
	TYPE					m_type;
	bool					m_bDirtyMember;
	XSEMI_GemConstObject*	m_pParentObject;
};



// スキーマ情報
#define XSEMI_CONST_SCHEMA_RTYPE_NULL		0
#define XSEMI_CONST_SCHEMA_RTYPE_UINT1		1
#define XSEMI_CONST_SCHEMA_RTYPE_UINT2		2
#define XSEMI_CONST_SCHEMA_RTYPE_UINT4		3
#define XSEMI_CONST_SCHEMA_RTYPE_UINT8		4
#define XSEMI_CONST_SCHEMA_RTYPE_INT1		5
#define XSEMI_CONST_SCHEMA_RTYPE_INT2		6
#define XSEMI_CONST_SCHEMA_RTYPE_INT4		7
#define XSEMI_CONST_SCHEMA_RTYPE_INT8		8
#define XSEMI_CONST_SCHEMA_RTYPE_FLOAT4		9
#define XSEMI_CONST_SCHEMA_RTYPE_FLOAT8		10
#define XSEMI_CONST_SCHEMA_RTYPE_BOOL		11
#define XSEMI_CONST_SCHEMA_RTYPE_RDATA		100
#define XSEMI_CONST_SCHEMA_RTYPE_CHAR_PTR	101
#define XSEMI_CONST_SCHEMA_RTYPE_XSTR		102



struct XSEMI_CONST_SCHEMA
{
	TCHAR		*clm_name;	
	XDATA_TYPE	data_type;
	int			real_type;
	bool		bKey;
};

/**	@brief	データアクセスヘルパークラス	*/
class XLIB_API XSEMI_GemDataAtom
{
public:
	XSEMI_GemDataAtom(){}

	virtual const TCHAR* getAtomName() = 0;
	virtual const TCHAR* getColmnName(int index) = 0;
	virtual XDATA_TYPE  getColmnType(int index) = 0;
	virtual bool        isColumnKey(int index)  = 0;
	virtual int getColmnCount() = 0;

	virtual const TCHAR* getString(const TCHAR* pKey) = 0;
	virtual XD_UINT4    getUint(const TCHAR* pKey) = 0;
	virtual XD_INT4     getInt(const TCHAR* pKey) = 0;
	virtual XD_FLOAT8   getDouble(const TCHAR* pKey) = 0;
	virtual bool        getBOOL(const TCHAR* pKey) = 0;
	virtual void        getRData(const TCHAR* pKey, XRichData& data) = 0;

	virtual const TCHAR* getString(int index) = 0;
	virtual XD_UINT4    getUint(int index) = 0;
	virtual XD_INT4     getInt(int index) = 0;
	virtual XD_FLOAT8   getDouble(int index) = 0;
	virtual bool        getBOOL(int index) = 0;
	virtual void        getRData(int index, XRichData& data) = 0;


	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);


	static XD_UINT4 getUint(XSEMI_CONST_SCHEMA* pSchema, int index, void* pVal);
	static const TCHAR* getString(XSEMI_CONST_SCHEMA* pSchema, int index, void* pVal, XStrBuff* pBuff);
	static XD_FLOAT8 getDouble(XSEMI_CONST_SCHEMA* pSchema, int index, void* pVal);

	void convClmnSQLString(int index, XLIB_STD_OUT_STR_STREAM& stream);
	static void convSQLString(XD_UINT4 val, XLIB_STD_OUT_STR_STREAM& stream);
	static void convSQLString(XD_FLOAT8 val, XLIB_STD_OUT_STR_STREAM& stream);
	static void convSQLString(bool val, XLIB_STD_OUT_STR_STREAM& stream);
	static void convSQLString(const char* val, XLIB_STD_OUT_STR_STREAM& stream);
	static void convSQLString(const wchar_t* val, XLIB_STD_OUT_STR_STREAM& stream);
protected:
};


/**	@brief		*/
class XLIB_API XSEMI_GemConstObjectAtom : public XSEMI_GemConstObject, public XSEMI_GemDataAtom
{
public:
	XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::TYPE type);
	virtual ~XSEMI_GemConstObjectAtom();


	virtual const TCHAR* getColmnName(int index);
	virtual XDATA_TYPE  getColmnType(int index);
	virtual bool        isColumnKey(int index);

	virtual const TCHAR* getString(const TCHAR* pKey);
	virtual XD_UINT4    getUint(const TCHAR* pKey);
	virtual XD_INT4     getInt(const TCHAR* pKey);
	virtual XD_FLOAT8   getDouble(const TCHAR* pKey);
	virtual bool        getBOOL(const TCHAR* pKey);
	virtual void        getRData(const TCHAR* pKey, XRichData& data);

	virtual const TCHAR* getString(int index);
	virtual XD_UINT4    getUint(int index);
	virtual XD_INT4     getInt(int index);
	virtual XD_FLOAT8   getDouble(int index);
	virtual bool        getBOOL(int index);
	virtual void        getRData(int index, XRichData& data);
protected:
	virtual int getAtomIndex(const TCHAR* pKey);
	virtual void* getAtomValPtr(int index) = 0;
protected:
	XSEMI_CONST_SCHEMA*	m_pBaseSchema;
};

/**	@brief	CSV形式の文字列データアクセスヘルパー	*/
class XLIB_API XSEMI_GemDataAtomCSV : public XSEMI_GemDataAtom
{
public:
	XSEMI_GemDataAtomCSV(const TCHAR* pKeys, const TCHAR* pAtomName);
	virtual ~XSEMI_GemDataAtomCSV();

	void setValues(const TCHAR* pValues) ;
	virtual const TCHAR* getAtomName();
	virtual const TCHAR* getColmnName(int index);
	virtual XDATA_TYPE  getColmnType(int index);
	virtual bool        isColumnKey(int index){ _XASSERT(0);	return false; }
	virtual int getColmnCount();

	virtual const TCHAR* getString(const TCHAR* pKey);
	virtual XD_UINT4    getUint(const TCHAR* pKey);
	virtual XD_INT4     getInt(const TCHAR* pKey);
	virtual XD_FLOAT8   getDouble(const TCHAR* pKey);
	virtual bool        getBOOL(const TCHAR* pKey);
	virtual void        getRData(const TCHAR* pKey, XRichData& data);

	virtual const TCHAR* getString(int index);
	virtual XD_UINT4    getUint(int index);
	virtual XD_INT4     getInt(int index);
	virtual XD_FLOAT8   getDouble(int index);
	virtual bool        getBOOL(int index);
	virtual void        getRData(int index, XRichData& data);
private:
	int getIndex(const TCHAR* pKey);

	XStrBuffSplit	m_strAtom;
	XStrBuffSplit	m_keys;
	XStrBuffSplit	m_values;
};
#include "xlib/xlib_tail.h"
#endif /* __XSECS_GEM_CONST__OBJ_H__ */
