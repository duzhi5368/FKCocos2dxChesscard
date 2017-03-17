#include "FKMobileManager.h"
#include "SimpleAudioEngine.h"
#include "../Base/FKBaseMacros.h"
#include "../Base/FKStringFunc.h"
#include "../Platform/FKPlatformInterfaceManager.h"
//------------------------------------------------------------
using namespace CocosDenshion;
//------------------------------------------------------------
static CFKMobileManager* g_pMobileMgr = NULL;
//------------------------------------------------------------
CFKMobileManager::CFKMobileManager()
	: m_pActionTargets( NULL )
	, m_pIsAppQuit( false )
	, m_pIsPause( false )
	, m_pStrLastMusic( NULL )
{

}
//------------------------------------------------------------
CFKMobileManager::~CFKMobileManager()
{
	CC_SAFE_RELEASE_NULL(m_pActionTargets);
	CC_SAFE_RELEASE_NULL(m_pStrLastMusic);
}
//------------------------------------------------------------
CFKMobileManager* CFKMobileManager::GetInstancePtr()
{
	if( g_pMobileMgr == NULL )
	{
		g_pMobileMgr = new CFKMobileManager();
		g_pMobileMgr->Init();
	}
	return g_pMobileMgr;
}
//------------------------------------------------------------
CFKMobileManager& CFKMobileManager::GetInstance()
{
	return *CFKMobileManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKMobileManager::Init()
{
	TryToLoadUserConfig();

	return true;
}
//------------------------------------------------------------
void CFKMobileManager::PauseGame()
{
	if( !m_pIsPause )
	{
		m_pIsPause = true;

		SimpleAudioEngine::sharedEngine()->stopAllEffects();
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();

		CC_SAFE_RELEASE(m_pActionTargets);
		m_pActionTargets = CCDirector::sharedDirector()->getActionManager()->pauseAllRunningActions();
		CC_SAFE_RETAIN(m_pActionTargets);	
	}
}
//------------------------------------------------------------
void CFKMobileManager::ResumeGame()
{
	if( m_pIsPause )
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		if (m_pActionTargets != NULL)
		{
			CCDirector::sharedDirector()->getActionManager()->resumeTargets(m_pActionTargets);
			CC_SAFE_RELEASE_NULL(m_pActionTargets);
		}

		m_pIsPause = false;
	}
}
//------------------------------------------------------------
void CFKMobileManager::QuitGame()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	SimpleAudioEngine::sharedEngine()->stopAllEffects();

	CC_SAFE_RELEASE_NULL(m_pActionTargets);
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
	CCDirector::sharedDirector()->getActionManager()->removeAllActions();

	m_pIsPause = false;
}
//------------------------------------------------------------
void CFKMobileManager::RestartGame()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	SimpleAudioEngine::sharedEngine()->stopAllEffects();

	CC_SAFE_RELEASE_NULL(m_pActionTargets);
	CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
	CCDirector::sharedDirector()->getActionManager()->removeAllActions();

	m_pIsPause = false;
}
//------------------------------------------------------------
void CFKMobileManager::PauseApp()
{
	if ( !m_pIsPause )
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	}
	CCDirector::sharedDirector()->pause();
	CCDirector::sharedDirector()->stopAnimation();
}
//------------------------------------------------------------
void CFKMobileManager::ResumeApp()
{
	if ( !m_pIsPause )
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	}
	CCDirector::sharedDirector()->resume();
	CCDirector::sharedDirector()->startAnimation();
}
//------------------------------------------------------------
void CFKMobileManager::QuitApp()
{
	if ( !m_pIsAppQuit )
	{
		m_pIsAppQuit = true;

		SimpleAudioEngine::end();
		CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
}
//------------------------------------------------------------
void CFKMobileManager::SetVibrate(bool p_bValue)
{
	m_tagUserCustonConfig.m_bIsVibrateOn = p_bValue;
	m_tagUserCustonConfig.Save();
}
//------------------------------------------------------------
void CFKMobileManager::SetSound(bool p_bValue)
{
	m_tagUserCustonConfig.m_bIsSoundOn = p_bValue;
	m_tagUserCustonConfig.Save();
}
//------------------------------------------------------------
void CFKMobileManager::SetMusic(bool p_bValue)
{
	m_tagUserCustonConfig.m_bIsMusicOn = p_bValue;
	m_tagUserCustonConfig.Save();

	if(m_tagUserCustonConfig.m_bIsMusicOn)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
	FK_DEBUG_LOG("CFKMobileManager SetMusic=%d", m_tagUserCustonConfig.m_bIsMusicOn?1:0);
}
//------------------------------------------------------------
void CFKMobileManager::PreloadBKMusic(const char* p_szFilePath)
{
	if ( m_tagUserCustonConfig.m_bIsMusicOn )
	{
		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(p_szFilePath);
	}
}
//------------------------------------------------------------
void CFKMobileManager::PlayBKMusic(const char* p_szFilePath, bool p_bIsLoop)
{
	if (p_szFilePath != NULL)
	{
		CC_SAFE_RELEASE_NULL(m_pStrLastMusic);
		m_pStrLastMusic = CCString::create(p_szFilePath);
		m_pStrLastMusic->retain();
	}
	if ( m_tagUserCustonConfig.m_bIsMusicOn )
	{
		if (p_szFilePath != NULL)
		{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic(p_szFilePath, p_bIsLoop);
		}
		else
		{
			if (m_pStrLastMusic != NULL)
			{
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_pStrLastMusic->getCString(), p_bIsLoop);
			}
		}
	}
}
//------------------------------------------------------------
void CFKMobileManager::StopBKMusic(bool p_bReleaseData)
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(p_bReleaseData);
}
//------------------------------------------------------------
void CFKMobileManager::PlayEffect(const char* p_szFilePath, bool p_bLoop)
{
	if ( m_tagUserCustonConfig.m_bIsMusicOn )
	{
		SimpleAudioEngine::sharedEngine()->playEffect(p_szFilePath, p_bLoop);
	}
}
//------------------------------------------------------------
void CFKMobileManager::StopAllEffect()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}
//------------------------------------------------------------
void CFKMobileManager::DoVibrator(int p_nTime)
{
	if( m_tagUserCustonConfig.m_bIsVibrateOn )
	{
		CFKPlatformInterfaceManager::GetInstancePtr()->DoVibrator(p_nTime);
	}
}
//------------------------------------------------------------
void CFKMobileManager::MessageBox(const char *pszMsg, const char *pszTitle)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CCMessageBox(FKStringFunc::UTF82GBK(pszMsg).c_str(), FKStringFunc::UTF82GBK(pszTitle).c_str());
#else
	CCMessageBox(FKStringFunc::BK2UTF8(pszMsg).c_str(), FKStringFunc::GBK2UTF8(pszTitle).c_str());
#endif
}
//------------------------------------------------------------
void CFKMobileManager::TryToLoadUserConfig()
{
	CCUserDefault::sharedUserDefault()->purgeSharedUserDefault();

	m_tagUserCustonConfig.m_unLoginTimes = FKConfigFunc::ReadIntegerValueAfterDecode(CONFIG_KEY_USER_LOGIN_TIMES);
	if( m_tagUserCustonConfig.m_unLoginTimes < 1 )	// first use config
	{
		// set default value
		m_tagUserCustonConfig.SetDefaultValue();

		// save default value
		m_tagUserCustonConfig.Save();
	}
	else	// really load config
	{
		// update login times and load config
		m_tagUserCustonConfig.m_unLoginTimes++;

		FKConfigFunc::SaveIntergerValueAfterEncode(CONFIG_KEY_USER_LOGIN_TIMES, m_tagUserCustonConfig.m_unLoginTimes);

		// load 
		m_tagUserCustonConfig.Load();
	}

	FK_DEBUG_LOG("CFKMobileManager TryToLoadUserConfig LoginTimes=%d VibrateOn=%d SoundOn=%d MusicOn=%d", 
		m_tagUserCustonConfig.m_unLoginTimes, 
		m_tagUserCustonConfig.m_bIsVibrateOn?1:0, 
		m_tagUserCustonConfig.m_bIsSoundOn?1:0, 
		m_tagUserCustonConfig.m_bIsMusicOn?1:0);
}
//------------------------------------------------------------