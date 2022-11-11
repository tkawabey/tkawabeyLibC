/**
 * @file xtime.h
 * @brief ���ԊǗ��N���X
 *
 * @author T.Fujiha
 * @date 2003-09-21
 * @version 1.0
 *
 * Copyright (C) 2003 T.Fujiha,
 */
#ifndef __X_TIME_HEADER_FILE__
#define __X_TIME_HEADER_FILE__

#include "xlib/xlib_head.h"
#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif
#endif //
#include <time.h>

#include "xlib/xstrbuff.h"

#define XTIME_STD_FORMAT_YYYYmmDDhhMMss_W	L"%Y/%m/%d %H:%M:%S"
#define XTIME_STD_FORMAT_YYYYmmDDhhMMss_A	"%Y/%m/%d %H:%M:%S"

#ifdef _UNICODE
#define XTIME_STD_FORMAT_YYYYmmDDhhMMss	XTIME_STD_FORMAT_YYYYmmDDhhMMss_W
#else
#define XTIME_STD_FORMAT_YYYYmmDDhhMMss	XTIME_STD_FORMAT_YYYYmmDDhhMMss_A
#endif

#define XTIME_OLE_DATETIME_ERROR (-1)

typedef XD_UINT8	XTIME_T64;

class XLIB_API XTime;
class XLIB_API XTimeSpan;
class XLIB_API XTimeNonRTC;

/**
 * ���ԊǗ��N���X
 */
class XLIB_API XTime
{
public:
	/** @brief	�R���X�g���N�^ */
	XTime();
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] dateSrc �����������ԃC���X�^���X
	 */
	XTime(const XTime& dateSrc);
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] dtSrc ������������
	 */
	XTime(double dtSrc, unsigned long ulMSec = 0);
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] timeSrc ������������
	 */
	XTime(time_t timeSrc, unsigned long ulMSec = 0);
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] timeSrc ������������
	 */
	XTime(XTIME_T64 timeSrc);
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] nYear �N
	 * @param[in] nMonth ��
	 * @param[in] nDay ��
	 * @param[in] nHour ����
	 * @param[in] nMin ��
	 * @param[in] nSec �b
	 */
	XTime(int nYear, int nMonth, int nDay,
		int nHour, int nMin, int nSec, unsigned long ulMSec = 0);
//	XTime(unsigned short wDosDate, unsigned short wDosTime);
#ifdef WIN32
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] varSrc ������������
	 */
	XTime(const VARIANT& varSrc);
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] systimeSrc ������������
	 */
	XTime(const SYSTEMTIME& systimeSrc);
	/**
	 * @brief	�R���X�g���N�^
	 * @param[in] filetimeSrc ������������
	 */
	XTime(const FILETIME& filetimeSrc);
#endif //WIN32

public:
	/**	@brief	���݂̎������擾���܂��B
	 *	@return	���݂̎���
	 */
	static XTime getCurrentTime();
	/**
	 * @brief	DATE�^���Ԃ̍쐬
	 * @param[in] wYear �N
	 * @param[in] wMonth ��
	 * @param[in] wDay ��
	 * @param[in] wHour ����
	 * @param[in] wMinute ��
	 * @param[in] wSecond ��
	 * @param[out] dtDest DATE�^���Ԃ��󂯎��o�b�t�@
	 * @retval true ����
	 * @retval false ���s
	 */
	static bool OleDateFromTm(unsigned short wYear, unsigned short wMonth, unsigned short wDay, unsigned short wHour, unsigned short wMinute, unsigned short wSecond, double& dtDest);
	/**
	 * @brief	DATE�^����tm�^�ɕϊ����܂��B
	 * @param dtSrc DATE�^����
	 * @param tmDest tm�^����
	 * @retval true ����
	 * @retval false ���s
	 */
	static bool TmFromOleDate(double dtSrc, struct tm& tmDest);
	/**
	 * @brief	tm�^���Ԃ�ANSI_API�Ŏg�p�ł���l�ɕϊ�����B
	 * @param tmSrc tm�^����
	 */
	static void TmConvertToStandardFormat(struct tm& tmSrc);
	/**
	 * @brief	DATE�^����double�l�ɕϊ�
	 * @param dt DATE�^
	 * @return double�l
	 */
	static double DoubleFromDate(double dt);
	/**
	 * @brief	double�l����DATE�^�ɕϊ�
	 * @param dbl double�l
	 * @return DATE�^
	 */
	static double DateFromDouble(double dbl);


// Attributes
public:
	/** ���ԃZ�b�g��̃X�e�[�^�X */
	enum DateTimeStatus
	{
		/** valid */
		valid = 0,
		/** invalid */
		invalid = 1,
		/** null */
		null = 2,
	};
	/** @brief	DATE�^���� */
	double			m_dt;
	/** @brief	�~���b��ێ����܂� */
	unsigned long	m_ulMSec;
	/** @brief	�X�e�[�^�X */
	DateTimeStatus m_status;
// �擾�E�ݒ�
	/**
	 * @brief	�X�e�[�^�X���Z�b�g���܂�
	 * @param status �X�e�[�^�X
	 */
	void SetStatus(DateTimeStatus status);
	/**
	 * @brief	�X�e�[�^�X���擾���܂�
	 * @return �X�e�[�^�X
	 */
	DateTimeStatus GetStatus() const;
#ifdef WIN32
	/**
	 * @brief	�V�X�e���^�C���̕ϊ��l���擾���܂��B
	 * @param sysTime �V�X�e���^�C�����󂯎��o�b�t�@
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetAsSystemTime(SYSTEMTIME& sysTime) const;
#endif //WIN32
	/**
	 * @brief	�N���擾
	 * @return �N
	 */
	int GetYear() const;
	/**
	 * @brief	�����擾
	 * @return ��
	 */
	int GetMonth() const;       // month of year (1 = Jan)
	/**
	 * @brief	�����擾
	 * @return ��
	 */
	int GetDay() const;         // day of month (0-31)
	/**
	 * @brief	���Ԃ��擾
	 * @return ����
	 */
	int GetHour() const;        // hour in day (0-23)
	/**
	 * @brief	�����擾
	 * @return ��
	 */
	int GetMinute() const;      // minute in hour (0-59)
	/**
	 * @brief	�b���擾
	 * @return �b
	 */
	int GetSecond() const;      // second in minute (0-59)
	/**
	 * @brief	�~���b���擾
	 * @return �~���b
	 */
	int GetMSecond() const;      // milli second in minute (0-999)
	/**
	 * @brief	�j�����擾
	 * @return 1 = ���j���A2 = ���j���A.... 7 = �y�j��
	 */
	int GetDayOfWeek() const;
	/**
	 * @brief	�N�̏��߂���̓��ɂ��̍��v�l���擾
	 * @return �N�̏��߂���̓��ɂ��̍��v�l
	 */
	int GetDayOfYear() const;   // days since start of year, Jan 1 = 1


	/**	@brief	�J�����_�[�l���擾�i�R���p�C���[�f�t�H���g�j
	 *	@return	�J�����_�[�l
	 */
	time_t mktime() const;


	/**	@brief	�J�����_�[�l���擾�i64BIT�����j
	 *	@return	�J�����_�[�l
	 */
	XTIME_T64 mktime64() const;

	/**	@brief	64bit���Ԓl���\���̂ɕϊ����܂��B
	 *
	 *	@param	ptm	�\���̂̃o�b�t�@
	 *	@param	timp	�i�[����Ă��鎞���ւ̃|�C���^
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool getGmtime(struct tm *ptm, const XTIME_T64 *timp);

	/**	@brief	64bit�����l�����n�����ɕϊ����܂�
	 *
	 *	@param	ptm	�\���̂̃o�b�t�@
	 *	@param	timp	�i�[����Ă��鎞���ւ̃|�C���^
	 *
	 *	@retval	true	����
	 *	@retval	false	���s
	 */
	static bool getLocaltime(struct tm *ptm, const XTIME_T64 *timp);

// Operations
public:
	/**
	 * @brief	���Ԃ��Z�b�g
	 * @param dateSrc ���ԃC���X�^���X
	 * @return ���g�̃C���X�^���X
	 */
	const XTime& operator=(const XTime& dateSrc);
	/**
	 * @brief	���Ԃ��Z�b�g
	 * @param dtSrc ����
	 * @return ���g�̃C���X�^���X
	 */
	const XTime& operator=(double dtSrc);
	/**
	 * @brief	���Ԃ��Z�b�g
	 * @param timeSrc ����
	 * @return ���g�̃C���X�^���X
	 */
	const XTime& operator=(const time_t& timeSrc);
	/**
	 * @brief	���Ԃ��Z�b�g
	 * @param timeSrc ����
	 * @return ���g�̃C���X�^���X
	 */
	const XTime& operator=(const XTIME_T64& timeSrc);
#ifdef WIN32
	/**
	 * @brief	���Ԃ��Z�b�g
	 * @param varSrc ����
	 * @return ���g�̃C���X�^���X
	 */
	const XTime& operator=(const VARIANT& varSrc);
	/**
	 * @brief	���Ԃ��Z�b�g
	 * @param systimeSrc ����
	 * @return ���g�̃C���X�^���X
	 */
	const XTime& operator=(const SYSTEMTIME& systimeSrc);
	/**
	 * @brief	���Ԃ��Z�b�g
	 * @param filetimeSrc ����
	 * @return ���g�̃C���X�^���X
	 */
	const XTime& operator=(const FILETIME& filetimeSrc);
#endif //WIN32
	/**
	 * @brief	���Ԃ�����������r���܂��B
	 * @param date ����
	 * @retval true ������
	 * @retval false �������Ȃ�
	 */
	bool operator==(const XTime& date) const;
	/**
	 * @brief	���Ԃ��������Ȃ�����r���܂��B
	 * @param date ����
	 * @retval true �������Ȃ�
	 * @retval false ������
	 */
	bool operator!=(const XTime& date) const;
	/**
	 * @brief	date��菬��������r���܂��B
	 * @param date ����
	 * @retval true ������
	 * @retval false �傫��
	 */
	bool operator<(const XTime& date) const;
	/**
	 * @brief	date���傫������r���܂��B
	 * @param date ����
	 * @retval true �傫��
	 * @retval false ������
	 */
	bool operator>(const XTime& date) const;
	/**
	 * @brief	date��菬�������܂��͓���������r���܂��B
	 * @param date ����
	 * @retval true ���������܂��͓�������
	 * @retval false �傫��
	 */
	bool operator<=(const XTime& date) const;
	/**
	 * @brief	date���傫�����܂��͓���������r���܂��B
	 * @param date ����
	 * @retval true �傫���܂��͓�������
	 * @retval false ������
	 */
	bool operator>=(const XTime& date) const;
	/**
	 * @brief	DATE�^�̒l���擾
	 * @return DATE�^�̒l
	 */
	operator double() const;




	// DateTime math
	XTime operator+(const XTimeSpan& dateSpan) const;
	XTime operator-(const XTimeSpan& dateSpan) const;
	const XTime& operator+=(const XTimeSpan dateSpan);
	const XTime& operator-=(const XTimeSpan dateSpan);

	// DateTimeSpan math
	XTimeSpan operator-(const XTime& date) const;


	/**
	 * @brief	���Ԃ��Z�b�g���܂�
	 * @param nYear		�N
	 * @param nMonth	��
	 * @param nDay		��
	 * @param nHour		����
	 * @param nMin		��
	 * @param nSec		�b
	 * @param ulMSec	�~���b
	 * @return ���ԃZ�b�g��̃X�e�[�^�X
	 */
	int SetDateTime(int nYear, int nMonth, int nDay,
		int nHour, int nMin, int nSec, unsigned long ulMSec = 0);
	/**
	 * @brief	�N�������Z�b�g���܂�
	 * @param nYear �N
	 * @param nMonth ��
	 * @param nDay ��
	 * @return ���ԃZ�b�g��̃X�e�[�^�X
	 */
	int SetDate(int nYear, int nMonth, int nDay);
	/**
	 * @brief	�������Z�b�g���܂�
	 * @param nHour ����
	 * @param nMin ��
	 * @param nSec �b
	 * @return ���ԃZ�b�g��̃X�e�[�^�X
	 */
	int SetTime(int nHour, int nMin, int nSec);

	/**
	 * @brief	���Ԃ��A�t�H�[�}�b�g���ꂽ������𐶐����܂�
	 * @param pFormat printf �̏������䕶����Ɠ����悤�ȏ������䕶������w�肵�܂��B�ڍׂɂ��ẮA�����^�C���֐��ustrftime�v���Q�Ƃ��Ă��������B
	 * @return �t�H�[�}�b�g���ꂽ����������XStrBuff�I�u�W�F�N�g��Ԃ��܂�
	 * @note
	 *	- %Y 10�i���ŕ\��4���̐���
 	 *	- %y 10�i���ŕ\��2���̐���(00�`99) 
	 *  - %m 10�i���ŕ\����(01�`12) 
	 *	- %b ���̏ȗ���
	 *	- %B ���̐�����
	 *  - %d 10�i���ŕ\�����̓��t(01�`31) 
	 *	- %H 24���ԕ\�L�̎���(00�`23)
	 *	- %I 12���ԕ\�L�̎���(01�`12)
	 *	- %M 10�i���ŕ\����(00�`59)
	 *	- %S 10�i���ŕ\���b(00�`59) 
	 *	- %a �j���̏ȗ���
	 *	- %A �j���̐�����
	 *	- %p ���݂̃��P�[����AM/PM
	 *	- %w 10�i���ŕ\���j���B���j����0 �Ƃ���(0�`6)�B 
	 *	- %W 10�i���ŕ\���T�̒ʂ��ԍ��B���j�����T�̍ŏ��̓��Ƃ���(00�`53)�B 
	 *	- %U 10�i���ŕ\���T�̒ʂ��ԍ��B���j�����T�̍ŏ��̓��Ƃ���(00�`53)�B 
	 *	- %c ���P�[���ɉ��������t�Ǝ��Ԃ̕\��
	 *	- %x ���݂̃��P�[���̓��t�\��
	 *	- %X ���݂̃��P�[���̎����\��
	 *
	 *	2000/01/01 00:00:00 = "%Y/%m/%d %H:%M:%S"
	 */
	XStrBuff Format(const char *pFormat) const;
	/** @copydoc XTime::Format(const char*) const */
	XStrBuff Format(const wchar_t *pFormat) const;
#ifdef WIN32
	/**
	 * @brief	���Ԃ̕����񂩂�A���Ԃ��Z�b�g���܂��B
	 * @param lpszDate ���ԕ�����
	 * @param dwFlags �t���O
	 * @param lcid ���[�P��ID
	 * @retval true ����
	 * @retval false ���s
	 */
	bool ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags = 0,
		LCID lcid = LANG_USER_DEFAULT);
#endif //WIN32
protected:
	/** �����W��ύX */
	void CheckRange();
};

/**	@brief	���Ύ��ԕ�	*/
class XLIB_API XTimeSpan
{
public:
	XTimeSpan();

	XTimeSpan(double dblSpanSrc);
	XTimeSpan(const XTimeSpan& dateSpanSrc);
	XTimeSpan(long lDays, int nHours, int nMins, int nSecs);

public:
	enum XTimeSpanStatus
	{
		valid = 0,
		invalid = 1,    // Invalid span (out of range, etc.)
		null = 2,       // Literally has no value
	};

	double m_span;
	XTimeSpanStatus m_status;


	void SetStatus(XTimeSpanStatus status);
	XTimeSpanStatus GetStatus() const;

	double GetTotalDays() const;    // span in days (about -3.65e6 to 3.65e6)
	double GetTotalHours() const;   // span in hours (about -8.77e7 to 8.77e6)
	double GetTotalMinutes() const; // span in minutes (about -5.26e9 to 5.26e9)
	double GetTotalSeconds() const; // span in seconds (about -3.16e11 to 3.16e11)

	long GetDays() const;       // component days in span
	long GetHours() const;      // component hours in span (-23 to 23)
	long GetMinutes() const;    // component minutes in span (-59 to 59)
	long GetSeconds() const;    // component seconds in span (-59 to 59)

// Operations
public:
	const XTimeSpan& operator=(double dblSpanSrc);
	const XTimeSpan& operator=(const XTimeSpan& dateSpanSrc);

	bool operator==(const XTimeSpan& dateSpan) const;
	bool operator!=(const XTimeSpan& dateSpan) const;
	bool operator<(const XTimeSpan& dateSpan) const;
	bool operator>(const XTimeSpan& dateSpan) const;
	bool operator<=(const XTimeSpan& dateSpan) const;
	bool operator>=(const XTimeSpan& dateSpan) const;

	// DateTimeSpan math
	XTimeSpan operator+(const XTimeSpan& dateSpan) const;
	XTimeSpan operator-(const XTimeSpan& dateSpan) const;
	const XTimeSpan& operator+=(const XTimeSpan dateSpan);
	const XTimeSpan& operator-=(const XTimeSpan dateSpan);
	XTimeSpan operator-() const;

	operator double() const;

	void SetDateTimeSpan(long lDays, int nHours, int nMins, int nSecs);

	// formatting
//	CString Format(LPCTSTR pFormat) const;
//	CString Format(UINT nID) const;

// Implementation
public:
	void checkRange();
	friend XTime;
};

/**	@brief	RTC�`�b�v��ʂ��Ȃ����Ԃ��擾	*/
class XLIB_API XTimeNonRTC 
{
public:
	/**	@brief	�R���X�g���N�^
	 *
	 *	@param	delta	�f���^����(�b)
	 *	@param	autoSyncDist	�����Ԋu(�b)�B�����������Ȃ��ꍇ�́A0���w�肵�܂��B
	 */
	XTimeNonRTC(time_t delta = 0, time_t autoSyncDist = 0);

	/**	@brief	�f�X�g���N�^	 */
	virtual ~XTimeNonRTC();

	/**	@brief	RTC��������ϐ��𓯊����܂�	 */
	void syncRTC();

	/**	@brief	���Ԃ��擾����ۂ̃f���^���Ԃ��擾���܂�
	 *	@param	sec	�f���^����(�b)
	 */
	void setDelta(time_t sec);

	/**	@brief	�����I�ɁARTC�`�b�v�Ɠ����ϐ��𓯊�����Ԋu
	 *	@param	sec	�����Ԋu(�b)�B�����������Ȃ��ꍇ�́A0���w�肵�܂��B
	 */
	void setAutoSyncDist(time_t sec);

	/**	@brief	���݂̎��Ԃ��擾���܂�
	 *
	 *	@param	pMsec	�~���b���擾����ꍇ�A�擾����ϐ��̃A�h���X���w�肵�܂��B
	 *
	 *	@return	����
	 */
	time_t	getTime(time_t* pMsec = NULL);

	/**	@brief	���݂̎������擾���܂��B
	 *
	 *	@return	���݂̎���
	 */
	XTime getCurrentTime();
protected:
#ifdef WIN32
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_initCounter;
#else
	clock_t			m_initCounter;
#endif
	time_t			m_sec;
	time_t			m_delta;
	time_t			m_autoSyncDist;
};


#include "xlib/xlib_tail.h"
#endif // __X_TIME_HEADER_FILE__
