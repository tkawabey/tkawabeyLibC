/**
 * @file xjson_obj.h
 * @brief JSON-Object�N���X
 *
 * @author T.Fujiha
 * @date 2012-02-15
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_JSON_OBJECT_HEADER_FILE__
#define __X_JSON_OBJECT_HEADER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"

class XLIB_API XJsonVal;

/**
 * @brief JSON-Object�N���X
 */
class XLIB_API XJsonObj
{
public:	
	
	/**
	 * @brief �v���p�e�B�[�ɃA�N�Z�X���邽�߂̃C�e���[�^�[�N���X
	 */
	class Iterator {
	public:
		/**
		 * @brief �R���X�g���N�^�B
		 */
		Iterator();
		/**
		 * @brief �R���X�g���N�^�B
		 * @param	right	�������C�e���[�^�[
		 */
		Iterator(const Iterator& right);
		/**
		 * @brief �f�X�g���N�^
		 */
		virtual ~Iterator();
		
		/**
		 * @brief ������Z�q
		 * @param	src	�������C�e���[�^�[
		 */
		Iterator &operator=(const Iterator &src);

		/**
		 * @brief �C�e���[�^�[�̎w�������v���p�e�B�[�̈ʒu�����̃v���p�e�B�[�Ɉړ������܂��B
		 */
		Iterator &operator++();
		/**
		 * @brief �C�e���[�^�[�̎w�������v���p�e�B�[�̈ʒu��O�̃v���p�e�B�[�Ɉړ������܂��B
		 */
		Iterator &operator--();
		
		/**
		 * @brief �C�e���[�^�[�̎w�������v���p�e�B�[�̈ʒu���擪���ǂ����`�F�b�N
		 * @retval	true	�擪
		 * @retval	false	�擪�łȂ�
		 */
		bool isBegin();
		/**
		 * @brief �C�e���[�^�[�̎w�������v���p�e�B�[�̈ʒu���Ōォ�ǂ����`�F�b�N
		 * @retval	true	�Ō�
		 * @retval	false	�Ō�łȂ�
		 */
		bool isEnd();

		/**
		 * @brief �C�e���[�^�[�̎w�������v���p�e�B�[�̖��O���擾
		 * @return	�v���p�e�B�[��
		 */
		const XStrBuff& getKey();
		/**
		 * @brief �C�e���[�^�[�̎w�������v���p�e�B�[�̒l���擾
		 * @return	�v���p�e�B�[�l
		 */
		XJsonVal getValue();

	protected:
		void*	m_pIte;
		void*	m_pMap;

		friend class XJsonObj;
	};


	/**
	 * @brief �R���X�g���N�^�B
	 */	
	XJsonObj();
	/**
	 * @brief �R���X�g���N�^�B
	 * @param	val	�������I�u�W�F�N�g
	 */	
	XJsonObj(const XJsonObj& val);
	/**
	 * @brief �f�X�g���N�^
	 */	
	virtual ~XJsonObj();
	
	/**
	 * @brief �v���p�e�B�[���Z�b�g���܂��B
	 * @param	key	�v���p�e�B�[��
	 * @param	val	�v���p�e�B�[�l
	 * @retval	true	����
	 * @retval	false	���s
	 */	
	bool add(const XStrBuff& key, XJsonVal& val);
	/**
	 * @brief �v���p�e�B�[���폜���܂��B
	 * @param	strKey	�v���p�e�B�[��
	 * @retval	true	����
	 * @retval	false	���s
	 */	
	bool del(const XStrBuff &strKey);
	/**
	 * @brief �Z�b�g����ăv���p�e�B�[����Ԃ��܂��B
	 * @return	�v���p�e�B�[��
	 */	
	size_t	size() const;
	/**
	 * @brief �C�e���[�^���擾���܂��B
	 * @return	�C�e���[�^
	 */	
	Iterator getIterator() const;
	
	/**
	 * @brief ��r�i��v�j�I�y���[�^�B
	 * @param src	��r����I�u�W�F�N�g
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool operator==(const XJsonObj &rhs) const;	
	/**
	 * @brief ��r�i�s��v�j�I�y���[�^�B
	 * @param src	��r����I�u�W�F�N�g
	 * @retval	true	�s��v
	 * @retval	false	��v
	 */
	bool operator!=(const XJsonObj &rhs) const;
	/**
	 * @brief ��r�i����j�I�y���[�^�B
	 * @param src	��r����I�u�W�F�N�g
	 * @retval	true	��r�I�u�W�F�N�g��菬�����B
	 * @retval	false	��r�I�u�W�F�N�g���傫���B
	 */
	bool operator<(const XJsonObj &rhs) const;
	/**
	 * @brief ��r�i����B�܂ށj�I�y���[�^�B
	 * @param src	��r����I�u�W�F�N�g
	 * @retval	true	��r�I�u�W�F�N�g��菬�����B�܂��́A�����B
	 * @retval	false	��r�I�u�W�F�N�g���傫���B
	 */
	bool operator<=(const XJsonObj &rhs) const;
	/**
	 * @brief ��r�i���j�I�y���[�^�B
	 * @param src	��r����I�u�W�F�N�g
	 * @retval	true	��r�I�u�W�F�N�g���傳���B
	 * @retval	false	��r�I�u�W�F�N�g��菬�����B
	 */
	bool operator>(const XJsonObj &rhs) const;
	/**
	 * @brief ��r�i���B�܂ށj�I�y���[�^�B
	 * @param src	��r����I�u�W�F�N�g
	 * @retval	true	��r�I�u�W�F�N�g���傳���B�܂��́A�����B
	 * @retval	false	��r�I�u�W�F�N�g��菬�����B
	 */
	bool operator>=(const XJsonObj &rhs) const;

	/**
	 * @brief �w�肵���v���p�e�B�[���̒l���擾
	 *        �v���p�e�B�[�������݂��Ȃ��ꍇ�́A�쐬����B
	 * @param key	�v���p�e�B�[��
	 * @return	�v���p�e�B�[�l
	 */
	XJsonVal &operator[](const char *key);
	
	/**
	 * @brief �w�肵���v���p�e�B�[���̒l���擾
	 *        �v���p�e�B�[�������݂��Ȃ��ꍇ�́A�쐬����B
	 * @param key	�v���p�e�B�[��
	 * @return	�v���p�e�B�[�l
	 */
	XJsonVal &operator[](const wchar_t *key);

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

#endif /*__X_JSON_OBJECT_HEADER_FILE__*/