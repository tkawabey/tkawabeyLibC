/**
 * @file ximagejpeg.h
 * @brief JPEG�摜�C���[�W�Ǘ��N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_IMAGE_JPEG_HEASER_FILE__
#define __X_IMAGE_JPEG_HEASER_FILE__


#include "ximage.h"


/**
 * JPEG�摜�C���[�W�Ǘ��N���X
 */
class XImageJPEG
{
public:

	/**
	 * JPEG�J���[�X�y�[�X
	 */
	enum COLOR_SPACE
	{
		/** �s�� */
		cs_UNKNOWN,
		/** ���m�N�� */
		cs_GRAYSCALE,
		/** RGB */
		cs_RGB,
		/** Y/Cb/Cr */
		cs_YCbCr,
		/** C/M/Y/K */
		cs_CMYK,
		/** Y/Cb/Cr/K */
		cs_YCCK
	};


	/**
	 * JPEG�f�R�[�_�[
	 */
	class Decoder : public XImageDecoder
	{
	public:
		/**	�R���X�g���N�^			
		*/
		Decoder();
		/**	�R���X�g���N�^
		 *	@param	source	�R�s�[���̃\�[�X
		 */
		Decoder(const Decoder& source);
		/** �f�X�g���N�^ */
		virtual ~Decoder();
		/** �w�肳�ꂽ�C���X�^���X�������܂��B
		 *	@param	source	�R�s�[���̃\�[�X
		 */
		Decoder& operator=(const Decoder& source);



		/**	@copydoc XImageEncoder::ReadImageFile(const char*, XImage&) */
		virtual void ReadImageFile(const char* lpszFileName, XImage& img);

		/** JPEG�t�@�C�����I�[�v�������X�g���[������C���[�W��ǂݍ��݂܂�
		 *	@param	strm	JPEG�t�@�C�����I�[�v�������X�g���[��
		 *	@param	img	�C���[�W�f�[�^���󂯎��C���X�^���X
		 *	@exception XImageException* �G���[�C���X�^���X
		*/
		virtual void ReadImage(XImageStream& strm, XImage& img);
		/* �R�[���o�b�N�p�����g�p�����o
		 *	@param	percent	�p�[�Z���e�[�W
		 */
		void _CallProgressFunction(unsigned int percent);
	private:
		/**	�w�肳�ꂽ�C���X�^���X�̃R�s�[���쐬���܂��B
		 *	@param	sorce	�R�s�[���C���X�^���X
		 */
		void DoCopy(const Decoder& sorce);
		/*	�ǂݍ��ݏ����̌o�߂��p�[�Z�e���W�Œʒm���܂��B
		 *	@param	percent	�p�[�Z���e�[�W
		 */
		void CallProgressFunction(unsigned int percent);
	};

	/**
	 * JPEG�G���R�[�_�[
	 */
	class Encoder : public XImageEncoder
	{
	public:
		/**	�R���X�g���N�^			
		*/
		Encoder();
		/**	�R���X�g���N�^
		 *	@param	source	�R�s�[���̃\�[�X
		 */
		Encoder(const Encoder& source);
		/** �f�X�g���N�^ */
		virtual ~Encoder();
		/** �w�肳�ꂽ�C���X�^���X�������܂��B
		 *	@param	source	�R�s�[���̃\�[�X
		 */
		Encoder& operator=(const Encoder& source);

		/**	JPEG�C���[�W�t�@�C���Ƃ��ď������݂܂��B
		 *	@param	strm	�I�[�v�����ꂽ�t�@�C���X�g���[��
		 *	@param	img	�t�@�C���ɏ������݃X�g���[��
		 *	@exception XImageException* �G���[�C���X�^���X
		*/
		virtual void WriteImage(XImageStream &strm, const XImage &img);
		/**	@copydoc XImageEncoder::WriteImage(const char*, XImage&) */
		virtual void WriteImageFile(const char* lpszFileName, XImage& img);

	
		int  getQuality();
		void putQuality(int val);
		COLOR_SPACE getColorSpace();
		void        putColorSpace(COLOR_SPACE val);

	
	private:
		/*	�ǂݍ��ݏ����̌o�߂��p�[�Z�e���W�Œʒm���܂��B
		 *	@param	percent	�p�[�Z���e�[�W
		 */
		void CallProgressFunction(unsigned int percent);

		
		/* �N�H���e�B*/
		int			m_nQuality;
		/* �J���[�X�y�[�X */
		COLOR_SPACE	m_nColorSpace;
		
	} ;
};


#endif /* __X_IMAGE_JPEG_HEASER_FILE__ */
