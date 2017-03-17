#include "FKLayerBase.h"
#include "../Base/FKBaseMacros.h"
#include "../Base/FKVisiableRect.h"
#include "../Mgr/FKViewManager.h"
#include "../Event/FKEventMacros.h"
#include "../Panel/FKPanelManager.h"
#include "../Socket/FKSocketManager.h"
#include "../Socket/FKRoomSocketManager.h"
//------------------------------------------------------------
CFKLayerBase::~CFKLayerBase(void)
{
	FK_DEBUG_LOG("LayerBase release");
}
//------------------------------------------------------------
bool CFKLayerBase::init()
{
	if(!CCLayer::init())
	{
		return false;
	}

	m_pLayout = FKCW_UIWidget_WidgetWindow::create();
	this->addChild(m_pLayout);

	this->setTouchEnabled(true);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->setKeypadEnabled( true );
#endif

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
		schedule_selector(CFKSocketManager::Update), 
		&CFKSocketManager::GetInstance(), 0.001f, false);
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
		schedule_selector(CFKRoomSocketManager::Update), 
		&CFKRoomSocketManager::GetInstance(), 0.001f, false);

	return true;
}
//------------------------------------------------------------
void CFKLayerBase::SetViewBack(CCSprite* p_pBackGround, enumLayerViewStyle p_eViewStyle )
{
	if (NULL == p_pBackGround)
	{
		return;
	}

	switch (p_eViewStyle)
	{
	case eLayerViewStyle_Normal:
		{
			CFKViewManager::GetInstancePtr()->SetSpriteSuitView(p_pBackGround);
			p_pBackGround->setPosition(CFKVisiableRect::Center());
			m_pLayout->addChild(p_pBackGround);

		}
		break;
	case eLayerViewStyle_X:
		{
			CFKViewManager::GetInstancePtr()->SetSpriteXSuitView(p_pBackGround);
			p_pBackGround->setPosition(CFKVisiableRect::Center());
			m_pLayout->addChild(p_pBackGround);

		}
		break;
	case eLayerViewStyle_Y:
		{
			CFKViewManager::GetInstancePtr()->SetSpriteYSuitView(p_pBackGround);
			p_pBackGround->setPosition(CFKVisiableRect::Center());
			m_pLayout->addChild(p_pBackGround);
		}
		break;
	case eLayerViewStyle_XY:
	case eLayerViewStyle_Full:
		{
			CFKViewManager::GetInstancePtr()->SetSpriteFullView(p_pBackGround);
			p_pBackGround->setPosition(CFKVisiableRect::Center());
			m_pLayout->addChild(p_pBackGround);
		}
		break;
	default:
		{
			FK_DEBUG_LOG("SetViewBack ViewStyle Error");
		}
		break;
	}
}
//------------------------------------------------------------
void CFKLayerBase::onEnter()
{
	CCLayer::onEnter();

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, 
		callfuncO_selector(CFKLayerBase::onSocketEvent), EVENT_SOCKET, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, 
		callfuncO_selector(CFKLayerBase::onMessageBox), EVENT_MSGBOX, NULL);
}
//------------------------------------------------------------
void CFKLayerBase::onExit()
{
	CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}
//------------------------------------------------------------
void CFKLayerBase::onSocketEvent(CCObject* obj)
{

}
//------------------------------------------------------------
void CFKLayerBase::onMessageBox(CCObject* obj)
{
	SBoxPanelInfo* pak = (SBoxPanelInfo*)obj;
	enumBoxOperateID boxopid = pak->m_eOperateID;
	switch (boxopid)
	{
	case eBoxOperateIDNull:
		break;
	case eBoxOperateIDShow:
		{
			CCNode *pNode = this->getChildByTag(FK_BOX_PANEL_TAG);
			if (NULL == pNode)
			{
				pNode = CFKBoxPanel::createBox(pak->m_tagShow.m_eType, pak->m_tagShow.m_strTitle.c_str(),
					pak->m_tagShow.m_strText.c_str(), pak->m_tagShow.m_bIsShowCancel);
				pNode->setPosition(CFKVisiableRect::Center());
				m_pLayout->addChild(pNode, eLayerZOrderTypeMax + 1, FK_BOX_PANEL_TAG);
			}
			else
			{
				((CFKBoxPanel*)pNode)->init(pak->m_tagShow.m_eType, pak->m_tagShow.m_strTitle.c_str(), 
					pak->m_tagShow.m_strText.c_str(), pak->m_tagShow.m_bIsShowCancel);
				((CFKBoxPanel*)pNode)->showPanel(true);
			}
		}
		break;
	case eBoxOperateIDHide:
		{
			CCNode *pNode = this->getChildByTag(FK_BOX_PANEL_TAG);
			if (NULL != pNode)
			{
				((CFKBoxPanel*)pNode)->showPanel(false);
			}
		}
		break;
	case eBoxOperateIDClick:
		break;
	default:
		break;
	}
	FK_DEBUG_LOG("LayerBase::onMessageBox Click type = %d", pak->m_tagClick.m_eType);
}
//------------------------------------------------------------