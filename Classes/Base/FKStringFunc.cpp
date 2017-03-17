#include "FKStringFunc.h"
//------------------------------------------------------------
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv/iconv.h"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include "../iconv/include/iconv.h"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iconv.h"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#pragma comment(lib,"libiconv.lib") 
#endif
//------------------------------------------------------------
static bool IConvConvert(const std::string& from_charset, 
	const std::string& to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t iconvH = iconv_open(to_charset.c_str(), from_charset.c_str());
	if (!iconvH) return NULL;
	memset(outbuf, 0, outlen);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	size_t ret = iconv(iconvH, pin, &inlen, pout, &outlen);
#else
	size_t ret = iconv(iconvH, &inbuf, &inlen, &outbuf, &outlen);
#endif
	iconv_close(iconvH);

	return ret == (size_t)(-1) ? false : true;
}
//------------------------------------------------------------
// GBK -> UTF8
std::string FKStringFunc::GBK2UTF8(const std::string& p_strSrc)
{
	char *inbuf = const_cast<char*>(p_strSrc.c_str());
	size_t inlen = strlen(inbuf);
	size_t outlen = inlen * 2 + 2;
	char *outbuf = new char[outlen];
	std::string strRet;
	if (IConvConvert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen))
	{
		strRet = outbuf;
	}
	else
	{
		strRet = inbuf;
	}
	delete[]outbuf;
	return strRet;
}
//------------------------------------------------------------
// UTF8 -> GBK
std::string FKStringFunc::UTF82GBK(const std::string& p_strSrc)
{
	char *inbuf = const_cast<char*>(p_strSrc.c_str());
	size_t inlen = strlen(inbuf);
	size_t outlen = inlen * 2 + 2;
	char *outbuf = new char[outlen];
	std::string strRet;
	if (IConvConvert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen))
	{
		strRet = outbuf;
	}
	else
	{
		strRet = inbuf;
	}
	delete[]outbuf;
	return strRet;
}
//------------------------------------------------------------
static int _UTF162UTF8Convert( const WCHAR_T* a_szSrc, int a_nSrcSize, char* a_szDest, int a_nDestSize )
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	wchar_t wCenter[2048] = {0};
	memcpy(wCenter, a_szSrc, a_nSrcSize);
	return WideCharToMultiByte( CP_ACP, 0, wCenter, -1, a_szDest, a_nDestSize, NULL, NULL );
#else
	size_t result;
	iconv_t env;
	env = iconv_open("utf-8","utf-16le");
	if (env==(iconv_t)-1)
	{
		return -1;
	}

	result = iconv(env,(char**)&a_szSrc,(size_t*)&a_nSrcSize,(char**)&a_szDest,(size_t*)&a_nDestSize);
	if (result==(size_t)-1)
	{
		return -1;
	}
	iconv_close(env);
	return (int)result;
#endif
}
//------------------------------------------------------------
// wchar_t -> UTF8
std::string FKStringFunc::UTF162UTF8(const WCHAR_T* p_strSrc, const int p_nSrcSize)
{
	char *a_szDest = new char[p_nSrcSize/2];
	_UTF162UTF8Convert(p_strSrc, p_nSrcSize, a_szDest, p_nSrcSize/2);
	std::string strRet = a_szDest;
	delete [] a_szDest;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return FKStringFunc::GBK2UTF8(strRet.c_str()).c_str();
#endif
	return strRet;
}
//------------------------------------------------------------
static int _UTF82UTF16Convert( const char* a_szSrc, int a_nDestSize, WCHAR_T* a_szDest )
{  
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	wchar_t wCenter[2048] = {0};

	int nCnt = MultiByteToWideChar( CP_ACP, 0, a_szSrc, -1, wCenter, a_nDestSize ); 
	memcpy(a_szDest, wCenter, a_nDestSize);

	return nCnt;
#else  
	size_t result;  
	iconv_t env;  
	int size = strlen(a_szSrc)+1 ;  
	env = iconv_open("utf-16le","utf-8");
	if (env==(iconv_t)-1)  
	{  
		return -1;  
	}  
	result = iconv(env,(char**)&a_szSrc,(size_t*)&size,(char**)&a_szDest,(size_t*)&a_nDestSize);  
	if (result==(size_t)-1)  
	{  
		return -1;  
	}  
	iconv_close(env);  
	return result;  
#endif  
}
//------------------------------------------------------------
// UTF8 -> wchar_t
int FKStringFunc::UTF82UTF16(const char* p_strSrc, const int p_nDestSize, WCHAR_T* p_szDest)
{
	return _UTF82UTF16Convert(p_strSrc, p_nDestSize, p_szDest);
}
//------------------------------------------------------------
// string -> wstring
std::wstring FKStringFunc::StringToWString(const std::string &p_Str)
{
	wchar_t temp[1024] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, p_Str.c_str(), -1, temp, p_Str.length());
	return temp;
}
//------------------------------------------------------------
// wstring -> string
std::string FKStringFunc::WStringToString(const std::wstring &p_WStr)
{
	std::string str(p_WStr.length(), ' ');
	std::copy(p_WStr.begin(), p_WStr.end(), str.begin());
	return str; 
}
//------------------------------------------------------------
int FKStringFunc::CountString( std::string p_Str )
{
	std::wstring wstr = StringToWString(p_Str);
	int nLen = wstr.length();
	return nLen;
}
//------------------------------------------------------------
int FKStringFunc::GetUTF8Btyes( unsigned char c )
{
	int count = 1;
	if(c >= 128) 
	{
		c <<= 1;
		do 
		{
			count++;
			c <<= 1;
		} while(c > 128);
	}

	return count;
}
//------------------------------------------------------------
unsigned char ToHex(unsigned char x)   
{   
	return  x > 9 ? x + 55 : x + 48;   
}  
//------------------------------------------------------------
unsigned char FromHex(unsigned char x)   
{   
	unsigned char y;  
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;  
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;  
	else if (x >= '0' && x <= '9') y = x - '0';  
	else assert(0);  
	return y;  
}  
//------------------------------------------------------------
std::string FKStringFunc::UrlEncode(const std::string& str)  
{  
	std::string strTemp = "";  
	size_t length = str.length();  
	for (size_t i = 0; i < length; i++)  
	{  
		if (isalnum((unsigned char)str[i]) ||   
			(str[i] == '-') ||  
			(str[i] == '_') ||   
			(str[i] == '.') ||   
			(str[i] == '~'))  
			strTemp += str[i];  
		else if (str[i] == ' ')  
			strTemp += "+";  
		else  
		{  
			strTemp += '%';  
			strTemp += ToHex((unsigned char)str[i] >> 4);  
			strTemp += ToHex((unsigned char)str[i] % 16);  
		}  
	}  
	return strTemp;  
}  
//------------------------------------------------------------
std::string FKStringFunc::UrlDecode(const std::string& str)  
{  
	std::string strTemp = "";  
	size_t length = str.length();  
	for (size_t i = 0; i < length; i++)  
	{  
		if (str[i] == '+') strTemp += ' ';  
		else if (str[i] == '%')  
		{  
			assert(i + 2 < length);  
			unsigned char high = FromHex((unsigned char)str[++i]);  
			unsigned char low = FromHex((unsigned char)str[++i]);  
			strTemp += high*16 + low;  
		}  
		else strTemp += str[i];  
	}  
	return strTemp;  
} 
//------------------------------------------------------------
std::string FKStringFunc::Format(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	const size_t SIZE = 512;
	char buffer[SIZE] = { 0 };
	vsnprintf(buffer, SIZE, fmt, ap);

	va_end(ap);

	return std::string(buffer);
}
//------------------------------------------------------------