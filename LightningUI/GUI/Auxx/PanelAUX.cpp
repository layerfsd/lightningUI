#include "PanelAUX.h"
#include "../../resource.h"

#if CVTE_EN_AUX
CPanelAUX::CPanelAUX(void)
{
	m_pVideoWindow = NULL;

	m_dwEnterTick = 0;

    m_bPhoneCallActive = FALSE;
	m_bSpeechOn = FALSE;
}

CPanelAUX::~CPanelAUX(void)
{
}
BOOL CPanelAUX::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelAUX::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

//	    ::PostMessage(m_hMainWnd,WM_MEDIA_SOURCE_MSG,MEDIA_TYPE_AUX,0);
}
void CPanelAUX::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

    RETAILMSG(1,(_T("APP:MSG:GUI:Main: [CPanelAUX::OnEnterPanel]: \r\n")));

    //make animation:
	if(idAni != ANIM_NONE)
	{
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
	}

	m_dwEnterTick = GetTickCount();

	//switch audio:
//	    MuteAudio(TRUE,FALSE,_ARM_MUTE_ALL,TRUE,1000);
	if(GetAudioSRC() != SRC_AUDIO_AUX1 /*&& !m_bSpeechOn */ &&!m_bPhoneCallActive)
	{
	    SwitchAudioSRC(SRC_AUDIO_AUX1,TRUE);
	}

// 	//switch video:
// 	if(m_pCommander)
// 	{
// // 		m_pCommander->SendCmd(_ARM_VIDEO_SW_KEY,SRC_VIDEO_TV);
// 		m_pCommander->SendCmd(_ARM_VIDEO_SW_KEY,SRC_VIDEO_AUX1);
// 	}

}
void CPanelAUX::Render()
{

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_229):NULL;
#else
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
	static CTexture *pTexBar2=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_056):NULL;
#endif
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};
	GLfloat fbackLumin=m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance;

	if(!BeginRender())return;
	
	//==============================================//
	//业务逻辑操作
	m_tagAUX_B.Show(m_BeShow);
	m_tagAUX_B.Enable(m_BeShow);
	m_tagAUX_B.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
	
	m_tagBT.Show(m_BeShow);
	m_tagBT.Enable(m_BeShow);
	m_tagBT.SetPos(-g_iClientWidth/2+110,  g_iClientHeight/2-40);
	
	m_tagDVD.Show(m_BeShow);
	m_tagDVD.Enable(m_BeShow);
	m_tagDVD.SetPos(-g_iClientWidth/2+180,  g_iClientHeight/2-40);
	
	m_tagUSB.Show(m_BeShow);
	m_tagUSB.Enable(m_BeShow);
	m_tagUSB.SetPos(-g_iClientWidth/2+250,  g_iClientHeight/2-40);

	m_tagLeft.Show(m_BeShow);
	m_tagRight.Show(!m_BeShow);

	switch(m_BeSelect){
		case SelectMode_AUX:
			if(!m_BeShow){
				m_tagAUX_B.Show(!m_BeShow);
				m_tagAUX_B.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
			}
			
			m_tagTitle.SetCaption(_T("Now Playing From AUX IN"));
			break;

		case SelectMode_BT:
			if(!m_BeShow){
				m_tagBT.Show(!m_BeShow);
				m_tagBT.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
			}
			
			m_tagTitle.SetCaption(_T("Now Playing From BT"));
			break;

		case SelectMode_DVD:
			if(!m_BeShow){
				m_tagDVD.Show(!m_BeShow);
				m_tagDVD.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
			}

			m_tagTitle.SetCaption(_T("Now Playing From DVD"));
			break;

		case SelectMode_USB:
			if(!m_BeShow){
				m_tagUSB.Show(!m_BeShow);
				m_tagUSB.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
			}

			m_tagTitle.SetCaption(_T("Now Playing From USB"));
			break;

		default:
			break;
	}
	
	//=================================================//
	//UI操作
/*
	if(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect)
	{
		m_tagBkBlur.SetAlpha(m_fCurAlpha);
		m_tagBkBlur.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkBlur.Render();
	}
	else
	{
		m_tagBkOrigin.SetAlpha(m_fCurAlpha);
		m_tagBkOrigin.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkOrigin.Render();
	}
*/
/*
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBackEq.SetAlpha(m_fCurAlpha);
	m_tagBackEq.Render();
#else
	VIEW_STATE vs=m_tagBackEq.GetCurViewState();
	vs.fRotateX=0;
	vs.fTransY=55;
	vs.fTransX=-253;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.3f);
	m_tagBackEq.Render();
	vs.fTransX=254;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.Render();
	vs.fRotateX=-180;
	vs.fTransY=-55;
	vs.fTransX=-253;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.SetAlpha(m_fCurAlpha*fbackLumin*0.2f);
	m_tagBackEq.Render();
	vs.fTransX=254;
	m_tagBackEq.SetCurViewState(&vs);
	m_tagBackEq.Render();
#endif
*/

	COLORGL cr_bg={0.38f,0.36f,0.36f,1.0f};
	DrawRect(pTexBar,0,0, g_iClientWidth-4,  g_iClientHeight-4, 2, &cr, TRUE);
	
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	//static CTexture *pTexFrame=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_296):NULL;
	static CTexture *pTexBar2=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_056):NULL;

	//DrawRect(pTexFrame,-200,15,280-16,280-16,8,&cr,TRUE);
	DrawRect(pTexBar2,0,(g_iClientHeight/2-30),g_iClientWidth-4,60-4,2,&cr,TRUE);

	////draw title
	//if(m_pTextGL)
	//{
	//	SIZE si;
	//	m_pTextGL->SetAlpha(m_fCurAlpha);
	//	m_pTextGL->SetLuminance(m_fCurLuminance);

	//	m_pTextGL->SetColor(&cr,FALSE);
	//	m_pTextGL->SetString(_T("AUX INPUT"));
	//	m_pTextGL->SetCharScale(0.8f,0.8f);
	//	m_pTextGL->GetExtension(&si);
	//	m_pTextGL->SetPos(-GLfloat(g_iClientWidth/2)+30,(g_iClientHeight/2-30+si.cy/2));
	//	m_pTextGL->Render();
	//}

	m_tagAUX.SetAlpha(m_fCurAlpha);
	m_tagAUX.Render();

	m_tagAUXBox.SetAlpha(m_fCurAlpha);
	m_tagAUXBox.Render();

	m_tagAUXTitle.SetAlpha(m_fCurAlpha);
	m_tagAUXTitle.Render();

	//m_tagAux1.SetAlpha(m_fCurAlpha);
	//m_tagAux1.Render();
	//m_tagAux2.SetAlpha(m_fCurAlpha);
	//m_tagAux2.Render();
#else

	m_iconAUX2.SetAlpha(m_fCurAlpha);
	m_iconAUX2.Render();
#endif

	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_tagTips.SetAlpha(m_fCurAlpha);
	m_tagTips.Render();

	//bar:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	DrawRect(pTexBar,0,(-g_iClientHeight/2+48),800,96,2,&cr,TRUE);
#else
	DrawRect(pTexBar,0,(-g_iClientHeight/2+40), g_iClientWidth-4, 80, 2, &cr, TRUE);
	DrawRect(pTexBar,0,(g_iClientHeight/2-40), g_iClientWidth-4, 80, 2, &cr, TRUE);
#endif

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	m_tagRight.SetAlpha(m_fCurAlpha);
	m_tagRight.Render();
	
	m_tagLeft.SetAlpha(m_fCurAlpha);
	m_tagLeft.Render();	
	
	m_tagAUX_B.SetAlpha(m_fCurAlpha);
	m_tagAUX_B.Render();

	m_tagBT.SetAlpha(m_fCurAlpha);
	m_tagBT.Render();

	m_tagDVD.SetAlpha(m_fCurAlpha);
	m_tagDVD.Render();	

	m_tagUSB.SetAlpha(m_fCurAlpha);
	m_tagUSB.Render();	

	if(m_pStatusBar && IsStatusBarVisible())
	{
		m_pStatusBar->SetAlpha(m_fCurAlpha);
		m_pStatusBar->Render();
	}

	EndRender();

}
BOOL CPanelAUX::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	SIZE si;
	VIEW_STATE vs;

	m_bShowStatusBar = TRUE;
	m_bEnableSmartBar = TRUE;

	m_BeSelect = SelectMode_AUX;
	m_BeShow = FALSE;
/*
	m_tagBackEq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_460));
	m_tagBackEq.SetPos(0,0);

	m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2000)));
	si=m_tagBkBlur.GetTexture()->GetImageSize();
	m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkBlur.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkBlur.SetIniViewState(&vs);

	m_tagBkOrigin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2001)));
	si=m_tagBkOrigin.GetTexture()->GetImageSize();
	m_tagBkOrigin.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkOrigin.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkOrigin.SetIniViewState(&vs);
*/

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	//COLORGL cr_blue={0.0f,0.58f,0.85f,1.0f};
	m_tagAUX.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_295));
	m_tagAUX.SetShowScale(0.94f);
	m_tagAUX.SetPos(-215,0);

	m_tagAUXBox.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_297));
	m_tagAUXBox.SetShowScale(0.94f);
	m_tagAUXBox.SetPos(115,0);

	m_tagAUXTitle.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_903));
	m_tagAUXTitle.SetShowScale(1.1f);
	m_tagAUXTitle.SetPos(-GLfloat(g_iClientWidth/2)+125,(g_iClientHeight/2-30));

	//m_tagAux2.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_296),NULL,
	//	NULL,NULL,
	//	NULL,NULL,
	//	NULL,NULL);
	//m_tagAux2.SetSize(400,280);
	//m_tagAux2.LinkTextGL(pTextGL);
	//m_tagAux2.SetPos(150,15);
	//m_tagAux2.SetCaption(GetResourceString(IDS_AUX_PLAYING));
	////m_tagAux2.SetCaptionColor(&cr_blue);
	//m_tagAux2.SetCaptionHeight(24);
	//m_tagAux2.SetCaptionPos(BTN_CAPTION_CENTER,0,76);
	//m_tagAux2.SetSubCaption(GetResourceString(IDS_AUX_TIP2));
	//m_tagAux2.SetSubCaptionPos(BTN_CAPTION_CENTER,10,-16);
	//m_tagAux2.SetCaptionColor(&cr_blue);
	
	m_btnHome.Initialize(IDC_AUX_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+80,-g_iClientHeight/2+40);
	m_btnHome.SetTouchRate(1.6f,1.6f);
	m_btnHome.SetShowScale(0.8f);
	m_btnHome.EnableClickAni(TRUE);
	m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));	
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	GLfloat texCoord_s01000[]={
		132.0/256.0, 132.0/256.0,
		132.0/256.0, 252.0/256.0,
		252.0/256.0, 132.0/256.0,
		252.0/256.0, 252.0/256.0};

	m_iconAUX.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_157),texCoord_s01000);
	m_iconAUX.SetPos(0,10);
	m_iconAUX.SetShowScale(0.9f);

	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_AUX));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,150);
	m_tagTitle.SetCaptionHeight(32);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);

	m_btnHome.Initialize(IDC_AUX_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+80,-g_iClientHeight/2+40);
	m_btnHome.SetTouchRate(1.6f,1.6f);
	m_btnHome.SetShowScale(0.8f);
	m_btnHome.EnableClickAni(TRUE);
	m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
	
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	m_iconAUX.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150));
	m_iconAUX.SetPos(0,10);
	m_iconAUX.SetShowScale(1.5f);

	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_AUX));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,130);
	m_tagTitle.SetCaptionHeight(32);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);

	//home button
	m_btnHome.Initialize(IDC_AUX_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_205),NULL,
		m_pSkinManager->GetTexture(TEXID_204),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+65,-g_iClientHeight/2+48);
	m_btnHome.SetTouchRate(2.0,2.0);

#else
	m_btnHome.Initialize(IDC_AUX_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_015),NULL,
		m_pSkinManager->GetTexture(TEXID_016),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+80,-g_iClientHeight/2+40);
	m_btnHome.SetTouchRate(1.6f,1.6f);
	m_btnHome.SetShowScale(0.8f);
	m_btnHome.EnableClickAni(TRUE);
	m_btnHome.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//AUX图标
	m_tagAUX_B.Initialize(IDC_AUX_BTN_AUX,this,m_pSkinManager->GetTexture(TEXID_125),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagAUX_B.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_tagAUX_B.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
	//m_tagAUX_B.SetTouchRate(1.6f,1.6f);
	m_tagAUX_B.SetShowScale(0.4f);
	m_tagAUX_B.EnableClickAni(TRUE);
	//m_tagAUX_B.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));
	
	//向右呼出按键
	m_tagRight.Initialize(IDC_AUX_BTN_RIGHT,this,m_pSkinManager->GetTexture(TEXID_083),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagRight.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_tagRight.SetPos(-g_iClientWidth/2+70,  g_iClientHeight/2-40);
	//m_tagRight.SetTouchRate(1.6f,1.6f);
	m_tagRight.SetShowScale(1.0f);
	m_tagRight.EnableClickAni(TRUE);
	//m_tagRight.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//蓝牙按键
	m_tagBT.Initialize(IDC_AUX_BTN_BT,this,m_pSkinManager->GetTexture(TEXID_121),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBT.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_tagBT.SetPos(-g_iClientWidth/2+110,  g_iClientHeight/2-40);
	//m_tagBT.SetTouchRate(1.6f,1.6f);
	m_tagBT.SetScale(0.4, 0.5);
	m_tagBT.EnableClickAni(TRUE);
	//m_tagBT.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//DVD按键
	m_tagDVD.Initialize(IDC_AUX_BTN_DVD,this,m_pSkinManager->GetTexture(TEXID_120),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagDVD.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_tagDVD.SetPos(-g_iClientWidth/2+180,  g_iClientHeight/2-40);
	//m_tagDVD.SetTouchRate(1.6f,1.6f);
	m_tagDVD.SetScale(0.4, 0.5);
	m_tagDVD.EnableClickAni(TRUE);
	//m_tagDVD.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//USB按键
	m_tagUSB.Initialize(IDC_AUX_BTN_USB,this,m_pSkinManager->GetTexture(TEXID_123),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagUSB.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_tagUSB.SetPos(-g_iClientWidth/2+250,  g_iClientHeight/2-40);
	//m_tagUSB.SetTouchRate(1.6f,1.6f);
	m_tagUSB.SetShowScale(0.4f);
	m_tagUSB.EnableClickAni(TRUE);
	//m_tagUSB.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//向左呼出按键
	m_tagLeft.Initialize(IDC_AUX_BTN_LEFT,this,m_pSkinManager->GetTexture(TEXID_082),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLeft.SetNotifyFunc(&CPanelAUX::OnNotify,this);
	m_tagLeft.SetPos(-g_iClientWidth/2+310,  g_iClientHeight/2-40);
	//m_tagLeft.SetTouchRate(1.6f,1.6f);
	m_tagLeft.SetShowScale(1.0f);
	m_tagLeft.EnableClickAni(TRUE);
	//m_tagLeft.EnableTouchGlow(TRUE,m_pSkinManager->GetTexture(TEXID_020));

	//碟片图标
	m_iconAUX2.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_202));	
	m_iconAUX2.SetPos(-g_iClientWidth/2 + 150, 0);
	m_iconAUX2.SetShowScale(0.6f);

	//显示标题
	COLORGL cr_blue={0.08f,0.87f,0.99f,1.0f};
	SIZE si1={800,32};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&si1);
	m_tagTitle.SetCaption(_T("Now Playing From AUX IN"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(g_iClientWidth/2-130,0);
	m_tagTitle.SetCaptionHeight(32);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);	
	m_tagTitle.SetCaptionColor(&cr_blue);

	COLORGL cr_white={1.0f, 1.0f, 1.0f, 1.0f};
	SIZE si2={800,15};
	m_tagTips.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTips.SetSize(&si2);
	m_tagTips.SetCaption(GetResourceString(IDS_AUX_TIP2));
	m_tagTips.LinkTextGL(pTextGL);
	m_tagTips.SetPos(70,-40);
	m_tagTips.SetCaptionHeight(15);
	m_tagTips.EnableDrawBack(FALSE);
	m_tagTips.SetCaptionPos(BTN_CAPTION_CENTER);	
	m_tagTips.SetCaptionColor(&cr_white);

	/*
	//初始显示状态栏
	m_tagAUX_B.Show(1);
	m_tagAUX_B.Enable(0);
	
	m_tagBT.Show(0);
	m_tagDVD.Show(0);
	m_tagUSB.Show(0);

	m_tagLeft.Show(0);
	m_tagRight.Show(1);
	*/
	
#endif
	return TRUE;
}


void CALLBACK CPanelAUX::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_AUX,(_T("APP:MSG:CPanelAUX OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelAUX *panel=(CPanelAUX*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelAUX::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(DEBUG_AUX,(_T("[MSG]CPanelAUX::HandleControlMSG id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));
	switch (idControl)
	{
	case IDC_AUX_BTN_HOME:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;

	case IDC_AUX_BTN_RIGHT:
		m_BeShow = TRUE;
		/*
		m_tagAUX_B.Show(m_BeShow);
		m_tagAUX_B.Enable(m_BeShow);
		m_tagAUX_B.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
		
		m_tagBT.Show(m_BeShow);
		m_tagBT.Enable(m_BeShow);
		m_tagBT.SetPos(-g_iClientWidth/2+110,  g_iClientHeight/2-40);
		
		m_tagDVD.Show(m_BeShow);
		m_tagDVD.Enable(m_BeShow);
		m_tagDVD.SetPos(-g_iClientWidth/2+180,  g_iClientHeight/2-40);
		
		m_tagUSB.Show(m_BeShow);
		m_tagUSB.Enable(m_BeShow);
		m_tagUSB.SetPos(-g_iClientWidth/2+250,  g_iClientHeight/2-40);

		m_tagLeft.Show(m_BeShow);
		m_tagRight.Show(!m_BeShow);
		*/
		PlaySoundEffect(0);
		break;
		
	case IDC_AUX_BTN_LEFT:
		m_BeShow = FALSE;
		/*
		m_tagAUX_B.Show(m_BeShow);
		m_tagAUX_B.Enable(m_BeShow);
		
		m_tagBT.Show(m_BeShow);
		m_tagBT.Enable(m_BeShow);
		
		m_tagDVD.Show(m_BeShow);
		m_tagDVD.Enable(m_BeShow);
		
		m_tagUSB.Show(m_BeShow);
		m_tagUSB.Enable(m_BeShow);

		m_tagLeft.Show(m_BeShow);
		m_tagRight.Show(m_BeShow);

		switch(m_BeSelect){
			case SelectMode_AUX:
				m_tagAUX_B.Show(1);
				m_tagAUX_B.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
				break;

			case SelectMode_BT:
				m_tagBT.Show(1);
				m_tagBT.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
				break;

			case SelectMode_DVD:
				m_tagDVD.Show(1);
				m_tagDVD.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
				break;

			case SelectMode_USB:
				m_tagUSB.Show(1);
				m_tagUSB.SetPos(-g_iClientWidth/2 + 40, g_iClientHeight/2 - 40);
				break;

			default:
				break;
		}
		*/
		PlaySoundEffect(0);
		break;

	case IDC_AUX_BTN_AUX:	
		//m_tagTitle.SetCaption(_T("Now Playing From AUX IN"));
		m_BeSelect = SelectMode_AUX;
		
		
		PlaySoundEffect(0);
		break;

	case IDC_AUX_BTN_BT:
		//m_tagTitle.SetCaption(_T("Now Playing From BT"));
		m_BeSelect = SelectMode_BT;
		PlaySoundEffect(0);
		break;

	case IDC_AUX_BTN_DVD:
		//m_tagTitle.SetCaption(_T("Now Playing From DVD"));
		m_BeSelect = SelectMode_DVD;
		PlaySoundEffect(0);
		break;

	case IDC_AUX_BTN_USB:
		//m_tagTitle.SetCaption(_T("Now Playing From USB"));
		m_BeSelect = SelectMode_USB;
		PlaySoundEffect(0);
		break;

	}
}
void CPanelAUX::OnVideoWndGesture(WPARAM wParam, LPARAM lParam,LPVOID lpContext)
{
	CPanelAUX *panel=(CPanelAUX*)lpContext;

	GESTUREINFO gi;
	memset(&gi, 0, sizeof(gi));
	gi.cbSize = sizeof(GESTUREINFO);
	if (GetGestureInfo((HGESTUREINFO)lParam, &gi))
	{

		// Handle gesture indicated by wParam or gi.dwID
		switch (gi.dwID)
		{
		case GID_BEGIN:
			break;
		case GID_END:
			break;
		case GID_PAN:
			break;
		case GID_ROTATE:
			break;
		case GID_SCROLL:
			break;
		case GID_HOLD:
			break;
		case GID_DOUBLESELECT:
			break;
		case GID_SELECT:
			break;
		}
	}

}

#if CVTE_EN_KEY_PROXY
BOOL CPanelAUX::OnKey(UINT idKey,UINT idStatus)
{
	switch (idKey)
	{
	case KEY_PLAY:
	case KEY_PLAYPAUSE:
	case KEY_RESUME:
		{
			if ( idStatus == EV_KEY_UP )
			{
				if(!m_bPhoneCallActive /*&& GetAudioSRC() != SRC_AUDIO_AUX1*/)
				{
					SwitchAudioSRC(SRC_AUDIO_AUX1,TRUE);
				}
			}
		}
		break;
	case KEY_STOP:
	case KEY_PAUSE:
		break;
	}

	return TRUE;
}
#else
BOOL CPanelAUX::OnKey(UINT idKey)
{
	switch (idKey)
	{
	case KEY_MCU_ENTER:
		break;
	case KEY_MCU_SEEK_NEXT:
		break;
	case KEY_MCU_SEEK_PREV:
		break;
	case KEY_MCU_TRACK_NEXT:
	case KEY_MEDIA_NEXT:
		break;
	case KEY_MCU_TRACK_PREV:
	case KEY_MEDIA_PREVIOUS:
		break;
	case KEY_MEDIA_PLAY:
		{
			if(/*!m_bSpeechOn && */!m_bPhoneCallActive/*&& GetAudioSRC() != SRC_AUDIO_AUX1*/)
			{
				SwitchAudioSRC(SRC_AUDIO_AUX1,TRUE);
			}
		}
		break;
	case KEY_MEDIA_PAUSE:
		break;
	case KEY_MEDIA_PLAY_PAUSE:
		break;
	}

	return TRUE;
}
#endif

void CPanelAUX::OnSpeechStart()
{
	m_bSpeechOn = TRUE;
}
void CPanelAUX::OnSpeechTerminate()
{
	m_bSpeechOn = FALSE;
}

void CPanelAUX::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;
    RETAILMSG(1, (TEXT("MSG:[CPanelAUX::OnPhoneCallState]: m_bPhoneCallActive:%d\r\n"),m_bPhoneCallActive));
    if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC) == UI_CLASS_AUX)
	{
		if(wParam)//active
		{
		}
		else
		{
            //if(!m_bSpeechOn)
            {
			    SwitchAudioSRC(SRC_AUDIO_AUX1,TRUE);
            }
		}
	}
}

#endif
