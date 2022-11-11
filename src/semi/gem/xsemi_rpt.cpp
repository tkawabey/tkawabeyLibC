#include "xlib/semi/gem/xsemi_rpt.h"
#include "xlib/semi/gem/xsemi_gem_const.h"
#include "xlib/semi/gem/xsemi_ceid.h"
#include "xlib/xeception.h"


//-----------------------------------------------------------------------------------------
//
//	XSEMI_Rpt
//
//-----------------------------------------------------------------------------------------
XSEMI_CONST_SCHEMA	XSEMI_Rpt::g_pSchema[] = {
		{XSEMI_DATA_CLMN_RptID,			XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_RptName,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_Comment,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_Enable,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}
	,	{_T(""),						XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_Rpt::XSEMI_Rpt(XD_UINT4 rptid, const TCHAR* p_name)
	:	XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::RPTID)
	,	m_rptid(rptid)
	,	m_bEnable(true)
	,	m_vs( true )
	,	m_strName(p_name)
	,	m_strComment("")
{
XEXCEPT_ENTRY
	m_vs.m_pParentObject = this;
	m_pBaseSchema	= g_pSchema;
XEXCEPT_EXIT
}
XSEMI_Rpt::~XSEMI_Rpt()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_Rpt::get_RptID()
{
XEXCEPT_ENTRY
	return m_rptid;
XEXCEPT_EXIT
}
const TCHAR*XSEMI_Rpt:: get_Name()
{
XEXCEPT_ENTRY
	return m_strName;
XEXCEPT_EXIT
}
void XSEMI_Rpt::set_Name(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strName = p_name;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_Rpt::get_Comment()
{
XEXCEPT_ENTRY
	return m_strComment;
XEXCEPT_EXIT
}
void XSEMI_Rpt::set_Comment(const TCHAR* p_comment)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_strComment = p_comment;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_Rpt::get_VsCount() const
{
XEXCEPT_ENTRY
	return m_vs.count();
XEXCEPT_EXIT
}
XSEMI_V* XSEMI_Rpt::get_V(XD_UINT4 index)
{
XEXCEPT_ENTRY
	return m_vs.get(index);
XEXCEPT_EXIT
}
XSEMI_V* XSEMI_Rpt::get_VByID(XD_UINT4 vid)
{
XEXCEPT_ENTRY
	return m_vs.get_byID(vid);
XEXCEPT_EXIT
}
bool XSEMI_Rpt::get_Enable() const
{
XEXCEPT_ENTRY
	return m_bEnable;
XEXCEPT_EXIT
}
void XSEMI_Rpt::set_Enable(bool val)
{
XEXCEPT_ENTRY
	m_bDirtyMember = true;
	m_bEnable = val;
XEXCEPT_EXIT
}


// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_Rpt::getAtomName()
{
XEXCEPT_ENTRY
	return _T("RPTID");
XEXCEPT_EXIT
}
int XSEMI_Rpt::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_ENABLE+1;
XEXCEPT_EXIT
}
void* XSEMI_Rpt::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	switch( index ) {
	case ATOM_POS_RPTID:
		return &m_rptid;
		break;
	case ATOM_POS_NAME:
		return &m_strName;
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
void XSEMI_Rpt::makeInsertString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_GemDataAtom::makeInsertString( stream );

	makeDefineReportClearString( stream );
XEXCEPT_EXIT
}
void XSEMI_Rpt::makeUpdateString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_GemDataAtom::makeUpdateString( stream );

	makeDefineReportClearString( stream );
XEXCEPT_EXIT
}
void XSEMI_Rpt::makeDeleteString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	XSEMI_GemDataAtom::makeDeleteString( stream );

	stream << _T("@@@") << std::endl;
	stream << _T("delete from ") << _T("RPTID_DVID");
	stream << _T(" where RptID=");
	convSQLString(get_RptID(), stream);
	stream << _T(";");
XEXCEPT_EXIT
}
void XSEMI_Rpt::makeDefineReportClearString(XLIB_STD_OUT_STR_STREAM& stream)
{
XEXCEPT_ENTRY
	stream << _T("@@@") << std::endl;
	stream << _T("delete from ") << _T("RPTID_DVID");
	stream << _T(" where RptID=");
	convSQLString(get_RptID(), stream);
	stream << _T(";");

	for(XD_UINT4 i = 0; i < this->get_VsCount(); i++) {
		XSEMI_V* pV = this->get_V( i );

		stream << _T("@@@") << std::endl;

		stream << _T("insert into ") << _T("RPTID_DVID");
		stream << _T(" values(");
		convSQLString(get_RptID(), stream);
		stream << _T(", ");
		convSQLString(i+1, stream);
		stream << _T(", ");
		convSQLString(pV->get_VID(), stream);
		stream << _T(");");
	}
XEXCEPT_EXIT
}


//
//	XSEMI_Rpts
//


XSEMI_Rpts::XSEMI_Rpts(bool bLinkeDatas)
	:	XSEMI_GemConstObject(XSEMI_GemConstObject::RPTIDs)
	,	m_bLinkDatas( bLinkeDatas )
{
XEXCEPT_ENTRY
	m_ite = m_rpts.begin();
XEXCEPT_EXIT
}
XSEMI_Rpts::~XSEMI_Rpts()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSEMI_Rpts::clear()
{
XEXCEPT_ENTRY
	if( m_bLinkDatas == false ) {
		std::vector<XSEMI_Rpt*>::iterator iteRpt = m_rpts.begin();	
		for( ; iteRpt != m_rpts.end(); iteRpt++) {
			delete (*iteRpt);
		}
	}
	m_rpts.clear();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_Rpts::count()
{
XEXCEPT_ENTRY
	return m_rpts.size();
XEXCEPT_EXIT
}

XSEMI_Rpt* XSEMI_Rpts::get(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_rpts.size() ) {
		return NULL;
	}
	return m_rpts[index];
XEXCEPT_EXIT	
}

XSEMI_Rpt* XSEMI_Rpts::get_byID(XD_UINT4 id)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_Rpt*>::iterator iteRpt = m_rpts.begin();	
	for( ; iteRpt != m_rpts.end(); iteRpt++) {
		if( (*iteRpt)->get_RptID() == id ) {
			return (*iteRpt);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}
XSEMI_Rpt* XSEMI_Rpts::get_byName(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_Rpt*>::iterator iteRpt = m_rpts.begin();	
	for( ; iteRpt != m_rpts.end(); iteRpt++) {
		if( XStrBuff::StrCmp((*iteRpt)->get_Name(), p_name) == 0 ) {
			return (*iteRpt);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}
XSEMI_Rpt* XSEMI_Rpts::getFirstItem()
{
XEXCEPT_ENTRY
	m_ite = m_rpts.begin();
	if( m_ite == m_rpts.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT	
}
XSEMI_Rpt* XSEMI_Rpts::getNextItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_rpts.end() ) {
		return NULL;
	}
	m_ite++;
	if( m_ite == m_rpts.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_Rpt* XSEMI_Rpts::getPrevItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_rpts.begin() ) {
		return NULL;
	}
	m_ite--;
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_Rpt* XSEMI_Rpts::add(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	if( m_bLinkDatas == true ) {
		return NULL;
	}
	XSEMI_Rpt* pNewItem = NULL;
	XD_UINT4 rptid       = pAtom->getUint(XSEMI_DATA_CLMN_RptID);
	XStrBuff strName    = pAtom->getString(XSEMI_DATA_CLMN_RptName);
	XStrBuff strComment = pAtom->getString(XSEMI_DATA_CLMN_Comment);

	pNewItem = get_byID( rptid );
	if( pNewItem != NULL ) {
		// 既に同じCEIDがある
		return NULL;
	}
	pNewItem = new XSEMI_Rpt(rptid, strName);
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->m_pParentObject = this;
	pNewItem->set_Comment( strComment );
	m_rpts.push_back( pNewItem );
	pNewItem->set_Enable( pAtom->getUint(XSEMI_DATA_CLMN_Enable) == 1 );

	return pNewItem;
XEXCEPT_EXIT	
}

bool XSEMI_Rpts::del(XSEMI_Rpt* pRpt)
{
XEXCEPT_ENTRY
	XD_UINT4 id = pRpt->get_RptID();
	std::vector<XSEMI_Rpt*>::iterator iteRpt = m_rpts.begin();	
	for( ; iteRpt != m_rpts.end(); iteRpt++) {
		XSEMI_Rpt* pRptDef = ( *iteRpt );
		if( pRptDef == pRpt ) {
		
			XSEMI_GemConst *pConst = NULL;
			// CEIDに緋もづくレポートIDを削除する。
			if( m_pParentObject != NULL ) {
				if( m_pParentObject->get_ObjectType() == XSEMI_GemConstObject::CONSTRUCTION ) {
					pConst = (XSEMI_GemConst*)m_pParentObject;
				}
			}

			if( pConst != NULL ) {
				XSEMI_CEIDs& ceids = pConst->get_CEIDs();
				XSEMI_CEID* pCEID = ceids.getFirstItem();
				while( pCEID != NULL ) {
					for(XD_UINT4 i = 0; i < pCEID->get_RptsCount(); i++) {
						if( pCEID->get_Rpt(i)->get_RptID() == id ) {
							pConst->unlinkEventReport(pCEID->get_CEID(),  id);
							break;
						}
					}
					pCEID = ceids.getNextItem();
				}
			}
			if( m_bLinkDatas == false ) {
				delete pRptDef;
			}
			m_rpts.erase( iteRpt );
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}