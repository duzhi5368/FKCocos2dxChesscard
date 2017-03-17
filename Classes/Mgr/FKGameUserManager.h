/********************************************************************
	created:	2016/08/02
	filename: 	FKGameUserManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Interface/IGameUserInfoMgr.h"
//------------------------------------------------------------
class CFKGameUserManager 
{
public:
	CFKGameUserManager();
	~CFKGameUserManager();

public:
	bool				SetGameUserMgr(IFKGameUserMgr* pGameUserInfoMgr);
	bool				ResetUserManager();
	bool				DeleteUserInfo(IFKGameUserInfo* pGameUserItem);
	bool				DeleteUserInfo(DWORD dwUserID);
	IFKGameUserInfo*	ActiveUserItem(SFKUserTotalInfo* pUserInfo);
	bool				UpdateUserItemScore(IFKGameUserInfo* pGameUserItem, 
		tagUserScore* pUserScore);
	//更新玩家分数
	bool UpdateUserItemScore(DWORD dwUserID, tagUserScore* pUserScore);
	//更新玩家状态
	bool UpdateUserItemStatus(IGameUserItem* pGameUserItem, BYTE cbUserStatus);
	//更新玩家分数
	bool UpdateUserItemStatus(DWORD dwUserID, BYTE cbUserStatus);
	//获取对应座位号玩家信息
	IGameUserItem* GetTableUserItem(WORD wChairID);

	//通过玩家ID搜索玩家
	IGameUserItem* SearchUserItemByUserID(DWORD dwUserID);

	//成员函数
private:
	GameUserItem*										m_GameUserItem[MAX_CHAIR];	//桌上玩家;
	GameUserItemVector									m_vctLookOnUser;			//旁观玩家
	GameUserItemVector									m_vctStorageUser;			//存储玩家

	IGameUserManagerSink*								m_pGameUserManagerSink;		//ClientKernel指针
};

