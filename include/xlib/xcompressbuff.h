/**
 * @file xcompressbuff.h
 * @brief バッファ圧縮クラス
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
 * バッファの圧縮仮想クラス
 */
class XBuffCompress : public XBuff
{
public:
	/** コンストラクタ */
	XBuffCompress();
	/** デストラクタ */
	virtual ~XBuffCompress();

	/** @copydoc XBuff::Alloc(size_t) */
	virtual void *Alloc(size_t nSize);
	/** @copydoc XBuff::Free() */
	virtual void Free();


	/**
	 * 圧縮します。仮想メンバなので、派生先で必ずインプリメントします
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Compress() = 0;
	/**
	 * 展開します。仮想メンバなので、派生先で必ずインプリメントします
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Expand() = 0;
protected:
	/*
	 * バッファから1Byte読み込み、内部ポインタを1進めます
	 * @retval true 最後に達した
	 * @retval false それ以外
	 */
	bool IsEOF();
	/*
	 * バッファから1Byte読み込み、内部ポインタを1進めます
	 * @return 読み込まれた１Byte
	 */
	unsigned char getChar();
	/*
	 * バッファからINT値書き込み、内部ポインタを1進めます。バッファのサイズが足らない場合は、m_iExpandAlloBuffの値を掛けた値
	 * @param c 書き込む１Byte
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool putChar(unsigned char c);
	/*
	 * バッファからLONG値書き込み、内部ポインタを1進めます。バッファのサイズが足らない場合は、m_iExpandAlloBuffの値を掛けた値。
	 * @param bit 書き込むLONG値
	 */
	void outputBit(int bit);
	/*
	 * バッファからLONG値書き込み、内部ポインタを1進めます。バッファのサイズが足らない場合は、m_iExpandAlloBuffの値を掛けた値。
	 * @param code 書き込むLONG値
	 * @param count マスク
	 */
	void outputBits(unsigned long code, int count);
	/*
	 * マスクが、0x80の場合、IOから、１Byteをラックに読み込む
	 * @return 読み込んだ１BYTE
	 */
	int inputBit();
	/*
	 * マスクが、0x80の場合、IOから、１Byteをラックに読み込む
	 * @param bit_count マスク
	 * @return 読み込んだ１BYTE
	 */
	int inputBits(int bit_count);
	/*
	 * 書き込みバッファで、ラックに値が存在する場合、バッファに残りのラックを書き込みます
	 */
	void closeOutPutPull();
	/*
	 * 書き込みバッファで、ラックに値が存在する場合、バッファに残りのラックを書き込みます
	 * @return カレントのバイト
	 */
	unsigned int getCurrentByte();
protected:
	/* カレントのバイト */
	unsigned int    m_nCurrentByte;
	/* Mask */
    unsigned char   m_cMask;
	/** Rack */
	int             m_nRack;
	/* カウンター */
	int             m_nPacifierCounter;
	/* 拡張バッファ */
	double          m_iExpandAlloBuff;
	/* FillCounter */
	unsigned int    m_nPACIFIER_COUNT;
};

/**
 * バッファの圧縮LZWアルゴリズムクラス
 */
class XBuffCompressLZW : public XBuffCompress
{
public:
	/**
	 * コンストラクタ
	 * @param nTableSize テーブルサイズ
	 * @param nEndOfStream ストリームの最後
	 * @param nBumpCode バンプコード
	 * @param nFlushCode フラッシュコード
	 * @param nFirstCode ファーストコード
	 * @param nBits Bitsコード
	 */
	XBuffCompressLZW(unsigned int nTableSize    = 35023,
		             unsigned int nEndOfStream  = 256,
					 unsigned int nBumpCode     = 257,
					 unsigned int nFlushCode    = 258,
					 unsigned int nFirstCode    = 259,
					 unsigned int nBits         = 15);
	/** デストラクタ */
	virtual ~XBuffCompressLZW();
	/** @copydoc XBuffCompress::Compress() */
	virtual bool Compress();
	/** @copydoc XBuffCompress::Expand() */
	virtual bool Expand();



protected:
	/* ディレクトリの初期化 */
	void InitializeDictionary();
	/*
	 * メモリーの初期化
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool InitializeStorage();
	/*
	 * コードの最大値を取得
	 * @return コードの最大値
	 */
	unsigned int MaxCode();
	/*
	 * 終了コードを求めます。
	 * @param parent_code 親
	 * @param child_character 子
	 * @return 終了コード
	 */
	unsigned int FindChildNode( int parent_code, int child_character );
	/*
	 * デコードします
	 * @param count カウント
	 * @param code コード
	 * @return カウント
	 */
	unsigned int DecodeString( unsigned int count, unsigned int code );


private:
	/*
	 * LZWディレクトリ構造体。
	 * @note Dictionaryの定義です。それぞれのDictionaryのエントリにはコードを持ちます。
	 * codeParent,charactorそれぞれのコードは、２つのペアから必ず作られます。
	 * コードの値が256より小さな値は、平分（暗号化していないTEXT）を含みます。
	 */
	typedef struct _Dictionary
	{
		/* コート値 */
		int    codeValue;
		/* 親コード */
		int    codeParent;
		/* 値 */
		char   charactor;
	}Dictionary;
	
	/* ディレクトリデータ */
	Dictionary         *m_pDict;
	/* デコードの間 */
	unsigned char*  m_DecodeStack;
	/* Dictionaryに追加するときインデックス。 */
	unsigned  int   m_nNextCode;
	/* 現在のBitコードを保存 */
	int             m_nCurrentCodeBits;
	/* 次にジャンプするワードサイズのトリガー */
	unsigned int    m_nNextBumpCode;
	/* ディレクトリのテーブルサイズ */
	unsigned int    m_nTableSize;
	/* ストリームの最後のコード */
	unsigned int    m_nEndOfStream;
	/* バンプコード */
	unsigned int    m_nBumpCode;
	/* フラッシュコード　*/
	unsigned int    m_nFlushCode;
	/* ファーストコード */
	unsigned int    m_nFirstCode;
	/* ビットサイズ */
	unsigned int    m_nBits;
	/*  テンプ変数*/
	int             m_nTmp;
};

/**
 * バッファの圧縮LZ77アルゴリズムクラス
 */
class XBuffCompressLZ77 : public XBuffCompress
{
public:
	/**
	 * コンストラクタ
	 * @param nIndexBitCount 参照BITカウント
	 * @param nLengthBitCount BITカウント長
	 */
	XBuffCompressLZ77(unsigned int nIndexBitCount = 12, unsigned int nLengthBitCount = 4);
	/** デストラクタ */
	virtual ~XBuffCompressLZ77();
	/** @copydoc XBuffCompress::Compress() */
	virtual bool Compress();
	/** @copydoc XBuffCompress::Expand() */
	virtual bool Expand();
protected:
	/*
	 * Treeを初期化します
	 * @param r 初期化する最初のルートポジション
	 */
	void InitTree( int r );
	/*
	 * 指定した旧ノードを、新ノードにセットし、旧ノードを未使用にセットします。
	 * @param old_node 設定する旧ノードのポジション
	 * @param new_node 設定する新ノードのポジション
	 */
	void ContractNode( int old_node, int new_node );
	/*
	 * 指定した旧ノード開放し、新ノードにセットします。
	 * @param old_node 開放するノードのポジション
	 * @param new_node 入れ替えるノードのポジション
	 */
	void ReplaceNode( int old_node, int new_node );
	/*
	 * 指定したノードの次のポジションを取得します。
	 * @param node 取得する前のノードのポジション
	 * @return 見つかったノードポジション
	 */
	int  FindNextNode( int node );
	/*
	 * 指定したノードポジションをクリアします
	 * @param p クリアするノードポジション
	 */
	void DeleteString( int p );
	/*
	 * ノードを追加します。
	 * @param new_node 追加するノードのポジション
	 * @param match_position 見つかったポジションのアドレス
	 * @return 追加したツリーの位置
	 */
	int  AddString( int new_node, int *match_position );
	/*
	 * ウィンドウバッファの位置を整形して返します。指定した位置が、オーバーしていいる場合は0を返します。
	 * @param b ウインドウバッファの位置
	 * @return ウィンドウバッファの位置
	 */
	int ModWindow( int b );
	/*
	 * 初期化します。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Init();

	/*
	 * LZ77ディレクトリ構造体。
	 */
    typedef struct _Tree {
		/* 親 */
        int parent;
		/* 前の子 */
        int smaller_child;
		/* 後の子 */
        int larger_child;
    } Tree;
private:
	/* ツーリーバッファ */
	Tree            *m_Tree;
	/* ウインドウバッファ */
	unsigned char   *m_Window;
	/* インデックスのビットカウント */
	int				m_nIndexBitCount;
	/* ビットカウント長さ */
	int				m_nLengthBitCount;
	/* ウインドウサイズm_nIndexBitCountの左ビットシフト値 */
	int				m_nWindowSize;
	/* ウインドウヘッドのオフセットm_nLengthBitCountの左ビットシフト値 */
	int				m_nRawLookAheadSize;
	/* ウインドウブレイクじの値(1+m_nIndexBitCount+m_nLengthBitCount/9 */
	int				m_nBreakEven;
	/* ウインドウヘッドのオフセット。m_nRawLookAheadSize+m_nBreakEven */
	int				m_nLookAheadSize;
	/* ツリーのルートの位置 */
	int				m_nTreeRoot;
	/* ツリーの最後 */
	int				m_nEndStream;
	/* ツリーの親の未使用の値 */
	int				m_nUnUsed;
	/* テンプ値 */
	int             m_nTmp;
};

/**
 * バッファの圧縮ハフマンアルゴリズムクラス
 */
class XBuffCompressHuff : public XBuffCompress
{
public:
	/** コンストラクタ */
	XBuffCompressHuff();
	/** デストラクタ */
	~XBuffCompressHuff();
	/** @copydoc XBuffCompress::Compress() */
	virtual bool Compress();
	/** @copydoc XBuffCompress::Expand() */
	virtual bool Expand();
protected:
	/*
	 * ツリーを初期化します
	 */
	void InitializeTree();
	/*
	 * シンボルをエンコードしまうす
	 * @param c エンコードするツリーのポジション
	 * @param pOutBuff 書き込むバッファのアドレス
	 */
	void EncodeSymbol( unsigned int c, XBuffCompressHuff *pOutBuff);
	/*
	 * シンボルをデコードします
	 * @return 変換後のサイズ
	 */
	int  DecodeSymbol();
	/*
	 * モデルの更新
	 * @param c 更新する値
	 */
	void UpdateModel( int c );
	/*
	 * ツリーを再構築します。
	 */
	void RebuildTree();
	/*
	 * ノードをスワップします。
	 * @param i スワップするノードのポジション
	 * @param j スワップするノードのポジション
	 */
	void SwapNodes( int i, int j );
	/*
	 * ノードを追加します。
	 * @param c 追加するノードのポジション
	 */
	void AddNewNode( int c );
	/*
	 * 初期化します
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Init();
private:
	/*
	 * ハフマンディレクトリ構造体。
	 */
    typedef struct _HuffTree {
		/* アイテム */
		int *leaf;
		/* 次のノード */
        int next_free_node;
		/*
		 * ノード
		 */
		typedef struct node {
			/* 重さ */
            unsigned int weight;
			/* 親 */
            int parent;
			/* Tail */
            int child_is_leaf;
			/* 子 */
            int child;
        }_node;
		/** ノード */
		_node  *nodes;
	}HuffTree;
	/* ハフマンツリーオブジェクト */
    HuffTree    m_Tree;
	/* ストリームの最後かどうか */
	int         m_nEndIfStream;
	/* エスケープ */
	int         m_nEscape;
	/* ルートノードポジション */
	int         m_nRootNode;
	/* 重さの最大値 */
	int         m_nMaxWeight;
	/* シンボル数 */
	int         m_nSymbolCount;
	/* テーブル数 */
	int         m_nNodeTableCount;
	/* テンプ値 */
	int         m_nTmp;
};



#endif // __X_COMPRESSBUFF_HEASER_FILE__
