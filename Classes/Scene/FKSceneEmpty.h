/********************************************************************
	created:	2016/07/16
	filename: 	FKSceneEmpty
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKSceneBase.h"
#include "../UI/FKCW_UIWidget.h"
//------------------------------------------------------------
class CFKSceneEmpty : public CFKSceneBase
{
public:
	CFKSceneEmpty();
	virtual ~CFKSceneEmpty();
public:
	virtual void				RunThisScene();
	virtual enumSceneID			GetSceneID();
private:
	FKCW_UIWidget_Label*		m_pInfoLabel;
};
