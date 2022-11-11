/**
 * @file xmapfile.h
 * @brief メモリーマップファイルクラスクラス
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
	 * 	@brief	ファイルのビューを、呼び出し側プロセスのアドレス空間からアンマップします。
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool Unmap();
	/**
	 * 	@brief	マップされたアドレスを取得します。
	 * @return マップされたアドレス
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
	/* サイズ */
	size_t			m_nSize;
	/* マップされたアドレス */
	void*			m_lpAddress;
	/* マップハンドル（Win32のみ使用） */
	int				m_hMap;
};


#endif //__X_MAPFILE_HEASER_FILE__

