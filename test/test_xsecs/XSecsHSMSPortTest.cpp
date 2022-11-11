#include <iostream>
#include <fstream>
#include "XSecsHSMSPortTest.h"
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "xlib/semi/secs/xsecsHSMSPort.h"
#include "xlib/semi/secs/xsecs.h"
#include "xlib/xeception.h"
#include "xlib/xbuff.h"

#ifdef WIN32
/*
 * �\�P�b�g���C�u�����̏��������s���܂�
 */
XSocketInit g_sock;
#endif /* WIN32 */


/**
�R���X�g���N�^
@param name [in] �e�X�g��
*/
XSecsHSMSPortTest::XSecsHSMSPortTest(const char* name)
	: CPPUNIT_NS::TestCase(name)
{
}


/** �e�X�g�O���� */
void XSecsHSMSPortTest::setUp(void)
{
}


/** �e�X�g�㏈�� */
void XSecsHSMSPortTest::tearDown(void)
{
}

/**	@brief	�e�X�g�X�C�[�g */
CPPUNIT_NS::Test* XSecsHSMSPortTest::suite(void)
{
	CPPUNIT_NS::TestSuite* suite = new CPPUNIT_NS::TestSuite("XSecsHSMSPortTest");
	suite->addTest( new CPPUNIT_NS::TestCaller<XSecsHSMSPortTest>("XSecsHSMSPortTest",&testNomalActiveServer) );
	return suite;
}



class ActiveCallBack : public XSecsCommHandler
{
public:
	ActiveCallBack(){}
	virtual ~ActiveCallBack(){}
	virtual bool receiveDataMessageCallBack(XSeceCommItf* pPort, XSECSHeader& header, unsigned char* pMsgData,XD_UINT4 ulMsgSize)
//	virtual bool receiveDataMessageCallBack(XSecsHSMSPort* pPort, XSECSHeader& header, unsigned char* pMsgData,XD_UINT4 ulMsgSize)
	{
		XSecsData secsData;
		static int iCheck = 0;
		secsData.setFromBinary(pMsgData, ulMsgSize, XSecsData::HEAD_AND_BODY);
		if( secsData.get_stream()   == 1 &&
			secsData.get_function() == 13) {

			std::cout << "--------------------------" << std::endl;
#ifdef _UNICODE
			secsData.print( std::wcout );
#else
			secsData.print( std::cout );
#endif
			XSecsData rspData;
			rspData.setResponseHeader( &secsData.get_header() );
			XRichData* pDataL = rspData.addChiled(XDATA_TYPE_L);
			_XASSERT(pDataL);
			pDataL->addChiled(XDATA_TYPE_B);
			pDataL->addChiled(XDATA_TYPE_L);

			pPort->sendResponse( &rspData);


			iCheck++;

		}
		return true;
	}
	virtual bool receiveHSMSCtrlCallBack(XSECSHeader& header)
	{
		return true;
	}
};
void* testNomalActiveServerPeer(void* pData)
{
	try {
		XSocketAddress address("localhost", 5000);
		XSEMI_HSMS port;	
		ActiveCallBack activeCallBack;
		bool bRet = false;


		port.setAddress(	XSocketAddress((const char*)"127.0.0.1", 5000) );
		bRet = port.initilize(true, 
								true, 
								0, 
								&activeCallBack);

		if( bRet == false ) {
			std::cerr << "ERR : fail to connect" << std::endl;
			return 0;
		}

		while( port.get_status() != XSECS_CONN_STS_CONNECTED ) {
			XThread::Sleep(0, 100);
		}

		while( port.get_status() == XSECS_CONN_STS_CONNECTED ) {
			XThread::Sleep(0, 100);
		}

		port.closeIf();
		return 0;
	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	} catch(...) {
		std::cerr << "invalid error occored" << std::endl;
	}
	return NULL;
}




class PassiveCallBack : public XSecsCommHandler
{
public:
	PassiveCallBack(){}
	virtual ~PassiveCallBack(){}
	virtual bool receiveDataMessageCallBack(XSeceCommItf* pPort, XSECSHeader& header, unsigned char* pMsgData,XD_UINT4 ulMsgSize)
//	virtual bool receiveDataMessageCallBack(XSecsHSMSPort* pPort, XSECSHeader& header, unsigned char* pMsgData,XD_UINT4 ulMsgSize)
	{
		XSecsData secsData;
		static int iCheck = 0;
		secsData.setFromBinary(pMsgData, ulMsgSize, XSecsData::HEAD_AND_BODY);	

		if( secsData.get_stream()   == 1 &&
			secsData.get_function() == 14) {
			// 2�����b�Z�[�W����M
			iCheck++;
		}

		return true;
	}
	virtual bool receiveHSMSCtrlCallBack(XSECSHeader& header)
	{
		return true;
	}
};

void XSecsHSMSPortTest::testNomalActiveServer(void)
{
	try {
		XSocketAddress address((unsigned int)0, 5000);
		XSEMI_HSMS port;
		PassiveCallBack passiveCallBack;
		bool bRet = false;
		XSECS_SEND_STS sendSts;

		// Passive ���̃Z�b�V�������I�[�v��
		port.setAddress(	XSocketAddress((const char*)"127.0.0.1", 5000) );
		bRet = port.initilize(true, 
								false, 
								0, 
								&passiveCallBack);
		CPPUNIT_ASSERT_EQUAL(bRet, true);


		// Active ���̃X���b�h���N��
		XThread* pActive = XThread::Create(testNomalActiveServerPeer, this);

		// 
		XThread::Sleep(2, 0);

		// 1�����b�Z�[�W�𑗐M
		/*
		<S1F13 W
		 <L[2/1]
		  <A[6/1] "ABCDEF">
		  <A[6/1] "123456">
		 >
		>
		*/
		XSecsData dataSECS;
		XSecsData secsDataRsp;
		dataSECS.put_stream(1);
		dataSECS.put_function(13);
		dataSECS.put_wait(true);
		XRichData* pData = dataSECS.addChiled(XDATA_TYPE_L);
		CPPUNIT_ASSERT( pData->addChiled(XDATA_TYPE_A)->putValueString("ABCDEF") );
		CPPUNIT_ASSERT( pData->addChiled(XDATA_TYPE_A)->putValueString("123456") );
		// ��N�G�X�g���M�B
		sendSts = port.sendRequest( &dataSECS, &secsDataRsp, true, true, -1);
		CPPUNIT_ASSERT_EQUAL(sendSts, XSECS_SEND_STS_S_AND_HAS_REP);

		// �Q�����b�Z�[�W�̃f�[�^�m�F
		/*
		<S1F14
		 <L[2/1]
		  <B[0/1] >
		  <L[0/1]>
		 >
		>
		*/
		CPPUNIT_ASSERT(1  == secsDataRsp.get_stream() );
		CPPUNIT_ASSERT(14 == secsDataRsp.get_function() );
		CPPUNIT_ASSERT(false == secsDataRsp.get_wait() );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0) != NULL );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0)->getType() == XDATA_TYPE_L );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0)->getChiledCount() == 2 );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0)->getChiled(0) != NULL );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0)->getChiled(0)->getType() == XDATA_TYPE_B );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0)->getChiled(0)->getChiledCount() == 0 );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0)->getChiled(1) != NULL );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0)->getChiled(1)->getType() == XDATA_TYPE_L );
		CPPUNIT_ASSERT( secsDataRsp.getChiled(0)->getChiled(1)->getChiledCount() == 0 );
		std::cout << std::endl << "--------------------------" << std::endl;
#ifdef _UNICODE
		secsDataRsp.print( std::wcout );
#else
		secsDataRsp.print( std::cout );
#endif
		CPPUNIT_ASSERT( port.closeIf() == true );


	} catch(XExcept &e) {
		e.printStackTrace();
		CPPUNIT_ASSERT( 0 );
	}

}




