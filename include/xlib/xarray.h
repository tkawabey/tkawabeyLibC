/**
 * @file xarray.h
 * @brief 配列クラス
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
 * 配列クラス＜テンプレート＞
 * T 配列に格納されるオブジェクトの型
 */
template <class T>
class XArray
{
public:
	/* データ **/
	T* m_aT;
	/* サイズ **/
	int m_nSize;
	/* メモリーサイズ **/
	int m_nAllocSize;
	/* 自動削除 **/
	bool m_AutoDelete;


	/** コンストラクタ */
	XArray();
	/** デストラクタ */
	virtual ~XArray();

	/**
	 * 配列のサイズを取得します
	 * @return 配列のサイズ
	 */
	int GetSize() const;
	/**
	 * 配列に要素を追加します
	 * @param t 追加する要素
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Add(T& t);
	/**
	 * 配列に要素から、指定したアドレスのデータを削除します
	 * @param t 削除する要素
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Remove(T& t);
	/**
	 * 配列に要素から、指定したアドレスのデータを削除します
	 * @param nIndex 削除するインデックス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool RemoveAt(int nIndex);
	/** 配列に要素から、指定したアドレスのデータを削除します */
	void RemoveAll();
	/**
	 * 配列に要素から、指定したアドレスのデータを削除します
	 * @param nIndex 取得するインデックス
	 * @return 指定したインデックスのデータ
	 */
	T& operator[] (int nIndex) const;
	/**
	 * 配列のデータを取得します
	 * @return 配列のアドレス
	 */
	T* GetData() const;
	/**
	 * 指定したインデックスに、要素をセットします
	 * @param nIndex セットするインデックス
	 * @param t セットする要素のアドレス
	 */
	void SetAtIndex(int nIndex, T& t);
	/**
	 * 指定したインデックスに、要素をセットします
	 * @param t 検索する要素のアドレス
	 * @return 見つかったインデックス 見つからなかった場合は-1
	 */
	int Find(T& t) const;
};

/** リストアクセス用ヘルパー構造体 */
struct __XPOSITION { };
/** リストアクセス用ヘルパー構造体 */
typedef __XPOSITION* XPOSITION; 

/**
 * リストアクセス用ヘルパー構造体
 */
struct XPlex
{
	XPlex* pNext;
	void* data(){ return this +1;}
	static XPlex* Create(XPlex*& pHead, unsigned int nMax, unsigned int cbElement);
	void FreeDataChain();
};

/**	@brief	リストクラス */
class XLIB_API XList
{
protected:
	/* @brief ブロック */
	struct XNode {
		XNode* pNext;	// 次のブロック
		XNode* pPrev;	// 前のブロック
		void* data;		// ブロックのデータ
	};
public:
	/**
	 * @brief	コンストラクタ
	 * @param[in] nBlockSize 初期ブロックサイズ
	 */
	XList(int  nBlockSize = 10);
	/** @brief	デストラクタ */
	virtual ~XList(){RemoveAll();}

	
	/**
	 * @brief	リストの要素数を返します。
	 * @return リストの要素数
	 */
	int GetCount() const;
	/**
	 * @brief	リストの状態が空(要素がない)かどうかを調べます。
	 * @retval true 空
	 * @retval false 空でない
	 */
	bool IsEmpty() const;


	/**
	 * @brief	リストの先頭要素を返します。リストが空のときは使用できません。
	 * @return リスト要素への参照
	 */
	void*& GetHead();
	/**
	 * @brief	リストの先頭要素を返します。リストが空のときは使用できません。
	 * @return リスト要素へのコピー
	 */
	void* GetHead() const;
	/**
	 * @brief	リストの末尾要素を返します。リストが空のときは使用できません。
	 * @return リスト要素への参照
	 */
	void*& GetTail();
	/**
	 * @brief	リストの末尾要素を返します。リストが空のときは使用できません。
	 * @return リスト要素へのコピー
	 */
	void* GetTail() const;

	
	/**
	 * @brief	リストの先頭要素を削除します。
	 * @return リストの先頭要素を削除し、その要素へのポインタを返します。
	 */
	void* RemoveHead();
	/**
	 * @brief	リストの末尾要素を削除します。
	 * @return リストの末尾要素を削除し、その要素へのポインタを返します。
	 */
	void* RemoveTail();

	
	/**
	 * @brief要素(または、他のリスト内のすべての要素)をリストの先頭に追加して、新しい先頭要素にします。
	 * @param[in] newElement 新しい要素。
	 * @return 最初の形式の関数は、新しく挿入した要素のXPOSITION値を返します。
	 */
	XPOSITION AddHead(void* newElement);
	/**
	 * @brief	要素(または、他のリスト内のすべての要素)をリストの末尾に追加して、新しい末尾要素にします。
	 * @param[in] newElement 新しい要素。
	 * @return 最初の形式の関数は、新しく挿入した要素のXPOSITION値を返します。
	 */
	XPOSITION AddTail(void* newElement);

	/**
	 * @brief	要素(または、他のリスト内のすべての要素)をリストの先頭に追加して、新しい先頭要素にします。
	 * @param[in] pNewList リストに追加する要素。
	 */
	void AddHead(XList* pNewList);
	/**
	 * @brief	要素(または、他のリスト内のすべての要素)をリストの末尾に追加して、新しい末尾要素にします。
	 * @param[in] pNewList 他のXListリストへのポインタ。pNewList の要素をすべてリストに追加します。
	 * @return 最初の形式の関数は、新しく挿入した要素のXPOSITION値を返します。
	 */
	void AddTail(XList* pNewList);



	/**
	 * @brief	リストからすべての要素を削除します。
	 */
	void RemoveAll();




	/**
	 * @briefリストの先頭要素の位置を返します。
	 * @return 次アクセス時またはオブジェクトのポインタの取得時に使うXPOSITION値を返します。リストが空の場合はNULLを返します。
	 */
	XPOSITION GetHeadPosition() const;
	/**
	 * @brief	リストの末尾要素の位置を返します。
	 * @return 次アクセス時またはオブジェクトのポインタの取得時に使うXPOSITION値を返します。リストが空の場合はNULLを返します。
	 */
	XPOSITION GetTailPosition() const;
	/**
	 * @brief	反復アクセス時の次の要素を取得します。
	 * @param[in] rPosition 直前のGetNext、GetHeadPositionなどの他のメンバ関数の呼び出しで返されたXPOSITION値への参照。
	 * @return 次の要素の参照を返します。
	 */
	void*& GetNext(XPOSITION& rPosition);
	/**
	 * @brief	反復アクセス時の次の要素を取得します。
	 * @param[in] rPosition 直前のGetNext、GetHeadPositionなどの他のメンバ関数の呼び出しで返されたXPOSITION値へのコピー。
	 * @return 次の要素のコピーを返します。
	 */
	void* GetNext(XPOSITION& rPosition) const;
	/**
	 * @brief	反復アクセス時の直前の要素を取得します。
	 * @param[in] rPosition 直前のGetPrevなどの他のメンバ関数の呼び出しで返されたXPOSITION値への参照。
	 * @return 前の要素の参照を返します。
	 */
	void*& GetPrev(XPOSITION& rPosition);
	/**
	 * @brief	反復アクセス時の直前の要素を取得します。
	 * @param[in] rPosition 直前のGetPrevなどの他のメンバ関数の呼び出しで返されたXPOSITION値へのコピー。
	 * @return 前の要素のコピーを返します。
	 */
	void* GetPrev(XPOSITION& rPosition) const;



	/**
	 * @brief	指定した位置の要素を取得します。
	 * @param[in] position XPOSITION値。
	 * @return リスト要素への参照
	 */
	void*& GetAt(XPOSITION position);
	/**
	 * @brief	指定した位置の要素を取得します。
	 * @param[in] position XPOSITION値。
	 * @return リスト要素へのコピー
	 */
	void* GetAt(XPOSITION position) const;
	/**
	 * @brief	指定した位置に要素を設定します。
	 * @param[in] pos XPOSITION値。
	 * @param[in] newElement リストに追加する要素
	 */
	void SetAt(XPOSITION pos, void* newElement);
	/**
	 * @brief	指定した位置の要素をリストから削除します
	 * @param[in] position XPOSITION値。
	 */
	void RemoveAt(XPOSITION position);

	/**
	 * @brief	指定した位置の直前に新しい要素を挿入します
	 * @param[in] position XPOSITION値。
	 * @param[in] newElement リストに追加する要素
	 * @return 順次アクセス時またはリスト要素の取得時に使うXPOSITION値を返します。リストが空の場合はNULLを返します。
	 */
	XPOSITION InsertBefore(XPOSITION position, void* newElement);
	/**
	 * @brief	指定した位置の直後に新しい要素を挿入します
	 * @param[in] position XPOSITION値。
	 * @param[in] newElement リストに追加する要素
	 * @return 順次アクセス時またはリスト要素の取得時に使うXPOSITION値を返します。リストが空の場合はNULLを返します。
	 */
	XPOSITION InsertAfter(XPOSITION position, void* newElement);


	/**
	 * @brief	ポインタ値で指定した要素の位置を取得します。
	 * @param[in] searchValue リストで検索する値
	 * @param[in] startAfter 検索開始位置
	 * @return 順次アクセス時またはリスト要素の取得時に使うXPOSITION値を返します。見つからない場合はNULLを返します。
	 */
	XPOSITION Find(void* searchValue, XPOSITION startAfter = NULL) const;
	/**
	 * @brief	0から始まるインデックスで指定した要素の位置を取得します。
	 * @param[in] nIndex 検索するリスト要素の位置を示す0から始まるインデックス値。
	 * @return 順次アクセス時またはリスト要素の取得時に使うXPOSITION値を返します。見つからない場合はNULLを返します。
	 */
	XPOSITION FindIndex(int nIndex) const;
protected:
	XNode*	m_pNodeHead;	//	ブロックの先頭
	XNode*	m_pNodeTail;	//	ブロックの最後
	int		m_nCount;		//	アイテムする
	XNode*	m_pNodeFree;	//	Free
	struct XPlex* m_pBlocks;//	アイテムデータ
	int m_nBlockSize;		//	ブロックサイズ
	/*
	 * 新しいブロック作成
	 * @param[in] p1 作成する要素
	 * @param[in] p2 作成する要素
	 * @return 新しいブロック
	 */
	XNode* NewNode(XNode* p1, XNode* p2);
	/*
	 * ブロック開放
	 * @param[in] 開放する要素
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

