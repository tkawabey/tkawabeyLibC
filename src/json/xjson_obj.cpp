
#include "xlib/xeception.h"
#include "xlib/json/xjson_obj.h"
#include "xlib/json/xjson_val.h"
#include <map>

//--------------------------------------------------------------------------
//	XJsonObj::Iterator
//--------------------------------------------------------------------------
XJsonObj::Iterator::Iterator()
	:	m_pIte( NULL )
	,	m_pMap( NULL )
{
XEXCEPT_ENTRY
	m_pIte = (void*)new std::map<XStrBuff, XJsonVal>::iterator();
XEXCEPT_EXIT
}


XJsonObj::Iterator::Iterator(const Iterator& right)
	:	m_pIte( NULL )
	,	m_pMap( NULL )
{
XEXCEPT_ENTRY
	m_pIte = (void*)new std::map<XStrBuff, XJsonVal>::iterator();

	std::map<XStrBuff, XJsonVal>::iterator* p_wkDstMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)right.m_pIte;
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	
	_XASSERT( p_wkMapIte );
	_XASSERT( p_wkDstMapIte );

	*p_wkMapIte = *p_wkDstMapIte;
	m_pMap = right.m_pMap;
XEXCEPT_EXIT
}


XJsonObj::Iterator::~Iterator()
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	
	_XASSERT( p_wkMapIte );
	delete p_wkMapIte;
XEXCEPT_EXIT
}


XJsonObj::Iterator &XJsonObj::Iterator::operator=(const Iterator &src)
{
XEXCEPT_ENTRY

	std::map<XStrBuff, XJsonVal>::iterator* p_wkDstMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)src.m_pIte;
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	
	_XASSERT( p_wkMapIte );
	_XASSERT( p_wkDstMapIte );
	
	*p_wkMapIte = *p_wkDstMapIte;
	m_pMap = src.m_pMap;


XEXCEPT_EXIT
	return *this;
}


XJsonObj::Iterator &XJsonObj::Iterator::operator++()
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	
	_XASSERT( p_wkMapIte );
	(*(p_wkMapIte))++;
XEXCEPT_EXIT
	return *this;
}


XJsonObj::Iterator &XJsonObj::Iterator::operator--()
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	
	_XASSERT( p_wkMapIte );
	(*(p_wkMapIte))--;
XEXCEPT_EXIT
	return *this;
}


bool XJsonObj::Iterator::isBegin()
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pMap;
	
	_XASSERT( p_wkMapIte );
	_XASSERT( p_wkMap );
	if( (*(p_wkMapIte)) == p_wkMap->begin() ) {
		return true;
	}
XEXCEPT_EXIT
	return false;
}


bool XJsonObj::Iterator::isEnd()
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pMap;
	
	_XASSERT( p_wkMapIte );
	_XASSERT( p_wkMap );
	if( (*(p_wkMapIte)) != p_wkMap->end() ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
	return false;
}


const XStrBuff& XJsonObj::Iterator::getKey()
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	return (*p_wkMapIte)->first;
XEXCEPT_EXIT
}
XJsonVal XJsonObj::Iterator::getValue()
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)m_pIte;
	return (*p_wkMapIte)->second;
XEXCEPT_EXIT
}
//--------------------------------------------------------------------------
//	XJsonObj
//--------------------------------------------------------------------------
XJsonObj::XJsonObj()
	:	m_pObj( NULL )
{
XEXCEPT_ENTRY

	m_pObj = (void*)new std::map<XStrBuff, XJsonVal>();
XEXCEPT_EXIT
}


XJsonObj::XJsonObj(const XJsonObj& val)
	:	m_pObj( NULL )
{
XEXCEPT_ENTRY
	m_pObj = (void*)new std::map<XStrBuff, XJsonVal>();
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	std::map<XStrBuff, XJsonVal>* p_wkDstMap = (std::map<XStrBuff, XJsonVal>*)val.m_pObj;


	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );


	*p_wkMap = *p_wkDstMap;
XEXCEPT_EXIT
}


XJsonObj::~XJsonObj()
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );
	m_pObj = NULL;
	p_wkMap->clear();
	delete p_wkMap;
XEXCEPT_EXIT
}


bool XJsonObj::add(const XStrBuff& ley, XJsonVal& val)
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );


	p_wkMap->insert(std::pair<XStrBuff, XJsonVal>(ley, val));
	return true;
XEXCEPT_EXIT
	return false;
}


bool XJsonObj::del(const XStrBuff &strKey)
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );

	std::map<XStrBuff, XJsonVal>::iterator ite = p_wkMap->find(strKey);
	if( ite != p_wkMap->end() ) {
		p_wkMap->erase( ite );
		return true;
	}
	
	return false;
XEXCEPT_EXIT
	return false;
}


size_t	XJsonObj::size() const
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );


	return p_wkMap->size();
XEXCEPT_EXIT
	return 0;
}


XJsonObj::Iterator XJsonObj::getIterator()  const
{
XEXCEPT_ENTRY
	Iterator ite;
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );

	std::map<XStrBuff, XJsonVal>::iterator* p_wkMapIte = (std::map<XStrBuff, XJsonVal>::iterator*)ite.m_pIte;
	*p_wkMapIte  = p_wkMap->begin();
	ite.m_pMap = m_pObj;
	return ite;
XEXCEPT_EXIT
}


bool XJsonObj::operator==(const XJsonObj &rhs) const
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	std::map<XStrBuff, XJsonVal>* p_wkDstMap = (std::map<XStrBuff, XJsonVal>*)rhs.m_pObj;

	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap == *p_wkDstMap;
XEXCEPT_EXIT
	return false;
}


bool XJsonObj::operator!=(const XJsonObj &rhs) const
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	std::map<XStrBuff, XJsonVal>* p_wkDstMap = (std::map<XStrBuff, XJsonVal>*)rhs.m_pObj;

	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap != *p_wkDstMap;
XEXCEPT_EXIT
	return false;
}


bool XJsonObj::operator<(const XJsonObj &rhs) const
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	std::map<XStrBuff, XJsonVal>* p_wkDstMap = (std::map<XStrBuff, XJsonVal>*)rhs.m_pObj;

	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap < *p_wkDstMap;
XEXCEPT_EXIT
	return false;
}


bool XJsonObj::operator<=(const XJsonObj &rhs) const
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	std::map<XStrBuff, XJsonVal>* p_wkDstMap = (std::map<XStrBuff, XJsonVal>*)rhs.m_pObj;

	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap <= *p_wkDstMap;
XEXCEPT_EXIT
	return false;
}


bool XJsonObj::operator>(const XJsonObj &rhs) const
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	std::map<XStrBuff, XJsonVal>* p_wkDstMap = (std::map<XStrBuff, XJsonVal>*)rhs.m_pObj;

	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap > *p_wkDstMap;
XEXCEPT_EXIT
	return false;
}


bool XJsonObj::operator>=(const XJsonObj &rhs) const
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	std::map<XStrBuff, XJsonVal>* p_wkDstMap = (std::map<XStrBuff, XJsonVal>*)rhs.m_pObj;

	_XASSERT( p_wkMap );
	_XASSERT( p_wkDstMap );

	return *p_wkMap >= *p_wkDstMap;
XEXCEPT_EXIT
	return false;
}


XJsonVal &XJsonObj::operator[](const char *key)
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );
	return (*p_wkMap)[ XStrBuff(key) ];
XEXCEPT_EXIT
}


XJsonVal &XJsonObj::operator[](const wchar_t *key)
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );
	return (*p_wkMap)[ XStrBuff(key) ];
XEXCEPT_EXIT
}


void XJsonObj::print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth /*= 0*/, bool bNonEscape /*= false*/) const
{
XEXCEPT_ENTRY
	std::map<XStrBuff, XJsonVal>* p_wkMap = (std::map<XStrBuff, XJsonVal>*)m_pObj;
	_XASSERT( p_wkMap );
	size_t		i = 0;


	if( (*p_wkMap).empty() ) {
		stream << _T("{}");
		return ;
	}

	stream << _T("{");
	if( nDepth != ~0 ) {
		stream << std::endl;
		for(i = 0; i < nDepth+1; i++ ) {
			stream << _T("\t");
		}
	}
	for(std::map<XStrBuff, XJsonVal>::const_iterator ite = (*p_wkMap).begin(); ite != (*p_wkMap).end(); ite++ ) {
		if (ite != (*p_wkMap).begin()) {
			stream << _T(",");
			if( nDepth != ~0 ) {
				stream << std::endl;
				for(i = 0; i < nDepth+1; i++ ) {
					stream << _T("\t");
				}
			}
		}
		if( bNonEscape == false ) {
			stream << _T("\"");
			XJsonVal::printEscapeString(stream, (*ite).first );
			stream << _T("\"");
		} else {
			stream << (*ite).first.c_str() ;
		}
		if( nDepth != ~0 ) {
			stream << _T(" : ");
		} else {
			stream << _T(":");
		}

		(*ite).second.print(stream, nDepth != ~0 ? nDepth+1 : nDepth, bNonEscape);

		if( nDepth != ~0 ) {
			stream << _T(" ");
		}
	}
	if( nDepth != ~0 ) {
		stream << std::endl;
		for(i = 0; i < nDepth; i++ ) {
			stream << _T("\t");
		}
	}
	stream << _T("}");
XEXCEPT_EXIT
}
