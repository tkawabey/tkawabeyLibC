#include "xlib/semi/gem/xsemi_gem_def.h"
#include "xlib/semi/gem/xsemi_gem_const.h"
#include "xlib/semi/gem/xsemi_ceid.h"
#include "xlib/semi/gem/xsemi_rpt.h"
#include "xlib/semi/gem/xsemi_v.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/semi/secs/xsecs_text_parser.h"



//
//	XSEMI_GemConst
//
XSEMI_GemConst::XSEMI_GemConst()
	:	XSEMI_GemConstObject( XSEMI_GemConstObject::CONSTRUCTION )
	,	m_pMasterData(NULL)
	,	m_rpts( false )
	,	m_vs( false )
{
XEXCEPT_ENTRY
	m_ceid.m_pParentObject	= this;
	m_rpts.m_pParentObject	= this;
	m_vs.m_pParentObject	= this;
	m_variable_grps.m_pParentObject	= this;
	m_alarms.m_pParentObject	= this;
	m_ackcodes.m_pParentObject= this;
XEXCEPT_EXIT
}
XSEMI_GemConst::~XSEMI_GemConst()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}

void XSEMI_GemConst::set_MasterData(XSEMI_GemMstData* pMasterData)
{
XEXCEPT_ENTRY
	if( m_pMasterData != NULL ) {
		pMasterData->m_pConst = NULL;
	}
	m_pMasterData = pMasterData;
	if( pMasterData != NULL ) {
		pMasterData->m_pConst = this;
	}
XEXCEPT_EXIT
}
bool XSEMI_GemConst::initialize(XSEMI_GemMstData* pMasterData)
{
XEXCEPT_ENTRY
	set_MasterData( pMasterData );

	return m_pMasterData->load(*this);
XEXCEPT_EXIT
}
void XSEMI_GemConst::resetDirtyAllItem()
{
XEXCEPT_ENTRY


	{
		XSEMI_CEID* pCEID = m_ceid.getFirstItem();
		while( pCEID != NULL ) {
			pCEID->resetDirtyMember();
			pCEID = m_ceid.getNextItem();
		}
	}
	{
		XSEMI_Rpt* pRpt = m_rpts.getFirstItem();
		while( pRpt != NULL ) {
			pRpt->resetDirtyMember();
			pRpt = m_rpts.getNextItem();
		}
	}
	{
		XSEMI_V* pV = m_vs.getFirstItem();
		while( pV != NULL ) {
			pV->resetDirtyMember();
			pV = m_vs.getNextItem();
		}
	}
	{
		XSEMI_ACKCODE* p = m_ackcodes.getFirstItem();
		while( p != NULL ) {
			p->resetDirtyMember();
			p = m_ackcodes.getNextItem();
		}
	}
	{
		XSEMI_VARIABLE_GRP* pV = m_variable_grps.getFirstItem();
		while( pV != NULL ) {
			pV->resetDirtyMember();
			pV = m_variable_grps.getNextItem();
		}
	}
XEXCEPT_EXIT
}
bool XSEMI_GemConst::get_ConfigValue(const TCHAR* pszConfName, XRichData &data, XStrBuff* pComment /*= NULL*/)
{
XEXCEPT_ENTRY
	if( m_pMasterData == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("not set master data."), 0, 0);
		return false;
	}
	return m_pMasterData->get_ConfigValue(pszConfName, data, pComment);
XEXCEPT_EXIT
}
bool XSEMI_GemConst::set_ConfigValue(const TCHAR* pszConfName, XRichData &data, const TCHAR* lpszComment /*= NULL*/)
{
XEXCEPT_ENTRY
	if( m_pMasterData == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("not set master data."), 0, 0);
		return false;
	}
	return m_pMasterData->set_ConfigValue(pszConfName, data, lpszComment);
XEXCEPT_EXIT
}

bool XSEMI_GemConst::loadVvalue(std::istream &stream) 
{
XEXCEPT_ENTRY
	/*
		<L.n
		  <L.2
			<U4 VID>
			<VALUSEs ...>
		  >
		>
	*/
	XInflateStreamBuff	buff(2048, 2048);
	std::ostream		tempStream(&buff);
	char				szTemp[2048];



	//
	// ストリームの情報を一旦バッファに更新します。
	//
	while(1) {
		stream.read(szTemp, sizeof(szTemp));
		std::streamsize ireaded = stream.gcount();
		if( ireaded == 0 ) {
			break;
		}
		tempStream.write(szTemp, ireaded);
	}



	XData data;
	XDataReader reader(buff.getBuff(), buff.size());
	// L.n
	if( reader.getValue( &data ) == false ) {
		return false;
	}
	if( data.getType() != XDATA_TYPE_L ) {
		return false;
	}
	XD_UINT4 ulTopRootCnt = data.getCount();
	for(XD_UINT4 i = 0; i < ulTopRootCnt; i++) {
		XD_UINT4 ulVID;
		// L.2
		if( reader.getValue( &data ) == false ) {
			return false;
		}
		if( data.getType() != XDATA_TYPE_L ) {
			return false;
		}
		if( data.getCount() != 2 ) {
			return false;
		}
		

		// U4 VID
		if( reader.getValue( &data ) == false ) {
			return false;
		}
		if( data.getType() != XDATA_TYPE_U4 ) {
			return false;
		}
		ulVID = data.getValAsUInt4();

		// VIDから、Vオブジェクトを探す
		XSEMI_V* pV = get_Vs().get_byID( ulVID );
		if( pV == NULL ) {
			return false;
		}
		

		// Values
		if( reader.getValue( &data ) == false ) {
			return false;
		}
		if( pV->putValueFromXData(reader, data, 
			true	// 変換有
		   ) == false ) {
			return false;
		}

	}
	return true;
XEXCEPT_EXIT
}
bool XSEMI_GemConst::storeVvalue(std::ostream &stream) 
{
XEXCEPT_ENTRY

	XDataWriter	writer(&stream);
	std::vector<XSEMI_V*>::iterator ite;


	// L.n
	if( writer.putValAsList( m_vs.count() ) == false ) {
		return false;
	}

	ite = m_vs.m_vs.begin();
	for(; ite != m_vs.m_vs.end(); ite++) {
		XSEMI_V *pV = (*ite);
		
		// L.2
		if( writer.putValAsList( 2 ) == false ) {
			return false;
		}
			// U4
		if( writer.putValAsUInt4( pV->get_VID() ) == false ) {
			return false;
		}
			// VALUSEs ...
		if( pV->getType() == XDATA_TYPE_END ) {
			if( writer.putValAsList( 0 ) == false ) {
				return false;
			}
		} else {
			if( pV->buildBinaryData( &stream ) == false ) {
				return false;
			}
		}
	}
	return true;
XEXCEPT_EXIT
}

void XSEMI_GemConst::clear()
{
XEXCEPT_ENTRY

	m_ceid.clear();
	m_rpts.clear();
	m_vs.clear();
	m_alarms.clear();
	m_variable_grps.clear();
	m_ackcodes.clear();
XEXCEPT_EXIT
}


bool XSEMI_GemConst::saveECValues()
{
XEXCEPT_ENTRY

	if( m_pMasterData == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, _T("not set master data."), 0, 0);
	}
	m_pMasterData->updateObject( &m_vs );
/*
	XD_UINT4 i, loop = m_vs.countECV();
	for(i = 0; i < loop; i++) {
		XSEMI_V* pV = m_vs.getECV( i );
		if( pV->isDirtyMember() == true ) {
			m_pMasterData->update( pV );
		}
	}	
*/
	return true;
XEXCEPT_EXIT
}
bool XSEMI_GemConst::linkEventReport(XD_UINT4 ceid, XD_UINT4 rptid, XD_UINT4 bf_rptid /*= 0xFFFFFFFF*/)
{
XEXCEPT_ENTRY
	XSEMI_CEID* pCEID = NULL;
	XSEMI_Rpt*  pRpt = NULL;
	XSEMI_Rpt*  pBforeRpt = NULL;
	std::vector<XSEMI_Rpt*>::iterator iteBfRpt;

	// 指定されたCEIDを探す
	pCEID = this->get_CEIDs().get_byID(ceid);
	if( pCEID == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found CEID:%u"), ceid), 0, 0);
	}

	// 指定されたREPORTIDを探す
	std::vector<XSEMI_Rpt*>::iterator iteRpt = m_rpts.m_rpts.begin();	
	for( ; iteRpt != m_rpts.m_rpts.end(); iteRpt++) {
		// 既に同じRptIDがある
		if( (*iteRpt)->get_RptID() == rptid ) {
			pRpt = (*iteRpt);
			if( bf_rptid == 0xFFFFFFFF || pBforeRpt != NULL ) {
				break;
			}
		}
		if( (*iteRpt)->get_RptID() == bf_rptid ) {
			pBforeRpt = (*iteRpt);
			iteBfRpt = iteRpt;
			if( pRpt != NULL ) {
				break;
			}
		}
	}
	if( pRpt == NULL ) {
		XStrBuff::FormatS(_T("not found RPTID:%u"), rptid);
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found RPTID:%u"), rptid), 0, 0);
	}
	if( pBforeRpt == NULL &&  bf_rptid != 0xFFFFFFFF) {
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found RPTID:%u"), bf_rptid), 0, 0);
	}


	// 既に同じRPTIDがリンクされていないか
	if( pCEID->get_RptByID( rptid ) != NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("already linkd RPTID:%u"), bf_rptid), 0, 0);
	}


	if( pBforeRpt == NULL ) {
		pCEID->m_rpts.m_rpts.push_back( pRpt );
	} else {
		pCEID->m_rpts.m_rpts.insert( iteBfRpt, pRpt );
	}
	pCEID->m_bDirtyLinked = true;	// Dirty にセット
	pCEID->m_bDirtyMember = true;	// Dirty にセット
	m_ceid.m_bDirtyMember = true;	// Dirty にセット
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemConst::unlinkEventReport(XD_UINT4 ceid, XD_UINT4 rptid)
{
XEXCEPT_ENTRY
	XSEMI_CEID* pCEID = NULL;
	XSEMI_Rpt*  pRpt = NULL;

	// 指定されたCEIDを探す
	pCEID = m_ceid.get_byID( ceid );
	if( pCEID == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found CEID:%u"), ceid), 0, 0);
	}

	if( rptid == 0xFFFFFFFF )  {
		// CEIDに緋も付くレポートIDをすべて削除する。
		pCEID->m_rpts.clear();
	} else {
		// 指定されたREPORTIDを探す
		std::vector<XSEMI_Rpt*>::iterator iteRpt = pCEID->m_rpts.m_rpts.begin();	
		for( ; iteRpt != pCEID->m_rpts.m_rpts.end(); iteRpt++) {
			// 既に同じRptIDがある
			if( (*iteRpt)->get_RptID() == rptid ) {
				pRpt = (*iteRpt);
				break;
			}
		}
		if( pRpt == NULL ) {
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found RPTID:%u"), rptid), 0, 0);
		}
		pCEID->m_rpts.m_rpts.erase( iteRpt );
	}

	pCEID->m_bDirtyLinked = true;	// Dirty にセット
	pCEID->m_bDirtyMember = true;	// Dirty にセット
	m_ceid.m_bDirtyMember = true;	// Dirty にセット
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemConst::defineReport(XD_UINT4 rptid, XD_UINT4 vid, XD_UINT4 bf_vid /*= 0xFFFFFFFF*/)
{
XEXCEPT_ENTRY

	std::vector<XSEMI_Rpt*>::iterator iteRpt;
	std::vector<XSEMI_V*>::iterator iteV;
	XSEMI_V* pV = NULL;
	XSEMI_V* pBforeV = NULL;

	for(iteRpt = m_rpts.m_rpts.begin(); iteRpt != m_rpts.m_rpts.end(); iteRpt++) {
		if( (*iteRpt)->get_RptID() == rptid) {
			break;
		}
	}
	if( iteRpt == m_rpts.m_rpts.end() ) {
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found RPTID:%u"), rptid), 0, 0);
	}

	for(iteV = m_vs.m_vs.begin(); iteV != m_vs.m_vs.end(); iteV++) {
		if( (*iteV)->get_VID() == vid) {
			pV = (*iteV);
			break;
		}
	}
	if( pV == NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found VID:%u"), vid), 0, 0);
	}
	if( (*iteRpt)->get_VByID( vid ) != NULL ) {
		XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("already linked VID:%u"), vid), 0, 0);
	}

	if( bf_vid != 0xFFFFFFFF ) {
		for(iteV = (*iteRpt)->m_vs.m_vs.begin(); iteV != (*iteRpt)->m_vs.m_vs.end(); iteV++) {
			if( (*iteV)->get_VID() == bf_vid) {
				break;
			}
		}
		if( iteV == (*iteRpt)->m_vs.m_vs.end() ) {
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not linked VID:%u"), bf_vid), 0, 0);
		}
		(*iteRpt)->m_vs.m_vs.insert( iteV, pV );
		(*iteRpt)->set_Enable( true );
	} else {
		(*iteRpt)->m_vs.m_vs.push_back( pV );
		(*iteRpt)->set_Enable( true );
	}
	(*iteRpt)->m_bDirtyMember = true;
	m_rpts.m_bDirtyMember = true;	// Dirty にセット
	return true;
XEXCEPT_EXIT
}

bool XSEMI_GemConst::unDefineReport(XD_UINT4 rptid, XD_UINT4 vid)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_Rpt*>::iterator iteRpt;
	std::vector<XSEMI_V*>::iterator iteV;
	XSEMI_V* pV = NULL;

	if( rptid == 0xFFFFFFFF) {
		// 全件削除
		XSEMI_Rpt* pRpt = m_rpts.getFirstItem();
		while( pRpt != NULL ) {
			pRpt->m_vs.clear();
			pRpt->m_bDirtyMember = true;
			pRpt->set_Enable( false );
			pRpt = m_rpts.getNextItem();
		}
	} else {
		for(iteRpt = m_rpts.m_rpts.begin(); iteRpt != m_rpts.m_rpts.end(); iteRpt++) {
			if( (*iteRpt)->get_RptID() == rptid) {
				break;
			}
		}
		if( iteRpt == m_rpts.m_rpts.end() ) {
			XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found RPTID:%u"), rptid), 0, 0);
		}

		if( vid != 0xFFFFFFFF) {
			for(iteV = m_vs.m_vs.begin(); iteV != m_vs.m_vs.end(); iteV++) {
				if( (*iteV)->get_VID() == vid) {
					pV = (*iteV);
					break;
				}
			}
			if( pV == NULL ) {
				XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not found VID:%u"), vid), 0, 0);
			}

			for(iteV = (*iteRpt)->m_vs.m_vs.begin(); iteV != (*iteRpt)->m_vs.m_vs.end(); iteV++) {
				if( (*iteV)->get_VID() == vid) {
					break;
				}
			}
			if( iteV == (*iteRpt)->m_vs.m_vs.end() ) {
				XEXCEPT_THROW_STR(XExcept_SECS, (const TCHAR*)XStrBuff::FormatS(_T("not linked VID:%u"), vid), 0, 0);
			}
			(*iteRpt)->m_vs.m_vs.erase( iteV );
			if( (*iteRpt)->get_VsCount() == 0 ) {
				(*iteRpt)->set_Enable( false );
			}
		} else {
			(*iteRpt)->m_vs.m_vs.clear();
			(*iteRpt)->set_Enable( false );
		}
		(*iteRpt)->m_bDirtyMember = true;
		m_rpts.m_bDirtyMember = true;	// Dirty にセット
	}
	return true;
XEXCEPT_EXIT
}


XSEMI_Rpt* XSEMI_GemConst::setRpt(XD_UINT4 ceid, XD_UINT4 rptid, XD_UINT4 preRptID /*= 0xFFFFFFFF*/)
{
XEXCEPT_ENTRY
	
	XSEMI_CEID* pCEID = NULL;
	XD_UINT4 i;
	XSEMI_Rpt* pPreRpt = NULL;
	std::vector<XSEMI_Rpt*>::iterator iteRpt = m_rpts.m_rpts.begin();	

	// 指定したCEIDがあるか調べる
	pCEID = m_ceid.get_byID( ceid );
	if( pCEID == NULL ) {
		return NULL;
	}
	for(i = 0; i < pCEID->get_RptsCount(); i++) {
		XSEMI_Rpt* pRpt = pCEID->get_Rpt(i);

		if( pRpt->get_RptID() == rptid ) {
			return NULL;
		}
		if( preRptID != 0xFFFFFFFF && pRpt->get_RptID() == preRptID ) {
			pPreRpt = pRpt;
		}
	}
	if( preRptID != 0xFFFFFFFF && pPreRpt == NULL ) {
		return NULL;
	}




	// 新規アイテムを追加
	XSEMI_Rpt* pNewRpt = NULL;

	for( ; iteRpt != m_rpts.m_rpts.end(); iteRpt++) {
		// 同じCEIDがある
		if( (*iteRpt)->get_RptID() == rptid ) {
			pNewRpt = (*iteRpt);
			break;
		}
	}
	if( pNewRpt == NULL ) {
		pNewRpt = new XSEMI_Rpt(rptid, _T(""));
		m_rpts.m_rpts.push_back(pNewRpt);
	}

	std::vector<XSEMI_Rpt*>::iterator iteCEIDonRpt = pCEID->m_rpts.m_rpts.begin();
	pCEID->m_rpts.m_rpts.push_back(pNewRpt);
	m_ceid.m_bDirtyMember = true;	// Dirty にセット
	m_rpts.m_bDirtyMember = true;	// Dirty にセット


	return pNewRpt;
XEXCEPT_EXIT
}

bool XSEMI_GemConst::deleteAllCEID_RptID()
{
XEXCEPT_ENTRY
	
	// 先ず、CEIDに緋も付くReptIDを削除する。
	{
		XSEMI_CEID* pCEID = m_ceid.getFirstItem();
		while( pCEID != NULL) {
			pCEID->m_rpts.clear();
			pCEID = m_ceid.getNextItem();
		}
	}

	// マスターの更新
	if( m_pMasterData != NULL ) {
		// レポートIDを削除
		m_pMasterData->deleteCEID_RptID(0xFFFFFFFF, 0xFFFFFFFF);
	}
	m_ceid.m_bDirtyMember = true;	// Dirty にセット
	m_rpts.m_bDirtyMember = true;	// Dirty にセット
	return true;
XEXCEPT_EXIT
}

void XSEMI_GemConst::loadFromMaster(XSEMI_GemConstObject* pData)
{
XEXCEPT_ENTRY
	if( m_pMasterData == NULL ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("master object not set."), 0, 0);
	}


XEXCEPT_EXIT
}

void XSEMI_GemConst::updateMaster(XSEMI_GemConstObject* pData)
{
XEXCEPT_ENTRY
	if( m_pMasterData == NULL ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("master object not set."), 0, 0);
	}
	if( pData == NULL ) {
		if( m_ceid.isDirtyMember() == true ) {
			m_pMasterData->updateObject( &m_ceid );
		}
		if( m_rpts.isDirtyMember() == true ) {
			m_pMasterData->updateObject( &m_rpts );
		}
		if( m_variable_grps.isDirtyMember() == true ) {
			m_pMasterData->updateObject( &m_variable_grps );
		}
		if( m_alarms.isDirtyMember() == true ) {
			m_pMasterData->updateObject( &m_alarms );
		}
		if( m_ackcodes.isDirtyMember() == true ) {
			m_pMasterData->updateObject( &m_ackcodes );
		}
		if( m_spoolSFs.isDirtyMember() == true ) {
			m_pMasterData->updateObject( &m_spoolSFs );
		}
	} else {
		m_pMasterData->updateObject( pData );
	}
XEXCEPT_EXIT
}

void XSEMI_GemConst::deleteMaster(XSEMI_GemConstObject* pData)
{
XEXCEPT_ENTRY
	if( m_pMasterData == NULL ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("master object not set."), 0, 0);
	}
	if( pData->get_ObjectType() == XSEMI_GemConstObject::CEID ) {
		// マスターから削除
		m_pMasterData->deleteObject(pData);
		// 内部配列から削除
		this->get_CEIDs().del( (XSEMI_CEID*)pData );
	} else
	if( pData->get_ObjectType() == XSEMI_GemConstObject::RPTID ) {
		// マスターから削除
		m_pMasterData->deleteObject(pData);
		// 内部配列から削除
		this->get_Rpts().del( (XSEMI_Rpt*)pData );
	} else
	if( pData->get_ObjectType() == XSEMI_GemConstObject::VID ) {
		// マスターから削除
		m_pMasterData->deleteObject(pData);
		// 内部配列から削除
		this->get_Vs().del( (XSEMI_V*)pData );		
	} else
	if( pData->get_ObjectType() == XSEMI_GemConstObject::ALARM ) {
		// マスターから削除
		m_pMasterData->deleteObject(pData);
		// 内部配列から削除
		this->get_Alarms().del( (XSEMI_ALARM*)pData );		
	} else
	if( pData->get_ObjectType() == XSEMI_GemConstObject::VARIABLE_GRP ) {
		// マスターから削除
		m_pMasterData->deleteObject(pData);
		// 内部配列から削除
		this->get_VARIABLE_GRPs().del( (XSEMI_VARIABLE_GRP*)pData );	
	} else
	if( pData->get_ObjectType() == XSEMI_GemConstObject::ACKCODE ) {
		// マスターから削除
		m_pMasterData->deleteObject(pData);
		// 内部配列から削除
		this->get_AckCodes().del( (XSEMI_ACKCODE*)pData );	
	} else {
		_XASSERT( 0 );
	}
XEXCEPT_EXIT
}


bool XSEMI_GemConst::setRiDataFromString(XRichData* pRdata, const TCHAR* p_str)
{
XEXCEPT_ENTRY
	try {
		XSecsTextParser::parseFromString(p_str, XStrBuff::StrLen(p_str), pRdata);
	} catch ( XExcept &e ) {
		// eが未使用になるため、コンパイルのwaningを出力しないための処理
		unsigned long err_code = e.getOsErrorCode();
	}
	return true;
XEXCEPT_EXIT
}

//
//	XSEMI_GemMstData
//
XSEMI_GemMstData::XSEMI_GemMstData()
	:	m_pConst( NULL )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}


XSEMI_GemMstData::~XSEMI_GemMstData()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

void XSEMI_GemMstData::setDefaultValues()
{
XEXCEPT_ENTRY
	int i = 0;
	int			index = 0;
	XLIB_STD_STREAM_BUFFER		buff;
	XLIB_STD_OUT_STR_STREAM		stream( &buff );



	//
	//	VARIABLE_GRPの追加
	//	
	struct tagVARIABLE_GRP{
		int id;	
		TCHAR	*szName;
		TCHAR	*szComment;
	} datasVGrps[] = {
			{0, _T("Non"), _T("Non")}
		,	{1, _T("Alarms"), _T("Alarms")}
		,	{2, _T("Commands"), _T("Commands")}
		,	{3, _T("Communication"), _T("Communication")}
		,	{4, _T("Control State"), _T("Control State")}
		,	{5, _T("Equipment Constants"), _T("Equipment Constants")}
		,	{6, _T("Events"), _T("Events")}
		,	{7, _T("Limits Monitoring"), _T("Limits Monitoring")}
		,	{8, _T("Process Programs"), _T("Process Programs")}
		,	{9, _T("Text Exchange"), _T("Text Exchange")}
		,	{99, _T("Other Variables"), _T("Other Variables")}
	};

	struct tagACKCODEs{
		XD_UINT4 kind;
		XD_UINT1 code;
		TCHAR	*szComment;
	} datasAckCodes[] = {
			{XSEMI_ACKCODE::KIND_ACKA,		0, _T("failure")}
		,	{XSEMI_ACKCODE::KIND_ACKA,		1, _T("successful")}
		
		,	{XSEMI_ACKCODE::KIND_ACKC3,		0, _T("accepted")}
		,	{XSEMI_ACKCODE::KIND_ACKC3,		(XD_UINT1)-1, _T("error. not accepted")}
		
		,	{XSEMI_ACKCODE::KIND_ACKC5,		0, _T("accepted")}
		,	{XSEMI_ACKCODE::KIND_ACKC5,		(XD_UINT1)-1, _T("error. not accepted")}

		,	{XSEMI_ACKCODE::KIND_ACKC6,		0, _T("accepted")}
		,	{XSEMI_ACKCODE::KIND_ACKC6,		(XD_UINT1)-1, _T("error. not accepted")}
		
		,	{XSEMI_ACKCODE::KIND_ACKC7,		0,_T("許可された")}
		,	{XSEMI_ACKCODE::KIND_ACKC7,		1,_T("不許可")}
		,	{XSEMI_ACKCODE::KIND_ACKC7,		2,_T("レングス・エラー")}
		,	{XSEMI_ACKCODE::KIND_ACKC7,		3,_T("配列オーバーフロー")}
		,	{XSEMI_ACKCODE::KIND_ACKC7,		4,_T("PPID未定義")}
		,	{XSEMI_ACKCODE::KIND_ACKC7,		5,_T("モード・エラー")}
		,	{XSEMI_ACKCODE::KIND_ACKC7,		64,_T("データ・エラー")}
		,	{XSEMI_ACKCODE::KIND_ACKC7,		65,_T("無効PPID")}
		,	{XSEMI_ACKCODE::KIND_ACKC7,		255,_T("その他のエラー")}

		,	{XSEMI_ACKCODE::KIND_ACKC7A,	0, _T("了解された")}
		,	{XSEMI_ACKCODE::KIND_ACKC7A,	1, _T("MDLNが一致しない")}
		,	{XSEMI_ACKCODE::KIND_ACKC7A,	2, _T("SOFTREVが一致しない")}
		,	{XSEMI_ACKCODE::KIND_ACKC7A,	3, _T("無効なCCODE")}
		,	{XSEMI_ACKCODE::KIND_ACKC7A,	4, _T("無効なPPARMの値")}
		,	{XSEMI_ACKCODE::KIND_ACKC7A,	5, _T("他のエラー（ERRW7によって示される）")}

		,	{XSEMI_ACKCODE::KIND_ACKC10,	0, _T("表示了解")}
		,	{XSEMI_ACKCODE::KIND_ACKC10,	1, _T("メッセージは表示されない")}
		,	{XSEMI_ACKCODE::KIND_ACKC10,	2, _T("端末使用できない")}


		,	{XSEMI_ACKCODE::KIND_ACKC13,	0, _T("O.K")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	1, _T("ERROR: Try Later")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	2, _T("ERROR: Unknown Data Set name")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	3, _T("ERROR: Illegal Checkpoint value")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	4, _T("ERROR: Too many open Data Sets")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	5, _T("ERROR: Data set open too many times")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	6, _T("ERROR: No open Data Set")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	7, _T("ERROR: Cannot continue")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	8, _T("ERROR: End of Data")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	9, _T("ERROR: Handle in Use")}
		,	{XSEMI_ACKCODE::KIND_ACKC13,	(XD_UINT1)-1, _T("ERROR: Pending Transaction")}

		,	{XSEMI_ACKCODE::KIND_CAACK,		0, _T("確認、コマンドは実行された")}
		,	{XSEMI_ACKCODE::KIND_CAACK,		1, _T("無効なコマンド")}
		,	{XSEMI_ACKCODE::KIND_CAACK,		2, _T("現在実行できない")}
		,	{XSEMI_ACKCODE::KIND_CAACK,		3, _T("無効なデータもしくは、無効な引数が指定された")}
		,	{XSEMI_ACKCODE::KIND_CAACK,		4, _T("確認、コマンドは実行され、イベントにより完了が知らされる")}
		,	{XSEMI_ACKCODE::KIND_CAACK,		5, _T("拒否、要求できる状態にない")}
		,	{XSEMI_ACKCODE::KIND_CAACK,		6, _T("コマンドはエラーを伴い実行された")}
		,	{XSEMI_ACKCODE::KIND_CAACK,		64, _T("転送許可がない")}
		,	{XSEMI_ACKCODE::KIND_CAACK,		128, _T("その他のエラー")}

		,	{XSEMI_ACKCODE::KIND_CEPACK,	0, _T("No error")}
		,	{XSEMI_ACKCODE::KIND_CEPACK,	1, _T("Parameter name (CPNAME) dose not exist")}
		,	{XSEMI_ACKCODE::KIND_CEPACK,	2, _T("Illegal value specified for CEPVAL")}
		,	{XSEMI_ACKCODE::KIND_CEPACK,	3, _T("Illegal format specified for CEPVAL")}
		,	{XSEMI_ACKCODE::KIND_CEPACK,	4, _T("Parameter name (CPNAME) not valid as used")}

		,	{XSEMI_ACKCODE::KIND_CPACK,		1, _T("パラメータ名（CPNAME）は存在しない")}
		,	{XSEMI_ACKCODE::KIND_CPACK,		2, _T("CPVAL用として指定された違法な値")}
		,	{XSEMI_ACKCODE::KIND_CPACK,		3, _T("CPVAL用として指定された違法なフォーマット")}
		,	{XSEMI_ACKCODE::KIND_CPACK,		64, _T("省略不可のパラメータを指定されなかった")}
		,	{XSEMI_ACKCODE::KIND_CPACK,		65, _T("指定されたパラメータは使用できる環境にない")}
		,	{XSEMI_ACKCODE::KIND_CPACK,		66, _T("プロセス･プログラムに範囲外の値が設定されている")}


		,	{XSEMI_ACKCODE::KIND_CSAACK,	0, _T("Everything correct")}
		,	{XSEMI_ACKCODE::KIND_CSAACK,	1, _T("Busy")}
		,	{XSEMI_ACKCODE::KIND_CSAACK,	2, _T("Invalid SPID")}
		,	{XSEMI_ACKCODE::KIND_CSAACK,	3, _T("Invalid data")}

		,	{XSEMI_ACKCODE::KIND_DATAACK,	0, _T("了解")}
		,	{XSEMI_ACKCODE::KIND_DATAACK,	1, _T("DATAIDが不明")}
		,	{XSEMI_ACKCODE::KIND_DATAACK,	2, _T("少なくともパラメータが無効")}


		,	{XSEMI_ACKCODE::KIND_DRACK,		0, _T("了解")}
		,	{XSEMI_ACKCODE::KIND_DRACK,		1, _T("否定、スペース不十分")}
		,	{XSEMI_ACKCODE::KIND_DRACK,		2, _T("否定、無効フォーマット")}
		,	{XSEMI_ACKCODE::KIND_DRACK,		3, _T("否定、少なくとも一つのRPTIDはすでに定義されている")}
		,	{XSEMI_ACKCODE::KIND_DRACK,		4, _T("否定、少なくとも一つのVIDが存在しない")}
		,	{XSEMI_ACKCODE::KIND_DRACK,		65, _T("否定、要求できる状態にない")}
		,	{XSEMI_ACKCODE::KIND_DRACK,		66, _T("存在しないRPTIDが指定された")}


		,	{XSEMI_ACKCODE::KIND_EAC,		0, _T("了解")}
		,	{XSEMI_ACKCODE::KIND_EAC,		1, _T("否定、少なくとも一つの定数が存在しない")}
		,	{XSEMI_ACKCODE::KIND_EAC,		2, _T("否定、ビジー")}
		,	{XSEMI_ACKCODE::KIND_EAC,		3, _T("否定、少なくとも一つの定数が範囲外にある")}
		,	{XSEMI_ACKCODE::KIND_EAC,		64, _T("否定、少なくとも一つの定数のフォーマットが異なる")}
		,	{XSEMI_ACKCODE::KIND_EAC,		100, _T("正常終了、Reset/Calibが必要")}
		,	{XSEMI_ACKCODE::KIND_EAC,		255, _T("その他のエラー")}

		,	{XSEMI_ACKCODE::KIND_ERACK,		0, _T("了解")}
		,	{XSEMI_ACKCODE::KIND_ERACK,		1, _T("否定、少なくとも一つのCEIDが存在しない")}
		,	{XSEMI_ACKCODE::KIND_ERACK,		65, _T("否定、要求できる状態にない")}


		,	{XSEMI_ACKCODE::KIND_ERRCODE,	0, _T("No Error")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	1, _T("Unknown object in Object Specifier")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	2, _T("Unknown target object type")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	3, _T("Unknown object instance")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	4, _T("Unknown attribute name")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	5, _T("Read-only attribute - access denied")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	6, _T("Unknown object type")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	7, _T("Invalid attribute value")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	8, _T("Syntax error")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	9, _T("Verification error")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	10, _T("Validation error")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	11, _T("Object identifier in use")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	12, _T("Parameters improperly specified")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	13, _T("Insufficient parameters specified")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	14, _T("Unsupported option requested")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	15, _T("Busy")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	16, _T("Not available for processing")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	17, _T("Command not valid for current state")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	18, _T("No material altered")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	19, _T("Material partially processed")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	20, _T("All material processed")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	21, _T("Recipe specification related error")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	22, _T("Failed during processing")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	23, _T("Failed while not processing")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	24, _T("Failed due to lack of material")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	25, _T("Job aborted")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	26, _T("Job stopped")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	27, _T("Job cancelled")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	28, _T("Cannot change selected recipe")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	29, _T("Unknown event")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	30, _T("Duplicate report ID")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	31, _T("Unknown data report")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	32, _T("Data report not linked")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	33, _T("Unknown trace report")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	34, _T("Duplicate trace ID")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	35, _T("Too many data reports")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	36, _T("Sample period out of range")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	37, _T("Group size to large")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	38, _T("Recovery action currently invalid")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	39, _T("Busy with another recovery currently unable to perform the recovery")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	40, _T("No active recovery action")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	41, _T("Exception recovery failed")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	42, _T("Exception recovery aborted")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	43, _T("Invalid table element")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	44, _T("Unknown table element")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	45, _T("Cannot delete predefined")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	46, _T("Invalid token")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	47, _T("Invalid parameter")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	48, _T("Load port does not exist")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	49, _T("Load port already in use")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	50, _T("Missing Carrier")}
		,	{XSEMI_ACKCODE::KIND_ERRCODE,	255, _T("Other error")}

		,	{XSEMI_ACKCODE::KIND_GRANT,		0, _T("Permission Granted")}
		,	{XSEMI_ACKCODE::KIND_GRANT,		1, _T("Busy, Try Again")}
		,	{XSEMI_ACKCODE::KIND_GRANT,		2, _T("No Space Available")}
		,	{XSEMI_ACKCODE::KIND_GRANT,		3, _T("Duplicate DATAID")}

		,	{XSEMI_ACKCODE::KIND_GRANT6,	0, _T("Permission Granted")}
		,	{XSEMI_ACKCODE::KIND_GRANT6,	1, _T("Busy, Try Again")}
		,	{XSEMI_ACKCODE::KIND_GRANT6,	2, _T("Not interested")}

		,	{XSEMI_ACKCODE::KIND_GRANT1,	0, _T("Positive response, transfer ok")}
		,	{XSEMI_ACKCODE::KIND_GRANT1,	1, _T("Busy, try again")}
		,	{XSEMI_ACKCODE::KIND_GRANT1,	2, _T("No Space")}
		,	{XSEMI_ACKCODE::KIND_GRANT1,	3, _T("Map too large")}
		,	{XSEMI_ACKCODE::KIND_GRANT1,	4, _T("Duplicate ID")}
		,	{XSEMI_ACKCODE::KIND_GRANT1,	5, _T("Material ID no found")}
		,	{XSEMI_ACKCODE::KIND_GRANT1,	6, _T("unknown map format")}

		,	{XSEMI_ACKCODE::KIND_HCACK,		0, _T("Acknowledge, command has benn performed")}
		,	{XSEMI_ACKCODE::KIND_HCACK,		1, _T("Command dose not exist")}
		,	{XSEMI_ACKCODE::KIND_HCACK,		2, _T("Cannot perform now")}
		,	{XSEMI_ACKCODE::KIND_HCACK,		3, _T("At leasr one parameter is invalid")}
		,	{XSEMI_ACKCODE::KIND_HCACK,		4, _T("Acknowledge, command will br performed with completion signaled later by an event")}
		,	{XSEMI_ACKCODE::KIND_HCACK,		5, _T("Reject, Already in Desired Condition")}
		,	{XSEMI_ACKCODE::KIND_HCACK,		6, _T("No such object exists")}



	};




	XEXCEPT_ENTRY
	for(i = 0; i < sizeof(datasVGrps)/sizeof(tagVARIABLE_GRP); i++) {
		XSEMI_VARIABLE_GRP vg( datasVGrps[i].id, datasVGrps[i].szName );
		vg.put_Comment( datasVGrps[i].szComment );

		buff.resetp();
		vg.makeInsertString(stream);
		execute( buff.toString() );
	}
	for(i = 0; i < sizeof(datasAckCodes)/sizeof(tagACKCODEs); i++) {
		XSEMI_ACKCODE  vg( datasAckCodes[i].kind, datasAckCodes[i].code );
		vg.set_Comment( datasAckCodes[i].szComment );

		buff.resetp();
		vg.makeInsertString(stream);
		execute( buff.toString() );
	}

	XEXCEPT_EXIT

	//
	//	Vの追加
	//	
	XSEMI_V* pV;
	const TCHAR* pName;
	index = 1;

	XEXCEPT_ENTRY
		// GemMDLN
		pName = XSEMI_GEM_CONF_MDLN;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_A, XSEMI_V::SV);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValueString(XSEMI_GEM_CONF_DEF_MDLN);
			pV->put_Comment(pName);
			pV->put_Enable( true );
		}
	XEXCEPT_EXIT
	XEXCEPT_ENTRY
		// GemSOFTREV
		pName = XSEMI_GEM_CONF_SOFTREV;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_A, XSEMI_V::SV);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValueString(XSEMI_GEM_CONF_DEF_SOFTREV);
			pV->put_Comment(pName);
			pV->put_Enable( true );
		}
	XEXCEPT_EXIT


	XEXCEPT_ENTRY
		// GemConfigConnect
		pName = XSEMI_GEM_CONF_CONF_CONNECT;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONF_CONNECT );
			pV->put_Comment(pName);
			pV->put_Enable( true );

			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONF_CONNECT_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONF_CONNECT_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONF_CONNECT );
			((XSEMI_ECV*)pV)->            putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONF_CONNECT );

			this->updateObject( pV );
		}
	XEXCEPT_EXIT
	XEXCEPT_ENTRY
		// GemEstabCommDelay
		pName = XSEMI_GEM_CONF_ESTAB_COMM_DELAY;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY );
			pV->put_Comment(pName);
			pV->put_Enable( true );

			pV->put_Unit(_T("sec"));
			pV->put_VariableGrpID( 3 );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY );
			((XSEMI_ECV*)pV)->            putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ESTAB_COMM_DELAY );

			this->updateObject( pV );
		}
	XEXCEPT_EXIT
	XEXCEPT_ENTRY
		// GemInitCommState
		pName = XSEMI_GEM_CONF_INIT_COMM_STATE;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_COMM_STATE );
			pV->put_Comment(pName);
			pV->put_Enable( true );


			pV->put_VariableGrpID( 3 );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_COMM_STATE_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_COMM_STATE_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_COMM_STATE );
			((XSEMI_ECV*)pV)->            putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_COMM_STATE );

			this->updateObject( pV );
		}
	XEXCEPT_EXIT
	XEXCEPT_ENTRY
		// GemInitControlState
		pName = XSEMI_GEM_CONF_INIT_CTRL_STATE;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_CTRL_STATE );
			pV->put_Comment(pName);
			pV->put_Enable( true );


			pV->put_VariableGrpID( 4 );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_CTRL_STATE_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_CTRL_STATE_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_CTRL_STATE );
			((XSEMI_ECV*)pV)->            putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_INIT_CTRL_STATE );

			this->updateObject( pV );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemOfflineSubstate
		pName = XSEMI_GEM_CONF_OFFLINE_SUB_STATE;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_OFFLINE_SUB_STATE );
			pV->put_Comment(pName);
			pV->put_Enable( true );


			pV->put_VariableGrpID( 4 );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_OFFLINE_SUB_STATE_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_OFFLINE_SUB_STATE_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_OFFLINE_SUB_STATE );
			((XSEMI_ECV*)pV)->            putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_OFFLINE_SUB_STATE );

			this->updateObject( pV );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemOnlineSubstate
		pName = XSEMI_GEM_CONF_ONLINE_SUB_STATE;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_SUB_STATE );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 4 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_SUB_STATE_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_SUB_STATE_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_SUB_STATE );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_SUB_STATE );
		}
	XEXCEPT_EXIT
	XEXCEPT_ENTRY
		// GemOnlineFailed
		pName = XSEMI_GEM_CONF_ONLINE_FAILED;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_FAILED );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 4 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_FAILED_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_FAILED_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_FAILED );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_ONLINE_FAILED );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemPollDelay
		pName = XSEMI_GEM_CONF_POOL_DELAY;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT2)XSEMI_GEM_CONF_DEF_POOL_DELAY );
			pV->put_Comment(pName);
			pV->put_Unit(_T("s"));
			pV->put_VariableGrpID( 4 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT2)XSEMI_GEM_CONF_DEF_POOL_DELAY_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT2)XSEMI_GEM_CONF_DEF_POOL_DELAY_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT2)XSEMI_GEM_CONF_DEF_POOL_DELAY );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT2)XSEMI_GEM_CONF_DEF_POOL_DELAY );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemConfigEvents
		pName = XSEMI_GEM_CONF_CONFIG_EVENT;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_EVENT );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 6 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_EVENT_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_EVENT_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_EVENT );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_EVENT );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemRpType
		pName = XSEMI_GEM_CONF_RP_TYPE;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_BOOL, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (bool)XSEMI_GEM_CONF_DEF_RP_TYPE );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 6 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (bool)XSEMI_GEM_CONF_DEF_RP_TYPE );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (bool)XSEMI_GEM_CONF_DEF_RP_TYPE );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemWBitS6
		pName = XSEMI_GEM_CONF_WBIT_6;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_WBIT_6 );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 6 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_WBIT_6_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_WBIT_6_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_WBIT_6 );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_WBIT_6 );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemConfigSpool
		pName = XSEMI_GEM_CONF_CONFIG_SPOOL;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_SPOOL );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_SPOOL_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_SPOOL_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_SPOOL );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_CONFIG_SPOOL );
		}
	XEXCEPT_EXIT
	XEXCEPT_ENTRY
		// GemMaxSpoolSize
		pName = XSEMI_GEM_CONF_MAX_SPOOL_SIZE;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U4, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_SIZE );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemMaxSpoolTransmit
		pName = XSEMI_GEM_CONF_MAX_SPOOL_TRANSMIT;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U4, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcMax().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT_MAX );
			((XSEMI_ECV*)pV)->get_EcMin().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT_MIN );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (XD_UINT4)XSEMI_GEM_CONF_DEF_MAX_SPOOL_TRANSMIT );
		}
	XEXCEPT_EXIT

	
	XEXCEPT_ENTRY
		// GemSpoolCountActual
		pName = XSEMI_GEM_CONF_SPPOL_CNT_ACTL;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U4, XSEMI_V::SV);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT4)0 );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_SV*)pV)->put_SizeMax(1);
			((XSEMI_SV*)pV)->put_SizeMin(1);
		}
	XEXCEPT_EXIT


	XEXCEPT_ENTRY
		// GemSpoolCountTotal
		pName = XSEMI_GEM_CONF_SPOOL_CNT_TOTAL;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U4, XSEMI_V::SV);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT4)0 );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_SV*)pV)->put_SizeMax(1);
			((XSEMI_SV*)pV)->put_SizeMin(1);
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemOverWriteSpool
		pName = XSEMI_GEM_CONF_OVR_WRITE_SPOOL;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_BOOL, XSEMI_V::EC);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (bool)XSEMI_GEM_CONF_DEF_OVR_WRITE_SPOOL );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (bool)XSEMI_GEM_CONF_DEF_OVR_WRITE_SPOOL );
			((XSEMI_ECV*)pV)->get_EcDef().putValue( (bool)XSEMI_GEM_CONF_DEF_OVR_WRITE_SPOOL );
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemSpoolFullTime
		pName = XSEMI_GEM_CONF_SPOOL_FULL_TIME;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_A, XSEMI_V::SV);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_SV*)pV)->put_SizeMax(0);
			((XSEMI_SV*)pV)->put_SizeMin(16);
		}
	XEXCEPT_EXIT
	XEXCEPT_ENTRY
		// GemSpoolLoadSubstate
		pName = XSEMI_GEM_CONF_SPOOL_LOAD_SUBST;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::SV);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_LOAD_SUBST );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_SV*)pV)->get_limitMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_LOAD_SUBST_MAX );
			((XSEMI_SV*)pV)->get_limitMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_LOAD_SUBST_MIN );
			((XSEMI_SV*)pV)->put_SizeMax(1);
			((XSEMI_SV*)pV)->put_SizeMin(1);
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemSpoolUnloadSubstate
		pName = XSEMI_GEM_CONF_SPOOL_UNLOAD_SUBST;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::SV);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_UNLOAD_SUBST );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_SV*)pV)->get_limitMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_UNLOAD_SUBST_MAX );
			((XSEMI_SV*)pV)->get_limitMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_UNLOAD_SUBST_MIN );
			((XSEMI_SV*)pV)->put_SizeMax(1);
			((XSEMI_SV*)pV)->put_SizeMin(1);
		}
	XEXCEPT_EXIT

	XEXCEPT_ENTRY
		// GemSpoolState
		pName = XSEMI_GEM_CONF_SPOOL_STAT;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_U1, XSEMI_V::SV);
		if( pV != NULL ) {
			XSEMI_SV tmpV(index++, pName, XDATA_TYPE_U1);
			pV->put_DisplayName( pName );
			pV->get_InitialValue().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_STAT );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_SV*)pV)->get_limitMax().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_STAT_MAX );
			((XSEMI_SV*)pV)->get_limitMin().putValue( (XD_UINT1)XSEMI_GEM_CONF_DEF_SPOOL_STAT_MIN );
			((XSEMI_SV*)pV)->put_SizeMax(1);
			((XSEMI_SV*)pV)->put_SizeMin(1);
		}
	XEXCEPT_EXIT
	XEXCEPT_ENTRY
		// GemSpoolStartTime
		pName = XSEMI_GEM_CONF_SPOOL_START_TIME;
		pV = m_pConst->get_Vs().add(index++, pName, XDATA_TYPE_A, XSEMI_V::SV);
		if( pV != NULL ) {
			pV->put_DisplayName( pName );
			pV->put_Comment(pName);
			pV->put_VariableGrpID( 9 );
			pV->put_Enable( true );
			((XSEMI_SV*)pV)->put_SizeMax(0);
			((XSEMI_SV*)pV)->put_SizeMin(16);
		}
	XEXCEPT_EXIT
	this->updateObject( &(m_pConst->get_Vs()) );
XEXCEPT_EXIT
}











