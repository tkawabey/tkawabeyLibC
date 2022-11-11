/**
 * @file xarray.h
 * @brief �z��N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_ARRAY_HEASER_FILE__
#define __X_ARRAY_HEASER_FILE__
#include "xlib/xlib_head.h"
#include <stdlib.h>
#include <string.h>

/**
 * �z��N���X���e���v���[�g��
 * T �z��Ɋi�[�����I�u�W�F�N�g�̌^
 */
template <class T>
class XArray
{
public:
	/* �f�[�^ **/
	T* m_aT;
	/* �T�C�Y **/
	int m_nSize;
	/* �������[�T�C�Y **/
	int m_nAllocSize;
	/* �����폜 **/
	bool m_AutoDelete;


	/** �R���X�g���N�^ */
	XArray();
	/** �f�X�g���N�^ */
	virtual ~XArray();

	/**
	 * �z��̃T�C�Y���擾���܂�
	 * @return �z��̃T�C�Y
	 */
	int GetSize() const;
	/**
	 * �z��ɗv�f��ǉ����܂�
	 * @param t �ǉ�����v�f
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Add(T& t);
	/**
	 * �z��ɗv�f����A�w�肵���A�h���X�̃f�[�^���폜���܂�
	 * @param t �폜����v�f
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Remove(T& t);
	/**
	 * �z��ɗv�f����A�w�肵���A�h���X�̃f�[�^���폜���܂�
	 * @param nIndex �폜����C���f�b�N�X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool RemoveAt(int nIndex);
	/** �z��ɗv�f����A�w�肵���A�h���X�̃f�[�^���폜���܂� */
	void RemoveAll();
	/**
	 * �z��ɗv�f����A�w�肵���A�h���X�̃f�[�^���폜���܂�
	 * @param nIndex �擾����C���f�b�N�X
	 * @return �w�肵���C���f�b�N�X�̃f�[�^
	 */
	T& operator[] (int nIndex) const;
	/**
	 * �z��̃f�[�^���擾���܂�
	 * @return �z��̃A�h���X
	 */
	T* GetData() const;
	/**
	 * �w�肵���C���f�b�N�X�ɁA�v�f���Z�b�g���܂�
	 * @param nIndex �Z�b�g����C���f�b�N�X
	 * @param t �Z�b�g����v�f�̃A�h���X
	 */
	void SetAtIndex(int nIndex, T& t);
	/**
	 * �w�肵���C���f�b�N�X�ɁA�v�f���Z�b�g���܂�
	 * @param t ��������v�f�̃A�h���X
	 * @return ���������C���f�b�N�X ������Ȃ������ꍇ��-1
	 */
	int Find(T& t) const;
};

/** ���X�g�A�N�Z�X�p�w���p�[�\���� */
struct __XPOSITION { };
/** ���X�g�A�N�Z�X�p�w���p�[�\���� */
typedef __XPOSITION* XPOSITION; 

/**
 * ���X�g�A�N�Z�X�p�w���p�[�\����
 */
struct XPlex
{
	XPlex* pNext;
	void* data(){ return this +1;}
	static XPlex* Create(XPlex*& pHead, unsigned int nMax, unsigned int cbElement);
	void FreeDataChain();
};

/**	@brief	���X�g�N���X */
class XLIB_API XList
{
protected:
	/* @brief �u���b�N */
	struct XNode {
		XNode* pNext;	// ���̃u���b�N
		XNode* pPrev;	// �O�̃u���b�N
		void* data;		// �u���b�N�̃f�[�^
	};
public:
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] nBlockSize �����u���b�N�T�C�Y
	 */
	XList(int  nBlockSize = 10);
	/** @brief	�f�X�g���N�^ */
	virtual ~XList(){RemoveAll();}

	
	/**
	 * @brief	���X�g�̗v�f����Ԃ��܂��B
	 * @return ���X�g�̗v�f��
	 */
	int GetCount() const;
	/**
	 * @brief	���X�g�̏�Ԃ���(�v�f���Ȃ�)���ǂ����𒲂ׂ܂��B
	 * @retval true ��
	 * @retval false ��łȂ�
	 */
	bool IsEmpty() const;


	/**
	 * @brief	���X�g�̐擪�v�f��Ԃ��܂��B���X�g����̂Ƃ��͎g�p�ł��܂���B
	 * @return ���X�g�v�f�ւ̎Q��
	 */
	void*& GetHead();
	/**
	 * @brief	���X�g�̐擪�v�f��Ԃ��܂��B���X�g����̂Ƃ��͎g�p�ł��܂���B
	 * @return ���X�g�v�f�ւ̃R�s�[
	 */
	void* GetHead() const;
	/**
	 * @brief	���X�g�̖����v�f��Ԃ��܂��B���X�g����̂Ƃ��͎g�p�ł��܂���B
	 * @return ���X�g�v�f�ւ̎Q��
	 */
	void*& GetTail();
	/**
	 * @brief	���X�g�̖����v�f��Ԃ��܂��B���X�g����̂Ƃ��͎g�p�ł��܂���B
	 * @return ���X�g�v�f�ւ̃R�s�[
	 */
	void* GetTail() const;

	
	/**
	 * @brief	���X�g�̐擪�v�f���폜���܂��B
	 * @return ���X�g�̐擪�v�f���폜���A���̗v�f�ւ̃|�C���^��Ԃ��܂��B
	 */
	void* RemoveHead();
	/**
	 * @brief	���X�g�̖����v�f���폜���܂��B
	 * @return ���X�g�̖����v�f���폜���A���̗v�f�ւ̃|�C���^��Ԃ��܂��B
	 */
	void* RemoveTail();

	
	/**
	 * @brief�v�f(�܂��́A���̃��X�g���̂��ׂĂ̗v�f)�����X�g�̐擪�ɒǉ����āA�V�����擪�v�f�ɂ��܂��B
	 * @param[in] newElement �V�����v�f�B
	 * @return �ŏ��̌`���̊֐��́A�V�����}�������v�f��XPOSITION�l��Ԃ��܂��B
	 */
	XPOSITION AddHead(void* newElement);
	/**
	 * @brief	�v�f(�܂��́A���̃��X�g���̂��ׂĂ̗v�f)�����X�g�̖����ɒǉ����āA�V���������v�f�ɂ��܂��B
	 * @param[in] newElement �V�����v�f�B
	 * @return �ŏ��̌`���̊֐��́A�V�����}�������v�f��XPOSITION�l��Ԃ��܂��B
	 */
	XPOSITION AddTail(void* newElement);

	/**
	 * @brief	�v�f(�܂��́A���̃��X�g���̂��ׂĂ̗v�f)�����X�g�̐擪�ɒǉ����āA�V�����擪�v�f�ɂ��܂��B
	 * @param[in] pNewList ���X�g�ɒǉ�����v�f�B
	 */
	void AddHead(XList* pNewList);
	/**
	 * @brief	�v�f(�܂��́A���̃��X�g���̂��ׂĂ̗v�f)�����X�g�̖����ɒǉ����āA�V���������v�f�ɂ��܂��B
	 * @param[in] pNewList ����XList���X�g�ւ̃|�C���^�BpNewList �̗v�f�����ׂă��X�g�ɒǉ����܂��B
	 * @return �ŏ��̌`���̊֐��́A�V�����}�������v�f��XPOSITION�l��Ԃ��܂��B
	 */
	void AddTail(XList* pNewList);



	/**
	 * @brief	���X�g���炷�ׂĂ̗v�f���폜���܂��B
	 */
	void RemoveAll();




	/**
	 * @brief���X�g�̐擪�v�f�̈ʒu��Ԃ��܂��B
	 * @return ���A�N�Z�X���܂��̓I�u�W�F�N�g�̃|�C���^�̎擾���Ɏg��XPOSITION�l��Ԃ��܂��B���X�g����̏ꍇ��NULL��Ԃ��܂��B
	 */
	XPOSITION GetHeadPosition() const;
	/**
	 * @brief	���X�g�̖����v�f�̈ʒu��Ԃ��܂��B
	 * @return ���A�N�Z�X���܂��̓I�u�W�F�N�g�̃|�C���^�̎擾���Ɏg��XPOSITION�l��Ԃ��܂��B���X�g����̏ꍇ��NULL��Ԃ��܂��B
	 */
	XPOSITION GetTailPosition() const;
	/**
	 * @brief	�����A�N�Z�X���̎��̗v�f���擾���܂��B
	 * @param[in] rPosition ���O��GetNext�AGetHeadPosition�Ȃǂ̑��̃����o�֐��̌Ăяo���ŕԂ��ꂽXPOSITION�l�ւ̎Q�ƁB
	 * @return ���̗v�f�̎Q�Ƃ�Ԃ��܂��B
	 */
	void*& GetNext(XPOSITION& rPosition);
	/**
	 * @brief	�����A�N�Z�X���̎��̗v�f���擾���܂��B
	 * @param[in] rPosition ���O��GetNext�AGetHeadPosition�Ȃǂ̑��̃����o�֐��̌Ăяo���ŕԂ��ꂽXPOSITION�l�ւ̃R�s�[�B
	 * @return ���̗v�f�̃R�s�[��Ԃ��܂��B
	 */
	void* GetNext(XPOSITION& rPosition) const;
	/**
	 * @brief	�����A�N�Z�X���̒��O�̗v�f���擾���܂��B
	 * @param[in] rPosition ���O��GetPrev�Ȃǂ̑��̃����o�֐��̌Ăяo���ŕԂ��ꂽXPOSITION�l�ւ̎Q�ƁB
	 * @return �O�̗v�f�̎Q�Ƃ�Ԃ��܂��B
	 */
	void*& GetPrev(XPOSITION& rPosition);
	/**
	 * @brief	�����A�N�Z�X���̒��O�̗v�f���擾���܂��B
	 * @param[in] rPosition ���O��GetPrev�Ȃǂ̑��̃����o�֐��̌Ăяo���ŕԂ��ꂽXPOSITION�l�ւ̃R�s�[�B
	 * @return �O�̗v�f�̃R�s�[��Ԃ��܂��B
	 */
	void* GetPrev(XPOSITION& rPosition) const;



	/**
	 * @brief	�w�肵���ʒu�̗v�f���擾���܂��B
	 * @param[in] position XPOSITION�l�B
	 * @return ���X�g�v�f�ւ̎Q��
	 */
	void*& GetAt(XPOSITION position);
	/**
	 * @brief	�w�肵���ʒu�̗v�f���擾���܂��B
	 * @param[in] position XPOSITION�l�B
	 * @return ���X�g�v�f�ւ̃R�s�[
	 */
	void* GetAt(XPOSITION position) const;
	/**
	 * @brief	�w�肵���ʒu�ɗv�f��ݒ肵�܂��B
	 * @param[in] pos XPOSITION�l�B
	 * @param[in] newElement ���X�g�ɒǉ�����v�f
	 */
	void SetAt(XPOSITION pos, void* newElement);
	/**
	 * @brief	�w�肵���ʒu�̗v�f�����X�g����폜���܂�
	 * @param[in] position XPOSITION�l�B
	 */
	void RemoveAt(XPOSITION position);

	/**
	 * @brief	�w�肵���ʒu�̒��O�ɐV�����v�f��}�����܂�
	 * @param[in] position XPOSITION�l�B
	 * @param[in] newElement ���X�g�ɒǉ�����v�f
	 * @return �����A�N�Z�X���܂��̓��X�g�v�f�̎擾���Ɏg��XPOSITION�l��Ԃ��܂��B���X�g����̏ꍇ��NULL��Ԃ��܂��B
	 */
	XPOSITION InsertBefore(XPOSITION position, void* newElement);
	/**
	 * @brief	�w�肵���ʒu�̒���ɐV�����v�f��}�����܂�
	 * @param[in] position XPOSITION�l�B
	 * @param[in] newElement ���X�g�ɒǉ�����v�f
	 * @return �����A�N�Z�X���܂��̓��X�g�v�f�̎擾���Ɏg��XPOSITION�l��Ԃ��܂��B���X�g����̏ꍇ��NULL��Ԃ��܂��B
	 */
	XPOSITION InsertAfter(XPOSITION position, void* newElement);


	/**
	 * @brief	�|�C���^�l�Ŏw�肵���v�f�̈ʒu���擾���܂��B
	 * @param[in] searchValue ���X�g�Ō�������l
	 * @param[in] startAfter �����J�n�ʒu
	 * @return �����A�N�Z�X���܂��̓��X�g�v�f�̎擾���Ɏg��XPOSITION�l��Ԃ��܂��B������Ȃ��ꍇ��NULL��Ԃ��܂��B
	 */
	XPOSITION Find(void* searchValue, XPOSITION startAfter = NULL) const;
	/**
	 * @brief	0����n�܂�C���f�b�N�X�Ŏw�肵���v�f�̈ʒu���擾���܂��B
	 * @param[in] nIndex �������郊�X�g�v�f�̈ʒu������0����n�܂�C���f�b�N�X�l�B
	 * @return �����A�N�Z�X���܂��̓��X�g�v�f�̎擾���Ɏg��XPOSITION�l��Ԃ��܂��B������Ȃ��ꍇ��NULL��Ԃ��܂��B
	 */
	XPOSITION FindIndex(int nIndex) const;
protected:
	XNode*	m_pNodeHead;	//	�u���b�N�̐擪
	XNode*	m_pNodeTail;	//	�u���b�N�̍Ō�
	int		m_nCount;		//	�A�C�e������
	XNode*	m_pNodeFree;	//	Free
	struct XPlex* m_pBlocks;//	�A�C�e���f�[�^
	int m_nBlockSize;		//	�u���b�N�T�C�Y
	/*
	 * �V�����u���b�N�쐬
	 * @param[in] p1 �쐬����v�f
	 * @param[in] p2 �쐬����v�f
	 * @return �V�����u���b�N
	 */
	XNode* NewNode(XNode* p1, XNode* p2);
	/*
	 * �u���b�N�J��
	 * @param[in] �J������v�f
	 */
	void FreeNode(XNode*);
};


// ---implemant---
template<class T>
inline XArray<T>::XArray() 
: m_aT(NULL), m_nSize(0), m_nAllocSize(0),m_AutoDelete(false)
{ }
template<class T>
XArray<T>::~XArray()
{
	if(m_AutoDelete) {
		for(int i = 0; i < GetSize(); i++) {
			T p = m_aT[i];
			delete p;
		}
	}
	RemoveAll();
}
template<class T>
inline int XArray<T>::GetSize() const
{
	return m_nSize;
}
template<class T>
inline bool XArray<T>::Add(T& t)
{
	if(m_nSize == m_nAllocSize) {
		T* aT;
		int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
		aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
		if(aT == NULL) {
			return false;
		}
		m_nAllocSize = nNewAllocSize;
		m_aT = aT;
	}
	m_nSize++;
	SetAtIndex(m_nSize - 1, t);
	return true;
}
template<class T>
inline bool XArray<T>::Remove(T& t)
{
	int nIndex = Find(t);
	if(nIndex == -1) {
		return false;
	}
	return RemoveAt(nIndex);
}
template<class T>
inline bool XArray<T>::RemoveAt(int nIndex)
{
	if(nIndex != (m_nSize - 1)){
		::memmove((void*)&m_aT[nIndex], (void*)&m_aT[nIndex + 1], (m_nSize - (nIndex + 1)) * sizeof(T));
	}
	m_nSize--;
	return true;
}
template<class T>
inline void XArray<T>::RemoveAll()
{
	if(m_aT != NULL) {
		free(m_aT);
		m_aT = NULL;
	}
	m_nSize = 0;
	m_nAllocSize = 0;
}

template<class T>
inline T& XArray<T>::operator[] (int nIndex) const
{
	return m_aT[nIndex];
}
template<class T>
inline T* XArray<T>::GetData() const
{
	return m_aT;
}
template<class T>
inline void XArray<T>::SetAtIndex(int nIndex, T& t)
{
	m_aT[nIndex] = t;
}
template<class T>
inline int XArray<T>::Find(T& t) const
{
	for(int i = 0; i < m_nSize; i++) {
		if(m_aT[i] == t)
			return i;
	}
	return -1;
}


#include "xlib/xlib_tail.h"
#endif // __X_ARRAY_HEASER_FILE__

