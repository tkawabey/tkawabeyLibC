#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/xfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

int XSEMI_Gem::processGemMessageS5(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XData data;
	XDataWriter	writer( &pOutStream );
	switch( function ) {
	case 1:
		return processGemMessageS5F1(inData, pOutStream);
		break;
	case 3:
		return processGemMessageS5F3(inData, pOutStream);
		break;
	case 5:
		return processGemMessageS5F5(inData, pOutStream);
		break;
	case 7:
		return processGemMessageS5F7(inData, pOutStream);
		break;
	}
	return XSEMI_GEM_ERR_HEAD_FUNCTION;
XEXCEPT_EXIT
}



XD_UINT4 XSEMI_Gem::innerSyncroAlarm(XSECSHeader& header, unsigned char* pMsgData,XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
	XMutexer					locker(m_lock);
	XDataReader reader(pMsgData+10, ulMsgSize-10);
	XData						data;
	XD_UINT4					loop, i;
	XD_UINT1					alcd;
	XD_UINT4					alid;
	XStrBuff					strTxt;

	// **IDのフォーマットを取得
	XDATA_TYPE					alidFormatType    = XDATA_TYPE_U4;

	if( get_XXID_Format(EC_INDEX_FORMAT_ALID,   &alidFormatType) == false  ) {
		// フォーマットが不正
		return XSEMI_GEM_ERR_ABORT;
	}

	/*
		構造： L,m
				1. L,3
					1. <B[1] ALCD1> * アラーム・コード
					2. <xx ALID1> * アラームID
					3. <A[1-40] ALTX1> * アラーム・テキスト
				:
				m. L,3
					1. <B[1] ALCDm>
					2. <xx ALIDm>
					3. <A[1-40] ALTXm>
		
		  ALCD：	bit8=1：アラーム状態発生
					bit8=0：アラーム状態解除	
	*/
	// L,3
	if( reader.getValue( &data ) == false ) {
		return XSEMI_GEM_ERR_DATA;
	}
	loop = data.getCount();
	for( i = 0; i < loop; i++) {
		// L,3
		if( reader.getValue( &data ) == false ) {
			return XSEMI_GEM_ERR_DATA;
		}
		if( data.getType() != XDATA_TYPE_L || data.getCount() != 3) {
			return XSEMI_GEM_ERR_DATA;
		}
		// B	ALCD
		if( reader.getValue( &data ) == false ) {
			return XSEMI_GEM_ERR_DATA;
		}
		if( data.getType() != XDATA_TYPE_B ) {
			return XSEMI_GEM_ERR_DATA;
		}
		alcd = data.getValAsUInt1();
		// U4	ALID
		if( reader.getValue( &data ) == false ) {
			return XSEMI_GEM_ERR_DATA;
		}
		if( data.getType() != alidFormatType ) {
			return XSEMI_GEM_ERR_DATA;
		}
		XSEMI_ALARM* pAlarm = NULL;
		if( data.getCount() != 1 ) {
			return XSEMI_GEM_ERR_IN_DATA;
		}
		alid = data.getCastValAsUInt4();
		pAlarm = m_constData.get_Alarms().get_byID( alid );
		// A	ALTX
		if( reader.getValue( &data ) == false ) {
			return XSEMI_GEM_ERR_DATA;
		}
		if( data.getType() != XDATA_TYPE_A) {
			return XSEMI_GEM_ERR_DATA;
		}
		strTxt = data.getValAsStringA();


		bool bChange = false;
		// Constの値を書き換える
		if( pAlarm != NULL ) {
			if( pAlarm->get_ALCD() != alcd ) {
				pAlarm->set_ALCD(alcd);
				bChange = true;
			}
			pAlarm->set_Text(strTxt);
		} else {
			bChange = true;
			if( m_pCommuIf != NULL ) {
				XStrBuff strErr;
				strErr.Format(_T("ALID (%u) ALTXT (%s).not defined"), alid, strTxt.c_str());
				writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr);
			}
		}

		// コールバックをコール
		if( bChange == true ) {
			occurreAlarm(alid, alcd, strTxt);
		}
	}
XEXCEPT_EXIT
	return XSEMI_GEM_ERR_NON_TRANSACT_DEF;
}



int XSEMI_Gem::processGemMessageS5F1(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XMutexer					locker(m_lock);
	XD_UINT1					ack5 = 0;
	unsigned char*				pRawData = NULL;
	int							iRawSize = 0;
	XData						data;
	XD_UINT1					alcd;
	XD_UINT4					alid;
	XStrBuff					strTxt;

	// **IDのフォーマットを取得
	XDATA_TYPE					alidFormatType    = XDATA_TYPE_U4;

	if( get_XXID_Format(EC_INDEX_FORMAT_ALID,   &alidFormatType) == false  ) {
		// フォーマットが不正
		return XSEMI_GEM_ERR_ABORT;
	}
	
	inData.getRaw(&pRawData, &iRawSize);

	XDataReader readerChk(pRawData, iRawSize);
	XDataWriter	writer( &pOutStream );

	/*
		構造： L,3
				1. <B[1] ALCD> * アラーム・コード
				2. <xx ALID> * アラームID
				3. <A[1-40] ALTX> * アラーム・テキスト
	*/
	// L,3
	if( readerChk.getValue( &data ) == false ) {
		return XSEMI_GEM_ERR_DATA;
	}
	if( data.getType() != XDATA_TYPE_L || data.getCount() != 3) {
		return XSEMI_GEM_ERR_DATA;
	}
	// B	ALCD
	if( readerChk.getValue( &data ) == false ) {
		return XSEMI_GEM_ERR_DATA;
	}
	if( data.getType() != XDATA_TYPE_B ) {
		return XSEMI_GEM_ERR_DATA;
	}
	alcd = data.getValAsUInt1();
	// U4	ALID
	if( readerChk.getValue( &data ) == false ) {
		return XSEMI_GEM_ERR_DATA;
	}
	if( data.getType() != alidFormatType ) {
		return XSEMI_GEM_ERR_DATA;
	}
	if( data.getCount() != 1 ) {
		return XSEMI_GEM_ERR_IN_DATA;
	}
	XSEMI_ALARM* pAlarm = NULL;
	alid = data.getCastValAsUInt4();
	pAlarm = m_constData.get_Alarms().get_byID( alid );



	// A	ALTX
	if( readerChk.getValue( &data ) == false ) {
		return XSEMI_GEM_ERR_DATA;
	}
	if( data.getType() != XDATA_TYPE_A) {
		return XSEMI_GEM_ERR_DATA;
	}
	strTxt = data.getValAsStringA();

	// ALARMの値を書き換える
	if( pAlarm != NULL ) {
		pAlarm->set_ALCD(alcd);
		pAlarm->set_Text(strTxt);
	} else {
		if( m_pCommuIf != NULL ) {
			XStrBuff strErr;
			strErr.Format(_T("ALID (%u) ALTXT (%s).not defined"), alid, strTxt.c_str());
			writeLogCallBack(XLIB_LOG_LEVEL_ERROR, strErr);
		}
	}

	// コールバックをコール
	ack5 = occurreAlarm(alid, alcd, strTxt);

	// ホストから送信
	if( writer.putValAsBinary( &ack5, 1 ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}
int XSEMI_Gem::processGemMessageS5F3(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XD_UINT1	ack5 = 0;

	XData data;
	XDataWriter	writer( &pOutStream );
	{
		XMutexer			locker(m_lock);

		unsigned char*				pRawData = NULL;
		int							iRawSize = 0;
		XD_UINT4					l = 0;
		XD_UINT4					ulCount = 0;
		XD_UINT1					ulALED;
		unsigned char				ack5 = 65;
		std::vector<XSEMI_ALARM*>	alarms;			// 更新するCEID

		// **IDのフォーマットを取得
		XDATA_TYPE					alidFormatType    = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_ALID,   &alidFormatType) == false  ) {
			// フォーマットが不正
			return XSEMI_GEM_ERR_ABORT;
		}

		inData.getRaw(&pRawData, &iRawSize);
		//
		//	データのチェック(i=0) & 更新(i=1)
		//
		for(int i = 0; i < 2; i++) {
			XDataReader readerChk(pRawData, iRawSize);

			//  L,2
			if( readerChk.getValue( &data ) == false ) {
				if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != XDATA_TYPE_L || data.getCount() != 2 ) {
				if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}

			// B[1] ALED
			if( readerChk.getValue( &data ) == false ) {
				if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != XDATA_TYPE_B || data.getCount() != 1 ) {
				if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			ulALED = data.getValAsUInt1();

			// ** ALID
			if( readerChk.getValue( &data ) == false ) {
				if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != alidFormatType ) {
				if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			ulCount = data.getCount();

			if( ulCount == 0 ) {
				// 配列０の場合は、すべてのアラームが対象
				if( i == 1 ) {
					XSEMI_ALARM* pAlarm = m_constData.get_Alarms().getFirstItem();
					while( pAlarm != NULL ) {
						pAlarm->set_Enable( (ulALED & 0x80) == 0 ? false : true );
						alarms.push_back( pAlarm );
						pAlarm = m_constData.get_Alarms().getNextItem();
					}
				}
			} else {
				for( l = 0; l < ulCount; l++) {
					XD_UINT4 ulALID = data.getCastValAsUInt4( l );
				
					XSEMI_ALARM* pAlarm = m_constData.get_Alarms().get_byID( ulALID );
					if( pAlarm == NULL ) {
						if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}

					if( i == 1 ) {
						pAlarm->set_Enable( (ulALED & 0x80) == 0 ? false : true );
						alarms.push_back( pAlarm );
					}
				}
			}
		}
		// マスターを更新
		try {
			std::vector<XSEMI_ALARM*>::iterator iteAlarm;
			iteAlarm = alarms.begin();
			for(; iteAlarm != alarms.end(); iteAlarm++) {
				m_constData.updateMaster( *iteAlarm );
			}
		} catch(...) {
			// 65=否定、要求できる状態にない
			ack5 = 65;
			if( writer.putValAsBinary( &ack5, 1 ) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			return XSEMI_GEM_ERR_NON_TRANSACT_OV;
		}

		ack5 = 0;
		if( writer.putValAsBinary( &ack5, 1 ) == false ) {
			return XSEMI_GEM_ERR_OUT_BUFF;
		}
		return XSEMI_GEM_ERR_NON_TRANSACT_OV;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS5F5(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XD_UINT1	ack5 = 0;

	XData data;
	XDataWriter	writer( &pOutStream );
	{
		XMutexer			locker(m_lock);
		XD_UINT4			l = 0;
		XD_UINT4			ulCount = 0;
		XD_UINT4			ulALCount = 0;

		// **IDのフォーマットを取得
		XDATA_TYPE					alidFormatType    = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_ALID,   &alidFormatType) == false  ) {
			// フォーマットが不正
			return XSEMI_GEM_ERR_ABORT;
		}

		//  L,2
		if( inData.getValue( &data ) == false ) {
			if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
			return XSEMI_GEM_ERR_NON_TRANSACT_OV;
		}
		if( data.getType() != XDATA_TYPE_U4 ) {
			if( writer.putValAsBinary( &ack5, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
			return XSEMI_GEM_ERR_NON_TRANSACT_OV;
		}
		ulCount = data.getCount();
		if( ulCount == 0 ) {

			ulALCount = m_constData.get_Alarms().count();
			if( writer.putValAsList( ulALCount ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
			
			for(l = 0; l < ulALCount; l++ ) {
				XSEMI_ALARM* pAlarm = m_constData.get_Alarms().get( l );
				XD_UINT1 alcd = pAlarm->get_ALCD();
				XD_UINT4 alid = pAlarm->get_AlarmID();

				if( writer.putValAsList( 3 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }

				if( writer.putValAsBinary( &alcd, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				if( writer.putCastValue(alidFormatType, XDATA_TYPE_U4, &alid, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				if( writer.putValAsStringA( pAlarm->get_Text() ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
			}
		} else {
			for(l = 0; l < ulCount; l++ ) {
				XD_UINT4 ulALID = data.getValAsUInt4( l );
				XD_UINT1 alcd = 0;
				XD_UINT4 alid = 0;
				XSEMI_ALARM* pAlarm = m_constData.get_Alarms().get_byID( ulALID );

				if( pAlarm != NULL ) {
					alcd = pAlarm->get_ALCD();
					alid = pAlarm->get_AlarmID();
					if( writer.putValAsBinary( &alcd, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
					if( writer.putCastValue(alidFormatType, XDATA_TYPE_U4, &alid, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
					if( writer.putValAsStringA( pAlarm->get_Text() ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				} else {
					if( writer.putValAsBinary( &alcd, 0 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
					if( writer.putCastValue(alidFormatType, XDATA_TYPE_U4, &alid, 0 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
					if( writer.putValAsStringA( "" ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				}
			}
		}
		return XSEMI_GEM_ERR_NON_TRANSACT_OV;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS5F7(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XD_UINT1	ack5 = 0;
	XD_UINT4	alid = 0;

	XData data;
	XDataWriter	writer( &pOutStream );
	{
		XMutexer			locker(m_lock);
		XD_UINT4			l = 0;
		XD_UINT4			ulCount = 0;
		// **IDのフォーマットを取得
		XDATA_TYPE					alidFormatType    = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_ALID,   &alidFormatType) == false  ) {
			// フォーマットが不正
			return XSEMI_GEM_ERR_ABORT;
		}

		for( l = 0; l < m_constData.get_Alarms().count(); l++) {
			XSEMI_ALARM* pAlarm = m_constData.get_Alarms().get( l );
			if( pAlarm->get_Enable() == true ) {
				ulCount++;
			}
		}

		if( writer.putValAsList( ulCount ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
		for( l = 0; l < m_constData.get_Alarms().count(); l++) {
			XSEMI_ALARM* pAlarm = m_constData.get_Alarms().get( l );
			if( pAlarm->get_Enable() == true ) {
				XD_UINT1 alcd = pAlarm->get_ALCD();
				alid = pAlarm->get_AlarmID();

				if( writer.putValAsList( 3 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }

				if( writer.putValAsBinary( &alcd, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				if( writer.putCastValue(alidFormatType, XDATA_TYPE_U4, &alid, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				if( writer.putValAsStringA( pAlarm->get_Text() ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
			}
		}
		return XSEMI_GEM_ERR_NON_TRANSACT_OV;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

 