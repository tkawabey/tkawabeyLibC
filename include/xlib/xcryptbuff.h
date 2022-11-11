/**
 * @file xcryptbuff.h
 * @brief バッファ暗号クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_CRYPTBUFF_HEASER_FILE__
#define __X_CRYPTBUFF_HEASER_FILE__





#include "xlib/xstrbuff.h"
#include "xlib/xfile.h"
#include "xlib/xeception.h"

/**
 * トリプルDESコーダクラス
 */
class XBuffD3DES : public XBuff
{
public:
	/** コンストラクタ */
	XBuffD3DES();
	/**
	 * コンストラクタ
	 * @param[in] lpszKey キー
	 */
	XBuffD3DES(const char* lpszKey);
	/**
	 * コンストラクタ
	 * @param[in] lpszKey キー
	 */
	XBuffD3DES(const wchar_t* lpszKey);
	/** デストラクタ */
	virtual ~XBuffD3DES();


	/** @copydoc XBuff::Alloc(size_t) */
	virtual void *Alloc(size_t nSize);
	/** @copydoc XBuff::Free() */
	virtual void Free();
	/** @copydoc XBuff::GetBuffSize() */
	virtual size_t GetBuffSize();
	

	/**
	 * キーをセットします。
	 * @param[in] lpszKey キー
	 */
	void SetKey(const char* lpszKey);
	/**
	 * キーをセットします。
	 * @param[in] lpszKey キー
	 */
	void SetKey(const wchar_t* lpszKey);

	/**
	 * 暗号化
	 */
	void EnCrypt();
	/**
	 * 復号化
	 */
	void DeCrypt();
protected:
	/**
	 * DESKey
	 * @param key キー
	 * @param edf バッファ
	 */
	void deskey(unsigned char *key, int edf);
	/**
	 * ロウ
	 * @param raw1 ロウ
	 */
	void cookey(unsigned long *raw1);
	/**
	 * フロム
	 * @param from フロム
	 */
	void usekey(unsigned long *from);
	/**
	 * コピー
	 * @param into コピー
	 */
	void cpkey(unsigned long *into);
	/**
	 * DES
	 * @param inblock 入力
	 * @param outblock 出力
	 */
	void des(unsigned char *inblock, unsigned char  *outblock);
	/**
	 * DES
	 * @param block ブロック
	 * @param keys キー
	 */
	static void desfunc(unsigned long *block, unsigned long *keys);
	/**
	 * DES
	 * @param outof 出力
	 * @param into 入力
	 */
	static void scrunch(unsigned char *outof, unsigned long *into);
	/**
	 * DES
	 * @param outof 出力
	 * @param into 入力
	 */
	static void unscrun(unsigned long *outof, unsigned char *into);
protected:
	/** 実サイズ */
	size_t		m_nRealSize;
	/** オフセット */
	unsigned    int  m_offSet;
	/** キー */
	unsigned	char m_key[8];
	/** ハッシュデータ */
	unsigned	long KnL[32];
	/** ハッシュデータ */
	unsigned	long KnR[32];
	/** ハッシュデータ */
	unsigned	long Kn3[32];
};

/**
 * MD5エラー
 */
class XMD5Except : XExcept
{
public:
	/** コンストラクタ */
	XMD5Except(){m_nErrorCode = 0; }
	/** デストラクタ */
	virtual ~XMD5Except() throw () {};
	/**
	 * エラーコードを取得します。
	 * @retval 0 Finalizeがコールされていません
	 * @retval 1 Finalizeが既にコールされています
	 * @retval 2 メモリ確保エラー
	 */
	int getErrorCode(){ return m_nErrorCode; }

	/**
	 * エラーをスローします。
	 * @param nErrorCode エラーコード
	 */
	static void Throw(int nErrorCode) {
		XMD5Except *p = new XMD5Except;
		p->m_nErrorCode = nErrorCode;
		throw p;
	}
protected:
	/** エラーコード */
	int		m_nErrorCode;
};

/**
 * MD5を使用し、バッファ内のデータからダイジェスト値を取得
 */
class XMD5 : public XStrBuff
{
private:
	/** ４バイトのサイズ定義 */
	typedef unsigned       int uint4;
	/** ２バイトのサイズ定義 */
	typedef unsigned short int uint2;
	/** １バイトのサイズ定義 */
	typedef unsigned      char uint1;
public:
	/** コンストラクタ */
	XMD5();
	/**
	 * コンストラクタ
	 * @param p 指定した文字列で、ダイジェストを作成
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	XMD5(const char* p);
	/** @copydoc XMD5::XMD5(const char*) */
	XMD5(const wchar_t* p);
	/**
	 * 文字列で、ダイジェストを作成
	 * @param lpsz 文字列
	 * @return 自身
	 */
	const XMD5& operator=(const char* lpsz);
	/**
	 * 文字列で、ダイジェストを作成
	 * @param lpsz 文字列
	 * @return 自身
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	const XMD5& operator=(const wchar_t* lpsz);
	/**
	 * 文字列を足しこんで、ダイジェストを再作成
	 * @param lpsz 文字列
	 * @return 自身
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	const XMD5& operator+=(const char* lpsz);
	/**
	 * 文字列を足しこんで、ダイジェストを再作成
	 * @param lpsz 文字列
	 * @return 自身
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	const XMD5& operator+=(const wchar_t* lpsz);


	/**
	 * オブジェクト内に設定されたデータから、MD5でハッシュをかけたダイジェストを求めます
	 * @exception XMD5Except* エラーオブジェクト。
	 */ 
	void Make();
	/**
	 * オープンされたファイルオブジェクトから、MD5でハッシュをかけたダイジェストを求めます
	 * @param[in] pFile オープンしたファイルのインスタンスのアドレス。
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	void Make(XFile *pFile);

	/**
	 * 指定された入力情報を基に、ダイジェストをスクランブルします。
	 * @param input 入力データのアドレス
	 * @param input_length 入力データのサイズ（BYTE）
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	void Update(uint1 *input, uint4 input_length);
	/**
	 * Updateメソッドをコールした結果ダイジェストを確定する時にコールします。
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	void Finalize();
	/**
	 * ダイジェストの生データを取得します。
	 * @param pBuff 生データを受け取るバッファ
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	void RawDigest(XBuff *pBuff);
	/**
	 * ダイジェストをASCII形式に変換した文字を取得します。
	 * @return ダイジェスト文字列
	 * @exception XMD5Except* エラーオブジェクト。
	 */
	XStrBuff HexDigest() const;
private:
	/**
	 * メンバを初期化します。
	 */
	void Init();
	/**
	 * blockによって、スクランブルします。
	 * @param block 入力
	 */
	void Transform(uint1 *block);
	

	/**
	 * エンコード
	 * @param dest (null)
	 * @param src (null)
	 * @param length (null)
	 */
	static void encode(uint1 *dest, uint4 *src, uint4 length);
	/**
	 * デコード
	 * @param dest (null)
	 * @param src (null)
	 * @param length (null)
	 */
	static void decode(uint4 *dest, uint1 *src, uint4 length);
	/**
	 * メモリーコピー
	 * @param dest (null)
	 * @param src (null)
	 * @param length (null)
	 */
	static void memcpy(uint1 *dest, uint1 *src, uint4 length);
	/**
	 * メモリーセットー
	 * @param start (null)
	 * @param val (null)
	 * @param length (null)
	 */
	static void memset(uint1 *start, uint1 val, uint4 length);

	/**
	 * @param x (null)
	 * @param n (null)
	 * @return
	 */
	static inline uint4  rotate_left(uint4 x, uint4 n);
	/**
	 * @param x (null)
	 * @param y (null)
	 * @param z (null)
	 * @return
	 */
	static inline uint4  F(uint4 x, uint4 y, uint4 z);
	/**
	 * @param x (null)
	 * @param y (null)
	 * @param z (null)
	 * @return
	 */
	static inline uint4  G(uint4 x, uint4 y, uint4 z);
	/**
	 * @param x (null)
	 * @param y (null)
	 * @param z (null)
	 * @return
	 */
	static inline uint4  H(uint4 x, uint4 y, uint4 z);
	/**
	 * @param x (null)
	 * @param y (null)
	 * @param z (null)
	 * @return
	 */
	static inline uint4  I(uint4 x, uint4 y, uint4 z);
	/**
	 * @param a (null)
	 * @param b (null)
	 * @param c (null)
	 * @param d (null)
	 * @param x (null)
	 * @param s (null)
	 * @param ac (null)
	 * @return
	 */
	static inline void   FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
	/**
	 * @param a (null)
	 * @param b (null)
	 * @param c (null)
	 * @param d (null)
	 * @param x (null)
	 * @param s (null)
	 * @param ac (null)
	 * @return
	 */
	static inline void   GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
	/**
	 * @param a (null)
	 * @param b (null)
	 * @param c (null)
	 * @param d (null)
	 * @param x (null)
	 * @param s (null)
	 * @param ac (null)
	 * @return
	 */
	static inline void   HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
	/**
	 * @param a (null)
	 * @param b (null)
	 * @param c (null)
	 * @param d (null)
	 * @param x (null)
	 * @param s (null)
	 * @param ac (null)
	 * @return
	 */
	static inline void   II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);

	uint4 m_state[4];
	uint4 m_count[2];
	uint1 m_buffer[64];
	uint1 m_digest[16];
	uint1 m_finalized;
};

#endif// __X_CRYPTBUFF_HEASER_FILE__

