#include "PanelSettingIperfClient.h"
#include "../../resource.h"
#include "../../CarPlay/CarplayAdapter.h"

#if CVTE_EN_CARPLAY
CPanelSettingIperfClient::CPanelSettingIperfClient(void)
:m_bIsUDP(FALSE)
{
}
CPanelSettingIperfClient::~CPanelSettingIperfClient(void)
{

}

BOOL CPanelSettingIperfClient::IsReady()
{
	return (CPanelGL::IsReady());

}

void CPanelSettingIperfClient::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//m_bIsUDP = FALSE;

	UpdateSwitchStatus();
	UpdateRunTestStatus();

}

void CPanelSettingIperfClient::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingIperfClient::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case CARPLAY_CONTROL_STATUS:
		{
			UpdateRunTestStatus();
		}
		break;
	}
}

void CPanelSettingIperfClient::Render()
{
	if(!BeginRender())return;

//	    SetFrameTime(30);

    m_tagTitle1.SetAlpha(m_fCurAlpha);
    m_tagTitle1.Render();

    m_tagTitle2.SetAlpha(m_fCurAlpha);
    m_tagTitle2.Render();

//		VIEW_STATE vs = m_labIconFocus.GetCurViewState();
//		vs.fRotateZ -= 6;
//		if(vs.fRotateZ < -360)
//			vs.fRotateZ = 0;
//		m_labIconFocus.SetCurViewState(&vs);

	m_labIconFocus.SetAlpha(m_fCurAlpha);
	m_labIconFocus.Render();

	m_btnReturn.Render();

	m_btnTCPSwitch.SetAlpha(m_fCurAlpha);
	m_btnTCPSwitch.Render();

	m_btnUDPSwitch.SetAlpha(m_fCurAlpha);
	m_btnUDPSwitch.Render();

	m_btnAddress.SetAlpha(m_fCurAlpha);
	m_btnAddress.Render();

	m_btnTagAddress.SetAlpha(m_fCurAlpha);
	m_btnTagAddress.Render();

	m_btnTagAlertAddress.SetAlpha(m_fCurAlpha);
	m_btnTagAlertAddress.Render();

	m_btnDelete.SetAlpha(m_fCurAlpha);
	m_btnDelete.Render();

	m_btnPort.SetAlpha(m_fCurAlpha);
	m_btnPort.Render();

	m_btnTagPort.SetAlpha(m_fCurAlpha);
	m_btnTagPort.Render();

	m_btnRunTestBack.SetAlpha(m_fCurAlpha);
	m_btnRunTestBack.Render();

	m_btnRunTest.SetAlpha(m_fCurAlpha);
	m_btnRunTest.Render();

	m_btnNum1.SetAlpha(m_fCurAlpha);
	m_btnNum1.Render();
	m_btnNum2.SetAlpha(m_fCurAlpha);
	m_btnNum2.Render();
	m_btnNum3.SetAlpha(m_fCurAlpha);
	m_btnNum3.Render();

	m_btnNum4.SetAlpha(m_fCurAlpha);
	m_btnNum4.Render();
	m_btnNum5.SetAlpha(m_fCurAlpha);
	m_btnNum5.Render();
	m_btnNum6.SetAlpha(m_fCurAlpha);
	m_btnNum6.Render();

	m_btnNum7.SetAlpha(m_fCurAlpha);
	m_btnNum7.Render();
	m_btnNum8.SetAlpha(m_fCurAlpha);
	m_btnNum8.Render();
	m_btnNum9.SetAlpha(m_fCurAlpha);
	m_btnNum9.Render();

	m_btnNumA.SetAlpha(m_fCurAlpha);
	m_btnNumA.Render();
	m_btnNumB.SetAlpha(m_fCurAlpha);
	m_btnNumB.Render();
	m_btnNumC.SetAlpha(m_fCurAlpha);
	m_btnNumC.Render();

	m_btnNumD.SetAlpha(m_fCurAlpha);
	m_btnNumD.Render();
	m_btnNumE.SetAlpha(m_fCurAlpha);
	m_btnNumE.Render();
	m_btnNumF.SetAlpha(m_fCurAlpha);
	m_btnNumF.Render();

	m_btnDot.SetAlpha(m_fCurAlpha);
	m_btnDot.Render();
	m_btnNum0.SetAlpha(m_fCurAlpha);
	m_btnNum0.Render();
	m_btnColon.SetAlpha(m_fCurAlpha);
	m_btnColon.Render();
	m_btnSign.SetAlpha(m_fCurAlpha);
	m_btnSign.Render();

    COLORGL cr_b={0.35,0.35,0.35,1.0};

	DrawRect(0,g_iClientHeight/2-50-80/2,g_iClientWidth-210-50,1,&cr_b);

	EndRender();
}

BOOL CPanelSettingIperfClient::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//large size:
	SetSize(g_iClientWidth-240,g_iClientHeight+460);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
    COLORGL cr_gray2={0.35,0.35,0.35,1.0};
	COLORGL cr_black={0.0,0.0,0.0,1.0};
    COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={0.0f/256.0f,144.0f/256.0f,255.0f/256.0f,1.0};
	COLORGL cr_b={0.0,161.0f/256.0f,228.0/256.0f,1.0};

	CTexture *pUp=pSkinManager->GetTexture(TEXID_521);
	CTexture *pDown=pSkinManager->GetTexture(TEXID_522);

	COLORGL cr_key={1.0,1.0,1.0,1.0};
	COLORGL cr_key_down={0.0,0.0,0.0,1.0};
    
    int margin_x=20;
	int cx=(g_iClientWidth-240-margin_x*2)/2-10;

	int cx_key=(g_iClientWidth-440)/4;
	int cy_key=cx_key+8;
	GLfloat px= -3 * cx_key;
    
    GLfloat py=g_iClientHeight/2-60;

	GLfloat numScale = 0.7f;

    SIZE sizebtn={g_iClientWidth-240-margin_x*2,80};

    SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=128;
	sli.size.cy=64;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_642;
	sli.idChannelTex=TEXID_641;
	sli.idThumbTex=TEXID_640;
	sli.bSwitchMode=TRUE;

    m_tagTitle1.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle1.SetSize((g_iClientWidth-210-margin_x*2),80);
	m_tagTitle1.SetCaption(_T("Iperf Client Test"));
	m_tagTitle1.LinkTextGL(pTextGL);
	m_tagTitle1.SetPos(10,py+24);
	m_tagTitle1.SetCaptionHeight(24);
	m_tagTitle1.EnableDrawBack(FALSE);
	m_tagTitle1.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle1.SetCaptionColor(&cr_dark);

    m_tagTitle2.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle2.SetSize((g_iClientWidth-210-margin_x*2),80);
	m_tagTitle2.SetCaption(_T("Please Select a iperf client test."));
	m_tagTitle2.LinkTextGL(pTextGL);
	m_tagTitle2.SetPos(10,py-12);
	m_tagTitle2.SetCaptionHeight(16);
	m_tagTitle2.EnableDrawBack(FALSE);
	m_tagTitle2.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle2.SetCaptionColor(&cr_gray2);

    //m_labIcon.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_623));
	//m_labIcon.SetPos(170+10+32,py);
    m_labIconFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_012));
	//m_labIconFocus.SetPos(cx/2+160,py+24);
	m_labIconFocus.SetPos(170+30+32,py);
	m_labIconFocus.SetLuminance(0.2);
    //m_labIconFocus.Show(FALSE);

    m_btnReturn.Initialize(IDC_SETTING_BTN_RETURN,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    //m_btnReturn.SetPos(cx/2+160,py+24);
	m_btnReturn.SetPos(170+30+32,py);
    m_btnReturn.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
    m_btnReturn.EnableDrawBack(FALSE);

    py-=90;

    m_btnTCPSwitch.Initialize(IDC_SETTING_BTN_TCP_SWITCH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTCPSwitch.SetSize(200,50);
	m_btnTCPSwitch.SetPos(-100,py);
	m_btnTCPSwitch.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
    m_btnTCPSwitch.SetCaption(L"TCP");
    m_btnTCPSwitch.SetCaptionColor(&cr_black, &cr_w);
    m_btnTCPSwitch.SetCaptionHeight(24);
    m_btnTCPSwitch.LinkTextGL(pTextGL);
    m_btnTCPSwitch.SetCaptionPos(BTN_CAPTION_CENTER);

	m_btnUDPSwitch.Initialize(IDC_SETTING_BTN_UDP_SWITCH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnUDPSwitch.SetSize(200,50);
	m_btnUDPSwitch.SetPos(100,py);
	m_btnUDPSwitch.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
    m_btnUDPSwitch.SetCaption(L"UDP");
    m_btnUDPSwitch.SetCaptionColor(&cr_black, &cr_w);
    m_btnUDPSwitch.SetCaptionHeight(24);
    m_btnUDPSwitch.LinkTextGL(pTextGL);
    m_btnUDPSwitch.SetCaptionPos(BTN_CAPTION_CENTER);

	py-=80;

	m_btnAddress.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnAddress.SetSize(cx,40);
	m_btnAddress.SetPos(-cx/2-10 +12,py);
    m_btnAddress.SetCaption(L"Address");
    m_btnAddress.SetCaptionColor(&cr_gray);
    m_btnAddress.SetCaptionHeight(20);
    m_btnAddress.LinkTextGL(pTextGL);
    m_btnAddress.SetCaptionPos(BTN_CAPTION_LEFT);
	m_btnAddress.EnableDrawBack(FALSE);

	py-=80;

	m_btnTagAddress.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTagAddress.SetSize(&sizebtn);
	m_btnTagAddress.SetCaption(L"");
	m_btnTagAddress.LinkTextGL(pTextGL);
	m_btnTagAddress.SetPos(0,py);
	m_btnTagAddress.SetCaptionHeight(22);
	m_btnTagAddress.SetCaptionPos(BTN_CAPTION_RIGHT,-80);
	m_btnTagAddress.SetCaptionColor(&cr_black);

	m_btnTagAlertAddress.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnTagAlertAddress.SetSize(g_iClientWidth-280-margin_x*2,80);
	m_btnTagAlertAddress.SetCaption(L"Device IP address");
	m_btnTagAlertAddress.LinkTextGL(pTextGL);
	m_btnTagAlertAddress.SetPos(0,py);
	m_btnTagAlertAddress.SetCaptionHeight(24);
	m_btnTagAlertAddress.EnableDrawBack(FALSE);
	m_btnTagAlertAddress.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_btnTagAlertAddress.SetCaptionColor(&cr_gray);
	m_btnTagAlertAddress.Enable(FALSE);
	m_btnTagAlertAddress.Show(FALSE);

	m_btnDelete.Initialize(IDC_SETTING_BTN_KEY_DEL,this,m_pSkinManager->GetTexture(TEXID_519),NULL,
		m_pSkinManager->GetTexture(TEXID_520),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDelete.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnDelete.SetPos(g_iClientWidth/2-180,py);
	m_btnDelete.SetTouchRate(2.0f,1.0f);
	m_btnDelete.EnableHoldNotify(TRUE);
	m_btnDelete.Show(FALSE);

	py-=80;

	m_btnPort.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnPort.SetSize(&sizebtn);
	m_btnPort.SetPos(0,py);
    m_btnPort.SetCaption(L"Port");
    m_btnPort.SetCaptionColor(&cr_black, &cr_black);
    m_btnPort.SetCaptionHeight(24);
    m_btnPort.LinkTextGL(pTextGL);
    m_btnPort.SetCaptionPos(BTN_CAPTION_LEFT,20);

	m_btnTagPort.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnTagPort.SetSize(g_iClientWidth-280-margin_x*2,80);
	m_btnTagPort.SetCaption(L"5001");
	m_btnTagPort.LinkTextGL(pTextGL);
	m_btnTagPort.SetPos(0,py);
	m_btnTagPort.SetCaptionHeight(24);
	m_btnTagPort.EnableDrawBack(FALSE);
	m_btnTagPort.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_btnTagPort.SetCaptionColor(&cr_black);

	py-=100;

	m_btnRunTestBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnRunTestBack.SetSize(&sizebtn);
	m_btnRunTestBack.SetPos(0,py);
    m_btnRunTestBack.SetCaption(L"");
    m_btnRunTestBack.SetCaptionColor(&cr_black, &cr_black);
    m_btnRunTestBack.SetCaptionHeight(24);
    m_btnRunTestBack.LinkTextGL(pTextGL);
    m_btnRunTestBack.SetCaptionPos(BTN_CAPTION_LEFT,20);

	m_btnRunTest.Initialize(IDC_SETTING_BTN_RUN_TESTS,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnRunTest.SetSize(&sizebtn);
	m_btnRunTest.SetPos(0,py);
	m_btnRunTest.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
    m_btnRunTest.SetCaption(L"Run Tests");
    m_btnRunTest.SetCaptionColor(&cr_b, &cr_b);
    m_btnRunTest.SetCaptionHeight(24);
	m_btnRunTest.EnableDrawBack(FALSE);
    m_btnRunTest.LinkTextGL(pTextGL);
    m_btnRunTest.SetCaptionPos(BTN_CAPTION_CENTER);

	py-=100;

	//1-7
	m_btnNum1.Initialize(IDC_SETTING_BTN_KEY_1,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum1.SetCaption(L"1");
	m_btnNum1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum1.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum1.SetCaptionHeight(56);
	m_btnNum1.LinkTextGL(pTextGL);
	//m_btnNum1.SetSize(&si_key);
	m_btnNum1.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum1.SetPos(px+45,py);
	m_btnNum1.SetShowScale(numScale);

	m_btnNum2.Initialize(IDC_SETTING_BTN_KEY_2,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum2.SetCaption(L"2");
	m_btnNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum2.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum2.SetCaptionHeight(56);
	m_btnNum2.LinkTextGL(pTextGL);
	//m_btnNum2.SetSize(&si_key);
	m_btnNum2.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum2.SetPos(px+cx_key+50,py);
	m_btnNum2.SetShowScale(numScale);

	m_btnNum3.Initialize(IDC_SETTING_BTN_KEY_3,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum3.SetCaption(L"3");
	m_btnNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum3.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum3.SetCaptionHeight(56);
	m_btnNum3.LinkTextGL(pTextGL);
	//m_btnNum3.SetSize(&si_key);
	m_btnNum3.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum3.SetPos(px+cx_key*2+50,py);
	m_btnNum3.SetShowScale(numScale);

	m_btnNum4.Initialize(IDC_SETTING_BTN_KEY_4,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum4.SetCaption(L"4");
	m_btnNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum4.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum4.SetCaptionHeight(56);
	m_btnNum4.LinkTextGL(pTextGL);
	//m_btnNum4.SetSize(&si_key);
	m_btnNum4.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum4.SetPos(px+cx_key*3+50,py);
	m_btnNum4.SetShowScale(numScale);

	m_btnNum5.Initialize(IDC_SETTING_BTN_KEY_5,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum5.SetCaption(L"5");
	m_btnNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum5.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum5.SetCaptionHeight(56);
	m_btnNum5.LinkTextGL(pTextGL);
	//m_btnNum5.SetSize(&si_key);
	m_btnNum5.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum5.SetPos(px+cx_key*4+50,py);
	m_btnNum5.SetShowScale(numScale);

	m_btnNum6.Initialize(IDC_SETTING_BTN_KEY_6,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum6.SetCaption(L"6");
	m_btnNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum6.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum6.SetCaptionHeight(56);
	m_btnNum6.LinkTextGL(pTextGL);
	//m_btnNum6.SetSize(&si_key);
	m_btnNum6.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum6.SetPos(px+cx_key*5+50,py);
	m_btnNum6.SetShowScale(numScale);

	//7-0 A B C 

	m_btnNum7.Initialize(IDC_SETTING_BTN_KEY_7,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum7.SetCaption(L"7");
	m_btnNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum7.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum7.SetCaptionHeight(56);
	m_btnNum7.LinkTextGL(pTextGL);
	//m_btnNum7.SetSize(&si_key);
	m_btnNum7.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum7.SetPos(px+45,py-cy_key*1 -10);
	m_btnNum7.SetShowScale(numScale);

	m_btnNum8.Initialize(IDC_SETTING_BTN_KEY_8,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum8.SetCaption(L"8");
	m_btnNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum8.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum8.SetCaptionHeight(56);
	m_btnNum8.LinkTextGL(pTextGL);
	//m_btnNum9.SetSize(&si_key);
	m_btnNum8.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum8.SetPos(px+cx_key*1+50,py-cy_key*1-10);
	m_btnNum8.SetShowScale(numScale);

	m_btnNum9.Initialize(IDC_SETTING_BTN_KEY_9,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum9.SetCaption(L"9");
	m_btnNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum9.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum9.SetCaptionHeight(56);
	m_btnNum9.LinkTextGL(pTextGL);
	//m_btnNum0.SetSize(&si_key);
	m_btnNum9.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum9.SetPos(px+cx_key*2+50,py-cy_key*1-10);
	m_btnNum9.SetShowScale(numScale);

	m_btnNum0.Initialize(IDC_SETTING_BTN_KEY_0,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum0.SetCaption(L"0");
	m_btnNum0.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum0.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum0.SetCaptionHeight(56);
	m_btnNum0.LinkTextGL(pTextGL);
	//m_btnNumA.SetSize(&si_key);
	m_btnNum0.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNum0.SetPos(px+cx_key*3+50,py-cy_key*1-10);
	m_btnNum0.SetShowScale(numScale);

	m_btnNumA.Initialize(IDC_SETTING_BTN_KEY_A,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumA.SetCaption(L"A");
	m_btnNumA.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumA.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumA.SetCaptionHeight(56);
	m_btnNumA.LinkTextGL(pTextGL);
	//m_btnNumB.SetSize(&si_key);
	m_btnNumA.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNumA.SetPos(px+cx_key*4+50,py-cy_key*1-10);
	m_btnNumA.SetShowScale(numScale);

	m_btnNumB.Initialize(IDC_SETTING_BTN_KEY_B,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumB.SetCaption(L"B");
	m_btnNumB.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumB.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumB.SetCaptionHeight(56);
	m_btnNumB.LinkTextGL(pTextGL);
	//m_btnNumC.SetSize(&si_key);
	m_btnNumB.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNumB.SetPos(px+cx_key*5+50,py-cy_key*1-10);
	m_btnNumB.SetShowScale(numScale);

	//C D E F . : and %
	m_btnNumC.Initialize(IDC_SETTING_BTN_KEY_C,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumC.SetCaption(L"C");
	m_btnNumC.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumC.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumC.SetCaptionHeight(56);
	m_btnNumC.LinkTextGL(pTextGL);
	//m_btnNumD.SetSize(&si_key);
	m_btnNumC.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNumC.SetPos(px+45,py-cy_key*2-10);
	m_btnNumC.SetShowScale(numScale);

	

	m_btnNumD.Initialize(IDC_SETTING_BTN_KEY_D,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumD.SetCaption(L"D");
	m_btnNumD.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumD.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumD.SetCaptionHeight(56);
	m_btnNumD.LinkTextGL(pTextGL);
	//m_btnNumE.SetSize(&si_key);
	m_btnNumD.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNumD.SetPos(px+cx_key*1+50,py-cy_key*2-10);
	m_btnNumD.SetShowScale(numScale);

	m_btnNumE.Initialize(IDC_SETTING_BTN_KEY_E,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumE.SetCaption(L"E");
	m_btnNumE.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumE.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumE.SetCaptionHeight(56);
	m_btnNumE.LinkTextGL(pTextGL);
	//m_btnNumF.SetSize(&si_key);
	m_btnNumE.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNumE.SetPos(px+cx_key*2+50,py-cy_key*2-10);
	m_btnNumE.SetShowScale(numScale);

	m_btnNumF.Initialize(IDC_SETTING_BTN_KEY_F,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumF.SetCaption(L"F");
	m_btnNumF.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumF.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumF.SetCaptionHeight(56);
	m_btnNumF.LinkTextGL(pTextGL);
	//m_btnNumF.SetSize(&si_key);
	m_btnNumF.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnNumF.SetPos(px+cx_key*3+50,py-cy_key*2-10);
	m_btnNumF.SetShowScale(numScale);

	m_btnDot.Initialize(IDC_SETTING_BTN_KEY_DOT,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDot.SetCaption(L".");
	m_btnDot.SetCaptionPos(BTN_CAPTION_CENTER,0,-6);
	m_btnDot.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnDot.SetCaptionHeight(56);
	m_btnDot.LinkTextGL(pTextGL);
	//m_btnDot.SetSize(&si_key);
	m_btnDot.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnDot.SetPos(px+cx_key*4+50,py-cy_key*2-10);
	m_btnDot.SetShowScale(numScale);
	
	m_btnColon.Initialize(IDC_SETTING_BTN_KEY_COLON,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnColon.SetCaption(L":");
	m_btnColon.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnColon.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnColon.SetCaptionHeight(56);
	m_btnColon.LinkTextGL(pTextGL);
	//m_btnColon.SetSize(&si_key);
	m_btnColon.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnColon.SetPos(px+cx_key*5+50,py-cy_key*2-10);
	m_btnColon.SetShowScale(numScale);

	m_btnSign.Initialize(IDC_SETTING_BTN_KEY_SIGN,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSign.SetCaption(L"%");
	m_btnSign.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnSign.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnSign.SetCaptionHeight(56);
	m_btnSign.LinkTextGL(pTextGL);
	//m_btnSign.SetSize(&si_key);
	m_btnSign.SetNotifyFunc(&CPanelSettingIperfClient::OnNotify,this);
	m_btnSign.SetPos(px+45,py-cy_key*3-10);
	m_btnSign.SetShowScale(numScale);

	return TRUE;
}


void CALLBACK CPanelSettingIperfClient::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingIperfClient *panel=(CPanelSettingIperfClient*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelSettingIperfClient::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingIperfClient::HandleControlMSG] idControl:%d wMsg:%d wPara:%d\r\n"),idControl,wMsg,wPara));

	switch (idControl)
	{
        case IDC_SETTING_BTN_RETURN:
            {
                SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_FACTORY);
				PlaySoundEffect(0);
            }
            break;
		case IDC_SETTING_BTN_TCP_SWITCH:
			{
				m_bIsUDP = FALSE;
				UpdateSwitchStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_UDP_SWITCH:
			{
				m_bIsUDP = TRUE;
				UpdateSwitchStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_RUN_TESTS:
			{
				if (m_bIsUDP)
				{
					SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_IPERF_UDP_CLIENT);
				}
				else
				{
					SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_IPERF_TCP_CLIENT);
				}
				PlaySoundEffect(0);
			}
			break;

		case IDC_SETTING_BTN_KEY_0:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('0');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_1:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('1');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_2:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('2');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_3:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('3');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_4:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('4');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_5:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('5');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_6:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('6');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_7:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('7');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_8:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('8');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_9:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('9');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_A:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('A');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_B:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('B');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_C:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('C');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_D:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('D');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_E:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('E');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_F:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('F');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_DOT:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('.');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_COLON:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress(':');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_SIGN:
			{
				CCarplayAdapter::GetInstance()->InputSessionAddress('%');
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_DEL:
			{
				CCarplayAdapter::GetInstance()->DeleteSessionAddress(1);
				UpdateRunTestStatus();
				PlaySoundEffect(0);
			}
			break;
		
	}
}

void CPanelSettingIperfClient::UpdateSwitchStatus()
{
	if (m_bIsUDP)
	{
		m_btnTCPSwitch.SetStatus(BS_UP);
		m_btnUDPSwitch.SetStatus(BS_DOWN);
	}
	else
	{
		m_btnTCPSwitch.SetStatus(BS_DOWN);
		m_btnUDPSwitch.SetStatus(BS_UP);
	}
}

void CPanelSettingIperfClient::UpdateRunTestStatus()
{
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_b={0.0,161.0f/256.0f,228.0/256.0f,1.0};
	size_t  strlength = 0;
	size_t	maxlen = 28;

	if (CCarplayAdapter::GetInstance()->GetSessionAddressStr() != NULL)
	{
		strlength = wcslen(CCarplayAdapter::GetInstance()->GetSessionAddressStr());
	}

	
	if(strlength<=maxlen)
	{
		m_btnTagAddress.SetCaption(CCarplayAdapter::GetInstance()->GetSessionAddressStr());
	}
	else
	{
		m_btnTagAddress.SetCaption(CCarplayAdapter::GetInstance()->GetSessionAddressStr()+strlength-maxlen);
	}
	

	if (strlength > 0)
	{
		m_btnTagAlertAddress.Show(FALSE);
		m_btnDelete.Show(TRUE);
	}
	else
	{
		m_btnTagAlertAddress.Show(TRUE);
		m_btnDelete.Show(FALSE);
	}

	if (CCarplayAdapter::GetInstance()->isSessionAddressValid())
	{
		m_btnRunTest.Enable(TRUE);
		m_btnRunTest.SetCaptionColor(&cr_b, &cr_b);
	}
	else
	{
		m_btnRunTest.Enable(FALSE);
		m_btnRunTest.SetCaptionColor(&cr_gray, &cr_gray);
	}
}

void CPanelSettingIperfClient::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);
	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}
}

#endif
