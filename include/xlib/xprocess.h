/**
 * @file xprocess.h
 * @brief �v���Z�X�Ǘ��N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_PROCCESS_HEASER_FILE__
#define __X_PROCCESS_HEASER_FILE__
#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif

 #define   XPOROCCESS_OS_DEPEND_MEM		HANDLE	m_hProccess;\
                                        HANDLE	m_hThread
#else


#endif //
#include "xio.h"


#define XIO_READ   0
#define XIO_WRITE  1

/**
 * �p�C�v�N���X
 */
class XPipe : public XIO
{
public:
	/** �R���X�g���N�^ */
	XPipe();
	/** �f�X�g���N�^ */
	virtual ~XPipe();
	/** @copydoc XIO::Close() */
	virtual bool Close();

	/**
	 * �p�C�v���쐬
	 * @retval true ����
	 * @retval false ���s
	 */
	bool CreatePipe();

	/**
	 * �ǂݍ��݃p�C�v����܂��B
	 */
	void CloseRead();
	/**
	 * �ǂݍ��݃p�C�v����܂��B
	 */
	void CloseWrite();
	/**  XIO::WriteRaw(const void* , size_t , int ) */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0);
	/**  XIO::ReadRaw(void* , size_t , int ) */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0);

#ifdef WIN32
	HANDLE	m_hPipe[2];
	HANDLE  m_hPopeChiledSideRd[2];
	HANDLE  m_hPopeChiledSideWr[2];
#else
	int		m_hPipe[2];
	int		m_hPopeChiledSideRd[2];
	int		m_hPopeChiledSideWr[2];
#endif
};


/**
 * �v���Z�X�N���X
 */
class XProccess : public XPipe
{
protected:
	/** �R���X�g���N�^ */
	XProccess();
public:
	enum EXEC_PIPE {
		EXEC_PIPE_NON,
		EXEC_PIPE_READ,
		EXEC_PIPE_WRITE,
		EXEC_PIPE_READ_WRITE
	};
	virtual ~XProccess();
	static XProccess *Execute(XStrBuffList &command, EXEC_PIPE execMode = EXEC_PIPE_NON, const TCHAR* pDir = NULL, bool bHide = false);
	static int getpid();
	bool Kill();
	unsigned long Wait();
protected:
	unsigned long	m_nProcID;
#ifdef WIN32
	HANDLE	m_hProccess;
	HANDLE	m_hThread;
#else

#endif
};

#endif // __X_PROCCESS_HEASER_FILE__
