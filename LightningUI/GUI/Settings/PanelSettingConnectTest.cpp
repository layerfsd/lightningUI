#include "PanelSettingConnectTest.h"
#include "../../resource.h"
#include "../../CarPlay/CarplayAdapter.h"

CPanelSettingConnectTest::CPanelSettingConnectTest(void)
{
	m_Consolestr = _T("");
}
CPanelSettingConnectTest::~CPanelSettingConnectTest(void)
{

}

BOOL CPanelSettingConnectTest::IsReady()
{
	return (CPanelGL::IsReady());

}

void CPanelSettingConnectTest::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	m_Consolestr = _T("");
}

void CPanelSettingConnectTest::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingConnectTest::Render()
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

	m_btnConnectCmd.SetAlpha(m_fCurAlpha);
    m_btnConnectCmd.Render();

	m_iconConnectCmd.SetAlpha(m_fCurAlpha);
    m_iconConnectCmd.Render();

	m_btnConsoleOutput.SetAlpha(m_fCurAlpha);
	m_btnConsoleOutput.Render();

	COLORGL cr_b={0.35,0.35,0.35,1.0};

	DrawRect(0,g_iClientHeight/2-50-80/2,g_iClientWidth-210-50,1,&cr_b);

	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	//
	if(m_pTextGL)
	{
		SIZE textsize;
		m_pTextGL->SetString(m_Consolestr);
		m_pTextGL->SetColor(&cr_gray,FALSE);
		m_pTextGL->SetCharScale(11.0f/32.0f,11.0f/32.0f);
		m_pTextGL->GetExtension(&textsize);
		m_pTextGL->SetPos(-(g_iClientWidth-290)/2-20,m_btnConnectCmd.GetPosY()-50);
		m_pTextGL->SetAlpha(1.0f);
		m_pTextGL->SetLuminance(1.0f);
		m_pTextGL->Render();
	}

	EndRender();
}

BOOL CPanelSettingConnectTest::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//large size:
	SetSize(g_iClientWidth-240,g_iClientHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
    COLORGL cr_gray2={0.35,0.35,0.35,1.0};
	COLORGL cr_black={0.0,0.0,0.0,1.0};
    COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={0.0f/256.0f,144.0f/256.0f,255.0f/256.0f,1.0};
	COLORGL cr_b={0.0,161.0f/256.0f,228.0/256.0f,1.0};
    
    int margin_x=20;
	int cx=(g_iClientWidth-240-margin_x*2)/2-10;
    
    GLfloat py=g_iClientHeight/2-60;

    SIZE sizebtn={200,64};

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
	m_tagTitle1.SetCaption(L"Connect Test");
	m_tagTitle1.LinkTextGL(pTextGL);
	m_tagTitle1.SetPos(10,py+24);
	m_tagTitle1.SetCaptionHeight(24);
	m_tagTitle1.EnableDrawBack(FALSE);
	m_tagTitle1.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle1.SetCaptionColor(&cr_dark);

    m_tagTitle2.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle2.SetSize((g_iClientWidth-210-margin_x*2),80);
	m_tagTitle2.SetCaption(L"Please Press The Test Button to start test.");
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
    m_btnReturn.SetNotifyFunc(&CPanelSettingConnectTest::OnNotify,this);
    m_btnReturn.EnableDrawBack(FALSE);

	py-=80+4;
    
    m_btnConnectCmd.Initialize(IDC_SETTING_BTN_START_CONNECT_TEST,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnConnectCmd.SetSize(&sizebtn);
	m_btnConnectCmd.SetPos(10,py);
    m_btnConnectCmd.SetCaption(L"Connect Test");
    m_btnConnectCmd.SetCaptionColor(&cr_black, &cr_black);
    m_btnConnectCmd.SetCaptionHeight(16);
    m_btnConnectCmd.LinkTextGL(pTextGL);
    m_btnConnectCmd.SetCaptionPos(BTN_CAPTION_LEFT,55);
	m_btnConnectCmd.SetNotifyFunc(&CPanelSettingConnectTest::OnNotify,this);

	m_iconConnectCmd.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconConnectCmd.SetPos(10-72,py);

	py-=140+40;

	m_btnConsoleOutput.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnConsoleOutput.SetSize(g_iClientWidth-180-margin_x*2,280);
	m_btnConsoleOutput.SetPos(0,py);
    m_btnConsoleOutput.SetCaption(L"");
    m_btnConsoleOutput.SetCaptionColor(&cr_gray, &cr_gray); 
	m_btnConsoleOutput.LinkTextGL(pTextGL);
    m_btnConsoleOutput.SetCaptionHeight(16);
    m_btnConsoleOutput.SetCaptionPos(BTN_CAPTION_LEFT);

	return TRUE;
}


void CALLBACK CPanelSettingConnectTest::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingConnectTest *panel=(CPanelSettingConnectTest*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelSettingConnectTest::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingConnectTest::HandleControlMSG] idControl:%d wMsg:%d wPara:%d\r\n"),idControl,wMsg,wPara));

	switch (idControl)
	{
        case IDC_SETTING_BTN_RETURN:
            {
                SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_FACTORY);
				PlaySoundEffect(0);
            }
            break;
		case IDC_SETTING_BTN_START_CONNECT_TEST:
			{
#if CVTE_EN_CARPLAY
				char response[2048];
				wchar_t wresponse[2048];
				OSStatus err = kNoErr;
				err = CCarplayAdapter::GetInstance()->ConnectTester( response, _countof(response) );
				if ( err == kNoErr )
				{
					int ret = MultiByteToWideChar( CP_ACP, 0, response, -1, wresponse, _countof(wresponse) );
					if ( ret > 0 )
					{
						m_Consolestr = wresponse;
					}
					else
					{
						m_Consolestr.Format(_T("Error : %d"), GetLastError());
					}
				}
				else
				{
					m_Consolestr.Format(_T("Error : %d"), err);
				}
#endif
			}
			break;
	}
}

void CPanelSettingConnectTest::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);
	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}
}

