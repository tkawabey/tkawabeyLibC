/**
 * @file xWinObject.h
 * @brief WINDOWSハンドル管理クラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_WIN_OBJECT_HEASER_FILE__
#define __X_WIN_OBJECT_HEASER_FILE__


#ifdef WIN32

  #ifdef __AFX_H__

  #else
    #include <windows.h>
  #endif

/**
 * WINDOWSハンドル管理クラス
 */
class XWinObject
{
public:
	/**
	 * コンストラクタ
	 * @param[in] hInitObject ハンドルが閉じている時の値
	 */
	XWinObject(HANDLE hInitObject = NULL);
	/**
	 * デストラクタ
	 */
	virtual ~XWinObject();
	/**
	 * クローズします。
	 */
	virtual void Close();
	/**
	 * ハンドルが有効であるかどうかしらべます。
	 * @retval true 有効
	 * @retval false 無効
	 */
	virtual bool IsValidate();
protected:
	/* ハンドル */
	HANDLE		m_hObject;
	/* ハンドルが無効な時の値 */
	HANDLE		m_hValidObject;
};



#endif // WIN32

#endif // __X_WIN_OBJECT_HEASER_FILE__
