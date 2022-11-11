#include "xlib/xeception.h"
#include "xlib/semi/gem/xsemi_ceid.h"
#include "xlib/semi/gem/xsemi_rpt.h"
#include "xlib/semi/gem/xsemi_gem_const.h"




//-----------------------------------------------------------------------------------------
//
//	XSEMI_CEID
//
//-----------------------------------------------------------------------------------------
XSEMI_CONST_SCHEMA	XSEMI_CEID::g_pSchema[] = {
		{XSEMI_DATA_CLMN_CeID,			XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_EventName,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_DisplayName,	XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_Comment,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_Enable,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{_T(""),							XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_CEID::XSEMI_CEID(XD_UINT4 ceid, const TCHAR* p_name, const TCHAR* p_comment)
	:	XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::CEID)
	,	m_ceid(ceid)
	,	m_strName(p_name)
	,	m_strDisplayName(_T(""))
	,	m_strComment(p_comment)
	,	m_bEnable(true)
	,	m_ulUserData( 0 )
	,	m_rpts( true )
	,	m_bDirtyLinked( false )
{
XEXCEPT_ENTRY
	m_rpts.m_pParentObject	= this;
	m_pBaseSchema	= g_pSchema;
XEXCEPT_EXIT
}

XSEMI_CEID::~XSEMI_CEID()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XD_UINT4 XSEMI_CEID::get_CEID() const
{
XEXCEPT_ENTRY
	return m_ceid;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_CEID::get_Name()
{
XEXCEPT_ENTRY
	return m_strName;
XEXCEPT_EXIT
}
void XSEMI_CEID::set_Name(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strName = p_name;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_CEID::get_DisplayName()
{
XEXCEPT_ENTRY
	return m_strDisplayName;
XEXCEPT_EXIT
}
void XSEMI_CEID::set_DisplayName(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strDisplayName = p_name;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_CEID::get_Comment()
{
XEXCEPT_ENTRY
	return m_strComment;
XEXCEPT_EXIT
}
void XSEMI_CEID::set_Comment(const TCHAR* p_comment)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strComment = p_comment;
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_CEID::get_UserData() const
{
XEXCEPT_ENTRY
	return m_ulUserData;
XEXCEPT_EXIT
}
void XSEMI_CEID::set_UserData(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulUserData = val;
XEXCEPT_EXIT
}

bool XSEMI_CEID::get_Enable() const
{
XEXCEPT_ENTRY
	return m_bEnable;
XEXCEPT_EXIT
}
void XSEMI_CEID::put_Enable(bool val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_bEnable = val;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_CEID::get_RptsCount()
{
XEXCEPT_ENTRY
	return m_rpts.count();
XEXCEPT_EXIT
}
XSEMI_Rpt* XSEMI_CEID::get_Rpt(XD_UINT4 index)
{
XEXCEPT_ENTRY
	return m_rpts.get(index);
XEXCEPT_EXIT
}
XSEMI_Rpt* XSEMI_CEID::get_RptByID(XD_UINT4 rptid)
{
XEXCEPT_ENTRY
	return m_rpts.get_byID(rptid);
XEXCEPT_EXIT
}


// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_CEID::getAtomName()
{
XEXCEPT_ENTRY
	return _T("CEID");
XEXCEPT_EXIT
}
int XSEMI_CEID::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_ENABLE+1;
XEXCEPT_EXIT
}
void* XSEMI_CEID::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	switch( index ) {
	case ATOM_POS_CEID:
		return &m_ceid;
		break;
	case ATOM_POS_NAME:
		return &m_strName;
		break;
	case ATOM_POS_DISPLAY_NAME:
		return &m_strDisplayName;
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

void XSEMI_CEID::makeInsertString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_GemDataAtom::makeInsertString( stream );
	makeEventLinkClearString( stream );
XEXCEPT_EXIT
}
void XSEMI_CEID::makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_GemDataAtom::makeUpdateString( stream );
	makeEventLinkClearString( stream );
XEXCEPT_EXIT
}
void XSEMI_CEID::makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_GemDataAtom::makeDeleteString( stream );
	
	stream << _T("@@@") << std::endl;
	stream << _T("delete from ") << _T("CEID_RPTID");
	stream << _T(" where ") << XSEMI_DATA_CLMN_CeID << _T("=");
	convSQLString(m_ceid, stream);
	stream << _T(";");
XEXCEPT_EXIT
}
void XSEMI_CEID::makeEventLinkClearString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY

	stream << _T("@@@") << std::endl;
	stream << _T("delete from ") << _T("CEID_RPTID");
	stream << _T(" where ") << XSEMI_DATA_CLMN_CeID << _T("=");
	convSQLString(m_ceid, stream);
	stream << _T(";");

	for(XD_UINT4 i = 0; i < this->get_RptsCount(); i++) {
		XSEMI_Rpt* pRpt = this->get_Rpt(i);

		stream << _T("@@@") << std::endl;

		stream << _T("insert into ") << _T("CEID_RPTID");
		stream << _T(" values(");
		convSQLString(m_ceid, stream);
		stream << _T(", ");
		convSQLString(i+1, stream);
		stream << _T(", ");
		convSQLString(pRpt->get_RptID(), stream);

		stream << _T(", 1, ''");

		stream << _T(");");
	}
XEXCEPT_EXIT
}

//
//	XSEMI_CEIDs
//
XSEMI_CEIDs::XSEMI_CEIDs()
	:	XSEMI_GemConstObject(XSEMI_GemConstObject::CEIDs)
{
XEXCEPT_ENTRY
	m_ite = m_ceid.begin();
XEXCEPT_EXIT
}
XSEMI_CEIDs::~XSEMI_CEIDs()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSEMI_CEIDs::clear()
{
XEXCEPT_ENTRY
	std::vector<XSEMI_CEID*>::iterator iteCEID = m_ceid.begin();	
	for( ; iteCEID != m_ceid.end(); iteCEID++) {
		delete (*iteCEID);
	}
	m_ceid.clear();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_CEIDs::count()
{
XEXCEPT_ENTRY
	return m_ceid.size();
XEXCEPT_EXIT
}

XSEMI_CEID* XSEMI_CEIDs::get(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_ceid.size()  ) {
		return NULL;
	}
	return m_ceid[index];
XEXCEPT_EXIT	
}

XSEMI_CEID* XSEMI_CEIDs::get_byID(XD_UINT4 id)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_CEID*>::iterator iteCEID = m_ceid.begin();	
	for( ; iteCEID != m_ceid.end(); iteCEID++) {
		if( (*iteCEID)->get_CEID() == id ) {
			return (*iteCEID);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}

XSEMI_CEID* XSEMI_CEIDs::get_byName(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_CEID*>::iterator iteCEID = m_ceid.begin();	
	for( ; iteCEID != m_ceid.end(); iteCEID++) {
		if( XStrBuff::StrCmp((*iteCEID)->get_Name(), p_name) == 0 ) {
			return (*iteCEID);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}
XSEMI_CEID* XSEMI_CEIDs::getFirstItem()
{
XEXCEPT_ENTRY
	m_ite = m_ceid.begin();
	if( m_ite == m_ceid.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT	
}
XSEMI_CEID* XSEMI_CEIDs::getNextItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_ceid.end() ) {
		return NULL;
	}
	m_ite++;
	if( m_ite == m_ceid.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_CEID* XSEMI_CEIDs::getPrevItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_ceid.begin() ) {
		return NULL;
	}
	m_ite--;
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_CEID* XSEMI_CEIDs::add(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_CEID* pNewItem = NULL;
	XD_UINT4 ceid       = pAtom->getUint(XSEMI_DATA_CLMN_CeID);
	XStrBuff strName    = pAtom->getString(XSEMI_DATA_CLMN_EventName);
	XStrBuff strDispName= pAtom->getString(XSEMI_DATA_CLMN_DisplayName);
	XStrBuff strComment = pAtom->getString(XSEMI_DATA_CLMN_Comment);


	pNewItem = get_byID( ceid );
	if( pNewItem != NULL ) {
		// Šù‚É“¯‚¶CEID‚ª‚ ‚é
		return NULL;
	}
	pNewItem = new XSEMI_CEID(ceid, strName, strComment);
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->m_pParentObject = this;
	m_ceid.push_back( pNewItem );
	pNewItem->set_DisplayName( strDispName );
	pNewItem->put_Enable( pAtom->getUint(XSEMI_DATA_CLMN_Enable) == 1 );

	return pNewItem;
XEXCEPT_EXIT	
}
XSEMI_CEID* XSEMI_CEIDs::add(XD_UINT4 ceid, const TCHAR* p_name)
{
XEXCEPT_ENTRY
	XSEMI_CEID* pNewItem = NULL;
	XSEMI_GemConst* pConst = (XSEMI_GemConst*)this->getParentByType( XSEMI_GemConstObject::CONSTRUCTION );
	if( pConst == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Const Object not found. on parent"), 0, 0);
		return NULL;
	}
	XSEMI_GemMstData* pMater = pConst->get_MasterData();
	if( pMater == NULL ) {
		XEXCEPT_THROW_STR(XExcept_APP, _T("Master Object not found. on parent"), 0, 0);
		return NULL;
	}

	pNewItem = get_byID( ceid );
	if( pNewItem != NULL ) {
		// Šù‚É“¯‚¶CEID‚ª‚ ‚é
		return NULL;
	}
	pNewItem = new XSEMI_CEID(ceid, p_name, _T(""));
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->set_DisplayName( p_name );
	pNewItem->put_Enable( true );
	// DB‚ÖXV‚·‚éB
	try {
		pMater->updateObject( pNewItem );
	} catch(...) {
		delete pNewItem;
		throw;
	}
	pNewItem->m_pParentObject = this;
	m_ceid.push_back( pNewItem );
	return pNewItem;
XEXCEPT_EXIT	
}

bool XSEMI_CEIDs::del(XSEMI_CEID* pCEID)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_CEID*>::iterator iteCEID = m_ceid.begin();	
	for( ; iteCEID != m_ceid.end(); iteCEID++) {
		XSEMI_CEID* pCEIDef = ( *iteCEID );
		if( pCEIDef == pCEID ) {

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

			pMater->deleteObject( pCEIDef );

			m_ceid.erase( iteCEID );
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}