/**
 * @file xjson_array.h
 * @brief JSON-Array�i�z��j�N���X
 *
 * @author T.Fujiha
 * @date 2012-02-15
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_JSON_ARRAY_HEADER_FILE__
#define __X_JSON_ARRAY_HEADER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"
#include "xlib/xmap.h"


class XLIB_API XJsonVal;

/**
 * @brief JSON-Array�i�z��j�N���X
 */
class XLIB_API XJsonArray
{
public:	
	/**
	 * @brief �R���X�g���N�^�B
	 */	
	XJsonArray();
	/**
	 * @brief �R���X�g���N�^�B
	 * @param	val	�������I�u�W�F�N�g
	 */	
	XJsonArray(const XJsonArray& val);
	/**
	 * @brief �f�X�g���N�^
	 */	
	virtual ~XJsonArray();
	
	/**
	 * @brief ��r�i��v�j�I�y���[�^�B
	 * @param src	��r����z��
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool operator==(const XJsonArray &rhs) const;
	/**
	 * @brief ��r�i�s��v�j�I�y���[�^�B
	 * @param src	��r����z��
	 * @retval	true	�s��v
	 * @retval	false	��v
	 */
	bool operator!=(const XJsonArray &rhs) const;
	/**
	 * @brief ��r�i����j�I�y���[�^�B
	 * @param src	��r����z��
	 * @retval	true	��r�z���菬�����B
	 * @retval	false	��r�z����傫���B
	 */
	bool operator<(const XJsonArray &rhs) const;
	/**
	 * @brief ��r�i����B�܂ށj�I�y���[�^�B
	 * @param src	��r����z��
	 * @retval	true	��r�z���菬�����B�܂��́A�����B
	 * @retval	false	��r�z����傫���B
	 */
	bool operator<=(const XJsonArray &rhs) const;
	/**
	 * @brief ��r�i���j�I�y���[�^�B
	 * @param src	��r����z��
	 * @retval	true	��r�z����傳���B
	 * @retval	false	��r�z���菬�����B
	 */
	bool operator>(const XJsonArray &rhs) const;
	/**
	 * @brief ��r�i���B�܂ށj�I�y���[�^�B
	 * @param src	��r����z��
	 * @retval	true	��r�z����傳���B�܂��́A�����B
	 * @retval	false	��r�z���菬�����B
	 */
	bool operator>=(const XJsonArray &rhs) const;

	/**
	 * @brief �w�肵���C���f�b�N�X�̒l���擾
	 *        �I�[�o�[�t���[�����C���f�b�N�X���w�肵���ꍇ�́A�z����w�肵���C���f�b�N�X���g�����āA�쐬�����I�u�W�F�N�g��Ԃ��B
	 * @param index	�C���f�b�N�X
	 * @return	�v���p�e�B�[�l
	 */
	XJsonVal &operator[](size_t index);
	
	/**
	 * @brief �w�肵���C���f�b�N�X�̒l���擾
	 *        �I�[�o�[�t���[�����C���f�b�N�X���w�肵���ꍇ�́A�z����w�肵���C���f�b�N�X���g�����āA�쐬�����I�u�W�F�N�g��Ԃ��B
	 * @param index	�C���f�b�N�X
	 * @return	�v���p�e�B�[�l
	 */
	XJsonVal &at(size_t index){ return (*this)[index];}

	/**
	 * @brief �z�񐔂��擾���܂��B
	 * @return	�z��
	 */
	size_t size() const;
	
	/**
	 * @brief �z��̐����g�����܂��B
	 * @param n		�z��
	 */
	void resize(size_t n);

	/**
	 * @brief �z��̍Ō��XJsonVal��ǉ����܂��B
	 */
	void push_back(XJsonVal& newVal);
	/**
	 * @brief �z��̍Ō�̗v�f���폜���܂��B
	 */
	void pop_back();
	/**
	 * @brief �z��̍Ō�̗v�f���擾���܂��B
	 */
	XJsonVal& back();

	/**
	 * @brief	JSON��������X�g���[���ɏ������ށB
	 * @param	stream	�X�g���[��
	 * @param	nDepth	�C���f���g�̐[���B~0���w�肷��ƁA�C���f���g���������܂Ȃ��B
	 * @param	bNonEscape	�������G�X�P�[�v����ꍇ��false�B
	 */
	void print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth = (size_t)~0, bool bNonEscape = false) const;
protected:
	void*								m_pObj;
};

#endif /*__X_JSON_ARRAY_HEADER_FILE__*/
