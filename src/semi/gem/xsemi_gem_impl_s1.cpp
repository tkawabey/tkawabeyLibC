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
		/*	S1,F2 (�I�����C���E�f�[�^)
			L,2
				1. <A[6] MDLN> * ���u�̌`��
				2. <A[6] SOFTREV> * �\�t�g�E�F�A�̃��r�W�����E�R�[�h


			i�j �z�X�g���瑗�M���郁�b�Z�[�W�̃t�H�[�}�b�g
				L,0
			ii�j ���u���瑗�M���郁�b�Z�[�W�̃t�H�[�}�b�g
				L,2
					1. <A[6] MDLN> * ���u�̌`��
					2. <A[6] SOFTREV> * �\�t�g�E�F�A�̃��r�W�����E�R�[�h
		*/
		if( m_bHost == true ) {
			// �z�X�g���瑗�M
			if( writer.putValAsList( 0 ) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
		} else {
			// ���u���瑗�M
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
			// S1F2�̃f�[�^�\���`�F�b�N
			/*
				// �z�X�g�̏ꍇ
				�\���F	i�j �z�X�g���瑗�M���郁�b�Z�[�W�̃t�H�[�}�b�g
							L,0
						ii�j ���u���瑗�M���郁�b�Z�[�W�̃t�H�[�}�b�g
							L,2
								1. <A[6] MDLN> * ���u�̌`��
								2. <A[6] SOFTREV> * �\�t�g�E�F�A�̃��r�W�����E�R�[�h
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
		/* �w�葕�u��ԗv */
		{
			/*
				�\���F L,n
					1. <U4 SVID1> * ��ԕϐ�ID
					:
					n. <U4 SVIDn>
				��O�F ����0�̃��X�g�in=0�j�̏ꍇ�A�\�ߒ�`����Ă��鏇�ԁiSVID�̔ԍ��̎Ⴂ��
						����v������B
					�ʏ�A�{���b�Z�[�W�ł͏�ԕϐ���ID�iSVID�j�̂ݎg�p�\�����A�{���u��
					�iDVVAL�j�⑕�u�萔�iECV�j�̎w����\�Ƃ���B
					�ȉ���S1,F3�t�H�[�}�b�g����M�\�ł���B
					<U4 SVID SVID �c SVID > * ��ԕϐ�ID
			*/
			/* S1,F3
				L,n
					1. <SV1> * ��ԕϐ��f�[�^
					:
					n. <SVn>
				��O�F n=0�Ȃ牞���f�[�^�������Ȃ����Ƃ��Ӗ�����B
				SVi�̂����ɒ���0�̃A�C�e�����Ԃ��ꂽ�ꍇ�́ASVIDi�����݂��Ȃ����Ƃ���
			
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
				// �L����SV�̐��𐔂���
				XD_UINT4 ulNumOfSV = 0,
						 i = 0, 
						 loop = vs.countSV();

				for(i = 0; i < loop; i++) {
					XSEMI_SV* pSV = vs.getSV( i ) ;

					if( pSV->get_Enable() == true &&
						pSV->get_Private() == false) {	// �L���@�����@Public
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
						pSV->get_Private() == false) {	// �L���@�����@Public
						
						if( pSV->buildBinaryData( &pOutStream )  == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}			
					}
				}
			} else {
				XD_UINT4 ulTopListNum = data.getCount();
				// VID�̃t�H�[�}�b�g���擾
				XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
				if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
					// �t�H�[�}�b�g���s��
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
						// VName����A��������B
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
						// V��������Ȃ��ꍇ�́AL.0�ŕԂ�
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
			// VID�̃t�H�[�}�b�g���擾
			XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
			if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
				// �t�H�[�}�b�g���s��
				return XSEMI_GEM_ERR_ABORT;
			}

			/*
			S1,F11 Status Variable Namelist Request (SVNR) S,H��E,��
				�i��ԕϐ������X�g�v���j
				�����F �z�X�g���瑕�u�ɑ΂����ԕϐ��m�F�̂��߂̗v��
				�\���F L,n
						1. <U4 SVID1> * ��ԕϐ�ID
							:
						n. <U4 SVIDn>
				��O�F ����0�̃��X�g�in=0�j�̏ꍇ�́A�\�ߒ�`����Ă��鏇�ԁiSVID�̔ԍ��̎Ⴂ���j�ł��ׂĂ̏�
						�ϐ���v������B
					�ʏ�A�{���b�Z�[�W�ł͏�ԕϐ���ID�iSVID�j�̂ݎg�p�\�����A�{���u�ł̓f�[�^�ϐ�
					�iDVVAL�j�⑕�u�萔�iECV�j�̎w����\�Ƃ���B
				���ӁF �{���b�Z�[�W�ɂĎg�p�ł���SVID�͊e�X�^���_�[�h�d�l���́u�f�[�^���ځv�̃N���X��SV�Ŏ���
						�Ă���VID�ł���B
			
			S1,F12 Status Variable Namelist Reply (SVNRR�j M,H��
			�i��ԕϐ������X�g�����j
				�����F ���u���v�����ꂽ��ԕϐ��̖��̂ƒP�ʂ�񍐂���B
				�\���F L,n
						1. L,3
							1. <U4 SVID1> * ��ԕϐ�ID
							2. <A SVNAME1> * ��ԕϐ���
							3. <A UNITS1> * �P��
						:
						n. L,3
							1. <U4 SVIDn> * ��ԕϐ�ID
							2. <A SVNAMEn> * ��ԕϐ���
							3. <A UNITSn> * �P��
				���ӁF �{���b�Z�[�W�ɂĎg�p�ł���SVID�͊e�X�^���_�[�h�d�l���́u�f�[�^���ځv�̃N���X��SV�Ŏ���
					�Ă���VID�ł���B
				��O�F SVNAMEi��UNITSi�̗���������0�̃A�C�e���̏ꍇ�ASVIDi�����݂��Ȃ����Ƃ��Ӗ�����B
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
				// �L����SV�̐��𐔂���
				XD_UINT4 ulNumOfSV = 0,
						 i = 0, 
						 loop = vs.countSV();

				for(i = 0; i < loop; i++) {
					XSEMI_SV* pSV = vs.getSV( i ) ;

					if( pSV->get_Enable() == true &&
						pSV->get_Private() == false) {	// �L���@�����@Public
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
						   pSV->get_Private() == false ) ) {	// �L���@�����@Public
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
						// VName����A��������B
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
							// �t�H�[�}�b�g���s��
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
				i�j �z�X�g���瑗�M���郁�b�Z�[�W�̃t�H�[�}�b�g
					L,0
				ii�j ���u���瑗�M���郁�b�Z�[�W�̃t�H�[�}�b�g
					L,2
						1. <A[6] MDLN> * ���u�̌`��
						2. <A[6] SOFTREV> * �\�t�g�E�F�A�̃�		
		  
			S1,F4 (�I�����C���E�f�[�^)
			i�j �z�X�g���瑗�M���郁�b�Z�[�W�̃t�H�[�}�b�g
				L,2
					1. <B[1] COMMACK> * �ʐM�m���m�F�R�[�h
					2. L,0
			ii�j ���u���瑗�M���郁�b�Z�[�W�̃t�H�[�}�b�g
				L,2
					1. <B[1] COMMACK> * �ʐM�m���m�F�R�[�h
					2. L,2
						1. <A[6] MDLN> * ���u�̌`��
						2. <A[6] SOFTREV> * �\�t�g�E�F�A�̃��r�W�����E�R�[�h

				COMMACK�F	0=����
							1=�ے�
							2-63 �ۗ�
		*/
		if( m_bHost == true ) {
			//
			// ���u�����M�����f�[�^�̉��
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
			// �z�X�g���瑗�M����f�[�^���쐬
			//
			if( writer.putValAsList( 2 )                == false ||
				writer.putValAsBinary( &commandack, 1 ) == false ||
				writer.putValAsList( 0 )                == false) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}


			if( m_emCommState == COMM_STS_WAIT_CR_FROM_HOST ) {
				//�@�ʐM��Ԃ̏�ԑJ�ڂ�ʒm
				innerCommunicationStateChange(COMM_STS_COMUNICATION);
			}
		} else {
			// ���u���瑗�M
			
			// ���f���}�́A�@(11) Host OFF-LINE -> ON-LINE
			/*
				COMMACK�F	0=����
							1=�ے�
							2-63 �ۗ�
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
			// �z�X�g���瑗�M����f�[�^���쐬
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

			
			
			// �R���g���[����Ԃ̕ύX
			if( commandack == 0 ) {
				// �R���g���[����Ԃ̕ω���ʒm
				innerNotifyContorolState( XSEMI_GEM_CTRL_STS_ONLINE_REMOTE );
			}
		}
		return 1;
		break;
	case 15:
		{
			// S1,F16 OFF-LINE Acknowledge (OFLA) S,H��E

			// �\���F <B[1] OFLACK> * �I�t���C���v���ɑ΂���m�F�R�[�h
			// 		OFLACK�F	0=�I�t���C������
			// 					1-63 �ۗ�
			// 					64=�I�t���C����s����

			if( m_bHost == false ) {
				unsigned char ofack = 64;
				if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_LOCAL || 
					m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_REMOTE ) {
					ofack = 0;
				}

				// �z�X�g���瑗�M
				if( writer.putValAsBinary( &ofack, 1 ) == false ) {
					return XSEMI_GEM_ERR_OUT_BUFF;
				}

				// �R���g���[����Ԃ̕ύX
				if( ofack == 0 ) {
					// �R���g���[����Ԃ̕ω���ʒm
					innerNotifyContorolState( XSEMI_GEM_CTRL_STS_OFFLINE_HOST );
				}

				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
		}
		break;
	case 17:
		// S1,F17 Request ON-LINE (RONL) S,H��E,�ԐM

		// S1,F18 ON-LINE Acknowledge (ONLA) S,H��E
		// �\���F <B[1] ONLACK> * �I�����C���v���ɑ΂���m�F�R�[�h
		// 		ONLACK�F	0=�I�����C������
		// 					1=�I�����C����s����
		// 					2=���u�͂��łɃI�����C��
		// 					3-63 �ۗ�		
		{
			unsigned char onlack = 0;
			if( m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_LOCAL ||
				m_ulControlStatus == XSEMI_GEM_CTRL_STS_ONLINE_REMOTE) {
				onlack = 2;
			}
			// �z�X�g���瑗�M
			if( writer.putValAsBinary( &onlack, 1 ) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}

			
			// �R���g���[����Ԃ̕ύX
			if( onlack == 0 ) {
				// �R���g���[����Ԃ̕ω���ʒm
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

	// �L����V�̐��𐔂���B
	for( i = 0; i < total_cnt; i++ ) {
		XSEMI_V* pV = m_constData.get_Vs().get( i );
		if( pV->get_Private() != XSEMI_V::PRIVAL_NON ) {
			enable_cnt++;
		}
	}

	// 2�����b�Z�[�W�̃f�[�^���\�z
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
	// �o�^����Ă���V�̏��Ɣ�r���āA���O�ɏo���B
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
		// �z�X�g�̏ꍇ
		int format = 0;
		if( ( inData.getValue( &data ) == true ) && data.getType() == XDATA_TYPE_L ) {
			format = 1;
		}

		//
		// �z�X�g���瑗�M����f�[�^���쐬
		//
		if( writer.putValAsList( 2 )                == false ||
			writer.putValAsBinary( &commack, 1 )	== false ||
			writer.putValAsList( 0 )                == false) {
			return XSEMI_GEM_ERR_OUT_BUFF;
		}
	} else {
		// ���u�̏ꍇ
		int format = 0;
		if( ( inData.getValue( &data ) == true ) && data.getType() == XDATA_TYPE_L ) {
			format = 1;
		}
		if( format == 0 ) {
			if( writer.putValAsBinary(&commack, 1) == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}
		} else {
			// �z�X�g���瑗�M����f�[�^���쐬
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
