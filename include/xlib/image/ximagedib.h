/**
 * @file ximagedib.h
 * @brief DIB画像イメージ管理クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_IMAGE_DIB_HEASER_FILE__
#define __X_IMAGE_DIB_HEASER_FILE__


#include "ximage.h"


/**
 * DIBイメージクラス
 */
class XImageDIB
{
public:

	/**
	 * ビットマップデコーダー
	 */
	class Decoder : public XImageDecoder
	{
	public:
		Decoder();
		Decoder(const Decoder& source);
		virtual ~Decoder();
		Decoder& operator=(const Decoder& source);



		virtual void ReadImageFile(const char* lpszFileName, XImage& img);
		virtual void ReadImage(XImageStream& strm, XImage& img);

	private:
		void CallProgressFunction(unsigned int percent);
	};

	/**
	 * ビットマップエンコーダー
	 */
	class Encoder : public XImageEncoder
	{
	public:
		Encoder();
		Encoder(const Encoder& source);
		virtual ~Encoder();
		Encoder& operator=(const Encoder& source);
		virtual void WriteImage(XImageStream &strm, const XImage &img);
		virtual void WriteImageFile(const char* lpszFileName, XImage &img);
	private:
		void CallProgressFunction(unsigned int percent);
	} ;
};

#endif /* __X_IMAGE_DIB_HEASER_FILE__ */







