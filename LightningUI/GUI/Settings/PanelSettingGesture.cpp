#include "PanelSettingGesture.h"
#include "../../resource.h"
#include <windev.h>
#include "CVTE_IoCtl.h"
#include "../../Gesture/GestureProxy.h"

#if (CVTE_EN_GESTURE_AIR == 1)
GLfloat g_ObjectPy;
GLfloat g_ObjectPx;
CPanelSettingGesture * g_pCPanelSettingGesture = NULL;

CPanelSettingGesture::CPanelSettingGesture(void)
{
	m_hGestProxy = NULL;
	m_bCursorTracekEnable = FALSE;
	m_nGesContinueCircleCount = 0;
	m_hGestProxy = GestureProxyOpen();
	if(m_hGestProxy == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:CPanelSettingGesture: GestureProxyOpen failed!!!!!!\r\n")));
	}
}
CPanelSettingGesture::~CPanelSettingGesture(void)
{
	if(m_hGestProxy)
	{
		GestureProxyClose(m_hGestProxy);
	}
}

BOOL CPanelSettingGesture::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingGesture::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	if(m_bCursorTracekEnable)
	{
		GestureThreadSwitch(TRUE);
	}
}

void CPanelSettingGesture::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
	if(m_bCursorTracekEnable)
	{
		GestureThreadSwitch(FALSE);
	}
}

void CPanelSettingGesture::Render()
{
	if(!BeginRender())return;

	m_tagTitleOperationDistance.SetAlpha(m_fCurAlpha);
	m_tagTitleOperationDistance.Render();
	m_sliOperationDistance.SetAlpha(m_fCurAlpha);
	m_sliOperationDistance.Render();

	m_tagTitleSensitivityX.SetAlpha(m_fCurAlpha);
	m_tagTitleSensitivityX.Render();
	m_sliSensitivityX.SetAlpha(m_fCurAlpha);
	m_sliSensitivityX.Render();

	m_tagTitleSensitivityY.SetAlpha(m_fCurAlpha);
	m_tagTitleSensitivityY.Render();
	m_sliSensitivityY.SetAlpha(m_fCurAlpha);
	m_sliSensitivityY.Render();

	m_tagTitleSensitivityZ.SetAlpha(m_fCurAlpha);
	m_tagTitleSensitivityZ.Render();
	m_sliSensitivityZ.SetAlpha(m_fCurAlpha);
	m_sliSensitivityZ.Render();

	m_tagTitleRotate.SetAlpha(m_fCurAlpha);
	m_tagTitleRotate.Render();
	m_sliRotate.SetAlpha(m_fCurAlpha);
	m_sliRotate.Render();

	m_tagTitleWaveCountThd.SetAlpha(m_fCurAlpha);
	m_tagTitleWaveCountThd.Render();
	m_sliWaveCountThd.SetAlpha(m_fCurAlpha);
	m_sliWaveCountThd.Render();

	m_tagTitleWaveAngleThd.SetAlpha(m_fCurAlpha);
	m_tagTitleWaveAngleThd.Render();
	m_sliWaveAngleThd.SetAlpha(m_fCurAlpha);
	m_sliWaveAngleThd.Render();

	m_tagTitleRotate.SetAlpha(m_fCurAlpha);
	m_tagTitleRotate.Render();
	m_sliRotate.SetAlpha(m_fCurAlpha);
	m_sliRotate.Render();

	m_tagTitleNoMotion.SetAlpha(m_fCurAlpha);
	m_tagTitleNoMotion.Render();
	m_sliNoMotionThd.SetAlpha(m_fCurAlpha);
	m_sliNoMotionThd.Render();

	m_tagTitleNoObject.SetAlpha(m_fCurAlpha);
	m_tagTitleNoObject.Render();
	m_sliNoObjectThd.SetAlpha(m_fCurAlpha);
	m_sliNoObjectThd.Render();

	m_tagTitleGestureCursorTrack.SetAlpha(m_fCurAlpha);
	m_tagTitleGestureCursorTrack.Render();
	m_sliGestureCursorTrack.SetAlpha(m_fCurAlpha);
	m_sliGestureCursorTrack.Render();

	m_tagBlock.SetAlpha(m_fCurAlpha);
	m_tagBlock.Render();

	m_tagUp.SetAlpha(m_fCurAlpha);
	m_tagUp.Render();

	m_tagDown.SetAlpha(m_fCurAlpha);
	m_tagDown.Render();

	m_tagLeft.SetAlpha(m_fCurAlpha);
	m_tagLeft.Render();

	m_tagRight.SetAlpha(m_fCurAlpha);
	m_tagRight.Render();

	m_tagForward.SetAlpha(m_fCurAlpha);
	m_tagForward.Render();

	m_tagBack.SetAlpha(m_fCurAlpha);
	m_tagBack.Render();

	m_tagCircle.SetAlpha(m_fCurAlpha);
	m_tagCircle.Render();

	COLORGL colorgreen={0.0f,144.0f/256.0f,255.0f/256.0f,1.0f};
	DrawRect(130,g_ObjectPy-LIST_BOX_TAG_HEIGHT,300,200,&colorgreen);

	if(m_bCursorTracekEnable)
	{
		m_tagObject.SetAlpha(m_fCurAlpha);
		m_tagObject.Render();

		SetFrameTime(8);
	}
	else
	{
		SetFrameTime(15);
	}
    EndRender();
}

BOOL CPanelSettingGesture::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);
    //RETAILMSG(1, (TEXT("##############  CPanelSettingGesture::Initialize  ################\r\n ")));
	const int box_size_x=LIST_BOX_ITEM_WIDTH;

	g_pCPanelSettingGesture = this;

	//large size:
	SetSize(g_iClientWidth-box_size_x,g_iClientHeight+LIST_BOX_TAG_HEIGHT+480);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
    COLORGL cr_gray2={0.35,0.35,0.35,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={0.0f/256.0f,144.0f/256.0f,255.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x=12;
	int cx=(g_iClientWidth-box_size_x-margin_x*2)/2;
	//int cy=cx;
    int tag_h = LIST_BOX_TAG_HEIGHT;

    int caption_h = 22;
    int caption2_h = 18;
    int caption3_h = 14;
	int tagDir_w=(g_iClientWidth-box_size_x-margin_x*2)/8;

	GLfloat py=g_iClientHeight/2-50;
	SLIDERSTRUCTURE sli;

	m_tagTitleOperationDistance.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleOperationDistance.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleOperationDistance.SetCaption(_T("Distance"));
	m_tagTitleOperationDistance.LinkTextGL(pTextGL);
	m_tagTitleOperationDistance.SetPos(0,py);
	m_tagTitleOperationDistance.SetCaptionHeight(caption_h);
	m_tagTitleOperationDistance.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleOperationDistance.SetCaptionColor(&cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_sliOperationDistance.Initialize(IDC_GESTURE_DISTANCE,this,&sli,pSkinManager);
	m_sliOperationDistance.SetRange(0,500);
	m_sliOperationDistance.SetValue(0);
	m_sliOperationDistance.SetPos(m_siCurSize.cx/2-144,py);
	m_sliOperationDistance.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliOperationDistance.LinkTextGL(pTextGL);
	m_sliOperationDistance.SetCaptionColor(&cr_gray);
	m_sliOperationDistance.ShowValue(TRUE);
	m_sliOperationDistance.EnableMoveNotify(TRUE);
	m_sliOperationDistance.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleSensitivityX.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleSensitivityX.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleSensitivityX.SetCaption(_T("Sensitivity X"));
	m_tagTitleSensitivityX.LinkTextGL(pTextGL);
	m_tagTitleSensitivityX.SetPos(0,py);
	m_tagTitleSensitivityX.SetCaptionHeight(caption_h);
	m_tagTitleSensitivityX.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleSensitivityX.SetCaptionColor(&cr_gray);

	m_sliSensitivityX.Initialize(IDC_GESTURE_SENSITIVITY_X,this,&sli,pSkinManager);
	m_sliSensitivityX.SetRange(0,30);
	m_sliSensitivityX.SetValue(0);
	m_sliSensitivityX.SetPos(m_siCurSize.cx/2-144,py);
	m_sliSensitivityX.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliSensitivityX.LinkTextGL(pTextGL);
	m_sliSensitivityX.SetCaptionColor(&cr_gray);
	m_sliSensitivityX.ShowValue(TRUE);
	m_sliSensitivityX.EnableMoveNotify(TRUE);
	m_sliSensitivityX.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleSensitivityY.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleSensitivityY.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleSensitivityY.SetCaption(_T("Sensitivity Y"));
	m_tagTitleSensitivityY.LinkTextGL(pTextGL);
	m_tagTitleSensitivityY.SetPos(0,py);
	m_tagTitleSensitivityY.SetCaptionHeight(caption_h);
	m_tagTitleSensitivityY.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleSensitivityY.SetCaptionColor(&cr_gray);

	m_sliSensitivityY.Initialize(IDC_GESTURE_SENSITIVITY_Y,this,&sli,pSkinManager);
	m_sliSensitivityY.SetRange(0,30);
	m_sliSensitivityY.SetValue(0);
	m_sliSensitivityY.SetPos(m_siCurSize.cx/2-144,py);
	m_sliSensitivityY.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliSensitivityY.LinkTextGL(pTextGL);
	m_sliSensitivityY.SetCaptionColor(&cr_gray);
	m_sliSensitivityY.ShowValue(TRUE);
	m_sliSensitivityY.EnableMoveNotify(TRUE);
	m_sliSensitivityY.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleSensitivityZ.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleSensitivityZ.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleSensitivityZ.SetCaption(_T("Sensitivity Z"));
	m_tagTitleSensitivityZ.LinkTextGL(pTextGL);
	m_tagTitleSensitivityZ.SetPos(0,py);
	m_tagTitleSensitivityZ.SetCaptionHeight(caption_h);
	m_tagTitleSensitivityZ.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleSensitivityZ.SetCaptionColor(&cr_gray);

	m_sliSensitivityZ.Initialize(IDC_GESTURE_SENSITIVITY_Z,this,&sli,pSkinManager);
	m_sliSensitivityZ.SetRange(0,30);
	m_sliSensitivityZ.SetValue(0);
	m_sliSensitivityZ.SetPos(m_siCurSize.cx/2-144,py);
	m_sliSensitivityZ.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliSensitivityZ.LinkTextGL(pTextGL);
	m_sliSensitivityZ.SetCaptionColor(&cr_gray);
	m_sliSensitivityZ.ShowValue(TRUE);
	m_sliSensitivityZ.EnableMoveNotify(TRUE);
	m_sliSensitivityZ.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleRotate.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRotate.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleRotate.SetCaption(_T("Rotate"));
	m_tagTitleRotate.LinkTextGL(pTextGL);
	m_tagTitleRotate.SetPos(0,py);
	m_tagTitleRotate.SetCaptionHeight(caption_h);
	m_tagTitleRotate.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleRotate.SetCaptionColor(&cr_gray);

	m_sliRotate.Initialize(IDC_GESTURE_ROTATE,this,&sli,pSkinManager);
	m_sliRotate.SetRange(0,32);
	m_sliRotate.SetValue(0);
	m_sliRotate.SetPos(m_siCurSize.cx/2-144,py);
	m_sliRotate.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliRotate.LinkTextGL(pTextGL);
	m_sliRotate.SetCaptionColor(&cr_gray);
	m_sliRotate.ShowValue(TRUE);
	m_sliRotate.EnableMoveNotify(TRUE);
	m_sliRotate.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleWaveCountThd.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleWaveCountThd.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleWaveCountThd.SetCaption(_T("WaveCount"));
	m_tagTitleWaveCountThd.LinkTextGL(pTextGL);
	m_tagTitleWaveCountThd.SetPos(0,py);
	m_tagTitleWaveCountThd.SetCaptionHeight(caption_h);
	m_tagTitleWaveCountThd.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleWaveCountThd.SetCaptionColor(&cr_gray);

	m_sliWaveCountThd.Initialize(IDC_GESTURE_WAVE_COUNT,this,&sli,pSkinManager);
	m_sliWaveCountThd.SetRange(0,15);
	m_sliWaveCountThd.SetValue(0);
	m_sliWaveCountThd.SetPos(m_siCurSize.cx/2-144,py);
	m_sliWaveCountThd.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliWaveCountThd.LinkTextGL(pTextGL);
	m_sliWaveCountThd.SetCaptionColor(&cr_gray);
	m_sliWaveCountThd.ShowValue(TRUE);
	m_sliWaveCountThd.EnableMoveNotify(TRUE);
	m_sliWaveCountThd.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleWaveAngleThd.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleWaveAngleThd.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleWaveAngleThd.SetCaption(_T("WaveAngle"));
	m_tagTitleWaveAngleThd.LinkTextGL(pTextGL);
	m_tagTitleWaveAngleThd.SetPos(0,py);
	m_tagTitleWaveAngleThd.SetCaptionHeight(caption_h);
	m_tagTitleWaveAngleThd.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleWaveAngleThd.SetCaptionColor(&cr_gray);

	m_sliWaveAngleThd.Initialize(IDC_GESTURE_WAVE_ANGLE,this,&sli,pSkinManager);
	m_sliWaveAngleThd.SetRange(0,15);
	m_sliWaveAngleThd.SetValue(0);
	m_sliWaveAngleThd.SetPos(m_siCurSize.cx/2-144,py);
	m_sliWaveAngleThd.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliWaveAngleThd.LinkTextGL(pTextGL);
	m_sliWaveAngleThd.SetCaptionColor(&cr_gray);
	m_sliWaveAngleThd.ShowValue(TRUE);
	m_sliWaveAngleThd.EnableMoveNotify(TRUE);
	m_sliWaveAngleThd.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleNoMotion.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleNoMotion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleNoMotion.SetCaption(_T("NoMotion"));
	m_tagTitleNoMotion.LinkTextGL(pTextGL);
	m_tagTitleNoMotion.SetPos(0,py);
	m_tagTitleNoMotion.SetCaptionHeight(caption_h);
	m_tagTitleNoMotion.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleNoMotion.SetCaptionColor(&cr_gray);

	m_sliNoMotionThd.Initialize(IDC_GESTURE_NOMOTION,this,&sli,pSkinManager);
	m_sliNoMotionThd.SetRange(0,15);
	m_sliNoMotionThd.SetValue(0);
	m_sliNoMotionThd.SetPos(m_siCurSize.cx/2-144,py);
	m_sliNoMotionThd.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliNoMotionThd.LinkTextGL(pTextGL);
	m_sliNoMotionThd.SetCaptionColor(&cr_gray);
	m_sliNoMotionThd.ShowValue(TRUE);
	m_sliNoMotionThd.EnableMoveNotify(TRUE);
	m_sliNoMotionThd.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleNoObject.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleNoObject.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleNoObject.SetCaption(_T("NoObject"));
	m_tagTitleNoObject.LinkTextGL(pTextGL);
	m_tagTitleNoObject.SetPos(0,py);
	m_tagTitleNoObject.SetCaptionHeight(caption_h);
	m_tagTitleNoObject.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleNoObject.SetCaptionColor(&cr_gray);

	m_sliNoObjectThd.Initialize(IDC_GESTURE_NOOBJECT,this,&sli,pSkinManager);
	m_sliNoObjectThd.SetRange(0,15);
	m_sliNoObjectThd.SetValue(0);
	m_sliNoObjectThd.SetPos(m_siCurSize.cx/2-144,py);
	m_sliNoObjectThd.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliNoObjectThd.LinkTextGL(pTextGL);
	m_sliNoObjectThd.SetCaptionColor(&cr_gray);
	m_sliNoObjectThd.ShowValue(TRUE);
	m_sliNoObjectThd.EnableMoveNotify(TRUE);
	m_sliNoObjectThd.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	//Gesture Cursor track
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_BOOL_BUTTON_WIDTH;
	sli.size.cy=SLI_BOOL_BUTTON_HEIGHT;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_642;
	sli.idChannelTex=TEXID_641;
	sli.idThumbTex=TEXID_640;
	sli.bSwitchMode=TRUE;

	m_tagTitleGestureCursorTrack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleGestureCursorTrack.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleGestureCursorTrack.SetCaption(_T("Cursor Track"));
	m_tagTitleGestureCursorTrack.LinkTextGL(pTextGL);
	m_tagTitleGestureCursorTrack.SetPos(0,py);
	m_tagTitleGestureCursorTrack.SetCaptionHeight(caption_h);
	m_tagTitleGestureCursorTrack.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleGestureCursorTrack.SetCaptionColor(&cr_gray);

	m_sliGestureCursorTrack.Initialize(IDC_GESTURE_CURSOR_TRACK,this,&sli,pSkinManager);
	m_sliGestureCursorTrack.SetRange(0,10);
	m_sliGestureCursorTrack.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect?10:0);
	m_sliGestureCursorTrack.SetPos(m_siCurSize.cx/2-70,py);
	m_sliGestureCursorTrack.SetNotifyFunc(&CPanelSettingGesture::OnNotify,this);
	m_sliGestureCursorTrack.LinkTextGL(pTextGL);
	m_sliGestureCursorTrack.SetCaptionColor(&cr_gray);
	m_sliGestureCursorTrack.ShowValue(FALSE);

	py-=tag_h;

    m_tagBlock.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagBlock.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h*3);
    m_tagBlock.SetPos(0,py-tag_h);

	py-=tag_h;

	m_tagUp.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_807),NULL,m_pSkinManager->GetTexture(TEXID_808),NULL,m_pSkinManager->GetTexture(TEXID_808),NULL,NULL,NULL);
	m_tagUp.SetPos(-tagDir_w*2-40,py-10);
	VIEW_STATE vs = m_tagDown.GetCurViewState();
	vs.fRotateZ -= 90;
	m_tagUp.SetCurViewState(&vs);
	m_tagUp.SetShowScale(2.0f);

	py-=70;

	m_tagLeft.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_807),NULL,m_pSkinManager->GetTexture(TEXID_808),NULL,m_pSkinManager->GetTexture(TEXID_808),NULL,NULL,NULL);
	m_tagLeft.SetPos(-tagDir_w*3-30,py);
	m_tagLeft.SetShowScale(2.0f);

	m_tagDown.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_807),NULL,m_pSkinManager->GetTexture(TEXID_808),NULL,m_pSkinManager->GetTexture(TEXID_808),NULL,NULL,NULL);
	m_tagDown.SetPos(-tagDir_w*2-40,py);
	vs = m_tagDown.GetCurViewState();
	vs.fRotateZ += 90;
	m_tagDown.SetCurViewState(&vs);
	m_tagDown.SetShowScale(2.0f);

	m_tagRight.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_809),NULL,m_pSkinManager->GetTexture(TEXID_810),NULL,m_pSkinManager->GetTexture(TEXID_810),NULL,NULL,NULL);
	m_tagRight.SetPos(-tagDir_w-50,py);
	m_tagRight.SetShowScale(2.0f);

	m_tagForward.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_819),NULL,m_pSkinManager->GetTexture(TEXID_820),NULL,m_pSkinManager->GetTexture(TEXID_820),NULL,NULL,NULL);
	m_tagForward.SetPos(-50,py+tag_h+tag_h/2);
	vs = m_tagForward.GetCurViewState();
	vs.fRotateZ += 44;
	m_tagForward.SetCurViewState(&vs);
	m_tagForward.SetShowScale(1.6f);

	m_tagBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_819),NULL,m_pSkinManager->GetTexture(TEXID_820),NULL,m_pSkinManager->GetTexture(TEXID_820),NULL,NULL,NULL);
	m_tagBack.SetPos(-50,py+tag_h/2);
	vs = m_tagBack.GetCurViewState();
	vs.fRotateZ += 224;
	m_tagBack.SetCurViewState(&vs);
	m_tagBack.SetShowScale(1.6f);

	m_tagCircle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_828),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagCircle.SetPos(-tagDir_w*2-40,py+tag_h*2-20);
	m_tagCircle.SetShowScale(1.5f);

	g_ObjectPy = py+tag_h*2;
	g_ObjectPx = 15;

	m_tagObject.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_020),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagObject.SetPos(g_ObjectPx,g_ObjectPy);
	m_tagObject.SetShowScale(0.10f);

	if(m_hGestProxy)
	{
		GestureProxyGetOperationDistance(m_hGestProxy, &m_nGesDistanceThd);
		GestureProxyGetOperationSensitivity(m_hGestProxy, &m_GesDirectionThd);
		GestureProxyGetOperationWave(m_hGestProxy, &m_GesWaveThd);
		GestureProxyGetOperationRotate(m_hGestProxy, &m_nGesRotateThd);
		GestureProxyGetOperationNoMotion(m_hGestProxy, &m_nNoMotionThd);
		GestureProxyGetOperationNoObject(m_hGestProxy, &m_nNoObjectThd);

		m_sliOperationDistance.SetValue(m_nGesDistanceThd);
		m_sliSensitivityX.SetValue(m_GesDirectionThd.nDataX);
		m_sliSensitivityY.SetValue(m_GesDirectionThd.nDataY);
		m_sliSensitivityZ.SetValue(m_GesDirectionThd.nDataZ);
		m_sliWaveCountThd.SetValue(m_GesWaveThd.nWaveCount);
		m_sliWaveAngleThd.SetValue(m_GesWaveThd.nWaveAngle);
		m_sliRotate.SetValue(m_nGesRotateThd);
		m_sliNoMotionThd.SetValue(m_nNoMotionThd);
		m_sliNoObjectThd.SetValue(m_nNoObjectThd);

		//GestureProxySetOperationDistance(m_hGestProxy, 0x024E);
	}

	//InitGestureProcThread();

	return TRUE;
}


void CALLBACK CPanelSettingGesture::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingGesture *panel=(CPanelSettingGesture*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingGesture::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
		case IDC_GESTURE_DISTANCE:
			RETAILMSG(1, (TEXT("MSG:[%s]: %04X\r\n"), TEXT(__FUNCTION__),wPara));
			UpdateDistance((UINT16)(wPara));
			break;
		case IDC_GESTURE_SENSITIVITY_X:
			m_GesDirectionThd.nDataX = (BYTE)wPara;
			UpdateSensitivity(&m_GesDirectionThd);
			break;
		case IDC_GESTURE_SENSITIVITY_Y:
			m_GesDirectionThd.nDataY = (BYTE)wPara;
			UpdateSensitivity(&m_GesDirectionThd);
			break;
		case IDC_GESTURE_SENSITIVITY_Z:
			m_GesDirectionThd.nDataZ = (BYTE)wPara;
			UpdateSensitivity(&m_GesDirectionThd);
			break;
		case IDC_GESTURE_WAVE_COUNT:
			m_GesWaveThd.nWaveCount = (BYTE)wPara;
			UpdateWave(&m_GesWaveThd);
			break;
		case IDC_GESTURE_WAVE_ANGLE:
			m_GesWaveThd.nWaveAngle = (BYTE)wPara;
			UpdateWave(&m_GesWaveThd);
			break;
		case IDC_GESTURE_ROTATE:
			UpdateRotate((UINT16)wPara);
			break;
		case IDC_GESTURE_NOMOTION:
			UpdateNoMotion((BYTE)wPara);
			break;
		case IDC_GESTURE_NOOBJECT:
			UpdateNoObject((BYTE)wPara);
			break;
		case IDC_GESTURE_CURSOR_TRACK:
			m_bCursorTracekEnable = (wPara?TRUE:FALSE);
			if(m_bCursorTracekEnable)
			{
				GestureThreadSwitch(TRUE);
			}
			else
			{
				GestureThreadSwitch(FALSE);
			}
			break;
		default:
			break;
	}

	PlaySoundEffect(0);
}

void CPanelSettingGesture::OnGestureCircle(BOOL bCircleRight)
{
	//RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureCircle: bCircleRight[%d]\r\n"),bCircleRight));
	if(bCircleRight)
	{
		VIEW_STATE vs = m_tagCircle.GetCurViewState();
		vs.fRotateZ -= 10;
		m_tagCircle.SetCurViewState(&vs);
		m_nGesContinueCircleCount = 0;
	}
	else
	{
		VIEW_STATE vs = m_tagCircle.GetCurViewState();
		vs.fRotateZ += 10;
		m_tagCircle.SetCurViewState(&vs);
		m_nGesContinueCircleCount = 0;
	}
	PlaySoundEffect(0);
}

void CPanelSettingGesture::OnGestureID(WPARAM wParam, LPARAM lParam)
{
	PPACGestureData_t pGestureData = (PPACGestureData_t)lParam;
 	switch(pGestureData->nGesture)
	{
		case GES_RIGHT_FLAG:
			//RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID: Right move\r\n")));
			m_tagRight.SetStatus(BS_FOCUS);
			SetTimer(m_hMainWnd,TIMER_SETTING_GESTURE_RIGHT,250,OnTimerProc);
			break;
		case GES_LEFT_FLAG:
			//RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID: Left move\r\n")));
			m_tagLeft.SetStatus(BS_FOCUS);
			PlaySoundEffect(0);
			SetTimer(m_hMainWnd,TIMER_SETTING_GESTURE_LEFT,250,OnTimerProc);
			break;
		case GES_UP_FLAG:
			//RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID: Up move\r\n")));
			m_tagUp.SetStatus(BS_FOCUS);
			PlaySoundEffect(0);
			SetTimer(m_hMainWnd,TIMER_SETTING_GESTURE_UP,250,OnTimerProc);
			break;
		case GES_DOWN_FLAG:
			//RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID: Down move\r\n")));
			m_tagDown.SetStatus(BS_FOCUS);
			PlaySoundEffect(0);
			SetTimer(m_hMainWnd,TIMER_SETTING_GESTURE_DOWN,250,OnTimerProc);
			break;
        case GES_FORWARD_FLAG:
            //RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID: Forward move\r\n")));
			m_tagForward.SetStatus(BS_FOCUS);
			PlaySoundEffect(0);
			SetTimer(m_hMainWnd,TIMER_SETTING_GESTURE_FORWARD,250,OnTimerProc);
            break;
        case GES_BACKWARD_FLAG:
            //RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID: Backward move\r\n")));
			m_tagBack.SetStatus(BS_FOCUS);
			PlaySoundEffect(0);
			SetTimer(m_hMainWnd,TIMER_SETTING_GESTURE_BACK,250,OnTimerProc);
            break;
        case GES_CLOCKWISE_FLAG:
            //RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID: ClockWise move\r\n")));
            break;
        case GES_COUNT_CLOCKWISE_FLAG:
            //RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID: Count ClockWise move\r\n")));
            break;
		default:
            //RETAILMSG(1, (TEXT("MSG:CPanelSettingGesture::OnGestureID:nGesture[%X]\r\n"), pGestureData->nGesture));
			break;
	}
}

BOOL CPanelSettingGesture::UpdateDistance(UINT16 nDistance)
{
	if(m_hGestProxy == NULL)
	{
		return FALSE;
	}
	m_nGesDistanceThd = nDistance;

	RETAILMSG(1, (TEXT("MSG:[%s]: m_nGesDistanceThd[%04X]\r\n"), TEXT(__FUNCTION__),m_nGesDistanceThd));

	return GestureProxySetOperationDistance(m_hGestProxy, m_nGesDistanceThd);
}

BOOL CPanelSettingGesture::UpdateSensitivity(GesDirection_t* pGesDirection)
{
	if(m_hGestProxy == NULL)
	{
		return FALSE;
	}
	m_GesDirectionThd.nDataX = pGesDirection->nDataX;
	m_GesDirectionThd.nDataY = pGesDirection->nDataY;
	m_GesDirectionThd.nDataZ = pGesDirection->nDataZ;


	return GestureProxySetOperationSensitivity(m_hGestProxy, &m_GesDirectionThd);
}

BOOL CPanelSettingGesture::UpdateWave(GesWave_t* pGesWave)
{
	if(m_hGestProxy == NULL)
	{
		return FALSE;
	}
	m_GesWaveThd.nWaveCount = pGesWave->nWaveCount;
	m_GesWaveThd.nWaveAngle = pGesWave->nWaveAngle;

	return GestureProxySetOperationWave(m_hGestProxy, &m_GesWaveThd);
}

BOOL CPanelSettingGesture::UpdateRotate(UINT16 nRotate)
{
	if(m_hGestProxy == NULL)
	{
		return FALSE;
	}
	m_nGesRotateThd = nRotate;
	if(!GestureProxySetOperationRotate(m_hGestProxy, m_nGesRotateThd))
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: failed!!! \r\n"), TEXT(__FUNCTION__)));
		return FALSE;
	}
	RETAILMSG(1, (TEXT("MSG:[%s]: m_nGesRotateThd[%d] \r\n"), TEXT(__FUNCTION__), m_nGesRotateThd));

	CGestureProxy::GetInstance()->ChangeRotateThd(m_nGesRotateThd);
	return TRUE;
}

BOOL CPanelSettingGesture::UpdateNoMotion(BYTE nNoMotion)
{
	if(m_hGestProxy == NULL)
	{
		return FALSE;
	}
	m_nNoMotionThd = nNoMotion;
	RETAILMSG(1, (TEXT("MSG:[%s]: m_nNoMotionThd[%d] \r\n"), TEXT(__FUNCTION__), m_nNoMotionThd));
	return GestureProxySetOperationNoMotion(m_hGestProxy, m_nNoMotionThd);
}

BOOL CPanelSettingGesture::UpdateNoObject(BYTE nNoObject)
{
	if(m_hGestProxy == NULL)
	{
		return FALSE;
	}
	m_nNoObjectThd = nNoObject;
	RETAILMSG(1, (TEXT("MSG:[%s]: m_nNoObjectThd[%d] \r\n"), TEXT(__FUNCTION__), m_nNoObjectThd));
	return GestureProxySetOperationNoObject(m_hGestProxy, m_nNoObjectThd);
}

void CPanelSettingGesture::DeinitGestureProcThread()
{
    m_bGestureThreadEnd = TRUE;
    m_bGestureThreadIsRunning = FALSE;
    if(m_hGestureEvent)
    {
        SetEvent(m_hGestureEvent);
        WaitForSingleObject(m_hGestureProcThread,INFINITE);
        CloseHandle(m_hGestureEvent);
        m_hGestureEvent = NULL;
    }

    if(m_hGestureProcThread)
    {
        CloseHandle(m_hGestureProcThread);
        m_hGestureProcThread = NULL;
    }
}

BOOL CPanelSettingGesture::InitGestureProcThread()
{
    m_dwGestureDetectTimeout = 8;
    m_bGestureThreadEnd = FALSE;
    m_bGestureThreadIsRunning = FALSE;

    if(m_hGestureEvent == NULL)
    {
        m_hGestureEvent = CreateEvent(0, FALSE, FALSE, NULL);
        if(m_hGestureEvent == NULL)
        {
            RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent [m_hGestureEvent] failed!!!\r\n"), TEXT(__FUNCTION__)));
            return FALSE;
        }
    }

    if(m_hGestureProcThread == NULL)
    {
    	m_hGestureProcThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GestureProcThread, this, 0, NULL);
        if(m_hGestureProcThread == NULL)
        {
            RETAILMSG(1, (TEXT("ERR:[%s]: CreateThread [m_hGestureProcThread] failed!!!\r\n"), TEXT(__FUNCTION__)));
            return FALSE;
        }
        CeSetThreadPriority(m_hGestureProcThread, 230);

    }
}

void CPanelSettingGesture::GestureThreadSwitch(BOOL bOn)
{
    if(bOn)
    {
        m_dwGestureDetectTimeout = 8;
        m_bGestureThreadIsRunning = TRUE;
    }
    else
    {
        m_dwGestureDetectTimeout = INFINITE;
        m_bGestureThreadIsRunning = FALSE;
    }

    if(m_hGestureEvent)
    {
        RETAILMSG(1, (TEXT("MSG:[%s]: bOn[%d]\r\n"), TEXT(__FUNCTION__), bOn));
        SetEvent(m_hGestureEvent);
    }
}

void CPanelSettingGesture::GestureDetect()
{
	GestureProxyGetObjectInfo(m_hGestProxy,  &m_GesObject);
	//GestureProxyGetOperationRotate(m_hGestProxy, &m_nGesRotate);
	if(abs(m_GesObjectPrev.dwPositionX-m_GesObject.dwPositionX)>40 || abs(m_GesObjectPrev.dwPositionY-m_GesObject.dwPositionY)>40)
	{
		memcpy(&m_GesObjectPrev, &m_GesObject, sizeof(m_GesObject));

		m_tagObject.SetPos(g_ObjectPx+m_GesObject.dwPositionX, g_ObjectPy-m_GesObject.dwPositionY/2);
		//RETAILMSG(1, (TEXT("MSG:XY[%d,%d] Size[%d] AvgY[%d]\r\n"),m_GesObject.dwPositionX,m_GesObject.dwPositionY,m_GesObject.nSize,m_GesObject.nAvgY));
	}
//		if(m_nGesRotate == m_nGesRotatePrev || m_nGesRotate == 0)
//		{
//			return;
//		}
//
//		if(m_nGesRotate<m_nGesRotatePrev)
//		{
//			if((m_nGesRotate-m_nGesRotatePrev)<(-m_nGesRotateThd))
//			{
//				RETAILMSG(1, (TEXT("Circle right\r\n")));
//				if(m_bGesClockCircle)
//				{
//					m_nGesContinueCircleCount++;
//				}
//				else
//				{
//					m_nGesContinueCircleCount = 0;
//				}
//
//				if(m_nGesContinueCircleCount)
//				{
//					VIEW_STATE vs = m_tagCircle.GetCurViewState();
//					vs.fRotateZ -= 10;
//					m_tagCircle.SetCurViewState(&vs);
//					m_nGesContinueCircleCount = 0;
//				}
//				m_nGesRotatePrev = m_nGesRotate;
//				m_bGesClockCircle = TRUE;
//			}
//		}
//		else
//		{
//			if((m_nGesRotate-m_nGesRotatePrev)>m_nGesRotateThd)
//			{
//				RETAILMSG(1, (TEXT("Circle left\r\n")));
//				if(!m_bGesClockCircle)
//				{
//					m_nGesContinueCircleCount++;
//				}
//				else
//				{
//					m_nGesContinueCircleCount = 0;
//				}
//
//				if(m_nGesContinueCircleCount)
//				{
//					VIEW_STATE vs = m_tagCircle.GetCurViewState();
//					vs.fRotateZ += 10;
//					m_tagCircle.SetCurViewState(&vs);
//					m_nGesContinueCircleCount = 0;
//				}
//
//				m_nGesRotatePrev = m_nGesRotate;
//				m_bGesClockCircle = FALSE;
//			}
//		}
}

DWORD WINAPI CPanelSettingGesture::GestureProcThread(LPVOID lpvParam)
{
    DWORD dwRet;
    CPanelSettingGesture *pDevice = (CPanelSettingGesture*)lpvParam;

    RETAILMSG(1, (TEXT("MSG:TOUCH:[%s]: ++\r\n"), TEXT(__FUNCTION__)));

    WaitForSingleObject(pDevice->m_hGestureEvent, INFINITE);
    RETAILMSG(1, (TEXT("MSG:TOUCH:[%s]: WATCHDOG START\r\n"), TEXT(__FUNCTION__)));

    while(!pDevice->m_bGestureThreadEnd)
    {
        dwRet = WaitForSingleObject(pDevice->m_hGestureEvent, pDevice->m_dwGestureDetectTimeout);
        if(dwRet == WAIT_OBJECT_0)
        {
            if(pDevice->m_bGestureThreadEnd)
            {
                RETAILMSG(1, (TEXT("MSG:[%s]: Gesture Thread END \r\n"), TEXT(__FUNCTION__)));
                break;
            }
            if(pDevice->m_bGestureThreadIsRunning)
            {
                RETAILMSG(1, (TEXT("MSG:[%s]: Gesture Thread PLAY \r\n"), TEXT(__FUNCTION__)));
				continue;
            }
            else
            {
                RETAILMSG(1, (TEXT("MSG:[%s]: Gesture Thread STOP \r\n"), TEXT(__FUNCTION__)));
                continue;
            }
        }

        if(pDevice->m_bGestureThreadIsRunning)
        {
            pDevice->GestureDetect();
        }
    }

    RETAILMSG(1, (TEXT("MSG:[%s]: --\r\n"), TEXT(__FUNCTION__)));

	return 0;
}

void CPanelSettingGesture::OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    switch(idEvent)
    {
    case TIMER_SETTING_GESTURE_LEFT:
		g_pCPanelSettingGesture->m_tagLeft.SetStatus(BS_UP);
		break;
	case TIMER_SETTING_GESTURE_RIGHT:
		g_pCPanelSettingGesture->m_tagRight.SetStatus(BS_UP);
		break;
	case TIMER_SETTING_GESTURE_UP:
		g_pCPanelSettingGesture->m_tagUp.SetStatus(BS_UP);
		break;
	case TIMER_SETTING_GESTURE_DOWN:
		g_pCPanelSettingGesture->m_tagDown.SetStatus(BS_UP);
		break;
	case TIMER_SETTING_GESTURE_FORWARD:
		g_pCPanelSettingGesture->m_tagForward.SetStatus(BS_UP);
		break;
	case TIMER_SETTING_GESTURE_BACK:
		g_pCPanelSettingGesture->m_tagBack.SetStatus(BS_UP);
		break;
	default:
		break;
    }
	KillTimer(g_pCPanelSettingGesture->m_hMainWnd,idEvent);
}

#endif
