/**
 * @file ximage.h
 * @brief �摜�C���[�W�Ǘ��N���X
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

/** �摜�C���[�W�Ǘ��N���X�G���[��` */
enum XIMG_ERR
{
	/** �X�g���[����EOF�ɒB���� */
	XIMG_ERR_END_STREAM,
	/** �n�t�}���̒����R�[�h�G���[�B�n�t�}���̒����R�[�h�́A0�|18�̊� */
	XIMG_ERR_BAD_HUFFMAN_LENGTH,
	/** �����O�X�I�[�o�[*/
	XIMG_ERR_BAD_LENGTH_OUT_OF_RANGE,
	/** �n�t�}���W���l�G���[ */
	XIMG_ERR_BAD_HUFFMAN_LITERAL,
	/** �n�t�}���̋����G���[ */
	XIMG_ERR_BAD_HUFFMAN_DISTANCE,
	/** �T�|�[�g����Ă��Ȃ��r�b�g�J�E���g */
	XIMG_ERR_UNSUPPORT_BITCOUNT,
	/** �J���[�}�b�v�G���g���ɂ��Ă��ꂽ�C���f�b�N�X���s���ł� */
	XIMG_ERR_SUBSCRIPT_OUT_RANGE,
	/** �w�肵���AROW�܂���COL�̒l���͈͂𒴂��Ă��܂� */
	XIMG_ERR_ROW_COLMN_OUT_OF_RANGE,
	/** �w�肳�ꂽ�J���[�}�b�v�̃J�E���g���s���ł� */
	XIMG_ERR_QUATIZATION_AREA_COUNT_INVALID,
	/**  */
	XIMG_ERR_COLOR_NO_QUATIZ,
	/** �C���[�W�f�[�^�ǂݍ��݊����O�ɁA�X�g���[����EOF�ɒB���� */
	XIMG_ERR_END_OF_STREAM,
	/** �T�|�[�g����Ă��Ȃ����k�`���ł� */
	XIMG_ERR_UNSUPPORT_COMPRESSION,
	/** �s���ȃf�[�^ */
	XIMG_ERR_CORUPPUT_DATA,
	/** �T�|�[�g����Ă��Ȃ��r�b�g�s�N�Z���l */
	XIMG_ERR_UNSUPORT_BITS_PER_PIXL,
	/** ���������m�ۂł��Ȃ� */
	XIMG_ERR_MALLOC,
	/** API�G���[���������� */
	XIMG_ERR_API,

	/** �r�b�g�}�b�v�t�@�C���łȂ� */
	XIMG_ERR_NOT_BMP_FILE,
	/** �T�|�[�g����Ă��Ȃ��r�b�g�}�b�v�t�@�C���w�b�_�[�ł� */
	XIMG_ERR_UNSUPPORT_BIT_MAP_HEADER,

	/** �w�肳�ꂽ���F�o�̓G���W���̃��\�b�h���s�� */
	XIMG_ERR_INVALID_PALLET_PRECISION,
	/** �w�肳�ꂽ���F���x�l���s��(0-20) */
	XIMG_ERR_INVALID_CRMA,
	/** �w�肳�ꂽ�s�N�`���[�}�X�N�l���s�� */
	XIMG_ERR_INVALID_PICTURE_MASK,
	/** �w�肳�ꂽ�f�B�U���x�����s��(0-10) */
	XIMG_ERR_INVALID_DITHER_LEVEL,
	/** �w�肳�ꂽ�f�B�U���[�h���s�� */
	XIMG_ERR_INVALID_DITHER_MODE,
	/** �w�肳�ꂽ�ϊ��J���[�l���s�� */
	XIMG_ERR_INVALID_TRANCE_COLOR,

	/** �s�� */
	XIMG_ERR_ROW_UNKOWN,
	/** �v���O���X�ŃL�����Z�����ꂽ */
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
 * �C���[�W�Ǘ��f�[�^��O�N���X
 */
class XImageException : public XExcept
{
public:
	/** �R���X�g���N�^ */
	XImageException();
	/** �f�X�g�g���N�^ */
	virtual ~XImageException() throw ();

	static void Throw(XIMG_ERR err);

	XIMG_ERR m_err;
};

/**
 * �C���[�W�t�@�C���ǂݍ��݃N���X
 */
class XImageStream : public XFile
{
public:
	/** �R���X�g���N�^ */
	XImageStream();
	/** �f�X�g���N�^ */
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
	/* ���݂̈ʒu */
	unsigned long		m_dwCurrentPos;
	/* �t�@�C���̃T�C�Y */
	unsigned long		m_dwFileSize;
	unsigned long		m_dwReadedCount;
	unsigned long       m_dwReaded;
};

/**
 * �C���[�W�G���R�[�h�A�f�R�[�h�A�N���X�B
 */
class XImageCoder
{
public:
	/** �R���X�g���N�^ */
	XImageCoder();
	/**	@brief	�R���X�g���N�^
	 *	@param	source	�R�s�[���f�[�^
	 */
	XImageCoder(const XImageCoder& source);
	/** �f�X�g���N�^ */
	virtual ~XImageCoder();

	/**	@brief	�I�u�W�F�N�g�������܂��B
	 *	@param	source	�R�s�[���f�[�^
	 *	@return	���g
	 */
	XImageCoder& operator=(const XImageCoder& source);

	
	/**	�X�e�[�^�X�ʒm�֐����Z�b�g���܂��B
	 *	@param	pfunc	�X�e�[�^�X�ʒm�֐�
	 *	@param	data	���[�U�[��`�f�[�^
	 */
	void SetProgressFunction(XIMG_PROGRESS_FUNC pfunc, void *data);

	/**	�C���[�W�f�[�^���X�V���܂��B
	 *	@return	virtual void
	 */
	virtual void UpdateImage();
protected:
	/* �X�e�[�^�X�ʒm�֐� */
	XIMG_PROGRESS_FUNC	m_ProgressFunction ;
	/* ���[�U�[��`�f�[�^ */
	void				*m_pProgressData;
};

/**
 * �摜�C���[�W�f�R�[�_�[�x�[�X�N���X
 */
class XImageDecoder : public XImageCoder
{
public:
	/** �f�X�g���N�^ */
	virtual ~XImageDecoder(){}
	/**	�C���[�W�f�[�^��ǂݍ��݂܂��B
	 *	@param	lpszFileName	�ǂݍ��ރt�@�C���̃p�X����
	 *	@param	img	�ǂݍ��񂾃C���[�W���󂯎��XImage�C���X�^���X
	 */
	virtual void ReadImageFile(const char* lpszFileName, XImage& img) = 0;
};

/**
 * �摜�C���[�W�G���R�[�_�[�x�[�X�N���X
 */
class XImageEncoder : public XImageCoder
{
public:
	/** �f�X�g���N�^ */
	virtual ~XImageEncoder(){}
	/**	�C���[�W�f�[�^���t�@�C���ɏ������݂܂��B
	 *	@param	lpszFileName	�������ރt�@�C���̃p�X
	 *	@param	img	�������ރC���[�W��XImage�C���X�^���X
	 *	@exception XImageException* �G���[�C���X�^���X
	*/
	virtual void WriteImageFile(const char* lpszFileName, XImage& img) = 0;
};

/**
 * �C���[�W�f�[�^
 */
class XImage
{
public:
	/**
	 * 24Bit�ȏ�ȊO�̃r�b�g�}�b�v�C���[�W�̃J���[�}�b�v�̒�`
	 */
	struct ColorMapEntry
	{
		/** �� */
		XUBYTE1 blue;
		/** �� */
		XUBYTE1 green;
		/** �� */
		XUBYTE1 red;
	};
	/**
	 * �F�̃I�t�Z�b�g�ʒu
	 */
	enum ColorOffSet{
		/** �� */
		REDOFFSET=2,
		/** �� */
		GREENOFFSET=1,
		/** �� */
		BLUEOFFSET=0
	};

	/** �R���X�g���N�^ */
	XImage() ;

	/** �R���X�g���N�^
	 *	@param	img	�C���[�W�f�[�^
	 */
	XImage(const XImage &img);
	/** �f�X�g���N�^ */
	virtual ~XImage();
	XImage &operator=(const XImage &img);

	/**	@brief	�������̊m��
	 *	@param	cc	�J���[�J�E���g
	 *	@param	bits	Bit�J�E���g
	 *	@param	ww	��
	 *	@param	hh	����
	 */
	void SetSize(unsigned int cc,
				unsigned int bits,
				unsigned int ww,
				unsigned int hh);
	/**	�C���[�W�f�[�^�̃f�[�^���Z�b�g���܂��B
	 *	@param	pImageDat	�Z�b�g���f�[�^
	 *	@param	nImageDataSize	�f�[�^�T�C�Y
	 */
	void CopyImageData(const unsigned char	*pImageDat, int nImageDataSize);
	/**	@brief	�C���[�W�f�[�^�̃f�[�^���擾���܂��B
	 *	@param	pImageDat	�擾����C���[�W�f�[�^���󂯎��A�h���X
	 *	@param	nImageDataSize	�擾����C���[�W�f�[�^�̃T�C�Y
	 */
	void GetImageData(unsigned char *pImageDat, int nImageDataSize);


	/**	�J���[�}�b�v����́A�w�肵���C���f�b�N�X��ColorMapEntry���擾���܂�
		@param	index	�C���f�b�N�X
		@return	ColorMapEntry
	*/
	ColorMapEntry& GetColorMap(unsigned int index);
	/** @copydoc XImage::GetColorMap(unsigned int) */
	ColorMapEntry  GetColorMap(unsigned int index) const;


	/**	@brief	�w�肵���ʒu�̃o�C�g���擾
		@param	xx	�ʒu
		@return	�o�C�g
	*/
	XUBYTE1 *operator[](unsigned int xx) const;

	
	/**	�w�肵�������̈ʒu�̃o�C�i���[�f�[�^�̃|�W�V�������擾���܂��B
	 *	@param	nYIndex	�I�t�Z�b�g���擾���鍂��
	 *	@return	�w�肵�������̈ʒu�̃o�C�i���[�f�[�^�̃|�W�V����
	 */
	unsigned int OffsetYPos(unsigned int nYIndex);
	/**	�w�肵�����̈ʒu�̃o�C�i���[�f�[�^�̃|�W�V�������擾���܂��B
	 *	@param	nYIndex	�I�t�Z�b�g���擾���镝
	 *	@return	�w�肵�����̈ʒu�̃o�C�i���[�f�[�^�̃|�W�V����
	 */
	unsigned int OffsetXPos(unsigned int nXIndex);

	//	�C���[�W�����N���A���܂�
	void ClearImage();

	/**	�C���[�W�f�[�^�̃f�[�^�̐擪�A�h���X���擾���܂��B
	 *	@return	�C���[�W�f�[�^�̃f�[�^�̐擪�A�h���X
	 */
	XUBYTE1 *ImageData() ;

	/**	�C���[�W�̕����擾���܂��B
	 *	@return	�C���[�W�̕�
	 */
	unsigned int GetWidth() const;
	/**	�C���[�W�̍������擾���܂��B
	 *	@return	�C���[�W�̍���
	 */
	unsigned int GetHeight() const;
	/**	�C���[�W�̃r�b�g�J�E���g���擾���܂��B
	 *	@return	�C���[�W�̃r�b�g�J�E���g
	 */
	unsigned int GetBitCount() const;
	/**	�C���[�W�̃J���[�J�E���g���擾���܂��B
	 *	@return	�C���[�W�̃J���[�J�E���g
	 */
	unsigned int GetColorCount() const;
	/**	@brief	�w�肵���ʒu�̐F�������擾���܂��B
	 *	@param	row	�擾����Y�̈ʒu
	 *	@param	col	�擾����X�̈ʒu
	 *	@param	red	�擾����Ԑ������擾����Q�ƒl
	 *	@param	green	�擾����ΐ������擾����Q�ƒl
	 *	@param	blue	�擾����������擾����Q�ƒl
	 */
	void GetRgb(unsigned int row, unsigned int col, XUBYTE1 &red, XUBYTE1 &green, XUBYTE1 &blue) const ;
	/**	24�r�b�g����A8�r�b�g�̃C���[�W�ɕϊ����܂��B
	 *	@param	img	�ϊ�����C���[�W
	 */
	void EightBitQuantization(const XImage& img) ;

	/**	�v���O���X�֐����Z�b�g���܂��B
		@param	func	�v���O���X�֐�
		@param	data	���[�U�[�f�[�^
	*/
	void SetProgressFunction(XIMAG_PROGRESSFUNCTION func, void *data);

	enum { RowRounding = 4 } ;

	/**	�摜�f�[�^���������߂̎��ۂ̕����擾���܂��B���ۂ̉摜�̕��ł͂���܂���B
	 * @note �摜�f�[�^�̕��́A4�r�b�g�P�ʂłȂ���΂Ȃ�Ȃ��B
	 * @return	�摜�f�[�^���������߂̎��ۂ̕�
	*/
	unsigned int GetBytesPerRow() const;

	static int SQUARE (int xx)
	{
		return xx * xx ;
	}

	/**	@brief	DIB�̕����S�̔{���łȂ��Ƃ����Ȃ��̂ŕ�����A��ʕ\���p�̕������߂܂�
	 *	@param	width	��
	 *	@return�@�摜�f�[�^���������߂̎��ۂ̕�
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
	unsigned int			m_nColorCount ;   // ColorMapEntry�̃J�E���g
	ColorMapEntry			*m_pColorMap ;    // 8-bit�C���[�W�p�̃J���[�}�b�v

	// �f�[�^���N���A���܂�
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
	// �SByte�̐F�f�[�^����ԐF�����o���܂�
	static XUBYTE1 _GetRvalue(XUBYTE4 rgb);
	// �SByte�̗΃f�[�^����ԐF�����o���܂�
	static XUBYTE1 _GetGvalue(XUBYTE4 rgb);
	// �SByte�̐f�[�^����ԐF�����o���܂�
	static XUBYTE1 _GetBvalue(XUBYTE4 rgb);
	// XImage::ColorMapEntry����SByte�̐F�f�[�^���쐬���܂��B
	static XUBYTE4 ColorMapEntry2UBYTE4(const XImage::ColorMapEntry &rgb);
	// RGB�̑���
	static XUBYTE4 __RGB(XUBYTE4 r, XUBYTE4 g, XUBYTE4 b);
	// �Q�̐F�̋��������߂�B�����̓���Ԃ��B
	static XUBYTE4 GetColorDistance(const XUBYTE4 c1, const XUBYTE4 c2);
	// �����̃J���[�e�[�u���̂Ȃ��ŁA��ԋ߂��F�ԍ���Ԃ��B
	static XUBYTE4 FindNearColorNo(const XImage src, int colnum, XCOLORREF c );
	//	�F�𖾓x���l�������O���C�X�P�[��(0.0�`1.0)�ɕϊ�����
	static double GetGrayScale(XUBYTE4 col);
	// 24BIT�C���[�W�̃J���[�J�E���g�����߂܂��B
	static XUBYTE4 GetColorCount24(const XImage &src);
	// �f�t�H���g�p���b�g���쐬���܂��B
	static void MakeDefPallet(XImage &src);
	// �F�𔽓]���܂��B
	static bool ReversImage(XImage &src);
	// 
	static bool ColorConvert(XImage &src, int rc, int gc, int bc);

};


#endif //__X_IMAGE_HEASER_FILE__

