/**
 * @file xio.h
 * @brief IO�A�N�Z�X���ۃN���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_IO_HEASER_FILE__
#define __X_IO_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"
#include <stdexcept>
#include <iosfwd>
#include <string>

inline XUBYTE2 XSystemToLittleEndian (XUBYTE2 value)
{
	return value ;
}
inline XUBYTE4 XSystemToLittleEndian (XUBYTE4 value)
{
	return value ;
}
inline XUBYTE2 XSystemToBigEndian (XUBYTE2 value)
{
	return (XUBYTE2) ((value&0xFF) << 8)|((value&0xFF00)>>8) ;
}
inline XUBYTE4 XSystemToBigEndian (XUBYTE4 value)
{
	return ((value&0xFF000000L)>>24)|((value&0xFF0000L)>>8)
         | ((value&0xFF00L)<<8) | ((value&0xFFL)<<24) ;
}

inline XUBYTE2 XLittleEndianToSystem (XUBYTE2 value)
{
	return value ;
}
inline XUBYTE4 XLittleEndianToSystem (XUBYTE4 value)
{
	return value ;
}
inline XUBYTE2 XBigEndianToSystem (XUBYTE2 value)
{
	return (XUBYTE2) ((value&0xFF) << 8)|((value&0xFF00)>>8) ;
}
inline XUBYTE4 XBigEndianToSystem (XUBYTE4 value)
{
	return ((value&0xFF000000L)>>24)|((value&0xFF0000L)>>8)
         | ((value&0xFF00L)<<8) | ((value&0xFFL)<<24) ;
}




/**
 * @brief	IO�A�N�Z�X���ۃN���X
 */
class XLIB_API XIO
{
public:
	/** @brief	�R���X�g���N�^ */
	XIO();
	/** @brief	�f�X�g���N�^ */
	virtual ~XIO();
	/**
	 * @brief	�N���[�Y���܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Close() = 0;
	/**
	 * @brief	�������ǂݍ���
	 * @param buff ��������擾���邽�߂̃o�b�t�@
	 * @param code_type �����R�[�h�̃T�C�Y
	 * @param flg �t���O�B�h����̃C���v���ɂ��ς��܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool ReadLine(XStrBuff &buff, int code_type = 1, int flg = 0);
	/**
	 * @brief	�o�b�t�@��ǂݍ��݂܂��B
	 * @param lpData �o�b�t�@
	 * @param nSize �o�b�t�@�̃T�C�Y
	 * @param flg �t���O�B�h����̃C���v���ɂ��ς��܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Read(void* lpData, size_t nSize, int flg = 0);
	/**
	 * @brief	��������������݂܂��B
	 * @param lpszData �������ޕ�����
	 * @param flg �t���O�B�h����̃C���v���ɂ��ς��܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool WriteString(const char* lpszData, int flg = 0);
	/**
	 * @brief	��������������݂܂��B
	 * @param lpszData �������ޕ�����
	 * @param flg �t���O�B�h����̃C���v���ɂ��ς��܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool WriteString(const wchar_t* lpszData, int flg = 0);
	/**
	 * @brief	�o�b�t�@���������݂܂��B
	 * @param lpData �������ރo�b�t�@
	 * @param nSize lpData�̃T�C�Y
	 * @param flg �t���O�B�h����̃C���v���ɂ��ς��܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Write(const void* lpData, size_t nSize, int flg = 0);
	/**
	 * @brief	�o�b�t�@���������݂܂��B������
	 * @param lpData �������ރo�b�t�@
	 * @param nSize lpData�̃T�C�Y
	 * @param flg �t���O�B�h����̃C���v���ɂ��ς��܂�
	 * @return �������񂾃o�C�g���B�ǂݍ��݃G���[�����������ꍇ�́A-�P��Ԃ��܂��B
	 */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0) = 0;
	/**
	 * @brief	�o�b�t�@��ǂݍ��݂܂��B������
	 * @param lpData �ǂݍ��ރo�b�t�@���i�[����̃A�h���X
	 * @param nSize lpData�̃T�C�Y
	 * @param flg �t���O�B�h����̃C���v���ɂ��ς��܂�
	 * @return �ǂݍ��񂾃o�C�g���B�ǂݍ��݃G���[�����������ꍇ�́A-�P��Ԃ��܂��B
	 */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0) = 0;
};


/**
 * �o�C�g�ǂݍ��݃X�g���[���N���X
 */
class XLIB_API XInputByteStream
{
public:
	/** �R���X�g���N�^ */
	XInputByteStream();
	/** �f�X�g���N�^ */
	virtual ~XInputByteStream(){}

	/**	@brief	1Byte��߂�l�Ƃ��ĕԂ��āA�����|�C���^���PByte�i�߂܂�
	 *	@return	����Byte
	*/
	XUBYTE1 GetByte();
	/**	@brief	2-Byete��bidendian�ɕϊ����߂�l�Ƃ��ĕԂ��āA�����|�C���^��2Byte�i�߂܂�
	 *	@return	���̂QByte��big-endian�ɕϊ�����Integer�^
	*/
	XUBYTE2 GetBigEndianWord();
	/**	@brief	4-Byete��bidendian�ɕϊ����߂�l�Ƃ��ĕԂ��āA�����|�C���^��4Byte�i�߂܂�
		@return	����4Byte��big-endian�ɕϊ�����Integer�^
	*/
	XUBYTE4 GetBigEndianLong();

	/**	@brief	�X�g���[������w�肵��BYTE��ǂݍ��݂܂�
	 *	@param	buffer	�ǂݍ��܂��o�b�t�@
	 *	@param	count	�o�b�t�@�̃T�C�Y
	 *	@return	�ǂݍ���Byte��
	 */
	int Read(char *buffer, int count);
	/**	@brief	The MoreData function
	 *	@return	bool
	 */
	bool MoreData();

	/**	@brief	BIT�̈ʒu���Z�b�g
		@param	initialposition	BIT�̈ʒu�B�O�`�W�B
	 */
	void EnterBitMode(int initialposition);
	
#if defined (_MSC_VER)
	/**	\brief	Redefinition of std::fpos<int> as POSITIONTYPE
	*/
	typedef std::fpos<int> POSITIONTYPE;
#elif (__linux__)

	typedef streamoff POSITIONTYPE;
#else
	/**	\brief	Redefinition of std::char_traits<char>::pos_type as POSITIONTYPE
	 */
	typedef std::char_traits<char>::pos_type POSITIONTYPE;
#endif
	/**	@brief	The Tellg function
		@return	virtual POSITIONTYPE
	*/
	virtual POSITIONTYPE Tellg(){ return 0 ; }
	
	/**	@brief	The Seekg function
	 *	@param	pos	a parameter of type POSITIONTYPE
	 *	@return	virtual void
	 */
	virtual void Seekg(POSITIONTYPE pos){}
protected:
	/**	@brief	The FillBuffer function
	 *	@return	virtual void
	 */
	virtual void FillBuffer() = 0 ;
	/**	@brief	The EndReached function
	 *	@return	virtual bool
	 */
	virtual bool EndReached() = 0 ;

	/* �J�����g�o�C�g */
	XUBYTE1		*m_pCurrentByte ;
	/* �o�b�t�@�̏���̈ʒu */
	XUBYTE1		*m_pBufferLimit ;
	/* BIT�̈ʒu */
	int			m_nBitPosition ;
} ;

/**
 * �o�C�g�������݃X�g���[���N���X
 */
class XLIB_API XOutputByteStream
{
public:
	/** �R���X�g���N�^ */
	XOutputByteStream();
	/** �f�X�g���N�^ */
	virtual ~XOutputByteStream(){};

	/**	@brief	�X�g���[���Ɏw�肵��Byte���������݂܂�
		@param	buffer	�������ރo�b�t�@
		@param	count	�o�b�t�@�̃T�C�Y
		@return	void
	*/
	void Write(const char *buffer, int count);
	/**	@brief	1BYTE�������݂܂�
		@param	value	��������BYTE
	*/
	void WriteByte(XUBYTE1 value);
	/**	@brief	2BYTE�������݂܂�
		@param	value	�������ރo�b�t�@
	*/
	void WriteBigEndianWord(XUBYTE2 value);
	/**	@brief�SBYTE�������݂܂�
		@param	value	�������ރo�b�t�@
	*/
	void WriteBigEndianLong(XUBYTE4 value);
	/**	@brief	BIT�̈ʒu���Z�b�g
		@param	initialposition BIT�̈ʒu�B�O�`�W�B
	*/
	void EnterBitMode(int initialposition);
	/**	@brief	�o�b�t�@���P�o�C�g���߂܂�
	*/
	void NextByte(); 
	/**	@brief	�o�b�t�@�̎c��̃o�C�g����Ԃ��܂�
	 *	@return	�o�b�t�@�̎c��̃o�C�g��
	 */
	unsigned int RemainingBufferSpace () const ;
protected:
	/**	�o�b�t�@���t���b�V�����܂��B */
	virtual void FlushBuffer() = 0 ;

	/* �J�����g�o�C�g */
	XUBYTE1	*m_pCurrentByte;
	/* �o�b�t�@�̏���̈ʒu */
	XUBYTE1	*m_pBufferLimit;
	/* BIT�̈ʒu */
	int		m_nBitPosition ;
} ;



#include "xlib/xlib_tail.h"
#endif // __X_IO_HEASER_FILE__
