/********************************************************************
	created:	2016/07/16
	filename: 	FKNodePanel
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "../UI/FKCW_UIWidget.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKNodePanel : public FKCW_UIWidget_Layout
{
public:
	CFKNodePanel(void);
	CREATE_FUNC(CFKNodePanel);
	static CFKNodePanel* create(int p_nExType);

public:
	virtual bool init();
	virtual bool init(int p_nExType);

protected:
	virtual bool addLCC();
	virtual bool drawView();

public:
	virtual void onClick(CCObject* pSender);
	virtual void onSocketEvent(CCObject* obj);
	virtual void showPanel(bool bShow);
	virtual void setCloseBtnPos(CCPoint pt, bool bShow = true);

	CC_SYNTHESIZE(int, m_nExType, ExType);
};
