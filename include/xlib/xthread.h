/**
 * @file xthread.h
 * @brief �X���b�h
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_THREAD_HEADER_FILE__
#define __X_THREAD_HEADER_FILE__
#include "xlib/xlib_head.h"
#include "xlib/xstrbuff.h"
#include "xlib/xlock.h"
#include "xlib/xeception.h"

#ifdef WIN32

#else /* POSIX */
   #include <pthread.h>
	extern "C" void* XThreadWrapper(void* ptr);
#endif /* WIN32 */


/**
 * @brief	XThredExcept��getErrorCode�����o�Ŏ󂯎��G���[�R�[�h
 */
enum XThredERROR
{
	XThredERROR_SUCCESS = 0,					/** ���� */
	XThredERROR_CREATE_SEM,						/** �Z�}�t�H�̍쐬�Ɏ��s */
	XThredERROR_UN_STS_NEW,						/** ���̃I�y���[�V�����́A�X�e�[�^�X��STATE_NEW�łȂ���΂����Ȃ� */
	XThredERROR_UN_STS_RUNNING,					/** ���̃I�y���[�V�����́A�X�e�[�^�X��STATE_RUNNING�łȂ���΂����Ȃ� */
	XThredERROR_UN_STS_RUNNING_TERMINATED,		/** ���̃I�y���[�V�����́A�X�e�[�^�X��STATE_RUNNING�܂��́ATERMINATED�łȂ���΂����Ȃ� */

	XThredERROR_CREATE_THREAD,					/** �X���b�h�̍쐬�Ɏ��s */
	XThredERROR_SET_PRIORITY,					/** �v���C�I���e�B�������ł��܂��� */
	XThredERROR_UN_INIT,						/** �C���X�^���X�̏��������������Ă܂��� */
	XThredERROR_JOIN,							/** JOIN�������ł��܂��� */
	XThredERROR_WAIT,							/** WAIT�������ł��܂��� */
	XThredERROR_SIGNAL,							/** SIGNAL�������ł��܂��� */
	XThredERROR_BROAD_CAST,						/** BroadCast�������ł��܂��� */
	XThredERROR_YIELD,							/** YIELD�������ł��܂��� */
	XThredERROR_SLEEP,							/** Sleep�������ł��܂��� */
	XThredERROR_SELF,							/** Self�������ł��܂��� */
	XThredERROR_GETTIME,						/** Self�������ł��܂��� */
	XThredERROR_INIT,							/** XThread::InitT�������ł��܂��� */
	XThredERROR_NO_SUPPORT,						/** �V�X�e���ł��̋@�\���T�|�[�g���Ă��܂��� */

};

class XLIB_API XThreadCondition;
class XLIB_API XThread;



/**
 * @brief	�X���b�h�N���X
 */
class XLIB_API XThread
{
public:
	/** @brief	�X���b�h�̃v���C�I���e�B */
    enum priority_t {
		PRIORITY_LOW,		/** LOW */
		PRIORITY_NORMAL,	/** NORMAL */
		PRIORITY_HIGH		/** HIGH */
    };

	/** @brief	�X���b�h�̃X�e�[�^�X */
    enum state_t {
		STATE_NEW,			/** �X���b�h�͊J�n����Ă��Ȃ� */
		STATE_RUNNING,		/** �X���b�h���s�� */
		STATE_TERMINATED	/** �X���b�h�I�� */
    };

	/**
	 *  @brief	�R���X�g���N�^
	 * @param pFnc  �X���b�h���R�[�������֐��ւ̃|�C���^
	 * @param pArgs �X���b�h�ɓn���p�����[�^
	 * @param pri �v���C�I���e�B
	 */
    XThread(void (*pFnc)(void*), void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/**
	 *  @brief	�R���X�g���N�^
	 * @param pFnc  �X���b�h���R�[�������֐��ւ̃|�C���^
	 * @param pArgs �X���b�h�ɓn���p�����[�^
	 * @param pri �v���C�I���e�B
	 */
    XThread(void* (*pFnc)(void*), void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/**
     * @brief	�X���b�h���J�n���܂�
	 */
    void Start();
protected:
	/**
	 * @brief	�X���b�h�̃v���C�I���e�B
	 * @param pArgs �X���b�h�ɓn���p�����[�^
	 * @param pri �X���b�h�v���C�I���e�B�[
	 */
    XThread(void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/** @brief	�f�X�g���N�^ */
    virtual ~XThread(void);
	/**
	 * @brief	RunUndetached���R�[������܂��B
	 */
    void StartUndetached(void);
public:
	/**
	 * @brief	�W���C�����܂��B
	 * @param status �n���l
	 */
    void Join(void** status);
	/**
	 * @brief	�v���C�I���e�B���Z�b�g���܂�
	 * @param pti �v���C�I���e�B
	 */
    void SetPriority(priority_t pti);
	/**
     * @brief	�X���b�h���쐬���܂��B
	 * @param pFn  �X���b�h���R�[�������֐��ւ̃|�C���^
	 * @param pArgs �X���b�h�ɓn���p�����[�^
	 * @param pri �v���C�I���e�B
	 * @return �V�����X���b�h�I�u�W�F�N�g�C���X�^���X�A�h���X
	 */
    static XThread* Create(void (*pFn)(void*), void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/**
     * @brief	�X���b�h���쐬���܂��B
	 * @param pFn  �X���b�h���R�[�������֐��ւ̃|�C���^
	 * @param pArgs �X���b�h�ɓn���p�����[�^
	 * @param pri �v���C�I���e�B
	 * @return �V�����X���b�h�I�u�W�F�N�g�C���X�^���X�A�h���X
	 */
    static XThread* Create(void* (*pFn)(void*), void* pArgs = NULL, priority_t pri = PRIORITY_NORMAL);
	/**
	 * @brief	�X���b�h���[�J���X�g���[�W�ɃZ�b�g���ꂽ�X���b�h���I�����܂�
	 * @param RetVale �X���b�h�̏I���R�[�h
	 * @param bCallToEndThread EndThread���R�[�����邩�ǂ���
	 */
    static void Exit(void* RetVale = NULL, bool bCallToEndThread = true);
	/**
	 * @brief	�X���b�h���[�J���X�g���[�W�ɃZ�b�g���ꂽ�X���b�h���擾���܂��B
	 * @return ���݂̃X���b�h����A�ʃX���b�h��CPU�����蓖�Ă܂�
	 */
    static XThread* Self(void);
	/**
	 * @brief	���݂̃X���b�h����A�ʃX���b�h��CPU�����蓖�Ă܂�
	 */
	static void yield(void);
	/**
	 * @brief	�w�肵�����ԑҋ@���܂�
	 * @param secs �~���b���w�肵�܂�
	 * @param nanosecs �i�m�b���w�肵�܂�
	 */
    static void Sleep(unsigned long secs, unsigned long nanosecs = 0);
	/**
	 * @brief	�~���b�ƃi�m�b�̎��Ԃ��v�Z�����l���擾���܂��B�J�����g���Ԃ��r�����I�t�Z�b�g���^�����܂��B
	 * @param abs_sec �I�t�Z�b�g�����b
	 * @param abs_nsec �I�t�Z�b�g�����i�m�b
	 * @param rel_sec ���ۂ̕b
	 * @param rel_nsec ���ۂ̃i�m�b
	 */
    static void GetTime(unsigned long* abs_sec, unsigned long* abs_nsec,
			 unsigned long rel_sec = 0, unsigned long rel_nsec=0);
protected:
	/**
	 * @brief	�h���N���X�ŁA�X���b�h�Ɏ��s��������e���I�[�o���C�h���܂��BStart���R�[�����邱�Ƃɂ���ČĂяo����܂��B
	 * @param pArg �R���X�g���N�^�œn���ꂽ�f�[�^
	 */
    virtual void Run(void* pArg) {}
	/**
	 * @brief	�h���N���X�ŁA�X���b�h�Ɏ��s��������e���I�[�o���C�h���܂��BStartUnterached���R�[�����邱�Ƃɂ���ČĂяo����܂��B
	 * @param pArg �R���X�g���N�^�œn���ꂽ�f�[�^
	 * @return �X���b�h�̖߂�l
	 */
    virtual void* RunUndetached(void* pArg) { return NULL; }
	/*
	 * @brief	�R���X�g���N�^�œn�����f�[�^
	 * @param pArg �X���b�h�ɓn���p�����[�^
	 * @param pri �v���C�I���e�B
	 * @param nDet �v���C�I���e�B
	 */
    void CommonConstructor(void* pArg, priority_t pri, int nDet);

	/* @brief	���������o�A�N�Z�X�p�I�u�W�F�N�g */
	XLock			m_mutex;
	/* @brief	�X���b�h�X�e�[�^�X */
    state_t			m_state;
	/* @brief	�X���b�h�v���C�I���e�B�[ */
    priority_t		m_priority;
 
	static XLock* m_NextIdMutex;
    static int		m_NextId;
    int				m_nId;
    void			(*m_pFnVoid)(void*);
    void*			(*m_pFnRet)(void*);
    void*			m_pThreadArg;
    int				m_nDetached;
public:
	/**
	 * @brief	�v���C�I���e�B���擾���܂�
	 * @return �v���C�I���e�B
	 */
    priority_t priority(void) {
		XLocker l(m_mutex);
		return m_priority;
    }
	/**
	 * @brief	�X�e�[�^�X���擾���܂�
	 * @return �X�e�[�^�X
	 */
    state_t state(void)  {
		XLocker l(m_mutex);
		return m_state;
    }
	/**
	 * @brief	ID���擾���܂�
	 * @return �X���b�hID
	 */
    int id(void) { return m_nId; }

	/*
	 * @brief	XThread�Ŏg�p������TLS�����������܂�
	 */
    class InitT {
		/* @brief	�J�E���^�[ */
		static int m_count;
	public:
		/* @brief	�R���X�g���N�^�[ */
		InitT(void);
		/* @brief	�ł��g���N�^�[ */
		~InitT(void);
    };
    friend class InitT;
private:
#ifdef WIN32
	/* @brief	�X���b�h�n���h�� */
	HANDLE		m_hThread;
	/* @brief	NT�X���b�hID */
    DWORD      m_dwNtId;
	/* @brief	�߂�l */
    void*      m_pReturnVal;
	/* @brief	�Z�}�t�H�p�n���h�� */
    HANDLE     m_hCondSemaphore;
	/* @brief	���̃X���b�h */
    XThread*   m_pCondNext;
	/* @brief	�O�̃X���b�h */
    XThread*   m_pCcondPrev;
	/* @brief	�X���b�h�z�񃍃b�N */
    BOOL       m_bCondWaiting;
	
    static int NTpriority(priority_t);
    friend class XThreadCondition;
    friend unsigned __stdcall XThreadWrapper(void* ptr);
#else /* POSIX */
	pthread_t		m_hThread;
	static int PosixPriority(priority_t npriority);
	friend void* XThreadWrapper(void* ptr);
#endif /* WIN32 */

};



class XLIB_API XThreadCondition
{
	XLock   *m_pLock;
public:
	XThreadCondition(XLock *pLock);
	~XThreadCondition();

    void Wait(void);
	// ���ި��݂̒l������قɂȂ�܂�WAIT�B
    int TimedWait(unsigned long abs_sec, unsigned long abs_nsec = 0);
	// �w�肳�ꂽ���Ԃ܂ŁAWAIT
    void Signal(void);

    void BroadCast(void);
private:
    XThreadCondition(const XThreadCondition&);
    XThreadCondition& operator=(const XThreadCondition&);

private:
#ifdef _WIN32
	CRITICAL_SECTION      m_crit;
	XThread*              m_pWaitingHead;
	XThread*              m_pWaitingTail;
#else /* POSIX */
    pthread_cond_t posix_cond;
#endif /* WIN32 */
};


#include "xlib/xlib_tail.h"
#endif // __X_THREAD_HEADER_FILE__
