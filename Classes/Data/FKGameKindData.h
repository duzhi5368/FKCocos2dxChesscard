/********************************************************************
	created:	2016/07/18
	filename: 	FKGameKindData
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
struct SDataGameKind
{
	int				m_nKind;
	int				m_nVersion;
	std::string		m_strName;
	std::string		m_strResBack;
	std::string		m_strResBack1;
	std::string		m_strResTitle;

	SDataGameKind()
	{
		Clear();
	} 

	void Clear()
	{
		m_nKind			= 0;
		m_nVersion		= 0;
		m_strName.clear();
		m_strResBack.clear();
		m_strResBack1.clear();
		m_strResTitle.clear();
	}
};
