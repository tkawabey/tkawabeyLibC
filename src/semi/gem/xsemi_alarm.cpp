#include "xlib/semi/gem/xsemi_alarm.h"
#include "xlib/xeception.h"



//
//	XSEMI_ALARM
//
XSEMI_CONST_SCHEMA	XSEMI_ALARM::g_pSchema[] = {
		{XSEMI_DATA_CLMN_ALID,			XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, true}
	,	{XSEMI_DATA_CLMN_ALCD,			XDATA_TYPE_U1,		XSEMI_CONST_SCHEMA_RTYPE_UINT1, false}
	,	{XSEMI_DATA_CLMN_AlarmName,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_AlarmText,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_OnCEID,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_OffCEID,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT4, false}
	,	{XSEMI_DATA_CLMN_Comment,		XDATA_TYPE_A,		XSEMI_CONST_SCHEMA_RTYPE_XSTR, false}
	,	{XSEMI_DATA_CLMN_Enable,		XDATA_TYPE_BOOL,	XSEMI_CONST_SCHEMA_RTYPE_BOOL, false}

	,	{_T(""),						XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_ALARM::XSEMI_ALARM(XD_UINT4 alarmID, const TCHAR* p_name)
	:	XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::ALARM)
	,	m_ulAlarmId( alarmID )
	,	m_strName( p_name )
	,	m_strText( _T("") )
	,	m_strComment( _T("") )
	,	m_ulOnEvent( 0 )
	,	m_ulOffEvent( 0 )
	,	m_bEnable( true )
{
XEXCEPT_ENTRY
	m_pBaseSchema	= g_pSchema;
XEXCEPT_EXIT
}


XSEMI_ALARM::~XSEMI_ALARM()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}


XD_UINT4 XSEMI_ALARM::get_AlarmID()
{
XEXCEPT_ENTRY
	return m_ulAlarmId;
XEXCEPT_EXIT
}
XD_UINT1 XSEMI_ALARM::get_ALCD() const
{
XEXCEPT_ENTRY
	return m_alcd;
XEXCEPT_EXIT
}
void XSEMI_ALARM::set_ALCD(XD_UINT1 val)
{
XEXCEPT_ENTRY
	m_alcd = val;
XEXCEPT_EXIT
}
const TCHAR* XSEMI_ALARM::get_Name()
{
XEXCEPT_ENTRY
	return m_strName;
XEXCEPT_EXIT
}

void XSEMI_ALARM::set_Name(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	m_strName = p_name;
XEXCEPT_EXIT
}

const TCHAR* XSEMI_ALARM::get_Text()
{
XEXCEPT_ENTRY
	return m_strText;
XEXCEPT_EXIT
}

void XSEMI_ALARM::set_Text(const TCHAR* p_text)
{
XEXCEPT_ENTRY
	m_strText = p_text;
XEXCEPT_EXIT
}

const TCHAR* XSEMI_ALARM::get_Comment()
{
XEXCEPT_ENTRY
	return m_strComment;
XEXCEPT_EXIT
}

void XSEMI_ALARM::set_Comment(const TCHAR* p_comment)
{
XEXCEPT_ENTRY
	m_strComment = p_comment;
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_ALARM::get_OnCEID() const
{
XEXCEPT_ENTRY
	return m_ulOnEvent;
XEXCEPT_EXIT
}
void XSEMI_ALARM::set_OnCEID(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulOnEvent = val;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_ALARM::get_OffCEID() const
{
XEXCEPT_ENTRY
	return m_ulOffEvent;
XEXCEPT_EXIT
}
void XSEMI_ALARM::set_OffCEID(XD_UINT4 val)
{
XEXCEPT_ENTRY
	m_ulOffEvent = val;
XEXCEPT_EXIT
}
bool XSEMI_ALARM::get_Enable() const
{
XEXCEPT_ENTRY
	return m_bEnable;
XEXCEPT_EXIT
}

void XSEMI_ALARM::set_Enable(bool val)
{
XEXCEPT_ENTRY
	m_bEnable =val;
XEXCEPT_EXIT
}



// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_ALARM::getAtomName()
{
XEXCEPT_ENTRY
	return _T("ALARM");
XEXCEPT_EXIT
}
int XSEMI_ALARM::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_ENABLE+1;
XEXCEPT_EXIT
}

void* XSEMI_ALARM::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	switch( index ) {
	case ATOM_POS_ALID:
		return &m_ulAlarmId;
		break;
	case ATOM_POS_ALCD:
		return &m_alcd;
		break;
	case ATOM_POS_NAME:
		return &m_strName;
		break;
	case ATOM_POS_TEXT:
		return &m_strText;
		break;
	case ATOM_POS_ON_CEID:
		return &m_ulOnEvent;
		break;
	case ATOM_POS_OFF_CEID:
		return &m_ulOffEvent;
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
//	XSEMI_ALARMs
//
XSEMI_ALARMs::XSEMI_ALARMs()
	:	XSEMI_GemConstObject(XSEMI_GemConstObject::ALARMs)
{
XEXCEPT_ENTRY
	m_ite = m_array.begin();
XEXCEPT_EXIT
}
XSEMI_ALARMs::~XSEMI_ALARMs()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSEMI_ALARMs::clear()
{
XEXCEPT_ENTRY
	std::vector<XSEMI_ALARM*>::iterator ite = m_array.begin();	
	for( ; ite != m_array.end(); ite++) {
		delete (*ite);
	}
	m_array.clear();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_ALARMs::count()
{
XEXCEPT_ENTRY
	return m_array.size();
XEXCEPT_EXIT
}

XSEMI_ALARM* XSEMI_ALARMs::get(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_array.size()  ) {
		return NULL;
	}
	return m_array[index];
XEXCEPT_EXIT	
}

XSEMI_ALARM* XSEMI_ALARMs::get_byID(XD_UINT4 id)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_ALARM*>::iterator ite = m_array.begin();	
	for( ; ite != m_array.end(); ite++) {
		if( (*ite)->get_AlarmID() == id ) {
			return (*ite);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}

XSEMI_ALARM* XSEMI_ALARMs::get_byName(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_ALARM*>::iterator ite = m_array.begin();	
	for( ; ite != m_array.end(); ite++) {
		if( XStrBuff::StrCmp((*ite)->get_Name(), p_name) == 0 ) {
			return (*ite);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}
XSEMI_ALARM* XSEMI_ALARMs::getFirstItem()
{
XEXCEPT_ENTRY
	m_ite = m_array.begin();
	if( m_ite == m_array.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT	
}
XSEMI_ALARM* XSEMI_ALARMs::getNextItem()
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
XSEMI_ALARM* XSEMI_ALARMs::getPrevItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_array.begin() ) {
		return NULL;
	}
	m_ite--;
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_ALARM* XSEMI_ALARMs::add(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_ALARM* pNewItem = NULL;
	XD_UINT4 alid       = pAtom->getUint(XSEMI_DATA_CLMN_ALID);
	XD_UINT4 alcd       = pAtom->getUint(XSEMI_DATA_CLMN_ALCD);
	XStrBuff strName    = pAtom->getString(XSEMI_DATA_CLMN_AlarmName);
	XStrBuff strText    = pAtom->getString(XSEMI_DATA_CLMN_AlarmText);
	XD_UINT4 onCeid     = pAtom->getUint(XSEMI_DATA_CLMN_OnCEID);
	XD_UINT4 offCeid    = pAtom->getUint(XSEMI_DATA_CLMN_OffCEID);
	XStrBuff strComment = pAtom->getString(XSEMI_DATA_CLMN_Comment);


	pNewItem = get_byID( alid );
	if( pNewItem != NULL ) {
		// Šù‚É“¯‚¶CEID‚ª‚ ‚é
		return NULL;
	}
	pNewItem = new XSEMI_ALARM(alid, strName);
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
	pNewItem->m_pParentObject = this;
	m_array.push_back( pNewItem );
	pNewItem->set_ALCD( (XD_UINT1)alcd );
	pNewItem->set_Name( strName );
	pNewItem->set_Text( strText );
	pNewItem->set_OnCEID( onCeid );
	pNewItem->set_OffCEID( offCeid );
	pNewItem->set_Comment( strComment );
	pNewItem->set_Enable( pAtom->getUint(XSEMI_DATA_CLMN_Enable) == 1 );

	return pNewItem;
XEXCEPT_EXIT	
}

bool XSEMI_ALARMs::del(XSEMI_ALARM* pAlarm)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_ALARM*>::iterator ite = m_array.begin();	
	for( ; ite != m_array.end(); ite++) {
		XSEMI_ALARM* pDef = ( *ite );
		if( pDef == pAlarm ) {
			delete pDef;
			m_array.erase( ite );
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}