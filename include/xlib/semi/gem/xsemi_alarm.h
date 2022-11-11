/**
 * @file xsemi_alarm.h
 * @brief �����̐���SEMI�K�i�@�A���[���f�[�^�Ǘ��N���X
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_ALARAM_H__
#define __XSEMI_ALARAM_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata_rich.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"


class XLIB_API XSEMI_GemConst;


class XLIB_API XSEMI_ALARM;
class XLIB_API XSEMI_ALARMs;
DLL_EXTERN template class XLIB_API std::vector<XSEMI_ALARM*>;

//typedef XLIB_API std::vector<XSEMI_ALARM*> XSEMI_ALARMs;


/**	@brief	�A���[���f�[�^�Ǘ��N���X	*/
class XLIB_API XSEMI_ALARM : public XSEMI_GemConstObjectAtom
{
public:
	enum ATOM_POS {
			ATOM_POS_ALID = 0
		,	ATOM_POS_ALCD
		,	ATOM_POS_NAME
		,	ATOM_POS_TEXT
		,	ATOM_POS_ON_CEID
		,	ATOM_POS_OFF_CEID
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};


// �v���p�e�B
	/**	@brief	�R���X�g���N�^
     *
	 *	@param	alarmID	ID
	 *	@param	p_name	���O
	*/
	XSEMI_ALARM(XD_UINT4 alarmID, const TCHAR* p_name);
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSEMI_ALARM();
	

	XD_UINT4 get_AlarmID();
	XD_UINT1 get_ALCD() const;
	void set_ALCD(XD_UINT1 val);
	const TCHAR* get_Name();
	void set_Name(const TCHAR* p_name);
	const TCHAR* get_Text();
	void set_Text(const TCHAR* p_text);
	const TCHAR* get_Comment();
	void set_Comment(const TCHAR* p_comment);

	XD_UINT4 get_OnCEID() const;
	void set_OnCEID(XD_UINT4 val);
	XD_UINT4 get_OffCEID() const;
	void set_OffCEID(XD_UINT4 val);

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


// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT4					m_ulAlarmId;	// ALARM ID
	XD_UINT1					m_alcd;			// ALCD
	XStrBuff					m_strName;		// Name
	XStrBuff					m_strText;		// Text
	XStrBuff					m_strComment;	// Comment
	XD_UINT4					m_ulOnEvent;	// On Event
	XD_UINT4					m_ulOffEvent;	// Off Event
	bool						m_bEnable;		// �L��/����

	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_ALARMs;
};





/**	@brief	�A���[���f�[�^�̈ꗗ�Ǘ��N���X	 */
class XLIB_API XSEMI_ALARMs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	�R���X�g���N�^ */
	XSEMI_ALARMs();
	/**	@brief	�f�X�g���N�^ */
	virtual ~XSEMI_ALARMs();
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
	XSEMI_ALARM* get(XD_UINT4 index);
	/**	@brief	�w�肵��ALARM�̃A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_ALARM* get_byID(XD_UINT4 id);

	/**	@brief	���O����ALARM�̃A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_ALARM* get_byName(const TCHAR* p_name);

	/**	@brief	�ꗗ�̐擪�A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B������Ȃ��ꍇ��NULL�B
	 */
	XSEMI_ALARM* getFirstItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̎��̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�Ō�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_ALARM* getNextItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̑O�̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�擪�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_ALARM* getPrevItem();
	/**	@brief	�A�C�e����ǉ����܂��B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_ALARM* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	�A�C�e�����폜���܂��B
	 *
	 *	@param	pCEID	�A�C�e�����폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool del(XSEMI_ALARM* pCEID);
protected:
	std::vector<XSEMI_ALARM*>			m_array;
	std::vector<XSEMI_ALARM*>::iterator	m_ite;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};


#include "xlib/xlib_tail.h"
#endif /* __XSEMI_ALARAM_H__ */
