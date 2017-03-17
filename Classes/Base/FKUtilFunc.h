/********************************************************************
	created:	2016/07/12
	filename: 	FKUtilFunc
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
namespace FKUtilFunc
{
	// String -> Int
	long long String2Int64(const char* p_strValue);
	// Get url value
	void GetUrlValues(const char* p_strResult, std::map<std::string, std::string>& p_ValueList);
}