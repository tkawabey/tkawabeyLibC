#include <string.h>
#ifdef WIN32
  #include <mbstring.h>
#endif
#include "xlib/xstrcordeconv.h"

//
//	XStrBuffCodeConv
//
XStrBuffCodeConv::XStrBuffCodeConv()
{
	CommonConstruct();
}
XStrBuffCodeConv::XStrBuffCodeConv(const char* p)
	:XStrBuff(p)
{
	CommonConstruct();
}
XStrBuffCodeConv::XStrBuffCodeConv(const wchar_t* p)
	:XStrBuff(p)
{
	CommonConstruct();
}

void XStrBuffCodeConv::CommonConstruct()
{
	m_nKanjiMode   = CONV_ASCII;
	m_cKanjiFst    = 0;
	m_cKanjiPrev   = 0;
	m_nEscNum      = 0;
	m_cEscCode[0]  = 0;
	m_cEscCode[1]  = 0;
	m_nKanaConv    = true;

	m_bConvCrLf = true;
	m_cTermCode = 0;
}
bool XStrBuffCodeConv::Convert(CodeType incode , CodeType outcode, unsigned long dwSrcSize, XBuff *pOutBuf, unsigned long *pdwOutSize)
{
	XBuff  buffOut;
	TCHAR* pBuff;
	TCHAR* pNew = NULL;
	unsigned long  dwOutSize;
	bool   bNullOut;

	if(dwSrcSize == -1) {
		dwSrcSize = this->GetBuffSize();
	}

	if(pOutBuf == NULL) {
		pBuff = (TCHAR*)buffOut.Alloc(this->GetBuffSize() * 2);
		pOutBuf = &buffOut;
		bNullOut = true;
	} else {
		pBuff = (TCHAR*)*pOutBuf;
		bNullOut = false;
	}
	
	if(pBuff == NULL) {
		return false;
	}

	if(incode ==  SJIS) {
		if(outcode == EUC) {
			dwOutSize = ConvSJIStoEUC(pOutBuf, dwSrcSize);
		} else
		if(outcode == EUC) {
			dwOutSize = ConvSJIStoJIS(pOutBuf, dwSrcSize);
		} else {
			return false;
		}
	} else
	if(incode ==  EUC) {
		if(outcode == SJIS) {
			dwOutSize = ConvEUCtoSJIS(pOutBuf, dwSrcSize); 
		} else {
			return false;
		}
	} else
	if(incode ==  JIS) {
		if(outcode == SJIS) {
			dwOutSize = ConvJIStoSJIS(pOutBuf, dwSrcSize);
		} else {
			return false;
		}
	} else {
		return false;
	}

	if( dwOutSize > 0 ) {
		if( bNullOut == true ) {
			if(dwOutSize != this->GetBuffSize()) {
				pNew = (TCHAR*)this->Alloc(dwOutSize);
			} else {
				pNew = (TCHAR*)*this;
			}

			if(pNew == NULL) {
				return false;
			}
			::memcpy(pNew, (void*)*pOutBuf, dwOutSize);
		}

		if(pdwOutSize != NULL) {
			*pdwOutSize = dwOutSize;
		}
		return true;
	} else {
		return false;
	}
}


bool XStrBuffCodeConv::convertUnicodeToUTF8(const wchar_t* pszSrc, size_t srcLen, XStrBuff& dst, bool bCESU8 )
{
    size_t i, c;
    char* pszUTF8;
    unsigned char *pb;
    unsigned int wch;

    c = 0;
    for(i = 0; i < srcLen; i++) {
        if (pszSrc[i] <= 0x007F) {
            c += 1;
        } else 
		if (0x0080 <= pszSrc[i] && pszSrc[i] <= 0x07FF) {
            c += 2;
		} else 
		if (0x0800 <= pszSrc[i])  {
            if (!bCESU8 && i + 1 < srcLen &&
                0xD800 <= pszSrc[i] && pszSrc[i] <= 0xDBFF &&
                0xDC00 <= pszSrc[i + 1] && pszSrc[i + 1] <= 0xDFFF)
            {
                c += 4;
                i++;
            }
            else {
                c += 3;
			}
        }
    }

	if( (c+1) > dst.GetBuffSize() ) {
		pszUTF8 = (char*)dst.Alloc(c+1);
	} else {
		pszUTF8 = (char*)(void*)dst;
	}
    c = 0;
    for(i = 0; i < srcLen; i++) {
        pb = (unsigned char *)&pszSrc[i];
        if (pszSrc[i] <= 0x007F) {
            pszUTF8[c] = pb[0];
            c++;
        } else 
		if (0x0080 <= pszSrc[i] && pszSrc[i] <= 0x07FF) {
            pszUTF8[c] = (unsigned char)(((pb[0] & 0xC0) >> 6) | 
                                ((pb[1] & 0x07) << 2) | 0xC0);
            pszUTF8[c + 1] = (unsigned char)((pb[0] & 0x3F) | 0x80);
            c++;
            c++;
        } else 
		if (0x0800 <= pszSrc[i]) {
            if (!bCESU8 && i + 1 < srcLen &&
                0xD800 <= pszSrc[i] && pszSrc[i] <= 0xDBFF &&
                0xDC00 <= pszSrc[i + 1] && pszSrc[i + 1] <= 0xDFFF)
            {
                wch = (pszSrc[i] - 0xD800) * 0x400 + 
                      (pszSrc[i + 1] - 0xDC00) + 0x10000;
                pszUTF8[c] =     (unsigned char)(((wch & 0x001C0000) >> 18) | 0xF0);
                pszUTF8[c + 1] = (unsigned char)(((wch & 0x0003F000) >> 12) | 0x80);
                pszUTF8[c + 2] = (unsigned char)(((wch & 0x00000FC0) >> 6) | 0x80);
                pszUTF8[c + 3] = (unsigned char)( (wch & 0x0000003F) | 0x80);
                c += 4;
                i++;
            } else {
                pszUTF8[c] = (unsigned char)(((pb[1] & 0xF0) >> 4) | 0xE0);
                pszUTF8[c + 1] = (unsigned char)(((pb[0] & 0xC0) >> 6) | 
                                        ((pb[1] & 0x0F) << 2) | 0x80);
                pszUTF8[c + 2] = (unsigned char)((pb[0] & 0x3F) | 0x80);
                c += 3;
            }
        }
    }
    pszUTF8[c] = 0;


    return true;
}


bool XStrBuffCodeConv::convertUTF8ToUnicode(const char* pszSrc, int srcLen, XBuff& dst, bool *pfCESU8 )
{
    wchar_t* pszWide;
	wchar_t*	pch;
    unsigned int wch, x;
    unsigned char b0, b1;
    const unsigned char *pb = (const unsigned char *)pszSrc;

    *pfCESU8 = false;


	if( ( (srcLen + 1)*sizeof(wchar_t) ) > dst.GetBuffSize() ) {
		pszWide = (wchar_t*)dst.Alloc((srcLen + 1)*sizeof(wchar_t));
	} else {
		pszWide = (wchar_t*)(unsigned short*)dst;
	}

	pch = pszWide;

    while(srcLen > 0)
    {
        if ((*pb & 0x80) == 0)
        {
            *(pch++) = (wchar_t)*pb;
            pb++;
            srcLen--;
        }
        else if (srcLen >= 2 && 
                 (pb[0] & 0xE0) == 0xC0 && 
                 (pb[1] & 0xC0) == 0x80)
        {
            b0 = (unsigned char)((pb[0] << 6) | (pb[1] & 0x3F));
            b1 = (unsigned char)((pb[0] & 0x1C) >> 2);
            *pch++ = MAKEWORD(b0, b1);
            pb++;
            pb++;
            srcLen--;
            srcLen--;
        }
        else if (srcLen >= 3 && 
                 (pb[0] & 0xF0) == 0xE0 && 
                 (pb[1] & 0xC0) == 0x80 &&
                 (pb[2] & 0xC0) == 0x80)
        {
            b0 = (unsigned char)((pb[2] & 0x3F) | ((int)(pb[1] & 0x03) << 6));
            b1 = (unsigned char)(((int)(pb[0] & 0x0F) << 4) | ((pb[1] & 0x3C) >> 2));
            *pch = MAKEWORD(b0, b1);
            if (0xD800 <= *pch && *pch <= 0xDBFF)
                *pfCESU8 = true;
            pch++;
            pb += 3;
            srcLen -= 3;
        }
        else if (srcLen >= 4 &&
                 (pb[0] & 0xF8) == 0xF0 && 
                 (pb[1] & 0xC0) == 0x80 &&
                 (pb[2] & 0xC0) == 0x80 &&
                 (pb[3] & 0xC0) == 0x80)
        {
            wch = ((UINT)(pb[0] & 0x03) << 18) |
                  ((UINT)(pb[1] & 0x3F) << 12) |
                  ((UINT)(pb[2] & 0x3F) << 6) |
                   (UINT)(pb[3] & 0x3F);
            x = wch - 0x10000;
            *pch++ = (WORD)(x / 0x400 + 0xD800);
            *pch++ = (WORD)(x % 0x400 + 0xDC00);
            pb += 4;
            srcLen -= 4;
        }
        else if (srcLen >= 5 &&
                 (pb[0] & 0xFC) == 0xF8 && 
                 (pb[1] & 0xC0) == 0x80 &&
                 (pb[2] & 0xC0) == 0x80 &&
                 (pb[3] & 0xC0) == 0x80 &&
                 (pb[4] & 0xC0) == 0x80)
        {
            *pch++ = '?';
            pb += 5;
            srcLen -= 5;
        }
        else if (srcLen >= 6 &&
                 (pb[0] & 0xFE) == 0xFC && 
                 (pb[1] & 0xC0) == 0x80 &&
                 (pb[2] & 0xC0) == 0x80 &&
                 (pb[3] & 0xC0) == 0x80 &&
                 (pb[4] & 0xC0) == 0x80 &&
                 (pb[5] & 0xC0) == 0x80)
        {
            *pch++ = '?';
            pb += 6;
            srcLen -= 6;
        }
        else
        {
            *pch++ = '?';
            pb++;
            srcLen--;
            while(srcLen > 0 && (*pb & 0xC0) == 0x80)
            {
                pb++;
                srcLen--;
            }
        }
    }
    *pch = 0;
    return true;
}



bool XStrBuffCodeConv::convertNativeToUTF8(const char* pszSrc, XStrBuff& dst, bool bCESU8 )
{
	XStrBuff str;
	str = pszSrc;
	return XStrBuffCodeConv::convertUnicodeToUTF8(str, str.GetLength(), dst, bCESU8 );
}


bool XStrBuffCodeConv::convertNativeToUTF8(const wchar_t* pszSrc, XStrBuff& dst, bool bCESU8 )
{
	return XStrBuffCodeConv::convertUnicodeToUTF8(pszSrc, ::wcslen(pszSrc), dst, bCESU8 );
}


bool XStrBuffCodeConv::convertUTF8ToNative(const char* pszSrc, XStrBuff& dst, bool *pfCESU )
{
	XBuff buff;

	if( XStrBuffCodeConv::convertUTF8ToUnicode(pszSrc, ::strlen(pszSrc), buff, pfCESU ) == false ) {
		return false;
	}

	dst = (const wchar_t*)(unsigned short*)buff;

	return true;
}





unsigned long XStrBuffCodeConv::ConvEUCtoSJIS(XBuff *pOutBuf, unsigned long dwSrcSize)
{
	char* pOut = (char*)*pOutBuf;
	char* pIn  = (char*)*this;
	unsigned int i;
	int          nKcode;

	for(i = 0; i < dwSrcSize; i++) {
		if( pOut >= pOutBuf->GetMaxBuffAddress() ) {
			break;
		}

/*
		// 改行コード処理
		if(m_bConvCrLf == true && *pIn == 0x0D)
		{
			if(m_cTermCode == 0x0D)
				*pOut++ = 0x0A;
			*pOut++ = 0x0D;
			m_cTermCode = *pIn++;
			continue;
		}
		else if(m_bConvCrLf == true)
		{
			if(*pIn == 0x0A)
			{
				if(m_cTermCode != 0x0D)
					*pOut++ = 0x0D;
			}
			else
			{
				if(m_cTermCode == 0x0D)
					*pOut++ = 0x0A;
			}
			m_cTermCode = 0;
		}
*/
		// VAX のエスケープシーケンスの場合
		if( *pIn == 0xffffff9b ){
			*pOut++ = *pIn++;
			continue;
		}
		
		if((*pIn & 0x80) != 0) {
			if(m_cKanjiFst == 0) {
				m_cKanjiFst = *pIn++;
			} else {
				if((unsigned char)m_cKanjiFst == (unsigned char)0x8E)	/* 半角カタカナ */
				{
					pOut = ConvEUCtoSJISkanaProc(*pIn++, pOut);
				} else {
					pOut = ConvEUCtoSJISkanaProc(0, pOut);
#ifdef WIN32
					nKcode = _mbcjistojms(((m_cKanjiFst & 0x7F) * 0x100) + (*pIn++ & 0x7F));
#else
					nKcode = *pIn++;
#endif /* WIN32 */
					*pOut++ = HIBYTE(nKcode);
					*pOut++ = LOBYTE(nKcode);
				}
				m_cKanjiFst = 0;
			}
		} else {
			pOut = ConvEUCtoSJISkanaProc(0, pOut);
			if(m_cKanjiFst != 0) {
				*pOut++ = m_cKanjiFst;
				m_cKanjiFst = 0;
			}
			*pOut++ = *pIn++;
		}
	}
	return (int)(pOut - (char*)*pOutBuf);
}
char *XStrBuffCodeConv::ConvEUCtoSJISkanaProc(char Dt, char *pOut)
{
	int          nKcode;
	int          nDaku;

	if(m_nKanaConv == false) {
		if(Dt != 0) {
			*pOut++ = Dt;
		}
	} else {
		if(m_cKanjiPrev != 0) {
			nDaku = AskDakuon(m_cKanjiPrev, Dt);
#ifdef WIN32
			nKcode = _mbcjistojms(HanKataToZen(m_cKanjiPrev)) + nDaku;
#else
			nKcode = m_cKanjiPrev;
#endif /* WIN32 */
			*pOut++ = HIBYTE(nKcode);
			*pOut++ = LOBYTE(nKcode);

			if(nDaku == 0) {
				m_cKanjiPrev = Dt;
			} else {
				m_cKanjiPrev = 0;
			}
		} else {
			m_cKanjiPrev = Dt;
		}
	}
	return(pOut);
}
unsigned long XStrBuffCodeConv::ConvJIStoSJIS(XBuff *pOutBuf, unsigned long dwSrcSize)
{
	char* pOut = (char*)*pOutBuf;
	char* pIn  = (char*)*this;
	unsigned int i;
	int          nKcode;


	for(i = 0; i < dwSrcSize; i++) {
		if( pOut >= pOutBuf->GetMaxBuffAddress() ) {
			break;
		}

		if(m_nEscNum == 0) {
			if(*pIn == 0x1B) {
				if(m_cKanjiFst != 0) {
					*pOut++ = m_cKanjiFst;
					m_cKanjiFst = 0;
				}
				pOut = ConvJIStoSJISkanaProc(0, pOut);

				m_cEscCode[m_nEscNum] = *pIn++;
				m_nEscNum++;
			} else {
				if(m_nKanjiMode == CONV_KANA) {
					if(m_cKanjiFst != 0) {
						*pOut++ = m_cKanjiFst;
						m_cKanjiFst = 0;
					}

					if((*pIn >= 0x21) && (*pIn <= 0x5F)) {
						pOut = ConvJIStoSJISkanaProc(*pIn++, pOut);
					} else {
						pOut = ConvJIStoSJISkanaProc(0, pOut);
						*pOut++ = *pIn++;
					}
				} else
				if(m_nKanjiMode == CONV_KANJI) {
					pOut = ConvJIStoSJISkanaProc(0, pOut);
					if((*pIn >= 0x21) && (*pIn <= 0x7E)) {
						if(m_cKanjiFst == 0) {
							m_cKanjiFst = *pIn++;
						} else {
#ifdef WIN32
							nKcode = _mbcjistojms((m_cKanjiFst * 0x100) + *pIn++);
#else
							nKcode = *pIn++;
#endif /* WIN32 */
							*pOut++ = HIBYTE(nKcode);
							*pOut++ = LOBYTE(nKcode);
							m_cKanjiFst = 0;
						}
					} else {
						if(m_cKanjiFst == 0) {
							*pOut++ = *pIn++;
						} else {
							*pOut++ = m_cKanjiFst;
							*pOut++ = *pIn++;
							m_cKanjiFst = 0;
						}
					}
				} else {
					pOut = ConvJIStoSJISkanaProc(0, pOut);
					*pOut++ = *pIn++;
				}
			}
		} else
		if(m_nEscNum == 1) {
			if((*pIn == '$') || (*pIn == '(')) {
				m_cEscCode[m_nEscNum] = *pIn++;
				m_nEscNum++;
			} else {
				*pOut++ = m_cEscCode[0];
				*pOut++ = *pIn++;
				m_nEscNum = 0;
			}
		} else
		if(m_nEscNum == 2) {
			if((m_cEscCode[1] == '$') && ((*pIn == 'B') || (*pIn == '@'))) {
				m_nKanjiMode = CONV_KANJI;
			} else
			if((m_cEscCode[1] == '(') && (*pIn == 'I')) {
				m_nKanjiMode = CONV_KANA;
			} else
			if((m_cEscCode[1] == '(') && ((*pIn == 'B') || (*pIn == 'J') || (*pIn == 'H'))) {
				m_nKanjiMode = CONV_ASCII;
			} else {
				*pOut++ = m_cEscCode[0];
				*pOut++ = m_cEscCode[1];
				if((m_nKanjiMode == CONV_KANJI) && ((*pIn >= 0x21) && (*pIn <= 0x7E))) {
					m_cKanjiFst = *pIn;
				} else {
					*pOut++ = *pIn;
				}
			}
			pIn++;
			m_nEscNum = 0;
		}
	}
	return (int)(pOut - (char*)*pOutBuf);
}
char *XStrBuffCodeConv::ConvJIStoSJISkanaProc(char cDt, char *pOut)
{
	int          nKcode;
	int          nDaku;

	cDt = (unsigned char)cDt + (unsigned char)0x80;
	if(m_nKanaConv == false) {
		if((unsigned char)cDt != (unsigned char)0x80) {
			*pOut++ = cDt;
		}
	} else {
		if(m_cKanjiPrev != 0) {
			nDaku = AskDakuon(m_cKanjiPrev, cDt);
#ifdef WIN32
			nKcode = _mbcjistojms(HanKataToZen(m_cKanjiPrev)) + nDaku;
#else
			nKcode = m_cKanjiPrev;
#endif /* WIN32 */
			*pOut++ = HIBYTE(nKcode);
			*pOut++ = LOBYTE(nKcode);

			if((nDaku == 0) && ((unsigned char)cDt != (unsigned char)0x80)) {
				m_cKanjiPrev = cDt;
			} else {
				m_cKanjiPrev = 0;
			}
		} else
		if((unsigned char)cDt != (unsigned char)0x80) {
			m_cKanjiPrev = cDt;
		}
	}
	return(pOut);
}
unsigned long XStrBuffCodeConv::ConvSJIStoEUC(XBuff *pOutBuf, unsigned long dwSrcSize)
{
	char* pOut = (char*)*pOutBuf;
	char* pIn  = (char*)*this;
	unsigned int i;
	int          nKcode;

	for(i = 0; i < dwSrcSize; i++) {
		if( pOut >= pOutBuf->GetMaxBuffAddress() ) {
			break;
		}

		// VAX のエスケープシーケンスの場合
		if( *pIn == 0xffffff9b ) {
			*pOut++ = *pIn++;
			continue;
		}		
		
		
		if(m_cKanjiFst == 0) {
			if((((unsigned char)*pIn >= (unsigned char)0x81) && ((unsigned char)*pIn <= (unsigned char)0x9F)) ||
			   ((unsigned char)*pIn >= (unsigned char)0xE0)) {
				pOut = ConvSJIStoEUCkanaProc(0, pOut);
				m_cKanjiFst = *pIn++;
			} else
			if(((unsigned char)*pIn >= (unsigned char)0xA0) && ((unsigned char)*pIn <= (unsigned char)0xDF)) {
				pOut = ConvSJIStoEUCkanaProc(*pIn++, pOut);
			} else {
				pOut = ConvSJIStoEUCkanaProc(0, pOut);
				*pOut++ = *pIn++;
			}
		} else {
			if((unsigned char)*pIn >= (unsigned char)0x40) {
#ifdef WIN32
				nKcode = _mbcjmstojis(((unsigned char)m_cKanjiFst * 0x100) + (unsigned char)*pIn++);
#else
				nKcode = *pIn++;
#endif /* WIN32 */
				*pOut++ = HIBYTE(nKcode) | 0x80;
				*pOut++ = LOBYTE(nKcode) | 0x80;
			} else {
				*pOut++ = m_cKanjiFst;
				*pOut++ = *pIn++;
			}
			m_cKanjiFst = 0;
		}
	}
	return (int)(pOut - (char*)*pOutBuf);
}
char *XStrBuffCodeConv::ConvSJIStoEUCkanaProc(char Dt, char *pOut)
{
	int          nKcode;
	int          nDaku;

	if(m_nKanaConv == false) {
		if(Dt != 0) {
			nKcode = 0x8E00 + (unsigned char)Dt;
			*pOut++ = HIBYTE(nKcode) | 0x80;
			*pOut++ = LOBYTE(nKcode) | 0x80;
		}
	} else {
		if(m_cKanjiPrev != 0) {
			nDaku = AskDakuon(m_cKanjiPrev, Dt);
			nKcode = HanKataToZen(m_cKanjiPrev) + nDaku;
			*pOut++ = HIBYTE(nKcode) | 0x80;
			*pOut++ = LOBYTE(nKcode) | 0x80;

			if(nDaku == 0) {
				m_cKanjiPrev = Dt;
			} else {
				m_cKanjiPrev = 0;
			}
		} else {
			m_cKanjiPrev = Dt;
		}
	}
	return(pOut);
}
unsigned long XStrBuffCodeConv::ConvSJIStoJIS(XBuff *pOutBuf, unsigned long dwSrcSize)
{
	char* pOut = (char*)*pOutBuf;
	char* pIn  = (char*)*this;
	unsigned int i;
	int          nKcode;


	for(i = 0; i < dwSrcSize; i++) {
		if( pOut >= pOutBuf->GetMaxBuffAddress() ) {
			break;
		}


		if(m_cKanjiFst == 0) {
			if((((unsigned char)*pIn >= (unsigned char)0x81) && ((unsigned char)*pIn <= (unsigned char)0x9F)) ||
			   ((unsigned char)*pIn >= (unsigned char)0xE0))
			{
				pOut = ConvSJIStoJISkanaProc(0, pOut);
				m_cKanjiFst = *pIn++;
			} else
			if(((unsigned char)*pIn >= (unsigned char)0xA0) && ((unsigned char)*pIn <= (unsigned char)0xDF)) {
				pOut = ConvSJIStoJISkanaProc(*pIn++, pOut);
			} else {
				pOut = ConvSJIStoJISkanaProc(0, pOut);
				if(m_nKanjiMode != CONV_ASCII) {
					*pOut++ = 0x1B;
					*pOut++ = '(';
					*pOut++ = 'B';
					m_nKanjiMode = CONV_ASCII;
				}
				*pOut++ = *pIn++;
			}
		} else {
			pOut = ConvSJIStoJISkanaProc(0, pOut);
			if((unsigned char)*pIn >= (unsigned char)0x40) {
				if(m_nKanjiMode != CONV_KANJI) {
					*pOut++ = 0x1B;
					*pOut++ = '$';
					*pOut++ = 'B';
					m_nKanjiMode = CONV_KANJI;
				}
#ifdef WIN32
				nKcode = _mbcjmstojis(((unsigned char)m_cKanjiFst * 0x100) + (unsigned char)*pIn++);
#else
				nKcode = *pIn++;
#endif /* WIN32 */
				*pOut++ = HIBYTE(nKcode);
				*pOut++ = LOBYTE(nKcode);
			} else {
				if(m_nKanjiMode != CONV_ASCII) {
					*pOut++ = 0x1B;
					*pOut++ = '(';
					*pOut++ = 'B';
					m_nKanjiMode = CONV_ASCII;
				}
				*pOut++ = m_cKanjiFst;
				*pOut++ = *pIn++;
			}
			m_cKanjiFst = 0;
		}
	}
	return (int)(pOut - (char*)*pOutBuf);
}
char *XStrBuffCodeConv::ConvSJIStoJISkanaProc(char Dt, char *pOut)
{
	int          nKcode;
	int          nDaku;

	if(m_nKanaConv == false) {
		if(Dt != 0) {
			if(m_nKanjiMode != CONV_KANA) {
				*pOut++ = 0x1B;
				*pOut++ = '(';
				*pOut++ = 'I';
				m_nKanjiMode = CONV_KANA;
			}
			*pOut++ = (unsigned char)Dt - (unsigned char)0x80;
		}
	} else {
		if(m_cKanjiPrev != 0) {
			if(m_nKanjiMode != CONV_KANJI) {
				*pOut++ = 0x1B;
				*pOut++ = '$';
				*pOut++ = 'B';
				m_nKanjiMode = CONV_KANJI;
			}
			nDaku = AskDakuon(m_cKanjiPrev, Dt);
			nKcode = HanKataToZen(m_cKanjiPrev) + nDaku;
			*pOut++ = HIBYTE(nKcode);
			*pOut++ = LOBYTE(nKcode);

			if(nDaku == 0) {
				m_cKanjiPrev = Dt;
			} else {
				m_cKanjiPrev = 0;
			}
		} else {
			m_cKanjiPrev = Dt;
		}
	}
	return(pOut);
}
const XStrBuffCodeConv& XStrBuffCodeConv::operator=(const wchar_t* lpsz)
{
	XStrBuff::operator=(lpsz);
	return *this;
}
const XStrBuffCodeConv& XStrBuffCodeConv::operator=(const char* lpsz)
{
	XStrBuff::operator=(lpsz);
	return *this;
}

int XStrBuffCodeConv::AskDakuon(char Ch, char nDaku)
{
	int nRet;

	nRet = 0;
	if((unsigned char)nDaku == (unsigned char)0xDE) {
		if((((unsigned char)Ch >= (unsigned char)0xB6) && ((unsigned char)Ch <= (unsigned char)0xC4)) ||
		   (((unsigned char)Ch >= (unsigned char)0xCA) && ((unsigned char)Ch <= (unsigned char)0xCE)))
		{
			nRet = 1;
		}
	}else
	if((unsigned char)nDaku == (unsigned char)0xDF) {
		if(((unsigned char)Ch >= (unsigned char)0xCA) && ((unsigned char)Ch <= (unsigned char)0xCE)) {
			nRet = 2;
		}
	}
	return(nRet);
}
int XStrBuffCodeConv::HanKataToZen(char Ch)
{
	static const int Katakana[] = {
		0x2121, 0x2123, 0x2156, 0x2157, 0x2122, 0x2126, 0x2572, 0x2521, 
		0x2523, 0x2525, 0x2527, 0x2529, 0x2563, 0x2565, 0x2567, 0x2543, 
		0x213C, 0x2522, 0x2524, 0x2526, 0x2528, 0x252A, 0x252B, 0x252D, 
		0x252F, 0x2531, 0x2533, 0x2535, 0x2537, 0x2539, 0x253B, 0x253D, 
		0x253F, 0x2541, 0x2544, 0x2546, 0x2548, 0x254A, 0x254B, 0x254C, 
		0x254D, 0x254E, 0x254F, 0x2552, 0x2555, 0x2558, 0x255B, 0x255E, 
		0x255F, 0x2560, 0x2561, 0x2562, 0x2564, 0x2566, 0x2568, 0x2569, 
		0x256A, 0x256B, 0x256C, 0x256D, 0x256F, 0x2573, 0x212B, 0x212C
	};

	return(Katakana[(unsigned char)Ch - (unsigned char)0xA0]);
}






//
//	XStrBuffBase64Coder
//
XStrBuffBase64Coder::XStrBuffBase64Coder()
	:XStrBuff()
{
}
XStrBuffBase64Coder::XStrBuffBase64Coder(const char* p)
	:XStrBuff(p)
{
}
XStrBuffBase64Coder::XStrBuffBase64Coder(const wchar_t* p)
	:XStrBuff(p)
{
}
XStrBuffBase64Coder::~XStrBuffBase64Coder()
{
}
const XStrBuffBase64Coder& XStrBuffBase64Coder::operator=(const char* lpsz)
{
	XStrBuff::operator=(lpsz);
	return *this;
}
const XStrBuffBase64Coder& XStrBuffBase64Coder::operator=(const wchar_t* lpsz)
{
	XStrBuff::operator=(lpsz);
	return *this;
}
int XStrBuffBase64Coder::encode( const char* lpszSrc, char* lpszDest, int nSrcLen)
{
	int iDestSize = 0;
	int i, hiteof = 0;
	int nPos = 0;
	unsigned char  dtable[256];


	for (i = 0; i < 26; i++) {
		dtable[i] = 'A' + i;
		dtable[26 + i] = 'a' + i;
	}
	for (i = 0; i < 10; i++) {
		dtable[52 + i] = '0' + i;
	}
	dtable[62] = '+';
	dtable[63] = '/';

	while (!hiteof) {
		unsigned char igroup[3], ogroup[4];
		int c, n;

		igroup[0] = igroup[1] = igroup[2] = 0;
		for (n = 0; n < 3; n++) {
			c = *lpszSrc;
			
			if (nPos == nSrcLen) {
				hiteof = 1;
				break;
			}
			lpszSrc++;
			nPos++;
			igroup[n] = (unsigned char) c;
		}
		if (n > 0) {
			ogroup[0] = dtable[igroup[0] >> 2];
			ogroup[1] = dtable[((igroup[0] & 3) << 4) | (igroup[1] >> 4)];
			ogroup[2] = dtable[((igroup[1] & 0xF) << 2) | (igroup[2] >> 6)];
			ogroup[3] = dtable[igroup[2] & 0x3F];

			if (n < 3) {
				ogroup[3] = '=';
				if (n < 2) {
					ogroup[2] = '=';
				}
			}
			for (i = 0; i < 4; i++) {
				*lpszDest = ogroup[i];
				lpszDest++;
				iDestSize++;
			}
		}
	}
	return iDestSize;
}
int XStrBuffBase64Coder::decode(const char* lpszSrc, char* lpszDest, int nSrcLen)
{
	int iDestSize = 0;
	int i;
	int nPos = 0;
	unsigned char   dtable[256];
	for (i = 0; i < 255; i++) {
		dtable[i] = 0x80;
	}
	for (i = 'A'; i <= 'Z'; i++) {
		dtable[i] = 0 + (i - 'A');
	}
	for (i = 'a'; i <= 'z'; i++) {
		dtable[i] = 26 + (i - 'a');
	}
	for (i = '0'; i <= '9'; i++) {
		dtable[i] = 52 + (i - '0');
	}
	dtable['+'] = 62;
	dtable['/'] = 63;
	dtable['='] = 0;

	while (1) {
		unsigned char a[4], b[4];

		for (i = 0; i < 4; i++) {
			// 改行は無視します
			if(*lpszSrc == '\r') {
				lpszSrc++;
				nPos++;
			}
			if(*lpszSrc == '\n') {
				lpszSrc++;
				nPos++;
			}


			int c = *lpszSrc;

			if (nPos == nSrcLen) {
				return iDestSize;
			}
			if (dtable[c] & 0x80) {
				return iDestSize;
			}
			a[i] = (unsigned char) c;
			b[i] = (unsigned char) dtable[c];
			lpszSrc++;
			nPos++;
		}
		*lpszDest = (b[0] << 2) | (b[1] >> 4);
		lpszDest++;
		iDestSize++;
		if(a[2] != '=') {
			*lpszDest = (b[1] << 4) | (b[2] >> 2);
			lpszDest++;
			iDestSize++;
		}
		if(a[3] != '=') {
			*lpszDest = (b[2] << 6) | b[3];
			lpszDest++;
			iDestSize++;
		}

		i = a[2] == '=' ? 1 : (a[3] == '=' ? 2 : 3);
		if (i < 3) {
			return iDestSize;
		}
	}
	return iDestSize;
}

const TCHAR* XStrBuffBase64Coder::Encode()
{
	XStrBuff		buff;
	unsigned long   nBuffLen;
	char*			lpszBuff;
	char*			lpszBuffOut;

	nBuffLen = this->GetBuffSize();
	lpszBuff = (char*)buff.Alloc(((nBuffLen*3)) * sizeof(char));
	if( lpszBuff == NULL ) {
		return NULL;
	}
	buff.MemSet(0);

	nBuffLen = encode(*this, lpszBuff, nBuffLen );
	if( nBuffLen == 0 ) {
		return NULL;
	}

	lpszBuffOut = (char*)this->Alloc(nBuffLen+1);
	if( lpszBuffOut == NULL ) {
		return NULL;
	}
	this->MemSet(0);
	::memcpy(lpszBuffOut, lpszBuff, nBuffLen);
	return *this;
}
const TCHAR* XStrBuffBase64Coder::Decode()
{
	XStrBuff		buff;
	unsigned long   nBuffLen;
	char*			lpszBuff;
	char*			lpszBuffOut;

	nBuffLen = this->GetBuffSize();
	lpszBuff = (char*)buff.Alloc(nBuffLen);
	if( lpszBuff == NULL ) {
		return NULL;
	}
	buff.MemSet(0);

	nBuffLen = decode(*this, lpszBuff, nBuffLen );
	if( nBuffLen == 0 ) {
		return NULL;
	}
	lpszBuffOut = (char*)this->Alloc(nBuffLen);
	if( lpszBuffOut == NULL ) {
		return NULL;
	}
	this->MemSet(0);
	::memcpy(lpszBuffOut, lpszBuff, nBuffLen);
	return *this;
}

// バッファをエンコードします。
bool XStrBuffBase64Coder::encode(XBuff& src, XStrBuff& dst)
{
	XStrBuff		tmpBuff;
	unsigned long   nBuffLen = 0;
	char*			lpszBuff = NULL;

	nBuffLen = src.GetBuffSize();
	lpszBuff = (char*)tmpBuff.Alloc(((nBuffLen*3)) * sizeof(char));
	if( lpszBuff == NULL ) {
		return false;
	}
	tmpBuff.MemSet(0);

	nBuffLen = encode(src, lpszBuff, nBuffLen );
	if( nBuffLen == 0 ) {
		return false;
	}

	dst = lpszBuff;
	return true;
}

// バッファをデコードします。
bool XStrBuffBase64Coder::decode(const char* psrc, XBuff& dst)
{
	XBuff			tmpBuff;
	unsigned long   nBuffLen = 0;
	char*			lpszBuff = NULL;
	char*			lpszBuffOut = NULL;

	nBuffLen = ::strlen(psrc);
	lpszBuff = (char*)tmpBuff.Alloc(nBuffLen);
	if( lpszBuff == NULL ) {
		return false;
	}
	tmpBuff.MemSet(0);

	nBuffLen = decode(psrc, lpszBuff, nBuffLen );
	if( nBuffLen == 0 ) {
		return false;
	}
	lpszBuffOut = (char*)dst.Alloc(nBuffLen);
	if( lpszBuffOut == NULL ) {
		return false;
	}
	dst.MemSet(0);
	::memcpy(lpszBuffOut, lpszBuff, nBuffLen);

	return true;
}
bool XStrBuffBase64Coder::decode(const wchar_t* psrc, XBuff& dst)
{
	XStrBuff tmpStr( psrc);
	return XStrBuffBase64Coder::decode((const char*)tmpStr, dst);
}
