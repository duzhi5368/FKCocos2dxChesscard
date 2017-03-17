/********************************************************************
	created:	2016/07/11
	filename: 	FKEncrpty
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
#define DEFAULT_DEF_MD5_LEN			33
#define MAX_XOR_TIMES				4
#define MAX_SOURCE_LEN				64
#define MAX_ENCRYPT_LEN				(MAX_SOURCE_LEN*MAX_XOR_TIMES)
#define XOR_PASS_LEN				DEFAULT_DEF_MD5_LEN+1
#define DEFAULT_MD5_LEN				33
//------------------------------------------------------------
namespace FKEncrypt
{
	std::string	FKEncodeString(const char* p_pData, unsigned int p_unDataByte);
	std::string	FKDecodeString(const char* p_pData, unsigned int p_unDataByte, unsigned int& p_OutByte);

	WORD		FKXOREncryptData(const char* pszSrcData, char* pszEncrypData, WORD wSize = MAX_ENCRYPT_LEN);
	WORD		FKXORDecryptData(const char* pszEncrypData, char* pszSrcData, WORD wSize = XOR_PASS_LEN);

	void		FKMD5EncryptData(std::string strSrcData, char szMD5Result[DEFAULT_MD5_LEN]);
}
