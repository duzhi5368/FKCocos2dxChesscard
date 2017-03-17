/********************************************************************
	created:	2016/07/13
	filename: 	FKViewManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKViewManager
{
public:
	CFKViewManager();
	~CFKViewManager();
public:
	static CFKViewManager*	GetInstancePtr();
	static CFKViewManager&	GetInstance();
public:
	bool					Init();
public:
	CCSize					GetShowSize();
	float					GetShowScale(CCSize szSmall, CCSize szBig);
	CCPoint					GetXYScale(CCSize szSmall);

	float					GetDefaultShowWidth();
	float					GetDefaultShowHeight();

	bool					SetSpriteSuitView(CCSprite* pSprite, bool bRotate = false);
	bool					SetSpriteFullView(CCSprite* pSprite);
	bool					SetSpriteXSuitView(CCSprite* pSprite);
	bool					SetSpriteYSuitView(CCSprite* pSprite);
};
