/********************************************************************
	created:	2016/07/16
	filename: 	FKSpriteBase
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKSpriteDefine.h"
#include <vector>
#include <string>
//------------------------------------------------------------
USING_NS_CC;
using std::vector;
using std::string;
//------------------------------------------------------------
class CFKSpriteBase : public CCSprite
{
public:
	CREATE_FUNC(CFKSpriteBase);
	~CFKSpriteBase(void);

	static CFKSpriteBase* create(const char* p_szFilename);
	static CFKSpriteBase* createWithSpriteFrame(CCSpriteFrame* p_pSpriteFrame);
	static CFKSpriteBase* createWithSpriteFrameName(const char* p_pSpriteFrameName);
public:
	void	SetPos(X_LCR x = X_CC, Y_TCB y = Y_CC, CCPoint pt = CCPoint());
	void	RunAniVector(vector<string> p_vecAniPath, float p_fDelay, bool p_bIsEndShow);
	void	RunAniPlist(const char* p_strList, const char* p_strName, 
		unsigned int p_unLoopTimes, bool p_bIsEndShow);

	// set the sprite fullScreen
	void	SetFull();

	/** get offset of the frame */
	inline const CCPoint&	getOffsetInPixels(void) const 
	{ return m_tagOffsetInPixels; }
	/** set offset of the frame */
	inline void				setOffsetInPixels(const CCPoint& offsetInPixels) 
	{ m_tagOffsetInPixels = offsetInPixels; }

	/** get original size of the trimmed image */
	inline const CCSize&	getOriginalSizeInPixels(void) const 
	{ return m_tagOriginalSizeInPixels; }
	/** set original size of the trimmed image */
	inline void				setOriginalSizeInPixels(const CCSize& sizeInPixels) 
	{ m_tagOriginalSizeInPixels = sizeInPixels; }

private:
	CCPoint m_tagOffsetInPixels;
	CCSize	m_tagOriginalSizeInPixels;

};
