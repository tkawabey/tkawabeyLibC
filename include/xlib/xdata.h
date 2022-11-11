/**
 * @file xdata.h
 * @brief バイナリーデータアクセスクラス郡
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DATA_HEASER_FILE__
#define __X_DATA_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"

#include <iostream>
/*

 ●TAG+Attributeなしのフォーマット
   ホストｰ装置間のSECSメッセージのデータと交換性があり

 1byte目　タイプ＋データサイズのサイズ
  ２ビットで表現すると
   　XXXXXXYY
  のようになる。

   XXXXXX=タイプ
	XD2_END=       000000  データの終わり
	XD2_L=         000100  リスト
	XD2_B=         001000  バイナリー
	XD2_BOOL=      001100  論理値
	XD2_A=         010000  ASCII文字
	XD2_UNI8=      010001  UTF8文字
	XD2_UNI16=     010010  UTF16文字
	XD2_J=         010100  JIS8文字
	XD2_U1=        011000  符号なし整数1Byte
	XD2_U2=        011001  符号なし整数2Byte
	XD2_U4=        011010  符号なし整数4Byte
	XD2_U8=        011011  符号なし整数8Byte
	XD2_I1=        011100  符号あり整数1Byte
	XD2_I2=        011101  符号あり整数2Byte
	XD2_I4=        011110  符号あり整数4Byte
	XD2_I8=        011111  符号あり整数8Byte
	XD2_F4=        100010  実数4Byte
	XD2_F8=        100011  実数8Byte
	XD2_STRING=    100100  ASCII文字
	XD2_JSTRING=   101000  JIS8文字
	XD2_UCHAR=     101100  符号なし整数1Byte
	XD2_USHORT=    101101  符号なし整数2Byte
	XD2_ULONG=     101110  符号なし整数4Byte
	XD2_UHUGE=     101111  符号なし整数8Byte
	XD2_CHAR=      110000  符号あり整数1Byte
	XD2_SHORT=     110001  符号あり整数2Byte
	XD2_LONG=      110010  符号あり整数4Byte
	XD2_HUGE=      110011  符号あり整数8Byte
	XD2_FLOAT=     110110  実数4Byte
	XD2_DOUBLE=    110111  実数8Byte
   YY=データサイズのサイズ
    01=1バイトで表現できるデータサイズ
    10=2バイトで表現できるデータサイズ
    11=4バイトで表現できるデータサイズ
 
   
 2byte目～　データサイズ
　　1byte目のYY=データサイズのサイズによってサイズが異なる。
	ネットワークバイトオーダーの形式のデータのサイズが入っている。

 Xbyte目～　データ


 ●TAG+Attribute付のフォーマット
 　<L.X+1
     <A. "タグ名">
     <L.x
       <L.2
         <A "属性名">
         <? "属性値">
       >
     >
	 ...
	 ...
   >

 　<L.3
     <A. "タグ名">
     <L.x
       <L.2
         <A "属性名">
         <? "属性値">
       >
     >
     <? "実際の値">
   >
*/




#define XD2_TYP(x,y) ((x<<2)|y)
#define XD2_END       XD2_TYP(0,0)        /* End of Message               */
#define XD2_L         XD2_TYP(1,0)        /* List                         */
#define XD2_NULL      XD2_TYP(1,1)        /* NULL                         */
#define XD2_B         XD2_TYP(2,0)        /* Binary                       */
#define XD2_BOOL      XD2_TYP(3,0)        /* Boolean                      */
#define XD2_A         XD2_TYP(4,0)        /* ASCII                        */
#define XD2_UTF16     XD2_TYP(4,1)        /* UTF16                        */
#define XD2_J         XD2_TYP(5,0)        /* JIS-8                        */
#define XD2_U1        XD2_TYP(6,0)        /* Unsigned Integer 1-Byte      */
#define XD2_U2        XD2_TYP(6,1)        /* Unsigned Integer 2-Byte      */
#define XD2_U4        XD2_TYP(6,2)        /* Unsigned Integer 4-Byte      */
#define XD2_U8        XD2_TYP(6,3)        /* Unsigned Integer 8-Byte      */
#define XD2_I1        XD2_TYP(7,0)        /* Signed Integer 1-Byte        */
#define XD2_I2        XD2_TYP(7,1)        /* Signed Integer 2-Byte        */
#define XD2_I4        XD2_TYP(7,2)        /* Signed Integer 4-Byte        */
#define XD2_I8        XD2_TYP(7,3)        /* Signed Integer 8-Byte        */
#define XD2_F4        XD2_TYP(8,2)        /* Floating Point 32-Bit        */
#define XD2_F8        XD2_TYP(8,3)        /* Floating Point 64-Bit        */
#define XD2_STRING    XD2_TYP(9,0)        /* 'string' target              */
#define XD2_JSTRING   XD2_TYP(10,0)       /* 'string' target              */
#define XD2_DATE      XD2_TYP(10,1)       /* Date                         */
#define XD2_TIME      XD2_TYP(10,2)       /* Time                         */
#define XD2_TIMESTAMP XD2_TYP(10,3)       /* TimeStamp                    */
#define XD2_UCHAR     XD2_TYP(11,0)       /* 'unsigned char' target       */
#define XD2_USHORT    XD2_TYP(11,1)       /* 'unsigned short' target      */
#define XD2_ULONG     XD2_TYP(11,2)       /* 'unsigned long' target       */
#define XD2_UHUGE     XD2_TYP(11,3)       /* 'unsigned huge' target       */
#define XD2_CHAR      XD2_TYP(12,0)       /* 'char' target                */
#define XD2_SHORT     XD2_TYP(12,1)       /* 'short' target               */
#define XD2_LONG      XD2_TYP(12,2)       /* 'long' target                */
#define XD2_HUGE      XD2_TYP(12,3)       /* 'signed huge' target         */
#define XD2_FLOAT     XD2_TYP(13,2)       /* 'float' target               */
#define XD2_DOUBLE    XD2_TYP(13,3)       /* 'double' target              */
/* -------------------------------------------------------------------- */
#define XD2_ERRDI -1             /* Data Item Error                      */
#define XD2_ERRSS -2             /* Data Size Error                      */
#define XD2_ERRAS -3             /* Array Size Error                     */
#define XD2_ERRTY -4             /* Data Item Type Error                 */
#define XD2_ERRCV -5             /* Data Conversion Error                */
#define XD2_ERRMA -6			 /* Buffer Allocate Error                */
#define XD2_ERRIT -7			 /* Invalid index/TAG */



/**
 * @brief フォーマットタイプ
 */
enum XDATA_FORMAT_TYPE {
	XDATA_TYPE_NOMAL,		///< 通常フォーマット。SECS Like 
	XDATA_TYPE_LIKE_XML,	///< 拡張データフォーマット。TAG+Attribute付のフォーマット 
};

/**
 * @brief データタイプ
 */
enum XDATA_TYPE {
	XDATA_TYPE_END      = XD2_END,			///< End of Message               
	XDATA_TYPE_L		= XD2_L,			///< List                         
	XDATA_TYPE_NULL		= XD2_NULL,			///< null
	XDATA_TYPE_B		= XD2_B,			///< Binary                       
	XDATA_TYPE_BOOL		= XD2_BOOL,			///< Boolean                      
	XDATA_TYPE_A		= XD2_A,			///< ASCII                       
	XDATA_TYPE_UTF16	= XD2_UTF16,		///< UTF16                        
	XDATA_TYPE_J		= XD2_J,			///< JIS-8                        
	XDATA_TYPE_DATE		= XD2_DATE,			///< Date                
	XDATA_TYPE_TIME 	= XD2_TIME,			///< Time
	XDATA_TYPE_TIMESTAMP= XD2_TIMESTAMP,	///< TimeStamp
	XDATA_TYPE_U1		= XD2_U1,			///< Unsigned Integer 1-Byte      
	XDATA_TYPE_U2		= XD2_U2,			///< Unsigned Integer 2-Byte      
	XDATA_TYPE_U4		= XD2_U4,			///< Unsigned Integer 4-Byte      
	XDATA_TYPE_U8		= XD2_U8,			///< Unsigned Integer 8-Byte      
	XDATA_TYPE_I1		= XD2_I1,			///< Signed Integer 1-Byte       
	XDATA_TYPE_I2		= XD2_I2,			///< Signed Integer 2-Byte        
	XDATA_TYPE_I4		= XD2_I4,			///< Signed Integer 4-Byte        
	XDATA_TYPE_I8		= XD2_I8,			///< Signed Integer 8-Byte        
	XDATA_TYPE_F4		= XD2_F4,			///< Floating Point 32-Bit        
	XDATA_TYPE_F8		= XD2_F8,			///< Floating Point 64-Bit      
	XDATA_TYPE_UX       = XD2_TYP(0,1),		///< Unsigned Integer
	XDATA_TYPE_IX       = XD2_TYP(0,2),		///< Integer
	XDATA_TYPE_FX       = XD2_TYP(0,3),		///< Floating
};

class XData;
class XDataXL;
class XDataCoder;
class XDataReader;
class XDataWriter;
class XDataXLWriter;
class XDataXLReader;
class XDataAttribute;
class XDataAttributeList;

/**
 * @brief 通常フォーマット用データクラス
 */
class XLIB_API XData
{
public:	
	/**	@brief	コンストラクタ	*/
	XData();

	/**	@brief	デストラクタ
	*/
	virtual ~XData();

	/**	@brief	データ配列数を取得します。
	 *	@return	データ配列数
	 */
	virtual XD_UINT4 getCount() const;

	/**	@brief	データの物理サイズを取得します。
	 *	@return	データの物理サイズ
	 */
	virtual XD_UINT4 getSize() const;
	
	/**	@brief	データのサイズのサイズを取得します。
	 *	@retval 1	データのサイズは1バイトで表現可能。(0XFF)
	 *	@retval 2	データのサイズは2バイトで表現可能。(0XFFFF)
	 *	@retval 3	データのサイズは4バイトで表現可能。(0XFFFFFFFF)
	 *	@retval その他	エラーです。
	 */
	virtual XD_UINT1 getLengthByte() const;


	/**	@brief	レングスバイトがじつデータサイズと一致しているか検査します。
	 *
	 *	@retval	true	一致している
	 *	@retval	false	一致していない
	 */
	virtual bool isFitLengthByte() const;
	
	/**	@brief	データのタイプを取得します。
	 *	@return	データタイプ
	 */
	virtual XDATA_TYPE getType() const;

	/**	@brief	発生したエラー情報を取得する
	 *	@return	エラー情報
	 */
	virtual int getError();

	/**	@brief	エラー情報をクリアします	 */
	virtual void resetError();

	
	
	/**	@brief	データをストリームに書き込みます。
	 *	@param	ppValue	参照値を取得するアドレス
	 *	@return	データのサイズ
	 *  @note U4のデータが、データ配列数：２で (10, 500) とセットされている場合は、ストリームにXD_INT4のデータが　４＋４＝８バイト書き込まれます。
	 *       XDATA_TYPE_Lの場合は、ストリームに書き込み出来ません。例外(XException)をスローします。 
	 */
	bool getValue(std::ostream *stream);

	/**	@brief	バイナリーデータとしてデータの参照値を取得する
	 *	@param	ppValue	参照値を取得するアドレス
	 *	@return	データのサイズ
	 */
	size_t  getValAsBinaryRef(XBIN **ppValue);

	/**	@brief	バイナリーデータとしてデータを取得する
	 *	@param	pValue	データを取得するバッファ
	 *	@param	nValueSize	バッファのサイズ
	 */
	void       getValAsBinary(XBIN *pValue, size_t nValueSize);


	/**	@brief	論理値としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	bool       getValAsBool(XD_UINT4 nIndex = 0);


	/**	@brief	ASCII文字列としてデータを取得
	 *
	 *	@param	str	取得す文字のバッファ
	 *	@note	データタイプがXDATA_TYPE_A、XDATA_TYPE_Jの場合のみ取得可能
	 */
	void getValAsStringA(XStrBuff& str);

	/**	@brief	ASCII文字列としてデータを取得
	 *
	 *	@param	str	取得す文字のバッファ
	 *	@note	データタイプがXDATA_TYPE_A、XDATA_TYPE_Jの場合のみ取得可能
	 */
	void getValAsStringJ(XStrBuff& str);

	/**	@brief	ASCII文字列としてデータを取得
	 *
	 *	@param	str	取得す文字のバッファ
	 *	@note	データタイプがXDATA_TYPE_UTF16の場合のみ取得可能
	 */
	void getValAsStringUTF16(XStrBuff& str);

	/**	@brief	ASCII文字列としてデータを取得
	 *	@return	値
	 *	@note	データタイプがXDATA_TYPE_A、XDATA_TYPE_Jの場合のみ取得可能
	 */
	XStrBuff   getValAsStringA();
	/**	@brief	JIS文字列としてデータを取得
	 *	@return	値
	 *	@note	データタイプがXDATA_TYPE_A、XDATA_TYPE_Jの場合のみ取得可能
	 */
	XStrBuff   getValAsStringJ();
	/**	@brief	UTF16文字列としてデータを取得
	 *	@return	値
	 *	@note	データタイプがXDATA_TYPE_UTF16の場合のみ取得可能
	 */
	XStrBuff   getValAsStringUTF16();
	/**	@brief	８ビット整数値（符号有）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_INT1    getValAsInt1(XD_UINT4 nIndex = 0);
	/**	@brief	１６ビット整数値（符号有）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_INT2   getValAsInt2(XD_UINT4 nIndex = 0);
	/**	@brief	３２ビット整数値（符号有）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_INT4   getValAsInt4(XD_UINT4 nIndex = 0);
	/**	@brief	６４ビット整数値（符号有）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT8  getValAsInt8(XD_UINT4 nIndex = 0);
	/**	@brief	８ビット整数値（符号無）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT1   getValAsUInt1(XD_UINT4 nIndex = 0);
	/**	@brief	１６ビット整数値（符号無）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT2  getValAsUInt2(XD_UINT4 nIndex = 0);
	/**	@brief	３２ビット整数値（符号無）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT4  getValAsUInt4(XD_UINT4 nIndex = 0);
	/**	@brief	６４ビット整数値（符号無）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT8  getValAsUInt8(XD_UINT4 nIndex = 0);
	/**	@brief	３２ビット実数値としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_FLOAT4 getValAsFloat4(XD_UINT4 nIndex = 0);
	/**	@brief	６４ビット実数値としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_FLOAT8 getValAsFloat8(XD_UINT4 nIndex = 0);


	/*
	Conversion	|										|	 SECS Data Item
	Type		|	From:								|	To:C-Language Variable
	------------|---------------------------------------|-----------------------------------
	S2_L		|	List (None)							|
	S2_A		|	A									|	unsigned char, char
	S2_J		|	J									|	unsigned char, char
	S2_B		|	B									|	unsigned char, char
	S2_I1		|	I1									|	char
	S2_I2		|	I2									|	short
	S2_I4		|	I4									|	long
	S2_I8		|	I8									|	long long
	S2_U1		|	U1									|	unsigned char
	S2_U2		|	U2									|	unsigned short
	S2_U4		|	U4									|	unsigned long
	S2_U8		|	U8									|	unsigned long long
	S2_F4		|	F4									|	float
	S2_F8		|	F8									|	double
	S2_BOOLEAN	|	Boolean								|	unsigned char, char
	S2_STRING	|	A, B								|	unsigned char, char (zero-term)
	S2_JSTRING	|	A, J, B								|	unsigned char, char (zero-term)
	S2_CHAR		|	I1, I2, I4, I8, U1, U2, U4, U8, B	|	char
	S2_SHORT	|	I1, I2, I4, I8, U1, U2, U4, U8, B	|	short
	S2_LONG		|	I1, I2, I4, I8, U1, U2, U4, U8, B	|	long
	S2_HUGE		|	I1, I2, I4, I8, U1, U2, U4, U8, B	|	long long
	S2_UCHAR	|	I1, I2, I4, I8, U1, U2, U4, U8, B	|	unsigned char
	S2_USHORT	|	I1, I2, I4, I8, U1, U2, U4, U8, B	|	unsigned short
	S2_ULONG	|	I1, I2, I4, I8, U1, U2, U4, U8, B	|	unsigned long
	S2_UHUGE	|	I1, I2, I4, I8, U1, U2, U4, U8, B	|	unsigned long long
	S2_FLOAT	|	F4, F8								|	float
	S2_DOUBLE	|	F4, F8								|	double
	*/

	/**	@brief	ASCII文字列としてデータを取得
	 *
	 *	@param	str	取得す文字のバッファ
	 *	@note	データタイプがXDATA_TYPE_A、XDATA_TYPE_Bの場合のみ取得可能
	 */
	void getCastValAsStringA(XStrBuff& str);

	/**	@brief	ASCII文字列としてデータを取得
	 *
	 *	@param	str	取得す文字のバッファ
	 *	@note	データタイプがXDATA_TYPE_A、XDATA_TYPE_J、XDATA_TYPE_Bの場合のみ取得可能
	 */
	void getCastValAsStringJ(XStrBuff& str);

	/**	@brief	ASCII文字列としてデータを取得
	 *	@return	値
	 *	@note	データタイプがXDATA_TYPE_A、XDATA_TYPE_Jの場合のみ取得可能
	 */
	XStrBuff   getCastValAsStringA();

	/**	@brief	JIS文字列としてデータを取得
	 *	@return	値
	 *	@note	データタイプがXDATA_TYPE_A、XDATA_TYPE_Jの場合のみ取得可能
	 */
	XStrBuff   getCastValAsStringJ();

	/**	@brief	８ビット整数値（符号有）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 *	@note	I1, I2, I4, I8, U1, U2, U4, U8, B	を	XD_INT1キャスト変換を行い取得する。
	 */
	XD_INT1    getCastValAsInt1(XD_UINT4 nIndex = 0);

	/**	@brief	１６ビット整数値（符号有）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_INT2   getCastValAsInt2(XD_UINT4 nIndex = 0);

	/**	@brief	３２ビット整数値（符号有）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_INT4   getCastValAsInt4(XD_UINT4 nIndex = 0);

	/**	@brief	６４ビット整数値（符号有）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT8  getCastValAsInt8(XD_UINT4 nIndex = 0);

	/**	@brief	８ビット整数値（符号無）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT1   getCastValAsUInt1(XD_UINT4 nIndex = 0);

	/**	@brief	１６ビット整数値（符号無）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT2  getCastValAsUInt2(XD_UINT4 nIndex = 0);

	/**	@brief	３２ビット整数値（符号無）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT4  getCastValAsUInt4(XD_UINT4 nIndex = 0);

	/**	@brief	６４ビット整数値（符号無）としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_UINT8  getCastValAsUInt8(XD_UINT4 nIndex = 0);

	/**	@brief	３２ビット実数値としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_FLOAT4 getCastValAsFloat4(XD_UINT4 nIndex = 0);

	/**	@brief	６４ビット実数値としてデータを取得
	 *	@param	nIndex	配列のインデックス
	 *	@return	値
	 */
	XD_FLOAT8 getCastValAsFloat8(XD_UINT4 nIndex = 0);




	/**	@brief	エラーが発生したときExceptionを発生させるかどうかを設定します
	 *	@param	val	true:Exceptionを発生します。/false:発生しません
	 */
	static void setErrorRise(bool val);
	/**	@brief	エラーが発生したときExceptionを発生させるかどうかを取得します
	 *	@retval	true	Exceptionを発生します。
	 *	@retval	false	発生しません
	 */
	static bool getErrorRise();

	/**	@brief	タイプ文字列を値に変換します。
	 *
	 *	@param	str	タイプ文字列
	 *
	 *	@return	変換された値
	 */
	static XDATA_TYPE parseDataType_From_String(const XStrBuff& str);
	/**	@brief	データタイプ文字列を取得します。
	 *
	 *	@param	type	データタイプ
	 *
	 *	@return	データタイプ文字列
	 */
	static const char* getDataTypeStringA_From_TYPE(const XDATA_TYPE type);
	/**	@brief	データタイプ文字列を取得します。
	 *
	 *	@param	type	データタイプ
	 *
	 *	@return	データタイプ文字列
	 */
	static const wchar_t* getDataTypeStringW_From_TYPE(const XDATA_TYPE type);
	/**	@brief	データタイプ文字列を取得します。
	 *
	 *	@param	type	データタイプ
	 *
	 *	@return	データタイプ文字列
	 */
	static const TCHAR* getDataTypeString_From_TYPE(const XDATA_TYPE type);

	/**	@brief	レングスバイトを取得する。
	 *
	 *	@param	type	データタイプ
	 *	@param	count	カウント値
	 *
	 *	@return	レングスバイト
	 */
	static XD_UINT1 getLengthByte(XD_UINT1 type, XD_UINT4 count);
	
	/**	@brief	指定したバイト数から、データカウントを求める。
	 *
	 *	@param	type	データタイプ
	 *	@param	count	byteSize値
	 *
	 *	@return	データカウント
	 */
	static XD_UINT4 getCountByByteSize(XDATA_TYPE type, XD_UINT4 byteSize);
protected:
	/*	@brief	データタイプ */
	XDATA_TYPE	m_nType;
	/*	@brief	データのサイズ */
	size_t		m_nSize;
	/**	@brief	データ */
	XBIN*		m_pValue;
	/*	@brief	データのサイズのサイズ */
	XD_UINT1	m_nSizeOfSize;
	/*	@brief	エラー情報 */
	int				m_error;
	/* 	@brief	エラーが発生したときExceptionを発生させるかどうかの設定 */
	static bool		g_bErrorRise;
friend class XDataReader;
friend class XDataXLWriter;
friend class XDataXLReader;
friend class XDataAttribute;
friend class XDataAttributeList;
};

/**
 * @brief	拡張データフォーマット用データクラス
 */
class XLIB_API XDataXL : public XData
{
public:
	XDataXL();
	virtual ~XDataXL();

	XStrBuff getTagName();


private:
	unsigned char*	m_ptagName;
	int				m_ntagNameSize;
	unsigned char*	m_pAttrList;
	int				m_nAttrListSize;
friend class XDataXLReader;
};


/**
 * @brief データコーダーベースクラス
 */
class XLIB_API XDataCoder
{
public:
	XDataCoder();
	virtual ~XDataCoder();

	static unsigned char biCS2pgCS(XBIN cs);
	static XD_INT2 _hton2(XD_INT2 h);
	static XD_INT4 _hton4(XD_INT4 h);
	static XD_UINT8 _hton8(XD_UINT8 h);
	static XD_INT2 _ntoh2(XD_INT2 h);
	static XD_INT4 _ntoh4(XD_INT4 h);
	static XD_UINT8 _ntoh8(XD_UINT8 h);


	/**	@brief	発生したエラー情報を取得する
	 *	@return	エラー情報
	 */
	virtual int getError();

	/**	@brief	エラー情報をクリアします	 */
	virtual void resetError();
protected:
	/*	@brief	エラー情報 */
	int				m_error;
};


/**
 * @brief データコーダー読込クラス
 */
class XLIB_API XDataReader : public XDataCoder
{
public:
	XDataReader(XBIN *pData, XD_UINT4 nSize);
	virtual ~XDataReader();
	bool getValue(XData *pData);
	bool skipList(XData *pData);
	
	bool print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth = 0, const TCHAR *pLF = _T("\n"));

	XD_INT4 test();
	void getRaw(XBIN** pAdder, int* pSize);
	void getCurrentRaw(XBIN** pAdder, int* pSize);
protected:
	XD_INT4 GetDataSize(XD_UINT1 nSizeOfSize);
	XD_INT4 SetNext();
	bool printInner(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth,  XStrBuff &strblank, const TCHAR *pLF = _T("\n"));
	bool testInner(XData &data);
	XBIN*				m_pDataFirst;
	XBIN*				m_pCurrentData;
	int					m_nDataSize;
	int					m_nCurrentDataSize;
	XDATA_TYPE			m_cCurrentType;
friend class XDataXLReader;
};

/**
 * @brief データコーダー書込クラス
 */
class XLIB_API XDataWriter : public XDataCoder
{
public:
	XDataWriter(std::ostream *stream);
	virtual ~XDataWriter();

	std::ostream* get_stream();

	/**	@brief	ENDを書き込みます。
		@retval	true	書き込み成功
		@retval	false	書き込み失敗
	*/
	virtual bool putValAsEnd();

	/**	@brief	リストを書き込みます。
		@param	nListCout	リスト数
		@retval	true	書き込み成功
		@retval	false	書き込み失敗
	*/
	virtual bool putValAsList(const XD_UINT4 nListCout);
	virtual bool putValAsBool(const bool value);
	virtual bool putValAsBinary(const unsigned char *pValue, int nSize);
	virtual bool putValAsStringA(const char *pValue);
	virtual bool putValAsStringA(const wchar_t *pValue);
	virtual bool putValAsStringW(const wchar_t *pValue);
	virtual bool putValAsStringJ(const char *pValue);
	virtual bool putValAsStringJ(const wchar_t *pValue);
	virtual bool putValAsInt1(const XD_INT1 value);
	virtual bool putValAsInt2(const XD_INT2 value);
	virtual bool putValAsInt4(const XD_INT4 value);
	virtual bool putValAsInt8(const XD_INT8 value);
	virtual bool putValAsUInt1(const XD_UINT1 value);
	virtual bool putValAsUInt2(const XD_UINT2 value);
	virtual bool putValAsUInt4(const XD_UINT4 value);
	virtual bool putValAsUInt8(const XD_UINT8 value);
	virtual bool putValAsFloat4(const XD_FLOAT4 value);
	virtual bool putValAsFloat8(const XD_FLOAT8 value);
	virtual bool putValue(XDATA_TYPE datatype, void *pValue, XD_UINT4 count, XD_INT1 lengthByte = -1);
	virtual bool putCastValue(XDATA_TYPE castType, XDATA_TYPE inputDatatype, void *pValue, XD_UINT4 count, XD_INT1 lengthByte = -1);

	
protected:
	XD_INT4 writeType(unsigned char type, int count, XD_INT1 lengthByte = -1);
	std::ostream*	m_stream;
};

/**
 *	@brief	属性データクラス
 */
class XLIB_API XDataAttribute : public XData, 
                       public XDataWriter
{
private:
	XDataAttribute();
public:	

	virtual ~XDataAttribute();

	/**	@brief	タグ名を取得
	 *	@return	タグ名
	 */
	const TCHAR* getTagName();

	
	virtual bool putValue(XDATA_TYPE datatype, XBIN *pValue, int count);

protected:
	XStrBuff				m_strTagName;
	XInflateStreamBuff		m_buf;
	std::ostream			m_stream;
friend class XDataAttributeList;
friend class XDataXLWriter;
};


DLL_EXTERN template class XLIB_API XArray<XDataAttribute*>;
typedef XLIB_API XArray<XDataAttribute*> XDataAttributes;


/**
 *	@brief	属性データリストクラス
 */
class XLIB_API XDataAttributeList
{
public:
	XDataAttributeList();
	virtual ~XDataAttributeList();

	XDataAttribute* add(XDATA_TYPE type, const TCHAR* pTagName);
	bool remove(const TCHAR* pTagName);
	void clear();
	int size();
	XDataAttribute* at(int nIndex);
protected:
	XDataAttributes			m_attrs;
};




/**
 * @brief データコーダー読込クラス
 */
class XLIB_API XDataXLReader : public XDataCoder
{
public:
	XDataXLReader(XBIN *pData, XD_UINT4 nSize);
	virtual ~XDataXLReader();

	bool getValue(XDataXL *pData);

	bool print(XLIB_STD_OUT_STR_STREAM &stream, XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));
protected:
	bool printInner(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth,  XStrBuff &strblank, const TCHAR *pLF = _T("\n"));
private:
	XDataReader	m_reader;
};


/**
 * @brief データコーダー書込クラス
 */
class XLIB_API XDataXLWriter : public XDataCoder
{
public:
	/**	@brief	コンストラクタ
		@param	stream	ストリームのアドレス
	*/
	XDataXLWriter(std::ostream *stream);
	virtual ~XDataXLWriter();

	/**	@brief	リストの書き込み
	 *	@param	pTagName	タグ名
	 *	@param	nListCout	リスト数
	 *	@param	pAttrList	属性のリストのアドレス。属性がない場合は、NULLを指定します。
	 *	@retval	true	書き込み成功
	 *	@retval	false	書き込み失敗
	 */
	virtual bool putValAsList(const char* pTagName, const int nListCout, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsBool(const char* pTagName, const bool value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsBinary(const char* pTagName, const XBIN *pValue, int nSize, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsStringA(const char* pTagName, const char *pValue, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsStringW(const char* pTagName, const wchar_t *pValue, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsStringJ(const char* pTagName, const char *pValue, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsInt1(const char* pTagName, const XD_INT1 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsInt2(const char* pTagName, const XD_INT2 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsInt4(const char* pTagName, const XD_INT4 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsInt8(const char* pTagName, const XD_INT8 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsUInt1(const char* pTagName, const XD_UINT1 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsUInt2(const char* pTagName, const XD_UINT2 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsUInt4(const char* pTagName, const XD_UINT4 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsUInt8(const char* pTagName, const XD_UINT8 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsFloat4(const char* pTagName, const XD_FLOAT4 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsFloat8(const char* pTagName, const XD_FLOAT8 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValue(const char* pTagName, XDATA_TYPE type, void *pValue, int count, XDataAttributeList *pAttrList = NULL);


	virtual bool putValAsList(const wchar_t* pTagName, const int nListCout, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsBool(const wchar_t* pTagName, const bool value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsBinary(const wchar_t* pTagName, const XBIN *pValue, int nSize, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsStringA(const wchar_t* pTagName, const char *pValue, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsStringW(const wchar_t* pTagName, const wchar_t *pValue, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsStringJ(const wchar_t* pTagName, const char *pValue, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsInt1(const wchar_t* pTagName, const XD_INT1 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsInt2(const wchar_t* pTagName, const XD_INT2 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsInt4(const wchar_t* pTagName, const XD_INT4 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsInt8(const wchar_t* pTagName, const XD_INT8 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsUInt1(const wchar_t* pTagName, const XD_UINT1 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsUInt2(const wchar_t* pTagName, const XD_UINT2 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsUInt4(const wchar_t* pTagName, const XD_UINT4 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsUInt8(const wchar_t* pTagName, const XD_UINT8 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsFloat4(const wchar_t* pTagName, const XD_FLOAT4 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValAsFloat8(const wchar_t* pTagName, const XD_FLOAT8 value, XDataAttributeList *pAttrList = NULL);
	virtual bool putValue(const wchar_t* pTagName, XDATA_TYPE type, void *pValue, int count, XDataAttributeList *pAttrList = NULL);

protected:
	bool putAttributes(XDataWriter &writer, XDataAttributeList *pAttrList);
	XDataWriter	m_writer;
};


#include "xlib/xlib_tail.h"
#endif /* __X_DATA_HEASER_FILE__ */
