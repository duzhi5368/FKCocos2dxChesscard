/********************************************************************
	created:	2016/07/21
	filename: 	FKGameTotalInfo
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "FKTableTotalInfo.h"
//------------------------------------------------------------
#define MAX_SERVER_NAME_LEN		32
#define MAX_SERVER_ADDR_LEN		32
//------------------------------------------------------------
struct SGameTables
{
	WORD							wTableCount;
	WORD							wChairCount;
	WORD							wGameType;
	DWORD							dwGameRuleType;		// see FKServerRuleMgr
	STableTotalInfo *				pTableInfoList;

	SGameTables()
	{
		wTableCount			= 0;
		wChairCount			= 0;
		wGameType			= 0;
		dwGameRuleType		= 0;
		pTableInfoList		= NULL;
	}
};
//------------------------------------------------------------
struct SGameServerInfo
{
	WORD							wSortID;
	WORD							wGameKindID;
	WORD							wGameServerID;
	WORD							wGameServerPort;
	DWORD							dwOnLineCount;
	DWORD							dwFullCount;
	WCHAR_T							szServerAddr[MAX_SERVER_ADDR_LEN];
	WCHAR_T							szServerName[MAX_SERVER_NAME_LEN];

	SGameServerInfo()
	{
		memset( this, 0, sizeof(SGameServerInfo));
	}
};
//------------------------------------------------------------