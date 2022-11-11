/**
 * @file xWinNTScurity.h
 * @brief NT�Z�L�����e�B�N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_WIN_NT_SECURITY_HEASER_FILE__
#define __X_WIN_NT_SECURITY_HEASER_FILE__

#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif
  #include <accctrl.h>
  #include <aclapi.h>
  #include "xlib/xarray.h"
  #include "xlib/xstrbuff.h"


#ifndef TRUSTEE_IS_OBJECTS_AND_SID
		#define  TRUSTEE_IS_OBJECTS_AND_SID 3
#endif //TRUSTEE_IS_OBJECTS_AND_SID

#ifndef __ACCESS_CONTROL__
		typedef struct _OBJECTS_AND_SID {
		  DWORD   ObjectsPresent;
		  GUID    ObjectTypeGuid;
		  GUID    InheritedObjectTypeGuid;
		  SID    *pSid;
		} OBJECTS_AND_SID, *POBJECTS_AND_SID;
#endif //OBJECTS_AND_SID

#ifndef TRUSTEE_IS_OBJECTS_AND_NAME
		#define TRUSTEE_IS_OBJECTS_AND_NAME				4
#endif //TRUSTEE_IS_OBJECTS_AND_NAME

#ifndef __ACCESS_CONTROL__
	typedef struct _OBJECTS_AND_NAME {
	  DWORD          ObjectsPresent;
	  SE_OBJECT_TYPE ObjectType;
	  LPTSTR   ObjectTypeName;
	  LPTSTR   InheritedObjectTypeName;
	  LPTSTR   ptstrName;
	} OBJECTS_AND_NAME, *POBJECTS_AND_NAME;
#endif //OBJECTS_AND_NAME



/**
 * �Z�L�����e�B�A�J�E���g���Ǘ��N���X
 */
class XNtSecAccount
{
public:
	/** �R���X�g���N�^ */
	XNtSecAccount();
	/** �f�X�g���N�^ */
	virtual ~XNtSecAccount();
// Atribute
	/**
	 * �A�J�E���g���A�V�X�e���ɑ��݂��邩���ׂ܂�
	 * @retval true ���݂���
	 * @retval false ���݂��Ȃ�
	 */
	bool IsMapped();
	/**
	 * SID���擾���܂�
	 * @return SID�̃A�h���X
	 */
	SID *GetSID();
	/**
	 * �A�J�E���g�̃^�C�v���擾���܂��B
	 * @note  �ڂ����́AMSDN:TRUSTEE_TYPE���Q��
	 * @return �A�J�E���g�̃^�C�v
	 */
	TRUSTEE_TYPE TrusteeType();
	/**
	 * �A�J�E���g�̃g���X�e�B�����擾���܂�
	 * @note  �ȉ��̂����ꂩ
	 *  {DOMAIN}\{ACCOUNT NAME}
	 *  {ACCOUNT NAME}
	 *  {SID STRING}
	 * @return �g���X�e�B��
	 */
	XStrBuff TrusteeName();
	/**
	 * �h���C�������擾���܂��B
	 * @return �h���C����
	 */
	LPCTSTR DomainName();
	/**
	 * �A�J�E���g�����擾���܂��B
	 * @return �A�J�E���g��
	 */
	LPCTSTR AccountName();
	/**
	 * SID�����ł킩�镶����Ŏ擾���܂��B
	 * @return SID������
	 */
	XStrBuff GetSIDString();
// Operation
	/**
	 * �A�J�E���g�����Z�b�g���܂�
	 * @param[in] lpszAccountName �A�J�E���g���̕�����
	 * @param[in] lpszServerOrDoamin �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @return SID������
	 */
	bool SetAccount(LPCTSTR lpszAccountName, LPCTSTR lpszServerOrDoamin = NULL);
	/**
	 * �A�J�E���g�����Z�b�g���܂�
	 * @param[in] pSID �Z�b�g����A�J�E���g��SID�̃A�h���X
	 * @param[in] lpszServerOrDoamin �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @return SID������
	 */
	bool SetAccount(SID *pSID, LPCTSTR lpszServerOrDoamin = NULL);
protected:
	/** �A�J�E���g�� */
	XStrBuff			m_strAccount;
	/** �h���C�� */
	XStrBuff			m_strDomain;
	/** SID�p�̃o�b�t�@ */
	XBuff				m_SID;
	/** �A�J�E���g�������������ǂ��� */
	bool				m_bMapped;
	/** �A�J�E���g�̃^�C�v */
	TRUSTEE_TYPE        m_nTrusteeType;
};

/**
 * �A�N�Z�X�R���g���[���G���g��(ACE)�̊Ǘ��N���X
 */
class XNtSecAce : public XNtSecAccount
{
public:
	/** �R���X�g���N�^ */
	XNtSecAce();
	/** �f�X�g���N�^ */
	virtual ~XNtSecAce();
// Atribute
	/**
	 * ����ACE���e����p�����ꂽACE�Ŏ��Ԃ̂Ȃ�ACE���ǂ������ׂ܂�
	 * @retval true �e����p�����ꂽACE
	 * @retval false ����
	 */
	bool IsIsInherited();
	/**
	 * �p���t���O���擾���܂�
	 * @return �p���t���O
	 */
	long get_Inheritance();
	/**
	 * �p���t���O���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_Inheritance(long newVal);
	/**
	 * �A�N�Z�X�}�X�N���擾���܂�
	 * @return �A�N�Z�X�}�X�N
	 */
	long get_AccessMask();
	/**
	 * �A�N�Z�X�}�X�N���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_AccessMask(long newVal);
	/**
	 * �A�A�N�Z�X���[�h���擾���܂�
	 * @return �A�A�N�Z�X���[�h
	 */
	ACCESS_MODE get_AccessMode();
	/**
	 * �A�A�N�Z�X���[�h���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_AccessMode(ACCESS_MODE newVal);
// GeneralCtrl
public:
	/**
	 * �A�N�Z�X�}�X�N���A�ǂݍ��݃A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskRead();
	/**
	 * �A�N�Z�X�}�X�N�ɁA�ǂݍ��݃A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskRead(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A�������݃A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskGenericWrite();
	/**
	 * �A�N�Z�X�}�X�N�ɁA�������݃A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskGenericWrite(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A���s�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskGenericExecute();
	/**
	 * �A�N�Z�X�}�X�N�ɁA���s�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskGenericExecute(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A�t���A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskGenericAll();
	/**
	 * �A�N�Z�X�}�X�N�ɁA�t���A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskGenericAll(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���ASynchronize�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskSynchronize();
	/**
	 * �A�N�Z�X�}�X�N�ɁASynchronize�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskSynchronize(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A�I�[�i�[��񏑂����݃A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskWriteOwner();
	/**
	 * �A�N�Z�X�}�X�N�ɁA�I�[�i�[��񏑂����݃A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskWriteOwner(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A���ӃA�N�Z�X���䃊�X�g�iDACL�j�������݃A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskWriteDACL();
	/**
	 * �A�N�Z�X�}�X�N�ɁA���ӃA�N�Z�X���䃊�X�g�iDACL�j�������݃A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskWriteDACL(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A�A�N�Z�X�R���g���[���̓ǂݍ��݃A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskReadCtrl();
	/**
	 * �A�N�Z�X�}�X�N�ɁA�A�N�Z�X�R���g���[���̓ǂݍ��݃A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskReadCtrl(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A�폜�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskDelete();
	/**
	 * �A�N�Z�X�}�X�N�ɁA�폜�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����ꍇ��true�B�A���Z�b�g����ꍇ��false
	 */
	void put_rawAccessMaskDelete(bool newVal);
private:
	/*
	 * 
	 * @param[in] nFlag 
	 * @param[in] pObjectType 
	 * @param[in] pInheritedObjectType 
	 * @return 
	 */
	bool _SetACEObjectInfo(long nFlag, GUID *pObjectType, GUID *pInheritedObjectType);
protected:
	/*
	 * �A�N�Z�X�}�X�N���A�w�肵���I�t�Z�b�g�̃A�N�Z�X�������L���Ă��邩���ׂ܂�
	 * @param[in] nOR �A�N�Z�X�}�X�N�̃I�t�Z�b�g
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_rawAccessMaskOr(long nOR);
	/*
	 * �A�N�Z�X�}�X�N���A�w�肵���I�t�Z�b�g�̃A�N�Z�X�����Z�b�g�A�܂��̓A���Z�b�g���܂�
	 * @param[in] nOR 
	 * @param[in] b 
	 */
	void put_rawAccessMaskOr(long nOR, bool b);
public:
	friend class XNtSecAcl;
protected:
	/* �A�N�Z�X���[�h */
	ACCESS_MODE			m_accessMode;
	/* �p�� */
	DWORD				m_dwInheritence;
	/* �A�N�Z�X�}�X�N */
	DWORD				m_dwAccessMask;
	/* �g���X�g�^�C�v */
	TRUSTEE_TYPE        m_nTrusteeType;
	/* �A�J�E���g�� */
	XStrBuff			m_strAccount;
	/* �h���C���� */
	XStrBuff			m_strDomain;
	/* SID�p�o�b�t�@*/
	XBuff				m_SID;
	/* �A�J�E���g�������������ǂ��� */
	bool				m_bMapped;
};


/**
 * �A�N�Z�X�R���g���[�����X�g(ACL)�̊Ǘ��N���X
 */
class XNtSecAcl
{
public:
	/** �R���X�g���N�^ */
	XNtSecAcl();
	/** �f�X�g���N�^ */
	virtual ~XNtSecAcl();
	/** �N���[���A�b�v���܂��B */
	virtual void Clear();

	
	/**
	 * �A�N�Z�X�R���g���[���G���g���[�iACE�j�I�u�W�F�N�g��ǉ����܂�
	 * @param[in] strAccountName �A�J�E���g���̕�����
	 * @param[in] lpszServerOrDoamin �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @return �쐬���ꂽ�A�N�Z�X�R���g���[���G���g���[�iACE�j�I�u�W�F�N�g�̃A�h���X�B���s�����ꍇ��NULL
	 */
	XNtSecAce *Add(LPCTSTR strAccountName, LPCTSTR lpszServerOrDoamin = NULL);
	/**
	 * �A�N�Z�X�R���g���[���G���g���[�iACE�j�I�u�W�F�N�g��ǉ����܂�
	 * @param[in] pSID �Z�b�g����A�J�E���g��SID�̃A�h���X
	 * @param[in] lpszServerOrDoamin �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @return �쐬���ꂽ�A�N�Z�X�R���g���[���G���g���[�iACE�j�I�u�W�F�N�g�̃A�h���X�B���s�����ꍇ��NULL
	 */
	XNtSecAce *Add(SID *pSID, LPCTSTR lpszServerOrDoamin = NULL);
	/**
	 * �w�肵���C���f�b�N�X�̃A�N�Z�X�R���g���[���G���g���[�iACE�j�I�u�W�F�N�g���擾���܂�
	 * @param[in] nIndex �擾���郊�X�g�̃C���f�b�N�X
	 * @return �쐬���ꂽ�A�N�Z�X�R���g���[���G���g���[�iACE�j�I�u�W�F�N�g�̃A�h���X�B���s�����ꍇ��NULL
	 */
	XNtSecAce *operator[] (int nIndex) const;
	/**
	 * �A�N�Z�X�R���g���[���G���g���[�̐����擾���܂�
	 * @return �A�N�Z�X�R���g���[���G���g���[�̐�
	 */
	int Count();
	/**
	 * �A�N�Z�X�R���g���[�����X�g�̃A�h���X����A���̃I�u�W�F�N�g�����������܂�
	 * @param[in] pAcl �A�N�Z�X�R���g���[�����X�g�̃A�h��
	 * @param[in] bInherit �e����p������A���Ԃ̂Ȃ�ACE���擾���܂�
	 * @param[in] lpszServer �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Fill(PACL pAcl, bool bInherit, LPCTSTR lpszServer);

// NOT OF USER SCOPE	
	/**
	 * �Z�L�����e�B���X�V���邽�߂́A�V����ACL���쐬���܂�
	 * @param[in] bInheritCopy �쐬���鎞�A�p�����ꂽACE������ACE�ɕϊ�����ꍇ�́ATRUE���w��B
	 * @param[in] pOldACL ���݂�ACL�̃A�h���X
	 * @param[in] ppNewACL �V�����쐬���ꂽACL���擾����ACL�̃A�h���X
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool _NewACL(bool bInheritCopy, ACL *pOldACL, ACL **ppNewACL);
	friend class XNtSecAce;
private:
	/* �V����ACE�I�u�W�F�N�g���쐬���I�u�W�F�N�g�̃A�h���X��Ԃ��܂��B�h���N���X�ŃI�[�o���C�h���Ă��������B
	 * @return �쐬�����I�u�W�F�N�g�̃A�h���X
	 */
	virtual void *_New();
	/* _New�����o�ō쐬�����A�h���X�̃f�[�^���폜���܂��B�h���N���X�ŃI�[�o���C�h���Ă��������B
	 * @param[in] pDeleteData �폜����I�u�W�F�N�g�̃A�h���X
	 * @return �쐬�����I�u�W�F�N�g�̃A�h���X
	 */
	virtual void _Delete(void *pDeleteData);
	/*
	 * _New�����o�ō쐬�����A�h���X�̃f�[�^������z��ɒǉ����܂��B�h���N���X�ŃI�[�o���C�h���Ă��������B
	 * @param[in] pNewData �ǉ�����I�u�W�F�N�g�̃A�h���X
	 */
	virtual void _SetArray(void* pNewData);
	/* EXPLICIT_ACCESS�̃A�h���X����V�K��ACE���쐬���܂��B
	 * @param[in] pExplicitAccess EXPLICIT_ACCESS�̃A�h���X
	 * @param[in] lpszServer �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool _Set2ExplicitAccess(EXPLICIT_ACCESS *pExplicitAccess , LPCTSTR lpszServer);
	/* ACE_HEADER�̃A�h���X����V�K��ACE���쐬���܂��B
	 * @param[in] pAce EXPLICIT_ACCESS�̃A�h���X
	 * @param[in] lpszServer �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool _Set2AceHead(ACE_HEADER *pAce , LPCTSTR lpszServer);
	/* �Z�L�����e�B���X�V���邽�߂́A�V����ACE�̐����擾���܂�
	 * @param[in] bInheritCopy �쐬���鎞�A�p�����ꂽACE������ACE�ɕϊ�����ꍇ�́ATRUE���w��B
	 * @retval true ����
	 * @retval false ���s
	 */
	DWORD _NewAclCount(bool bInheritCopy);
protected:
	/* �A�N�Z�X�R���g���[���G���g���[�p�z�� */
	XArray<void*>        m_array;
	/* �A�N�Z�X�R���g���[�����X�g(ACL)�p�o�b�t�@ */
	XBuff                m_buffACL;
};


/**
 * �Z�L�����e�B�f�X�N���v�^�Ǘ��N���X
 */
class XNtSecDescriptor
{
public:
	enum OwnSTATUS
	{
		STATUS_OPEND = 0x00000001,                  // �I�[�v������Ă���
		STATUS_GET_INHERITED = 0x00000002,          // �I�[�u������Ă��鎞�ɁA�p�����ꂽ�Z�L�����e�B�I�u�W�F�N�g���擾���Ă���
		STATUS_INHERIT_DACL_COPY = 0x00000004,		// 
		STATUS_INHERIT_SACL_COPY = 0x00000008,		// 
	};
	typedef struct _SECCTRL
	{
		SECURITY_DESCRIPTOR_CONTROL	    m_nCtrl;		// �Z�L�����e�B�ŃX�N���v�^�[�R���g���[��
		DWORD                           m_dwRevisuin;	// ���r�W����
	}SECCTRL;
public:
	/** �R���X�g���N�^ */
	XNtSecDescriptor(XNtSecAcl* pDaclBuff, XNtSecAcl* pSaclBuff);
	/** �f�X�g���N�^ */
	virtual ~XNtSecDescriptor();
	/** �I�u�W�F�N�g�����������܂� */
	void Free();

	
// Atributes
	/**
	 * �I�[�v�������I�u�W�F�N�g�̃^�C�v���擾���܂�
	 * @return �I�u�W�F�N�g�̃^�C�v�B�ڂ����́AMSDN:SE_OBJECT_TYPE ���Q��
	 */
	SE_OBJECT_TYPE ObjectType();
	/**
	 * �I�[�v�������I�u�W�F�N�g�����擾���܂�
	 * @return �I�u�W�F�N�g���B
	 */
	LPCTSTR ObjectName();
	/**
	 * �I�[�v�������Z�L�����e�B�̐��ӃA�N�Z�X���䃊�X�g�̃I�u�W�F�N�g�̃A�h���X�iXNtSecAcl�j���擾���܂�
	 * @return ���ӃA�N�Z�X���䃊�X�g�̃I�u�W�F�N�g�̃A�h���X�iXNtSecAcl
	 */
	XNtSecAcl *raw_GetDACL();
	/**
	 * �I�[�v�������Z�L�����e�B�̃V�X�e���A�N�Z�X���䃊�X�g�̃I�u�W�F�N�g�̃A�h���X�iXNtSecAcl�j���擾���܂�
	 * @return �V�X�e���A�N�Z�X���䃊�X�g�̃I�u�W�F�N�g�̃A�h���X
	 */
	XNtSecAcl *raw_GetSACL();
	/**
	 * �I�[�v�������Z�L�����e�B�̃I�[�i�[���擾���܂�
	 * @return �I�[�v�������Z�L�����e�B�̃I�[�i�[
	 */
	XNtSecAccount *GetOwner();
	/**
	 * �I�[�v�������Z�L�����e�B�̃O���[�v���擾���܂�
	 * @return �I�[�v�������Z�L�����e�B�̃O���[�v
	 */
	XNtSecAccount *GetGroup();
	/**
	 * �I�[�v�������Z�L�����e�B�̃Z�L�����e�B�R���g���[���t���O���擾���܂�
	 * @return �O���[�v
	 */
	SECURITY_DESCRIPTOR_CONTROL GetSecCtrl();
// Operation
	/**
	 * �I�u�W�F�N�g�̃Z�L�����e�B���I�[�v�����܂�
	 * @param[in] lpszObjectName �I�u�W�F�N�g���i�I�u�W�F�N�g�̃p�X�j
	 * @param[in] nOblectType �I�u�W�F�N�g�̃^�C�v
	 * @param[in] nSecurityInfo �擾����Z�L�����e�B���̃I�t�Z�b�g�B���̃t���O�̑g�ݍ��킹���w�肵�܂��BDACL_SECURITY_INFORMATION,SACL_SECURITY_INFORMATION,OWNER_SECURITY_INFORMATION,GROUP_SECURITY_INFORMATION
	 * @param[in] bInherit �e����p������A���Ԃ̂Ȃ�ACE���擾����ꍇ��TRUE�B�擾���Ȃ��ꍇFALSE�B
	 * @param[in] lpszServerOrDomain �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Open(LPCTSTR lpszObjectName, 
					SE_OBJECT_TYPE nOblectType, 
					long nSecurityInfo = (DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION),
					bool bInherit = TRUE, 
					LPCTSTR lpszServerOrDomain = NULL);
	virtual bool Open(HANDLE hHangle,
					SE_OBJECT_TYPE nOblectType, 
					long nSecurityInfo = (DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION),
					bool bInherit = TRUE,
					LPCTSTR lpszServerOrDomain = NULL);
	/**
	 * �I�[�v�������I�u�W�F�N�g�̃Z�L�����e�B���X�V���܂�
	 * @param[in] nSecurityInfo �擾����Z�L�����e�B���̃I�t�Z�b�g�B���̃t���O�̑g�ݍ��킹���w�肵�܂��BDACL_SECURITY_INFORMATION,SACL_SECURITY_INFORMATION,OWNER_SECURITY_INFORMATION,GROUP_SECURITY_INFORMATION
	 * @param[in] dwUpdateOptions �X�V���̍X�V�t���O�B�I�[�o�[���C�h���Ƀ��[�U�[��`�Ŏg�p���܂�
	 * @param[in] lpszServerOrDomain �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Update(long nSecurityInfo, DWORD dwUpdateOptions = 0);
private:
// UOT OF USER SCOPE	
	/**
	 * SE_PRIVILEGE_ENABLED�̓�����L���E�����ɐݒ肵�܂��B
	 * @param[in] bEnable �L��=true�E����=false
	 * @param[in] lpszServerOrDomain �h���C�����܂��́A�R���s���[�^���̕�����B���[�J���R���s���[�^�̏ꍇ��NULL���w�肵�܂�
	 * @param[out] pOldToken �g�[�N�����󂯎��o�b�t�@
	 * @param[out] buffOldToken �g�[�N�����󂯎��o�b�t�@
	 * @retval true ����
	 * @retval false ���s
	 */
	DWORD PrivilegeEnableGetSACL(bool bEnable, LPCTSTR lpszServerOrDomain, PTOKEN_PRIVILEGES *pOldToken, XBuff &buffOldToken);
	// Update ���V�����Z�L�����e�B�����\�z���擾���܂�
	/**
	 * ���V�����Z�L�����e�B�����\�z���擾���܂�
	 * @param[in] nSecurityInfo �L��=true�E����=false
	 * @param[out] pDACL DACL���󂯎��A�h���X
	 * @param[out] pSACL  SACL���󂯎��A�h���X
	 * @param[out] pOwner �I�[�i�[�󂯎��A�h���X
	 * @param[out] pGroup �O���[�v�󂯎��A�h���X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool _GetSecurityDescriptorData(long nSecurityInfo, ACL **pDACL, ACL **pSACL, SID **pOwner, SID **pGroup);
protected:
	/* �I�u�W�F�N�g�̃^�C�v */
	SE_OBJECT_TYPE      m_tyObject;
	/* �I�u�W�F�N�g�� */
	XStrBuff            m_strObjectName;
	/* �Z�L�����e�B�ŃX�N���v�^�[�R���g���[�� */
	SECCTRL             m_secCtrl;
	/* ���̃I�u�W�F�g�̏�ԃX�e�[�^�X */
	DWORD               m_dwStatus;
	/* �I�[�v�����̃Z�L�����e�B�t���O */
	DWORD               m_dwOpendSecurityFlag;
	/* �Z�L�����e�B�L�q�q����DACL�i���ӃA�N�Z�X���䃊�X�g�j */
	XNtSecAcl           *m_DACL;
	/* �Z�L�����e�B�L�q�q����SACL�i�V�X�e���A�N�Z�X���䃊�X�g�j */
	XNtSecAcl           *m_SACL;
	/* �Z�L�����e�B�L�q�q���珊�L�ҏ�� */
	XNtSecAccount       m_Owner;
	/* �Z�L�����e�B�L�q�q�̃v���C�}���O���[�v��� */
	XNtSecAccount       m_Group;
	/* �Z�L�����e�B�L�q�q�ւ̃|�C���^���󂯎��܂� */
	PSECURITY_DESCRIPTOR	m_psd;
	/* ���L��SID�ւ̃|�C���^���󂯎��܂� */
	PSID					m_ppsidOwner;
	/* �v���C�}���O���[�vSID�ւ̃|�C���^���󂯎��܂� */
	PSID					m_ppsidGroup;
	/* DACL�ւ̃|�C���^���󂯎��܂� */
	PACL					m_ppDacl;
	/* SACL�ւ̃|�C���^���󂯎��܂� */
	PACL					m_ppSacl;
};



/**
 * �A�N�Z�X�R���g���[���G���g��(ACE)�̊Ǘ��N���Xfor�t�@�C��
 */
class XFileSecAce : public XNtSecAce
{
public:
	enum  FileAccessMASK
	{
		enumFolderExecFile             = 0x00000001,
		createFileWriteData            = 0x00000002,
		createFolderAddData            = 0x00000004,
		readExAtribute                 = 0x00000008,
		writExAtribute                 = 0x00000010,
		scanFolderExecFile             = 0x00000020,
		deleteSubFolderFile            = 0x00000040,
		readAtribute                   = 0x00000080,
		writAtribute                   = 0x00000100,
	};
public:
	/** �R���X�g���N�^ */
	XFileSecAce();
	/** �f�X�g���N�^ */
	virtual ~XFileSecAce();


	/**
	 * �A�N�Z�X�}�X�N���A�t�H���_�̈ꗗ�E�f�[�^�̓ǂݎ��A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_enumFolderExecFile();
	/**
	 * �A�N�Z�X�}�X�N�ɁA�t�H���_�̈ꗗ�E�f�[�^�̓ǂݎ��A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_enumFolderExecFile(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A�t�@�C���̍쐬�E�f�[�^�̏������݃A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_createFileWriteData();
	/**
	 * �A�N�Z�X�}�X�N�ɁA'�t�@�C���̍쐬�E�f�[�^�̏�������'�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_createFileWriteData(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A'�t�H���_�̍쐬�E�f�[�^�̒ǉ�'�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_createFolderAddData();
	/**
	 * �A�N�Z�X�}�X�N�ɁA'�t�H���_�̍쐬�E�f�[�^�̒ǉ�'�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_createFolderAddData(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A'�g�������̓ǂݍ���'�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_readExAtribute();
	/**
	 * �A�N�Z�X�}�X�N�ɁA'�g�������̓ǂݍ���'�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_readExAtribute(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A'�g�������̏�������'�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_writExAtribute();
	/**
	 * �A�N�Z�X�}�X�N�ɁA'�g�������̏�������'�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_writExAtribute(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A'�t�H���_�̃X�L�����E�t�@�C�����s'�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_scanFolderExecFile();
	/**
	 * �A�N�Z�X�}�X�N�ɁA'�t�H���_�̃X�L�����E�t�@�C�����s'�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_scanFolderExecFile(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A'�T�u�t�H���_�̍폜'�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_deleteSubFolderFile();
	/**
	 *�A�N�Z�X�}�X�N�ɁA'�T�u�t�H���_�̍폜'�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_deleteSubFolderFile(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A'�����̓ǂݍ���'�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_readAtribute();
	/**
	 * �A�N�Z�X�}�X�N�ɁA'�����̓ǂݍ���'�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_readAtribute(bool newVal);
	/**
	 * �A�N�Z�X�}�X�N���A'�����̏�������'�A�N�Z�X�}�X�N�����L���Ă��邩���ׂ܂�
	 * @retval true ���L���Ă���
	 * @retval false ���L���Ă��Ȃ�
	 */
	bool get_writAtribute();
	/**
	 * �A�N�Z�X�}�X�N�ɁA'�����̏�������'�A�N�Z�X�}�X�N�̃Z�b�g�܂��̓A���Z�b�g���܂�
	 * @param[in] newVal �Z�b�g����V�����l
	 */
	void put_writAtribute(bool newVal);
};


/**
 * �A�N�Z�X�R���g���[�����X�g(ACL)�̊Ǘ��N���Xfor�t�@�C��
 */
class XFileSecAcl : public XNtSecAcl
{
public:
	/** �R���X�g���N�^ */
	XFileSecAcl();
	/** �f�X�g���N�^ */
	virtual ~XFileSecAcl();
	/**  XNtSecAcl::Clear() */
	virtual void Clear();
	/**
	 * �w�肵���C���f�b�N�X�̃A�N�Z�X�R���g���[���G���g���[���擾
	 * @param[in] nIndex �C���f�b�N�X
	 * @return �A�N�Z�X�R���g���[���G���g���[
	 */
	XFileSecAce *GetAt(int nIndex);
private:
	/** @copydoc XNtSecAcl::_New() */
	virtual void *_New();
	/** @copydoc XNtSecAcl::_Delete(void*) */
	virtual void _Delete(void *pDeleteData);
	/** @copydoc XNtSecAcl::_SetArray(void*) */
	virtual void _SetArray(void* pNewData);
};


/**
 * �Z�L�����e�B�f�X�N���v�^�Ǘ��N���Xfor�t�@�C��
 */
class XFileSecDescriptor : public XNtSecDescriptor
{
public:
	/** �R���X�g���N�^ */
	XFileSecDescriptor();
	/** �f�X�g���N�^ */
	virtual ~XFileSecDescriptor();
//Atibute
	/** @copydoc XNtSecDescriptor::GetDACL() */
	XFileSecAcl *GetDACL();
	/** @copydoc XNtSecDescriptor::GetSACL() */
	XFileSecAcl *GetSACL();
// Operation
	/** @copydoc XNtSecDescriptor::Open() */
	bool Open(LPCTSTR lpszFileName, 
					long nSecurityInfo = (DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION), 
					bool bInherit = TRUE, 
					LPCTSTR lpszServerOrDomain = NULL);
	/** @copydoc XNtSecDescriptor::Update() */
	virtual bool Update(long nSecurityInfo, DWORD dwUpdateOptions = 0);
protected:
	/* DACL */
	XFileSecAcl m_buffDACL;
	/* SACL */
	XFileSecAcl m_buffSACL;
};
#endif //WIN32
#endif //__X_WIN_NT_SECURITY_HEASER_FILE__

