#include "FKBoxPanel.h"
#include "FKPanelMacros.h"
#include "FKBoxPanelRes.h"
#include "../Base/FKVisiableRect.h"
#include "../Base/FKStringFunc.h"
#include "../Event/FKEventMacros.h"
#include "../Base/FKBaseMacros.h"
//------------------------------------------------------------
CFKBoxPanel::CFKBoxPanel(void)
	:m_pTitle(NULL)
	,m_pPanel(NULL)
	,m_pText(NULL)
	,m_pBtnOK(NULL)
	,m_pBtnCancel(NULL)
	,m_pBox(NULL)
	,m_idx(eBoxTypeNull)
	,m_bShow(false)
	,m_bShowCancel(false)
{
	m_vecBoxPanelList.clear();
}
//------------------------------------------------------------
CFKBoxPanel::~CFKBoxPanel(void)
{
}
//------------------------------------------------------------
CFKBoxPanel* CFKBoxPanel::createBox(enumBoxType idx, const char* pTitle, const char* pText, bool bShowCancel)
{
	CFKBoxPanel *pBox = new CFKBoxPanel();
	if (pBox && pBox->creatLCC() && pBox->init(idx, pTitle, pText, bShowCancel))
	{
		return pBox;
	}
	else
	{
		delete pBox;
		pBox = NULL;
		return NULL;
	}
}
//------------------------------------------------------------
bool CFKBoxPanel::creatLCC()
{
	if (false == CFKNodePanel::init())
	{
		return false;
	}

	m_pTitle		= FKCW_UIWidget_Label::create("", "", FK_DEFAULT_BOXPANEL_TITLE_FONT_SIZE); 
	m_pTitle->setColor( FK_DEFAULT_BOXPANEL_TITLE_FONT_COLOR );
	m_pText			= FKCW_UIWidget_Label::create("", "", FK_DEFAULT_BOXPANEL_TEXT_FONT_SIZE); 
	m_pText->setColor( FK_DEFAULT_BOXPANEL_TEXT_FONT_COLOR );
	m_pBtnOK		= FKCW_UIWidget_Button::create(Res_Image_CN_BoxPanel_OKBtn_Normal, Res_Image_CN_BoxPanel_OKBtn_Down);
	m_pBtnCancel	= FKCW_UIWidget_Button::create(Res_Image_CN_BoxPanel_CancelBtn_Normal, Res_Image_CN_BoxPanel_CancelBtn_Down);
	m_pBtnOK->setOnClickListener(this, fkcw_uiwidget_click_selector(CFKBoxPanel::onBtnCheck));
	m_pBtnCancel->setOnClickListener(this, fkcw_uiwidget_click_selector(CFKBoxPanel::onBtnCheck));
	m_pBtnOK->setUserTag(FK_BOXPANEL_OK_BTN_TAG);
	m_pBtnCancel->setUserTag(FK_BOXPANEL_CANCEL_BTN_TAG);

	m_pPanel = FKCW_UIWidget_Layout::create();
	m_pPanel->setPosition(CFKVisiableRect::Center());
	m_pBox = FKCW_UIWidget_Scale9Sprite::create(Res_Image_CN_BoxPanel_BackgroundBox);
	m_pPanel->addChild(m_pBox);

	m_pBox->addChild(m_pTitle);
	m_pBox->addChild(m_pText);
	m_pPanel->addChild(m_pBtnOK);
	m_pPanel->addChild(m_pBtnCancel);

	addChild(m_pPanel);
	m_vecBoxPanelList.clear();

	setCloseBtnPos(CCPointZero, false);

	return true;
}
//------------------------------------------------------------
bool CFKBoxPanel::init(enumBoxType idx, const char* pTitle, const char* pText, bool bShowCancel)
{
	if (m_bShow)
	{
		SBoxPanelInfo pak;
		pak.m_eOperateID = eBoxOperateIDShow;
		pak.m_tagShow.m_eType = idx;
		pak.m_tagShow.m_strText = pText;
		pak.m_tagShow.m_strTitle = pTitle;
		pak.m_tagShow.m_bIsShowCancel = bShowCancel;

		m_vecBoxPanelList.push_back(pak);
		return true;
	}
	m_idx = idx;
	m_bShowCancel = bShowCancel;

	m_pTitle->setString(FKStringFunc::GBK2UTF8(pTitle).c_str());
	m_pText->setString(FKStringFunc::GBK2UTF8(pText).c_str());

	drawView();
	setLCC();
	m_bShow = true;

	return true;
}
//------------------------------------------------------------
bool CFKBoxPanel::drawView()
{
	return true;
}
//------------------------------------------------------------
bool CFKBoxPanel::addLCC()
{
	return true;
}
//------------------------------------------------------------
bool CFKBoxPanel::setLCC()
{
	CCSize tagDefaultPanelSize = FK_BOXPANEL_MIN_SIZE;
	CCSize szText = m_pText->getContentSize();
	if (szText.width + FK_DEFAULT_BOXPANEL_BOX_X> tagDefaultPanelSize.width)
	{
		tagDefaultPanelSize.width = szText.width + FK_DEFAULT_BOXPANEL_BOX_X;
	}
	if (szText.height + FK_DEFAULT_BOXPANEL_BOX_Y> tagDefaultPanelSize.height)
	{
		tagDefaultPanelSize.height = szText.height + FK_DEFAULT_BOXPANEL_BOX_Y;
	}

	// panel back
	m_pPanel->setContentSize(tagDefaultPanelSize);

	m_pBox->setContentSize(tagDefaultPanelSize);
	m_pBox->setPosition(tagDefaultPanelSize.width / 2, tagDefaultPanelSize.height / 2);

	if (m_pTitle && m_pText)
	{
		m_pTitle->setPosition(CCPoint(tagDefaultPanelSize.width / 2, 
			tagDefaultPanelSize.height - FK_DEFAULT_BOXPANEL_TITLE_Y_DISTANCE));
		m_pText->setPosition(CCPoint(tagDefaultPanelSize.width / 2, 
			tagDefaultPanelSize.height / 2 - FK_DEFAULT_BOXPANEL_TEXT_Y_DISTANCE));
	}

	if (m_bShowCancel)
	{
		m_pBtnOK->setPosition(CCPoint(tagDefaultPanelSize.width / 2 - FK_DEFAULT_BOXPANEL_BTN_X_DISTANCE, 
			FK_DEFAULT_BOXPANEL_BTN_Y_DISTANCE));
		m_pBtnCancel->setPosition(CCPoint(tagDefaultPanelSize.width / 2 + FK_DEFAULT_BOXPANEL_BTN_X_DISTANCE, 
			FK_DEFAULT_BOXPANEL_BTN_Y_DISTANCE));
		m_pBtnOK->setVisible(true);
		m_pBtnCancel->setVisible(true);
	}
	else
	{
		m_pBtnOK->setPosition(CCPoint(tagDefaultPanelSize.width / 2, FK_DEFAULT_BOXPANEL_BTN_Y_DISTANCE));
		m_pBtnCancel->setVisible(false);
	}

	return true;
}
//------------------------------------------------------------
void CFKBoxPanel::onBtnCheck(CCObject* pSender)
{
	FKCW_UIWidget_Button* pBtn = (FKCW_UIWidget_Button*)pSender;
	FK_DEBUG_LOG("BoxPanel::onBtnCheck tag = %d", pBtn->getUserTag());

	if (FK_BOXPANEL_OK_BTN_TAG == pBtn->getUserTag())
	{
		onSendMsg(true);
	}

	if (FK_BOXPANEL_CANCEL_BTN_TAG == pBtn->getUserTag())
	{
		onSendMsg(false);
	}
}
//------------------------------------------------------------
void CFKBoxPanel::onSendMsg(bool bOK)
{
	SBoxPanelInfo tag;
	tag.m_eOperateID			= eBoxOperateIDClick;
	tag.m_tagClick.m_eType		= m_idx;
	tag.m_tagClick.m_bIsOkBtn	= bOK;

	CCNotificationCenter::sharedNotificationCenter()->postNotification( EVENT_MSGBOX, (CCObject*)&tag);

	showPanel(false);
}
//------------------------------------------------------------
void CFKBoxPanel::showPanel(bool bShow)
{
	if (false == bShow && m_vecBoxPanelList.size() > 0)
	{
		SBoxPanelInfo pak = m_vecBoxPanelList.back();
		m_vecBoxPanelList.pop_back();
		m_bShow = false;
		init(pak.m_tagShow.m_eType, 
			pak.m_tagShow.m_strTitle.c_str(), 
			pak.m_tagShow.m_strText.c_str(), 
			pak.m_tagShow.m_bIsShowCancel);

		return;
	}
	this->setVisible(bShow);
	setTouchEnabled(bShow);

	m_bShow = bShow;
}
//------------------------------------------------------------
void CFKBoxPanel::onShowBox(bool bShow, enumBoxType idx, const char* pTitle, 
	const char* pText, bool bShowCancel)
{
	SBoxPanelInfo tag;
	tag.m_eOperateID = bShow ? eBoxOperateIDShow : eBoxOperateIDHide;
	tag.m_tagShow.m_eType = idx;
	tag.m_tagShow.m_strText = pText;
	tag.m_tagShow.m_strTitle = pTitle;
	tag.m_tagShow.m_bIsShowCancel = bShowCancel;

	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_MSGBOX, (CCObject*)&tag);
}
//------------------------------------------------------------