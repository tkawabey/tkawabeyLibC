/**
 * @file xmsgqueue.h
 * @brief メッセージキュークラス
 *
 * @author T.Fujiha
 * @date 2007/07/15
 * @version 1.0
 *
 * Copyright (C) 2007 T.Fujiha,
 */
#ifndef __X_MESSAGE_QUEUE_FILE__
#define __X_MESSAGE_QUEUE_FILE__


#include "xlib/xlib_head.h"
#include "xlib/xio.h"
#include "xlib/xlock.h"
#include "xlib/xbuff.h"
#include "xlib/xthread.h"
#include "xlib/xmap.h"

class XLIB_API XQueueMsg;
class XLIB_API XQueueMsgCS;
class XLIB_API XQueue;
struct XMsgQueueBuff;
class XLIB_API XIPCQueuePuller;
class XLIB_API XIPCQueuePusher;



/**	@brief	実行ステータス */
enum XMsgQueueERR
{
		XMsgQueueERR_SUCCESS			///<	エラーは発生していない
	,	XMsgQueueERR_OPEN_DEVICE		///<	デバイスのオープンに失敗
	,	XMsgQueueERR_GET_QUEUE_INFO		///<	デバイスから情報を取得できない
	,	XMsgQueueERR_SET_QUEUE_INFO		///<	Queue情報の設定に失敗した
	,	XMsgQueueERR_NOT_OPEND			///<	openが成功していない
	,	XMsgQueueERR_PULL				///<	キューからメッセージの読み取りでエラーが発生した。
	,	XMsgQueueERR_WRITE				///<	デバイスへの書き込みにでエラーが発生
	,	XMsgQueueERR_NAK				///<	Peerによって操作が拒否されました。
	,	XMsgQueueERR_RCV_DATA_BROKEN	///<	受信データが壊れていた
	,	XMsgQueueERR_RCV_TIMEO			///<	受信タイムアウト
};

#define XMsgQueueCS_REQUEST				0
#define XMsgQueueCS_RESPONSE			1
#define XMsgQueueCS_EVENT				2
#define XMsgQueueCS_CLINET_CONNECT		50
#define XMsgQueueCS_CLINET_CONNECT_OK	51
#define XMsgQueueCS_CLINET_CONNECT_NG	52
#define XMsgQueueCS_CLINET_DISCONNECT	60
#define XMsgQueueCS_CLINET_TERMINATE	(XD_UINT2)-1

#define XQueue_MsgID_ANY				(XD_UINT4)-1

#define XQueue_TIMEOUT_NOWAIT			0
#define XQueue_TIMEOUT_FOREVER			(XD_UINT4)-1

#define XQueue_POP_CALLBCK_IGNORE		0	///<	popコールバック関数で処理しなかった
#define XQueue_POP_CALLBCK_PROCESS		1	///<	popコールバック関数で処理した


#define XMsgQueueCS_SIZE				10
#define XMsgQueueCS_POS_CLINET_ID		2

struct XMsgQueueCS {
    unsigned short	type;			// メッセージタイプ
	XD_UINT1		clientid[8];	// クライアントID
};




/**	@brief	キューメッセージ	*/
class XLIB_API XQueueMsg 
{
protected:
	XQueueMsg(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);
	virtual ~XQueueMsg();
public:
	/**	@brief	このインスタンスを削除します。 */
	virtual void deleteMe();

	/**	@brief	メッセージIDを取得します。
	 *
	 *	@return	メッセージID
	 */
	XD_UINT4 getMsgID() const;

	/**	@brief	データサイズを取得します。
	 *
	 *	@return	データサイズ
	 */
	XD_UINT4 getDataSize() const;

	/**	@brief	データのアドレスを取得します。
	 *
	 *	@return	データのアドレス
	 */
	const char* getData() const;
protected:
	XD_UINT4	m_ulMsgID;
	XD_UINT4	m_ulMsgDataSize;
	char*		m_pMsgData;
friend class XLIB_API XQueue ;
friend class XLIB_API XQueueMsgCS;
};



/**	@brief	キュー
@verbatim
{
	XQueue	queue;
	XQueueMsg	*pMsg;
	XD_UINT4 ulMsgId = 0;
	char	szData[5000];

	memset(szData, 'a', sizeof(szData));



	// 自分で入れて、
	ulMsgId++;
	queue.push(ulMsgId, szData, sizeof(szData));
	// 自分で取り出す
	pMsg = queue.pop(ulMsgId);
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == ulMsgId );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	// いらなくなったら削除する。
	pMsg->deleteMe();

	// キューにデータをpushするスレッドを開始
	ThreadTestXQueue*	thread = new ThreadTestXQueue(queue);
	thread->start();

	// スレッドが、メッセージID：２番で書き込み、それを取得
	pMsg = queue.pop( 2 );
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 2 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	// いらなくなったら削除する。
	pMsg->deleteMe();

	// エントリーに入っているメッセージの先頭を取得
	pMsg = queue.pop();
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 1 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();

	//  メッセージID：3番でまつ、書き込みがないのでタイムアウトするはず
	pMsg = queue.pop(3, 1000);
	CPPUNIT_ASSERT( pMsg == NULL );

	thread->Join(0);

	// popコールバック関数を登録します。
	queue.connectPopCallBack(_PopCallBack, &iUserData);

	queue.push(10, szData, sizeof(szData));
	queue.push(11, szData, sizeof(szData));
	queue.push(12, szData, sizeof(szData));
	queue.push(13, szData, sizeof(szData));
	CPPUNIT_ASSERT( iUserData == 4 );	// ユーザーデータが更新されているか確認
	// メッセージＩＤ：１２番だけは無視されているはずなので、popで取得できる。
	pMsg = queue.pop();
	CPPUNIT_ASSERT( pMsg != NULL );
	CPPUNIT_ASSERT( pMsg->getMsgID() == 12 );
	CPPUNIT_ASSERT( pMsg->getDataSize() == sizeof(szData) );
	CPPUNIT_ASSERT( memcmp(pMsg->getData(), szData, pMsg->getDataSize()) == 0  );
	pMsg->deleteMe();
}


// popコールバック関数
int	_PopCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize,void* pUserData)
{
	int* pAddr = (int*)pUserData;

	// ユーザーデータで、コールバックがコールされた回数をカウントアップ
	(*pAddr)++;

	if( ulMsgID == 12 ) {
		// メッセージＩＤ：１２番は無視する。
		return XQueue_POP_CALLBCK_IGNORE;
	}

	return XQueue_POP_CALLBCK_PROCESS;
}
	...

// キューにデータをpushするスレッド
class ThreadTestXQueue : public XThread
{
public:
	ThreadTestXQueue(XQueue& Q)
		:	queue( Q )
	{}
	~ThreadTestXQueue(){}
	void start(){ StartUndetached(); }
	virtual void* RunUndetached(void* pArg) {
		XD_UINT4 ulMsgId = 0;
		char	szData[5000];
		memset(szData, 'a', sizeof(szData));

		
		// メッセージID　1番で書き込み
		queue.push(1, szData, sizeof(szData));

		XThread::Sleep(2, 0);

		// メッセージID　2番で書き込み
		queue.push(2, szData, sizeof(szData));

		
		return NULL;
	
	}
	XQueue& queue;
};
@endverbatim
*/
class XLIB_API XQueue 
{
public:
	XQueue();
	virtual ~XQueue();

	/**	@brief	キューエントリーを空にします。 */
	void clear();
	
	/**	@brief	キューにメッセージをセットします。
	 *
	 *	@param	ulMsgID	メッセージID
	 *	@param	pData	データ
	 *	@param	ulDataSize	データのサイズ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool push(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);

	/**	@brief	キューからメッセージを取り出しします。
	 *
	 *	@param	ulMsgID	取り出すメッセージID。
	 *				- XQueue_MsgID_ANYを指定すると、キューの先頭にあるメッセージを取り出す。
	 *	@param	ulTimeout	取り出すまでのタイムアウト値(ミリ秒)を指定。
	 *				- XQueue_TIMEOUT_NOWAITは、キューにデータがない場合、待たずに直ぐにリターンする。
	 *				- XQueue_TIMEOUT_FOREVERは、無限に待つ。
	 *
	 *	@return	メッセージ。タイムアウトした場合は、NULL。
	 */
	XQueueMsg* pop(XD_UINT4 ulMsgID = XQueue_MsgID_ANY, XD_UINT4 ulTimeout = XQueue_TIMEOUT_FOREVER);

	/**	@brief	popコールバック関数を登録します。
	 *	@note	popコールバック関数を登録すると、呼び出し元は、pop関数をコールしなくても、
	 *			コールバックからデータを得ることばできます。コールバック関数の型は以下の通り
	 *			- int	PopCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize, void* pUserData);
	 *			 - パラメータ:
	 *			  - ulMsgID メッセージID
	 *			  - pData データ
	 *			  - ulDataSize データサイズ
	 *			  - pUserData ユーザーデータ
	 *			 - 戻り値:
	 *			  - XQueue_POP_CALLBCK_IGNORE	処理を無視した。呼び出し元は、pop関数をコールして、
	 *													データの処理を行わなければ行けない
	 *			  - XQueue_POP_CALLBCK_PROCESS	データを処理した。
	 *	@param	pPopCallBack	popコールバック関数。登録解除する場合は、NULLを指定します。	
	 *	@param	pUserData	popコールバック関数に渡されるユーザーデータを指定します。
	 */
	void connectPopCallBack(int (*pPopCallBack)(XD_UINT4, char*, XD_UINT4,void*), void* pUserData);
private:
	class popperEntry{
	public:
		popperEntry(XD_UINT4 ulMsgID, XQueueMsg** ppMsg);
		~popperEntry();
		XD_UINT4	m_ulMsgID;
		XQueueMsg**		m_ppMsg;
		XSemaphore	m_lock;
	};
	XMutex		m_push_lock;
	XMutex		m_pop_lock;
	XMutex		m_popperEntry_lock;
	XList		m_list;
	XList		m_listPopperEntry;
	int			(*m_pPopCallBack)(XD_UINT4, char*, XD_UINT4,void*);
	void*		m_pPopCallBackUserData;
friend class XQueueMsg;
};





/** @brief キューからメッセージを取得するクラス(IPC:InterProcess Communication)	*/
class XLIB_API XIPCQueuePuller
{
public:
	/** @brief コンストラクタ*/
	XIPCQueuePuller();
	virtual ~XIPCQueuePuller();

	
	/**	@brief	キューをオープンします。
	 *
	 *	@param	pName	一意に識別する名前を指定します。
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR open(const char* pName);
	/**	@brief	キューをオープンします。
	 *
	 *	@param	pName	一意に識別する名前を指定します。
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR open(const wchar_t* pName);
	/**	@brief	キューを閉じます
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR close();
	/**	@brief	キューからメッセージを取り出しします。
	 *
	 *	@param	ulMsgID	取り出すメッセージID。
	 *				- XQueue_MsgID_ANYを指定すると、キューの先頭にあるメッセージを取り出す。
	 *	@param	ulTimeout	取り出すまでのタイムアウト値を指定。
	 *				- XQueue_TIMEOUT_NOWAITは、キューにデータがない場合、待たずに直ぐにリターンする。
	 *				- XQueue_TIMEOUT_FOREVERは、無限に待つ。
	 *
	 *	@return	メッセージ。タイムアウトした場合は、NULL。
	 */
	XQueueMsg* pop(XD_UINT4 ulMsgID = XQueue_MsgID_ANY, XD_UINT4 ulTimeout = XQueue_TIMEOUT_FOREVER);

	/**	@brief	キューにあるメッセージの数を返します。
	 *	@return	メッセージ数
	 */
	XD_UINT4 getMessageCount();

	/**	@brief	popコールバック関数を登録します。
	 *	@note	popコールバック関数を登録すると、呼び出し元は、pop関数をコールしなくても、
	 *			コールバックからデータを得ることばできます。コールバック関数の型は以下の通り
	 *			- int	PopCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize, void* pUserData);
	 *			 - パラメータ:
	 *			  - ulMsgID メッセージID
	 *			  - pData データ
	 *			  - ulDataSize データサイズ
	 *			  - pUserData ユーザーデータ
	 *			 - 戻り値:
	 *			  - XQueue_POP_CALLBCK_IGNORE	処理を無視した。呼び出し元は、pop関数をコールして、
	 *													データの処理を行わなければ行けない
	 *			  - XQueue_POP_CALLBCK_PROCESS	データを処理した。
	 *	@param	pPopCallBack	popコールバック関数。登録解除する場合は、NULLを指定します。	
	 *	@param	pUserData	popコールバック関数に渡されるユーザーデータを指定します。
	 */
	void connectPopCallBack(int (*pPopCallBack)(XD_UINT4, char*, XD_UINT4,void*), void* pUserData);
protected:

	XMsgQueueERR innerReading();

	class readerThread : public XThread
	{
	public:
		readerThread(XIPCQueuePuller* puller);
		~readerThread();
		void start();
		virtual void* RunUndetached(void* pArg);
		XIPCQueuePuller*	m_puller;
	};


	XMutex			m_lock;
	int				m_iHandle;
	int				m_iMsgQueueID;
	XD_UINT4		m_ulCurrentTimeOut;
	bool			m_bRunning;
	readerThread*	m_pReaderThread;
	XQueue			m_q;
friend class readerThread;
friend class XLIB_API XMsgQueueServer;
};


/** @brief キューへデータをセットするクラス(IPC:InterProcess Communication)　*/
class XLIB_API XIPCQueuePusher
{
public:
	/** @brief コンストラクタ*/
	XIPCQueuePusher();

	virtual ~XIPCQueuePusher();

	/**	@brief	キューをオープンします。
	 *
	 *	@param	pName	既に、オープンされているPullerの名前を指定します。
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR open(const char* pName);

	/**	@brief	キューをオープンします。
	 *
	 *	@param	pName	既に、オープンされているPullerの名前を指定します。
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR open(const wchar_t* pName);
	/**	@brief	キューを閉じます
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR close();
	/**	@brief	キューへデータをセットします。
	 *
	 *	@param	pData	セットするデータ
	 *	@param	nDataSize	データサイズ
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR push(XD_UINT4 ulMsgID, void *pData, size_t nDataSize);

protected:
	XMutex	m_lock;
	int		m_iHandle;
	int		m_iMsgQueueID;
};



/** @brief キューを使用して通信を行うクライアントクラス　*/
class XLIB_API XMsgQueueClient
{
public:
	/** @brief コンストラクタ*/
	XMsgQueueClient();
	virtual ~XMsgQueueClient();

	/**	@brief	キューサーバーのとの接続を確立します。
	 *
	 *	@param	pName	サーバーがオープンsした名前
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR open(const char* pName);
	/**	@brief	キューサーバーのとの接続を確立します。
	 *
	 *	@param	pName	サーバーがオープンsした名前
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR open(const wchar_t* pName);
	
	/**	@brief	キューサーバーのとの接続を閉じます。
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR close();

	/**	@brief	クライアントIDを取得します。
	 *	@return	クライアントID
	 */
	XD_UINT8 getID() const { return m_id; }
	
	/**	@brief	リクエストを送信して、レスポンスを受信します。
	 *
	 *	@param	pData	送信データ
	 *	@param	nDataSize	送信データのサイズ
	 *	@param	dataBuff	レスポンスを受け取るバッファ
	 *	@param	pDataSize	レスポンスのサイズ
	 *	@param	ulTimeOut	タイムアウト値。
	 *				- XQueue_TIMEOUT_NOWAITは、キューにデータがない場合、待たずに直ぐにリターンする。
	 *				- XQueue_TIMEOUT_FOREVERは、無限に待つ。
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR sendRequset(void *pData, size_t nDataSize, XBuff& dataBuff, size_t *pDataSize, XD_UINT4 ulTimeOut = XQueue_TIMEOUT_NOWAIT);

	/**	@brief	キューからメッセージを取り出しします。
	 *
	 *	@param	dataBuff	メッセージを受け取るバッファ
	 *	@param	pDataSize	受け取ったメッセージサイズ
	 *	@param	ulMsgID	取り出すメッセージID。
	 *				- XQueue_MsgID_ANYを指定すると、キューの先頭にあるメッセージを取り出す。
	 *	@param	ulTimeout	取り出すまでのタイムアウト値を指定。
	 *				- XQueue_TIMEOUT_NOWAITは、キューにデータがない場合、待たずに直ぐにリターンする。
	 *				- XQueue_TIMEOUT_FOREVERは、無限に待つ。
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR pop(XBuff& dataBuff, size_t *pDataSize, XD_UINT4 ulMsgID = XQueue_MsgID_ANY, XD_UINT4 ulTimeout = XQueue_TIMEOUT_FOREVER);

	/**	@brief	派生クラスで、popコールバックを実装します。デフォルトは、何もせず、キューにセットされます。
	 *
	 *	@param	ulMsgID	メッセージＩＤ
	 *	@param	pData	データ
	 *	@param	ulDataSize	データサイズ
	 *
	 *	@retval	XQueue_POP_CALLBCK_IGNORE	処理を無視した。呼び出し元は、pop関数をコールして、データの処理を行わなければ行けない
	 *	@retval	XQueue_POP_CALLBCK_PROCESS	データを処理した。
	 */
	virtual int popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);
	

	/*	@brief	クライアントID */
	union ClinentID {
		XD_UINT8			m_id8;
		XD_UINT4			m_id4[2];	// 0:プロセスID 1:このインスタンスのID
		XD_UINT2			m_id2[4];
		XD_UINT1			m_id1[8];
	};
private:
	static int _popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize, void* pUserData);
	int _popCallBack(XD_UINT4 ulMsgID, char* pData, XD_UINT4 ulDataSize);

protected:
	XIPCQueuePuller		m_puller;
	XIPCQueuePusher		m_pusher;
	XD_UINT8			m_id;

	XMutex				m_lockpop;		//  POP,ロックオブジェクト
	XMapAddr			m_popIDs;		//  送信メッセージＩＤ管理データ
	XMutex				m_lockSequence;	//  シーケンスナンバー,ロックオブジェクト
	XD_UINT4			m_ulSequenceNo;	//	シーケンスナンバー
};


/** @brief キューを使用して通信を行うサーバークラス　*/
class XLIB_API XMsgQueueServer : public XThread
{
protected:
	class ClinetData;	// サーバーに接続したクライアントの情報
public:
	XMsgQueueServer();
	virtual ~XMsgQueueServer();


	/**	@brief	サーバーをオープンします。
	 *
	 *	@param	pName	名前
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR open(const char* pName);
	/**	@brief	サーバーをオープンします。
	 *
	 *	@param	pName	名前
	 *
	 *	@return	実行ステータス
	 */
	XMsgQueueERR open(const wchar_t* pName);
	/**	@brief	サーバーを閉じます。
	 *
	 *	@return	実行ステータス	
	 */
	XMsgQueueERR close();

	/**	@brief	サーバーのメインループスレッドを開始します。
	 */
	void startMainLoopThread();

	/**	@brief	イベントを送信します。
	 *
	 *	@param	pData	送信データ
	 *	@param	dataSize	データのサイズ
	 *	@param	uClientID	クライアントID。
	 *
	 *	@return	実行ステータス		
	 */
	XMsgQueueERR sendEvent(void *pData, size_t dataSize, XD_UINT8 uClientID = (XD_UINT8)(-1));
	
// property
	int getClientsCount();
	XD_UINT8 getClientID(int index);


// must Overwrite
	/**	@brief	派生クラスで、クライアントからリクエストを受信した時の動作を記述します。
	 *
	 *	@param	pClient	クライアント情報
	 *	@param	pData	リクエストのデータ
	 *	@param	dataSize	リクエストデータサイズ
	 */
	virtual void receiveRequest(ClinetData* pClient, XD_UINT4 ulMsgID, void *pData, size_t dataSize) = 0;

	/**	@brief	クライアントの接続の状況変化を通知します。
	 *	@param	ulClientID	クライアントID
	 *	@param	bConnect	true:接続/false:切断
	 */
	virtual void ntfyConnection(XD_UINT8 ulClientID, bool bConnect){};
protected:
	virtual void* RunUndetached(void* pArg);

	/**	@brief	サーバーに接続したクライアントの情報*/
	class ClinetData {
	public:
		/**	@brief	クライアントIDを取得します。
		 *	@return	クライアントID
		 */
		XD_UINT8 getClinetID(){ return m_clientID; }
		/**	@brief	レスポンスを送信します。
		 *
		 *	@param	pData	レスポンスデータ
		 *	@param	dataSize	レスポンスデータのサイズ
		 *
		 *	@return	実行ステータス	
		 */
		XMsgQueueERR sendResponse(XD_UINT4 ulMsgID, const void *pData, size_t dataSize);
	private:
		ClinetData(XD_UINT8 clientID):m_clientID(clientID){}
		~ClinetData(){ m_pusher.close(); }
		XIPCQueuePusher		m_pusher;
		XD_UINT8			m_clientID;

	friend class XMsgQueueServer;
	};
protected:
	XIPCQueuePuller		m_puller;
	XStrBuff			m_strName;
	XList				m_clients;
	XMutex				m_lockClinets;
	bool				m_bRunning;
	XMutex				m_lockSequence;	//  シーケンスナンバー,ロックオブジェクト
	XD_UINT4			m_ulSequenceNo;	//	シーケンスナンバー
};


#include "xlib/xlib_tail.h"
#endif /*__X_MESSAGE_QUEUE_FILE__*/


