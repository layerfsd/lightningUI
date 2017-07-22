#include "PanelSettingFactory.h"
#include <pkfuncs.h>
#include "../../resource.h"
#include "../Media/MediaCommon.h"
#include "../../Include/oemsector.h"
#include "../../CarPlay/CarplayAdapter.h"

const int box_size_x=LIST_BOX_ITEM_WIDTH;
const int margin_x=12;
const int tag_h = LIST_BOX_TAG_HEIGHT;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
const int cx=g_iClientWidth-box_size_x;
#else
const int cx=g_iClientWidth-box_size_x-margin_x*2;
#endif
const int tag_w=cx/3;
const int caption_h = 22;
const int caption2_h = 18;
const int caption3_h = 14;

#define RESET_TOUCH_EVENT_NAME TEXT("RESET_TOUCH_EVENT_NAME")
#define RESET_TOUCH_FEEDBACK_EVENT_NAME TEXT("RESET_TOUCH_FEEDBACK_EVENT_NAME")

CPanelSettingFactory::CPanelSettingFactory(void)
{
	m_DataCS = new CCriticalSection;
	strAvccPath = _T("\\GPS CARD\\960x540.avcc");
	strH264Path = _T("\\GPS CARD\\960x540.h264");
    m_bMediaTesting = FALSE;
    m_bTouchTesting = FALSE;
	m_bTpAutoRecalibrating = FALSE;
	m_bCpuMonitorActivate = FALSE;
	m_bExplorerIsRunning = FALSE;
	m_bUsbTestSpread = FALSE;
}
CPanelSettingFactory::~CPanelSettingFactory(void)
{
	if(m_DataCS != NULL)
	{
		delete m_DataCS;
		m_DataCS=NULL;
	}

	DeleteCriticalSection(&m_csMonitor);
}

BOOL CPanelSettingFactory::IsReady()
{
	return (CPanelGL::IsReady());

}

void CPanelSettingFactory::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingFactory::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingFactory::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif

    m_tagTitle1.SetAlpha(m_fCurAlpha);
    m_tagTitle1.Render();

    m_tagTitle2.SetAlpha(m_fCurAlpha);
    m_tagTitle2.Render();

    if(m_bMediaTesting)
    {
//			VIEW_STATE vs = m_labIconFocus.GetCurViewState();
//			vs.fRotateZ -= 1;
//			if(vs.fRotateZ < -360)
//				vs.fRotateZ = 0;
//			m_labIconFocus.SetCurViewState(&vs);

		//VIEW_STATE vs = m_btnReturn.GetCurViewState();
		//vs.fRotateZ -= 1;
		//if(vs.fRotateZ < -360)
		//	vs.fRotateZ = 0;
		//m_btnReturn.SetCurViewState(&vs);
		UpdateTime();
    }

	m_btnReturn.SetAlpha(m_fCurAlpha);
	m_btnReturn.Render();

    COLORGL cr_b={0.35,0.35,0.35,1.0};

	DrawRect(0,g_iClientHeight/2-50-tag_h/2,g_iClientWidth-box_size_x-50,1,&cr_b);
//		DrawRect(0,g_iClientHeight/2-50-tag_h*5-tag_h/2-15,g_iClientWidth-box_size_x-50,1,&cr_b);

	m_tagSystemInfo.SetAlpha(m_fCurAlpha);
    m_tagSystemInfo.Render();
	m_btnCpu.SetAlpha(m_fCurAlpha);
    m_btnCpu.Render();
	m_iconCpuFocus.SetAlpha(m_fCurAlpha);
    m_iconCpuFocus.Render();

    m_btnRearColorSignalType.SetAlpha(m_fCurAlpha);
	m_btnRearColorSignalType.Render();
//		m_tagRearColorSignalType.SetAlpha(m_fCurAlpha);
//		m_tagRearColorSignalType.Render();
	m_iconRearColorSignalType.SetAlpha(m_fCurAlpha);
	m_iconRearColorSignalType.Render();

//		m_tagMemory.SetAlpha(m_fCurAlpha);
//	    m_tagMemory.Render();

    m_btnMediaAutoTest.SetAlpha(m_fCurAlpha);
    m_btnMediaAutoTest.Render();
    m_tagMediaAutoTestInfo.SetAlpha(m_fCurAlpha);
    m_tagMediaAutoTestInfo.Render();
    m_iconMediaAutoTest.SetAlpha(m_fCurAlpha);
    m_iconMediaAutoTest.Render();
    m_iconMediaAutoTestFocus.SetAlpha(m_fCurAlpha);
    m_iconMediaAutoTestFocus.Render();

	m_tagTime.SetAlpha(m_fCurAlpha);
	m_tagTime.Render();

    m_btnBluetoothAutoTest.SetAlpha(m_fCurAlpha);
    m_btnBluetoothAutoTest.Render();
//	    m_tagBluetoothAutoTestInfo.SetAlpha(m_fCurAlpha);
//	    m_tagBluetoothAutoTestInfo.Render();
    m_iconBluetoothAutoTest.SetAlpha(m_fCurAlpha);
    m_iconBluetoothAutoTest.Render();
    m_iconBluetoothAutoTestFocus.SetAlpha(m_fCurAlpha);
    m_iconBluetoothAutoTestFocus.Render();

    m_btnTouchAutoTest.SetAlpha(m_fCurAlpha);
    m_btnTouchAutoTest.Render();
//	    m_tagTouchAutoTestInfo.SetAlpha(m_fCurAlpha);
//	    m_tagTouchAutoTestInfo.Render();
    m_iconTouchAutoTest.SetAlpha(m_fCurAlpha);
    m_iconTouchAutoTest.Render();
    m_iconTouchAutoTestFocus.SetAlpha(m_fCurAlpha);
    m_iconTouchAutoTestFocus.Render();

	m_btnTPAutoReCalibrate.SetAlpha(m_fCurAlpha);
    m_btnTPAutoReCalibrate.Render();
	m_tagTPAutoReCalibrateInfo.SetAlpha(m_fCurAlpha);
    m_tagTPAutoReCalibrateInfo.Render();
	m_iconTPAutoReCalibrate.SetAlpha(m_fCurAlpha);
    m_iconTPAutoReCalibrate.Render();
	m_iconTPAutoReCalibrateFocus.SetAlpha(m_fCurAlpha);
    m_iconTPAutoReCalibrateFocus.Render();

	m_btnSnapshot.SetAlpha(m_fCurAlpha);
    m_btnSnapshot.Render();
//		m_tagSnapshotInfo.SetAlpha(m_fCurAlpha);
//	    m_tagSnapshotInfo.Render();
	m_iconSnapshot.SetAlpha(m_fCurAlpha);
    m_iconSnapshot.Render();
	m_iconSnapshotFocus.SetAlpha(m_fCurAlpha);
    m_iconSnapshotFocus.Render();

	m_btnExplorer.SetAlpha(m_fCurAlpha);
    m_btnExplorer.Render();
	m_iconExplorer.SetAlpha(m_fCurAlpha);
    m_iconExplorer.Render();
	m_iconExplorerFocus.SetAlpha(m_fCurAlpha);
    m_iconExplorerFocus.Render();

	m_btnLED.SetAlpha(m_fCurAlpha);
    m_btnLED.Render();
	m_iconLED.SetAlpha(m_fCurAlpha);
    m_iconLED.Render();
	m_iconLEDFocus.SetAlpha(m_fCurAlpha);
    m_iconLEDFocus.Render();

	m_btnConnectTest.SetAlpha(m_fCurAlpha);
    m_btnConnectTest.Render();
	m_iconConnectTest.SetAlpha(m_fCurAlpha);
    m_iconConnectTest.Render();

	m_btnDeleteMAC.SetAlpha(m_fCurAlpha);
    m_btnDeleteMAC.Render();
	m_iconDeleteMAC.SetAlpha(m_fCurAlpha);
    m_iconDeleteMAC.Render();

	m_btnDeleteCPCA.SetAlpha(m_fCurAlpha);
    m_btnDeleteCPCA.Render();
	m_iconDeleteCPCA.SetAlpha(m_fCurAlpha);
    m_iconDeleteCPCA.Render();

	m_btnCarPlayRestart.SetAlpha(m_fCurAlpha);
    m_btnCarPlayRestart.Render();
	m_iconCarPlayRestart.SetAlpha(m_fCurAlpha);
    m_iconCarPlayRestart.Render();

	m_tagReleaseDay.SetAlpha(m_fCurAlpha);
	m_tagReleaseDay.Render();

	m_tagH264Test.SetAlpha(m_fCurAlpha);
	m_tagH264Test.Render();

	m_btnAvccPath.SetAlpha(m_fCurAlpha);
	m_btnAvccPath.Render();

	m_tagValueAvccPath.SetAlpha(m_fCurAlpha);
	m_tagValueAvccPath.Render();

	m_btnH264Path.SetAlpha(m_fCurAlpha);
	m_btnH264Path.Render();

	m_tagValueH264Path.SetAlpha(m_fCurAlpha);
	m_tagValueH264Path.Render();

	m_btnH264Test.SetAlpha(m_fCurAlpha);
	m_btnH264Test.Render();

	m_tagUSBTest.SetAlpha(m_fCurAlpha);
	m_tagUSBTest.Render();

	m_tagUSBTestMode.SetAlpha(m_fCurAlpha);
	m_tagUSBTestMode.Render();

	m_btnUSBTestSpread.SetAlpha(m_fCurAlpha);
	m_btnUSBTestSpread.Render();

	m_iconUSBTest.SetAlpha(m_fCurAlpha);
	m_iconUSBTest.Render();

    if (m_bUsbTestSpread)
    {
		DrawRect(0,-tag_h*12+25,g_iClientWidth-LIST_BOX_ITEM_WIDTH-90,3,&cr_b);

		m_btnUSBTestModeDisable.SetAlpha(m_fCurAlpha);
		m_btnUSBTestModeDisable.Render();

		m_btnUSBTestModeJState.SetAlpha(m_fCurAlpha);
		m_btnUSBTestModeJState.Render();

		m_btnUSBTestModeKState.SetAlpha(m_fCurAlpha);
		m_btnUSBTestModeKState.Render();

		m_btnUSBTestModeSe0Nak.SetAlpha(m_fCurAlpha);
		m_btnUSBTestModeSe0Nak.Render();

		DrawRect(0,-tag_h*13+25,g_iClientWidth-LIST_BOX_ITEM_WIDTH-90,1,&cr_b);
		m_btnUSBTestModePacket.SetAlpha(m_fCurAlpha);
		m_btnUSBTestModePacket.Render();

		m_btnUSBTestModeForceHS.SetAlpha(m_fCurAlpha);
		m_btnUSBTestModeForceHS.Render();

		m_btnUSBTestModeForceFS.SetAlpha(m_fCurAlpha);
		m_btnUSBTestModeForceFS.Render();

		m_btnUSBTestModeForceLS.SetAlpha(m_fCurAlpha);
		m_btnUSBTestModeForceLS.Render();

		DrawRect(0,-tag_h*14+25,g_iClientWidth-LIST_BOX_ITEM_WIDTH-90,1,&cr_b);
		m_btniPerfServer.SetAlpha(m_fCurAlpha);
		m_btniPerfServer.Render();

		DrawRect(0,-tag_h*15+25,g_iClientWidth-LIST_BOX_ITEM_WIDTH-90,1,&cr_b);
		m_btniPerfClient.SetAlpha(m_fCurAlpha);
		m_btniPerfClient.Render();

		m_iconiPerfServer.SetAlpha(m_fCurAlpha);
		m_iconiPerfServer.Render();

		m_iconiPerfClient.SetAlpha(m_fCurAlpha);
		m_iconiPerfClient.Render();

    }


	EndRender();
}

BOOL CPanelSettingFactory::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//large size:
	SetSize(g_iClientWidth-box_size_x,g_iClientHeight+tag_h*9 - 8);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

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

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
    COLORGL cr_gray2={0.35,0.35,0.35,1.0};
	COLORGL cr_dark={0.0,0.0,0.0,1.0};
	COLORGL cr_green={0.0f/256.0f,144.0f/256.0f,255.0f/256.0f,1.0};
	COLORGL cr_b={0.0,161.0f/256.0f,228.0/256.0f,1.0};
	SIZE sizeValue={200,80};
    GLfloat py=g_iClientHeight/2-50;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.Initialize(0,this,
	m_pSkinManager->GetTexture(TEXID_065),NULL,
	m_pSkinManager->GetTexture(TEXID_065),NULL,
	NULL,NULL,
	NULL,NULL);
	m_tagBk.SetSize(cx, g_iClientHeight*4);
	m_tagBk.SetPos(0, -g_iClientHeight);
#endif

    m_tagTitle1.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle1.SetSize(cx,tag_h);
	m_tagTitle1.SetCaption(_T("工厂模式"));
	m_tagTitle1.LinkTextGL(pTextGL);
	m_tagTitle1.SetPos(10,py+12);
	m_tagTitle1.SetCaptionHeight(caption_h);
	m_tagTitle1.EnableDrawBack(FALSE);
	m_tagTitle1.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle1.SetCaptionColor(&cr_dark);

    m_tagTitle2.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle2.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitle2.SetCaption(_T("选择测试功能项，测试完毕请恢复出厂设置"));
	m_tagTitle2.LinkTextGL(pTextGL);
	m_tagTitle2.SetPos(10,py-16);
	m_tagTitle2.SetCaptionHeight(caption3_h);
	m_tagTitle2.EnableDrawBack(FALSE);
	m_tagTitle2.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle2.SetCaptionColor(&cr_gray2);

    //m_labIcon.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_623));
	//m_labIcon.SetPos(170+10+32,py);
//	    m_labIconFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_624));
//		m_labIconFocus.SetPos(170+10+32,py);
    //m_labIconFocus.Show(FALSE);

	m_btnReturn.Initialize(IDC_NB_BTN_RETURN,this,m_pSkinManager->GetTexture(TEXID_095),NULL,
		m_pSkinManager->GetTexture(TEXID_012),NULL,NULL,NULL,NULL,NULL);
	m_btnReturn.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnReturn.SetPos(170+30+32,py);
	m_btnReturn.SetTouchRate(2.0f,1.5f);
	m_btnReturn.EnableClickAni(FALSE);

    py-=tag_h+6;

	SIZE sizebtn={170,64};
	SIZE sizeinfo={350,64};

	m_tagSystemInfo.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagSystemInfo.SetSize(530,64);
	m_tagSystemInfo.SetCaption(_T("系统性能检测:"));
	m_tagSystemInfo.SetSubCaption(_T("CPU:--  内存:--  User used:--KB  System used:--KB"));
	m_tagSystemInfo.LinkTextGL(pTextGL);
// 	m_tagTime.SetPos(-348,215);
	m_tagSystemInfo.SetPos(0,py);
	m_tagSystemInfo.SetCaptionHeight(16);
//		m_tagSystemInfo.EnableDrawBack(FALSE);
	m_tagSystemInfo.SetCaptionPos(BTN_CAPTION_LEFT,60,8);
	m_tagSystemInfo.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);
	m_tagSystemInfo.SetCaptionColor(&cr_dark);

	m_btnCpu.Initialize(IDC_SETTING_BTN_CPU_MONITOR,this,m_pSkinManager->GetTexture(TEXID_821),NULL,
		m_pSkinManager->GetTexture(TEXID_822),NULL,m_pSkinManager->GetTexture(TEXID_822),NULL,NULL,NULL);
	m_btnCpu.SetPos(-170-10-62,py);
	m_btnCpu.SetTouchRate(2.0f,1.5f);
	m_btnCpu.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnCpu.EnableClickAni(FALSE);

    m_iconCpuFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconCpuFocus.SetPos(-170-10-62,py);
    m_iconCpuFocus.Show(FALSE);

	py-=tag_h;

    m_btnRearColorSignalType.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnRearColorSignalType.SetSize(530,64);
	m_btnRearColorSignalType.LinkTextGL(pTextGL);
	m_btnRearColorSignalType.SetPos(0,py);
	m_btnRearColorSignalType.SetCaptionHeight(16);
	m_btnRearColorSignalType.SetCaption(_T("Camera"));
	m_btnRearColorSignalType.SetCaptionPos(BTN_CAPTION_LEFT,60,8);
	m_btnRearColorSignalType.SetCaptionColor(&cr_dark, &cr_dark);
	m_btnRearColorSignalType.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	switch (m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard)
	{
	case OEMCONFIG_COLORSIGNAL_NTSM_MJ:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  NTSC-STD"));
		break;
    case OEMCONFIG_COLORSIGNAL_NTSC_443:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  NTSC-4.43M"));
		break;
	case OEMCONFIG_COLORSIGNAL_PAL_M:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  PAL-M"));
		break;
    case OEMCONFIG_COLORSIGNAL_PAL_60:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  PAL-60"));
		break;
    case OEMCONFIG_COLORSIGNAL_PAL_BGHID:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  PAL-STD"));
		break;
    case OEMCONFIG_COLORSIGNAL_PAL_COMBINATION_N:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  PAL-N"));
		break;
	case OEMCONFIG_COLORSIGNAL_SECAM:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  SECAM"));
		break;
	case OEMCONFIG_COLORSIGNAL_SECAM_525:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  SECAM-525"));
		break;
    default:
		m_btnRearColorSignalType.SetSubCaption(_T("倒车摄像头制式:  NTSC-STD"));
		break;
    }
	m_btnRearColorSignalType.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);

    m_iconRearColorSignalType.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconRearColorSignalType.SetPos(-170-10-62,py);
//
//		m_tagRearColorSignalType.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagRearColorSignalType.SetSize(100,64);
//		switch (m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard)
//		{
//		case OEMCONFIG_COLORSIGNAL_NTSM_MJ:
//			m_tagRearColorSignalType.SetCaption(_T("NTSC-STD"));
//			break;
//	    case OEMCONFIG_COLORSIGNAL_NTSC_443:
//			m_tagRearColorSignalType.SetCaption(_T("NTSC-4.43M"));
//			break;
//		case OEMCONFIG_COLORSIGNAL_PAL_M:
//			m_tagRearColorSignalType.SetCaption(_T("PAL-M"));
//			break;
//	    case OEMCONFIG_COLORSIGNAL_PAL_60:
//			m_tagRearColorSignalType.SetCaption(_T("PAL-60"));
//			break;
//	    case OEMCONFIG_COLORSIGNAL_PAL_BGHID:
//			m_tagRearColorSignalType.SetCaption(_T("PAL-STD"));
//			break;
//	    case OEMCONFIG_COLORSIGNAL_PAL_COMBINATION_N:
//			m_tagRearColorSignalType.SetCaption(_T("PAL-N"));
//			break;
//		case OEMCONFIG_COLORSIGNAL_SECAM:
//			m_tagRearColorSignalType.SetCaption(_T("SECAM"));
//			break;
//		case OEMCONFIG_COLORSIGNAL_SECAM_525:
//			m_tagRearColorSignalType.SetCaption(_T("SECAM-525"));
//			break;
//	    default:
//			m_tagRearColorSignalType.SetCaption(_T("NTSC-STD"));
//			break;
//	    }
//
//		m_tagRearColorSignalType.LinkTextGL(pTextGL);
//		m_tagRearColorSignalType.SetPos(0,py);
//		m_tagRearColorSignalType.SetCaptionHeight(16);
//		m_tagRearColorSignalType.EnableDrawBack(FALSE);
//		m_tagRearColorSignalType.SetCaptionPos(BTN_CAPTION_RIGHT);
//		m_tagRearColorSignalType.SetCaptionColor(&cr_dark);

	py-=tag_h;

    m_btnMediaAutoTest.Initialize(IDC_SETTING_BTN_MEDIA_TEST,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnMediaAutoTest.SetSize(&sizebtn);
	m_btnMediaAutoTest.SetPos(-170-10,py);
	m_btnMediaAutoTest.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnMediaAutoTest.SetCaption(_T("多媒体"));
    m_btnMediaAutoTest.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnMediaAutoTest.SetCaptionHeight(16);
    m_btnMediaAutoTest.SetSubCaption(_T("Stop"));
    m_btnMediaAutoTest.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);
    m_btnMediaAutoTest.LinkTextGL(pTextGL);
    m_btnMediaAutoTest.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconMediaAutoTest.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconMediaAutoTest.SetPos(-170-10-62,py);
    m_iconMediaAutoTestFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconMediaAutoTestFocus.SetPos(-170-10-62,py);
    m_iconMediaAutoTestFocus.Show(FALSE);

	m_tagTime.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTime.SetSize(300,64);
	m_tagTime.SetCaption(_T("00:00:00"));
	m_tagTime.LinkTextGL(pTextGL);
// 	m_tagTime.SetPos(-348,215);
	m_tagTime.SetPos(90,py);
	m_tagTime.SetCaptionHeight(16);
	m_tagTime.EnableDrawBack(FALSE);
	m_tagTime.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagTime.SetCaptionColor(&cr_dark);

    m_tagMediaAutoTestInfo.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    m_tagMediaAutoTestInfo.SetSize(&sizeinfo);
    m_tagMediaAutoTestInfo.SetPos(90,py);
    m_tagMediaAutoTestInfo.SetCaptionHeight(16);
	m_tagMediaAutoTestInfo.SetCaption(_T(""));
	m_tagMediaAutoTestInfo.SetCaptionColor(&cr_dark);
    m_tagMediaAutoTestInfo.LinkTextGL(pTextGL);
    m_tagMediaAutoTestInfo.SetCaptionPos(BTN_CAPTION_LEFT,20,0);

    py-=tag_h;

    m_btnTPAutoReCalibrate.Initialize(IDC_SETTING_BTN_TP_AUTO_RECALIBRATE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTPAutoReCalibrate.SetSize(&sizebtn);
	m_btnTPAutoReCalibrate.SetPos(-170-10,py);
	m_btnTPAutoReCalibrate.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnTPAutoReCalibrate.SetCaption(_T("TP"));
    m_btnTPAutoReCalibrate.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnTPAutoReCalibrate.SetCaptionHeight(16);
    m_btnTPAutoReCalibrate.SetSubCaption(_T("整机自动校正"));
    m_btnTPAutoReCalibrate.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);
    m_btnTPAutoReCalibrate.LinkTextGL(pTextGL);
    m_btnTPAutoReCalibrate.SetCaptionPos(BTN_CAPTION_LEFT,60,8);
	//m_btnTPAutoReCalibrate.Enable(FALSE);	//GT911 do not support carlibration mode

    m_tagTPAutoReCalibrateInfo.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    m_tagTPAutoReCalibrateInfo.SetSize(&sizeinfo);
    m_tagTPAutoReCalibrateInfo.SetPos(90,py);
    m_tagTPAutoReCalibrateInfo.SetCaptionHeight(16);
	m_tagTPAutoReCalibrateInfo.SetCaption(_T(""));
	m_tagTPAutoReCalibrateInfo.SetCaptionColor(&cr_dark);
    m_tagTPAutoReCalibrateInfo.LinkTextGL(pTextGL);
    m_tagTPAutoReCalibrateInfo.SetCaptionPos(BTN_CAPTION_LEFT,20,0);

    m_iconTPAutoReCalibrate.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconTPAutoReCalibrate.SetPos(-170-10-62,py);
    m_iconTPAutoReCalibrateFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconTPAutoReCalibrateFocus.SetPos(-170-10-62,py);
    m_iconTPAutoReCalibrateFocus.Show(FALSE);

	py-=tag_h;

    m_btnSnapshot.Initialize(IDC_SETTING_BTN_SNAPSHOT_ENABLE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnSnapshot.SetSize(&sizebtn);
	m_btnSnapshot.SetPos(-170-10,py);
	m_btnSnapshot.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnSnapshot.SetCaption(_T("截屏"));
    m_btnSnapshot.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnSnapshot.SetCaptionHeight(16);
    m_btnSnapshot.SetSubCaption(_T("关闭"));
    m_btnSnapshot.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);
    m_btnSnapshot.LinkTextGL(pTextGL);
    m_btnSnapshot.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconSnapshot.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconSnapshot.SetPos(-170-10-62,py);
    m_iconSnapshotFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconSnapshotFocus.SetPos(-170-10-62,py);
    m_iconSnapshotFocus.Show(FALSE);

//	    m_tagSnapshotInfo.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//	    m_tagSnapshotInfo.SetSize(&sizeinfo);
//	    m_tagSnapshotInfo.SetPos(90,py);
//	    m_tagSnapshotInfo.SetCaptionHeight(16);
//		m_tagSnapshotInfo.SetCaption(_T("..."));
//		m_tagSnapshotInfo.SetCaptionColor(&cr_dark);
//	    m_tagSnapshotInfo.LinkTextGL(pTextGL);
//	    m_tagSnapshotInfo.SetCaptionPos(BTN_CAPTION_LEFT,20,0);

    m_btnTouchAutoTest.Initialize(IDC_SETTING_BTN_TOUCH_TEST,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnTouchAutoTest.SetSize(&sizebtn);
	m_btnTouchAutoTest.SetPos(0,py);
	m_btnTouchAutoTest.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnTouchAutoTest.SetCaption(_T("触摸屏"));
    m_btnTouchAutoTest.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnTouchAutoTest.SetCaptionHeight(16);
    m_btnTouchAutoTest.SetSubCaption(_T("画线测试"));
    m_btnTouchAutoTest.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);
    m_btnTouchAutoTest.LinkTextGL(pTextGL);
    m_btnTouchAutoTest.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconTouchAutoTest.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconTouchAutoTest.SetPos(-62,py);
    m_iconTouchAutoTestFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconTouchAutoTestFocus.SetPos(-62,py);
    m_iconTouchAutoTestFocus.Show(FALSE);

//	    m_tagTouchAutoTestInfo.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//	    m_tagTouchAutoTestInfo.SetSize(&sizeinfo);
//	    m_tagTouchAutoTestInfo.SetPos(90,py);
//	    m_tagTouchAutoTestInfo.SetCaptionHeight(16);
//		m_tagTouchAutoTestInfo.SetCaption(_T("..."));
//		m_tagTouchAutoTestInfo.SetCaptionColor(&cr_dark);
//	    m_tagTouchAutoTestInfo.LinkTextGL(pTextGL);
//	    m_tagTouchAutoTestInfo.SetCaptionPos(BTN_CAPTION_LEFT,20,0);

    m_btnBluetoothAutoTest.Initialize(IDC_SETTING_BTN_BLUETOOTH_TEST,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnBluetoothAutoTest.SetSize(&sizebtn);
	m_btnBluetoothAutoTest.SetPos(170+10,py);
	m_btnBluetoothAutoTest.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnBluetoothAutoTest.SetCaption(_T("蓝牙"));
    m_btnBluetoothAutoTest.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnBluetoothAutoTest.SetCaptionHeight(16);
    m_btnBluetoothAutoTest.SetSubCaption(_T("暂不支持"));
    m_btnBluetoothAutoTest.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);
    m_btnBluetoothAutoTest.LinkTextGL(pTextGL);
    m_btnBluetoothAutoTest.SetCaptionPos(BTN_CAPTION_LEFT,60,8);
	//m_btnBluetoothAutoTest.Enable(FALSE);

    m_iconBluetoothAutoTest.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconBluetoothAutoTest.SetPos(170+10-62,py);
    m_iconBluetoothAutoTestFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconBluetoothAutoTestFocus.SetPos(170+10-62,py);
    m_iconBluetoothAutoTestFocus.Show(FALSE);

//	    m_tagBluetoothAutoTestInfo.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
//	    m_tagBluetoothAutoTestInfo.SetSize(&sizeinfo);
//	    m_tagBluetoothAutoTestInfo.SetPos(90,py);
//	    m_tagBluetoothAutoTestInfo.SetCaptionHeight(16);
//		m_tagBluetoothAutoTestInfo.SetCaption(_T("尚不支持"));
//		m_tagBluetoothAutoTestInfo.SetCaptionColor(&cr_dark);
//	    m_tagBluetoothAutoTestInfo.LinkTextGL(pTextGL);
//	    m_tagBluetoothAutoTestInfo.SetCaptionPos(BTN_CAPTION_LEFT,20,0);

	py-=tag_h;

    m_btnExplorer.Initialize(IDC_SETTING_BTN_EXPLORER,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnExplorer.SetSize(&sizebtn);
	m_btnExplorer.SetPos(-170-10,py);
	m_btnExplorer.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnExplorer.SetCaption(_T("Explorer"));
    m_btnExplorer.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnExplorer.SetCaptionHeight(16);
    m_btnExplorer.SetSubCaption(_T("WINCE界面"));
    m_btnExplorer.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);
    m_btnExplorer.LinkTextGL(pTextGL);
    m_btnExplorer.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconExplorer.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconExplorer.SetPos(-170-10-62,py);
    m_iconExplorerFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconExplorerFocus.SetPos(-170-10-62,py);
    m_iconExplorerFocus.Show(FALSE);

	//LED
    m_btnLED.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnLED.SetSize(&sizebtn);
	m_btnLED.SetPos(0,py);
	m_btnLED.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnLED.SetCaption(_T("LED"));
    m_btnLED.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnLED.SetCaptionHeight(16);
    m_btnLED.SetSubCaption(_T("暂不支持"));
    m_btnLED.SetSubCaptionPos(BTN_CAPTION_LEFT,60,0);
    m_btnLED.LinkTextGL(pTextGL);
    m_btnLED.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconLED.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconLED.SetPos(-62,py);
    m_iconLEDFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconLEDFocus.SetPos(-62,py);
    m_iconLEDFocus.Show(FALSE);

	m_btnConnectTest.Initialize(IDC_SETTING_BTN_CONNECT_TEST,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnConnectTest.SetSize(&sizebtn);
	m_btnConnectTest.SetPos(170+10,py);
	m_btnConnectTest.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnConnectTest.SetCaption(_T("连接测试"));
    m_btnConnectTest.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnConnectTest.SetCaptionHeight(16);
    m_btnConnectTest.SetSubCaption(_T("点击进入测试"));
    m_btnConnectTest.SetSubCaptionPos(BTN_CAPTION_LEFT,50,0);
	m_btnConnectTest.SetSubCaptionHeight(14);
    m_btnConnectTest.LinkTextGL(pTextGL);
    m_btnConnectTest.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconConnectTest.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconConnectTest.SetPos(170+10-62,py);

	py-=tag_h;

	m_btnDeleteMAC.Initialize(IDC_SETTING_BTN_DELETE_MAC,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnDeleteMAC.SetSize(&sizebtn);
	m_btnDeleteMAC.SetPos(-180,py);
	m_btnDeleteMAC.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnDeleteMAC.SetCaption(_T("删除MAC"));
    m_btnDeleteMAC.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnDeleteMAC.SetCaptionHeight(16);
    m_btnDeleteMAC.SetSubCaption(_T("点击删除"));
    m_btnDeleteMAC.SetSubCaptionPos(BTN_CAPTION_LEFT,50,0);
	m_btnDeleteMAC.SetSubCaptionHeight(14);
    m_btnDeleteMAC.LinkTextGL(pTextGL);
    m_btnDeleteMAC.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconDeleteMAC.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconDeleteMAC.SetPos(-180-62,py);

	m_btnDeleteCPCA.Initialize(IDC_SETTING_BTN_DELETE_CPCA,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnDeleteCPCA.SetSize(&sizebtn);
	m_btnDeleteCPCA.SetPos(0,py);
	m_btnDeleteCPCA.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnDeleteCPCA.SetCaption(_T("删除激活码"));
    m_btnDeleteCPCA.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnDeleteCPCA.SetCaptionHeight(16);
    m_btnDeleteCPCA.SetSubCaption(_T("点击删除"));
    m_btnDeleteCPCA.SetSubCaptionPos(BTN_CAPTION_LEFT,50,0);
	m_btnDeleteCPCA.SetSubCaptionHeight(14);
    m_btnDeleteCPCA.LinkTextGL(pTextGL);
    m_btnDeleteCPCA.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconDeleteCPCA.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconDeleteCPCA.SetPos(-62,py);

	m_btnCarPlayRestart.Initialize(IDC_SETTING_BTN_CARPLAY_RESTART,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
	m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnCarPlayRestart.SetSize(&sizebtn);
	m_btnCarPlayRestart.SetPos(180,py);
	m_btnCarPlayRestart.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
    m_btnCarPlayRestart.SetCaption(_T("重启CarPlay"));
    m_btnCarPlayRestart.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnCarPlayRestart.SetCaptionHeight(16);
    m_btnCarPlayRestart.SetSubCaption(_T("点击重启"));
    m_btnCarPlayRestart.SetSubCaptionPos(BTN_CAPTION_LEFT,50,0);
	m_btnCarPlayRestart.SetSubCaptionHeight(14);
    m_btnCarPlayRestart.LinkTextGL(pTextGL);
    m_btnCarPlayRestart.SetCaptionPos(BTN_CAPTION_LEFT,60,8);

    m_iconCarPlayRestart.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconCarPlayRestart.SetPos(180-62,py);

	py-=tag_h;

	BuildDateTime MyBuildDateTime;
	wchar_t str[64]={0};
	GetBuildDateTime(&MyBuildDateTime);
	swprintf_s(str,64,_T("%04d-%02d-%02d %02d:%02d"),MyBuildDateTime.Year,MyBuildDateTime.Month,MyBuildDateTime.Date,MyBuildDateTime.Hours,MyBuildDateTime.Minutes);

	m_tagReleaseDay.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_tagReleaseDay.SetSize(&sizebtn);
	m_tagReleaseDay.SetPos(180,py);
	m_tagReleaseDay.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_tagReleaseDay.SetCaption(_T("内部版本日期"));
	m_tagReleaseDay.SetCaptionColor(&cr_dark, &cr_dark);
	m_tagReleaseDay.SetCaptionHeight(16);
 	m_tagReleaseDay.SetSubCaption(str);
 	m_tagReleaseDay.SetSubCaptionPos(BTN_CAPTION_LEFT,20,0);
	m_tagReleaseDay.LinkTextGL(pTextGL);
	m_tagReleaseDay.SetCaptionPos(BTN_CAPTION_LEFT,20,8);


	py-=60+4;

	//USB Test:
	m_tagH264Test.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagH264Test.SetSize(cx,40);
	m_tagH264Test.SetCaption(_T("H.264 Test"));
	m_tagH264Test.LinkTextGL(pTextGL);
	m_tagH264Test.SetPos(20,py);
	m_tagH264Test.SetCaptionHeight(20);
	m_tagH264Test.EnableDrawBack(FALSE);
	m_tagH264Test.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagH264Test.SetCaptionColor(&cr_dark);

	py-=60+4;

	m_btnAvccPath.Initialize(IDC_SETTING_BTN_AVCC_PATH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAvccPath.SetSize((g_iClientWidth-240-margin_x*2),60);
	m_btnAvccPath.SetCaption(_T("AVCC File Path"));
	m_btnAvccPath.LinkTextGL(pTextGL);
	m_btnAvccPath.SetPos(0,py);
	m_btnAvccPath.SetCaptionHeight(20);
	m_btnAvccPath.SetCaptionPos(BTN_CAPTION_LEFT,10);
	m_btnAvccPath.SetCaptionColor(&cr_gray);
	m_btnAvccPath.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);

	m_tagValueAvccPath.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueAvccPath.LinkTextGL(pTextGL);
	// 	m_tagValueAvccPath.SetSize(cx,40);
	// 	m_tagValueAvccPath.SetPos(cx/2+10 -20,py);
	m_tagValueAvccPath.SetSize((g_iClientWidth-240-margin_x*2)-220,40);
	m_tagValueAvccPath.SetPos(110-10,py);
	m_tagValueAvccPath.SetCaptionHeight(18);
	m_tagValueAvccPath.SetCaption(strAvccPath.String(),TRUE);
	m_tagValueAvccPath.EnableDrawBack(FALSE);
	m_tagValueAvccPath.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueAvccPath.SetCaptionColor(&cr_gray);

	py-=tag_h;

	m_btnH264Path.Initialize(IDC_SETTING_BTN_H264_PATH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnH264Path.SetSize((g_iClientWidth-240-margin_x*2),60);
	m_btnH264Path.SetCaption(_T("H.264 File Path"));
	m_btnH264Path.LinkTextGL(pTextGL);
	m_btnH264Path.SetPos(0,py);
	m_btnH264Path.SetCaptionHeight(20);
	m_btnH264Path.SetCaptionPos(BTN_CAPTION_LEFT,10);
	m_btnH264Path.SetCaptionColor(&cr_gray);
	m_btnH264Path.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);

	m_tagValueH264Path.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueH264Path.LinkTextGL(pTextGL);
	// 	m_tagValueH264Path.SetSize(cx,40);
	// 	m_tagValueH264Path.SetPos(cx/2+10 -20,py);
	m_tagValueH264Path.SetSize((g_iClientWidth-240-margin_x*2)-220,40);
	m_tagValueH264Path.SetPos(110-10,py);
	m_tagValueH264Path.SetCaptionHeight(18);
	m_tagValueH264Path.SetCaption(strH264Path.String(),TRUE);
	m_tagValueH264Path.EnableDrawBack(FALSE);
	m_tagValueH264Path.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueH264Path.SetCaptionColor(&cr_gray);

	py-=tag_h;

	m_btnH264Test.Initialize(IDC_SETTING_BTN_H264_TEST,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnH264Test.SetSize((g_iClientWidth-240-margin_x*2),60);
	m_btnH264Test.SetCaption(_T("H.264 Test"));
	m_btnH264Test.LinkTextGL(pTextGL);
	m_btnH264Test.SetPos(0,py);
	m_btnH264Test.SetCaptionHeight(20);
	m_btnH264Test.SetCaptionPos(BTN_CAPTION_LEFT,10);
	m_btnH264Test.SetCaptionColor(&cr_gray);
	m_btnH264Test.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);

	py-=60+4;

	//USB Test:
	m_tagUSBTest.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagUSBTest.SetSize(cx,40);
	m_tagUSBTest.SetCaption(_T("USB Test"));
	m_tagUSBTest.LinkTextGL(pTextGL);
	m_tagUSBTest.SetPos(20,py);
	m_tagUSBTest.SetCaptionHeight(20);
	m_tagUSBTest.EnableDrawBack(FALSE);
	m_tagUSBTest.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagUSBTest.SetCaptionColor(&cr_dark);

	py-=tag_h;

	m_tagUSBTestMode.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagUSBTestMode.SetSize(530,64);
	m_tagUSBTestMode.SetPos(0,py);
	m_tagUSBTestMode.SetCaption(_T("USB测试模式"));
	m_tagUSBTestMode.LinkTextGL(pTextGL);
	m_tagUSBTestMode.SetCaptionHeight(caption_h);
	m_tagUSBTestMode.SetCaptionPos(BTN_CAPTION_LEFT,60);
	m_tagUSBTestMode.SetCaptionColor(&cr_gray, &cr_b);

	m_iconUSBTest.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
	m_iconUSBTest.SetPos(-242,py);

	m_btnUSBTestSpread.Initialize(IDC_SETTING_USB_TEST_MODE_SPREAD,this,m_pSkinManager->GetTexture(TEXID_080),NULL,
		NULL,NULL,m_pSkinManager->GetTexture(TEXID_081),NULL,NULL,NULL);
	m_btnUSBTestSpread.SetPos(200,py);
	m_btnUSBTestSpread.SetTouchRate(2.0f,1.5f);
	m_btnUSBTestSpread.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);


	py-=tag_h;

	m_btnUSBTestModeDisable.Initialize(IDC_SETTING_BTN_USB_TEST_MODE_DISABLE,this,m_pSkinManager->GetTexture(TEXID_642),NULL,
		m_pSkinManager->GetTexture(TEXID_643),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUSBTestModeDisable.SetCaption(_T("Disable"));
	m_btnUSBTestModeDisable.LinkTextGL(pTextGL);
	m_btnUSBTestModeDisable.SetPos(-200,py);
	m_btnUSBTestModeDisable.SetCaptionHeight(16);
	m_btnUSBTestModeDisable.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUSBTestModeDisable.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUSBTestModeDisable.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnUSBTestModeDisable.Show(FALSE);

	m_btnUSBTestModeJState.Initialize(IDC_SETTING_BTN_USB_TEST_MODE_J_STATE,this,m_pSkinManager->GetTexture(TEXID_642),NULL,
		m_pSkinManager->GetTexture(TEXID_643),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUSBTestModeJState.SetCaption(_T("J State"));
	m_btnUSBTestModeJState.LinkTextGL(pTextGL);
	m_btnUSBTestModeJState.SetPos(-70,py);
	m_btnUSBTestModeJState.SetCaptionHeight(16);
	m_btnUSBTestModeJState.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUSBTestModeJState.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUSBTestModeJState.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnUSBTestModeJState.Show(FALSE);

	m_btnUSBTestModeKState.Initialize(IDC_SETTING_BTN_USB_TEST_MODE_K_STATE,this,m_pSkinManager->GetTexture(TEXID_642),NULL,
		m_pSkinManager->GetTexture(TEXID_643),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUSBTestModeKState.SetCaption(_T("K State"));
	m_btnUSBTestModeKState.LinkTextGL(pTextGL);
	m_btnUSBTestModeKState.SetPos(60,py);
	m_btnUSBTestModeKState.SetCaptionHeight(16);
	m_btnUSBTestModeKState.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUSBTestModeKState.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUSBTestModeKState.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnUSBTestModeKState.Show(FALSE);

	m_btnUSBTestModeSe0Nak.Initialize(IDC_SETTING_BTN_USB_TEST_MODE_SE0_NAK,this,m_pSkinManager->GetTexture(TEXID_642),NULL,
		m_pSkinManager->GetTexture(TEXID_643),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUSBTestModeSe0Nak.SetCaption(_T("SE0/NAK"));
	m_btnUSBTestModeSe0Nak.LinkTextGL(pTextGL);
	m_btnUSBTestModeSe0Nak.SetPos(190,py);
	m_btnUSBTestModeSe0Nak.SetCaptionHeight(16);
	m_btnUSBTestModeSe0Nak.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUSBTestModeSe0Nak.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUSBTestModeSe0Nak.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnUSBTestModeSe0Nak.Show(FALSE);

	py-=tag_h;

	m_btnUSBTestModePacket.Initialize(IDC_SETTING_BTN_USB_TEST_MODE_PACKET,this,m_pSkinManager->GetTexture(TEXID_642),NULL,
		m_pSkinManager->GetTexture(TEXID_643),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUSBTestModePacket.SetCaption(_T("Packet"));
	m_btnUSBTestModePacket.LinkTextGL(pTextGL);
	m_btnUSBTestModePacket.SetPos(-200,py);
	m_btnUSBTestModePacket.SetCaptionHeight(16);
	m_btnUSBTestModePacket.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUSBTestModePacket.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUSBTestModePacket.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnUSBTestModePacket.Show(FALSE);

	m_btnUSBTestModeForceHS.Initialize(IDC_SETTING_BTN_USB_TEST_MODE_FORCE_HS,this,m_pSkinManager->GetTexture(TEXID_642),NULL,
		m_pSkinManager->GetTexture(TEXID_643),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUSBTestModeForceHS.SetCaption(_T("Force HS"));
	m_btnUSBTestModeForceHS.LinkTextGL(pTextGL);
	m_btnUSBTestModeForceHS.SetPos(-70,py);
	m_btnUSBTestModeForceHS.SetCaptionHeight(16);
	m_btnUSBTestModeForceHS.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUSBTestModeForceHS.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUSBTestModeForceHS.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnUSBTestModeForceHS.Show(FALSE);

	m_btnUSBTestModeForceFS.Initialize(IDC_SETTING_BTN_USB_TEST_MODE_FORCE_FS,this,m_pSkinManager->GetTexture(TEXID_642),NULL,
		m_pSkinManager->GetTexture(TEXID_643),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUSBTestModeForceFS.SetCaption(_T("Force FS"));
	m_btnUSBTestModeForceFS.LinkTextGL(pTextGL);
	m_btnUSBTestModeForceFS.SetPos(60,py);
	m_btnUSBTestModeForceFS.SetCaptionHeight(16);
	m_btnUSBTestModeForceFS.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUSBTestModeForceFS.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUSBTestModeForceFS.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnUSBTestModeForceFS.Show(FALSE);

	m_btnUSBTestModeForceLS.Initialize(IDC_SETTING_BTN_USB_TEST_MODE_FORCE_LS,this,m_pSkinManager->GetTexture(TEXID_642),NULL,
		m_pSkinManager->GetTexture(TEXID_643),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnUSBTestModeForceLS.SetCaption(_T("Force LS"));
	m_btnUSBTestModeForceLS.LinkTextGL(pTextGL);
	m_btnUSBTestModeForceLS.SetPos(190,py);
	m_btnUSBTestModeForceLS.SetCaptionHeight(16);
	m_btnUSBTestModeForceLS.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnUSBTestModeForceLS.SetCaptionColor(&cr_w,&cr_gray);
	m_btnUSBTestModeForceLS.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btnUSBTestModeForceLS.Show(FALSE);

	py-=tag_h;

//  	m_btniPerfServer.Initialize(IDC_SETTING_BTN_IPERF_SERVER_TEST,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
//  		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btniPerfServer.Initialize(IDC_SETTING_BTN_IPERF_SERVER_TEST,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btniPerfServer.SetSize(530,64);
	m_btniPerfServer.SetPos(0,py);
	m_btniPerfServer.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btniPerfServer.SetCaption(_T("iPerf Server Test"));
	m_btniPerfServer.SetCaptionColor(&cr_dark, &cr_dark);
	m_btniPerfServer.SetCaptionHeight(16);
	m_btniPerfServer.LinkTextGL(pTextGL);
	m_btniPerfServer.SetCaptionPos(BTN_CAPTION_LEFT,60);
	m_btniPerfServer.Show(FALSE);
	m_btniPerfServer.EnableDrawBack(FALSE);

	m_iconiPerfServer.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
	m_iconiPerfServer.SetPos(-242,py);

	py-=tag_h;

// 	m_btniPerfClient.Initialize(IDC_SETTING_BTN_IPERF_CLIENT_TEST,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
// 		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btniPerfClient.Initialize(IDC_SETTING_BTN_IPERF_CLIENT_TEST,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btniPerfClient.SetSize(530,64);
	m_btniPerfClient.SetPos(0,py);
	m_btniPerfClient.SetNotifyFunc(&CPanelSettingFactory::OnNotify,this);
	m_btniPerfClient.SetCaption(_T("iPerf Client Test"));
	m_btniPerfClient.SetCaptionColor(&cr_dark, &cr_dark);
	m_btniPerfClient.SetCaptionHeight(16);
	m_btniPerfClient.LinkTextGL(pTextGL);
	m_btniPerfClient.SetCaptionPos(BTN_CAPTION_LEFT,60);
	m_btniPerfClient.Show(FALSE);
	m_btniPerfClient.EnableDrawBack(FALSE);

	m_iconiPerfClient.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
	m_iconiPerfClient.SetPos(-242,py);

	InitializeCriticalSection(&m_csMonitor);

	m_hActivateResetTouchEvent = CreateEvent(0, FALSE, FALSE, RESET_TOUCH_EVENT_NAME);
	if(m_hActivateResetTouchEvent == NULL || m_hActivateResetTouchEvent == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent[RESET_TOUCH_EVENT_NAME] failed.\r\n"), _T(__FUNCTION__)));
		return FALSE;
	}

	m_hResetTouchFeedbackEvent = CreateEvent(0, FALSE, FALSE, RESET_TOUCH_FEEDBACK_EVENT_NAME);
	if(m_hResetTouchFeedbackEvent == NULL || m_hResetTouchFeedbackEvent == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent[RESET_TOUCH_FEEDBACK_EVENT_NAME] failed.\r\n"), _T(__FUNCTION__)));
		return FALSE;
	}

	m_hResetTouchStart = CreateEvent(0, FALSE, FALSE, NULL);
	if(m_hResetTouchStart == NULL || m_hResetTouchStart == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent[m_hResetTouchStart] failed.\r\n"), _T(__FUNCTION__)));
		return FALSE;
	}

	m_hResetTouchThread = CreateThread(NULL, 0, CPanelSettingFactory::TPAutoRecalibrateThread, this, 0, NULL);
	if(m_hResetTouchThread == NULL || m_hResetTouchThread == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateThread[TPAutoRecalibrateThread] failed.\r\n"), _T(__FUNCTION__)));
		return FALSE;
	}

	m_hCpuMonitorEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(m_hCpuMonitorEvent == NULL || m_hCpuMonitorEvent == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateEvent[m_hCpuMonitorEvent] failed.\r\n"), _T(__FUNCTION__)));
		return FALSE;
	}

	m_hCpuMonitorThread = CreateThread(NULL, 0, CPanelSettingFactory::SystemMonitorThread, this, 0, NULL);
	if(m_hCpuMonitorThread == NULL || m_hCpuMonitorThread == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: CreateThread[SystemMonitorThread] failed.\r\n"), _T(__FUNCTION__)));
		return FALSE;
	}
	return TRUE;
}


void CALLBACK CPanelSettingFactory::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingFactory *panel=(CPanelSettingFactory*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

BOOL   CPanelSettingFactory::IsFileExists(LPCTSTR   lpszFileName)
{
	//
	WIN32_FIND_DATA   wfd;
	HANDLE   hFind;
	BOOL   bRet;

	hFind   =   FindFirstFile(lpszFileName,   &wfd);
	bRet   =   (hFind   !=   INVALID_HANDLE_VALUE);
	FindClose(hFind);
	return   bRet;
}

void CPanelSettingFactory::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingFactory::HandleControlMSG] idControl:%d wMsg:%d wPara:%d\r\n"),idControl,wMsg,wPara));

	switch (idControl)
	{
		case IDC_NB_BTN_RETURN:
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_NONE,PAGE_SETTINGS_ABOUT);
			break;
        case IDC_SETTING_BTN_MEDIA_TEST:
            {
                if(m_bMediaTesting)
                {
                    m_bMediaTesting = FALSE;
                    SendFactoryCommand(FACTORY_CMD_MEDIA_TEST_VIDEO,FACTORY_CMD_TEST_STOP);
                    m_btnMediaAutoTest.SetSubCaption(_T("Stop"));
					//m_tagTime.SetCaption(_T("00:00:00"));
                    m_iconMediaAutoTest.Show(TRUE);
                    m_iconMediaAutoTestFocus.Show(FALSE);
                    SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_STOP);
                    if(m_pCommander)
                	{
                		m_pCommander->SendCmd(_ARM_BREAKE_TEST_KEY,TRUE);
                	}
                }
                else
                {
                    m_bMediaTesting = TRUE;
					GetLocalTime(&m_SystimeStart);
        			SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_DEFAULT,PAGE_MEDIA_MAIN);
                    SendFactoryCommand(FACTORY_CMD_MEDIA_TEST_VIDEO,FACTORY_CMD_TEST_START);
                    m_btnMediaAutoTest.SetSubCaption(_T("Testing"));
                    m_iconMediaAutoTest.Show(FALSE);
                    m_iconMediaAutoTestFocus.Show(TRUE);
                    if(m_pCommander)
                	{
                		m_pCommander->SendCmd(_ARM_BREAKE_TEST_KEY,FALSE);
                	}
                }
            }
            break;
        case IDC_SETTING_BTN_BLUETOOTH_TEST:
            {

            }
            break;
        case IDC_SETTING_BTN_TOUCH_TEST:
            {
#if 0
				SwitchPage(UI_CLASS_TOUCH,PAGE_TOUCH_MAIN,PAGE_SWITCH_ANI_DEFAULT,PAGE_TOUCH_MAIN);
#else
                if(m_bTouchTesting)
                {
                    //SendFactoryCommand(FACTORY_CMD_TOUCH_TEST,FACTORY_CMD_TEST_STOP);
                    m_iconTouchAutoTest.Show(TRUE);
                    m_iconTouchAutoTestFocus.Show(FALSE);
                    //m_btnTouchAutoTest.SetSubCaption(_T("Stop"));
                    m_bTouchTesting = FALSE;
//	        		PROCESS_INFORMATION pi;
//		            CM_String strFile=CM_String(CM_String(_T("USB Disk\\TPTest.exe")));
//	        		if(CreateProcess(strFile.String(),NULL,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))//set correct location:
//	        		{
//	                    RETAILMSG(1, (TEXT("Run USB Disk TPTest.exe successfully\r\n")));
//	                }
//	                else
//	                {
//	                    RETAILMSG(1, (TEXT("Run USB Disk TPTest.exe failed.\r\n")));
//	                }
                }
                else
                {
                    SwitchPage(UI_CLASS_TOUCH,PAGE_TOUCH_MAIN,PAGE_SWITCH_ANI_DEFAULT,PAGE_TOUCH_MAIN);
					//SendFactoryCommand(FACTORY_CMD_TOUCH_TEST,FACTORY_CMD_TEST_START);
                    //m_btnTouchAutoTest.SetSubCaption(_T("Testing"));
                    m_iconTouchAutoTest.Show(FALSE);
                    m_iconTouchAutoTestFocus.Show(TRUE);
                    m_bTouchTesting = TRUE;
                }
#endif
            }
            break;
		case IDC_SETTING_BTN_TP_AUTO_RECALIBRATE:
			{
				if(!m_bTpAutoRecalibrating)
				{
					if(m_hActivateResetTouchEvent)
					{
						SetEvent(m_hResetTouchStart);
						m_bTpAutoRecalibrating = TRUE;
					    m_btnTPAutoReCalibrate.Enable(FALSE);
					    m_iconTPAutoReCalibrate.Show(FALSE);
					    m_iconTPAutoReCalibrateFocus.Show(TRUE);
					}
				}
			}
			break;
		case IDC_SETTING_BTN_SNAPSHOT_ENABLE:
			{
				if(m_pSkinManager->GetConfig()->GetCurParam()->bSnapshotEnable)
				{
					m_pSkinManager->GetConfig()->GetCurParam()->bSnapshotEnable = FALSE;
					m_btnSnapshot.SetSubCaption(_T("关闭"));
					m_iconSnapshot.Show(TRUE);
					m_iconSnapshotFocus.Show(FALSE);
				}
				else
				{
					m_pSkinManager->GetConfig()->GetCurParam()->bSnapshotEnable = TRUE;
					m_btnSnapshot.SetSubCaption(_T("激活"));
					m_iconSnapshot.Show(FALSE);
					m_iconSnapshotFocus.Show(TRUE);
				}
			}
			break;
		case IDC_SETTING_BTN_EXPLORER:
			{
//					if(m_bExplorerIsRunning)
//					{
//
//					}
//					else
				{
					CM_String strFile=CM_String(_T("\\Windows\\explorer.exe"));

					//start APP
//						SHELLEXECUTEINFO ShExeInfo={0};
//						ShExeInfo.cbSize=sizeof(SHELLEXECUTEINFO);
//						ShExeInfo.fMask=SEE_MASK_NOCLOSEPROCESS;
//						ShExeInfo.hwnd=NULL;
//						ShExeInfo.lpVerb=NULL;
//						ShExeInfo.lpFile = strFile.String();
//						ShExeInfo.lpParameters=NULL;
//						ShExeInfo.lpDirectory=NULL;
//						ShExeInfo.nShow=SW_SHOW;
//						ShExeInfo.hInstApp=NULL;
					PROCESS_INFORMATION pi;

					if(CreateProcess(strFile.String(),NULL,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))//set correct location:
					{
						m_bExplorerIsRunning = TRUE;
						RETAILMSG(1, (TEXT("MSG:[%s]: CreateProcess SUCCEED:%s\r\n"),TEXT(__FUNCTION__),strFile.String()));
					}
//						if(!ShellExecuteEx(&ShExeInfo))
//						{
//							RETAILMSG(1, (TEXT("ERR:[%s]: ShellExecuteEx %s failed!!!\r\n"),TEXT(__FUNCTION__),ShExeInfo.lpFile));
//						}

				}
			}
			break;
		case IDC_SETTING_BTN_CONNECT_TEST:
			{
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_CONNECT_TEST);
			}
			break;
		case IDC_SETTING_BTN_DELETE_MAC:
			{
				ShowNotificationBox(m_hMainWnd,IDC_SETTING_BTN_DELETE_MAC_CONFIRM,
				&CPanelSettingFactory::OnNotify,
				this,
				0,
				_T("Delete MAC Address"),
				_T("Device will automatically restart after deleting"),
				0);
			}
			break;
		case IDC_SETTING_BTN_DELETE_MAC_CONFIRM:
			{
#if CVTE_EN_CPMAC
				if(wMsg == NB_OK)//
				{
					CCarplayAdapter::GetInstance()->DeleteMAC();
					::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
				}
#endif
			}
			break;
		case IDC_SETTING_BTN_DELETE_CPCA:
			{
#if CVTE_EN_CPCA
				ShowNotificationBox(m_hMainWnd,IDC_SETTING_BTN_DELETE_CPCA_CONFIRM,
				&CPanelSettingFactory::OnNotify,
				this,
				0,
				_T("Delete CarPlay CA"),
				_T("After Deleting will automatically restart"),
				0);
#endif
			}
			break;
		case IDC_SETTING_BTN_DELETE_CPCA_CONFIRM:
			{
#if CVTE_EN_CPCA
				if(wMsg == NB_OK)//
				{
					CCarplayAdapter::GetInstance()->DeleteCA();
					::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
				}
#endif
			}
			break;
		case IDC_SETTING_BTN_CARPLAY_RESTART:
			{
#if CVTE_EN_CARPLAY
				CCarplayAdapter::GetInstance()->Restart();
#endif
			}
			break;
		case IDC_SETTING_BTN_CPU_MONITOR:
			{
				if(m_bCpuMonitorActivate)
				{
					m_bCpuMonitorActivate = FALSE;
					m_dwCpuMonitorTimeout = INFINITE;
					m_iconCpuFocus.Show(FALSE);
					//m_tagSystemInfo.SetCaption(_T("系统性能检测:"));
					m_tagSystemInfo.SetSubCaption(_T("CPU:--  内存:--  User used:--KB  System used:--KB"));

				}
				else
				{
					m_bCpuMonitorActivate = TRUE;
					m_dwCpuMonitorTimeout = 100;
					m_iconCpuFocus.Show(TRUE);
				}
				SetEvent(m_hCpuMonitorEvent);
			}
			break;
		case IDC_SETTING_BTN_AVCC_PATH:
			{
				RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingFactory::HandleControlMSG]: IDC_SETTING_BTN_AVCC_PATH: \r\n")));
				ShowExplorer(IDC_PANEL_EXPLORER+IDC_SETTING_BTN_AVCC_PATH,&CPanelSettingFactory::OnNotify,this,
					_T("AVCC File Path"),
					_T("\\"),_T("avcc"));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_H264_PATH:
			{
				RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingFactory::HandleControlMSG]: IDC_SETTING_BTN_H264_PATH: \r\n")));
				ShowExplorer(IDC_PANEL_EXPLORER+IDC_SETTING_BTN_H264_PATH,&CPanelSettingFactory::OnNotify,this,
					_T("H.264 File Path"),
					_T("\\"),_T("h264"));
				PlaySoundEffect(0);
			}
			break;
		case IDC_PANEL_EXPLORER+IDC_SETTING_BTN_AVCC_PATH:
			{
				RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingFactory::HandleControlMSG]: IDC_PANEL_EXPLORER+IDC_SETTING_BTN_AVCC_PATH: lpPara=%s\r\n"),(const wchar_t*)lpPara));
				if(lpPara)
				{
					strAvccPath=(const wchar_t*)lpPara;

					m_tagValueAvccPath.SetCaption(strAvccPath.String(),TRUE);
				}
				PlaySoundEffect(0);
			}
			break;
		case IDC_PANEL_EXPLORER+IDC_SETTING_BTN_H264_PATH:
			{
				RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingFactory::HandleControlMSG]: IDC_PANEL_EXPLORER+IDC_SETTING_BTN_H264_PATH: lpPara=%s\r\n"),(const wchar_t*)lpPara));
				if(lpPara)
				{
					strH264Path=(const wchar_t*)lpPara;

					m_tagValueH264Path.SetCaption(strH264Path.String(),TRUE);
				}
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_H264_TEST:
			{
				if (IsFileExists(strAvccPath.String()) &&  IsFileExists(strH264Path.String()))
				{
					//terminate media player first:
					SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_TERMINATE);
#if CVTE_EN_CARPLAY
					CCarplayAdapter::GetInstance()->StartH264Test(strAvccPath.String(), strH264Path.String());
#endif
				}
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_USB_TEST_MODE_SPREAD:
			{
				#if CVTE_EN_CARPLAY
				if (FALSE == m_bUsbTestSpread)
				{
					m_bUsbTestSpread = TRUE;
					CCarplayAdapter::GetInstance()->SetTestMode(m_bUsbTestSpread);
					SetTestMode(CCarplayAdapter::GetInstance()->GetTestMode());
					m_tagUSBTestMode.SetSize(530,365);
					m_tagUSBTestMode.SetPosIni(0,-tag_h*14+62);
					m_tagUSBTestMode.SetCaptionPos(BTN_CAPTION_LEFT,60,tag_h*2);
					SetSize(g_iClientWidth-box_size_x,g_iClientHeight+tag_h*13);
					SetPos(100,g_iClientHeight-60);
				}
				else
				{
					m_bUsbTestSpread = FALSE;
					CCarplayAdapter::GetInstance()->SetTestMode(m_bUsbTestSpread);
					SetTestMode(CCarplayAdapter::GetInstance()->GetTestMode());
					m_tagUSBTestMode.SetSize(530,64);
					m_tagUSBTestMode.SetPosIni(0,-tag_h*12+62);
					m_tagUSBTestMode.SetCaptionPos(BTN_CAPTION_LEFT,60);
					SetSize(g_iClientWidth-box_size_x,g_iClientHeight+tag_h*9);
					SetPos(100,g_iClientHeight-360);

				}
				#endif
			}
			break;
		case IDC_SETTING_BTN_USB_TEST_MODE_DISABLE:
		case IDC_SETTING_BTN_USB_TEST_MODE_J_STATE:
		case IDC_SETTING_BTN_USB_TEST_MODE_K_STATE:
		case IDC_SETTING_BTN_USB_TEST_MODE_SE0_NAK:
		case IDC_SETTING_BTN_USB_TEST_MODE_PACKET:
		case IDC_SETTING_BTN_USB_TEST_MODE_FORCE_HS:
		case IDC_SETTING_BTN_USB_TEST_MODE_FORCE_FS:
		case IDC_SETTING_BTN_USB_TEST_MODE_FORCE_LS:
			{
#if CVTE_EN_CARPLAY
				CarplayAPITestModeType testmode;
				testmode = (CarplayAPITestModeType)(idControl-IDC_SETTING_BTN_USB_TEST_MODE_DISABLE);
				CCarplayAdapter::GetInstance()->SetUSBTestMode(testmode);
#endif
			}
			break;
		case IDC_SETTING_BTN_IPERF_SERVER_TEST:
			{
#if CVTE_EN_CARPLAY
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_IPERF_SERVER);
				PlaySoundEffect(0);
#endif
			}
			break;
		case IDC_SETTING_BTN_IPERF_CLIENT_TEST:
			{
#if CVTE_EN_CARPLAY
				CCarplayAdapter::GetInstance()->UpdateSessionAddress();
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_IPERF_CLIENT);
				PlaySoundEffect(0);
#endif
			}
			break;
		default:
			break;
	}
}

void CPanelSettingFactory::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);
    //BYTE  mcudata;
	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}

    //RETAILMSG(1,(_T("MSG:[%s]: dwType:%d,dwID:0x%x,val:0x%x\r\n"),TEXT(__FUNCTION__),dwType,dwID,pCommander->GetItemData(dwID)));


	if(dwType == 0)//package
	{
	}
	else if(dwType == 1)//single command
	{
	}
}

DWORD CPanelSettingFactory::TPAutoRecalibrateThread(LPVOID pDevice)
{
	CPanelSettingFactory* pCPanel = (CPanelSettingFactory*)pDevice;
	while(1)
	{
		WaitForSingleObject(pCPanel->m_hResetTouchStart,INFINITE);
		pCPanel->TPAutoRecalibrateProc();
	}
	return 0;
}

void CPanelSettingFactory::TPAutoRecalibrateProc()
{
	DWORD dwWait;
	do
	{
		Sleep(1000);
		m_tagTPAutoReCalibrateInfo.SetCaption(_T("在校正完成前，请勿触碰屏幕 5s"));
		Sleep(1000);
		m_tagTPAutoReCalibrateInfo.SetCaption(_T("在校正完成前，请勿触碰屏幕 4s"));
		Sleep(1000);
		m_tagTPAutoReCalibrateInfo.SetCaption(_T("在校正完成前，请勿触碰屏幕 3s"));
		Sleep(1000);
		m_tagTPAutoReCalibrateInfo.SetCaption(_T("在校正完成前，请勿触碰屏幕 2s"));
		Sleep(1000);
		m_tagTPAutoReCalibrateInfo.SetCaption(_T("在校正完成前，请勿触碰屏幕 1s"));
		Sleep(1000);
		m_tagTPAutoReCalibrateInfo.SetCaption(_T("校正中...请勿触碰屏幕"));

		SetEvent(m_hActivateResetTouchEvent);

		dwWait = WaitForSingleObject(m_hResetTouchFeedbackEvent, 10000);

		if(dwWait == WAIT_OBJECT_0)
		{
			DWORD dwResult = GetEventData(m_hResetTouchFeedbackEvent);
			if(dwResult == 5)
			{
				m_tagTPAutoReCalibrateInfo.SetCaption(_T("整机校准完毕"));
			}
			else
			{
				WCHAR str[MAX_PATH];
				swprintf_s(str,MAX_PATH,_T("整机校准失败 错误代码%d"),dwResult);
				m_tagTPAutoReCalibrateInfo.SetCaption(str);
			}
		}
		else
		{
			m_tagTPAutoReCalibrateInfo.SetCaption(_T("超时，整机校准失败"));
		}

		m_btnTPAutoReCalibrate.Enable(TRUE);
		m_iconTPAutoReCalibrate.Show(TRUE);
		m_iconTPAutoReCalibrateFocus.Show(FALSE);
	}
	while(0);
	m_bTpAutoRecalibrating = FALSE;

}

void CPanelSettingFactory::OnRearColorSignalChange(UINT uColorSignal)
{
	m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard = uColorSignal;
    RETAILMSG(1, (TEXT("MSG:[CPanelSettingFactory::OnRearColorSignalChange] uColorSignal:%d\r\n"),uColorSignal));

	switch (m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard)
	{
	case OEMCONFIG_COLORSIGNAL_NTSM_MJ:
		m_tagRearColorSignalType.SetCaption(_T("NTSC(STD)"));
		break;
    case OEMCONFIG_COLORSIGNAL_NTSC_443:
		m_tagRearColorSignalType.SetCaption(_T("NTSC(4.43M)"));
		break;
	case OEMCONFIG_COLORSIGNAL_PAL_M:
		m_tagRearColorSignalType.SetCaption(_T("PAL(M)"));
		break;
    case OEMCONFIG_COLORSIGNAL_PAL_60:
		m_tagRearColorSignalType.SetCaption(_T("PAL(60)"));
		break;
    case OEMCONFIG_COLORSIGNAL_PAL_BGHID:
		m_tagRearColorSignalType.SetCaption(_T("PAL(STD)"));
		break;
    case OEMCONFIG_COLORSIGNAL_PAL_COMBINATION_N:
		m_tagRearColorSignalType.SetCaption(_T("PAL(N)"));
		break;
	case OEMCONFIG_COLORSIGNAL_SECAM_525:
		m_tagRearColorSignalType.SetCaption(_T("SECAM"));
		break;
    default:
		m_tagRearColorSignalType.SetCaption(_T("NTSC"));
		break;
    }

}

void CPanelSettingFactory::UpdateTime()
{
	static DWORD last_tick=0;
	DWORD this_tick=GetTickCount();

	if(this_tick-last_tick>1000)//update per second
	{
		SYSTEMTIME sys_time;
		//int hour1,hour2,min1,min2;
		wchar_t str[16]={0};
// 		RETAILMSG(DEBUG_STATUSBAR,(_T("UpdateTime...\r\n")));
		last_tick=this_tick;
		GetLocalTime(&sys_time);

		DWORD nowtime = sys_time.wHour * 3600 + sys_time.wMinute*60 + sys_time.wSecond;
		DWORD countime = nowtime - (m_SystimeStart.wHour*3600+m_SystimeStart.wMinute*60+m_SystimeStart.wSecond);
		int   hour = countime/3600;
		int   minute = countime/60;
		int   second = countime%60;

		swprintf_s(str,16,_T("%02d:%02d:%02d"),hour,minute,second);
		m_tagTime.SetCaption(str);

		/*
		if(hour1!=m_tagHour1.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),hour1);
			m_tagHour1.SetCaption(str);
			m_tagHour1.UpdateID(hour1);
			VIEW_STATE vs=m_tagHour1.GetCurViewState();
			vs.fRotateX=-180;
			m_tagHour1.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagHour1.SetAimViewState(&vs);
		}
		if(hour2!=m_tagHour2.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),hour2);
			m_tagHour2.SetCaption(str);
			m_tagHour2.UpdateID(hour2);
			VIEW_STATE vs=m_tagHour2.GetCurViewState();
			vs.fRotateX=-180;
			m_tagHour2.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagHour2.SetAimViewState(&vs);
		}
		if(min1!=m_tagMin1.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),min1);
			m_tagMin1.SetCaption(str);
			m_tagMin1.UpdateID(min1);
			VIEW_STATE vs=m_tagMin1.GetCurViewState();
			vs.fRotateX=-180;
			m_tagMin1.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagMin1.SetAimViewState(&vs);
		}
		if(min2!=m_tagMin2.GetID())//use id to identify
		{
			swprintf_s(str,16,_T("%d"),min2);
			m_tagMin2.SetCaption(str);
			m_tagMin2.UpdateID(min2);
			VIEW_STATE vs=m_tagMin2.GetCurViewState();
			vs.fRotateX=-180;
			m_tagMin2.SetCurViewState(&vs);
			vs.fRotateX=0;
			m_tagMin2.SetAimViewState(&vs);
		}
		*/
	}
}

DWORD CPanelSettingFactory::SystemMonitorThread(LPVOID pDevice)
{
	DWORD dwStartTick;
	DWORD dwIdleStart;
	DWORD dwStopTick;
	DWORD dwIdleEnd;
	int   nPresentUage;
	int   nCpuUage[10]={0};
	int   nCount=0;
	int   ave=0;
	DWORD sum=0;
	int i;
	wchar_t str[64]={0};
	MEMORYSTATUS memorystatus;
	STORE_INFORMATION storeinfo;
	memset(&memorystatus, 0, sizeof(memorystatus));
	memset(&storeinfo, 0, sizeof(storeinfo));
	CPanelSettingFactory* pCPanel = (CPanelSettingFactory*)pDevice;

	do
	{
		WaitForSingleObject(pCPanel->m_hCpuMonitorEvent, INFINITE);
		RETAILMSG(1,(TEXT("MSG:CPU Monitor Start\r\n")));

		while(pCPanel->m_bCpuMonitorActivate)
		{
			dwStartTick=GetTickCount();    // 此处为关键代码 Leo.Zheng
			dwIdleStart=GetIdleTime();
			WaitForSingleObject(pCPanel->m_hCpuMonitorEvent, pCPanel->m_dwCpuMonitorTimeout);
			dwStopTick=GetTickCount();
			dwIdleEnd=GetIdleTime();

			if(!pCPanel->m_bCpuMonitorActivate)
			{
				break;
			}

			nPresentUage=100-(100*(dwIdleEnd-dwIdleStart))/((dwStopTick-dwStartTick));

			if(nCount<10)
			{
				nCpuUage[nCount]=nPresentUage;
				nCount++;
				if(nCount==10)
				{
					for(i=0;i<10;i++)
						sum+=nCpuUage[i];
					ave=sum/10;

					if(pCPanel)
					{
				        if (TryEnterCriticalSection(&pCPanel->m_csMonitor))
				        {
				            memorystatus.dwLength = sizeof(MEMORYSTATUS);
				            GlobalMemoryStatus(&memorystatus);
							GetStoreInformation(&storeinfo);
				            LeaveCriticalSection(&pCPanel->m_csMonitor);
				        }
						RETAILMSG(1,(TEXT("CPU:%3d%% MEM:%3d%% User used:%6dKB System used:%4dKB\r\n"),ave, memorystatus.dwMemoryLoad, (memorystatus.dwTotalPhys-memorystatus.dwAvailPhys)/1024, (storeinfo.dwStoreSize-storeinfo.dwFreeSize)/1024));

						//pCPanel->m_nCpuUtilzation = ave;
						swprintf_s(str,64,_T("CPU:%3d%% 内存:%3d%% User used:%6dKB System used:%4dKB"),ave, memorystatus.dwMemoryLoad, (memorystatus.dwTotalPhys-memorystatus.dwAvailPhys)/1024, (storeinfo.dwStoreSize-storeinfo.dwFreeSize)/1024);
						pCPanel->m_tagSystemInfo.SetSubCaption(str);
					}
					nCount=0;
					sum=0;
//						RETAILMSG(1,(TEXT("CPU laoding is %d%% \r\n"),ave));
				}
			}
		}
		}while(1);
	return 0;
}


void CPanelSettingFactory::SetTestMode(BOOL bTestMode)
{
	if (bTestMode)
	{
		m_btnUSBTestModeDisable.Show(TRUE);
		m_btnUSBTestModeJState.Show(TRUE);
		m_btnUSBTestModeKState.Show(TRUE);
		m_btnUSBTestModeSe0Nak.Show(TRUE);
		m_btnUSBTestModePacket.Show(TRUE);
		m_btnUSBTestModeForceHS.Show(TRUE);
		m_btnUSBTestModeForceFS.Show(TRUE);
		m_btnUSBTestModeForceLS.Show(TRUE);
		m_btniPerfServer.Show(TRUE);
		m_btniPerfClient.Show(TRUE);
#if CVTE_EN_CARPLAY
		CCarplayAdapter::GetInstance()->StartIperfServer();
#endif
	}
	else
	{
		m_btnUSBTestModeDisable.Show(FALSE);
		m_btnUSBTestModeJState.Show(FALSE);
		m_btnUSBTestModeKState.Show(FALSE);
		m_btnUSBTestModeSe0Nak.Show(FALSE);
		m_btnUSBTestModePacket.Show(FALSE);
		m_btnUSBTestModeForceHS.Show(FALSE);
		m_btnUSBTestModeForceFS.Show(FALSE);
		m_btnUSBTestModeForceLS.Show(FALSE);
		m_btniPerfServer.Show(FALSE);
		m_btniPerfClient.Show(FALSE);
	}
}