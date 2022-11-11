/**
 * @file xdata.h
 * @brief �o�C�i���[�f�[�^�A�N�Z�X�N���X�S
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

 ��TAG+Attribute�Ȃ��̃t�H�[�}�b�g
   �z�X�g����u�Ԃ�SECS���b�Z�[�W�̃f�[�^�ƌ�����������

 1byte�ځ@�^�C�v�{�f�[�^�T�C�Y�̃T�C�Y
  �Q�r�b�g�ŕ\�������
   �@XXXXXXYY
  �̂悤�ɂȂ�B

   XXXXXX=�^�C�v
	XD2_END=       000000  �f�[�^�̏I���
	XD2_L=         000100  ���X�g
	XD2_B=         001000  �o�C�i���[
	XD2_BOOL=      001100  �_���l
	XD2_A=         010000  ASCII����
	XD2_UNI8=      010001  UTF8����
	XD2_UNI16=     010010  UTF16����
	XD2_J=         010100  JIS8����
	XD2_U1=        011000  �����Ȃ�����1Byte
	XD2_U2=        011001  �����Ȃ�����2Byte
	XD2_U4=        011010  �����Ȃ�����4Byte
	XD2_U8=        011011  �����Ȃ�����8Byte
	XD2_I1=        011100  �������萮��1Byte
	XD2_I2=        011101  �������萮��2Byte
	XD2_I4=        011110  �������萮��4Byte
	XD2_I8=        011111  �������萮��8Byte
	XD2_F4=        100010  ����4Byte
	XD2_F8=        100011  ����8Byte
	XD2_STRING=    100100  ASCII����
	XD2_JSTRING=   101000  JIS8����
	XD2_UCHAR=     101100  �����Ȃ�����1Byte
	XD2_USHORT=    101101  �����Ȃ�����2Byte
	XD2_ULONG=     101110  �����Ȃ�����4Byte
	XD2_UHUGE=     101111  �����Ȃ�����8Byte
	XD2_CHAR=      110000  �������萮��1Byte
	XD2_SHORT=     110001  �������萮��2Byte
	XD2_LONG=      110010  �������萮��4Byte
	XD2_HUGE=      110011  �������萮��8Byte
	XD2_FLOAT=     110110  ����4Byte
	XD2_DOUBLE=    110111  ����8Byte
   YY=�f�[�^�T�C�Y�̃T�C�Y
    01=1�o�C�g�ŕ\���ł���f�[�^�T�C�Y
    10=2�o�C�g�ŕ\���ł���f�[�^�T�C�Y
    11=4�o�C�g�ŕ\���ł���f�[�^�T�C�Y
 
   
 2byte�ځ`�@�f�[�^�T�C�Y
�@�@1byte�ڂ�YY=�f�[�^�T�C�Y�̃T�C�Y�ɂ���ăT�C�Y���قȂ�B
	�l�b�g���[�N�o�C�g�I�[�_�[�̌`���̃f�[�^�̃T�C�Y�������Ă���B

 Xbyte�ځ`�@�f�[�^


 ��TAG+Attribute�t�̃t�H�[�}�b�g
 �@<L.X+1
     <A. "�^�O��">
     <L.x
       <L.2
         <A "������">
         <? "�����l">
       >
     >
	 ...
	 ...
   >

 �@<L.3
     <A. "�^�O��">
     <L.x
       <L.2
         <A "������">
         <? "�����l">
       >
     >
     <? "���ۂ̒l">
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
 * @brief �t�H�[�}�b�g�^�C�v
 */
enum XDATA_FORMAT_TYPE {
	XDATA_TYPE_NOMAL,		///< �ʏ�t�H�[�}�b�g�BSECS Like 
	XDATA_TYPE_LIKE_XML,	///< �g���f�[�^�t�H�[�}�b�g�BTAG+Attribute�t�̃t�H�[�}�b�g 
};

/**
 * @brief �f�[�^�^�C�v
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
 * @brief �ʏ�t�H�[�}�b�g�p�f�[�^�N���X
 */
class XLIB_API XData
{
public:	
	/**	@brief	�R���X�g���N�^	*/
	XData();

	/**	@brief	�f�X�g���N�^
	*/
	virtual ~XData();

	/**	@brief	�f�[�^�z�񐔂��擾���܂��B
	 *	@return	�f�[�^�z��
	 */
	virtual XD_UINT4 getCount() const;

	/**	@brief	�f�[�^�̕����T�C�Y���擾���܂��B
	 *	@return	�f�[�^�̕����T�C�Y
	 */
	virtual XD_UINT4 getSize() const;
	
	/**	@brief	�f�[�^�̃T�C�Y�̃T�C�Y���擾���܂��B
	 *	@retval 1	�f�[�^�̃T�C�Y��1�o�C�g�ŕ\���\�B(0XFF)
	 *	@retval 2	�f�[�^�̃T�C�Y��2�o�C�g�ŕ\���\�B(0XFFFF)
	 *	@retval 3	�f�[�^�̃T�C�Y��4�o�C�g�ŕ\���\�B(0XFFFFFFFF)
	 *	@retval ���̑�	�G���[�ł��B
	 */
	virtual XD_UINT1 getLengthByte() const;


	/**	@brief	�����O�X�o�C�g�����f�[�^�T�C�Y�ƈ�v���Ă��邩�������܂��B
	 *
	 *	@retval	true	��v���Ă���
	 *	@retval	false	��v���Ă��Ȃ�
	 */
	virtual bool isFitLengthByte() const;
	
	/**	@brief	�f�[�^�̃^�C�v���擾���܂��B
	 *	@return	�f�[�^�^�C�v
	 */
	virtual XDATA_TYPE getType() const;

	/**	@brief	���������G���[�����擾����
	 *	@return	�G���[���
	 */
	virtual int getError();

	/**	@brief	�G���[�����N���A���܂�	 */
	virtual void resetError();

	
	
	/**	@brief	�f�[�^���X�g���[���ɏ������݂܂��B
	 *	@param	ppValue	�Q�ƒl���擾����A�h���X
	 *	@return	�f�[�^�̃T�C�Y
	 *  @note U4�̃f�[�^���A�f�[�^�z�񐔁F�Q�� (10, 500) �ƃZ�b�g����Ă���ꍇ�́A�X�g���[����XD_INT4�̃f�[�^���@�S�{�S���W�o�C�g�������܂�܂��B
	 *       XDATA_TYPE_L�̏ꍇ�́A�X�g���[���ɏ������ݏo���܂���B��O(XException)���X���[���܂��B 
	 */
	bool getValue(std::ostream *stream);

	/**	@brief	�o�C�i���[�f�[�^�Ƃ��ăf�[�^�̎Q�ƒl���擾����
	 *	@param	ppValue	�Q�ƒl���擾����A�h���X
	 *	@return	�f�[�^�̃T�C�Y
	 */
	size_t  getValAsBinaryRef(XBIN **ppValue);

	/**	@brief	�o�C�i���[�f�[�^�Ƃ��ăf�[�^���擾����
	 *	@param	pValue	�f�[�^���擾����o�b�t�@
	 *	@param	nValueSize	�o�b�t�@�̃T�C�Y
	 */
	void       getValAsBinary(XBIN *pValue, size_t nValueSize);


	/**	@brief	�_���l�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	bool       getValAsBool(XD_UINT4 nIndex = 0);


	/**	@brief	ASCII������Ƃ��ăf�[�^���擾
	 *
	 *	@param	str	�擾�������̃o�b�t�@
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_A�AXDATA_TYPE_J�̏ꍇ�̂ݎ擾�\
	 */
	void getValAsStringA(XStrBuff& str);

	/**	@brief	ASCII������Ƃ��ăf�[�^���擾
	 *
	 *	@param	str	�擾�������̃o�b�t�@
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_A�AXDATA_TYPE_J�̏ꍇ�̂ݎ擾�\
	 */
	void getValAsStringJ(XStrBuff& str);

	/**	@brief	ASCII������Ƃ��ăf�[�^���擾
	 *
	 *	@param	str	�擾�������̃o�b�t�@
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_UTF16�̏ꍇ�̂ݎ擾�\
	 */
	void getValAsStringUTF16(XStrBuff& str);

	/**	@brief	ASCII������Ƃ��ăf�[�^���擾
	 *	@return	�l
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_A�AXDATA_TYPE_J�̏ꍇ�̂ݎ擾�\
	 */
	XStrBuff   getValAsStringA();
	/**	@brief	JIS������Ƃ��ăf�[�^���擾
	 *	@return	�l
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_A�AXDATA_TYPE_J�̏ꍇ�̂ݎ擾�\
	 */
	XStrBuff   getValAsStringJ();
	/**	@brief	UTF16������Ƃ��ăf�[�^���擾
	 *	@return	�l
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_UTF16�̏ꍇ�̂ݎ擾�\
	 */
	XStrBuff   getValAsStringUTF16();
	/**	@brief	�W�r�b�g�����l�i�����L�j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_INT1    getValAsInt1(XD_UINT4 nIndex = 0);
	/**	@brief	�P�U�r�b�g�����l�i�����L�j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_INT2   getValAsInt2(XD_UINT4 nIndex = 0);
	/**	@brief	�R�Q�r�b�g�����l�i�����L�j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_INT4   getValAsInt4(XD_UINT4 nIndex = 0);
	/**	@brief	�U�S�r�b�g�����l�i�����L�j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT8  getValAsInt8(XD_UINT4 nIndex = 0);
	/**	@brief	�W�r�b�g�����l�i�������j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT1   getValAsUInt1(XD_UINT4 nIndex = 0);
	/**	@brief	�P�U�r�b�g�����l�i�������j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT2  getValAsUInt2(XD_UINT4 nIndex = 0);
	/**	@brief	�R�Q�r�b�g�����l�i�������j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT4  getValAsUInt4(XD_UINT4 nIndex = 0);
	/**	@brief	�U�S�r�b�g�����l�i�������j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT8  getValAsUInt8(XD_UINT4 nIndex = 0);
	/**	@brief	�R�Q�r�b�g�����l�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_FLOAT4 getValAsFloat4(XD_UINT4 nIndex = 0);
	/**	@brief	�U�S�r�b�g�����l�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
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

	/**	@brief	ASCII������Ƃ��ăf�[�^���擾
	 *
	 *	@param	str	�擾�������̃o�b�t�@
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_A�AXDATA_TYPE_B�̏ꍇ�̂ݎ擾�\
	 */
	void getCastValAsStringA(XStrBuff& str);

	/**	@brief	ASCII������Ƃ��ăf�[�^���擾
	 *
	 *	@param	str	�擾�������̃o�b�t�@
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_A�AXDATA_TYPE_J�AXDATA_TYPE_B�̏ꍇ�̂ݎ擾�\
	 */
	void getCastValAsStringJ(XStrBuff& str);

	/**	@brief	ASCII������Ƃ��ăf�[�^���擾
	 *	@return	�l
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_A�AXDATA_TYPE_J�̏ꍇ�̂ݎ擾�\
	 */
	XStrBuff   getCastValAsStringA();

	/**	@brief	JIS������Ƃ��ăf�[�^���擾
	 *	@return	�l
	 *	@note	�f�[�^�^�C�v��XDATA_TYPE_A�AXDATA_TYPE_J�̏ꍇ�̂ݎ擾�\
	 */
	XStrBuff   getCastValAsStringJ();

	/**	@brief	�W�r�b�g�����l�i�����L�j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 *	@note	I1, I2, I4, I8, U1, U2, U4, U8, B	��	XD_INT1�L���X�g�ϊ����s���擾����B
	 */
	XD_INT1    getCastValAsInt1(XD_UINT4 nIndex = 0);

	/**	@brief	�P�U�r�b�g�����l�i�����L�j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_INT2   getCastValAsInt2(XD_UINT4 nIndex = 0);

	/**	@brief	�R�Q�r�b�g�����l�i�����L�j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_INT4   getCastValAsInt4(XD_UINT4 nIndex = 0);

	/**	@brief	�U�S�r�b�g�����l�i�����L�j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT8  getCastValAsInt8(XD_UINT4 nIndex = 0);

	/**	@brief	�W�r�b�g�����l�i�������j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT1   getCastValAsUInt1(XD_UINT4 nIndex = 0);

	/**	@brief	�P�U�r�b�g�����l�i�������j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT2  getCastValAsUInt2(XD_UINT4 nIndex = 0);

	/**	@brief	�R�Q�r�b�g�����l�i�������j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT4  getCastValAsUInt4(XD_UINT4 nIndex = 0);

	/**	@brief	�U�S�r�b�g�����l�i�������j�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_UINT8  getCastValAsUInt8(XD_UINT4 nIndex = 0);

	/**	@brief	�R�Q�r�b�g�����l�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_FLOAT4 getCastValAsFloat4(XD_UINT4 nIndex = 0);

	/**	@brief	�U�S�r�b�g�����l�Ƃ��ăf�[�^���擾
	 *	@param	nIndex	�z��̃C���f�b�N�X
	 *	@return	�l
	 */
	XD_FLOAT8 getCastValAsFloat8(XD_UINT4 nIndex = 0);




	/**	@brief	�G���[�����������Ƃ�Exception�𔭐������邩�ǂ�����ݒ肵�܂�
	 *	@param	val	true:Exception�𔭐����܂��B/false:�������܂���
	 */
	static void setErrorRise(bool val);
	/**	@brief	�G���[�����������Ƃ�Exception�𔭐������邩�ǂ������擾���܂�
	 *	@retval	true	Exception�𔭐����܂��B
	 *	@retval	false	�������܂���
	 */
	static bool getErrorRise();

	/**	@brief	�^�C�v�������l�ɕϊ����܂��B
	 *
	 *	@param	str	�^�C�v������
	 *
	 *	@return	�ϊ����ꂽ�l
	 */
	static XDATA_TYPE parseDataType_From_String(const XStrBuff& str);
	/**	@brief	�f�[�^�^�C�v��������擾���܂��B
	 *
	 *	@param	type	�f�[�^�^�C�v
	 *
	 *	@return	�f�[�^�^�C�v������
	 */
	static const char* getDataTypeStringA_From_TYPE(const XDATA_TYPE type);
	/**	@brief	�f�[�^�^�C�v��������擾���܂��B
	 *
	 *	@param	type	�f�[�^�^�C�v
	 *
	 *	@return	�f�[�^�^�C�v������
	 */
	static const wchar_t* getDataTypeStringW_From_TYPE(const XDATA_TYPE type);
	/**	@brief	�f�[�^�^�C�v��������擾���܂��B
	 *
	 *	@param	type	�f�[�^�^�C�v
	 *
	 *	@return	�f�[�^�^�C�v������
	 */
	static const TCHAR* getDataTypeString_From_TYPE(const XDATA_TYPE type);

	/**	@brief	�����O�X�o�C�g���擾����B
	 *
	 *	@param	type	�f�[�^�^�C�v
	 *	@param	count	�J�E���g�l
	 *
	 *	@return	�����O�X�o�C�g
	 */
	static XD_UINT1 getLengthByte(XD_UINT1 type, XD_UINT4 count);
	
	/**	@brief	�w�肵���o�C�g������A�f�[�^�J�E���g�����߂�B
	 *
	 *	@param	type	�f�[�^�^�C�v
	 *	@param	count	byteSize�l
	 *
	 *	@return	�f�[�^�J�E���g
	 */
	static XD_UINT4 getCountByByteSize(XDATA_TYPE type, XD_UINT4 byteSize);
protected:
	/*	@brief	�f�[�^�^�C�v */
	XDATA_TYPE	m_nType;
	/*	@brief	�f�[�^�̃T�C�Y */
	size_t		m_nSize;
	/**	@brief	�f�[�^ */
	XBIN*		m_pValue;
	/*	@brief	�f�[�^�̃T�C�Y�̃T�C�Y */
	XD_UINT1	m_nSizeOfSize;
	/*	@brief	�G���[��� */
	int				m_error;
	/* 	@brief	�G���[�����������Ƃ�Exception�𔭐������邩�ǂ����̐ݒ� */
	static bool		g_bErrorRise;
friend class XDataReader;
friend class XDataXLWriter;
friend class XDataXLReader;
friend class XDataAttribute;
friend class XDataAttributeList;
};

/**
 * @brief	�g���f�[�^�t�H�[�}�b�g�p�f�[�^�N���X
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
 * @brief �f�[�^�R�[�_�[�x�[�X�N���X
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


	/**	@brief	���������G���[�����擾����
	 *	@return	�G���[���
	 */
	virtual int getError();

	/**	@brief	�G���[�����N���A���܂�	 */
	virtual void resetError();
protected:
	/*	@brief	�G���[��� */
	int				m_error;
};


/**
 * @brief �f�[�^�R�[�_�[�Ǎ��N���X
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
 * @brief �f�[�^�R�[�_�[�����N���X
 */
class XLIB_API XDataWriter : public XDataCoder
{
public:
	XDataWriter(std::ostream *stream);
	virtual ~XDataWriter();

	std::ostream* get_stream();

	/**	@brief	END���������݂܂��B
		@retval	true	�������ݐ���
		@retval	false	�������ݎ��s
	*/
	virtual bool putValAsEnd();

	/**	@brief	���X�g���������݂܂��B
		@param	nListCout	���X�g��
		@retval	true	�������ݐ���
		@retval	false	�������ݎ��s
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
 *	@brief	�����f�[�^�N���X
 */
class XLIB_API XDataAttribute : public XData, 
                       public XDataWriter
{
private:
	XDataAttribute();
public:	

	virtual ~XDataAttribute();

	/**	@brief	�^�O�����擾
	 *	@return	�^�O��
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
 *	@brief	�����f�[�^���X�g�N���X
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
 * @brief �f�[�^�R�[�_�[�Ǎ��N���X
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
 * @brief �f�[�^�R�[�_�[�����N���X
 */
class XLIB_API XDataXLWriter : public XDataCoder
{
public:
	/**	@brief	�R���X�g���N�^
		@param	stream	�X�g���[���̃A�h���X
	*/
	XDataXLWriter(std::ostream *stream);
	virtual ~XDataXLWriter();

	/**	@brief	���X�g�̏�������
	 *	@param	pTagName	�^�O��
	 *	@param	nListCout	���X�g��
	 *	@param	pAttrList	�����̃��X�g�̃A�h���X�B�������Ȃ��ꍇ�́ANULL���w�肵�܂��B
	 *	@retval	true	�������ݐ���
	 *	@retval	false	�������ݎ��s
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
