/**
 * @file xstrcordeconv.h
 * @brief �����R�[�h�ϊ��N���X�Q
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_STRCORDCONV_HEASER_FILE__
#define __X_STRCORDCONV_HEASER_FILE__
#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"


/**
 * �����R�[�h�ϊ��N���X
 */
class XLIB_API XStrBuffCodeConv : public XStrBuff
{
public:
	/**
	 * �R�[�h�̎��
	 */
	enum  CodeType { 
		UNKNOWN,		/** Unkown */
		SJIS,			/** SJIS */
		EUC,			/** EUC */
		JIS,			/** JIS */
		SMB_HEX,		/** Samba-HEX */
		SMB_CAP			/** Samba-CAP */
	};
public:
	/** @copydoc XStrBuff::XStrBuff() */
	XStrBuffCodeConv();
	/** @copydoc XStrBuff::XStrBuff(const char* ) */
	XStrBuffCodeConv(const char* lpsz);
	/** @copydoc XStrBuff::XStrBuff(const wchar_t* ) */
	XStrBuffCodeConv(const wchar_t* lpsz);
	/** @copydoc XStrBuff::operator=(const wchar_t* ) */
	const XStrBuffCodeConv& operator=(const wchar_t* lpsz);
	/** @copydoc XStrBuff::operator=(const char* ) */
	const XStrBuffCodeConv& operator=(const char* lpsz);

	/**
	 * �����R�[�h��ϊ����܂��B
	 * @param incode �ϊ��O�̕����R�[�h
	 * @param outcode �ϊ���̕����R�[�h
	 * @param dwSrcSize �\�[�X�̃T�C�Y�B -1 ���w�肷��ƁA���g�̃C���X�^���X��ω������ɃZ�b�g���܂��B����ȊO�́ApOutBuf,�ɕϊ��������Z�b�g���܂��B
	 * @param pOutBuf �ϊ���̃o�b�t�@�BNULL ���w�肷��ƁA���g�̃C���X�^���X��ω������ɃZ�b�g���܂��B����ȊO�́ApOutBuf,�ɕϊ��������Z�b�g���܂��BdwSrcSize,pdwOutSize �������Ɏw�肵�Ă��������B
	 * @param pdwOutSize �ϊ���̃o�b�t�@�̃T�C�Y�BNULL ���w�肷��ƁA���g�̃C���X�^���X��ω������ɃZ�b�g���܂��B����ȊO�́ApOutBuf,�ɕϊ��������Z�b�g���܂��BdwSrcSize�ApOutBuf�������Ɏw�肵�Ă��������B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Convert(CodeType incode , CodeType outcode, unsigned long dwSrcSize = -1, XBuff *pOutBuf = NULL, unsigned long *pdwOutSize = NULL);

	/**	@brief	UNICODE������UTF8�����ɕϊ�
	 *
	 *	@param	pszSrc	UNICODE����
	 *	@param	srcLen	UNICODE�����̒���
	 *	@param	dst	UTF8�����ɕϊ������l���󂯎��o�b�t�@
	 *	@param	bCESU8	a parameter of type bool
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool convertUnicodeToUTF8(const wchar_t* pszSrc, size_t srcLen, XStrBuff& dst, bool bCESU8 );

	/**	@brief	UTF8������UNICODE�����ɕϊ�
	 *
	 *	@param	pszSrc	UTF8����
	 *	@param	srcLen	UTF8�����̒���
	 *	@param	dst	UNICODE�����ɕϊ������l���󂯎��o�b�t�@
	 *	@param	pfCESU	a parameter of type bool *
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool convertUTF8ToUnicode(const char* size_t, int srcLen, XBuff& dst, bool *pfCESU );


	/**	@brief	Native������UTF8�����ɕϊ�
	 *
	 *	@param	pszSrc	Native����
	 *	@param	srcLen	Native�����̒���
	 *	@param	dst	UTF8�������󂯎��o�b�t�@
	 *	@param	pfCESU	a parameter of type bool *
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool convertNativeToUTF8(const char* pszSrc, XStrBuff& dst, bool bCESU8 );

	/**	@brief	Native������UTF8�����ɕϊ�
	 *
	 *	@param	pszSrc	Native����
	 *	@param	srcLen	Native�����̒���
	 *	@param	dst	UTF8�������󂯎��o�b�t�@
	 *	@param	pfCESU	a parameter of type bool *
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool convertNativeToUTF8(const wchar_t* pszSrc, XStrBuff& dst, bool bCESU8 );


	/**	@brief	UTF8������Native�ɕϊ�
	 *
	 *	@param	pszSrc	UTF8����
	 *	@param	srcLen	UTF8�����̒���
	 *	@param	dstVaitive�����ɕϊ������l���󂯎��o�b�t�@
	 *	@param	pfCESU	a parameter of type bool *
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool convertUTF8ToNative(const char* pszSrc, XStrBuff& dst, bool *pfCESU );

protected:
	/** ������ */
	void CommonConstruct();
	/**
	 * EUC->SJIS �ϊ�
	 * @param pOutBuf �o�͐�o�b�t�@
	 * @param dwSrcSize �o�b�t�@�T�C�Y
	 * @return �ϊ����ꂽ�o�C�g��
	 */
	unsigned long ConvEUCtoSJIS(XBuff *pOutBuf, unsigned long dwSrcSize);
	/**
	 * JIS->SJIS �ϊ�
	 * @param pOutBuf �o�͐�o�b�t�@
	 * @param dwSrcSize �o�b�t�@�T�C�Y
	 * @return �ϊ����ꂽ�o�C�g��
	 */
	unsigned long ConvJIStoSJIS(XBuff *pOutBuf, unsigned long dwSrcSize);
	/**
	 * SJIS->EUC �ϊ�
	 * @param pOutBuf �o�͐�o�b�t�@
	 * @param dwSrcSize �o�b�t�@�T�C�Y
	 * @return �ϊ����ꂽ�o�C�g��
	 */
	unsigned long ConvSJIStoEUC(XBuff *pOutBuf, unsigned long dwSrcSize);
	/**
	 * SJIS->JIS �ϊ�
	 * @param pOutBuf �o�͐�o�b�t�@
	 * @param dwSrcSize �o�b�t�@�T�C�Y
	 * @return �ϊ����ꂽ�o�C�g��
	 */
	unsigned long ConvSJIStoJIS(XBuff *pOutBuf, unsigned long dwSrcSize);

	/**
	 * EUC->SJIS �J�i�����ϊ�
	 * @param Dt ���ȕ����̒l
	 * @param pOut �o�͐�o�b�t�@
	 * @return �ϊ���̕�����
	 */
	char *ConvEUCtoSJISkanaProc(char Dt, char *pOut);
	/**
	 * JIS->SJIS �J�i�����ϊ�
	 * @param Dt ���ȕ����̒l
	 * @param pOut �o�͐�o�b�t�@
	 * @return �ϊ���̕�����
	 */
	char *ConvJIStoSJISkanaProc(char Dt, char *pOut);
	/**
	 * SJIS->EUC �J�i�����ϊ�
	 * @param Dt ���ȕ����̒l
	 * @param pOut �o�͐�o�b�t�@
	 * @return �ϊ���̕�����
	 */
	char *ConvSJIStoEUCkanaProc(char Dt, char *pOut);
	/**
	 * SJIS->JIS �J�i�����ϊ�
	 * @param Dt ���ȕ����̒l
	 * @param pOut �o�͐�o�b�t�@
	 * @return �ϊ���̕�����
	 */
	char *ConvSJIStoJISkanaProc(char Dt, char *pOut);

// static
	/**
	 * �����^�������ɂȂ镶�����`�F�b�N
	 * @param Ch �P�o�C�g�J�^�J�i�R�[�h
	 * @param nDaku ���_�^�����_
	 * @retval 0 ���_�^�����_
	 * @retval 0�ȊO �����R�[�h�ɉ�����l
	 */
	static int AskDakuon(char Ch, char nDaku);
	/**
	 * �P�o�C�g�J�^�J�i��JIS�����R�[�h�ɕϊ�
	 * @param Ch �P�o�C�g�J�^�J�i�R�[�h
	 * @return JIS�����R�[�h
	 */
	static int HanKataToZen(char Ch);
protected:
	/** �������[�h */
	enum KanaKanji {
		CONV_ASCII =		0,		/** ASCII���������� */
		CONV_KANJI =		1,		/** ���������� */
		CONV_KANA	 =		2		/** ���p�J�^�J�i������ */
	};
	/** �������[�h */
	int         m_nKanjiMode;
	/** �������[�h */
	char        m_cKanjiFst;
	/** �������[�h */
	char        m_cKanjiPrev;
	/** �G�X�P�[�v��r */
	int         m_nEscNum;
	/** �G�X�P�[�v�R�[�h */
	char        m_cEscCode[2];
	/** ���ȕϊ����[�h */
	bool        m_nKanaConv;
	/** ���ƕϊ����[�h */
	bool        m_bConvCrLf;
	/** �I�[�R�[�h */
	char        m_cTermCode;
};

/**
 * Base64�̃R�[�_�[�N���X
 */
class XLIB_API XStrBuffBase64Coder :  public XStrBuff
{
public:
	/** @copydoc XStrBuff::XStrBuff() */
	XStrBuffBase64Coder();
	/** @copydoc XStrBuff::XStrBuff(const char* ) */
	XStrBuffBase64Coder(const char* lpsz);
	/** @copydoc XStrBuff::XStrBuff(const wchar_t* ) */
	XStrBuffBase64Coder(const wchar_t* lpsz);
	/** �f�X�g���N�^ */
	virtual ~XStrBuffBase64Coder();
	/** @copydoc XStrBuff::operator=(const char* ) */
	const XStrBuffBase64Coder& operator=(const char* lpsz);
	/** @copydoc XStrBuff::operator=(const wchar_t* ) */
	const XStrBuffBase64Coder& operator=(const wchar_t* lpsz);

	
	/**
	 * �G���R�[�h
	 * @return �G���R�[�h���ꂽ������
	 */
	const TCHAR* Encode();
	/**
	 * �f�R�[�h
	 * @return �f�R�[�h���ꂽ������
	 */
	const TCHAR* Decode();

	/**	@brief	�o�b�t�@���G���R�[�h���܂��B
	 *
	 *	@param	src	�ϊ�����o�b�t�@
	 *	@param	dst	�ϊ���̃f�[�^���󂯎��o�b�t�@
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool encode(XBuff& src, XStrBuff& dst);
	/**	@brief	�o�b�t�@���f�R�[�h���܂��B
	 *
	 *	@param	src	�ϊ�����o�b�t�@
	 *	@param	dst	�ϊ���̃f�[�^���󂯎��o�b�t�@
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool decode(const char* psrc, XBuff& dst);
	/**	@brief	�o�b�t�@���f�R�[�h���܂��B
	 *
	 *	@param	src	�ϊ�����o�b�t�@
	 *	@param	dst	�ϊ���̃f�[�^���󂯎��o�b�t�@
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool decode(const wchar_t* psrc, XBuff& dst);
protected:
	/**
	 * �G���R�[�h
	 * @param lpszSrc �ϊ����f�[�^
	 * @param lpszDest �ϊ���f�[�^�o�b�t�@
	 * @param nSrcLen �ϊ����f�[�^�T�C�Y
	 * @return �G���R�[�h��̃T�C�Y
	 */
	static int encode(const char* lpszSrc, char* lpszDest, int nSrcLen);
	/**
	 * �f�R�[�h
	 * @param lpszSrc �ϊ����f�[�^
	 * @param lpszDest �ϊ���f�[�^�o�b�t�@
	 * @param nSrcLen �ϊ����f�[�^�T�C�Y
	 * @return �f�R�[�h��̃T�C�Y
	 */
	static int decode(const char* lpszSrc, char* lpszDest, int nSrcLen);
};




#include "xlib/xlib_tail.h"
#endif // __X_STRCORDCONV_HEASER_FILE__
