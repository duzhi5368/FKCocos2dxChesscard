/********************************************************************
	created:	2016/07/13
	filename: 	FKDataTimeSpan
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKDateTimeSpan
{
public:
	CFKDateTimeSpan();
	CFKDateTimeSpan(const CFKDateTimeSpan& span);
	CFKDateTimeSpan(double dblspan);
	CFKDateTimeSpan(int lDays, int nHours, int nMins, int nSecs, int nMillisecs);

	int					GetDays();
	int					GetHours();
	int					GetMinutes();
	int					GetSeconds();
	int					GetMilliseconds();
	double				GetTotalDays();
	double				GetTotalHours();
	double				GetTotalMinutes();
	double				GetTotalSeconds();
	double				GetTotalMilliseconds();

	operator			double() const;
	CFKDateTimeSpan&	operator =(double dblsrc);
	CFKDateTimeSpan&	operator =(const CFKDateTimeSpan& span);
	CFKDateTimeSpan		operator +(const CFKDateTimeSpan& span);
	CFKDateTimeSpan		operator -(const CFKDateTimeSpan& span);
	CFKDateTimeSpan&	operator +=(const CFKDateTimeSpan& span);
	CFKDateTimeSpan&	operator -=(const CFKDateTimeSpan& span);
	bool				operator ==(const CFKDateTimeSpan& span);
	bool				operator !=(const CFKDateTimeSpan& span);
	bool				operator <(const CFKDateTimeSpan& span);
	bool				operator >(const CFKDateTimeSpan& span);
	bool				operator <=(const CFKDateTimeSpan& span);
	bool				operator >=(const CFKDateTimeSpan& span);

protected:
	double				m_dblspan;
};