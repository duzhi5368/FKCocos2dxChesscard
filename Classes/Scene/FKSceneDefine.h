/********************************************************************
	created:	2016/07/13
	filename: 	FKSceneDefine
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
// the default start scene id
#define START_SCENE_ID		eSceneID_Logo
// the default transform scene time cost
#define TRANS_DURATION		1.0f
//------------------------------------------------------------
// scene id enum
enum enumSceneID
{
	eSceneID_Unknown	= 0,
	eSceneID_Logo,
	eSceneID_Login,

	eSceneID_Site,

	eSceneID_Max,
};
//------------------------------------------------------------
// the type of scene transform
enum enumSceneTransType
{
	eSceneTransType_Default = 0,

	eSceneTransType_JumpZoom,
	eSceneTransType_ProgressRadialCW,
	eSceneTransType_ProgressRadialCCW,
	eSceneTransType_ProgressHorizontal,
	eSceneTransType_ProgressVertical,
	eSceneTransType_ProgressInOut,
	eSceneTransType_ProgressOutIn,
	eSceneTransType_CrossFade,
	eSceneTransType_PageForward,
	eSceneTransType_PageBackward,
	eSceneTransType_FadeTR,
	eSceneTransType_FadeBL,
	eSceneTransType_FadeUp,
	eSceneTransType_FadeDown,
	eSceneTransType_TurnOffTiles,
	eSceneTransType_SplitRows,
	eSceneTransType_SplitCols,
	eSceneTransType_Fade,
	eSceneTransType_FadeWhite,
	eSceneTransType_FilpLeftOver,
	eSceneTransType_FilpRightOver,
	eSceneTransType_FilpUpOver,
	eSceneTransType_FilpDownOver,
	eSceneTransType_FilpAngularLeftOver,
	eSceneTransType_FilpAngularRightOver,
	eSceneTransType_ZoomFilpLeftOver,
	eSceneTransType_ZoomFilpRightOver,
	eSceneTransType_ZoomFilpUpOver,
	eSceneTransType_ZoomFilpDownOver,
	eSceneTransType_ZoomFilpAngularLeftOver,
	eSceneTransType_ZoomFilpAngularRightOver,
	eSceneTransType_ShrinkGrow,
	eSceneTransType_RotoZoom,
	eSceneTransType_MoveInL,
	eSceneTransType_MoveInR,
	eSceneTransType_MoveInT,
	eSceneTransType_MoveInB,
	eSceneTransType_SlideInL,
	eSceneTransType_SlideInR,
	eSceneTransType_SlideInT,
	eSceneTransType_SlideInB,

	eSceneTransType_Max,
};
//------------------------------------------------------------
// layer view style
enum enumLayerViewStyle
{
	eLayerViewStyle_Normal = 0,
	eLayerViewStyle_X,
	eLayerViewStyle_Y,
	eLayerViewStyle_XY,
	eLayerViewStyle_Full
};
//------------------------------------------------------------
// layer z-order type
enum enumLayerZOrderType
{
	eLayerZOrderTypeBegin	= 1,
	eLayerZOrderType100		= 100,
	eLayerZOrderType200		= 200,
	eLayerZOrderTypeMax		= 999
};
//------------------------------------------------------------