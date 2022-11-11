/**
 * @file xtime.h
 * @brief 時間管理クラス
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
 * 時間管理クラス
 */
class XLIB_API XTime
{
public:
	/** @brief	コンストラクタ */
	XTime();
	/**
	 * @brief	コンストラクタ
	 * @param[in] dateSrc 初期化元時間インスタンス
	 */
	XTime(const XTime& dateSrc);
	/**
	 * @brief	コンストラクタ
	 * @param[in] dtSrc 初期化元時間
	 */
	XTime(double dtSrc, unsigned long ulMSec = 0);
	/**
	 * @brief	コンストラクタ
	 * @param[in] timeSrc 初期化元時間
	 */
	XTime(time_t timeSrc, unsigned long ulMSec = 0);
	/**
	 * @brief	コンストラクタ
	 * @param[in] timeSrc 初期化元時間
	 */
	XTime(XTIME_T64 timeSrc);
	/**
	 * @brief	コンストラクタ
	 * @param[in] nYear 年
	 * @param[in] nMonth 月
	 * @param[in] nDay 日
	 * @param[in] nHour 時間
	 * @param[in] nMin 分
	 * @param[in] nSec 秒
	 */
	XTime(int nYear, int nMonth, int nDay,
		int nHour, int nMin, int nSec, unsigned long ulMSec = 0);
//	XTime(unsigned short wDosDate, unsigned short wDosTime);
#ifdef WIN32
	/**
	 * @brief	コンストラクタ
	 * @param[in] varSrc 初期化元時間
	 */
	XTime(const VARIANT& varSrc);
	/**
	 * @brief	コンストラクタ
	 * @param[in] systimeSrc 初期化元時間
	 */
	XTime(const SYSTEMTIME& systimeSrc);
	/**
	 * @brief	コンストラクタ
	 * @param[in] filetimeSrc 初期化元時間
	 */
	XTime(const FILETIME& filetimeSrc);
#endif //WIN32

public:
	/**	@brief	現在の時刻を取得します。
	 *	@return	現在の時刻
	 */
	static XTime getCurrentTime();
	/**
	 * @brief	DATE型時間の作成
	 * @param[in] wYear 年
	 * @param[in] wMonth 月
	 * @param[in] wDay 日
	 * @param[in] wHour 時間
	 * @param[in] wMinute 分
	 * @param[in] wSecond 分
	 * @param[out] dtDest DATE型時間を受け取るバッファ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	static bool OleDateFromTm(unsigned short wYear, unsigned short wMonth, unsigned short wDay, unsigned short wHour, unsigned short wMinute, unsigned short wSecond, double& dtDest);
	/**
	 * @brief	DATE型からtm型に変換します。
	 * @param dtSrc DATE型時間
	 * @param tmDest tm型時間
	 * @retval true 成功
	 * @retval false 失敗
	 */
	static bool TmFromOleDate(double dtSrc, struct tm& tmDest);
	/**
	 * @brief	tm型時間をANSI_APIで使用できる値に変換する。
	 * @param tmSrc tm型時間
	 */
	static void TmConvertToStandardFormat(struct tm& tmSrc);
	/**
	 * @brief	DATE型からdouble値に変換
	 * @param dt DATE型
	 * @return double値
	 */
	static double DoubleFromDate(double dt);
	/**
	 * @brief	double値からDATE型に変換
	 * @param dbl double値
	 * @return DATE型
	 */
	static double DateFromDouble(double dbl);


// Attributes
public:
	/** 時間セット後のステータス */
	enum DateTimeStatus
	{
		/** valid */
		valid = 0,
		/** invalid */
		invalid = 1,
		/** null */
		null = 2,
	};
	/** @brief	DATE型時間 */
	double			m_dt;
	/** @brief	ミリ秒を保持します */
	unsigned long	m_ulMSec;
	/** @brief	ステータス */
	DateTimeStatus m_status;
// 取得・設定
	/**
	 * @brief	ステータスをセットします
	 * @param status ステータス
	 */
	void SetStatus(DateTimeStatus status);
	/**
	 * @brief	ステータスを取得します
	 * @return ステータス
	 */
	DateTimeStatus GetStatus() const;
#ifdef WIN32
	/**
	 * @brief	システムタイムの変換値を取得します。
	 * @param sysTime システムタイムを受け取るバッファ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetAsSystemTime(SYSTEMTIME& sysTime) const;
#endif //WIN32
	/**
	 * @brief	年を取得
	 * @return 年
	 */
	int GetYear() const;
	/**
	 * @brief	月を取得
	 * @return 月
	 */
	int GetMonth() const;       // month of year (1 = Jan)
	/**
	 * @brief	日を取得
	 * @return 日
	 */
	int GetDay() const;         // day of month (0-31)
	/**
	 * @brief	時間を取得
	 * @return 時間
	 */
	int GetHour() const;        // hour in day (0-23)
	/**
	 * @brief	分を取得
	 * @return 分
	 */
	int GetMinute() const;      // minute in hour (0-59)
	/**
	 * @brief	秒を取得
	 * @return 秒
	 */
	int GetSecond() const;      // second in minute (0-59)
	/**
	 * @brief	ミリ秒を取得
	 * @return ミリ秒
	 */
	int GetMSecond() const;      // milli second in minute (0-999)
	/**
	 * @brief	曜日を取得
	 * @return 1 = 日曜日、2 = 月曜日、.... 7 = 土曜日
	 */
	int GetDayOfWeek() const;
	/**
	 * @brief	年の初めからの日にちの合計値を取得
	 * @return 年の初めからの日にちの合計値
	 */
	int GetDayOfYear() const;   // days since start of year, Jan 1 = 1


	/**	@brief	カレンダー値を取得（コンパイラーデフォルト）
	 *	@return	カレンダー値
	 */
	time_t mktime() const;


	/**	@brief	カレンダー値を取得（64BIT強制）
	 *	@return	カレンダー値
	 */
	XTIME_T64 mktime64() const;

	/**	@brief	64bit時間値を構造体に変換します。
	 *
	 *	@param	ptm	構造体のバッファ
	 *	@param	timp	格納されている時刻へのポインタ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool getGmtime(struct tm *ptm, const XTIME_T64 *timp);

	/**	@brief	64bit時刻値を現地時刻に変換します
	 *
	 *	@param	ptm	構造体のバッファ
	 *	@param	timp	格納されている時刻へのポインタ
	 *
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	static bool getLocaltime(struct tm *ptm, const XTIME_T64 *timp);

// Operations
public:
	/**
	 * @brief	時間をセット
	 * @param dateSrc 時間インスタンス
	 * @return 自身のインスタンス
	 */
	const XTime& operator=(const XTime& dateSrc);
	/**
	 * @brief	時間をセット
	 * @param dtSrc 時間
	 * @return 自身のインスタンス
	 */
	const XTime& operator=(double dtSrc);
	/**
	 * @brief	時間をセット
	 * @param timeSrc 時間
	 * @return 自身のインスタンス
	 */
	const XTime& operator=(const time_t& timeSrc);
	/**
	 * @brief	時間をセット
	 * @param timeSrc 時間
	 * @return 自身のインスタンス
	 */
	const XTime& operator=(const XTIME_T64& timeSrc);
#ifdef WIN32
	/**
	 * @brief	時間をセット
	 * @param varSrc 時間
	 * @return 自身のインスタンス
	 */
	const XTime& operator=(const VARIANT& varSrc);
	/**
	 * @brief	時間をセット
	 * @param systimeSrc 時間
	 * @return 自身のインスタンス
	 */
	const XTime& operator=(const SYSTEMTIME& systimeSrc);
	/**
	 * @brief	時間をセット
	 * @param filetimeSrc 時間
	 * @return 自身のインスタンス
	 */
	const XTime& operator=(const FILETIME& filetimeSrc);
#endif //WIN32
	/**
	 * @brief	時間が等しいか比較します。
	 * @param date 時間
	 * @retval true 等しい
	 * @retval false 等しくない
	 */
	bool operator==(const XTime& date) const;
	/**
	 * @brief	時間が等しくないか比較します。
	 * @param date 時間
	 * @retval true 等しくない
	 * @retval false 等しい
	 */
	bool operator!=(const XTime& date) const;
	/**
	 * @brief	dateより小さいか比較します。
	 * @param date 時間
	 * @retval true 小さい
	 * @retval false 大きい
	 */
	bool operator<(const XTime& date) const;
	/**
	 * @brief	dateより大きいか比較します。
	 * @param date 時間
	 * @retval true 大きい
	 * @retval false 小さい
	 */
	bool operator>(const XTime& date) const;
	/**
	 * @brief	dateより小さいかまたは等しいか比較します。
	 * @param date 時間
	 * @retval true 小さいかまたは等しいか
	 * @retval false 大きい
	 */
	bool operator<=(const XTime& date) const;
	/**
	 * @brief	dateより大きいかまたは等しいか比較します。
	 * @param date 時間
	 * @retval true 大きいまたは等しいか
	 * @retval false 小さい
	 */
	bool operator>=(const XTime& date) const;
	/**
	 * @brief	DATE型の値を取得
	 * @return DATE型の値
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
	 * @brief	時間をセットします
	 * @param nYear		年
	 * @param nMonth	月
	 * @param nDay		日
	 * @param nHour		時間
	 * @param nMin		分
	 * @param nSec		秒
	 * @param ulMSec	ミリ秒
	 * @return 時間セット後のステータス
	 */
	int SetDateTime(int nYear, int nMonth, int nDay,
		int nHour, int nMin, int nSec, unsigned long ulMSec = 0);
	/**
	 * @brief	年月日をセットします
	 * @param nYear 年
	 * @param nMonth 月
	 * @param nDay 日
	 * @return 時間セット後のステータス
	 */
	int SetDate(int nYear, int nMonth, int nDay);
	/**
	 * @brief	時刻をセットします
	 * @param nHour 時間
	 * @param nMin 分
	 * @param nSec 秒
	 * @return 時間セット後のステータス
	 */
	int SetTime(int nHour, int nMin, int nSec);

	/**
	 * @brief	時間を、フォーマットされた文字列を生成します
	 * @param pFormat printf の書式制御文字列と同じような書式制御文字列を指定します。詳細については、ランタイム関数「strftime」を参照してください。
	 * @return フォーマットされた時刻を持つXStrBuffオブジェクトを返します
	 * @note
	 *	- %Y 10進数で表す4桁の西暦
 	 *	- %y 10進数で表す2桁の西暦(00～99) 
	 *  - %m 10進数で表す月(01～12) 
	 *	- %b 月の省略名
	 *	- %B 月の正式名
	 *  - %d 10進数で表す月の日付(01～31) 
	 *	- %H 24時間表記の時間(00～23)
	 *	- %I 12時間表記の時間(01～12)
	 *	- %M 10進数で表す分(00～59)
	 *	- %S 10進数で表す秒(00～59) 
	 *	- %a 曜日の省略名
	 *	- %A 曜日の正式名
	 *	- %p 現在のロケールのAM/PM
	 *	- %w 10進数で表す曜日。日曜日を0 とする(0～6)。 
	 *	- %W 10進数で表す週の通し番号。月曜日を週の最初の日とする(00～53)。 
	 *	- %U 10進数で表す週の通し番号。日曜日を週の最初の日とする(00～53)。 
	 *	- %c ロケールに応じた日付と時間の表現
	 *	- %x 現在のロケールの日付表示
	 *	- %X 現在のロケールの時刻表示
	 *
	 *	2000/01/01 00:00:00 = "%Y/%m/%d %H:%M:%S"
	 */
	XStrBuff Format(const char *pFormat) const;
	/** @copydoc XTime::Format(const char*) const */
	XStrBuff Format(const wchar_t *pFormat) const;
#ifdef WIN32
	/**
	 * @brief	時間の文字列から、時間をセットします。
	 * @param lpszDate 時間文字列
	 * @param dwFlags フラグ
	 * @param lcid ローケルID
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags = 0,
		LCID lcid = LANG_USER_DEFAULT);
#endif //WIN32
protected:
	/** レンジを変更 */
	void CheckRange();
};

/**	@brief	相対時間幅	*/
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

/**	@brief	RTCチップを通さない時間を取得	*/
class XLIB_API XTimeNonRTC 
{
public:
	/**	@brief	コンストラクタ
	 *
	 *	@param	delta	デルタ時間(秒)
	 *	@param	autoSyncDist	同期間隔(秒)。自動同期しない場合は、0を指定します。
	 */
	XTimeNonRTC(time_t delta = 0, time_t autoSyncDist = 0);

	/**	@brief	デストラクタ	 */
	virtual ~XTimeNonRTC();

	/**	@brief	RTCから内部変数を同期します	 */
	void syncRTC();

	/**	@brief	時間を取得する際のデルタ時間を取得します
	 *	@param	sec	デルタ時間(秒)
	 */
	void setDelta(time_t sec);

	/**	@brief	自動的に、RTCチップと内部変数を同期する間隔
	 *	@param	sec	同期間隔(秒)。自動同期しない場合は、0を指定します。
	 */
	void setAutoSyncDist(time_t sec);

	/**	@brief	現在の時間を取得します
	 *
	 *	@param	pMsec	ミリ秒を取得する場合、取得する変数のアドレスを指定します。
	 *
	 *	@return	時間
	 */
	time_t	getTime(time_t* pMsec = NULL);

	/**	@brief	現在の時刻を取得します。
	 *
	 *	@return	現在の時刻
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
