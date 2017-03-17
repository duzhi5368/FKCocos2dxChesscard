/********************************************************************
	created:	2016/07/18
	filename: 	FKRoomKindData
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
struct SDataRoomKind
{
	std::string		m_strName;
	std::string		m_strResBack;
	std::string		m_strResBack1;
	std::string		m_strResTitle;

	SDataRoomKind()
	{
		Clear();
	} 

	void Clear()
	{
		m_strName.clear();
		m_strResBack.clear();
		m_strResBack1.clear();
		m_strResTitle.clear();
	}
};

