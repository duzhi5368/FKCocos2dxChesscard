#include "FKGameSocket.h"
#include "../Base/FKBaseMacros.h"
#include "FKSocketCommon.h"
//------------------------------------------------------------
CFKGameSocket::CFKGameSocket()
{
	ResetSocketData();
}
//------------------------------------------------------------
CFKGameSocket::~CFKGameSocket()
{

}
//------------------------------------------------------------
bool CFKGameSocket::SendData(UInt16 wMainCmdID, UInt16 wSubCmdID, void * pData, int wDataSize)
{
	// check msg size
	CC_ASSERT(wDataSize <= MAX_MSG_STRUCT_LEN);
	if (wDataSize > MAX_MSG_STRUCT_LEN) 
		return false;

	// construct
	BYTE cbDataBuffer[MAX_MSG_STRUCT_LEN]={0};
	memset(cbDataBuffer, 0, MAX_MSG_STRUCT_LEN);

	SMsgBase* pHead = (SMsgBase*)cbDataBuffer;
	pHead->m_tagMsgType.m_usMainMsgType = wMainCmdID;
	pHead->m_tagMsgType.m_usSubMsgType	= wSubCmdID;
	if (wDataSize>0)
	{
		CC_ASSERT(pData!=NULL);
		memcpy(pHead+1,pData,wDataSize);
	}

	// encrypt data
	WORD wSendSize=EncryptBuffer(cbDataBuffer,sizeof(SMsgBase)+wDataSize,sizeof(cbDataBuffer));
	FK_DEBUG_LOG(">>>>>>>>>>>>>>>>  SocketMgr.SendPacket [ID = %d:%d] size = %d byte  <<<<<<<<<<<<<<<<", 
		wMainCmdID, wSubCmdID, wSendSize);

#if (SINGLE_MODEL == 1)
	// add respone to myself
	return true;
#endif
	SendMsg(cbDataBuffer, wSendSize);
	return Flush(); 
}
//------------------------------------------------------------
bool CFKGameSocket::ReceiveGameMsg(void* pBuf, int& nSize)
{
	if (ReceiveMsg(pBuf, nSize))
	{
		char cbBuffer[MAX_MSG_STRUCT_LEN]={0};
		memcpy(cbBuffer, pBuf, nSize);
		char *pbufMsg = cbBuffer;

		while(true)
		{
			SMsgBase *pReceiveMsg = (SMsgBase *)pbufMsg;
			UInt16	dwCurMsgSize = pReceiveMsg->m_tagMsgHead.m_wPacketSize;
			FK_DEBUG_LOG("pReceiveMsg dwCurMsgSize: %d", dwCurMsgSize);
			if((int)dwCurMsgSize > nSize || dwCurMsgSize <= 0)
			{
				break;
			}

			BYTE cbBuffer[MAX_MSG_STRUCT_LEN] = {0};
			memcpy(cbBuffer, pbufMsg, dwCurMsgSize);
			UInt16 nRecvSize = DecryptBuffer(cbBuffer, dwCurMsgSize);

			SMsgBuffer pBuf;
			memcpy(&pBuf, (SMsgBuffer*)cbBuffer, nRecvSize);
			m_vBuffer.push_back(pBuf );
			m_dwRecvPacketCount++;

			pbufMsg	+= dwCurMsgSize;
			nSize	-= dwCurMsgSize;
			if(nSize <= 0)
			{
				break;
			}
		}

		return (m_vBuffer.size() > 0);
	}
	else
	{
		return false;
	}

	return false;
}
//------------------------------------------------------------
bool CFKGameSocket::GetBuffer(MSG_PACKS &paks)
{
	paks = m_vBuffer;
	m_vBuffer.clear();

	return true;
}
//------------------------------------------------------------
UInt16 CFKGameSocket::DecryptBuffer(BYTE pcbDataBuffer[], UInt16 wDataSize)
{
	// Check params
	CC_ASSERT(wDataSize >= sizeof(SMsgBase));
	CC_ASSERT(((SMsgBase *)pcbDataBuffer)->m_tagMsgHead.m_wPacketSize == wDataSize);

	// reset length
	UInt16 wSnapCount = 0;
	if ((wDataSize % sizeof(DWORD)) != 0)
	{
		wSnapCount = sizeof(DWORD) - wDataSize % sizeof(DWORD);
		memset(pcbDataBuffer + wDataSize, 0, wSnapCount);
	}

	UInt16 i = 0;
	// Get key
	if (m_dwRecvPacketCount == 0)
	{
		CC_ASSERT(wDataSize >= (sizeof(SMsgBase) + sizeof(DWORD)));
		if (wDataSize < (sizeof(SMsgBase) + sizeof(DWORD)))
		{
			FK_DEBUG_LOG("Decrpty data length error");
			return 0;
		}
		m_dwRecvXorKey = *(DWORD *)(pcbDataBuffer + sizeof(SMsgBase));
		m_dwSendXorKey = m_dwRecvXorKey;
		memmove(pcbDataBuffer + sizeof(SMsgBase), pcbDataBuffer + sizeof(SMsgBase) + sizeof(DWORD),
			wDataSize - sizeof(SMsgBase) - sizeof(DWORD));
		wDataSize -= sizeof(DWORD);
		((SMsgBase *)pcbDataBuffer)->m_tagMsgHead.m_wPacketSize -= sizeof(DWORD);
	}

	// decrypt data
	DWORD dwXorKey = m_dwRecvXorKey;
	DWORD * pdwXor = (DWORD *)(pcbDataBuffer + sizeof(SMsgHead));
	UInt16  * pwSeed = (UInt16 *)(pcbDataBuffer + sizeof(SMsgHead));
	UInt16 wEncrypCount = (wDataSize + wSnapCount - sizeof(SMsgHead)) / 4;
	for(i = 0; i < wEncrypCount; i++)
	{
		if((i == (wEncrypCount - 1)) && (wSnapCount > 0))
		{
			BYTE * pcbKey = ((BYTE *) & m_dwRecvXorKey) + sizeof(DWORD) - wSnapCount;
			memcpy(pcbDataBuffer + wDataSize, pcbKey, wSnapCount);
		}
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwPacketKey;
		*pdwXor++ ^= m_dwRecvXorKey;
		m_dwRecvXorKey = dwXorKey;
	}

	// mapping check code
	SMsgHead * pHead = (SMsgHead *)pcbDataBuffer;
	BYTE cbCheckCode = pHead->m_cbCheckCode;
	for(i = sizeof(SMsgHead); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode += pcbDataBuffer[i];
	}

	return wDataSize;
}
//------------------------------------------------------------
UInt16 CFKGameSocket::EncryptBuffer(BYTE pcbDataBuffer[], UInt16 wDataSize, UInt16 wBufferSize)
{
	// Check params
	CC_ASSERT(wDataSize>=sizeof(SMsgBase));
	CC_ASSERT(wBufferSize>=(wDataSize+2*sizeof(DWORD)));
	CC_ASSERT(wDataSize<=(sizeof(SMsgBase)+MAX_MSG_STRUCT_LEN));

	// reset length
	WORD wEncryptSize=wDataSize-sizeof(SMsgType),wSnapCount=0;
	if ((wEncryptSize%sizeof(DWORD))!=0)
	{
		wSnapCount=sizeof(DWORD)-wEncryptSize%sizeof(DWORD);
		memset(pcbDataBuffer+sizeof(SMsgHead)+wEncryptSize,0,wSnapCount);
	}

	// mapping check code 
	BYTE cbCheckCode=0;
	for (UInt16 i=sizeof(SMsgHead);i<wDataSize;i++)
	{
		cbCheckCode+=pcbDataBuffer[i];
		pcbDataBuffer[i]=MapSendByte(pcbDataBuffer[i]);
	}

	// create header
	SMsgBase * pHead=(SMsgBase *)pcbDataBuffer;
	pHead->m_tagMsgHead.m_cbCheckCode=~cbCheckCode+1;
	pHead->m_tagMsgHead.m_wPacketSize=wDataSize;
	pHead->m_tagMsgHead.m_cbDataKind=SOCKET_VERION;

	// create key
	DWORD dwXorKey=m_dwSendXorKey;
	if (m_dwSendPacketCount==0)
	{
		dwXorKey = DEFAULT_XOR_KEY;
	}

	// encrypt datas
	UInt16 * pwSeed=(UInt16 *)(pcbDataBuffer+sizeof(SMsgHead));
	DWORD * pdwXor=(DWORD *)(pcbDataBuffer+sizeof(SMsgHead));
	UInt16 wEncrypCount=(wEncryptSize+wSnapCount)/sizeof(DWORD);
	for (UInt16 i=0;i<wEncrypCount;i++)
	{
		*pdwXor++^=dwXorKey;
		dwXorKey=SeedRandMap(*pwSeed++);
		dwXorKey|=((DWORD)SeedRandMap(*pwSeed++))<<16;
		dwXorKey^=g_dwPacketKey;
	}

	// insert key
	if (m_dwSendPacketCount==0)
	{
		memmove(pcbDataBuffer+sizeof(SMsgBase)+sizeof(DWORD),pcbDataBuffer+sizeof(SMsgBase),wDataSize);
		*((DWORD *)(pcbDataBuffer+sizeof(SMsgBase)))=m_dwSendXorKey;
		pHead->m_tagMsgHead.m_wPacketSize+=sizeof(DWORD);
		wDataSize+=sizeof(DWORD);
	}

	// update value
	m_dwSendPacketCount++;
	m_dwSendXorKey=dwXorKey;

	return wDataSize;
}
//------------------------------------------------------------
void CFKGameSocket::ResetSocketData()
{
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;

	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_dwSendXorKey=DEFAULT_XOR_KEY;
	m_dwRecvXorKey=DEFAULT_XOR_KEY;

	m_vBuffer.clear();
}
//------------------------------------------------------------
WORD CFKGameSocket::SeedRandMap(UInt16 wSeed)
{
	DWORD dwHold=wSeed;
	return (WORD)((dwHold=dwHold*241103L+2533101L)>>16);
}
//------------------------------------------------------------
BYTE CFKGameSocket::MapSendByte(BYTE cbData)
{
	BYTE cbMap = g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}
//------------------------------------------------------------
BYTE CFKGameSocket::MapRecvByte(BYTE cbData)
{
	BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}
//------------------------------------------------------------