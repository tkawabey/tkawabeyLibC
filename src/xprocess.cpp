#include "xlib/xprocess.h"
#ifdef WIN32
  #include <process.h>
#else
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <string.h>
  #include <signal.h>
  #include <termios.h>
  #include <unistd.h>


#endif //
#include "xlib/xeception.h"


///////////////////////////////////////////////////////////////////////////
//
//	XPipe
//
XPipe::XPipe()
{
	m_hPipe[XIO_READ] = m_hPipe[XIO_WRITE] = 0;
	m_hPopeChiledSideRd[XIO_READ] = m_hPopeChiledSideRd[XIO_WRITE] = 0;
	m_hPopeChiledSideWr[XIO_READ] = m_hPopeChiledSideWr[XIO_WRITE] = 0;
}
XPipe::~XPipe()
{

}
bool XPipe::Close()
{
	CloseRead();
	CloseWrite();
	return true;
}
void XPipe::CloseRead()
{
#ifdef WIN32
	if(m_hPipe[XIO_READ] != 0) {
		::CloseHandle(m_hPipe[XIO_READ]);
	}
	m_hPipe[XIO_READ] = 0;
#else
	if(m_hPipe[XIO_READ] != 0) {
		::close(m_hPipe[XIO_READ]);
	}
	m_hPipe[XIO_READ] = 0;
#endif
}
void XPipe::CloseWrite()
{
#ifdef WIN32
	if(m_hPipe[XIO_WRITE] != 0) {
		::CloseHandle(m_hPipe[XIO_WRITE]);
	}
	m_hPipe[XIO_WRITE] = 0;
#else
	if(m_hPipe[XIO_WRITE] != 0) {
		::close(m_hPipe[XIO_WRITE]);
	}
	m_hPipe[XIO_WRITE] = 0;
#endif
}
bool XPipe::CreatePipe()
{
	this->Close();
#ifdef WIN32
	SECURITY_ATTRIBUTES  sa = {sizeof(PSECURITY_ATTRIBUTES), NULL, TRUE};
	HANDLE hParent = ::GetCurrentProcess();


	/*
	  相手のWをクローズしているので、自分はWで書き込み
	  m_hPopeChiledSideWr[XIO_WRITE]
	*/
	if( ::CreatePipe(&m_hPopeChiledSideWr[XIO_READ], &m_hPopeChiledSideWr[XIO_WRITE], &sa, 0) == FALSE ) {
		return false;
	}
	if( ::DuplicateHandle(hParent, m_hPopeChiledSideWr[XIO_READ], hParent, &m_hPipe[XIO_READ], 0, FALSE, DUPLICATE_SAME_ACCESS) == FALSE ) {
		return false;
	}
	if( ::CloseHandle( m_hPopeChiledSideWr[XIO_READ] ) == FALSE ) { return false; }
	m_hPopeChiledSideWr[XIO_READ] = 0;

	/*
	  相手のRをクローズしているので、自分はWで読み込み
	  m_hPopeChiledSideRd[XIO_READ]
	*/
	if( ::CreatePipe(&m_hPopeChiledSideRd[XIO_READ], &m_hPopeChiledSideRd[XIO_WRITE], &sa, 0) == FALSE ) {
		return false;
	}
	if( ::DuplicateHandle(hParent, m_hPopeChiledSideRd[XIO_READ], hParent, &m_hPipe[XIO_WRITE], 0, FALSE, DUPLICATE_SAME_ACCESS) == FALSE ) {
		return false;
	}
	if( ::CloseHandle( m_hPopeChiledSideRd[XIO_READ] ) == FALSE ) { return false; }
	m_hPopeChiledSideRd[XIO_READ] = 0;	
	return true;
#else
/*
	if( ::pipe(m_hPopeChiledSideWr) != 0 ) {
		return false;
	}
	m_hPipe[XIO_READ] = ::dup(m_hPopeChiledSideWr[XIO_READ]);
	::close( m_hPopeChiledSideWr[XIO_READ] );
	m_hPopeChiledSideWr[XIO_READ] = 0;


	if( ::pipe(m_hPopeChiledSideRd) != 0 ) {
		return false;
	}
	m_hPipe[XIO_WRITE] = ::dup(m_hPopeChiledSideRd[XIO_WRITE]);
	::close( m_hPopeChiledSideWr[XIO_WRITE] );
	m_hPopeChiledSideWr[XIO_WRITE] = 0;
*/
	return true;
#endif
}
size_t XPipe::WriteRaw(const void* lpData, size_t nSize, int flg) {
#ifdef WIN32
	DWORD dwReadSize = nSize;
	DWORD dwReadedSize = 0;

	if( m_hPipe[XIO_WRITE] == 0 ) { 	return (size_t)-1;}
	if( ::WriteFile(m_hPipe[XIO_WRITE], lpData, dwReadSize, &dwReadedSize, NULL) == FALSE ) {
		return (size_t)-1;
	}
	return (size_t)dwReadedSize;
#else
	if( m_hPipe[XIO_WRITE] == 0 ) { 	return (size_t)-1;}
	return ::write(m_hPipe[XIO_WRITE],  lpData, nSize);
#endif
}
size_t XPipe::ReadRaw(void* lpData, size_t nSize, int flg)
{
#ifdef WIN32
	DWORD dwWriteSize = nSize;
	DWORD dwWritedSize = 0;

	if( m_hPipe[XIO_READ] == 0 ) { return (size_t)-1;}
	if( ::ReadFile(m_hPipe[XIO_READ], lpData, dwWriteSize, &dwWritedSize, NULL) == FALSE ) {
		return (size_t)-1;
	}
	return (size_t)dwWritedSize;
#else
	if( m_hPipe[XIO_READ] == 0 ) { 	return (size_t)-1;}
	return ::read(m_hPipe[XIO_READ],  lpData, nSize);
#endif
}

///////////////////////////////////////////////////////////////////////////
//
//	XProccess
//
XProccess::XProccess()
{
	m_nProcID = 0;
#ifdef WIN32
	m_hProccess = NULL;
	m_hThread = NULL;
#else

#endif
}
XProccess::~XProccess()
{
	m_nProcID = 0;
#ifdef WIN32
	if( m_hProccess != NULL ) {
		::CloseHandle(m_hProccess);
		m_hProccess = NULL;
	}
	if( m_hThread != NULL ) {
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
#else

#endif
}
XProccess *XProccess::Execute(XStrBuffList &command, EXEC_PIPE execMode/*= EXEC_PIPE_NON*/, const TCHAR* pDir /*= NULL*/, bool bHide /*= false*/)
{
	XProccess *pProc = new XProccess();

	if( execMode != EXEC_PIPE_NON ) {
		// パイプを作成
		if( pProc->CreatePipe() == false ) {
			delete pProc;
			return NULL;
		}
	}
#ifdef WIN32
	XStrBuff strCommendLine(_T(""));
	
	PROCESS_INFORMATION  prcInf;
	SECURITY_ATTRIBUTES  sa = {sizeof(PSECURITY_ATTRIBUTES), NULL, TRUE};
	STARTUPINFO          startup;

	::memset(&prcInf, 0, sizeof(prcInf));
	::memset(&startup, 0, sizeof(startup));
	::GetStartupInfo(&startup);
/*
--
CreatePipe(&pfd_out[R], &pfd_out[W], &SA, 0);
DuplicateHandle(hParent, pfd_out[R], hParent, &fd_write, 0, FALSE, DUPLICATE_SAME_ACCESS);
CloseHandle(pfd_out[R]);
-----

　まずはCreatePipeによりpfd_out配列の二つのデータをそれぞれREAD,WRITE
とします。READの方をfd_writeに複写してCloseHandleで閉じます。これで
「pfd_out[W]」と「fd_write」がパイプでつながったことになります。次の３
行も同じです。CreatePipeによりpfd_err配列の二つをそれぞれREAD,WRITEと
します。READの方をfd_errに複写して閉じます。これで同じく「pfd_err[W]」
と「fd_err」がつながったことになります。
  
 
	CreatePipe(&pfd_out[R], &pfd_out[W], &SA, 0);
    DuplicateHandle(hParent, pfd_out[R], hParent, &fd_write, 0, FALSE, DUPLICATE_SAME_ACCESS);
    CloseHandle(pfd_out[R]);
    
    CreatePipe(&pfd_in[R], &pfd_in[W], &SA, 0); 
    DuplicateHandle(hParent, pfd_in[W], hParent, &fd_read, 0, FALSE, DUPLICATE_SAME_ACCESS);
    CloseHandle(pfd_in[W]);

    SI.hStdInput   = pfd_in[R];
    SI.hStdOutput  = pfd_out[W];

    WriteFile(fd_read, "dir\r\n",  5, &Len, NULL);
    WriteFile(fd_read, "exit\r\n", 6, &Len, NULL);

    FlushFileBuffers(fd_write);
    FlushFileBuffers(fd_read);

    WaitForSingleObject(PI.hProcess, INFINITE);
    
    ReadFile(fd_write, MainMemory, TEXT_SIZE - 1, &Len, NULL);
    MainMemory[Len] = '\0';

    CloseHandle(PI.hProcess);
 */


	if( execMode != EXEC_PIPE_NON ) {
		startup.hStdInput  = pProc->m_hPopeChiledSideRd[XIO_READ];
		startup.hStdOutput = pProc->m_hPopeChiledSideWr[XIO_WRITE];
		startup.dwFlags		= STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;	
	}
	for(int i = 0; i < command.GetCount(); i++) {
		XStrBuff *pTemp = command.GetAt( i );
		bool bSpace = false;
		if( i != 0 ) {
			strCommendLine += _T(" ");
		}
		if( XStrBuff::StrChr(*pTemp, _T(' ') ) != NULL ) {
			strCommendLine += _T("\"");
			bSpace = true;
		}
		strCommendLine += *pTemp;
		if( bSpace == true ) {
			strCommendLine += _T("\"");
		}
	}

	if( bHide == true ) {
		startup.dwFlags |= STARTF_USESHOWWINDOW;
		startup.wShowWindow = SW_HIDE;
	}

	if( ::CreateProcess(NULL, 
					strCommendLine, 
					&sa, 
					&sa, 
					TRUE, 
					//DETACHED_PROCESS,//0, 
					0,
					NULL, 
					pDir, 
					&startup, 
					&prcInf) == FALSE ) {
		
		delete pProc;
		return NULL;
	}
	pProc->m_hProccess   = prcInf.hProcess;
	pProc->m_hThread     = prcInf.hThread;
	pProc->m_nProcID     = prcInf.dwProcessId;
#else
	XStrBuffList newCmd;
	int i;
	pid_t pid;
	char *pCommandName;


	for(i = 0; i < command.GetCount(); i++) {
		XStrBuff *pTemp = command.GetAt( i );
		if( XStrBuff::StrChr(*pTemp, _T(' ') ) != NULL ) {
			newCmd.append(_T("\"%s\""), (const TCHAR*)*pTemp);
		} else {
			newCmd.append((const TCHAR*)*pTemp);
		}
	}

	char **pArgv = new char*[ command.GetCount() + 1 ];
	if( pArgv == NULL ) {
		delete pProc;
		return NULL;		
	}

	pCommandName = *newCmd.GetAt( 0 );
	for(i = 1; i < newCmd.GetCount(); i++) {
		XStrBuff *pTemp = newCmd.GetAt( i );
		pArgv[i] = (char*)*pTemp;
	}

	::pipe( pProc->m_hPopeChiledSideRd );
	::pipe( pProc->m_hPopeChiledSideWr );


	pArgv[i] = 0;
	pid = vfork();																//	プロセスの分岐
	if(pid == -1){
		::close(pProc->m_hPopeChiledSideWr[XIO_WRITE]); 
		::close(pProc->m_hPopeChiledSideWr[XIO_READ]);
		::close(pProc->m_hPopeChiledSideRd[XIO_WRITE]); 
		::close(pProc->m_hPopeChiledSideRd[XIO_READ]);
	
		pProc->m_hPopeChiledSideRd[XIO_READ] = pProc->m_hPopeChiledSideRd[XIO_WRITE] = 0;
		pProc->m_hPopeChiledSideWr[XIO_READ] = pProc->m_hPopeChiledSideWr[XIO_WRITE] = 0;

		delete[] pArgv;
		delete pProc;
		return NULL;
	} else
	if( pid == 0 ) { 
		
		::close(pProc->m_hPopeChiledSideWr[XIO_WRITE]); 
		::close(pProc->m_hPopeChiledSideRd[XIO_READ]);
		::dup2(pProc->m_hPopeChiledSideWr[XIO_READ],0);
		::dup2(pProc->m_hPopeChiledSideRd[XIO_WRITE],1);
		::close(pProc->m_hPopeChiledSideWr[XIO_READ]);
		::close(pProc->m_hPopeChiledSideRd[XIO_WRITE]);

		if( ::execvp(pCommandName, pArgv) == -1 ){
			delete[] pArgv;
			delete pProc;
			_exit(-1);
		}
		delete[] pArgv;
		_exit(0);
	}
	delete[] pArgv;
	pProc->m_nProcID     = pid;

  termios t;

  // 親プロセス標準入力の行バッファリングを停止
//  tcgetattr(0, &t);
//  t.c_lflag &= ~ICANON; t.c_lflag &= ~ECHO; t.c_lflag &= ~ECHONL; //t.c_lflag &= ~ISIG;
//  t.c_cc[VMIN] = 1; t.c_cc[VTIME] = 0;
//  tcsetattr(0, TCSANOW, &t);
	::close(pProc->m_hPopeChiledSideWr[XIO_READ]);
	::close(pProc->m_hPopeChiledSideRd[XIO_WRITE]);
	pProc->m_hPipe[XIO_WRITE]  = pProc->m_hPopeChiledSideWr[XIO_WRITE];
	pProc->m_hPipe[XIO_READ]   = pProc->m_hPopeChiledSideRd[XIO_READ];
#endif
	return pProc;
}

int XProccess::getpid()
{
XEXCEPT_ENTRY
#ifdef WIN32
	return _getpid();
#else
	return getpid();
#endif
XEXCEPT_EXIT
}
bool XProccess::Kill()
{
#ifdef WIN32
	if( m_hProccess == NULL ) {
		return false;
	}

	if( ::TerminateProcess(m_hProccess, 0) == FALSE ) {
		return false;
	}
	if( m_hProccess != NULL ) {
		::CloseHandle(m_hProccess);
		m_hProccess = NULL;
	}
	if( m_hThread != NULL ) {
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	return true;
#else
	if(m_nProcID == 0 || ((long)m_nProcID) < 0) {
		return false;
	}

	if( ::kill((pid_t)m_nProcID, SIGTERM) == -1) {
		return false;
	}
	m_nProcID = 0;
	return true;
#endif
}
unsigned long XProccess::Wait()
{
	unsigned long nRet = (unsigned long)-1;
#ifdef WIN32
	if( m_hProccess == NULL ) {
		return (unsigned long)-1;
	}

	if( ::WaitForSingleObject(m_hProccess, INFINITE) != WAIT_OBJECT_0 ) {
		return (unsigned long)-1;
	}
	::GetExitCodeProcess(m_hProccess, &nRet);

	if( m_hProccess != NULL ) {
		::CloseHandle(m_hProccess);
		m_hProccess = NULL;
	}
	if( m_hThread != NULL ) {
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
#else
	if(m_nProcID == 0 || ((long)m_nProcID) < 0) {
		return (unsigned long)-1;
	}

	pid_t pid = ::waitpid(m_nProcID, (int*)&nRet, 0);
	if(pid == -1){
		return (unsigned long)-1;
	}
#endif
	return nRet;
}

