#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata_tbl.h"
#include "xlib/xeception.h"
#include <fstream>


XDataTblBase::XDataTblBase()
{

}

XDataTblBase::~XDataTblBase()
{

}


//
//	XDataTblFs
//
XDataTblFs::XDataTblFs()
	:	m_pClmns( NULL )
	,	m_pFs( NULL )
	,	m_bNewFs( false )
	,	m_bWriteMode( false )
	,	m_nLine( 0 )
	,	m_strSystem( true )
{
}

XDataTblFs::~XDataTblFs()
{
	close();
}
#include <iostream>

bool XDataTblFs::open(const char* pFileName, 
					  bool bFirstClumnName, 
					  XDataTblFs::FsFormat format, 
					  bool bWriteMode /*= false*/)
{
XEXCEPT_ENTRY
	std::fstream* pFs = NULL;


	close();

	switch( format ) {
	case FsFormat_CSV:
	case FsFormat_TAB:
		break;
	default:
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("uknown format:[%u]"),  format),
				0, 0);
		}
		return false;
		break;
	}


	if( bWriteMode == false ) {
		pFs = new std::fstream(pFileName, std::ios_base::in);
	} else {
		pFs = new std::fstream(pFileName, std::ios_base::out);
	}
	
	if( pFs->is_open() == false ) {
		delete pFs;

		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("fail to open file:[%s]"), 
						(const TCHAR*)XStrBuff(pFileName)),
				0, 0);
		}
		return false;
	}
	m_bNewFs = true;
	m_bWriteMode = bWriteMode;
	m_pFs = pFs;
	m_strSystem = pFileName;
	m_format = format;

	switch( format ) {
	case FsFormat_CSV:
		m_lineDatas.SetDelimite( _T(',') );
		m_lineDatas.SetIndentChar( _T('\"') );
		break;
	case FsFormat_TAB:
		m_lineDatas.SetDelimite( _T('\t') );
		m_lineDatas.SetIndentChar( 0 );
		break;
	}

	// カラム名の情報をロード
	if( bFirstClumnName == true && bWriteMode == false  ) {
		// 読み込み

		if( ! m_pFs->eof() ) {
			char szBuff[4029];
			XStrBuffSplit splitter;
			memset(szBuff, 0, sizeof(szBuff));
			m_pFs->getline( szBuff, sizeof( szBuff )-sizeof(szBuff[0]) );

			switch( format ) {
			case FsFormat_CSV:
				splitter.SetDelimite( _T(',') );
				splitter.SetIndentChar( _T('\"') );
				break;
			case FsFormat_TAB:
				splitter.SetDelimite( _T('\t') );
				splitter.SetIndentChar( 0 );
				break;
			}

			splitter = szBuff;

			for(int i = 0; i < splitter.GetArgCouunt(); i++ ) {
				XDATA_TYPE		dataType = XDATA_TYPE_A;	// デフォルトA
				TCHAR* pClmnName = (TCHAR*)splitter.GetArgv()[ i ];
				if( pClmnName != NULL ) {
					TCHAR *pType = XStrBuff::StrChr(pClmnName, _T(':'));
					if( pType != 0 ) {
						dataType = XData::parseDataType_From_String( XStrBuff(pClmnName, pType-pClmnName) );
						if( dataType != XDATA_TYPE_END ) {
							pClmnName = (pType + 1);
						}
					}
					ClmnInf* pNewClmn = new ClmnInf();
					pNewClmn->dataType = dataType;
					pNewClmn->strClmnName = pClmnName;

					m_cntClmnSchemas.AddTail( pNewClmn );
				}				
			}
		}
	}

	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::close()
{
XEXCEPT_ENTRY

	if( m_pFs != NULL && m_bNewFs == true ) {
		std::fstream* pFs = (std::fstream*)m_pFs;
		pFs->close();
		delete pFs;
	}
	m_pFs = NULL;
	m_bNewFs = false;
	m_bWriteMode = false;

	if( m_pClmns != NULL ) {
		delete m_pClmns;
	}
	m_pClmns = NULL;


	if( m_cntWriteDatas.IsEmpty() == false ) {
		XPOSITION pos1, pos2;
		for( pos1 = m_cntWriteDatas.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
			XStrBuff *pItem = (XStrBuff*)m_cntWriteDatas.GetNext( pos1 );
			delete pItem;
		}
	}

	if( m_cntClmnSchemas.IsEmpty() == false ) {
		XPOSITION pos1, pos2;
		for( pos1 = m_cntClmnSchemas.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
			ClmnInf *pItem = (ClmnInf*)m_cntClmnSchemas.GetNext( pos1 );
			delete pItem;
		}
	}
	m_nLine = 0;
	return true;
XEXCEPT_EXIT
	return false;
}

void XDataTblFs::setClmnsInf(int cnt, XDataTblFs::ClmnInf* pClmns)
{
XEXCEPT_ENTRY
	// 現在設定されている情報のクリア
	if( m_cntClmnSchemas.IsEmpty() == false ) {
		XPOSITION pos1, pos2;
		for( pos1 = m_cntClmnSchemas.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
			ClmnInf *pItem = (ClmnInf*)m_cntClmnSchemas.GetNext( pos1 );
			delete pItem;
		}
	}


	for(int i = 0; i < cnt; i++ ) {
		ClmnInf* pNewClmn = new ClmnInf();
		pNewClmn->dataType = pClmns[i].dataType;
		pNewClmn->strClmnName = pClmns[i].strClmnName;
		m_cntClmnSchemas.AddTail(pNewClmn);
	}
	

XEXCEPT_EXIT
}


bool XDataTblFs::getColumnName(int index, XStrBuff &buf)
{
XEXCEPT_ENTRY
	ClmnInf* p = (ClmnInf*)m_cntClmnSchemas.GetAt( m_cntClmnSchemas.FindIndex(index) );
	if( p == NULL ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("column range over. specify:[%d] def:[%d]"), 
						index,
						m_cntClmnSchemas.GetCount(),
						(const TCHAR*)m_strSystem),
				0, 0);
		}
		return false;
	}
XEXCEPT_EXIT
	return true;
}

int XDataTblFs::getColumnCount()
{
XEXCEPT_ENTRY
	return m_cntClmnSchemas.GetCount();
XEXCEPT_EXIT
	return 0;
}

XDATA_TYPE XDataTblFs::getColumnType(int index)
{
XEXCEPT_ENTRY
	ClmnInf* p = (ClmnInf*)m_cntClmnSchemas.GetAt( m_cntClmnSchemas.FindIndex(index) );
	if( p == NULL ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("column range over. specify:[%d] def:[%d]"), 
						index,
						m_cntClmnSchemas.GetCount(),
						(const TCHAR*)m_strSystem),
				0, 0);
		}
		return XDATA_TYPE_END;
	}
	 return p->dataType;
XEXCEPT_EXIT
	return XDATA_TYPE_END;
}





bool XDataTblFs::next()
{
XEXCEPT_ENTRY


	if( m_pFs == NULL ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("not initilized. system:[%s]"), 
						(const TCHAR*)m_strSystem),
				0, 0);
		}
		return false;
	}
	if( m_bWriteMode == false ) {
		// 読み込み
		char szBuff[4029];
		if( m_pFs->eof() ) {
			return true;
		}
		memset(szBuff, 0, sizeof(szBuff));
		m_pFs->getline( szBuff, sizeof( szBuff )-sizeof(szBuff[0]) );

		m_lineDatas = szBuff;
	} else {
		// 書き込み
		bool bFirst = true;
		if( m_cntWriteDatas.IsEmpty() == false ) {
			XPOSITION pos1, pos2;
			for( pos1 = m_cntWriteDatas.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ) {
				XStrBuff *pItem = (XStrBuff*)m_cntWriteDatas.GetNext( pos1 );
				
				if( bFirst == false ) {
					switch( m_format ) {
					case FsFormat_CSV:
						(*m_pFs) << _T(",");
						break;
					case FsFormat_TAB:
						(*m_pFs) << _T("\t");
						break;
					default:
						_XASSERT(0);
						break;
					}
				}
				switch( m_format ) {
				case FsFormat_CSV:
					(*m_pFs) << (const TCHAR*)XStrBuffCSV( (const TCHAR*)(*pItem) );
					break;
				case FsFormat_TAB:
					(*m_pFs) << (const TCHAR*)(*pItem);
					break;
				default:
					_XASSERT(0);
					break;
				}
				bFirst = false;
			}
		}
		(*m_pFs) << _T("\n");
	}
	m_nLine++;
	return true;
XEXCEPT_EXIT
	return false;
}

// setXXX
bool XDataTblFs::setText(int index, const char* val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setText(int index, const wchar_t* val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setBool(int index, bool val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << (int)val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setInt1(int index, XD_INT1 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << (int)val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setUInt1(int index, XD_UINT1 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << (unsigned int)val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setInt2(int index, XD_INT2 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setUInt2(int index, XD_UINT2 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setInt4(int index, XD_INT4 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setUInt4(int index, XD_UINT4 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setInt8(int index, XD_INT8 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
//	*pStr << val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setUInt8(int index, XD_UINT8 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
//	*pStr << val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setFloat4(int index, XD_FLOAT4 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setFloat8(int index, XD_FLOAT8 val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");
	*pStr << val;
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setBlob(int index, void* val, int size)
{
XEXCEPT_ENTRY
	TCHAR	sz[10];
	int		i = 0;
	unsigned char* pData = (unsigned char*)val;
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = _T("");

	::memset(sz, 0, sizeof(sz));
	for(i = 0; i < size; i++ ) {
		XStrBuff::nprintf(sz, XCOUNTOF(sz), XCOUNTOF(sz)-sizeof(TCHAR), _T("%02X"),  pData[i] );
		*pStr << sz;
	}
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setDate(int index, const XTime* val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = val->Format(_T("%Y/%m/%d"));
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setTime(int index, const XTime* val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = val->Format(_T("%H:%M:%S"));
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setTimeStamp(int index, const XTime* val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	*pStr = val->Format(XTIME_STD_FORMAT_YYYYmmDDhhMMss);
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setNull(int index)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
	return false;
}

bool XDataTblFs::setRData(int index, XRichData* val)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = getWriteData( index );
	if( pStr == NULL ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
	return false;
}


// getXXX
bool XDataTblFs::getText(int index, XStrBuff &buf)
{
XEXCEPT_ENTRY
	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	if( m_lineDatas.GetArgv()[index] == NULL ) {
		buf = _T("");
	} else {
		buf = m_lineDatas.GetArgv()[index];
	}
	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getBool(int index, bool* val)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}

	*val = false;

	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("no data. specify:[%d] system:[%s]. line:[%d]"), 
						index,
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	if( XStrBuff::StrICmp(p, _T("true")) == 0 ) {
		*val = true;
	} else 
	if( XStrBuff::StrICmp(p, _T("false")) == 0 ) {
		*val = false;
	} else {
		if( XStrBuff::isDigitsEx( p ) != 0 ) {
			XD_INT4 lVal = XStrBuff::strtol(p, NULL, -1);
			if( errno == ERANGE ) {
				return false;
			}
			if( lVal == 0 ) { 
				*val = false;
			} else {
				*val = true;
			}
		} else {
			// 真偽値の判定不可だった
			return false;
		}
	}	
	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getInt1(int index, XD_INT1* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	if( XStrBuff::isDigitsEx( p ) != 0 ) {
		XD_INT4 iVal = XStrBuff::strtol(p, NULL, -1);
		if( (iVal & 0xFFFFFF00 ) != 0 ) {
			// オーバーフロー
			return false;
		}
		*pVal = (XD_INT1)iVal;
	} else {
		// 数値の判定不可だった
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getUInt1(int index, XD_UINT1* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	if( XStrBuff::isDigitsEx( p ) != 0 ) {
		XD_UINT4 iVal = XStrBuff::strtoul(p, NULL, -1);
		if( (iVal & 0xFFFFFF00 ) != 0 ) {
			// オーバーフロー
			return false;
		}
		*pVal = (XD_UINT1)iVal;
	} else {
		// 数値の判定不可だった
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getInt2(int index, XD_INT2* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	if( XStrBuff::isDigitsEx( p ) != 0 ) {
		XD_INT4 iVal = XStrBuff::strtol(p, NULL, -1);
		if( (iVal & 0xFFFF0000 ) != 0 ) {
			// オーバーフロー
			return false;
		}
		*pVal = (XD_INT2)iVal;
	} else {
		// 数値の判定不可だった
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getUInt2(int index, XD_UINT2* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	if( XStrBuff::isDigitsEx( p ) != 0 ) {
		XD_UINT4 iVal = XStrBuff::strtoul(p, NULL, -1);
		if( (iVal & 0xFFFF0000 ) != 0 ) {
			// オーバーフロー
			return false;
		}
		*pVal = (XD_UINT2)iVal;
	} else {
		// 数値の判定不可だった
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getInt4(int index, XD_INT4* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	if( XStrBuff::isDigitsEx( p ) != 0 ) {
		*pVal = XStrBuff::strtol(p, NULL, -1);
	} else {
		// 数値の判定不可だった
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getUInt4(int index, XD_UINT4* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	if( XStrBuff::isDigitsEx( p ) != 0 ) {
		*pVal = XStrBuff::strtoul(p, NULL, -1);
	} else {
		// 数値の判定不可だった
		return false;
	}

	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getInt8(int index, XD_INT8* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	if( XStrBuff::isDigitsEx( p ) != 0 ) {
		*pVal = XStrBuff::strtoll(p, NULL, -1);
	} else {
		// 数値の判定不可だった
		return false;
	}
	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getUInt8(int index, XD_UINT8* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	if( XStrBuff::isDigitsEx( p ) != 0 ) {
		*pVal = XStrBuff::strtoull(p, NULL, -1);
	} else {
		// 数値の判定不可だった
		return false;
	}
	return true;
XEXCEPT_EXIT
	return false;
}
bool XDataTblFs::getFloat4(int index, XD_FLOAT4* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	*pVal = (XD_FLOAT4)XStrBuff::strtod(p, NULL);

	return true;
XEXCEPT_EXIT
	return false;
}


bool XDataTblFs::getFloat8(int index, XD_FLOAT8* pVal)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}
	*pVal = (XD_FLOAT8)XStrBuff::strtod(p, NULL);

	return true;
XEXCEPT_EXIT
	return false;
}


bool XDataTblFs::getBlob(int index, XBuff& buf, int* p_size)
{
XEXCEPT_ENTRY

	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		*p_size = 0;
		return true;
	}
	// Blobは16進数ダンプ
	XInflateStreamBuff tmpBuff( XStrBuff::StrLen(p) );
	std::ostream stream(&tmpBuff);
	unsigned char c, ctmp, odd;
	c = 0;
	ctmp = 0;
	odd = 0;
	while( *p != 0 ) {
		if( *p <= 'a' && *p <= 'f' ) {
			ctmp = 10 + (*p - 'a');
		} else
		if( *p <= 'A' && *p <= 'F' ) {
			ctmp = 10 + (*p - 'A');
		} else
		if( *p <= '0' &&*p <= '9' ) {
			ctmp = *p - '0';
		}
		if( odd == 0 ) {
			c = ctmp;
			c <<= 4;
			odd = 1;
		} else {
			c |= ctmp;
			stream.write((const char*)&c, 1);
			c = 0;
			ctmp = 0;
			odd = 0;
		}
		p++;
	}
	buf.memcpy( tmpBuff.getBuff(), tmpBuff.size() );
	return true;
XEXCEPT_EXIT
	return false;
}


bool XDataTblFs::getDate(int index, XTime* val)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}

	TCHAR* pEndPtr = NULL;
	TCHAR* pWorkPtr = NULL;
	int year, month, day;

	pWorkPtr = (TCHAR*)p;
	year = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;
	month = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;
	day = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);

	return true;
XEXCEPT_EXIT
	return false;
}


bool XDataTblFs::getTime(int index, XTime* val)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}

	TCHAR* pEndPtr = NULL;
	TCHAR* pWorkPtr = NULL;
	int hour, minute, second;

	pWorkPtr = (TCHAR*)p;
	hour = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;
	
	minute = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;
	

	second = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);

	return true;
XEXCEPT_EXIT
	return false;
}


bool XDataTblFs::getTimeStamp(int index, XTime* val)
{
XEXCEPT_ENTRY
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return false;
	}

	TCHAR* pEndPtr = NULL;
	TCHAR* pWorkPtr = NULL;
	int year, month, day, hour, minute, second;

	pWorkPtr = (TCHAR*)p;
	year = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;
	month = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != '-') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;
	day = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ' ') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;

	
	hour = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;
	
	minute = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);
	if( pEndPtr == NULL && *pEndPtr != ':') {
		return false;
	}
	pWorkPtr = (TCHAR*)++pEndPtr;
	

	second = (int)XStrBuff::strtoul(pWorkPtr, &pEndPtr, 10);

	return true;
XEXCEPT_EXIT
	return false;
}


bool XDataTblFs::isNull(int index, bool* pVal)
{
XEXCEPT_ENTRY	
	const TCHAR* p = NULL;

	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	p = m_lineDatas.GetArgv()[index];
	if( p == NULL ) {
		return true;
	}
	return XStrBuff::StrLen(p) == 0 ;
XEXCEPT_EXIT
	return false;
}


bool XDataTblFs::getRData(int index, XRichData* val)
{
XEXCEPT_ENTRY
	// Non Support	
	if( m_lineDatas.GetArgCouunt() <= index ) {
		if( m_bOnErrorThrow == true ) {
			XEXCEPT_THROW_STR(XExcept_FILE, 
				(const TCHAR*)XStrBuff::FormatS(
					_T("range over. specify:[%d] def:[%d] system:[%s]. line:[%d]"), 
						index,
						m_lineDatas.GetArgCouunt(),
						(const TCHAR*)m_strSystem,
						m_nLine ),
				0, 0);
		}
		return false;
	}
	return false;
XEXCEPT_EXIT
	return false;
}

// protected:
XStrBuff* XDataTblFs::getWriteData(int index)
{
XEXCEPT_ENTRY
	XStrBuff *pStr = NULL;

	// 要素が足りていない場合は、足す
	if( m_cntWriteDatas.GetCount() <= index ) {
		int fewCnt = (m_cntWriteDatas.GetCount() - index) + 1;
		for(int i = m_cntWriteDatas.GetCount(); i < (index+1); i++ ) {
			pStr = new XStrBuff(_T(""));	
			m_cntWriteDatas.AddTail( pStr );
		}
		pStr = NULL;
	}

	pStr = (XStrBuff*)m_cntWriteDatas.GetAt( m_cntWriteDatas.FindIndex(index) );
	if( pStr == NULL ) {
		return NULL;
	}
	return pStr;
XEXCEPT_EXIT
	return NULL;
}