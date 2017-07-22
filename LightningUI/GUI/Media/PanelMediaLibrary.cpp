#include "PanelMediaLibrary.h"
#include "../../resource.h"
#include "../Settings/SettingCommon.h"

#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
#define LIB_BROWSE_BTN_DISPLAY_REVERSE	1
#else
#define LIB_BROWSE_BTN_DISPLAY_REVERSE	0
#endif

//library

CPanelMediaLibrary::CPanelMediaLibrary(void)
{
	m_hExploreThread = INVALID_HANDLE_VALUE;
	m_bRefreshingLib=FALSE;
	m_hRefreshEvent = INVALID_HANDLE_VALUE;

	m_mediaCategory = MEDIA_AUDIO;
	m_bCancelRefresh=FALSE;
	m_iTotalFileScanned=0;
	m_type=MEDIA_LIBRARY_TRACKS;
// 	m_strCurPath=_T("\\");

	m_iExplorerFilesCount=0;
	m_iExplorerFolderCount=0;
	m_iExplorerPlayIndex=0;

	m_hwndNotify = NULL;

	for (int i=0;i<MEDIA_TYPE_NUMBER;i++)
	{
		m_idValidSrcList[i]=0;
	}
	m_idCurMediaType=0;

	m_bAutoReturn = FALSE;
	m_dwRefreshTick = 0;

	m_bPlayingFlag = FALSE;

	m_bEnableOperate = TRUE;
	m_bEnableShowSlider = TRUE;
	m_bNeedUpdateSlider = FALSE;

	m_bEnableVideo = TRUE;
    m_iFactoryTestType = FACTORY_CMD_MEDIA_TEST_INVALID;
    m_iFactoryTestCmd = FACTORY_CMD_INVALID;

	m_bUsb1Available = FALSE;
	m_bUsb2Available = FALSE;

	m_dwSetShowTypeTick = 0;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_dwNoValidFileTick = 0;
#endif
}
CPanelMediaLibrary::~CPanelMediaLibrary(void)
{
	if(m_hExploreThread != INVALID_HANDLE_VALUE)
		CloseHandle(m_hExploreThread);

	if(m_hRefreshEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hRefreshEvent);

}
BOOL CPanelMediaLibrary::IsReady()
{
	return CPanelGL::IsReady();
}
void CPanelMediaLibrary::OnTouchDown(POINT *pt)
{
	RETAILMSG(DEBUG_MEDIA,(_T("OnTouchDown\r\n")));
	CPanelGL::OnTouchDown(pt);

	//
	m_bAutoReturn = (m_idCurMediaType == MEDIA_TYPE_A2DP || m_mediaCategory == MEDIA_VIDEO); //FALSE;
	m_dwRefreshTick = GetTickCount();

	m_bEnableShowSlider = FALSE;
	m_sliProgress.Show(FALSE);
}
void CPanelMediaLibrary::OnTouchUp(POINT *pt)
{
	RETAILMSG(DEBUG_MEDIA,(_T("OnTouchUp\r\n")));
	CPanelGL::OnTouchUp(pt);

	m_bEnableShowSlider = TRUE;
	m_bNeedUpdateSlider = TRUE;
}
void CPanelMediaLibrary::OnTouchMove(POINT *pt)
{
	RETAILMSG(DEBUG_MEDIA,(_T("OnTouchMove\r\n")));
	CPanelGL::OnTouchMove(pt);
	m_dwRefreshTick = GetTickCount();
}

void CPanelMediaLibrary::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
void CPanelMediaLibrary::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);


	if(m_boxLibrary.IsOnQuickSearchMode())
	{
		m_boxLibrary.ShowQuickSearchMode(FALSE);
	}
	if(m_boxExplorer.IsOnQuickSearchMode())
	{
		m_boxExplorer.ShowQuickSearchMode(FALSE);
	}

	//
	m_bAutoReturn = FALSE;
}
void CPanelMediaLibrary::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	 RETAILMSG(1, (TEXT("MSG:Media:CPanelMediaLibrary::OnEnterPanel\r\n")));
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

	//
//		if(m_idCurMediaType)
//		{
//			m_boxDevice.MoveToIndex(m_boxDevice.SearchID(m_idCurMediaType));
//		}
//		else
	if(iParam)
	{
		//SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,iParam);
		SetMediaSrcType(iParam);
	}

	m_bAutoReturn = (m_idCurMediaType == MEDIA_TYPE_A2DP || m_mediaCategory == MEDIA_VIDEO || m_mediaCategory == MEDIA_AUDIO); //FALSE;
	m_dwRefreshTick = GetTickCount();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_dwNoValidFileTick = m_dwRefreshTick;
#endif
}
void CPanelMediaLibrary::Render()
{
// 	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_034):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,0.8};
// 	cr.a=m_pSkinManager->GetConfig()->GetCurParam()->fUITransparency;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	static BOOL bNoValidFileCanPlay = FALSE; 
#endif

	if(!BeginRender())return;

	BOOL bMassStorage=IsMassStorageMediaType(m_idCurMediaType);
	BOOL bControlEnable = (m_bEnableOperate && !m_bRefreshingLib);
	//auto return:
	if(m_bAutoReturn)
	{
		if(GetTickCount() - m_dwRefreshTick >/*8000*/30000 && bControlEnable && m_bPlayingFlag)
		{
			m_bAutoReturn = FALSE;
			if(IsMassStorageMediaType(m_idCurMediaType))
			{
				if(m_mediaCategory == MEDIA_VIDEO || m_mediaCategory == MEDIA_AUDIO)
				{
					SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_RIGHT);
				}
			}
			else if(m_idCurMediaType == MEDIA_TYPE_A2DP)
			{
				SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_RIGHT);
			}
		}
	}

// 	DrawRect(pTexBack,0,15,800-0,350-0,16,&cr,TRUE);

// 	m_tagSeperator1.SetAlpha(m_fCurAlpha);
// 	m_tagSeperator1.Render();

// 	m_btnRefresh.SetAlpha(m_fCurAlpha);
// 	m_btnRefresh.Render();

// 	m_barTitle.SetAlpha(m_fCurAlpha);
// 	m_barTitle.Render();

	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		BOOL bOnSearch=m_boxLibrary.IsOnQuickSearchMode();

		m_boxLibrary.EnableNotify(bControlEnable && bMassStorage);
		m_boxDevice.EnableNotify(bControlEnable && !bOnSearch );
		m_btnNowPlaying.EnableNotify(bControlEnable && !bOnSearch );
		m_btnHome.EnableNotify(bControlEnable && !bOnSearch );
		m_btnCatalogAudio.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnCatalogVideo.EnableNotify(bControlEnable && !bOnSearch && bMassStorage && m_bEnableVideo);
		m_btnCatalogImage.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
// 		m_btnCatalogArtist.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnSearch.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnBrowseMode.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);

#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
		m_sliProgress.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
#endif
		m_barPlayingFlag.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
	}
	else if(m_type==MEDIA_LIBRARY_SEARCH_RESULT)
	{
		BOOL bOnSearch=m_boxSearched.IsOnQuickSearchMode();

		m_boxSearched.EnableNotify(bControlEnable && bMassStorage);
		m_boxDevice.EnableNotify(bControlEnable && !bOnSearch );
		m_btnNowPlaying.EnableNotify(bControlEnable && !bOnSearch );
		m_btnHome.EnableNotify(bControlEnable && !bOnSearch );
		m_btnCatalogAudio.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnCatalogVideo.EnableNotify(bControlEnable && !bOnSearch && bMassStorage && m_bEnableVideo);
		m_btnCatalogImage.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
// 		m_btnCatalogArtist.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnSearch.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnBrowseMode.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);

#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
		m_sliProgress.EnableNotify(FALSE);
#endif
		m_barPlayingFlag.EnableNotify(FALSE);
	}
	else
	{
		BOOL bOnSearch=m_boxExplorer.IsOnQuickSearchMode();

		m_boxExplorer.EnableNotify(bControlEnable && bMassStorage);
		m_boxDevice.EnableNotify(bControlEnable && !bOnSearch );
		m_btnNowPlaying.EnableNotify(bControlEnable && !bOnSearch );
		m_btnHome.EnableNotify(bControlEnable && !bOnSearch );
		m_btnCatalogAudio.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnCatalogVideo.EnableNotify(bControlEnable && !bOnSearch && bMassStorage && m_bEnableVideo);
		m_btnCatalogImage.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
// 		m_btnCatalogArtist.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnSearch.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnBrowseMode.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
		m_sliProgress.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
#endif
		m_barPlayingFlag.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
	}

	//
	if(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect)
	{
		m_tagBkBlur.SetAlpha(m_fCurAlpha);
		m_tagBkBlur.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkBlur.Render();
	}
	else
	{
		m_tagBkOrigin.SetAlpha(m_fCurAlpha);
		m_tagBkOrigin.SetLuminance(m_fCurLuminance*m_pSkinManager->GetConfig()->GetCurParam()->fBackLuminance);
		m_tagBkOrigin.Render();
	}

	m_boxDevice.SetAlpha(m_fCurAlpha);
	m_boxDevice.Render();

// 	m_boxDevice.Show(!m_bRefreshingLib);

	m_btnNowPlaying.SetAlpha(m_fCurAlpha);
	m_btnNowPlaying.Render();

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	//
// 	if(m_bNeedUpdateSlider && m_bEnableShowSlider && IsReady())
// 	{
// 		RETAILMSG(DEBUG_MEDIA,(_T("**m_bNeedUpdateSlider :%d\r\n"),m_bNeedUpdateSlider));
//
// 		m_bNeedUpdateSlider = FALSE;
// 		if(m_type==MEDIA_LIBRARY_EXPLORER)
// 		{
// 			m_sliProgress.Show(m_boxExplorer.GetCurFocusItem()->IsRenderEnabled());
// 			m_sliProgress.SetPos(124.0f,m_boxExplorer.GetCurFocusItemPosY()-86);
//
// 		}
// 		else if(m_type==MEDIA_LIBRARY_TRACKS)
// 		{
// 			m_sliProgress.Show(m_boxLibrary.GetCurFocusItem()->IsRenderEnabled());
// 			m_sliProgress.SetPos(124.0f,m_boxLibrary.GetCurFocusItemPosY()-86);
// 		}
// 	}


	BOOL bShowLoadingFlag=(m_mediaCategory==MEDIA_VIDEO)?(!bControlEnable):(m_bRefreshingLib);

	{
		//box:
		if(m_type==MEDIA_LIBRARY_TRACKS)
		{
			if(!bShowLoadingFlag)
			{
// 				m_boxLibrary.Show(!m_bRefreshingLib);
				m_boxLibrary.SetAlpha(m_fCurAlpha);
				m_boxLibrary.Render();
			}

			m_tagNote.Show(bMassStorage?(m_boxLibrary.GetCount()==0 && bControlEnable):bControlEnable);
			m_tagNote.SetAlpha(m_fCurAlpha);
			m_tagNote.Render();

		}
		else if(m_type==MEDIA_LIBRARY_SEARCH_RESULT)
		{
			if(!bShowLoadingFlag)
			{
				m_boxSearched.SetAlpha(m_fCurAlpha);
				m_boxSearched.Render();
			}

			m_tagNote.Show(bMassStorage?(m_boxSearched.GetCount()==0 && bControlEnable):bControlEnable);
			m_tagNote.SetAlpha(m_fCurAlpha);
			m_tagNote.Render();

			if(m_boxSearched.GetCount()==0 && GetTickCount()-m_dwSetShowTypeTick > 3000)
			{
				SetShowType(MEDIA_LIBRARY_TRACKS);
			}
		}
		else if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			if(!bShowLoadingFlag)
			{
// 				m_boxExplorer.Show(TRUE);
				m_boxExplorer.SetAlpha(m_fCurAlpha);
				m_boxExplorer.Render();
			}

			m_tagNote.Show(bMassStorage?(m_boxExplorer.GetCount()==0 && bControlEnable):bControlEnable);
			m_tagNote.SetAlpha(m_fCurAlpha);
			m_tagNote.Render();
		}

		//slider:
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			if(bControlEnable && bMassStorage && m_boxExplorer.GetCurFocusItem())
			{
				if(m_boxExplorer.GetCurFocusItemID() == MEDIA_FILE_TYPE_FOLDER)
				{
                    RETAILMSG(1, (TEXT("MSG:Media:EXPLORER: m_barPlayingFlag.Show(FALSE)\r\n")));
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
					m_barPlayingFlag.Show(FALSE);
					m_barPlayingFlag.Enable(FALSE);
#else
					m_sliProgress.Show(FALSE);
					m_barPlayingFlag.Show(FALSE);
					m_barPlayingFlag.Enable(FALSE);
#endif
				}
				else
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
					//
					m_barPlayingFlag.Show(m_bPlayingFlag && m_boxExplorer.GetCurFocusItem()->IsRenderEnabled() &&!m_boxExplorer.IsOnQuickSearchMode() && m_boxExplorer.GetCurFocusItemPosY()-39 < 160);
					m_barPlayingFlag.SetPos(g_iClientWidth/2-50,m_boxExplorer.GetCurFocusItemPosY()-37);
					m_barPlayingFlag.Enable(TRUE);

					m_barPlayingFlag.SetAlpha(m_fCurAlpha);
					m_barPlayingFlag.Render();
#else
					//
					m_sliProgress.Show(m_boxExplorer.GetCurFocusItem()->IsRenderEnabled() && !m_boxExplorer.IsOnQuickSearchMode() && !m_boxExplorer.IsKeywordBtnVisible() );
					m_sliProgress.SetPos(116.0f,m_boxExplorer.GetCurFocusItemPosY()-69);

					m_sliProgress.SetAlpha(m_fCurAlpha);
					m_sliProgress.Render();

					//
    				m_barPlayingFlag.Show(m_bPlayingFlag && m_boxExplorer.GetCurFocusItem()->IsRenderEnabled() &&!m_boxExplorer.IsOnQuickSearchMode() && m_boxExplorer.GetCurFocusItemPosY()-39 < 160);
					m_barPlayingFlag.SetPos(-g_iClientWidth/2+210+34,m_boxExplorer.GetCurFocusItemPosY()-37);
					m_barPlayingFlag.Enable(TRUE);

					m_barPlayingFlag.SetAlpha(m_fCurAlpha);
					m_barPlayingFlag.Render();
#endif
				}
			}
		}
		else if(m_type==MEDIA_LIBRARY_TRACKS)
		{
			if(bControlEnable && bMassStorage && m_boxLibrary.GetCurFocusItem())
			{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				//
				//RETAILMSG(1, (TEXT("MSG:Media:TRACKS: m_bPlayingFlag[%d] IsRenderEnabled[%d] IsOnQuickSearchMode[%d] GetCurFocusItemPosY[%d]\r\n"),m_bPlayingFlag,m_boxLibrary.GetCurFocusItem()->IsRenderEnabled(),m_boxLibrary.IsOnQuickSearchMode(),m_boxLibrary.GetCurFocusItemPosY()));
				m_barPlayingFlag.Show(m_bPlayingFlag && m_boxLibrary.GetCurFocusItem()->IsRenderEnabled() && !m_boxLibrary.IsOnQuickSearchMode() && m_boxLibrary.GetCurFocusItemPosY()-39 < 160);
				m_barPlayingFlag.SetPos(g_iClientWidth/2-50,m_boxLibrary.GetCurFocusItemPosY()-37);

				m_barPlayingFlag.SetAlpha(m_fCurAlpha);
				m_barPlayingFlag.Render();
#else
				m_sliProgress.Show(m_boxLibrary.GetCurFocusItem()->IsRenderEnabled() && !m_boxLibrary.IsOnQuickSearchMode() && !m_boxLibrary.IsKeywordBtnVisible());
				m_sliProgress.SetPos(116.0f,m_boxLibrary.GetCurFocusItemPosY()-69);

				m_sliProgress.SetAlpha(m_fCurAlpha);
				m_sliProgress.Render();

				//
               // RETAILMSG(1, (TEXT("MSG:Media:TRACKS: m_bPlayingFlag[%d] IsRenderEnabled[%d] IsOnQuickSearchMode[%d] GetCurFocusItemPosY[%d]\r\n"),m_bPlayingFlag,m_boxLibrary.GetCurFocusItem()->IsRenderEnabled(),m_boxLibrary.IsOnQuickSearchMode(),m_boxLibrary.GetCurFocusItemPosY()));
				m_barPlayingFlag.Show(m_bPlayingFlag && m_boxLibrary.GetCurFocusItem()->IsRenderEnabled() && !m_boxLibrary.IsOnQuickSearchMode() && m_boxLibrary.GetCurFocusItemPosY()-39 < 160);
				m_barPlayingFlag.SetPos(-g_iClientWidth/2+210+34,m_boxLibrary.GetCurFocusItemPosY()-37);

				m_barPlayingFlag.SetAlpha(m_fCurAlpha);
				m_barPlayingFlag.Render();
#endif
			}
		}

	}

	//
	m_btnCatalogVideo.Enable(bMassStorage && m_bEnableVideo);

	if(bMassStorage)
	{
		m_btnCatalogAudio.SetAlpha(m_fCurAlpha);
		m_btnCatalogAudio.Render();
		m_btnCatalogVideo.SetAlpha(m_fCurAlpha);
		m_btnCatalogVideo.Render();
		m_btnCatalogImage.SetAlpha(m_fCurAlpha);
		m_btnCatalogImage.Render();
		m_btnBrowseMode.SetAlpha(m_fCurAlpha);
		m_btnBrowseMode.Render();

	// 	m_btnCatalogArtist.SetAlpha(m_fCurAlpha);
	// 	m_btnCatalogArtist.Render();

		m_btnSearch.SetAlpha(m_fCurAlpha);
		m_btnSearch.Render();
	}

// 	m_tagLine.SetAlpha(m_fCurAlpha);
// 	m_tagLine.Render();

	SetFrameTime(bShowLoadingFlag?30:100);

	//SetFrameTime(100);


	if(/*!bControlEnable*/bShowLoadingFlag)
	{
// 		COLORGL cr={0.0,0.0,0.0,0.8};
// 		DrawRect(0,0,g_iClientWidth,g_iClientHeight,&cr);

		VIEW_STATE vs=m_iconLoading.GetCurViewState();
		vs.fRotateZ-=5;
		if(vs.fRotateZ<-360)
			vs.fRotateZ=0;
		m_iconLoading.SetCurViewState(&vs);
		m_iconLoading.SetAlpha(m_fCurAlpha);
		m_iconLoading.Render();
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	if((m_idCurMediaType == MEDIA_TYPE_USB1 || m_idCurMediaType == MEDIA_TYPE_USB2) && (m_idLoadingStatus == LOAD_STATUS_FAILED_NO_FILE || (bMassStorage?(m_boxExplorer.GetCount()==0 && bControlEnable):bControlEnable)))
	{
		if(bNoValidFileCanPlay)
		{
			DWORD dwTick = GetTickCount();
			if(dwTick - m_dwNoValidFileTick > 3000)
			{
				//Auto return to Radio
				bNoValidFileCanPlay = FALSE;
				SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_DEFAULT);
			}
		}
		else
		{
			bNoValidFileCanPlay = TRUE;
			m_dwNoValidFileTick = GetTickCount();
		}
	}
	else
	{
		bNoValidFileCanPlay = FALSE;
	}
#endif
	EndRender();

}
BOOL CPanelMediaLibrary::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	RETAILMSG(DEBUG_MEDIA, (_T("APP:MSG:GUI:Media: [CPanelMediaLibrary::Initialize]: \r\n")));

	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);

	m_hRefreshEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr={0.0f/255.0f,122.0f/255.0f,223.0f/255.0f,1.0};
	m_barPlayingFlag.Initialize(IDC_MEDIA_BAR_PLAYING_FLAG,this,pSkinManager,pTextGL);
	m_barPlayingFlag.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_barPlayingFlag.SetColor(&cr);

	SIZE si={64,64};
	VIEW_STATE vs;

	m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2000)));
	si=m_tagBkBlur.GetTexture()->GetImageSize();
	m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkBlur.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkBlur.SetIniViewState(&vs);

	m_tagBkOrigin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2001)));
	si=m_tagBkOrigin.GetTexture()->GetImageSize();
	m_tagBkOrigin.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkOrigin.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkOrigin.SetIniViewState(&vs);

	
	m_iconArrowOn.Initialize(0, this, m_pSkinManager->GetTexture(TEXID_083));
	m_iconArrowOn.SetShowScale(0.8f);
	m_iconArrowOff.Initialize(0, this,  m_pSkinManager->GetTexture(TEXID_068));

	m_iconDevBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1503));
	m_iconDevBTFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1504));

	m_iconDevUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_252));
	m_iconDevUSBFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_251));

	m_iconDevSD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_249));
	m_iconDevSDFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_248));

	m_iconDevCD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_258));
	m_iconDevCDFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_257));

	m_iconDeviPod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_267));
	m_iconDeviPodFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_266));

	m_iconAudio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_244));
	m_iconAudioFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_243));

	m_iconVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_270));
	m_iconVideoFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_269));

	m_iconImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_264));
	//m_iconImageFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_263));

	m_iconFolder.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_275));
	m_iconFolderFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_274));

	m_iconExplorer.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_275));
	m_iconExplorerFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_274));

	m_iconSearch.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_272));
	//m_iconSearchFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_273));

	m_iconCatalogAudio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_244));
	m_iconCatalogAudioFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_243));
	m_iconCatalogVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_270));
	m_iconCatalogVideoFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_269));
	m_iconCatalogImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_264));
	m_iconCatalogImageFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_263));

	COLORGL cr_gray={128.0f/256.0f,129.0f/256.0f,132.0f/256.0f,1.0};
	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
	COLORGL cr_highlight={1.0f,1.0f,1.0f,1.0};

	int cx=LIST_BOX_ITEM_WIDTH;	//186
	int cy=LIST_BOX_ITEM_HEIGHT;

	COLORGL crb={0.9,0.9,0.9,1.0};
	SIZE siInfo={600,40};

	//
	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(1.44f);
	m_iconLoading.SetPos(cx/2,-cy/2);

	m_tagNote.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNote.SetSize(&siInfo);
	m_tagNote.SetCaption(GetResourceString(IDS_NO_MEDIA_FILES));
	m_tagNote.LinkTextGL(pTextGL);
	m_tagNote.SetPos(cx/2,-cy/2+80);
	m_tagNote.SetCaptionHeight(32);
	m_tagNote.EnableDrawBack(FALSE);
	m_tagNote.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNote.SetCaptionColor(&crb);


	int cx_btn=(g_iClientWidth-cx)/5;
	short category_font = 14;
	SIZE sic={cx_btn,cy+0};

	m_mediaCategory =MEDIA_CATEGORY(m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory);

	m_btnCatalogAudio.Initialize(IDC_MEDIA_BTN_CATALOG_AUDIO,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogAudio.SetSize(&sic);
	m_btnCatalogAudio.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnCatalogAudio.SetPos(cx/2-cx_btn*2,g_iClientHeight/2-cy/2);
	m_btnCatalogAudio.LinkTextGL(pTextGL);
	m_btnCatalogAudio.SetIcon(&m_iconCatalogAudio);
	m_btnCatalogAudio.SetCaption(GetResourceString(IDS_TITLE_MUSIC));
	m_btnCatalogAudio.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnCatalogAudio.SetCaptionColor(&cr_gray);
	m_btnCatalogAudio.SetCaptionHeight(category_font);
	m_btnCatalogAudio.SetIconPos(-sic.cx/2+32,0);

	m_btnCatalogVideo.Initialize(IDC_MEDIA_BTN_CATALOG_VIDEO,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogVideo.SetSize(&sic);
	m_btnCatalogVideo.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnCatalogVideo.SetPos(cx/2-cx_btn,g_iClientHeight/2-cy/2);
	m_btnCatalogVideo.LinkTextGL(pTextGL);
	m_btnCatalogVideo.SetIcon(&m_iconCatalogVideo);
	m_btnCatalogVideo.SetCaption(GetResourceString(IDS_TITLE_VIDEO));
	m_btnCatalogVideo.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnCatalogVideo.SetCaptionColor(&cr_gray);
	m_btnCatalogVideo.SetCaptionHeight(category_font);
	m_btnCatalogVideo.SetIconPos(-sic.cx/2+32,0);

	m_btnCatalogImage.Initialize(IDC_MEDIA_BTN_CATALOG_IMAGE,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogImage.SetSize(&sic);
	m_btnCatalogImage.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnCatalogImage.SetPos(cx/2-0,g_iClientHeight/2-cy/2);
	m_btnCatalogImage.LinkTextGL(pTextGL);
	m_btnCatalogImage.SetIcon(&m_iconCatalogImage);
	m_btnCatalogImage.SetCaption(GetResourceString(IDS_TITLE_IMAGE));
	m_btnCatalogImage.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnCatalogImage.SetCaptionColor(&cr_gray);
    m_btnCatalogImage.SetCaptionHeight(category_font);
	m_btnCatalogImage.SetIconPos(-sic.cx/2+32,0);

	//
	m_btnBrowseMode.Initialize(IDC_MEDIA_BTN_BROWSE_MODE,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBrowseMode.SetSize(&sic);
	m_btnBrowseMode.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnBrowseMode.SetPos(cx/2+cx_btn,g_iClientHeight/2-cy/2);
	m_btnBrowseMode.LinkTextGL(pTextGL);

#if LIB_BROWSE_BTN_DISPLAY_REVERSE
	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		m_btnBrowseMode.SetIcon(&m_iconExplorer);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
	}
	else
	{
		m_btnBrowseMode.SetIcon(m_mediaCategory == MEDIA_AUDIO?&m_iconCatalogAudio:&m_iconCatalogVideo);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
#else
	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		m_btnBrowseMode.SetIcon(m_mediaCategory == MEDIA_AUDIO?&m_iconCatalogAudio:&m_iconCatalogVideo);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
	else
	{
		m_btnBrowseMode.SetIcon(&m_iconExplorer);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
	}
#endif
	m_btnBrowseMode.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnBrowseMode.SetCaptionColor(&cr_gray);
	m_btnBrowseMode.SetCaptionHeight(category_font);
	m_btnBrowseMode.SetIconPos(-sic.cx/2+32,0);

	//
	m_btnSearch.Initialize(IDC_MEDIA_BTN_SEARCH,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearch.SetSize(&sic);
	m_btnSearch.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnSearch.SetPos(cx/2+cx_btn*2,g_iClientHeight/2-cy/2);
	m_btnSearch.SetIcon(&m_iconSearch);
	m_btnSearch.SetIconPos(-sic.cx/2+32,0);
	m_btnSearch.LinkTextGL(pTextGL);
	m_btnSearch.SetCaption(GetResourceString(IDS_SEARCH));
	m_btnSearch.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnSearch.SetCaptionColor(&cr_gray);
	m_btnSearch.SetCaptionHeight(category_font);
	


	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));

	//find param:
	int count=(g_iClientHeight-cy*2)/cy;
	if((g_iClientHeight-cy*2)%cy > (cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy*2)/count/2*2;
	box.siClient.cx = cx;	//176;	//cx;
	box.siClient.cy = (box.iSpacing * count);	//78;	//(box.iSpacing * count);
	box.siBorder.cx = cx;	//182;	//cx;
	box.siBorder.cy = g_iClientHeight-cy*2;	//80;	//g_iClientHeight-cy*2;//(box.iSpacing * count);
	box.idTexIndexBase=TEXID_289;
	box.idTexIndexFocus=TEXID_242;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_285;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;	//TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxDevice.Initialize(IDC_MEDIA_BOX_DEVICE,this,&box,pSkinManager);
	m_boxDevice.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_boxDevice.SetPos(-g_iClientWidth/2+cx/2,0);
	m_boxDevice.LinkTextGL(pTextGL);
	m_boxDevice.Show(TRUE,FALSE);
	m_boxDevice.SetAlpha(1.0f);
	m_boxDevice.SetCaptioniHeight(18);
	m_boxDevice.SetCaptionColor(&cr_gray,&cr_w);


	//
	memset(&box,0,sizeof(box));

	box.iSpacing = (g_iClientHeight - cy)/5;
	box.siClient.cx = g_iClientWidth-cx;
	box.siClient.cy = g_iClientHeight - cy;	
	box.siBorder.cx = g_iClientWidth-cx;	
	box.siBorder.cy = g_iClientHeight - cy;
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


	m_boxLibrary.Initialize(IDC_MEDIA_BOX_LIBRARY,this,&box,pSkinManager);
	m_boxLibrary.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
    m_boxLibrary.SetPos(cx/2, -cy/2);
	m_boxLibrary.LinkTextGL(pTextGL);
	m_boxLibrary.Show(TRUE,FALSE);
	m_boxLibrary.SetCaptionColor(&cr_gray,&cr_highlight);
    m_boxLibrary.SetCaptioniHeight(22);


	//
	m_boxExplorer.Initialize(IDC_MEDIA_BOX_EXPLORER,this,&box,pSkinManager);
	m_boxExplorer.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_boxExplorer.SetPos(cx/2,-g_iClientHeight/2+203);
	m_boxExplorer.LinkTextGL(pTextGL);
	m_boxExplorer.Show(TRUE,FALSE);
	m_boxExplorer.SetCaptionColor(&cr_gray,&cr_highlight);
    m_boxExplorer.SetCaptioniHeight(22);

	m_boxSearched.Initialize(IDC_MEDIA_BOX_SEARCH_RESULT,this,&box,pSkinManager);
	m_boxSearched.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_boxSearched.SetPos(cx/2,-cy/2);
	m_boxSearched.LinkTextGL(pTextGL);
	m_boxSearched.Show(FALSE,FALSE);
	m_boxSearched.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxSearched.SetCaptioniHeight(22);

	SetShowType(m_type);
// 	SetCurPath(m_strCurPath.String());
// 	SetDevice(DEV_NAME_USB1);

	SIZE sibtn={cx,cy};

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_204));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));
	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_280));
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_279));

	m_btnHome.Initialize(IDC_MEDIA_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_288),NULL,
		m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL);
    //m_btnHome.SetShowScale(0.9f);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.SetIconPos(-cx/2+35,0);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,68);
	m_btnHome.SetCaptionColor(&cr_w,&cr_gray);
	m_btnHome.SetCaptionHeight(20);


	m_btnNowPlaying.Initialize(IDC_MEDIA_BTN_NOWPLAYING,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
    //m_btnNowPlaying.SetShowScale(0.9f);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnNowPlaying.SetPos(-g_iClientWidth/2+cx/2,g_iClientHeight/2-cy/2);
	m_btnNowPlaying.SetIcon(&m_iconNowPlayingUp,&m_iconNowPlayingDown);
	m_btnNowPlaying.SetIconPos(-cx/2+14+16,0);
	m_btnNowPlaying.LinkTextGL(pTextGL);
	m_btnNowPlaying.SetCaption(GetResourceString(IDS_TITLE_NOWPLAYING));
	m_btnNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT,58);
	m_btnNowPlaying.SetCaptionColor(&cr_gray,&cr_w);

    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
        case LANGUAGE_SPANISH:
    	    m_btnNowPlaying.SetCaptionHeight(14);
            break;
        case LANGUAGE_RUSSIAN:
            m_btnNowPlaying.SetCaptionHeight(14);
            break;
        case LANGUAGE_ITALIAN:
            m_btnNowPlaying.SetCaptionHeight(14);
            break;
        default:
	        m_btnNowPlaying.SetCaptionHeight(20);
            break;
    }

#else
	COLORGL cr={208.0f/255.0f,55.0f/255.0f,61.0f/255.0f,1.0};
	m_barPlayingFlag.Initialize(IDC_MEDIA_BAR_PLAYING_FLAG,this,pSkinManager,pTextGL);
	m_barPlayingFlag.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_barPlayingFlag.SetColor(&cr);
    //m_barPlayingFlag.SetShowScale(0.8f);

	//
	SIZE si={64,64};
	VIEW_STATE vs;

	m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2000)));
	si=m_tagBkBlur.GetTexture()->GetImageSize();
	m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkBlur.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkBlur.SetIniViewState(&vs);

	m_tagBkOrigin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID(2001)));
	si=m_tagBkOrigin.GetTexture()->GetImageSize();
	m_tagBkOrigin.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkOrigin.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkOrigin.SetIniViewState(&vs);

	//
	m_iconDevBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_207));
    m_iconDevBT.SetShowScale(0.8f);
	m_iconDevUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_209));
    m_iconDevUSB.SetShowScale(0.8f);
	m_iconDevSD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_211));
    m_iconDevSD.SetShowScale(0.8f);
	m_iconDevCD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_213));
	m_iconDevCD.SetShowScale(0.8f);
	m_iconDeviPod.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_254));
	m_iconDeviPod.SetShowScale(0.8f);

	m_iconDevBTFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_208));
    m_iconDevBTFocus.SetShowScale(0.8f);
	m_iconDevUSBFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_210));
    m_iconDevUSBFocus.SetShowScale(0.8f);
	m_iconDevSDFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_212));
    m_iconDevSDFocus.SetShowScale(0.8f);
	m_iconDevCDFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_214));
    m_iconDevCDFocus.SetShowScale(0.8f);
	m_iconDeviPodFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_253));
	m_iconDeviPodFocus.SetShowScale(0.8f);
	m_iconVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_225));
    m_iconVideo.SetShowScale(0.8f);
	m_iconAudio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_217));
    m_iconAudio.SetShowScale(0.8f);
	m_iconImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_226));
    m_iconImage.SetShowScale(0.8f);
	m_iconFolder.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_224));
    m_iconFolder.SetShowScale(0.8f);
	m_iconExplorer.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_223));
    m_iconExplorer.SetShowScale(0.8f);
	m_iconSearch.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_222));
    m_iconSearch.SetShowScale(0.8f);

	m_iconAudioFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(/*TEXID_068*/TEXID_204));
	m_iconVideoFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(/*TEXID_068*/TEXID_204));

	m_iconCatalogAudio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_219));
    m_iconCatalogAudio.SetShowScale(0.8f);
	m_iconCatalogVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_220));
    m_iconCatalogVideo.SetShowScale(0.8f);
	m_iconCatalogImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_227));
    m_iconCatalogImage.SetShowScale(0.8f);

// 	m_btnExplorer.Initialize(IDC_MEDIA_BTN_EXPLORER,this,m_pSkinManager->GetTexture(TEXID_149),NULL,
// 		m_pSkinManager->GetTexture(TEXID_150),NULL,
// 		NULL,NULL,
// 		NULL,NULL);
// 	m_btnExplorer.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
// 	m_btnExplorer.SetPos(-350,-31);
// 	m_btnExplorer.SetIcon(&m_iconExplorer);
// 	m_btnExplorer.SetIconScale(0.7,0.7);
// 	m_btnExplorer.SetShowScale(1.25);
//
// 	m_btnTracks.Initialize(IDC_MEDIA_BTN_TRACKS,this,m_pSkinManager->GetTexture(TEXID_149),NULL,
// 		m_pSkinManager->GetTexture(TEXID_150),NULL,
// 		NULL,NULL,
// 		NULL,NULL);
// 	m_btnTracks.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
// 	m_btnTracks.SetPos(-350,60);
// 	m_btnTracks.SetIcon(&m_iconTracksAudio);
// 	m_btnTracks.SetIconScale(0.7,0.7);
// 	m_btnTracks.SetShowScale(1.25);

// 	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
// 	COLORGL cr_gray={176.0f/256.0f,109.0f/256.0f,142.0f/256.0f,1.0};
// 	COLORGL cr_gray={189.0f/256.0f,168.0f/256.0f,173.0f/256.0f,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
	COLORGL cr_highlight={255.0f/255.0f,63.0f/255.0f,63.0f/255.0f,1.0};

	int cx=LIST_BOX_ITEM_WIDTH;//234    //240;
	int cy=LIST_BOX_ITEM_HEIGHT;//100

	COLORGL crb={0.9,0.9,0.9,1.0};
	SIZE siInfo={600,40};


	//slider
	SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=g_iClientWidth-cx-109;//520/*776*/;
	sli.size.cy=10;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_052;
	sli.idChannelTex=TEXID_053;
	sli.idThumbTex=0;//TEXID_051;
	m_sliProgress.Initialize(IDC_MEDIA_SLI_PROGRESS,this,&sli,pSkinManager);
	m_sliProgress.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_sliProgress.SetRange(0,500);
	m_sliProgress.SetValue(0);
	m_sliProgress.SetAutoHideBtns(TRUE);
	m_sliProgress.SetPos(116.0f,-g_iClientHeight/2+140);
	m_sliProgress.ShowThumb(FALSE);
	m_sliProgress.SetAutoBackFill(TRUE);
	m_sliProgress.Show(FALSE);

	//
	m_iconLoading.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_713));
	m_iconLoading.SetShowScale(1.44f);
	m_iconLoading.SetPos(cx/2,-cy/2);

	m_tagNote.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNote.SetSize(&siInfo);
	m_tagNote.SetCaption(GetResourceString(IDS_NO_MEDIA_FILES));
	m_tagNote.LinkTextGL(pTextGL);
	m_tagNote.SetPos(cx/2,-cy/2+80);
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_GREEK:
		m_tagNote.SetCaptionHeight(28);
		break;
	default:
		m_tagNote.SetCaptionHeight(32);
		break;
	}
	m_tagNote.EnableDrawBack(FALSE);
	m_tagNote.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNote.SetCaptionColor(&crb);


	int cx_btn=(g_iClientWidth-cx)/5;
	SIZE sic={cx_btn,cy+0};

	m_mediaCategory =MEDIA_CATEGORY(m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory);

	/*
	m_btnCatalogMusic.Initialize(IDC_MEDIA_BTN_CATALOG_MUSIC,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogMusic.SetSize(&sic);
	m_btnCatalogMusic.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnCatalogMusic.SetPos(-(g_iClientWidth-cx)/4+cx/2,g_iClientHeight/2-cy/2);
	m_btnCatalogMusic.LinkTextGL(pTextGL);
	if(m_mediaCategory == MEDIA_AUDIO)
	{
		m_btnCatalogMusic.SetIcon(&m_iconCatalogMusic);
		m_btnCatalogMusic.SetCaption(GetResourceString(IDS_TITLE_MUSIC));
	}
	else if(m_mediaCategory == MEDIA_VIDEO)
	{
		m_btnCatalogMusic.SetIcon(&m_iconCatalogVideo);
		m_btnCatalogMusic.SetCaption(GetResourceString(IDS_TITLE_VIDEO));
	}
	m_btnCatalogMusic.SetCaptionPos(BTN_CAPTION_CENTER,0,-26);
	m_btnCatalogMusic.SetCaptionColor(&cr_gray);
	m_btnCatalogMusic.SetCaptionHeight(18);
	m_btnCatalogMusic.SetIconPos(0,10);
	*/

	/*
	m_iconCatalogArtist.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_221));

	m_btnCatalogArtist.Initialize(IDC_MEDIA_BTN_CATALOG_ARTIST,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogArtist.SetSize(&sic);
	m_btnCatalogArtist.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnCatalogArtist.SetPos(cx/2,g_iClientHeight/2-cy/2);
	m_btnCatalogArtist.SetIcon(&m_iconCatalogArtist);
	m_btnCatalogArtist.LinkTextGL(pTextGL);
	m_btnCatalogArtist.SetCaption(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
	m_btnCatalogArtist.SetCaptionPos(BTN_CAPTION_CENTER,0,-26);
	m_btnCatalogArtist.SetCaptionColor(&cr_gray);
	m_btnCatalogArtist.SetCaptionHeight(18);
	m_btnCatalogArtist.SetIconPos(0,10);
*/

	m_btnCatalogAudio.Initialize(IDC_MEDIA_BTN_CATALOG_AUDIO,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogAudio.SetSize(&sic);
    //m_btnCatalogAudio.SetShowScale(0.9f);
	m_btnCatalogAudio.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnCatalogAudio.SetPos(cx/2-cx_btn*2,g_iClientHeight/2-cy/2);
	m_btnCatalogAudio.LinkTextGL(pTextGL);
	m_btnCatalogAudio.SetIcon(&m_iconCatalogAudio);
	m_btnCatalogAudio.SetCaption(GetResourceString(IDS_TITLE_MUSIC));
	m_btnCatalogAudio.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnCatalogAudio.SetCaptionColor(&cr_gray);
	m_btnCatalogAudio.SetCaptionHeight(16);
	m_btnCatalogAudio.SetIconPos(0,10);

	m_btnCatalogVideo.Initialize(IDC_MEDIA_BTN_CATALOG_VIDEO,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogVideo.SetSize(&sic);
    //m_btnCatalogVideo.SetShowScale(0.9f);
	m_btnCatalogVideo.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnCatalogVideo.SetPos(cx/2-cx_btn,g_iClientHeight/2-cy/2);
	m_btnCatalogVideo.LinkTextGL(pTextGL);
	m_btnCatalogVideo.SetIcon(&m_iconCatalogVideo);
	m_btnCatalogVideo.SetCaption(GetResourceString(IDS_TITLE_VIDEO));
	m_btnCatalogVideo.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnCatalogVideo.SetCaptionColor(&cr_gray);
	m_btnCatalogVideo.SetCaptionHeight(16);
	m_btnCatalogVideo.SetIconPos(0,10);

	m_btnCatalogImage.Initialize(IDC_MEDIA_BTN_CATALOG_IMAGE,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogImage.SetSize(&sic);
    //m_btnCatalogImage.SetShowScale(0.9f);
	m_btnCatalogImage.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnCatalogImage.SetPos(cx/2-0,g_iClientHeight/2-cy/2);
	m_btnCatalogImage.LinkTextGL(pTextGL);
	m_btnCatalogImage.SetIcon(&m_iconCatalogImage);
	m_btnCatalogImage.SetCaption(GetResourceString(IDS_TITLE_IMAGE));
	m_btnCatalogImage.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnCatalogImage.SetCaptionColor(&cr_gray);

    if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
    	m_btnCatalogImage.SetCaptionHeight(14);
    else
    	m_btnCatalogImage.SetCaptionHeight(16);

	m_btnCatalogImage.SetIconPos(0,10);

	//
	m_btnBrowseMode.Initialize(IDC_MEDIA_BTN_BROWSE_MODE,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBrowseMode.SetSize(&sic);
    //m_btnBrowseMode.SetShowScale(0.9f);
	m_btnBrowseMode.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnBrowseMode.SetPos(cx/2+cx_btn,g_iClientHeight/2-cy/2);
	m_btnBrowseMode.LinkTextGL(pTextGL);
#if LIB_BROWSE_BTN_DISPLAY_REVERSE
	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		m_btnBrowseMode.SetIcon(&m_iconExplorer);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
	}
	else
	{
		m_btnBrowseMode.SetIcon(m_mediaCategory == MEDIA_AUDIO?&m_iconCatalogAudio:&m_iconCatalogVideo);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
#else
	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		m_btnBrowseMode.SetIcon(m_mediaCategory == MEDIA_AUDIO?&m_iconCatalogAudio:&m_iconCatalogVideo);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
	else
	{
		m_btnBrowseMode.SetIcon(&m_iconExplorer);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
	}
#endif

	m_btnBrowseMode.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnBrowseMode.SetCaptionColor(&cr_gray);
	m_btnBrowseMode.SetCaptionHeight(16);
	m_btnBrowseMode.SetIconPos(0,10);

	//
	m_btnSearch.Initialize(IDC_MEDIA_BTN_SEARCH,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearch.SetSize(&sic);
    //m_btnSearch.SetShowScale(0.9f);
	m_btnSearch.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnSearch.SetPos(cx/2+cx_btn*2,g_iClientHeight/2-cy/2);
	m_btnSearch.SetIcon(&m_iconSearch);
	m_btnSearch.LinkTextGL(pTextGL);
	m_btnSearch.SetCaption(GetResourceString(IDS_SEARCH));
	m_btnSearch.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnSearch.SetCaptionColor(&cr_gray);
	m_btnSearch.SetCaptionHeight(16);
	m_btnSearch.SetIconPos(0,10);


	LISTBOXSTRUCTURE box;


	int ispace=0;

	memset(&box,0,sizeof(box));

	//find param:
	int count=(g_iClientHeight-cy*2)/cy;
	if((g_iClientHeight-cy*2)%cy > (cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy*2)/count/2*2;

	ispace=(g_iClientHeight-cy*2-box.iSpacing*count) / 2;

	box.siClient.cx=cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=cx;
	box.siBorder.cy=g_iClientHeight-cy*2;//(box.iSpacing * count);

	box.idTexIndexBase=TEXID_251;//TEXID_068;//
	box.idTexIndexFocus=TEXID_068;//TEXID_251;//
	box.idTexBase=TEXID_251;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxDevice.Initialize(IDC_MEDIA_BOX_DEVICE,this,&box,pSkinManager);
	m_boxDevice.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_boxDevice.SetPos(-g_iClientWidth/2+cx/2,0);
	m_boxDevice.LinkTextGL(pTextGL);
	m_boxDevice.Show(TRUE,FALSE);
	m_boxDevice.SetAlpha(1.0f);
	m_boxDevice.SetCaptioniHeight(/*24*/22);
	m_boxDevice.SetCaptionColor(&cr_gray,&cr_w);

	CListBoxItemGL *pItem;
// 	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB));
// 	pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus);
// 	//pItem->SetIcon2(&m_iconLibTracks);
// 	m_boxDevice.AddItem(pItem,MEDIA_TYPE_USB1);
// 	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD));
// 	pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus);
// 	//pItem->SetIcon2(&m_iconLibExplorer);
// 	m_boxDevice.AddItem(pItem,MEDIA_TYPE_SD1);

// 	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_BT));
// 	pItem->SetIcon(&m_iconDevBT,&m_iconDevBTFocus);
// 	m_boxDevice.AddItem(pItem,MEDIA_TYPE_A2DP);
// 	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_CD));
// 	pItem->SetIcon(&m_iconDevCD,&m_iconDevCDFocus);
// 	m_boxDevice.AddItem(pItem,MEDIA_TYPE_DISK);

	pItem=new CListBoxItemGL(_T(" "));
	pItem->EnableClick(FALSE);
	m_boxDevice.AddItem(pItem,0);
	pItem=new CListBoxItemGL(_T(" "));
	pItem->EnableClick(FALSE);
	m_boxDevice.AddItem(pItem,0);
	pItem=new CListBoxItemGL(_T(" "));
	pItem->EnableClick(FALSE);
	m_boxDevice.AddItem(pItem,0);
	pItem=new CListBoxItemGL(_T(" "));
	pItem->EnableClick(FALSE);
	m_boxDevice.AddItem(pItem,0);

	//

	memset(&box,0,sizeof(box));

	//find param:
	count=(g_iClientHeight-cy)/65;
	if((g_iClientHeight-cy)%65 > 32)
		count+=1;

	box.iSpacing=(g_iClientHeight-cy)/count/2*2;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=g_iClientHeight-cy;//(box.iSpacing * count);


	box.idTexIndexBase=TEXID_285;
	box.idTexIndexFocus=TEXID_285;//TEXID_361;
	box.idTexBase=TEXID_066;
	box.idTexSliThumb=TEXID_064;
	box.bNoSlider=FALSE;
	box.bNoBorder=TRUE;//FALSE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=TRUE;
	box.pxSlider=-20;
	box.fIndexBaseOffset=60;    //74

	m_boxLibrary.Initialize(IDC_MEDIA_BOX_LIBRARY,this,&box,pSkinManager);
	m_boxLibrary.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_boxLibrary.SetMarginHor(38);
	//m_boxLibrary.SetAlpha(0.9f);
    m_boxLibrary.SetPos(cx/2,-cy/2);
	m_boxLibrary.LinkTextGL(pTextGL);
	m_boxLibrary.Show(TRUE,FALSE);
	m_boxLibrary.SetCaptionColor(&cr_gray,&cr_highlight);
    m_boxLibrary.SetCaptioniHeight(22);


	//
	m_boxExplorer.Initialize(IDC_MEDIA_BOX_EXPLORER,this,&box,pSkinManager);
	m_boxExplorer.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_boxExplorer.SetMarginHor(48);
	m_boxExplorer.SetPos(cx/2,-cy/2);
	m_boxExplorer.LinkTextGL(pTextGL);
	m_boxExplorer.Show(TRUE,FALSE);
	//m_boxExplorer.SetAlpha(1.0f);
	m_boxExplorer.SetCaptionColor(&cr_gray,&cr_highlight);
    m_boxExplorer.SetCaptioniHeight(22);

	m_boxSearched.Initialize(IDC_MEDIA_BOX_SEARCH_RESULT,this,&box,pSkinManager);
	m_boxSearched.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_boxSearched.SetMarginHor(48);
	m_boxSearched.SetPos(cx/2,-cy/2);
	m_boxSearched.LinkTextGL(pTextGL);
	m_boxSearched.Show(FALSE,FALSE);
	m_boxSearched.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxSearched.SetCaptioniHeight(22);

	SetShowType(m_type);
// 	SetCurPath(m_strCurPath.String());
// 	SetDevice(DEV_NAME_USB1);

	SIZE sibtn={cx,cy};

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_215));
	m_iconHomeUp.SetShowScale(0.8f);
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_216));
	m_iconHomeDown.SetShowScale(0.8f);
	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));
    m_iconNowPlayingUp.SetShowScale(0.8f);
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_206));
    m_iconNowPlayingDown.SetShowScale(0.8f);

	m_btnHome.Initialize(IDC_MEDIA_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_251),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
    //m_btnHome.SetShowScale(0.9f);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2+ispace/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_w);
	m_btnHome.SetCaptionHeight(22);
	m_btnHome.SetIconPos(-cx/2+31,0);

	m_btnNowPlaying.Initialize(IDC_MEDIA_BTN_NOWPLAYING,this,m_pSkinManager->GetTexture(TEXID_251),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
    //m_btnNowPlaying.SetShowScale(0.9f);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPanelMediaLibrary::OnNotify,this);
	m_btnNowPlaying.SetPos(-g_iClientWidth/2+cx/2,g_iClientHeight/2-cy/2-ispace/2);
	m_btnNowPlaying.SetIcon(&m_iconNowPlayingUp,&m_iconNowPlayingDown);
	m_btnNowPlaying.LinkTextGL(pTextGL);
	m_btnNowPlaying.SetCaption(GetResourceString(IDS_TITLE_NOWPLAYING));
	m_btnNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnNowPlaying.SetCaptionColor(&cr_gray,&cr_w);

    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
        case LANGUAGE_SPANISH:
    	    m_btnNowPlaying.SetCaptionHeight(15);
            break;
        case LANGUAGE_RUSSIAN:
            m_btnNowPlaying.SetCaptionHeight(15);
            break;
        case LANGUAGE_ITALIAN:
            m_btnNowPlaying.SetCaptionHeight(15);
            break;
        default:
	        m_btnNowPlaying.SetCaptionHeight(22);
            break;
    }
	m_btnNowPlaying.SetIconPos(-cx/2+31,0);
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	RETAILMSG(1, (TEXT("TEST:[%s]: m_mediaCategory:%d\r\n"),TEXT(__FUNCTION__),m_mediaCategory));
	m_btnCatalogAudio.SetCaptionColor(m_mediaCategory == MEDIA_AUDIO?(&cr_w):(&cr_gray));
	m_btnCatalogVideo.SetCaptionColor(m_mediaCategory == MEDIA_VIDEO?(&cr_w):(&cr_gray));
	m_btnCatalogImage.SetCaptionColor(m_mediaCategory == MEDIA_IMAGE?(&cr_w):(&cr_gray));
	m_btnCatalogAudio.SetIcon(m_mediaCategory == MEDIA_AUDIO?(&m_iconCatalogAudioFocus):(&m_iconCatalogAudio));
	m_btnCatalogVideo.SetIcon(m_mediaCategory == MEDIA_VIDEO?(&m_iconCatalogVideoFocus):(&m_iconCatalogVideo));
	m_btnCatalogImage.SetIcon(m_mediaCategory == MEDIA_IMAGE?(&m_iconCatalogImageFocus):(&m_iconCatalogImage));
#else
	//
	m_btnCatalogAudio.SetStatus(m_mediaCategory == MEDIA_AUDIO?BS_FOCUS:BS_UP);
	m_btnCatalogVideo.SetStatus(m_mediaCategory == MEDIA_VIDEO?BS_FOCUS:BS_UP);
	m_btnCatalogImage.SetStatus(m_mediaCategory == MEDIA_IMAGE?BS_FOCUS:BS_UP);
#endif
	return TRUE;
}
void CPanelMediaLibrary::ClearList()
{
	RETAILMSG(DEBUG_MEDIA, (TEXT("TEST:[%s]: ****\r\n"),TEXT(__FUNCTION__)));
	m_boxLibrary.ClearList();
	m_boxExplorer.ClearList();

	m_strCurPath=NULL;
}

void CPanelMediaLibrary::SetMediaCategory(MEDIA_CATEGORY category)
{
	if(m_mediaCategory != category)
	{
		m_mediaCategory = category;

		//reset playfile
		m_strCurPlayFile=NULL;

		m_pSkinManager->GetConfig()->GetCurParam()->idMediaCategory = category;

// 		SendMediaCommand(MEDIA_CMD_CATEGORY,category);

// 		SetShowType(MEDIA_LIBRARY_TRACKS);

		RefreshLibrary();

		//for explorer:
		if( m_idCurMediaType == MEDIA_TYPE_USB1 ) // mass storages
		{
			WCHAR str[MAX_PATH];
			swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_USB1);
			m_strCurRoot=str;
		}
		else if( m_idCurMediaType == MEDIA_TYPE_USB2 ) // mass storages
		{
			WCHAR str[MAX_PATH];
			swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_USB2);
			m_strCurRoot=str;
		}
		else if( m_idCurMediaType == MEDIA_TYPE_SD1 ) // mass storages
		{
			WCHAR str[MAX_PATH];
			swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_MEDIACARD);
			m_strCurRoot=str;
		}
#ifdef USE_GPS_CARD_MEDIA_PLAY
		else if( m_idCurMediaType == MEDIA_TYPE_SD2 ) // mass storages
		{
			WCHAR str[MAX_PATH];
			swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_GPSCARD);
			m_strCurRoot=str;
		}
#endif
		else
		{
			m_strCurRoot = NULL;
		}

		//update explorer path:
		m_strCurPath = NULL;
		if(m_strCurRoot.String())
		{
			SetCurPath(m_strCurRoot.String());
		}

		m_btnCatalogAudio.SetStatus(m_mediaCategory == MEDIA_AUDIO?BS_FOCUS:BS_UP);
		m_btnCatalogVideo.SetStatus(m_mediaCategory == MEDIA_VIDEO?BS_FOCUS:BS_UP);
		m_btnCatalogImage.SetStatus(m_mediaCategory == MEDIA_IMAGE?BS_FOCUS:BS_UP);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		COLORGL cr_gray={128.0f/256.0f,129.0f/256.0f,132.0f/256.0f,1.0};
		COLORGL cr_w={1.0f,1.0f,1.0f,1.0};

		m_btnCatalogAudio.SetCaptionColor(m_mediaCategory == MEDIA_AUDIO?(&cr_w):(&cr_gray));
		m_btnCatalogVideo.SetCaptionColor(m_mediaCategory == MEDIA_VIDEO?(&cr_w):(&cr_gray));
		m_btnCatalogImage.SetCaptionColor(m_mediaCategory == MEDIA_IMAGE?(&cr_w):(&cr_gray));
		m_btnCatalogAudio.SetIcon(m_mediaCategory == MEDIA_AUDIO?(&m_iconCatalogAudioFocus):(&m_iconCatalogAudio));
		m_btnCatalogVideo.SetIcon(m_mediaCategory == MEDIA_VIDEO?(&m_iconCatalogVideoFocus):(&m_iconCatalogVideo));
		m_btnCatalogImage.SetIcon(m_mediaCategory == MEDIA_IMAGE?(&m_iconCatalogImageFocus):(&m_iconCatalogImage));
#endif

		//
		SetShowType(MEDIA_LIBRARY_TYPE(m_pSkinManager->GetConfig()->GetCurParam()->idMediaLibraryType));

// 		//update buttons:
// 		if(m_mediaCategory == MEDIA_AUDIO)
// 		{
// 			m_btnCatalogMusic.SetIcon(&m_iconCatalogMusic);
// 			m_btnCatalogMusic.SetCaption(GetResourceString(IDS_TITLE_MUSIC));
// 		}
// 		else if(m_mediaCategory == MEDIA_VIDEO)
// 		{
// 			m_btnCatalogMusic.SetIcon(&m_iconCatalogVideo);
// 			m_btnCatalogMusic.SetCaption(GetResourceString(IDS_TITLE_VIDEO));
// 		}
	}
}
void CPanelMediaLibrary::EnterCurPath()
{
	RETAILMSG(DEBUG_MEDIA,(_T("****CPanelMediaLibrary::EnterCurPath\r\n")));

	if(m_type==MEDIA_LIBRARY_EXPLORER)
	{
		if(m_boxExplorer.GetCurFocusItemID()==MEDIA_FILE_TYPE_FOLDER)
		{
			wchar_t temp[MAX_PATH];
			if(m_strCurRoot.String())
			{
				RETAILMSG(DEBUG_MEDIA,(_T("****CPanelMediaLibrary::EnterCurPath begin :%s\r\n"),m_boxExplorer.GetCurFocusItemName()));
				if(m_strCurPath.String())
				{
					swprintf_s(temp,MAX_PATH,_T("%s%s\\"),m_strCurPath.String(),m_boxExplorer.GetCurFocusItemName());
				}
				else
				{
					swprintf_s(temp,MAX_PATH,_T("%s%s\\"),m_strCurRoot.String(),m_boxExplorer.GetCurFocusItemName());
				}
				SetCurPath(temp);
				RETAILMSG(DEBUG_MEDIA,(_T("****CPanelMediaLibrary::EnterCurPath end :%s\r\n"),temp));
			}
		}
	}
}
void CPanelMediaLibrary::GotoParent()
{
	CM_String strTemp(m_strCurPath);

	if(IsOnRoot())
		return;
	else if(strTemp.Size()>2)
	{
		m_strCurPath-=1;//cut '\\'
		for(int i=strTemp.Size()-2;i>=0;i--)
		{
			if (*(strTemp.String()+i)!='\\')
			{
				m_strCurPath-=1;
			}
			else
			{
				//m_strCurPath-=1;
				break;
			}
		}
		ExplorePath(m_strCurPath.String());
		//m_bPathChanged=TRUE;
	}

}
BOOL CPanelMediaLibrary::IsOnRoot()
{
	if(!m_strCurPath.String() || !m_strCurRoot.String())
	{
		return TRUE;
	}
	else
	{
		return !wcscmp(m_strCurPath.String(),m_strCurRoot.String());
	}

}

void CPanelMediaLibrary::SetCurPath(const wchar_t* lpszPath)
{
	if(lpszPath)
	{
		static MEDIA_CATEGORY last_category=MEDIA_NULL;

		if(m_mediaCategory!=last_category || !m_strCurPath.String() || (m_strCurPath.String()&&(lstrcmpi(m_strCurPath.String(), lpszPath) != 0)) )
		{
			last_category = m_mediaCategory;

			RETAILMSG(DEBUG_MEDIA,(_T(" ##### CPanelMediaLibrary::SetCurPath:%s\r\n"),lpszPath));

			m_strCurPath=lpszPath;
			//m_bPathChanged=TRUE;
			//搜索文件
			ExplorePath(lpszPath);
		}
		else
		{
			//the same path:
			//move to current:
			if(m_strCurPlayFile.String())
			{
				int idx=m_boxExplorer.SearchValue(m_strCurPlayFile.String());
				m_boxExplorer.MoveToIndex(idx==-1?0:idx);
				m_iExplorerPlayIndex = (idx==-1)?0:idx-m_iExplorerFolderCount;
			}
		}
	}
}
void CPanelMediaLibrary::ExplorePath(const wchar_t* lpszPath)
{
	TCHAR szFind[MAX_PATH];
	TCHAR szFile[1024];

	RETAILMSG(DEBUG_MEDIA,(_T("CPanelMediaLibrary::ExplorePath:%s\r\n"),lpszPath));

	//MultiByteToWideChar (CP_ACP, 0, lpszPath, -1, szFind, MAX_PATH);
	wcscpy_s(szFind,MAX_PATH,lpszPath); //
	wcscat_s(szFind,MAX_PATH,_T("*.*")); //
	WIN32_FIND_DATA wfd;
	HANDLE hFind=FindFirstFile(szFind,& wfd);

	TCHAR szwFile[MAX_PATH];

	CListBoxItemGL* pItem;
	BOOL bRoot=FALSE;

	//clear list
	m_boxExplorer.ClearList();

	m_iExplorerFilesCount=0;
	m_iExplorerFolderCount=0;
	m_iExplorerPlayIndex=0;

	bRoot=IsOnRoot();

// 	if(!wcscmp(lpszPath,L"\\"))//root
	if(!bRoot)
	{
		//add parent:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_MEDIA_BROWSE_PARENT_FOLDER),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconFolder,&m_iconFolderFocus,14,0);
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_MEDIA_BROWSE_PARENT_FOLDER));
		pItem->SetIcon(&m_iconFolder);
#endif
		m_boxExplorer.AddItem(pItem,MEDIA_FILE_TYPE_FOLDER);
		m_iExplorerFolderCount++;
	}

	if(hFind==INVALID_HANDLE_VALUE) //
	{
		return;
	}

	//遍历设备里的文件
	do
	{
		RETAILMSG(DEBUG_MEDIA,(_T("CPanelMediaLibrary::find:%s,dwFileAttributes:0x%x\r\n"),wfd.cFileName,wfd.dwFileAttributes));

		if(wfd.cFileName[0]=='.')
			continue;

		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			swprintf_s(szwFile,MAX_PATH,_T("%s"),wfd.cFileName);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			pItem=new CListBoxItemGL(szwFile,NULL,NULL,30,0);
			pItem->SetIcon(&m_iconFolder,&m_iconFolderFocus,14,0);
#else
			pItem=new CListBoxItemGL(szwFile);
			pItem->SetIcon(&m_iconFolder);
#endif
			//添加到list列表里
			m_boxExplorer.AddItemByOrder(pItem,MEDIA_FILE_TYPE_FOLDER);

			m_iExplorerFolderCount++;
		}
		else if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			//cut max path:
			swprintf_s(szFile,1024,_T("%s%s"),lpszPath,wfd.cFileName);
			if(wcslen(szFile)>MAX_PATH)
			{
				RETAILMSG(DEBUG_MEDIA,(_T("ExploreMeidaFiles:filename too long, pass\r\n")));
				continue;
			}
			if(m_mediaCategory == MEDIA_ALL)
			{
				if(IsVideoFile(wfd.cFileName))
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
					pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
					pItem->ShowValue(FALSE);
					pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus,14,0);
#else
					pItem=new CListBoxItemGL(wfd.cFileName,szFile);
					pItem->ShowValue(FALSE);
					pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus);
#endif
					m_boxExplorer.AddItemByOrder(pItem,MEDIA_FILE_TYPE_VIDEO,TRUE);

					m_iExplorerFilesCount++;
				}
				else if(IsAudioFile(wfd.cFileName))
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
					pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
					pItem->ShowValue(FALSE);
					pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus,14,0);
#else
					pItem=new CListBoxItemGL(wfd.cFileName,szFile);
					pItem->ShowValue(FALSE);
					pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus);
#endif
					m_boxExplorer.AddItemByOrder(pItem,MEDIA_FILE_TYPE_AUDIO,TRUE);

					m_iExplorerFilesCount++;
				}
			}
			else if(m_mediaCategory == MEDIA_VIDEO)
			{
				if(IsVideoFile(wfd.cFileName))
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
					pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
					pItem->ShowValue(FALSE);
					pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus,14,0);

#else
					pItem=new CListBoxItemGL(wfd.cFileName,szFile);
					pItem->ShowValue(FALSE);
					pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus);
#endif
					m_boxExplorer.AddItemByOrder(pItem,MEDIA_FILE_TYPE_VIDEO,TRUE);

					m_iExplorerFilesCount++;
				}
			}
			else if(m_mediaCategory == MEDIA_AUDIO)
			{
				if(IsAudioFile(wfd.cFileName))
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
					pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
					pItem->ShowValue(FALSE);
					pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus,14,0);
#else
					pItem=new CListBoxItemGL(wfd.cFileName,szFile);
					pItem->ShowValue(FALSE);
					pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus);
#endif
					//添加到m_boxExplorer文件夹列表里
					m_boxExplorer.AddItemByOrder(pItem,MEDIA_FILE_TYPE_AUDIO,TRUE);

					m_iExplorerFilesCount++;
				}
			}
		}
		Sleep(0);
	}
	while(FindNextFile(hFind,&wfd));
	FindClose(hFind); //

	//move to current:
	if(m_strCurPlayFile.String())
	{
		int idx=m_boxExplorer.SearchValue(m_strCurPlayFile.String());
		m_boxExplorer.MoveToIndex(idx==-1?0:idx);
		m_iExplorerPlayIndex = (idx==-1)?0:idx-m_iExplorerFolderCount;
	}

}


BOOL CPanelMediaLibrary::IsOnQuickSearchMode()
{
	CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;

	return p->IsOnQuickSearchMode();
}
void CPanelMediaLibrary::SetQuickSearchMode(BOOL bSearchMode)
{
	CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;

	p->ShowQuickSearchMode(bSearchMode);
}

const wchar_t* CPanelMediaLibrary::GetNextValue()
{
	CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;

	INT track_count=GetTrackCount();

	if(track_count<=0)
	{
		RETAILMSG(DEBUG_MEDIA,(_T("*****CPanelMediaLibrary GetNextValue no item....\r\n")));
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			//current folder has no file:
			//get from library:
			track_count=m_boxLibrary.GetCount();
			if(track_count>0)
			{
				SetShowType(MEDIA_LIBRARY_TRACKS);
				m_boxLibrary.MoveToNext(TRUE,FALSE);
				return m_boxLibrary.GetCurFocusItemValue();
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		//如果是文件夹模式
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			m_iExplorerPlayIndex++;
			if(m_iExplorerPlayIndex>=track_count)
				m_iExplorerPlayIndex=0;

			if(p->MoveToIndex(m_iExplorerFolderCount+m_iExplorerPlayIndex))
			{
				return p->GetCurFocusItemValue();
			}
			else
			{
				return NULL;
			}
		}
		//如果是列表模式
		else
		{
			p->MoveToNext(TRUE,FALSE);
			return p->GetCurFocusItemValue();
		}
	}
}
const wchar_t* CPanelMediaLibrary::GetPreviousValue()
{
	CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;

	INT track_count=GetTrackCount();

	if(track_count<=0)
	{
		RETAILMSG(DEBUG_MEDIA,(_T("*****CPanelMediaLibrary GetNextValue no item....\r\n")));
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			//current folder has no file:
			//get from library:
			track_count=m_boxLibrary.GetCount();
			if(track_count>0)
			{
				SetShowType(MEDIA_LIBRARY_TRACKS);
				m_boxLibrary.MoveToLast(TRUE,FALSE);
				return m_boxLibrary.GetCurFocusItemValue();
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			m_iExplorerPlayIndex--;
			if(m_iExplorerPlayIndex<0)
				m_iExplorerPlayIndex=track_count-1;

			if(p->MoveToIndex(m_iExplorerFolderCount+m_iExplorerPlayIndex))
			{
				return p->GetCurFocusItemValue();
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			p->MoveToLast(TRUE,FALSE);
			return p->GetCurFocusItemValue();
		}
	}
}
void CPanelMediaLibrary::MoveToNext()
{
	CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;
	if(p->IsOnQuickSearchMode())
	{
		p->ShowQuickSearchMode(FALSE);
	}
	else
	{
		p->MoveToNext(TRUE,FALSE);
	}
}
void CPanelMediaLibrary::MoveToPrevious()
{
	CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;
	if(p->IsOnQuickSearchMode())
	{
		p->ShowQuickSearchMode(FALSE);
	}
	else
	{
		p->MoveToLast(TRUE,FALSE);
	}
}

const wchar_t* CPanelMediaLibrary::GetRandomValue()
{
	CListBoxGL *p=(m_type==MEDIA_LIBRARY_EXPLORER)?&m_boxExplorer:&m_boxLibrary;
	int count=GetTrackCount();//m_boxLibrary.GetCount();

	if(count>0)
	{
		srand(GetTickCount());
		int index=rand()%count;

		RETAILMSG(DEBUG_MEDIA,(_T("GetRandomValue:%d.........\r\n"),index));

		if(m_type == MEDIA_LIBRARY_EXPLORER)
		{
			if(p->MoveToIndex(m_iExplorerFolderCount+index))
			{
				m_iExplorerPlayIndex=index;
				return p->GetCurFocusItemValue();
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			p->MoveToIndex(index,0,FALSE);
			return p->GetCurFocusItemValue();
		}
	}
	else
	{
		RETAILMSG(DEBUG_MEDIA,(_T("*****CPanelMediaLibrary GetRandomValue no item....\r\n")));

		if(m_type == MEDIA_LIBRARY_EXPLORER)
		{
			//current folder has no file:
			//get from library:
			count=m_boxLibrary.GetCount();
			if(count>0)
			{
				srand(GetTickCount());
				int index=rand()%count;

				SetShowType(MEDIA_LIBRARY_TRACKS);
				m_boxLibrary.MoveToIndex(index,0,FALSE);
				return m_boxLibrary.GetCurFocusItemValue();
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
}
void CPanelMediaLibrary::SetShowType(MEDIA_LIBRARY_TYPE type)
{
	RETAILMSG(DEBUG_MEDIA,(_T("CPanelMediaLibrary SetShowType id:%d\r\n"),type));

	BOOL bMassStorage=IsMassStorageMediaType(m_idCurMediaType);

	RETAILMSG(DEBUG_MEDIA,(_T("CPanelMediaLibrary SetShowType bMassStorage:%d\r\n"),bMassStorage));

	m_type=type;
	m_dwSetShowTypeTick = GetTickCount();

	//文件夹模式，列出设备里的文件夹,只有在USB等大容量存储设备的时候才显示，蓝牙播放音乐不显示
	m_boxLibrary.Show(type==MEDIA_LIBRARY_TRACKS && bMassStorage);
	//列表模式，列出所有有效的文件类型
	m_boxExplorer.Show(type==MEDIA_LIBRARY_EXPLORER && bMassStorage);
	//搜索模式
	m_boxSearched.Show(type==MEDIA_LIBRARY_SEARCH_RESULT && bMassStorage);

	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
#if LIB_BROWSE_BTN_DISPLAY_REVERSE
		m_btnBrowseMode.SetIcon(&m_iconExplorer);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
#else
		m_btnBrowseMode.SetIcon(m_mediaCategory == MEDIA_AUDIO?&m_iconCatalogAudio:&m_iconCatalogVideo);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
#endif

		//移动到播放文件
		if(m_strCurPlayFile.String())
		{
			int idx=m_boxExplorer.SearchValue(m_strCurPlayFile.String());
			m_boxExplorer.MoveToIndex(idx==-1?0:idx);
			m_iExplorerPlayIndex = (idx==-1)?0:idx-m_iExplorerFolderCount;
		}
	}
	else if(m_type==MEDIA_LIBRARY_SEARCH_RESULT)
	{
		m_btnBrowseMode.SetIcon(m_mediaCategory == MEDIA_AUDIO?&m_iconCatalogAudio:&m_iconCatalogVideo);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
	else
	{
#if LIB_BROWSE_BTN_DISPLAY_REVERSE
		m_btnBrowseMode.SetIcon(m_mediaCategory == MEDIA_AUDIO?&m_iconCatalogAudio:&m_iconCatalogVideo);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
#else
		m_btnBrowseMode.SetIcon(&m_iconExplorer);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
#endif
		//
		if(m_strCurPlayFile.String())
		{
			int idx=m_boxLibrary.SearchValue(m_strCurPlayFile.String());
			m_boxLibrary.MoveToIndex(idx==-1?0:idx);
		}
	}

	//只有在USB等大容量存储设备的时候才显示，蓝牙播放音乐等其他状态不显示
	m_btnCatalogAudio.Enable(bMassStorage);
	m_btnCatalogVideo.Enable(bMassStorage && m_bEnableVideo);
	m_btnCatalogImage.Enable(bMassStorage);
// 	m_btnCatalogArtist.Enable(bMassStorage);
	m_btnSearch.Enable(bMassStorage);
	m_btnBrowseMode.Enable(bMassStorage);

// 	m_btnExplorer.SetStatus(type==MEDIA_LIBRARY_EXPLORER?BS_FOCUS:BS_UP);
// 	m_btnTracks.SetStatus(type==MEDIA_LIBRARY_TRACKS?BS_FOCUS:BS_UP);

	if(type==MEDIA_LIBRARY_EXPLORER && bMassStorage)
	{
		if(!m_strCurPath.String())
		{
			if(m_strCurRoot.String())
			{
				SetCurPath(m_strCurRoot.String());
			}
		}
	}
}
void CPanelMediaLibrary::AbortRefresh()
{
	if(m_bRefreshingLib)
	{
		 		RETAILMSG(DEBUG_MEDIA,(_T(".....Still refreshing! cancel..........\r\n")));

		if(m_hExploreThread != INVALID_HANDLE_VALUE)
		{
			RETAILMSG(DEBUG_MEDIA,(_T("++++Already refreshing..., abort--\r\n")));

			m_bCancelRefresh=TRUE;

			RETAILMSG(DEBUG_MEDIA,(_T("++++Already Sending..., wait...--\r\n")));
			DWORD dwExitCode;
			do
			{
				Sleep(10);
				if (!GetExitCodeThread(m_hExploreThread, &dwExitCode))
				{
					break;
				}
			} while (dwExitCode == STILL_ACTIVE);

			if(m_hExploreThread!=INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_hExploreThread);
				m_hExploreThread = INVALID_HANDLE_VALUE;
			}

		}
	}
}

//更新播放列表
void CPanelMediaLibrary::RefreshLibrary()
{
	RETAILMSG(DEBUG_MEDIA,(_T("MSG:CPanelMediaLibrary::RefreshLibrary ++\r\n")));

	if(m_bRefreshingLib)
	{
		AbortRefresh();
	}
	else
	{
		if(m_hExploreThread!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hExploreThread);
			m_hExploreThread = INVALID_HANDLE_VALUE;
		}
	}

	//
	if(m_hwndNotify)
	{
		::SendMessage(m_hwndNotify,WM_MEDIAPLAYER_MSG_EX,MEDIA_MSG_SCAN_STATUS,MEDIA_SCAN_BEGIN);
	}

	RETAILMSG(DEBUG_MEDIA,(_T(".......begin to RefreshLibrary..........\r\n")));

	m_bRefreshingLib = TRUE;

	m_bCancelRefresh=FALSE;

	ResetEvent(m_hRefreshEvent);

	m_boxLibrary.ClearList();
	m_boxExplorer.ClearList();
	
	//Factory mode
#if 1
	CListBoxItemGL *pItem;

   RETAILMSG(1, (TEXT("m_iFactoryTestType :%d \r\n"),m_iFactoryTestType));

    if(m_pSkinManager->GetConfig()->GetCurParam()->bActiveFactory/* && IsFileExists(CVTE_DEF_MEDIA_TEST_VIDEO)*/)
    {
        RETAILMSG(1, (TEXT("Add test.mp4 \r\n")));
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(_T("Test.mp4"),CVTE_DEF_MEDIA_TEST_VIDEO,NULL,22,0);
		pItem->ShowValue(FALSE);
		pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus,22,0);
#else
    	pItem=new CListBoxItemGL(_T("Test.mp4"),CVTE_DEF_MEDIA_TEST_VIDEO);
    	pItem->ShowValue(FALSE);
    	pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus);
#endif
    	m_boxLibrary.AddItemByOrder(pItem,MEDIA_FILE_TYPE_VIDEO);
    }
    else
    {
        RETAILMSG(1, (TEXT("no activate factorytest. \r\n")));
    }
#endif

    //begin thread to explore for media files:
	m_hExploreThread=CreateThread(NULL,NULL,ExploreThreadPro,this,NULL,NULL);

	RETAILMSG(DEBUG_MEDIA,(_T("MSG:CPanelMediaLibrary::RefreshLibrary --\r\n")));

}
void CPanelMediaLibrary::EnableVideo(BOOL bEnable)
{
	m_bEnableVideo = bEnable;
	//m_btnCatalogVideo.Enable(bEnable);
}
void CPanelMediaLibrary::RemoveInvalidFile(const wchar_t *filename)
{
	//test:
	//m_boxLibrary.DeleteCurFocusItem();
}
void CPanelMediaLibrary::SetCurPlayFile(const wchar_t *filename)
{
	if(filename)
	{
		m_strCurPlayFile=filename;

		//change explorer path:
		INT iLen=wcslen(filename);

		if(iLen>0)
		{
			wchar_t path[MAX_PATH];
			wcscpy_s(path,MAX_PATH,filename);

			for(int i=iLen-1;i>=0;i--)
			{
				if(*(path+i)=='\\')
				{
					*(path+i+1)=0;
					SetCurPath(path);
					break;
				}
			}
		}

		//change library focus:
		//
		if(m_strCurPlayFile.String())
		{
			int idx=m_boxLibrary.SearchValue(m_strCurPlayFile.String());
			m_boxLibrary.MoveToIndex(idx==-1?0:idx);
		}
	}
}

void CPanelMediaLibrary::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
    RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: wParam:%d lParam:%d m_idCurMediaType:%d\r\n"),TEXT(__FUNCTION__),wParam,lParam,m_idCurMediaType));

	if(wParam>MEDIA_TYPE_NUMBER-1)
		return;

	if(m_idValidSrcList[wParam] == lParam)
		return;

	m_idValidSrcList[wParam] = lParam;


	//update widget:
	if(wParam == MEDIA_TYPE_A2DP)
	{
		if(m_pSkinManager)
		{
			BOOL bshow=lParam;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_A2DP,0,UIOPRT_SHOWHIDE,&bshow);
		}
	}
	//Here support two usb device
	else if(wParam == MEDIA_TYPE_USB1)
	{
		if(m_pSkinManager)
		{
			BOOL bshow;
			m_bUsb1Available = lParam;
			bshow = (m_bUsb1Available || m_bUsb2Available);
			//RETAILMSG(1, (TEXT("MSG:[%s]: Usb1 Insert Icon Show[%d]\r\n"), TEXT(__FUNCTION__), bshow));
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_USB,0,UIOPRT_SHOWHIDE,&bshow);
		}
	}
	else if(wParam == MEDIA_TYPE_USB2)
	{
		if(m_pSkinManager)
		{
			BOOL bshow;
			m_bUsb2Available = lParam;
			bshow = (m_bUsb1Available || m_bUsb2Available);
			//RETAILMSG(1, (TEXT("MSG:[%s]: Usb2 Insert Icon Show[%d]\r\n"), TEXT(__FUNCTION__), bshow));
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_USB,0,UIOPRT_SHOWHIDE,&bshow);
		}
	}
	else if(wParam == MEDIA_TYPE_SD1)
	{
		if(m_pSkinManager)
		{
			BOOL bshow=lParam;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_SD,0,UIOPRT_SHOWHIDE,&bshow);
		}
	}
#ifdef USE_GPS_CARD_MEDIA_PLAY
	else if(wParam == MEDIA_TYPE_SD2)
	{
		if(m_pSkinManager)
		{
			BOOL bshow=lParam;
			m_pSkinManager->GetGUI()->OperateWidget(IDC_WIDGET_SD,0,UIOPRT_SHOWHIDE,&bshow);
		}
	}
#endif
	//refresh device list:
	m_boxDevice.ClearList();

	CListBoxItemGL *pItem;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)	
	int iconMarginRight = -14;
#endif
	if(m_idValidSrcList[MEDIA_TYPE_SD1])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD),NULL,NULL,24,0);
		pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus,14,0);

		if(m_idCurMediaType == MEDIA_TYPE_SD1)
		{
			pItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight ,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight ,0);
		}

#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD));
		pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_SD1);
	}
#ifdef USE_GPS_CARD_MEDIA_PLAY
	if(m_idValidSrcList[MEDIA_TYPE_SD2])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus,14,0);
		//if (m_idCurMediaType == MEDIA_TYPE_SD1)
		//{
		//	pItem->SetIcon2(&m_iconArrow,NULL,9,0);
		//}
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD));
		pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_SD2);
	}
#endif
	if(m_idValidSrcList[MEDIA_TYPE_USB1])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB),NULL,NULL,24,0);
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus,14,0);
		if(m_idCurMediaType == MEDIA_TYPE_USB1)
		{
			pItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight ,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight ,0);
		}

#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB));
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_USB1);
	}
	if(m_idValidSrcList[MEDIA_TYPE_USB2])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB),NULL,NULL,24,0);
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus,14,0);
		if(m_idCurMediaType == MEDIA_TYPE_USB2)
		{
			pItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight ,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight ,0);
		}
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB));
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_USB2);
	}
	if(m_idValidSrcList[MEDIA_TYPE_A2DP])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_BT),NULL,NULL,24,0);
		pItem->SetIcon(&m_iconDevBT,&m_iconDevBTFocus,14,0);
		if(m_idCurMediaType == MEDIA_TYPE_A2DP)
		{
			pItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight ,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight ,0);
		}
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_BT));
		pItem->SetIcon(&m_iconDevBT,&m_iconDevBTFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_A2DP);
	}
	if(m_idValidSrcList[MEDIA_TYPE_IPOD])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_IPOD),NULL,NULL,24,0);
		pItem->SetIcon(&m_iconDeviPod,&m_iconDeviPodFocus,14,0);
		if(m_idCurMediaType == MEDIA_TYPE_IPOD)
		{
			pItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight ,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight ,0);
		}
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_IPOD));
		pItem->SetIcon(&m_iconDeviPod,&m_iconDeviPodFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_IPOD);
	}

	//add blank item:
	if(m_boxDevice.GetCount()<4)
	{
		int iblank=4-m_boxDevice.GetCount();

		for (int i=0;i<iblank;i++)
		{
			pItem=new CListBoxItemGL(_T(" "));
			pItem->EnableClick(FALSE);
			m_boxDevice.AddItem(pItem,0);
		}
	}


	//
	if(!m_boxDevice.MoveToIndex(m_boxDevice.SearchID(m_idCurMediaType)))
	{
		RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s] m_boxDevice.MoveToID :%d , no device \r\n"),TEXT(__FUNCTION__),m_idCurMediaType));
		//select index 0:
		UINT idClass = GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurSRC);
		if(/*idClass==UI_CLASS_MEDIA || */idClass==UI_CLASS_BT)
		{
// 			if(m_boxDevice.GetCount()>0)
// 				SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,m_boxDevice.GetCurFocusItemID());
			//SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_NONE);
		}
	}

}

BOOL CPanelMediaLibrary::ExploreMeidaFiles(LPCTSTR lpszPath)
{
	if(lpszPath)
	{
		TCHAR szFind[MAX_PATH];
		wcscpy_s(szFind,MAX_PATH,lpszPath); //
		wcscat_s(szFind,MAX_PATH,_T("*.*")); //
		WIN32_FIND_DATA wfd;
		HANDLE hFind=FindFirstFile(szFind,& wfd);

		CListBoxItemGL* pItem;

		TCHAR szFile[1024];

		if(hFind==INVALID_HANDLE_VALUE) //
			return FALSE;
		do
		{
			if(wfd.cFileName[0]=='.')
				continue; //
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				swprintf_s(szFile,MAX_PATH,_T("%s%s\\"),lpszPath,wfd.cFileName);
				ExploreMeidaFiles(szFile); //
			}
			else if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
 				RETAILMSG(DEBUG_MEDIA,(_T("ExploreMeidaFiles:%s%s\r\n"),lpszPath,wfd.cFileName));

				//cut max path:
				swprintf_s(szFile,1024,_T("%s%s"),lpszPath,wfd.cFileName);
				if(wcslen(szFile)>MAX_PATH)
				{
					RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: filename too long, pass\r\n"),TEXT(__FUNCTION__)));
					continue;
				}


				if(m_mediaCategory == MEDIA_ALL)
				{
					if(IsVideoFile(wfd.cFileName))
					{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
						pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
						pItem->ShowValue(FALSE);
						pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus,14,0);
#else
						pItem=new CListBoxItemGL(wfd.cFileName,szFile);
						pItem->ShowValue(FALSE);
						pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus);
#endif
						m_boxLibrary.AddItemByOrder(pItem,MEDIA_FILE_TYPE_VIDEO);
					}
					else if(IsAudioFile(wfd.cFileName))
					{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
						pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
						pItem->ShowValue(FALSE);
						pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus,14,0);
#else
						pItem=new CListBoxItemGL(wfd.cFileName,szFile);
						pItem->ShowValue(FALSE);
						pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus);
#endif
						m_boxLibrary.AddItemByOrder(pItem,MEDIA_FILE_TYPE_AUDIO);
					}
				}
				else if(m_mediaCategory == MEDIA_VIDEO)
				{
					if(IsVideoFile(wfd.cFileName))
					{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
						pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
						pItem->ShowValue(FALSE);
						pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus,14,0);
#else
						pItem=new CListBoxItemGL(wfd.cFileName,szFile);
						pItem->ShowValue(FALSE);
						pItem->SetIcon(&m_iconVideo,&m_iconVideoFocus);
#endif
						m_boxLibrary.AddItemByOrder(pItem,MEDIA_FILE_TYPE_VIDEO);
						//Sleep(200);
					}
				}
				else if(m_mediaCategory == MEDIA_AUDIO)
				{
					if(IsAudioFile(wfd.cFileName))
					{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
						pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
						pItem->ShowValue(FALSE);
						pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus,14,0);
#else
						pItem=new CListBoxItemGL(wfd.cFileName,szFile);
						pItem->ShowValue(FALSE);
						pItem->SetIcon(&m_iconAudio,&m_iconAudioFocus);
#endif
						m_boxLibrary.AddItemByOrder(pItem,MEDIA_FILE_TYPE_AUDIO);
						//Sleep(200);
					}
				}

			}
			m_iTotalFileScanned++;
			Sleep(0);
		}
		while(FindNextFile(hFind,&wfd) && !m_bCancelRefresh);
		FindClose(hFind);
	}

	return TRUE;
}
DWORD CPanelMediaLibrary::WaitForRefreshDone(DWORD dwWait)
{
	if(m_bRefreshingLib)
		return WaitForSingleObject(m_hRefreshEvent,dwWait);
	else
		return WAIT_OBJECT_0;
}
void CPanelMediaLibrary::SetMediaSrcType(UINT idType)
{
	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: idType:%d\r\n"),TEXT(__FUNCTION__),idType));

	//auto switch:
	BOOL bValid=FALSE;
	UINT idSelect=idType;
	if(idType<MEDIA_TYPE_NUMBER)
		bValid = m_idValidSrcList[idType];


	if(idType==MEDIA_TYPE_USB1 && !bValid)
	{
		idSelect = MEDIA_TYPE_USB2;
	}
	//确定播放设备列表的播放设备
	else if(idType==MEDIA_TYPE_USB2 && !bValid)
	{
		idSelect = MEDIA_TYPE_USB1;
	}
	else if(idType==MEDIA_TYPE_SD1 && !bValid)
	{
		idSelect = MEDIA_TYPE_SD2;
	}
	else if(idType==MEDIA_TYPE_SD2 && !bValid)
	{
		idSelect = MEDIA_TYPE_SD1;
	}
	else if(idType ==  MEDIA_TYPE_FLASH)
	{
		//Just for factory mode media test
		idSelect = MEDIA_TYPE_USB1;
		idType = MEDIA_TYPE_USB1;
		m_idValidSrcList[idSelect] = IsFileExists(CVTE_DEF_MEDIA_TEST_VIDEO);
		bValid = m_idValidSrcList[idSelect];
	}

	if(!m_idValidSrcList[idSelect])
	{
		idSelect = MEDIA_TYPE_INVALID;
	}

	if(idSelect != MEDIA_TYPE_INVALID)
	{
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("==SelectMediaType: %d, valid=%d, actual select=%d\r\n"),idType,bValid,idSelect));
	}
	else
	{
		RETAILMSG(1/*DEBUG_MEDIA*/,(_T("==SelectMediaType: %d, valid=%d, no valid src selected\r\n"),idType,bValid));
		return;
	}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	int iconMarginRight = -14;
	{
		CListBoxItemGL* pitem = m_boxDevice.m_listItems.GetItemByID(m_idCurMediaType);
		if (pitem)
		{
			pitem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
		}
	}
#endif

	m_idCurMediaType=idSelect;

	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: idType:%d,category:%d\r\n"),TEXT(__FUNCTION__),idType,m_mediaCategory));

// 	SetShowType(MEDIA_LIBRARY_TRACKS);

	//move to current device:
	//设备列表移动到当前播放设备
	m_boxDevice.MoveToIndex(m_boxDevice.SearchID(m_idCurMediaType));

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	{
		CListBoxItemGL* pitem = m_boxDevice.GetCurFocusItem();
		if (pitem)
		{
			pitem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
		}
	}
#endif

	RETAILMSG(1, (TEXT("MSG:[%s]: IsMassStorageMediaType(%d) = %d\r\n"),TEXT(__FUNCTION__),idType,IsMassStorageMediaType(idType)));
	//查询是否是大容量存储设备
	if(IsMassStorageMediaType(idType))
	{
		EnableOperate(FALSE);

		//reset playfile
		m_strCurPlayFile=NULL;

		//刷新播放列表
		RefreshLibrary();

		if( m_idCurMediaType == MEDIA_TYPE_USB1 ) // mass storages
		{
			WCHAR str[MAX_PATH];
			swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_USB1);
			m_strCurRoot=str;
		}
		else if( m_idCurMediaType == MEDIA_TYPE_USB2 ) // mass storages
		{
			WCHAR str[MAX_PATH];
			swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_USB2);
			m_strCurRoot=str;
		}
		else if( m_idCurMediaType == MEDIA_TYPE_SD1 ) // mass storages
		{
			WCHAR str[MAX_PATH];
			swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_MEDIACARD);
			m_strCurRoot=str;
		}
#ifdef USE_GPS_CARD_MEDIA_PLAY
		else if( m_idCurMediaType == MEDIA_TYPE_SD2 ) // mass storages
		{
			WCHAR str[MAX_PATH];
			swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_GPSCARD);
			m_strCurRoot=str;
		}
#endif
        else
		{
			m_strCurRoot = NULL;
		}

		//update explorer path:
		m_strCurPath = NULL;
		if(m_strCurRoot.String())
		{
			SetCurPath(m_strCurRoot.String());
		}

		//
		m_tagNote.SetCaption(GetResourceString(IDS_NO_MEDIA_FILES));

	}
	else
	{
		EnableOperate(TRUE);


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		m_tagNote.SetCaption(_T(""));
#else
		m_tagNote.SetCaption(GetResourceString(IDS_NO_LISTITEM));
#endif

	}

	//设置显示类型，列表模式，文件夹模式，搜索模式
	SetShowType(MEDIA_LIBRARY_TYPE(m_pSkinManager->GetConfig()->GetCurParam()->idMediaLibraryType));

}
BOOL CPanelMediaLibrary::IsMassStorageMediaType(UINT idType)
{
	return ( idType == MEDIA_TYPE_USB1) || ( idType == MEDIA_TYPE_USB2) || ( idType == MEDIA_TYPE_SD1) || ( idType == MEDIA_TYPE_SD2);
}
// const wchar_t* CPanelMediaLibrary::GetDeviceFriendlyName(LPCTSTR lpDeviceName)
// {
// 	if (lstrcmpi(lpDeviceName, DEV_NAME_NAND) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_RESIDENTFLASH);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_BT) == 0)//A2DP
// 	{
// 		return GetResourceString(IDS_DEVICE_BT);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_MEDIACARD) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_SD);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_GPSCARD) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_SD);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_USB1) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_USB);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_USB2) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_USB);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_IPOD) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_IPOD);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_LINEIN_FRONT) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_LINEIN_FRONT);
// 	}
// 	else if (lstrcmpi(lpDeviceName, DEV_NAME_LINEIN_REAR) == 0)
// 	{
// 		return GetResourceString(IDS_DEVICE_LINEIN_REAR);
// 	}
// 	else
// 	{
// 		return NULL;
// 	}
// }
//
//保存文件的绝对路径
DWORD CPanelMediaLibrary::ExploreThreadPro(LPVOID lpParam)
{
	CPanelMediaLibrary *panel=(CPanelMediaLibrary*)lpParam;

	Sleep(100);

	DWORD dwTick=GetTickCount();
 	//RETAILMSG(DEBUG_MEDIA,(_T("......Begin ExploreMeidaFiles:%s\r\n"),panel->m_strDeviceName.String()));
	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: ThreadID[%x] ++ mediatype[%d]\r\n"),TEXT(__FUNCTION__),GetCurrentThreadId(),panel->m_idCurMediaType));

 	//storage only

	panel->m_iTotalFileScanned=0;

	if( panel->m_idCurMediaType == MEDIA_TYPE_IPOD )
	{
		//refresh ipod list:
		RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: begin to refresh ipod list..........\r\n"),TEXT(__FUNCTION__)));

	}
	else if( panel->m_idCurMediaType == MEDIA_TYPE_A2DP )
	{
		//refresh bluetooth list:
		RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: begin to refresh bluetooth list..........\r\n"),TEXT(__FUNCTION__)));

	}
	else if( panel->m_idCurMediaType == MEDIA_TYPE_USB1 ) // mass storages
	{
		WCHAR str[MAX_PATH];
		swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_USB1);
		panel->ExploreMeidaFiles(str);
	}
	else if( panel->m_idCurMediaType == MEDIA_TYPE_USB2 ) // mass storages
	{
		WCHAR str[MAX_PATH];
		swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_USB2);
		panel->ExploreMeidaFiles(str);
	}
	else if( panel->m_idCurMediaType == MEDIA_TYPE_SD1 ) // mass storages
	{
		WCHAR str[MAX_PATH];
		swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_MEDIACARD);
		panel->ExploreMeidaFiles(str);
	}
	else if( panel->m_idCurMediaType == MEDIA_TYPE_SD2 ) // mass storages
	{
		WCHAR str[MAX_PATH];
		swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_GPSCARD);
		panel->ExploreMeidaFiles(str);
	}
#ifdef USE_RESIDENTFLASH_PLAY	
	else if( panel->m_idCurMediaType == MEDIA_TYPE_RESIDENTFLASH ) // mass storages
	{
		WCHAR str[MAX_PATH];
		swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_NAND);
		panel->ExploreMeidaFiles(str);
	}
#endif
	DWORD dwCost = GetTickCount()-dwTick;
	INT iCount = panel->m_boxLibrary.GetCount();

	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[%s]: ThreadID[%x] -- counts:%d take %d ms,total file scanned:%d\r\n"),TEXT(__FUNCTION__),GetCurrentThreadId(),iCount,dwCost,panel->m_iTotalFileScanned));

	if(iCount==0)
	{
		Sleep(1500);
	}
	else
	{
		Sleep(100);
	}

	//
	SetEvent(panel->m_hRefreshEvent);
	panel->m_bRefreshingLib=FALSE;

	if(panel->m_hwndNotify)
	{
		//post出WM_MEDIAPLAYER_MSG_EX开始播放
		::PostMessage(panel->m_hwndNotify,WM_MEDIAPLAYER_MSG_EX,MEDIA_MSG_SCAN_STATUS,
			panel->m_bCancelRefresh?MEDIA_SCAN_ABORT:MEDIA_SCAN_FINISH);

		//
		panel->m_dwRefreshTick = GetTickCount();
		panel->m_bAutoReturn = TRUE;
	}

	return 0;
}
BOOL CPanelMediaLibrary::IsAudioFile(LPCTSTR szFileName)
{
	WCHAR strExt[16]={0};
// 	INT iLen=wcslen(szFileName);

	if(!GetFileExtensionFromPath(szFileName,strExt,16))
		return FALSE;

	if(wcslen(strExt)<1)
		return FALSE;

	static LPCTSTR s_pszExts[] =
	{
#if ( CVTE_DEF_AUDIO_SUPPORT_FORMAT & CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WAV )
		_T("wav"),
#endif
#if ( CVTE_DEF_AUDIO_SUPPORT_FORMAT & CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MP3 )
		_T("mp3"),
#endif
#if ( CVTE_DEF_AUDIO_SUPPORT_FORMAT & CVTE_AUDIO_SUPPORT_FORMAT_TYPE_M4A )
	    _T("m4a"),
#endif
#if ( CVTE_DEF_AUDIO_SUPPORT_FORMAT & CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WMA )
		_T("wma"),
#endif
#if ( CVTE_DEF_AUDIO_SUPPORT_FORMAT & CVTE_AUDIO_SUPPORT_FORMAT_TYPE_APE )
		_T("ape"),
#endif
#if ( CVTE_DEF_AUDIO_SUPPORT_FORMAT & CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLA )
		_T("fla"),
#endif
#if ( CVTE_DEF_AUDIO_SUPPORT_FORMAT & CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLAC )
		_T("flac"),
#endif
#if ( CVTE_DEF_AUDIO_SUPPORT_FORMAT & CVTE_AUDIO_SUPPORT_FORMAT_TYPE_OGG )
		_T("ogg")
#endif
	};

	for (int i = 0; i < _countof(s_pszExts); i++)
	{
		if (lstrcmpi(s_pszExts[i], strExt) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL CPanelMediaLibrary::IsVideoFile(LPCTSTR szFileName)
{
	WCHAR strExt[16]={0};
// 	INT iLen=wcslen(szFileName);

	if(!GetFileExtensionFromPath(szFileName,strExt,16))
		return FALSE;

	if(wcslen(strExt)<1)
		return FALSE;

	static LPCTSTR s_pszExts[] =
	{
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_AVI)
		_T("avi"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MPG)
		_T("mpg"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MPEG)
		_T("mpeg"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_VOB)
		_T("vob"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_TS)
		_T("ts"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_TP)
		_T("tp"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_OGM)
		_T("ogm"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MKV)
		_T("mkv"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_RM)
		_T("rm"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_RMVB)
		_T("rmvb"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MP4)
		_T("mp4"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_M4V)
		_T("m4v"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MOV)
		_T("mov"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_3GP)
		_T("3gp"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_ASF)
		_T("asf"),
#endif
#if ( CVTE_DEF_VIDEO_SUPPORT_FORMAT & CVTE_VIDEO_SUPPORT_FORMAT_TYPE_WMV)
		_T("wmv"),
#endif
		/*_T("flv"),*/
	};

	for (int i = 0; i < _countof(s_pszExts); i++)
	{
		if (lstrcmpi(s_pszExts[i], strExt) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CALLBACK CPanelMediaLibrary::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaLibrary::OnNotify]: id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara,lpUserPara));

	CPanelMediaLibrary *panel=(CPanelMediaLibrary*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelMediaLibrary::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_MEDIA_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_NOWPLAYING:
		{
			if(IsMassStorageMediaType(m_idCurMediaType))
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_RIGHT);
			else if(m_idCurMediaType==MEDIA_TYPE_A2DP)
				SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_RIGHT);
			else
				SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_RIGHT);

			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_CATALOG_VIDEO:
		{
			if(m_type == MEDIA_LIBRARY_SEARCH_RESULT)
			{
				SetShowType(MEDIA_LIBRARY_TRACKS);
			}
			SendMediaCommand(MEDIA_CMD_CATEGORY,MEDIA_VIDEO);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_SLI_PROGRESS:
		{
			SendMediaCommand(MEDIA_CMD_SET_POSITION,wPara);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_CATALOG_AUDIO:
		{
			if(m_type == MEDIA_LIBRARY_SEARCH_RESULT)
			{
				SetShowType(MEDIA_LIBRARY_TRACKS);
			}
			SendMediaCommand(MEDIA_CMD_CATEGORY,MEDIA_AUDIO);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_CATALOG_IMAGE:
		{
			SwitchPage(UI_CLASS_IMAGE,PAGE_IMAGE_LIBRARY,PAGE_SWITCH_ANI_NONE);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_BROWSE_MODE:
		{
			SetShowType(m_type==MEDIA_LIBRARY_TRACKS?MEDIA_LIBRARY_EXPLORER:MEDIA_LIBRARY_TRACKS);
			m_pSkinManager->GetConfig()->GetCurParam()->idMediaLibraryType = m_type;
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BTN_SEARCH:
		{
#if (CVTE_EN_SEARCH_USE_FULL_KEYBOARD)
			ShowKeyboard(IDC_PANEL_KEYBOARD+IDC_MEDIA_BTN_SEARCH,&CPanelMediaLibrary::OnNotify,this,INPUT_LETTERS_CAP,
				L"",L"",20);
#else
			if(m_type == MEDIA_LIBRARY_TRACKS)
			{
				m_boxLibrary.ShowQuickSearchMode(TRUE);
			}
			else if(m_type == MEDIA_LIBRARY_EXPLORER)
			{
				m_boxExplorer.ShowQuickSearchMode(TRUE);
			}
#endif
			PlaySoundEffect(0);
		}
		break;
	case IDC_PANEL_KEYBOARD+IDC_MEDIA_BTN_SEARCH:
		{
			if(lpPara)
			{
				RETAILMSG(1,(_T("search media file :[%s]\r\n"),(const wchar_t*)lpPara));
				if(wcslen((const wchar_t*)lpPara))
				{
					CreateSearchResultList((const wchar_t*)lpPara,&m_boxLibrary,&m_boxSearched);
					SetShowType(MEDIA_LIBRARY_SEARCH_RESULT);
				}
			}
		}
		break;
	case IDC_MEDIA_BOX_DEVICE:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
			case NOTIFY_DOUBLE_CLICK:
				{
					if(m_type == MEDIA_LIBRARY_SEARCH_RESULT)
					{
						SetShowType(MEDIA_LIBRARY_TRACKS);
					}
					if(m_idCurMediaType != id_item)
					{
						//select device
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
						ArrowChange();
#endif
						SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,id_item);
						PlaySoundEffect(0);
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case IDC_MEDIA_BAR_PLAYING_FLAG:
		{
			SendMediaCommand(MEDIA_CMD_PLAY_PAUSE_SWITCH,0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_MEDIA_BOX_LIBRARY:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			m_bAutoReturn = TRUE;

			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
				{
					if(m_mediaCategory == MEDIA_VIDEO)
					{
						SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_RIGHT);
					}

					//begin to play
					//SendMediaCommand(MEDIA_CMD_PLAY_CONTROL,MEDIA_CONTROL_STOP);
					SendMediaCommand(MEDIA_CMD_PLAY_INDEX,index);

					//test:
					m_bNeedUpdateSlider = TRUE;

					PlaySoundEffect(0);
				}
				break;
			case NOTIFY_DOUBLE_CLICK:
				{
					SendMediaCommand(MEDIA_CMD_PLAY_PAUSE_SWITCH,0);

					PlaySoundEffect(0);
				}
				break;
			case NOTIFY_VALUE_CHANGE:
				{
					m_bNeedUpdateSlider = TRUE;
				}
				break;
			default:
				break;
			}
		}
		break;
	case IDC_MEDIA_BOX_SEARCH_RESULT:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;

			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
			case NOTIFY_DOUBLE_CLICK:
				{
					RETAILMSG(1,(_T("click search media file index :[%d]\r\n"),id_item));
					SendMediaCommand(MEDIA_CMD_PLAY_INDEX,id_item);
					SetShowType(MEDIA_LIBRARY_TRACKS);
				}
				break;
			default:
				break;
			}
		}
		break;
	case IDC_MEDIA_BOX_EXPLORER:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
				{
					if(id_item==MEDIA_FILE_TYPE_FOLDER)
					{
						if(!wcscmp(m_boxExplorer.GetCurFocusItemName(),GetResourceString(IDS_MEDIA_BROWSE_PARENT_FOLDER)))
							GotoParent();
						else
							EnterCurPath();

						PlaySoundEffect(0);
					}
					else
					{
						if(m_mediaCategory == MEDIA_VIDEO)
						{
							SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_RIGHT);
						}

						SendMediaCommand(MEDIA_CMD_PLAY_INDEX,index);
						MoveToTrackIndex(index-GetFolderCount());

						//test:
						m_bNeedUpdateSlider = TRUE;

						PlaySoundEffect(0);
					}

				}
				break;
			case NOTIFY_DOUBLE_CLICK:
				{
					if(id_item==MEDIA_FILE_TYPE_FOLDER)
					{
						if(!wcscmp(m_boxExplorer.GetCurFocusItemName(),GetResourceString(IDS_MEDIA_BROWSE_PARENT_FOLDER)))
							GotoParent();
						else
							EnterCurPath();

						PlaySoundEffect(0);
					}
					else
					{
						SendMediaCommand(MEDIA_CMD_PLAY_PAUSE_SWITCH,0);

						PlaySoundEffect(0);
					}
				}
				break;
			case NOTIFY_VALUE_CHANGE:
				{
					m_bNeedUpdateSlider = TRUE;
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

void CPanelMediaLibrary::SetLoadingStatus(FILE_LOADING_STATUS status)
{
	RETAILMSG(DEBUG_MEDIA,(_T("MSG:[CPanelMediaLibrary::SetLoadingStatus]: status:%d\r\n"),status));

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)	
	m_idLoadingStatus = status;
#endif

	m_boxLibrary.EnableNotify(status!=LOAD_STATUS_LOADING);
	m_boxExplorer.EnableNotify(status!=LOAD_STATUS_LOADING);
}

BOOL CPanelMediaLibrary::IsFileExists(LPCTSTR   lpszFileName)
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
/*
void CPanelMediaLibrary::OnFactoryCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType = DWORD(wParam);
	DWORD dwCmd = DWORD(lParam);

    switch(dwType)
    {
        case FACTORY_CMD_MEDIA_TEST_VIDEO:
        case FACTORY_CMD_MEDIA_TEST_AUDIO:
        case FACTORY_CMD_MEDIA_TEST_IMAGE:
        case FACTORY_CMD_MEDIA_TEST_IPOD:
        case FACTORY_CMD_MEDIA_TEST_A2DP:
        case FACTORY_CMD_MEDIA_TEST_ALL:
            m_iFactoryTestType = dwType;
            break;
        default:
            break;
    }

    m_iFactoryTestCmd = dwCmd;

    //RefreshLibrary();

    RETAILMSG(DEBUG_MEDIA, (TEXT("MSG:[CPanelMediaLibrary::OnFactoryCmd]: m_iFactoryTestType:%d m_iFactoryTestCmd:%d\r\n"),m_iFactoryTestType,m_iFactoryTestCmd));
}
*/
int CPanelMediaLibrary::CreateSearchResultList(const wchar_t *SearchString,CListBoxGL *plistboxIn,CListBoxGL *plistboxOut)
{
	wchar_t str_temp[MAX_PATH];
	int i = 0;

	if(!SearchString || !plistboxOut || !plistboxIn)
		return 0;

	DWORD dwTick=GetTickCount();

	//clear first:
	plistboxOut->ClearList();

	plistboxIn->m_listItems.Head();
	while (!plistboxIn->m_listItems.End())
	{
		CListBoxItemGL *pCursor=plistboxIn->m_listItems.Get();
		wcscpy_s(str_temp,MAX_PATH,pCursor->GetItemName());
		wchar_t *strTmp = str_temp;
		wchar_t *strTmpSearch = const_cast<wchar_t*>(SearchString);
		_wcsupr(strTmp);
		_wcsupr(strTmpSearch);
		if (wcsstr(strTmp,strTmpSearch)!=NULL)
		{
			CListBoxItemGL *pItem;
			pItem=new CListBoxItemGL(pCursor->GetItemName());
			pItem->SetUserParam(pCursor->GetUserParam());
			pItem->SetIcon(pCursor->GetIcon());
			plistboxOut->AddItem(pItem,pCursor->GetIndex());
			i++;
		}
		else if(pCursor->GetKeyWords())
		{
			if (wcsstr(pCursor->GetKeyWords(),strTmpSearch)!=NULL)
			{
				CListBoxItemGL *pItem;
				pItem=new CListBoxItemGL(pCursor->GetItemName());
				pItem->SetUserParam(pCursor->GetUserParam());
				pItem->SetIcon(pCursor->GetIcon());
				plistboxOut->AddItem(pItem,pCursor->GetIndex());
				i++;
			}
		}
		plistboxIn->m_listItems.Next();
	}

	RETAILMSG(1,(_T("####SearchResultList: %s, cnt=%d, take %dms\r\n"),SearchString,i,GetTickCount()-dwTick));

	return i;
}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
void CPanelMediaLibrary::ArrowChange()
{
	int iconMarginRight = -14;
	CListBoxItemGL* pitem = m_boxDevice.m_listItems.GetItemByID(m_idCurMediaType);
	if (pitem)
	{
		pitem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginRight, 0);
	}
	pitem = m_boxDevice.GetCurFocusItem();
	if (pitem)
	{
		pitem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginRight, 0);
	}
}
#endif