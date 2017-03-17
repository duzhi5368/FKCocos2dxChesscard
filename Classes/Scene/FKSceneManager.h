/********************************************************************
	created:	2016/07/13
	filename: 	FKSceneManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKSceneDefine.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKSceneManager
{
public:
	CFKSceneManager();
	~CFKSceneManager();
public:
	static CFKSceneManager*	GetInstancePtr();
	static CFKSceneManager&	GetInstance();
public:
	bool					Init( enumSceneID p_eStartScene = START_SCENE_ID );
	bool					ChangeScene(enumSceneID p_eFrom, enumSceneID p_eTo, enumSceneTransType p_eTransType);
	bool					GetSceneRes(enumSceneID id, std::vector<std::string>& p_vOutRes, unsigned short& p_usNeedToReleaseCount);
};