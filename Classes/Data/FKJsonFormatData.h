/********************************************************************
	created:	2016/07/19
	filename: 	FKJsonFormatData
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
static const char JSON_DATA_FILENAME[]			= "Config/GameData.json";
//------------------------------------------------------------
// login
static const char DATA_LOGIN[]					= "login";
static const char DATA_LOGIN_IP[]				= "ip";
static const char DATA_LOGIN_PORT[]				= "port";
static const char DATA_VER[]					= "ver";
// data
static const char DATA_DATA[]					= "data";
static const char DATA_DATA_FONTSIZE[]			= "fontsize";
static const char DATA_DATA_ANNURL[]			= "annurl";
static const char DATA_DATA_VIEWNAME[]			= "viewname";
static const char DATA_DATA_VIEWVERSION[]		= "viewversion";

// game
static const char DATA_GAME[]					= "game";
static const char DATA_GAME_KIND[]				= "kind";
static const char DATA_GAME_VER[]				= "ver";
static const char DATA_GAME_STR_NAME[]			= "name";
static const char DATA_GAME_RES_BACK[]			= "back";
static const char DATA_GAME_RES_BACK1[]			= "back1";
static const char DATA_GAME_RES_TITLE[]			= "title";

// roomgroup
static const char DATA_ROOM_GROUP[]				= "roomgroup";
static const char DATA_ROOM_GROUP_RES_BACK[]	= "back";
static const char DATA_ROOM_GROUP_RES_BACK1[]	= "back1";
static const char DATA_ROOM_GROUP_SORTID_START[]= "sortid_start";
static const char DATA_ROOM_GROUP_SORTID_END[]	= "sortid_end";

// site
static const char DATA_ROOM[]					= "room";
static const char DATA_ROOM_RES_BACK[]			= "back";
static const char DATA_ROOM_RES_BACK1[]			= "back1";
static const char DATA_ROOM_STR_NAME[]			= "name";
static const char DATA_ROOM_STR_TITLE[]			= "title";

// good
static const char DATA_GOOD[]					= "store";
static const char DATA_GOOD_INT_GOODID[]		= "goodid";
static const char DATA_GOOD_FLOAT_PRICE[]		= "price";
static const char DATA_GOOD_INT_NUM[]			= "num";
static const char DATA_GOOD_INT_GOLD[]			= "gold";
static const char DATA_GOOD_STR_NAME[]			= "name";
static const char DATA_GOOD_STR_DES1[]			= "strDes1";
static const char DATA_GOOD_STR_DES2[]			= "strDes2";
static const char DATA_GOOD_RES_ICON[]			= "strGoodsPicName";
static const char DATA_GOOD_STR_LIST_INFO[]		= "strListInfo";

// account
static const char DATA_USER_INDEX[]				= "index";
static const char DATA_USER_CNT[]				= "usercnt";
static const char DATA_USER_ACCOUNT[]			= "account_%d";
static const char DATA_USER_PASSWORD[]			= "password_%d";

// voicephrase
static const char DATA_VOICE[]					= "voicephrase";
static const char DATA_VOICE_ID[]				= "id";
static const char DATA_VOICE_SND_0[]			= "sound0";
static const char DATA_VOICE_SND_1[]			= "sound1";
static const char DATA_VOICE_TEXT_0[]			= "text0";
static const char DATA_VOICE_TEXT_1[]			= "text1";

// game set
static const char DATA_SET_OUTMAH[]				= "setoutmah";
static const char DATA_SET_SOUND[]				= "setsound";