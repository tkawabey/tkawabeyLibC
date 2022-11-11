/**
 * @file xlock.h
 * @brief �t�@�C���A�N�Z�X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_LOCK_HEASER_FILE__
#define __X_LOCK_HEASER_FILE__

#include "xlib/xlib_head.h"


#ifdef WIN32
  #include <windows.h>
#else /* POSIX */
  #include <pthread.h>
  #define INFINITE 0
#endif /* WIN32 */


#define XSEMAPHORE_MAX 0x7fffffff

/**
 * ���b�N�I�u�W�F�N�g
 */
class XLIB_API XLock
{
public:
	/** �R���X�g���N�^ */
	XLock();
	/** �f�X�g���N�^ */
	~XLock();
	/** ���b�N���܂��B */
	void lock();
	/** �A�����b�N���܂��B */
	void unlock();
	friend class XLocker;
private:
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 */
	XLock(const XLock& l);
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 * @return ���g�̃C���X�^���X
	 */
	XLock& operator=(const XLock& l);	

#ifdef WIN32
	/** ���b�N�I�u�W�F�N�g */
	CRITICAL_SECTION rc;
#else /* POSIX */
	/** ���b�N�I�u�W�F�N�g */
	pthread_mutex_t m_hMutex;
#endif /* WIN32 */
};

/**
 * �������b�N���A�����b�N�w���p�[
 */
class XLIB_API XLocker
{
protected:
	/** ���b�N�I�u�W�F�N�g�Q�ƒl */
	XLock	&critsync;
public:
	/**
	 * �R���X�g���N�^
	 * @param m ���b�N�I�u�W�F�N�g
	 */
    XLocker(XLock& m) : critsync(m) { critsync.lock(); }
	/** �f�X�g���N�^ */
    ~XLocker(void) { critsync.unlock(); }
private:
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 */
    XLocker(const XLock& l);
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 * @return ���g�̃C���X�^���X
	 */
    XLocker operator=(const XLock& l);
};




class XLIB_API XMutexer;
/**
 * �����I�u�W�F�N�gMutex��Wrap
 */
class XLIB_API XMutex
{
public:
	/** �R���X�g���N�^ */
	XMutex();
	/** �f�X�g���N�^ */
	~XMutex();
	/**
	 * ���b�N���܂��B
	 */
	void lock(long lTimeOut = -1);
	/**
	 * �A�����b�N���܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool ulock();

	friend class XMutexer;
#ifdef WIN32
	/** ���b�N�I�u�W�F�N�g */
	HANDLE			m_hMutex;
#else /* POSIX */
	/** ���b�N�I�u�W�F�N�g */
	pthread_mutex_t		m_hMutex;
	pthread_mutexattr_t m_mutexattr;
	pthread_mutex_t		m_sig_mutex;
	pthread_cond_t		m_sig_cond;
	int					m_ilocks;
#endif /* WIN32 */
};

/**
 * �������b�N���A�����b�N�w���p�[
 */
class XLIB_API XMutexer
{
protected:
	/** ���b�N�I�u�W�F�N�g�Q�ƒl */
	XMutex &mutex;
public:
	/**
	 * �R���X�g���N�^
	 * @param m ���b�N�I�u�W�F�N�g
	 */
	XMutexer(XMutex& m) : mutex(m) {mutex.lock();}
	/** �f�X�g���N�^ */
	~XMutexer(){mutex.ulock();}
private:
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 */
    XMutexer(const XMutex& l);
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 * @return ���g�̃C���X�^���X
	 */
    XMutexer operator=(const XMutex& l);
};


#ifndef WIN32 // for POSIX implement pthread_cond...
/**
 * �Z�}�t�H�p�w���p�[�N���X
 */
class XLIB_API XCondition
{
public:
	/**
	 * �R���X�g���N�^
	 * @param pMutex ���b�N�I�u�W�F�N�g
	 */
	XCondition(XMutex *pMutex);
	/** �f�X�g���N�^ */
	~XCondition();
	/**
	 * �V�O�i������������܂Ń��b�N���܂��B
	 */
    void Wait(void);
	/**
	 * �w�莞�ԁA�V�O�i������������܂Ń��b�N���܂��B
	 * @param[in] abs_sec �b
	 * @param[in] abs_nsec �i�m�b
	 * @retval true ����
	 * @retval false ���s
	 */
    int TimedWait(unsigned long abs_sec, unsigned long abs_nsec = 0);

	// �w�肳�ꂽ���Ԃ܂ŁAWAIT
	/**
	 * �V�O�i���𑗐M���܂��B
	 */
    void Signal(void);
	/**
	 * �ҋ@���Ă���I�u�W�F�N�g�S�ĂɃV�O�i���𑗐M���܂��B
	 */
    void BroadCast(void);
private:
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 */
    XCondition(const XCondition& l);
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 * @return ���g�̃C���X�^���X
	 */
    XCondition& operator=(const XCondition& l);
	/** ���b�N�I�u�W�F�N�g */
    XMutex         *m_pMutex;
	/** �R���f�B�V�����I�u�W�F�N�g */
    pthread_cond_t m_Cond;
};
#endif


class XLIB_API XSemaphorer;
/**
 * �����I�u�W�F�N�gSemaphore��Wrap
 */
class XLIB_API XSemaphore
{
public:
	/**
	 * �R���X�g���N�^
	 * @param nInitial ��������
	 */
	XSemaphore(unsigned int nInitial = 0, unsigned int nMaxCount = 1);
	/** �f�X�g���N�^ */
	virtual ~XSemaphore();

	bool lock(long lTimeOut = -1);

	/** �A�����b�N���܂��B */
	bool unlock();
	/**
	 * �V�O�i������������܂Ń��b�N���܂��B
	 */
	void Wait(void);
	/**
	 * ���b�N�ł��邩�������������܂��B
	 * @retval 1 �ł���
	 * @retval 0 �ł��Ȃ�
	 */
	int Trywait(void);
	/**
	 * �w�肳�ꂽ�Z�}�t�H�I�u�W�F�N�g�̃J�E���g�l���A�w�肳�ꂽ���������₵�܂��B
	 */
	void Post(void);
private:
#ifdef WIN32
	/** �Z�}�t�H�p�n���h�� */
	HANDLE			m_hMutex;
#else /* POSIX */
	/** �Z�}�t�H�p�n���h�� */
    XMutex         m_Mutex;
	/** �R���f�B�V�����l */
    XCondition     m_Condition;
	/** �J�E���^�[ */
    int            m_iValue;
#endif /* WIN32 */
	friend class XSemaphorer;
};

/**
 * �������b�N���A�����b�N�w���p�[
 */
class XLIB_API XSemaphorer
{
protected:
	/** ���b�N�I�u�W�F�N�g�Q�ƒl */
	XSemaphore & m_sec;
public:
	/**
	 * �R���X�g���N�^
	 * @param s ���b�N�I�u�W�F�N�g
	 */
	XSemaphorer(XSemaphore &s) : m_sec(s){ m_sec.Wait(); }
	/** �f�X�g���N�^ */
	~XSemaphorer(void){ m_sec.Post(); }

private:
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 */
	XSemaphorer(const XSemaphore& l);
	/**
	 * �R�s�[�֎~�̂��߂̑΍�
	 * @param l ���b�N�I�u�W�F�N�g
	 * @return ���g�̃C���X�^���X
	 */
	XSemaphorer& operator=(const XSemaphorer& l);
};


#ifdef WIN32
/**
 * �����I�u�W�F�N�gCEvent��Wrap
 */
class XLIB_API XEvent
{
public:
	/**
	 * �R���X�g���N�^
	 * @param[in] bInitiallyOwn �C�x���g�I�u�W�F�N�g�̏�����Ԃ��w�肵�܂��B
	 * @param[in] bManualReset �蓮���Z�b�g�I�u�W�F�N�g���쐬���邩�A�������Z�b�g�I�u�W�F�N�g���쐬���邩���w�肵�܂��B
	 * @param[in] lpszNAme ���O
	 * @param[in] lpsaAttribute �Z�L�����e�B�L�q�q
	 */
	XEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE,
		LPCTSTR lpszNAme = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);

public:
	/**
	 * �w�肳�ꂽ�I�u�W�F�N�g���V�O�i����Ԃɐݒ肵�܂��B
	 * @retval TRUE ����
	 * @retval FALSE ���s
	 */
	BOOL SetEvent();
	/**
	 * �w�肳�ꂽ�C�x���g�I�u�W�F�N�g���V�O�i����Ԃɐݒ肵�A�ҋ@�X���b�h���������ꍇ�͂��̃X���b�h��������A�C�x���g�I�u�W�F�N�g���V�O�i����ԂɍĐݒ肵�܂��B
	 * @retval TRUE ����
	 * @retval FALSE ���s
	 */
	BOOL PulseEvent();
	/**
	 * �w�肳�ꂽ�C�x���g�I�u�W�F�N�g���V�O�i����Ԃɐݒ肵�܂��B
	 * @retval TRUE ����
	 * @retval FALSE ���s
	 */
	BOOL ResetEvent();
	/**
	 * �A�����b�N���܂��B
	 * @retval TRUE ����
	 * @retval FALSE ���s
	 */
	BOOL Unlock();
	/**
	 * �w�肵�����ԃ��b�N���܂�
	 * @return WaitForSingleObject�̖߂�l�B�n���h���������ȏꍇ�́AERROR_INVALID_HANDLE�B
	 */
	unsigned long Wait(unsigned long dwWaitTime = INFINITE);
protected:
	/** ���b�N�I�u�W�F�N�g */
	HANDLE			m_hMutex;
};
#endif /* WIN32 */

#include "xlib/xlib_tail.h"
#endif // __X_LOCK_HEASER_FILE__
