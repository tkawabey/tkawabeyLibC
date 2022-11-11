/**
 * @file xsemi_v.h
 * @brief �����̐���SEMI�K�i�@�ϐ��f�[�^�iV�j�Ǘ��N���X
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
	/**	@brief	V�̃N���X	*/
	enum Class {
		SV   = 0,	///< SV
		EC,			///< EC
		DV,			///< DV
		ALARM,		///< ALARM
	};
	/**	@brief	Private�l	*/
	enum PRIVAL {
		PRIVAL_NON = 0,		// Private�łȂ�
		PRIVAL_EQP,			// ���u����Private�ł���
		PRIVAL_HOST,		// HOST����Private�ł���
		PRIVAL_BOTH,		// ������Private�ł���
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
// �v���p�e�B
	/**	@brief	�R���X�g���N�^
     *
	 *	@param	vid	ID
	 *	@param	p_name	���O
	 *	@param	type	�f�[�^�^�C�v
	 *	@param	Class	�N���X
	*/
	XSEMI_V(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, Class Class, bool bLoaded = false);
	/**	@brief	�f�X�g���N�^	*/
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
// ���\�b�h
	/**	@brief	���̓f�[�^�̑Ó������`�F�b�N���܂��B
	 *
	 *	@param	data	���̓f�[�^
	 *
	 *	@retval	0	�Ó�
	 *	@retval	-1	�f�[�^�^�C�v���Ⴄ
	 *	@retval	-2	�ŏ��l���I�[�o�[���Ă���
	 *	@retval	-3	�ő�l���I�[�o�[���Ă���
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
	/**	@brief	�I�u�W�F�N�g�̃����o�[���ύX���ꂽ���ǂ������ׂ܂��B
	 *	@copydoc XSEMI_GemConstObject::isDirtyMember()
	 */
	virtual bool isDirtyMember();	
	/**	@brief	�I�u�W�F�N�g�̃����o�[���ύX��Ԃ����Z�b�g���܂��B
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
	// �����l
	XRichData	m_InitValue;
	// Enable
	bool		m_bEnable;
	// VARIABLE_GRP
	XD_UINT4	m_variable_grpid;
	// Private
	PRIVAL		m_bPrivate;
	// get_ex
	bool		m_bGetEx;
	// VVALS�ꗗ
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


/**	@brief	ECV�i���u�萔�j	*/
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


	/**	@brief	�R���X�g���N�^
     *
	 *	@param	vid	ID
	 *	@param	p_name	���O
	 *	@param	type	�f�[�^�^�C�v
	*/
	XSEMI_ECV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded = false);
	/**	@brief	�f�X�g���N�^	*/
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
	/**	@brief	�I�u�W�F�N�g�̃����o�[���ύX���ꂽ���ǂ������ׂ܂��B
	 *	@copydoc XSEMI_GemConstObject::isDirtyMember()
	 */
	virtual bool isDirtyMember();	
	/**	@brief	�I�u�W�F�N�g�̃����o�[���ύX��Ԃ����Z�b�g���܂��B
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

/**	@brief	SV�i��ԕϐ��j	*/
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


	/**	@brief	�R���X�g���N�^
     *
	 *	@param	vid	ID
	 *	@param	p_name	���O
	 *	@param	type	�f�[�^�^�C�v
	*/
	XSEMI_SV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded = false);
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSEMI_SV();




	/**	@brief	�f�[�^�T�C�Y�̍ŏ��l���擾
	 *
	 *	@return	�f�[�^�T�C�Y�̍ŏ��l
	 */
	XD_UINT4 get_SizeMax() ;
	/**	@brief	�f�[�^�T�C�Y�̍ŏ��l���Z�b�g
	 *
	 *	@param	val	�f�[�^�T�C�Y�̍ŏ��l
	 */
	void put_SizeMax(XD_UINT4 val);

	/**	@brief	�f�[�^�T�C�Y�̍ő�l���擾
	 *
	 *	@return	�f�[�^�T�C�Y�̍ő�l
	 */
	XD_UINT4 get_SizeMin() ;
	/**	@brief	�f�[�^�T�C�Y�̍ő�l���Z�b�g
	 *
	 *	@param	val	�f�[�^�T�C�Y�̍ő�l
	 */
	void put_SizeMin(XD_UINT4 val);


	/**	@brief	���~�b�g�I�[�oMAX�l
	 *
	 *	@return	���~�b�g�I�[�oMAX�l
	 */
	XRichData& get_limitMax();
	/**	@brief	���~�b�g�I�[�oMIN�l
	 *
	 *	@return	���~�b�g�I�[�oMIN�l
	 */
	XRichData& get_limitMin();

	/**	@brief	���~�b�g�I�[�o�[�C�x���gID���擾
	 *
	 *	@return	���~�b�g�I�[�o�[�C�x���gID
	 */
	XD_UINT4 get_CeLimit() ;
	/**	@brief	���~�b�g�I�[�o�[�C�x���gID���Z�b�g
	 *
	 *	@param	val	���~�b�g�I�[�o�[�C�x���gID
	 */
	void put_CeLimit(XD_UINT4 val);

// overwrite	XSEMI_GemConstObject
	/**	@brief	�I�u�W�F�N�g�̃����o�[���ύX���ꂽ���ǂ������ׂ܂��B
	 *	@copydoc XSEMI_GemConstObject::isDirtyMember()
	 */
	virtual bool isDirtyMember();	
	/**	@brief	�I�u�W�F�N�g�̃����o�[���ύX��Ԃ����Z�b�g���܂��B
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

/**	@brief	DV�i�ϐ��f�[�^�j	*/
class XLIB_API XSEMI_DV : public XSEMI_V
{
public:
	enum ATOM_POS_DV {
			ATOM_POS_DV_SIZE_MAX = XSEMI_V::ATOM_POS_VVALUE+1
		,	ATOM_POS_DV_SIZE_MIN
	};
	/**	@brief	�R���X�g���N�^
     *
	 *	@param	vid	ID
	 *	@param	p_name	���O
	 *	@param	type	�f�[�^�^�C�v
	*/
	XSEMI_DV(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, bool bLoaded = false);
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSEMI_DV();




	/**	@brief	�f�[�^�T�C�Y�̍ŏ��l���擾
	 *
	 *	@return	�f�[�^�T�C�Y�̍ŏ��l
	 */
	XD_UINT4 get_SizeMax() ;
	/**	@brief	�f�[�^�T�C�Y�̍ŏ��l���Z�b�g
	 *
	 *	@param	val	�f�[�^�T�C�Y�̍ŏ��l
	 */
	void put_SizeMax(XD_UINT4 val);

	/**	@brief	�f�[�^�T�C�Y�̍ő�l���擾
	 *
	 *	@return	�f�[�^�T�C�Y�̍ő�l
	 */
	XD_UINT4 get_SizeMin() ;
	/**	@brief	�f�[�^�T�C�Y�̍ő�l���Z�b�g
	 *
	 *	@param	val	�f�[�^�T�C�Y�̍ő�l
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



/**	@brief	V���ރN���X	*/
class XLIB_API XSEMI_VARIABLE_GRP :		public XSEMI_GemConstObjectAtom	
{
public:
	enum ATOM_POS {
			ATOM_POS_ID = 0
		,	ATOM_POS_NAME
		,	ATOM_POS_COMMENT

		,	ATOM_POS_OBJECT_TYPE = 99
	};

	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	id	ID
	 *	@param	p_name	���O
	 */
	XSEMI_VARIABLE_GRP(XD_UINT4 id, const TCHAR* p_name);
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSEMI_VARIABLE_GRP();


	
	/**	@brief	ID���擾���܂��B
	 *	@return	ID
	 */
	XD_UINT4 get_ID() const;
	/**	@brief	���O���擾���܂��B
	 *	@return	���O
	 */
	const TCHAR* get_Name();
	/**	@brief	���O��ݒ肵�܂��B
	 *	@param	val	���O
	 */
	void put_Name(const TCHAR* val);
	/**	@brief	�R�����g���擾���܂��B
	 *	@return	�R�����g
	 */
	const TCHAR* get_Comment();
	/**	@brief	�R�����g��ݒ肵�܂��B
	 *	@param	val	�R�����g
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



/**	@brief	VID�̈ꗗ�Ǘ��N���X	 */
class XLIB_API XSEMI_Vs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	�R���X�g���N�^ */
	XSEMI_Vs(bool bLinkeDatas);
	/**	@brief	�f�X�g���N�^ */
	virtual ~XSEMI_Vs();
	/**	@brief	VID�̈ꗗ��������܂��B */
	void clear();

	/**	@brief	�ꗗ�����擾���܂�
	 *	@return	�ꗗ��
	 */
	XD_UINT4 count() const;
	/**	@brief	SV�ꗗ�����擾���܂�
	 *	@return	SV�ꗗ��
	 */
	XD_UINT4 countSV() const;
	/**	@brief	ECV�ꗗ�����擾���܂�
	 *	@return	ECV�ꗗ��
	 */
	XD_UINT4 countECV() const;
	/**	@brief	DV�ꗗ�����擾���܂�
	 *	@return	DV�ꗗ��
	 */
	XD_UINT4 countDV() const;


	/**	@brief	�w�肵���C���f�b�N�X�̃A�C�e�����擾���܂��B
	 *
	 *	@param	index	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_V* get(XD_UINT4 index);

	/**	@brief	�w�肵���C���f�b�N�X��ECV�A�C�e�����擾���܂��B
	 *
	 *	@param	index	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_ECV* getECV(XD_UINT4 index);
	/**	@brief	�w�肵���C���f�b�N�X��SV�A�C�e�����擾���܂��B
	 *
	 *	@param	index	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_SV* getSV(XD_UINT4 index);
	/**	@brief	�w�肵���C���f�b�N�X��DV�A�C�e�����擾���܂��B
	 *
	 *	@param	index	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_DV* getDV(XD_UINT4 index);

	/**	@brief	�w�肵��VID�̃A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_V* get_byID(XD_UINT4 id);
	/**	@brief	���O����VID�̃A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_V* get_byName(const TCHAR* p_name);

	/**	@brief	�w�肵��VID��ECV�A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_ECV* get_EC_byID(XD_UINT4 id);
	/**	@brief	�w�肵��VID��SV�A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_SV*  get_SV_byID(XD_UINT4 id);
	/**	@brief	�w�肵��VID��DV�A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_DV*  get_DV_byID(XD_UINT4 id);

	/**	@brief	�ꗗ�̐擪�A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B������Ȃ��ꍇ��NULL�B
	 */
	XSEMI_V* getFirstItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̎��̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�Ō�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_V* getNextItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̑O�̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�擪�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_V* getPrevItem();
	/**	@brief	�A�C�e����ǉ����܂��B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_V* add(XSEMI_GemDataAtom* pAtom);
	/**	@brief	�A�C�e����ǉ����܂��BpAtom����ڍ׍��ڂ͎��o���A�ݒ肵�܂���B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_V* addNoDetail(XSEMI_GemDataAtom* pAtom);
	/**	@brief	�A�C�e����ǉ����܂��BpAtom����ڍ׍��ڂ͎��o���A�}�X�^�[�f�[�^�ɓo�^���܂��B
     *
	 *	@param	vid	ID
	 *	@param	p_name	���O
	 *	@param	type	�f�[�^�^�C�v
	 *	@param	Class	�N���X
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	*/
	XSEMI_V* add(XD_UINT4 vid, const TCHAR* p_name, XDATA_TYPE type, XSEMI_V::Class Class);

	/**	@brief	�A�C�e�����폜���܂��B
	 *
	 *	@param	pCEID	�A�C�e�����폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
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



/**	@brief	V���ރN���X(�ꗗ�Ǘ��N���X)	 */
class XLIB_API XSEMI_VARIABLE_GRPs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	�R���X�g���N�^ */
	XSEMI_VARIABLE_GRPs();
	/**	@brief	�f�X�g���N�^ */
	virtual ~XSEMI_VARIABLE_GRPs();


	/**	@brief	XSEMI_VARIABLE_GRP�̈ꗗ��������܂��B */
	void clear();

	/**	@brief	�ꗗ�����擾���܂�
	 *	@return	�ꗗ��
	 */
	XD_UINT4 count();
	/**	@brief	�w�肵���C���f�b�N�X�̃A�C�e�����擾���܂��B
	 *
	 *	@param	index	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_VARIABLE_GRP* get(XD_UINT4 index);
	/**	@brief	�w�肵��XSEMI_VARIABLE_GRP�̃A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_VARIABLE_GRP* get_byID(XD_UINT4 id);
	/**	@brief	�ꗗ�̐擪�A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B������Ȃ��ꍇ��NULL�B
	 */
	XSEMI_VARIABLE_GRP* getFirstItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̎��̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�Ō�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_VARIABLE_GRP* getNextItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̑O�̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�擪�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_VARIABLE_GRP* getPrevItem();
	/**	@brief	�A�C�e����ǉ����܂��B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_VARIABLE_GRP* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	�A�C�e�����폜���܂��B
	 *
	 *	@param	pCEID	�A�C�e�����폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
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
