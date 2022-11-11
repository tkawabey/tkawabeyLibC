#include "xlib/xarray.h"
#include "xlib/xeception.h"



////////////////////////////////////////////////////////////
//
//	XPlex
//
////////////////////////////////////////////////////////////
XPlex* XPlex::Create(XPlex*& pHead, unsigned int nMax, unsigned int cbElement)
{
XEXCEPT_ENTRY
	_XASSERT(nMax > 0 && cbElement > 0);
	XPlex* p = (XPlex*) new unsigned char[sizeof(XPlex) + nMax * cbElement];
			// may throw exception
	p->pNext = pHead;
	pHead = p;  // change head (adds in reverse order for simplicity)
	return p;
XEXCEPT_EXIT
}
void XPlex::FreeDataChain()
{
XEXCEPT_ENTRY
	XPlex* p = this;
	while (p != NULL)
	{
		unsigned char* bytes = (unsigned char*) p;
		XPlex* pNext = p->pNext;
		delete[] bytes;
		p = pNext;
	}
XEXCEPT_EXIT
}


////////////////////////////////////////////////////////////
//
//	XList
//
////////////////////////////////////////////////////////////
int XList::GetCount() const
{
XEXCEPT_ENTRY
	return 	m_nCount;
XEXCEPT_EXIT
}
bool XList::IsEmpty() const
{
XEXCEPT_ENTRY
	return m_nCount == 0;
XEXCEPT_EXIT
}

// peek at head or tail
void*& XList::GetHead()
{
XEXCEPT_ENTRY
	return m_pNodeHead->data;
XEXCEPT_EXIT
}
void* XList::GetHead() const
{
XEXCEPT_ENTRY
	return m_pNodeHead->data;
XEXCEPT_EXIT
}
void*& XList::GetTail()
{
XEXCEPT_ENTRY
	return m_pNodeTail->data;
XEXCEPT_EXIT
}
void* XList::GetTail() const
{
XEXCEPT_ENTRY
	return m_pNodeTail->data;
XEXCEPT_EXIT
}


XList::XList(int  nBlockSize )
{
XEXCEPT_ENTRY
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
XEXCEPT_EXIT
}

void* XList::RemoveHead()
{
XEXCEPT_ENTRY
	_XASSERT(m_pNodeHead != NULL);  // don't call on empty list !!!


	XNode* pOldNode = m_pNodeHead;
	void* returnValue = pOldNode->data;

	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = NULL;
	else
		m_pNodeTail = NULL;
	FreeNode(pOldNode);
	return returnValue;
XEXCEPT_EXIT
}

void* XList::RemoveTail()
{
XEXCEPT_ENTRY

	_XASSERT(m_pNodeTail != NULL);  // don't call on empty list !!!

	XNode* pOldNode = m_pNodeTail;
	void* returnValue = pOldNode->data;

	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = NULL;
	else
		m_pNodeHead = NULL;
	FreeNode(pOldNode);
	return returnValue;

XEXCEPT_EXIT
}

XPOSITION XList::AddHead(void* newElement)
{
XEXCEPT_ENTRY
	XNode* pNewNode = NewNode(NULL, m_pNodeHead);
	pNewNode->data = newElement;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = pNewNode;
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;
	return (XPOSITION) pNewNode;

XEXCEPT_EXIT
}

XPOSITION XList::AddTail(void* newElement)
{
XEXCEPT_ENTRY
	XNode* pNewNode = NewNode(m_pNodeTail, NULL);
	pNewNode->data = newElement;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;
	m_pNodeTail = pNewNode;
	return (XPOSITION) pNewNode;

XEXCEPT_EXIT
}

void XList::AddHead(XList* pNewList)
{
XEXCEPT_ENTRY
	_XASSERT(pNewList != NULL);

	// add a list of same elements to head (maintain order)
	XPOSITION pos = pNewList->GetTailPosition();
	while (pos != NULL)
		AddHead(pNewList->GetPrev(pos));
XEXCEPT_EXIT
}

void XList::AddTail(XList* pNewList)
{
XEXCEPT_ENTRY
	_XASSERT(pNewList != NULL);

	// add a list of same elements
	XPOSITION pos = pNewList->GetHeadPosition();
	while (pos != NULL)
		AddTail(pNewList->GetNext(pos));
XEXCEPT_EXIT
}


// iteration
XPOSITION XList::GetHeadPosition() const
{
XEXCEPT_ENTRY
	return (XPOSITION)m_pNodeHead;
XEXCEPT_EXIT
}
XPOSITION XList::GetTailPosition() const
{
XEXCEPT_ENTRY
	return (XPOSITION)m_pNodeTail;
XEXCEPT_EXIT
}
// return *Position++
void*& XList::GetNext(XPOSITION& rPosition)
{
XEXCEPT_ENTRY
	XNode* pNode = (XNode*) rPosition;
	rPosition = (XPOSITION) pNode->pNext;
	return pNode->data;
XEXCEPT_EXIT
}
// return *Position++
void* XList::GetNext(XPOSITION& rPosition) const
{
XEXCEPT_ENTRY
	XNode* pNode = (XNode*) rPosition;
	rPosition = (XPOSITION) pNode->pNext;
	return pNode->data;
XEXCEPT_EXIT
}
// return *Position--
void*& XList::GetPrev(XPOSITION& rPosition)
{
XEXCEPT_ENTRY
	XNode* pNode = (XNode*) rPosition;
	rPosition = (XPOSITION) pNode->pPrev;
	return pNode->data;
XEXCEPT_EXIT
}
// return *Position--
void* XList::GetPrev(XPOSITION& rPosition) const
{
XEXCEPT_ENTRY
	XNode* pNode = (XNode*) rPosition;
	rPosition = (XPOSITION) pNode->pPrev;
	return pNode->data;
XEXCEPT_EXIT
}
// getting/modifying an element at a given position
void*& XList::GetAt(XPOSITION position)
{
XEXCEPT_ENTRY
	XNode* pNode = (XNode*) position;
	return pNode->data;
XEXCEPT_EXIT
}
void* XList::GetAt(XPOSITION position) const
{
XEXCEPT_ENTRY
	XNode* pNode = (XNode*) position;
	return pNode->data;
XEXCEPT_EXIT
}
void XList::SetAt(XPOSITION pos, void* newElement)
{
XEXCEPT_ENTRY
	XNode* pNode = (XNode*) pos;
	pNode->data = newElement;
XEXCEPT_EXIT
}


void XList::RemoveAll()
{
XEXCEPT_ENTRY
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
XEXCEPT_EXIT
}

void XList::RemoveAt(XPOSITION position)
{
XEXCEPT_ENTRY
	XNode* pOldNode = (XNode*) position;

	// remove pOldNode from list
	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
	}
	else
	{
		pOldNode->pPrev->pNext = pOldNode->pNext;
	}
	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
	}
	else
	{
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}
	FreeNode(pOldNode);
XEXCEPT_EXIT
}

XPOSITION XList::InsertBefore(XPOSITION position, void* newElement)
{
XEXCEPT_ENTRY
	if (position == NULL)
		return AddHead(newElement); // insert before nothing -> head of the list

	// Insert it before position
	XNode* pOldNode = (XNode*) position;
	XNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = newElement;

	if (pOldNode->pPrev != NULL)
	{
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		_XASSERT(pOldNode == m_pNodeHead);
		m_pNodeHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;
	return (XPOSITION) pNewNode;
XEXCEPT_EXIT
}
XPOSITION XList::InsertAfter(XPOSITION position, void* newElement)
{
XEXCEPT_ENTRY
	if (position == NULL)
		return AddTail(newElement); // insert after nothing -> tail of the list

	// Insert it before position
	XNode* pOldNode = (XNode*) position;
	XNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = newElement;

	if (pOldNode->pNext != NULL)
	{
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		_XASSERT(pOldNode == m_pNodeTail);
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	return (XPOSITION) pNewNode;
XEXCEPT_EXIT
}

XPOSITION XList::Find(void* searchValue, XPOSITION startAfter) const
{
XEXCEPT_ENTRY
	XNode* pNode = (XNode*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead;  // start at head
	}
	else
	{
		pNode = pNode->pNext;  // start after the one specified
	}

	for (; pNode != NULL; pNode = pNode->pNext)
		if (pNode->data == searchValue)
			return (XPOSITION) pNode;
	return NULL;
XEXCEPT_EXIT
}
XPOSITION XList::FindIndex(int nIndex) const
{
XEXCEPT_ENTRY
	if (nIndex >= m_nCount || nIndex < 0)
		return NULL;  // went too far

	XNode* pNode = m_pNodeHead;
	while (nIndex--) {
		pNode = pNode->pNext;
	}
	return (XPOSITION) pNode;
XEXCEPT_EXIT
}
void XList::FreeNode(XList::XNode* pNode)
{
XEXCEPT_ENTRY
	pNode->pNext = m_pNodeFree;
	m_pNodeFree = pNode;
	m_nCount--;
	_XASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
XEXCEPT_EXIT
}
XList::XNode* XList::NewNode(XList::XNode* pPrev, XList::XNode* pNext)
{
XEXCEPT_ENTRY
	if (m_pNodeFree == NULL)
	{
		// add another block
		XPlex* pNewBlock = XPlex::Create(m_pBlocks, m_nBlockSize,
				 sizeof(XNode));

		// chain them into free list
		XNode* pNode = (XNode*) pNewBlock->data();
		// free in reverse order to make it easier to debug
		pNode += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
		}
	}
	_XASSERT(m_pNodeFree != NULL);  // we must have something

	XList::XNode* pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;
	_XASSERT(m_nCount > 0);  // make sure we don't overflow


	pNode->data = 0; // start with zero

	return pNode;
XEXCEPT_EXIT
}




