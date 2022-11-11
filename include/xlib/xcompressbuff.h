/**
 * @file xcompressbuff.h
 * @brief �o�b�t�@���k�N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_COMPRESSBUFF_HEASER_FILE__
#define __X_COMPRESSBUFF_HEASER_FILE__
#include "xlib/xbuff.h"

/**
 * �o�b�t�@�̈��k���z�N���X
 */
class XBuffCompress : public XBuff
{
public:
	/** �R���X�g���N�^ */
	XBuffCompress();
	/** �f�X�g���N�^ */
	virtual ~XBuffCompress();

	/** @copydoc XBuff::Alloc(size_t) */
	virtual void *Alloc(size_t nSize);
	/** @copydoc XBuff::Free() */
	virtual void Free();


	/**
	 * ���k���܂��B���z�����o�Ȃ̂ŁA�h����ŕK���C���v�������g���܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Compress() = 0;
	/**
	 * �W�J���܂��B���z�����o�Ȃ̂ŁA�h����ŕK���C���v�������g���܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Expand() = 0;
protected:
	/*
	 * �o�b�t�@����1Byte�ǂݍ��݁A�����|�C���^��1�i�߂܂�
	 * @retval true �Ō�ɒB����
	 * @retval false ����ȊO
	 */
	bool IsEOF();
	/*
	 * �o�b�t�@����1Byte�ǂݍ��݁A�����|�C���^��1�i�߂܂�
	 * @return �ǂݍ��܂ꂽ�PByte
	 */
	unsigned char getChar();
	/*
	 * �o�b�t�@����INT�l�������݁A�����|�C���^��1�i�߂܂��B�o�b�t�@�̃T�C�Y������Ȃ��ꍇ�́Am_iExpandAlloBuff�̒l���|�����l
	 * @param c �������ނPByte
	 * @retval true ����
	 * @retval false ���s
	 */
	bool putChar(unsigned char c);
	/*
	 * �o�b�t�@����LONG�l�������݁A�����|�C���^��1�i�߂܂��B�o�b�t�@�̃T�C�Y������Ȃ��ꍇ�́Am_iExpandAlloBuff�̒l���|�����l�B
	 * @param bit ��������LONG�l
	 */
	void outputBit(int bit);
	/*
	 * �o�b�t�@����LONG�l�������݁A�����|�C���^��1�i�߂܂��B�o�b�t�@�̃T�C�Y������Ȃ��ꍇ�́Am_iExpandAlloBuff�̒l���|�����l�B
	 * @param code ��������LONG�l
	 * @param count �}�X�N
	 */
	void outputBits(unsigned long code, int count);
	/*
	 * �}�X�N���A0x80�̏ꍇ�AIO����A�PByte�����b�N�ɓǂݍ���
	 * @return �ǂݍ��񂾂PBYTE
	 */
	int inputBit();
	/*
	 * �}�X�N���A0x80�̏ꍇ�AIO����A�PByte�����b�N�ɓǂݍ���
	 * @param bit_count �}�X�N
	 * @return �ǂݍ��񂾂PBYTE
	 */
	int inputBits(int bit_count);
	/*
	 * �������݃o�b�t�@�ŁA���b�N�ɒl�����݂���ꍇ�A�o�b�t�@�Ɏc��̃��b�N���������݂܂�
	 */
	void closeOutPutPull();
	/*
	 * �������݃o�b�t�@�ŁA���b�N�ɒl�����݂���ꍇ�A�o�b�t�@�Ɏc��̃��b�N���������݂܂�
	 * @return �J�����g�̃o�C�g
	 */
	unsigned int getCurrentByte();
protected:
	/* �J�����g�̃o�C�g */
	unsigned int    m_nCurrentByte;
	/* Mask */
    unsigned char   m_cMask;
	/** Rack */
	int             m_nRack;
	/* �J�E���^�[ */
	int             m_nPacifierCounter;
	/* �g���o�b�t�@ */
	double          m_iExpandAlloBuff;
	/* FillCounter */
	unsigned int    m_nPACIFIER_COUNT;
};

/**
 * �o�b�t�@�̈��kLZW�A���S���Y���N���X
 */
class XBuffCompressLZW : public XBuffCompress
{
public:
	/**
	 * �R���X�g���N�^
	 * @param nTableSize �e�[�u���T�C�Y
	 * @param nEndOfStream �X�g���[���̍Ō�
	 * @param nBumpCode �o���v�R�[�h
	 * @param nFlushCode �t���b�V���R�[�h
	 * @param nFirstCode �t�@�[�X�g�R�[�h
	 * @param nBits Bits�R�[�h
	 */
	XBuffCompressLZW(unsigned int nTableSize    = 35023,
		             unsigned int nEndOfStream  = 256,
					 unsigned int nBumpCode     = 257,
					 unsigned int nFlushCode    = 258,
					 unsigned int nFirstCode    = 259,
					 unsigned int nBits         = 15);
	/** �f�X�g���N�^ */
	virtual ~XBuffCompressLZW();
	/** @copydoc XBuffCompress::Compress() */
	virtual bool Compress();
	/** @copydoc XBuffCompress::Expand() */
	virtual bool Expand();



protected:
	/* �f�B���N�g���̏����� */
	void InitializeDictionary();
	/*
	 * �������[�̏�����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool InitializeStorage();
	/*
	 * �R�[�h�̍ő�l���擾
	 * @return �R�[�h�̍ő�l
	 */
	unsigned int MaxCode();
	/*
	 * �I���R�[�h�����߂܂��B
	 * @param parent_code �e
	 * @param child_character �q
	 * @return �I���R�[�h
	 */
	unsigned int FindChildNode( int parent_code, int child_character );
	/*
	 * �f�R�[�h���܂�
	 * @param count �J�E���g
	 * @param code �R�[�h
	 * @return �J�E���g
	 */
	unsigned int DecodeString( unsigned int count, unsigned int code );


private:
	/*
	 * LZW�f�B���N�g���\���́B
	 * @note Dictionary�̒�`�ł��B���ꂼ���Dictionary�̃G���g���ɂ̓R�[�h�������܂��B
	 * codeParent,charactor���ꂼ��̃R�[�h�́A�Q�̃y�A����K������܂��B
	 * �R�[�h�̒l��256��菬���Ȓl�́A�����i�Í������Ă��Ȃ�TEXT�j���܂݂܂��B
	 */
	typedef struct _Dictionary
	{
		/* �R�[�g�l */
		int    codeValue;
		/* �e�R�[�h */
		int    codeParent;
		/* �l */
		char   charactor;
	}Dictionary;
	
	/* �f�B���N�g���f�[�^ */
	Dictionary         *m_pDict;
	/* �f�R�[�h�̊� */
	unsigned char*  m_DecodeStack;
	/* Dictionary�ɒǉ�����Ƃ��C���f�b�N�X�B */
	unsigned  int   m_nNextCode;
	/* ���݂�Bit�R�[�h��ۑ� */
	int             m_nCurrentCodeBits;
	/* ���ɃW�����v���郏�[�h�T�C�Y�̃g���K�[ */
	unsigned int    m_nNextBumpCode;
	/* �f�B���N�g���̃e�[�u���T�C�Y */
	unsigned int    m_nTableSize;
	/* �X�g���[���̍Ō�̃R�[�h */
	unsigned int    m_nEndOfStream;
	/* �o���v�R�[�h */
	unsigned int    m_nBumpCode;
	/* �t���b�V���R�[�h�@*/
	unsigned int    m_nFlushCode;
	/* �t�@�[�X�g�R�[�h */
	unsigned int    m_nFirstCode;
	/* �r�b�g�T�C�Y */
	unsigned int    m_nBits;
	/*  �e���v�ϐ�*/
	int             m_nTmp;
};

/**
 * �o�b�t�@�̈��kLZ77�A���S���Y���N���X
 */
class XBuffCompressLZ77 : public XBuffCompress
{
public:
	/**
	 * �R���X�g���N�^
	 * @param nIndexBitCount �Q��BIT�J�E���g
	 * @param nLengthBitCount BIT�J�E���g��
	 */
	XBuffCompressLZ77(unsigned int nIndexBitCount = 12, unsigned int nLengthBitCount = 4);
	/** �f�X�g���N�^ */
	virtual ~XBuffCompressLZ77();
	/** @copydoc XBuffCompress::Compress() */
	virtual bool Compress();
	/** @copydoc XBuffCompress::Expand() */
	virtual bool Expand();
protected:
	/*
	 * Tree�����������܂�
	 * @param r ����������ŏ��̃��[�g�|�W�V����
	 */
	void InitTree( int r );
	/*
	 * �w�肵�����m�[�h���A�V�m�[�h�ɃZ�b�g���A���m�[�h�𖢎g�p�ɃZ�b�g���܂��B
	 * @param old_node �ݒ肷�鋌�m�[�h�̃|�W�V����
	 * @param new_node �ݒ肷��V�m�[�h�̃|�W�V����
	 */
	void ContractNode( int old_node, int new_node );
	/*
	 * �w�肵�����m�[�h�J�����A�V�m�[�h�ɃZ�b�g���܂��B
	 * @param old_node �J������m�[�h�̃|�W�V����
	 * @param new_node ����ւ���m�[�h�̃|�W�V����
	 */
	void ReplaceNode( int old_node, int new_node );
	/*
	 * �w�肵���m�[�h�̎��̃|�W�V�������擾���܂��B
	 * @param node �擾����O�̃m�[�h�̃|�W�V����
	 * @return ���������m�[�h�|�W�V����
	 */
	int  FindNextNode( int node );
	/*
	 * �w�肵���m�[�h�|�W�V�������N���A���܂�
	 * @param p �N���A����m�[�h�|�W�V����
	 */
	void DeleteString( int p );
	/*
	 * �m�[�h��ǉ����܂��B
	 * @param new_node �ǉ�����m�[�h�̃|�W�V����
	 * @param match_position ���������|�W�V�����̃A�h���X
	 * @return �ǉ������c���[�̈ʒu
	 */
	int  AddString( int new_node, int *match_position );
	/*
	 * �E�B���h�E�o�b�t�@�̈ʒu�𐮌`���ĕԂ��܂��B�w�肵���ʒu���A�I�[�o�[���Ă�����ꍇ��0��Ԃ��܂��B
	 * @param b �E�C���h�E�o�b�t�@�̈ʒu
	 * @return �E�B���h�E�o�b�t�@�̈ʒu
	 */
	int ModWindow( int b );
	/*
	 * ���������܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Init();

	/*
	 * LZ77�f�B���N�g���\���́B
	 */
    typedef struct _Tree {
		/* �e */
        int parent;
		/* �O�̎q */
        int smaller_child;
		/* ��̎q */
        int larger_child;
    } Tree;
private:
	/* �c�[���[�o�b�t�@ */
	Tree            *m_Tree;
	/* �E�C���h�E�o�b�t�@ */
	unsigned char   *m_Window;
	/* �C���f�b�N�X�̃r�b�g�J�E���g */
	int				m_nIndexBitCount;
	/* �r�b�g�J�E���g���� */
	int				m_nLengthBitCount;
	/* �E�C���h�E�T�C�Ym_nIndexBitCount�̍��r�b�g�V�t�g�l */
	int				m_nWindowSize;
	/* �E�C���h�E�w�b�h�̃I�t�Z�b�gm_nLengthBitCount�̍��r�b�g�V�t�g�l */
	int				m_nRawLookAheadSize;
	/* �E�C���h�E�u���C�N���̒l(1+m_nIndexBitCount+m_nLengthBitCount/9 */
	int				m_nBreakEven;
	/* �E�C���h�E�w�b�h�̃I�t�Z�b�g�Bm_nRawLookAheadSize+m_nBreakEven */
	int				m_nLookAheadSize;
	/* �c���[�̃��[�g�̈ʒu */
	int				m_nTreeRoot;
	/* �c���[�̍Ō� */
	int				m_nEndStream;
	/* �c���[�̐e�̖��g�p�̒l */
	int				m_nUnUsed;
	/* �e���v�l */
	int             m_nTmp;
};

/**
 * �o�b�t�@�̈��k�n�t�}���A���S���Y���N���X
 */
class XBuffCompressHuff : public XBuffCompress
{
public:
	/** �R���X�g���N�^ */
	XBuffCompressHuff();
	/** �f�X�g���N�^ */
	~XBuffCompressHuff();
	/** @copydoc XBuffCompress::Compress() */
	virtual bool Compress();
	/** @copydoc XBuffCompress::Expand() */
	virtual bool Expand();
protected:
	/*
	 * �c���[�����������܂�
	 */
	void InitializeTree();
	/*
	 * �V���{�����G���R�[�h���܂���
	 * @param c �G���R�[�h����c���[�̃|�W�V����
	 * @param pOutBuff �������ރo�b�t�@�̃A�h���X
	 */
	void EncodeSymbol( unsigned int c, XBuffCompressHuff *pOutBuff);
	/*
	 * �V���{�����f�R�[�h���܂�
	 * @return �ϊ���̃T�C�Y
	 */
	int  DecodeSymbol();
	/*
	 * ���f���̍X�V
	 * @param c �X�V����l
	 */
	void UpdateModel( int c );
	/*
	 * �c���[���č\�z���܂��B
	 */
	void RebuildTree();
	/*
	 * �m�[�h���X���b�v���܂��B
	 * @param i �X���b�v����m�[�h�̃|�W�V����
	 * @param j �X���b�v����m�[�h�̃|�W�V����
	 */
	void SwapNodes( int i, int j );
	/*
	 * �m�[�h��ǉ����܂��B
	 * @param c �ǉ�����m�[�h�̃|�W�V����
	 */
	void AddNewNode( int c );
	/*
	 * ���������܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool Init();
private:
	/*
	 * �n�t�}���f�B���N�g���\���́B
	 */
    typedef struct _HuffTree {
		/* �A�C�e�� */
		int *leaf;
		/* ���̃m�[�h */
        int next_free_node;
		/*
		 * �m�[�h
		 */
		typedef struct node {
			/* �d�� */
            unsigned int weight;
			/* �e */
            int parent;
			/* Tail */
            int child_is_leaf;
			/* �q */
            int child;
        }_node;
		/** �m�[�h */
		_node  *nodes;
	}HuffTree;
	/* �n�t�}���c���[�I�u�W�F�N�g */
    HuffTree    m_Tree;
	/* �X�g���[���̍Ōォ�ǂ��� */
	int         m_nEndIfStream;
	/* �G�X�P�[�v */
	int         m_nEscape;
	/* ���[�g�m�[�h�|�W�V���� */
	int         m_nRootNode;
	/* �d���̍ő�l */
	int         m_nMaxWeight;
	/* �V���{���� */
	int         m_nSymbolCount;
	/* �e�[�u���� */
	int         m_nNodeTableCount;
	/* �e���v�l */
	int         m_nTmp;
};



#endif // __X_COMPRESSBUFF_HEASER_FILE__
