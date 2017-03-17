#include "FKPlatformInterfaceManager.h"
//------------------------------------------------------------
static CFKPlatformInterfaceManager* g_pInterfaceMgr = NULL;
//------------------------------------------------------------
CFKPlatformInterfaceManager::CFKPlatformInterfaceManager()
{

}
//------------------------------------------------------------
CFKPlatformInterfaceManager::~CFKPlatformInterfaceManager()
{

}
//------------------------------------------------------------
CFKPlatformInterfaceManager* CFKPlatformInterfaceManager::GetInstancePtr()
{
	if( g_pInterfaceMgr == NULL )
	{
		g_pInterfaceMgr = new CFKPlatformInterfaceManager();
		g_pInterfaceMgr->Init();
	}
	return g_pInterfaceMgr;
}
//------------------------------------------------------------
CFKPlatformInterfaceManager& CFKPlatformInterfaceManager::GetInstance()
{
	return *CFKPlatformInterfaceManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKPlatformInterfaceManager::Init()
{
	return true;
}
//------------------------------------------------------------
std::string CFKPlatformInterfaceManager::GetDeviceID()
{
	return "TEST1111-2222-3333-4444-5555FKFK";
}
//------------------------------------------------------------
std::string CFKPlatformInterfaceManager::GetDeviceModel()
{
	return "PC";
}
//------------------------------------------------------------
std::string	CFKPlatformInterfaceManager::GetUserName()
{
	return "FreeKnight";
}
//------------------------------------------------------------
std::string	CFKPlatformInterfaceManager::GetMacAddress()
{
	return "0";
}
//------------------------------------------------------------
void CFKPlatformInterfaceManager::DoVibrator(int p_nTime)
{

}
//------------------------------------------------------------
void CFKPlatformInterfaceManager::SetWakeLock(long p_lTimeOut)
{

}
//------------------------------------------------------------
void CFKPlatformInterfaceManager::ShowExitDialog(const char* p_szTitle, const char* p_szMsg)
{

}
//------------------------------------------------------------
void CFKPlatformInterfaceManager::OpenUrl(const char* p_szUrl)
{
	ShellExecuteA(NULL, "open", p_szUrl, NULL, NULL, SW_SHOWNORMAL);
}
//------------------------------------------------------------