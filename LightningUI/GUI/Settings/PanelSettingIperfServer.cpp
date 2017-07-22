#include "PanelSettingIperfServer.h"
#include "../../resource.h"
#include "../../CarPlay/CarplayAdapter.h"

CPanelSettingIperfServer::CPanelSettingIperfServer(void)
:m_bServerIPAddressUpdated(FALSE)
{
	
}
CPanelSettingIperfServer::~CPanelSettingIperfServer(void)
{

}

BOOL CPanelSettingIperfServer::IsReady()
{
	return (CPanelGL::IsReady());

}

void CPanelSettingIperfServer::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
	ServerIPAddressUpdate();
}

void CPanelSettingIperfServer::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingIperfServer::ServerIPAddressUpdate()
{
	OSStatus err;
	size_t paddresslen = 0;
	char paddress[MAX_PATH] = {0};
	wchar_t pwcaddress[MAX_PATH] = {0};
	wchar_t formatcmd[MAX_PATH] = {0};

	err = CarplayAPIGetServerAddress( paddress, _countof(paddress) );
	if ( err == kNoErr )
	{
		paddresslen = strlen(paddress);
		if (paddresslen != 0)
		{
			paddresslen = MultiByteToWideChar( CP_ACP, 0, paddress, -1, pwcaddress, _countof(pwcaddress));
			if ( paddresslen > 0 )
			{
				m_btnServerIPAddress.SetCaption(pwcaddress);
				m_tagAlertMessage.SetCaption(L"Make Sure the server responds to ping from the CarPlay \nDevice.Use the below TCP & UDP Iperf Client commands \nto run performance tests.");
				m_tagAlertTitleTCP.SetCaption(L"TCP:");
				swprintf(formatcmd, L"iperf -c %s \n-w 128k -t 10.0 -i 1.0 -p 5001 -V", pwcaddress);
				m_tagAlertCmdTCP.SetCaption(formatcmd);
				m_tagAlertTitleUDP.SetCaption(L"UDP:");
				swprintf(formatcmd, L"iperf -c %s \n-w 128k -t 10.0 -i 1.0 -p 5001 -V -l 1448 -u -b 100m", pwcaddress );
				m_tagAlertCmdUDP.SetCaption(formatcmd);
			}
		}
	}
	if ( paddresslen == 0 )
	{
		m_btnServerIPAddress.SetCaption(L"");
		m_tagAlertMessage.SetCaption(L"No network interfaces are available for incoming \nconnections.Try disconnecting and reconnecting the \nCarPlay Device.");
		m_tagAlertTitleTCP.SetCaption(L"");
		m_tagAlertCmdTCP.SetCaption(L"");
		m_tagAlertTitleUDP.SetCaption(L"");
		m_tagAlertCmdUDP.SetCaption(L"");
	}
}

void CPanelSettingIperfServer::OnCarPlayMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case CARPLAY_CONTROL_STATUS:
		{
			m_bServerIPAddressUpdated = TRUE;
		}
		break;
	}
}

void CPanelSettingIperfServer::Render()
{
	if(!BeginRender())return;

//	    SetFrameTime(30);

	if (m_bServerIPAddressUpdated)
	{
		m_bServerIPAddressUpdated = FALSE;
		ServerIPAddressUpdate();
	}

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

	m_tagIperfServerCmd.SetAlpha(m_fCurAlpha);
    m_tagIperfServerCmd.Render();

	m_btnTCPCmd.SetAlpha(m_fCurAlpha);
    m_btnTCPCmd.Render();

	m_tagTCPCmd.SetAlpha(m_fCurAlpha);
    m_tagTCPCmd.Render();

	m_btnUDPCmd.SetAlpha(m_fCurAlpha);
    m_btnUDPCmd.Render();

	m_tagUDPCmd.SetAlpha(m_fCurAlpha);
    m_tagUDPCmd.Render();

	m_tagIperfServerIPAddress.SetAlpha(m_fCurAlpha);
    m_tagIperfServerIPAddress.Render();

	m_btnServerIPAddress.SetAlpha(m_fCurAlpha);
    m_btnServerIPAddress.Render();

	m_tagAlertMessage.SetAlpha(m_fCurAlpha);
    m_tagAlertMessage.Render();

	m_tagAlertTitleTCP.SetAlpha(m_fCurAlpha);
    m_tagAlertTitleTCP.Render();

	m_tagAlertCmdTCP.SetAlpha(m_fCurAlpha);
    m_tagAlertCmdTCP.Render();

	m_tagAlertTitleUDP.SetAlpha(m_fCurAlpha);
    m_tagAlertTitleUDP.Render();

	m_tagAlertCmdUDP.SetAlpha(m_fCurAlpha);
    m_tagAlertCmdUDP.Render();

    COLORGL cr_b={0.35,0.35,0.35,1.0};

	DrawRect(0,g_iClientHeight/2-50-80/2,g_iClientWidth-210-50,1,&cr_b);

	EndRender();
}

BOOL CPanelSettingIperfServer::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//large size:
	SetSize(g_iClientWidth-240,g_iClientHeight+210);

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
	m_tagTitle1.SetCaption(L"Iperf Server Test");
	m_tagTitle1.LinkTextGL(pTextGL);
	m_tagTitle1.SetPos(10,py+24);
	m_tagTitle1.SetCaptionHeight(24);
	m_tagTitle1.EnableDrawBack(FALSE);
	m_tagTitle1.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle1.SetCaptionColor(&cr_dark);

    m_tagTitle2.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle2.SetSize((g_iClientWidth-210-margin_x*2),80);
	m_tagTitle2.SetCaption(L"Please restart the head unit to exit iperf server.");
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
    m_btnReturn.SetNotifyFunc(&CPanelSettingIperfServer::OnNotify,this);
    m_btnReturn.EnableDrawBack(FALSE);

    py-=60+4;

	//IPERF SERVER COMMAND:
	m_tagIperfServerCmd.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagIperfServerCmd.SetSize(cx,40);
	m_tagIperfServerCmd.SetCaption(L"IPERF SERVER COMMAND");
	m_tagIperfServerCmd.LinkTextGL(pTextGL);
	m_tagIperfServerCmd.SetPos(-cx/2-10 +12,py);
	m_tagIperfServerCmd.SetCaptionHeight(22);
	m_tagIperfServerCmd.EnableDrawBack(FALSE);
	m_tagIperfServerCmd.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagIperfServerCmd.SetCaptionColor(&cr_dark);

	py-=60+4;
    
    m_btnTCPCmd.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTCPCmd.SetSize(&sizebtn);
	m_btnTCPCmd.SetPos(0,py);
    m_btnTCPCmd.SetCaption(L"TCP");
    m_btnTCPCmd.SetCaptionColor(&cr_black, &cr_black);
    m_btnTCPCmd.SetCaptionHeight(22);
    m_btnTCPCmd.LinkTextGL(pTextGL);
    m_btnTCPCmd.SetCaptionPos(BTN_CAPTION_LEFT,20);

    m_tagTCPCmd.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTCPCmd.SetSize(cx,40);
	m_tagTCPCmd.SetCaption(L"iperf -p 5001 -l 128k -w 128k -i 1 -V -s");
	m_tagTCPCmd.LinkTextGL(pTextGL);
	m_tagTCPCmd.SetPos(-cx/2+80,py);
	m_tagTCPCmd.SetCaptionHeight(18);
	m_tagTCPCmd.EnableDrawBack(FALSE);
	m_tagTCPCmd.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTCPCmd.SetCaptionColor(&cr_gray);

	py-=80;

    m_btnUDPCmd.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnUDPCmd.SetSize(&sizebtn);
	m_btnUDPCmd.SetPos(0,py);
    m_btnUDPCmd.SetCaption(L"UDP");
    m_btnUDPCmd.SetCaptionColor(&cr_black, &cr_black);
    m_btnUDPCmd.SetCaptionHeight(22);
    m_btnUDPCmd.LinkTextGL(pTextGL);
    m_btnUDPCmd.SetCaptionPos(BTN_CAPTION_LEFT,20);

	m_tagUDPCmd.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagUDPCmd.SetSize(cx,40);
	m_tagUDPCmd.SetCaption(L"iperf -p 5001 -l 1448 -w 256k -i 1 -V -u -s");
	m_tagUDPCmd.LinkTextGL(pTextGL);
	m_tagUDPCmd.SetPos(-cx/2+80,py);
	m_tagUDPCmd.SetCaptionHeight(18);
	m_tagUDPCmd.EnableDrawBack(FALSE);
	m_tagUDPCmd.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagUDPCmd.SetCaptionColor(&cr_gray);

	py-=80+4;

	//IPERF SERVER IP ADDRESS:
	m_tagIperfServerIPAddress.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagIperfServerIPAddress.SetSize(cx,40);
	m_tagIperfServerIPAddress.SetCaption(L"IPERF SERVER IP ADDRESS");
	m_tagIperfServerIPAddress.LinkTextGL(pTextGL);
	m_tagIperfServerIPAddress.SetPos(-cx/2-10 +12,py);
	m_tagIperfServerIPAddress.SetCaptionHeight(22);
	m_tagIperfServerIPAddress.EnableDrawBack(FALSE);
	m_tagIperfServerIPAddress.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagIperfServerIPAddress.SetCaptionColor(&cr_dark);

	py-=60+4;
    
    m_btnServerIPAddress.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnServerIPAddress.SetSize(&sizebtn);
	m_btnServerIPAddress.SetPos(0,py);
	//m_btnServerIPAddress.SetNotifyFunc(&CPanelSettingIperfServer::OnNotify,this);
    m_btnServerIPAddress.SetCaption(L"");
    m_btnServerIPAddress.SetCaptionColor(&cr_black, &cr_black);
    m_btnServerIPAddress.SetCaptionHeight(20);
    m_btnServerIPAddress.LinkTextGL(pTextGL);
    m_btnServerIPAddress.SetCaptionPos(BTN_CAPTION_LEFT,20);

	py-=80+4;

	//IPERF SERVER IP ADDRESS:
	m_tagAlertMessage.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlertMessage.SetSize(cx,40);
	m_tagAlertMessage.SetCaption(L"");
	m_tagAlertMessage.LinkTextGL(pTextGL);
	m_tagAlertMessage.SetPos(-cx/2-10 +12,py);
	m_tagAlertMessage.SetCaptionHeight(16);
	m_tagAlertMessage.EnableDrawBack(FALSE);
	m_tagAlertMessage.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlertMessage.SetCaptionColor(&cr_dark);

	py-=80+4;
	
	//TCP:
	m_tagAlertTitleTCP.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlertTitleTCP.SetSize(cx,40);
	m_tagAlertTitleTCP.SetCaption(_T(""));
	m_tagAlertTitleTCP.LinkTextGL(pTextGL);
	m_tagAlertTitleTCP.SetPos(-cx/2-10 +12,py);
	m_tagAlertTitleTCP.SetCaptionHeight(16);
	m_tagAlertTitleTCP.EnableDrawBack(FALSE);
	m_tagAlertTitleTCP.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlertTitleTCP.SetCaptionColor(&cr_dark);

	m_tagAlertCmdTCP.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlertCmdTCP.SetSize(cx-70,60);
	m_tagAlertCmdTCP.SetCaption(L"");
	m_tagAlertCmdTCP.LinkTextGL(pTextGL);
	m_tagAlertCmdTCP.SetPos(-cx/2-10+12+35,py);
	m_tagAlertCmdTCP.SetCaptionHeight(16);
	m_tagAlertCmdTCP.EnableDrawBack(FALSE);
	m_tagAlertCmdTCP.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlertCmdTCP.SetCaptionColor(&cr_dark);

	py-=70+4;
	
	//UDP:
	m_tagAlertTitleUDP.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlertTitleUDP.SetSize(cx,40);
	m_tagAlertTitleUDP.SetCaption(L"");
	m_tagAlertTitleUDP.LinkTextGL(pTextGL);
	m_tagAlertTitleUDP.SetPos(-cx/2-10 +12,py);
	m_tagAlertTitleUDP.SetCaptionHeight(16);
	m_tagAlertTitleUDP.EnableDrawBack(FALSE);
	m_tagAlertTitleUDP.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlertTitleUDP.SetCaptionColor(&cr_dark);

	m_tagAlertCmdUDP.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAlertCmdUDP.SetSize(cx-70,60);
	m_tagAlertCmdUDP.SetCaption(L"");
	m_tagAlertCmdUDP.LinkTextGL(pTextGL);
	m_tagAlertCmdUDP.SetPos(-cx/2-10+12+35,py);
	m_tagAlertCmdUDP.SetCaptionHeight(16);
	m_tagAlertCmdUDP.EnableDrawBack(FALSE);
	m_tagAlertCmdUDP.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagAlertCmdUDP.SetCaptionColor(&cr_dark);
	

	return TRUE;
}


void CALLBACK CPanelSettingIperfServer::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingIperfServer *panel=(CPanelSettingIperfServer*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelSettingIperfServer::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingIperfServer::HandleControlMSG] idControl:%d wMsg:%d wPara:%d\r\n"),idControl,wMsg,wPara));

	switch (idControl)
	{
        case IDC_SETTING_BTN_RETURN:
            {
                SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_FACTORY);
				PlaySoundEffect(0);
            }
            break;
	}
}

void CPanelSettingIperfServer::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);
	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}
}

