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
		S2,F13 Equipment Constant Request (ECR) S,H��E,�ԐM
	
		�����F �w�肵�����݂̑��u�萔�̗v�����s���B
		�\���F L,n
				1. <U4 ECID1> * ���u�萔ID
				:
				n. <U4 ECIDn>
		��O�F ����0�̃��X�g�in=0�j�̏ꍇ�́A�\�ߒ�`����Ă��鏇�ԁiECID�̔ԍ��̎Ⴂ���j�ł��ׂĂ̒萔	��v������B
		�ʏ�A�{���b�Z�[�W�ł͑��u�萔�iECID�j�̂ݎg�p�\�����A�{���u�ł͏�ԕϐ��iSVID�j��f�[�^�ϐ��iDVVAL�j�̎w����\�Ƃ���B
		�܂��A�ȉ���S2,F13�t�H�[�}�b�g����M�\�ł���B
		<U4 ECID1 ECID2 .... ECIDn> * ���u�萔ID
		���ӁF �{���b�Z�[�W�ɂĎg�p�ł���ECID�͊e�X�^���_�[�h�d�l���́u�f�[�^���ځv�̃N���X��ECV�Ŏ�����Ă���VID�ł���B
		
		  
		S2,F14 Equipment Constant Data (ECD) M,H��E

		�����F S2,F13�ɂėv�����ꂽ���Ԃő��u�萔����������B
		�\���F L,n
				1. <ECV1> * ���u�萔
				:
				n. <ECVn>
		��O�F n=0�̏ꍇ�́A�����萔���Ȃ����Ƃ��Ӗ�����B
		ECVi�̂����ɒ���0�̃A�C�e�����Ԃ��ꂽ�ꍇ�́A�w�肳�ꂽECIDi�����݂��Ȃ����Ƃ��Ӗ�����B
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
		// �L����SV�̐��𐔂���
		XD_UINT4 ulNumOfSV = 0,
				 i = 0, 
				 loop = vs.countECV();

		for(i = 0; i < loop; i++) {
			XSEMI_ECV* pECV = vs.getECV( i ) ;

			if( pECV->get_Enable() == true &&
				pECV->get_Private() == false) {	// �L���@�����@Public
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
				   pECV->get_Private() == false ) ) {	// �L���@�����@Public
				continue;
			}


			if( pECV->buildBinaryData( &pOutStream )  == false ) {
				return XSEMI_GEM_ERR_OUT_BUFF;
			}					
		}
	} else {
		// VID�̃t�H�[�}�b�g���擾
		XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
		if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
			// �t�H�[�}�b�g���s��
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
				// VName����A��������B
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
				// V��������Ȃ��ꍇ�́AL.0�ŕԂ�
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
		S2,F15 New Equipment Constant Send (ECS) S,H��E,�ԐM
		�����F �w�肵�����u�萔�̕ύX��v������B
		�\���F L,n
				1. L,2
					1. <xx ECID1> * ���u�萔ID
					2. <ECV1> * ���u�萔
				:
				n. L,2
					1. <xx ECIDn>
					2. <ECVn> * ���u�萔
				2. <ECVn>
		���ӁF	1. �{���b�Z�[�W�ɂĎg�p�ł���ECID�͊e�X�^���_�[�h�d�l���́u�f�[�^���ځv�̃N���X��ECV�Ŏ�����Ă���VID�ł���B
					���u�萔�iECID�j�݂̂��g�p�\�ł���B
				2. IDLE��Ԃł̂ݗL���ł���B
				3. ��M����ECV�̃t�H�[�}�b�g�Ɋւ��āA�����O�X�̈Ⴂ�͖������ď�������B�Ⴆ�΁AU2�t�H�[�}�b�g�ł���ׂ��A�C�e���ɑ΂��āAU1�t�H�[�}�b�g�Ŏw�肳��Ă��G���[�Ƃ�������ɏ����\�Ƃ���B�������AU2�t�H�[�}�b�g�ł���ׂ��A�C�e���ɑ΂��āAI2�t�H�[�}�b�g�̎w��̓G���[�Ƃ���B



		S2,F16 New Equipment Constant Acknowledge (ECA) S,H��E
		�����F S2,F15�ɂėv�����ꂽ���u�萔�ύX�̉������s���B
		�\���F <B[1] EAC>
				EAC�F ���u�m�F�R�[�h
					0=����
					1=�ے�A���Ȃ��Ƃ���̒萔�����݂��Ȃ�
					2=�ے�A�r�W�[
					3=�ے�A���Ȃ��Ƃ���̒萔���͈͊O�ɂ���
					4-63 �ۗ�
					64=�ے�A���Ȃ��Ƃ���̒萔�̃t�H�[�}�b�g���قȂ�
					255=���̑��̃G���[
		���ӁF �w�肳�ꂽVID�̂ЂƂł����݂��Ȃ��ꍇ�ɂ́AS2,F15�Ŏw�肳�ꂽ���ׂĂ̐ݒ�v�������ۂ���B
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
		// VID�̃t�H�[�}�b�g���擾
		XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
		if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
			// �t�H�[�}�b�g���s��
			return XSEMI_GEM_ERR_ABORT;
		}


		inData.getRaw(&pRawData, &iRawSize);

		//
		//	�f�[�^�̃`�F�b�N(i=0) & �X�V(i=1)
		//
		for(int i = 0; i < 2; i++) {
			// EC�����邩���ׂ�
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
					// VName����A��������B
					pEC = m_constData.get_Vs().get_byName( strVName );
				} else {						
					if( data.getCount() != 1 ) {
						return XSEMI_GEM_ERR_IN_DATA;
					}
					XD_UINT4 ulECID =  data.getCastValAsUInt4();
					pEC = m_constData.get_Vs().get_EC_byID( ulECID );
				}
				

				if( pEC == NULL ) {
					// 1=�ے�A���Ȃ��Ƃ���̒萔�����݂��Ȃ�
					eca = 1;

					// �z�X�g���瑗�M
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
					// 64=�ے�A���Ȃ��Ƃ���̒萔�̃t�H�[�}�b�g���قȂ�
					eca = 64;

					// �z�X�g���瑗�M
					if( writer.putValAsBinary( &eca, 1 ) == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}
				if( i == 0 ) {
					// �f�[�^�̃`�F�b�N
					if( pEC->check( data ) != 0 ) {
						// 3=�ے�A���Ȃ��Ƃ���̒萔���͈͊O�ɂ���
						eca = 3;

						// �z�X�g���瑗�M
						if( writer.putValAsBinary( &eca, 1 ) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}
				} else {
					// �X�V
					if( pEC->putValueFromXData(readerChk, data) == false ) {
						// 255=���̑��̃G���[
						eca = 255;

						// �z�X�g���瑗�M
						if( writer.putValAsBinary( &eca, 1 ) == false ) {
							return XSEMI_GEM_ERR_OUT_BUFF;
						}
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}
				}
			}
		}
		// 0=����
		eca = 0;
		// �z�X�g���瑗�M
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

		S2,F18 Date and Time Data (DTD) S,H��E
		�\���F <A[12 or 16] TIME> * ����
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

	// �f���^���Ԃ��Z�b�g����B
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
    // XSEMI_Gem::receiveDataMessageCallBack�Ŋ��ɐݒ�ς݂Ȃ̂ŁA�Z�b�g���Ȃ��B
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
		S2,F29 Equipment Constant Namelist Request (ECNR) S,H��E,�ԐM

		�����F �z�X�g�����u���ŗL���ȑ��u�萔�Ɋւ����{�I�ȏ���v������B
		�\���F L,n
				1. <xx ECID1> * ���u�萔ID
				:
				n. <xx ECIDn>
		��O�F ����0���X�g�in=0�j�̏ꍇ�́A���ׂĂ̒萔��v������B���̂Ƃ��AS2,F30�ł�ECID�̏��Ԃł��ׂĂ̑��u�萔�̑�����񍐂���B
		���ӁF �{���b�Z�[�W�ɂĎg�p�ł���ECID�͊e�X�^���_�[�h�d�l���́u�f�[�^���ځv�̃N���X��ECV�Ŏ�����Ă���VID�ł���B

		S2,F30 Equipment Constant Namelist (ECN) M,H��E

		�����F S2,F29�̗v���ɑ΂��ĉ�������B
		�\���F L,n
				1. L,6
					1. <xx ECID1> * ���u�萔
					2. <A ECNAME1> * ���u�萔��
					3. < ECMIN1> * ���u�萔�ŏ��l
					4. < ECMAX1> * ���u�萔�ő�l
					5. < ECDEF1> * ���u�萔�f�t�H���g�l
					6. <A UNITS1> * �P��
				:
				n. L,6
					1. <U4 ECIDn>
					2. <A ECNAMEn>
					3. < ECMINn>
					4. < ECMAXn>
					5. < ECDEFn>
					6. <A UNITSn>
		���ӁF �P�ʂ��Ȃ��ꍇ�AUNITS�̓[��������O�X�ŕ񍐂���B
		��O�F ���݂��Ȃ�ECID��v�����ꂽ�ꍇ�́AECNAME�AECMIN�AECMAX�AECDEF�AUNIT��ASCII�̃[��������O�X�ɂĉ�������B
	*/
	XData data;
	XDataWriter	writer( &pOutStream );
	XMutexer			locker(m_lock);
	XRichData* pRData = NULL;
	// VID�̃t�H�[�}�b�g���擾
	XDATA_TYPE vidFormatType = XDATA_TYPE_U4;
	if( get_XXID_Format(EC_INDEX_FORMAT_VID, &vidFormatType) == false ) {
		// �t�H�[�}�b�g���s��
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
		// �L����SV�̐��𐔂���
		XD_UINT4 ulNumOfECV = 0,
				 i = 0, 
				 loop = vs.countECV();

		for(i = 0; i < loop; i++) {
			XSEMI_ECV* pECV = vs.getECV( i ) ;

			if( pECV->get_Enable() == true &&
				pECV->get_Private() == false) {	// �L���@�����@Public
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
					pECV->get_Private() == false ) ) {	// �L���@�����@Public
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
				// VName����A��������B
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
					// �t�H�[�}�b�g���s��
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

		�����F ���t����ю����Z�b�g�̗�������������B
		�\���F <B[1] TIACK> * ���Ԋm�F�R�[�h
					TIACK:	0=OK
							1=�G���[�A��������Ȃ�
							2-63 �ۗ�			
	*/

	XData data;
	XDataWriter	writer( &pOutStream );
	unsigned char tiack = 0;

	// �z�X�g���瑗�M
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
		S2,F33 Define Report (DR) M,H��E,�ԐM

		�\���F L,2
				1. <U4 DATAID> * �f�[�^ID
				2. L,a
					1. L,2
						1. <U4 RPTID1> * ���|�[�gID
						2. L,b
							1. <U4 VID1> * �ϐ�ID
							:
							b. <U4 VIDb>
					:
					a. L,2
						1. <U4 RPTIDa>
						2. L,c
							1. <U4 VID1>
							:
							c. <U4 VID


		S2,F34 Define Report Acknowledge (DRA) S,H��E
		�\���F <B[1] DRACK> * ��`����񍐃R�[�h
				DRACK�F 0=����
						1=�ے�A�X�y�[�X�s�\��
						2=�ے�A�����t�H�[�}�b�g
						3=�ے�A���Ȃ��Ƃ����RPTID�͂��łɒ�`����Ă���
						4=�ے�A���Ȃ��Ƃ����VID�����݂��Ȃ�
						5-63 �ۗ�
						65=�ے�A�v���ł����ԂɂȂ�
						66=���݂��Ȃ�RPTID���w�肳�ꂽ
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
		std::vector<XSEMI_Rpt*>		rpts;			// �X�V���郌�|�[�gID
		// **ID�̃t�H�[�}�b�g���擾
		XDATA_TYPE					vidFormatType    = XDATA_TYPE_U4;
		XDATA_TYPE					dataIdFormatType = XDATA_TYPE_U4;
		XDATA_TYPE					rtpIdFormatType  = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_VID,    &vidFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_DATAID, &dataIdFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_RPTID,  &rtpIdFormatType) == false ) {
			// �t�H�[�}�b�g���s��
			return XSEMI_GEM_ERR_ABORT;
		}

		inData.getRaw(&pRawData, &iRawSize);

		//
		//	�f�[�^�̃`�F�b�N(i=0) & �X�V(i=1)
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
				// 2=�ے�A�����t�H�[�}�b�g
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
				// 1. <DATAID>�ɑ�������0�̃��X�g�ia=0�j�́A���ׂẴ��|�[�g�K�肨��ъ֘A�����N����������B
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
				// �z�X�g���瑗�M
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
							// VName����A��������B
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
						// �Z�b�g����V�̐�
						XD_UINT4 ulVCount = data.getCount();

						if( pRpt == NULL ) {
							if( ulVCount != 0 && i == 1 ) {
								if( rtpIdFormatType == XDATA_TYPE_A ) {
									// RPTID���̔�
									for(ulReptID = 1; ulReptID <= 0xFFFFFFFF; ulReptID++ ) {
										pRpt = m_constData.get_Rpts().get_byID( ulReptID );
										if( pRpt == NULL ) {
											break;
										}
									}
								} else {
									// RPTID���𐶐�
									for(XD_UINT4 ii = 0; ii <= 0xFFFFFFFF; ii++ ) {
										strRtpIDName.Format(_T("Rpt_%u"), (ulReptID+ii));
										pRpt = m_constData.get_Rpts().get_byName( strRtpIDName );
										if( pRpt == NULL ) {
											break;
										}
									}
								}
								// ���|�[�gID�̍쐬
								XSEMI_Rpt tmpRpt(ulReptID, strRtpIDName);
								pRpt = m_constData.get_Rpts().add( &tmpRpt );
								if( pRpt == NULL ) {
									// 65=�ے�A�v���ł����ԂɂȂ�
									drack = 65;
									if( writer.putValAsBinary( &drack, 1 ) == false ) {
										return XSEMI_GEM_ERR_OUT_BUFF;
									}
									return XSEMI_GEM_ERR_NON_TRANSACT_OV;
								}
								pRpt->set_Comment(_T("Add from host"));
								pRpt->set_Enable( true );
							}
//							// 66=���݂��Ȃ�RPTID���w�肳�ꂽ
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
									// 3=�ے�A���Ȃ��Ƃ����RPTID�͂��łɒ�`����Ă���
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

						// �w�肳�ꂽV�����݂��邩���ׂ�
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
								// VName����A��������B
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
								// 4=�ے�A���Ȃ��Ƃ����VID�����݂��Ȃ�
								drack = 4;
								innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("[S2F33] RPTID (%u) VID(%s). DRACK=4. VID dose not exist."), ulReptID, (const TCHAR*)strVName);
							} else 
							if( i == 1 ) {
								m_constData.defineReport(ulReptID, pV->get_VID());
							}
						}
					}
				} catch(...) {
					// 65=�ے�A�v���ł����ԂɂȂ�
					drack = 65;
					if( writer.putValAsBinary( &drack, 1 ) == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}
			}
		} // for(int i = 0; i < 3; i++) 

		// �}�X�^�[���X�V
		try {
			m_constData.updateMaster( &m_constData.get_Rpts() );
		} catch(...) {
			// 65=�ے�A�v���ł����ԂɂȂ�
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
		S2,F35 Link Event Report (LER) M,H��E,�ԐM
		�����F �z�X�g���A�ʒm�C�x���gID�iCEID�j�ɑ΂��ă��|�[�g�������N����B
			�����N�������_�ł́A�ʒm�C�x���gID�iCEIDs�j�̕񍐂͏�ɖ����ɐݒ肳���B
			�C�x���g�����������ہA�z�X�g�ւ̒ʒm�C�x���gID�̕񍐂��K�v�ȏꍇ�́AS2,F37���g�p���ėL
			���ɐݒ肷��K�v������B
			�����AS2,F35���}���`�u���b�N�ł���Ȃ�΁AS2,F35�̑��M�ɐ旧���āAS2,F39/F40�i�₢����
			���^���j�g�����U�N�V�������s�����Ƃ��\�ł���B�܂����u�́ADATAID�𖳎�����B
		�\���F L,2
				1. <U4 DATAID> * �f�[�^ID
					2. L,a
						1. L,2
							1. <U4 CEID1> * �ʒm�C�x���gID
							2. L,b
								1. <U4 RPTID1> * ���|�[�gID
								:
								b. <U4 RPTIDb>
						:
						a. L,2
							1. <U4 CEIDa>
							2. L,c
								1. <U4 RPTID1>
								:
								c. <U4 RPTID>
	  

		S2,F36 Link Event Report Acknowledge (LERA) S,H��E
		�����F �����܂��̓G���[����������B
			�G���[��Ԃ����o�����ƁA���ׂẴ��b�Z�[�W�͋��ۂ����B���Ȃ킿�A�����I�ȕύX�͋���
			��Ȃ��B
		�\���F <B[1] LRACK> * �����N�񍐊m�F�R�[�h
				LRACK�F 0=����
				1=�ے�A�X�y�[�X�s�\��
				2=�ے�A�����t�H�[�}�b�g						( �d��CEID���w�肳�ꂽ)
				3=�ے�A���Ȃ��Ƃ����CEID�͂��łɒ�`����Ă���
				4=�ے�A���Ȃ��Ƃ����CEID�����݂��Ȃ�
				5=�ے�A���Ȃ��Ƃ����RPTID�����݂��Ȃ�
				6-63 �ۗ�
				65=�ے�A�v���ł����ԂɂȂ�

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
		std::vector<XSEMI_CEID*>	ceids;			// �X�V����CEID
		// **ID�̃t�H�[�}�b�g���擾
		XDATA_TYPE					ceidFormatType    = XDATA_TYPE_U4;
		XDATA_TYPE					dataIdFormatType = XDATA_TYPE_U4;
		XDATA_TYPE					rtpIdFormatType  = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_CEID,   &ceidFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_DATAID, &dataIdFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_RPTID,  &rtpIdFormatType) == false ) {
			// �t�H�[�}�b�g���s��
			return XSEMI_GEM_ERR_ABORT;
		}

		inData.getRaw(&pRawData, &iRawSize);
		//
		//	�f�[�^�̃`�F�b�N(i=0) & �X�V(i=1)
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
				// 2=�ے�A�����t�H�[�}�b�g
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
				// 1. <DATAID>�ɑ�������0�̃��X�g�ia=0�j�́A���ׂẴ��|�[�g�K�肨��ъ֘A�����N����������B
				lrack = 65;
				try {
					if( m_constData.deleteAllCEID_RptID() == true ) {
						lrack = 0;
					}
				} catch(...) {
				}
				// �z�X�g���瑗�M
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
							// VName����A��������B
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
							// 4=�ے�A���Ȃ��Ƃ����CEID�����݂��Ȃ�
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
						// �Z�b�g����V�̐�
						XD_UINT4 ulRptCount = data.getCount();

						if( i == 0 ) {
							if( pCEID != NULL ) {
								if( pCEID->get_RptsCount() != 0 && ulRptCount != 0 ) {
									// 3=�ے�A���Ȃ��Ƃ����RPTID�͂��łɒ�`����Ă���
									lrack = 3;

									innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("[S2F35] CEID (%s). LRACK=3. CEID link already defined."),  (const TCHAR*)strCEIDName);
								}
							}
						}


						if( i == 1 ) {
							// CEID�ɔ���t�����|�[�gID�����ׂč폜����B
							m_constData.unlinkEventReport(ulCEID, 0xFFFFFFFF);

							ceids.push_back( pCEID );

						}

						// �w�肳�ꂽReptID�����݂��邩���ׂ�
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
								// VName����A��������B
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
								// 5=�ے�A���Ȃ��Ƃ����RPTID�����݂��Ȃ�
								lrack = 5;
								
								innerWriteLog(XLIB_LOG_LEVEL_WARNING, _T("[S2F35] RPTID (%s). LRACK=5. RPTID dose not exist."), (const TCHAR*)strRptName);
							}
							if( i == 1 ) {
								m_constData.linkEventReport(ulCEID, ulRPTID);
							}
						}
					}
				} catch(...) {
					// 65=�ے�A�v���ł����ԂɂȂ�
					lrack = 65;
					if( writer.putValAsBinary( &lrack, 1 ) == false ) {
						return XSEMI_GEM_ERR_OUT_BUFF;
					}
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}
			}
		}




		// �}�X�^�[���X�V
		try {
			m_constData.updateMaster( &m_constData.get_CEIDs() );
		} catch(...) {
			// 65=�ے�A�v���ł����ԂɂȂ�
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
	ERACK�F 0=����
			1=�ے�A���Ȃ��Ƃ����CEID�����݂��Ȃ�
			2-63 �ۗ�
			65=�ے�A�v���ł����ԂɂȂ�
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
		std::vector<XSEMI_CEID*>	ceids;			// �X�V����CEID
		// **ID�̃t�H�[�}�b�g���擾
		XDATA_TYPE					ceidFormatType    = XDATA_TYPE_U4;
		XDATA_TYPE					dataIdFormatType = XDATA_TYPE_U4;

		if( get_XXID_Format(EC_INDEX_FORMAT_CEID,   &ceidFormatType) == false ||
			get_XXID_Format(EC_INDEX_FORMAT_DATAID, &dataIdFormatType) == false ) {
			// �t�H�[�}�b�g���s��
			return XSEMI_GEM_ERR_ABORT;
		}


		inData.getRaw(&pRawData, &iRawSize);
		//
		//	�f�[�^�̃`�F�b�N(i=0) & �X�V(i=1)
		//
		for(int i = 0; i < 2; i++) {
			XDataReader readerChk(pRawData, iRawSize);

			//  L,2
			if( readerChk.getValue( &data ) == false ) {
				// 2=�ے�A�����t�H�[�}�b�g
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != XDATA_TYPE_L || data.getCount() != 2 ) {
				// 2=�ے�A�����t�H�[�}�b�g
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}

			// U4 DATAID
			if( readerChk.getValue( &data ) == false ) {
				// 2=�ے�A�����t�H�[�}�b�g
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != XDATA_TYPE_BOOL || data.getCount() != 1 ) {
				// 2=�ے�A�����t�H�[�}�b�g
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			bEnable = data.getValAsBool();

			// 2. L,n
			if( readerChk.getValue( &data ) == false ) {
				// 2=�ے�A�����t�H�[�}�b�g
				erack = 2;
				if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			}
			if( data.getType() != XDATA_TYPE_L ) {
				// 2=�ے�A�����t�H�[�}�b�g
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
					// �}�X�^�[���X�V
//					m_pMasterData->updateCEIDs( m_constData );
				} catch(...) {
					// 65=�ے�A�v���ł����ԂɂȂ�
					erack = 65;
					if( writer.putValAsBinary( &erack, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
					return XSEMI_GEM_ERR_NON_TRANSACT_OV;
				}

				erack = 0;
				// �z�X�g���瑗�M
				if( writer.putValAsBinary( &erack, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
				return XSEMI_GEM_ERR_NON_TRANSACT_OV;
			} else {
				for( l = 0; l < ulCount; l++) {
					// U4 CEID
					if( readerChk.getValue( &data ) == false ) {
						// 2=�ے�A�����t�H�[�}�b�g
						erack = 2;
						if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}
					if( data.getType() != ceidFormatType && data.getCount() != 1 ) {
						// 2=�ے�A�����t�H�[�}�b�g
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
						// VName����A��������B
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
						// 1=�ے�A���Ȃ��Ƃ����CEID�����݂��Ȃ�
						erack = 1;
						if( writer.putValAsBinary( &erack, 1 ) == false ) {	return XSEMI_GEM_ERR_OUT_BUFF; }
						return XSEMI_GEM_ERR_NON_TRANSACT_OV;
					}

					if( i == 1 ) {
						// �X�V
						pCEID->put_Enable( bEnable );
						ceids.push_back( pCEID );
					}
				}


			}
		}
		// �}�X�^�[���X�V
		try {
			std::vector<XSEMI_CEID*>::iterator iteCEID;
			iteCEID = ceids.begin();
			for(; iteCEID != ceids.end(); iteCEID++) {
				m_constData.updateMaster( *iteCEID );
			}
			m_constData.clear();
		} catch(...) {
			// 65=�ے�A�v���ł����ԂɂȂ�
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
	// �z�X�g���瑗�M
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
			RSPACK�F 0=�����A�X�v�[�����O�ݒ���
					1=�X�v�[�����O�ݒ苑��
					2-63 �ۗ�
			STRACK�F	1=�X�v�[�����O�s�i�X�g���[��1�܂��́A�X�g���[��9���w�����ꂽ�j
						2=�X�g���[�����s��
						3=�t�@���N�V�������s��
						4=�񎟃��b�Z�[�W���w�肳�ꂽ
			���ӁF ����`�X�g���[���A�܂��́A����`�t�@���N�V��		
		*/
		//
		//	�f�[�^�̃`�F�b�N(i=0) & �X�V(i=1)
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
						// STRACK�F	4=�񎟃��b�Z�[�W���w�肳�ꂽ
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


	// �}�X�^�[���X�V
	try {
		m_constData.updateMaster( &m_constData.get_SpoolSFs() );
	} catch(...) {
		// 65=�ے�A�v���ł����ԂɂȂ�
		//erack = 65;
		//if( writer.putValAsBinary( &erack, 1 ) == false ) { return XSEMI_GEM_ERR_OUT_BUFF; }
		//return XSEMI_GEM_ERR_NON_TRANSACT_OV;
	}


	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

