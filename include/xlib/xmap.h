/**
 * @file xmap.h
 * @brief ハッシュマップクラス
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

/**	@brief	マップアドレスクラス*/
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


/**	@brief	ハッシュマップクラス
 *		- KEY	マップのキーとして使うオブジェクトのクラス。
 *		- ARG_KEY KEY 引数として使うデータ型。通常、KEY への参照です。
 *		- VALUE マップに格納されるオブジェクトのクラス。
 *		- ARG_VALUE VALUE 引数として使うデータ型。通常、VALUE への参照です
 */
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class XMap
{
protected:
	/* マップデータ */
	struct XAssoc {
		/* 次のマップデータ */
		XAssoc* pNext;
		/** ハッシュ値 */
		unsigned int nHashValue;
		/* キー */
		KEY key;
		/* 値 */
		VALUE value;
	};
public:
	/**
	 * コンストラクタ
	 * @param nBlockSize 初期ブロックサイズ
	 */
	XMap(int nBlockSize = 10);
	/**
	 * マップ内の要素数を返します。
	 * @return マップ内の要素数
	 */
	int GetCount() const;
	/**
	 * マップが空の状態かどうかを調べます。
	 * @retval true 空
	 * @retval false 空でない
	 */
	bool IsEmpty() const;

	/**
	 * 指定したキーにマップされている値を検索します
	 * @param[in] key 検索する要素を識別するキー 
	 * @param[in,out] rValue 検索した値を受け取ります 
	 * @retval true 見つかった
	 * @retval false 見つからない
	 */
	bool Lookup(ARG_KEY key, VALUE& rValue) const;
	/**
	 * マップに要素を挿入します。operatorはSetAt関数の代わりに使えます。 
	 * @param[in] key 検索する要素を識別するキー 
	 * @return 新しい要素の値。
	 */
	VALUE& operator[](ARG_KEY key);
	/**
	 * マップに要素を挿入します。 
	 * @param[in] key 検索する要素を識別するキー 
	 * @param[in] newValue 新しい要素の値。
	 */
	void SetAt(ARG_KEY key, ARG_VALUE newValue);
	/**
	 * キーで指定された要素を削除します 
	 * @param[in] key 検索する要素を識別するキー 
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool RemoveKey(ARG_KEY key);
	/**
	 * マップ内のすべての要素を削除します。
	 */
	void RemoveAll();

	/**
	 * 最初の要素の位置を返します
	 * @return マップを順次アクセスするときの開始位置を示すPOSITION値を返します
	 */
	XPOSITION GetStartPosition() const;
	/**
	 * 次の要素を順番に取得します
	 * @param[in,out] rNextPosition 直前に呼び出したGetNextAssoc関数またはGetStartPosition関数が返したPOSITION値への参照。
	 * @param[in,out] rKey 取得した要素のキーが返されます 
	 * @param[in,out] rValue 取得した要素の値が返されます 
	 */
	void GetNextAssoc(XPOSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;

	/**
	 * ハッシュテーブルのサイズを取得します。
	 * preturn ハッシュテーブルのサイズ
	 */
	unsigned int GetHashTableSize() const;
	/**
	 * ハッシュ テーブルを初期化しサイズを指定します。
	 * @param[in] hashSize ハッシュ テーブルのエントリ数。 
	 * @param[in] bAllocNow 未使用 
	 */
	void InitHashTable(unsigned int hashSize, bool bAllocNow = true);

protected:
	/* ハッシュテーブル */
	XAssoc**		m_pHashTable;
	/* ハッシュテーブルのサイズ */
	unsigned int	m_nHashTableSize;
	/* 要素数 */
	int				m_nCount;
	/* ハッシュテーブルの開放用ポインタ */
	XAssoc*			m_pFreeList;
	/* ブロック */
	struct XPlex*	m_pBlocks;
	/* ブロックサイズ */
	int				m_nBlockSize;

	/* 
	 * 新しいブロックを作成
	 * @return 新しいブロック
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
	 * ブロックジ開放
	 * @param[in] p 開放するブロック 
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
	 * ブロックを取得
	 * @param[in] key キー 
	 * @param[out] v 値
	 * @return ブロック
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
	/** デストラクタ */
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



