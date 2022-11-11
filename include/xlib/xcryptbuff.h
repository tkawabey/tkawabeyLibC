/**
 * @file xcryptbuff.h
 * @brief �o�b�t�@�Í��N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_CRYPTBUFF_HEASER_FILE__
#define __X_CRYPTBUFF_HEASER_FILE__





#include "xlib/xstrbuff.h"
#include "xlib/xfile.h"
#include "xlib/xeception.h"

/**
 * �g���v��DES�R�[�_�N���X
 */
class XBuffD3DES : public XBuff
{
public:
	/** �R���X�g���N�^ */
	XBuffD3DES();
	/**
	 * �R���X�g���N�^
	 * @param[in] lpszKey �L�[
	 */
	XBuffD3DES(const char* lpszKey);
	/**
	 * �R���X�g���N�^
	 * @param[in] lpszKey �L�[
	 */
	XBuffD3DES(const wchar_t* lpszKey);
	/** �f�X�g���N�^ */
	virtual ~XBuffD3DES();


	/** @copydoc XBuff::Alloc(size_t) */
	virtual void *Alloc(size_t nSize);
	/** @copydoc XBuff::Free() */
	virtual void Free();
	/** @copydoc XBuff::GetBuffSize() */
	virtual size_t GetBuffSize();
	

	/**
	 * �L�[���Z�b�g���܂��B
	 * @param[in] lpszKey �L�[
	 */
	void SetKey(const char* lpszKey);
	/**
	 * �L�[���Z�b�g���܂��B
	 * @param[in] lpszKey �L�[
	 */
	void SetKey(const wchar_t* lpszKey);

	/**
	 * �Í���
	 */
	void EnCrypt();
	/**
	 * ������
	 */
	void DeCrypt();
protected:
	/**
	 * DESKey
	 * @param key �L�[
	 * @param edf �o�b�t�@
	 */
	void deskey(unsigned char *key, int edf);
	/**
	 * ���E
	 * @param raw1 ���E
	 */
	void cookey(unsigned long *raw1);
	/**
	 * �t����
	 * @param from �t����
	 */
	void usekey(unsigned long *from);
	/**
	 * �R�s�[
	 * @param into �R�s�[
	 */
	void cpkey(unsigned long *into);
	/**
	 * DES
	 * @param inblock ����
	 * @param outblock �o��
	 */
	void des(unsigned char *inblock, unsigned char  *outblock);
	/**
	 * DES
	 * @param block �u���b�N
	 * @param keys �L�[
	 */
	static void desfunc(unsigned long *block, unsigned long *keys);
	/**
	 * DES
	 * @param outof �o��
	 * @param into ����
	 */
	static void scrunch(unsigned char *outof, unsigned long *into);
	/**
	 * DES
	 * @param outof �o��
	 * @param into ����
	 */
	static void unscrun(unsigned long *outof, unsigned char *into);
protected:
	/** ���T�C�Y */
	size_t		m_nRealSize;
	/** �I�t�Z�b�g */
	unsigned    int  m_offSet;
	/** �L�[ */
	unsigned	char m_key[8];
	/** �n�b�V���f�[�^ */
	unsigned	long KnL[32];
	/** �n�b�V���f�[�^ */
	unsigned	long KnR[32];
	/** �n�b�V���f�[�^ */
	unsigned	long Kn3[32];
};

/**
 * MD5�G���[
 */
class XMD5Except : XExcept
{
public:
	/** �R���X�g���N�^ */
	XMD5Except(){m_nErrorCode = 0; }
	/** �f�X�g���N�^ */
	virtual ~XMD5Except() throw () {};
	/**
	 * �G���[�R�[�h���擾���܂��B
	 * @retval 0 Finalize���R�[������Ă��܂���
	 * @retval 1 Finalize�����ɃR�[������Ă��܂�
	 * @retval 2 �������m�ۃG���[
	 */
	int getErrorCode(){ return m_nErrorCode; }

	/**
	 * �G���[���X���[���܂��B
	 * @param nErrorCode �G���[�R�[�h
	 */
	static void Throw(int nErrorCode) {
		XMD5Except *p = new XMD5Except;
		p->m_nErrorCode = nErrorCode;
		throw p;
	}
protected:
	/** �G���[�R�[�h */
	int		m_nErrorCode;
};

/**
 * MD5���g�p���A�o�b�t�@���̃f�[�^����_�C�W�F�X�g�l���擾
 */
class XMD5 : public XStrBuff
{
private:
	/** �S�o�C�g�̃T�C�Y��` */
	typedef unsigned       int uint4;
	/** �Q�o�C�g�̃T�C�Y��` */
	typedef unsigned short int uint2;
	/** �P�o�C�g�̃T�C�Y��` */
	typedef unsigned      char uint1;
public:
	/** �R���X�g���N�^ */
	XMD5();
	/**
	 * �R���X�g���N�^
	 * @param p �w�肵��������ŁA�_�C�W�F�X�g���쐬
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	XMD5(const char* p);
	/** @copydoc XMD5::XMD5(const char*) */
	XMD5(const wchar_t* p);
	/**
	 * ������ŁA�_�C�W�F�X�g���쐬
	 * @param lpsz ������
	 * @return ���g
	 */
	const XMD5& operator=(const char* lpsz);
	/**
	 * ������ŁA�_�C�W�F�X�g���쐬
	 * @param lpsz ������
	 * @return ���g
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	const XMD5& operator=(const wchar_t* lpsz);
	/**
	 * ������𑫂�����ŁA�_�C�W�F�X�g���č쐬
	 * @param lpsz ������
	 * @return ���g
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	const XMD5& operator+=(const char* lpsz);
	/**
	 * ������𑫂�����ŁA�_�C�W�F�X�g���č쐬
	 * @param lpsz ������
	 * @return ���g
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	const XMD5& operator+=(const wchar_t* lpsz);


	/**
	 * �I�u�W�F�N�g���ɐݒ肳�ꂽ�f�[�^����AMD5�Ńn�b�V�����������_�C�W�F�X�g�����߂܂�
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */ 
	void Make();
	/**
	 * �I�[�v�����ꂽ�t�@�C���I�u�W�F�N�g����AMD5�Ńn�b�V�����������_�C�W�F�X�g�����߂܂�
	 * @param[in] pFile �I�[�v�������t�@�C���̃C���X�^���X�̃A�h���X�B
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	void Make(XFile *pFile);

	/**
	 * �w�肳�ꂽ���͏�����ɁA�_�C�W�F�X�g���X�N�����u�����܂��B
	 * @param input ���̓f�[�^�̃A�h���X
	 * @param input_length ���̓f�[�^�̃T�C�Y�iBYTE�j
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	void Update(uint1 *input, uint4 input_length);
	/**
	 * Update���\�b�h���R�[���������ʃ_�C�W�F�X�g���m�肷�鎞�ɃR�[�����܂��B
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	void Finalize();
	/**
	 * �_�C�W�F�X�g�̐��f�[�^���擾���܂��B
	 * @param pBuff ���f�[�^���󂯎��o�b�t�@
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	void RawDigest(XBuff *pBuff);
	/**
	 * �_�C�W�F�X�g��ASCII�`���ɕϊ������������擾���܂��B
	 * @return �_�C�W�F�X�g������
	 * @exception XMD5Except* �G���[�I�u�W�F�N�g�B
	 */
	XStrBuff HexDigest() const;
private:
	/**
	 * �����o�����������܂��B
	 */
	void Init();
	/**
	 * block�ɂ���āA�X�N�����u�����܂��B
	 * @param block ����
	 */
	void Transform(uint1 *block);
	

	/**
	 * �G���R�[�h
	 * @param dest (null)
	 * @param src (null)
	 * @param length (null)
	 */
	static void encode(uint1 *dest, uint4 *src, uint4 length);
	/**
	 * �f�R�[�h
	 * @param dest (null)
	 * @param src (null)
	 * @param length (null)
	 */
	static void decode(uint4 *dest, uint1 *src, uint4 length);
	/**
	 * �������[�R�s�[
	 * @param dest (null)
	 * @param src (null)
	 * @param length (null)
	 */
	static void memcpy(uint1 *dest, uint1 *src, uint4 length);
	/**
	 * �������[�Z�b�g�[
	 * @param start (null)
	 * @param val (null)
	 * @param length (null)
	 */
	static void memset(uint1 *start, uint1 val, uint4 length);

	/**
	 * @param x (null)
	 * @param n (null)
	 * @return
	 */
	static inline uint4  rotate_left(uint4 x, uint4 n);
	/**
	 * @param x (null)
	 * @param y (null)
	 * @param z (null)
	 * @return
	 */
	static inline uint4  F(uint4 x, uint4 y, uint4 z);
	/**
	 * @param x (null)
	 * @param y (null)
	 * @param z (null)
	 * @return
	 */
	static inline uint4  G(uint4 x, uint4 y, uint4 z);
	/**
	 * @param x (null)
	 * @param y (null)
	 * @param z (null)
	 * @return
	 */
	static inline uint4  H(uint4 x, uint4 y, uint4 z);
	/**
	 * @param x (null)
	 * @param y (null)
	 * @param z (null)
	 * @return
	 */
	static inline uint4  I(uint4 x, uint4 y, uint4 z);
	/**
	 * @param a (null)
	 * @param b (null)
	 * @param c (null)
	 * @param d (null)
	 * @param x (null)
	 * @param s (null)
	 * @param ac (null)
	 * @return
	 */
	static inline void   FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
	/**
	 * @param a (null)
	 * @param b (null)
	 * @param c (null)
	 * @param d (null)
	 * @param x (null)
	 * @param s (null)
	 * @param ac (null)
	 * @return
	 */
	static inline void   GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
	/**
	 * @param a (null)
	 * @param b (null)
	 * @param c (null)
	 * @param d (null)
	 * @param x (null)
	 * @param s (null)
	 * @param ac (null)
	 * @return
	 */
	static inline void   HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);
	/**
	 * @param a (null)
	 * @param b (null)
	 * @param c (null)
	 * @param d (null)
	 * @param x (null)
	 * @param s (null)
	 * @param ac (null)
	 * @return
	 */
	static inline void   II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, 
			    uint4 s, uint4 ac);

	uint4 m_state[4];
	uint4 m_count[2];
	uint1 m_buffer[64];
	uint1 m_digest[16];
	uint1 m_finalized;
};

#endif// __X_CRYPTBUFF_HEASER_FILE__

