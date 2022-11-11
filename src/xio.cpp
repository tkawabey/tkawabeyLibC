#include "xlib/xio.h"
#include "xlib/xeception.h"
#include <string.h>


/*
/////////////////////////////////////////////////////////////////////////////////////////
//
//	XIO
//
*/
XIO::XIO()
{

}
XIO::~XIO()
{

}
bool XIO::ReadLine(XStrBuff &buff, 
						  int code_type /*= 1*/, 
						  int flg/* = 0*/)
{
	static size_t		nIncreSize = 256;
	size_t				nBuffSize;
	size_t				nCurrentSize;
	unsigned char		c;
	unsigned char*		pBuff;

	// バッファの確保
	if( (nBuffSize=buff.GetBuffSize()) == 0 ) {
		nBuffSize = nIncreSize;
		pBuff = (unsigned char*)buff.Alloc(nBuffSize);
		if(pBuff == NULL) {
			return false;
		}
		buff.MemSet(0);
	} else {
		buff.MemSet(0);
		pBuff = buff;
	}
	nCurrentSize = 0;

	while(1) {
		if(this->Read((void*)&c, sizeof(unsigned char), flg) ) {
			if( c == (unsigned char)'\n' ) {
				if(nCurrentSize > 1) {
					if(*(pBuff-1) == '\r') {
						*(pBuff-1) = 0;
					}
				}
				*pBuff = 0;
#ifdef _UNICODE
				XBuff workBuff;
				workBuff.DoCopy( &buff );
				const char* p_tmp = workBuff;
				buff = p_tmp;
#endif
				return true;
			}
			*pBuff++ = c;
			nCurrentSize++;
			// バッファの再確保
			if( nCurrentSize >= nBuffSize ) {
				nBuffSize += nIncreSize;
				pBuff = (unsigned char*)buff.ReAlloc(nBuffSize);
				if(pBuff == NULL) {
					return false;
				}
				pBuff += nCurrentSize;
			}
		} else {
			*pBuff = 0;
#ifdef _UNICODE
			XBuff workBuff;
			workBuff.DoCopy( &buff );
			const char* p_tmp = workBuff;
			buff = p_tmp;
#endif

			return XStrBuff::StrLen(buff) ? true : false;
		}
	}
	return false;
}

bool XIO::Read(void* lpData, 
					  size_t nSize, 
					  int flg/* = 0*/)
{
	size_t			nIn;
	int				loop = 1;
	unsigned char*	pbuff = (unsigned char*)lpData;
	bool			bSuccess, bRet = false;

	while( loop ) {
		nIn = ReadRaw(pbuff, nSize, flg);
		bSuccess = nIn != -1;
		if(bSuccess) {
			if(nIn == 0) {
				loop = 0;
				bRet = false;
			}
			if(nIn < nSize) {
				nSize -= nIn;
				pbuff += nIn;
			} else {
				loop = 0;
				bRet = true;
			}
		} else {
			loop = 0;
			bRet = false;
		}
	}
	return bRet;
}
bool XIO::WriteString(const char* lpszData, int flg/* = 0*/)
{
	size_t nLen = ::strlen(lpszData);
	return Write((void*)lpszData, nLen);
}
bool XIO::WriteString(const wchar_t* lpszData, int flg/* = 0*/)
{
	size_t nLen = ::wcslen(lpszData) * sizeof(wchar_t);
	return Write((void*)lpszData, nLen);
}
bool XIO::Write(const void* lpData, size_t nSize, int flg/* = 0*/)
{
	size_t			nOut;
	int				loop = 1;
	unsigned char*	pbuff = (unsigned char*)lpData;
	bool			bSuccess, bRet = false;
	while( loop ) {
		nOut = WriteRaw(pbuff, nSize, flg);
		bSuccess = nOut != -1;
		if(bSuccess){
			if(nOut == 0) {
				loop = 0;
				bRet = false;
			}
			if(nOut < nSize) {
				nSize -= nOut;
				pbuff += nOut;
			} else {
				loop = 0;
				bRet = true;
			}
		} else {
			loop = 0;
			bRet = false;
		}
	}
	return bRet;
}


/*
/////////////////////////////////////////////////////////////////////////////////////////
//
//	XInputByteStream
//
*/
XInputByteStream::XInputByteStream ()
	:	m_nBitPosition (-1)
	,	m_pCurrentByte (0)
	,	m_pBufferLimit (0)
{
}
XUBYTE1 XInputByteStream::GetByte()
{
	_XASSERT(m_nBitPosition < 0);
	if (m_pCurrentByte >= m_pBufferLimit)
		FillBuffer () ;

	XBYTE1 result = *m_pCurrentByte ;
	++m_pCurrentByte ;
	return result ;
}
XUBYTE2 XInputByteStream::GetBigEndianWord ()
{
	_XASSERT (m_nBitPosition < 0);
	XUBYTE2 result ;
	Read (reinterpret_cast<char*>(&result), sizeof (result)) ;
	result = XBigEndianToSystem (result) ;
	return result ;
}
XUBYTE4 XInputByteStream::GetBigEndianLong () 
{
	_XASSERT (m_nBitPosition < 0);
	XUBYTE4 result ;
	Read (reinterpret_cast<char*>(&result), sizeof (result)) ;
	result = XBigEndianToSystem (result) ;
	return result ;
}
int  XInputByteStream::Read(char *buffer, int count)
{
	_XASSERT (m_nBitPosition < 0);
	int result ;
	size_t remaining = m_pBufferLimit - m_pCurrentByte ;
	if (count <= remaining) {
		memcpy (buffer, m_pCurrentByte, count) ;
		m_pCurrentByte += count ;
		result = count ;
	} else {
		result = remaining ;
		memcpy (buffer, m_pCurrentByte, remaining) ;
		buffer += remaining ;
		count -= remaining ;
		while (count > 0 && MoreData ())
		{
			FillBuffer () ;
			remaining = m_pBufferLimit - m_pCurrentByte ;
			if (count <= remaining) {
				memcpy (buffer, m_pCurrentByte, count) ;
				m_pCurrentByte += count ;
				result += count ;
				break ;
			} else {
				result += remaining ;
				memcpy (buffer, m_pCurrentByte, remaining) ;
				buffer += remaining ;
				count -= remaining ;
			}
		}
	}
	return result ;
}
bool XInputByteStream::MoreData ()
{
	bool result = (m_pCurrentByte < m_pBufferLimit) || ! EndReached () ;
	return result ;
}

void XInputByteStream::EnterBitMode (int initialposition)
{
	if(m_nBitPosition >= 0)
		return ;
	m_nBitPosition = initialposition ;
}


/*
/////////////////////////////////////////////////////////////////////////////////////////
//
//	XInputByteStream
//
*/
XOutputByteStream::XOutputByteStream()
	:	m_nBitPosition (-1)
{
}
void XOutputByteStream::Write(const char *buffer, int count)
{
	_XASSERT (m_nBitPosition == -1) ;

	while (count > 0) {
		if (count <= m_pBufferLimit - m_pCurrentByte) {
			::memcpy (m_pCurrentByte, buffer, count) ;
			m_pCurrentByte += count ;
			count = 0 ;
		} else {
			int outputcount = m_pBufferLimit - m_pCurrentByte ;
			::memcpy (m_pCurrentByte, buffer, outputcount) ;
			m_pBufferLimit += outputcount ;
			buffer += outputcount ;
			count -= outputcount ;
			FlushBuffer();
		}
	}
}

void XOutputByteStream::WriteByte(XUBYTE1 value)
{
	_XASSERT (m_nBitPosition == -1) ;
	if (m_pCurrentByte >= m_pBufferLimit) {
		FlushBuffer();
	}
	*m_pCurrentByte = value ;
	++ m_pCurrentByte ;
}
void XOutputByteStream::WriteBigEndianWord(XUBYTE2 value)
{
	_XASSERT (m_nBitPosition == -1) ;
	if (m_pCurrentByte + sizeof (value) >= m_pBufferLimit) {
		FlushBuffer();
	}

	*reinterpret_cast<XUBYTE2*>(m_pCurrentByte) = XSystemToBigEndian (value) ;
	m_pCurrentByte += sizeof (value) ;
	if (m_pCurrentByte >= m_pBufferLimit) {
		FlushBuffer();
	}
}
void XOutputByteStream::WriteBigEndianLong(XUBYTE4 value)
{
	_XASSERT (m_nBitPosition == -1) ;
	if (m_pCurrentByte + sizeof (value) >= m_pBufferLimit) {
		FlushBuffer();
	}

	*reinterpret_cast<XUBYTE4*>(m_pCurrentByte) = XSystemToBigEndian (value) ;
	m_pCurrentByte += sizeof (value) ;
	if (m_pCurrentByte >= m_pBufferLimit) {
		FlushBuffer();
	}
}
void XOutputByteStream::EnterBitMode(int initialposition)
{
	_XASSERT (m_nBitPosition == -1) ;
	if (m_pCurrentByte >= m_pBufferLimit) {
		FlushBuffer();
	}
	m_nBitPosition = initialposition ;
	*m_pCurrentByte = 0 ;
}
unsigned int XOutputByteStream::RemainingBufferSpace() const
{
	unsigned int result = m_pBufferLimit - m_pCurrentByte ;
	return result ;
}
void XOutputByteStream::NextByte()
{
	++m_pCurrentByte ;
	if (m_pCurrentByte >= m_pBufferLimit) {
		FlushBuffer();
	}
	*m_pCurrentByte = 0 ;
}

