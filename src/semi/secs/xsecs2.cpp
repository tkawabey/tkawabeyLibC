#include "xlib/semi/secs/xsecs2.h"
#include "xlib/semi/secs/xsecs2_item.h"
#include "xlib/semi/secs/xsecs_text_parser.h"
#include "xlib/xeception.h"
#include "xlib/xmlhelper.h"
#include "xlib/xdata_rich.h"


//-----------------------------------------------------------
//
//	XSecs2Obj
//
//-----------------------------------------------------------
XSecs2Obj::XSecs2Obj(XSecs2Obj::TYPE type, XSecs2Obj* pParent)
	:	m_type( type )
	,	m_pParent( pParent )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XSecs2Obj::~XSecs2Obj()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XSecs2Obj::TYPE XSecs2Obj::getType() const
{
XEXCEPT_ENTRY
	return m_type;
XEXCEPT_EXIT
}
XSecs2Obj* XSecs2Obj::getParentByType(XSecs2Obj::TYPE type)
{
XEXCEPT_ENTRY
	if( m_pParent == NULL ) {
		return NULL;
	}
	if( m_pParent->getType() == type ) {
		return m_pParent;
	}
	return m_pParent->getParentByType( type );
XEXCEPT_EXIT
	return NULL;
}
//-----------------------------------------------------------
//
//	XSecs2
//
//-----------------------------------------------------------
XSecs2::XSecs2()
	:	XSecs2Obj( XSecs2Obj::SECS2, NULL )
	,	m_items( NULL )
	,	m_messages( NULL )
{
XEXCEPT_ENTRY
	m_items.m_pParent = this;
	m_messages.m_pParent = this;
XEXCEPT_EXIT
}

XSecs2::~XSecs2()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

void XSecs2::clear()
{
XEXCEPT_ENTRY
	m_items.clear();
	m_messages.clear();
	m_messages.m_categories.clear();
XEXCEPT_EXIT
}

XSecs2Items& XSecs2::getItems()
{
XEXCEPT_ENTRY
	return m_items;
XEXCEPT_EXIT
}

XSecs2Messages& XSecs2::getMessages()
{
XEXCEPT_ENTRY
	return m_messages;
XEXCEPT_EXIT
}

bool XSecs2::loadXML(const TCHAR* p_file_path)
{
XEXCEPT_ENTRY
	XmlHlpErrReporter				errRepoter;

	this->clear();

	if( m_xmlDoc.loadFromFile( XStrBuff(p_file_path), &errRepoter )  == false ) {
		return false;
	}	
	XmlHlpElementActl elmTop = (XmlHlpElementRawType*)m_xmlDoc.firstChildElement();
	if( elmTop.getNodeName() != _T("secs2") ) {
		return false;
	}
	XmlHlpElementActl elmCatg = elmTop.firstChild();
	for(; elmCatg != NULL; elmCatg = elmCatg.nextSiblingElement() ) {
		if( elmCatg.getNodeName() == _T("itemTypes") ) {
			m_items.loadXML( elmCatg.m_pT );
		} else
		if( elmCatg.getNodeName() == _T("messages") ) {
			m_messages.loadXML( elmCatg.m_pT );
		}else
		if( elmCatg.getNodeName() == _T("categories") ) {
			m_messages.getCategories().loadXML( elmCatg.m_pT );
		} else {
			return false;
		}
	}
XEXCEPT_EXIT
	return true;
}
bool XSecs2::saveXML(const TCHAR* p_file_path)
{
XEXCEPT_ENTRY
	XmlHlpErrReporter				errRepoter;

	if( m_xmlDoc == NULL ) {
		return false;
	}
	if( m_xmlDoc.saveFromFile( XStrBuff(p_file_path), &errRepoter )  == false ) {
		return false;
	}
XEXCEPT_EXIT
	return true;
}



//-----------------------------------------------------------
//
//	XSecs2ItemData
//
//-----------------------------------------------------------
XSecs2ItemData::XSecs2ItemData(XmlHlpElementRawType* pElem, XSecs2Obj* pParent)
	:	XSecs2Obj( XSecs2Obj::DATA, pParent )
	,	m_pElem( pElem )
	,	m_bDistribute( false )
	,	m_pos( NULL )
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSecs2ItemData::~XSecs2ItemData()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

bool XSecs2ItemData::isDistribute() const 
{
XEXCEPT_ENTRY
	return m_bDistribute;
XEXCEPT_EXIT
}

bool XSecs2ItemData::remove()
{
XEXCEPT_ENTRY
	if( m_pParent == NULL ) {
		return false;
	}
	if( m_pParent->getType() == XSecs2Obj::DATA ) {
		XSecs2ItemData* pParent = (XSecs2ItemData*)m_pParent;
		if( pParent->isDistribute() == true ) {
			return false;
		}
		return ((XSecs2ItemDataL*)pParent)->innerRemove( this );
	} else
	if( m_pParent->getType() == XSecs2Obj::FORMAT ) {
//		XSecs2ItemFormat* pParent = (XSecs2ItemFormat*)m_pParent;
//		pParent->m_pListItem = NULL;
//		delete this;
		return false;
	} else
	if( m_pParent->getType() == XSecs2Obj::MESSAGE ) {
		XSecs2Message* pParent = (XSecs2Message*)m_pParent;
		return pParent->removeBody( );
	} else {
		_XASSERT( 0 );
	}
XEXCEPT_EXIT
	return false;
}

bool XSecs2ItemData::innerExpandRealMsg(XRichData* pRData)
{
XEXCEPT_ENTRY
	if( m_bDistribute == false ) {
		XSecs2ItemDataL* pL = (XSecs2ItemDataL*)this;
		int cnt = pL->getListCnt();
		if( cnt != -1 ) {
			XSecs2ItemData* pItem = pL->getHeadItem();
			for(; pItem != NULL; pItem = pItem->nextSibling() ) {
				XRichData* pRChild = NULL;
				if( pItem->isDistribute() == false ) {
					pRChild = pRData->addChiled( XDATA_TYPE_L );
				} else {
					XSecs2Item* pDstItem = ((XSecs2ItemDataD*)pItem)->getDistItem();
					if( pDstItem == NULL ) {
						return false;
					}
					pRChild = pRData->addChiled( pDstItem->getDataType() );
				}
				pRChild->setUserData( pItem );
				pItem->innerExpandRealMsg( pRChild );
			}
		}
	} else {
		
	}
XEXCEPT_EXIT
	return true;
}

bool XSecs2ItemData::innerSetRealMessage(XRichData* pSrcRData, XRichData* pDstRData)
{
XEXCEPT_ENTRY
	if( m_bDistribute == false ) {
		if( pSrcRData->getType() != XDATA_TYPE_L ) {
			return false;
		}
		XSecs2ItemDataL* pL = (XSecs2ItemDataL*)this;
		int cnt = pL->getListCnt();
		if( cnt != -1 ) {
			// リスト数は固定である
			if( pSrcRData->getChiledCount() != cnt ) {
				return false;
			}
			XSecs2ItemData* pItem = pL->getHeadItem();
			for(XD_UINT4 i = 0; pItem != NULL; pItem = pItem->nextSibling(), i++ ) {
				XRichData* pSrcChld = pSrcRData->getChiled( i );
				if( pSrcChld == NULL ) {
					return false;
				}
				
				XRichData* pRChild = NULL;
				if( pItem->isDistribute() == false ) {
					pRChild = pDstRData->addChiled( XDATA_TYPE_L );
				} else {
					XSecs2Item* pDstItem = ((XSecs2ItemDataD*)pItem)->getDistItem();
					if( pDstItem == NULL ) {
						return false;
					}
					pRChild = pDstRData->addChiled( pDstItem->getDataType() );
				}
				pRChild->setUserData( pItem );
				pItem->innerSetRealMessage( pSrcChld, pRChild );
			}
		} else {
			XSecs2ItemData* pItem = pL->getHeadItem();
			if( pSrcRData->getChiledCount() != 0 ) {
				// 実リスト数は、定義リスト数で割り切れるはず
				if( ( pSrcRData->getChiledCount() % pL->m_items.GetCount() ) != 0 ) {
					return false;
				}
			}
			
			XTRACE(_T("IPOS_START \n"));
			for(XD_UINT4 i = 0; i < pSrcRData->getChiledCount(); i++ ) {
				XRichData* pSrcChld = pSrcRData->getChiled( i );
				if( pSrcChld == NULL ) {
					return false;
				}
				int ipos = 0;
				if( (i+1) > (XD_UINT4)pL->m_items.GetCount() ) {
					ipos = i % pL->m_items.GetCount();
				} else {
					ipos = i;
				}
				XTRACE(_T("IPOS=%d (%d/%d)\n"), ipos, pL->m_items.GetCount(), i);
				if( ipos >= pL->m_items.GetCount() ) {
					_XASSERT( 0 );
				}
				XPOSITION pos = pL->m_items.FindIndex( ipos );
				if( pos == NULL ) {
					return false;
				}
				XSecs2ItemData* pItem = (XSecs2ItemData*)pL->m_items.GetAt( pos );
				if( pItem == NULL ) {
					return false;
				}


				XRichData* pRChild = NULL;
				pRChild = pDstRData->addChiled( pSrcChld->getType() );
				pRChild->setUserData( pItem );
				if( pItem->innerSetRealMessage( pSrcChld, pRChild ) == false ) {
					return false;
				}
			}
		}
	} else {
		XSecs2ItemDataD* pD = (XSecs2ItemDataD*)this;
		XSecs2Item* pFmt =  pD->getDistItem();
		if( pFmt == NULL ) {
			return false;
		}
		if( pFmt->getIsFixVals() == true ) {
			
		} else {
			
		}
		pDstRData->copy( pSrcRData );
	}
XEXCEPT_EXIT
	return true;
}


XSecs2ItemData* XSecs2ItemData::nextSibling()
{
XEXCEPT_ENTRY
	XPOSITION pos = m_pos;
	if( pos == NULL ) {
		return NULL;
	}
	if( m_pParent == NULL ) {
		return NULL;
	}
	if( m_pParent->getType() != XSecs2Obj::DATA ) {
		return NULL;
	}
	if( ((XSecs2ItemData*)m_pParent)->isDistribute() == true ) {
		return NULL;
	}
	XSecs2ItemDataL* pL = (XSecs2ItemDataL*)m_pParent;
	
	XSecs2ItemData *pItem = (XSecs2ItemData*)pL->m_items.GetNext( pos );
	if( pos == NULL ) {
		return NULL;
	}
	return (XSecs2ItemData*)pL->m_items.GetNext( pos );
XEXCEPT_EXIT
	return NULL;
}
bool XSecs2ItemData::validate(XRichData* pData, bool bThrow /*= true*/)
{
XEXCEPT_ENTRY
	return true;
XEXCEPT_EXIT
	return false;
}

//-----------------------------------------------------------
//
//	XSecs2ItemDataL
//
//-----------------------------------------------------------
XRichData* XSecs2ItemDataL::addReal(XRichData* pRParent)
{
XEXCEPT_ENTRY

	XSecs2ItemData* pItem = this->getHeadItem();
	if( pItem == NULL ) {
		return NULL;
	}
	XRichData* pRChild = NULL;
	if( pItem->isDistribute() == false ) {
		pRChild = pRParent->addChiled(XDATA_TYPE_L);
	} else {
		XSecs2Item* pDstItem = ((XSecs2ItemDataD*)pItem)->getDistItem();
		if( pDstItem == NULL ) {
			return NULL;
		}
		pRChild = pRParent->addChiled( pDstItem->getDataType() );
	}
	pRChild->setUserData( pItem );
	pItem->innerExpandRealMsg( pRChild );
	return pRChild;
XEXCEPT_EXIT
	return NULL;
}


XSecs2ItemDataL::XSecs2ItemDataL(XmlHlpElementRawType* pElem, XSecs2Obj* pParent)
	:	XSecs2ItemData(pElem, pParent)
{
XEXCEPT_ENTRY
	loadXML( pElem );
XEXCEPT_EXIT
}
XSecs2ItemDataL::~XSecs2ItemDataL()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}

void XSecs2ItemDataL::clear()
{
XEXCEPT_ENTRY
	XPOSITION pos1, pos2;
	for( pos1 = m_items.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
		XSecs2ItemData *pNeItem = (XSecs2ItemData*)m_items.GetNext( pos1 );
		delete pNeItem; 
		m_items.RemoveAt( pos2 );
	}
XEXCEPT_EXIT
}
int XSecs2ItemDataL::getListCnt() const
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return -1;
	}
	XmlHlpElementActl elem( m_pElem );
	if( elem.isAttribute(XmlT("cnt")) == false ) {
		return -1;
	}
	return elem.getAttributeInt(XmlT("cnt"));
XEXCEPT_EXIT
}

void XSecs2ItemDataL::setListCnt(int val)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	if( val == -1 ) {
		XmlHlpElementActl( m_pElem ).removeAttr( XmlT("cnt") );
	} else {
		XmlHlpElementActl( m_pElem ).setAttribute(XmlT("cnt"), val);
	}
XEXCEPT_EXIT
}

XStrBuff XSecs2ItemDataL::getCaption()  const
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return _T("");
	}
	XmlHlpElementActl elem( m_pElem );
	if( elem.isAttribute(XmlT("caption")) == false ) {
		return _T("");
	}
	return elem.getAttribute(XmlT("caption"));
XEXCEPT_EXIT
}

void XSecs2ItemDataL::setCaption(XStrBuff str)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return ;
	}
	if( str.GetLength() == 0  ) {
		XmlHlpElementActl( m_pElem ).removeAttr( XmlT("caption") );
	} else {
		XmlHlpElementActl( m_pElem ).setAttribute(XmlT("caption"), str);
	}
XEXCEPT_EXIT
}

XSecs2ItemDataD* XSecs2ItemDataL::addDistribute( const TCHAR* pDistName)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return NULL;
	}

	XmlHlpElementActl newElem = XmlHlpElementActl(m_pElem).addElement(_T("item"));
	newElem.setAttribute( XmlT("dstItem"), (const TCHAR*)XStrBuff(pDistName));

	XSecs2ItemDataD* pNeItem = new XSecs2ItemDataD( newElem.m_pT, this);
	pNeItem->m_pos = m_items.AddTail( pNeItem );
	return pNeItem;
XEXCEPT_EXIT
}
XSecs2ItemDataL* XSecs2ItemDataL::addList()
{
XEXCEPT_ENTRY

	if( m_pElem == NULL ) {
		return NULL;
	}

	XmlHlpElementActl newElem = XmlHlpElementActl(m_pElem).addElement(_T("list"));

	XSecs2ItemDataL* pNeItem = new XSecs2ItemDataL( newElem.m_pT, this);
	pNeItem->m_pos = m_items.AddTail( pNeItem );
	return pNeItem;
XEXCEPT_EXIT
}
bool XSecs2ItemDataL::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XStrBuff	strErr;

	m_pElem = pElem;
	XmlHlpElementActl elem = pElem;
	XmlHlpElementActl elem1 = elem.firstChild();
	for(; elem1 != NULL; elem1 = elem1.nextSiblingElement() ) {
		if( elem1.getNodeName() == _T("list") ) {
			XSecs2ItemDataL* pNeItem = new XSecs2ItemDataL( elem1.m_pT, this);
			pNeItem->m_pos = m_items.AddTail( pNeItem );
		} else
		if( elem1.getNodeName() == _T("item") ) {
			XSecs2ItemDataD* pNeItem = new XSecs2ItemDataD( elem1.m_pT, this);
			pNeItem->m_pos = m_items.AddTail( pNeItem );
		}
	}
XEXCEPT_EXIT
	return true;
}


bool XSecs2ItemDataL::innerRemove(XSecs2ItemData* pItem)
{
XEXCEPT_ENTRY
	if( m_pElem == NULL ) {
		return false;
	}
	XmlHlpElementActl elem = m_pElem;



	//既に同じタイプがセットされていないか？
	XSecs2ItemData* pListItem = this->getHeadItem();

	while( pListItem != NULL ) {
		if( pListItem == pItem ) {
			elem.removeChild( pListItem->m_pElem );	// XML
			m_items.RemoveAt( pListItem->m_pos );		// XList
			delete pListItem;							// <object>
			return true;
		}
		pListItem = pListItem->nextSibling();
	}

XEXCEPT_EXIT
	return true;
}

XSecs2ItemData* XSecs2ItemDataL::getHeadItem()
{
XEXCEPT_ENTRY
	if( m_items.IsEmpty() ) {
		return NULL;
	}
	return (XSecs2ItemData*)m_items.GetHead();	
XEXCEPT_EXIT
}

bool XSecs2ItemDataL::validate(XRichData* pData, bool bThrow /*= true*/)
{
XEXCEPT_ENTRY
	
	// フォーマットが一致しているか？
	if( pData->getType() != XDATA_TYPE_L ) {
		if( bThrow == true ) {
			XStrBuff strErrorMsg = _T("invalid data type. must be List. tag:");
			strErrorMsg << (const TCHAR*)getCaption();
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
		}
		return false;
	}
	// リスト数が一致しているか？
	int cnt = getListCnt();
	if( cnt != -1 ) {
		// 固定リスト数
		if( pData->getChiledCount() != cnt ) {
			if( bThrow == true ) {
				XStrBuff strErrorMsg;
				strErrorMsg.Format(_T("list count unmach. defined(%d) specify(%d). tag:"),
					cnt, pData->getChiledCount());
				strErrorMsg << (const TCHAR*)getCaption();
				XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
			}
			return false;
		}
	}

	for(XD_UINT4 i = 0; i < pData->getChiledCount(); i++) {
		XRichData* pR = pData->getChiled( i );
		_XASSERT( pR );
		XSecs2ItemData* pItem = (XSecs2ItemData*)pR->getUserData();
		if( pItem == NULL ) {
			if( bThrow == true ) {
				XStrBuff strErrorMsg = _T("XSecs2ItemData is null. tag:");
				strErrorMsg << (const TCHAR*)getCaption();
				XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
			}
			return false;
		}
		if( pItem->validate( pR, bThrow) == false ) {
			return false;
		}
	}
	return true;
XEXCEPT_EXIT
}
//-----------------------------------------------------------
//
//	XSecs2ItemDataD
//
//-----------------------------------------------------------
XSecs2ItemDataD::XSecs2ItemDataD(XmlHlpElementRawType* pElem, XSecs2Obj* pParent)
	:	XSecs2ItemData(pElem, pParent)
	,	m_pDistItem( NULL )
{
XEXCEPT_ENTRY
	loadXML( pElem );
XEXCEPT_EXIT
}
XSecs2ItemDataD::~XSecs2ItemDataD()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSecs2ItemDataD::clear()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XStrBuff& XSecs2ItemDataD::getDistName()
{
XEXCEPT_ENTRY
	return m_strDistName;
XEXCEPT_EXIT
}

XSecs2Item* XSecs2ItemDataD::getDistItem()
{
XEXCEPT_ENTRY
	if( m_pDistItem != NULL ) {
		return m_pDistItem;
	}
	XSecs2* pSecs2 = (XSecs2*)this->getParentByType( XSecs2Obj::SECS2 );
	if( pSecs2 == NULL ) {
		return NULL;
	}
	m_pDistItem = pSecs2->getItems().getItemByName( m_strDistName );
XEXCEPT_EXIT
	return m_pDistItem;
}
bool XSecs2ItemDataD::loadXML(XmlHlpElementRawType* pElem)
{
XEXCEPT_ENTRY
	XStrBuff	strErr;

	m_pElem = pElem;
	XmlHlpElementActl elem = pElem;

	if( elem.getNodeName() == _T("item") ) {
		if( elem.isAttribute(_T("dstItem")) == false ) {
			strErr = _T("attribute(dstItem) not found. path:[");
			elem.getPathString( strErr );
			strErr << _T("]");

			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErr, 0, 0);

			return NULL;
		}
		m_bDistribute = true;
		m_strDistName = elem.getAttribute(_T("dstItem"));
	}
XEXCEPT_EXIT
	return true;
}

bool XSecs2ItemDataD::validate(XRichData* pData, bool bThrow /*= true*/)
{
XEXCEPT_ENTRY
	XSecs2Item* pDistItem = getDistItem();
	if( pDistItem == NULL ) {
		if( bThrow == true ) {
			XStrBuff strErrorMsg = _T("dist item not defined. tag:");
			strErrorMsg << getDistName();
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
		}
		return false;
	}
	XSecs2ItemFormat* pFormat = NULL;
	if( pDistItem->getIsMultiFmt() == false ) {
		// マルチフォーマットではない
		pFormat = pDistItem->getCurrentFormat();
		if( pFormat == NULL ) {
			if( bThrow == true ) {
				XStrBuff strErrorMsg = _T("format not defined. tag:");
				strErrorMsg << getDistName();
				XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
			}
			return false;
		}
		
		// フォーマットが一致しているか？
		if( pFormat->isEqulDataType( pData->getType() ) == false ) {
			if( bThrow == true ) {
				XStrBuff strErrorMsg;
				strErrorMsg.Format(_T("invalid data type. must be %s. tag:"), XData::getDataTypeStringA_From_TYPE( pFormat->getDataType() ));
				strErrorMsg << getDistName();
				XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
			}
			return false;
		}
	} else {
		// マルチフォーマットの場合
		bool bFindFormat = false;
		pFormat = pDistItem->getFormats().getHeadItem();
		for(; pFormat != NULL; pFormat = pFormat->nextSibling() ) {
			if( pFormat->isEqulDataType( pData->getType() ) == true ) {
				bFindFormat = true;
				break;
			}
		}
		if( bFindFormat == false ) {
			XStrBuff strErrorMsg;
			strErrorMsg.Format(_T("invalid data type. tag:"));
			strErrorMsg << getDistName();
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
		}
	}

	// サイズ最小値
	if( pFormat->isSizeMin() == true ) {
		if( pFormat->getSizeMin() > (int)pData->getDataCount() ) {
			XStrBuff strErrorMsg;
			strErrorMsg.Format(_T("minimum size error. defiend(%d) specify(%d) tag:"),
				pFormat->getSizeMin() , (int)pData->getDataCount() 
				);
			strErrorMsg << getDistName();
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
		}
	}
	// サイズ最大
	if( pFormat->isSizeMax() == true ) {
		if( pFormat->getSizeMax() < (int)pData->getDataCount() ) {
			XStrBuff strErrorMsg;
			strErrorMsg.Format(_T("maximum size error. defiend(%d) specify(%d) tag:"),
				pFormat->getSizeMax() , (int)pData->getDataCount() 
				);
			strErrorMsg << getDistName();
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
		}
	}
	if( pDistItem->getIsFixVals() ) {
		// 固定値の場合
		bool bFindFixValue = false;
		XSecs2ItemValue* pValue = pFormat->getValueHeadItem();
		for(;	pValue != NULL;		pValue = pValue->nextSibling() ) {
			if( pData->compare( pValue ) == 0 ) {
				bFindFixValue = true;
				break;
			}
		}		
		if( bFindFixValue == false ) {
			XStrBuff strErrorMsg;
			strErrorMsg.Format(_T("unknown value."));
			strErrorMsg << (const TCHAR*)pData->toString();
			strErrorMsg << _T(". tag:");
			strErrorMsg << getDistName();
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)strErrorMsg, 0, 0);
		}
	}
	return true;
XEXCEPT_EXIT
}






