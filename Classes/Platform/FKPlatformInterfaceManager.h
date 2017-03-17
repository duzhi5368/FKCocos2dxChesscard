/********************************************************************
	created:	2016/07/12
	filename: 	FKPlatformInterfaceManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKPlatformInterfaceManager : public CCObject
{
public:
	CFKPlatformInterfaceManager();
	virtual ~CFKPlatformInterfaceManager();
public:
	static CFKPlatformInterfaceManager*	GetInstancePtr();
	static CFKPlatformInterfaceManager&	GetInstance();
public:
	virtual bool		Init();
public:
	std::string			GetDeviceID();
	std::string			GetDeviceModel();
	std::string			GetUserName();
	std::string			GetMacAddress();

	void				DoVibrator(int p_nTime = 500);
	void				SetWakeLock(long p_lTimeOut);

	void				ShowExitDialog(const char* p_szTitle, const char* p_szMsg);
	
	void				OpenUrl(const char* p_szUrl);
};