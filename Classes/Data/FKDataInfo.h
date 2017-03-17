/********************************************************************
	created:	2016/07/18
	filename: 	FKDataInfo
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
struct SDataInfo
{
	float			m_fFontSize;
	std::string		m_strAnnurl;
	std::string		m_strViewName;
	std::string		m_strViewVersion;
	float			m_fKindID;

	SDataInfo()
	{
		Init();
	}

	void Init()
	{
		m_fFontSize = 24.0f;
		m_strAnnurl.clear();
		m_strViewVersion.clear();
		m_strViewName.clear();
		m_fKindID = 1;
	}
};
