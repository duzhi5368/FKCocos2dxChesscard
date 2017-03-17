/********************************************************************
	created:	2016/07/11
	filename: 	FKConfigFunc
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
namespace FKConfigFunc
{
	void			SaveStringValue(const char* p_pKey, const std::string& p_Str);
	void			SaveIntergerValue(const char* p_pKey, int p_nValue);
	void			SaveFloatValue(const char* p_pKey, float p_fValue);
	void			SaveBoolValue(const char* p_pKey, bool p_bValue);

	std::string		ReadStringValue(const char* p_pKey);
	int				ReadIntegerValue(const char* p_pKey);
	float			ReadFloatValue(const char* p_pKey, float p_fDefaultValue);
	bool			ReadBoolValue(const char* p_pKey);

	void			SaveStringValueAfterEncode(const char* p_pKey, const std::string& p_Str);
	void			SaveIntergerValueAfterEncode(const char* p_pKey, int p_nValue);
	void			SaveFloatValueAfterEncode(const char* p_pKey, float p_fValue);
	void			SaveBoolValueAfterEncode(const char* p_pKey, bool p_bValue);

	std::string		ReadStringValueAfterDecode(const char* p_pKey);
	int				ReadIntegerValueAfterDecode(const char* p_pKey);
	float			ReadFloatValueAfterDecode(const char* p_pKey);
	bool			ReadBoolValueAfterDecode(const char* p_pKey);
}
