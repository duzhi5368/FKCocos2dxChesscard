/********************************************************************
	created:	2016/07/12
	filename: 	FKStringFunc
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKTypeDefine.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
// cookies
#define FK_UTF8(arg)	FKStringFunc::GBK2UTF8(arg).c_str()
//------------------------------------------------------------
namespace FKStringFunc
{
	// GBK -> UTF8
	std::string		GBK2UTF8(const std::string& p_strSrc);
	// UTF8 -> GBK
	std::string		UTF82GBK(const std::string& p_strSrc);

	// wchar_t -> UTF8
	std::string		UTF162UTF8(const WCHAR_T* p_strSrc, const int p_nSrcSize);
	// UTF8 -> wchar_t
	int				UTF82UTF16(const char* p_strSrc, const int p_nDestSize, WCHAR_T* p_szDest);

	// string -> wstring
	std::wstring	StringToWString(const std::string &p_Str);
	// wstring -> string
	std::string		WStringToString(const std::wstring &p_WStr);

	// get string really length
	int				CountString( std::string p_Str );
	// get character's bytes
	int				GetUTF8Btyes( unsigned char c );

	// transform string to url like(张三 = %D5%C5%C8%FD), used for http url
	std::string		UrlEncode(const std::string& str);
	// transform url to string like(%D5%C5%C8%FD = 张三), used for http url
	std::string		UrlDecode(const std::string& str);

	std::string		Format(const char *fmt, ...);
}
