#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/types.h>
	#include <errno.h>
    #include <string.h>
#endif // WIN32

#include "xlib/xbuff.h"
#include "xlib/xeception.h"


XBuff::XBuff()
{
XEXCEPT_ENTRY
	m_pBuff		= NULL;
	m_nBuffSize = 0;
XEXCEPT_EXIT
}


XBuff::XBuff(const XBuff& dst, int dmmy /*= 0*/)
{
XEXCEPT_ENTRY
	m_pBuff		= NULL;
	m_nBuffSize = 0;
	*this = dst;
XEXCEPT_EXIT
}

XBuff::~XBuff()
{
XEXCEPT_ENTRY
	Free();
XEXCEPT_EXIT
}
void *XBuff::Alloc(size_t nSize)
{
	#undef  RNAME
	#define RNAME "XBuff::Alloc"
	int		iCheckPoint = 0;
	try {
		unsigned char *p;

		___XEXCEPT_CHECK_VALID_TRUE(nSize != 0);


		Free();
		iCheckPoint++;
		m_pBuff = (void*)new unsigned char[nSize];
		iCheckPoint++;
		if(m_pBuff) {
			p = (unsigned char*)m_pBuff;
			m_nBuffSize = nSize;
		} else {
#if WIN32
			::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
#else
			errno = ENOMEM;
#endif// WIN32
		}
		iCheckPoint++;
		return  m_pBuff;
	} catch (XExcept& ex) { 
		XEXCEPT_RETHROW(ex, XExcept_RETHROW); 
	} catch (std::bad_alloc&) { 
		XEXCEPT_THROW(XExcept_MEMORY_ALLOC, iCheckPoint, nSize); 
	} catch (std::exception&) { 
		XEXCEPT_THROW(XExcept_NO_SUPPORT, iCheckPoint, nSize); 
	} catch (...) { 
		XEXCEPT_THROW(XExcept_UNKNOWN, iCheckPoint, nSize); 
	}
}
void *XBuff::ReAlloc(size_t nSize)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XBuff::ReAlloc"
	XBuff tmpBuff;
	unsigned char *p;
	unsigned int  nCpsize;

	___XEXCEPT_CHECK_VALID_TRUE(nSize != 0);

	if( tmpBuff.DoCopy(this) == NULL ) {
		return NULL;
	}
	p = (unsigned char *)this->Alloc(nSize);
	if(p == NULL) {
		return NULL;
	}
	nCpsize = tmpBuff.GetBuffSize();
	if( this->GetBuffSize() < nCpsize ) {
		nCpsize  = this->GetBuffSize();
	}
	::memcpy(*this, tmpBuff, nCpsize);
	return m_pBuff;
XEXCEPT_EXIT
	return NULL;
}

void* XBuff::memcpy(const void *src, size_t count, bool bReUse /*= true*/)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XBuff::memcpy"
	void *p = NULL;

	___XEXCEPT_CHECK_VALID_TRUE(src != 0);
	___XEXCEPT_CHECK_VALID_TRUE(count != 0);



	if( bReUse == true ) {
		if( m_nBuffSize > count ) {
			this->MemSet(0);
			return ::memcpy(m_pBuff, src, count);
		}
	}

	p = this->Alloc( count );
	if( p == NULL ) {
		return NULL;
	}
	this->MemSet(0);
	return ::memcpy(p, src, count);
XEXCEPT_EXIT
	return NULL;
}

void XBuff::Free()
{
XEXCEPT_ENTRY
	unsigned char *p = (unsigned char*)m_pBuff;
	if( p ) {
		delete []p; 
		m_pBuff = NULL;
	}
	m_nBuffSize = 0;
XEXCEPT_EXIT
}
void XBuff::MemSet(unsigned char c /*= 0*/)
{
XEXCEPT_ENTRY
	if( m_nBuffSize == 0) {
		return;
	}
	::memset(m_pBuff, c, m_nBuffSize);
XEXCEPT_EXIT
}

void XBuff::memsetA(unsigned char c /*= 0*/)
{
XEXCEPT_ENTRY
	if( m_nBuffSize == 0) {
		return;
	}
	::memset(m_pBuff, c, m_nBuffSize);
XEXCEPT_EXIT
}
void XBuff::memsetW(wchar_t c /*= 0*/)
{
XEXCEPT_ENTRY
	size_t count = m_nBuffSize / sizeof(wchar_t);
	if( m_nBuffSize == 0) {
		return;
	}

	::wmemset((wchar_t*)m_pBuff, c, count);
XEXCEPT_EXIT
}
void XBuff::memsetT(TCHAR c /*= 0*/)
{
#ifdef _UNICODE
	XBuff::memsetW(c);
#else
	XBuff::memsetA(c);
#endif
}


XBuff::operator void*()
{
XEXCEPT_ENTRY
	return m_pBuff;
XEXCEPT_EXIT
}
XBuff::operator char*()
{
XEXCEPT_ENTRY
	return (char*)m_pBuff;
XEXCEPT_EXIT
}
XBuff::operator unsigned char*()
{
XEXCEPT_ENTRY
	return (unsigned char*)m_pBuff;
XEXCEPT_EXIT
}
XBuff::operator unsigned short*()
{
XEXCEPT_ENTRY
	return (unsigned short*)m_pBuff;
XEXCEPT_EXIT
}
XBuff::operator unsigned long*()
{
XEXCEPT_ENTRY
	return (unsigned long*)m_pBuff;
XEXCEPT_EXIT
}
#ifdef _NATIVE_WCHAR_T_DEFINED
XBuff::operator wchar_t*()
{
XEXCEPT_ENTRY
	return (wchar_t*)m_pBuff;
XEXCEPT_EXIT
}
#endif
void *XBuff::GetMaxBuffAddress() const
{
XEXCEPT_ENTRY
	unsigned char *p = (unsigned char*)m_pBuff;
	if( p ) {
		return p += m_nBuffSize;
	} else {
		return NULL;
	}
XEXCEPT_EXIT
}
size_t XBuff::GetBuffSize()  const
{
XEXCEPT_ENTRY
	return m_nBuffSize;
XEXCEPT_EXIT
}
void *XBuff::DoCopy(XBuff *pSrc)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XBuff::DoCopy"

	___XEXCEPT_CHECK_VALID_TRUE(pSrc != 0);


//	_ASSERT( pSrc->GetBuffSize() );
	void *p = Alloc( pSrc->GetBuffSize() );
	if( p == NULL )
		return NULL;
	::memcpy(p, *pSrc,  pSrc->GetBuffSize());
	return p;
XEXCEPT_EXIT
}
// 指定したバッファと比較します。
bool XBuff::operator==(const XBuff& dst) const
{
XEXCEPT_ENTRY
	if( this->m_nBuffSize != dst.m_nBuffSize ) {
		return false;
	}

	if( ::memcmp(this->m_pBuff, dst.m_pBuff, this->m_nBuffSize) != 0 ) {
		return false;
	}
	return true;
XEXCEPT_EXIT
}
// 指定したバッファと比較します。
bool XBuff::operator!=(const XBuff& dst) const
{
XEXCEPT_ENTRY
	if( this->m_nBuffSize != dst.m_nBuffSize ) {
		return true;
	}

	if( ::memcmp(this->m_pBuff, dst.m_pBuff, this->m_nBuffSize) != 0 ) {
		return true;
	}
	return false;
XEXCEPT_EXIT
}
XBuff& XBuff::operator=(const XBuff& dst)
{
XEXCEPT_ENTRY
	Free();

	if( dst.m_nBuffSize == 0 ) {
		return *this;
	}
	void *p = this->Alloc( dst.m_nBuffSize);
	if( p == NULL ) {
		return *this;
	}
	::memcpy(p, dst.m_pBuff,  dst.m_nBuffSize );
	return *this;
XEXCEPT_EXIT
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XInflateStreamBuff
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
XInflateStreamBuff::XInflateStreamBuff(unsigned long nInitSize, unsigned long nIncrementSize)
	:_Seekhigh(0)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XInflateStreamBuff::XInflateStreamBuff"

	___XEXCEPT_CHECK_VALID_TRUE(nInitSize != 0);



	m_pBuffer = (char*)::malloc(nInitSize);
	m_nBufferSize = nInitSize;
	m_nIncrementSize = nIncrementSize;
	*m_pBuffer = 0;

	_Seekhigh = m_pBuffer;
	setp( m_pBuffer, m_pBuffer + (nInitSize-1) );
//	setg( m_pBuffer, m_pBuffer, m_pBuffer + (nInitSize-1) );
	setg( m_pBuffer, m_pBuffer, m_pBuffer );
XEXCEPT_EXIT
}

XInflateStreamBuff::~XInflateStreamBuff()
{
XEXCEPT_ENTRY
	if( m_pBuffer ) {
		::free(m_pBuffer);
		m_pBuffer = NULL;
	}
XEXCEPT_EXIT
}
std::streampos XInflateStreamBuff::seekoff( std::streamoff off, 
								std::ios::seekdir way, 
								std::ios::openmode which /*= std::ios::in | std::ios::out*/ )
{
XEXCEPT_ENTRY
	if (pptr() != 0 && _Seekhigh < pptr())
		_Seekhigh = pptr();
	if (which & std::ios::in && gptr() != 0) {
		if (way == std::ios::end)
			off += _Seekhigh - eback();
		else if (way == std::ios::cur && !(which & std::ios::out))
			off += gptr() - eback();
		else if (way != std::ios::beg || off == EOF)
			off = EOF;
		if (0 <= off && off <= _Seekhigh - eback())	{
			gbump((int)(eback() - gptr() + off));
			if (which & std::ios::out && pptr() != 0) {
//				setp(pbase(), gptr(), epptr());
				setp(pbase(), epptr());
			}
		}
		else {
			off = EOF;
		}
	} else
	if (which & std::ios::out && pptr() != 0) {
		if (way == std::ios::end)
			off += _Seekhigh - eback();
		else if (way == std::ios::cur) {
//			off += pptr() - m_pBuffer;
			off += pptr() - eback();
		}
		else if (way != std::ios::beg || off == EOF)
			off = EOF;

		std::streamoff off_cmp = _Seekhigh - eback();
		if (0 <= off && off <= _Seekhigh - eback())
			pbump((int)(eback() - pptr() + off));
		else
			off = EOF;
	} else {
		off = EOF;
	}
	return (std::streampos(off));
XEXCEPT_EXIT
}
std::streampos XInflateStreamBuff::seekpos( std::streampos sp, std::ios::openmode which /*= std::ios::in | std::ios::out*/)
{
XEXCEPT_ENTRY
	std::streamoff off = (std::streamoff)sp;
	if (pptr() != 0 && _Seekhigh < pptr())
		_Seekhigh = pptr();
	if (off == EOF)
		;
	else if (which & std::ios::in && gptr() != 0) {	// set input (and maybe output) pointer
		if (0 <= off && off <= _Seekhigh - eback())	{	// set valid offset
			gbump((int)(eback() - gptr() + off));
			if (which & std::ios::out && pptr() != 0) {
//				setp(pbase(), gptr(), epptr());
				setp(pbase(), epptr());
			}
		}
		else
			off = EOF;
	}
	else if (which & std::ios::out && pptr() != 0) {	// set output pointer
		if (0 <= off && off <= _Seekhigh - eback())
			pbump((int)(eback() - pptr() + off));
		else
			off = EOF;
	}
	else	// nothing to set
		off = EOF;
	return (std::streampos(off));
XEXCEPT_EXIT
}
int XInflateStreamBuff::overflow( int nCh /*= EOF*/ )
{
XEXCEPT_ENTRY
	unsigned long n = m_nBufferSize;

	if (nCh == EOF)
		return (0);

	m_nBufferSize += m_nIncrementSize;
	m_pBuffer = (char*)::realloc(m_pBuffer, m_nBufferSize);
	if( m_pBuffer == NULL ) {
		return EOF;
	}
//	m_pBuffer[n-1] = (char)nCh;

	_Seekhigh = _Seekhigh - eback() + m_pBuffer;
#ifdef WIN32
	setp(pbase() - eback() + m_pBuffer, pptr() - eback() + m_pBuffer, m_pBuffer + (m_nBufferSize));
#else 
	setp(pbase() - eback() + m_pBuffer, pptr() - eback() + m_pBuffer);
#endif
	setg(m_pBuffer, gptr() - eback() + m_pBuffer, pptr() + 1);
//	setp( m_pBuffer+(n), m_pBuffer + (m_nBufferSize-1) );
//	setg( m_pBuffer+(n), m_pBuffer+(n), m_pBuffer + (m_nBufferSize-1) );
//	_Seekhigh = pptr();
#ifdef WIN32
	return ((unsigned char)(*_Pninc() = (char)nCh));
#else 
	return nCh;
#endif
XEXCEPT_EXIT
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XInflateStreamBuffW
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
XInflateStreamBuffW::XInflateStreamBuffW(unsigned long nInitSize, unsigned long nIncrementSize)
	:_Seekhigh(0)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XInflateStreamBuffW::XInflateStreamBuffW"

	___XEXCEPT_CHECK_VALID_TRUE(nInitSize != 0);



	m_pBuffer = (wchar_t*)::malloc(nInitSize*sizeof(wchar_t));
	m_nBufferSize = nInitSize*sizeof(wchar_t);
	m_nIncrementSize = nIncrementSize*sizeof(wchar_t);
	*m_pBuffer = 0;

	_Seekhigh = m_pBuffer;
	setp( m_pBuffer, m_pBuffer + (nInitSize-1) );					// 書き込みポインタ。
	setg( m_pBuffer, m_pBuffer, m_pBuffer + (nInitSize-1) );		// 読み取りポインタ。
XEXCEPT_EXIT
}

XInflateStreamBuffW::~XInflateStreamBuffW()
{
XEXCEPT_ENTRY
//XTRACE(_T("XInflateStreamBuffW::~XInflateStreamBuffW size:%u\n"), m_nBufferSize);
	if( m_pBuffer ) {
		::free(m_pBuffer);
		m_pBuffer = NULL;
	}
XEXCEPT_EXIT
}
std::streampos XInflateStreamBuffW::seekoff( std::streamoff off, 
								std::ios::seekdir way, 
								std::ios::openmode which /*= std::ios::in | std::ios::out*/ )
{
XEXCEPT_ENTRY
XTRACE(_T("XInflateStreamBuffW::seekoff  new size:%u"), m_nBufferSize);
	if (pptr() != 0 && _Seekhigh < pptr())
		_Seekhigh = pptr();
	if (which & std::ios::in && gptr() != 0) {
		if (way == std::ios::end)
			off += _Seekhigh - eback();
		else if (way == std::ios::cur && !(which & std::ios::out))
			off += gptr() - eback();
		else if (way != std::ios::beg || off == EOF)
			off = EOF;
		if (0 <= off && off <= _Seekhigh - eback())	{
			gbump((int)(eback() - gptr() + off));
			if (which & std::ios::out && pptr() != 0) {
//				setp(pbase(), gptr(), epptr());
				setp(pbase(), epptr());
			}
		}
		else {
			off = EOF;
		}
	} else
	if (which & std::ios::out && pptr() != 0) {
		if (way == std::ios::end)
			off += _Seekhigh - eback();
		else if (way == std::ios::cur) {
//			off += pptr() - m_pBuffer;
			off += pptr() - eback();
		}
		else if (way != std::ios::beg || off == EOF)
			off = EOF;

		std::streamoff off_cmp = _Seekhigh - eback();
		if (0 <= off && off <= _Seekhigh - eback())
			pbump((int)(eback() - pptr() + off));
		else
			off = EOF;
	} else {
		off = EOF;
	}
	return (std::streampos(off));
XEXCEPT_EXIT
}
std::streampos XInflateStreamBuffW::seekpos( std::streampos sp, std::ios::openmode which /*= std::ios::in | std::ios::out*/)
{
XEXCEPT_ENTRY
XTRACE(_T("XInflateStreamBuffW::seekpos  new size:%u\n"), m_nBufferSize);
	std::streamoff off = (std::streamoff)sp;
	if (pptr() != 0 && _Seekhigh < pptr())
		_Seekhigh = pptr();
	if (off == EOF)
		;
	else if (which & std::ios::in && gptr() != 0) {	// set input (and maybe output) pointer
		if (0 <= off && off <= _Seekhigh - eback())	{	// set valid offset
			gbump((int)(eback() - gptr() + off));
			if (which & std::ios::out && pptr() != 0) {
//				setp(pbase(), gptr(), epptr());
				setp(pbase(), epptr());
			}
		}
		else
			off = EOF;
	}
	else if (which & std::ios::out && pptr() != 0) {	// set output pointer
		if (0 <= off && off <= _Seekhigh - eback())
			pbump((int)(eback() - pptr() + off));
		else
			off = EOF;
	}
	else	// nothing to set
		off = EOF;
	return (std::streampos(off));
XEXCEPT_EXIT
}
wint_t XInflateStreamBuffW::overflow( wint_t nCh /*= EOF*/ )
{
XEXCEPT_ENTRY
	unsigned long n = m_nBufferSize;
	wchar_t*	  pEnd = NULL;
	if (nCh == EOF)
		return (0);

	m_nBufferSize += m_nIncrementSize;
	m_pBuffer = (wchar_t*)::realloc(m_pBuffer, m_nBufferSize);
	if( m_pBuffer == NULL ) {
		return EOF;
	}
	pEnd = m_pBuffer;
	pEnd +=  ( (m_nBufferSize/(sizeof(wchar_t))) - 4);

//	m_pBuffer[n-1] = (char)nCh;

	_Seekhigh = _Seekhigh - eback() + m_pBuffer;
#ifdef WIN32
XTRACE(_T("XInflateStreamBuffW::overflow  new size:%u\n"), m_nBufferSize);
	setp(
		pbase() - eback() + m_pBuffer,	// アドレスの先頭
		pptr() - eback() + m_pBuffer,	// 次に書き込む位置
		m_pBuffer + ((m_nBufferSize/(sizeof(wchar_t))))	// アドレスの最後
	);
#else 
	setp(pbase() - eback() + m_pBuffer, pptr() - eback() + m_pBuffer);
#endif
	setg(m_pBuffer, gptr() - eback() + m_pBuffer, pptr() + 1);
//	setp( m_pBuffer+(n), m_pBuffer + (m_nBufferSize-1) );
//	setg( m_pBuffer+(n), m_pBuffer+(n), m_pBuffer + (m_nBufferSize-1) );
//	_Seekhigh = pptr();
#ifdef WIN32
	return ((wchar_t)(*_Pninc() = (wchar_t)nCh));
#else 
	return nCh;
#endif
XEXCEPT_EXIT
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XRingBuffer
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
XInflateBinStrstream::XInflateBinStrstream()
	: std::iostream(&m_buff)
	, m_buff(128, 256)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XInflateBinStrstream::XInflateBinStrstream(unsigned long nInitSize, unsigned long nIncrementSize /*= 1024*/)
	: std::iostream(&m_buff)
	, m_buff(nInitSize, nIncrementSize)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XInflateBinStrstream::~XInflateBinStrstream()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XInflateStreamBuff *XInflateBinStrstream::rdbuf() const
{
XEXCEPT_ENTRY
	return (XInflateStreamBuff*)&m_buff;
XEXCEPT_EXIT
}
XBIN *	XInflateBinStrstream::buffer()
{
XEXCEPT_ENTRY
	return (XBIN*)m_buff.getBuff();
XEXCEPT_EXIT
}
std::streamsize XInflateBinStrstream::pcount()
{
XEXCEPT_ENTRY
	return (std::streamsize)m_buff.size();
XEXCEPT_EXIT
}
void XInflateBinStrstream::resetp()
{
XEXCEPT_ENTRY
	m_buff.resetp();
XEXCEPT_EXIT
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	XRingBuffer
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
XRingBuffer::XRingBuffer()
	:	m_ulRealSize(1)
	,	m_ulFirst(0)
	,	m_ulLast(0) 
	,	m_bAlloc( false )
	,	m_pBuffer(NULL)
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}

XRingBuffer::~XRingBuffer(void)
{
XEXCEPT_ENTRY
	if( m_pBuffer != NULL && m_bAlloc == true ) {
		delete [] m_pBuffer;
	}
XEXCEPT_EXIT
}

bool XRingBuffer::alloc(unsigned long size)
{
XEXCEPT_ENTRY
	if( m_pBuffer != NULL && m_bAlloc == true ) {
		delete [] m_pBuffer;
	}
	m_pBuffer = NULL;
	m_bAlloc  = false;
	m_ulRealSize = 1;
	while (size > (m_ulRealSize -1)) {
		m_ulRealSize <<= 1;
	}
	m_pBuffer = new unsigned char[m_ulRealSize];
	return true;
XEXCEPT_EXIT
}
bool XRingBuffer::attach(unsigned char* p_buff, unsigned long size)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XRingBuffer::attach"

	___XEXCEPT_CHECK_VALID_TRUE(p_buff != 0);
	___XEXCEPT_CHECK_VALID_TRUE(size != 0);


	if( m_pBuffer != NULL && m_bAlloc == true ) {
		delete [] m_pBuffer;
	}
	m_pBuffer = NULL;
	m_bAlloc  = false;

	m_ulRealSize = 1;
	while (size > (m_ulRealSize -1)) {
		m_ulRealSize <<= 1;
	}
	m_pBuffer = p_buff;
	return true;
XEXCEPT_EXIT
}


unsigned long XRingBuffer::size(void) const
{
XEXCEPT_ENTRY
	if( m_pBuffer == NULL ) { return 0; }
	return (m_ulLast >= m_ulFirst) ? m_ulLast - m_ulFirst : m_ulRealSize - (m_ulFirst - m_ulLast);
XEXCEPT_EXIT
}

unsigned long XRingBuffer::free_size(void) const
{
XEXCEPT_ENTRY
	if( m_pBuffer == NULL ) { return 0; }
	return m_ulRealSize -1 - size();
XEXCEPT_EXIT
}

bool XRingBuffer::put(const unsigned char* data, unsigned long size)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XRingBuffer::put"

	___XEXCEPT_CHECK_VALID_TRUE(data != 0);
	___XEXCEPT_CHECK_VALID_TRUE(size != 0);



	if( m_pBuffer == NULL ) { return false; }

	unsigned long free_length = free_size();
	unsigned long len = (size > free_length) ? free_length : size;
	unsigned long put_end = (len > (m_ulRealSize - m_ulLast)) ? m_ulRealSize : m_ulLast + len;
	register unsigned char *p = &m_pBuffer[m_ulLast];
	register unsigned char *q = (unsigned char *)data;

	if( size > free_length ) {
		return false;
	}
	for (unsigned long i = m_ulLast; i < put_end; ++i) {
		*p++ = *q++;
	}

	register unsigned char *data_last = (unsigned char *)data + len;
	if (q < data_last) {
		p = &m_pBuffer[0];
		while (q < data_last) {
			*p++ = *q++;
		}
	}
	m_ulLast = static_cast<unsigned long>((p - &m_pBuffer[0]) & (m_ulRealSize -1));
	return true;
XEXCEPT_EXIT
}

int XRingBuffer::copy(unsigned char* data, unsigned long size) const
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XRingBuffer::copy"

	___XEXCEPT_CHECK_VALID_TRUE(data != 0);
	___XEXCEPT_CHECK_VALID_TRUE(size != 0);

	if( m_pBuffer == NULL ) { return 0; }

	unsigned long filled_length = this->size();
	unsigned long len = (size > filled_length) ? filled_length : size;

	unsigned long get_end = (len > (m_ulRealSize - m_ulFirst)) ? m_ulRealSize : m_ulFirst + len;
	unsigned char *p = data;
	unsigned char *q = &m_pBuffer[m_ulFirst];
	
	for (unsigned long i = m_ulFirst; i < get_end; ++i) {
		*p++ = *q++;
	}

	unsigned char *data_last = data + len;
	if (p < data_last) {
		q = &m_pBuffer[0];
		while (p < data_last) {
			*p++ = *q++;
		}
	}
	return len;
XEXCEPT_EXIT
}

int XRingBuffer::get(unsigned char* data, unsigned long size)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XRingBuffer::get"

	___XEXCEPT_CHECK_VALID_TRUE(data != 0);
	___XEXCEPT_CHECK_VALID_TRUE(size != 0);

	if( m_pBuffer == NULL ) { return 0; }

	int len = copy(data, size);
	m_ulFirst = (m_ulFirst + len) & (m_ulRealSize -1);
	return len;
XEXCEPT_EXIT
}

void XRingBuffer::clear(void)
{
XEXCEPT_ENTRY
	m_ulFirst = 0;
	m_ulLast = 0;
XEXCEPT_EXIT
}




XRingBufferBL::XRingBufferBL(XRingBufferBL::MODE mode /*= XRingBufferBL::LOOP*/)
	:	m_ulCurBlock( 0 )
	,	m_mode( mode )
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
XRingBufferBL::~XRingBufferBL()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}

bool XRingBufferBL::alloc(unsigned long size)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XRingBufferBL::alloc"

	___XEXCEPT_CHECK_VALID_TRUE(size != 0);


	return m_buff.alloc( size );
XEXCEPT_EXIT
}
bool XRingBufferBL::attach(unsigned char* p_buff, unsigned long size)
{
	#undef  RNAME
	#define RNAME "XRingBufferBL::attach"

	___XEXCEPT_CHECK_VALID_TRUE(p_buff != 0);
	___XEXCEPT_CHECK_VALID_TRUE(size != 0);


	return m_buff.attach( p_buff, size);
}

unsigned long XRingBufferBL::size(void) const
{
XEXCEPT_ENTRY
	return m_buff.size();
XEXCEPT_EXIT
}

unsigned long XRingBufferBL::free_size(void) const
{
XEXCEPT_ENTRY
	if( m_buff.free_size() < sizeof(unsigned long) ) {
		return 0;
	}
	return m_buff.free_size() - sizeof(unsigned long);
XEXCEPT_EXIT
}

unsigned long XRingBufferBL::cur_blk_size(void) const
{
XEXCEPT_ENTRY
	unsigned long	ret = 0;

	if( m_buff.size() < sizeof(unsigned long) ) {
		return 0;
	}
	if( m_buff.copy((unsigned char *)&ret, sizeof(unsigned long) ) != sizeof(unsigned long) ) {
		return 0;
	}
	return ret;
XEXCEPT_EXIT
}
unsigned long XRingBufferBL::blk_count(void) const
{
XEXCEPT_ENTRY
	return m_ulCurBlock;
XEXCEPT_EXIT
}
bool XRingBufferBL::put(const unsigned char* data, unsigned long size)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XRingBufferBL::put"

	___XEXCEPT_CHECK_VALID_TRUE(data != 0);
	___XEXCEPT_CHECK_VALID_TRUE(size != 0);

	unsigned long ulSize = 0;
	unsigned long ulReadSize = 0;

	if( this->free_size() < (size + sizeof(unsigned long) )) {

		if( this->size() < (size + sizeof(unsigned long) )) {
			// バッファサイズより書き込みサイズが大きい
			return false;
		}

		if( m_mode == XRingBufferBL::LINE ) {
			// ラインモードは破棄しない
			return false;
		}

		// 書き込めるサイズまで開放する。
		for(; this->free_size() < (size + sizeof(unsigned long) ) ;) {
			// 先頭のブロックを開放する。
			if( m_buff.get((unsigned char *)&ulSize, sizeof(unsigned long)) != sizeof(unsigned long) ) {
				return false;
			}
			unsigned char tmp[511];
			
			while( ulSize != 0 ) {
				ulReadSize = __min( ulSize, sizeof(tmp));

				if( m_buff.get(tmp, ulReadSize) != ulReadSize ) {
					return false;
				}
				ulSize -= ulReadSize;
			}

			m_ulCurBlock--;
		}
	}

	if( m_buff.put((unsigned char *)&size, sizeof(size)) != true
			||
		m_buff.put(data, size) != true
		) {
		return false;
	}
	m_ulCurBlock++;
	return true;
XEXCEPT_EXIT
}
int XRingBufferBL::copy(unsigned char* data, unsigned long buff_size) const
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XRingBufferBL::copy"

	___XEXCEPT_CHECK_VALID_TRUE(data != 0);
	___XEXCEPT_CHECK_VALID_TRUE(buff_size != 0);

	unsigned long ulSize = 0;

	if( m_ulCurBlock == 0 ) {
		// 書き込まれたブロックがない
		return 0;
	}
	if( this->cur_blk_size() > ( buff_size + sizeof(unsigned long) ) ) {
		// 指定バッファサイズが少ない
		return 0;
	}

	if( m_buff.copy((unsigned char *)&ulSize, sizeof(unsigned long)) != sizeof(unsigned long) ) {
		// データが壊れている
		return 0;
	}
	if( ulSize > m_buff.size() ) {
		// データが壊れている
		return 0;
	}
	if( m_buff.copy(data, ulSize + sizeof(unsigned long)) != ( ulSize + sizeof(unsigned long)) ) {
		// データが壊れている
		return 0;
	}
	::memmove( data, &data[sizeof(unsigned long)], ulSize);
	return (int)ulSize;
XEXCEPT_EXIT
}
int XRingBufferBL::get(unsigned char* data, unsigned long buff_size)
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XRingBufferBL::get"

	___XEXCEPT_CHECK_VALID_TRUE(data != 0);
	___XEXCEPT_CHECK_VALID_TRUE(buff_size != 0);

	unsigned long ulSize = 0;

	if( m_ulCurBlock == 0 ) {
		// 書き込まれたブロックがない
		return 0;
	}
	if( this->cur_blk_size() > buff_size ) {
		// 指定バッファサイズが少ない
		return 0;
	}

	if( m_buff.get((unsigned char *)&ulSize, sizeof(unsigned long)) != sizeof(unsigned long) ) {
		// データが壊れている
		m_buff.clear();
		m_ulCurBlock = 0;
		return 0;
	}
	if( ulSize > m_buff.size() ) {
		// データが壊れている
		m_buff.clear();
		m_ulCurBlock = 0;
		return 0;
	}
	if( m_buff.get(data, ulSize) != ulSize ) {
		// データが壊れている
		m_buff.clear();
		m_ulCurBlock = 0;
		return 0;
	}
	m_ulCurBlock--;
	return (int)ulSize;
XEXCEPT_EXIT
}
