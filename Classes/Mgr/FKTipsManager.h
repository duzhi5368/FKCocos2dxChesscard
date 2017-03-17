/********************************************************************
	created:	2016/07/19
	filename: 	FKTipsManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "cocos-ext.h"
//------------------------------------------------------------
USING_NS_CC;
USING_NS_CC_EXT;
//------------------------------------------------------------
class CFKTipsManager : public CCObject
{
public:
	CFKTipsManager();
	~CFKTipsManager();
public:
	static CFKTipsManager*	GetInstancePtr();
	static CFKTipsManager&	GetInstance();
public:
	bool					Init();
	void					Clear();
public:
	void					SetScene(CCNode* p_pNode);
	void					PushMsg(const char* p_szNickName, const char* p_szChatString, unsigned int m_unMaxLen = 120 );
	CCArray*				GetHistory(){ return m_pHistory; }
private:
	void					OnCheckTips();
	void					OnOverShow();
private:
	CCArray*				m_pMsgList;
	CCArray*				m_pHistory;
	CCNode*					m_pViewNode;
	bool					m_bIsShow;
};
