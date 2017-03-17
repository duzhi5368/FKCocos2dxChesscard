/********************************************************************
	created:	2016/07/12
	filename: 	FKUserConfig
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
// user custom config keys
#define CONFIG_KEY_USER_LOGIN_TIMES		"10001"
#define CONFIG_KEY_USER_SOUND_SWITCH	"10002"
#define CONFIG_KEY_USER_MUSIC_SWITCH	"10003"
#define CONFIG_KEY_USER_VIBRATE_SWITCH	"10004"
#define CONFIG_KEY_USER_SAVE_PSW_SWITCH	"10005"
//------------------------------------------------------------
// user custom config info
typedef struct SUserCustomConfig
{
	unsigned int	m_unLoginTimes;
	bool			m_bIsSoundOn;
	bool			m_bIsMusicOn;
	bool			m_bIsVibrateOn;

	SUserCustomConfig()
		: m_unLoginTimes	( 0 )
		, m_bIsSoundOn		( true )
		, m_bIsMusicOn		( true )
		, m_bIsVibrateOn	( true )
	{
	}

	// when you first use config, default value
	void SetDefaultValue()
	{
		m_unLoginTimes = 1;
		m_bIsMusicOn = m_bIsSoundOn = m_bIsVibrateOn = true;
	}

	// save to config
	void Save()
	{
		FKConfigFunc::SaveBoolValueAfterEncode(CONFIG_KEY_USER_VIBRATE_SWITCH, m_bIsVibrateOn);
		FKConfigFunc::SaveBoolValueAfterEncode(CONFIG_KEY_USER_SOUND_SWITCH, m_bIsSoundOn);
		FKConfigFunc::SaveBoolValueAfterEncode(CONFIG_KEY_USER_MUSIC_SWITCH, m_bIsMusicOn);
		FKConfigFunc::SaveIntergerValueAfterEncode(CONFIG_KEY_USER_LOGIN_TIMES, m_unLoginTimes);
	}

	// load from config
	void Load()
	{
		m_bIsVibrateOn		= FKConfigFunc::ReadBoolValueAfterDecode(CONFIG_KEY_USER_VIBRATE_SWITCH);
		m_bIsSoundOn		= FKConfigFunc::ReadBoolValueAfterDecode(CONFIG_KEY_USER_SOUND_SWITCH);
		m_bIsMusicOn		= FKConfigFunc::ReadBoolValueAfterDecode(CONFIG_KEY_USER_MUSIC_SWITCH);
		m_unLoginTimes		= FKConfigFunc::ReadIntegerValueAfterDecode(CONFIG_KEY_USER_LOGIN_TIMES);
	}

}USER_CUSTON_CONFIG;
//------------------------------------------------------------
