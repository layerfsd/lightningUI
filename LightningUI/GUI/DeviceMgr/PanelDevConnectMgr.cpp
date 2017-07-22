#include "PanelDevConnectMgr.h"
#include "../../resource.h"


CPanelDevConnectMgr::CPanelDevConnectMgr(void)
{
// 	m_strTitle = _T("Device Inserted...");
	m_dwEnterTick = 0;
	m_bInsert = FALSE;
	m_bChecked = FALSE;
}

CPanelDevConnectMgr::~CPanelDevConnectMgr(void)
{
	m_idDeviceType = DEV_NULL;
}
BOOL CPanelDevConnectMgr::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelDevConnectMgr::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelDevConnectMgr::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	m_idDeviceType = (DEV_TYPE)(LOWORD(iParam));
	m_bInsert = (BOOL)(HIWORD(iParam));

	RETAILMSG(DEBUG_DEV_MGR,(_T("+++++++++CPanelDevConnectMgr OnEnterPanel m_idDeviceType[%d], m_bInsert[%d]\r\n"),m_idDeviceType,m_bInsert));

	m_tagTitle.SetCaption(GetResourceString(m_bInsert?IDS_DEVICE_INSERT:IDS_DEVICE_REMOVE));

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

	//////////////////////////////

	m_bEnableQuit = (m_bInsert?FALSE:TRUE);

	m_bChecked = FALSE;

	m_dwEnterTick = GetTickCount();
}
BOOL CPanelDevConnectMgr::IsDeviceReady(DEV_TYPE device)
{
	TCHAR szFind[MAX_PATH]=_T("");
	TCHAR szDevice[MAX_PATH]=_T("");

	WIN32_FIND_DATA wfd;

	switch (device)
	{
	case DEV_USBDISK:
		wcscpy_s(szDevice,MAX_PATH,_T("\\USB Disk\\")); //
		break;
	case 	DEV_USBDISK2:
		wcscpy_s(szDevice,MAX_PATH,_T("\\USB Disk2\\")); //
		break;
	case	DEV_SDMEDIACARD:
		wcscpy_s(szDevice,MAX_PATH,_T("\\MEDIA CARD\\")); //
		break;
	case	DEV_SDGPSCARD:
		wcscpy_s(szDevice,MAX_PATH,_T("\\GPS CARD\\")); //
		break;

		//test:
	case DEV_IAP:
		return TRUE;
		break;

	default:
		return TRUE;
		break;
	}

	swprintf_s(szFind,MAX_PATH,_T("%s*.*"),szDevice);
	//wcscat_s(szFind,MAX_PATH,_T("*.*")); //

	HANDLE hFind=FindFirstFile(szFind,& wfd);

	if(hFind==INVALID_HANDLE_VALUE) //
	{
		wcscat_s(szDevice,MAX_PATH,_T("temp\\"));
		if(CreateDirectory(szDevice,NULL))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	FindClose(hFind);

	return TRUE;
}
void CPanelDevConnectMgr::Render()
{
	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
// 	static COLORGL cr2={1,1,1,1};

// 	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_100):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};
// 	cr.a=m_pSkinManager->GetConfig()->fUITransparency;

	DWORD dwWait=2000;
	DWORD dwUI=MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
	BOOL bDeviceChecked=FALSE;
	DWORD param=0;

	if(!BeginRender())return;

	//RETAILMSG(1, (TEXT("CPanelDevConnectMgr::Render m_idDeviceType == %d\r\n"),m_idDeviceType));


	m_tagTitle.SetAlpha(m_fCurAlpha);
	m_tagTitle.Render();

	if(m_idDeviceType == DEV_USBDISK)
	{
		m_iconUSB.SetAlpha(m_fCurAlpha);
		m_iconUSB.Render();
		dwUI = MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
	}

	else if(m_idDeviceType == DEV_EASYCONNECTED)
	{
		m_iconUSB.SetAlpha(m_fCurAlpha);
		m_iconUSB.Render();
		dwUI = m_bInsert?MAKE_PANEL_ID(UI_CLASS_EASYCONNECTED,PAGE_EASYCONNECTED_WRAPPER):MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		dwWait=1000;
	}

	else if(m_idDeviceType == DEV_USBDISK2)
	{
		m_iconUSB.SetAlpha(m_fCurAlpha);
		m_iconUSB.Render();
		dwUI = MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
	}
	else if(m_idDeviceType == DEV_SDMEDIACARD)
	{
		m_iconSD.SetAlpha(m_fCurAlpha);
		m_iconSD.Render();
		dwUI = MAKE_PANEL_ID(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN);
	}
	else if(m_idDeviceType == DEV_SDGPSCARD)
	{
		m_iconSD.SetAlpha(m_fCurAlpha);
		m_iconSD.Render();
#if CVTE_EN_NAVI
		dwUI = MAKE_PANEL_ID(UI_CLASS_NAVI,PAGE_NAVI_MAIN);//UI_MEDIACORE;
#else
		dwUI = MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
#endif
	}
	else if(m_idDeviceType == DEV_IAP)
	{
		m_iconIAP.SetAlpha(m_fCurAlpha);
		m_iconIAP.Render();
		dwUI = MAKE_PANEL_ID(UI_CLASS_IPOD,PAGE_IPOD_MAIN);
// 		dwUI = MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		param=-1;
		dwWait=1000;
	}
	else if(m_idDeviceType == DEV_PHONELINK)
	{
		m_iconUSB.SetAlpha(m_fCurAlpha);
		m_iconUSB.Render();
		dwUI = m_bInsert?MAKE_PANEL_ID(UI_CLASS_PHONELINK,PAGE_PHONELINK_WRAPPER):MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		dwWait=3000;
	}
	else if(m_idDeviceType == DEV_DECK)
	{
		m_iconUSB.SetAlpha(m_fCurAlpha);
		m_iconUSB.Render();
		dwUI = m_bInsert?MAKE_PANEL_ID(UI_CLASS_DISK,PAGE_DISK_MAIN):MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
	}

	if(m_bInsert)
	{
		if(!m_bChecked && GetTickCount() - m_dwEnterTick > dwWait)
		{
			//RETAILMSG(DEBUG_DEV_MGR,(_T("+++++++++CPanelDevConnectMgr check device :%d\r\n"),m_idDeviceType));

			if(IsDeviceReady(m_idDeviceType))
			{
				RETAILMSG(DEBUG_DEV_MGR,(_T("+++++++++CPanelDevConnectMgr device ready:%d\r\n"),m_idDeviceType));

				//auto detect device is used for update:
				BOOL bForUpdate=FALSE;
				if(m_idDeviceType == DEV_USBDISK)
                {
					
					#if CVTE_EN_PACKAGE_UPDATE
					bForUpdate = (IsFileExists(UPDATE_FILE_MCU_USBDISK) || IsFileExists(UPDATE_FILE_PACKAGE_USBDISK));
					#else
					bForUpdate = (IsFileExists(UPDATE_FILE_MCU_USBDISK) || IsFileExists(UPDATE_FILE_IMAGE_USBDISK));
					#endif
                }

				m_bEnableQuit = TRUE;
	// 			::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,dwUI,NULL);
				if(bForUpdate)
					SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_NONE,PAGE_SETTINGS_ABOUT);
				else
					::PostMessage(m_hMainWnd,WM_DEVICE_INSERT_READY,dwUI,param);
				m_bChecked = TRUE;
			}
			else if(GetTickCount()-m_dwEnterTick>10000)
			{
				RETAILMSG(DEBUG_DEV_MGR,(_T("+++++++++CPanelDevConnectMgr no device or device has no files: %d\r\n"),m_idDeviceType));
				m_bEnableQuit = TRUE;
				m_bChecked = TRUE;

				if(m_idPreviousPanel)
					SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,0/*m_iPreviousPanelParam*/);
				else
					SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
			}
		}

		SetFrameTime(m_bChecked?500:30);
		if(!m_bChecked)
		{
			VIEW_STATE vs=m_iconLoading.GetCurViewState();
			vs.fRotateZ-=5;
			if(vs.fRotateZ<-360)
				vs.fRotateZ=0;
			m_iconLoading.SetCurViewState(&vs);
			m_iconLoading.SetAlpha(m_fCurAlpha);
			m_iconLoading.Render();
		}
	}
	else
	{
		m_bEnableQuit = TRUE;

		if(!m_bChecked)
		{
// 			if(m_idDeviceType == DEV_SDGPSCARD)
// 			{
// 				::PostMessage(m_hMainWnd,WM_SYSTEM_POWER_STATE,SYS_POWER_SOFT_RESET,NULL);
//
// 				m_bChecked = TRUE;
// 			}
// 			else
			{
				if(GetTickCount() - m_dwEnterTick > dwWait)
				{
					RETAILMSG(DEBUG_DEV_MGR,(_T("+++++++++CPanelDevConnectMgr device remove:%d\r\n"),m_idDeviceType));

// 					m_bEnableQuit = TRUE;

					if((m_idDeviceType == DEV_PHONELINK && GET_PANEL_CLASS_ID(m_idPreviousPanel) == UI_CLASS_PHONELINK) ||
						(m_idDeviceType == DEV_EASYCONNECTED && GET_PANEL_CLASS_ID(m_idPreviousPanel) == UI_CLASS_EASYCONNECTED) ||
						(m_idDeviceType == DEV_IAP && GET_PANEL_CLASS_ID(m_idPreviousPanel) == UI_CLASS_IPOD) ||
#if (CVTE_EN_UNDER_MEDIA_PLAYER_PULLOUT_UDISK_AUTO_RETURN_HOME)
						((m_idDeviceType == DEV_USBDISK||m_idDeviceType == DEV_USBDISK2) && (GET_PANEL_CLASS_ID(m_idPreviousPanel) == UI_CLASS_MEDIA||GET_PANEL_CLASS_ID(m_idPreviousPanel) == UI_CLASS_IMAGE)) ||
#endif						
						FALSE)
					{
						::PostMessage(m_hMainWnd,WM_DEVICE_REMOVE_READY,MAKE_PANEL_ID(UI_CLASS_HOME,PAGE_HOME_WRAPPER),NULL);
					}
					else
					{
						if(m_idPreviousPanel)
							SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,0/*m_iPreviousPanelParam*/);
						else
							SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
					}
					m_bChecked = TRUE;
				}
			}

			//
			SetFrameTime(m_bChecked?500:30);
			if(!m_bChecked)
			{
				VIEW_STATE vs=m_iconLoading.GetCurViewState();
				vs.fRotateZ-=5;
				if(vs.fRotateZ<-360)
					vs.fRotateZ=0;
				m_iconLoading.SetCurViewState(&vs);
				m_iconLoading.SetAlpha(m_fCurAlpha);
				m_iconLoading.Render();
			}
		}
	}


	EndRender();

}
BOOL CPanelDevConnectMgr::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bEnableQuit = FALSE;
	m_bShowStatusBar = FALSE;

// 	//define ani:
// 	SetAniMoveIn(ANIM_LINE_TOP_IN);
// 	SetAniPrePanelMoveOut(ANIM_LINE_BOTTOM_OUT);
// 	SetAniMoveOut(ANIM_LINE_TOP_OUT);
// 	SetAniNextPanelMoveIn(ANIM_LINE_BOTTOM_IN);

	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(1.5f);
	m_iconLoading.SetPos(0,0);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	//notification icons:
	m_iconUSB.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_133));
	m_iconUSB.SetShowScale(0.96);

	m_iconSD.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_127));
	m_iconSD.SetShowScale(0.96);

	m_iconIAP.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_153));
	m_iconIAP.SetShowScale(0.96);

#else
	//notification icons:
	m_iconUSB.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_123));
	m_iconUSB.SetShowScale(0.96);

	m_iconSD.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_124));
	m_iconSD.SetShowScale(0.96);

	m_iconIAP.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_123));
	m_iconIAP.SetShowScale(0.96);
#endif

	SIZE siInfo={600,40};
	COLORGL crg={0.85,0.85,0.85,1.0};
	m_tagTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitle.SetSize(&siInfo);
	m_tagTitle.LinkTextGL(pTextGL);
	m_tagTitle.SetPos(0,160);
	m_tagTitle.SetCaptionHeight(32);
	m_tagTitle.EnableDrawBack(FALSE);
	//m_tagTitle.SetCaptionBackTex(m_pSkinManager->GetTexture(TEXID_054));
	m_tagTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitle.SetCaptionColor(&crg);

	return TRUE;
}


void CPanelDevConnectMgr::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelDevConnectMgr::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
}
void CPanelDevConnectMgr::OnTouchMove(POINT *pt)
{

}


void CALLBACK CPanelDevConnectMgr::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_DEV_MGR,(_T("CPanelDevConnectMgr OnNotify id:%d,msg:%d,para:%d\r\n"),idControl,wMsg,wPara));

	CPanelDevConnectMgr *panel=(CPanelDevConnectMgr*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelDevConnectMgr::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_NB_BTN_CANCEL://cancel
		break;
	}
}

BOOL CPanelDevConnectMgr::IsFileExists(LPCTSTR   lpszFileName)
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