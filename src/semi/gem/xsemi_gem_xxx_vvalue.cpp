#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/semi/gem/xsemi_gem_def.h"
#include "xlib/semi/gem/xsemi_gem_api.h"
#include <fstream>
#include <strstream>


//
//	WELLKNOWN_V_INDEXで指定したVIDを取得
//
XD_UINT4 XSEMI_Gem::getWellKnownVID(WELLKNOWN_V_INDEX index)
{
XEXCEPT_ENTRY
	if( index > V_INDEX_MAX ) {
		return XSEMI_INVALID_VID;
	}
	return m_ulVIDs[index];
XEXCEPT_EXIT
}

//
//	名前を指定して、VIDを取得
//
XD_UINT4 XSEMI_Gem::getVID_by_Name(const TCHAR* p_name)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	XD_UINT4 retVal;

	::memset(&retVal, 0xff, sizeof(retVal));

	XSEMI_V* pV = m_constData.get_Vs().get_byName( p_name );
	if( pV != NULL ) {
		retVal = pV->get_VID();
	}
	return retVal;
XEXCEPT_EXIT
}



//
//	指定したVの値を取得します。
//
XSEMI_Gem::GETV_STS XSEMI_Gem::getVValue(XD_UINT4 vid, XDATA_TYPE *type, std::ostream *stream)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);

	return this->innerGetVValue(0, vid, type, stream);
XEXCEPT_EXIT
}


XSEMI_Gem::GETV_STS XSEMI_Gem::getVValue(XD_UINT4 vid, XDATA_TYPE *type, XDataWriter& writer)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	return this->innerGetVValue(0, vid, type, writer);
XEXCEPT_EXIT
}

XSEMI_Gem::GETV_STS XSEMI_Gem::getVValue(XD_UINT4 vid, XDATA_TYPE *pType, void* pValue, XD_UINT4 nsize)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	XSEMI_V				*pV 
		
		= m_constData.get_Vs().get_byID( vid );	
	if( pV == NULL ) {
		return GETV_STS_NOT_FOUND_VID;
	}

	return innerGetVValue(0, pV, pType, pValue, nsize);
XEXCEPT_EXIT
}

XSEMI_Gem::GETV_STS XSEMI_Gem::getVValue(XD_UINT4 vid, XRichData& rData)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	XDATA_TYPE			type;
	XInflateStreamBuff	buff;
	std::ostream		outp( &buff );
	XDataWriter			writer( &outp );
	GETV_STS			sts = GETV_STS_ERROR;


	sts = this->innerGetVValue(0, vid, &type, writer);
	if( sts != GETV_STS_SUCCESS ) {
		return sts;
	}

	XData data;
	XDataReader reader(buff.getBuff(), buff.size());
	if( reader.getValue( &data ) == false ) {
		return GETV_STS_ERROR;
	}

	rData.putValueFromXData(reader, data, true);

	return GETV_STS_SUCCESS;
//	return rData.getDataCount();
XEXCEPT_EXIT
}

//
//	Vの値を更新する。
//
XSEMI_Gem::PUTV_STS XSEMI_Gem::putVValue(XD_UINT4 vid, const void* pValue, XD_UINT4 nsize)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	XSEMI_V				*pV ;

	if( (pV =m_constData.get_Vs().get_byID( vid )) == NULL ) {
		return PUTV_STS_NOT_FOUND_VID;
	}
	if(pV->get_GetEx() == true ) {
		return externPutVValue(vid, pV->getType(), pValue, nsize);
	} else {
		switch( pV->getType() ) {
		case XDATA_TYPE_A:
		case XDATA_TYPE_J:
			pV->putValueString((const char*)pValue, nsize);
			break;
		case XDATA_TYPE_UTF16:
			pV->putValueStringUTF16((const wchar_t*)pValue, nsize/sizeof(wchar_t));
			break;
		default:
			pV->putValue((XD_INT1*)pValue, nsize, true /* HostOrder　*/);
			break;
		}

		m_constData.updateMaster( pV );


		return PUTV_STS_SUCCESS;
	}
XEXCEPT_EXIT
}

XSEMI_Gem::PUTV_STS XSEMI_Gem::putVValue(XD_UINT4 vid, XRichData& rData)
{
XEXCEPT_ENTRY
	XMutexer			locker(m_lock);
	XSEMI_V				*pV ;



	// 指定されたVIDを探す。
	if( (pV =m_constData.get_Vs().get_byID( vid )) == NULL ) {
		return PUTV_STS_NOT_FOUND_VID;
	}
	if( pV->getType() != rData.getType() ) {
		// 型が異なる。
		return GETV_STS_DIFF_DATA_TYPE;
	}

	if(pV->get_GetEx() == true ) {
		XInflateStreamBuff	buffTmp;
		std::ostream		streamTmp( &buffTmp );
		if( rData.getType() == XDATA_TYPE_L ) {
			if( rData.buildBinaryData( &streamTmp ) == false ) {
				return PUTV_STS_ERROR;
			}
		} else {
			rData.getValue( &streamTmp );
		}

		return externPutVValue(
			vid, 
			pV->getType(), 
			buffTmp.getBuff(), 
			buffTmp.size() );
	} else {
		if( pV->copy( &rData )  == false ) {
			return PUTV_STS_ERROR;
		}
		m_constData.updateMaster( pV );
	}
	return PUTV_STS_SUCCESS;
XEXCEPT_EXIT
}




XSEMI_Gem::GETV_STS XSEMI_Gem::externGetVValue(const XD_UINT4 ceid, const XD_UINT4 vid, const XDATA_TYPE type, std::ostream *stream)
{
XEXCEPT_ENTRY
	if( type == XDATA_TYPE_L ) {
		XDataWriter writer( stream );
		writer.putValAsList( 0 );
	}
	return GETV_STS_NO_IMPLE;
XEXCEPT_EXIT
}

XSEMI_Gem::PUTV_STS XSEMI_Gem::externPutVValue(const XD_UINT4 vid, const XDATA_TYPE type, const void* pValue, XD_UINT4 ulValueSize)
{
XEXCEPT_ENTRY
	return PUTV_STS_NO_IMPLE;
XEXCEPT_EXIT
}



XSEMI_Gem::GETV_STS XSEMI_Gem::innerGetVValue(XD_UINT4 ceid, XD_UINT4 vid, XDATA_TYPE *type, std::ostream *stream)
{
XEXCEPT_ENTRY

	XSEMI_V *pV = m_constData.get_Vs().get_byID( vid );	
	if( pV == NULL ) {
		return GETV_STS_NOT_FOUND_VID;
	}
	if( type != NULL ) {
		*type = pV->getType();
	}	
	if(pV->get_GetEx() == true ) {
		*type = pV->getType();
		return externGetVValue(ceid, vid, pV->getType(), stream);
	} else {
		if( pV->getType() == XDATA_TYPE_L ) {
			pV->buildBinaryData( stream );
		} else {
			pV->getValue(stream);
		}
		return GETV_STS_SUCCESS;
	}
XEXCEPT_EXIT
}
XSEMI_Gem::GETV_STS XSEMI_Gem::innerGetVValue(XD_UINT4 ceid, XD_UINT4 vid, XDATA_TYPE *type, XDataWriter& writer)
{
XEXCEPT_ENTRY
	XD_INT1				*pU1Value = NULL;
	XD_UINT4			nValueCount = 0;
	GETV_STS			sts = GETV_STS_ERROR;

	XSEMI_V *pV = m_constData.get_Vs().get_byID( vid );	
	if( pV == NULL ) {
		// 指定されたVIDが見つからない。
		return GETV_STS_NOT_FOUND_VID;
	}
	if( type != NULL ) {
		// Type取得設定がある場合。
		*type = pV->getType();
	}
	if(pV->get_GetEx() == true ) {
		if( pV->getType() == XDATA_TYPE_L ) {
			return externGetVValue(ceid, vid, pV->getType(), writer.get_stream());
		} else {
			XInflateStreamBuff	buffTmp;
			std::ostream		streamTmp( &buffTmp );

			sts = externGetVValue(ceid, vid, pV->getType(), &streamTmp);
			if( sts != GETV_STS_SUCCESS ) {
				return sts;
			}

			int count = buffTmp.size();
			switch( pV->getType() ) {
			case XDATA_TYPE_U2:
			case XDATA_TYPE_I2:
				count /= sizeof(XD_INT2);
				break;
			case XDATA_TYPE_U4:
			case XDATA_TYPE_I4:
			case XDATA_TYPE_F4:
				count /= sizeof(XD_INT4);
				break;
			case XDATA_TYPE_U8:
			case XDATA_TYPE_I8:
			case XDATA_TYPE_F8:
				count /= sizeof(XD_INT4);
				break;
			}
			writer.putValue(pV->getType(), buffTmp.getBuff(), count);
		}
		return GETV_STS_SUCCESS;
	} else {
		if( pV->getType() == XDATA_TYPE_L ) {
			pV->buildBinaryData( writer.get_stream() );
		} else {
			pV->getValue(&pU1Value, nValueCount);
			writer.putValue(pV->getType(), pU1Value, nValueCount);
		}
		return GETV_STS_SUCCESS;
	}
XEXCEPT_EXIT
}

XSEMI_Gem::GETV_STS  XSEMI_Gem::innerGetVValue(XD_UINT4 ceid, XSEMI_V* pV, XDATA_TYPE *pType, void* pValue, XD_UINT4 nsize)
{
XEXCEPT_ENTRY
	XD_INT1				*pU1Value = NULL;
	XD_UINT4			nValueCount = 0;
	std::ostrstream		stream((char*)pValue, nsize);
	XDATA_TYPE			type = pV->getType();
	GETV_STS			sts = GETV_STS_ERROR;

	if( pType != NULL ) {
		// Type取得設定がある場合。
		*pType = type;
	}
	if(pV->get_GetEx() == true ) {
		return externGetVValue(ceid, pV->get_VID(), type, &stream);
	} else {
		return innerGetVValue(ceid,  pV->get_VID(), &type, &stream);
	}
XEXCEPT_EXIT
}





