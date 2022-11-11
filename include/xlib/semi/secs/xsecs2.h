/**
 * @file xsecs2.h
 * @brief �����̐���SEMI�K�i�@SECS-II
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_SECS2_HEASER_FILE__
#define __X_SECS2_HEASER_FILE__

#include "xlib/xlib_head.h"

#include "xlib/xeception.h"
#include "xlib/xbuff.h"
#include "xlib/xarray.h"
#include "xlib/xstrbuff.h"
#include "xlib/xdata.h"
#include "xlib/xdata_rich.h"
#include "xlib/xmlhelper.h"
#include "xlib/semi/secs/xsecs.h"


class XLIB_API XSecs2Obj;
class XLIB_API XSecs2Items;
class XLIB_API XSecs2Item;
class XLIB_API XSecs2ItemData;
class XLIB_API XSecs2ItemDataL;
class XLIB_API XSecs2ItemDataD;
class XLIB_API XSecs2ItemFormats;
class XLIB_API XSecs2ItemFormat;
class XLIB_API XSecs2ItemValue;
class XLIB_API XSecs2;
class XLIB_API XSecs2Messages;
class XLIB_API XSecs2Message;
class XLIB_API XSecs2MessageCats;
class XLIB_API XSecs2MessageCat;


/**	@brief	class of operate secs data	*/
class XLIB_API XSecs2Obj
{
public:
	enum TYPE {
			SECS2		///<	SECS
		,	ITEMS		///<	Items
		,	ITEM		///<	Item
		,	DATA		///<	DATA
		,	FORMATS		///<	Formats
		,	FORMAT		///<	Format
		,	VALUE		///<	Value

		,	MESSAGES	///<	Messages
		,	MESSAGE		///<	Message
		,	MSG_CATS	///<	MessagesCats
		,	MSG_CAT		///<	MessagesCat
	};
protected:
	/**	@brief	construction */
	XSecs2Obj(TYPE type, XSecs2Obj* pParent);
	/**	@brief	destruction */
	virtual ~XSecs2Obj();
	
	XSecs2Obj* getParentByType(TYPE type);

public:	
	TYPE getType() const;
protected:
	TYPE		m_type;
	XSecs2Obj*	m_pParent;
};




/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2Items : public XSecs2Obj
{
protected:
	XSecs2Items(XSecs2Obj *pParent);
	virtual ~XSecs2Items();
public:
	void clear();
	XSecs2Item* getHeadItem();
	XSecs2Item* getItemByName(XStrBuff &strName);
private:
	XSecs2Item* add(XmlHlpElementRawType* pElem);
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XList					m_items;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2;
friend class XSecs2Item;
};




/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2ItemData : public XSecs2Obj
{
protected:
	XSecs2ItemData(XmlHlpElementRawType* pElem, XSecs2Obj* pParent);
	virtual ~XSecs2ItemData();
public:
	virtual void clear() = 0;
	bool isDistribute() const;
	bool remove();
// children operation
	XSecs2ItemData* nextSibling();


	virtual bool validate(XRichData* pData, bool bThrow = true);
protected:
	virtual bool innerExpandRealMsg(XRichData* pRData);
	virtual bool innerSetRealMessage(XRichData* pSrcRData, XRichData* pDstRData);
	virtual bool loadXML(XmlHlpElementRawType* pElem) = 0;
protected:
	bool					m_bDistribute;
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2ItemFormat;
friend class XSecs2Message;
friend class XSecs2ItemDataL;
};

/**
 *	@brief	XSecs2ItemDataL
 */
class XLIB_API XSecs2ItemDataL : public XSecs2ItemData
{
protected:
	XSecs2ItemDataL(XmlHlpElementRawType* pElem, XSecs2Obj* pParent);
	virtual ~XSecs2ItemDataL();
public:
	virtual void clear();
	XSecs2ItemData* getHeadItem();


	int  getListCnt() const;
	void setListCnt(int val);
	XStrBuff getCaption() const;
	void setCaption(XStrBuff str);
	XSecs2ItemDataD* addDistribute( const TCHAR* pDistName);
	XSecs2ItemDataL* addList();

	XRichData* addReal(XRichData* pRParent);

	virtual bool validate(XRichData* pData, bool bThrow = true);
protected:
	virtual bool loadXML(XmlHlpElementRawType* pElem);
	virtual bool innerRemove(XSecs2ItemData* pItem);

	XList					m_items;

friend class XSecs2Message;
friend class XSecs2ItemData;
friend class XSecs2ItemFormat;
};

/**
 *	@brief	XSecs2ItemDataD
 */
class XLIB_API XSecs2ItemDataD : public XSecs2ItemData
{
protected:
	XSecs2ItemDataD(XmlHlpElementRawType* pElem, XSecs2Obj* pParent);
	virtual ~XSecs2ItemDataD();
public:
	virtual void clear();
	XStrBuff& getDistName();
	XSecs2Item* getDistItem();

	virtual bool validate(XRichData* pData, bool bThrow = true);
protected:
	virtual bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XStrBuff				m_strDistName;
	XSecs2Item*				m_pDistItem;
friend class XSecs2Message;
friend class XSecs2ItemData;
friend class XSecs2ItemDataL;
};
/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2ItemFormats : public XSecs2Obj
{
protected:
	XSecs2ItemFormats(XSecs2Obj* pParent);
	virtual ~XSecs2ItemFormats();
public:
	void clear();
	XSecs2ItemFormat* add(XmlHlpElementRawType* pElem);
	XSecs2ItemFormat* add(XDATA_TYPE type);
	bool remove(XSecs2ItemFormat* pItem);
	XSecs2ItemFormat* getHeadItem();
private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XList					m_items;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2Item;
friend class XSecs2ItemFormat;
};




/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2ItemFormat : public XSecs2Obj
{
protected:
	XSecs2ItemFormat(XDATA_TYPE type, XSecs2ItemFormats* pParent, XmlHlpElementRawType* pElem );
	virtual ~XSecs2ItemFormat();
public:
	void clear();
	XDATA_TYPE getDataType() const;
	bool isEqulDataType( XDATA_TYPE type );
	bool isSizeMin();
	bool isSizeMax();
	int  getSizeMin();
	int  getSizeMax();
	void setSizeMin(int val);
	void setSizeMax(int val);

	XRichData* getMaxData();
	XRichData* getMinData();


	bool remove();
	XSecs2ItemDataL*	getListItem() const;
	XSecs2ItemData*	getListHeadItem() const;
	XSecs2ItemFormat* nextSibling();
	


	bool removeValue(XSecs2ItemValue* pItem);
	XSecs2ItemValue*	getValueHeadItem() const;
	XSecs2ItemValue* addValue(XRichData* pRData);

private:
	bool loadXML(XmlHlpElementRawType* pElem);
	void innerSetVal(XStrBuff& str, XRichData* pData);
	XSecs2ItemValue* addValue(XmlHlpElementRawType* pElem);
protected:
	XDATA_TYPE				m_type;
	XRichData*				m_pMaxVal;
	XRichData*				m_pMinVal;
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
	XSecs2ItemDataL*		m_pListItem;
	XList					m_items;		// value
friend class XSecs2Items;
friend class XSecs2ItemFormats;
friend class XSecs2ItemData;
friend class XSecs2ItemValue;
};







/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2ItemValue : public XSecs2Obj, public XRichData
{
private:
	XSecs2ItemValue(XDATA_TYPE type, XSecs2ItemFormat* pParent, XmlHlpElementRawType* pElem );
	virtual ~XSecs2ItemValue();
public:
	XStrBuff getComment();
	void setComment(XStrBuff val);
	bool remove();
	XSecs2ItemValue* nextSibling();
	

private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2ItemFormat;
};


/**
 *	@brief	SECS2�ݒ���-���b�Z�[�W�J�e�S���ꗗ�N���X
 */
class XLIB_API XSecs2MessageCats : public XSecs2Obj
{
protected:
	XSecs2MessageCats(XSecs2Obj *pParent);
	virtual ~XSecs2MessageCats();
public:
	/**	@brief	�ꗗ������ɂ��܂��B */
	void clear();
	/**	@brief	SECS2�ݒ���-���b�Z�[�W�J�e�S����V�K�ɒǉ����܂��B
	 *
	 *	@param	strName	���O
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���A���s�����ꍇ��NULL�B
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	XSecs2MessageCat* add(XStrBuff strName);
	/**	@brief	SECS2�ݒ���-���b�Z�[�W�J�e�S�����폜���܂��B
	 *
	 *	@param	pItem	�폜����A�C�e��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool remove(XSecs2MessageCat* pItem);

	/**	@brief	�擪�̃A�C�e�����擾���܂��B
	 *	@return	�擪�̃A�C�e���B��̏ꍇ�́ANULL�B
	 */
	XSecs2MessageCat* getHeadItem();
private:
	XSecs2MessageCat* add(XmlHlpElementRawType* pElem);
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XmlHlpElementRawType*	m_pElem;
	XList					m_items;
friend class XSecs2;
friend class XSecs2Messages;
friend class XSecs2MessageCat;
};




/**
 *	@brief	SECS2�ݒ���-���b�Z�[�W�J�e�S���[�N���X
 */
class XLIB_API XSecs2MessageCat : public XSecs2Obj
{
private:
	XSecs2MessageCat(XSecs2MessageCats* pParent, XmlHlpElementRawType* pElem);
	virtual ~XSecs2MessageCat();
public:
	/**	@brief	�����������擾���܂��B
	 *	@return	��������
	 */
	XStrBuff getName();
	/**	@brief	����������ݒ肵�܂��B
	 *	@param	str	�ݒ�l
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	void setName(XStrBuff str);


	/**	@brief	���̃C���X�^���X��Ԃ��܂��B
	 *	@return	���̃C���X�^���X�B�Ō�̃C���X�^���X�̏ꍇ�́ANULL
	 */
	XSecs2MessageCat* nextSibling();


	/**	@brief	���C���X�^���X���폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool remove();
private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
	XStrBuff				m_strName;
friend class XSecs2MessageCats;
};

/**
 *	@brief	SECS2�ݒ���-���b�Z�[�W�ꗗ�N���X
 */
class XLIB_API XSecs2Messages : public XSecs2Obj
{
protected:
	XSecs2Messages(XSecs2Obj *pParent);
	virtual ~XSecs2Messages();
public:
	/**	@brief	�ꗗ������ɂ��܂��B */
	void clear();
	/**	@brief	SECS2�ݒ���-���b�Z�[�W��V�K�ɒǉ����܂��B
	 *
	 *	@param	stream	�X�g���[��
	 *	@param	function	�t�@���N�V����
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���A���s�����ꍇ��NULL�B
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	XSecs2Message* add(XD_UINT1 stream, XD_UINT1 function);
	/**	@brief	SECS2�ݒ���-���b�Z�[�W���폜���܂��B
	 *
	 *	@param	pItem	�폜����A�C�e��
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool remove(XSecs2Message* pItem);

	/**	@brief	�擪�̃A�C�e�����擾���܂��B
	 *	@return	�擪�̃A�C�e���B��̏ꍇ�́ANULL�B
	 */
	XSecs2Message* getHeadItem();

	/**	@brief	SECS2�ݒ���-���b�Z�[�W�J�e�S���ꗗ�N���X�C���X�^���X���擾
	 *
	 *	@return	SECS2�ݒ���-���b�Z�[�W�J�e�S���ꗗ�N���X�C���X�^���X
	 */
	XSecs2MessageCats& getCategories();


	/**	@brief	The setRealMessage function
	 *
	 *	@param	pSECSData	a parameter of type XSecsData*
	 *	@param	bThrow	a parameter of type bool
	 *
	 *	@return	XSecs2Message*
	 *
	 *	
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	XSecs2Message* setRealMessage(XSecsData* pSECSData, bool bThrow = true);
private:
	XSecs2Message* add(XmlHlpElementRawType* pElem);
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XmlHlpElementRawType*	m_pElem;
	XList					m_items;
	XSecs2MessageCats		m_categories;
friend class XSecs2;
friend class XSecs2Message;
};



/**
 *	@brief	SECS2�ݒ���-���b�Z�[�W�N���X
 */
class XLIB_API XSecs2Message : public XSecs2Obj
{
private:
	XSecs2Message(XSecs2Messages* pParent, XmlHlpElementRawType* pElem);
	virtual ~XSecs2Message();
public:
	enum DIRECTION {
			BOTH		///< ����
		,	EtoH		///< �z�X�g�����u
		,	HtoE		///< ���u���z�X�g
		,	INVALID		///< �s��
	};
	enum MLT_BLK {
			MLT_NO		///< �}���`�u���b�N���b�Z�[�W�s��
		,	MLT_YES		///< �}���`�u���b�N���b�Z�[�W�\
		,	MLT_INQ		///< �}���`�u���b�N���b�Z�[�W�m�F���b�Z�[�W
		,	MLT_GRT		///< �}���`�u���b�N���b�Z�[�W�󂯓��ꋖ���b�Z�[�W
	};


	/**	@brief	�X�g���[���ԍ����擾���܂��B
	 *	@return	�X�g���[���ԍ�
	 */
	XD_UINT1 getStream() const;
	/**	@brief	�t�@���N�V�����ԍ����擾���܂��B
	 *	@return	�t�@���N�V�����ԍ�
	 */
	XD_UINT1 getFunction() const;
	/**	@brief	�����������擾���܂��B
	 *	@return	��������
	 */
	XStrBuff getAction();
	/**	@brief	����������ݒ肵�܂��B
	 *	@param	str	�ݒ�l
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	void setAction(XStrBuff str);

	/**	@brief	�ʐM�̌����̏����擾���܂��B
	 *	@return	�ʐM�̌����̏��
	 */
	DIRECTION getDirection() const;
	/**	@brief	�ʐM�̌����̏���ݒ肵�܂��B
	 *	@param	val	�ݒ�l
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	void setDirection(DIRECTION val);

	/**	@brief	�}���`�u���b�N�̐ݒ�����擾���܂��B
	 *	@return	�}���`�u���b�N�̐ݒ���
	 */
	MLT_BLK getMltCont() const;
	/**	@brief	�}���`�u���b�N�̐ݒ����ݒ肵�܂��B
	 *	@param	val	�ݒ�l
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	void setMltCont(MLT_BLK val);


	/**	@brief	���v���C�̐ݒ�����擾���܂��B
	 *	@return	���v���C�̐ݒ���
	 */
	bool getReply() const;
	/**	@brief	���v���C�̐ݒ����ݒ肵�܂��B
	 *	@param	val	�ݒ�l
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	void setReply(bool val);

	/**	@brief	�{�f�B�[�����擾���܂��B
	 *	@return	�{�f�B�[���
	 */
	XSecs2ItemData* getBody();

	/**	@brief	�{�f�B�f�[�^�Ƀ��X�g�A�C�e����ǉ�
	 *
	 *	@return	�A�C�e��
	 */
	XSecs2ItemDataL* addList() ;
	/**	@brief	�{�f�B�f�[�^�ɃA�C�e����ǉ�
	 *	@param	str	�A�C�e����
	 *	@return	�A�C�e��
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	XSecs2ItemDataD* addDistribute(XStrBuff str) ;

	/**	@brief	�{�f�B�[���폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	bool removeBody() ;


	/**	@brief	���̃C���X�^���X��Ԃ��܂��B
	 *	@return	���̃C���X�^���X�B�Ō�̃C���X�^���X�̏ꍇ�́ANULL
	 */
	XSecs2Message* nextSibling();


	/**	@brief	���C���X�^���X���폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g	
	 */
	bool remove();


	/**	@brief	�J�e�S���[���擾
	 *
	 *	@return	�J�e�S���[�B�w�肳��Ă��Ȃ��ꍇ�́ANULL
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	XSecs2MessageCat* getCategory();

	/**	@brief	�J�e�S���[���Z�b�g����
	 *
	 *	@param	val	�J�e�S���[��
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	void setCategory(XSecs2MessageCat* val);


	XRichData* newRealMessage();
	XRichData* getRealMessage();
	/**	@brief	The setRealMessage function
	 *
	 *	@param	pSECSData	a parameter of type XSecsData*
	 *	@param	bThrow	a parameter of type bool
	 *
	 *	@retval	-1	Stream����v���Ȃ�
	 *	@retval	-2	Function����v���Ȃ�
	 *
	 *	
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	int setRealMessage(XSecsData* pSECSData, bool bThrow = true);
	bool validateRealMessage(bool bThrow = true);

private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XPOSITION				m_pos;
	XmlHlpElementRawType*	m_pElem;
	XD_UINT2				m_sf;
	XSecs2ItemData*			m_pBody;
	XSecs2MessageCat*		m_pCategory;
	XStrBuff				m_strCategoryName;
	XSecsData				m_msg;
friend class XSecs2Messages;
};




/**
 *	@brief	The XLIB_API class
 */
class XLIB_API XSecs2Item : public XSecs2Obj
{
private:
	XSecs2Item(const TCHAR* pName, XDATA_TYPE type, XSecs2Items* pParent, XmlHlpElementRawType* pElem);
	virtual ~XSecs2Item();
public:
	
	XStrBuff& getName(){  return m_strName; }
	XDATA_TYPE getDataType(){ return m_type; }
	XStrBuff getComment();
	void setComment(XStrBuff val);

	bool getIsFixVals() const;
	void setIsFixVals(bool val);


	bool getIsMultiFmt() const;
	void setIsMultiFmt(bool val);


	XSecs2ItemFormat* getCurrentFormat();
	XSecs2ItemFormats& getFormats();

	XSecs2Item* nextSibling();



private:
	bool loadXML(XmlHlpElementRawType* pElem);
protected:
	XStrBuff				m_strName;
	XDATA_TYPE				m_type;
	XPOSITION				m_pos;
	XSecs2ItemFormats		m_formats;
	XmlHlpElementRawType*	m_pElem;
friend class XSecs2Items;
};







/**	@brief	SECS-II��`�N���X	*/
class XLIB_API XSecs2 : public XSecs2Obj
{
public:
	/**	@brief	construction */
	XSecs2();

	/**	@brief	destruction */
	virtual ~XSecs2();

	/**	@brief	������ɂ��܂��B */
	virtual void clear();

	/**	@brief	XML�t�@�C������������[�h����B
	 *
	 *	@param	p_file_path	XML�t�@�C���̃p�X
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	virtual bool loadXML(const TCHAR* p_file_path);
	/**	@brief	XML�t�@�C���ɕۑ�
	 *
	 *	@param	p_file_path	XML�t�@�C���̃p�X
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 *
	 *	@exception	XExcept	��O�I�u�W�F�N�g
	 */
	virtual bool saveXML(const TCHAR* p_file_path);

	/**	@brief	�A�C�e���ꗗ�I�u�W�F�N�g���擾
	 *	@return	�A�C�e���ꗗ�I�u�W�F�N�g
	 */
	XSecs2Items& getItems();
	/**	@brief	���b�Z�[�W�ꗗ�I�u�W�F�N�g���擾
	 *	@return	���b�Z�[�W�ꗗ�I�u�W�F�N�g
	 */
	XSecs2Messages& getMessages();
protected:
	XSecs2Items						m_items;
	XSecs2Messages					m_messages;
	XmlHlpDocumentActl				m_xmlDoc;
};


#include "xlib/xlib_tail.h"
#endif /* __X_SECS2_HEASER_FILE__ */
