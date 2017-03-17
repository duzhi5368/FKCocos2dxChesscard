#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "Base/FKBaseMacros.h"
#include "Mgr/FKViewManager.h"
#include "Scene/FKSceneManager.h"
//------------------------------------------------------------
USING_NS_CC;
using namespace CocosDenshion;
//------------------------------------------------------------
AppDelegate::AppDelegate() {

}
//------------------------------------------------------------
AppDelegate::~AppDelegate() 
{
}
//------------------------------------------------------------
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	pEGLView->setFrameSize(960, 640);

    pDirector->setOpenGLView(pEGLView);

	// calculate view scale ratio and view size
	CCSize tagViewSize = CFKViewManager::GetInstancePtr()->GetShowSize();
	pEGLView->setDesignResolutionSize(tagViewSize.width, tagViewSize.height, kResolutionNoBorder);
	
    // turn on display FPS
    pDirector->setDisplayStats(SHOW_DEBUG_FPS);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / FPS_LIMIT);

    // start the scene
	CFKSceneManager::GetInstancePtr()->Init( START_SCENE_ID );

    return true;
}
//------------------------------------------------------------
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}
//------------------------------------------------------------
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
//------------------------------------------------------------
/// Update app's view size.
void AppDelegate::applicationUpdateViewSize(unsigned int p_unWidthPixel, unsigned int p_unHeightPixel)
{
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	pEGLView->setFrameSize(p_unWidthPixel, p_unHeightPixel);
}
//------------------------------------------------------------