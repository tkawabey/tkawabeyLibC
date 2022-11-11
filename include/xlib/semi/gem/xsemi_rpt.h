/**
 * @file xsemi_rpt.h
 * @brief �����̐���SEMI�K�i�@���|�[�gID�Ǘ��N���X
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_RPT_H__
#define __XSEMI_RPT_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/semi/gem/xsemi_v.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"
#include <vector>

class XLIB_API XSEMI_GemConst;
class XLIB_API XSEMI_CEID;

/**	@brief	���|�[�gID�Ǘ��N���X	 */
class XLIB_API XSEMI_Rpt : public XSEMI_GemConstObjectAtom
{
public:
	enum ATOM_POS {
			ATOM_POS_RPTID = 0
		,	ATOM_POS_NAME
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};


	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	rptid	���|�[�gID
	 *	@param	p_name	���|�[�g��
	 */
	XSEMI_Rpt(XD_UINT4 rptid, const TCHAR* p_name);
	/**	@brief	�f�X�g���N�^	 */
	virtual ~XSEMI_Rpt();

	/**	@brief	���|�[�gID���擾���܂��B
	 *
	 *	@return	���|�[�gID
	 */
	XD_UINT4 get_RptID();
	/**	@brief	���|�[�g�����擾���܂��B
	 *
	 *	@return	���|�[�g��
	 */
	const TCHAR* get_Name();
	/**	@brief	���|�[�g����ݒ肵�܂��B
	 *
	 *	@param	p_name	���|�[�g��
	 */
	void set_Name(const TCHAR* p_name);
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
	void set_Enable(bool val);


	/**	@brief	���|�[�g�ɁA�����N���Ă���V�̐����擾���܂��B
	 *
	 *	@return	���|�[�g�ɁA�����N���Ă���V�̐�
	 */
	XD_UINT4 get_VsCount() const;
	/**	@brief	�w�肵���C���f�b�N�X�̃��|�[�g�ɁA�����N���Ă���V���擾���܂��B
	 *
	 *	@return	V
	 */
	XSEMI_V* get_V(XD_UINT4 index);
	/**	@brief	�w�肵��ID�̃��|�[�g�ɁA�����N���Ă���V���擾���܂��B
	 *
	 *	@return	V
	 */
	XSEMI_V* get_VByID(XD_UINT4 vid);



// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();

	virtual void makeInsertString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream);
	virtual void makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	virtual void* getAtomValPtr(int index);
	virtual void makeDefineReportClearString(XLIB_STD_OUT_STR_STREAM& stream);
protected:
	XD_UINT4					m_rptid;		// Report ID
	XStrBuff					m_strName;		// Name
	XStrBuff					m_strComment;	// �R�����g
	bool						m_bEnable;		// Enable
	XSEMI_Vs					m_vs;
	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Rpts ;
};

/**	@brief	���|�[�g�ꗗ�Ǘ��N���X	 */
class XLIB_API XSEMI_Rpts : public XSEMI_GemConstObject
{
public:
	/**	@brief	�R���X�g���N�^
	 *  @param bLinkeDatas ���̃C���X�^���X���AXSEMI_Rpt�̃����N���̂݊Ǘ����邩�ǂ����H
	 */
	XSEMI_Rpts(bool bLinkeDatas);
	/**	@brief	�f�X�g���N�^ */
	virtual ~XSEMI_Rpts();
	/**	@brief	���|�[�g�̈ꗗ��������܂��B */
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
	XSEMI_Rpt* get(XD_UINT4 index);
	/**	@brief	�w�肵�����|�[�g�̃A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_Rpt* get_byID(XD_UINT4 id);

	/**	@brief	���O���烌�|�[�g�̃A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_Rpt* get_byName(const TCHAR* p_name);

	/**	@brief	�ꗗ�̐擪�A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B������Ȃ��ꍇ��NULL�B
	 */
	XSEMI_Rpt* getFirstItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̎��̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�Ō�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_Rpt* getNextItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̑O�̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�擪�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_Rpt* getPrevItem();
	/**	@brief	�A�C�e����ǉ����܂��B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_Rpt* add(XSEMI_GemDataAtom* pAtom);
	/**	@brief	�A�C�e�����폜���܂��B
	 *
	 *	@param	pRpt	�A�C�e�����폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool del(XSEMI_Rpt* pRpt);
protected:
	std::vector<XSEMI_Rpt*>				m_rpts;
	std::vector<XSEMI_Rpt*>::iterator	m_ite;
	bool								m_bLinkDatas;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_CEID;
};


#include "xlib/xlib_tail.h"
#endif /* __XSEMI_RPT_H__ */
