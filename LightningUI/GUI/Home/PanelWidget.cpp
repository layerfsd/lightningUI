
#include "../Bluetooth FLC/BluetoothCommon.h"
#include "PanelWidget.h"
#include "../../resource.h"
#include "../Media/MediaCommon.h"
#include "../Radio/RadioCommon.h"
#include "../iPod/iPodCommon.h"
#include "../../Communication/CommanderEx.h"
#include "../../CarPlay/CarplayAdapter.h"
#include "../../Config/customer.h"


static HomeWidgetParam g_HomeWidgetParam[] =
{
	{IDC_WIDGET_MEDIA,	CVTE_DEF_MEDIA_WIDGET_CX,	CVTE_DEF_MEDIA_WIDGET_CY,	CVTE_DEF_MEDIA_WIDGET_PX,	CVTE_DEF_MEDIA_WIDGET_PY,	CVTE_DEF_MEDIA_WIDGET_VISIBLE},
#if CVTE_EN_RADIO
	{IDC_WIDGET_RADIO,	CVTE_DEF_RADIO_WIDGET_CX,	CVTE_DEF_RADIO_WIDGET_CY,	CVTE_DEF_RADIO_WIDGET_PX,	CVTE_DEF_RADIO_WIDGET_PY,	CVTE_DEF_RADIO_WIDGET_VISIBLE},
#endif

#if CVTE_EN_CARPLAY
	{IDC_WIDGET_CARPLAY,	CVTE_DEF_CARPLAY_WIDGET_CX,	CVTE_DEF_CARPLAY_WIDGET_CY,	CVTE_DEF_CARPLAY_WIDGET_PX,	CVTE_DEF_CARPLAY_WIDGET_PY,	CVTE_DEF_CARPLAY_WIDGET_VISIBLE},
#endif

#if CVTE_EN_IPOD
	{IDC_WIDGET_IPOD,	CVTE_DEF_IPOD_WIDGET_CX,	CVTE_DEF_IPOD_WIDGET_CY,	CVTE_DEF_IPOD_WIDGET_PX,	CVTE_DEF_IPOD_WIDGET_PY,	CVTE_DEF_IPOD_WIDGET_VISIBLE},
#endif

#if CVTE_EN_EASYCONNECT
	{IDC_WIDGET_EASYCONNECTED,	CVTE_DEF_ANDROID_LINK_WIDGET_CX,	CVTE_DEF_ANDROID_LINK_WIDGET_CY,	CVTE_DEF_ANDROID_LINK_WIDGET_PX,	CVTE_DEF_ANDROID_LINK_WIDGET_PY,	CVTE_DEF_ANDROID_LINK_WIDGET_VISIBLE},
#endif

#if CVTE_EN_WEBLINK
	{IDC_WIDGET_WEBLINK,	CVTE_DEF_WEBLINK_WIDGET_CX,	CVTE_DEF_WEBLINK_WIDGET_CY,	CVTE_DEF_WEBLINK_WIDGET_PX,	CVTE_DEF_WEBLINK_WIDGET_PY,	CVTE_DEF_WEBLINK_WIDGET_VISIBLE},
#endif

#if CVTE_EN_ANDROID_AUTO
	{IDC_WIDGET_ANDROID_AUTO,	CVTE_DEF_ANDROID_AUTO_WIDGET_CX,	CVTE_DEF_ANDROID_AUTO_WIDGET_CY,	CVTE_DEF_ANDROID_AUTO_WIDGET_PX,	CVTE_DEF_ANDROID_AUTO_WIDGET_PY,	CVTE_DEF_ANDROID_AUTO_WIDGET_VISIBLE},
#endif

#if CVTE_EN_BT_PHONE
	{IDC_WIDGET_PHONE,	CVTE_DEF_BT_PHONE_WIDGET_CX,	CVTE_DEF_BT_PHONE_WIDGET_CY,	CVTE_DEF_BT_PHONE_WIDGET_PX,	CVTE_DEF_BT_PHONE_WIDGET_PY,	CVTE_DEF_BT_PHONE_WIDGET_VISIBLE},
#endif

#if CVTE_EN_NAVI
	{IDC_WIDGET_NAVI,	CVTE_DEF_NAVI_WIDGET_CX,	CVTE_DEF_NAVI_WIDGET_CY,	CVTE_DEF_NAVI_WIDGET_PX,	CVTE_DEF_NAVI_WIDGET_PY,	CVTE_DEF_NAVI_WIDGET_VISIBLE},
#endif

#if CVTE_EN_BT_A2DP
	{IDC_WIDGET_A2DP,	CVTE_DEF_BT_A2DP_WIDGET_CX,	CVTE_DEF_BT_A2DP_WIDGET_CY,	CVTE_DEF_BT_A2DP_WIDGET_PX,	CVTE_DEF_BT_A2DP_WIDGET_PY,	CVTE_DEF_BT_A2DP_WIDGET_VISIBLE},
#endif

#if CVTE_EN_USB
	{IDC_WIDGET_USB,	CVTE_DEF_USB_WIDGET_CX,	CVTE_DEF_USB_WIDGET_CY,	CVTE_DEF_USB_WIDGET_PX,	CVTE_DEF_USB_WIDGET_PY,	CVTE_DEF_USB_WIDGET_VISIBLE},
#endif

#if CVTE_EN_DTV
	{IDC_WIDGET_TV,	CVTE_DEF_DTV_WIDGET_CX,	CVTE_DEF_DTV_WIDGET_CY,	CVTE_DEF_DTV_WIDGET_PX,	CVTE_DEF_DTV_WIDGET_PY,	CVTE_DEF_DTV_WIDGET_VISIBLE},
#endif

#if CVTE_EN_AUX
	{IDC_WIDGET_AUX,	CVTE_DEF_AUX_WIDGET_CX,	CVTE_DEF_AUX_WIDGET_CY,	CVTE_DEF_AUX_WIDGET_PX,	CVTE_DEF_AUX_WIDGET_PY,	CVTE_DEF_AUX_WIDGET_VISIBLE},
#endif

#if CVTE_EN_DVD
	{IDC_WIDGET_DISK,	CVTE_DEF_DVD_WIDGET_CX,	CVTE_DEF_DVD_WIDGET_CY,	CVTE_DEF_DVD_WIDGET_PX,	CVTE_DEF_DVD_WIDGET_PY,	CVTE_DEF_DVD_WIDGET_VISIBLE},
#endif

#if CVTE_EN_CAMERA
	{IDC_WIDGET_CAMERA,	CVTE_DEF_CAMERA_WIDGET_CX,	CVTE_DEF_CAMERA_WIDGET_CY,	CVTE_DEF_CAMERA_WIDGET_PX,	CVTE_DEF_CAMERA_WIDGET_PY,	CVTE_DEF_CAMERA_WIDGET_VISIBLE},
#endif

#if (CUSTOMER_ID == CUSTOMER_ALDINET)
#if CVTE_EN_DAB
	{IDC_WIDGET_DAB,	CVTE_DEF_DAB_WIDGET_CX,	CVTE_DEF_DAB_WIDGET_CY,	CVTE_DEF_DAB_WIDGET_PX,	CVTE_DEF_DAB_WIDGET_PY,	CVTE_DEF_DAB_WIDGET_VISIBLE},
#endif
#endif

#if CVTE_EN_DSP
	{IDC_WIDGET_DSP,	CVTE_DEF_DSP_WIDGET_CX,	CVTE_DEF_DSP_WIDGET_CY,	CVTE_DEF_DSP_WIDGET_PX,	CVTE_DEF_DSP_WIDGET_PY,	CVTE_DEF_DSP_WIDGET_VISIBLE},
#endif

	{IDC_WIDGET_SETTING,	CVTE_DEF_SETTING_WIDGET_CX,	CVTE_DEF_SETTING_WIDGET_CY,	CVTE_DEF_SETTING_WIDGET_PX,	CVTE_DEF_SETTING_WIDGET_PY,	CVTE_DEF_SETTING_WIDGET_VISIBLE},

#if CVTE_EN_ISDB
	{IDC_WIDGET_ISDB,	CVTE_DEF_ISDB_WIDGET_CX,	CVTE_DEF_ISDB_WIDGET_CY,	CVTE_DEF_ISDB_WIDGET_PX,	CVTE_DEF_ISDB_WIDGET_PY,	CVTE_DEF_ISDB_WIDGET_VISIBLE}
#endif
};


CPanelWidget::CPanelWidget(void)
{
	m_pCurWidget=NULL;
	m_bPhoneCallActive = FALSE;
	m_bHasCanbus = FALSE;
	m_bCarPlayConnected = FALSE;
	m_bAndroidAutoConnect = FALSE;
	m_biPodConnected = FALSE;
#if (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON == CVTE_ENABLE)
	m_bShowiPod = FALSE;
	m_idBtnStatusSwitch = BS_UP;
#endif	

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	m_gfPageX = 0.0f;
#endif
}

CPanelWidget::~CPanelWidget(void)
{

	//close widget:
	m_listWidgets.Head();
	while (!m_listWidgets.End())
	{
		delete m_listWidgets.Get();
		m_listWidgets.Next();
	}
}
void CALLBACK CPanelWidget::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{

	CPanelWidget *panel=(CPanelWidget*)lpUserPara;

	RETAILMSG(DEBUG_MAINMENU,(_T("CPanelWidget OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelWidget::Render()
{
	if(!BeginRender())return;

	CWidgetGL *pFocus = NULL;

	m_listWidgets.Head();
	while (!m_listWidgets.End())
	{
		if(pFocus == NULL && m_listWidgets.Get()->IsOnFocus())
		{
			pFocus = m_listWidgets.Get();
		}
		if( ( m_listWidgets.Get() != m_pCurWidget && (pFocus ? m_listWidgets.Get() != pFocus : TRUE) )  
			&& m_listWidgets.Get()->IsVisible() 
			&& m_listWidgets.Get()->IsOnScreen())
		{
			m_listWidgets.Get()->SetAlpha(m_fCurAlpha);
			m_listWidgets.Get()->Render();
		}

		m_listWidgets.Next();
	}

	if(m_pCurWidget && m_pCurWidget->IsVisible())
	{
		m_pCurWidget->SetAlpha(m_fCurAlpha);
		m_pCurWidget->Render();
	}

	if(pFocus && pFocus->IsVisible())
	{
		pFocus->SetAlpha(m_fCurAlpha);
		pFocus->Render();
	}

#if (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON == CVTE_ENABLE)
	m_btnSwitch.SetLuminance(m_fCurLuminance);
	m_btnSwitch.SetAlpha(m_fCurAlpha);
	m_btnSwitch.Render();
#endif

	EndRender();
}
void CPanelWidget::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

}
void CPanelWidget::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);
}

BOOL CPanelWidget::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

 	//large size:
 	SetSize(CVTE_DEF_HOME_WIDGET_WIDTH,g_iClientHeight);

 	//enable scrolling horizontal:
	if (CVTE_DEF_HOME_WIDGET_WIDTH <= g_iClientWidth)
		EnableScrollingX(FALSE);
	else
		EnableScrollingX(TRUE);
 	
	return InitializeWidget(pSkinManager,pTextGL,hWnd,entry,g_HomeWidgetParam, _countof(g_HomeWidgetParam));
}

void CPanelWidget::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(1,(_T(" FUNC [%s] idcontrol   : [%d] =========== \r\n"),TEXT(__FUNCTION__),idControl));
	
	switch (idControl)
	{
	case IDC_WIDGET_VEHICLEINFO:
		//if(m_bHasCanbus)
		{
			SwitchPage(UI_CLASS_VEHICLEINFO,PAGE_VEHICLE_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_MEDIA:
		if(!m_bPhoneCallActive)
		{
			UINT idMediaType=wPara;
			if(idMediaType==MEDIA_TYPE_USB1 || idMediaType==MEDIA_TYPE_USB2 ||
				idMediaType==MEDIA_TYPE_SD1
#ifdef USE_GPS_CARD_MEDIA_PLAY
				|| idMediaType==MEDIA_TYPE_SD2
#endif
				)//mass storage
			{
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
			else if(idMediaType == MEDIA_TYPE_A2DP)
			{
				SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_ZOOMOUT,-1,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
			else if(idMediaType == MEDIA_TYPE_IPOD)
			{
				SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
			else
			{
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
		}
		break;
	case IDC_WIDGET_RADIO:
		if(!m_bPhoneCallActive)
		{
			CWidgetGL *p=GetWidget(IDC_WIDGET_RADIO);
			if(p)
			{
#if CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
					SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
#else
				if(((CWidgetRadioCore*)p)->GetType() == UI_CLASS_DAB)
					SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
				else
					SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
#endif

			}
		}
		break;
	case IDC_WIDGET_MHL:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_MHL,PAGE_MHL_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_TV:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_TV,PAGE_TV_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_DISK:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_DISK,PAGE_DISK_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_AUX:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_AUX,PAGE_AUX_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_NAVI:
		{
#if 0//(CUSTOMER_ID == CUSTOMER_CVTE_CERTIFICATION)
			SwitchPage(UI_CLASS_NAVI_SIMULATE,0,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
#else
			SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
#endif
		}
		break;
	case IDC_WIDGET_PHONE:
		{
		    //Do not allow user to enter PhoneCall panel once Carplay connected.
			if(m_bCarPlayConnected == TRUE)
			{
				break;
			}

			if(m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn)
			{
				if(m_bPhoneCallActive)
					SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
				else
					SwitchPage(UI_CLASS_BT,PAGE_BT_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
			else
			{
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,PAGE_SETTINGS_BLUETOOTH,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
		}
		break;
	case IDC_WIDGET_A2DP:
		if(!m_bPhoneCallActive && !m_bAndroidAutoConnect && !m_bCarPlayConnected)
		{
			if(m_pSkinManager->GetConfig()->GetCurParam()->bBluetoothPowerOn /*&& (m_bCarPlayConnected == FALSE)*/)
			{
				if(wPara)//connected:
				{
					SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
					//SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,MEDIA_TYPE_A2DP);
				}
				else
				{
					SwitchPage(UI_CLASS_BT,PAGE_BT_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,(BT_PICK_DEVICE_FOR_A2DP<<16)+BT_HOMEPAGE_DEVICE,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
				}
			}
			else
			{
				SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,PAGE_SETTINGS_BLUETOOTH,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
		}
		break;
	case IDC_WIDGET_PHONELINK:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);

			/*
			{
				static DWORD dwtick=0;

				if(GetTickCount()-dwtick>2000)
				{
					PROCESS_INFORMATION pi;
					if(CreateProcess(_T("\\GPS CARD\\vnc viewer\\automotiveViewer.exe"),_T("-pf RGB565 -enc TRLE -perfmetric FramesRendered -disc"),NULL,NULL,0,0,NULL,NULL,NULL,&pi))//set correct location:
					{
						RETAILMSG(DEBUG_MAINMENU,(_T("start vnc succeed.\r\n")));
					}
					else
					{
						RETAILMSG(DEBUG_MAINMENU,(_T("start vnc failed.\r\n")));
					}

					dwtick = GetTickCount();
				}

			}*/
		}
		break;
	case IDC_WIDGET_EASYCONNECTED:
		{
			if(!m_bPhoneCallActive)
			{
				SwitchPage(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
		}
		break;
	case IDC_WIDGET_WEBLINK:
		{
			if(!m_bPhoneCallActive)
			{
				SwitchPage(UI_CLASS_WEBLINK,PAGE_WEBLINK_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
		}
		break;
	case IDC_WIDGET_ANDROID_AUTO:
		{
			if(m_bAndroidAutoConnect)
			{
				SwitchPage(UI_CLASS_ANDROID_AUTO,PAGE_ANDROID_AUTO_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
		}
		break;
	case IDC_WIDGET_SETTING:
		{
			//SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER);
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_SD:
		if(!m_bPhoneCallActive)
		{
			UINT media_type=MEDIA_TYPE_INVALID;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);
			if (media_type == MEDIA_TYPE_SD2)
			{
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,MEDIA_TYPE_SD2,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
			else
			{
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,MEDIA_TYPE_SD1,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
		}
		break;
	case IDC_WIDGET_USB:
		if(!m_bPhoneCallActive)
		{
			UINT media_type=MEDIA_TYPE_INVALID;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_TAG_MEDIATYPE,UIOPRT_GETVALUE,&media_type);
			if (media_type == MEDIA_TYPE_USB2)
			{
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,MEDIA_TYPE_USB2,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
			else
			{
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,MEDIA_TYPE_USB1,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
			}
		}
		break;
	case IDC_WIDGET_IPOD:
		if(!m_bPhoneCallActive)
		{
		//SwitchPage(UI_CLASS_IMAGE,PAGE_IMAGE_LIBRARY);
		//SwitchPage(UI_CLASS_SR,PAGE_SR_MAIN,PAGE_SWITCH_ANI_NONE);
		//SwitchPage(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN,PAGE_SWITCH_ANI_NONE);
		//SwitchPage(UI_CLASS_BT,PAGE_BT_TALKING,PAGE_SWITCH_ANI_NONE);
// 		ShowExplorer(IDC_PANEL_EXPLORER,&CPanelWidget::OnNotify,this,
// 			GetResourceString(IDS_TITLE_NAVI),
// 			_T("\\"),_T("EXE"));

			SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,MEDIA_TYPE_IPOD,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_CARPLAY:
		//if(!m_bPhoneCallActive)
		if(m_bCarPlayConnected)
		{
			//20160428 new carplay lparam must be 0
			SwitchPage(UI_CLASS_CARPLAY,PAGE_CARPLAY_WRAPPER,PAGE_SWITCH_ANI_ZOOMOUT,/*(1<<16)+*/0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_CAMERA:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_REARVIEW,PAGE_REAVIEW_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_DAB:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,0,(LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	case IDC_WIDGET_DSP:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_DEFAULT,PAGE_SETTINGS_AUDIO);
		}
		break;

	case IDC_WIDGET_ISDB:
		if(!m_bPhoneCallActive)
		{
			SwitchPage(UI_CLASS_ISDB, PAGE_ISDB_WRAPPER, PAGE_SWITCH_ANI_ZOOMOUT, 0, (LPVOID)m_listWidgets.GetItemByID(idControl),TRUE);
		}
		break;
	default:
		break;
	}

}

BOOL CPanelWidget::CreateWidget(UINT id,int cx,int cy,GLfloat px,GLfloat py,BOOL bCreateVisible)
{
	CWidgetGL *pWidget=NULL;

	switch (id)
	{
	case IDC_WIDGET_MEDIA:
		pWidget = new CWidgetMediaCore;
		break;
	case IDC_WIDGET_RADIO:
		pWidget = new CWidgetRadioCore;
		break;
	case IDC_WIDGET_DISK:
		pWidget = new CWidgetDiskCore;
		break;
	case IDC_WIDGET_IPOD:
		pWidget = new CWidgetiPod;
		break;
	case IDC_WIDGET_CARPLAY:
		pWidget = new CWidgetCarPlay;
		break;
	case IDC_WIDGET_NAVI:
		pWidget = new CWidgetNaviCore;
		break;
	case IDC_WIDGET_PHONELINK:
		pWidget = new CWidgetPhoneLink;
		break;
	case IDC_WIDGET_EASYCONNECTED:
		pWidget = new CWidgetEasyConnected;
		break;
	case IDC_WIDGET_WEBLINK:
#if CVTE_EN_COMBINE_ANDROID_LINK
		if (m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType == DEVICE_TYPE_WEBLINK)
			pWidget = new CWidgetWeblink;
#else
		pWidget = new CWidgetWeblink;
#endif	
		break;
	case IDC_WIDGET_PHONE:
		pWidget = new CWidgetPhoneCore;
		break;
	case IDC_WIDGET_SETTING:
		pWidget = new CWidgetSettingCore;
		break;
	case IDC_WIDGET_SD:
		pWidget = new CWidgetSD;
		break;
	case IDC_WIDGET_USB:
		pWidget = new CWidgetUSB;
		break;
	case IDC_WIDGET_MHL:
		pWidget = new CWidgetMHL;
		break;
	case IDC_WIDGET_TV:
		pWidget = new CWidgetTVCore;
		break;
	case IDC_WIDGET_A2DP:
		pWidget = new CWidgetA2DP;
		break;
	case IDC_WIDGET_VEHICLEINFO:
		pWidget = new CWidgetVehicleInfo;
		break;
	case IDC_WIDGET_AUX:
		pWidget = new CWidgetAUXCore;
		break;
	case IDC_WIDGET_CAMERA:
		pWidget = new CWidgetCamera;
		break;
	case IDC_WIDGET_DAB:
		pWidget = new CWidgetDAB;
		break;
	case IDC_WIDGET_DSP:
		pWidget = new CWidgetDSP;
		break;
	case IDC_WIDGET_ANDROID_AUTO:
#if CVTE_EN_COMBINE_ANDROID_LINK
		if (m_pSkinManager->GetConfig()->GetCurParam()->dwAndroidLinkType == DEVICE_TYPE_ANDROID_AUTO)
			pWidget = new CWidgetAndroidAuto;
#else
		pWidget = new CWidgetAndroidAuto;
#endif	
		break;

	case IDC_WIDGET_ISDB:
		pWidget = new CWidgetISDBCore;
		break;
		
	default:
		break;
	}

	if(pWidget)
	{
		pWidget->Initialize(id,cx,cy,this,m_pSkinManager,m_pTextGL,m_hMainWnd,m_moduleEntry);
		pWidget->SetNotifyFunc(CPanelWidget::OnNotify,this);
		pWidget->SetPos(px,py);
		pWidget->Show(bCreateVisible);
		pWidget->Enable(bCreateVisible);
		m_listWidgets.Add(pWidget,id);
	}

	return pWidget!=NULL;
}
BOOL CPanelWidget::InitializeWidget(CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry,HomeWidgetParam *pParam, size_t count)
{

	if(pParam == NULL || count == 0)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: Param check failed.\r\n"), TEXT(__FUNCTION__)));
		return FALSE;
	}

	for(size_t i=0; i < count; i++)
	{
		RETAILMSG(DEBUG_MAINMENU, (TEXT("MSG:[%s]: CreateWidget[%d] CX[%d] CY[%d] PX[%d] PY[%d] Visible[%d]\r\n"), TEXT(__FUNCTION__),
			pParam[i].nWidgetID,pParam[i].nWidth, pParam[i].nHeight, pParam[i].nPX, pParam[i].nPY, pParam[i].bVisible));

		CreateWidget((UINT)pParam[i].nWidgetID, (int)pParam[i].nWidth, (int)pParam[i].nHeight, (GLfloat)pParam[i].nPX, (GLfloat)pParam[i].nPY, (BOOL)pParam[i].bVisible);

	}


#if (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON == CVTE_ENABLE)
	m_iconSwitch.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_294));
	m_iconSwitchFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_293));

	m_btnSwitch.Initialize(IDC_IPOD_CARPLAY_BTN_SWITCH,this,NULL,NULL,
		NULL,NULL,NULL,NULL,NULL,NULL);
	//m_btnSwitch.SetNotifyFunc(CPanelWidget::OnNotify,this);
	//m_btnSwitch.SetPos(CVTE_DEF_CARPLAY_WIDGET_PX+90,CVTE_DEF_CARPLAY_WIDGET_PY+100);
#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	{
		m_btnSwitch.SetPos(CVTE_DEF_CARPLAY_WIDGET_PX-CVTE_DEF_CARPLAY_WIDGET_CX/2+36,CVTE_DEF_CARPLAY_WIDGET_PY+CVTE_DEF_CARPLAY_WIDGET_CY/2-36);
	}
#else
	{
		m_btnSwitch.SetPos(CVTE_DEF_CARPLAY_WIDGET_PX-CVTE_DEF_CARPLAY_WIDGET_CX/2+36,CVTE_DEF_CARPLAY_WIDGET_PY+CVTE_DEF_CARPLAY_WIDGET_CY/2-36);
	}
#endif
	//m_btnSwitch.SetShowScale(1.1f);
	m_btnSwitch.EnableDrawBack(FALSE);
	m_btnSwitch.SetIcon(&m_iconSwitch, &m_iconSwitchFocus);
	m_btnSwitch.SetTouchRate(1.5f,1.5f);
	m_btnSwitch.Show(m_bCarPlayConnected && m_biPodConnected);
#endif

	SIZE si={0,0};
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	si.cx = CVTE_DEF_SCREEN_VIDEOWIDTH * 2;
	si.cy = CVTE_DEF_SCREEN_VIDEOHEIGHT;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	si.cx = CVTE_DEF_HOME_WIDGET_WIDTH;
	si.cy = CVTE_DEF_SCREEN_VIDEOHEIGHT;
#else
	CalculatePageSize(&si);
	RETAILMSG(1,(_T("## CalculatePageSize, cx=%d, cy=%d\r\n"),si.cx,si.cy));
	if(si.cx>g_iClientWidth)
		si.cx+=10;//margin_x;
	if(si.cy>g_iClientHeight)
		si.cy+=10;//margin_y;

	if(si.cx<g_iClientWidth)
		si.cx=g_iClientWidth;
	if(si.cy<g_iClientHeight)
		si.cy=g_iClientHeight;
#endif

	SetSize(si.cx,si.cy);

	return TRUE;
}
BOOL CPanelWidget::CalculatePageSize(SIZE *pSize)
{
	if(pSize)
	{
		int mx=0;
		int my=0;

		m_listWidgets.Head();
		while (!m_listWidgets.End())
		{
			CWidgetGL* p=m_listWidgets.Get();
			int cx=p->GetPosxIni()+p->GetSize().cx/2+g_iClientWidth/2;
			int cy=p->GetPosyIni()+p->GetSize().cy/2+g_iClientHeight/2;

			if(mx<cx)
				mx=cx;
			if(my<cy)
				my=cy;

			m_listWidgets.Next();
		}

		pSize->cx = mx;
		pSize->cy = my;

		return TRUE;
	}

	return FALSE;
}

void CPanelWidget::OnCanbusStatus(WPARAM wParam, LPARAM lParam)
{
	m_bHasCanbus = wParam;
}
CWidgetGL*	CPanelWidget::GetWidget(UINT idWidget)
{
	return m_listWidgets.GetItemByID(idWidget);
}
void CPanelWidget::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_MAINMENU,(_T("------CPanelWidget OnAvailableSourceChanged:%d,%d\r\n"),wParam,(BOOL)lParam));

#if (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON == CVTE_ENABLE && CVTE_EN_CARPLAY && CVTE_EN_IPOD)
	if(wParam == MEDIA_TYPE_IPOD)
	{
		m_biPodConnected = (BOOL)lParam;

		CWidgetGL *pCarplay=GetWidget(IDC_WIDGET_CARPLAY);
		if(pCarplay)
		{
			pCarplay->Show(!m_biPodConnected);
			pCarplay->Enable(!m_biPodConnected);
		}

		CWidgetGL *piPod=GetWidget(IDC_WIDGET_IPOD);
		if(piPod)
		{
			piPod->Show(m_biPodConnected);
			piPod->Enable(m_biPodConnected);
		}

		m_bShowiPod = m_biPodConnected;
	}
	else if(wParam ==MEDIA_TYPE_CARPLAY)
	{
		m_bCarPlayConnected = (BOOL)lParam;
	
		BOOL bShowCarplay = (m_bCarPlayConnected || !m_biPodConnected)?TRUE:FALSE;
		CWidgetGL *pCarplay=GetWidget(IDC_WIDGET_CARPLAY);
		if(pCarplay)
		{
			pCarplay->Show(bShowCarplay);
			pCarplay->Enable(bShowCarplay);
		}

		CWidgetGL *piPod=GetWidget(IDC_WIDGET_IPOD);
		if(piPod)
		{
			piPod->Show(!bShowCarplay);
			piPod->Enable(!bShowCarplay);
		}

		m_bShowiPod = !bShowCarplay;
	}
	m_btnSwitch.Show(m_bCarPlayConnected && m_biPodConnected);
#else
	//RETAILMSG(1, (TEXT("MSG:Do not hide iPod widget.\r\n")));
	if(wParam == MEDIA_TYPE_IPOD)
	{
		m_biPodConnected = (BOOL)lParam;
	}
	else if(wParam ==MEDIA_TYPE_CARPLAY)
	{
		m_bCarPlayConnected = (BOOL)lParam;
	}
#endif

#if CVTE_EN_ANDROID_AUTO
	if(wParam == MEDIA_TYPE_ANDROID_AUTO)
	{
		m_bAndroidAutoConnect = (BOOL)lParam;
		BOOL bshow=m_bAndroidAutoConnect;
		OperateWidget(IDC_WIDGET_ANDROID_AUTO,0,UIOPRT_SHOWHIDE,&bshow);
	}
#endif
}
void CPanelWidget::OnWidgetChanged(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(DEBUG_MAINMENU,(_T("------CPanelWidget OnWidgetChanged:%d,%d\r\n"),wParam,lParam));


}

#if CVTE_EN_KEY_PROXY
BOOL CPanelWidget::OnKey(UINT idKey,UINT idStatus)
{
	return TRUE;
}
#else
BOOL CPanelWidget::OnKey(UINT idKey)
{
	switch (idKey)
	{
	case KEY_MCU_SEEK_NEXT:

		break;
	case KEY_MCU_SEEK_PREV:

		break;
	}

	return TRUE;
}
#endif

void CPanelWidget::OnTouchDown(POINT *pt)
{
#if (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON == CVTE_ENABLE)
	if(m_btnSwitch.IsPtnOnObject(pt))
	{
		//m_dwBtnSwitchLastPressTick = GetTickCount();
		m_idBtnStatusSwitch = BS_DOWN;
		//RETAILMSG(1, (TEXT("MSG:[%s]: m_btnSwitch DOWN!!!\r\n"), TEXT(__FUNCTION__)));
		m_btnSwitch.SetIcon(&m_iconSwitchFocus, &m_iconSwitch);
		return;
	}
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	m_gfPageX = GetPosX();
#endif
	m_ptLastPress = *pt;

	CPanelGL::OnTouchDown(pt);

	m_pCurWidget = NULL;
	m_listWidgets.Head();
	while (!m_listWidgets.End())
	{
		if(m_listWidgets.Get()->IsPtnOnObject(pt))
		{
			m_pCurWidget = m_listWidgets.Get();
			break;
		}
		m_listWidgets.Next();
	}


}
void CPanelWidget::OperateWidget(UINT idUI,UINT idControl,UINT idOperation,LPVOID lpara)
{
// 	RETAILMSG(DEBUG_MAINMENU,(_T("###UpdateWidget idUI[%d], idControl[%d], idOperation[%d]...\r\n"),idUI,idControl,idOperation));

	CWidgetGL *p=m_listWidgets.GetItemByID(idUI);
	if(p)
	{
		p->OperateControl(idControl,idOperation,lpara);
	}
}

// void CPanelWidget::SetCurrentWidgetID(UINT idClass)
// {
// 	CWidgetGL *p=m_listWidgets.GetItemByID(idClass);
// 	if(p)
// 	{
// 		m_pCurWidget = p;
// 	}
// }

void CPanelWidget::OnTouchUp(POINT *pt)
{
#if (CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON == CVTE_ENABLE)
	if(m_idBtnStatusSwitch == BS_DOWN)
	{
		m_btnSwitch.SetIcon(&m_iconSwitch, &m_iconSwitchFocus);
		if(m_btnSwitch.IsPtnOnObject(pt))
		{
			m_idBtnStatusSwitch = BS_UP;
			//RETAILMSG(1, (TEXT("MSG:[%s]: m_btnSwitch UP!!! m_bShowiPod[%d]\r\n"), TEXT(__FUNCTION__),m_bShowiPod));

			if(m_bShowiPod)
				m_bShowiPod = FALSE;
			else
				m_bShowiPod = TRUE;

			//RETAILMSG(1, (TEXT("MSG:[%s]: m_bShowiPod:%d\r\n"),TEXT(__FUNCTION__),m_bShowiPod));
			CWidgetGL *pCarplay=GetWidget(IDC_WIDGET_CARPLAY);
			if(pCarplay)
			{
				pCarplay->Show(!m_bShowiPod);
				pCarplay->Enable(!m_bShowiPod);
			}

			CWidgetGL *piPod=GetWidget(IDC_WIDGET_IPOD);
			if(piPod)
			{
				piPod->Show(m_bShowiPod);
				piPod->Enable(m_bShowiPod);
			}

			PlaySoundEffect(0);	

			return;
		}
	}
#endif

	CPanelGL::OnTouchUp(pt);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	//printf("MSG:[%s]: GetPosX[%f] GetPosxIni[%f]\r\n", TEXT(__FUNCTION__), GetPosX(), GetPosxIni());
	GLfloat glfCurPageX = GetPosX();
	BOOL bRightMove = (glfCurPageX>m_gfPageX)?TRUE:FALSE;
	if(bRightMove)
	{
		if(glfCurPageX>-600 || (GetTickCount()-m_dwLastPressTick<500 && abs(pt->x-m_ptLastPress.x)>30))
			SetPage(1);
		else
			SetPage(2);
	}
	else
	{
		if(glfCurPageX<-200 || (GetTickCount()-m_dwLastPressTick<500 && abs(pt->x-m_ptLastPress.x)>30))
			SetPage(2);
		else
			SetPage(1);
	}
#endif	
}
void CPanelWidget::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);
}
void CPanelWidget::OnGestureMsg(UINT idGesture)
{
	RETAILMSG(DEBUG_MAINMENU,(_T("###CPanelWidget::OnGestureMsg idGesture: %d\r\n"),idGesture));

	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
// 		SwitchPage(UI_CLASS_NAVI,PAGE_NAVI_MAIN,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
// 		SwitchPage(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC),
// 			GET_PANEL_PAGE_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC),
// 			PAGE_SWITCH_ANI_LEFT);
		break;
	}
}
void CPanelWidget::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	m_bPhoneCallActive = wParam;

	CWidgetGL *pMediaCore=GetWidget(IDC_WIDGET_MEDIA);
	if(pMediaCore)
	{
		BOOL bEnable = !m_bPhoneCallActive;
		OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PLAY,UIOPRT_ENABLE,&bEnable);
		OperateWidget(IDC_WIDGET_MEDIA,IDC_MEDIA_BTN_PAUSE,UIOPRT_ENABLE,&bEnable);
	}
}
void CPanelWidget::OnGesture(GESTUREINFO *pgi)
{
	if(!pgi)
		return;

	POINT pt={pgi->ptsLocation.x,pgi->ptsLocation.y};

	CPanelGL::OnGesture(pgi);

	// Handle gesture indicated by wParam or gi.dwID

// 	if(pgi->dwID != GID_END && pgi->dwID != GID_BEGIN)
// 	{
// 		m_dwLastGestureTick = GetTickCount();
// 	}

	switch (pgi->dwID)
	{
	case GID_BEGIN:
		break;
	case GID_END:
		break;
	case GID_PAN:
		break;
	case GID_ROTATE:
		break;
	case GID_SCROLL:
		{
			LONG velocity = GID_SCROLL_VELOCITY (pgi->ullArguments );
			LONG angle =   GID_SCROLL_ANGLE (pgi->ullArguments );
			LONG direction = GID_SCROLL_DIRECTION (pgi->ullArguments );

			RETAILMSG(DEBUG_MAINMENU, (TEXT("++++GID_SCROLL:velocity %d, angle:%d, direction:%d, \r\n"),velocity, angle, direction));

			switch (direction )
			{
			case ARG_SCROLL_DOWN:
				break;
			case ARG_SCROLL_UP:
				break;
			case ARG_SCROLL_LEFT:
				break;
			case ARG_SCROLL_RIGHT:
				break;
			}
		}
		break;

	case GID_HOLD:
		break;
	case GID_SELECT:
		break;
	case GID_DOUBLESELECT:
		break;
	}

}

BOOL CPanelWidget::IsReady()
{
// 	BOOL btemp=TRUE;
//
// 	m_listWidgets.Head();
// 	while (!m_listWidgets.End())
// 	{
// 		btemp = m_listWidgets.Get()->IsReady();
// 		if(!btemp)
// 			break;
// 		m_listWidgets.Next();
// 	}
	return CPanelGL::IsReady();
}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
void CPanelWidget::SetPage(UINT nPageNum)
{
	switch(nPageNum)
	{
	case 1:
		//SetPos(0, 0, 0,TRUE);
		SlideToPosX(0);
		if(m_pChangeHomePageCallback)
		{
			m_pChangeHomePageCallback(1,m_lparam);
		}
		break;
	case 2:
		//SetPos(-CVTE_DEF_SCREEN_VIDEOWIDTH, 0, 0, TRUE);
		SlideToPosX(-CVTE_DEF_SCREEN_VIDEOWIDTH);
		if(m_pChangeHomePageCallback)
		{
			m_pChangeHomePageCallback(2,m_lparam);
		}
		break;
		//case 3:
		//	SetPosIni(-(CVTE_DEF_SCREEN_VIDEOWIDTH*2), 0);
		//	break;
	default:
		break;
	}
	PlaySoundEffect(0);	
}
#endif

//music widget:
CWidgetMediaCore::CWidgetMediaCore(void)
{
	m_bPressed=FALSE;
	m_bVideoMode = FALSE;
	m_idCurMediaType=MEDIA_TYPE_INVALID;
	m_bHasAlbumArt = FALSE;
}

CWidgetMediaCore::~CWidgetMediaCore(void)
{

}

BOOL CWidgetMediaCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagAlbumArt.IsReady() ;
}
void CWidgetMediaCore::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_170):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_176):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	if(m_idCurMediaType == MEDIA_TYPE_IPOD)
	{
		m_tagAlbumArtForIpod.SetLuminance(m_bHasAlbumArt?m_fCurLuminance*0.5f:m_fCurLuminance);
		m_tagAlbumArtForIpod.SetAlpha(m_fCurAlpha);
		m_tagAlbumArtForIpod.Render();
	}
	else
	{
		BOOL bShow = ( !m_bVideoMode || m_idCurMediaType==MEDIA_TYPE_A2DP);
		m_tagAlbumArt.Show(bShow);
		m_tagVideo.Show(!bShow);

		m_tagAlbumArt.SetLuminance(m_bHasAlbumArt?m_fCurLuminance*0.5f:m_fCurLuminance);
		m_tagAlbumArt.SetAlpha(m_fCurAlpha);
		m_tagAlbumArt.Render();

		m_tagVideo.SetLuminance(m_fCurLuminance);
		m_tagVideo.SetAlpha(m_fCurAlpha);
		m_tagVideo.Render();
	}

	DrawRect(pTexBar,0,-m_siCurSize.cy/2+30,m_siCurSize.cx-14-8,46-8,8,&cr,TRUE);
	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_btnPlay.SetAlpha(m_fCurAlpha);
	m_btnPlay.Render();

	m_btnPause.SetAlpha(m_fCurAlpha);
	m_btnPause.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	//COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};
	//COLORGL cr_t={1.0,1.0,1.0,0.0};
	m_btnMedia.SetStatus(m_bPressed?BS_DOWN:BS_UP);

	if(m_idCurMediaType == MEDIA_TYPE_IPOD)
	{
		m_tagAlbumArtForIpod.SetLuminance(m_fCurLuminance);
		m_tagAlbumArtForIpod.SetAlpha(m_fCurAlpha*0.9f);
		m_tagAlbumArtForIpod.Render();
	}
	else
	{
		BOOL bShow = ( !m_bVideoMode || m_idCurMediaType==MEDIA_TYPE_A2DP);
		m_tagAlbumArt.Show(bShow);
		m_tagVideo.Show(!bShow);

		m_tagAlbumArt.SetLuminance(m_fCurLuminance);
		m_tagAlbumArt.SetAlpha(m_fCurAlpha*0.9f);
		m_tagAlbumArt.Render();

		m_tagVideo.SetLuminance(m_fCurLuminance);
		m_tagVideo.SetAlpha(m_fCurAlpha*0.9f);
		m_tagVideo.Render();
	}

	//m_btnMedia.SetAlpha(m_fCurAlpha);
	m_btnMedia.Render();

	m_tagMediaType.SetAlpha(m_fCurAlpha*0.6f);
	m_tagMediaType.Render();

	m_tagTrack.SetAlpha(m_fCurAlpha);
	m_tagTrack.Render();

	m_tagArtist.SetAlpha(m_fCurAlpha);
	m_tagArtist.Render();

	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_btnPlay.SetAlpha(m_fCurAlpha);
	m_btnPlay.Render();

	m_btnPause.SetAlpha(m_fCurAlpha);
	m_btnPause.Render();
#else
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_131):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	COLORGL cr_t={1.0,1.0,1.0,0.0};
	// 	COLORGL cr_down={0.0,1.0,0.0,m_fCurAlpha};
// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,&cr_t);

	if(m_idCurMediaType == MEDIA_TYPE_IPOD)
	{
		m_tagAlbumArtForIpod.SetLuminance(m_fCurLuminance);
		m_tagAlbumArtForIpod.SetAlpha(m_fCurAlpha*0.9f);
		m_tagAlbumArtForIpod.Render();
	}
	else
	{
		BOOL bShow = ( !m_bVideoMode || m_idCurMediaType==MEDIA_TYPE_A2DP);
		m_tagAlbumArt.Show(bShow);
		m_tagVideo.Show(!bShow);

		m_tagAlbumArt.SetLuminance(m_fCurLuminance);
		m_tagAlbumArt.SetAlpha(m_fCurAlpha*0.9f);
		m_tagAlbumArt.Render();

		m_tagVideo.SetLuminance(m_fCurLuminance);
		m_tagVideo.SetAlpha(m_fCurAlpha*0.9f);
		m_tagVideo.Render();
	}

// 	DrawRect(pTexWidget,0,0,300-32,300-32,16,&cr,TRUE);
// 	DrawRect(pTexUp,0,(-m_siCurSize.cy/4),m_siCurSize.cx-16-6,(m_siCurSize.cy/2)-16-6,8,&cr,TRUE);
	DrawRect(pTexBar,0,(-m_siCurSize.cy/2+51),(m_siCurSize.cx-10)-16,94-16,8,&cr,TRUE);


// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);


// 	DrawRect(pTexFrame,m_tagAlbumArt.GetPosX(),m_tagAlbumArt.GetPosY(),176-4,176-4,12,&cr);

// 	COLORGL crw={0.6,0.6,0.6,1.0};
// 	DrawRect(1,m_tagAlbumArt.GetPosX(),m_tagAlbumArt.GetPosY(),180-4,180-4,12,&crw,FALSE,GL_ADD);



// 	m_sliProgress.SetLuminance(m_fCurLuminance);
// 	m_sliProgress.SetAlpha(m_fCurAlpha);
// 	m_sliProgress.Render();

	if(m_idCurMediaType==MEDIA_TYPE_USB1 || m_idCurMediaType==MEDIA_TYPE_USB2)
	{
		m_tagIconBack.SetAlpha(m_fCurAlpha*0.5f);
		m_tagIconBack.Render();
		m_tagUSB.SetAlpha(m_fCurAlpha*0.85f);
		m_tagUSB.Render();
	}
	else if(m_idCurMediaType==MEDIA_TYPE_SD1
#ifdef USE_GPS_CARD_MEDIA_PLAY
        || m_idCurMediaType==MEDIA_TYPE_SD2
#endif
        )
	{
		m_tagIconBack.SetAlpha(m_fCurAlpha*0.5f);
		m_tagIconBack.Render();
		m_tagSD.SetAlpha(m_fCurAlpha*0.85f);
		m_tagSD.Render();
	}
	else if(m_idCurMediaType==MEDIA_TYPE_A2DP)
	{
		m_tagIconBack.SetAlpha(m_fCurAlpha*0.5f);
		m_tagIconBack.Render();
		m_tagBT.SetAlpha(m_fCurAlpha*0.85f);
		m_tagBT.Render();
	}
	else if(m_idCurMediaType==MEDIA_TYPE_IPOD)
	{
		m_tagIconBack.SetAlpha(m_fCurAlpha*0.5f);
		m_tagIconBack.Render();
		m_tagiPod.SetAlpha(m_fCurAlpha*0.85f);
		m_tagiPod.Render();
	}

	m_tagTrack.SetAlpha(m_fCurAlpha);
	m_tagTrack.Render();

	m_tagArtist.SetAlpha(m_fCurAlpha);
	m_tagArtist.Render();

	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_btnPlay.SetAlpha(m_fCurAlpha);
	m_btnPlay.Render();

	m_btnPause.SetAlpha(m_fCurAlpha);
	m_btnPause.Render();

// 	m_tagPlayedTime.SetAlpha(m_fCurAlpha);
// 	m_tagPlayedTime.Render();
//
// 	m_tagRemainTime.SetAlpha(m_fCurAlpha);
// 	m_tagRemainTime.Render();
#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetMediaCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableSendNotify = CVTE_DEF_MEDIA_WIDGET_VISIBLE;

	m_bVideoMode = m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory == MEDIA_VIDEO;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	VIEW_STATE vs;
	m_tagAlbumArt.Initialize(IDC_MEDIA_TAG_ALBUMART,NULL,m_pSkinManager->GetTexture(TEXID_001),NULL);
	// 	m_tagAlbumArt.SetShowScale((cx-6.0f-4.0f)/(256.0f/*-8.0f*/));
	m_tagAlbumArt.SetShowScale((cx-16)/(256.0f));
	m_tagAlbumArt.SetPos(0,0);
	vs=m_tagAlbumArt.GetCurViewState();
	vs.fRotateX=180;
	m_tagAlbumArt.SetIniViewState(&vs);
	m_tagAlbumArt.Show(!m_bVideoMode);

	//ipod album:
	m_tagAlbumArtForIpod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_003),NULL);
	// 	m_tagAlbumArt.SetShowScale((cx-6.0f-4.0f)/(256.0f/*-8.0f*/));
	m_tagAlbumArtForIpod.SetShowScale((cx-16)/(320.0f));
	m_tagAlbumArtForIpod.SetPos(0,0);
	vs=m_tagAlbumArtForIpod.GetCurViewState();
	vs.fRotateX=180;
	m_tagAlbumArtForIpod.SetIniViewState(&vs);

	m_tagIconBack.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_085),NULL);
	m_tagIconBack.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);

	m_tagUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_210),NULL);
	m_tagSD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_212),NULL);
	m_tagBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_208),NULL);
	m_tagiPod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_253),NULL);

	m_tagUSB.SetShowScale(0.75f);
	m_tagSD.SetShowScale(0.75f);
	m_tagBT.SetShowScale(0.75f);
	m_tagiPod.SetShowScale(0.75f);

	m_tagUSB.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);
	m_tagSD.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);
	m_tagBT.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);
	m_tagiPod.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);

	m_tagVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_002),NULL);
	m_tagVideo.SetShowScale((cx-16)/(256.0f));
	m_tagVideo.SetPos(0,0);
	m_tagVideo.Show(m_bVideoMode);

	COLORGL crw={1.0,1.0,1.0,1.0};
	COLORGL crg={0.1,0.1,0.1,1.0};
	SIZE siInfo={m_siCurSize.cx,40};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaptionHeight(22);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+30);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_tagTitle.SetCaption(_T("Media Player"));

	m_ptagTitle = &m_tagTitle;
	/*
	m_tagTrack.Initialize(IDC_MEDIA_TAG_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTrack.SetSize(&siInfo);
	m_tagTrack.SetCaptionHeight(22);
	m_tagTrack.LinkTextGL(pTextGL);
	m_tagTrack.SetPos(-32,-m_siCurSize.cy/2+70);
	m_tagTrack.EnableDrawBack(FALSE);
	m_tagTrack.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTrack.SetCaptionColor(&crw);

	m_tagArtist.Initialize(IDC_MEDIA_TAG_ARTIST,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagArtist.SetSize(&siInfo);
	m_tagArtist.SetCaptionHeight(18);
	m_tagArtist.LinkTextGL(pTextGL);
	m_tagArtist.SetPos(-32,-m_siCurSize.cy/2+36);
	m_tagArtist.EnableDrawBack(FALSE);
	m_tagArtist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagArtist.SetCaptionColor(&crw);
	*/

	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,m_pSkinManager->GetTexture(TEXID_113),NULL,
		m_pSkinManager->GetTexture(TEXID_118),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(CWidgetMediaCore::OnNotify,this);
	m_btnPlay.SetScale(1.2f,1.2f*pix_ratio);
	m_btnPlay.SetPos(0,23);
	//m_btnPlay.SetTouchRate(2.0f,2.0f);
	m_btnPlay.EnableClickAni(TRUE);

	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,m_pSkinManager->GetTexture(TEXID_115),NULL,
		m_pSkinManager->GetTexture(TEXID_119),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(CWidgetMediaCore::OnNotify,this);
	m_btnPause.SetScale(1.2f,1.2f*pix_ratio);
	m_btnPause.SetPos(0,23);
	//m_btnPause.SetTouchRate(2.0f,2.0f);
	m_btnPause.EnableClickAni(TRUE);
	m_btnPause.Show(FALSE);
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat texCoord_Media[]={
	111.0/512, 98.0/512.0,
	111.0/512, 300.0/512.0,
	507.0/512, 98.0/512.0,
	507.0/512.0, 300.0/512.0};

	GLfloat texCoord_MediaSelect[]={
		111.0/512, 305.0/512.0,
		111.0/512, 507.0/512.0,
		507.0/512, 305.0/512.0,
		507.0/512.0, 507.0/512.0};

	GLfloat texCoord_Album[]={
		0.0/256.0, 0.0/256.0,
		0.0/256.0, 202.0/256.0,
		256.0/256.0, 0.0/256.0,
		256.0/256.0, 202.0/256.0};
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_iconMedia.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_143),texCoord_Media);
	m_iconMediaSelect.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_143),texCoord_MediaSelect);
	m_btnMedia.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnMedia.SetIcon(&m_iconMedia,&m_iconMediaSelect);
	m_btnMedia.EnableDrawBack(FALSE);

	VIEW_STATE vs;
	m_tagAlbumArt.Initialize(IDC_MEDIA_TAG_ALBUMART,NULL,m_pSkinManager->GetTexture(TEXID_001),texCoord_Album);
	//m_tagAlbumArt.SetShowScale(202.0f/256.0f);
	m_tagAlbumArt.SetPos(70,0);
	vs = m_tagAlbumArt.GetCurViewState();
	vs.fRotateX = 180;
	m_tagAlbumArt.SetIniViewState(&vs);
	m_tagAlbumArt.Show(!m_bVideoMode);

	//ipod album:
	m_tagAlbumArtForIpod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_003),NULL);
	m_tagAlbumArtForIpod.SetShowScale(202.0f/320.0f);
	m_tagAlbumArtForIpod.SetPos(97,0);
	vs=m_tagAlbumArtForIpod.GetCurViewState();
	vs.fRotateX = 180;
	m_tagAlbumArtForIpod.SetIniViewState(&vs);
	
	//Video album
	m_tagVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_002),NULL);
	m_tagVideo.SetShowScale(202.0f/256.0f);
	m_tagVideo.SetPos(97,0);
	vs = m_tagVideo.GetCurViewState();
	vs.fRotateX = 180;
	m_tagVideo.SetIniViewState(&vs);
	m_tagVideo.Show(m_bVideoMode);
	
	COLORGL crw={1.0,1.0,1.0,1.0};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crb={0.17,0.66,0.77,1.0};
	SIZE siInfo={m_siCurSize.cx-88,20};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.SetPos(21-400,139-480);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_MEDIA));
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

	SIZE siTagTack = {270, 38};
	m_tagTrack.Initialize(IDC_MEDIA_TAG_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTrack.LinkTextGL(pTextGL);
	m_tagTrack.SetSize(&siTagTack);
	m_tagTrack.SetCaptionHeight(28);
	m_tagTrack.SetPos(-m_siCurSize.cx/2+17+siTagTack.cx/2,m_siCurSize.cy/2-69-siTagTack.cy/2);
	m_tagTrack.EnableDrawBack(FALSE);
	m_tagTrack.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTrack.SetCaptionColor(&crb);
	m_tagTrack.SetCaption(m_track.String(),TRUE);

	SIZE siTagArtist = {270,24};
	m_tagArtist.Initialize(IDC_MEDIA_TAG_ARTIST,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagArtist.LinkTextGL(pTextGL);
	m_tagArtist.SetSize(&siTagArtist);
	m_tagArtist.SetCaptionHeight(18);
	m_tagArtist.SetPos(-m_siCurSize.cx/2+17+siTagArtist.cx/2,-m_siCurSize.cy/2+63+siTagArtist.cy/2);
	m_tagArtist.EnableDrawBack(FALSE);
	m_tagArtist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagArtist.SetCaptionColor(&crw);
	m_tagArtist.SetCaption(m_artist.String(),TRUE);

	m_tagMediaType.Initialize(IDC_MEDIA_TYPE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagMediaType.SetSize(200,24);
	m_tagMediaType.SetCaptionHeight(18);
	m_tagMediaType.SetCaption(GetResourceString(IDS_TITLE_MEDIA));
	m_tagMediaType.LinkTextGL(pTextGL);
	m_tagMediaType.SetPos(-m_siCurSize.cx/2+119,m_siCurSize.cy/2-27);
	m_tagMediaType.EnableDrawBack(FALSE);
	m_tagMediaType.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagMediaType.SetCaptionColor(&crw);
	
	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,m_pSkinManager->GetTexture(TEXID_144),NULL,
						 m_pSkinManager->GetTexture(TEXID_145),NULL,
						 NULL,NULL,
						 NULL,NULL);
	m_btnPlay.SetNotifyFunc(CWidgetMediaCore::OnNotify,this);
	m_btnPlay.SetPos(m_siCurSize.cx/2-89,m_siCurSize.cy/2-101);
	m_btnPlay.SetScale(1.0f,1.0f*pix_ratio);
	m_btnPlay.SetTouchRate(1.5f,1.5f);
	//m_btnPlay.EnableClickAni(TRUE);

	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,m_pSkinManager->GetTexture(TEXID_146),NULL,
						  m_pSkinManager->GetTexture(TEXID_147),NULL,
						  NULL,NULL,
						  NULL,NULL);
	m_btnPause.SetNotifyFunc(CWidgetMediaCore::OnNotify,this);
	m_btnPause.SetPos(m_siCurSize.cx/2-89,m_siCurSize.cy/2-101);
	m_btnPause.SetScale(1.0f,1.0f*pix_ratio);
	m_btnPause.SetTouchRate(1.5f,1.5f);
	//m_btnPause.EnableClickAni(TRUE);
	m_btnPause.Show(FALSE);
#else

	VIEW_STATE vs;
	m_tagAlbumArt.Initialize(IDC_MEDIA_TAG_ALBUMART,NULL,m_pSkinManager->GetTexture(TEXID_001),NULL);
// 	m_tagAlbumArt.SetShowScale((cx-6.0f-4.0f)/(256.0f/*-8.0f*/));
	m_tagAlbumArt.SetShowScale((cx-14)/(256.0f));
	m_tagAlbumArt.SetPos(0,(cy-cx)/2/*-1.0f*/);
	vs=m_tagAlbumArt.GetCurViewState();
	vs.fRotateX=180;
	m_tagAlbumArt.SetIniViewState(&vs);
	m_tagAlbumArt.Show(!m_bVideoMode);

	//ipod album:
	m_tagAlbumArtForIpod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_003),NULL);
	// 	m_tagAlbumArt.SetShowScale((cx-6.0f-4.0f)/(256.0f/*-8.0f*/));
	m_tagAlbumArtForIpod.SetShowScale((cx-14)/(320.0f));
	m_tagAlbumArtForIpod.SetPos(0,(cy-cx)/2/*-1.0f*/);
	vs=m_tagAlbumArtForIpod.GetCurViewState();
	vs.fRotateX=180;
	m_tagAlbumArtForIpod.SetIniViewState(&vs);

	m_tagIconBack.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_085),NULL);
	m_tagIconBack.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);

	m_tagUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_210),NULL);
	m_tagSD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_212),NULL);
	m_tagBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_208),NULL);
	m_tagiPod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_253),NULL);

	m_tagUSB.SetShowScale(0.75f);
	m_tagSD.SetShowScale(0.75f);
	m_tagBT.SetShowScale(0.75f);
	m_tagiPod.SetShowScale(0.75f);

	m_tagUSB.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);
	m_tagSD.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);
	m_tagBT.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);
	m_tagiPod.SetPos(m_siCurSize.cx/2-40,m_siCurSize.cy/2-40);

	m_tagVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_002),NULL);
	m_tagVideo.SetShowScale((cx-14)/(256.0f));
	m_tagVideo.SetPos(0,(cy-cx)/2/*-1.0f*/);
	m_tagVideo.Show(m_bVideoMode);

	COLORGL crw={1.0,1.0,1.0,1.0};
	COLORGL crg={0.1,0.1,0.1,1.0};
	SIZE siInfo={m_siCurSize.cx-88,20};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(-32,-m_siCurSize.cy/2+22);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_MEDIA));
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

	m_tagTrack.Initialize(IDC_MEDIA_TAG_TITLE,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTrack.LinkTextGL(pTextGL);
	m_tagTrack.SetSize(&siInfo);
	m_tagTrack.SetCaptionHeight(22);
	m_tagTrack.SetPos(-32,-m_siCurSize.cy/2+70);
	m_tagTrack.EnableDrawBack(FALSE);
	m_tagTrack.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTrack.SetCaptionColor(&crw);
	m_tagTrack.SetCaption(m_track.String(),TRUE);

	m_tagArtist.Initialize(IDC_MEDIA_TAG_ARTIST,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagArtist.LinkTextGL(pTextGL);
	m_tagArtist.SetSize(&siInfo);
	m_tagArtist.SetCaptionHeight(18);
	m_tagArtist.SetPos(-32,-m_siCurSize.cy/2+36);
	m_tagArtist.EnableDrawBack(FALSE);
	m_tagArtist.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagArtist.SetCaptionColor(&crw);
	m_tagArtist.SetCaption(m_artist.String(),TRUE);

	//slider:
// 	SLIDERSTRUCTURE sli;
// 	memset(&sli,0,sizeof(sli));
// 	sli.size.cx=140;
// 	sli.size.cy=16;
// 	sli.bHasBtns=FALSE;
// 	sli.bFilled=FALSE;
// 	sli.bFilledBoth=FALSE;
// 	sli.idBackTex=TEXID_052;
// 	sli.idChannelTex=TEXID_053;
// 	sli.idThumbTex=TEXID_051;
// 	m_sliProgress.Initialize(IDC_MEDIA_SLI_PROGRESS,this,&sli,pSkinManager);
// 	m_sliProgress.SetRange(0,512);
// 	m_sliProgress.SetValue(0);
// 	m_sliProgress.SetAutoHideBtns(TRUE);
// 	m_sliProgress.SetPos(0.0f,-120.0f);
// 	m_sliProgress.SetAutoBackFill(TRUE);


	m_btnPlay.Initialize(IDC_MEDIA_BTN_PLAY,this,m_pSkinManager->GetTexture(TEXID_113),NULL,
		m_pSkinManager->GetTexture(TEXID_118),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlay.SetNotifyFunc(CWidgetMediaCore::OnNotify,this);
	m_btnPlay.SetPos(m_siCurSize.cx/2-64,-m_siCurSize.cy/2+52);
	m_btnPlay.SetTouchRate(2.0f,2.0f);
	m_btnPlay.EnableClickAni(TRUE);

	m_btnPause.Initialize(IDC_MEDIA_BTN_PAUSE,this,m_pSkinManager->GetTexture(TEXID_115),NULL,
		m_pSkinManager->GetTexture(TEXID_119),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPause.SetNotifyFunc(CWidgetMediaCore::OnNotify,this);
	m_btnPause.SetPos(m_siCurSize.cx/2-64,-m_siCurSize.cy/2+52);
	m_btnPause.SetTouchRate(2.0f,2.0f);
	m_btnPause.EnableClickAni(TRUE);
	m_btnPause.Show(FALSE);

#endif

	return TRUE;
}
void CWidgetMediaCore::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idControl)
	{
	case IDC_MEDIA_SLI_PROGRESS:
		{
// 			if (idOperation == UIOPRT_SETVALUE)
// 			{
// 				m_sliProgress.SetValue(*((INT*)lpara));
// 			}
		}
		break;
	case IDC_MEDIA_TAG_MEDIATYPE:
		{
			if(idOperation == UIOPRT_SETVALUE)
			{
				m_idCurMediaType = (UINT)lpara;
				m_iNotifyPara = (UINT)lpara;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				switch(m_idCurMediaType)
				{
				case MEDIA_TYPE_A2DP:
					m_tagMediaType.SetCaption(_T("BT"));
					break;
				case MEDIA_TYPE_IPOD:
					m_tagMediaType.SetCaption(_T("iPod"));
					break;
				case MEDIA_TYPE_USB1:
					m_tagMediaType.SetCaption(_T("USB1"));
					break;
				case MEDIA_TYPE_USB2:
					m_tagMediaType.SetCaption(_T("USB2"));
					break;
				case MEDIA_TYPE_SD1:
					m_tagMediaType.SetCaption(_T("SD1"));
					break;
				case MEDIA_TYPE_SD2:
					m_tagMediaType.SetCaption(_T("SD2"));
					break;
				default:
					break;
				}
#endif
			}
			else if(idOperation == UIOPRT_GETVALUE)
			{
				if(lpara)
				{
					*((UINT*)(lpara)) = m_idCurMediaType ;
				}
			}
		}
		break;
	case IDC_MEDIA_TAG_ALBUMART:
		{
			if(idOperation == UIOPRT_SHOWHIDE)
			{
// 				m_tagAlbumArt.Show(*((BOOL*)lpara));
// 				m_tagVideo.Show(!*((BOOL*)lpara));

				m_bVideoMode = (!*((BOOL*)lpara));
			}
			else if(idOperation == UIOPRT_SETVALUE)
			{
				m_bHasAlbumArt = (*((BOOL*)lpara));
			}
		}
		break;
	case IDC_MEDIA_TAG_TITLE:
		{
			//if(m_idCurMediaType!=0)
			{
				if(idOperation == UIOPRT_SETSTRING)
				{
					m_track = (const wchar_t*)lpara;
					m_tagTrack.SetCaption(m_track.String(),TRUE);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

#else
					m_tagTitle.Show(lpara==NULL);
#endif
				}
			}
		}
		break;
	case IDC_MEDIA_TAG_ARTIST:
		{
			//if(m_idCurMediaType!=0)
			{
				if(idOperation == UIOPRT_SETSTRING)
				{
					m_artist = (const wchar_t*)lpara;
					m_tagArtist.SetCaption(m_artist.String(),TRUE);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

#else
					m_tagTitle.Show(lpara==NULL);
#endif
				}
			}
		}
		break;
	case IDC_MEDIA_BTN_PAUSE:
		{
			//if(m_idCurMediaType!=0)
			{
				if(idOperation == UIOPRT_SHOWHIDE)
				{
					m_btnPause.Show(*((BOOL*)lpara));
				}
				else if (idOperation == UIOPRT_ENABLE)
				{
					m_btnPause.Enable(*((BOOL*)lpara));
				}
			}
		}
		break;
	case IDC_MEDIA_BTN_PLAY:
		{
			//if(m_idCurMediaType!=0)
			{
				if(idOperation == UIOPRT_SHOWHIDE)
				{
					m_btnPlay.Show(*((BOOL*)lpara));
				}
				else if (idOperation == UIOPRT_ENABLE)
				{
					m_btnPlay.Enable(*((BOOL*)lpara));
				}
			}
		}
		break;
	}
}
void CALLBACK CWidgetMediaCore::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{

	CWidgetMediaCore *panel=(CWidgetMediaCore*)lpUserPara;

	RETAILMSG(DEBUG_MAINMENU/*DEBUG_MAINMENU*/,(_T("CWidgetMediaCore OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CWidgetMediaCore::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_MEDIA_BTN_PLAY:
		{
			#if CVTE_EN_CARPLAY
			{
				const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();
				if(mode->phoneCall == kAirPlayEntity_Controller)
				{
					RETAILMSG(1,(_T("MSG:[CWidgetMediaCore::HandleControlMSG] PLAY skip!!!\r\n")));
					break;
				}
			}
			#endif

			UINT idcurSRC=m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC;

			if(m_idCurMediaType==MEDIA_TYPE_USB1 || m_idCurMediaType==MEDIA_TYPE_USB2 ||
				m_idCurMediaType==MEDIA_TYPE_SD1
#ifdef USE_GPS_CARD_MEDIA_PLAY
				|| m_idCurMediaType==MEDIA_TYPE_SD2
#endif
				)//mass storage
			{
				if(GET_PANEL_CLASS_ID(idcurSRC) == UI_CLASS_MEDIA && !m_bVideoMode)
				{
                    SwitchAudioSRC(SRC_AUDIO_ARM, TRUE);
					SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PLAY);
				}
				else
				{// BUG 2738,fix press main menu media play button switch panel enter video don`t auto play
					SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,this,TRUE);
				}
			}
			else if(m_idCurMediaType==MEDIA_TYPE_A2DP)
			{
				if(GET_PANEL_CLASS_ID(idcurSRC) == UI_CLASS_BT)
				{
                    SwitchAudioSRC(SRC_AUDIO_ARM, TRUE);
					SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PLAY);
				}
				else
				{
					SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_ZOOMOUT,-1,this,TRUE);
				}
			}
			else if(m_idCurMediaType==MEDIA_TYPE_IPOD)
			{
				if(GET_PANEL_CLASS_ID(idcurSRC) == UI_CLASS_IPOD)
				{
                    SwitchAudioSRC(SRC_AUDIO_ARM, TRUE);
					SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_PLAY);
				}
				else
				{
					SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,this,TRUE);
				}
			}
			else
			{
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,this,TRUE);
			}
		}
		break;
	case IDC_MEDIA_BTN_PAUSE:
		{
			#if CVTE_EN_CARPLAY
			{
				const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();
				if(mode->phoneCall == kAirPlayEntity_Controller)
				{
					RETAILMSG(1,(_T("MSG:[CWidgetMediaCore::HandleControlMSG] PAUSE skip!!!\r\n")));
					break;
				}
			}
			#endif

			if(m_idCurMediaType==MEDIA_TYPE_USB1 || m_idCurMediaType==MEDIA_TYPE_USB2 ||
				m_idCurMediaType==MEDIA_TYPE_SD1
#ifdef USE_GPS_CARD_MEDIA_PLAY
				|| m_idCurMediaType==MEDIA_TYPE_SD2
#endif
				)//mass storage
			{
				SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_PAUSE);
			}
			else if(m_idCurMediaType==MEDIA_TYPE_A2DP)
			{
				SendBluetoothCommand(BT_CMD_A2DP_CONTROL,A2DP_CONTROL_PAUSE);
			}
			else if(m_idCurMediaType==MEDIA_TYPE_IPOD)
			{
				SendiPodCommand(IPOD_CMD_PLAY_CONTROL,IPOD_CONTROL_PAUSE);
			}
		}
		break;
	}
}

//radio widget:
CWidgetRadioCore::CWidgetRadioCore(void)
{
	m_idType = UI_CLASS_RADIO;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_idRadioband = RADIO_BAND_ID_FM;
#endif
}
CWidgetRadioCore::~CWidgetRadioCore(void)
{

}

BOOL CWidgetRadioCore::IsReady()
{
	return CWidgetGL::IsReady();

}
void CWidgetRadioCore::Render()
{

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
 	m_tagTitle.SetLuminance(m_fCurLuminance);
 	m_tagTitle.SetAlpha(m_fCurAlpha);
 	m_tagTitle.Render();

	m_tagRadio.SetLuminance(m_fCurLuminance);
	m_tagRadio.SetAlpha(m_fCurAlpha*0.9f);
	m_tagRadio.Render();
#elif(CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_172):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	m_iconRadio.SetLuminance(m_fCurLuminance);
	m_iconRadio.SetAlpha(m_fCurAlpha);
	m_iconRadio.Render();

	m_tagFrequency.SetLuminance(m_fCurLuminance);
	m_tagFrequency.SetAlpha(m_fCurAlpha);
	m_tagFrequency.Render();

	m_btnPsInfo.SetLuminance(m_fCurLuminance);
	m_btnPsInfo.SetAlpha(m_fCurAlpha);
	m_btnPsInfo.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	m_btnRadio.SetStatus(m_bPressed?BS_DOWN:BS_UP);
	m_btnRadio.SetLuminance(m_fCurLuminance);
	m_btnRadio.SetAlpha(m_fCurAlpha);
	m_btnRadio.Render();

	m_tagTitle.SetAlpha(m_fCurAlpha*0.6f);
	m_tagTitle.Render();

	m_tagFrequency.SetLuminance(m_fCurLuminance);
	m_tagFrequency.SetAlpha(m_fCurAlpha);
	m_tagFrequency.Render();

	m_tagFreqUnit.SetLuminance(m_fCurLuminance);
	m_tagFreqUnit.SetAlpha(m_fCurAlpha);
	m_tagFreqUnit.Render();

	m_tagRadioStation.SetLuminance(m_fCurLuminance);
	m_tagRadioStation.SetAlpha(m_fCurAlpha);
	m_tagRadioStation.Render();
#else
	m_tagRadio.SetLuminance(m_fCurLuminance);
	m_tagRadio.SetAlpha(m_fCurAlpha*0.9f);
	m_tagRadio.Render();

	m_tagFrequency.SetLuminance(m_fCurLuminance);
	m_tagFrequency.SetAlpha(m_fCurAlpha);
	m_tagFrequency.Render();

// 	m_tagBandTitle.SetLuminance(m_fCurLuminance);
// 	m_tagBandTitle.SetAlpha(m_fCurAlpha);
// 	m_tagBandTitle.Render();

	m_tagFreqUnit.SetLuminance(m_fCurLuminance);
	m_tagFreqUnit.SetAlpha(m_fCurAlpha);
	m_tagFreqUnit.Render();
	m_btnPowerOn.SetLuminance(m_fCurLuminance);
	m_btnPowerOn.SetAlpha(m_fCurAlpha);
	m_btnPowerOn.Render();

	m_btnPowerOff.SetLuminance(m_fCurLuminance);
	m_btnPowerOff.SetAlpha(m_fCurAlpha);
	m_btnPowerOff.Render();
#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetRadioCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
// 	GLfloat texCoord_s01000[]={
// 		282.0/512.0, 122.0/512.0,
// 		282.0/512.0, 506.0/512.0,
// 		450.0/512.0, 122.0/512.0,
// 		450.0/512.0, 506.0/512.0};

// 		m_tagRadio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150),texCoord_s01000);
// 		VIEW_STATE vs=m_tagRadio.GetCurViewState();
// 		vs.fRotateZ=90;
// 		m_tagRadio.SetIniViewState(&vs);
// 		//m_tagRadio.SetScale((cy-14)/167.0f,(cx-14)/384.0f);

	m_iconRadio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_180),NULL);
	m_iconRadio.SetScale(1.3f,1.0f);
	m_iconRadio.SetPos(0,14);

		SIZE siInfo={m_siCurSize.cx-24,20};
		COLORGL crw={1.0,1.0,1.0,1.0};


		m_tagFrequency.Initialize(IDC_RADIO_TAG_CURFREQUENCY,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagFrequency.SetSize(&siInfo);
		m_tagFrequency.SetCaption(_T("87.50"));
		m_tagFrequency.LinkTextGL(pTextGL);
		m_tagFrequency.SetPos(-14,19);
		m_tagFrequency.SetCaptionHeight(20);
		m_tagFrequency.EnableDrawBack(FALSE);
		m_tagFrequency.SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagFrequency.SetCaptionColor(&crw);

		m_btnPsInfo.Initialize(IDC_RADIO_TAG_PS_INFO,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
		m_btnPsInfo.SetSize(&siInfo);
		m_btnPsInfo.SetCaption(_T(""));
		m_btnPsInfo.LinkTextGL(pTextGL);
		m_btnPsInfo.SetPos(0,-64);
		m_btnPsInfo.SetCaptionHeight(16);
		m_btnPsInfo.EnableDrawBack(FALSE);
		m_btnPsInfo.SetCaptionPos(BTN_CAPTION_LEFT);
		m_btnPsInfo.SetCaptionColor(&crw);

		//m_tagFrequency.SetCaptionCharStyle(STYLE_ART);
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	GLfloat texCoord_s01000[]={
		132.0/256.0, 4.0/256.0,
		132.0/256.0, 124.0/256.0,
		252.0/256.0, 4.0/256.0,
		252.0/256.0, 124.0/256.0};

	m_tagRadio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_153),texCoord_s01000);
	m_tagRadio.SetScale(1.2f,1.2f);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_RADIO));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat texCoord_Radio[]={
		111.0/512, 98.0/512.0,
		111.0/512, 300.0/512.0,
		507.0/512, 98.0/512.0,
		507.0/512.0, 300.0/512.0};

	GLfloat texCoord_RadioSelect[]={
		111.0/512, 305.0/512.0,
		111.0/512, 507.0/512.0,
		507.0/512, 305.0/512.0,
		507.0/512.0, 507.0/512.0};

	m_iconRadio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_100),texCoord_Radio);
	m_iconRadioSelect.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_100),texCoord_RadioSelect);
	m_btnRadio.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnRadio.SetIcon(&m_iconRadio,&m_iconRadioSelect);
	m_btnRadio.EnableDrawBack(FALSE);
	//m_tagRadio.SetScale(1.0f,1.0f);

	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(79,24);
	m_tagTitle.SetCaption(_T("FM Radio"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(-m_siCurSize.cx/2+59,m_siCurSize.cy/2-27);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);

	m_ptagTitle = &m_tagTitle;

	SIZE siTagFreq = {180,24};
	m_tagFrequency.Initialize(IDC_RADIO_TAG_CURFREQUENCY,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagFrequency.SetSize(&siTagFreq);
	m_tagFrequency.SetCaption(_T("87.50"));
	m_tagFrequency.LinkTextGL(pTextGL);
	m_tagFrequency.SetPos(-m_siCurSize.cx/2+104,-m_siCurSize.cy/2+83);
	m_tagFrequency.SetCaptionHeight(28);
	m_tagFrequency.EnableDrawBack(FALSE);
	m_tagFrequency.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagFrequency.SetCaptionColor(&crw);

	SIZE siTagFreqUnit = {60,24};
	m_tagFreqUnit.Initialize(IDC_RADIO_TAG_FREQ_UNIT,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagFreqUnit.SetSize(&siTagFreqUnit);
	m_tagFreqUnit.SetCaption(GetResourceString(IDS_MHZ));
	m_tagFreqUnit.LinkTextGL(pTextGL);
	m_tagFreqUnit.SetPos(-m_siCurSize.cx/2+160,-m_siCurSize.cy/2+83);
	m_tagFreqUnit.SetCaptionHeight(28);
	m_tagFreqUnit.EnableDrawBack(FALSE);
	m_tagFreqUnit.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagFreqUnit.SetCaptionColor(&crw);

	COLORGL crb={0.17,0.66,0.77,1.0};
	m_tagRadioStation.Initialize(IDC_RADIO_TAG_PS_INFO,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagRadioStation.SetSize(&siTagFreq);
	m_tagRadioStation.SetCaption(m_ps.String());
	m_tagRadioStation.LinkTextGL(pTextGL);
	m_tagRadioStation.SetPos(-m_siCurSize.cx/2+104,m_siCurSize.cy/2-88);
	m_tagRadioStation.SetCaptionHeight(28);
	m_tagRadioStation.EnableDrawBack(FALSE);
	m_tagRadioStation.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagRadioStation.SetCaptionColor(&crb);
#else
	GLfloat texCoord_s01000[]={
		334.0/512.0, 174.0/512.0,
		334.0/512.0, 498.0/512.0,
		502.0/512.0, 174.0/512.0,
		502.0/512.0, 498.0/512.0};

	m_tagRadio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150),texCoord_s01000);
	VIEW_STATE vs=m_tagRadio.GetCurViewState();
	vs.fRotateZ=90;
	m_tagRadio.SetIniViewState(&vs);
// 	m_tagRadio.SetScale((cx-14)/316.0f,(cy-14)/160.0f);
	m_tagRadio.SetScale((cy-14)/160.0f,(cx-14)/316.0f);


	SIZE siInfo={m_siCurSize.cx-/*24*/80,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crr={1.0,0.0,0.0,1.0};

	//m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	//m_tagTitle.SetSize(&siInfo);
	//m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_RADIO));
	//m_tagTitle.LinkTextGL(pTextGL);
	//m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	//m_tagTitle.SetCaptionHeight(18);
	//m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	//m_tagTitle.SetCaptionColor(&crg);
	////m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));

	SIZE siInfo2={80,40};
	m_tagBandTitle.Initialize(IDC_RADIO_TAG_BAND_TITLE,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagBandTitle.SetSize(&siInfo2);
	m_tagBandTitle.SetCaption(_T("FM"));
	m_tagBandTitle.LinkTextGL(pTextGL);
	m_tagBandTitle.SetPos(-94,-10);
	m_tagBandTitle.SetCaptionHeight(20);
	m_tagBandTitle.EnableDrawBack(FALSE);
	m_tagBandTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagBandTitle.SetCaptionColor(&crg);

	COLORGL cry={0.59,0.44,0.19,1.0};

	m_tagFreqUnit.Initialize(IDC_RADIO_TAG_FREQ_UNIT,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagFreqUnit.SetSize(&siInfo2);
	m_tagFreqUnit.SetCaption(GetResourceString(IDS_MHZ));
	m_tagFreqUnit.LinkTextGL(pTextGL);
	m_tagFreqUnit.SetPos(24,-24);
	m_tagFreqUnit.SetCaptionHeight(18);
	m_tagFreqUnit.EnableDrawBack(FALSE);
	m_tagFreqUnit.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagFreqUnit.SetCaptionColor(&cry);

 
	m_tagFrequency.Initialize(IDC_RADIO_TAG_CURFREQUENCY,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagFrequency.SetSize(&siInfo);
	m_tagFrequency.SetCaption(_T("87.50"));
	m_tagFrequency.LinkTextGL(pTextGL);
	m_tagFrequency.SetPos(/*13*/-16,-21);
	m_tagFrequency.SetCaptionHeight(26);
	m_tagFrequency.EnableDrawBack(FALSE);
	m_tagFrequency.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagFrequency.SetCaptionColor(&cry);

	//m_tagFrequency.SetCaptionCharStyle(STYLE_ART);

	m_btnPowerOn.Initialize(IDC_RADIO_BTN_POWERON,this,m_pSkinManager->GetTexture(TEXID_116),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPowerOn.SetNotifyFunc(CWidgetRadioCore::OnNotify,this);
	m_btnPowerOn.SetPos(m_siCurSize.cx/2-44,-24);
	m_btnPowerOn.SetTouchRate(1.0f,1.0f);

	m_btnPowerOff.Initialize(IDC_RADIO_BTN_POWEROFF,this,m_pSkinManager->GetTexture(TEXID_114),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPowerOff.SetNotifyFunc(CWidgetRadioCore::OnNotify,this);
	m_btnPowerOff.SetPos(m_siCurSize.cx/2-44,-24);
	m_btnPowerOff.SetTouchRate(1.0f,1.0f);
	m_btnPowerOff.Show(FALSE);
#endif
	

	return TRUE;
}

void CWidgetRadioCore::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	switch (idControl)
	{
	case IDM_RADIO_WIDGET_TYPE:
		{
			if (idOperation == UIOPRT_SETVALUE)
			{
				m_idType = (UINT)lpara;
			}
			else if (idOperation == UIOPRT_GETVALUE)
			{
				if(lpara)
				{
					*((UINT*)lpara) = m_idType;
				}
			}
		}
		break;
	case IDC_RADIO_TAG_CURFREQUENCY:
		break;
	case IDC_RADIO_TAG_FREQ_UNIT:
		break;
	case IDC_RADIO_BTN_POWERON:
		break;
	case IDC_RADIO_BTN_POWEROFF:
		break;
	case IDC_RADIO_TAG_PS_INFO:
		break;
	default:
		break;
	}
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	switch (idControl)
	{
	case IDM_RADIO_WIDGET_TYPE:
		{
			if (idOperation == UIOPRT_SETVALUE)
			{
				m_idType = (UINT)lpara;
			}
			else if (idOperation == UIOPRT_GETVALUE)
			{
				if(lpara)
				{
					*((UINT*)lpara) = m_idType;
				}
			}
		}
		break;
	case IDC_RADIO_TAG_CURFREQUENCY:
		{
			//RETAILMSG(1,(_T("####: CWidgetRadioCore::OperateControl: IDC_RADIO_TAG_CURFREQUENCY %s\r\n"),lpara));
			if (idOperation == UIOPRT_SETSTRING)
			{
				m_tagFrequency.SetCaption((const wchar_t*)lpara,TRUE);
			}
		}
		break;
	case IDC_RADIO_TAG_FREQ_UNIT:
		break;
	case IDC_RADIO_BTN_POWERON:
		break;
	case IDC_RADIO_BTN_POWEROFF:
		break;
	case IDC_RADIO_TAG_PS_INFO:
		{
			if (idOperation == UIOPRT_SETSTRING)
			{
				m_btnPsInfo.SetCaption((const wchar_t*)lpara,TRUE);
			}
		}
		break;
	default:
		break;
	}
#else
	switch (idControl)
	{
	case IDM_RADIO_WIDGET_TYPE:
		{
			if (idOperation == UIOPRT_SETVALUE)
			{
				m_idType = (UINT)lpara;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				if (m_idType == UI_CLASS_RADIO)
				{
					if (m_idRadioband == RADIO_BAND_ID_FM)
					{
						m_tagTitle.SetCaption(_T("FM Radio"));
					}
					else if (m_idRadioband == RADIO_BAND_ID_AM)
					{
						m_tagTitle.SetCaption(_T("AM Radio"));
					}
				}
				else if (m_idType == UI_CLASS_DAB)
				{
					m_tagTitle.SetCaption(_T("DAB Radio"));
				}
#endif
			}
			else if (idOperation == UIOPRT_GETVALUE)
			{
				if(lpara)
				{
					*((UINT*)lpara) = m_idType;
				}
			}
		}
		break;
	case IDM_RADIO_BAND_TYPE:
		{
			//RETAILMSG(1,(_T("####: CWidgetRadioCore::OperateControl: IDM_RADIO_BAND_TYPE idOperation=%d, RadioBandId=%d\r\n"),idOperation,lpara));
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			if (idOperation == UIOPRT_SETVALUE)
			{
				m_idRadioband = *(RadioBandId*)lpara;
				if (m_idType == UI_CLASS_RADIO)
				{
					if (m_idRadioband == RADIO_BAND_ID_FM)
					{
						m_tagTitle.SetCaption(_T("FM Radio"));
					}
					else if (m_idRadioband == RADIO_BAND_ID_AM)
					{
						m_tagTitle.SetCaption(_T("AM Radio"));
					}
				}
			}
#endif
		}
		break;
	case IDC_RADIO_TAG_CURFREQUENCY:
		{
			RETAILMSG(1,(_T("####: CWidgetRadioCore::OperateControl: IDC_RADIO_TAG_CURFREQUENCY %s\r\n"),lpara));
			if (idOperation == UIOPRT_SETSTRING)
			{
				m_tagFrequency.SetCaption((const wchar_t*)lpara,TRUE);
			}
		}
		break;
	case IDC_RADIO_TAG_FREQ_UNIT:
		{
			if (idOperation == UIOPRT_SETSTRING)
			{
				RETAILMSG(1,(_T("####: CWidgetRadioCore::OperateControl: IDC_RADIO_TAG_FREQ_UNIT %s\r\n"),lpara));
				if(lpara)
				{
					//m_tagFrequency.SetPos(8,-24);
					m_tagFreqUnit.Show(TRUE);
					m_tagFreqUnit.SetCaption((const wchar_t*)lpara);
				}
				else
				{
					//m_tagFrequency.SetPos(8,-24);
					m_tagFreqUnit.Show(FALSE);
				}
			}
		}
		break;
	case IDC_RADIO_BTN_POWERON:
		{
			if(idOperation == UIOPRT_SHOWHIDE)
			{
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
				m_btnPowerOn.Show(*((BOOL*)lpara));
#endif
			}
		}
		break;
	case IDC_RADIO_BTN_POWEROFF:
		{
			if(idOperation == UIOPRT_SHOWHIDE)
			{
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
				m_btnPowerOff.Show(*((BOOL*)lpara));
#endif
			}
		}
		break;
	case IDC_RADIO_TAG_PS_INFO:
		{
			if (idOperation == UIOPRT_SETSTRING)
			{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				m_ps = (const wchar_t*)lpara;
				if (m_ps.Size() > 0)
				{
					m_tagFrequency.SetCaptionHeight(18);
					m_tagFrequency.SetPos(-m_siCurSize.cx/2+104,-m_siCurSize.cy/2+72);
					m_tagFreqUnit.SetCaptionHeight(18);
					m_tagFreqUnit.SetPos(-m_siCurSize.cx/2+94,-m_siCurSize.cy/2+72);
					m_tagRadioStation.SetCaption(m_ps.String());
					m_tagRadioStation.Show(TRUE);
				}
				else
				{
					m_tagFrequency.SetCaptionHeight(28);
					m_tagFrequency.SetPos(-m_siCurSize.cx/2+104,-m_siCurSize.cy/2+83);
					m_tagFreqUnit.SetCaptionHeight(28);
					m_tagFreqUnit.SetPos(-m_siCurSize.cx/2+160,-m_siCurSize.cy/2+83);
					m_tagRadioStation.Show(FALSE);
				}
#endif
			}
		}
		break;
	default:
		break;
	}
#endif
}
void CALLBACK CWidgetRadioCore::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{

	CWidgetRadioCore *panel=(CWidgetRadioCore*)lpUserPara;

	RETAILMSG(DEBUG_MAINMENU/*DEBUG_MAINMENU*/,(_T("CWidgetRadioCore OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CWidgetRadioCore::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_RADIO_BTN_POWERON:
		{
			#if CVTE_EN_CARPLAY
			{
				const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();
				if(mode->phoneCall == kAirPlayEntity_Controller)
				{
					RETAILMSG(1,(_T("MSG:[CWidgetRadioCore::HandleControlMSG] POWER ON skip!!!\r\n")));
					break;
				}
			}
			#endif
// 			m_btnPowerOn.Show(FALSE);
// 			m_btnPowerOff.Show(TRUE);

			if(m_idType == UI_CLASS_RADIO && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC)==UI_CLASS_RADIO)
				SendRadioCommand(RADIO_CMD_SET_PLAYSTATE,1);
			else if(m_idType == UI_CLASS_DAB)
				SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,this,TRUE);
			else
				SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,this,TRUE);
		}
		break;
	case IDC_RADIO_BTN_POWEROFF:
		{
			#if CVTE_EN_CARPLAY
			{
				const AirPlayModeState* mode = CCarplayAdapter::GetInstance()->GetModeState();
				if(mode->phoneCall == kAirPlayEntity_Controller)
				{
					RETAILMSG(1,(_T("MSG:[CWidgetRadioCore::HandleControlMSG] POWER OFF skip!!!\r\n")));
					break;
				}
			}
			#endif
// 			m_btnPowerOn.Show(TRUE);
// 			m_btnPowerOff.Show(FALSE);

			if(m_idType == UI_CLASS_RADIO && GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC)==UI_CLASS_RADIO)
				SendRadioCommand(RADIO_CMD_SET_PLAYSTATE,0);
			else if(m_idType == UI_CLASS_DAB)
				SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,this,TRUE);
			else
				SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_ZOOMOUT,-1,this,TRUE);
		}
		break;
	}
}
//phone widget:
CWidgetPhoneCore::CWidgetPhoneCore(void)
{
	m_bPlugin = FALSE;
	m_bOnFocus = FALSE;
	m_bPressed = FALSE;
}
CWidgetPhoneCore::~CWidgetPhoneCore(void)
{

}

BOOL CWidgetPhoneCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagPhone.IsReady();
}
void CWidgetPhoneCore::Render()
{

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	m_tagPhone.SetLuminance(m_fCurLuminance);
	m_tagPhone.SetAlpha(m_fCurAlpha);
	m_tagPhone.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_177):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_174):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	m_tagPhone.SetLuminance(m_fCurLuminance);
	m_tagPhone.SetAlpha(m_fCurAlpha);
	m_tagPhone.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

	DrawRect(pTexBar,0,-m_siCurSize.cy/2+30,m_siCurSize.cx-14-8,46-8,8,&cr,TRUE);

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagPhone.SetAlpha(m_fCurAlpha);
	m_tagPhone.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();
#else

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	m_tagPhone.SetLuminance(m_fCurLuminance);
	m_tagPhone.SetAlpha(m_fCurAlpha);
	m_tagPhone.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

// 	m_tagNumber.SetLuminance(m_fCurLuminance);
// 	m_tagNumber.SetAlpha(m_fCurAlpha);
// 	m_tagNumber.Render();

#endif

	EndRender();

	ProcessNotify();

}

void CWidgetPhoneCore::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
			m_tagPlugin.Show(m_bPlugin);

			//m_bEnableSendNotify = m_bPlugin;

			m_iNotifyPara = m_bPlugin;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagPhone.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
#endif
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}
}

BOOL CWidgetPhoneCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_tagPhone.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_185),NULL);
	m_tagPhone.SetPos(10/*18*/,23);
	m_tagPhone.SetScale(0.65f,0.65f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crr={1.0,0.0,0.0,1.0};

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetScale(0.8f,0.8f*pix_ratio);
	m_tagPlugin.SetPos(cx/2-28,cy/2-28);
	m_tagPlugin.Show(FALSE);

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("Handsfree")/*GetResourceString(IDS_TITLE_PHONE)*/);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+30);
	m_tagTitle.SetCaptionHeight(22);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	GLfloat texCoord_s01000[]={
		4.0/256.0, 4.0/256.0,
		4.0/256.0, 124.0/256.0,
		124.0/256.0, 4.0/256.0,
		124.0/256.0, 124.0/256.0};

	m_tagPhone.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_153),texCoord_s01000);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
	m_tagPlugin.Show(FALSE);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_PHONE));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagPhone.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_123),NULL,
		m_pSkinManager->GetTexture(TEXID_123),NULL,
		m_pSkinManager->GetTexture(TEXID_124),NULL,
		m_pSkinManager->GetTexture(TEXID_122),NULL);
	m_tagPhone.SetPos(0,21);
	m_tagPhone.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	m_tagPhone.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_PHONE));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else

	GLfloat texCoord_s01000[]={
		4.0/512.0, 172/512.0,
		4.0/512.0, 340.0/512.0,
		328.0/512.0, 172.0/512.0,
		328.0/512.0, 340.0/512.0};

	m_tagPhone.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_155),texCoord_s01000);
	m_tagPhone.SetScale((cx-14)/316.0f,(cy-14)/160.0f);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	//m_tagPlugin.SetPos(96,-34);
#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	{
		m_tagPlugin.SetPos(cx/2-32,cy/2-32);
	}
#else
	{
		m_tagPlugin.SetPos(96,-34);
	}
#endif
	m_tagPlugin.Show(FALSE);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_PHONE));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

#endif
	
	return TRUE;
}

//a2dp widget:
CWidgetA2DP::CWidgetA2DP(void)
{
	m_bPlugin = FALSE;
	m_bOnFocus = FALSE;
	m_bPressed = FALSE;
}
CWidgetA2DP::~CWidgetA2DP(void)
{

}

BOOL CWidgetA2DP::IsReady()
{
	return CWidgetGL::IsReady() && m_tagA2DP.IsReady();
}
void CWidgetA2DP::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	m_tagA2DP.SetLuminance(m_fCurLuminance);
	m_tagA2DP.SetAlpha(m_fCurAlpha);
	m_tagA2DP.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_171):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha/**0.7f*/};

	if(m_bPressed || m_bOnFocus)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	m_tagA2DP.SetLuminance(m_fCurLuminance);
	m_tagA2DP.SetAlpha(m_fCurAlpha);
	m_tagA2DP.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagA2DP.SetAlpha(m_fCurAlpha);
	m_tagA2DP.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();
#else
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.7f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);
	// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);

	m_tagA2DP.SetLuminance(m_fCurLuminance);
	m_tagA2DP.SetAlpha(m_fCurAlpha);
	m_tagA2DP.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();
#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetA2DP::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	//m_bEnableSendNotify = FALSE;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	GLfloat texCoord_s01000[]={
		132.0/256.0, 4.0/256.0,
		132.0/256.0, 124.0/256.0,
		252.0/256.0, 4.0/256.0,
		252.0/256.0, 124.0/256.0};

	m_tagA2DP.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_157),texCoord_s01000);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
	m_tagPlugin.Show(FALSE);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_DEVICE_BT));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_tagA2DP.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_181),NULL);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetScale(0.8f,0.8f*pix_ratio);
	m_tagPlugin.SetPos(cx/2-28,cy/2-28);
	m_tagPlugin.Show(FALSE);
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagA2DP.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_108),NULL,
		m_pSkinManager->GetTexture(TEXID_108),NULL,
		m_pSkinManager->GetTexture(TEXID_109),NULL,
		m_pSkinManager->GetTexture(TEXID_107),NULL);
	m_tagA2DP.SetPos(0,21);
	m_tagA2DP.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	m_tagA2DP.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_DEVICE_BT));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else
	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	{
		m_tagPlugin.SetPos(cx/2-32,cy/2-32);
	}
	#else
	{
		m_tagPlugin.SetPos(20,-20);
	}
	#endif

	m_tagPlugin.Show(FALSE);

	m_tagA2DP.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_121),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagA2DP.SetPos(0,10);
	#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	{
		m_tagA2DP.SetShowScale(GLfloat(cy-50)/m_tagA2DP.GetSize().cy);
	}
	#else
	{
		m_tagA2DP.SetShowScale(GLfloat(cx-50)/m_tagA2DP.GetSize().cx);
	}
	#endif

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_DEVICE_BT));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

#endif

	return TRUE;
}


void CWidgetA2DP::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagA2DP.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
#else
			m_tagPlugin.Show(m_bPlugin);
#endif
			//m_bEnableSendNotify = m_bPlugin;

			m_iNotifyPara = m_bPlugin;
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}
}


//Navi widget:
CWidgetNaviCore::CWidgetNaviCore(void)
{
	m_bPressed = FALSE;
	m_bOnFocus = FALSE;
	m_bPlugin = FALSE;
}
CWidgetNaviCore::~CWidgetNaviCore(void)
{

}

BOOL CWidgetNaviCore::IsReady()
{
	return CWidgetGL::IsReady() ;//&& m_tagNavi.IsReady();
}
void CWidgetNaviCore::Render()
{

	if(!BeginRender())return;

// 	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagMap.SetAlpha(m_fCurAlpha);
	m_tagMap.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();
#else
	m_tagMap.SetLuminance(m_fCurLuminance);
	m_tagMap.SetAlpha(m_fCurAlpha*0.9f);
	m_tagMap.Render();

// 	m_tagNavi.SetLuminance(m_fCurLuminance);
// 	m_tagNavi.SetAlpha(m_fCurAlpha);
// 	m_tagNavi.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();
#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetNaviCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	GLfloat texCoord_s01000[]={
		4.0/256.0, 132.0/256.0,
		4.0/256.0, 252.0/256.0,
		124.0/256.0, 132.0/256.0,
		124.0/256.0, 252.0/256.0};
	m_tagMap.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_153),texCoord_s01000);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_NAVI));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagMap.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_117),NULL,
		m_pSkinManager->GetTexture(TEXID_117),NULL,
		m_pSkinManager->GetTexture(TEXID_118),NULL,
		m_pSkinManager->GetTexture(TEXID_116),NULL);
	m_tagMap.SetPos(0,21);
	m_tagMap.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	m_tagMap.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_NAVI));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else
	GLfloat texCoord_s01000[]={
		4.0/512.0, 340.0/512.0,
		4.0/512.0, 508.0/512.0,
		328.0/512.0, 340.0/512.0,
		328.0/512.0, 508.0/512.0};
	m_tagMap.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150),texCoord_s01000);
	m_tagMap.SetScale((cx-14)/316.0f,(cy-14)/160.0f);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_NAVI));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crg);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

#endif

	return TRUE;
}

void CWidgetNaviCore::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			//m_tagMap.SetStatus(m_bOnFocus?BS_FOCUS:BS_UP);
			RETAILMSG(1, (TEXT("TEXT:[%s]: m_bOnFocus:%d\r\n"),TEXT(__FUNCTION__),m_bOnFocus));
#endif
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagMap.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
#endif
		}
		break;
	}
}
//Video widget:
CWidgetVideoCore::CWidgetVideoCore(void)
{

}
CWidgetVideoCore::~CWidgetVideoCore(void)
{

}

BOOL CWidgetVideoCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagVideo.IsReady();
}
void CWidgetVideoCore::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	if(!BeginRender())return;


	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);


	m_tagVideo.SetLuminance(m_fCurLuminance);
	m_tagVideo.SetAlpha(m_fCurAlpha);
	m_tagVideo.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();


	EndRender();

	ProcessNotify();

}
BOOL CWidgetVideoCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);


	m_tagVideo.Initialize(0/*IDC_VIDEOCORE_TAG_VIDEO*/,this,m_pSkinManager->GetTexture(TEXID_108),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagVideo.SetPos(0,10);
	m_tagVideo.SetShowScale(GLfloat(cx-30)/m_tagVideo.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_VIDEO));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crg);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;


	return TRUE;
}




//TV widget:
CWidgetTVCore::CWidgetTVCore(void)
{
	m_bPlugin = TRUE;
}
CWidgetTVCore::~CWidgetTVCore(void)
{

}

BOOL CWidgetTVCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagTV.IsReady();
}
void CWidgetTVCore::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagTV.SetAlpha(m_fCurAlpha);
	m_tagTV.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();
#else
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;

	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	m_tagTV.SetLuminance(m_fCurLuminance);
	m_tagTV.SetAlpha(m_fCurAlpha);
	m_tagTV.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();
#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetTVCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);

	#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ISDB)
	m_tagTV.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_171),NULL,
		m_pSkinManager->GetTexture(TEXID_171),NULL,
		m_pSkinManager->GetTexture(TEXID_172),NULL,
		m_pSkinManager->GetTexture(TEXID_170),NULL);
	#elif (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	m_tagTV.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_168),NULL,
		m_pSkinManager->GetTexture(TEXID_168),NULL,
		m_pSkinManager->GetTexture(TEXID_169),NULL,
		m_pSkinManager->GetTexture(TEXID_167),NULL);
	#else
	m_tagTV.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_162),NULL,
		m_pSkinManager->GetTexture(TEXID_162),NULL,
		m_pSkinManager->GetTexture(TEXID_163),NULL,
		m_pSkinManager->GetTexture(TEXID_161),NULL);
	#endif
	m_tagTV.SetScale(1.0f,1.0f*pix_ratio);
	m_tagTV.SetPos(0,21);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(CVTE_TV_WIDGET_NAME_STRING);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else
	#if (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ISDB)
	m_tagTV.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_190),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTV.SetPos(0,10);
	m_tagTV.SetShowScale(GLfloat(cx-30)/m_tagTV.GetSize().cx);
	#elif (CVTE_DEF_TV_WIDGET_TYPE == CVTE_DEF_TV_WIDGET_TYPE_ANDROID_WAZE)
	m_tagTV.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_189),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTV.SetPos(0,10);
	m_tagTV.SetShowScale(GLfloat(cx-30)/m_tagTV.GetSize().cx);
	#else
	m_tagTV.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_129),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTV.SetPos(0,10);
	m_tagTV.SetShowScale(GLfloat(cx-30)/m_tagTV.GetSize().cx);
	#endif

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(CVTE_TV_WIDGET_NAME_STRING);   

	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#endif

	return TRUE;
}

void CWidgetTVCore::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			if (m_bOnFocus && m_bPlugin)
			{
				m_tagTV.SetStatus(BS_FOCUS);
			}
			else
			{
				m_tagTV.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
			}
			//RETAILMSG(1, (TEXT("TEXT:[%s]: m_bOnFocus:%d\r\n"),TEXT(__FUNCTION__),m_bOnFocus));			
#endif
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagTV.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
#endif
		}
		break;
	default:
		break;
	}
}

//Disk widget:
CWidgetDiskCore::CWidgetDiskCore(void)
{

}
CWidgetDiskCore::~CWidgetDiskCore(void)
{

}

BOOL CWidgetDiskCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagDisk.IsReady();
}
void CWidgetDiskCore::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};


	if(!BeginRender())return;

	 DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);


	m_tagDisk.SetLuminance(m_fCurLuminance);
	m_tagDisk.SetAlpha(m_fCurAlpha);
	m_tagDisk.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();


	EndRender();

	ProcessNotify();

}
BOOL CWidgetDiskCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_tagDisk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_120),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagDisk.SetPos(0,10);
	m_tagDisk.SetShowScale(GLfloat(cx-30)/m_tagDisk.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};

	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_DISK));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;


	return TRUE;
}


//Setting widget:
CWidgetSettingCore::CWidgetSettingCore(void)
{

}
CWidgetSettingCore::~CWidgetSettingCore(void)
{

}

BOOL CWidgetSettingCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagSetting.IsReady();
}
void CWidgetSettingCore::Render()
{

	if(!BeginRender())return;
	
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	m_tagSetting.SetLuminance(m_fCurLuminance);
	m_tagSetting.SetAlpha(m_fCurAlpha);
	m_tagSetting.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_170):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	m_tagSetting.SetLuminance(m_fCurLuminance);
	m_tagSetting.SetAlpha(m_fCurAlpha);
	m_tagSetting.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus(m_bOnFocus?BS_FOCUS:BS_UP);
	}
	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagSetting.SetAlpha(m_fCurAlpha);
	m_tagSetting.Render();


	m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	m_tagTitle.Render();
#else

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.7f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	m_tagSetting.SetLuminance(m_fCurLuminance);
	m_tagSetting.SetAlpha(m_fCurAlpha);
	m_tagSetting.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetSettingCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE ==CVTE_UI_STYLE_FIATUI)
	GLfloat texCoord_s01000[]={
		132.0/256.0, 4.0/256.0,
		132.0/256.0, 124.0/256.0,
		252.0/256.0, 4.0/256.0,
		252.0/256.0, 124.0/256.0};

	m_tagSetting.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_156),texCoord_s01000);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_SETTINGS));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_GERMAN)
		m_tagTitle.SetCaptionHeight(15);
	else
		m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE ==CVTE_UI_STYLE_BOSSAUDIOUI)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_tagSetting.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_117),NULL);
	m_tagSetting.SetScale(0.8f,0.8f*pix_ratio);
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	//m_tagbk.SetStatus(BS_DISABLED);

	m_tagSetting.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_129),NULL,
		m_pSkinManager->GetTexture(TEXID_129),NULL,
		m_pSkinManager->GetTexture(TEXID_130),NULL,
		m_pSkinManager->GetTexture(TEXID_128),NULL);
	m_tagSetting.SetPos(0,21);
	m_tagSetting.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_SETTINGS));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else
	m_tagSetting.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_117),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagSetting.SetPos(0,10);
	m_tagSetting.SetShowScale(GLfloat(cx-30)/m_tagSetting.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_SETTINGS));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_GERMAN:
		m_tagTitle.SetCaptionHeight(14);
		break;
	case LANGUAGE_RUSSIAN:
		m_tagTitle.SetCaptionHeight(16);
		break;
	default:
		m_tagTitle.SetCaptionHeight(18);
		break;
	}
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#endif

	return TRUE;
}

void CWidgetSettingCore::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagSetting.SetStatus(m_bOnFocus?BS_FOCUS:BS_UP);
#endif
			RETAILMSG(1, (TEXT("TEXT:[%s]: m_bOnFocus:%d\r\n"),TEXT(__FUNCTION__),m_bOnFocus));
		}
		break;
	}
}

//DSP widget:
CWidgetDSP::CWidgetDSP(void)
{

}
CWidgetDSP::~CWidgetDSP(void)
{

}

BOOL CWidgetDSP::IsReady()
{
	return CWidgetGL::IsReady() && m_tagSetting.IsReady();
}
void CWidgetDSP::Render()
{

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	m_tagSetting.SetLuminance(m_fCurLuminance);
	m_tagSetting.SetAlpha(m_fCurAlpha);
	m_tagSetting.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_170):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	m_tagSetting.SetLuminance(m_fCurLuminance);
	m_tagSetting.SetAlpha(m_fCurAlpha);
	m_tagSetting.Render();

#else

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.7f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	m_tagSetting.SetLuminance(m_fCurLuminance);
	m_tagSetting.SetAlpha(m_fCurAlpha);
	m_tagSetting.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetDSP::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE ==CVTE_UI_STYLE_FIATUI)
	GLfloat texCoord_s01000[]={
		132.0/256.0, 4.0/256.0,
		132.0/256.0, 124.0/256.0,
		252.0/256.0, 4.0/256.0,
		252.0/256.0, 124.0/256.0};

		m_tagSetting.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_156),texCoord_s01000);

		SIZE siInfo={m_siCurSize.cx,20};
		COLORGL crw={1.0,1.0,1.0,1.0};

		m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagTitle.SetSize(&siInfo);
		m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_SETTINGS));
		m_tagTitle.LinkTextGL(pTextGL);
		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
		if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_GERMAN)
			m_tagTitle.SetCaptionHeight(15);
		else
			m_tagTitle.SetCaptionHeight(18);
		m_tagTitle.EnableDrawBack(FALSE);
		m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagTitle.SetCaptionColor(&crw);
		m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE ==CVTE_UI_STYLE_BOSSAUDIOUI)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_tagSetting.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_120),NULL);
	m_tagSetting.SetScale(0.8f,0.8f*pix_ratio);

#else
	m_tagSetting.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_117),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagSetting.SetPos(0,10);
	m_tagSetting.SetShowScale(GLfloat(cx-30)/m_tagSetting.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_SETTINGS));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_GERMAN)
		m_tagTitle.SetCaptionHeight(15);
	else
		m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#endif

	return TRUE;
}




//AUX widget:
CWidgetAUXCore::CWidgetAUXCore(void)
{
	m_bPlugin = TRUE;
	m_bOnFocus = FALSE;
	m_bPressed = FALSE;
}
CWidgetAUXCore::~CWidgetAUXCore(void)
{

}

BOOL CWidgetAUXCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagAUX.IsReady();
}
void CWidgetAUXCore::Render()
{
	if(!BeginRender())return;

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	m_tagAUX.SetLuminance(m_fCurLuminance);
	m_tagAUX.SetAlpha(m_fCurAlpha);
	m_tagAUX.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#elif  (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_171):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha/**0.7f*/};

	if(m_bPressed || m_bOnFocus)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	m_tagAUX.SetLuminance(m_fCurLuminance);
	m_tagAUX.SetAlpha(m_fCurAlpha);
	m_tagAUX.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagAUX.SetAlpha(m_fCurAlpha);
	m_tagAUX.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();
#else

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.7f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	m_tagAUX.SetLuminance(m_fCurLuminance);
	m_tagAUX.SetAlpha(m_fCurAlpha);
	m_tagAUX.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetAUXCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
	m_tagPlugin.Show(FALSE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI) 

	m_tagAUX.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_183),NULL);

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	GLfloat texCoord_s01000[]={
		132.0/256.0, 132.0/256.0,
		132.0/256.0, 252.0/256.0,
		252.0/256.0, 132.0/256.0,
		252.0/256.0, 252.0/256.0};

	m_tagAUX.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_157),texCoord_s01000);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_AUX));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagAUX.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_149),NULL,
		m_pSkinManager->GetTexture(TEXID_149),NULL,
		m_pSkinManager->GetTexture(TEXID_150),NULL,
		m_pSkinManager->GetTexture(TEXID_148),NULL);
	m_tagAUX.SetPos(0,21);
	m_tagAUX.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	m_tagAUX.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_AUX));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else

/*
	m_tagAUX.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_125),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
*/
	m_tagAUX.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_189),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);

	m_tagAUX.SetPos(0,10);
	m_tagAUX.SetShowScale(0.7f);    //GLfloat(cx-50)/m_tagAUX.GetSize().cx

	SIZE siInfo={m_siCurSize.cx-24,20};
	//COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
#if (CUSTOMER_ID == CUSTOMER_SOUNDEXPRESS || CUSTOMER_ID == CUSTOMER_SOUNDEXPRESS_BOA)
	m_tagTitle.SetCaption(_T("AUX Audio"));
#else
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_AUX));
#endif
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

#endif

	return TRUE;
}
void CWidgetAUXCore::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			if (m_bOnFocus && m_bPlugin)
			{
				m_tagAUX.SetStatus(BS_FOCUS);
			}
			else
			{
				m_tagAUX.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
			}
			//RETAILMSG(1, (TEXT("TEXT:[%s]: m_bOnFocus:%d\r\n"),TEXT(__FUNCTION__),m_bOnFocus));			
#endif
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin = *((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagAUX.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
			RETAILMSG(1, (TEXT("TEXT:[%s]: m_bPlugin:%d\r\n"),TEXT(__FUNCTION__),m_bPlugin));
#else
			m_tagPlugin.Show(m_bPlugin);
#endif
		}
		break;
	default:
		break;
	}
}

//Image widget:
CWidgetImageCore::CWidgetImageCore(void)
{
	m_bPressed = FALSE;
}
CWidgetImageCore::~CWidgetImageCore(void)
{

}

BOOL CWidgetImageCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagImage.IsReady();
}
void CWidgetImageCore::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	if(!BeginRender())return;


	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);


	m_tagImage.SetLuminance(m_fCurLuminance);
	m_tagImage.SetAlpha(m_fCurAlpha);
	m_tagImage.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();


	EndRender();

	ProcessNotify();

}
BOOL CWidgetImageCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_tagImage.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_109),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagImage.SetPos(0,10);
	m_tagImage.SetShowScale(GLfloat(cx-30)/m_tagImage.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_IMAGE));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crg);
	m_ptagTitle = &m_tagTitle;


	return TRUE;
}

//USB widget:
CWidgetUSB::CWidgetUSB(void)
{
	m_bPlugin = FALSE;
	m_bOnFocus = FALSE;
	m_bPressed = FALSE;
}
CWidgetUSB::~CWidgetUSB(void)
{

}

BOOL CWidgetUSB::IsReady()
{
	//RETAILMSG(1, (TEXT("MSG:CWidgetUSB::IsReady   [%d]--------\r\n"),CWidgetGL::IsReady() && m_tagUSB.IsReady()));
	return CWidgetGL::IsReady() && m_tagUSB.IsReady();
}
void CWidgetUSB::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	m_tagUSB.SetLuminance(m_fCurLuminance);
	m_tagUSB.SetAlpha(m_fCurAlpha);
	m_tagUSB.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#elif  (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_171):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha/**0.7f*/};

	if(m_bPressed || m_bOnFocus)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	m_tagUSB.SetLuminance(m_fCurLuminance);
	m_tagUSB.SetAlpha(m_fCurAlpha);
	m_tagUSB.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagUSB.SetAlpha(m_fCurAlpha);
	m_tagUSB.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();

#else

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	m_tagUSB.SetLuminance(m_fCurLuminance);
	m_tagUSB.SetAlpha(m_fCurAlpha);
	m_tagUSB.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetUSB::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableSendNotify = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_tagUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_182),NULL);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetScale(0.8f,0.8f*pix_ratio);
	m_tagPlugin.SetPos(cx/2-28,cy/2-28);
	m_tagPlugin.Show(FALSE);

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)

	GLfloat texCoord_s01000[]={
		132.0/256.0, 132.0/256.0,
		132.0/256.0, 252.0/256.0,
		252.0/256.0, 132.0/256.0,
		252.0/256.0, 252.0/256.0};

	m_tagUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_156),texCoord_s01000);
	m_tagUSB.SetScale(0.9f,0.9f);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
	m_tagPlugin.Show(FALSE);


	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_USB));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagUSB.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_132),NULL,
		m_pSkinManager->GetTexture(TEXID_132),NULL,
		m_pSkinManager->GetTexture(TEXID_133),NULL,
		m_pSkinManager->GetTexture(TEXID_131),NULL);
	m_tagUSB.SetPos(0,21);
	m_tagUSB.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	m_tagUSB.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_USB));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else

	m_tagUSB.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_123),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagUSB.SetPos(0,10);

	#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	{
		m_tagUSB.SetShowScale(GLfloat(cy-50)/m_tagUSB.GetSize().cy);
	}
	#else
	{
		m_tagUSB.SetShowScale(GLfloat(cx-50)/m_tagUSB.GetSize().cx);
	}
	#endif
	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	{
		m_tagPlugin.SetPos(cx/2-32,cy/2-32);
	}
	#else
	{
		m_tagPlugin.SetPos(20,-20);
	}
	#endif
	m_tagPlugin.Show(FALSE);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_USB));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#endif

	return TRUE;
}
void CWidgetUSB::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagUSB.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
			RETAILMSG(1, (TEXT("TEXT:[%s]: m_bPlugin:%d\r\n"),TEXT(__FUNCTION__),m_bPlugin));
#else
			m_tagPlugin.Show(m_bPlugin);
#endif
			m_bEnableSendNotify = m_bPlugin;
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}
}

//SD widget:
CWidgetSD::CWidgetSD(void)
{
	m_bPlugin = FALSE;
}
CWidgetSD::~CWidgetSD(void)
{

}

BOOL CWidgetSD::IsReady()
{
	return CWidgetGL::IsReady() && m_tagSD.IsReady();
}
void CWidgetSD::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

// 	COLORGL cr_up={0.0,0.0,0.8,falpha};
// 	COLORGL cr_down={0.0,0.0,1.0,falpha};

	if(!BeginRender())return;

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);

	m_tagSD.SetLuminance(m_fCurLuminance);
	m_tagSD.SetAlpha(m_fCurAlpha);
	m_tagSD.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();


	EndRender();

	ProcessNotify();

}
BOOL CWidgetSD::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableSendNotify = FALSE;

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetPos(20,-20);
	m_tagPlugin.Show(FALSE);

	m_tagSD.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_124),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagSD.SetPos(0,10);
	m_tagSD.SetShowScale(GLfloat(cx-50)/m_tagSD.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_SD));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;


	return TRUE;
}


void CWidgetSD::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
			m_tagPlugin.Show(m_bPlugin);

			m_bEnableSendNotify = m_bPlugin;
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}

}


//CWidgetiPod widget:
CWidgetiPod::CWidgetiPod(void)
{
	m_bPlugin = FALSE;
}
CWidgetiPod::~CWidgetiPod(void)
{

}

BOOL CWidgetiPod::IsReady()
{
	return CWidgetGL::IsReady() && m_tagiPod.IsReady();
}
void CWidgetiPod::Render()
{
// 	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_154):NULL;
// 
// 	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.95f};

	if(!BeginRender())return;

//		DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	m_tagiPod.SetLuminance(m_fCurLuminance);
	m_tagiPod.SetAlpha(m_fCurAlpha);
	m_tagiPod.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_175):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_174):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}
	m_tagiPod.SetLuminance(m_fCurLuminance);
	m_tagiPod.SetAlpha(m_fCurAlpha);
	m_tagiPod.Render();

	DrawRect(pTexBar,0,-m_siCurSize.cy/2+30,m_siCurSize.cx-14-8,46-8,8,&cr,TRUE);
	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);;
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagiPod.SetAlpha(m_fCurAlpha);
	m_tagiPod.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();
#else

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_159):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.8f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-24,m_siCurSize.cy-24,8,&cr,TRUE);

	m_tagiPod.SetLuminance(m_fCurLuminance);
	m_tagiPod.SetAlpha(m_fCurAlpha);
	m_tagiPod.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetiPod::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableSendNotify = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	GLfloat texCoord_s01000[]={
		132.0/256.0, 132.0/256.0,
		132.0/256.0, 252.0/256.0,
		252.0/256.0, 132.0/256.0,
		252.0/256.0, 252.0/256.0};

	m_tagiPod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_153),texCoord_s01000);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_IPOD));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;


	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
	m_tagPlugin.Show(FALSE);

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_tagiPod.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_162),NULL);
	m_tagiPod.SetPos(0,23);
	m_tagiPod.SetScale(1.2f,1.2f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("iPod"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+30);
	m_tagTitle.SetCaptionHeight(22);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagiPod.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_152),NULL,
		m_pSkinManager->GetTexture(TEXID_152),NULL,
		m_pSkinManager->GetTexture(TEXID_153),NULL,
		m_pSkinManager->GetTexture(TEXID_151),NULL);
	m_tagiPod.SetPos(0,21);
	//m_tagiPod.Show(FALSE);
	m_tagiPod.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	m_tagiPod.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("iPod"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#else
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	GLfloat scale=(m_siCurSize.cy-36)/128.0f;
	if(scale>1.3f)
		scale = 1.3f;
	m_tagiPod.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_153),NULL);
	m_tagiPod.SetPos(0,/*23*/10);
	m_tagiPod.SetScale(scale/pix_ratio,scale);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("iPod"));
	m_tagTitle.LinkTextGL(pTextGL);
	if(cy > CVTE_DEF_HOME_WIDGET_UNIT_CY)
	{
		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+30);
		m_tagTitle.SetCaptionHeight(22);
	}
	else
	{
		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
		m_tagTitle.SetCaptionHeight(18);
	}
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crg);
	m_ptagTitle = &m_tagTitle;


	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	//m_tagPlugin.SetPos(cx/2-28,-cy/2+32);
#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	{
		m_tagPlugin.SetPos(cx/2-32,cy/2-32);
	}
#else
	{
		m_tagPlugin.SetPos(cx/2-28,-cy/2+32);
	}
#endif
	m_tagPlugin.Show(FALSE);
#endif
// 	GLfloat texCoord_s01000[]={
// 		4.0/512.0, 172.0/512.0,
// 		4.0/512.0, 340.0/512.0,
// 		328.0/512.0, 172.0/512.0,
// 		328.0/512.0, 340.0/512.0};
//
// 	m_tagiPod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150),texCoord_s01000);
// 	m_tagiPod.SetScale((cx-14)/316.0f,(cy-14)/160.0f);

//		m_tagiPod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_153),NULL);
//		m_tagiPod.SetPos(cx/2-100,0);
//		m_tagiPod.SetShowScale(1.0f);

//		SIZE siInfo={m_siCurSize.cx-24,20};
//		COLORGL crg={1.0,1.0,1.0,1.0};
//		m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
//		m_tagTitle.SetSize(&siInfo);
//		m_tagTitle.SetCaption(_T("iPod"));
//		m_tagTitle.LinkTextGL(pTextGL);
//		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
//		m_tagTitle.SetCaptionHeight(18);
//		m_tagTitle.EnableDrawBack(FALSE);
//		//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
//		m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
//		m_tagTitle.SetCaptionColor(&crg);


	return TRUE;
}
void CWidgetiPod::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	RETAILMSG(1, (TEXT("MSG:iPod Plugin idOperation [%d] lpara[%d] m_bPlugin[%d]\r\n"),idOperation,*(BOOL*)lpara, m_bPlugin));
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			if (m_bOnFocus && m_bPlugin)
			{
				m_tagiPod.SetStatus(BS_FOCUS);
			}
			else
			{
				m_tagiPod.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
			}
			RETAILMSG(1, (TEXT("TEXT:[%s]: m_bOnFocus:%d\r\n"),TEXT(__FUNCTION__),m_bOnFocus));
#endif
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagiPod.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
#else
			m_tagPlugin.Show(m_bPlugin);
#endif
			m_bEnableSendNotify = m_bPlugin;
			RETAILMSG(1, (TEXT("MSG:iPod Plugin ----------SHOW[%d]\r\n"),m_bPlugin));
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
			RETAILMSG(1, (TEXT("MSG:iPod Plugin ----------ENABLE[%d]\r\n"),bEnbale));
		}
		break;
	}

}

//CWidgetCarPlay widget:
CWidgetCarPlay::CWidgetCarPlay(void)
{
	m_bPlugin = FALSE;
	m_bOnFocus = FALSE;
	m_bPressed = FALSE;
}
CWidgetCarPlay::~CWidgetCarPlay(void)
{

}

BOOL CWidgetCarPlay::IsReady()
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	return CWidgetGL::IsReady() && m_tagCarPlay.IsReady();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	return CWidgetGL::IsReady();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	return CWidgetGL::IsReady();
#else
	return CWidgetGL::IsReady() && m_tagCarPlayActive.IsReady();
#endif
}
void CWidgetCarPlay::Render()
{
	// 	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	if(!BeginRender())return;

	// 	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	m_tagCarPlay.SetLuminance(m_fCurLuminance);
	m_tagCarPlay.SetAlpha(m_fCurAlpha);
	m_tagCarPlay.Render();

	m_tagCarPlayPlugin.SetLuminance(m_fCurLuminance);
	m_tagCarPlayPlugin.SetAlpha(m_fCurAlpha);
	m_tagCarPlayPlugin.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_175):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_174):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}
	if(m_bPlugin)
	{
		m_tagCarPlayActive.SetLuminance(m_fCurLuminance);
		m_tagCarPlayActive.SetAlpha(m_fCurAlpha);
		m_tagCarPlayActive.Render();

		DrawRect(pTexBar,0,-m_siCurSize.cy/2+30,m_siCurSize.cx-14-8,46-8,8,&cr,TRUE);
		m_tagTitle.SetLuminance(m_fCurLuminance);
		m_tagTitle.SetAlpha(m_fCurAlpha);
		m_tagTitle.Render();
	}
	else
	{
		m_tagCarPlayDisable.SetLuminance(m_fCurLuminance);
		m_tagCarPlayDisable.SetAlpha(m_fCurAlpha);
		m_tagCarPlayDisable.Render();

		m_tagTitle.SetLuminance(m_fCurLuminance);
		m_tagTitle.SetAlpha(m_fCurAlpha);
		m_tagTitle.Render();
		DrawRect(pTexBar,0,-m_siCurSize.cy/2+30,m_siCurSize.cx-14-8,46-8,8,&cr,TRUE);
	}
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);;
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagCarPlay.SetAlpha(m_fCurAlpha);
	m_tagCarPlay.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();

#else
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_159):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.8f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-24,m_siCurSize.cy-24,8,&cr,TRUE);

	m_tagCarPlayActive.SetLuminance(m_fCurLuminance);
	m_tagCarPlayActive.SetAlpha(m_fCurAlpha);
	m_tagCarPlayActive.Render();

	m_tagCarPlayDisable.SetLuminance(m_fCurLuminance);
	m_tagCarPlayDisable.SetAlpha(m_fCurAlpha*0.8f);
	m_tagCarPlayDisable.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();


#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetCarPlay::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableSendNotify = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	GLfloat texCoord_s01000[]={
		4.0/256.0, 132.0/256.0,
		4.0/512.0, 252.0/256.0,
		124.0/256.0, 132.0/256.0,
		124.0/256.0, 252.0/256.0};

	m_tagCarPlay.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_154),texCoord_s01000);
	m_tagCarPlayPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_156),texCoord_s01000);
	m_tagCarPlayPlugin.Show(FALSE);
	

	//m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	//m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
	//m_tagPlugin.Show(FALSE);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("CarPlay"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_tagCarPlayActive.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_161),NULL);
	m_tagCarPlayActive.SetPos(0,23);
	m_tagCarPlayActive.SetScale(1.2f,1.2f*pix_ratio);

	m_tagCarPlayDisable.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_160),NULL);
	m_tagCarPlayDisable.SetPos(0,23);
	m_tagCarPlayDisable.SetScale(1.2f,1.2f*pix_ratio);

// 	m_tagCarPlayActive.SetScale(GLfloat(cx)/m_tagCarPlayActive.GetSize().cx,GLfloat(cy)/m_tagCarPlayActive.GetSize().cy);    
// 	m_tagCarPlayDisable.SetScale(GLfloat(cx)/m_tagCarPlayDisable.GetSize().cx,GLfloat(cy)/m_tagCarPlayDisable.GetSize().cy);    

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("Apple CarPlay"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+30);
	m_tagTitle.SetCaptionHeight(22);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagCarPlay.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_114),NULL,
		m_pSkinManager->GetTexture(TEXID_114),NULL,
		m_pSkinManager->GetTexture(TEXID_115),NULL,
		m_pSkinManager->GetTexture(TEXID_113),NULL);
	m_tagCarPlay.SetPos(0,21);
	m_tagCarPlay.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	m_tagCarPlay.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("Apple CarPlay"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(16);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	GLfloat scale=(m_siCurSize.cy-50)/128.0f;
	if(scale>1.2f)
		scale = 1.2f;
	m_tagCarPlayActive.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_156),NULL);
	m_tagCarPlayActive.SetPos(0,/*23*/10);
	m_tagCarPlayActive.SetScale(scale/pix_ratio,scale);
	m_tagCarPlayActive.Show(FALSE);

	m_tagCarPlayDisable.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_157),NULL);
	m_tagCarPlayDisable.SetPos(0,/*23*/10);
	m_tagCarPlayDisable.SetScale(scale/pix_ratio,scale);
	m_tagCarPlayDisable.Show(TRUE);

	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crg={0.1,0.1,0.1,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("Apple CarPlay"));
	m_tagTitle.LinkTextGL(pTextGL);
	if(cy > CVTE_DEF_HOME_WIDGET_UNIT_CY)
	{
		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+30);
		m_tagTitle.SetCaptionHeight(22);
	}
	else
	{
		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
		m_tagTitle.SetCaptionHeight(18);
	}
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crg);
	m_ptagTitle = &m_tagTitle;

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	{
		m_tagPlugin.SetPos(cx/2-32,cy/2-32);
	}
#else
	{
		m_tagPlugin.SetPos(cx/2-28,-cy/2+32);
	}
#endif
	m_tagPlugin.Show(FALSE);



#endif

		return TRUE;
}
void CWidgetCarPlay::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			if (m_bOnFocus && m_bPlugin)
			{
				m_tagCarPlay.SetStatus(BS_FOCUS);
			}
			else
			{
				m_tagCarPlay.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
			}
			RETAILMSG(1, (TEXT("TEXT:[%s]: m_bOnFocus:%d\r\n"),TEXT(__FUNCTION__),m_bOnFocus));
#endif
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
			
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)	
			m_tagCarPlay.Show(!m_bPlugin);
			m_tagCarPlayPlugin.Show(m_bPlugin);
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagCarPlay.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
#else
			m_tagPlugin.Show(m_bPlugin);
			m_tagCarPlayActive.Show(m_bPlugin);
			m_tagCarPlayDisable.Show(!m_bPlugin);
#endif			
			m_bEnableSendNotify = m_bPlugin;
			//RETAILMSG(1, (TEXT("MSG:Carplay Plugin Show[%d]\r\n"),m_bPlugin));
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}

}



//CWidgetVehicleInfo widget:
CWidgetVehicleInfo::CWidgetVehicleInfo(void)
{
	m_bPressed = FALSE;
}
CWidgetVehicleInfo::~CWidgetVehicleInfo(void)
{

}

BOOL CWidgetVehicleInfo::IsReady()
{
	return CWidgetGL::IsReady() && m_tagVehicle.IsReady();
}
void CWidgetVehicleInfo::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus(BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagVehicle.SetAlpha(m_fCurAlpha);
	m_tagVehicle.Render();

	m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	m_tagTitle.Render();
#else
	// 	DrawRect(pTexWidget,0,0,300-32,300-32,16,&cr,TRUE);
	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);

	m_tagVehicle.SetLuminance(m_fCurLuminance);
	m_tagVehicle.SetAlpha(m_fCurAlpha);
	m_tagVehicle.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#endif
	EndRender();

	ProcessNotify();

}
BOOL CWidgetVehicleInfo::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	//m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagVehicle.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_111),NULL,
		m_pSkinManager->GetTexture(TEXID_111),NULL,
		m_pSkinManager->GetTexture(TEXID_112),NULL,
		m_pSkinManager->GetTexture(TEXID_110),NULL);
	m_tagVehicle.SetPos(0,21);
	//m_tagVehicle.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_VEHICLEINFO));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else
	m_tagVehicle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_126),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagVehicle.SetPos(0,10);
	m_tagVehicle.SetShowScale(GLfloat(cx-40)/m_tagVehicle.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_VEHICLEINFO));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#endif

	return TRUE;
}



//CWidgetDateTime
CWidgetDateTime::CWidgetDateTime(void)
{

}
CWidgetDateTime::~CWidgetDateTime(void)
{

}

BOOL CWidgetDateTime::IsReady()
{
	return CWidgetGL::IsReady() ;

}
void CWidgetDateTime::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;

	static DWORD lastTick=0;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	if(!BeginRender())return;



	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	m_clock.SetAlpha(m_fCurAlpha);
	m_clock.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	//draw date:

	if(GetTickCount()-lastTick>5000)
	{
		wchar_t str[16];
		SYSTEMTIME CurTime;
		GetLocalTime(&CurTime);
		swprintf_s(str,16,_T("%02d"),CurTime.wDay);
		m_tagDay.SetCaption(str);
// 		swprintf_s(str,16,_T("%02d"),CurTime.wMonth);
		m_tagMonth.SetCaption(GetResourceString(IDS_JANUARY+CurTime.wMonth-1));

		m_tagWeek.SetCaption(GetResourceString(IDS_SUNDAY+CurTime.wDayOfWeek));

		lastTick = GetTickCount();
	}

	m_tagDay.SetAlpha(m_fCurAlpha);
	m_tagMonth.SetAlpha(m_fCurAlpha);
	m_tagWeek.SetAlpha(m_fCurAlpha);

	m_tagDay.Render();
	m_tagMonth.Render();
	m_tagWeek.Render();


	EndRender();

	ProcessNotify();

}
BOOL CWidgetDateTime::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	CLOCKBARSTRUCTURE stru;
	memset(&stru,sizeof(stru),0);
	stru.idTexBase=TEXID(2001);
	stru.idTexHour=TEXID(2002);
	stru.idTexMinute=TEXID(2003);
	stru.idTexSecond=TEXID(2004);
	stru.bDashboardOnly=TRUE;

	m_clock.Initialize(0,NULL,&stru,pSkinManager,pTextGL);
	m_clock.SetPos(58,0);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crr={1.0,0.0,0.0,1.0};

	m_tagTitle.Initialize(0,NULL,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_TIME));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crg);
	m_ptagTitle = &m_tagTitle;


	m_tagDay.Initialize(0,NULL,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDay.SetSize(&siInfo);
	m_tagDay.SetCaption(_T("18"));
	m_tagDay.LinkTextGL(pTextGL);
	m_tagDay.SetPos(-50,0);
	m_tagDay.SetCaptionHeight(50);
	m_tagDay.EnableDrawBack(FALSE);
// 	m_tagDay.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagDay.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagDay.SetCaptionColor(&crr);
	//m_tagDay.SetCaptionCharStyle(STYLE_ART);

	m_tagMonth.Initialize(0,NULL,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagMonth.SetSize(&siInfo);
	m_tagMonth.SetCaption(_T("Oct."));
	m_tagMonth.LinkTextGL(pTextGL);
	m_tagMonth.SetPos(-50,46);
	m_tagMonth.SetCaptionHeight(20);
	m_tagMonth.EnableDrawBack(FALSE);
// 	m_tagMonth.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagMonth.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagMonth.SetCaptionColor(&crg);

	m_tagWeek.Initialize(0,NULL,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagWeek.SetSize(&siInfo);
	m_tagWeek.SetCaption(_T("Oct."));
	m_tagWeek.LinkTextGL(pTextGL);
	m_tagWeek.SetPos(-50,-46);
	m_tagWeek.SetCaptionHeight(20);
	m_tagWeek.EnableDrawBack(FALSE);
// 	m_tagWeek.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagWeek.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagWeek.SetCaptionColor(&crg);

	return TRUE;
}


//CWidgetMHL
CWidgetMHL::CWidgetMHL(void)
{

}
CWidgetMHL::~CWidgetMHL(void)
{

}

BOOL CWidgetMHL::IsReady()
{
	return CWidgetGL::IsReady() && m_tagMHL.IsReady();

}
void CWidgetMHL::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;

	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	if(!BeginRender())return;

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);
// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);


	m_tagMHL.SetLuminance(m_fCurLuminance);
	m_tagMHL.SetAlpha(m_fCurAlpha);
	m_tagMHL.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();



	EndRender();

	ProcessNotify();

}
BOOL CWidgetMHL::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_tagMHL.Initialize(IDC_MHL_TAG_MHL,this,m_pSkinManager->GetTexture(TEXID_127),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagMHL.SetPos(0,10);
	m_tagMHL.SetShowScale(GLfloat(cx-30)/m_tagMHL.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};

	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(IDC_MHL_TAG_TITLE,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_MHL));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;


	return TRUE;
}



//CWidgetPhoneLink
CWidgetPhoneLink::CWidgetPhoneLink(void)
{
	m_bPlugin = FALSE;

}
CWidgetPhoneLink::~CWidgetPhoneLink(void)
{

}

BOOL CWidgetPhoneLink::IsReady()
{
	return CWidgetGL::IsReady() && m_tagPhoneLink.IsReady();

}
void CWidgetPhoneLink::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	if(!BeginRender())return;

	m_tagPhoneLink.SetLuminance(m_fCurLuminance);
	m_tagPhoneLink.SetAlpha(m_fCurAlpha*0.9f);
	m_tagPhoneLink.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();


	EndRender();

	ProcessNotify();

}
BOOL CWidgetPhoneLink::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	//m_bEnableSendNotify = FALSE;

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetPos(cx/2-30,-cy/2+30);
	m_tagPlugin.Show(FALSE);

	GLfloat texCoord_s01000[]={
		4.0/512.0, 4.0/512.0,
		4.0/512.0, 172.0/512.0,
		328.0/512.0, 4.0/512.0,
		328.0/512.0, 172.0/512.0};

		m_tagPhoneLink.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150),texCoord_s01000);
		m_tagPhoneLink.SetScale((cx-14)/316.0f,(cy-14)/160.0f);

		COLORGL crg={0.1,0.1,0.1,1.0};

	SIZE siInfo={m_siCurSize.cx-24,20};
	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_PHONELINK));
// 	m_tagTitle.SetCaption(_T("Android Auto"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crg);
	m_ptagTitle = &m_tagTitle;

	return TRUE;
}

void CWidgetPhoneLink::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
			m_tagPlugin.Show(m_bPlugin);

			//m_bEnableSendNotify = m_bPlugin;
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}

}



//CWidgetEasyConnected
CWidgetEasyConnected::CWidgetEasyConnected(void)
{
	m_bPlugin = FALSE;
	m_bOnFocus = FALSE;
	m_bPressed = FALSE;
}
CWidgetEasyConnected::~CWidgetEasyConnected(void)
{

}

BOOL CWidgetEasyConnected::IsReady()
{
	return CWidgetGL::IsReady() && m_tagEasyConnected.IsReady();

}
void CWidgetEasyConnected::Render()
{
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.9f};

	if(!BeginRender())return;

	// 	DrawRect(pTexWidget,0,0,300-32,300-32,16,&cr,TRUE);
	// 	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);
	// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);
	}
	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagMirrorLink.SetAlpha(m_fCurAlpha);
	m_tagMirrorLink.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();

#else 

	m_tagEasyConnected.SetLuminance(m_fCurLuminance);
	m_tagEasyConnected.SetAlpha(m_fCurAlpha*0.9f);
	m_tagEasyConnected.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetEasyConnected::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableSendNotify = FALSE;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	GLfloat texCoord_s01000[]={
		4.0/256.0, 4.0/256.0,
		4.0/256.0, 124.0/256.0,
		124.0/256.0, 4.0/256.0,
		124.0/256.0, 124.0/256.0};

	m_tagEasyConnected.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_157),texCoord_s01000);
	//m_tagEasyConnected.SetScale((cx-14)/316.0f,(cy-14)/160.0f);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	// 	m_tagPlugin.SetPos(120,-44);
	m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
	m_tagPlugin.Show(FALSE);


	SIZE siInfo={m_siCurSize.cx,20};
	COLORGL crw={1.0,1.0,1.0,1.0};
	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_PHONELINK));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	//widget Initialize£º background
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	
	//widget Initialize£º icon
	m_tagMirrorLink.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_120),NULL,
		m_pSkinManager->GetTexture(TEXID_120),NULL,
		m_pSkinManager->GetTexture(TEXID_121),NULL,
		m_pSkinManager->GetTexture(TEXID_119),NULL);
	m_tagMirrorLink.SetPos(0,21);
	m_tagMirrorLink.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	//widget Initialize£º Text
	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_MIRRORLINK));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

#else
	GLfloat texCoord_s01000[]={
		4.0/512.0, 4.0/512.0,
		4.0/512.0, 172.0/512.0,
		328.0/512.0, 4.0/512.0,
		328.0/512.0, 172.0/512.0};

	m_tagEasyConnected.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150),texCoord_s01000);
	m_tagEasyConnected.SetScale((cx-14)/316.0f,(cy-14)/160.0f);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	// 	m_tagPlugin.SetPos(120,-44);
	m_tagPlugin.SetPos(cx/2-30,-cy/2+30);
	m_tagPlugin.Show(FALSE);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(GetResourceString(IDS_TITLE_PHONELINK));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crg);
	m_ptagTitle = &m_tagTitle;

#endif
	return TRUE;
}

void CWidgetEasyConnected::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	RETAILMSG(1, (TEXT("APP:MSG:GUI:Home: [%s] idControl[%d] idOperation[%d] *lpara[%d]\r\n"),TEXT(__FUNCTION__), idControl,idOperation,*((BOOL*)lpara)));
	switch (idOperation)
	{
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
			m_tagPlugin.Show(m_bPlugin);

			m_bEnableSendNotify = m_bPlugin;
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}
}

//CWidgetWeblink
CWidgetWeblink::CWidgetWeblink(void)
{

}
CWidgetWeblink::~CWidgetWeblink(void)
{

}

BOOL CWidgetWeblink::IsReady()
{
	return CWidgetGL::IsReady() && m_tagWeblink.IsReady();

}
void CWidgetWeblink::Render()
{
	if(!BeginRender())return;

	// 	DrawRect(pTexWidget,0,0,300-32,300-32,16,&cr,TRUE);
	// 	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_148):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*1.0f};
	DrawRect(pTexUp,0,0,m_siCurSize.cx-24,m_siCurSize.cy-24,8,&cr,TRUE);

	m_tagWeblink.SetLuminance(m_fCurLuminance);
	m_tagWeblink.SetAlpha(m_fCurAlpha*0.9f);
	m_tagWeblink.Render();


	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

	EndRender();

	ProcessNotify();

}
BOOL CWidgetWeblink::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableSendNotify = FALSE;

	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
#if CVTE_EN_COMBINE_ANDROID_LINK
	m_tagWeblink.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_149),NULL);
#else
	m_tagWeblink.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_128),NULL);
#endif
	m_tagWeblink.SetScale(0.6f,0.6f*pix_ratio);
	m_tagWeblink.SetPos(0,10);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	// 	m_tagPlugin.SetPos(120,-44);
	m_tagPlugin.SetPos(cx/2-25,-cy/2+35);
	m_tagPlugin.Show(FALSE);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("WebLink"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_GERMAN)
		m_tagTitle.SetCaptionHeight(15);
	else
		m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;

	return TRUE;
}

void CWidgetWeblink::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	RETAILMSG(1, (TEXT("APP:MSG:GUI:Home: [%s] idControl[%d] idOperation[%d] *lpara[%d]\r\n"),TEXT(__FUNCTION__), idControl,idOperation,*((BOOL*)lpara)));
	switch (idOperation)
	{
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
			m_tagPlugin.Show(m_bPlugin);

			m_bEnableSendNotify = m_bPlugin;
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}
}

//CWidgetAndroidAuto
CWidgetAndroidAuto::CWidgetAndroidAuto(void)
{
	m_bPlugin = FALSE;
	m_bOnFocus = FALSE;
	m_bPressed = FALSE;
}
CWidgetAndroidAuto::~CWidgetAndroidAuto(void)
{

}

BOOL CWidgetAndroidAuto::IsReady()
{
	return CWidgetGL::IsReady();

}
void CWidgetAndroidAuto::Render()
{
	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
	if(m_bPlugin)
	{
		m_tagAndroidAuto.SetLuminance(m_fCurLuminance);
		m_tagAndroidAuto.SetAlpha(m_fCurAlpha*0.9f);
		m_tagAndroidAuto.Render();
	}
	else
	{
		m_tagAndroidAutoDisable.SetLuminance(m_fCurLuminance);
		m_tagAndroidAutoDisable.SetAlpha(m_fCurAlpha*0.9f);
		m_tagAndroidAutoDisable.Render();
	}

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_175):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_174):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	if(m_bPlugin)
	{
		m_tagAndroidAuto.SetLuminance(m_fCurLuminance);
		m_tagAndroidAuto.SetAlpha(m_fCurAlpha);
		m_tagAndroidAuto.Render();

		DrawRect(pTexBar,0,-m_siCurSize.cy/2+30,m_siCurSize.cx-14-8,46-8,8,&cr,TRUE);
		m_tagTitle.SetLuminance(m_fCurLuminance);
		m_tagTitle.SetAlpha(m_fCurAlpha);
		m_tagTitle.Render();
	}
	else
	{
		m_tagAndroidAutoDisable.SetLuminance(m_fCurLuminance);
		m_tagAndroidAutoDisable.SetAlpha(m_fCurAlpha);
		m_tagAndroidAutoDisable.Render();

		m_tagTitle.SetLuminance(m_fCurLuminance);
		m_tagTitle.SetAlpha(m_fCurAlpha);
		m_tagTitle.Render();
		DrawRect(pTexBar,0,-m_siCurSize.cy/2+30,m_siCurSize.cx-14-8,46-8,8,&cr,TRUE);
	}
	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus((m_bOnFocus&&m_bPlugin)?BS_FOCUS:BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagAndroidAuto.SetAlpha(m_fCurAlpha);
	m_tagAndroidAuto.Render();

	if (m_bPlugin)
	{
		m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	}
	else
	{
		m_tagTitle.SetAlpha(m_fCurAlpha*0.2f);
	}
	m_tagTitle.Render();
#else
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_159):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.95f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-24,m_siCurSize.cy-24,8,&cr,TRUE);

	m_tagAndroidAuto.SetLuminance(m_fCurLuminance);
	m_tagAndroidAuto.SetAlpha(m_fCurAlpha);
	m_tagAndroidAuto.Render();

	m_tagAndroidAutoDisable.SetLuminance(m_fCurLuminance);
	m_tagAndroidAutoDisable.SetAlpha(m_fCurAlpha);
	m_tagAndroidAutoDisable.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();
#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetAndroidAuto::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
// 	GLfloat texCoord_s01000[]={
// 		4.0/256.0, 4.0/256.0,
// 		4.0/256.0, 124.0/256.0,
// 		124.0/256.0, 4.0/256.0,
// 		124.0/256.0, 124.0/256.0};
// 
// 		m_tagAndroidAuto.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_157),texCoord_s01000);
// 		//m_tagEasyConnected.SetScale((cx-14)/316.0f,(cy-14)/160.0f);
// 
// 		m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
// 		// 	m_tagPlugin.SetPos(120,-44);
// 		m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
// 		m_tagPlugin.Show(FALSE);

		m_tagAndroidAuto.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_187),NULL);
		m_tagAndroidAutoDisable.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_188),NULL);

		SIZE siInfo={m_siCurSize.cx,20};
		COLORGL crw={1.0,1.0,1.0,1.0};
		m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagTitle.SetSize(&siInfo);
		m_tagTitle.SetCaption(/*GetResourceString(IDS_TITLE_PHONELINK)*/_T("Android Auto"));
		m_tagTitle.LinkTextGL(pTextGL);
		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+18);
		m_tagTitle.SetCaptionHeight(18);
		m_tagTitle.EnableDrawBack(FALSE);
		//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
		m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagTitle.SetCaptionColor(&crw);
		m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

		m_tagAndroidAuto.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_187),NULL);
		m_tagAndroidAuto.SetScale(0.8f,0.8f*pix_ratio);
		m_tagAndroidAuto.SetPos(0,23);

		m_tagAndroidAutoDisable.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_188),NULL);
		m_tagAndroidAutoDisable.SetScale(0.8f,0.8f*pix_ratio);
		m_tagAndroidAutoDisable.SetPos(0,23);

		m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
		m_tagPlugin.SetScale(0.8f,0.8f*pix_ratio);
		m_tagPlugin.SetPos(cx/2-28,cy/2-28);
		m_tagPlugin.Show(FALSE);

		SIZE siInfo={m_siCurSize.cx,20};
		COLORGL crw={1.0,1.0,1.0,1.0};

		m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagTitle.SetSize(&siInfo);
		m_tagTitle.SetCaption(_T("Android Auto"));
		m_tagTitle.LinkTextGL(pTextGL);
		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+30);
		m_tagTitle.SetCaptionHeight(22);
		m_tagTitle.EnableDrawBack(FALSE);
		m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagTitle.SetCaptionColor(&crw);
		m_ptagTitle = &m_tagTitle;
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);
	m_tagbk.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);

	m_tagAndroidAuto.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_120),NULL,
		m_pSkinManager->GetTexture(TEXID_120),NULL,
		m_pSkinManager->GetTexture(TEXID_121),NULL,
		m_pSkinManager->GetTexture(TEXID_119),NULL);
	m_tagAndroidAuto.SetPos(0,21);
	m_tagAndroidAuto.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
	m_tagAndroidAuto.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("Android Auto"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(16);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else

// 	GLfloat texCoord_s01000[]={
// 		4.0/512.0, 4.0/512.0,
// 		4.0/512.0, 172.0/512.0,
// 		328.0/512.0, 4.0/512.0,
// 		328.0/512.0, 172.0/512.0};
// 
// 		m_tagAndroidAuto.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_155),texCoord_s01000);
// 		m_tagAndroidAuto.SetScale((cx-14)/316.0f,(cy-14)/160.0f);

		GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

		m_tagAndroidAuto.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_187),NULL);
		m_tagAndroidAuto.SetScale(0.6f,0.6f*pix_ratio);
		m_tagAndroidAuto.SetPos(0,10);
		m_tagAndroidAuto.Show(FALSE);

		m_tagAndroidAutoDisable.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_188),NULL);
		m_tagAndroidAutoDisable.SetScale(0.6f,0.6f*pix_ratio);
		m_tagAndroidAutoDisable.SetPos(0,10);
		m_tagAndroidAutoDisable.Show(TRUE);

		SIZE siInfo={m_siCurSize.cx,20};
		COLORGL crg={0.1,0.1,0.1,1.0};

		m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagTitle.SetSize(&siInfo);
		m_tagTitle.SetCaption(_T("Android Auto"));
		m_tagTitle.LinkTextGL(pTextGL);
		m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
		m_tagTitle.SetCaptionHeight(18);
		m_tagTitle.EnableDrawBack(FALSE);
		m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagTitle.SetCaptionColor(&crg);
		m_ptagTitle = &m_tagTitle;

		m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
		// 	m_tagPlugin.SetPos(120,-44);
		m_tagPlugin.SetPos(cx/2-30,-cy/2+30);
		m_tagPlugin.Show(FALSE);
#endif
		return TRUE;
}

void CWidgetAndroidAuto::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	RETAILMSG(1, (TEXT("APP:MSG:GUI:Home: [%s] idControl[%d] idOperation[%d] *lpara[%d]\r\n"),TEXT(__FUNCTION__), idControl,idOperation,*((BOOL*)lpara)));
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			if (m_bOnFocus && m_bPlugin)
			{
				m_tagAndroidAuto.SetStatus(BS_FOCUS);
			}
			else
			{
				m_tagAndroidAuto.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
			}
			RETAILMSG(1, (TEXT("TEXT:[%s]: m_bOnFocus:%d\r\n"),TEXT(__FUNCTION__),m_bOnFocus));
#endif
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin=*((BOOL*)lpara);
			m_tagPlugin.Show(m_bPlugin);

			m_bEnableSendNotify = m_bPlugin;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagAndroidAuto.SetStatus(m_bPlugin?BS_UP:BS_DISABLED);
#else
			m_tagAndroidAuto.Show(m_bPlugin);
			m_tagAndroidAutoDisable.Show(!m_bPlugin);
#endif
		}
		break;
	case UIOPRT_ENABLE:
		{
			BOOL bEnbale = *((BOOL*)lpara);
			//m_bEnableSendNotify = m_bPlugin&&bEnbale;
			Enable(bEnbale);
		}
		break;
	}
}

//Camera widget:
CWidgetCamera::CWidgetCamera(void)
{

}
CWidgetCamera::~CWidgetCamera(void)
{

}

BOOL CWidgetCamera::IsReady()
{
	return CWidgetGL::IsReady() && m_tagCamera.IsReady();
}
void CWidgetCamera::Render()
{

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_184):NULL;
	static CTexture *pTexDown=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_173):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(m_bPressed)
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
		DrawRect(pTexDown,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,FALSE);
	}
	else
	{
		DrawRect(pTexUp,0,0,m_siCurSize.cx-16,m_siCurSize.cy-16,8,&cr,TRUE);
	}

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if (m_bPressed)
	{
		m_tagbk.SetStatus(BS_DOWN);
	}
	else
	{
		m_tagbk.SetStatus(BS_UP);
	}

	m_tagbk.SetAlpha(m_fCurAlpha);
	m_tagbk.Render();

	m_tagCamera.SetAlpha(m_fCurAlpha);
	m_tagCamera.Render();

	m_tagTitle.SetAlpha(m_bOnFocus?m_fCurAlpha:(m_fCurAlpha*0.5f));
	m_tagTitle.Render();
#else
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);

	m_tagCamera.SetLuminance(m_fCurLuminance);
	m_tagCamera.SetAlpha(m_fCurAlpha);
	m_tagCamera.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

#endif

	EndRender();

	ProcessNotify();

}
BOOL CWidgetCamera::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

// 	m_tagCamera.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_184),NULL);
// 	m_tagCamera.SetScale(GLfloat(cx)/m_tagCamera.GetSize().cx,GLfloat(cy)/m_tagCamera.GetSize().cy);

	SIZE siInfo={m_siCurSize.cx-24,20};

	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("Camera"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+30);
	m_tagTitle.SetCaptionHeight(22);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);
	m_tagbk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_102),NULL,
		m_pSkinManager->GetTexture(TEXID_101),NULL,
		m_pSkinManager->GetTexture(TEXID_103),NULL,
		m_pSkinManager->GetTexture(TEXID_102),NULL);
	m_tagbk.SetPos(0,0);

	m_tagCamera.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_158),NULL,
		m_pSkinManager->GetTexture(TEXID_158),NULL,
		m_pSkinManager->GetTexture(TEXID_159),NULL,
		m_pSkinManager->GetTexture(TEXID_157),NULL);
	m_tagCamera.SetPos(0,21);
	m_tagCamera.SetScale(1.0f,1.0f*pix_ratio);

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(CVTE_CAMERA_WIDGET_NAME_STRING);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-42);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crw);
	m_ptagTitle = &m_tagTitle;
#else

	m_tagCamera.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_158),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagCamera.SetPos(0,10);
	m_tagCamera.SetShowScale(GLfloat(cx-30)/m_tagCamera.GetSize().cx);


	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};

	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(CVTE_CAMERA_WIDGET_NAME_STRING);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;

#endif

	

	return TRUE;
}



//DAB widget:
CWidgetDAB::CWidgetDAB(void)
{
}
CWidgetDAB::~CWidgetDAB(void)
{

}

BOOL CWidgetDAB::IsReady()
{
	return CWidgetGL::IsReady() && m_tagDAB.IsReady();
}
void CWidgetDAB::Render()
{
	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.7f};


	if(!BeginRender())return;

	//DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

// 	DrawRect(0,0,m_siCurSize.cx-6,m_siCurSize.cy-6,(m_bPressed||m_bNotifyFlag)?&cr_down:&cr_up);


	m_tagDAB.SetLuminance(m_fCurLuminance);
	m_tagDAB.SetAlpha(m_fCurAlpha);
	m_tagDAB.Render();

#if CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();


	/*
	m_tagFrequency.SetLuminance(m_fCurLuminance);
	m_tagFrequency.SetAlpha(m_fCurAlpha);
	m_tagFrequency.Render();
	*/
#endif

	EndRender();

	ProcessNotify();

}

BOOL CWidgetDAB::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CUSTOMER_ID == CUSTOMER_ALDINET)
	m_tagDAB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_463),NULL);
	m_tagDAB.SetScale((cx-12)/(256.0f),(cy-12)/(256.0f));
#else
	GLfloat texCoord_s01000[]={
		4.0/512.0, 172/512.0,
		4.0/512.0, 340.0/512.0,
		328.0/512.0, 172.0/512.0,
		328.0/512.0, 340.0/512.0};

	m_tagDAB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_150),texCoord_s01000);
	m_tagDAB.SetScale((cx-14)/316.0f,(cy-14)/160.0f);
#endif

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crg={0.1,0.1,0.1,1.0};

	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("DAB"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_ptagTitle = &m_tagTitle;


	m_tagFrequency.Initialize(IDC_RADIO_TAG_CURFREQUENCY,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagFrequency.SetSize(&siInfo);
	m_tagFrequency.SetCaption(_T("NONE"));
	m_tagFrequency.LinkTextGL(pTextGL);
	m_tagFrequency.SetPos(0,-m_siCurSize.cy/2+60);
	m_tagFrequency.SetCaptionHeight(22);
	m_tagFrequency.EnableDrawBack(FALSE);
	m_tagFrequency.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagFrequency.SetCaptionColor(&crw);

	return TRUE;
}

void CWidgetDAB::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idControl)
	{
	case IDC_RADIO_TAG_CURFREQUENCY:
		{
			if (idOperation == UIOPRT_SETSTRING)
			{
				m_tagFrequency.SetCaption((const wchar_t*)lpara);				
			}
		}
		break;
	default:
		break;
	}
}

//===========================================================//
//ISDB
CWidgetISDBCore::CWidgetISDBCore(void)
{
	m_bPlugin = TRUE;
	m_bOnFocus = FALSE;
	m_bPressed = FALSE;
}

CWidgetISDBCore::~CWidgetISDBCore(void)
{

}

BOOL CWidgetISDBCore::IsReady()
{
	return CWidgetGL::IsReady() && m_tagISDB.IsReady();
}

void CWidgetISDBCore::Render()
{
	if(!BeginRender())return;

	m_tagPlugin.SetLuminance(m_fCurLuminance);
	m_tagPlugin.SetAlpha(m_fCurAlpha);
	m_tagPlugin.Render();

	static CTexture *pTexUp=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_130):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha*0.7f};

	DrawRect(pTexUp,0,0,m_siCurSize.cx-16-10,m_siCurSize.cy-16-10,8,&cr,TRUE);

	m_tagISDB.SetLuminance(m_fCurLuminance);
	m_tagISDB.SetAlpha(m_fCurAlpha);
	m_tagISDB.Render();

	m_tagTitle.SetLuminance(m_fCurLuminance);
	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	EndRender();

	ProcessNotify();

}

BOOL CWidgetISDBCore::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CWidgetGL::Initialize(id,cx,cy,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_tagPlugin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_132),NULL);
	m_tagPlugin.SetPos(cx/2-50,-cy/2+50);
	m_tagPlugin.Show(FALSE);
	
	m_tagISDB.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_190),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);

	m_tagISDB.SetPos(0,10);
	m_tagISDB.SetShowScale(0.7f);  

	SIZE siInfo={m_siCurSize.cx-24,20};
	COLORGL crw={1.0,1.0,1.0,1.0};

	m_tagTitle.Initialize(0,this,NULL,NULL,	NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.SetCaption(_T("ISDB"));
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,-m_siCurSize.cy/2+22);
	m_tagTitle.SetCaptionHeight(18);
	m_tagTitle.EnableDrawBack(FALSE);
	m_tagTitle.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitle.SetCaptionColor(&crw);

	m_ptagTitle = &m_tagTitle;

	return TRUE;
}

void CWidgetISDBCore::OperateControl(UINT idControl,UINT idOperation,LPVOID lpara)
{
	switch (idOperation)
	{
	case UIOPRT_SETSTATUS:
		{
			m_bOnFocus = *((BOOL*)lpara);
		}
		break;
	case UIOPRT_SHOWHIDE:
		{
			m_bPlugin = *((BOOL*)lpara);
			m_tagPlugin.Show(m_bPlugin);
		}
		break;
	default:
		break;
	}
}
//=================================================//
CWidgetGL::CWidgetGL(void)
{
	m_bPressed =FALSE;
	m_bNotifyFlag=FALSE;
	m_dwLastGestureTick = 0;
	m_pCurActiveControl = NULL;
	m_bEnableSendNotify = TRUE;
	m_iNotifyPara=0;
	m_bOnFocus = FALSE;
	m_ptagTitle = NULL;
}
CWidgetGL::~CWidgetGL(void)
{

}

BOOL CWidgetGL::Initialize(UINT id,int cx,int cy,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CControlGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

// 	wchar_t caption[8];

	SetSize(cx,cy);

	return TRUE;
}
void CWidgetGL::SetPos(GLfloat x,GLfloat y,GLfloat z,BOOL bAni)
{
	CObjectGL::SetPos(x,y,z,bAni);

	m_listChildControls.Head();
	while (!m_listChildControls.End())
	{
		m_listChildControls.Get()->SetPos(m_listChildControls.Get()->GetPosxIni(),m_listChildControls.Get()->GetPosyIni(),m_listChildControls.Get()->GetPoszIni());
		m_listChildControls.Next();
	}
}
BOOL CWidgetGL::IsOnFocus()
{
	return m_bOnFocus;
}
BOOL CWidgetGL::IsReady(void)
{
	return CControlGL::IsReady() && (m_pCurActiveControl?m_pCurActiveControl->IsReady():TRUE);

}

void CWidgetGL::OnTouchDown(POINT *pt)
{
	m_ptLastPress = *pt;
	m_dwLastPressTick = GetTickCount();

	m_pCurActiveControl = NULL;

	m_listChildControls.Head();
	while (!m_listChildControls.End())
	{
		if(m_listChildControls.Get()->IsNotifyEnabled())
		{
			m_listChildControls.Get()->OnTouchDown(pt);
			if(!m_pCurActiveControl)
			{
				if(m_listChildControls.Get()->IsPtnOnObject(pt))
				{
					m_pCurActiveControl=m_listChildControls.Get();
					break;
				}
			}
		}
		m_listChildControls.Next();
	}

	if(m_pCurActiveControl)
	{
		PlaySoundEffect(0);
		return;
	}


	m_bPressed = IsPtnOnObject(pt) && IsAniReady();
	if(m_bPressed)
	{
#if (CVTE_EN_HOME_WIDGET_ANIM == CVTE_ENABLE)
		GLfloat offset_x=pt->x-(g_iScreenWidth/2)-GetPosX();
		GLfloat offset_y=(g_iScreenHeight/2)-pt->y-GetPosY();

		VIEW_STATE vs=GetCurViewState();
		vs.fTransZ=-50;
		vs.fRotateY=15*(offset_x)/GetSize().cx;
		vs.fRotateX=-15*(offset_y)/GetSize().cy;
// 		SetAimViewState(&vs);
		SetCurViewState(&vs);
#endif
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
		SetShowScale(1.2f);
		if(m_ptagTitle)
			m_ptagTitle->Show(FALSE);
#endif
		PlaySoundEffect(0);
	}
}
void CWidgetGL::OnTouchUp(POINT *pt)
{

	if(m_pCurActiveControl)
	{
		m_pCurActiveControl->OnTouchUp(pt);
		/*
		m_idCurControlUp=-1;
		m_listChildControls.Head();
		while (!m_listChildControls.End())
		{
			if(m_listChildControls.Get()->IsNotifyEnabled())
			{
				m_listChildControls.Get()->OnTouchUp(pt);
				if(m_idCurControlUp==-1)
				{
					if(m_listChildControls.Get()->IsPtnOnObject(pt))
					{
						UINT id=m_listChildControls.Get()->GetID();
						if(id==m_idCurControlDown)//up down both on the control, valid
						{
							m_idCurControlUp=m_listChildControls.Get()->GetID();
							break;
						}
					}
				}
			}
			m_listChildControls.Next();
		}

		m_bClickOnControl = FALSE;
		*/

		return;
	}


	if(IsPtnOnObject(pt) && m_bPressed)//notify
	{
		if(m_lpNotifyFunc && IsReady())
		{
			//m_lpNotifyFunc(m_ID,NOTIFY_SHORT_CLICK,px,&py,m_lparam);
			DWORD delt_tick=GetTickCount() - m_dwLastPressTick;
			if(/*delt_tick>10 &&*/ delt_tick<1000 && (abs(pt->y-m_ptLastPress.y)<20) && (abs(pt->x-m_ptLastPress.x)<20))
			{
				m_bNotifyFlag = TRUE;
				m_idNotifyID = NOTIFY_SHORT_CLICK;
				m_ptLastPress = *pt;
			}
		}
	}

	if(m_bPressed)
	{
#if (CVTE_EN_HOME_WIDGET_ANIM == CVTE_ENABLE)
		if(m_bNotifyFlag)
		{
			//animation:
			static VIEW_STATE vsAni[17];
			VIEW_STATE vs=GetCurViewState();
			GLfloat rx=vs.fRotateX;
			GLfloat ry=vs.fRotateY;

			//
			if(m_siCurSize.cx<m_siCurSize.cy)
			{
				if(fabs(rx)<15.0f)
					rx=rx>0?15.0f:-15.0f;
				ry=0;
			}
			else
			{
				if(fabs(ry)<15.0f)
					ry=ry>0?15.0f:-15.0f;
				rx=0;
			}

			for (int i=0;i<17;i++)
			{
				vsAni[i].fScaleX=1.0f;//vs.fScaleX*(1.0f+i*0.01f);
				vsAni[i].fScaleY=1.0f;//vs.fScaleY*(1.0f+i*0.01f);
				vsAni[i].fScaleZ=1.0f;//vs.fScaleZ*(1.0f+i*0.01f);
				vsAni[i].fTransX=GetPosX();
				vsAni[i].fTransY=GetPosY();
				vsAni[i].fTransZ=0;
				vsAni[i].fRotateX=rx*(16-i)/16.0f*cos(i*45.0f*PI/180.0f);
				vsAni[i].fRotateY=ry*(16-i)/16.0f*cos(i*45.0f*PI/180.0f);

				vsAni[i].fRotateZ=0;
			}

			LoadAnimatePath(vsAni,17);
		}
		else
		{
			VIEW_STATE vs=GetCurViewState();
			vs.fScaleX=1.0f;
			vs.fScaleY=1.0f;
			vs.fScaleZ=1.0f;
			vs.fTransZ=0;
			vs.fRotateX=0;
			vs.fRotateY=0;
			SetAimViewState(&vs);
// 			SetCurViewState(&vs);
		}
#endif
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_FIATUI)
		SetShowScale(1.0f);
		if(m_ptagTitle)
			m_ptagTitle->Show(TRUE);
#endif
 		m_bPressed=FALSE;
	}
}
void CWidgetGL::OnTouchMove(POINT *pt)
{
#if (CVTE_EN_HOME_WIDGET_ANIM == CVTE_ENABLE)
	if(m_bPressed)
	{
		GLfloat offset_x=pt->x-(g_iScreenWidth/2)-GetPosX();
		GLfloat offset_y=(g_iScreenHeight/2)-pt->y-GetPosY();

		VIEW_STATE vs=GetCurViewState();
		vs.fTransZ=-50;
		vs.fRotateY=15*offset_x/GetSize().cx;
		vs.fRotateX=-15*offset_y/GetSize().cy;
// 		SetAimViewState(&vs);
		SetCurViewState(&vs);
	}
#endif
}

void CWidgetGL::OnGesture(GESTUREINFO *pgi)
{
	if(!pgi)
		return;

	POINT pt={pgi->ptsLocation.x,pgi->ptsLocation.y};

	switch (pgi->dwID)
	{
	case GID_BEGIN:
		break;
	case GID_END:
		break;
	case GID_SELECT:
		{
			if(IsPtnOnObject(&pt) && IsAniReady())
			{
				m_bPressed = TRUE;

				m_bNotifyFlag = TRUE;
				m_idNotifyID = NOTIFY_SHORT_CLICK;

				//animation://////////////////////////////////////////
				static VIEW_STATE vsAni[17];
				VIEW_STATE vs=GetCurViewState();
				GLfloat rx=vs.fRotateX;
				GLfloat ry=vs.fRotateY;

				//
				if(m_siCurSize.cx<m_siCurSize.cy)
				{
					if(fabs(rx)<10.0f)
						rx=rx>0?10.0f:-10.0f;
					ry=0;
				}
				else
				{
					if(fabs(ry)<10.0f)
						ry=ry>0?10.0f:-10.0f;
					rx=0;
				}

				for (int i=0;i<17;i++)
				{
					vsAni[i].fScaleX=1.0f;//vs.fScaleX*(1.0f+i*0.01f);
					vsAni[i].fScaleY=1.0f;//vs.fScaleY*(1.0f+i*0.01f);
					vsAni[i].fScaleZ=1.0f;//vs.fScaleZ*(1.0f+i*0.01f);
					vsAni[i].fTransX=GetPosX();
					vsAni[i].fTransY=GetPosY();
					vsAni[i].fTransZ=0;
					vsAni[i].fRotateX=rx*cos(i*45.0f*PI/180.0f);
					vsAni[i].fRotateY=ry*cos(i*45.0f*PI/180.0f);

					vsAni[i].fRotateZ=0;
				}

				LoadAnimatePath(vsAni,17);
				/////////////////////////////////////////////////////

			}
		}
		break;
	default:
		{
			m_bNotifyFlag = TRUE;
			m_idNotifyID = NOTIFY_NULL;
		}
		break;
	}
}

void CWidgetGL::ProcessNotify()
{
	if(m_bNotifyFlag)
	{
		if(IsAniReady())
		{
			GLfloat offset_x=m_ptLastPress.x-(g_iScreenWidth/2)-GetPosX();
			GLfloat offset_y=(g_iScreenHeight/2)-m_ptLastPress.y-GetPosY();

			int px=int(offset_x);
			int py=int(offset_y);

			if(m_idNotifyID != NOTIFY_NULL && m_lpNotifyFunc && m_bEnableSendNotify)
			{
				m_lpNotifyFunc(m_ID,m_idNotifyID,m_iNotifyPara,LPVOID(1),m_lparam);
			}

			m_bNotifyFlag = FALSE;
			m_bPressed = FALSE;

			//reset vs:
			VIEW_STATE vs=GetCurViewState();
			vs.fScaleX=1.0f;
			vs.fScaleY=1.0f;
			vs.fScaleZ=1.0f;
			vs.fTransZ=0;
			vs.fRotateX=0;
			vs.fRotateY=0;
// 			SetAimViewState(&vs);
			SetCurViewState(&vs);
		}
	}
}
