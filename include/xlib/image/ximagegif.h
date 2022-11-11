/**
 * @file ximagegif.h
 * @brief JPEG画像イメージ管理クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_IMAGE_GIF_HEASER_FILE__
#define __X_IMAGE_GIF_HEASER_FILE__


#include "xlib/image/ximage.h"
#include "xlib/xarray.h"


/**
 * GIF画像イメージ管理クラス
 */
class XImageGIF
{
public:
	typedef struct tagAnimeImage {
		int		nLeft;
		int		nTop;
		int		nLateDelay;
		int		nTrance;
		XImage	image;
	}AnimeImage;

	/**
	 * GIFデコーダー
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



		/* アニメーションデータ */
		AnimeImage	m_AnimeImageData;
		/* アニメーションのイメージ配列 */
		XArray<AnimeImage*>		m_AnimeImage;
	};

	/**
	 * GIFエンコーダー
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
	
	private:
		/*	読み込み処理の経過をパーセテンジで通知します。
		 *	@param	percent	パーセンテージ
		 */
		void CallProgressFunction(unsigned int percent);

	};
};


#endif /* __X_IMAGE_GIF_HEASER_FILE__ */
