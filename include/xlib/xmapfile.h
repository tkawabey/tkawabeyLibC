/**
 * @file xmapfile.h
 * @brief �������[�}�b�v�t�@�C���N���X�N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_MAPFILE_HEASER_FILE__
#define __X_MAPFILE_HEASER_FILE__

#include "xfile.h"


class XMapFile : public XFile
{
public:
	XMapFile(size_t nSize);
	virtual ~XMapFile();

	/**
	 * 	@brief	�t�@�C���̃r���[���A�Ăяo�����v���Z�X�̃A�h���X��Ԃ���A���}�b�v���܂��B
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool Unmap();
	/**
	 * 	@brief	�}�b�v���ꂽ�A�h���X���擾���܂��B
	 * @return �}�b�v���ꂽ�A�h���X
	 */
	void* getMemAddress();


public:
	/** @copydoc XIO::Close() */
	virtual bool Close();
	/**  XIO::WriteRaw(const void* , size_t , int ) */
	virtual size_t WriteRaw(const void* lpData, size_t nSize, int flg = 0);
	/**  XIO::ReadRaw(void* , size_t , int ) */
	virtual size_t ReadRaw(void* lpData, size_t nSize, int flg = 0);

	virtual bool CreateMap(unsigned long nSize, int nFd, bool bReadOnly = false);
	/** @copydoc XFile::OpenRawA(const char *,int , bool , int ) */
	virtual bool OpenRawA(const char *filename,    int flg, bool bShare, int pmode);
	/** @copydoc XFile::OpenRawA(const char *,int , bool , int ) */
	virtual bool OpenRawW(const wchar_t *filename, int flg, bool bShare, int pmode);


	/** @copydoc XFile::GetLength() */
	virtual size_t GetLength();
	/** @copydoc XFile::Seek(long, SeekPosition) */
	virtual bool Seek(long offset, SeekPosition origin);
	/** @copydoc XFile::Fulush() */
	virtual bool Fulush();


protected:
	/* �T�C�Y */
	size_t			m_nSize;
	/* �}�b�v���ꂽ�A�h���X */
	void*			m_lpAddress;
	/* �}�b�v�n���h���iWin32�̂ݎg�p�j */
	int				m_hMap;
};


#endif //__X_MAPFILE_HEASER_FILE__

