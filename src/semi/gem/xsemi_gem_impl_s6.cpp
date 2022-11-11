#include "xlib/semi/gem/xsemi_gem.h"
#include "xlib/xfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>


int XSEMI_Gem::processGemMessageS6(XD_UINT4 function, XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XData data;
	XDataWriter	writer( &pOutStream );
	switch( function ) {
	case 1:
		return processGemMessageS6F1(inData, pOutStream);
		break;
	case 5:
		return processGemMessageS6F5(inData, pOutStream);
		break;
	case 11:
		return processGemMessageS6F11(inData, pOutStream);
		break;
	}
	return XSEMI_GEM_ERR_HEAD_FUNCTION;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS6F1(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XD_UINT1	ack6 = 0;
	XDataWriter	writer( &pOutStream );
	// ホストから送信
	if( writer.putValAsBinary( &ack6, 1 ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS6F5(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XD_UINT1	grant6 = 0;
	XDataWriter	writer( &pOutStream );
	// ホストから送信
	if( writer.putValAsBinary( &grant6, 1 ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}

int XSEMI_Gem::processGemMessageS6F11(XDataReader &inData, std::ostream& pOutStream)
{
XEXCEPT_ENTRY
	XD_UINT1	ack6 = 0;
	XDataWriter	writer( &pOutStream );
	// ホストから送信
	if( writer.putValAsBinary( &ack6, 1 ) == false ) {
		return XSEMI_GEM_ERR_OUT_BUFF;
	}
	return XSEMI_GEM_ERR_NON_TRANSACT_OV;
XEXCEPT_EXIT
}