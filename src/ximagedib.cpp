#include "xlib/image/ximagedib.h"
#include "xlib/xfunctions.h"


#define XBI_RGB        0
#define XBI_RLE8       1
#define XBI_RLE4       2
#define XBI_BITFIELDS  3





typedef struct tagXBITMAPCOREHEADER {
		unsigned long   bcSize;                 /* used to get to color table */
		XUBYTE2    bcWidth;
		XUBYTE2    bcHeight;
		XUBYTE2    bcPlanes;
		XUBYTE2    bcBitCount;
} XBITMAPCOREHEADER;

typedef struct tagXBITMAPINFOHEADER{
		unsigned long      biSize;
		long       biWidth;
		long       biHeight;
		XUBYTE2       biPlanes;
		XUBYTE2       biBitCount;
		unsigned long      biCompression;
		unsigned long      biSizeImage;
		long       biXPelsPerMeter;
		long       biYPelsPerMeter;
		unsigned long      biClrUsed;
		unsigned long      biClrImportant;
} XBITMAPINFOHEADER;
typedef struct tagXRGBTRIPLE {
        XUBYTE1    rgbtBlue;
        XUBYTE1    rgbtGreen;
        XUBYTE1    rgbtRed;
} XRGBTRIPLE;
typedef struct tagXRGBQUAD {
        XUBYTE1    rgbBlue;
        XUBYTE1    rgbGreen;
        XUBYTE1    rgbRed;
        XUBYTE1    rgbReserved;
} XRGBQUAD;
/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImageDIB::Decoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImageDIB::Decoder::Decoder()
{
}
XImageDIB::Decoder::Decoder(const XImageDIB::Decoder& source)
	:	XImageDecoder(source) 
{
}
XImageDIB::Decoder::~Decoder()
{
}
XImageDIB::Decoder& XImageDIB::Decoder::operator=(const XImageDIB::Decoder& source)
{
	XImageDecoder::operator=(source);
	return *this;
}
void XImageDIB::Decoder::ReadImageFile(const char* lpszFileName, XImage& image)
{
	XImageStream inputstream ;
	inputstream.OpenA(lpszFileName, true, false, false, true);
	ReadImage(inputstream, image);
}
void XImageDIB::Decoder::ReadImage(XImageStream& strm, XImage& image)
{
	unsigned int ii;
	bool os2format;
	unsigned int bytesread = 0;
	XUBYTE2		bfType = 0;
	XUBYTE2		tmp2 = 0;
	XUBYTE4		nHeadSize = 0;
	XUBYTE4		nOffBits = 0;
	const XUBYTE2 signature = 'B' | ('M' << 8) ;
	static int nFileHeaderSize = 14;

	// ビットマップのファイル署名が先頭にあるか？
	if( strm.Read((unsigned char*) reinterpret_cast<char *>(&bfType), sizeof(bfType)) == false ) {
		XImageException::Throw( XIMG_ERR_END_OF_STREAM );
	}
	if( bfType != signature ) {
		XImageException::Throw( XIMG_ERR_NOT_BMP_FILE );
	}

	// サイズを取得
	if( strm.Read((unsigned char*) reinterpret_cast<char *>(&nHeadSize), sizeof(nHeadSize)) == false ) {
		XImageException::Throw( XIMG_ERR_END_OF_STREAM );
	}
	// ヘッダーの予約部を取得
	if( strm.Read((unsigned char*) reinterpret_cast<char *>(&tmp2), sizeof(tmp2)) == false ||
		strm.Read((unsigned char*) reinterpret_cast<char *>(&tmp2), sizeof(tmp2)) == false) {
		XImageException::Throw( XIMG_ERR_END_OF_STREAM );
	}
	// オフセットを取得
	if( strm.Read((unsigned char*) reinterpret_cast<char *>(&nOffBits), sizeof(nOffBits)) == false ) {
		XImageException::Throw( XIMG_ERR_END_OF_STREAM );
	}
	bytesread += nFileHeaderSize;

	// ヘッダサイズを読み込み
	XUBYTE4 headersize = 0;
	if( strm.Read((unsigned char*) reinterpret_cast<char *>(&headersize), sizeof(headersize)) == false ) {
		XImageException::Throw( XIMG_ERR_END_OF_STREAM );
	}

	// ヘッダ情報を読み込みます
	unsigned long width ;
	long height ;
	unsigned int bitcount ;
	unsigned int compression ;
	unsigned int colorcount = 0 ;
	if (headersize == sizeof(XBITMAPCOREHEADER)) {
		// OS/2 Format
		XBITMAPCOREHEADER header ;
		header.bcSize = headersize ;
		if( strm.Read((unsigned char*) reinterpret_cast<char *>(&header.bcWidth),
						sizeof (header) - sizeof (headersize)) == false ) {
			XImageException::Throw( XIMG_ERR_END_OF_STREAM );
		}
		bytesread += sizeof(header);
		width = header.bcWidth ;
		height = header.bcHeight ;
		bitcount = header.bcBitCount ;
		compression = XBI_RGB ;
		os2format = true ;
	} else
	if (headersize >= sizeof(XBITMAPINFOHEADER) ) {
		XBITMAPINFOHEADER header ;
		header.biSize = headersize ;
		if( strm.Read ((unsigned char*) reinterpret_cast<char *>(&header.biWidth),
				sizeof(header) - sizeof (headersize)) == false ) {
			XImageException::Throw( XIMG_ERR_END_OF_STREAM );
		}
		bytesread += sizeof(header);
		compression = header.biCompression ;

		width = header.biWidth ;
		height = header.biHeight ;
		bitcount = header.biBitCount ;

		for(ii = 0; ii < headersize - sizeof (XBITMAPINFOHEADER); ++ ii) {
			++bytesread ;
			XUBYTE1 data;
			if( strm.Read((unsigned char*) reinterpret_cast<char *>(&data), sizeof (data)) == false ) {
				XImageException::Throw( XIMG_ERR_END_OF_STREAM );
			}
		}
		os2format = false ;
		colorcount = header.biClrUsed ;
	} else {
		XImageException::Throw( XIMG_ERR_UNSUPPORT_BIT_MAP_HEADER );
	}

	// カラーを取得します
	if (colorcount == 0 && bitcount != 24)
		colorcount = 1 << bitcount ;

	// ヘッダ情報からビットマップのイメージの妥当せいを確かめます
	switch (bitcount)
	{
	case 1:
		if (compression != XBI_RGB)
			XImageException::Throw( XIMG_ERR_UNSUPPORT_COMPRESSION );
		break ;
	case 4:
		if (compression != XBI_RGB && compression != XBI_RLE4)
			XImageException::Throw( XIMG_ERR_UNSUPPORT_COMPRESSION );
		break ;
	case 8:
		if (compression != XBI_RGB &&  compression != XBI_RLE8)
			XImageException::Throw( XIMG_ERR_UNSUPPORT_COMPRESSION );
		break ;
	case 24:
		if (compression != XBI_RGB)
			XImageException::Throw( XIMG_ERR_UNSUPPORT_COMPRESSION );
		break ;
	default:
		XImageException::Throw( XIMG_ERR_UNSUPPORT_BITCOUNT );
	}


	// メモリの確保
	image.SetSize(colorcount, bitcount, width, height) ;

	// カラーマップの読み込み
	if (os2format) {
		// OS2用のカラーマップの作成
		for (ii = 0 ; ii < colorcount ; ++ ii) {
			XRGBTRIPLE color ;
			if( strm.Read ((unsigned char*) reinterpret_cast<char *>(&color), sizeof (color)) == false ) {
				XImageException::Throw( XIMG_ERR_END_OF_STREAM );
			}
			image.GetColorMap(ii).red = color.rgbtRed ;
			image.GetColorMap(ii).blue = color.rgbtBlue ;
			image.GetColorMap(ii).green = color.rgbtGreen ;
			bytesread += sizeof(color);
		}
	} else {
		for (ii = 0 ; ii < colorcount ; ++ ii) {
			XRGBQUAD color ;
			if( strm.Read((unsigned char*) reinterpret_cast<char *>(&color), sizeof (color)) == false ) {
				XImageException::Throw( XIMG_ERR_END_OF_STREAM );
			}
			image.GetColorMap(ii).red = color.rgbRed ;
			image.GetColorMap(ii).blue = color.rgbBlue ;
			image.GetColorMap(ii).green = color.rgbGreen ;
			bytesread += sizeof(color);
		}
	}

	if (bytesread > nOffBits) {
		XImageException::Throw( XIMG_ERR_CORUPPUT_DATA );
	}

	// 余分ビットがある意場合はスキップします。
	for (ii = bytesread ; ii < nOffBits ; ++ ii) {
		XUBYTE1 data ;
		if( strm.Read((unsigned char*) reinterpret_cast<char *>(&data), sizeof (data)) == false ) {
			XImageException::Throw( XIMG_ERR_END_OF_STREAM );
		}
	}


	CallProgressFunction(0);
	if (bitcount != 24) {
		if (compression == XBI_RGB) {	
			// 横の論理データサイズ
			unsigned int bitwidth = bitcount * width ;
			// 横の実データサイズ
			unsigned int rowwidth = (bitwidth + 7)/8 ;
			// 横の実データサイズ
			unsigned int physicalrowsize = (rowwidth + 0x3) & ~0x3 ;
			// 実サイズと論理サイズの差分
			unsigned int padsize = physicalrowsize -  rowwidth ;

			
			if(height > 0) {
				for(unsigned int ii = 0 ; ii < (unsigned int)height ; ++ ii) {
					CallProgressFunction (ii * 100 / height) ;
				
					unsigned int index = (height - ii - 1) ;
					if( strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[index][0]), rowwidth) == false ) {
						XImageException::Throw( XIMG_ERR_END_OF_STREAM );
					}

					// 実サイズと論理サイズの差分をスキップ
					static char pad [4] ;
					strm.Read ((unsigned char*) pad, padsize) ;
				}
			} else {
				for(unsigned int ii = 0 ; ii < (unsigned int)- height ; ++ ii) {
					CallProgressFunction (ii * 100 / - height) ;

					if( strm.Read((unsigned char*) reinterpret_cast<char *>(&image [ii][0]), rowwidth) == false ) {
						XImageException::Throw( XIMG_ERR_END_OF_STREAM );
					}
					// 実サイズと論理サイズの差分をスキップ
					static char pad [4] ;
					strm.Read ((unsigned char*) pad, padsize) ;
				}
			}
		} else
		if (compression == XBI_RLE8) {
			// 8bit-Encordingの処理
			unsigned int row = height - 1;     // 現在のraw
			unsigned int col = 0 ;             // 現在のcol
			bool done = false ;
			while (! strm.IsEOF () && ! done) {			
				CallProgressFunction ((height - row - 1) * 100 / height) ;

				// Structure for reading RLE commands.
				struct
				{
					XUBYTE1 count ;
					XUBYTE1 command ;
				}opcode ;

				strm.Read ((unsigned char*) reinterpret_cast<char *>(&opcode), sizeof (opcode)) ;
				if (opcode.count == 0) {
					switch(opcode.command) {
					case 0:
						--row;
						col = 0 ;
						break ;
					case 1:
						done = true ;
						break ;
					case 2:
						{
							XUBYTE1 dx ;
							XUBYTE1 dy ;
							strm.Read ((unsigned char*) reinterpret_cast<char *>(&dx), sizeof (dx)) ;
							strm.Read ((unsigned char*) reinterpret_cast<char *>(&dy), sizeof (dy)) ;
							col += dx ;
							row -= dy ;
						}
						break ;
					default:
						{
							if ((unsigned int)row >= (unsigned int)height || 
								(unsigned int)(col + opcode.command) > (unsigned int)width)
								XImageException::Throw( XIMG_ERR_CORUPPUT_DATA );

							XUBYTE1 data ;
							for(unsigned int ii = 0 ; ii < opcode.command ; ++ ii) {
								strm.Read ((unsigned char*) reinterpret_cast<char *>(&data), sizeof (data));
								image [row][col] = data ;
								++ col ;
							}
							if ((opcode.command & 1) != 0) {
								strm.Read ((unsigned char*) reinterpret_cast<char *>(&data), sizeof (data));
							}
						}
						break ;
					}
				} else {
					if ((unsigned int)row >= (unsigned int)height || 
						(unsigned int)(col + opcode.count) > (unsigned int)width) {
						XImageException::Throw( XIMG_ERR_CORUPPUT_DATA );
					}

					for(unsigned int ii = 0 ; ii < (unsigned int)opcode.count ; ++ ii) {
						image [row][col] = opcode.command ;
						++ col ;
					}
				}
			}
			if (! done) {
				XImageException::Throw( XIMG_ERR_CORUPPUT_DATA );
			}
		} else
		if (compression == XBI_RLE4) {
			unsigned int row = height - 1 ;
			unsigned int col = 0 ;
			bool done = false ;
			while(! strm.IsEOF () && ! done) {
				CallProgressFunction ((height - row - 1) * 100 / height) ;

				struct
				{
					XUBYTE1 count ;
					XUBYTE1 command ;
				} opcode ;

				strm.Read ((unsigned char*) reinterpret_cast<char *>(&opcode), sizeof (opcode)) ;
				if (opcode.count == 0) {
					switch (opcode.command)
					{
					case 0:
						-- row ;
						col = 0 ;
						break ;
					case 1:
						done = true ;
						break ;
					case 2:
						{
							XUBYTE1 dx ;
							XUBYTE1 dy ;
							strm.Read ((unsigned char*) reinterpret_cast<char *>(&dx), sizeof (dx)) ;
							strm.Read ((unsigned char*) reinterpret_cast<char *>(&dy), sizeof (dy)) ;
							col += dx ;
							row -= dy ;
						}
						break ;
					default:
						{
							XUBYTE1 data ;
							XUBYTE1 hi ;
							XUBYTE1 lo ;
							if ((unsigned int )row >= (unsigned int )height || 
								(unsigned int )(col + opcode.command) > (unsigned int )width)
								XImageException::Throw( XIMG_ERR_CORUPPUT_DATA );

							for (unsigned int ii = 0 ; ii < (unsigned int )opcode.command ; ++ ii) {
								if ((ii & 1) == 0) {
									strm.Read ((unsigned char*) reinterpret_cast<char *>(&data), sizeof (data)) ;
									lo = data & 0xF ;
									hi = (data & 0xF0) >> 4 ;
								}
								if ((col & 1) == 0) {
									if ((ii & 1) == 0) {
										image [row][col/2] = hi << 4 ;
									} else {
										image [row][col/2] = lo << 4 ;
									}
								} else {
									if ((ii & 1) == 0) {
										image [row][col/2] |= hi  ;
									} else {
										image [row][col/2] |= lo ;
									}
								}
								++ col ;
							}

							switch (opcode.command & 0x3) {
							case 1: case 2:
								strm.Read ((unsigned char*) reinterpret_cast<char *>(&data), sizeof (data)) ;
								break ;
							}
						}
						break ;
					}
				} else {
					XUBYTE1 hi = opcode.command >> 4 ;
					XUBYTE1 lo = opcode.command & 0xF ;
					if ((unsigned int)row >= (unsigned int)height || (unsigned int)(col + opcode.count) > (unsigned int)width)
						XImageException::Throw( XIMG_ERR_CORUPPUT_DATA );

					for (unsigned int ii = 0 ; ii < (unsigned int)opcode.count ; ++ ii) {
						if ((col & 1) == 0) {
							if ((ii & 1) == 0) {
								image [row][col/2] = hi << 4 ;
							} else {
								image [row][col/2] = lo << 4 ;
							}
						} else {
							if ((ii & 1) == 0) {
								image [row][col/2] |= hi  ;
							} else {
								image [row][col/2] |= lo ;
							}
						}
						++ col ;
					}
				}
			}
			if(! done) {
				XImageException::Throw( XIMG_ERR_CORUPPUT_DATA );
			}
		}
		else {
			XImageException::Throw( XIMG_ERR_UNSUPPORT_COMPRESSION );
		}
	} else {
		unsigned int physicalrowsize = (3 * width + 0x3) & ~0x3 ;
		unsigned int padsize = physicalrowsize -  3 * width ;

		if (height > 0) {
			if (XImage::BLUEOFFSET == 0 
			  && XImage::GREENOFFSET == 1
			  && XImage::REDOFFSET == 2)
			{
				for (unsigned int yy = 0 ; yy < (unsigned int)height ; ++ yy) {
					CallProgressFunction (yy * 100 / height) ;
					unsigned int index = height - yy - 1 ;
					strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[index][0]), 3 * width) ;
					static char pad [4] ;
					strm.Read ((unsigned char*) pad, padsize) ;
				}
			} else {
				for (unsigned int yy = 0 ; yy < (unsigned int)height ; ++ yy) {
					CallProgressFunction (yy * 100 / height) ;

					unsigned int index = height - yy - 1 ;
					for (unsigned int xx = 0 ; xx < 3 * width ; xx += 3) {
						strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[index][xx + XImage::BLUEOFFSET]), 1) ;
						strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[index][xx + XImage::GREENOFFSET]), 1) ;
						strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[index][xx + XImage::REDOFFSET]), 1) ;
					}
					static char pad [4] ;
					strm.Read ((unsigned char*) pad, padsize) ;
				}
			}
		} else {
			if (XImage::BLUEOFFSET == 0 
			  && XImage::GREENOFFSET == 1
			  && XImage::REDOFFSET == 2)
			{
				for (unsigned int yy = 0 ; yy < (unsigned int)height ; ++ yy) {
					CallProgressFunction (yy * 100 / -height) ;
					strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[ii][0]), 3 * width) ;
					static char pad [4] ;
					strm.Read ((unsigned char*) pad, padsize) ;
				}
			} else {
				for (unsigned int yy = 0 ; yy < (unsigned int)-height ; ++ yy) {
					CallProgressFunction (yy * 100 / -height) ;

					for (unsigned int xx = 0 ; xx < 3 * width ; xx += 3) {
						strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[ii][xx + XImage::BLUEOFFSET]), 1) ;
						strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[ii][xx + XImage::GREENOFFSET]), 1) ;
						strm.Read ((unsigned char*) reinterpret_cast<char *>(&image[ii][xx + XImage::REDOFFSET]), 1) ;
					}
					static char pad [4] ;
					strm.Read ((unsigned char*)pad, padsize) ;
				}
			}
		}
	}
	CallProgressFunction (100) ;
}

void XImageDIB::Decoder::CallProgressFunction(unsigned int percent)
{
	if (m_ProgressFunction == 0)
		return ;
	bool cancel = false ;
	m_ProgressFunction (*this, m_pProgressData, 1, 1, percent, cancel) ;
	if (cancel)
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL );
}


/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImageDIB::Encoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImageDIB::Encoder::Encoder()
{
}
XImageDIB::Encoder::Encoder(const XImageDIB::Encoder& source)
	:	XImageEncoder(source)
{

}
XImageDIB::Encoder::~Encoder()
{

}
XImageDIB::Encoder& XImageDIB::Encoder::operator=(const XImageDIB::Encoder& source)
{
	if (&source != this) {
		XImageEncoder::operator=(source);
	}
	return *this ;
}
void XImageDIB::Encoder::WriteImage(XImageStream &strm, const XImage &image)
{
	// 大きなバッファを確保する為のバッファのサイズ
	const int BUFFERSIZE = 1 << 17 ;
	XUBYTE2		Signature = 'B'|('M'<<8) ;
	XUBYTE2		tmp2 = 0;
	XUBYTE4		nHeadSize = 0;
	XUBYTE4		nOffBits = 0;
	XBuff obBuff;
	char *buffer = (char*)obBuff.Alloc(BUFFERSIZE);
	unsigned int ii ;
	static int nFileHeaderSize = 14;


	switch (image.GetBitCount ())
	{
	case 1: case 4: case 8: case 24:
		break ;
	default:
		XImageException::Throw( XIMG_ERR_UNSUPORT_BITS_PER_PIXL );
	}


	XBITMAPINFOHEADER infoheader = {
		  XFunctions::SystemToLittleEndian((XUBYTE4)sizeof (XBITMAPINFOHEADER)),
		  XFunctions::SystemToLittleEndian ((XUBYTE4)image.GetWidth ()),
		  XFunctions::SystemToLittleEndian ((XUBYTE4)image.GetHeight ()),
		  XFunctions::SystemToLittleEndian ((XUBYTE2) 1),
		  0,
		  XFunctions::SystemToLittleEndian ((XUBYTE4) XBI_RGB),
		  0,
		  0,
		  0,
		  0,
		  };
	// パレットサイズ
	unsigned int colorsize = sizeof (XRGBQUAD) * image.GetColorCount () ;

	unsigned int outputwidth ;
	if( image.GetBitCount () != 24 ) {
		unsigned int bitwidth = image.GetBitCount () * image.GetWidth () ;
		outputwidth = (bitwidth + 7)/8 ;
	} else {
		outputwidth = sizeof (XRGBTRIPLE) * image.GetWidth () ;
	}

	// 遊びのサイズ
	unsigned int padsize = ((outputwidth + 0x3) & ~0x3) - outputwidth ;

	unsigned int datasize = image.GetHeight () * (outputwidth + padsize) ;
	unsigned int spacerequired = nFileHeaderSize
							 + sizeof (XBITMAPINFOHEADER)
							 + colorsize + datasize ;


	infoheader.biBitCount = XFunctions::SystemToLittleEndian ((XUBYTE2) image.GetBitCount ()) ;
	
	nHeadSize = XFunctions::SystemToLittleEndian((XUBYTE4)spacerequired);
	nOffBits  = XFunctions::SystemToLittleEndian( nFileHeaderSize + sizeof (XBITMAPINFOHEADER) + colorsize) ;


	// ファイルヘッダーを書き込み
	strm.Write(reinterpret_cast<char*>(&Signature), sizeof(Signature));
	strm.Write(reinterpret_cast<char*>(&nHeadSize), sizeof(nHeadSize));
	strm.Write(reinterpret_cast<char*>(&tmp2), sizeof(tmp2));
	strm.Write(reinterpret_cast<char*>(&tmp2), sizeof(tmp2));
	strm.Write(reinterpret_cast<char*>(&nOffBits), sizeof(nOffBits));


//	strm.Write(reinterpret_cast<char*>(&fileheader), sizeof (XBITMAPFILEHEADER)) ;
	strm.Write(reinterpret_cast<char*>(&infoheader), sizeof (XBITMAPINFOHEADER)) ;

	for(ii = 0 ; ii < image.GetColorCount () ; ++ ii) {
		XRGBQUAD data ;
		data.rgbRed = image.GetColorMap (ii).red ;
		data.rgbGreen = image.GetColorMap (ii).green ;
		data.rgbBlue = image.GetColorMap (ii).blue ;
		data.rgbReserved = 0 ;
		strm.Write (reinterpret_cast<char*>(&data), sizeof (XRGBQUAD)) ;
	}

	CallProgressFunction (0) ;
	static const char pad [4] = { 0, 0, 0, 0, } ;
	if(image.GetBitCount () != 24) {
		for (ii = 0 ; ii < image.GetHeight () ; ++ ii) {
			CallProgressFunction (ii * 100 /image.GetHeight ()) ;
			unsigned int index = image.GetHeight () - ii - 1 ;
			strm.Write (reinterpret_cast<char*>(&image [index][0]), outputwidth) ;
			strm.Write (pad, padsize) ;
		}
	} else {
		if (XImage::BLUEOFFSET == 0 
			&& XImage::GREENOFFSET == 1 
			&& XImage::REDOFFSET == 2) {
			for (ii = 0 ; ii < image.GetHeight () ; ++ ii) {
				CallProgressFunction (ii * 100 /image.GetHeight ()) ;
				unsigned int index = image.GetHeight () - ii - 1 ;
				strm.Write (reinterpret_cast<char*>(&image [index][0]), 3*image.GetWidth ()) ;
				strm.Write (pad, padsize) ;
			}
		} else {
			for (ii = 0 ; ii < image.GetHeight () ; ++ ii) {
				CallProgressFunction (ii * 100 /image.GetHeight ()) ;
				unsigned int index = image.GetHeight () - ii - 1 ;
				for (unsigned int ii = 0 ; ii < 3 * image.GetWidth () ; ii += 3) {
					strm.Write (reinterpret_cast<char*>(&image [index][ii+XImage::BLUEOFFSET]), 1) ;
					strm.Write (reinterpret_cast<char*>(&image [index][ii+XImage::GREENOFFSET]), 1) ;
					strm.Write (reinterpret_cast<char*>(&image [index][ii+XImage::REDOFFSET]), 1) ;
				}
				strm.Write (pad, padsize) ;
			}
		}
	}

	CallProgressFunction (100) ;
	return ;
}
void XImageDIB::Encoder::WriteImageFile(const char* lpszFileName, XImage &image)
{
	XImageStream strm;
	strm.CreateA(lpszFileName, false, false, true, 0);
	WriteImage(strm, image);
}
void XImageDIB::Encoder::CallProgressFunction(unsigned int percent)
{
	if (m_ProgressFunction == 0)
		return ;
	bool cancel = false ;
	m_ProgressFunction (*this, m_pProgressData, 1, 1, percent, cancel) ;
	if (cancel)
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL);
}




