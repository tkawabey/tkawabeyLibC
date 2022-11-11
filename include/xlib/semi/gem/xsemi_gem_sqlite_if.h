/**
 * @file xsemi_gem_sqlite_if.h
 * @brief 半導体製造SEMI規格　収集イベント(CEID)管理クラス
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_GEM_SQLITE_IF_H__
#define __XSEMI_GEM_SQLITE_IF_H__

#include "xlib/xlib_head.h"
#include "xlib/xeception.h"
#include "xlib/xstrbuff.h"
#include "xlib/semi/gem/xsemi_gem_const.h"



class XLIB_API XSEMI_GemSQLiteIF : public XSEMI_GemMstData
{
public:
	XSEMI_GemSQLiteIF(const TCHAR* p_file_path);
	virtual ~XSEMI_GemSQLiteIF();

	const TCHAR* get_DBFilePath();



	virtual bool createNewDatabase();
	virtual bool load(XSEMI_GemConst& eventmap) ;

	/** @copydoc XSEMI_GemMstData::get_ConfigValue(const TCHAR*, XRichData&, XStrBuff*) */
	bool get_ConfigValue(const TCHAR* pszConfName, XRichData &data, XStrBuff* pComment = NULL) ;
	/** @copydoc XSEMI_GemMstData::set_ConfigValue(const TCHAR*, const XRichData&, const TCHAR*) */
	bool set_ConfigValue(const TCHAR* pszConfName, XRichData &data, const TCHAR* lpszComment = NULL) ;


	/** @copydoc XSEMI_GemMstData::importCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader) */
	virtual bool importCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader);
	/** @copydoc XSEMI_GemMstData::exportCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader) */
	virtual bool exportCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader);




	virtual bool deleteCEID_RptID(XD_UINT4 ceid, XD_UINT4 rptid);
	virtual bool loadVVAlS(XSEMI_V* pVID);

	virtual bool reloadObject(XSEMI_GemConstObject* pObject);
	virtual bool updateObject(XSEMI_GemConstObject* pObject);
	virtual bool deleteObject(XSEMI_GemConstObject* pObject);
	virtual bool execute(const TCHAR* p_strSQL, bool throwError = true);


protected:
	virtual bool innerExportCSV(		FILE* fp, const TCHAR* pSQL, const TCHAR* pHeaderFirstClmn = NULL);
	virtual bool innerUpdateCEID(void* pdb, XSEMI_CEID *pCEID);
	virtual bool innerdeleteCEID_RptID(XD_UINT4 ceid, XD_UINT4 rptid);
	virtual bool innerUpdateRpt(void* pdb, XSEMI_Rpt *pV);
	virtual bool innerUpdateV(void* pdb, XSEMI_V *pV);
	virtual bool innerUpdateAlaram(void* pdb, XSEMI_ALARM *pAlarm);
	virtual bool innerUpdateCEID_RPTID(void* pdb, XSEMI_GemDataAtom* pAtmData);
	virtual bool innerUpdateRPTID_DVID(void* pdb, XSEMI_GemDataAtom* pAtmData);
	virtual bool innerUpdateVariableGRP(void* pdb, XSEMI_VARIABLE_GRP *pVG);

	virtual bool innerReloadV(void* pdb, XSEMI_V *pV);

	virtual bool setRiDataFromString(XRichData* pRdata, const TCHAR* p_str);
private:
	void openDB() ;
	void closeDB();

	void setRDataFromString(const TCHAR* p_str, XRichData& val);

	void beginTansction();
	void endTansction();
	void rollBackTansction();
private:
	XStrBuff		m_strFilePath;
	int				m_db;
	int				m_iTansactin;
};

#include "xlib/xlib_tail.h"
#endif /* __XSEMI_GEM_SQLITE_IF_H__ */
