#include "PanelSettingAdvanced.h"
#include "../../resource.h"
#include "../../VIP/CVideoWindow.h"
#include "../Radio/RadioCommon.h"
#include "../../Include/oemsector.h"
#if CVTE_EN_CARPLAY
#include "CarplayAPI.h"
#endif
#include "../Common/controls_res.h"


#define STEER_WHEEL_STATUS_IDLE         0x00
#define STEER_WHEEL_STATUS_CALIB_START  0x01
#define STEER_WHEEL_STATUS_CALIB_STOP   0x02
#define STEER_WHEEL_STATUS_CALIB_WAIT_FOR_NET_READY 0x03
#define STEER_WHEEL_STATUS_CALIB_NET_READY    0x04
#define STEER_WHEEL_STATUS_CALIB_NET_ERROR    0x05
#define STEER_WHEEL_STATUS_QUERY        0x06
#define STEER_WHEEL_STATUS_QUERY_DONE   0x07

CPanelSettingAdvanced * g_pCPanelSettingAdvanced = NULL;

const int box_size_x=LIST_BOX_ITEM_WIDTH;
const int margin_x=12;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	const int tag_h = 80;
#else
	const int tag_h = LIST_BOX_TAG_HEIGHT;
#endif

const int cx=g_iClientWidth-box_size_x-margin_x*2;
const int tag_w=cx/3;
const int caption_h = 22;
const int caption2_h = 18;
const int caption3_h = 14;
static int iSteerWheelPanelPy;
const int bar_h	=	70;
CPanelSettingAdvanced::CPanelSettingAdvanced(void)
{
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_bSteerWheelBlockShow = TRUE;
#else
    m_bSteerWheelBlockShow = FALSE;
#endif
    m_bSteerWheelKeyCalibrating = FALSE;
    m_nCalibCurrentKey = 0x00;
    m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_IDLE;
    m_nQueryKey = 0;
    m_bMCUUpdating = FALSE;
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
    m_bRDSEnable = FALSE;
#endif
//		m_bVerify = FALSE;
//		m_bPasswordHide = TRUE;
	//m_bRearViewColorSignalReset = FALSE;
	m_bLock = FALSE;
	m_bCalibrated = FALSE;
    memset(&m_steerwheelkey, 0, sizeof(m_steerwheelkey));
}
CPanelSettingAdvanced::~CPanelSettingAdvanced(void)
{
	//close widget:
	m_listSTW.Head();
	while (!m_listSTW.End())
	{
		delete m_listSTW.Get();
		m_listSTW.Next();
	}
}

BOOL CPanelSettingAdvanced::IsReady()
{
// 	return (CPanelGL::IsReady() && (m_tagSteerWheelPanel.IsReady()) && !(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START) && !(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_WAIT_FOR_NET_READY));
	return CPanelGL::IsReady() ;
}

void CPanelSettingAdvanced::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingAdvanced::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START ||
		m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_WAIT_FOR_NET_READY ||
		m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_QUERY ||
		m_bSteerWheelKeyCalibrating)
    {
		ClearSTW();
    }

//		if(m_bRearViewColorSignalReset)
//		{
//			m_bRearViewColorSignalReset = FALSE;
//			m_btnNTSC_MJ.Enable(FALSE);
//			m_btnNTSC_443.Enable(FALSE);
//			m_btnPAL_M.Enable(FALSE);
//			m_btnPAL_60.Enable(FALSE);
//			m_btnPAL_BGHID.Enable(FALSE);
//			m_btnPAL_COM_N.Enable(FALSE);
//			m_btnSECAM.Enable(FALSE);
//			m_btnSECAM_525.Enable(FALSE);
//			m_btnRearReset.SetCaption(GetResourceString(IDS_RESET));
//		}
}

void CPanelSettingAdvanced::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	static COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
#else
	static COLORGL cr_gray={0.25,0.25,0.25,1.0};
#endif
	static COLORGL cr_gray2={0.35,0.35,0.35,1.0};
	static COLORGL cr_dark={0.0,0.0,0.0,1.0};

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif


#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
#if CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE
	DrawRect(0,g_iClientHeight/2-bar_h-10,g_iClientWidth-LIST_BOX_ITEM_WIDTH-50,1,&cr_gray);

	if(m_pTextGL)
	{
		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);

		//Title
		{
			m_pTextGL->SetString(GetResourceString(IDS_SETTING_STEER_WHEEL));
			m_pTextGL->SetCharScale(0.75f,0.75f);
			m_pTextGL->SetColor(&cr_dark);
			//m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-20);
			//m_pTextGL->SetCharScale(1.0f,1.0f);
			//m_pTextGL->GetExtension(&si);
			//m_pTextGL->SetSize(g_iClientWidth/2, caption_h);
			//m_pTextGL->SetPos(0,g_iClientHeight/2-25);
			m_pTextGL->Render();
		}

		//Little title
		{
			m_pTextGL->SetString(GetResourceString(IDS_SETTING_STEER_WHEEL_DESC));
			m_pTextGL->SetCharScale(0.45f,0.45f);
			m_pTextGL->SetColor(&cr_dark);
			//m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-30-25);
			//m_pTextGL->SetCharScale(0.8f,0.8f);
			//m_pTextGL->GetExtension(&si);
			//m_pTextGL->SetSize(g_iClientWidth/2, caption2_h);
	 		//m_pTextGL->SetPos(0/*-cx/2*/,g_iClientHeight/2-50);
			m_pTextGL->Render();
		}
	}
#endif	//CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE
#else

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	DrawRect(0,g_iClientHeight/2-80, g_iClientWidth-LIST_BOX_ITEM_WIDTH - 10, 1, &cr_gray);

	m_btnReturn.SetAlpha(m_fCurAlpha);
	m_btnReturn.Render();

	m_btnLock.SetAlpha(m_fCurAlpha);
	m_btnLock.Render();

	if(m_pTextGL)
	{
		//Title
		{
			m_pTextGL->SetString(GetResourceString(IDS_SETTING_ADVANCE));
			m_pTextGL->SetColor(&cr_gray);
			m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-6);
			m_pTextGL->Render();
		}

		//Little title
		{
			m_pTextGL->SetString(GetResourceString(IDS_SETTING_ADVANCED));
			m_pTextGL->SetCharScale(0.4f,0.4f);
			m_pTextGL->SetColor(&cr_gray);
			m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-36-8);
			m_pTextGL->Render();
		}
	}
#else
	DrawRect(0,g_iClientHeight/2-bar_h-8,g_iClientWidth-LIST_BOX_ITEM_WIDTH-50,1,&cr_gray);

	m_btnReturn.SetAlpha(m_fCurAlpha);
	m_btnReturn.Render();

	m_btnLock.SetAlpha(m_fCurAlpha);
	m_btnLock.Render();

	if(m_pTextGL)
	{
		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);

		//Title
		{
			m_pTextGL->SetString(GetResourceString(IDS_SETTING_ADVANCE));
			m_pTextGL->SetCharScale(0.8f,0.8f);
			m_pTextGL->SetColor(&cr_dark);
			//m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-6);
			//m_pTextGL->SetCharScale(1.0f,1.0f);
			//m_pTextGL->GetExtension(&si);
			//m_pTextGL->SetSize(g_iClientWidth/2, caption_h);
			//m_pTextGL->SetPos(0,g_iClientHeight/2-25);
			m_pTextGL->Render();
		}

		//Little title
		{
			m_pTextGL->SetString(GetResourceString(IDS_SETTING_ADVANCED));
			m_pTextGL->SetCharScale(0.4f,0.4f);
			m_pTextGL->SetColor(&cr_gray2);
			//m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-36-8);
			//m_pTextGL->SetCharScale(0.8f,0.8f);
			//m_pTextGL->GetExtension(&si);
			//m_pTextGL->SetSize(g_iClientWidth/2, caption2_h);
			//m_pTextGL->SetPos(0/*-cx/2*/,g_iClientHeight/2-50);
			m_pTextGL->Render();
		}
	}

#endif
	


	//if(m_bVerify)
#if (CUSTOMER_ID != CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
		//drive type:
		m_btnDriveType.SetAlpha(m_fCurAlpha);
		m_btnDriveType.Render();
		m_tagValueDriveType.SetAlpha(m_fCurAlpha);
		m_tagValueDriveType.Render();
#endif

		//brake test:
		m_tagTitleBrakeTest.SetAlpha(m_fCurAlpha);
		m_tagTitleBrakeTest.Render();
		m_sliBrakeTest.SetAlpha(m_fCurAlpha);
		m_sliBrakeTest.Render();

		//lamp control:
		m_tagTitleLampControl.SetAlpha(m_fCurAlpha);
		m_tagTitleLampControl.Render();
		m_sliLampControl.SetAlpha(m_fCurAlpha);
		m_sliLampControl.Render();

#if CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE
		//StreerWheel
		m_tagSteerWheelPanel.SetAlpha(m_fCurAlpha);
	    m_tagSteerWheelPanel.Render();

	    m_tagTitleStreerWheel1.SetAlpha(m_fCurAlpha);
	    m_tagTitleStreerWheel1.Render();

	    m_tagTitleStreerWheel2.SetAlpha(m_fCurAlpha);
	    m_tagTitleStreerWheel2.Render();
#endif	//CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE

#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE

#if CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE
		m_btnStreerWheelSpread.SetAlpha(m_fCurAlpha);
	    m_btnStreerWheelSpread.Render();

	    if(m_bSteerWheelBlockShow)
	    {

		    if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START ||
		        m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_WAIT_FOR_NET_READY ||
		        m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_QUERY ||
		        m_bSteerWheelKeyCalibrating)
		    {
				SetFrameTime(30);

				VIEW_STATE vs = m_btnStreerWheelSpread.GetCurViewState();
				vs.fRotateZ -= 6;
				if(vs.fRotateZ < -360)
					vs.fRotateZ = 0;
				m_btnStreerWheelSpread.SetCurViewState(&vs);
		    }
			else
			{
				SetFrameTime(500);
			}

	//			m_iconSteerWheelFocus.SetAlpha(m_fCurAlpha);
	//			m_iconSteerWheelFocus.Render();

	        COLORGL cr_gray={0.35,0.35,0.35,1.0};

		    m_btnVolPlus.SetAlpha(m_fCurAlpha);
		    m_btnVolPlus.Render();

		    m_btnVolMinus.SetAlpha(m_fCurAlpha);
		    m_btnVolMinus.Render();

		    m_btnVolMute.SetAlpha(m_fCurAlpha);
		    m_btnVolMute.Render();

		    m_btnPrev.SetAlpha(m_fCurAlpha);
		    m_btnPrev.Render();

		    m_btnNext.SetAlpha(m_fCurAlpha);
		    m_btnNext.Render();

		    m_btnSeek.SetAlpha(m_fCurAlpha);
		    m_btnSeek.Render();

		    m_btnCall.SetAlpha(m_fCurAlpha);
		    m_btnCall.Render();

		    m_btnHangup.SetAlpha(m_fCurAlpha);
		    m_btnHangup.Render();

		    m_btnMode.SetAlpha(m_fCurAlpha);
		    m_btnMode.Render();

		    m_btnSpeech.SetAlpha(m_fCurAlpha);
		    m_btnSpeech.Render();

		    m_btnPower.SetAlpha(m_fCurAlpha);
		    m_btnPower.Render();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
			m_btnHome.SetAlpha(m_fCurAlpha);
			m_btnHome.Render();
#else
		    m_btnNavi.SetAlpha(m_fCurAlpha);
		    m_btnNavi.Render();
#endif
		    m_iconVolMinus.SetAlpha(m_fCurAlpha);
		    m_iconVolMinus.Render();
		    m_iconVolMinusFocus.SetAlpha(m_fCurAlpha);
		    m_iconVolMinusFocus.Render();

		    m_iconVolPlus.SetAlpha(m_fCurAlpha);
		    m_iconVolPlus.Render();
		    m_iconVolPlusFocus.SetAlpha(m_fCurAlpha);
		    m_iconVolPlusFocus.Render();

		    m_iconVolMute.SetAlpha(m_fCurAlpha);
		    m_iconVolMute.Render();
		    m_iconVolMuteFocus.SetAlpha(m_fCurAlpha);
		    m_iconVolMuteFocus.Render();

		    m_iconPrev.SetAlpha(m_fCurAlpha);
		    m_iconPrev.Render();
		    m_iconPrevFocus.SetAlpha(m_fCurAlpha);
		    m_iconPrevFocus.Render();

		    m_iconNext.SetAlpha(m_fCurAlpha);
		    m_iconNext.Render();
		    m_iconNextFocus.SetAlpha(m_fCurAlpha);
		    m_iconNextFocus.Render();

		    m_iconSeek.SetAlpha(m_fCurAlpha);
		    m_iconSeek.Render();
		    m_iconSeekFocus.SetAlpha(m_fCurAlpha);
		    m_iconSeekFocus.Render();

		    m_iconCall.SetAlpha(m_fCurAlpha);
		    m_iconCall.Render();
		    m_iconCallFocus.SetAlpha(m_fCurAlpha);
		    m_iconCallFocus.Render();

		    m_iconHangup.SetAlpha(m_fCurAlpha);
		    m_iconHangup.Render();
		    m_iconHangupFocus.SetAlpha(m_fCurAlpha);
		    m_iconHangupFocus.Render();

		    m_iconSpeech.SetAlpha(m_fCurAlpha);
		    m_iconSpeech.Render();
		    m_iconSpeechFocus.SetAlpha(m_fCurAlpha);
		    m_iconSpeechFocus.Render();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
			m_iconHome.SetAlpha(m_fCurAlpha);
			m_iconHome.Render();
			m_iconHomeFocus.SetAlpha(m_fCurAlpha);
			m_iconHomeFocus.Render();
#else
#if CVTE_EN_NAVI
		    m_iconNavi.SetAlpha(m_fCurAlpha);
		    m_iconNavi.Render();
		    m_iconNaviFocus.SetAlpha(m_fCurAlpha);
		    m_iconNaviFocus.Render();
#endif
#endif
		    m_iconMode.SetAlpha(m_fCurAlpha);
		    m_iconMode.Render();
		    m_iconModeFocus.SetAlpha(m_fCurAlpha);
		    m_iconModeFocus.Render();

		    m_iconPower.SetAlpha(m_fCurAlpha);
		    m_iconPower.Render();
		    m_iconPowerFocus.SetAlpha(m_fCurAlpha);
		    m_iconPowerFocus.Render();

		    m_btnSteerWheelConfirm.SetAlpha(m_fCurAlpha);
		    m_btnSteerWheelConfirm.Render();

		    m_btnSteerWheelCancel.SetAlpha(m_fCurAlpha);
		    m_btnSteerWheelCancel.Render();


			if(m_bSteerWheelKeyCalibrating)
			{
				m_tagSteerWheelWindow.SetAlpha(0.9f);
				m_tagSteerWheelWindow.Render();
			}

#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)

#if !CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
			DrawRect(0,iSteerWheelPanelPy-tag_h/2,g_iClientWidth-box_size_x-60,1,&cr_gray);
#endif
	        DrawRect(0,iSteerWheelPanelPy-tag_h/2-tag_h*4-15,g_iClientWidth-box_size_x-60,1,&cr_gray);


#endif
	    }
#endif	//CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE

#if 0
	    {
	        COLORGL cr_gray={0.35,0.35,0.35,1.0};

			DrawRect(0,g_iClientHeight/2-50-36-tag_h*6-tag_h/2,g_iClientWidth-box_size_x-50,1,&cr_gray);
			DrawRect(0,g_iClientHeight/2-50-36-tag_h*9-tag_h/2-15,g_iClientWidth-box_size_x-50,1,&cr_gray);
		}
		m_tagTitleRearView.SetAlpha(m_fCurAlpha);
	    m_tagTitleRearView.Render();
		m_tagTitleRearView2.SetAlpha(m_fCurAlpha);
	    m_tagTitleRearView2.Render();
		m_iconRearView.SetAlpha(m_fCurAlpha);
	    m_iconRearView.Render();

		m_btnNTSC_MJ.SetAlpha(m_fCurAlpha);
	    m_btnNTSC_MJ.Render();
		m_btnNTSC_443.SetAlpha(m_fCurAlpha);
	    m_btnNTSC_443.Render();
		m_btnPAL_M.SetAlpha(m_fCurAlpha);
	    m_btnPAL_M.Render();
		m_btnPAL_60.SetAlpha(m_fCurAlpha);
	    m_btnPAL_60.Render();
		m_btnPAL_BGHID.SetAlpha(m_fCurAlpha);
	    m_btnPAL_BGHID.Render();
		m_btnPAL_COM_N.SetAlpha(m_fCurAlpha);
	    m_btnPAL_COM_N.Render();
		m_btnSECAM.SetAlpha(m_fCurAlpha);
	    m_btnSECAM.Render();
		m_btnSECAM_525.SetAlpha(m_fCurAlpha);
	    m_btnSECAM_525.Render();
		m_btnRearReset.SetAlpha(m_fCurAlpha);
	    m_btnRearReset.Render();
#endif
//		else
//		{
//			m_tagPasswordTitle.SetAlpha(m_fCurAlpha*0.7f);
//			m_tagPasswordTitle.Render();
//
//			for (DWORD i = 0; i<_countof(m_tagNumber); i++)
//			{
//				m_tagNumber[i].SetAlpha(m_fCurAlpha);
//				m_tagNumber[i].Render();
//			}
//
//			for (DWORD i = 0; i<_countof(m_btnNumber); i++)
//			{
//				m_btnNumber[i].SetAlpha(m_fCurAlpha);
//				m_btnNumber[i].Render();
//			}
//		}

	EndRender();
}

BOOL CPanelSettingAdvanced::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);
//		VIEW_STATE vs;
//		SIZE siBk;
//
//		m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_100));
//		m_tagBkBlur.SetSize(g_iClientWidth-box_size_x,g_iClientHeight);
//		siBk=m_tagBkBlur.GetTexture()->GetImageSize();
//		m_tagBkBlur.SetScale(GLfloat(g_iClientWidth-box_size_x)/siBk.cx,GLfloat(g_iClientHeight)/siBk.cy);
//		vs=m_tagBkBlur.GetCurViewState();
//		vs.fRotateX=-180;
//		m_tagBkBlur.SetIniViewState(&vs);

    g_pCPanelSettingAdvanced = this;
	//large size:
	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	COLORGL cr_w={1.0,1.0,1.0,1.0};
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
#else
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
#endif
    COLORGL cr_gray2={0.35,0.35,0.35,1.0};
	COLORGL cr_dark={0.0,0.0,0.0,1.0};
	COLORGL cr_green={0.0f/256.0f,144.0f/256.0f,255.0f/256.0f,1.0};
//		COLORGL cr_gray={0.0,161.0f/256.0f,228.0/256.0f,1.0};
	SIZE sizeValue={200,80};
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	GLfloat py=g_iClientHeight/2-45;
#else
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
    GLfloat py=g_iClientHeight/2-110;
#else
	GLfloat py=g_iClientHeight/2-50-70;
#endif
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(g_iClientWidth-box_size_x, g_iClientHeight*4);
	m_tagBk.SetPos(0, -g_iClientHeight);
#endif

	SLIDERSTRUCTURE sli;
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

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	////Steer wheel
	iSteerWheelPanelPy = py;

	m_btnStreerWheelSpread.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_828),NULL,
		NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnStreerWheelSpread.SetPos(170+60,py+5);
#else
	//Return
	m_btnReturn.Initialize(IDC_SETTING_BTN_ADVANCE_RETURN,this,
		m_pSkinManager->GetTexture(TEXID_095),NULL,
		m_pSkinManager->GetTexture(TEXID_012),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnReturn.SetShowScale(0.9f);
	m_btnReturn.SetPos(cx/2-40,g_iClientHeight/2-40);
	m_btnReturn.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);

	//Lock
	m_iconLock.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_829));
	m_iconUnLock.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_830));

	m_btnLock.Initialize(IDC_SETTING_BTN_LOCK,this,NULL,NULL,
		NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnLock.SetTouchRate(1.2f,1.2f);
	m_btnLock.SetShowScale(0.65f);
	m_btnLock.EnableDrawBack(FALSE);
	m_btnLock.SetIcon(&m_iconUnLock, &m_iconLock);
	m_btnLock.SetPos(cx/2-40-60,g_iClientHeight/2-40);
	m_btnLock.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);

#if (CUSTOMER_ID != CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
	//drive type

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_btnDriveType.Initialize(IDC_SETTING_BTN_SET_DRIVE_TYPE,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
#else
	m_btnDriveType.Initialize(IDC_SETTING_BTN_SET_DRIVE_TYPE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	m_btnDriveType.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_TYPE));
	m_btnDriveType.LinkTextGL(pTextGL);
	m_btnDriveType.SetPos(0,py);
	m_btnDriveType.SetCaptionHeight(caption_h);
	m_btnDriveType.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnDriveType.SetCaptionColor(&cr_gray);
	m_btnDriveType.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);

	m_tagValueDriveType.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueDriveType.SetSize((g_iClientWidth-box_size_x-margin_x*2),caption2_h);
	switch (m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType)
	{
	case 0://left
		m_tagValueDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_LEFT));
		break;
	case 1://right
		m_tagValueDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_RIGHT));
		break;
	}
	m_tagValueDriveType.LinkTextGL(pTextGL);
	m_tagValueDriveType.SetPos(-10,py);
	m_tagValueDriveType.SetCaptionHeight(caption_h);
	m_tagValueDriveType.EnableDrawBack(FALSE);
	m_tagValueDriveType.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueDriveType.SetCaptionColor(&cr_gray);

#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
	//Update Driver Type Info to MCU
	if (m_pCommander)
	{
		BYTE buf = m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType;
		m_pCommander->SendExtenCmd(_ARM_EXTEN_TAPE_SIDE,&buf,1);
	}	
#endif

	py-=tag_h;
#endif

	//brake test:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagTitleBrakeTest.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
#else
	m_tagTitleBrakeTest.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	m_tagTitleBrakeTest.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleBrakeTest.SetCaption(GetResourceString(IDS_SETTING_HANDBRAKE_TEST));
	m_tagTitleBrakeTest.LinkTextGL(pTextGL);
	m_tagTitleBrakeTest.SetPos(0,py);
	m_tagTitleBrakeTest.SetCaptionHeight(caption_h);
	m_tagTitleBrakeTest.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleBrakeTest.SetCaptionColor(&cr_gray);

	m_sliBrakeTest.Initialize(IDC_SETTING_SLI_BREAK_TEST,this,&sli,pSkinManager);
	m_sliBrakeTest.SetRange(0,10);
	m_sliBrakeTest.SetValue(0);
	m_sliBrakeTest.SetPos(m_siCurSize.cx/2-70,py);
	m_sliBrakeTest.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);

	py-=tag_h;

	//lamp control:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagTitleLampControl.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
#else
	m_tagTitleLampControl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	m_tagTitleLampControl.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLampControl.SetCaption(GetResourceString(IDS_SETTING_LAMP_CONTROL));
	m_tagTitleLampControl.LinkTextGL(pTextGL);
	m_tagTitleLampControl.SetPos(0,py);
	m_tagTitleLampControl.SetCaptionHeight(caption_h);
	m_tagTitleLampControl.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleLampControl.SetCaptionColor(&cr_gray);

	m_sliLampControl.Initialize(IDC_SETTING_SLI_LAMP_CONTROL,this,&sli,pSkinManager);
	m_sliLampControl.SetRange(0,10);
	m_sliLampControl.SetValue(0);
	m_sliLampControl.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLampControl.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);

#if 0	
	py-=tag_h;

	//camera type
	m_btnCameraType.Initialize(IDC_SETTING_BTN_SET_CAMERA_TYPE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCameraType.SetSize((g_iClientWidth-240-margin_x*2),80);
	m_btnCameraType.SetCaption(GetResourceString(IDS_SETTING_CAMERA_TYPE));
	m_btnCameraType.LinkTextGL(pTextGL);
	m_btnCameraType.SetPos(0,py);
	m_btnCameraType.SetCaptionHeight(24);
	m_btnCameraType.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnCameraType.SetCaptionColor(&cr_gray);
	m_btnCameraType.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);

	m_tagValueCameraType.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueCameraType.SetSize(cx,40);
	switch (m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType)
	{
	case 0://CVBS NTSC
	case 1://CVBS PAL
		m_tagValueCameraType.SetCaption(_T("CVBS"));
		break;
	case 2://ARGB
		m_tagValueCameraType.SetCaption(_T("RGB"));
		break;
	}
	m_tagValueCameraType.LinkTextGL(pTextGL);
	m_tagValueCameraType.SetPos(cx/2+10 -20,py);
	m_tagValueCameraType.SetCaptionHeight(24);
	m_tagValueCameraType.EnableDrawBack(FALSE);
	m_tagValueCameraType.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueCameraType.SetCaptionColor(&cr_gray);
#endif


#if CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE
	py-=tag_h;

	//Steer wheel
	iSteerWheelPanelPy = py;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagSteerWheelPanel.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
#else
 m_tagSteerWheelPanel.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
   
	m_tagSteerWheelPanel.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h+6);	//((g_iClientWidth-box_size_x-margin_x*2),tag_h*6+6);
    m_tagSteerWheelPanel.SetPos(0,py-3);//(0,py-LIST_BOX_TAG_HEIGHT*2-LIST_BOX_TAG_HEIGHT/2-3);
//	    m_iconSteerWheel.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_827));
//		m_iconSteerWheel.SetPos(170+10+32,py);

	m_btnStreerWheelSpread.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SPREAD,this,m_pSkinManager->GetTexture(TEXID_828),NULL,
		m_pSkinManager->GetTexture(TEXID_827),NULL,NULL,NULL,NULL,NULL);
	m_btnStreerWheelSpread.SetPos(170+60,py);
	m_btnStreerWheelSpread.SetTouchRate(5.0f, 1.5f);
	m_btnStreerWheelSpread.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);

//	    m_iconSteerWheelFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_828));
//		m_iconSteerWheelFocus.SetPos(170+10+32+40,py);
//		m_iconSteerWheelFocus.Show(FALSE);


    m_tagTitleStreerWheel1.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleStreerWheel1.SetSize(cx,tag_h);
	m_tagTitleStreerWheel1.SetCaption(GetResourceString(IDS_SETTING_STEER_WHEEL));
	m_tagTitleStreerWheel1.LinkTextGL(pTextGL);
	m_tagTitleStreerWheel1.SetPos(0,py+12);
	m_tagTitleStreerWheel1.SetCaptionHeight(caption_h);
	m_tagTitleStreerWheel1.EnableDrawBack(FALSE);
	m_tagTitleStreerWheel1.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleStreerWheel1.SetCaptionColor(&cr_gray);

    m_tagTitleStreerWheel2.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleStreerWheel2.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleStreerWheel2.SetCaption(GetResourceString(IDS_SETTING_STEER_WHEEL_DESC));
	m_tagTitleStreerWheel2.LinkTextGL(pTextGL);
	m_tagTitleStreerWheel2.SetPos(0,py-18);
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
		case LANGUAGE_GREEK:
			m_tagTitleStreerWheel2.SetCaptionHeight(12);
			break;
		default:
			m_tagTitleStreerWheel2.SetCaptionHeight(caption3_h);
			break;
	}
	m_tagTitleStreerWheel2.EnableDrawBack(FALSE);
	m_tagTitleStreerWheel2.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleStreerWheel2.SetCaptionColor(&cr_gray);
#endif

#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	int tag_h2 = 75;
	py -= tag_h2;
#else
	py-=tag_h+6;
#endif

	SIZE sizebtn={170,64};
#if CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnVolMinus.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_MINUS,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnVolMinus.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_MINUS,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnVolMinus.SetSize(&sizebtn);
	m_btnVolMinus.SetPos(-170-10,py);
	m_btnVolMinus.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnVolMinus.SetCaption(GetResourceString(IDS_STW_VOL_SUB));
    m_btnVolMinus.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnVolMinus.SetCaptionHeight(16);
    m_btnVolMinus.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnVolMinus.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnVolMinus.LinkTextGL(pTextGL);
    m_btnVolMinus.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnVolMinus.Show(FALSE);

    m_iconVolMinus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_801));
    m_iconVolMinus.SetPos(-170-10-62,py);
	//m_iconVolMinus.Show(FALSE);
    m_iconVolMinusFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_802));
    m_iconVolMinusFocus.SetPos(-170-10-62,py);
    m_iconVolMinusFocus.Show(FALSE);
    m_steerwheelkey[0].keyname = KEY_STW_VOL_MINUS;
    m_steerwheelkey[0].keyvalue = -1;
    m_steerwheelkey[0].valid = FALSE;

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnVolPlus.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_PLUS,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnVolPlus.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_PLUS,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnVolPlus.SetSize(&sizebtn);
	m_btnVolPlus.SetPos(0,py);
	m_btnVolPlus.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnVolPlus.SetCaption(GetResourceString(IDS_STW_VOL_ADD));
    m_btnVolPlus.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnVolPlus.SetCaptionHeight(16);
    m_btnVolPlus.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnVolPlus.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnVolPlus.LinkTextGL(pTextGL);
    m_btnVolPlus.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnVolPlus.Show(FALSE);

    m_iconVolPlus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_803));
    m_iconVolPlus.SetPos(-62,py);
	//m_iconVolPlus.Show(FALSE);
    m_iconVolPlusFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_804));
    m_iconVolPlusFocus.SetPos(-62,py);
    m_iconVolPlusFocus.Show(FALSE);
    m_steerwheelkey[1].keyname = KEY_STW_VOL_PLUS;
    m_steerwheelkey[1].keyvalue = -1;
    m_steerwheelkey[1].valid = FALSE;

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnVolMute.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_MUTE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnVolMute.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_MUTE,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnVolMute.SetSize(&sizebtn);
	m_btnVolMute.SetPos(170+10,py);
	m_btnVolMute.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnVolMute.SetCaption(GetResourceString(IDS_STW_MUTE));
    m_btnVolMute.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnVolMute.SetCaptionHeight(16);
    m_btnVolMute.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnVolMute.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnVolMute.LinkTextGL(pTextGL);
    m_btnVolMute.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnVolMute.Show(FALSE);

    m_iconVolMute.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_805));
    m_iconVolMute.SetPos(170+10-62,py);
	//m_iconVolMute.Show(FALSE);
    m_iconVolMuteFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_806));
    m_iconVolMuteFocus.SetPos(170+10-62,py);
    m_iconVolMuteFocus.Show(FALSE);
    m_steerwheelkey[2].keyname = KEY_STW_VOL_MUTE;
    m_steerwheelkey[2].keyvalue = -1;
    m_steerwheelkey[2].valid = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
    py -= tag_h2;
#else
	py-=tag_h;
#endif	

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnPrev.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_PREV,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnPrev.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_PREV,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnPrev.SetSize(&sizebtn);
	m_btnPrev.SetPos(-170-10,py);
	m_btnPrev.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnPrev.SetCaption(GetResourceString(IDS_STW_PREVIOUS));
    m_btnPrev.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnPrev.SetCaptionHeight(16);
    m_btnPrev.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnPrev.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnPrev.LinkTextGL(pTextGL);
    m_btnPrev.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnPrev.Show(FALSE);

    m_iconPrev.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_807));
    m_iconPrev.SetPos(-170-10-62,py);
	//m_iconPrev.Show(FALSE);
    m_iconPrevFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_808));
    m_iconPrevFocus.SetPos(-170-10-62,py);
    m_iconPrevFocus.Show(FALSE);
    m_steerwheelkey[3].keyname = KEY_STW_PREV;
    m_steerwheelkey[3].keyvalue = -1;
    m_steerwheelkey[3].valid = FALSE;

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnNext.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_NEXT,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnNext.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_NEXT,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnNext.SetSize(&sizebtn);
	m_btnNext.SetPos(0,py);
	m_btnNext.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnNext.SetCaption(GetResourceString(IDS_STW_NEXT));
    m_btnNext.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnNext.SetCaptionHeight(16);
    m_btnNext.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnNext.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnNext.LinkTextGL(pTextGL);
    m_btnNext.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnNext.Show(FALSE);

    m_iconNext.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_809));
    m_iconNext.SetPos(0-62,py);
	//m_iconNext.Show(FALSE);
    m_iconNextFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_810));
    m_iconNextFocus.SetPos(0-62,py);
    m_iconNextFocus.Show(FALSE);
    m_steerwheelkey[4].keyname = KEY_STW_NEXT;
    m_steerwheelkey[4].keyvalue = -1;
    m_steerwheelkey[4].valid = FALSE;

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnSeek.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SEEK,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnSeek.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SEEK,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnSeek.SetSize(&sizebtn);
	m_btnSeek.SetPos(170+10,py);
	m_btnSeek.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnSeek.SetCaption(GetResourceString(IDS_STW_SEEK));
    m_btnSeek.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnSeek.SetCaptionHeight(16);
    m_btnSeek.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnSeek.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnSeek.LinkTextGL(pTextGL);
    m_btnSeek.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnSeek.Show(FALSE);

    m_iconSeek.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_811));
    m_iconSeek.SetPos(170+10-62,py);
	//m_iconSeek.Show(FALSE);
    m_iconSeekFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_812));
    m_iconSeekFocus.SetPos(170+10-62,py);
    m_iconSeekFocus.Show(FALSE);
    m_steerwheelkey[5].keyname = KEY_STW_SEEK;
    m_steerwheelkey[5].keyvalue = -1;
    m_steerwheelkey[5].valid = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	py -= tag_h2;
#else
	py-=tag_h;
#endif

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnCall.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CALL,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnCall.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CALL,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);

#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnCall.SetSize(&sizebtn);
	m_btnCall.SetPos(-170-10,py);
	m_btnCall.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnCall.SetCaption(GetResourceString(IDS_STW_CALL));
    m_btnCall.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnCall.SetCaptionHeight(16);
    m_btnCall.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnCall.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnCall.LinkTextGL(pTextGL);
    m_btnCall.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnCall.Show(FALSE);

    m_iconCall.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_813));
    m_iconCall.SetPos(-170-10-62,py);
	//m_iconCall.Show(FALSE);
    m_iconCallFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_814));
    m_iconCallFocus.SetPos(-170-10-62,py);
    m_iconCallFocus.Show(FALSE);
    m_steerwheelkey[6].keyname = KEY_STW_PHONE_CALL;
    m_steerwheelkey[6].keyvalue = -1;
    m_steerwheelkey[6].valid = FALSE;

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnHangup.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_HANGUP,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnHangup.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_HANGUP,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnHangup.SetSize(&sizebtn);
	m_btnHangup.SetPos(0,py);
	m_btnHangup.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnHangup.SetCaption(GetResourceString(IDS_STW_HANGUP));
    m_btnHangup.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnHangup.SetCaptionHeight(16);
    m_btnHangup.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnHangup.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnHangup.LinkTextGL(pTextGL);
    m_btnHangup.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnHangup.Show(FALSE);

    m_iconHangup.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_815));
    m_iconHangup.SetPos(-62,py);
	//m_iconHangup.Show(FALSE);
    m_iconHangupFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_816));
    m_iconHangupFocus.SetPos(-62,py);
    m_iconHangupFocus.Show(FALSE);
    m_steerwheelkey[7].keyname = KEY_STW_PHONE_HANGUP;
    m_steerwheelkey[7].keyvalue = -1;
    m_steerwheelkey[7].valid = FALSE;

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnSpeech.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SPEECH,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnSpeech.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SPEECH,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnSpeech.SetSize(&sizebtn);
	m_btnSpeech.SetPos(170+10,py);
	m_btnSpeech.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
    m_btnSpeech.SetCaption(_T("Siri"));
#else
    m_btnSpeech.SetCaption(GetResourceString(IDS_STW_SPEECH));
#endif
    m_btnSpeech.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnSpeech.SetCaptionHeight(16);
    m_btnSpeech.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnSpeech.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnSpeech.LinkTextGL(pTextGL);
    m_btnSpeech.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnSpeech.Show(FALSE);

    m_iconSpeech.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_823));
    m_iconSpeech.SetPos(170+10-62,py);
	//m_iconSpeech.Show(FALSE);
    m_iconSpeechFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_824));
    m_iconSpeechFocus.SetPos(170+10-62,py);
    m_iconSpeechFocus.Show(FALSE);
    m_steerwheelkey[8].keyname = KEY_STW_SPEECH;
    m_steerwheelkey[8].keyvalue = -1;
    m_steerwheelkey[8].valid = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	py -= tag_h2;
#else
	py-=tag_h;
#endif

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnPower.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_POWER,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnPower.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_POWER,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnPower.SetSize(&sizebtn);
	m_btnPower.SetPos(-170-10,py);
	m_btnPower.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnPower.SetCaption(GetResourceString(IDS_STW_POWER));
    m_btnPower.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnPower.SetCaptionHeight(16);
    m_btnPower.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnPower.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnPower.LinkTextGL(pTextGL);
    m_btnPower.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnPower.Show(FALSE);

    m_iconPower.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_825));
    m_iconPower.SetPos(-170-10-62,py);
	//m_iconPower.Show(FALSE);
    m_iconPowerFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_826));
    m_iconPowerFocus.SetPos(-170-10-62,py);
    m_iconPowerFocus.Show(FALSE);
    m_steerwheelkey[9].keyname = KEY_STW_POWER;
    m_steerwheelkey[9].keyvalue = -1;
    m_steerwheelkey[9].valid = FALSE;

#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnMode.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_MODE,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnMode.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_MODE,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnMode.SetSize(&sizebtn);
	m_btnMode.SetPos(0,py);
	m_btnMode.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnMode.SetCaption(GetResourceString(IDS_STW_MODE));
    m_btnMode.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnMode.SetCaptionHeight(16);
    m_btnMode.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnMode.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnMode.LinkTextGL(pTextGL);
    m_btnMode.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnMode.Show(FALSE);

    m_iconMode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_821));
    m_iconMode.SetPos(-62,py);
	//m_iconMode.Show(FALSE);
    m_iconModeFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_822));
    m_iconModeFocus.SetPos(-62,py);
    m_iconModeFocus.Show(FALSE);
    m_steerwheelkey[10].keyname = KEY_STW_MODE;
    m_steerwheelkey[10].keyvalue = -1;
    m_steerwheelkey[10].valid = FALSE;

#if (CUSTOMER_ID == CUSTOMER_ENTERFRONT)
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnNavi.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnNavi.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif
	m_btnNavi.SetSize(&sizebtn);
	m_btnNavi.SetPos(170+10,py);
	m_btnNavi.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnNavi.SetCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnNavi.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnNavi.SetCaptionHeight(16);
    m_btnNavi.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnNavi.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnNavi.LinkTextGL(pTextGL);
    m_btnNavi.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnNavi.Show(FALSE);

    m_iconNavi.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_819));
    m_iconNavi.SetPos(170+10-62,py);
	//m_iconNavi.Show(FALSE);
    m_iconNaviFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_820));
    m_iconNaviFocus.SetPos(170+10-62,py);
    m_iconNaviFocus.Show(FALSE);
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnHome.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_HOME,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
	m_btnHome.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_HOME,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif
	m_btnHome.SetSize(&sizebtn);
	m_btnHome.SetPos(170+10,py);
	m_btnHome.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
	m_btnHome.SetCaption(_T("Home"));
	m_btnHome.SetCaptionColor(&cr_dark, &cr_dark);
	m_btnHome.SetCaptionHeight(16);
	m_btnHome.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
	m_btnHome.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnNavi.Show(FALSE);

	m_iconHome.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_831));
	m_iconHome.SetPos(170+10-62,py);
	//m_iconNavi.Show(FALSE);
	m_iconHomeFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_832));
	m_iconHomeFocus.SetPos(170+10-62,py);
	m_iconHomeFocus.Show(FALSE);
#else
#if CVTE_EN_NAVI
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_btnNavi.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_NAVI,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
#else
    m_btnNavi.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_NAVI,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
		m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
#endif	//CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_btnNavi.SetSize(&sizebtn);
	m_btnNavi.SetPos(170+10,py);
	m_btnNavi.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnNavi.SetCaption(GetResourceString(IDS_STW_NAVI));
    m_btnNavi.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnNavi.SetCaptionHeight(16);
    m_btnNavi.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
    m_btnNavi.SetSubCaptionPos(BTN_CAPTION_LEFT,50,-1);
    m_btnNavi.LinkTextGL(pTextGL);
    m_btnNavi.SetCaptionPos(BTN_CAPTION_LEFT,50,8);
	//m_btnNavi.Show(FALSE);

    m_iconNavi.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_819));
    m_iconNavi.SetPos(170+10-62,py);
	//m_iconNavi.Show(FALSE);
    m_iconNaviFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_820));
    m_iconNaviFocus.SetPos(170+10-62,py);
    m_iconNaviFocus.Show(FALSE);
#endif	//CUSTOMER_ID
#endif

    py-=tag_h;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	int gap = 5;
    m_btnSteerWheelConfirm.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CONFIRM,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
    m_btnSteerWheelConfirm.SetSize((g_iClientWidth - LIST_BOX_ITEM_WIDTH - margin_x + gap)/2, 80);
	m_btnSteerWheelConfirm.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnSteerWheelConfirm.SetCaption(GetResourceString(IDS_CONFIRM));
    m_btnSteerWheelConfirm.SetPos(-(g_iClientWidth - LIST_BOX_ITEM_WIDTH - margin_x)/4 - gap, py);
	m_btnSteerWheelConfirm.SetCaptionPos(BTN_CAPTION_CENTER);
    m_btnSteerWheelConfirm.SetCaptionColor(&cr_w, &cr_w);
	m_btnSteerWheelConfirm.LinkTextGL(pTextGL);  
	m_btnSteerWheelConfirm.SetCaptionHeight(22);


    m_btnSteerWheelCancel.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CANCEL,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
    m_btnSteerWheelCancel.SetSize((g_iClientWidth - LIST_BOX_ITEM_WIDTH - margin_x + gap)/2, 80);
	m_btnSteerWheelCancel.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnSteerWheelCancel.SetCaption(GetResourceString(IDS_SETTING_STW_CALIBRATE));
    m_btnSteerWheelCancel.SetPos((g_iClientWidth - LIST_BOX_ITEM_WIDTH - margin_x)/4 + gap,py);
    m_btnSteerWheelCancel.SetCaptionColor(&cr_w, &cr_w);
    m_btnSteerWheelCancel.SetTouchRate(1.2f,1.2f);
	m_btnSteerWheelCancel.LinkTextGL(pTextGL);
	m_btnSteerWheelCancel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnSteerWheelCancel.SetCaptionHeight(22);
#else
    m_btnSteerWheelConfirm.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CONFIRM,this,
		pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
    m_btnSteerWheelConfirm.SetSize((g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x)/2,60);
	m_btnSteerWheelConfirm.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnSteerWheelConfirm.SetCaption(GetResourceString(IDS_CONFIRM));
    m_btnSteerWheelConfirm.EnableDrawBack(FALSE);
    m_btnSteerWheelConfirm.SetPos(-(g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x)/4,py);
	m_btnSteerWheelConfirm.SetCaptionPos(BTN_CAPTION_CENTER);
    m_btnSteerWheelConfirm.SetCaptionColor(&cr_dark, &cr_gray);
    //m_btnSteerWheelConfirm.SetTouchRate(1.2f,1.2f);
	//m_btnSteerWheelConfirm.Show(FALSE);
	m_btnSteerWheelConfirm.LinkTextGL(pTextGL);
	//m_btnSteerWheelConfirm.SetStatus(BS_DISABLED);    

	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
        case LANGUAGE_GERMAN:
            m_btnSteerWheelConfirm.SetCaptionHeight(20);
            break;
		case LANGUAGE_SPANISH:
        case LANGUAGE_ENGLISH:
        case LANGUAGE_RUSSIAN:
        case LANGUAGE_ITALIAN:
		case LANGUAGE_FRANCH:
			m_btnSteerWheelConfirm.SetCaptionHeight(24);
			break;
		case LANGUAGE_GREEK:
			m_btnSteerWheelConfirm.SetCaptionHeight(20);
			break;
        default:
            m_btnSteerWheelConfirm.SetCaptionHeight(25);
            break;

    }

    m_btnSteerWheelCancel.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CANCEL,this,
		pSkinManager->GetTexture(TEXID_606),NULL,
		pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
    m_btnSteerWheelCancel.SetSize((g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x)/2,60);
	m_btnSteerWheelCancel.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnSteerWheelCancel.SetCaption(GetResourceString(IDS_SETTING_STW_CALIBRATE));
    m_btnSteerWheelCancel.SetPos((g_iClientWidth-LIST_BOX_ITEM_WIDTH-margin_x)/4,py);
    m_btnSteerWheelCancel.EnableDrawBack(FALSE);
    m_btnSteerWheelCancel.SetCaptionColor(&cr_dark, &cr_gray);
    m_btnSteerWheelCancel.SetTouchRate(1.2f,1.2f);
	//m_btnSteerWheelCancel.Show(FALSE);
	m_btnSteerWheelCancel.LinkTextGL(pTextGL);
	m_btnSteerWheelCancel.SetCaptionPos(BTN_CAPTION_CENTER);

    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
        case LANGUAGE_GERMAN:
            m_btnSteerWheelCancel.SetCaptionHeight(20);
            break;
		case LANGUAGE_SPANISH:
        case LANGUAGE_ENGLISH:
        case LANGUAGE_RUSSIAN:
        case LANGUAGE_ITALIAN:
		case LANGUAGE_FRANCH:
			m_btnSteerWheelCancel.SetCaptionHeight(24);
			break;
		case LANGUAGE_GREEK:
			m_btnSteerWheelCancel.SetCaptionHeight(20);
			break;
        default:
            m_btnSteerWheelCancel.SetCaptionHeight(25);
            break;
    }
#endif

    m_tagSteerWheelWindow.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_609),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    m_tagSteerWheelWindow.SetSize(448,224);
#if CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
	m_tagSteerWheelWindow.SetPos(0,0);
#else
	#if (CUSTOMER_ID == CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
	m_tagSteerWheelWindow.SetPos(0,-bar_h-LIST_BOX_TAG_HEIGHT*2-3);
	//	//					    m_tagSteerWheelPanel.SetPosIni(0,iSteerWheelPanelPy-LIST_BOX_TAG_HEIGHT*2-LIST_BOX_TAG_HEIGHT/2-3);
	//						SetPosIni(LIST_BOX_ITEM_WIDTH/2, LIST_BOX_TAG_HEIGHT*2+3);
	#else
	m_tagSteerWheelWindow.SetPos(0,-bar_h-LIST_BOX_TAG_HEIGHT*3-3);
	//	//					    m_tagSteerWheelPanel.SetPosIni(0,iSteerWheelPanelPy-LIST_BOX_TAG_HEIGHT*3-LIST_BOX_TAG_HEIGHT/2-3);
	//						SetPosIni(LIST_BOX_ITEM_WIDTH/2, LIST_BOX_TAG_HEIGHT*3+3);
	#endif
#endif //CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
    m_tagSteerWheelWindow.SetCaption(GetResourceString(IDS_SETTING_STEER_WHEEL_PROMPT));
    m_tagSteerWheelWindow.LinkTextGL(pTextGL);
	m_tagSteerWheelWindow.SetCaptionHeight(caption_h);
    m_tagSteerWheelWindow.SetCaptionColor(&cr_dark);
	m_tagSteerWheelWindow.SetCaptionPos(BTN_CAPTION_CENTER,0);
    m_tagSteerWheelWindow.Show(FALSE);

    RETAILMSG(1, (TEXT("MSG: #### QuerySteerWheelValue ###\r\n")));
    QuerySteerWheelValue();
#if 0
    py-=tag_h+30;

    m_tagTitleRearView.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRearView.SetSize(cx,tag_h);
	m_tagTitleRearView.SetCaption(GetResourceString(IDS_SETTING_REARVIEW_SETTING));
	m_tagTitleRearView.LinkTextGL(pTextGL);
	m_tagTitleRearView.SetPos(10,py+12);
	m_tagTitleRearView.SetCaptionHeight(caption_h);
	m_tagTitleRearView.EnableDrawBack(FALSE);
	m_tagTitleRearView.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleRearView.SetCaptionColor(&cr_dark);

    m_tagTitleRearView2.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRearView2.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleRearView2.SetCaption(GetResourceString(IDS_SETTING_REARVIEW_TITLE));
	m_tagTitleRearView2.LinkTextGL(pTextGL);
	m_tagTitleRearView2.SetPos(10,py-16);
	m_tagTitleRearView2.SetCaptionHeight(caption3_h);
	m_tagTitleRearView2.EnableDrawBack(FALSE);
	m_tagTitleRearView2.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleRearView2.SetCaptionColor(&cr_gray2);

    m_iconRearView.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_830));
	m_iconRearView.SetPos(170+10+32,py);

	py-=tag_h+6;

    m_btnNTSC_MJ.Initialize(IDC_REARVIEW_COLORSIGNAL_NTSC_MJ,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnNTSC_MJ.SetSize(&sizebtn);
	m_btnNTSC_MJ.SetPos(-170-10,py);
	m_btnNTSC_MJ.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnNTSC_MJ.SetCaption(_T("NTSC(STD)"));
    m_btnNTSC_MJ.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnNTSC_MJ.SetCaptionHeight(16);
    m_btnNTSC_MJ.LinkTextGL(pTextGL);
    m_btnNTSC_MJ.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNTSC_MJ.Enable(FALSE);

    m_btnNTSC_443.Initialize(IDC_REARVIEW_COLORSIGNAL_NTSC_443,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnNTSC_443.SetSize(&sizebtn);
	m_btnNTSC_443.SetPos(0,py);
	m_btnNTSC_443.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnNTSC_443.SetCaption(_T("NTSC(4.43M)"));
    m_btnNTSC_443.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnNTSC_443.SetCaptionHeight(16);
    m_btnNTSC_443.LinkTextGL(pTextGL);
    m_btnNTSC_443.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNTSC_443.Enable(FALSE);

    m_btnPAL_M.Initialize(IDC_REARVIEW_COLORSIGNAL_PAL_M,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnPAL_M.SetSize(&sizebtn);
	m_btnPAL_M.SetPos(170+10,py);
	m_btnPAL_M.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnPAL_M.SetCaption(_T("PAL(M)"));
    m_btnPAL_M.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnPAL_M.SetCaptionHeight(16);
    m_btnPAL_M.LinkTextGL(pTextGL);
    m_btnPAL_M.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnPAL_M.Enable(FALSE);

    py-=tag_h;

    m_btnPAL_60.Initialize(IDC_REARVIEW_COLORSIGNAL_PAL_60,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnPAL_60.SetSize(&sizebtn);
	m_btnPAL_60.SetPos(-170-10,py);
	m_btnPAL_60.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnPAL_60.SetCaption(_T("PAL(60)"));
    m_btnPAL_60.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnPAL_60.SetCaptionHeight(16);
    m_btnPAL_60.LinkTextGL(pTextGL);
    m_btnPAL_60.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnPAL_60.Enable(FALSE);

    m_btnPAL_BGHID.Initialize(IDC_REARVIEW_COLORSIGNAL_PAL_BGHID,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnPAL_BGHID.SetSize(&sizebtn);
	m_btnPAL_BGHID.SetPos(0,py);
	m_btnPAL_BGHID.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnPAL_BGHID.SetCaption(_T("PAL(STD)"));
    m_btnPAL_BGHID.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnPAL_BGHID.SetCaptionHeight(16);
    m_btnPAL_BGHID.LinkTextGL(pTextGL);
    m_btnPAL_BGHID.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnPAL_BGHID.Enable(FALSE);

    m_btnPAL_COM_N.Initialize(IDC_REARVIEW_COLORSIGNAL_PAL_COMBIN_N,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnPAL_COM_N.SetSize(&sizebtn);
	m_btnPAL_COM_N.SetPos(170+10,py);
	m_btnPAL_COM_N.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnPAL_COM_N.SetCaption(_T("PAL(N)"));
    m_btnPAL_COM_N.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnPAL_COM_N.SetCaptionHeight(16);
    m_btnPAL_COM_N.LinkTextGL(pTextGL);
    m_btnPAL_COM_N.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnPAL_COM_N.Enable(FALSE);

    py-=tag_h;

    m_btnSECAM.Initialize(IDC_REARVIEW_COLORSIGNAL_SECAM,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnSECAM.SetSize(&sizebtn);
	m_btnSECAM.SetPos(-170-10,py);
	m_btnSECAM.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnSECAM.SetCaption(_T("SECAM(STD)"));
    m_btnSECAM.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnSECAM.SetCaptionHeight(16);
    m_btnSECAM.LinkTextGL(pTextGL);
    m_btnSECAM.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnSECAM.Enable(FALSE);

    m_btnSECAM_525.Initialize(IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_PLUS,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,NULL,NULL,NULL,NULL);
	m_btnSECAM_525.SetSize(&sizebtn);
	m_btnSECAM_525.SetPos(0,py);
	m_btnSECAM_525.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnSECAM_525.SetCaption(_T("SECAM(525)"));
    m_btnSECAM_525.SetCaptionColor(&cr_dark, &cr_dark);
    m_btnSECAM_525.SetCaptionHeight(16);
    m_btnSECAM_525.LinkTextGL(pTextGL);
    m_btnSECAM_525.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnSECAM_525.Enable(FALSE);

	py-=tag_h;

    m_btnRearReset.LinkTextGL(pTextGL);
    m_btnRearReset.SetCaptionPos(BTN_CAPTION_LEFT,30,0);
    //m_btnSteerWheelConfirm.SetStatus(BS_DISABLED);
    m_btnRearReset.Initialize(IDC_REARVIEW_COLORSIGNAL_RESET,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    m_btnRearReset.SetSize(100,60);
	m_btnRearReset.SetNotifyFunc(&CPanelSettingAdvanced::OnNotify,this);
    m_btnRearReset.SetCaption(GetResourceString(IDS_RESET));
    m_btnRearReset.EnableDrawBack(FALSE);
    m_btnRearReset.SetCaptionColor(&cr_dark, &cr_gray);
    m_btnRearReset.SetTouchRate(1.2f,1.2f);

    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
        case LANGUAGE_SPANISH:
            m_btnRearReset.SetCaptionHeight(24);
    	    m_btnRearReset.SetPos(80+100,py);
            break;
        case LANGUAGE_GERMAN:
            m_btnRearReset.SetCaptionHeight(20);
    	    m_btnRearReset.SetPos(80+55,py);
            break;
        case LANGUAGE_KOREAN:
            m_btnRearReset.SetCaptionHeight(25);
    	    m_btnRearReset.SetPos(80+130,py);
            break;
        case LANGUAGE_ENGLISH:
            m_btnRearReset.SetCaptionHeight(24);
    	    m_btnRearReset.SetPos(80+130,py);
            break;
        case LANGUAGE_RUSSIAN:
            m_btnRearReset.SetCaptionHeight(24);
    	    m_btnRearReset.SetPos(80+110,py);
            break;
        default:
            m_btnRearReset.SetCaptionHeight(25);
    	    m_btnRearReset.SetPos(80+130,py);
            break;
    }

	OnRearColorSignalChange(m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard);
#endif


#endif	//CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE

	int iPanelHeight;
	
	if (m_bSteerWheelBlockShow)
	{
		iPanelHeight = g_iClientHeight/2 - py + tag_h;

		if(iPanelHeight < g_iClientHeight)
			iPanelHeight = g_iClientHeight;
	}
	else
	{
		iPanelHeight = g_iClientHeight;
	}

	SetSize(g_iClientWidth-box_size_x,iPanelHeight);

	return TRUE;
}


void CALLBACK CPanelSettingAdvanced::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingAdvanced *panel=(CPanelSettingAdvanced*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}

void CPanelSettingAdvanced::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
    //RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::HandleControlMSG] idControl:%d wMsg:%d wPara:%d\r\n"),idControl,wMsg,wPara));
    //RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::HandleControlMSG] m_nSteerWheelCurrentStatus:%d m_bSteerWheelKeyCalibrating:%d m_pCommander:%d\r\n"),m_nSteerWheelCurrentStatus,m_bSteerWheelKeyCalibrating,m_pCommander));

	switch (idControl)
	{
#if 0
    //Camera
	case IDC_SETTING_BTN_SET_CAMERA_TYPE:
		{
			RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingAdvanced::HandleControlMSG]: IDC_SETTING_BTN_SET_CAMERA_TYPE: \r\n")));
			if(m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType == 0 || m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType == 1)//CVBS
			{
				m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType = 2;
				m_tagValueCameraType.SetCaption(_T("RGB"));
				SetCamerType(2);
			}
			else if(m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType == 2)//ARGB
			{
				m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType = 0;
				m_tagValueCameraType.SetCaption(_T("CVBS"));
				SetCamerType(0);
			}
			PlaySoundEffect(0);
		}
		break;
#endif
		case IDC_SETTING_BTN_ADVANCE_RETURN:
			{
				RETAILMSG(1,(TEXT("MSG:################# m_bLock[%d] ###################\r\n"),m_bLock));
				if(m_bLock)
				{
					::PostMessage(m_hMainWnd,WM_PASSWORD_BOX_CMD,1,NULL);
				}
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_NONE,PAGE_SETTINGS_GENERAL);
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_LOCK:
			{
				if(m_bLock)
				{
					Lock(FALSE);
				}
				else
				{
					Lock(TRUE);
				}
				PlaySoundEffect(0);
			}
			break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CONFIRM:
	        {
				m_btnStreerWheelSpread.Enable(TRUE);
	            if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	            {
	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_WAIT_FOR_NET_READY;
	                m_pCommander->SendCmd(_ARM_STW_CMD_KEY,1);
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	                if( m_bRDSEnable )
	                {
	                    SendRadioCommand(RADIO_CMD_RDS_SWITCH,1);   //Open RDS
	                }
#endif
	                //m_btnSteerWheelConfirm.SetStatus(BS_DISABLED);
					m_btnSteerWheelCancel.SetCaption(GetResourceString(IDS_SETTING_STW_CALIBRATE));

	//	                m_iconVolMinus.Show(FALSE);
	//	                m_iconVolMinusFocus.Show(TRUE);
	//
	//	                m_iconVolPlus.Show(FALSE);
	//	                m_iconVolPlusFocus.Show(TRUE);
	//
	//	                m_iconVolMute.Show(FALSE);
	//	                m_iconVolMuteFocus.Show(TRUE);
	//
	//	                m_iconPrev.Show(FALSE);
	//	                m_iconPrevFocus.Show(TRUE);
	//
	//	                m_iconNext.Show(FALSE);
	//	                m_iconNextFocus.Show(TRUE);
	//
	//	                m_iconSeek.Show(FALSE);
	//	                m_iconSeekFocus.Show(TRUE);
	//
	//	                m_iconCall.Show(FALSE);
	//	                m_iconCallFocus.Show(TRUE);
	//
	//	                m_iconHangup.Show(FALSE);
	//	                m_iconHangupFocus.Show(TRUE);
	//
	//	                m_iconMode.Show(FALSE);
	//	                m_iconModeFocus.Show(TRUE);
	//
	//	                m_iconNavi.Show(FALSE);
	//	                m_iconNaviFocus.Show(TRUE);
	//
	//	                m_iconSpeech.Show(FALSE);
	//	                m_iconSpeechFocus.Show(TRUE);
	//
	//	                m_iconPower.Show(FALSE);
	//	                m_iconPowerFocus.Show(TRUE);
	            }

	            PlaySoundEffect(0);
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CANCEL:
	        {
				//STW logic has problems, need fixed:
				//"CANCEL" button do not send any required command to MCU
				//also RDS can't recover properly.
	            if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_READY ||
					m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	            {
					ShowNotificationBox(m_hMainWnd,IDC_SETTING_BTN_ADVANCE_STEERWHEEL_RESET,
						&CPanelSettingAdvanced::OnNotify,
						this,
						0,
						GetResourceString(IDS_SETTING_STEER_WHEEL),
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
						_T("Reset Controls?"),
#else
						GetResourceString(IDS_SETTING_STEER_WHEEL_RECALIBRATE),
#endif
						0);
	            }
	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START ||
	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_WAIT_FOR_NET_READY)
	            {
					ClearSTW();
	            }

				PlaySoundEffect(0);
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_RESET:
	        {
	            if(wMsg == NB_OK)
	            {
	                if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	                    m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	                    m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_READY)
	                {
						StartSTW();
	                }
	            }
	    		PlaySoundEffect(0);
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_MINUS:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	            {
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_MINUS\r\n")));
	        		m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_VOL_MINUS);
	        		PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	//	                m_iconVolMinus.Show(FALSE);
	                m_iconVolMinusFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_VOL_MINUS;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_PLUS:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	            {
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_PLUS\r\n")));
	        		m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_VOL_PLUS);
	        		PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconVolPlus.Show(FALSE);
	                m_iconVolPlusFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_VOL_PLUS;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,CPanelSettingAdvanced::OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_MUTE:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	            {
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_VOL_MUTE\r\n")));
	        		m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_VOL_MUTE);
	        		PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconVolMute.Show(FALSE);
	                m_iconVolMuteFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_VOL_MUTE;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);

	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_PREV:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_PREV\r\n")));
	    			m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_PREV);
	        		PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconPrev.Show(FALSE);
	                m_iconPrevFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_PREV;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_NEXT:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_NEXT\r\n")));
	    			m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_NEXT);
	        		PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconNext.Show(FALSE);
	                m_iconNextFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_NEXT;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SEEK:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SEEK\r\n")));
	    			m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_SEEK);
	        		PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconSeek.Show(FALSE);
	                m_iconSeekFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_SEEK;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CALL:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_CALL\r\n")));
	    			m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_PHONE_CALL);
	        		PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconCall.Show(FALSE);
	                m_iconCallFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_PHONE_CALL;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_HANGUP:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_HANGUP\r\n")));
	    			m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_PHONE_HANGUP);
	        		PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconHangup.Show(FALSE);
	                m_iconHangupFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_PHONE_HANGUP;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_NAVI:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_NAVI\r\n")));
	                m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_NAVI);
	                PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconNavi.Show(FALSE);
	                m_iconNaviFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_NAVI;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }

	        }
	        break;
		case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_HOME:
			{
				if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
				{
					RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_HOME\r\n")));
					m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_HOME);
					PlaySoundEffect(0);
					m_bSteerWheelKeyCalibrating = TRUE;
					m_tagSteerWheelWindow.Show(TRUE);
					m_iconHome.Show(FALSE);
					m_iconHomeFocus.Show(TRUE);
					m_nCalibCurrentKey = KEY_STW_HOME;
					SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
				}				
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
			}
			break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_MODE:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_MODE\r\n")));
	                m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_MODE);
	                PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconMode.Show(FALSE);
	                m_iconModeFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_MODE;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SPEECH:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SPEECH\r\n")));
	                m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_SPEECH);
	                PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconSpeech.Show(FALSE);
	                m_iconSpeechFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_SPEECH;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;
        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_POWER:
	        {
	            if(!m_bSteerWheelKeyCalibrating && m_pCommander && m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
	    		{
	                RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_ADVANCE_STEERWHEEL_POWER\r\n")));
	                m_pCommander->SendCmd(_ARM_CHANGE_STW_KEY,KEY_STW_POWER);
	                PlaySoundEffect(0);
	                m_bSteerWheelKeyCalibrating = TRUE;
	                m_tagSteerWheelWindow.Show(TRUE);
	                m_iconPower.Show(FALSE);
	                m_iconPowerFocus.Show(TRUE);
	                m_nCalibCurrentKey = KEY_STW_POWER;
	                SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL,10000,OnTimerProc);
	            }
	//	            else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
	//	                m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_ERROR)
	//	            {
	//	                m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	//	            }
	        }
	        break;

        case IDC_SETTING_BTN_ADVANCE_STEERWHEEL_SPREAD:
	        {
				RETAILMSG(1, (TEXT("MSG:[%s]: m_bSteerWheelBlockShow:%d\r\n"),TEXT(__FUNCTION__),m_bSteerWheelBlockShow));
	            if(m_bSteerWheelBlockShow)
	            {
	                
					VIEW_STATE vs = m_btnStreerWheelSpread.GetCurViewState();
					vs.fRotateZ += (90);
					if(vs.fRotateZ < -360)
						vs.fRotateZ = 0;
					m_btnStreerWheelSpread.SetAimViewState(&vs);

	                m_bSteerWheelBlockShow = FALSE;
					m_tagSteerWheelPanel.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h+6);

					m_tagSteerWheelPanel.SetPosIni(0,iSteerWheelPanelPy-3);
					SetSize(g_iClientWidth-box_size_x,g_iClientHeight);
					SetPosIni(LIST_BOX_ITEM_WIDTH/2, 0);

#if (CVTE_EN_RDS || CVTE_EN_RBDS)
					if( m_bRDSEnable )
					{
						SendRadioCommand(RADIO_CMD_RDS_SWITCH,1);
					}
#endif
	            }
	            else
	            {
	                m_bSteerWheelBlockShow = TRUE;
					VIEW_STATE vs = m_btnStreerWheelSpread.GetCurViewState();
					vs.fRotateZ -= (90);
					if(vs.fRotateZ < -360)
						vs.fRotateZ = 0;
					m_btnStreerWheelSpread.SetAimViewState(&vs);

					m_tagSteerWheelPanel.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h*6+6);
					m_tagSteerWheelPanel.SetPosIni(0,iSteerWheelPanelPy-LIST_BOX_TAG_HEIGHT*2-LIST_BOX_TAG_HEIGHT/2-3);

#if (CUSTOMER_ID == CUSTOMER_DAIICHI) && (CUSTOMER_ID != CUSTOMER_DAIICHI_MITSUBISHI_L200)
					SetSize(g_iClientWidth-box_size_x,g_iClientHeight + bar_h+LIST_BOX_TAG_HEIGHT * 2);
#else
					//SetSize(g_iClientWidth-box_size_x,g_iClientHeight + bar_h+LIST_BOX_TAG_HEIGHT * 3);


					int iPanelHeight = g_iClientHeight/2 - iSteerWheelPanelPy + tag_h*6;

					if(iPanelHeight < g_iClientHeight)
						iPanelHeight = g_iClientHeight;

					SetSize(g_iClientWidth-box_size_x,iPanelHeight);
#endif
					SetPosIni(LIST_BOX_ITEM_WIDTH/2, g_iClientHeight/2-40-20-iSteerWheelPanelPy);

					if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE && !m_bCalibrated)
					{
						StartSTW();
					}
	            }
	            PlaySoundEffect(0);
	        }
	        break;
#if 0
		case IDC_REARVIEW_COLORSIGNAL_NTSC_MJ:
			OnRearColorSignalChange(OEMCONFIG_COLORSIGNAL_NTSM_MJ);
			break;
		case IDC_REARVIEW_COLORSIGNAL_NTSC_443:
			OnRearColorSignalChange(OEMCONFIG_COLORSIGNAL_NTSC_443);
			break;
		case IDC_REARVIEW_COLORSIGNAL_PAL_M:
			OnRearColorSignalChange(OEMCONFIG_COLORSIGNAL_PAL_M);
			break;
		case IDC_REARVIEW_COLORSIGNAL_PAL_60:
			OnRearColorSignalChange(OEMCONFIG_COLORSIGNAL_PAL_60);
			break;
		case IDC_REARVIEW_COLORSIGNAL_PAL_BGHID:
			OnRearColorSignalChange(OEMCONFIG_COLORSIGNAL_PAL_BGHID);
			break;
		case IDC_REARVIEW_COLORSIGNAL_PAL_COMBIN_N:
			OnRearColorSignalChange(OEMCONFIG_COLORSIGNAL_PAL_COMBINATION_N);
			break;
		case IDC_REARVIEW_COLORSIGNAL_SECAM:
			OnRearColorSignalChange(OEMCONFIG_COLORSIGNAL_SECAM);
			break;
		case IDC_REARVIEW_COLORSIGNAL_SECAM_525:
			OnRearColorSignalChange(OEMCONFIG_COLORSIGNAL_SECAM_525);
			break;
		case IDC_REARVIEW_COLORSIGNAL_RESET:
			{
				if(m_bRearViewColorSignalReset)
				{
					m_bRearViewColorSignalReset = FALSE;
					m_btnNTSC_MJ.Enable(FALSE);
					m_btnNTSC_443.Enable(FALSE);
					m_btnPAL_M.Enable(FALSE);
					m_btnPAL_60.Enable(FALSE);
					m_btnPAL_BGHID.Enable(FALSE);
					m_btnPAL_COM_N.Enable(FALSE);
					m_btnSECAM.Enable(FALSE);
					m_btnSECAM_525.Enable(FALSE);
    				m_btnRearReset.SetCaption(GetResourceString(IDS_RESET));
				}
				else
				{
					m_bRearViewColorSignalReset = TRUE;
					m_btnNTSC_MJ.Enable(TRUE);
					m_btnNTSC_443.Enable(TRUE);
					m_btnPAL_M.Enable(TRUE);
					m_btnPAL_60.Enable(TRUE);
					m_btnPAL_BGHID.Enable(TRUE);
					m_btnPAL_COM_N.Enable(TRUE);
					m_btnSECAM.Enable(TRUE);
					m_btnSECAM_525.Enable(TRUE);
    				m_btnRearReset.SetCaption(GetResourceString(IDS_CONFIRM));

				}
			}
			break;
#endif
		case IDC_SETTING_SLI_BREAK_TEST:
			{
				//ConfigBrakeTest(wPara);
				if(m_pCommander)
				{
					m_pCommander->SendCmd(_ARM_BREAKE_TEST_KEY,wPara?1:0);
				}
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_SET_DRIVE_TYPE:
			{
				if(m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType == 1)//right
				{
					m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType = 0;
					m_tagValueDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_LEFT));
				}
				else
				{
					m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType = 1;
					m_tagValueDriveType.SetCaption(GetResourceString(IDS_SETTING_DRIVE_RIGHT));
				}
#if CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
				//Update Driver Type Info to MCU
				if (m_pCommander)
				{
					BYTE buf = m_pSkinManager->GetConfig()->GetCurParam()->dwDriveType;
					m_pCommander->SendExtenCmd(_ARM_EXTEN_TAPE_SIDE,&buf,1);
				}	
#endif
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_SLI_LAMP_CONTROL:
				{
					//ConfigLampControl(wPara);
					if(m_pCommander)
					{
						m_pCommander->SendCmd(_ARM_LAMP_CONTROL_KEY,wPara?1:0);
					}
					PlaySoundEffect(0);
				}
				break;
//		case IDC_SETTING_PASSWORD_NUMBER_0:
//				Input('0');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_1:
//				Input('1');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_2:
//				Input('2');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_3:
//				Input('3');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_4:
//				Input('4');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_5:
//				Input('5');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_6:
//				Input('6');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_7:
//				Input('7');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_8:
//				Input('8');
//				break;
//		case IDC_SETTING_PASSWORD_NUMBER_9:
//				Input('9');
//				break;
	}
}

//	void CPanelSettingAdvanced::Input(wchar_t c)
//	{
//		wchar_t str[8];
//		swprintf_s(str,8,_T("%c"),c);
//
//		m_strPassword += str;
//
//		RETAILMSG(1, (TEXT("MSG:[%s]: [%s]\r\n"), TEXT(__FUNCTION__),m_strPassword.String()));
//
//		if(m_strPassword.Size())
//		{
//			if(m_strPassword.Size()<=PASSWORD_MAX_LENGTH)
//			{
//				for(int i=0;i<m_strPassword.Size();i++)
//				{
//					if(!m_btagNumberInput[i])
//					{
//						//m_tagNumber[i].SetCaption(_T("*"));
//						m_tagNumber[i].SetCaption(str);
//						m_btagNumberInput[i] = TRUE;
//					}
//				}
//
//				if(m_strPassword.Size()==PASSWORD_MAX_LENGTH)
//				{
//					if(!VerifyPassword(m_strPassword.String()))
//					{
//						SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_PASSWORD_CLEAR,500,OnTimerProc);
//					}
//					else
//					{
//						RETAILMSG(1, (TEXT("MSG:[%s]: Verify successfully.\r\n"), TEXT(__FUNCTION__)));
//					}
//				}
//			}
//			else
//			{
//				//m_tagDialString.SetCaption(m_strPassword.String()+m_strPassword.Size()-maxlen);
//			}
//		}
//	}
//	void CPanelSettingAdvanced::ClearInput(int iNumChar)
//	{
//		if(m_strPassword.Size()>0)
//		{
//			if(iNumChar<=0)
//			{
//				m_strPassword = NULL;
//				for(int i=0;i<PASSWORD_MAX_LENGTH;i++)
//				{
//					m_btagNumberInput[i] = FALSE;
//					m_tagNumber[i].SetCaption(_T(""));
//				}
//			}
//			else
//			{
//				int iClearNum = min(iNumChar,PASSWORD_MAX_LENGTH);
//				m_strPassword -= iClearNum;
//				for(int i=1;i<=iClearNum;i++)
//				{
//					m_btagNumberInput[PASSWORD_MAX_LENGTH-i] = FALSE;
//					m_tagNumber[PASSWORD_MAX_LENGTH-i].SetCaption(_T(""));
//				}
//			}
//
//			if(m_strPassword.Size())
//			{
//				if(m_strPassword.Size()<=PASSWORD_MAX_LENGTH)
//				{
//					//m_tagDialString.SetCaption(m_strPassword.String());
//				}
//				else
//				{
//					//m_tagDialString.SetCaption(m_strPassword.String()+m_strPassword.Size()-maxlen);
//				}
//			}
//			else
//			{
//				//m_tagDialString.SetCaption(NULL);
//			}
//		}
//	}
//
//
//	BOOL CPanelSettingAdvanced::VerifyPassword(LPCTSTR lpPassword)
//	{
//		BOOL bRet = FALSE;
//		if (lpPassword == NULL)
//		{
//			return bRet;
//		}
//	    if (wcslen(lpPassword) < PASSWORD_MAX_LENGTH)
//	    {
//	        return bRet;
//	    }
//		if(lstrcmpi(_T("0521"), lpPassword) == 0)
//		{
//			m_bVerify = TRUE;
//			bRet = TRUE;
//		}
//
//		return bRet;
//	}
//	void CPanelSettingAdvanced::RefreshPassword(void)
//	{
//		//wchar_t string[PASSWORD_MAX_LENGTH+1] = {0};
//		//memcpy(string, m_strPassword.String(), m_strPassword.Size());
//		if (m_bPasswordHide)
//		{
//			for (DWORD i=0; i<m_strPassword.Size(); i++)
//			{
//				if(m_bPasswordHide)
//				{
//					m_tagNumber[i].SetCaption(_T("*"));
//				}
//	//				else
//	//				{
//	//					m_tagNumber[i].SetCaption(string[i]);
//	//				}
//			}
//		}
//
//	    //m_btnEnter.Enable(m_strPassword.Size() >= CVT_DEF_PASSWORD_MIN_LENGTH);
//	}

void CPanelSettingAdvanced::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);
	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}

    //RETAILMSG(1,(_T("MSG:[%s]: dwType:%d,dwID:0x%x,val:0x%x\r\n"),TEXT(__FUNCTION__),dwType,dwID,pCommander->GetItemData(dwID)));


	if(dwType == 0)//package
	{
//			//reverse polar:
//			m_sliReversePolar.SetValue(pCommander->GetItemData(MCU_RESERVE_POLA)?10:0);

		//break test:
		m_sliBrakeTest.SetValue(pCommander->GetItemData(MCU_BREAKE_DETECT)?10:0);

		//lamp:
		m_sliLampControl.SetValue(pCommander->GetItemData(MCU_LIGHT_CONTROL)?10:0);

//		    UpdateSteerWheelPara(STEER_WHEEL_RESISTANCE,pCommander->GetItemData(MCU_SET_STUDY_RESISTANCE_KEY));

//			UpdateSteerWheelPara(KEY_STW_VOL_PLUS,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_VOL_MINUS,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_VOL_MUTE,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_PREV,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_NEXT,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_SEEK,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_PHONE_CALL,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_MODE,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_POWER,pCommander->GetItemData(MCU_STW_VALUE_KEY));
//			UpdateSteerWheelPara(KEY_STW_SPEECH,pCommander->GetItemData(MCU_STW_VALUE_KEY));
	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
            //RETAILMSG(1, (TEXT("APP:MSG:OnMCUCmd: dwID[%d]\r\n"),dwID));
			case MCU_KEY_STUDY_RESISTANCE:
				RETAILMSG(1, (TEXT("MSG:OnMCUCmd: MCU_KEY_STUDY_RESISTANCE[%d] \r\n"),pCommander->GetItemData(MCU_KEY_STUDY_RESISTANCE)));
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
				wchar_t strNet[MAX_PATH];
				swprintf_s(strNet,MAX_PATH,_T("%s NET%d"),GetResourceString(IDS_SETTING_STEER_WHEEL),pCommander->GetItemData(MCU_KEY_STUDY_RESISTANCE));
				m_tagTitleStreerWheel1.SetCaption(strNet);
#endif
				break;
            case MCU_CHANGE_STW_KEY:
                BYTE  mcudata;
                mcudata = pCommander->GetItemData(MCU_CHANGE_STW_KEY);
                if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START)
                {
                    if(m_nCalibCurrentKey == mcudata)
                    {
                        //RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::OnMCUCmd]: Calibrating...  key[%d] is done.\r\n"),m_nCalibCurrentKey));
                        m_tagSteerWheelWindow.Show(FALSE);
                        m_bSteerWheelKeyCalibrating = FALSE;
                        KillTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL);
                    }
                }
                else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_READY)
                {
                    m_nCalibCurrentKey = mcudata;
                    //RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::OnMCUCmd]: Net reday!  m_nCalibCurrentKey[%d]\r\n"),m_nCalibCurrentKey));
                }
                break;
			case MCU_STW_VALUE_KEY:
				{
                    BYTE  mcudata = 0;
                    mcudata = pCommander->GetItemData(dwID);

//	                    if(CheckPara(mcudata))
//	                    {
//	                        m_steerwheelkey[m_nQueryKey].keyvalue = mcudata;
//	                        m_steerwheelkey[m_nQueryKey].valid = TRUE;
//	                    }
//	                    else
//	                    {
//	                        mcudata = -1;
//	                        m_steerwheelkey[m_nQueryKey].keyvalue = -1;
//	                        m_steerwheelkey[m_nQueryKey].valid = FALSE;
//	                    }

                    if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_READY || m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_QUERY)
                    {
                        //UpdateSteerWheelPara(m_nCalibCurrentKey,mcudata);
                        UpdateSteerWheelPara(m_nCalibCurrentKey,mcudata);
					    //RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::OnMCUCmd]: Update Key[0x%x] [%d] \r\n"),m_nCalibCurrentKey, mcudata));

                        //m_btnSteerWheelConfirm.SetStatus(BS_FOCUS);
                        if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_QUERY)
                        {
                            m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_IDLE;
                            m_nQueryKey++;
                            //RETAILMSG(1, (TEXT("APP:MSG:OnMCUCmd: m_nSteerWheelCurrentStatus[%d] and m_nQueryKey[0x%x]\r\n"),m_nSteerWheelCurrentStatus,m_nQueryKey));
                        }
                    }
				}
                break;
			case MCU_STW_CMD_KEY:
				{
                    BYTE  mcudata;
					mcudata = pCommander->GetItemData(MCU_STW_CMD_KEY);
					//RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::OnMCUCmd]: MCU_STW_CMD_KEY[%d]\r\n"),mcudata));
					if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_STOP)
					{
					    RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::OnMCUCmd]: MCU=>ARM [0xBA,%d]\r\n"),mcudata));
					}
                    else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_WAIT_FOR_NET_READY)
                    {
						m_bSteerWheelKeyCalibrating = FALSE;
                        if(mcudata == 0X01)
                        {
                            m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_NET_READY;
                            //m_btnSteerWheelConfirmFocus.Show(TRUE);
                            RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::OnMCUCmd]: MCU=>ARM [0xBA,1] net is ready.\r\n")));
                        }
                        else
                        {
                            m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_NET_ERROR;
                            //m_btnSteerWheelConfirm.SetStatus(BS_UP);
                            RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::OnMCUCmd]: MCU=>ARM [0xBA,0] net is not ready.\r\n")));
							ClearSTW();
                        }
                    }
                    else if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_NET_READY)
                    {
                        RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::OnMCUCmd]: MCU send error cmd [0xBA,%d].\r\n"),mcudata));
                    }
				}
                break;
    		case MCU_UPDATE_STATUS:
    			{
                    if(!m_bMCUUpdating)
    				{
    					RETAILMSG(1,(_T("MSG:[CPanelSettingAdvanced::OnMCUCmd]: updating: %d\r\n"),pCommander->GetItemData(MCU_UPDATE_STATUS)));
    					m_bMCUUpdating = (pCommander->GetItemData(MCU_UPDATE_STATUS) == 2) ;
    				}
    			}
    			break;
		}
	}
}

BOOL CPanelSettingAdvanced::CheckPara(BYTE nSet)
{
    int samenum = 0;
    for(int i=0;i<KEY_STW_LASTKEY-KEY_STW_FIRSTKEY;i++)
    {
        if(nSet == m_steerwheelkey[i].keyvalue)
        {
            samenum++;
        }
    }

    return (samenum == 0);
}

void CPanelSettingAdvanced::ClearPara()
{
    memset(&m_steerwheelkey, 0, sizeof(m_steerwheelkey));
    m_nQueryKey = 0;
}

void CPanelSettingAdvanced::UpdateSteerWheelPara(BYTE item,INT iSet)
{
	STEERWHEELKEY * pSTW = new STEERWHEELKEY;
	pSTW->keyname = item;
	pSTW->keyvalue = iSet;
	pSTW->valid = (iSet>0 && iSet<0xFF)?TRUE:FALSE;
	//RETAILMSG(1, (TEXT("MSG:[CPanelSettingAdvanced::UpdateSteerWheelPara] keyname[0x%x] keyvalue[%d] valid[%d].\r\n"),pSTW->keyname, pSTW->keyvalue,pSTW->valid));

	m_listSTW.Add(pSTW,item);
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
	wchar_t str[16];
#endif

	switch (item)
	{
//		case STEER_WHEEL_RESISTANCE:
//			{
//				m_btnR1.SetStatus(iSet==STEER_WHEEL_R1?BS_FOCUS:BS_UP);
//				m_btnR2.SetStatus(iSet==STEER_WHEEL_R2?BS_FOCUS:BS_UP);
//				m_btnR3.SetStatus(iSet==STEER_WHEEL_R3?BS_FOCUS:BS_UP);
//				m_btnR4.SetStatus(iSet==STEER_WHEEL_R4?BS_FOCUS:BS_UP);
//			}
//			break;
    case KEY_STW_VOL_PLUS:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnVolPlus.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconVolPlus.Show(TRUE);
                m_iconVolPlusFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnVolPlus.SetSubCaption(str);
#else
        		m_btnVolPlus.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconVolPlus.Show(FALSE);
                m_iconVolPlusFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_VOL_MINUS:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnVolMinus.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconVolMinus.Show(TRUE);
                m_iconVolMinusFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnVolMinus.SetSubCaption(str);
#else
        		m_btnVolMinus.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconVolMinus.Show(FALSE);
                m_iconVolMinusFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_VOL_MUTE:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnVolMute.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconVolMute.Show(TRUE);
                m_iconVolMuteFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnVolMute.SetSubCaption(str);
#else
        		m_btnVolMute.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconVolMute.Show(FALSE);
                m_iconVolMuteFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_PREV:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnPrev.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconPrev.Show(TRUE);
                m_iconPrevFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnPrev.SetSubCaption(str);
#else
        		m_btnPrev.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconPrev.Show(FALSE);
                m_iconPrevFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_NEXT:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnNext.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconNext.Show(TRUE);
                m_iconNextFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnNext.SetSubCaption(str);
#else
        		m_btnNext.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconNext.Show(FALSE);
                m_iconNextFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_SEEK:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnSeek.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconSeek.Show(TRUE);
                m_iconSeekFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnSeek.SetSubCaption(str);
#else
        		m_btnSeek.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconSeek.Show(FALSE);
                m_iconSeekFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_PHONE_CALL:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnCall.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconCall.Show(TRUE);
                m_iconCallFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnCall.SetSubCaption(str);
#else
        		m_btnCall.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconCall.Show(FALSE);
                m_iconCallFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_PHONE_HANGUP:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnHangup.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconHangup.Show(TRUE);
                m_iconHangupFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnHangup.SetSubCaption(str);
#else
        		m_btnHangup.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconHangup.Show(FALSE);
                m_iconHangupFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_MODE:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnMode.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconMode.Show(TRUE);
                m_iconModeFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnMode.SetSubCaption(str);
#else
        		m_btnMode.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconMode.Show(FALSE);
                m_iconModeFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_NAVI:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnNavi.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconNavi.Show(TRUE);
                m_iconNaviFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnNavi.SetSubCaption(str);
#else
        		m_btnNavi.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconNavi.Show(FALSE);
                m_iconNaviFocus.Show(TRUE);
            }
        }
        break;
    case KEY_STW_SPEECH:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnSpeech.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconSpeech.Show(TRUE);
                m_iconSpeechFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnSpeech.SetSubCaption(str);
#else
        		m_btnSpeech.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconSpeech.Show(FALSE);
                m_iconSpeechFocus.Show(TRUE);

            }
        }
        break;
    case KEY_STW_POWER:
        {
            if(iSet<0 || iSet>=0xFF)
            {
                m_btnPower.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
                m_iconPower.Show(TRUE);
                m_iconPowerFocus.Show(FALSE);
            }
            else
            {
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
        		swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
        		m_btnPower.SetSubCaption(str);
#else
        		m_btnPower.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
                m_iconPower.Show(FALSE);
                m_iconPowerFocus.Show(TRUE);

            }
        }
        break;
	case KEY_STW_HOME:
		{
			if(iSet<0 || iSet>=0xFF)
			{
				m_btnHome.SetSubCaption(GetResourceString(IDS_STW_UNDEFINE));
				m_iconHome.Show(TRUE);
				m_iconHomeFocus.Show(FALSE);
			}
			else
			{
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
				swprintf_s(str,16,_T("%.2fV"),iSet*3.3f/255);
				m_btnHome.SetSubCaption(str);
#else
				m_btnHome.SetSubCaption(GetResourceString(IDS_STW_ACTIVATED));
#endif
				m_iconHome.Show(FALSE);
				m_iconHomeFocus.Show(TRUE);
			}
		}
		break;
     default:
        break;
	}
}

void CPanelSettingAdvanced::OnCameraTypeChange(UINT uCameraType)
{
	RETAILMSG(DEBUG_SETTING, (TEXT("APP:MSG:GUI:Settings: [CPanelSettingAdvanced::OnCameraTypeChange] [%d].\r\n"),uCameraType));
	switch(uCameraType)
	{
	case 0:
	case 1:
		m_tagValueCameraType.SetCaption(_T("CVBS"));
		break;
//		case 2:
//			m_tagValueCameraType.SetCaption(_T("RGB"));
//			break;
	}
	m_pSkinManager->GetConfig()->GetCurParam()->dwCameraType = uCameraType;
	SetCamerType(uCameraType);
}

void CPanelSettingAdvanced::SetCamerType(UINT idType)
{
	RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingAdvanced::SetCamerType] ++\r\n")));
/*

	//
	HANDLE hDevTW8836 = CreateFile(TEXT("TWD1:"), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, 0, 0);

	if(hDevTW8836 != INVALID_HANDLE_VALUE)
	{
		TW8836INPUTCHANNEL input;
		switch(idType)
		{
		case 0:	// CVBS NTSC
			input=TW8836_INPUT_CHANNEL_CVBS_NTSC;
			break;
		case 1: // CVBS PAL
			input=TW8836_INPUT_CHANNEL_CVBS_PAL;
			break;
		case 2: // ARGB
			input=TW8836_INPUT_CHANNEL_ARGB;
			break;
		default:
			CloseHandle(hDevTW8836);
			return;
		}

		if (!DeviceIoControl(hDevTW8836, TW8836_IOCTL_CHANGE_VIDEOIN_CHANNEL, &input, sizeof(input),	NULL, 0, NULL, NULL))
		{
			RETAILMSG(DEBUG_SETTING, (TEXT("APP:ERROR:GUI:Settings: [CPanelSettingAdvanced::SetCamerType]: DeviceIoControl TW8836_IOCTL_CHANGE_VIDEOIN_CHANNEL failed. error=0x%x\r\n"), GetLastError()));
		}
		else
		{
		}

		CloseHandle(hDevTW8836);
	}

*/
	RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:GUI:Settings: [CPanelSettingAdvanced::SetCamerType] --\r\n")));
}

void CPanelSettingAdvanced::OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	//RETAILMSG(1,(_T("APP:MSG:CPanelSettingAdvanced::OnTimerProc: m_nSteerWheelCurrentStatus: %d, uMsg: %d,idEvent: %d , dwTime: %d++++\r\n"),g_pCPanelSettingAdvanced->m_nSteerWheelCurrentStatus,uMsg,idEvent,dwTime));
    switch(idEvent)
    {
    case TIMER_SETTING_ADVANCE_STEERWHEEL:
        if(g_pCPanelSettingAdvanced->m_bSteerWheelKeyCalibrating)
        {
            g_pCPanelSettingAdvanced->m_tagSteerWheelWindow.Show(FALSE);
            g_pCPanelSettingAdvanced->m_bSteerWheelKeyCalibrating = FALSE;
            KillTimer(g_pCPanelSettingAdvanced->m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL);
        }
        break;
    case TIMER_SETTING_ADVANCE_QUERY_STW:
        {
            if(!g_pCPanelSettingAdvanced->m_bMCUUpdating)
            {
                if(g_pCPanelSettingAdvanced->m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_IDLE ||
                    g_pCPanelSettingAdvanced->m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_QUERY)
                {
                    g_pCPanelSettingAdvanced->m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_QUERY;
                    //g_pCPanelSettingAdvanced->m_nCalibCurrentKey = g_pCPanelSettingAdvanced->m_nQueryKey;
                    //g_pCPanelSettingAdvanced->m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,g_pCPanelSettingAdvanced->m_nQueryKey);

                    g_pCPanelSettingAdvanced->m_nCalibCurrentKey = g_pCPanelSettingAdvanced->m_steerwheelkey[g_pCPanelSettingAdvanced->m_nQueryKey].keyname;
                    g_pCPanelSettingAdvanced->m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,g_pCPanelSettingAdvanced->m_nCalibCurrentKey);
                    //RETAILMSG(1, (TEXT("MSG:CPanelSettingAdvanced::OnTimerProc SendCmd[0x%x] m_nQueryKey[0x%x]\r\n"),g_pCPanelSettingAdvanced->m_nCalibCurrentKey,g_pCPanelSettingAdvanced->m_nQueryKey));


                    if(g_pCPanelSettingAdvanced->m_nQueryKey > KEY_STW_LASTKEY-KEY_STW_FIRSTKEY)
                    {
                        KillTimer(g_pCPanelSettingAdvanced->m_hMainWnd,TIMER_SETTING_ADVANCE_QUERY_STW);
                        g_pCPanelSettingAdvanced->m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_IDLE;
						g_pCPanelSettingAdvanced->CheckCalibrate();

                    }
                }
            }
        }
        break;
	//case TIMER_SETTING_ADVANCE_PASSWORD_CLEAR:
	//	{
	//		g_pCPanelSettingAdvanced->ClearInput(-1);
	//		KillTimer(g_pCPanelSettingAdvanced->m_hMainWnd,TIMER_SETTING_ADVANCE_PASSWORD_CLEAR);
	//	}
	//	break;
    }
}

void CPanelSettingAdvanced::QuerySteerWheelValue()
{
    //RETAILMSG(1, (TEXT("APP:MSG:CPanelSettingAdvanced::QuerySteerWheelValue\r\n")));
    SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_QUERY_STW,200,OnTimerProc);

//	    BYTE nQueryKey;
//	    if(m_pCommander)
//	    {
//	        nQueryKey = KEY_STW_VOL_PLUS
//	        m_nCalibCurrentKey = KEY_STW_VOL_PLUS;
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_VOL_PLUS);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_VOL_MINUS);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_VOL_MUTE);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_PREV);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_NEXT);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_SEEK);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_PHONE_CALL);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_MODE);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_POWER);
//	        m_pCommander->SendCmd(_ARM_STW_VALUE_KEY,KEY_STW_SPEECH);
//	    }
}

//	void CPanelSettingAdvanced::OnRearColorSignalChange(UINT uColorSignal)
//	{
//		m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard = uColorSignal;
//
//		switch (m_pSkinManager->GetConfig()->GetCurParam()->iRearStandard)
//		{
//		case OEMCONFIG_COLORSIGNAL_NTSM_MJ:
//			m_btnNTSC_MJ.SetStatus(BS_FOCUS);
//			m_btnNTSC_443.SetStatus(BS_UP);
//			m_btnPAL_M.SetStatus(BS_UP);
//			m_btnPAL_60.SetStatus(BS_UP);
//			m_btnPAL_BGHID.SetStatus(BS_UP);
//			m_btnPAL_COM_N.SetStatus(BS_UP);
//			m_btnSECAM.SetStatus(BS_UP);
//			m_btnSECAM_525.SetStatus(BS_UP);
//			break;
//	    case OEMCONFIG_COLORSIGNAL_NTSC_443:
//			m_btnNTSC_MJ.SetStatus(BS_UP);
//			m_btnNTSC_443.SetStatus(BS_FOCUS);
//			m_btnPAL_M.SetStatus(BS_UP);
//			m_btnPAL_60.SetStatus(BS_UP);
//			m_btnPAL_BGHID.SetStatus(BS_UP);
//			m_btnPAL_COM_N.SetStatus(BS_UP);
//			m_btnSECAM.SetStatus(BS_UP);
//			m_btnSECAM_525.SetStatus(BS_UP);
//			break;
//		case OEMCONFIG_COLORSIGNAL_PAL_M:
//			m_btnNTSC_MJ.SetStatus(BS_UP);
//			m_btnNTSC_443.SetStatus(BS_UP);
//			m_btnPAL_M.SetStatus(BS_FOCUS);
//			m_btnPAL_60.SetStatus(BS_UP);
//			m_btnPAL_BGHID.SetStatus(BS_UP);
//			m_btnPAL_COM_N.SetStatus(BS_UP);
//			m_btnSECAM.SetStatus(BS_UP);
//			m_btnSECAM_525.SetStatus(BS_UP);
//			break;
//	    case OEMCONFIG_COLORSIGNAL_PAL_60:
//			m_btnNTSC_MJ.SetStatus(BS_UP);
//			m_btnNTSC_443.SetStatus(BS_UP);
//			m_btnPAL_M.SetStatus(BS_UP);
//			m_btnPAL_60.SetStatus(BS_FOCUS);
//			m_btnPAL_BGHID.SetStatus(BS_UP);
//			m_btnPAL_COM_N.SetStatus(BS_UP);
//			m_btnSECAM.SetStatus(BS_UP);
//			m_btnSECAM_525.SetStatus(BS_UP);
//			break;
//	    case OEMCONFIG_COLORSIGNAL_PAL_BGHID:
//			m_btnNTSC_MJ.SetStatus(BS_UP);
//			m_btnNTSC_443.SetStatus(BS_UP);
//			m_btnPAL_M.SetStatus(BS_UP);
//			m_btnPAL_60.SetStatus(BS_UP);
//			m_btnPAL_BGHID.SetStatus(BS_FOCUS);
//			m_btnPAL_COM_N.SetStatus(BS_UP);
//			m_btnSECAM.SetStatus(BS_UP);
//			m_btnSECAM_525.SetStatus(BS_UP);
//			break;
//	    case OEMCONFIG_COLORSIGNAL_PAL_COMBINATION_N:
//			m_btnNTSC_MJ.SetStatus(BS_UP);
//			m_btnNTSC_443.SetStatus(BS_UP);
//			m_btnPAL_M.SetStatus(BS_UP);
//			m_btnPAL_60.SetStatus(BS_UP);
//			m_btnPAL_BGHID.SetStatus(BS_UP);
//			m_btnPAL_COM_N.SetStatus(BS_FOCUS);
//			m_btnSECAM.SetStatus(BS_UP);
//			m_btnSECAM_525.SetStatus(BS_UP);
//			break;
//		case OEMCONFIG_COLORSIGNAL_SECAM:
//			m_btnNTSC_MJ.SetStatus(BS_UP);
//			m_btnNTSC_443.SetStatus(BS_UP);
//			m_btnPAL_M.SetStatus(BS_UP);
//			m_btnPAL_60.SetStatus(BS_UP);
//			m_btnPAL_BGHID.SetStatus(BS_UP);
//			m_btnPAL_COM_N.SetStatus(BS_UP);
//			m_btnSECAM.SetStatus(BS_FOCUS);
//			m_btnSECAM_525.SetStatus(BS_UP);
//			break;
//		case OEMCONFIG_COLORSIGNAL_SECAM_525:
//			m_btnNTSC_MJ.SetStatus(BS_UP);
//			m_btnNTSC_443.SetStatus(BS_UP);
//			m_btnPAL_M.SetStatus(BS_UP);
//			m_btnPAL_60.SetStatus(BS_UP);
//			m_btnPAL_BGHID.SetStatus(BS_UP);
//			m_btnPAL_COM_N.SetStatus(BS_UP);
//			m_btnSECAM.SetStatus(BS_UP);
//			m_btnSECAM_525.SetStatus(BS_FOCUS);
//			break;
//	    default:
//			break;
//	    }
//	}

void CPanelSettingAdvanced::SteerWheelShow(BOOL bShow)
{
    m_btnVolPlus.Show(bShow);
    m_btnVolMinus.Show(bShow);
    m_btnVolMute.Show(bShow);
    m_btnPrev.Show(bShow);
    m_btnNext.Show(bShow);
    m_btnSeek.Show(bShow);
    m_btnCall.Show(bShow);
	m_btnHangup.Show(bShow);
	m_btnSpeech.Show(bShow);
	m_btnPower.Show(bShow);
    m_btnMode.Show(bShow);
	m_btnNavi.Show(bShow);
	m_btnSteerWheelConfirm.Show(bShow);
	m_btnSteerWheelCancel.Show(bShow);

	m_iconVolMinus.Show(bShow);
	m_iconVolMinusFocus.Show(bShow);
	m_iconVolPlus.Show(bShow);
	m_iconVolPlusFocus.Show(bShow);
	m_iconVolMute.Show(bShow);
	m_iconVolMuteFocus.Show(bShow);
	m_iconPrev.Show(bShow);
	m_iconPrevFocus.Show(bShow);
	m_iconNext.Show(bShow);
	m_iconNextFocus.Show(bShow);
	m_iconSeek.Show(bShow);
	m_iconSeekFocus.Show(bShow);
	m_iconCall.Show(bShow);
	m_iconCallFocus.Show(bShow);
	m_iconHangup.Show(bShow);
	m_iconHangupFocus.Show(bShow);
	m_iconSpeech.Show(bShow);
	m_iconSpeechFocus.Show(bShow);
	m_iconPower.Show(bShow);
	m_iconPowerFocus.Show(bShow);
	m_iconMode.Show(bShow);
	m_iconModeFocus.Show(bShow);
	m_iconNavi.Show(bShow);
	m_iconNaviFocus.Show(bShow);
}

void CPanelSettingAdvanced::Lock(BOOL bLock)
{
	m_bLock = bLock;
	if(bLock)
	{
		if(m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_START ||
			m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_CALIB_WAIT_FOR_NET_READY ||
			m_nSteerWheelCurrentStatus == STEER_WHEEL_STATUS_QUERY ||
			m_bSteerWheelKeyCalibrating)
		{
			ClearSTW();
		}

		m_btnDriveType.Enable(FALSE);
		m_sliBrakeTest.Enable(FALSE);
		m_sliLampControl.Enable(FALSE);
		m_btnLock.SetIcon(&m_iconLock,&m_iconUnLock);
#if !CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
		::PostMessage(m_hMainWnd,WM_PASSWORD_BOX_CMD,1,NULL);

		if(m_bSteerWheelBlockShow)
		{
			RETAILMSG(1, (TEXT("APP:MSG:IDC_SETTING_BTN_LOCK\r\n")));
			VIEW_STATE vs = m_btnStreerWheelSpread.GetCurViewState();
			vs.fRotateZ += (90);
			if(vs.fRotateZ < -360)
				vs.fRotateZ = 0;
			m_btnStreerWheelSpread.SetAimViewState(&vs);

			m_bSteerWheelBlockShow = FALSE;
			m_tagSteerWheelPanel.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h+6);
			m_tagSteerWheelPanel.SetPosIni(0,iSteerWheelPanelPy-3);
			SetSize(g_iClientWidth-box_size_x,g_iClientHeight);
			SetPosIni(LIST_BOX_ITEM_WIDTH/2, 0);
		}
#endif
		m_btnStreerWheelSpread.Enable(FALSE);
	}
	else
	{
		m_btnDriveType.Enable(TRUE);
		m_sliBrakeTest.Enable(TRUE);
		m_sliLampControl.Enable(TRUE);
		m_btnStreerWheelSpread.Enable(TRUE);
		m_btnLock.SetIcon(&m_iconUnLock,&m_iconLock);
#if !CVTE_EN_STEER_WHEEL_OLD_UI_STYLE
		::PostMessage(m_hMainWnd,WM_PASSWORD_BOX_CMD,2,NULL);
#endif
	}
}

void CPanelSettingAdvanced::CheckCalibrate()
{
	m_listSTW.Head();
	BYTE nCount = 0;
	m_bCalibrated = FALSE;
	while (!m_listSTW.End())
	{
		if(m_listSTW.Get()->valid)
		{
			m_bCalibrated = TRUE;
			break;
		}
		nCount++;
		m_listSTW.Next();
	}

	RETAILMSG(1, (TEXT("MSG:[%s]: invalid STW count :%d m_bCalibrated:%d\r\n"), TEXT(__FUNCTION__), nCount,m_bCalibrated));

}

void CPanelSettingAdvanced::StartSTW()
{
	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	m_btnStreerWheelSpread.Enable(FALSE);
	m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_CALIB_START;
	m_btnSteerWheelCancel.SetCaption(GetResourceString(IDS_CANCEL));
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
	m_tagTitleStreerWheel1.SetCaption(GetResourceString(IDS_SETTING_STEER_WHEEL));
#endif
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	if(m_pSkinManager->GetConfig()->GetCurParam()->bRadioRDS)
	{
		m_bRDSEnable = TRUE;
		SendRadioCommand(RADIO_CMD_RDS_SWITCH,0);   //Close RDS
	}
	else
	{
		m_bRDSEnable = FALSE;
	}
#endif
	ClearPara();

	m_iconVolMinus.Show(TRUE);
	m_iconVolMinusFocus.Show(FALSE);

	m_iconVolPlus.Show(TRUE);
	m_iconVolPlusFocus.Show(FALSE);

	m_iconVolMute.Show(TRUE);
	m_iconVolMuteFocus.Show(FALSE);

	m_iconPrev.Show(TRUE);
	m_iconPrevFocus.Show(FALSE);

	m_iconNext.Show(TRUE);
	m_iconNextFocus.Show(FALSE);

	m_iconSeek.Show(TRUE);
	m_iconSeekFocus.Show(FALSE);

	m_iconCall.Show(TRUE);
	m_iconCallFocus.Show(FALSE);

	m_iconHangup.Show(TRUE);
	m_iconHangupFocus.Show(FALSE);

	m_iconMode.Show(TRUE);
	m_iconModeFocus.Show(FALSE);

	m_iconNavi.Show(TRUE);
	m_iconNaviFocus.Show(FALSE);

	m_iconSpeech.Show(TRUE);
	m_iconSpeechFocus.Show(FALSE);

	m_iconPower.Show(TRUE);
	m_iconPowerFocus.Show(FALSE);

	UpdateSteerWheelPara(KEY_STW_VOL_MINUS,-1);
	UpdateSteerWheelPara(KEY_STW_VOL_PLUS,-1);
	UpdateSteerWheelPara(KEY_STW_VOL_MUTE,-1);
	UpdateSteerWheelPara(KEY_STW_PREV,-1);
	UpdateSteerWheelPara(KEY_STW_NEXT,-1);
	UpdateSteerWheelPara(KEY_STW_SEEK,-1);
	UpdateSteerWheelPara(KEY_STW_PHONE_CALL,-1);
	UpdateSteerWheelPara(KEY_STW_PHONE_HANGUP,-1);
	UpdateSteerWheelPara(KEY_STW_SPEECH,-1);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	UpdateSteerWheelPara(KEY_STW_HOME,-1);																				    
#else
	UpdateSteerWheelPara(KEY_STW_NAVI,-1);
#endif
	UpdateSteerWheelPara(KEY_STW_MODE,-1);
	UpdateSteerWheelPara(KEY_STW_POWER,-1);

	//m_iconSteerWheelFocus.Show(TRUE);
	m_pCommander->SendCmd(_ARM_STW_CMD_KEY,0);

	if(g_pCPanelSettingAdvanced->m_bSteerWheelKeyCalibrating)
	{
		g_pCPanelSettingAdvanced->m_tagSteerWheelWindow.Show(FALSE);
		g_pCPanelSettingAdvanced->m_bSteerWheelKeyCalibrating = FALSE;
		KillTimer(g_pCPanelSettingAdvanced->m_hMainWnd,TIMER_SETTING_ADVANCE_STEERWHEEL);
	}
}

void CPanelSettingAdvanced::ClearSTW()
{
	RETAILMSG(1, (TEXT("MSG:[%s]: \r\n"), TEXT(__FUNCTION__)));
	m_btnStreerWheelSpread.Enable(TRUE);
	m_bSteerWheelKeyCalibrating = FALSE;
	m_nSteerWheelCurrentStatus = STEER_WHEEL_STATUS_IDLE;
	m_btnSteerWheelCancel.SetCaption(GetResourceString(IDS_SETTING_STW_CALIBRATE));
#if CVTE_EN_SHOW_STW_VOLTAGE_VALUE
	m_tagTitleStreerWheel1.SetCaption(GetResourceString(IDS_SETTING_STEER_WHEEL));
#endif
	m_tagSteerWheelWindow.Show(FALSE);

	m_iconVolMinus.Show(TRUE);
	m_iconVolMinusFocus.Show(FALSE);

	m_iconVolPlus.Show(TRUE);
	m_iconVolPlusFocus.Show(FALSE);

	m_iconVolMute.Show(TRUE);
	m_iconVolMuteFocus.Show(FALSE);

	m_iconPrev.Show(TRUE);
	m_iconPrevFocus.Show(FALSE);

	m_iconNext.Show(TRUE);
	m_iconNextFocus.Show(FALSE);

	m_iconSeek.Show(TRUE);
	m_iconSeekFocus.Show(FALSE);

	m_iconCall.Show(TRUE);
	m_iconCallFocus.Show(FALSE);

	m_iconHangup.Show(TRUE);
	m_iconHangupFocus.Show(FALSE);

	m_iconMode.Show(TRUE);
	m_iconModeFocus.Show(FALSE);

	m_iconNavi.Show(TRUE);
	m_iconNaviFocus.Show(FALSE);

	m_iconSpeech.Show(TRUE);
	m_iconSpeechFocus.Show(FALSE);

	m_iconPower.Show(TRUE);
	m_iconPowerFocus.Show(FALSE);

	UpdateSteerWheelPara(KEY_STW_VOL_MINUS,-1);
	UpdateSteerWheelPara(KEY_STW_VOL_PLUS,-1);
	UpdateSteerWheelPara(KEY_STW_VOL_MUTE,-1);
	UpdateSteerWheelPara(KEY_STW_PREV,-1);
	UpdateSteerWheelPara(KEY_STW_NEXT,-1);
	UpdateSteerWheelPara(KEY_STW_SEEK,-1);
	UpdateSteerWheelPara(KEY_STW_PHONE_CALL,-1);
	UpdateSteerWheelPara(KEY_STW_PHONE_HANGUP,-1);
	UpdateSteerWheelPara(KEY_STW_SPEECH,-1);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	UpdateSteerWheelPara(KEY_STW_HOME,-1);																				    
#else
	UpdateSteerWheelPara(KEY_STW_NAVI,-1);		
#endif
	UpdateSteerWheelPara(KEY_STW_MODE,-1);
	UpdateSteerWheelPara(KEY_STW_POWER,-1);

	//quit MCU setting and re-open RDS
	m_pCommander->SendCmd(_ARM_STW_CMD_KEY,0); // Clear stw 
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	if( m_bRDSEnable )
	{
		SendRadioCommand(RADIO_CMD_RDS_SWITCH,1);
	}
#endif
}