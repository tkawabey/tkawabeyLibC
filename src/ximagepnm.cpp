#include "xlib/image/ximagepnm.h"
//#include <errno.h>
#ifdef WIN32
	#define XMD_H
	#undef FAR
#endif

char* XImagePNM::NextToken(char **m_pData, size_t* nBuffSize)
{
	while( *nBuffSize != 0 ) {
		if( **m_pData == '#' ) {
			(*m_pData) ++;
			(*nBuffSize) --;
			// コメントをスキップ
			while( *nBuffSize != 0 ) {
				if( **m_pData == '\n' ) {
					break;
				}
				XImagePNM::SkipValue(m_pData, nBuffSize, 1);
			}
			if( *nBuffSize == 0 ) {
				return NULL;
			}
			
		}
		if( !(**m_pData == ' ' || **m_pData == '\n' || **m_pData == '\t' || **m_pData == '\r') ) {
			break;
		}		
		XImagePNM::SkipValue(m_pData, nBuffSize, 1);
	}
	if( *nBuffSize == 0 ) {
		return NULL;
	}
	return *m_pData;
}
char* XImagePNM::SkipValue(char **m_pData, size_t* nBuffSize, int m_nDataSize)
{
	(*m_pData) += m_nDataSize;
	(*nBuffSize) -= m_nDataSize;
	return *m_pData;
}
bool XImagePNM::Str2UL(char **m_pData, size_t* nBuffSize, unsigned long *nValue)
{
	char *pEndPoint = NULL;
	int nStringLen;

	if( XImagePNM::NextToken(m_pData, nBuffSize) == NULL ) {
		return false;
	}

	*nValue = ::strtoul(*m_pData, &pEndPoint, 10);
	if( *nValue == ULONG_MAX ) {
		return false;
	} else
//	if(*nValue == 0 && errno == ERANGE ) {
//		return false;
//	}
	nStringLen = (int)pEndPoint - (int)*m_pData;
	XImagePNM::SkipValue(m_pData, nBuffSize, nStringLen);
	return true;
}
/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImagePNM::Decoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImagePNM::Decoder::Decoder()
{
	m_nBitLeft = 0;
	m_nOldBalue = 0;
	m_nMask = 0;
	m_nDataSize = 0;
	m_pData = NULL;
}
XImagePNM::Decoder::Decoder(const XImagePNM::Decoder& source)
	:	XImageDecoder(source) 
{

}
XImagePNM::Decoder::~Decoder()
{
}
XImagePNM::Decoder& XImagePNM::Decoder::operator=(const XImagePNM::Decoder& source)
{
	XImageDecoder::operator=(source);
	return *this;
}
void XImagePNM::Decoder::ReadImageFile(const char* lpszFileName, XImage& image)
{
	XImageStream inputstream ;
	inputstream.OpenA(lpszFileName, true, false, false, true);
	ReadImage(inputstream, image);
}

void XImagePNM::Decoder::ReadImage(XImageStream& strm, XImage& image)
{
	XStrBuff strBuff;
	bool bGrayscal = false;
	unsigned long  nRaw;
	unsigned long  bitcount;
	unsigned long  nWidth;
	unsigned long  nHight;
	unsigned long  nMaxValue;
	unsigned long  nRow;
	unsigned long  nCol;
	unsigned long  nCannel;
	

	m_nDataSize = strm.GetLength();
	m_pData = (char*)strBuff.Alloc(m_nDataSize);
	if( m_pData == NULL ) {
		XImageException::Throw( XIMG_ERR_MALLOC );
	}
	if( strm.Read(m_pData, m_nDataSize) == false ) {
		XImageException::Throw( XIMG_ERR_END_STREAM );
	}
	if( *m_pData != 'P' ) {
		XImageException::Throw( XIMG_ERR_UNSUPPORT_BIT_MAP_HEADER );
	}

	XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1);

	if( *m_pData == '1' || *m_pData == '4' ) {
		bGrayscal = true;

		nRaw = (*m_pData == '4');
		bitcount = 1;

		XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1);

		XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nWidth);
		XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nHight);
	} else
	if( *m_pData == '2' || *m_pData == '5' ) {
		bGrayscal = true;

		nRaw = (*m_pData == '5');

		XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1);

		XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nWidth);
		XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nHight);
		XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nMaxValue);
		
		if (nMaxValue <= 1)
			bitcount = 1;
		else if (nMaxValue <= 3)
			bitcount = 2;
		else if (nMaxValue <= 15)
			bitcount = 4;
		else if (nMaxValue <= 255)
			bitcount = 8;
		else /* if (nMaxValue <= 65535) */
			bitcount = 16;
	} else
	if( *m_pData == '3' || *m_pData == '6' ) {

		nRaw = (*m_pData == '6');

		XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1);

		XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nWidth);
		XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nHight);
		XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nMaxValue);
		
		if (nMaxValue <= 1)
			bitcount = 1;
		else if (nMaxValue <= 3)
			bitcount = 2;
		else if (nMaxValue <= 15)
			bitcount = 4;
		else if (nMaxValue <= 255)
			bitcount = 8;
		else /* if (nMaxValue <= 65535) */
			bitcount = 16;
	} else {
		XImageException::Throw( XIMG_ERR_UNSUPPORT_BIT_MAP_HEADER );
	}

	XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1);

	if( bGrayscal == true ) {
		nCannel = 1;

		image.SetSize(1 << bitcount,     // Color Table Entries
				bitcount,  // Bits
				nWidth, nHight);
	} else {
		nCannel = 3;

		image.SetSize(0,     // Color Table Entries
				bitcount * 3,  // Bits
				nWidth, nHight);
	}

	if( bGrayscal == true ) {
		if( bitcount == 1) {
			image.GetColorMap(0).red   =(XUBYTE1)255 ;
			image.GetColorMap(0).green =(XUBYTE1)255;
			image.GetColorMap(0).blue  =(XUBYTE1)255;
			image.GetColorMap(1).red   =(XUBYTE1)0 ;
			image.GetColorMap(1).green =(XUBYTE1)0;
			image.GetColorMap(1).blue  =(XUBYTE1)0;
		} else {
			// グレースケール用カラーマップの作成
			for(unsigned int ii = 0 ; ii <(1 << bitcount); ++ ii) {
				image.GetColorMap(ii).red   =(XUBYTE1) ii ;
				image.GetColorMap(ii).green =(XUBYTE1) ii ;
				image.GetColorMap(ii).blue  =(XUBYTE1) ii ;
			}
		}



		register int nSrcBitLeft = 0;
		register int nSrcOldValue   = 0;
		register int nSrcMask    = 0;
		for (nRow = 0; nRow < nHight; nRow++) {

			CallProgressFunction(nRow * 100 / nHight) ;

			register int nMask = 0;
			register int nValue = 0;
			register int nOldValue = 0;
			register int nLeft = 0;
			XUBYTE1 *pImageData = image[ nRow ];
			for (nCol = 0; nCol < nWidth; nCol++) {
				if( bitcount >= 8 ) {
					if( nRaw ) {
						if( m_nDataSize == 0 ) {
							XImageException::Throw( XIMG_ERR_END_STREAM );
						}
						*pImageData = *m_pData;
						m_pData++;
						m_nDataSize--;
						pImageData++;
					} else {
						unsigned long nTemp;
						if( XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nTemp) == false ) {
							XImageException::Throw( XIMG_ERR_END_STREAM );
						}
						*pImageData = nTemp;
						pImageData++;
					}
				} else {
					register int i;
					register int nTmp;


					if( nRaw ) {
						if (nSrcMask == 0) {
							for (i = 0; i < bitcount; i++){
								nSrcMask = (nSrcMask >> 1) | 0x80;
							}
						}
						if (nSrcBitLeft <= 0) {

							if( m_nDataSize == 0 ) {
								XImageException::Throw( XIMG_ERR_END_STREAM );
							}

							nSrcOldValue = *m_pData;

							m_pData++;
							m_nDataSize--;
							nSrcBitLeft = 8;
						}
						nTmp = nSrcOldValue & nSrcMask;
						for (i = 1; i < (8 / bitcount); i++) {
							nTmp = nTmp || (nTmp >> bitcount);
						}
						nSrcOldValue = (nSrcOldValue << bitcount) & 0xFF;



					} else {
						if( bitcount == 1 ) {
							while( *m_pData == '\n' ||  *m_pData == '\r' ||  *m_pData == '\t' ||  *m_pData == ' ' ) {
								if( m_nDataSize == 0 ) {
									XImageException::Throw( XIMG_ERR_END_STREAM );
								}
								if( XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1) == false ) {
									XImageException::Throw( XIMG_ERR_END_STREAM );
								}
							}
							nTmp = *m_pData == '0' ? 0 : 1;
							if( XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1) == false ) {
								XImageException::Throw( XIMG_ERR_END_STREAM );
							}
						} else {
							if( XImagePNM::Str2UL(&m_pData, &m_nDataSize, (unsigned long*)nTmp) == false ) {
								XImageException::Throw( XIMG_ERR_END_STREAM );
							}
						}
					}
					if (nLeft <= 0) {
						nLeft = 8;
					}
					nTmp = (nTmp <<  ((nLeft/bitcount)-1) ) & 0xFF;

					nValue |= nTmp;
					nLeft -= bitcount;
					nSrcBitLeft -= bitcount;
					if (nLeft <= 0) {
						*pImageData = nValue;
						pImageData++;

						nValue = 0;
					}
				}
			}
		}
	} else {
		for (nRow = 0; nRow < nHight; nRow++) {

			CallProgressFunction(nRow * 100 / nHight) ;

			XUBYTE1 *pImageData = image[ nRow ];
			for (nCol = 0; nCol < nWidth; nCol++) {
				if( nRaw ) {
					*pImageData++ = *m_pData;
					XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1);
					*pImageData++ = *m_pData;
					XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1);
					*pImageData++ = *m_pData;
					XImagePNM::SkipValue(&m_pData, &m_nDataSize, 1);
				} else {
					unsigned long nTemp;					
					if( XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nTemp) == false ) {
						XImageException::Throw( XIMG_ERR_END_STREAM );
					}
					*pImageData++ = nTemp;
					if( XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nTemp) == false ) {
						XImageException::Throw( XIMG_ERR_END_STREAM );
					}
					*pImageData++ = nTemp;
					if( XImagePNM::Str2UL(&m_pData, &m_nDataSize, &nTemp) == false ) {
						XImageException::Throw( XIMG_ERR_END_STREAM );
					}
					*pImageData++ = nTemp;
				}
			}
		}
	}

}
void XImagePNM::Decoder::DoCopy(const XImagePNM::Decoder& source)
{

}
void XImagePNM::Decoder::CallProgressFunction(unsigned int percent)
{
}
void XImagePNM::Decoder::_CallProgressFunction(unsigned int percent)
{
}
/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImagePNM::Encoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImagePNM::Encoder::Encoder()
{

}
XImagePNM::Encoder::Encoder(const XImagePNM::Encoder& source)
	:	XImageEncoder(source) 
{
}
XImagePNM::Encoder::~Encoder()
{

}
XImagePNM::Encoder& XImagePNM::Encoder::operator=(const XImagePNM::Encoder& source)
{
	XImageEncoder::operator=(source);
	return *this;
}
void XImagePNM::Encoder::WriteImage(XImageStream &strm, const XImage &image)
{

}
void XImagePNM::Encoder::WriteImageFile(const char* lpszFileName, XImage &image)
{

}
void XImagePNM::Encoder::CallProgressFunction(unsigned int percent)
{

}
