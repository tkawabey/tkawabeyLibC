/**
 * @file xbuff.h
 * @brief バッファ管クラス
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
 * バッファ管クラス
 */
class XLIB_API XBuff
{
public:
	/** コンストラクタ */
	XBuff();
	/** コンストラクタ */
	XBuff(const XBuff& dst, int dmmy = 0);
	/** デストラクタ */
	virtual ~XBuff();


	/**
	 * 指定したサイズのメモリを確保します。
	 * @note メモリー確保に失敗した場合、WIN32 は、GetLastError で、ERROR_NOT_ENOUGH_MEMORYを取得できます。
	 * その他OSの場合は、errno に、ENOMEM　をセットします。
	 * @param nSize 確保するメモリサイズ
	 * @return 確保したメモリのアドレス。メモリー確保に失敗した場合はNULL。
	 */
	virtual void *Alloc(size_t nSize);
	/**
	 * 指定したサイズのメモリを再確保します。
	 * @note メモリー確保に失敗した場合、WIN32 は、GetLastError で、ERROR_NOT_ENOUGH_MEMORYを取得できます。
	 * その他OSの場合は、errno に、ENOMEM　をセットします。
	 * @param nSize 確保するメモリサイズ
	 * @return 確保したメモリのアドレス
	 */
	virtual void *ReAlloc(size_t nSize);
	/**
	 * メモリーを解放します。
	 */
	virtual void Free();
	/**
	 * 指定した値で、メモリを初期化します
	 * @param c 初期化する値
	 */
	virtual void MemSet(unsigned char c = 0);

	virtual void memsetA(unsigned char c = 0);
	virtual void memsetW(wchar_t c = 0);
	virtual void memsetT(TCHAR c = 0);


	/**	@brief	バッファ間でデータをコピーします。
	 *
	 *	@param	src	コピーするデータ
	 *	@param	count	データのサイズ
	 *	@param	bReUse	割り当て済みのメモリーが指定範囲ないなら、そのバッファを使い回す。場合は、true。一度freeして再割り当てを行う場合はfalse
	 *
	 *	@retval	value	DESC
	 *	@retval	value	DESC
	 *	@verbatim
	 *	@endverbatim
	 *	
	 */
	virtual void* memcpy(const void *src, size_t count, bool bReUse = true);
// 値の取得
	/**
	 * バッファの先頭アドレスを取得します。
	 * @return バッファの先頭アドレス
	 */
	operator void*();
	/**
	 * バッファの先頭アドレスを取得します。
	 * @return バッファの先頭アドレス
	 */
	operator char*();
	/**
	 * バッファの先頭アドレスを取得します。
	 * @return バッファの先頭アドレス
	 */
	operator unsigned char*();
	/**
	 * バッファの先頭アドレスを取得します。
	 * @return バッファの先頭アドレス
	 */
	operator unsigned short*();
	/**
	 * バッファの先頭アドレスを取得します。
	 * @return バッファの先頭アドレス
	 */
	operator unsigned long*();
#ifdef _NATIVE_WCHAR_T_DEFINED
	/**
	 * バッファの先頭アドレスを取得します。
	 * @return バッファの先頭アドレス
	 * @note Zc:wchar_t を指定すると、_WCHAR_T_DEFINED シンボルと _NATIVE_WCHAR_T_DEFINED シンボルが定義されます
	 */
	operator wchar_t*();
#endif

	/**
	 * 確保したバッファのサイズを取得します。
	 * @return 確保したバッファのサイズ
	 */
	virtual size_t GetBuffSize() const;
	/**
	 * バッファの最後のアドレスを取得します。
	 * @return バッファの最後のアドレス
	 */
	void *GetMaxBuffAddress() const;
	/**
	 * 指定したバッファインスタンスと同じバッファを確保します。
	 * @param pSrc コピー元バッファインスタンス
	 * @return 確保したバッファの先頭アドレス
	 */
	virtual void *DoCopy(XBuff *pSrc);


// Operator
	/**
	 * @brief	指定したバッファと比較します。
	 * @param lpsz 比較するバッファ
	 * @retval true 等しい
	 * @retval false 等しくない
	 */
	bool operator==(const XBuff& dst) const;
	/**
	 * @brief	指定したバッファと比較します。
	 * @param lpsz 比較するバッファ
	 * @retval true 等しくない
	 * @retval false 等しい
	 */
	bool operator!=(const XBuff& dst) const;
	/**
	 * @brief	指定したバッファと代入します。
	 * @param lpsz 代入するバッファ
	 * @return	自身
	 */
	XBuff& operator=(const XBuff& dst);
protected:
	/** バッファのアドレス */
	void*			m_pBuff;
	/** バッファサイズ */
	size_t			m_nBuffSize;
};



/**
 * STLストリーム用バッファ自動拡張クラス

@verbatim
	// リングバッファのサンプル
	XLIB_STD_STREAM_BUFFER buff(
			10,		//	初期バッファサイズ
			10		//  バッファが足らなくなったら、増やすサイズ
	  );
	// ストリームにセットする
	XLIB_STD_OUT_STR_STREAM strm(&buff);

	// バッファに書き込み
	strm << "aaa" << "bbb" << (int)100;
	strm.write("0123456789", 10);

	// 書き込まれたバッファのサイズ
	printf("size:%d", buff.size());
	
	// バッファの先頭アドレスを取得
	char *p = buff.getBuff();

	// バッファの書き込み位置を先頭に戻す。
	buff,resetp();

	// 書き込まれたバッファのサイズは０に戻る
	printf("size:%d", buff.size());

@endverbatim

 */
class XLIB_API XInflateStreamBuff :  public std::streambuf
{
public:

	/**	@brief	コンストラクタ
		@param	nInitSize	初期確保するバッファサイズ
		@param	nIncrementSize	バッファが足りなくなったら、増加するサイズ
	*/
	explicit XInflateStreamBuff(unsigned long nInitSize = 2048, unsigned long nIncrementSize = 1024);
	/**	@brief	デストラクタ
	*/
	virtual ~XInflateStreamBuff();

	/**	@brief	バッファに書き込まれた文字列を読み込む
		@return	バッファに書き込まれた文字列
	*/
	const char *toString() {
		*pptr() = 0;
		return m_pBuffer;
	}
	/**	@brief	現在に位置のアドレスを返します。
	 *	@return	現在に位置のアドレス
	 */
	const char *getCurrentAddress() {
		return pptr();
	}
	/**	@brief	バッファを取得
		@return	バッファ
	*/
	unsigned char *getBuff() {
		return (unsigned char*)m_pBuffer;
	}
	/**	@brief	バッファのサイズを取得
		@return	バッファのサイズ
	*/
	unsigned long getBuffSize() {
		return m_nBufferSize;
	}

	/**	@brief	書き込んだサイズを取得
		@return	書き込んだサイズ
	*/
	unsigned long size() {
		unsigned long nSize = (unsigned long)(pptr() - m_pBuffer);
		return nSize;
	}

	/**	@brief	入力ポインタを先頭に戻します。	*/
	void resetp() {
		setp( m_pBuffer, m_pBuffer + (m_nBufferSize-1) );
		setg( m_pBuffer, m_pBuffer, m_pBuffer + (m_nBufferSize-1) );
	}



protected:

	/*	@brief	指定したオフセット位置に移動します。
	 *	@param	off	新しいオフセット値。streamoff は、typedef 文により long と同義です
	 *	@param	dir	移動方向。次の seek_dir 列挙子のどれかを指定します。
	 *	@param	nMode ios::in と ios::out 列挙子をビットごとの OR (|) で組み合わせた整数値
	 *	@return	移動した後の位置を返します。
	 */
	virtual std::streampos seekoff( std::streamoff off, std::ios::seekdir dir, std::ios::openmode nMode  = std::ios::in | std::ios::out );
	virtual std::streampos seekpos( std::streampos, std::ios::openmode = std::ios::in | std::ios::out);
	/*	@brief バッファが足らなくなるとこーるされる。
		@param	nCh	EOF または、出力する文字
		@return	エラーが発生した時は、EOF を返します。
	*/
	virtual int overflow( int nCh = EOF );
	/* バッファのアドレス */
	char*         m_pBuffer;
	char*		  _Seekhigh;
	/* バッファのサイズ */
	unsigned long m_nBufferSize;
	/* バッファを増加するサイズ */
	unsigned long m_nIncrementSize;
};


class XLIB_API XInflateStreamBuffW :  public std::wstreambuf
{
public:

	/**	@brief	コンストラクタ
		@param	nInitSize	初期確保するバッファサイズ
		@param	nIncrementSize	バッファが足りなくなったら、増加するサイズ
	*/
	explicit XInflateStreamBuffW(unsigned long nInitSize = 2048, unsigned long nIncrementSize = 1024);
	/**	@brief	デストラクタ
	*/
	virtual ~XInflateStreamBuffW();

	/**	@brief	バッファに書き込まれた文字列を読み込む
		@return	バッファに書き込まれた文字列
	*/
	const wchar_t *toString() {
		*pptr() = 0;
		return m_pBuffer;
	}
	/**	@brief	現在に位置のアドレスを返します。
	 *	@return	現在に位置のアドレス
	 */
	const wchar_t *getCurrentAddress() {
		return pptr();
	}
	/**	@brief	バッファを取得
		@return	バッファ
	*/
	wchar_t *getBuff() {
		return (wchar_t*)m_pBuffer;
	}
	/**	@brief	バッファのサイズを取得
		@return	バッファのサイズ
	*/
	unsigned long getBuffSize() {
		return m_nBufferSize;
	}

	/**	@brief	書き込んだサイズを取得
		@return	書き込んだサイズ
	*/
	unsigned long size() {
		unsigned long nSize = (unsigned long)(pptr() - m_pBuffer);
		return nSize;
	}

	/**	@brief	入力ポインタを先頭に戻します。	*/
	void resetp() {
		setp( m_pBuffer, m_pBuffer + ((m_nBufferSize/sizeof(wchar_t))-1) );
		setg( m_pBuffer, m_pBuffer, m_pBuffer + ((m_nBufferSize/sizeof(wchar_t))-1) );
	}



protected:

	/*	@brief	指定したオフセット位置に移動します。
	 *	@param	off	新しいオフセット値。streamoff は、typedef 文により long と同義です
	 *	@param	dir	移動方向。次の seek_dir 列挙子のどれかを指定します。
	 *	@param	nMode ios::in と ios::out 列挙子をビットごとの OR (|) で組み合わせた整数値
	 *	@return	移動した後の位置を返します。
	 */
	virtual std::streampos seekoff( std::streamoff off, std::ios::seekdir dir, std::ios::openmode nMode  = std::ios::in | std::ios::out );
	virtual std::streampos seekpos( std::streampos, std::ios::openmode = std::ios::in | std::ios::out);
	/*	@brief バッファが足らなくなるとこーるされる。
		@param	nCh	EOF または、出力する文字
		@return	エラーが発生した時は、EOF を返します。
	*/
	virtual wint_t overflow( wint_t nCh = EOF );
	/* バッファのアドレス */
	wchar_t*         m_pBuffer;
	wchar_t*		  _Seekhigh;
	/* バッファのサイズ */
	unsigned long m_nBufferSize;
	/* バッファを増加するサイズ */
	unsigned long m_nIncrementSize;
};




/**	@brief	自動メモリ拡張バイナリーバッファストリーム	*/
class XLIB_API XInflateBinStrstream : public std::iostream 

{
public:
	/**	@brief	コンストラクタ	*/
	XInflateBinStrstream();
	/**	@brief	コンストラクタ
	 *	@param	nInitSize	初期確保するバッファサイズ
	 *	@param	nIncrementSize	バッファが足りなくなったら、増加するサイズ
	 */
    XInflateBinStrstream(unsigned long nInitSize, unsigned long nIncrementSize = 1024);
	/**	@brief	デストラクタ	*/
	virtual ~XInflateBinStrstream();
	/**	@brief	ストリームバッファを取得
	 *	@return	ストリームバッファ
	 */
    XInflateStreamBuff *rdbuf() const;
	/**	@brief	バッファに書き込まれた文字列を読み込む
	 *	@return	バッファに書き込まれた文字列
	 */
	XBIN *buffer();
	/**	@brief	書き込んだサイズを取得
	 *	@return	書き込んだサイズ
	 */
	std::streamsize pcount();
	/**	@brief	入力ポインタを先頭に戻します。	*/
	void resetp();
protected:
	XInflateStreamBuff	m_buff;
};





/**	@brief	リングバッファ	
@verbatim
	// リングバッファのサンプル
	XRingBufferBL buff();
	// バッファを確保
	bull.alloc(500);

	// Data1 ブロック書き込み
	bull.put(pData1, 200);
	
	// Data2 ブロック書き込み
	bull.put(pData2, 250);


	// Data3 ブロック書き込み
	// バッファFullになっているので、これ以上書き込めないい
	bull.put(pData3, 200);

	// リングバッファから、100バイト読み込み
	len = buff.copy(rcvbuff, 100);
@endverbatim

*/
class XLIB_API XRingBuffer {
public:
	/**	@brief	コンストラクタ	*/
	XRingBuffer();
	/**	@brief	デストラクタ	*/
	virtual ~XRingBuffer(void);

	/**	@brief	バッファのメモリを確保します。
	 *
	 *	@param	size	確保するメモリのサイズ
	 *
	 *	@retval	true	succeeded
	 *	@retval	false	failure
	 */
	virtual bool alloc(unsigned long size);
	/**	@brief	バッファをアタッチします。
	 *
	 *	@param	p_buff	アタッチするバッファのアドレス
	 *	@param	size	バッファのサイズ
	 *
 	 *	@retval	true	succeeded
	 *	@retval	false	failure	
	 */
	virtual bool attach(unsigned char* p_buff, unsigned long size);

	/**	@brief	書き込まれているバッファのサイズを取得します。
	 *
 	 *	@return 書き込まれているバッファのサイズ
	 */
	virtual unsigned long size(void) const;
	/**	@brief	空バッファのサイズを取得します。
	 *
 	 *	@return 空バッファのサイズ
	 */
	virtual unsigned long free_size(void) const;
	/**	@brief	データを書き込みます。
	 *
	 *	@param	data	データ
	 *	@param	size	データのサイズ
	 *
 	 *	@retval	true	succeeded
	 *	@retval	false	空領域が足らない	
	 */
	virtual bool put(const unsigned char* data, unsigned long size);
	/**	@brief	データをコピーします。読み取り位置の変化はありません。
	 *
	 *	@param	data	読み取るバッファ
	 *	@param	size	バッファのサイズ
	 *
	 *	@return	実際に読み取ったサイズ
	 */
	virtual int copy(unsigned char* data, unsigned long size) const;
	/**	@brief	データを取得します。読み取り位置を進めます。
	 *
	 *	@param	data	読み取るバッファ
	 *	@param	size	バッファのサイズ
	 *
	 *	@return	実際に読み取ったサイズ
	 */
	virtual int get(unsigned char* data, unsigned long size);
	/**	@brief	クリアします。 */
	virtual void clear(void);
private:
	unsigned long	m_ulRealSize;
	unsigned long	m_ulFirst;
	unsigned long	m_ulLast;
	unsigned char	*m_pBuffer;
	bool			m_bAlloc;
};

/**	@brief	ブロック単位のリングバッファ
@verbatim
	// ループリングバッファのサンプル
	XRingBufferBL buff(XRingBufferBL::LOOP);
	// バッファを確保
	bull.alloc(500);

	// Data1 ブロック書き込み
	bull.put(pData1, 200);
	
	// Data2 ブロック書き込み
	bull.put(pData2, 250);

	// 書き込まれたブロックのカウント数と、popした時に読み込みに必要なバッファのサイズ
	printf("block count:%d block size:%d", 
		bull.blk_count(), 
		bull.cur_blk_size());

	// Data3 ブロック書き込み
	// バッファFullになっているので、pData1は破棄されている
	bull.put(pData3, 200);

	// pop します。リード位置は変わりません。(pData2を取得できます)
	//  len は書き込んだブロックサイズ。つまり250.
	len = buff.copy(rcvbuff, sizeof(rcvbuff));

	// pop します。(pData3を取得できます)
	//  len は書き込んだブロックサイズ。つまり200.
	len = buff.get(rcvbuff, sizeof(rcvbuff));
@endverbatim

@verbatim
	// ラインリングバッファのサンプル
	XRingBufferBL buff(XRingBufferBL::LINE);
	// バッファを確保
	bull.alloc(500);

	// Data1 ブロック書き込み
	bull.put(pData1, 200);
	
	// Data2 ブロック書き込み
	bull.put(pData2, 250);

	// 書き込まれたブロックのカウント数と、popした時に読み込みに必要なバッファのサイズ
	printf("block count:%d block size:%d", 
		bull.blk_count(), 
		bull.cur_blk_size());

	// Data3 ブロック書き込み
	// バッファFullになっているので、書き込めない
	if( bull.put(pData3, 200) == false ) {
		fprintf(stderr, "buff full!!.");
	}

	// pop します。リード位置は変わりません。(pData2を取得できます)
	//  len は書き込んだブロックサイズ。つまり250.
	len = buff.copy(rcvbuff, sizeof(rcvbuff));

	// pop します。(pData3を取得できます)
	//  len は書き込んだブロックサイズ。つまり200.
	len = buff.get(rcvbuff, sizeof(rcvbuff));
@endverbatim


*/
class XLIB_API XRingBufferBL {
public:

	/**	@brief	バッファリングモード	*/
	enum MODE {
		LOOP,		///< ループモード(バッファFullになったら、書き込めるまで、最初の割きこみを破棄する)
		LINE,		///< ラインモード(バッファFullになったら、それ以上書き込めない)
	};
	/**	@brief	コンストラクタ
	 *
	 *	@param	mode	バッファリングモード
	 */
	XRingBufferBL(MODE mode = LOOP);
	/**	@brief	デストラクタ	*/
	virtual ~XRingBufferBL(void);

	/**	@brief	モードを変更します。
	 *
	 *	@param	mode	バッファリングモード	
	 */
	void changeMode(MODE mode){ m_mode = mode; }
	/**	@brief	モードを取得します。
	 *	@return	バッファリングモード
	 */
	MODE getMode() const { return m_mode; }

	/**	@brief	バッファのメモリを確保します。
	 *
	 *	@param	size	確保するメモリのサイズ
	 *
	 *	@retval	true	succeeded
	 *	@retval	false	failure
	 */
	virtual bool alloc(unsigned long size);
	/**	@brief	バッファをアタッチします。
	 *
	 *	@param	p_buff	アタッチするバッファのアドレス
	 *	@param	size	バッファのサイズ
	 *
 	 *	@retval	true	succeeded
	 *	@retval	false	failure	
	 */
	virtual bool attach(unsigned char* p_buff, unsigned long size);

	/**	@brief	書き込まれているバッファのサイズを取得します。
	 *
 	 *	@return 書き込まれているバッファのサイズ
	 */
	virtual unsigned long size(void) const;
	/**	@brief	空バッファのサイズを取得します。
	 *
 	 *	@return 空バッファのサイズ
	 */
	virtual unsigned long free_size(void) const;
	/**	@brief	カレントのブロックサイズを取得します
	 *
 	 *	@return カレントのブロックサイズ。ブロックがない場合は０
	 */
	virtual unsigned long cur_blk_size(void) const;
	/**	@brief	書き込まれているブロックのカウント数を返します
	 *
 	 *	@return 書き込まれているブロックのカウント数。
	 */
	virtual unsigned long blk_count(void) const;
	/**	@brief	データを書き込みます。
	 *
	 *	@param	data	データ
	 *	@param	size	データのサイズ
	 *
 	 *	@retval	true	succeeded
	 *	@retval	false	空領域が足らない	
	 */
	virtual bool put(const unsigned char* data, unsigned long size);
	/**	@brief	ブロックデータをコピーします。読み取り位置の変化はありません。
	 *
	 *	@param	data	読み取るバッファ
	 *	@param	buff_size	dataのサイズ
	 *
	 *	@return	ブロックのサイズ
	 */
	virtual int copy(unsigned char* data, unsigned long size) const;
	/**	@brief	ブロックデータを取得します。読み取り位置を進めます。
	 *
	 *	@param	data	読み取るバッファ
	 *	@param	buff_size	dataのサイズ
	 *
	 *	@return	ブロックのサイズ
	 */
	virtual int get(unsigned char* data, unsigned long buff_size);

protected:
	XRingBuffer				m_buff;
	unsigned long			m_ulCurBlock;
	XRingBufferBL::MODE		m_mode;
};


#include "xlib/xlib_tail.h"
#endif // __X_BUFF_HEASER_FILE__
