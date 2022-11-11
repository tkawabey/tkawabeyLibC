#ifndef __XSECS_GEM_CONST_H__
#define __XSECS_GEM_CONST_H__


#include "xlib/xlib_head.h"
#include "xlib/xeception.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/semi/gem/xsemi_ceid.h"
#include "xlib/semi/gem/xsemi_rpt.h"
#include "xlib/semi/gem/xsemi_v.h"
#include "xlib/semi/gem/xsemi_alarm.h"
#include "xlib/semi/gem/xsemi_ackcode.h"
#include "xlib/semi/gem/xsemi_spool.h"
#include <vector>

class XLIB_API XSEMI_GemMstData;
class XLIB_API XSEMI_Gem;


/**	@brief	GEM構造【construction】データ	*/
class XLIB_API XSEMI_GemConst : public XSEMI_GemConstObject
{
public:
	XSEMI_GemConst();
	virtual ~XSEMI_GemConst();

// propery
	void set_MasterData(XSEMI_GemMstData* pMasterData);
	XSEMI_GemMstData* get_MasterData(){ return m_pMasterData; }

	bool initialize(XSEMI_GemMstData* pMasterData);
	void resetDirtyAllItem();
	
	/**	@brief	Vの値をストリームから読み込み
	 *
	 *	@param	stream	読み込むストリーム
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool loadVvalue(std::istream  &stream);
	
	/**	@brief	Vの値をストリームに書き込みます。
	 *
	 *	@param	stream	書き込むストリーム
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 *
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool storeVvalue(std::ostream &stream);

// 設定情報関連
	bool get_ConfigValue(const TCHAR* pszConfName, XRichData &data, XStrBuff* pComment = NULL);
	bool set_ConfigValue(const TCHAR* pszConfName, XRichData &data, const TCHAR* lpszComment = NULL);
	void clear();
// CEID
	XSEMI_CEIDs& get_CEIDs(){ return m_ceid; }
// VARIABLE_GRP
	XSEMI_VARIABLE_GRPs& get_VARIABLE_GRPs(){ return m_variable_grps; }
// RportID
	XSEMI_Rpts& get_Rpts(){ return m_rpts; }
// VID
	XSEMI_Vs& get_Vs(){ return m_vs; }
// Alaram
	XSEMI_ALARMs& get_Alarms(){ return m_alarms; }
//	ACKCODE
	XSEMI_ACKCODEs& get_AckCodes(){ return m_ackcodes; }
//	SPOOL_SF
	XSEMI_SpoolSFs& get_SpoolSFs(){ return m_spoolSFs; }


	/**	@brief	ECの値を保存します。 */
	bool saveECValues();
// リンク・イベント・レポート
	/**	@brief	CEIDとRPTIDのリンクを設定する。
	 *
	 *	@retval	ceid  CEID
	 *	@retval	rptid  リンクするRPTID
	 *	@retval	bf_rptid  指定したRPTIDの後にRPTIDをリンク
	 *
	 *	@retval	true  成功
	 *	@retval	false 失敗
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool   linkEventReport(XD_UINT4 ceid, XD_UINT4 rptid, XD_UINT4 bf_rptid = 0xFFFFFFFF);
	/**	@brief	CEIDとRPTIDのリンクを解除する。
	 *
	 *	@retval	ceid  解除対象のCEID。0xFFFFFFFFを指定すると、すべてのCEIDが解除される。
	 *	@retval	rptid  解除するRPTID。0xFFFFFFFFを指定すると、CEIDにリンクされているすべてのRPTIDを解除する。
	 *
	 *	@retval	true  成功
	 *	@retval	false 失敗
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool unlinkEventReport(XD_UINT4 ceid, XD_UINT4 rptid);
	// ↓消す予定
	XSEMI_Rpt* setRpt(XD_UINT4 ceid, XD_UINT4 rptid, XD_UINT4 preRptID = 0xFFFFFFFF);
	/**	@brief	CEIDに緋も付くReptIDを全て削除する。
	 *
	 *	@retval	true  成功
	 *	@retval	false 失敗
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool deleteAllCEID_RptID();

// 規定レポート
	/**	@brief	RPTIDとVIDのリンクを設定する。
	 *
	 *	@retval	rptid  RPTID
	 *	@retval	vid  リンクするVID
	 *	@retval	bf_vid  指定したVIDの後にVIDをリンク
	 *
	 *	@retval	true  成功
	 *	@retval	false 失敗
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool defineReport(XD_UINT4 rptid, XD_UINT4 vid, XD_UINT4 bf_vid = 0xFFFFFFFF);
	/**	@brief	RPTIDとVIDのリンクを解除する。
	 *
	 *	@retval	rptid  解除対象のRPTID。0xFFFFFFFFを指定すると、すべてのRPTIDが解除される。
	 *	@retval	vid  解除するVID。0xFFFFFFFFを指定すると、RPTIDにリンクされているすべてのVIDを解除する。
	 *
	 *	@retval	true  成功
	 *	@retval	false 失敗
	 *	@exception	XExcept	例外オブジェクト	
	 */
	bool unDefineReport(XD_UINT4 rptid, XD_UINT4 vid);


// Database Update
	void loadFromMaster(XSEMI_GemConstObject* pData);
	void updateMaster(XSEMI_GemConstObject* pData);
	void deleteMaster(XSEMI_GemConstObject* pData);
private:
	bool setRiDataFromString(XRichData* pRdata, const TCHAR* p_str);

protected:
	XSEMI_CEIDs							m_ceid;
	XSEMI_Rpts							m_rpts;
	XSEMI_Vs							m_vs;
	XSEMI_VARIABLE_GRPs					m_variable_grps;
	XSEMI_ALARMs						m_alarms;
	XSEMI_ACKCODEs						m_ackcodes;
	XSEMI_SpoolSFs						m_spoolSFs;

	XSEMI_GemMstData*					m_pMasterData;
friend class XSEMI_Gem;
friend class XSEMI_GemMstData;
};


/**	@brief	GEMの管理マスターデータを操作するクラス	*/
class XLIB_API XSEMI_GemMstData
{
public:
	/**	@brief	コンストラクタ	*/
	XSEMI_GemMstData();
	/**	@brief	デストラクタ	*/
	virtual ~XSEMI_GemMstData();

	/**	@brief	テーブルタイプ	*/
	enum TABLE_TYPE {
		TABLE_TYPE_ALL,					///< [all items]
		TABLE_TYPE_CONFIG,				///< CONFIG
		TABLE_TYPE_CEID,				///< CEID
		TABLE_TYPE_RPTID,				///< RPTID
		TABLE_TYPE_VARIABLE_GRP,		///< VARIABLE_GRP
		TABLE_TYPE_VID,					///< VID
		TABLE_TYPE_VID_EC,				///< EC
		TABLE_TYPE_VID_SV,				///< SV
		TABLE_TYPE_VID_DV,				///< DV
		TABLE_TYPE_VVAL,				///< VVAL
		TABLE_TYPE_CEID_RPTID,			///< CEID_RPTID
		TABLE_TYPE_RPTID_DVID,			///< RPTID_DVID
		TABLE_TYPE_ALARM,				///< ALARM
		TABLE_TYPE_KEYWORD,				///< KEYWORD
		TABLE_TYPE_MODEL,				///< MODEL
		TABLE_TYPE_MODEL_KEYWORD,		///< MODEL_KEYWORD
		TABLE_TYPE_MODEL_TRANSIT,		///< MODEL_TRANSIT
		TABLE_TYPE_MODEL_TRANSIT_VALS,	///< MODEL_TRANSIT_VALS
		TABLE_TYPE_CEID_DVVALS,			///< CEID_DVVALS
		TABLE_TYPE_ACKCODES,			///< ACKCODES
		TABLE_TYPE_SPOOL_SF,			///< SPOOL_SF
	};


	/**	@brief	新規データベースを作成します。
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual bool createNewDatabase() = 0;

	/**	@brief	データをロードします。
	 *
	 *	@param	eventmap	GEMコンスタントオブジェクト
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual bool load(XSEMI_GemConst& eventmap)  = 0;


	/**	@brief	CSVファイルから、データをインポートします。
	 *
	 *	@param	tableType	テーブルタイプ
	 *	@param	p_file	CSVファイルのパス
	 *	@param	bExitHeader	CSVファイルの先頭にヘッダー情報を記載している場合はtrue/ない場合はfalseを指定します。
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual bool importCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader)  = 0;

	/**	@brief	CSVファイルに、データをエクスポートします。
	 *
	 *	@param	tableType	テーブルタイプ
	 *	@param	p_file	CSVファイルのパス
	 *	@param	bExitHeader	CSVファイルの先頭にヘッダー情報を記載している場合はtrue/ない場合はfalseを指定します。
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual bool exportCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader)  = 0;

	/**	@brief	Config値を取得する。
	 *
	 *	@param	pszConfName	Configの名前
	 *	@param	data	取得するバッファ
	 *	@param	pComment	コメントを受け取るためのバッファ。必要ない場合はNULL
	 *
	 *
	 *	@return	true	指定した、Configの名前のデータが見つかった
	 *	@return	false	指定した、Configの名前のデータが見つからなかった。その他エラーはXExceptが発生する。
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual bool get_ConfigValue(const TCHAR* pszConfName, XRichData &data, XStrBuff* pComment = NULL)  = 0;
	/**	@brief	Config値を設定する。
	 *
	 *	@param	pszConfName	Configの名前
	 *	@param	data	設定する値
	 *	@param	lpszComment	コメント
	 *
	 *
	 *	@return	true	成功
	 *	@return	false	失敗
	 *	@exception	XExcept	例外オブジェクト
	 */
	virtual bool set_ConfigValue(const TCHAR* pszConfName, XRichData &data, const TCHAR* lpszComment = NULL)  = 0;






	virtual bool deleteCEID_RptID(XD_UINT4 ceid, XD_UINT4 rptid) = 0;
	virtual bool loadVVAlS(XSEMI_V* pVID) = 0;

	virtual bool reloadObject(XSEMI_GemConstObject* pObject) = 0;
	virtual bool updateObject(XSEMI_GemConstObject* pObject) = 0;
	virtual bool deleteObject(XSEMI_GemConstObject* pObject) = 0;
	virtual bool execute(const TCHAR* p_strSQL, bool throwError = true) = 0;


	virtual void setDefaultValues();

protected:
	XSEMI_GemConst*	m_pConst;
friend class XSEMI_GemConst;
};


#include "xlib/xlib_tail.h"
#endif /* __XSECS_GEM_CONST_H__ */