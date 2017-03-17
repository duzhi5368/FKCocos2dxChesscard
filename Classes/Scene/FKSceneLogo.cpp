#include "FKSceneLogo.h"
#include "../Base/FKVisiableRect.h"
#include "../Base/FKStringFunc.h"
#include "../Mgr/FKViewManager.h"
#include "FKSceneManager.h"
#include "FKSceneLogoRes.h"
//------------------------------------------------------------
CFKSceneLogo::CFKSceneLogo()
	: m_nImageLoaded( 0 )
	, m_nImageTotal( 0 )
	, m_pLoadingLabel( NULL )
{
	m_strResArray.clear();
}
//------------------------------------------------------------
CFKSceneLogo::~CFKSceneLogo()
{

}
//------------------------------------------------------------
void CFKSceneLogo::onBeginLoad()
{
	unsigned short usReleaseCnt = 0;

	CFKSceneManager::GetInstancePtr()->GetSceneRes(GetSceneID(), m_strResArray, usReleaseCnt);
	m_nImageLoaded = 0;
	m_nImageTotal = m_strResArray.size() / 2;

	CCTextureCache* pCache = CCTextureCache::sharedTextureCache();
	for (int i = 0; i < m_nImageTotal; i++)
	{
		pCache->addImageAsync(m_strResArray[i].c_str(), this, callfuncO_selector(CFKSceneLogo::onProgress));
	}
}
//------------------------------------------------------------
void CFKSceneLogo::onProgress(CCObject *sender)
{
	// add load image num
	m_nImageLoaded++;

	// update label text
	if (m_nImageLoaded + 1 < m_nImageTotal)
	{
		m_pLoadingLabel->setString(FK_UTF8(CCString::createWithFormat(FK_UTF8( Res_String_CN_SceneLogo_NowLoading ), 
			m_nImageLoaded + 1, m_nImageTotal)->getCString()));
	}
	else
	{
		m_pLoadingLabel->setString(FK_UTF8( Res_String_CN_SceneLogo_LoadDone ));
	}

	// done!
	if (m_nImageLoaded == m_nImageTotal)
	{
		CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();

		for (int i = 0; i < m_nImageTotal; i++)
		{
			cache->addSpriteFramesWithFile(m_strResArray[m_nImageTotal + i].c_str());
		}
		CCFiniteTimeAction *pAction1 = CCDelayTime::create(0.4f);
		CCFiniteTimeAction *pAction2 = CCCallFunc::create(this, callfunc_selector(CFKSceneLogo::onComplete));
		this->runAction(CCSequence::create(pAction1, pAction2, NULL));
	}
}
//------------------------------------------------------------
void CFKSceneLogo::onComplete()
{
	if (m_pLoadingLabel)
	{
		m_pLoadingLabel->setString(FK_UTF8( Res_String_CN_SceneLogo_LoadDone ));
	}

	CFKSceneManager::GetInstancePtr()->ChangeScene( eSceneID_Logo, eSceneID_Login, eSceneTransType_CrossFade );
}
//------------------------------------------------------------
void CFKSceneLogo::RunThisScene()
{
	CCSprite* pSpriteBackground = CCSprite::create( Res_Image_CN_SceneLog_LoadingBK );
	pSpriteBackground->setPosition(CFKVisiableRect::Center());
	addChild(pSpriteBackground);

	pSpriteBackground->runAction(CCSequence::createWithTwoActions(
		CCFadeTo::create(0.5f, 255.0),
		CCCallFunc::create(this, callfunc_selector(CFKSceneLogo::onBeginLoad))));

	m_pLoadingLabel = FKCW_UIWidget_Label::create(FK_UTF8( Res_String_CN_SceneLogo_BeginLoad ), "", 36);
	m_pLoadingLabel->setColor(ccWHITE);
	m_pLoadingLabel->setPosition(ccp(CFKVisiableRect::Bottom().x, CFKVisiableRect::Bottom().y + 80));
	this->addChild(m_pLoadingLabel);
}
//------------------------------------------------------------
enumSceneID CFKSceneLogo::GetSceneID()
{
	return eSceneID_Logo;
}
//------------------------------------------------------------