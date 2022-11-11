/**
 * @file xstrbuff.h
 * @brief ������Ǘ��N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_STRBUFF_HEASER_FILE__
#define __X_STRBUFF_HEASER_FILE__
#pragma warning( disable: 4250)
#include "xlib/xlib_head.h"
#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif
#endif //
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>
#include "xlib/xbuff.h"
#include "xlib/xarray.h"



#ifdef _UNICODE
  #ifndef UNICODE
    #define UNICODE 1
  #endif
#endif

#ifdef WIN32
	#include <tchar.h>
#endif // WIN32


#ifndef _TCHAR_DEFINED
 #ifdef _UNICODE
  #define TCHAR	wchar_t
 #else
  #define TCHAR	char
 #endif	// _UNICODE
#endif // TCHAR


#ifndef _T
 #ifdef _UNICODE
	#define _T(x)       L ## x
 #else
	#define _T(x)       x
 #endif	// _UNICODE
#endif /* _T */ 

#define XCOUNTOF(A) (sizeof(A)/sizeof(A[1]))

#define XSTRBUFF_DEF_INFLATE_SIZE	((size_t)256)

/** 
 * ������Ǘ��N���X
 */
class XLIB_API XStrBuff : public XBuff
{
public:
	/** ������̃g���~���O���[�h */
	enum TrimMode {
		TRIM_RIGHTONLY = 1,	/** �E�[�̂� */
		TRIM_LEFTONLY  = 2,	/** ���[�̂� */
		TRIM_BOTH      = 3	/** ���� */
	};

	/**	@brief	�R���X�g���N�^
	 *	@param	iInflateSize	�]���o�b�t�@�̑����T�C�Y
	 */
	XStrBuff(size_t iInflateSize = XSTRBUFF_DEF_INFLATE_SIZE);
	/**
	 * �R���X�g���N�^
	 *	@param lpsz ������������
	 *	@param	iInflateSize	�]���o�b�t�@�̑����T�C�Y
	 */
	XStrBuff(const char* lpsz, size_t size = -1, size_t iInflateSize = XSTRBUFF_DEF_INFLATE_SIZE);
	/**
	 * �R���X�g���N�^
	 *	@param lpsz ������������
	 *	@param	iInflateSize	�]���o�b�t�@�̑����T�C�Y
	 */
	XStrBuff(const wchar_t* lpsz, size_t size = -1, size_t iInflateSize = XSTRBUFF_DEF_INFLATE_SIZE);
	/**
	 * �R���X�g���N�^
	 *	@param src �R�s�[���\�[�X
	 *	@param	iInflateSize	�]���o�b�t�@�̑����T�C�Y
	 */
	XStrBuff(const XStrBuff& src, size_t iInflateSize = XSTRBUFF_DEF_INFLATE_SIZE);
	/** @brief	�f�X�g���N�^ */
	virtual ~XStrBuff();

	

	/**	@brief	�]���o�b�t�@�̑����T�C�Y��ݒ�
	 *	@param	iInflateSize	�]���o�b�t�@�̑����T�C�Y
	 *�@@exception �C���f�b�N�X�I�[�o�[�̏ꍇ�AXExcept(XExcept_UNKNOWN_PARAM)������
	 */
	bool setInflateSize(size_t nInflateSize);
	/**	@brief	�]���o�b�t�@�̑����T�C�Y���擾
	 *	@return	�]���o�b�t�@�̑����T�C�Y
	 */
	size_t getInflateSize() const{ return m_iInflateSize; }

	/** @copydoc XBuff::Alloc(size_t) */
	virtual void *Alloc(size_t nSize);
	/** @copydoc XBuff::ReAlloc(size_t) */
	virtual void *ReAlloc(size_t nSize);
	/** @copydoc XBuff::Free() */
	virtual void Free();

	/**	@brief	�������Ԃ��܂��B
	 *
	 *	@return	������
	 */
	virtual const TCHAR* c_str() const;

	virtual XStrBuff& set(const char* lpsz, int size = -1);
	virtual XStrBuff& set(const wchar_t* lpsz, int size = -1);

	/**
	 * @brief	�������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XStrBuff& operator=(const char* lpsz);
	/**
	 * @brief	�������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */
	const XStrBuff& operator=(const wchar_t* lpsz);
	/**
	 * @brief	�������}�����܂��B
	 * @param stringSrc �R�s�[���C���X�^���X
	 * @return ���g�̃C���X�^���X
	 */
	const XStrBuff& operator=(const XStrBuff& stringSrc);
	/**
	 * @brief	��������擾���܂��B
	 * @return ������
	 */
	operator const char*();
	/**
	 * @brief	��������擾���܂��B
	 * @return ������
	 */
	operator char*();
	/**
	 * @brief	��������擾���܂��B
	 * @return ������
	 */
	operator const wchar_t*();
	/**
	 * @brief	��������擾���܂��B
	 * @return ������
	 */
	operator wchar_t*();
	/**
	 * @brief	�w�肵��������Ɣ�r���܂��B
	 * @param lpsz ��r���镶����
	 * @retval true ������
	 * @retval false �������Ȃ�
	 */
	const bool operator==(const TCHAR* lpsz) const;

	/**
	 * @brief	�w�肵��������Ɣ�r���܂��B
	 * @param lpsz ��r���镶����
	 * @retval true ������
	 * @retval false �������Ȃ�
	 */
	const bool operator==(const XStrBuff& right) const;

	/**
	 * @brief	�w�肵��������Ɣ�r���܂��B
	 * @param lpsz ��r���镶����
	 * @retval true �������Ȃ�
	 * @retval false ������
	 */
	const bool operator!=(const TCHAR* lpsz) const;

	bool operator < (const XStrBuff& rhight) const;

	/**
	 * @brief	�w�肵���C���f�b�N�X�̃L�����N�^�[���擾���܂��B
	 * @param nIndex �L�����N�^�[���擾����C���f�b�N�X
	 * @return �L�����N�^�[
	 */
	TCHAR& operator[](int nIndex);
	/**
	 * @brief	���݂̃C���X�^���X�̕�����Ɏw�肵���������A�����܂��B
	 * @param lpsz �A�����镶����
	 * @return ���g
	 */
	const XStrBuff& operator+=(const char* lpsz);
	/**
	 * @brief	���݂̃C���X�^���X�̕�����Ɏw�肵���������A�����܂��B
	 * @param lpsz �A�����镶����
	 * @return ���g
	 */
	const XStrBuff& operator+=(const wchar_t* lpsz);
	/**
	 * @brief	���݂̃C���X�^���X�̕�����Ɏw�肵���������A�����܂��B
	 * @param lpsz �A�����镶����
	 * @return ���g
	 */
	const XStrBuff& operator+=(XStrBuff& val);

	XStrBuff& operator<<(short val);
	XStrBuff& operator<<(unsigned short val);
	XStrBuff& operator<<(int val);
	XStrBuff& operator<<(unsigned int val);
	XStrBuff& operator<<(long val);
	XStrBuff& operator<<(unsigned long val);
	XStrBuff& operator<<(XD_INT8 val);
	XStrBuff& operator<<(XD_UINT8 val);
	XStrBuff& operator<<(float val);
	XStrBuff& operator<<(double val);
	XStrBuff& operator<<(const char* val);
	XStrBuff& operator<<(const wchar_t* val);
	XStrBuff& operator<<(XStrBuff& val);

	XStrBuff& appendChar(char c);
	XStrBuff& appendChar(wchar_t c);

	/**
	 * ������̒������擾���܂��B
	 * @return ������̒�
	 */
	size_t GetLength() const;
	/**
	 * ������̒������擾���܂��B
	 * @return ������̒�
	 */
	size_t getLength() const { return this->GetLength(); }
	
	/**
	 * ������̒������̃o�b�t�@�T�C�Y���擾���܂��B
	 * @return ������̒������̃o�b�t�@�T�C�Y
	 */
	size_t getLengthOfByteSize() const { return this->GetLength()*sizeof(TCHAR); }

	
	/**
	 * �o�b�t�@�J�E���g�i�������j���擾
	 * @return �o�b�t�@�J�E���g�i�������j���擾
	 */
	size_t getCountOfBuff()  const{  return this->GetBuffSize() / sizeof(TCHAR); }
	
	/**
	 * �]��o�b�t�@�J�E���g�i�������j�̂��擾
	 * @return �]��o�b�t�@�J�E���g�i�������j���擾
	 */
	size_t getCountOfRestBuff()  const { return this->getCountOfBuff() - this->getLength(); }

	/**
	 * �w�肵���C���f�b�N�X�̃L�����N�^�[���擾���܂��B
	 * @param nIndex �L�����N�^�[���擾����C���f�b�N�X
	 * @return �L�����N�^�[
	 * @exception �C���f�b�N�X�I�[�o�[�̏ꍇ�AXExcept(XExcept_OVER_INDEX)������
	 */
	TCHAR& GetAt(size_t nIndex);

	/**
	 * �w�肵���C���f�b�N�X�̈ʒu�̃L�����N�^�[��ch�ƒu�������܂��B
	 * @param nIndex �u��������C���f�b�N�X
	 * @param ch �u��������L�����N�^�[
	 * @exception �C���f�b�N�X�I�[�o�[�̏ꍇ�AXExcept(XExcept_OVER_INDEX)������
	 */
	void SetAt(size_t nIndex, TCHAR ch);
	/**
	 * �����t�f�[�^�ŁA��������쐬���܂��B
	 * @param lpszFormat �������䕶����
	 * @param argList  �������X�g�ւ̃|�C���^
	 */
	void FormatV(const TCHAR* lpszFormat, va_list argList);
	/**
	 * �����t�f�[�^�ŁA��������쐬���܂��B
	 * @param lpszFormat �������䕶����
	 * @param ...  �I�v�V�����̈���
	 */
	void Format(const TCHAR* lpszFormat, ...);

	/**
	 * �����t�f�[�^�ŁA��������쐬���܂��B
	 * @param lpszFormat �������䕶����
	 * @param argList  �������X�g�ւ̃|�C���^
	 * @return ���������ꂽ�������XStrBuff
	 */
	static XStrBuff FormatS(const TCHAR* lpszFormat, va_list argList);
	/**
	 * �����t�f�[�^�ŁA��������쐬���܂��B
	 * @param lpszFormat �������䕶����
	 * @param ...  �I�v�V�����̈���
	 * @return ���������ꂽ�������XStrBuff
	 */
	static XStrBuff FormatS(const TCHAR* lpszFormat, ...);
	/**
	 * �w�肵��������Ɣ�r���܂��B
	 * @param lpsz ��r���镶����
	 * @retval 0 ����������ł��B
	 * @retval ���̒l lpsz�̕���������
	 * @retval ���̒l lpsz�̕����傫��
	 */
	int Compare(const TCHAR* lpsz) const;
	/**
	 * �w�肵��������𕶎��̑啶��������������������r���܂��B
	 * @param lpsz ��r���镶����
	 * @retval 0 ����������ł��B
	 * @retval ���̒l lpsz�̕���������
	 * @retval ���̒l lpsz�̕����傫��
	 */
	int CompareNoCase(const TCHAR* lpsz) const;
	/** �������啶���ɕϊ����܂��B */
	void MakeUpper();
	/** ��������������ɕϊ����܂��B */
	void MakeLower();
	/**
	 * ������̑O����g���~���O���܂��B
	 * @param mode �g�������郂�[�h
	 * @return �g������̕�����
	 */
	const TCHAR* Trim(TrimMode mode = TRIM_BOTH);
	/**
	 * �w�肳�ꂽ�����𑼂̕����ɒu�������܂��B
	 * @param lpszFind lpszDest�ɒu�������镶���B
	 * @param lpszDest lpszFind�ƒu�������镶���B
	 * @return �u��������̕�����
	 */
	const TCHAR* Replace(const TCHAR* lpszFind, const TCHAR* lpszDest);
	/**
	 * ������̍��擪����AnCount �����𒊏o���A���o��������������̃R�s�[��Ԃ��܂�
	 * @param nCount ���o���镶����
	 * @return �R�s�[�C���X�^���X
	 */
	XStrBuff Left(size_t nCount) const;
	/**
	 * ������̉E�[����AnCount �����𒊏o���A���o��������������̃R�s�[��Ԃ��܂�
	 * @param nCount ���o���镶����
	 * @return �R�s�[�C���X�^���X
	 */
	XStrBuff Right(size_t nCount) const;
	/**
	 * �w�肳�ꂽ�����𕶎��񂩂�폜���܂��B
	 * @param chRemove �����񂩂�폜���镶���B
	 * @return �����񂩂�폜������������Ԃ��܂��B
	 */
	int Remove(TCHAR chRemove);
	/**
	 * �傫�ȕ�������̕����܂��͕�����������������܂��B
	 * @param pstr �������镔����������w�肵�܂��B
	 * @param nStart ��������̌����J�n�����̃C���f�b�N�X
	 * @return �w�肵������������܂��͕����ƈ�v�����ʒu�B
	 */
	int Find( const TCHAR* pstr, int nStart ) const;
	/**
	 * �傫�ȕ�������̕����܂��͕�����������������܂��B
	 * @param lpszSub �������镔����������w�肵�܂��B
	 * @return �w�肵������������܂��͕����ƈ�v�����ʒu�B
	 */
	int Find( const TCHAR* lpszSub ) const;


	/**
	 * ������̒������擾���܂��B
	 * @param lpsz ������
	 * @return ������̒�
	 */
	static size_t StrLen(const TCHAR* lpsz);
	/**
	 * ������̒������̃o�b�t�@�T�C�Y���擾���܂��B
	 * @param lpsz ������
	 * @return ������̒����̃o�b�t�@�T�C�Y
	 */
	static size_t StrLenOfByteSize(const TCHAR* lpsz){ return StrLen(lpsz)*sizeof(TCHAR); }
	/**
	 * ��������R�s�[���܂��B
	 * @param lpszDst �R�s�[��̕�����
	 * @param lpszSrc �R�s�[���� NULL �I�[������
	 * @return �R�s�[��̕������Ԃ��܂��B
	 */
	static TCHAR* StrCpy(TCHAR* lpszDst, size_t buff_count, const TCHAR* lpszSrc);
	/**
	 * �����񂩂�ʂ̕�����ɕ������R�s�[���܂��B
	 * @param lpszDst �R�s�[��̕�����
	 * @param lpszSrc �R�s�[���� NULL �I�[������
	 * @param count �R�s�[���镶����
	 * @return �R�s�[��̕������Ԃ��܂��B
	 */
	static TCHAR* StrNCpy(TCHAR* lpszDst, size_t buff_count, const TCHAR* lpszSrc, size_t count);
	/**
	 * ��������r���܂��B
	 * @param lpsz1 ��r���� NULL �I�[������
	 * @param lpsz2 ��r���� NULL �I�[������
	 * @retval 0 ����������ł��B
	 * @retval ���̒l psz2�̕���������
	 * @retval ���̒l psz1�̕����傫��
	 */
	static int    StrCmp(const TCHAR* lpsz1, const TCHAR* lpsz2);
	/**
	 * �����񒆂̕������������Ƃ��Ĕ�r���܂��B
	 * @param lpsz1 ��r���� NULL �I�[������
	 * @param lpsz2 ��r���� NULL �I�[������
	 * @retval 0 ����������ł��B
	 * @retval ���̒l psz2�̕���������
	 * @retval ���̒l psz1�̕����傫��
	 */
	static int    StrICmp(const TCHAR* lpsz1, const TCHAR* lpsz2);
	/**
	 * 2 �̕�����̕������r���܂��B
	 * @param lpsz1 ��r���� NULL �I�[������
	 * @param lpsz2 ��r���� NULL �I�[������
	 * @param count ��r���镶����
	 * @retval 0 ����������ł��B
	 * @retval ���̒l psz2�̕���������
	 * @retval ���̒l psz1�̕����傫��
	 */
	static int    StrNCmp(const TCHAR* lpsz1, const TCHAR* lpsz2, size_t count);
	/**
	 * �}���`�o�C�g�����̑�1�o�C�g�ւ̃|�C���^��Ԃ��܂��B
	 * @param lpsz ����̃|�C���^
	 * @return lpsz����̃}���`�o�C�g�����̑�1�o�C�g�ւ̃|�C���^
	 */
	static TCHAR* StrInc( const TCHAR *lpsz);
	/**
	 * ������𑖍����A�w�肳�ꂽ�����̍Ō�̏o���ӏ��������܂��B
	 * @param string �����Ώۂ� NULL �I�[������
	 * @param c �����镶��
	 * @return ���� c �� string ���ɍŌ�Ɍ��ꂽ�ʒu�ւ̃|�C���^��Ԃ��܂��B�w�肳�ꂽ������������Ȃ��ꍇ�� NULL �|�C���^��Ԃ��܂��B
	 */	
	static TCHAR* StrRChr( const TCHAR *string, int c);

	/**
	 * ������𐔒l�ɕϊ����܂��B
	 * @param string ������
	 * @return�@�ϊ���̐��l
	 */
	static int    AtoI(const TCHAR* string);

	/**
	 * ������𐔒l�ɕϊ����܂��B
	 * @param string ������
	 * @return�@�ϊ���̐��l
	 */
	static XD_INT8 AtoI64(const TCHAR* string);

	/**	@brief	������𐔒l�ɕϊ�����B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *	@param	base	���l�̊
	 *
	 *	@return	�I�[�o�[�t���[����������� LONG_MAX �܂��� LONG_MIN ��Ԃ��܂��B�ϊ��ł��Ȃ������ꍇ�� 0 ��Ԃ��܂��B
	 */
	static XD_INT8 strtollA(const char *nptr,char **endptr,int base);
	/**	@brief	������𐔒l�ɕϊ�����B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *	@param	base	���l�̊
	 *
	 *	@return	�I�[�o�[�t���[����������� LONG_MAX �܂��� LONG_MIN ��Ԃ��܂��B�ϊ��ł��Ȃ������ꍇ�� 0 ��Ԃ��܂��B
	 */
	static XD_INT8 strtollW(const wchar_t *nptr,wchar_t **endptr,int base);


	/**	@brief	������𐔒l�ɕϊ�����B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *	@param	base	���l�̊
	 *
	 *	@return	�I�[�o�[�t���[����������� LONG_MAX �܂��� LONG_MIN ��Ԃ��܂��B�ϊ��ł��Ȃ������ꍇ�� 0 ��Ԃ��܂��B
	 */
	static XD_UINT8 strtoullA(const char *nptr,char **endptr,int base);
	/**	@brief	������𐔒l�ɕϊ�����B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *	@param	base	���l�̊
	 *
	 *	@return	�I�[�o�[�t���[����������� LONG_MAX �܂��� LONG_MIN ��Ԃ��܂��B�ϊ��ł��Ȃ������ꍇ�� 0 ��Ԃ��܂��B
	 */
	static XD_UINT8 strtoullW(const wchar_t *nptr,wchar_t **endptr,int base);
	/**	@brief	������𐔒l�ɕϊ�����B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *	@param	base	���l�̊�B�|�P���w�肷��ƁA��̎�������BOXFFFF(16�i���w��\)�B�f�t�H���g�͂P�O�i��
	 *
	 *	@return	�I�[�o�[�t���[����������� LONG_MAX �܂��� LONG_MIN ��Ԃ��܂��B�ϊ��ł��Ȃ������ꍇ�� 0 ��Ԃ��܂��B
	 */
	static XD_INT8 strtoll(const TCHAR *nptr,TCHAR **endptr,int base);
	/**	@brief	������𐔒l�ɕϊ�����B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *	@param	base	���l�̊�B�|�P���w�肷��ƁA��̎�������BOXFFFF(16�i���w��\)�B�f�t�H���g�͂P�O�i��	 *
	 *
	 *	@return	�I�[�o�[�t���[����������� LONG_MAX �܂��� LONG_MIN ��Ԃ��܂��B�ϊ��ł��Ȃ������ꍇ�� 0 ��Ԃ��܂��B
	 */
	static XD_INT4 strtol(const TCHAR *nptr,TCHAR **endptr,int base);
	/**	@brief	������𐔒l�ɕϊ�����B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *	@param	base	���l�̊�B�|�P���w�肷��ƁA��̎�������BOXFFFF(16�i���w��\)�B�f�t�H���g�͂P�O�i��	 *
	 *
	 *	@return	�I�[�o�[�t���[����������� LONG_MAX �܂��� LONG_MIN ��Ԃ��܂��B�ϊ��ł��Ȃ������ꍇ�� 0 ��Ԃ��܂��B
	 */
	static XD_UINT8 strtoull(const TCHAR *nptr,TCHAR **endptr,int base);
	/**	@brief	������𐔒l�ɕϊ�����B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *	@param	base	���l�̊�B�|�P���w�肷��ƁA��̎�������BOXFFFF(16�i���w��\)�B�f�t�H���g�͂P�O�i��
	 *
	 *	@return	�I�[�o�[�t���[����������� LONG_MAX �܂��� LONG_MIN ��Ԃ��܂��B�ϊ��ł��Ȃ������ꍇ�� 0 ��Ԃ��܂��B
	 */
	static XD_UINT4 strtoul(const TCHAR *nptr,TCHAR **endptr,int base);


	/**	@brief	������������ɕϊ����܂��B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *
	 *	@return	�ϊ����ꂽ�l
	 */
	static XD_FLOAT8 strtodA( const char *nptr, char **endptr );

	/**	@brief	������������ɕϊ����܂��B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *
	 *	@return	�ϊ����ꂽ�l
	 */
	static XD_FLOAT8 strtodW( const wchar_t *nptr, wchar_t **endptr  );

	/**	@brief	������������ɕϊ����܂��B
	 *
	 *	@param	nptr	�ϊ����� NULL �I�[������
	 *	@param	*endptr	�����̏I���ʒu�����������ւ̃|�C���^
	 *
	 *	@return	�ϊ����ꂽ�l
	 */
	static XD_FLOAT8 strtod( const TCHAR *nptr, TCHAR **endptr  );

	/** 
	 * t�������10�i�����\���ł���ƁA�^��Ԃ��܂��B
	 * @param t �e�X�g���鐮���l
	 * @return t�������10�i�����\���ł���ƁA�^��Ԃ��܂��B
	 */
	static int    isDigit(const TCHAR t);
	/** 
	 * t�������16�i�����\���ł���ƁA�^��Ԃ��܂��B
	 * @param t �e�X�g���鐮���l
	 * @return t�������16�i�����\���ł���ƁA�^��Ԃ��܂��B
	 */
	static int    isXDigit(const TCHAR t);
	/**	@brief	�w�蕶���񂪐��l���\���ł���ƁA�^��Ԃ��܂�.
	 *			16�^���\�L�́A������̐擪�ɁA0x��t�����ĕ�������w�肵�܂��B
	 *	@param	pStr	���肷�镶����
	 *
	 *	@return t�������10�i�����\���ł���ƁA�^��Ԃ��܂��B
	 */
	static int    isDigitsEx(const TCHAR* pStr);
	/**	@brief	�������X�g�ւ̃|�C���^���g���āA�����t���o�͂��������݂܂��B
	 *	@param buffer �o�͐�̋L���̈�
	 *	@param format ��������
	 *	@param argptr �������X�g�ւ̃|�C���^
	 *	@retval	1	���l�ł���
	 *	@retval	0	���l�łȂ�
	 */
	static int    VsPrintf(TCHAR *buffer, size_t buff_count, const TCHAR *format, va_list argptr);
	static int    VsnPrintf(TCHAR *buffer, size_t buff_count, size_t maxlen, const TCHAR *format, va_list argptr);
	static int  printf(TCHAR *buffer, size_t buff_count, const TCHAR *format, ...);
	static int  nprintf(TCHAR *buffer, size_t buff_count, size_t maxlen, const TCHAR *format, ...);
	/**
	 * �����񒆂̕�����啶���ɕϊ����܂��B
	 * @param string ������
	 * @return �ϊ����ꂽ������ւ̃|�C���^
	 */
	static TCHAR* StrUpr(TCHAR *string);
	/**
	 * �����񒆂̕������������ɕϊ����܂��B
	 * @param string ������
	 * @return �ϊ����ꂽ������ւ̃|�C���^
	 */
	static TCHAR* StrLwr(TCHAR *string);
	/**
	 * ����������������܂��B
	 * @param string1 (null)
	 * @param string2 (null)
	 * @return string2 �� string1 ���ɍŏ��Ɍ��ꂽ�ʒu�ւ̃|�C���^�Bstring2 ��string1 ���Ɍ�����Ȃ��ꍇ�� NULL �|�C���^��Ԃ��܂��Bstring2 ������ 0 �̕�������w���Ă���ꍇ�́Astring1 ��Ԃ��܂��B
	 */
	static const TCHAR* StrStr(const TCHAR *string1, const TCHAR *string2);
	/**
	 * ����������������܂��B
	 * @param string1 (null)
	 * @param string2 (null)
	 * @return string2 �� string1 ���ɍŏ��Ɍ��ꂽ�ʒu�ւ̃|�C���^�Bstring2 ��string1 ���Ɍ�����Ȃ��ꍇ�� NULL �|�C���^��Ԃ��܂��Bstring2 ������ 0 �̕�������w���Ă���ꍇ�́Astring1 ��Ԃ��܂��B
	 */
	static TCHAR* StrStr(TCHAR *string1, const TCHAR *string2);
	/**
	 * ��������������ĕ����������܂��B
	 * @param lpsz NULL �I�[������
	 * @param c �������镶��
	 * @return string ���� c ���ŏ��Ɍ��ꂽ�ʒu�ւ̃|�C���^��Ԃ��܂��B������������Ȃ��� NULL ��Ԃ��܂��B
	 */
	static TCHAR* StrChr(const TCHAR *lpsz, TCHAR c);
	/**
	 * �������ǉ����܂��B
	 * @param string1 �ǉ���� NULL �I�[������
	 * @param string2 �ǉ����� NULL �I�[������
	 * @return ����̕����� (string1) ��Ԃ��܂�
	 * @note 2�ڂ̃p�����[�^�[(buff_count)�́A�o�b�t�@�[�̎c��̃T�C�Y�ł͂Ȃ��A���v�T�C�Y�ł��邱�Ƃɒ��ӂ��Ă��������B
	 */
	static TCHAR* StrCat(TCHAR *string1, size_t buff_count, const TCHAR *string2 );
	/**
	 * �w�肵��������̃A�h���X���A������̍�����A�X�y�[�X�܂��́A�^�u�����t����܂Ń|�C���^�[��i�߂܂��B
	 * @param pWork ������̃A�h���X
	 * @retval true ���t������
	 * @retval false ���t����Ȃ�
	 */
	static bool MvPointerSPACE(TCHAR* *pWork);
	/**
	 * �w�肵��������̃A�h���X���A������̍�����A�X�y�[�X�܂��́A�^�u�łȂ����������t����܂ŁA�|�C���^�[��i�߂܂��B
	 * @param pWork ������̃A�h���X
	 * @retval true ���t������
	 * @retval false ���t����Ȃ�
	 */
	static bool MvPointerUnSPACE(TCHAR* *pWork);
	/**
	 * �w�肵��������̃A�h���X���A������̍�����Ac ���������t����܂ŁA�|�C���^�[��i�߂܂��B
	 * @param pWork ������̃A�h���X
	 * @param c �T�[�`���镶��
	 * @retval true ���t������
	 * @retval false ���t����Ȃ�
	 */
	static bool MvPointerCHAR(TCHAR* *pWork, TCHAR c);

	/**	@brief	���C���h�J�[�h�w��\�ȕ������r
	 *
	 *	@param	ptn	�p�^�[������
	 *	@param	str	��r�ΏƂ̕���
	 *
	 *	@retval	�^�F��v
	 *	@retval	�U�F�s��v
	 */
	static int strmatchA(const char *ptn, const char *str);
	/**	@brief	���C���h�J�[�h�w��\�ȕ������r
	 *
	 *	@param	ptn	�p�^�[������
	 *	@param	str	��r�ΏƂ̕���
	 *
	 *	@retval	�^�F��v
	 *	@retval	�U�F�s��v
	 */
	static int strmatchW(const wchar_t *ptn, const wchar_t *str);
	/**	@brief	���C���h�J�[�h�w��\�ȕ������r
	 *
	 *	@param	ptn	�p�^�[������
	 *	@param	str	��r�ΏƂ̕���
	 *
	 *	@retval	�^�F��v
	 *	@retval	�U�F�s��v
	 */
	static int strmatch(const TCHAR *ptn, const TCHAR *str);


	static errno_t __strcpyA(char* p_dst, size_t buff_count, const char* p_src);
	static errno_t __strcpyW(wchar_t* p_dst, size_t buff_count, const wchar_t* p_src);
	static errno_t __strncpyA(char* p_dst, size_t buff_count, const char* p_src, size_t max_count);
	static errno_t __strncpyW(wchar_t* p_dst, size_t buff_count, const wchar_t* p_src, size_t max_count);
	static errno_t __wcstombs(char* p_dst, size_t buff_count, const wchar_t* p_src, size_t max_count);
	static errno_t __mbstowcs(wchar_t* p_dst, size_t buff_count, const char* p_src, size_t src_count);

	static errno_t __sprintf(char* p_dst, size_t buff_count, const char* p_format, ...);
	static errno_t __swprintf(wchar_t* p_dst, size_t buff_count, const wchar_t* p_format, ...);
	static errno_t __vsprintf(char *buffer, size_t buff_count, const char *format, va_list argptr);
	static errno_t __vswprintf(wchar_t *buffer, size_t buff_count, const wchar_t *format, va_list argptr);
	static errno_t __vsnprintf(char *buffer, size_t buff_count, size_t max_count, const char *format, va_list argptr);
	static errno_t __vsnwprintf(wchar_t *buffer, size_t buff_count, size_t max_count, const wchar_t *format, va_list argptr);
	
	/** @copydoc XStrBuff::StrCmp(const TCHAR*, const TCHAR*) */
	static int strcmp(const char *lpsz1, const char *lpsz2);
	/** @copydoc XStrBuff::StrCmp(const TCHAR*, const TCHAR*) */
	static int wcscmp (const wchar_t * lpsz1, const wchar_t * lpsz2);

	/** @copydoc XStrBuff::StrICmp(const TCHAR*, const TCHAR*) */
	static int stricmp(const char *lpsz1, const char *lpsz2);
	/** @copydoc XStrBuff::StrICmp(const TCHAR*, const TCHAR*) */
	static int wcsicmp (const wchar_t * lpsz1, const wchar_t * lpsz2);
	/** @copydoc XStrBuff::StrUpr(TCHAR*) */
	static char* strupr (char * string);
	/** @copydoc XStrBuff::StrUpr(TCHAR*) */
	static wchar_t* wcsupr(wchar_t * string);
	/** @copydoc XStrBuff::StrLwr(TCHAR*) */
	static char* strlwr(char * string);
	/** @copydoc XStrBuff::StrLwr(TCHAR*) */
	static wchar_t* wcslwr (wchar_t * string);
	/** @copydoc XStrBuff::StrInc( const TCHAR*) */
	static char* strinc( const char *lpsz );
	/** @copydoc XStrBuff::StrInc( const TCHAR*) */
	static wchar_t* wcsinc( const wchar_t *lpsz );
	/** @copydoc XStrBuff::AtoI(const TCHAR*) */
	static int atoi(const char *string);
	/** @copydoc XStrBuff::AtoI(const TCHAR*) */
	static int wtoi( const wchar_t *string );
protected:
	/* ������̃o�b�t�@ */
	TCHAR*	m_pStrAddress;
	/* �����R�[�h�ύX�p�o�b�t�@ */
	XBuff	m_ConvertBuff;
	/* �o�b�t�@�̑��T�C�Y */
	size_t	m_iInflateSize;
};


/**
 * ������z��N���X(JAVA��java.lang.StringBuffer�̂悤�ȓ���������)
 */
class XLIB_API XStrBuffList
{
public:
	/** �R���X�g���N�^ */
	XStrBuffList();
	/** �f�X�g���N�^ */
	virtual ~XStrBuffList();

	/** ���X�g����ɂ��܂� */
	void clear();

	/**
	 * ��������o�b�t�@�̍Ō�ɒǉ�����B
	 * @param[in] lpszFormat �ǉ����镶����̏����t�H�[�}�b�g
	 * @param[in] ... �����t�H�[�}�b�g�̃I�v�V����
	 * @return ���g
	 */
	XStrBuffList& append(const char* lpszFormat, ...);
	/** @copydoc XStrBuffList::append(const char* , ...) */
	XStrBuffList& append(const wchar_t* lpszFormat, ...);
	/**
	 * ��������o�b�t�@�̍Ō�ɒǉ�����B
	 * @param[in] str ������
	 * @return ���g
	 */
	XStrBuffList& append(XStrBuff& str);

	/**
	 *  �����𕶎���o�b�t�@�ɑ}������B
	 * @param[in] iPos �}������I�t�Z�b�g
	 * @param[in] lpszFormat �}�����镶����̏����t�H�[�}�b�g
	 * @param[in] ... �����t�H�[�}�b�g�̃I�v�V����
	 * @return ���g
	 */
	XStrBuffList& insert(int iPos, const char* lpszFormat, ...);
	/** @copydoc XStrBuffList::insert(int, const char* , ...) */
	XStrBuffList& insert(int iPos, const wchar_t* lpszFormat, ...);
	/**
	 *  �����𕶎���o�b�t�@�ɑ}������B
	 * @param[in] iPos �}������I�t�Z�b�g
	 * @param[in] str �}�����镶����
	 * @return ���g
	 */
	XStrBuffList& insert(int iPos, XStrBuff& str);
	/**
	 * �z�񐔂�Ԃ��܂��B
	 * @return �z��
	 */ 
	int GetCount() const;
	/**
	 * �w�肵���C���f�b�N�X��XStrBuff�C���X�^���X�̃A�h���X��Ԃ��܂��B
	 * @param[in] iPos 
	 * @return XStrBuff�C���X�^���X�̃A�h���X�B�C���f�b�N�X���s���ȏꍇ�́ANULL�B
	 */ 
	XStrBuff* GetAt(int iPos);

	/**
	 * toString�ŕԂ���镶���������߂܂��B
	 * @param[in] pBefore �z�񂲂Ƃ̑O�ɒǉ�����镶��
	 * @param[in] pAfter �z�񂲂Ƃ̌�ɒǉ�����镶��
	 * @return ������
	 */
	size_t capacityA(const char *pBefore = NULL, const char* pAfter = NULL);
	/** XStrBuffList::capacityA(const char *pBefore , const char*)  */
	size_t capacityW(const wchar_t *pBefore = NULL, const wchar_t* pAfter = NULL);
	/** XStrBuffList::capacityA(const char *pBefore , const char*)  */
	size_t capacity(const TCHAR *pBefore = NULL, const TCHAR* pAfter = NULL);

	/**
	 * ��������쐬���܂��B
	 * @param[in] pBefore �z�񂲂Ƃ̑O�ɒǉ�����镶��
	 * @param[in] pAfter �z�񂲂Ƃ̌�ɒǉ�����镶��
	 * @return ����
	 */
	XStrBuff toStringA(const char *pBefore = NULL, const char* pAfter = NULL);
	/** XStrBuffList::toStringA(const char *pBefore , const char*)  */
	XStrBuff toStringW(const wchar_t *pBefore = NULL, const wchar_t* pAfter = NULL);
	/** XStrBuffList::toStringA(const char *pBefore , const char*)  */
	XStrBuff toString(const TCHAR *pBefore = NULL, const TCHAR* pAfter = NULL);
protected:
	/** ���X�g */
	XList		m_list;
};


/**
 * ��������AExcel�œǂݍ��߂�CSV�`���ɐ��`���܂�
 */
class XStrBuffCSV : public XStrBuff
{
public:
// Construction
	/**
	 * �R���X�g���N�^
	 * @param cDelimit �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffCSV(TCHAR cDelimit = ',', TCHAR cIndeChar = '\"');
	/**
	 * �R���X�g���N�^
	 * @param p ������镶����
	 * @param cDelimit �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffCSV(const char* p, TCHAR cDelimit = ',', TCHAR cIndeChar = '\"');
	/**
	 * �R���X�g���N�^
	 * @param p ������镶����
	 * @param cDelimit �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffCSV(const wchar_t* p, TCHAR cDelimit = ',', TCHAR cIndeChar = '\"');
	/**
	 * �������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XStrBuffCSV& operator=(const char* lpsz);
	/**
	 * �������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XStrBuffCSV& operator=(const wchar_t* lpsz);
protected:
	/**
	 * �o�b�t�@���̕������Excel�œǂݍ��߂�CSV�`���ɐ��`���܂��B
	 * @return �ϊ���̕�����
	 */
	const TCHAR* Conv();
	/** �f���~�^ */
	TCHAR m_cDelimit;
	/** �C���f���g */
	TCHAR m_cIndeChar;
	/** �ϊ��ς݃t���O */
	bool m_bConv;
};

/**
 * ��������ASQL�����p�ɁA' ������ƁA�����ЂƂ�' �̂��������ɐ��`���܂�
 */
class XStrBuffSQL : public XStrBuff
{
public:
	/**
	 * �R���X�g���N�^
	 * @param cDelimit �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffSQL(TCHAR cDelimit = '\'', TCHAR cIndeChar = '\'');
	/**
	 * �R���X�g���N�^
	 * @param p ������镶����
	 * @param cDelimit �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffSQL(const char* p, TCHAR cDelimit = '\'', TCHAR cIndeChar = '\'');
	/**
	 * �R���X�g���N�^
	 * @param p ������镶����
	 * @param cDelimit �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffSQL(const wchar_t* p, TCHAR cDelimit = '\'', TCHAR cIndeChar = '\'');
	/**
	 * �������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XStrBuffSQL& operator=(const char* lpsz);
	/**
	 * �������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XStrBuffSQL& operator=(const wchar_t* lpsz);
protected:
	/**
	 * ��������ASQL�����p�ɁA' ������ƁA�����ЂƂ�' �̂��������ɐ��`���܂�
	 * @return �ϊ���̕�����
	 */
	const TCHAR* Conv();
	/** �f���~�^ */
	TCHAR m_cDelimit;
	/** �C���f���g */
	TCHAR m_cIndeChar;
	/** �ϊ��ς݃t���O */
	bool m_bConv;
};

/** 
 * ��������X�v���b�g����N���X
 */
class XStrBuffSplit :  public XStrBuff
{
public:
	/**
	 * �R���X�g���N�^
	 * @param cDelimiter �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffSplit(TCHAR cDelimiter = ',', TCHAR cIndeChar = '\"');
	/**
	 * �R���X�g���N�^
	 * @param p ������镶����
	 * @param cDelimiter �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffSplit(const char* p, TCHAR cDelimiter = ',', TCHAR cIndeChar = '\"');
	/**
	 * �R���X�g���N�^
	 * @param p ������镶����
	 * @param cDelimiter �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	XStrBuffSplit(const wchar_t* p, TCHAR cDelimiter = ',', TCHAR cIndeChar = '\"');
	/** �f�X�g���N�^ */
	virtual ~XStrBuffSplit();

	/**
	 * �������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XStrBuffSplit& operator=(const wchar_t* lpsz);
	/**
	 * �������}�����܂��B
	 * @param lpsz ������
	 * @return ���g�̃C���X�^���X
	 */	
	const XStrBuffSplit& operator=(const char* lpsz);



	/**
	 * �X�v���b�g���ꂽ�����z��̐����擾���܂��B
	 * @return �X�v���b�g���ꂽ�����z��̐�
	 */
	int GetArgCouunt() const;
	/**
	 * �X�v���b�g���ꂽ�����z����擾���܂��B
	 * @return �X�v���b�g���ꂽ�����z��
	 */
	const TCHAR** GetArgv();
	/**
	 * �X�v���b�g���ꂽ�����z��̎w�肵���C���f�b�N�X�̒l���擾���܂��B
	 * @param nIndex �C���f�b�N�X
	 * @return ������
	 */
	const TCHAR* GetDivid(int nIndex);
	/**
	 * �C���f���g�������Z�b�g���܂��B
	 * @param cIndeChar �C���f���g����
	 */
	void SetIndentChar(TCHAR cIndeChar);
	/**
	 * �f���~�^�������Z�b�g���܂��B
	 * @param cDelimite �C���f���g����
	 */
	void SetDelimite(TCHAR cDelimite);
	/**
	 * Split�������ʁANULL�����̃f�[�^������ꍇ�z��ɉ����邩���f����t���O���X�V���܂�
	 * @param bIsNullCount true NULL�����̃f�[�^��z��ɉ�����B<br>false �����Ȃ�
	 */
	void SetNullCount(bool bIsNullCount);
	/**
	 * Split�������ʁANULL�����̃f�[�^������ꍇ�z��ɉ����邩���f����t���O���擾���܂�
	 * @retval true NULL�����̃f�[�^��z��ɉ�����B
	 * @retval false NULL�����̃f�[�^��z��ɉ����Ȃ��B
	 */
	bool GetNullCount();
protected:
	/**
	 * �C���X�^���X�̏�����
	 * @param cDelimiter �f���~�^
	 * @param cIndeChar �C���f���g
	 */
	void CommounConstruct(TCHAR cDelimiter, TCHAR cIndeChar);

	/**
	 * lpszBuffOrg�̏I�[��NULL��2�Z�b�g���ꂢ��f�[�^�̃������m��
	 * @param lpszBuffOrg �I�[��NULL��2�Z�b�g���ꂢ��f�[�^
	 * @return �m�ۂ����������T�C�Y
	 */
	unsigned long AllocNullNullTerminateA(const char*  lpszBuffOrg);
	/**
	 * lpszBuffOrg�̏I�[��NULL��2�Z�b�g���ꂢ��f�[�^�̃������m��
	 * @param lpszBuffOrg �I�[��NULL��2�Z�b�g���ꂢ��f�[�^
	 * @return �m�ۂ����������T�C�Y
	 */
	unsigned long AllocNullNullTerminateW(const wchar_t* lpszBuffOrg);
	/**
	 * �����f�[�^���ANULL��؂�A�f�[�^�̏I�[��NULL2�̃f�[�^�̃X�v���b�g�������s���܂��B
	 * @return �X�v���b�g���ꂽ�����z��̐�
	 */
	int SpritNullNullTerminate();
	/**
	 * ������������X�v���b�g���܂��B
	 * @return �X�v���b�g���ꂽ�����z��̐�
	 */
	int Sprit();
	/** �f���~�^ */
	TCHAR m_cDelimiter;
	/** �C���f���g */
	TCHAR m_cIndeChar;
	/** �z��p�̃o�b�t�@ */
	XBuff m_ArgBuff;
	/** �z�� */
	int   m_ArgCount;
	/** Split�������ʁANULL�����̃f�[�^������ꍇ�z��ɉ����邩���f����t���O */
	bool  m_bIsNullCount;

};


/**	@brief	�����������g���o�b�t�@�X�g���[��	*/
class XLIB_API XInflateStrstream :public 
#ifdef _UNICODE
	std::wiostream 
#else
	std::iostream 
#endif
{
public:
	/**	@brief	�R���X�g���N�^	*/
	XInflateStrstream();
	/**	@brief	�R���X�g���N�^
	 *	@param	nInitSize	�����m�ۂ���o�b�t�@�T�C�Y
	 *	@param	nIncrementSize	�o�b�t�@������Ȃ��Ȃ�����A��������T�C�Y
	 */
    XInflateStrstream(unsigned long nInitSize, unsigned long nIncrementSize = 1024);
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XInflateStrstream();
	/**	@brief	�X�g���[���o�b�t�@���擾
	 *	@return	�X�g���[���o�b�t�@
	 */
#ifdef _UNICODE
    XInflateStreamBuffW *rdbuf() const;
#else
    XInflateStreamBuff *rdbuf() const;
#endif
	/**	@brief	�o�b�t�@�ɏ������܂ꂽ�������ǂݍ���
	 *	@return	�o�b�t�@�ɏ������܂ꂽ������
	 */
	TCHAR *str();
	/**	@brief	�������񂾃T�C�Y���擾
	 *	@return	�������񂾃T�C�Y
	 */
	std::streamsize pcount();
	/**	@brief	���̓|�C���^��擪�ɖ߂��܂��B	*/
	void resetp();
protected:
#ifdef _UNICODE
	XInflateStreamBuffW	m_buff;
#else
	XInflateStreamBuff	m_buff;
#endif
};

#include "xlib/xlib_tail.h"
#endif //__X_STRBUFF_HEASER_FILE__
