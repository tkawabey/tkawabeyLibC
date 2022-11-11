/**
 * @file xsemi_spool.h
 * @brief 半導体製造SEMI規格　スプール管理クラス
 *
 * @author T.Fujiha
 * @date 2006/05/11
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __XSEMI_SPOOL_H__
#define __XSEMI_SPOOL_H__

#include "xlib/xlib_head.h"
#include "xlib/xdata.h"
#include "xlib/xstrbuff.h"
#include "xlib/xmapfile.h"
#include "xlib/semi/gem/xsemi_gem_const_obj.h"
#include "xlib/semi/secs/xsecs.h"
#include <vector>


#define XSEMI_SPOOL_STS_OK				1
#define XSEMI_SPOOL_STS_NA_SF			0
#define XSEMI_SPOOL_STS_NG_INIT			-1
#define XSEMI_SPOOL_STS_NG_FULL			-2
#define XSEMI_SPOOL_STS_NG_ALLOC		-3
#define XSEMI_SPOOL_STS_NG_NON			-4
#define XSEMI_SPOOL_STS_NG_BROKEN		-5
#define XSEMI_SPOOL_STS_NG_TRANSMIT		-6

#define XSEMI_SPOOL_ALL_FUNCTIONS		0xFFFF	///<	全てのFunctionが対象

class XLIB_API XSEMI_Gem;
class XLIB_API XSEMI_GemConst;

/**	@brief	スプールSxFx管理クラス	 */
class XLIB_API XSEMI_SpoolSF : public XSEMI_GemConstObjectAtom
{
public:
	enum ATOM_POS {
			ATOM_POS_STREAM = 0
		,	ATOM_POS_FUNCTION

		,	ATOM_POS_OBJECT_TYPE = 99
	};


	/**	@brief	コンストラクタ
	 *
	 *	@param	stream	ストリーム
	 *	@param	function	ファンクション
	 */
	XSEMI_SpoolSF(XD_UINT1 stream, XD_UINT2 function);
	/**	@brief	デストラクタ	 */
	virtual ~XSEMI_SpoolSF();

	/**	@brief	ストリームを取得します。
	 *
	 *	@return	ストリーム
	 */
	XD_UINT1 get_Stream() const;

	/**	@brief	ファンクションIDを取得します。
	 *
	 *	@return	ファンクションID
	 */
	XD_UINT2 get_Function() const;



// overwrite	XSEMI_GemDataAtom
	virtual const TCHAR* getAtomName();
	virtual int getColmnCount();
protected:
	virtual void* getAtomValPtr(int index);
protected:
	XD_UINT1					m_stream;
	XD_UINT2					m_function;

	static XSEMI_CONST_SCHEMA	g_pSchema[];
friend class XSEMI_GemConst;
//friend class XSEMI_Rpts ;
};

/**	@brief	スプールSxFx一覧管理クラス	 */
class XLIB_API XSEMI_SpoolSFs : public XSEMI_GemConstObject
{
public:
	/**	@brief	コンストラクタ	 */
	XSEMI_SpoolSFs();
	/**	@brief	デストラクタ */
	virtual ~XSEMI_SpoolSFs();
	/**	@brief	XSEMI_SpoolSF */
	void clear();

	/**	@brief	一覧数を取得します
	 *	@return	一覧数
	 */
	XD_UINT4 count();
	/**	@brief	指定したインデックスのアイテムを取得します。
	 *
	 *	@param	index	インデックス
	 *
	 *	@return	アイテム
	 */
	XSEMI_SpoolSF* get(XD_UINT4 index);
	/**	@brief	指定したレポートのアイテムを取得します。
	 *
	 *	@param	stream	ストリーム
	 *	@param	function	ファンクション
	 *
	 *	@return	アイテム
	 */
	XSEMI_SpoolSF* get_byID(XD_UINT1 stream, XD_UINT1 function);
	/**	@brief	一覧の先頭アイテムを取得する。
	 *
	 *	@return	アイテム。見つからない場合はNULL。
	 */
	XSEMI_SpoolSF* getFirstItem();
	/**	@brief	現在の一覧のカレント位置の次のアイテムを取得する。
	 *
	 *	@return	アイテム。最後に達した場合は、NULLを返す。
	 */
	XSEMI_SpoolSF* getNextItem();
	/**	@brief	現在の一覧のカレント位置の前のアイテムを取得する。
	 *
	 *	@return	アイテム。先頭に達した場合は、NULLを返す。
	 */
	XSEMI_SpoolSF* getPrevItem();
	/**	@brief	アイテムを追加します。
	 *
	 *	@param	pAtom	アトムオブジェクト
	 *
	 *	@return	追加されたアイテム。失敗した場合はNULL。
	 */
	XSEMI_SpoolSF* add(XSEMI_GemDataAtom* pAtom);
	/**	@brief	アイテムを削除します。
	 *
	 *	@param	pRpt	アイテムを削除します。
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool del(XSEMI_SpoolSF* pSF);
protected:
	std::vector<XSEMI_SpoolSF*>				m_datas;
	std::vector<XSEMI_SpoolSF*>::iterator	m_ite;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};

/**	@brief	スプール覧管理クラス	 */
class XLIB_API XSEMI_Spool : protected XRingBufferBL
{
public:
	/**	@brief	コンストラクタ	 */
	XSEMI_Spool(XSEMI_SpoolSFs&	sfs);

	/**	@brief	デストラクタ */
	virtual ~XSEMI_Spool();

	/**	@brief	GEMインスタンスをセットします。
	 *
	 *	@param	pGem	GEMインスタンス
	 */
	void setGem(XSEMI_Gem* pGem);

	/**	@brief	実スプール数を取得します。
	 *	@return	実スプール数
	 */
	XD_UINT4 getActualCnt() const;
	/**	@brief	スプールしようとした数を取得します。
	 *	@return	スプールしようとした数
	 */
	XD_UINT4 getTotalCnt() const;
	/**	@brief	スプール状態を取得します。
	 *
	 *	@retval	XSEMI_GEM_SPOOL_INACTIVE	スプール休止
	 *	@retval	XSEMI_GEM_SPOOL_ACTIVE	スプール活動
	 *	@retval	XSEMI_GEM_SPOOL_FULL	スプールフル
	 */
	XD_UINT1 getSpoolState();
	/**	@brief	スプールロード状態を取得します。
	 *
	 *	@retval	XSEMI_GEM_SPOOL_NOTFULL	スプール空
	 *	@retval	XSEMI_GEM_SPOOL_FULL	スプールフル
	 */
	XD_UINT1 getSpoolLoadState();
	/**	@brief	スプールアンロード状態を取得します。
	 *
	 *	@retval	XSEMI_GEM_SPOOL_NOOUTPUT	スプール出力無し
	 *	@retval	XSEMI_GEM_SPOOL_PURGE	スプール一掃
	 *	@retval	XSEMI_GEM_SPOOL_TRANSMIT	スプール転送
	 */
	XD_UINT1 getSpoolUnLoadState();


	/**	@brief	The open function
	 *
	 *	@param	p_map_file	a parameter of type const TCHAR*
	 *	@param	ulFileSize	a parameter of type XD_UINT4
	 *	@param	ulMaxSpoolTransmit	a parameter of type XD_UINT4
	 *	@param	bOverWrite	a parameter of type bool
	 *
	 *	@return	bool
	 */
	bool open(const TCHAR* p_map_file, XD_UINT4 ulFileSize, XD_UINT4 ulMaxSpoolTransmit, bool bOverWrite);
	
	/**	@brief	クローズ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	bool close();
	
	/**	@brief	キューされているアイテム数を取得。
	 *
	 *	@note getActualCntと同じ値のはずである
	 *
	 *	@return	キューされているアイテム数
	 */
	XD_UINT4 count();

	/**	@brief	The push function
	 *
	 *	@param	ulSec	a parameter of type XD_UINT4
	 *	@param	ulMSec	a parameter of type XD_UINT4
	 *	@param	pHeader	a parameter of type XSECSHeader*
	 *	@param	pData	a parameter of type unsigned TCHAR *
	 *	@param	ulDataSize	a parameter of type unsigned long
	 *
	 *	@return	int
	 */
	int push(XD_UINT4 ulSec, XD_UINT4 ulMSec, XSECSHeader* pHeader, unsigned char *pData, unsigned long ulDataSize);

	/**	@brief	スプールデータを破棄する。
	 *
	 *	@return	
	 */
	int purge();

	/**	@brief	スプールデータを転送する。
	 *
	 *	@return	
	 */
	int transmit();

	/**	@brief	The pop function
	 *
	 *	@param	ulSec	a parameter of type XD_UINT4&
	 *	@param	ulMSec	a parameter of type XD_UINT4&
	 *	@param	pHeader	a parameter of type XSECSHeader&
	 *	@param	buff	a parameter of type XBuff &
	 *	@param	ulDataSize	a parameter of type unsigned long&
	 *
	 *	@return	int
	 */
	int pop(XD_UINT4& ulSec, XD_UINT4& ulMSec, XSECSHeader& pHeader, XBuff &buff, unsigned long& ulDataSize);
protected:
	XSEMI_SpoolSFs&	m_sfs;
	XMapFile*		m_mapFile;

	XD_UINT4		m_ulActualCnt;
	XD_UINT4		m_ulTotalCnt;
	bool			m_bSpoolFull;
	XD_UINT1		m_ucSpoolSts;
	XD_UINT1		m_ucLoadSubSts;
	XD_UINT1		m_ucUnLoadSubSts;
	XTime			m_startTime;
	XTime			m_fullTime;

	

	XStrBuff		m_strFileName;
	XD_UINT4		m_ulMaxSpoolSize;
	XD_UINT4		m_ulMaxSpoolTransmit;
	bool			m_bOverWrite;
	bool			m_bInitOK;
	XSEMI_Gem*		m_pGem;
};

#include "xlib/xlib_tail.h"
#endif /* __XSEMI_SPOOL_H__ */
