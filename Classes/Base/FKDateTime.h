/********************************************************************
	created:	2016/07/12
	filename: 	FKDateTime
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "../Data/FKTime.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKDateTimeSpan;
class CFKDateTime  
{
public:
	CFKDateTime();
	CFKDateTime(const CFKDateTime& dt);
	CFKDateTime(double dt);
	CFKDateTime(STimeInfo& info);
	CFKDateTime(UInt16 nYear, UInt16 nMonth, UInt16 nDay, 
		UInt16 nHour, UInt16 nMin, UInt16 nSec, UInt16 nMillisec);
	virtual ~CFKDateTime();
public:
	UInt16			GetYear();
	UInt16			GetMonth();
	UInt16			GetDay();
	UInt16			GetHour();
	UInt16			GetMinute();
	UInt16			GetSecond();
	UInt16			GetMillisecond();
	UInt16			GetDayOfWeek();
	UInt16			GetDayOfYear();

	STimeInfo		GetTimeInfo();
public:
	static CFKDateTime GetCurrentDateTime();
public:
	operator		double();
	operator		double*();
	CFKDateTime&	operator =(const CFKDateTime& p_SrcTime);
	CFKDateTime&	operator =(const double p_SrcTime);
	CFKDateTime		operator +(const CFKDateTimeSpan& span);
	CFKDateTime		operator -(const CFKDateTimeSpan& span);
	CFKDateTimeSpan operator -(const CFKDateTime& dt);
	CFKDateTime&	operator +=(const CFKDateTimeSpan span);
	CFKDateTime&	operator -=(const CFKDateTimeSpan span);
	bool			operator ==(const CFKDateTime& dt);
	bool			operator !=(const CFKDateTime& dt);
	bool			operator <(const CFKDateTime& dt);
	bool			operator >(const CFKDateTime& dt);
	bool			operator <=(const CFKDateTime& dt);
	bool			operator >=(const CFKDateTime& dt);

private:
	static bool		_Double2TimeInfo(double dt, STimeInfo& tagSrc);
	static bool		_TimeInfo2Double(UInt16 wYear, UInt16 wMonth, UInt16 wDay,
		UInt16 wHour, UInt16 wMinute, UInt16 wSecond, UInt16 wMillisec, double& dtDest);
	static bool		_TimeInfo2Double(STimeInfo& tagSrc, double& dtDest);

protected:
	double			m_dTime;
};
