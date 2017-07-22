#include "PanelVehicleInfoAC.h"
#include "../../resource.h"
#include "../../PWM/pwm.h"
#include <windev.h>
#include "../../Communication/keydef.h"

#define TIMER_ECON_PROC	301
CPanelVehicleInfoAC *g_pVehicleInfoAC=NULL;

//CPanelVehicleInfoAC
CPanelVehicleInfoAC::CPanelVehicleInfoAC(void)
{
	m_iWindLevel = 0;

	m_iTempDriver = 0;
	m_iTempCoDriver = 0;
	m_iTempOutside = 0;
	m_bOnDual = TRUE;
	m_bAutoBig = FALSE;
	m_bAutoSmall = FALSE;
}
CPanelVehicleInfoAC::~CPanelVehicleInfoAC(void)
{

}

BOOL CPanelVehicleInfoAC::IsReady()
{
	return CPanelGL::IsReady() && m_btnTempDriver.IsReady() && m_btnTempCoDriver.IsReady();

}

void CPanelVehicleInfoAC::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		//SwitchPage(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_MAIN,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}

void CPanelVehicleInfoAC::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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
	SetInfoAirTempDriver(270);
	SetInfoAirTempCoDriver(260);
	SetInfoOutsideTemp(310);
	SetInfoAirStatus(0x40);
	SetInfoAirSeatHeating(0);
	SetInfoAirWind(0x03);
	*/

}
void CPanelVehicleInfoAC::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}

void CPanelVehicleInfoAC::Render()
{
	if(!BeginRender())return;

	COLORGL cr={0.0,0.0,0.0,m_fCurAlpha*0.6};

	m_tagACBack.SetAlpha(m_fCurAlpha);
	m_tagACBack.Render();

	DrawRect(0,-g_iScreenHeight/2+50,g_iClientWidth-240,100,&cr);

	m_btnTempOutside.SetAlpha(m_fCurAlpha);
	m_btnTempOutside.Render();

// 	m_iconDegree1.SetAlpha(m_fCurAlpha);
// 	m_iconDegree1.Render();
// 	m_iconDegree2.SetAlpha(m_fCurAlpha);
// 	m_iconDegree2.Render();
	m_iconDegree3.SetAlpha(m_fCurAlpha);
	m_iconDegree3.Render();

	/*

	////
	if(m_iWindLevel>0)
	{
		VIEW_STATE vs;
		vs=m_tagFan.GetCurViewState();	
		vs.fRotateZ+=2*m_iWindLevel;
		if(vs.fRotateZ>360)
			vs.fRotateZ=vs.fRotateZ-360;
		m_tagFan.SetCurViewState(&vs);
	}
	m_tagFan.SetAlpha(m_fCurAlpha);
	m_tagFan.Render();
	//////////////////////////////////////////////////////////////////////////

	m_tagSeatLeft.SetAlpha(m_fCurAlpha);
	m_tagSeatLeft.Render();

	m_tagSeatRight.SetAlpha(m_fCurAlpha);
	m_tagSeatRight.Render();
*/

	m_btnSeatDriver.SetAlpha(m_fCurAlpha);
	m_btnSeatDriver.Render();
	m_btnSeatCoDriver.SetAlpha(m_fCurAlpha);
	m_btnSeatCoDriver.Render();

	//wind
	m_iconBlowHorizontalLeft.SetAlpha(m_fCurAlpha);
	m_iconBlowHorizontalLeft.Render();

	m_iconBlowHorizontalRight.SetAlpha(m_fCurAlpha);
	m_iconBlowHorizontalRight.Render();

	m_iconBlowDownLeft.SetAlpha(m_fCurAlpha);
	m_iconBlowDownLeft.Render();

	m_iconBlowDownRight.SetAlpha(m_fCurAlpha);
	m_iconBlowDownRight.Render();

	//
	m_btnTempCoDriver.SetAlpha(m_fCurAlpha);
	m_btnTempCoDriver.Render();

	m_btnTempDriver.SetAlpha(m_fCurAlpha);
	m_btnTempDriver.Render();


	//
	BOOL br=IsReady();
	BOOL br1=m_btnTempDriver.IsReady();
	BOOL br2=m_btnTempCoDriver.IsReady();
	for (int i=0;i<7;i++)
	{
		m_tagFanBarLeft[i].Show(br1);
		m_tagFanBarLeft[i].SetAlpha(m_fCurAlpha);
		m_tagFanBarLeft[i].Render();

		m_tagFanBarRight[i].Show(br2&&m_bOnDual);
		m_tagFanBarRight[i].SetAlpha(m_fCurAlpha);
		m_tagFanBarRight[i].Render();
	}
	/*	

// 	m_btnBlowHorizontal.SetAlpha(m_fCurAlpha);
// 	m_btnBlowHorizontal.Render();
// 	m_btnBlowDown.SetAlpha(m_fCurAlpha);
// 	m_btnBlowDown.Render();

/*
	m_btnACStatus.SetAlpha(m_fCurAlpha);
	m_btnACStatus.Render();
	m_btnPowerStatus.SetAlpha(m_fCurAlpha);
	m_btnPowerStatus.Render();
*/
	

	//
	m_btnAutoSmall.Show(!m_bOnDual&&br&&m_bAutoSmall);
	m_btnAutoSmall.SetAlpha(m_fCurAlpha);
	m_btnAutoSmall.Render();

	m_btnAutoBig.Show(!m_bOnDual&&br&&m_bAutoBig);
	m_btnAutoBig.SetAlpha(m_fCurAlpha);
	m_btnAutoBig.Render();
	
	//
	m_btnAQS.SetAlpha(m_fCurAlpha);
	m_btnAQS.Render();
	/*
	m_btnDual.SetAlpha(m_fCurAlpha);
	m_btnDual.Render();
	*/


	//
	m_btnMaxFront.SetAlpha(m_fCurAlpha);
	m_btnMaxFront.Render();
	m_btnBlowUp.SetAlpha(m_fCurAlpha);
	m_btnBlowUp.Render();

	m_btnInnerLoop.SetAlpha(m_fCurAlpha);
	m_btnInnerLoop.Render();
	m_btnRear.SetAlpha(m_fCurAlpha);
	m_btnRear.Render();

#if ENABLE_ECON_UI
	m_btnECON.SetAlpha(m_fCurAlpha);
	m_btnECON.Render();
	m_sliECON.SetAlpha(m_fCurAlpha);
	m_sliECON.Render();
#endif	
	

	EndRender();

}
BOOL CPanelVehicleInfoAC::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	g_pVehicleInfoAC = this;

	COLORGL cr_w={1,1,1,1};
	COLORGL cr_black={0,0,0,1};
	COLORGL cr_blue={0.0f/256.0f,255.0f/256.0f,255.0f/256.0f,1.0f};
	COLORGL cr_warm={255.0f/256.0f,80.0f/256.0f,0.0f/256.0f,1.0f};

	VIEW_STATE vs;

	GLfloat texCoord_s01000[]={	
		0.0, 64.0/512.0,	
		0.0, 1.0,	
		784.0/1024.0, 64.0/512.0,	
		784.0/1024.0, 1.0};

	m_tagACBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_362),texCoord_s01000);
	m_tagACBack.SetPos(0,-76);

	SIZE sibtn={220,80};

// 	m_iconDegree1.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_365));
// 	m_iconDegree1.SetPos(-60,170-4);
// 	m_iconDegree2.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_365));
// 	m_iconDegree2.SetPos(260,170-4);
	m_iconDegree3.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_365));
	m_iconDegree3.SetPos(g_iClientWidth/2-120-32,g_iClientHeight/2-40);
	m_iconDegree3.SetShowScale(0.65);
	m_iconDegree3.Show(FALSE);

	//
	m_iconTempOutside.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_329));

	m_btnTempOutside.Initialize(1,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTempOutside.SetSize(&sibtn);
	m_btnTempOutside.SetPos(g_iClientWidth/2-120-110-52,g_iClientHeight/2-40);
	m_btnTempOutside.SetIcon(&m_iconTempOutside);
	m_btnTempOutside.LinkTextGL(pTextGL);
	m_btnTempOutside.SetCaption(_T("N/A"));
	m_btnTempOutside.SetCaptionPos(BTN_CAPTION_RIGHT/*,88*/);
	m_btnTempOutside.SetCaptionColor(&cr_black);
	m_btnTempOutside.SetCaptionHeight(32);
	m_btnTempOutside.SetIconPos(-16,4);
	m_btnTempOutside.EnableDrawBack(FALSE);
	m_btnTempOutside.Show(FALSE);
// 	m_btnTempOutside.SetNotifyFunc(&CPanelVehicleInfoAC::OnNotify,this);

	/*
	m_tagFan.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_323));
	m_tagFan.SetShowScale(1.25f);
	m_tagFan.SetPos(0,20);

	VIEW_STATE vs;
	m_tagSeatLeft.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_324));
	vs=m_tagSeatLeft.GetCurViewState();
	vs.fRotateY=-180;
	m_tagSeatLeft.SetIniViewState(&vs);
	m_tagSeatLeft.SetPos(-260,0);

	m_tagSeatRight.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_324));
	m_tagSeatRight.SetPos(260,0);
*/

	m_iconBlowHorizontalLeft.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_363));
	vs=m_iconBlowHorizontalLeft.GetCurViewState();
// 	vs.fRotateY=-180;
	m_iconBlowHorizontalLeft.SetIniViewState(&vs);
	m_iconBlowHorizontalLeft.SetPos(-130,40);
	m_iconBlowHorizontalLeft.EnableLight(TRUE);
	m_iconBlowHorizontalLeft.SetLightColor(&cr_blue);
	m_iconBlowHorizontalLeft.Show(FALSE);

	m_iconBlowHorizontalRight.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_363));
	vs=m_iconBlowHorizontalRight.GetCurViewState();
	vs.fRotateY=-180;
	m_iconBlowHorizontalRight.SetIniViewState(&vs);
	m_iconBlowHorizontalRight.SetPos(130,40);
	m_iconBlowHorizontalRight.EnableLight(TRUE);
	m_iconBlowHorizontalRight.SetLightColor(&cr_blue);
	m_iconBlowHorizontalRight.Show(FALSE);

	m_iconBlowDownLeft.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_364));
	vs=m_iconBlowDownLeft.GetCurViewState();
	vs.fRotateZ=-45;
	m_iconBlowDownLeft.SetIniViewState(&vs);
	m_iconBlowDownLeft.SetPos(-200,-120);
	m_iconBlowDownLeft.EnableLight(TRUE);
	m_iconBlowDownLeft.SetLightColor(&cr_blue);
	m_iconBlowDownLeft.Show(FALSE);

	m_iconBlowDownRight.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_364));
	vs=m_iconBlowDownRight.GetCurViewState();
	vs.fRotateY=-180;
	vs.fRotateZ=-45;
	m_iconBlowDownRight.SetIniViewState(&vs);
	m_iconBlowDownRight.SetPos(200,-120);
	m_iconBlowDownRight.EnableLight(TRUE);
	m_iconBlowDownRight.SetLightColor(&cr_blue);
	m_iconBlowDownRight.Show(FALSE);

	
	for (int i=0;i<7;i++)
	{
		m_tagFanBarLeft[i].Initialize(0,this,
			m_pSkinManager->GetTexture(TEXID_325),NULL,
			m_pSkinManager->GetTexture(TEXID_326),NULL,
			NULL,NULL,
			NULL,NULL);

		VIEW_STATE vs=m_tagFanBarLeft[i].GetCurViewState();
		vs.fRotateZ=90-30*i;
		m_tagFanBarLeft[i].SetIniViewState(&vs);
		m_tagFanBarLeft[i].SetPos(-300+64*cos((180-30*i)*PI/180),-70+64*sin((180-30*i)*PI/180));
		//m_tagFanBarLeft[i].SetStatus(BS_FOCUS);
	}
	for (int i=0;i<7;i++)
	{
		m_tagFanBarRight[i].Initialize(0,this,
			m_pSkinManager->GetTexture(TEXID_325),NULL,
			m_pSkinManager->GetTexture(TEXID_326),NULL,
			NULL,NULL,
			NULL,NULL);

		VIEW_STATE vs=m_tagFanBarRight[i].GetCurViewState();
		vs.fRotateZ=90-30*i;
		m_tagFanBarRight[i].SetIniViewState(&vs);
		m_tagFanBarRight[i].SetPos(300+64*cos((180-30*i)*PI/180),-70+64*sin((180-30*i)*PI/180));
		//m_tagFanBarRight[i].SetStatus(BS_FOCUS);
	}

	SIZE siInfo={400,64};
	m_btnTempDriver.Initialize(IDC_VEHICLEINFO_TAG_TEMP_DRIVER,this,
		m_pSkinManager->GetTexture(TEXID_366),NULL,
		NULL,NULL,NULL,NULL,NULL,NULL);
	//m_btnTempDriver.SetSize(&siInfo);
	m_btnTempDriver.SetCaption(_T("25.0"));
	m_btnTempDriver.LinkTextGL(pTextGL);
	m_btnTempDriver.SetPos(-300,-70);
	m_btnTempDriver.SetCaptionHeight(42);
	//m_btnTempDriver.SetCaptionCharStyle(STYLE_ART);
	//m_btnTempDriver.EnableDrawBack(FALSE);
	m_btnTempDriver.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnTempDriver.SetCaptionColor(&cr_w);

	m_btnTempCoDriver.Initialize(IDC_VEHICLEINFO_TAG_TEMP_CODRIVER,this,
		m_pSkinManager->GetTexture(TEXID_366),NULL,
		NULL,NULL,NULL,NULL,NULL,NULL);
	//m_btnTempCoDriver.SetSize(&siInfo);
	m_btnTempCoDriver.SetCaption(_T("25.0"));
	m_btnTempCoDriver.LinkTextGL(pTextGL);
	m_btnTempCoDriver.SetPos(300,-70);
	m_btnTempCoDriver.SetCaptionHeight(42);
	//m_btnTempCoDriver.SetCaptionCharStyle(STYLE_ART);
	//m_btnTempCoDriver.EnableDrawBack(FALSE);
	m_btnTempCoDriver.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnTempCoDriver.SetCaptionColor(&cr_w);

	

// 	SIZE si4={130,80};

	m_iconMaxFront.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_318));//test
// 	m_iconBlowDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_310));//test
// 	m_iconBlowHorizontal.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_311));//test
	m_iconBlowUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_313));
	m_iconBlowRear.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_312));
	m_iconSeatHeat0.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_314));
	m_iconSeatHeat1.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_315));
	m_iconSeatHeat2.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_316));
	m_iconSeatHeat3.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_317));
// 	m_iconAutoSmall.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_319));
// 	m_iconAuotoBig.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_320));
	m_iconAQS.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_319));

	//
	m_iconLoopOut.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_321));
	m_iconLoopInner.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_322));

	m_btnMaxFront.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
		m_pSkinManager->GetTexture(TEXID_326),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnMaxFront.SetIcon(&m_iconMaxFront);
// 	m_btnMaxFront.SetSize(&si4);
	m_btnMaxFront.SetPos(-260,-280);
// 	m_btnMaxFront.SetStatus(BS_FOCUS);
	m_btnMaxFront.SetIconPos(0,32);

	m_btnBlowUp.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
		m_pSkinManager->GetTexture(TEXID_326),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBlowUp.SetIcon(&m_iconBlowUp);
// 	m_btnBlowUp.SetSize(&si4);
	m_btnBlowUp.SetPos(-130,-280);
	m_btnBlowUp.SetIconPos(0,32);

	m_btnInnerLoop.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
		m_pSkinManager->GetTexture(TEXID_326),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnInnerLoop.SetIcon(&m_iconLoopOut);
// 	m_btnInnerLoop.SetSize(&si4);
	m_btnInnerLoop.SetPos(0,-280);
	m_btnInnerLoop.SetIconPos(0,32);

	m_btnRear.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
		m_pSkinManager->GetTexture(TEXID_326),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnRear.SetIcon(&m_iconBlowRear);
// 	m_btnRear.SetSize(&si4);
	m_btnRear.SetPos(130,-280);
	m_btnRear.SetIconPos(0,32);

	m_btnAQS.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
		m_pSkinManager->GetTexture(TEXID_326),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAQS.SetIcon(&m_iconAQS);
	// 	m_btnAQS.SetStatus(BS_FOCUS);
	m_btnAQS.SetPos(260,-280);
	m_btnAQS.SetIconPos(0,32);



	//
	m_btnAutoSmall.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_323),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
// 	m_btnAutoSmall.SetIcon(&m_iconAutoSmall);
// 	m_btnAutoSmall.SetSize(&si4);
	m_btnAutoSmall.SetShowScale(0.7);
	m_btnAutoSmall.SetPos(0,-70-46);
	m_btnAutoSmall.Show(FALSE);
// 	m_btnAutoSmall.EnableDrawBack(FALSE);
// 	m_btnAutoSmall.LinkTextGL(pTextGL);
// 	m_btnAutoSmall.SetCaption(_T("LOW"));
// 	m_btnAutoSmall.SetCaptionPos(BTN_CAPTION_TOP);

	m_btnAutoBig.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_323),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
// 	m_btnAutoBig.SetIcon(&m_iconAuotoBig);
// 	m_btnAutoBig.SetSize(&si4);
	m_btnAutoBig.SetShowScale(1.0);
	m_btnAutoBig.SetPos(0,-70+46);
	m_btnAutoBig.Show(FALSE);
// 	m_btnAutoBig.EnableDrawBack(FALSE);
// 	m_btnAutoBig.LinkTextGL(pTextGL);
// 	m_btnAutoBig.SetCaption(_T("HIGH"));
// 	m_btnAutoBig.SetCaptionPos(BTN_CAPTION_TOP);

// 	m_btnACStatus.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
// 		m_pSkinManager->GetTexture(TEXID_326),NULL,
// 		NULL,NULL,
// 		NULL,NULL);
// // 	m_btnACStatus.SetSize(&si4);
// 	m_btnACStatus.SetPos(-130,-115);
// 	m_btnACStatus.LinkTextGL(pTextGL);
// 	m_btnACStatus.SetCaption(_T("AC"));
// 	m_btnACStatus.SetCaptionPos(BTN_CAPTION_TOP);
// // 	m_btnACStatus.EnableDrawBack(FALSE);


	//
	m_btnSeatDriver.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
		m_pSkinManager->GetTexture(TEXID_326),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSeatDriver.SetIcon(&m_iconSeatHeat0);
// 	m_btnSeatDriver.SetSize(&si4);
	m_btnSeatDriver.SetPos(-180,-170);
	m_btnSeatDriver.EnableDrawBack(FALSE);

	m_btnSeatCoDriver.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
		m_pSkinManager->GetTexture(TEXID_326),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSeatCoDriver.SetIcon(&m_iconSeatHeat0);
	// 	m_btnSeatCoDriver.SetSize(&si4);
	m_btnSeatCoDriver.SetPos(180,-170);
	m_btnSeatCoDriver.EnableDrawBack(FALSE);

	m_btnPowerStatus.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_325),NULL,
		m_pSkinManager->GetTexture(TEXID_326),NULL,
		NULL,NULL,
		NULL,NULL);
// 	m_btnPowerStatus.SetIcon(&m_iconMaxFront);
// 	m_btnPowerStatus.SetSize(&si4);
	m_btnPowerStatus.SetPos(0,-50);
	m_btnPowerStatus.LinkTextGL(pTextGL);
	m_btnPowerStatus.SetCaption(_T("OFF"));
	m_btnPowerStatus.SetCaptionPos(BTN_CAPTION_TOP);
// 	m_btnPowerStatus.EnableDrawBack(FALSE);
// 	m_btnPowerStatus.ShowCaption(FALSE);

/*
	m_btnDual.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDual.SetSize(&sibtn);
	m_btnDual.SetPos(-g_iClientWidth/2+120+140,g_iClientHeight/2-40);
	m_btnDual.LinkTextGL(pTextGL);
	m_btnDual.SetCaption(_T("DUAL ON"));
	m_btnDual.SetCaptionPos(BTN_CAPTION_LEFT);
	m_btnDual.EnableDrawBack(FALSE);
	m_btnDual.SetCaptionColor(&cr_black);
	m_btnDual.SetCaptionHeight(28);
// 	m_btnDual.SetNotifyFunc(&CPanelVehicleInfoAC::OnNotify,this);
*/

#if ENABLE_ECON_UI
	m_btnECON.Initialize(IDC_VEHICLEINFO_BTN_ECON,this,m_pSkinManager->GetTexture(TEXID_370),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnECON.SetPos(-280,180);
	m_btnECON.SetShowScale(0.8f);
	m_btnECON.SetNotifyFunc(&CPanelVehicleInfoAC::OnNotify,this);

	COLORGL cr_gray={0.25,0.25,0.25,1.0};

	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=340;
	sli.size.cy=24;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;
	sli.dwType=ST_HOR_NORMAL;

	m_sliECON.Initialize(IDC_VEHICLEINFO_SLI_ECON,this,&sli,pSkinManager);
	m_sliECON.SetRange(0,10);
	m_sliECON.SetValue(/*0*/m_pSkinManager->GetConfig()->GetCurParam()->dwECONLevel);
	m_sliECON.SetPos(0,180);
	m_sliECON.LinkTextGL(pTextGL);
	m_sliECON.SetCaptionColor(&cr_gray);
	m_sliECON.ShowValue(TRUE);
	m_sliECON.EnableMoveNotify(TRUE);
	m_sliECON.SetNotifyFunc(&CPanelVehicleInfoAC::OnNotify,this);
	m_sliECON.Show(FALSE);

	//
	SetTimer(m_hMainWnd,TIMER_ECON_PROC,100,EconTimerProc);

#endif
	

	return TRUE;
}
void CPanelVehicleInfoAC::OnEconProc()
{
	static bool bon=0;
	static DWORD s_cnt_off=0;
	static DWORD s_cnt_on=0;

	if(m_pSkinManager->GetConfig()->GetCurParam()->dwECONLevel > 0)//econ on
	{
		if(!bon)
		{
			s_cnt_off++;
			if(s_cnt_off>=17)
			{
				bon=1;
				s_cnt_on=0;
				if(m_pCommander)
				{
					RETAILMSG(1/*DEBUG_GUI*/,(_T(">>SetECONLevel : %d\r\n"),m_pSkinManager->GetConfig()->GetCurParam()->dwECONLevel ));
					m_pCommander->SendCmd(_ARM_ECON_SET,m_pSkinManager->GetConfig()->GetCurParam()->dwECONLevel );
				}
			}
		}
		else
		{
			s_cnt_on++;
			if(s_cnt_on>=10)
			{
				bon=0;
				s_cnt_off=0;
				if(m_pCommander)
				{
					RETAILMSG(1/*DEBUG_GUI*/,(_T(">>SetECONLevel : %d\r\n"),0));
					m_pCommander->SendCmd(_ARM_ECON_SET,0);
				}
			}
		}
	}
}
void CPanelVehicleInfoAC::EconTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//RETAILMSG(1,(_T("++++EconTimerProc:: hwnd: %d, uMsg: %d,idEvent: %d , dwTime: %d++++\r\n"),hwnd,uMsg,idEvent,dwTime));

	switch (idEvent)
	{
	case TIMER_ECON_PROC:
		if(g_pVehicleInfoAC)
		{
			g_pVehicleInfoAC->OnEconProc();
		}
		break;
	}
}
void CPanelVehicleInfoAC::SetInfoAirStatus(BYTE iStatus)
{
	m_btnPowerStatus.SetStatus((iStatus&0x80)?BS_FOCUS:BS_UP);

	m_btnPowerStatus.SetCaption((iStatus&0x80)?_T("ON"):_T("OFF"));

	//ac
// 	m_btnACStatus.SetStatus((iStatus&0x40)?BS_FOCUS:BS_UP);
	if(!(iStatus&0x40))//ac status on
	{
		//COLORGL cr_blue={0.0f/256.0f,255.0f/256.0f,255.0f/256.0f,1.0f};
		COLORGL cr_w={1,1,1,1.0f};

		m_iconBlowHorizontalLeft.SetLightColor(&cr_w);
		m_iconBlowDownLeft.SetLightColor(&cr_w);
		m_iconBlowHorizontalRight.SetLightColor(&cr_w);
		m_iconBlowDownRight.SetLightColor(&cr_w);
	}
	else
	{
		//COLORGL cr_w={1,1,1,1.0f};
		COLORGL cr_warm={255.0f/256.0f,80.0f/256.0f,0.0f/256.0f,1.0f};
		COLORGL cr_blue={0.0f/256.0f,255.0f/256.0f,255.0f/256.0f,1.0f};

		if(m_iTempDriver<=m_iTempOutside)
		{
			m_iconBlowHorizontalLeft.SetLightColor(&cr_blue);
			m_iconBlowDownLeft.SetLightColor(&cr_blue);
		}
		else
		{
			m_iconBlowHorizontalLeft.SetLightColor(&cr_warm);
			m_iconBlowDownLeft.SetLightColor(&cr_warm);
		}

		if(m_iTempCoDriver<=m_iTempOutside)
		{
			m_iconBlowHorizontalRight.SetLightColor(&cr_blue);
			m_iconBlowDownRight.SetLightColor(&cr_blue);
		}
		else
		{
			m_iconBlowHorizontalRight.SetLightColor(&cr_warm);
			m_iconBlowDownRight.SetLightColor(&cr_warm);
		}
	}
	//

	m_btnInnerLoop.SetStatus((iStatus&0x20)?BS_FOCUS:BS_UP);
	m_btnInnerLoop.SetIcon((iStatus&0x20)?&m_iconLoopInner:&m_iconLoopOut);


	m_btnMaxFront.SetStatus((iStatus&0x02)?BS_FOCUS:BS_UP);
	m_btnRear.SetStatus((iStatus&0x01)?BS_FOCUS:BS_UP);

	//dual:
	//m_btnDual.SetStatus((iStatus&0x04)?BS_FOCUS:BS_UP);
	//m_btnDual.SetCaption((iStatus&0x04)?_T("DUAL ON"):_T("DUAL OFF"));

// 	m_bOnDual=(iStatus&0x04);
// 	if(m_bOnDual)
	//need fixed:
	m_bOnDual=1;//(iStatus&0x04);
	if((m_bOnDual))
	{
		m_btnTempDriver.SetPos(-300,-70,0,TRUE);
		m_btnTempCoDriver.SetPos(300,-70,0,TRUE);

		m_btnTempDriver.SetScale(1.0,1.0,1.0,TRUE);
		m_btnTempCoDriver.SetScale(1.0,1.0,1.0,TRUE);

		//
		for (int i=0;i<7;i++)
		{
			m_tagFanBarLeft[i].SetShowScale(1.0);
			m_tagFanBarLeft[i].SetPos(-300+64*cos((180-30*i)*PI/180),-70+64*sin((180-30*i)*PI/180));

			m_tagFanBarRight[i].Show(TRUE);
			m_tagFanBarRight[i].SetShowScale(1.0);
			m_tagFanBarRight[i].SetPos(300+64*cos((180-30*i)*PI/180),-70+64*sin((180-30*i)*PI/180));
		}
	}
	else
	{
		m_btnTempDriver.SetPos(0,-70,0,TRUE);
		m_btnTempCoDriver.SetPos(0,-70,0,TRUE);

		m_btnTempDriver.SetScale(1.4,1.4,1.0,TRUE);
		m_btnTempCoDriver.SetScale(1.4,1.4,1.0,TRUE);

		//
		for (int i=0;i<7;i++)
		{
			m_tagFanBarLeft[i].SetShowScale(1.4);
			m_tagFanBarLeft[i].SetPos(0+90*cos((180-30*i)*PI/180),-70+90*sin((180-30*i)*PI/180));

			m_tagFanBarRight[i].Show(FALSE);
			m_tagFanBarRight[i].SetShowScale(1.4);
			m_tagFanBarRight[i].SetPos(0+90*cos((180-30*i)*PI/180),-70+90*sin((180-30*i)*PI/180));
		}
	}

	m_bAutoBig = (iStatus&0x10);
	m_bAutoSmall = (iStatus&0x08);

	// 	m_btnAutoBig.SetStatus((iStatus&0x10)?BS_FOCUS:BS_UP);
	// 	m_btnAutoSmall.SetStatus((iStatus&0x08)?BS_FOCUS:BS_UP);

}
void CPanelVehicleInfoAC::SetInfoAirWind(BYTE iStatus)
{
	int iLevel = (iStatus&0x0F);

	m_iWindLevel=iLevel;

	//
	for (int i=0;i<7;i++)
	{
		m_tagFanBarLeft[i].SetStatus(i>=iLevel?BS_UP:BS_FOCUS);
		m_tagFanBarRight[i].SetStatus(i>=iLevel?BS_UP:BS_FOCUS);
	}

	m_btnBlowUp.SetStatus((iStatus&0x80)?BS_FOCUS:BS_UP);
// 	m_btnBlowHorizontal.SetStatus((iStatus&0x40)?BS_FOCUS:BS_UP);
// 	m_btnBlowDown.SetStatus((iStatus&0x20)?BS_FOCUS:BS_UP);
	m_iconBlowHorizontalLeft.Show(iStatus&0x40 && iLevel>0);
	m_iconBlowHorizontalRight.Show(iStatus&0x40 && iLevel>0);

	m_iconBlowDownLeft.Show(iStatus&0x20 && iLevel>0);
	m_iconBlowDownRight.Show(iStatus&0x20 && iLevel>0);

}
void CPanelVehicleInfoAC::SetInfoAirTempDriver(BYTE iStatus)
{
	wchar_t temp[16];
	if(iStatus==0)//low
	{

	}
	else if(iStatus==0x1f)//hih
	{

	}
	else if(iStatus>=0x1 && iStatus<=0x11)
	{
		swprintf_s(temp,16,_T("%.1f"),18.0f+(iStatus-1)*0.5);
		m_btnTempDriver.SetCaption(temp);

		m_iTempDriver = int((18.0f+(iStatus-1)*0.5)*10);
	}
}
void CPanelVehicleInfoAC::SetInfoAirTempCoDriver(BYTE iStatus)
{
	wchar_t temp[16];
	if(iStatus==0)//low
	{

	}
	else if(iStatus==0x1f)//hih
	{

	}
	else if(iStatus>=0x1 && iStatus<=0x11)
	{
		swprintf_s(temp,16,_T("%.1f"),18.0f+(iStatus-1)*0.5);
		m_btnTempCoDriver.SetCaption(temp);

		m_iTempCoDriver = int((18.0f+(iStatus-1)*0.5)*10);
	}
}
void CPanelVehicleInfoAC::SetInfoOutsideTemp(INT iC)
{
	wchar_t temp[16];
	swprintf_s(temp,16,_T("%.1f"),iC*0.1);

	m_iTempOutside = iC;

	m_btnTempOutside.SetCaption(temp);
	m_btnTempOutside.Show(TRUE);
	m_iconDegree3.Show(TRUE);
}
void CPanelVehicleInfoAC::SetInfoAirSeatHeating(BYTE iStatus)
{
	int iLeft,iRight;

	iLeft=((iStatus&0x30)>>4);
	iRight=(iStatus&0x03);

	RETAILMSG(DEBUG_VEHICLEINFO,( (L"iLeft: %d,iRight: %d\r\n"), iLeft,iRight ));

	m_btnAQS.SetStatus((iStatus&0x80)?BS_FOCUS:BS_UP);
// 	m_btnAQS.SetCaption((iStatus&0x80)?_T("AQS ON"):_T("AQS OFF"));


	if(iLeft==0)
		m_btnSeatDriver.SetIcon(&m_iconSeatHeat0);
	else if(iLeft==1)
		m_btnSeatDriver.SetIcon(&m_iconSeatHeat1);
	else if(iLeft==2)
		m_btnSeatDriver.SetIcon(&m_iconSeatHeat2);
	else if(iLeft==3)
		m_btnSeatDriver.SetIcon(&m_iconSeatHeat3);

// 	m_btnSeatDriver.SetStatus(iLeft!=0);

	if(iRight==0)
		m_btnSeatCoDriver.SetIcon(&m_iconSeatHeat0);
	else if(iRight==1)
		m_btnSeatCoDriver.SetIcon(&m_iconSeatHeat1);
	else if(iRight==2)
		m_btnSeatCoDriver.SetIcon(&m_iconSeatHeat2);
	else if(iRight==3)
		m_btnSeatCoDriver.SetIcon(&m_iconSeatHeat3);

// 	m_btnSeatCoDriver.SetStatus(iRight!=0);

}

void CALLBACK CPanelVehicleInfoAC::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelVehicleInfoAC *panel=(CPanelVehicleInfoAC*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelVehicleInfoAC::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_VEHICLEINFO_SLI_ECON:
		{
#if ENABLE_ECON_UI
			SetECONLevel(wPara);
#endif
		}
		break;
	case IDC_VEHICLEINFO_BTN_ECON:
		{
#if ENABLE_ECON_UI
			m_sliECON.Show(!m_sliECON.IsVisible());
#endif
		}
		break;
	}
}
void CPanelVehicleInfoAC::SetECONLevel(int iLevel)
{
// 	if(m_pCommander)
// 	{
// 		RETAILMSG(1/*DEBUG_GUI*/,(_T(">>SetECONLevel : %d\r\n"),iLevel));
// 		m_pCommander->SendCmd(_ARM_ECON_SET,iLevel);
// 	}

	m_pSkinManager->GetConfig()->GetCurParam()->dwECONLevel = iLevel;

	/*
	ENABLE_PORT_INFO BLPwmSetting;

	//RETAILMSG(1, (TEXT("SocBacklightSetLevel, the level is %d\r\n"), dwLevel));

	// initialize the PwmInfo structure
	BLPwmSetting.PwmFlag = PWM_PEGASUS_MODE;
	// se the duty cycle is 50%
	BLPwmSetting.PwmDutyCycle = (PWM_DUTY_CYCLE)(iLevel*10);//g_dwPhyLevelTable[dwLevel];

	// here we use CK1 not I2S
	BLPwmSetting.PwmPort = PWM0;//
	// we need the output as accurate as possible, so set
	// the source as 12m
	BLPwmSetting.PwmSourceClk =SOURCE_XINW; //SOURCE_XIN;//
	// we divide the output with 100 in order to make different levels easier

	BLPwmSetting.PwmOutputFreq = 400;//

	// we don't need the returning precision
	PWM_Configure(IOCTL_PWM_ENABLE_PORT, &BLPwmSetting, sizeof(BLPwmSetting), NULL);
	*/
}