/********************************************************************
	created:	2016/07/18
	filename: 	FKDataCenterManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "cocos-ext.h"

#include "FKDataInfo.h"
#include "FKLoginData.h"
#include "FKGameKindData.h"
#include "FKGoodData.h"
#include "FKRoomGroupKindData.h"
#include "FKRoomKindData.h"
#include "FKVoiceData.h"
#include "FKUserData.h"
//------------------------------------------------------------
USING_NS_CC;
USING_NS_CC_EXT;
//------------------------------------------------------------
class CFKDataCenterManager : public CCObject
{
public:
	CFKDataCenterManager();
	~CFKDataCenterManager();
public:
	static CFKDataCenterManager*	GetInstancePtr();
	static CFKDataCenterManager&	GetInstance();
public:
	bool							Init(std::string strNewsUrl);

	bool							LoadData();
	bool							LoadUserData();
	void							Clear();

	bool							IsNeedUpdate(SDataUser user, int &index);
	bool							AddUserData(SDataUser user);
	bool							LoadUserDataByOrder(int order, SDataUser &user);
	std::string						GetPswKey();
	bool							GetLoginData(int index, SDataLogin &data);
	SDataVoice*						GetDataVoice(int idx);
	const char*						OnPlayVoice(int idx, bool bMan);

	bool							IsKindID(int nKind);
	int								GetPlazaVer();
	int								GetGameVer(int nKind);
	int								GetGameKindCnt();

	void							LoadNewsData(std::string strUrl);
	void							LoadUrlData(std::string strTag, std::string strUrl);
	void							OnHttpRequestFinished(CCHttpClient* client, CCHttpResponse* response);
private:
	SDataInfo						m_Data_Info;
	std::vector<SDataLogin>			m_Data_Logins;
	std::vector<SDataGameKind>		m_Data_GameKinds;
	std::vector<SDataRoomKind>		m_Data_RoomKinds;
	std::vector<SDataRoomGroupKind> m_Data_RoomGroupKinds;
	std::vector<SDataGood>			m_Data_Goods;
	std::map<int, SDataVoice>		m_Data_Voices;
	std::vector<std::string>		m_Data_News;
	std::vector<SDataUser>			m_Data_Users;
};