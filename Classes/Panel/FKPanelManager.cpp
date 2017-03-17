#include "FKPanelManager.h"
#include "../Base/FKVisiableRect.h"
#include "../Scene/FKSceneDefine.h"
//------------------------------------------------------------
static CFKPanelManager* g_pPanelMgr = NULL;
//------------------------------------------------------------
CFKPanelManager::CFKPanelManager()
{

}
//------------------------------------------------------------
CFKPanelManager::~CFKPanelManager()
{

}
//------------------------------------------------------------
CFKPanelManager* CFKPanelManager::GetInstancePtr()
{
	if( g_pPanelMgr == NULL )
	{
		g_pPanelMgr = new CFKPanelManager();
		g_pPanelMgr->Init();
	}
	return g_pPanelMgr;
}
//------------------------------------------------------------
CFKPanelManager& CFKPanelManager::GetInstance()
{
	return *CFKPanelManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKPanelManager::Init()
{
	return true;
}
//------------------------------------------------------------
CFKNodePanel* CFKPanelManager::CreatePanel(int p_nPanelTag, int p_nExType)
{
	switch (p_nPanelTag)
	{
	case FK_BOX_PANEL_TAG:
		{
			return CFKBoxPanel::create(p_nExType);
		}
		break;
	default:
		{
			return CFKNodePanel::create(p_nExType);
		}
		break;
	}

	return CFKNodePanel::create(p_nExType);
}
//------------------------------------------------------------
CFKNodePanel* CFKPanelManager::ShowPanel(CCNode* p_pParentNode, int p_nPanelID, int p_nExType)
{
	CCNode *pNode = p_pParentNode->getChildByTag(p_nPanelID);
	if (NULL == pNode)
	{
		pNode = CreatePanel(p_nPanelID, p_nExType);
		pNode->setPosition(CFKVisiableRect::Center());
		p_pParentNode->addChild(pNode, eLayerZOrderTypeMax, p_nPanelID);
	}
	else
	{
		((CFKNodePanel*)pNode)->setExType(p_nExType);
		((CFKNodePanel*)pNode)->showPanel(true);
	}

	return (CFKNodePanel*)pNode;
}
//------------------------------------------------------------
void CFKPanelManager::HidePanel(CCNode* p_pParentNode, int p_nPanelID)
{
	CCNode *pNode = p_pParentNode->getChildByTag(p_nPanelID);
	if (pNode)
	{
		((CFKNodePanel*)pNode)->showPanel(false);
	}
}
//------------------------------------------------------------