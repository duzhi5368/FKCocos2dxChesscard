/********************************************************************
	created:	2016/07/12
	filename: 	FKVisiableRect
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKVisiableRect
{
public:
	static CCRect	GetVisibleRect();
	static CCSize	GetVisibleSize();
	static CCSize	GetFrameSize();

	static CCPoint	Left();
	static CCPoint	Right();
	static CCPoint	Top();
	static CCPoint	Bottom();
	static CCPoint	Center();
	static CCPoint	LeftTop();
	static CCPoint	RightTop();
	static CCPoint	LeftBottom();
	static CCPoint	RightBottom();
private:
	static void		LazyInit();
private:
	static CCRect	s_VisibleRect;
	static CCSize	s_Framesize;
};