#include "FKGameManager.h"
#include "../Base/FKBaseMacros.h"
#include "../Base/FKStringFunc.h"
#include "../Data/FKTableTotalInfo.h"
#include "../Data/FKDataCenterManager.h"
#include "../Mgr/FKServerRulesMgr.h"
#include "../NetMsg/CMD_Login.h"
#include "../Socket/FKRoomSocketManager.h"
//------------------------------------------------------------
static CFKGameManager* g_pGameMgr = NULL;
//------------------------------------------------------------
CFKGameManager::CFKGameManager()
	: m_pServerList( NULL )
	, m_pTablesList( NULL )
	, m_pUserInfo( NULL )
	, m_wReqChairID( INVALID_CHAIR_ID )
	, m_wReqTableID( INVALID_TABLE_ID )
{
	m_pRoomUserInfos.clear();
	m_vecGamesInfos.clear();
}
//------------------------------------------------------------
CFKGameManager::~CFKGameManager()
{
	CC_SAFE_DELETE( m_pUserInfo );
	CC_SAFE_RELEASE_NULL( m_pServerList );
	CC_SAFE_DELETE( m_pTablesList );
}
//------------------------------------------------------------
CFKGameManager*	CFKGameManager::GetInstancePtr()
{
	if( g_pGameMgr == NULL )
	{
		g_pGameMgr = new CFKGameManager();
		g_pGameMgr->Init();
	}
	return g_pGameMgr;
}
//------------------------------------------------------------
CFKGameManager&	CFKGameManager::GetInstance()
{
	return *CFKGameManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKGameManager::Init()
{
	return true;
}
//------------------------------------------------------------
void CFKGameManager::Clear()
{
	if( m_pUserInfo )
	{
		CC_SAFE_DELETE( m_pUserInfo );
	}
	if( m_pServerList )
	{
		m_pServerList->removeAllObjects();
		CC_SAFE_RELEASE_NULL( m_pServerList );
	}
	if( m_pTablesList )
	{
		CC_SAFE_DELETE( m_pTablesList );
	}
	m_vecGamesInfos.clear();
	m_pRoomUserInfos.clear();
	FK_DEBUG_LOG("CFKGameManager clear data");
}
//------------------------------------------------------------
void CFKGameManager::OnLoginIn(SMsgBuffer *buffer, SDataUser user)
{
	if(m_pUserInfo == NULL)
	{
		m_pUserInfo = new SFKUserLoginInfo();
		memset(m_pUserInfo, 0, sizeof(SFKUserLoginInfo));
	}
	if(m_pServerList == NULL)
	{
		m_pServerList = CCArray::create();
		CC_SAFE_RETAIN(m_pServerList);
	}

	CMD_L2C_LoginAccountResult *pak = (CMD_L2C_LoginAccountResult*)(buffer->GetBuffer());
	WORD wDataSize = buffer->GetMsgSize() - sizeof(SMsgBase);

	m_pUserInfo->wFaceID		= pak->tagUserLoginInfo.wFaceID;
	m_pUserInfo->cbGender		= pak->tagUserLoginInfo.cbGender;
	m_pUserInfo->dwUserID		= pak->tagUserLoginInfo.dwUserID;
	m_pUserInfo->cbMember		= pak->tagUserLoginInfo.cbMember;
	m_pUserInfo->llUserScore	= pak->tagUserLoginInfo.llUserScore;
	m_pUserInfo->dwExperience	= pak->tagUserLoginInfo.dwExperience;

	strcpy(m_pUserInfo->szNickName,		pak->tagUserLoginInfo.szNickName);
	strcpy(m_pUserInfo->szAccount,		pak->tagUserLoginInfo.szAccount);
	strcpy(m_pUserInfo->szMachineID,	user.m_szMachineID);
	strcpy(m_pUserInfo->szMD5Password,	user.m_szMD5Password);

	/*
	// extra info
	void* pDataBuffer = NULL;
	SExtraData tagExtraData;

	CRecvPacketHelper RecvPacket(pak+1,wDataSize-sizeof(CMD_L2C_LoginAccountResult));
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(tagExtraData);
		if (tagExtraData.wDataDescribe==eExtraDataType_Null) 
			break;
		switch (tagExtraData.wDataDescribe)
		{
			case eExtraDataType_OtherInfo:
			{
				break;
			}
		}
	}
	*/
	FK_DEBUG_LOG("CFKGameManager OnLoginIn");
}
//------------------------------------------------------------
SFKUserTotalInfo* CFKGameManager::GetMyInfo()
{
	if( m_pUserInfo == NULL )
		return NULL;
	std::map<DWORD, SFKUserTotalInfo*>::iterator Ite;
	Ite = m_pRoomUserInfos.find( m_pUserInfo->dwUserID );
	if( Ite != m_pRoomUserInfos.end() )
		return Ite->second;
	return NULL;
}
//------------------------------------------------------------
SFKUserTotalInfo* CFKGameManager::GetUserData(WORD wTable, WORD wChair)
{
	if( wTable == INVALID_TABLE_ID || wChair == INVALID_CHAIR_ID )
		return NULL;
	STableTotalInfo* pTableInfo = m_pTablesList ? m_pTablesList->pTableInfoList + wTable : NULL;
	if( pTableInfo == NULL )
		return NULL;
	return pTableInfo->pUserData[wChair];
}
//------------------------------------------------------------
SFKUserTotalInfo* CFKGameManager::GetUserDataByChairIDInSameTable(WORD wChair)
{
	if( wChair == INVALID_CHAIR_ID )
		return NULL;
	SFKUserTotalInfo* pMyInfo = GetMyInfo();
	if( pMyInfo == NULL )
		return NULL;
	WORD wTable = pMyInfo->wTableID;
	return GetUserData(wTable, wChair);
}
//------------------------------------------------------------
SFKUserTotalInfo* CFKGameManager::GetUserData(DWORD dwUserID)
{
	std::map<DWORD, SFKUserTotalInfo*>::iterator Ite;
	Ite = m_pRoomUserInfos.find( dwUserID );
	if( Ite != m_pRoomUserInfos.end() )
	{
		return Ite->second;
	}
	FK_DEBUG_LOG("CFKGameManager::GetUserData ID=%d failed.", dwUserID);
	return NULL;
}
//------------------------------------------------------------
SFKUserLoginInfo* CFKGameManager::GetUserGlobalInfo()
{
	return m_pUserInfo;
}
//------------------------------------------------------------
bool CFKGameManager::SetMyInfo(SFKUserTotalInfo* pak)
{
	CCAssert(m_pUserInfo && m_pUserInfo->dwUserID == pak->dwUserID,
		"SetMyInfo failed");
	AddRoomUsers(pak);

	FK_DEBUG_LOG("GameManager::SetMyInfo");

	return true;
}
//------------------------------------------------------------
bool CFKGameManager::SetUserInfo(SFKUserTotalInfo *pak)
{
	if( pak->wTableID == INVALID_TABLE_ID ||
		m_pTablesList == NULL)
		return false;

	STableTotalInfo* pTable = m_pTablesList->pTableInfoList + pak->wTableID;
	pTable->SetUserInfo(pak->wChairID, pak);
	return true;
}
//------------------------------------------------------------
bool CFKGameManager::IsAllowAvertCheatMode()
{
	if( m_pTablesList == NULL )
		return false;
	return FKServerRuleMgr::IsAllowAvertCheatMode(m_pTablesList->dwGameRuleType);
}
//------------------------------------------------------------
bool CFKGameManager::AddRoomUsers(SFKUserTotalInfo *pak)
{
	SFKUserTotalInfo* pInfo = new SFKUserTotalInfo;
	memcpy( pInfo, pak, sizeof(SFKUserTotalInfo) );
	m_pRoomUserInfos[pak->dwUserID] = pInfo;
	return true;
}
//------------------------------------------------------------
bool CFKGameManager::DelRoomUsers(DWORD dwUserID)
{
	std::map<DWORD, SFKUserTotalInfo*>::iterator Ite;
	Ite = m_pRoomUserInfos.find(dwUserID);
	if ( Ite != m_pRoomUserInfos.end() )
	{
		m_pRoomUserInfos.erase(Ite);
	}
	return true;
}
//------------------------------------------------------------
void CFKGameManager::ClearRoomUsers()
{
	m_pRoomUserInfos.clear();
}
//------------------------------------------------------------
bool CFKGameManager::AddRooms(SGameServerInfo* pak)
{
	SGameServerInfo tagPak;
	memcpy(&tagPak, pak, sizeof(SGameServerInfo));
	if( !CFKDataCenterManager::GetInstancePtr()->IsKindID(tagPak.wGameKindID) )
		return false;
	m_vecGamesInfos.push_back( tagPak );

	FK_DEBUG_LOG("Add room name=%s port=%d wServerID=%d wSortID=%d", 
		FKStringFunc::UTF162UTF8(pak->szServerName, sizeof(pak->szServerName)).c_str(), 
		pak->wGameServerPort, pak->wGameServerID, pak->wSortID);

	return true;
}
//------------------------------------------------------------
void CFKGameManager::ClearRooms()
{
	m_vecGamesInfos.clear();
}
//------------------------------------------------------------
int	 CFKGameManager::GetGameKindCnt()
{
	return CFKDataCenterManager::GetInstancePtr()->GetGameKindCnt();
}
//------------------------------------------------------------
int	 CFKGameManager::GetRoomCnt(int kind, std::vector<SGameServerInfo>& vrooms,
	unsigned int sortid_start, unsigned int sortid_end)
{
	int nAllCnt = m_vecGamesInfos.size();
	vrooms.clear();
	for( int k = 0; k < nAllCnt; ++k )
	{
		if( kind == m_vecGamesInfos[k].wGameKindID &&
			m_vecGamesInfos[k].wSortID >= sortid_start &&
			m_vecGamesInfos[k].wSortID <= sortid_end )
		{
			vrooms.push_back( m_vecGamesInfos[k] );
		}
	}
	return vrooms.size();
}
//------------------------------------------------------------
bool compareGameInfo(const SGameServerInfo& game1, const SGameServerInfo& game2)
{
	return game1.wSortID < game2.wSortID;
}
//------------------------------------------------------------
void CFKGameManager::SortRooms()
{
	std::sort(m_vecGamesInfos.begin(), m_vecGamesInfos.end(), compareGameInfo );
}
//------------------------------------------------------------
bool CFKGameManager::GetRoom(int index, SGameServerInfo& pak, int kind)
{
	if( index < 0 || m_vecGamesInfos.size() <= (unsigned int)index )
		return false;
	std::vector<SGameServerInfo>	rooms;
	int nSize = m_vecGamesInfos.size();
	for( int k = 0; k < nSize; ++k )
	{
		if( kind == m_vecGamesInfos[k].wGameKindID )
		{
			rooms.push_back( m_vecGamesInfos[k] );
		}
	}
	if( rooms.size() <= (unsigned int)index )
		return false;

	pak = rooms[index];
	return true;
}
//------------------------------------------------------------
void CFKGameManager::ResetReqValue()
{
	m_wReqChairID = INVALID_CHAIR_ID;
	m_wReqTableID = INVALID_TABLE_ID;
}
//------------------------------------------------------------
bool CFKGameManager::OnSearchPos()
{
	FK_DEBUG_LOG("GameManager::OnSearchPos start");

	if (IsAllowAvertCheatMode())
	{
		return true;
	}

	CCAssert(GetMyInfo() != NULL, "OnSearchPos failed, no my info");
	const SFKUserTotalInfo *pUserData = GetMyInfo();

	// check if need to stand up
	if (( pUserData->cbUserStatus == eUserCurrentState_SIT
		|| pUserData->cbUserStatus == eUserCurrentState_LOOKON ) &&
		(pUserData->wTableID != INVALID_TABLE_ID && pUserData->wChairID != INVALID_CHAIR_ID ))
	{
		// stand up
		CFKRoomSocketManager::GetInstancePtr()->SendStandUpPacket();
	}

	bool bSuccess = false;
	WORD wBeginTableID = (INVALID_TABLE_ID == m_wReqTableID)? 0 : ((m_wReqTableID + 1)%20);
	
	SFindTableLimit sFindInfo;
	sFindInfo.bAllNull=false;
	sFindInfo.bFilterPass=true;
	sFindInfo.bNotFull=false;
	sFindInfo.bOneNull=true;
	sFindInfo.bTwoNull=false;
	sFindInfo.wBeginTableID=wBeginTableID;
	sFindInfo.wResultTableID=INVALID_TABLE_ID;
	sFindInfo.wResultChairID=INVALID_CHAIR_ID;
	bSuccess = FindGameTable(sFindInfo);

	WORD wFindTableID=sFindInfo.wResultTableID;

	if (!bSuccess)
	{
		// search only one/two empty tables
		sFindInfo.bAllNull=false;
		sFindInfo.bFilterPass=true;
		sFindInfo.bNotFull=false;
		sFindInfo.bOneNull=true;
		sFindInfo.bTwoNull=true;
		sFindInfo.wBeginTableID=wBeginTableID;
		sFindInfo.wResultTableID=INVALID_TABLE_ID;
		sFindInfo.wResultChairID=INVALID_CHAIR_ID;
		bSuccess=FindGameTable(sFindInfo);
	}

	if (!bSuccess)
	{
		// search all tables without password
		sFindInfo.bAllNull=true;
		sFindInfo.bFilterPass=true;
		sFindInfo.bNotFull=true;
		sFindInfo.bOneNull=true;
		sFindInfo.bTwoNull=true;
		sFindInfo.wBeginTableID=wBeginTableID;
		sFindInfo.wResultTableID=INVALID_TABLE_ID;
		sFindInfo.wResultChairID=INVALID_CHAIR_ID;
		bSuccess=FindGameTable(sFindInfo);
	}

	if (!bSuccess)
	{
		// search all tables, no limit
		sFindInfo.bAllNull=true;
		sFindInfo.bFilterPass=true;
		sFindInfo.bNotFull=true;
		sFindInfo.bOneNull=true;
		sFindInfo.bTwoNull=true;
		sFindInfo.wBeginTableID=0;
		sFindInfo.wResultTableID=INVALID_TABLE_ID;
		sFindInfo.wResultChairID=INVALID_CHAIR_ID;
		bSuccess=FindGameTable(sFindInfo);
	}

	if(!bSuccess)
	{
		// really no free chairs
		return false;
	}

	// is need to stand up
	if(pUserData->cbUserStatus == eUserCurrentState_SIT ||
		pUserData->cbUserStatus == eUserCurrentState_LOOKON )
	{
		// stand up
		CFKRoomSocketManager::GetInstancePtr()->SendStandUpPacket();
	}

	// save
	m_wReqTableID = sFindInfo.wResultTableID;
	m_wReqChairID = sFindInfo.wResultChairID;

	// try to sit down
	CFKRoomSocketManager::GetInstancePtr()->SendSitDownPacket(m_wReqTableID,m_wReqChairID);
	return true;
}
//------------------------------------------------------------
bool CFKGameManager::FindGameTable(SFindTableLimit& tagTableInfo)
{
	if( m_pTablesList == NULL )
		return false;

	tagTableInfo.wResultTableID		= INVALID_TABLE_ID;
	tagTableInfo.wResultChairID		= INVALID_CHAIR_ID;

	WORD		wNullCount			= 0;
	WORD		wChairCount			= m_pTablesList->wChairCount;
	WORD		wMeTableID			= m_wReqTableID;

	STableTotalInfo *pTableState	= NULL;

	// search tables
	WORD wTableCount				= m_pTablesList->wTableCount;
	for (int i=0;i<wTableCount;i++)
	{
		tagTableInfo.wResultTableID	= (tagTableInfo.wBeginTableID+i) % wTableCount;
		if (wMeTableID != tagTableInfo.wResultTableID)
		{
			// get the table
			pTableState	= m_pTablesList->pTableInfoList + tagTableInfo.wResultTableID;
			CCAssert(pTableState!=NULL, "pTableView!=NULL");

			// if is playing
			if (pTableState->bIsPlaying==true) 
				continue;

			// if has password
			bool bTablePass = pTableState->bIsHasPassword;
			if ((tagTableInfo.bFilterPass == true)&&(bTablePass == true)) 
				continue;

			// get empty chairs form table
			wNullCount=pTableState->GetNullChairCount(tagTableInfo.wResultChairID);
			if ((tagTableInfo.bOneNull && (1 == wNullCount)) 
				|| (tagTableInfo.bTwoNull && (2 == wNullCount))
				|| (tagTableInfo.bNotFull && (0 <= wNullCount)) 
				|| (tagTableInfo.bAllNull && (MAX_PLAYER_IN_ROOM == wNullCount)))
			{
				// check other rules
				if ( !CanSitDownTable(tagTableInfo.wResultTableID,tagTableInfo.wResultChairID) ) 
					continue;

				return true;
			}
		}
	}

	// reset value
	tagTableInfo.wResultTableID			= INVALID_TABLE_ID;
	tagTableInfo.wResultChairID			= INVALID_CHAIR_ID;

	return false;
}
//------------------------------------------------------------
WORD CFKGameManager::GetNullChairCount(WORD& wNullChairID, STableTotalInfo *pak)
{
	if (0 == pak->wChairCount)
	{
		return 0;
	}

	WORD	wNullCount	=	0;
	int		startindex	=	0;
	int		nowindex	=	0;
	wNullChairID		=	INVALID_CHAIR_ID;

	for (WORD i=0;i<pak->wChairCount;i++)
	{
		nowindex = (startindex+i)%pak->wChairCount;
		if (pak->pUserData[nowindex] == NULL)
		{
			++wNullCount;
			wNullChairID=nowindex;
		}
	}
	return wNullCount;
}
//------------------------------------------------------------
bool CFKGameManager::CanSitDownTable(WORD wTableID, WORD wChairID)
{
	if( m_pTablesList == NULL )
		return false;
	if (wTableID>= m_pTablesList->wTableCount) 
		return false;
	if (wChairID>= m_pTablesList->wChairCount) 
		return false;

	STableTotalInfo *pTableState = m_pTablesList->pTableInfoList + wTableID;
	if (NULL == pTableState)
	{
		return false;
	}

	SFKUserTotalInfo * pITableUserItem = pTableState->pUserData[wChairID];
	if (pITableUserItem!=NULL && pITableUserItem->dwUserID > 0) 
		return false;	
	if (pTableState->bIsPlaying)
	{	
		return false;
	}

	return true;
}
//------------------------------------------------------------