#include "xlib/xmap.h"
#include "xlib/xeception.h"

//
//	XMapAddr
//
XMapAddr::XMapAddr()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XMapAddr::~XMapAddr()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XMapAddr::clear()
{
XEXCEPT_ENTRY
	XPOSITION pos;
	for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
		_tafData* p = (_tafData*)m_list.GetNext( pos );
		delete p;
	}
	m_list.RemoveAll();
XEXCEPT_EXIT
}
int XMapAddr::count() const
{
XEXCEPT_ENTRY
	return m_list.GetCount();
XEXCEPT_EXIT
}
bool XMapAddr::isExist(unsigned long first)
{
XEXCEPT_ENTRY
	XPOSITION pos;
	for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
		_tafData* p = (_tafData*)m_list.GetNext( pos );
		if( p->m_first == first ) {
			return true;
		}
	}
XEXCEPT_EXIT
	return false;
}
void XMapAddr::add(unsigned long first, void* pSecond)
{
XEXCEPT_ENTRY
	_tafData* p = new _tafData;
	p->m_first  = first;
	p->m_second = pSecond;
	m_list.AddTail( p );
XEXCEPT_EXIT
}
bool XMapAddr::get(unsigned long first, void** ppSecond)
{
XEXCEPT_ENTRY

	XPOSITION pos;
	for( pos = m_list.GetHeadPosition(); pos != NULL; ) {
		_tafData* p = (_tafData*)m_list.GetNext( pos );
		if( p->m_first == first ) {
			*ppSecond = p->m_second;
			return true;
		}
	}

	return false;
XEXCEPT_EXIT
}
bool XMapAddr::getIndex(int index, unsigned long* pFirst, void** ppSecond)
{
XEXCEPT_ENTRY
	int cur = 0;
	XPOSITION pos;
	for( pos = m_list.GetHeadPosition(); pos != NULL; cur ) {
		_tafData* p = (_tafData*)m_list.GetNext( pos );
		if( index == cur ) {
			*pFirst = p->m_first;
			*ppSecond = p->m_second;
			return true;
		}
	}

	return true;
XEXCEPT_EXIT
}
bool XMapAddr::del(unsigned long first)
{
XEXCEPT_ENTRY

	XPOSITION pos1, pos2;
	for( pos1 = m_list.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		_tafData* p = (_tafData*)m_list.GetNext( pos1 );
		if( p->m_first == first ) {
			delete p;
			m_list.RemoveAt( pos2 );
			return true;
		}
	}

	return false;
XEXCEPT_EXIT
}
