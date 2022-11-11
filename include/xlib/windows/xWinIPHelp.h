/**
 * @file xWinIPHelp.h
 * @brief サービスコントロール
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_WIN_IPHELP_HEASER_FILE__
#define __X_WIN_IPHELP_HEASER_FILE__

#include <vector>
#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"


typedef struct _xWinIPHelpNetRow
{
	DWORD				dwIndex;
	DWORD				dwPhysAddrLen;
	unsigned char		bPhysAddr[8];
	DWORD				dwAddr;
	DWORD				dwType;
} xWinIPHelpNetRow;

typedef struct _xWinIPHelpAddrRow
{
	DWORD		dwAddr;
	DWORD		dwIndex;
	DWORD		dwMask;
	DWORD		dwBCastAddr;
	DWORD		dwReasmSize;
	unsigned short	unused1;
	unsigned short	unused2;
} xWinIPHelpAddrRow;



typedef std::vector<xWinIPHelpNetRow>  xWinIPNetRows;
typedef std::vector<xWinIPHelpAddrRow> xWinIPAddrRows;

class xWinIPHelp
{
public:

public:
	xWinIPHelp();
	virtual ~xWinIPHelp();


	static unsigned long inetAddrW(wchar_t* pAddr);
	static unsigned long inetAddrA(char* pAddr);
	static XStrBuff inetNtot(unsigned long ulIP);
	static struct hostent* getHostByNameW(wchar_t* pName);
	static struct hostent* getHostByNameA(char* pName);
	static void strToMAC_W(wchar_t* pMAC, unsigned char* pMACAddress);
	static void strToMAC_A(char*    pMAC, unsigned char* pMACAddress);
	static int pingProccess(unsigned long ipaddress , int loop, int timeout, int ttl);
	static unsigned long wakeUpLan(unsigned long uBroadCast, unsigned char* pMac);
	
	bool getIpNetTable(xWinIPNetRows &raws, BOOL bOrder);
	bool getIpAddrTable(xWinIPAddrRows& rows, BOOL bOrder);
private:
	HMODULE	m_module;
	FARPROC	m_pGetIpNetTable;
	FARPROC	m_pGetIpAddrTable;
};

#endif /*__X_WIN_IPHELP_HEASER_FILE__*/


