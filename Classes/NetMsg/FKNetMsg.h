/********************************************************************
	created:	2016/07/19
	filename: 	FKNetMsg
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Data/FKUserData.h"
//------------------------------------------------------------
namespace FKNetMsg
{
	void		Send_C2L_UserLoginRequest(SDataUser user, 
		int nPlazaVer, std::string strDeviceID );
	void		Send_C2L_RegisterAccountRequest(SDataUser user, 
		int nPlazaVer, char* szMd5SubPsw, std::string strNickName);
}
