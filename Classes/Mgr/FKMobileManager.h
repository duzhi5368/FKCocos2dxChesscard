/********************************************************************
	created:	2016/07/11
	filename: 	FKMobileManager
	author:		FreeKnight
	
	purpose:	Mobile state controller, it's a singleton object.
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "../Base/FKConfigFunc.h"
#include "../Data/FKUserConfig.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKMobileManager
{
public:
	CFKMobileManager();
	~CFKMobileManager();
public:
	static CFKMobileManager*	GetInstancePtr();
	static CFKMobileManager&	GetInstance();
public:	// state control function
	bool						Init();
	void						PauseGame();
	void						ResumeGame();
	void						QuitGame();
	void						RestartGame();

	void						PauseApp();
	void						ResumeApp();
	void						QuitApp();
public:	// sound fuction
	void						SetVibrate(bool p_bValue);
	void						SetSound(bool p_bValue);
	void						SetMusic(bool p_bValue);

	void						PreloadBKMusic(const char* p_szFilePath);
	void						PlayBKMusic(const char* p_szFilePath, bool p_bIsLoop = true);
	void						StopBKMusic(bool p_bReleaseData);

	void						PlayEffect(const char* p_szFilePath, bool p_bLoop = false);
	void						StopAllEffect();
public:	// mobile power function
	void						DoVibrator(int p_nTime = 500);

	void						MessageBox(const char *pszMsg, const char *pszTitle);
private:
	void						TryToLoadUserConfig();
private:
	USER_CUSTON_CONFIG			m_tagUserCustonConfig;
private:
	bool						m_pIsPause;
	bool						m_pIsAppQuit;
	CCSet*						m_pActionTargets;
	CCString*					m_pStrLastMusic;
};