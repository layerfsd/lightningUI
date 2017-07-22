#include "PanelVehicleInfoWrapper.h"
#include "../../resource.h"
#include "../../CarPlay/CarplayAdapter.h"

#define VEHICLE_INFO_EVENT  TEXT("VEHICLE_INFO_EVENT")

#define CAN_SLAVE_TYPE_SPEED								0x16
#define CAN_SLAVE_TYPE_KEY									0x20
#define CAN_SLAVE_TYPE_AIR									0x21
#define CAN_SLAVE_TYPE_RADAR_REAR					0x22
#define CAN_SLAVE_TYPE_RADAR_FRONT				0x23
#define CAN_SLAVE_TYPE_COMMON						0x24
#define CAN_SLAVE_TYPE_PARKING_ASSI					0x25
#define CAN_SLAVE_TYPE_STEER								0x26
#define CAN_SLAVE_TYPE_VCHICLE_BODY				0x41

#define CAN_HOST_TYPE_START_END						0x81
#define CAN_HOST_TYPE_QUERY								0x90
#define CAN_HOST_TYPE_SOURCE							0xC0
#define CAN_HOST_TYPE_ICON								0xC1
#define CAN_HOST_TYPE_RADIO								0xC2
#define CAN_HOST_TYPE_MEDIA								0xC3
#define CAN_HOST_TYPE_VOLUME							0xC4
#define CAN_HOST_TYPE_RADAR_VOL						0xC6


extern WPARAM g_wParamVolumeInfo;
extern WPARAM g_lParamVolumeInfo;

extern WPARAM g_wParamSourceInfo;
extern WPARAM g_lParamSourceInfo;

extern WPARAM g_wParamMediaPlaybackInfo;
extern WPARAM g_lParamMediaPlaybackInfo;

extern WPARAM g_wParamRadioInfo;
extern WPARAM g_lParamRadioInfo;



//CPanelVehicleInfoWrapper
CPanelVehicleInfoWrapper::CPanelVehicleInfoWrapper(void)
{
	m_idCurSubpage = PAGE_VEHICLE_DRIVE;

	m_hACKEvent = INVALID_HANDLE_VALUE;
	m_bQuickMode = FALSE;
	m_bRadarOn=FALSE;
	m_bAssiOn=FALSE;
	m_dwAssiNotifyTick=0;
	m_bAutoShowRadar=FALSE;

	m_bHasCanBus = FALSE/*TRUE*/;
	m_bConnecting = FALSE;

	m_dwLastShowACTick = 0;
	m_dwLastShowBodyTick = 0;
	m_dwLastShowRadarTick = 0;

	m_idQuickShowPage = -1;
	m_idLastPage = -1;

	m_pCmdSender  = NULL;
	m_dwEnterTick = 0;
	m_bIniQueryData = FALSE;

	m_uRadarData = 0;
	m_dwLastTick = 0;
	m_hRadarInfoEvent = INVALID_HANDLE_VALUE;

	m_doorStatus = 0;
	m_bGotTpmsSensorID = FALSE;

	m_dwShowACTimeout = 3000;
	m_dwShowBodyTimeout = 3000;
	m_dwShowRadarTimeout = 3000;

	m_dwTpmsAlarmTick = 0;
	m_dwShowTpmsAlarmTick = 0;

	m_bSpeechOn = FALSE;
	m_bPhoneCallActive = FALSE;
}

CPanelVehicleInfoWrapper::~CPanelVehicleInfoWrapper(void)
{
	if(m_hACKEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hACKEvent);
// 	if(m_hCmdBuffReadyEvent != INVALID_HANDLE_VALUE)
// 		CloseHandle(m_hCmdBuffReadyEvent);

	if(m_pCmdSender)
	{
		delete m_pCmdSender;
		m_pCmdSender = NULL;
	}
}
BOOL CPanelVehicleInfoWrapper::IsReady()
{
	CPanelGL *panel=GetChildPanel(m_idCurSubpage);

	return CPanelGL::IsReady() && (panel?panel->IsReady():TRUE);
}
void CPanelVehicleInfoWrapper::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//make animation:
	if(idAni!=ANIM_NONE)
	{
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
	}


	if(iParam)
	{
		m_idCurSubpage =LOWORD(iParam);
		m_bQuickMode = HIWORD(iParam);

		m_boxMenu.MoveToIndex(m_boxMenu.SearchID(m_idCurSubpage));
	}
	else
	{
		m_bQuickMode=FALSE;
	}

	RETAILMSG(1,(_T("CPanelVehicleInfoWrapper::OnEnterPanel: m_bQuickMode %d, m_idCurSubpage: %d\r\n"),m_bQuickMode,m_idCurSubpage));

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnEnterPanel();
	}

	m_dwEnterTick = GetTickCount();

}
void CPanelVehicleInfoWrapper::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnQuitPanel();
	}

	//
	m_idQuickShowPage = -1;
	m_idLastPage = -1;
}

void CPanelVehicleInfoWrapper::NotifySourceInfo(BYTE iSourceId,BYTE iMediaType)
{
	if(m_bHasCanBus)
	{
		static BYTE s_SourceId=0;
		static WORD s_MediaType=0;

		if(s_SourceId!=iSourceId || s_MediaType!=iMediaType)
		{
			BYTE buff[8]={CAN_HOST_TYPE_SOURCE,0x02,iSourceId,iMediaType};

			SendCanbusCommand(buff,4);

			s_SourceId=iSourceId;
			s_MediaType=iMediaType;

		}
	}

}
void CPanelVehicleInfoWrapper::NotifyRadioInfo(BYTE iBand,WORD iFreq,BYTE iPreset,BOOL bAlwaysSet)
{
	if(m_bHasCanBus)
	{
		static BYTE s_band=0;
		static WORD s_iFreq=0;
		static BYTE s_iPreset=0;

		if(iBand!=s_band || iFreq!=s_iFreq || iPreset!=s_iPreset || bAlwaysSet)
		{
	// 		RETAILMSG(DEBUG_VEHICLEINFO,(_T("CPanelVehicleInfoWrapper::NotifyRadioInfo iBand:%d,iFreq:%d,iPreset:%d\r\n"),iBand,iFreq,iPreset));

			BYTE buff[8]={CAN_HOST_TYPE_RADIO,0x04,iBand,BYTE(iFreq),BYTE(iFreq>>8),iPreset};

			SendCanbusCommand(buff,6);

			s_band=iBand;
			s_iFreq=iFreq;
			s_iPreset=iPreset;

		}
	}

}
void CPanelVehicleInfoWrapper::NotifyVolumeInfo(INT iVol, BOOL bMute)
{
	if(m_bHasCanBus)
	{
		static BYTE s_set=0;

		int vol=iVol;//*40/64; //max 40

		BYTE set=bMute?(0x80+(BYTE(vol)&0x7F)):((BYTE(vol)&0x7F));

		if(s_set!=set)
		{
			BYTE buff[8]={CAN_HOST_TYPE_VOLUME,0x01,set};
			SendCanbusCommand(buff,3);

			s_set = set;
		}
	}

}
void CPanelVehicleInfoWrapper::NotifyMediaPlaybackInfo(DWORD dwData1, DWORD dwData2)
{
	if(m_bHasCanBus)
	{
		static DWORD s_data1=0;
		static DWORD s_data2=0;

		if(s_data1!=dwData1 || s_data2!=dwData2)
		{
			BYTE buff[8]={CAN_HOST_TYPE_MEDIA,0x06,BYTE(dwData1>>24),BYTE(dwData1>>16),BYTE(dwData1>>8),BYTE(dwData1),BYTE(dwData2>>24),BYTE(dwData2>>16)};
			SendCanbusCommand(buff,8);

			s_data1 = dwData1;
			s_data2 = dwData2;
		}
	}

}
void CPanelVehicleInfoWrapper::OnSRCChanged(UINT idSRC)
{

}


void CPanelVehicleInfoWrapper::Render()
{

	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_301):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(!BeginRender())return;

	DrawRect(pTexBack,0,0,g_iClientWidth-4,g_iClientHeight-4,2,&cr,TRUE);

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	m_boxMenu.SetAlpha(m_fCurAlpha);
	m_boxMenu.Render();


	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->SetAlpha(m_fCurAlpha);
		panel->Render();
	}

	//
	//if(m_bQuickMode)
	{
		if(m_idQuickShowPage == PAGE_VEHICLE_AC)
		{
			if(GetTickCount()-m_dwEnterTick>3000 && GetTickCount()-m_dwLastShowACTick>m_dwShowACTimeout)
			{
				//on back:
				if(m_bQuickMode)
					SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
				else if(m_idLastPage!=-1)
					ShowPage(m_idLastPage);
				m_idQuickShowPage = -1;
				m_idLastPage = -1;
			}
		}
		else if(m_idQuickShowPage == PAGE_VEHICLE_BODY)
		{
			if(GetTickCount()-m_dwEnterTick>3000 
				&& GetTickCount()-m_dwTpmsAlarmTick>m_dwShowBodyTimeout
				&& GetTickCount()-m_dwLastShowBodyTick>m_dwShowBodyTimeout)
			{
				//on back:
				if(m_bQuickMode)
					SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
				else if(m_idLastPage!=-1)
					ShowPage(m_idLastPage);
				m_idQuickShowPage = -1;
				m_idLastPage = -1;
			}
		}
		else if(m_idQuickShowPage == PAGE_VEHICLE_RADAR)
		{
			if(!m_bAssiOn && GetTickCount()-m_dwEnterTick>3000 && GetTickCount()-m_dwLastShowRadarTick>m_dwShowRadarTimeout)
			{
				//on back:
				if(m_bQuickMode)
					SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
				else if(m_idLastPage!=-1)
					ShowPage(m_idLastPage);
				m_idQuickShowPage = -1;
				m_idLastPage = -1;
			}
		}
	}

	EndRender();
}

void CPanelVehicleInfoWrapper::OnTimerProc()
{
	static DWORD cnt=0;

// 	RETAILMSG(DEBUG_VEHICLEINFO,(_T("----CPanelVehicleInfoWrapper::OnTimerProc,cnt:%d\r\n"),cnt));

	BOOL bIniUIReady=m_pSkinManager->GetGUI()->IsIniUIReady();

	if(!bIniUIReady || m_bSpeechOn || m_bPhoneCallActive)
	{
		return;
	}

	if(!m_bIniQueryData)
	{
		RETAILMSG(DEBUG_VEHICLEINFO,(_T("----CPanelVehicleInfoWrapper::m_bIniQueryData,cnt:%d\r\n"),cnt));
		m_bIniQueryData = TRUE;
		//RequestSpeedInfo();

		//
		RequestVchicleBodyInfo(0x2);
	}
	else if(m_idSystemPowerState == SYS_POWER_ON)
	{
//			if(m_idCurSubpage==PAGE_VEHICLE_DRIVE &&
//				GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_VEHICLEINFO)
//			{
//				RequestVchicleBodyInfo(0x2);
//			}

		//request speed info per 200ms
		if(cnt%2 == 0)
		{
			RequestVchicleBodyInfo(0x2);//RequestSpeedInfo();
		}
	}

	//auto show radar page
	if(m_bAutoShowRadar)
	{
		if(m_bAssiOn && !m_bOnReverseMode && GetTickCount()-m_dwAssiNotifyTick>1000)
		{
			m_bAutoShowRadar = FALSE;
			ShowQuickRadarPage(5000);
		}
	}

	cnt++;
}
void CPanelVehicleInfoWrapper::OnGPSGroundSpeed(INT iKmh)
{
	if(!m_bHasCanBus)
	{
		m_panelDrive.OnGPSGroundSpeed(iKmh);
	}
}
BOOL CPanelVehicleInfoWrapper::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);


	//test:
// 	m_bHasCanBus = TRUE;

	m_panelDrive.Initialize(PAGE_VEHICLE_DRIVE,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelDrive.SetPos(120,0);

	m_panelAC.Initialize(PAGE_VEHICLE_AC,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelAC.SetPos(120,0);

	m_panelBody.Initialize(PAGE_VEHICLE_BODY,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelBody.SetPos(120,0);

	m_panelRadar.Initialize(PAGE_VEHICLE_RADAR,this,pSkinManager,pTextGL,hWnd,entry);
	m_panelRadar.SetPos(120,0);


	m_hACKEvent = CreateEvent(NULL,FALSE,FALSE,_T("CAN_BUS_ACK_EVENT"));
	m_hRadarInfoEvent = CreateEvent(NULL,FALSE,FALSE, VEHICLE_INFO_EVENT);
	if(m_hRadarInfoEvent == NULL)
	{
		RETAILMSG(1, (TEXT("APP:ERR:%s: CreateEvent [%s] failed.\r\n"),VEHICLE_INFO_EVENT));
	}
// 	m_hCmdBuffReadyEvent = CreateEvent(NULL,FALSE,FALSE,NULL);


	//controls:
	COLORGL cr_gray={164.0f/256.0f,164.0f/256.0f,164.0f/256.0f,1.0};
	COLORGL cr_red={245.0f/256.0f,59.0f/256.0f,16.0f/256.0f,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int cx=240;
	int cy=90;

	SIZE sibtn={cx,cy+2};

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_625));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_626));


	m_btnHome.Initialize(IDC_VEHICLEINFO_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelVehicleInfoWrapper::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,88);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_b);
	m_btnHome.SetCaptionHeight(24);
	m_btnHome.SetIconPos(-cx/2+46,0);


	//

	m_iconSpeed.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_355));
	m_iconSpeedFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_354));

	m_iconBodyInfo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_349));
	m_iconBodyInfoFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_348));
	m_iconDoors.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_353));
	m_iconDoorsFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_352));
	m_iconAC.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_351));
	m_iconACFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_350));


	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));
	box.siClient.cx=cx;
	box.siClient.cy=g_iClientHeight-cy;
	box.siBorder.cx=cx;
	box.siBorder.cy=g_iClientHeight-cy;
	box.iSpacing=(g_iClientHeight-cy)/6;

	box.idTexIndexBase=TEXID_603;
	box.idTexIndexFocus=TEXID_068;//TEXID_604;
	box.idTexBase=TEXID_603;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;//FALSE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxMenu.Initialize(IDC_VEHICLEINFO_BOX_MENU,this,&box,pSkinManager);
	m_boxMenu.SetNotifyFunc(&CPanelVehicleInfoWrapper::OnNotify,this);
	m_boxMenu.SetPos(-g_iClientWidth/2+cx/2,cy/2);
	m_boxMenu.LinkTextGL(pTextGL);
	m_boxMenu.SetAlpha(1.0f);
	m_boxMenu.SetCaptioniHeight(24);
	m_boxMenu.SetCaptionColor(&cr_gray,&cr_b);

	CListBoxItemGL *pItem;
	pItem=new CListBoxItemGL(GetResourceString(IDS_VEHICLE_DRIVE));
	pItem->SetIcon(&m_iconSpeed,&m_iconSpeedFocus);
	m_boxMenu.AddItem(pItem,PAGE_VEHICLE_DRIVE);
	pItem=new CListBoxItemGL(GetResourceString(IDS_VEHICLE_AC));
	pItem->SetIcon(&m_iconAC,&m_iconACFocus);
	m_boxMenu.AddItem(pItem,PAGE_VEHICLE_AC);
	pItem=new CListBoxItemGL(GetResourceString(IDS_VEHICLE_BODY));
	pItem->SetIcon(&m_iconDoors,&m_iconDoorsFocus);
	m_boxMenu.AddItem(pItem,PAGE_VEHICLE_BODY);
	pItem=new CListBoxItemGL(GetResourceString(IDS_VEHICLE_RADAR));
	pItem->SetIcon(&m_iconBodyInfo,&m_iconBodyInfoFocus);
	m_boxMenu.AddItem(pItem,PAGE_VEHICLE_RADAR);

	//blanks:
	pItem=new CListBoxItemGL(_T(" "));
	pItem->EnableClick(FALSE);
	m_boxMenu.AddItem(pItem);
	pItem=new CListBoxItemGL(_T(" "));
	pItem->EnableClick(FALSE);
	m_boxMenu.AddItem(pItem);
// 	pItem=new CListBoxItemGL(_T(" "));
// 	pItem->EnableClick(FALSE);
// 	m_boxMenu.AddItem(pItem);

	return TRUE;
}

void CPanelVehicleInfoWrapper::SendCanbusACK(BYTE ack)
{
// 	RETAILMSG(DEBUG_VEHICLEINFO,(_T(">>>>CAN SEND: %x\r\n"),ack));

	BYTE buff[512]={0xF8,0x7D,0x01,  ack,    0x01+ack};

	if(m_pCommander)
	{
		m_pCommander->WriteSerialPort(buff,5);
	}
}
void CPanelVehicleInfoWrapper::OnInfoCommon(BYTE iInfo)
{
	RETAILMSG(1/*DEBUG_VEHICLEINFO*/,(_T("--OnInfoCommon: 0x%x\r\n"),iInfo));

	BOOL bParking = (iInfo&0x02);
	BOOL bReverse = (iInfo&0x01);
	#if CVTE_EN_CARPLAY
	if (iInfo&0x01)
	{
		CCarplayAdapter::GetInstance()->OnTransmissionState(kNMEATransmissionState_Reverse);
	}
	else if (iInfo&0x02)
	{
		CCarplayAdapter::GetInstance()->OnTransmissionState(kNMEATransmissionState_Park);
	}
	else
	{
		CCarplayAdapter::GetInstance()->OnTransmissionState(kNMEATransmissionState_Drive);
	}
	#endif
}
void CPanelVehicleInfoWrapper::OnInfoSpeed(UINT iSpeedKmh)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoSpeed: %d\r\n"),iSpeedKmh));
	//CCarplayAdapter::GetInstance()->OnSpeedSample(((double)iSpeedKmh)/3.6);
	//CCarplayAdapter::GetInstance()->OnSpeedSample((double)iSpeedKmh);
}
void CPanelVehicleInfoWrapper::OnInfoKey(INT idKey,INT idStatus)
{
	::SendMessage(m_hMainWnd,WM_VCHICLE_KEY,idKey,idStatus);
// 	::PostMessage(m_hMainWnd,WM_VCHICLE_KEY,idKey,idStatus);

}

void CPanelVehicleInfoWrapper::OnInfoBelt(BOOL bNormal)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoBelt:%d\r\n"),bNormal));
		m_panelDrive.SetInfoBelt(bNormal);
}
void CPanelVehicleInfoWrapper::OnInfoWashWater(BOOL bNormal)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoWashWater:%d\r\n"),bNormal));
		m_panelDrive.SetInfoWashWater(bNormal);
}
void CPanelVehicleInfoWrapper::OnInfoHandBrake(BOOL bNormal)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoHandBrake:%d\r\n"),bNormal));
		m_panelDrive.SetInfoHandBrake(bNormal);

}
void CPanelVehicleInfoWrapper::OnInfoTrunk(BOOL bNormal)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoTrunk:%d\r\n"),bNormal));
		m_panelBody.SetInfoTrunk(bNormal);
}
void CPanelVehicleInfoWrapper::OnInfoDoorFL(BOOL bNormal)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoDoorFL:%d\r\n"),bNormal));
		m_panelBody.SetInfoDoorFL(bNormal);
}
void CPanelVehicleInfoWrapper::OnInfoDoorFR(BOOL bNormal)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoDoorFR:%d\r\n"),bNormal));
		m_panelBody.SetInfoDoorFR(bNormal);
}
void CPanelVehicleInfoWrapper::OnInfoDoorRL(BOOL bNormal)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoDoorRL:%d\r\n"),bNormal));
		m_panelBody.SetInfoDoorRL(bNormal);
}
void CPanelVehicleInfoWrapper::OnInfoDoorRR(BOOL bNormal)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoDoorRR:%d\r\n"),bNormal));
		m_panelBody.SetInfoDoorRR(bNormal);

}

void CPanelVehicleInfoWrapper::OnInfoEnginSpeed(INT iRPM)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoEnginSpeed:%d\r\n"),iRPM));
		m_panelDrive.SetInfoEnginSpeed(iRPM);
}
void CPanelVehicleInfoWrapper::OnInfoInstantVel(INT iKmh)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoInstantVel:%d\r\n"),iKmh));
		m_panelDrive.SetInfoInstantVel(iKmh);
	#if CVTE_EN_CARPLAY
	CCarplayAdapter::GetInstance()->OnSpeedSample((double)iKmh/360.0);
	#endif
}
void CPanelVehicleInfoWrapper::OnInfoBattery(INT iV)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoBattery:%d\r\n"),iV));
		m_panelDrive.SetInfoBattery(iV);
}
void CPanelVehicleInfoWrapper::OnInfoOutsideTemp(INT iC)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoOutsideTemp:%d\r\n"),iC));
		m_panelDrive.SetInfoOutsideTemp(iC);
		m_panelAC.SetInfoOutsideTemp(iC);

}
void CPanelVehicleInfoWrapper::OnInfoTrip(INT iKm)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoTrip:%d\r\n"),iKm));
		m_panelDrive.SetInfoTrip(iKm);

}
void CPanelVehicleInfoWrapper::OnInfoOil(INT iL)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoOil:%d\r\n"),iL));
		m_panelDrive.SetInfoOil(iL);

}

void CPanelVehicleInfoWrapper::OnInfoAlertOil(BOOL bAlert)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoAlertOil:%d\r\n"),bAlert));
		m_panelDrive.SetInfoAlertOil(bAlert);

}
void CPanelVehicleInfoWrapper::OnInfoAlertBettery(BOOL bAlert)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoAlertBettery:%d\r\n"),bAlert));
		m_panelDrive.SetInfoAlertBettery(bAlert);

}
void CPanelVehicleInfoWrapper::RequestVchicleBodyInfo(BYTE idType)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--RequestVchicleBodyInfo:%d\r\n"),idType));
	if(m_bHasCanBus)
	{
		BYTE buff[8]={CAN_HOST_TYPE_QUERY,0x02,0x41,idType};
		SendCanbusCommand(buff,4);
	}
}
void CPanelVehicleInfoWrapper::RequestSpeedInfo()
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--RequestSpeedInfo--\r\n")));
	if(m_bHasCanBus)
	{
		BYTE buff[8]={CAN_HOST_TYPE_QUERY,0x01,0x16};
		SendCanbusCommand(buff,3);
	}
}
void CPanelVehicleInfoWrapper::RequestACInfo()
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--RequestACInfo--\r\n")));
	if(m_bHasCanBus)
	{
		BYTE buff[8]={CAN_HOST_TYPE_QUERY,0x01,0x21};
		SendCanbusCommand(buff,3);
	}
}
void CPanelVehicleInfoWrapper::OnInfoRadar(INT id,INT iSignal)
{
	m_panelRadar.SetInfoRadar(id,iSignal);
	::PostMessage(m_hMainWnd,WM_VEHICLE_RADAR_MSG,id,iSignal);
}
void CPanelVehicleInfoWrapper::OnInfoAirStatus(BYTE iStatus)
{
		m_panelAC.SetInfoAirStatus(iStatus);
}
void CPanelVehicleInfoWrapper::ShowPage(UINT idPage)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("############ShowPage: %d\r\n"),idPage));

	m_idCurSubpage=(PAGE_VEHICLE)idPage;
	SetPanelParam(m_idCurSubpage);
	m_bQuickMode=FALSE;
	m_idQuickShowPage = -1;
	//m_idLastPage = -1;

	m_boxMenu.MoveToIndex(m_boxMenu.SearchID(m_idCurSubpage));

	if(m_idCurSubpage == PAGE_VEHICLE_DRIVE)
	{
		RequestVchicleBodyInfo(0x2);
	}
	else if(m_idCurSubpage == PAGE_VEHICLE_AC)
	{
		RequestACInfo();
	}

// 	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
// 	if(panel)
// 	{
// 		panel->OnEnterPanel();
// 	}
}
void CPanelVehicleInfoWrapper::ShowQuickBodyPage(UINT iTimeOut)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("############ShowBodyPage\r\n")));
#if 0
	if(m_idSystemPowerState == SYS_POWER_ON)
	{
		UINT idClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI);

		if(idClass==UI_CLASS_VEHICLEINFO && !m_bQuickMode)
		{
			UINT temp=(m_idQuickShowPage==-1)?m_idCurSubpage:m_idLastPage;
			ShowPage(PAGE_VEHICLE_BODY);
			m_idLastPage=temp;
			m_dwShowBodyTimeout = iTimeOut;
			m_dwLastShowBodyTick = GetTickCount();
			m_idQuickShowPage = PAGE_VEHICLE_BODY;
		}
		else
		{
			if(	SwitchPage(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER,PAGE_SWITCH_ANI_NONE,(1<<16)+PAGE_VEHICLE_BODY))
			{
				m_dwShowBodyTimeout = iTimeOut;
				m_dwLastShowBodyTick = GetTickCount();
				m_idQuickShowPage = PAGE_VEHICLE_BODY;
			}
		}
	}
#endif
}
void CPanelVehicleInfoWrapper::ShowQuickACPage(UINT iTimeOut)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("############ShowAirPage\r\n")));
#if 0
	if(m_idSystemPowerState == SYS_POWER_ON)
	{
		UINT idClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI);

		if(idClass==UI_CLASS_VEHICLEINFO && !m_bQuickMode)
		{
			UINT temp=(m_idQuickShowPage==-1)?m_idCurSubpage:m_idLastPage;
			ShowPage(PAGE_VEHICLE_AC);
			m_idLastPage=temp;
			m_dwShowACTimeout = iTimeOut;
			m_dwLastShowACTick = GetTickCount();
			m_idQuickShowPage = PAGE_VEHICLE_AC;
		}
		else
		{
			if(SwitchPage(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER,PAGE_SWITCH_ANI_NONE,(1<<16)+PAGE_VEHICLE_AC))
			{
				m_dwShowACTimeout = iTimeOut;
				m_dwLastShowACTick = GetTickCount();
				m_idQuickShowPage = PAGE_VEHICLE_AC;
			}
		}
	}
#endif
}
void CPanelVehicleInfoWrapper::ShowQuickRadarPage(UINT iTimeOut)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("############ShowRadarPage\r\n")));
#if 0
	if(m_idSystemPowerState == SYS_POWER_ON)
	{
		UINT idClass=GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI);

		if(idClass==UI_CLASS_VEHICLEINFO && !m_bQuickMode)
		{
			UINT temp=(m_idQuickShowPage==-1)?m_idCurSubpage:m_idLastPage;
			ShowPage(PAGE_VEHICLE_RADAR);
			m_idLastPage=temp;
			m_dwShowRadarTimeout = iTimeOut;
			m_dwLastShowRadarTick = GetTickCount();
			m_idQuickShowPage = PAGE_VEHICLE_RADAR;
		}
		else
		{
			if(	SwitchPage(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER,PAGE_SWITCH_ANI_NONE,(1<<16)+PAGE_VEHICLE_RADAR))
			{
				m_dwShowRadarTimeout = iTimeOut;
				m_dwLastShowRadarTick = GetTickCount();
				m_idQuickShowPage = PAGE_VEHICLE_RADAR;
			}
		}
	}
#endif
}
void CPanelVehicleInfoWrapper::OnInfoAirWind(BYTE iStatus)
{
		m_panelAC.SetInfoAirWind(iStatus);

	//
	BOOL bShow=((iStatus&0x10)!=0);
	//signal UI:
	if(bShow)
	{
		ShowQuickACPage();
	}
}
void CPanelVehicleInfoWrapper::OnInfoAirTempDriver(BYTE iStatus)
{
		m_panelAC.SetInfoAirTempDriver(iStatus);

}
void CPanelVehicleInfoWrapper::OnInfoAirTempCoDriver(BYTE iStatus)
{
		m_panelAC.SetInfoAirTempCoDriver(iStatus);

}
void CPanelVehicleInfoWrapper::OnInfoAirSeatHeating(BYTE iStatus)
{
		m_panelAC.SetInfoAirSeatHeating(iStatus);

}
void CPanelVehicleInfoWrapper::OnInfoSteerWheel(INT iAngle)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("--OnInfoSteerWheel:%d\r\n"),iAngle));

}
void CPanelVehicleInfoWrapper::OnInfoParkingAssi(BYTE iStatus)
{
	BOOL bRadarOn=((iStatus&0x01)!=0);
	BOOL bAssiOn=((iStatus&0x02)!=0);

	RETAILMSG(1/*DEBUG_VEHICLEINFO*/,(_T("----OnInfoParkingAssi: bRadarOn :%d, bAssiOn:%d\r\n"),bRadarOn,bAssiOn));

	m_bRadarOn =  bRadarOn;
	m_bAssiOn = bAssiOn;
	m_dwAssiNotifyTick = GetTickCount();
	m_bAutoShowRadar = (m_bRadarOn || m_bAssiOn);

	//signal UI:
// 	if(bAssiOn && !m_bOnReverseMode)
// 	{
// 		ShowRadarPage();
// 	}
}
void CPanelVehicleInfoWrapper::ProcessCanbusLine(LPBYTE pCmd,DWORD dwCmdLen)
{
	//
	if(!pCmd || dwCmdLen==0)
		return;

	BYTE DataType = pCmd[0];
	int len=pCmd[1];

//		RETAILMSG(1, (TEXT("APP:MSG:%s: DataType[0x%x].\r\n"), TEXT(__FUNCTION__), DataType));

	switch (DataType)
	{
	case CAN_SLAVE_TYPE_SPEED:
		OnInfoSpeed(((pCmd[3]<<8)+(pCmd[2]))/16);
		break;
	case CAN_SLAVE_TYPE_KEY:
		OnInfoKey(pCmd[2],pCmd[3]);
		break;

	case CAN_SLAVE_TYPE_VCHICLE_BODY:
		{
			int cmd=pCmd[2];
			if(cmd==0x01 && len==2)
			{
				OnInfoBelt( !(pCmd[3]&0x80) );
				OnInfoWashWater( !(pCmd[3]&0x40) );

// 				OnInfoHandBrake( !(pCmd[3]&0x20) );
				OnInfoHandBrake( (pCmd[3]&0x20) );

				OnInfoTrunk( !(pCmd[3]&0x10) );

				OnInfoDoorRR( !(pCmd[3]&0x08) );
				OnInfoDoorRL( !(pCmd[3]&0x04) );
				OnInfoDoorFR( !(pCmd[3]&0x02) );
				OnInfoDoorFL( !(pCmd[3]&0x01) );

				BYTE door=(pCmd[3]&0x1F);
				RETAILMSG(1, (TEXT("door[0x%x].\r\n"), door));
				if(m_doorStatus != door /*&& door*/)
				{
					ShowQuickBodyPage();
				}
				m_doorStatus = door;
			}
			else if(cmd==0x02 && len==13)
			{
				OnInfoEnginSpeed(pCmd[3]*256+pCmd[4]);
				OnInfoInstantVel(pCmd[5]*256+pCmd[6]);
				OnInfoBattery(pCmd[7]*256+pCmd[8]);
				OnInfoOutsideTemp(INT16(pCmd[9]*256+pCmd[10]));
				OnInfoTrip(pCmd[11]*65536+pCmd[12]*256+pCmd[13]);
				OnInfoOil(pCmd[14]);
			}
			else if(cmd==0x03 && len==2)
			{
				OnInfoAlertOil( (pCmd[3]&0x80)!=0 );
				OnInfoAlertBettery( (pCmd[3]&0x40)!=0 );
			}
		}

		break;
	case CAN_SLAVE_TYPE_RADAR_REAR:
		{
			DWORD dwRadarInfo;

			if(len == 4)
			{
				if(m_hRadarInfoEvent)
				{
					dwRadarInfo = pCmd[2] + (pCmd[3] << 4) + (pCmd[4] << 8) + (pCmd[5] << 12) + (0 << 31);
					RETAILMSG(DEBUG_VEHICLEINFO, (TEXT("APP:MSG:UI: BackRadarInfo[%x] \r\n"), dwRadarInfo));
					SetEventData(m_hRadarInfoEvent, dwRadarInfo);
					SetEvent(m_hRadarInfoEvent);
				}

				OnInfoRadar(4,pCmd[2]);
				OnInfoRadar(5,pCmd[3]);
				OnInfoRadar(6,pCmd[4]);
				OnInfoRadar(7,pCmd[5]);
			}
		}
		break;
	case CAN_SLAVE_TYPE_RADAR_FRONT:
		{
			DWORD dwRadarInfo;
			if(len == 4)
			{
				if(m_hRadarInfoEvent)
				{
					dwRadarInfo = pCmd[2] + (pCmd[3] << 4) + (pCmd[4] << 8) + (pCmd[5] << 12) + (1 << 31);
					RETAILMSG(DEBUG_VEHICLEINFO, (TEXT("APP:MSG:UI: FrontRadarInfo[%u] \r\n"), dwRadarInfo));
					SetEventData(m_hRadarInfoEvent, dwRadarInfo);
					SetEvent(m_hRadarInfoEvent);
				}
				OnInfoRadar(0,pCmd[2]);
				OnInfoRadar(1,pCmd[3]);
				OnInfoRadar(2,pCmd[4]);
				OnInfoRadar(3,pCmd[5]);
			}
		}
		break;
	case CAN_SLAVE_TYPE_PARKING_ASSI:
		{
			OnInfoParkingAssi(pCmd[2]);
		}
		break;
	case CAN_SLAVE_TYPE_AIR:
		{
			if(len == 5)
			{
				OnInfoAirStatus(pCmd[2]);
				OnInfoAirWind(pCmd[3]);
				OnInfoAirTempDriver(pCmd[4]);
				OnInfoAirTempCoDriver(pCmd[5]);
				OnInfoAirSeatHeating(pCmd[6]);
			}
		}
		break;
	case CAN_SLAVE_TYPE_COMMON:
		{
			OnInfoCommon(pCmd[2]);
		}
		break;
	case CAN_SLAVE_TYPE_STEER:
		{
			//OnInfoSteerWheel(short(( ((WORD)pCmd[3])<<8 )+ (WORD)pCmd[2] ) );

#if 0	//Auto Test
			DWORD dwRadarInfo;
			if(m_uRadarData <= 0 || m_uRadarData > 0xa)
			{
				m_uRadarData = 0xa;
			}
			if(GetTickCount() - m_dwLastTick >= 50)
			{
				//RETAILMSG(1, (TEXT("APP:MSG:%s m_uRadarData[%d]\r\n"), TEXT(__FUNCTION__), m_uRadarData));

				if(m_hRadarInfoEvent)
				{
					dwRadarInfo = m_uRadarData + (m_uRadarData << 4) + (m_uRadarData << 8) + (m_uRadarData << 12) + (0 << 31);
//						RETAILMSG(1, (TEXT("APP:MSG:UI: FrontRadarInfo[%u] \r\n"), dwRadarInfo));
					SetEventData(m_hRadarInfoEvent, dwRadarInfo);
					SetEvent(m_hRadarInfoEvent);
				}

				if(m_hRadarInfoEvent)
				{
					dwRadarInfo = m_uRadarData + (m_uRadarData << 4) + (m_uRadarData << 8) + (m_uRadarData << 12) + (1 << 31);
//						RETAILMSG(1, (TEXT("APP:MSG:UI: BackRadarInfo[%u] \r\n"), dwRadarInfo));
					SetEventData(m_hRadarInfoEvent, dwRadarInfo);
					SetEvent(m_hRadarInfoEvent);
				}

				m_uRadarData--;
				m_dwLastTick = GetTickCount();
			}
#endif
		}
		break;
	}

}
void CPanelVehicleInfoWrapper::Connect(BOOL bConnect)
{
	RETAILMSG(1/*DEBUG_VEHICLEINFO*/,(_T("----CPanelVehicleInfoWrapper:: begin to Connect:%d\r\n"),bConnect));

	//connect:
// 	BYTE buff[8]={CAN_HOST_TYPE_START_END,0x01,(bConnect?0x1:0x0)};
// 	SendCanbusCommand(buff,3);

	//
	BYTE buff[8]={CAN_HOST_TYPE_QUERY,0x02,0x41,0x02};
	SendCanbusCommand(buff,4);

}

static BYTE GetHex2(char *string)
{
	BYTE hi=0;
	BYTE lo=0;

	if(string[0]>='A' && string[0]<='F')
		hi = 10 + string[0] - 'A';
	else if(string[0]>='0' && string[0]<='9')
		hi = string[0] - '0';

	if(string[1]>='A' && string[1]<='F')
		lo = 10 + string[1] - 'A';
	else if(string[1]>='0' && string[1]<='9')
		lo = string[1] - '0';

	return (hi<<4) + lo;
}

void CPanelVehicleInfoWrapper::ProcessTpmsPack(LPBYTE pCmd,DWORD dwCmdLen)
{
	RETAILMSG(1,(_T("<<%S\r\n"),(char*)pCmd));

// 	wchar_t strPrint[512];
// 	wchar_t element[8];
// 	swprintf_s(strPrint,512,_T(">>>TPMS[%d] "),dwCmdLen);
// 	for (int i=0;i<dwCmdLen;i++)
// 	{
// 		swprintf_s(element,8,_T("%x "),*(pCmd+i));
// 		wcscat_s(strPrint,512,element);
// 	}
// 	wcscat_s(strPrint,512,_T("\r\n"));
// 	RETAILMSG(1,(strPrint));

	BYTE *pHead=pCmd;
	BYTE *pTail=pCmd;
	INT iCmdLen=dwCmdLen;

	while((pTail-pCmd)<(iCmdLen-2))
	{
		if(*pHead != '$')
		{
			pHead++;
			pTail = pHead;
		}
		else
		{
			if(*pTail != '*')
			{
				pTail ++;
			}
		}

		if(*pHead == '$' && *pTail == '*')
		{
			int len=pTail-pHead+1;
			BYTE checksum=pHead[1];
			for (int i=0;i<len-3;i++)
			{
				checksum ^= pHead[i+2];
			}

			if(GetHex2((char*)(pTail)+1) == checksum)
			{
				ProcessTpmsLine(pHead,len);//check sum
			}

			//move to next:
			pTail += 3;
			pHead = pTail;
		}
	}
}
void CPanelVehicleInfoWrapper::ProcessCanbusPack(LPBYTE pCmd,DWORD dwCmdLen)
{
#if 0
	static WCHAR strPrint[4096];
	//test://///////////////////////////////////////////////////////////////////////
	WCHAR element[8];
	int pos=0;
	BOOL bflag=FALSE;
	while(pos<(INT)dwCmdLen)
	{
		swprintf_s(strPrint,4096,bflag?_T("----"):_T("<<<<CAN REV:(%d,%d)"),dwCmdLen,GetTickCount());
		int left_len=min(128,dwCmdLen-pos);
		for (int i=0;i<left_len;i++)
		{
			swprintf_s(element,8,_T("%x "),*(pCmd+pos+i));
			wcscat_s(strPrint,4096,element);
		}
		wcscat_s(strPrint,4096,_T("\r\n"));
		RETAILMSG(DEBUG_COMMANDER_EX,(strPrint));
		pos+=left_len;
		bflag++;
	}
#endif
	//////////////////////////////////////////////////////////////////////////////////

	//
	if(!pCmd || dwCmdLen==0)
		return;

//		RETAILMSG(1,(_T("ProcessCommandPack: len:%d\r\n"),dwCmdLen));

	BYTE *pCursor=pCmd;
	INT iCmdLen=dwCmdLen;

	while((pCursor-pCmd)<iCmdLen)
	{
		if(*pCursor == 0x2E)
		{
			if(!m_bHasCanBus)
			{
				m_bHasCanBus = TRUE;
				RETAILMSG(1/*DEBUG_VEHICLEINFO*/,(_T("### CPanelVehicleInfoWrapper:: Connect: SUCCEED!!!  ###\r\n")));
				::PostMessage(m_hMainWnd,WM_VEHICLE_CANBUS_MSG,1,NULL);

				//test:
				SetTimer(m_hMainWnd,TIMER_VEHICLE_INFO_PROC,100,NULL);
			}

			int len=*(pCursor+2);
			BYTE checksum=0;
			for (int i=0;i<len+2;i++)
			{
				checksum+=*(pCursor+1+i);
			}

			checksum=~checksum;
// 			RETAILMSG(DEBUG_VEHICLEINFO,(_T("ProcessCommandPack: checksum:%x,tick:%d\r\n"),checksum,GetTickCount()));

			if(checksum == *(pCursor+len+3))
			{
				SendCanbusACK(CAN_ACK);
				ProcessCanbusLine(pCursor+1,len+2);
			}
			else
			{
				SendCanbusACK(CAN_NACK_CHECK_NG);
			}

			pCursor+=len+4;
		}
		else if(*pCursor == 0xFF)
		{
			//notify ACK event
			SetEventData(m_hACKEvent,0xFF);
			SetEvent(m_hACKEvent);

			//
			if(!m_bHasCanBus)
			{
				m_bHasCanBus = TRUE;
				RETAILMSG(1/*DEBUG_VEHICLEINFO*/,(_T("### CPanelVehicleInfoWrapper:: Connect: SUCCEED!!!  ###\r\n")));
				::PostMessage(m_hMainWnd,WM_VEHICLE_CANBUS_MSG,1,NULL);

				//test:
				SetTimer(m_hMainWnd,TIMER_VEHICLE_INFO_PROC,100,NULL);

				///////////////////////////////////////
				/*
				//send ini data to canbus:
				if(g_wParamVolumeInfo!=0 || g_lParamVolumeInfo!=0)
				{
					RETAILMSG(DEBUG_VEHICLEINFO,(_T("----------CPanelVehicleInfoWrapper:: set ini data: VolumeInfo\r\n")));
					NotifyVolumeInfo(g_wParamVolumeInfo,g_lParamVolumeInfo);
					Sleep(200);
				}

				if(g_wParamMediaPlaybackInfo!=0 || g_lParamMediaPlaybackInfo!=0)
				{
					RETAILMSG(DEBUG_VEHICLEINFO,(_T("----------CPanelVehicleInfoWrapper:: set ini data: MediaPlaybackInfo\r\n")));
					NotifyMediaPlaybackInfo(g_wParamMediaPlaybackInfo,g_lParamMediaPlaybackInfo);
					Sleep(200);
				}

				if(g_wParamRadioInfo!=0 || g_lParamRadioInfo!=0)
				{
					RETAILMSG(DEBUG_VEHICLEINFO,(_T("----------CPanelVehicleInfoWrapper:: set ini data: RadioInfo\r\n")));
					if(m_pCommander)
					{
						BYTE preset=m_pCommander->GetItemData(MCU_CUR_CHANNEL_STORE);
						if(preset>5)
							preset=0;
						else
							preset+=1;

						BYTE band=m_pCommander->GetItemData(MCU_CUR_BAND_STORE);
						WORD freq;

						if(band<=3)//FM
						{
							if(band==3)
								band=0;
							else
								band+=1;
							freq=m_pCommander->GetItemData(MCU_CUR_FREQU_STORE_H)*100+m_pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
						}
						else
						{
							band+=13;
							freq=m_pCommander->GetItemData(MCU_CUR_FREQU_STORE_H)*100+m_pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
						}
						NotifyRadioInfo(band,freq,preset,g_lParamRadioInfo);
						Sleep(200);
					}
				}
				if(g_wParamSourceInfo!=0 || g_lParamSourceInfo!=0)
				{
					RETAILMSG(DEBUG_VEHICLEINFO,(_T("----------CPanelVehicleInfoWrapper:: set ini data: SourceInfo\r\n")));
					NotifySourceInfo(g_wParamSourceInfo,g_lParamSourceInfo);
					Sleep(200);
				}
				*/
				///////////////////////////////////////

			}

			pCursor++;
		}
		else
		{
			pCursor++;
		}

		Sleep(0);
	}
}
void CPanelVehicleInfoWrapper::SendCanbusCommand(BYTE *pBuff,INT  iLen,INT iSendDelay)
{
	if(!m_pCmdSender)
	{
		m_pCmdSender = new CmdSender_CanBus();
		if(m_pCmdSender)
		{
			m_pCmdSender->LinkCommander(m_pCommander);
		}
	}

	if(m_pCmdSender)
	{
		m_pCmdSender->SkipCmd(pBuff[0]);
		m_pCmdSender->QueueCmdBuff(pBuff[0],pBuff,iLen,iSendDelay);
	}
}

void CALLBACK CPanelVehicleInfoWrapper::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("CPanelVehicleInfoWrapper OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelVehicleInfoWrapper *panel=(CPanelVehicleInfoWrapper*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelVehicleInfoWrapper::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_VEHICLEINFO_BTN_HOME:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;
	case IDC_VEHICLEINFO_BOX_MENU:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
					ShowPage((PAGE_VEHICLE)id_item);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDM_TPMS_CHANGE_TIRE:
		{
			OnTpmsSwitchTire(wMsg,wPara);
		}
		break;
	}
}

void CPanelVehicleInfoWrapper::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchDown(pt);
	}

	//test TPMS
// 	char temp[]="$C,PING,LF,Kpa,230,C,30,OK*5A 88ff $C,PING,LR,Kpa,230,C,30,LOW*1E oopp $C,PING,RF,Kpa,230,C,-4,OK*5E $C,PING,RR,Kpa,230,C,-5,OK*4B $C,ALARM,RF,LP*34";
// 	ProcessTpmsPack((LPBYTE)temp,strlen(temp));
// 	char buf[]="$C,IDFO,RF,0AAAAAAA*";
// 	SendTpmsCommand((BYTE*)buf,strlen(buf));
}
void CPanelVehicleInfoWrapper::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchUp(pt);
	}
}
void CPanelVehicleInfoWrapper::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);

	CPanelGL *panel=GetChildPanel(m_idCurSubpage);
	if(panel)
	{
		panel->OnTouchMove(pt);
	}
}

void CPanelVehicleInfoWrapper::OnGesture(GESTUREINFO *pgi)
{

}

void CPanelVehicleInfoWrapper::OnGestureMsg(UINT idGesture)
{

}

static UINT GetTireID(BYTE *name)
{
	if(name[0]=='L' && name[1]=='F')
		return 0;
	else if(name[0]=='R' && name[1]=='F')
		return 1;
	else if(name[0]=='L' && name[1]=='R')
		return 2;
	else if(name[0]=='R' && name[1]=='R')
		return 3;
	else if(name[0]=='S' && name[1]=='P')
		return 4;
	else if(name[0]=='A' && name[1]=='L')
		return 5;
	else
		return -1;
}
static UINT GetStringValue(BYTE *string,int len)
{
	for (int i=0;i<len;i++)
	{
		if(string[i] == ',')
			return i;
	}

	return 0;
}
void CPanelVehicleInfoWrapper::ProcessTpmsLine(LPBYTE pCmd,DWORD dwCmdLen)
{
	//RETAILMSG(1,(_T("ProcessTpmsLine: len:%d, cmd:%S\r\n"),dwCmdLen,(char*)pCmd));

	static UINT ping_cnt=0;
	static UINT onle_cnt=0;
	BYTE* pStart = pCmd;
	int val_len=0;

	if(pCmd[3]=='O' && pCmd[4]=='N' && pCmd[5]=='L' && pCmd[6]=='E')
	{
		onle_cnt++;
		if(!m_bGotTpmsSensorID && onle_cnt%5==0)
		{
			QueryTpmsSensorID();
		}
	}
	else if(pCmd[3]=='T' && pCmd[4]=='R' && pCmd[5]=='R' && pCmd[6]=='T')
	{
		QueryTpmsSensorID();
	}
	else if(pCmd[3]=='P' && pCmd[4]=='I' && pCmd[5]=='N' && pCmd[6]=='G')
	{
		pStart = pCmd + 8;

		ping_cnt++;

		//tire id:
		val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
		int tire_id=GetTireID(pStart);
		pStart += val_len+1;

		//pressure unit and value:
		wchar_t strpressure[16];
		char str_pressure[16];
		char str_pressure_unit[16];
		val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
		memcpy(str_pressure_unit,pStart,val_len);
		str_pressure_unit[val_len]=0;
		pStart += val_len+1;
		val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
		memcpy(str_pressure,pStart,val_len);
		str_pressure[val_len]=0;
		pStart += val_len+1;
		swprintf_s(strpressure,16,_T("%S%S"),val_len?str_pressure:("--"),str_pressure_unit);
		OnInfoTpmsPressure(tire_id,strpressure);

		//temp unit and value:
		wchar_t strtemper[16];
		char str_temper[16];
		char str_temper_unit[16];
		val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
		memcpy(str_temper_unit,pStart,val_len);
		str_temper_unit[val_len]=0;
		pStart += val_len+1;
		val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
		memcpy(str_temper,pStart,val_len);
		str_temper[val_len]=0;
		pStart += val_len+1;
		swprintf_s(strtemper,16,_T("%S%S"),val_len?str_temper:("--"),str_temper_unit);
		OnInfoTpmsTemper(tire_id,strtemper);

		//RETAILMSG(1,(_T("##[PING][%d]: tire_id=%d, pressure=%s, temper=%s \r\n"),ping_cnt,tire_id,strpressure,strtemper));
	}
	else if(pCmd[3]=='I' && pCmd[4]=='D' && pCmd[5]=='F' && pCmd[6]=='O' )
	{
		pStart = pCmd + 8;

		//tire id:
		val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
		int tire_id=GetTireID(pStart);
		pStart += val_len+1;

		RETAILMSG(1,(_T("##[IDFO]: tire_id=%d\r\n"),tire_id));

		if(tire_id == 5)
		{
			m_bGotTpmsSensorID = TRUE;
		
			//ids:
			for (int i=0;i<5;i++)
			{
				val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
				int idx=0;
				if(i==0)
					idx=1;
				else if(i==1)
					idx=3;	
				else if(i==2)
					idx=2;
				else if(i==3)
					idx=0;
				else
					idx=4;

				if(val_len)
				{
					memcpy(m_strTpmsSensorId[idx],pStart,val_len);
					m_strTpmsSensorId[idx][val_len]=0;
					pStart += val_len+1;
					RETAILMSG(1,(_T("##[IDFO]: %d, id=%S\r\n"),idx,m_strTpmsSensorId[idx]));
				}
				else
				{
					m_strTpmsSensorId[idx][0]=0;
				}
			}
		}
	}
	else if(pCmd[3]=='A' && pCmd[4]=='L' && pCmd[5]=='A' && pCmd[6]=='R' && pCmd[7]=='M')
	{
		pStart = pCmd + 9;

		//tire id:
		val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
		int tire_id=GetTireID(pStart);
		pStart += val_len+1;

		//alarm type:
		val_len=GetStringValue(pStart,dwCmdLen-(pStart-pCmd));
		UINT idType=(pStart[0]<<8)+pStart[1];
		OnInfoTpmsAlarm(tire_id,idType);
		pStart += val_len+1;

		#if 0
		m_dwTpmsAlarmTick = GetTickCount();
		if(GetTickCount() - m_dwShowTpmsAlarmTick > 60000 * 10)//show per 10 min
		{
			m_dwShowTpmsAlarmTick = GetTickCount();

			if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI)!=UI_CLASS_VEHICLEINFO)
			{
				ShowQuickBodyPage(10000);
			}
			else if(m_idCurSubpage!=PAGE_VEHICLE_BODY)
			{
				ShowPage(PAGE_VEHICLE_BODY);
			}
		}
		#endif
		//RETAILMSG(1,(_T("##[ALARM] tire_id=%d, type=%d \r\n"),tire_id,idType));
	}
}
void CPanelVehicleInfoWrapper::SendTpmsCommand(BYTE *pBuff,INT  iLen,INT iSendDelay)
{
	BYTE buf[128]={0xF8,0x5C,iLen+4};
	BYTE checksum_mcu=0;
	BYTE checksum_tpms=pBuff[1];
	char check[8];
	memcpy(buf+3,pBuff,iLen);
	for (int i=2;i<iLen-1;i++)
	{
		checksum_tpms ^= pBuff[i];
	}
	sprintf_s(check,8,"%02X",checksum_tpms);
	buf[3+iLen]=check[0];
	buf[3+iLen+1]=check[1];
	buf[3+iLen+2]=0x0d;
	buf[3+iLen+3]=0x0a;

	for (int i=0;i<iLen+5;i++)
	{
		checksum_mcu += buf[i+2];
	}
	buf[3+iLen+4]=checksum_mcu;

	m_pCommander->WriteSerialPort(buf,iLen+8);

	RETAILMSG(1,(_T(">>%S\r\n"),(char*)(buf+3)));
}
void CPanelVehicleInfoWrapper::OnInfoTpmsPressure(UINT idTire,const wchar_t* pValue)
{
	m_panelBody.SetTpmsPressure(idTire,pValue);
}
void CPanelVehicleInfoWrapper::OnInfoTpmsTemper(UINT idTire,const wchar_t* pValue)
{
	m_panelBody.SetTpmsTemper(idTire,pValue);
}
void CPanelVehicleInfoWrapper::OnInfoTpmsAlarm(UINT idTire,UINT idAlarmType)
{
	m_panelBody.SetTpmsAlarm(idTire,idAlarmType);
}

void CPanelVehicleInfoWrapper::QueryTpmsSensorID(UINT idTire)
{
	if(m_pCommander)
	{
		switch (idTire)
		{
		case -1:
			{
				char buf[]="$C,IDFO,AL*";
				SendTpmsCommand((BYTE*)buf,strlen(buf));
			}
			break;
		}
	}
}
void CPanelVehicleInfoWrapper::OnTpmsSwitchTire(UINT id1,UINT id2)
{
	if(m_bGotTpmsSensorID && id2<5 && id1<5)
	{
		RETAILMSG(1,(_T(">>OnTpmsSwitchTire: id1:%d, id2:%d\r\n"),id1,id2));
		char buf[128];
		char temp[16];
		memcpy(temp,m_strTpmsSensorId[id1],16);
		memcpy(m_strTpmsSensorId[id1],m_strTpmsSensorId[id2],16);
		memcpy(m_strTpmsSensorId[id2],temp,16);

		sprintf_s(buf,128,"$C,TRRT,%s,%s,%s,%s,*",m_strTpmsSensorId[0],m_strTpmsSensorId[1],m_strTpmsSensorId[2],m_strTpmsSensorId[3]);
		SendTpmsCommand((BYTE*)buf,strlen(buf));
	}
}
void CPanelVehicleInfoWrapper::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	switch (idState)
	{
	case SYS_POWER_OFF:
	case SYS_POWER_SOFT_RESET:
		{

		}
		break;
	case SYS_POWER_IDLE:
		{

		}
		break;
	case SYS_POWER_ON:
		{

		}
		break;
	}

}
void CPanelVehicleInfoWrapper::OnReverseModeChange(BOOL bReverse)
{
	RETAILMSG(DEBUG_VEHICLEINFO,(_T("CPanelVehicleInfoWrapper::OnReverseModeChange: %d\r\n"),bReverse));

	CPanelGL::OnReverseModeChange(bReverse);

	if(!m_bHasCanBus)
	{
		#if CVTE_EN_CARPLAY
		//notify carplay here:
		CCarplayAdapter::GetInstance()->OnTransmissionState(bReverse?kNMEATransmissionState_Reverse:kNMEATransmissionState_Drive);
		#endif
	}

}
void CPanelVehicleInfoWrapper::OnSpeechStart()
{
	m_bSpeechOn = TRUE;
}
void CPanelVehicleInfoWrapper::OnSpeechTerminate()
{
	m_bSpeechOn = FALSE;
}
void CPanelVehicleInfoWrapper::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;
}