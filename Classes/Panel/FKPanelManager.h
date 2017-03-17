/********************************************************************
	created:	2016/07/16
	filename: 	FKPanelManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKNodePanel.h"
#include "FKBoxPanel.h"
#include "FKPanelMacros.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKPanelManager
{
public:
	CFKPanelManager();
	~CFKPanelManager();
public:
	static CFKPanelManager*	GetInstancePtr();
	static CFKPanelManager&	GetInstance();
public:
	bool			Init();
public:
	CFKNodePanel*	CreatePanel(int p_nPanelTag, int p_nExType = -1);
	CFKNodePanel*	ShowPanel(CCNode* p_pParentNode, int p_nPanelID, int p_nExType = -1);
	void			HidePanel(CCNode* p_pParentNode, int p_nPanelID);
};
