
#include "xlib/xeception.h"
#include "xlib/json/xjson_array.h"
#include "xlib/json/xjson_val.h"
#include <vector>



XJsonArray::XJsonArray()
	:	m_pObj( NULL )
{
XEXCEPT_ENTRY
	m_pObj = (void*)new std::vector<XJsonVal>();
XEXCEPT_EXIT
}


XJsonArray::XJsonArray(const XJsonArray& val)
	:	m_pObj( NULL )
{
XEXCEPT_ENTRY
	m_pObj = (void*)new std::vector<XJsonVal>();
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	std::vector<XJsonVal>* p_wkDstMap = (std::vector<XJsonVal>*)val.m_pObj;
	
	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	*p_wkMap = *p_wkDstMap ;
XEXCEPT_EXIT
}


XJsonArray::~XJsonArray()
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );
	m_pObj = NULL;
	p_wkMap->clear();
	delete p_wkMap;
XEXCEPT_EXIT
}

	
bool XJsonArray::operator==(const XJsonArray &rhs) const
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	std::vector<XJsonVal>* p_wkDstMap = (std::vector<XJsonVal>*)rhs.m_pObj;
	
	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap == *p_wkDstMap ;
XEXCEPT_EXIT
}


bool XJsonArray::operator!=(const XJsonArray &rhs) const
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	std::vector<XJsonVal>* p_wkDstMap = (std::vector<XJsonVal>*)rhs.m_pObj;
	
	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap != *p_wkDstMap ;
XEXCEPT_EXIT
}


bool XJsonArray::operator<(const XJsonArray &rhs) const
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	std::vector<XJsonVal>* p_wkDstMap = (std::vector<XJsonVal>*)rhs.m_pObj;
	
	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap < *p_wkDstMap ;
XEXCEPT_EXIT
}


bool XJsonArray::operator<=(const XJsonArray &rhs) const
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	std::vector<XJsonVal>* p_wkDstMap = (std::vector<XJsonVal>*)rhs.m_pObj;
	
	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap < *p_wkDstMap || *p_wkMap == *p_wkDstMap ;
XEXCEPT_EXIT
}


bool XJsonArray::operator>(const XJsonArray &rhs) const
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	std::vector<XJsonVal>* p_wkDstMap = (std::vector<XJsonVal>*)rhs.m_pObj;
	
	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap > *p_wkDstMap ;
XEXCEPT_EXIT
}


bool XJsonArray::operator>=(const XJsonArray &rhs) const
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	std::vector<XJsonVal>* p_wkDstMap = (std::vector<XJsonVal>*)rhs.m_pObj;
	
	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap > *p_wkDstMap || *p_wkMap == *p_wkDstMap ;
XEXCEPT_EXIT
}


XJsonVal &XJsonArray::operator[](size_t index)
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	
	_XASSERT( p_wkMap );

	return (*p_wkMap)[index];
XEXCEPT_EXIT
}

size_t XJsonArray::size() const
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	
	_XASSERT( p_wkMap );
	return (*p_wkMap).size();
XEXCEPT_EXIT
}

void XJsonArray::resize(size_t n)
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	
	_XASSERT( p_wkMap );
	(*p_wkMap).resize( n );
XEXCEPT_EXIT
}

void XJsonArray::push_back(XJsonVal& newVal) 
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	
	_XASSERT( p_wkMap );
	(*p_wkMap).push_back( newVal );
XEXCEPT_EXIT
}
void XJsonArray::pop_back() 
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	
	_XASSERT( p_wkMap );
	(*p_wkMap).pop_back();
XEXCEPT_EXIT
}
XJsonVal& XJsonArray::back() 
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	
	_XASSERT( p_wkMap );
	return (*p_wkMap).back();
XEXCEPT_EXIT
}


void XJsonArray::print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth /*= 0*/, bool bNonEscape /*= false*/) const
{
XEXCEPT_ENTRY
	std::vector<XJsonVal>* p_wkMap = (std::vector<XJsonVal>*)m_pObj;
	
	_XASSERT( p_wkMap );

	if( (*p_wkMap).empty() ) {
		stream << _T("[]");
		return ;
	}

	stream << _T("[");
	for(std::vector<XJsonVal>::const_iterator ite = (*p_wkMap).begin(); ite != (*p_wkMap).end(); ite++ ) {
		if (ite != (*p_wkMap).begin()) {
			stream << _T(",");
		}
		(*ite).print(stream, nDepth, bNonEscape);
	}
	stream << _T("]");
XEXCEPT_EXIT
}
