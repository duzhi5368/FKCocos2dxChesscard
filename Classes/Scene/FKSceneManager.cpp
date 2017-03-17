#include "../Base/FKBaseMacros.h"
#include "FKSceneManager.h"
#include "FKSceneBase.h"
#include "FKSceneRes.h"

#include "FKSceneEmpty.h"
#include "FKSceneLogo.h"
#include "FKSceneLogin.h"
//------------------------------------------------------------
static CFKSceneManager* g_pSceneMgr = NULL;
//------------------------------------------------------------
static CFKSceneBase* __CreateScene(enumSceneID nIdx)
{    
	CFKSceneBase* pScene = NULL;


	FK_DEBUG_LOG("Create scene id = %d",(int)nIdx);
	switch (nIdx)
	{
	case eSceneID_Logo:
		pScene = new CFKSceneLogo(); 
		break;
	case eSceneID_Login:
		pScene = new CFKSceneLogin();
		break;

	default:
		pScene = new CFKSceneEmpty();
		break;
	}

	return pScene;
}
//------------------------------------------------------------
static CCScene* __TransScene(CCScene* s, enumSceneTransType type, float t = TRANS_DURATION)
{
	CCDirector::sharedDirector()->setDepthTest(false);

	switch (type)
	{
	case eSceneTransType_Default:
		return s;
		break;
	case eSceneTransType_JumpZoom: 
		return CCTransitionJumpZoom::create(t, s);
		break;
	case eSceneTransType_ProgressRadialCW: 
		return CCTransitionProgressRadialCW::create(t, s);
		break;
	case eSceneTransType_ProgressRadialCCW: 
		return CCTransitionProgressRadialCCW::create(t, s);
		break;
	case eSceneTransType_ProgressHorizontal: 
		return CCTransitionProgressHorizontal::create(t, s);
		break;
	case eSceneTransType_ProgressVertical: 
		return CCTransitionProgressVertical::create(t, s);
		break;
	case eSceneTransType_ProgressInOut: 
		return CCTransitionProgressInOut::create(t, s);
		break;
	case eSceneTransType_ProgressOutIn: 
		return CCTransitionProgressOutIn::create(t, s);
		break;
	case eSceneTransType_CrossFade: 
		return CCTransitionCrossFade::create(t,s);
		break;
	case eSceneTransType_PageForward:
		CCDirector::sharedDirector()->setDepthTest(true);
		return CCTransitionPageTurn::create(t, s, false);
		break;
	case eSceneTransType_PageBackward:
		CCDirector::sharedDirector()->setDepthTest(true);
		return CCTransitionPageTurn::create(t, s, true);
		break;
	case eSceneTransType_FadeTR: 
		return CCTransitionFadeTR::create(t, s);
		break;
	case eSceneTransType_FadeBL: 
		return CCTransitionFadeBL::create(t, s);
		break;
	case eSceneTransType_FadeUp: 
		return CCTransitionFadeUp::create(t, s);
		break;
	case eSceneTransType_FadeDown: 
		return CCTransitionFadeDown::create(t, s);
		break;
	case eSceneTransType_TurnOffTiles: 
		return CCTransitionTurnOffTiles::create(t, s);
		break;
	case eSceneTransType_SplitRows: 
		return CCTransitionSplitRows::create(t, s);
		break;
	case eSceneTransType_SplitCols: 
		return CCTransitionSplitCols::create(t, s);
		break;
	case eSceneTransType_Fade: 
		return CCTransitionFade::create(t, s);
		break;
	case eSceneTransType_FadeWhite: 
		return CCTransitionFade::create(t, s, ccWHITE);
		break;
	case eSceneTransType_FilpLeftOver:
		return CCTransitionFlipX::create(t, s, kCCTransitionOrientationLeftOver);
		break;
	case eSceneTransType_FilpRightOver:
		return CCTransitionFlipX::create(t, s, kCCTransitionOrientationRightOver);
		break;
	case eSceneTransType_FilpUpOver:
		return CCTransitionFlipY::create(t, s, kCCTransitionOrientationUpOver); 
		break;
	case eSceneTransType_FilpDownOver:
		return CCTransitionFlipY::create(t, s, kCCTransitionOrientationDownOver); 
		break;
	case eSceneTransType_FilpAngularLeftOver:
		return CCTransitionFlipAngular::create(t, s, kCCTransitionOrientationLeftOver);
		break;
	case eSceneTransType_FilpAngularRightOver:
		return CCTransitionFlipAngular::create(t, s, kCCTransitionOrientationRightOver);
		break;
	case eSceneTransType_ZoomFilpLeftOver:
		return CCTransitionZoomFlipX::create(t, s, kCCTransitionOrientationLeftOver);
		break;
	case eSceneTransType_ZoomFilpRightOver:
		return CCTransitionZoomFlipX::create(t, s, kCCTransitionOrientationRightOver);
		break;
	case eSceneTransType_ZoomFilpUpOver:
		return CCTransitionZoomFlipY::create(t, s, kCCTransitionOrientationUpOver); 
		break;
	case eSceneTransType_ZoomFilpDownOver:
		return CCTransitionZoomFlipY::create(t, s, kCCTransitionOrientationDownOver); 
		break;
	case eSceneTransType_ZoomFilpAngularLeftOver:
		return CCTransitionZoomFlipAngular::create(t, s, kCCTransitionOrientationLeftOver); 
		break;
	case eSceneTransType_ZoomFilpAngularRightOver:
		return CCTransitionZoomFlipAngular::create(t, s, kCCTransitionOrientationRightOver);
		break;
	case eSceneTransType_ShrinkGrow:
		return CCTransitionShrinkGrow::create(t, s);
		break;
	case eSceneTransType_RotoZoom:
		return CCTransitionRotoZoom::create(t, s);
		break;
	case eSceneTransType_MoveInL:
		return CCTransitionMoveInL::create(t, s);
		break;
	case eSceneTransType_MoveInR:
		return CCTransitionMoveInR::create(t, s);
		break;
	case eSceneTransType_MoveInT:
		return CCTransitionMoveInT::create(t, s);
		break;
	case eSceneTransType_MoveInB:
		return CCTransitionMoveInB::create(t, s);
		break;
	case eSceneTransType_SlideInL:
		return CCTransitionSlideInL::create(t, s);
		break;
	case eSceneTransType_SlideInR:
		return CCTransitionSlideInR::create(t, s);
		break;
	case eSceneTransType_SlideInT:
		return CCTransitionSlideInT::create(t, s);
		break;
	case eSceneTransType_SlideInB:
		return CCTransitionSlideInB::create(t, s);
		break;
	default:
		return s;
		break;
	}

	return s;
}
//------------------------------------------------------------
CFKSceneManager::CFKSceneManager()
{
#ifdef DEBUG
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
#endif
}
//------------------------------------------------------------
CFKSceneManager::~CFKSceneManager()
{

}
//------------------------------------------------------------
CFKSceneManager* CFKSceneManager::GetInstancePtr()
{
	if( g_pSceneMgr == NULL )
	{
		g_pSceneMgr = new CFKSceneManager();
		// no init here
	}
	return g_pSceneMgr;
}
//------------------------------------------------------------
CFKSceneManager& CFKSceneManager::GetInstance()
{
	return *CFKSceneManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKSceneManager::Init(enumSceneID p_eStartScene)
{
	CFKSceneBase* pScene = __CreateScene(p_eStartScene);
	if( pScene == NULL )
		return false;

	pScene->RunThisScene();
	pScene->autorelease();

	CCDirector::sharedDirector()->runWithScene(pScene);

	return true;
}
//------------------------------------------------------------
bool CFKSceneManager::ChangeScene(enumSceneID p_eFrom, enumSceneID p_eTo, enumSceneTransType p_eTransType)
{
	CFKSceneBase* pScene = __CreateScene(p_eTo);

	if (pScene)
	{
		FK_DEBUG_LOG("ChangeScene: from=%d, to=%d", p_eFrom, p_eTo);

		pScene->RunThisScene();
		CCDirector::sharedDirector()->replaceScene(__TransScene(pScene, p_eTransType));
		pScene->release();

		return true;
	}
	else
	{
		FK_DEBUG_LOG("ChangeScene Failed: from=%d, to=%d create scene error", p_eFrom, p_eTo);
		return false;
	}

	return false;
}
//------------------------------------------------------------
bool CFKSceneManager::GetSceneRes(enumSceneID id, vector<string>& p_vOutRes, unsigned short& p_usNeedToReleaseCount)
{
	bool bSuccess = true;
	unsigned short usCount = 0;
	p_vOutRes.clear();

	switch (id)
	{
	case eSceneID_Logo:
		{
			// importane : you must laod png first, and you must load plist second.
			p_vOutRes.push_back(Res_Image_CN_SceneGlobal_Share);
			p_vOutRes.push_back(Res_Image_CN_SceneGlobal_Share2);
			p_vOutRes.push_back(Res_Image_CN_SceneGlobal_SceneGame);

			p_vOutRes.push_back(Res_Plist_CN_SceneGlobal_Share);
			p_vOutRes.push_back(Res_Plist_CN_SceneGlobal_Share2);
			p_vOutRes.push_back(Res_Plist_CN_SceneGlobal_SceneGame);
			
			usCount = 0;
		}
		break;
	case eSceneID_Login:
		{
			usCount;
		}
		break;
	default:
		bSuccess = false;
		break;
	}
	p_usNeedToReleaseCount = usCount;
	return bSuccess;
}
//------------------------------------------------------------