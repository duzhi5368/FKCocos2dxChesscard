#include "FKConfigFunc.h"
#include "FKEncrpty.h"
//------------------------------------------------------------
void FKConfigFunc::SaveStringValue(const char* p_pKey, const std::string& p_Str)
{
	CCUserDefault::sharedUserDefault()->setStringForKey(p_pKey, p_Str);
}
//------------------------------------------------------------
void FKConfigFunc::SaveIntergerValue(const char* p_pKey, int p_nValue)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey(p_pKey, p_nValue);
}
//------------------------------------------------------------
void FKConfigFunc::SaveFloatValue(const char* p_pKey, float p_fValue)
{
	CCUserDefault::sharedUserDefault()->setFloatForKey(p_pKey, p_fValue);
}
//------------------------------------------------------------
void FKConfigFunc::SaveBoolValue(const char* p_pKey, bool p_bValue)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey(p_pKey, p_bValue);
}
//------------------------------------------------------------
std::string FKConfigFunc::ReadStringValue(const char* p_pKey)
{
	return CCUserDefault::sharedUserDefault()->getStringForKey(p_pKey);
}
//------------------------------------------------------------
int FKConfigFunc::ReadIntegerValue(const char* p_pKey)
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(p_pKey);
}
//------------------------------------------------------------
float FKConfigFunc::ReadFloatValue(const char* p_pKey, float p_fDefaultValue)
{
	return CCUserDefault::sharedUserDefault()->getFloatForKey(p_pKey, p_fDefaultValue);
}
//------------------------------------------------------------
bool FKConfigFunc::ReadBoolValue(const char* p_pKey)
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey(p_pKey);
}
//------------------------------------------------------------
void FKConfigFunc::SaveStringValueAfterEncode(const char* p_pKey, const std::string& p_Str)
{
	std::string output = FKEncrypt::FKEncodeString(p_Str.c_str(), p_Str.length());
	SaveStringValue(p_pKey, output);
}
//------------------------------------------------------------
void FKConfigFunc::SaveIntergerValueAfterEncode(const char* p_pKey, int p_nValue)
{
	SaveStringValueAfterEncode(p_pKey, CCString::createWithFormat("%d", p_nValue)->getCString());
}
//------------------------------------------------------------
void FKConfigFunc::SaveFloatValueAfterEncode(const char* p_pKey, float p_fValue)
{
	SaveStringValueAfterEncode(p_pKey, CCString::createWithFormat("%.2f", p_fValue)->getCString());
}
//------------------------------------------------------------
void FKConfigFunc::SaveBoolValueAfterEncode(const char* p_pKey, bool p_bValue)
{
	SaveStringValueAfterEncode(p_pKey, p_bValue ? "true" : "false");
}
//------------------------------------------------------------
std::string	FKConfigFunc::ReadStringValueAfterDecode(const char* p_pKey)
{
	unsigned int outLen;
	std::string input = ReadStringValue(p_pKey);
	return FKEncrypt::FKDecodeString(input.c_str(), strlen(input.c_str()), outLen);
}
//------------------------------------------------------------
int FKConfigFunc::ReadIntegerValueAfterDecode(const char* p_pKey)
{
	std::string input = ReadStringValueAfterDecode(p_pKey);
	if (input.length() < 1) 
		return 0;
	return atoi(input.c_str());
}
//------------------------------------------------------------
float FKConfigFunc::ReadFloatValueAfterDecode(const char* p_pKey)
{
	std::string input = ReadStringValueAfterDecode(p_pKey);
	if (input.length() < 1) 
		return 0.0f;
	return atof(input.c_str());
}
//------------------------------------------------------------
bool FKConfigFunc::ReadBoolValueAfterDecode(const char* p_pKey)
{
	std::string input = ReadStringValueAfterDecode(p_pKey);
	return (input == "true");
}
//------------------------------------------------------------