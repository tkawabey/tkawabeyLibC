/**
 * @file xWinService.h
 * @brief �T�[�r�X�R���g���[��
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_WIN_SERVICE_HEASER_FILE__
#define __X_WIN_SERVICE_HEASER_FILE__


#ifdef WIN32

  #ifdef __AFX_H__

  #else
    #include <windows.h>
  #endif
  #include <winsvc.h>
  #include "xlib/xstrbuff.h"
  #include "xlib/windows/xWinObject.h"

/**
 * SC�}�l�[�W���Ǘ�
 *
 */
class XWinSCManager : public XWinObject
{
public:
	/** �R���X�g���N�^  */
	XWinSCManager();
	/** �f�X�g���N�^ */
	virtual ~XWinSCManager();

	/**
	 * SC�}�l�[�W�����I�[�v�����܂�
	 * @param[in] dwSCAccess �I�[�v������A�N�Z�X�}���w�肵�܂��A
	 * @param[in] lpszMachine �I�[�v������悪�����[�g�}�V�[���̏ꍇ�A�}�V�[�������w�肵�܂��B���[�J���R���s���[�^��NULL���w�肵�܂�
	 * @param[in] lpszDataBase �I�[�v������f�[�^�x�[�X���w�肵�܂��B�f�t�H���g�f�[�^�x�[�X��NULL���w�肵�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Open(DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const char* lpszMachine = NULL, 
				const char* lpszDataBase = NULL);
	/** Open::OpenSCManager(DWORD,const char*, const char*) */
	bool Open(DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const wchar_t* lpszMachine = NULL, 
				const wchar_t* lpszDataBase = NULL);
	/** @copydoc  XWinObject::Close() */
	virtual void Close();

protected:
	/** SC�}�l�[�W���[���I�[�v�������}�V�[�� */
	XStrBuff	m_strMachine;
	/** SC�}�l�[�W���[���I�[�v������DB */
	XStrBuff	m_strDataBase;
	/* �A�N�Z�X���[�h */
	DWORD		m_dwAccessMode;
};

/**
 * ���W�X�g���A�N�Z�X�N���X
 */
class XWinServiceCtrl
{
public:
	/** �C�x���g���O�̃^�C�v */
	enum EVENT_KEY_PARENT { 
		/** �A�v���P�[�V�������O */
		App, 
		/** �Z�L�����e�B�[���O */
		Sec, 
		/** �V�X�e�����O */
		Sys
	};
	/** �R���X�g���N�^ */
	XWinServiceCtrl();
	/** �f�X�g���N�^ */
	virtual ~XWinServiceCtrl();

	/**
	 * �T�[�r�X�����擾
	 * @return �T�[�r�X��
	 */
	XStrBuff GetServiceName() const;
	/**
	 * ���[�U�[��`�̃G���[�R�[�h���擾���܂�
	 * @return ���[�U�[��`�̃G���[�R�[�h
	 */
	DWORD GetServiceSpecificExitCode() const;
	/**
	 * �I�[�v�����ꂽSC�}�l�[�W���̃n���h�����擾���܂��B
	 * @return �I�[�v�����ꂽSC�}�l�[�W���̃n���h��
	 */
	HANDLE GetSCManagerHandle();

	/**
	 * SC�}�l�[�W�����I�[�v�����܂�
	 * @param[in] dwSCAccess �I�[�v������A�N�Z�X�}���w�肵�܂��A
	 * @param[in] lpszMachine �I�[�v������悪�����[�g�}�V�[���̏ꍇ�A�}�V�[�������w�肵�܂��B���[�J���R���s���[�^��NULL���w�肵�܂�
	 * @param[in] lpszDataBase �I�[�v������f�[�^�x�[�X���w�肵�܂��B�f�t�H���g�f�[�^�x�[�X��NULL���w�肵�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool OpenSCManager(DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const char* lpszMachine = NULL, 
				const char* lpszDataBase = NULL);
	/** XWinServiceCtrl::OpenSCManager(DWORD,const char*, const char*) */
	bool OpenSCManager(DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const wchar_t* lpszMachine = NULL, 
				const wchar_t* lpszDataBase = NULL);

	/**
	 * �T�[�r�X���I�[�v�����܂��B
	 * @param[in] lpszServiceName �I�[�v������T�[�r�X��
	 * @param[in] dwAccess �T�[�r�X�̃I�[�v������A�N�Z�X�}���w�肵�܂��A
	 * @param[in] dwSCAccess SC�}�l�[�W���̃I�[�v������A�N�Z�X�}���w�肵�܂��A
	 * @param[in] lpszMachine SC�}�l�[�W���̃I�[�v������悪�����[�g�}�V�[���̏ꍇ�A�}�V�[�������w�肵�܂��B���[�J���R���s���[�^��NULL���w�肵�܂�
	 * @param[in] lpszDataBase SC�}�l�[�W���̃I�[�v������f�[�^�x�[�X���w�肵�܂��B�f�t�H���g�f�[�^�x�[�X��NULL���w�肵�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Open(	const char* lpszServiceName,
		        DWORD dwAccess = SERVICE_ALL_ACCESS ,
				DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const char* lpszMachine = NULL, 
				const char* lpszDataBase = NULL);
	/** XWinServiceCtrl::Open(const char*,DWORD,DWORD,const char*, const char*) */
	bool Open(	const wchar_t* lpszServiceName,
		        DWORD dwAccess = SERVICE_ALL_ACCESS ,
				DWORD dwSCAccess =  SC_MANAGER_ALL_ACCESS | GENERIC_WRITE | GENERIC_READ ,
				const wchar_t* lpszMachine = NULL, 
				const wchar_t* lpszDataBase = NULL);
	/**
	 * �T�[�r���쐬���܂��B
	 * @note �쐬����O��SC�}�l�[�W�����I�[�v������K�v���L��܂�
	 *       �e�ڍׂȃp�����[�^�̐����́AMSDN�FCreateService���Q��
	 * @param[in] lpszName �T�[�r�X��
	 * @param[in] lpszDisplayName �T�[�r�X�̕\����
	 * @param[in] lpszImage �T�[�r�X�̎��s�C���[�W�̃t���p�X
	 * @param[in] dwAccess �T�[�r�X�̃I�[�v������A�N�Z�X�}���w�肵�܂�
	 * @param[in] dwServiceType �T�[�r�X�̎��
	 * @param[in] dwStart �T�[�r�X�̋N�����[�h
	 * @param[in] dwErrorControl �G���[���̏���
	 * @param[in] lpszOrderGroup �I�[�_�[�O���[�v
	 * @param[in] lpszDependencies �ˑ�����T�[�r�X
	 * @param[in] lpszServiceAccount �N�����̃A�J�E���g�ANULL�̓��[�J���V�X�e���A�J�E���g
	 * @param[in] lpszPasswd lpszServiceAccount�Ŏw�肵���A�J�E���g�̃p�X���[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Create(const char* lpszName,
						const char* lpszDisplayName ,
						const char* lpszImage,
						DWORD dwAccess = SERVICE_ALL_ACCESS, 
						DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS,			
						DWORD dwStart = SERVICE_AUTO_START , 
						DWORD dwErrorControl = SERVICE_ERROR_NORMAL ,
						const char* lpszOrderGroup = NULL, 
						const char* lpszDependencies = NULL, 
						const char* lpszServiceAccount = NULL,
						const char* lpszPasswd = NULL);
	/** XWinServiceCtrl::Create(const char*,const char*,const char*,DWORD, DWORD ,DWORD ,DWORD ,const char* ,const char* ,const char* ,const char*) */
	bool Create(const wchar_t* lpszName,
						const wchar_t* lpszDisplayName ,
						const wchar_t* lpszImage,
						DWORD dwAccess = SERVICE_ALL_ACCESS, 
						DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS,			
						DWORD dwStart = SERVICE_AUTO_START , 
						DWORD dwErrorControl = SERVICE_ERROR_NORMAL ,
						const wchar_t* lpszOrderGroup = NULL, 
						const wchar_t* lpszDependencies = NULL, 
						const wchar_t* lpszServiceAccount = NULL,
						const wchar_t* lpszPasswd = NULL);
	/**
	 * �T�[�r���쐬���܂��B
	 * @note �쐬����O��SC�}�l�[�W�����I�[�v������K�v���L��܂�
	 *       �e�ڍׂȃp�����[�^�̐����́AMSDN�FCreateService���Q��
	 * @param[in] lpszName �T�[�r�X��
	 * @param[in] lpSorce �T�[�r�X�̐ݒ���\���̂̃A�h���X
	 * @param[in] dwAccess �T�[�r�X�̃I�[�v������A�N�Z�X�}���w�肵�܂�
	 * @param[in] lpszServiceAccount �N�����̃A�J�E���g�ANULL�̓��[�J���V�X�e���A�J�E���g
	 * @param[in] lpszPasswd lpszServiceAccount�Ŏw�肵���A�J�E���g�̃p�X���[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Create(const char* lpszName,
						LPQUERY_SERVICE_CONFIGA lpSorce, 
						DWORD dwAccess = SERVICE_ALL_ACCESS,
						const char* lpszAccount = NULL, 
						const char* lpszPasswd = NULL);
	/** XWinServiceCtrl::Create(const char* ,LPQUERY_SERVICE_CONFIGA,DWORD ,const char* ,const char*) */
	bool Create(const wchar_t* lpszName,
						LPQUERY_SERVICE_CONFIGW lpSorce, 
						DWORD dwAccess = SERVICE_ALL_ACCESS,
						const wchar_t* lpszAccount = NULL, 
						const wchar_t* lpszPasswd = NULL);
	/**
	 * �T�[�r���폜���܂��B
	 * @note �T�[�r�X���I�[�v�����Ă����K�v������܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Delete();

	/**
	 * �T�[�r�X�̐ݒ��ύX���܂��B
	 * @note �T�[�r�X���I�[�v�����Ă����K�v������܂��B
	 * @param[in] lpszDisplayName �T�[�r�X�̕\����
	 * @param[in] lpszImage �T�[�r�X�̎��s�C���[�W�̃t���p�X
	 * @param[in] dwServiceType �T�[�r�X�̎��
	 * @param[in] dwStart �T�[�r�X�̋N�����[�h
	 * @param[in] dwErrorControl �G���[���̏���
	 * @param[in] lpszOrderGroup �I�[�_�[�O���[�v
	 * @param[in] lpszDependencies �ˑ�����T�[�r�X
	 * @param[in] lpszServiceAccount �N�����̃A�J�E���g�ANULL�̓��[�J���V�X�e���A�J�E���g
	 * @param[in] lpszPasswd lpszServiceAccount�Ŏw�肵���A�J�E���g�̃p�X���[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool ChangeConfig( const char* lpszDisplayName = NULL,
						const char* lpszImage = NULL,
						DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS,			
						DWORD dwStart = SERVICE_AUTO_START , 
						DWORD dwErrorControl = SERVICE_ERROR_NORMAL ,
						const char* lpszOrderGroup = NULL, 
						const char* lpszDependencies = NULL, 
						const char* lpszServiceAccount = NULL,
						const char* lpszPasswd = NULL);
	/** XWinServiceCtrl::ChangeConfig(const char*, const char*, DWORD, DWORD, DWORD, const char*, const char*, const char*, const char*) */
	bool ChangeConfig( const wchar_t* lpszDisplayName = NULL,
						const wchar_t* lpszImage = NULL,
						DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS,			
						DWORD dwStart = SERVICE_AUTO_START , 
						DWORD dwErrorControl = SERVICE_ERROR_NORMAL ,
						const wchar_t* lpszOrderGroup = NULL, 
						const wchar_t* lpszDependencies = NULL, 
						const wchar_t* lpszServiceAccount = NULL,
						const wchar_t* lpszPasswd = NULL);
	/**
	 * �T�[�r�X�̐ݒ��ύX���܂��B
	 * @note �T�[�r�X���I�[�v�����Ă����K�v������܂��B
	 * @param[in] lpSorce �T�[�r�X�̐ݒ���\���̂̃A�h���X
	 * @param[in] lpszAccount �N�����̃A�J�E���g�ANULL�̓��[�J���V�X�e���A�J�E���g
	 * @param[in] lpszPasswd lpszAccount�Ŏw�肵���A�J�E���g�̃p�X���[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool ChangeConfig( LPQUERY_SERVICE_CONFIGA lpSorce, 
						const char* lpszAccount = NULL, 
						const char* lpszPasswd = NULL);
	/** XWinServiceCtrl::ChangeConfig(LPQUERY_SERVICE_CONFIGW, const wchar_t* , const wchar_t*) */
	bool ChangeConfig( LPQUERY_SERVICE_CONFIGW lpSorce, 
						const wchar_t* lpszAccount = NULL, 
						const wchar_t* lpszPasswd = NULL);
	/**
	 * �I�[�v�����ꂽ�T�[�r�X�Ɏw�肳�ꂽID�̃R���g���[���n���h���C�x���g�𑗐M���܂��B
	 * @note ���[�U�[��`�̃R���g���[���C�x���gID�́A�P�Q�W�|�Q�T�T�̊Ԓl�ɂȂ�܂�
	 * @param[in] dwContorolID �R���g���[���C�x���gID
	 * @param[out] pStatus �X�e�[�^�X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Cotrol(DWORD dwContorolID, SERVICE_STATUS *pStatus = NULL);
	/**
	 * �I�[�v���T�[�r�X���J�n���܂��B
     * @param[in] dwArgc �T�[�r�X�Ɉ����n���p�����[�^�����w�肵�܂��B
	 * @param[in] lpszArgv �T�[�r�X�Ɉ����n���p�����[�^�̔z��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Start(DWORD dwArgc = 0, char** lpszArgv = NULL);
	/** XWinServiceCtrl::Start(DWORD,char**) */
	bool Start(DWORD dwArgc = 0, wchar_t** lpszArgv = NULL);
	/**
	 * �T�[�r�X���~���܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Stop();
	/**
	 * �T�[�r�X�����s�����邩���ׂ܂��B
	 * @retval true ���s��
	 * @retval false ��~��
	 */
	bool IsMove();
	/**
	 * �T�[�r�X���~���ł��邩���ׂ܂��B
	 * @retval true ��~��
	 * @retval false ���s��
	 */
	bool IsStop();
	/**
	 * �w�肵���T�[�r�X�C���X�g�[������Ă��邩���ׂ܂��B
	 * @param[in] lpszServiceName �T�[�r�X��
	 * @retval true �C���X�g�[������Ă���
	 * @retval false �C���X�g�[������Ă��Ȃ�
	 */
	static bool IsInstall(LPCTSTR lpszServiceName);

	/**
	 * �C�x���g���O�Ɉˑ����郁�b�Z�[�WDLL��ݒ肵�܂��B
	 */
//	DWORD SetEventKey(const char* lpszTsImage, 
//					 DWORD dwTsType = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE, 
//					 const char* lpszCaImange = NULL, 
//					 DWORD dwCaCount = 0, 
//					 DWORD dwSystemKey = XServiceCtrl::App,
//					 const char* lpszSorceName = NULL);

	/**
	 * �T�[�r�X�̃X�e�[�^�X���擾���܂��B
	 * @param[out] lpServiceStatus �T�[�r�X�̃X�e�[�^�X���󂯎��o�b�t�@
	 * @retval true ����
	 * @retval false ���s
	 */
	bool QueryServiceStatus(LPSERVICE_STATUS lpServiceStatus);
	/**
	 * �T�[�r�X�̐ݒ�����擾���܂��B
	 * @param[out] lpServiceConfig �T�[�r�X�̐ݒ�����󂯎��o�b�t�@
	 * @retval true ����
	 * @retval false ���s
	 */
	bool QueryServiceConfig(LPQUERY_SERVICE_CONFIGA *lpServiceConfig);
	/** XWinServiceCtrl::QueryServiceConfig(LPQUERY_SERVICE_CONFIGA*) */
	bool QueryServiceConfig(LPQUERY_SERVICE_CONFIGW *lpServiceConfig);
	/**
	 * �T�[�r�X�̃X�e�[�^�X�ꗗ���擾���܂��B
	 * @param[out] lpServiceStatus �T�[�r�X�̃X�e�[�^�X���󂯎��o�b�t�@
	 * @param[out] pDword �X�e�[�^�X�����󂯎��o�b�t�@
	 * @param[in] dwServiceType �X�e�[�^�X���擾����T�[�r�X�^�C�v
	 * @param[in] dwServiceState �X�e�[�^�X���擾����T�[�r�X�X�e�[�^�X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool EnumServiceStatus(LPENUM_SERVICE_STATUS *lpServices, 
						   LPDWORD pDword, 
						   DWORD dwServiceType = SERVICE_WIN32, 
						   DWORD dwServiceState = SERVICE_STATE_ALL);
	/**
	 * �I�[�v�����ꂽ�T�[�r�X���N���[�Y���܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Close(void);
protected:
	SC_HANDLE		m_hSCManager;
	SC_HANDLE		m_hService;
	XStrBuff		m_strServiceName;
	DWORD			m_dwID;
	DWORD			m_dwAccess;
	DWORD			m_dwServiceSpecificExitCode;
};

#endif // WIN32
#endif //__X_WIN_SERVICE_HEASER_FILE__

