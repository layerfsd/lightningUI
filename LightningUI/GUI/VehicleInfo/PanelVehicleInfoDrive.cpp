#include "PanelVehicleInfoDrive.h"
#include "../../resource.h"



//CPanelVehicleInfoDrive
CPanelVehicleInfoDrive::CPanelVehicleInfoDrive(void)
{
	m_pCurveVertex = NULL;
	m_iSpeedSempleCount=0;
	m_dwSpeedSampleTick=0;
	m_bSamplingSpeed = FALSE;
	m_bGetSpeedAccData = FALSE;
	m_iEnginSpeed=0;
	m_iInstantVel=0;
	m_bCheckDashboard=FALSE;
}
CPanelVehicleInfoDrive::~CPanelVehicleInfoDrive(void)
{
	if(m_pCurveVertex)
	{
		delete[] m_pCurveVertex;
		m_pVertex=NULL;
	}
}

BOOL CPanelVehicleInfoDrive::IsReady()
{
	return CPanelGL::IsReady() && m_barEngin.IsReady() && m_barSpeed.IsReady();

}
void CPanelVehicleInfoDrive::OnGestureMsg(UINT idGesture)
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
void CPanelVehicleInfoDrive::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

	//test
	//ClearSpeedData();
	if(m_iEnginSpeed==0 && m_iInstantVel==0)
	{
		m_bCheckDashboard=TRUE;
		m_barSpeed.SetValue(220,FALSE);
		m_barEngin.SetValue(7000,FALSE);
	}
}
void CPanelVehicleInfoDrive::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}

void CPanelVehicleInfoDrive::Render()
{
// 	static CTexture *pTexRadarBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_360):NULL;
	//COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};

	if(!BeginRender())return;

	COLORGL cr={0.0,0.0,0.0,m_fCurAlpha*0.2};

	DrawRect(0,-g_iScreenHeight/2+50,g_iClientWidth-240,100,&cr);

// 	m_tagBodyTest.SetAlpha(m_fCurAlpha);
// 	m_tagBodyTest.Render();

	
// 	DrawRect(pTexRadarBack,298,15,196-16,340-16,8,&cr,TRUE);

// 	m_box1.SetAlpha(m_fCurAlpha);
// 	m_box1.Render();

// 	m_box2.SetAlpha(m_fCurAlpha);
// 	m_box2.Render();

	m_tagGridBack.SetAlpha(m_fCurAlpha);
	m_tagGridBack.Render();

	//test curve:
	if(m_pCurveVertex && m_iSpeedSempleCount)
	{
		glDisable(GL_TEXTURE_2D);
		glVertexPointer(3, GL_FLOAT, 0, m_pCurveVertex);
		glLineWidth(3.0);

		glColor4f(0.8f,0.0f,0.0f,m_fCurAlpha);

		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);

		glDrawArrays(GL_LINE_STRIP, 0, m_iSpeedSempleCount);
		// 		glDrawArrays(GL_POINTS, 0, m_iSpeedSempleCount);

	}

	/*
	//test:
	static DWORD dwlast=0;
	if(GetTickCount()-dwlast>500)
	{
		dwlast=GetTickCount();

		if(m_barEngin.GetValue()==0)
			m_barEngin.SetValue(7000);
		else if(m_barEngin.GetValue()==7000)
			m_barEngin.SetValue(0);
		else
			m_barEngin.SetValue(0);
	}
	//
	*/
	

	//test animation:
	if(m_bCheckDashboard && m_iEnginSpeed==0 && m_iInstantVel==0)
	{
		if(m_barEngin.IsReady() && m_barSpeed.IsReady())
		{
			m_bCheckDashboard = FALSE;
			m_barSpeed.SetValue(0);
			m_barEngin.SetValue(0);
		}
	}

	m_barSpeed.SetAlpha(m_fCurAlpha);
	m_barSpeed.Render();

	m_barEngin.SetAlpha(m_fCurAlpha);
	m_barEngin.Render();

// 	m_barSpeed.SetAlpha(m_fCurAlpha);
// 	m_barSpeed.Render();


	//
	m_btnBattery.SetAlpha(m_fCurAlpha);
	m_btnBattery.Render();
	m_btnTrip.SetAlpha(m_fCurAlpha);
	m_btnTrip.Render();
	m_btnOil.SetAlpha(m_fCurAlpha);
	m_btnOil.Render();

	//
	m_btnSpeedData.SetAlpha(m_fCurAlpha);
	m_btnSpeedData.Render();

	//alert:
	m_iconHandbrakeAlert.SetAlpha(m_fCurAlpha);
	m_iconHandbrakeAlert.Render();
	m_iconWashWaterAlert.SetAlpha(m_fCurAlpha);
	m_iconWashWaterAlert.Render();
	m_iconBeltAlert.SetAlpha(m_fCurAlpha);
	m_iconBeltAlert.Render();

	/*
	//test:
	static int i=0;
	static DWORD last_sample=0;
	if(GetTickCount()-last_sample>100)
	{
		SetInfoInstantVel(i*200);
		i++;
		if(i>100)
			i=0;

		last_sample=GetTickCount();
	}
	*/
	

	//

	EndRender();

}
BOOL CPanelVehicleInfoDrive::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//test curve:
	m_pCurveVertex = new GLfloat[MAX_SPEED_SAMPLE*3];
// 	for (int i=0;i<100;i+=3)
// 	{
// 		m_pCurveVertex[i] = -300+i;
// 		m_pCurveVertex[i+1]=100*sin(3.14*i/100.0);
// 		m_pCurveVertex[i+2]=0;
// 	}

	//
	DASHBOARDSTRUCTURE stru;
	memset(&stru,0,sizeof(stru));

	stru.idBack=TEXID_360;
	stru.idPointer=TEXID_361;
	stru.fStartAngle=180.0f;
	stru.fEndAngle=-108.0f;

	m_barEngin.Initialize(0,this,&stru,pSkinManager);
// 	m_barEngin.SetPos(0,52);
// 	m_barEngin.SetShowScale(1.4f);
	m_barEngin.SetPos(250,0);
	m_barEngin.SetShowScale(1.0f);
	m_barEngin.SetRange(0,8000);
	m_barEngin.SetValue(0,FALSE);

	stru.idBack=TEXID_359;
	stru.fEndAngle=-150.0f;

	m_barSpeed.Initialize(0,this,&stru,pSkinManager);
// 	m_barSpeed.SetPos(250,0);
	m_barSpeed.SetPos(-10,52);
	m_barSpeed.SetShowScale(1.4f);
	m_barSpeed.SetRange(0,220);
	m_barSpeed.SetValue(0,FALSE);
	//m_barSpeed.SetResponseSpeed(25);

	//
	m_tagGridBack.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_367));
	m_tagGridBack.SetPos(-230,-40);

	//
	m_iconOil.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_331));
	m_iconOilAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_336));

	m_iconWashWaterAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_334));
	//m_iconWashWaterAlert.SetShowScale(0.6f);
	m_iconWashWaterAlert.SetPos(25/*40*/,0);
	m_iconWashWaterAlert.Show(FALSE);

	m_iconHandbrakeAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_333));
	//m_iconHandbrakeAlert.SetShowScale(0.6f);
	m_iconHandbrakeAlert.SetPos(60/*90*/,-45/*-25*/);
	m_iconHandbrakeAlert.Show(FALSE);

	m_iconBeltAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_335));
	//m_iconBeltAlert.SetShowScale(0.6f);
	m_iconBeltAlert.SetPos(25/*40*/,-60/*-55*/);
	m_iconBeltAlert.Show(FALSE);

	m_iconBattery.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_332));
	m_iconBatteryAlert.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_338));

	m_iconTrip.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_327));

	COLORGL cr_w={1,1,1,1};
	COLORGL cr_gray={0.7,0.7,0.7,1};
	COLORGL cr_dark={0.2,0.2,0.2,1};
	COLORGL cr_black={0.0,0.0,0.0,1};
	COLORGL cr_red={1.0,0.0,0.0,1};

	/*
	int cx=240+20;
	int cy=100;

	LISTBOXSTRUCTURE box;

	int count=(g_iClientHeight-24)/96;
	if(count<=0)
		count=1;

	memset(&box,0,sizeof(box));

	box.iSpacing=(g_iClientHeight-24)/count;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=g_iClientHeight;//(box.iSpacing * count);


	box.idTexIndexBase=TEXID_606;
	box.idTexIndexFocus=TEXID_606;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.pxSlider=50;

	m_box1.Initialize(0,this,&box,pSkinManager);
	m_box1.SetPos(0,0);
	m_box1.LinkTextGL(pTextGL);
	m_box1.Show(TRUE,FALSE);
	m_box1.SetAlpha(1.0f);
	m_box1.SetCaptionColor(&cr_black,&cr_black);
	m_box1.SetNotifyFunc(NULL,NULL);


// 	m_box2.Initialize(0,this,&box,pSkinManager);
// 	m_box2.SetPos(180,15);
// 	m_box2.LinkTextGL(pTextGL);
// 	m_box2.Show(TRUE,FALSE);
// 	m_box2.SetAlpha(1.0f);
// 	m_box2.SetCaptionColor(&cr_dark,&cr_dark);
// 	m_box2.SetNotifyFunc(NULL,NULL);

	//test:
	CListBoxItemGL *pItem=NULL;

	//box1
	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_OIL),_T("0.0 L"));
	pItem->SetIcon(&m_iconOil);
	m_box1.AddItem(pItem,IDS_CAN_OIL);

	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_BATTERY),_T("0.0 V"));
	pItem->SetIcon(&m_iconBattery);
	m_box1.AddItem(pItem,IDS_CAN_BATTERY);

	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_TRIP),_T("0 Km"));
	pItem->SetIcon(&m_iconTrip);
	m_box1.AddItem(pItem,IDS_CAN_TRIP);
*/

// 	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_OUTSIDE_TEMP),_T("0.0 C"));
// 	pItem->SetIcon(&m_iconTemprature);
// 	m_box1.AddItem(pItem,IDS_CAN_OUTSIDE_TEMP);


// 	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_ENGIN_SPEED),_T("0 RPM"));
// 	pItem->SetIcon(&m_iconEngin);
// 	m_box1.AddItem(pItem,IDS_CAN_ENGIN_SPEED);

// 	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_VELOCITY),_T("0.00 Km/h"));
// 	pItem->SetIcon(&m_iconSpeed);
// 	m_box1.AddItem(pItem,IDS_CAN_VELOCITY);

	/*
	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_BELT),GetResourceString(IDS_SETTINGS_NORMAL));
	pItem->SetIcon(&m_iconBelt);
	m_box1.AddItem(pItem,IDS_CAN_BELT);

	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_CLEANINGWATER),GetResourceString(IDS_SETTINGS_NORMAL));
	pItem->SetIcon(&m_iconWashWater);
	m_box1.AddItem(pItem,IDS_CAN_CLEANINGWATER);

	pItem = new CListBoxItemGL(GetResourceString(IDS_CAN_HANDBRAKE),GetResourceString(IDS_SETTINGS_NORMAL));
	pItem->SetIcon(&m_iconHandbrake);
	m_box1.AddItem(pItem,IDS_CAN_HANDBRAKE);
*/

	SIZE sibtn={240,80};

	m_btnBattery.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBattery.SetSize(&sibtn);
	m_btnBattery.SetPos(-220,-g_iClientHeight/2+50);
	m_btnBattery.SetIcon(&m_iconBattery);
	m_btnBattery.LinkTextGL(pTextGL);
	m_btnBattery.SetCaption(_T("0.0V"));
	m_btnBattery.SetCaptionPos(BTN_CAPTION_LEFT,90);
	m_btnBattery.SetCaptionColor(&cr_black);
	m_btnBattery.SetCaptionHeight(28);
	m_btnBattery.SetIconPos(-66,0);
	m_btnBattery.EnableDrawBack(FALSE);

	m_btnTrip.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTrip.SetSize(&sibtn);
	m_btnTrip.SetPos(220,-g_iClientHeight/2+50);
	m_btnTrip.SetIcon(&m_iconTrip);
	m_btnTrip.LinkTextGL(pTextGL);
	m_btnTrip.SetCaption(_T("0Km"));
	m_btnTrip.SetCaptionPos(BTN_CAPTION_LEFT,90);
	m_btnTrip.SetCaptionColor(&cr_black);
	m_btnTrip.SetCaptionHeight(28);
	m_btnTrip.SetIconPos(-66,0);
	m_btnTrip.EnableDrawBack(FALSE);

	m_btnOil.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnOil.SetSize(&sibtn);
	m_btnOil.SetPos(0,-g_iClientHeight/2+50);
	m_btnOil.SetIcon(&m_iconOil);
	m_btnOil.LinkTextGL(pTextGL);
	m_btnOil.SetCaption(_T("0.0L"));
	m_btnOil.SetCaptionPos(BTN_CAPTION_LEFT,90);
	m_btnOil.SetCaptionColor(&cr_black);
	m_btnOil.SetCaptionHeight(28);
	m_btnOil.SetIconPos(-66,0);
	m_btnOil.EnableDrawBack(FALSE);

	//
	SIZE sibtn2={120,80};
	m_btnSpeedData.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSpeedData.SetSize(&sibtn);
	m_btnSpeedData.SetPos(-80,-140);
	m_btnSpeedData.LinkTextGL(pTextGL);
	m_btnSpeedData.SetCaption(_T(""));
	m_btnSpeedData.SetCaptionPos(BTN_CAPTION_LEFT);
	m_btnSpeedData.SetCaptionColor(&cr_red);
	m_btnSpeedData.SetCaptionHeight(32);
	m_btnSpeedData.EnableDrawBack(FALSE);



	return TRUE;
}


void CPanelVehicleInfoDrive::SetInfoBelt(BOOL bNormal)
{
	//m_box1.SetItemValue(IDS_CAN_BELT,GetResourceString(bNormal?IDS_SETTINGS_NORMAL:IDS_CAN_BELT_UNBUCKLED));

	m_iconBeltAlert.Show(!bNormal);
}
void CPanelVehicleInfoDrive::SetInfoWashWater(BOOL bNormal)
{
	//m_box1.SetItemValue(IDS_CAN_CLEANINGWATER,GetResourceString(bNormal?IDS_SETTINGS_NORMAL:IDS_SETTINGS_LOW));

	//m_box1.SetIndexIcon(IDS_CAN_CLEANINGWATER,bNormal?&m_iconWashWater:&m_iconWashWaterAlert);

	m_iconWashWaterAlert.Show(!bNormal);
}
void CPanelVehicleInfoDrive::SetInfoHandBrake(BOOL bNormal)
{
	//m_box1.SetItemValue(IDS_CAN_HANDBRAKE,GetResourceString(bNormal?IDS_SETTINGS_NORMAL:IDS_CAN_HANDBRAKE_UNRELEASED));

	//m_box1.SetIndexIcon(IDS_CAN_HANDBRAKE,bNormal?&m_iconHandbrake:&m_iconHandbrakeAlert);

	m_iconHandbrakeAlert.Show(!bNormal);
}


void CPanelVehicleInfoDrive::SetInfoEnginSpeed(INT iRPM)
{
	m_iEnginSpeed=iRPM;
	m_barEngin.SetValue(iRPM);
}
void CPanelVehicleInfoDrive::SetInfoInstantVel(INT iKmh)
{
	static int last=0;
	static DWORD dwlast=0;

	if(GetTickCount()-dwlast<1000 && abs(iKmh-last)>5000)
	{
		//invalid data:
		return;
	}

	dwlast = GetTickCount();
	last = iKmh;

	m_iInstantVel=iKmh;

	m_barSpeed.SetValue(int(iKmh*0.01f));

	if(iKmh>0)
	{
		if(!m_bSamplingSpeed)
		{
			ClearSpeedData();
		}

		if(SampleSpeed(iKmh))
		{
			m_bSamplingSpeed = TRUE;
		}
		else
		{
			//m_bSamplingSpeed = FALSE;
		}
	}
	else
	{
		//ClearSpeedData();
		m_bSamplingSpeed = FALSE;
	}
}
void CPanelVehicleInfoDrive::OnGPSGroundSpeed(INT iKmh)
{
	SetInfoInstantVel(iKmh*100);
}
void CPanelVehicleInfoDrive::SetInfoBattery(INT iV)
{
	wchar_t temp[16];
	swprintf_s(temp,16,_T("%.2f V"),iV*0.01);
	m_btnBattery.SetCaption(temp);
}
void CPanelVehicleInfoDrive::SetInfoOutsideTemp(INT iC)
{
	wchar_t temp[16];
	swprintf_s(temp,16,_T("%.1f C"),iC*0.1);

}
void CPanelVehicleInfoDrive::SetInfoTrip(INT iKm)
{
	wchar_t temp[16];
	swprintf_s(temp,16,_T("%d Km"),iKm);
	m_btnTrip.SetCaption(temp);
}
void CPanelVehicleInfoDrive::SetInfoOil(INT iL)
{
	wchar_t temp[16];
	swprintf_s(temp,16,_T("%d L"),iL);
	m_btnOil.SetCaption(temp);
}

void CPanelVehicleInfoDrive::SetInfoAlertOil(BOOL bAlert)
{
	m_btnOil.SetIcon(bAlert?&m_iconOilAlert:&m_iconOil);
}
void CPanelVehicleInfoDrive::SetInfoAlertBettery(BOOL bAlert)
{
	m_btnBattery.SetIcon(bAlert?&m_iconBatteryAlert:&m_iconBattery);

}

void CPanelVehicleInfoDrive::testFun()
{
// 	m_box1.MakeDefaultAni(ANIM_FLY_IN);
// 	m_box2.MakeDefaultAni(ANIM_FLY_IN);
}

BOOL CPanelVehicleInfoDrive::SampleSpeed(INT iSpeed)
{
	if(m_iSpeedSempleCount>=MAX_SPEED_SAMPLE)
		return FALSE;

	if(m_iSpeedSempleCount==0)//first sample
	{
		m_dwSpeedSampleTick=GetTickCount();
	}

	DWORD delt=GetTickCount()-m_dwSpeedSampleTick;

	if(delt<100000)
	{
		m_pCurveVertex[m_iSpeedSempleCount*3] = -320.0f+delt*35.0f/2000.0f;
		m_pCurveVertex[m_iSpeedSempleCount*3+1]=-83.0f+iSpeed/100.0f*32.0f/20.0f;
		m_pCurveVertex[m_iSpeedSempleCount*3+2]=0;

		m_iSpeedSempleCount++;

		//get data:
		if(iSpeed>=10000 && !m_bGetSpeedAccData)
		{
			wchar_t str[32];
			swprintf_s(str,32,_T("%.2fS"),delt/1000.0f);
			m_btnSpeedData.SetCaption(str);
			m_bGetSpeedAccData=TRUE;
		}

		return TRUE;
	}

	return FALSE;
}
void CPanelVehicleInfoDrive::ClearSpeedData()
{
	m_iSpeedSempleCount = 0;
// 	m_btnSpeedData.SetCaption(_T("--S"));
	m_btnSpeedData.SetCaption(_T(""));
	m_bGetSpeedAccData=FALSE;
}