#include "FKLayerLogin.h"
#include "FKLayerLoginRes.h"
#include "FKSceneManager.h"
#include "FKSceneDefine.h"
#include "../Mgr/FKGameManager.h"
#include "../Mgr/FKTipsManager.h"
#include "../Mgr/FKMobileManager.h"
#include "../Base/FKConfigFunc.h"
#include "../Base/FKBaseMacros.h"
#include "../Base/FKStringFunc.h"
#include "../Base/FKVisiableRect.h"
#include "../Base/FKEncrpty.h"
#include "../Data/FKUserConfig.h"
#include "../Data/FKDataCenterManager.h"
#include "../Panel/FKPanelManager.h"
#include "../Panel/FKPanelMacros.h"
#include "../Event/FKEventMacros.h"
#include "../Sound/FKSoundRes.h"
#include "../Socket/FKSocketManager.h"
#include "../NetMsg/CMD_MsgDefine.h"
#include "../NetMsg/CMD_Login.h"
#include "../NetMsg/FKNetMsg.h"
#include "../Platform/FKPlatformInterfaceManager.h"
//------------------------------------------------------------
CFKLayerLogin::~CFKLayerLogin()
{
	ExitRelease();
}
//------------------------------------------------------------
bool CFKLayerLogin::init()
{
	if( !CFKLayerBase::init() )
	{
		FK_DEBUG_LOG("init fklayer error");
	}

	InitValue();
	LoadDataConfig();
	DrawView();
	AddLCC();

	CFKDataCenterManager::GetInstancePtr()->Init(Res_String_CN_SceneLogin_NewPageUrl);

	SDataLogin tagData;
	if( !CFKDataCenterManager::GetInstancePtr()->GetLoginData(0,tagData))
	{
		FK_DEBUG_LOG("Get login data error");
	}
	CFKSocketManager::GetInstancePtr()->Init( Res_String_CN_SceneLogin_LoginSvrIP, Res_String_CN_SceneLogin_LoginSvrPort );
	CFKTipsManager::GetInstancePtr()->SetScene( this );

	FK_DEBUG_LOG("LayerLogin::init successed");
	return true;
}
//------------------------------------------------------------
bool CFKLayerLogin::ExitRelease()
{
	return true;
}
//------------------------------------------------------------
void CFKLayerLogin::onEnter()
{
	CFKLayerBase::onEnter();

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(CFKLayerLogin::OnSocketDispose), EVENT_NET_TIMEOUT, NULL);
}
//------------------------------------------------------------
void CFKLayerLogin::onExit()
{
	CFKPanelManager::GetInstancePtr()->HidePanel( m_pLayout, FK_ANNOUNCE_PANEL_TAG );
	CFKLayerBase::onExit();	
}
//------------------------------------------------------------
void CFKLayerLogin::onSocketEvent(CCObject* obj)
{
	SMsgBuffer *buffer = (SMsgBuffer *)obj;
	FK_DEBUG_LOG("LayerLogin::onSocketEvent : %d %d", buffer->GetMainMsgType(), buffer->GetSubMsgType());
	
	switch( buffer->GetSubMsgType() )
	{
	case eLoginMsgType_L2C_LoginAccountResult:
		{
			CMD_L2C_LoginAccountResult* pMsg = (CMD_L2C_LoginAccountResult*)(buffer->GetBuffer());
			if( pMsg == NULL )
			{
				CFKMobileManager::GetInstancePtr()->MessageBox("",Res_String_CN_SceneLogin_LoginResultMsgBox_ErrTitle);
				return;
			}
			else if( pMsg->ucLoginResult == CMD_L2C_LoginAccountResult::eLoginResult_Failed )
			{
				CFKMobileManager::GetInstancePtr()->MessageBox( Res_String_CN_SceneLogin_LoginResultMsgBox_ErrPWorName,Res_String_CN_SceneLogin_LoginResultMsgBox_ErrTitle);
				return;
			}
			else if( pMsg->ucLoginResult == CMD_L2C_LoginAccountResult::eLoginResult_Success )
			{
				if( !FKConfigFunc::ReadBoolValue(CONFIG_KEY_USER_SAVE_PSW_SWITCH) )
				{
					m_TempDataUser.ClearPassword();
				}
				CFKDataCenterManager::GetInstancePtr()->AddUserData( m_TempDataUser );
				CFKGameManager::GetInstancePtr()->OnLoginIn( buffer, m_TempDataUser );
				return;
			}
			else if( pMsg->ucLoginResult == CMD_L2C_LoginAccountResult::eLoginResult_NeedUpdate )
			{
				int nCurVer = CFKDataCenterManager::GetInstancePtr()->GetPlazaVer();
				int nSuggestVer = pMsg->unUpdateVersion;
				char szInfo[64];
				memset(szInfo, 0, 64);
				sprintf_s( szInfo, Res_String_CN_SceneLogin_LoginResultMsgBox_UpdateVer, nCurVer, nSuggestVer );
				CFKMobileManager::GetInstancePtr()->MessageBox( std::string(szInfo).c_str(), Res_String_CN_SceneLogin_LoginResultMsgBox_NoticeTitle );
				return;
			}
		}
		break;
	default:
		CCAssert(false, "CFKLayerLogin::onSocketEvent get error msg");
		break;
	}
}
//------------------------------------------------------------
void CFKLayerLogin::onMessageBox(CCObject* obj)
{
	CFKLayerBase::onMessageBox(obj);
	SBoxPanelInfo* pInfo = (SBoxPanelInfo*)obj;
	FK_DEBUG_LOG("LayerLogin Messagebox type = %d", pInfo->m_tagClick.m_eType );
}
//------------------------------------------------------------
void CFKLayerLogin::OnSocketDispose(CCObject* obj)
{
	SSocketEvent *pEvent = (SSocketEvent *)obj;
	if (SSocketEvent::eSocketEvent_SocketMsg == pEvent->m_eEventType)
	{
		CFKBoxPanel::onShowBox(true, eBoxTypeNotify, 
			Res_String_CN_SceneLogin_LoginResultMsgBox_NoticeTitle, pEvent->m_strMsg.c_str());
	}
}
//------------------------------------------------------------
void CFKLayerLogin::onPanelMsg(CCObject* obj)
{
	if( this )
	{
		if( this->getChildByTag(FK_LOGINLAYER_ACCOUNT_EDIT_TAG) )
		{
			this->getChildByTag(FK_LOGINLAYER_ACCOUNT_EDIT_TAG)->setVisible( true );
		}
	}
	if( this )
	{
		if( this->getChildByTag(FK_LOGINLAYER_PASSWORD_EDIT_TAG) )
		{
			this->getChildByTag(FK_LOGINLAYER_PASSWORD_EDIT_TAG)->setVisible( true );
		}
	}
}
//------------------------------------------------------------
void CFKLayerLogin::LoadDataConfig()
{
	CFKDataCenterManager::GetInstancePtr()->LoadUserDataByOrder( 0, m_DataUser );
}
//------------------------------------------------------------
void CFKLayerLogin::InitValue()
{
	m_DataUser.Clear();
	m_TempDataUser.Clear();
	CFKGameManager::GetInstancePtr()->Clear();
	FKConfigFunc::SaveBoolValue( CONFIG_KEY_USER_SAVE_PSW_SWITCH, true );
}
//------------------------------------------------------------
void CFKLayerLogin::DrawView()
{
	CCSprite *pBackground = CCSprite::create(Res_Image_CN_SceneLogin_BK);
	SetViewBack(pBackground);
}
//------------------------------------------------------------
void CFKLayerLogin::AddLCC()
{
	// back btn
	static CCPoint		centePt			= CFKVisiableRect::Center();
	static CCSize		backsize		= CCSize(542, 397);
	static CCPoint		disLoginBackPt	= CCPoint(190, -100);
	static float		disy			= 60;
	static float		disx			= 120;
	static float		backcenter		= 275;

	static ccColor3B	colorFornt		= ccc3(250, 255, 24);
	static CCPoint		disAccountkPt	= CCPoint(backcenter + 58, 282);
	static CCPoint		disPswPt		= CCPoint(backcenter + 58, 170);
	static CCSize		szEditSize		= CCSize(336, 52);
	static float		fontSize		= 36;

	FKCW_UIWidget_Layout* pLoginBack	= FKCW_UIWidget_Layout::create(backsize);
	if (pLoginBack)
	{
		pLoginBack->setPosition(centePt + disLoginBackPt);
		pLoginBack->setBackgroundImage(Res_Image_CN_SceneLogin_LoginPanelBK);
		m_pLayout->addChild(pLoginBack, 0, FK_LOGINLAYER_LOGINBACK_LAYOUT_TAG);
	}

	// add btn
	FKCW_UIWidget_Button *pBtnLogin		= FKCW_UIWidget_Button::create(
		Res_Image_CN_SceneLogin_BtnLogin_Normal, Res_Image_CN_SceneLogin_BtnLogin_Down);
	FKCW_UIWidget_Button *pBtnReg		= FKCW_UIWidget_Button::create(
		Res_Image_CN_SceneLogin_BtnReg_Normal, Res_Image_CN_SceneLogin_BtnReg_Down);
	FKCW_UIWidget_Button *pQuit			= FKCW_UIWidget_Button::createWithSpriteFrameName(
		Res_Image_CN_SceneLogin_BtnClose_Normal, Res_Image_CN_SceneLogin_BtnClose_Down);
	pBtnLogin->setPosition(CCPoint(backcenter + disx, disy)); 
	pBtnLogin->setOnClickListener(this, fkcw_uiwidget_click_selector(CFKLayerLogin::BtnLogin));
	pBtnReg->setPosition(CCPoint(backcenter - disx, disy)); 
	pBtnReg->setOnClickListener(this, fkcw_uiwidget_click_selector(CFKLayerLogin::BtnReg));
	pQuit->setPosition(CFKVisiableRect::RightTop() + CCPoint(-50, -50)); 
	pQuit->setOnClickListener(this, fkcw_uiwidget_click_selector(CFKLayerLogin::BtnQuit));
	pLoginBack->addChild(pBtnLogin);
	pLoginBack->addChild(pBtnReg);
	m_pLayout->addChild(pQuit);

	CCEditBox* pAccoutEditCtrl = CCEditBox::create(szEditSize, 
		CCScale9Sprite::createWithSpriteFrameName(Res_Image_CN_SceneLogin_EditBox));
	pAccoutEditCtrl->setFontSize(32);
	pAccoutEditCtrl->setFontColor(colorFornt);
	pAccoutEditCtrl->setPlaceHolder(FKStringFunc::GBK2UTF8(Res_String_CN_SceneLogin_InsertAccout).c_str());
	pAccoutEditCtrl->setPlaceholderFontColor(ccORANGE);
	pAccoutEditCtrl->setMaxLength(MAX_USER_NAME_LENGTH);
	pAccoutEditCtrl->setReturnType(kKeyboardReturnTypeDone);
	pAccoutEditCtrl->setInputFlag(kEditBoxInputFlagInitialCapsAllCharacters);
	pAccoutEditCtrl->setInputMode(kEditBoxInputModeSingleLine);
	pAccoutEditCtrl->setPosition(disAccountkPt);
	pAccoutEditCtrl->setTag(FK_LOGINLAYER_ACCOUNT_EDIT_TAG);
	if (m_DataUser.m_strAccount.size() > 0)
	{
		pAccoutEditCtrl->setText(m_DataUser.m_strAccount.c_str());
	}
	pLoginBack->addChild(pAccoutEditCtrl, eLayerZOrderType100 + 2);

	CCEditBox* pPasswordEditCtrl = CCEditBox::create(szEditSize, 
		CCScale9Sprite::createWithSpriteFrameName(Res_Image_CN_SceneLogin_EditBox));
	pPasswordEditCtrl->setFontSize(32);
	pPasswordEditCtrl->setFontColor(colorFornt);
	pPasswordEditCtrl->setPlaceHolder(FKStringFunc::GBK2UTF8(Res_String_CN_SceneLogin_InsertPassword).c_str());
	pPasswordEditCtrl->setPlaceholderFontColor(ccORANGE);
	pPasswordEditCtrl->setMaxLength(MAX_USER_PASSWORD_LENGTH);
	pPasswordEditCtrl->setReturnType(kKeyboardReturnTypeDone);
	pPasswordEditCtrl->setInputFlag(kEditBoxInputFlagPassword);
	pPasswordEditCtrl->setInputMode(kEditBoxInputModeSingleLine);
	pPasswordEditCtrl->setPosition(disPswPt);
	pPasswordEditCtrl->setTag(FK_LOGINLAYER_PASSWORD_EDIT_TAG);
	if (m_DataUser.m_strPassword.size() > 0)
	{
		pPasswordEditCtrl->setText(m_DataUser.m_strPassword.c_str());
	}
	pLoginBack->addChild(pPasswordEditCtrl, eLayerZOrderType100 + 2);

	//////////////////////////////////////////////////////////////////////////
	{	// registe panel
		static CCPoint disRegAccPt		= CCPoint(backcenter + 68, 370);
		static CCPoint disRegNamePt		= CCPoint(backcenter + 68, 285);
		static CCPoint disRegPsw1Pt		= CCPoint(backcenter + 68, 200);
		static CCPoint disRegPsw2Pt		= CCPoint(backcenter + 68, 125);
		static CCPoint disCheckBtnPt	= CCPoint(backcenter - 100, 150);

		FKCW_UIWidget_Layout* pRegBack = FKCW_UIWidget_Layout::create(backsize);
		if (pRegBack)
		{
			pRegBack->setBackgroundImage(Res_Image_CN_SceneLogin_RegPanelBK);
			pRegBack->setPosition(centePt + disLoginBackPt + CCPoint(0, 20));
			m_pLayout->addChild(pRegBack, 0, FK_LOGINLAYER_REG_LAYOUT_TAG);
			if( pRegBack )
			{
				pRegBack->setVisible( false );
			}
		}

		FKCW_UIWidget_Button *pBtnOk = FKCW_UIWidget_Button::create(
			Res_Image_CN_SceneLogin_BtnOK_Normal, Res_Image_CN_SceneLogin_BtnOK_Down);
		pBtnOk->setPosition(CCPoint(backcenter - disx, disy-20)); 
		pBtnOk->setOnClickListener(this, fkcw_uiwidget_click_selector(CFKLayerLogin::BtnOK));
		FKCW_UIWidget_Button *pBtnReturn = FKCW_UIWidget_Button::create(
			Res_Image_CN_SceneLogin_BtnReturn_Normal, Res_Image_CN_SceneLogin_BtnReturn_Down);
		pBtnReturn->setPosition(CCPoint(backcenter + disx, disy-20)); 
		pBtnReturn->setOnClickListener(this, fkcw_uiwidget_click_selector(CFKLayerLogin::BtnBack));

		pRegBack->addChild(pBtnOk);
		pRegBack->addChild(pBtnReturn);

		CCEditBox* pRegAccoutEditCtrl = CCEditBox::create(
			szEditSize, CCScale9Sprite::createWithSpriteFrameName(Res_Image_CN_SceneLogin_EditBox));
		pRegAccoutEditCtrl->setFontSize(fontSize);
		pRegAccoutEditCtrl->setFontColor(colorFornt);
		pRegAccoutEditCtrl->setPlaceHolder(FKStringFunc::GBK2UTF8(Res_String_CN_SceneLogin_RegAccout).c_str());
		pRegAccoutEditCtrl->setPlaceholderFontColor(colorFornt);
		pRegAccoutEditCtrl->setMaxLength(MAX_USER_NAME_LENGTH);
		pRegAccoutEditCtrl->setReturnType(kKeyboardReturnTypeDone);
		pRegAccoutEditCtrl->setInputFlag(kEditBoxInputFlagInitialCapsAllCharacters);
		pRegAccoutEditCtrl->setInputMode(kEditBoxInputModeSingleLine);
		pRegAccoutEditCtrl->setPosition(disRegAccPt);
		pRegAccoutEditCtrl->setTag(FK_LOGINLAYER_REGACCOUNT_EDIT_TAG);
		pRegBack->addChild(pRegAccoutEditCtrl, eLayerZOrderType100 + 2);

		CCEditBox* pRegNickNameEditCtrl = CCEditBox::create(szEditSize, 
			CCScale9Sprite::createWithSpriteFrameName(Res_Image_CN_SceneLogin_EditBox));
		pRegNickNameEditCtrl->setFontSize(fontSize);
		pRegNickNameEditCtrl->setFontColor(colorFornt);
		pRegNickNameEditCtrl->setPlaceHolder(FKStringFunc::GBK2UTF8(Res_String_CN_SceneLogin_RegNickName).c_str());
		pRegNickNameEditCtrl->setPlaceholderFontColor(colorFornt);
		pRegNickNameEditCtrl->setMaxLength(MAX_USER_NAME_LENGTH);
		pRegNickNameEditCtrl->setReturnType(kKeyboardReturnTypeDone);
		pRegNickNameEditCtrl->setInputFlag(kEditBoxInputFlagInitialCapsAllCharacters);
		pRegNickNameEditCtrl->setInputMode(kEditBoxInputModeSingleLine);
		pRegNickNameEditCtrl->setPosition(disRegNamePt);
		pRegNickNameEditCtrl->setTag(FK_LOGINLAYER_REGNICKNAME_EDIT_TAG);
		pRegBack->addChild(pRegNickNameEditCtrl, eLayerZOrderType100 + 2);

		CCEditBox* pRegPasswordEditCtrl = CCEditBox::create(szEditSize, 
			CCScale9Sprite::createWithSpriteFrameName(Res_Image_CN_SceneLogin_EditBox));
		pRegPasswordEditCtrl->setFontSize(fontSize);
		pRegPasswordEditCtrl->setFontColor(colorFornt);
		pRegPasswordEditCtrl->setPlaceHolder(FKStringFunc::GBK2UTF8(Res_String_CN_SceneLogin_RegPassword).c_str());
		pRegPasswordEditCtrl->setPlaceholderFontColor(colorFornt);
		pRegPasswordEditCtrl->setMaxLength(MAX_USER_PASSWORD_LENGTH);
		pRegPasswordEditCtrl->setReturnType(kKeyboardReturnTypeDone);
		pRegPasswordEditCtrl->setInputFlag(kEditBoxInputFlagPassword);
		pRegPasswordEditCtrl->setInputMode(kEditBoxInputModeSingleLine);
		pRegPasswordEditCtrl->setPosition(disRegPsw1Pt);
		pRegPasswordEditCtrl->setTag(FK_LOGINLAYER_REGPASSWORD_EDIT_TAG);
		pRegBack->addChild(pRegPasswordEditCtrl, eLayerZOrderType100 + 2);

		CCEditBox* pRegSubPasswordEditCtrl = CCEditBox::create(
			szEditSize, CCScale9Sprite::createWithSpriteFrameName(Res_Image_CN_SceneLogin_EditBox));
		pRegSubPasswordEditCtrl->setFontSize(fontSize);
		pRegSubPasswordEditCtrl->setFontColor(colorFornt);
		pRegSubPasswordEditCtrl->setPlaceHolder(FKStringFunc::GBK2UTF8(Res_String_CN_SceneLogin_RegSubPassword).c_str());
		pRegSubPasswordEditCtrl->setPlaceholderFontColor(colorFornt);
		pRegSubPasswordEditCtrl->setMaxLength(MAX_USER_PASSWORD_LENGTH);
		pRegSubPasswordEditCtrl->setReturnType(kKeyboardReturnTypeDone);
		pRegSubPasswordEditCtrl->setInputFlag(kEditBoxInputFlagPassword);
		pRegSubPasswordEditCtrl->setInputMode(kEditBoxInputModeSingleLine);
		pRegSubPasswordEditCtrl->setPosition(disRegPsw2Pt);
		pRegSubPasswordEditCtrl->setTag(FK_LOGINLAYER_REGSUBPASSWORD_EDIT_TAG);
		pRegBack->addChild(pRegSubPasswordEditCtrl, eLayerZOrderType100 + 2);
	}
	//////////////////////////////////////////////////////////////////////////
	{ // snow particle
		CCParticleSystem* pSnowParticleEmitter;
		pSnowParticleEmitter = CCParticleSnow::create();
		pSnowParticleEmitter->retain();

		pSnowParticleEmitter->setLife(4);
		pSnowParticleEmitter->setLifeVar(1);
		pSnowParticleEmitter->setGravity(CCPoint(0,-10));
		pSnowParticleEmitter->setSpeed(130);
		pSnowParticleEmitter->setSpeedVar(30);

		ccColor4F startColor = pSnowParticleEmitter->getStartColor();
		startColor.r = 0.9f;
		startColor.g = 0.9f;
		startColor.b = 0.9f;
		pSnowParticleEmitter->setStartColor(startColor);

		ccColor4F startColorVar = pSnowParticleEmitter->getStartColorVar();
		startColorVar.b = 0.1f;
		pSnowParticleEmitter->setStartColorVar(startColorVar);
		pSnowParticleEmitter->setTexture( 
			CCTextureCache::sharedTextureCache()->addImage(Res_Image_SceneLogin_SnowParticle) );

		CCSize s = CCDirector::sharedDirector()->getWinSize();
		if (pSnowParticleEmitter != NULL)
		{
			pSnowParticleEmitter->setPosition( CCPoint(s.width / 2, s.height) );
		}
		addChild(pSnowParticleEmitter, eLayerZOrderTypeMax); 
	}
}
//------------------------------------------------------------
void CFKLayerLogin::BtnLogin(CCObject* pSender)
{
	CCEditBox *pPsw = (CCEditBox*)m_pLayout->getChildByTag(FK_LOGINLAYER_LOGINBACK_LAYOUT_TAG)->getChildByTag(FK_LOGINLAYER_PASSWORD_EDIT_TAG);
	CCEditBox *pName = (CCEditBox*)m_pLayout->getChildByTag(FK_LOGINLAYER_LOGINBACK_LAYOUT_TAG)->getChildByTag(FK_LOGINLAYER_ACCOUNT_EDIT_TAG);
	if (pName && pPsw)
	{
		UpdateEdit(FK_LOGINLAYER_ACCOUNT_EDIT_TAG, pName->getText());
		UpdateEdit(FK_LOGINLAYER_PASSWORD_EDIT_TAG, pPsw->getText());
	}

	OnLogin(m_DataUser);
	CFKMobileManager::GetInstancePtr()->PlayEffect(Res_Sound_BtnClick);
}
//------------------------------------------------------------
void CFKLayerLogin::BtnReg(CCObject* pSender)
{
	if( m_pLayout )
	{
		if( m_pLayout->getChildByTag(FK_LOGINLAYER_LOGINBACK_LAYOUT_TAG) )
		{
			m_pLayout->getChildByTag(FK_LOGINLAYER_LOGINBACK_LAYOUT_TAG)->setVisible( false );
		}
	}
	if( m_pLayout )
	{
		if( m_pLayout->getChildByTag(FK_LOGINLAYER_REG_LAYOUT_TAG) )
		{
			m_pLayout->getChildByTag(FK_LOGINLAYER_REG_LAYOUT_TAG)->setVisible( true );
		}
	}

	CFKMobileManager::GetInstancePtr()->PlayEffect(Res_Sound_BtnClick);
}
//------------------------------------------------------------
void CFKLayerLogin::BtnBack(CCObject* pSender)
{
	if( m_pLayout )
	{
		if( m_pLayout->getChildByTag(FK_LOGINLAYER_LOGINBACK_LAYOUT_TAG) )
		{
			m_pLayout->getChildByTag(FK_LOGINLAYER_LOGINBACK_LAYOUT_TAG)->setVisible( true );
		}
	}
	if( m_pLayout )
	{
		if( m_pLayout->getChildByTag(FK_LOGINLAYER_REG_LAYOUT_TAG) )
		{
			m_pLayout->getChildByTag(FK_LOGINLAYER_REG_LAYOUT_TAG)->setVisible( false );
		}
	}

	CFKMobileManager::GetInstancePtr()->PlayEffect(Res_Sound_BtnClick);
}
//------------------------------------------------------------
void CFKLayerLogin::BtnOK(CCObject* pSender)
{
	CCNode*		pbg = m_pLayout->getChildByTag(FK_LOGINLAYER_REG_LAYOUT_TAG);
	if( pbg == NULL ) return;
	CCEditBox *	pPsw1 = (CCEditBox*)pbg->getChildByTag(FK_LOGINLAYER_REGPASSWORD_EDIT_TAG);
	CCEditBox *	pPsw2 = (CCEditBox*)pbg->getChildByTag(FK_LOGINLAYER_REGSUBPASSWORD_EDIT_TAG);
	CCEditBox *	pName = (CCEditBox*)pbg->getChildByTag(FK_LOGINLAYER_REGNICKNAME_EDIT_TAG);
	CCEditBox *	pAcc = (CCEditBox*)pbg->getChildByTag(FK_LOGINLAYER_REGACCOUNT_EDIT_TAG);

	SDataUser temp;
	if (pName && pPsw1 && pPsw2 && pAcc)
	{
		temp.m_strAccount	= pAcc->getText();
		temp.m_strPassword	= pPsw1->getText();

		if (temp.m_strAccount.length() < MIN_USER_NAME_LENGTH ||
			temp.m_strAccount.length() > MAX_USER_NAME_LENGTH)
		{
			CFKMobileManager::GetInstancePtr()->MessageBox( Res_String_CN_SceneLogin_RegMsgbox_ErrName, Res_String_CN_SceneLogin_RegMsgbox_Title);
			return;
		}
		if (temp.m_strPassword.length() < MIN_USER_PASSWORD_LENGTH || 
			temp.m_strPassword.length() > MAX_USER_PASSWORD_LENGTH)
		{
			CFKMobileManager::GetInstancePtr()->MessageBox( Res_String_CN_SceneLogin_RegMsgbox_ErrPW, Res_String_CN_SceneLogin_RegMsgbox_Title);
			return;
		}

		FKEncrypt::FKMD5EncryptData(temp.m_strPassword.c_str(), temp.m_szMD5Password);
		strcpy(temp.m_szMachineID, CFKPlatformInterfaceManager::GetInstancePtr()->GetDeviceID().c_str());

		string strName = pName->getText();
		string strBankPsw = pPsw2->getText();
		FK_DEBUG_LOG("Registe account=%s name=%s", pAcc->getText() ,pName->getText());
		OnReg(temp, strName, strBankPsw);
	}
	CFKMobileManager::GetInstancePtr()->PlayEffect(Res_Sound_BtnClick);
}
//------------------------------------------------------------
void CFKLayerLogin::BtnCheckPsw(CCObject* pSender)
{
	FKCW_UIWidget_CheckBox* pCheck = (FKCW_UIWidget_CheckBox*)pSender;
	if (pCheck)
	{
		bool bCheck = pCheck->isChecked();
		FKConfigFunc::SaveBoolValue(CONFIG_KEY_USER_SAVE_PSW_SWITCH, bCheck);
	}
}
//------------------------------------------------------------
void CFKLayerLogin::BtnQuit(CCObject* pSender)
{
	CFKMobileManager::GetInstancePtr()->QuitApp();
}
//------------------------------------------------------------
void CFKLayerLogin::BtnGongGao(CCObject* pSender)
{
	CFKPanelManager::GetInstancePtr()->ShowPanel(m_pLayout, FK_ANNOUNCE_PANEL_TAG);
}
//------------------------------------------------------------
void CFKLayerLogin::OnLogin(SDataUser user)
{
	if (user.m_strAccount.length() < MIN_USER_NAME_LENGTH || 
		user.m_strPassword.length() < MIN_USER_PASSWORD_LENGTH)
	{
		CFKMobileManager::GetInstancePtr()->MessageBox(Res_String_CN_SceneLogin_LoginMsgbox_ErrPWorName, Res_String_CN_SceneLogin_LoginMsgbox_Title);
		return;
	}

	m_TempDataUser = user;
	std::string strNewPsw = user.m_strPassword + CFKDataCenterManager::GetInstancePtr()->GetPswKey();
	FKEncrypt::FKMD5EncryptData(strNewPsw, user.m_szMD5Password);
	FKNetMsg::Send_C2L_UserLoginRequest( user, CFKDataCenterManager::GetInstancePtr()->GetPlazaVer(),
		CFKPlatformInterfaceManager::GetInstancePtr()->GetDeviceID());

	FK_DEBUG_LOG("LayerLogin::onLogin"); 
}
//------------------------------------------------------------
void CFKLayerLogin::OnReg(SDataUser user, string strNickName, string strSubPsw)
{
	char szMd5SubPsw[USER_PASSWORD_MD5_LENGTH];
	FKEncrypt::FKMD5EncryptData(strSubPsw, szMd5SubPsw);
	FKNetMsg::Send_C2L_RegisterAccountRequest( user, CFKDataCenterManager::GetInstancePtr()->GetPlazaVer(),
		szMd5SubPsw, strNickName );
	m_TempDataUser = user;

	FK_DEBUG_LOG("LayerLogin::OnReg"); 
}
//------------------------------------------------------------
void CFKLayerLogin::GoSite(CCObject* pSender)
{
	CFKSceneManager::GetInstancePtr()->ChangeScene( eSceneID_Login, eSceneID_Site, eSceneTransType_CrossFade );
}
//------------------------------------------------------------
void CFKLayerLogin::UpdateEdit(int flag, string strText)
{
	switch (flag)
	{
	case FK_LOGINLAYER_ACCOUNT_EDIT_TAG:
	case FK_LOGINLAYER_REGNICKNAME_EDIT_TAG:
		{
			m_DataUser.m_strAccount = strText;
		}
		break;
	case  FK_LOGINLAYER_PASSWORD_EDIT_TAG:
		{
			m_DataUser.m_strPassword = strText;
		}
		break;
	default:
		{
			FK_DEBUG_LOG("UpdateEdit err flag=%d text=%s", flag, strText.c_str());
		}
		break;
	}
}
//------------------------------------------------------------