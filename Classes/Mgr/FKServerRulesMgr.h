/********************************************************************
	created:	2016/07/22
	filename: 	FKServerRulesMgr
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Base/FKTypeDefine.h"
//------------------------------------------------------------
// chat rules
#define SR_FORFEND_GAME_CHAT		0x00000001		// no public chat in games
#define SR_FORFEND_ROOM_CHAT		0x00000002		// no public chat in rooms
#define SR_FORFEND_WISPER_CHAT		0x00000004		// no wisper chat
#define SR_FORFEND_WISPER_ON_GAME	0x00000008		// no wisper char in games

// other rules
#define SR_ALLOW_DYNAMIC_JOIN		0x00000010		// allow dynamic join games
#define SR_ALLOW_OFFLINE_TRUSTEE	0x00000020		// if offline, allow robot
#define SR_ALLOW_AVERT_CHEAT_MODE	0x00000040		// allow hide information

// game rules
#define SR_RECORD_GAME_SCORE		0x00000100		// recode game sorce
#define SR_RECORD_GAME_TRACK		0x00000200		// recode game detail track
#define SR_DYNAMIC_CELL_SCORE		0x00000400		// dynamic cell sorce
#define SR_IMMEDIATE_WRITE_SCORE	0x00000800		// immediate write sorce

// room rules
#define SR_FORFEND_ROOM_ENTER		0x00001000		// no enter room
#define SR_FORFEND_GAME_ENTER		0x00002000		// no enter game
#define SR_FORFEND_GAME_LOOKON		0x00004000		// no look on

// bank rules
#define SR_FORFEND_TAKE_IN_ROOM		0x00010000		// no allow to take money out
#define SR_FORFEND_TAKE_IN_GAME		0x00020000		// no allow to take money out
#define SR_FORFEND_SAVE_IN_ROOM		0x00040000		// no allow to save money in
#define SR_FORFEND_SAVE_IN_GAME		0x00080000		// no allow to save money in

// other rules
#define SR_FORFEND_GAME_RULE		0x00100000		// no change room config
#define SR_FORFEND_LOCK_TABLE		0x00200000		// no lock table
#define SR_ALLOW_ANDROID_ATTEND		0x00400000		// allow play service
#define SR_ALLOW_ANDROID_SIMULATE	0x00800000		// allow reserving a Seat

//------------------------------------------------------------
namespace FKServerRuleMgr
{
	bool IsForfendGameChat(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_CHAT)!=0; }
	bool IsForfendRoomChat(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_ROOM_CHAT)!=0; }
	bool IsForfendWisperChat(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_WISPER_CHAT)!=0; }
	bool IsForfendWisperOnGame(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_WISPER_ON_GAME)!=0; }

	bool IsAllowDynamicJoin(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_DYNAMIC_JOIN)!=0; }
	bool IsAllowOffLineTrustee(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_OFFLINE_TRUSTEE)!=0; }
	bool IsAllowAvertCheatMode(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_AVERT_CHEAT_MODE)!=0; }

	bool IsRecordGameScore(DWORD dwServerRule) { return (dwServerRule&SR_RECORD_GAME_SCORE)!=0; }
	bool IsRecordGameTrack(DWORD dwServerRule) { return (dwServerRule&SR_RECORD_GAME_TRACK)!=0; }
	bool IsDynamicCellScore(DWORD dwServerRule) { return (dwServerRule&SR_DYNAMIC_CELL_SCORE)!=0; }
	bool IsImmediateWriteScore(DWORD dwServerRule) { return (dwServerRule&SR_IMMEDIATE_WRITE_SCORE)!=0; }

	bool IsForfendRoomEnter(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_ROOM_ENTER)!=0; }
	bool IsForfendGameEnter(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_ENTER)!=0; }
	bool IsForfendGameLookon(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_LOOKON)!=0; }

	bool IsForfendTakeInRoom(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_TAKE_IN_ROOM)!=0; }
	bool IsForfendTakeInGame(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_TAKE_IN_GAME)!=0; }
	bool IsForfendSaveInRoom(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_SAVE_IN_ROOM)!=0; }
	bool IsForfendSaveInGame(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_SAVE_IN_GAME)!=0; }

	bool IsForfendGameRule(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_RULE)!=0; }
	bool IsForfendLockTable(DWORD dwServerRule) { return (dwServerRule&SR_FORFEND_LOCK_TABLE)!=0; }
	bool IsAllowAndroidAttend(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_ANDROID_ATTEND)!=0; }
	bool IsAllowAndroidSimulate(DWORD dwServerRule) { return (dwServerRule&SR_ALLOW_ANDROID_SIMULATE)!=0; }


	void SetForfendGameChat(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_CHAT:dwServerRule&=~SR_FORFEND_GAME_CHAT; }
	void SetForfendRoomChat(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_ROOM_CHAT:dwServerRule&=~SR_FORFEND_ROOM_CHAT; }
	void SetForfendWisperChat(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_WISPER_CHAT:dwServerRule&=~SR_FORFEND_WISPER_CHAT; }
	void SetForfendWisperOnGame(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_WISPER_ON_GAME:dwServerRule&=~SR_FORFEND_WISPER_ON_GAME; }

	void SetAllowDynamicJoin(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_DYNAMIC_JOIN:dwServerRule&=~SR_ALLOW_DYNAMIC_JOIN; }
	void SetAllowOffLineTrustee(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_OFFLINE_TRUSTEE:dwServerRule&=~SR_ALLOW_OFFLINE_TRUSTEE; }
	void SetAllowAvertCheatMode(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_AVERT_CHEAT_MODE:dwServerRule&=~SR_ALLOW_AVERT_CHEAT_MODE; }

	void SetRecordGameScore(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_RECORD_GAME_SCORE:dwServerRule&=~SR_RECORD_GAME_SCORE; }
	void SetRecordGameTrack(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_RECORD_GAME_TRACK:dwServerRule&=~SR_RECORD_GAME_TRACK; }
	void SetDynamicCellScore(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_DYNAMIC_CELL_SCORE:dwServerRule&=~SR_DYNAMIC_CELL_SCORE; }
	void SetImmediateWriteScore(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_IMMEDIATE_WRITE_SCORE:dwServerRule&=~SR_IMMEDIATE_WRITE_SCORE; }

	void SetForfendRoomEnter(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_ROOM_ENTER:dwServerRule&=~SR_FORFEND_ROOM_ENTER; }
	void SetForfendGameEnter(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_ENTER:dwServerRule&=~SR_FORFEND_GAME_ENTER; }
	void SetForfendGameLookon(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_LOOKON:dwServerRule&=~SR_FORFEND_GAME_LOOKON; }

	void SetForfendTakeInRoom(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_TAKE_IN_ROOM:dwServerRule&=~SR_FORFEND_TAKE_IN_ROOM; }
	void SetForfendTakeInGame(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_TAKE_IN_GAME:dwServerRule&=~SR_FORFEND_TAKE_IN_GAME; }
	void SetForfendSaveInRoom(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_SAVE_IN_ROOM:dwServerRule&=~SR_FORFEND_SAVE_IN_ROOM; }
	void SetForfendSaveInGame(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_SAVE_IN_GAME:dwServerRule&=~SR_FORFEND_SAVE_IN_GAME; }

	void SetForfendGameRule(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_RULE:dwServerRule&=~SR_FORFEND_GAME_RULE; }
	void SetForfendLockTable(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_LOCK_TABLE:dwServerRule&=~SR_FORFEND_LOCK_TABLE; }
	void SetAllowAndroidAttend(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_ANDROID_ATTEND:dwServerRule&=~SR_ALLOW_ANDROID_ATTEND; }
	void SetAllowAndroidSimulate(DWORD & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_ANDROID_SIMULATE:dwServerRule&=~SR_ALLOW_ANDROID_SIMULATE; }
};