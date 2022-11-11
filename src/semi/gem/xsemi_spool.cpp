#include "xlib/xeception.h"
#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/semi/gem/xsemi_spool.h"



//
//	XSEMI_SpoolSF
//
XSEMI_CONST_SCHEMA	XSEMI_SpoolSF::g_pSchema[] = {
		{XSEMI_DATA_CLMN_STREAM,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT1, true}
	,	{XSEMI_DATA_CLMN_FUNCTION,		XDATA_TYPE_U4,		XSEMI_CONST_SCHEMA_RTYPE_UINT2, true}
	,	{_T(""),						XDATA_TYPE_END,		XSEMI_CONST_SCHEMA_RTYPE_NULL, false}
};
XSEMI_SpoolSF::XSEMI_SpoolSF(XD_UINT1 stream, XD_UINT2 function)
	:	XSEMI_GemConstObjectAtom(XSEMI_GemConstObject::SPOOL_SF)
	,	m_stream(stream)
	,	m_function(function)
{
XEXCEPT_ENTRY
	m_pBaseSchema	= g_pSchema;
XEXCEPT_EXIT
}

XSEMI_SpoolSF::~XSEMI_SpoolSF()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XD_UINT1 XSEMI_SpoolSF::get_Stream() const
{
XEXCEPT_ENTRY
	return m_stream;
XEXCEPT_EXIT
}
XD_UINT2 XSEMI_SpoolSF::get_Function() const
{
XEXCEPT_ENTRY
	return m_function;
XEXCEPT_EXIT
}


// overwrite	XSEMI_GemDataAtom
const TCHAR* XSEMI_SpoolSF::getAtomName()
{
XEXCEPT_ENTRY
	return _T("SPOOL_SF");
XEXCEPT_EXIT
}
int XSEMI_SpoolSF::getColmnCount()
{
XEXCEPT_ENTRY
	return ATOM_POS_FUNCTION+1;
XEXCEPT_EXIT
}
void* XSEMI_SpoolSF::getAtomValPtr(int index)
{
XEXCEPT_ENTRY
	if( index >= getColmnCount() ) {
		XEXCEPT_THROW_STR(XExcept_XDATA, _T("index overfllow."), 0, index);
	}

	switch( index ) {
	case ATOM_POS_STREAM:
		return &m_stream;
		break;
	case ATOM_POS_FUNCTION:
		return &m_function;
		break;
	}
	return NULL;
XEXCEPT_EXIT
}


//
//	XSEMI_SpoolSFs
//
XSEMI_SpoolSFs::XSEMI_SpoolSFs()
	:	XSEMI_GemConstObject(XSEMI_GemConstObject::SPOOL_SFs)
{
XEXCEPT_ENTRY
	m_ite = m_datas.begin();
XEXCEPT_EXIT
}
XSEMI_SpoolSFs::~XSEMI_SpoolSFs()
{
XEXCEPT_ENTRY
	clear();
XEXCEPT_EXIT
}
void XSEMI_SpoolSFs::clear()
{
XEXCEPT_ENTRY
	std::vector<XSEMI_SpoolSF*>::iterator ite = m_datas.begin();	
	for( ; ite != m_datas.end(); ite++) {
		delete (*ite);
	}
	m_datas.clear();
XEXCEPT_EXIT
}

XD_UINT4 XSEMI_SpoolSFs::count()
{
XEXCEPT_ENTRY
	return m_datas.size();
XEXCEPT_EXIT
}

XSEMI_SpoolSF* XSEMI_SpoolSFs::get(XD_UINT4 index)
{
XEXCEPT_ENTRY
	if( index >= m_datas.size()  ) {
		return NULL;
	}
	return m_datas[index];
XEXCEPT_EXIT	
}

XSEMI_SpoolSF* XSEMI_SpoolSFs::get_byID(XD_UINT1 stream, XD_UINT1 function)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_SpoolSF*>::iterator ite = m_datas.begin();	
	for( ; ite != m_datas.end(); ite++) {
		if( (*ite)->get_Stream() == stream &&
			(*ite)->get_Function() == function) {
			return (*ite);
		}
	}
	return NULL;
XEXCEPT_EXIT	
}
XSEMI_SpoolSF* XSEMI_SpoolSFs::getFirstItem()
{
XEXCEPT_ENTRY
	m_ite = m_datas.begin();
	if( m_ite == m_datas.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT	
}
XSEMI_SpoolSF* XSEMI_SpoolSFs::getNextItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_datas.end() ) {
		return NULL;
	}
	m_ite++;
	if( m_ite == m_datas.end() ) {
		return NULL;
	}
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_SpoolSF* XSEMI_SpoolSFs::getPrevItem()
{
XEXCEPT_ENTRY
	if( m_ite == m_datas.begin() ) {
		return NULL;
	}
	m_ite--;
	return *m_ite;
XEXCEPT_EXIT
}
XSEMI_SpoolSF* XSEMI_SpoolSFs::add(XSEMI_GemDataAtom* pAtom)
{
XEXCEPT_ENTRY
	XSEMI_SpoolSF* pNewItem = NULL;
	XD_UINT1 stream       = (XD_UINT1)pAtom->getUint(XSEMI_DATA_CLMN_STREAM);
	XD_UINT1 function     = (XD_UINT1)pAtom->getUint(XSEMI_DATA_CLMN_FUNCTION);


	pNewItem = get_byID( stream, function  );
	if( pNewItem != NULL ) {
		// 既に同じCEIDがある
		return NULL;
	}
	pNewItem = new XSEMI_SpoolSF( stream, function );
	if( pNewItem == NULL ) {
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);
		return NULL;
	}
//	pNewItem->m_pParentObject = this;
	m_datas.push_back( pNewItem );

	return pNewItem;
XEXCEPT_EXIT	
}

bool XSEMI_SpoolSFs::del(XSEMI_SpoolSF* pSF)
{
XEXCEPT_ENTRY
	std::vector<XSEMI_SpoolSF*>::iterator ite = m_datas.begin();	
	for( ; ite != m_datas.end(); ite++) {
		XSEMI_SpoolSF* pDef = ( *ite );
		if( pDef == pSF ) {
			delete pDef;
			m_datas.erase( ite );
			return true;
		}
	}
	return false;
XEXCEPT_EXIT
}




//
//	XSEMI_Spool
//
XSEMI_Spool::XSEMI_Spool(XSEMI_SpoolSFs& sfs)
	:	m_sfs(sfs)

	,	m_ulActualCnt( 0 )
	,	m_ulTotalCnt( 0 )
	,	m_bSpoolFull( false )
	,	m_ucSpoolSts( XSEMI_GEM_SPOOL_INACTIVE )
	,	m_ucLoadSubSts( XSEMI_GEM_SPOOL_NOTFULL )
	,	m_ucUnLoadSubSts( XSEMI_GEM_SPOOL_NOOUTPUT )

	,	m_ulMaxSpoolSize( 0 )
	,	m_ulMaxSpoolTransmit( 0 )

	,	m_bOverWrite(false)
	,	m_bInitOK( false )

	,	m_pGem( NULL )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

XSEMI_Spool::~XSEMI_Spool()
{
XEXCEPT_ENTRY
	close();
XEXCEPT_EXIT
}

void XSEMI_Spool::setGem(XSEMI_Gem* pGem)
{
XEXCEPT_ENTRY
	m_pGem = pGem;
XEXCEPT_EXIT
}


XD_UINT4 XSEMI_Spool::getActualCnt() const
{
XEXCEPT_ENTRY
	return m_ulActualCnt;
XEXCEPT_EXIT	
}

XD_UINT4 XSEMI_Spool::getTotalCnt() const
{
XEXCEPT_ENTRY
	return m_ulTotalCnt;
XEXCEPT_EXIT	
}

XD_UINT1 XSEMI_Spool::getSpoolState()
{
XEXCEPT_ENTRY
	return m_ucSpoolSts;
XEXCEPT_EXIT	
}

XD_UINT1 XSEMI_Spool::getSpoolLoadState()
{
XEXCEPT_ENTRY
	return m_ucLoadSubSts;
XEXCEPT_EXIT	
}

XD_UINT1 XSEMI_Spool::getSpoolUnLoadState()
{
XEXCEPT_ENTRY
	return m_ucUnLoadSubSts;
XEXCEPT_EXIT	
}

bool XSEMI_Spool::open(const TCHAR* p_map_file, 
					   XD_UINT4 ulFileSize, 
					   XD_UINT4 ulMaxSpoolTransmit, 
					   bool bOverWrite)
{
XEXCEPT_ENTRY
	XFileStatus fSts;
	m_bInitOK = false;




	m_bOverWrite         = bOverWrite;
	m_ulMaxSpoolSize     = ulFileSize;
	m_ulMaxSpoolTransmit = ulMaxSpoolTransmit;
	m_strFileName		 = p_map_file;

	if( this->alloc( ulFileSize ) == false ) {
		return false;
	}

	// Spoolファイルが有る？
	if( XFile::GetStatus(p_map_file, fSts) == true ) {
		XFile file;
		if( file.Open(p_map_file, true, false, false) == true ) {
			XD_UINT4    i;
			XBuff		buffTmp;
			double		dTime;
			char*		pBuffer;
			char*		pWork;


			file.Read(&m_ulActualCnt, sizeof(m_ulActualCnt));
			// SpoolTotalカウント
			file.Read(&m_ulTotalCnt, sizeof(XD_UINT4));
			// Status
			file.Read(&m_ucSpoolSts, sizeof(XD_UINT1));
			// LoadStatus
			file.Read(&m_ucLoadSubSts, sizeof(XD_UINT1));
			// UnLoadStatus
			file.Read(&m_ucUnLoadSubSts, sizeof(XD_UINT1));

			// StartTime
			file.Read(&dTime, sizeof(double));
			m_startTime = dTime;
			file.Read(&m_startTime.m_ulMSec, sizeof(XD_UINT4));

			// FullTime
			file.Read(&dTime, sizeof(double));
			m_fullTime = dTime;
			file.Read(&m_fullTime.m_ulMSec, sizeof(XD_UINT4));

			for(i = 0; i < m_ulActualCnt; i++) {
				XD_UINT4 ulSize;
				if( file.Read(&ulSize, sizeof(ulSize)) == false ) {
					break;
				}
				pBuffer = (char*)buffTmp.Alloc( ulSize );
				if( pBuffer == NULL ) {
					break;
				}
				pWork = pBuffer;
				::memcpy(pWork, &ulSize, sizeof(ulSize));
				pWork += sizeof(ulSize);
				if( file.Read(pWork, ulSize-sizeof(ulSize)) == false ) {
					break;
				}
				this->put((unsigned char*)pBuffer, ulSize);
			}
			file.Close();
		}
	}


	m_bInitOK = true;
	return m_bInitOK;
XEXCEPT_EXIT
}

bool XSEMI_Spool::close()
{
XEXCEPT_ENTRY
	bool bRet = true;

	if( m_bInitOK == false ) {
		return false;
	}

	if( count() != NULL ) {
		XD_UINT4	ulCount = count();
		XD_UINT4	i;
		XD_UINT4	ulRcvSize;
		XD_UINT4	ulTmpBuffSize;
		XFile		file;
		XBuff		buffTmp;
		char*		pBuffer;

		if( file.Create(m_strFileName, false) == true ) {
			// SpoolAcutualカウント
			file.Write(&ulCount, sizeof(XD_UINT4));
			// SpoolTotalカウント
			file.Write(&m_ulTotalCnt, sizeof(XD_UINT4));
			// Status
			file.Write(&m_ucSpoolSts, sizeof(XD_UINT1));
			// LoadStatus
			file.Write(&m_ucLoadSubSts, sizeof(XD_UINT1));
			// UnLoadStatus
			file.Write(&m_ucUnLoadSubSts, sizeof(XD_UINT1));

			// StartTime
			file.Write(&m_startTime.m_dt, sizeof(double));
			// StartTime
			file.Write(&m_startTime.m_ulMSec, sizeof(XD_UINT4));

			// FullTime
			file.Write(&m_fullTime.m_dt, sizeof(double));
			// FullTime
			file.Write(&m_fullTime.m_ulMSec, sizeof(XD_UINT4));


			for(i = 0; i < ulCount; i++) {

				ulTmpBuffSize = this->cur_blk_size();
				pBuffer = (char*)buffTmp.Alloc( ulTmpBuffSize );
				if( pBuffer == NULL ) {
					return false;
				}
				ulRcvSize = this->get((unsigned char *)pBuffer, ulTmpBuffSize);
				if( ulRcvSize == 0 ) {
					return false;
				}
				if( *((XD_UINT4*)pBuffer) != ulRcvSize ) {
					return false;
				}
				file.Write(pBuffer, ulRcvSize);
			}
			file.Close();
		} else {
			bRet = false;
		}
	} else {
		XFileStatus fSts;
		if( XFile::GetStatus((const TCHAR*)m_strFileName, fSts) == true ) {
			XFileName::Remove((const TCHAR*)m_strFileName);
		}
	}
	m_bInitOK = false;
	return bRet;
XEXCEPT_EXIT
}
XD_UINT4 XSEMI_Spool::count()
{
XEXCEPT_ENTRY
	if( m_bInitOK == false ) {
		return 0;
	}
	return this->blk_count();
XEXCEPT_EXIT
}

int XSEMI_Spool::push(XD_UINT4 ulSec, XD_UINT4 ulMSec, XSECSHeader* pHeader, unsigned char *pData, unsigned long ulDataSize)
{
XEXCEPT_ENTRY

	XD_UINT4 vid = 0;

	if( m_bInitOK == false ) {
		return XSEMI_SPOOL_STS_NG_INIT;
	}
	/*  　１ブロックのデータ構造
	 *   ---------------------
	 *   4 | data size
	 *   8 | 書き込み時間
	 *   10| SECSヘッダー
	 *   xx| データ
	 */
	XD_UINT4 ulBlockSize = 4+8+10+ulDataSize;
	XSEMI_SpoolSF* pSF = NULL;
	
	pSF = m_sfs.get_byID( XSecsData::get_stream(*pHeader), XSecsData::get_function(*pHeader) );
	if( pSF == NULL ) {
		// 対象となるSxFxでなかった
		return XSEMI_SPOOL_STS_NA_SF;
	}
	XBuff buff;
	unsigned char* pBuffer = (unsigned char*)buff.Alloc(ulBlockSize);
	if( pBuffer == NULL ) {
		// メモリーの確保に失敗
		return XSEMI_SPOOL_STS_NG_ALLOC;
	}



	// 状態変化
	if( m_ucSpoolSts == XSEMI_GEM_SPOOL_INACTIVE ) {
		m_ulActualCnt = 0;
		m_ulTotalCnt  = 0;
		XD_UINT1 ulOldSts = m_ucSpoolSts;
		m_ucSpoolSts = XSEMI_GEM_SPOOL_ACTIVE;
		m_ucLoadSubSts = XSEMI_GEM_SPOOL_NOTFULL;
		if( m_pGem != NULL ) {
			// 状態
			vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_STS );
			if( vid != XSEMI_INVALID_VID ) {
				m_pGem->putVValue(vid, &m_ucSpoolSts, sizeof(m_ucSpoolSts));
			}
			// ロード状態
			vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_LOAD_SUBSTS );
			if( vid != XSEMI_INVALID_VID ) {
				m_pGem->putVValue(vid, &m_ucLoadSubSts, sizeof(m_ucLoadSubSts));
			}
			// アンロード状態
			vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_UNLOAD_STS );
			if( vid != XSEMI_INVALID_VID ) {
				m_pGem->putVValue(vid, &m_ucUnLoadSubSts, sizeof(m_ucUnLoadSubSts));
			}
			// スプール数
			vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_CNT_TOTAL );
			if( vid != XSEMI_INVALID_VID ) {
				m_pGem->putVValue(vid, &m_ulTotalCnt, sizeof(m_ulTotalCnt));
			}
			// 実スプール数
			vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_CNT_ACTUAL );
			if( vid != XSEMI_INVALID_VID ) {
				m_pGem->putVValue(vid, &m_ulActualCnt, sizeof(m_ulActualCnt));
			}
			// スプール開始時間
			XStrBuff strTime;
			m_startTime = m_pGem->getGemTime();
			m_pGem->getTimeString(m_startTime, strTime );
			vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_START_TIME );
			if( vid != XSEMI_INVALID_VID ) {
				m_pGem->putVValue(vid, (char*)strTime, strlen(strTime));
			}

			m_pGem->notifySpoolState(ulOldSts, m_ucSpoolSts);
		} else {
			m_startTime = XTime::getCurrentTime();
		}
	}

	bool bSoolFull = false;
	m_ulTotalCnt++;
	if( m_pGem != NULL ) {
		// スプール数
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_CNT_TOTAL );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ulTotalCnt, sizeof(m_ulTotalCnt));
		}
	}

	if( this->free_size() <= ulBlockSize && m_bOverWrite == false ) {
		bSoolFull = true;
		if( m_bSpoolFull == false ) {
			m_bSpoolFull = true;

			// 状態遷移２：
			XD_UINT1 ulOldSts = m_ucSpoolSts;
			m_ucLoadSubSts = m_ucSpoolSts = XSEMI_GEM_SPOOL_FULL;
			// スプール満杯開始時間
			if( m_pGem != NULL ) {
				XStrBuff strTime;
				m_fullTime = m_pGem->getGemTime();
				m_pGem->getTimeString(m_fullTime, strTime );
				vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SOOL_FULLTIME );
				if( vid != XSEMI_INVALID_VID ) {
					m_pGem->putVValue(vid, (char*)strTime, strlen(strTime));
				}
				
				vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_STS );
				if( vid != XSEMI_INVALID_VID ) {
					m_pGem->putVValue(vid, &m_ucSpoolSts, sizeof(m_ucSpoolSts));
				}
				m_pGem->notifySpoolState(ulOldSts, m_ucSpoolSts);
			} else {
				m_fullTime = XTime::getCurrentTime();
			}
		}
		// 書き込めるサイズがオーバーした
		if( m_bOverWrite == false ) {
			return XSEMI_SPOOL_STS_NG_FULL;
		}
	}
	if( m_ulMaxSpoolTransmit != 0 ) {
		if( m_ulMaxSpoolTransmit <= this->blk_count() ) {

			m_ucUnLoadSubSts = XSEMI_GEM_SPOOL_TRANSMIT;
			// アンロード状態
			if( m_pGem != NULL ) {
				vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_UNLOAD_STS );
				if( vid != XSEMI_INVALID_VID ) {
					m_pGem->putVValue(vid, &m_ucUnLoadSubSts, sizeof(m_ucUnLoadSubSts));
				}
			}

			// トランスミットオーバー
			return XSEMI_SPOOL_STS_NG_TRANSMIT;
		}
	}



	unsigned char* pWork = pBuffer;
	::memcpy(pWork, &ulBlockSize, sizeof(XD_UINT4));
	pWork += sizeof(XD_UINT4);
	::memcpy(pWork, &ulSec, sizeof(XD_UINT4));
	pWork += sizeof(XD_UINT4);
	::memcpy(pWork, &ulMSec, sizeof(XD_UINT4));
	pWork += sizeof(XD_UINT4);
	::memcpy(pWork, pHeader, 10);
	pWork += 10;
	::memcpy(pWork, pData, ulDataSize);

	this->put(pBuffer, ulBlockSize);




	m_ulActualCnt = this->blk_count();
	if( m_pGem != NULL ) {
		// 実スプール数
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_CNT_ACTUAL );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ulActualCnt, sizeof(m_ulActualCnt));
		}
	}
	return XSEMI_SPOOL_STS_OK;
XEXCEPT_EXIT
}

int XSEMI_Spool::purge()
{
XEXCEPT_ENTRY
	int			sts;
	XD_UINT4	vid;

	if( m_bInitOK == false ) {
		return XSEMI_SPOOL_STS_NG_INIT;
	}
	if( this->blk_count() == 0 ) {
		return XSEMI_SPOOL_STS_NG_NON;
	}
	

	XD_UINT4 ulSec,  ulMSec;
	XSECSHeader header;
	XBuff buff;
	unsigned long ulDataSize;
	XD_UINT1 ulOldSts;
	// ---------------------------------------------------
	// 状態遷移４
	m_ucUnLoadSubSts = XSEMI_GEM_SPOOL_PURGE;
	if( m_pGem != NULL ) {
		// アンロード状態
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_UNLOAD_STS );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ucUnLoadSubSts, sizeof(m_ucUnLoadSubSts));
		}
	}
	// ---------------------------------------------------


	while(1) {
		sts = this->pop(ulSec, ulMSec, header, buff, ulDataSize);
		if( XSEMI_SPOOL_STS_OK != sts ) {
			break;
		}
		m_bSpoolFull = false;
	}


	// ---------------------------------------------------
	//　状態遷移３
	ulOldSts = m_ucSpoolSts;
	m_ulActualCnt = this->blk_count();
	m_ucUnLoadSubSts = XSEMI_GEM_SPOOL_NOOUTPUT;

	if( m_bSpoolFull == false ) {
		m_ucLoadSubSts = XSEMI_GEM_SPOOL_NOTFULL;
	}
	if( this->blk_count() == 0 ) {
		m_ucSpoolSts = XSEMI_GEM_SPOOL_INACTIVE;
	} else {
		if( m_bSpoolFull == true ) {
			m_ucSpoolSts = XSEMI_GEM_SPOOL_FULL;
		} else {
			m_ucSpoolSts = XSEMI_GEM_SPOOL_ACTIVE;
		}
	}
	if( m_pGem != NULL ) {
		// 状態
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_STS );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ucSpoolSts, sizeof(m_ucSpoolSts));
		}
		// ロード状態
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_LOAD_SUBSTS );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ucLoadSubSts, sizeof(m_ucLoadSubSts));
		}
		// アンロード状態
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_UNLOAD_STS );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ucUnLoadSubSts, sizeof(m_ucUnLoadSubSts));
		}
		// 実スプール数
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_CNT_ACTUAL );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ulActualCnt, sizeof(m_ulActualCnt));
		}
		m_pGem->notifySpoolState(ulOldSts, m_ucSpoolSts);
	}
	// ---------------------------------------------------



	return XSEMI_SPOOL_STS_OK;
XEXCEPT_EXIT
}

int XSEMI_Spool::transmit()
{
XEXCEPT_ENTRY
	int			sts;
	XD_UINT4	vid;

	if( m_bInitOK == false ) {
		return XSEMI_SPOOL_STS_NG_INIT;
	}
	if( this->blk_count() == 0 ) {
		return XSEMI_SPOOL_STS_NG_NON;
	}
	if( m_pGem == NULL ) {
		return XSEMI_SPOOL_STS_NG_INIT;
	}
	

	XD_UINT4 ulSec,  ulMSec;
	XSECSHeader header;
	XBuff buff;
	unsigned long ulDataSize;
	XD_UINT1 ulOldSts;
	// ---------------------------------------------------
	// 状態遷移５
	m_ucUnLoadSubSts = XSEMI_GEM_SPOOL_TRANSMIT;
	if( m_pGem != NULL ) {
		// アンロード状態
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_UNLOAD_STS );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ucUnLoadSubSts, sizeof(m_ucUnLoadSubSts));
		}
	}
	// ---------------------------------------------------

	while(1) {
		sts = this->pop(ulSec, ulMSec, header, buff, ulDataSize);
		if( XSEMI_SPOOL_STS_OK != sts ) {
			break;
		}
		m_bSpoolFull = false;
		XSECS_SEND_STS sendSts =  m_pGem->sendRequest(&header, (unsigned char*)buff, ulDataSize);
		if( sendSts != XSECS_SEND_STS_S ) {
			break;
		}
	}

	ulOldSts = m_ucSpoolSts;
	m_ulActualCnt = this->blk_count();
	m_ucUnLoadSubSts = XSEMI_GEM_SPOOL_NOOUTPUT;

	
	if( m_bSpoolFull == false ) {
		m_ucLoadSubSts = XSEMI_GEM_SPOOL_NOTFULL;
	}
	if( this->blk_count() == 0 ) {
		m_ucSpoolSts = XSEMI_GEM_SPOOL_INACTIVE;
	} else {
		if( m_bSpoolFull == true ) {
			m_ucSpoolSts = XSEMI_GEM_SPOOL_FULL;
		} else {
			m_ucSpoolSts = XSEMI_GEM_SPOOL_ACTIVE;
		}
	}
	if( m_pGem != NULL ) {
		// 状態
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_STS );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ucSpoolSts, sizeof(m_ucSpoolSts));
		}
		// ロード状態
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_LOAD_SUBSTS );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ucLoadSubSts, sizeof(m_ucLoadSubSts));
		}
		// アンロード状態
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_UNLOAD_STS );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ucUnLoadSubSts, sizeof(m_ucUnLoadSubSts));
		}
		// 実スプール数
		vid = m_pGem->getWellKnownVID( XSEMI_Gem::SV_INDEX_SPOOL_CNT_ACTUAL );
		if( vid != XSEMI_INVALID_VID ) {
			m_pGem->putVValue(vid, &m_ulActualCnt, sizeof(m_ulActualCnt));
		}
		m_pGem->notifySpoolState(ulOldSts, m_ucSpoolSts);
	}
	// ---------------------------------------------------
	return XSEMI_SPOOL_STS_OK;
XEXCEPT_EXIT
}

int XSEMI_Spool::pop(XD_UINT4& ulSec, XD_UINT4& ulMSec, XSECSHeader& pHeader, XBuff &buff, unsigned long& ulDataSize)
{
XEXCEPT_ENTRY

	if( m_bInitOK == false ) {
		return XSEMI_SPOOL_STS_NG_INIT;
	}
	if( this->blk_count() == 0 ) {
		return XSEMI_SPOOL_STS_NG_NON;
	}
	
	XD_UINT4	ulBlockSize = this->cur_blk_size();
	unsigned char* pBuffer = (unsigned char*)buff.Alloc(ulBlockSize);
	if( pBuffer == 0 ) {
		return XSEMI_SPOOL_STS_NG_ALLOC;
	}
	XD_UINT4	ulRevSize = this->get(pBuffer, ulBlockSize);
	if( ulBlockSize != ulRevSize ) {
		return XSEMI_SPOOL_STS_NG_BROKEN;
	}


	unsigned char* pWork = pBuffer;
	::memcpy(&ulRevSize, pWork, sizeof(XD_UINT4));
	pWork += sizeof(XD_UINT4);
	::memcpy(&ulSec, pWork, sizeof(XD_UINT4));
	pWork += sizeof(XD_UINT4);
	::memcpy(&ulMSec, pWork, sizeof(XD_UINT4));
	pWork += sizeof(XD_UINT4);
	::memcpy(&pHeader, pWork, 10);
	pWork += 10;
	ulDataSize = ulBlockSize-(4+8+10);
	::memmove(pBuffer, pWork, ulBlockSize-(4+8+10));
	
	return XSEMI_SPOOL_STS_OK;
XEXCEPT_EXIT
}


