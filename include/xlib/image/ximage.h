/**
 * @file ximage.h
 * @brief 画像イメージ管理クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_IMAGE_HEASER_FILE__
#define __X_IMAGE_HEASER_FILE__
#include <vector>
#include <stdexcept>
#include "xlib/xbuff.h"
#include "xlib/xfile.h"
#include "xlib/xeception.h"

class XImageException ;
class XImageStream;
class XImageCoder;
class XImage;


typedef unsigned long XCOLORREF;

/** 画像イメージ管理クラスエラー定義 */
enum XIMG_ERR
{
	/** ストリームがEOFに達した */
	XIMG_ERR_END_STREAM,
	/** ハフマンの長さコードエラー。ハフマンの長さコードは、0－18の間 */
	XIMG_ERR_BAD_HUFFMAN_LENGTH,
	/** レングスオーバー*/
	XIMG_ERR_BAD_LENGTH_OUT_OF_RANGE,
	/** ハフマン係数値エラー */
	XIMG_ERR_BAD_HUFFMAN_LITERAL,
	/** ハフマンの距離エラー */
	XIMG_ERR_BAD_HUFFMAN_DISTANCE,
	/** サポートされていないビットカウント */
	XIMG_ERR_UNSUPPORT_BITCOUNT,
	/** カラーマップエントリにしてされたインデックスが不正です */
	XIMG_ERR_SUBSCRIPT_OUT_RANGE,
	/** 指定した、ROWまたはCOLの値が範囲を超えています */
	XIMG_ERR_ROW_COLMN_OUT_OF_RANGE,
	/** 指定されたカラーマップのカウントが不正です */
	XIMG_ERR_QUATIZATION_AREA_COUNT_INVALID,
	/**  */
	XIMG_ERR_COLOR_NO_QUATIZ,
	/** イメージデータ読み込み完了前に、ストリームがEOFに達した */
	XIMG_ERR_END_OF_STREAM,
	/** サポートされていない圧縮形式です */
	XIMG_ERR_UNSUPPORT_COMPRESSION,
	/** 不正なデータ */
	XIMG_ERR_CORUPPUT_DATA,
	/** サポートされていないビットピクセル値 */
	XIMG_ERR_UNSUPORT_BITS_PER_PIXL,
	/** メモリを確保できない */
	XIMG_ERR_MALLOC,
	/** APIエラーが発生した */
	XIMG_ERR_API,

	/** ビットマップファイルでない */
	XIMG_ERR_NOT_BMP_FILE,
	/** サポートされていないビットマップファイルヘッダーです */
	XIMG_ERR_UNSUPPORT_BIT_MAP_HEADER,

	/** 指定された原色出力エンジンのメソッドが不正 */
	XIMG_ERR_INVALID_PALLET_PRECISION,
	/** 指定された原色制度値が不正(0-20) */
	XIMG_ERR_INVALID_CRMA,
	/** 指定されたピクチャーマスク値が不正 */
	XIMG_ERR_INVALID_PICTURE_MASK,
	/** 指定されたディザレベルが不正(0-10) */
	XIMG_ERR_INVALID_DITHER_LEVEL,
	/** 指定されたディザモードが不正 */
	XIMG_ERR_INVALID_DITHER_MODE,
	/** 指定された変換カラー値が不正 */
	XIMG_ERR_INVALID_TRANCE_COLOR,

	/** 不明 */
	XIMG_ERR_ROW_UNKOWN,
	/** プログレスでキャンセルされた */
	XIMG_ERR_PROGRESS_CANCEL,
};

typedef void (*XIMG_PROGRESS_FUNC)(XImageCoder &coder,
                                 void *data,
                                 unsigned int currentpass,
                                 unsigned int passcount,
                                 unsigned int progress,
                                 bool &cancel);

typedef void (*XIMAG_PROGRESSFUNCTION)(XImage &image,
                                      void *data,
                                      unsigned int currentpass,
                                      unsigned int passcount,
                                      unsigned int progress,
                                      bool &cancel) ;
//


/**
 * イメージ管理データ例外クラス
 */
class XImageException : public XExcept
{
public:
	/** コンストラクタ */
	XImageException();
	/** デストトラクタ */
	virtual ~XImageException() throw ();

	static void Throw(XIMG_ERR err);

	XIMG_ERR m_err;
};

/**
 * イメージファイル読み込みクラス
 */
class XImageStream : public XFile
{
public:
	/** コンストラクタ */
	XImageStream();
	/** デストラクタ */
	virtual ~XImageStream();

	/** @copydoc XIO::Close() */
	virtual bool Close();
	/** @copydoc XFile::OpenRawA(const char *,int , bool , int ) */
	virtual bool OpenRawA(const char *filename,    int flg, bool bShare, int pmode);
	/** @copydoc XFile::OpenRawA(const char *,int , bool , int ) */
	virtual bool OpenRawW(const wchar_t *filename, int flg, bool bShare, int pmode);
	/**  XIO::WriteRaw(const void* , size_t , int ) */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0);
	/**  XIO::ReadRaw(void* , size_t , int ) */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0);

	int FileNo(){ return m_iFile; }
	bool IsEOF();
protected:
	/* 現在の位置 */
	unsigned long		m_dwCurrentPos;
	/* ファイルのサイズ */
	unsigned long		m_dwFileSize;
	unsigned long		m_dwReadedCount;
	unsigned long       m_dwReaded;
};

/**
 * イメージエンコード、デコード、クラス。
 */
class XImageCoder
{
public:
	/** コンストラクタ */
	XImageCoder();
	/**	@brief	コンストラクタ
	 *	@param	source	コピー元データ
	 */
	XImageCoder(const XImageCoder& source);
	/** デストラクタ */
	virtual ~XImageCoder();

	/**	@brief	オブジェクトを代入します。
	 *	@param	source	コピー元データ
	 *	@return	自身
	 */
	XImageCoder& operator=(const XImageCoder& source);

	
	/**	ステータス通知関数をセットします。
	 *	@param	pfunc	ステータス通知関数
	 *	@param	data	ユーザー定義データ
	 */
	void SetProgressFunction(XIMG_PROGRESS_FUNC pfunc, void *data);

	/**	イメージデータを更新します。
	 *	@return	virtual void
	 */
	virtual void UpdateImage();
protected:
	/* ステータス通知関数 */
	XIMG_PROGRESS_FUNC	m_ProgressFunction ;
	/* ユーザー定義データ */
	void				*m_pProgressData;
};

/**
 * 画像イメージデコーダーベースクラス
 */
class XImageDecoder : public XImageCoder
{
public:
	/** デストラクタ */
	virtual ~XImageDecoder(){}
	/**	イメージデータを読み込みます。
	 *	@param	lpszFileName	読み込むファイルのパス文字
	 *	@param	img	読み込んだイメージを受け取るXImageインスタンス
	 */
	virtual void ReadImageFile(const char* lpszFileName, XImage& img) = 0;
};

/**
 * 画像イメージエンコーダーベースクラス
 */
class XImageEncoder : public XImageCoder
{
public:
	/** デストラクタ */
	virtual ~XImageEncoder(){}
	/**	イメージデータをファイルに書き込みます。
	 *	@param	lpszFileName	書き込むファイルのパス
	 *	@param	img	書き込むイメージのXImageインスタンス
	 *	@exception XImageException* エラーインスタンス
	*/
	virtual void WriteImageFile(const char* lpszFileName, XImage& img) = 0;
};

/**
 * イメージデータ
 */
class XImage
{
public:
	/**
	 * 24Bit以上以外のビットマップイメージのカラーマップの定義
	 */
	struct ColorMapEntry
	{
		/** 青 */
		XUBYTE1 blue;
		/** 緑 */
		XUBYTE1 green;
		/** 赤 */
		XUBYTE1 red;
	};
	/**
	 * 色のオフセット位置
	 */
	enum ColorOffSet{
		/** 赤 */
		REDOFFSET=2,
		/** 緑 */
		GREENOFFSET=1,
		/** 青 */
		BLUEOFFSET=0
	};

	/** コンストラクタ */
	XImage() ;

	/** コンストラクタ
	 *	@param	img	イメージデータ
	 */
	XImage(const XImage &img);
	/** デストラクタ */
	virtual ~XImage();
	XImage &operator=(const XImage &img);

	/**	@brief	メモリの確保
	 *	@param	cc	カラーカウント
	 *	@param	bits	Bitカウント
	 *	@param	ww	幅
	 *	@param	hh	高さ
	 */
	void SetSize(unsigned int cc,
				unsigned int bits,
				unsigned int ww,
				unsigned int hh);
	/**	イメージデータのデータをセットします。
	 *	@param	pImageDat	セットすデータ
	 *	@param	nImageDataSize	データサイズ
	 */
	void CopyImageData(const unsigned char	*pImageDat, int nImageDataSize);
	/**	@brief	イメージデータのデータを取得します。
	 *	@param	pImageDat	取得するイメージデータを受け取るアドレス
	 *	@param	nImageDataSize	取得するイメージデータのサイズ
	 */
	void GetImageData(unsigned char *pImageDat, int nImageDataSize);


	/**	カラーマップからの、指定したインデックスのColorMapEntryを取得します
		@param	index	インデックス
		@return	ColorMapEntry
	*/
	ColorMapEntry& GetColorMap(unsigned int index);
	/** @copydoc XImage::GetColorMap(unsigned int) */
	ColorMapEntry  GetColorMap(unsigned int index) const;


	/**	@brief	指定した位置のバイトを取得
		@param	xx	位置
		@return	バイト
	*/
	XUBYTE1 *operator[](unsigned int xx) const;

	
	/**	指定した高さの位置のバイナリーデータのポジションを取得します。
	 *	@param	nYIndex	オフセットを取得する高さ
	 *	@return	指定した高さの位置のバイナリーデータのポジション
	 */
	unsigned int OffsetYPos(unsigned int nYIndex);
	/**	指定した幅の位置のバイナリーデータのポジションを取得します。
	 *	@param	nYIndex	オフセットを取得する幅
	 *	@return	指定した幅の位置のバイナリーデータのポジション
	 */
	unsigned int OffsetXPos(unsigned int nXIndex);

	//	イメージ情報をクリアします
	void ClearImage();

	/**	イメージデータのデータの先頭アドレスを取得します。
	 *	@return	イメージデータのデータの先頭アドレス
	 */
	XUBYTE1 *ImageData() ;

	/**	イメージの幅を取得します。
	 *	@return	イメージの幅
	 */
	unsigned int GetWidth() const;
	/**	イメージの高さを取得します。
	 *	@return	イメージの高さ
	 */
	unsigned int GetHeight() const;
	/**	イメージのビットカウントを取得します。
	 *	@return	イメージのビットカウント
	 */
	unsigned int GetBitCount() const;
	/**	イメージのカラーカウントを取得します。
	 *	@return	イメージのカラーカウント
	 */
	unsigned int GetColorCount() const;
	/**	@brief	指定した位置の色成分を取得します。
	 *	@param	row	取得するYの位置
	 *	@param	col	取得するXの位置
	 *	@param	red	取得する赤成分を取得する参照値
	 *	@param	green	取得する緑成分を取得する参照値
	 *	@param	blue	取得する青成分を取得する参照値
	 */
	void GetRgb(unsigned int row, unsigned int col, XUBYTE1 &red, XUBYTE1 &green, XUBYTE1 &blue) const ;
	/**	24ビットから、8ビットのイメージに変換します。
	 *	@param	img	変換するイメージ
	 */
	void EightBitQuantization(const XImage& img) ;

	/**	プログレス関数をセットします。
		@param	func	プログレス関数
		@param	data	ユーザーデータ
	*/
	void SetProgressFunction(XIMAG_PROGRESSFUNCTION func, void *data);

	enum { RowRounding = 4 } ;

	/**	画像データを扱うための実際の幅を取得します。実際の画像の幅ではありません。
	 * @note 画像データの幅は、4ビット単位でなければならない。
	 * @return	画像データを扱うための実際の幅
	*/
	unsigned int GetBytesPerRow() const;

	static int SQUARE (int xx)
	{
		return xx * xx ;
	}

	/**	@brief	DIBの幅が４の倍数でないといけないので幅から、画面表示用の幅を求めます
	 *	@param	width	幅
	 *	@return　画像データを扱うための実際の幅
	 */
	static unsigned int RoundRow(unsigned int width)
	{
		unsigned int result = (width + XImage::RowRounding - 1) & ~(XImage::RowRounding - 1) ;
		return result ;
	}

	XUBYTE1 * OffsetOf(int x, int y) const;
	bool SetPixel(int x, int y, int r, int g, int b);
	bool SetPixelNo(int x, int y, int no);
	XUBYTE4 GetPixel(int x, int y) const;
	XUBYTE4 GetPixelNo(int x, int y) const;
  
protected:
	void DoCopy (const XImage &) ;
private:
	unsigned int			m_nRowWidth ;
	unsigned int			m_nBitCount ;     // (1, 2, 4, 8, or 24)
	unsigned int			m_nImageWidth ;
	unsigned int			m_nImageHeight ;
	unsigned char			*m_pImageData ;
	unsigned int			m_nColorCount ;   // ColorMapEntryのカウント
	ColorMapEntry			*m_pColorMap ;    // 8-bitイメージ用のカラーマップ

	// データをクリアします
	void ClearData () ;

	struct ColorUsage
	{
		XUBYTE1 m_cColors[3] ;
		XUBYTE4 m_nUsage ;
		ColorUsage *m_pNext[3] ;
	} ;

	struct ColorUsageTable
	{
		ColorUsage *m_pLists[256][3];
		unsigned int m_nColorCount ;
	} ;

	struct ColorArea
	{
		struct
		{
			XUBYTE1 m_low;
			XUBYTE1 m_high;
		} m_ColorValues[3];
		unsigned int m_nColorCount;
		unsigned int m_nPixelCount;
	};

	int LargestColorRange (ColorArea &area) ;
	void AddColor (XUBYTE1 red, XUBYTE1 green, XUBYTE1 blue) ;
	void SplitAreaInHalf (unsigned int depth, unsigned int retrydepth, unsigned int area, unsigned int splitcolor) ;
	void CreateColor (unsigned int color) ;
	ColorUsage *FindColor (XUBYTE1 red, XUBYTE1 green, XUBYTE1 blue) ;
	void FindColorUsage (const XImage &image) ;
	void FreeColorQuantizationData () ;
	unsigned int QuantizedColor (XUBYTE1 red, XUBYTE1 green, XUBYTE1 blue) ;
	void QuantizeSourceImage (const XImage &image) ;
	void CallProgressFunction (unsigned int, unsigned int, unsigned int) ;

	ColorUsageTable			*m_pCcolorUsage ;
	ColorArea				*m_pColorAreas ;
	unsigned int			m_nColorAreaCount ;

	XIMAG_PROGRESSFUNCTION	m_pProgressFunction;
	void					*m_pProgressData;
};


class XImgTools
{
public:
	// ４Byteの色データから赤色を取り出します
	static XUBYTE1 _GetRvalue(XUBYTE4 rgb);
	// ４Byteの緑データから赤色を取り出します
	static XUBYTE1 _GetGvalue(XUBYTE4 rgb);
	// ４Byteの青データから赤色を取り出します
	static XUBYTE1 _GetBvalue(XUBYTE4 rgb);
	// XImage::ColorMapEntryから４Byteの色データを作成します。
	static XUBYTE4 ColorMapEntry2UBYTE4(const XImage::ColorMapEntry &rgb);
	// RGBの代わり
	static XUBYTE4 __RGB(XUBYTE4 r, XUBYTE4 g, XUBYTE4 b);
	// ２つの色の距離を求める。距離の二乗を返す。
	static XUBYTE4 GetColorDistance(const XUBYTE4 c1, const XUBYTE4 c2);
	// 引数のカラーテーブルのなかで、一番近い色番号を返す。
	static XUBYTE4 FindNearColorNo(const XImage src, int colnum, XCOLORREF c );
	//	色を明度を考慮したグレイスケール(0.0～1.0)に変換する
	static double GetGrayScale(XUBYTE4 col);
	// 24BITイメージのカラーカウントを求めます。
	static XUBYTE4 GetColorCount24(const XImage &src);
	// デフォルトパレットを作成します。
	static void MakeDefPallet(XImage &src);
	// 色を反転します。
	static bool ReversImage(XImage &src);
	// 
	static bool ColorConvert(XImage &src, int rc, int gc, int bc);

};


#endif //__X_IMAGE_HEASER_FILE__

