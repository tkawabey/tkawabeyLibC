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


/**	@brief	GEM�\���yconstruction�z�f�[�^	*/
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
	
	/**	@brief	V�̒l���X�g���[������ǂݍ���
	 *
	 *	@param	stream	�ǂݍ��ރX�g���[��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool loadVvalue(std::istream  &stream);
	
	/**	@brief	V�̒l���X�g���[���ɏ������݂܂��B
	 *
	 *	@param	stream	�������ރX�g���[��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool storeVvalue(std::ostream &stream);

// �ݒ���֘A
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


	/**	@brief	EC�̒l��ۑ����܂��B */
	bool saveECValues();
// �����N�E�C�x���g�E���|�[�g
	/**	@brief	CEID��RPTID�̃����N��ݒ肷��B
	 *
	 *	@retval	ceid  CEID
	 *	@retval	rptid  �����N����RPTID
	 *	@retval	bf_rptid  �w�肵��RPTID�̌��RPTID�������N
	 *
	 *	@retval	true  ����
	 *	@retval	false ���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool   linkEventReport(XD_UINT4 ceid, XD_UINT4 rptid, XD_UINT4 bf_rptid = 0xFFFFFFFF);
	/**	@brief	CEID��RPTID�̃����N����������B
	 *
	 *	@retval	ceid  �����Ώۂ�CEID�B0xFFFFFFFF���w�肷��ƁA���ׂĂ�CEID�����������B
	 *	@retval	rptid  ��������RPTID�B0xFFFFFFFF���w�肷��ƁACEID�Ƀ����N����Ă��邷�ׂĂ�RPTID����������B
	 *
	 *	@retval	true  ����
	 *	@retval	false ���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool unlinkEventReport(XD_UINT4 ceid, XD_UINT4 rptid);
	// �������\��
	XSEMI_Rpt* setRpt(XD_UINT4 ceid, XD_UINT4 rptid, XD_UINT4 preRptID = 0xFFFFFFFF);
	/**	@brief	CEID�ɔ���t��ReptID��S�č폜����B
	 *
	 *	@retval	true  ����
	 *	@retval	false ���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool deleteAllCEID_RptID();

// �K�背�|�[�g
	/**	@brief	RPTID��VID�̃����N��ݒ肷��B
	 *
	 *	@retval	rptid  RPTID
	 *	@retval	vid  �����N����VID
	 *	@retval	bf_vid  �w�肵��VID�̌��VID�������N
	 *
	 *	@retval	true  ����
	 *	@retval	false ���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool defineReport(XD_UINT4 rptid, XD_UINT4 vid, XD_UINT4 bf_vid = 0xFFFFFFFF);
	/**	@brief	RPTID��VID�̃����N����������B
	 *
	 *	@retval	rptid  �����Ώۂ�RPTID�B0xFFFFFFFF���w�肷��ƁA���ׂĂ�RPTID�����������B
	 *	@retval	vid  ��������VID�B0xFFFFFFFF���w�肷��ƁARPTID�Ƀ����N����Ă��邷�ׂĂ�VID����������B
	 *
	 *	@retval	true  ����
	 *	@retval	false ���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
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


/**	@brief	GEM�̊Ǘ��}�X�^�[�f�[�^�𑀍삷��N���X	*/
class XLIB_API XSEMI_GemMstData
{
public:
	/**	@brief	�R���X�g���N�^	*/
	XSEMI_GemMstData();
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSEMI_GemMstData();

	/**	@brief	�e�[�u���^�C�v	*/
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


	/**	@brief	�V�K�f�[�^�x�[�X���쐬���܂��B
	 *
	 *	@return	true	����
	 *	@return	false	���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	virtual bool createNewDatabase() = 0;

	/**	@brief	�f�[�^�����[�h���܂��B
	 *
	 *	@param	eventmap	GEM�R���X�^���g�I�u�W�F�N�g
	 *
	 *	@return	true	����
	 *	@return	false	���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	virtual bool load(XSEMI_GemConst& eventmap)  = 0;


	/**	@brief	CSV�t�@�C������A�f�[�^���C���|�[�g���܂��B
	 *
	 *	@param	tableType	�e�[�u���^�C�v
	 *	@param	p_file	CSV�t�@�C���̃p�X
	 *	@param	bExitHeader	CSV�t�@�C���̐擪�Ƀw�b�_�[�����L�ڂ��Ă���ꍇ��true/�Ȃ��ꍇ��false���w�肵�܂��B
	 *
	 *	@return	true	����
	 *	@return	false	���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	virtual bool importCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader)  = 0;

	/**	@brief	CSV�t�@�C���ɁA�f�[�^���G�N�X�|�[�g���܂��B
	 *
	 *	@param	tableType	�e�[�u���^�C�v
	 *	@param	p_file	CSV�t�@�C���̃p�X
	 *	@param	bExitHeader	CSV�t�@�C���̐擪�Ƀw�b�_�[�����L�ڂ��Ă���ꍇ��true/�Ȃ��ꍇ��false���w�肵�܂��B
	 *
	 *	@return	true	����
	 *	@return	false	���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	virtual bool exportCSV(XSEMI_GemMstData::TABLE_TYPE tableType, const TCHAR* p_file, bool bExitHeader)  = 0;

	/**	@brief	Config�l���擾����B
	 *
	 *	@param	pszConfName	Config�̖��O
	 *	@param	data	�擾����o�b�t�@
	 *	@param	pComment	�R�����g���󂯎�邽�߂̃o�b�t�@�B�K�v�Ȃ��ꍇ��NULL
	 *
	 *
	 *	@return	true	�w�肵���AConfig�̖��O�̃f�[�^����������
	 *	@return	false	�w�肵���AConfig�̖��O�̃f�[�^��������Ȃ������B���̑��G���[��XExcept����������B
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	virtual bool get_ConfigValue(const TCHAR* pszConfName, XRichData &data, XStrBuff* pComment = NULL)  = 0;
	/**	@brief	Config�l��ݒ肷��B
	 *
	 *	@param	pszConfName	Config�̖��O
	 *	@param	data	�ݒ肷��l
	 *	@param	lpszComment	�R�����g
	 *
	 *
	 *	@return	true	����
	 *	@return	false	���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g
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