/**
 * @file xWinImageView.h
 * @brief MS-Windowに画像を表示するクラス
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_WIN_IMAGE_VIEW_HEASER_FILE__
#define __X_WIN_IMAGE_VIEW_HEASER_FILE__


#include "xlib/xstrbuff.h"
#include "xlib/xeception.h"
#include "xlib/image/ximage.h"
#include "xlib/image/ximagedib.h"

/**
 * @brief MS-Windowに画像を表示するクラス
 */
class XWinImageView
{
public:
	/** コンストラクタ */
	XWinImageView();
	/** デストラクタ */
	virtual ~XWinImageView();


	enum {
		_IsAttach = 0x00001
	};

	DWORD Attach( XImage &image );
	void Detach();



	BOOL IsAttach();
	// スクリーンに表示します
	long ViewDC(
		HDC *pDc, 
		XImage &image, 
		int nDestX,
		int nDestY,
		int nSrcX, 
		int nSrcY, 
		BOOL bUseeToStrech = FALSE);
	// スクリーンに表示します
	long ViewDC2(
		HDC *pDc, 
		XImage &image, 
		int XDest,                    // コピー先長方形の左上隅の x 座標
		int YDest,                    // コピー先長方形の左上隅の y 座標
		int nDestWidth,               // コピー先長方形の幅
		int nDestHeight,              // コピー先長方形の高さ
		int XSrc,                     // コピー元長方形の左上隅の x 座標
		int YSrc,                     // コピー元長方形の左上隅の x 座標
		int nSrcWidth,                // コピー元長方形の幅
		int nSrcHeight,               // コピー元長方形の高さ
		UINT iUsage = DIB_RGB_COLORS,                  // データの種類
		DWORD dwRop = SRCCOPY);
	long ViewDC(
		HDC *pDc, 
		XImage &image, 
		PAINTSTRUCT &ps);
	// パレットを取得します
	HPALETTE GetPalette();
	// クリップボードからDIBを取得します。
	bool GetClipboardImage(HWND hWnd, XImage &image);
	// クリップボードにDIBを貼り付けます。
	bool PutClipboardImage(HWND hWnd, XImage &image);
	// 指定されウィンドウのイメージを取得します。
	void WndCopy(HWND hWnd, XImage &image);
	// スクロールの設定
	virtual void SetScrollInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// WndCopyで取得したWindowのタイトルを取得します。
	LPCTSTR GetTitle();

	HBITMAP CreateHBITMAP(HWND hWnd, XImage &image);
	bool SetFromHBITMAP(HBITMAP hBitmap, HWND hWnd, XImage &image);
	void SetTitle( LPCTSTR lpszTitle );
	BITMAPINFO *GetBITMAPINFO();

	
	void OnSize(HWND hWnd);
	void OnHScrollScreen(HWND hwnd, WPARAM wParam, int nMvScale = 2);
	void OnVScrollScreen(HWND hwnd, WPARAM wParam, int nMvScale = 2);
	void UpdateScrollbars(HWND hWnd);
	bool ScrollScreen(HWND hWnd, int dx, int dy);
protected:
	void Free();
protected:
	BITMAPINFO			*m_pBInfo;
	BITMAPFILEHEADER	m_Bf;
	HPALETTE			m_hPalette;
	DWORD				m_nFlg;
	

	int                 m_nScaleNum;
	int                 m_nScaleDen;


	int                 m_nYImage;			// イメージの高さ
	int                 m_nXImage;			// イメージの幅
	
	int                 m_nYRenge;			// 立てスクロールの幅
	int                 m_nYScrollPos;		// 立てスクロールの位置
	int					m_nYClient;         // 立てクライアントの高さ
	int                 m_nYWindow;
	int                 m_nYScrollMax;		// 立てスクロールの位置
	

	int                 m_nXRenge;			// 横スクロールの幅
	int                 m_nXScrollPos;		// 横スクロールの位置
	int					m_nXClient;         // 横クライアントの高さ
	int                 m_nXWindow;
	int                 m_nXScrollMax;		// 立てスクロールの位置


	SCROLLINFO          m_YSiInf;
	SCROLLINFO          m_XSiInf;

};

#endif /* __X_WIN_IMAGE_VIEW_HEASER_FILE__ */
