#include "PanelSettingRadio.h"
#include "../../resource.h"
#include "../../DAB/Inc/DABRadioLib.h"
#include "../../DAB/Adapter/DABAdapter.h"

#define DEFAULT_FM_LEVEL		23
#define DEFAULT_AM_LEVEL		45
//#define DEFAULT_AF_LEVEL		70

COLORGL cr_w={1.0,1.0,1.0,1.0};
COLORGL cr_gray={0.25,0.25,0.25,1.0};
COLORGL cr_gray2={0.35,0.35,0.35,1.0};
COLORGL cr_dark={0.0,0.0,0.0,1.0};
COLORGL cr_green={0.0f/256.0f,144.0f/256.0f,255.0f/256.0f,1.0};
COLORGL cr_b={0.0,161.0f/256.0f,228.0/256.0f,1.0};
const   int box_size_x=LIST_BOX_ITEM_WIDTH;
const   int margin_x=12;
const   int tag_h = LIST_BOX_TAG_HEIGHT;
const	int cx=(g_iClientWidth-box_size_x-margin_x*2)/2;
GLfloat py=g_iClientHeight/2-40;
GLfloat g_pyRadioRegionTitle;
GLfloat g_pyRDSTitle;
GLfloat g_pyDABTitle;
int		g_iPanelHeight;
CPanelSettingRadio::CPanelSettingRadio(void)
{
    m_bRegionSpread = FALSE;
}
CPanelSettingRadio::~CPanelSettingRadio(void)
{

}

BOOL CPanelSettingRadio::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingRadio::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
	UpdateRegionDisplay(m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion);
}

void CPanelSettingRadio::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingRadio::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif

	//stop level:
	m_tagTitleStopLevel.SetAlpha(m_fCurAlpha);
	m_tagTitleStopLevel.Render();

	m_tagTitleStopLevelFM.SetAlpha(m_fCurAlpha);
	m_tagTitleStopLevelFM.Render();
	m_sliStopLevelFM.SetAlpha(m_fCurAlpha);
	m_sliStopLevelFM.Render();

	m_tagTitleStopLevelAM.SetAlpha(m_fCurAlpha);
	m_tagTitleStopLevelAM.Render();
	m_sliStopLevelAM.SetAlpha(m_fCurAlpha);
	m_sliStopLevelAM.Render();

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagTitleStopLevelAF.SetAlpha(m_fCurAlpha);
	m_tagTitleStopLevelAF.Render();
	m_sliStopLevelAF.SetAlpha(m_fCurAlpha);
	m_sliStopLevelAF.Render();
#endif

	m_btnResetStopLevel.SetAlpha(m_fCurAlpha);
	m_btnResetStopLevel.Render();

	//region:
	m_tagTitleRegion.SetAlpha(m_fCurAlpha);
	m_tagTitleRegion.Render();

    m_btnRadioRegion.SetAlpha(m_fCurAlpha);
	m_btnRadioRegion.Render();

//		m_iconSelected.SetAlpha(m_fCurAlpha);
//		m_iconSelected.Render();

	//rds
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	m_tagTitleRDS.SetAlpha(m_fCurAlpha);
	m_tagTitleRDS.Render();
	m_tagTitleRDSSwitch.SetAlpha(m_fCurAlpha);
	m_tagTitleRDSSwitch.Render();
	m_sliRDSSwitch.SetAlpha(m_fCurAlpha);
	m_sliRDSSwitch.Render();

	m_tagTitleAF.SetAlpha(m_fCurAlpha);
	m_tagTitleAF.Render();
	m_sliAF.SetAlpha(m_fCurAlpha);
	m_sliAF.Render();

	m_tagTitleTA.SetAlpha(m_fCurAlpha);
	m_tagTitleTA.Render();
	m_sliTA.SetAlpha(m_fCurAlpha);
	m_sliTA.Render();

// 	m_btnPTY.SetAlpha(m_fCurAlpha);
// 	m_btnPTY.Render();
// 	m_sliPTY.SetAlpha(m_fCurAlpha);
// 	m_sliPTY.Render();
#endif


#if CVTE_EN_DAB
	m_tagTitleDAB.SetAlpha(m_fCurAlpha);
	m_tagTitleDAB.Render();

	m_tagTitleServiceLink.SetAlpha(m_fCurAlpha);
	m_tagTitleServiceLink.Render();
	m_sliServiceLink.SetAlpha(m_fCurAlpha);
	m_sliServiceLink.Render();

	m_tagDABTitleTA.SetAlpha(m_fCurAlpha);
	m_tagDABTitleTA.Render();
	m_sliDABTA.SetAlpha(m_fCurAlpha);
	m_sliDABTA.Render();

	m_tagTitleLBand.SetAlpha(m_fCurAlpha);
	m_tagTitleLBand.Render();
	m_sliLBand.SetAlpha(m_fCurAlpha);
	m_sliLBand.Render();

	m_tagDABAntennaPower.SetAlpha(m_fCurAlpha);
	m_tagDABAntennaPower.Render();
	m_sliDABAntennaPower.SetAlpha(m_fCurAlpha);
	m_sliDABAntennaPower.Render();
#endif

	EndRender();
}

BOOL CPanelSettingRadio::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//large size:

	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);


	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	SLIDERSTRUCTURE sli;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};


	int margin_x=12;
	int margin_y = 6;
	int cx = g_iClientWidth-LIST_BOX_ITEM_WIDTH;
	int caption_h = 22;
	int caption2_h = 22;
	int value_Font = 20;
	int gridHeight = 80;
	int margin_left = 40;
	int space = 80;
	int tag_h = space;
	int captionMargin = 22;

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(cx, g_iClientHeight*4);
	m_tagBk.SetPos(0, -g_iClientHeight);

	//stop level:
	m_tagTitleStopLevel.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleStopLevel.SetSize(cx,gridHeight);
	m_tagTitleStopLevel.SetCaption(GetResourceString(IDS_SETTING_RADIO_SEEK_QUALITY));
	m_tagTitleStopLevel.LinkTextGL(pTextGL);
	m_tagTitleStopLevel.SetPos(0,py);
	m_tagTitleStopLevel.SetCaptionHeight(caption_h);
	m_tagTitleStopLevel.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleStopLevel.SetCaptionColor(&cr_gray, &cr_gray);

	py-=tag_h;

	m_tagTitleStopLevelFM.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleStopLevelFM.SetSize(cx - margin_left,tag_h);
	m_tagTitleStopLevelFM.SetCaption(GetResourceString(IDS_SETTING_RADIO_FM));
	m_tagTitleStopLevelFM.LinkTextGL(pTextGL);
	m_tagTitleStopLevelFM.SetPos(margin_x/2, py);
	m_tagTitleStopLevelFM.SetCaptionHeight(caption_h);
	m_tagTitleStopLevelFM.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleStopLevelFM.SetCaptionColor(&cr_gray, &cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;
	sli.dwType=ST_HOR_MIDDLE;

	m_sliStopLevelFM.Initialize(IDC_RADIO_SLI_FM_STOP_LEVEL,this,&sli,pSkinManager);
	m_sliStopLevelFM.SetRange(-5,5);
	m_sliStopLevelFM.SetValue(0);
	m_sliStopLevelFM.SetPos(m_siCurSize.cx/2-126,py);
	m_sliStopLevelFM.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	m_sliStopLevelFM.LinkTextGL(pTextGL);
	m_sliStopLevelFM.SetCaptionColor(&cr_dark);
	m_sliStopLevelFM.ShowValue(TRUE);
	m_sliStopLevelFM.EnableMoveNotify(TRUE);
	m_sliStopLevelFM.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleStopLevelAM.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleStopLevelAM.SetSize(cx - margin_left, tag_h);
	m_tagTitleStopLevelAM.SetCaption(GetResourceString(IDS_SETTING_RADIO_AM));
	m_tagTitleStopLevelAM.LinkTextGL(pTextGL);
	m_tagTitleStopLevelAM.SetPos(margin_x/2, py);
	m_tagTitleStopLevelAM.SetCaptionHeight(caption_h);
	m_tagTitleStopLevelAM.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleStopLevelAM.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliStopLevelAM.Initialize(IDC_RADIO_SLI_AM_STOP_LEVEL,this,&sli,pSkinManager);
	m_sliStopLevelAM.SetRange(-5,5);
	m_sliStopLevelAM.SetValue(0);
	m_sliStopLevelAM.SetPos(m_siCurSize.cx/2-126,py);
	m_sliStopLevelAM.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	m_sliStopLevelAM.LinkTextGL(pTextGL);
	m_sliStopLevelAM.SetCaptionColor(&cr_dark);
	m_sliStopLevelAM.ShowValue(TRUE);
	m_sliStopLevelAM.EnableMoveNotify(TRUE);
	m_sliStopLevelAM.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_btnResetStopLevel.Initialize(IDC_RADIO_BTN_RESET_STOP_LEVEL,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnResetStopLevel.SetSize(cx - margin_left, tag_h);
	m_btnResetStopLevel.SetCaption(GetResourceString(IDS_SETTING_RADIO_RESET_SEEK_QUALITY));
	m_btnResetStopLevel.LinkTextGL(pTextGL);
	m_btnResetStopLevel.SetPos(margin_x/2,py);
	m_btnResetStopLevel.SetCaptionHeight(caption_h);
	m_btnResetStopLevel.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnResetStopLevel.SetCaptionColor(&cr_gray, &cr_gray);
	m_btnResetStopLevel.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	py-=tag_h;

	//Region:
	g_pyRadioRegionTitle = py;

	m_tagTitleRegion.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleRegion.SetSize(cx, gridHeight);
	m_tagTitleRegion.SetCaption(GetResourceString(IDS_SETTING_RADIO_REGION));
	m_tagTitleRegion.LinkTextGL(pTextGL);
	m_tagTitleRegion.SetPos(0,py);
	m_tagTitleRegion.SetCaptionHeight(caption_h);
	m_tagTitleRegion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleRegion.SetCaptionColor(&cr_gray, &cr_gray);

	py-=tag_h;

	//
	m_btnRadioRegion.Initialize(IDC_RADIO_BTN_REGION,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnRadioRegion.SetSize(cx - margin_left,tag_h);
	m_btnRadioRegion.SetPos(margin_x/2,py);
	m_btnRadioRegion.LinkTextGL(pTextGL);
	m_btnRadioRegion.SetCaptionHeight(caption_h);
	m_btnRadioRegion.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnRadioRegion.SetCaptionColor(&cr_gray);
	m_btnRadioRegion.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	UpdateRegionDisplay(m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion);

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	py-=tag_h;
	g_pyRDSTitle = py;

	//rds:
	m_tagTitleRDS.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_289),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRDS.SetSize(cx, gridHeight);
#if (CVTE_EN_RBDS)
	m_tagTitleRDS.SetCaption(_T("RBDS"));
#else
	m_tagTitleRDS.SetCaption(_T("RDS"));
#endif
	m_tagTitleRDS.LinkTextGL(pTextGL);
	m_tagTitleRDS.SetPos(0,py);
	m_tagTitleRDS.SetCaptionHeight(caption_h);
	m_tagTitleRDS.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleRDS.SetCaptionColor(&cr_gray, &cr_gray);

	py-=tag_h;

	m_tagTitleRDSSwitch.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_289),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRDSSwitch.SetSize(cx - margin_left, tag_h);
#if (CVTE_EN_RBDS)
	m_tagTitleRDSSwitch.SetCaption(_T("Enable RBDS"));
#else
	m_tagTitleRDSSwitch.SetCaption(GetResourceString(IDS_SETTING_RADIO_RDS_SWITCH));
#endif
	m_tagTitleRDSSwitch.LinkTextGL(pTextGL);
	m_tagTitleRDSSwitch.SetPos(margin_x/2, py);
	m_tagTitleRDSSwitch.SetCaptionHeight(caption_h);
	m_tagTitleRDSSwitch.SetCaptionPos(BTN_CAPTION_LEFT,captionMargin);
	m_tagTitleRDSSwitch.SetCaptionColor(&cr_gray);

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

	m_sliRDSSwitch.Initialize(IDC_RADIO_SLI_ENABLE_RDS,this,&sli,pSkinManager);
	m_sliRDSSwitch.SetRange(0,10);
	m_sliRDSSwitch.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS);
	m_sliRDSSwitch.SetPos(m_siCurSize.cx/2-70,py);
	m_sliRDSSwitch.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);


	py-=tag_h;

	m_tagTitleAF.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_289),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAF.SetSize(cx - margin_left, tag_h);
	m_tagTitleAF.SetCaption(_T("AF"));
	m_tagTitleAF.LinkTextGL(pTextGL);
	m_tagTitleAF.SetPos(margin_x/2, py);
	m_tagTitleAF.SetCaptionHeight(caption_h);
	m_tagTitleAF.SetCaptionPos(BTN_CAPTION_LEFT,captionMargin);
	m_tagTitleAF.SetCaptionColor(&cr_gray);

	m_sliAF.Initialize(IDC_RADIO_SLI_AF,this,&sli,pSkinManager);
	m_sliAF.SetRange(0,10);
	m_sliAF.SetValue(0);
	m_sliAF.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAF.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	py-=tag_h;

	m_tagTitleTA.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_289),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleTA.SetSize(cx - margin_left, tag_h);
	m_tagTitleTA.SetCaption(_T("TA"));
	m_tagTitleTA.LinkTextGL(pTextGL);
	m_tagTitleTA.SetPos(margin_x/2, py);
	m_tagTitleTA.SetCaptionHeight(caption_h);
	m_tagTitleTA.SetCaptionPos(BTN_CAPTION_LEFT,captionMargin);
	m_tagTitleTA.SetCaptionColor(&cr_gray);

	m_sliTA.Initialize(IDC_RADIO_SLI_TA,this,&sli,pSkinManager);
	m_sliTA.SetRange(0,10);
	m_sliTA.SetValue(0);
	m_sliTA.SetPos(m_siCurSize.cx/2-70,py);
	m_sliTA.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
#endif
#if CVTE_EN_DAB
	py-=tag_h;
	g_pyDABTitle = py;

	//dab:
	m_tagTitleDAB.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleDAB.SetSize(cx, gridHeight);
	m_tagTitleDAB.SetCaption(_T("DAB"));
	m_tagTitleDAB.LinkTextGL(pTextGL);
	m_tagTitleDAB.SetPos(0,py);
	m_tagTitleDAB.SetCaptionHeight(caption_h);
	m_tagTitleDAB.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleDAB.SetCaptionColor(&cr_gray, &cr_gray);

	py -=tag_h;

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

	m_sliServiceLink.Initialize(IDC_DAB_SLI_ENABLE_SERVICELINK,this,&sli,pSkinManager);
	m_sliServiceLink.SetRange(0,10);
	m_sliServiceLink.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDABServiceLink?10:0);
	m_sliServiceLink.SetPos(m_siCurSize.cx/2-70,py);
	m_sliServiceLink.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	m_tagTitleServiceLink.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleServiceLink.SetSize(cx - margin_left, tag_h);
	m_tagTitleServiceLink.SetCaption(_T("Service Link"));
	m_tagTitleServiceLink.LinkTextGL(pTextGL);
	m_tagTitleServiceLink.SetPos(margin_x/2, py);
	m_tagTitleServiceLink.SetCaptionHeight(caption_h);
	m_tagTitleServiceLink.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleServiceLink.SetCaptionColor(&cr_gray, &cr_gray);

	py -= tag_h;

	m_sliLBand.Initialize(IDC_DAB_SLI_ENABLE_LBAND,this,&sli,pSkinManager);
	m_sliLBand.SetRange(0,10);
	m_sliLBand.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDABLBAND?10:0);
	m_sliLBand.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLBand.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	m_tagTitleLBand.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleLBand.SetSize(cx - margin_left, tag_h);
	m_tagTitleLBand.SetCaption(_T("LBAND"));
	m_tagTitleLBand.LinkTextGL(pTextGL);
	m_tagTitleLBand.SetPos(margin_x/2,py);
	m_tagTitleLBand.SetCaptionHeight(caption_h);
	m_tagTitleLBand.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleLBand.SetCaptionColor(&cr_gray, &cr_gray);

	py -= tag_h;

	m_sliDABTA.Initialize(IDC_DAB_SLI_ENABLE_TA,this,&sli,pSkinManager);
	m_sliDABTA.SetRange(0,10);
	m_sliDABTA.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDABTA?10:0);
	m_sliDABTA.SetPos(m_siCurSize.cx/2-70,py);
	m_sliDABTA.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	m_tagDABTitleTA.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagDABTitleTA.SetSize(cx - margin_left, tag_h);
	m_tagDABTitleTA.SetCaption(_T("DAB TA"));
	m_tagDABTitleTA.LinkTextGL(pTextGL);
	m_tagDABTitleTA.SetPos(margin_x/2, py);
	m_tagDABTitleTA.SetCaptionHeight(caption_h);
	m_tagDABTitleTA.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagDABTitleTA.SetCaptionColor(&cr_gray, &cr_gray);

	py -= tag_h;

	m_sliDABAntennaPower.Initialize(IDC_DAB_SLI_ENABLE_ANTENNAPOWER,this,&sli,pSkinManager);
	m_sliDABAntennaPower.SetRange(0,10);
	m_sliDABAntennaPower.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDABAntPowerOn?10:0);
	m_sliDABAntennaPower.SetPos(m_siCurSize.cx/2-70,py);
	m_sliDABAntennaPower.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	m_tagDABAntennaPower.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagDABAntennaPower.SetSize(cx - margin_left, tag_h);
	m_tagDABAntennaPower.SetCaption(_T("DAB Antenna Power"));
	m_tagDABAntennaPower.LinkTextGL(pTextGL);
	m_tagDABAntennaPower.SetPos(margin_x/2,py);
	m_tagDABAntennaPower.SetCaptionHeight(caption_h);
	m_tagDABAntennaPower.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagDABAntennaPower.SetCaptionColor(&cr_gray, &cr_gray);

	DABApi_ServiceLinkOnOff(m_pSkinManager->GetConfig()->GetCurParam()->bDABServiceLink);
	BYTE bON;
	if(DABApi_GetLBandSetting(&bON))
	{
		if(m_pSkinManager->GetConfig()->GetCurParam()->bDABLBAND != bON)
		{
			DABApi_SetLBandSetting();
		}
	}
	if(DABApi_GetTASetting(&bON))
	{
		if(m_pSkinManager->GetConfig()->GetCurParam()->bDABTA != bON)
		{
			DABApi_SetTASetting();
		}
	}
	SendDABCommand(DAB_CMD_ANTENNA_POWER,m_pSkinManager->GetConfig()->GetCurParam()->bDABAntPowerOn);
#endif

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	UpdateRDS(m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS);
	UpdateAF(m_pSkinManager->GetConfig()->GetCurParam()->bRadioAF);
	UpdateTA(m_pSkinManager->GetConfig()->GetCurParam()->bRadioTA);

	//not set dwStopLevelAF in config, use default value:
	if(m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF == 0)
	{
		m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF = CVTE_REG_RDS_AF_LEVEL_DEFAULT;
	}
	UpdateAFStopLevel(m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF);
#endif
	// SetRegion(m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion);

	g_iPanelHeight = g_iClientHeight/2 - py + tag_h;
#else
	//int cy=cx;
	int caption_h = 22;

	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
	{
		caption_h = 20;
	}

	/*
	//local:
	m_tagTitleLocal.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLocal.SetSize((g_iClientWidth-box_size_x-margin_x*2),80);
	m_tagTitleLocal.SetCaption(_T("Local"));
	m_tagTitleLocal.LinkTextGL(pTextGL);
	m_tagTitleLocal.SetPos(0,py);
	m_tagTitleLocal.SetCaptionHeight(caption_h);
	m_tagTitleLocal.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleLocal.SetCaptionColor(&cr_gray);

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

	m_sliLocal.Initialize(IDC_RADIO_SLI_LOCAL,this,&sli,pSkinManager);
	m_sliLocal.SetRange(0,10);
	m_sliLocal.SetValue(0);
	m_sliLocal.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLocal.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	*/

	//stop level:
	m_tagTitleStopLevel.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleStopLevel.SetSize(cx,40);
	m_tagTitleStopLevel.SetCaption(GetResourceString(IDS_SETTING_RADIO_SEEK_QUALITY));
	m_tagTitleStopLevel.LinkTextGL(pTextGL);
	m_tagTitleStopLevel.SetPos(-cx/2-10 +12,py);
	m_tagTitleStopLevel.SetCaptionHeight(20);
	m_tagTitleStopLevel.EnableDrawBack(FALSE);
	m_tagTitleStopLevel.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleStopLevel.SetCaptionColor(&cr_gray);

	py-=60;

	m_tagTitleStopLevelFM.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleStopLevelFM.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleStopLevelFM.SetCaption(GetResourceString(IDS_SETTING_RADIO_FM));
	m_tagTitleStopLevelFM.LinkTextGL(pTextGL);
	m_tagTitleStopLevelFM.SetPos(0,py);
	m_tagTitleStopLevelFM.SetCaptionHeight(caption_h);
	m_tagTitleStopLevelFM.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleStopLevelFM.SetCaptionColor(&cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;
	sli.dwType=ST_HOR_MIDDLE;

	m_sliStopLevelFM.Initialize(IDC_RADIO_SLI_FM_STOP_LEVEL,this,&sli,pSkinManager);
	m_sliStopLevelFM.SetRange(-5,5);
	m_sliStopLevelFM.SetValue(0);
	m_sliStopLevelFM.SetPos(m_siCurSize.cx/2-144,py);
	m_sliStopLevelFM.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	m_sliStopLevelFM.LinkTextGL(pTextGL);
	m_sliStopLevelFM.SetCaptionColor(&cr_gray);
	m_sliStopLevelFM.ShowValue(TRUE);
	m_sliStopLevelFM.EnableMoveNotify(TRUE);
	m_sliStopLevelFM.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	m_tagTitleStopLevelAM.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleStopLevelAM.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleStopLevelAM.SetCaption(GetResourceString(IDS_SETTING_RADIO_AM));
	m_tagTitleStopLevelAM.LinkTextGL(pTextGL);
	m_tagTitleStopLevelAM.SetPos(0,py);
	m_tagTitleStopLevelAM.SetCaptionHeight(caption_h);
	m_tagTitleStopLevelAM.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleStopLevelAM.SetCaptionColor(&cr_gray);

	m_sliStopLevelAM.Initialize(IDC_RADIO_SLI_AM_STOP_LEVEL,this,&sli,pSkinManager);
	m_sliStopLevelAM.SetRange(-5,5);
	m_sliStopLevelAM.SetValue(0);
	m_sliStopLevelAM.SetPos(m_siCurSize.cx/2-144,py);
	m_sliStopLevelAM.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	m_sliStopLevelAM.LinkTextGL(pTextGL);
	m_sliStopLevelAM.SetCaptionColor(&cr_gray);
	m_sliStopLevelAM.ShowValue(TRUE);
	m_sliStopLevelAM.EnableMoveNotify(TRUE);
	m_sliStopLevelAM.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

#if (CVTE_EN_RDS || CVTE_EN_RBDS) && (CVTE_EN_RDS_AF_STOP_LEVEL)
	m_tagTitleStopLevelAF.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleStopLevelAF.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleStopLevelAF.SetCaption(_T("AF"));
	m_tagTitleStopLevelAF.LinkTextGL(pTextGL);
	m_tagTitleStopLevelAF.SetPos(0,py);
	m_tagTitleStopLevelAF.SetCaptionHeight(caption_h);
	m_tagTitleStopLevelAF.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleStopLevelAF.SetCaptionColor(&cr_gray);

	m_sliStopLevelAF.Initialize(IDC_RADIO_BTN_AF_STOP_LEVEL,this,&sli,pSkinManager);
#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	m_sliStopLevelAF.SetRange(-20,20);
#else
	m_sliStopLevelAF.SetRange(-20,20);
#endif
	m_sliStopLevelAF.SetValue(0);
	m_sliStopLevelAF.SetPos(m_siCurSize.cx/2-144,py);
	m_sliStopLevelAF.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	m_sliStopLevelAF.LinkTextGL(pTextGL);
	m_sliStopLevelAF.SetCaptionColor(&cr_gray);
	m_sliStopLevelAF.ShowValue(TRUE);
	m_sliStopLevelAF.EnableMoveNotify(TRUE);
	m_sliStopLevelAF.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;
#endif

	m_btnResetStopLevel.Initialize(IDC_RADIO_BTN_RESET_STOP_LEVEL,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnResetStopLevel.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnResetStopLevel.SetCaption(GetResourceString(IDS_SETTING_RADIO_RESET_SEEK_QUALITY));
	m_btnResetStopLevel.LinkTextGL(pTextGL);
	m_btnResetStopLevel.SetPos(0,py);
	m_btnResetStopLevel.SetCaptionHeight(caption_h);
	m_btnResetStopLevel.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnResetStopLevel.SetCaptionColor(&cr_gray);
	m_btnResetStopLevel.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	py-=70;

	//Region:
	g_pyRadioRegionTitle = py;

	m_tagTitleRegion.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRegion.SetSize(cx,40);
	m_tagTitleRegion.SetCaption(GetResourceString(IDS_SETTING_RADIO_REGION));
	m_tagTitleRegion.LinkTextGL(pTextGL);
	m_tagTitleRegion.SetPos(-cx/2-10 +12,py);
	m_tagTitleRegion.SetCaptionHeight(20);
	m_tagTitleRegion.EnableDrawBack(FALSE);
	m_tagTitleRegion.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleRegion.SetCaptionColor(&cr_gray);

	py-=60;

	//
	m_btnResetStopLevel.Initialize(IDC_RADIO_BTN_RESET_STOP_LEVEL,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnResetStopLevel.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnResetStopLevel.SetCaption(GetResourceString(IDS_SETTING_RADIO_RESET_SEEK_QUALITY));
	m_btnResetStopLevel.LinkTextGL(pTextGL);
	m_btnResetStopLevel.SetPos(0,py);
	m_btnResetStopLevel.SetCaptionHeight(caption_h);
	m_btnResetStopLevel.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnResetStopLevel.SetCaptionColor(&cr_gray);
	m_btnResetStopLevel.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	//

	m_btnRadioRegion.Initialize(IDC_RADIO_BTN_REGION,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnRadioRegion.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnRadioRegion.SetPos(0,py);

	m_btnRadioRegion.LinkTextGL(pTextGL);
	m_btnRadioRegion.SetCaptionHeight(caption_h);
	m_btnRadioRegion.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnRadioRegion.SetCaptionColor(&cr_gray);
	m_btnRadioRegion.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	UpdateRegionDisplay(m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion);

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	py-=70;
	g_pyRDSTitle = py;

	//rds:
	m_tagTitleRDS.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRDS.SetSize(cx,40);
#if (CVTE_EN_RBDS)
	m_tagTitleRDS.SetCaption(_T("RBDS"));
#else
	m_tagTitleRDS.SetCaption(_T("RDS"));
#endif
	m_tagTitleRDS.LinkTextGL(pTextGL);
	m_tagTitleRDS.SetPos(-cx/2-10 +12,py);
	m_tagTitleRDS.SetCaptionHeight(20);
	m_tagTitleRDS.EnableDrawBack(FALSE);
	m_tagTitleRDS.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleRDS.SetCaptionColor(&cr_gray);

	py-=60;

	m_tagTitleRDSSwitch.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRDSSwitch.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
#if (CVTE_EN_RBDS)
	m_tagTitleRDSSwitch.SetCaption(_T("RBDS"));
#else
	m_tagTitleRDSSwitch.SetCaption(GetResourceString(IDS_SETTING_RADIO_RDS_SWITCH));
#endif
	m_tagTitleRDSSwitch.LinkTextGL(pTextGL);
	m_tagTitleRDSSwitch.SetPos(0,py);
	m_tagTitleRDSSwitch.SetCaptionHeight(caption_h);
	m_tagTitleRDSSwitch.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleRDSSwitch.SetCaptionColor(&cr_gray);

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

	m_sliRDSSwitch.Initialize(IDC_RADIO_SLI_ENABLE_RDS,this,&sli,pSkinManager);
	m_sliRDSSwitch.SetRange(0,10);
	m_sliRDSSwitch.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS);
	m_sliRDSSwitch.SetPos(m_siCurSize.cx/2-70,py);
	m_sliRDSSwitch.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);


	py-=tag_h;

	m_tagTitleAF.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAF.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleAF.SetCaption(_T("AF"));
	m_tagTitleAF.LinkTextGL(pTextGL);
	m_tagTitleAF.SetPos(0,py);
	m_tagTitleAF.SetCaptionHeight(caption_h);
	m_tagTitleAF.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleAF.SetCaptionColor(&cr_gray);

	m_sliAF.Initialize(IDC_RADIO_SLI_AF,this,&sli,pSkinManager);
	m_sliAF.SetRange(0,10);
	m_sliAF.SetValue(0);
	m_sliAF.SetPos(m_siCurSize.cx/2-70,py);
	m_sliAF.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	py-=tag_h;

	m_tagTitleTA.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleTA.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleTA.SetCaption(_T("TA"));
	m_tagTitleTA.LinkTextGL(pTextGL);
	m_tagTitleTA.SetPos(0,py);
	m_tagTitleTA.SetCaptionHeight(caption_h);
	m_tagTitleTA.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleTA.SetCaptionColor(&cr_gray);

	m_sliTA.Initialize(IDC_RADIO_SLI_TA,this,&sli,pSkinManager);
	m_sliTA.SetRange(0,10);
	m_sliTA.SetValue(0);
	m_sliTA.SetPos(m_siCurSize.cx/2-70,py);
	m_sliTA.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
#endif
#if CVTE_EN_DAB
	py-=70;
	g_pyDABTitle = py;

	//rds:
	m_tagTitleDAB.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleDAB.SetSize(cx,40);
	m_tagTitleDAB.SetCaption(_T("DAB"));
	m_tagTitleDAB.LinkTextGL(pTextGL);
	m_tagTitleDAB.SetPos(-cx/2-10 +12,py);
	m_tagTitleDAB.SetCaptionHeight(20);
	m_tagTitleDAB.EnableDrawBack(FALSE);
	m_tagTitleDAB.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleDAB.SetCaptionColor(&cr_gray);

	py -=60;

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

	m_sliServiceLink.Initialize(IDC_DAB_SLI_ENABLE_SERVICELINK,this,&sli,pSkinManager);
	m_sliServiceLink.SetRange(0,10);
	m_sliServiceLink.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDABServiceLink?10:0);
	m_sliServiceLink.SetPos(m_siCurSize.cx/2-70,py);
	m_sliServiceLink.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	m_tagTitleServiceLink.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleServiceLink.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleServiceLink.SetCaption(_T("Service Link"));
	m_tagTitleServiceLink.LinkTextGL(pTextGL);
	m_tagTitleServiceLink.SetPos(0,py);
	m_tagTitleServiceLink.SetCaptionHeight(24);
	m_tagTitleServiceLink.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleServiceLink.SetCaptionColor(&cr_gray);

	py -= tag_h;

	m_sliLBand.Initialize(IDC_DAB_SLI_ENABLE_LBAND,this,&sli,pSkinManager);
	m_sliLBand.SetRange(0,10);
	m_sliLBand.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDABLBAND?10:0);
	m_sliLBand.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLBand.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	m_tagTitleLBand.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLBand.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLBand.SetCaption(_T("LBAND"));
	m_tagTitleLBand.LinkTextGL(pTextGL);
	m_tagTitleLBand.SetPos(0,py);
	m_tagTitleLBand.SetCaptionHeight(24);
	m_tagTitleLBand.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleLBand.SetCaptionColor(&cr_gray);

	py -= tag_h;

	m_sliDABTA.Initialize(IDC_DAB_SLI_ENABLE_TA,this,&sli,pSkinManager);
	m_sliDABTA.SetRange(0,10);
	m_sliDABTA.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDABTA?10:0);
	m_sliDABTA.SetPos(m_siCurSize.cx/2-70,py);
	m_sliDABTA.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	m_tagDABTitleTA.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDABTitleTA.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagDABTitleTA.SetCaption(_T("DAB TA"));
	m_tagDABTitleTA.LinkTextGL(pTextGL);
	m_tagDABTitleTA.SetPos(0,py);
	m_tagDABTitleTA.SetCaptionHeight(24);
	m_tagDABTitleTA.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagDABTitleTA.SetCaptionColor(&cr_gray);

	py -= tag_h;

	m_sliDABAntennaPower.Initialize(IDC_DAB_SLI_ENABLE_ANTENNAPOWER,this,&sli,pSkinManager);
	m_sliDABAntennaPower.SetRange(0,10);
	m_sliDABAntennaPower.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bDABAntPowerOn?10:0);
	m_sliDABAntennaPower.SetPos(m_siCurSize.cx/2-70,py);
	m_sliDABAntennaPower.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

	m_tagDABAntennaPower.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDABAntennaPower.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagDABAntennaPower.SetCaption(_T("DAB Antenna Power"));
	m_tagDABAntennaPower.LinkTextGL(pTextGL);
	m_tagDABAntennaPower.SetPos(0,py);
	m_tagDABAntennaPower.SetCaptionHeight(24);
	m_tagDABAntennaPower.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagDABAntennaPower.SetCaptionColor(&cr_gray);

	DABApi_ServiceLinkOnOff(m_pSkinManager->GetConfig()->GetCurParam()->bDABServiceLink);
	BYTE bON;
	if(DABApi_GetLBandSetting(&bON))
	{
		if(m_pSkinManager->GetConfig()->GetCurParam()->bDABLBAND != bON)
		{
			DABApi_SetLBandSetting();
		}
	}
	if(DABApi_GetTASetting(&bON))
	{
		if(m_pSkinManager->GetConfig()->GetCurParam()->bDABTA != bON)
		{
			DABApi_SetTASetting();
		}
	}
	SendDABCommand(DAB_CMD_ANTENNA_POWER,m_pSkinManager->GetConfig()->GetCurParam()->bDABAntPowerOn);
#endif

	// 	py-=80;
	//
	// 	m_sliPTY.Initialize(IDC_RADIO_SLI_ENABLE_PTY,this,&sli,pSkinManager);
	// 	m_sliPTY.SetRange(0,10);
	// 	m_sliPTY.SetValue(0);
	// 	m_sliPTY.SetPos(m_siCurSize.cx/2-70,py);
	// 	m_sliPTY.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);
	//
	// 	m_btnPTY.Initialize(IDC_RADIO_BTN_PTY_SEL,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
	// 		m_pSkinManager->GetTexture(TEXID_607),NULL,
	// 		NULL,NULL,
	// 		NULL,NULL);
	// 	m_btnPTY.SetSize((g_iClientWidth-box_size_x-margin_x*2),80);
	// 	m_btnPTY.SetCaption(_T("PTY"));
	// 	m_btnPTY.LinkTextGL(pTextGL);
	// 	m_btnPTY.SetPos(0,py);
	// 	m_btnPTY.SetCaptionHeight(caption_h);
	// 	m_btnPTY.SetCaptionPos(BTN_CAPTION_LEFT,20);
	// 	m_btnPTY.SetCaptionColor(&cr_gray);
	// 	m_btnPTY.SetNotifyFunc(&CPanelSettingRadio::OnNotify,this);

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	UpdateRDS(m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS);
	UpdateAF(m_pSkinManager->GetConfig()->GetCurParam()->bRadioAF);
	UpdateTA(m_pSkinManager->GetConfig()->GetCurParam()->bRadioTA);

	//not set dwStopLevelAF in config, use default value:
	if(m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF == 0)
	{
		m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF = CVTE_REG_RDS_AF_LEVEL_DEFAULT;
	}
	UpdateAFStopLevel(m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF);
#endif
	// SetRegion(m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion);

	g_iPanelHeight = g_iClientHeight/2 - py + tag_h;
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	if(g_iPanelHeight < g_iClientHeight)
		g_iPanelHeight = g_iClientHeight;

	SetSize(g_iClientWidth-box_size_x,g_iPanelHeight);

	return TRUE;
}


void CALLBACK CPanelSettingRadio::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingRadio *panel=(CPanelSettingRadio*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingRadio::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_RADIO_SLI_LOCAL:
		SetLocal(wPara);
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_BTN_RESET_STOP_LEVEL:
		ResetStopLevel();
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
		SetAFStopLevel(CVTE_REG_RDS_AF_LEVEL_DEFAULT);
		m_sliStopLevelAF.SetValue(0);
#endif
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_SLI_FM_STOP_LEVEL:
		SetFMStopLevel(int(wPara)+DEFAULT_FM_LEVEL);
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_SLI_AM_STOP_LEVEL:
		SetAMStopLevel(int(wPara)+DEFAULT_AM_LEVEL);
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_BTN_AF_STOP_LEVEL:
		{
			SetAFStopLevel(int(wPara)*2+CVTE_REG_RDS_AF_LEVEL_DEFAULT);
			PlaySoundEffect(0);
		}
		break;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	case IDC_RADIO_SLI_ENABLE_RDS:
		SetRDS(wPara);
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_SLI_AF:
		SetAF(wPara);
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_SLI_TA:
		SetTA(wPara);
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_SLI_ENABLE_PTY:
		SetPTY(wPara);
		PlaySoundEffect(0);
		break;
	case IDC_RADIO_BTN_PTY_SEL:
		SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_RDS_PTY);
		PlaySoundEffect(0);
		break;
#endif
#if CVTE_EN_DAB
	case IDC_DAB_SLI_ENABLE_SERVICELINK:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bDABServiceLink = wPara?TRUE:FALSE;
			DABApi_ServiceLinkOnOff(wPara?TRUE:FALSE);

			byte ServiceLink;
			DABApi_GetSrvLinkSetting(&ServiceLink);
			RETAILMSG(1,(_T("===DAB set ServiceLink=%d\r\n"),ServiceLink));

			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_SLI_ENABLE_LBAND:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bDABLBAND = wPara?TRUE:FALSE;
			byte bON;
			if(DABApi_GetLBandSetting(&bON))
			{
				if(wPara != bON)
				{
					DABApi_SetLBandSetting();
				}

				DABApi_GetLBandSetting(&bON);
				RETAILMSG(1,(_T("===DAB set LBand=%d\r\n"),bON));
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_SLI_ENABLE_TA:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bDABTA = wPara?TRUE:FALSE;
			byte bON;
			if(DABApi_GetTASetting(&bON))
			{
				if(wPara != bON)
				{
					DABApi_SetTASetting();
				}

				DABApi_GetTASetting(&bON);
				RETAILMSG(1,(_T("===DAB set TA=%d\r\n"),bON));
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_SLI_ENABLE_ANTENNAPOWER:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bDABAntPowerOn = wPara?TRUE:FALSE;
			SendDABCommand(DAB_CMD_ANTENNA_POWER,wPara?1:0);
			PlaySoundEffect(0);
		}
		break;
#endif
	case IDC_RADIO_BTN_REGION:
		{
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_RADIO_REGION);
			PlaySoundEffect(0);
		}
		break;
	default:
		break;
	}
}

void CPanelSettingRadio::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}


	if(dwType == 0)//package
	{
		UpdateLocal(pCommander->GetItemData(MCU_LOC));
 		UpdateRegionDisplay(pCommander->GetItemData(MCU_LOCAL_AREA));
		//m_iFMStopLevel = pCommander->GetItemData(MCU_RADIO_FM_STOP_LEVEL);
		//m_iAMStopLevel = pCommander->GetItemData(MCU_RADIO_AM_STOP_LEVEL);
    	//RETAILMSG(1, (TEXT("MSG:[CPanelSettingRadio::OnMCUCmd]: m_iFMStopLevel:%d m_iAMStopLevel:%d\r\n"),m_iFMStopLevel,m_iAMStopLevel));
		UpdateFMStopLevel(pCommander->GetItemData(MCU_RADIO_FM_STOP_LEVEL));
		UpdateAMStopLevel(pCommander->GetItemData(MCU_RADIO_AM_STOP_LEVEL));
	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
		case MCU_LOC:
			UpdateLocal(pCommander->GetItemData(MCU_LOC));
			break;
		case MCU_LOCAL_AREA:
 			UpdateRegionDisplay(pCommander->GetItemData(MCU_LOCAL_AREA));
			break;
		case MCU_RADIO_FM_STOP_LEVEL:
			UpdateFMStopLevel(pCommander->GetItemData(MCU_RADIO_FM_STOP_LEVEL));
			break;
		case MCU_RADIO_AM_STOP_LEVEL:
			UpdateAMStopLevel(pCommander->GetItemData(MCU_RADIO_AM_STOP_LEVEL));
			break;
		}
	}
}
void CPanelSettingRadio::UpdateRegionDisplay(DWORD dwRegion)
{
    RETAILMSG(1,(_T("MSG:[CPanelSettingRadio::UpdateRegionDisplay]: dwRegion%d\r\n"),dwRegion));

	switch (dwRegion)
	{
	case RADIO_REGION_Europe:
		//MCU有个bug，当我们发收音区域4时，它回0
		if (m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_Euro_EastEuro || m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_EastEuro_Euro)
		{
			wchar_t temp[256];
			if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
				swprintf_s(temp,256,_T("%s и %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
			else
				swprintf_s(temp,256,_T("%s & %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));

			m_btnRadioRegion.SetCaption(temp);
		}
		else
		{
			m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_EUROPE));
		}
		break;
	case RADIO_REGION_USA://USA
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_USA));
		break;
	case RADIO_REGION_EasternEurope://EasternEurope
		//当选择了欧洲&东欧区域后，可在收音区域手动切到东欧，此时MCU回2，需要特殊处理
		if (m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_Euro_EastEuro || m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_EastEuro_Euro)
		{
			wchar_t temp[256];
			if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
				swprintf_s(temp,256,_T("%s и %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
			else
				swprintf_s(temp,256,_T("%s & %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
			m_btnRadioRegion.SetCaption(temp);
		}
		else
		{
			m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_EASTERN_EUROPE));
		}
		break;
	case RADIO_REGION_Japan://Japan
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_JAPAN));
		break;
	case RADIO_REGION_Euro_EastEuro://Europe + EasternEurope
		{
			wchar_t temp[256];
			if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
				swprintf_s(temp,256,_T("%s и %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
			else
				swprintf_s(temp,256,_T("%s & %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
			m_btnRadioRegion.SetCaption(temp);
		}
		break;
	case RADIO_REGION_Korean:
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_SETTING_KOREA));
		break;
	case RADIO_REGION_TaiWang:
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_TAIWAN));
		break;
	case RADIO_REGION_USA2:
		{
			wchar_t temp[256];
			swprintf_s(temp,256,_T("%s 2"),GetResourceString(IDS_SETTING_USA));
			m_btnRadioRegion.SetCaption(temp);
		}
		break;
	case RADIO_REGION_SouthAmerica:
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_SOUTH_AMERICA));
		break;
	case RADIO_REGION_ArabiaAustralia:
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_ARABIA_AUSTRALIA));
		break;
	case RADIO_REGION_LatinAmerica:
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_LATIN_AMERICA));
		break;
	case RADIO_REGION_MiddleEast:
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_MIDDLE_EAST));
		break;
	case RADIO_REGION_Aust:
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_AUST));
		break;
	case RADIO_REGION_Brazil:
		m_btnRadioRegion.SetCaption(GetResourceString(IDS_BRAZIL));
		break;
	//case RADIO_REGION_Russia:
	//	m_btnRadioRegion.SetCaption(GetResourceString(IDS_RUSSIA));
	//	break;
	case RADIO_REGION_EastEuro_Euro://EasternEurope + Europe
		{
			wchar_t temp[256];
			if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
				swprintf_s(temp,256,_T("%s и %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
			else
				swprintf_s(temp,256,_T("%s & %s"),GetResourceString(IDS_SETTING_EUROPE),GetResourceString(IDS_SETTING_EASTERN_EUROPE));
			m_btnRadioRegion.SetCaption(temp);
		}
		break;
	default:
		break;
	}
}

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
void CPanelSettingRadio::UpdateRDS(BOOL bON)
{
	RETAILMSG(1, (TEXT(" @@@@ UpdateRDS state :%d\r\n"),bON));

	m_sliRDSSwitch.SetValue(bON?10:0);
	m_sliTA.Enable(bON);
	m_sliAF.Enable(bON);
	m_sliPTY.Enable(bON);
	m_btnPTY.EnableNotify(bON&&m_sliPTY.GetValue()!=0);
}
void CPanelSettingRadio::UpdateTA(BOOL bON)
{
	m_sliTA.SetValue(bON?10:0);
}
void CPanelSettingRadio::UpdateAF(BOOL bON)
{
	m_sliAF.SetValue(bON?10:0);
}
void CPanelSettingRadio::UpdatePTY(BOOL bON)
{
	m_sliPTY.SetValue(bON?10:0);
	m_btnPTY.EnableNotify(m_sliPTY.GetValue()!=0);
}
void CPanelSettingRadio::UpdatePTYid(ProgramType id)
{
	RETAILMSG(1, (TEXT(" @@@@ UpdatePTYid:%d\r\n"),id));

	if(id<PT_Count)
	{
		CM_String str(_T("PTY ("));
		str+=ProgramTypeToString(id);
		str+=_T(")");
		m_btnPTY.SetCaption(str.String());
	}
	else
	{
		m_btnPTY.SetCaption(_T("PTY"));
	}
}
#endif

void CPanelSettingRadio::UpdateLocal(BOOL bON)
{
// 	if(bON)
// 	{
// 		m_sliLocal.SetValue(10);
// 	}
// 	else
// 	{
// 		m_sliLocal.SetValue(0);
// 	}

}
void CPanelSettingRadio::UpdateFMStopLevel(DWORD dwLevel)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingRadio::UpdateFMStopLevel]: dwLevel:%d \r\n"),dwLevel));
	m_sliStopLevelFM.SetValue(int(dwLevel)-DEFAULT_FM_LEVEL);
}
void CPanelSettingRadio::UpdateAMStopLevel(DWORD dwLevel)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingRadio::UpdateAMStopLevel]: dwLevel:%d \r\n"),dwLevel));
	m_sliStopLevelAM.SetValue(int(dwLevel)-DEFAULT_AM_LEVEL);
}
void CPanelSettingRadio::UpdateAFStopLevel(DWORD dwLevel)
{
	RETAILMSG(1, (TEXT("MSG:[CPanelSettingRadio::UpdateAFStopLevel]: dwLevel:%d \r\n"),dwLevel));
	m_sliStopLevelAF.SetValue((int(dwLevel)-CVTE_REG_RDS_AF_LEVEL_DEFAULT)/2);
}

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
void CPanelSettingRadio::SetRDS(BOOL bON)
{
	SendRadioCommand(RADIO_CMD_RDS_SWITCH,(1<<16)+(bON?1:0));

	m_sliTA.Enable(bON);
	m_sliAF.Enable(bON);
	m_sliPTY.Enable(bON);
	m_btnPTY.EnableNotify(bON&&m_sliPTY.GetValue()!=0);
}
void CPanelSettingRadio::SetTA(BOOL bON)
{
	SendRadioCommand(RADIO_CMD_TA_SWITCH,bON?1:0);
}
void CPanelSettingRadio::SetAF(BOOL bON)
{
	SendRadioCommand(RADIO_CMD_AF_SWITCH,bON?1:0);
}
void CPanelSettingRadio::SetPTY(BOOL bON)
{
	SendRadioCommand(RADIO_CMD_PTY_SWITCH,bON?1:0);
}
#endif
void CPanelSettingRadio::ResetStopLevel()
{
	SendRadioCommand(RADIO_CMD_RESET_STOP_LEVEL,0);
}
void CPanelSettingRadio::SetLocal(BOOL bON)
{
	SendRadioCommand(RADIO_CMD_SET_LOCAL_ONOFF,bON?1:0);
}

void CPanelSettingRadio::SetFMStopLevel(DWORD dwLevel)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingRadio::SetFMStopLevel]: dwLevel:%d\r\n"),dwLevel));
	SendRadioCommand(RADIO_CMD_SET_FM_STOP_LEVEL,BYTE(dwLevel));
}
void CPanelSettingRadio::SetAMStopLevel(DWORD dwLevel)
{
	SendRadioCommand(RADIO_CMD_SET_AM_STOP_LEVEL,BYTE(dwLevel));
}

void CPanelSettingRadio::SetAFStopLevel(DWORD dwLevel)
{
	RETAILMSG(1, (TEXT("MSG:[CPanelSettingRadio::SetAFStopLevel]: [%d] \r\n"),dwLevel));
	m_pSkinManager->GetConfig()->GetCurParam()->dwStopLevelAF = dwLevel;
}
