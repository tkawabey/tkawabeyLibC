#include "xlib/xmapfile.h"

#ifdef WIN32

#else 
  #include <sys/mman.h>
  #include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "xlib/xeception.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XMapFile
//
XMapFile::XMapFile(size_t nSize)
	:	XFile()
	,	m_nSize(nSize)
	,	m_hMap(0)
	,	m_lpAddress(NULL)
{
}
XMapFile::~XMapFile()
{
	Close();
}
bool XMapFile::Close()
{
	if(m_lpAddress != NULL){
		Unmap();
	}
#ifdef WIN32
	if( m_iFile == -1 ) {
		::CloseHandle((HANDLE)m_iFile);
		m_iFile = -1;
	}
#else
	XFile::Close();
#endif
	m_nSize = 0;
	m_hMap = (int)0;
	m_lpAddress = NULL;
	return true;
}
size_t XMapFile::WriteRaw(const void* lpData, size_t nSize, int flg)
{
	return (size_t)-1;
}
size_t XMapFile::ReadRaw(void* lpData, size_t nSize, int flg)
{
	return (size_t)-1;
}
bool XMapFile::CreateMap(unsigned long nSize, int nFd, bool bReadOnly /*= false*/)
{
#ifdef WIN32
	// マップフハンドル取得
	HANDLE hMap = ::CreateFileMapping((HANDLE)nFd, 
								NULL, 
								bReadOnly == true ? PAGE_READONLY : PAGE_READWRITE,
                                0,     // Not worrying about 64-bit
                                nSize, // file sizes 
								NULL);
	if( nSize > 0 ) {
		m_nSize = nSize;
	} else {
		m_nSize = ::GetFileSize((HANDLE)nFd, NULL);
	}
	m_hMap = (int)hMap;
	if(hMap == NULL) {
		return false;
	}
	// メモリ割当
	m_lpAddress = ::MapViewOfFile(
					   (HANDLE)m_hMap,
					   bReadOnly == true ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS,
					   0,   // Offset high
					   0,//dwOffset,
					   m_nSize);
	return (m_lpAddress != NULL) ? true : false;
#else // WIN32
	if( nSize > 0 ) {
		long	psize, size;
		char	c;
		
		psize = ::getpagesize();
//	    psize = ::sysconf(_SC_PAGESIZE);
		size =  (nSize / psize+1 ) * psize;

		/* ファイルの必要サイズ分先にシークし、0を書き込み */
		/* ファイルのサイズをマップしたいサイズにする為 */
		if( ::lseek(nFd, size, SEEK_SET) <0 ) {
			XTRACE(_T("XMapFile::CreateMap lseek err(%s)\n"), strerror(errno) );
			return false;
		}
		if( ::read(nFd, &c, sizeof(char))==-1){
			c='\0';
			XTRACE(_T("XMapFile::CreateMap read err(%s)\n"), strerror(errno) );
			return false;
		}
		if( ::write(nFd, &c, sizeof(char))==-1){
			XTRACE(_T("XMapFile::CreateMap write err(%s)\n"), strerror(errno) );
			return false;
		}
		m_nSize = size;
	} else {
		struct stat  sts;
		::fstat(nFd, &sts);
		m_nSize = sts.st_size;
	}
	// メモリ割当
	m_lpAddress = ::mmap(0, m_nSize,
					bReadOnly == true ? PROT_READ : PROT_READ | PROT_WRITE,
					MAP_SHARED, 
					nFd,
					0);
	return (m_lpAddress != NULL) ? true : false;
#endif // WIN32
}
bool XMapFile::OpenRawA(const char *filename, int flg, bool bShare, int pmode)
{
	int  nSize = 0;
	bool bReadOnly = false;
	

	if( flg & O_CREAT ) {
		nSize = m_nSize;
	}
#ifdef WIN32
	if( (flg & _O_RDWR) == 0 ) {
		bReadOnly = true;
	}

	pmode = 0;
	int pmodeWin = 0;
	if( pmode & S_IREAD) {
		pmodeWin |= S_IREAD;
	}
	if( pmode & S_IWRITE) {
		pmodeWin |= S_IWRITE;
	}
	pmode = pmodeWin;

    DWORD fileaccess = 0; 
    DWORD fileshare = 0;
    DWORD filecreate = 0;
    DWORD fileattrib = 0;
    DWORD isdev = 0;
	SECURITY_ATTRIBUTES SecurityAttributes;

    SecurityAttributes.nLength = sizeof( SecurityAttributes );
    SecurityAttributes.lpSecurityDescriptor = NULL;


	switch( flg & (_O_RDONLY | _O_WRONLY | _O_RDWR) ) {
	case _O_RDONLY:
		fileaccess = GENERIC_READ;
		break;
	case _O_WRONLY:
		fileaccess = GENERIC_WRITE;
		break;
	case _O_RDWR:
		fileaccess = GENERIC_READ | GENERIC_WRITE;
		break;
	default:
		errno = EINVAL;
		_doserrno = 0L;
		return false;
	}

	if( bShare ) {
		fileshare = FILE_SHARE_READ | FILE_SHARE_WRITE;
	}
	
	switch ( flg & (_O_CREAT | _O_EXCL | _O_TRUNC) ) {
	case 0:
	case _O_EXCL:
		filecreate = OPEN_EXISTING;
		break;
	case _O_CREAT:
		filecreate = OPEN_ALWAYS;
		break;
	case _O_CREAT | _O_EXCL:
	case _O_CREAT | _O_TRUNC | _O_EXCL:
		filecreate = CREATE_NEW;
		break;
	case _O_TRUNC:
	case _O_TRUNC | _O_EXCL:
		filecreate = TRUNCATE_EXISTING;
		break;
	case _O_CREAT | _O_TRUNC:
		filecreate = CREATE_ALWAYS;
		break;

	default:
		errno = EINVAL;
		_doserrno = 0L;
		return false;
	}
	fileattrib = FILE_ATTRIBUTE_NORMAL;

	HANDLE hFile = ::CreateFileA( filename,
                                 fileaccess,
                                 fileshare,
                                 &SecurityAttributes,
                                 filecreate,
                                 fileattrib,
                                 NULL );
	if( hFile == (HANDLE)0xffffffff ) {
		return false;
	}
	
	m_iFile = (int)hFile;

#else
	if( (flg & O_RDWR) == 0 ) {
		bReadOnly = true;
	}
	if( XFile::OpenRawA(filename, flg, bShare, pmode ) == false ) {
		return false;
	}
#endif
	return this->CreateMap(nSize, m_iFile, bReadOnly);
}

bool XMapFile::OpenRawW(const wchar_t *filename, int flg, bool bShare, int pmode)
{
	return this->OpenRawA( XStrBuff(filename), flg, bShare, pmode);
}
bool XMapFile::Unmap()
{
   bool bRetcode = false;
   if(m_lpAddress != NULL){
#ifdef WIN32
	   bRetcode = ::UnmapViewOfFile(m_lpAddress) == TRUE ? true : false;
#else
	   bRetcode = ::munmap(m_lpAddress, m_nSize) == 0 ? true : false;
#endif
	   m_lpAddress = NULL;
   }
   return bRetcode;
}
bool XMapFile::Fulush()
{
#ifdef WIN32
	::FlushViewOfFile(m_lpAddress, m_nSize);
#else
	::msync(m_lpAddress, m_nSize, 0);
#endif
	return true;
}
void* XMapFile::getMemAddress()
{
	return m_lpAddress;
}
size_t XMapFile::GetLength()
{
	return m_nSize;
}
bool XMapFile::Seek(long offset, SeekPosition origin)
{
	return true;
}


