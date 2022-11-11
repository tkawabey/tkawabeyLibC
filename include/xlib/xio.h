/**
 * @file xio.h
 * @brief IOアクセス抽象クラス
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
 * @brief	IOアクセス抽象クラス
 */
class XLIB_API XIO
{
public:
	/** @brief	コンストラクタ */
	XIO();
	/** @brief	デストラクタ */
	virtual ~XIO();
	/**
	 * @brief	クローズします。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Close() = 0;
	/**
	 * @brief	文字列を読み込み
	 * @param buff 文字列を取得するためのバッファ
	 * @param code_type 文字コードのサイズ
	 * @param flg フラグ。派生先のインプリにより変わります
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool ReadLine(XStrBuff &buff, int code_type = 1, int flg = 0);
	/**
	 * @brief	バッファを読み込みます。
	 * @param lpData バッファ
	 * @param nSize バッファのサイズ
	 * @param flg フラグ。派生先のインプリにより変わります
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Read(void* lpData, size_t nSize, int flg = 0);
	/**
	 * @brief	文字列を書き込みます。
	 * @param lpszData 書き込む文字列
	 * @param flg フラグ。派生先のインプリにより変わります
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool WriteString(const char* lpszData, int flg = 0);
	/**
	 * @brief	文字列を書き込みます。
	 * @param lpszData 書き込む文字列
	 * @param flg フラグ。派生先のインプリにより変わります
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool WriteString(const wchar_t* lpszData, int flg = 0);
	/**
	 * @brief	バッファを書き込みます。
	 * @param lpData 書き込むバッファ
	 * @param nSize lpDataのサイズ
	 * @param flg フラグ。派生先のインプリにより変わります
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Write(const void* lpData, size_t nSize, int flg = 0);
	/**
	 * @brief	バッファを書き込みます。実装部
	 * @param lpData 書き込むバッファ
	 * @param nSize lpDataのサイズ
	 * @param flg フラグ。派生先のインプリにより変わります
	 * @return 書き込んだバイト数。読み込みエラーが発生した場合は、-１を返します。
	 */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0) = 0;
	/**
	 * @brief	バッファを読み込みます。実装部
	 * @param lpData 読み込むバッファを格納するのアドレス
	 * @param nSize lpDataのサイズ
	 * @param flg フラグ。派生先のインプリにより変わります
	 * @return 読み込んだバイト数。読み込みエラーが発生した場合は、-１を返します。
	 */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0) = 0;
};


/**
 * バイト読み込みストリームクラス
 */
class XLIB_API XInputByteStream
{
public:
	/** コンストラクタ */
	XInputByteStream();
	/** デストラクタ */
	virtual ~XInputByteStream(){}

	/**	@brief	1Byteを戻り値として返して、内部ポインタを１Byte進めます
	 *	@return	次のByte
	*/
	XUBYTE1 GetByte();
	/**	@brief	2-Byeteをbidendianに変換し戻り値として返して、内部ポインタを2Byte進めます
	 *	@return	次の２Byteとbig-endianに変換したInteger型
	*/
	XUBYTE2 GetBigEndianWord();
	/**	@brief	4-Byeteをbidendianに変換し戻り値として返して、内部ポインタを4Byte進めます
		@return	次の4Byteとbig-endianに変換したInteger型
	*/
	XUBYTE4 GetBigEndianLong();

	/**	@brief	ストリームから指定したBYTEを読み込みます
	 *	@param	buffer	読み込まれるバッファ
	 *	@param	count	バッファのサイズ
	 *	@return	読み込んだByte数
	 */
	int Read(char *buffer, int count);
	/**	@brief	The MoreData function
	 *	@return	bool
	 */
	bool MoreData();

	/**	@brief	BITの位置をセット
		@param	initialposition	BITの位置。０～８。
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

	/* カレントバイト */
	XUBYTE1		*m_pCurrentByte ;
	/* バッファの上限の位置 */
	XUBYTE1		*m_pBufferLimit ;
	/* BITの位置 */
	int			m_nBitPosition ;
} ;

/**
 * バイト書き込みストリームクラス
 */
class XLIB_API XOutputByteStream
{
public:
	/** コンストラクタ */
	XOutputByteStream();
	/** デストラクタ */
	virtual ~XOutputByteStream(){};

	/**	@brief	ストリームに指定したByteを書き込みます
		@param	buffer	書き込むバッファ
		@param	count	バッファのサイズ
		@return	void
	*/
	void Write(const char *buffer, int count);
	/**	@brief	1BYTE書き込みます
		@param	value	書き込むBYTE
	*/
	void WriteByte(XUBYTE1 value);
	/**	@brief	2BYTE書き込みます
		@param	value	書き込むバッファ
	*/
	void WriteBigEndianWord(XUBYTE2 value);
	/**	@brief４BYTE書き込みます
		@param	value	書き込むバッファ
	*/
	void WriteBigEndianLong(XUBYTE4 value);
	/**	@brief	BITの位置をセット
		@param	initialposition BITの位置。０～８。
	*/
	void EnterBitMode(int initialposition);
	/**	@brief	バッファを１バイト勧めます
	*/
	void NextByte(); 
	/**	@brief	バッファの残りのバイト数を返します
	 *	@return	バッファの残りのバイト数
	 */
	unsigned int RemainingBufferSpace () const ;
protected:
	/**	バッファをフラッシュします。 */
	virtual void FlushBuffer() = 0 ;

	/* カレントバイト */
	XUBYTE1	*m_pCurrentByte;
	/* バッファの上限の位置 */
	XUBYTE1	*m_pBufferLimit;
	/* BITの位置 */
	int		m_nBitPosition ;
} ;



#include "xlib/xlib_tail.h"
#endif // __X_IO_HEASER_FILE__
