#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/xfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>



int XSEMI_Gem::processGemMessageS2(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XData data;
	XDataWriter	writer( &pOutStream );
	switch( function ) {
	case 13:
		// 	S2,F13 Equipment Constant Request (ECR)
		return processGemMessageS2F13(inData, pOutStream);
		break;
	case 15:
		// S2,F15 New Equipment Constant Send (ECS)
		return processGemMessageS2F15(inData, pOutStream);
		break;
	case 17:
		//S2,F17 Date and Time Request
		return processGemMessageS2F17(inData, pOutStream);
		break;
	case 25:
		// S2,F25 Loopback Diagnostic Reques
		return processGemMessageS2F25(inData, pOutStream);
		break;

	case 29:
		// 	S2,F29 Equipment Constant Namelist Request (ECNR)
		return processGemMessageS2F29(inData, pOutStream);
		break;
	case 31:
		// S2,F31 Date and Time Set Request (DTS)
		return processGemMessageS2F31(inData, pOutStream);
		break;
	case 33:
		// S2,F33 Define Report (DR)
		return processGemMessageS2F33(inData, pOutStream);
	case 35:
		// S2,F35 Link Event Report (LER)
		return processGemMessageS2F35(inData, pOutStream);
		break;
	case 37:
		// S2,F37 Enable/Disable Event Report (EDER)
		return processGemMessageS2F37(inData, pOutStream);
		break;
	case 39:
		// S2,F39 Multi-block Inquire (DMBI)
		return processGemMessageS2F39(inData, pOutStream);
		break;
	case 43:
		// S2,F43 Reset Spooling Streams and Functions (RSSF)
		return processGemMessageS2F43(inData, pOutStream);
		break;
	}
	return XSEMI_GEM_ERR_HEAD_FUNCTION;
XEXCEPT_EXIT
}



int XSEMI_Gem::processGemMessageS2F13(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
		S2,F13 Equipment Constant Request (ECR) S,H→E,返信
	
		説明： 指定した現在の装置定数の要求を行う。
		構造： L,n
				1. <U4 ECID1> * 装置定数ID
				:
				n. <U4 ECIDn>
		例外： 長さ0のリスト（n=0）の場合は、予め定義されている順番（ECIDの番号の若い順）ですべての定数	を要求する。
		通常、本メッセージでは装置定数（ECID）のみ使用可能だが、本装置では状態変数（SVID）やデータ変数（DVVAL）の指定も可能とする。
		また、以下のS2,F13フォーマットも受信可能である。
		<U4 ECID1 ECID2 .... ECIDn> * 装置定数ID
		注意： 本メッセージにて使用できるECIDは各スタンダード仕様書の「データ項目」のクラスがECVで示されているVIDである。
		
		  
		S2,F14 Equipment Constant Data (ECD) M,H←E

		説明： S2,F13にて要求された順番で装置定数を応答する。
		構造： L,n
				1. <ECV1> * 装置定数
				:
				n. <ECVn>
		例外： n=0の場合は、応答定数がないことを意味する。
		ECViのかわりに長さ0のアイテムが返された場合は、指定されたECIDiが存在しないことを意味する。
	*/
	XMutexer			locker(m_lock);
	XData data;
	XDataWriter	writer( &pOutStream );

	// L.n
	if( inData.getValue( &data ) == false ) {
		return XSEMI_GEM_ERR_DATA;
	}
	if( data.getType() != XDATA_TYPE_L ) {
		return XSEMI_GEM_ERR_DATA;
	}
	if( data.getCount() == 0 ) {

		XSEMI_Vs &vs = m_constData.get_Vs();
		// 有効なSVの数を数える
		XD_UINT4 ulNumOfSV = 0,
				 i = 0, 
				 loop = vs.countECV();

		for(i = 0; i < loop; i++) {
			XSEMI_ECV* pECV = vs.getECV( i ) ;

			if( pECV->get_Enable() == true &&
				pECV->get_Private() == false) {	// 有効　＆＆　Public
				ulNumOfSV++;
			}
		}


		// L.n
		if( writer.putValAsList( ulNumOfSV )  == false ) {
			return XSEMI_GEM_ERR_DATA;
		}
		for(i = 0; i < loop; i++) {
			XSEMI_ECV* pECV = vs.getECV( i ) ;

			if( !( pECV->get_Enable() == true &&
				   pECV->get_Private() == false ) ) {	// 有効　＆＆　Public
				continue;
			}


			if( pECV->buildBinaryData( &pOutStream )  == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}					
		}
	} else {
		// VIDのフォーマットを取得
		XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
		if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
			// フォーマットが不正
			return XSEMI_GEM_ERR_ABORT;
		}

		XD_UINT4 ulTopListNum = data.getCount();
		// L.n
		if( writer.putValAsList( data.getCount() )  == false ) {
			return XSEMI_GEM_ERR_DATA;
		}
		for(XD_UINT4 i = 0; i < ulTopListNum; i++) {
			// xx ECVn
			if( inData.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != XDATA_TYPE_U4 ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getCount() != 1 ) {
				return XSEMI_GEM_ERR_DATA;
			}
			
			XSEMI_V* pEC = NULL;

			if( vidFormatType == XDATA_TYPE_A ) {
				XStrBuff strVName = data.getValAsStringA();
				// VNameから、検索する。
				pEC = m_constData.get_Vs().get_byName( strVName );
			} else {						
				if( data.getCount() != 1 ) {
					return XSEMI_GEM_ERR_IN_DATA;
				}
				XD_UINT4 ulEC =  data.getCastValAsUInt4();
				pEC = m_constData.get_Vs().get_SV_byID( ulEC );
			}
			
			if( pEC != NULL) {
				if( pEC->buildBinaryData( &pOutStream )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}		
			} else {
				// Vが見つからない場合は、L.0で返す
				if( writer.putValAsList( 0 )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}
		}
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F15(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
		S2,F15 New Equipment Constant Send (ECS) S,H→E,返信
		説明： 指定した装置定数の変更を要求する。
		構造： L,n
				1. L,2
					1. <xx ECID1> * 装置定数ID
					2. <ECV1> * 装置定数
				:
				n. L,2
					1. <xx ECIDn>
					2. <ECVn> * 装置定数
				2. <ECVn>
		注意：	1. 本メッセージにて使用できるECIDは各スタンダード仕様書の「データ項目」のクラスがECVで示されているVIDである。
					装置定数（ECID）のみが使用可能である。
				2. IDLE状態でのみ有効である。
				3. 受信するECVのフォーマットに関して、レングスの違いは無視して処理する。例えば、U2フォーマットであるべきアイテムに対して、U1フォーマットで指定されてもエラーとせず正常に処理可能とする。ただし、U2フォーマットであるべきアイテムに対して、I2フォーマットの指定はエラーとする。



		S2,F16 New Equipment Constant Acknowledge (ECA) S,H←E
		説明： S2,F15にて要求された装置定数変更の応答を行う。
		構造： <B[1] EAC>
				EAC： 装置確認コード
					0=了解
					1=否定、少なくとも一つの定数が存在しない
					2=否定、ビジー
					3=否定、少なくとも一つの定数が範囲外にある
					4-63 保留
					64=否定、少なくとも一つの定数のフォーマットが異なる
					255=その他のエラー
		注意： 指定されたVIDのひとつでも存在しない場合には、S2,F15で指定されたすべての設定要求を拒否する。
	*/
	{
		XMutexer			locker(m_lock);
		XData data;
		XDataWriter	writer( &pOutStream );

		unsigned char*				pRawData = NULL;
		int							iRawSize = 0;
		XD_UINT4					l = 0;
		XD_UINT4					ulCount = 0;
		unsigned char				eca = 0;
		// VIDのフォーマットを取得
		XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
		if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
			// フォーマットが不正
			return XSEMI_GEM_ERR_ABORT;
		}


		inData.getRaw(&pRawData, &iRawSize);

		//
		//	データのチェック(i=0) & 更新(i=1)
		//
		for(int i = 0; i < 2; i++) {
			// ECがあるか調べる
			XDataReader readerChk(pRawData, iRawSize);

			// L,n
			if( readerChk.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getCount() == 0 ) {
				return XSEMI_GEM_ERR_DATA;
			}
			ulCount = data.getCount();
			for(l = 0; l < ulCount; l++) {
				// L,n
				if( readerChk.getValue( &data ) == false ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( data.getType() != XDATA_TYPE_L ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( data.getCount() != 2 ) {
					return XSEMI_GEM_ERR_DATA;
				}

				// U4 ECID
				if( readerChk.getValue( &data ) == false ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( data.getType() != vidFormatType ) {
					return XSEMI_GEM_ERR_DATA;
				}

				XSEMI_V* pEC = NULL;
				if( vidFormatType == XDATA_TYPE_A ) {
					XStrBuff strVName = data.getValAsStringA();
					// VNameから、検索する。
					pEC = m_constData.get_Vs().get_byName( strVName );
				} else {						
					if( data.getCount() != 1 ) {
						return XSEMI_GEM_ERR_IN_DATA;
					}
					XD_UINT4 ulECID =  data.getCastValAsUInt4();
					pEC = m_constData.get_Vs().get_EC_byID( ulECID );
				}
				

				if( pEC == NULL ) {
					// 1=否定、少なくとも一つの定数が存在しない
					eca = 1;

					// ホストから送信
					if( writer.putValAsBinary( &eca, 1 ) == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}

				// ECV
				if( readerChk.getValue( &data ) == false ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( data.getType() != pEC->getType() ) {
					// 64=否定、少なくとも一つの定数のフォーマットが異なる
					eca = 64;

					// ホストから送信
					if( writer.putValAsBinary( &eca, 1 ) == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}
				if( i == 0 ) {
					// データのチェック
					if( pEC->check( data ) != 0 ) {
						// 3=否定、少なくとも一つの定数が範囲外にある
						eca = 3;

						// ホストから送信
						if( writer.putValAsBinary( &eca, 1 ) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}
				} else {
					// 更新
					if( pEC->putValueFromXData(readerChk, data) == false ) {
						// 255=その他のエラー
						eca = 255;

						// ホストから送信
						if( writer.putValAsBinary( &eca, 1 ) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}
				}
			}
		}
		// 0=了解
		eca = 0;
		// ホストから送信
		if( writer.putValAsBinary( &eca, 1 ) == false ) {
			return XSEMI_GEM_ERR_OUT_BUFF;
		}
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F17(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
		S2,F17 Date and Time Request

		S2,F18 Date and Time Data (DTD) S,H⇔E
		構造： <A[12 or 16] TIME> * 日時
	*/
	XData data;
	XDataWriter	writer( &pOutStream );

	XTime time = XTime::getCurrentTime();
	char szTime[64] = {0};

	
	XStrBuff::__sprintf(szTime, XCOUNTOF(szTime), "%04u%02u%02u%02u%02u%02u", time.GetYear(), time.GetMonth(),
		time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());


	if( writer.putValAsStringA( szTime ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F18(XSECSHeader& header, unsigned char* pMsgData,XD_UINT4 ulMsgSize)
{
XEXCEPT_ENTRY
	XDataReader reader(pMsgData+10, ulMsgSize-10);
	XData       data;
	XStrBuff	strTime;
	XTime		nowTime = this->getGemTime();
	int			len = 0;
	int			year = 0;
	int			month = 0;
	int			day = 0;
	int			hour = 0;
	int			minite = 0;
	int			second = 0;
	int			millisecond = 0;
	const TCHAR* pWk;

	if( reader.getValue( &data ) == false || data.getType() != XDATA_TYPE_A ) {
		return XSEMI_GEM_ERR_DATA;
	}
	data.getValAsStringA(strTime);
	len = strTime.GetLength();
	if( !(len == 12 || len == 16) ) { 
		return XSEMI_GEM_ERR_DATA;
	}
	if( XStrBuff::isDigitsEx(strTime) != 1 ) {
		return XSEMI_GEM_ERR_DATA;
	}
	pWk = strTime;
	if( len == 12 ) {
		year  = ((*pWk) - '0')*10;
		pWk++;
		year += ((*pWk) - '0')*1;
		pWk++;
		if( year > 90 ) {
			year+=1900;
		} else {
			year+=2000;
		}
	} else {
		year  = ((*pWk) - '0')*1000;
		pWk++;
		year += ((*pWk) - '0')*100;
		pWk++;
		year += ((*pWk) - '0')*10;
		pWk++;
		year += ((*pWk) - '0')*1;
		pWk++;

	}

	month  = ((*pWk) - '0')*10;
	pWk++;
	month += ((*pWk) - '0')*1;
	pWk++;


	day  = ((*pWk) - '0')*10;
	pWk++;
	day += ((*pWk) - '0')*1;
	pWk++;

	hour  = ((*pWk) - '0')*10;
	pWk++;
	hour += ((*pWk) - '0')*1;
	pWk++;

	minite  = ((*pWk) - '0')*10;
	pWk++;
	minite += ((*pWk) - '0')*1;
	pWk++;

	second  = ((*pWk) - '0')*10;
	pWk++;
	second += ((*pWk) - '0')*1;
	pWk++;
	if( len == 16 ) {
		millisecond  = ((*pWk) - '0')*10;
		pWk++;
		millisecond += ((*pWk) - '0')*1;
		pWk++;
	}
	XTime xtime = XTime(year, month, day, hour, minite, second, millisecond);

	time_t	tNowTime = nowTime.mktime();
	time_t	tGemTime = xtime.mktime();

	// デルタ時間をセットする。
	m_time.setDelta( tGemTime - tNowTime );

	return XSEMI_GEM_ERR_NON_TRANSACT_DEF;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F25(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/* S2,F25 Loopback Diagnostic Request */
	XData data;
	XDataWriter	writer( &pOutStream );

	unsigned char*				pRawData = NULL;
	XD_UINT4					iRawSize = 0;
	// B
	if( inData.getValue( &data ) == false ) {
		return XSEMI_GEM_ERR_DATA;
	}
	if( data.getType() != XDATA_TYPE_B ) {
		return XSEMI_GEM_ERR_DATA;
	}
/*
    // XSEMI_Gem::receiveDataMessageCallBackで既に設定済みなので、セットしない。
	iRawSize = data.getValAsBinaryRef( &pRawData );
	if( writer.putValue(XDATA_TYPE_B, pRawData, iRawSize ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
*/
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F29(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
		S2,F29 Equipment Constant Namelist Request (ECNR) S,H→E,返信

		説明： ホストが装置内で有効な装置定数に関する基本的な情報を要求する。
		構造： L,n
				1. <xx ECID1> * 装置定数ID
				:
				n. <xx ECIDn>
		例外： 長さ0リスト（n=0）の場合は、すべての定数を要求する。このとき、S2,F30ではECIDの順番ですべての装置定数の属性を報告する。
		注意： 本メッセージにて使用できるECIDは各スタンダード仕様書の「データ項目」のクラスがECVで示されているVIDである。

		S2,F30 Equipment Constant Namelist (ECN) M,H←E

		説明： S2,F29の要求に対して応答する。
		構造： L,n
				1. L,6
					1. <xx ECID1> * 装置定数
					2. <A ECNAME1> * 装置定数名
					3. < ECMIN1> * 装置定数最小値
					4. < ECMAX1> * 装置定数最大値
					5. < ECDEF1> * 装置定数デフォルト値
					6. <A UNITS1> * 単位
				:
				n. L,6
					1. <U4 ECIDn>
					2. <A ECNAMEn>
					3. < ECMINn>
					4. < ECMAXn>
					5. < ECDEFn>
					6. <A UNITSn>
		注意： 単位がない場合、UNITSはゼロ･レングスで報告する。
		例外： 存在しないECIDを要求された場合は、ECNAME、ECMIN、ECMAX、ECDEF、UNITはASCIIのゼロ･レングスにて応答する。
	*/
	XData data;
	XDataWriter	writer( &pOutStream );
	XMutexer			locker(m_lock);
	XRichData* pRData = NULL;
	// VIDのフォーマットを取得
	XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
	if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
		// フォーマットが不正
		return XSEMI_GEM_ERR_ABORT;
	}
		
		
	// L.n
	if( inData.getValue( &data ) == false ) {
		XSEMI_GEM_ERR_OUT_BUFF;
	}
	if( data.getType() != XDATA_TYPE_L ) {
		XSEMI_GEM_ERR_OUT_BUFF;
	}
	if( data.getCount() == 0 ) {
		XSEMI_Vs &vs = m_constData.get_Vs();
		// 有効なSVの数を数える
		XD_UINT4 ulNumOfECV = 0,
				 i = 0, 
				 loop = vs.countECV();

		for(i = 0; i < loop; i++) {
			XSEMI_ECV* pECV = vs.getECV( i ) ;

			if( pECV->get_Enable() == true &&
				pECV->get_Private() == false) {	// 有効　＆＆　Public
				ulNumOfECV++;
			}
		}



		// L.n
		if( writer.putValAsList( ulNumOfECV )  == false ) {
			XSEMI_GEM_ERR_OUT_BUFF;
		}
		for(i = 0; i < loop; i++) {
			XSEMI_ECV* pECV = vs.getECV( i ) ;
			if( !(	pECV->get_Enable() == true &&
					pECV->get_Private() == false ) ) {	// 有効　＆＆　Public
				continue;
			}
			// L.3
			if( writer.putValAsList( 6 )  == false ) {
				XSEMI_GEM_ERR_OUT_BUFF;
			}
			//   xx ECID
			if( this->innerWriteVID(vidFormatType, pECV, writer) == false ) {
				XSEMI_GEM_ERR_OUT_BUFF;
			}
			//   A ECNAME
			if( writer.putValAsStringA( pECV->get_Name() )  == false ) {
				XSEMI_GEM_ERR_OUT_BUFF;
			}
			//	   ECMIN
			if( pECV->get_EcMin().buildBinaryData( &pOutStream ) ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			//	   ECMAX
			if( pECV->get_EcMax().buildBinaryData( &pOutStream ) ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			//	   ECDEF
			if( pECV->get_EcDef().buildBinaryData( &pOutStream ) ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			//   A UNITS
			if( writer.putValAsStringA( pECV->get_Unit() )  == false ) {
				XSEMI_GEM_ERR_OUT_BUFF;
			}						
		}
	} else {
		XD_UINT4 ulTopListNum = data.getCount();
		// L.n
		if( writer.putValAsList( data.getCount() )  == false ) {
			return XSEMI_GEM_ERR_OUT_BUFF;
		}
		for(XD_UINT4 i = 0; i < ulTopListNum; i++) {
			// xx ECIDn
			if( inData.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			if( data.getType() != vidFormatType ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			XSEMI_ECV* pECV = NULL;

			if( vidFormatType == XDATA_TYPE_A ) {
				XStrBuff strVName = data.getValAsStringA();
				// VNameから、検索する。
				XSEMI_V* pV = m_constData.get_Vs().get_byName( strVName );
				if( pV != NULL ) {
					if( pV->get_Class() == XSEMI_V::EC ) {
						pECV = (XSEMI_ECV*)pV;
					}
				}
			} else {						
				if( data.getCount() != 1 ) {
					return XSEMI_GEM_ERR_IN_DATA;
				}
				XD_UINT4 ulECID =  data.getCastValAsUInt4();
				pECV = m_constData.get_Vs().get_EC_byID( ulECID );
			}


			// L.3
			if( writer.putValAsList( 3 )  == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}


			if( pECV != NULL) {
				//   xx ECID
				if( this->innerWriteVID(vidFormatType, pECV, writer) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				//   A ECNAME
				if( writer.putValAsStringA( pECV->get_Name() )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				//	   ECMIN
				if( writer.putValue( XDATA_TYPE_L, 0, 0 )  == false ) {
					XSEMI_GEM_ERR_OUT_BUFF;
				}
				//	   ECMAX
				if( writer.putValue( XDATA_TYPE_L, 0, 0 )  == false ) {
					XSEMI_GEM_ERR_OUT_BUFF;
				}
				//	   ECDEF
				if( writer.putValue( XDATA_TYPE_L, 0, 0 )  == false ) {
					XSEMI_GEM_ERR_OUT_BUFF;
				}
				//   A UNITS
				if( writer.putValAsStringA( pECV->get_Unit() )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			} else {
				//   xx SVID
				switch ( data.getType() )
				{
				case XDATA_TYPE_A:
					if( writer.putValAsStringA( (const TCHAR*)data.getValAsStringA() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
				case XDATA_TYPE_U1:
					if( writer.putValAsUInt1( data.getValAsUInt1() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
				case XDATA_TYPE_U2:
					if( writer.putValAsUInt2( data.getValAsUInt2() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
				case XDATA_TYPE_U4:
					if( writer.putValAsUInt4( data.getValAsUInt4() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
				case XDATA_TYPE_U8:
					if( writer.putValAsUInt8( data.getValAsUInt8() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
				case XDATA_TYPE_I1:
					if( writer.putValAsInt1( data.getValAsInt1() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
				case XDATA_TYPE_I2:
					if( writer.putValAsInt2( data.getValAsInt2() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
				case XDATA_TYPE_I4:
					if( writer.putValAsInt4( data.getValAsInt4() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
				case XDATA_TYPE_I8:
					if( writer.putValAsInt8( data.getValAsInt8() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					break;
					break;
				default:
					// フォーマットが不正
					return XSEMI_GEM_ERR_ABORT;
					break;
				}
				//   A ECNAME
				if( writer.putValAsStringA( "" )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				//	   ECMIN
				if( writer.putValAsList( 0 )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				//	   ECMAX
				if( writer.putValAsList( 0 )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				//	   ECDEF
				if( writer.putValAsList( 0 )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				//   A UNITS
				if( writer.putValAsStringA( "" )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}
		}
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;

XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F31(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*	S2,F31 Date and Time Set Request (DTS)
				
		S2,F32 Date and Time Set Acknowledge (DTA)

		説明： 日付および時刻セットの了解を応答する。
		構造： <B[1] TIACK> * 時間確認コード
					TIACK:	0=OK
							1=エラー、了解されない
							2-63 保留			
	*/

	XData data;
	XDataWriter	writer( &pOutStream );
	unsigned char tiack = 0;

	// ホストから送信
	if( writer.putValAsBinary( &tiack, 1 ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F33(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
		S2,F33 Define Report (DR) M,H→E,返信

		構造： L,2
				1. <U4 DATAID> * データID
				2. L,a
					1. L,2
						1. <U4 RPTID1> * レポートID
						2. L,b
							1. <U4 VID1> * 変数ID
							:
							b. <U4 VIDb>
					:
					a. L,2
						1. <U4 RPTIDa>
						2. L,c
							1. <U4 VID1>
							:
							c. <U4 VID


		S2,F34 Define Report Acknowledge (DRA) S,H←E
		構造： <B[1] DRACK> * 定義了解報告コード
				DRACK： 0=了解
						1=否定、スペース不十分
						2=否定、無効フォーマット
						3=否定、少なくとも一つのRPTIDはすでに定義されている
						4=否定、少なくとも一つのVIDが存在しない
						5-63 保留
						65=否定、要求できる状態にない
						66=存在しないRPTIDが指定された
	*/
	XData data;
	XDataWriter	writer( &pOutStream );
	{
		XMutexer			locker(m_lock);

		unsigned char*				pRawData = NULL;
		int							iRawSize = 0;
		XD_UINT4					l = 0;
		XD_UINT4					m = 0;
		XD_UINT4					ulCount = 0;
		unsigned char				drack = 0;
		std::vector<XSEMI_Rpt*>		rpts;			// 更新するレポートID
		// **IDのフォーマットを取得
		XDATA_TYPE					vidFormatType    = XDATA_TYPE_U4;
		XDATA_TYPE					dataIdFormatType = XDATA_TYPE_U4;
		XDATA_TYPE					rtpIdFormatType  = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_VID,    &vidFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_DATAID, &dataIdFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_RPTID,  &rtpIdFormatType) == false ) {
			// フォーマットが不正
			return XSEMI_GEM_ERR_ABORT;
		}

		inData.getRaw(&pRawData, &iRawSize);

		//
		//	データのチェック(i=0) & 更新(i=1)
		//
		for(int i = 0; i < 2; i++) {
			if( i == 1 && drack != 0 ) {
				if( writer.putValAsBinary( &drack, 1 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;				
			}

			XDataReader readerChk(pRawData, iRawSize);

			//  L,2
			if( readerChk.getValue( &data ) == false ) {
				// 2=否定、無効フォーマット
//				drack = 2;
//				if( writer.putValAsBinary( &drack, 1 ) == false ) {
//					return XSEMI_GEM_ERR_OUT_BUFF;
//				}
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != XDATA_TYPE_L || data.getCount() != 2 ) {
				return XSEMI_GEM_ERR_DATA;
			}

			// U4 DATAID
			if( readerChk.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != dataIdFormatType ) {
				return XSEMI_GEM_ERR_DATA;
			}
//			ulDATAID = data.getValAsUInt4();

			// 2. L,a
			if( readerChk.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				return XSEMI_GEM_ERR_DATA;
			}
			ulCount = data.getCount();

			if( ulCount == 0 ) {
				// 1. <DATAID>に続く長さ0のリスト（a=0）は、すべてのレポート規定および関連リンクを消去する。
				drack = 65;
				try {
					if( m_constData.deleteAllCEID_RptID() == true ) {
						drack = 0;
					}
					if( m_constData.unDefineReport((XD_UINT4)-1,(XD_UINT4)-1) == true ) {
						drack = 0;
					}
					m_constData.updateMaster( &m_constData.get_Rpts() );
				} catch(...) {
				}
				// ホストから送信
				if( writer.putValAsBinary( &drack, 1 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			} else {
				try {
					for(l = 0; l < ulCount; l++) {
						//  L,2
						if( readerChk.getValue( &data ) == false ) {
							return XSEMI_GEM_ERR_DATA;
						}
						if( data.getType() != XDATA_TYPE_L || data.getCount() != 2 ) {
							return XSEMI_GEM_ERR_DATA;
						}


						XD_UINT4	ulReptID = ~0;
						XStrBuff	strRtpIDName(_T(""));
						XSEMI_Rpt*	pRpt = NULL;

						// U4 RPTID
						if( readerChk.getValue( &data ) == false ) {
							return XSEMI_GEM_ERR_DATA;
						}
						if( data.getType() != rtpIdFormatType ) {
							return XSEMI_GEM_ERR_DATA;
						}
						if( rtpIdFormatType == XDATA_TYPE_A ) {
							strRtpIDName = data.getValAsStringA();
							// VNameから、検索する。
							pRpt = m_constData.get_Rpts().get_byName( strRtpIDName );
						} else {						
							if( data.getCount() != 1 ) {
								return XSEMI_GEM_ERR_IN_DATA;
							}
							ulReptID =  data.getCastValAsUInt4();
							pRpt = m_constData.get_Rpts().get_byID( ulReptID );
						}

		
						// L,b
						if( readerChk.getValue( &data ) == false ) {
							return XSEMI_GEM_ERR_DATA;
						}
						if( data.getType() != XDATA_TYPE_L ) {
							return XSEMI_GEM_ERR_DATA;
						}
						// セットするVの数
						XD_UINT4 ulVCount = data.getCount();

						if( pRpt == NULL ) {
							if( ulVCount != 0 && i == 1 ) {
								if( rtpIdFormatType == XDATA_TYPE_A ) {
									// RPTIDを採番
									for(ulReptID = 1; ulReptID <= 0xFFFFFFFF; ulReptID++ ) {
										pRpt = m_constData.get_Rpts().get_byID( ulReptID );
										if( pRpt == NULL ) {
											break;
										}
									}
								} else {
									// RPTID名を生成
									for(XD_UINT4 ii = 0; ii <= 0xFFFFFFFF; ii++ ) {
										strRtpIDName.Format(_T("Rpt_%u"), (ulReptID+ii));
										pRpt = m_constData.get_Rpts().get_byName( strRtpIDName );
										if( pRpt == NULL ) {
											break;
										}
									}
								}
								// レポートIDの作成
								XSEMI_Rpt tmpRpt(ulReptID, strRtpIDName);
								pRpt = m_constData.get_Rpts().add( &tmpRpt );
								if( pRpt == NULL ) {
									// 65=否定、要求できる状態にない
									drack = 65;
									if( writer.putValAsBinary( &drack, 1 ) == false ) {
										return XSEMI_GEM_ERR_OUT_BUFF;
									}
									return XSEMI_GEM_ERR_NON_TRANSACT_OV;
								}
								pRpt->set_Comment(_T("Add from host"));
								pRpt->set_Enable( true );
							}
//							// 66=存在しないRPTIDが指定された
//							drack = 66;
//							innerWriteLog(XLIB_LOG_LEVEL_WARNING, "[S2F33] RPTID (%u). DRACK=66. RPTID dose not exit.", ulReptID);
//							if( writer.putValAsBinary( &drack, 1 ) == false ) {
//								return XSEMI_GEM_ERR_OUT_BUFF;
//							}
//							return XSEMI_GEM_ERR_NON_TRANSACT_OV;
						}


						if( i == 0 ) {
							if( pRpt != NULL ) {
								if( pRpt->get_VsCount() != 0 && ulVCount != 0 ) {
									// 3=否定、少なくとも一つのRPTIDはすでに定義されている
									drack = 3;
									innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("[S2F33] RPTID (%u). DRACK=3. RPTID already defined."), ulReptID);
								}
							}
						}


						if( i == 1 ) {
							pRpt->set_Enable(ulVCount == 0 ? false : true);

							m_constData.unDefineReport(ulReptID, 0xFFFFFFFF);

							rpts.push_back(  pRpt );

						}

						// 指定されたVが存在するか調べる
						for(m = 0; m < ulVCount; m++) {
							// L,b
							if( readerChk.getValue( &data ) == false ) {
								return XSEMI_GEM_ERR_DATA;
							}

							XSEMI_V* pV = NULL;
							XStrBuff	strVName(_T(""));
							
							// VID
							if( vidFormatType == XDATA_TYPE_A ) {
								strVName = data.getValAsStringA();
								// VNameから、検索する。
								pV = m_constData.get_Vs().get_byName( strVName );
							} else {						
								if( data.getCount() != 1 ) {
									return XSEMI_GEM_ERR_IN_DATA;
								}
								XD_UINT4 ulVID =  data.getCastValAsUInt4();
								strVName.Format(_T("%u"), ulVID);

								pV = m_constData.get_Vs().get_byID( ulVID );
							}
							if( pV == NULL ) {
								// 4=否定、少なくとも一つのVIDが存在しない
								drack = 4;
								innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("[S2F33] RPTID (%u) VID(%s). DRACK=4. VID dose not exist."), ulReptID, (const TCHAR*)strVName);
							} else 
							if( i == 1 ) {
								m_constData.defineReport(ulReptID, pV->get_VID());
							}
						}
					}
				} catch(...) {
					// 65=否定、要求できる状態にない
					drack = 65;
					if( writer.putValAsBinary( &drack, 1 ) == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}
			}
		} // for(int i = 0; i < 3; i++) 

		// マスターを更新
		try {
			m_constData.updateMaster( &m_constData.get_Rpts() );
		} catch(...) {
			// 65=否定、要求できる状態にない
			drack = 65;
			if( writer.putValAsBinary( &drack, 1 ) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			return XSEMI_GEM_ERR_NON_TRANSACT_OV;
		}

		drack = 0;
		if( writer.putValAsBinary( &drack, 1 ) == false ) {
			return XSEMI_GEM_ERR_OUT_BUFF;
		}
		return XSEMI_GEM_ERR_NON_TRANSACT_OV;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F35(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
		S2,F35 Link Event Report (LER) M,H→E,返信
		説明： ホストが、通知イベントID（CEID）に対してレポートをリンクする。
			リンクした時点では、通知イベントID（CEIDs）の報告は常に無効に設定される。
			イベントが発生した際、ホストへの通知イベントIDの報告が必要な場合は、S2,F37を使用して有
			効に設定する必要がある。
			もし、S2,F35がマルチブロックであるならば、S2,F35の送信に先立って、S2,F39/F40（問い合わ
			せ／許可）トランザクションを行うことが可能である。また装置は、DATAIDを無視する。
		構造： L,2
				1. <U4 DATAID> * データID
					2. L,a
						1. L,2
							1. <U4 CEID1> * 通知イベントID
							2. L,b
								1. <U4 RPTID1> * レポートID
								:
								b. <U4 RPTIDb>
						:
						a. L,2
							1. <U4 CEIDa>
							2. L,c
								1. <U4 RPTID1>
								:
								c. <U4 RPTID>
	  

		S2,F36 Link Event Report Acknowledge (LERA) S,H←E
		説明： 了解またはエラーを応答する。
			エラー状態が検出されると、すべてのメッセージは拒否される。すなわち、部分的な変更は許さ
			れない。
		構造： <B[1] LRACK> * リンク報告確認コード
				LRACK： 0=了解
				1=否定、スペース不十分
				2=否定、無効フォーマット						( 重複CEIDが指定された)
				3=否定、少なくとも一つのCEIDはすでに定義されている
				4=否定、少なくとも一つのCEIDが存在しない
				5=否定、少なくとも一つのRPTIDが存在しない
				6-63 保留
				65=否定、要求できる状態にない

	*/
	XData data;
	XDataWriter	writer( &pOutStream );
	{
		XMutexer			locker(m_lock);

		unsigned char*				pRawData = NULL;
		int							iRawSize = 0;
		XD_UINT4					l = 0;
		XD_UINT4					m = 0;
		XD_UINT4					ulCount = 0;
		unsigned char				lrack = 0;
		std::vector<XSEMI_CEID*>	ceids;			// 更新するCEID
		// **IDのフォーマットを取得
		XDATA_TYPE					ceidFormatType    = XDATA_TYPE_U4;
		XDATA_TYPE					dataIdFormatType = XDATA_TYPE_U4;
		XDATA_TYPE					rtpIdFormatType  = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_CEID,   &ceidFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_DATAID, &dataIdFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_RPTID,  &rtpIdFormatType) == false ) {
			// フォーマットが不正
			return XSEMI_GEM_ERR_ABORT;
		}

		inData.getRaw(&pRawData, &iRawSize);
		//
		//	データのチェック(i=0) & 更新(i=1)
		//
		for(int i = 0; i < 2; i++) {
			if( i == 1 && lrack != 0 ) {
				if( writer.putValAsBinary( &lrack, 1 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;				
			}

			XDataReader readerChk(pRawData, iRawSize);

			//  L,2
			if( readerChk.getValue( &data ) == false ) {
				// 2=否定、無効フォーマット
				/*lrack = 2;
				if( writer.putValAsBinary( &lrack, 1 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}*/
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != XDATA_TYPE_L || data.getCount() != 2 ) {
				return XSEMI_GEM_ERR_DATA;
			}

			// xx DATAID
			if( readerChk.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != dataIdFormatType  ) {
				return XSEMI_GEM_ERR_DATA;
			}

			// 2. L,a
			if( readerChk.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				return XSEMI_GEM_ERR_DATA;
			}
			ulCount = data.getCount();

			if( ulCount == 0 ) {
				// 1. <DATAID>に続く長さ0のリスト（a=0）は、すべてのレポート規定および関連リンクを消去する。
				lrack = 65;
				try {
					if( m_constData.deleteAllCEID_RptID() == true ) {
						lrack = 0;
					}
				} catch(...) {
				}
				// ホストから送信
				if( writer.putValAsBinary( &lrack, 1 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			} else {
				try {
					for(l = 0; l < ulCount; l++) {
						//  L,2
						if( readerChk.getValue( &data ) == false ) {
							return XSEMI_GEM_ERR_DATA;
						}
						if( data.getType() != XDATA_TYPE_L || data.getCount() != 2 ) {
							return XSEMI_GEM_ERR_DATA;
						}

						XSEMI_CEID* pCEID = NULL;
						XD_UINT4	ulCEID = 0;
						XStrBuff    strCEIDName(_T(""));
						// U4 CEID
						if( readerChk.getValue( &data ) == false ) {
							return XSEMI_GEM_ERR_DATA;
						}
						if( data.getType() != ceidFormatType || data.getCount() != 1) {
							return XSEMI_GEM_ERR_DATA;
						}
						if( ceidFormatType == XDATA_TYPE_A ) {
							strCEIDName = data.getValAsStringA();
							// VNameから、検索する。
							pCEID = m_constData.get_CEIDs().get_byName( strCEIDName );
							if( pCEID != NULL ) {
								ulCEID = pCEID->get_CEID();
							}
						} else {						
							if( data.getCount() != 1 ) {
								return XSEMI_GEM_ERR_IN_DATA;
							}
							XD_UINT4 ulCEID = data.getValAsUInt4();
							strCEIDName.Format(_T("%u"), ulCEID);
							pCEID = m_constData.get_CEIDs().get_byID( ulCEID );
						}

						
						
						if( pCEID == NULL ) {
							// 4=否定、少なくとも一つのCEIDが存在しない
							lrack = 4;

							innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("[S2F35] CEID (%s). LRACK=4. CEID dose not exit."), (const TCHAR*)strCEIDName);
						}
						
						// L,b
						if( readerChk.getValue( &data ) == false ) {
							return XSEMI_GEM_ERR_DATA;
						}
						if( data.getType() != XDATA_TYPE_L ) {
							return XSEMI_GEM_ERR_DATA;
						}
						// セットするVの数
						XD_UINT4 ulRptCount = data.getCount();

						if( i == 0 ) {
							if( pCEID != NULL ) {
								if( pCEID->get_RptsCount() != 0 && ulRptCount != 0 ) {
									// 3=否定、少なくとも一つのRPTIDはすでに定義されている
									lrack = 3;

									innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("[S2F35] CEID (%s). LRACK=3. CEID link already defined."),  (const TCHAR*)strCEIDName);
								}
							}
						}


						if( i == 1 ) {
							// CEIDに緋も付くレポートIDをすべて削除する。
							m_constData.unlinkEventReport(ulCEID, 0xFFFFFFFF);

							ceids.push_back( pCEID );

						}

						// 指定されたReptIDが存在するか調べる
						for(m = 0; m < ulRptCount; m++) {
							// L,b
							if( readerChk.getValue( &data ) == false ) {
								return XSEMI_GEM_ERR_DATA;
							}
							if( data.getType() != rtpIdFormatType  || data.getCount() != 1) {
								return XSEMI_GEM_ERR_DATA;
							}
							// RPTID
							XStrBuff strRptName(_T(""));
							XSEMI_Rpt *pRptID = NULL;
							XD_UINT4 ulRPTID = data.getValAsUInt4();
							if( rtpIdFormatType == XDATA_TYPE_A ) {
								XStrBuff strRptName = data.getValAsStringA();
								// VNameから、検索する。
								pRptID = m_constData.get_Rpts().get_byName( strRptName );
								if( pRptID != NULL ) {
									ulRPTID = pRptID->get_RptID();
								}
							} else {						
								if( data.getCount() != 1 ) {
									return XSEMI_GEM_ERR_IN_DATA;
								}
								ulRPTID = data.getValAsUInt4();
								strRptName.Format(_T("%u"), ulRPTID);
								pRptID = m_constData.get_Rpts().get_byID( ulRPTID );
							}


							if( pRptID == NULL ) {
								// 5=否定、少なくとも一つのRPTIDが存在しない
								lrack = 5;
								
								innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("[S2F35] RPTID (%s). LRACK=5. RPTID dose not exist."), (const TCHAR*)strRptName);
							}
							if( i == 1 ) {
								m_constData.linkEventReport(ulCEID, ulRPTID);
							}
						}
					}
				} catch(...) {
					// 65=否定、要求できる状態にない
					lrack = 65;
					if( writer.putValAsBinary( &lrack, 1 ) == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}
			}
		}




		// マスターを更新
		try {
			m_constData.updateMaster( &m_constData.get_CEIDs() );
		} catch(...) {
			// 65=否定、要求できる状態にない
			lrack = 65;
			if( writer.putValAsBinary( &lrack, 1 ) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			return XSEMI_GEM_ERR_NON_TRANSACT_OV;
		}

		lrack = 0;
		if( writer.putValAsBinary( &lrack, 1 ) == false ) {
			return XSEMI_GEM_ERR_OUT_BUFF;
		}
		return XSEMI_GEM_ERR_NON_TRANSACT_OV;

	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F37(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
	ERACK： 0=了解
			1=否定、少なくとも一つのCEIDが存在しない
			2-63 保留
			65=否定、要求できる状態にない
	*/
	XData data;
	XDataWriter	writer( &pOutStream );
	{
		XMutexer			locker(m_lock);

		unsigned char*				pRawData = NULL;
		int							iRawSize = 0;
		XD_UINT4					l = 0;
		XD_UINT4					ulCount = 0;
		bool						bEnable = false;
		unsigned char				erack = 65;
		std::vector<XSEMI_CEID*>	ceids;			// 更新するCEID
		// **IDのフォーマットを取得
		XDATA_TYPE					ceidFormatType    = XDATA_TYPE_U4;
		XDATA_TYPE					dataIdFormatType = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_CEID,   &ceidFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_DATAID, &dataIdFormatType) == false ) {
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
				// 2=否定、無効フォーマット
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != XDATA_TYPE_L || data.getCount() != 2 ) {
				// 2=否定、無効フォーマット
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}

			// U4 DATAID
			if( readerChk.getValue( &data ) == false ) {
				// 2=否定、無効フォーマット
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != XDATA_TYPE_BOOL || data.getCount() != 1 ) {
				// 2=否定、無効フォーマット
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			bEnable = data.getValAsBool();

			// 2. L,n
			if( readerChk.getValue( &data ) == false ) {
				// 2=否定、無効フォーマット
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				// 2=否定、無効フォーマット
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			ulCount = data.getCount();

			if( ulCount == 0 ) {
				for( l = 0; l < m_constData.get_CEIDs().count(); l++) {
					XSEMI_CEID* pCEID = m_constData.get_CEIDs().get( l );
					pCEID->put_Enable( bEnable );
				}

				try {
					// マスターを更新
//					m_pMasterData->updateCEIDs( m_constData );
				} catch(...) {
					// 65=否定、要求できる状態にない
					erack = 65;
					if( writer.putValAsBinary( &erack, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}

				erack = 0;
				// ホストから送信
				if( writer.putValAsBinary( &erack, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			} else {
				for( l = 0; l < ulCount; l++) {
					// U4 CEID
					if( readerChk.getValue( &data ) == false ) {
						// 2=否定、無効フォーマット
						erack = 2;
						if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}
					if( data.getType() != ceidFormatType && data.getCount() != 1 ) {
						// 2=否定、無効フォーマット
						erack = 2;
						if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}



					XSEMI_CEID* pCEID = NULL;
					XD_UINT4	ulCEID = 0;
					XStrBuff    strCEIDName(_T(""));
					// U4 CEID
					if( readerChk.getValue( &data ) == false ) {
						return XSEMI_GEM_ERR_DATA;
					}
					if( data.getType() != ceidFormatType ) {
						return XSEMI_GEM_ERR_DATA;
					}
					if( ceidFormatType == XDATA_TYPE_A ) {
						strCEIDName = data.getValAsStringA();
						// VNameから、検索する。
						pCEID = m_constData.get_CEIDs().get_byName( strCEIDName );
						if( pCEID != NULL ) {
							ulCEID = pCEID->get_CEID();
						}
					} else {						
						if( data.getCount() != 1 ) {
							return XSEMI_GEM_ERR_IN_DATA;
						}
						ulCEID = data.getValAsUInt4();
						strCEIDName.Format(_T("%u"), ulCEID);
						pCEID = m_constData.get_CEIDs().get_byID( ulCEID );
					}
					

					if( pCEID == NULL )  {
						// 1=否定、少なくとも一つのCEIDが存在しない
						erack = 1;
						if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}

					if( i == 1 ) {
						// 更新
						pCEID->put_Enable( bEnable );
						ceids.push_back( pCEID );
					}
				}


			}
		}
		// マスターを更新
		try {
			std::vector<XSEMI_CEID*>::iterator iteCEID;
			iteCEID = ceids.begin();
			for(; iteCEID != ceids.end(); iteCEID++) {
				m_constData.updateMaster( *iteCEID );
			}
			m_constData.clear();
		} catch(...) {
			// 65=否定、要求できる状態にない
			erack = 65;
			if( writer.putValAsBinary( &erack, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
			return XSEMI_GEM_ERR_NON_TRANSACT_OV;
		}

		erack = 0;
		if( writer.putValAsBinary( &erack, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
		return XSEMI_GEM_ERR_NON_TRANSACT_OV;

	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS2F39(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XD_UINT1	grant = 0;
	XDataWriter	writer( &pOutStream );
	// ホストから送信
	if( writer.putValAsBinary( &grant, 1 ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}



int XSEMI_Gem::processGemMessageS2F43(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XD_UINT1	RSPACK = 0;
	int			nStrErr = 0;
	XData data;
	XDataWriter	writer( &pOutStream );
	{
		unsigned char*				pRawData = NULL;
		int							iRawSize = 0;
		
		inData.getRaw(&pRawData, &iRawSize);

		/*
			RSPACK： 0=応答、スプーリング設定受諾
					1=スプーリング設定拒否
					2-63 保留
			STRACK：	1=スプーリング不可（ストリーム1または、ストリーム9が指示された）
						2=ストリームが不明
						3=ファンクションが不明
						4=二次メッセージが指定された
			注意： 未定義ストリーム、または、未定義ファンクショ		
		*/
		//
		//	データのチェック(i=0) & 更新(i=1)
		//
		for(int i = 0; i < 2; i++) {
			XD_UINT4	ulStreamNum = 0;
			XD_UINT4	ulFuncNum = 0;
			XDataReader readerChk(pRawData, iRawSize);


			
			//  L,m
			if( readerChk.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				return XSEMI_GEM_ERR_DATA;
			}
			ulStreamNum = data.getCount();

			if( i == 1 ) {
				if( RSPACK == 0 ) {
					m_constData.get_SpoolSFs().clear();
				}
				// L.2
				if( writer.putValAsList( 2 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				// 1. <B[1] RSPACK>
				if( writer.putValAsBinary( &RSPACK, 1 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				// 2. L,m
				if( writer.putValAsList( nStrErr ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}
			for( XD_UINT4 j = 0; j < ulStreamNum; j++) {
				XD_UINT1 ucStream = 0;
				XD_UINT1 STRACK = 0;
				//  L,2
				if( readerChk.getValue( &data ) == false ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( data.getType() != XDATA_TYPE_L && data.getCount() != 2 ) {
					return XSEMI_GEM_ERR_DATA;
				}
				// 1. <U1 STRIDx>
				if( readerChk.getValue( &data ) == false ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( data.getType() != XDATA_TYPE_U1 && data.getCount() != 1 ) {
					return XSEMI_GEM_ERR_DATA;
				}
				ucStream = data.getValAsUInt1();
				// 2. L,n
				if( readerChk.getValue( &data ) == false ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( data.getType() != XDATA_TYPE_L ) {
					return XSEMI_GEM_ERR_DATA;
				}
				ulFuncNum = data.getCount();

				std::vector<XD_UINT1>	errorFunc;

				for( XD_UINT4 k = 0; k < ulFuncNum; k++) {
					// 1. <U1 FCNID1>
					XD_UINT1 ucFunc = 0;
					if( readerChk.getValue( &data ) == false ) {
						return XSEMI_GEM_ERR_DATA;
					}
					if( data.getType() != XDATA_TYPE_U1 && data.getCount() != 1 ) {
						return XSEMI_GEM_ERR_DATA;
					}
					ucFunc = data.getValAsUInt1();
					if( (ucFunc % 2 ) == 0 ) {
						// STRACK：	4=二次メッセージが指定された
						RSPACK = 1;
						STRACK = 4;
						if( i == 1 ) {
							errorFunc.push_back( ucFunc );
						}
					} else if( i == 1 && RSPACK == 0 ) {
						XSEMI_SpoolSF tmpData(ucStream, ucFunc);
						m_constData.get_SpoolSFs().add(&tmpData);
					}
				}
				
				if( i == 0 ) {
					if( STRACK != 0 ) {
						nStrErr++;
					}
				} else
				if( i == 1 ) {

					if( ulFuncNum == 0 && RSPACK == 0 ) {
						XSEMI_SpoolSF tmpData(ucStream, 0);
						m_constData.get_SpoolSFs().add(&tmpData);
					}

					if( STRACK != 0 ) {
						// L.3
						if( writer.putValAsList( 3 ) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						// 1. <U1 STRID1> 
						if( writer.putValAsInt1( ucStream ) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						// 2 <U1 STRACK> 
						if( writer.putValAsInt1( STRACK ) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						// 3. L,n
						if( writer.putValAsList( errorFunc.size() ) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						std::vector<XD_UINT1>::iterator ite = errorFunc.begin();
						for(; ite != errorFunc.end(); ite++) {
							if( writer.putValAsInt1( *ite ) == false ) {
								return XSEMI_GEM_ERR_OUT_BUFF;
							}	
						}
					}
				}
				errorFunc.clear();
			}
		}
	}


	// マスターを更新
	try {
		m_constData.updateMaster( &m_constData.get_SpoolSFs() );
	} catch(...) {
		// 65=否定、要求できる状態にない
		//erack = 65;
		//if( writer.putValAsBinary( &erack, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
		//return XSEMI_GEM_ERR_NON_TRANSACT_OV;
	}


	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

