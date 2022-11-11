#include "xlib/semi/gem/xsemi_ackcode.h"
#include "xlib/xeception.h"
#include "xlib/semi/gem/xsemi_gem_const.h"



XSEMI_CONST_SCHEMA	XSEMI_ACKCODE::g_pSchema[] = {
		{XSEMI_DATA_CLMN_ACKKIND,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_ACKCODE,		XDATA_TYPE_U1,		XSEMI_CONST_SCHEMA_RTYPE_UINT1, true}
	,	{XSEMI_DATA_CLMN_Comment,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_Enable,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{_T(""),							XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_ACKCODE::XSEMI_ACKCODE(XD_UINT4 ackkind, XD_UINT1 code)
	:	XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::ACKCODE)
	,	m_ulAckKind( ackkind )
	,	m_ulAckCode( code )
	,	m_strComment( _T("") )
	,	m_bEnable( true )
{
XEXCEPT_ENTRY
	m_pBaseSchema	= g_pSchema;
XEXCEPT_EXIT
}


XSEMI_ACKCODE::~XSEMI_ACKCODE()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}


XD_UINT4 XSEMI_ACKCODE::get_AckKind() const
{
XEXCEPT_ENTRY
	return m_ulAckKind;
XEXCEPT_EXIT
}
XD_UINT1 XSEMI_ACKCODE::get_AckCode() const
{
XEXCEPT_ENTRY
	return m_ulAckCode;
XEXCEPT_EXIT
}
void XSEMI_ACKCODE::set_AckCode(XD_UINT1 val)
{
XEXCEPT_ENTRY
	m_ulAckCode = val;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_ACKCODE::get_Comment()
{
XEXCEPT_ENTRY
	return m_strComment;
XEXCEPT_EXIT
}

void XSEMI_ACKCODE::set_Comment(const TCHAR* p_comment)
{
XEXCEPT_ENTRY
	m_strComment = p_comment;
XEXCEPT_EXIT
}

bool XSEMI_ACKCODE::get_Enable() const
{
XEXCEPT_ENTRY
	return m_bEnable;
XEXCEPT_EXIT
}

void XSEMI_ACKCODE::set_Enable(bool val)
{
XEXCEPT_ENTRY
	m_bEnable =val;
XEXCEPT_EXIT
}


// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_ACKCODE::getAtomName()
{
XEXCEPT_ENTRY
	return _T("ACKCODES");
XEXCEPT_EXIT
}
int XSEMI_ACKCODE::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_ENABLE+1;
XEXCEPT_EXIT
}
void* XSEMI_ACKCODE::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	switch( index ) {
	case ATOM_POS_ACKKIND:
		return &m_ulAckKind;
		break;
	case ATOM_POS_ACKCODE:
		return &m_ulAckCode;
		break;
	case ATOM_POS_COMMENT:
		return &m_strComment;
		break;
	case ATOM_POS_ENABLE:
		return &m_bEnable;
		break;
	}
	return NULL;
XEXCEPT_EXIT
}



//
//	XSEMI_ACKCODEs
//	
XSEMI_ACKCODEs::XSEMI_ACKCODEs()
	:	XSEMI_GemConstObject(XSEMI_GemConstObject::ACKCODEs)
	,	m_bloaded( false )
{
XEXCEPT_ENTRY
	m_ite = m_array.begin();
XEXCEPT_EXIT
}
XSEMI_ACKCODEs::~XSEMI_ACKCODEs()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSEMI_ACKCODEs::clear()
{
XEXCEPT_ENTRY
	std::vector<XSEMI_ACKCODE*>::iterator ite = m_array.begin();	
	for( ; ite != m_array.end(); ite++) {
		delete (*ite);
	}
	m_array.clear();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_ACKCODEs::count()
{
XEXCEPT_ENTRY
	return m_array.size();
XEXCEPT_EXIT
}

XSEMI_ACKCODE* XSEMI_ACKCODEs::get(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_array.size() ) {
		return NULL;
	}
	return m_array[index];
XEXCEPT_EXIT	
}

XSEMI_ACKCODE* XSEMI_ACKCODEs::get_byID(XD_UINT4 kind, XD_UINT4 code)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_ACKCODE*>::iterator ite = m_array.begin();	
	for( ; ite != m_array.end(); ite++) {
		if( (*ite)->get_AckKind() == kind && 
			(*ite)->get_AckCode() == code ) {
			return (*ite);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}
XSEMI_ACKCODE* XSEMI_ACKCODEs::getFirstItem()
{
XEXCEPT_ENTRY
	m_ite = m_array.begin();
	if( m_ite == m_array.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT	
}
XSEMI_ACKCODE* XSEMI_ACKCODEs::getNextItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_array.end() ) {
		return NULL;
	}
	m_ite++;
	if( m_ite == m_array.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_ACKCODE* XSEMI_ACKCODEs::getPrevItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_array.begin() ) {
		return NULL;
	}
	m_ite--;
	return *m_ite;
XEXCEPT_EXIT
}

XSEMI_ACKCODE* XSEMI_ACKCODEs::add(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_ACKCODE* pNewItem = NULL;
	XD_UINT4 ackKind       = pAtom->getUint(XSEMI_DATA_CLMN_ACKKIND);
	XD_UINT4 ackCode       = pAtom->getUint(XSEMI_DATA_CLMN_ACKCODE);
	XStrBuff strComment    = pAtom->getString(XSEMI_DATA_CLMN_Comment);


	pNewItem = get_byID( ackKind, ackCode );
	if( pNewItem != NULL ) {
		// Šù‚É“¯‚¶CEID‚ª‚ ‚é
		return NULL;
	}
	pNewItem = new XSEMI_ACKCODE( ackKind, ackCode );
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->set_Comment(strComment);

	pNewItem->m_pParentObject = this;
	m_array.push_back( pNewItem );
	pNewItem->set_Enable( pAtom->getUint(XSEMI_DATA_CLMN_Enable) == 1 );

	return pNewItem;
XEXCEPT_EXIT	
}

bool XSEMI_ACKCODEs::del(XSEMI_ACKCODE* pAckCode)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_ACKCODE*>::iterator ite = m_array.begin();	
	for( ; ite != m_array.end(); ite++) {
		XSEMI_ACKCODE* pDef = ( *ite );
		if( pDef == pAckCode ) {

			XSEMI_GemConst* pConst = (XSEMI_GemConst*)this->getParentByType( XSEMI_GemConstObject::CONSTRUCTION );
			if( pConst == NULL ) {
				XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object not found. on parent"), 0, 0);
				return false;
			}
			XSEMI_GemMstData* pMater = pConst->get_MasterData();
			if( pMater == NULL ) {
				XEXCEPT_THROW_STR(XExcept_APP, _T("Master Object not found. on parent"), 0, 0);
				return false;
			}

			pMater->deleteObject( pDef );

			m_array.erase( ite );
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}


