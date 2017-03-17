#include "FKTipsManager.h"
#include "FKTipsManagerRes.h"
#include "../Base/FKStringFunc.h"
#include "../Base/FKBaseMacros.h"
#include "../Base/FKVisiableRect.h"
//------------------------------------------------------------
static CFKTipsManager* s_pTipsMgr = NULL;
//------------------------------------------------------------
CFKTipsManager::CFKTipsManager()
	: m_pMsgList( NULL )
	, m_pHistory( NULL )
	, m_pViewNode( NULL )
	, m_bIsShow( false )
{

}
//------------------------------------------------------------
CFKTipsManager::~CFKTipsManager()
{
	CC_SAFE_RELEASE_NULL( m_pMsgList );
	CC_SAFE_RELEASE_NULL( m_pHistory );
}
//------------------------------------------------------------
CFKTipsManager*	CFKTipsManager::GetInstancePtr()
{
	if( s_pTipsMgr == NULL )
	{
		s_pTipsMgr = new CFKTipsManager();
		s_pTipsMgr->Init();
	}
	return s_pTipsMgr;
}
//------------------------------------------------------------
CFKTipsManager&	CFKTipsManager::GetInstance()
{
	return *CFKTipsManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKTipsManager::Init()
{
	return true;
}
//------------------------------------------------------------
void CFKTipsManager::Clear()
{
	if(m_pMsgList)
	{
		m_pMsgList->removeAllObjects();
	}
	if(m_pHistory)
	{
		m_pHistory->removeAllObjects();
	}
}
//------------------------------------------------------------
void CFKTipsManager::SetScene(CCNode* p_pNode)
{
	m_pViewNode = p_pNode;
}
//------------------------------------------------------------
void CFKTipsManager::PushMsg(const char* p_szNickName, const char* p_szChatString, unsigned int m_unMaxLen)
{
	FK_DEBUG_LOG("%s: %s", FKStringFunc::GBK2UTF8(p_szNickName), FKStringFunc::GBK2UTF8(p_szChatString));
	if(strlen(p_szChatString) > m_unMaxLen) 
		return;
	if(m_pMsgList == NULL)
	{
		m_pMsgList = CCArray::create();
		CC_SAFE_RETAIN(m_pMsgList);
	}
	if(m_pHistory == NULL)
	{
		m_pHistory = CCArray::create();
		CC_SAFE_RETAIN(m_pHistory);
	}
	m_pMsgList->addObject(CCString::createWithFormat("%s:%s", 
		FKStringFunc::GBK2UTF8(p_szNickName), FKStringFunc::GBK2UTF8(p_szChatString)));

	if(m_bIsShow == false)
	{
		OnCheckTips();
	}
}
//------------------------------------------------------------
void CFKTipsManager::OnCheckTips()
{
	if( m_pMsgList->count() <= 0 )
		return;
	if( !m_pViewNode )
		return;

	m_bIsShow = true;
	CCString *pString = (CCString *)m_pMsgList->objectAtIndex(0);
	if(m_pHistory->count() > MAX_TIPS_HISTORY_NUM)
	{
		m_pHistory->removeObjectAtIndex(0);
	}
	m_pHistory->addObject(CCString::create(pString->getCString()));
	CCScale9Sprite *pBackground = CCScale9Sprite::createWithSpriteFrameName(DEFAULT_TIPS_BACKGROUND_IMAGE);
	if (pBackground == NULL)
	{
		return;
	}
	CCLabelTTF *pText = CCLabelTTF::create(pString->getCString(), 
		DEFAULT_TIPS_FONT_NAME, DEFAULT_TIPS_FONT_SIZE_TITLE*FK_SCALE);
	pText->setColor(ccORANGE);
	pText->setAnchorPoint(ccp(0, 0));
	pText->setPosition(ccp(60.0f*FK_SCALE, 5.0f*FK_SCALE));
	
	pBackground->setPosition(ccp(CFKVisiableRect::Top().x, CFKVisiableRect::Top().y - 120.0f*FK_SCALE));
	pBackground->setContentSize(CCSizeMake(pText->getContentSize().width + 120.0f*FK_SCALE, 
		pText->getContentSize().height + 10.0f*FK_SCALE));
	pBackground->addChild(pText);
	pBackground->setOpacity(0);
	m_pMsgList->removeObjectAtIndex(0);
	pBackground->runAction(CCSequence::create(
		CCFadeIn::create(0.3f),
		CCDelayTime::create(2.0f),
		CCMoveTo::create(0.3f, ccp(pBackground->getPositionX(), CFKVisiableRect::Top().y + 60.0f*FK_SCALE)),
		CCRemoveSelf::create(),
		CCCallFunc::create(this, callfunc_selector(CFKTipsManager::OnOverShow)), NULL));

	if (m_pViewNode)
	{
		m_pViewNode->addChild(pBackground);
	}
}
//------------------------------------------------------------
void CFKTipsManager::OnOverShow()
{
	m_bIsShow = false;
	OnCheckTips();
}
//------------------------------------------------------------