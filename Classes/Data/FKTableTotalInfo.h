/********************************************************************
	created:	2016/07/21
	filename: 	FKTableTotalInfo
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Base/FKTypeDefine.h"
#include "FKUserTotalInfo.h"
//------------------------------------------------------------
#define MAX_CHAIR_IN_TABLE			8
#define MAX_TABLE_DESCAPTION		32
#define INVALID_TABLE_ID			0xFFFF
#define INVALID_CHAIR_ID			0xFFFF
#define MAX_PLAYER_IN_ROOM			8
//------------------------------------------------------------
#pragma pack(1)
struct STableTotalInfo
{
	bool					bIsHasPassword;
	bool					bIsPlaying;
	WORD					wTableID;
	WORD					wWatcherPlayerCount;
	WORD					wPlayerCount;
	WORD					wChairCount;
	DWORD					dwTableMasterID;
	char					szTableNote[MAX_TABLE_DESCAPTION];
	SFKUserTotalInfo*		pUserData[MAX_CHAIR_IN_TABLE];

	STableTotalInfo()
	{
		bIsHasPassword		= false;
		bIsPlaying			= false;
		wTableID			= INVALID_TABLE_ID;
		wChairCount			= MAX_CHAIR_IN_TABLE;
		wPlayerCount		= 0;
		wWatcherPlayerCount = 0;
		dwTableMasterID		= INVALID_CHAIR_ID;
		memset(szTableNote, 0, sizeof(szTableNote));
		for (int k=0; k<MAX_CHAIR_IN_TABLE; k++)
		{
			pUserData[k] = NULL;
		}
	}

	WORD GetNullChairCount(WORD& wNullChairID)
	{
		if (0 == wChairCount)
		{
			return 0;
		}

		WORD	wNullCount		= 0;
		int		startindex		= 0;
		int		nowindex		= 0;

		wNullChairID			= INVALID_CHAIR_ID;

		for (WORD i=0;i<wChairCount;i++)
		{
			nowindex = (startindex+i)%wChairCount;
			if (pUserData[nowindex]==NULL || 0==pUserData[nowindex]->dwUserID)
			{
				++wNullCount;
				wNullChairID=nowindex;
			}
		}
		return wNullCount;
	};

	bool IsInTable(DWORD dwUserID, WORD &wChairID)
	{
		if (0 == wChairCount)
		{
			return false;
		}
		for (WORD i=0;i<wChairCount;i++)
		{
			if (pUserData[i]!=NULL && pUserData[i]->dwUserID == dwUserID)
			{
				wChairID = i;
				return true;
			}
		}
		return false;
	};

	bool SetUserInfo(WORD wChairID, SFKUserTotalInfo *pak)
	{
		if (INVALID_CHAIR_ID == wChairID)
		{
			return false;
		}
		if (NULL == pUserData[wChairID])
		{
			pUserData[wChairID] = new SFKUserTotalInfo;
			memset(pUserData[wChairID], 0, sizeof(SFKUserTotalInfo));
		}
		memcpy(pUserData[wChairID], pak, sizeof(SFKUserTotalInfo));
		return true;
	}
};

struct SFindTableLimit
{
	bool		bOneNull;							
	bool		bTwoNull;							
	bool		bAllNull;							
	bool		bNotFull;							
	bool		bFilterPass;							
	WORD		wBeginTableID;	

	WORD		wResultTableID;			// result [Out]					
	WORD		wResultChairID;			// result [Out]

	SFindTableLimit()
	{
		memset(this, 0, sizeof(SFindTableLimit));
	}
};
#pragma pack()