/********************************************************************
	created:	2016/07/18
	filename: 	FKUserData
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
#define MIN_USER_PASSWORD_LENGTH				5
#define MAX_USER_PASSWORD_LENGTH				33
#define USER_PASSWORD_MD5_LENGTH				33
#define MAX_USER_MACHINE_LENGTH					33
#define MIN_USER_NAME_LENGTH					5
#define MAX_USER_NAME_LENGTH					24
#define MAX_USER_NICKNAME_LENGTH				32
//------------------------------------------------------------
struct SDataUser
{
	std::string m_strAccount;
	std::string m_strPassword;
	char		m_szMD5Password[USER_PASSWORD_MD5_LENGTH];
	char		m_szMachineID[MAX_USER_MACHINE_LENGTH];
	int			m_nIndex;

	SDataUser()
	{
		Clear();
	}
	void Clear()
	{
		m_nIndex = 0;
		m_strAccount.clear();
		m_strPassword.clear();
		memset(m_szMD5Password, 0, sizeof(m_szMD5Password));
		memset(m_szMachineID, 0, sizeof(m_szMachineID));
	};

	void ClearPassword()
	{
		m_strPassword.clear();
		memset(m_szMD5Password, 0, sizeof(m_szMD5Password));
	};
};
