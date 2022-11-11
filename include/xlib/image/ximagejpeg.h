/**
 * @file ximagejpeg.h
 * @brief JPEG画像イメージ管理クラス
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
 * JPEG画像イメージ管理クラス
 */
class XImageJPEG
{
public:

	/**
	 * JPEGカラースペース
	 */
	enum COLOR_SPACE
	{
		/** 不明 */
		cs_UNKNOWN,
		/** モノクロ */
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
	 * JPEGデコーダー
	 */
	class Decoder : public XImageDecoder
	{
	public:
		/**	コンストラクタ			
		*/
		Decoder();
		/**	コンストラクタ
		 *	@param	source	コピー元のソース
		 */
		Decoder(const Decoder& source);
		/** デストラクタ */
		virtual ~Decoder();
		/** 指定されたインスタンスを代入します。
		 *	@param	source	コピー元のソース
		 */
		Decoder& operator=(const Decoder& source);



		/**	@copydoc XImageEncoder::ReadImageFile(const char*, XImage&) */
		virtual void ReadImageFile(const char* lpszFileName, XImage& img);

		/** JPEGファイルをオープンしたストリームからイメージを読み込みます
		 *	@param	strm	JPEGファイルをオープンしたストリーム
		 *	@param	img	イメージデータを受け取るインスタンス
		 *	@exception XImageException* エラーインスタンス
		*/
		virtual void ReadImage(XImageStream& strm, XImage& img);
		/* コールバック用内部使用メンバ
		 *	@param	percent	パーセンテージ
		 */
		void _CallProgressFunction(unsigned int percent);
	private:
		/**	指定されたインスタンスのコピーを作成します。
		 *	@param	sorce	コピー元インスタンス
		 */
		void DoCopy(const Decoder& sorce);
		/*	読み込み処理の経過をパーセテンジで通知します。
		 *	@param	percent	パーセンテージ
		 */
		void CallProgressFunction(unsigned int percent);
	};

	/**
	 * JPEGエンコーダー
	 */
	class Encoder : public XImageEncoder
	{
	public:
		/**	コンストラクタ			
		*/
		Encoder();
		/**	コンストラクタ
		 *	@param	source	コピー元のソース
		 */
		Encoder(const Encoder& source);
		/** デストラクタ */
		virtual ~Encoder();
		/** 指定されたインスタンスを代入します。
		 *	@param	source	コピー元のソース
		 */
		Encoder& operator=(const Encoder& source);

		/**	JPEGイメージファイルとして書き込みます。
		 *	@param	strm	オープンされたファイルストリーム
		 *	@param	img	ファイルに書き込みストリーム
		 *	@exception XImageException* エラーインスタンス
		*/
		virtual void WriteImage(XImageStream &strm, const XImage &img);
		/**	@copydoc XImageEncoder::WriteImage(const char*, XImage&) */
		virtual void WriteImageFile(const char* lpszFileName, XImage& img);

	
		int  getQuality();
		void putQuality(int val);
		COLOR_SPACE getColorSpace();
		void        putColorSpace(COLOR_SPACE val);

	
	private:
		/*	読み込み処理の経過をパーセテンジで通知します。
		 *	@param	percent	パーセンテージ
		 */
		void CallProgressFunction(unsigned int percent);

		
		/* クォリティ*/
		int			m_nQuality;
		/* カラースペース */
		COLOR_SPACE	m_nColorSpace;
		
	} ;
};


#endif /* __X_IMAGE_JPEG_HEASER_FILE__ */
