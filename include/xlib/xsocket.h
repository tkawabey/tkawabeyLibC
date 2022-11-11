/**
 * @file xsocket.h
 * @brief ソケット
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
 * ソケットライブラリの初期化を行います
 */
class XLIB_API XSocketInit
{
public:
// 構築/消滅
	/**
	 * コンストラクタ
	 * param wVersionRequested ソケットのバージョン指定する場合は１を指定します
	 */
	XSocketInit(WORD wVersionRequested = 0);
	/** デストラクタ */
	~XSocketInit();
protected:
	/** winsock データ */
	WSADATA m_wsaData;
};
#endif /* WIN32 */





/**
 * ソケットアドレス構造体のラップクラス
 */
class XLIB_API XSocketAddress : public sockaddr_in
{
public:
// Construction
	/**
	 * コンストラクタ
	 */
	XSocketAddress();
	/**
	 * コンストラクタ
	 * @param nAddress IPアドレス
	 * @param iPort ポート番号
	 * @param iFamily アドレスファミリー
	 */
	XSocketAddress(unsigned int nAddress, unsigned short iPort, short iFamily = AF_INET);
	/**
	 * コンストラクタ
	 * @param lpszHostName ホスト名
	 * @param iPort ポート番号
	 * @param iFamily アドレスファミリー
	 */
	XSocketAddress(const char* lpszHostName, unsigned short iPort, short iFamily = AF_INET);
	/**
	 * コンストラクタ
	 * @param pAddr ソケットアドレス構造体インスタンスのアドレス
	 */
	XSocketAddress(const struct sockaddr_in* pAddr);
	/**
	 * コンストラクタ
	 * @param pAddr ソケットアドレス構造体インスタンスのアドレス
	 */
	XSocketAddress(const struct sockaddr* pAddr);
	/** デストラクタ */
	virtual ~XSocketAddress();

// Operation
	XSocketAddress& operator=(XSocketAddress& address);
	/**
	 * ソケットアドレスの初期化
	 * @param nAddress IPアドレス
	 * @param iPort ポート番号
	 * @param iFamily アドレスファミリー
	 */
	void SetAddress(unsigned int nAddress, unsigned short iPort, short iFamily = AF_INET);
	/**
	 * ソケットアドレスの初期化
	 * @param lpszHostName ホスト名
	 * @param iPort ポート番号
	 * @param iFamily アドレスファミリー
	 */
	void SetAddress(const char* lpszHostName, unsigned short iPort, short iFamily = AF_INET);
	/**
	 * ソケットアドレスの初期化
	 * @param pAddr ソケットアドレス構造体インスタンスのアドレス
	 */
	void SetAddress(const struct sockaddr_in* pAddr);
	/**
	 * ソケットアドレスの初期化
	 * @param pAddr ソケットアドレス構造体インスタンスのアドレス
	 */
	void SetAddress(const struct sockaddr* pAddr);

	/**
	 * アドレスのFQDNを取得します。
	 * @return アドレスのFQDN
	 */
	XStrBuff GetFQDN() const;
	/**
	 * IPアドレスを文字列として取得します。
	 * @return IPアドレスの文字列
	 */
	XStrBuff GetIPAddress() const;
	/**
	 * ソケットアドレス構造体を取得します。
	 * @return ソケットアドレス構造体のアドレス
	 */
	operator sockaddr_in*();
	/**
	 * ソケットアドレス構造体を取得します。
	 * @return ソケットアドレス構造体のアドレス
	 */
	operator sockaddr*();
protected:
	/** エイリアス一覧 */
	XStrBuffList              m_Aliases;
	/** IPアドレスオブジェクト配列 */
	std::vector<XSocketAddress*>   m_IPAddresses;
};


/**
 * ソケットアドレス構造体のラップクラス
 */
class XLIB_API XSocket : public XIO
{
public:
	/**
	 * コンストラクタ
	 */
	XSocket();
	/**
	 * コンストラクタ
	 * @param Socket ソケット
	 */
	XSocket(SOCKET Socket);
	/**
	 * 指定したソケットで、インスタンスを初期化します。
	 * @param Socket ソケット
	 */
	void Attach(SOCKET Socket);
	/**
	 * インスタンス内部のソケットを切り離します
	 * @return Socket ソケット
	 */
	SOCKET Detach();

// overlap
	/**
	 * ソケットを閉じます。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Close();
	/**
	 * バッファを書き込みます。実装部
	 * @param lpData 書き込むバッファ
	 * @param nSize lpDataのサイズ
	 * @param flg オプションフラグ。MSG_OOBや、MSG_PEEKなどが指定可能。詳しくは、ソケット:recvのマニュアルを参照]
	 * @return 書き込んだバイト数。読み込みエラーが発生した場合は、-１を返します。
	 */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0);
	/**
	 * バッファを読み込みます。実装部
	 * @param lpData 読み込むバッファを格納するのアドレス
	 * @param nSize lpDataのサイズ
	 * @param flg オプションフラグ。MSG_OOBや、MSG_PEEKなどが指定可能。詳しくは、ソケット:sendのマニュアルを参照]
	 * @return 読み込んだバイト数。読み込みエラーが発生した場合は、-１を返します。
	 */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0);

public:
// Low Operation
	/**
	 * ソケットを構築します。
	 * @param[in] type ソケットタイプ
	 * @param[in] af アドレスファミリー
	 * @param[in] protocol プロトコル
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Socket(int type, int af = AF_INET, int protocol = IPPROTO_TCP);
	/**
	 * ソケットを構築します。派生クラスでインプリします。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Socket(){return true;}

	/**
	 * ソケットに名前をつけます
	 * @param[in] address ソケットアドレス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Bind(XSocketAddress& address);
	/**
	 * ソケットの接続を行います。
	 * @param[in] address ソケットアドレス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Connect(XSocketAddress& address);
	/**
	 * 全二重接続の一部を遮断します。
	 * @param[in] nHow SHUT_RD ならば、それ以降の受信を禁止する。SHUT_WR ならば、それ以降の送信を禁止する。SHUT_RDWR ならば、それ以降の送受信を禁止する。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool ShutDown(int nHow);

	
	/**
	 * ソケットからデータを取得します。
	 * @note 接続思考でないUDPなどのプロトコルで使用します。
	 * @param[in] lpData ユーザーサイドで確保したデータのバッファのアドレス
	 * @param[in] nSize lpDataで指定したデータのサイズ
	 * @param[in] address 受信先アドレス
	 * @param[in] flags オプションフラグ。MSG_OOBや、MSG_PEEKなどが指定可能。詳しくは、ソケット:recvのマニュアルを参照]
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool ReadFrom(void* lpData, unsigned long nSize, XSocketAddress& address, int flags = 0);
	/**
	 * ソケットに指定された文字列を書き込みます。
	 * @note 接続思考でないUDPなどのプロトコルで使用します。
	 * @param[in] lpData ユーザーサイドで確保したデータのバッファのアドレス
	 * @param[in] nSize lpDataで指定したデータのサイズ
	 * @param[in] address 送信先のアドレス
	 * @param[in] flags オプションフラグ。MSG_OOBや、MSG_PEEKなどが指定可能。詳しくは、ソケット:sendのマニュアルを参照]
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool WriteTo(const void* lpData, unsigned long nSize, XSocketAddress &address, int flags = 0);



	/**
	 * 接続思考のソケットで、接続された、ソケットの相手のアドレスを取得します。
	 * @param[out] address ソケットアドレス情報
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetPeerName(XSocketAddress& address);
	/**
	 * ソケットのアドレスを取得します。
	 * @param[out] address ソケットアドレス情報
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetSockName(XSocketAddress& address);
	/**
	 * ソケットを制御します。
	 * @param[out] cmd 制御コマンド
	 * @param[out] argp コマンドによるパラメータ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool IOCtrl(long cmd = FIONREAD, void* argp = 0 );
	/**
	 * ソケットオプションを取得します。
	 * @param[out] optval1 オプションの値を取得するユーザーサイドで確保した、バッファのアドレス
	 * @param[in,out] optlen optval1で指定した、バッファのサイズを指定します。関数が成功した場合は、実際取得したサイズがセットされます。
	 * @param[in] optname オプション名の値
	 * @param[in] level プロトコル層
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetSocketOpt(void* optval1, socklen_t *optlen, int optname, int level = SOL_SOCKET);
	/**
	 * ソケットオプションを取得します。
	 * @param[in] optval1 設定するオプション値
	 * @param[in] optlen optval1で指定した、バッファのサイズを指定します。
	 * @param[in] optname オプション名の値
	 * @param[in] level プロトコル層
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SetSocketOpt(void* optval1, int optlen , int optname, int level = SOL_SOCKET);
	/**
	 * ソケットが初期化されているか、（通信のための端点(endpoint)を作成されているか）調べます。
	 * @retval true 作成されている
	 * @retval false 作成されていない
	 */
	bool IsValid();

// Server Side
	/**
	 * ソケット上の接続を待ちます。
	 * @param[in] nBacklog 保留中の接続のキューが拡張することのできる最大長を指定します。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Listen(int nBacklog);
	/**
	 * ソケットへの接続を受けるます。
	 * @param[in] pClient 受け入れたソケット取得用のXSocketまたは、その派生クラスのインスタンスのアドレス。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Accept(XSocket *pClient);

// SOCKET OPTIONS
	/**
	 * ソケットで、ブロードキャストメッセージの転送を許可の有無を取得します
	 * @retval true 許可する場合
	 * @retval false 許可しない場合
	 */
	bool get_Broadcast();
	/**
	 * ソケットで、ブロードキャストメッセージの転送を許可を設定します
	 * @param[in] newVal 許可する場合はtrue。許可しない場合はfalseを指定。
	 */
	void put_Broadcast(bool newVal);
	/**
	 * デバッグ情報を記録の有無を取得します
	 * @retval true 記録する場合
	 * @retval false 記録しない場合
	 */
	bool get_Debug();
	/**
	 * デバッグ情報を記録の有無を設定します
	 * @param[in] newVal 記録する場合はtrue。記録しない場合はfalseを指定。
	 */
	void put_Debug(bool newVal);

	/**
	 * ルーチングしません。直接インターフェイスに送信のOn/Offを取得します
	 * @retval true On
	 * @retval false Off
	 */
	bool get_DontRoute();
	/**
	 * ルーチングしません。直接インターフェイスに送信のOn/Offを設定します
	 * @param[in] newVal Onはtrue。Offはfalseを指定。
	 */
	void put_DontRoute(bool newVal);
	/**
	 * 接続保持パケットを送信のOn/Offを取得します。
	 * @retval true On
	 * @retval false Off
	 */
	bool get_KeepAlive();
	/**
	 * 接続保持パケットを送信のOn/Offを設定します。
	 * @param[in] newVal Onはtrue。Offはfalseを指定。
	 */
	void put_KeepAlive(bool newVal);
	/**
	 * 未送信データがあるとき、Closeを遅延の設定を取得します。
	 * @param[in] pLinger LINGER構造体のアドレス
	 */
	void get_Linger(LINGER *pLinger);
	/**
	 * 未送信データがあるとき、Closeを遅延の設定を設定します
	 * @param[in] pLinger LINGER構造体のアドレス
	 */
	void put_Linger(const LINGER *pLinger);
	/**
	 * 通常のデータストリームで帯域外データを受信のOn/Offを取得します。
	 * @retval true On
	 * @retval false Off
	 */
	bool get_Oobline();
	/**
	 * 通常のデータストリームで帯域外データを受信のOn/Offを設定します。
	 * @param[in] newVal Onはtrue。Offはfalseを指定。
	 */
	void put_Oobline(bool newVal);
	/**
	 * 受信用のバッファサイズをを取得します。/Offを取得します。
	 * @return 受信用のバッファサイズ
	 */
	int  get_RcvBuf();
	/**
	 * 受信用のバッファサイズをを設定します。
	 * @param[in] newVal 受信用のバッファサイズ
	 */
	void put_RcvBuf(int newVal);
	/**
	 * ソケットを使用中のアドレスにバインドのOn/Offを取得します。
	 * @retval true On
	 * @retval false Off
	 * @note ポートハイジャック
	 */
	bool get_ReuseAddr();
	/**
	 * ソケットを使用中のアドレスにバインドのOn/Offを設定します。
	 * @param[in] newVal Onはtrue。Offはfalseを指定。
	 * @note ポートハイジャック
	 */
	void put_ReuseAddr(bool newVal);

	/**
	 * ポートの再利用を禁止で、バインドするかどうかを得る
	 * @retval true On
	 * @retval false Off
	 * @note ポートハイジャックを禁止する。
	 */
	bool get_ExclusiveAddr();

	/**
	 * ポートの再利用を禁止で、バインドするかどうかを設定する。
	 * @param[in] newVal Onはtrue。Offはfalseを指定。
	 * @note ポートハイジャックを禁止する。
	 */
	void put_ExclusiveAddr(bool newVal);
	/**
	 * 送信用のバッファサイズをを取得します。/Offを取得します。
	 * @return 送信用のバッファサイズ
	 */
	int  get_SndBuf();
	/**
	 * 送信用のバッファサイズをを設定します。
	 * @param[in] newVal 送信用のバッファサイズ
	 */
	void put_SndBuf(int newVal);
	/**
	 * 受信時のタイムアウト値を設定します。
	 * @return 受信時のタイムアウト値
	 */
	int  get_RcvTimeO();
	/**
	 * 受信時のタイムアウト値を設定します。
	 * @param[in] newVal 受信時のタイムアウト値
	 */
	void put_RcvTimeO(int newVal);
	/**
	 * 送信時のタイムアウト値を取得します。
	 * @return 送信時のタイムアウト値
	 */
	int  get_SndTimeO();
	/**
	 * 送信時のタイムアウト値を設定します。
	 * @param[in] newVal 送信時のタイムアウト値
	 */
	void put_SndTimeO(int newVal);
#if defined(WIN32)
	/**
	 * Closeは未送信データの送信を待を待たないの有無。
	 * @note このオプションを設定することは、l_onoffを0にしてSO_LINGERを設定することと同じです。
	 * @retval true 記録する場合
	 * @retval false 記録しない場合
	 */
	bool get_DontLinger();
	/**
	 * Closeは未送信データの送信を待を待たないの有無。
	 * @note このオプションを設定することは、l_onoffを0にしてSO_LINGERを設定することと同じです。
	 * @param[in] newVal 未送信データの送信を待を待たないの有有する場合はtrue。未送信データの送信を待を待たないの有有しない場合はfalseを指定。
	 */
	void put_DontLinger(bool newVal);

	/**
	 * 将来の為に予約されています。現在は使用できません。
	 * @return プライオリティ
	 */
	int  get_GroupPriority();
	/**
	 * 将来の為に予約されています。現在は使用できません。
	 * @param[in] newValプライオリティ
	 */
	void put_GroupPriority(int newVal);

	/**
	 * 送信データをまとめて送るためのNagleアルゴリズムを無効のOn/Offを取得します。
	 * @retval true On
	 * @retval false Off
	 */
	bool get_NoDelay();
	/**
	 * 送信データをまとめて送るためのNagleアルゴリズムを無効のOn/Offを設定します。
	 * @param[in] newVal Onはtrue。Offはfalseを指定。
	 */
	void put_NoDelay(bool newVal);
#endif /*WIN32*/

	/**	@brief	8バイトのホストコードの値をネットワークバイトオーダーに変更する。
	 *	@param	val	ホストコードの値
	 *	@return	ネットワークバイトオーダーの値
	 */
	static double htond(double val);
	/**	@brief	8バイトのをネットワークバイトオーダーの値をホストコードに変更する。
	 *	@param	val	ホストコードの値
	 *	@return	ネットワークバイトオーダーの値
	 */
	static double ntohd(double val);
	/**	@brief	4バイトのホストコードの値をネットワークバイトオーダーに変更する。
	 *	@param	val	ホストコードの値
	 *	@return	ネットワークバイトオーダーの値
	 */
	static unsigned long _htonl(unsigned long val);
	/**	@brief	4バイトのをネットワークバイトオーダーの値をホストコードに変更する。
	 *	@param	val	ホストコードの値
	 *	@return	ネットワークバイトオーダーの値
	 */
	static unsigned long _ntohl(unsigned long val);
	/**	@brief	2バイトのホストコードの値をネットワークバイトオーダーに変更する。
	 *	@param	val	ホストコードの値
	 *	@return	ネットワークバイトオーダーの値
	 */
	static unsigned short _htons(unsigned short val);
	/**	@brief	2バイトのをネットワークバイトオーダーの値をホストコードに変更する。
	 *	@param	val	ホストコードの値
	 *	@return	ネットワークバイトオーダーの値
	 */
	static unsigned short _ntohs(unsigned short val);
protected:
	/* ソケット */
	SOCKET                     m_Socket;
	/* エラー */
	unsigned long              m_dwLastError;
	/* エラー */
	unsigned long              m_dwOwnError;
	friend class XSocketExcept;
};





/**
 * クライアント、ソケット、ベース、クラス
 */
class XLIB_API XSocketClinet : public XSocket
{
public:
	/** コンストラクタ */
	XSocketClinet();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketClinet(SOCKET Socket);
	/** デストラクタ */
	virtual ~XSocketClinet();
	/**
	 * ソケットをオープンします。
	 * @param[in] address アドレス
	 * @retval true 成功
	 * @retval false 失敗
	 * @exception XSocketExcept* 例外データ
	 */
	virtual bool Open(XSocketAddress& address) = 0;
};


/**
 * サーバー、ソケット、ベース、クラス
 */
class XLIB_API XSocketServer : public XSocket
{
public:
	/** コンストラクタ */
	XSocketServer();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketServer(SOCKET Socket);
	/** デストラクタ */
	virtual ~XSocketServer();
	/**
	 * サーバーソケットをオープンします。
	 * @param[in] address アドレス
	 * @param[in] pFnc クライアントサイド処理の関数アドレス
	 * @param[in] pUserData pFncのpUserDataに渡すユーザー定義データのアドレス。
	 * @param[in] nBroking 保留中の接続のキューが拡張することのできる最大長を指定します。
	 * @exception XExcept& 例外データ
	 */
	virtual void Open(XSocketAddress& address, XSOCK_CLIENTPROC pFnc, void *pUserData,  int nBroking = 5) = 0;
	/** クライアントサイド処理の関数アドレス */
	XSOCK_CLIENTPROC m_pFnClient;
	/** pFncのpUserDataに渡すユーザー定義データのアドレス。 */
	void*   m_pUserData;
};


/**
 * TCPソケットクラス
 */
class XLIB_API XSocketTCPClinet : public XSocketClinet
{
public:
	/** コンストラクタ */
	XSocketTCPClinet();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketTCPClinet(SOCKET Socket);
	/** デストラクタ */
	virtual ~XSocketTCPClinet();
	/** @copydoc XSocket::Socket() */
	virtual bool Socket();
	/** @copydoc XSocketClinet::Open(XSocketAddress&) */
	virtual bool Open(XSocketAddress& address);
};


/**
 * TCPソケットサーバークラス
 */
class XLIB_API XSocketTCPServer : public XSocketServer , XThread
{
public:
	/**
	 * クライアントデータ管理クラス
	 */
	class ClientData {
	public:
		/**
		 * 初期化します。
		 * @param[in] ClientSocket ソケット
		 * @param[in] pServer サーバーオブジェクトのインスタンス
		 */
		ClientData(SOCKET ClientSocket, XSocketTCPServer* pServer);
		/** クライアントサイドソケット */
		XSocketTCPClinet      m_Client;
		/** サーバーオブジェクトのインスタンスのアドレス */
		XSocketTCPServer*     m_pServer;
		/** クライアントスレッドオブジェクト */
		XThread*              m_pThread;
	};


	/** コンストラクタ */
	XSocketTCPServer();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketTCPServer(SOCKET Socket);
	/** デストラクタ */
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
	 * クライアントソケットと情報の一覧をコールバック関数で取得します。
	 * @param[in] pFunc コールバック関数のアドレス。
	 * @param[in] pUserData  pFuncのコールバック関数のユーザー定義データに引き渡される、データ。
	 */
	void ClientDataAccess(void (*pFunc)(unsigned long, unsigned long, ClientData*, void*), void *pUserData = NULL);
private:

	/* クライアントデータの配列 */
	XList					m_arrayClientDatas;
	/* クライアントデータアクセス用ロックオブジェクト */
	XMutex					m_lockClientData;
	/* Accept用ロックオブジェクト */
	XSemaphore				m_lockAcceptThread;

	/**	@brief	サーバーが終了する際クライアントの終了を待つ同期オブジェクト */
	XSemaphore				*m_pTerminateSync;
	/*
	 * クライアントデータの追加
	 * @param[in] pData クライアントデータ
	 */
	void AddClientData(ClientData* pData);
	/*
	 * クライアントデータの削除
	 * @param[in] pData クライアントデータ
	 */
	void DelClientData(ClientData* pData);
	/*
	 * クライアント用スレッドルーチン
	 * @param[in] pData クライアントデータ
	 * preturn 戻り値
	 */
	static void* RunClient(void *pData);
};


/**
 * UDPソケットクラス
 */
class XLIB_API XSocketUDPClinet : public XSocketClinet
{
public:
	/** コンストラクタ */
	XSocketUDPClinet();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketUDPClinet(SOCKET Socket);
	/** デストラクタ */
	virtual ~XSocketUDPClinet();
	/** @copydoc XSocket::Socket() */
	virtual bool Socket();
	/** @copydoc XSocketClinet::Open(XSocketAddress&) */
	virtual bool Open(XSocketAddress& address);
	/** @copydoc XSocket::Close() */
	virtual bool Close();
private:
	/** クローズしたかどうかのフラグ */
	bool        m_bClose;
	friend class XSocketUDPServer;

};


/**
 * UDPソケットサーバークラス
 */
class XLIB_API XSocketUDPServer : public XSocketServer , XThread
{
public:
	/** コンストラクタ */
	XSocketUDPServer();
	/** @copydoc XSocket::XSocket(SOCKET) */
	XSocketUDPServer(SOCKET Socket);
	/** デストラクタ */
	virtual ~XSocketUDPServer();
	/** @copydoc XSocket::Socket() */
	virtual bool Socket();
	/** @copydoc XSocket::Close() */
	virtual bool Close();
	/** @copydoc XSocketServer::Open(XSocketAddress&, XSOCK_CLIENTPROC, void*, int) */
	virtual void Open(XSocketAddress& address, XSOCK_CLIENTPROC pFnc, void *pUserData, int nBroking = 5);
	virtual void* RunUndetached(void* pArg); 
private:
	/** Accept用ロックオブジェクト */
	XSemaphore            m_lockAcceptThread;
};




#include "xlib/xlib_tail.h"
#endif // __X_SOCKET_HEASER_FILE__

