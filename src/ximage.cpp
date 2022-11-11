#include "xlib/image/ximage.h"



/*
//////////////////////////////////////////////////////////////////////////////
//
//	XImageException
//
*/
XImageException::XImageException()
{
}
XImageException::~XImageException() throw ()
{
}
void XImageException::Throw(XIMG_ERR err)
{
	XImageException *pE = new XImageException();
	pE->m_err = err;
	throw pE;
}	

/*
//////////////////////////////////////////////////////////////////////////////
//
//	XImageStream
//
*/
XImageStream::XImageStream()
{
	m_dwCurrentPos		= 0;
	m_dwFileSize		= 0;
	m_dwReadedCount		= 0;
	m_dwReaded			= 0;
}
XImageStream::~XImageStream()
{
	m_dwCurrentPos		= 0;
	m_dwFileSize		= 0;
	m_dwReadedCount		= 0;
	m_dwReaded			= 0;
}
bool XImageStream::Close()
{
	m_dwCurrentPos		= 0;
	m_dwFileSize		= 0;
	m_dwReadedCount		= 0;
	m_dwReaded			= 0;
	return XFile::Close();
}
bool XImageStream::OpenRawA(const char *filename,    int flg, bool bShare, int pmode)
{
	bool bRet = XFile::OpenRawA(filename, flg, bShare, pmode);
	if( bRet == true ) {
		m_dwFileSize = this->GetLength();
	}
	return bRet;
}
bool XImageStream::OpenRawW(const wchar_t *filename, int flg, bool bShare, int pmode)
{
	bool bRet = XFile::OpenRawW(filename, flg, bShare, pmode);
	if( bRet == true ) {
		m_dwFileSize = this->GetLength();
	}
	return bRet;
}
size_t XImageStream::WriteRaw(const void* lpData, size_t nSize, int flg/* = 0*/)
{
	m_dwReaded = 0;


	size_t s = XFile::WriteRaw(lpData, nSize, flg);
	if( s != (size_t)-1 ) {
		m_dwCurrentPos += s;
		m_dwReaded += s;
	}
	return s;
}
size_t XImageStream::ReadRaw(void* lpData, size_t nSize, int flg/* = 0*/)
{
	size_t s = XFile::ReadRaw(lpData, nSize, flg);
	if( s != (size_t)-1 ) {
		m_dwCurrentPos += s;
	}
	return s;
}
bool XImageStream::IsEOF()
{
	if( m_dwCurrentPos ==  m_dwFileSize ) {
		return true;
	} else {
		return false;
	}
}


/*
//////////////////////////////////////////////////////////////////////////////
//
//	XImageCoder
//
*/
XImageCoder::XImageCoder()
	:	m_ProgressFunction(0)
	,	m_pProgressData(0)
{
}
XImageCoder::XImageCoder(const XImageCoder &source)
	:	m_ProgressFunction(source.m_ProgressFunction)
	,	m_pProgressData(source.m_pProgressData)
{
}
XImageCoder::~XImageCoder()
{
}
XImageCoder& XImageCoder::operator=(const XImageCoder &source)
{
	if (this == &source)
		return *this ;

	m_ProgressFunction = source.m_ProgressFunction ;
	m_pProgressData = source.m_pProgressData ;
	return *this ;
}
void XImageCoder::SetProgressFunction (XIMG_PROGRESS_FUNC func, void *data)
{
	m_ProgressFunction = func;
	m_pProgressData = data;
}
void XImageCoder::UpdateImage()
{
	XTRACE(_T("** TRACE ** No Imple XImageCoder::UpdateImage\n"));
}



/*
//////////////////////////////////////////////////////////////////////////////
//
//	XImage
//
*/
XImage::XImage () 
	:	m_pProgressFunction(0)
	,	m_pProgressData(0)
{
	ClearData ();
}
XImage::XImage(const XImage &source)
{
	DoCopy(source) ;
}
XImage::~XImage ()
{
	if(m_pColorMap)
		delete [] m_pColorMap ; 
	m_pColorMap = 0 ; 
	if(m_pImageData)
		delete [] m_pImageData ; 
	m_pImageData = 0 ;
}
XImage &XImage::operator=(const XImage &source)
{
	if (&source == this)
		return *this ;

	if(m_pColorMap)
		delete [] m_pColorMap ; 
	m_pColorMap = 0 ; 
	if(m_pImageData)
		delete [] m_pImageData ; 
	m_pImageData = 0 ;

	DoCopy(source) ;
	return *this ;
}
void XImage::SetSize(unsigned int cc,
                           unsigned int bits,
                           unsigned int ww,
                           unsigned int hh)
{
	if(m_pColorMap)
		delete [] m_pColorMap ; 
	m_pColorMap = 0 ; 
	if(m_pImageData)
		delete [] m_pImageData ; 
	m_pImageData = 0 ;

	ClearData();

	switch (bits)
	{
	case 1:
	case 2:
	case 4:
	case 8:
		{
			m_nBitCount = bits ;
			m_nColorCount = cc ;
			m_nImageWidth = ww ;
			m_nImageHeight = hh ;
			
			m_pColorMap = new ColorMapEntry [m_nColorCount] ;
			memset (m_pColorMap, 0, sizeof (ColorMapEntry) * m_nColorCount) ;
			unsigned int bitsize = m_nBitCount * m_nImageWidth ;
			m_nRowWidth = RoundRow ((bitsize + 7)/8) ;
			unsigned int bytecount = m_nRowWidth * m_nImageHeight ;
			m_pImageData = new XUBYTE1[bytecount] ;
			::memset (m_pImageData, 0, bytecount) ;
		}
		break ;
	default:
		if (bits >= 24 ){
			int nRoundSize = bits / 8;

			m_nBitCount = bits ;
			m_nColorCount = cc ;
			m_nImageWidth = ww ;
			m_nImageHeight = hh ;
			m_nRowWidth = RoundRow (nRoundSize * m_nImageWidth) ;
			m_pImageData = new XUBYTE1[m_nRowWidth * m_nImageHeight] ;
			::memset (m_pImageData, 0, m_nRowWidth * m_nImageHeight) ;
		} else {
			XImageException::Throw(XIMG_ERR_UNSUPPORT_BITCOUNT);
		}
	}
}
void XImage::CopyImageData(const unsigned char	*pImageDat, int nImageDataSize)
{
	unsigned int bytecount = m_nRowWidth * m_nImageHeight ;
	_XASSERT(m_pImageData);
	_XASSERT( nImageDataSize == bytecount );
	::memcpy(m_pImageData , pImageDat, nImageDataSize);
}
void XImage::GetImageData(unsigned char *pImageDat, int nImageDataSize)
{
	unsigned int bytecount = m_nRowWidth * m_nImageHeight ;
	_XASSERT(m_pImageData);
	_XASSERT( nImageDataSize == bytecount );
	::memcpy(pImageDat , m_pImageData, nImageDataSize);
}


XUBYTE1* XImage::operator[](unsigned int xx) const
{
	if (xx >= m_nImageHeight)
		XImageException::Throw(XIMG_ERR_UNSUPPORT_BITCOUNT);
	return  &m_pImageData[(m_nImageHeight - xx - 1) * m_nRowWidth];
}
XImage::ColorMapEntry &XImage::GetColorMap (unsigned int index)
{
	if (index >= m_nColorCount)
		XImageException::Throw( XIMG_ERR_SUBSCRIPT_OUT_RANGE );

	return m_pColorMap[index];
}
XImage::ColorMapEntry XImage::GetColorMap (unsigned int index) const
{
	if (index >= m_nColorCount)
		XImageException::Throw( XIMG_ERR_SUBSCRIPT_OUT_RANGE );

	return m_pColorMap[index];
}

void XImage::ClearImage ()
{
	if(m_pColorMap)
		delete [] m_pColorMap ; 
	m_pColorMap = 0 ; 
	if(m_pImageData)
		delete [] m_pImageData ; 
	m_pImageData = 0 ;
	ClearData();
}
XUBYTE1 *XImage::ImageData()
{
	return m_pImageData ;
}
unsigned int XImage::GetWidth () const
{
	return m_nImageWidth ;
}
unsigned int XImage::GetHeight () const
{
	return m_nImageHeight ;
}
unsigned int XImage::GetBitCount () const
{
	return m_nBitCount ;
}
unsigned int XImage::GetColorCount () const
{
	return m_nColorCount ;
}
void XImage::GetRgb(unsigned int row, unsigned int col,
                          XUBYTE1 &red, XUBYTE1 &green, XUBYTE1 &blue) const
{
	if (row >= m_nImageHeight && col >= m_nImageWidth) {
		XImageException::Throw(XIMG_ERR_ROW_COLMN_OUT_OF_RANGE);
	}

	switch (m_nBitCount)
	{
		unsigned int index ;
		unsigned int offset ;
		case 1:
		offset = col / 8  ;
		index = (((*this)[row][offset] >> (7 - (col % 8))) & 0x1) ;
		red = m_pColorMap [index].red ;
		green = m_pColorMap [index].green ;
		blue = m_pColorMap [index].blue ;
		break ;
	case 2:
		offset = col / 4 ;
		index = (((*this)[row][offset] >> (2 * (3 - (col % 4)))) & 0x3) ;
		red = m_pColorMap [index].red ;
		green = m_pColorMap [index].green ;
		blue = m_pColorMap [index].blue ;
		break ;
	case 4:
		offset = col / 2 ;
		if (col % 2 == 0)
		  index = ((*this)[row][offset] & 0xF0) >> 4 ;
		else
		  index = ((*this)[row][offset] & 0x0F) ;

		red = m_pColorMap [index].red ;
		green = m_pColorMap [index].green ;
		blue = m_pColorMap [index].blue ;
		break ;

	case 8:
		red = m_pColorMap [(*this)[row][col]].red ;
		green = m_pColorMap [(*this)[row][col]].green ;
		blue = m_pColorMap [(*this)[row][col]].blue ;
		break ;
	default:
		if( m_nBitCount >= 24 ) {
			int nRoundSize = m_nBitCount / 8;
			red =   (*this)[row][nRoundSize * col + REDOFFSET] ;
			green = (*this)[row][nRoundSize * col + GREENOFFSET] ;
			blue =  (*this)[row][nRoundSize * col + BLUEOFFSET] ;
		} else {
			XImageException::Throw(XIMG_ERR_UNSUPPORT_BITCOUNT);
		}
		break ;
	}
}
void XImage::EightBitQuantization(const XImage &image)
{
	if (image.m_nBitCount != 24) {
		*this = image ;
		return ;
	}

	m_pProgressFunction = image.m_pProgressFunction ;
	m_pProgressData = image.m_pProgressData ;

	this->SetSize(256, 8, image.m_nImageWidth, image.m_nImageHeight) ;


	m_pCcolorUsage = new ColorUsageTable ;
	::memset (m_pCcolorUsage, 0, sizeof (*m_pCcolorUsage)) ;
	m_pColorAreas = new ColorArea [256] ;

	try {
		FindColorUsage(image) ;
	} catch (XImageException *e) {
		FreeColorQuantizationData () ;
		throw e;
	} catch (...) {
		FreeColorQuantizationData () ;
		XImageException::Throw( XIMG_ERR_ROW_UNKOWN );
	}


	m_pColorAreas [0].m_ColorValues [REDOFFSET].m_low = 0 ;
	m_pColorAreas [0].m_ColorValues [REDOFFSET].m_high = 255 ;
	m_pColorAreas [0].m_ColorValues [GREENOFFSET].m_low = 0 ;
	m_pColorAreas [0].m_ColorValues [GREENOFFSET].m_high = 255 ;
	m_pColorAreas [0].m_ColorValues [BLUEOFFSET].m_low = 0 ;
	m_pColorAreas [0].m_ColorValues [BLUEOFFSET].m_high = 255 ;
	m_pColorAreas [0].m_nPixelCount = m_nImageHeight * m_nImageWidth ;
	m_pColorAreas [0].m_nColorCount = m_pCcolorUsage->m_nColorCount ;
	m_nColorAreaCount = 1 ;


	SplitAreaInHalf (7, // Depth
			  0, // Retry Count
			  0, // Area Number
			  REDOFFSET) ; // Split Color

	while (m_nColorAreaCount < 256) {
		int cb = 0 ;
		unsigned int value
				= SQUARE (m_pColorAreas [cb].m_ColorValues [REDOFFSET].m_high -
						  m_pColorAreas [cb].m_ColorValues [REDOFFSET].m_low)
				+ SQUARE (m_pColorAreas [cb].m_ColorValues [GREENOFFSET].m_high -
						  m_pColorAreas [cb].m_ColorValues [GREENOFFSET].m_low)
				+ SQUARE (m_pColorAreas [cb].m_ColorValues [BLUEOFFSET].m_high -
						  m_pColorAreas [cb].m_ColorValues [BLUEOFFSET].m_low)
				* m_pColorAreas [cb].m_nColorCount ;

		for (unsigned int ii = 1 ; ii < m_nColorAreaCount ; ++ ii) {
			if (m_pColorAreas [ii].m_nColorCount > 1) {
				unsigned int newvalue
							= SQUARE (m_pColorAreas [ii].m_ColorValues [REDOFFSET].m_high -
									  m_pColorAreas [ii].m_ColorValues [REDOFFSET].m_low)
							+ SQUARE (m_pColorAreas [ii].m_ColorValues [GREENOFFSET].m_high -
									  m_pColorAreas [ii].m_ColorValues [GREENOFFSET].m_low)
							+ SQUARE (m_pColorAreas [ii].m_ColorValues [BLUEOFFSET].m_high -
									  m_pColorAreas [ii].m_ColorValues [BLUEOFFSET].m_low)
							* m_pColorAreas [ii].m_nColorCount ;
				if (newvalue > value) {
				  value = newvalue ;
				  cb = ii ;
				}
			}
		}
		if (m_pColorAreas [cb].m_nColorCount == 1)
			break ;


		SplitAreaInHalf (0, // Depth
					0, // Retry Count
					cb, // Area Number
					LargestColorRange (m_pColorAreas [cb])) ; // Split Color
	}

	for (unsigned int ii = 0 ; ii < m_nColorAreaCount ; ++ ii) {
		CreateColor (ii) ;
	}
	try {
		QuantizeSourceImage (image) ;
	} catch (XImageException *e) {
		FreeColorQuantizationData () ;
		throw e;
	}
	catch (...) {
		FreeColorQuantizationData () ;
		XImageException::Throw( XIMG_ERR_ROW_UNKOWN );
	}
	FreeColorQuantizationData () ;
}
void XImage::SetProgressFunction (
                              XIMAG_PROGRESSFUNCTION function,
                              void *data)
{
	m_pProgressFunction = function;
	m_pProgressData = data;
}
unsigned int XImage::GetBytesPerRow () const
{
	return m_nRowWidth ;
}

unsigned int XImage::OffsetYPos(unsigned int nYIndex)
{
	return GetHeight () - nYIndex - 1 ;
}
unsigned int XImage::OffsetXPos(unsigned int nXIndex)
{
	switch( GetBitCount() )
	{
	case 2:
	case 4:
	case 8:
		return nXIndex;
	default:
		if( GetBitCount() >= 24 ) {
			int nRoundSize = GetBitCount() / 8;
			return nXIndex * nRoundSize;
		} else {
			return nXIndex;
		}
	}
	return nXIndex;
}

XUBYTE1 * XImage::OffsetOf(int x, int y) const
{
	if( x<0 || x >= GetWidth() )
		return NULL;
	if( y<0 || y >= GetHeight() )
		return NULL;
	switch( GetBitCount() )
	{
	case 1:
		return m_pImageData + m_nRowWidth * (GetHeight() - y - 1) + x/8;
		break;
	case 4:
		return m_pImageData + m_nRowWidth * (GetHeight() - y - 1) + x/2;
		break;
	case 8:
		return m_pImageData + m_nRowWidth *  (GetHeight() - y - 1) + x;
		break;
	default:
		if( GetBitCount() >= 24 ) {
			int nRoundSize = GetBitCount() / 8;
//			int y = m_nRowWidth;
//			int k = (GetHeight() - y - 1) + nRoundSize * x;
			return m_pImageData + m_nRowWidth *  (GetHeight() - y - 1) + nRoundSize * x;
		}
	}
	return NULL;
}
bool XImage::SetPixel(int x, int y, int r, int g, int b)
{
	XUBYTE1* p = OffsetOf(x,y);
	if( p == NULL ) return false;
	if(  GetBitCount() < 24 )
		return false;
	*(p++) = b;
	*(p++) = g;
	*(p)   = r;
	return true;
}
bool XImage::SetPixelNo(int x, int y, int no)
{
	XUBYTE1* p = OffsetOf(x,y);

	if( p == NULL )
		return false;

	switch(GetBitCount())
	{
	case 24:
		return false;	//	サポートしていない
	case 8:
		if( no < 0 || no > 255 )
			return false;
		*p = no;
		return true;
	case 4:
		if( no < 0 || no > 15 )
			return false;
		if(x%2==0)
		{		// 前
			*p &= 0x0F;
			*p |= (no<<4);
		}
		else
		{			// 後ろ
			*p &= 0xF0;
			*p |= no;
		}
		return true;
	case 1:
		if( no < 0 || no > 1 )
			return false;

		XBYTE1 ptn = 0x80 >> (x%8);
		if(no==0)
			*p &= ~ptn;		// 0に
		else
			*p |= ptn;		// 1に
		return true;
	}
	return false;
}
XUBYTE4 XImage::GetPixel(int x, int y) const
{
	XUBYTE1* p = OffsetOf(x,y);

	if( p==NULL )
		return (XUBYTE4)-1;
	switch( GetBitCount() )
	{
	case  1:	
	case  4:
	case  8:
		{
			int no = GetPixelNo(x, y);
			if( no == -1 )
				return (XUBYTE4)-1;
			return XImgTools::ColorMapEntry2UBYTE4( GetColorMap(no) );
		}
		break;
	default:
		if( GetBitCount() >= 24 ) {
			return XImgTools::__RGB(p[2],p[1],p[0]);
		}
		break;
	}
	return (XUBYTE4)-1;
}
XUBYTE4 XImage::GetPixelNo(int x, int y) const 
{
	XUBYTE1* p = OffsetOf(x,y);
	
	if( p == NULL ) {
		return (XUBYTE4)-1;
	}

	switch( GetBitCount() )
	{
	case  8:
		return *p;
		break;
	case  4:
		if( x % 2 ) 
			return (*p)&0x0F;
		else
			return ((*p)>>4)&0x0F;
		break;
	case  1:
		return ((*p)>>(7-x%8))&1;
		break;
	}
	return (XUBYTE4)-1;
}

void XImage::DoCopy(const XImage &source)
{
	m_pProgressFunction = source.m_pProgressFunction;
	m_pProgressData = source.m_pProgressData;


	m_nBitCount = source.m_nBitCount ;
	m_nImageWidth = source.m_nImageWidth ;
	m_nImageHeight = source.m_nImageHeight ;
	m_nColorCount = source.m_nColorCount ;
	m_pColorMap = 0 ;
	m_pImageData = 0 ;

	m_pCcolorUsage = 0 ;
	m_pColorAreas = 0 ;
	m_nColorAreaCount = 0 ;

	// Only copy the image data if the size values are valid.
	if (m_nImageWidth > 0 && m_nImageHeight > 0 && m_nBitCount > 0
	  && ( m_nBitCount == 24 || 
	       m_nBitCount == 32 || 
		   m_nColorCount != 0))
	{
		unsigned int bitwidth ;
		unsigned int bytecount ;
		switch (m_nBitCount)
		{
		case 1:
		case 2:
		case 4:
		case 8:
			m_pColorMap = new ColorMapEntry [m_nColorCount] ;
			memcpy (m_pColorMap,
				  source.m_pColorMap,
				  sizeof (ColorMapEntry) * m_nColorCount) ;
			bitwidth = m_nBitCount * m_nImageWidth ;
			m_nRowWidth = RoundRow ((bitwidth + 7)/8) ;

			bytecount = m_nRowWidth * m_nImageHeight ;
			m_pImageData = new XUBYTE1[bytecount] ;
			::memcpy (m_pImageData, source.m_pImageData, bytecount) ;
			break ;

		case 24:
			m_nRowWidth = RoundRow (3 * m_nImageWidth) ;
			m_pImageData = new XUBYTE1[m_nRowWidth * m_nImageHeight] ;
			::memcpy (m_pImageData, source.m_pImageData, m_nRowWidth * m_nImageHeight) ;
			break ;
		default:
			if( GetBitCount() >= 24 ) {
				int nRoundSize = GetBitCount() / 8;

				m_nRowWidth = RoundRow (nRoundSize * m_nImageWidth) ;
				m_pImageData = new XUBYTE1[m_nRowWidth * m_nImageHeight] ;
				::memcpy (m_pImageData, source.m_pImageData, m_nRowWidth * m_nImageHeight) ;
			} else {
				if (m_nImageWidth != 0 ||m_nImageHeight != 0)
					XImageException::Throw( XIMG_ERR_UNSUPPORT_BITCOUNT );
			}
		}
	}
}
void XImage::ClearData ()
{
	m_nBitCount = 0 ;
	m_nImageWidth = 0 ;
	m_nImageHeight = 0 ;
	m_nColorCount = 0 ;
	m_pColorMap = 0 ;
	m_pImageData = 0 ;
	m_pCcolorUsage = 0 ;
	m_pColorAreas = 0 ;
	m_nColorAreaCount = 0 ;
}
int XImage::LargestColorRange(ColorArea &area)
{
	unsigned int deltared   = area.m_ColorValues [REDOFFSET].m_high
							- area.m_ColorValues [REDOFFSET].m_low ;
	unsigned int deltagreen = area.m_ColorValues [GREENOFFSET].m_high
							- area.m_ColorValues [GREENOFFSET].m_low ;
	unsigned int deltablue  = area.m_ColorValues [BLUEOFFSET].m_high
							- area.m_ColorValues [BLUEOFFSET].m_low ;

	if (deltared >= deltagreen && deltared >= deltablue)
		return REDOFFSET ;

	if (deltablue >= deltagreen && deltablue >= deltared)
		return BLUEOFFSET ;

	return GREENOFFSET ;
}
void XImage::AddColor(XUBYTE1 red, XUBYTE1 green, XUBYTE1 blue)
{
	ColorUsage *entry = new ColorUsage ;
	::memset (entry, 0, sizeof (*entry)) ;
	entry->m_nUsage = 1 ;
	entry->m_cColors [REDOFFSET] = red ;
	entry->m_cColors [GREENOFFSET] = green ;
	entry->m_cColors [BLUEOFFSET] = blue ;

	// Add the new entry to each hash chain.
	if (m_pCcolorUsage->m_pLists[red][REDOFFSET] == 0) {
		m_pCcolorUsage->m_pLists[red][REDOFFSET] = entry ;
	} else {
		entry->m_pNext[REDOFFSET] = m_pCcolorUsage->m_pLists[red][REDOFFSET] ;
		m_pCcolorUsage->m_pLists[red][REDOFFSET] = entry ;
	}
	if (m_pCcolorUsage->m_pLists[green][GREENOFFSET] == 0) {
		m_pCcolorUsage->m_pLists[green][GREENOFFSET] = entry ;
	} else {
		entry->m_pNext [GREENOFFSET] = m_pCcolorUsage->m_pLists[green][GREENOFFSET] ;
		m_pCcolorUsage->m_pLists[green][GREENOFFSET] = entry ;
	}
	if (m_pCcolorUsage->m_pLists[blue][BLUEOFFSET] == 0) {
		m_pCcolorUsage->m_pLists[blue][BLUEOFFSET] = entry ;
	} else {
		entry->m_pNext [BLUEOFFSET] = m_pCcolorUsage->m_pLists[blue][BLUEOFFSET] ;
		m_pCcolorUsage->m_pLists[blue][BLUEOFFSET] = entry ;
	}
	++m_pCcolorUsage->m_nColorCount;
}
void XImage::SplitAreaInHalf(unsigned int depth,
                             unsigned int retrydepth,
                             unsigned int areaid,
                             unsigned int splitcolor)
{
	if (m_pColorAreas [areaid].m_nColorCount == 1) {
		return ;
	} else
	if (m_pColorAreas [areaid].m_ColorValues [splitcolor].m_high
		   == m_pColorAreas [areaid].m_ColorValues [splitcolor].m_low)
	{
		if (retrydepth < 2) {
			SplitAreaInHalf (depth, retrydepth + 1, areaid, (splitcolor + 1) % 3) ;
		}
		return ;
	}

	unsigned int c1 = (splitcolor + 1) % 3 ;
	unsigned int c2 = (splitcolor + 2) % 3 ;
	unsigned int splitsize = m_pColorAreas [areaid].m_nPixelCount / 2 ;
	unsigned int splitpixelcount = 0 ;
	unsigned int splitcolorcount = 0 ;
	unsigned int newlimit ;
	unsigned int newpixelcount ;
	unsigned int newcolorcount ;


	for(newlimit = m_pColorAreas [areaid].m_ColorValues [splitcolor].m_low ;
	    newlimit <= m_pColorAreas [areaid].m_ColorValues [splitcolor].m_high ;
	    ++newlimit)
	{
		newpixelcount = 0 ;
		newcolorcount = 0 ;
		for(ColorUsage *entry = m_pCcolorUsage->m_pLists[newlimit][splitcolor] ;
			entry != 0 ;
			entry = entry->m_pNext [splitcolor])
		{
			if(entry->m_cColors [c1] >= m_pColorAreas [areaid].m_ColorValues [c1].m_low
				&& entry->m_cColors [c1] <= m_pColorAreas [areaid].m_ColorValues [c1].m_high
				&& entry->m_cColors [c2] >= m_pColorAreas [areaid].m_ColorValues [c2].m_low
				&& entry->m_cColors [c2] <= m_pColorAreas [areaid].m_ColorValues [c2].m_high)
			{
				newpixelcount += entry->m_nUsage ;
				++ newcolorcount ;
			}
		}

		if(newcolorcount == m_pColorAreas [areaid].m_nColorCount) {
			if (retrydepth < 2) {
				SplitAreaInHalf (depth, retrydepth + 1, areaid, (splitcolor + 1) % 3) ;
			}
			return ;
		} else
		if (newcolorcount >m_pColorAreas[areaid].m_nColorCount) {
			XImageException::Throw( XIMG_ERR_QUATIZATION_AREA_COUNT_INVALID );
		}

		if (splitpixelcount + newpixelcount >= splitsize) {
			if (splitpixelcount + newpixelcount != m_pColorAreas [areaid].m_nPixelCount) {
				splitpixelcount += newpixelcount ;
				splitcolorcount += newcolorcount ;
			} else {
				--newlimit ;
			}
			m_pColorAreas [m_nColorAreaCount] = m_pColorAreas [areaid] ;
			m_pColorAreas [m_nColorAreaCount].m_nPixelCount = m_pColorAreas [areaid].m_nPixelCount - splitpixelcount ;
			m_pColorAreas [m_nColorAreaCount].m_nColorCount = m_pColorAreas [areaid].m_nColorCount - splitcolorcount ;
			m_pColorAreas [m_nColorAreaCount].m_ColorValues [splitcolor].m_low = newlimit + 1 ;
			++ m_nColorAreaCount ;

			m_pColorAreas [areaid].m_ColorValues [splitcolor].m_high = newlimit ;
			m_pColorAreas [areaid].m_nPixelCount = splitpixelcount ;
			m_pColorAreas [areaid].m_nColorCount = splitcolorcount ;

			if (depth > 0) {
				SplitAreaInHalf (depth - 1, 0, m_nColorAreaCount - 1, LargestColorRange (m_pColorAreas [m_nColorAreaCount-1])) ;
				SplitAreaInHalf (depth - 1, 0, areaid, LargestColorRange (m_pColorAreas [areaid])) ;
			}
			return ;
		} else {
			splitpixelcount += newpixelcount ;
			splitcolorcount += newcolorcount ;
		}
	}
	XImageException::Throw( XIMG_ERR_QUATIZATION_AREA_COUNT_INVALID );
}
void XImage::CreateColor(unsigned int color)
{
	unsigned int red = 0 ;
	unsigned int green = 0 ;
	unsigned int blue = 0 ;

	const int c0 = REDOFFSET ;
	const int c1 = GREENOFFSET ;
	const int c2 = BLUEOFFSET ;

	unsigned int itemcount = 0 ;
	for(unsigned int cc = m_pColorAreas [color].m_ColorValues [c0].m_low ;
			cc <= m_pColorAreas [color].m_ColorValues [c0].m_high ;
			++ cc)
	{
		for(ColorUsage *entry = m_pCcolorUsage->m_pLists [cc][c0] ;
				entry != 0 ;
				entry = entry->m_pNext [c0])
		{
			if (entry->m_cColors [c1] >= m_pColorAreas [color].m_ColorValues [c1].m_low
				&& entry->m_cColors [c1] <= m_pColorAreas [color].m_ColorValues [c1].m_high
				&& entry->m_cColors [c2] >= m_pColorAreas [color].m_ColorValues [c2].m_low
				&& entry->m_cColors [c2] <= m_pColorAreas [color].m_ColorValues [c2].m_high)
			{
				red += entry->m_cColors [REDOFFSET] * entry->m_nUsage ;
				green += entry->m_cColors [GREENOFFSET] * entry->m_nUsage ;
				blue += entry->m_cColors [BLUEOFFSET] * entry->m_nUsage ;
				itemcount += entry->m_nUsage ;
			}
		}
	}

	if (itemcount == 0) {
		return ;
	}

	m_pColorMap [color].red = (red + itemcount/2) / itemcount ;
	m_pColorMap [color].green = (green + itemcount/2) / itemcount ;
	m_pColorMap [color].blue = (blue + itemcount/2) / itemcount ;
}
XImage::ColorUsage* XImage::FindColor(XUBYTE1 red,
                                      XUBYTE1 green,
                                      XUBYTE1 blue)
{
	if(m_pCcolorUsage->m_pLists [red][REDOFFSET] == 0
		|| m_pCcolorUsage->m_pLists [green][GREENOFFSET] == 0
		|| m_pCcolorUsage->m_pLists [blue][BLUEOFFSET] == 0)
	{
		return 0 ;
	}

	for (ColorUsage *entry = m_pCcolorUsage->m_pLists[red][REDOFFSET] ;
			entry != 0 ;
			entry = entry->m_pNext[REDOFFSET])
	{
		if (entry->m_cColors[GREENOFFSET] == green && entry->m_cColors[BLUEOFFSET] == blue) {
			return entry ;
		}
	}
	return 0 ;
}
void XImage::FindColorUsage(const XImage &image)
{
	const unsigned int climit = m_nImageWidth * 3 ;
	for (unsigned int rr = 0 ; rr < m_nImageHeight ; ++ rr) {
		XUBYTE1 *rowdata = &image.m_pImageData[rr * image.m_nRowWidth] ;
		for (unsigned int cc = 0 ; cc < climit ; cc += 3 ) {
			XUBYTE1 red, green, blue ;
			
			red = rowdata [cc + REDOFFSET] ;
			green = rowdata [cc + GREENOFFSET]  ;
			blue = rowdata [cc + BLUEOFFSET] ;

			
			ColorUsage *entry = FindColor(red, green, blue) ;
			if(entry == 0) {
				AddColor(red, green, blue) ;
			} else {
				++ entry->m_nUsage ;
			}
		}
		CallProgressFunction (100 * rr/m_nImageHeight, 1, 2) ;
	}
	CallProgressFunction (100, 1, 2) ;
}
void XImage::FreeColorQuantizationData()
{
	for (unsigned int ii = 0 ; ii < 256 ; ++ ii) {
		ColorUsage *m_pNext ;
		for (ColorUsage *entry = m_pCcolorUsage->m_pLists [ii][REDOFFSET] ;
			 entry != 0 ;
			 entry = m_pNext)
		{
			m_pNext = entry->m_pNext [REDOFFSET] ;
			delete entry ;
		}
	}

	delete m_pCcolorUsage ; m_pCcolorUsage = 0 ;
	delete [] m_pColorAreas ;
}
unsigned int XImage::QuantizedColor(XUBYTE1 red, XUBYTE1 green, XUBYTE1 blue)
{
	for (unsigned int color = 0 ; color < m_nColorAreaCount ; ++ color)
	{
		if (red >= m_pColorAreas [color].m_ColorValues[REDOFFSET].m_low
			&& red <= m_pColorAreas [color].m_ColorValues[REDOFFSET].m_high
			&& green >= m_pColorAreas [color].m_ColorValues[GREENOFFSET].m_low
			&& green <= m_pColorAreas [color].m_ColorValues[GREENOFFSET].m_high
			&& blue >= m_pColorAreas [color].m_ColorValues[BLUEOFFSET].m_low
			&& blue <= m_pColorAreas [color].m_ColorValues[BLUEOFFSET].m_high)
		{
			return color ;
		}
	}
	XImageException::Throw( XIMG_ERR_COLOR_NO_QUATIZ );
	return 0;
}
void XImage::QuantizeSourceImage(const XImage &src)
{
	for (unsigned int rr = 0 ; rr < m_nImageHeight ; ++ rr) {
		CallProgressFunction (rr * 100 / m_nImageHeight, 2, 2) ;

		XUBYTE1 *srcdata = &src.m_pImageData [rr * src.m_nRowWidth] ;
		XUBYTE1 *dstdata = &m_pImageData [rr * m_nRowWidth] ;
		for (unsigned int cc = 0 ; cc < m_nImageWidth ; ++ cc) {
			XUBYTE1 red = srcdata [3 * cc + REDOFFSET] ;
			XUBYTE1 green = srcdata [3 * cc + GREENOFFSET]  ;
			XUBYTE1 blue = srcdata [3 * cc + BLUEOFFSET] ;
			dstdata [cc] = QuantizedColor(red, green, blue) ;
		}
	}
	CallProgressFunction (100, 2, 2) ;
	return ;
}
void XImage::CallProgressFunction(	unsigned int percent,
									unsigned int pass,
									unsigned int passcount)
{
	if (m_pProgressFunction == 0)
		return ;
	if (percent > 100)
		percent = 100 ;

	bool cancel = false ;
	m_pProgressFunction (*this, m_pProgressData, pass, passcount, percent, cancel) ;
	if( cancel )
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL );
}



/*
//////////////////////////////////////////////////////////////////////////////
//
//	XImgTools
//
*/
XUBYTE1 XImgTools::_GetRvalue(XUBYTE4 rgb)
{
	XUBYTE4    ds;
	ds = rgb & 0xff;
	return (XUBYTE1)ds;
}
XUBYTE1 XImgTools::_GetGvalue(XUBYTE4 rgb)
{
	XUBYTE4    ds;
	ds = rgb & 0xff00;
	ds >>=8;
	return (XUBYTE1)ds;
}
XUBYTE1 XImgTools::_GetBvalue(XUBYTE4 rgb)
{
	XUBYTE4    ds;
	ds = rgb & 0xff0000;
	ds >>=16;
	return (XUBYTE1)ds;
}
XUBYTE4 XImgTools::ColorMapEntry2UBYTE4(const XImage::ColorMapEntry &rgb)
{
	return  (rgb.blue << 16) | (rgb.green << 8) | (rgb.red);
}
XUBYTE4 XImgTools::GetColorDistance(const XUBYTE4 c1, const XUBYTE4 c2)
{
	long dR = _GetRvalue(c1)-_GetRvalue(c2);
	long dG = _GetGvalue(c1)-_GetGvalue(c2);
	long dB = _GetBvalue(c1)-_GetBvalue(c2);
	return dR*dR+dG*dG+dB*dB;

}
XUBYTE4	XImgTools::FindNearColorNo(const XImage src, int colnum, XCOLORREF c )
{
	XUBYTE4 min_no = 0;
	long mindist = GetColorDistance(ColorMapEntry2UBYTE4( src.GetColorMap(0) ), c);
	for(int i=1; i<colnum; i++)
	{
		long dist = GetColorDistance(ColorMapEntry2UBYTE4(  src.GetColorMap(i) ), c);
		if( mindist > dist )
		{
			mindist = dist;
			min_no = i;
		}
	}
	return min_no;
}
double	XImgTools::GetGrayScale(XUBYTE4 col)
{
	return double(_GetRvalue(col)) / 255.0 * 0.299
		 + double(_GetGvalue(col)) / 255.0 * 0.587
		 + double(_GetBvalue(col)) / 255.0 * 0.114;
}
// RGBの代わり
XUBYTE4 XImgTools::__RGB(XUBYTE4 r, XUBYTE4 g, XUBYTE4 b)
{
	XUBYTE4 rr = (XUBYTE4)r;
	XUBYTE4 gg = (XUBYTE4)g;
	XUBYTE4 bb = (XUBYTE4)b;
	return (bb << 16 ) | (gg << 8) | rr;
}
XUBYTE4 XImgTools::GetColorCount24(const XImage &src)
{
	int       x, y;
	XUBYTE4    dwBit,dwByte,dwCount=0,dwPixel=0;
	XUBYTE1*   fColors;
	XBuff     buff;

	fColors = (XUBYTE1*)buff.Alloc((256 * 256 * 33) * sizeof(XUBYTE1));
	::memset(fColors, 0, (256 * 256 * 33) * sizeof(XUBYTE1));
	for( y=0 ; y < src.GetHeight() ; y++ ) {
		for( x = 0 ; x < src.GetWidth() ; x++ ) {
			/* (y,x)の色をdwPixelに３２ビットで格納 */
			dwPixel = src.GetPixel(x, y);

			/* dwPixelビット目の位置を計算 */
			dwByte = dwPixel / 8;
			dwBit  = dwPixel % 8;

			/* フラグ領域のdwPixelビット目を検査 */
			if ( (fColors[dwByte] & (1 << dwBit) )==0)
			{
				/* フラグ領域のdwPixelビット目を立てる */
				fColors[ dwByte ] = fColors[ dwByte ]|(1 << dwBit);
				 /* 色数カウンタ更新 */
				dwCount++;
			}
		}
	}
	return dwCount;
}
void XImgTools::MakeDefPallet(XImage &src)
{
	int i, j , k;

	for (i=0;i<6;i++)
	{
		for (j=0;j<6;j++)
		{
			for (k=0;k<6;k++)
			{
				src.GetColorMap(i*36+j*6+k).red= k*51 ;
				src.GetColorMap(i*36+j*6+k).green = j*51;
				src.GetColorMap(i*36+j*6+k).blue = i*51;
			}
		}
	}

	for (i=1;i<32;i++)
	{
		src.GetColorMap(i+223).red=i*8;
		src.GetColorMap(i+223).green=i*8;
		src.GetColorMap(i+223).blue=i*8;
	}

	src.GetColorMap(255).red=255;
	src.GetColorMap(255).green=255;
	src.GetColorMap(255).blue=255;

}
bool XImgTools::ReversImage(XImage &src)
{
	XUBYTE4 r, g, b;
	if( src.GetBitCount() >= 24 )
	{
		for (int nnY = 0 ; nnY < src.GetHeight () ; ++nnY)
		{
			for (int nnX = 0 ; nnX < src.GetWidth () ; ++ nnX)
			{
				XUBYTE4 rr = src.GetPixel(nnX, nnY);
				r = _GetRvalue(rr);
				g = _GetGvalue(rr);
				b = _GetBvalue(rr);
				r = ~r;
				g = ~g;
				b = ~b;
				src.SetPixel(nnX, nnY, r, g, b);
			}
		}
	}
	else
	{
		for( int nColor = 0; nColor < src.GetColorCount(); ++nColor)
		{
			r = src.GetColorMap(nColor).red;
			g = src.GetColorMap(nColor).green;
			b = src.GetColorMap(nColor).blue;
			r = ~r;
			g = ~g;
			b = ~b;
			src.GetColorMap(nColor).red     = r;
			src.GetColorMap(nColor).green   = g;
			src.GetColorMap(nColor).blue    = b;
		}
	}
	return true;
}
bool XImgTools::ColorConvert(XImage &src, int rc, int gc, int bc)
{
	XBYTE4 r, g, b;
	if( src.GetBitCount() >= 24 )
	{
		for (int nnY = 0 ; nnY < src.GetHeight () ; ++nnY)
		{
			for (int nnX = 0 ; nnX < src.GetWidth () ; ++ nnX)
			{
				XUBYTE4 rr = src.GetPixel(nnX, nnY);
				r = _GetRvalue(rr);
				g = _GetGvalue(rr);
				b = _GetBvalue(rr);

				r += rc;
				g += gc;
				b += bc;
				if( r < 0 )
					r = 0;
				if( r >= 255 )
					r = 255;
				if( g < 0 )
					g = 0;
				if( g >= 255 )
					g = 255;
				if( b < 0 )
					b = 0;
				if( b >= 255 )
					b = 255;

				src.SetPixel(nnX, nnY, r, g, b);
			}
		}
	}
	else
	{
		for( int nColor = 0; nColor < src.GetColorCount(); ++nColor)
		{
			r = src.GetColorMap(nColor).red;
			g = src.GetColorMap(nColor).green;
			b = src.GetColorMap(nColor).blue;
			
			r += rc;
			g += gc;
			b += bc;
			if( r < 0 )
				r = 0;
			if( r >= 255 )
				r = 255;
			if( g < 0 )
				g = 0;
			if( g >= 255 )
				g = 255;
			if( b < 0 )
				b = 0;
			if( b >= 255 )
				b = 255;

			src.GetColorMap(nColor).red     = r;
			src.GetColorMap(nColor).green   = g;
			src.GetColorMap(nColor).blue    = b;
		}
	}
	return true;
}

