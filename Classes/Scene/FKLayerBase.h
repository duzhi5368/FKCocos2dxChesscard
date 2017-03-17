/********************************************************************
	created:	2016/07/16
	filename: 	FKLayerBase
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKSceneDefine.h"
#include "../UI/FKCW_UIWidget.h"
#include "../Wrapper/FKSpriteBase.h"
#include <vector>
//------------------------------------------------------------
USING_NS_CC;
using std::vector;
//------------------------------------------------------------
class CFKLayerBase : public CCLayer
{
public:
	CREATE_FUNC(CFKLayerBase);
	~CFKLayerBase(void);

public:
	virtual bool	init();
	virtual void	onEnter();
	virtual void	onExit();

	virtual void	onSocketEvent(CCObject* obj);
	virtual void	onMessageBox(CCObject* obj);
public:
	void			SetViewBack(CCSprite* p_pBackGround, enumLayerViewStyle p_eViewStyle = eLayerViewStyle_Normal);

private:
	CFKSpriteBase			m_ViewBack;
	vector<CFKSpriteBase>	m_vecGUI;

protected:
	FKCW_UIWidget_WidgetWindow*		m_pLayout;
};
