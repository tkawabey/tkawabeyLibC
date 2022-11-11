/**
 * @file xserialport.h
 * @brief シリアルポートラップクラス
 *
 * @author T.Fujiha
 * @date 2007/06/29
 * @version 1.0
 *
 * Copyright (C) 2007 T.Fujiha,
 */
#ifndef __XLIB_SERIAL_PORT_H__
#define __XLIB_SERIAL_PORT_H__

#include "xlib/xlib_head.h"
#include "xlib/xio.h"
#include "xlib/xeception.h"

/**	@brief	シリアルポートラップクラス	*/
class XLIB_API XSerialPort : public  XIO
{
public:
	enum PRIO {
			PRIO_NOPARITY
		,	PRIO_EVENPARITY
		,	PRIO_ODDPARITY
		,	PRIO_MARKPARITY
		,	PRIO_SPACEPARITY
		,
	};
	// 
	enum STOPBITS {
			STOPBITS_ONESTOPBIT
		,	STOPBITS_ONE5STOPBITS
		,	STOPBITS_TWOSTOPBITS
		,
	};
	// 
	enum RTS {
			RTS_DISABLE
		,	RTS_ENABLE
		,	RTS_HANDSHAKE
		,	RTS_TOGGLE
	};
	// 
	enum DTR {
			DTR_DISABLE
		,	DTR_ENABLE
		,	DTR_HANDSHAKE
	};

	


	/** @brief	コンストラクタ */
	XSerialPort();
	/** @brief	デストラクタ */
	virtual ~XSerialPort();

	/**	@brief	オプション	*/
	class Opt {
	public:
		Opt();
		~Opt();

		Opt& operator=(const Opt& c)
		{
		XEXCEPT_ENTRY
			m_Priority			= c.m_Priority;
			m_ulBaudRate		= c.m_ulBaudRate;
			m_ucByte			= c.m_ucByte;
			m_stopBits			= c.m_stopBits;

			m_ulInQueueSize		= c.m_ulInQueueSize;
			m_ulOutQueueSize	= c.m_ulOutQueueSize;


			m_bOutxCtsFlow		= c.m_bOutxCtsFlow;
			m_RtsControl		= c.m_RtsControl;
			m_bOutxDsrFlow		= c.m_bOutxDsrFlow;
			m_DtrControl		= c.m_DtrControl;
			m_bDsrSensitivity	= c.m_bDsrSensitivity;

			m_bOutX				= c.m_bOutX;
			m_bInX				= c.m_bInX;
			m_bTXContinueOnXoff = c.m_bTXContinueOnXoff;
			m_usXonLim			= c.m_usXonLim;
			m_usXoffLim			= c.m_usXoffLim;
			m_XonChar			= c.m_XonChar;
			m_XoffChar			= c.m_XoffChar;
			m_ErrorChar			= c.m_ErrorChar; 
			m_EofChar			= c.m_EofChar; 
			m_EvtChar			= c.m_EvtChar; 
		XEXCEPT_EXIT	
			return *this;
		}

		Opt& Priority(PRIO val){ m_Priority = val; return *this; }
		Opt& BaudRate(unsigned long val){ m_ulBaudRate = val; return *this; }
		Opt& ByteSize(unsigned char val);
		Opt& StopBits(STOPBITS val){ m_stopBits = val; return *this; }
		Opt& InQueueSize(unsigned long val){ m_ulInQueueSize = val; return *this;}
		Opt& OutQueueSize(unsigned long val){ m_ulOutQueueSize = val; return *this;}
		/*
		 ハンドシェイクとフロー制御
			RTS/CTS ハンドシェイク
		    RTS ハンドシェイクが有効になっている場合、 通信相手に対して RTS を ON ( アクティブ ) にすることで、
			受信可能であることを示します。
		    受信バッファがいっぱいになると RTS を OFF にして、 通信相手に対してデータの送信を停止するように指
			示します。
			これに対し、 通信相手は、CTS を ON ( アクティブ ) にして受信可能であることを示します。 また、CTS を
			OFF にして受信不可能であることを示します。
			このように、RTS/CTS ハンドシェイクを使用する場合、DCB 構造体のメンバを次のように変更する必要がありま
			す。 
		*/
		/**	@brief	送信時に、CTS を監視するかどうかを設定します。 
		 *	@param	val	trueに設定すると、CTSがONになるまで送信が停止状態となります。
		 *	@return	このインスタンス
		 */
		Opt& OutxCtsFlow(bool val){m_bOutxCtsFlow = val;return *this;}
		/**	@brief	RTS_HANDSHAKE (ハンドシェイク)
		 *	RTS_TOGGLE (トグル)
		 *	RTS_ENABLE (RTSは常にON)
		 *	RTS_DISABLE (RTSは常にOFF)
		 *	
		 *	ハンドシェイクの場合、 受信バッファが3/4以上埋まると RTS がOFFになり、 1/2以下になるとONになります。
		 *	
		 *	トグルの場合、 受信バッファにデータがなければ RTS がOFFになり、 データがあればONになります。 
		 *
		 *	@param	val	設定値
		 *	@return	このインスタンス
		 */		
		Opt& RtsControl(RTS val){m_RtsControl = val;return *this;}

		/**	@brief	送信時に、DSR を監視するかどうかを設定します。 
		 *	@param	val	trueに設定すると、DSRがONになるまで送信が停止状態となります。CTSと同じような使い方をします。
		 *	@return	このインスタンス
		 */
		Opt& OutxDsrFlow(bool val){m_bOutxDsrFlow = val;return *this;}
		/**	@brief	 DTR_HANDSHAKE (ハンドシェイク)
		 *	DTR_ENEBLE (DTRは常にON)
		 *	DTR_DISABLE (DTRは常にOFF)
		 *	
		 *	RTS/CTS ハンドシェイクと同様に、DTR/DSR ハンドシェイクに使用します。 
		 *
		 *	@param	val	設定値
		 *	@return	このインスタンス
		 */
		Opt& DtrControl(DTR val){m_DtrControl = val;return *this;}
		/**	@brief	 trueに設定した場合、DSRがOFFの間は受信データを無視します。 
		 *	@param	val	設定値
		 *	@return	このインスタンス
		 */
		Opt& DsrSensitivity(bool val){m_bDsrSensitivity = val;return *this;}

		/*
			XON / XOFF
				XON/XOFF は通信相手に対して、
				送信を許可・禁止するための制御方法です。
		*/
		/**	@brief	XoffCharを受信すると送信を停止し、XonCharを受信すると送信を再開します。
		 *
		 *	@param	val	trueの場合に有効です。
		 *	@return	このインスタンス
		 */
		Opt& OutX(bool val){m_bOutX = val;return *this;}

		/**	@brief	受信バッファの空き容量が XoffLimバイト未満になるとXoffCharが送信され、受信バッファのデータがXonLimバイト以下になるとXonCharが送信されます。
		 *
		 *	@param	val	trueの場合に有効です。
		 *	@return	このインスタンス
		 */
		Opt& InX(bool val){m_bInX = val;return *this;}
		/**	@brief	XOffChar を送信した後もデータの送信を継続します。
		 *
		 *	@param	val	trueの場合に有効です。
		 *	@return	このインスタンス
		 */
		Opt& TXContinueOnXoff(bool val){m_bTXContinueOnXoff = val;return *this;}

		/**	@brief	受信バッファのデータが XonLimバイト以下になるとXonCharが送信され、受信可能であることを示します。
		 *
		 *	@param	val	trueの場合に有効です。
		 *	@return	このインスタンス
		 */
		Opt& XonLim(unsigned short val){m_usXonLim = val;return *this;}

		/**	@brief	受信バッファの空き容量がXoffLimバイト未満になるとXoffCharが送信され、受信不可能であることを示します。
		 *
		 *	@param	val	trueの場合に有効です。
		 *	@return	このインスタンス
		 */
		Opt& XoffLim(unsigned short val){m_usXoffLim = val;return *this;}


		/**	@brief	XONキャラクタ。通常は 0x11。
		 *
		 *	@param	val	設定値
		 *	@return	このインスタンス
		 */
		Opt& XonChar(char val){m_XonChar = val;return *this;}

		/**	@brief	XOFFキャラクタ。通常は 0x13。
		 *
		 *	@param	val	設定値
		 *	@return	このインスタンス
		 */
		Opt& XoffChar(char val){m_XoffChar = val;return *this;}


		// Timeout
		Opt& ReadIntervalTimeout(unsigned long val){m_ulReadIntervalTimeout = val; return *this;}
		Opt& ReadTotalTimeoutMultiplier(unsigned long val){m_ulReadTotalTimeoutMultiplier = val; return *this;}
		Opt& ReadTotalTimeoutConstant(unsigned long val){m_ulReadTotalTimeoutConstant = val; return *this;}
		Opt& WriteTotalTimeoutMultiplier(unsigned long val){m_ulWriteTotalTimeoutMultiplier = val; return *this;}
		Opt& WriteTotalTimeoutConstant(unsigned long val){m_ulWriteTotalTimeoutConstant = val; return *this;}
	public:
		PRIO				m_Priority;		//	プライオリティー
		unsigned long		m_ulBaudRate;	//	ボーレート
		unsigned char		m_ucByte;		//	データビットサイズ(4-8)
		STOPBITS			m_stopBits;		//  ストップビット
		unsigned long		m_ulInQueueSize;
		unsigned long		m_ulOutQueueSize;
		// フロー制御
		bool				m_bOutxCtsFlow;	// 
		RTS					m_RtsControl;
		bool				m_bOutxDsrFlow;	// 
		DTR					m_DtrControl;
		bool				m_bDsrSensitivity;

		// XON / XOFF
		bool				m_bOutX;
		bool				m_bInX;
		bool				m_bTXContinueOnXoff;
		unsigned short		m_usXonLim;
		unsigned short		m_usXoffLim;
		char				m_XonChar; 
		char				m_XoffChar; 
		char				m_ErrorChar; 
		char				m_EofChar; 
		char				m_EvtChar; 

		// Timeout
		unsigned long m_ulReadIntervalTimeout;          /* Maximum time between read chars. */
		unsigned long m_ulReadTotalTimeoutMultiplier;   /* Multiplier of characters.        */
		unsigned long m_ulReadTotalTimeoutConstant;     /* Constant in milliseconds.        */
		unsigned long m_ulWriteTotalTimeoutMultiplier;  /* Multiplier of characters.        */
		unsigned long m_ulWriteTotalTimeoutConstant;    /* Constant in milliseconds.        */

	};


	bool open(const char* pPort, const XSerialPort::Opt& opt);

	Opt getOpt();
	bool setOpt(Opt& opt);
	virtual bool isValid();
	
// XIO
	/** @copydoc XIO::Close()	 */
	virtual bool Close();
	/** @copydoc XIO::WriteRaw(const void* , size_t , int )	 */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0);
	/** @copydoc XIO::ReadRaw(const void* , size_t , int )	 */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0);
protected:
	int	m_handle;	//
};

#include "xlib/xlib_tail.h"
#endif /*__XLIB_SERIAL_PORT_H__*/
