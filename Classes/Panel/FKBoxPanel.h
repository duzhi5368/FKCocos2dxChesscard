/********************************************************************
	created:	2016/07/16
	filename: 	FKBoxPanel
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "FKNodePanel.h"
//------------------------------------------------------------
enum enumBoxType
{
	eBoxTypeNull		= 0,
	eBoxTypeSocket,
	eBoxTypeEnterRule,
	eBoxTypeNotify,
	eBoxTypeQuit,
};
//------------------------------------------------------------
enum enumBoxOperateID
{
	eBoxOperateIDNull	= 0,
	eBoxOperateIDShow,
	eBoxOperateIDHide,
	eBoxOperateIDClick,
};
//------------------------------------------------------------
struct SBoxBackgroundInfo
{
	enumBoxType	m_eType;
	string		m_strTitle;
	string		m_strText;
	bool		m_bIsShowCancel;

	SBoxBackgroundInfo()
	{
		Clear();
	}
	void Clear()
	{
		m_eType = eBoxTypeNull;
		m_strText.clear();
		m_strTitle.clear();
		m_bIsShowCancel = false;
	}
};
//------------------------------------------------------------
struct SBoxBtnInfo
{
	enumBoxType	m_eType;
	bool		m_bIsOkBtn;
	string		m_strText;

	SBoxBtnInfo()
	{
		Clear();
	}
	void Clear()
	{
		m_eType = eBoxTypeNull;
		m_bIsOkBtn = false;
		m_strText.clear();
	}
};
//------------------------------------------------------------
struct SBoxPanelInfo
{
	enumBoxOperateID	m_eOperateID;
	SBoxBackgroundInfo	m_tagShow;
	SBoxBtnInfo			m_tagClick;

	SBoxPanelInfo()
	{
		Clear();
	}
	void Clear()
	{
		m_eOperateID = eBoxOperateIDNull;
		m_tagClick.Clear();
		m_tagShow.Clear();
	}
};
//------------------------------------------------------------
class CFKBoxPanel : public CFKNodePanel
{
public:
	CFKBoxPanel(void);
	~CFKBoxPanel(void);
	static void onShowBox(bool bShow, enumBoxType idx, const char* pTitle, const char* pText, bool bShowCancel = false);
	static CFKBoxPanel* createBox(enumBoxType idx, const char* pTitle, const char* pText, bool bShowCancel = false);

public:
	virtual bool creatLCC();
	virtual bool init(enumBoxType idx, const char* pTitle, const char* pText, bool bShowCancel);
	virtual bool setLCC();
	virtual bool drawView();
	virtual bool addLCC();

public:
	virtual void onBtnCheck(CCObject* pSender);
	virtual void onSendMsg(bool bOK);
	virtual void showPanel(bool bShow);

private:
	bool							m_bShow;
	bool							m_bShowCancel;
	FKCW_UIWidget_Layout *			m_pPanel;
	FKCW_UIWidget_Label *			m_pTitle;
	FKCW_UIWidget_Label *			m_pText;
	FKCW_UIWidget_Button*			m_pBtnOK;
	FKCW_UIWidget_Button*			m_pBtnCancel;
	FKCW_UIWidget_Scale9Sprite *	m_pBox;
	enumBoxType						m_idx;

	std::vector<SBoxPanelInfo>		m_vecBoxPanelList;
};
