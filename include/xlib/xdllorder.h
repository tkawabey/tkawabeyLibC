/**
 * @file xdllorder.h
 * @brief 動的リンクを行うローダ
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_DL_LORDER_FILE__
#define __X_DL_LORDER_FILE__

#include "xlib/xlib_head.h"



/**	@brief	動的リンクを行うローダ */
class XLIB_API XDlLorder
{
public:
	/**	@brief	コンストラクタ */
	XDlLorder();
	/**	@brief	デストラクタ */
	virtual ~XDlLorder();

	/**	@brief	シェアーライブラリーをロード
	 *
	 *	@param	filename	ファイル名
	 *	@param	flag	フラグ。OSに依存します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool open(const char* filename, int flag = 0);

	/**	@brief	シェアーライブラリーをロード
	 *
	 *	@param	filename	ファイル名
	 *	@param	flag	フラグ。OSに依存します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	virtual bool open(const wchar_t* filename, int flag = 0);
	/**	@brief	シェアーライブラリーをアンロード
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗	
	 */
	virtual bool close();

	/**	@brief	シンボル名の文字列を引き数に取り、そのシンボルがロードされたメモリのアドレスを返す。
	 *
	 *	@param	symbol	シンボル名の文字列
	 *
	 *	@return	ンボルがロードされたメモリのアドレス
	 */
	virtual void* loadSymbol(const char* symbol); 

	/**	@brief	シンボル名の文字列を引き数に取り、そのシンボルがロードされたメモリのアドレスを返す。
	 *
	 *	@param	symbol	シンボル名の文字列
	 *
	 *	@return	ンボルがロードされたメモリのアドレス
	 */
	virtual void* loadSymbol(const wchar_t* symbol); 
private:
	/*	@brief 動的ライブラリの「ハンドル」 */
	void*	m_pHandle;
};


#include "xlib/xlib_tail.h"
#endif // __X_DL_LORDER_FILE__
