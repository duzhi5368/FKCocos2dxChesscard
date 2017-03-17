#include "FKSocketManager.h"
#include "../Base/FKBaseMacros.h"
#include "../Event/FKEventMacros.h"
//------------------------------------------------------------
static CFKSocketManager* g_pSocketMgr = NULL;
//------------------------------------------------------------
static void InitSocket(void)
{
#ifdef _WIN32  
	WSADATA data;
	if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
	{
		FK_DEBUG_LOG("Could not initialise Winsock.\n");
	}
#endif  
}
//------------------------------------------------------------
static void UninitSocket(void)
{
#ifdef _WIN32  
	WSACleanup();
#endif  
}
//------------------------------------------------------------
CFKSocketManager::CFKSocketManager()
	: m_pSocket(NULL),
	m_bIsConnected(false),
	m_nPort(0),
	m_cbSendRound(0),
	m_cbRecvRound(0),
	m_dwSendXorKey(DEFAULT_XOR_KEY),
	m_dwRecvXorKey(DEFAULT_XOR_KEY),
	m_dwSendPacketCount(0),
	m_dwRecvPacketCount(0),
	m_bIsNoticeOffline(true)
{

}
//------------------------------------------------------------
CFKSocketManager::~CFKSocketManager()
{
	Dispose();
}
//------------------------------------------------------------
CFKSocketManager* CFKSocketManager::GetInstancePtr()
{
	if( g_pSocketMgr == NULL )
	{
		g_pSocketMgr = new CFKSocketManager();
	}
	return g_pSocketMgr;
}
//------------------------------------------------------------
CFKSocketManager& CFKSocketManager::GetInstance()
{
	return *CFKSocketManager::GetInstancePtr();
}
//------------------------------------------------------------
std::string CFKSocketManager::UrlToIP(const char* p_szUrl)
{
	InitSocket();
	struct hostent* h = gethostbyname( p_szUrl );
	if (h != NULL)
	{
		unsigned char* p = (unsigned char *)(h->h_addr_list)[0];
		if (p != NULL)
		{
			char ip[16] = { 0 };
			sprintf(ip, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
			UninitSocket();
			return ip;
		}
	}
	UninitSocket();
	return "";
}
//------------------------------------------------------------
bool CFKSocketManager::Init(const char *ip, UInt16 port)
{
	memset((char *)&m_sHost, 0, sizeof(m_sHost));
	if (ip[0]>'0' && ip[0]<'3')
	{
		strcpy(m_sHost, ip);
	}
	else
	{
		strcpy(m_sHost, UrlToIP(ip).c_str());
	}
	m_nPort = port;
	return true;
}
//------------------------------------------------------------
void CFKSocketManager::Update(float dt)
{
	if(!m_bIsConnected || (m_pSocket == NULL))
	{
		return;
	}

	if(!m_pSocket->Check())
	{
		m_pSocket = NULL;
		OnConnectionAbort();
		return;
	}

	// flush msg to svr
	m_pSocket->Flush();

	// Recv msg from recv buffer, until recv buffer is empty
	while(true)
	{
		char buffer[MAX_MSG_BUFFER_LEN] = {0};
		int nSize = sizeof(buffer);
		char *pbufMsg = buffer;
		if(m_pSocket == NULL)
		{
			break;
		}
		if(!m_pSocket->ReceiveMsg(pbufMsg, nSize))
		{
			break;
		}
		while(true)
		{
			SMsgBase *pReceiveMsg = (SMsgBase *)pbufMsg;
			UInt16	dwCurMsgSize = pReceiveMsg->m_tagMsgHead.m_wPacketSize;
			if((int)dwCurMsgSize > nSize || dwCurMsgSize <= 0)
			{
				break;
			}

			BYTE cbBuffer[MAX_MSG_BUFFER_LEN] = {0};
			memcpy(cbBuffer, pbufMsg, dwCurMsgSize);
			UInt16 nRecvSize = DecrptyBuffer((BYTE *)cbBuffer, dwCurMsgSize);
			OnMessage(cbBuffer, nRecvSize);

			pbufMsg	+= dwCurMsgSize;
			nSize	-= dwCurMsgSize;
			if(nSize <= 0)
			{
				break;
			}
		}
	}
}
//------------------------------------------------------------
bool CFKSocketManager::SendData(UInt16 wMainCmdID, UInt16 wSubCmdID, UInt16 wDataSize, void *pData)
{
	if(!m_bIsConnected) 
		Connect();
	if( m_pSocket == NULL )
		return false;

	return m_pSocket->SendData(wMainCmdID, wSubCmdID, pData, wDataSize);
}
//------------------------------------------------------------
bool CFKSocketManager::SendData(UInt16 wMainCmdID, UInt16 wSubCmdID)
{
	if(!m_bIsConnected) 
		Connect();

	if( m_pSocket == NULL )
		return false;

	return m_pSocket->SendData(wMainCmdID, wSubCmdID, NULL, sizeof(SMsgBase));
}
//------------------------------------------------------------
void CFKSocketManager::DelSendBuffer(UInt16 bufflen, char *buff)
{
	CC_SAFE_DELETE_ARRAY(buff);
}
//------------------------------------------------------------
bool CFKSocketManager::Connect()
{
	FK_DEBUG_LOG(">>>>>>>>>>>>>>>>  SocketManager try to connect  <<<<<<<<<<<<<<<<");
	if(m_pSocket) 
		return false;
	m_pSocket = new CFKGameSocket();
	m_bIsConnected = m_pSocket->Create(m_sHost, m_nPort, 1);
#if (SINGLE_MODEL == 1 )
	m_bIsConnected = true;
#endif
	if(m_bIsConnected)
	{
		ResetValue();
		FK_DEBUG_LOG("Connect success\n");
	}
	else
	{
		FK_DEBUG_LOG("Connect failed - m_sHost=%s m_nPort=%d\n", m_sHost, m_nPort);
		OnSendNotifyMsg(SSocketEvent::eSocketEvent_SocketMsg, Res_String_CN_Notify_ErrorSocket.c_str());
		Dispose();
	}
	return m_bIsConnected;
}
//------------------------------------------------------------
void CFKSocketManager::OnMessage(BYTE pcbDataBuffer[], UInt16 wDataSize)
{
	SMsgBuffer *buffer = (SMsgBuffer *)pcbDataBuffer;
	SMsgType Command = ((SMsgBase *)buffer)->m_tagMsgType;
	FK_DEBUG_LOG("RecvMsg MainCmdID:%d SubCmdID:%d", Command.GetMsgMainType(), Command.GetMsgSubType());

	switch(Command.GetMsgMainType())
	{
		// TODO:
		/*
	case MDM_KN_COMMAND:
		{
			if(SUB_KN_DETECT_SOCKET == Command.wSubCmdID)
			{
				SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
				//CCLOG("SendData MDM_KN_COMMAND SUB_KN_DETECT_SOCKET");
			}
		}break;

	case MDM_GP_LOGON:
		{
			CCNotificationCenter::sharedNotificationCenter()->postNotification(EVT_SOCKET, (CCObject *)buffer);
			CCNotificationCenter::sharedNotificationCenter()->postNotification(EVT_GP_REGISTER, (CCObject *)buffer);
		}break;
		//case MDM_GP_CHEST:
		//	{
		//		CCNotificationCenter::sharedNotificationCenter()->postNotification(EVT_SOCKET, (CCObject *)buffer);
		//	}break;
	case MDM_GP_SERVER_LIST:
		{
			switch(Command.wSubCmdID)
			{
			case SUB_GP_LIST_SERVER:
				{
					int wSize = sizeof(tagGameServer);
					int wCnt = buffer->Head.TCPInfo.wPacketSize / wSize;
					tagGameServer *pGameServer = (tagGameServer *)buffer->cbBuffer;
					//LogonSocket.sendLogToServer(CCString::createWithFormat("SUB_GP_LIST_FINISH wCnt:%d", wCnt)->getCString());
					while (wCnt > 0)
					{
						GameMgr.addRooms(pGameServer++);
						wCnt--;
					}
					//LogonSocket.sendLogToServer("SUB_GP_LIST_SERVER1");
				}break;

			case SUB_GP_LIST_FINISH:
				{
					GameMgr.sortRooms();
					CCLOG("SUB_GP_LIST_FINISH (end)");
					//LogonSocket.sendLogToServer("SUB_GP_LIST_FINISH (end)");
				}break;

			default:break;
			}
		}break;

	case MDM_GP_USER_SERVICE:
		{
			CCNotificationCenter::sharedNotificationCenter()->postNotification(EVT_GP_USER_SERVICE, (CCObject *)buffer);
			switch (Command.wSubCmdID)
			{
			case SUB_GP_USER_INSURE_INFO:
				{
					CMD_GP_UserInsureInfo *pak = (CMD_GP_UserInsureInfo *)buffer->cbBuffer;

					App_UserInfo* pak2 = GameMgr.getGlobalUserInfo();
					if (pak2)
					{
						pak2->lUserScore = pak->lUserScore;
						pak2->lUserInsure = pak->lUserInsure;
						CCNotificationCenter::sharedNotificationCenter()->postNotification(EVT_GP_UPDATE_SCORE, NULL);
					}
				}break;
			}
		}break;
		*/
	default:
		{

		}break;
	}
}
//------------------------------------------------------------
int	CFKSocketManager::Send(Char *buff, UInt16 bufflen)
{
	int nRet = SOCKET_ERROR;
	if(m_pSocket && m_bIsConnected)
	{
		nRet = m_pSocket->SendMsg(buff, bufflen);
	}
	if(nRet == SOCKET_ERROR)
	{
		FK_DEBUG_LOG("Send Buff failed!\n");
	}

	return nRet;
}
//------------------------------------------------------------
void CFKSocketManager::ResetValue()
{
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = DEFAULT_XOR_KEY;
	m_dwRecvXorKey = DEFAULT_XOR_KEY;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
}
//------------------------------------------------------------
void CFKSocketManager::Dispose()
{
	FK_DEBUG_LOG(">>>>>>>>>>>>>>>>  SocketManager dispose  <<<<<<<<<<<<<<<<\n");
	if(m_pSocket)
	{
		m_pSocket->Destroy();
		delete m_pSocket;
		m_pSocket = NULL;

	}
	m_bIsConnected = false;
}
//------------------------------------------------------------
void CFKSocketManager::OnConnectionAbort()
{
	FK_DEBUG_LOG(">>>>>>>>>>>>>>>>  SocketManager offline  <<<<<<<<<<<<<<<<\n");
	if (m_bIsNoticeOffline)
	{
		OnSendNotifyMsg(SSocketEvent::eSocketEvent_Offline, Res_String_CN_Notify_SocketOffline.c_str() );
	}

	Dispose();
}
//------------------------------------------------------------
UInt16 CFKSocketManager::DecrptyBuffer(BYTE pcbDataBuffer[], UInt16 wDataSize)
{
	if( m_pSocket )
	{
		return m_pSocket->DecryptBuffer( pcbDataBuffer, wDataSize );
	}
	return 0;
}
//------------------------------------------------------------
UInt16 CFKSocketManager::EncryptBuffer(BYTE pcbDataBuffer[], UInt16 wDataSize, UInt16 wBufferSize)
{
	if( m_pSocket )
	{
		return m_pSocket->EncryptBuffer( pcbDataBuffer, wDataSize, wBufferSize );
	}
	return 0;
}
//------------------------------------------------------------
UInt16 CFKSocketManager::SeedRandMap(UInt16 wSeed)
{
	if( m_pSocket == NULL )
		return 0;
	return m_pSocket->SeedRandMap( wSeed );
}
//------------------------------------------------------------
BYTE CFKSocketManager::MapSendByte(BYTE const cbData)
{
	if( m_pSocket == NULL )
		return 0;
	return m_pSocket->MapSendByte( cbData );
}
//------------------------------------------------------------
BYTE CFKSocketManager::MapRecvByte(BYTE const cbData)
{
	if( m_pSocket == NULL )
		return 0;
	return m_pSocket->MapRecvByte( cbData );
}
//------------------------------------------------------------
void CFKSocketManager::OnSendNotifyMsg(int nEventIndex, const char* szMsg)
{
	SSocketEvent pak;
	pak.m_eEventType	= (SSocketEvent::ENUM_SocketEvent)nEventIndex;
	pak.m_strMsg		= szMsg;

	CCNotificationCenter::sharedNotificationCenter()->postNotification(
		EVENT_NET_TIMEOUT, (CCObject *)&pak);
}
//------------------------------------------------------------