/********************************************************************
	created:	2016/07/16
	filename: 	FKSceneLogin
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKSceneBase.h"
#include "../UI/FKCW_UIWidget.h"
//------------------------------------------------------------
class CFKSceneLogin : public CFKSceneBase
{
public:
	CFKSceneLogin();
	virtual ~CFKSceneLogin();
public:
	virtual void				RunThisScene();
	virtual enumSceneID			GetSceneID();
};
