/**
 * @file xdeamon.h
 * @brief �f�[�����N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DEAMON_HEASER_FILE__
#define __X_DEAMON_HEASER_FILE__

#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif
  #include <winsvc.h>
#endif

#include "xlib/xstrbuff.h"
#include "xlib/xarray.h"
#include "xlib/xlock.h"




class XDeamon;
 class XDeamonCompnent;

#ifdef _WIN32
	#define XDeamon_CA	__stdcall
#else
	#define XDeamon_CA	
#endif
/** �f�[�����̃X�e�[�^�X */
enum XDEMON_STS {
	/** ��~ */
	XDEMON_STS_STOPPED = 1,
	/** �J�n���Ă���r�� */
	XDEMON_STS_START_PENDING,
	/** ��~���Ă���r�� */
	XDEMON_STS_STOP_PENDING ,
	/** ���s�� */
	XDEMON_STS_RUNNING,
	/** �ĊJ���Ă���r�� */
	XDEMON_STS_CONTINUE_PENDING,
	/** �ꎞ��~���Ă���r�� */
	XDEMON_STS_PAUSE_PENDING,
	/** �ꎞ��~ */
	XDEMON_STS_PAUSED,
};

/*
 * �f�[�����R���|�[�l���g���C���֐��̒�`
 */
typedef void (XDeamon_CA *XDEAMON_MAIN)(unsigned long   dwNumDeamonsArgs,TCHAR  *lpDeamonArgVectors[]);
/*
 * �f�[�����C�x���g�n���h���[�֐��̒�`
 */
typedef void (XDeamon_CA *XDEAMON_FUNCTION)(unsigned long);

/**
 * �f�[�����Ǘ��e�[�u��
 */
typedef struct _XDEAMON_TABLE_ENTRY {
	/* �R���|�[�l���g�� */
    TCHAR*                      lpDeamonName;
	/* �f�[�����R���|�[�l���g���C���֐� */
    XDEAMON_MAIN			    lpDeamonProc;
}XDEAMON_TABLE_ENTRY;


/**
 * �R���|�[�l���g��`�}�N��
 * ME �R���|�[�l���g�N���X�̌^
 * NAME �R���|�[�l���g��
 */
#define XDEAMON_MAINHANDLER(ME, NAME)                                              \
  TCHAR  *g_lpsz##ME##DeamonName = NAME;                                           \
  ME    g_Deamon##ME##Component;                                                   \
  void XDeamon_CA ME##DeamonMain(unsigned long dwArgc, TCHAR* lpszArgv[])  {        \
     g_Deamon##ME##Component.Main(dwArgc, lpszArgv);                               \
  }                                                                                \
  void XDeamon_CA ME##Handler(unsigned long fdwControl) {                           \
     g_Deamon##ME##Component.CtrlHandler(fdwControl);                              \
  }
/**
 * �f�[�����Ǘ��e�[�u���̒�`�J�n
 */
#define XDEAMON_TABLSE_START()                                          \
  XDEAMON_TABLE_ENTRY g_DeamonTableEntry[] = {
/**
 * �f�[�����Ǘ��e�[�u���̃R���|�[�l���g��`
 * ME �R���|�[�l���g�N���X�̌^
 */
#define XDEAMON_TABLSE_ENTRY(ME)                                        \
    {g_lpsz##ME##DeamonName, ME##DeamonMain},
/**
 * �f�[�����Ǘ��e�[�u���̒�`�I��
 */
#define XDEAMON_TABLSE_END()                                            \
    {NULL, NULL}                                                        \
  };                                                                    \
  XDeamon::g_pDemonTableEntry = g_DeamonTableEntry;
/**
 * �f�[�����̃R���|�[�l���g�̏�����
 * ME �R���|�[�l���g�N���X�̌^
 */
#define XDEAMON_COMPONENT_INIT(ME)                                      \
  g_Deamon##ME##Component.Initialize(g_lpsz##ME##DeamonName,            \
          ME##Handler);                                                 \
  _ASSERT(XDeamon::m_pGlobalDemon);                                     \
  XDeamon::m_pGlobalDemon->Add(&g_Deamon##ME##Component);


/* �T���v��

// �R���|�[�l���g�̃C���v��
class Componet1 : public XDeamonCompnent, XThread 
{ 
public: 
	Componet1(); 
	virtual ~Componet1(); 
	virtual bool Setup(unsigned long argc, LPTSTR* argv){
        ...
    }
	virtual bool Stop(void){
        ...
    }
	virtual void* RunUndetached(void* pArg); 
}; 
// �R���|�[�l���g�̃C���v��
class Componet2 : public XDeamonCompnent, XThread 
{ 
public: 
	�E�E�E
}; 

// �O���[�o���̃f�[�����C���X�^���X���`
XDeamon g_XDeamonInstance; 

// �f�[�����̃R���|�[�l���g�̒�` 
XDEAMON_MAINHANDLER(Componet1, "TesComponetName1") 
XDEAMON_MAINHANDLER(Componet2, "TesComponetName1") 

void main(int argc, char *argv[])
{
	// �f�[�����e�[�u���̒�` 
	XDEAMON_TABLSE_START() 
	  XDEAMON_TABLSE_ENTRY(Componet1) 
	  XDEAMON_TABLSE_ENTRY(Componet2) 
	XDEAMON_TABLSE_END() 

	// �f�[�����̊J�n
	g_XDeamonInstance.Start(argc, argv);
}
*/

/**	�G���[
*/
enum XDEMON_ERROR
{
	/** �f�[�����R���|�[�l���g.�o�^�G���[ */
	XDEMON_ERROR_REGIST			= 1,
	/** �f�[�����R���|�[�l���g.���|�[�g�񍐃G���[ */
	XDEMON_ERROR_REPORT,
	/** �f�[�����R���|�[�l���g.�Z�b�g�A�b�v�G���[ */
	XDEMON_ERROR_SETUP,
};


/**
 * �f�[�����C���X�^���X�N���X
 */
class XDeamon
{
public:
	/**
	 * �R���|�[�l���g�X���b�h�f�[�^
	 */
	struct STARTUPINFO {
		/** �f�[�����R���|�[�l���g���C���֐��̃A�h���X */
		XDEAMON_MAIN			 lpDeamonMain;
		/** �R���|�[�l���g�ɓn���p�����[�^�� */
		int                      argc;
		/** �R���|�[�l���g�ɓn���p�����[�^ */
		TCHAR                    **argv;
	};
	/** �R���X�g���N�^ */
	XDeamon();
	/** �f�X�g���N�^ */
	virtual ~XDeamon();


	/**
	 * �w�肵���C���f�b�N�X�̃R���|�[�l���g���擾
	 *	@param	nIndes	�C���f�b�N�X
	 *	@return	�R���|�[�l���g
	*/
	XDeamonCompnent *GetComponent(int nIndes) const;
	/**	�R���|�[�l���g�����擾���܂��B
	 *	@return	�R���|�[�l���g��
	*/
	int GetComponentCount() const;
	/**	�R���|�[�l���g��o�^���܂��B
	 *	@param	pConponent	�o�^����R���|�[�l���g
	*/
	void Add(XDeamonCompnent *pConponent);


	/**	�f�[�������J�n���܂��B
	 * @param	argc	�R���|�[�l���g�ɓn���p�����[�^��
	 * @param	argv[]	�R���|�[�l���g�ɓn���p�����[�^
	 * @param	bDebug	�f�o�b�O���[�h�ŊJ�n���邩�ǂ����B
	 *					true���w�肵���ꍇ�ADebugCallBak���I�[�o�[���C�g���A�f�[�����̒�~�Ȃǂ��Ǘ����܂��B
	 * @retval true ����
	 * @retval false ���s
	*/
	bool Start(int argc, TCHAR *argv[], bool bDebug = false);
	/**
	 * �f�o�b�O���[�h���̏������I�[�o�[���C�g���Ă��������B
	 */
	virtual void DebugCallBak();

	/**	�f�o�b�O���[�h���ǂ���
	 * @retval true �f�o�b�O���[�h�ł���
	 * @retval false �f�o�b�O���[�h�łȂ�
	*/
	bool IsDebugMode() const { return m_bDebug; }
	
	/**	@brief	�V�O�i�����A�S�ẴR���|�[�l���g�ɃC�x���g(CtrlHandler)�Ƀu���[�h�L���X�g���܂��B
		@param	signo	�V�O�i���̒l�BOS�Ɉˑ����܂��B
	*/
	void SendBroadCastSignal(unsigned long signo);

// non scope

	/* �f�[�����o�^�e�[�u�� */
	static XDEAMON_TABLE_ENTRY *g_pDemonTableEntry;
	/** �v���O�����ŗB��̃C���X�^���X�̃|�C���^ */
	static XDeamon*				m_pGlobalDemon;
protected:
	/* �R���|�[�l���g�z�� */
	XArray<XDeamonCompnent*>   m_svcCompornent;
	/* �f�o�b�O�t���O */
	bool			m_bDebug;
	/* �R���|�[�l���g��~�҂� */
	XSemaphore		m_semapho;
	/* ���s���̃R���|�[�l���g */
	unsigned long	m_nRunningCompornent;
};

/**
 * �f�[�����R���|�[�l���g�N���X
 */
class XDeamonCompnent
{
public:
	/* �C�x���g�n���h���[�֐��̃|�C���^ */
	XDEAMON_FUNCTION			m_lpHandler;
	
	/**	
     * �R���X�g���N�^
	 *	@param	dwStartWaitHint	Start���̕K�v�Ƃ��鎞�Ԃ��~���b�P�ʂŎw�肷��B
	 *	@param	dwStopWaitHint	Stop���̕K�v�Ƃ��鎞�Ԃ��~���b�P�ʂŎw�肷��B
	 */
	XDeamonCompnent(unsigned long dwStartWaitHint = 5000, 
				    unsigned long dwStopWaitHint = 15000);
	/** �f�X�g���N�^ */
	virtual ~XDeamonCompnent();
	/**	�R���|�[�l���g�̏�����
	 *	@param	lpszDeamonName	�R���|�[�l���g��
	 *	@param	pHandler	�C�x���g�n���h��[
	 */
	void Initialize(const TCHAR* lpszDeamonName, XDEAMON_FUNCTION pHandler);

	/**	@brief	�R���|�[�l���g�����������܂��B
	 *  @note �h���N���X�ŕK���I�[�o�[���C�g���Ă��������B
	 *	@param	argc	�R���|�[�l���g�ɓn���p�����[�^��
	 *	@param	argv[]	�R���|�[�l���g�ɓn���p�����[�^
	 * @retval true ����
	 * @retval false ���s
	*/
	virtual bool Setup(unsigned long argc, TCHAR* argv[]) = 0;
	/**	@brief  �R���|�[�l���g�I�����̌㏈�������s���܂��B
	*/
	virtual bool Cleanup(void) = 0;
	/**	@brief �R���|�[�l���g���~���܂��B
	 * @retval true ����
	 * @retval false ���s
	*/
	bool Stop(void);
	/**	@brief �R���|�[�l���g���ꎞ��~���܂��B
	 * @retval true ����
	 * @retval false ���s
	*/
	bool Pause(void);
	/**	@brief �ꎞ��~���ꂽ�R���|�[�l���g���ĊJ���܂��B
	 * @retval true ����
	 * @retval false ���s
	*/
	bool Continue(void);

	/**	@brief �R���|�[�l���g�̒�~���������܂��B
	 * @retval true ����
	 * @retval false ���s
	*/
	virtual bool StopImple(void);
	/**	@brief �R���|�[�l���g�̈ꎞ��~���������܂��B
	 * @retval true ����
	 * @retval false ���s
	*/
	virtual bool PauseImple(void);
	/**	@brief �R���|�[�l���g�̍ĊJ���������܂��B
	 * @retval true ����
	 * @retval false ���s
	*/
	virtual bool ContinueImple(void);



//	virtual unsigned long UserHandler(unsigned long cmd);
	/**	@brief	�R���|�[�l���g�̃��C�������B���W�X�^�[�A�X�e�[�^�X�񍐁A�R���|�[�l���g�I���҂����s���܂�
	 *	@param	dwArgc		�R���|�[�l���g�ɓn���p�����[�^��
	 *	@param	lpszArgv	�R���|�[�l���g�ɓn���p�����[�^
	*/
	virtual void Main(unsigned long dwArgc, TCHAR* *lpszArgv);
	/**	@brief	�C�x���g�n���h���[
	 *	@param	dwCommand	OS�Ɉˑ�
	 */
	virtual void CtrlHandler(unsigned long dwCommand);
	/**	@brief	�R���|�[�l���g�Ŕ��������G���[��񍐂��܂��B
	 *	@param	nErrorNo	�G���[No
	 *	@param	nOsErrorNo	OS�ˑ��̃G���[�R�[�h
	*/
	virtual void ErrorReport(XDEMON_ERROR nErrorNo, unsigned long nOsErrorNo);


	/**	@brief	�R���|�[�l���g�̌��݂̃X�e�[�^�X���擾���܂��B
	 *	@return	�X�e�[�^�X
	 */
	const XDEMON_STS GetStatus(){ return m_nStatus; }
	/**	@brief	�R���|�[�l���g�����擾���܂��B
	 *	@return	�R���|�[�l���g��
	 */
	const TCHAR* GetCompornentName(){  return m_lpszDeamonName; }
	/**	@brief	�R���|�[�l���g�̊J�n�����~�Ȃǂ̃X�e�[�^�X��񍐂��܂��B
	 *	@param	dwCurrentState	���݂̃X�e�[�^�X
	 *	@param	dwWaitHint		���̃X�e�[�^�X�̕ύX����܂łɕK�v�Ȏ��Ԃ��~���b�Ŏw�肵�܂��BPending�̃X�e�[�^�X�Ŏg�p���܂��B�K�v�Ȃ���΂O���w�肵�܂��B
	 *	@param	dwSysExitCode	�G���[�R�[�h�B�G���[���Ȃ��ꍇ�́A�O���w�肵�܂��B
	 *	@param	dwProcessSpecificExitCode	�V�X�e���ˑ��̃G���[�R�[�h�B�G���[���Ȃ��ꍇ�́A�O���w�肵�܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool ReportStatusChange(
					 unsigned long dwCurrentState,
					 unsigned long dwWaitHint      = 5000,
					 unsigned long dwSysExitCode = 0,
					 unsigned long dwProcessSpecificExitCode = 0);
private:
	/*	@brief	NT�T�[�r�X�̃��W�X�^�[�o�^�A�܂��́A�V�O�i���o�^�����s
	 * @retval true ����
	 * @retval false ���s
	*/
	bool RegisterHandler();

	/**	@brief	Peinding��Ԃ̓r���o�߂�񍐂��܂��B
	 *	@param	dwCurrentState	���݂̃X�e�[�^�X
	 *	@param	dwWaitHint	���̃X�e�[�^�X�̕ύX����܂łɕK�v�Ȏ��Ԃ��~���b�Ŏw�肵�܂��BPending�̃X�e�[�^�X�Ŏg�p���܂��B�K�v�Ȃ���΂O���w�肵�܂��B
	 *	@param	dwCheckPoint	�`�F�b�N�|�C���g�̒l
	 * @retval true ����
	 * @retval false ���s
	*/
	bool SetPendingStatus(unsigned long dwCurrentState,
						  unsigned long dwWaitHint = 5000,
						  unsigned long dwCheckPoint = 1);
	/*	@brief �X�e�[�^�X�̍X�V
	 * @note NT�T�[�r�X�̏ꍇ�A�`�F�b�N�|�C���g���P�C���N�������g���܂��B
	 * @retval true ����
	 * @retval false ���s
	*/
	bool UpdateStatus(void);
	/*	@brief �X�e�[�^�X�̃Z�b�g�B
	 * @note NT�T�[�r�X�́ASetServiceStatus�����s�B
	 * @retval true ����
	 * @retval false ���s
	*/
	bool SetStatus(void);


protected:
	/* Start&Continue���̕K�v�Ƃ��鎞�ԁB�~���b�P�ʁB */
	unsigned long	m_dwStartWaitHint;
	/* Stop&Pause���̕K�v�Ƃ��鎞�ԁB�~���b�P�ʁB */
	unsigned long	m_dwStopWaitHint;
	/* �R���|�[�l���g�� */
	const TCHAR*    m_lpszDeamonName;
	/* �ҋ@�I�u�W�F�N�g */
	XSemaphore		m_SvcWait;
	/* �X�e�[�^�X */
	XDEMON_STS		m_nStatus;
	/* �e�C���X�^���X */
	XDeamon			*m_pParent;
#ifdef WIN32
	/* NT�T�[�r�X��SC�}�l�[�W���[�n���h�� */
	SC_HANDLE		m_hSCHandle;
	/* NT�T�[�r�X��SC�}�l�[�ʒm����X�e�[�^�X�f�[�^ */
	SERVICE_STATUS	m_DeamonStatus;
#endif
	friend class XDeamon;
};

#endif //__X_DEAMON_HEASER_FILE__
