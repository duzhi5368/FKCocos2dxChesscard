/********************************************************************
	created:	2016/07/18
	filename: 	FKGoodData
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
struct SDataGood
{
	int				m_nGoodID;
	float			m_fPrice;
	int				m_nNum;
	int				m_nGold;
	std::string		m_strName;
	std::string		m_strDes1;
	std::string		m_strDes2;
	std::string		m_strGoodsPicName;
	std::string		m_strListInfo;

	SDataGood()
	{
		Clear();
	}

	void Clear()
	{
		m_nGoodID = m_nGold = m_nNum = 0;
		m_fPrice = 0.0f;
		m_strName.clear();
		m_strDes1.clear();
		m_strDes2.clear();
		m_strGoodsPicName.clear();
		m_strListInfo.clear();
	}
};