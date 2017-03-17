/********************************************************************
	created:	2016/07/20
	filename: 	FKTCPSocket
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKSocketCommon.h"
#include "../Base/FKBaseMacros.h"

#ifdef WIN32
	#include <windows.h>
	#include <WinSock.h>
	#pragma comment( lib, "ws2_32.lib" )
#else
	#include <sys/socket.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>

	#define SOCKET int
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
#endif
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKTCPSocket
{
public:
	CFKTCPSocket();
	virtual ~CFKTCPSocket();
public:
	bool		Create(const char* pszServerIP, int nServerPort, int tagid, 
		int nBlockSec = MAX_INITSOCKET_BLOCK_SECONDS, bool bKeepAlive = false);
	bool		SendMsg(void* pBuf, int nSize);
	bool		ReceiveMsg(void* pBuf, int& nSize);
	bool		Flush();
	bool		Check();
	void		Destroy();

	SOCKET		GetSocket() const;
	int			GetTagID();
#if (SINGLE_MODEL == 1)
	bool		AddTestMsg(void* pBuf, int nSize);
#endif
private:
	bool		RecvFromSock();
	bool		IsHasError();
	void		CloseSocket();
private:
	SOCKET		m_ClientSocket;

	char		m_pOutputBuffer[OUTBUFSIZE];
	int			m_nOutputBufferLen;

	char		m_pInputBuffer[INBUFSIZE];
	int			m_nInputBufferLen;
	int			m_nInputBuffeStartPos;
	int			m_nTag;
};
