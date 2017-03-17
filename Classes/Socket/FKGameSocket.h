/********************************************************************
	created:	2016/07/20
	filename: 	FKGameSocket
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "FKTCPSocket.h"
#include "../Base/FKTypeDefine.h"
#include "../NetMsg/CMD_MsgDefine.h"
//------------------------------------------------------------
typedef std::vector<SMsgBuffer>		MSG_PACKS;
//------------------------------------------------------------
class CFKGameSocket : public CFKTCPSocket
{
public:
	CFKGameSocket();
	~CFKGameSocket();

	bool		SendData(UInt16 wMainCmdID, UInt16 wSubCmdID, void* pData, int wDataSize);
	bool		ReceiveGameMsg(void* pBuf, int& nSize);
	bool		GetBuffer(MSG_PACKS &paks);
	UInt16		DecryptBuffer(BYTE pcbDataBuffer[], UInt16 wDataSize);
	UInt16		EncryptBuffer(BYTE cbDataBuffer[], UInt16 wDataSize, UInt16 wBufferSize);
	void		ResetSocketData();
	inline WORD SeedRandMap(UInt16 wSeed);
	inline BYTE MapSendByte(BYTE cbData);
	inline BYTE MapRecvByte(BYTE cbData);
private:
	MSG_PACKS	m_vBuffer;
	DWORD		m_dwSendTickCount;
	DWORD		m_dwRecvTickCount;
	DWORD		m_dwSendPacketCount;
	DWORD		m_dwRecvPacketCount;
	BYTE		m_cbSendRound;	
	BYTE		m_cbRecvRound;	
	DWORD		m_dwSendXorKey;	
	DWORD		m_dwRecvXorKey;
};