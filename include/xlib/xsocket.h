/**
 * @file xsocket.h
 * @brief �\�P�b�g
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_SOCKET_HEASER_FILE__
#define __X_SOCKET_HEASER_FILE__
#include "xlib/xlib_head.h"

#ifdef WIN32
 
 #ifndef _INC_WINDOWS
  #ifndef __AFXSOCK_H__
    #include <winsock2.h>
  #endif /*__AFXSOCK_H__*/
 #else
    #include <winsock.h>
 #endif //_WINSOCK2API_



 #ifndef _WINSOCKAPI_ 
   #define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */ 
 #endif 

 // ws2_32.lib
// #pragma comment(lib, "ws2_32.lib")


 #ifndef SD_RECEIVE
	#define SD_RECEIVE      0x00
 #endif //SD_RECEIV
 #ifndef SD_SEND
    #define SD_SEND         0x01
 #endif //SD_SEND
 #ifndef SD_BOTH
	#define SD_BOTH         0x02
 #endif //SD_SEND


 #define   XSOCIOERR()     GetLastError()
 #define XSOKET_ERRORNO    h_errno
 #ifndef socklen_t
	typedef int socklen_t;
 #endif


#else /* for UX */

 #if defined(__osf1__) /* Tru64*/
   #if !defined(_POSIX_PII_SOCKET)
     #define  _POSIX_PII_SOCKET
   #endif
 #endif
 #include <sys/ioctl.h> 
 #include <sys/types.h> 
 #include <sys/socket.h> 
 #include <netinet/in.h> 
 #include <arpa/inet.h> 
 #include <unistd.h> 
 #include <stdarg.h> 
 #include <netdb.h> 

 #ifndef SOCKET
  typedef int SOCKET;
 #endif

 #ifndef closesocket
   #define  closesocket close
 #endif /*closesocket*/

	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)

	typedef struct sockaddr SOCKADDR;
	typedef struct sockaddr *PSOCKADDR;
	typedef struct sockaddr *LPSOCKADDR;

	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr_in *PSOCKADDR_IN;
	typedef struct sockaddr_in *LPSOCKADDR_IN;

	typedef struct linger LINGER;
	typedef struct linger *PLINGER;
	typedef struct linger *LPLINGER;

	typedef struct in_addr IN_ADDR;
	typedef struct in_addr *PIN_ADDR;
	typedef struct in_addr *LPIN_ADDR;

	#ifndef FD_SET
//		typedef struct fd_set FD_SET;
	#endif
	#ifndef PFD_SET
//		typedef struct fd_set *PFD_SET;
	#endif
	#ifndef LPFD_SET
//		typedef struct fd_set FAR *LPFD_SET;
	#endif

	typedef struct hostent HOSTENT;
	typedef struct hostent *PHOSTENT;
	typedef struct hostent *LPHOSTENT;

	typedef struct servent SERVENT;
	typedef struct servent *PSERVENT;
	typedef struct servent *LPSERVENT;

	typedef struct protoent PROTOENT;
	typedef struct protoent *PPROTOENT;
	typedef struct protoent *LPPROTOENT;

	typedef struct timeval TIMEVAL;
	typedef struct timeval *PTIMEVAL;
	typedef struct timeval *LPTIMEVAL;

 #define   XSOCIOERR()     errno
 #define XSOKET_ERRORNO    errno

#endif
#include "xlib/xthread.h"
#include "xlib/xarray.h"
#include "xlib/xio.h"
#include "vector"

class XSocketExcept;
class XSocketAddress;
class XSocket;
  class XSocketClinet;
    class XSocketTCPClinet;
  class XSocketServer;
    class XSocketTCPServer;



DLL_EXTERN template class XLIB_API std::vector<XSocketAddress*>;

/* typedef define */
typedef void* (*XSOCK_CLIENTPROC)(XSocketClinet*, void *pUserData);


#define XSOCK_ERR_OWN            0xFFFFFFFF
#define XSOCK_ERR_OWN_RCV_XERO   1
#define XSOCK_ERR_OWN_WRT_XERO   2


#ifdef WIN32
/**
 * �\�P�b�g���C�u�����̏��������s���܂�
 */
class XLIB_API XSocketInit
{
public:
// �\�z/����
	/**
	 * �R���X�g���N�^
	 * param wVersionRequested �\�P�b�g�̃o�[�W�����w�肷��ꍇ�͂P���w�肵�܂�
	 */
	XSocketInit(WORD wVersionRequested = 0);
	/** �f�X�g���N�^ */
	~XSocketInit();
protected:
	/** winsock �f�[�^ */
	WSADATA m_wsaData;
};
#endif /* WIN32 */





/**
 * �\�P�b�g�A�h���X�\���̂̃��b�v�N���X
 */
class XLIB_API XSocketAddress : public sockaddr_in
{
public:
// Construction
	/**
	 * �R���X�g���N�^
	 */
	XSocketAddress();
	/**
	 * �R���X�g���N�^
	 * @param nAddress IP�A�h���X
	 * @param iPort �|�[�g�ԍ�
	 * @param iFamily �A�h���X�t�@�~���[
	 */
	XSocketAddress(unsigned int nAddress, unsigned short iPort, short iFamily = AF_INET);
	/**
	 * �R���X�g���N�^
	 * @param lpszHostName �z�X�g��
	 * @param iPort �|�[�g�ԍ�
	 * @param iFamily �A�h���X�t�@�~���[
	 */
	XSocketAddress(const char* lpszHostName, unsigned short iPort, short iFamily = AF_INET);
	/**
	 * �R���X�g���N�^
	 * @param pAddr �\�P�b�g�A�h���X�\���̃C���X�^���X�̃A�h���X
	 */
	XSocketAddress(const struct sockaddr_in* pAddr);
	/**
	 * �R���X�g���N�^
	 * @param pAddr �\�P�b�g�A�h���X�\���̃C���X�^���X�̃A�h���X
	 */
	XSocketAddress(const struct sockaddr* pAddr);
	/** �f�X�g���N�^ */
	virtual ~XSocketAddress();

// Operation
	XSocketAddress& operator=(XSocketAddress& address);
	/**
	 * �\�P�b�g�A�h���X�̏�����
	 * @param nAddress IP�A�h���X
	 * @param iPort �|�[�g�ԍ�
	 * @param iFamily �A�h���X�t�@�~���[
	 */
	void SetAddress(unsigned int nAddress, unsigned short iPort, short iFamily = AF_INET);
	/**
	 * �\�P�b�g�A�h���X�̏�����
	 * @param lpszHostName �z�X�g��
	 * @param iPort �|�[�g�ԍ�
	 * @param iFamily �A�h���X�t�@�~���[
	 */
	void SetAddress(const char* lpszHostName, unsigned short iPort, short iFamily = AF_INET);
	/**
	 * �\�P�b�g�A�h���X�̏�����
	 * @param pAddr �\�P�b�g�A�h���X�\���̃C���X�^���X�̃A�h���X
	 */
	void SetAddress(const struct sockaddr_in* pAddr);
	/**
	 * �\�P�b�g�A�h���X�̏�����
	 * @param pAddr �\�P�b�g�A�h���X�\���̃C���X�^���X�̃A�h���X
	 */
	void SetAddress(const struct sockaddr* pAddr);

	/**
	 * �A�h���X��FQDN���擾���܂��B
	 * @return �A�h���X��FQDN
	 */
	XStrBuff GetFQDN() const;
	/**
	 * IP�A�h���X�𕶎���Ƃ��Ď擾���܂��B
	 * @return IP�A�h���X�̕�����
	 */
	XStrBuff GetIPAddress() const;
	/**
	 * �\�P�b�g�A�h���X�\���̂��擾���܂��B
	 * @return �\�P�b�g�A�h���X�\���̂̃A�h���X
	 */
	operator sockaddr_in*();
	/**
	 * �\�P�b�g�A�h���X�\���̂��擾���܂��B
	 * @return �\�P�b�g�A�h���X�\���̂̃A�h���X
	 */
	operator sockaddr*();
protected:
	/** �G�C���A�X�ꗗ */
	XStrBuffList              m_Aliases;
	/** IP�A�h���X�I�u�W�F�N�g�z�� */
	std::vector<XSocketAddress*>   m_IPAddresses;
};


/**
 * �\�P�b�g�A�h���X�\���̂̃��b�v�N���X
 */
class XLIB_API XSocket : public XIO
{
public:
	/**
	 * �R���X�g���N�^
	 */
	XSocket();
	/**
	 * �R���X�g���N�^
	 * @param Socket �\�P�b�g
	 */
	XSocket(SOCKET Socket);
	/**
	 * �w�肵���\�P�b�g�ŁA�C���X�^���X�����������܂��B
	 * @param Socket �\�P�b�g
	 */
	void Attach(SOCKET Socket);
	/**
	 * �C���X�^���X�����̃\�P�b�g��؂藣���܂�
	 * @return Socket �\�P�b�g
	 */
	SOCKET Detach();

// overlap
	/**
	 * �\�P�b�g����܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Close();
	/**
	 * �o�b�t�@���������݂܂��B������
	 * @param lpData �������ރo�b�t�@
	 * @param nSize lpData�̃T�C�Y
	 * @param flg �I�v�V�����t���O�BMSG_OOB��AMSG_PEEK�Ȃǂ��w��\�B�ڂ����́A�\�P�b�g:recv�̃}�j���A�����Q��]
	 * @return �������񂾃o�C�g���B�ǂݍ��݃G���[�����������ꍇ�́A-�P��Ԃ��܂��B
	 */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0);
	/**
	 * �o�b�t�@��ǂݍ��݂܂��B������
	 * @param lpData �ǂݍ��ރo�b�t�@���i�[����̃A�h���X
	 * @param nSize lpData�̃T�C�Y
	 * @param flg �I�v�V�����t���O�BMSG_OOB��AMSG_PEEK�Ȃǂ��w��\�B�ڂ����́A�\�P�b�g:send�̃}�j���A�����Q��]
	 * @return �ǂݍ��񂾃o�C�g���B�ǂݍ��݃G���[�����������ꍇ�́A-�P��Ԃ��܂��B
	 */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0);

public:
// Low Operation
	/**
	 * �\�P�b�g���\�z���܂��B
	 * @param[in] type �\�P�b�g�^�C�v
	 * @param[in] af �A�h���X�t�@�~���[
	 * @param[in] protocol �v���g�R��
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Socket(int type, int af = AF_INET, int protocol = IPPROTO_TCP);
	/**
	 * �\�P�b�g���\�z���܂��B�h���N���X�ŃC���v�����܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Socket(){return true;}

	/**
	 * �\�P�b�g�ɖ��O�����܂�
	 * @param[in] address �\�P�b�g�A�h���X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Bind(XSocketAddress& address);
	/**
	 * �\�P�b�g�̐ڑ����s���܂��B
	 * @param[in] address �\�P�b�g�A�h���X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Connect(XSocketAddress& address);
	/**
	 * �S��d�ڑ��̈ꕔ���Ւf���܂��B
	 * @param[in] nHow SHUT_RD �Ȃ�΁A����ȍ~�̎�M���֎~����BSHUT_WR �Ȃ�΁A����ȍ~�̑��M���֎~����BSHUT_RDWR �Ȃ�΁A����ȍ~�̑���M���֎~����B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool ShutDown(int nHow);

	
	/**
	 * �\�P�b�g����f�[�^���擾���܂��B
	 * @note �ڑ��v�l�łȂ�UDP�Ȃǂ̃v���g�R���Ŏg�p���܂��B
	 * @param[in] lpData ���[�U�[�T�C�h�Ŋm�ۂ����f�[�^�̃o�b�t�@�̃A�h���X
	 * @param[in] nSize lpData�Ŏw�肵���f�[�^�̃T�C�Y
	 * @param[in] address ��M��A�h���X
	 * @param[in] flags �I�v�V�����t���O�BMSG_OOB��AMSG_PEEK�Ȃǂ��w��\�B�ڂ����́A�\�P�b�g:recv�̃}�j���A�����Q��]
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool ReadFrom(void* lpData, unsigned long nSize, XSocketAddress& address, int flags = 0);
	/**
	 * �\�P�b�g�Ɏw�肳�ꂽ��������������݂܂��B
	 * @note �ڑ��v�l�łȂ�UDP�Ȃǂ̃v���g�R���Ŏg�p���܂��B
	 * @param[in] lpData ���[�U�[�T�C�h�Ŋm�ۂ����f�[�^�̃o�b�t�@�̃A�h���X
	 * @param[in] nSize lpData�Ŏw�肵���f�[�^�̃T�C�Y
	 * @param[in] address ���M��̃A�h���X
	 * @param[in] flags �I�v�V�����t���O�BMSG_OOB��AMSG_PEEK�Ȃǂ��w��\�B�ڂ����́A�\�P�b�g:send�̃}�j���A�����Q��]
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool WriteTo(const void* lpData, unsigned long nSize, XSocketAddress &address, int flags = 0);



	/**
	 * �ڑ��v�l�̃\�P�b�g�ŁA�ڑ����ꂽ�A�\�P�b�g�̑���̃A�h���X���擾���܂��B
	 * @param[out] address �\�P�b�g�A�h���X���
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetPeerName(XSocketAddress& address);
	/**
	 * �\�P�b�g�̃A�h���X���擾���܂��B
	 * @param[out] address �\�P�b�g�A�h���X���
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetSockName(XSocketAddress& address);
	/**
	 * �\�P�b�g�𐧌䂵�܂��B
	 * @param[out] cmd ����R�}���h
	 * @param[out] argp �R�}���h�ɂ��p�����[�^
	 * @retval true ����
	 * @retval false ���s
	 */
	bool IOCtrl(long cmd = FIONREAD, void* argp = 0 );
	/**
	 * �\�P�b�g�I�v�V�������擾���܂��B
	 * @param[out] optval1 �I�v�V�����̒l���擾���郆�[�U�[�T�C�h�Ŋm�ۂ����A�o�b�t�@�̃A�h���X
	 * @param[in,out] optlen optval1�Ŏw�肵���A�o�b�t�@�̃T�C�Y���w�肵�܂��B�֐������������ꍇ�́A���ێ擾�����T�C�Y���Z�b�g����܂��B
	 * @param[in] optname �I�v�V�������̒l
	 * @param[in] level �v���g�R���w
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetSocketOpt(void* optval1, socklen_t *optlen, int optname, int level = SOL_SOCKET);
	/**
	 * �\�P�b�g�I�v�V�������擾���܂��B
	 * @param[in] optval1 �ݒ肷��I�v�V�����l
	 * @param[in] optlen optval1�Ŏw�肵���A�o�b�t�@�̃T�C�Y���w�肵�܂��B
	 * @param[in] optname �I�v�V�������̒l
	 * @param[in] level �v���g�R���w
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SetSocketOpt(void* optval1, int optlen , int optname, int level = SOL_SOCKET);
	/**
	 * �\�P�b�g������������Ă��邩�A�i�ʐM�̂��߂̒[�_(endpoint)���쐬����Ă��邩�j���ׂ܂��B
	 * @retval true �쐬����Ă���
	 * @retval false �쐬����Ă��Ȃ�
	 */
	bool IsValid();

// Server Side
	/**
	 * �\�P�b�g��̐ڑ���҂��܂��B
	 * @param[in] nBacklog �ۗ����̐ڑ��̃L���[���g�����邱�Ƃ̂ł���ő咷���w�肵�܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Listen(int nBacklog);
	/**
	 * �\�P�b�g�ւ̐ڑ����󂯂�܂��B
	 * @param[in] pClient �󂯓��ꂽ�\�P�b�g�擾�p��XSocket�܂��́A���̔h���N���X�̃C���X�^���X�̃A�h���X�B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Accept(XSocket *pClient);

// SOCKET OPTIONS
	/**
	 * �\�P�b�g�ŁA�u���[�h�L���X�g���b�Z�[�W�̓]�������̗L�����擾���܂�
	 * @retval true ������ꍇ
	 * @retval false �����Ȃ��ꍇ
	 */
	bool get_Broadcast();
	/**
	 * �\�P�b�g�ŁA�u���[�h�L���X�g���b�Z�[�W�̓]��������ݒ肵�܂�
	 * @param[in] newVal ������ꍇ��true�B�����Ȃ��ꍇ��false���w��B
	 */
	void put_Broadcast(bool newVal);
	/**
	 * �f�o�b�O�����L�^�̗L�����擾���܂�
	 * @retval true �L�^����ꍇ
	 * @retval false �L�^���Ȃ��ꍇ
	 */
	bool get_Debug();
	/**
	 * �f�o�b�O�����L�^�̗L����ݒ肵�܂�
	 * @param[in] newVal �L�^����ꍇ��true�B�L�^���Ȃ��ꍇ��false���w��B
	 */
	void put_Debug(bool newVal);

	/**
	 * ���[�`���O���܂���B���ڃC���^�[�t�F�C�X�ɑ��M��On/Off���擾���܂�
	 * @retval true On
	 * @retval false Off
	 */
	bool get_DontRoute();
	/**
	 * ���[�`���O���܂���B���ڃC���^�[�t�F�C�X�ɑ��M��On/Off��ݒ肵�܂�
	 * @param[in] newVal On��true�BOff��false���w��B
	 */
	void put_DontRoute(bool newVal);
	/**
	 * �ڑ��ێ��p�P�b�g�𑗐M��On/Off���擾���܂��B
	 * @retval true On
	 * @retval false Off
	 */
	bool get_KeepAlive();
	/**
	 * �ڑ��ێ��p�P�b�g�𑗐M��On/Off��ݒ肵�܂��B
	 * @param[in] newVal On��true�BOff��false���w��B
	 */
	void put_KeepAlive(bool newVal);
	/**
	 * �����M�f�[�^������Ƃ��AClose��x���̐ݒ���擾���܂��B
	 * @param[in] pLinger LINGER�\���̂̃A�h���X
	 */
	void get_Linger(LINGER *pLinger);
	/**
	 * �����M�f�[�^������Ƃ��AClose��x���̐ݒ��ݒ肵�܂�
	 * @param[in] pLinger LINGER�\���̂̃A�h���X
	 */
	void put_Linger(const LINGER *pLinger);
	/**
	 * �ʏ�̃f�[�^�X�g���[���őш�O�f�[�^����M��On/Off���擾���܂��B
	 * @retval true On
	 * @retval false Off
	 */
	bool get_Oobline();
	/**
	 * �ʏ�̃f�[�^�X�g���[���őш�O�f�[�^����M��On/Off��ݒ肵�܂��B
	 * @param[in] newVal On��true�BOff��false���w��B
	 */
	void put_Oobline(bool newVal);
	/**
	 * ��M�p�̃o�b�t�@�T�C�Y�����擾���܂��B/Off���擾���܂��B
	 * @return ��M�p�̃o�b�t�@�T�C�Y
	 */
	int  get_RcvBuf();
	/**
	 * ��M�p�̃o�b�t�@�T�C�Y����ݒ肵�܂��B
	 * @param[in] newVal ��M�p�̃o�b�t�@�T�C�Y
	 */
	void put_RcvBuf(int newVal);
	/**
	 * �\�P�b�g���g�p���̃A�h���X�Ƀo�C���h��On/Off���擾���܂��B
	 * @retval true On
	 * @retval false Off
	 * @note �|�[�g�n�C�W���b�N
	 */
	bool get_ReuseAddr();
	/**
	 * �\�P�b�g���g�p���̃A�h���X�Ƀo�C���h��On/Off��ݒ肵�܂��B
	 * @param[in] newVal On��true�BOff��false���w��B
	 * @note �|�[�g�n�C�W���b�N
	 */
	void put_ReuseAddr(bool newVal);

	/**
	 * �|�[�g�̍ė��p���֎~�ŁA�o�C���h���邩�ǂ����𓾂�
	 * @retval true On
	 * @retval false Off
	 * @note �|�[�g�n�C�W���b�N���֎~����B
	 */
	bool get_ExclusiveAddr();

	/**
	 * �|�[�g�̍ė��p���֎~�ŁA�o�C���h���邩�ǂ�����ݒ肷��B
	 * @param[in] newVal On��true�BOff��false���w��B
	 * @note �|�[�g�n�C�W���b�N���֎~����B
	 */
	void put_ExclusiveAddr(bool newVal);
	/**
	 * ���M�p�̃o�b�t�@�T�C�Y�����擾���܂��B/Off���擾���܂��B
	 * @return ���M�p�̃o�b�t�@�T�C�Y
	 */
	int  get_SndBuf();
	/**
	 * ���M�p�̃o�b�t�@�T�C�Y����ݒ肵�܂��B
	 * @param[in] newVal ���M�p�̃o�b�t�@�T�C�Y
	 */
	void put_SndBuf(int newVal);
	/**
	 * ��M���̃^�C���A�E�g�l��ݒ肵�܂��B
	 * @return ��M���̃^�C���A�E�g�l
	 */
	int  get_RcvTimeO();
	/**
	 * ��M���̃^�C���A�E�g�l��ݒ肵�܂��B
	 * @param[in] newVal ��M���̃^�C���A�E�g�l
	 */
	void put_RcvTimeO(int newVal);
	/**
	 * ���M���̃^�C���A�E�g�l���擾���܂��B
	 * @return ���M���̃^�C���A�E�g�l
	 */
	int  get_SndTimeO();
	/**
	 * ���M���̃^�C���A�E�g�l��ݒ肵�܂��B
	 * @param[in] newVal ���M���̃^�C���A�E�g�l
	 */
	void put_SndTimeO(int newVal);
#if defined(WIN32)
	/**
	 * Close�͖����M�f�[�^�̑��M��҂�҂��Ȃ��̗L���B
	 * @note ���̃I�v�V������ݒ肷�邱�Ƃ́Al_onoff��0�ɂ���SO_LINGER��ݒ肷�邱�ƂƓ����ł��B
	 * @retval true �L�^����ꍇ
	 * @retval false �L�^���Ȃ��ꍇ
	 */
	bool get_DontLinger();
	/**
	 * Close�͖����M�f�[�^�̑��M��҂�҂��Ȃ��̗L���B
	 * @note ���̃I�v�V������ݒ肷�邱�Ƃ́Al_onoff��0�ɂ���SO_LINGER��ݒ肷�邱�ƂƓ����ł��B
	 * @param[in] newVal �����M�f�[�^�̑��M��҂�҂��Ȃ��̗L�L����ꍇ��true�B�����M�f�[�^�̑��M��҂�҂��Ȃ��̗L�L���Ȃ��ꍇ��false���w��B
	 */
	void put_DontLinger(bool newVal);

	/**
	 * �����ׂ̈ɗ\�񂳂�Ă��܂��B���݂͎g�p�ł��܂���B
	 * @return �v���C�I���e�B
	 */
	int  get_GroupPriority();
	/**
	 * �����ׂ̈ɗ\�񂳂�Ă��܂��B���݂͎g�p�ł��܂���B
	 * @param[in] newVal�v���C�I���e�B
	 */
	void put_GroupPriority(int newVal);

	/**
	 * ���M�f�[�^���܂Ƃ߂đ��邽�߂�Nagle�A���S���Y���𖳌���On/Off���擾���܂��B
	 * @retval true On
	 * @retval false Off
	 */
	bool get_NoDelay();
	/**
	 * ���M�f�[�^���܂Ƃ߂đ��邽�߂�Nagle�A���S���Y���𖳌���On/Off��ݒ肵�܂��B
	 * @param[in] newVal On��true�BOff��false���w��B
	 */
	void put_NoDelay(bool newVal);
#endif /*WIN32*/

	/**	@brief	8�o�C�g�̃z�X�g�R�[�h�̒l���l�b�g���[�N�o�C�g�I�[�_�[�ɕύX����B
	 *	@param	val	�z�X�g�R�[�h�̒l
	 *	@return	�l�b�g���[�N�o�C�g�I�[�_�[�̒l
	 */
	static double htond(double val);
	/**	@brief	8�o�C�g�̂��l�b�g���[�N�o�C�g�I�[�_�[�̒l���z�X�g�R�[�h�ɕύX����B
	 *	@param	val	�z�X�g�R�[�h�̒l
	 *	@return	�l�b�g���[�N�o�C�g�I�[�_�[�̒l
	 */
	static double ntohd(double val);
	/**	@brief	4�o�C�g�̃z�X�g�R�[�h�̒l���l�b�g���[�N�o�C�g�I�[�_�[�ɕύX����B
	 *	@param	val	�z�X�g�R�[�h�̒l
	 *	@return	�l�b�g���[�N�o�C�g�I�[�_�[�̒l
	 */
	static unsigned long _htonl(unsigned long val);
	/**	@brief	4�o�C�g�̂��l�b�g���[�N�o�C�g�I�[�_�[�̒l���z�X�g�R�[�h�ɕύX����B
	 *	@param	val	�z�X�g�R�[�h�̒l
	 *	@return	�l�b�g���[�N�o�C�g�I�[�_�[�̒l
	 */
	static unsigned long _ntohl(unsigned long val);
	/**	@brief	2�o�C�g�̃z�X�g�R�[�h�̒l���l�b�g���[�N�o�C�g�I�[�_�[�ɕύX����B
	 *	@param	val	�z�X�g�R�[�h�̒l
	 *	@return	�l�b�g���[�N�o�C�g�I�[�_�[�̒l
	 */
	static unsigned short _htons(unsigned short val);
	/**	@brief	2�o�C�g�̂��l�b�g���[�N�o�C�g�I�[�_�[�̒l���z�X�g�R�[�h�ɕύX����B
	 *	@param	val	�z�X�g�R�[�h�̒l
	 *	@return	�l�b�g���[�N�o�C�g�I�[�_�[�̒l
	 */
	static unsigned short _ntohs(unsigned short val);
protected:
	/* �\�P�b�g */
	SOCKET                     m_Socket;
	/* �G���[ */
	unsigned long              m_dwLastError;
	/* �G���[ */
	unsigned long              m_dwOwnError;
	friend class XSocketExcept;
};





/**
 * �N���C�A���g�A�\�P�b�g�A�x�[�X�A�N���X
 */
class XLIB_API XSocketClinet : public XSocket
{
public:
	/** �R���X�g���N�^ */
	XSocketClinet();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketClinet(SOCKET Socket);
	/** �f�X�g���N�^ */
	virtual ~XSocketClinet();
	/**
	 * �\�P�b�g���I�[�v�����܂��B
	 * @param[in] address �A�h���X
	 * @retval true ����
	 * @retval false ���s
	 * @exception XSocketExcept* ��O�f�[�^
	 */
	virtual bool Open(XSocketAddress& address) = 0;
};


/**
 * �T�[�o�[�A�\�P�b�g�A�x�[�X�A�N���X
 */
class XLIB_API XSocketServer : public XSocket
{
public:
	/** �R���X�g���N�^ */
	XSocketServer();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketServer(SOCKET Socket);
	/** �f�X�g���N�^ */
	virtual ~XSocketServer();
	/**
	 * �T�[�o�[�\�P�b�g���I�[�v�����܂��B
	 * @param[in] address �A�h���X
	 * @param[in] pFnc �N���C�A���g�T�C�h�����̊֐��A�h���X
	 * @param[in] pUserData pFnc��pUserData�ɓn�����[�U�[��`�f�[�^�̃A�h���X�B
	 * @param[in] nBroking �ۗ����̐ڑ��̃L���[���g�����邱�Ƃ̂ł���ő咷���w�肵�܂��B
	 * @exception XExcept& ��O�f�[�^
	 */
	virtual void Open(XSocketAddress& address, XSOCK_CLIENTPROC pFnc, void *pUserData,  int nBroking = 5) = 0;
	/** �N���C�A���g�T�C�h�����̊֐��A�h���X */
	XSOCK_CLIENTPROC m_pFnClient;
	/** pFnc��pUserData�ɓn�����[�U�[��`�f�[�^�̃A�h���X�B */
	void*   m_pUserData;
};


/**
 * TCP�\�P�b�g�N���X
 */
class XLIB_API XSocketTCPClinet : public XSocketClinet
{
public:
	/** �R���X�g���N�^ */
	XSocketTCPClinet();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketTCPClinet(SOCKET Socket);
	/** �f�X�g���N�^ */
	virtual ~XSocketTCPClinet();
	/** @copydoc XSocket::Socket() */
	virtual bool Socket();
	/** @copydoc XSocketClinet::Open(XSocketAddress&) */
	virtual bool Open(XSocketAddress& address);
};


/**
 * TCP�\�P�b�g�T�[�o�[�N���X
 */
class XLIB_API XSocketTCPServer : public XSocketServer , XThread
{
public:
	/**
	 * �N���C�A���g�f�[�^�Ǘ��N���X
	 */
	class ClientData {
	public:
		/**
		 * ���������܂��B
		 * @param[in] ClientSocket �\�P�b�g
		 * @param[in] pServer �T�[�o�[�I�u�W�F�N�g�̃C���X�^���X
		 */
		ClientData(SOCKET ClientSocket, XSocketTCPServer* pServer);
		/** �N���C�A���g�T�C�h�\�P�b�g */
		XSocketTCPClinet      m_Client;
		/** �T�[�o�[�I�u�W�F�N�g�̃C���X�^���X�̃A�h���X */
		XSocketTCPServer*     m_pServer;
		/** �N���C�A���g�X���b�h�I�u�W�F�N�g */
		XThread*              m_pThread;
	};


	/** �R���X�g���N�^ */
	XSocketTCPServer();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketTCPServer(SOCKET Socket);
	/** �f�X�g���N�^ */
	virtual ~XSocketTCPServer();
	/** @copydoc XSocket::Socket() */
	virtual bool Socket();
	/** @copydoc XSocket::Close() */
	virtual bool Close();
	/** @copydoc XSocketServer::Open(XSocketAddress&, XSOCK_CLIENTPROC, void*, int) */
	virtual void Open(XSocketAddress& address, XSOCK_CLIENTPROC pFnc, void *pUserData, int nBroking = 5);
	/** @copydoc XThread::RunUndetached(void*) */
	virtual void* RunUndetached(void* pArg); 

	/**
	 * �N���C�A���g�\�P�b�g�Ə��̈ꗗ���R�[���o�b�N�֐��Ŏ擾���܂��B
	 * @param[in] pFunc �R�[���o�b�N�֐��̃A�h���X�B
	 * @param[in] pUserData  pFunc�̃R�[���o�b�N�֐��̃��[�U�[��`�f�[�^�Ɉ����n�����A�f�[�^�B
	 */
	void ClientDataAccess(void (*pFunc)(unsigned long, unsigned long, ClientData*, void*), void *pUserData = NULL);
private:

	/* �N���C�A���g�f�[�^�̔z�� */
	XList					m_arrayClientDatas;
	/* �N���C�A���g�f�[�^�A�N�Z�X�p���b�N�I�u�W�F�N�g */
	XMutex					m_lockClientData;
	/* Accept�p���b�N�I�u�W�F�N�g */
	XSemaphore				m_lockAcceptThread;

	/**	@brief	�T�[�o�[���I������ۃN���C�A���g�̏I����҂����I�u�W�F�N�g */
	XSemaphore				*m_pTerminateSync;
	/*
	 * �N���C�A���g�f�[�^�̒ǉ�
	 * @param[in] pData �N���C�A���g�f�[�^
	 */
	void AddClientData(ClientData* pData);
	/*
	 * �N���C�A���g�f�[�^�̍폜
	 * @param[in] pData �N���C�A���g�f�[�^
	 */
	void DelClientData(ClientData* pData);
	/*
	 * �N���C�A���g�p�X���b�h���[�`��
	 * @param[in] pData �N���C�A���g�f�[�^
	 * preturn �߂�l
	 */
	static void* RunClient(void *pData);
};


/**
 * UDP�\�P�b�g�N���X
 */
class XLIB_API XSocketUDPClinet : public XSocketClinet
{
public:
	/** �R���X�g���N�^ */
	XSocketUDPClinet();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketUDPClinet(SOCKET Socket);
	/** �f�X�g���N�^ */
	virtual ~XSocketUDPClinet();
	/** @copydoc XSocket::Socket() */
	virtual bool Socket();
	/** @copydoc XSocketClinet::Open(XSocketAddress&) */
	virtual bool Open(XSocketAddress& address);
	/** @copydoc XSocket::Close() */
	virtual bool Close();
private:
	/** �N���[�Y�������ǂ����̃t���O */
	bool        m_bClose;
	friend class XSocketUDPServer;

};


/**
 * UDP�\�P�b�g�T�[�o�[�N���X
 */
class XLIB_API XSocketUDPServer : public XSocketServer , XThread
{
public:
	/** �R���X�g���N�^ */
	XSocketUDPServer();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketUDPServer(SOCKET Socket);
	/** �f�X�g���N�^ */
	virtual ~XSocketUDPServer();
	/** @copydoc XSocket::Socket() */
	virtual bool Socket();
	/** @copydoc XSocket::Close() */
	virtual bool Close();
	/** @copydoc XSocketServer::Open(XSocketAddress&, XSOCK_CLIENTPROC, void*, int) */
	virtual void Open(XSocketAddress& address, XSOCK_CLIENTPROC pFnc, void *pUserData, int nBroking = 5);
	virtual void* RunUndetached(void* pArg); 
private:
	/** Accept�p���b�N�I�u�W�F�N�g */
	XSemaphore            m_lockAcceptThread;
};




#include "xlib/xlib_tail.h"
#endif // __X_SOCKET_HEASER_FILE__

