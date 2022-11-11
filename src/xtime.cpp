
#ifdef WIN32
  #ifdef __AFX_H__
  #else
    #include <windows.h>
  #endif
  #include <comdef.h>
#else
  #include <sys/time.h>	
#endif //


#include <math.h>
#include "xlib/xeception.h"
#include "xlib/xtime.h"







/*#define XTIME_MAX_TIME_BUFFER_SIZE    128*/
#define XTIME_MIN_DATE                (-657434L)
#define XTIME_MAX_DATE                2958465L
#define XTIME_MAX_DAYS_IN_SPAN				3615897L

// Half a second, expressed in days
#define XTIME_HALF_SECOND  (1.0/172800.0)
//#define AFX_OLE_DATETIME_ERROR (-1)
//#define AFX_OLE_DATETIME_HALFSECOND (1.0 / (2.0 * (60.0 * 60.0 * 24.0)))

// One-based array of days in year at month start
static int XTIME_MonthDays[13] =
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};


//----------------------------------------------------------------------
//
//	XTime
//
//----------------------------------------------------------------------
XTime::XTime()
{
XEXCEPT_ENTRY
	m_dt = 0;
	m_ulMSec = 0;
	SetStatus(valid);
XEXCEPT_EXIT
}
XTime::XTime(const XTime& dateSrc)
{
XEXCEPT_ENTRY
	m_dt = dateSrc.m_dt;
	m_ulMSec = dateSrc.m_ulMSec;
	m_status = dateSrc.m_status;
XEXCEPT_EXIT
}
XTime::XTime(double dtSrc, unsigned long ulMSec /*= 0*/)
{
XEXCEPT_ENTRY
	m_dt = dtSrc;
	m_ulMSec = ulMSec;
	SetStatus(valid);
XEXCEPT_EXIT
}
XTime::XTime(time_t timeSrc, unsigned long ulMSec /*= 0*/)
{
XEXCEPT_ENTRY
	*this = timeSrc;
	m_ulMSec = ulMSec;
XEXCEPT_EXIT
}
XTime::XTime(XTIME_T64 timeSrc)
{
XEXCEPT_ENTRY
	*this = timeSrc;
XEXCEPT_EXIT
}
#ifdef WIN32
XTime::XTime(const VARIANT& varSrc)
{
XEXCEPT_ENTRY
	*this = varSrc;
XEXCEPT_EXIT
}
XTime::XTime(const SYSTEMTIME& systimeSrc)
{
XEXCEPT_ENTRY
	*this = systimeSrc;
XEXCEPT_EXIT
}
XTime::XTime(const FILETIME& filetimeSrc)
{
XEXCEPT_ENTRY
	*this = filetimeSrc;
XEXCEPT_EXIT
}
#endif //WIN32
XTime::XTime(int nYear, int nMonth, int nDay,
	int nHour, int nMin, int nSec, unsigned long ulMSec /*= 0*/)
{
XEXCEPT_ENTRY
	SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec, ulMSec);
XEXCEPT_EXIT
}

//XTime::XTime(unsigned short wDosDate, unsigned short wDosTime);

XTime XTime::getCurrentTime()
{
XEXCEPT_ENTRY
#ifdef WIN32
	SYSTEMTIME	systime;
	::GetLocalTime(&systime);
	XTime timeObj(systime);
#else
	timeval tv;
	gettimeofday(&tv, 0);
	XTime timeObj((time_t)tv.tv_sec);

#endif
	return timeObj;
XEXCEPT_EXIT
}

void XTime::SetStatus(DateTimeStatus status)
{
XEXCEPT_ENTRY
	m_status = status;
XEXCEPT_EXIT
}
XTime::DateTimeStatus XTime::GetStatus() const
{
XEXCEPT_ENTRY
	return m_status;
XEXCEPT_EXIT
}
#ifdef WIN32
bool XTime::GetAsSystemTime(SYSTEMTIME& sysTime) const
{
XEXCEPT_ENTRY
	bool bRetVal = false;
	if (GetStatus() == valid)
	{
		struct tm tmTemp;
		if(TmFromOleDate(m_dt, tmTemp))
		{
			sysTime.wYear = (unsigned short) tmTemp.tm_year;
			sysTime.wMonth = (unsigned short) tmTemp.tm_mon;
			sysTime.wDayOfWeek = (unsigned short) (tmTemp.tm_wday - 1);
			sysTime.wDay = (unsigned short) tmTemp.tm_mday;
			sysTime.wHour = (unsigned short) tmTemp.tm_hour;
			sysTime.wMinute = (unsigned short) tmTemp.tm_min;
			sysTime.wSecond = (unsigned short) tmTemp.tm_sec;
			sysTime.wMilliseconds = 0;

			bRetVal = true;
		}
	}

	return bRetVal;
XEXCEPT_EXIT
}
#endif //WIN32
int XTime::GetYear() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;
	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_year;
	else
		return XTIME_OLE_DATETIME_ERROR;
XEXCEPT_EXIT
}
int XTime::GetMonth() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;
	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_mon;
	else
		return XTIME_OLE_DATETIME_ERROR;
XEXCEPT_EXIT
}
int XTime::GetDay() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;

	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_mday;
	else
		return XTIME_OLE_DATETIME_ERROR;
XEXCEPT_EXIT
}
int XTime::GetHour() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;

	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_hour;
	else
		return XTIME_OLE_DATETIME_ERROR;
XEXCEPT_EXIT
}
int XTime::GetMinute() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;

	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_min;
	else
		return XTIME_OLE_DATETIME_ERROR;
XEXCEPT_EXIT
}
int XTime::GetSecond() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;

	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_sec;
	else
		return XTIME_OLE_DATETIME_ERROR;
XEXCEPT_EXIT
}
int XTime::GetMSecond() const
{
XEXCEPT_ENTRY
	return m_ulMSec;
XEXCEPT_EXIT
}
int XTime::GetDayOfWeek() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;

	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_wday;
	else
		return XTIME_OLE_DATETIME_ERROR;
XEXCEPT_EXIT
}
int XTime::GetDayOfYear() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;
	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp))
		return tmTemp.tm_yday;
	else
		return XTIME_OLE_DATETIME_ERROR;
XEXCEPT_EXIT
}
time_t XTime::mktime() const
{
XEXCEPT_ENTRY
	struct tm tmTemp;

	if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp)) {
		TmConvertToStandardFormat(tmTemp);
		return ::mktime(&tmTemp);
	} else {
		return 0;
	}
XEXCEPT_EXIT
}
XTIME_T64 XTime::mktime64() const
{
XEXCEPT_ENTRY

	if( GetStatus() != valid ) {
		return 0;
	}
	if( sizeof(time_t) == 8 ) {
		// time_tが８バイトなら、Nativeで取得
		struct tm tmTemp;

		if (GetStatus() == valid && TmFromOleDate(m_dt, tmTemp)) {
			TmConvertToStandardFormat(tmTemp);
			return (XTIME_T64)::mktime(&tmTemp);
		} else {
			return 0;
		}	
	}


	XD_INT8 ss_days[] = {
			-1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
	};
	XTIME_T64	retVal = 0;
	XD_INT8		tmptm1, tmptm2, tmptm3;

#define __MY_BASE_YEAR         70L                  /* 1970 is the base year */
#define __MY_LEAP_YEAR_ADJUST  17L                  /* Leap years 1900 - 1970 */

	tmptm1 = this->GetYear();
	tmptm1 -= 1900;
	tmptm2 = ss_days[this->GetMonth()-1];
	if ( !(tmptm1 & 3) && ( (this->GetMonth()-1) > 1) )
        tmptm2++;


    tmptm3 = (tmptm1 - __MY_BASE_YEAR) * 365L + ((tmptm1 - 1L) >> 2)
      - __MY_LEAP_YEAR_ADJUST;

    tmptm3 += tmptm2;

	tmptm1 = tmptm3 + (tmptm2 = (this->GetDay()));

	tmptm2 = tmptm1 * 24L;

	tmptm1 = tmptm2 + (tmptm3 = this->GetHour());

	tmptm2 = tmptm1 * 60L;

	tmptm1 = tmptm2 + (tmptm3 = this->GetMinute());

	tmptm2 = tmptm1 * 60L;

	tmptm1 = tmptm2 + (tmptm3 = this->GetSecond());

	_tzset();
	
#if _MSC_VER >= 1310
	long thTimeZone = 0;
	::_get_timezone( &thTimeZone );
	tmptm1 += (XD_INT8)thTimeZone;
#else
    tmptm1 += (XD_INT8)_timezone;
#endif
	tmptm1 *= 1000;

	tmptm1 += this->GetMSecond();

	return tmptm1;
XEXCEPT_EXIT
}

const XTime& XTime::operator=(const XTime& dateSrc)
{
XEXCEPT_ENTRY
	m_dt = dateSrc.m_dt;
	m_status = dateSrc.m_status;
	return *this; 
XEXCEPT_EXIT
}
const XTime& XTime::operator=(double dtSrc)
{
XEXCEPT_ENTRY
	m_dt = dtSrc;
	SetStatus(valid);
	return *this;
XEXCEPT_EXIT
}

const XTime& XTime::operator=(const time_t& timeSrc)
{
XEXCEPT_ENTRY
	tm *ptm = NULL;
#if _MSC_VER >= 1310
	tm thTm = {0};
	
	if( ::localtime_s(&thTm, &timeSrc) != 0 ) {
		SetStatus(invalid);
		_XASSERT(false);
		return *this;
	}
	ptm = &thTm;
#else
	ptm = localtime(&timeSrc);
#endif

	if (ptm != NULL)
	{
		m_status = OleDateFromTm((unsigned short)(ptm->tm_year + 1900),
			(unsigned short)(ptm->tm_mon + 1), (unsigned short)ptm->tm_mday,
			(unsigned short)ptm->tm_hour, (unsigned short)ptm->tm_min,
			(unsigned short)ptm->tm_sec, m_dt) ? valid : invalid;
	}
	else
	{
		// Local time must have failed (timsSrc before 1/1/70 12am)
		SetStatus(invalid);
		_XASSERT(false);
	}

	return *this;
XEXCEPT_EXIT
}
const XTime& XTime::operator=(const XTIME_T64& timeSrc)
{

XEXCEPT_ENTRY
	tm			tM;
	XTIME_T64	ltime = timeSrc;
	if( getLocaltime(&tM, &timeSrc) == true ) {

		m_status = OleDateFromTm((unsigned short)(tM.tm_year + 1900),
			(unsigned short)(tM.tm_mon + 1), (unsigned short)tM.tm_mday,
			(unsigned short)tM.tm_hour, (unsigned short)tM.tm_min,
			(unsigned short)tM.tm_sec, m_dt) ? valid : invalid;

		m_ulMSec = (unsigned long)((XTIME_T64)timeSrc%1000);
	}
	else
	{
		// Local time must have failed (timsSrc before 1/1/70 12am)
		SetStatus(invalid);
		_XASSERT(false);
	}

	return *this;
XEXCEPT_EXIT
}
#ifdef WIN32
const XTime& XTime::operator=(const VARIANT& varSrc)
{
XEXCEPT_ENTRY
	if (varSrc.vt != VT_DATE)
	{
		_variant_t varTemp(varSrc);
		varTemp.ChangeType(VT_DATE);
		m_dt = (DATE)varTemp;
		SetStatus(valid);
	}
	else
	{
		m_dt = varSrc.date;
		SetStatus(valid);
	}
	return *this;
XEXCEPT_EXIT
}
const XTime& XTime::operator=(const SYSTEMTIME& systimeSrc)
{
XEXCEPT_ENTRY
	m_status = OleDateFromTm(systimeSrc.wYear, systimeSrc.wMonth,
		systimeSrc.wDay, systimeSrc.wHour, systimeSrc.wMinute,
		systimeSrc.wSecond, m_dt) ? valid : invalid;

	m_ulMSec = systimeSrc.wMilliseconds;
	return *this;
XEXCEPT_EXIT
}

const XTime& XTime::operator=(const FILETIME& filetimeSrc)
{
XEXCEPT_ENTRY
	FILETIME filetimeLocal;
	if (!::FileTimeToLocalFileTime( &filetimeSrc, &filetimeLocal))
	{
		m_status = invalid;
	}
	else
	{
		SYSTEMTIME systime;
		m_status = ::FileTimeToSystemTime(&filetimeLocal, &systime) ?	valid : invalid;

		// At this point systime should always be valid, but...
		if (GetStatus() == valid)
		{
			m_status = OleDateFromTm(systime.wYear, systime.wMonth,
				systime.wDay, systime.wHour, systime.wMinute,
				systime.wSecond, m_dt) ? valid : invalid;

			m_ulMSec = systime.wMilliseconds;
		}
	}
	return *this;
XEXCEPT_EXIT
}
#endif //WIN32
bool XTime::operator==(const XTime& date) const
{
XEXCEPT_ENTRY
	return (m_status == date.m_status && m_dt == date.m_dt);
XEXCEPT_EXIT
}
bool XTime::operator!=(const XTime& date) const
{
XEXCEPT_ENTRY
	return (m_status != date.m_status || m_dt != date.m_dt);
XEXCEPT_EXIT
}
bool XTime::operator<(const XTime& date) const
{
XEXCEPT_ENTRY
	_XASSERT(GetStatus() == valid);
	_XASSERT(date.GetStatus() == valid);

	// Handle negative dates
	return DoubleFromDate(m_dt) < DoubleFromDate(date.m_dt);
XEXCEPT_EXIT
}
bool XTime::operator>(const XTime& date) const
{
XEXCEPT_ENTRY
	_XASSERT(GetStatus() == valid);
	_XASSERT(date.GetStatus() == valid);

	// Handle negative dates
	return DoubleFromDate(m_dt) > DoubleFromDate(date.m_dt);
XEXCEPT_EXIT
}
bool XTime::operator<=(const XTime& date) const
{
XEXCEPT_ENTRY
	_XASSERT(GetStatus() == valid);
	_XASSERT(date.GetStatus() == valid);

	// Handle negative dates
	return DoubleFromDate(m_dt) <= DoubleFromDate(date.m_dt);
XEXCEPT_EXIT
}
bool XTime::operator>=(const XTime& date) const
{
XEXCEPT_ENTRY
	_XASSERT(GetStatus() == valid);
	_XASSERT(date.GetStatus() == valid);

	// Handle negative dates
	return DoubleFromDate(m_dt) >= DoubleFromDate(date.m_dt);
XEXCEPT_EXIT
}
XTime::operator double() const
{
XEXCEPT_ENTRY
	return m_dt;
XEXCEPT_EXIT
}
int XTime::SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, unsigned long ulMSec /*= 0*/)
{
XEXCEPT_ENTRY
	m_ulMSec = ulMSec;
	return m_status = OleDateFromTm((unsigned short)nYear, (unsigned short)nMonth,	(unsigned short)nDay, (unsigned short)nHour, (unsigned short)nMin, (unsigned short)nSec, m_dt) ? valid : invalid;
XEXCEPT_EXIT
}
int XTime::SetDate(int nYear, int nMonth, int nDay)
{
XEXCEPT_ENTRY
	return SetDateTime(nYear, nMonth, nDay, 0, 0, 0); 
XEXCEPT_EXIT
}
int XTime::SetTime(int nHour, int nMin, int nSec)
{
XEXCEPT_ENTRY
	return SetDateTime(1899, 12, 30, nHour, nMin, nSec);
XEXCEPT_EXIT
}
#ifdef WIN32
/*
bool XTime::ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags, LCID lcid)
{
XEXCEPT_ENTRY
	_bstr_t strDate(lpszDate);

	SCODE sc;
	if (FAILED(sc = ::VarDateFromStr((LPOLESTR)(LPCOLESTR)strDate, lcid,	dwFlags, &m_dt)))
	{
		if (sc == DISP_E_TYPEMISMATCH)
		{
			// Can't convert string to date, set 0 and invalidate
			m_dt = 0;
			SetStatus(invalid);
			return false;
		}
		else if (sc == DISP_E_OVERFLOW)
		{
			// Can't convert string to date, set -1 and invalidate
			m_dt = -1;
			SetStatus(invalid);
			return false;
		}
		else
		{
			_com_issue_error(sc);
		}
	}
	SetStatus(valid);
	return true;
XEXCEPT_EXIT
}
*/
#endif //WIN32

XStrBuff XTime::Format(const char *pFormat) const
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XTime::Format"
	___XEXCEPT_CHECK_VALID_TRUE(pFormat != NULL);


	size_t iRet = 0;
	char sz[512] = "";
	struct tm sTimeO;
	XStrBuff  str;
	TmFromOleDate(m_dt, sTimeO);
	TmConvertToStandardFormat( sTimeO );
	iRet = ::strftime(sz, sizeof(sz), pFormat, &sTimeO);
	str = sz;
	return str;
XEXCEPT_EXIT
}
XStrBuff XTime::Format(const wchar_t *pFormat) const
{
XEXCEPT_ENTRY
	#undef  RNAME
	#define RNAME "XTime::Format"
	___XEXCEPT_CHECK_VALID_TRUE(pFormat != NULL);

	return Format( (const char*)XStrBuff(pFormat) );
XEXCEPT_EXIT
}

bool XTime::OleDateFromTm(unsigned short wYear, unsigned short wMonth, unsigned short wDay,
	unsigned short wHour, unsigned short wMinute, unsigned short wSecond, double& dtDest)
{
XEXCEPT_ENTRY
	// 年と月の妥当性のチェック
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return false;

	//  （閏（うるう）年の）2月29日の確認
	bool bLeapYear = ((wYear & 3) == 0) &&((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth =	XTIME_MonthDays[wMonth] - XTIME_MonthDays[wMonth-1] + ((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

	if (wDay < 1 || wDay > nDaysInMonth ||	wHour > 23 || wMinute > 59 ||
		wSecond > 59)
	{
		return false;
	}

	long nDate;
	double dblTime;


	nDate = wYear*365L + wYear/4 - wYear/100 + wYear/400 + XTIME_MonthDays[wMonth-1] + wDay;

	if (wMonth <= 2 && bLeapYear)
		--nDate;

	nDate -= 693959L;

	dblTime = (((long)wHour * 3600L) +
		((long)wMinute * 60L) +
		((long)wSecond)) / 86400.;

	dtDest = (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);

	return true;
XEXCEPT_EXIT
}

bool XTime::TmFromOleDate(double dtSrc, struct tm& tmDest)
{
XEXCEPT_ENTRY
	if (dtSrc > XTIME_MAX_DATE || dtSrc < XTIME_MIN_DATE) // about year 100 to about 9999
		return false;

	long nDays;
	long nDaysAbsolute;
	long nSecsInDay;
	long nMinutesInDay;
	long n400Years;
	long n400Century;
	long n4Years;
	long n4Day;
	long n4Yr;
	bool bLeap4 = true;

	double dblDate = dtSrc;

	nDays = (long)dblDate;

	dblDate += ((dtSrc > 0.0) ? XTIME_HALF_SECOND : -XTIME_HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	tmDest.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

	n400Years = (long)(nDaysAbsolute / 146097L);

	nDaysAbsolute %= 146097L;

	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	if (n400Century != 0)
	{
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = false;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	tmDest.tm_yday = (int)n4Day + 1;
	tmDest.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

	if (n4Yr == 0 && bLeap4)
	{
		if (n4Day == 59)
		{
			tmDest.tm_mon = 2;
			tmDest.tm_mday = 29;
			goto DoTime;
		}

		if (n4Day >= 60)
			--n4Day;
	}

	++n4Day;

	for (tmDest.tm_mon = (n4Day >> 5) + 1;
	n4Day > XTIME_MonthDays[tmDest.tm_mon]; tmDest.tm_mon++);

	tmDest.tm_mday = (int)(n4Day - XTIME_MonthDays[tmDest.tm_mon-1]);

DoTime:
	if (nSecsInDay == 0)
		tmDest.tm_hour = tmDest.tm_min = tmDest.tm_sec = 0;
	else
	{
		tmDest.tm_sec = (int)nSecsInDay % 60L;
		nMinutesInDay = nSecsInDay / 60L;
		tmDest.tm_min = (int)nMinutesInDay % 60;
		tmDest.tm_hour = (int)nMinutesInDay / 60;
	}

	return true;
XEXCEPT_EXIT
}

void XTime::TmConvertToStandardFormat(struct tm& tmSrc)
{
XEXCEPT_ENTRY
	tmSrc.tm_year -= 1900;
	tmSrc.tm_mon -= 1;
	tmSrc.tm_wday -= 1;
	tmSrc.tm_yday -= 1;
XEXCEPT_EXIT
}

double XTime::DoubleFromDate(double dt)
{
XEXCEPT_ENTRY
	if (dt >= 0)
		return dt;

	double temp = ceil(dt);
	return temp - (dt - temp);
XEXCEPT_EXIT
}

double XTime::DateFromDouble(double dbl)
{
XEXCEPT_ENTRY
	if (dbl >= 0)
		return dbl;
	double temp = floor(dbl);
	return temp + (temp - dbl);
XEXCEPT_EXIT
}
void XTime::CheckRange()
{
XEXCEPT_ENTRY
	if (m_dt > XTIME_MAX_DATE || m_dt < XTIME_MIN_DATE)
		SetStatus(invalid);
XEXCEPT_EXIT
}
const XTime& XTime::operator+=(
	const XTimeSpan dateSpan)
{
	*this = *this + dateSpan;
	return *this;
}
const XTime& XTime::operator-=(
	const XTimeSpan dateSpan)
{
	*this = *this - dateSpan;
	return *this;
}

XTime XTime::operator+(const XTimeSpan& dateSpan) const
{
	XTime dateResult;    // Initializes m_status to valid

	// If either operand NULL, result NULL
	if (GetStatus() == null || dateSpan.GetStatus() == null)
	{
		dateResult.SetStatus(null);
		return dateResult;
	}

	// If either operand invalid, result invalid
	if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
	{
		dateResult.SetStatus(invalid);
		return dateResult;
	}

	// Compute the actual date difference by adding underlying dates
	dateResult = XTime::DateFromDouble(XTime::DoubleFromDate(m_dt) + dateSpan.m_span);

	// Validate within range
	dateResult.CheckRange();

	return dateResult;
}

XTime XTime::operator-(const XTimeSpan& dateSpan) const
{
	XTime dateResult;    // Initializes m_status to valid

	// If either operand NULL, result NULL
	if (GetStatus() == null || dateSpan.GetStatus() == null)
	{
		dateResult.SetStatus(null);
		return dateResult;
	}

	// If either operand invalid, result invalid
	if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
	{
		dateResult.SetStatus(invalid);
		return dateResult;
	}

	// Compute the actual date difference by subtracting underlying dates
	dateResult = XTime::DateFromDouble(XTime::DoubleFromDate(m_dt) - dateSpan.m_span);

	// Validate within range
	dateResult.CheckRange();

	return dateResult;
}

XTimeSpan XTime::operator-(const XTime& date) const
{
	XTimeSpan spanResult;

	// If either operand NULL, result NULL
	if (GetStatus() == null || date.GetStatus() == null)
	{
		spanResult.SetStatus(XTimeSpan::null);
		return spanResult;
	}

	// If either operand invalid, result invalid
	if (GetStatus() == invalid || date.GetStatus() == invalid)
	{
		spanResult.SetStatus(XTimeSpan::invalid);
		return spanResult;
	}

	// Return result (span can't be invalid, so don't check range)
	return XTime::DoubleFromDate(m_dt) - XTime::DoubleFromDate(date.m_dt);
}


//----------------------------------------------------------------------
//
//	XTimeSpan
//
//----------------------------------------------------------------------
// XTimeSpan
XTimeSpan::XTimeSpan()
{
	m_span = 0;
	SetStatus(valid);
}
XTimeSpan::XTimeSpan(double dblSpanSrc)
{	m_span = dblSpanSrc;
	SetStatus(valid);
}
XTimeSpan::XTimeSpan(
	const XTimeSpan& dateSpanSrc)
{
	m_span = dateSpanSrc.m_span;
	m_status = dateSpanSrc.m_status;
}
XTimeSpan::XTimeSpan(
	long lDays, int nHours, int nMins, int nSecs)
{
	SetDateTimeSpan(lDays, nHours, nMins, nSecs);
}
XTimeSpan::XTimeSpanStatus XTimeSpan::GetStatus() const
{
	return m_status;
}
void XTimeSpan::SetStatus(XTimeSpanStatus status)
{
	m_status = status;
}
double XTimeSpan::GetTotalDays() const
{
	_XASSERT(GetStatus() == valid);
	return m_span;
}
double XTimeSpan::GetTotalHours() const
{
	_XASSERT(GetStatus() == valid);
	long lReturns = (long)(m_span * 24 + XTIME_HALF_SECOND);
	return lReturns;
}
double XTimeSpan::GetTotalMinutes() const
{
	_XASSERT(GetStatus() == valid);
	long lReturns = (long)(m_span * 24 * 60 + XTIME_HALF_SECOND);
	return lReturns;
}
double XTimeSpan::GetTotalSeconds() const
{
	_XASSERT(GetStatus() == valid);
	long lReturns = (long)(m_span * 24 * 60 * 60 + XTIME_HALF_SECOND);
	return lReturns;
}
long XTimeSpan::GetDays() const
{
	_XASSERT(GetStatus() == valid);
	return (long)m_span;
}
bool XTimeSpan::operator==(
	const XTimeSpan& dateSpan) const
{
	return (m_status == dateSpan.m_status &&
		m_span == dateSpan.m_span);
}
bool XTimeSpan::operator!=(
	const XTimeSpan& dateSpan) const
{
	return (m_status != dateSpan.m_status ||
		m_span != dateSpan.m_span);
}
bool XTimeSpan::operator<(
	const XTimeSpan& dateSpan) const
{
	_XASSERT(GetStatus() == valid);
	_XASSERT(dateSpan.GetStatus() == valid);
	return m_span < dateSpan.m_span;
}
bool XTimeSpan::operator>(
	const XTimeSpan& dateSpan) const
{
	_XASSERT(GetStatus() == valid);
	_XASSERT(dateSpan.GetStatus() == valid);
	return m_span > dateSpan.m_span;
}
bool XTimeSpan::operator<=(
	const XTimeSpan& dateSpan) const
{
	_XASSERT(GetStatus() == valid);
	_XASSERT(dateSpan.GetStatus() == valid);
	return m_span <= dateSpan.m_span;
}
bool XTimeSpan::operator>=(
	const XTimeSpan& dateSpan) const
{
	_XASSERT(GetStatus() == valid);
	_XASSERT(dateSpan.GetStatus() == valid);
	return m_span >= dateSpan.m_span;
}
const XTimeSpan& XTimeSpan::operator+=(
	const XTimeSpan dateSpan)
{
	*this = *this + dateSpan;
	return *this;
}
const XTimeSpan& XTimeSpan::operator-=(
	const XTimeSpan dateSpan)
{
	*this = *this - dateSpan;
	return *this;
}
XTimeSpan XTimeSpan::operator-() const
{
	return -this->m_span;
}
XTimeSpan::operator double() const
{
	return m_span;
}


long XTimeSpan::GetHours() const
{
	_XASSERT(GetStatus() == valid);

	double dblTemp;

	// Truncate days and scale up
	dblTemp = modf(m_span, &dblTemp);

	long lReturns = (long)((dblTemp + XTIME_HALF_SECOND) * 24);
	if (lReturns >= 24)
		lReturns -= 24;

	return lReturns;
}

long XTimeSpan::GetMinutes() const
{
	_XASSERT(GetStatus() == valid);

	double dblTemp;

	// Truncate hours and scale up
	dblTemp = modf(m_span * 24, &dblTemp);

	long lReturns = (long) ((dblTemp + XTIME_HALF_SECOND) * 60);
	if (lReturns >= 60)
		lReturns -= 60;

	return lReturns;
}

long XTimeSpan::GetSeconds() const
{
	_XASSERT(GetStatus() == valid);

	double dblTemp;

	// Truncate minutes and scale up
	dblTemp = modf(m_span * 24 * 60, &dblTemp);

	long lReturns = (long) ((dblTemp + XTIME_HALF_SECOND) * 60);
	if (lReturns >= 60)
		lReturns -= 60;

	return lReturns;
}

const XTimeSpan& XTimeSpan::operator=(double dblSpanSrc)
{
	m_span = dblSpanSrc;
	SetStatus(valid);
	return *this;
}

const XTimeSpan& XTimeSpan::operator=(const XTimeSpan& dateSpanSrc)
{
	m_span = dateSpanSrc.m_span;
	m_status = dateSpanSrc.m_status;
	return *this;
}

XTimeSpan XTimeSpan::operator+(const XTimeSpan& dateSpan) const
{
	XTimeSpan dateSpanTemp;

	// If either operand Null, result Null
	if (GetStatus() == null || dateSpan.GetStatus() == null)
	{
		dateSpanTemp.SetStatus(null);
		return dateSpanTemp;
	}

	// If either operand Invalid, result Invalid
	if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
	{
		dateSpanTemp.SetStatus(invalid);
		return dateSpanTemp;
	}

	// Add spans and validate within legal range
	dateSpanTemp.m_span = m_span + dateSpan.m_span;
	dateSpanTemp.checkRange();

	return dateSpanTemp;
}

XTimeSpan XTimeSpan::operator-(const XTimeSpan& dateSpan) const
{
	XTimeSpan dateSpanTemp;

	// If either operand Null, result Null
	if (GetStatus() == null || dateSpan.GetStatus() == null)
	{
		dateSpanTemp.SetStatus(null);
		return dateSpanTemp;
	}

	// If either operand Invalid, result Invalid
	if (GetStatus() == invalid || dateSpan.GetStatus() == invalid)
	{
		dateSpanTemp.SetStatus(invalid);
		return dateSpanTemp;
	}

	// Subtract spans and validate within legal range
	dateSpanTemp.m_span = m_span - dateSpan.m_span;
	dateSpanTemp.checkRange();

	return dateSpanTemp;
}

void XTimeSpan::SetDateTimeSpan(
	long lDays, int nHours, int nMins, int nSecs)
{
	// Set date span by breaking into fractional days (all input ranges valid)
	m_span = lDays + ((double)nHours)/24 + ((double)nMins)/(24*60) +
		((double)nSecs)/(24*60*60);

	SetStatus(valid);
}
/*
CString XTimeSpan::Format(LPCTSTR pFormat) const
{
	CString strSpan;
	struct tm tmTemp;

	// If null, return empty string
	if (GetStatus() == null)
		return strSpan;

	// If invalid, return DateTimeSpan resource string
	if (GetStatus() == invalid || !_AfxTmFromOleDate(m_span, tmTemp))
	{
		VERIFY(strSpan.LoadString(AFX_IDS_INVALID_DATETIMESPAN));
		return strSpan;
	}

	// Convert tm from afx internal format to standard format
	_AfxTmConvertToStandardFormat(tmTemp);

	// _tcsftime() doesn't handle %D, so do it here

	CString strPreParsed;
	LPCTSTR pstrSource = pFormat;
	int nTargetChar = 0;
	int nAccumulatedLength = lstrlen(pFormat);
	LPTSTR pstrTarget = strPreParsed.GetBuffer(nAccumulatedLength);

	while (*pstrSource)
	{
		if (*pstrSource == '%' && pstrSource[1] == 'D')
		{
			TCHAR szDay[12];
			_itot(GetDays(), szDay, 10);
			strPreParsed.ReleaseBuffer(nTargetChar);
			strPreParsed += szDay;
			int nTemp = lstrlen(szDay);
			nAccumulatedLength += nTemp;
			nTargetChar += nTemp;
			pstrTarget = strPreParsed.GetBuffer(nAccumulatedLength)
				+ nTargetChar;
			pstrSource = _tcsinc(pstrSource);
			pstrSource = _tcsinc(pstrSource);
		}
		*pstrTarget = *pstrSource;
		nTargetChar++;
		pstrSource = _tcsinc(pstrSource);
		pstrTarget = _tcsinc(pstrTarget);
	}
	strPreParsed.ReleaseBuffer(nTargetChar);

	// Fill in the buffer, disregard return value as it's not necessary
	LPTSTR lpszTemp = strSpan.GetBufferSetLength(MAX_TIME_BUFFER_SIZE);
	_tcsftime(lpszTemp, strSpan.GetLength(), (LPCTSTR) strPreParsed, &tmTemp);
	strSpan.ReleaseBuffer();

	return strSpan;
}
*/
void XTimeSpan::checkRange()
{
	if(m_span < -XTIME_MAX_DAYS_IN_SPAN || m_span > XTIME_MAX_DAYS_IN_SPAN)
		SetStatus(invalid);
}



//----------------------------------------------------------------------
//
//	XTimeNonRTC
//
//----------------------------------------------------------------------
XTimeNonRTC::XTimeNonRTC(time_t delta /*= 0*/, time_t autoSyncDist /*= 0*/)
	:	m_delta( delta )
	,	m_autoSyncDist( autoSyncDist )
{
XEXCEPT_ENTRY
#ifdef WIN32
	::QueryPerformanceFrequency(&m_Frequency);
#endif
	syncRTC();
XEXCEPT_EXIT
}
XTimeNonRTC::~XTimeNonRTC()
{
XEXCEPT_ENTRY

XEXCEPT_EXIT
}
void XTimeNonRTC::syncRTC()
{
XEXCEPT_ENTRY
#ifdef WIN32
	::QueryPerformanceCounter(&m_initCounter);
#else
	m_initCounter = ::clock();
#endif
	m_sec = ::time(0);	
XEXCEPT_EXIT
}

void XTimeNonRTC::setDelta(time_t sec)
{
XEXCEPT_ENTRY
	m_delta = sec;
XEXCEPT_EXIT
}

void XTimeNonRTC::setAutoSyncDist(time_t sec)
{
XEXCEPT_ENTRY
	m_autoSyncDist = sec;
XEXCEPT_EXIT
}

time_t	XTimeNonRTC::getTime(time_t* pMsec /*= NULL*/)
{
XEXCEPT_ENTRY
	unsigned long	ulSEC = 0;
	for( int i = 0; i < 2; i++) {
#ifdef WIN32
		LARGE_INTEGER	llCounter;
		::QueryPerformanceCounter(&llCounter);
		ulSEC = (unsigned long)((llCounter.QuadPart - m_initCounter.QuadPart)/m_Frequency.QuadPart);
		if( pMsec != NULL ) {
			*pMsec = (unsigned long)(
									( ((llCounter.QuadPart - m_initCounter.QuadPart)*1000)/m_Frequency.QuadPart )

														-
									(ulSEC*1000)
								);
		}
#else
		clock_t	llCounter = clock();

		ulSEC = (unsigned long)((llCounter - m_initCounter)/CLOCKS_PER_SEC);
		if( pMsec != NULL ) {
			*pMsec = (unsigned long)(
									( ((llCounter - m_initCounter)*1000)/CLOCKS_PER_SEC )

														-
									(ulSEC*1000)
								);
		}
#endif
		if( m_autoSyncDist != 0 ) {
			if( m_autoSyncDist < ulSEC ) {
				syncRTC();
			} else {
				break;
			}
		} else {
			break;
		}
	}
	return (m_sec+ulSEC+m_delta);
XEXCEPT_EXIT
}

XTime XTimeNonRTC::getCurrentTime()
{
XEXCEPT_ENTRY
	time_t ulSec, ulMSec = 0;

	ulSec = this->getTime( &ulMSec );

	return XTime(ulSec, (unsigned long)ulMSec);
XEXCEPT_EXIT
}
#define _XTIME_DAY_SEC           (24 * 60 * 60)       /* secs in a day */
#define _XTIME_YEAR_SEC          (365 * _XTIME_DAY_SEC)
#define _XTIME_MIN_LOCAL_TIME    (-12 * 60 * 60) 
#define _XTIME_MAX__TIME64_T     0x793406fffi64 
#define _XTIME_MAX_LOCAL_TIME    (13 * 60 * 60)
#define _XTIME_LEAP_YEAR_ADJUST  17                   /* Leap years 1900 - 1970 */
#define _XTIME_ELAPSED_LEAP_YEARS(y)  (((y - 1)/4) - ((y - 1)/100) + ((y + 299)/400) \
                                - _XTIME_LEAP_YEAR_ADJUST)

#define _XTIME_IS_LEAP_YEAR(y)        (((y % 4 == 0) && (y % 100 != 0)) || \
                                ((y + 1900) % 400 == 0))


bool XTime::getGmtime(struct tm *ptm, const XTIME_T64 *timp)
{
XEXCEPT_ENTRY
	XD_INT8 caltim;
    int islpyr = 0;                 /* is-current-year-a-leap-year flag */
    int tmptim;
    int *mdays;                     /* pointer to days or lpdays */
    struct tm *ptb = ptm;
	int _lpdays[] = {
			-1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
	};

	int _days[] = {
			-1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
	};
	
	#undef  RNAME
	#define RNAME "XTime::getGmtime"
	___XEXCEPT_CHECK_VALID_TRUE(ptm != NULL);
	___XEXCEPT_CHECK_VALID_TRUE(timp != NULL);

	memset( ptm, 0xff, sizeof( struct tm ) );

	caltim = (XD_INT8)*timp;
	caltim /= 1000;

/*
	if( !(caltim >= _XTIME_MIN_LOCAL_TIME) ) {
		return false;
	}
	if( ( caltim <= _XTIME_MAX__TIME64_T + _XTIME_MAX_LOCAL_TIME ) ) {
		return false;
	}
*/
    tmptim = (int)(caltim / _XTIME_YEAR_SEC) + 70;
    caltim -= ((XTIME_T64)(tmptim - 70) * _XTIME_YEAR_SEC);

	
	caltim -= ((XTIME_T64)_XTIME_ELAPSED_LEAP_YEARS(tmptim) * _XTIME_DAY_SEC);


	if ( caltim < 0 ) {
		caltim += (XTIME_T64)_XTIME_YEAR_SEC;
		tmptim--;
		if ( _XTIME_IS_LEAP_YEAR(tmptim) ) {
			caltim += _XTIME_DAY_SEC;
			islpyr++;
		}
	}
	else
	if ( _XTIME_IS_LEAP_YEAR(tmptim) )
		islpyr++;



	ptb->tm_year = tmptim;


	ptb->tm_yday = (int)(caltim / _XTIME_DAY_SEC);
	caltim -= (XTIME_T64)(ptb->tm_yday) * _XTIME_DAY_SEC;

	if ( islpyr )
		mdays = _lpdays;
	else
		mdays = _days;


	for ( tmptim = 1 ; mdays[tmptim] < ptb->tm_yday ; tmptim++ ) ;

	ptb->tm_mon = --tmptim;

	ptb->tm_mday = ptb->tm_yday - mdays[tmptim];

	
    ptb->tm_hour = (int)(caltim / 3600);
    caltim -= (XTIME_T64)ptb->tm_hour * 3600L;

    ptb->tm_min = (int)(caltim / 60);
    ptb->tm_sec = (int)(caltim - (ptb->tm_min) * 60);

    ptb->tm_isdst = 0;


	return true;
XEXCEPT_EXIT
}

bool XTime::getLocaltime(struct tm *ptm, const XTIME_T64 *ptime)
{
XEXCEPT_ENTRY	
	#undef  RNAME
	#define RNAME "XTime::getLocaltime"


    XTIME_T64 ltime;
    int daylight = 0;
    long dstbias = 0;
    long timezone = 0;
	long thTimeZone = 0;
#if _MSC_VER >= 1310
	::_get_timezone( &thTimeZone );
#else
	thTimeZone = _timezone;
#endif
	___XEXCEPT_CHECK_VALID_TRUE(ptm != NULL);
	___XEXCEPT_CHECK_VALID_TRUE(ptime != NULL);
	if( ptime == NULL ||  ptm == NULL ) {
		return false;
	}

	ltime = *ptime;


	_tzset();

	
	if ( (*ptime/1000) > 3 * _XTIME_DAY_SEC ) {

		ltime = *ptime - (thTimeZone*1000);
		if( getGmtime(ptm, &ltime) == false ) {
			return false;
		}
//		if ( _daylight && _isindst( ptm ) ) {
//			// 夏時間は未実装
//		}
	} else {
		if( getGmtime(ptm, ptime) == false ) {
			return false;
		}

//		if ( daylight && _isindst(ptm) ) {
			// 夏時間は未実装
//			ltime = (XTIME_T64)ptm->tm_sec - (timezone + dstbias);
//			ptm->tm_isdst = 1;
//		}
//		else
			ltime = (XTIME_T64)ptm->tm_sec - timezone;

		ptm->tm_sec = (int)(ltime % 60);
		if ( ptm->tm_sec < 0 ) {
			ptm->tm_sec += 60;
			ltime -= 60;
		}

		ltime = (XTIME_T64)ptm->tm_min + ltime/60;
		ptm->tm_min = (int)(ltime % 60);
		if ( ptm->tm_min < 0 ) {
			ptm->tm_min += 60;
			ltime -= 60;
		}

		ltime = (XTIME_T64)ptm->tm_hour + ltime/60;
		ptm->tm_hour = (int)(ltime % 24);
		if ( ptm->tm_hour < 0 ) {
			ptm->tm_hour += 24;
			ltime -=24;
		}

		ltime /= 24;

		if ( ltime > 0 ) {
			/*
			 * There is no possibility of overflowing the tm_mday
			 * and tm_yday fields since the date can be no later
			 * than January 19.
			 */
			ptm->tm_wday = (ptm->tm_wday + (int)ltime) % 7;
			ptm->tm_mday += (int)ltime;
			ptm->tm_yday += (int)ltime;
		}
		else if ( ltime < 0 ) {
			/*
			 * It is possible to underflow the tm_mday and tm_yday
			 * fields. If this happens, then adjusted date must
			 * lie in December 1969.
			 */
			ptm->tm_wday = (ptm->tm_wday + 7 + (int)ltime) % 7;
			if ( (ptm->tm_mday += (int)ltime) <= 0 ) {
				ptm->tm_mday += 31;
				// according to assumption 4 above, timezone can
				// cause the date to underflow the epoch by
				// more than a day
				ptm->tm_yday = ptm->tm_yday + (int)ltime + 365;
				ptm->tm_mon = 11;
				ptm->tm_year--;
			}
			else {
				ptm->tm_yday += (int)ltime;
			}
		}


	}




	return true;
XEXCEPT_EXIT
}
