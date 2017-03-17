#include "CMD_ExtraMsg.h"
//------------------------------------------------------------
CSendPacketHelper::CSendPacketHelper(void * pcbBuffer, WORD wMaxBytes)
{
	m_wDataSize	= 0;
	m_wMaxBytes	= wMaxBytes;
	m_pcbBuffer	= (BYTE *)pcbBuffer;

	return;
}
//------------------------------------------------------------
bool CSendPacketHelper::AddPacket(void * pData, WORD wDataSize, WORD wDataType)
{
	// check params
	CC_ASSERT(wDataType != (WORD)eExtraDataType_Null);
	CC_ASSERT((wDataSize+sizeof(SExtraData)+m_wDataSize) <= m_wMaxBytes);
	if ((wDataSize+sizeof(SExtraData)+m_wDataSize) > m_wMaxBytes) 
		return false;

	// insert data
	CC_ASSERT(m_pcbBuffer!=NULL);
	SExtraData * pDataDescribe		= (SExtraData *)(m_pcbBuffer+m_wDataSize);
	pDataDescribe->wDataSize		= wDataSize;
	pDataDescribe->wDataDescribe	= wDataType;
	if (wDataSize>0)
	{
		CC_ASSERT(pData!=NULL);
		memcpy(pDataDescribe+1,pData,wDataSize);
	}
	m_wDataSize += sizeof(SExtraData) + wDataSize;

	return true;
}
//------------------------------------------------------------
CRecvPacketHelper::CRecvPacketHelper(const void* pcbBuffer, WORD wDataSize)
{
	m_wDataPos		= 0;
	m_wDataSize		= wDataSize;
	m_pcbBuffer		= (BYTE *)pcbBuffer;
	return;
}
//------------------------------------------------------------
void * CRecvPacketHelper::GetData(SExtraData & DataDescribe)
{
	if (m_wDataPos>=m_wDataSize) 
	{
		CC_ASSERT(m_wDataPos==m_wDataSize);
		DataDescribe.wDataSize=0;
		DataDescribe.wDataDescribe=eExtraDataType_Null;
		return NULL;
	}

	// get data
	CC_ASSERT((m_wDataPos+sizeof(SExtraData))<=m_wDataSize);
	memcpy(&DataDescribe,m_pcbBuffer+m_wDataPos,sizeof(SExtraData));
	CC_ASSERT((m_wDataPos+sizeof(SExtraData)+DataDescribe.wDataSize)<=m_wDataSize);

	// check data
	if ((m_wDataPos+sizeof(SExtraData)+DataDescribe.wDataSize)>m_wDataSize)
	{
		DataDescribe.wDataSize=0;
		DataDescribe.wDataDescribe=eExtraDataType_Null;
		return NULL;
	}

	// set data
	void * pData=NULL;
	if (DataDescribe.wDataSize>0)
	{
		pData=m_pcbBuffer+m_wDataPos+sizeof(SExtraData);
	}
	m_wDataPos+=sizeof(SExtraData)+DataDescribe.wDataSize;

	return pData;
};
//------------------------------------------------------------