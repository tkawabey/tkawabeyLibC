#ifdef WIN32
 #ifndef _WINDOWS_
  #include <windows.h>
 #endif //_WINDOWS_
 #ifndef _WINDEF_
  #include <windef.h>
 #endif //_WINDEF_
 #ifndef _WINERROR_
  #include <winerr.h>
 #endif //_WINERROR_

  #include <crtdbg.h>
  #include <tchar.h>
  #include <tchar.h>
  #include <stdio.h>


#include "xlib/xeception.h"
#include "xlib/windows/xWinNTScurity.h"


//////////////////////////////////////////////////////////////////////////////////////
//
//	Gloval Function
//
static DWORD GetSidFromString(PSID pSID, LPTSTR lpszBuff);
static PSID GetStringFromSid(LPCTSTR lpszStringSID);
static TRUSTEE_TYPE GetSidUseName2TrusteeType(SID_NAME_USE SidNameUse);


//////////////////////////////////////////////////////////////////////////////////////
//
//	XNtSecAccount
//
XNtSecAccount::XNtSecAccount()
{
	m_nTrusteeType  = TRUSTEE_IS_INVALID;
	m_strAccount    = _T("");
	m_strDomain     = _T("");
	m_bMapped       = false;
}
XNtSecAccount::~XNtSecAccount()
{

}
SID *XNtSecAccount::GetSID(){	return (SID*)(void*)m_SID;}
TRUSTEE_TYPE XNtSecAccount::TrusteeType(){	return m_nTrusteeType;}
XStrBuff XNtSecAccount::TrusteeName()
{
	XStrBuff strTrusteeName=_T("");
	if(m_bMapped == true)
	{
		if( lstrlen(m_strDomain) )
		{
			strTrusteeName.Format(_T("%s\\%s"), (LPCTSTR)m_strDomain ,(LPCTSTR)m_strAccount);
		}
		else
		{
			strTrusteeName = m_strAccount;
		}
	}
	else
	{
		strTrusteeName = this->GetSIDString();
	}
	return strTrusteeName;
}
bool XNtSecAccount::IsMapped(){	return m_bMapped;}
LPCTSTR XNtSecAccount::DomainName(){	return m_strDomain;}
LPCTSTR XNtSecAccount::AccountName(){	return m_strAccount;}
XStrBuff XNtSecAccount::GetSIDString()
{
	XStrBuff strSID;
	TCHAR    buff[1024];
	DWORD    dwError;
	dwError = GetSidFromString((void*)m_SID, buff);
	if(dwError == NO_ERROR)
	{
		strSID = buff;
	}	
	return strSID;
}
bool XNtSecAccount::SetAccount(LPCTSTR lpszAccountName, LPCTSTR lpszServerOrDoamin/* = NULL*/)
{
	SID_NAME_USE	SidNameUse;
	DWORD			dwSidSize, dwDomainSize;
	DWORD			dwError = NO_ERROR;
	XStrBuff        strSid, strDomain;
	LPCTSTR         lpszSystem;
	// 変数初期化
	dwSidSize =  dwDomainSize = 0;
	if( lstrlen(lpszServerOrDoamin) )
		lpszSystem = lpszServerOrDoamin;
	else
		lpszSystem = NULL;

	// アカウント情報を取得するための、バッファサイズの取得
	::LookupAccountName(
				lpszSystem,
				lpszAccountName,
				NULL,
				&dwSidSize,
				NULL,
				&dwDomainSize,
				&SidNameUse);
	dwError = ::GetLastError();
	if( dwError != ERROR_INSUFFICIENT_BUFFER ){return false;}
	// SID + DomainName のバッファの確保
	if( strSid.Alloc(dwSidSize)       == NULL){return false;}
	if( strDomain.Alloc(dwDomainSize) == NULL){return false;}

	// アカウント情報を取得
	if( !::LookupAccountName(
				lpszSystem,
				(LPCTSTR)lpszAccountName,
				strSid,
				&dwSidSize,
				strDomain,
				&dwDomainSize,
				&SidNameUse) )
	{return NULL;}

	m_bMapped       = true;
	m_strAccount    = (LPCTSTR)lpszAccountName;
	m_strDomain     = (LPCTSTR)strDomain;
	m_nTrusteeType  = (TRUSTEE_TYPE)GetSidUseName2TrusteeType(SidNameUse);
	m_SID.DoCopy(&strSid);
	return true;
}
bool XNtSecAccount::SetAccount(SID *pSID, LPCTSTR lpszServerOrDoamin/* = NULL*/)
{
	SID_NAME_USE	SidNameUse;
	DWORD			dwSIDSize, dwNameSize, dwDomainSize;
	DWORD			dwError = NO_ERROR;
	XStrBuff        strSid, strNameBuff, strDomain;
	TRUSTEE_TYPE    trusteeType;
	bool            bMapped;
	LPCTSTR         lpszSystem;
	
	// 変数初期化
	dwNameSize = dwSIDSize =  dwDomainSize = 0;
	if( lstrlen(lpszServerOrDoamin) )
		lpszSystem = lpszServerOrDoamin;
	else
		lpszSystem = NULL;



	// SIDのバッファ確保
	if( ::IsValidSid(pSID) == FALSE )
	{
		::SetLastError(ERROR_INVALID_DATA);
		return false;
	}
	dwSIDSize = ::GetLengthSid(pSID);
	if( strSid.Alloc(dwSIDSize) == NULL ){return false;}
	memcpy(strSid, pSID, dwSIDSize);
	// アカウント情報を取得するための、バッファサイズの取得
	::LookupAccountSid(
				lpszSystem,
				pSID,
				NULL,
				&dwNameSize,
				NULL,
				&dwDomainSize,
				&SidNameUse);
	dwError = ::GetLastError();

	if( dwError == ERROR_NONE_MAPPED )
	{
		// SIDはあるが、アカウントが見つからない
		trusteeType = TRUSTEE_IS_UNKNOWN;
		strNameBuff = _T("");
		strDomain = _T("");
		bMapped = false;
	}
	else
	{
		if( dwError != ERROR_INSUFFICIENT_BUFFER ){return false;}

		// AccountName + DomainName のバッファの確保
		if( strNameBuff.Alloc(dwNameSize)  == NULL){return false;}
		if( strDomain.Alloc(dwDomainSize) == NULL){return false;}

		// アカウント情報を取得
		if( !::LookupAccountSid(
					lpszSystem,
					pSID,
					strNameBuff,
					&dwNameSize,
					strDomain,
					&dwDomainSize,
					&SidNameUse) )
		{return false;}

		trusteeType = GetSidUseName2TrusteeType(SidNameUse);
		bMapped = true;
	}

	m_bMapped       = bMapped;
	m_strAccount    = (LPCTSTR)strNameBuff;
	m_strDomain     = (LPCTSTR)strDomain;
	m_nTrusteeType  = trusteeType;
	m_SID.DoCopy(&strSid);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//	XNtSecAce
//
XNtSecAce::XNtSecAce()
{
	m_accessMode    = NOT_USED_ACCESS;
	m_dwInheritence = 0;
	m_dwAccessMask  = 0;
	m_nTrusteeType  = TRUSTEE_IS_INVALID;
	m_strAccount    = _T("");
	m_strDomain     = _T("");
	m_bMapped       = false;
}
XNtSecAce::~XNtSecAce(){}
bool XNtSecAce::IsIsInherited(){	return (m_dwInheritence & INHERITED_ACE ) ? true : false;}
long XNtSecAce::get_Inheritance(){	return m_dwInheritence;}
void XNtSecAce::put_Inheritance(long newVal){	m_dwInheritence = newVal;}
long XNtSecAce::get_AccessMask(){	return m_dwAccessMask;}
void XNtSecAce::put_AccessMask(long newVal){	m_dwAccessMask = newVal;}
ACCESS_MODE XNtSecAce::get_AccessMode(){	return m_accessMode;}
void XNtSecAce::put_AccessMode(ACCESS_MODE newVal){	m_accessMode = newVal;}
bool XNtSecAce::get_rawAccessMaskRead(){	return get_rawAccessMaskOr(GENERIC_READ);}
void XNtSecAce::put_rawAccessMaskRead(bool newVal){	put_rawAccessMaskOr(GENERIC_READ, newVal);}
bool XNtSecAce::get_rawAccessMaskGenericWrite(){	return get_rawAccessMaskOr(GENERIC_WRITE);}
void XNtSecAce::put_rawAccessMaskGenericWrite(bool newVal){	put_rawAccessMaskOr(GENERIC_WRITE, newVal);}
bool XNtSecAce::get_rawAccessMaskGenericExecute(){	return get_rawAccessMaskOr(GENERIC_EXECUTE);}
void XNtSecAce::put_rawAccessMaskGenericExecute(bool newVal){	put_rawAccessMaskOr(GENERIC_EXECUTE, newVal);}
bool XNtSecAce::get_rawAccessMaskGenericAll(){	return get_rawAccessMaskOr(GENERIC_ALL);}
void XNtSecAce::put_rawAccessMaskGenericAll(bool newVal){	put_rawAccessMaskOr(GENERIC_ALL, newVal);}
bool XNtSecAce::get_rawAccessMaskSynchronize(){	return get_rawAccessMaskOr(SYNCHRONIZE);}
void XNtSecAce::put_rawAccessMaskSynchronize(bool newVal){	put_rawAccessMaskOr(SYNCHRONIZE, newVal);}
bool XNtSecAce::get_rawAccessMaskWriteOwner(){	return get_rawAccessMaskOr(WRITE_OWNER);}
void XNtSecAce::put_rawAccessMaskWriteOwner(bool newVal){	put_rawAccessMaskOr(WRITE_OWNER, newVal);}
bool XNtSecAce::get_rawAccessMaskWriteDACL(){	return get_rawAccessMaskOr(WRITE_DAC);}
void XNtSecAce::put_rawAccessMaskWriteDACL(bool newVal){	put_rawAccessMaskOr(WRITE_DAC, newVal);}
bool XNtSecAce::get_rawAccessMaskReadCtrl(){	return get_rawAccessMaskOr(READ_CONTROL);}
void XNtSecAce::put_rawAccessMaskReadCtrl(bool newVal){	put_rawAccessMaskOr(READ_CONTROL, newVal);}
bool XNtSecAce::get_rawAccessMaskDelete(){	return get_rawAccessMaskOr(DELETE);}
void XNtSecAce::put_rawAccessMaskDelete(bool newVal){	put_rawAccessMaskOr(DELETE, newVal);}
bool XNtSecAce::_SetACEObjectInfo(long nFlag, GUID *pObjectType, GUID *pInheritedObjectType)
{
	return true;
}
bool XNtSecAce::get_rawAccessMaskOr(long nOR)
{
	bool b;
	if( (m_dwAccessMask & (DWORD)nOR) == (DWORD)nOR )
		b = true;
	else
		b = false;
	return b;
}
void XNtSecAce::put_rawAccessMaskOr(long nOR, bool b)
{
	if(b == true)
		m_dwAccessMask |= nOR;
	else
		m_dwAccessMask &= ~nOR;
}
//////////////////////////////////////////////////////////////////////////////////////
//
//	XNtSecAcl
//
XNtSecAcl::XNtSecAcl()
{
}
XNtSecAcl::~XNtSecAcl()
{
	Clear();
}
void XNtSecAcl::Clear()
{
	for(int i = 0; i < m_array.GetSize(); i++)
	{
		XNtSecAce *p = (XNtSecAce*)m_array[i];
		delete p;
	}
	m_array.RemoveAll();
}
XNtSecAce *XNtSecAcl::Add(LPCTSTR lpszAccountName, LPCTSTR lpszServerOrDoamin)
{
	// 新しいオブジェクトの作成
	XNtSecAce *pNew = (XNtSecAce*)_New();
	if(pNew == NULL)
	{
		::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}
	try
	{
		if( pNew->SetAccount(lpszAccountName, lpszServerOrDoamin) == false )
		{
			DWORD rc = ::GetLastError();
			delete pNew;
			::SetLastError(rc);
			return NULL;
		}
		pNew->m_accessMode    = GRANT_ACCESS;
		_SetArray(pNew);
	}
	catch(...)
	{
		::SetLastError(ERROR_INVALID_DATA);
		_Delete(pNew);
		return NULL;
	}
	return pNew;
}
XNtSecAce *XNtSecAcl::Add(SID *pSID, LPCTSTR lpszServerOrDoamin)
{
	// 新しいオブジェクトの作成
	XNtSecAce *pNew = (XNtSecAce*)_New();
	if(pNew == NULL)
	{
		::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}
	try
	{
		if( pNew->SetAccount(pSID, lpszServerOrDoamin) == false )
		{
			DWORD rc = ::GetLastError();
			delete pNew;
			::SetLastError(rc);
			return NULL;
		}
		pNew->m_accessMode    = GRANT_ACCESS;
		_SetArray(pNew);
	}
	catch(...)
	{
		::SetLastError(ERROR_INVALID_DATA);
		_Delete(pNew);
		return NULL;
	}
	return pNew;
}
XNtSecAce *XNtSecAcl::operator[] (int nIndex) const
{
	return (XNtSecAce*)m_array[nIndex];
}
int XNtSecAcl::Count()
{
	return m_array.GetSize();
}
void *XNtSecAcl::_New()
{
	XNtSecAce *p = new XNtSecAce;
	return p;
}
void XNtSecAcl::_Delete(void *pDeleteData)
{
	XNtSecAce *p = (XNtSecAce*)pDeleteData;
	delete p;
}
void XNtSecAcl::_SetArray(void *pNewData)
{
	m_array.Add(pNewData);
}
bool XNtSecAcl::_NewACL(bool bInheritCopy, ACL *pOldACL, ACL **ppNewACL)
{
	ACL                 *pDest = NULL;
	try
	{
		long                nCount;
		DWORD               dwError;
		EXPLICIT_ACCESS		*pobACLExplicitAccess = NULL;
		EXPLICIT_ACCESS		*pMbExplicitAccess = NULL;
		XBuff               buffExplicitAccess;
		
		*ppNewACL = NULL;


		nCount = _NewAclCount(bInheritCopy);
		if(nCount)
		{
			if( buffExplicitAccess.Alloc(sizeof(EXPLICIT_ACCESS)*nCount) == NULL )
			{
				return false;
			}
			pMbExplicitAccess = pobACLExplicitAccess = (EXPLICIT_ACCESS*)(void*)buffExplicitAccess;

			for(int i = 0; i < m_array.GetSize(); i++)
			{
				XNtSecAce       *pACE;
				long            nAccessMask;
				long            nInherited;
				ACCESS_MODE     nAccessMode;
				SID             *pSID;


				pACE = (XNtSecAce*)m_array[i];

				// アクセスマスク
				nAccessMask = pACE->get_AccessMask();
				// 継承フラグ
				nInherited = pACE->get_Inheritance();
				// アクセスモード
				nAccessMode = pACE->get_AccessMode();
				// SID
				pSID = pACE->GetSID();

				if(nInherited & INHERITED_ACE )
				{
					if(bInheritCopy == VARIANT_FALSE)
						continue;
					nInherited &= ~INHERITED_ACE;
				}
#ifdef _DEBUG
				XStrBuff   strTrusteeName;
				strTrusteeName = pACE->TrusteeName();
				XTRACE(_T("ACE Info   %s\n"), (LPCTSTR)strTrusteeName);
				XTRACE(_T("   AccessMode%04d  Inherit:%04d    AccessMask;%d\n"), (long)nAccessMode,(long)nInherited,(long)nAccessMask);
#endif
				// アクセスモードがGRANTの場合は、SETに変更
				if( nAccessMode == GRANT_ACCESS )
					nAccessMode = SET_ACCESS;

				//呼び出し側が指定したデータを使って、EXPLICIT_ACCESS 構造体を初期化します。
				// 名前文字列を使って、トラスティを識別します。
				::BuildExplicitAccessWithName(
						pMbExplicitAccess,
						(LPTSTR)_T(""),
						nAccessMask,
						(ACCESS_MODE)nAccessMode,
						nInherited);
				// TrusteeのタイプをSIDに設定
				pMbExplicitAccess->Trustee.TrusteeForm = TRUSTEE_IS_SID;
				pMbExplicitAccess->Trustee.ptstrName  = (LPTSTR)pSID;

				pMbExplicitAccess++;
			}
		}
		// 新しいアクセス制御情報または新しい監査制御情報を既存の ACL（アクセス制御リスト）
		// に結合することにより、新しい ACL を作成します。
		if(bInheritCopy == ((VARIANT_TRUE) ? true : false) )
		{
			dwError = ::SetEntriesInAcl(
						 nCount,
						 pobACLExplicitAccess,
						 pOldACL,
						 &pDest);
		}
		else
		{
			dwError = ::SetEntriesInAcl(
						 nCount,
						 pobACLExplicitAccess,
						 NULL,
						 &pDest);
		}

		if( dwError != NO_ERROR){throw (DWORD)dwError;}

		// ACL の妥当性をチェック
		if( !::IsValidAcl(pDest) ){	throw (DWORD)::GetLastError(); }

		// 新しいACLのサイズは？
		ACL_SIZE_INFORMATION   aclSizeInfo;
		if( ::GetAclInformation(pDest, &aclSizeInfo, sizeof(aclSizeInfo), AclSizeInformation) == FALSE ){	throw (DWORD)::GetLastError(); }

#ifdef _DEBUG
		XTRACE(_T("\n   ACL Size Info\n"));
		XTRACE(_T("       Count :%d      BytesInUse :%d       BytesFree :%d\n"), aclSizeInfo.AceCount, aclSizeInfo.AclBytesInUse , aclSizeInfo.AclBytesFree );
#endif
		if( m_buffACL.Alloc(aclSizeInfo.AclBytesInUse) == NULL )
		{
			DWORD rc = ::GetLastError();
			::LocalFree(pDest);
			throw (DWORD)rc;
		}
		memcpy(m_buffACL, pDest, aclSizeInfo.AclBytesInUse);
		::LocalFree(pDest);
		*ppNewACL = (ACL*)(void*)m_buffACL;
		pDest = NULL;
	}
	catch(DWORD dwErr)
	{
		if(pDest != NULL)
			::LocalFree(pDest);
		::SetLastError(dwErr);
		return false;
	}
	catch(...)
	{
		if(pDest != NULL)
			::LocalFree(pDest);
		::SetLastError(ERROR_INVALID_DATA);
		return false;
	}
	return true;
}
bool XNtSecAcl::Fill(PACL pAcl, bool bInherit, LPCTSTR lpszServer)
{
	DWORD                dwError = NO_ERROR;
	ULONG                cCountOfExplicitEntries, ul;
	PEXPLICIT_ACCESS	 pExplicitAccess = NULL;


	try
	{
		this->Clear();

		if(bInherit == false)
		{
			dwError = ::GetExplicitEntriesFromAcl(pAcl, &cCountOfExplicitEntries, &pExplicitAccess);
			if( dwError != NO_ERROR )				throw dwError;
			for( ul = 0; ul < cCountOfExplicitEntries; ul++)
			{
				if( _Set2ExplicitAccess(&pExplicitAccess[ul], lpszServer) == false ) 
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
			if(pExplicitAccess)
				::LocalFree(pExplicitAccess);
			pExplicitAccess = NULL;
		}
		else
		{
			void *pACE;
			cCountOfExplicitEntries = (ULONG)((PACL)pAcl)->AceCount;
			for( ul = 0; ul < (ULONG)cCountOfExplicitEntries; ul++)
			{
				if( !::GetAce(pAcl, ul, &pACE) )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
				if( _Set2AceHead((ACE_HEADER*)pACE, lpszServer) == false )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
	}
	catch(DWORD dwE)
	{
		if(pExplicitAccess)
			::LocalFree(pExplicitAccess);
		::SetLastError(dwE);
		return false;
	}
	return true;
}
bool XNtSecAcl::_Set2ExplicitAccess(EXPLICIT_ACCESS *pExplicitAccess ,LPCTSTR lpszServer)
{
	_ASSERT(pExplicitAccess);
	TRUSTEE			          *pTrustee = &pExplicitAccess->Trustee;
	XNtSecAce                 *pACEInterFace;

	try
	{
		XStrBuff      strServerOrDomain = lpszServer;
		if( pTrustee->TrusteeForm == TRUSTEE_IS_SID )
		{
			pACEInterFace = this->Add((SID*)pTrustee->ptstrName, lpszServer);
			if(pACEInterFace == NULL){return false;}
		}
		else if( pTrustee->TrusteeForm == TRUSTEE_IS_OBJECTS_AND_SID)
		{
			POBJECTS_AND_SID pObAndSID = (POBJECTS_AND_SID)pTrustee->ptstrName;
			pACEInterFace = this->Add((SID*)pObAndSID->pSid , strServerOrDomain);
			if(pACEInterFace == NULL){return false;}
					
			pACEInterFace->_SetACEObjectInfo(pObAndSID->ObjectsPresent, &pObAndSID->ObjectTypeGuid , &pObAndSID->InheritedObjectTypeGuid);
		}
		else
		{
			_ASSERT(false);
		}
		pACEInterFace->put_AccessMode((ACCESS_MODE)pExplicitAccess->grfAccessMode);
		pACEInterFace->put_AccessMask(pExplicitAccess->grfAccessPermissions);
		pACEInterFace->put_Inheritance(pExplicitAccess->grfInheritance);
	}
	catch( DWORD dwE )
	{
		::SetLastError(dwE);
		return false;
	}
	return true;
}
bool XNtSecAcl::_Set2AceHead(ACE_HEADER *pAce , LPCTSTR lpszServer)
{
	_ASSERT(pAce);
	ACCESS_MODE               dwAccessMode;
	DWORD                     dwInheritFlg;
	DWORD                     dwAccessMask;
	XNtSecAce                 *pACEInterFace;
	PSID				      pSID;
	GUID                      gidObjectType, gidInheritedObjectType;
	long                      dwFlg;
	bool                      bIsAceObject = false;
	XStrBuff                  strServerOrDomain = lpszServer;

	dwInheritFlg = pAce->AceFlags;
	switch( pAce->AceType )
	{
	case ACCESS_ALLOWED_ACE_TYPE:
		{
			PACCESS_ALLOWED_ACE pacc;
			dwAccessMode = GRANT_ACCESS;
			pacc = (PACCESS_ALLOWED_ACE)pAce;
			dwAccessMask = pacc->Mask;
			pSID = &(pacc->SidStart);
		}
		break;
	case ACCESS_DENIED_ACE_TYPE:
		{
			PACCESS_DENIED_ACE pacc;
			dwAccessMode = DENY_ACCESS;
			pacc = (PACCESS_DENIED_ACE)pAce;
			dwAccessMask = pacc->Mask;
			pSID = &(pacc->SidStart);
		}
		break;
	case ACCESS_ALLOWED_OBJECT_ACE_TYPE:
		{
			PACCESS_ALLOWED_OBJECT_ACE pacc;
			dwAccessMode = GRANT_ACCESS;
			pacc = (PACCESS_ALLOWED_OBJECT_ACE)pAce;
			dwAccessMask = pacc->Mask;
			if( pacc->Flags & ACE_OBJECT_TYPE_PRESENT) 
				::CopyMemory(&gidObjectType, &pacc->ObjectType, sizeof(GUID));
			if( pacc->Flags & ACE_INHERITED_OBJECT_TYPE_PRESENT) 
				::CopyMemory(&gidInheritedObjectType, &pacc->InheritedObjectType, sizeof(GUID));
			dwFlg = pacc->Flags;
			if( pacc->Flags == 0 )
				pSID = &(pacc->ObjectType);
			else if( (pacc->Flags & (ACE_OBJECT_TYPE_PRESENT | ACE_INHERITED_OBJECT_TYPE_PRESENT)) == (ACE_OBJECT_TYPE_PRESENT | ACE_INHERITED_OBJECT_TYPE_PRESENT) )
				pSID = &(pacc->SidStart);
			else
				pSID = &(pacc->InheritedObjectType);

			bIsAceObject = true;
		}
		break;
	case ACCESS_DENIED_OBJECT_ACE_TYPE:
		{
			PACCESS_DENIED_OBJECT_ACE pacc;
			dwAccessMode = DENY_ACCESS;
			pacc = (PACCESS_DENIED_OBJECT_ACE)pAce;
			dwAccessMask = pacc->Mask;
			if( pacc->Flags & ACE_OBJECT_TYPE_PRESENT) 
				::CopyMemory(&gidObjectType, &pacc->ObjectType, sizeof(GUID));
			if( pacc->Flags & ACE_INHERITED_OBJECT_TYPE_PRESENT) 
				::CopyMemory(&gidInheritedObjectType, &pacc->InheritedObjectType, sizeof(GUID));
			dwFlg = pacc->Flags;
			if( pacc->Flags == 0 )
				pSID = &(pacc->ObjectType);
			else if( (pacc->Flags & (ACE_OBJECT_TYPE_PRESENT | ACE_INHERITED_OBJECT_TYPE_PRESENT)) == (ACE_OBJECT_TYPE_PRESENT | ACE_INHERITED_OBJECT_TYPE_PRESENT) )
				pSID = &(pacc->SidStart);
			else
				pSID = &(pacc->InheritedObjectType);
			bIsAceObject = true;
		}
		break;
	case SYSTEM_AUDIT_ACE_TYPE:
	case SYSTEM_AUDIT_OBJECT_ACE_TYPE:
	default:
		_ASSERT(false);
	}

	pACEInterFace = this->Add((SID*)pSID, strServerOrDomain);
	if( pACEInterFace == NULL)
	{
		return false;
	}

	pACEInterFace->put_AccessMode(dwAccessMode);
	pACEInterFace->put_AccessMask(dwAccessMask);
	pACEInterFace->put_Inheritance(dwInheritFlg);

	if(bIsAceObject == true)
	{
		pACEInterFace->_SetACEObjectInfo(dwFlg, &gidObjectType, &gidInheritedObjectType);
	}
	return true;
}
DWORD XNtSecAcl::_NewAclCount(bool bInheritCopy)
{
	int iCount = 0;
	int i = 0;

	for(; i < m_array.GetSize(); i++)
	{
		XNtSecAce *p = (XNtSecAce*)m_array[i];
		_ASSERT(p);
		if( p->IsIsInherited() == false || (bInheritCopy == true) )
		{
			iCount++;
		}
	}
	return iCount;
}
//////////////////////////////////////////////////////////////////////////////////////
//
//	XNtSecDescriptor
//
XNtSecDescriptor::XNtSecDescriptor(XNtSecAcl* pDaclBuff, XNtSecAcl* pSaclBuff)
	:	m_psd( NULL )
	,	m_ppsidOwner( NULL )
	,	m_ppsidGroup( NULL )
	,	m_ppDacl( NULL )
	,	m_ppSacl( NULL )
	,	m_tyObject( SE_UNKNOWN_OBJECT_TYPE )
	,	m_dwOpendSecurityFlag( 0 )
	,	m_DACL( pDaclBuff )
	,	m_SACL( pSaclBuff )
{
	m_secCtrl.m_nCtrl   = (SECURITY_DESCRIPTOR_CONTROL)0;
	m_secCtrl.m_dwRevisuin = 0;
}
XNtSecDescriptor::~XNtSecDescriptor()
{
	Free();
}
void XNtSecDescriptor::Free()
{
	if(m_psd)
	{
		::LocalFree(m_psd);
	}
	m_psd = NULL;
	m_ppsidOwner = NULL;
	m_ppsidGroup = NULL;
	m_ppDacl = NULL;
	m_ppSacl = NULL;
	m_psd = NULL;
	m_tyObject = SE_UNKNOWN_OBJECT_TYPE;
	m_dwOpendSecurityFlag = 0;
	m_secCtrl.m_nCtrl   = (SECURITY_DESCRIPTOR_CONTROL)0;
	m_secCtrl.m_dwRevisuin = 0;
	if( m_DACL != NULL ) {
		m_DACL->Clear();
	}
	if( m_SACL != NULL ) {
		m_SACL->Clear();
	}
}
SE_OBJECT_TYPE XNtSecDescriptor::ObjectType(){return m_tyObject;}
LPCTSTR XNtSecDescriptor::ObjectName(){return m_strObjectName;}
XNtSecAcl *XNtSecDescriptor::raw_GetDACL(){return m_DACL;}
XNtSecAcl *XNtSecDescriptor::raw_GetSACL(){return m_SACL;}
XNtSecAccount *XNtSecDescriptor::GetOwner(){return &m_Owner;}
XNtSecAccount *XNtSecDescriptor::GetGroup(){return &m_Group;}
SECURITY_DESCRIPTOR_CONTROL XNtSecDescriptor::GetSecCtrl(){return m_secCtrl.m_nCtrl;}
bool XNtSecDescriptor::Open(LPCTSTR lpszObjectName, SE_OBJECT_TYPE nOblectType, 
										long nSecurityInfo, 
										bool bInherit, 
										LPCTSTR lpszServerOrDomain/* = NULL*/)
{
	try
	{
		DWORD              dwError   = NO_ERROR;
		PTOKEN_PRIVILEGES  pOldToken = NULL;
		XBuff              buffOldToken;

		Free();

		// SACLを取得するには、SE_PRIVILEGE_ENABLEDの特権を有効にします。
		if(nSecurityInfo & SACL_SECURITY_INFORMATION)
		{
			dwError = this->PrivilegeEnableGetSACL(true, lpszServerOrDomain, &pOldToken, buffOldToken);
			if( dwError != ERROR_SUCCESS )         throw (DWORD)dwError;
		}

		// セキュリティのオープン
		dwError = ::GetNamedSecurityInfo(
					 (LPTSTR)(LPCTSTR)lpszObjectName,
					 (SE_OBJECT_TYPE)nOblectType,
					 nSecurityInfo,
					 &m_ppsidOwner,
					 &m_ppsidGroup,
					 &m_ppDacl,
					 &m_ppSacl,
					 &m_psd );
		if(nSecurityInfo & SACL_SECURITY_INFORMATION)
		{
			this->PrivilegeEnableGetSACL(false, lpszServerOrDomain, &pOldToken, buffOldToken);
		}
		if( dwError != ERROR_SUCCESS )         throw (DWORD)dwError;



		// オープンされたセキュリティ情報フラグにDACL情報が含まれている場合はDACL
		// の情報をセットします。
		if(nSecurityInfo & DACL_SECURITY_INFORMATION)
		{
			if( m_ppDacl != NULL ) {
				if(m_DACL->Fill(m_ppDacl, bInherit, lpszServerOrDomain) == false )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
		// オープンされたセキュリティ情報フラグにSACL情報が含まれている場合はSACL
		// の情報をセットします。
		if(nSecurityInfo & SACL_SECURITY_INFORMATION)
		{
			if( m_ppSacl != NULL ) {
				if(m_SACL->Fill(m_ppSacl, bInherit, lpszServerOrDomain) == false )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
		// オープンされたセキュリティ情報フラグにOWNER情報が含まれている場合はOWNER
		// の情報をセットします。
		if(nSecurityInfo & OWNER_SECURITY_INFORMATION)
		{
			if( m_ppsidOwner != NULL ) {
				if( m_Owner.SetAccount((SID*)m_ppsidOwner, lpszServerOrDomain) == false  )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
		// オープンされたセキュリティ情報フラグにGROUP情報が含まれている場合はGROUP
		// の情報をセットします。
		if(nSecurityInfo & GROUP_SECURITY_INFORMATION)
		{
			if( m_ppsidGroup != NULL ) {
				if( m_Group.SetAccount((SID*)m_ppsidGroup, lpszServerOrDomain) == false  )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
		// セキュリティ記述子の制御情報とリビジョン情報を取得します。
		if(m_psd)
		{
			if( !::GetSecurityDescriptorControl(m_psd, &m_secCtrl.m_nCtrl, &m_secCtrl.m_dwRevisuin) )
			{
				dwError = ::GetLastError();
				if(dwError != NO_ERROR)				throw (DWORD)dwError;
			}
		}

	}
	catch(DWORD dwErr)
	{
		::SetLastError(dwErr);
		return false;
	}
	catch(...)
	{
		::SetLastError(ERROR_INVALID_DATA);
		return false;
	}
	return true;
}
bool XNtSecDescriptor::Open(HANDLE hHangle,
					SE_OBJECT_TYPE nOblectType, 
					long nSecurityInfo /*= (DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION)*/,
					bool bInherit /*= TRUE*/,
					LPCTSTR lpszServerOrDomain /*= NULL*/)
{
	try
	{
		DWORD              dwError   = NO_ERROR;
		PTOKEN_PRIVILEGES  pOldToken = NULL;
		XBuff              buffOldToken;

		Free();

		// SACLを取得するには、SE_PRIVILEGE_ENABLEDの特権を有効にします。
		if(nSecurityInfo & SACL_SECURITY_INFORMATION)
		{
			dwError = this->PrivilegeEnableGetSACL(true, lpszServerOrDomain, &pOldToken, buffOldToken);
			if( dwError != ERROR_SUCCESS )         throw (DWORD)dwError;
		}
		// セキュリティのオープン
		dwError = ::GetSecurityInfo(hHangle, 
					 (SE_OBJECT_TYPE)nOblectType,
					 nSecurityInfo,
					 &m_ppsidOwner,
					 &m_ppsidGroup,
					 &m_ppDacl,
					 &m_ppSacl,
					 &m_psd );

		if(nSecurityInfo & SACL_SECURITY_INFORMATION)
		{
			this->PrivilegeEnableGetSACL(false, lpszServerOrDomain, &pOldToken, buffOldToken);
		}
		if( dwError != ERROR_SUCCESS )         throw (DWORD)dwError;



		// オープンされたセキュリティ情報フラグにDACL情報が含まれている場合はDACL
		// の情報をセットします。
		if(nSecurityInfo & DACL_SECURITY_INFORMATION)
		{
			if( m_ppDacl != NULL ) {
				if(m_DACL->Fill(m_ppDacl, bInherit, lpszServerOrDomain) == false )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
		// オープンされたセキュリティ情報フラグにSACL情報が含まれている場合はSACL
		// の情報をセットします。
		if(nSecurityInfo & SACL_SECURITY_INFORMATION)
		{
			if( m_ppSacl != NULL ) {
				if(m_SACL->Fill(m_ppSacl, bInherit, lpszServerOrDomain) == false )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
		// オープンされたセキュリティ情報フラグにOWNER情報が含まれている場合はOWNER
		// の情報をセットします。
		if(nSecurityInfo & OWNER_SECURITY_INFORMATION)
		{
			if( m_ppsidOwner != NULL ) {
				if( m_Owner.SetAccount((SID*)m_ppsidOwner, lpszServerOrDomain) == false  )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
		// オープンされたセキュリティ情報フラグにGROUP情報が含まれている場合はGROUP
		// の情報をセットします。
		if(nSecurityInfo & GROUP_SECURITY_INFORMATION)
		{
			if( m_ppsidGroup != NULL ) {
				if( m_Group.SetAccount((SID*)m_ppsidGroup, lpszServerOrDomain) == false  )
				{
					dwError = ::GetLastError();
					throw dwError;
				}
			}
		}
		// セキュリティ記述子の制御情報とリビジョン情報を取得します。
		if(m_psd)
		{
			if( !::GetSecurityDescriptorControl(m_psd, &m_secCtrl.m_nCtrl, &m_secCtrl.m_dwRevisuin) )
			{
				dwError = ::GetLastError();
				if(dwError != NO_ERROR)				throw (DWORD)dwError;
			}
		}

	}
	catch(DWORD dwErr)
	{
		::SetLastError(dwErr);
		return false;
	}
	catch(...)
	{
		::SetLastError(ERROR_INVALID_DATA);
		return false;
	}
	return true;
}
DWORD XNtSecDescriptor::PrivilegeEnableGetSACL(bool bEnable, 
											   LPCTSTR lpszServerOrDomain, 
											   PTOKEN_PRIVILEGES *pOldToken, 
											   XBuff &buffOldToken)
{
	TOKEN_PRIVILEGES tkp;
	HANDLE           hToken;              // handle to process token 
	DWORD            dwSize, dwErr;
	
	// プロセスのトークンを取得します
	if (!::OpenProcessToken(::GetCurrentProcess(), 	TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return ::GetLastError();
    
	// 
	::LookupPrivilegeValue(lpszServerOrDomain, SE_SECURITY_NAME, &tkp.Privileges[0].Luid); 
 
	if(bEnable == true)
		tkp.PrivilegeCount = 1;  // one privilege to set    
	else
		tkp.PrivilegeCount = 0;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
	if(bEnable)
	{
		::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, &dwSize);
		dwErr = ::GetLastError();
		
		if( buffOldToken.Alloc(dwSize) != NULL)
		{
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		*pOldToken = (PTOKEN_PRIVILEGES)(void*)buffOldToken;
		::AdjustTokenPrivileges(hToken, FALSE, &tkp, dwSize, (PTOKEN_PRIVILEGES)*pOldToken, &dwSize);
	}
	else
	{
		::AdjustTokenPrivileges(hToken, FALSE, *pOldToken, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	}
 	if(::GetLastError() != ERROR_SUCCESS) 
		return ::GetLastError();
	return NO_ERROR;
}
bool XNtSecDescriptor::Update(long nSecurityInfo, DWORD dwUpdateOptions)
{
	PACL				pNewDACL = NULL;
	PACL				pNewSACL = NULL;
	SID*				pOwner = NULL;
	SID*				pGroup = NULL;
	try
	{
		HRESULT             hr;
		DWORD				dwError = S_OK;

		if( (m_dwStatus & STATUS_OPEND) == false)
		{
			throw (DWORD)ERROR_INVALID_PARAMETER;
		}

		// セキュリティを設定するバイナリデータを取得する
		hr = _GetSecurityDescriptorData(nSecurityInfo, &pNewDACL, &pNewSACL, &pOwner, &pGroup);
		if( FAILED(hr) )		return false;

		if(pNewDACL)
		{
			// ACL の妥当性をチェック
			if( !::IsValidAcl(pNewDACL) )		throw (DWORD)::GetLastError();
		}
		if(pNewSACL)
		{
			// ACL の妥当性をチェック
			if( !::IsValidAcl(pNewSACL) )		throw (DWORD)::GetLastError();
		}
		if(pOwner)
		{
			// SIDの妥当性をチェック
			if( ::IsValidSid(pOwner) == FALSE )	throw (DWORD)::GetLastError();
		}
		if(pGroup)
		{
			// SIDの妥当性をチェック
			if( ::IsValidSid(pGroup) == FALSE )	throw (DWORD)::GetLastError();
		}

		dwError = ::SetNamedSecurityInfo(
					 m_strObjectName,
					 (SE_OBJECT_TYPE)m_tyObject, // object type
					 nSecurityInfo,
					 pOwner,
					 pGroup,
					 pNewDACL,
					 pNewSACL);
		if(dwError != NO_ERROR) 
			throw dwError;
	}
	catch(DWORD dwE)
	{
		::SetLastError(dwE);
		return false;
	}
	return true;
}
bool XNtSecDescriptor::_GetSecurityDescriptorData(long nSecurityInfo, ACL **pDACL, ACL **pSACL, SID **pOwner, SID **pGroup)
{
	try
	{
		DWORD				dwError = NO_ERROR;

		if( (m_dwStatus & STATUS_OPEND) == false)
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			return false;
		}


		if(nSecurityInfo & DACL_SECURITY_INFORMATION)
		{
			if(!(m_dwOpendSecurityFlag & DACL_SECURITY_INFORMATION))
			{
				::SetLastError(ERROR_INVALID_PARAMETER);
				return false;
			}
			if( m_DACL->_NewACL((m_dwStatus & STATUS_INHERIT_DACL_COPY) ? true : false, m_ppDacl, pDACL) == false )
			{
				return false;
			}
		}
		if(nSecurityInfo & SACL_SECURITY_INFORMATION)
		{
			if(!(m_dwOpendSecurityFlag & SACL_SECURITY_INFORMATION))
			{
				::SetLastError(ERROR_INVALID_PARAMETER);
				return false;
			}
			if( m_SACL->_NewACL((m_dwStatus & STATUS_INHERIT_SACL_COPY) ? true : false, m_ppSacl, pSACL) == false )
			{
				return false;
			}
		}

		if(nSecurityInfo & OWNER_SECURITY_INFORMATION)
		{
			if(!(m_dwOpendSecurityFlag & OWNER_SECURITY_INFORMATION))
			{
				::SetLastError(ERROR_INVALID_PARAMETER);
				return false;
			}
			*pOwner  = m_Owner.GetSID();
		}

		if(nSecurityInfo & GROUP_SECURITY_INFORMATION)
		{
			if(!(m_dwOpendSecurityFlag & GROUP_SECURITY_INFORMATION))
			{
				::SetLastError(ERROR_INVALID_PARAMETER);
				return false;
			}
			*pGroup  = m_Group.GetSID();
		}

	}
	catch(DWORD dwE)
	{
		::SetLastError(dwE);
		return false;
	}
	return true;
}
//########################################################################################################
//
//	File Security
//
//########################################################################################################
//////////////////////////////////////////////////////////////////////////////////////
//
//	XFileSecAce
//
XFileSecAce::XFileSecAce(){}
XFileSecAce::~XFileSecAce(){}
bool XFileSecAce::get_enumFolderExecFile(){	return get_rawAccessMaskOr(enumFolderExecFile);}
void XFileSecAce::put_enumFolderExecFile(bool newVal){	put_rawAccessMaskOr(enumFolderExecFile, newVal);}
bool XFileSecAce::get_createFileWriteData(){	return get_rawAccessMaskOr(createFileWriteData);}
void XFileSecAce::put_createFileWriteData(bool newVal){	put_rawAccessMaskOr(createFileWriteData, newVal);}
bool XFileSecAce::get_createFolderAddData(){	return get_rawAccessMaskOr(createFolderAddData);}
void XFileSecAce::put_createFolderAddData(bool newVal){	put_rawAccessMaskOr(createFolderAddData, newVal);}
bool XFileSecAce::get_readExAtribute(){	return get_rawAccessMaskOr(readExAtribute);}
void XFileSecAce::put_readExAtribute(bool newVal){	put_rawAccessMaskOr(readExAtribute, newVal);}
bool XFileSecAce::get_writExAtribute(){	return get_rawAccessMaskOr(writExAtribute);}
void XFileSecAce::put_writExAtribute(bool newVal){	put_rawAccessMaskOr(writExAtribute, newVal);}
bool XFileSecAce::get_scanFolderExecFile(){	return get_rawAccessMaskOr(scanFolderExecFile);}
void XFileSecAce::put_scanFolderExecFile(bool newVal){	put_rawAccessMaskOr(scanFolderExecFile, newVal);}
bool XFileSecAce::get_deleteSubFolderFile(){	return get_rawAccessMaskOr(deleteSubFolderFile);}
void XFileSecAce::put_deleteSubFolderFile(bool newVal){	put_rawAccessMaskOr(deleteSubFolderFile, newVal);}
bool XFileSecAce::get_readAtribute(){	return get_rawAccessMaskOr(readAtribute);}
void XFileSecAce::put_readAtribute(bool newVal){	put_rawAccessMaskOr(readAtribute, newVal);}
bool XFileSecAce::get_writAtribute(){	return get_rawAccessMaskOr(writAtribute);}
void XFileSecAce::put_writAtribute(bool newVal){	put_rawAccessMaskOr(writAtribute, newVal);}
//////////////////////////////////////////////////////////////////////////////////////
//
//	XFileSecAcl
//
XFileSecAcl::XFileSecAcl()
{
}
XFileSecAcl::~XFileSecAcl()
{
}
void XFileSecAcl::Clear()
{
	for(int i = 0; i < m_array.GetSize(); i++)
	{
		XFileSecAce *p = (XFileSecAce*)m_array[i];
		delete p;
	}
	m_array.RemoveAll();
}
XFileSecAce *XFileSecAcl::GetAt(int nIndex)
{
	XFileSecAce *p = (XFileSecAce*)m_array[nIndex];
	return p;
}
void *XFileSecAcl::_New()
{
	XFileSecAce *p = new XFileSecAce;
	return p;
}
void XFileSecAcl::_Delete(void *pDeleteData)
{
	XFileSecAce *p = (XFileSecAce*)pDeleteData;
	delete p;
}
void XFileSecAcl::_SetArray(void* pNewData)
{
	m_array.Add(pNewData);
}
//////////////////////////////////////////////////////////////////////////////////////
//
//	XFileSecDescriptor
//
XFileSecDescriptor::XFileSecDescriptor()
	:	XNtSecDescriptor((XNtSecAcl*)&m_buffDACL, (XNtSecAcl*)&m_buffSACL)
{
	m_DACL = (XNtSecAcl*)&m_buffDACL;
	m_SACL = (XNtSecAcl*)&m_buffSACL;
}
XFileSecDescriptor::~XFileSecDescriptor()
{
}
XFileSecAcl *XFileSecDescriptor::GetDACL()
{
	return &m_buffDACL;
}
XFileSecAcl *XFileSecDescriptor::GetSACL()
{
	return &m_buffSACL;
}
bool XFileSecDescriptor::Open(LPCTSTR lpszFileName, 
				long nSecurityInfo, 
				bool bInherit, 
				LPCTSTR lpszServerOrDomain/* = NULL*/)
{
	return XNtSecDescriptor::Open(lpszFileName, (SE_OBJECT_TYPE)SE_FILE_OBJECT, nSecurityInfo, bInherit, lpszServerOrDomain);
}
bool XFileSecDescriptor::Update(long nSecurityInfo, DWORD dwUpdateOptions/* = 0*/)
{

	return true;
}



//////////////////////////////////////////////////////////////////////////////////////
//
//	Local Function Implement
//
DWORD GetSidFromString(PSID pSID, LPTSTR lpszBuff)
{
	DWORD dwSubAuthorities;
	PSID_IDENTIFIER_AUTHORITY psia;
	DWORD dwSidRev=SID_REVISION;
	LPTSTR lpsz = lpszBuff;

	if( !IsValidSid(pSID) )
		return (DWORD)::GetLastError();
	
	wsprintf(lpsz, _T("S-%lu-"), dwSidRev );


	// SidIdentifierAuthority を得る
	psia = ::GetSidIdentifierAuthority(pSID);

	// sidsubauthority カウントを得る
	dwSubAuthorities= *GetSidSubAuthorityCount(pSID);

	// SidIdentifierAuthority の準備
	//
	if ( (psia->Value[0] != 0) || (psia->Value[1] != 0) )
	{
	  wsprintf(lpsz + lstrlen(lpsz),
				  _T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
				  (USHORT)psia->Value[0],
				  (USHORT)psia->Value[1],
				  (USHORT)psia->Value[2],
				  (USHORT)psia->Value[3],
				  (USHORT)psia->Value[4],
				  (USHORT)psia->Value[5]);
	}
	else
	{
	  wsprintf(lpsz + lstrlen(lpsz),
				  _T("%lu"),
				  (ULONG)(psia->Value[5]      )   +
				  (ULONG)(psia->Value[4] <<  8)   +
				  (ULONG)(psia->Value[3] << 16)   +
				  (ULONG)(psia->Value[2] << 24)   );
	}

	for (DWORD dwCounter=0 ; dwCounter < dwSubAuthorities ; dwCounter++)
	{
		wsprintf(lpsz + lstrlen(lpsz), _T("-%lu"), *GetSidSubAuthority(pSID, dwCounter));
	}

	return NO_ERROR;
}
PSID GetStringFromSid(LPCTSTR lpszStringSID)
{
	PSID						pSid = NULL;
    SID_IDENTIFIER_AUTHORITY	identAuthority;
    TCHAR						buffer[1024];
    int							i;
    LPTSTR						ptr, ptr1;
    BYTE						nByteAuthorityCount = 0;
    DWORD						dwSubAuthority[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    ::ZeroMemory(&identAuthority, sizeof(identAuthority));

    lstrcpy(buffer, lpszStringSID);

    // S-SID_REVISION- + identifierauthority- + subauthorities- + NULL

    // Skip S
    if (!(ptr = _tcschr(buffer, _T('-'))))
    {
		::SetLastError(ERROR_INVALID_DATA);
        return pSid;
    }

    // Skip -
    ptr++;

    // Skip SID_REVISION
    if (!(ptr = _tcschr(ptr, _T('-'))))
    {
		::SetLastError(ERROR_INVALID_DATA);
        return pSid;
    }

    // Skip -
    ptr++;

    // Skip identifierauthority
    if (!(ptr1 = _tcschr(ptr, _T('-'))))
    {
		::SetLastError(ERROR_INVALID_DATA);
        return pSid;
    }
    *ptr1= 0;

    if ((*ptr == '0') && (*(ptr+1) == 'x'))
    {
        _stscanf(ptr, _T("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
            &identAuthority.Value[0],
            &identAuthority.Value[1],
            &identAuthority.Value[2],
            &identAuthority.Value[3],
            &identAuthority.Value[4],
            &identAuthority.Value[5]);
    }
    else
    {
        DWORD value;

        _stscanf(ptr, _T("%lu"), &value);

        identAuthority.Value[5] = (BYTE)(value & 0x000000FF);
        identAuthority.Value[4] = (BYTE)(value & 0x0000FF00) >> 8;
        identAuthority.Value[3] = (BYTE)(value & 0x00FF0000) >> 16;
        identAuthority.Value[2] = (BYTE)(value & 0xFF000000) >> 24;
    }

    // Skip -
    *ptr1 = '-';
    ptr = ptr1;
    ptr1++;

    for (i = 0; i < 8; i++)
    {
        // get subauthority
        if (!(ptr = _tcschr(ptr, '-')))
        {
            break;
        }
        *ptr=0;
        ptr++;
        nByteAuthorityCount++;
    }

    for (i = 0; i < nByteAuthorityCount; i++)
    {
        // Get subauthority.
        _stscanf(ptr1, _T("%lu"), &dwSubAuthority[i]);
        ptr1 += lstrlen(ptr1) + 1;
    }

    if (!::AllocateAndInitializeSid(&identAuthority,
        nByteAuthorityCount,
        dwSubAuthority[0],
        dwSubAuthority[1],
        dwSubAuthority[2],
        dwSubAuthority[3],
        dwSubAuthority[4],
        dwSubAuthority[5],
        dwSubAuthority[6],
        dwSubAuthority[7],
        &pSid))
    {
        pSid = 0;
    }
	return pSid;
}

TRUSTEE_TYPE GetSidUseName2TrusteeType(SID_NAME_USE SidNameUse)
{
	TRUSTEE_TYPE trusteeType;
	switch(SidNameUse)
	{
	case SidTypeUser:
		trusteeType = TRUSTEE_IS_USER;
		break;
	case SidTypeGroup:
		trusteeType = TRUSTEE_IS_GROUP;
		break;
	case SidTypeDomain:
		trusteeType = TRUSTEE_IS_DOMAIN;
		break;
	case SidTypeAlias:
		trusteeType = TRUSTEE_IS_ALIAS;
		break;
	case SidTypeWellKnownGroup:
		trusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
		break;
	case SidTypeDeletedAccount:
		trusteeType = TRUSTEE_IS_DELETED;
		break;
	case SidTypeInvalid:
		trusteeType = TRUSTEE_IS_INVALID;
		break;
	case SidTypeComputer:
		trusteeType = (TRUSTEE_TYPE)8;/*TRUSTEE_IS_COMPUTER*/
		break;
	case SidTypeUnknown:
	default:
		trusteeType = TRUSTEE_IS_UNKNOWN;
		break;
	}
	return trusteeType;
}





#endif
