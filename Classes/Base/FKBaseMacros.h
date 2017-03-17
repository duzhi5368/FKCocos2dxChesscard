/********************************************************************
	created:	2016/07/11
	filename: 	FKBaseMacros
	author:		FreeKnight
	
	purpose:	Base common macros.
*********************************************************************/
#pragma once
//------------------------------------------------------------
// is single model
#define SINGLE_MODEL		1
//------------------------------------------------------------
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)      do { if(p) { delete (p); (p) = 0; } } while(0)
#endif

#ifndef WIN32 
#define ZeroMemory(a, b)	(memset(a, 0, b))
#endif

#ifndef max2
#define max2(a,b)           (((a) > (b)) ? (a) : (b))
#endif

#ifndef min2
#define min2(a,b)           (((a) < (b)) ? (a) : (b))
#endif

#ifndef FK_DEBUG_LOG
#define FK_DEBUG_LOG(format,...)	cocos2d::CCLog("%s,%d:"format,__FILE__,__LINE__, ##__VA_ARGS__)
#endif

#ifndef FK_SCALE
#define FK_SCALE			(1/CCDirector::sharedDirector()->getContentScaleFactor())
#endif
//------------------------------------------------------------
#pragma region DebugMacro
#ifdef	DEBUG
#define SHOW_DEBUG_FPS		true
#define FPS_LIMIT			50000
#else
#define SHOW_DEBUG_FPS		false
#define FPS_LIMIT			60
#endif
#pragma endregion
//------------------------------------------------------------
#pragma region VersionMacro
// app version
#define BULID_VER											0
#define PRODUCT_VER											1
// current socket system version
#define SOCKET_VERION										0x01

#define INTERFACE_VERSION(cbMainVer,cbSubVer)				\
	(DWORD)(												\
	(((BYTE)(PRODUCT_VER)) << 24) +							\
	(((BYTE)(cbMainVer)) << 16) +							\
	((BYTE)(cbSubVer) << 8)) +								\
	((BYTE)(BULID_VER))

#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)		\
	(DWORD)(												\
	(((BYTE)(PRODUCT_VER)) << 24) +							\
	(((BYTE)(cbMainVer)) << 16) +							\
	((BYTE)(cbSubVer) << 8) +								\
	(BYTE)(cbBuildVer))

#define GAME_VERSION(a,b,c,d)								\
	(DWORD)(												\
	(((BYTE)(a)) << 24) +									\
	(((BYTE)(b)) << 16) +									\
	((BYTE)(c) << 8) +										\
	(BYTE)(d))
#pragma endregion
//------------------------------------------------------------