#include "FKUtilFunc.h"
//------------------------------------------------------------
long long FKUtilFunc::String2Int64(const char* p_strValue)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return _atoi64(p_strValue);
#else
	return atoll(p_strValue);
#endif
}
//------------------------------------------------------------
void FKUtilFunc::GetUrlValues(const char* p_strResult, 
	std::map<std::string, std::string>& p_ValueList)
{
	std::string item, key, value;
	std::string::size_type pos;
	const char *sp1 = "&";
	const char *sp2 = "=";
	char *p1 = strtok((char *)p_strResult, sp1);
	while(p1)
	{
		item = p1;
		pos = item.find(sp2);
		if(pos > 0)
		{
			key = item.substr(0, pos);
			value = item.substr(pos + 1);
			p_ValueList[key] = value;
		}
		p1 = strtok(NULL, sp1);
	}
}
//------------------------------------------------------------