#include "xlib/semi/secs/xsecs2.h"
#include "xlib/semi/secs/xsecs_text_parser.h"
#include "xlib/xeception.h"
#include "xlib/xmlhelper.h"



//-----------------------------------------------------------
//
//	XSecs2Messages
//
//-----------------------------------------------------------
XSecs2Messages::XSecs2Messages(XSecs2Obj *pParent)
	:	XSecs2Obj( XSecs2Obj::MESSAGES, pParent )
	,	m_pElem( NULL) 
	,	m_categories( NULL )
{
XEXCEPT_ENTRY
	m_categories.m_pParent = this;

XEXCEPT_EXIT
}
XSecs2Messages::~XSecs2Messages()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSecs2Messages::clear()
{
XEXCEPT_ENTRY
	XPOSITION pos1, pos2;
	for( pos1 = m_items.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		XSecs2Message *pNeItem = (XSecs2Message*)m_items.GetNext( pos1 );
		delete pNeItem; 
		m_items.RemoveAt( pos2 );
	}
	m_pElem = NULL;
XEXCEPT_EXIT
}
XSecs2Message* XSecs2Messages::add(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XSecs2Message *pNeItem = new XSecs2Message(this, pElem);
	return pNeItem;
XEXCEPT_EXIT
	return NULL;
}

XSecs2Message* XSecs2Messages::add(XD_UINT1 stream, XD_UINT1 function)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
	}

	//既に同じタイプがセットされていないか？
	for(XSecs2Message* pMessage = this->getHeadItem(); pMessage != NULL; ) {
		if( pMessage->getStream() == stream && pMessage->getFunction() == function ) {
			XStrBuff strErr;
			strErr.Format(_T("Already Same S%uF%u exist."), stream,function );
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
			return NULL;
		}
		pMessage = pMessage->nextSibling();
	}


	XmlHlpElementActl elemParent = m_pElem;
	XmlHlpElementActl elem = elemParent.addElement( XmlT("message") );
	if( elem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	elem.setAttribute(_T("s"), (int)stream);
	elem.setAttribute(_T("f"), (int)function);
	return this->add( elem.m_pT );
XEXCEPT_EXIT
	return NULL;
}
bool XSecs2Messages::remove(XSecs2Message* pItem)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
	}
	//既に同じタイプがセットされていないか？
	for(	XSecs2Message* pMessage = this->getHeadItem();
			pMessage != NULL ;
			pMessage = pMessage->nextSibling()) {

		if( pMessage == pItem ) {
			XmlHlpElementActl(m_pElem).removeChild( pMessage->m_pElem );	// XML
			m_items.RemoveAt( pMessage->m_pos );		// XList
			delete pMessage;							// <object>
			return true;
		}
	}
XEXCEPT_EXIT
	return false;
}

XSecs2Message* XSecs2Messages::getHeadItem()
{
XEXCEPT_ENTRY
	if( m_items.IsEmpty() ) {
		return NULL;
	}
	return (XSecs2Message*)m_items.GetHead();
XEXCEPT_EXIT
	return NULL;
}

bool XSecs2Messages::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	_XASSERT( pElem );
	m_pElem = pElem;
	for(	XmlHlpElementActl elmItemType = XmlHlpElementActl(m_pElem).firstChild(); 
			elmItemType != NULL; 
			elmItemType = elmItemType.nextSiblingElement() ) {
		if( elmItemType.getNodeName() == _T("message") ) {
			XSecs2Message* pMessage = this->add( elmItemType.m_pT );
			if( pMessage == NULL ) {
				return false;	
			}
		}
	}
	return true;
XEXCEPT_EXIT
	return false;
}
XSecs2MessageCats& XSecs2Messages::getCategories()
{
XEXCEPT_ENTRY
	return m_categories;
XEXCEPT_EXIT
}

XSecs2Message* XSecs2Messages::setRealMessage(XSecsData* pSECSData, bool bThrow /*= true*/)
{
XEXCEPT_ENTRY
	// Messages
	for(	XSecs2Message*  pMessage =this->getHeadItem(); 
			pMessage != NULL ;
			pMessage = pMessage->nextSibling()) {

		if( pSECSData->get_stream()   == pMessage->getStream()
					&&
			pSECSData->get_function() == pMessage->getFunction() ) {

			if( pMessage->setRealMessage(pSECSData, bThrow) == 0 ) {
				return pMessage;
			}
		}
	}
XEXCEPT_EXIT
	return NULL;
}


//-----------------------------------------------------------
//
//	XSecs2Message
//
//-----------------------------------------------------------
XSecs2Message::XSecs2Message(XSecs2Messages* pParent, XmlHlpElementRawType* pElem)
	:	XSecs2Obj( XSecs2Obj::MESSAGE, pParent )
	,	m_pElem( pElem) 

	,	m_sf( 0 )
	,	m_pBody( NULL )

	,	m_strCategoryName( _T("") )
	,	m_pCategory( NULL )
{
XEXCEPT_ENTRY
	loadXML( pElem );

	m_pos = pParent->m_items.AddTail( this );
XEXCEPT_EXIT
}
XSecs2Message::~XSecs2Message()
{
XEXCEPT_ENTRY
	if( m_pBody != NULL ) {
		delete m_pBody;
	}
XEXCEPT_EXIT
}
XD_UINT1 XSecs2Message::getStream() const
{
XEXCEPT_ENTRY
	return (m_sf & 0xFF00) >> 8;
XEXCEPT_EXIT
}
XD_UINT1 XSecs2Message::getFunction() const
{
XEXCEPT_ENTRY
	return m_sf & 0x00FF;
XEXCEPT_EXIT
}
XStrBuff XSecs2Message::getAction()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return _T("");
	}
	XmlHlpElementActl elem = m_pElem;
	if( elem.isAttribute(XmlT("action")) == false ) {
		return _T("");
	}
	return elem.getAttribute(XmlT("action")) ;
XEXCEPT_EXIT
	return _T("");
}
void XSecs2Message::setAction(XStrBuff str)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	XmlHlpElementActl elem = m_pElem;
	elem.setAttribute(XmlT("action"), str.c_str());
XEXCEPT_EXIT
}

XSecs2Message::DIRECTION XSecs2Message::getDirection() const
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return XSecs2Message::BOTH;
	}
	XmlHlpElementActl elem = m_pElem;
	if( elem.isAttribute(XmlT("dir")) == false ) {
		return XSecs2Message::BOTH;
	}
	XStrBuff strTmp = elem.getAttribute(XmlT("dir"));
	if(strTmp.CompareNoCase(_T("HtoE")) == 0 ) {
		return XSecs2Message::HtoE;
	}
	if( strTmp.CompareNoCase(_T("EtoH")) == 0 ) {
		return XSecs2Message::EtoH;
	}
	if( strTmp.CompareNoCase(_T("BOTH")) == 0 ) {
		return XSecs2Message::BOTH;
	}
XEXCEPT_EXIT
	return XSecs2Message::INVALID;
}
void XSecs2Message::setDirection(XSecs2Message::DIRECTION val)
{
XEXCEPT_ENTRY
	XStrBuff strErr;
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return ;
	}

	XmlHlpElementActl elem = m_pElem;
	switch( val ) {
	case XSecs2Message::BOTH:
		elem.setAttribute(XmlT("dir"), XmlT("BOTH"));
		break;
	case XSecs2Message::HtoE:
		elem.setAttribute(XmlT("dir"), XmlT("HtoE"));
		break;
	case XSecs2Message::EtoH:
		elem.setAttribute(XmlT("dir"), XmlT("EtoH"));
		break;
	default:
		strErr.Format(_T("invalid data type. XSecs2Message::DIRECTION(%u)"), val);
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, val, 0);
		break;
	}
XEXCEPT_EXIT

}

XSecs2Message::MLT_BLK XSecs2Message::getMltCont() const
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return XSecs2Message::MLT_NO;
	}
	XmlHlpElementActl elem = m_pElem;
	if( elem.isAttribute(XmlT("multi")) == false ) {
		return XSecs2Message::MLT_NO;
	}
	XStrBuff strTmp = elem.getAttribute(XmlT("multi"));
	if(strTmp.CompareNoCase(_T("YES")) == 0 ) {
		return XSecs2Message::MLT_YES;
	}
	if( strTmp.CompareNoCase(_T("INQUIRE")) == 0 ) {
		return XSecs2Message::MLT_INQ;
	}
	if( strTmp.CompareNoCase(_T("GRANT")) == 0 ) {
		return XSecs2Message::MLT_GRT;
	}
XEXCEPT_EXIT
	return XSecs2Message::MLT_NO;
}
void XSecs2Message::setMltCont(XSecs2Message::MLT_BLK val)
{
XEXCEPT_ENTRY
	XStrBuff strErr;
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return ;
	}

	XmlHlpElementActl elem = m_pElem;
	switch( val ) {
	case XSecs2Message::MLT_NO:
		elem.setAttribute(XmlT("multi"), XmlT("NO"));
		break;
	case XSecs2Message::MLT_YES:
		elem.setAttribute(XmlT("multi"), XmlT("YES"));
		break;
	case XSecs2Message::MLT_INQ:
		elem.setAttribute(XmlT("multi"), XmlT("INQUIRE"));
		break;
	case XSecs2Message::MLT_GRT:
		elem.setAttribute(XmlT("multi"), XmlT("GRANT"));
		break;
	default:
		strErr.Format(_T("invalid data type. XSecs2Message::MLT_BLK(%u)"), val);
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, val, 0);
		break;
	}
XEXCEPT_EXIT
}


bool XSecs2Message::getReply() const
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return false;
	}
	XmlHlpElementActl elem = m_pElem;
	if( elem.isAttribute(XmlT("reply")) == false ) {
		return false;
	}
	XStrBuff strTmp = elem.getAttribute(XmlT("reply"));
	if(strTmp.CompareNoCase(_T("true")) == 0 ) {
		return true;
	}
XEXCEPT_EXIT
	return false;
}
void XSecs2Message::setReply(bool val)
{
XEXCEPT_ENTRY
	XStrBuff strErr;
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return ;
	}
	XmlHlpElementActl(m_pElem).setAttribute(XmlT("reply"), 
		val == true ? XmlT("true") : XmlT("false"));
XEXCEPT_EXIT
}


XSecs2ItemData* XSecs2Message::getBody()
{
XEXCEPT_ENTRY
	return m_pBody;
XEXCEPT_EXIT
}


XSecs2ItemDataL* XSecs2Message::addList()
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return NULL;
	}

	// bodyエレメントがあれば、削除する。
	XmlHlpElementActl elem( m_pElem );
	XmlHlpElementActl elem1 = elem.firstChild();
	for(; elem1 != NULL; elem1 = elem1.nextSiblingElement() ) {
		if( elem1.getNodeName() == _T("body") ) {
			elem.removeChild(elem1);
			break;
		}
	}
	// ボディエレメントを追加
	XmlHlpElementActl elemBody = elem.addElement( XmlT("body") );
	if( elemBody == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	XmlHlpElementActl elemNew= elemBody.addElement( XmlT("list") );
		if( elemNew == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	if( m_pBody != NULL ) {
		delete m_pBody;
		m_pBody = NULL;
	}
	m_pBody = new XSecs2ItemDataL( elemNew.m_pT, this);
XEXCEPT_EXIT
	return (XSecs2ItemDataL*)m_pBody;
}

XSecs2ItemDataD* XSecs2Message::addDistribute(XStrBuff str)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return NULL;
	}

	// bodyエレメントがあれば、削除する。
	XmlHlpElementActl elem( m_pElem );
	XmlHlpElementActl elem1 = elem.firstChild();
	for(; elem1 != NULL; elem1 = elem1.nextSiblingElement() ) {
		if( elem1.getNodeName() == _T("body") ) {
			elem.removeChild(elem1);
			break;
		}
	}
	// ボディエレメントを追加
	XmlHlpElementActl elemBody = elem.addElement( XmlT("body") );
	if( elemBody == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	XmlHlpElementActl elemNew= elemBody.addElement( XmlT("item") );
	if( elemNew == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	elemNew.setAttribute( XmlT("dstItem"), str);
	if( m_pBody != NULL ) {
		delete m_pBody;
		m_pBody = NULL;
	}
	m_pBody = new XSecs2ItemDataD( elemNew.m_pT, this);
XEXCEPT_EXIT
	return (XSecs2ItemDataD*)m_pBody;
}

bool XSecs2Message::removeBody() 
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return false;
	}

	// bodyエレメントがあれば、削除する。
	XmlHlpElementActl elem( m_pElem );
	XmlHlpElementActl elem1 = elem.firstChild();
	for(; elem1 != NULL; elem1 = elem1.nextSiblingElement() ) {
		if( elem1.getNodeName() == _T("body") ) {
			elem.removeChild(elem1);
			break;
		}
	}
	// ボディエレメントを追加
	XmlHlpElementActl elemBody = elem.addElement( XmlT("body") );
	if( elemBody == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	if( m_pBody != NULL ) {
		delete m_pBody;
		m_pBody = NULL;
	}
XEXCEPT_EXIT
	return true;
}


XSecs2Message* XSecs2Message::nextSibling()
{
XEXCEPT_ENTRY
	XPOSITION pos = m_pos;
	if( m_pParent == NULL || pos == NULL ) {
		return NULL;
	}
	XSecs2Message *pItem = (XSecs2Message*)((XSecs2Messages*)m_pParent)->m_items.GetNext( pos );
	if( pos == NULL ) {
		return NULL;
	}
	return (XSecs2Message*)((XSecs2Messages*)m_pParent)->m_items.GetNext( pos );
XEXCEPT_EXIT
}

bool XSecs2Message::remove()
{
XEXCEPT_ENTRY
	_XASSERT( m_pParent );
	((XSecs2Messages*)m_pParent)->remove( this );
XEXCEPT_EXIT
	return true;
}


bool XSecs2Message::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XmlHlpElementActl elem = pElem;
	XStrBuff strErr;

	// strema
	if( elem.isAttribute(XmlT("s")) !=  true ) {
		strErr = _T("attribute(s) not found. path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
		return false;
	}
	// function
	if( elem.isAttribute(XmlT("f")) !=  true ) {
		strErr = _T("attribute(s) not found. path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
		return false;
	}
	XD_UINT1 s = elem.getAttributeUInt( XmlT("s") );
	XD_UINT1 f = elem.getAttributeUInt( XmlT("f") );
	m_sf = s << 8 | f;
	// dir

	// category
	if( elem.isAttribute(XmlT("category")) ==  true ) {
		m_strCategoryName = elem.getAttribute( XmlT("category") );
	}


	XmlHlpElementActl elem1 = elem.firstChild();
	for(; elem1 != NULL; elem1 = elem1.nextSiblingElement() ) {
		if( elem1.getNodeName() == _T("body") ) {
			XmlHlpElementActl elemL = elem1.firstChild();
			for(; elemL != NULL; elemL = elemL.nextSiblingElement() ) {
				if( m_pBody != NULL ) {
					delete m_pBody;
					m_pBody = NULL;
				}
				if( elemL.getNodeName() == _T("list") ) {
					m_pBody = new XSecs2ItemDataL( elemL.m_pT, this);
				} else
				if( elemL.getNodeName() == _T("item") ) {
					m_pBody = new XSecs2ItemDataD( elemL.m_pT, this);
				}
			}
		}
	}
	return true;
XEXCEPT_EXIT
}
XSecs2MessageCat* XSecs2Message::getCategory()
{
XEXCEPT_ENTRY
	if( m_pCategory != NULL ) {
		return m_pCategory;
	}
	if( m_strCategoryName.GetLength() == 0 ) {
		return NULL;
	}
	XSecs2MessageCats& cats = ((XSecs2Messages*)m_pParent)->getCategories();
	XSecs2MessageCat* pCat = cats.getHeadItem();
	for( ; pCat != NULL; pCat = pCat->nextSibling() ) {
		if( pCat->getName() == m_strCategoryName ) {
			m_pCategory = pCat;
			return pCat;
		}
	}
	XStrBuff strError;
	strError.Format(_T("category not found. name:"));
	strError << m_strCategoryName;
	XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strError, 0, 0);
XEXCEPT_EXIT
	return NULL;
}

void XSecs2Message::setCategory(XSecs2MessageCat* val)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
		return ;
	}

	if( val != NULL ) {
		m_strCategoryName = val->getName();

		XmlHlpElementActl( m_pElem ).setAttribute(XmlT("category"), m_strCategoryName);

	} else {
		XmlHlpElementActl( m_pElem ).removeAttr( XmlT("category") );
	}
	m_pCategory = val;
XEXCEPT_EXIT
}


XRichData* XSecs2Message::newRealMessage()
{
XEXCEPT_ENTRY

	// Realメッセージをクリアする。
	m_msg.clear();

	if( m_pBody == NULL ) {
		return NULL;
	}
	XRichData* pRData = NULL;
	if( m_pBody->isDistribute() == false ) {
		pRData = m_msg.addChiled( XDATA_TYPE_L );
	} else {
		XSecs2ItemDataD* pD = (XSecs2ItemDataD*)m_pBody;
		XSecs2Item* pDistITem = pD->getDistItem();
		if( pDistITem == NULL ) {
			return NULL;
		}
		pRData = m_msg.addChiled( pDistITem->getDataType() );
	}
	pRData->setUserData( m_pBody );

	m_pBody->innerExpandRealMsg( pRData );
	

	return pRData;
XEXCEPT_EXIT
	return NULL;
}

XRichData* XSecs2Message::getRealMessage()
{
XEXCEPT_ENTRY
	if( m_msg.getChiledCount() == 0 ) {
		return NULL;
	}
	return m_msg.getChiled( 0 );
XEXCEPT_EXIT
	return NULL;
}

int XSecs2Message::setRealMessage(XSecsData* pSECSData, 
								  bool bThrow /*= true*/)
{
XEXCEPT_ENTRY

	if( pSECSData->get_stream()   != this->getStream() ) {
		return -1;
	}
	if( pSECSData->get_function() != this->getFunction() ) {
		return -2;
	}
	

	// Realメッセージをクリアする。
	m_msg.clear();

	if( m_pBody == NULL ) {
		return -3;
	}


	XRichData* pRData = NULL;
	XRichData* pSrcRData = NULL;
	if( m_pBody->isDistribute() == false ) {
		if( pSECSData->getChiledCount() == 0 ) {
			return -4;
		}
		pSrcRData = pSECSData->getChiled( 0 );
		pRData = m_msg.addChiled( XDATA_TYPE_L );
	} else {
		XSecs2ItemDataD* pD = (XSecs2ItemDataD*)m_pBody;
		XSecs2Item* pDistITem = pD->getDistItem();
		if( pDistITem == NULL ) {
			return -100;
		}
		
		if( pSECSData->getChiledCount() == 0 ) {
			return -4;
		}
		pSrcRData = pSECSData->getChiled( 0 );
		pRData = m_msg.addChiled( pSrcRData->getType() );
	}
	pRData->setUserData( m_pBody );

	return m_pBody->innerSetRealMessage( pSrcRData,  pRData);
XEXCEPT_EXIT
	return 0;
}


bool XSecs2Message::validateRealMessage(bool bThrow /*= true*/)
{
XEXCEPT_ENTRY
	if( m_pBody == NULL ) {
		return true;
	}
	for( XD_UINT4 i = 0; i < m_msg.getChiledCount(); i++) {
		XRichData* pR = m_msg.getChiled(i);
		_XASSERT( pR );
		XSecs2ItemData* pItem = (XSecs2ItemData*)pR->getUserData();
		if( pItem == NULL ) {
			if( bThrow == true ) {
				XEXCEPT_THROW_STR(XExcept_SECS, _T("XSecs2ItemData is null."), 0, 0);
			}
			return false;
		}
		if( pItem->validate( pR, bThrow ) == false ) {
			return false;
		}		
	}
XEXCEPT_EXIT
	return true;
}



//-----------------------------------------------------------
//
//	XSecs2MessageCats
//
//-----------------------------------------------------------
XSecs2MessageCats::XSecs2MessageCats(XSecs2Obj *pParent)
	:	XSecs2Obj( XSecs2Obj::MSG_CATS, pParent )
	,	m_pElem( NULL) 
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSecs2MessageCats::~XSecs2MessageCats()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}

void XSecs2MessageCats::clear()
{
XEXCEPT_ENTRY
	XPOSITION pos1, pos2;
	for( pos1 = m_items.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		XSecs2MessageCat *pNeItem = (XSecs2MessageCat*)m_items.GetNext( pos1 );
		delete pNeItem; 
		m_items.RemoveAt( pos2 );
	}
	m_pElem = NULL;
XEXCEPT_EXIT
}


XSecs2MessageCat* XSecs2MessageCats::add(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XSecs2MessageCat *pNeItem = new XSecs2MessageCat(this, pElem);
	return pNeItem;
XEXCEPT_EXIT
	return NULL;
}

XSecs2MessageCat* XSecs2MessageCats::add(XStrBuff strName)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
	}



	XmlHlpElementActl elemParent = m_pElem;
	XmlHlpElementActl elem = elemParent.addElement( XmlT("category") );
	if( elem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("faild add element."), 0, 0);
		return NULL;
	}
	elem.setAttribute(_T("name"), strName);
	return this->add( elem.m_pT );
XEXCEPT_EXIT
	return NULL;
}

bool XSecs2MessageCats::remove(XSecs2MessageCat* pItem)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("xml elemnt is null."), 0, 0);
	}
	//既に同じタイプがセットされていないか？
	for(	XSecs2MessageCat* pCategory = this->getHeadItem();
			pCategory != NULL ;
			pCategory = pCategory->nextSibling()) {

		if( pCategory == pItem ) {
			XmlHlpElementActl(m_pElem).removeChild( pCategory->m_pElem );	// XML
			m_items.RemoveAt( pCategory->m_pos );		// XList
			delete pCategory;							// <object>
			return true;
		}
	}
XEXCEPT_EXIT
	return false;
}

XSecs2MessageCat* XSecs2MessageCats::getHeadItem()
{
XEXCEPT_ENTRY
	if( m_items.IsEmpty() ) {
		return NULL;
	}
	return (XSecs2MessageCat*)m_items.GetHead();
XEXCEPT_EXIT
	return NULL;
}

bool XSecs2MessageCats::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	_XASSERT( pElem );
	m_pElem = pElem;
	for(	XmlHlpElementActl elmItemType = XmlHlpElementActl(m_pElem).firstChild(); 
			elmItemType != NULL; 
			elmItemType = elmItemType.nextSiblingElement() ) {
		if( elmItemType.getNodeName() == _T("category") ) {
			XSecs2MessageCat* pCategory = this->add( elmItemType.m_pT );
			if( pCategory == NULL ) {
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
//	XSecs2MessageCat
//
//-----------------------------------------------------------
XSecs2MessageCat::XSecs2MessageCat(XSecs2MessageCats* pParent, XmlHlpElementRawType* pElem)
	:	XSecs2Obj( XSecs2Obj::MSG_CAT, pParent )
	,	m_pElem( pElem) 
	,	m_strName( _T("") ) 
{
XEXCEPT_ENTRY
	loadXML( pElem );
	m_pos = pParent->m_items.AddTail( this );
XEXCEPT_EXIT
}
XSecs2MessageCat::~XSecs2MessageCat()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XStrBuff XSecs2MessageCat::getName()
{
XEXCEPT_ENTRY
	return m_strName ;
XEXCEPT_EXIT
	return _T("");
}
void XSecs2MessageCat::setName(XStrBuff str)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	XmlHlpElementActl elem = m_pElem;
	m_strName = str;
	elem.setAttribute(XmlT("name"), str.c_str());
XEXCEPT_EXIT
}

XSecs2MessageCat* XSecs2MessageCat::nextSibling()
{
XEXCEPT_ENTRY
	XPOSITION pos = m_pos;
	if( m_pParent == NULL || pos == NULL ) {
		return NULL;
	}
	XSecs2MessageCat *pItem = (XSecs2MessageCat*)((XSecs2MessageCats*)m_pParent)->m_items.GetNext( pos );
	if( pos == NULL ) {
		return NULL;
	}
	return (XSecs2MessageCat*)((XSecs2MessageCats*)m_pParent)->m_items.GetNext( pos );
XEXCEPT_EXIT
}

bool XSecs2MessageCat::remove()
{
XEXCEPT_ENTRY
	_XASSERT( m_pParent );
	((XSecs2MessageCats*)m_pParent)->remove( this );
XEXCEPT_EXIT
	return true;
}


bool XSecs2MessageCat::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XmlHlpElementActl elem = pElem;


	// name
	if( elem.isAttribute(XmlT("name")) !=  true ) {
		XStrBuff strErr;
		strErr = _T("attribute(name) not found. path:[");
		elem.getPathString( strErr );
		strErr << _T("]");

		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);
		return false;
	}
	m_strName = elem.getAttribute( XmlT("name") );
	return true;
XEXCEPT_EXIT
}


