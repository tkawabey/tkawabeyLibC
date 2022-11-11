/**
 * @file xfinctions.h
 * @brief ã§í ä÷êî
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_FUNCTIONS_FILE__
#define __X_FUNCTIONS_FILE__

#include "xlib/xlib_head.h"

namespace XFunctions
{
	XLIB_API unsigned short SystemToLittleEndian(unsigned short value);
	XLIB_API unsigned int   SystemToLittleEndian(unsigned int value);
	XLIB_API unsigned short SystemToBigEndian(unsigned short value);
	XLIB_API unsigned int   SystemToBigEndian(unsigned int value);
	XLIB_API unsigned short LittleEndianToSystem(unsigned short value);
	XLIB_API unsigned int   LittleEndianToSystem(unsigned int value);
	XLIB_API unsigned short BigEndianToSystem(unsigned short value);
	XLIB_API unsigned int   BigEndianToSystem(unsigned int value);
};

#include "xlib/xlib_tail.h"
#endif /*__X_FUNCTIONS_FILE__*/
