/**
 * @file xdllorder.h
 * @brief ���I�����N���s�����[�_
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DL_LORDER_FILE__
#define __X_DL_LORDER_FILE__

#include "xlib/xlib_head.h"



/**	@brief	���I�����N���s�����[�_ */
class XLIB_API XDlLorder
{
public:
	/**	@brief	�R���X�g���N�^ */
	XDlLorder();
	/**	@brief	�f�X�g���N�^ */
	virtual ~XDlLorder();

	/**	@brief	�V�F�A�[���C�u�����[�����[�h
	 *
	 *	@param	filename	�t�@�C����
	 *	@param	flag	�t���O�BOS�Ɉˑ����܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool open(const char* filename, int flag = 0);

	/**	@brief	�V�F�A�[���C�u�����[�����[�h
	 *
	 *	@param	filename	�t�@�C����
	 *	@param	flag	�t���O�BOS�Ɉˑ����܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool open(const wchar_t* filename, int flag = 0);
	/**	@brief	�V�F�A�[���C�u�����[���A�����[�h
	 *
	 *	@retval	true	����
	 *	@retval	false	���s	
	 */
	virtual bool close();

	/**	@brief	�V���{�����̕�������������Ɏ��A���̃V���{�������[�h���ꂽ�������̃A�h���X��Ԃ��B
	 *
	 *	@param	symbol	�V���{�����̕�����
	 *
	 *	@return	���{�������[�h���ꂽ�������̃A�h���X
	 */
	virtual void* loadSymbol(const char* symbol); 

	/**	@brief	�V���{�����̕�������������Ɏ��A���̃V���{�������[�h���ꂽ�������̃A�h���X��Ԃ��B
	 *
	 *	@param	symbol	�V���{�����̕�����
	 *
	 *	@return	���{�������[�h���ꂽ�������̃A�h���X
	 */
	virtual void* loadSymbol(const wchar_t* symbol); 
private:
	/*	@brief ���I���C�u�����́u�n���h���v */
	void*	m_pHandle;
};


#include "xlib/xlib_tail.h"
#endif // __X_DL_LORDER_FILE__
