#include "xlib/image/ximagegif.h"
#ifdef WIN32
	#define XMD_H
	#undef FAR
#endif
extern "C" {
	#include <libungif/lib/gif_lib.h>
}
#include <setjmp.h>


/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImageGIF::Decoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImageGIF::Decoder::Decoder()
{

}
XImageGIF::Decoder::Decoder(const XImageGIF::Decoder& source)
	:	XImageDecoder(source) 
{

}
XImageGIF::Decoder::~Decoder()
{
	for(int i = 0; i < m_AnimeImage.GetSize(); i++) {
		AnimeImage *p = m_AnimeImage[i];
		delete p;
	}
	m_AnimeImage.RemoveAll();
}
XImageGIF::Decoder& XImageGIF::Decoder::operator=(const XImageGIF::Decoder& source)
{
	XImageDecoder::operator=(source);
	return *this;
}
void XImageGIF::Decoder::ReadImageFile(const char* lpszFileName, XImage& image)
{
	XImageStream inputstream ;
	inputstream.OpenA(lpszFileName, true, false, false, true);
	ReadImage(inputstream, image);
}


void XImageGIF::Decoder::ReadImage(XImageStream& strm, XImage& image)
{
	GifFileType *pGifFile;
	int			i;
	int			nBackGround;
	int			nColorMapSize;
	int			nSize;
	GifRecordType	RecordType;
	GifColorType	*ColorMapEntry;
	ColorMapObject	*pColorMaoObj = NULL;
	GifRowType		GifRow = NULL;
	XBuff			growBuff;
	bool			bFirstImage = true;
	int				nTrans = -1;
	int				nDelay = 0;
	XImage			*pCurrentImage = &image;
	AnimeImage		*pAnimeImageData = &m_AnimeImageData;

	pAnimeImageData->nLeft = 0;
	pAnimeImageData->nTop = 0;
	pAnimeImageData->nLateDelay = 0;
	pAnimeImageData->nTrance = 0;

	if ((pGifFile = ::DGifOpenFileHandle( strm.FileNo() )) == NULL) { 
		return ;
	}
	// バックグラウンド
	nBackGround = pGifFile->SBackGroundColor;
	pColorMaoObj = (pGifFile->Image.ColorMap ? pGifFile->Image.ColorMap : pGifFile->SColorMap);
	nColorMapSize = pColorMaoObj->ColorCount;


	nSize = pGifFile->SWidth * sizeof(GifPixelType); 
	image.SetSize(1 << 8, 8, pGifFile->SWidth, pGifFile->SHeight);


	// カラーテーブルのセット
	for(i = 0; i < nColorMapSize; i++) {
		ColorMapEntry = &pColorMaoObj->Colors[i];
		image.GetColorMap(i).red   =(XUBYTE1)ColorMapEntry->Red;
		image.GetColorMap(i).green =(XUBYTE1)ColorMapEntry->Green;
		image.GetColorMap(i).blue  =(XUBYTE1)ColorMapEntry->Blue;
	}


	// ライン用バッファの確保
	GifRow = (GifRowType)growBuff.Alloc(nSize);
	for(i = 0; i < pGifFile->SWidth; i++) {
		GifRow[i] = pGifFile->SBackGroundColor;
	}

	do {
		// レコードタイプの取得
		if (DGifGetRecordType(pGifFile, &RecordType) == GIF_ERROR) {
			return ;
		}
		switch (RecordType) {
		case IMAGE_DESC_RECORD_TYPE:
			{
				int Row, Col, Width, Height;
				if (DGifGetImageDesc(pGifFile) == GIF_ERROR) {
					return ;
				}
				Row = pGifFile->Image.Top;
				Col = pGifFile->Image.Left;
				Width = pGifFile->Image.Width;
				Height = pGifFile->Image.Height;
				pAnimeImageData->nLeft = Col;
				pAnimeImageData->nTop = Row;

				if( bFirstImage == false ) {
					AnimeImage *pAnimeImage = new AnimeImage;
					pAnimeImage->nLateDelay = 0;
					pAnimeImage->nTrance = 0;
					pAnimeImage->nTop = Row;
					pAnimeImage->nLeft = Col;
					pCurrentImage = &pAnimeImage->image;
					pCurrentImage->SetSize(1 << 8, 8, Width, Height);

					m_AnimeImage.Add(pAnimeImage);
				}


				if (pGifFile->Image.Left + pGifFile->Image.Width > pGifFile->SWidth ||
					pGifFile->Image.Top + pGifFile->Image.Height > pGifFile->SHeight) {

					return ;
				}

				if (pGifFile->Image.Interlace) {

					int InterlacedOffset[] = { 0, 4, 2, 1 };
					int InterlacedJumps[] = { 8, 8, 4, 2 };

					for ( i = 0; i < 4; i++ ) {
						for (Row = InterlacedOffset[i]; Row < Height; Row += InterlacedJumps[i]) {
							memset( GifRow, nBackGround, Width);

							if (DGifGetLine(pGifFile, &GifRow[Col], Width) == GIF_ERROR) {
								return ;
							}
							XUBYTE1 *pData = (*pCurrentImage)[Row];
							::memcpy(pData, GifRow, pGifFile->SWidth);
					
						}
					}
				}else {
					for (i = 0; i < Height; i++) {
						memset( GifRow, nBackGround, Width);
						if (DGifGetLine(pGifFile, &GifRow[Col], Width) == GIF_ERROR) {
							return ;
						}
						XUBYTE1 *pData = (*pCurrentImage)[Row];
						::memcpy(pData, GifRow, pGifFile->SWidth);
						Row++;
					}
				}
			}

			bFirstImage = false;

			break;
		case EXTENSION_RECORD_TYPE:
			{
				GifByteType *Extension;
				int ExtCode;
				/* Skip any extension blocks in file: */
				if (DGifGetExtension(pGifFile, &ExtCode, &Extension) == GIF_ERROR) {
					return ;
				}

				if ( ExtCode == 0xf9 ) {
					if ( Extension[1] & 1 ) {
						// アニメーション情報
						nTrans = Extension[4];
						nDelay = ((Extension[3] << 8) | Extension[2]) * 10;
						pAnimeImageData->nLateDelay = nDelay;
						pAnimeImageData->nTrance = nTrans;
					}
				}
				else if ( ExtCode == 0xff ) {
					// アプリケーション独自の
				}

				while (Extension != NULL) {
					if (DGifGetExtensionNext(pGifFile, &Extension) == GIF_ERROR) {
						return ;
					}
				}
			}
			break;
		case TERMINATE_RECORD_TYPE:
			break;
		}
	} while (RecordType != TERMINATE_RECORD_TYPE);



	::DGifCloseFile(pGifFile);
}
void XImageGIF::Decoder::DoCopy(const XImageGIF::Decoder& sorce)
{

}
void XImageGIF::Decoder::CallProgressFunction(unsigned int percent)
{
	if (m_ProgressFunction == 0)
		return ;
	bool cancel = false ;
	m_ProgressFunction (*this, m_pProgressData, 1, 1, percent, cancel) ;
	if (cancel)
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL );
}
void XImageGIF::Decoder::_CallProgressFunction(unsigned int percent)
{
	this->CallProgressFunction(percent);
}
/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImageGIF::Encoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImageGIF::Encoder::Encoder()
{

}
XImageGIF::Encoder::Encoder(const XImageGIF::Encoder& source)
	:	XImageEncoder(source)
{
}
XImageGIF::Encoder::~Encoder()
{

}
XImageGIF::Encoder& XImageGIF::Encoder::operator=(const XImageGIF::Encoder& source)
{
	XImageEncoder::operator=(source);
	return *this;
}

void XImageGIF::Encoder::WriteImage(XImageStream &strm, const XImage &image)
{
	GifFileType		*pGifFile;
	int				nColorSize = image.GetColorCount();
	ColorMapObject	*pOutputColorMap = NULL;
	GifColorType	*ColorMapEntry;
	int				i;


	if ((pOutputColorMap = ::MakeMapObject(nColorSize, NULL)) == NULL) {
		return ;
	}
	for(i = 0; i < nColorSize; i++) {
		ColorMapEntry = &pOutputColorMap->Colors[i];
		ColorMapEntry->Red    = image.GetColorMap(i).red;
		ColorMapEntry->Green  = image.GetColorMap(i).green;
		ColorMapEntry->Blue   = image.GetColorMap(i).blue;
	}

	if ((pGifFile = ::EGifOpenFileHandle(strm.FileNo() )) == NULL) {
		return ;
	}
	if(	::EGifPutScreenDesc(pGifFile, image.GetWidth(), image.GetHeight(), nColorSize, 0,pOutputColorMap) == GIF_ERROR ||
		::EGifPutImageDesc(pGifFile,0, 0, image.GetWidth(), image.GetHeight(), FALSE, NULL) == GIF_ERROR) {
		return ;
	}


	for(int y = 0; y < image.GetHeight(); y++) {
		XUBYTE1 *pData = image[y];
		if (EGifPutLine(pGifFile, pData, image.GetWidth()) == GIF_ERROR) {
			
		}
	}
	::EGifCloseFile(pGifFile);
	::FreeMapObject(pOutputColorMap);
}
void XImageGIF::Encoder::WriteImageFile(const char* lpszFileName, XImage &image)
{
	XImageStream strm;
	strm.CreateA(lpszFileName, false, false, true, 0);
	WriteImage(strm, image);
}
void XImageGIF::Encoder::CallProgressFunction(unsigned int percent)
{
	if (m_ProgressFunction == 0)
		return ;
	bool cancel = false ;
	m_ProgressFunction (*this, m_pProgressData, 1, 1, percent, cancel) ;
	if (cancel)
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL);
}
