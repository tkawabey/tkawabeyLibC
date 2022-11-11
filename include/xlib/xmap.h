/**
 * @file xmap.h
 * @brief �n�b�V���}�b�v�N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_MAP_HEASER_FILE__
#define __X_MAP_HEASER_FILE__
#include "xlib/xlib_head.h"
#include <stdlib.h>
#include "xarray.h"

/**	@brief	�}�b�v�A�h���X�N���X*/
class XLIB_API XMapAddr
{
public:
	XMapAddr();
	virtual ~XMapAddr();

	void clear();
	int count() const;
	bool isExist(unsigned long first);
	void add(unsigned long first, void* pSecond);
	bool get(unsigned long first, void** ppSecond);
	bool getIndex(int index, unsigned long* pFirst, void** ppSecond);
	bool del(unsigned long first);
private:
	struct _tafData
	{
		unsigned long	m_first;
		void*			m_second;
	};
	XList	m_list;
};


/**	@brief	�n�b�V���}�b�v�N���X
 *		- KEY	�}�b�v�̃L�[�Ƃ��Ďg���I�u�W�F�N�g�̃N���X�B
 *		- ARG_KEY KEY �����Ƃ��Ďg���f�[�^�^�B�ʏ�AKEY �ւ̎Q�Ƃł��B
 *		- VALUE �}�b�v�Ɋi�[�����I�u�W�F�N�g�̃N���X�B
 *		- ARG_VALUE VALUE �����Ƃ��Ďg���f�[�^�^�B�ʏ�AVALUE �ւ̎Q�Ƃł�
 */
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class XMap
{
protected:
	/* �}�b�v�f�[�^ */
	struct XAssoc {
		/* ���̃}�b�v�f�[�^ */
		XAssoc* pNext;
		/** �n�b�V���l */
		unsigned int nHashValue;
		/* �L�[ */
		KEY key;
		/* �l */
		VALUE value;
	};
public:
	/**
	 * �R���X�g���N�^
	 * @param nBlockSize �����u���b�N�T�C�Y
	 */
	XMap(int nBlockSize = 10);
	/**
	 * �}�b�v���̗v�f����Ԃ��܂��B
	 * @return �}�b�v���̗v�f��
	 */
	int GetCount() const;
	/**
	 * �}�b�v����̏�Ԃ��ǂ����𒲂ׂ܂��B
	 * @retval true ��
	 * @retval false ��łȂ�
	 */
	bool IsEmpty() const;

	/**
	 * �w�肵���L�[�Ƀ}�b�v����Ă���l���������܂�
	 * @param[in] key ��������v�f�����ʂ���L�[ 
	 * @param[in,out] rValue ���������l���󂯎��܂� 
	 * @retval true ��������
	 * @retval false ������Ȃ�
	 */
	bool Lookup(ARG_KEY key, VALUE& rValue) const;
	/**
	 * �}�b�v�ɗv�f��}�����܂��Boperator��SetAt�֐��̑���Ɏg���܂��B 
	 * @param[in] key ��������v�f�����ʂ���L�[ 
	 * @return �V�����v�f�̒l�B
	 */
	VALUE& operator[](ARG_KEY key);
	/**
	 * �}�b�v�ɗv�f��}�����܂��B 
	 * @param[in] key ��������v�f�����ʂ���L�[ 
	 * @param[in] newValue �V�����v�f�̒l�B
	 */
	void SetAt(ARG_KEY key, ARG_VALUE newValue);
	/**
	 * �L�[�Ŏw�肳�ꂽ�v�f���폜���܂� 
	 * @param[in] key ��������v�f�����ʂ���L�[ 
	 * @retval true ����
	 * @retval false ���s
	 */
	bool RemoveKey(ARG_KEY key);
	/**
	 * �}�b�v���̂��ׂĂ̗v�f���폜���܂��B
	 */
	void RemoveAll();

	/**
	 * �ŏ��̗v�f�̈ʒu��Ԃ��܂�
	 * @return �}�b�v�������A�N�Z�X����Ƃ��̊J�n�ʒu������POSITION�l��Ԃ��܂�
	 */
	XPOSITION GetStartPosition() const;
	/**
	 * ���̗v�f�����ԂɎ擾���܂�
	 * @param[in,out] rNextPosition ���O�ɌĂяo����GetNextAssoc�֐��܂���GetStartPosition�֐����Ԃ���POSITION�l�ւ̎Q�ƁB
	 * @param[in,out] rKey �擾�����v�f�̃L�[���Ԃ���܂� 
	 * @param[in,out] rValue �擾�����v�f�̒l���Ԃ���܂� 
	 */
	void GetNextAssoc(XPOSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;

	/**
	 * �n�b�V���e�[�u���̃T�C�Y���擾���܂��B
	 * preturn �n�b�V���e�[�u���̃T�C�Y
	 */
	unsigned int GetHashTableSize() const;
	/**
	 * �n�b�V�� �e�[�u�������������T�C�Y���w�肵�܂��B
	 * @param[in] hashSize �n�b�V�� �e�[�u���̃G���g�����B 
	 * @param[in] bAllocNow ���g�p 
	 */
	void InitHashTable(unsigned int hashSize, bool bAllocNow = true);

protected:
	/* �n�b�V���e�[�u�� */
	XAssoc**		m_pHashTable;
	/* �n�b�V���e�[�u���̃T�C�Y */
	unsigned int	m_nHashTableSize;
	/* �v�f�� */
	int				m_nCount;
	/* �n�b�V���e�[�u���̊J���p�|�C���^ */
	XAssoc*			m_pFreeList;
	/* �u���b�N */
	struct XPlex*	m_pBlocks;
	/* �u���b�N�T�C�Y */
	int				m_nBlockSize;

	/* 
	 * �V�����u���b�N���쐬
	 * @return �V�����u���b�N
	 */
	XAssoc* NewAssoc()
	{

		if (m_pFreeList == NULL) {
			XPlex* newBlock = XPlex::Create(m_pBlocks, m_nBlockSize, sizeof(XMap::XAssoc));
			XMap::XAssoc* pAssoc = (XMap::XAssoc*) newBlock->data();
			pAssoc += m_nBlockSize - 1;
			for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--) {
				pAssoc->pNext = m_pFreeList;
				m_pFreeList = pAssoc;
			}
		}
		//	ASSERT(m_pFreeList != NULL);  // we must have something

		XMap::XAssoc* pAssoc = m_pFreeList;
		m_pFreeList = m_pFreeList->pNext;
		m_nCount++;
		//	ASSERT(m_nCount > 0);  // make sure we don't overflow
		_XConstructElements<KEY>(&pAssoc->key, 1);
		_XConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values
		return pAssoc;
	} 
	/* 
	 * �u���b�N�W�J��
	 * @param[in] p �J������u���b�N 
	 */
	void FreeAssoc(XAssoc* pAssoc)
	{

		_XDestructElements<VALUE>(&pAssoc->value, 1);
		_XDestructElements<KEY>(&pAssoc->key, 1);
		pAssoc->pNext = m_pFreeList;
		m_pFreeList = pAssoc;
		m_nCount--;
		//	ASSERT(m_nCount >= 0);  // make sure we don't underflow
		if (m_nCount == 0) {
			RemoveAll();
		}
	} 

	/* 
	 * �u���b�N���擾
	 * @param[in] key �L�[ 
	 * @param[out] v �l
	 * @return �u���b�N
	 */
	XAssoc* GetAssocAt(ARG_KEY key, unsigned int& nHash) const
	{
		nHash = XHashKey<ARG_KEY>(key) % m_nHashTableSize;

		if (m_pHashTable == NULL) {
			return NULL;
		}

		XAssoc* pAssoc;
		for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext) {
			if (_XCompareElements(&pAssoc->key, &key)) {
				return pAssoc;
			}
		}
		return NULL;
	}
public:
	/** �f�X�g���N�^ */
	virtual ~XMap();
};


// ---implemant---
template<class ARG_KEY> unsigned int inline XHashKey(ARG_KEY key)
{
	return ((unsigned int)(void*)(unsigned long)key) >> 4;
}
template<> unsigned int inline  XHashKey<const wchar_t*> (const wchar_t* key)
{
	unsigned int nHash = 0;
	while (*key) {
		nHash = (nHash<<5) + nHash + *key++;
	}
	return nHash;
}
template<> unsigned int inline  XHashKey<const char*> (const char* key)
{
	unsigned int nHash = 0;
	while (*key) {
		nHash = (nHash<<5) + nHash + *key++;
	}
	return nHash;
}


template<class TYPE>
inline void _XConstructElements(TYPE* pElements, int nCount)
{
	::memset((void*)pElements, 0, nCount * sizeof(TYPE));

	for(; nCount--; pElements++) {
		::new((void*)pElements) TYPE;
	}
}
template<class TYPE>
inline void _XDestructElements(TYPE* pElements, int nCount)
{
	for(; nCount--; pElements++) {
		pElements->~TYPE();
	}
}
template<class TYPE, class ARG_TYPE>
inline bool _XCompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	return *pElement1 == *pElement2;
}
/*
template<> inline bool CompareElements<XStrBuff, LPCTSTR>(const XStrBuff* pElement1, const LPCTSTR* pElement2)
{
	return pElement1->Compare(*pElement2) == 0;
}
template<> void ConstructElements<XStrBuff> (XStrBuff* pElements, int nCount)
{
	for (; nCount--; ++pElements)
		memcpy(pElements, &afxEmptyString, sizeof(*pElements));
}
template<> void DestructElements<XStrBuff> (XStrBuff* pElements, int nCount)
{
	for (; nCount--; ++pElements)
		pElements->~XStrBuff();
}
*/

#define XMap_XAssoc XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::XAssoc


template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline int XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetCount() const
	{ return m_nCount; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline bool XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::IsEmpty() const
	{ return m_nCount == 0 ? true : false; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue)
	{ (*this)[key] = newValue; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline XPOSITION XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline unsigned int XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetHashTableSize() const
	{ return m_nHashTableSize; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::XMap(int nBlockSize)
{
	//ASSERT(nBlockSize > 0);
	m_pHashTable = NULL;
	m_nHashTableSize = 17;  // default size
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::InitHashTable(
	unsigned int nHashSize, bool bAllocNow)
{
//	ASSERT_VALID(this);
//	ASSERT(m_nCount == 0);
//	ASSERT(nHashSize > 0);

	if (m_pHashTable != NULL) {
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}

	if (bAllocNow) {
		m_pHashTable = new XAssoc*[nHashSize];
		::memset(m_pHashTable, 0, sizeof(XAssoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveAll()
{
//	ASSERT_VALID(this);
	if (m_pHashTable != NULL) {
		for (unsigned int nHash = 0; nHash < m_nHashTableSize; nHash++) {
			XAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext) {
				_XDestructElements<VALUE>(&pAssoc->value, 1);
				_XDestructElements<KEY>(&pAssoc->key, 1);
			}
		}
	}

	// free hash table
	delete[] m_pHashTable;
	m_pHashTable = NULL;

	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::~XMap()
{
	RemoveAll();
//	ASSERT(m_nCount == 0);
}

#if 0
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
#ifdef WIN32
inline XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::XAssoc* XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::NewAssoc()
#else
inline XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::XAssoc* XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::NewAssoc()
#endif
{
	if (m_pFreeList == NULL) {
		XPlex* newBlock = XPlex::Create(m_pBlocks, m_nBlockSize, sizeof(XMap::XAssoc));
		XMap::XAssoc* pAssoc = (XMap::XAssoc*) newBlock->data();
		pAssoc += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--) {
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
//	ASSERT(m_pFreeList != NULL);  // we must have something

	XMap::XAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
//	ASSERT(m_nCount > 0);  // make sure we don't overflow
	_XConstructElements<KEY>(&pAssoc->key, 1);
	_XConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values
	return pAssoc;
}
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::FreeAssoc(XMap::XAssoc* pAssoc)
{
	_XDestructElements<VALUE>(&pAssoc->value, 1);
	_XDestructElements<KEY>(&pAssoc->key, 1);
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
//	ASSERT(m_nCount >= 0);  // make sure we don't underflow
	if (m_nCount == 0) {
		RemoveAll();
	}
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::XAssoc*  XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetAssocAt(ARG_KEY key, unsigned int& nHash) const
{
	nHash = XHashKey<ARG_KEY>(key) % m_nHashTableSize;

	if (m_pHashTable == NULL) {
		return NULL;
	}

	XAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext) {
		if (CompareElements(&pAssoc->key, &key)) {
			return pAssoc;
		}
	}
	return NULL;
}
#endif

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline bool XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key, VALUE& rValue) const
{
//	ASSERT_VALID(this);

	unsigned int nHash;
	XAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL) {
		return false;  // not in map
	}
	rValue = pAssoc->value;
	return true;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline VALUE& XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator[](ARG_KEY key)
{
//	ASSERT_VALID(this);

	unsigned int nHash;
	XAssoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nHash)) == NULL) {
		if (m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);

		pAssoc = NewAssoc();
		pAssoc->nHashValue = nHash;
		pAssoc->key = key;

		pAssoc->pNext = m_pHashTable[nHash];
		m_pHashTable[nHash] = pAssoc;
	}
	return pAssoc->value;  // return new reference
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline bool XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key)
{
//	ASSERT_VALID(this);

	if (m_pHashTable == NULL)
		return false;  // nothing in the table

	XAssoc** ppAssocPrev;
	ppAssocPrev = &m_pHashTable[XHashKey<ARG_KEY>(key) % m_nHashTableSize];

	XAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext) {
		if(_XCompareElements(&pAssoc->key, &key)) {
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			FreeAssoc(pAssoc);
			return true;
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return false;  // not found
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void XMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(XPOSITION& rNextPosition,
	KEY& rKey, VALUE& rValue) const
{
//	ASSERT_VALID(this);
//	ASSERT(m_pHashTable != NULL);  // never call on empty map

	XAssoc* pAssocRet = (XAssoc*)rNextPosition;
//	ASSERT(pAssocRet != NULL);

	if (pAssocRet == (XAssoc*) BEFORE_START_POSITION) {
		// find the first association
		for (unsigned int nBucket = 0; nBucket < m_nHashTableSize; nBucket++) 
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
//		ASSERT(pAssocRet != NULL);  // must find something
	}

//	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(XAssoc)));
	XAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL) {
		// go to next bucket
		for (unsigned int nBucket = pAssocRet->nHashValue + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (XPOSITION) pAssocNext;

	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}

#include "xlib/xlib_tail.h"
#endif //__X_MAP_HEASER_FILE__



