/********************************************************************
	created:	2016/07/18
	filename: 	FKLoginData
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
struct SDataLogin
{
	std::string m_strIp;
	std::string m_strPort;
	int			m_nVersion;

	SDataLogin()
	{
		Init();
	}

	void Init()
	{
		m_strIp			= "127.0.0.1";
		m_strPort		= "8100";
		m_nVersion		= 0;
	}
};
