/********************************************************************
	created:	2016/08/02
	filename: 	IGameUserMgr
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "IGameUserInfo.h"
//------------------------------------------------------------
class IFKGameUserMgr
{
public:
	virtual void OnUserInfoAcitve(IFKGameUserInfo* pGameUserItem) = 0;
	virtual void OnUserInfoDelete(IFKGameUserInfo* pGameUserItem) = 0;
	virtual void OnUserInfoUpdateScore(IFKGameUserInfo* pGameUserItem) = 0;
	virtual void OnUserInfoUpdateStatue(IFKGameUserInfo* pGameUserItem) = 0;
};
