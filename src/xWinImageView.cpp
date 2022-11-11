#include "xlib/windows/xWinImageView.h"

XWinImageView::XWinImageView()
{
	m_nFlg   = 0;
	m_pBInfo = NULL;
	m_hPalette = NULL;


	m_nYImage        = 10;        // イメージの高さ
	m_nXImage        = 10;        // イメージの幅
	
	m_nYRenge        = 0;         // 立てスクロールの幅
	m_nYScrollPos    = 0;		  // 立てスクロールの位置
	m_nYClient       = 0;         // 立てクライアントの高さ

	m_YSiInf.cbSize      = sizeof(SCROLLINFO);
	m_YSiInf.fMask      = SIF_POS | SIF_RANGE | SIF_PAGE;
	m_YSiInf.nMin       = 0;
	m_YSiInf.nMax       = 0;
	m_YSiInf.nPage      = 0;
	m_YSiInf.nPos       = 0;
	m_YSiInf.nTrackPos  = 0;


	m_XSiInf.cbSize      = sizeof(SCROLLINFO);
	m_XSiInf.fMask      = SIF_POS | SIF_RANGE | SIF_PAGE;
	m_XSiInf.nMin       = 0;
	m_XSiInf.nMax       = 0;
	m_XSiInf.nPage      = 0;
	m_XSiInf.nPos       = 0;
	m_XSiInf.nTrackPos  = 0;

	m_nScaleNum         = 1;
	m_nScaleDen         = 1;
}

XWinImageView::~XWinImageView()
{
	Free();
}

DWORD XWinImageView::Attach( XImage &image )
{
#ifdef _DEBUG
//	XTRACE(_T("#####  XImgMSDependence::Attach Enter  #####\r\n"));
#endif
	Free();
	m_Bf.bfType = MAKEWORD('B','M');
	m_Bf.bfReserved1 = 0;
	m_Bf.bfReserved2 = 0;
	m_Bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*image.GetColorCount();
	m_Bf.bfSize = m_Bf.bfOffBits + ( image.GetWidth() * image.GetBitCount() )  * image.GetHeight();

	int iiii = sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*image.GetColorCount();
	m_pBInfo = (BITMAPINFO*) new unsigned char[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*image.GetColorCount()];
	if( m_pBInfo == NULL)
	{
#ifdef _DEBUG
//		XTRACE(_T("Err Memalloc (%d)\r\n"), sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*image.GetColorCount() );
#endif
		return 1;
	}
	m_pBInfo->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	m_pBInfo->bmiHeader.biWidth         = image.GetWidth();
	m_pBInfo->bmiHeader.biHeight        = image.GetHeight();
	m_pBInfo->bmiHeader.biPlanes        = 1;
	m_pBInfo->bmiHeader.biBitCount      = (WORD)image.GetBitCount();
	m_pBInfo->bmiHeader.biCompression   = BI_RGB;
	m_pBInfo->bmiHeader.biSizeImage     = 0;
	m_pBInfo->bmiHeader.biClrUsed       = 0;
	m_pBInfo->bmiHeader.biClrImportant  = 0;
	m_pBInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBInfo->bmiHeader.biYPelsPerMeter = 0;
#ifdef _DEBUG
//	XTRACE(_T("  Image Infomation\r\n"));
//	XTRACE(_T("    Width:%d  Height:%d  BitCount:%d  Color:%d\r\n"), image.GetWidth(), image.GetHeight(), image.GetBitCount(), image.GetColorCount());
#endif

	if( image.GetColorCount() )
	{
		HANDLE			hPal;
		LPLOGPALETTE	lpPal;
		LPRGBQUAD		lpRGB;


		hPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + image.GetColorCount() * sizeof(PALETTEENTRY));
		lpPal = (LPLOGPALETTE)GlobalLock(hPal);

		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = (WORD)image.GetColorCount();

		lpRGB = (LPRGBQUAD)((LPSTR)m_pBInfo + m_pBInfo->bmiHeader.biSize);
		
		for( unsigned int i = 0; i < image.GetColorCount(); i++, lpRGB++ )
		{
			lpPal->palPalEntry[i].peRed    = image.GetColorMap(i).red;
			lpPal->palPalEntry[i].peGreen  = image.GetColorMap(i).green;
			lpPal->palPalEntry[i].peBlue   = image.GetColorMap(i).blue;
			lpPal->palPalEntry[i].peFlags  = 0;
			lpRGB->rgbRed       = lpPal->palPalEntry[i].peRed;
			lpRGB->rgbGreen     = lpPal->palPalEntry[i].peGreen;
			lpRGB->rgbBlue      = lpPal->palPalEntry[i].peBlue;
			lpRGB->rgbReserved  = lpPal->palPalEntry[i].peFlags;
		}
		m_hPalette = CreatePalette(lpPal);	
		GlobalUnlock(hPal);
		GlobalFree(hPal);
	}
	else
	{

		if( image.GetBitCount() == 24 )
		{
			XImage dest;
			dest.SetSize(256, 8, image.GetWidth(), image.GetHeight() );
			XImgTools::MakeDefPallet(dest);
			HANDLE			hPal;
			LPLOGPALETTE	lpPal;

			hPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + dest.GetColorCount() * sizeof(PALETTEENTRY));
			lpPal = (LPLOGPALETTE)GlobalLock(hPal);

			lpPal->palVersion = 0x300;
			lpPal->palNumEntries = (WORD)dest.GetColorCount();

		
			for( unsigned int i = 0; i < dest.GetColorCount(); i++)
			{
				lpPal->palPalEntry[i].peRed    = dest.GetColorMap(i).red;
				lpPal->palPalEntry[i].peGreen  = dest.GetColorMap(i).green;
				lpPal->palPalEntry[i].peBlue   = dest.GetColorMap(i).blue;
				lpPal->palPalEntry[i].peFlags  = 0;
			}
			m_hPalette = CreatePalette(lpPal);	
			GlobalUnlock(hPal);
			GlobalFree(hPal);
		}

	}
	
	
	m_nYImage = image.GetHeight();
    m_YSiInf.nMax = m_nYRenge;
    m_YSiInf.nPage = m_nYClient / m_nYImage;
    m_YSiInf.nPos = m_nYScrollPos;
	m_nYScrollPos = 0;


	m_nXImage = image.GetWidth();
    m_XSiInf.nMax = m_nXRenge;
    m_XSiInf.nPage = m_nYClient / m_nXImage;
    m_XSiInf.nPos = m_nXScrollPos;
	m_nXScrollPos = 0;


	m_nFlg |= _IsAttach;
	return ERROR_SUCCESS;
}
void XWinImageView::Detach()
{
	Free();
}

BOOL XWinImageView::IsAttach()
{
	return (m_nFlg & _IsAttach);
}

// スクリーンに表示します
long XWinImageView::ViewDC(
	HDC *pDc, 
	XImage &image, 
	int nDestX,
	int nDestY,
	int nSrcX, 
	int nSrcY, 
	BOOL bUseeToStrech /*= FALSE*/)
{
	HDC					hMemDC=NULL;
	BOOL b = m_nFlg & _IsAttach;
	if( !(m_nFlg & _IsAttach) )
		return 1;

	if( GetPalette() )
	{
		hMemDC=CreateCompatibleDC(*pDc);
		SelectPalette(*pDc, GetPalette(), FALSE);
		RealizePalette(*pDc);
		if(hMemDC)
		{
		   SelectPalette(hMemDC, m_hPalette, FALSE);
		   RealizePalette(hMemDC);
		}
	}

	if( bUseeToStrech == TRUE)
	{
		::StretchDIBits(*pDc, 
					  nDestX, 
					  nDestY, 
					  nSrcX, 
					  nSrcY,
					  0 , 
					  0, 
					  image.GetWidth(), 
					  image.GetHeight(), 
					  image.ImageData(), 
					  m_pBInfo, 
					  DIB_RGB_COLORS, 
					  SRCCOPY);
	}
	else
	{
		::SetDIBitsToDevice(*pDc,
						   0-m_nXScrollPos,
						   0-m_nYScrollPos,
						   image.GetWidth (),
						   image.GetHeight (),
						   nSrcX,
						   nSrcY,
						   0,
						   m_pBInfo->bmiHeader.biHeight,
						   image.ImageData(), 
						   m_pBInfo,
						   DIB_RGB_COLORS) ;
	}
	return ERROR_SUCCESS;
}
// スクリーンに表示します
long XWinImageView::ViewDC2(
	HDC *pDc, 
	XImage &image, 
	int XDest,
	int YDest,
	int nDestWidth,
	int nDestHeight,
	int XSrc,
	int YSrc,
	int nSrcWidth,
	int nSrcHeight,
	UINT iUsage /*= DIB_RGB_COLORS*/,
	DWORD dwRop /*= SRCCOPY*/)
{
	HDC					hMemDC=NULL;
	BOOL b = m_nFlg & _IsAttach;
	if( !(m_nFlg & _IsAttach) )
		return 1;

	if( GetPalette() )
	{
		hMemDC=CreateCompatibleDC(*pDc);
		SelectPalette(*pDc, GetPalette(), FALSE);
		RealizePalette(*pDc);
		if(hMemDC)
		{
		   SelectPalette(hMemDC, m_hPalette, FALSE);
		   RealizePalette(hMemDC);
		}
	}

		StretchDIBits(*pDc, 
					  XDest - m_nXScrollPos, 
					  YDest - m_nYScrollPos, 
					  nDestWidth, 
					  nDestHeight,
					  XSrc , 
					  YSrc, 
					  nSrcWidth, 
					  nSrcHeight, 
					  image.ImageData(), 
					  m_pBInfo, 
					  iUsage, 
					  dwRop);

	return ERROR_SUCCESS;
}
long XWinImageView::ViewDC(
	HDC *pDc, 
	XImage &image, 
	PAINTSTRUCT &ps)
{
	if(m_nScaleNum != m_nScaleDen)
	{

	}
	else
	{
		::SetDIBitsToDevice(*pDc,
			0,
			0,
			m_nXScrollMax,
			m_nYScrollMax,
			m_nXScrollPos,
			-m_nYScrollPos,
			0,
			m_pBInfo->bmiHeader.biHeight,
			image.ImageData(), 
			m_pBInfo,
			DIB_RGB_COLORS) ;
	}
	return ERROR_SUCCESS;
}

HPALETTE XWinImageView::GetPalette()
{
	return m_hPalette;
}

bool XWinImageView::GetClipboardImage(HWND hWnd, XImage &image)
{
	XUBYTE4              nColNum;
	XUBYTE4              nWideWidth;
	XUBYTE4				bitsize;
	LPBYTE              pBuff;
	BITMAPINFO			*pBInfo;	

#ifdef _DEBUG
//	XTRACE(_T("#####  XImgMSDependence::GetClipboardImage Enter  #####\r\n"));
#endif

	if( ::IsClipboardFormatAvailable(CF_DIB) == FALSE )
		return false;

	if( OpenClipboard(hWnd) == FALSE )
		return false;


	HANDLE hDib = GetClipboardData(CF_DIB);
	if( hDib == NULL )
		return false;

	pBInfo = (BITMAPINFO*)GlobalLock(hDib);

    switch( pBInfo->bmiHeader.biBitCount )
    {
        case 1:
            nColNum = 2;
            break;
        case 4:
            nColNum = 16;
            break;
        case 8:
            nColNum = 256;
            break;
        case 24:
            nColNum = 0;
            break;
    }
	
    // Widthの実Byteサイズを求めます。
	bitsize = pBInfo->bmiHeader.biBitCount * pBInfo->bmiHeader.biWidth;
	nWideWidth = XImage::RoundRow ((bitsize + 7)/8) ;

	image.SetSize(nColNum, pBInfo->bmiHeader.biBitCount, pBInfo->bmiHeader.biWidth, pBInfo->bmiHeader.biHeight);

	LPRGBQUAD lpRGB;
	if(nColNum)
	{
		lpRGB = (LPRGBQUAD)((LPSTR)pBInfo + pBInfo->bmiHeader.biSize);
		for( XUBYTE4 i = 0; i < nColNum; i++, lpRGB++ )
		{
			image.GetColorMap(i).red   = lpRGB->rgbRed;
			image.GetColorMap(i).blue  = lpRGB->rgbBlue;
			image.GetColorMap(i).green = lpRGB->rgbGreen;
		}
	}

	pBuff = (LPBYTE)((LPSTR)pBInfo + (sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*nColNum) );
	image.CopyImageData(pBuff, nWideWidth * pBInfo->bmiHeader.biHeight);

	GlobalUnlock(hDib);
	CloseClipboard();
	this->Attach(image);

	HBITMAP p = CreateHBITMAP(hWnd, image);
	if( p )
		DeleteObject( p );
	return true;
}

bool XWinImageView::PutClipboardImage(HWND hWnd, XImage &image)
{
	XUBYTE4            nWidth, nWidWidth, nHight;
	HANDLE            hDib;
	LPBYTE            pBuff;
	BITMAPINFOHEADER  *pBMPHeader;
	LPRGBQUAD		  lpRGB;

	nWidth = image.GetWidth();
	nHight = image.GetHeight();
	nWidWidth = XImage::RoundRow ((image.GetBitCount() * nWidth + 7)/8) ;
	hDib = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * image.GetColorCount() + nWidWidth * nHight);
	if( hDib == NULL )
		return false;
	pBuff = (LPBYTE)GlobalLock(hDib);
	if( pBuff == NULL )
	{
		::GlobalFree( hDib );
		return false;
	}
	pBMPHeader = (BITMAPINFOHEADER*)pBuff;
	pBMPHeader->biSize          = sizeof(BITMAPINFOHEADER);
	pBMPHeader->biWidth         = nWidth;
	pBMPHeader->biHeight        = nHight;
	pBMPHeader->biPlanes        = 1;
	pBMPHeader->biBitCount      = image.GetBitCount();
	pBMPHeader->biCompression   = BI_RGB;
	pBMPHeader->biClrUsed       = 0;
	pBMPHeader->biClrImportant  = 0;
	pBMPHeader->biXPelsPerMeter = 0;
	pBMPHeader->biYPelsPerMeter = 0;

	if( image.GetColorCount() )
	{
		lpRGB = (LPRGBQUAD)((LPSTR)pBuff + pBMPHeader->biSize );
		for(XUBYTE4 i = 0; i < image.GetColorCount(); i++, lpRGB++ )
		{
			lpRGB->rgbRed       = image.GetColorMap(i).red;
			lpRGB->rgbGreen     = image.GetColorMap(i).green;
			lpRGB->rgbBlue      = image.GetColorMap(i).blue;
			lpRGB->rgbReserved  = image.GetColorMap(i).red;
		}
	}
	int nImageDataOffset = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * image.GetColorCount();
	image.GetImageData( ((unsigned char *)pBuff + nImageDataOffset),  nWidWidth * nHight);
	GlobalUnlock(hDib);

	if( OpenClipboard(hWnd) == FALSE )
	{
		::GlobalFree( hDib );
		return false;
	}
	EmptyClipboard();
	SetClipboardData(CF_DIB, hDib);
	CloseClipboard();
	return true;
}

void XWinImageView::WndCopy(HWND hWnd, XImage &image)
{
	int nPlanNum;		// カラープレーンの数
	int nBits;			// ピクセルあたりのカラービットの数。
	HDC					hDC;
	HDC					hMemDC=NULL;
	HBITMAP				hBitmap;
	RECT				rc;
	int                 nWidth, nHight;
	int                 nWideWidth;
	int                 nColNum;
	XBuff               buffBmpData;
	XBuff               buffBmpInfo;
	LPBYTE              pBuff;
	BITMAPINFO			*pBInfo;	

	

	// ウインドウの座標を取得します。
	if(hWnd != NULL)
	{
		::GetWindowRect(hWnd, &rc);
		nWidth = rc.right - rc.left;
		nHight = rc.bottom-rc.top;
	}
	else
	{
		nWidth = ::GetSystemMetrics(SM_CXSCREEN);
		nHight = ::GetSystemMetrics(SM_CYSCREEN);
	}
/*
	if( hWnd == NULL )
	{
		m_strTitle = _T("Desk Top");
	}
	else
	{
		m_strTitle = _T("Desk Top");

		TCHAR szTitle[_MAX_PATH];
		szTitle[0] = 0;
		::GetWindowText(hWnd, szTitle, sizeof(szTitle));
		if( lstrlen(szTitle) )
			m_strTitle = szTitle;
		else
			m_strTitle = _T("Invalid Window");

	}

*/

	// 画面全体のイメージを取得します。
	if((hDC=::GetWindowDC(hWnd))!=NULL)
	{
		Free();
		// カラープレーンの数を取得します。
		nPlanNum = ::GetDeviceCaps(hDC,PLANES);
		// ピクセルあたりのカラービットの数を取得します。
		nBits = ::GetDeviceCaps(hDC,BITSPIXEL);
        switch(nBits)
        {
            case 1:
                nColNum = 2;
                break;
            case 4:
                nColNum = 16;
                break;
            case 8:
                nColNum = 256;
                break;
            case 24:
                nColNum = 0;
                break;
            case 32:
				nColNum = 0;
                break;
        }
		unsigned int bitsize = nBits * nWidth ;
		nWideWidth = XImage::RoundRow ((bitsize + 7)/8) ;


		image.SetSize(nColNum, nBits, nWidth, nHight);

		// ビットマップ情報用のメモリを確保
		if((pBInfo=(BITMAPINFO *)buffBmpInfo.Alloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*nColNum)) == NULL )
		{
			XImageException::Throw( XIMG_ERR_MALLOC );
		}

		// データ用のバッファの確保
		if( (pBuff = (LPBYTE)buffBmpData.Alloc( nWideWidth * nHight )) == NULL )
		{
			XImageException::Throw( XIMG_ERR_MALLOC );
		}
	
		// 指定されたデバイスコンテキストに関連付けられているデバイスと互換性のある、ビットマップを作成します。
		if((hMemDC=CreateCompatibleDC(hDC))==0)
		{
			XImageException::Throw( XIMG_ERR_API );
		}

		// 指定されたデバイスコンテキストに関連付けられているデバイスと互換性のある、ビットマップを作成します。
		if((hBitmap=CreateCompatibleBitmap(hDC,  nWidth,  nHight))==0)
		{
			XImageException::Throw( XIMG_ERR_API );
		}

		::SelectObject(hMemDC,hBitmap);
		::BitBlt(hMemDC,0,0,nWidth,nHight,hDC,0,0,SRCCOPY);
		::ReleaseDC(hWnd, hDC);





		pBInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBInfo->bmiHeader.biWidth = nWidth;
		pBInfo->bmiHeader.biHeight = nHight;
		pBInfo->bmiHeader.biPlanes = nPlanNum;
		pBInfo->bmiHeader.biBitCount = nBits;
		pBInfo->bmiHeader.biCompression = BI_RGB;
		pBInfo->bmiHeader.biClrUsed = 0;
		pBInfo->bmiHeader.biClrImportant = 0;
		pBInfo->bmiHeader.biXPelsPerMeter = 0;
		pBInfo->bmiHeader.biYPelsPerMeter = 0;

		// パレットの作成
		{
			LPLOGPALETTE	lpPal;
			LPRGBQUAD		lpRGB;
			HANDLE			hPal;
			WORD			i;
			HDC				hPlDC;
			DWORD			dwEntry;

			hPlDC = GetDC(hWnd);
			if(nColNum)
			{
				if (!(GetDeviceCaps(hPlDC, RASTERCAPS) == RC_PALETTE)) 
				{						
					hPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColNum * sizeof(PALETTEENTRY));
					lpPal = (LPLOGPALETTE)GlobalLock(hPal);
			
					lpPal->palVersion = 0x300;
					lpPal->palNumEntries = (WORD)nColNum;

					lpRGB = (LPRGBQUAD)((LPSTR)pBInfo + pBInfo->bmiHeader.biSize);

					::SetLastError(0);
					if( (dwEntry = GetSystemPaletteEntries(hPlDC, 0, 256,
					       (LPPALETTEENTRY)lpPal->palPalEntry)) != 0 )
					{
						for (i = 0; i < dwEntry; i++)
						{
							lpRGB->rgbRed              = lpPal->palPalEntry[i].peRed;
							lpRGB->rgbGreen            = lpPal->palPalEntry[i].peGreen;
							lpRGB->rgbBlue             = lpPal->palPalEntry[i].peBlue;
							lpRGB->rgbReserved         = lpPal->palPalEntry[i].peFlags;
							image.GetColorMap(i).red   = lpPal->palPalEntry[i].peRed;
							image.GetColorMap(i).blue  = lpPal->palPalEntry[i].peBlue;
							image.GetColorMap(i).green = lpPal->palPalEntry[i].peGreen;
						}
						m_hPalette = CreatePalette(lpPal);					


					   SelectPalette(hPlDC, m_hPalette, FALSE);
					   RealizePalette(hPlDC); 
					   SelectPalette(hMemDC, m_hPalette, FALSE);
					   RealizePalette(hMemDC); 
					}
					GlobalUnlock(hPal);
					GlobalFree(hPal);
				}
				ReleaseDC(hWnd, hPlDC); 
			}
		}


		// イメージデータの取得
		if(GetDIBits(hMemDC, hBitmap,0, nHight, pBuff,  pBInfo,  DIB_RGB_COLORS)==0)
		{
			DeleteObject(hBitmap);
			XImageException::Throw( XIMG_ERR_API );
		}
		DeleteDC(hMemDC);
		DeleteObject(hBitmap);
		hMemDC = NULL;
		hBitmap = NULL;
		image.CopyImageData(pBuff, nWideWidth * nHight);


		this->Attach(image);
	}
}

void XWinImageView::SetScrollInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int   dy;//増分
	static int   dx;
	switch( message ) 
	{
		case WM_CREATE:
			m_nYImage = 0;
			m_nXImage = 0;
			break;
		case WM_HSCROLL:
			switch(LOWORD(wParam))
			{
			case SB_LEFT:
				dx = -10;
				break;
			case SB_RIGHT:
				dx = 10;
				break;
			case SB_THUMBPOSITION:
				dx = HIWORD(wParam)-m_nXScrollPos;
				break;
			case SB_LINELEFT:
				dx = -10;
				break;
			case SB_LINERIGHT:
				dx = 10;
				break;
			case SB_PAGELEFT:
				dx = -10;
				break;
			case SB_PAGERIGHT:
				dx = 10;
				break;
			case SB_ENDSCROLL:
			case SB_THUMBTRACK:
			default:
				break;
			}
			dx = max(-m_nXScrollPos, min(dx, m_nXRenge - m_nXScrollPos));
            if (dx != 0)
			{
                m_nXScrollPos += dx;
				m_XSiInf.nPos = m_nXScrollPos;

				::SetScrollInfo(hWnd, SB_HORZ, &m_XSiInf, TRUE);
                ::ScrollWindow(hWnd, -dx, 0, NULL, NULL);
                ::UpdateWindow(hWnd);
            }			
			break;

		case WM_VSCROLL:
			switch(LOWORD(wParam))
			{
			case SB_LINEUP:
				dy = -10;
				break;
			case SB_LINEDOWN:
				dy = 10;
				break;
			case SB_THUMBPOSITION:
				dy = HIWORD(wParam)-m_nYScrollPos;
				break;
			case SB_PAGEDOWN:
				dy = 10;
				break;
			case SB_PAGEUP:
				dy = -10;
				break;
			default:
				dy = 0;
				break;
			}
			dy = max(-m_nYScrollPos, min(dy, m_nYRenge - m_nYScrollPos));
            if (dy != 0)
			{
                m_nYScrollPos += dy;
				m_YSiInf.nPos = m_nYScrollPos;

				::SetScrollInfo(hWnd, SB_VERT, &m_YSiInf, TRUE);
                ::ScrollWindow(hWnd, 0, -dy, NULL, NULL);
                ::UpdateWindow(hWnd);
            }			
			break;
		case WM_SIZE:
			// 立てスクロールの情報を設定
			m_nYClient = HIWORD(lParam);
            m_nYRenge = m_nYImage - m_nYClient;
			if(m_nYRenge < 0)
				m_nYRenge = 0;
            m_nYScrollPos = min(m_nYScrollPos, m_nYRenge);
			if(m_nYClient > m_nYImage)
				m_nYScrollPos = 0;

            m_YSiInf.nMax = m_nYRenge;
			if(m_nYRenge == 0)
				m_YSiInf.nPage = 0;
			else
			{
				if( m_nYClient )
					m_YSiInf.nPage = (m_nYImage / m_nYClient);
			}
            m_YSiInf.nPos = m_nYScrollPos;
			::SetScrollInfo(hWnd, SB_VERT, &m_YSiInf, TRUE);

			// 横スクロールの情報を設定
			m_nXClient = LOWORD(lParam);
			m_nXRenge = m_nXImage - m_nXClient;
			if(m_nXRenge < 0)
				m_nXRenge = 0;
			m_nXScrollPos = min(m_nXScrollPos, m_nXRenge);
			if(m_nXClient > m_nXImage)
				m_nXScrollPos = 0;

            m_XSiInf.nMax = m_nXRenge;
			if(m_nXRenge == 0)
				m_XSiInf.nPage = 0;
			else
			{
				if( m_nXClient )
				m_XSiInf.nPage = (m_nXImage / m_nXClient);
			}
            m_XSiInf.nPos = m_nXScrollPos;
			::SetScrollInfo(hWnd, SB_HORZ, &m_XSiInf, TRUE);
			break;
		default:
			break;
	}
}

LPCTSTR XWinImageView::GetTitle()
{
	return _T("");
}

void XWinImageView::SetTitle( LPCTSTR lpszTitle )
{

}


HBITMAP XWinImageView::CreateHBITMAP(HWND hWnd, XImage &image)
{
	XBuff        buff;
	HBITMAP      hBitmap;
	LPBYTE       pBuff = NULL;
	BITMAPINFO   *bb;
	HDC          hDC;
	XUBYTE4       nWidWidth = image.GetWidth();

	bb = (BITMAPINFO*)buff.Alloc( sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*image.GetColorCount());
	

	memset(bb, 0,  sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*image.GetColorCount()  );
	bb->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bb->bmiHeader.biWidth       = image.GetWidth();
	bb->bmiHeader.biHeight      = image.GetHeight();
	bb->bmiHeader.biPlanes      = 1;
	bb->bmiHeader.biBitCount    = image.GetBitCount();
	bb->bmiHeader.biCompression = BI_RGB;
	bb->bmiHeader.biClrUsed     = image.GetColorCount();

	nWidWidth = XImage::RoundRow ((image.GetBitCount() * image.GetWidth() + 7)/8) ;

	for(XUBYTE4 i = 0; i < image.GetColorCount(); i++ )
	{
		bb->bmiColors[i].rgbRed       = image.GetColorMap(i).red;
		bb->bmiColors[i].rgbGreen     = image.GetColorMap(i).green;
		bb->bmiColors[i].rgbBlue      = image.GetColorMap(i).blue;
		bb->bmiColors[i].rgbReserved  = 0;
	}

	hDC = GetDC(hWnd);
	hBitmap = ::CreateDIBSection(hDC, bb, DIB_RGB_COLORS, (LPVOID*)&pBuff, NULL, 0);
	::ReleaseDC(hWnd, hDC);
	image.GetImageData(pBuff,  nWidWidth * image.GetHeight());

	return hBitmap;
}

bool XWinImageView::SetFromHBITMAP(HBITMAP hBitmap, HWND hWnd, XImage &image)
{
	BITMAP     bm;
	XUBYTE4     nColNum, nWideWidth;


	// イメージの情報を取得
	if( ::GetObject(hBitmap, sizeof(BITMAP), &bm) == FALSE )
		return false;

	// 色数の決定
    switch(bm.bmBitsPixel)
    {
        case 1:
            nColNum = 2;
            break;
        case 4:
            nColNum = 16;
            break;
        case 8:
            nColNum = 256;
            break;
        case 24:
            nColNum = 0;
            break;
    }
	// イメージ幅のByteを求める
	nWideWidth = XImage::RoundRow ((bm.bmBitsPixel * bm.bmWidth + 7)/8) ;
	// メモリの確保
	image.SetSize(nColNum, bm.bmBitsPixel, bm.bmWidth, bm.bmHeight);

	
	if(bm.bmBits)
	{
		// イメージ情報にイメージRAWデータがある場合はそれを、コピー
		image.CopyImageData((const unsigned char *)bm.bmBits, nWideWidth * bm.bmHeight);
	}
	else
	{
		HDC           hDC;
		XBuff         buff;
		LPBYTE        pBuff;
		XBuff         buffBmpInfo;
		BITMAPINFO    *pBInfo;
		
		pBuff  = (LPBYTE)buff.Alloc(nWideWidth * bm.bmHeight);
		pBInfo = (BITMAPINFO*)buffBmpInfo.Alloc(  sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColNum );


		pBInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBInfo->bmiHeader.biWidth = bm.bmWidth;
		pBInfo->bmiHeader.biHeight = bm.bmHeight;
		pBInfo->bmiHeader.biPlanes = 1;
		pBInfo->bmiHeader.biBitCount = bm.bmBitsPixel;
		pBInfo->bmiHeader.biCompression = BI_RGB;
		pBInfo->bmiHeader.biClrUsed = nColNum;//nColNum;
		pBInfo->bmiHeader.biClrImportant = 0;
		pBInfo->bmiHeader.biXPelsPerMeter = 0;
		pBInfo->bmiHeader.biYPelsPerMeter = 0;


		hDC = GetDC(NULL);
		// 仮想デバイス表示されたイメージのRAWデータを取得
		::GetDIBits(hDC, hBitmap,0, bm.bmHeight, pBuff,  pBInfo,  DIB_RGB_COLORS);
		image.CopyImageData((const unsigned char *)pBuff, nWideWidth * bm.bmHeight);
		ReleaseDC( NULL, hDC);
	}
	// パレットを使用している場合
	if( ( bm.bmBitsPixel * bm.bmPlanes ) <= 8 )
	{
		HDC           hMemDC;
		HBITMAP       hOldBitmap;
		RGBQUAD       rgb[256];
		WORD          i;


		hMemDC = ::CreateCompatibleDC( NULL );
		hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );
		// HBITMAPからぱっレットを取得できる
		if( GetDIBColorTable( hMemDC, 0, 256, rgb ) )
		{
			for(i=0;i<256;i++)
			{
				image.GetColorMap(i).red   = rgb[i].rgbRed;
				image.GetColorMap(i).blue  = rgb[i].rgbBlue;
				image.GetColorMap(i).green = rgb[i].rgbGreen;
			}
		}
		else
		{
			// HBITMAPからぱっレットを取得できない場合は、ストックオブジェクトを使用
			PALETTEENTRY pal[256];
			DWORD dwEntry;
			HPALETTE  hPal;
			hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
			if( (dwEntry = GetPaletteEntries(hPal, 0, nColNum,  (LPPALETTEENTRY)&pal)) != 0 )
			{
				for( DWORD k = 0; k < dwEntry; k++)
				{
					image.GetColorMap(k).red   = pal[k].peRed;
					image.GetColorMap(k).blue  = pal[k].peBlue;
					image.GetColorMap(k).green = pal[k].peGreen;
				}
			}
		}
		SelectObject( hMemDC, hOldBitmap );
		DeleteDC( hMemDC );
	} 
	return true;
}

BITMAPINFO* XWinImageView::GetBITMAPINFO()
{
	return m_pBInfo;
}

void XWinImageView::OnSize(HWND hWnd)
{
	RECT rect;


	if( !IsAttach() )
	{
		::ShowScrollBar(hWnd, SB_HORZ, FALSE);
		::ShowScrollBar(hWnd, SB_VERT, FALSE);
		return ;
	}


	::GetWindowRect(hWnd, &rect);
	m_nXWindow = rect.right - rect.left;  // クライアントの画面の幅
	m_nYWindow = rect.bottom - rect.top;  // クライアントの画面の高さ


	::GetClientRect(hWnd, &rect);
	m_nXClient = min( rect.right - rect.left, 
							 m_nXWindow );		// クライアントの画面の幅とサーバーの画面の幅の小さい方を求めます
	m_nYClient = min( rect.bottom - rect.top,
							 m_nYImage );		// クライアントの画面の高さとサーバーの画面の高さの小さい方を求めます


	if( m_nYImage > m_nYClient ){	::ShowScrollBar(hWnd, SB_VERT, TRUE);	}
	else{							::ShowScrollBar(hWnd, SB_VERT, FALSE);  }
	if( m_nXImage > m_nXClient ){	::ShowScrollBar(hWnd, SB_HORZ, TRUE);	}
	else						{	::ShowScrollBar(hWnd, SB_HORZ, FALSE);	}

	m_nXScrollMax = m_nXImage * m_nScaleNum / m_nScaleDen;
	m_nYScrollMax = m_nYImage * m_nScaleNum / m_nScaleDen;


	int newhpos, newvpos;
	newhpos = max(0, min(m_nXScrollPos, 
						 m_nXScrollMax - max(m_nXClient, 0)));
	newvpos = max(0, min(m_nYScrollPos, 
						 m_nYScrollMax - max(m_nYClient, 0)));

	::ScrollWindowEx(hWnd, m_nXScrollPos-newhpos, m_nYScrollPos-newvpos,
		NULL, &rect, NULL, NULL,  SW_INVALIDATE);
	
	m_nXScrollPos = newhpos;
	m_nYScrollPos = newvpos;
    UpdateScrollbars(hWnd);
}
void XWinImageView::OnHScrollScreen(HWND hWnd, WPARAM wParam, int nMvScale /*= 2*/)
{
	int dx = 0;
	int pos = HIWORD(wParam);
	switch (LOWORD(wParam))
	{
	case SB_LINEUP:
		dx = (-1*nMvScale); break;
	case SB_LINEDOWN:
		dx = nMvScale; break;
	case SB_PAGEUP:
		dx = m_nXClient * -1/4; break;
	case SB_PAGEDOWN:
		dx = m_nXClient * 1/4; break;
	case SB_THUMBPOSITION:
		dx = pos - m_nXScrollPos;
	case SB_THUMBTRACK:
		dx = pos - m_nXScrollPos;
	}
	ScrollScreen(hWnd, dx,0);
}
void XWinImageView::OnVScrollScreen(HWND hWnd, WPARAM wParam, int nMvScale /*= 2*/)
{
	int dy = 0;
	int pos = HIWORD(wParam);
	switch (LOWORD(wParam))
	{
	case SB_LINEUP:
		dy = (-1*nMvScale); break;
	case SB_LINEDOWN:
		dy = nMvScale; break;
	case SB_PAGEUP:
		dy = m_nYClient * -1/4; break;
	case SB_PAGEDOWN:
		dy = m_nYClient * 1/4; break;
	case SB_THUMBPOSITION:
		dy = pos - m_nYScrollPos;
	case SB_THUMBTRACK:
		dy = pos - m_nYScrollPos;
	}
	ScrollScreen(hWnd, 0,dy);
}
void XWinImageView::UpdateScrollbars(HWND hWnd)
{
	SCROLLINFO scri;
	scri.cbSize = sizeof(scri);
	scri.fMask = SIF_ALL;
	scri.nMin = 0;
	scri.nMax = m_nXScrollMax; 
	scri.nPage= m_nXClient;
	scri.nPos = m_nXScrollPos; 
	::SetScrollInfo(hWnd, SB_HORZ, &scri, TRUE);
	
	scri.cbSize = sizeof(scri);
	scri.fMask = SIF_ALL;
	scri.nMin = 0;
	scri.nMax = m_nYScrollMax;     
	scri.nPage= m_nYClient;
	scri.nPos = m_nYScrollPos; 
	::SetScrollInfo(hWnd, SB_VERT, &scri, TRUE);
}
bool XWinImageView::ScrollScreen(HWND hWnd, int dx, int dy)
{
	if( dx )
	{
		dx = max(dx, -m_nXScrollPos);
		dx = min(dx, m_nXScrollMax-(m_nXClient)-m_nXScrollPos);
	}
	if(dy)
	{
		dy = max(dy, -m_nYScrollPos);
		dy = min(dy, m_nYScrollMax-(m_nYClient)-m_nYScrollPos);
	}
	if (dx || dy)
	{
		m_nXScrollPos += dx;
		m_nYScrollPos += dy;
		RECT clirect;
		::GetClientRect(hWnd, &clirect);
		::ScrollWindowEx(hWnd, -dx, -dy, NULL, &clirect, NULL, NULL,  SW_INVALIDATE);
		UpdateScrollbars(hWnd);
		::UpdateWindow(hWnd);
		return true;
	}
	return false;
}
void XWinImageView::Free()
{
	if(m_pBInfo)
		delete [] m_pBInfo;
	if(m_hPalette)
		DeleteObject(m_hPalette);    
	m_pBInfo = NULL;
	m_hPalette = NULL;
	m_nFlg   = 0;
}



