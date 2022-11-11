#include "xlib/semi/secs/xsecs2.h"
#include "xlib/semi/secs/xsecs_text_parser.h"
#include "xlib/xeception.h"
#include "xlib/xmlhelper.h"




//-----------------------------------------------------------
//
//	XSecs2Items
//
//-----------------------------------------------------------
XSecs2Items::XSecs2Items(XSecs2Obj *pParent)
	:	XSecs2Obj( XSecs2Obj::ITEMS, pParent )
	,	m_pElem( NULL) 
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XSecs2Items::~XSecs2Items()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
XSecs2Item* XSecs2Items::add(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XmlHlpElementActl elem = pElem;

	XStrBuff strErr;

	// 必須属性のチェック
	if( elem.isAttribute(_T("name")) == false ) {

		strErr = _T("attribute(name) not found. path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
		return NULL;
	}
	if( elem.isAttribute(_T("type")) == false ) {
		strErr = _T("attribute(type) not found. path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);

		return NULL;
	}

	XStrBuff	strName = elem.getAttribute(_T("name"));
	XStrBuff	strType = elem.getAttribute(_T("type"));
	XStrBuff	strComment(_T(""));

	if( elem.isAttribute(_T("comment")) == true ) {
		strComment = elem.getAttribute(_T("comment"));
	}

	XDATA_TYPE  dataType = XData::parseDataType_From_String(strType);
	if( dataType == XDATA_TYPE_END ) {
		strErr = _T("invalid type string. type=");
		strErr << strType << _T(" ");
		strErr << _T(" path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
		return NULL;
	}
	XSecs2Item *pNeItem = new XSecs2Item(strName, dataType, this, pElem);
	return pNeItem;
XEXCEPT_EXIT
}


void XSecs2Items::clear()
{
XEXCEPT_ENTRY
	XPOSITION pos1, pos2;
	for( pos1 = m_items.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		XSecs2Item *pNeItem = (XSecs2Item*)m_items.GetNext( pos1 );
		delete pNeItem; 
		m_items.RemoveAt( pos2 );
	}
	m_pElem = NULL;
XEXCEPT_EXIT
}
XSecs2Item* XSecs2Items::getHeadItem()
{
XEXCEPT_ENTRY
	if( m_items.IsEmpty() ) {
		return NULL;
	}
	return (XSecs2Item*)m_items.GetHead();
XEXCEPT_EXIT
}
XSecs2Item* XSecs2Items::getItemByName(XStrBuff &strName)
{
XEXCEPT_ENTRY
	XSecs2Item* pItem = getHeadItem();
	for(; pItem != NULL; pItem = pItem->nextSibling()) {
		if( pItem->getName().Compare( strName ) == 0 ) {
			return pItem;
		}
	}
XEXCEPT_EXIT
	return NULL;
}


bool XSecs2Items::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	_XASSERT( pElem );
	m_pElem = pElem;
	for(	XmlHlpElementActl elmItemType = XmlHlpElementActl(m_pElem).firstChild(); 
			elmItemType != NULL; 
			elmItemType = elmItemType.nextSiblingElement() ) {
		if( elmItemType.getNodeName() == _T("itemType") ) {
			XSecs2Item* pItem = this->add( elmItemType.m_pT );
			if( pItem == NULL ) {
				return false;	
			}
		}
	}
	return true;
XEXCEPT_EXIT
	return false;
}

//-----------------------------------------------------------
//
//	XSecs2Item
//
//-----------------------------------------------------------
XSecs2Item::XSecs2Item(		const TCHAR* pName
					   ,	XDATA_TYPE type
					   ,	XSecs2Items* pParent
					   ,	XmlHlpElementRawType* pElem)
	:	XSecs2Obj( XSecs2Obj::ITEM, pParent )
	,	m_strName( pName )
	,	m_type( type )
	,	m_pElem( pElem )
	,	m_formats( NULL )
{
XEXCEPT_ENTRY

	m_formats.m_pParent = this;

	loadXML( m_pElem );

	m_pos = pParent->m_items.AddTail( this );
XEXCEPT_EXIT
}

XSecs2Item::~XSecs2Item()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

XStrBuff XSecs2Item::getComment()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return _T("");
	}
	XmlHlpElementActl elem = m_pElem;
	if( elem.isAttribute(XmlT("comment")) == false ) {
		return _T("");
	}
	return elem.getAttribute(XmlT("comment")) ;
XEXCEPT_EXIT
	return _T("");
}
void XSecs2Item::setComment(XStrBuff str)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	XmlHlpElementActl(m_pElem).setAttribute(XmlT("comment"), str.c_str());
XEXCEPT_EXIT
}


bool XSecs2Item::getIsFixVals() const
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return false;
	}
	XmlHlpElementActl elem = m_pElem;
	if( elem.isAttribute(XmlT("fixed")) == false ) {
		return false;
	}
	return elem.getAttribute(XmlT("fixed")) == _T("true");
XEXCEPT_EXIT
return false;
}

void XSecs2Item::setIsFixVals(bool val)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	XmlHlpElementActl(m_pElem).setAttribute(XmlT("fixed"), val ? _T("true"): _T("false") );
XEXCEPT_EXIT
}

bool XSecs2Item::getIsMultiFmt() const
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return false;
	}
	XmlHlpElementActl elem = m_pElem;
	if( elem.isAttribute(XmlT("mlt_fml")) == false ) {
		return false;
	}
	return elem.getAttribute(XmlT("mlt_fml")) == _T("true");
XEXCEPT_EXIT
return false;
}

void XSecs2Item::setIsMultiFmt(bool val)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	XmlHlpElementActl(m_pElem).setAttribute(XmlT("mlt_fml"), val ? _T("true"): _T("false") );
XEXCEPT_EXIT
}



XSecs2ItemFormat* XSecs2Item::getCurrentFormat()
{
XEXCEPT_ENTRY
	XSecs2ItemFormat* pFormat = m_formats.getHeadItem();
	for(; pFormat != NULL; pFormat = pFormat->nextSibling() ) {
		switch( pFormat->getDataType() ) {
		case XDATA_TYPE_UX:
			if( m_type == XDATA_TYPE_U1 ||	m_type == XDATA_TYPE_U2 ||
				m_type == XDATA_TYPE_U4 ||	m_type == XDATA_TYPE_U8 ) {
				return pFormat;
			}
			break;
		case XDATA_TYPE_IX:
			if( m_type == XDATA_TYPE_I1 ||	m_type == XDATA_TYPE_I2 ||
				m_type == XDATA_TYPE_I4 ||	m_type == XDATA_TYPE_I8 ) {
				return pFormat;
			}
			break;
		case XDATA_TYPE_FX:
			if( m_type == XDATA_TYPE_F4 ||	m_type == XDATA_TYPE_F8 ) {
				return pFormat;
			}
			break;
		default:
			if( pFormat->getDataType() == m_type ) {
				return pFormat;
			}
			break;
		}
	}
XEXCEPT_EXIT
	return NULL;
}
XSecs2ItemFormats& XSecs2Item::getFormats()
{
XEXCEPT_ENTRY
	return m_formats;
XEXCEPT_EXIT
}

XSecs2Item* XSecs2Item::nextSibling()
{
XEXCEPT_ENTRY
	XPOSITION pos = m_pos;
	if( m_pParent == NULL || pos == NULL ) {
		return NULL;
	}
	XSecs2Item *pItem = (XSecs2Item*)((XSecs2Items*)m_pParent)->m_items.GetNext( pos );
	if( pos == NULL ) {
		return NULL;
	}
	return (XSecs2Item*)((XSecs2Items*)m_pParent)->m_items.GetNext( pos );
XEXCEPT_EXIT
}
bool XSecs2Item::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XmlHlpElementActl elem = pElem;

	XmlHlpElementActl elem1 = elem.firstChild();
	for(; elem1 != NULL; elem1 = elem1.nextSiblingElement() ) {
		if( elem1.getNodeName() == _T("formats") ) {
			if( m_formats.loadXML( elem1 ) == false ) {
				return false;
			}
		} else {
			XTRACE(_T("!! WARNING !! UNKNOWN ELEMENT (%s)\n"), (LPCTSTR)elem1.getNodeName() );
		}
	}
	return true;
XEXCEPT_EXIT
}


//-----------------------------------------------------------
//
//	XSecs2ItemFormats
//
//-----------------------------------------------------------
XSecs2ItemFormats::XSecs2ItemFormats(XSecs2Obj* pParent)
	:	XSecs2Obj( XSecs2Obj::FORMATS, pParent )
	,	m_pElem( NULL )
{
XEXCEPT_ENTRY
	
XEXCEPT_EXIT
}
XSecs2ItemFormats::~XSecs2ItemFormats()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSecs2ItemFormats::clear()
{
XEXCEPT_ENTRY
	XPOSITION pos1, pos2;
	for( pos1 = m_items.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		XSecs2ItemFormat *pNeItem = (XSecs2ItemFormat*)m_items.GetNext( pos1 );
		delete pNeItem; 
		m_items.RemoveAt( pos2 );
	}	
XEXCEPT_EXIT
}
XSecs2ItemFormat* XSecs2ItemFormats::add(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XmlHlpElementActl elem = pElem;
	XStrBuff strErr;

	if( elem.isAttribute(_T("type")) == false ) {
		strErr = _T("attribute(type) not found. path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);

		return NULL;
	}

	XStrBuff	strType = elem.getAttribute(_T("type"));
	XDATA_TYPE  dataType = XData::parseDataType_From_String(strType);
	if( dataType == XDATA_TYPE_END ) {
		strErr = _T("invalid type string. type=");
		strErr << strType << _T(" ");
		strErr << _T(" path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
		return NULL;
	}
	XSecs2ItemFormat *pNeItem = new XSecs2ItemFormat(dataType, this, pElem);
	pNeItem->m_pos = m_items.AddTail( pNeItem );
	return pNeItem;
XEXCEPT_EXIT
	return NULL;
}
XSecs2ItemFormat* XSecs2ItemFormats::add(XDATA_TYPE type)
{
XEXCEPT_ENTRY

	//既に同じタイプがセットされていないか？
	XSecs2ItemFormat* pFormat = this->getHeadItem();

	while( pFormat != NULL ) {
		if( pFormat->getDataType() == type ) {
			XEXCEPT_THROW_STR(XExcept_SECS, _T("Already Same type exist."), 0, 0);
			return NULL;
		}
		pFormat = pFormat->nextSibling();
	}

	XmlHlpElementActl elemParent = m_pElem;
	XmlHlpElementActl elem = elemParent.addElement( XmlT("format") );
	if( elem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	XmlHlpElementActl elemL = elem.addElement( XmlT("list") );


	elem.setAttribute(_T("type"), XData::getDataTypeString_From_TYPE(type) );
	return this->add( elem.m_pT );
XEXCEPT_EXIT
	return NULL;
}

bool XSecs2ItemFormats::remove(XSecs2ItemFormat* pItem)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return false;
	}
	XmlHlpElementActl elem = m_pElem;


	//既に同じタイプがセットされていないか？
	XSecs2ItemFormat* pFormat = this->getHeadItem();

	while( pFormat != NULL ) {
		if( pFormat == pItem ) {
			elem.removeChild( pFormat->m_pElem );	// XML
			m_items.RemoveAt( pFormat->m_pos );		// XList
			delete pFormat;							// <object>
			return true;
		}
		pFormat = pFormat->nextSibling();
	}

XEXCEPT_EXIT
	return true;
}

XSecs2ItemFormat* XSecs2ItemFormats::getHeadItem()
{
XEXCEPT_ENTRY
	if( m_items.IsEmpty() ) {
		return NULL;
	}
	return (XSecs2ItemFormat*)m_items.GetHead();		
XEXCEPT_EXIT
}
bool XSecs2ItemFormats::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XStrBuff	strErr;

	m_pElem = pElem;
	XmlHlpElementActl elem = pElem;


	XmlHlpElementActl elem1 = elem.firstChild();

	for(; elem1 != NULL; elem1 = elem1.nextSiblingElement() ) {
		if( elem1.getNodeName() == _T("format") ) {
		
			this->add( elem1.m_pT );
		}
	}
XEXCEPT_EXIT
	return true;
}


//-----------------------------------------------------------
//
//	XSecs2ItemFormat
//
//-----------------------------------------------------------
XSecs2ItemFormat::XSecs2ItemFormat(XDATA_TYPE type, XSecs2ItemFormats* pParent, XmlHlpElementRawType* pElem )
	:	XSecs2Obj( XSecs2Obj::FORMAT, pParent )
	,	m_pElem( pElem )
	,	m_type( type )
	,	m_pListItem( NULL )
	,	m_pMaxVal( NULL )
	,	m_pMinVal( NULL )
{
XEXCEPT_ENTRY
	loadXML( pElem );
XEXCEPT_EXIT
}

XSecs2ItemFormat::~XSecs2ItemFormat()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}

void XSecs2ItemFormat::clear()
{
XEXCEPT_ENTRY
	if( m_pListItem != NULL ) {
		delete m_pListItem;
	}
	if( m_pMaxVal != NULL ) {
		delete m_pMaxVal;
	}
	if( m_pMinVal != NULL ) {
		delete m_pMinVal;
	}
	m_pListItem = NULL;
	m_pMaxVal = NULL;
	m_pMinVal = NULL;


	XPOSITION pos1, pos2;
	for( pos1 = m_items.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		XSecs2ItemValue *pNeItem = (XSecs2ItemValue*)m_items.GetNext( pos1 );
		delete pNeItem; 
		m_items.RemoveAt( pos2 );
	}	
XEXCEPT_EXIT
}

XDATA_TYPE XSecs2ItemFormat::getDataType() const
{
XEXCEPT_ENTRY
	return m_type;
XEXCEPT_EXIT
}
bool XSecs2ItemFormat::isEqulDataType( XDATA_TYPE type )
{
XEXCEPT_ENTRY
	switch( m_type ) {
	case XDATA_TYPE_UX:
		switch(type) {
		case XDATA_TYPE_U1:
		case XDATA_TYPE_U2:
		case XDATA_TYPE_U4:
		case XDATA_TYPE_U8:
			return true;
		default:
			return false;
		}
		break;

	case XDATA_TYPE_IX:
		switch(type) {
		case XDATA_TYPE_I1:
		case XDATA_TYPE_I2:
		case XDATA_TYPE_I4:
		case XDATA_TYPE_I8:
			return true;
		default:
			return false;
		}
		break;

	case XDATA_TYPE_FX:
		switch(type) {
		case XDATA_TYPE_F4:
		case XDATA_TYPE_F8:
			return true;
		default:
			return false;
		}
		break;
	default:
		return m_type == type;
	}
	return false;
XEXCEPT_EXIT

}
bool XSecs2ItemFormat::isSizeMin()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return NULL;
	}
	XmlHlpElementActl elem( m_pElem );
	return elem.isAttribute(XmlT("size_min"));
XEXCEPT_EXIT
}
bool XSecs2ItemFormat::isSizeMax()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return NULL;
	}
	XmlHlpElementActl elem( m_pElem );
	return elem.isAttribute(XmlT("size_max"));
XEXCEPT_EXIT
}
int  XSecs2ItemFormat::getSizeMin()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return 0;
	}
	XmlHlpElementActl elem( m_pElem );
	if( elem.isAttribute(XmlT("size_min")) == false ) {
		return 0;
	}
	return elem.getAttributeInt(XmlT("size_min"));
XEXCEPT_EXIT

}
int  XSecs2ItemFormat::getSizeMax()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return 0;
	}
	XmlHlpElementActl elem( m_pElem );
	if( elem.isAttribute(XmlT("size_max")) == false ) {
		return 0;
	}
	return elem.getAttributeInt(XmlT("size_max"));
XEXCEPT_EXIT
}
void XSecs2ItemFormat::setSizeMin(int val)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	if( val == -1 ) {
		XmlHlpElementActl( m_pElem ).removeAttr( XmlT("size_min") );
	} else {
		XmlHlpElementActl( m_pElem ).setAttribute(XmlT("size_min"), val);
	}
XEXCEPT_EXIT
}
void XSecs2ItemFormat::setSizeMax(int val)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	if( val == -1 ) {
		XmlHlpElementActl( m_pElem ).removeAttr( XmlT("size_max") );
	} else {
		XmlHlpElementActl( m_pElem ).setAttribute(XmlT("size_max"), val);
	}
XEXCEPT_EXIT
}

XRichData* XSecs2ItemFormat::getMaxData()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return NULL;
	}
	if( m_pMaxVal != NULL ) {
		return m_pMaxVal;
	}

	XmlHlpElementActl elem( m_pElem );
	if( elem.isAttribute(XmlT("max")) == false ) {
		return NULL;
	}
	XStrBuff strVal = elem.getAttribute( XmlT("max") );
	m_pMaxVal = new XRichData( m_type );
	innerSetVal(strVal, m_pMaxVal);
XEXCEPT_EXIT
	return m_pMaxVal;
}

XRichData* XSecs2ItemFormat::getMinData()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return NULL;
	}
	if( m_pMinVal != NULL ) {
		return m_pMinVal;
	}

	XmlHlpElementActl elem( m_pElem );
	if( elem.isAttribute(XmlT("min")) == false ) {
		return NULL;
	}
	XStrBuff strVal = elem.getAttribute( XmlT("min") );
	m_pMinVal = new XRichData( m_type );
	innerSetVal(strVal, m_pMinVal);
XEXCEPT_EXIT
	return m_pMinVal;
}

bool XSecs2ItemFormat::remove()
{
XEXCEPT_ENTRY
	XSecs2ItemFormats* pFormats = (XSecs2ItemFormats*)m_pParent;
	pFormats->remove( this );
XEXCEPT_EXIT
	return true;
}


XSecs2ItemDataL*	XSecs2ItemFormat::getListItem() const
{
XEXCEPT_ENTRY
	return m_pListItem;
XEXCEPT_EXIT
}
XSecs2ItemData*	XSecs2ItemFormat::getListHeadItem() const
{
XEXCEPT_ENTRY
	if( m_pListItem == NULL ) {
		return NULL;
	}
	return m_pListItem->getHeadItem();
XEXCEPT_EXIT
}
XSecs2ItemFormat* XSecs2ItemFormat::nextSibling()
{
XEXCEPT_ENTRY
	XPOSITION pos = m_pos;
	if( m_pParent == NULL || pos == NULL ) {
		return NULL;
	}
	XSecs2ItemFormat *pItem = (XSecs2ItemFormat*)((XSecs2ItemFormats*)m_pParent)->m_items.GetNext( pos );
	if( pos == NULL ) {
		return NULL;
	}
	return (XSecs2ItemFormat*)((XSecs2ItemFormats*)m_pParent)->m_items.GetNext( pos );	
XEXCEPT_EXIT
}


XSecs2ItemValue* XSecs2ItemFormat::addValue(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XSecs2ItemValue *pNeItem = new XSecs2ItemValue(m_type, this, pElem);
	pNeItem->m_pos = m_items.AddTail( pNeItem );
	return pNeItem;
XEXCEPT_EXIT
	return NULL;
}

bool XSecs2ItemFormat::removeValue(XSecs2ItemValue* pItem)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return false;
	}
	XmlHlpElementActl elem = m_pElem;

	//既に同じタイプがセットされていないか？
	XSecs2ItemValue* pDstItem = this->getValueHeadItem();
	for(; pDstItem != NULL; pDstItem = pDstItem->nextSibling() ) {
		if( pDstItem == pItem ) {
			elem.removeChild( pDstItem->m_pElem );	// XML
			m_items.RemoveAt( pDstItem->m_pos );		// XList
			delete pDstItem;							// <object>
			return true;
		}
	}

XEXCEPT_EXIT
	return true;
}
XSecs2ItemValue* XSecs2ItemFormat::getValueHeadItem() const
{
XEXCEPT_ENTRY
	if( m_items.IsEmpty() ) {
		return NULL;
	}
	return (XSecs2ItemValue*)m_items.GetHead();
XEXCEPT_EXIT
	return NULL;
}

XSecs2ItemValue* XSecs2ItemFormat::addValue(XRichData* pRData)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return NULL;
	}
	

	XmlHlpElementActl elemNew= XmlHlpElementActl( m_pElem ).addElement( XmlT("value") );
	if( elemNew == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	elemNew.setAttribute(XmlT("val"), pRData->toString() );
	return this->addValue( elemNew.m_pT );
XEXCEPT_EXIT
	return NULL;
}


bool XSecs2ItemFormat::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XStrBuff	strErr;

	m_pElem = pElem;
	XmlHlpElementActl elem = pElem;


	XmlHlpElementActl elem1 = elem.firstChild();

	for(; elem1 != NULL; elem1 = elem1.nextSiblingElement() ) {
		if( elem1.getNodeName() == _T("list") ) {
			if( m_pListItem != NULL ) {
				delete m_pListItem;
				m_pListItem = NULL;
			}
			m_pListItem = new XSecs2ItemDataL(elem1, this);
		} else	
		if( elem1.getNodeName() == _T("value") ) {
			this->addValue( elem1.m_pT );
		}	

	}
	return true;
XEXCEPT_EXIT
}

void XSecs2ItemFormat::innerSetVal(XStrBuff& str, XRichData* pData)
{
XEXCEPT_ENTRY
	XSecsTextParser::parseFromString(str, str.GetLength(), pData);
XEXCEPT_EXIT
}



//-----------------------------------------------------------
//
//	XSecs2ItemValue
//
//-----------------------------------------------------------
XSecs2ItemValue::XSecs2ItemValue(XDATA_TYPE type, XSecs2ItemFormat* pParent, XmlHlpElementRawType* pElem )
	:	XSecs2Obj( XSecs2Obj::VALUE, pParent )
	,	XRichData( type )
	,	m_pElem( pElem )
{
XEXCEPT_ENTRY
	loadXML( pElem );
XEXCEPT_EXIT
}
XSecs2ItemValue::~XSecs2ItemValue()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XStrBuff XSecs2ItemValue::getComment()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return _T("");
	}
	return XmlHlpElementActl(m_pElem).getValue();
XEXCEPT_EXIT
	return _T("");
}
void XSecs2ItemValue::setComment(XStrBuff val)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return ;
	}
	XmlHlpElementActl(m_pElem).setValue(val);
XEXCEPT_EXIT
}
bool XSecs2ItemValue::remove()
{
XEXCEPT_ENTRY
	XSecs2ItemFormat* pValType = (XSecs2ItemFormat*)XSecs2Obj::m_pParent;
	pValType->removeValue( this );
XEXCEPT_EXIT
	return true;
}

XSecs2ItemValue* XSecs2ItemValue::nextSibling()
{
XEXCEPT_ENTRY
	XPOSITION pos = m_pos;
	if( XSecs2Obj::m_pParent == NULL || pos == NULL ) {
		return NULL;
	}
	XSecs2ItemValue *pItem = (XSecs2ItemValue*)((XSecs2ItemFormat*)XSecs2Obj::m_pParent)->m_items.GetNext( pos );
	if( pos == NULL ) {
		return NULL;
	}
	return (XSecs2ItemValue*)((XSecs2ItemFormat*)XSecs2Obj::m_pParent)->m_items.GetNext( pos );	
XEXCEPT_EXIT
}

bool XSecs2ItemValue::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY

	XStrBuff	strErr;
	XStrBuff	strData;
	m_pElem = pElem;
	XmlHlpElementActl elem = pElem;

	// 必須属性のチェック
	if( elem.isAttribute(_T("val")) == false ) {

		strErr = _T("attribute(val) not found. path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
		return NULL;
	}
	strData = elem.getAttribute(_T("val"));

	XSecsTextParser::parseFromString(strData, strData.GetLength(), this);


	return true;
XEXCEPT_EXIT
}
