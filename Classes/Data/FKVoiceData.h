/********************************************************************
	created:	2016/07/18
	filename: 	FKVoiceData
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
struct SDataVoice
{
	int			m_nID;
	std::string m_strPath0;			// woman's voice
	std::string m_strPath1;			// man's voice
	std::string m_strText0;
	std::string m_strText1;

	SDataVoice()
	{
		Clear();
	}

	void Clear()
	{
		m_nID		= 0;
		m_strPath0.clear();
		m_strPath1.clear();
		m_strText0.clear();
		m_strText1.clear();
	}
};
