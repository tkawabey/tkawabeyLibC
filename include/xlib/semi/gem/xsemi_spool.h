/**
 * @file xsemi_spool.h
 * @brief �����̐���SEMI�K�i�@�X�v�[���Ǘ��N���X
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

#define XSEMI_SPOOL_ALL_FUNCTIONS		0xFFFF	///<	�S�Ă�Function���Ώ�

class XLIB_API XSEMI_Gem;
class XLIB_API XSEMI_GemConst;

/**	@brief	�X�v�[��SxFx�Ǘ��N���X	 */
class XLIB_API XSEMI_SpoolSF : public XSEMI_GemConstObjectAtom
{
public:
	enum ATOM_POS {
			ATOM_POS_STREAM = 0
		,	ATOM_POS_FUNCTION

		,	ATOM_POS_OBJECT_TYPE = 99
	};


	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	stream	�X�g���[��
	 *	@param	function	�t�@���N�V����
	 */
	XSEMI_SpoolSF(XD_UINT1 stream, XD_UINT2 function);
	/**	@brief	�f�X�g���N�^	 */
	virtual ~XSEMI_SpoolSF();

	/**	@brief	�X�g���[�����擾���܂��B
	 *
	 *	@return	�X�g���[��
	 */
	XD_UINT1 get_Stream() const;

	/**	@brief	�t�@���N�V����ID���擾���܂��B
	 *
	 *	@return	�t�@���N�V����ID
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

/**	@brief	�X�v�[��SxFx�ꗗ�Ǘ��N���X	 */
class XLIB_API XSEMI_SpoolSFs : public XSEMI_GemConstObject
{
public:
	/**	@brief	�R���X�g���N�^	 */
	XSEMI_SpoolSFs();
	/**	@brief	�f�X�g���N�^ */
	virtual ~XSEMI_SpoolSFs();
	/**	@brief	XSEMI_SpoolSF */
	void clear();

	/**	@brief	�ꗗ�����擾���܂�
	 *	@return	�ꗗ��
	 */
	XD_UINT4 count();
	/**	@brief	�w�肵���C���f�b�N�X�̃A�C�e�����擾���܂��B
	 *
	 *	@param	index	�C���f�b�N�X
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_SpoolSF* get(XD_UINT4 index);
	/**	@brief	�w�肵�����|�[�g�̃A�C�e�����擾���܂��B
	 *
	 *	@param	stream	�X�g���[��
	 *	@param	function	�t�@���N�V����
	 *
	 *	@return	�A�C�e��
	 */
	XSEMI_SpoolSF* get_byID(XD_UINT1 stream, XD_UINT1 function);
	/**	@brief	�ꗗ�̐擪�A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B������Ȃ��ꍇ��NULL�B
	 */
	XSEMI_SpoolSF* getFirstItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̎��̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�Ō�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_SpoolSF* getNextItem();
	/**	@brief	���݂̈ꗗ�̃J�����g�ʒu�̑O�̃A�C�e�����擾����B
	 *
	 *	@return	�A�C�e���B�擪�ɒB�����ꍇ�́ANULL��Ԃ��B
	 */
	XSEMI_SpoolSF* getPrevItem();
	/**	@brief	�A�C�e����ǉ����܂��B
	 *
	 *	@param	pAtom	�A�g���I�u�W�F�N�g
	 *
	 *	@return	�ǉ����ꂽ�A�C�e���B���s�����ꍇ��NULL�B
	 */
	XSEMI_SpoolSF* add(XSEMI_GemDataAtom* pAtom);
	/**	@brief	�A�C�e�����폜���܂��B
	 *
	 *	@param	pRpt	�A�C�e�����폜���܂��B
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool del(XSEMI_SpoolSF* pSF);
protected:
	std::vector<XSEMI_SpoolSF*>				m_datas;
	std::vector<XSEMI_SpoolSF*>::iterator	m_ite;
friend class XSEMI_GemConst;
friend class XSEMI_Gem;
};

/**	@brief	�X�v�[�����Ǘ��N���X	 */
class XLIB_API XSEMI_Spool : protected XRingBufferBL
{
public:
	/**	@brief	�R���X�g���N�^	 */
	XSEMI_Spool(XSEMI_SpoolSFs&	sfs);

	/**	@brief	�f�X�g���N�^ */
	virtual ~XSEMI_Spool();

	/**	@brief	GEM�C���X�^���X���Z�b�g���܂��B
	 *
	 *	@param	pGem	GEM�C���X�^���X
	 */
	void setGem(XSEMI_Gem* pGem);

	/**	@brief	���X�v�[�������擾���܂��B
	 *	@return	���X�v�[����
	 */
	XD_UINT4 getActualCnt() const;
	/**	@brief	�X�v�[�����悤�Ƃ��������擾���܂��B
	 *	@return	�X�v�[�����悤�Ƃ�����
	 */
	XD_UINT4 getTotalCnt() const;
	/**	@brief	�X�v�[����Ԃ��擾���܂��B
	 *
	 *	@retval	XSEMI_GEM_SPOOL_INACTIVE	�X�v�[���x�~
	 *	@retval	XSEMI_GEM_SPOOL_ACTIVE	�X�v�[������
	 *	@retval	XSEMI_GEM_SPOOL_FULL	�X�v�[���t��
	 */
	XD_UINT1 getSpoolState();
	/**	@brief	�X�v�[�����[�h��Ԃ��擾���܂��B
	 *
	 *	@retval	XSEMI_GEM_SPOOL_NOTFULL	�X�v�[����
	 *	@retval	XSEMI_GEM_SPOOL_FULL	�X�v�[���t��
	 */
	XD_UINT1 getSpoolLoadState();
	/**	@brief	�X�v�[���A�����[�h��Ԃ��擾���܂��B
	 *
	 *	@retval	XSEMI_GEM_SPOOL_NOOUTPUT	�X�v�[���o�͖���
	 *	@retval	XSEMI_GEM_SPOOL_PURGE	�X�v�[����|
	 *	@retval	XSEMI_GEM_SPOOL_TRANSMIT	�X�v�[���]��
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
	
	/**	@brief	�N���[�Y
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	bool close();
	
	/**	@brief	�L���[����Ă���A�C�e�������擾�B
	 *
	 *	@note getActualCnt�Ɠ����l�̂͂��ł���
	 *
	 *	@return	�L���[����Ă���A�C�e����
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

	/**	@brief	�X�v�[���f�[�^��j������B
	 *
	 *	@return	
	 */
	int purge();

	/**	@brief	�X�v�[���f�[�^��]������B
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
