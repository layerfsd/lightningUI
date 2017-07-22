#include "PanelSettingIperfUDPClient.h"
#include "../../resource.h"
#include "../../CarPlay/CarplayAdapter.h"
#include <mmsystem.h>

#pragma comment(lib,"Mmtimer.lib") 

#if CVTE_EN_CARPLAY

CPanelSettingIperfUDPClient::CPanelSettingIperfUDPClient(void)
:m_bOutput(FALSE),m_testmode(0),m_testresults(0),m_uIDEvent(0)
{
}
CPanelSettingIperfUDPClient::~CPanelSettingIperfUDPClient(void)
{

}

BOOL CPanelSettingIperfUDPClient::IsReady()
{
	return (CPanelGL::IsReady());

}

void CPanelSettingIperfUDPClient::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	UpdateButtonsStatus(_T("50m"));
	m_IperfReportstr = _T("");
	m_IperfResultsstr50m = _T("");
	m_IperfResultsstr100m = _T("");
	m_IperfResultsstr150m = _T("");
	CCarplayAdapter::GetInstance()->ClearIperfReport();
	m_testmode = 0;
	m_testresults = 0;
	m_uIDEvent = ::timeSetEvent(500, 1, TimerProc, (DWORD_PTR)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

}

void CPanelSettingIperfUDPClient::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	m_bOutput = FALSE;
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingIperfUDPClient::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
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
						if ( n++ >= 90 )
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
			if(m_bOutput)
			{
				COLORGL cr_black={0.0,0.0,0.0,1.0};
				if (m_testresults == 0)
				{
					m_IperfResultsstr50m = CCarplayAdapter::GetInstance()->GetIperfResults();
					m_tagUDP50m.SetCaptionColor(&cr_black);
					m_tagUDP50m.Enable(TRUE);
				}
				else if (m_testresults == 1)
				{
					m_IperfResultsstr100m = CCarplayAdapter::GetInstance()->GetIperfResults();
					m_tagUDP100m.SetCaptionColor(&cr_black);
					m_tagUDP100m.Enable(TRUE);
				}
				else if (m_testresults == 2)
				{
					m_IperfResultsstr150m = CCarplayAdapter::GetInstance()->GetIperfResults();
					m_tagUDP150m.SetCaptionColor(&cr_black);
					m_tagUDP150m.Enable(TRUE);
				}
				m_testresults++;
			}
		}
		break;
	}
}

void CPanelSettingIperfUDPClient::Render()
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

    m_tagIperfUDPClientCmd.SetAlpha(m_fCurAlpha);
	m_tagIperfUDPClientCmd.Render();

	m_btnUDPCmd.SetAlpha(m_fCurAlpha);
	m_btnUDPCmd.Render();

	m_tagUDPResults.SetAlpha(m_fCurAlpha);
	m_tagUDPResults.Render();

	m_btnUDP50m.SetAlpha(m_fCurAlpha);
	m_btnUDP50m.Render();

	m_tagUDP50m.SetAlpha(m_fCurAlpha);
	m_tagUDP50m.Render();

	m_btnUDP100m.SetAlpha(m_fCurAlpha);
	m_btnUDP100m.Render();

	m_tagUDP100m.SetAlpha(m_fCurAlpha);
	m_tagUDP100m.Render();

	m_btnUDP150m.SetAlpha(m_fCurAlpha);
	m_btnUDP150m.Render();

	m_tagUDP150m.SetAlpha(m_fCurAlpha);
	m_tagUDP150m.Render();

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
		m_pTextGL->SetPos(-(g_iClientWidth-290)/2,m_tagConsoleOutput.GetPosY()-30);
		m_pTextGL->SetAlpha(1.0f);
		m_pTextGL->SetLuminance(1.0f);
		m_pTextGL->Render();
	}

	m_tagUDP50m.SetCaption(m_IperfResultsstr50m);
	m_tagUDP100m.SetCaption(m_IperfResultsstr100m);
	m_tagUDP150m.SetCaption(m_IperfResultsstr150m);

	EndRender();
}

BOOL CPanelSettingIperfUDPClient::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//large size:
	SetSize(g_iClientWidth-240,g_iClientHeight+1760);

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
	m_tagTitle2.SetCaption(_T("UDP Performance."));
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
    m_btnReturn.SetNotifyFunc(&CPanelSettingIperfUDPClient::OnNotify,this);
    m_btnReturn.EnableDrawBack(FALSE);

    py-=80;

	//UDP CLIENT COMMAND
	m_tagIperfUDPClientCmd.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagIperfUDPClientCmd.SetSize(cx,40);
	m_tagIperfUDPClientCmd.SetCaption(L"UDP CLIENT COMMAND");
	m_tagIperfUDPClientCmd.LinkTextGL(pTextGL);
	m_tagIperfUDPClientCmd.SetPos(-cx/2-10 +12,py);
	m_tagIperfUDPClientCmd.SetCaptionHeight(24);
	m_tagIperfUDPClientCmd.EnableDrawBack(FALSE);
	m_tagIperfUDPClientCmd.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagIperfUDPClientCmd.SetCaptionColor(&cr_dark);

	py-=60+4;

	m_btnUDPCmd.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnUDPCmd.SetSize(&sizebtn);
	m_btnUDPCmd.SetPos(0,py);
    m_btnUDPCmd.SetCaption(L"");
    m_btnUDPCmd.SetCaptionColor(&cr_gray, &cr_gray);
    m_btnUDPCmd.SetCaptionHeight(20);
    m_btnUDPCmd.LinkTextGL(pTextGL);
    m_btnUDPCmd.SetCaptionPos(BTN_CAPTION_CENTER);

	py-=80;

	//BANDWIDTH RESULTS
	m_tagUDPResults.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagUDPResults.SetSize(cx,40);
	m_tagUDPResults.SetCaption(L"UDP RESULTS");
	m_tagUDPResults.LinkTextGL(pTextGL);
	m_tagUDPResults.SetPos(-cx/2-10 +12,py);
	m_tagUDPResults.SetCaptionHeight(24);
	m_tagUDPResults.EnableDrawBack(FALSE);
	m_tagUDPResults.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagUDPResults.SetCaptionColor(&cr_dark);

	py-=60+4;

	m_btnUDP50m.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnUDP50m.SetSize(&sizebtn);
	m_btnUDP50m.SetPos(0,py);
    m_btnUDP50m.SetCaption(L"50 Mbps");
    m_btnUDP50m.SetCaptionColor(&cr_black, &cr_black);
    m_btnUDP50m.SetCaptionHeight(24);
    m_btnUDP50m.LinkTextGL(pTextGL);
    m_btnUDP50m.SetCaptionPos(BTN_CAPTION_LEFT,20);

	m_tagUDP50m.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagUDP50m.SetSize(g_iClientWidth-280-margin_x*2,80);
	m_tagUDP50m.SetCaption(L"");
	m_tagUDP50m.LinkTextGL(pTextGL);
	m_tagUDP50m.SetPos(0,py);
	m_tagUDP50m.SetCaptionHeight(24);
	m_tagUDP50m.EnableDrawBack(FALSE);
	m_tagUDP50m.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagUDP50m.SetCaptionColor(&cr_black);
	//m_tagUDP50m.Enable(FALSE);

	py-=80+4;

	m_btnUDP100m.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnUDP100m.SetSize(&sizebtn);
	m_btnUDP100m.SetPos(0,py);
    m_btnUDP100m.SetCaption(L"100 Mbps");
    m_btnUDP100m.SetCaptionColor(&cr_black, &cr_black);
    m_btnUDP100m.SetCaptionHeight(24);
    m_btnUDP100m.LinkTextGL(pTextGL);
    m_btnUDP100m.SetCaptionPos(BTN_CAPTION_LEFT,20);

	m_tagUDP100m.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagUDP100m.SetSize(g_iClientWidth-280-margin_x*2,80);
	m_tagUDP100m.SetCaption(L"");
	m_tagUDP100m.LinkTextGL(pTextGL);
	m_tagUDP100m.SetPos(0,py);
	m_tagUDP100m.SetCaptionHeight(24);
	m_tagUDP100m.EnableDrawBack(FALSE);
	m_tagUDP100m.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagUDP100m.SetCaptionColor(&cr_black);
	//m_tagUDP100m.Enable(FALSE);

	py-=80+4;

	m_btnUDP150m.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnUDP150m.SetSize(&sizebtn);
	m_btnUDP150m.SetPos(0,py);
    m_btnUDP150m.SetCaption(L"150 Mbps");
    m_btnUDP150m.SetCaptionColor(&cr_black, &cr_black);
    m_btnUDP150m.SetCaptionHeight(24);
    m_btnUDP150m.LinkTextGL(pTextGL);
    m_btnUDP150m.SetCaptionPos(BTN_CAPTION_LEFT,20);

	m_tagUDP150m.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagUDP150m.SetSize(g_iClientWidth-280-margin_x*2,80);
	m_tagUDP150m.SetCaption(L"");
	m_tagUDP150m.LinkTextGL(pTextGL);
	m_tagUDP150m.SetPos(0,py);
	m_tagUDP150m.SetCaptionHeight(24);
	m_tagUDP150m.EnableDrawBack(FALSE);
	m_tagUDP150m.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagUDP150m.SetCaptionColor(&cr_black);
	//m_tagUDP150m.Enable(FALSE);

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

	py-=20+800;

	m_btnConsoleOutput.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnConsoleOutput.SetSize(g_iClientWidth-240-margin_x*2,1600);
	m_btnConsoleOutput.SetPos(0,py);
    m_btnConsoleOutput.SetCaption(L"");
    m_btnConsoleOutput.SetCaptionColor(&cr_gray, &cr_gray);
    m_btnConsoleOutput.SetCaptionHeight(16);
    m_btnConsoleOutput.LinkTextGL(pTextGL);
    m_btnConsoleOutput.SetCaptionPos(BTN_CAPTION_LEFT);

	return TRUE;
}


void CALLBACK CPanelSettingIperfUDPClient::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingIperfUDPClient *panel=(CPanelSettingIperfUDPClient*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelSettingIperfUDPClient::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingIperfUDPClient::HandleControlMSG] idControl:%d wMsg:%d wPara:%d\r\n"),idControl,wMsg,wPara));

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

void CPanelSettingIperfUDPClient::TimerProc(UINT wTimerID,UINT msg,DWORD_PTR dwUser,DWORD dw1,DWORD dw2)
{
	CPanelSettingIperfUDPClient *panel=(CPanelSettingIperfUDPClient*)dwUser;
	if(panel)
	{
		panel->OnTimer(wTimerID,msg,dw1,dw2);
	}
}

void CPanelSettingIperfUDPClient::OnTimer(UINT wTimerID,UINT msg,DWORD dw1,DWORD dw2)
{
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	
	if (m_testmode == 0)
	{
		if (m_uIDEvent != 0)
		{
			::timeKillEvent(m_uIDEvent);
			m_uIDEvent = ::timeSetEvent(13000, 1, TimerProc, (DWORD_PTR)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
		}
		m_bOutput = TRUE;
		StartUDPTest();
	}
	else if (m_testmode == 1)
	{
		if (m_testresults != m_testmode)
		{
			m_IperfResultsstr50m = _T("Timed Out");
			m_tagUDP50m.SetCaptionColor(&cr_gray);
			m_tagUDP50m.Enable(FALSE);
			m_testresults = m_testmode;
		}
		m_bOutput = TRUE;
		StartUDPTest();
	}
	else if (m_testmode == 2)
	{
		if (m_testresults != m_testmode)
		{
			m_IperfResultsstr100m = _T("Timed Out");
			m_tagUDP100m.SetCaptionColor(&cr_gray);
			m_tagUDP100m.Enable(FALSE);
			m_testresults = m_testmode;
		}
		m_bOutput = TRUE;
		StartUDPTest();
	}
	else if (m_testmode == 3)
	{
		if (m_testresults != m_testmode)
		{
			m_IperfResultsstr150m = _T("Timed Out");
			m_tagUDP150m.SetCaptionColor(&cr_gray);
			m_tagUDP150m.Enable(FALSE);
			m_testresults = m_testmode;
		}
		m_testmode = 0;
		if (m_uIDEvent != 0)
		{
			::timeKillEvent(m_uIDEvent);
			m_bOutput = FALSE;
		}
	}
	else
	{
		if (m_uIDEvent != 0)
		{
			::timeKillEvent(m_uIDEvent);
			m_bOutput = FALSE;
		}
	}
}

void CPanelSettingIperfUDPClient::UpdateButtonsStatus(LPCWSTR inBandwidth)
{
	wchar_t formatcmd[MAX_PATH] = {0};
	wchar_t formatmsg[MAX_PATH] = {0};
	
	if (CCarplayAdapter::GetInstance()->isSessionAddressValid())
	{
		if( CCarplayAdapter::GetInstance()->GetSessionAddressFamily() == AF_INET )
		{
			swprintf(formatcmd, L"iperf -p 5001 -l 1448 -w 128k -i 1.0 -u -c\n%s -t 10.0 -b %s", CCarplayAdapter::GetInstance()->GetSessionAddressStr(), inBandwidth);
			swprintf(formatmsg, L"Make sure the device responds to ping at\n%s\nand that a UDP Iperf(2.0.5) server is running on port 5001", CCarplayAdapter::GetInstance()->GetSessionAddressStr());
		}
		else if( CCarplayAdapter::GetInstance()->GetSessionAddressFamily() == AF_INET6 )
		{
			swprintf(formatcmd, L"iperf -p 5001 -l 1448 -w 128k -i 1.0 -u -V -c\n%s -t 10.0 -b %s", CCarplayAdapter::GetInstance()->GetSessionAddressStr(), inBandwidth);
			swprintf(formatmsg, L"Make sure the device responds to ping at\n%s\nand that a UDP Iperf(2.0.5) server is running on port 5001", CCarplayAdapter::GetInstance()->GetSessionAddressStr());
		}
		else
		{
			swprintf(formatcmd, L"The IP address is error!");
		}
		m_btnUDPCmd.SetCaption(formatcmd);
		m_tagAlertMessage.SetCaption(formatmsg);
	}
}

void CPanelSettingIperfUDPClient::StartUDPTest()
{
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	if (m_testmode == 0)
	{
		m_IperfResultsstr50m = _T("Testing..");
		m_tagUDP50m.SetCaptionColor(&cr_gray);
		m_tagUDP50m.Enable(FALSE);
		CCarplayAdapter::GetInstance()->StartUDPClient(_T("50m"));
		m_testmode++;
	}
	else if (m_testmode == 1)
	{
		m_IperfResultsstr100m = _T("Testing..");
		m_tagUDP100m.SetCaptionColor(&cr_gray);
		m_tagUDP100m.Enable(FALSE);
		CCarplayAdapter::GetInstance()->StartUDPClient(_T("100m"));
		m_testmode++;
	}
	else if (m_testmode == 2)
	{
		m_IperfResultsstr150m = _T("Testing..");
		m_tagUDP150m.SetCaptionColor(&cr_gray);
		m_tagUDP150m.Enable(FALSE);
		CCarplayAdapter::GetInstance()->StartUDPClient(_T("150m"));
		m_testmode++;
	}
	else
	{
		m_testmode = 0;
	}
}

void CPanelSettingIperfUDPClient::OnMCUCmd(WPARAM wParam, LPARAM lParam)
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

