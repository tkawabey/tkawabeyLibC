/**
 * @file ximageconv.h
 * @brief 画像イメージ変換管理クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_IMAGE_CONVERT_HEASER_FILE__
#define __X_IMAGE_CONVERT_HEASER_FILE__

#include "ximage.h"

class XImageConverter;

//const int XIMGE_SAMPLING_WIDTH = 8 ;
//const int XIMGE_SAMPLING_SIZE = XIMGE_SAMPLING_WIDTH * XIMGE_SAMPLING_WIDTH;


typedef XUBYTE2 XIMGE_SAMPLE_12BIT;
typedef XUBYTE1 XIMGE_SAMPLE_8BIT;
typedef XIMGE_SAMPLE_8BIT XIMGE_SAMPLE; 


const int XIMGE_MINSAMPLEVALUE  = 0 ;
const int XIMGE_MAX8BITSAMPLEVALUE = 255 ; 
const int XIMGE_MAX12BITSAMPLEVALUE = 4095 ;
const int XIMGE_MAXSAMPLEVALUE = XIMGE_MAX8BITSAMPLEVALUE ;
const int XIMGE_8BITMIDPOINTSAMPLEVALUE = 128 ;
const int XIMGE_12BITMIDPOINTSAMPLEVALUE = 2048 ;
const int XIMGE_MIDPOINTSAMPLEVALUE = XIMGE_MAX8BITSAMPLEVALUE ;

//typedef XBYTE2 XIMGE_CoefficientBlock[XIMGE_SAMPLING_SIZE];


typedef void (*XIMAGE_COMV_PROGRESSFUNCTION)(XImageConverter &pThis,
									  void *data,
                                      unsigned int currentpass,
                                      unsigned int passcount,
                                      unsigned int progress,
                                      bool &cancel) ;


#define BARCO (0)
#define BARCR (20)
#define BARDO (20)
#define BARDR (80)
#define BAREO (100)
#define BARER (300)
#define BARFO (400)
#define BARFR (50)
#define BARAO (450)
#define BARAR (350)
#define BARBO (800)
#define BARBR (200)

#define CUBEX (40)
#define CUBEY (40)
#define CUBEZ (40)
#define CUBEZO (64*512*16)
#define CUBEALL (CUBEX*CUBEY*CUBEZ)
#define CUBEH (64*2560*16)
#define CUBESTEP (64*128*16)
#define CUBESTEPH (64*64*16)





/**
 * 画像イメージ変換管理クラス
 */
class XImageConverter
{
public:
	/** コンストラクタ */
	XImageConverter();
	/** デストラクタ */
	virtual ~XImageConverter();



	typedef struct _PALSTR
	{
		int c;
		int z;
	}PALSTR, *LPPALSTR;

	typedef struct _Anime2line
	{
		int pal;
		int d;
	}Anime2line, *LPAnime2line;

	typedef struct _MRX
	{
		double m11;
		double m12;
		double m21;
		double m22;
	}MRX, *LPMRX;

	typedef struct _GCS
	{
		int X;
		int Y;
		int Z;
	}GCS, *LPGCS;

	typedef struct RMA
	{
		double x;
		double y;
		double z;
		double c;
	};

	typedef struct MED
	{
		GCS gcs;
		int cnt;
		double flag;
		//	int flag;
	};

	typedef struct MEDBOX
	{
		double strain;
		int bufstart;
		int bufnum;
		GCS mid;
		int rgb;

		RMA prin;
		RMA std;
		RMA avg;
		double firstd;
	};


	//
	//
	//
	class ClorSpace
	{
	public:
		ClorSpace();
		~ClorSpace();

		int MaskRGB(int rgb );
		double DistanceGCSd(GCS *g1,int x2,int y2,int z2) const;
		double DistanceGCS(GCS *g1,GCS *g2) const;
		int ConvGCStoRGB(int X,int Y,int Z) const;
		void ConvRGBtoGCS(GCS *gcs,unsigned int rgb) const;
		void Create(int b,int r,int g,int p);

	protected:
		bool MakeMatrix(MRX *a,MRX *b,double Cbm,double Crm,double Cgm);
		int  MaskRGBsub(int m[],int mc,int in,int s);
		void MakeMatrixsub(MRX *M,MRX *N);

		MRX    m_M1;
		MRX    m_M2;

		int    m_MaskRTable[256];
		int    m_MaskGTable[256];
		int    m_MaskBTable[256];
		int    m_MaskRCount;
		int    m_MaskGCount;
		int    m_MaskBCount;

		double m_dGtable[256];
		double m_dR2table[256];
		double m_dG2table[256];
		double m_dB2table[256];

		int    m_PaletteBitMask;
		int    m_PaletteBitMaskAdd;
	};

	//
	//
	//
	class DivideBox  
	{
	public:
		DivideBox();
		virtual ~DivideBox();
		bool divideBox(MEDBOX *medbox,MED **medbuf,int mxa,int oi,ClorSpace *cs);
		void MakeMEDbox(MEDBOX *medbox,MED **medbuf,int ad,int bs,int bn,ClorSpace *cs);
	//	double dsqr(double a) const { return a*a; };
		static void DivHsort(MED **ab, int n);
	protected:
		double Setrma(MED **medbuf,int bs,int bn,RMA *prin,RMA *std,RMA *avg);
		void Jacobi(int n, double a[3][3],double u[3][3], double eps);
		int FindSquareDiv(MED **ix,int st,int n);


		double m_Jca[3][3];
		double m_Jcw[3][3];
	};


	//  ディザ方法 
	enum DitherMode
	{
		//(なし)
		//gPeanos
		//誤差拡散
		//平均誤差
		//アニメ
		mdDitherMode_None,
		mdDitherMode_gPeanos,
		mdDitherMode_ObservationalDiffusion,
		mdDitherMode_AverageObservational,
		mdDitherMode_Anime,
	};

	enum PalettePrecision
	{
		/*
		Web
		332
		333
		444
		555
		565
		666
		777
		888
		*/
		PalettePrecision_Web,
		PalettePrecision_332,
		PalettePrecision_333,
		PalettePrecision_444,
		PalettePrecision_555,
		PalettePrecision_565,
		PalettePrecision_666,
		PalettePrecision_777,
		PalettePrecision_888
	};

	enum PictureMask
	{
		PictureMask_5Bits    = 7,
		PictureMask_6Bits    = 3,
		PictureMask_7Bits    = 1,
		PictureMask_8Bits    = 0
	};



	/**	イメージをグレースケールに変換します。
	 *	@param	image	変換するイメージ
	 *	@retval true 成功
	 *	@retval false 失敗
	 */
	bool convertGrayscale(XImage &image);



	// 出力制度の取得＆設定
	PalettePrecision GetPalettePrecision();
	void SetPalettePrecision(PalettePrecision nPalettePrecision);
	// 入力制度の取得＆設定
	PictureMask GetPicturMask();
	void SetPicturMask(PictureMask nPictureMask);
	// 減色エンジン赤(0-20)の取得＆設定
	unsigned int GetCrMag();
	void SetCrMag(unsigned int nCrMag);
	// 減色エンジン緑(0-20)の取得＆設定
	unsigned int GetCgMag();
	void SetCgMag(unsigned int nCgMag);
	// 減色エンジン青(0-20)の取得＆設定
	unsigned int GetCbMag();
	void SetCbMag(unsigned int nCbMag);
	// カラー数の取得＆設定
	unsigned int GetIndexVolume();
	void SetIndexVolume(unsigned int nIndexVolume);
	// ディザレベルの取得＆設定
	unsigned int GetDitherLevel();
	void SetDitherLevel(unsigned int nDitherLevel);
	// ディザモードの取得＆設定
	DitherMode GetDitherMode();
	void SetDitherMode(DitherMode nDitherMode);
	// 固定16色付加の取得＆設定
	BOOL GetAdd16Pallete();
	void SetAdd16Pallete(BOOL bAdd16Pallete);
	// 透過パレットの取得＆設定
	int GetTransparencyColor();
	void SetTransparencyColor(int nTransparencyColor);


    


	static int           m_fbpattern[2][3];
	static int           m_maempattern[3][5];



	static int Scaled(double value);
	static XIMGE_SAMPLE RgbToY(XIMGE_SAMPLE red, XIMGE_SAMPLE green, XIMGE_SAMPLE blue);

	void SetProgressFunction(XIMAGE_COMV_PROGRESSFUNCTION pFunc, void *pUserData);
	int Execute(const XImage &m_pSorceImage, XImage &dst);





protected:
	void Bic(const XImage &m_pSorceImage, int m,int n,int a,int b,int c,int d);
	void Biscan(const XImage &m_pSorceImage, int width,int height);
	void Bit(const XImage &m_pSorceImage, int m,int n,int a,int b,int c,int d);
	void Transpose(int *a,int *b,int *c,int *d,int *e,int *f);
	void Link(const XImage &m_pSorceImage, int a, int b);
	void Mtscan(const XImage &m_pSorceImage, int width,int height);
	int FindClosepaletteSub(short int *p,int x,int y,int z);
	int FindClosepalette(int x,int y,int z);
	void MakeCube(int x,int y,int z);
	int GetCubeadr(int x,int y,int z);

	void CallProgressFunction(unsigned int progress);


	unsigned long        m_nCRC;
	/* 出力制度 */
	PalettePrecision     m_nPalettePrecision;
	/* 入力制度(7,3,1,0) */
	PictureMask          m_nPictureMask;
	/* 減色エンジン赤(0-20) */
	int                  m_nCrMag;
	/* 減色エンジン緑(0-20) */
	int                  m_nCgMag;
	/* 減色エンジン青(0-20) */
	int                  m_nCbMag;
	/* カラー数 */
	int                  m_nIndexVolume;
	/* ディザレベル(0～10) */
	int                  m_nDitherLevel;
	/* ディザ方法  */
	DitherMode           m_nDitherMode;
	/* 固定16色付加 */
	BOOL                 m_bAdd16Pallete;
	/* 透過パレットの設定(-1=割り当て無し, 0=左上、1=左下、2=右上、3=右下) */
	int                  m_nTransparencyColor;

	XIMAGE_COMV_PROGRESSFUNCTION	m_ProgressFunction ;
	void				*m_pProgressData;


	MEDBOX               m_Medbox[768+1];
	GCS                  m_GcsTable[256];
	MED                  *m_pMedbuf;
	MED                  **m_pSmedbuf;
	int                  m_nTransPal;
	int                  *m_pMadePalette;
	unsigned char        *m_pPalbuf;
	int                  *m_pPicBuf;
	int                  m_nPicx;			// ソースイメージの幅
	int                  m_nPicy;			// ソースイメージの高さ
	int                  m_nGpsx;
	int                  m_nGpsy;
	int                  m_nGpsc;
	int                  m_nGpsdx;
	int                  m_nGpsdy;
	int                  m_nGpsdz;

	int                  m_nMxdx;
	int                  m_nMxdy;
	int                  m_nMxdz;
	int                  m_nMakePalettEnum;
	short int            *m_pQcube[CUBEALL];
	int                  m_nTrans;


	ClorSpace            m_ColorSpace;
	DivideBox            m_DivideBox;
};
#endif /* __X_IMAGE_CONVERT_HEASER_FILE__ */
