#include "PanelSettingIperfTCPClient.h"
#include "../../resource.h"
#include "../../CarPlay/CarplayAdapter.h"

#if CVTE_EN_CARPLAY
CPanelSettingIperfTCPClient::CPanelSettingIperfTCPClient(void)
:m_bOutput(FALSE),m_testmode(FALSE),m_uIDEvent(0)
{
}
CPanelSettingIperfTCPClient::~CPanelSettingIperfTCPClient(void)
{

}

BOOL CPanelSettingIperfTCPClient::IsReady()
{
	return (CPanelGL::IsReady());

}

void CPanelSettingIperfTCPClient::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	UpdateButtonsStatus();
	m_IperfReportstr = _T("");
	m_IperfResultsstr = _T("Testing..");
	m_tagBandwidth.SetCaptionColor(&cr_gray);
	m_tagBandwidth.Enable(FALSE);
	CCarplayAdapter::GetInstance()->ClearIperfReport();
	m_testmode = FALSE;
	m_uIDEvent = ::timeSetEvent(500, 1, TimerProc, (DWORD_PTR)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

}

void CPanelSettingIperfTCPClient::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	m_bOutput = FALSE;
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingIperfTCPClient::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case CARPLAY_IPERF_REPORT:
		{
			if(m_bOutput)
			{
				if (CCarplayAdapter::GetInstance()->GetIperfReport().GetLength() != 0)
				{
					int strpt = 0;
					int n = 0;
					do
					{
						strpt = CCarplayAdapter::GetInstance()->GetIperfReport().Find('\n', strpt+1);
						if ( n++ >= 19 )
						{
							break;
						}
					}while(strpt != -1);
					if (strpt == -1)
					{
						m_IperfReportstr = CCarplayAdapter::GetInstance()->GetIperfReport();
					}
					else
					{
						m_IperfReportstr = CCarplayAdapter::GetInstance()->GetIperfReport().Left(strpt);
					}
				}
				else
				{
					m_IperfReportstr = _T("");
				}
			}
		}
		break;
	case CARPLAY_IPERF_RESULTS:
		{
			COLORGL cr_black={0.0,0.0,0.0,1.0};
			if(m_bOutput)
			{
				if (m_uIDEvent != 0)
				{
					::timeKillEvent(m_uIDEvent);
				}
				m_testmode = FALSE;
				m_IperfResultsstr = CCarplayAdapter::GetInstance()->GetIperfResults();
				m_tagBandwidth.SetCaptionColor(&cr_black);
				m_tagBandwidth.Enable(TRUE);
				m_bOutput = FALSE;
			}
		}
		break;
	}
}

void CPanelSettingIperfTCPClient::Render()
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

	m_tagIperfTCPClientCmd.SetAlpha(m_fCurAlpha);
	m_tagIperfTCPClientCmd.Render();

	m_btnTCPCmd.SetAlpha(m_fCurAlpha);
	m_btnTCPCmd.Render();

	m_tagBandwidthResults.SetAlpha(m_fCurAlpha);
	m_tagBandwidthResults.Render();

	m_btnBandwidth.SetAlpha(m_fCurAlpha);
	m_btnBandwidth.Render();

	m_tagBandwidth.SetAlpha(m_fCurAlpha);
	m_tagBandwidth.Render();

	m_tagAlertMessage.SetAlpha(m_fCurAlpha);
	m_tagAlertMessage.Render();

	m_tagConsoleOutput.SetAlpha(m_fCurAlpha);
	m_tagConsoleOutput.Render();

	m_btnConsoleOutput.SetAlpha(m_fCurAlpha);
	m_btnConsoleOutput.Render();

    COLORGL cr_b={0.35,0.35,0.35,1.0};

	DrawRect(0,g_iClientHeight/2-50-80/2,g_iClientWidth-210-50,1,&cr_b);

	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	//
	if(m_pTextGL)
	{
		SIZE textsize;
		m_pTextGL->SetString(m_IperfReportstr);
		m_pTextGL->SetColor(&cr_gray,FALSE);
		m_pTextGL->SetCharScale(11.0f/32.0f,11.0f/32.0f);
		m_pTextGL->GetExtension(&textsize);
		m_pTextGL->SetPos(-(g_iClientWidth-290)/2-20,m_tagConsoleOutput.GetPosY()-30);
		m_pTextGL->SetAlpha(1.0f);
		m_pTextGL->SetLuminance(1.0f);
		m_pTextGL->Render();
	}

	m_tagBandwidth.SetCaption(m_IperfResultsstr);

	EndRender();
}

BOOL CPanelSettingIperfTCPClient::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//large size:
	SetSize(g_iClientWidth-240,g_iClientHeight+500);

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
	m_tagTitle2.SetCaption(_T("TCP Performance."));
	m_tagTitle2.LinkTextGL(pTextGL);
	m_tagTitle2.SetPos(10,py-12);
	m_tagTitle2.SetCaptionHeight(16);
	m_tagTitle2.EnableDrawBack(FALSE);
	m_tagTitle2.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle2.SetCaptionColor(&cr_gray2);

    //m_labIcon.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_623));
	//m_labIcon.SetPos(170+10+32,py);
    m_labIconFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_012));
	m_labIconFocus.SetPos(170+30+32,py);
	m_labIconFocus.SetLuminance(0.2);
    //m_labIconFocus.Show(FALSE);

    m_btnReturn.Initialize(IDC_SETTING_BTN_RETURN,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    m_btnReturn.SetPos(170+30+32,py);
    m_btnReturn.SetNotifyFunc(&CPanelSettingIperfTCPClient::OnNotify,this);
    m_btnReturn.EnableDrawBack(FALSE);

    py-=80;

	//TCP CLIENT COMMAND
	m_tagIperfTCPClientCmd.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagIperfTCPClientCmd.SetSize(cx,40);
	m_tagIperfTCPClientCmd.SetCaption(L"TCP CLIENT COMMAND");
	m_tagIperfTCPClientCmd.LinkTextGL(pTextGL);
	m_tagIperfTCPClientCmd.SetPos(-cx/2-10 +12,py);
	m_tagIperfTCPClientCmd.SetCaptionHeight(24);
	m_tagIperfTCPClientCmd.EnableDrawBack(FALSE);
	m_tagIperfTCPClientCmd.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagIperfTCPClientCmd.SetCaptionColor(&cr_dark);

	py-=60+4;

	m_btnTCPCmd.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTCPCmd.SetSize(&sizebtn);
	m_btnTCPCmd.SetPos(0,py);
    m_btnTCPCmd.SetCaption(L"");
    m_btnTCPCmd.SetCaptionColor(&cr_gray, &cr_gray);
    m_btnTCPCmd.SetCaptionHeight(20);
    m_btnTCPCmd.LinkTextGL(pTextGL);
    m_btnTCPCmd.SetCaptionPos(BTN_CAPTION_CENTER);

	py-=80;

	//BANDWIDTH RESULTS
	m_tagBandwidthResults.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagBandwidthResults.SetSize(cx,40);
	m_tagBandwidthResults.SetCaption(L"BANDWIDTH RESULTS");
	m_tagBandwidthResults.LinkTextGL(pTextGL);
	m_tagBandwidthResults.SetPos(-cx/2-10 +12,py);
	m_tagBandwidthResults.SetCaptionHeight(24);
	m_tagBandwidthResults.EnableDrawBack(FALSE);
	m_tagBandwidthResults.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagBandwidthResults.SetCaptionColor(&cr_dark);

	py-=60+4;

	m_btnBandwidth.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnBandwidth.SetSize(&sizebtn);
	m_btnBandwidth.SetPos(0,py);
    m_btnBandwidth.SetCaption(L"Bandwidth");
    m_btnBandwidth.SetCaptionColor(&cr_black, &cr_black);
    m_btnBandwidth.SetCaptionHeight(24);
    m_btnBandwidth.LinkTextGL(pTextGL);
    m_btnBandwidth.SetCaptionPos(BTN_CAPTION_LEFT,20);

	m_tagBandwidth.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagBandwidth.SetSize(g_iClientWidth-280-margin_x*2,80);
	m_tagBandwidth.SetCaption(L"");
	m_tagBandwidth.LinkTextGL(pTextGL);
	m_tagBandwidth.SetPos(0,py);
	m_tagBandwidth.SetCaptionHeight(24);
	m_tagBandwidth.EnableDrawBack(FALSE);
	m_tagBandwidth.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagBandwidth.SetCaptionColor(&cr_black);
	//m_tagBandwidth.Enable(FALSE);

	py-=100;

	//Alert Message:
	m_tagAlertMessage.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlertMessage.SetSize(cx,40);
	m_tagAlertMessage.SetCaption(L"");
	m_tagAlertMessage.LinkTextGL(pTextGL);
	m_tagAlertMessage.SetPos(-cx/2-10 +12,py);
	m_tagAlertMessage.SetCaptionHeight(16);
	m_tagAlertMessage.EnableDrawBack(FALSE);
	m_tagAlertMessage.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlertMessage.SetCaptionColor(&cr_dark);

	py-=100;

	//CONSOLE OUTPUT
	m_tagConsoleOutput.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagConsoleOutput.SetSize(cx,40);
	m_tagConsoleOutput.SetCaption(L"CONSOLE OUTPUT");
	m_tagConsoleOutput.LinkTextGL(pTextGL);
	m_tagConsoleOutput.SetPos(-cx/2-10 +12,py);
	m_tagConsoleOutput.SetCaptionHeight(24);
	m_tagConsoleOutput.EnableDrawBack(FALSE);
	m_tagConsoleOutput.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagConsoleOutput.SetCaptionColor(&cr_dark);

	py-=20+250;

	m_btnConsoleOutput.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnConsoleOutput.SetSize(g_iClientWidth-180-margin_x*2,500);
	m_btnConsoleOutput.SetPos(0,py);
    m_btnConsoleOutput.SetCaption(L"");
    m_btnConsoleOutput.SetCaptionColor(&cr_gray, &cr_gray); 
	m_btnConsoleOutput.LinkTextGL(pTextGL);
    m_btnConsoleOutput.SetCaptionHeight(16);
    m_btnConsoleOutput.SetCaptionPos(BTN_CAPTION_LEFT);

	return TRUE;
}


void CALLBACK CPanelSettingIperfTCPClient::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingIperfTCPClient *panel=(CPanelSettingIperfTCPClient*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelSettingIperfTCPClient::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingIperfTCPClient::HandleControlMSG] idControl:%d wMsg:%d wPara:%d\r\n"),idControl,wMsg,wPara));

	switch (idControl)
	{
        case IDC_SETTING_BTN_RETURN:
            {
                SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_IPERF_CLIENT);
				PlaySoundEffect(0);
            }
            break;
	}
}

void CPanelSettingIperfTCPClient::TimerProc(UINT wTimerID,UINT msg,DWORD_PTR dwUser,DWORD dw1,DWORD dw2)
{
	CPanelSettingIperfTCPClient *panel=(CPanelSettingIperfTCPClient*)dwUser;
	if(panel)
	{
		panel->OnTimer(wTimerID,msg,dw1,dw2);
	}
}

void CPanelSettingIperfTCPClient::OnTimer(UINT wTimerID,UINT msg,DWORD dw1,DWORD dw2)
{
	COLORGL cr_gray={0.25,0.25,0.25,1.0};

	if (m_uIDEvent != 0)
	{
		::timeKillEvent(m_uIDEvent);
	}
	
	if (m_testmode)
	{
		m_testmode = FALSE;
		m_IperfResultsstr = _T("Timed Out");
		m_tagBandwidth.SetCaptionColor(&cr_gray);
		m_tagBandwidth.Enable(FALSE);
		m_bOutput = FALSE;
	}
	else
	{
		m_testmode = TRUE;
		m_uIDEvent = ::timeSetEvent(13000, 1, TimerProc, (DWORD_PTR)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
		m_bOutput = TRUE;
		CCarplayAdapter::GetInstance()->StartTCPClient();
	}
}

void CPanelSettingIperfTCPClient::UpdateButtonsStatus()
{
	wchar_t formatcmd[MAX_PATH] = {0};
	wchar_t formatmsg[MAX_PATH] = {0};
	
	if (CCarplayAdapter::GetInstance()->isSessionAddressValid())
	{
		if( CCarplayAdapter::GetInstance()->GetSessionAddressFamily() == AF_INET )
		{
			swprintf(formatcmd, L"iperf -p 5001 -l 128k -w 128k -i 1.0 -c\n%s -t 10.0", CCarplayAdapter::GetInstance()->GetSessionAddressStr());
			swprintf(formatmsg, L"Make sure the device responds to ping at\n%s\nand that a TCP Iperf(2.0.5) server is running on port 5001", CCarplayAdapter::GetInstance()->GetSessionAddressStr());
		}
		else if( CCarplayAdapter::GetInstance()->GetSessionAddressFamily() == AF_INET6 )
		{
			swprintf(formatcmd, L"iperf -p 5001 -l 128k -w 128k -i 1.0 -V -c\n%s -t 10.0", CCarplayAdapter::GetInstance()->GetSessionAddressStr());
			swprintf(formatmsg, L"Make sure the device responds to ping at\n%s\nand that a TCP Iperf(2.0.5) server is running on port 5001", CCarplayAdapter::GetInstance()->GetSessionAddressStr());
		}
		else
		{
			swprintf(formatcmd, L"The IP address is error!");
		}
		m_btnTCPCmd.SetCaption(formatcmd);
		m_tagAlertMessage.SetCaption(formatmsg);
	}
}

void CPanelSettingIperfTCPClient::OnMCUCmd(WPARAM wParam, LPARAM lParam)
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

