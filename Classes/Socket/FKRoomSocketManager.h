/********************************************************************
	created:	2016/07/18
	filename: 	FKRoomSocketManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKSocketManager.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKRoomSocketManager : public CFKSocketManager
{
public:
	CFKRoomSocketManager();
	~CFKRoomSocketManager();
public:
	static CFKRoomSocketManager*	GetInstancePtr();
	static CFKRoomSocketManager&	GetInstance();
public:
	bool						Init();
	void						Update(float dt);
public:
	bool SendStandUpPacket();
	bool SendSitDownPacket(WORD wTableID, WORD wChairID, std::string pszTablePass = "");
};