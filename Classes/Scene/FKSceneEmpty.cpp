#include "FKSceneEmpty.h"
#include "../Base/FKStringFunc.h"
#include "../Base/FKVisiableRect.h"
#include "FKSceneEmptyRes.h"
//------------------------------------------------------------
CFKSceneEmpty::CFKSceneEmpty()
	: m_pInfoLabel( NULL )
{

}
//------------------------------------------------------------
CFKSceneEmpty::~CFKSceneEmpty()
{

}
//------------------------------------------------------------
void CFKSceneEmpty::RunThisScene()
{
	m_pInfoLabel = FKCW_UIWidget_Label::create(FK_UTF8( Res_String_CN_SceneEmpty_Info ), "", 36);
	m_pInfoLabel->setColor(ccWHITE);
	m_pInfoLabel->setPosition(ccp(CFKVisiableRect::Bottom().x, CFKVisiableRect::Bottom().y + 80));
	this->addChild(m_pInfoLabel);
}
//------------------------------------------------------------
enumSceneID CFKSceneEmpty::GetSceneID()
{
	return eSceneID_Unknown;
}
//------------------------------------------------------------