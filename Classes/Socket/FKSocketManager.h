/********************************************************************
	created:	2016/07/16
	filename: 	FKSocketManager
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "../Base/FKTypeDefine.h"
#include "FKGameSocket.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKSocketManager : public CCObject
{
public:
	CFKSocketManager();
	~CFKSocketManager();
public:
	static CFKSocketManager*	GetInstancePtr();
	static CFKSocketManager&	GetInstance();
public:
	std::string					UrlToIP(const char* p_szUrl);
	bool						Init(const char *ip, UInt16 port);
	void						Update(float dt);
	bool						SendData(UInt16 wMainCmdID, UInt16 wSubCmdID, UInt16 wDataSize, void *pData);
	bool						SendData(UInt16 wMainCmdID, UInt16 wSubCmdID);
	void						DelSendBuffer(UInt16 bufflen, char *buff);

	virtual bool				Connect();
	virtual void				OnMessage(BYTE pcbDataBuffer[], UInt16 wDataSize);

	int							Send(Char *buff, UInt16 bufflen);
	void						ResetValue();
	void						Dispose();
private:
	void						OnConnectionAbort();
	UInt16						DecrptyBuffer(BYTE pcbDataBuffer[], UInt16 wDataSize);
	UInt16						EncryptBuffer(BYTE pcbDataBuffer[], UInt16 wDataSize, UInt16 wBufferSize);
	UInt16						SeedRandMap(UInt16 wSeed);
	BYTE						MapSendByte(BYTE const cbData);
	BYTE						MapRecvByte(BYTE const cbData);
	void						OnSendNotifyMsg(int nEventIndex, const char* szMsg);
private:
	UInt16						m_nPort;
	char						m_sHost[20];
	DWORD						m_dwSendPacketCount;
	DWORD						m_dwRecvPacketCount;
	BYTE						m_cbSendRound;
	BYTE						m_cbRecvRound;
	DWORD						m_dwSendXorKey;
	DWORD						m_dwRecvXorKey;
	CFKGameSocket *				m_pSocket;
	bool						m_bIsConnected;
	bool						m_bIsNoticeOffline;
};