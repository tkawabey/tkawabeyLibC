#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/xfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

int XSEMI_Gem::processGemMessageS1(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
//	XDATA_TYPE type;
	XData data;
	XDataWriter	writer( &pOutStream );
	switch( function ) {
	case 1:
		/*	S1,F2 (オンライン・データ)
			L,2
				1. <A[6] MDLN> * 装置の形式
				2. <A[6] SOFTREV> * ソフトウェアのレビジョン・コード


			i） ホストから送信するメッセージのフォーマット
				L,0
			ii） 装置から送信するメッセージのフォーマット
				L,2
					1. <A[6] MDLN> * 装置の形式
					2. <A[6] SOFTREV> * ソフトウェアのレビジョン・コード
		*/
		if( m_bHost == true ) {
			// ホストから送信
			if( writer.putValAsList( 0 ) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
		} else {
			// 装置から送信
			if( writer.putValAsList( 2 )               == false  ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			if( m_ulVIDs[EC_INDEX_MDLN] == (XD_UINT4)-1 ) {
				if( writer.putValAsStringA( "" )    == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			} else {
				XDATA_TYPE type;
				if( this->getVValue(m_ulVIDs[EC_INDEX_MDLN], &type, writer) != GETV_STS_SUCCESS ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}
			if( m_ulVIDs[EC_INDEX_SOFTREV] == (XD_UINT4)-1 ) {
				if( writer.putValAsStringA( "" )    == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			} else {
				XDATA_TYPE type;
				if( this->getVValue(m_ulVIDs[EC_INDEX_SOFTREV], &type, writer) != GETV_STS_SUCCESS ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}
		}
		return XSEMI_GEM_ERR_NON_TRANSACT_OV;
		break;
	case 2:
		{
			// S1F2のデータ構造チェック
			/*
				// ホストの場合
				構造：	i） ホストから送信するメッセージのフォーマット
							L,0
						ii） 装置から送信するメッセージのフォーマット
							L,2
								1. <A[6] MDLN> * 装置の形式
								2. <A[6] SOFTREV> * ソフトウェアのレビジョン・コード
			*/
			if( inData.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_DATA;
			}
			if(m_bHost == false) {
				if( data.getType() != XDATA_TYPE_L && data.getCount() != 0 ) {
					return XSEMI_GEM_ERR_DATA;
				}
			} else {
				if( data.getType() != XDATA_TYPE_L && data.getCount() != 2 ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( inData.getValue( &data ) == false || data.getType() != XDATA_TYPE_A ) {
					return XSEMI_GEM_ERR_DATA;
				}
				if( inData.getValue( &data ) == false || data.getType() != XDATA_TYPE_A ) {
					return XSEMI_GEM_ERR_DATA;
				}
			}
		}
		return XSEMI_GEM_ERR_NON_TRANSACT_DEF;
		break;
	case 3:
		/* 指定装置状態要 */
		{
			/*
				構造： L,n
					1. <U4 SVID1> * 状態変数ID
					:
					n. <U4 SVIDn>
				例外： 長さ0のリスト（n=0）の場合、予め定義されている順番（SVIDの番号の若い順
						数を要求する。
					通常、本メッセージでは状態変数のID（SVID）のみ使用可能だが、本装置で
					（DVVAL）や装置定数（ECV）の指定も可能とする。
					以下のS1,F3フォーマットも受信可能である。
					<U4 SVID SVID … SVID > * 状態変数ID
			*/
			/* S1,F3
				L,n
					1. <SV1> * 状態変数データ
					:
					n. <SVn>
				例外： n=0なら応答データが何もないことを意味する。
				SViのかわりに長さ0のアイテムが返された場合は、SVIDiが存在しないことを意
			
			*/
			XMutexer			locker(m_lock);


			// L.n
			if( inData.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			if( data.getCount() == 0 ) {

				XSEMI_Vs &vs = m_constData.get_Vs();
				// 有効なSVの数を数える
				XD_UINT4 ulNumOfSV = 0,
						 i = 0, 
						 loop = vs.countSV();

				for(i = 0; i < loop; i++) {
					XSEMI_SV* pSV = vs.getSV( i ) ;

					if( pSV->get_Enable() == true &&
						pSV->get_Private() == false) {	// 有効　＆＆　Public
						ulNumOfSV++;
					}
				}
				// L.n
				if( writer.putValAsList( ulNumOfSV )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
				for(i = 0; i < loop; i++) {
					XSEMI_SV* pSV = vs.getSV( i ) ;
					if( pSV->get_Enable() == true &&
						pSV->get_Private() == false) {	// 有効　＆＆　Public
						
						if( pSV->buildBinaryData( &pOutStream )  == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}			
					}
				}
			} else {
				XD_UINT4 ulTopListNum = data.getCount();
				// VIDのフォーマットを取得
				XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
				if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
					// フォーマットが不正
					return XSEMI_GEM_ERR_ABORT;
				}

				// L.n
				if( writer.putValAsList( data.getCount() )  == false ) {
					return 2;
				}
				for(XD_UINT4 i = 0; i < ulTopListNum; i++) {
					// U4 SVIDn
					if( inData.getValue( &data ) == false ) {
						return XSEMI_GEM_ERR_IN_DATA;
					}
					if( data.getType() != vidFormatType ) {
						return XSEMI_GEM_ERR_IN_DATA;
					}
					
					XSEMI_V* pSV = NULL;

					if( vidFormatType == XDATA_TYPE_A ) {
						XStrBuff strVName = data.getValAsStringA();
						// VNameから、検索する。
						pSV = m_constData.get_Vs().get_byName( strVName );
					} else {						
						if( data.getCount() != 1 ) {
							return XSEMI_GEM_ERR_IN_DATA;
						}
						XD_UINT4 ulSV =  data.getCastValAsUInt4();
						pSV = m_constData.get_Vs().get_SV_byID( ulSV );
					}
					
					if( pSV != NULL) {
						if( pSV->buildBinaryData( &pOutStream )  == false ) {
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
		}
		return 1;
		break;
	case 11:
		{
			// VIDのフォーマットを取得
			XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
			if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
				// フォーマットが不正
				return XSEMI_GEM_ERR_ABORT;
			}

			/*
			S1,F11 Status Variable Namelist Request (SVNR) S,H→E,返
				（状態変数名リスト要求）
				説明： ホストから装置に対する状態変数確認のための要求
				構造： L,n
						1. <U4 SVID1> * 状態変数ID
							:
						n. <U4 SVIDn>
				例外： 長さ0のリスト（n=0）の場合は、予め定義されている順番（SVIDの番号の若い順）ですべての状
						変数を要求する。
					通常、本メッセージでは状態変数のID（SVID）のみ使用可能だが、本装置ではデータ変数
					（DVVAL）や装置定数（ECV）の指定も可能とする。
				注意： 本メッセージにて使用できるSVIDは各スタンダード仕様書の「データ項目」のクラスがSVで示さ
						ているVIDである。
			
			S1,F12 Status Variable Namelist Reply (SVNRR） M,H←
			（状態変数名リスト応答）
				説明： 装置が要求された状態変数の名称と単位を報告する。
				構造： L,n
						1. L,3
							1. <U4 SVID1> * 状態変数ID
							2. <A SVNAME1> * 状態変数名
							3. <A UNITS1> * 単位
						:
						n. L,3
							1. <U4 SVIDn> * 状態変数ID
							2. <A SVNAMEn> * 状態変数名
							3. <A UNITSn> * 単位
				注意： 本メッセージにて使用できるSVIDは各スタンダード仕様書の「データ項目」のクラスがSVで示さ
					ているVIDである。
				例外： SVNAMEiとUNITSiの両方が長さ0のアイテムの場合、SVIDiが存在しないことを意味する。
			*/

			XMutexer			locker(m_lock);


			// L.n
			if( inData.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			if( data.getCount() == 0 ) {
				XSEMI_Vs &vs = m_constData.get_Vs();
				// 有効なSVの数を数える
				XD_UINT4 ulNumOfSV = 0,
						 i = 0, 
						 loop = vs.countSV();

				for(i = 0; i < loop; i++) {
					XSEMI_SV* pSV = vs.getSV( i ) ;

					if( pSV->get_Enable() == true &&
						pSV->get_Private() == false) {	// 有効　＆＆　Public
						ulNumOfSV++;
					}
				}


				// L.n
				if( writer.putValAsList( ulNumOfSV )  == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}

				for(i = 0; i < loop; i++) {
					XSEMI_SV* pSV = vs.getSV( i ) ;

					if( !( pSV->get_Enable() == true &&
						   pSV->get_Private() == false ) ) {	// 有効　＆＆　Public
						continue;
					}

					// L.3
					if( writer.putValAsList( 3 )  == false ) {
						return 2;
					}
					//   ** SVID
					if( this->innerWriteVID(vidFormatType, pSV, writer) == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					//   A SVNAME
					if( writer.putValAsStringA( pSV->get_Name() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					//   A UNITS
					if( writer.putValAsStringA( pSV->get_Unit() )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}						
				}
			} else {
				XD_UINT4 ulTopListNum = data.getCount();
				// L.n
				if( writer.putValAsList( data.getCount() )  == false ) {
					return 2;
				}
				for(XD_UINT4 i = 0; i < ulTopListNum; i++) {
					// U4 SVIDn
					if( inData.getValue( &data ) == false ) {
						return XSEMI_GEM_ERR_ABORT;
					}
					if( data.getType() != XDATA_TYPE_L ) {
						return XSEMI_GEM_ERR_IN_DATA;
					}
					if( data.getCount() != 1 ) {
						return XSEMI_GEM_ERR_IN_DATA;
					}
					// L.3
					if( writer.putValAsList( 3 )  == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}


					XSEMI_V* pSV = NULL;

					if( vidFormatType == XDATA_TYPE_A ) {
						XStrBuff strVName = data.getValAsStringA();
						// VNameから、検索する。
						pSV = m_constData.get_Vs().get_byName( strVName );
					} else {						
						if( data.getCount() != 1 ) {
							return XSEMI_GEM_ERR_IN_DATA;
						}
						XD_UINT4 ulSV =  data.getCastValAsUInt4();
						pSV = m_constData.get_Vs().get_SV_byID( ulSV );
					}
					
					if( pSV != NULL) {
						//   xx SVID
						if( this->innerWriteVID(vidFormatType, pSV, writer) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						//   A SVNAME
						if( writer.putValAsStringA( pSV->get_Name() )  == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						//   A UNITS
						if( writer.putValAsStringA( pSV->get_Unit() )  == false ) {
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
						//   A SVNAME
						if( writer.putValAsStringA( "" )  == false ) {
							return 2;
						}
						//   A UNITS
						if( writer.putValAsStringA( "" )  == false ) {
							return 2;
						}
					}

				}
			}
		}
		return 1;
		break;
	case 13:
		/*
			S1,F13 Establish Communications Request (CR)
				i） ホストから送信するメッセージのフォーマット
					L,0
				ii） 装置から送信するメッセージのフォーマット
					L,2
						1. <A[6] MDLN> * 装置の形式
						2. <A[6] SOFTREV> * ソフトウェアのレ		
		  
			S1,F4 (オンライン・データ)
			i） ホストから送信するメッセージのフォーマット
				L,2
					1. <B[1] COMMACK> * 通信確立確認コード
					2. L,0
			ii） 装置から送信するメッセージのフォーマット
				L,2
					1. <B[1] COMMACK> * 通信確立確認コード
					2. L,2
						1. <A[6] MDLN> * 装置の形式
						2. <A[6] SOFTREV> * ソフトウェアのレビジョン・コード

				COMMACK：	0=了解
							1=否定
							2-63 保留
		*/
		if( m_bHost == true ) {
			//
			// 装置から受信したデータの解析
			//

			// L.2
			if( inData.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			if( data.getCount() != 2 ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}

			// <A[6] MDLN>
			if( inData.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			if( data.getType() != XDATA_TYPE_A ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			// <A[6] SOFTREV>
			if( inData.getValue( &data ) == false ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}
			if( data.getType() != XDATA_TYPE_A ) {
				return XSEMI_GEM_ERR_IN_DATA;
			}


			unsigned char commandack = 0;
			//
			// ホストから送信するデータを作成
			//
			if( writer.putValAsList( 2 )                == false ||
				writer.putValAsBinary( &commandack, 1 ) == false ||
				writer.putValAsList( 0 )                == false) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}


			if( m_emCommState == COMM_STS_WAIT_CR_FROM_HOST ) {
				//　通信状態の状態遷移を通知
				innerCommunicationStateChange(COMM_STS_COMUNICATION);
			}
		} else {
			// 装置から送信
			
			// モデル図の、　(11) Host OFF-LINE -> ON-LINE
			/*
				COMMACK：	0=了解
							1=否定
							2-63 保留
			*/
			unsigned char commandack = 0;

			switch( m_ulControlStatus ) {
			case XSEMI_GEM_CTRL_STS_OFFLINE_HOST:
			case XSEMI_GEM_CTRL_STS_ONLINE_LOCAL:
			case XSEMI_GEM_CTRL_STS_ONLINE_REMOTE:

				break;
			default:
				commandack = 1;
				break;
			}
			if( m_ulControlStatus != XSEMI_GEM_CTRL_STS_OFFLINE_HOST ) {
				
			}
			// ホストから送信するデータを作成
			if( writer.putValAsList( 2 )                == false ||
				 writer.putValAsBinary( &commandack, 1 ) == false ||
				 writer.putValAsList( 2 )                == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			if( m_ulVIDs[EC_INDEX_MDLN] == (XD_UINT4)-1 ) {
				if( writer.putValAsStringA( "" )    == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			} else {
				XDATA_TYPE type;
				if( this->getVValue(m_ulVIDs[EC_INDEX_MDLN], &type, writer) != GETV_STS_SUCCESS ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}

			if( m_ulVIDs[EC_INDEX_SOFTREV] == (XD_UINT4)-1 ) {
				if( writer.putValAsStringA( "" )    == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			} else {
				XDATA_TYPE type;
				if( this->getVValue(m_ulVIDs[EC_INDEX_SOFTREV], &type, writer) != GETV_STS_SUCCESS ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}

			
			
			// コントロール状態の変更
			if( commandack == 0 ) {
				// コントロール状態の変化を通知
				innerNotifyContorolState( XSEMI_GEM_CTRL_STS_ONLINE_REMOTE );
			}
		}
		return 1;
		break;
	case 15:
		{
			// S1,F16 OFF-LINE Acknowledge (OFLA) S,H←E

			// 構造： <B[1] OFLACK> * オフライン要求に対する確認コード
			// 		OFLACK：	0=オフライン了解
			// 					1-63 保留
			// 					64=オフラインを不許可

			if( m_bHost == false ) {
				unsigned char ofack = 64;
				if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_LOCAL || 
					m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_REMOTE ) {
					ofack = 0;
				}

				// ホストから送信
				if( writer.putValAsBinary( &ofack, 1 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}

				// コントロール状態の変更
				if( ofack == 0 ) {
					// コントロール状態の変化を通知
					innerNotifyContorolState( XSEMI_GEM_CTRL_STS_OFFLINE_HOST );
				}

				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
		}
		break;
	case 17:
		// S1,F17 Request ON-LINE (RONL) S,H→E,返信

		// S1,F18 ON-LINE Acknowledge (ONLA) S,H←E
		// 構造： <B[1] ONLACK> * オンライン要求に対する確認コード
		// 		ONLACK：	0=オンライン了解
		// 					1=オンラインを不許可
		// 					2=装置はすでにオンライン
		// 					3-63 保留		
		{
			unsigned char onlack = 0;
			if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_LOCAL ||
				m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_REMOTE) {
				onlack = 2;
			}
			// ホストから送信
			if( writer.putValAsBinary( &onlack, 1 ) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}

			
			// コントロール状態の変更
			if( onlack == 0 ) {
				// コントロール状態の変化を通知
				innerNotifyContorolState( XSEMI_GEM_CTRL_STS_ONLINE_REMOTE );
			}
			return 1;		
		}
		break;
	case 21:
		return processGemMessageS1F21(inData, pOutStream);
		break;
	case 22:
		return processGemMessageS1F22(inData, pOutStream);
		break;
	case 23:
		return processGemMessageS1F23(inData, pOutStream);
		break;
	case 65:
		return processGemMessageS1F65(inData, pOutStream);
		break;
	}
	return XSEMI_GEM_ERR_HEAD_FUNCTION;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS1F21(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
		S1F22
		<L.n
		  <L.3
		    <VID>
            <A > // Name
            <A > // Unit
		  >
		  ....
	*/
	XData data;
	XDataWriter	writer( &pOutStream );
	XD_UINT4 total_cnt = m_constData.get_Vs().count();
	XD_UINT4 enable_cnt = 0;
	XD_UINT4 i = 0;

	// 有効なVの数を数える。
	for( i = 0; i < total_cnt; i++ ) {
		XSEMI_V* pV = m_constData.get_Vs().get( i );
		if( pV->get_Private() != XSEMI_V::PRIVAL_NON ) {
			enable_cnt++;
		}
	}

	// 2次メッセージのデータを構築
	// L.n
	writer.putValAsList(enable_cnt);

	for( i = 0; i < total_cnt; i++ ) {
		XSEMI_V* pV = m_constData.get_Vs().get( i );
		if( pV->get_Private() != XSEMI_V::PRIVAL_NON ) {
			// L.3
			writer.putValAsList( 3 );
			writer.putValAsUInt4( pV->get_VID() );
			writer.putValAsStringA( pV->get_Name() );
			writer.putValAsStringA( pV->get_Unit() );
		}
	}

	return XSEMI_GEM_ERR_HEAD_FUNCTION;
XEXCEPT_EXIT
}
int XSEMI_Gem::processGemMessageS1F22(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	// 登録されているVの情報と比較して、ログに出す。
	XData data;
	XD_UINT4 data_cnt = 0;
	XD_UINT4 i = 0;

	if( ( inData.getValue( &data ) == true ) && data.getType() == XDATA_TYPE_L ) {
		return XSEMI_GEM_ERR_IN_DATA;
	}
	data_cnt = data.getCount();


	for( i = 0; i < data_cnt; i++ ) {
		if( ( inData.getValue( &data ) == true ) && data.getType() == XDATA_TYPE_L ) {
			return XSEMI_GEM_ERR_IN_DATA;
		}
		if( data.getCount() != 3 ) {
			return XSEMI_GEM_ERR_IN_DATA;
		}
		// VID
		if( ( inData.getValue( &data ) == true ) 
				&& 
			data.getType() == XDATA_TYPE_U4 ) {
			return XSEMI_GEM_ERR_IN_DATA;
		}
		XD_UINT4 vid = data.getCastValAsUInt4();

		XSEMI_V* pV = m_constData.get_Vs().get_byID( vid );
		if( pV == NULL ) {
			innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("VID:(%d) dose not regist."), vid);
		}

		// Name
		inData.getValue( &data );
		// Unit
		inData.getValue( &data );
	}

	return XSEMI_GEM_ERR_HEAD_FUNCTION;
XEXCEPT_EXIT
}
int XSEMI_Gem::processGemMessageS1F23(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY

	return XSEMI_GEM_ERR_HEAD_FUNCTION;
XEXCEPT_EXIT
}
int XSEMI_Gem::processGemMessageS1F65(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	/*
		The Equipment sends this message using the following format:
		S1F65 W * H<-E
		<L
			<A MDLN>
			<A SOFTREV>
		> .
		Format 1: The Host sends this message using the following format:
		S1F65 W * H->E
		<L> .
		Format 2: The Equipment also accepts the following form of this message:
		S1F65 W . * H->E
	*/
	/*
		If the Host sends S1F65, Format 1, the Equipment replies S1F66 using the following format:
		S1F66 * H<-E
		<L
			<B [1] 00> * COMMACK
			<L
				<A MDLN>
				<A SOFTREV>
			>
		> .
		If the Host sends S1F65, format 2, the Equipment replies S1F66 using the following format:
		S1F66 * H<-E
		<B [1] 00> . * COMMACK
		The Host sends this message using the following format:
		S1F66 * H->E
		<L
			<B [1] 00>
			<L>
		> .
		The Equipment also accepts the following form of this message.
		S1F66 * H->E
		<B [1] 00> . * COMMACK
	*/
	XData data;
	XDataWriter	writer( &pOutStream );
	unsigned char commack = 0;
	if( m_bHost == true ) {
		// ホストの場合
		int format = 0;
		if( ( inData.getValue( &data ) == true ) && data.getType() == XDATA_TYPE_L ) {
			format = 1;
		}

		//
		// ホストから送信するデータを作成
		//
		if( writer.putValAsList( 2 )                == false ||
			writer.putValAsBinary( &commack, 1 )	== false ||
			writer.putValAsList( 0 )                == false) {
			return XSEMI_GEM_ERR_OUT_BUFF;
		}
	} else {
		// 装置の場合
		int format = 0;
		if( ( inData.getValue( &data ) == true ) && data.getType() == XDATA_TYPE_L ) {
			format = 1;
		}
		if( format == 0 ) {
			if( writer.putValAsBinary(&commack, 1) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
		} else {
			// ホストから送信するデータを作成
			if( writer.putValAsList( 2 )					  == false ||
				 writer.putValAsBinary( &commack, 1 )		  == false ||
				 writer.putValAsList( 2 )					  == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
			if( m_ulVIDs[EC_INDEX_MDLN] == (XD_UINT4)-1 ) {
				if( writer.putValAsStringA( "" )    == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			} else {
				XDATA_TYPE type;
				if( this->getVValue(m_ulVIDs[EC_INDEX_MDLN], &type, writer) != GETV_STS_SUCCESS ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}
			if( m_ulVIDs[EC_INDEX_SOFTREV] == (XD_UINT4)-1 ) {
				if( writer.putValAsStringA( "" )    == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			} else {
				XDATA_TYPE type;
				if( this->getVValue(m_ulVIDs[EC_INDEX_SOFTREV], &type, writer) != GETV_STS_SUCCESS ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}
			}
		}
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}
