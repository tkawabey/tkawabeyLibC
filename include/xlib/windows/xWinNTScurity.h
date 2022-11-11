/**
 * @file xWinNTScurity.h
 * @brief NTセキュリティクラス
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
 * セキュリティアカウント情報管理クラス
 */
class XNtSecAccount
{
public:
	/** コンストラクタ */
	XNtSecAccount();
	/** デストラクタ */
	virtual ~XNtSecAccount();
// Atribute
	/**
	 * アカウントが、システムに存在するか調べます
	 * @retval true 存在する
	 * @retval false 存在しない
	 */
	bool IsMapped();
	/**
	 * SIDを取得します
	 * @return SIDのアドレス
	 */
	SID *GetSID();
	/**
	 * アカウントのタイプを取得します。
	 * @note  詳しくは、MSDN:TRUSTEE_TYPEを参照
	 * @return アカウントのタイプ
	 */
	TRUSTEE_TYPE TrusteeType();
	/**
	 * アカウントのトラスティ名を取得します
	 * @note  以下のいずれか
	 *  {DOMAIN}\{ACCOUNT NAME}
	 *  {ACCOUNT NAME}
	 *  {SID STRING}
	 * @return トラスティ名
	 */
	XStrBuff TrusteeName();
	/**
	 * ドメイン名を取得します。
	 * @return ドメイン名
	 */
	LPCTSTR DomainName();
	/**
	 * アカウント名を取得します。
	 * @return アカウント名
	 */
	LPCTSTR AccountName();
	/**
	 * SIDを可視でわかる文字列で取得します。
	 * @return SID文字列
	 */
	XStrBuff GetSIDString();
// Operation
	/**
	 * アカウント情報をセットします
	 * @param[in] lpszAccountName アカウント名の文字列
	 * @param[in] lpszServerOrDoamin ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @return SID文字列
	 */
	bool SetAccount(LPCTSTR lpszAccountName, LPCTSTR lpszServerOrDoamin = NULL);
	/**
	 * アカウント情報をセットします
	 * @param[in] pSID セットするアカウントのSIDのアドレス
	 * @param[in] lpszServerOrDoamin ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @return SID文字列
	 */
	bool SetAccount(SID *pSID, LPCTSTR lpszServerOrDoamin = NULL);
protected:
	/** アカウント名 */
	XStrBuff			m_strAccount;
	/** ドメイン */
	XStrBuff			m_strDomain;
	/** SID用のバッファ */
	XBuff				m_SID;
	/** アカウントが見つかったかどうか */
	bool				m_bMapped;
	/** アカウントのタイプ */
	TRUSTEE_TYPE        m_nTrusteeType;
};

/**
 * アクセスコントロールエントリ(ACE)の管理クラス
 */
class XNtSecAce : public XNtSecAccount
{
public:
	/** コンストラクタ */
	XNtSecAce();
	/** デストラクタ */
	virtual ~XNtSecAce();
// Atribute
	/**
	 * このACEが親から継承されたACEで実態のないACEかどうか調べます
	 * @retval true 親から継承されたACE
	 * @retval false 実態
	 */
	bool IsIsInherited();
	/**
	 * 継承フラグを取得します
	 * @return 継承フラグ
	 */
	long get_Inheritance();
	/**
	 * 継承フラグをセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_Inheritance(long newVal);
	/**
	 * アクセスマスクを取得します
	 * @return アクセスマスク
	 */
	long get_AccessMask();
	/**
	 * アクセスマスクをセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_AccessMask(long newVal);
	/**
	 * アアクセスモードを取得します
	 * @return アアクセスモード
	 */
	ACCESS_MODE get_AccessMode();
	/**
	 * アアクセスモードをセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_AccessMode(ACCESS_MODE newVal);
// GeneralCtrl
public:
	/**
	 * アクセスマスクが、読み込みアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskRead();
	/**
	 * アクセスマスクに、読み込みアクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
	 */
	void put_rawAccessMaskRead(bool newVal);
	/**
	 * アクセスマスクが、書き込みアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskGenericWrite();
	/**
	 * アクセスマスクに、書き込みアクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
	 */
	void put_rawAccessMaskGenericWrite(bool newVal);
	/**
	 * アクセスマスクが、実行アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskGenericExecute();
	/**
	 * アクセスマスクに、実行アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
	 */
	void put_rawAccessMaskGenericExecute(bool newVal);
	/**
	 * アクセスマスクが、フルアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskGenericAll();
	/**
	 * アクセスマスクに、フルアクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
	 */
	void put_rawAccessMaskGenericAll(bool newVal);
	/**
	 * アクセスマスクが、Synchronizeアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskSynchronize();
	/**
	 * アクセスマスクに、Synchronizeアクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
	 */
	void put_rawAccessMaskSynchronize(bool newVal);
	/**
	 * アクセスマスクが、オーナー情報書き込みアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskWriteOwner();
	/**
	 * アクセスマスクに、オーナー情報書き込みアクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
	 */
	void put_rawAccessMaskWriteOwner(bool newVal);
	/**
	 * アクセスマスクが、随意アクセス制御リスト（DACL）書き込みアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskWriteDACL();
	/**
	 * アクセスマスクに、随意アクセス制御リスト（DACL）書き込みアクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
	 */
	void put_rawAccessMaskWriteDACL(bool newVal);
	/**
	 * アクセスマスクが、アクセスコントロールの読み込みアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskReadCtrl();
	/**
	 * アクセスマスクに、アクセスコントロールの読み込みアクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
	 */
	void put_rawAccessMaskReadCtrl(bool newVal);
	/**
	 * アクセスマスクが、削除アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskDelete();
	/**
	 * アクセスマスクに、削除アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする場合はtrue。アンセットする場合はfalse
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
	 * アクセスマスクが、指定したオフセットのアクセス権を所有しているか調べます
	 * @param[in] nOR アクセスマスクのオフセット
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_rawAccessMaskOr(long nOR);
	/*
	 * アクセスマスクが、指定したオフセットのアクセス権をセット、またはアンセットします
	 * @param[in] nOR 
	 * @param[in] b 
	 */
	void put_rawAccessMaskOr(long nOR, bool b);
public:
	friend class XNtSecAcl;
protected:
	/* アクセスモード */
	ACCESS_MODE			m_accessMode;
	/* 継承 */
	DWORD				m_dwInheritence;
	/* アクセスマスク */
	DWORD				m_dwAccessMask;
	/* トラストタイプ */
	TRUSTEE_TYPE        m_nTrusteeType;
	/* アカウント名 */
	XStrBuff			m_strAccount;
	/* ドメイン名 */
	XStrBuff			m_strDomain;
	/* SID用バッファ*/
	XBuff				m_SID;
	/* アカウントが見つかったかどうか */
	bool				m_bMapped;
};


/**
 * アクセスコントロールリスト(ACL)の管理クラス
 */
class XNtSecAcl
{
public:
	/** コンストラクタ */
	XNtSecAcl();
	/** デストラクタ */
	virtual ~XNtSecAcl();
	/** クリーンアップします。 */
	virtual void Clear();

	
	/**
	 * アクセスコントロールエントリー（ACE）オブジェクトを追加します
	 * @param[in] strAccountName アカウント名の文字列
	 * @param[in] lpszServerOrDoamin ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @return 作成されたアクセスコントロールエントリー（ACE）オブジェクトのアドレス。失敗した場合はNULL
	 */
	XNtSecAce *Add(LPCTSTR strAccountName, LPCTSTR lpszServerOrDoamin = NULL);
	/**
	 * アクセスコントロールエントリー（ACE）オブジェクトを追加します
	 * @param[in] pSID セットするアカウントのSIDのアドレス
	 * @param[in] lpszServerOrDoamin ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @return 作成されたアクセスコントロールエントリー（ACE）オブジェクトのアドレス。失敗した場合はNULL
	 */
	XNtSecAce *Add(SID *pSID, LPCTSTR lpszServerOrDoamin = NULL);
	/**
	 * 指定したインデックスのアクセスコントロールエントリー（ACE）オブジェクトを取得します
	 * @param[in] nIndex 取得するリストのインデックス
	 * @return 作成されたアクセスコントロールエントリー（ACE）オブジェクトのアドレス。失敗した場合はNULL
	 */
	XNtSecAce *operator[] (int nIndex) const;
	/**
	 * アクセスコントロールエントリーの数を取得します
	 * @return アクセスコントロールエントリーの数
	 */
	int Count();
	/**
	 * アクセスコントロールリストのアドレスから、このオブジェクトを初期化します
	 * @param[in] pAcl アクセスコントロールリストのアドレ
	 * @param[in] bInherit 親から継承され、実態のないACEも取得します
	 * @param[in] lpszServer ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Fill(PACL pAcl, bool bInherit, LPCTSTR lpszServer);

// NOT OF USER SCOPE	
	/**
	 * セキュリティを更新するための、新しいACLを作成します
	 * @param[in] bInheritCopy 作成する時、継承されたACEを実態ACEに変換する場合は、TRUEを指定。
	 * @param[in] pOldACL 現在のACLのアドレス
	 * @param[in] ppNewACL 新しく作成されたACLを取得するACLのアドレス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool _NewACL(bool bInheritCopy, ACL *pOldACL, ACL **ppNewACL);
	friend class XNtSecAce;
private:
	/* 新しいACEオブジェクトを作成しオブジェクトのアドレスを返します。派生クラスでオーバライドしてください。
	 * @return 作成したオブジェクトのアドレス
	 */
	virtual void *_New();
	/* _Newメンバで作成したアドレスのデータを削除します。派生クラスでオーバライドしてください。
	 * @param[in] pDeleteData 削除するオブジェクトのアドレス
	 * @return 作成したオブジェクトのアドレス
	 */
	virtual void _Delete(void *pDeleteData);
	/*
	 * _Newメンバで作成したアドレスのデータを内部配列に追加します。派生クラスでオーバライドしてください。
	 * @param[in] pNewData 追加するオブジェクトのアドレス
	 */
	virtual void _SetArray(void* pNewData);
	/* EXPLICIT_ACCESSのアドレスから新規にACEを作成します。
	 * @param[in] pExplicitAccess EXPLICIT_ACCESSのアドレス
	 * @param[in] lpszServer ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool _Set2ExplicitAccess(EXPLICIT_ACCESS *pExplicitAccess , LPCTSTR lpszServer);
	/* ACE_HEADERのアドレスから新規にACEを作成します。
	 * @param[in] pAce EXPLICIT_ACCESSのアドレス
	 * @param[in] lpszServer ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool _Set2AceHead(ACE_HEADER *pAce , LPCTSTR lpszServer);
	/* セキュリティを更新するための、新しいACEの数を取得します
	 * @param[in] bInheritCopy 作成する時、継承されたACEを実態ACEに変換する場合は、TRUEを指定。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	DWORD _NewAclCount(bool bInheritCopy);
protected:
	/* アクセスコントロールエントリー用配列 */
	XArray<void*>        m_array;
	/* アクセスコントロールリスト(ACL)用バッファ */
	XBuff                m_buffACL;
};


/**
 * セキュリティデスクリプタ管理クラス
 */
class XNtSecDescriptor
{
public:
	enum OwnSTATUS
	{
		STATUS_OPEND = 0x00000001,                  // オープンされている
		STATUS_GET_INHERITED = 0x00000002,          // オーブンされている時に、継承されたセキュリティオブジェクトも取得している
		STATUS_INHERIT_DACL_COPY = 0x00000004,		// 
		STATUS_INHERIT_SACL_COPY = 0x00000008,		// 
	};
	typedef struct _SECCTRL
	{
		SECURITY_DESCRIPTOR_CONTROL	    m_nCtrl;		// セキュリティでスクリプターコントロール
		DWORD                           m_dwRevisuin;	// リビジョン
	}SECCTRL;
public:
	/** コンストラクタ */
	XNtSecDescriptor(XNtSecAcl* pDaclBuff, XNtSecAcl* pSaclBuff);
	/** デストラクタ */
	virtual ~XNtSecDescriptor();
	/** オブジェクトを初期化します */
	void Free();

	
// Atributes
	/**
	 * オープンしたオブジェクトのタイプを取得します
	 * @return オブジェクトのタイプ。詳しくは、MSDN:SE_OBJECT_TYPE を参照
	 */
	SE_OBJECT_TYPE ObjectType();
	/**
	 * オープンしたオブジェクト名を取得します
	 * @return オブジェクト名。
	 */
	LPCTSTR ObjectName();
	/**
	 * オープンしたセキュリティの随意アクセス制御リストのオブジェクトのアドレス（XNtSecAcl）を取得します
	 * @return 随意アクセス制御リストのオブジェクトのアドレス（XNtSecAcl
	 */
	XNtSecAcl *raw_GetDACL();
	/**
	 * オープンしたセキュリティのシステムアクセス制御リストのオブジェクトのアドレス（XNtSecAcl）を取得します
	 * @return システムアクセス制御リストのオブジェクトのアドレス
	 */
	XNtSecAcl *raw_GetSACL();
	/**
	 * オープンしたセキュリティのオーナーを取得します
	 * @return オープンしたセキュリティのオーナー
	 */
	XNtSecAccount *GetOwner();
	/**
	 * オープンしたセキュリティのグループを取得します
	 * @return オープンしたセキュリティのグループ
	 */
	XNtSecAccount *GetGroup();
	/**
	 * オープンしたセキュリティのセキュリティコントロールフラグを取得します
	 * @return グループ
	 */
	SECURITY_DESCRIPTOR_CONTROL GetSecCtrl();
// Operation
	/**
	 * オブジェクトのセキュリティをオープンします
	 * @param[in] lpszObjectName オブジェクト名（オブジェクトのパス）
	 * @param[in] nOblectType オブジェクトのタイプ
	 * @param[in] nSecurityInfo 取得するセキュリティ情報のオフセット。次のフラグの組み合わせを指定します。DACL_SECURITY_INFORMATION,SACL_SECURITY_INFORMATION,OWNER_SECURITY_INFORMATION,GROUP_SECURITY_INFORMATION
	 * @param[in] bInherit 親から継承され、実態のないACEも取得する場合はTRUE。取得しない場合FALSE。
	 * @param[in] lpszServerOrDomain ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @retval true 成功
	 * @retval false 失敗
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
	 * オープンしたオブジェクトのセキュリティを更新します
	 * @param[in] nSecurityInfo 取得するセキュリティ情報のオフセット。次のフラグの組み合わせを指定します。DACL_SECURITY_INFORMATION,SACL_SECURITY_INFORMATION,OWNER_SECURITY_INFORMATION,GROUP_SECURITY_INFORMATION
	 * @param[in] dwUpdateOptions 更新時の更新フラグ。オーバーライド時にユーザー定義で使用します
	 * @param[in] lpszServerOrDomain ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Update(long nSecurityInfo, DWORD dwUpdateOptions = 0);
private:
// UOT OF USER SCOPE	
	/**
	 * SE_PRIVILEGE_ENABLEDの特権を有効・無効に設定します。
	 * @param[in] bEnable 有効=true・無効=false
	 * @param[in] lpszServerOrDomain ドメイン名または、コンピュータ名の文字列。ローカルコンピュータの場合はNULLを指定します
	 * @param[out] pOldToken トークンを受け取るバッファ
	 * @param[out] buffOldToken トークンを受け取るバッファ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	DWORD PrivilegeEnableGetSACL(bool bEnable, LPCTSTR lpszServerOrDomain, PTOKEN_PRIVILEGES *pOldToken, XBuff &buffOldToken);
	// Update 自新しいセキュリティ情報を構築し取得します
	/**
	 * 自新しいセキュリティ情報を構築し取得します
	 * @param[in] nSecurityInfo 有効=true・無効=false
	 * @param[out] pDACL DACLを受け取るアドレス
	 * @param[out] pSACL  SACLを受け取るアドレス
	 * @param[out] pOwner オーナー受け取るアドレス
	 * @param[out] pGroup グループ受け取るアドレス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool _GetSecurityDescriptorData(long nSecurityInfo, ACL **pDACL, ACL **pSACL, SID **pOwner, SID **pGroup);
protected:
	/* オブジェクトのタイプ */
	SE_OBJECT_TYPE      m_tyObject;
	/* オブジェクト名 */
	XStrBuff            m_strObjectName;
	/* セキュリティでスクリプターコントロール */
	SECCTRL             m_secCtrl;
	/* このオブジェトの状態ステータス */
	DWORD               m_dwStatus;
	/* オープン時のセキュリティフラグ */
	DWORD               m_dwOpendSecurityFlag;
	/* セキュリティ記述子内のDACL（随意アクセス制御リスト） */
	XNtSecAcl           *m_DACL;
	/* セキュリティ記述子内のSACL（システムアクセス制御リスト） */
	XNtSecAcl           *m_SACL;
	/* セキュリティ記述子から所有者情報 */
	XNtSecAccount       m_Owner;
	/* セキュリティ記述子のプライマリグループ情報 */
	XNtSecAccount       m_Group;
	/* セキュリティ記述子へのポインタを受け取ります */
	PSECURITY_DESCRIPTOR	m_psd;
	/* 所有者SIDへのポインタを受け取ります */
	PSID					m_ppsidOwner;
	/* プライマリグループSIDへのポインタを受け取ります */
	PSID					m_ppsidGroup;
	/* DACLへのポインタを受け取ります */
	PACL					m_ppDacl;
	/* SACLへのポインタを受け取ります */
	PACL					m_ppSacl;
};



/**
 * アクセスコントロールエントリ(ACE)の管理クラスforファイル
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
	/** コンストラクタ */
	XFileSecAce();
	/** デストラクタ */
	virtual ~XFileSecAce();


	/**
	 * アクセスマスクが、フォルダの一覧・データの読み取りアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_enumFolderExecFile();
	/**
	 * アクセスマスクに、フォルダの一覧・データの読み取りアクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_enumFolderExecFile(bool newVal);
	/**
	 * アクセスマスクが、ファイルの作成・データの書き込みアクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_createFileWriteData();
	/**
	 * アクセスマスクに、'ファイルの作成・データの書き込み'アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_createFileWriteData(bool newVal);
	/**
	 * アクセスマスクが、'フォルダの作成・データの追加'アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_createFolderAddData();
	/**
	 * アクセスマスクに、'フォルダの作成・データの追加'アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_createFolderAddData(bool newVal);
	/**
	 * アクセスマスクが、'拡張属性の読み込み'アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_readExAtribute();
	/**
	 * アクセスマスクに、'拡張属性の読み込み'アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_readExAtribute(bool newVal);
	/**
	 * アクセスマスクが、'拡張属性の書き込み'アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_writExAtribute();
	/**
	 * アクセスマスクに、'拡張属性の書き込み'アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_writExAtribute(bool newVal);
	/**
	 * アクセスマスクが、'フォルダのスキャン・ファイル実行'アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_scanFolderExecFile();
	/**
	 * アクセスマスクに、'フォルダのスキャン・ファイル実行'アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_scanFolderExecFile(bool newVal);
	/**
	 * アクセスマスクが、'サブフォルダの削除'アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_deleteSubFolderFile();
	/**
	 *アクセスマスクに、'サブフォルダの削除'アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_deleteSubFolderFile(bool newVal);
	/**
	 * アクセスマスクが、'属性の読み込み'アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_readAtribute();
	/**
	 * アクセスマスクに、'属性の読み込み'アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_readAtribute(bool newVal);
	/**
	 * アクセスマスクが、'属性の書き込み'アクセスマスクを所有しているか調べます
	 * @retval true 所有している
	 * @retval false 所有していない
	 */
	bool get_writAtribute();
	/**
	 * アクセスマスクに、'属性の書き込み'アクセスマスクのセットまたはアンセットします
	 * @param[in] newVal セットする新しい値
	 */
	void put_writAtribute(bool newVal);
};


/**
 * アクセスコントロールリスト(ACL)の管理クラスforファイル
 */
class XFileSecAcl : public XNtSecAcl
{
public:
	/** コンストラクタ */
	XFileSecAcl();
	/** デストラクタ */
	virtual ~XFileSecAcl();
	/**  XNtSecAcl::Clear() */
	virtual void Clear();
	/**
	 * 指定したインデックスのアクセスコントロールエントリーを取得
	 * @param[in] nIndex インデックス
	 * @return アクセスコントロールエントリー
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
 * セキュリティデスクリプタ管理クラスforファイル
 */
class XFileSecDescriptor : public XNtSecDescriptor
{
public:
	/** コンストラクタ */
	XFileSecDescriptor();
	/** デストラクタ */
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

