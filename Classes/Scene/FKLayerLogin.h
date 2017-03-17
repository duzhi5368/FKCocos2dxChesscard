/********************************************************************
	created:	2016/07/18
	filename: 	FKLayerLogin
	author:		FreeKnight
	
	purpose:	
*********************************************************************/
#pragma once
//------------------------------------------------------------
#include "cocos2d.h"
#include "FKLayerBase.h"
#include "../Data/FKUserData.h"
//------------------------------------------------------------
USING_NS_CC;
//------------------------------------------------------------
class CFKLayerLogin : public CFKLayerBase
{
public:
	CREATE_FUNC(CFKLayerLogin);
	~CFKLayerLogin();
public:
	virtual bool		init();
	virtual bool		ExitRelease();

	virtual void		onEnter();
	virtual void		onExit();

	virtual void		onSocketEvent(CCObject* obj);
	virtual void		onMessageBox(CCObject* obj);
	virtual void		OnSocketDispose(CCObject* obj);
	virtual void		onPanelMsg(CCObject* obj);
public:
	void				LoadDataConfig();
	void				InitValue();
	void				DrawView();
	void				AddLCC();
public:
	void				BtnLogin(CCObject* pSender);
	void				BtnReg(CCObject* pSender);
	void				BtnBack(CCObject* pSender);
	void				BtnOK(CCObject* pSender);
	void				BtnCheckPsw(CCObject* pSender);
	void				BtnQuit(CCObject* pSender);
	void				BtnGongGao(CCObject* pSender);
public:
	void				OnLogin(SDataUser user);
	void				OnReg(SDataUser user, string strNickName, string strSubPsw);
	void				GoSite(CCObject* pSender);
	void				UpdateEdit(int flag, string strText);
private:
	SDataUser			m_DataUser;
	SDataUser			m_TempDataUser;
};