#include "xlib/image/ximagejpeg.h"
#ifdef WIN32
	#define XMD_H
	#undef FAR
#endif
extern "C" {
	#include <jpeg/jpeglib.h>
}
#include <setjmp.h>

struct XJpegErrorMgr {
  struct jpeg_error_mgr pub;
  jmp_buf				setjmp_buffer;
};

static void JpegErrorExit(j_common_ptr cinfo)
{
	XJpegErrorMgr* myerr = (XJpegErrorMgr*) cinfo->err;

	XTRACE(_T("JPEG Error Routine\n"));
	::longjmp(myerr->setjmp_buffer, 1);
}

static void JpegProgressFunction(j_common_ptr cinfo)
{
	struct jpeg_progress_mgr *progres = cinfo->progress;
	XImageJPEG::Decoder *pDecoder = (XImageJPEG::Decoder*)cinfo->client_data;

	double nPercent;

	nPercent = 	progres->pass_counter /
			progres->pass_limit;

	pDecoder->_CallProgressFunction((unsigned int)(nPercent * 100));
}


/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImageJPEG::Decoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImageJPEG::Decoder::Decoder()
{

}
XImageJPEG::Decoder::Decoder(const XImageJPEG::Decoder& source)
	:	XImageDecoder(source) 
{

}
XImageJPEG::Decoder::~Decoder()
{
}
XImageJPEG::Decoder& XImageJPEG::Decoder::operator=(const XImageJPEG::Decoder& source)
{
	XImageDecoder::operator=(source);
	return *this;
}
void XImageJPEG::Decoder::ReadImageFile(const char* lpszFileName, XImage& image)
{
	XImageStream inputstream ;
	inputstream.OpenA(lpszFileName, true, false, false, true);
	ReadImage(inputstream, image);
}
void XImageJPEG::Decoder::ReadImage(XImageStream& strm, XImage& image)
{
	struct jpeg_decompress_struct cinfo;
	struct XJpegErrorMgr          jerr;
	struct jpeg_progress_mgr      progres;
	FILE* infile;
	JSAMPARRAY buffer;
	int row_stride;

	::memset(&progres, 0, sizeof(progres));

	infile = ::fdopen(strm.FileNo(), "rb");
	if( infile == NULL ) {
		XImageException::Throw( XIMG_ERR_END_STREAM );
	}


	cinfo.client_data = this;
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = JpegErrorExit;




	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		XImageException::Throw( XIMG_ERR_END_STREAM );
	}


	jpeg_create_decompress(&cinfo);


	progres.progress_monitor = JpegProgressFunction;
	cinfo.progress = &progres;
	cinfo.progressive_mode = true;

	jpeg_stdio_src(&cinfo, infile);

	jpeg_read_header(&cinfo, true);

	jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;

	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);



	if(cinfo.output_components == 1) {
		// メモリの確保
		image.SetSize(1 << 8, 8, cinfo.image_width, cinfo.image_height);

		// グレースケール用カラーマップの作成
		for(unsigned int ii = 0 ; ii <(1 << 8); ++ ii) {
			image.GetColorMap(ii).red   =(XUBYTE1) ii ;
			image.GetColorMap(ii).green =(XUBYTE1) ii ;
			image.GetColorMap(ii).blue  =(XUBYTE1) ii ;
		}
	} else {
		// メモリの確保
		image.SetSize(0,     // Color Table Entries
				8 * cinfo.output_components,  // Bits
				cinfo.image_width, cinfo.image_height);
	}
	
	while(cinfo.output_scanline < cinfo.output_height ) {

		// イメージデータの指定ラインのバッファ取得
		XUBYTE1 *pImageData = image[cinfo.output_scanline];

		// １ライン読み込み
		jpeg_read_scanlines(&cinfo, buffer, 1);


		XUBYTE1 *pBuff = buffer[0];

		if(cinfo.output_components == 1) {
			// グレースケール
			for(int i = 0; i < row_stride; i++ ) {
			   pImageData[i] = pBuff[i];
			}
		} else {
			// RGBイメージ
			for(int i = 0; i < row_stride; i+=3 ) {
			   *pImageData++ = pBuff[i+2];
			   *pImageData++ = pBuff[i+1];
			   *pImageData++ = pBuff[i];
			}
		}
	}

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);
}
void XImageJPEG::Decoder::DoCopy(const XImageJPEG::Decoder& sorce)
{

}
void XImageJPEG::Decoder::CallProgressFunction(unsigned int percent)
{
	if (m_ProgressFunction == 0)
		return ;
	bool cancel = false ;
	m_ProgressFunction (*this, m_pProgressData, 1, 1, percent, cancel) ;
	if (cancel)
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL );
}
void XImageJPEG::Decoder::_CallProgressFunction(unsigned int percent)
{
	this->CallProgressFunction(percent);
}
/*
///////////////////////////////////////////////////////////////////////////////////
//
//	XImageJPEG::Encoder
//
///////////////////////////////////////////////////////////////////////////////////
*/
XImageJPEG::Encoder::Encoder()
	:	m_nQuality(80)
	,	m_nColorSpace(cs_RGB)
{

}
XImageJPEG::Encoder::Encoder(const XImageJPEG::Encoder& source)
	:	XImageEncoder(source) 
	,	m_nQuality(80)
	,	m_nColorSpace(cs_RGB)
{
}
XImageJPEG::Encoder::~Encoder()
{

}
XImageJPEG::Encoder& XImageJPEG::Encoder::operator=(const XImageJPEG::Encoder& source)
{
	XImageEncoder::operator=(source);
	return *this;
}
void XImageJPEG::Encoder::WriteImage(XImageStream &strm, const XImage &image2)
{
	struct jpeg_compress_struct		cinfo;
	//struct XJpegErrorMgr          jerr;
	struct jpeg_error_mgr			jerr;
	FILE*							outfile;
	int								row_stride;	// 物理的なイメージの横サイズ
	JSAMPROW						row_pointer[1];
	XBuff							buff;
	XUBYTE1							*pD, *pW;
	XImage							dstImage;
	unsigned int					m, n;


	// 24ビットイメージに変換する。
	dstImage.SetSize(0, 24, image2.GetWidth(), image2.GetHeight());
	for(m = 0; m < image2.GetHeight(); m++) {
		for(n = 0; n < image2.GetWidth(); n++) {
			XUBYTE1 red, green, blue;
			image2.GetRgb(m, n, red, green, blue);
			dstImage.SetPixel(n, m, red, green, blue);
		}
	}

	// エラー情報をセット。
	cinfo.client_data = this;
//	cinfo.err = jpeg_std_error(&jerr.pub);
	cinfo.err = jpeg_std_error(&jerr);
//	jerr.pub.error_exit = JpegErrorExit;

//	if (setjmp(jerr.setjmp_buffer)) {
//		jpeg_destroy_compress(&cinfo);
//		XImageException::Throw( XIMG_ERR_END_STREAM );
//	}


	// 構造体の初期化
	jpeg_create_compress(&cinfo);

	// ファイルをオープン
	outfile = ::fdopen(strm.FileNo(), "wb");
	if( outfile == NULL ) {
		XImageException::Throw( XIMG_ERR_END_STREAM );
	}

	/* 圧縮データを指定ファイルに送るようにセット */
	jpeg_stdio_dest(&cinfo, outfile);

	// 画像のイメージ情報をセット
	cinfo.image_width = dstImage.GetWidth();
	cinfo.image_height = dstImage.GetHeight();
	switch(m_nColorSpace) {
	case cs_GRAYSCALE:
		cinfo.input_components = 1;/* コンポーネント数 */
		cinfo.in_color_space = JCS_GRAYSCALE;
		break;
	case cs_RGB:
		cinfo.input_components = 3;/* コンポーネント数 */
		cinfo.in_color_space = JCS_RGB;
		break;
	case cs_YCbCr:
		cinfo.input_components = 3;/* コンポーネント数 */
		cinfo.in_color_space = JCS_YCbCr;
		break;
	case cs_CMYK:
		cinfo.input_components = 4;/* コンポーネント数 */
		cinfo.in_color_space = JCS_CMYK;
		break;
	case cs_YCCK:
		cinfo.input_components = 3;/* コンポーネント数 */
		cinfo.in_color_space = JCS_YCCK;
		break;
	default:
		cinfo.input_components = 3;/* コンポーネント数 */
		cinfo.in_color_space = JCS_RGB;
		break;
	}
	

	/* デフォルト圧縮ルーチンをセットする */
	jpeg_set_defaults(&cinfo);

	/* 画像クオリティをセット */
	jpeg_set_quality(&cinfo, 
					m_nQuality, 
					true);

	jpeg_start_compress(&cinfo, true);

	/* 物理的なイメージの横サイズ */
	row_stride = cinfo.image_width * cinfo.input_components;

	pD = (XUBYTE1*)buff.Alloc(row_stride);

	while (cinfo.next_scanline < cinfo.image_height) {
		XUBYTE1 *pImageData = dstImage[cinfo.next_scanline];
		row_pointer[0] = pImageData;
		
		if( m_nColorSpace != cs_GRAYSCALE ) {
			pW = pD;
			for(int i = 0; i < row_stride; i+=3 ) {
			   *pW++ = pImageData[i+2];
			   *pW++ = pImageData[i+1];
			   *pW++ = pImageData[i];
			}
			row_pointer[0] = pD;
		}
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	/* 確定 */
	jpeg_finish_compress(&cinfo);

	/* 後処理 */
	jpeg_destroy_compress(&cinfo);

	::fclose(outfile);
}
void XImageJPEG::Encoder::WriteImageFile(const char* lpszFileName, XImage &image)
{
	XImageStream strm;
	strm.CreateA(lpszFileName, false, false, true, 0);
	WriteImage(strm, image);
}
void XImageJPEG::Encoder::CallProgressFunction(unsigned int percent)
{
	if (m_ProgressFunction == 0)
		return ;
	bool cancel = false ;
	m_ProgressFunction (*this, m_pProgressData, 1, 1, percent, cancel) ;
	if (cancel)
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL);
}
int  XImageJPEG::Encoder::getQuality()
{
	return m_nQuality;
}
void XImageJPEG::Encoder::putQuality(int val)
{
	m_nQuality = val;
}
XImageJPEG::COLOR_SPACE XImageJPEG::Encoder::getColorSpace()
{
	return m_nColorSpace;
}
void XImageJPEG::Encoder::putColorSpace(XImageJPEG::COLOR_SPACE val)
{
	m_nColorSpace = val;
}



