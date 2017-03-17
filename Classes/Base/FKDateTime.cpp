#include "FKDateTime.h"
#include "FKDataTimeSpan.h"
//------------------------------------------------------------
CFKDateTime::CFKDateTime()
	: m_dTime( 0.0 )
{

}
//------------------------------------------------------------
CFKDateTime::CFKDateTime(const CFKDateTime& dt)
	: m_dTime( dt.m_dTime )
{

}
//------------------------------------------------------------
CFKDateTime::CFKDateTime(double dt)
	: m_dTime( dt )
{

}
//------------------------------------------------------------
CFKDateTime::CFKDateTime(STimeInfo& info)
{
	_TimeInfo2Double(info, m_dTime);
}
//------------------------------------------------------------
CFKDateTime::CFKDateTime(UInt16 nYear, UInt16 nMonth, UInt16 nDay, 
	UInt16 nHour, UInt16 nMin, UInt16 nSec, UInt16 nMillisec)
{
	_TimeInfo2Double(nYear, nMonth, nDay, nHour, nMin, nSec, nMillisec, m_dTime);
}
//------------------------------------------------------------
CFKDateTime::~CFKDateTime()
{

}
//------------------------------------------------------------
UInt16 CFKDateTime::GetYear()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info.wYear;
}
//------------------------------------------------------------
UInt16 CFKDateTime::GetMonth()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info.wMonth;
}
//------------------------------------------------------------
UInt16 CFKDateTime::GetDay()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info.wDay;
}
//------------------------------------------------------------
UInt16 CFKDateTime::GetHour()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info.wHour;
}
//------------------------------------------------------------
UInt16 CFKDateTime::GetMinute()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info.wMinute;
}
//------------------------------------------------------------
UInt16 CFKDateTime::GetSecond()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info.wSecond;
}
//------------------------------------------------------------
UInt16 CFKDateTime::GetMillisecond()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info.wMilliseconds;
}
//------------------------------------------------------------
UInt16 CFKDateTime::GetDayOfWeek()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info.wDayOfWeek;
}
//------------------------------------------------------------
UInt16 CFKDateTime::GetDayOfYear()
{
	return 0;
}
//------------------------------------------------------------
STimeInfo CFKDateTime::GetTimeInfo()
{
	STimeInfo info;
	_Double2TimeInfo(m_dTime, info);

	return info;
}
//------------------------------------------------------------
CFKDateTime CFKDateTime::GetCurrentDateTime()
{
	UInt16 nYear = 0;
	UInt16 nMonth = 0;
	UInt16 nDay = 0;
	UInt16 nHour = 0;
	UInt16 nMinute = 0;
	UInt16 nSecond = 0;
	UInt16 nMilSec = 0;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	SYSTEMTIME t;
	GetLocalTime(&t);
	//	GetSystemTime(&t);
	nYear = t.wYear;
	nMonth = t.wMonth;
	nDay = t.wDay;
	nHour = t.wHour;
	nMinute = t.wMinute;
	nSecond = t.wSecond;
	nMilSec = t.wMilliseconds;
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	// posix compliant system
	struct timeval tv;
	gettimeofday(&tv, 0);
	time_t ltime = tv.tv_sec;
	time_t ldate;
	time(&ldate);
	tm *time, *date;

#if defined(QT_THREAD_SUPPORT) && defined(_POSIX_THREAD_SAFE_FUNCTIONS)
	// use the reentrant versions of localtime() and gmtime() where available
	tm res;
	time = localtime_r(&ltime, &res);
	date = localtime_r(&ldate, &res);
#else
	time = localtime(&ltime);
	date = localtime(&ldate);
#endif // QT_THREAD_SUPPORT && _POSIX_THREAD_SAFE_FUNCTIONS

	nYear = date->tm_year + 1900;
	nMonth = date->tm_mon + 1;
	nDay = date->tm_mday;
	nHour = time->tm_hour;
	nMinute = time->tm_min;
	nSecond = time->tm_sec;
	nMilSec = tv.tv_usec / 1000;
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	time_t ltime;	// no millisecond resolution
	::time(&ltime);
	tm *t;
	t = localtime(&ltime);

	nYear = t->tm_year + 1900;
	nMonth = t->tm_mon + 1;
	nDay = t->tm_mday;
	nHour = t->tm_hour;
	nMinute = t->tm_min;
	nSecond = t->tm_sec;
	nMilSec = 0;
#endif

	CFKDateTime dt = CFKDateTime(nYear, nMonth, nDay, nHour, nMinute, nSecond, nMilSec);
	return dt;
}
//------------------------------------------------------------
CFKDateTime::operator double()
{
	return m_dTime;
}
//------------------------------------------------------------
CFKDateTime::operator double*()
{
	return &m_dTime;
}
//------------------------------------------------------------
CFKDateTime& CFKDateTime::operator =(const CFKDateTime& p_SrcTime)
{
	m_dTime = p_SrcTime.m_dTime;
	return *this;
}
//------------------------------------------------------------
CFKDateTime& CFKDateTime::operator =(const double p_SrcTime)
{
	m_dTime = p_SrcTime;
	return *this;
}
//------------------------------------------------------------
CFKDateTime	 CFKDateTime::operator +(const CFKDateTimeSpan& span)
{
	CFKDateTime dst( m_dTime + (double)span );
	return dst;
}
//------------------------------------------------------------
CFKDateTime	 CFKDateTime::operator -(const CFKDateTimeSpan& span)
{
	CFKDateTime dst( m_dTime - (double)span );
	return dst;
}
//------------------------------------------------------------
CFKDateTimeSpan CFKDateTime::operator -(const CFKDateTime& dt)
{
	CFKDateTime dst( m_dTime - dt.m_dTime );
	return CFKDateTimeSpan(dst);
}
//------------------------------------------------------------
CFKDateTime& CFKDateTime::operator +=(const CFKDateTimeSpan span)
{
	m_dTime += (double)span;
	return *this;
}
//------------------------------------------------------------
CFKDateTime& CFKDateTime::operator -=(const CFKDateTimeSpan span)
{
	m_dTime -= (double)span;
	return *this;
}
//------------------------------------------------------------
bool CFKDateTime::operator ==(const CFKDateTime& dt)
{
	return m_dTime == dt.m_dTime;
}
//------------------------------------------------------------
bool CFKDateTime::operator !=(const CFKDateTime& dt)
{
	return m_dTime != dt.m_dTime;
}
//------------------------------------------------------------
bool CFKDateTime::operator <(const CFKDateTime& dt)
{
	return m_dTime < dt.m_dTime;
}
//------------------------------------------------------------
bool CFKDateTime::operator >(const CFKDateTime& dt)
{
	return m_dTime > dt.m_dTime;
}
//------------------------------------------------------------
bool CFKDateTime::operator <=(const CFKDateTime& dt)
{
	return m_dTime <= dt.m_dTime;
}
//------------------------------------------------------------
bool CFKDateTime::operator >=(const CFKDateTime& dt)
{
	return m_dTime >= dt.m_dTime;
}
//------------------------------------------------------------
#define MAX_TIME_BUFFER_SIZE    128         // matches that in timecore.cpp
#define MIN_DATE                (-657434)	// about year 100
#define MAX_DATE                2958465		// about year 9999
#define HALF_SECOND				(1.0/172800.0)
#define HALF_MILLINSEC			(1.0/172800.0/1000.0)

static int s_nMonthDays[13] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
//------------------------------------------------------------
bool CFKDateTime::_Double2TimeInfo(double dtSrc, STimeInfo& systm)
{
	// The legal range does not actually span year 0 to 9999.
	if ((dtSrc > MAX_DATE) || (dtSrc < MIN_DATE)) // about year 100 to about 9999
	{
		return false;
	}

	int nDays = 0;				// Number of days since Dec. 30, 1899
	int nDaysAbsolute = 0;		// Number of days since 1/1/0
	int nSecsInDay = 0;			// Time in seconds since midnight
	int nMinutesInDay = 0;		// Minutes in day

	int n400Years = 0;			// Number of 400 year increments since 1/1/0
	int n400Century = 0;		// Century within 400 year block (0,1,2 or 3)
	int n4Years = 0;			// Number of 4 year increments since 1/1/0
	int n4Day = 0;				// Day within 4 year block
								//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	int n4Yr = 0;				// Year within 4 year block (0,1,2 or 3)
	bool bLeap4 = true;			// true if 4 year block includes leap year

	double dblDate = dtSrc;		// tempory serial date

	// If a valid date, then this conversion should not overflow
	nDays = (int)dblDate;

	// Round to the second
	if(dtSrc > 0.0)
	{
		dblDate += HALF_MILLINSEC;
	}
	else
	{
		dblDate += -HALF_MILLINSEC;
	}

	nDaysAbsolute = (int)dblDate + 693959; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	int nMillinsecsInDay = (int)((dblDate - floor(dblDate)) * 86400.0 * 1000.0);

	// Calculate the day of week (sun=1, mon=2...)
	//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
	systm.wDayOfWeek = (UInt16)((nDaysAbsolute - 1) % 7) + 1;

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (int)(nDaysAbsolute / 146097);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097;

	// -1 because first century has extra day
	n400Century = (int)((nDaysAbsolute - 1) / 36524);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (int)((nDaysAbsolute + 1) / 1461);

		if (n4Years != 0)
		{
			n4Day = (int)((nDaysAbsolute + 1) % 1461);
		}
		else
		{
			bLeap4 = false;
			n4Day = (int)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (int)(nDaysAbsolute / 1461);
		n4Day = (int)(nDaysAbsolute % 1461);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
		{
			n4Day = (n4Day - 1) % 365;
		}
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	// n4Day is now 0-based day of year. Save 1-based day of year, year number
	systm.wYear = (UInt16)(n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr);

	// Handle leap year: before, on, and after Feb. 29.
	if ((n4Yr == 0) && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			// Feb. 29 
			systm.wMonth = 2;
			systm.wDay = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
		{
			--n4Day;
		}
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time 
	for (systm.wMonth = (n4Day >> 5) + 1;
		n4Day > s_nMonthDays[systm.wMonth]; systm.wMonth++);

		systm.wDay = (int)(n4Day - s_nMonthDays[systm.wMonth-1]);

DoTime:
	if (nMillinsecsInDay == 0)
	{
		systm.wHour = systm.wMinute = systm.wSecond = systm.wMilliseconds = 0;
	}
	else
	{
		systm.wMilliseconds = nMillinsecsInDay % 1000;
		nSecsInDay = nMillinsecsInDay / 1000;
		systm.wSecond = (unsigned short)(nSecsInDay % 60);
		nMinutesInDay = nSecsInDay / 60;
		systm.wMinute = (int)nMinutesInDay % 60;
		systm.wHour = (int)nMinutesInDay / 60;
	}

	return true;
}
//------------------------------------------------------------
bool CFKDateTime::_TimeInfo2Double(UInt16 nYear, UInt16 nMonth, UInt16 nDay,
	UInt16 nHour, UInt16 nMinute, UInt16 nSecond, UInt16 nMillisec, double& dtDest)
{
	// Validate year and month (ignore day of week and milliseconds)
	if ((nYear > 9999) || (nMonth < 1) || (nMonth > 12))
	{
		return false;
	}

	//  Check for leap year and set the number of days in the month
	bool bLeapYear = ((nYear & 3) == 0) &&
		((nYear % 100) != 0 || (nYear % 400) == 0);

	int nDaysInMonth = 0;
	if(bLeapYear && nDay == 29 && nMonth == 2)
	{
		nDaysInMonth = s_nMonthDays[nMonth] - s_nMonthDays[nMonth-1] + 1;
	}
	else
	{
		nDaysInMonth = s_nMonthDays[nMonth] - s_nMonthDays[nMonth-1];
	}

	// Finish validating the date
	if ((nDay < 1) || (nDay > nDaysInMonth) ||
		(nHour > 23) || (nMinute > 59) ||
		(nSecond > 59))
	{
		return false;
	}

	// Cache the date in days and time in fractional days
	int nDate = 0;
	double dblTime = 0;

	//It is a valid date; make Jan 1, 1AD be 1
	nDate = nYear*365 + nYear/4 - nYear/100 + nYear/400 +
		s_nMonthDays[nMonth-1] + nDay;

	//  If leap year and it's before March, subtract 1:
	if ((nMonth <= 2) && bLeapYear)
	{
		--nDate;
	}

	//  Offset so that 12/30/1899 is 0
	nDate -= 693959;

	dblTime = (((int)nHour * 3600) +	// hrs in seconds
		((int)nMinute * 60) +			// mins in seconds
		((int)nSecond)) / 86400. + ((int)nMillisec) / 86400000.0;

	if(nDate >= 0)
	{
		dtDest = (double)nDate + dblTime;
	}
	else
	{
		dtDest = (double)nDate - dblTime;
	}

	return true;
}
//------------------------------------------------------------
bool CFKDateTime::_TimeInfo2Double(STimeInfo& tagSrc, double& dtDest)
{
	return _TimeInfo2Double( tagSrc.wYear, tagSrc.wMonth, tagSrc.wDay,
		tagSrc.wHour, tagSrc.wMinute, tagSrc.wSecond, tagSrc.wMilliseconds, dtDest );
}
//------------------------------------------------------------