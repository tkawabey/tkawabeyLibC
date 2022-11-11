#include "xlib/xeception.h"
#include "xlib/semi/secs/xsecs.h"



XSecsData::XSecsData()
	:	m_datas(XDATA_TYPE_L)
{
XEXCEPT_ENTRY

	::memset(&m_header, 0, sizeof(m_header));

XEXCEPT_EXIT
}

XSecsData::XSecsData(XD_UINT1 stream, XD_UINT1 function, bool bWait)
	:	m_datas(XDATA_TYPE_L)
{
XEXCEPT_ENTRY

	::memset(&m_header, 0, sizeof(m_header));
	setSxFx(m_header, stream, function, bWait);

XEXCEPT_EXIT
}

XSecsData::~XSecsData()
{
XEXCEPT_ENTRY
//	std::vector<XRichData*>::iterator ite;
//	for(ite = m_datas.begin(); ite != m_datas.end(); ite++) {
//		XRichData* pData = (*ite);
//		delete pData;
//	}
	m_datas.clear();
XEXCEPT_EXIT
}

void XSecsData::clear()
{
XEXCEPT_ENTRY
//	std::vector<XRichData*>::iterator ite;
//	for(ite = m_datas.begin(); ite != m_datas.end(); ite++) {
//		XRichData* pData = (*ite);
//		delete pData;
//	}
	m_datas.clear();

	::memset(&m_header, 0, sizeof(m_header));
XEXCEPT_EXIT
}

XRichData* XSecsData::addChiled(XDATA_TYPE type)
{
XEXCEPT_ENTRY
/*
	XRichData *pValue = new XRichData(type);
	if( pValue == NULL ) {
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);	}
		return NULL;
	}
	m_datas.push_back(pValue);
*/
	return m_datas.addChiled(type);
XEXCEPT_EXIT
}

XRichData* XSecsData::getChiled(XD_UINT4 index)
{
XEXCEPT_ENTRY
/*
	if( getChiledCount() <= index ) {
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, XD2_ERRIT);	}
		return NULL;
	}
	return m_datas[index];
*/
	return m_datas.getChiled(index);
XEXCEPT_EXIT
}

bool XSecsData::atachChild(XRichData* pItem, XRichData* pPreItem /*= NULL */)
{
XEXCEPT_ENTRY
	return m_datas.atachChild(pItem, pPreItem);
XEXCEPT_EXIT
}

bool XSecsData::detachChild(XRichData* pItem)
{
XEXCEPT_ENTRY
	return m_datas.detachChild( pItem );
XEXCEPT_EXIT
}

bool XSecsData::removeChiled(XRichData* pData)
{
XEXCEPT_ENTRY
/*
	std::vector<XRichData*>::iterator ite;
	for(ite = m_datas.begin(); ite != m_datas.end(); ite++) {
		if( (*ite) == pData ) {
			m_datas.erase(ite);
			delete pData;
			return true;
		}
	}
	if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);	}
	return false;
*/
	return m_datas.removeChiled(pData);
XEXCEPT_EXIT
}
XD_UINT4 XSecsData::getChiledCount()
{
XEXCEPT_ENTRY
	return m_datas.getChiledCount();
XEXCEPT_EXIT
}

void XSecsData::setResponseHeader( XSECSHeader *pHeader )
{
XEXCEPT_ENTRY
	XD_UINT4 us4Value = 0;
	XD_UINT2 us2Value = 0;	

	::memcpy(&m_header, pHeader, sizeof(XSECSHeader));
	m_header.status++;
	this->put_wait(false);
XEXCEPT_EXIT
}

bool XSecsData::buildBinaryData(std::ostream &stream, BIN_MODE mode)
{
XEXCEPT_ENTRY
	XD_UINT4 ulDataSize = 0;
	XD_UINT2 us2Value = 0;	
	XD_UINT4 us4Value = 0;	
	std::ostream::pos_type firstpos = stream.tellp();
	std::ostream::pos_type endpos;
	
	switch( mode ) {
	case SIZE_HEAD_AND_BODY:
		// write data size
		stream.write((char*)&ulDataSize, sizeof(XD_UINT4));
	case HEAD_AND_BODY:
		// write SECS header
		us2Value = XDataCoder::_hton2( m_header.sessionID );
		stream.write((char*)&us2Value, sizeof(m_header.sessionID));
		stream.write((char*)&m_header.byte2, sizeof(m_header.byte2));
		stream.write((char*)&m_header.status, sizeof(m_header.status));
		stream.write((char*)&m_header.ptype, sizeof(m_header.ptype));
		stream.write((char*)&m_header.stype, sizeof(m_header.stype));
		::memcpy(&us2Value, m_header.src_id, sizeof(us2Value));
		us2Value = XDataCoder::_hton2( us2Value );
		stream.write((char*)&us2Value, sizeof(us2Value));
		::memcpy(&us2Value, m_header.tr_id, sizeof(us2Value));
		us2Value = XDataCoder::_hton2( us2Value );
		stream.write((char*)&us2Value, sizeof(us2Value));
		break;
	}

	// write datas
	for(XD_UINT4 i = 0; i < m_datas.getChiledCount(); i++) {
		if( m_datas.getChiled(i)->buildBinaryData(&stream) == false ) { 
			return false;
		}
	}

	if( mode == SIZE_HEAD_AND_BODY ) {
		// over write data size
		
		endpos =  stream.tellp();
		// calcurate secs datasize( header + body)
		if( (std::streamoff)firstpos == -1 ) {
			ulDataSize = XDataCoder::_hton4( ((std::streamoff)endpos) - sizeof(XD_UINT4) );
			stream.seekp(0, std::ios::beg);
		} else {
			ulDataSize = XDataCoder::_hton4( ((std::streamoff)endpos - (std::streamoff)firstpos) - sizeof(XD_UINT4) );
			stream.seekp(firstpos);
		}
		stream.write((char*)&ulDataSize, sizeof(XD_UINT4));
		stream.seekp(endpos);
	}
	return true;
XEXCEPT_EXIT
}
bool XSecsData::setFromBinary(XBIN* pRawData, XD_UINT4 len, BIN_MODE mode)
{
XEXCEPT_ENTRY
	XD_UINT4	ulData;
	XD_UINT2	usData;

	switch( mode ) {
	case SIZE_HEAD_AND_BODY:
		if( len < 14 ) {
			m_error = XD2_ERRSS;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
			return false;
		}
		::memcpy(&ulData, pRawData, sizeof(XD_UINT4));
		pRawData += sizeof(XD_UINT4);
		ulData = XDataCoder::_ntoh4(ulData);

		len -= 4;
	case HEAD_AND_BODY:
		if( len < 10 ) {
			m_error = XD2_ERRSS;
			if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
			return false;
		}
		::memcpy(&m_header.sessionID, pRawData, sizeof(m_header.sessionID));
		m_header.sessionID = XDataCoder::_ntoh2(m_header.sessionID);
		pRawData += sizeof(m_header.sessionID);

		::memcpy(&m_header.byte2, pRawData, sizeof(m_header.byte2));
		pRawData += sizeof(m_header.byte2);

		::memcpy(&m_header.status, pRawData, sizeof(m_header.status));
		pRawData += sizeof(m_header.status);

		::memcpy(&m_header.ptype, pRawData, sizeof(m_header.ptype));
		pRawData += sizeof(m_header.ptype);

		::memcpy(&m_header.stype, pRawData, sizeof(m_header.stype));
		pRawData += sizeof(m_header.stype);

		::memcpy(&usData, pRawData, sizeof(m_header.src_id));
		usData = XDataCoder::_ntoh2(usData);
		::memcpy(m_header.src_id, &usData, sizeof(usData));
		pRawData += sizeof(m_header.src_id);

		::memcpy(&usData, pRawData, sizeof(m_header.tr_id));
		usData = XDataCoder::_ntoh2(usData);
		::memcpy(m_header.tr_id, &usData, sizeof(usData));
		pRawData += sizeof(m_header.src_id);


		len -= 10;
		break;
	}

	XDataReader reader(pRawData, len);
	XData data;

	while( reader.getValue(&data) != false ) {
		XRichData *pItem = this->addChiled( data.getType() );
		if( pItem == NULL ) {
			return false;
		}
		if( this->innerSetFromBinary(reader, data, pItem) == false ) {
			return false;
		}
	}


	return true;
XEXCEPT_EXIT
}
bool XSecsData::innerSetFromBinary(XDataReader &reader, XData &data, XRichData* pItem)
{
XEXCEPT_ENTRY

	switch( data.getType() ) {
	case XDATA_TYPE_L:
		{
			XData dataChiled;
			for( XD_UINT4 i = 0; i < data.getCount(); i++) {
				if( reader.getValue(&dataChiled) == false ) {
					return false;
				}
				XRichData *pChiledItem = pItem->addChiled(dataChiled.getType()); 
				if( pChiledItem == NULL ) {
					return false;
				}
				if( this->innerSetFromBinary(reader, dataChiled, pChiledItem) == false ) {
					return false;
				}
			}
		}
		break;
	case XDATA_TYPE_A:
	case XDATA_TYPE_J:
		{
			char *p = NULL;
			XD_UINT4     size = data.getValAsBinaryRef((unsigned char **)&p);
			if( pItem->putValueString(p, size) == false ) {
				return false;
			}
		}
		break;
	case XDATA_TYPE_UTF16:
		{
			wchar_t	*p = NULL;
			XBuff	buff;

			if( data.getCount() != 0 ) {
				p = (wchar_t*)buff.Alloc( (data.getCount() + 1) * sizeof(wchar_t) );
				if( p == NULL ) {
					m_error = XD2_ERRMA;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
					return false;
				}
				buff.MemSet();
				for( XD_UINT4 i = 0; i < data.getCount(); i++ ) {
					p[i] = data.getValAsUInt2(i);
				}
				if( pItem->putValueStringUTF16(p) == false ) {
					return false;
				}
			} else {
				if( pItem->putValueStringUTF16(L"") == false ) {
					return false;
				}
			}
		}
		break;
	case XDATA_TYPE_B:
	case XDATA_TYPE_BOOL:
	case XDATA_TYPE_I1:
	case XDATA_TYPE_U1:
		{
			XBIN *p = NULL;
			XD_UINT4     size = data.getValAsBinaryRef(&p);
			if( pItem->putValue(p, size) == false ) {
				return false;
			}
		}
		break;
	case XDATA_TYPE_I2:
	case XDATA_TYPE_U2:
		{
			XD_UINT2	*p = NULL;
			XBuff	buff;

			if( data.getCount() != 0 ) {
				p = (XD_UINT2*)buff.Alloc( (data.getCount() ) * sizeof(XD_UINT2) );
				if( p == NULL ) {
					m_error = XD2_ERRMA;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
					return false;
				}
				buff.MemSet();
				for( XD_UINT4 i = 0; i < data.getCount(); i++ ) {
					p[i] = data.getValAsUInt2(i);
				}
				if( pItem->putValue(p, data.getCount()) == false ) {
					return false;
				}
			}
		}
		break;
	case XDATA_TYPE_I4:
	case XDATA_TYPE_U4:
	case XDATA_TYPE_F4:
		{
			XD_UINT4	*p = NULL;
			XBuff	buff;

			if( data.getCount() != 0 ) {
				p = (XD_UINT4*)buff.Alloc( (data.getCount() ) * sizeof(XD_UINT4) );
				if( p == NULL ) {
					m_error = XD2_ERRMA;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
					return false;
				}
				buff.MemSet();
				for( XD_UINT4 i = 0; i < data.getCount(); i++ ) {
					p[i] = data.getValAsUInt4(i);
				}
				if( pItem->putValue(p, data.getCount()) == false ) {
					return false;
				}
			}
		}
		break;
	case XDATA_TYPE_I8:
	case XDATA_TYPE_U8:
	case XDATA_TYPE_F8:
		{
			XD_UINT8	*p = NULL;
			XBuff	buff;

			if( data.getCount() != 0 ) {
				p = (XD_UINT8*)buff.Alloc( (data.getCount() ) * sizeof(XD_UINT8) );
				if( p == NULL ) {
					m_error = XD2_ERRMA;
					if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, m_error);	}
					return false;
				}
				buff.MemSet();
				for( XD_UINT4 i = 0; i < data.getCount(); i++ ) {
					p[i] = data.getValAsUInt8(i);
				}
				if( pItem->putValue(p, data.getCount()) == false ) {
					return false;
				}
			}
		}
		break;
	default:
		m_error = XD2_ERRTY;
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_XDATA, 0, m_error);	}
		return false;
		break;
	}

	// レングスバイト
	if( data.isFitLengthByte() == false ) {
		pItem->setLengthByte( data.getLengthByte() );
	}
	return true;
XEXCEPT_EXIT
}
bool XSecsData::print(XLIB_STD_OUT_STR_STREAM &stream,  XD_INT4 nDepth /*= 0*/, const TCHAR *pLF /*= "\n"*/)
{
XEXCEPT_ENTRY
	XStrBuff brank;
	XD_INT4 ii = 0;
	TCHAR *pBlak = NULL;

	pBlak = (TCHAR*)brank.Alloc( (nDepth+2) * sizeof(TCHAR) );
	if( pBlak == NULL ) {
		if( XData::getErrorRise() == true ) {	XEXCEPT_THROW(XExcept_MEMORY_ALLOC, 0, 0);	}
		return false;
	}	
#ifdef _UNICODE
	for(ii = 0; ii < nDepth; ii++ ) {
		pBlak[ii] = _T(' ');
	}
#else
	brank.MemSet(' ');
#endif
	pBlak[nDepth] = 0;

	stream << (const TCHAR*)brank << _T("<S") << (int)this->get_stream() << _T("F") << (int)this->get_function();
	if( this->get_wait() ) {
		stream << _T(" W");
	}
	stream << pLF;

	// write datas
	for(XD_UINT4 i = 0; i < m_datas.getChiledCount(); i++) {
		if( m_datas.getChiled(i)->print(stream, nDepth + 1, pLF) == false ) { 
			return false;
		}
	}
	stream << (const TCHAR*)brank << _T(">");
	stream << pLF;
	return true;
XEXCEPT_EXIT
}

XD_UINT2 XSecsData::get_session()
{
XEXCEPT_ENTRY
	return m_header.sessionID;
XEXCEPT_EXIT
}
void XSecsData::put_session(XD_UINT2 val)
{
XEXCEPT_ENTRY
	m_header.sessionID = val;
XEXCEPT_EXIT
}

XD_UINT1 XSecsData::get_stream()
{
XEXCEPT_ENTRY

	XD_UINT1 val = (m_header.byte2 & 0x80 ) ? m_header.byte2 - 0x80 : m_header.byte2;
	return val;

XEXCEPT_EXIT
}
void XSecsData::put_stream(XD_UINT1 val)
{
XEXCEPT_ENTRY
	bool bW = m_header.byte2 & 0x80 ? true : false;
	m_header.byte2 = val;
	if( bW == true ) {
		m_header.byte2 |= 0x80;
	}
XEXCEPT_EXIT
}
XD_UINT1 XSecsData::get_function()
{
XEXCEPT_ENTRY
	return m_header.status;
XEXCEPT_EXIT
}
void XSecsData::put_function(XD_UINT1 val)
{
XEXCEPT_ENTRY
	m_header.status = val;
XEXCEPT_EXIT
}
bool XSecsData::get_wait()
{
XEXCEPT_ENTRY
	return (m_header.byte2 & 0x80 ) ? true : false;
XEXCEPT_EXIT
}
void XSecsData::put_wait(bool val)
{
XEXCEPT_ENTRY
	if( val == true ) {
		m_header.byte2 |= 0x80;
	} else {
		m_header.byte2 &= ~0x80;
	}
XEXCEPT_EXIT
}
XD_UINT2 XSecsData::get_ticket()
{
XEXCEPT_ENTRY
	XD_UINT2 val;
	::memcpy(&val, m_header.tr_id, sizeof(val));
	return val;
XEXCEPT_EXIT
}
void XSecsData::put_ticket(XD_UINT2 val)
{
XEXCEPT_ENTRY
	::memcpy(m_header.tr_id, &val, sizeof(val));
XEXCEPT_EXIT
}
XSECSHeader& XSecsData::get_header()
{
XEXCEPT_ENTRY
	return m_header;
XEXCEPT_EXIT
}

// <-- Static
bool XSecsData::get_rbit(XSECSHeader& head)
{
XEXCEPT_ENTRY
	return (head.sessionID & 0x8000 ) ? true : false;
XEXCEPT_EXIT

}
void XSecsData::set_rbit(XSECSHeader& head, bool val)
{
XEXCEPT_ENTRY
	head.sessionID |= 0x8000;
XEXCEPT_EXIT
}
bool XSecsData::get_ebit(XSECSHeader& head)
{
XEXCEPT_ENTRY
	return (head.ptype & 0x80 ) ? true : false;
XEXCEPT_EXIT
}
void XSecsData::set_ebit(XSECSHeader& head, bool val)
{
XEXCEPT_ENTRY
	head.ptype |= 0x80;
XEXCEPT_EXIT
}
bool XSecsData::get_wait(XSECSHeader& head)
{
XEXCEPT_ENTRY
	return (head.byte2 & 0x80 ) ? true : false;
XEXCEPT_EXIT
}
void XSecsData::set_wait(XSECSHeader& head, bool val)
{
XEXCEPT_ENTRY
	head.byte2 |= 0x80;
XEXCEPT_EXIT
}

XD_UINT2 XSecsData::get_deviceID(XSECSHeader& head)
{
XEXCEPT_ENTRY
	XD_UINT2 val = (head.sessionID & 0x8000 ) ? head.sessionID - 0x8000 : head.sessionID;
	return val;
XEXCEPT_EXIT
}
void XSecsData::set_deviceID(XSECSHeader& head, XD_UINT2 val)
{
XEXCEPT_ENTRY
	bool bR = get_rbit(head);
	head.sessionID = val;
	if( bR == true ) {
		head.sessionID  = 0x8000 ;
	}
XEXCEPT_EXIT
}



XD_UINT1 XSecsData::get_stream(XSECSHeader& head)
{
XEXCEPT_ENTRY
	XD_UINT1 val = (head.byte2 & 0x80 ) ? head.byte2 - 0x80 : head.byte2;
	return val;
XEXCEPT_EXIT
}
void XSecsData::set_stream(XSECSHeader& head, XD_UINT1 val)
{
XEXCEPT_ENTRY
	bool bW = get_wait(head);
	head.byte2 = val;
	if( bW == true ) {
		head.byte2  |= 0x80 ;
	}
XEXCEPT_EXIT
}


XD_UINT1 XSecsData::get_function(XSECSHeader& head)
{
XEXCEPT_ENTRY
	return head.status;
XEXCEPT_EXIT
}
void XSecsData::set_function(XSECSHeader& head, XD_UINT1 val)
{
XEXCEPT_ENTRY
	head.status = val;
XEXCEPT_EXIT
}


XD_UINT2 XSecsData::get_blockNo(XSECSHeader& head)
{
XEXCEPT_ENTRY
	XD_UINT2 ulBlockNo = (head.ptype & ~0x80);
	ulBlockNo <<= 4;
	ulBlockNo |= head.stype;
	return ulBlockNo;
XEXCEPT_EXIT
}
void XSecsData::set_blockNo(XSECSHeader& head, XD_UINT2 val)
{
XEXCEPT_ENTRY
	bool bE = get_ebit(head);
	
	head.ptype = ( (val & 0xFF00) >> 4 );
	head.stype = val & 0x00FF;
	if( bE == true ) {
		head.ptype  = 0x80 ;
	}
XEXCEPT_EXIT
}


XD_UINT2 XSecsData::get_source_id(XSECSHeader& head)
{
XEXCEPT_ENTRY
	XD_UINT2 val;
	::memcpy(&val, head.src_id, sizeof(val));
	return val;
XEXCEPT_EXIT
}
void XSecsData::set_source_id(XSECSHeader& head, XD_UINT2 val)
{
XEXCEPT_ENTRY
	::memcpy(head.src_id, &val, sizeof(val));
XEXCEPT_EXIT
}

XD_UINT2 XSecsData::get_ticket(XSECSHeader& head)
{
XEXCEPT_ENTRY
	XD_UINT2 val;
	::memcpy(&val, head.tr_id, sizeof(val));
	return val;
XEXCEPT_EXIT
}
void XSecsData::set_ticket(XSECSHeader& head, XD_UINT2 val)
{
XEXCEPT_ENTRY
	::memcpy(head.tr_id, &val, sizeof(val));
XEXCEPT_EXIT
}


void XSecsData::setSxFx(XSECSHeader& head, XD_UINT1 stream, XD_UINT1 function, bool bwait, XD_UINT2 thiket /*= 0*/)
{
XEXCEPT_ENTRY
	::memset(&head, 0, sizeof(XSECSHeader));
	head.byte2 = stream;
	if( bwait == true ) {
		head.byte2 |= 0x80;
	} else {
		head.byte2 &= ~0x80;
	}
	head.status = function;
	::memcpy(head.tr_id, &thiket, sizeof(thiket));
XEXCEPT_EXIT
}
void XSecsData::generateHostCode(XSECSHeader& head)
{
XEXCEPT_ENTRY
	XD_UINT2 val2;

	::memcpy(&val2, &head.sessionID, 2);
	val2 = ::ntohs(val2);
	::memcpy(&head.sessionID, &val2, 2);

	::memcpy(&val2, head.src_id, 2);
	val2 = ::ntohs(val2);
	::memcpy(head.src_id, &val2, 2);

	::memcpy(&val2, head.tr_id, 2);
	val2 = ::ntohs(val2);
	::memcpy(head.tr_id, &val2, 2);

XEXCEPT_EXIT
}

void XSecsData::generateNetWorkCode(XSECSHeader& head)
{
XEXCEPT_ENTRY
	XD_UINT2 val2;

	::memcpy(&val2, &head.sessionID, 2);
	val2 = ::htons(val2);
	::memcpy(&head.sessionID, &val2, 2);



	::memcpy(&val2, head.src_id, 2);
	val2 = ::htons(val2);
	::memcpy(head.src_id, &val2, 2);


	::memcpy(&val2, head.tr_id, 2);
	val2 = ::htons(val2);
	::memcpy(head.tr_id, &val2, 2);

XEXCEPT_EXIT
}



// -->

XSecsHSMSLog::XSecsHSMSLog()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}
XSecsHSMSLog::~XSecsHSMSLog()
{
XEXCEPT_ENTRY
XEXCEPT_EXIT
}


