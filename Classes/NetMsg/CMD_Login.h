/********************************************************************
	created:	2016/07/19
	filename: 	CMD_Login
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Base/FKTypeDefine.h"
#include "CMD_MsgDefine.h"
#include "../Data/FKUserTotalInfo.h"
#include "../Data/FKUserData.h"
//------------------------------------------------------------
enum ENUM_LoginMsgType
{
	eLoginMsgType_Unknown = 0,

	eLoginMsgType_C2L_UserLoginRequest,
	eLoginMsgType_C2L_RegisterAccountRequest,
	eLoginMsgType_L2C_LoginAccountResult,

	eLoginMsgType_Max,
};
//------------------------------------------------------------
// eMainMsgType_Login eLoginMsgType_C2L_UserLoginRequest
struct CMD_C2L_UserLoginRequest : public SMsgBase
{
	const static unsigned char DEFAULT_VALIDATA_FLAGS	= 10;
	enum ENUM_PlazaType
	{
		ePlazaType_PC = 0,
		ePlazaType_Android,
		ePlazaType_iOS,
	};
	// system info
	unsigned long					dwPlazaVersion;							// plaza version
	unsigned short					szMachineID[MAX_USER_MACHINE_LENGTH];	// user machine id

	// user login info
	unsigned short					szPassword[USER_PASSWORD_MD5_LENGTH];	// user password
	unsigned short					szAccounts[MAX_USER_NAME_LENGTH];		// user name
	unsigned char					cbValidateFlags;						// validate code
	unsigned char					cbPlazaType;							// see ENUM_PlazaType

	CMD_C2L_UserLoginRequest(){
		memset(this, 0, sizeof(CMD_C2L_UserLoginRequest));
	}
};
//------------------------------------------------------------
// eMainMsgType_Login eLoginMsgType_C2L_RegisterAccountRequest
struct CMD_C2L_RegisterAccountRequest : public SMsgBase
{
	enum ENUM_PlazaType
	{
		ePlazaType_PC = 0,
		ePlazaType_Android,
		ePlazaType_iOS,
	};
	// system info
	unsigned long					dwPlazaVersion;							// plaza version
	unsigned short					szMachineID[MAX_USER_MACHINE_LENGTH];	// user machine id

	// user login info
	unsigned short					szPassword[USER_PASSWORD_MD5_LENGTH];	// user password
	unsigned short					szSubPassword[USER_PASSWORD_MD5_LENGTH];// user insured password
	unsigned short					szAccounts[MAX_USER_NAME_LENGTH];		// user name
	unsigned short					szNickName[MAX_USER_NICKNAME_LENGTH];	// user nickname


	CMD_C2L_RegisterAccountRequest(){
		memset(this, 0, sizeof(CMD_C2L_RegisterAccountRequest));
	}
};
//------------------------------------------------------------
// eMainMsgType_Login eLoginMsgType_L2C_LoginAccountResult
struct CMD_L2C_LoginAccountResult : public SMsgBase
{
	enum ENUM_LoginResult
	{
		eLoginResult_Failed		= 0,
		eLoginResult_Success	= 1,
		eLoginResult_NeedUpdate	= 2,
	};
	unsigned char					ucLoginResult;			// ENUM_LoginResult
	unsigned int					unUpdateVersion;		// server suggest version

	// user login info
	SFKUserLoginInfo				tagUserLoginInfo;		// User login info
};
//------------------------------------------------------------