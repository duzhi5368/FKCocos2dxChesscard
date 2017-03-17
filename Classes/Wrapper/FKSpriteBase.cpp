#include "FKSpriteBase.h"
#include "../Mgr/FKViewManager.h"
//------------------------------------------------------------
static inline CCPoint GetXYPoint(X_LCR x, Y_TCB y, CCSize os, CCSize is,
	CCPoint pt, CCPoint origin)
{
	CCPoint point=origin;
	switch (x)
	{
	case X_LL:
		{
			point.x += is.width/2;
			point.x += pt.x;
		}
		break;
	case X_LC:
		{

			point.x += pt.x;
		}
		break;
	case X_LR:
		{
			point.x -= is.width;
			point.x += pt.x;
		}
		break;
	case X_CL:
		{
			point.x += os.width/2;
			point.x += is.width/2;
			point.x += pt.x;
		}
		break;
	case X_CC:
		{
			point.x += os.width/2;
			point.x += pt.x;
		}
		break;
	case X_CR:
		{
			point.x += os.width/2;
			point.x -= is.width/2;
			point.x += pt.x;
		}
		break;
	case X_RL:
		{
			point.x += os.width;
			point.x += is.width/2;
			point.x += pt.x;
		}
		break;
	case X_RC:
		{
			point.x += os.width;
			point.x += pt.x;
		}
		break;
	case X_RR:
		{
			point.x += os.width;
			point.x -= is.width/2;
			point.x += pt.x;
		}
		break;
	case X_CNT:
		{
			point.x += pt.x;
		}
		break;
	default:
		{
			point.x += pt.x;
		}
		break;
	}

	switch (y)
	{
	case Y_TT:
		{
			point.y += os.height;
			point.y -= is.height/2;
			point.y += pt.y;
		}
		break;
	case Y_TC:
		{
			point.y += os.height;
			point.y += pt.y;
		}
		break;
	case Y_TB:
		{
			point.y += os.height;
			point.y += is.height/2;
			point.y += pt.y;
		}
		break;
	case Y_CT:
		{
			point.y += os.height/2;
			point.y -= is.height/2;
			point.y += pt.y;
		}
		break;
	case Y_CC:
		{
			point.y += os.height/2;
			point.y += pt.y;
		}
		break;
	case Y_CB:
		{
			point.y += os.height/2;
			point.y += is.height/2;
			point.y += pt.y;
		}
		break;
	case Y_BT:
		{
			point.y -= is.height/2;
			point.y += pt.y;
		}
		break;
	case Y_BC:
		{
			point.y += pt.y;
		}
		break;
	case Y_BB:
		{
			point.y += is.height/2;
			point.y += pt.y;
		}
		break;
	case Y_CNT:
		{
			point.y += pt.y;
		}
		break;
	default:
		{
			point.y += pt.y;
		}
		break;
	}


	return point;
}
//------------------------------------------------------------
CFKSpriteBase::~CFKSpriteBase(void)
{
}
//------------------------------------------------------------
CFKSpriteBase* CFKSpriteBase::create(const char *p_szFilename)
{
	CFKSpriteBase *pSprite = new CFKSpriteBase();
	if (pSprite && pSprite->initWithFile(p_szFilename))
	{
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}
//------------------------------------------------------------
CFKSpriteBase* CFKSpriteBase::createWithSpriteFrame(CCSpriteFrame *p_pSpriteFrame)
{
	CFKSpriteBase *pSprite = new CFKSpriteBase();
	if (p_pSpriteFrame && pSprite && pSprite->initWithSpriteFrame(p_pSpriteFrame))
	{
		pSprite->setOffsetInPixels(p_pSpriteFrame->getOffsetInPixels());
		pSprite->setOriginalSizeInPixels(p_pSpriteFrame->getOriginalSizeInPixels());
		pSprite->autorelease();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}
//------------------------------------------------------------
CFKSpriteBase* CFKSpriteBase::createWithSpriteFrameName(const char *p_pSpriteFrameName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(p_pSpriteFrameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", p_pSpriteFrameName);
	CCAssert(pFrame != NULL, msg);
#endif

	return createWithSpriteFrame(pFrame);
}
//------------------------------------------------------------
void CFKSpriteBase::SetPos(X_LCR x, Y_TCB y, CCPoint pt)
{
	CCPoint point(0,0);
	CCSize visibleSize	= CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin		= CCDirector::sharedDirector()->getVisibleOrigin();
	CCRect tagRect		= getTextureRect();
	float fScaleX		= getScaleX();
	float fScaleY		= getScaleY();
	tagRect.size.width	= fScaleX * tagRect.size.width;
	tagRect.size.height = fScaleY * tagRect.size.height;
	point = GetXYPoint(x, y, visibleSize, tagRect.size, pt, origin);

	setPosition(point);
}
//------------------------------------------------------------
void CFKSpriteBase::RunAniVector(vector<string> _vPath, float fdelay, bool bEndShow)
{
	int cnt = _vPath.size();
	if (0 == cnt)
	{
		return;
	}

	// add
	CCAnimation* animation = CCAnimation::create();
	for (int i = 0; i < cnt; i++)
	{
		animation->addSpriteFrameWithFileName(_vPath[i].c_str());
	}

	// set
	animation->setDelayPerUnit(fdelay);
	animation->setRestoreOriginalFrame(!bEndShow);

	// start
	CCAnimate* action = CCAnimate::create(animation);
	this->runAction(CCSequence::create(action, action->reverse(), NULL));
}
//------------------------------------------------------------
void CFKSpriteBase::RunAniPlist(const char* plist, const char* name, unsigned int loops, bool bEndShow)
{
	CCAnimationCache *pCache = CCAnimationCache::sharedAnimationCache();
	pCache->addAnimationsWithFile(plist);

	CCAnimation *pAnimation = pCache->animationByName(name);
	pAnimation->setLoops(loops);
	pAnimation->setRestoreOriginalFrame(!bEndShow);

	CCAnimate* pAction = CCAnimate::create(pAnimation);
	this->runAction(CCSequence::create(pAction, pAction->reverse(), NULL));
}
//------------------------------------------------------------
void CFKSpriteBase::SetFull()
{
	CCSize szBig = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize szSmall = CCSize( CFKViewManager::GetInstancePtr()->GetDefaultShowWidth(),
		CFKViewManager::GetInstancePtr()->GetDefaultShowHeight());

	float fScale = CFKViewManager::GetInstancePtr()->GetShowScale(szBig, szSmall);
	setScale(fScale);

	CCPoint pt = getPosition();
	setPosition(pt);
}
//------------------------------------------------------------