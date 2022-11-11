/**
 * @file ximagedib.h
 * @brief DIB�摜�C���[�W�Ǘ��N���X
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
 * DIB�C���[�W�N���X
 */
class XImageDIB
{
public:

	/**
	 * �r�b�g�}�b�v�f�R�[�_�[
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
	 * �r�b�g�}�b�v�G���R�[�_�[
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







