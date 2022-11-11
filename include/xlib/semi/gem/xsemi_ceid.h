/**
 * @file xsemi_ceid.h
 * @brief �����̐���SEMI�K�i�@���W�C�x���g(CEID)�Ǘ��N���X
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2006 T.Fujiha,
 */
#ifndef __XSEMI_CEID_H__
#define __XSEMI_CEID_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"

class XLIB_API XSEMI_Gem;
class XLIB_API XSEMI_GemConst;
class XLIB_API XSEMI_Rpt;
class XLIB_API XSEMI_Rpts;
class XLIB_API XSEMI_CEID;


#include "xlib/semi/gem/xsemi_gem_const_obj.h"
#include "xlib/semi/gem/xsemi_rpt.h"
#include <vector>

class XLIB_API XSEMI_CEIDs;

//typedef XLIB_API std::vector<XSEMI_Rpt*> XSEMI_Rpts;

DLL_EXTERN template class XLIB_API std::vector<XSEMI_CEID*>;


/**	@brief	���W�C�x���g(CEID)�Ǘ��N���X	 */

class XLIB_API XSEMI_CEID :  public XSEMI_GemConstObjectAtom
{
public:
	enum ATOM_POS {
			ATOM_POS_CEID = 0
		,	ATOM_POS_NAME
		,	ATOM_POS_DISPLAY_NAME
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};


	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	ceid	CEID
	 *	@param	p_name	�C�x���g��
	 *	@param	p_comment	�R�����g
	 */
	XSEMI_CEID(XD_UINT4 ceid, const TCHAR* p_name, const TCHAR* p_comment);
	/**	@brief	�f�X�g���N�^	 */
	virtual ~XSEMI_CEID();


	/**	@brief	CEID���擾���܂��B
	 *
	 *	@return	CEID
	 */
	XD_UINT4 get_CEID() const ;
	/**	@brief	�C�x���g�����擾���܂��B
	 *
	 *	@return	�C�x���g��
	 */
	const TCHAR* get_Name();
	/**	@brief	�C�x���g����ݒ肵�܂��B
	 *
	 *	@param	p_name	�C�x���g��
	 */
	void set_Name(const TCHAR* p_name);


	/**	@brief	�C�x���g�\�������擾���܂��B
	 *
	 *	@return	�C�x���g�\����
	 */
	const TCHAR* get_DisplayName();
	/**	@brief	�C�x���g�\������ݒ肵�܂��B
	 *
	 *	@param	p_name	�C�x���g�\����
	 */
	void set_DisplayName(const TCHAR* p_name);


		
	/**	@brief	�R�����g���擾���܂��B
	 *
	 *	@return	�R�����g
	 */
	const TCHAR* get_Comment();
	/**	@brief	�R�����g��ݒ肵�܂��B
	 *
	 *	@param	p_comment	�R�����g
	 *	
	 */
	void set_Comment(const TCHAR* p_comment);

	/**	@brief	�L��/�����t���O���擾���܂��B
	 *
	 *	@retval	true	�L��
	 *	@retval	false	����
	 */
	bool get_Enable() const;
	/**	@brief	�L��/�����t���O���擾���܂��B
	 *
	 *	@param	val	true:�L��/false:����
	 */
	void put_Enable(bool val);


	/**	@brief	���[�U�[�f�[�^���擾���܂��B
	 *
	 *	@return	���[�U�[�f�[�^
	 */
	XD_UINT4 get_UserData() const;
	/**	@brief	���[�U�[�f�[�^��ݒ肵�܂��B
	 *
	 *	@param	val	���[�U�[�f�[�^
	 */
	void set_UserData(XD_UINT4 val);

	/**	@brief	CEID�ɔ���t�����|�[�gID�̐����A���܂��B
	 *
	 *	@return	CEID�ɔ���t��CEID�̐�
	 */
	XD_UINT4 get_RptsCount();
	/**	@brief	�w�肵���C���f�b�N�X��CEID�ɔ���t�����|�[�gID�I�u�W�F�N�g���擾����B
	 *
	 *	@param	index	�O����n�܂�C���f�b�N�X
	 *
	 *	@return	���|�[�gID�I�u�W�F�N�g
	 */
	XSEMI_Rpt* get_Rpt(XD_UINT4 index);
	/**	@brief	���|�[�gID��CEID�ɔ���t�����|�[�gID�I�u�W�F�N�g���擾����B
	 *
	 *	@param	rptid	���|�[�gID
	 *
	 *	@return	���|�[�gID�I�u�W�F�N�g
	 */
	XSEMI_Rpt* get_RptByID(XD_UINT4 rptid);

// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();

	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
// overwrite	XSEMI_GemConstObjectAtom
	virtual void* getAtomValPtr(int index);
	virtual void makeEventLinkClearString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	XD_UINT4					m_ceid;				// CEID
	XStrBuff					m_strName;			// Name
	XStrBuff					m_strDisplayName;	// DisplayName
	XStrBuff					m_strComment;		// Comment
	bool						m_bEnable;			// Enable
	XD_UINT4					m_ulUserData;		// UserData
	XSEMI_Rpts					m_rpts;				// RportIDs
	bool						m_bDirtyLinked;		// �����N��񂪕ύX���ꂽ���ǂ����̃t���O


	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_CEIDs;
};

/**	@brief	���W�C�x���g(CEID)�̈ꗗ�Ǘ��N���X	 */
class XLIB_API XSEMI_CEIDs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	�R���X�g���N�^ */
	XSEMI_CEIDs();
	/**	@brief	�f�X�g���N�^ */
	virtual ~XSEMI_CEIDs();
	/**	@brief	CEID�̈ꗗ��������܂��B */
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
	XSEMI_CEID* get(XD_UINT4 index);

	/**	@brief	�w�肵��CEID�N���X�̃C���X�^���X���擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	CEID�N���X�̃C���X�^���X
	 */
	XSEMI_CEID* get_byID(XD_UINT4 id);

	/**	@brief	���O����CEID�N���X�̃C���X�^���X���擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	CEID�N���X�̃C���X�^���X
	 */
	XSEMI_CEID* get_byName(const TCHAR* p_name);

	/**	@brief	�ꗗ�̐擪CEID�N���X�̃C���X�^���X���擾����B
	 *
	 *	@return	CEID�N���X�̃C���X�^���X�B������Ȃ��ꍇ��NULL�B
	 */
	XSEMI_CEID* getFirstItem();

	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̎���CEID�N���X�̃C���X�^���X���擾����B
	 *
	 *	@return	CEID�N���X�̃C���X�^���X�B�Ō�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_CEID* getNextItem();

	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̑O��CEID�N���X�̃C���X�^���X���擾����B
	 *
	 *	@return	CEID�N���X�̃C���X�^���X�B�擪�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_CEID* getPrevItem();

	/**	@brief	CEID�N���X�̃C���X�^���X��ǉ����܂��B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_CEID* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	�A�C�e����ǉ����܂��B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_CEID* add(XD_UINT4 ceid, const TCHAR* p_name);

	/**	@brief	CEID�N���X�̃C���X�^���X���폜���܂��B
	 *
	 *	@param	pCEID	CEID�N���X�̃C���X�^���X���폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool del(XSEMI_CEID* pCEID);
protected:
	std::vector<XSEMI_CEID*>			m_ceid;
	std::vector<XSEMI_CEID*>::iterator	m_ite;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};

#include "xlib/xlib_tail.h"
#endif /* __XSEMI_CEID_H__ */