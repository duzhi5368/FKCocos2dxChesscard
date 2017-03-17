/********************************************************************
	created:	2016/07/21
	filename: 	FKUserTotalInfo
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Base/FKTypeDefine.h"
//------------------------------------------------------------
enum ENUM_UserCurrentState
{
	eUserCurrentState_Null		=	0x00,
	eUserCurrentState_FREE		=	0x01,
	eUserCurrentState_SIT		=	0x02,
	eUserCurrentState_READY		=	0x03,
	eUserCurrentState_LOOKON	=	0x04,
	eUserCurrentState_PLAYING	=	0x05,
	eUserCurrentState_OFFLINE	=	0x06,
};
#pragma pack(1)
struct SFKUserLoginInfo
{
	WORD								wFaceID;								// user face	
	BYTE								cbGender;								// user gender	male/female
	BYTE								cbMember;								// user vip level
	DWORD								dwUserID;								// user id
	DWORD								dwExperience;							// user experience
	LONGLONG							llUserScore;							// user gold
	char								szNickName[MAX_USER_NICKNAME_LENGTH];	// user nickname
	char								szAccount[MAX_USER_NAME_LENGTH];
	char								szMachineID[MAX_USER_MACHINE_LENGTH];
	char								szMD5Password[USER_PASSWORD_MD5_LENGTH];

	SFKUserLoginInfo()
	{
		wFaceID = 0;
		cbGender = 0;
		cbMember = 0;
		dwUserID = 0;
		dwExperience = 0;
		llUserScore = 0;
		memset( szNickName, 0, MAX_USER_NICKNAME_LENGTH );
		memset( szAccount, 0, MAX_USER_NAME_LENGTH);
		memset( szMachineID, 0, MAX_USER_MACHINE_LENGTH );
		memset( szMD5Password, 0, USER_PASSWORD_MD5_LENGTH );
	};
};

struct SFKUserTotalInfo : public SFKUserLoginInfo
{
	WORD								wGameServerID;
	WORD								wTableID;
	WORD								wChairID;
	BYTE								cbUserStatus;		// User current state SEE ENUM_UserCurrentState

	SFKUserTotalInfo()
	{
		wGameServerID	= 0;
		wTableID		= 0;
		wChairID		= 0;
	}
};

#pragma pack()