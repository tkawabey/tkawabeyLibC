/**
 * @file xbuff.h
 * @brief �o�b�t�@�ǃN���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_BUFF_HEASER_FILE__
#define __X_BUFF_HEASER_FILE__
#include "xlib/xlib_head.h"
#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif
#endif //
#include <time.h>
#include <stdlib.h>
#ifdef __linux__
	#include <streambuf.h>
#else
	#include <streambuf>
#endif
#include <iostream>
#include <ostream>


typedef char			XBYTE1;
typedef unsigned char	XUBYTE1;
typedef unsigned char	XBIN;
typedef short			XBYTE2;
typedef unsigned short	XUBYTE2;
typedef int				XBYTE4;
typedef unsigned int	XUBYTE4;



#ifdef WIN32
typedef _int64  XD_INT8;
typedef unsigned _int64  XD_UINT8;
#else
typedef long long  XD_INT8;
typedef unsigned long long  XD_UINT8;
#endif

typedef char				XD_INT1;
typedef unsigned char		XD_UINT1;
typedef short				XD_INT2;
typedef unsigned short		XD_UINT2;
typedef long				XD_INT4;
typedef unsigned long		XD_UINT4;
typedef float				XD_FLOAT4;
typedef double				XD_FLOAT8;




#ifndef __max
#define __max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef __min
#define __min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


#ifndef MAKEWORD
  #define MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(a)) | ((unsigned short)((unsigned char)(b))) << 8))
#endif
#ifndef MAKELONG
  #define MAKELONG(a, b)      ((long)(((unsigned short)(a)) | ((unsigned long)((unsigned short)(b))) << 16))
#endif 
#ifndef LOWORD
  #define LOWORD(l)           ((unsigned short)(l))
#endif 
#ifndef HIWORD
  #define HIWORD(l)           ((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#endif 
#ifndef LOBYTE
  #define LOBYTE(w)           ((unsigned char)(w))
#endif 
#ifndef HIBYTE
  #define HIBYTE(w)           ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))
#endif 

#if defined(__CYGWIN__) || defined(__MINGW32__) || defined(__SC__) || !defined(WIN32)
#ifndef XD_ULLONG_MAX
#define XD_ULLONG_MAX 18446744073709551615ULL
#endif
#ifndef XD_LLONG_MAX
#define XD_LLONG_MAX  9223372036854775807LL
#endif
#ifndef XD_LLONG_MIN
#define XD_LLONG_MIN  (-XD_LLONG_MAX-1LL)
#endif

#else
#ifndef XD_ULLONG_MAX
#define XD_ULLONG_MAX 18446744073709551615UL
#endif
#ifndef XD_LLONG_MAX
#define XD_LLONG_MAX  9223372036854775807L
#endif
#ifndef XD_LLONG_MIN
#define XD_LLONG_MIN  (-XD_LLONG_MAX-1L)
#endif
#endif



#ifdef _UNICODE
#define XLIB_STD_OUT_STR_STREAM		std::wostream
#define XLIB_STD_STREAM_BUFFER		XInflateStreamBuffW
#else
#define XLIB_STD_OUT_STR_STREAM		std::ostream
#define XLIB_STD_STREAM_BUFFER		XInflateStreamBuff
#endif



/**
 * �o�b�t�@�ǃN���X
 */
class XLIB_API XBuff
{
public:
	/** �R���X�g���N�^ */
	XBuff();
	/** �R���X�g���N�^ */
	XBuff(const XBuff& dst, int dmmy = 0);
	/** �f�X�g���N�^ */
	virtual ~XBuff();


	/**
	 * �w�肵���T�C�Y�̃��������m�ۂ��܂��B
	 * @note �������[�m�ۂɎ��s�����ꍇ�AWIN32 �́AGetLastError �ŁAERROR_NOT_ENOUGH_MEMORY���擾�ł��܂��B
	 * ���̑�OS�̏ꍇ�́Aerrno �ɁAENOMEM�@���Z�b�g���܂��B
	 * @param nSize �m�ۂ��郁�����T�C�Y
	 * @return �m�ۂ����������̃A�h���X�B�������[�m�ۂɎ��s�����ꍇ��NULL�B
	 */
	virtual void *Alloc(size_t nSize);
	/**
	 * �w�肵���T�C�Y�̃��������Ċm�ۂ��܂��B
	 * @note �������[�m�ۂɎ��s�����ꍇ�AWIN32 �́AGetLastError �ŁAERROR_NOT_ENOUGH_MEMORY���擾�ł��܂��B
	 * ���̑�OS�̏ꍇ�́Aerrno �ɁAENOMEM�@���Z�b�g���܂��B
	 * @param nSize �m�ۂ��郁�����T�C�Y
	 * @return �m�ۂ����������̃A�h���X
	 */
	virtual void *ReAlloc(size_t nSize);
	/**
	 * �������[��������܂��B
	 */
	virtual void Free();
	/**
	 * �w�肵���l�ŁA�����������������܂�
	 * @param c ����������l
	 */
	virtual void MemSet(unsigned char c = 0);

	virtual void memsetA(unsigned char c = 0);
	virtual void memsetW(wchar_t c = 0);
	virtual void memsetT(TCHAR c = 0);


	/**	@brief	�o�b�t�@�ԂŃf�[�^���R�s�[���܂��B
	 *
	 *	@param	src	�R�s�[����f�[�^
	 *	@param	count	�f�[�^�̃T�C�Y
	 *	@param	bReUse	���蓖�čς݂̃������[���w��͈͂Ȃ��Ȃ�A���̃o�b�t�@���g���񂷁B�ꍇ�́Atrue�B��xfree���čĊ��蓖�Ă��s���ꍇ��false
	 *
	 *	@retval	value	DESC
	 *	@retval	value	DESC
	 *	@verbatim
	 *	@endverbatim
	 *	
	 */
	virtual void* memcpy(const void *src, size_t count, bool bReUse = true);
// �l�̎擾
	/**
	 * �o�b�t�@�̐擪�A�h���X���擾���܂��B
	 * @return �o�b�t�@�̐擪�A�h���X
	 */
	operator void*();
	/**
	 * �o�b�t�@�̐擪�A�h���X���擾���܂��B
	 * @return �o�b�t�@�̐擪�A�h���X
	 */
	operator char*();
	/**
	 * �o�b�t�@�̐擪�A�h���X���擾���܂��B
	 * @return �o�b�t�@�̐擪�A�h���X
	 */
	operator unsigned char*();
	/**
	 * �o�b�t�@�̐擪�A�h���X���擾���܂��B
	 * @return �o�b�t�@�̐擪�A�h���X
	 */
	operator unsigned short*();
	/**
	 * �o�b�t�@�̐擪�A�h���X���擾���܂��B
	 * @return �o�b�t�@�̐擪�A�h���X
	 */
	operator unsigned long*();
#ifdef _NATIVE_WCHAR_T_DEFINED
	/**
	 * �o�b�t�@�̐擪�A�h���X���擾���܂��B
	 * @return �o�b�t�@�̐擪�A�h���X
	 * @note Zc:wchar_t ���w�肷��ƁA_WCHAR_T_DEFINED �V���{���� _NATIVE_WCHAR_T_DEFINED �V���{������`����܂�
	 */
	operator wchar_t*();
#endif

	/**
	 * �m�ۂ����o�b�t�@�̃T�C�Y���擾���܂��B
	 * @return �m�ۂ����o�b�t�@�̃T�C�Y
	 */
	virtual size_t GetBuffSize() const;
	/**
	 * �o�b�t�@�̍Ō�̃A�h���X���擾���܂��B
	 * @return �o�b�t�@�̍Ō�̃A�h���X
	 */
	void *GetMaxBuffAddress() const;
	/**
	 * �w�肵���o�b�t�@�C���X�^���X�Ɠ����o�b�t�@���m�ۂ��܂��B
	 * @param pSrc �R�s�[���o�b�t�@�C���X�^���X
	 * @return �m�ۂ����o�b�t�@�̐擪�A�h���X
	 */
	virtual void *DoCopy(XBuff *pSrc);


// Operator
	/**
	 * @brief	�w�肵���o�b�t�@�Ɣ�r���܂��B
	 * @param lpsz ��r����o�b�t�@
	 * @retval true ������
	 * @retval false �������Ȃ�
	 */
	bool operator==(const XBuff& dst) const;
	/**
	 * @brief	�w�肵���o�b�t�@�Ɣ�r���܂��B
	 * @param lpsz ��r����o�b�t�@
	 * @retval true �������Ȃ�
	 * @retval false ������
	 */
	bool operator!=(const XBuff& dst) const;
	/**
	 * @brief	�w�肵���o�b�t�@�Ƒ�����܂��B
	 * @param lpsz �������o�b�t�@
	 * @return	���g
	 */
	XBuff& operator=(const XBuff& dst);
protected:
	/** �o�b�t�@�̃A�h���X */
	void*			m_pBuff;
	/** �o�b�t�@�T�C�Y */
	size_t			m_nBuffSize;
};



/**
 * STL�X�g���[���p�o�b�t�@�����g���N���X

@verbatim
	// �����O�o�b�t�@�̃T���v��
	XLIB_STD_STREAM_BUFFER buff(
			10,		//	�����o�b�t�@�T�C�Y
			10		//  �o�b�t�@������Ȃ��Ȃ�����A���₷�T�C�Y
	  );
	// �X�g���[���ɃZ�b�g����
	XLIB_STD_OUT_STR_STREAM strm(&buff);

	// �o�b�t�@�ɏ�������
	strm << "aaa" << "bbb" << (int)100;
	strm.write("0123456789", 10);

	// �������܂ꂽ�o�b�t�@�̃T�C�Y
	printf("size:%d", buff.size());
	
	// �o�b�t�@�̐擪�A�h���X���擾
	char *p = buff.getBuff();

	// �o�b�t�@�̏������݈ʒu��擪�ɖ߂��B
	buff,resetp();

	// �������܂ꂽ�o�b�t�@�̃T�C�Y�͂O�ɖ߂�
	printf("size:%d", buff.size());

@endverbatim

 */
class XLIB_API XInflateStreamBuff :  public std::streambuf
{
public:

	/**	@brief	�R���X�g���N�^
		@param	nInitSize	�����m�ۂ���o�b�t�@�T�C�Y
		@param	nIncrementSize	�o�b�t�@������Ȃ��Ȃ�����A��������T�C�Y
	*/
	explicit XInflateStreamBuff(unsigned long nInitSize = 2048, unsigned long nIncrementSize = 1024);
	/**	@brief	�f�X�g���N�^
	*/
	virtual ~XInflateStreamBuff();

	/**	@brief	�o�b�t�@�ɏ������܂ꂽ�������ǂݍ���
		@return	�o�b�t�@�ɏ������܂ꂽ������
	*/
	const char *toString() {
		*pptr() = 0;
		return m_pBuffer;
	}
	/**	@brief	���݂Ɉʒu�̃A�h���X��Ԃ��܂��B
	 *	@return	���݂Ɉʒu�̃A�h���X
	 */
	const char *getCurrentAddress() {
		return pptr();
	}
	/**	@brief	�o�b�t�@���擾
		@return	�o�b�t�@
	*/
	unsigned char *getBuff() {
		return (unsigned char*)m_pBuffer;
	}
	/**	@brief	�o�b�t�@�̃T�C�Y���擾
		@return	�o�b�t�@�̃T�C�Y
	*/
	unsigned long getBuffSize() {
		return m_nBufferSize;
	}

	/**	@brief	�������񂾃T�C�Y���擾
		@return	�������񂾃T�C�Y
	*/
	unsigned long size() {
		unsigned long nSize = (unsigned long)(pptr() - m_pBuffer);
		return nSize;
	}

	/**	@brief	���̓|�C���^��擪�ɖ߂��܂��B	*/
	void resetp() {
		setp( m_pBuffer, m_pBuffer + (m_nBufferSize-1) );
		setg( m_pBuffer, m_pBuffer, m_pBuffer + (m_nBufferSize-1) );
	}



protected:

	/*	@brief	�w�肵���I�t�Z�b�g�ʒu�Ɉړ����܂��B
	 *	@param	off	�V�����I�t�Z�b�g�l�Bstreamoff �́Atypedef ���ɂ�� long �Ɠ��`�ł�
	 *	@param	dir	�ړ������B���� seek_dir �񋓎q�̂ǂꂩ���w�肵�܂��B
	 *	@param	nMode ios::in �� ios::out �񋓎q���r�b�g���Ƃ� OR (|) �őg�ݍ��킹�������l
	 *	@return	�ړ�������̈ʒu��Ԃ��܂��B
	 */
	virtual std::streampos seekoff( std::streamoff off, std::ios::seekdir dir, std::ios::openmode nMode  = std::ios::in | std::ios::out );
	virtual std::streampos seekpos( std::streampos, std::ios::openmode = std::ios::in | std::ios::out);
	/*	@brief �o�b�t�@������Ȃ��Ȃ�Ƃ��[�邳���B
		@param	nCh	EOF �܂��́A�o�͂��镶��
		@return	�G���[�������������́AEOF ��Ԃ��܂��B
	*/
	virtual int overflow( int nCh = EOF );
	/* �o�b�t�@�̃A�h���X */
	char*         m_pBuffer;
	char*		  _Seekhigh;
	/* �o�b�t�@�̃T�C�Y */
	unsigned long m_nBufferSize;
	/* �o�b�t�@�𑝉�����T�C�Y */
	unsigned long m_nIncrementSize;
};


class XLIB_API XInflateStreamBuffW :  public std::wstreambuf
{
public:

	/**	@brief	�R���X�g���N�^
		@param	nInitSize	�����m�ۂ���o�b�t�@�T�C�Y
		@param	nIncrementSize	�o�b�t�@������Ȃ��Ȃ�����A��������T�C�Y
	*/
	explicit XInflateStreamBuffW(unsigned long nInitSize = 2048, unsigned long nIncrementSize = 1024);
	/**	@brief	�f�X�g���N�^
	*/
	virtual ~XInflateStreamBuffW();

	/**	@brief	�o�b�t�@�ɏ������܂ꂽ�������ǂݍ���
		@return	�o�b�t�@�ɏ������܂ꂽ������
	*/
	const wchar_t *toString() {
		*pptr() = 0;
		return m_pBuffer;
	}
	/**	@brief	���݂Ɉʒu�̃A�h���X��Ԃ��܂��B
	 *	@return	���݂Ɉʒu�̃A�h���X
	 */
	const wchar_t *getCurrentAddress() {
		return pptr();
	}
	/**	@brief	�o�b�t�@���擾
		@return	�o�b�t�@
	*/
	wchar_t *getBuff() {
		return (wchar_t*)m_pBuffer;
	}
	/**	@brief	�o�b�t�@�̃T�C�Y���擾
		@return	�o�b�t�@�̃T�C�Y
	*/
	unsigned long getBuffSize() {
		return m_nBufferSize;
	}

	/**	@brief	�������񂾃T�C�Y���擾
		@return	�������񂾃T�C�Y
	*/
	unsigned long size() {
		unsigned long nSize = (unsigned long)(pptr() - m_pBuffer);
		return nSize;
	}

	/**	@brief	���̓|�C���^��擪�ɖ߂��܂��B	*/
	void resetp() {
		setp( m_pBuffer, m_pBuffer + ((m_nBufferSize/sizeof(wchar_t))-1) );
		setg( m_pBuffer, m_pBuffer, m_pBuffer + ((m_nBufferSize/sizeof(wchar_t))-1) );
	}



protected:

	/*	@brief	�w�肵���I�t�Z�b�g�ʒu�Ɉړ����܂��B
	 *	@param	off	�V�����I�t�Z�b�g�l�Bstreamoff �́Atypedef ���ɂ�� long �Ɠ��`�ł�
	 *	@param	dir	�ړ������B���� seek_dir �񋓎q�̂ǂꂩ���w�肵�܂��B
	 *	@param	nMode ios::in �� ios::out �񋓎q���r�b�g���Ƃ� OR (|) �őg�ݍ��킹�������l
	 *	@return	�ړ�������̈ʒu��Ԃ��܂��B
	 */
	virtual std::streampos seekoff( std::streamoff off, std::ios::seekdir dir, std::ios::openmode nMode  = std::ios::in | std::ios::out );
	virtual std::streampos seekpos( std::streampos, std::ios::openmode = std::ios::in | std::ios::out);
	/*	@brief �o�b�t�@������Ȃ��Ȃ�Ƃ��[�邳���B
		@param	nCh	EOF �܂��́A�o�͂��镶��
		@return	�G���[�������������́AEOF ��Ԃ��܂��B
	*/
	virtual wint_t overflow( wint_t nCh = EOF );
	/* �o�b�t�@�̃A�h���X */
	wchar_t*         m_pBuffer;
	wchar_t*		  _Seekhigh;
	/* �o�b�t�@�̃T�C�Y */
	unsigned long m_nBufferSize;
	/* �o�b�t�@�𑝉�����T�C�Y */
	unsigned long m_nIncrementSize;
};




/**	@brief	�����������g���o�C�i���[�o�b�t�@�X�g���[��	*/
class XLIB_API XInflateBinStrstream : public std::iostream 

{
public:
	/**	@brief	�R���X�g���N�^	*/
	XInflateBinStrstream();
	/**	@brief	�R���X�g���N�^
	 *	@param	nInitSize	�����m�ۂ���o�b�t�@�T�C�Y
	 *	@param	nIncrementSize	�o�b�t�@������Ȃ��Ȃ�����A��������T�C�Y
	 */
    XInflateBinStrstream(unsigned long nInitSize, unsigned long nIncrementSize = 1024);
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XInflateBinStrstream();
	/**	@brief	�X�g���[���o�b�t�@���擾
	 *	@return	�X�g���[���o�b�t�@
	 */
    XInflateStreamBuff *rdbuf() const;
	/**	@brief	�o�b�t�@�ɏ������܂ꂽ�������ǂݍ���
	 *	@return	�o�b�t�@�ɏ������܂ꂽ������
	 */
	XBIN *buffer();
	/**	@brief	�������񂾃T�C�Y���擾
	 *	@return	�������񂾃T�C�Y
	 */
	std::streamsize pcount();
	/**	@brief	���̓|�C���^��擪�ɖ߂��܂��B	*/
	void resetp();
protected:
	XInflateStreamBuff	m_buff;
};





/**	@brief	�����O�o�b�t�@	
@verbatim
	// �����O�o�b�t�@�̃T���v��
	XRingBufferBL buff();
	// �o�b�t�@���m��
	bull.alloc(500);

	// Data1 �u���b�N��������
	bull.put(pData1, 200);
	
	// Data2 �u���b�N��������
	bull.put(pData2, 250);


	// Data3 �u���b�N��������
	// �o�b�t�@Full�ɂȂ��Ă���̂ŁA����ȏ㏑�����߂Ȃ���
	bull.put(pData3, 200);

	// �����O�o�b�t�@����A100�o�C�g�ǂݍ���
	len = buff.copy(rcvbuff, 100);
@endverbatim

*/
class XLIB_API XRingBuffer {
public:
	/**	@brief	�R���X�g���N�^	*/
	XRingBuffer();
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XRingBuffer(void);

	/**	@brief	�o�b�t�@�̃��������m�ۂ��܂��B
	 *
	 *	@param	size	�m�ۂ��郁�����̃T�C�Y
	 *
	 *	@retval	true	succeeded
	 *	@retval	false	failure
	 */
	virtual bool alloc(unsigned long size);
	/**	@brief	�o�b�t�@���A�^�b�`���܂��B
	 *
	 *	@param	p_buff	�A�^�b�`����o�b�t�@�̃A�h���X
	 *	@param	size	�o�b�t�@�̃T�C�Y
	 *
 	 *	@retval	true	succeeded
	 *	@retval	false	failure	
	 */
	virtual bool attach(unsigned char* p_buff, unsigned long size);

	/**	@brief	�������܂�Ă���o�b�t�@�̃T�C�Y���擾���܂��B
	 *
 	 *	@return �������܂�Ă���o�b�t�@�̃T�C�Y
	 */
	virtual unsigned long size(void) const;
	/**	@brief	��o�b�t�@�̃T�C�Y���擾���܂��B
	 *
 	 *	@return ��o�b�t�@�̃T�C�Y
	 */
	virtual unsigned long free_size(void) const;
	/**	@brief	�f�[�^���������݂܂��B
	 *
	 *	@param	data	�f�[�^
	 *	@param	size	�f�[�^�̃T�C�Y
	 *
 	 *	@retval	true	succeeded
	 *	@retval	false	��̈悪����Ȃ�	
	 */
	virtual bool put(const unsigned char* data, unsigned long size);
	/**	@brief	�f�[�^���R�s�[���܂��B�ǂݎ��ʒu�̕ω��͂���܂���B
	 *
	 *	@param	data	�ǂݎ��o�b�t�@
	 *	@param	size	�o�b�t�@�̃T�C�Y
	 *
	 *	@return	���ۂɓǂݎ�����T�C�Y
	 */
	virtual int copy(unsigned char* data, unsigned long size) const;
	/**	@brief	�f�[�^���擾���܂��B�ǂݎ��ʒu��i�߂܂��B
	 *
	 *	@param	data	�ǂݎ��o�b�t�@
	 *	@param	size	�o�b�t�@�̃T�C�Y
	 *
	 *	@return	���ۂɓǂݎ�����T�C�Y
	 */
	virtual int get(unsigned char* data, unsigned long size);
	/**	@brief	�N���A���܂��B */
	virtual void clear(void);
private:
	unsigned long	m_ulRealSize;
	unsigned long	m_ulFirst;
	unsigned long	m_ulLast;
	unsigned char	*m_pBuffer;
	bool			m_bAlloc;
};

/**	@brief	�u���b�N�P�ʂ̃����O�o�b�t�@
@verbatim
	// ���[�v�����O�o�b�t�@�̃T���v��
	XRingBufferBL buff(XRingBufferBL::LOOP);
	// �o�b�t�@���m��
	bull.alloc(500);

	// Data1 �u���b�N��������
	bull.put(pData1, 200);
	
	// Data2 �u���b�N��������
	bull.put(pData2, 250);

	// �������܂ꂽ�u���b�N�̃J�E���g���ƁApop�������ɓǂݍ��݂ɕK�v�ȃo�b�t�@�̃T�C�Y
	printf("block count:%d block size:%d", 
		bull.blk_count(), 
		bull.cur_blk_size());

	// Data3 �u���b�N��������
	// �o�b�t�@Full�ɂȂ��Ă���̂ŁApData1�͔j������Ă���
	bull.put(pData3, 200);

	// pop ���܂��B���[�h�ʒu�͕ς��܂���B(pData2���擾�ł��܂�)
	//  len �͏������񂾃u���b�N�T�C�Y�B�܂�250.
	len = buff.copy(rcvbuff, sizeof(rcvbuff));

	// pop ���܂��B(pData3���擾�ł��܂�)
	//  len �͏������񂾃u���b�N�T�C�Y�B�܂�200.
	len = buff.get(rcvbuff, sizeof(rcvbuff));
@endverbatim

@verbatim
	// ���C�������O�o�b�t�@�̃T���v��
	XRingBufferBL buff(XRingBufferBL::LINE);
	// �o�b�t�@���m��
	bull.alloc(500);

	// Data1 �u���b�N��������
	bull.put(pData1, 200);
	
	// Data2 �u���b�N��������
	bull.put(pData2, 250);

	// �������܂ꂽ�u���b�N�̃J�E���g���ƁApop�������ɓǂݍ��݂ɕK�v�ȃo�b�t�@�̃T�C�Y
	printf("block count:%d block size:%d", 
		bull.blk_count(), 
		bull.cur_blk_size());

	// Data3 �u���b�N��������
	// �o�b�t�@Full�ɂȂ��Ă���̂ŁA�������߂Ȃ�
	if( bull.put(pData3, 200) == false ) {
		fprintf(stderr, "buff full!!.");
	}

	// pop ���܂��B���[�h�ʒu�͕ς��܂���B(pData2���擾�ł��܂�)
	//  len �͏������񂾃u���b�N�T�C�Y�B�܂�250.
	len = buff.copy(rcvbuff, sizeof(rcvbuff));

	// pop ���܂��B(pData3���擾�ł��܂�)
	//  len �͏������񂾃u���b�N�T�C�Y�B�܂�200.
	len = buff.get(rcvbuff, sizeof(rcvbuff));
@endverbatim


*/
class XLIB_API XRingBufferBL {
public:

	/**	@brief	�o�b�t�@�����O���[�h	*/
	enum MODE {
		LOOP,		///< ���[�v���[�h(�o�b�t�@Full�ɂȂ�����A�������߂�܂ŁA�ŏ��̊������݂�j������)
		LINE,		///< ���C�����[�h(�o�b�t�@Full�ɂȂ�����A����ȏ㏑�����߂Ȃ�)
	};
	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	mode	�o�b�t�@�����O���[�h
	 */
	XRingBufferBL(MODE mode = LOOP);
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XRingBufferBL(void);

	/**	@brief	���[�h��ύX���܂��B
	 *
	 *	@param	mode	�o�b�t�@�����O���[�h	
	 */
	void changeMode(MODE mode){ m_mode = mode; }
	/**	@brief	���[�h���擾���܂��B
	 *	@return	�o�b�t�@�����O���[�h
	 */
	MODE getMode() const { return m_mode; }

	/**	@brief	�o�b�t�@�̃��������m�ۂ��܂��B
	 *
	 *	@param	size	�m�ۂ��郁�����̃T�C�Y
	 *
	 *	@retval	true	succeeded
	 *	@retval	false	failure
	 */
	virtual bool alloc(unsigned long size);
	/**	@brief	�o�b�t�@���A�^�b�`���܂��B
	 *
	 *	@param	p_buff	�A�^�b�`����o�b�t�@�̃A�h���X
	 *	@param	size	�o�b�t�@�̃T�C�Y
	 *
 	 *	@retval	true	succeeded
	 *	@retval	false	failure	
	 */
	virtual bool attach(unsigned char* p_buff, unsigned long size);

	/**	@brief	�������܂�Ă���o�b�t�@�̃T�C�Y���擾���܂��B
	 *
 	 *	@return �������܂�Ă���o�b�t�@�̃T�C�Y
	 */
	virtual unsigned long size(void) const;
	/**	@brief	��o�b�t�@�̃T�C�Y���擾���܂��B
	 *
 	 *	@return ��o�b�t�@�̃T�C�Y
	 */
	virtual unsigned long free_size(void) const;
	/**	@brief	�J�����g�̃u���b�N�T�C�Y���擾���܂�
	 *
 	 *	@return �J�����g�̃u���b�N�T�C�Y�B�u���b�N���Ȃ��ꍇ�͂O
	 */
	virtual unsigned long cur_blk_size(void) const;
	/**	@brief	�������܂�Ă���u���b�N�̃J�E���g����Ԃ��܂�
	 *
 	 *	@return �������܂�Ă���u���b�N�̃J�E���g���B
	 */
	virtual unsigned long blk_count(void) const;
	/**	@brief	�f�[�^���������݂܂��B
	 *
	 *	@param	data	�f�[�^
	 *	@param	size	�f�[�^�̃T�C�Y
	 *
 	 *	@retval	true	succeeded
	 *	@retval	false	��̈悪����Ȃ�	
	 */
	virtual bool put(const unsigned char* data, unsigned long size);
	/**	@brief	�u���b�N�f�[�^���R�s�[���܂��B�ǂݎ��ʒu�̕ω��͂���܂���B
	 *
	 *	@param	data	�ǂݎ��o�b�t�@
	 *	@param	buff_size	data�̃T�C�Y
	 *
	 *	@return	�u���b�N�̃T�C�Y
	 */
	virtual int copy(unsigned char* data, unsigned long size) const;
	/**	@brief	�u���b�N�f�[�^���擾���܂��B�ǂݎ��ʒu��i�߂܂��B
	 *
	 *	@param	data	�ǂݎ��o�b�t�@
	 *	@param	buff_size	data�̃T�C�Y
	 *
	 *	@return	�u���b�N�̃T�C�Y
	 */
	virtual int get(unsigned char* data, unsigned long buff_size);

protected:
	XRingBuffer				m_buff;
	unsigned long			m_ulCurBlock;
	XRingBufferBL::MODE		m_mode;
};


#include "xlib/xlib_tail.h"
#endif // __X_BUFF_HEASER_FILE__
