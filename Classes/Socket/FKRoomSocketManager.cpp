#include "FKRoomSocketManager.h"
//------------------------------------------------------------
static CFKRoomSocketManager* g_pRoomSocketMgr = NULL;
//------------------------------------------------------------
CFKRoomSocketManager::CFKRoomSocketManager()
{

}
//------------------------------------------------------------
CFKRoomSocketManager::~CFKRoomSocketManager()
{

}
//------------------------------------------------------------
CFKRoomSocketManager* CFKRoomSocketManager::GetInstancePtr()
{
	if( g_pRoomSocketMgr == NULL )
	{
		g_pRoomSocketMgr = new CFKRoomSocketManager();
		g_pRoomSocketMgr->Init();
	}
	return g_pRoomSocketMgr;
}
//------------------------------------------------------------
CFKRoomSocketManager& CFKRoomSocketManager::GetInstance()
{
	return *CFKRoomSocketManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKRoomSocketManager::Init()
{
	return true;
}
//------------------------------------------------------------
void CFKRoomSocketManager::Update(float dt)
{

}
//------------------------------------------------------------
bool CFKRoomSocketManager::SendStandUpPacket()
{
	return true;
}
//------------------------------------------------------------
bool CFKRoomSocketManager::SendSitDownPacket(WORD wTableID, WORD wChairID, std::string pszTablePass )
{
	return true;
}
//------------------------------------------------------------