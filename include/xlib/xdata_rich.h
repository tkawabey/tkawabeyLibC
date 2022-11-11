/**
 * @file xdata_rich.h
 * @brief �o�C�i���[�f�[�^�A�N�Z�X�N���X�S
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DATA_RICH_HEASER_FILE__
#define __X_DATA_RICH_HEASER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xeception.h"
#include <vector>


class XLIB_API XRichDataBase;
class XLIB_API XRichData;
class XLIB_API XRichDataXL;



DLL_EXTERN template class XLIB_API std::vector<XRichDataBase*>;
typedef XLIB_API std::vector<XRichDataBase*> XRichDataBases;

DLL_EXTERN template class XLIB_API std::vector<XRichData*>;
typedef XLIB_API std::vector<XRichData*> XRichDatas;

DLL_EXTERN template class XLIB_API std::vector<XRichDataXL*>;
typedef XLIB_API std::vector<XRichDataXL*> XRichDataXLs;


/**
 * @brief �f�[�^�A�N�Z�X�N���X�B�ǉ��A�폜�A�ҏW���\�B
 *         XDataReader��XDataWriter�ɔ�ׂāA�������[�A���P�[�g����������̂ŁA�p�t�H�[�}���X�͗��B
 */
class XLIB_API XRichDataBase : public XDataCoder 
{
public:
	/**	@brief	�R���X�g���N�^
	 *	@param	format	�f�[�^�t�H�[�}�b�g
	 */
	XRichDataBase(XRichDataBase* pParent);
public:	
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XRichDataBase();
	
	/**	@brief	�f�[�^�^�C�v���擾���܂�
	 *	@return	�f�[�^�^�C�v
	 */
	virtual XDATA_TYPE getType() const;
	/**	@brief	�f�[�^�^�C�v�𕶎���Ŏ擾���܂�
	 *	@return	�f�[�^�^�C�v�̕�����
	 */
	virtual const char* getTypeStringA() const;

	/**	@brief	�w�肵���q���X�g�A�C�e�����폜���܂��B
	 *	@param	pItem	�폜����q���X�g�A�C�e��
	 *	@return	true:����/false:���s
	 */
	virtual bool removeChiled(XRichDataBase* pItem);

	/**	@brief	�q���X�g�ɃA�C�e�����Z�b�g���܂��B
	 *
	 *	@param	pItem	�Z�b�g����A�C�e��
	 *	@param	pPreItem	���̃A�C�e���Ŏw�肵���ʒu�̌�ɒǉ����܂��B�Ō�ɃA�C�e����ǉ�����ꍇ�́ANULL���w�肵�܂��B
	 *
	 *	@return	true:����/false:���s
	 */
	virtual bool atachChild(XRichDataBase* pItem, XRichDataBase* pPreItem = NULL );
	/**	@brief	�q���X�g����A�C�e�����������܂��B�q�A�C�e���̃������͍폜���܂���B
	 *
	 *	@param	pItem	���X�g�����������A�C�e��
	 *
	 *	@return	true:����/false:���s	
	 */
	virtual bool detachChild(XRichDataBase* pItem);

	/**	@brief	�w�肵���q���X�g�A�C�e�������擾���܂��B
	 *	@return	�q���X�g�A�C�e�����B
	 */
	virtual XD_UINT4 getChiledCount() const;

	/**	@brief	�e�A�C�e���̃C���X�^���X�A�h���X��Ԃ��܂��B
	 *	@return	�e�A�C�e���̃C���X�^���X�A�h���X�B�e�������ꍇ��NULL�B
	 */
	virtual XRichDataBase* getParent() const;

	/**	@brief	�e�A�C�e�����猩���A���̃C���X�^���X�̔z��̈ʒu��Ԃ��܂��B
	 *
	 *	@return	�e�A�C�e�����猩���A���̃C���X�^���X�̔z��̈ʒu�B�e�������ꍇ��-1�B
	 */
	virtual int getIndexFromParent() const;

	virtual void freeValue();

	virtual void clear();

	XStrBuff toString();

	virtual int compare(XRichDataBase* pDst);

	virtual bool isDirty() const;
	virtual void resetDirty();

	/**	@brief	�����O�X�o�C�g���擾���܂��B
	 *
	 *	@retval	-1	�f�[�^�T�C�Y�Ɉ�v�����邱�Ƃ��Ӗ����܂��B
	 *	@retval	1	1�o�C�g
	 *	@retval	2	2�o�C�g
	 *	@retval	3	3�o�C�g
	 */
	virtual XD_INT1 getLengthByte();
	/**	@brief	�����O�X�o�C�g��ݒ肵�܂��B
	 *
	 *	@param	val	�����O�X�o�C�g�B�w��ł���l�́A-1,1,2,3�̂����ꂩ
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	virtual void    setLengthByte(XD_INT1 val) ;

	/**	@brief	���[�U�[�f�[�^���擾���܂��B
	 *
	 *	@return	���[�U�[�f�[�^
	 */
	virtual void*   getUserData() const;
	/**	@brief	���[�U�[�f�[�^��ݒ肵�܂��B
	 *
	 *	@param	val	���[�U�[�f�[�^
	 */
	virtual void    setUserData(void* val);
	
	virtual XD_UINT4 getDataCount() const;
	virtual XD_UINT4 getBinarySize() const;
	virtual bool assign(XD_UINT4 nValueCount) ;
	virtual bool putValueString(const char* pValue, int len = -1);
	virtual bool putValueString(const wchar_t* pValue, int len = -1);
	virtual bool putValueStringUTF16(const wchar_t* pValue, int len = -1);
	virtual bool putValue(const bool* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_INT1* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_UINT1* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_INT2* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_UINT2* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_INT4* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_UINT4* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_FLOAT4* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_INT8* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_UINT8* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const XD_FLOAT8* pValue, XD_UINT4 nValueCount, bool bHostOrder = true);
	virtual bool putValue(const bool value);
	virtual bool putValue(const XD_INT1 value);
	virtual bool putValue(const XD_UINT1 value);
	virtual bool putValue(const XD_INT2 value);
	virtual bool putValue(const XD_UINT2 value);
	virtual bool putValue(const XD_INT4 value);
	virtual bool putValue(const XD_UINT4 value);
	virtual bool putValue(const XD_FLOAT4 value);
	virtual bool putValue(const XD_INT8 value);
	virtual bool putValue(const XD_UINT8 value);
	virtual bool putValue(const XD_FLOAT8 value);


	virtual bool setValue(XD_UINT4 index, const bool value) ;
	virtual bool setValue(XD_UINT4 index, const XD_INT1 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_UINT1 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_INT2 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_UINT2 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_INT4 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_UINT4 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_FLOAT4 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_INT8 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_UINT8 value) ;
	virtual bool setValue(XD_UINT4 index, const XD_FLOAT8 value) ;



	virtual void getValue(std::ostream *stream) const;
	virtual void getValue(XD_INT1 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_UINT1 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_INT2 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_UINT2 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_INT4 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_UINT4 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_FLOAT4 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_INT8 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_UINT8 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValue(XD_FLOAT8 **pValue, XD_UINT4 &nValueCount) const;
	virtual void getValueBool(bool **pValue, XD_UINT4 &nValueCount) const;
	virtual const char* getValueString(int *err = NULL) const;
	virtual const unsigned char getValueBinary(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const bool getValueBoolean(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_INT1 getValueInt1(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_UINT1 getValueUInt1(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_INT2 getValueInt2(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_UINT2 getValueUInt2(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_INT4 getValueInt4(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_UINT4 getValueUInt4(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_FLOAT4 getValueFloat4(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_INT8 getValueInt8(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_UINT8 getValueUInt8(XD_UINT4 index = 0, int *err = NULL) const;
	virtual const XD_FLOAT8 getValueFloat8(XD_UINT4 index = 0, int *err = NULL) const;

	/**	@brief	�f�[�^�^�C�v��ύX����B
	 *
	 *	@param	type	�f�[�^�^�C�v
	 *
	 *	@return	true:����/false:���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	virtual bool changeType(XDATA_TYPE type) ;


	virtual bool buildBinaryData(std::ostream *stream) = 0;
	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream,  XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n")) = 0;
protected:
	virtual bool innerPutValue(const unsigned char* pValue, XD_UINT4 nValueSize, bool bHostOrder = true);
protected:
	XDATA_TYPE						m_type;
	XBIN*							m_pData;
	XD_UINT4						m_nDataSize;
	XRichDataBase*					m_pParent;
	std::vector<XRichDataBase*>		m_children;
	bool							m_bDirty;
	XD_INT1							m_nLengthByte;
	void*							m_pUserData;
};


/**
 * @brief	�f�[�^�A�N�Z�X�N���X�B�ǉ��A�폜�A�ҏW���\�B
 *			XDataReader��XDataWriter�ɔ�ׂāA�������[�A���P�[�g����������̂ŁA�p�t�H�[�}���X�͗��B
 */
class XLIB_API XRichData : public XRichDataBase
{
public:
	/**	@brief	�R���X�g���N�^
	 *	@param	format	�f�[�^�t�H�[�}�b�g
	 */
	XRichData(XDATA_TYPE type);
	XRichData(XRichData &srcdata);
	
	/**	@brief	�f�X�g���N�^	*/
	virtual ~XRichData();

	/**	@brief	XData����A�l���Z�b�g����B
	 *
	 *	@param	reader	���[�_�[�I�u�W�F�N�g
	 *	@param	value	XData�I�u�W�F�N�g
	 *	@param	bConvert	XData�̌^�ƁA���݂̌^���Ⴄ�ꍇ�́A�ϊ����邩�ǂ����̃t���O�B
	 *						true  : �ϊ�����B
	 *						false : �ϊ����Ȃ��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s	
	 */
	virtual bool putValueFromXData(XDataReader &reader, XData& value, bool bConvert = false);

	/**	@brief	���X�g�f�[�^�̎q�f�[�^��ǉ����܂��B
	 *	@param	type	�ǉ�����f�[�^�^�C�v
	 *	@param	pPrePos	�w�肵���q�f�[�^�A�C�e���̑O�ɒǉ����܂��B���X�g�̍Ō�ɒǉ�����ꍇ��NULL���w�肵�܂��B
	 *	@return	�V�K�쐬���ꂽ�f�[�^�A�C�e���B�V�K�쐬�Ɏ��s�����ꍇ��NULL�B
	 */
	virtual XRichData* addChiled(XDATA_TYPE type, XRichData* pPrePos = NULL);
	
	/**	@brief	�w�肵���C���f�b�N�X�̎q�A�C�e�����擾���܂��B
	 *	@param	index	�擾����q�A�C�e���̃��X�g�̃C���f�b�N�X�ʒu�B
	 *	@return	�q�A�C�e���B�C���f�b�N�X���s���Ώꍇ��NULL��Ԃ��܂��B�A
	 */
	virtual XRichData* getChiled(XD_UINT4 index);

	/**	@brief	�w�肵���f�[�^���R�s�[���܂��B
	 *
	 *	@param	pSrcData	�R�s�[���̃f�[�^
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	virtual bool copy(XRichData* pSrcData);

	/**	@brief	���̃C���X�^���X�̕������쐬���܂��B
	 *	@return	�쐬���ꂽ�C���X�^���X
	 */
	virtual XRichData* clone();



	XRichData* find(const TCHAR* lpszFindpath, ...);

	virtual bool buildBinaryData(std::ostream *stream);

	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream,  XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));


	 bool operator==(XRichData& pDst);
	 bool operator!=(XRichData& pDst);
	 const XRichData& operator=(XRichData& pSrcData);
protected:
	/*	@brief	�R���X�g���N�^
	 *	@param	pParentt	�e�f�[�^�B�f�[�^���X�g�^�C�v�B
	 */
	XRichData(XRichData* pParentt);
	virtual bool clone(XRichData *pTarget);
	virtual XRichData* find(const TCHAR* lpszFindpath, va_list argList);
	virtual bool buildBinaryData(XDataWriter &writer);
	virtual bool innerPutValueFromXData(XDataReader &reader, XData& value, bool bConvert);
private:
//	XRichDatas			m_list;
};




/**
 * @brief �f�[�^�A�N�Z�X�N���X�B�ǉ��A�폜�A�ҏW���\�B
 *         XDataReader��XDataWriter�ɔ�ׂāA�������[�A���P�[�g����������̂ŁA�p�t�H�[�}���X�͗��B
 */
class XLIB_API XRichDataXL : public XRichDataBase
{
public:
	XRichDataXL();
	virtual ~XRichDataXL();

	/**	@brief	�^�O�����擾���܂��B
	 *	@return	�^�O��
	 */
	virtual const TCHAR* getTagName();
	/**	@brief	�^�O�����Z�b�g���܂��B
	 *	@param	pTagName	�^�O��
	 */
	virtual void setTagName(const TCHAR* pTagName);
	/**	@brief	���X�g�f�[�^�̎q�f�[�^��ǉ����܂��B
	 *	@param	type	�ǉ�����f�[�^�^�C�v
	 *	@param	pTagName	�^�O��
	 *	@param	pPrePos	�w�肵���q�f�[�^�A�C�e���̑O�ɒǉ����܂��B���X�g�̍Ō�ɒǉ�����ꍇ��NULL���w�肵�܂��B
	 *	@return	�V�K�쐬���ꂽ�f�[�^�A�C�e���B�V�K�쐬�Ɏ��s�����ꍇ��NULL�B
	 */
	virtual XRichDataXL* addChiled(XDATA_TYPE type, const TCHAR* pTagName, XRichDataXL* pPrePos = NULL);

	/**	@brief	�w�肵���C���f�b�N�X�̎q�A�C�e�����擾���܂��B
	 *	@param	index	�擾����q�A�C�e���̃��X�g�̃C���f�b�N�X�ʒu�B
	 *	@return	�q�A�C�e���B�C���f�b�N�X���s���Ώꍇ��NULL��Ԃ��܂��B�A
	 */
	virtual XRichDataXL* getChiled(XD_UINT4 index);

	virtual bool copy(XRichDataXL* pSrcData);
	virtual XRichDataXL* clone();


	virtual bool buildBinaryData(std::ostream *stream);

	virtual bool print(XLIB_STD_OUT_STR_STREAM &stream,  XD_INT4 nDepth = 0, const TCHAR *pLF = _T("\n"));

protected:
	/*	@brief	�R���X�g���N�^
	 *	@param	pParentt	�e�f�[�^�B�f�[�^���X�g�^�C�v�B
	 */
	XRichDataXL(XRichDataXL* pParentt);
	virtual bool clone(XRichDataXL *pTarget);
	virtual bool buildBinaryData(XDataXLWriter &writer);
private:
	XRichDataXLs					m_list;
	XStrBuff						m_strTagName;
};


#include "xlib/xlib_tail.h"
#endif /* __X_DATA_RICH_HEASER_FILE__ */
