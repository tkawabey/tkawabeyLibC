/**
 * @file xWinImageView.h
 * @brief MS-Window�ɉ摜��\������N���X
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
 * @brief MS-Window�ɉ摜��\������N���X
 */
class XWinImageView
{
public:
	/** �R���X�g���N�^ */
	XWinImageView();
	/** �f�X�g���N�^ */
	virtual ~XWinImageView();


	enum {
		_IsAttach = 0x00001
	};

	DWORD Attach( XImage &image );
	void Detach();



	BOOL IsAttach();
	// �X�N���[���ɕ\�����܂�
	long ViewDC(
		HDC *pDc, 
		XImage &image, 
		int nDestX,
		int nDestY,
		int nSrcX, 
		int nSrcY, 
		BOOL bUseeToStrech = FALSE);
	// �X�N���[���ɕ\�����܂�
	long ViewDC2(
		HDC *pDc, 
		XImage &image, 
		int XDest,                    // �R�s�[�撷���`�̍������ x ���W
		int YDest,                    // �R�s�[�撷���`�̍������ y ���W
		int nDestWidth,               // �R�s�[�撷���`�̕�
		int nDestHeight,              // �R�s�[�撷���`�̍���
		int XSrc,                     // �R�s�[�������`�̍������ x ���W
		int YSrc,                     // �R�s�[�������`�̍������ x ���W
		int nSrcWidth,                // �R�s�[�������`�̕�
		int nSrcHeight,               // �R�s�[�������`�̍���
		UINT iUsage = DIB_RGB_COLORS,                  // �f�[�^�̎��
		DWORD dwRop = SRCCOPY);
	long ViewDC(
		HDC *pDc, 
		XImage &image, 
		PAINTSTRUCT &ps);
	// �p���b�g���擾���܂�
	HPALETTE GetPalette();
	// �N���b�v�{�[�h����DIB���擾���܂��B
	bool GetClipboardImage(HWND hWnd, XImage &image);
	// �N���b�v�{�[�h��DIB��\��t���܂��B
	bool PutClipboardImage(HWND hWnd, XImage &image);
	// �w�肳��E�B���h�E�̃C���[�W���擾���܂��B
	void WndCopy(HWND hWnd, XImage &image);
	// �X�N���[���̐ݒ�
	virtual void SetScrollInfo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// WndCopy�Ŏ擾����Window�̃^�C�g�����擾���܂��B
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


	int                 m_nYImage;			// �C���[�W�̍���
	int                 m_nXImage;			// �C���[�W�̕�
	
	int                 m_nYRenge;			// ���ăX�N���[���̕�
	int                 m_nYScrollPos;		// ���ăX�N���[���̈ʒu
	int					m_nYClient;         // ���ăN���C�A���g�̍���
	int                 m_nYWindow;
	int                 m_nYScrollMax;		// ���ăX�N���[���̈ʒu
	

	int                 m_nXRenge;			// ���X�N���[���̕�
	int                 m_nXScrollPos;		// ���X�N���[���̈ʒu
	int					m_nXClient;         // ���N���C�A���g�̍���
	int                 m_nXWindow;
	int                 m_nXScrollMax;		// ���ăX�N���[���̈ʒu


	SCROLLINFO          m_YSiInf;
	SCROLLINFO          m_XSiInf;

};

#endif /* __X_WIN_IMAGE_VIEW_HEASER_FILE__ */
