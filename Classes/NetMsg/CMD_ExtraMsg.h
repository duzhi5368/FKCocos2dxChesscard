/********************************************************************
	created:	2016/07/22
	filename: 	CMD_ExtraMsg
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "../Base/FKTypeDefine.h"
#include "cocos2d.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
#pragma pack(1)
//------------------------------------------------------------
enum ENUM_ExtraDataType
{
	eExtraDataType_Null		= 0,
};
//------------------------------------------------------------
// any msg after normal msg structs 
struct SExtraData
{
	WORD							wDataSize;		// extra data size
	WORD							wDataDescribe;	// SEE ENUM_ExtraDataType
};
//------------------------------------------------------------
class CSendPacketHelper
{
protected:
	WORD							m_wDataSize;	
	WORD							m_wMaxBytes;	
	BYTE							*m_pcbBuffer;	

public:
	CSendPacketHelper(void * pcbBuffer, WORD wMaxBytes);

public:
	inline void		CleanData() { m_wDataSize=0; }
	inline WORD		GetDataSize() { return m_wDataSize; }
	inline void *	GetDataBuffer() { return m_pcbBuffer; }
public:
	inline bool		AddPacket(void * pData, WORD wDataSize, WORD wDataType);
};
//------------------------------------------------------------
class CRecvPacketHelper
{
protected:
	WORD							m_wDataPos;
	WORD							m_wDataSize;
	BYTE							*m_pcbBuffer;

public:
	CRecvPacketHelper(const void* pcbBuffer, WORD wDataSize);

public:
	inline void * GetData(SExtraData & tagExtraData);
};
//------------------------------------------------------------
#pragma pack()
