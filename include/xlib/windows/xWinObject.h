/**
 * @file xWinObject.h
 * @brief WINDOWS�n���h���Ǘ��N���X
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
 * WINDOWS�n���h���Ǘ��N���X
 */
class XWinObject
{
public:
	/**
	 * �R���X�g���N�^
	 * @param[in] hInitObject �n���h�������Ă��鎞�̒l
	 */
	XWinObject(HANDLE hInitObject = NULL);
	/**
	 * �f�X�g���N�^
	 */
	virtual ~XWinObject();
	/**
	 * �N���[�Y���܂��B
	 */
	virtual void Close();
	/**
	 * �n���h�����L���ł��邩�ǂ�������ׂ܂��B
	 * @retval true �L��
	 * @retval false ����
	 */
	virtual bool IsValidate();
protected:
	/* �n���h�� */
	HANDLE		m_hObject;
	/* �n���h���������Ȏ��̒l */
	HANDLE		m_hValidObject;
};



#endif // WIN32

#endif // __X_WIN_OBJECT_HEASER_FILE__
