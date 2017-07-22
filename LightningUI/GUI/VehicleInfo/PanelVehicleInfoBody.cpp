#include "PanelVehicleInfoBody.h"
#include "../../resource.h"



//CPanelVehicleInfoBody
CPanelVehicleInfoBody::CPanelVehicleInfoBody(void)
{
#if ENABLE_TPMS_UI
	for (int i=0;i<5;i++)
	{
		m_idAlarmType[i] = 0 ;
		m_dwAlarmTick[i] = 0;
		m_dwSensorID[i] = 0;
	}
	m_idTireSwitch0 = -1;
	m_idTireSwitch1 = -1;
	m_EnableTireSwitch = FALSE;
#endif
}
CPanelVehicleInfoBody::~CPanelVehicleInfoBody(void)
{

}

BOOL CPanelVehicleInfoBody::IsReady()
{
#if ENABLE_TPMS_UI
	return CPanelGL::IsReady() && m_tagDoorFL.IsReady() && m_tagDoorFR.IsReady() && m_tagDoorRL.IsReady() && m_tagDoorRR.IsReady()
		&& m_btnTire[0].IsReady() && m_btnTire[1].IsReady() && m_btnTire[2].IsReady() && m_btnTire[3].IsReady();
#else
	return CPanelGL::IsReady() && m_tagDoorFL.IsReady() && m_tagDoorFR.IsReady() && m_tagDoorRL.IsReady() && m_tagDoorRR.IsReady();
#endif
}
void CPanelVehicleInfoBody::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		//SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		//SwitchPage(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_AC,PAGE_SWITCH_ANI_LEFT);
		break;
	}
}	
void CPanelVehicleInfoBody::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);

#if ENABLE_TPMS_UI
	m_idTireSwitch0 = -1;
	m_idTireSwitch1 = -1;
	m_EnableTireSwitch = FALSE;
	for (int i=0;i<4;i++)
	{
		if(m_btnTire[i].IsPtnOnObject(pt))
		{
			m_idTireSwitch0 = i;
			break;
		}
	}
#endif
}
void CPanelVehicleInfoBody::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);

#if ENABLE_TPMS_UI
	RETAILMSG(1,(_T("m_idTireSwitch0: %d,   m_idTireSwitch1: %d\r\n"),m_idTireSwitch0,m_idTireSwitch1));

	if(m_idTireSwitch1!=-1 && m_idTireSwitch0!=-1)
	{
		GLfloat px1=m_btnTire[m_idTireSwitch1].GetPosxIni();
		GLfloat py1=m_btnTire[m_idTireSwitch1].GetPosyIni();
		GLfloat px0=m_btnTire[m_idTireSwitch0].GetPosxIni();
		GLfloat py0=m_btnTire[m_idTireSwitch0].GetPosyIni();

		m_btnTire[m_idTireSwitch1].SetPosIni(px0,py0,0,TRUE);
		m_btnTire[m_idTireSwitch0].SetPosIni(px1,py1,0,TRUE);

		//switch tire:
		RETAILMSG(1,(_T("switch tire: m_idTireSwitch0: %d,   m_idTireSwitch1: %d\r\n"),m_idTireSwitch0,m_idTireSwitch1));

		CPanelGL *parent=GetParentPanel();
		if(parent)
		{
			parent->HandleControlMSG(IDM_TPMS_CHANGE_TIRE,m_idTireSwitch0,m_idTireSwitch1,0);
		}
	}
	else if(m_idTireSwitch0!=-1)
	{
		GLfloat px0=m_btnTire[m_idTireSwitch0].GetPosxIni();
		GLfloat py0=m_btnTire[m_idTireSwitch0].GetPosyIni();
		m_btnTire[m_idTireSwitch0].SetPos(px0,py0,0,TRUE);
	}

	m_idTireSwitch0 = -1;
	m_idTireSwitch1 = -1;
	m_EnableTireSwitch = FALSE;
#endif
}
void CPanelVehicleInfoBody::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);

#if ENABLE_TPMS_UI
	if(m_idTireSwitch0 != -1 && GetTickCount()-m_dwLastPressTick>500)
	{
		if(m_btnTire[m_idTireSwitch0].IsPtnOnObject(pt))
		{
			m_EnableTireSwitch = TRUE;
		}
	}

	if(m_EnableTireSwitch)
	{
		GLfloat px0=m_btnTire[m_idTireSwitch0].GetPosxIni();
		GLfloat py0=m_btnTire[m_idTireSwitch0].GetPosyIni();

		m_btnTire[m_idTireSwitch0].SetPos(px0+pt->x-m_ptLastPress.x,py0+m_ptLastPress.y-pt->y,0,FALSE);

		m_idTireSwitch1 = -1;
		for (int i=0;i<4;i++)
		{
			if(m_btnTire[i].IsPtnOnObject(pt) && i!=m_idTireSwitch0)
			{
				m_idTireSwitch1 = i;
				break;
			}
		}	
	}
#endif
}

void CPanelVehicleInfoBody::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

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

	/*
	//test
	static BOOL i=0;
	SetInfoDoorFL(i);
	SetInfoDoorFR(i);
	SetInfoDoorRL(i);
	SetInfoDoorRR(i);
	i=!i;
	*/
}
void CPanelVehicleInfoBody::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}

void CPanelVehicleInfoBody::Render()
{
// 	static CTexture *pTexRadarBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_360):NULL;
	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

	if(!BeginRender())return;

// 	DrawRect(pTexRadarBack,298,15,196-16,340-16,8,&cr,TRUE);

	m_tagCar.SetAlpha(m_fCurAlpha);
	m_tagCar.Render();

	m_tagDoorFL.SetAlpha(m_fCurAlpha);
	m_tagDoorFL.Render();
	m_tagDoorFR.SetAlpha(m_fCurAlpha);
	m_tagDoorFR.Render();
	m_tagDoorRL.SetAlpha(m_fCurAlpha);
	m_tagDoorRL.Render();
	m_tagDoorRR.SetAlpha(m_fCurAlpha);
	m_tagDoorRR.Render();
	m_tagDoorTrunk.SetAlpha(m_fCurAlpha);
	m_tagDoorTrunk.Render();

#if ENABLE_TPMS_UI
	for (int i=0;i<4;i++)
	{
		m_tagTireBack[i].SetAlpha(m_fCurAlpha);
		m_tagTireBack[i].Render();
		m_tagPressure[i].SetAlpha(m_fCurAlpha);
		m_tagPressure[i].Render();
		m_tagTemper[i].SetAlpha(m_fCurAlpha);
		m_tagTemper[i].Render();

		//auto cancel alarm:
		if(m_idAlarmType[i] != 0)
		{
			if(GetTickCount() - m_dwAlarmTick[i] > 10000)
			{
				CancelTpmsAlarm(i);
			}
		}
	}

	for (int i=0;i<4;i++)
	{
		if(i!=m_idTireSwitch0)
		{
			m_btnTire[i].SetAlpha(m_fCurAlpha);
			m_btnTire[i].Render();
		}
	}	
	if(m_idTireSwitch0!=-1 && m_idTireSwitch0<4)
	{
		m_btnTire[m_idTireSwitch0].SetAlpha(m_fCurAlpha);
		m_btnTire[m_idTireSwitch0].Render();
	}
#endif

	EndRender();

}
BOOL CPanelVehicleInfoBody::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	GLfloat texCoord_s01002[]={	
		784.0/1024.0, 0.0,	
		784.0/1024.0, 1.0,	
		1.0, 0.0,	
		1.0, 1.0};

	m_tagCar.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_362),texCoord_s01002);
	m_tagCar.SetPos(0,0);

	VIEW_STATE vs;

	GLfloat texCoord_s01000[]={	
		208.0/512.0, 0.0,	
		208.0/512.0, 1.0,	
		512.0/512.0, 0.0,	
		512.0/512.0, 1.0};

	m_tagDoorFL.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_343),texCoord_s01000);
	vs=m_tagDoorFL.GetCurViewState();
	vs.fRotateZ=-90;
	vs.fRotateY=-180;
	m_tagDoorFL.SetIniViewState(&vs);
	m_tagDoorFL.SetPos(-106,105);
	m_tagDoorFL.Show(TRUE);

	m_tagDoorFR.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_343),texCoord_s01000);
	vs=m_tagDoorFR.GetCurViewState();
	vs.fRotateZ=-90;
	m_tagDoorFR.SetIniViewState(&vs);
	m_tagDoorFR.SetPos(107,105);
	m_tagDoorFR.Show(TRUE);

	GLfloat texCoord_s01001[]={	
		0.0/512.0, 0.0,	
		0.0/512.0, 1.0,	
		320.0/512.0, 0.0,	
		320.0/512.0, 1.0};

	m_tagDoorRL.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_343),texCoord_s01001);
	vs=m_tagDoorRL.GetCurViewState();
	vs.fRotateZ=90;
	m_tagDoorRL.SetIniViewState(&vs);
	m_tagDoorRL.SetPos(-95,-31);
	m_tagDoorRL.Show(TRUE);

	m_tagDoorRR.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_343),texCoord_s01001);
	vs=m_tagDoorRR.GetCurViewState();
	vs.fRotateZ=90;
	vs.fRotateY=180;
	m_tagDoorRR.SetIniViewState(&vs);
	m_tagDoorRR.SetPos(96,-31);
	m_tagDoorRR.Show(TRUE);

	//
	m_tagDoorTrunk.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_372));
	m_tagDoorTrunk.SetPos(0,-192);
	m_tagDoorTrunk.Show(FALSE);

#if ENABLE_TPMS_UI

	//TPMS:
	COLORGL cr_b={1,1,1,1};
// 	COLORGL cr_r={1,0,0,1};
// 	COLORGL cr_g={0,1,0,1};
	SIZE siInfo={220,60};
	for (int i=0;i<4;i++)
	{
		m_tagPressure[i].Initialize(0,this,
			NULL,NULL,
			NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagPressure[i].SetSize(&siInfo);
		m_tagPressure[i].SetCaption(_T("N/A"));
		m_tagPressure[i].LinkTextGL(pTextGL);
		if(i==0)
		{
			m_tagPressure[i].SetPos(-240,180);
			m_tagPressure[i].SetCaptionPos(BTN_CAPTION_LEFT);
		}
		else if(i==1)
		{
			m_tagPressure[i].SetPos(240,180);
			m_tagPressure[i].SetCaptionPos(BTN_CAPTION_RIGHT);
		}
		else if(i==2)
		{
			m_tagPressure[i].SetPos(-240,-150);
			m_tagPressure[i].SetCaptionPos(BTN_CAPTION_LEFT);
		}
		else if(i==3)
		{
			m_tagPressure[i].SetPos(240,-150);
			m_tagPressure[i].SetCaptionPos(BTN_CAPTION_RIGHT);
		}
		m_tagPressure[i].SetCaptionHeight(32);
		//m_tagPressure[i].SetCaptionCharStyle(STYLE_ART);
		m_tagPressure[i].EnableDrawBack(FALSE);
		//m_tagPressure[i].SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagPressure[i].SetCaptionColor(&cr_b);

		m_tagTemper[i].Initialize(0,this,
			NULL,NULL,
			NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagTemper[i].SetSize(&siInfo);
		m_tagTemper[i].SetCaption(_T("N/A"));
		m_tagTemper[i].LinkTextGL(pTextGL);
		if(i==0)
		{
			m_tagTemper[i].SetPos(-240,136);
			m_tagTemper[i].SetCaptionPos(BTN_CAPTION_LEFT);
		}
		else if(i==1)
		{
			m_tagTemper[i].SetPos(240,136);
			m_tagTemper[i].SetCaptionPos(BTN_CAPTION_RIGHT);
		}
		else if(i==2)
		{
			m_tagTemper[i].SetPos(-240,-194);
			m_tagTemper[i].SetCaptionPos(BTN_CAPTION_LEFT);
		}
		else if(i==3)
		{
			m_tagTemper[i].SetPos(240,-194);
			m_tagTemper[i].SetCaptionPos(BTN_CAPTION_RIGHT);
		}
		m_tagTemper[i].SetCaptionHeight(24);
		//m_tagTemper[i].SetCaptionCharStyle(STYLE_ART);
		m_tagTemper[i].EnableDrawBack(FALSE);
		//m_tagTemper[i].SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagTemper[i].SetCaptionColor(&cr_b);

		m_tagTireBack[i].Initialize(0,this,m_pSkinManager->GetTexture(TEXID_463));
		m_tagTireBack[i].SetScale(1.1f,1.0f);
		m_tagTireBack[i].EnableLight(TRUE);
// 		if(i==0)
// 		m_tagTireBack[i].SetLightColor(&cr_r);
// 		if(i==3)
// 			m_tagTireBack[i].SetLightColor(&cr_g);

		//btn
		m_btnTire[i].Initialize(i,this,m_pSkinManager->GetTexture(TEXID_371),NULL,
			NULL,NULL,NULL,NULL,NULL,NULL);
		m_btnTire[i].SetNotifyFunc(CPanelVehicleInfoBody::OnNotify,this);
		m_btnTire[i].SetShowScale(0.7f);

		if(i==0)
		{
			m_tagTireBack[i].SetPos(-244,158);
			m_btnTire[i].SetPos(-160,158);
		}
		else if(i==1)
		{
			m_tagTireBack[i].SetPos(244,158);
			m_btnTire[i].SetPos(160,158);
		}
		else if(i==2)
		{
			m_tagTireBack[i].SetPos(-244,-172);
			m_btnTire[i].SetPos(-160,-172);
		}
		else if(i==3)
		{
			m_tagTireBack[i].SetPos(244,-172);
			m_btnTire[i].SetPos(160,-172);
		}
	}
#endif

// 	SetInfoDoorFL(0);
// 	SetInfoDoorFR(0);
// 	SetInfoDoorRL(0);
// 	SetInfoDoorRR(0);

	return TRUE;
}

void CALLBACK CPanelVehicleInfoBody::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(1,(_T("CPanelVehicleInfoBody OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelVehicleInfoBody *panel=(CPanelVehicleInfoBody*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelVehicleInfoBody::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case 0://btn tire 0
		if (wMsg == NOTIFY_SHORT_CLICK)
		{
			//SetInfoDoorFL(0);
		}
		else if (wMsg == NOTIFY_LONG_CLICK)
		{
		}
		break;
	case 1://btn tire 1
		if (wMsg == NOTIFY_SHORT_CLICK)
		{
			//SetInfoDoorFR(0);
		}
		else if (wMsg == NOTIFY_LONG_CLICK)
		{
		}
		break;
	case 2://btn tire 2
		if (wMsg == NOTIFY_SHORT_CLICK)
		{
			//SetInfoDoorRL(0);
		}
		else if (wMsg == NOTIFY_LONG_CLICK)
		{
		}
		break;
	case 3://btn tire 3
		if (wMsg == NOTIFY_SHORT_CLICK)
		{
// 			static bool flag=0;
// 			flag=!flag;
// 			SetInfoTrunk(!flag);
		}
		else if (wMsg == NOTIFY_LONG_CLICK)
		{
		}
		break;
	}
}

void CPanelVehicleInfoBody::SetInfoTrunk(BOOL bNormal)
{
	m_tagDoorTrunk.Show(!bNormal);
}
void CPanelVehicleInfoBody::SetInfoDoorFL(BOOL bNormal)
{
	if(m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType == 0)//left
	{
		VIEW_STATE vs=m_tagDoorFL.GetCurViewState();
		vs.fRotateZ=bNormal?-90:-30;
// 		m_tagDoorFL.SetIniViewState(&vs);
		m_tagDoorFL.SetAimViewState(&vs);
	}
	else
	{
		VIEW_STATE vs=m_tagDoorFR.GetCurViewState();
		vs.fRotateZ=bNormal?-90:-30;
// 		m_tagDoorFR.SetIniViewState(&vs);
		m_tagDoorFR.SetAimViewState(&vs);
	}
}
void CPanelVehicleInfoBody::SetInfoDoorFR(BOOL bNormal)
{
	if(m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType == 0)//left
	{
		VIEW_STATE vs=m_tagDoorFR.GetCurViewState();
		vs.fRotateZ=bNormal?-90:-30;
		m_tagDoorFR.SetAimViewState(&vs);
	}
	else
	{
		VIEW_STATE vs=m_tagDoorFL.GetCurViewState();
		vs.fRotateZ=bNormal?-90:-30;
		m_tagDoorFL.SetAimViewState(&vs);
	}
}
void CPanelVehicleInfoBody::SetInfoDoorRL(BOOL bNormal)
{
	if(m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType == 0)//left
	{
		VIEW_STATE vs=m_tagDoorRL.GetCurViewState();
		vs.fRotateZ=bNormal?90:30;
		m_tagDoorRL.SetAimViewState(&vs);
	}
	else
	{
		VIEW_STATE vs=m_tagDoorRR.GetCurViewState();
		vs.fRotateZ=bNormal?90:30;
		m_tagDoorRR.SetAimViewState(&vs);
	}
}
void CPanelVehicleInfoBody::SetInfoDoorRR(BOOL bNormal)
{
	if(m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType == 0)//left
	{
		VIEW_STATE vs=m_tagDoorRR.GetCurViewState();
		vs.fRotateZ=bNormal?90:30;
		m_tagDoorRR.SetAimViewState(&vs);
	}
	else
	{
		VIEW_STATE vs=m_tagDoorRL.GetCurViewState();
		vs.fRotateZ=bNormal?90:30;
		m_tagDoorRL.SetAimViewState(&vs);
	}
}

void CPanelVehicleInfoBody::SetTpmsPressure(UINT idTire,const wchar_t* pValue)
{
#if ENABLE_TPMS_UI
	if(idTire<5)
	{
		m_tagPressure[idTire].SetCaption(pValue);
	}
#endif
}
void CPanelVehicleInfoBody::SetTpmsTemper(UINT idTire,const wchar_t* pValue)
{
#if ENABLE_TPMS_UI
	if(idTire<5)
	{
		m_tagTemper[idTire].SetCaption(pValue);
	}
#endif
}
void CPanelVehicleInfoBody::SetTpmsAlarm(UINT idTire,UINT idAlarmType)
{
#if ENABLE_TPMS_UI
	COLORGL cr_r={1,0,0,1};
	//RETAILMSG(1,(_T("SetTpmsAlarm idTire=%d, idAlarmType=%c%c\r\n"),idTire,char(idAlarmType>>8),char(idAlarmType)));
	if(idTire<5)
	{
		m_tagTireBack[idTire].SetLightColor(&cr_r);
		m_idAlarmType[idTire] = idAlarmType;
		m_dwAlarmTick[idTire] = GetTickCount();
	}
#endif
}
void CPanelVehicleInfoBody::SetTpmsSensorID(UINT idTire,DWORD idSensorID)
{
#if ENABLE_TPMS_UI
	RETAILMSG(1,(_T("SetTpmsSensorID idTire=%d, idSensorID=%X\r\n"),idTire,idSensorID));
	if(idTire<5)
	{
		m_dwSensorID[idTire] = idSensorID;
	}
#endif
}
void CPanelVehicleInfoBody::CancelTpmsAlarm(UINT idTire)
{
#if ENABLE_TPMS_UI
	COLORGL cr_b={1,1,1,1};
	if(idTire<5)
	{
		m_tagTireBack[idTire].SetLightColor(&cr_b);
		m_idAlarmType[idTire] = 0;
	}
#endif
}
