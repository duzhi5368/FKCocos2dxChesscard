#include "FKNetMsg.h"
#include "CMD_Login.h"
#include "../Socket/FKSocketManager.h"
#include "../Base/FKStringFunc.h"
//------------------------------------------------------------
void FKNetMsg::Send_C2L_UserLoginRequest(SDataUser user, int nPlazaVer, std::string strDeviceID )
{
	CMD_C2L_UserLoginRequest msg;
	msg.dwPlazaVersion	= nPlazaVer;
	msg.cbValidateFlags = CMD_C2L_UserLoginRequest::DEFAULT_VALIDATA_FLAGS;

	strcpy(user.m_szMachineID, strDeviceID.c_str());
	FKStringFunc::UTF82UTF16(user.m_szMD5Password, sizeof(msg.szPassword), msg.szPassword);
	FKStringFunc::UTF82UTF16(user.m_strAccount.c_str(), sizeof(msg.szAccounts), msg.szAccounts);
	FKStringFunc::UTF82UTF16(user.m_szMachineID, sizeof(msg.szMachineID), msg.szMachineID);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	msg.cbPlazaType = CMD_C2L_UserLoginRequest::ePlazaType_iOS;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	msg.cbPlazaType = CMD_C2L_UserLoginRequest::ePlazaType_Android;
#else
	msg.cbPlazaType = CMD_C2L_UserLoginRequest::ePlazaType_PC;
#endif

	CFKSocketManager::GetInstancePtr()->SendData( eMainMsgType_Login, eLoginMsgType_C2L_UserLoginRequest, sizeof(msg), &msg);
}
//------------------------------------------------------------
void FKNetMsg::Send_C2L_RegisterAccountRequest( SDataUser user, 
	int nPlazaVer, char* szMd5SubPsw, std::string strNickName )
{
	CMD_C2L_RegisterAccountRequest pRegisterAccounts;
	pRegisterAccounts.dwPlazaVersion = nPlazaVer;
	FKStringFunc::UTF82UTF16(user.m_szMD5Password, 
		sizeof(pRegisterAccounts.szPassword), pRegisterAccounts.szPassword);

	FKStringFunc::UTF82UTF16(szMd5SubPsw, sizeof(pRegisterAccounts.szSubPassword), pRegisterAccounts.szSubPassword);
	FKStringFunc::UTF82UTF16(user.m_szMachineID, sizeof(pRegisterAccounts.szMachineID), pRegisterAccounts.szMachineID);
#if (CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM)
	FKStringFunc::UTF82UTF16(FKStringFunc::UTF82GBK(user.m_strAccount.c_str()).c_str(), sizeof(pRegisterAccounts.szAccounts), pRegisterAccounts.szAccounts);
	FKStringFunc::UTF82UTF16(FKStringFunc::UTF82GBK(strNickName.c_str()).c_str(), sizeof(pRegisterAccounts.szNickName), pRegisterAccounts.szNickName);
#else
	FKStringFunc::UTF82UTF16(user.m_strAccount.c_str(), sizeof(pRegisterAccounts.szAccounts), pRegisterAccounts.szAccounts);
	FKStringFunc::UTF82UTF16(strNickName.c_str(), sizeof(pRegisterAccounts.szNickName), pRegisterAccounts.szNickName);
#endif

	CFKSocketManager::GetInstancePtr()->SendData(eMainMsgType_Login, eLoginMsgType_C2L_RegisterAccountRequest, sizeof(CMD_C2L_RegisterAccountRequest), &pRegisterAccounts);
}
//------------------------------------------------------------