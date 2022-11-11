/**
 * @file xsemi_ackcode.h
 * @brief �����̐���SEMI�K�i ACK�R�[�h�f�[�^�Ǘ��N���X
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_ACKCODE_H__
#define __XSEMI_ACKCODE_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata_rich.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"


class XLIB_API XSEMI_GemConst;

class XLIB_API XSEMI_ACKCODE;
class XLIB_API XSEMI_ACKCODEs;

DLL_EXTERN template class XLIB_API std::vector<XSEMI_ACKCODE*>;


/**	@brief	ACK�R�[�h	*/
class XLIB_API XSEMI_ACKCODE : public XSEMI_GemConstObjectAtom	
{

public:
	enum ATOM_POS {
			ATOM_POS_ACKKIND = 0
		,	ATOM_POS_ACKCODE
		,	ATOM_POS_COMMENT
		,	ATOM_POS_ENABLE

		,	ATOM_POS_OBJECT_TYPE = 99
	};
	enum KIND {
			KIND_ACKA = 0		// S5F14,F15,F18,S16F2,F4,F6,F12,F14,F16,F18,F24,F26,F30,S17F4,F8,F14
		,	KIND_ACKC3			// S3F6,F8,F10
		,	KIND_ACKC5			// S5F2,F4
		,	KIND_ACKC6			// S6F2,F4,F10,F12,F14
		,	KIND_ACKC7			// S7F4,F12,F14,F16,F18,F24,F32
		,	KIND_ACKC7A			// S7F27
		,	KIND_ACKC10			// S10F2,F4,F6,F10
		,	KIND_ACKC13			// S13F2,F4,F6,F8
		,	KIND_CAACK			// Carrier Action Acknowledge Code, S3F18,F20,F22,F24,F26
		,	KIND_CEPACK			// S2F50
		,	KIND_CPACK			// S2F42
		,	KIND_CSAACK			// S2F8
		,	KIND_DATAACK		// S14F22
		,	KIND_DRACK			// S2F34
		,	KIND_EAC			// S2F16
		,	KIND_ERACK			// S2F38
		,	KIND_ERRCODE		//
		,	KIND_GRANT			// S2F2,F40,S3F16,S4F26,S13F12,S14F24,S16F2,F12
		,	KIND_GRANT6			// S6F6
		,	KIND_GRANT1			// S12F6
		,	KIND_HCACK			// S2F42,F50
		,	KIND_HOHALTACK		// S4F41
		,	KIND_LIMITACK		// S2F46
		,	KIND_LRACK			// S2F36
		,	KIND_LVACK			// S2F46
		,	KIND_MAPER			// S12F19
		,	KIND_MAPFT			// S12F3,F5
		,	KIND_MDACK			// S12F8,F10,F12
		,	KIND_MIDAC			// S3F14
		,	KIND_MIDRA			// S3F12
		,	KIND_OBJACK			// S14F2,F6,F8,F10,F12,F14,F16,F18,F26,F28
		,	KIND_OFLACK			// S1F16
		,	KIND_ONLACK			// S1F18
		,	KIND_PPGNT			// S7F2,F30
		,	KIND_RAC			// S2F20
		,	KIND_RMACK			// S15F4,F6,F8,F10,F12,F14.....
		,	KIND_RMGRANT		// S15F2,F37,F46
		,	KIND_RRACK			// S4F18
		,	KIND_RSACK			// S4F2
		,	KIND_RSDA			// S6F24
		,	KIND_RSPACK			// S2F44
		,	KIND_SDACK			// S12F2
		,	KIND_SPAACK			// S2F4
		,	KIND_SSACK			// S18F2,F4,F6,F10,F12,F14
		,	KIND_SVACK			// S14F20
		,	KIND_TBLACK			// S13F14,F16
		,	KIND_TIAACK			// S2F24
		,	KIND_TIACK			// S2F32
		,	KIND_TRACK			// S4F20,F22,F23
		,	KIND_VLAACK			// S2F46
	};
// �v���p�e�B
	/**	@brief	�R���X�g���N�^
     *
	 *	@param	ackkind	ACK�̎��
	 *	@param	code	�R�[�h
	*/
	XSEMI_ACKCODE(XD_UINT4 ackkind, XD_UINT1 code);
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XSEMI_ACKCODE();

// property	
	/**	@brief	ACK�̎�ނ��擾���܂��B
	 *
	 *	@return	XD_UINT4
	 */
	XD_UINT4 get_AckKind() const;

	/**	@brief	ACK�R�[�h���擾���܂��B
	 *	@return	ACK�R�[�h
	 */
	XD_UINT1 get_AckCode() const;
	/**	@brief	ACK�R�[�h��ݒ肵�܂��B
	 *
	 *	@param	val	ACK�R�[�h
	 */
	void set_AckCode(XD_UINT1 val);

	/**	@brief	�R�����g���擾���܂��B
	 *	@return	�R�����g
	 */
	const TCHAR* get_Comment();
	/**	@brief	�R�����g��ݒ肵�܂��B
	 *	@param	p_comment	�R�����g
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

// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT4					m_ulAckKind;	// Ack Kind
	XD_UINT1					m_ulAckCode;	// ACKCODE
	XStrBuff					m_strComment;	// Comment
	bool						m_bEnable;		// �L��/����

	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
friend class XSEMI_ACKCODEs;
};



/**	@brief	ACK�R�[�h�̈ꗗ�Ǘ��N���X	 */
class XLIB_API XSEMI_ACKCODEs :  public XSEMI_GemConstObject
{
public:
	/**	@brief	�R���X�g���N�^ */
	XSEMI_ACKCODEs();

	/**	@brief	�f�X�g���N�^ */
	virtual ~XSEMI_ACKCODEs();

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
	XSEMI_ACKCODE* get(XD_UINT4 index);

	/**	@brief	�w�肵��CEID�̃A�C�e�����擾���܂��B
	 *
	 *	@param	id	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_ACKCODE* get_byID(XD_UINT4 kind, XD_UINT4 code);

	/**	@brief	�ꗗ�̐擪�A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B������Ȃ��ꍇ��NULL�B
	 */
	XSEMI_ACKCODE* getFirstItem();

	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̎��̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�Ō�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_ACKCODE* getNextItem();

	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̑O�̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�擪�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_ACKCODE* getPrevItem();

	/**	@brief	�A�C�e����ǉ����܂��B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_ACKCODE* add(XSEMI_GemDataAtom* pAtom);

	/**	@brief	�A�C�e�����폜���܂��B
	 *
	 *	@param	pCEID	�A�C�e�����폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool del(XSEMI_ACKCODE* pAckCode);
protected:
	std::vector<XSEMI_ACKCODE*>				m_array;
	std::vector<XSEMI_ACKCODE*>::iterator	m_ite;
	bool									m_bloaded;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};


#include "xlib/xlib_tail.h"
#endif /* __XSEMI_ACKCODE_H__ */
