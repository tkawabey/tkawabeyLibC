#include "xlib/xfunctions.h"





unsigned short XFunctions::SystemToLittleEndian(unsigned short value)
{
	return value ;
}
unsigned int XFunctions::SystemToLittleEndian(unsigned int value)
{
	return value ;
}
unsigned short XFunctions::SystemToBigEndian(unsigned short value)
{
	return (unsigned short) ((value&0xFF) << 8)|((value&0xFF00)>>8) ;
}
unsigned int XFunctions::SystemToBigEndian(unsigned int value)
{
	return ((value&0xFF000000L)>>24)|((value&0xFF0000L)>>8)
		 | ((value&0xFF00L)<<8) | ((value&0xFFL)<<24) ;
}

unsigned short XFunctions::LittleEndianToSystem(unsigned short value)
{
	return value ;
}
unsigned int XFunctions::LittleEndianToSystem(unsigned int value)
{
	return value ;
}
unsigned short XFunctions::BigEndianToSystem(unsigned short value)
{
	return (unsigned short) ((value&0xFF) << 8)|((value&0xFF00)>>8) ;
}
unsigned int XFunctions::BigEndianToSystem(unsigned int value)
{
	return ((value&0xFF000000L)>>24)|((value&0xFF0000L)>>8)
		 | ((value&0xFF00L)<<8) | ((value&0xFFL)<<24) ;
}
