#include "xlib/image/ximagepng.h"
#ifdef WIN32
	#define XMD_H
	#undef FAR
#endif
extern "C" {
	#include <libpng/png.h>
}
#include <setjmp.h>


/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImagePNG::Decoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImagePNG::Decoder::Decoder()
{

}
XImagePNG::Decoder::Decoder(const XImagePNG::Decoder& source)
	:	XImageDecoder(source) 
{

}
XImagePNG::Decoder::~Decoder()
{
}
XImagePNG::Decoder& XImagePNG::Decoder::operator=(const XImagePNG::Decoder& source)
{
	XImageDecoder::operator=(source);
	return *this;
}
void XImagePNG::Decoder::ReadImageFile(const char* lpszFileName, XImage& image)
{
	XImageStream inputstream ;
	inputstream.OpenA(lpszFileName, true, false, false, true);
	ReadImage(inputstream, image);
}


void read_row_callback(png_structp png_ptr, png_uint_32 row, int pass)
{
//      XTRACE("%d  %3d%% readed\n", row, (row * 100) / png_ptr->height);
}
void user_error_fn(png_structp png_ptr, png_const_charp error_msg)
{
	png_const_charp p = error_msg;
	XTRACE(_T("ERROR ** %s\n"), error_msg);
}

void XImagePNG::Decoder::ReadImage(XImageStream& strm, XImage& image)
{
	png_structp     png_ptr;
	png_infop       info_ptr;
	FILE*			infile;
	png_uint_32		width, height;
	int PNG_BYTES_TO_CHECK = 4;
	XBuff			imageBuff;


	infile = ::fdopen(strm.FileNo(), "rb");
	if( infile == NULL ) {
		XImageException::Throw( XIMG_ERR_END_STREAM );
	}


	// 読み込みオブジェクトの作成
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, user_error_fn, NULL);
	if( png_ptr ==   NULL ) {
		
		return ;
	}
	// ヘッダー情報の初期化
	info_ptr = png_create_info_struct(png_ptr);
	if( info_ptr ==   NULL ) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return ;
	}
	// エラーがあった場合にここに飛んでくるように設定
	if( ::setjmp( png_ptr->jmpbuf) ) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return ;
	}

	png_set_read_status_fn(png_ptr, read_row_callback);
	// 読み込みストリームをセット
	png_init_io(png_ptr, infile);
//	png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);
	// ヘッダー情報を読み込み
	png_read_info(png_ptr, info_ptr);
	/*
XTRACE(_T("W:%d H:%d\n"), info_ptr->width, info_ptr->height);
XTRACE(_T("Palette:%d\n"), info_ptr->palette);
XTRACE(_T("num_palette:%d\n"), info_ptr->num_palette);
XTRACE(_T("num_trans:%d\n"), info_ptr->num_trans);
XTRACE(_T("bit_depth:%d\n"), info_ptr->bit_depth);
XTRACE(_T("color_type:%d\n"), info_ptr->color_type);
XTRACE(_T("compression_type:%d\n"), info_ptr->compression_type);
XTRACE(_T("interlace_type:%d\n"), info_ptr->interlace_type);
XTRACE(_T("valid:%d\n"), info_ptr->valid);
*/


	// ガンマ補正をセット
	double          display_gamma, file_gamma;
	display_gamma = 2.2;
	if( png_get_gAMA(png_ptr, info_ptr, &file_gamma) ) {
		png_set_gamma(png_ptr, display_gamma, file_gamma);
	} else {
		png_set_gamma(png_ptr, 0.0, 0.0);
	}

	// 画像情報の取得
	width  = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	
	// 必要なメモリを確保
	png_bytepp pImage = (png_bytepp)imageBuff.Alloc(height * sizeof(png_bytep));
	if( pImage == NULL ) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return ;
	}
	// メモリの確保
	image.SetSize(0,     // Color Table Entries
			8 * 3,  // Bits
			width, height);

	// 画像の読み込み	
	png_uint_32 ii;
	for(ii = 0; ii < height; ii++) {
		pImage[ii] = (png_bytep)image[ii];
	}
	png_read_image(png_ptr, pImage);
	for(ii = 0; ii < height; ii++) {
		XUBYTE1 *pData = (png_bytep)image[ii];
		png_uint_32 jj = 0;
		png_uint_32 currentWidth = 0;

		for(jj = 0; currentWidth < width; jj++) {
			XUBYTE1 swap   = pData[ jj + 0 ];
			pData[ jj + 0 ] = pData[ jj + 2 ];
			pData[ jj + 2 ] = swap;
			jj += 2;
			currentWidth++;
		}
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
}
void XImagePNG::Decoder::DoCopy(const XImagePNG::Decoder& sorce)
{

}
void XImagePNG::Decoder::CallProgressFunction(unsigned int percent)
{
	if (m_ProgressFunction == 0)
		return ;
	bool cancel = false ;
	m_ProgressFunction (*this, m_pProgressData, 1, 1, percent, cancel) ;
	if (cancel)
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL );
}
void XImagePNG::Decoder::_CallProgressFunction(unsigned int percent)
{
	this->CallProgressFunction(percent);
}
/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImagePNG::Encoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImagePNG::Encoder::Encoder()
	:	m_nQuality(80)
	,	m_nColorSpace(cs_RGB)
{

}
XImagePNG::Encoder::Encoder(const XImagePNG::Encoder& source)
	:	XImageEncoder(source) 
	,	m_nQuality(80)
	,	m_nColorSpace(cs_RGB)
{
}
XImagePNG::Encoder::~Encoder()
{

}
XImagePNG::Encoder& XImagePNG::Encoder::operator=(const XImagePNG::Encoder& source)
{
	XImageEncoder::operator=(source);
	return *this;
}
void write_row_callback(png_structp png_ptr, png_uint_32 row, int pass)
{
//	XTRACE("%d  %3d%% saved\n", row, (row * 100) / png_ptr->height);
}
void XImagePNG::Encoder::WriteImage(XImageStream &strm, const XImage &image)
{
	FILE*			outfile;
	png_structp		png_ptr;
	png_infop		info_ptr;
	XBuff			imageBuff;
	int				i, j, k;
	png_byte*		pWriteWidthBuff;

	// ファイルをオープン
	outfile = ::fdopen(strm.FileNo(), "wb");
	if( outfile == NULL ) {
		XImageException::Throw( XIMG_ERR_END_STREAM );
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, user_error_fn, NULL);
	if (png_ptr == NULL) {
		return;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
		return;
	}
	if (setjmp(png_ptr->jmpbuf)) {
		png_destroy_write_struct(&png_ptr,  &info_ptr);
		return;
	}

	png_init_io(png_ptr, outfile);
//	png_set_write_status_fn(png_ptr, write_row_callback);
	png_set_filter(png_ptr, 0, PNG_ALL_FILTERS);
	png_set_compression_level(png_ptr, /*Z_BEST_SPEED*/ Z_BEST_COMPRESSION);
	
	png_set_IHDR(png_ptr
		, info_ptr
		, image.GetWidth()
		, image.GetHeight()
		, 8
		, PNG_COLOR_TYPE_RGB
		, PNG_INTERLACE_NONE /* PNG_INTERLACE_NONE or PNG_INTERLACE_ADAM7  */
		, PNG_COMPRESSION_TYPE_DEFAULT	/* must be  PNG_COMPRESSION_TYPE_DEFAULT */
		, PNG_FILTER_TYPE_DEFAULT /* must be PNG_FILTER_TYPE_DEFAULT
                     or, if you are writing a PNG to
                     be embedded in a MNG datastream,
                     can also be
                     PNG_INTRAPIXEL_DIFFERENCING */
	);

	pWriteWidthBuff = (png_byte*)imageBuff.Alloc( image.GetWidth() * 3);
	png_set_gAMA(png_ptr, info_ptr, 0.0);
/*	{
		time_t		gmt;		// G.M.T.
		png_time	mod_time;
		png_text	text_ptr[5];

		time(&gmt);
		png_convert_from_time_t(&mod_time, gmt);
		png_set_tIME(png_ptr, info_ptr, &mod_time);
		
		text_ptr[0].key = "Title";
		text_ptr[0].text = "Test";
		text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
		text_ptr[1].key = "Author";
		text_ptr[1].text = "Fujiha Tsuyoshi";
		text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
		text_ptr[2].key = "Description";
		text_ptr[2].text = "Test Pattern";
		text_ptr[2].compression = PNG_TEXT_COMPRESSION_NONE;
		text_ptr[3].key = "Creation Time";
		text_ptr[3].text = png_convert_to_rfc1123(png_ptr, &mod_time);
		text_ptr[3].compression = PNG_TEXT_COMPRESSION_NONE;
		text_ptr[4].key = "Software";
		text_ptr[4].text = "test.exe";
		text_ptr[4].compression = PNG_TEXT_COMPRESSION_NONE;
		png_set_text(png_ptr, info_ptr, text_ptr, 5);
	}
*/
	png_write_info(png_ptr, info_ptr);
	//png_write_image(png_ptr, pImageBuff);
	for(i = 0; i < image.GetHeight(); i++) {
		XUBYTE1 *pData = (png_bytep)image[i];
		png_uint_32 currentWidth = 0;

		for(j = 0, k = 0; currentWidth < image.GetWidth(); j++, k++) {
			pWriteWidthBuff[k] = pData[j+2];
			pWriteWidthBuff[k+1] = pData[j+1];
			pWriteWidthBuff[k+2] = pData[j];
			if( image.GetBitCount() == 32 ) {
				j += 3;
			} else {
				j += 2;
			}
			k += 2;
			currentWidth++;
		}
		png_write_rows(png_ptr, (png_byte**)&pWriteWidthBuff, 1);
	}
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	::fflush(outfile);
	::fclose(outfile);
	strm.Close();	
}
void XImagePNG::Encoder::WriteImageFile(const char* lpszFileName, XImage &image)
{
	XImageStream strm;
	strm.CreateA(lpszFileName, false, false, true, 0);
	WriteImage(strm, image);
}
void XImagePNG::Encoder::CallProgressFunction(unsigned int percent)
{
	if (m_ProgressFunction == 0)
		return ;
	bool cancel = false ;
	m_ProgressFunction (*this, m_pProgressData, 1, 1, percent, cancel) ;
	if (cancel)
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL);
}
int  XImagePNG::Encoder::getQuality()
{
	return m_nQuality;
}
void XImagePNG::Encoder::putQuality(int val)
{
	m_nQuality = val;
}
XImagePNG::COLOR_SPACE XImagePNG::Encoder::getColorSpace()
{
	return m_nColorSpace;
}
void XImagePNG::Encoder::putColorSpace(XImagePNG::COLOR_SPACE val)
{
	m_nColorSpace = val;
}



