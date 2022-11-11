#include <string.h>
#include "xlib/xeception.h"
#include "xlib/xstrbuff.h"
#include "xlib/xcompressbuff.h"


//
//	XBuffCompress
//
XBuffCompress::XBuffCompress()
{
	m_cMask              = 0x80;
	m_nRack              = 0;
	m_nPacifierCounter   = 0;
	m_nCurrentByte       = 0;
	m_iExpandAlloBuff    = 5.0;
	m_nPACIFIER_COUNT    = 2047;
}
XBuffCompress::~XBuffCompress()
{
}
void *XBuffCompress::Alloc(size_t nSize)
{
     m_cMask              = 0x80;
	m_nRack              = 0;
	m_nPacifierCounter   = 0;
	m_nCurrentByte       = 0;
	void *p = XBuff::Alloc(nSize);
	if(p != NULL) {
		this->MemSet(0);
	}
	return p;
}

void XBuffCompress::Free()
{
	XBuff::Free();
}
bool XBuffCompress::IsEOF()
{
	return (m_nCurrentByte > m_nBuffSize);
}
unsigned char XBuffCompress::getChar()
{
	// バッファから1Byte読み込み、内部ポインタを1進めます
	unsigned char *p = (unsigned char*)m_pBuff;
	unsigned char c;

	if( m_nCurrentByte > m_nBuffSize ) {
//		XExcept *pE = new XExcept(_T("Current Buff Equal Buff Size"));
//		pE->SetDateTime( __DATE__, __TIME__);
//		pE->SetSource( __FILE__ );
//		pE->SetLine( __LINE__ );
//		throw pE;
		return 0;
	}
	c = p[m_nCurrentByte];

	m_nCurrentByte++;
	return c;
}

bool XBuffCompress::putChar(unsigned char c)
{
	//  バッファから1Byte書き込み、内部ポインタを1進めます
	unsigned char *p = (unsigned char*)m_pBuff;
	if( m_nCurrentByte >= m_nBuffSize )
	{
		XBuff buff;              // Temp用のバッファ
		double dExpandDataSize;

		// Tempのバッファに現在のバッファをコピー
		TCHAR* pBuff = (TCHAR*)buff.Alloc( GetBuffSize() );
		if( pBuff == NULL )
			return false;
		::memcpy(pBuff, *this , GetBuffSize());
		
		// バッファ拡張後のバッファサイズ
		dExpandDataSize = buff.GetBuffSize() * m_iExpandAlloBuff;
		
		// 新たに拡張後のメモリを確保し、Tempのバッファをコピー
		pBuff = (TCHAR*)XBuff::Alloc( (int)dExpandDataSize );
		if( pBuff == NULL )
			return false;
		::memcpy(pBuff, buff, buff.GetBuffSize());

		p = (unsigned char*)m_pBuff;
	}
	p[m_nCurrentByte] = c;

	m_nCurrentByte++;

	return true;
}
void XBuffCompress::outputBit(int bit)
{
	// bitがtrueの時、ラックに現在のORを取る(現在のラックのマスクの位置に必ず０が立っている状態)。
	// マスクを1Bit右シフト。
	// マスクが０に達した場合は、ラックをストリームに書き込み、ラックとマスクをリセット
    if ( bit )
        m_nRack |= m_cMask;
    m_cMask >>= 1;
    if ( m_cMask== 0 )
	{
		putChar( m_nRack );
		if ( ( m_nPacifierCounter++ & m_nPACIFIER_COUNT ) == 0 )
		{
		}
		m_nRack = 0;
		m_cMask = 0x80;
    }
}
void XBuffCompress::outputBits(unsigned long code, int count)
{
	// マスクが０に達した場合は、ラックをストリームに書き込み、ラックとマスクをリセット
    unsigned long   mask;

    mask = 1L << ( count - 1 );
    while ( mask != 0)
	{
        if ( mask & code )
            m_nRack |= m_cMask;

        m_cMask >>= 1;

        if ( m_cMask == 0 )
		{
            putChar( m_nRack );
			if ( ( m_nPacifierCounter++ & m_nPACIFIER_COUNT ) == 0 )
			{
			}
			m_nRack = 0;
			m_cMask = 0x80;
		}
        mask >>= 1;
    }
}
int XBuffCompress::inputBit()
{
    int value;
	
	// マスクが、0x80の場合、IOから、１Byteをラックに読み込む
	// 
	//
    if( m_cMask == 0x80 )
	{
		if ( IsEOF() )
		{
//       		XExcept *pE = new XExcept(_T("Fatal error in inputBit"));
//			pE->SetDateTime( __DATE__, __TIME__);
//			pE->SetSource( __FILE__ );
//			pE->SetLine( __LINE__ );
//			throw pE;
		}
        m_nRack = getChar();
		if ( ( m_nPacifierCounter++ & m_nPACIFIER_COUNT ) == 0 )
		{
		}
    }
    value = m_nRack & m_cMask;
    m_cMask >>= 1;
    if ( m_cMask == 0 )
		m_cMask = 0x80;

    return( value ? 1 : 0 );
}

int XBuffCompress::inputBits(int bit_count)
{
    unsigned long mask;
    unsigned long return_value;

    mask = 1L << ( bit_count - 1 );
    return_value = 0;

    while( mask != 0)
	{
		if( m_cMask == 0x80 )
		{
		    if ( IsEOF() )
			{
//       			XExcept *pE = new XExcept(_T("Fatal error in inputBit"));
//				pE->SetDateTime( __DATE__, __TIME__);
//				pE->SetSource( __FILE__ );
//				pE->SetLine( __LINE__ );
//				throw pE;
			}
		    m_nRack = getChar();
			if ( ( m_nPacifierCounter++ & m_nPACIFIER_COUNT ) == 0 )
			{
			}
		}
		if( m_nRack & m_cMask )
		{
            return_value |= mask;
	    }
        mask >>= 1;
        m_cMask >>= 1;
        if( m_cMask == 0 )
            m_cMask = 0x80;
    }
    return( return_value );
}
void XBuffCompress::closeOutPutPull()
{
	char r = m_nRack;
	if( m_cMask != 0x80 )
	{
		putChar(r);
	}
}
unsigned int XBuffCompress::getCurrentByte()
{
	return m_nCurrentByte ;
}




//
//	XBuffCompressLZW
//
/**
 * コンストラクタ
 * @param nTableSize テーブルサイズ
 * @param nEndOfStream ストリームの最後
 * @param nBumpCode バンプコード
 * @param nFlushCode フラッシュコード
 * @param nFirstCode ファーストコード
 * @param nBits Bitsコード
 */
XBuffCompressLZW::XBuffCompressLZW(unsigned int nTableSize/*   = 35023  */,
								   unsigned int nEndOfStream/* = 256    */,
								   unsigned int nBumpCode/*    = 257    */,
								   unsigned int nFlushCode/*   = 258    */,
								   unsigned int nFirstCode/*   = 259    */,
								   unsigned int nBits/*        = 15     */)
	:	XBuffCompress()
{
	m_pDict         = NULL;
	m_DecodeStack   = NULL;
	m_nTableSize    = nTableSize;
	m_nEndOfStream  = nEndOfStream;
	m_nBumpCode     = nBumpCode;
	m_nFlushCode    = nFlushCode;
	m_nFirstCode    = nFirstCode;
	m_nBits         = nBits;
}
XBuffCompressLZW::~XBuffCompressLZW()
{
	if( m_pDict == NULL )
		delete [] m_pDict;
	m_pDict = NULL;
	if( m_DecodeStack == NULL )
		delete [] m_DecodeStack;
	m_DecodeStack = NULL;
}

bool XBuffCompressLZW::Compress()
{
	XBuffCompressLZW   outBuff;		// 書き込みバッファ
    int character;
    int string_code;
    unsigned int index;

	if( InitializeStorage() == false ) {
		return false;
	}
	InitializeDictionary();
	
	if( outBuff.Alloc( GetBuffSize() * 2  ) == NULL ) {
		return false;
	}

    m_cMask              = 0x80;
	m_nRack              = 0;
	m_nPacifierCounter   = 0;

	string_code = this->getChar();
    if ( this->IsEOF() ) {
        string_code = m_nEndOfStream;
    }
    while ( IsEOF() == 0 ) {
        character = getChar();
        index = this->FindChildNode( string_code, character );

		int iiii = m_pDict[index].codeValue;

		if( m_pDict[index].codeValue != - 1 ) {
            string_code = m_pDict[index].codeValue;
        } else {
            m_pDict[index].codeValue = m_nNextCode++;
            m_pDict[index].codeParent = string_code;
            m_pDict[index].charactor = (char) character;

            outBuff.outputBits((unsigned long) string_code, m_nCurrentCodeBits );
            string_code = character;
            if ( m_nNextCode > this->MaxCode() ) {
                outBuff.outputBits((unsigned long) m_nFlushCode, m_nCurrentCodeBits );
                InitializeDictionary();
            } else
			if ( m_nNextCode > m_nNextBumpCode ) {
                outBuff.outputBits((unsigned long) m_nBumpCode, m_nCurrentCodeBits );
                m_nCurrentCodeBits++;
                m_nNextBumpCode <<= 1;
                m_nNextBumpCode |= 1;
            }
        }
    }
    outBuff.outputBits((unsigned long) string_code, m_nCurrentCodeBits );
    outBuff.outputBits((unsigned long) m_nEndOfStream, m_nCurrentCodeBits);
	outBuff.closeOutPutPull();


	if( this->Alloc( outBuff.getCurrentByte() ) == NULL ) {
		return false;
	}
	::memcpy(*this, outBuff, outBuff.getCurrentByte() );

	return true;
}
bool XBuffCompressLZW::Expand()
{
	XBuffCompressLZW   outBuff;		// 書き込みバッファ
    unsigned int new_code;
    unsigned int old_code;
    int character;
    unsigned int count;

	if( outBuff.Alloc( GetBuffSize() * 2  ) == NULL )
		return false;

    m_cMask              = 0x80;
	m_nRack              = 0;
	m_nPacifierCounter   = 0;
		
	if( InitializeStorage() == false )
		return false;


    for ( ; ; )
	{
        InitializeDictionary();
        old_code = (unsigned int) inputBits( m_nCurrentCodeBits );
        if( old_code == m_nEndOfStream )
		{
			outBuff.closeOutPutPull();
			if( Alloc( outBuff.getCurrentByte() -1   ) == NULL )
					return false;
			::memcpy(*this, outBuff, outBuff.getCurrentByte() -1 );
            return true;
        }
        character = old_code;
        outBuff.putChar(old_code);
        for ( ; ; )
		{
            new_code = (unsigned int) inputBits( m_nCurrentCodeBits );
            if( new_code == m_nEndOfStream )
			{
				outBuff.closeOutPutPull();
				if( Alloc( outBuff.getCurrentByte() -1  ) == NULL )
					return false;
				::memcpy(*this, outBuff, outBuff.getCurrentByte() -1 );
                return true;
            }
            if ( new_code == m_nFlushCode )
			{
                break;
            }
            if ( new_code == m_nBumpCode )
			{
                m_nCurrentCodeBits++;
                continue;
            }
			if ( new_code >= -1 )
			{
				return false;
			}
            if ( new_code >= m_nNextCode )
			{
                m_DecodeStack[ 0 ] = (char) character;
                count = DecodeString( 1, old_code );
            }
            else
			{
                count = DecodeString( 0, new_code );
            }
            character = m_DecodeStack[ count - 1 ];
            while ( count > 0 )
			{
                outBuff.putChar(m_DecodeStack[ --count ]);
            }

            m_pDict[m_nNextCode].codeParent = old_code;
            m_pDict[m_nNextCode].charactor = (char) character;

            m_nNextCode++;
            old_code = new_code;
        }   
    }
	outBuff.closeOutPutPull();
	if( Alloc( outBuff.getCurrentByte() -1   ) == NULL )
		return false;
	::memcpy(*this, outBuff, outBuff.getCurrentByte() -1 );
	return true;
}
unsigned int XBuffCompressLZW::MaxCode()
{
	return ( ( 1 << m_nBits ) - 1 );
}
void XBuffCompressLZW::InitializeDictionary()
{
	// Dictionaryを初期化します。
    unsigned int i;
	_XASSERT( m_pDict );
	for( i = 0; i < m_nTableSize; i++ )
	{
		m_pDict[i].codeValue = -1;
	}

	m_nNextCode        = m_nFirstCode;
	m_nCurrentCodeBits = 9;
	m_nNextBumpCode    = 511;
}
bool XBuffCompressLZW::InitializeStorage()
{
	if( m_pDict != NULL )
		delete [] m_pDict;
	m_pDict = NULL;

	if( m_DecodeStack != NULL )
		delete [] m_DecodeStack;
	m_DecodeStack = NULL;

	m_pDict = new Dictionary[m_nTableSize + 1];
	m_DecodeStack = new unsigned char[m_nTableSize + 1];

	if( m_pDict == NULL || m_DecodeStack == NULL )
		return false;

	memset(m_DecodeStack, 0, sizeof(unsigned char) * (m_nTableSize + 1) );

	return true;
}
unsigned int XBuffCompressLZW::FindChildNode( int parent_code, int child_character )
{
	//	このハッシュルーチンは、string/character組み合わせの位置を求める
	//  ことを担当します。このテーブルのインデックスは、プレフィックスとキャラ
	//  クターの除外または、組み合わせの使用によって作られている。
	//
    unsigned int index  = 0;
    unsigned int offset = 0;
	

    index = ( child_character << ( m_nBits - 8 ) ) ^ parent_code;

    if ( index == 0 )
	{
        offset = 1;
	}
    else
	{
        offset = m_nTableSize - index;
	}
    if(index > m_nTableSize)
	{
//		XExcept *pE = new XExcept(_T("Bad Index"));
//		pE->SetDateTime( __DATE__, __TIME__);
//		pE->SetSource( __FILE__ );
//		pE->SetLine( __LINE__ );
//		throw pE;
    }
    for( ; ; )
	{
        if ( m_pDict[index].codeValue == -1 )
            return( (unsigned int) index );
        if ( m_pDict[index].codeParent == parent_code &&
             m_pDict[index].charactor == (char) child_character )
            return( index );


        if ( (int) index >= offset )
            index -= offset;
        else
            index += m_nTableSize - offset;
    }
}
unsigned int XBuffCompressLZW::DecodeString( unsigned int count, unsigned int code )
{
    if(code > m_nTableSize )
	{
//		XExcept *pE = new XExcept(_T("Bad Code"));
//		pE->SetDateTime( __DATE__, __TIME__);
//		pE->SetSource( __FILE__ );
//		pE->SetLine( __LINE__ );
        return 0;
    }
    while ( code > 255 )
	{
		m_DecodeStack[ count++ ] = m_pDict[code].charactor;
        code = m_pDict[code].codeParent;
    }
    m_DecodeStack[ count++ ] = (char) code;
    return( count );
}



//
//	XBuffCompressLZ77
//
XBuffCompressLZ77::XBuffCompressLZ77(unsigned int nIndexBitCount/* = 12*/,
									 unsigned int nLengthBitCount/* = 4*/)
	:	XBuffCompress()
{

	m_nIndexBitCount          = nIndexBitCount;
	m_nLengthBitCount         = nLengthBitCount;
	m_nWindowSize             = 1 << m_nIndexBitCount;
	m_nRawLookAheadSize       = 1 << m_nLengthBitCount;
	m_nBreakEven              = ( 1 + m_nIndexBitCount + m_nLengthBitCount) / 9;
	m_nLookAheadSize          = m_nRawLookAheadSize + m_nBreakEven;
	m_nTreeRoot               = m_nWindowSize;
	m_nEndStream              = 0;
	m_nUnUsed                 = 0;

	m_Tree   = NULL;
	m_Window = NULL;

	m_nTmp    = 0;
}
XBuffCompressLZ77::~XBuffCompressLZ77()
{
	if( m_Tree != NULL )
		delete m_Tree;
	if( m_Window != NULL )
		delete m_Window;

	m_Tree   = NULL;
	m_Window = NULL;
}
bool XBuffCompressLZ77::Init()
{
	if( m_Tree != NULL )
		delete m_Tree;
	if( m_Window != NULL )
		delete m_Window;

	m_Tree   = NULL;
	m_Window = NULL;

	m_Tree    = new Tree[ m_nWindowSize + 1 ];
	m_Window  = new unsigned char[ m_nWindowSize ];
	if( m_Tree == NULL || m_Window == NULL )
		return false;
	return true;
}
int XBuffCompressLZ77::ModWindow( int b )
{
	return ( ( b ) & ( m_nWindowSize - 1 ) );
}
bool XBuffCompressLZ77::Compress()
{
	XBuffCompressLZ77   outBuff;		// 書き込みバッファ
    int			i;
    int			c;
    int			look_ahead_bytes;
    int			current_position;
    int			replace_count;
    int			match_length;
    int			match_position;

	if( Init() == false )
		return false;

	if( outBuff.Alloc( GetBuffSize() * 2  ) == NULL )
		return false;

    m_cMask              = 0x80;
	m_nRack              = 0;
	m_nPacifierCounter   = 0;

    current_position = 1;
    for( i = 0 ; i < m_nLookAheadSize ; i++ )
	{
		if( IsEOF() )
			break;
        c = getChar();
        m_Window[ current_position + i ] = (unsigned char) c;
    }
    look_ahead_bytes = i;
    InitTree( current_position );
    match_length = 0;
    match_position = 0;

	XBuffCompressLZ77 *pp = &outBuff;
    while ( look_ahead_bytes > 0 )
	{
        if ( match_length > look_ahead_bytes )
		{
            match_length = look_ahead_bytes;
		}
        if ( match_length <= m_nBreakEven )
		{
            replace_count = 1;
			XBuffCompressLZ77 *gh02 = &outBuff;
            outBuff.outputBit( 1 );
            outBuff.outputBits( (unsigned long) m_Window[ current_position ], 8 );
        } 
		else
		{
            outBuff.outputBit( 0 );
            outBuff.outputBits((unsigned long) match_position, m_nIndexBitCount );
            outBuff.outputBits((unsigned long) ( match_length - ( m_nBreakEven + 1 ) ), m_nLengthBitCount );
            replace_count = match_length;
		}
        for ( i = 0 ; i < replace_count ; i++ )
		{
            DeleteString( ModWindow( current_position + m_nLookAheadSize ) );
            if( IsEOF() == true )
			{
                look_ahead_bytes--;
			}
            else
			{
				c = getChar();
                m_Window[ ModWindow( current_position + m_nLookAheadSize ) ] = (unsigned char) c;
			}
            current_position = ModWindow( current_position + 1 );
            if ( look_ahead_bytes )
			{
                match_length = AddString( current_position, &match_position );
			}
        }
    };
    outBuff.outputBit( 0 );
    outBuff.outputBits( (unsigned long) m_nEndStream, m_nIndexBitCount );

	outBuff.closeOutPutPull();

	if( Alloc( outBuff.getCurrentByte() ) == NULL )
			return false;
	::memcpy(*this, outBuff, outBuff.getCurrentByte() );

	return true;
}

bool XBuffCompressLZ77::Expand()
{
	XBuffCompressLZ77   outBuff;		// 書き込みバッファ
    int			i;
    int			current_position;
    int			c;
    int			match_length;
    int			match_position;

	if( Init() == false )
		return false;

	if( outBuff.Alloc( GetBuffSize() * 2  ) == NULL )
		return false;

    m_cMask              = 0x80;
	m_nRack              = 0;
	m_nPacifierCounter   = 0;

    current_position = 1;
    for ( ; ; )
	{
        if ( inputBit() )
		{
            c = (int) inputBits( 8 );
            outBuff.putChar( c );
//			TCHAR* pp = outBuff;
            m_Window[ current_position ] = (unsigned char) c;
            current_position = ModWindow( current_position + 1 );
        }
		else
		{
            match_position = (int) inputBits( m_nIndexBitCount );
            if ( match_position == m_nEndStream )
                break;
            match_length = (int) inputBits( m_nLengthBitCount );
            match_length += m_nBreakEven;
            for ( i = 0 ; i <= match_length ; i++ )
			{
                c = m_Window[ ModWindow( match_position + i ) ];
				outBuff.putChar( c );
                m_Window[ current_position ] = (unsigned char) c;
                current_position = ModWindow( current_position + 1 );
            }
//			TCHAR* pp = outBuff;
        }
    }
	
	outBuff.closeOutPutPull();


	if( Alloc( outBuff.getCurrentByte() -1   ) == NULL )
			return false;
	::memcpy(*this, outBuff, outBuff.getCurrentByte() -1 );

	return true;
}
void XBuffCompressLZ77::InitTree( int r )
{
	memset(m_Tree, m_nUnUsed, sizeof(Tree) * (m_nWindowSize) );
    m_Tree[ m_nTreeRoot ].larger_child = r;
    m_Tree[ r ].parent = m_nTreeRoot;
    m_Tree[ r ].larger_child = m_nUnUsed;
    m_Tree[ r ].smaller_child = m_nUnUsed;
}
void XBuffCompressLZ77::ContractNode( int old_node, int new_node )
{
    m_Tree[ new_node ].parent = m_Tree[ old_node ].parent;
    if ( m_Tree[ m_Tree[ old_node ].parent ].larger_child == old_node )
	{
        m_Tree[ m_Tree[ old_node ].parent ].larger_child = new_node;
	}
    else
	{
        m_Tree[ m_Tree[ old_node ].parent ].smaller_child = new_node;
	}
    m_Tree[ old_node ].parent = m_nUnUsed;
}
void XBuffCompressLZ77::ReplaceNode( int old_node, int new_node )
{
    int parent;

    parent = m_Tree[ old_node ].parent;
    if ( m_Tree[ parent ].smaller_child == old_node )
	{
        m_Tree[ parent ].smaller_child = new_node;
	}
    else
	{
        m_Tree[ parent ].larger_child = new_node;
	}
    m_Tree[ new_node ] = m_Tree[ old_node ];
    m_Tree[ m_Tree[ new_node ].smaller_child ].parent = new_node;
    m_Tree[ m_Tree[ new_node ].larger_child  ].parent = new_node;
    m_Tree[ old_node ].parent = m_nUnUsed;
}
int XBuffCompressLZ77::FindNextNode( int node )
{
    int next;

    next = m_Tree[ node ].smaller_child;
    while ( m_Tree[ next ].larger_child != m_nUnUsed )
	{
        next = m_Tree[ next ].larger_child;
	}
    return( next );
}
void XBuffCompressLZ77::DeleteString( int p )
{
    int  replacement;

    if ( m_Tree[ p ].parent == m_nUnUsed )
	{
        return;
	}
    if ( m_Tree[ p ].larger_child == m_nUnUsed )
	{
        ContractNode( p, m_Tree[ p ].smaller_child );
	}
    else if ( m_Tree[ p ].smaller_child == m_nUnUsed )
	{
        ContractNode( p, m_Tree[ p ].larger_child );
	}
    else
	{
        replacement = FindNextNode( p );
        DeleteString( replacement );
        ReplaceNode( p, replacement );
    }
}
int  XBuffCompressLZ77::AddString( int new_node, int *match_position )
{
    int i;
    int test_node;
    int delta;
    int match_length;
    int *child;

    if ( new_node == m_nEndStream )
	{
        return( 0 );
	}
    test_node = m_Tree[ m_nTreeRoot ].larger_child;
    match_length = 0;
    for ( ; ; )
	{
        for ( i = 0 ; i < m_nLookAheadSize ; i++ )
		{
            delta = m_Window[ ModWindow( new_node + i ) ] -
                    m_Window[ ModWindow( test_node + i ) ];
            if ( delta != 0 )
                break;
        }
        if ( i >= match_length )
		{
            match_length = i;
            *match_position = test_node;
            if ( match_length >= m_nLookAheadSize )
			{
                ReplaceNode( test_node, new_node );
                return( match_length );
            }
        }
        if ( delta >= 0 )
		{
            child = &m_Tree[ test_node ].larger_child;
		}
        else
		{
            child = &m_Tree[ test_node ].smaller_child;
		}


        if ( *child == m_nUnUsed )
		{
            *child = new_node;
            m_Tree[ new_node ].parent = test_node;
            m_Tree[ new_node ].larger_child = m_nUnUsed;
            m_Tree[ new_node ].smaller_child = m_nUnUsed;
            return( match_length );
        }
        test_node = *child;
    }
}



//
//
//
XBuffCompressHuff::XBuffCompressHuff()
 :XBuffCompress()
{

	m_nEndIfStream         = 256;
	m_nEscape              = 257;
	m_nRootNode            = 0;
	m_nMaxWeight           = 0x8000;
	m_nSymbolCount         = 258;
	m_nNodeTableCount      = ( ( m_nSymbolCount * 2 ) - 1 );
	m_nTmp                 = 0;

	m_Tree.leaf            = NULL;
	m_Tree.nodes           = NULL;

}
XBuffCompressHuff::~XBuffCompressHuff()
{
	if( m_Tree.leaf  != NULL )
		delete [] m_Tree.leaf;
	if( m_Tree.nodes != NULL )
		delete [] m_Tree.nodes;

	m_Tree.leaf            = NULL;
	m_Tree.nodes           = NULL;
}
bool XBuffCompressHuff::Compress()
{
	XBuffCompressHuff   outBuff;		// 書き込みバッファ
    int                 c;

	if( outBuff.Alloc( GetBuffSize() * 2  ) == NULL )
		return false;

    m_cMask              = 0x80;
	m_nRack              = 0;
	m_nPacifierCounter   = 0;

	if( Init() == false )
		return false;

	InitializeTree();
    for(;;)
	{
        c = getChar(); 
		if( IsEOF() )
			break;
        EncodeSymbol( c, &outBuff );
        UpdateModel( c );
    }
	EncodeSymbol( m_nEndIfStream, &outBuff );

	outBuff.closeOutPutPull();
	
	if( Alloc( outBuff.getCurrentByte()    ) == NULL )
			return false;
	::memcpy(*this, outBuff, outBuff.getCurrentByte() );

	return true;
}
bool XBuffCompressHuff::Expand()
{
	XBuffCompressHuff   outBuff;		// 書き込みバッファ
    int                 c;

	if( outBuff.Alloc( GetBuffSize() * 2  ) == NULL ) {
		this->Free();
		return false;
	}

    m_cMask              = 0x80;
	m_nRack              = 0;
	m_nPacifierCounter   = 0;

	if( Init() == false ) {
		this->Free();
		return false;
	}

	InitializeTree();
    while ( ( c = DecodeSymbol() ) != m_nEndIfStream )
	{
        outBuff.putChar( c );
        UpdateModel( c );
    }
	outBuff.closeOutPutPull();

	if( outBuff.getCurrentByte()  == 0 ) {
		this->Free();
		return false;
	}
	
	if( Alloc( outBuff.getCurrentByte()   ) == NULL ) {
		this->Free();
		return false;
	}
	::memcpy(*this, outBuff, outBuff.getCurrentByte() );

	return true;
}
bool XBuffCompressHuff::Init()
{
	if( m_Tree.leaf  != NULL )
		delete [] m_Tree.leaf;
	if( m_Tree.nodes != NULL )
		delete [] m_Tree.nodes;

	m_Tree.leaf            = NULL;
	m_Tree.nodes           = NULL;

	m_Tree.leaf            = new int[ m_nSymbolCount + 1 ];
	m_Tree.nodes           = new HuffTree::node[ m_nNodeTableCount + 1 ];
	if( m_Tree.leaf == NULL || m_Tree.nodes == NULL )
		return false;

	return true;
}
void XBuffCompressHuff::InitializeTree()
{
    int i;
	for ( i = 0 ; i < m_nNodeTableCount ; i++ )
	{
		m_Tree.nodes[ i ].child             = -1;
		m_Tree.nodes[ i ].child_is_leaf     = -1;
		m_Tree.nodes[ i ].weight            = (unsigned int)-1;	// コンパイルのワーニング対応
		m_Tree.nodes[ i ].parent            = -1;
	}
    m_Tree.nodes[ m_nRootNode ].child             = m_nRootNode + 1;
    m_Tree.nodes[ m_nRootNode ].child_is_leaf     = 0;
    m_Tree.nodes[ m_nRootNode ].weight            = 2;
    m_Tree.nodes[ m_nRootNode ].parent            = -1;

    m_Tree.nodes[ m_nRootNode + 1 ].child         = m_nEndIfStream;
    m_Tree.nodes[ m_nRootNode + 1 ].child_is_leaf = 1;
    m_Tree.nodes[ m_nRootNode + 1 ].weight        = 1;
    m_Tree.nodes[ m_nRootNode + 1 ].parent        = m_nRootNode;
    m_Tree.leaf[ m_nEndIfStream ]                = m_nRootNode + 1;

    m_Tree.nodes[ m_nRootNode + 2 ].child         = m_nEscape;
    m_Tree.nodes[ m_nRootNode + 2 ].child_is_leaf = 1;
    m_Tree.nodes[ m_nRootNode + 2 ].weight        = 1;
    m_Tree.nodes[ m_nRootNode + 2 ].parent        = m_nRootNode;
    m_Tree.leaf[ m_nEscape ]                       = m_nRootNode + 2;

    m_Tree.next_free_node                       = m_nRootNode + 3;

	for ( i = 0 ; i < m_nEndIfStream ; i++ )
        m_Tree.leaf[ i ] = -1;

}
void XBuffCompressHuff::EncodeSymbol( unsigned int c, XBuffCompressHuff *pOutBuff)
{
    unsigned long code;
    unsigned long current_bit;
    int code_size;
    int current_node;

    code = 0;
    current_bit = 1;
    code_size = 0;
    current_node = m_Tree.leaf[ c ];
    if ( current_node == -1 )
	{
        current_node = m_Tree.leaf[ m_nEscape ];
	}
    while( current_node != m_nRootNode )
	{
        if( ( current_node & 1 ) == 0 )
		{
            code |= current_bit;
		}
        current_bit <<= 1;
        code_size++;
        current_node = m_Tree.nodes[ current_node ].parent;
    };
    pOutBuff->outputBits( code, code_size );
    if ( m_Tree.leaf[ c ] == -1 )
	{
        pOutBuff->outputBits( (unsigned long) c, 8 );
        AddNewNode( c );
    }
}
int XBuffCompressHuff::DecodeSymbol()
{
	//	
	//
	//
	//
    int current_node;
    int c;

    current_node = m_nRootNode;
    while ( !m_Tree.nodes[ current_node ].child_is_leaf )
	{
        current_node   = m_Tree.nodes[ current_node ].child;
        current_node   += inputBit();
    }
    c = m_Tree.nodes[ current_node ].child;
    if ( c == m_nEscape )
	{
        c = (int) inputBits( 8 );
        AddNewNode( c );
    }
    return( c );
}
void XBuffCompressHuff::AddNewNode( int c )
{
    int lightest_node;
    int new_node;
    int zero_weight_node;

    lightest_node = m_Tree.next_free_node - 1;
    new_node = m_Tree.next_free_node;
    zero_weight_node = m_Tree.next_free_node + 1;
    m_Tree.next_free_node += 2;

    m_Tree.nodes[ new_node ] = m_Tree.nodes[ lightest_node ];
    m_Tree.nodes[ new_node ].parent = lightest_node;
    m_Tree.leaf[ m_Tree.nodes[ new_node ].child ]	= new_node;

    m_Tree.nodes[ lightest_node ].child				= new_node;
    m_Tree.nodes[ lightest_node ].child_is_leaf		= 0;

    m_Tree.nodes[ zero_weight_node ].child          = c;
    m_Tree.nodes[ zero_weight_node ].child_is_leaf  = 1;
    m_Tree.nodes[ zero_weight_node ].weight         = 0;
    m_Tree.nodes[ zero_weight_node ].parent         = lightest_node;
    m_Tree.leaf[ c ] = zero_weight_node;
}
void XBuffCompressHuff::UpdateModel( int c )
{
    int current_node;
    int new_node;

    if ( m_Tree.nodes[ m_nRootNode].weight == (unsigned int)m_nMaxWeight )
	{
        RebuildTree();
	}
    current_node = m_Tree.leaf[ c ];
    while ( current_node != -1 )
	{
        m_Tree.nodes[ current_node ].weight++;
        for ( new_node = current_node ; new_node > m_nRootNode ; new_node-- )
		{
            if ( m_Tree.nodes[ new_node - 1 ].weight >= m_Tree.nodes[ current_node ].weight )
                break;
		}
        if ( current_node != new_node )
		{
            SwapNodes( current_node, new_node );
            current_node = new_node;
        }
        current_node = m_Tree.nodes[ current_node ].parent;
    }
}

void XBuffCompressHuff::RebuildTree( void )
{
    int i;
    int j;
    int k;
    unsigned int weight;

    j = m_Tree.next_free_node - 1;
    for ( i = j ; i >= m_nRootNode ; i-- )
	{
        if ( m_Tree.nodes[ i ].child_is_leaf )
		{
            m_Tree.nodes[ j ] = m_Tree.nodes[ i ];
            m_Tree.nodes[ j ].weight = ( m_Tree.nodes[ j ].weight + 1 ) / 2;
            j--;
        }
    }

    for ( i = m_Tree.next_free_node - 2 ; j >= m_nRootNode ; i -= 2, j-- )
	{
        k = i + 1;
        m_Tree.nodes[ j ].weight = m_Tree.nodes[ i ].weight +
                                   m_Tree.nodes[ k ].weight;
        weight = m_Tree.nodes[ j ].weight;
        m_Tree.nodes[ j ].child_is_leaf = 0;
        for ( k = j + 1 ; weight < m_Tree.nodes[ k ].weight ; k++ )
            ;
        k--;
        memmove( &(m_Tree.nodes[ j ]), &(m_Tree.nodes[ j + 1 ]), ( k - j ) * sizeof( struct _HuffTree::node ) );
        m_Tree.nodes[ k ].weight = weight;
        m_Tree.nodes[ k ].child = i;
        m_Tree.nodes[ k ].child_is_leaf = 0;
    }

    for ( i = m_Tree.next_free_node - 1 ; i >= m_nRootNode ; i-- )
	{
        if ( m_Tree.nodes[ i ].child_is_leaf )
		{
            k = m_Tree.nodes[ i ].child;
            m_Tree.leaf[ k ] = i;
        }
		else
		{
            k = m_Tree.nodes[ i ].child;
            m_Tree.nodes[ k ].parent = m_Tree.nodes[ k + 1 ].parent = i;
        }
    }
}
void XBuffCompressHuff::SwapNodes( int i, int j )
{
    HuffTree::node temp;

    if ( m_Tree.nodes[ i ].child_is_leaf )
	{
         m_Tree.leaf[ m_Tree.nodes[ i ].child ] = j;
	}
    else
	{
        m_Tree.nodes[ m_Tree.nodes[ i ].child ].parent = j;
        m_Tree.nodes[ m_Tree.nodes[ i ].child + 1 ].parent = j;
    }
    if( m_Tree.nodes[ j ].child_is_leaf )
	{
         m_Tree.leaf[ m_Tree.nodes[ j ].child ] = i;
	}
    else
	{
        m_Tree.nodes[ m_Tree.nodes[ j ].child ].parent = i;
        m_Tree.nodes[ m_Tree.nodes[ j ].child + 1 ].parent = i;
    }
    temp = m_Tree.nodes[ i ];
    m_Tree.nodes[ i ] = m_Tree.nodes[ j ];
    m_Tree.nodes[ i ].parent = temp.parent;
    temp.parent = m_Tree.nodes[ j ].parent;
    m_Tree.nodes[ j ] = temp;
}
