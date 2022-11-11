/**
 * @file xserialport.h
 * @brief �V���A���|�[�g���b�v�N���X
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

/**	@brief	�V���A���|�[�g���b�v�N���X	*/
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

	


	/** @brief	�R���X�g���N�^ */
	XSerialPort();
	/** @brief	�f�X�g���N�^ */
	virtual ~XSerialPort();

	/**	@brief	�I�v�V����	*/
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
		 �n���h�V�F�C�N�ƃt���[����
			RTS/CTS �n���h�V�F�C�N
		    RTS �n���h�V�F�C�N���L���ɂȂ��Ă���ꍇ�A �ʐM����ɑ΂��� RTS �� ON ( �A�N�e�B�u ) �ɂ��邱�ƂŁA
			��M�\�ł��邱�Ƃ������܂��B
		    ��M�o�b�t�@�������ς��ɂȂ�� RTS �� OFF �ɂ��āA �ʐM����ɑ΂��ăf�[�^�̑��M���~����悤�Ɏw
			�����܂��B
			����ɑ΂��A �ʐM����́ACTS �� ON ( �A�N�e�B�u ) �ɂ��Ď�M�\�ł��邱�Ƃ������܂��B �܂��ACTS ��
			OFF �ɂ��Ď�M�s�\�ł��邱�Ƃ������܂��B
			���̂悤�ɁARTS/CTS �n���h�V�F�C�N���g�p����ꍇ�ADCB �\���̂̃����o�����̂悤�ɕύX����K�v�������
			���B 
		*/
		/**	@brief	���M���ɁACTS ���Ď����邩�ǂ�����ݒ肵�܂��B 
		 *	@param	val	true�ɐݒ肷��ƁACTS��ON�ɂȂ�܂ő��M����~��ԂƂȂ�܂��B
		 *	@return	���̃C���X�^���X
		 */
		Opt& OutxCtsFlow(bool val){m_bOutxCtsFlow = val;return *this;}
		/**	@brief	RTS_HANDSHAKE (�n���h�V�F�C�N)
		 *	RTS_TOGGLE (�g�O��)
		 *	RTS_ENABLE (RTS�͏��ON)
		 *	RTS_DISABLE (RTS�͏��OFF)
		 *	
		 *	�n���h�V�F�C�N�̏ꍇ�A ��M�o�b�t�@��3/4�ȏ㖄�܂�� RTS ��OFF�ɂȂ�A 1/2�ȉ��ɂȂ��ON�ɂȂ�܂��B
		 *	
		 *	�g�O���̏ꍇ�A ��M�o�b�t�@�Ƀf�[�^���Ȃ���� RTS ��OFF�ɂȂ�A �f�[�^�������ON�ɂȂ�܂��B 
		 *
		 *	@param	val	�ݒ�l
		 *	@return	���̃C���X�^���X
		 */		
		Opt& RtsControl(RTS val){m_RtsControl = val;return *this;}

		/**	@brief	���M���ɁADSR ���Ď����邩�ǂ�����ݒ肵�܂��B 
		 *	@param	val	true�ɐݒ肷��ƁADSR��ON�ɂȂ�܂ő��M����~��ԂƂȂ�܂��BCTS�Ɠ����悤�Ȏg���������܂��B
		 *	@return	���̃C���X�^���X
		 */
		Opt& OutxDsrFlow(bool val){m_bOutxDsrFlow = val;return *this;}
		/**	@brief	 DTR_HANDSHAKE (�n���h�V�F�C�N)
		 *	DTR_ENEBLE (DTR�͏��ON)
		 *	DTR_DISABLE (DTR�͏��OFF)
		 *	
		 *	RTS/CTS �n���h�V�F�C�N�Ɠ��l�ɁADTR/DSR �n���h�V�F�C�N�Ɏg�p���܂��B 
		 *
		 *	@param	val	�ݒ�l
		 *	@return	���̃C���X�^���X
		 */
		Opt& DtrControl(DTR val){m_DtrControl = val;return *this;}
		/**	@brief	 true�ɐݒ肵���ꍇ�ADSR��OFF�̊Ԃ͎�M�f�[�^�𖳎����܂��B 
		 *	@param	val	�ݒ�l
		 *	@return	���̃C���X�^���X
		 */
		Opt& DsrSensitivity(bool val){m_bDsrSensitivity = val;return *this;}

		/*
			XON / XOFF
				XON/XOFF �͒ʐM����ɑ΂��āA
				���M�����E�֎~���邽�߂̐�����@�ł��B
		*/
		/**	@brief	XoffChar����M����Ƒ��M���~���AXonChar����M����Ƒ��M���ĊJ���܂��B
		 *
		 *	@param	val	true�̏ꍇ�ɗL���ł��B
		 *	@return	���̃C���X�^���X
		 */
		Opt& OutX(bool val){m_bOutX = val;return *this;}

		/**	@brief	��M�o�b�t�@�̋󂫗e�ʂ� XoffLim�o�C�g�����ɂȂ��XoffChar�����M����A��M�o�b�t�@�̃f�[�^��XonLim�o�C�g�ȉ��ɂȂ��XonChar�����M����܂��B
		 *
		 *	@param	val	true�̏ꍇ�ɗL���ł��B
		 *	@return	���̃C���X�^���X
		 */
		Opt& InX(bool val){m_bInX = val;return *this;}
		/**	@brief	XOffChar �𑗐M��������f�[�^�̑��M���p�����܂��B
		 *
		 *	@param	val	true�̏ꍇ�ɗL���ł��B
		 *	@return	���̃C���X�^���X
		 */
		Opt& TXContinueOnXoff(bool val){m_bTXContinueOnXoff = val;return *this;}

		/**	@brief	��M�o�b�t�@�̃f�[�^�� XonLim�o�C�g�ȉ��ɂȂ��XonChar�����M����A��M�\�ł��邱�Ƃ������܂��B
		 *
		 *	@param	val	true�̏ꍇ�ɗL���ł��B
		 *	@return	���̃C���X�^���X
		 */
		Opt& XonLim(unsigned short val){m_usXonLim = val;return *this;}

		/**	@brief	��M�o�b�t�@�̋󂫗e�ʂ�XoffLim�o�C�g�����ɂȂ��XoffChar�����M����A��M�s�\�ł��邱�Ƃ������܂��B
		 *
		 *	@param	val	true�̏ꍇ�ɗL���ł��B
		 *	@return	���̃C���X�^���X
		 */
		Opt& XoffLim(unsigned short val){m_usXoffLim = val;return *this;}


		/**	@brief	XON�L�����N�^�B�ʏ�� 0x11�B
		 *
		 *	@param	val	�ݒ�l
		 *	@return	���̃C���X�^���X
		 */
		Opt& XonChar(char val){m_XonChar = val;return *this;}

		/**	@brief	XOFF�L�����N�^�B�ʏ�� 0x13�B
		 *
		 *	@param	val	�ݒ�l
		 *	@return	���̃C���X�^���X
		 */
		Opt& XoffChar(char val){m_XoffChar = val;return *this;}


		// Timeout
		Opt& ReadIntervalTimeout(unsigned long val){m_ulReadIntervalTimeout = val; return *this;}
		Opt& ReadTotalTimeoutMultiplier(unsigned long val){m_ulReadTotalTimeoutMultiplier = val; return *this;}
		Opt& ReadTotalTimeoutConstant(unsigned long val){m_ulReadTotalTimeoutConstant = val; return *this;}
		Opt& WriteTotalTimeoutMultiplier(unsigned long val){m_ulWriteTotalTimeoutMultiplier = val; return *this;}
		Opt& WriteTotalTimeoutConstant(unsigned long val){m_ulWriteTotalTimeoutConstant = val; return *this;}
	public:
		PRIO				m_Priority;		//	�v���C�I���e�B�[
		unsigned long		m_ulBaudRate;	//	�{�[���[�g
		unsigned char		m_ucByte;		//	�f�[�^�r�b�g�T�C�Y(4-8)
		STOPBITS			m_stopBits;		//  �X�g�b�v�r�b�g
		unsigned long		m_ulInQueueSize;
		unsigned long		m_ulOutQueueSize;
		// �t���[����
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
