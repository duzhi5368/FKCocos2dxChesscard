/********************************************************************
	created:	2016/07/18
	filename: 	FKRoomGroupKindData
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
struct SDataRoomGroupKind
{
	int				m_nStartSortID;
	int				m_nEndSortID;
	std::string		m_strResBack;
	std::string		m_strResBack1;

	SDataRoomGroupKind()
	{
		Init();
	} 

	void Init()
	{
		m_nStartSortID = 0;
		m_nEndSortID = 100000;
		m_strResBack.clear();
		m_strResBack1.clear();
	}
};
