/********************************************************************
	created:	2016/07/19
	filename: 	CMD_MsgDefine
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#define MAX_MSG_BUFFER_LEN			4096
#define MAX_MSG_STRUCT_LEN			(MAX_MSG_BUFFER_LEN-sizeof(SMsgBase))
//------------------------------------------------------------
enum ENUM_MainMsgType
{
	eMainMsgType_Unknown = 0,

	eMainMsgType_Login,


	eMainMsgType_Max,
};
//------------------------------------------------------------
// Msg struct
/*
	SMsgBase
		SMsgHead	4 bytes
			+
		SMsgType	4 bytes
			+
	MsgStruct
		Your struct define ...
*/
//------------------------------------------------------------
struct SMsgHead
{
	unsigned char				m_cbDataKind;
	unsigned char				m_cbCheckCode;
	unsigned short				m_wPacketSize;
	SMsgHead()
		: m_cbCheckCode( 0 )
		, m_cbDataKind( 0 )
		, m_wPacketSize( 0 )
	{
	}
};
//------------------------------------------------------------
struct SMsgType
{
	unsigned short		m_usMainMsgType;
	unsigned short		m_usSubMsgType;

	SMsgType()
		: m_usMainMsgType( 0 )
		, m_usSubMsgType( 0 )
	{
	}

	ENUM_MainMsgType GetMsgMainType()
	{
		return (ENUM_MainMsgType)m_usMainMsgType;
	}
	int				GetMsgSubType()
	{
		return	m_usSubMsgType;
	}
};
//------------------------------------------------------------
struct SMsgBase
{
	SMsgHead			m_tagMsgHead;
	SMsgType			m_tagMsgType;

	static int			GetMsgSizePosIndex()
	{
		return			2;	// sizeof(m_cbDataKind) + sizeof(m_cbCheckCode)
	}
};
//------------------------------------------------------------
struct SMsgBuffer
{
private:
	SMsgBase			m_tagMsgBase;
	char				m_tagMsg[MAX_MSG_STRUCT_LEN];
public:
	SMsgBuffer()
	{
		memset( &m_tagMsg, 0, MAX_MSG_STRUCT_LEN );
	}
	unsigned short		GetMsgSize()
	{
		return m_tagMsgBase.m_tagMsgHead.m_wPacketSize;
	}
	unsigned short		GetSubMsgType()
	{
		return m_tagMsgBase.m_tagMsgType.GetMsgSubType();
	}
	unsigned short		GetMainMsgType()
	{
		return m_tagMsgBase.m_tagMsgType.GetMsgMainType();
	}
	char*				GetBuffer()
	{
		return m_tagMsg;
	}
};
//------------------------------------------------------------