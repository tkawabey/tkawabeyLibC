#include "xlib/semi/secs/xsecs_text_parser.h"
#include "xlib/xfile.h"
#include "xlib/xeception.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

XSecsTextParser::XSecsTextParser()
	:	m_ulLine(1)
	,	m_strLastErrorString(_T(""))
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XStrBuff XSecsTextParser::getErrorString()
{
XEXCEPT_ENTRY
	return m_strLastErrorString;
XEXCEPT_EXIT
}

XSecsTextParser::~XSecsTextParser()
{
XEXCEPT_ENTRY


XEXCEPT_EXIT
}


bool XSecsTextParser::parseFromFile(const XStrBuff& strFilePath, XSecsData* pData)
{
XEXCEPT_ENTRY	
	XFile		file;
	XStrBuff	buff;
	size_t		data_size = 0;
	TCHAR*		pBuff = NULL;

	if( file.Open( strFilePath.c_str() ) == false ) {
		m_strLastErrorString.Format(_T("fail to opne file [%s] errno=%d"), strFilePath.c_str(), errno );
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
		return false;
	}
	if( file.readAllText( buff ) == false ) {
		file.Close();
		m_strLastErrorString.Format(_T("fail to read file [%s]"), strFilePath.c_str() );
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
		return false;
	}
	file.Close();
	pBuff = buff;
	data_size = buff.getLength();

	m_ulLine = 1;
	return XSecsTextParser::parse(&pBuff, data_size, pData);
XEXCEPT_EXIT
}


bool XSecsTextParser::parse(TCHAR* *pBuff, size_t &data_size, XSecsData* pData)
{
XEXCEPT_ENTRY
	TCHAR*	pwork;

	if( moveNextValue(pBuff, data_size) == false ) {
		return false;
	}

	/*
		<S6F11 W
		  ...
		>
	*/
	if( movePointer(pBuff, data_size, _T("<S")) == false ||
		movePointer(pBuff, data_size, 2) == false) {

		m_strLastErrorString.Format(_T("invalid SxFx String"));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);

		return false;
	}

	/*
		6F11 W
		  ...
		>
	*/
	// Stream
	pData->put_stream( (XD_UINT1)XStrBuff::strtol(*pBuff, &pwork, 10) );
	if( *pwork != 'F' ) {

		m_strLastErrorString.Format(_T("invalid SxFx String"));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);

		return false;
	}
	/* pwork is 
		F11 W
		  ...
		>
	*/
	pwork += 1;
	/* pwork is 
		11 W
		  ...
		>
	*/
	data_size -= (pwork - *pBuff);
	*pBuff = pwork;
	pData->put_function( (XD_UINT1)XStrBuff::strtol(*pBuff, &pwork, 10) );
	/* pwork is 
		 W
		  ...
		>
	*/
	*pBuff = pwork;
	if( movePointerUnSPACE(pBuff, data_size) == false) {

		m_strLastErrorString.Format(_T("abnormal terminate."));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);

		return false;
	}
	/* pwork is 
		W
		  ...
		>
	*/
	if( **pBuff == 'W' ) {
		pData->put_wait(true);
		if( movePointer(pBuff, data_size, 1) == false) {

			m_strLastErrorString.Format(_T("abnormal terminate."));
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);

			return false;
		}
	} else {
		pData->put_wait(false);
	}
	if( movePointerUnSPACE(pBuff, data_size) == false) {

		m_strLastErrorString.Format(_T("abnormal terminate."));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);

		return false;
	}
	if( **pBuff == '>' ) {
		return true;
	}

	if( **pBuff != '<' ) {
		return true;
	}

	
	XDATA_TYPE dataType;
	switch( (*pBuff)[1] ) {
	case 'L':
		dataType = XDATA_TYPE_L;
		if( movePointer(pBuff, data_size, 2 ) == false ) {
			return false;
		}
		break;
	case 'B':
		if( XStrBuff::StrNCmp(&(*pBuff)[1], _T("BOOL"), 4)    == 0 ) {
			if( movePointer(pBuff, data_size, 5 ) == false ) {
				return false;
			}
			dataType = XDATA_TYPE_BOOL;
		} else
		if( XStrBuff::StrNCmp(&(*pBuff)[1], _T("BOOLEAN"), 7) == 0) {
			if( movePointer(pBuff, data_size, 8 ) == false ) {
				return false;
			}
			dataType = XDATA_TYPE_BOOL;
		} else {
			dataType = XDATA_TYPE_B;
			if( movePointer(pBuff, data_size, 2 ) == false ) {
				return false;
			}
		}
		break;
	case 'A':
		dataType = XDATA_TYPE_A;
		if( movePointer(pBuff, data_size, 2 ) == false ) {
			return false;
		}
		break;
	case 'J':
		if( movePointer(pBuff, data_size, 2 ) == false ) {
			return false;
		}
		dataType = XDATA_TYPE_J;
		break;
	case 'U':
		if( (*pBuff)[2] == '1' ) {
			dataType = XDATA_TYPE_U1;
		} else
		if( (*pBuff)[2] == '2' ) {
			dataType = XDATA_TYPE_U2;
		} else 
		if( (*pBuff)[2] == '4' ) {
			dataType = XDATA_TYPE_U4;
		} else
		if( (*pBuff)[2] == '8' ) {
			dataType = XDATA_TYPE_U8;
		} else {
			m_strLastErrorString.Format(_T("invalid message type U%c"), (*pBuff)[2] );
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
			return false;

		}
		if( movePointer(pBuff, data_size, 3 ) == false ) {
			return false;
		}
		break;
	case 'I':
		if( (*pBuff)[2] == '1' ) {
			dataType = XDATA_TYPE_I1;
		} else
		if( (*pBuff)[2] == '2' ) {
			dataType = XDATA_TYPE_I2;
		} else 
		if( (*pBuff)[2] == '4' ) {
			dataType = XDATA_TYPE_I4;
		} else
		if( (*pBuff)[2] == '8' ) {
			dataType = XDATA_TYPE_I8;
		} else {
			m_strLastErrorString.Format(_T("invalid message type I%c"), (*pBuff)[2] );
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
			return false;
		}
		if( movePointer(pBuff, data_size, 3 ) == false ) {
			return false;
		}
		break;
	case 'F':
		if( (*pBuff)[2] == '4' ) {
			dataType = XDATA_TYPE_F4;
		} else
		if( (*pBuff)[2] == '8' ) {
			dataType = XDATA_TYPE_F8;
		} else {
			m_strLastErrorString.Format(_T("invalid message type F%c"), (*pBuff)[2] );
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
			return false;
		}
		if( movePointer(pBuff, data_size, 3 ) == false ) {
			return false;
		}
		break;
	default:
		m_strLastErrorString.Format(_T("invalid message type %c%c"), (*pBuff)[1],(*pBuff)[2] );
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
		return false;
		break;
	}

	// 型の次は、ブランクか、[であること
	if( !(**pBuff == ' '  || 
		  **pBuff == '\t' ||
		  **pBuff == '\r' ||
		  **pBuff == '\n' ||
		  **pBuff == '['  ) ) {
		m_strLastErrorString.Format(_T("invalid message type"));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
	}


	// 子アイテムを作成
	XRichData *pItem = pData->addChiled( dataType );
	if( pItem == NULL ) {

		m_strLastErrorString.Format(_T("addChiled"));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);

		return false;
	}


	return innerParseSmlText(pBuff, data_size, pItem);
XEXCEPT_EXIT
}

bool XSecsTextParser::innerParseSmlText(TCHAR* *pData, size_t &data_size, XRichData* pObj)
{
XEXCEPT_ENTRY
	TCHAR *pWork = NULL;
	TCHAR *pWork2 = NULL;	
	int  nLengthByte = -1;


	if( moveNextValue(pData, data_size) == false  ) {
		return false;
	}
	if( **pData == '[' ) {
		/*
		<L[1/1]
			  <U1[1/1] 100>
			>
		  >
		*/
		if( movePointer(pData, data_size, _T("[")) == false ) {

			m_strLastErrorString.Format(_T("SizeOfSize Error"));
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);


			return false;
		}
		pWork = *pData;
		if( movePointer(pData, data_size, _T("]")) == false ) {

			m_strLastErrorString.Format(_T("SizeOfSize Error"));
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);

			return false;
		}

		// LengthByte
		**pData = 0;
		pWork++;
		while( *pWork != 0 ) {
			if( *pWork == '/' ) {
				break;
			}
			pWork++;
		}
		if( *pWork == '/' ) {
			pWork++;
			pWork2 = pWork;
			while( *pWork2 != 0 ) {
				if( !('0' <= *pWork2  && *pWork2 <= '9') ) {
					break;
				}
				pWork2++;
			}
			if( *pWork2 == 0 && *pWork != 0 ) {
				nLengthByte = XStrBuff::strtol(pWork, NULL, 10);
			}
		}


		/*
		]
			  <U1[1/1] 100>
			>
		  >
		*/
		if( movePointer(pData, data_size, 1) == false ) {
			return false;
		}
	}

	/*
	<U1[1/1] 100>
		>
	  >
	*/
	if( moveNextValue(pData, data_size) == false ) {
		return false;
	}




	switch( pObj->getType() ) {
	case XDATA_TYPE_L:
		if( **pData == '>') {
			if( movePointer(pData, data_size, 1) == false ) {
				return false;
			}
			if( moveNextValue(pData, data_size) == false ) {
				return false;
			}

			if( 2 <= nLengthByte && nLengthByte <= 3 ) {
				pObj->setLengthByte( nLengthByte );
			}
			return true;
		} else {
			while(1) {
				XDATA_TYPE dataType;
				switch( (*pData)[1] ) {
				case 'L':
					dataType = XDATA_TYPE_L;
					if( movePointer(pData, data_size, 2 ) == false ) {
						return false;
					}
					break;
				case 'B':
					if( XStrBuff::StrNCmp(&(*pData)[1], _T("BOOLEAN"), 7)    == 0 ) {
						dataType = XDATA_TYPE_BOOL;
						if( movePointer(pData, data_size, 8 ) == false ) {
							return false;
						}
					}  else
					if( XStrBuff::StrNCmp(&(*pData)[1], _T("BOOL"), 4) == 0) {
						dataType = XDATA_TYPE_BOOL;
						if( movePointer(pData, data_size, 5 ) == false ) {
							return false;
						}
					} else {
						dataType = XDATA_TYPE_B;
						if( movePointer(pData, data_size, 2 ) == false ) {
							return false;
						}
					}
					break;
				case 'A':
					dataType = XDATA_TYPE_A;
					if( movePointer(pData, data_size, 2 ) == false ) {
						return false;
					}
					break;
				case 'J':
					dataType = XDATA_TYPE_J;
					if( movePointer(pData, data_size, 2 ) == false ) {
						return false;
					}
					break;
				case 'U':
					if( (*pData)[2] == '1' ) {
						dataType = XDATA_TYPE_U1;
					} else
					if( (*pData)[2] == '2' ) {
						dataType = XDATA_TYPE_U2;
					} else 
					if( (*pData)[2] == '4' ) {
						dataType = XDATA_TYPE_U4;
					} else
					if( (*pData)[2] == '8' ) {
						dataType = XDATA_TYPE_U8;
					} else {
						m_strLastErrorString.Format(_T("invalid message type %c%c"), (*pData)[1],(*pData)[2] );
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( movePointer(pData, data_size, 3 ) == false ) {
						return false;
					}
					break;
				case 'I':
					if( (*pData)[2] == '1' ) {
						dataType = XDATA_TYPE_I1;
					} else
					if( (*pData)[2] == '2' ) {
						dataType = XDATA_TYPE_I2;
					} else 
					if( (*pData)[2] == '4' ) {
						dataType = XDATA_TYPE_I4;
					} else
					if( (*pData)[2] == '8' ) {
						dataType = XDATA_TYPE_I8;
					} else {
						m_strLastErrorString.Format(_T("invalid message type %c%c"), (*pData)[1],(*pData)[2] );
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( movePointer(pData, data_size, 3 ) == false ) {
						return false;
					}
					break;
				case 'F':
					if( (*pData)[2] == '4' ) {
						dataType = XDATA_TYPE_F4;
					} else
					if( (*pData)[2] == '8' ) {
						dataType = XDATA_TYPE_F8;
					} else {
						m_strLastErrorString.Format(_T("invalid message type %c%c"), (*pData)[1],(*pData)[2] );
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( movePointer(pData, data_size, 3 ) == false ) {
						return false;
					}
					break;
				default:
					m_strLastErrorString.Format(_T("invalid message type %c%c"), (*pData)[1],(*pData)[2] );
					XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
					return false;
					break;
				}

				// 型の次は、ブランクか、[であること
				if( !(**pData == ' '  || 
					  **pData == '\t' ||
					  **pData == '\r' ||
					  **pData == '\n' ||
					  **pData == '['  ) ) {
					m_strLastErrorString.Format(_T("invalid message type"));
					XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
					return false;
				}


				XRichData *pRichData = pObj->addChiled(dataType);
				if( pRichData == NULL ) {
					m_strLastErrorString.Format(_T("addChiled"));
					XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
					return false;
				}
				if( innerParseSmlText(pData, data_size, pRichData) == false ) {
					return false;
				}
				if( moveNextValue(pData, data_size) == false ) {
					return false;
				}
				if( **pData == '>') {
					if( movePointer(pData, data_size, 1) == false ) {
						m_strLastErrorString.Format(_T("abnormal terminate."));
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( movePointerUnSPACE(pData, data_size) == false ) {
						m_strLastErrorString.Format(_T("abnormal terminate."));
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( moveNextValue(pData, data_size) == false ) {
						return false;
					}
					break;
				} else
				if( **pData != '<') {
					return false;
				}
			}
		}
		break;
	case XDATA_TYPE_BOOL:
	case XDATA_TYPE_U1:
	case XDATA_TYPE_U2:
	case XDATA_TYPE_U4:
	case XDATA_TYPE_U8:
	case XDATA_TYPE_I1:
	case XDATA_TYPE_I2:
	case XDATA_TYPE_I4:
	case XDATA_TYPE_I8:
	case XDATA_TYPE_F4:
	case XDATA_TYPE_F8:
		if( parseDigitOrBollFromString(pData, data_size, pObj) == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_B:
		if( parseBinaryFromString(pData, data_size, pObj) == false ) {
			return false;
		}
		break;
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
	case XDATA_TYPE_UTF16:
		if( parseStringFromString(pData, data_size, pObj) == false ) {
			return false;
		}
		break;
	default:
		_XASSERT(0);
		break;
	}


	if( 1 <= nLengthByte && nLengthByte <= 3 ) {
		switch( pObj->getType() ) {
		case XDATA_TYPE_L:
			if( nLengthByte != XData::getLengthByte(pObj->getType(), pObj->getChiledCount() ) ){
				pObj->setLengthByte( nLengthByte );
			}
			break;
		default:
			if( nLengthByte != XData::getLengthByte(pObj->getType(), pObj->getDataCount() ) ){
				pObj->setLengthByte( nLengthByte );
			}
			break;
		}
	}

	return true;
XEXCEPT_EXIT
}


bool XSecsTextParser::parseBinaryFromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData)
{
XEXCEPT_ENTRY
	TCHAR*		pStart;
	TCHAR bEnd = 0;
	unsigned char c = 0;
	unsigned char t = 0;
	unsigned char j = 0;
	std::vector<unsigned char>   binArray;


	if( movePointerUnSPACE(pData, data_size) == false ) {
		m_strLastErrorString.Format(_T("data size error"));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
		return false;
	}



	while( bEnd != '>' ) {
		pStart = *pData;
		if( movePointer(pData, data_size, _T(" \r\n\t>")) == false ) {
			m_strLastErrorString.Format(_T("not found endtag"));
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
			return false;
		}
		bEnd = **pData;
		**pData = 0;
		if( movePointer(pData, data_size, 1) == false ) {
			m_strLastErrorString.Format(_T("data size error"));
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
			return false;
		}

		if( movePointerUnSPACE(pData, data_size) == false ) {
			m_strLastErrorString.Format(_T("data size error"));
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
			return false;
		}

		while( *pStart != 0 ) {
			if( XStrBuff::StrLen(pStart) == 1 ) {
				if( '0' <= *pStart && *pStart <= '9' ) {
					c = *pStart - '0';
				} else 
				if( 'a' <= *pStart && *pStart <= 'f' ) {
					c = *pStart - 'a';
					c += 10;
				} else 
				if( 'A' <= *pStart && *pStart <= 'F' ) {
					c = *pStart - 'A';
					c += 10;
				} else {
					m_strLastErrorString.Format(_T("Invalid S2_B Value %c "), *pStart);
					XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
					return false;
				}
				binArray.push_back(c);
				pStart++;
			} else
			if( XStrBuff::StrLen(pStart) >= 2 ) {

				int base = 16;
				c = t = j = 0;
				if( XStrBuff::StrNCmp(pStart, _T("0x"), 2) == 0 ) { 
					pStart += 2;
				} else if( XStrBuff::StrNCmp(pStart, _T("0X"), 2) == 0 ) { 
					pStart += 2;
				} else
				if( XStrBuff::StrNCmp(pStart, _T("0d"), 2) == 0 ) { 
					pStart += 2;
					base = 10;
				} else if( XStrBuff::StrNCmp(pStart, _T("0D"), 2) == 0 ) { 
					pStart += 2;
					base = 10;
				}

				if( base == 16 ) {
					while( j <= 1 ) {
						if( j == 1 ) {
							c <<= 4;
						}
						j++;
						if( '0' <= *pStart && *pStart <= '9' ) {
							t = *pStart - '0';
						} else 
						if( 'a' <= *pStart && *pStart <= 'f' ) {
							t = *pStart - 'a';
							t += 10;
						} else 
						if( 'A' <= *pStart && *pStart <= 'F' ) {
							t = *pStart - 'A';
							t += 10;
						} else {
							m_strLastErrorString.Format(_T("Invalid S2_B Value %c "), *pStart);
							XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
							return false;
						}
						c |= t;
						pStart++;
					}
				} else 
				if( base == 10 ) {
					TCHAR *pEnd = NULL;
					c = XStrBuff::strtol(pStart, &pEnd, 10);
					if( pEnd == NULL ) {
						m_strLastErrorString.Format(_T("Invalid S2_B Value %s "), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					pStart = pEnd;
				}


				binArray.push_back(c);
			}
		}
	}
	if( binArray.size() != 0 ) {
		unsigned char *pdat = new unsigned char[binArray.size()];
		for(int i = 0; i < binArray.size(); i++ ) {
			pdat[i] = binArray[i];
		}
		pNewData->putValue(pdat, binArray.size());

		delete []pdat;
	} else {
		pNewData->putValue((unsigned char*)0, 0);
	}

	if( movePointerUnSPACE(pData, data_size) == false ) {
		m_strLastErrorString.Format(_T("data size error"));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
		return false;
	}
	return true;
XEXCEPT_EXIT
}

bool XSecsTextParser::parseDigitFromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData)
{
XEXCEPT_ENTRY
	return this->parseDigitOrBollFromString(pData, data_size, pNewData);
XEXCEPT_EXIT
}

bool XSecsTextParser::parseBoolromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData)
{
XEXCEPT_ENTRY
	return this->parseDigitOrBollFromString(pData, data_size, pNewData);
XEXCEPT_EXIT
}

bool XSecsTextParser::parseDigitOrBollFromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData)
{
XEXCEPT_ENTRY
	TCHAR*		pStart;
	TCHAR*		pEnd = NULL;
	int			ibase = 10;		// 数値の基底値


	if( moveNextValue(pData, data_size) == false ) {
		m_strLastErrorString.Format(_T("data size error"));
		XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
		return false;
	}

	char bEnd = 0;
	std::vector<XD_UINT1>	bin1Array;
	std::vector<XD_UINT2>   bin2Array;
	std::vector<XD_UINT4>   bin4Array;
	std::vector<XD_UINT8>   bin8Array;
	std::vector<XD_FLOAT4>	binFloatArray;
	std::vector<XD_FLOAT8>	binDoubleArray;
	std::vector<bool>		binBoolArray;

	while( bEnd != '>' ) {
		pStart = *pData;
		if( movePointer(pData, data_size, _T(" \r\n\t>")) == false ) {
			m_strLastErrorString.Format(_T("not found endtag"));
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
			return false;
		}
		bEnd = **pData;
		**pData = 0;
		if( movePointer(pData, data_size, 1) == false ) {
			m_strLastErrorString.Format(_T("data size error"));
			XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
			return false;
		}
		if( XStrBuff::StrLen(pStart) != 0 ) {

			pEnd = pStart;

			// 基底値を調べる
			switch( pNewData->getType() ) {
			case XDATA_TYPE_I1:	case XDATA_TYPE_I2:	case XDATA_TYPE_I4:	case XDATA_TYPE_I8:
			case XDATA_TYPE_U1:	case XDATA_TYPE_U2:	case XDATA_TYPE_U4:	case XDATA_TYPE_U8:
			case XDATA_TYPE_UX:	case XDATA_TYPE_IX:
				{
					if(XStrBuff::StrLen(pStart) >= 3 ) {
						if( XStrBuff::StrNCmp(pStart, _T("0x"), 2) == 0 ||
							XStrBuff::StrNCmp(pStart, _T("0X"), 2) == 0) {
							// 16進数表記
							ibase = 16;
							pStart += 2;
							pEnd = pStart;
						}
					} else {
						ibase = 10;
					}
				}
				break;
			}

			switch( pNewData->getType() ) {
			case XDATA_TYPE_I1:	case XDATA_TYPE_I2:	case XDATA_TYPE_I4:	case XDATA_TYPE_I8: case XDATA_TYPE_IX:
				// 文字列が数値であるか検査します。
				if( ibase == 10 && *pEnd == '-' ) {
					pEnd++;
				}
			case XDATA_TYPE_U1:	case XDATA_TYPE_U2:	case XDATA_TYPE_U4:	case XDATA_TYPE_U8: case XDATA_TYPE_UX:
				{
					// 文字列が数値であるか検査します。
					while( *pEnd != NULL ) {
						if( !( '0' <= *pEnd && *pEnd <= '9' ) ) {
							if( ibase == 16 ) {
								if( !( 'a' <= *pEnd && *pEnd <= 'f' || 
									   'A' <= *pEnd && *pEnd <= 'F') ) {
									m_strLastErrorString.Format(_T("invalid digit value. '%c'"), *pEnd);
									XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
									return false;
								}
							} else {
								m_strLastErrorString.Format(_T("invalid digit value. '%c'"), *pEnd);
								XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
								return false;
							}
						}
						pEnd++;
					}
				}
				break;
			}



			switch( pNewData->getType() ) {
			case XDATA_TYPE_BOOL:
				{
					bool val = false;
					if( XStrBuff::StrICmp(pStart, _T("true")) == 0	||
						XStrBuff::StrICmp(pStart, _T("1")) == 0		||
						XStrBuff::StrICmp(pStart, _T("0x01")) == 0) {
						val = true;
					} else
					if( !(XStrBuff::StrICmp(pStart, _T("false")) == 0	||
						  XStrBuff::StrICmp(pStart, _T("0")) == 0		||
						XStrBuff::StrICmp(pStart, _T("0x00")) == 0 )) {
						m_strLastErrorString.Format(_T("invalid value. '%s'"), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					binBoolArray.push_back(val);
				}
				break;
			case XDATA_TYPE_U1:
			case XDATA_TYPE_UX:
				{
					errno = 0;
					unsigned long val = XStrBuff::strtoul(pStart, &pEnd, ibase);
					if( errno == ERANGE ) {
						m_strLastErrorString.Format(_T("%s. '%s'"), (const TCHAR*)XExcept::getSrdErrorStr(errno), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( val > 0xFF ) {
						m_strLastErrorString.Format(_T("overfllow on U1. '%s%s'"), 
							ibase ==16 ? _T("0x") : _T(""), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					unsigned char dd = (unsigned char)val;
					bin1Array.push_back(dd);
				}
				break;
			case XDATA_TYPE_I1:
			case XDATA_TYPE_IX:
				{
					errno = 0;
					long val = XStrBuff::strtol(pStart, &pEnd, ibase);
					if( errno == ERANGE ) {
						m_strLastErrorString.Format(_T("%s. '%s'"), (const TCHAR*)XExcept::getSrdErrorStr(errno), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( ibase == 16 && ( 0xFFFFFF00 & val ) != 0 ) {
						m_strLastErrorString.Format(_T("overfllow on I1. '%s%s'"), 
							ibase ==16 ? _T("0x") : _T(""), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( ibase == 10 && val > 127 ) {
						m_strLastErrorString.Format(_T("overfllow on I1. '%s%s'"), 
							ibase ==16 ? _T("0x") : _T(""), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					} 
					if( val < -128 ) {
						m_strLastErrorString.Format(_T("underfllow on I1. '%s%s'"), 
							ibase ==16 ? _T("0x") : _T(""), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}


					char dd = 0;
					if( ibase == 10  ) {
						dd = (char)val;
					} else {
						::memcpy(&dd, &val, sizeof(char));
					}
					bin1Array.push_back((unsigned char)dd);
				}
				break;
			case XDATA_TYPE_U2:
				{
					errno = 0;
					unsigned long val = XStrBuff::strtoul(pStart, &pEnd, ibase);
					if( errno == ERANGE ) {
						m_strLastErrorString.Format(_T("%s. '%s'"), (const TCHAR*)XExcept::getSrdErrorStr(errno), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( val > 0xFFFF ) {
						m_strLastErrorString.Format(_T("overfllow on U2. '%s%s'"), 
							ibase ==16 ? _T("0x") : _T(""), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					unsigned short dd = (unsigned short)val;
					bin2Array.push_back(dd);
				}
				break;
			case XDATA_TYPE_I2:
				{
					errno = 0;
					long val = XStrBuff::strtol(pStart, &pEnd, ibase);
					if( errno == ERANGE ) {
						m_strLastErrorString.Format(_T("%s. '%s'"), (const TCHAR*)XExcept::getSrdErrorStr(errno), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( ibase == 16 && ( 0xFFFF0000 & val ) != 0 ) {
						m_strLastErrorString.Format(_T("overfllow on I2. '%s%s'"), 
							ibase ==16 ? _T("0x") : _T(""), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					if( ibase == 10 && val > 32767 ) {
						m_strLastErrorString.Format(_T("overfllow on I2. '%s%s'"), 
							ibase ==16 ? _T("0x") : _T(""), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					} 
					if( val < -32768 ) {
						m_strLastErrorString.Format(_T("underfllow on I2. '%s%s'"), 
							ibase ==16 ? _T("0x") : _T(""), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}


					short dd = 0;
					if( ibase == 10  ) {
						dd = (short)val;
					} else {
						::memcpy(&dd, &val, sizeof(short));
					}
					bin2Array.push_back((unsigned short)dd);
				}
				break;
			case XDATA_TYPE_U4:
				{
					errno = 0;
					unsigned long val = XStrBuff::strtoul(pStart, &pEnd, ibase);
					if( errno == ERANGE ) {
						m_strLastErrorString.Format(_T("%s. '%s'"), (const TCHAR*)XExcept::getSrdErrorStr(errno), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					bin4Array.push_back(val);
				}
				break;
			case XDATA_TYPE_I4:
				{
					errno = 0;
					long dd = 0;
					long val = 0;//XStrBuff::strtol(pStart, &pEnd, ibase);
					if( ibase == 16 ) {
						val = XStrBuff::strtoul(pStart, &pEnd, ibase);
						::memcpy(&dd, &val, sizeof(long));
					} else {
						val = XStrBuff::strtol(pStart, &pEnd, ibase);
						::memcpy(&dd, &val, sizeof(long));
					}
					if( errno == ERANGE ) {
						m_strLastErrorString.Format(_T("%s. '%s'"), (const TCHAR*)XExcept::getSrdErrorStr(errno), pStart);
						XEXCEPT_THROW_STR(XExcept_SECS, (LPCTSTR)m_strLastErrorString, m_ulLine, 0);
						return false;
					}
					bin4Array.push_back((unsigned long)dd);
				}
				break;
			case XDATA_TYPE_U8:
				{
					XD_UINT8 dd = XStrBuff::strtoull(pStart, &pEnd, ibase);
					bin8Array.push_back(dd);
				}
				break;
			case XDATA_TYPE_I8:
				{
					XD_INT8 dd = XStrBuff::strtoll(pStart, &pEnd, ibase);
					bin8Array.push_back((XD_INT8)dd);
				}
				break;
			case XDATA_TYPE_F4:
			case XDATA_TYPE_FX:
				{
					float dd = (float)XStrBuff::strtod(pStart, &pEnd);
					binFloatArray.push_back(dd);
				}
				break;
			case XDATA_TYPE_F8:
				{
					double dd = XStrBuff::strtod(pStart, &pEnd);
					binDoubleArray.push_back(dd);
				}
				break;
			default:
				_XASSERT(0);
				break;
			}
		}
	}


	switch( pNewData->getType() ) {
	case XDATA_TYPE_BOOL:
		if( binBoolArray.size() != 0 ) {
			bool *pdat = new bool[binBoolArray.size()];
			for(int i = 0; i < binBoolArray.size(); i++ ) {
				pdat[i] = binBoolArray[i];
			}
			pNewData->putValue(pdat, binBoolArray.size());

			delete []pdat;
		} else {
			pNewData->putValue((bool*)0, 0);
		}
		break;
	case XDATA_TYPE_U1:
	case XDATA_TYPE_UX:
		if( bin1Array.size() != 0 ) {
			XD_UINT1 *pdat = new XD_UINT1[bin1Array.size()];
			for(int i = 0; i < bin1Array.size(); i++ ) {
				pdat[i] = bin1Array[i];
			}
			pNewData->putValue(pdat, bin1Array.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_UINT1*)0, 0);
		}
		break;
	case XDATA_TYPE_I1:
	case XDATA_TYPE_IX:
		if( bin1Array.size() != 0 ) {
			XD_INT1 *pdat = new XD_INT1[bin1Array.size()];
			for(int i = 0; i < bin1Array.size(); i++ ) {
				pdat[i] = bin1Array[i];
			}
			pNewData->putValue(pdat, bin1Array.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_INT1*)0, 0);
		}
		break;
	case XDATA_TYPE_U2:
		if( bin2Array.size() != 0 ) {
			XD_UINT2 *pdat = new XD_UINT2[bin2Array.size()];
			for(int i = 0; i < bin2Array.size(); i++ ) {
				pdat[i] = bin2Array[i];
			}
			pNewData->putValue(pdat, bin2Array.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_UINT2*)0, 0);
		}
		break;
	case XDATA_TYPE_I2:
		if( bin2Array.size() != 0 ) {
			XD_INT2 *pdat = new XD_INT2[bin2Array.size()];
			for(int i = 0; i < bin2Array.size(); i++ ) {
				pdat[i] = bin2Array[i];
			}
			pNewData->putValue(pdat, bin2Array.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_INT2*)0, 0);
		}
		break;
	case XDATA_TYPE_U4:
		if( bin4Array.size() != 0 ) {
			XD_UINT4 *pdat = new XD_UINT4[bin4Array.size()];
			for(int i = 0; i < bin4Array.size(); i++ ) {
				pdat[i] = bin4Array[i];
			}
			pNewData->putValue(pdat, bin4Array.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_UINT4*)0, 0);
		}
		break;
	case XDATA_TYPE_I4:
		if( bin4Array.size() != 0 ) {
			XD_INT4 *pdat = new XD_INT4[bin4Array.size()];
			for(int i = 0; i < bin4Array.size(); i++ ) {
				pdat[i] = bin4Array[i];
			}
			pNewData->putValue(pdat, bin4Array.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_INT4*)0, 0);
		}
		break;
	case XDATA_TYPE_U8:
		if( bin8Array.size() != 0 ) {
			XD_UINT8 *pdat = new XD_UINT8[bin8Array.size()];
			for(int i = 0; i < bin8Array.size(); i++ ) {
				pdat[i] = bin8Array[i];
			}
			pNewData->putValue(pdat, bin8Array.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_UINT8*)0, 0);
		}
		break;
	case XDATA_TYPE_I8:
		if( bin8Array.size() != 0 ) {
			XD_INT8 *pdat = new XD_INT8[bin8Array.size()];
			for(int i = 0; i < bin8Array.size(); i++ ) {
				pdat[i] = bin8Array[i];
			}
			pNewData->putValue(pdat, bin8Array.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_INT8*)0, 0);
		}
		break;
	case XDATA_TYPE_F4:
	case XDATA_TYPE_FX:
		if( binFloatArray.size() != 0 ) {
			XD_FLOAT4 *pdat = new XD_FLOAT4[binFloatArray.size()];
			for(int i = 0; i < binFloatArray.size(); i++ ) {
				pdat[i] = binFloatArray[i];
			}
			pNewData->putValue(pdat, binFloatArray.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_FLOAT4*)0, 0);
		}
		break;
	case XDATA_TYPE_F8:
		if( binDoubleArray.size() != 0 ) {
			XD_FLOAT8 *pdat = new XD_FLOAT8[binDoubleArray.size()];
			for(int i = 0; i < binDoubleArray.size(); i++ ) {
				pdat[i] = binDoubleArray[i];
			}
			pNewData->putValue(pdat, binDoubleArray.size());

			delete []pdat;
		} else {
			pNewData->putValue((XD_FLOAT8*)0, 0);
		}
		break;
	default:
		_XASSERT(0);
		break;
	}

	if( movePointerUnSPACE(pData, data_size) == false ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}

bool XSecsTextParser::parseStringFromString(TCHAR* *pData, size_t &data_size, XRichData *pNewData)
{
XEXCEPT_ENTRY
	TCHAR*		pStart = NULL;
	bool bDblQuot = true;
	bool bMultiQuot = false;


	if( moveNextValue(pData, data_size) == false ) {
		return false;
	}

	if( **pData != '\"') {
		bDblQuot = false;
	} else {
		bMultiQuot = true;
		if( movePointer(pData, data_size, 1) == false ) {
			return false;
		}
	}
	// 文字列の終端は、"> が続くものとする
	//   ">\ の各文字は、１６進表記されているものとする
	pStart = *pData;

	while( **pData != 0 ) {
		if( **pData == '\"' ) {
			bMultiQuot = !bMultiQuot;
		}
		if( bDblQuot == true ) {
			if( bMultiQuot == false && **pData == '\"' && *((*pData)+1) == '>' ) {
				break;
			}
		} else {
			if( bMultiQuot == false && **pData == '>' ) {
				break;
			}
		}
		if( movePointer(pData, data_size, 1) == false ) {
			return false;
		}
	}
	if( **pData == 0 ) {
		return false;
	}
	**pData = 0;
	if( bDblQuot == true ) {
		if( movePointer(pData, data_size, 1) == false ) {
			return false;
		}
	}
	size_t nSize = 0;
	if( convertRealSMLString2RealString(pStart, nSize) == false ) {
		return false;
	}
	if( pNewData->getType() == XDATA_TYPE_UTF16 ) {
		pNewData->putValueStringUTF16(XStrBuff(pStart));	
	} else {
		if( XStrBuff::StrLen(pStart) > 8 && XStrBuff::StrNCmp(pStart, _T("${FILE:"), 7) == 0 && pStart[ XStrBuff::StrLen(pStart)-1 ] == '}' )
		{
			/*
			${FILE:ファイルのパス}の場合は、ファイルからデータを取得します。
			*/	
			do
			{
				char* pReadData = NULL;
				struct _stat fsbuf;
				pStart[ XStrBuff::StrLen(pStart)-1 ] = 0;
				pStart += 7;
				FILE* fp = NULL;
				fp = _tfopen(pStart, _T("r"));
				if(fp ==  NULL)
				{
					break;
				}
				if(  _fstat( _fileno(fp), &fsbuf ) != 0 )
				{
					break;
				}
				pReadData = new char[fsbuf.st_size + 1];
				::memset(pReadData, 0,  fsbuf.st_size + 1);
				::fread(pReadData, fsbuf.st_size, 1, fp);
				fclose(fp);			
				pNewData->putValueString(pReadData);
				delete[] pReadData;
			}while(false);
		} else {
			pNewData->putValueString(pStart);
		}
	}

	if( bDblQuot == false ) {
		**pData = '>';
	}

	if( movePointer(pData, data_size, _T(">")) == false ) {
		return false;
	}

	if( movePointer(pData, data_size, 1) == false ) {
		return false;
	}
	if( moveNextValue(pData, data_size) == false ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}

bool XSecsTextParser::parseFromString(const TCHAR* pData, size_t datalen, XRichData *pNewData)
{
XEXCEPT_ENTRY

	XStrBuff buff;
	TCHAR* pBuff;
	unsigned int len = datalen;
	XSecsTextParser parser;

	pBuff = (TCHAR*)buff.Alloc( (datalen + 10) * sizeof(TCHAR) );
	buff.MemSet();
	switch( pNewData->getType() ) {
	case XDATA_TYPE_A:case XDATA_TYPE_J:case XDATA_TYPE_UTF16:
		XStrBuff::printf(pBuff, buff.getCountOfBuff(), _T("\"%s\">"), pData);
		break;
	default:
		::memcpy(pBuff, pData, datalen*sizeof(TCHAR));
		break;
	}





	switch( pNewData->getType() ) {
	case XDATA_TYPE_L:case XDATA_TYPE_END:
		break;
	case XDATA_TYPE_A:case XDATA_TYPE_J:case XDATA_TYPE_UTF16:
		len = XStrBuff::StrLen(pBuff);
		parser.parseStringFromString(&pBuff, len, pNewData);
		break;
	case XDATA_TYPE_B:
		pBuff[datalen] = '>';
		len = datalen + 1;
		parser.parseBinaryFromString(&pBuff, len, pNewData);
		break;
	case XDATA_TYPE_BOOL:
		pBuff[datalen] = '>';
		len = datalen + 1;
		parser.parseBoolromString(&pBuff, len, pNewData);
		break;
	default:
		pBuff[datalen] = '>';
		len = datalen + 1;
		parser.parseDigitFromString(&pBuff, len, pNewData);
		break;
	}
	return true;
XEXCEPT_EXIT
}


bool XSecsTextParser::convertRealSMLString2RealString(TCHAR *pString, size_t &nSize)
{
XEXCEPT_ENTRY
	nSize = 0;
	bool bFindDbQ = false;
	while( *pString != 0 ) {

		// 改行のスキップ
		if( *pString == '\"' ) {
			// ダブルクォートの次が改行の場合は、次のダブルクォートまでスキップ
			if( (*(pString+1) == '\r' || *(pString+1) == '\n') && bFindDbQ == false   ) {
				TCHAR *pMv = pString;
				pMv++;
				if( movePointerUnSPACE(&pMv, nSize) == false ) {
					return false;
				}
				if( *pMv != '\"') {
					return false;
				}
				pMv++;
				::memmove(pString, pMv, XStrBuff::StrLen(pMv)+1);
			} else {
				bFindDbQ = !bFindDbQ;
			}

		}


		// 16進数表記の解析
		if( ::XStrBuff::StrNCmp(pString, _T("\\0x0"), 4) == 0 ) {
			TCHAR c = 0;
			TCHAR t = 0;
			TCHAR *pWk = pString;
			pWk += 4;
			if( XStrBuff::StrLen(pWk) <  2) {
				return false;
			}
			

			if( '0' <= pWk[0] && pWk[0] <= '9' ) {
				t = pWk[0] - '0';
			} else 
			if( 'a' <= pWk[0] && pWk[0] <= 'f' ) {
				t = pWk[0] - 'a';
				t += 10;
			} else 
			if( 'A' <= pWk[0] && pWk[0] <= 'F' ) {
				t = pWk[0] - 'A';
				t += 10;
			} else {
				return false;
			}
			c |= t;
			c <<= 4;
			if( '0' <= pWk[1] && pWk[1] <= '9' ) {
				t = pWk[1] - '0';
			} else 
			if( 'a' <= pWk[1] && pWk[1] <= 'f' ) {
				t = pWk[1] - 'a';
				t += 10;
			} else 
			if( 'A' <= pWk[1] && pWk[1] <= 'F' ) {
				t = pWk[1] - 'A';
				t += 10;
			} else {
				return false;
			}
			c |= t;

			::memmove(pString, &pString[5], XStrBuff::StrLen(&pString[5])+1);
			*pString = c;
		}
		pString++;
		nSize++;
	}
	return true;
XEXCEPT_EXIT
}
bool XSecsTextParser::movePointer(TCHAR** p, size_t &size, size_t mv_size)
{
XEXCEPT_ENTRY
	if( size < mv_size ) {
		m_strLastErrorString.Format(_T("Not Terminate String"));
		return false;
	}
	while( mv_size != 0) {
		(*p) += 1;
		size -= 1;
		mv_size --;

		if( (**p) == '\n' ) {
			m_ulLine++;
		}
	}
	return true;
XEXCEPT_EXIT
}
bool XSecsTextParser::moveNextLine(TCHAR** p, size_t &size)
{
XEXCEPT_ENTRY
	while( size != 0 ) {

		if( (**p) == '\n' ) {
			m_ulLine++;
			(*p)++;
			size--;
			return true;
		}
		(*p)++;
		size--;
	}
	m_strLastErrorString.Format(_T("Not Terminate String"));
	return false;
XEXCEPT_EXIT
}
bool XSecsTextParser::moveNextValue(TCHAR** p, size_t &size)
{
XEXCEPT_ENTRY
	while( size != 0 ) {
		if( **p == '\n' ) { m_ulLine++; }
		if(!( (**p) == ' ' || (**p) == '\t'||
			  (**p) == '\r' || (**p) == '\n') ) 
		{

			// コメントの場合はコメントをスキップする。
			if( (**p) == '/' && *((*p)+1) == '*' ) {
				while( size != 0 ) {
					if( (**p) == '*' && *((*p)+1) == '/' ) {
						(*p)++;(*p)++;
						size--;size--;
						break;
					}
					(*p)++;
					size--;
				}
				if(size == 0 ) {
					m_strLastErrorString.Format(_T("Not Terminate String"));
					return false;
				}
			} else {
				return true;
			}
		}
		(*p)++;
		size--;
	}
	m_strLastErrorString.Format(_T("Not Terminate String"));
	return false;
XEXCEPT_EXIT
}
bool XSecsTextParser::movePointer(TCHAR* *ppData, size_t &size, const TCHAR* lpszC)
{
XEXCEPT_ENTRY
	const TCHAR* lpszStart = *ppData;
	while( size != 0 ) {
		if( **ppData == '\n' ) { m_ulLine++; }
		const TCHAR* lpsz = lpszC;
		while( *lpsz != 0 ) {
			if( **ppData == *lpsz ) {
				return true;
			}
			lpsz++;
		}
		(*ppData)++;
		size--;
	}
	m_strLastErrorString.Format(_T("Not Terminate String"));
	return false;
XEXCEPT_EXIT
}
bool XSecsTextParser::movePointerUnSPACE(TCHAR* *ppData, size_t &size)
{
XEXCEPT_ENTRY
	const TCHAR* lpszStart = *ppData;
	while(1) {
		while(**ppData == ' ' || **ppData == '\t' || 
			  **ppData == '\r' || **ppData == '\n') {
			if(size == 0) {
				m_strLastErrorString.Format(_T("Not Terminate String"));
				return false;
			}
			if( **ppData == '\n' ) { m_ulLine++; }
			(*ppData)++;
			size--;
		}
		// コメント部でストップした場合の対処
		if( **ppData == '!' ) {
			// 改行が見付かるまでスキップ
			while(size != 0 ) {
				if( **ppData == '\r' || **ppData == '\n' ) {
					break;
				}
				(*ppData)++;
				size--;
			}
		} else {
			break;
		}
	}
	return true;
XEXCEPT_EXIT
}


