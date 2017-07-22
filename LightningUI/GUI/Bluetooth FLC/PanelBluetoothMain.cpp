#include "PanelBluetoothMain.h"
#include "../Media/MediaCommon.h"
#include "../../resource.h"

extern const wchar_t* g_strInstallPath;

CPanelBluetoothMain::CPanelBluetoothMain(void)
{
	m_type = BT_HOMEPAGE_DEVICE;
	m_statusContact = CONTACT_NULL;

	m_hfStatus = CONNECTION_STATUS_IDLE;

	m_bDeviceSearching = FALSE;
	m_bContactManualDownload = FALSE;

	m_bIniStatus = FALSE;
	m_bConnected = FALSE;
	m_bContanctSearching = FALSE;
	m_dwIniTick = 0;

#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	m_statusCallLog = CONTACT_NULL;
	m_bCallLogManualDownload = FALSE;
	m_bUserDownloadCallLogPermit = FALSE;
#endif

	m_bDownloadingCalllog = FALSE;
	m_bStoredContactAvailable = FALSE;
	m_bSyncingData = FALSE;

}
CPanelBluetoothMain::~CPanelBluetoothMain(void)
{

}

BOOL CPanelBluetoothMain::IsReady()
{
	if(m_type == BT_HOMEPAGE_DEVICE)
		return CPanelGL::IsReady();// && (m_bShowSearchedDevice?!m_bDeviceSearching:TRUE);
	else if(m_type == BT_HOMEPAGE_CONTACT)
		return CPanelGL::IsReady() && (m_boxContact.IsVisible()?m_boxContact.IsReady():TRUE) && (m_boxContanctSearch.IsVisible()?m_boxContanctSearch.IsReady():TRUE);
	else if(m_type == BT_HOMEPAGE_HISTORY)
		return CPanelGL::IsReady() && (m_boxHistory.IsVisible()?m_boxHistory.IsReady():TRUE);
	else
		return CPanelGL::IsReady();

}

void CPanelBluetoothMain::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	RETAILMSG(1, (TEXT("CPanelBluetoothMain::OnEnterPanel: iParam[%d], idAni[%d] \r\n", iParam, idAni)));
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

	//
	if(iParam!=0)
	{
		BT_HOMEPAGE_TYPE page=BT_HOMEPAGE_TYPE(LOWORD(iParam));
		m_idPickDeviceFor=HIWORD(iParam);

		SetShowType(page);
		if(page==BT_HOMEPAGE_DEVICE && m_boxDeviceList.GetCount()==0)
		{
			//SearchDevice();
			m_btnSearchDevice.SetCaption(GetResourceString(m_bDeviceSearching?IDS_CANCEL:IDS_SEARCH));
		}
	}
	else
	{
		m_idPickDeviceFor=0;
		if (m_bContanctSearching)
		{
			SetShowType(BT_HOMEPAGE_CONTACT);
		}
		else
		{
			if(m_hfStatus == CONNECTION_STATUS_CONNECTED)
			{
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
				if (BT_HOMEPAGE_HISTORY == m_type)
				{
					SetShowType(BT_HOMEPAGE_HISTORY);
				}
				else if (BT_HOMEPAGE_CONTACT == m_type)
				{
					SetShowType(BT_HOMEPAGE_CONTACT);
				}
#else
				SetShowType(BT_HOMEPAGE_DIAL);
#endif

			}
			else if(m_boxDeviceList.GetCount()==0)
			{
				//SearchDevice();
				m_btnSearchDevice.SetCaption(GetResourceString(m_bDeviceSearching?IDS_CANCEL:IDS_SEARCH));
			}
		}

	}
}

void CPanelBluetoothMain::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	//
	if(m_type == BT_HOMEPAGE_CONTACT)
	{
		ShowContactDetail(FALSE,NULL);
		if(m_boxContact.IsOnQuickSearchMode())
		{
			m_boxContact.ShowQuickSearchMode(FALSE);
		}
		ClearInput(-1);//clear all
	}

	//default
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelBluetoothMain::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
void CPanelBluetoothMain::Render()
{
	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_604):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
#else
	COLORGL cr_gray={0.1,0.1,0.1,m_fCurAlpha};
#endif

	if(!BeginRender())return;

	DrawRect(pTexBack,0,0,g_iClientWidth-4,g_iClientHeight-4,2,&cr,TRUE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif

	//
	if(m_type == BT_HOMEPAGE_CONTACT)
	{
		m_boxLists.Enable(!m_boxContact.IsOnQuickSearchMode());
		m_btnHome.Enable(!m_boxContact.IsOnQuickSearchMode());
	}
	else
	{
		m_boxLists.Enable(TRUE);
		m_btnHome.Enable(TRUE);
	}

	//box:
	m_boxLists.SetAlpha(m_fCurAlpha);
	m_boxLists.Render();


	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

		if(m_type == BT_HOMEPAGE_CONTACT)
		{
#if BT_PHONEBOOK_AUTOSTORE
			static BOOL bSearMode=FALSE;
			if(m_boxContact.IsOnQuickSearchMode() != bSearMode)
			{
				bSearMode = m_boxContact.IsOnQuickSearchMode();
				m_btnSyncContact.SetCaption(m_boxContact.IsOnQuickSearchMode()?GetResourceString(IDS_BT_PHONEBOOK_BACK):GetResourceString(IDS_BT_PHONEBOOK_SYNC));
			}
#endif
			if (m_bContanctSearching)
			{
				m_boxContanctSearch.Show(TRUE);
				m_boxContanctSearch.SetAlpha(m_fCurAlpha);
				m_boxContanctSearch.Render();
				m_boxContact.Show(FALSE);

#if BT_PHONEBOOK_AUTOSTORE
				m_btnSyncContact.Show(m_statusContact==CONTACT_DOWNLOADED && GetTickCount()-m_dwContactDownloadTick>1000);
#endif
				if(m_boxContactDetail.IsVisible() && (GetTickCount()-m_dwShowContactDetailTick>10000))
					m_boxContactDetail.Show(FALSE);

				m_boxContanctSearch.EnableNotify(!m_boxContactDetail.IsVisible());

				m_boxContactDetail.SetAlpha(m_fCurAlpha);
				m_boxContactDetail.Render();

				m_btnSearchContact.SetAlpha(m_fCurAlpha);
				m_btnSearchContact.Render();

#if BT_PHONEBOOK_AUTOSTORE
				m_btnSyncContact.SetAlpha(m_fCurAlpha);
				m_btnSyncContact.Render();
#endif
			}
			else
			{
				m_boxContact.Show(m_statusContact==CONTACT_DOWNLOADED);

				m_btnDownloadPhonebook.Show(m_statusContact==CONTACT_NULL || m_statusContact==CONTACT_DOWNLOADFAILED || m_statusContact==CONTACT_DOWNLOADCANCLED);
				m_btnCancelDownloadPhonebook.Show(m_statusContact==CONTACT_DOWNLOADING && GetTickCount()-m_dwContactDownloadTick>1000);

				m_btnSearchContact.Show(m_statusContact==CONTACT_DOWNLOADED && GetTickCount()-m_dwContactDownloadTick>1000);
#if BT_PHONEBOOK_AUTOSTORE
				m_btnSyncContact.Show(m_statusContact==CONTACT_DOWNLOADED && GetTickCount()-m_dwContactDownloadTick>1000);
#endif

				if(m_boxContactDetail.IsVisible() && (GetTickCount()-m_dwShowContactDetailTick>10000 || m_boxContact.IsOnQuickSearchMode()))
					m_boxContactDetail.Show(FALSE);

				BOOL bEnableKey=(!m_boxContactDetail.IsVisible() && !m_boxContact.IsOnQuickSearchMode());

				m_boxContact.EnableNotify(!m_boxContactDetail.IsVisible());

				m_btnSearchContact.SetAlpha(m_fCurAlpha);
				m_btnSearchContact.Render();
#if BT_PHONEBOOK_AUTOSTORE
				m_btnSyncContact.SetAlpha(m_fCurAlpha);
				m_btnSyncContact.Render();
#endif
				m_boxContact.SetAlpha(m_fCurAlpha);
				m_boxContact.Render();
				m_boxContactDetail.SetAlpha(m_fCurAlpha);
				m_boxContactDetail.Render();

				m_btnDownloadPhonebook.SetAlpha(m_fCurAlpha);
				m_btnDownloadPhonebook.Render();

				m_btnCancelDownloadPhonebook.SetAlpha(m_fCurAlpha);
				m_btnCancelDownloadPhonebook.Render();

				SetFrameTime((m_statusContact == CONTACT_DOWNLOADING)?30:500);

				//
				if(m_statusContact == CONTACT_DOWNLOADING)
				{
					int cx=LIST_BOX_ITEM_WIDTH;
					int cy=LIST_BOX_ITEM_HEIGHT;

					m_iconSearching.SetPos(cx/2,-55);

					VIEW_STATE vs=m_iconSearching.GetCurViewState();
					vs.fRotateZ-=5;
					if(vs.fRotateZ<-360)
						vs.fRotateZ=0;
					m_iconSearching.SetCurViewState(&vs);
					m_iconSearching.SetAlpha(m_fCurAlpha);
					m_iconSearching.Render();
				}
			}
			

		}
		else if(m_type == BT_HOMEPAGE_DIAL)
		{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			m_tagKeyboardBk.SetAlpha(m_fCurAlpha);
			m_tagKeyboardBk.Render();

			m_tagLine1.SetAlpha(m_fCurAlpha);
			m_tagLine1.Render();

			m_tagLine2.SetAlpha(m_fCurAlpha);
			m_tagLine2.Render();

			m_tagLine3.SetAlpha(m_fCurAlpha);
			m_tagLine3.Render();

			m_tagLine4.SetAlpha(m_fCurAlpha);
			m_tagLine4.Render();

			m_tagLine5.SetAlpha(m_fCurAlpha);
			m_tagLine5.Render();

			m_tagLine6.SetAlpha(m_fCurAlpha);
			m_tagLine6.Render();

			m_tagLine7.SetAlpha(m_fCurAlpha);
			m_tagLine7.Render();

			m_tagLine8.SetAlpha(m_fCurAlpha);
			m_tagLine8.Render();

			m_tagLine9.SetAlpha(m_fCurAlpha);
			m_tagLine9.Render();
#endif
			m_btnDelete.Show(m_type == BT_HOMEPAGE_DIAL && m_strDialString.Size()>0);

			m_tagDialString.SetAlpha(m_fCurAlpha);
			m_tagDialString.Render();

			m_btnNum1.SetAlpha(m_fCurAlpha);
			m_btnNum1.Render();
			m_btnNum2.SetAlpha(m_fCurAlpha);
			m_btnNum2.Render();
			m_btnNum3.SetAlpha(m_fCurAlpha);
			m_btnNum3.Render();

			m_btnNum4.SetAlpha(m_fCurAlpha);
			m_btnNum4.Render();
			m_btnNum5.SetAlpha(m_fCurAlpha);
			m_btnNum5.Render();
			m_btnNum6.SetAlpha(m_fCurAlpha);
			m_btnNum6.Render();

			m_btnNum7.SetAlpha(m_fCurAlpha);
			m_btnNum7.Render();
			m_btnNum8.SetAlpha(m_fCurAlpha);
			m_btnNum8.Render();
			m_btnNum9.SetAlpha(m_fCurAlpha);
			m_btnNum9.Render();

			m_btnStar.SetAlpha(m_fCurAlpha);
			m_btnStar.Render();
			m_btnNum0.SetAlpha(m_fCurAlpha);
			m_btnNum0.Render();
			m_btnWell.SetAlpha(m_fCurAlpha);
			m_btnWell.Render();

			m_btnDial.SetAlpha(m_fCurAlpha);
			m_btnDial.Render();
			m_btnDelete.SetAlpha(m_fCurAlpha);
			m_btnDelete.Render();


			m_tagNum2.SetAlpha(m_fCurAlpha);
			m_tagNum2.Render();

			m_tagNum3.SetAlpha(m_fCurAlpha);
			m_tagNum3.Render();

			m_tagNum4.SetAlpha(m_fCurAlpha);
			m_tagNum4.Render();

			m_tagNum5.SetAlpha(m_fCurAlpha);
			m_tagNum5.Render();

			m_tagNum6.SetAlpha(m_fCurAlpha);
			m_tagNum6.Render();

			m_tagNum7.SetAlpha(m_fCurAlpha);
			m_tagNum7.Render();

			m_tagNum8.SetAlpha(m_fCurAlpha);
			m_tagNum8.Render();

			m_tagNum9.SetAlpha(m_fCurAlpha);
			m_tagNum9.Render();
			m_tagNum0.SetAlpha(m_fCurAlpha);
			m_tagNum0.Render();

		}
		else if(m_type == BT_HOMEPAGE_DEVICE)
		{
			SetFrameTime((m_bDeviceSearching || !m_bIniStatus)?30:500);

			m_boxDeviceList.SetAlpha(m_fCurAlpha);
			m_boxDeviceList.Render();
			m_boxDeviceList.Show(/*!m_bDeviceSearching*/TRUE);

			m_btnSearchDevice.SetAlpha(m_fCurAlpha);
			m_btnSearchDevice.Render();


			//show status:
			if(m_bDeviceSearching || !m_bIniStatus)
			{
				//m_iconBT.SetAlpha(m_fCurAlpha);
				//m_iconBT.Render();

				int cx=LIST_BOX_ITEM_WIDTH;
				int cy=LIST_BOX_ITEM_HEIGHT;

				m_iconSearching.SetPos(cx/2,m_bIniStatus?cy/2:0);

				VIEW_STATE vs=m_iconSearching.GetCurViewState();
				vs.fRotateZ-=5;
				if(vs.fRotateZ<-360)
					vs.fRotateZ=0;
				m_iconSearching.SetCurViewState(&vs);
				m_iconSearching.SetAlpha(m_fCurAlpha);
				m_iconSearching.Render();
			}
			else
			{
				if(m_boxDeviceList.GetCount()==0)
				{
					SIZE si;

					const wchar_t *pStNote=GetResourceString(IDS_NO_DEVICE_FOUND);
					if(pStNote)
					{
						m_pTextGL->SetColor(&cr_gray,FALSE);
						m_pTextGL->SetString(pStNote);
						m_pTextGL->SetCharScale(1.0f,1.0f);
						m_pTextGL->GetExtension(&si);
						m_pTextGL->SetPos(-si.cx/2+110,130);
						m_pTextGL->Render();
					}

					const wchar_t *pStNote2=GetResourceString(IDS_PHONECORE_FINDME_NOTE);
					if(pStNote2)
					{
						m_pTextGL->SetColor(&cr_gray,FALSE);
						m_pTextGL->SetString(pStNote2);
						if (m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_PERSIAN)
							m_pTextGL->SetCharScale(0.65f,0.65f);
						else
							m_pTextGL->SetCharScale(0.80f,0.80f);
						m_pTextGL->GetExtension(&si);
						m_pTextGL->SetPos(-si.cx/2+110,40);
						m_pTextGL->Render();
					}
				}
			}

			// 		m_btnPairedDevice.SetAlpha(m_fCurAlpha);
			// 		m_btnPairedDevice.Render();

			// 		m_tagLine.SetAlpha(m_fCurAlpha);
			// 		m_tagLine.Render();

		}
		else if(m_type == BT_HOMEPAGE_HISTORY)
		{
			m_boxHistory.Show(!m_bDownloadingCalllog);
			m_boxHistory.SetAlpha(m_fCurAlpha);
			m_boxHistory.Render();
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
			if(m_boxHistory.IsVisible() && m_boxHistory.GetCount())
			{
				m_btnDownloadCallLog.Show(FALSE);
				m_btnCancelDownloadCallLog.Show(FALSE);
			}
			else
			{
				m_btnDownloadCallLog.Show(!m_bDownloadingCalllog&&(m_statusCallLog==CONTACT_NULL || m_statusCallLog==CONTACT_DOWNLOADFAILED || m_statusCallLog==CONTACT_DOWNLOADCANCLED));
				m_btnCancelDownloadCallLog.Show(m_bDownloadingCalllog || (m_statusCallLog==CONTACT_DOWNLOADING && GetTickCount()-m_dwContactDownloadTick>1000));
			}

			m_btnDownloadCallLog.SetAlpha(m_fCurAlpha);
			m_btnDownloadCallLog.Render();

			m_btnCancelDownloadCallLog.SetAlpha(m_fCurAlpha);
			m_btnCancelDownloadCallLog.Render();

			SetFrameTime((m_bDownloadingCalllog || m_statusCallLog == CONTACT_DOWNLOADING)?30:500);

			//
			if(m_bDownloadingCalllog || m_statusCallLog == CONTACT_DOWNLOADING)
			{
				int cx=LIST_BOX_ITEM_WIDTH;
				int cy=LIST_BOX_ITEM_HEIGHT;

				m_iconSearching.SetPos(cx/2,-55);

				VIEW_STATE vs=m_iconSearching.GetCurViewState();
				vs.fRotateZ-=5;
				if(vs.fRotateZ<-360)
					vs.fRotateZ=0;
				m_iconSearching.SetCurViewState(&vs);
				m_iconSearching.SetAlpha(m_fCurAlpha);
				m_iconSearching.Render();
			}
#else
			SetFrameTime((m_bDownloadingCalllog)?30:500);

			if(m_bDownloadingCalllog)
			{
				int cx=LIST_BOX_ITEM_WIDTH;
				int cy=LIST_BOX_ITEM_HEIGHT;

				m_iconSearching.SetPos(cx/2,0);

				VIEW_STATE vs=m_iconSearching.GetCurViewState();
				vs.fRotateZ-=5;
				if(vs.fRotateZ<-360)
					vs.fRotateZ=0;
				m_iconSearching.SetCurViewState(&vs);
				m_iconSearching.SetAlpha(m_fCurAlpha);
				m_iconSearching.Render();
			}
#endif
		}



	EndRender();
}

BOOL CPanelBluetoothMain::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	const int cx=LIST_BOX_ITEM_WIDTH;
	const int cy=LIST_BOX_ITEM_HEIGHT;

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(g_iScreenWidth-cx, g_iClientHeight*4);
	m_tagBk.SetPos(cx/2, -g_iClientHeight);
	
	m_iconArrowOn.Initialize(0, NULL, m_pSkinManager->GetTexture(TEXID_083));
	m_iconArrowOn.SetShowScale(0.8f);
	m_iconArrowOff.Initialize(0, NULL, m_pSkinManager->GetTexture(TEXID_068));
	m_iconArrowOff.SetShowScale(0.8f);

	m_iconTypeHome.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_549));
	m_iconTypeMobile.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_548));
	m_iconTypeWork.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_547));
	m_iconTypeDefault.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_518));

	m_iconOut.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1208));
	m_iconOut.SetShowScale(1.2f);
	m_iconOutFocus.Initialize(0, NULL, m_pSkinManager->GetTexture(TEXID_1209));
	m_iconOutFocus.SetShowScale(1.2f);

	m_iconIn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1206));
	m_iconIn.SetShowScale(1.2f);
	m_iconInFocus.Initialize(0, NULL, m_pSkinManager->GetTexture(TEXID_1207));
	m_iconInFocus.SetShowScale(1.2f);
	m_iconMissed.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1213));
	m_iconMissed.SetShowScale(1.2f);
	m_iconMissedFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1214));
	m_iconMissedFocus.SetShowScale(1.2f);


	//list:
	m_iconDevice.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1211));
	m_iconDevice.SetShowScale(1.2f);
	m_iconDeviceFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1212));
	m_iconDeviceFocus.SetShowScale(1.2f);

	m_iconHistory.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1205));
	m_iconHistory.SetShowScale(1.2f);
	m_iconHistoryFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1210));
	m_iconHistoryFocus.SetShowScale(1.2f);

	m_iconContact.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1201));
	m_iconContact.SetShowScale(1.2f);
	m_iconContactFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1202));
	m_iconContactFocus.SetShowScale(1.2f);

	m_iconDelete.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1401));
	m_iconDelete.SetShowScale(1.2f);
	m_iconDeleteFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1402));
	m_iconDeleteFocus.SetShowScale(1.2f);

	m_iconKeyboard.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1203));
	m_iconKeyboard.SetShowScale(1.2f);
	m_iconKeyboardFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1204));
	m_iconKeyboardFocus.SetShowScale(1.2f);

	m_iconSearchDeviceUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_273));
	//m_iconSearchDeviceDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_272));

	COLORGL cr_gray={128.0f/256.0f,129.0f/256.0f,132.0f/256.0f,1.0};
	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};

	SIZE sib={cx,cy};

	//
	m_iconSearching.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconSearching.SetShowScale(1.2f);
	m_iconSearching.SetPos(cx/2,cy/2);
	m_iconBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_121));
	m_iconBT.SetShowScale(1.7f);
	m_iconBT.SetPos(cx/2,-cy/2);

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_204));
	m_iconHomeUp.SetShowScale(1.2f);
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));
	m_iconHomeDown.SetShowScale(1.2f);
	m_btnHome.Initialize(IDC_BT_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_285),NULL,
		m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sib);
	m_btnHome.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,68);
	m_btnHome.SetCaptionColor(&cr_w,&cr_gray);
	m_btnHome.SetCaptionHeight(20);
	m_btnHome.SetIconPos(-cx/2+35,0);

	//
	SIZE si_btn={(g_iClientWidth-cx),cy+0};

	m_btnSearchDevice.Initialize(IDC_BT_BTN_SEARCHDEVICE,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnSearchDevice.SetSize(&si_btn);
	m_btnSearchDevice.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnSearchDevice.SetPos(/*(g_iClientWidth-cx)/4+*/cx/2,-g_iClientHeight/2+cy/2);
	m_btnSearchDevice.LinkTextGL(pTextGL);

	m_btnSearchDevice.SetIcon(&m_iconSearchDeviceUp);
	m_btnSearchDevice.SetCaption(GetResourceString(IDS_SEARCH));
	m_btnSearchDevice.SetCaptionPos(BTN_CAPTION_LEFT,(g_iClientWidth-cx)/2-10,0);
	m_btnSearchDevice.SetCaptionColor(&cr_w);
	m_btnSearchDevice.SetCaptionHeight(22);
	m_btnSearchDevice.SetIconPos(-50,0);

	//
	int cx_box=(g_iScreenWidth-cx);

	SIZE si0={cx_box-20,80};

	//download
	m_btnDownloadPhonebook.Initialize(IDC_BT_BTN_CONTACT_DOWNLOAD,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnDownloadPhonebook.SetCaption(GetResourceString(IDS_DOWNLOAD));
	m_btnDownloadPhonebook.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDownloadPhonebook.SetCaptionHeight(22);
	m_btnDownloadPhonebook.LinkTextGL(pTextGL);
	m_btnDownloadPhonebook.SetSize(&si0);
	m_btnDownloadPhonebook.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnDownloadPhonebook.SetPos(cx/2,g_iClientHeight/2-60);

	m_btnCancelDownloadPhonebook.Initialize(IDC_BT_BTN_CONTACT_CANCEL_DOWNLOAD,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnCancelDownloadPhonebook.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancelDownloadPhonebook.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancelDownloadPhonebook.SetCaptionHeight(22);
	m_btnCancelDownloadPhonebook.LinkTextGL(pTextGL);
	m_btnCancelDownloadPhonebook.SetSize(&si0);
	m_btnCancelDownloadPhonebook.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnCancelDownloadPhonebook.SetPos(cx/2,g_iClientHeight/2-60);
	m_btnCancelDownloadPhonebook.Show(FALSE);

#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	//download CallLog button
	m_btnDownloadCallLog.Initialize(IDC_BT_BTN_CONTACT_DOWNLOAD,this,m_pSkinManager->GetTexture(TEXID_513),NULL,
		m_pSkinManager->GetTexture(TEXID_512),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDownloadCallLog.SetCaption(GetResourceString(IDS_DOWNLOAD));
	m_btnDownloadCallLog.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDownloadCallLog.SetCaptionHeight(28);
	m_btnDownloadCallLog.LinkTextGL(pTextGL);
	m_btnDownloadCallLog.SetSize(&si0);
	m_btnDownloadCallLog.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnDownloadCallLog.SetPos(cx/2,g_iClientHeight/2-60);

	m_btnCancelDownloadCallLog.Initialize(IDC_BT_DOWNLOAD_CANCEL_CALLHISTORY,this,m_pSkinManager->GetTexture(TEXID_553),NULL,
		m_pSkinManager->GetTexture(TEXID_512),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancelDownloadCallLog.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancelDownloadCallLog.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancelDownloadCallLog.SetCaptionHeight(28);
	m_btnCancelDownloadCallLog.LinkTextGL(pTextGL);
	m_btnCancelDownloadCallLog.SetSize(&si0);
	m_btnCancelDownloadCallLog.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnCancelDownloadCallLog.SetPos(cx/2,g_iClientHeight/2-60);
	m_btnCancelDownloadCallLog.Show(FALSE);
#endif

	SIZE si1={cx_box-20-120,80};
	SIZE si2={138,80};

	m_iconSearchContactUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_013));

	m_btnSearchContact.Initialize(IDC_BT_BTN_SEARCH_CONTACT,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnSearchContact.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnSearchContact.SetCaptionHeight(22);
	m_btnSearchContact.LinkTextGL(pTextGL);
	m_btnSearchContact.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
#if BT_PHONEBOOK_AUTOSTORE
	m_btnSearchContact.SetSize(&si1);
	m_btnSearchContact.SetPos(cx/2 + 68,g_iClientHeight/2 - si1.cy/2+1);
#else
	m_btnSearchContact.SetSize(&si0);
	m_btnSearchContact.SetPos(cx/2,g_iClientHeight/2-45);
#endif
	m_btnSearchContact.SetIcon(&m_iconSearchContactUp);
	m_btnSearchContact.Show(FALSE);

#if BT_PHONEBOOK_AUTOSTORE

	m_btnSyncContact.Initialize(IDC_BT_BTN_SYNC_CONTACT,this,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL);
	m_btnSyncContact.SetCaption(GetResourceString(IDS_BT_PHONEBOOK_SYNC));
	m_btnSyncContact.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnSyncContact.SetCaptionHeight(22);
	m_btnSyncContact.LinkTextGL(pTextGL);
	m_btnSyncContact.SetSize(&si2);
	m_btnSyncContact.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnSyncContact.SetPos(-g_iClientWidth/2 + cx + si2.cx/2 , g_iClientHeight/2 - si2.cy/2);
	m_btnSyncContact.Show(FALSE);

#endif

	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));

	//find param:
	int count=(g_iClientHeight-cy)/cy;
	if((g_iClientHeight-cy)%(cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy)/count;
	box.siClient.cx=cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=cx;
	box.siBorder.cy=g_iClientHeight-cy;

	box.idTexIndexBase=TEXID_289;
	box.idTexIndexFocus=TEXID_242;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_285;
	box.bNoSlider=TRUE;
	box.bNoBorder=FALSE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxLists.Initialize(IDC_BT_BOX_MENU,this,&box,pSkinManager);
	m_boxLists.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxLists.SetPos(-g_iClientWidth/2+cx/2,cy/2);
	m_boxLists.LinkTextGL(pTextGL);
	m_boxLists.SetAlpha(1.0f);
	m_boxLists.SetCaptioniHeight(20);
	m_boxLists.SetCaptionColor(&cr_gray,&cr_w);
	CListBoxItemGL* pDeviceItem = new CListBoxItemGL(GetResourceString(IDS_DEVICE_MGR),NULL ,NULL, 30, 0);
	pDeviceItem->SetIcon(&m_iconDevice,&m_iconDeviceFocus, 16, 0);
	pDeviceItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, -16, 0);
	m_boxLists.AddItem(pDeviceItem,BT_HOMEPAGE_DEVICE);


	//list:
	memset(&box,0,sizeof(box));
	//find param:
	count=(g_iClientHeight-cy)/cy;
	if((g_iClientHeight-cy)%cy > (cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy)/count/2*2;

	box.siClient.cx=cx_box;
	box.siClient.cy=(box.iSpacing * count);

// 	box.iSpacing = g_iClientHeight/6;
// 	box.siClient.cx = 610;
// 	box.siClient.cy = g_iClientHeight;	
	box.siBorder.cx = cx_box;
	box.siBorder.cy = (box.iSpacing * count);

	box.idTexIndexBase=TEXID_066;
	box.idTexIndexFocus=TEXID_242;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_065;
	box.idTexBtnUp=TEXID_066;
	box.idTexBtnDown=TEXID_241;
	box.idTexSliBase=TEXID_060;
	box.idTexSliChannel=TEXID_060;
	box.idTexSliThumb=TEXID_276;
	box.bNoSlider=FALSE;
	box.bNoBorder=FALSE;	
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=TRUE;
	box.pxSlider=-8;
	box.fIndexBaseOffset = 0;

	m_boxDeviceList.Initialize(IDC_BT_BOX_DEVICE_LIST,this,&box,pSkinManager);
	m_boxDeviceList.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxDeviceList.SetPos(cx/2,cy/2);
	m_boxDeviceList.LinkTextGL(pTextGL);
	m_boxDeviceList.Show(TRUE, FALSE);
	m_boxDeviceList.SetAlpha(1.0f);
	m_boxDeviceList.SetCaptioniHeight(22,20,14);
	m_boxDeviceList.SetCaptionColor(&cr_gray,&cr_w);
	m_boxDeviceList.SetCaptioniOffset(-20,0,50);


	//Contact:
	m_boxContact.Initialize(IDC_BT_BOX_CONTACT,this,&box,pSkinManager);
	m_boxContact.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxContact.SetPos(-g_iClientWidth/2+cx+cx_box/2,-cy/2);
	m_boxContact.LinkTextGL(pTextGL);
	m_boxContact.SetAlpha(1.0f);
	m_boxContact.SetCaptionColor(&cr_gray, &cr_w);
	m_boxContact.SetCaptioniHeight(22);

	m_boxContanctSearch.Initialize(IDC_BT_BOX_CONTANCT_SEARCH,this,&box,pSkinManager);
	m_boxContanctSearch.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxContanctSearch.SetPos(-g_iClientWidth/2+cx+cx_box/2, -cy/2);
	m_boxContanctSearch.LinkTextGL(pTextGL);
	m_boxContanctSearch.SetAlpha(1.0f);
	m_boxContanctSearch.SetCaptionColor(&cr_gray, &cr_w);
	m_boxContanctSearch.Show(FALSE);

	//find param:
	count=(g_iClientHeight)/cy;
	if((g_iClientHeight)%cy > (cy/2))
		count+=1;
	box.iSpacing=(g_iClientHeight)/count/2*2;
	box.siClient.cx=cx_box;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx = cx_box;
	box.siBorder.cy = (box.iSpacing * count);
	//History
	m_boxHistory.Initialize(IDC_BT_BOX_HISTORY,this,&box,pSkinManager);
	m_boxHistory.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxHistory.SetPos(cx/2,0);
	m_boxHistory.LinkTextGL(pTextGL);
	m_boxHistory.Show(TRUE,FALSE);
	m_boxHistory.SetAlpha(1.0f);
	m_boxHistory.SetCaptionColor(&cr_gray,&cr_w);
	m_boxHistory.SetCaptioniHeight(22,20,18);
	m_boxHistory.SetCaptioniOffset(-20,0,50);

	//contact detail box:
	box.idTexBase=TEXID_603;
	box.siClient.cx=cx_box;
	box.siClient.cy=(box.iSpacing * 3);
	box.siBorder.cx=cx_box;
	box.siBorder.cy=(box.iSpacing * 3);
	box.bQuickSearchBoard=FALSE;
	box.bNoBorder=FALSE;
	box.bNoSlider = FALSE;
	box.pxSlider=-15;
	box.fIndexBaseOffset=60;

	m_boxContactDetail.Initialize(IDC_BT_BOX_CONTACT_DETAIL,this,&box,pSkinManager);
	m_boxContactDetail.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxContactDetail.SetMarginHor(40);
	m_boxContactDetail.SetPos(-g_iClientWidth/2+cx+cx_box/2,-g_iClientHeight/2+box.iSpacing * 3/2);
	m_boxContactDetail.LinkTextGL(pTextGL);
	m_boxContactDetail.Show(FALSE,FALSE);
	m_boxContactDetail.EnableMouseMove(FALSE);
	m_boxContactDetail.SetCaptionColor(&cr_w,&cr_w);

	//
	COLORGL cr_black={0.0,0.0,0.0,1.0};
	COLORGL cr_key={1.0f,1.0f,1.0f,1.0};
	COLORGL cr_sub_key={128.0f/256.0f,128.0f/256.0f,128.0f/256.0f,1.0};
	COLORGL cr_key_down={1.0,1.0,1.0,1.0};


	CTexture *pUp=pSkinManager->GetTexture(TEXID_068);
	CTexture *pDown=pSkinManager->GetTexture(TEXID_043);

	int key_margin_x=10;
	int key_margin_y=4;
	int dele_margin = 53;
	int box_width = g_iClientWidth-cx;
	int box_height = g_iClientHeight;
	int margin_x1 = 155;
	int margin_x2 = 295;
	int margin_x3 = 435;

	int cx_key=(g_iClientWidth-cx-key_margin_x*2)/5;
	int cy_key=(g_iClientHeight-key_margin_y*2)/5;

	SIZE si_key={cx_key-4,cy_key-4};
	GLfloat px=-g_iClientWidth/2+cx/2+key_margin_x*2+cx_key*1.5;
	GLfloat py=g_iClientHeight/2-key_margin_y-1.5f*cy_key +10;

	SIZE si_dial={cx_key*3-4,cy_key-4+8};

	SIZE siInfo2={cx_key*5/2, cy};

	m_tagDialString.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_066),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagDialString.SetSize(&si_btn);
	m_tagDialString.LinkTextGL(pTextGL);
	m_tagDialString.SetPos((-g_iClientWidth + box_width)/2 + cx + 2, (g_iScreenHeight - cy)/2);
	m_tagDialString.SetCaptionHeight(36);
	m_tagDialString.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagDialString.SetCaptionColor(&cr_w, &cr_w);

	m_tagKeyboardBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagKeyboardBk.SetSize(g_iClientWidth-cx, g_iClientHeight);
	m_tagKeyboardBk.SetPos(cx/2, 0);

	m_btnDelete.Initialize(IDC_BT_BTN_KEY_DEL,this,m_pSkinManager->GetTexture(TEXID_1104),NULL,
		m_pSkinManager->GetTexture(TEXID_1103),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDelete.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnDelete.SetPos(g_iClientWidth/2 - dele_margin, (g_iScreenHeight - cy)/2);
	m_btnDelete.SetShowScale(1.2f);
	m_btnDelete.SetTouchRate(4.0f,4.0f);
	m_btnDelete.EnableHoldNotify(TRUE);
	m_btnDelete.Show(FALSE);

	m_iconDial.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1105));
	m_btnDial.Initialize(IDC_BT_BTN_KEY_DIAL,this,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		m_pSkinManager->GetTexture(TEXID_1102),NULL,
		m_pSkinManager->GetTexture(TEXID_1101),NULL,
		NULL,NULL);
	m_btnDial.SetSize(&si_btn);
	m_btnDial.SetIcon(&m_iconDial);
	m_btnDial.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnDial.SetPos(cx/2, -g_iClientHeight/2+cy/2);

	int posX1 = (-g_iClientWidth)/2 + cx + margin_x1;
	int posX2 = (-g_iClientWidth)/2 + cx + margin_x2;
	int posX3 = (-g_iClientWidth)/2 + cx + margin_x3;
	int posY1 = (g_iScreenHeight - cy)/2 - cy+10;
	int posY2 = (g_iScreenHeight - cy)/2 - 2*cy+10;
	int posY3 = (g_iScreenHeight - cy)/2 - 3*cy+10;
	int posY4 = (g_iScreenHeight - cy)/2 - 4*cy+10;

	//1-3
	m_btnNum1.Initialize(IDC_BT_BTN_KEY_1,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum1.SetCaption(L"1");
	m_btnNum1.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum1.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum1.SetCaptionHeight(28);
	m_btnNum1.LinkTextGL(pTextGL);
	m_btnNum1.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum1.SetPos(posX1, posY1-10);
	m_btnNum1.SetShowScale(1.2f);
	m_btnNum1.EnableTouchDownNotify(TRUE);

	m_tagLine1.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine1.SetSize(120, cy);
	m_tagLine1.LinkTextGL(pTextGL);
	m_tagLine1.SetPos(posX1, posY1 - 10);

	m_btnNum2.Initialize(IDC_BT_BTN_KEY_2,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum2.SetCaption(L"2");
	m_btnNum2.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum2.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum2.SetCaptionHeight(28);
	m_btnNum2.LinkTextGL(pTextGL);
	m_btnNum2.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum2.SetPos(posX2, posY1-10);
	m_btnNum2.SetShowScale(1.2f);
	m_btnNum2.EnableTouchDownNotify(TRUE);

	m_tagNum2.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagNum2.SetCaption(_T("ABC"));
	m_tagNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum2.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum2.SetCaptionHeight(12);
	m_tagNum2.LinkTextGL(pTextGL);
	m_tagNum2.SetPos(posX2, posY1 - 30);
	m_tagNum2.SetSize(50, 20);

	m_tagLine2.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine2.SetSize(120, cy);
	m_tagLine2.LinkTextGL(pTextGL);
	m_tagLine2.SetPos(posX2, posY1 - 10);

	m_btnNum3.Initialize(IDC_BT_BTN_KEY_3,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum3.SetCaption(L"3");
	m_btnNum3.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum3.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum3.SetCaptionHeight(28);
	m_btnNum3.LinkTextGL(pTextGL);
	m_btnNum3.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum3.SetPos(posX3, posY1-10);
	m_btnNum3.SetShowScale(1.2f);
	m_btnNum3.EnableTouchDownNotify(TRUE);

	m_tagNum3.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum3.SetCaption(L"DEF");
	m_tagNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum3.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum3.SetCaptionHeight(12);
	m_tagNum3.LinkTextGL(pTextGL);
	m_tagNum3.SetPos(posX3, posY1 - 30);
	m_tagNum3.SetSize(50,20);

	m_tagLine3.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine3.SetSize(120, cy);
	m_tagLine3.LinkTextGL(pTextGL);
	m_tagLine3.SetPos(posX3, posY1 - 10);


	//4-6:
	m_btnNum4.Initialize(IDC_BT_BTN_KEY_4,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum4.SetCaption(L"4");
	m_btnNum4.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum4.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum4.SetCaptionHeight(28);
	m_btnNum4.LinkTextGL(pTextGL);
	m_btnNum4.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum4.SetPos(posX1, posY2-10);
	m_btnNum4.SetShowScale(1.2f);
	m_btnNum4.EnableTouchDownNotify(TRUE);

	m_tagNum4.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum4.SetCaption(L"GHI");
	m_tagNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum4.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum4.SetCaptionHeight(12);
	m_tagNum4.LinkTextGL(pTextGL);
	m_tagNum4.SetPos(posX1, posY2-30);
	m_tagNum4.SetSize(50,20);

	m_tagLine4.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine4.SetSize(120, cy);
	m_tagLine4.LinkTextGL(pTextGL);
	m_tagLine4.SetPos(posX1, posY2 - 10);

	m_btnNum5.Initialize(IDC_BT_BTN_KEY_5,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum5.SetCaption(L"5");
	m_btnNum5.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum5.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum5.SetCaptionHeight(28);
	m_btnNum5.LinkTextGL(pTextGL);
	m_btnNum5.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum5.SetPos(posX2, posY2-10);
	m_btnNum5.SetShowScale(1.2f);
	m_btnNum5.EnableTouchDownNotify(TRUE);

	m_tagNum5.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum5.SetCaption(L"JKL");
	m_tagNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum5.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum5.SetCaptionHeight(12);
	m_tagNum5.LinkTextGL(pTextGL);
	m_tagNum5.SetPos(posX2, posY2-30);
	m_tagNum5.SetSize(50,20);

	m_tagLine5.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine5.SetSize(120, cy);
	m_tagLine5.LinkTextGL(pTextGL);
	m_tagLine5.SetPos(posX2, posY2 - 10);

	m_btnNum6.Initialize(IDC_BT_BTN_KEY_6,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum6.SetCaption(L"6");
	m_btnNum6.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum6.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum6.SetCaptionHeight(28);
	m_btnNum6.LinkTextGL(pTextGL);
	m_btnNum6.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum6.SetPos(posX3, posY2-10);
	m_btnNum6.SetShowScale(1.2f);
	m_btnNum6.EnableTouchDownNotify(TRUE);

	m_tagNum6.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum6.SetCaption(L"MNO");
	m_tagNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum6.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum6.SetCaptionHeight(12);
	m_tagNum6.LinkTextGL(pTextGL);
	m_tagNum6.SetPos(posX3, posY2-30);
	m_tagNum6.SetSize(50,20);

	m_tagLine6.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine6.SetSize(120, cy);
	m_tagLine6.LinkTextGL(pTextGL);
	m_tagLine6.SetPos(posX3, posY2 - 10);

	//7-9:
	m_btnNum7.Initialize(IDC_BT_BTN_KEY_7,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum7.SetCaption(L"7");
	m_btnNum7.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum7.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum7.SetCaptionHeight(28);
	m_btnNum7.LinkTextGL(pTextGL);
	m_btnNum7.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum7.SetPos(posX1, posY3-10);
	m_btnNum7.SetShowScale(1.2f);
	m_btnNum7.EnableTouchDownNotify(TRUE);

	m_tagNum7.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum7.SetCaption(L"PQRS");
	m_tagNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum7.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum7.SetCaptionHeight(12);
	m_tagNum7.LinkTextGL(pTextGL);
	m_tagNum7.SetPos(posX1, posY3-30);
	m_tagNum7.SetSize(50,20);

	m_tagLine7.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine7.SetSize(120, cy);
	m_tagLine7.LinkTextGL(pTextGL);
	m_tagLine7.SetPos(posX1, posY3 - 10);

	m_btnNum8.Initialize(IDC_BT_BTN_KEY_8,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum8.SetCaption(L"8");
	m_btnNum8.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum8.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum8.SetCaptionHeight(28);
	m_btnNum8.LinkTextGL(pTextGL);
	m_btnNum8.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum8.SetPos(posX2, posY3-10);
	m_btnNum8.SetShowScale(1.2f);
	m_btnNum8.EnableTouchDownNotify(TRUE);

	m_tagNum8.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum8.SetCaption(L"TUV");
	m_tagNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum8.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum8.SetCaptionHeight(12);
	m_tagNum8.LinkTextGL(pTextGL);
	m_tagNum8.SetPos(posX2, posY3-30);
	m_tagNum8.SetSize(50,20);

	m_tagLine8.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine8.SetSize(120, cy);
	m_tagLine8.LinkTextGL(pTextGL);
	m_tagLine8.SetPos(posX2, posY3 - 10);

	m_btnNum9.Initialize(IDC_BT_BTN_KEY_9,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum9.SetCaption(L"9");
	m_btnNum9.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnNum9.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum9.SetCaptionHeight(28);
	m_btnNum9.LinkTextGL(pTextGL);
	m_btnNum9.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum9.SetPos(posX3, posY3-10);
	m_btnNum9.SetShowScale(1.2f);
	m_btnNum9.EnableTouchDownNotify(TRUE);

	m_tagNum9.Initialize(IDC_BT_BTN_KEY_9,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum9.SetCaption(L"WXYZ");
	m_tagNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum9.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum9.SetCaptionHeight(12);
	m_tagNum9.LinkTextGL(pTextGL);
	m_tagNum9.SetPos(posX3, posY3-30);
	m_tagNum9.SetSize(50,20);

	m_tagLine9.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagLine9.SetSize(120, cy);
	m_tagLine9.LinkTextGL(pTextGL);
	m_tagLine9.SetPos(posX3, posY3 - 10);

	//*0#:
	m_btnStar.Initialize(IDC_BT_BTN_KEY_STAR,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnStar.SetCaption(L"*");
	m_btnStar.SetCaptionPos(BTN_CAPTION_CENTER,0,4);
	m_btnStar.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnStar.SetCaptionHeight(28);
	m_btnStar.LinkTextGL(pTextGL);
	m_btnStar.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnStar.SetPos(posX1, posY4-10);
	m_btnStar.SetShowScale(1.2f);
	m_btnStar.EnableTouchDownNotify(TRUE);
	m_btnStar.EnableLongClickNotify(TRUE);

	m_btnNum0.Initialize(IDC_BT_BTN_KEY_0,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum0.SetCaption(L"0");
	m_btnNum0.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	//m_btnNum0.SetSubCaption(L"+");
	//m_btnNum0.SetSubCaptionPos(BTN_CAPTION_CENTER,0,/*10*/14);
	m_btnNum0.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum0.SetCaptionHeight(28);
	m_btnNum0.LinkTextGL(pTextGL);
	m_btnNum0.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum0.SetPos(posX2, posY4-10);
	m_btnNum0.SetShowScale(1.2f);
	m_btnNum0.EnableTouchDownNotify(TRUE);
	m_btnNum0.EnableLongClickNotify(TRUE);

	m_tagNum0.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum0.SetCaption(L"+");
	m_tagNum0.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum0.SetCaptionColor(&cr_sub_key,&cr_key_down);
	m_tagNum0.SetCaptionHeight(12);
	m_tagNum0.LinkTextGL(pTextGL);
	m_tagNum0.SetPos(posX2, posY4-30);
	m_tagNum0.SetSize(50,20);

	m_btnWell.Initialize(IDC_BT_BTN_KEY_WELL,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnWell.SetCaption(L"#");
	m_btnWell.SetCaptionPos(BTN_CAPTION_CENTER,0,10);
	m_btnWell.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnWell.SetCaptionHeight(28);
	m_btnWell.LinkTextGL(pTextGL);
	m_btnWell.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnWell.SetPos(posX3, posY4-10);
	m_btnWell.SetShowScale(1.2f);
	m_btnWell.EnableTouchDownNotify(TRUE);

#else
	m_iconTypeHome.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_549));
	m_iconTypeMobile.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_548));
	m_iconTypeWork.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_547));
	m_iconTypeDefault.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_518));

	m_iconOut.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_552));
	m_iconIn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_550));
	m_iconMissed.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_551));


	//list:
	m_iconDevice.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_504));
    m_iconDevice.SetShowScale(0.8f);
	m_iconHistory.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_506));
    m_iconHistory.SetShowScale(0.8f);
	m_iconContact.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_508));
    m_iconContact.SetShowScale(0.8f);
	m_iconDelete.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_510));
    m_iconDelete.SetShowScale(0.8f);

	m_iconDeviceFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_505));
    m_iconDeviceFocus.SetShowScale(0.8f);
	m_iconHistoryFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_507));
    m_iconHistoryFocus.SetShowScale(0.8f);
	m_iconContactFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_509));
    m_iconContactFocus.SetShowScale(0.8f);
	m_iconDeleteFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_511));
    m_iconDeleteFocus.SetShowScale(0.8f);
	m_iconKeyboard.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_544));
	m_iconKeyboard.SetShowScale(0.8f);
	m_iconKeyboardFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_545));
	m_iconKeyboardFocus.SetShowScale(0.8f);

	COLORGL cr_gray={164.0f/256.0f,164.0f/256.0f,164.0f/256.0f,1.0};
	COLORGL cr_red={255.0f/256.0f,16.0f/256.0f,62.0f/256.0f,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};

	int cx=LIST_BOX_ITEM_WIDTH;//-20; //220
	int cy=LIST_BOX_ITEM_HEIGHT;//100

	SIZE sib={cx,cy};

	//
	m_iconSearching.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconSearching.SetShowScale(1.2f);
	m_iconSearching.SetPos(cx/2,cy/2);
	m_iconBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_121));
	m_iconBT.SetShowScale(1.7f);
	m_iconBT.SetPos(cx/2,-cy/2);

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_502));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_503));

	m_btnHome.Initialize(IDC_BT_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		m_pSkinManager->GetTexture(TEXID_604),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sib);
	m_btnHome.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_b);
	m_btnHome.SetCaptionHeight(22);
	m_btnHome.SetIconPos(-cx/2+31,0);

	//
	SIZE si_btn={(g_iClientWidth-cx),cy+0};

/*
	m_iconPairedDeviceUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_504));
	m_iconPairedDeviceDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_530));

	m_btnPairedDevice.Initialize(IDC_BT_BTN_PAIRED_DEVICE,this,m_pSkinManager->GetTexture(TEXID_066),NULL,
		m_pSkinManager->GetTexture(TEXID_067),NULL,
		m_pSkinManager->GetTexture(TEXID_066),NULL,
		NULL,NULL);
	m_btnPairedDevice.SetSize(&sic);
	m_btnPairedDevice.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnPairedDevice.SetPos(-(g_iClientWidth-cx)/4+cx/2,g_iClientHeight/2-cy/2);
	m_btnPairedDevice.LinkTextGL(pTextGL);

	m_btnPairedDevice.SetIcon(&m_iconPairedDeviceUp,&m_iconPairedDeviceDown);
	m_btnPairedDevice.SetCaption(GetResourceString(IDS_PAIRED));
	m_btnPairedDevice.SetCaptionPos(BTN_CAPTION_CENTER,0,-28);
	m_btnPairedDevice.SetCaptionColor(&cr_gray,&cr_red);
	m_btnPairedDevice.SetCaptionHeight(18);
	m_btnPairedDevice.SetIconPos(0,10);
*/

	m_iconSearchDeviceUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_013));
	m_iconSearchDeviceDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_019));

	m_btnSearchDevice.Initialize(IDC_BT_BTN_SEARCHDEVICE,this,m_pSkinManager->GetTexture(TEXID_066),NULL,
		m_pSkinManager->GetTexture(TEXID_067),NULL,
		m_pSkinManager->GetTexture(TEXID_066),NULL,
		NULL,NULL);
	m_btnSearchDevice.SetSize(&si_btn);
	m_btnSearchDevice.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnSearchDevice.SetPos(/*(g_iClientWidth-cx)/4+*/cx/2,-g_iClientHeight/2+cy/2);
	m_btnSearchDevice.LinkTextGL(pTextGL);

	m_btnSearchDevice.SetIcon(&m_iconSearchDeviceUp,&m_iconSearchDeviceDown);
	m_btnSearchDevice.SetCaption(GetResourceString(IDS_SEARCH));
// 	m_btnSearchDevice.SetCaptionPos(BTN_CAPTION_CENTER,0,-28);
// 	m_btnSearchDevice.SetCaptionColor(&cr_gray,&cr_red);
// 	m_btnSearchDevice.SetCaptionHeight(18);
// 	m_btnSearchDevice.SetIconPos(0,10);
	m_btnSearchDevice.SetCaptionPos(BTN_CAPTION_LEFT,(g_iClientWidth-cx)/2-10,0);
	m_btnSearchDevice.SetCaptionColor(&cr_w,&cr_red);
	m_btnSearchDevice.SetCaptionHeight(28);
	m_btnSearchDevice.SetIconPos(-50,0);


	//
	int cx_box=(g_iScreenWidth-cx);

	SIZE si0={cx_box-20,80};

	//download
	m_btnDownloadPhonebook.Initialize(IDC_BT_BTN_CONTACT_DOWNLOAD,this,m_pSkinManager->GetTexture(TEXID_513),NULL,
		m_pSkinManager->GetTexture(TEXID_512),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDownloadPhonebook.SetCaption(GetResourceString(IDS_DOWNLOAD));
	m_btnDownloadPhonebook.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDownloadPhonebook.SetCaptionHeight(28);
	m_btnDownloadPhonebook.LinkTextGL(pTextGL);
	m_btnDownloadPhonebook.SetSize(&si0);
	m_btnDownloadPhonebook.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnDownloadPhonebook.SetPos(cx/2,g_iClientHeight/2-60);

	m_btnCancelDownloadPhonebook.Initialize(IDC_BT_BTN_CONTACT_CANCEL_DOWNLOAD,this,m_pSkinManager->GetTexture(TEXID_553),NULL,
		m_pSkinManager->GetTexture(TEXID_512),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancelDownloadPhonebook.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancelDownloadPhonebook.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancelDownloadPhonebook.SetCaptionHeight(28);
	m_btnCancelDownloadPhonebook.LinkTextGL(pTextGL);
	m_btnCancelDownloadPhonebook.SetSize(&si0);
	m_btnCancelDownloadPhonebook.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnCancelDownloadPhonebook.SetPos(cx/2,g_iClientHeight/2-60);
	m_btnCancelDownloadPhonebook.Show(FALSE);

#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	//download CallLog button
	m_btnDownloadCallLog.Initialize(IDC_BT_BTN_CONTACT_DOWNLOAD,this,m_pSkinManager->GetTexture(TEXID_513),NULL,
		m_pSkinManager->GetTexture(TEXID_512),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDownloadCallLog.SetCaption(GetResourceString(IDS_DOWNLOAD));
	m_btnDownloadCallLog.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDownloadCallLog.SetCaptionHeight(28);
	m_btnDownloadCallLog.LinkTextGL(pTextGL);
	m_btnDownloadCallLog.SetSize(&si0);
	m_btnDownloadCallLog.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnDownloadCallLog.SetPos(cx/2,g_iClientHeight/2-60);

	m_btnCancelDownloadCallLog.Initialize(IDC_BT_DOWNLOAD_CANCEL_CALLHISTORY,this,m_pSkinManager->GetTexture(TEXID_553),NULL,
		m_pSkinManager->GetTexture(TEXID_512),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCancelDownloadCallLog.SetCaption(GetResourceString(IDS_CANCEL));
	m_btnCancelDownloadCallLog.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnCancelDownloadCallLog.SetCaptionHeight(28);
	m_btnCancelDownloadCallLog.LinkTextGL(pTextGL);
	m_btnCancelDownloadCallLog.SetSize(&si0);
	m_btnCancelDownloadCallLog.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnCancelDownloadCallLog.SetPos(cx/2,g_iClientHeight/2-60);
	m_btnCancelDownloadCallLog.Show(FALSE);
#endif

	SIZE si1={cx_box-20-150,80};
	SIZE si2={150,80};

	m_iconSearchContactUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_013));

	m_btnSearchContact.Initialize(IDC_BT_BTN_SEARCH_CONTACT,this,m_pSkinManager->GetTexture(TEXID_512),NULL,
		m_pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
// 	m_btnSearchContact.SetCaption(GetResourceString(IDS_DOWNLOAD));
	m_btnSearchContact.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnSearchContact.SetCaptionHeight(32);
	m_btnSearchContact.LinkTextGL(pTextGL);
	m_btnSearchContact.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
#if BT_PHONEBOOK_AUTOSTORE
	m_btnSearchContact.SetSize(&si1);
	m_btnSearchContact.SetPos(cx/2+si2.cx/2,g_iClientHeight/2-45);
#else
	m_btnSearchContact.SetSize(&si0);
	m_btnSearchContact.SetPos(cx/2,g_iClientHeight/2-45);
#endif
	m_btnSearchContact.SetIcon(&m_iconSearchContactUp);
	m_btnSearchContact.Show(FALSE);

#if BT_PHONEBOOK_AUTOSTORE
	m_btnSyncContact.Initialize(IDC_BT_BTN_SYNC_CONTACT,this,m_pSkinManager->GetTexture(TEXID_512),NULL,
		m_pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSyncContact.SetCaption(GetResourceString(IDS_BT_PHONEBOOK_SYNC));
	m_btnSyncContact.SetCaptionPos(BTN_CAPTION_CENTER);

	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_RUSSIAN:
		m_btnSyncContact.SetCaptionHeight(14);
		break;
	default:
		m_btnSyncContact.SetCaptionHeight(28);
		break;
	}

	m_btnSyncContact.LinkTextGL(pTextGL);
	m_btnSyncContact.SetSize(&si2);
	m_btnSyncContact.SetNotifyFunc(& CPanelBluetoothMain::OnNotify,this);
	m_btnSyncContact.SetPos(-g_iClientWidth/2+cx+10+si2.cx/2,g_iClientHeight/2-45);
	//m_btnSyncContact.SetIcon(&m_iconSearchContactUp);
	m_btnSyncContact.Show(FALSE);
#endif

	//
	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));

	//find param:
	int count=(g_iClientHeight-cy)/cy;
	if((g_iClientHeight-cy)%(cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy)/count;///2*2;

	box.siClient.cx=cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=cx;
	box.siBorder.cy=g_iClientHeight-cy;

	box.idTexIndexBase=TEXID_603;
	box.idTexIndexFocus=TEXID_604;
	box.idTexBase=TEXID_603;
	box.bNoSlider=TRUE;
	box.bNoBorder=FALSE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxLists.Initialize(IDC_BT_BOX_MENU,this,&box,pSkinManager);
	m_boxLists.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxLists.SetPos(-g_iClientWidth/2+cx/2,cy/2);
	m_boxLists.LinkTextGL(pTextGL);
	m_boxLists.SetAlpha(1.0f);

    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
        case LANGUAGE_RUSSIAN:
		case LANGUAGE_PERSIAN:
	        m_boxLists.SetCaptioniHeight(20);
            break;
		case LANGUAGE_SPANISH:
        case LANGUAGE_ITALIAN:
            m_boxLists.SetCaptioniHeight(22);
            break;
        default:
	        m_boxLists.SetCaptioniHeight(24);
            break;
    }

	m_boxLists.SetCaptionColor(&cr_gray,&cr_b);

	CListBoxItemGL *pItem;
	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_MGR));
	pItem->SetIcon(&m_iconDevice,&m_iconDeviceFocus);
	m_boxLists.AddItem(pItem,BT_HOMEPAGE_DEVICE);
//  	pItem=new CListBoxItemGL(GetResourceString(IDS_CALL_HISTORY));
//  	pItem->SetIcon(&m_iconHistory,&m_iconHistoryFocus);
//  	m_boxLists.AddItem(pItem,BT_HOMEPAGE_HISTORY);
//  	pItem=new CListBoxItemGL(GetResourceString(IDS_CONTACT));
//  	pItem->SetIcon(&m_iconContact,&m_iconContactFocus);
//  	m_boxLists.AddItem(pItem,BT_HOMEPAGE_CONTACT);


	//list:
	memset(&box,0,sizeof(box));

	//find param:
	count=(g_iClientHeight-cy)/cy;
	if((g_iClientHeight-cy)%cy > (cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy)/count/2*2;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=g_iClientHeight-cy;//(box.iSpacing * count);


	box.idTexIndexBase=TEXID_077;//TEXID_606;//
	box.idTexIndexFocus=TEXID_077;//TEXID_607;//
	box.idTexBase=TEXID_066;//TEXID_301;//
	box.idTexIndexClick=TEXID_513;
	box.bNoSlider=FALSE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=FALSE;
	box.pxSlider=-20;
	box.fIndexBaseOffset=60;

	m_boxDeviceList.Initialize(IDC_BT_BOX_DEVICE_LIST,this,&box,pSkinManager);
	m_boxDeviceList.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxDeviceList.SetMarginHor(20);
	m_boxDeviceList.SetPos(cx/2,cy/2);
	m_boxDeviceList.LinkTextGL(pTextGL);
	m_boxDeviceList.SetAlpha(1.0f);
	m_boxDeviceList.SetCaptioniHeight(28,20,14);
	m_boxDeviceList.SetCaptionColor(&cr_b,&cr_b,&cr_w);

	//paired device:
// 	m_boxDevicePaired.Initialize(IDC_BT_BOX_DEVICE_PAIRED,this,&box,pSkinManager);
// 	m_boxDevicePaired.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
// 	m_boxDevicePaired.SetMarginHor(40);
// 	m_boxDevicePaired.SetPos(cx/2,-cy/2);
// 	m_boxDevicePaired.LinkTextGL(pTextGL);
// 	m_boxDevicePaired.SetAlpha(1.0f);
// 	m_boxDevicePaired.SetCaptioniHeight(32,24,18);
//  	m_boxDevicePaired.SetCaptionColor(&cr_b,&cr_b);

	//test:
//  	pItem=new CListBoxItemGL(_T("wangyiping's Window Phone "),NULL,_T("Paired"));
//  	pItem->SetIcon(&m_iconDeviceFocus);
//  	pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
//  	m_boxDevicePaired.AddItem(pItem);
// 	pItem=new CListBoxItemGL(_T("wangyiping's Window Phone "),NULL,_T("Paired"));
// 	pItem->SetIcon(&m_iconDeviceFocus);
// 	pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
// 	m_boxDevicePaired.AddItem(pItem);


	//paired device:
// 	m_boxDeviceSearched.Initialize(IDC_BT_BOX_DEVICE_SEARCHED,this,&box,pSkinManager);
// 	m_boxDeviceSearched.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
// 	m_boxDeviceSearched.SetMarginHor(40);
// 	m_boxDeviceSearched.SetPos(cx/2,-cy/2);
// 	m_boxDeviceSearched.LinkTextGL(pTextGL);
// 	m_boxDeviceSearched.SetAlpha(1.0f);
// 	m_boxDeviceSearched.SetCaptioniHeight(32,24,18);
// 	m_boxDeviceSearched.SetCaptionColor(&cr_b,&cr_b);

 	//test:
//  	pItem=new CListBoxItemGL(_T("Window Phone "),NULL,_T("unPaired"));
//  	pItem->SetIcon(&m_iconDeviceFocus);
//  	m_boxDeviceSearched.AddItem(pItem);
// 	pItem=new CListBoxItemGL(_T("Window Phone "),NULL,_T("unPaired"));
// 	pItem->SetIcon(&m_iconDeviceFocus);
// 	m_boxDeviceSearched.AddItem(pItem);

	//
	//find param:
	count=(g_iClientHeight-20)/cy;
	if((g_iClientHeight-20)%cy > (cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-20)/count/2*2;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=g_iClientHeight;//(box.iSpacing * count);
	box.fIndexBaseOffset=60;

	//History
	m_boxHistory.Initialize(IDC_BT_BOX_HISTORY,this,&box,pSkinManager);
	m_boxHistory.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxHistory.SetMarginHor(40);
	m_boxHistory.SetPos(cx/2,0);
	m_boxHistory.LinkTextGL(pTextGL);
	m_boxHistory.SetAlpha(1.0f);
 	m_boxHistory.SetCaptionColor(&cr_b,&cr_b,&cr_w);
	m_boxHistory.SetCaptioniHeight(26,20,18);

	//test:
//  		for (int i=0;i<32;i++)
//  		{
//  			pItem=new CListBoxItemGL(_T("wangyiping "),_T("13888888888"),_T("2012-09-03 12:22:23"));
//  			pItem->SetIcon(&m_iconIn);
//  			m_boxHistory.AddItem(pItem);
//
//  		}



// 	int cx_box=340;
// 	box.siClient.cx=cx_box;
// 	box.siClient.cy=(box.iSpacing * count);
// 	box.siBorder.cx=cx_box;
// 	box.siBorder.cy=g_iClientHeight;//(box.iSpacing * count);

	//find param:
	count=(g_iClientHeight-cy)/65;
	if((g_iClientHeight-cy)%65 > 32)
		count+=1;

	box.iSpacing=(g_iClientHeight-cy)/count/2*2;

	box.siClient.cx=cx_box;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=cx_box;
	box.siBorder.cy=g_iClientHeight-cy;//(box.iSpacing * count);
	box.bQuickSearchBoard=TRUE;
	box.fIndexBaseOffset=60;//15;

	//Contact:
	m_boxContact.Initialize(IDC_BT_BOX_CONTACT,this,&box,pSkinManager);
	m_boxContact.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxContact.SetMarginHor(40);
	m_boxContact.SetPos(-g_iClientWidth/2+cx+cx_box/2,-42);
	m_boxContact.LinkTextGL(pTextGL);
	m_boxContact.SetAlpha(1.0f);
	m_boxContact.SetCaptionColor(&cr_b,&cr_b,&cr_w);
	m_boxContact.SetCaptioniHeight(26);

	m_boxContanctSearch.Initialize(IDC_BT_BOX_CONTANCT_SEARCH,this,&box,pSkinManager);
	m_boxContanctSearch.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxContanctSearch.SetMarginHor(40);
	m_boxContanctSearch.SetPos(-g_iClientWidth/2+cx+cx_box/2,-42);
	m_boxContanctSearch.LinkTextGL(pTextGL);
	m_boxContanctSearch.SetAlpha(1.0f);
	m_boxContanctSearch.SetCaptionColor(&cr_b,&cr_b);
	m_boxContanctSearch.Show(FALSE);

//  	for (int i=0;i<32;i++)
//  	{
//  		wchar_t str[32];
//  		swprintf_s(str,32,_T("%cYiping Wang"),'z'-i);
//  		pItem=new CListBoxItemGL(str,NULL,NULL);
//  		pItem->SetIcon(&m_iconContactFocus);
//  		m_boxContact.AddItemByOrder(pItem);
//  	}

	//contact detail:
	//box.iSpacing=(g_iClientHeight-20)/count;

	box.idTexBase=TEXID_603;
	box.siClient.cx=cx_box;
	box.siClient.cy=(box.iSpacing * 3);
	box.siBorder.cx=cx_box;
	box.siBorder.cy=(box.iSpacing * 3);
	box.bQuickSearchBoard=FALSE;
	box.bNoBorder=FALSE;
    box.bNoSlider = FALSE;
	box.pxSlider=-15;
	box.fIndexBaseOffset=60;


	m_boxContactDetail.Initialize(IDC_BT_BOX_CONTACT_DETAIL,this,&box,pSkinManager);
	m_boxContactDetail.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_boxContactDetail.SetMarginHor(40);
	m_boxContactDetail.SetPos(-g_iClientWidth/2+cx+cx_box/2,-g_iClientHeight/2+box.iSpacing * 3/2);
	m_boxContactDetail.LinkTextGL(pTextGL);
	m_boxContactDetail.Show(FALSE,FALSE);
	m_boxContactDetail.EnableMouseMove(FALSE);
 	m_boxContactDetail.SetCaptionColor(&cr_w,&cr_w);

	//
	COLORGL cr_black={0.0,0.0,0.0,1.0};
	COLORGL cr_key={0.0,0.0,0.0,1.0};
	COLORGL cr_key_down={1.0,1.0,1.0,1.0};


	CTexture *pUp=pSkinManager->GetTexture(TEXID_514);
	CTexture *pDown=pSkinManager->GetTexture(TEXID_515);

	int key_margin_x=10;
	int key_margin_y=4;

// 	int cx_key=(g_iClientWidth-cx/*-cx_box*/-key_margin_x*2)/3;
// 	int cy_key=(g_iClientHeight-key_margin_y*2)/6;
	int cx_key=(g_iClientWidth-cx/*-cx_box*/-key_margin_x*2)/5;
	int cy_key=(g_iClientHeight-key_margin_y*2)/5;

	SIZE si_key={cx_key-4,cy_key-4};
// 	GLfloat px=g_iClientWidth/2-key_margin_x-2.5f*cx_key;
	GLfloat px=-g_iClientWidth/2+cx/2+key_margin_x*2+cx_key*1.5;
	GLfloat py=g_iClientHeight/2-key_margin_y-1.5f*cy_key +10;

	SIZE si_dial={cx_key*3-4,cy_key-4+8};

	SIZE siInfo2={cx_key*5/2,cy_key-4};

	m_tagDialString.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDialString.SetSize(&siInfo2);
	//m_tagDialString.SetCaption(_T("13870204516888888"));
	m_tagDialString.LinkTextGL(pTextGL);
	m_tagDialString.SetPos(g_iClientWidth/2-key_margin_x-62-cx_key*5/4,py+cy_key);
	m_tagDialString.SetCaptionHeight(40);
	m_tagDialString.EnableDrawBack(FALSE);
	m_tagDialString.SetCaptionPos(BTN_CAPTION_RIGHT);
 	m_tagDialString.SetCaptionColor(&cr_b);

	m_tagKeyboardBk.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_603),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagKeyboardBk.SetSize(g_iClientWidth-cx-cx_box,g_iClientHeight);
	m_tagKeyboardBk.SetPos(px+cx_key,0);

	m_btnDelete.Initialize(IDC_BT_BTN_KEY_DEL,this,m_pSkinManager->GetTexture(TEXID_519),NULL,
		m_pSkinManager->GetTexture(TEXID_520),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnDelete.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnDelete.SetPos(g_iClientWidth/2-key_margin_x-28,py+cy_key);
	m_btnDelete.SetTouchRate(1.5f,1.5f);
	m_btnDelete.EnableHoldNotify(TRUE);
	m_btnDelete.Show(FALSE);

	m_iconDial.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_518));
	m_btnDial.Initialize(IDC_BT_BTN_KEY_DIAL,this,m_pSkinManager->GetTexture(TEXID_516),NULL,
		m_pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnDial.SetSize(&si_dial);
	m_btnDial.SetIcon(&m_iconDial);
	m_btnDial.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);

	m_btnDial.SetPos(px+cx_key*3+40,py-cy_key*1.5f);
	m_btnDial.SetShowScale(1.0f);

	//1-3
	m_btnNum1.Initialize(IDC_BT_BTN_KEY_1,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum1.SetCaption(L"1");
	m_btnNum1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum1.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum1.SetCaptionHeight(40);
	m_btnNum1.LinkTextGL(pTextGL);
	//m_btnNum1.SetSize(&si_key);
	m_btnNum1.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum1.SetPos(px,py);
	m_btnNum1.SetShowScale(0.72f);
    m_btnNum1.EnableTouchDownNotify(TRUE);

	m_btnNum2.Initialize(IDC_BT_BTN_KEY_2,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum2.SetCaption(L"2");
	m_btnNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum2.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum2.SetCaptionHeight(40);
	m_btnNum2.LinkTextGL(pTextGL);
	//m_btnNum2.SetSize(&si_key);
	m_btnNum2.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum2.SetPos(px+cx_key,py);
	m_btnNum2.SetShowScale(0.72f);
    m_btnNum2.EnableTouchDownNotify(TRUE);

	m_tagNum2.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum2.SetCaption(L"ABC");
	m_tagNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum2.SetCaptionColor(&cr_key,&cr_key_down);
	m_tagNum2.SetCaptionHeight(12);
	m_tagNum2.LinkTextGL(pTextGL);
	m_tagNum2.SetPos(px+cx_key,py-30);
	m_tagNum2.SetSize(50,20);
	m_tagNum2.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);

	m_btnNum3.Initialize(IDC_BT_BTN_KEY_3,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum3.SetCaption(L"3");
	m_btnNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum3.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum3.SetCaptionHeight(40);
	m_btnNum3.LinkTextGL(pTextGL);
	//m_btnNum3.SetSize(&si_key);
	m_btnNum3.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum3.SetPos(px+cx_key*2,py);
	m_btnNum3.SetShowScale(0.72f);
    m_btnNum3.EnableTouchDownNotify(TRUE);

	m_tagNum3.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum3.SetCaption(L"DEF");
	m_tagNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum3.SetCaptionColor(&cr_key,&cr_key_down);
	m_tagNum3.SetCaptionHeight(12);
	m_tagNum3.LinkTextGL(pTextGL);
	m_tagNum3.SetPos(px+cx_key*2,py-30);
	m_tagNum3.SetSize(50,20);
	m_tagNum3.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	//4-6:
	m_btnNum4.Initialize(IDC_BT_BTN_KEY_4,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum4.SetCaption(L"4");
	m_btnNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum4.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum4.SetCaptionHeight(40);
	m_btnNum4.LinkTextGL(pTextGL);
	//m_btnNum4.SetSize(&si_key);
	m_btnNum4.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum4.SetPos(px,py-cy_key);
	m_btnNum4.SetShowScale(0.72f);
    m_btnNum4.EnableTouchDownNotify(TRUE);

	m_tagNum4.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum4.SetCaption(L"GHI");
	m_tagNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum4.SetCaptionColor(&cr_key,&cr_key_down);
	m_tagNum4.SetCaptionHeight(12);
	m_tagNum4.LinkTextGL(pTextGL);
	m_tagNum4.SetPos(px,py-cy_key-30);
	m_tagNum4.SetSize(50,20);
	m_tagNum4.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);


	m_btnNum5.Initialize(IDC_BT_BTN_KEY_5,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum5.SetCaption(L"5");
	m_btnNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum5.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum5.SetCaptionHeight(40);
	m_btnNum5.LinkTextGL(pTextGL);
	//m_btnNum5.SetSize(&si_key);
	m_btnNum5.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum5.SetPos(px+cx_key,py-cy_key);
	m_btnNum5.SetShowScale(0.72f);
    m_btnNum5.EnableTouchDownNotify(TRUE);

	m_tagNum5.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum5.SetCaption(L"JKL");
	m_tagNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum5.SetCaptionColor(&cr_key,&cr_key_down);
	m_tagNum5.SetCaptionHeight(12);
	m_tagNum5.LinkTextGL(pTextGL);
	m_tagNum5.SetPos(px+cx_key,py-cy_key-30);
	m_tagNum5.SetSize(50,20);
	m_tagNum5.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);

	m_btnNum6.Initialize(IDC_BT_BTN_KEY_6,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum6.SetCaption(L"6");
	m_btnNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum6.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum6.SetCaptionHeight(40);
	m_btnNum6.LinkTextGL(pTextGL);
	//m_btnNum6.SetSize(&si_key);
	m_btnNum6.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum6.SetPos(px+cx_key*2,py-cy_key);
	m_btnNum6.SetShowScale(0.72f);
    m_btnNum6.EnableTouchDownNotify(TRUE);

	m_tagNum6.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum6.SetCaption(L"MNO");
	m_tagNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum6.SetCaptionColor(&cr_key,&cr_key_down);
	m_tagNum6.SetCaptionHeight(12);
	m_tagNum6.LinkTextGL(pTextGL);
	m_tagNum6.SetPos(px+cx_key*2,py-cy_key-30);
	m_tagNum6.SetSize(50,20);
	m_tagNum6.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);

	//7-9:
	m_btnNum7.Initialize(IDC_BT_BTN_KEY_7,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum7.SetCaption(L"7");
	m_btnNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum7.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum7.SetCaptionHeight(40);
	m_btnNum7.LinkTextGL(pTextGL);
	//m_btnNum7.SetSize(&si_key);
	m_btnNum7.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum7.SetPos(px,py-cy_key*2);
	m_btnNum7.SetShowScale(0.72f);
    m_btnNum7.EnableTouchDownNotify(TRUE);

	m_tagNum7.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum7.SetCaption(L"PQRS");
	m_tagNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum7.SetCaptionColor(&cr_key,&cr_key_down);
	m_tagNum7.SetCaptionHeight(12);
	m_tagNum7.LinkTextGL(pTextGL);
	m_tagNum7.SetPos(px,py-cy_key*2-30);
	m_tagNum7.SetSize(50,20);
	m_tagNum7.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);

	m_btnNum8.Initialize(IDC_BT_BTN_KEY_8,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum8.SetCaption(L"8");
	m_btnNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum8.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum8.SetCaptionHeight(40);
	m_btnNum8.LinkTextGL(pTextGL);
	//m_btnNum8.SetSize(&si_key);
	m_btnNum8.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum8.SetPos(px+cx_key,py-cy_key*2);
	m_btnNum8.SetShowScale(0.72f);
    m_btnNum8.EnableTouchDownNotify(TRUE);

	m_tagNum8.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum8.SetCaption(L"TUV");
	m_tagNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum8.SetCaptionColor(&cr_key,&cr_key_down);
	m_tagNum8.SetCaptionHeight(12);
	m_tagNum8.LinkTextGL(pTextGL);
	m_tagNum8.SetPos(px+cx_key,py-cy_key*2-30);
	m_tagNum8.SetSize(50,20);
	m_tagNum8.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);

	m_btnNum9.Initialize(IDC_BT_BTN_KEY_9,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum9.SetCaption(L"9");
	m_btnNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum9.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum9.SetCaptionHeight(40);
	m_btnNum9.LinkTextGL(pTextGL);
	//m_btnNum9.SetSize(&si_key);
	m_btnNum9.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum9.SetPos(px+cx_key*2,py-cy_key*2);
	m_btnNum9.SetShowScale(0.72f);
    m_btnNum9.EnableTouchDownNotify(TRUE);

	m_tagNum9.Initialize(IDC_BT_BTN_KEY_9,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNum9.SetCaption(L"WXYZ");
	m_tagNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNum9.SetCaptionColor(&cr_key,&cr_key_down);
	m_tagNum9.SetCaptionHeight(12);
	m_tagNum9.LinkTextGL(pTextGL);
	m_tagNum9.SetPos(px+cx_key*2,py-cy_key*2-30);
	m_tagNum9.SetSize(50,20);
	m_tagNum9.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);

	//*0#:
	m_btnStar.Initialize(IDC_BT_BTN_KEY_STAR,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnStar.SetCaption(L"*");
	m_btnStar.SetCaptionPos(BTN_CAPTION_CENTER,0,-6);
	m_btnStar.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnStar.SetCaptionHeight(40);
	m_btnStar.LinkTextGL(pTextGL);
	//m_btnStar.SetSize(&si_key);
	m_btnStar.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnStar.SetPos(px,py-cy_key*3);
	m_btnStar.SetShowScale(0.72f);
    m_btnStar.EnableTouchDownNotify(TRUE);
    m_btnStar.EnableLongClickNotify(TRUE);

	m_btnNum0.Initialize(IDC_BT_BTN_KEY_0,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum0.SetCaption(L"0");
	m_btnNum0.SetCaptionPos(BTN_CAPTION_CENTER/*,0,12*/);
	m_btnNum0.SetSubCaption(L"+");
    m_btnNum0.SetSubCaptionPos(BTN_CAPTION_CENTER,0,/*10*/14);
	m_btnNum0.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum0.SetCaptionHeight(38);
	m_btnNum0.LinkTextGL(pTextGL);
	//m_btnNum0.SetSize(&si_key);
	m_btnNum0.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnNum0.SetPos(px+cx_key,py-cy_key*3);
	m_btnNum0.SetShowScale(0.72f);
    m_btnNum0.EnableTouchDownNotify(TRUE);
    m_btnNum0.EnableLongClickNotify(TRUE);

	m_btnWell.Initialize(IDC_BT_BTN_KEY_WELL,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnWell.SetCaption(L"#");
	m_btnWell.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnWell.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnWell.SetCaptionHeight(40);
	m_btnWell.LinkTextGL(pTextGL);
	//m_btnWell.SetSize(&si_key);
	m_btnWell.SetNotifyFunc(&CPanelBluetoothMain::OnNotify,this);
	m_btnWell.SetPos(px+cx_key*2,py-cy_key*3);
	m_btnWell.SetShowScale(0.72f);
    m_btnWell.EnableTouchDownNotify(TRUE);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET


	//
	SetShowType(BT_HOMEPAGE_DEVICE);


	return TRUE;
}

void CALLBACK CPanelBluetoothMain::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_BT,(_T("CPanelBluetoothMain OnNotify id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara,lpUserPara));

	CPanelBluetoothMain *panel=(CPanelBluetoothMain*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelBluetoothMain::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(DEBUG_BT,(_T("CPanelBluetoothMain OnNotify id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara));
	switch (idControl)
	{
	case IDC_BT_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_KEY_0:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
                Input('0');
                PlaySoundEffect(0);
                break;
			case NOTIFY_LONG_CLICK:
                ClearInput(1);
                Input('+');
                PlaySoundEffect(0);
                break;
            }
		}
		break;
	case IDC_BT_BTN_KEY_1:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
                Input('1');
			    PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_2:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
                Input('2');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_3:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
    			Input('3');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_4:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
    			Input('4');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_5:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
    			Input('5');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_6:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
    			Input('6');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_7:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
    			Input('7');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_8:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
    			Input('8');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_9:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
    			Input('9');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_STAR:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
                Input('*');
                PlaySoundEffect(0);
                break;
			case NOTIFY_LONG_CLICK:
                ClearInput(1);
                Input(',');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_WELL:
		{
			switch (wMsg)
			{
			case NOTIFY_TOUCH_DOWN:
    			Input('#');
                PlaySoundEffect(0);
                break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_DEL:
		{
			ClearInput(1);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BOX_MENU:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			if (m_bContanctSearching)
			{
				m_btnSyncContact.SetCaption(GetResourceString(IDS_BT_PHONEBOOK_SYNC));
				//reset search status:
				m_bContanctSearching = FALSE;
				m_boxContanctSearch.Show(FALSE);
				//m_boxContanctSearch.ClearList();
				m_btnSearchContact.SetCaption(_T(""));
				m_btnSearchContact.SetIcon(&m_iconSearchContactUp);
			}
			switch (wMsg)
			{
			case NOTIFY_CONTROL:
				m_boxLists.SetFocus(index);
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
					//switch type
					if(id_item==BT_HOMEPAGE_DEVICE)
					{
					}
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
					//else if(id_item==BT_HOMEPAGE_HISTORY && m_type!=id_item && m_bStoredContactAvailable)
					else if(FALSE)
#else
					else if(id_item==BT_HOMEPAGE_HISTORY && m_type!=id_item)
#endif
					{
						//if no call history exist, download it.
						if(!m_bSyncingData && m_statusContact != CONTACT_DOWNLOADING && m_boxHistory.GetCount() == 0)
						{
							m_bDownloadingCalllog = TRUE;
#if BT_PHONEBOOK_AUTOSTORE
							m_btnSyncContact.Enable(FALSE);
#endif
							SendBluetoothCommand(BT_CMD_DOWNLOAD_CALLLOG,0);
						}
					}

					SetShowType(BT_HOMEPAGE_TYPE(id_item));
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_BT_BOX_CONTACT:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			ContactInfo *contact=(ContactInfo *)m_boxContact.GetCurFocusItemUserParam();
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
			case NOTIFY_DOUBLE_CLICK:
				{
					ShowContactDetail(TRUE,contact);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_BT_BOX_CONTANCT_SEARCH:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			ContactInfo *contact=(ContactInfo *)m_boxContanctSearch.GetCurFocusItemUserParam();
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
			case NOTIFY_DOUBLE_CLICK:
				{
					ShowContactDetail(TRUE,contact);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_BT_BOX_CONTACT_DETAIL:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
					ShowContactDetail(FALSE,NULL);
					//SetDialString(m_boxContactDetail.GetCurFocusItemName());
					PlaySoundEffect(0);
					BT_Dial(m_boxContactDetail.GetCurFocusItemName());
				}
				break;
			case NOTIFY_VALUE_CHANGE:
				{
					m_dwShowContactDetailTick = GetTickCount();
				}
				break;
			}
		}
		break;
	case IDC_BT_BTN_KEY_DIAL:
		{
			if(m_strDialString.String())
			{
				m_strRedialString = m_strDialString;
				BT_Dial((wchar_t*)m_strDialString.String());
				ClearInput(0);
			}
			else if(m_strRedialString.String())
			{
				SetDialString(m_strRedialString.String());
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_CONTACT_DOWNLOAD:
		if(!m_bSyncingData)
		{
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
			if (BT_HOMEPAGE_CONTACT == m_type)
			{			
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				ShowNotificationBox(m_hMainWnd,IDC_BT_DOWNLOAD_PB_CONFIRM,
					&CPanelBluetoothMain::OnNotify,
					this,
					0,
					_T("Download phonebook?"),
					L"",
					0);
#else
				ShowNotificationBox(m_hMainWnd,IDC_BT_DOWNLOAD_PB_CONFIRM,
					&CPanelBluetoothMain::OnNotify,
					this,
					0,
					GetResourceString(IDS_CONFIRM_DOWNLOAD_PB),
					L"",
					0);
#endif
			}
			else if (BT_HOMEPAGE_HISTORY == m_type)
			{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				ShowNotificationBox(m_hMainWnd,IDC_BT_DOWNLOAD_CALLLOG_CONFIRM,
					&CPanelBluetoothMain::OnNotify,
					this,
					0,
					_T("Download call history?"),
					L"",
					0);
#else
				ShowNotificationBox(m_hMainWnd,IDC_BT_DOWNLOAD_CALLLOG_CONFIRM,
					&CPanelBluetoothMain::OnNotify,
					this,
					0,
					GetResourceString(IDS_CONFIRM_DOWNLOAD_CALLHISTORY),
					L"",
					0);
#endif
			}
#else
			SendBluetoothCommand(BT_CMD_DOWNLOAD_PHONEBOOK,0);
			m_statusContact = CONTACT_DOWNLOADING;
			m_bContactManualDownload = TRUE;
#endif

			PlaySoundEffect(0);
		}
		break;
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	case IDC_BT_DOWNLOAD_PB_CONFIRM:
		{
			if(wMsg == NB_OK && !m_bSyncingData)
			{
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_BOSSAUDIOUI)
				//if user give permit to phonebook, they shall also give permit to call log.
				m_bUserDownloadCallLogPermit = TRUE;
#endif
				SendBluetoothCommand(BT_CMD_DOWNLOAD_PHONEBOOK,0);
				m_statusContact = CONTACT_DOWNLOADING;
				m_bContactManualDownload = TRUE;
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_DOWNLOAD_CALLLOG_CONFIRM:
		{
			if (NB_OK == wMsg && !m_bSyncingData)
			{
				m_bDownloadingCalllog = TRUE;
#if BT_PHONEBOOK_AUTOSTORE
				m_btnSyncContact.Enable(FALSE);
#endif
				m_bUserDownloadCallLogPermit = TRUE;
				SendBluetoothCommand(BT_CMD_DOWNLOAD_CALLLOG,0);
				m_statusCallLog = CONTACT_DOWNLOADING;
				m_bCallLogManualDownload = TRUE;
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_DOWNLOAD_CANCEL_CALLHISTORY:
		{
			m_statusCallLog = CONTACT_NULL;
			m_bCallLogManualDownload = FALSE;
			m_bUserDownloadCallLogPermit = FALSE;
			PlaySoundEffect(0);
		}
		break;
#endif
	case IDC_BT_BTN_CONTACT_CANCEL_DOWNLOAD:
		{
			//BT_DownloadPB(DOWNLOAD_PB_ACTION_NO);
			m_statusContact = CONTACT_NULL;
			m_bContactManualDownload = FALSE;
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_SYNC_CONTACT:
		{
			ShowContactDetail(FALSE,NULL);

			if(m_bContanctSearching)
			{
				m_btnSyncContact.SetCaption(GetResourceString(IDS_BT_PHONEBOOK_SYNC));
				//reset search status:
				m_bContanctSearching = FALSE;
				m_boxContanctSearch.Show(FALSE);
				//m_boxContanctSearch.ClearList();
				m_btnSearchContact.SetCaption(_T(""));
				m_btnSearchContact.SetIcon(&m_iconSearchContactUp);
			}
			else if(!m_bSyncingData)
			{
				if(m_boxContact.IsOnQuickSearchMode())
				{
					m_boxContact.ShowQuickSearchMode(FALSE);
				}
				else
				{
					SendBluetoothCommand(BT_CMD_DOWNLOAD_PHONEBOOK,0);
					m_statusContact = CONTACT_DOWNLOADING;
					m_bContactManualDownload = TRUE;
				}
			}
		}
		break;
	case IDC_BT_BTN_SEARCH_CONTACT:
		{
#if (CVTE_EN_SEARCH_USE_FULL_KEYBOARD)
			ShowKeyboard(IDC_PANEL_KEYBOARD+IDC_BT_BTN_SEARCH_CONTACT,&CPanelBluetoothMain::OnNotify,this,INPUT_LETTERS_CAP,
				L"",L"",20);
#else
			m_boxContact.ShowQuickSearchMode(!m_boxContact.IsOnQuickSearchMode());
#endif
			PlaySoundEffect(0);
		}
		break;
	case IDC_PANEL_KEYBOARD+IDC_BT_BTN_SEARCH_CONTACT:
		{
			if(lpPara)
			{
				RETAILMSG(1,(_T("CPanelBluetoothMain IDC_PANEL_KEYBOARD search name :[%s]\r\n"),(const wchar_t*)lpPara));
				if(wcslen((const wchar_t*)lpPara))
				{
					m_btnSyncContact.SetCaption(GetResourceString(IDS_BT_PHONEBOOK_BACK));
					m_bContanctSearching = TRUE;
					SetShowType(BT_HOMEPAGE_CONTACT);
					m_boxContanctSearch.Show(TRUE);
					m_boxContact.Show(FALSE);
					CreateSearchResultList((const wchar_t*)lpPara);
					m_btnSearchContact.SetCaption((const wchar_t*)lpPara);
					m_btnSearchContact.SetIcon(NULL);
				}
				else
				{
					m_btnSyncContact.SetCaption(GetResourceString(IDS_BT_PHONEBOOK_SYNC));
					m_bContanctSearching = FALSE;
					SetShowType(BT_HOMEPAGE_CONTACT);
					m_btnSearchContact.SetCaption(_T(""));
					m_btnSearchContact.SetIcon(&m_iconSearchContactUp);
				}
			}
		}
		break;
	case IDC_BT_BOX_HISTORY:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
// 				{
// 					BT_Dial(m_boxHistory.GetCurFocusItemValue());
// 					PlaySoundEffect(0);
// 				}
// 				break;
			case NOTIFY_SHORT_CLICK:
				{
					CM_String str(GetResourceString(IDS_CALL));
					str+=_T(" ");
					str+=m_boxHistory.GetCurFocusItemName();

					ShowNotificationBox(m_hMainWnd,IDC_BT_BOX_HISTORY_DIAL_CONFIRM,
						&CPanelBluetoothMain::OnNotify,
						this,
						0,
						str.String(),
						m_boxHistory.GetCurFocusItemValue(),
						0);

					PlaySoundEffect(0);
				}
				break;

			}
		}
		break;

	case IDC_BT_BOX_DEVICE_LIST:
		{
			static DWORD dwLast=0;
			DWORD dwTick=GetTickCount();
			if(dwTick-dwLast>1500 && dwTick-m_dwIniTick>4000)
			{
				dwLast=dwTick;
				int index=wPara;
				UINT id_item=*(UINT*)lpPara;
				switch (wMsg)
				{
				case NOTIFY_DOUBLE_CLICK:
				case NOTIFY_SHORT_CLICK:
					if(id_item == 2)//connected device:
					{
						RETAILMSG(DEBUG_BT,(_T("BT_ConnectPairedDevice, id_item:%d\r\n"),id_item));

						BT_ConnectPairedDevice(m_boxDeviceList.GetIndexValue(index), DEVICE_CONNECT_ACTION_DISCONNECT,SERVICE_TYPE_ALL_SERVICE);
					}
					else if(id_item == 1)//paired device:
					{
						RETAILMSG(DEBUG_BT,(_T("BT_ConnectPairedDevice, id_item:%d\r\n"),id_item));

						if(m_bConnected)
						{
							BT_ConnectPairedDevice(m_boxDeviceList.GetIndexValue(index), DEVICE_CONNECT_ACTION_DISCONNECT,SERVICE_TYPE_ALL_SERVICE);
						}
						else
						{
							BT_ConnectPairedDevice(m_boxDeviceList.GetIndexValue(index), DEVICE_CONNECT_ACTION_CONNECT,SERVICE_TYPE_ALL_SERVICE);
							m_boxDeviceList.SetCurFocusItemSubName(GetResourceString(IDS_CONNECTING));
							m_btnSearchDevice.Enable(FALSE);
							m_boxDeviceList.Enable(FALSE);
						}
					}
					else if(id_item == 0)//not paired
					{
						if(m_bConnected)
						{
							BT_ConnectPairedDevice(m_boxDeviceList.GetIndexValue(index), DEVICE_CONNECT_ACTION_DISCONNECT,SERVICE_TYPE_ALL_SERVICE);
						}
						else
						{
							RETAILMSG(DEBUG_BT,(_T("BT_PairDevice, id_item:%d\r\n"),id_item));

							BT_PairDevice(m_boxDeviceList.GetIndexValue(index), PAIR_ACTION_INITIATE);
							m_boxDeviceList.SetCurFocusItemSubName(GetResourceString(IDS_BT_PAIRING));
							m_btnSearchDevice.Enable(FALSE);
							m_boxDeviceList.Enable(FALSE);
						}
					}

					PlaySoundEffect(0);

					break;
				case NOTIFY_CONTROL://delete current:
					if(id_item == 2)//connected
					{
						strDeviceInfo *dev=(strDeviceInfo *)m_boxDeviceList.GetIndexUserParam(index);
						DeleteStoredInfo(dev);

						wchar_t *id=m_boxDeviceList.GetIndexValue(index);
						BT_ConnectPairedDevice(id, DEVICE_CONNECT_ACTION_DISCONNECT,SERVICE_TYPE_ALL_SERVICE);
						BT_DeletePairedDevice(id);
						PlaySoundEffect(0);
					}
					else if(id_item == 1)//paired
					{
						strDeviceInfo *dev=(strDeviceInfo *)m_boxDeviceList.GetIndexUserParam(index);
						DeleteStoredInfo(dev);

						wchar_t *id=m_boxDeviceList.GetIndexValue(index);
						BT_DeletePairedDevice(id);
						PlaySoundEffect(0);
					}
					break;
				}
			}
		}
		break;

	case IDC_BT_BOX_HISTORY_DIAL_CONFIRM:
		{
			if(wMsg == NB_OK)//confirm to dial:
			{
				RETAILMSG(DEBUG_BT,(_T("----confirm to dial-----\r\n")));
				BT_Dial(m_boxHistory.GetCurFocusItemValue());
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_DISCONNECT_CONFIRM:
		{
			if(wMsg == NB_OK)
			{

			}
			PlaySoundEffect(0);
		}
		break;

	case IDC_BT_BTN_SEARCHDEVICE:
		{
			//if any device connected, disconnect it first
			if(m_hfStatus == CONNECTION_STATUS_CONNECTED)
			{
				SendBluetoothCommand(BT_CMD_DISCONNECT_DEVICE,0);
			}
			if(m_bDeviceSearching)
			{
				m_bDeviceSearching = FALSE;
				BT_InquiryDevice(INQUIRY_ACTION_STOP);
			}
			else
			{
				m_bDeviceSearching = TRUE;
				m_boxDeviceList.ClearList();
				BT_InquiryDevice(INQUIRY_ACTION_START);
			}

			m_boxDeviceList.Show(m_type == BT_HOMEPAGE_DEVICE/* && !m_bDeviceSearching*/);
			m_btnSearchDevice.SetCaption(GetResourceString(m_bDeviceSearching?IDS_CANCEL:IDS_SEARCH));

			PlaySoundEffect(0);
		}
		break;
	default:
		break;
	}
}
void CPanelBluetoothMain::DeleteStoredInfo(strDeviceInfo *dev)
{
	if(dev)
	{
		RETAILMSG(DEBUG_BT,(_T("----DeleteStoredInfo, %s-----\r\n"),dev->DevAdd));
		if(dev->DevAdd[0])
		{
			//delete stored contact:
			wchar_t tmp[20];
			wcscpy_s(tmp,20,dev->DevAdd);
			int len=wcslen(tmp);
			for (int i=0;i<len;i++)
			{
				if(tmp[i] == ':')
				{
					tmp[i]='-';
				}
			}

			WCHAR filename[MAX_PATH];
			swprintf_s(filename,MAX_PATH,_T("%sPhonebook\\"),g_strInstallPath);
			wcscat_s(filename,MAX_PATH,tmp);
			wcscat_s(filename,MAX_PATH,_T(".pb"));

			BOOL bRet = DeleteFile(filename);
			RETAILMSG(1,(_T("\n*************Delete StoredPhonebook.%s, Ret=%d\n"),filename,bRet));
		}
	}
}
void CPanelBluetoothMain::ShowContactDetail(BOOL bShow,ContactInfo* contact)
{
	if(bShow)
	{
		if(contact)
		{
			int count=contact->PhoneNumber.size();//contact->PhoneCount;

			CListBoxItemGL *pItem;

			m_boxContactDetail.ClearList();

			for (int i=0;i<contact->PhoneNumber.size();i++)
			{
				pItem=new CListBoxItemGL(contact->PhoneNumber.at(i).number);
				switch (contact->PhoneNumber.at(i).nNumerType)
				{
				case PB_NUMER_TYPE_CELL:
					pItem->SetIcon(&m_iconTypeMobile);
					break;
				case PB_NUMER_TYPE_HOME:
					pItem->SetIcon(&m_iconTypeHome);
					break;
				case PB_NUMER_TYPE_WORK:
					pItem->SetIcon(&m_iconTypeWork);
					break;
				default:
// 					pItem->SetIcon(&m_iconHistory);
					pItem->SetIcon(&m_iconTypeDefault);
					pItem->SetIconScale(0.70f,0.70f);
					break;
				}
				m_boxContactDetail.AddItem(pItem);
			}

			if(count>1)
			{
				m_dwShowContactDetailTick = GetTickCount();
				m_boxContactDetail.Show(bShow);
			}
			else if(count==1)
			{
				/*
				m_boxContactDetail.Show(FALSE);
				SetDialString(m_boxContactDetail.GetIndexName(0));
				//
				SetShowType(BT_HOMEPAGE_DIAL);
				*/

				//or dial out directly:
				BT_Dial(m_boxContactDetail.GetIndexName(0));
			}
		}
	}
	else
	{
		m_dwShowContactDetailTick = GetTickCount();
		m_boxContactDetail.Show(bShow);
	}

}
void CPanelBluetoothMain::SetShowType(BT_HOMEPAGE_TYPE type)
{
	if(type!=BT_HOMEPAGE_DEVICE
		&& type!=BT_HOMEPAGE_CONTACT
		&& type!=BT_HOMEPAGE_HISTORY
		&& type!=BT_HOMEPAGE_DIAL)
	{
		return;
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CListBoxItemGL* pItem;
	m_boxLists.m_listItems.Head();
	while( (pItem = m_boxLists.m_listItems.Get()) != NULL )
	{
		if ((BT_HOMEPAGE_TYPE)m_boxLists.m_listItems.GetID() == type)
		{
			pItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, -16 , 0);
		}
		else
		{
			pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, -16 , 0);
		}
		m_boxLists.m_listItems.Next();
	}
#endif

	m_type = type;

	m_iParam = type;

	ShowContactDetail(FALSE,NULL);
	//device
// 	m_btnPairedDevice.Show(type == BT_HOMEPAGE_DEVICE);
	m_btnSearchDevice.Show(m_bIniStatus && type == BT_HOMEPAGE_DEVICE);

	m_boxDeviceList.Show(type == BT_HOMEPAGE_DEVICE /*&& !m_bDeviceSearching*/);
// 	m_boxDevicePaired.Show(type == BT_HOMEPAGE_DEVICE && !m_bShowSearchedDevice);
// 	m_boxDeviceSearched.Show(type == BT_HOMEPAGE_DEVICE && m_bShowSearchedDevice);

// 	m_btnPairedDevice.SetStatus(m_bShowSearchedDevice?BS_UP:BS_FOCUS);
// 	m_btnSearchDevice.SetStatus(m_bShowSearchedDevice?BS_FOCUS:BS_UP);

	//history
	m_boxHistory.Show(type == BT_HOMEPAGE_HISTORY);

	//contact
//	m_tagKeyboardBk.Show(type == BT_HOMEPAGE_CONTACT);
	//m_boxContactDetail.Show(type == BT_HOMEPAGE_CONTACT);
//	m_tagDialString.Show(type == BT_HOMEPAGE_CONTACT);

	m_boxContact.Show(type == BT_HOMEPAGE_CONTACT && m_statusContact==CONTACT_DOWNLOADED);

	m_btnDownloadPhonebook.Show(type == BT_HOMEPAGE_CONTACT  && m_statusContact==CONTACT_NULL || m_statusContact==CONTACT_DOWNLOADFAILED || m_statusContact==CONTACT_DOWNLOADCANCLED);
	m_btnCancelDownloadPhonebook.Show(type == BT_HOMEPAGE_CONTACT  && m_statusContact==CONTACT_DOWNLOADING && GetTickCount()-m_dwContactDownloadTick>1000);
	m_btnSearchContact.Show(type == BT_HOMEPAGE_CONTACT  && m_statusContact==CONTACT_DOWNLOADED );
#if BT_PHONEBOOK_AUTOSTORE
	m_btnSyncContact.Show(type == BT_HOMEPAGE_CONTACT  && m_statusContact==CONTACT_DOWNLOADED );
#endif

#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	m_btnDownloadCallLog.Show(type == BT_HOMEPAGE_HISTORY  && (m_statusCallLog==CONTACT_NULL || m_statusCallLog==CONTACT_DOWNLOADFAILED || m_statusCallLog==CONTACT_DOWNLOADCANCLED));
	m_btnCancelDownloadCallLog.Show(type == BT_HOMEPAGE_HISTORY  && m_statusCallLog==CONTACT_DOWNLOADING && GetTickCount()-m_dwCallLogDownloadTick>1000);
#endif
	m_tagKeyboardBk.Show(type == BT_HOMEPAGE_DIAL);
	m_tagDialString.Show(type == BT_HOMEPAGE_DIAL);

	m_btnDial.Show(type == BT_HOMEPAGE_DIAL);
	m_btnDelete.Show(m_type == BT_HOMEPAGE_DIAL && m_strDialString.Size()>0);

	m_btnNum0.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum1.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum2.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum3.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum4.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum5.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum6.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum7.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum8.Show(type == BT_HOMEPAGE_DIAL);
	m_btnNum9.Show(type == BT_HOMEPAGE_DIAL);
	m_btnStar.Show(type == BT_HOMEPAGE_DIAL);
	m_btnWell.Show(type == BT_HOMEPAGE_DIAL);

	//move to current device:
	m_boxLists.MoveToIndex(m_boxLists.SearchID(type));

}

void CPanelBluetoothMain::SetDialString(const wchar_t* pString)
{
	m_strDialString = pString;

	if(m_strDialString.Size())
	{
		int maxlen=20;
		if(m_strDialString.Size()<=maxlen)
		{
			m_tagDialString.SetCaption(m_strDialString.String());
		}
		else
		{
			m_tagDialString.SetCaption(m_strDialString.String()+m_strDialString.Size()-maxlen);
		}
	}
	else
	{
		m_tagDialString.SetCaption(NULL);
	}
}
void CPanelBluetoothMain::Input(wchar_t c)
{
	wchar_t str[8];
	swprintf_s(str,8,_T("%c"),c);

	m_strDialString += str;
	if(m_strDialString.Size())
	{
		int maxlen=19;
		if(m_strDialString.Size()<=maxlen)
		{
			m_tagDialString.SetCaption(m_strDialString.String());
		}
		else
		{
			m_tagDialString.SetCaption(m_strDialString.String()+m_strDialString.Size()-maxlen);
		}
	}
}
void CPanelBluetoothMain::ClearInput(int i)
{
	if(m_strDialString.Size()>0)
	{
		if(i<=0)
			m_strDialString = NULL;
		else
			m_strDialString -= i;

		if(m_strDialString.Size())
		{
			int maxlen=20;
			if(m_strDialString.Size()<=maxlen)
			{
				m_tagDialString.SetCaption(m_strDialString.String());
			}
			else
			{
				m_tagDialString.SetCaption(m_strDialString.String()+m_strDialString.Size()-maxlen);
			}
		}
		else
		{
			m_tagDialString.SetCaption(NULL);
		}
	}
}
void CPanelBluetoothMain::ClearDeviceList()
{
	m_boxDeviceList.ClearList();
	m_bConnected=FALSE;
}
void CPanelBluetoothMain::AddPairedDevice(strDeviceInfo *dev)
{
	CListBoxItemGL *pItem;

	RETAILMSG(DEBUG_BT,(_T("####AddPairedDevice %s :%s, connect:%d  HFPState:%d A2DPState:%d\r\n"),dev->DevAdd,dev->DeviceName,dev->Connected,dev->HFPState,dev->A2DPState));

	if((dev->HFPState==HFP_CONNECT_STATE_CONNECT) || (dev->A2DPState==A2DP_CONNECT_STATE_CONNECT))
	{
		CM_String str=GetResourceString(IDS_CONNECTED);

		if(dev->HFPState == HFP_CONNECT_STATE_CONNECT)
		{
			str+=_T(" HFP");
			m_bConnected=TRUE;
		}
		if(dev->A2DPState == A2DP_CONNECT_STATE_CONNECT)
		{
			str+=_T(" A2DP AVRCP");
			m_bConnected=TRUE;
		}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
		pItem=new CListBoxItemGL(dev->DeviceName,dev->ID,str.String(), 70, 0);
		pItem->SetIcon(&m_iconDeviceFocus, NULL , 30, 0);
		pItem->SetIcon2(&m_iconDeleteFocus, &m_iconDeleteFocus, 32, 0);
		pItem->SetCaptionColor(&cr_w, &cr_w);
		pItem->ShowValue(FALSE);
		pItem->SetUserParam(LPARAM(dev));
#else
		COLORGL cr_blue={0.0f/256.0f,30.0f/256.0f,180.0f/256.0f,1.0f};
		pItem=new CListBoxItemGL(dev->DeviceName,dev->ID,str.String());
		pItem->SetIcon(&m_iconDeviceFocus);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
		pItem->SetCaptionColor(&cr_blue,&cr_blue);
		pItem->ShowValue(FALSE);
		pItem->SetUserParam(LPARAM(dev));

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

		m_boxDeviceList.AddItem(pItem,2);
		m_boxDeviceList.SetFocus(m_boxDeviceList.m_listItems.GetIndexByID(2));
	}
	else
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(dev->DeviceName,dev->ID,GetResourceString(IDS_BT_DEVICE_PAIRED), 70, 0);
		pItem->SetIcon(&m_iconDeviceFocus, NULL , 30, 0);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus, 32, 0);
		pItem->ShowValue(FALSE);
		pItem->SetUserParam(LPARAM(dev));
#else
		pItem=new CListBoxItemGL(dev->DeviceName,dev->ID,GetResourceString(IDS_BT_DEVICE_PAIRED));
		pItem->SetIcon(&m_iconDeviceFocus);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
		pItem->ShowValue(FALSE);
		pItem->SetUserParam(LPARAM(dev));
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
		m_boxDeviceList.AddItem(pItem,1);
	}
}

void CPanelBluetoothMain::AddInquiredDevice(strDeviceInfo *dev)
{
	RETAILMSG(DEBUG_BT,(_T("####AddInquiredDevice %s :%s, connect:%d  HFPState:%d A2DPState:%d\r\n"),dev->DevAdd,dev->DeviceName,dev->Connected,dev->HFPState,dev->A2DPState));

	CListBoxItemGL *pItem;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	if(wcslen(dev->DeviceName))
		pItem=new CListBoxItemGL(dev->DeviceName,dev->ID,NULL, 70, 0);
	else
		pItem=new CListBoxItemGL(dev->DevAdd,dev->ID,NULL, 70, 0);

	pItem->SetIcon(&m_iconDevice, NULL , 30, 0);
#else
	if(wcslen(dev->DeviceName))
		pItem=new CListBoxItemGL(dev->DeviceName,dev->ID,NULL);
	else
		pItem=new CListBoxItemGL(dev->DevAdd,dev->ID,NULL);

	pItem->SetIcon(&m_iconDevice);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	pItem->ShowValue(FALSE);
	pItem->SetUserParam(LPARAM(dev));
	m_boxDeviceList.AddItem(pItem,0);
}
void CPanelBluetoothMain::OnUpdatePBFinish(int cnt)
{
	RETAILMSG(DEBUG_BT,(_T("####OnUpdatePBFinish, cnt=%d\r\n"),cnt));

	if(cnt >= 0)
	{
		m_statusContact=(cnt||m_bStoredContactAvailable)?CONTACT_DOWNLOADED:CONTACT_NULL;
	}
}
void CPanelBluetoothMain::OnUpdateCallHistoryFinish(int cnt)
{
	RETAILMSG(DEBUG_BT,(_T("####OnUpdateCallHistoryFinish, cnt=%d\r\n"),cnt));

	m_bDownloadingCalllog = FALSE;
	m_bSyncingData = FALSE;

#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	m_statusCallLog = cnt?CONTACT_DOWNLOADED:CONTACT_NULL;
#endif

#if BT_PHONEBOOK_AUTOSTORE
	m_btnSyncContact.Enable(TRUE);
#endif
}
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
void CPanelBluetoothMain::OnUpdateCallLogFinish(DWORD cnt)
{
	m_statusCallLog = cnt?CONTACT_DOWNLOADED:CONTACT_NULL;
}

BOOL CPanelBluetoothMain::IsUserDownloadCallLogPermit()
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	RETAILMSG(1, (TEXT("MSG:[%s]: user:%d\r\n"), TEXT(__FUNCTION__),m_bUserDownloadCallLogPermit ));
	return m_bUserDownloadCallLogPermit ;
#else
	//if stored phonebook exist, we already have user permit.
	RETAILMSG(1, (TEXT("MSG:[%s]: user:%d, storeContact=%d\r\n"), TEXT(__FUNCTION__),m_bUserDownloadCallLogPermit,m_bStoredContactAvailable ));
	return m_bUserDownloadCallLogPermit || m_bStoredContactAvailable;
#endif
}
#endif

void CPanelBluetoothMain::ClearContactList()
{
	m_boxContact.ClearList();
}
void CPanelBluetoothMain::AddContact(ContactInfo *contact)
{
	CListBoxItemGL *pItem;

	m_statusContact=CONTACT_DOWNLOADED;
	m_bContactManualDownload = FALSE;

	if(m_pSkinManager->GetConfig()->GetCurParam()->bVcardFirstNameFirst)
	{
		//swap family name and first name:
		wchar_t temp[64];
		wchar_t *p=temp;
		int family_len=0;
		int total_len=wcslen(contact->Name);
		//cut tail space
		int ispace_tail=0;
		int ispace_mid=0;
		if(total_len>1)
		{
			wchar_t *p=contact->Name+total_len-1;
			while (*p == ' ')
			{
				ispace_tail++;	*p = 0;		p--;
			}
		}
		total_len-=ispace_tail;
		wcscpy_s(temp,64,contact->Name);
		if(p = wcsstr(temp,_T(" ")))
		{
			if(*(p+1) != 0)
			{
				family_len=p-temp;
				temp[total_len]=' ';
				memcpy(temp+total_len+1,temp,family_len*2);
				temp[total_len+family_len+1]=0;
				while (*(p++)==' ')
				{
					ispace_mid++;
				}
			}
		}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		if(family_len)
			pItem=new CListBoxItemGL(temp+family_len+ispace_mid, NULL, NULL, 70, 0);
		else
			pItem=new CListBoxItemGL(temp, NULL, NULL, 70, 0);
#else
		if(family_len)
			pItem=new CListBoxItemGL(temp+family_len+ispace_mid);
		else
			pItem=new CListBoxItemGL(temp);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
	}
	else
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(contact->Name, NULL, NULL, 70, 0);
#else
		pItem=new CListBoxItemGL(contact->Name);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	pItem->SetIcon(&m_iconContact,&m_iconContactFocus, 34, 0);
#else
	pItem->SetIcon(&m_iconContact,&m_iconContactFocus);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	pItem->SetUserParam(LPARAM(contact));
	m_boxContact.AddItemByOrder(pItem,contact->Index);

}
CONTACT_STATUS CPanelBluetoothMain::GetContactStatus()
{
	return m_statusContact;
}
void CPanelBluetoothMain::ClearHistoryList()
{
	m_boxHistory.ClearList();
}

void CPanelBluetoothMain::AddCallLog(strCallLog *log,BOOL bAdjustNameOrder)
{
	CListBoxItemGL *pItem;
	wchar_t strTime[128];
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	wchar_t wcDate[14];
	memcpy(wcDate, log->calldate, sizeof(log->calldate));
	for (int i=0;i<14;i++)
	{
		if(wcDate[i] == '-')
			wcDate[i] = 0;
	}

	if(m_pSkinManager->GetConfig()->GetCurParam()->idTimeMode == TIME_MODE_12H)
	{
		WORD hour,minute,second;
		BOOL bPM = FALSE;

		swscanf(log->calltime, _T("%d:%d:%d"), &hour, &minute, &second);

		if (hour >= 12 )
			bPM = TRUE;

		hour = hour%12;
		if(hour == 0)
			hour = 12;

		swprintf_s(strTime,128,_T("%s/%s/%s %02d:%02d:%02d %s"),&wcDate[5], &wcDate[8], &wcDate[0], hour, minute, second, bPM?_T("PM"):_T("AM"));
	}
	else
	{
		swprintf_s(strTime,128,_T("%s/%s/%s %s"),&wcDate[5], &wcDate[8], &wcDate[0], log->calltime);
	}
#else
	swprintf_s(strTime,128,_T("%s %s"),log->calldate,log->calltime);
#endif

	if(bAdjustNameOrder && m_pSkinManager->GetConfig()->GetCurParam()->bVcardFirstNameFirst)
	{
		//swap family name and first name:
		wchar_t temp[64];
		wchar_t *p=temp;
		int family_len=0;
		int total_len=wcslen(log->callname);
		//cut tail space
		int ispace_tail=0;
		int ispace_mid=0;
		if(total_len>1)
		{
			wchar_t *p=log->callname+total_len-1;
			while (*p == ' ')
			{
				ispace_tail++;	*p = 0;		p--;
			}
		}
		total_len-=ispace_tail;
		wcscpy_s(temp,64,log->callname);
		if(p = wcsstr(temp,_T(" ")))
		{
			if(*(p+1) != 0)
			{
				family_len=p-temp;
				temp[total_len]=' ';
				memcpy(temp+total_len+1,temp,family_len*2);
				temp[total_len+family_len+1]=0;
				while (*(p++)==' ')
				{
					ispace_mid++;
				}
			}
		}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		if(family_len)
			pItem=new CListBoxItemGL(temp+family_len+ispace_mid,log->callnum,strTime, 70, 0);
		else
			pItem=new CListBoxItemGL(temp,log->callnum,strTime, 70, 0);
#else
		if(family_len)
			pItem=new CListBoxItemGL(temp+family_len+ispace_mid,log->callnum,strTime);
		else
			pItem=new CListBoxItemGL(temp,log->callnum,strTime);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
	}
	else
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(log->callname,log->callnum,strTime, 70, 0);
#else
		pItem=new CListBoxItemGL(log->callname,log->callnum,strTime);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_red={255.0f/255.0f, 48.0f/255.0f, 73.0f/255.0f, 1.0};
	if(log->pbtype==CALLLOG_TYPE_LASTDIAL)
		pItem->SetIcon(&m_iconOut, &m_iconOutFocus, 34, 0);
	else if(log->pbtype==CALLLOG_TYPE_RECEIVEDCALL)
		pItem->SetIcon(&m_iconIn, &m_iconInFocus, 34, 0);
	else if(log->pbtype==CALLLOG_TYPE_MISSEDCALL)
	{
		pItem->SetIcon(&m_iconMissed, &m_iconMissedFocus, 34, 0);
		pItem->SetCaptionColor(&cr_red);
	}
	else
		pItem->SetIcon(&m_iconContact, NULL, 34, 0);
#else
	if(log->pbtype==CALLLOG_TYPE_LASTDIAL)
		pItem->SetIcon(&m_iconOut);
	else if(log->pbtype==CALLLOG_TYPE_RECEIVEDCALL)
		pItem->SetIcon(&m_iconIn);
	else if(log->pbtype==CALLLOG_TYPE_MISSEDCALL)
		pItem->SetIcon(&m_iconMissed);
	else
		pItem->SetIcon(&m_iconContact);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	m_boxHistory.AddItem(pItem,0);

#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	m_statusCallLog=CONTACT_DOWNLOADED;
	m_bCallLogManualDownload = FALSE;
#endif
}
void CPanelBluetoothMain::OnNotifySetupInI(strSetupInI *ptIni)
{

}
void CPanelBluetoothMain::OnNotifyStartSyncContact()
{
	m_bSyncingData = TRUE;

	if(m_bContactManualDownload)
	{
		m_statusContact = CONTACT_DOWNLOADING;
	}

#if BT_PHONEBOOK_AUTOSTORE
	m_btnSyncContact.Enable(FALSE);
#endif
}
void CPanelBluetoothMain::OnNotifyDeviceInquiryFinish()
{
	m_bDeviceSearching = FALSE;

	m_boxDeviceList.Show(m_type == BT_HOMEPAGE_DEVICE /*&& !m_bDeviceSearching*/);
	m_btnSearchDevice.SetCaption(GetResourceString(m_bDeviceSearching?IDS_CANCEL:IDS_SEARCH));
}
void CPanelBluetoothMain::OnIniStatus(BOOL bSucc)
{
	m_bIniStatus = bSucc;
	m_dwIniTick = GetTickCount();
	m_btnSearchDevice.Show(m_bIniStatus && m_type == BT_HOMEPAGE_DEVICE);
}
void CPanelBluetoothMain::OnNotifyPairResult(PAIR_RESULT result)
{
	m_btnSearchDevice.Enable(TRUE);
	m_boxDeviceList.Enable(TRUE);
}
void CPanelBluetoothMain::OnNotifyA2dpConnected(BOOL bConnected)
{
	if(bConnected)
	{
		if(m_idPickDeviceFor == BT_PICK_DEVICE_FOR_A2DP)
		{
			//SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_RIGHT);
			SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,MEDIA_TYPE_A2DP);
			//reset:
			m_idPickDeviceFor = 0;
		}
	}
}
void CPanelBluetoothMain::OnNotifyHfpConnected(BOOL bConnected)
{
	if(bConnected)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		CListBoxItemGL *pItem;
		pItem = new CListBoxItemGL(GetResourceString(IDS_CALL_HISTORY),NULL ,NULL, 30, 0);
		pItem->SetIcon(&m_iconHistory, &m_iconHistoryFocus, 16, 0);
		pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, -16, 0);
		m_boxLists.AddItem(pItem, BT_HOMEPAGE_HISTORY);

		pItem = new CListBoxItemGL(GetResourceString(IDS_CONTACT),NULL ,NULL, 30, 0);
		pItem->SetIcon(&m_iconContact,&m_iconContactFocus,16,0);
		pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, -16, 0);
		m_boxLists.AddItem(pItem, BT_HOMEPAGE_CONTACT);

		pItem = new CListBoxItemGL(GetResourceString(IDS_DIAL),NULL ,NULL, 30, 0);
		pItem->SetIcon(&m_iconKeyboard,&m_iconKeyboardFocus,16,0);
		pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, -16, 0);
		m_boxLists.AddItem(pItem, BT_HOMEPAGE_DIAL);
#else
		CListBoxItemGL *pItem;
		pItem=new CListBoxItemGL(GetResourceString(IDS_CALL_HISTORY));
		pItem->SetIcon(&m_iconHistory,&m_iconHistoryFocus);
		m_boxLists.AddItem(pItem,BT_HOMEPAGE_HISTORY);
		pItem=new CListBoxItemGL(GetResourceString(IDS_CONTACT));
		pItem->SetIcon(&m_iconContact,&m_iconContactFocus);
		m_boxLists.AddItem(pItem,BT_HOMEPAGE_CONTACT);
		pItem=new CListBoxItemGL(GetResourceString(IDS_DIAL));
		pItem->SetIcon(&m_iconKeyboard,&m_iconKeyboardFocus);
		m_boxLists.AddItem(pItem,BT_HOMEPAGE_DIAL);

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

		if(m_idPickDeviceFor == BT_PICK_DEVICE_FOR_HF || m_idPickDeviceFor == 0)
		{
// 			SetShowType(BT_HOMEPAGE_CONTACT);
			SetShowType(BT_HOMEPAGE_DIAL);
		}

		m_hfStatus = CONNECTION_STATUS_CONNECTED;
	}
	else
	{
		SetShowType(BT_HOMEPAGE_DEVICE);

		m_boxLists.DeleteItem(BT_HOMEPAGE_HISTORY);
		m_boxLists.DeleteItem(BT_HOMEPAGE_CONTACT);
		m_boxLists.DeleteItem(BT_HOMEPAGE_DIAL);

		m_boxContact.ClearList();
		m_boxHistory.ClearList();

		m_statusContact = CONTACT_NULL;
		m_dwContactDownloadedCount = 0;
		m_bContactManualDownload = FALSE;

		m_hfStatus = CONNECTION_STATUS_IDLE;

#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
		m_statusCallLog = CONTACT_NULL;
		m_bCallLogManualDownload = FALSE;
		m_bUserDownloadCallLogPermit = FALSE;
#endif
		m_bStoredContactAvailable = FALSE;
	}

	m_btnSearchDevice.Enable(TRUE);
	m_boxDeviceList.Enable(TRUE);
}
void CPanelBluetoothMain::SetStoredContactStatus(BOOL bAvailable)
{
	m_bStoredContactAvailable = bAvailable;
}
int CPanelBluetoothMain::CreateSearchResultList(const wchar_t *SearchString)
{
	wchar_t str_temp[MAX_PATH];
	int i = 0;

	if(!SearchString)
		return 0;

	//clear first:
	m_boxContanctSearch.ClearList();

	m_boxContact.m_listItems.Head();
	while (!m_boxContact.m_listItems.End())
	{
		wcscpy_s(str_temp,MAX_PATH,m_boxContact.m_listItems.Get()->GetItemName());
		wchar_t *strTmp = str_temp;
		wchar_t *strTmpSearch = const_cast<wchar_t*>(SearchString);
		_wcsupr(strTmp);
		_wcsupr(strTmpSearch);
		if (wcsstr(strTmp,strTmpSearch)!=NULL)
		{
			CListBoxItemGL *pItem;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			pItem=new CListBoxItemGL(m_boxContact.m_listItems.Get()->GetItemName(), NULL, NULL, 70, 0);
			pItem->SetUserParam(m_boxContact.m_listItems.Get()->GetUserParam());
			pItem->SetIcon(&m_iconContact,&m_iconContactFocus, 34, 0);
#else
			pItem=new CListBoxItemGL(m_boxContact.m_listItems.Get()->GetItemName());
			pItem->SetUserParam(m_boxContact.m_listItems.Get()->GetUserParam());
			pItem->SetIcon(&m_iconContact,&m_iconContactFocus);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

			m_boxContanctSearch.AddItemByOrder(pItem,i);
			i++;
		}
		else if(m_boxContact.m_listItems.Get()->GetKeyWords())
		{
			if (wcsstr(m_boxContact.m_listItems.Get()->GetKeyWords(),strTmpSearch)!=NULL)
			{
				CListBoxItemGL *pItem;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				pItem=new CListBoxItemGL(m_boxContact.m_listItems.Get()->GetItemName(), NULL, NULL, 70, 0);
				pItem->SetUserParam(m_boxContact.m_listItems.Get()->GetUserParam());
				pItem->SetIcon(&m_iconContact,&m_iconContactFocus, 34, 0);
#else
				pItem=new CListBoxItemGL(m_boxContact.m_listItems.Get()->GetItemName());
				pItem->SetUserParam(m_boxContact.m_listItems.Get()->GetUserParam());
				pItem->SetIcon(&m_iconContact,&m_iconContactFocus);
#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

				m_boxContanctSearch.AddItemByOrder(pItem,i);
				i++;
			}
		}
		m_boxContact.m_listItems.Next();
	}

	RETAILMSG(1,(_T("####SearchResultList: %s, cnt=%d\r\n"),SearchString,i));

	return i;
}
