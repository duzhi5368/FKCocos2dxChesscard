#include "FKViewManager.h"
//------------------------------------------------------------
static CFKViewManager* g_pViewMgr = NULL;
//------------------------------------------------------------
CFKViewManager::CFKViewManager()
{

}
//------------------------------------------------------------
CFKViewManager::~CFKViewManager()
{

}
//------------------------------------------------------------
CFKViewManager*	CFKViewManager::GetInstancePtr()
{
	if( g_pViewMgr == NULL )
	{
		g_pViewMgr = new CFKViewManager();
		g_pViewMgr->Init();
	}
	return g_pViewMgr;
}
//------------------------------------------------------------
CFKViewManager&	CFKViewManager::GetInstance()
{
	return *CFKViewManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKViewManager::Init()
{
	return true;
}
//------------------------------------------------------------
CCSize CFKViewManager::GetShowSize()
{
	CCSize szBig = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize szSmall = CCSize( GetDefaultShowWidth(), GetDefaultShowHeight() );

	// get scale value
	float _fScale = GetShowScale(szBig, szSmall);

	return CCSize(_fScale * szSmall.width, _fScale * szSmall.height);
}
//------------------------------------------------------------
float CFKViewManager::GetShowScale(CCSize szSmall, CCSize szBig)
{
	CCAssert(szSmall.width > 0 && szSmall.height > 0, "GetShowScale error");

	float scaleX = (float) szBig.width / szSmall.width;
	float scaleY = (float) szBig.height / szSmall.height;

	float _fScale = 0.0f;
	if (scaleX > scaleY) 
	{
		_fScale = scaleX / (szBig.height / (float) szSmall.height);
	} 
	else 
	{
		_fScale = scaleY / (szBig.width / (float) szSmall.width);
	}

	return _fScale;
}
//------------------------------------------------------------
CCPoint CFKViewManager::GetXYScale(CCSize szSmall)
{
	CCAssert(szSmall.width > 0 && szSmall.height > 0, "szSmall error");

	CCSize szBig = CCEGLView::sharedOpenGLView()->getFrameSize();

	float scaleX = (float) szBig.width / GetDefaultShowWidth();
	float scaleY = (float) szBig.height / GetDefaultShowHeight();
	float scaleX2 = (float) GetDefaultShowWidth() / szSmall.width;
	float scaleY2 = (float) GetDefaultShowHeight() / szSmall.height;

	CCPoint _fScale = CCPoint(scaleX2, scaleY2);
	if (scaleX > scaleY) 
	{
		_fScale.x = szBig.width / szSmall.width / scaleY;
		_fScale.y = scaleY2;
	} 
	else 
	{
		_fScale.y = szBig.height / szSmall.height / scaleX;
		_fScale.x = scaleX2;
	}

	return _fScale;
}
//------------------------------------------------------------
float CFKViewManager::GetDefaultShowWidth()
{
	return 960.0f;
}
//------------------------------------------------------------
float CFKViewManager::GetDefaultShowHeight()
{
	return 640.0f;
}
//------------------------------------------------------------
bool CFKViewManager::SetSpriteSuitView(CCSprite* pSprite, bool bRotate)
{
	if (NULL == pSprite)
	{
		return false;
	}

	CCSize szTemp = pSprite->getContentSize();
	CCSize szFram = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize szSmall = szTemp;
	if (bRotate) 
	{
		szSmall.width = szTemp.height;
		szSmall.height = szTemp.width;
	}
	float fScale = GetShowScale(szSmall, szFram);
	pSprite->setScale(fScale);
	if (bRotate) 
	{
		pSprite->setRotation(270);
	}

	return true;
}
//------------------------------------------------------------
bool CFKViewManager::SetSpriteFullView(CCSprite* pSprite)
{
	if (NULL == pSprite) 
	{
		return false;
	}

	CCSize szSmall = pSprite->getContentSize();
	CCPoint fScale = GetXYScale(szSmall);
	pSprite->setScaleX(fScale.x);
	pSprite->setScaleY(fScale.y);

	return true;
}
//------------------------------------------------------------
bool CFKViewManager::SetSpriteXSuitView(CCSprite* pSprite)
{
	if (NULL == pSprite) 
	{
		return false;
	}

	CCSize szSmall = pSprite->getContentSize();
	CCPoint fScale = GetXYScale(szSmall);
	pSprite->setScaleX(fScale.x);

	return true;
}
//------------------------------------------------------------
bool CFKViewManager::SetSpriteYSuitView(CCSprite* pSprite)
{
	if (NULL == pSprite) 
	{
		return false;
	}

	CCSize szSmall = pSprite->getContentSize();
	CCPoint fScale = GetXYScale(szSmall);
	pSprite->setScaleY(fScale.y);

	return true;
}
//------------------------------------------------------------