#include "FKNodePanel.h"
#include "FKNodePanelRes.h"
#include "FKPanelMacros.h"
#include "../Base/FKVisiableRect.h"
#include "../Scene/FKSceneDefine.h"
//------------------------------------------------------------
CFKNodePanel::CFKNodePanel(void)
	:m_nExType(-1)
{
}
//------------------------------------------------------------
bool CFKNodePanel::init()
{
	if (false == FKCW_UIWidget_Layout::init())
	{
		return false;
	}

	CCRect rcWin = CFKVisiableRect::GetVisibleRect();
	setContentSize(CCSize(rcWin.size.width + 2*rcWin.origin.x, rcWin.size.height + 2*rcWin.origin.y));

	drawView();
	addLCC();

	return true;
}
//------------------------------------------------------------
bool CFKNodePanel::init(int nExType)
{
	m_nExType = nExType;
	return init();
}
//------------------------------------------------------------
bool CFKNodePanel::drawView()
{
	// background
	FKCW_UIWidget_ColorView* pPanel = FKCW_UIWidget_ColorView::create();
	pPanel->setColor(FK_DEFAULT_NODEPANEL_BACKGROUND_COLOR);
	pPanel->setPosition(CFKVisiableRect::Center());
	pPanel->setContentSize(CFKVisiableRect::GetVisibleRect().size);
	pPanel->setOpacity(150);
	pPanel->setTouchEnabled(true);
	addChild(pPanel);

	return true;
}
//------------------------------------------------------------
bool CFKNodePanel::addLCC()
{
	// buttons
	FKCW_UIWidget_Button *pBtnBack = FKCW_UIWidget_Button::createWithSpriteFrameName( 
		Res_Image_CN_NodePanel_CloseBtn_Normal, Res_Image_CN_NodePanel_CloseBtn_Down);
	pBtnBack->setOnClickListener(this, fkcw_uiwidget_click_selector(CFKNodePanel::onClick));
	pBtnBack->setPosition(CFKVisiableRect::Center());
	pBtnBack->setUserTag(FK_NODE_PANEL_RETUEN_BTN_TAG);
	addChild(pBtnBack, eLayerZOrderTypeMax, FK_NODE_PANEL_RETUEN_BTN_TAG);

	return true;
}
//------------------------------------------------------------
void CFKNodePanel::onClick(CCObject* pSender)
{
	FKCW_UIWidget_Button* pBtn = (FKCW_UIWidget_Button*)pSender;
	int nBtnID = pBtn->getUserTag();

	switch(nBtnID)
	{
	case FK_NODE_PANEL_RETUEN_BTN_TAG:
		{
			showPanel(false);
		}
		break;
	default:
		break;
	}
}
//------------------------------------------------------------
void CFKNodePanel::onSocketEvent(CCObject* obj)
{

}
//------------------------------------------------------------
void CFKNodePanel::showPanel(bool bShow)
{
	this->setVisible(bShow);
}
//------------------------------------------------------------
void CFKNodePanel::setCloseBtnPos(CCPoint pt, bool bShow)
{
	FKCW_UIWidget_Button *pBtnBack = (FKCW_UIWidget_Button *)getChildByTag(FK_NODE_PANEL_RETUEN_BTN_TAG);
	if (pBtnBack)
	{
		pBtnBack->setPosition(pt);
		pBtnBack->setVisible(bShow);
	}
}
//------------------------------------------------------------
CFKNodePanel* CFKNodePanel::create(int p_nExType)
{
	CFKNodePanel* pRet = new CFKNodePanel();
	if (pRet && pRet->init(p_nExType))
	{
		pRet->autorelease(); 
		return pRet;
	}
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}
//------------------------------------------------------------