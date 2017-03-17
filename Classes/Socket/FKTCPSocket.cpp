#include "FKTCPSocket.h"
#include "../NetMsg/CMD_MsgDefine.h"
#include "../Base/FKBaseMacros.h"
//------------------------------------------------------------
CFKTCPSocket::CFKTCPSocket()
	: m_ClientSocket( INVALID_SOCKET )
	, m_nOutputBufferLen( 0 )
	, m_nInputBufferLen( 0 )
	, m_nInputBuffeStartPos( 0 )
	, m_nTag( 0 )
{
	memset(m_pOutputBuffer, 0, sizeof(m_pOutputBuffer));
	memset(m_pInputBuffer, 0, sizeof(m_pInputBuffer));
}
//------------------------------------------------------------
CFKTCPSocket::~CFKTCPSocket()
{

}
//------------------------------------------------------------
bool CFKTCPSocket::Create(const char* pszServerIP, int nServerPort, int tagid, 
	int nBlockSec, bool bKeepAlive)
{
	// Check params
	if( pszServerIP == 0 || strlen(pszServerIP) > MAX_IP_LEN) 
	{
		return false;
	}

#ifdef WIN32
	// Init windows socket
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);
	if (ret != 0) 
	{
		return false;
	}
#endif

	// Create socket
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_ClientSocket == INVALID_SOCKET) 
	{
		CloseSocket();
		return false;
	}

	// Set socket keep alive
	if(bKeepAlive)
	{
		int optval = 1;
		if(setsockopt(m_ClientSocket, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval)))
		{
			CloseSocket();
			return false;
		}
	}

	// set nonblock model socket
#ifdef WIN32
	DWORD nMode = 1;
	int nRes = ioctlsocket(m_ClientSocket, FIONBIO, &nMode);
	if (nRes == SOCKET_ERROR) 
	{
		CloseSocket();
		return false;
	}
#else
	fcntl(m_ClientSocket, F_SETFL, O_NONBLOCK);
#endif

	unsigned long serveraddr = inet_addr(pszServerIP);
	if(serveraddr == INADDR_NONE)
	{
		// error ip address
		CloseSocket();
		return false;
	}

	sockaddr_in addr_in;
	memset((void *)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family			= AF_INET;
	addr_in.sin_port			= htons(nServerPort);
	addr_in.sin_addr.s_addr		= serveraddr;

	if( connect(m_ClientSocket, (sockaddr *)&addr_in, sizeof(addr_in)) == SOCKET_ERROR )
	{
		if ( IsHasError() )
		{
			CloseSocket();
			return false;
		}
		else    // WSAWOLDBLOCK
		{
			timeval timeout;
			timeout.tv_sec		= nBlockSec;
			timeout.tv_usec		= 0;
			fd_set writeset, exceptset;
			FD_ZERO(&writeset);
			FD_ZERO(&exceptset);
			FD_SET(m_ClientSocket, &writeset);
			FD_SET(m_ClientSocket, &exceptset);

			int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
			if (ret == 0 || ret < 0) 
			{
				CloseSocket();
				return false;
			} 
			else    // ret > 0
			{
				ret = FD_ISSET(m_ClientSocket, &exceptset);
				if(ret)        // or (!FD_ISSET(m_sockClient, &writeset)
				{
					CloseSocket();
					return false;
				}
			}
		}
	}

	m_nInputBufferLen			= 0;
	m_nInputBuffeStartPos		= 0;
	m_nOutputBufferLen			= 0;

	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	setsockopt(m_ClientSocket, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	m_nTag						= tagid;

	return true;
}
//------------------------------------------------------------
bool CFKTCPSocket::SendMsg(void* pBuf, int nSize)
{
	// Base check
	if(pBuf == NULL || nSize <= 0) 
	{
		return false;
	}
	if ( m_ClientSocket == INVALID_SOCKET ) 
	{
		FK_DEBUG_LOG("TCPSocket::SendMsg INVALID_SOCKET");
		return false;
	}

	int packsize = 0;
	packsize = nSize;

	// Check output buffer is full
	if(m_nOutputBufferLen + nSize > OUTBUFSIZE) 
	{
		// Send output msg immediately
		Flush();

		if(m_nOutputBufferLen + nSize > OUTBUFSIZE) 
		{
			// Something wrong ocurred
			Destroy();
			return false;
		}
	}
	// Add data to buffer's tail
	memcpy( m_pOutputBuffer + m_nOutputBufferLen, pBuf, nSize);
	m_nOutputBufferLen += nSize;

	return true;
}
//------------------------------------------------------------
bool CFKTCPSocket::ReceiveMsg(void* pBuf, int& nSize)
{
	// Base check
	if(pBuf == NULL || nSize <= 0) {
		return false;
	}
	if (m_ClientSocket == INVALID_SOCKET) {
		return false;
	}

	// Check if the msg is too small
	if( m_nInputBufferLen < sizeof(SMsgBase) ) 
	{
		// If msg is too small, try to recv it. and recheck again
		if(!RecvFromSock() || m_nInputBufferLen < sizeof(SMsgBase) ) 
		{
			return false;
		}
	}

	// get msg packege size
	int packsize = (unsigned char)m_pInputBuffer[(m_nInputBuffeStartPos + SMsgBase::GetMsgSizePosIndex()) % INBUFSIZE] +
		(unsigned char)m_pInputBuffer[(m_nInputBuffeStartPos + SMsgBase::GetMsgSizePosIndex() + 1) % INBUFSIZE] * 256;

	// Too big msg
	if (packsize <= 0 || packsize > MAX_MSG_BUFFER_LEN) 
	{
		// clear buffer
		m_nInputBufferLen = 0;
		m_nInputBuffeStartPos = 0;
		return false;
	}

	// if msg package' size is bigger than inputBuffer's size
	if (packsize > m_nInputBufferLen) 
	{
		// retry to get complate msg package
		if (!RecvFromSock() || packsize > m_nInputBufferLen)
		{
			return false;
		}
	}

	// copy a msg
	if(m_nInputBuffeStartPos + packsize > INBUFSIZE) 
	{
		// msg has rollback, must copy the end of the buffer( part 1 )
		int copylen = INBUFSIZE - m_nInputBuffeStartPos;
		memcpy(pBuf, m_pInputBuffer + m_nInputBuffeStartPos, copylen);

		// copy the head of the buffer( part 2 )
		memcpy((unsigned char *)pBuf + copylen, m_pInputBuffer, packsize - copylen);
		nSize = packsize;
	} 
	else 
	{
		// msg has not rollback, copy it easily
		memcpy(pBuf, m_pInputBuffer + m_nInputBuffeStartPos, packsize);
		nSize = packsize;
	}

	// get buffer head pos
	m_nInputBuffeStartPos = (m_nInputBuffeStartPos + packsize) % INBUFSIZE;
	m_nInputBufferLen -= packsize;

	return true;
}
//------------------------------------------------------------
bool CFKTCPSocket::Flush()
{
	if (m_ClientSocket == INVALID_SOCKET) 
	{
		return false;
	}
	if( m_nOutputBufferLen <= 0) 
	{
		// no data need to flush
		return true;
	}

	// send data
	int	outsize;
	outsize = send( m_ClientSocket, m_pOutputBuffer, m_nOutputBufferLen, 0);
	if(outsize > 0) 
	{
		// delete already sended data
		if( m_nOutputBufferLen - outsize > 0) 
		{
			memcpy(m_pOutputBuffer, m_pOutputBuffer + outsize, m_nOutputBufferLen - outsize);
		}

		m_nOutputBufferLen -= outsize;

		if (m_nOutputBufferLen < 0) 
		{
			return false;
		}
	} 
	else 
	{
		if ( IsHasError() ) 
		{
			Destroy();
			return false;
		}
	}

	return true;
}
//------------------------------------------------------------
bool CFKTCPSocket::Check()
{
	if ( m_ClientSocket == INVALID_SOCKET) 
	{
		return false;
	}

	char buf[1];
	int ret = recv(m_ClientSocket, buf, 1, MSG_PEEK);
	if(ret == 0) 
	{
		Destroy();
		return false;
	} 
	else if(ret < 0) 
	{
		if ( IsHasError() ) 
		{
			Destroy();
			return false;
		} 
		else 
		{ 
			// now block
			return true;
		}
	} 
	else 
	{
		// has data
		return true;
	}

	return true;
}
//------------------------------------------------------------
void CFKTCPSocket::Destroy()
{
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	int ret = setsockopt(m_ClientSocket, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	CloseSocket();

	m_ClientSocket			= INVALID_SOCKET;
	m_nOutputBufferLen		= 0;
	m_nInputBufferLen		= 0;
	m_nInputBuffeStartPos	= 0;

	memset(m_pOutputBuffer, 0, sizeof(m_pOutputBuffer));
	memset(m_pInputBuffer, 0, sizeof(m_pInputBuffer));
}
//------------------------------------------------------------
SOCKET CFKTCPSocket::GetSocket() const
{
	return m_ClientSocket;
}
//------------------------------------------------------------
int CFKTCPSocket::GetTagID()
{
	return m_nTag;
}
//------------------------------------------------------------
#if (SINGLE_MODEL == 1)
bool CFKTCPSocket::AddTestMsg(void* pBuf, int nSize)
{
	return true;
}
#endif
//------------------------------------------------------------
bool CFKTCPSocket::RecvFromSock()
{
	if ( m_nInputBufferLen >= INBUFSIZE || m_ClientSocket == INVALID_SOCKET) 
	{
		return false;
	}

	// recv first part data
	int savelen;
	int savepos;
	if( m_nInputBuffeStartPos + m_nInputBufferLen < INBUFSIZE)    
	{    // still has free buffer in InputBuffer
		savelen = INBUFSIZE - (m_nInputBuffeStartPos + m_nInputBufferLen);
	} 
	else 
	{
		savelen = INBUFSIZE - m_nInputBufferLen;
	}

	// end of inputBuffer
	savepos = (m_nInputBuffeStartPos + m_nInputBufferLen) % INBUFSIZE;
	int inlen = recv( m_ClientSocket, m_pInputBuffer + savepos, savelen, 0);
	if(inlen > 0) 
	{
		// recv data
		m_nInputBufferLen += inlen;

		if (m_nInputBufferLen > INBUFSIZE) 
		{
			return false;
		}

		// recv second part data 
		if(inlen == savelen && m_nInputBufferLen < INBUFSIZE)
		{
			int savelen = INBUFSIZE - m_nInputBufferLen;
			int savepos = (m_nInputBuffeStartPos + m_nInputBufferLen) % INBUFSIZE;
			inlen = recv(m_ClientSocket, m_pInputBuffer + savepos, savelen, 0);
			if(inlen > 0) 
			{
				m_nInputBufferLen += inlen;
				if (m_nInputBufferLen > INBUFSIZE) 
				{
					return false;
				}    
			} 
			else if(inlen == 0) 
			{
				Destroy();
				return false;
			} 
			else 
			{
				// disconnect or error (include block)
				if (IsHasError()) 
				{
					Destroy();
					return false;
				}
			}
		}
	} 
	else if( inlen == 0 ) 
	{
		Destroy();
		return false;
	} 
	else 
	{
		// disconnect or error (include block)
		if ( IsHasError() )
		{
			Destroy();
			return false;
		}
	}

	return true;
}
//------------------------------------------------------------
bool CFKTCPSocket::IsHasError()
{
#if (SINGLE_MODEL == 1)
	return false;
#endif
#ifdef WIN32
	int err = WSAGetLastError();
	if(err != WSAEWOULDBLOCK)
	{
#else
	int err = errno;
	if(err != EINPROGRESS && err != EAGAIN)
	{
#endif
		return true;
	}

	return false;
}
//------------------------------------------------------------
void CFKTCPSocket::CloseSocket()
{
#ifdef WIN32
	closesocket(m_ClientSocket);
	WSACleanup();
#else
	close(m_ClientSocket);
#endif
}
//------------------------------------------------------------