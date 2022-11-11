/**
 * @file xjson_val.h
 * @brief JSON-Value�N���X
 *
 * @author T.Fujiha
 * @date 2012-02-15
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_JSON_VALUE_HEADER_FILE__
#define __X_JSON_VALUE_HEADER_FILE__

#include "xlib/xlib_head.h"
#include "xlib/xeception.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"
#include "xlib/xmap.h"

class XLIB_API XJsonObj;
class XLIB_API XJsonArray;


/**
 * @brief JSON��͎��ɃG���[�񍐂���C���^�[�t�F�[�X
 */
class XLIB_API XJsonValErrorReport
{
public:
	XJsonValErrorReport();
	void setStartJsonStrings(const TCHAR* pStart) { if( m_pStart != NULL) return ; m_pStart = pStart; }
	void getPositionInfo(const TCHAR* pCheckStr, size_t* pCharPos = NULL, size_t *pLineNumber = NULL, size_t *pLineCharPos = NULL);


	virtual void error(const TCHAR* p_msg, size_t nCharPos, size_t nLineNumber, size_t nLinePos) = 0;


private:
	const TCHAR* m_pStart;
};

/**
 * @brief �ʏ�t�H�[�}�b�g�p�f�[�^�N���X
 */
class XLIB_API XJsonVal
{
public:
	/**
	 * @brief	�f�[�^�^�C�v
	 */
	enum TYPE {
		STRING,		///<	������
		INTEGER,	///<	����
		DOUBLE,		///<	����
		OBJECT,		///<	�I�u�W�F�N�g
		ARRAY,		///<	�z��
		BOOLEAN,	///<	�^�U�l
		NULL_VALUE,	///<	NULL
		UNKNOWN		///<	�s��
	};


	/* ��f�[�^*/
	static const XStrBuff	EMPTY_STRING;
	static const int		EMPTY_INT;
	static const double		EMPTY_DOUBLE;
	static const XJsonObj	EMPTY_OBJECT;
	static const XJsonArray	EMPTY_ARRAY;
	static const bool		EMPTY_BOOL;

	/**
	 * @brief �R���X�g���N�^�BNULL�l�ŏ�����
	 */
	XJsonVal();

	/**
	 * @brief �R���X�g���N�^�B������ŏ�����
	 * @param newCString	C-style��������g�p���āA������ŏ�����
	 */
	XJsonVal(const char *newCString);
	
	/**
	 * @brief �R���X�g���N�^�B������ŏ�����
	 * @param newCString	C-style��������g�p���āA������ŏ�����
	 */
	XJsonVal(const wchar_t *newCString);


	/**
	 * @brief �R���X�g���N�^�B�����ŏ�����
	 * @param newInt	���������鐮���̒l
	 */
	XJsonVal(int newInt);

	/**
	 * @brief �R���X�g���N�^�B�����ŏ�����
	 * @param newDouble	��������������̒l
	 */
	XJsonVal(double newDouble);
	
	/**
	 * @brief �R���X�g���N�^�B�I�u�W�F�N�g�ŏ�����
	 * @param newObject	����������I�u�W�F�N�g�̒l
	 */
	XJsonVal(const XJsonObj &newObject);

	/**
	 * @brief �R���X�g���N�^�B�z��ŏ�����
	 * @param XJsonArray	����������z��̒l
	 */
	XJsonVal(const XJsonArray &XJsonArray);

	/**
	 * @brief �R���X�g���N�^�B�^�U�l�ŏ�����
	 * @param newBoolean	����������^�U�l�̒l
	 */
	XJsonVal(bool newBoolean);

	/**
	 * @brief �R���X�g���N�^�B�����Ŏw�肵���R�s�[����XJsonVal�Ɠ������e�ŏ�����
	 * @param src	�R�s�[����XJsonVal
	 */
	XJsonVal(const XJsonVal &src);

	/**
	 * @brief	�f�X�g���N�^
	 */
	virtual ~XJsonVal();



	/**
	 * @brief ����I�y���[�^�B
	 * @param src	�R�s�[����XJsonVal
	 * @return ���g
	 */
	XJsonVal &operator=(const XJsonVal &src);

	/**
	 * @brief ���������I�y���[�^�B
	 * @param src	�R�s�[���̕�����
	 * @return ���g
	 */
	XJsonVal &operator=(const char *src);

	/**
	 * @brief ���������I�y���[�^�B
	 * @param src	�R�s�[���̕�����
	 * @return ���g
	 */
	XJsonVal &operator=(const wchar_t *src);

	/**
	 * @brief ��������I�y���[�^�B
	 * @param src	�R�s�[���̐���
	 * @return ���g
	 */
	XJsonVal &operator=(int src);

	/**
	 * @brief ��������I�y���[�^�B
	 * @param src	�R�s�[���̎���
	 * @return ���g
	 */
	XJsonVal &operator=(double src);

	/**
	 * @brief �I�u�W�F�N�g����I�y���[�^�B
	 * @param src	�R�s�[���̃I�u�W�F�N�g
	 * @return ���g
	 */
	XJsonVal &operator=(const XJsonObj &src);

	/**
	 * @brief �z�����I�y���[�^�B
	 * @param src	�R�s�[���̔z��
	 * @return ���g
	 */
	XJsonVal &operator=(const XJsonArray &src);

	/**
	 * @brief �^�U�l����I�y���[�^�B
	 * @param src	�R�s�[���̐^�U�l
	 * @return ���g
	 */
	XJsonVal &operator=(bool src);

	/**
	 * @brief ��r�i��v�j�I�y���[�^�B
	 * @param src	��r����XJsonVal
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool operator==(const XJsonVal &rhs) const;


	/**
	 * @brief ��r�i�s��v�j�I�y���[�^�B
	 * @param src	��r����XJsonVal
	 * @retval	true	�s��v
	 * @retval	false	��v
	 */
	bool operator!=(const XJsonVal &rhs) const;

	/**
	 * @brief ��r�i����j�I�y���[�^�B
	 * @param src	��r����XJsonVal
	 * @retval	true	��r�I�u�W�F�N�g��菬�����B
	 * @retval	false	��r�I�u�W�F�N�g���傫���B
	 */
	bool operator <(const XJsonVal &rhs) const;

	/**
	 * @brief ��r�i����B�܂ށj�I�y���[�^�B
	 * @param src	��r����XJsonVal
	 * @retval	true	��r�I�u�W�F�N�g��菬�����B�܂��́A�����B
	 * @retval	false	��r�I�u�W�F�N�g���傫���B
	 */
	bool operator <=(const XJsonVal &rhs) const;

	/**
	 * @brief ��r�i���j�I�y���[�^�B
	 * @param src	��r����XJsonVal
	 * @retval	true	��r�I�u�W�F�N�g���傳���B
	 * @retval	false	��r�I�u�W�F�N�g��菬�����B
	 */
	bool operator >(const XJsonVal &rhs) const;

	/**
	 * @brief ��r�i���B�܂ށj�I�y���[�^�B
	 * @param src	��r����XJsonVal
	 * @retval	true	��r�I�u�W�F�N�g���傳���B�܂��́A�����B
	 * @retval	false	��r�I�u�W�F�N�g��菬�����B
	 */
	bool operator >=(const XJsonVal &rhs) const;
	

	/**
	 * @brief �w�肵���v���p�e�B�[���̒l���擾
	 *        �v���p�e�B�[�������݂��Ȃ��ꍇ�́A�쐬����B
	 * @param key	�v���p�e�B�[��
	 * @return	�v���p�e�B�[�l
	 */
	XJsonVal &operator[](const char *key);
	
	
	/**
	 * @brief �w�肵���v���p�e�B�[���̒l���擾
	 *        �v���p�e�B�[�������݂��Ȃ��ꍇ�́A�쐬����B
	 * @param key	�v���p�e�B�[��
	 * @return	�v���p�e�B�[�l
	 */
	XJsonVal &operator[](const wchar_t *key);

	/**
	 * @brief �w�肵���C���f�b�N�X�̒l���擾
	 *        �I�[�o�[�t���[�����C���f�b�N�X���w�肵���ꍇ�́A�z����w�肵���C���f�b�N�X���g�����āA�쐬�����I�u�W�F�N�g��Ԃ��B
	 * @param index	�C���f�b�N�X
	 * @return	�v���p�e�B�[�l
	 */
	XJsonVal &operator[](size_t index);

	/**
	 * @brief �f�[�^�^�C�v���擾�B
	 * @return	�f�[�^�^�C�v
	 */
	TYPE getType() const;


	/**
	 * @brief �����f�[�^���ǂ������`�F�b�N���܂��B
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool isString() const;

	/**
	 * @brief �����f�[�^�ɕϊ��\���ǂ����`�F�b�N���܂��B
	 * @retval	true	�\
	 * @retval	false	�s�\
	 */
	bool isStringable() const;

	/**
	 * @brief �����f�[�^���ǂ������`�F�b�N���܂��B
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool isInteger() const;

	/**
	 * @brief �����f�[�^���ǂ������`�F�b�N���܂��B
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool isDouble() const;

	/**
	 * @brief ���l�i�����E�����j�f�[�^���ǂ������`�F�b�N���܂��B
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool isNumeric() const;

	/**
	 * @brief �I�u�W�F�N�g�f�[�^���ǂ������`�F�b�N���܂��B
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool isObject() const;

	/**
	 * @brief �z��f�[�^���ǂ������`�F�b�N���܂��B
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool isArray() const;

	/**
	 * @brief �^�U�l�f�[�^���ǂ������`�F�b�N���܂��B
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool isBoolean() const;

	/**
	 * @brief NULL�f�[�^���ǂ������`�F�b�N���܂��B
	 * @retval	true	��v
	 * @retval	false	�s��v
	 */
	bool isNull() const;


	/**
	 * @brief �����f�[�^���擾���܂��B
	 * @return	�����f�[�^
	 */
	const XStrBuff &getString() const;

	/**
	 * @brief �����f�[�^���擾���܂��B�����f�[�^�Ŗ����ꍇ�́A�����̒l��Ԃ��܂��B
	 * @param	defaultValue	�����f�[�^�Ŗ����ꍇ�̃f�t�H���g�l�B
	 * @return	�����f�[�^
	 */
	const XStrBuff &tryGetString(const XStrBuff &defaultValue) const;


	/**
	 * @brief ������ɕϊ������l���擾���܂��B�ϊ��\�ȃf�[�^�^�C�v�́A�i�����A�����A�����A�^�U�l�ANULL�j
	 * @return	�����f�[�^
	 */
	const XStrBuff getToString() const;

	/**
	 * @brief �����f�[�^���Z�b�g���܂��B
	 * @param	newString	�Z�b�g����C-Style����
	 */
	void setString(const char* newString);

	/**
	 * @brief �����f�[�^���Z�b�g���܂��B
	 * @param	newString	�Z�b�g����C-Style����
	 */
	void setString(const wchar_t* newString);


	/**
	 * @brief �����f�[�^���擾���܂��B
	 * @return	�����f�[�^
	 */
	int getInteger() const;

	/**
	 * @brief	�����f�[�^���擾���܂��B�����f�[�^�Ŗ����ꍇ�́A�����̒l��Ԃ��܂��B
	 * @param	defaultValue	�����f�[�^�Ŗ����ꍇ�̃f�t�H���g�l�B
	 * @return	�����f�[�^
	 */
	int tryGetInteger(int defaultValue) const;

	/**
	 * @brief �����f�[�^���Z�b�g���܂��B
	 * @param	newInt	�Z�b�g���鐮��
	 */
	void setInteger(int newInt);

	/**
	 * @brief �����f�[�^���擾���܂��B
	 * @return	�����f�[�^
	 */
	double getDouble() const;

	/**
	 * @brief	�����f�[�^���擾���܂��B�����f�[�^�Ŗ����ꍇ�́A�����̒l��Ԃ��܂��B
	 * @param	defaultValue	�����f�[�^�Ŗ����ꍇ�̃f�t�H���g�l�B
	 * @return	�����f�[�^
	 */
	double tryGetDouble(double defaultValue) const;


	/**
	 * @brief ����(16Bye)�f�[�^���擾���܂��B
	 * @return	����(16Bye)�f�[�^
	 */
	float getFloat() const;

	/**
	 * @brief	����(16Bye)�f�[�^���擾���܂��B�����f�[�^�Ŗ����ꍇ�́A�����̒l��Ԃ��܂��B
	 * @param	defaultValue	�����f�[�^�Ŗ����ꍇ�̃f�t�H���g�l�B
	 * @return	����(16Bye)�f�[�^
	 */
	float tryGetFloat(float defaultValue) const;


	/**
	 * @brief �����f�[�^���Z�b�g���܂��B
	 * @param	newDouble	�Z�b�g�������
	 */
	void setDouble(double newDouble);

	/**
	 * @brief �I�u�W�F�N�g�f�[�^���擾���܂��B
	 * @return	�I�u�W�F�N�g�f�[�^
	 */
	const XJsonObj &getObject() const;

	/**
	 * @brief �I�u�W�F�N�g�f�[�^���Z�b�g���܂��B
	 * @param	newObject	�Z�b�g����I�u�W�F�N�g
	 */
	void setObject(const XJsonObj &newObject);

	/**
	 * @brief �z��f�[�^���擾���܂��B
	 * @return	�z��f�[�^
	 */
	const XJsonArray &getArray() const;

	/**
	 * @brief �z��f�[�^���Z�b�g���܂��B
	 * @param	newArray	�Z�b�g����z��
	 */
	void setArray(const XJsonArray &newArray);

	/**
	 * @brief �^�U�l�f�[�^���擾���܂��B
	 * @return	�^�U�l�f�[�^
	 */
	bool getBoolean() const;

	/**
	 * @brief	�^�U�l�f�[�^���擾���܂��B�^�U�l�f�[�^�Ŗ����ꍇ�́A�����̒l��Ԃ��܂��B
	 * @param	defaultValue	�^�U�l�f�[�^�Ŗ����ꍇ�̃f�t�H���g�l�B
	 * @return	�^�U�l�f�[�^
	 */
	bool tryGetBoolean(bool defaultValue) const;

	/**
	 * @brief �^�U�l�f�[�^���Z�b�g���܂��B
	 * @param	newBoolean	�Z�b�g����^�U�l
	 */
	void setBoolean(bool newBoolean);

	/**
	 * @brief NULL���Z�b�g���܂��B
	 */
	void setNull();


	/**
	 * @brief	�����Ŏw�肵��Json���������͂��āA�l���\�z���܂��B
	 * @param	json	Json������
	 * @param	pErrorRepoter	Json�������̓G���[���ɁA�G���[��񍐂���C���^�[�t�F�[�X�BNULL���w�肷��ƕ񍐂���Ȃ��B
	 * @retval	!=NULL	��͂��I������Json�����̈ʒu�B
 	 * @retval	==NULL	��̓G���[�����������B
	 */
	const TCHAR* loadFromString(const TCHAR* json, XJsonValErrorReport* pErrorRepoter = NULL);
	


	/**
	 * @brief	JSON��������X�g���[���ɏ������ށB
	 * @param	stream	�X�g���[��
	 * @param	nDepth	�C���f���g�̐[���B~0���w�肷��ƁA�C���f���g���������܂Ȃ��B
	 * @param	bNonEscape	�������G�X�P�[�v����ꍇ��false�B
	 */
	void print(XLIB_STD_OUT_STR_STREAM &stream, size_t nDepth = (size_t)~0, bool bNonEscape = false) const;

	/**
	 * @brief	�w�肵���������G�X�P�[�v���āA�X�g���[���ɏ������݁B
	 * @param	stream	�X�g���[��
	 * @param	str	����
	 */
	static void printEscapeString(XLIB_STD_OUT_STR_STREAM &stream, const XStrBuff& str);

private:
	union ValueDataPointer {
		XStrBuff		*m_pStringValue;
		int				*m_pIntValue;
		double			*m_pDoubleValue;
		XJsonObj		*m_pObjectValue;
		XJsonArray		*m_pArrayValue;
		bool			*m_pBoolValue;

		ValueDataPointer();
		ValueDataPointer(XStrBuff *newStringValue);
		ValueDataPointer(int *newIntValue);
		ValueDataPointer(double *newDoubleValue);
		ValueDataPointer(XJsonObj *newObjectValue);
		ValueDataPointer(XJsonArray *newArrayValue);
		ValueDataPointer(bool *newBoolValue);
	};

	static bool isHexDigit(TCHAR digit);
	static bool isWhiteSpace(TCHAR whiteSpace);
	static const TCHAR* readString(const TCHAR* p_str, XStrBuff &result, XJsonValErrorReport* pErrorRepoter);
	static const TCHAR* readObject(const TCHAR* p_str, XJsonObj &result, XJsonValErrorReport* pErrorRepoter);
	static const TCHAR* readArray(const TCHAR* p_str, XJsonArray &result, XJsonValErrorReport* pErrorRepoter);
	static const TCHAR* readNumber(const TCHAR* p_str, XJsonVal &result, XJsonValErrorReport* pErrorRepoter);
	static const TCHAR* readToNonWhiteSpace(const TCHAR* p_str,
		                            TCHAR &currentCharacter);
	void clear();


	TYPE					m_type;
	ValueDataPointer		m_data;



public:
};


#endif /*__X_JSON_VALUE_HEADER_FILE__*/