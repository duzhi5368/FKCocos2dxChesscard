/********************************************************************
	created:	2016/07/13
	filename: 	FKSceneBase
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
class CFKSceneBase : public CCScene
{
public:
	virtual void		RunThisScene() = 0;
	virtual enumSceneID GetSceneID() = 0;

	virtual bool		Init(){ return true; };
	virtual bool		ExitRelease(){ return true; };
};
//------------------------------------------------------------