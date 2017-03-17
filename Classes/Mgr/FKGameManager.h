/********************************************************************
	created:	2016/07/19
	filename: 	FKGameManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "../Data/FKUserData.h"
#include "../NetMsg/CMD_MsgDefine.h"
#include "../Data/FKUserTotalInfo.h"
#include "../Data/FKTableTotalInfo.h"
#include "../Data/FKGameTotalInfo.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKGameManager : public CCObject
{
public:
	CFKGameManager();
	~CFKGameManager();
public:
	static CFKGameManager*	GetInstancePtr();
	static CFKGameManager&	GetInstance();
public:
	bool					Init();
public:
	void					Clear();
	void					OnLoginIn(SMsgBuffer *buffer, SDataUser user);
public:
	SFKUserTotalInfo*		GetMyInfo();
	SFKUserTotalInfo*		GetUserData(WORD wTable, WORD wChair);
	SFKUserTotalInfo*		GetUserDataByChairIDInSameTable(WORD wChair);
	SFKUserTotalInfo*		GetUserData(DWORD dwUserID);
	SFKUserLoginInfo*		GetUserGlobalInfo();
	bool					SetMyInfo(SFKUserTotalInfo* pak);
	bool					SetUserInfo(SFKUserTotalInfo *pak);
	bool					IsAllowAvertCheatMode();
	bool					AddRoomUsers(SFKUserTotalInfo *pak);
	bool					DelRoomUsers(DWORD dwUserID);
	void					ClearRoomUsers();

	bool					AddRooms(SGameServerInfo* pak);
	void					ClearRooms();
	int						GetGameKindCnt();
	int						GetRoomCnt(int kind, std::vector<SGameServerInfo>& verRooms,
		unsigned int sortid_start = 0, unsigned int sortid_end = -1);
	void					SortRooms();
	bool					GetRoom(int index, SGameServerInfo& pak, int kind);

	void					ResetReqValue();
	bool					OnSearchPos();
	bool					FindGameTable(SFindTableLimit& tagTableInfo);
	WORD					GetNullChairCount(WORD& wNullChairID, STableTotalInfo *pak);
	bool					CanSitDownTable(WORD wTableID, WORD wChairID);
private:
	SFKUserLoginInfo*					m_pUserInfo;
	CCArray*							m_pServerList;
	SGameTables*						m_pTablesList;
	std::vector<SGameServerInfo>		m_vecGamesInfos;
	std::map<DWORD, SFKUserTotalInfo*>	m_pRoomUserInfos;
	WORD                				m_wReqTableID;
	WORD                				m_wReqChairID;  
};