#include "FKSceneLogin.h"
#include "../Base/FKStringFunc.h"
#include "../Base/FKVisiableRect.h"
#include "FKSceneEmptyRes.h"
#include "FKLayerLogin.h"
//------------------------------------------------------------
CFKSceneLogin::CFKSceneLogin()
{
	CFKSceneBase::Init();
}
//------------------------------------------------------------
CFKSceneLogin::~CFKSceneLogin()
{
	CFKSceneBase::ExitRelease();
}
//------------------------------------------------------------
void CFKSceneLogin::RunThisScene()
{
	CFKLayerLogin* pLayer = CFKLayerLogin::create();
	addChild(pLayer);
}
//------------------------------------------------------------
enumSceneID CFKSceneLogin::GetSceneID()
{
	return eSceneID_Login;
}
//------------------------------------------------------------