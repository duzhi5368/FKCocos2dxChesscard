/********************************************************************
	created:	2016/07/13
	filename: 	FKSceneLogo
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKSceneBase.h"
#include <vector>
#include "../UI/FKCW_UIWidget.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKSceneLogo : public CFKSceneBase
{
public:
	CFKSceneLogo();
	virtual ~CFKSceneLogo();
public:
	void						onBeginLoad();
	void						onProgress(CCObject *sender);
	void						onComplete();
public:
	virtual void				RunThisScene();
	virtual enumSceneID			GetSceneID();
private:
	FKCW_UIWidget_Label*		m_pLoadingLabel;
	int							m_nImageLoaded;
	int							m_nImageTotal;
	std::vector<std::string>	m_strResArray;
};