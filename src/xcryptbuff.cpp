#include <string.h>
#include "xlib/xstrbuff.h"
#include "xlib/xcryptbuff.h"
#include "xlib/xeception.h"

#ifdef WIN32
  #pragma warning( disable : 4244 )
#endif




// DEC:
//  トリプルDESコーダクラス
static unsigned char Df_Key[24] = 
{
	0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,
	0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,
	0x89,0xab,0xcd,0xef,0x01,0x23,0x45,0x67 
};

static unsigned short bytebit[8]	= 
{
	01, 02, 04, 010, 020, 040, 0100, 0200 
};

static unsigned long bigbyte[24] = 
{
	0x800000L,	0x400000L,	0x200000L,	0x100000L,
	0x80000L,	0x40000L,	0x20000L,	0x10000L,
	0x8000L,	0x4000L,	0x2000L,	0x1000L,
	0x800L, 	0x400L, 	0x200L, 	0x100L,
	0x80L,		0x40L,		0x20L,		0x10L,
	0x8L,		0x4L,		0x2L,		0x1L	
};

/* Use the key schedule specified in the Standard (ANSI X3.92-1981). */

static unsigned char pc1[56] = 
{
	56, 48, 40, 32, 24, 16,  8,	 0, 57, 49, 41, 33, 25, 17,
	 9,  1, 58, 50, 42, 34, 26,	18, 10,  2, 59, 51, 43, 35,
	62, 54, 46, 38, 30, 22, 14,	 6, 61, 53, 45, 37, 29, 21,
	13,  5, 60, 52, 44, 36, 28,	20, 12,  4, 27, 19, 11,  3 
};

static unsigned char totrot[16] = 
{
	1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28 
};

static unsigned char pc2[48] = 
{
	13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
	22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
	40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
	43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31 
};
XBuffD3DES::XBuffD3DES()
{
	SetKey( "SysAdWks" );
	m_nRealSize = 0;
}
XBuffD3DES::XBuffD3DES(const char* lpszKey/* = _T("SysAdWks")*/ )
{
	SetKey(lpszKey);
	m_nRealSize = 0;
}
XBuffD3DES::XBuffD3DES(const wchar_t* lpszKey/* = _T("SysAdWks")*/ )
{
	SetKey(lpszKey);
	m_nRealSize = 0;
}
XBuffD3DES::~XBuffD3DES()
{
}
void XBuffD3DES::SetKey(const char* lpszKey)
{
	size_t i;
	for (i = 0; i < 8; i++) {
		if (i < ::strlen(lpszKey)) {
			m_key[i] = lpszKey[i];
		} else {
			m_key[i] = 0;
		}
	}
}
void XBuffD3DES::SetKey(const wchar_t* lpszKey)
{
	this->SetKey( (const char*)XStrBuff(lpszKey) );
}
void *XBuffD3DES::Alloc(size_t nSize)
{
	//
	// 8 で割り切れるバッファを作成する
	//
	size_t       nOrgSize = nSize;
	unsigned int dwAml;
	if( nSize == 0 ) {
		return NULL;
	}

	dwAml = nSize % 8; 
	if(dwAml != 0) {
		nSize += (8-dwAml);
	}
	m_offSet = nSize / 8;
	void *p =  XBuff::Alloc(nSize);
	if( p ) {
		m_nRealSize = nOrgSize;
	}
	return p;
}
void XBuffD3DES::Free()
{
	XBuff::Free();
	m_nRealSize = 0;
}
size_t XBuffD3DES::GetBuffSize()
{
	return XBuff::GetBuffSize();
}
void XBuffD3DES::EnCrypt()
{
	unsigned int i;
	unsigned int j;
	unsigned char *where = (unsigned char*)m_pBuff;

	deskey(m_key, 0);

	for (i = 0, j = 0; j < m_offSet; i += 8, j++)
	{
		des(where+i, where+i);
	}
}
void XBuffD3DES::DeCrypt()
{
    unsigned int i;
	unsigned int j;
	unsigned char *where = (unsigned char*)m_pBuff;
	
	deskey(m_key, 1);

    for (i = 0, j = 0; j < m_offSet; i += 8, j++)
	{
		des(where+i, where+i);
    }
}
void XBuffD3DES::deskey(unsigned char *key, int edf)
{
	register int i, j, l, m, n;
	unsigned char pc1m[56], pcr[56];
	unsigned long kn[32];

	for ( j = 0; j < 56; j++ ) 
	{
		l = pc1[j];
		m = l & 07;
		pc1m[j] = (key[l >> 3] & bytebit[m]) ? 1 : 0;
	}
	for( i = 0; i < 16; i++ ) 
	{
		if( edf == 1 ) 
			m = (15 - i) << 1;
		else 
			m = i << 1;
		n = m + 1;
		kn[m] = kn[n] = 0L;
		for( j = 0; j < 28; j++ ) 
		{
			l = j + totrot[i];
			if( l < 28 ) 
				pcr[j] = pc1m[l];
			else 
				pcr[j] = pc1m[l - 28];
		}
		for( j = 28; j < 56; j++ ) 
		{
		    l = j + totrot[i];
		    if( l < 56 ) 
				pcr[j] = pc1m[l];
		    else 
				pcr[j] = pc1m[l - 28];
		}
		for( j = 0; j < 24; j++ ) 
		{
			if( pcr[pc2[j]] ) 
				kn[m] |= bigbyte[j];
			if( pcr[pc2[j+24]] ) 
				kn[n] |= bigbyte[j];
		}
	}
	cookey(kn);
	return;	
}
void XBuffD3DES::cookey(unsigned long *raw1)
{
	unsigned long *cook, *raw0;
	unsigned long dough[32];
	int i;

	cook = dough;
	for( i = 0; i < 16; i++, raw1++ )
	{
		raw0 = raw1++;
		*cook	 = (*raw0 & 0x00fc0000L) << 6;
		*cook	|= (*raw0 & 0x00000fc0L) << 10;
		*cook	|= (*raw1 & 0x00fc0000L) >> 10;
		*cook++ |= (*raw1 & 0x00000fc0L) >> 6;
		*cook	 = (*raw0 & 0x0003f000L) << 12;
		*cook	|= (*raw0 & 0x0000003fL) << 16;
		*cook	|= (*raw1 & 0x0003f000L) >> 4;
		*cook++ |= (*raw1 & 0x0000003fL);
	}
	usekey(dough);
	return;	
}
void XBuffD3DES::usekey(unsigned long *from)
{
	unsigned long *to, *endp;

	to = KnL, endp = &KnL[32];
	while( to < endp ) 
		*to++ = *from++;
	return;	
}

void XBuffD3DES::cpkey(unsigned long *into)
{
	register unsigned long *from, *endp;

	from = KnL, endp = &KnL[32];
	while( from < endp ) *into++ = *from++;
	return;	
}

void XBuffD3DES::des(unsigned char *inblock, unsigned char  *outblock)
{
	unsigned long work[2];

	scrunch(inblock, work);
	desfunc(work, KnL);
	unscrun(work, outblock);
	return;
}
void XBuffD3DES::scrunch(unsigned char *outof, unsigned long *into)
{
	*into	 = (*outof++ & 0xffL) << 24;
	*into	|= (*outof++ & 0xffL) << 16;
	*into	|= (*outof++ & 0xffL) << 8;
	*into++ |= (*outof++ & 0xffL);
	*into	 = (*outof++ & 0xffL) << 24;
	*into	|= (*outof++ & 0xffL) << 16;
	*into	|= (*outof++ & 0xffL) << 8;
	*into	|= (*outof   & 0xffL);
	return;	
}
void XBuffD3DES:: unscrun(unsigned long *outof, unsigned char *into)
{
	*into++ = (unsigned char)(*outof >> 24) & 0xffL;
	*into++ = (unsigned char)(*outof >> 16) & 0xffL;
	*into++ = (unsigned char)(*outof >>  8) & 0xffL;
	*into++ = (unsigned char)*outof++	 & 0xffL;
	*into++ = (unsigned char)(*outof >> 24) & 0xffL;
	*into++ = (unsigned char)(*outof >> 16) & 0xffL;
	*into++ = (unsigned char)(*outof >>  8) & 0xffL;
	*into	= (unsigned char)*outof	 & 0xffL;
	return;	
}

static unsigned long SP1[64] = {
	0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L,
	0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
	0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L,
	0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
	0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L,
	0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
	0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L,
	0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
	0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L,
	0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
	0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L,
	0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
	0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L,
	0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
	0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L,
	0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L };

static unsigned long SP2[64] = {
	0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L,
	0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
	0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L,
	0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
	0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L,
	0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
	0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L,
	0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
	0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L,
	0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
	0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L,
	0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
	0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L,
	0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
	0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L,
	0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L };

static unsigned long SP3[64] = {
	0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L,
	0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
	0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L,
	0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
	0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L,
	0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
	0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L,
	0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
	0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L,
	0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
	0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L,
	0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
	0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L,
	0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
	0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L,
	0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L };

static unsigned long SP4[64] = {
	0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
	0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
	0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L,
	0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
	0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L,
	0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
	0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L,
	0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
	0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L,
	0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
	0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L,
	0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
	0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L,
	0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
	0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L,
	0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L };

static unsigned long SP5[64] = {
	0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L,
	0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
	0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L,
	0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
	0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L,
	0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
	0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L,
	0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
	0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L,
	0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
	0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L,
	0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
	0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L,
	0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
	0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L,
	0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L };

static unsigned long SP6[64] = {
	0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L,
	0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
	0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L,
	0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
	0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L,
	0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
	0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L,
	0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
	0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L,
	0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
	0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L,
	0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
	0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L,
	0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
	0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L,
	0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L };

static unsigned long SP7[64] = {
	0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
	0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
	0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
	0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
	0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
	0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
	0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
	0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
	0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
	0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
	0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
	0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
	0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
	0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
	0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
	0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };

static unsigned long SP8[64] = {
	0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
	0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
	0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
	0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
	0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
	0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
	0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
	0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
	0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
	0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
	0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
	0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
	0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
	0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
	0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
	0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L };

void XBuffD3DES::desfunc(unsigned long *block, unsigned long *keys)

{
	register unsigned long fval, work, right, leftt;
	register int round;

	leftt = block[0];
	right = block[1];
	work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
	right ^= work;
	leftt ^= (work << 4);
	work = ((leftt >> 16) ^ right) & 0x0000ffffL;
	right ^= work;
	leftt ^= (work << 16);
	work = ((right >> 2) ^ leftt) & 0x33333333L;
	leftt ^= work;
	right ^= (work << 2);
	work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
	leftt ^= work;
	right ^= (work << 8);
	right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;

	for( round = 0; round < 8; round++ )
	{
		work  = (right << 28) | (right >> 4);
		work ^= *keys++;
		fval  = SP7[ work		 & 0x3fL];
		fval |= SP5[(work >>  8) & 0x3fL];
		fval |= SP3[(work >> 16) & 0x3fL];
		fval |= SP1[(work >> 24) & 0x3fL];
		work  = right ^ *keys++;
		fval |= SP8[ work		 & 0x3fL];
		fval |= SP6[(work >>  8) & 0x3fL];
		fval |= SP4[(work >> 16) & 0x3fL];
		fval |= SP2[(work >> 24) & 0x3fL];
		leftt ^= fval;
		work  = (leftt << 28) | (leftt >> 4);
		work ^= *keys++;
		fval  = SP7[ work		 & 0x3fL];
		fval |= SP5[(work >>  8) & 0x3fL];
		fval |= SP3[(work >> 16) & 0x3fL];
		fval |= SP1[(work >> 24) & 0x3fL];
		work  = leftt ^ *keys++;
		fval |= SP8[ work		 & 0x3fL];
		fval |= SP6[(work >>  8) & 0x3fL];
		fval |= SP4[(work >> 16) & 0x3fL];
		fval |= SP2[(work >> 24) & 0x3fL];
		right ^= fval;
	}

	right = (right << 31) | (right >> 1);
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = (leftt << 31) | (leftt >> 1);
	work = ((leftt >> 8) ^ right) & 0x00ff00ffL;
	right ^= work;
	leftt ^= (work << 8);
	work = ((leftt >> 2) ^ right) & 0x33333333L;
	right ^= work;
	leftt ^= (work << 2);
	work = ((right >> 16) ^ leftt) & 0x0000ffffL;
	leftt ^= work;
	right ^= (work << 16);
	work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
	leftt ^= work;
	right ^= (work << 4);
	*block++ = right;
	*block = leftt;
	return;	
}


//
//	XMD5
//
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

XMD5::XMD5()
{
	Init();
}
XMD5::XMD5(const char* p)
{
	XMD5::operator=(p);
}
XMD5::XMD5(const wchar_t* p)
{
	XMD5::operator=(p);
}
const XMD5& XMD5::operator=(const char* lpsz)
{
	uint4 size = strlen(lpsz) * sizeof(char);
	XStrBuff::operator=(lpsz);
	
	Init();
	Update((uint1*)lpsz, size);

	return *this;
}
const XMD5& XMD5::operator=(const wchar_t* lpsz)
{
	uint4 size = wcslen(lpsz) * sizeof(wchar_t);
	XStrBuff::operator=(lpsz);

	Init();
	Update((uint1*)lpsz, size);

	return *this;
}
const XMD5& XMD5::operator+=(const char* lpsz)
{
#ifdef _UNICODE
	XStrBuff strTemp(lpsz);
	uint4 size = strlen(strTemp) * sizeof(char);
	XStrBuff::operator+=((const wchar_t*)strTemp);

	Update((uint1*)strTemp, size);
#else 
	uint4 size = strlen(lpsz) * sizeof(char);
	XStrBuff::operator+=(lpsz);

	Update((uint1*)lpsz, size);
#endif
	return *this;
}
const XMD5& XMD5::operator+=(const wchar_t* lpsz)
{
#ifdef _UNICODE
	uint4 size = wcslen(lpsz) * sizeof(wchar_t);
	XStrBuff::operator+=(lpsz);

	Update((uint1*)lpsz, size);
#else
	XStrBuff strTemp(lpsz);
	uint4 size = wcslen(strTemp) * sizeof(wchar_t);
	XStrBuff::operator+=((const char*)strTemp);
	Update((uint1*)strTemp, size);
#endif
	return *this;
}
void XMD5::Make()
{
	Init();
	Update((uint1*)m_pBuff, m_nBuffSize);
	Finalize();
}
void XMD5::Make(XFile *pFile)
{
	unsigned long dwSize = pFile->GetLength();
	unsigned long dwRead;
	uint1 szbuff[1024];
	while(dwSize != 0) {
		if( dwSize > sizeof(szbuff)) {
			dwRead = sizeof(szbuff);
		} else {
			dwRead = dwSize;
		}
		if( pFile->Read(szbuff, dwRead) == false ) {
			return ;
		}
		Update(szbuff, dwRead);
		dwSize -= dwRead;
	}
	Finalize();
}
void XMD5::Update(uint1 *input, uint4 input_length)
{
	uint4 input_index, buffer_index;
	uint4 buffer_space;                // how much space is left in buffer

	if(m_finalized) {
		XMD5Except::Throw(1);
	}


	buffer_index = (unsigned int)((m_count[0] >> 3) & 0x3F);


	if(  (m_count[0] += ((uint4) input_length << 3))<((uint4) input_length << 3) ) {
		m_count[1]++;
	}

	m_count[1] += ((uint4)input_length >> 29);


	buffer_space = 64 - buffer_index;  // how much space is left in buffer

	// Transform as many times as possible.
	if (input_length >= buffer_space)
	{
		// ie. we have enough to fill the buffer
		// fill the rest of the buffer and transform
		memcpy (m_buffer + buffer_index, input, buffer_space);
		Transform(m_buffer);

		// now, transform each 64-byte piece of the input, bypassing the m_buffer
		for (input_index = buffer_space; input_index + 63 < input_length; input_index += 64)
			Transform (input+input_index);

		buffer_index = 0;  // so we can m_buffer remaining
	}
	else
		input_index=0;     // so we can m_buffer the whole input


	// and here we do the buffering:
	memcpy(m_buffer+buffer_index, input+input_index, input_length-input_index);
}
void XMD5::Finalize()
{
	unsigned char bits[8];
	unsigned int index, padLen;
	static uint1 PADDING[64]={
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	if (m_finalized) {
		XMD5Except::Throw(1);
		return;
	}

	// Save number of bits
	encode (bits, m_count, 8);

	// Pad out to 56 mod 64.
	index = (uint4) ((m_count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	Update(PADDING, padLen);

	// Append length (before padding)
	Update(bits, 8);

	// Store state in digest
	encode(m_digest, m_state, 16);

	// Zeroize sensitive information
	memset(m_buffer, 0, sizeof(*m_buffer));

	m_finalized=1;
}

void XMD5::RawDigest(XBuff *pBuff)
{
	uint1 *s;
	s = (uint1*)pBuff->Alloc( sizeof(uint1)*16 );
	if(s == NULL) {
		XMD5Except::Throw(2);
	}
	if(!m_finalized) {
		XMD5Except::Throw(0);
	}
	memcpy(s, m_digest, 16);
	return ;
}
XStrBuff XMD5::HexDigest() const
{
	XStrBuff str;
	char     sz[33];
	int      i;


	if(!m_finalized) {
		XMD5Except::Throw(0);
	}
	for (i=0; i<16; i++) {
		XStrBuff::__sprintf(sz+i*2, sizeof(sz)-(i*2), "%02x", m_digest[i]);
	}

	sz[32]='\0';
	str = sz;
	return str;
}

void XMD5::Init()
{
	m_finalized = 0;


	m_count[0] = 0;
	m_count[1] = 0;

	m_state[0] = 0x67452301;
	m_state[1] = 0xefcdab89;
	m_state[2] = 0x98badcfe;
	m_state[3] = 0x10325476;
}
void XMD5::Transform(uint1 *block)
{
	uint4 a = m_state[0], b = m_state[1], c = m_state[2], d = m_state[3], x[16];

	decode(x, block, 64);

	_XASSERT(!m_finalized);  // not just a user error, since the method is private

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	m_state[0] += a;
	m_state[1] += b;
	m_state[2] += c;
	m_state[3] += d;

	// Zeroize sensitive information.
	memset ( (uint1 *) x, 0, sizeof(x));
}
void XMD5::encode (uint1 *output, uint4 *input, uint4 len) {

	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[j]   = (uint1)  (input[i] & 0xff);
		output[j+1] = (uint1) ((input[i] >> 8) & 0xff);
		output[j+2] = (uint1) ((input[i] >> 16) & 0xff);
		output[j+3] = (uint1) ((input[i] >> 24) & 0xff);
	}
}
void XMD5::decode (uint4 *output, uint1 *input, uint4 len){

	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[i] = ((uint4)input[j]) | (((uint4)input[j+1]) << 8) |
				(((uint4)input[j+2]) << 16) | (((uint4)input[j+3]) << 24);
	}
}
void XMD5::memcpy (uint1 *output, uint1 *input, uint4 len){

	unsigned int i;
	for (i = 0; i < len; i++)
		output[i] = input[i];
}
void XMD5::memset (uint1 *output, uint1 value, uint4 len){

	unsigned int i;

	for (i = 0; i < len; i++)
		output[i] = value;
}
inline unsigned int XMD5::rotate_left  (uint4 x, uint4 n)
{
	return (x << n) | (x >> (32-n))  ;
}
inline unsigned int XMD5::F(uint4 x, uint4 y, uint4 z)
{
	return (x & y) | (~x & z);
}
inline unsigned int XMD5::G(uint4 x, uint4 y, uint4 z){
	return (x & z) | (y & ~z);
}
inline unsigned int XMD5::H(uint4 x, uint4 y, uint4 z)
{
	return x ^ y ^ z;
}
inline unsigned int XMD5::I(uint4 x, uint4 y, uint4 z)
{
	return y ^ (x | ~z);
}
inline void XMD5::FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4  s, uint4 ac)
{
	a += F(b, c, d) + x + ac;
	a = rotate_left (a, s) +b;
}
inline void XMD5::GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,  uint4 s, uint4 ac)
{
	a += G(b, c, d) + x + ac;
	a = rotate_left (a, s) +b;
}
inline void XMD5::HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x,  uint4 s, uint4 ac)
{
	a += H(b, c, d) + x + ac;
	a = rotate_left (a, s) +b;
}
inline void XMD5::II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
	a += I(b, c, d) + x + ac;
	a = rotate_left (a, s) +b;
}

#ifdef WIN32
  #pragma warning( default : 4244 )
#endif
