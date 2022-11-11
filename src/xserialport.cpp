#ifdef WIN32
  #include <windows.h>
  #include <direct.h>
  #include <wchar.h>
  #include <io.h>
  #include <share.h>
#else
  #include <termios.h>
  #include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>
#include "xlib/xserialport.h"
#include "xlib/xeception.h"

XSerialPort::Opt::Opt()
{
XEXCEPT_ENTRY
	m_Priority			= PRIO_NOPARITY;
	m_ulBaudRate		= 38400;
	m_ucByte			= 8;
	m_stopBits			= STOPBITS_ONESTOPBIT;

	m_ulInQueueSize		= 4096;
	m_ulOutQueueSize	= 4096;


	m_bOutxCtsFlow		= false;
	m_RtsControl		= RTS_DISABLE;
	m_bOutxDsrFlow		= false;
	m_DtrControl		= DTR_DISABLE;
	m_bDsrSensitivity	= false;

	m_bOutX = false;
	m_bInX = false;
	m_bTXContinueOnXoff = true;
	m_usXonLim = 2048;
	m_usXoffLim = 512;
	m_XonChar = 0x11;
	m_XoffChar = 0x13;
	m_ErrorChar = 0; 
	m_EofChar = 0; 
	m_EvtChar = 0; 

	m_ulReadIntervalTimeout = 0;
	m_ulReadTotalTimeoutMultiplier = 0;
	m_ulReadTotalTimeoutConstant = 0;
	m_ulWriteTotalTimeoutMultiplier = 0;
	m_ulWriteTotalTimeoutConstant = 0;

XEXCEPT_EXIT
}
XSerialPort::Opt::~Opt()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSerialPort::Opt& XSerialPort::Opt::ByteSize(unsigned char val)
{
XEXCEPT_ENTRY
	if( !(4 <= val && val <= 8) ) {
		XEXCEPT_THROW_STR(XExcept_FILE, (const TCHAR*)XStrBuff::FormatS( _T("XSerialPort::Opt::ByteSize invalid value. following [4-8].")), 0, 0);
	}
	m_ucByte		= val;
	return *this;
XEXCEPT_EXIT	
}



//
//	XSerialPort
//
XSerialPort::XSerialPort()
	:	m_handle(-1)
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

XSerialPort::~XSerialPort()
{
XEXCEPT_ENTRY
	Close();
XEXCEPT_EXIT
}
bool XSerialPort::isValid()
{
XEXCEPT_ENTRY
	return m_handle != -1 ? true : false;
XEXCEPT_EXIT
}

bool XSerialPort::open(const char* pPort, const XSerialPort::Opt& opt)
{
XEXCEPT_ENTRY
#ifdef WIN32
	HANDLE	h = ::CreateFileA(pPort, 
			GENERIC_READ | GENERIC_WRITE,
			0, 
			NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
	if(h == INVALID_HANDLE_VALUE) {
		XTRACE(_T("CreateFile(%s) Error %d\n"), pPort, GetLastError() );
		return false;
	}
	m_handle = (int)h;

	// イベントマスクの設定
	if(!::SetCommMask(h, 0)) {
		Close();
		return false;
	}
	if(!::SetupComm(h, 4096, 4096)) {
		Close();
		return false;
	}

#else
	m_handle = ::open(pPort, O_RDWR);
	if( m_handle < 0 ) {
		m_handle = -1;
		return false;
	}
#endif
XEXCEPT_EXIT
	return true;
}

XSerialPort::Opt XSerialPort::getOpt()
{
	Opt opt;
XEXCEPT_ENTRY
	#ifdef WIN32
		if( m_handle == -1 ) {
			return opt;
		}
		COMMTIMEOUTS timeOut;
		DCB dcb;
		if( !::GetCommTimeouts((HANDLE)m_handle, &timeOut) ) {
			return opt;	
		}
		if(!::GetCommState((HANDLE)m_handle, &dcb)) {
			return opt;
		}


		opt.m_ulReadIntervalTimeout = timeOut.ReadIntervalTimeout;
		opt.m_ulReadTotalTimeoutMultiplier = timeOut.ReadTotalTimeoutMultiplier;
		opt.m_ulReadTotalTimeoutConstant = timeOut.ReadTotalTimeoutConstant;
		opt.m_ulWriteTotalTimeoutMultiplier = timeOut.WriteTotalTimeoutMultiplier;
		opt.m_ulWriteTotalTimeoutConstant = timeOut.WriteTotalTimeoutConstant;


		opt.m_ulBaudRate = dcb.BaudRate;
		opt.m_ucByte = dcb.ByteSize;

		switch( dcb.Parity ) {
		case NOPARITY:			opt.m_Priority = PRIO_NOPARITY;		break;
		case EVENPARITY:		opt.m_Priority = PRIO_EVENPARITY;	break;
		case ODDPARITY:			opt.m_Priority = PRIO_ODDPARITY;	break;
		case MARKPARITY:		opt.m_Priority = PRIO_MARKPARITY;	break;
		case SPACEPARITY:		opt.m_Priority = PRIO_SPACEPARITY;	break;
		}
		switch( dcb.StopBits ) {
		case ONESTOPBIT:		opt.m_stopBits  = STOPBITS_ONESTOPBIT;	break;
		case ONE5STOPBITS:		opt.m_stopBits  = STOPBITS_ONE5STOPBITS;	break;
		case TWOSTOPBITS:		opt.m_stopBits  = STOPBITS_TWOSTOPBITS;	break;
		}
		// フロー制御
		opt.m_bOutxCtsFlow = dcb.fOutxCtsFlow;
		switch( dcb.fRtsControl ) {
		case RTS_CONTROL_DISABLE:		opt.m_RtsControl  = RTS_DISABLE;	break;
		case RTS_CONTROL_ENABLE:		opt.m_RtsControl  = RTS_ENABLE;		break;
		case RTS_CONTROL_HANDSHAKE:		opt.m_RtsControl  = RTS_HANDSHAKE;	break;
		case RTS_CONTROL_TOGGLE:		opt.m_RtsControl  = RTS_TOGGLE;		break;
		}
		opt.m_bOutxDsrFlow = dcb.fOutxDsrFlow;
		switch( dcb.fDtrControl ) {
		case DTR_CONTROL_DISABLE:		opt.m_DtrControl  = DTR_DISABLE;	break;
		case DTR_CONTROL_ENABLE:		opt.m_DtrControl  = DTR_ENABLE;		break;
		case DTR_CONTROL_HANDSHAKE:		opt.m_DtrControl  = DTR_HANDSHAKE;	break;
		}
		opt.m_bDsrSensitivity = dcb.fDsrSensitivity;

		/// XON / XOFF
		opt.m_bOutX = dcb.fOutX;
		opt.m_bInX = dcb.fInX;
		opt.m_bTXContinueOnXoff = dcb.fTXContinueOnXoff;
		opt.m_usXonLim = dcb.XonLim;
		opt.m_usXoffLim = dcb.XoffLim;
		opt.m_XonChar = dcb.XonChar; 
		opt.m_XoffChar = dcb.XoffChar; 
		opt.m_ErrorChar = dcb.ErrorChar; 
		opt.m_EofChar = dcb.EofChar; 
		opt.m_EvtChar = dcb.EvtChar; 




	#else

	#endif
XEXCEPT_EXIT
	return opt;
}
bool XSerialPort::setOpt(XSerialPort::Opt& opt)
{
XEXCEPT_ENTRY
	#ifdef WIN32
		if( m_handle == -1 ) {
			return false;
		}
		DCB dcb;
		if(!::GetCommState((HANDLE)m_handle, &dcb)) {
			return false;
		}
		dcb.DCBlength = sizeof(DCB);
		dcb.BaudRate = opt.m_ulBaudRate;
		dcb.ByteSize = opt.m_ucByte;
		switch( opt.m_Priority ) {
		case PRIO_NOPARITY:			dcb.Parity = NOPARITY;		break;
		case PRIO_EVENPARITY:		dcb.Parity = EVENPARITY;	break;
		case PRIO_ODDPARITY:		dcb.Parity = ODDPARITY;		break;
		case PRIO_MARKPARITY:		dcb.Parity = MARKPARITY;	break;
		case PRIO_SPACEPARITY:		dcb.Parity = SPACEPARITY;	break;
		}
		switch( opt.m_stopBits ) {
		case STOPBITS_ONESTOPBIT:		dcb.StopBits = ONESTOPBIT;		break;
		case STOPBITS_ONE5STOPBITS:		dcb.StopBits = ONE5STOPBITS;	break;
		case STOPBITS_TWOSTOPBITS:		dcb.StopBits = TWOSTOPBITS;		break;
		}
		dcb.fParity = (opt.m_Priority == PRIO_NOPARITY) ? FALSE : TRUE;

		// フロー制御
		dcb.fOutxCtsFlow = opt.m_bOutxCtsFlow;
		switch( opt.m_RtsControl ) {
		case RTS_DISABLE:	dcb.fRtsControl = RTS_CONTROL_DISABLE;	break;
		case RTS_ENABLE:	dcb.fRtsControl = RTS_CONTROL_ENABLE;	break;
		case RTS_HANDSHAKE:	dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;	break;
		case RTS_TOGGLE:	dcb.fRtsControl = RTS_CONTROL_TOGGLE;	break;
		}
		dcb.fOutxDsrFlow = opt.m_bOutxDsrFlow;
		switch( opt.m_DtrControl ) {
		case DTR_DISABLE:	dcb.fDtrControl = DTR_CONTROL_DISABLE;	break;
		case DTR_ENABLE:	dcb.fDtrControl = DTR_CONTROL_ENABLE;	break;
		case DTR_HANDSHAKE:	dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;	break;
		}
		dcb.fDsrSensitivity = opt.m_bDsrSensitivity;

		/// XON / XOFF
		dcb.fOutX = opt.m_bOutX;
		dcb.fInX = opt.m_bInX;
		dcb.fTXContinueOnXoff = opt.m_bTXContinueOnXoff;
		dcb.XonLim = opt.m_usXonLim;
		dcb.XoffLim = opt.m_usXoffLim;
		dcb.XonChar = opt.m_XonChar; 
		dcb.XoffChar = opt.m_XoffChar; 
		dcb.ErrorChar = opt.m_ErrorChar; 
		dcb.EofChar = opt.m_EofChar; 
		dcb.EvtChar = opt.m_EvtChar; 


		// call API
		if(!::SetCommState((HANDLE)m_handle, &dcb)) {
			return false;
		}



		COMMTIMEOUTS timeOut;
		timeOut.ReadIntervalTimeout         = opt.m_ulReadIntervalTimeout;
		timeOut.ReadTotalTimeoutMultiplier  = opt.m_ulReadTotalTimeoutMultiplier;
		timeOut.ReadTotalTimeoutConstant    = opt.m_ulReadTotalTimeoutConstant;
		timeOut.WriteTotalTimeoutMultiplier = opt.m_ulWriteTotalTimeoutMultiplier;
		timeOut.WriteTotalTimeoutConstant   = opt.m_ulWriteTotalTimeoutConstant;
		if( !::SetCommTimeouts((HANDLE)m_handle, &timeOut) ) {
			return false;
		}
	#else

	#endif
XEXCEPT_EXIT
	return true;
}


bool XSerialPort::Close()
{
XEXCEPT_ENTRY
	if( m_handle != -1 ) {
#ifdef WIN32
		::CloseHandle( (HANDLE)m_handle );
#else
		::close(m_handle);
#endif
		m_handle = -1;
	}
XEXCEPT_EXIT
	return true;
}
size_t XSerialPort::WriteRaw(const void* lpData, size_t nSize, int flg/* = 0*/)
{
XEXCEPT_ENTRY
	if( m_handle == -1 ) {
		return (size_t)-1;
	}
	size_t iRet = 0;
#ifdef WIN32
	DWORD dwWritten;
	if( ::WriteFile((HANDLE)m_handle, lpData, nSize, &dwWritten, NULL) == FALSE ) {
		return -1;
	}
	iRet = dwWritten;
#else
	iRet = ::write(m_handle, lpData, nSize);
#endif
	return iRet;
XEXCEPT_EXIT
}


size_t XSerialPort::ReadRaw(void* lpData, size_t nSize, int flg/* = 0*/)
{
XEXCEPT_ENTRY
	if( m_handle == -1 ) {
		return (size_t)-1;
	}
	size_t iRet = 0;
#ifdef WIN32
	DWORD dwRead;
//	DWORD dwError;

//	COMSTAT ComStat;
//	::ClearCommError((HANDLE)m_handle, &dwError, &ComStat);
/*
	switch( dwError ) {
	case CE_BREAK:		printf("CE_BREAK\n");	break;
	case CE_FRAME:		printf("CE_BREAK\n");	break;
	case CE_OVERRUN:	printf("CE_OVERRUN\n");	break;
	case CE_IOE:		printf("CE_IOE\n");	break;
	case CE_RXOVER:		printf("CE_RXOVER\n");	break;
	case CE_RXPARITY:	printf("CE_RXPARITY\n");	break;
	case CE_TXFULL:		printf("CE_TXFULL\n");	break;
	}
*/
	if( ::ReadFile((HANDLE)m_handle, lpData, nSize, &dwRead, NULL) == FALSE ) {
		return -1;
	}
	iRet = dwRead;
#else
	iRet = ::read(m_handle, lpData, nSize);
#endif
	return iRet;
XEXCEPT_EXIT
}
