/********************************************************************
	created:	2016/07/12
	filename: 	FKTime
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Base/FKTypeDefine.h"
#include "../Base/FKBaseMacros.h"
//------------------------------------------------------------
typedef struct STimeInfo 
{
	UInt16 wYear;
	UInt16 wMonth;
	UInt16 wDayOfWeek;
	UInt16 wDay;
	UInt16 wHour;
	UInt16 wMinute;
	UInt16 wSecond;
	UInt16 wMilliseconds;
	
	STimeInfo()
	{
		ZeroMemory(this, sizeof(STimeInfo));
	}
}TIME_INFO;
//------------------------------------------------------------