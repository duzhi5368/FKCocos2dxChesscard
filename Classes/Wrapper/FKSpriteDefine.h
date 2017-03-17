/********************************************************************
	created:	2016/07/16
	filename: 	FKSpriteDefine
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
enum X_LCR  // X axes - Left Center Right
{
	X_LL = 0,
	X_LC,
	X_LR,
	X_CL,
	X_CC,
	X_CR,
	X_RL,
	X_RC,
	X_RR,
	X_CNT
};
//------------------------------------------------------------
enum Y_TCB	// X axes - Top Center Bottom
{
	Y_TT = 0,
	Y_TC,
	Y_TB,
	Y_CT,
	Y_CC,
	Y_CB,
	Y_BT,
	Y_BC,
	Y_BB,
	Y_CNT
};
//------------------------------------------------------------
typedef struct SSpritePos
{
	X_LCR x;
	Y_TCB y;
	CCPoint pt;

	SSpritePos()
	{
		x = X_CC;
		y = Y_CC;
		pt = CCPoint();
	}
}POS;
//------------------------------------------------------------