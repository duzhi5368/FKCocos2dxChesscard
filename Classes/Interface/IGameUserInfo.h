/********************************************************************
	created:	2016/08/02
	filename: 	IGameUserInfo
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Base/FKTypeDefine.h"
#include "../Base/FKUserTotalInfo.h"
//------------------------------------------------------------
class IFKGameUserInfo 
{
public:
	virtual DWORD				GetUserID()		const = 0;
	virtual std::string			GetNickName()	const = 0;
	virtual WORD				GetTableID()	const = 0;
	virtual WORD				GetChairID()	const = 0;
	virtual Bytes				GetUserStatus() const = 0;
	virtual LONGLONG			GetUserScore()	const = 0;
	virtual SFKUserTotalInfo*	GetUserInfo()	= 0;
};
