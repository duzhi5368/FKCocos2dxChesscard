#include "FKDataCenterManager.h"
#include "FKJsonFormatData.h"
#include "../Base/FKEncrpty.h"
#include "../Base/FKBaseMacros.h"
#include "../Base/FKConfigFunc.h"
#include "../Base/FKStringFunc.h"
#include "../Base/FKJson.h"
#include "../Mgr/FKMobileManager.h"
#include "../Platform/FKPlatformInterfaceManager.h"
//------------------------------------------------------------
#define PRIVATE_PASSWORD_KEY		"FK"
//------------------------------------------------------------
static CFKDataCenterManager* g_DataCenterMgr = NULL;
//------------------------------------------------------------
CFKDataCenterManager::CFKDataCenterManager()
{

}
//------------------------------------------------------------
CFKDataCenterManager::~CFKDataCenterManager()
{

}
//------------------------------------------------------------
CFKDataCenterManager* CFKDataCenterManager::GetInstancePtr()
{
	if( g_DataCenterMgr == NULL )
	{
		g_DataCenterMgr = new CFKDataCenterManager();
	}
	return g_DataCenterMgr;
}
//------------------------------------------------------------
CFKDataCenterManager& CFKDataCenterManager::GetInstance()
{
	return *CFKDataCenterManager::GetInstancePtr();
}
//------------------------------------------------------------
bool CFKDataCenterManager::Init(std::string strNewsUrl)
{
	Clear();
	LoadData();
	LoadNewsData( strNewsUrl );
	return true;
}
//------------------------------------------------------------
bool CFKDataCenterManager::LoadData()
{
	FK_DEBUG_LOG("DataCenterMgr : load data begin");
	m_Data_Logins.clear();
	m_Data_GameKinds.clear();
	m_Data_RoomGroupKinds.clear();
	m_Data_RoomKinds.clear();
	m_Data_Voices.clear();
	m_Data_Goods.clear();

	unsigned long size;
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(JSON_DATA_FILENAME);
	const char *pFileContent = (char *)CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str() , "r", &size);

	FKJson::Json *root = FKJson::Json_create(pFileContent);
	if(root && root->type == Json_Object)
	{
		FKJson::Json *record				= FKJson::Json_getItem(root, DATA_LOGIN)->child;

		while(record)
		{
			const char *ip					= Json_getItem(record, DATA_LOGIN_IP)->valuestring;
			const char *port				= Json_getItem(record, DATA_LOGIN_PORT)->valuestring;
			const char *strVerSion			= Json_getItem(record, DATA_VER)->valuestring;
			int temp[4] = {0};
			int i=0;
			char tempVer[32] = {0};
			memcpy(tempVer, strVerSion, sizeof(tempVer));
			char * pch = strtok(tempVer, ",");
			while(pch&&i<4)
			{
				temp[i++] = atoi(pch);
				pch = strtok(NULL, ",");;
			}
			SDataLogin data_login;
			data_login.m_strIp				= ip;
			data_login.m_strPort			= port;
			data_login.m_nVersion			= GAME_VERSION(PRODUCT_VER, temp[0], temp[1], temp[2]);

			m_Data_Logins.push_back(data_login);

			record = record->next;
		}

		//////////////////////////////////////////////////////////////////////////
		record = FKJson::Json_getItem(root, DATA_DATA)->child;
		while(record)
		{
			FKJson::Json *jannurl			= FKJson::Json_getItem(record, DATA_DATA_ANNURL);
			FKJson::Json *jfontsize			= FKJson::Json_getItem(record, DATA_DATA_FONTSIZE);
			FKJson::Json *jname				= FKJson::Json_getItem(record, DATA_DATA_VIEWNAME);
			FKJson::Json *jversion			= FKJson::Json_getItem(record, DATA_DATA_VIEWVERSION);

			m_Data_Info.m_strAnnurl			= FKJson::GetStrJson(jannurl);
			m_Data_Info.m_fFontSize			= FKJson::GetFloatJson(jfontsize);
			m_Data_Info.m_strViewName		= FKJson::GetStrJson(jname);
			m_Data_Info.m_strViewVersion	= FKJson::GetStrJson(jversion);

			break;
		}

		//////////////////////////////////////////////////////////////////////////
		record = FKJson::Json_getItem(root, DATA_GAME)->child;
		while(record)
		{
			const char *resback				= FKJson::Json_getItem(record, DATA_GAME_RES_BACK)->valuestring;
			const char *resback1			= FKJson::Json_getItem(record, DATA_GAME_RES_BACK1)->valuestring;
			const char *strname				= FKJson::Json_getItem(record, DATA_GAME_STR_NAME)->valuestring;
			const char *strkind				= FKJson::Json_getItem(record, DATA_GAME_KIND)->valuestring;
			const char *strVerSion			= FKJson::Json_getItem(record, DATA_GAME_VER)->valuestring;
			const char *restitle			= FKJson::Json_getItem(record, DATA_GAME_RES_TITLE)->valuestring;
			int temp[4] = {0};
			int i=0;
			char tempVer[32] = {0};
			memcpy(tempVer, strVerSion, sizeof(tempVer));
			char * pch = strtok(tempVer, ",");
			while(pch&&i<4)
			{
				temp[i++] = atoi(pch);
				pch = strtok(NULL, ",");;
			}
			SDataGameKind data_game;
			data_game.m_strResBack			= resback;
			data_game.m_strResBack1			= resback1;
			data_game.m_strResTitle			= restitle;
			data_game.m_strName				= strname;
			data_game.m_nKind				= atoi(strkind);
			data_game.m_nVersion			= GAME_VERSION(temp[0], temp[1], temp[2], temp[3]);

			m_Data_GameKinds.push_back(data_game);

			record = record->next;
		}

		//////////////////////////////////////////////////////////////////////////
		FKJson::Json *tempj = FKJson::Json_getItem(root, DATA_ROOM_GROUP);
		if (tempj)
		{
			record = tempj->child;
			while (record)
			{
				const char *sort0			= FKJson::Json_getItem(record, DATA_ROOM_GROUP_SORTID_START)->valuestring;
				const char *sort1			= FKJson::Json_getItem(record, DATA_ROOM_GROUP_SORTID_END)->valuestring;
				const char *resback			= FKJson::Json_getItem(record, DATA_ROOM_GROUP_RES_BACK)->valuestring;
				const char *resback1		= FKJson::Json_getItem(record, DATA_ROOM_GROUP_RES_BACK1)->valuestring;

				SDataRoomGroupKind data_group;
				data_group.m_strResBack		= resback;
				data_group.m_strResBack1	= resback1;
				data_group.m_nStartSortID	= atoi(sort0);
				data_group.m_nEndSortID		= atoi(sort1);

				m_Data_RoomGroupKinds.push_back(data_group);

				record = record->next;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		record = FKJson::Json_getItem(root, DATA_ROOM)->child;
		while(record)
		{
			const char *resback				= FKJson::Json_getItem(record, DATA_ROOM_RES_BACK)->valuestring;
			const char *resback1			= FKJson::Json_getItem(record, DATA_ROOM_RES_BACK1)->valuestring;
			const char *strname				= FKJson::Json_getItem(record, DATA_ROOM_STR_NAME)->valuestring;
			const char *strtitle			= FKJson::Json_getItem(record, DATA_ROOM_STR_TITLE)->valuestring;

			SDataRoomKind data_site;
			data_site.m_strResBack			= resback;
			data_site.m_strResBack1			= resback1;
			data_site.m_strName				= strname;
			data_site.m_strResTitle			= strtitle;

			m_Data_RoomKinds.push_back(data_site);

			record = record->next;
		}

		//////////////////////////////////////////////////////////////////////////
		record = FKJson::Json_getItem(root, DATA_GOOD)->child;
		while(record)
		{
			SDataGood goods;
			goods.m_nGoodID				= FKJson::Json_getItem(record, DATA_GOOD_INT_GOODID)->valueint;
			goods.m_fPrice				= FKJson::Json_getItem(record, DATA_GOOD_FLOAT_PRICE)->valuefloat;
			goods.m_nNum				= FKJson::Json_getItem(record, DATA_GOOD_INT_NUM)->valueint;
			goods.m_nGold				= FKJson::Json_getItem(record, DATA_GOOD_INT_GOLD)->valueint;
			goods.m_strName				= FKJson::Json_getItem(record, DATA_GOOD_STR_NAME)->valuestring;
			goods.m_strDes1				= FKJson::Json_getItem(record, DATA_GOOD_STR_DES1)->valuestring;
			goods.m_strDes2				= FKJson::Json_getItem(record, DATA_GOOD_STR_DES2)->valuestring;
			goods.m_strGoodsPicName		= FKJson::Json_getItem(record, DATA_GOOD_RES_ICON)->valuestring;
			goods.m_strListInfo			= FKJson::Json_getItem(record, DATA_GOOD_STR_LIST_INFO)->valuestring;

			m_Data_Goods.push_back(goods);

			record = record->next;
		}

		//////////////////////////////////////////////////////////////////////////
		record = FKJson::Json_getItem(root, DATA_VOICE)->child;
		while(record)
		{
			int idx						= FKJson::Json_getItem(record, DATA_VOICE_ID)->valueint;
			if (0 == idx)
			{
				continue;
			}

			const char *snd0			= FKJson::Json_getItem(record, DATA_VOICE_SND_0)->valuestring;
			const char *snd1			= FKJson::Json_getItem(record, DATA_VOICE_SND_1)->valuestring;
			const char *text0			= FKJson::Json_getItem(record, DATA_VOICE_TEXT_0)->valuestring;
			const char *text1			= FKJson::Json_getItem(record, DATA_VOICE_TEXT_1)->valuestring;

			SDataVoice data_voice;
			data_voice.m_nID			= idx;
			data_voice.m_strPath0		= snd0;
			data_voice.m_strPath1		= snd1;
			data_voice.m_strText0		= text0;
			data_voice.m_strText1		= text1;

			m_Data_Voices.insert(std::make_pair(idx, data_voice));

			record = record->next;
		}
		//////////////////////////////////////////////////////////////////////////
		delete record;
	}

	if (!LoadUserData())
	{
		FK_DEBUG_LOG("LoadUserData err");
	}

	return true;
}
//------------------------------------------------------------
bool CFKDataCenterManager::LoadUserData()
{
	m_Data_Users.clear(); 

	// Decode user
	int cnt				= FKConfigFunc::ReadIntegerValue(DATA_USER_CNT);
	int index			= FKConfigFunc::ReadIntegerValue(DATA_USER_INDEX);
	for (int i = 0; i < cnt; i++)
	{
		int k = 0;
		int temp = index + i;
		if (temp >= cnt)
		{
			k = cnt - i - 1;
		}
		else
		{
			k = (index + i) % cnt;
		}

		SDataUser data_user;
		std::string acc,psw;
		acc							= FKStringFunc::Format(DATA_USER_ACCOUNT, k);
		psw							= FKStringFunc::Format(DATA_USER_PASSWORD, k);
		data_user.m_nIndex			= k;
		data_user.m_strAccount		= FKConfigFunc::ReadStringValue(acc.c_str());
		data_user.m_strPassword		= FKConfigFunc::ReadStringValue(psw.c_str());
		if (data_user.m_strAccount.length() < 4)
		{
			continue;
		}
		m_Data_Users.push_back(data_user);
	}
	return true;
}
//------------------------------------------------------------
void CFKDataCenterManager::Clear()
{
	m_Data_Logins.clear();
	m_Data_GameKinds.clear();
	m_Data_RoomGroupKinds.clear();
	m_Data_RoomKinds.clear();
	m_Data_Voices.clear();
	m_Data_Goods.clear();

	m_Data_News.clear();
	m_Data_Users.clear();
}
//------------------------------------------------------------
bool CFKDataCenterManager::IsNeedUpdate(SDataUser p_tagUserData, int &index)
{
	int cnt = m_Data_Users.size();
	index = -1;
	for (int k=0; k<cnt; k++)
	{
		if (m_Data_Users[k].m_strAccount == p_tagUserData.m_strAccount)
		{
			char szPsw[MAX_USER_PASSWORD_LENGTH]={0};
			FKEncrypt::FKXORDecryptData(m_Data_Users[k].m_strPassword.c_str(), szPsw);
			if (0 == strcmp(szPsw, p_tagUserData.m_strPassword.c_str()))
			{
				index = m_Data_Users[k].m_nIndex;
				return false;
			}
			else
			{
				index = m_Data_Users[k].m_nIndex;
				return true;
			}
		}
	}

	return true;
}
//------------------------------------------------------------
bool CFKDataCenterManager::AddUserData(SDataUser user)
{
	SDataUser data = user;
	char md5pass[MAX_ENCRYPT_LEN] = {0};

	FKEncrypt::FKXOREncryptData(data.m_strPassword.c_str(), md5pass);
	data.m_strPassword = md5pass;

	// find is account already exsit
	int index = -1;
	if (false == IsNeedUpdate(user, index))
	{
		FK_DEBUG_LOG("AddUserData: add already exsit account");
		FKConfigFunc::SaveIntergerValue(DATA_USER_INDEX, index);
		LoadUserData();
		return false;
	}

	if (-1 != index && ((unsigned int)index) < m_Data_Users.size())
	{
		m_Data_Users[index].m_strPassword = data.m_strPassword;
		std::string psw = FKStringFunc::Format(DATA_USER_PASSWORD, index);
		FKConfigFunc::SaveStringValue(psw.c_str(), data.m_strPassword);
		FKConfigFunc::SaveIntergerValue(DATA_USER_INDEX, index);
		LoadUserData();
		FK_DEBUG_LOG("AddUserData: update exist account's password");
		return true;
	}

	// save unexist acccout
	m_Data_Users.push_back(data);

	// save to config
	int cnt = FKConfigFunc::ReadIntegerValue(DATA_USER_CNT) + 1;
	int k = cnt - 1;

	std::string acc,psw;
	acc = FKStringFunc::Format(DATA_USER_ACCOUNT, k);
	psw = FKStringFunc::Format(DATA_USER_PASSWORD, k);
	FKConfigFunc::SaveStringValue(acc.c_str(), data.m_strAccount);
	FKConfigFunc::SaveStringValue(psw.c_str(), data.m_strPassword);
	FKConfigFunc::SaveIntergerValue(DATA_USER_CNT, cnt);
	FKConfigFunc::SaveIntergerValue(DATA_USER_INDEX, k);

	FK_DEBUG_LOG("AddUserData add account successed");
	LoadUserData();

	return true;
}
//------------------------------------------------------------
bool CFKDataCenterManager::LoadUserDataByOrder(int order, SDataUser &user)
{
	if (m_Data_Users.size() <= (unsigned int)order)
		return false;

	unsigned int cnt = FKConfigFunc::ReadIntegerValue(DATA_USER_CNT);
	if (cnt < 1)
		return false;

	int k						= order;
	char md5pass[XOR_PASS_LEN]	= {0};
	FKEncrypt::FKXORDecryptData(m_Data_Users[k].m_strPassword.c_str(), md5pass);
	user.m_strAccount			= m_Data_Users[k].m_strAccount;
	user.m_strPassword			= md5pass;

	return true;
}
//------------------------------------------------------------
std::string CFKDataCenterManager::GetPswKey()
{
	return PRIVATE_PASSWORD_KEY;
}
//------------------------------------------------------------
bool CFKDataCenterManager::GetLoginData(int index, SDataLogin &data)
{
	if(index < 0 || m_Data_Logins.size() <= (unsigned int)index) 
	{
		return false; 
	}

	data = m_Data_Logins[index];
	return true;
}
//------------------------------------------------------------
SDataVoice* CFKDataCenterManager::GetDataVoice(int idx)
{
	m_Data_Voices.find(idx);
	std::map<int, SDataVoice>::iterator iter;

	iter = m_Data_Voices.find(idx);
	if(iter != m_Data_Voices.end())
	{
		return &iter->second;
	}
	return NULL;
}
//------------------------------------------------------------
const char* CFKDataCenterManager::OnPlayVoice(int idx, bool bMan)
{
	SDataVoice* pVoice = GetDataVoice(idx);
	if (NULL == pVoice)
		return "";
	return ( bMan ? pVoice->m_strPath1.c_str() : pVoice->m_strPath0.c_str() );
}
//------------------------------------------------------------
bool CFKDataCenterManager::IsKindID(int nKind)
{
	int nCnt = m_Data_GameKinds.size();
	for (int k=0; k<nCnt; k++)
	{
		if (nKind == m_Data_GameKinds[k].m_nKind)
		{
			return true;
		}
	}

	return false;
}
//------------------------------------------------------------
int	CFKDataCenterManager::GetPlazaVer()
{
	if (m_Data_Logins.size() == 0)
	{
		return 0;
	}
	return m_Data_Logins[0].m_nVersion;
}
//------------------------------------------------------------
int	CFKDataCenterManager::GetGameVer(int nKind)
{
	int nCnt = m_Data_GameKinds.size();

	for (int k=0; k<nCnt; k++)
	{
		if (nKind == m_Data_GameKinds[k].m_nKind)
		{
			return m_Data_GameKinds[k].m_nVersion;
		}
	}

	return 0;
}
//------------------------------------------------------------
int CFKDataCenterManager::GetGameKindCnt()
{
	return m_Data_GameKinds.size();
}
//------------------------------------------------------------
void CFKDataCenterManager::LoadNewsData(std::string strUrl)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	const char *url = CCString::createWithFormat(strUrl.c_str(), m_Data_Info.m_strViewVersion.c_str())->getCString();
	LoadUrlData("LoadNews", url);
#endif
}
//------------------------------------------------------------
void CFKDataCenterManager::LoadUrlData(std::string strTag, std::string strUrl)
{
	FK_DEBUG_LOG("LoadUrl: %s", strUrl.c_str());

	CCHttpRequest *request = new CCHttpRequest();
	request->setUrl(strUrl.c_str());
	request->setTag(strTag.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(CFKDataCenterManager::OnHttpRequestFinished));
	CCHttpClient::getInstance()->send(request);
	request->release();
}
//------------------------------------------------------------
void CFKDataCenterManager::OnHttpRequestFinished(CCHttpClient* client, CCHttpResponse* response)
{
	if ( response 
		&& strlen(response->getHttpRequest()->getTag()) != 0 
		&& response->isSucceed() )
	{
		std::string strTag			= response->getHttpRequest()->getTag();
		std::vector<char> *buffer	= response->getResponseData();
		std::string ret				= std::string(buffer->begin(), buffer->end());

		if ("LoadNews" == strTag)
		{
			ret = std::string(buffer->begin() + 3, buffer->end());
			FKJson::Json *root = FKJson::Json_create(ret.c_str());

			//FK_DEBUG_LOG("%s", ret.c_str());

			if (root && root->type == Json_Object)
			{
				std::string strStatus = FKJson::Json_getItem(root, "status")->valuestring;
				if ("0" != strStatus)
				{
					FKJson::Json *jitem = FKJson::Json_getItem(root, "data");
					if (jitem)
					{
						std::string strUrl = FKJson::Json_getItem(jitem, "url")->valuestring;

						//FK_DEBUG_LOG("%s", strUrl.c_str());

						CFKPlatformInterfaceManager::GetInstancePtr()->OpenUrl(strUrl.c_str());
						CFKMobileManager::GetInstancePtr()->QuitApp();
					}
				}				
			}
		}
	}
}
//------------------------------------------------------------