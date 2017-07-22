#include "PanelImageLibrary.h"
#include "../../resource.h"

#if (CUSTOMER_ID == CUSTOMER_PROLOGY)
#define LIB_BROWSE_BTN_DISPLAY_REVERSE	1
#else
#define LIB_BROWSE_BTN_DISPLAY_REVERSE	0
#endif

CPanelImageLibrary::CPanelImageLibrary(void)
{
	m_hExploreThread = INVALID_HANDLE_VALUE;
	m_hRefreshEvent = INVALID_HANDLE_VALUE;

	m_bRefreshingLib = FALSE;

	m_bNeedRefreshLib = TRUE;

	m_type = MEDIA_LIBRARY_TRACKS;

	for (int i=0;i<MEDIA_TYPE_NUMBER;i++)
	{
		m_idValidSrcList[i]=0;
	}
	m_idCurDeviceID = MEDIA_TYPE_INVALID;
	m_dwSetShowTypeTick = 0;
}
CPanelImageLibrary::~CPanelImageLibrary(void)
{

}

BOOL CPanelImageLibrary::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelImageLibrary::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{	
	RETAILMSG(1,(_T("MSG:CPanelImageLibrary::OnEnterPanel\r\n")));

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

void CPanelImageLibrary::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

	if(m_boxLibrary.IsOnQuickSearchMode())
	{
		m_boxLibrary.ShowQuickSearchMode(FALSE);
	}
}

void CPanelImageLibrary::Render()
{
	if(!BeginRender())return;
	BOOL bControlEnable = !m_bRefreshingLib;

	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		BOOL bOnSearch=m_boxLibrary.IsOnQuickSearchMode();

		m_boxLibrary.EnableNotify(bControlEnable);
		m_boxDevice.EnableNotify(bControlEnable && !bOnSearch );
		m_btnNowPlaying.EnableNotify(bControlEnable && !bOnSearch );
		m_btnHome.EnableNotify(bControlEnable && !bOnSearch );
		m_btnCatalogAudio.EnableNotify(bControlEnable && !bOnSearch);
		m_btnCatalogVideo.EnableNotify(bControlEnable && !bOnSearch);
		m_btnCatalogImage.EnableNotify(bControlEnable && !bOnSearch);
// 		m_btnCatalogArtist.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnSearch.EnableNotify(bControlEnable && !bOnSearch);
		m_btnBrowseMode.EnableNotify(bControlEnable && !bOnSearch);
	}
	else if(m_type==MEDIA_LIBRARY_SEARCH_RESULT)
	{
		BOOL bOnSearch=m_boxSearched.IsOnQuickSearchMode();

		m_boxSearched.EnableNotify(bControlEnable);
		m_boxDevice.EnableNotify(bControlEnable && !bOnSearch );
		m_btnNowPlaying.EnableNotify(bControlEnable && !bOnSearch );
		m_btnHome.EnableNotify(bControlEnable && !bOnSearch );
		m_btnCatalogAudio.EnableNotify(bControlEnable && !bOnSearch);
		m_btnCatalogVideo.EnableNotify(bControlEnable && !bOnSearch);
		m_btnCatalogImage.EnableNotify(bControlEnable && !bOnSearch);
// 		m_btnCatalogArtist.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnSearch.EnableNotify(bControlEnable && !bOnSearch);
		m_btnBrowseMode.EnableNotify(bControlEnable && !bOnSearch);
	}
	else
	{
		BOOL bOnSearch=m_boxExplorer.IsOnQuickSearchMode();

		m_boxExplorer.EnableNotify(bControlEnable);
		m_boxDevice.EnableNotify(bControlEnable && !bOnSearch );
		m_btnNowPlaying.EnableNotify(bControlEnable && !bOnSearch );
		m_btnHome.EnableNotify(bControlEnable && !bOnSearch );
		m_btnCatalogAudio.EnableNotify(bControlEnable && !bOnSearch);
		m_btnCatalogVideo.EnableNotify(bControlEnable && !bOnSearch);
		m_btnCatalogImage.EnableNotify(bControlEnable && !bOnSearch);
// 		m_btnCatalogArtist.EnableNotify(bControlEnable && !bOnSearch && bMassStorage);
		m_btnSearch.EnableNotify(bControlEnable && !bOnSearch);
		m_btnBrowseMode.EnableNotify(bControlEnable && !bOnSearch);
	}

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

	m_btnNowPlaying.SetAlpha(m_fCurAlpha);
	m_btnNowPlaying.Render();

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	if(!m_bRefreshingLib)
	{
		if(m_type==MEDIA_LIBRARY_TRACKS)
		{
			m_boxLibrary.SetAlpha(m_fCurAlpha);
			m_boxLibrary.Render();

			m_tagNote.Show(m_boxLibrary.GetCount()==0);
			m_tagNote.SetAlpha(m_fCurAlpha);
			m_tagNote.Render();
		}
		else if(m_type==MEDIA_LIBRARY_SEARCH_RESULT)
		{
			m_boxSearched.SetAlpha(m_fCurAlpha);
			m_boxSearched.Render();

			m_tagNote.Show(m_boxSearched.GetCount()==0);
			m_tagNote.SetAlpha(m_fCurAlpha);
			m_tagNote.Render();

			if(m_boxSearched.GetCount()==0 && GetTickCount()-m_dwSetShowTypeTick > 3000)
			{
				SetBrowseType(MEDIA_LIBRARY_TRACKS);
			}
		}
		else
		{
			m_boxExplorer.SetAlpha(m_fCurAlpha);
			m_boxExplorer.Render();
		}

		m_btnCatalogAudio.SetAlpha(m_fCurAlpha);
		m_btnCatalogAudio.Render();
		m_btnCatalogVideo.SetAlpha(m_fCurAlpha);
		m_btnCatalogVideo.Render();
		m_btnCatalogImage.SetAlpha(m_fCurAlpha);
		m_btnCatalogImage.Render();
		m_btnBrowseMode.SetAlpha(m_fCurAlpha);
		m_btnBrowseMode.Render();
		m_btnSearch.SetAlpha(m_fCurAlpha);
		m_btnSearch.Render();
	}

// 	if(IsAniReady() && m_bNeedRefreshLib)
// 	{
// 		RefreshLibrary();
// 	}

	SetFrameTime(m_bRefreshingLib?30:500);

	if(m_bRefreshingLib)
	{
		VIEW_STATE vs=m_iconLoading.GetCurViewState();
		vs.fRotateZ-=5;
		if(vs.fRotateZ<-360)
			vs.fRotateZ=0;
		m_iconLoading.SetCurViewState(&vs);
		m_iconLoading.SetAlpha(m_fCurAlpha);
		m_iconLoading.Render();
	}


	EndRender();
}

BOOL CPanelImageLibrary::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
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

	m_iconArrow.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_277));

	m_iconDevUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_252));
	m_iconDevUSBFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_251));
	m_iconDevSD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_249));
	m_iconDevSDFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_248));
	m_iconImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_264));
	m_iconImageFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_263));
	m_iconFolder.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_275));
	m_iconFolderFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_274));
	m_iconExplorer.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_275));
	m_iconExplorerFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_274));
	m_iconSearch.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_272));
	m_iconCatalogAudio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_244));
	m_iconCatalogVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_270));
	m_iconCatalogImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_264));
	m_iconCatalogImageFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_263));
	m_iconBrowseFiles.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_264));
	m_iconBrowseFolders.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_275));

	COLORGL cr_gray={128.0f/256.0f,129.0f/256.0f,132.0f/256.0f,1.0};
	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
	COLORGL cr_highlight={1.0f,1.0f,1.0f,1.0};

	int cx=LIST_BOX_ITEM_WIDTH;	//186
	int cy=LIST_BOX_ITEM_HEIGHT;//100

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
	SIZE sic={cx_btn,cy+0};

	short category_font = 14;

	m_btnCatalogAudio.Initialize(IDC_IMAGE_BTN_CATALOG_AUDIO,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogAudio.SetSize(&sic);
	//m_btnCatalogAudio.SetShowScale(0.9f);
	m_btnCatalogAudio.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnCatalogAudio.SetPos(cx/2-cx_btn*2,g_iClientHeight/2-cy/2);
	m_btnCatalogAudio.LinkTextGL(pTextGL);
	m_btnCatalogAudio.SetIcon(&m_iconCatalogAudio);
	m_btnCatalogAudio.SetCaption(GetResourceString(IDS_TITLE_MUSIC));
	m_btnCatalogAudio.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnCatalogAudio.SetCaptionColor(&cr_gray);
	m_btnCatalogAudio.SetCaptionHeight(category_font);
	m_btnCatalogAudio.SetIconPos(-sic.cx/2+32,0);

	m_btnCatalogVideo.Initialize(IDC_IMAGE_BTN_CATALOG_VIDEO,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogVideo.SetSize(&sic);
	//m_btnCatalogVideo.SetShowScale(0.9f);
	m_btnCatalogVideo.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnCatalogVideo.SetPos(cx/2-cx_btn,g_iClientHeight/2-cy/2);
	m_btnCatalogVideo.LinkTextGL(pTextGL);
	m_btnCatalogVideo.SetIcon(&m_iconCatalogVideo);
	m_btnCatalogVideo.SetCaption(GetResourceString(IDS_TITLE_VIDEO));
	m_btnCatalogVideo.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnCatalogVideo.SetCaptionColor(&cr_gray);
	m_btnCatalogVideo.SetCaptionHeight(category_font);
	m_btnCatalogVideo.SetIconPos(-sic.cx/2+32,0);

	m_btnCatalogImage.Initialize(IDC_IMAGE_BTN_CATALOG_IMAGE,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogImage.SetSize(&sic);
	//m_btnCatalogImage.SetShowScale(0.9f);
	m_btnCatalogImage.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnCatalogImage.SetPos(cx/2-0,g_iClientHeight/2-cy/2);
	m_btnCatalogImage.LinkTextGL(pTextGL);
	m_btnCatalogImage.SetIcon(&m_iconCatalogImage,&m_iconCatalogImageFocus);
	m_btnCatalogImage.SetCaption(GetResourceString(IDS_TITLE_IMAGE));
	m_btnCatalogImage.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnCatalogImage.SetCaptionColor(&cr_w);
	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
		m_btnCatalogImage.SetCaptionHeight(category_font);
	else
		m_btnCatalogImage.SetCaptionHeight(category_font);
	m_btnCatalogImage.SetIconPos(-sic.cx/2+32,0);
	m_btnCatalogImage.SetStatus(BS_FOCUS);

	//
	m_btnBrowseMode.Initialize(IDC_IMAGE_BTN_BROWSE_MODE,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBrowseMode.SetSize(&sic);
	//m_btnBrowseMode.SetShowScale(0.9f);
	m_btnBrowseMode.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnBrowseMode.SetPos(cx/2+cx_btn,g_iClientHeight/2-cy/2);
	m_btnBrowseMode.LinkTextGL(pTextGL);

#if LIB_BROWSE_BTN_DISPLAY_REVERSE
	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFolders);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
	}
	else
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFiles);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
#else
	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFiles);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
	else
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFolders);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
	}
#endif

	m_btnBrowseMode.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnBrowseMode.SetCaptionColor(&cr_gray);
	m_btnBrowseMode.SetCaptionHeight(category_font);
	m_btnBrowseMode.SetIconPos(-sic.cx/2+32,0);

	//
	m_btnSearch.Initialize(IDC_IMAGE_BTN_SEARCH,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_282),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearch.SetSize(&sic);
	//m_btnSearch.SetShowScale(0.9f);
	m_btnSearch.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnSearch.SetPos(cx/2+cx_btn*2,g_iClientHeight/2-cy/2);
	m_btnSearch.SetIcon(&m_iconSearch);
	m_btnSearch.LinkTextGL(pTextGL);
	m_btnSearch.SetCaption(GetResourceString(IDS_SEARCH));
	m_btnSearch.SetCaptionPos(BTN_CAPTION_LEFT,-sic.cx/2+119,0);
	m_btnSearch.SetCaptionColor(&cr_gray);
	m_btnSearch.SetCaptionHeight(category_font);
	m_btnSearch.SetIconPos(-sic.cx/2+32,0);


	LISTBOXSTRUCTURE box;


	int ispace=0;

	memset(&box,0,sizeof(box));

	//find param:
	int count=(g_iClientHeight-cy*2)/cy;
	if((g_iClientHeight-cy*2)%cy > (cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy*2)/count/2*2;
	//ispace=(g_iClientHeight-cy*2-box.iSpacing*count) / 2;
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

	m_boxDevice.Initialize(IDC_IMAGE_BOX_DEVICE,this,&box,pSkinManager);
	m_boxDevice.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_boxDevice.SetPos(-g_iClientWidth/2+cx/2,0);
	m_boxDevice.LinkTextGL(pTextGL);
	m_boxDevice.Show(TRUE,FALSE);
	m_boxDevice.SetAlpha(1.0f);
	m_boxDevice.SetCaptioniHeight(18);
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
	box.iSpacing=(g_iClientHeight - cy)/5;
	box.siClient.cx = g_iClientWidth-cx;
	box.siClient.cy = g_iClientHeight - cy;
	box.siBorder.cx = g_iClientWidth-cx;
	box.siBorder.cy = g_iClientHeight - cy;
	box.idTexIndexBase=TEXID_066;
	box.idTexIndexFocus=TEXID_242;//TEXID_361;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_065;
	box.idTexBtnUp=TEXID_066;
	box.idTexBtnDown=TEXID_241;
	box.idTexSliBase=TEXID_060;
	box.idTexSliChannel=TEXID_060;
	box.idTexSliThumb=TEXID_276;
	box.bNoSlider=FALSE;
	box.bNoBorder=FALSE;	//TRUE
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=TRUE;
	box.pxSlider=-8;
	box.fIndexBaseOffset = 0;//60;    //74

	m_boxLibrary.Initialize(IDC_IMAGE_BOX_LIBRARY,this,&box,pSkinManager);
	m_boxLibrary.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	//m_boxLibrary.SetMarginHor(38);
	//m_boxLibrary.SetAlpha(0.9f);
	m_boxLibrary.SetPos(cx/2,-g_iClientHeight/2+203);
	m_boxLibrary.LinkTextGL(pTextGL);
	m_boxLibrary.Show(TRUE,FALSE);
	m_boxLibrary.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxLibrary.SetCaptioniHeight(22);

	//
	m_boxExplorer.Initialize(IDC_IMAGE_BOX_EXPLORER,this,&box,pSkinManager);
	m_boxExplorer.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	//m_boxExplorer.SetMarginHor(48);
	m_boxExplorer.SetPos(cx/2,-g_iClientHeight/2+203);
	m_boxExplorer.LinkTextGL(pTextGL);
	m_boxExplorer.Show(TRUE,FALSE);
	//m_boxExplorer.SetAlpha(1.0f);
	m_boxExplorer.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxExplorer.SetCaptioniHeight(22);

	m_boxSearched.Initialize(IDC_IMAGE_BOX_SEARCH,this,&box,pSkinManager);
	m_boxSearched.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_boxSearched.SetMarginHor(48);
	m_boxSearched.SetPos(cx/2,-cy/2);
	m_boxSearched.LinkTextGL(pTextGL);
	m_boxSearched.Show(FALSE,FALSE);
	m_boxSearched.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxSearched.SetCaptioniHeight(22);

	//SetShowType(m_type);

	SIZE sibtn={cx,cy};

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_204));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));
	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_280));
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_279));

	m_btnHome.Initialize(IDC_IMAGE_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_288),NULL,
		m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL);
	//m_btnHome.SetShowScale(0.9f);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2+ispace/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.SetIconPos(-cx/2+35,0);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,68);
	m_btnHome.SetCaptionColor(&cr_w,&cr_gray);
	m_btnHome.SetCaptionHeight(20);


	m_btnNowPlaying.Initialize(IDC_IMAGE_BTN_NOWPLAYING,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnNowPlaying.SetShowScale(0.9f);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnNowPlaying.SetPos(-g_iClientWidth/2+cx/2,g_iClientHeight/2-cy/2-ispace/2);
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
// 	COLORGL cr_gray={176.0f/256.0f,109.0f/256.0f,142.0f/256.0f,1.0};
// 	COLORGL cr_red={255.0f/256.0f,16.0f/256.0f,62.0f/256.0f,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	//COLORGL cr_highlight={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_highlight={255.0f/255.0f,63.0f/255.0f,63.0f/255.0f,1.0};

	SIZE si;
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

	//m_iconImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_090));

	m_iconDevUSB.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_209));
    m_iconDevUSB.SetShowScale(0.8f);
	m_iconDevSD.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_211));
    m_iconDevSD.SetShowScale(0.8f);
	m_iconDevUSBFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_210));
    m_iconDevUSBFocus.SetShowScale(0.8f);
	m_iconDevSDFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_212));
    m_iconDevSDFocus.SetShowScale(0.8f);

	int cx=LIST_BOX_ITEM_WIDTH;     //234    //240;
	int cy=LIST_BOX_ITEM_HEIGHT;    //100

	int cx_btn=(g_iClientWidth-cx)/5;
	SIZE sic={cx_btn,cy+0};


	m_iconLoading.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_713),NULL);
	m_iconLoading.SetShowScale(1.44f);
	m_iconLoading.SetPos(cx/2,-cy/2);
	m_iconSearch.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_222));
    m_iconSearch.SetShowScale(0.8f);
	m_iconCatalogAudio.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_219));
    m_iconCatalogAudio.SetShowScale(0.8f);
	m_iconCatalogVideo.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_220));
    m_iconCatalogVideo.SetShowScale(0.8f);
	m_iconCatalogImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_227));
    m_iconCatalogImage.SetShowScale(0.8f);
	m_iconImage.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_227));
	m_iconImage.SetShowScale(0.8f);
	m_iconFolders.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_223));
	m_iconFolders.SetShowScale(0.8f);
	m_iconBrowseFiles.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_227));
	m_iconBrowseFiles.SetShowScale(0.8f);
	m_iconBrowseFolders.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_223));
	m_iconBrowseFolders.SetShowScale(0.8f);


	SIZE sibtn={cx,cy+0};

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_215));
	m_iconHomeUp.SetShowScale(0.8f);
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_216));
	m_iconHomeDown.SetShowScale(0.8f);
	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));
	m_iconNowPlayingUp.SetShowScale(0.8f);
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_206));
	m_iconNowPlayingDown.SetShowScale(0.8f);

	m_btnHome.Initialize(IDC_IMAGE_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_251),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_w);
	m_btnHome.SetCaptionHeight(22);
	m_btnHome.SetIconPos(-cx/2+31,0);

	m_btnNowPlaying.Initialize(IDC_IMAGE_BTN_NOWPLAYING,this,m_pSkinManager->GetTexture(TEXID_251),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnNowPlaying.SetPos(-g_iClientWidth/2+cx/2,g_iClientHeight/2-cy/2);
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

	//
	m_btnCatalogAudio.Initialize(IDC_IMAGE_BTN_CATALOG_AUDIO,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogAudio.SetSize(&sic);
	m_btnCatalogAudio.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnCatalogAudio.SetPos(cx/2-cx_btn*2,g_iClientHeight/2-cy/2);
	m_btnCatalogAudio.LinkTextGL(pTextGL);
	m_btnCatalogAudio.SetIcon(&m_iconCatalogAudio);
	m_btnCatalogAudio.SetCaption(GetResourceString(IDS_TITLE_MUSIC));
	m_btnCatalogAudio.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnCatalogAudio.SetCaptionColor(&cr_gray);
	m_btnCatalogAudio.SetCaptionHeight(16);
	m_btnCatalogAudio.SetIconPos(0,10);

	m_btnCatalogVideo.Initialize(IDC_IMAGE_BTN_CATALOG_VIDEO,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogVideo.SetSize(&sic);
	m_btnCatalogVideo.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnCatalogVideo.SetPos(cx/2-cx_btn,g_iClientHeight/2-cy/2);
	m_btnCatalogVideo.LinkTextGL(pTextGL);
	m_btnCatalogVideo.SetIcon(&m_iconCatalogVideo);
	m_btnCatalogVideo.SetCaption(GetResourceString(IDS_TITLE_VIDEO));
	m_btnCatalogVideo.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnCatalogVideo.SetCaptionColor(&cr_gray);
	m_btnCatalogVideo.SetCaptionHeight(16);
	m_btnCatalogVideo.SetIconPos(0,10);

	m_btnCatalogImage.Initialize(IDC_IMAGE_BTN_CATALOG_IMAGE,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCatalogImage.SetSize(&sic);
	m_btnCatalogImage.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnCatalogImage.SetPos(cx/2-0,g_iClientHeight/2-cy/2);
	m_btnCatalogImage.LinkTextGL(pTextGL);
	m_btnCatalogImage.SetIcon(&m_iconCatalogImage);
	m_btnCatalogImage.SetCaption(GetResourceString(IDS_TITLE_IMAGE));
	m_btnCatalogImage.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnCatalogImage.SetCaptionColor(&cr_gray);
    if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
    	m_btnCatalogImage.SetCaptionHeight(14);
    else
    	m_btnCatalogImage.SetCaptionHeight(16);	m_btnCatalogImage.SetIconPos(0,10);
	m_btnCatalogImage.SetStatus(BS_FOCUS);

	//
	m_btnBrowseMode.Initialize(IDC_IMAGE_BTN_BROWSE_MODE,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnBrowseMode.SetSize(&sic);
	m_btnBrowseMode.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnBrowseMode.SetPos(cx/2+cx_btn,g_iClientHeight/2-cy/2);
	m_btnBrowseMode.LinkTextGL(pTextGL);

#if LIB_BROWSE_BTN_DISPLAY_REVERSE
	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFolders);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
	}
	else
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFiles);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
#else
	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFiles);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
	else
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFolders);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
	}
#endif

	m_btnBrowseMode.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnBrowseMode.SetCaptionColor(&cr_gray);
	m_btnBrowseMode.SetCaptionHeight(16);
	m_btnBrowseMode.SetIconPos(0,10);

	//
	m_btnSearch.Initialize(IDC_IMAGE_BTN_SEARCH,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSearch.SetSize(&sic);
	m_btnSearch.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_btnSearch.SetPos(cx/2+cx_btn*2,g_iClientHeight/2-cy/2);
	m_btnSearch.SetIcon(&m_iconSearch);
	m_btnSearch.LinkTextGL(pTextGL);
	m_btnSearch.SetCaption(GetResourceString(IDS_SEARCH));
	m_btnSearch.SetCaptionPos(BTN_CAPTION_CENTER,0,-24);
	m_btnSearch.SetCaptionColor(&cr_gray);
	m_btnSearch.SetCaptionHeight(16);
	m_btnSearch.SetIconPos(0,10);


	LISTBOXSTRUCTURE box;
	memset(&box,0,sizeof(box));
	int ispace=0;

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
	box.bNoBorder=TRUE;//FALSE;//
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxDevice.Initialize(IDC_IMAGE_BOX_DEVICE,this,&box,pSkinManager);
	m_boxDevice.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_boxDevice.SetPos(-g_iClientWidth/2+cx/2,0);
	m_boxDevice.LinkTextGL(pTextGL);
	m_boxDevice.Show(TRUE,FALSE);
	m_boxDevice.SetAlpha(1.0f);
	m_boxDevice.SetCaptioniHeight(24);
	m_boxDevice.SetCaptionColor(&cr_gray,&cr_w);


// 	CListBoxItemGL *pItem;
// 	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB));
// 	pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus);
// 	// 	//pItem->SetIcon2(&m_iconLibTracks);
// 	m_boxDevice.AddItem(pItem,MEDIA_TYPE_USB1);
// 	pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD));
// 	pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus);
// 	// 	//pItem->SetIcon2(&m_iconLibExplorer);
// 	m_boxDevice.AddItem(pItem,MEDIA_TYPE_SD1);


	//list:
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

	m_boxLibrary.Initialize(IDC_IMAGE_BOX_LIBRARY,this,&box,pSkinManager);
	m_boxLibrary.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_boxLibrary.SetMarginHor(38);
	m_boxLibrary.SetPos(cx/2,-cy/2);
	m_boxLibrary.LinkTextGL(pTextGL);
	m_boxLibrary.Show(TRUE,FALSE);
	m_boxLibrary.SetAlpha(1.0f);
	m_boxLibrary.SetCaptionColor(&cr_gray,&cr_highlight);
    m_boxLibrary.SetCaptioniHeight(22);

	m_boxExplorer.Initialize(IDC_IMAGE_BOX_EXPLORER,this,&box,pSkinManager);
	m_boxExplorer.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_boxExplorer.SetMarginHor(48);
	m_boxExplorer.SetPos(cx/2,-cy/2);
	m_boxExplorer.LinkTextGL(pTextGL);
	m_boxExplorer.Show(FALSE,FALSE);
	m_boxExplorer.SetAlpha(1.0f);
	m_boxExplorer.SetCaptionColor(&cr_gray,&cr_highlight);
    m_boxExplorer.SetCaptioniHeight(22);

	m_boxSearched.Initialize(IDC_IMAGE_BOX_SEARCH,this,&box,pSkinManager);
	m_boxSearched.SetNotifyFunc(&CPanelImageLibrary::OnNotify,this);
	m_boxSearched.SetMarginHor(48);
	m_boxSearched.SetPos(cx/2,-cy/2);
	m_boxSearched.LinkTextGL(pTextGL);
	m_boxSearched.Show(FALSE,FALSE);
	m_boxSearched.SetAlpha(1.0f);
	m_boxSearched.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxSearched.SetCaptioniHeight(22);


	COLORGL crb={0.9,0.9,0.9,1.0};
	SIZE siInfo={600,40};
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
#endif

	return TRUE;
}


void CALLBACK CPanelImageLibrary::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_IMAGE,(_T("APP:MSG:GUI:Main: [CPanelImageLibrary::OnNotify]: id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara,lpUserPara));

	CPanelImageLibrary *panel=(CPanelImageLibrary*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelImageLibrary::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(1, (TEXT("MSG:[%s]: idControl[%d]\r\n"),TEXT(__FUNCTION__),idControl));
	switch (idControl)
	{
	case IDC_IMAGE_BTN_HOME:
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
		PlaySoundEffect(0);
		break;
	case IDC_IMAGE_BTN_NOWPLAYING:
		//SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY,PAGE_SWITCH_ANI_NONE);
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_RIGHT);
		PlaySoundEffect(0);
		break;
	case IDC_IMAGE_BTN_CATALOG_AUDIO:
		SendMediaCommand(MEDIA_CMD_CATEGORY,MEDIA_AUDIO);
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY,PAGE_SWITCH_ANI_NONE);
		PlaySoundEffect(0);
		break;
	case IDC_IMAGE_BTN_CATALOG_VIDEO:
		SendMediaCommand(MEDIA_CMD_CATEGORY,MEDIA_VIDEO);
		SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_LIBRARY,PAGE_SWITCH_ANI_NONE);
		PlaySoundEffect(0);
		break;
	case IDC_IMAGE_BTN_CATALOG_IMAGE:
		{
			if(m_type == MEDIA_LIBRARY_SEARCH_RESULT)
			{
				SetBrowseType(MEDIA_LIBRARY_TRACKS);
			}
		}
		break;
	case IDC_IMAGE_BTN_BROWSE_MODE:
		SetBrowseType(m_type==MEDIA_LIBRARY_EXPLORER?MEDIA_LIBRARY_TRACKS:MEDIA_LIBRARY_EXPLORER);
		PlaySoundEffect(0);
		break;
	case IDC_IMAGE_BTN_SEARCH:
		{
#if (CVTE_EN_SEARCH_USE_FULL_KEYBOARD)
			ShowKeyboard(IDC_PANEL_KEYBOARD+IDC_IMAGE_BTN_SEARCH,&CPanelImageLibrary::OnNotify,this,INPUT_LETTERS_CAP,
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
	case IDC_PANEL_KEYBOARD+IDC_IMAGE_BTN_SEARCH:
		{
			if(lpPara)
			{
				RETAILMSG(1,(_T("search media file :[%s]\r\n"),(const wchar_t*)lpPara));
				if(wcslen((const wchar_t*)lpPara))
				{
					CreateSearchResultList((const wchar_t*)lpPara,&m_boxLibrary,&m_boxSearched);
					SetBrowseType(MEDIA_LIBRARY_SEARCH_RESULT);
				}
			}
		}
		break;
	case IDC_IMAGE_BOX_SEARCH:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;

			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
			case NOTIFY_DOUBLE_CLICK:
				{
					RETAILMSG(1,(_T("click search media file index :[%d]\r\n"),id_item));
					SendImageCommand(IMAGE_CMD_PLAY_INDEX,id_item);
					SetBrowseType(MEDIA_LIBRARY_TRACKS);
				}
				break;
			default:
				break;
			}
		}
		break;
	case IDC_IMAGE_BOX_LIBRARY:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
			case NOTIFY_DOUBLE_CLICK:
				{
					//begin to play
					SendImageCommand(IMAGE_CMD_PLAY_INDEX,index);

					SwitchPage(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_IMAGE_BOX_DEVICE:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
			case NOTIFY_DOUBLE_CLICK:
				{
					SelectDevice(id_item);
					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_IMAGE_BOX_EXPLORER:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_SHORT_CLICK:
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

						//在OnTouchMsg里面抬起手的时候会load图片，感觉这里可以不用重复两次?
						SendImageCommand(IMAGE_CMD_LOAD_FILE,(LPARAM)m_boxExplorer.GetCurFocusItemValue());
						//切换到图片主页面
						SwitchPage(UI_CLASS_IMAGE,PAGE_IMAGE_MAIN);
						PlaySoundEffect(0);
					}
				}
				break;
			}
		}
		break;
	}
}
void CPanelImageLibrary::GotoParent()
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
void CPanelImageLibrary::EnterCurPath()
{
	RETAILMSG(DEBUG_IMAGE,(_T("****CPanelImageLibrary::EnterCurPath\r\n")));

	if(m_type==MEDIA_LIBRARY_EXPLORER)
	{
		if(m_boxExplorer.GetCurFocusItemID()==MEDIA_FILE_TYPE_FOLDER)
		{
			wchar_t temp[MAX_PATH];
			if(m_strCurRoot.String())
			{
				RETAILMSG(DEBUG_MEDIA,(_T("****CPanelImageLibrary::EnterCurPath begin :%s\r\n"),m_boxExplorer.GetCurFocusItemName()));
				if(m_strCurPath.String())
				{
					swprintf_s(temp,MAX_PATH,_T("%s%s\\"),m_strCurPath.String(),m_boxExplorer.GetCurFocusItemName());
				}
				else
				{
					swprintf_s(temp,MAX_PATH,_T("%s%s\\"),m_strCurRoot.String(),m_boxExplorer.GetCurFocusItemName());
				}
				SetCurPath(temp);
				RETAILMSG(DEBUG_IMAGE,(_T("****CPanelMediaLibrary::EnterCurPath end :%s\r\n"),temp));
			}
		}
	}
}

BOOL CPanelImageLibrary::IsImageFile(LPCTSTR szFileName)
{
	WCHAR strExt[16]={0};
	INT iLen=wcslen(szFileName);

	if(!GetFileExtensionFromPath(szFileName,strExt,16))
		return FALSE;

	if(wcslen(strExt)<1)
		return FALSE;

	static LPCTSTR s_pszExts[] =
	{
		_T("bmp"),
		_T("gif"),
		_T("jpg"),
		_T("png")
	};
	// 	_T("bmp"),
	// 		_T("tif")

	for (int i = 0; i < _countof(s_pszExts); i++)
	{
		if (lstrcmpi(s_pszExts[i], strExt) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL CPanelImageLibrary::ExploreImageFiles(LPCTSTR lpszPath)
{

	TCHAR szFind[MAX_PATH];
	CListBoxItemGL* pItem;
	wcscpy_s(szFind,MAX_PATH,lpszPath); //
	wcscat_s(szFind,MAX_PATH,_T("*.*")); //
	WIN32_FIND_DATA wfd;
	HANDLE hFind=FindFirstFile(szFind,& wfd);

	TCHAR szFile[1024];

	if(hFind==INVALID_HANDLE_VALUE) //
		return FALSE;
	do
	{
		if(wfd.cFileName[0]=='.')
			continue; //


		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			TCHAR szFile[MAX_PATH];

			swprintf_s(szFile,MAX_PATH,_T("%s%s\\"),lpszPath,wfd.cFileName);
			ExploreImageFiles(szFile); //
		}
		else if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			//cut max path:
			int len=swprintf_s(szFile,1024,_T("%s%s"),lpszPath,wfd.cFileName);
			if(len>MAX_PATH)
				//if(wcslen(szFile)>MAX_PATH)
			{
				RETAILMSG(DEBUG_IMAGE,(_T("ExploreMeidaFiles:filename too long, pass\r\n")));
				continue;
			}


			if(IsImageFile(wfd.cFileName))
			{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
				pItem->ShowValue(FALSE);
				pItem->SetIcon(&m_iconImage,&m_iconImageFocus,14,0);
#else
				pItem=new CListBoxItemGL(wfd.cFileName,szFile);
				pItem->ShowValue(FALSE);
				pItem->SetIcon(&m_iconImage);
#endif
				m_boxLibrary.AddItemByOrder(pItem);
				//m_boxLibrary.AddItem(pItem);

				int num=m_boxLibrary.GetCount();
				//m_iTotalPage=num/IMAGE_THUMB_COUNT_PERPAGE+((num%IMAGE_THUMB_COUNT_PERPAGE)?1:0);
			}
		}
		Sleep(0);
	}
	while(FindNextFile(hFind,&wfd));
	FindClose(hFind);

	return TRUE;
}

DWORD CPanelImageLibrary::ExploreThreadPro(LPVOID lpParam)
{
	CPanelImageLibrary *panel=(CPanelImageLibrary*)lpParam;

	DWORD dwTick=GetTickCount();

	RETAILMSG(DEBUG_IMAGE,(_T("APP:MSG:GUI:Main: [CPanelImageLibrary::ExploreThreadPro]: Begin ExploreImageFiles....\r\n")));

	// 	//storage only

	// 	panel->ExploreImageFiles(_T("\\ResidentFlash\\"));
	// 	panel->ExploreImageFiles(_T("\\GPS CARD\\"));
	if(panel->m_idCurDeviceID == MEDIA_TYPE_USB1)
		panel->ExploreImageFiles(_T("\\USB Disk\\"));
	else if(panel->m_idCurDeviceID == MEDIA_TYPE_USB2)
		panel->ExploreImageFiles(_T("\\USB Disk2\\"));
	else if(panel->m_idCurDeviceID == MEDIA_TYPE_SD1)
		panel->ExploreImageFiles(_T("\\MEDIA CARD\\"));

	Sleep(400);

	RETAILMSG(DEBUG_IMAGE,(_T("APP:MSG:GUI:Main: [CPanelImageLibrary::ExploreThreadPro]: End ExploreImageFiles, counts:%d..take:%d ms\r\n"),panel->m_boxLibrary.GetCount(),GetTickCount()-dwTick));

	panel->m_bRefreshingLib=FALSE;

	SetEvent(panel->m_hRefreshEvent);

	return 0;
}

BOOL CPanelImageLibrary::IsOnRoot()
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
void CPanelImageLibrary::SetCurPath(const wchar_t* lpszPath)
{
	if(lpszPath)
	{
		if( !m_strCurPath.String() || (m_strCurPath.String()&&(lstrcmpi(m_strCurPath.String(), lpszPath) != 0)) )
		{
			RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageLibrary::SetCurPath]: %s\r\n"),lpszPath));

			m_strCurPath=lpszPath;
			//m_bPathChanged=TRUE;
			ExplorePath(lpszPath);
		}
		else
		{
			//the same path:

		}
	}
}
void CPanelImageLibrary::ExplorePath(const wchar_t* lpszPath)
{
	TCHAR szFind[MAX_PATH];
	TCHAR szFile[1024];

	RETAILMSG(DEBUG_IMAGE,(_T("CPanelMediaLibrary::ExplorePath:%s\r\n"),lpszPath));

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
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_MEDIA_BROWSE_PARENT_FOLDER),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconFolder,&m_iconFolderFocus,14,0);
#else
		//add parent:
		pItem=new CListBoxItemGL(GetResourceString(IDS_MEDIA_BROWSE_PARENT_FOLDER));
		pItem->SetIcon(&m_iconFolders);
#endif
		m_boxExplorer.AddItem(pItem,MEDIA_FILE_TYPE_FOLDER);
		m_iExplorerFolderCount++;
	}

	if(hFind==INVALID_HANDLE_VALUE) //
	{
		return;
	}


	do
	{
		RETAILMSG(DEBUG_IMAGE,(_T("CPanelImageLibrary::find:%s,dwFileAttributes:0x%x\r\n"),wfd.cFileName,wfd.dwFileAttributes));

		if(wfd.cFileName[0]=='.')
			continue;

		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			swprintf_s(szwFile,MAX_PATH,_T("%s"),wfd.cFileName);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			pItem=new CListBoxItemGL(szwFile,NULL,NULL,30,0);
			pItem->ShowValue(FALSE);
			pItem->SetIcon(&m_iconFolder,&m_iconFolderFocus,14,0);
#else
			pItem=new CListBoxItemGL(szwFile);
			pItem->SetIcon(&m_iconFolders);
#endif
			m_boxExplorer.AddItemByOrder(pItem,MEDIA_FILE_TYPE_FOLDER);

			m_iExplorerFolderCount++;
		}
		else if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			//cut max path:
			swprintf_s(szFile,1024,_T("%s%s"),lpszPath,wfd.cFileName);
			if(wcslen(szFile)>MAX_PATH)
			{
				RETAILMSG(DEBUG_IMAGE,(_T("CPanelImageLibrary:filename too long, pass\r\n")));
				continue;
			}

			if(IsImageFile(wfd.cFileName))
			{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				pItem=new CListBoxItemGL(wfd.cFileName,szFile,NULL,30,0);
				pItem->ShowValue(FALSE);
				pItem->SetIcon(&m_iconImage,&m_iconImageFocus,14,0);
#else
				pItem=new CListBoxItemGL(wfd.cFileName,szFile);
				pItem->ShowValue(FALSE);
				pItem->SetIcon(&m_iconImage);
#endif
				m_boxExplorer.AddItemByOrder(pItem,MEDIA_FILE_TYPE_ALL,TRUE);

				m_iExplorerFilesCount++;
			}
		}
		Sleep(0);
	}
	while(FindNextFile(hFind,&wfd));
	FindClose(hFind); //
}



void CPanelImageLibrary::RefreshLibrary()
{
	if(m_bRefreshingLib)
	{
		RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageLibrary::RefreshLibrary]: Still refreshing! cancelr\n")));

		return;
	}

	RETAILMSG(DEBUG_IMAGE,(_T("MSG:[CPanelImageLibrary::RefreshLibrary]: begin to RefreshLibraryr\n")));

	m_bRefreshingLib = TRUE;

	ResetEvent(m_hRefreshEvent);

	m_boxLibrary.ClearList();

	if(m_hExploreThread!=INVALID_HANDLE_VALUE)
		CloseHandle(m_hExploreThread);

	//begin thread to explore for video files:
	m_hExploreThread=CreateThread(NULL,NULL,ExploreThreadPro,this,NULL,NULL);

	m_bNeedRefreshLib = FALSE;
}

void CPanelImageLibrary::ClearList()
{
	m_boxLibrary.ClearList();
}

DWORD CPanelImageLibrary::WaitForRefreshDone(DWORD dwWait)
{
	return WaitForSingleObject(m_hRefreshEvent,dwWait);
}

void CPanelImageLibrary::MoveToIndex(INT index)
{
	m_boxLibrary.MoveToIndex(index,0,FALSE);
}

const wchar_t* CPanelImageLibrary::GetIndexValue(INT index, BOOL bMoveIndex)
{
	if(bMoveIndex)
	{
		if(m_boxLibrary.MoveToIndex(index,0,FALSE))
			return m_boxLibrary.GetCurFocusItemValue();
		else
			return NULL;
	}
	else
	{
		return m_boxLibrary.GetIndexValue(index);
	}
}

const wchar_t* CPanelImageLibrary::GetCurValue()
{
	return m_boxLibrary.GetCurFocusItemValue();
}

int CPanelImageLibrary::GetCurIndex()
{
	return m_boxLibrary.GetCurIndex();
}
const wchar_t* CPanelImageLibrary::GetNextValue()
{
	INT track_count=GetTrackCount();

	if(track_count<=0)
	{
		RETAILMSG(DEBUG_MEDIA,(_T("*****CPanelImageLibrary GetNextValue no item....\r\n")));
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			//current folder has no file:
			//get from library:
			track_count=m_boxLibrary.GetCount();
			if(track_count>0)
			{
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
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			m_iExplorerPlayIndex++;
			if(m_iExplorerPlayIndex>=track_count)
				m_iExplorerPlayIndex=0;

			if(m_boxExplorer.MoveToIndex(m_iExplorerFolderCount+m_iExplorerPlayIndex))
			{
				return m_boxExplorer.GetCurFocusItemValue();
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			m_boxLibrary.MoveToNext(TRUE,FALSE);
			return m_boxLibrary.GetCurFocusItemValue();
		}
	}
}
const wchar_t* CPanelImageLibrary::GetPreviousValue()
{
	INT track_count=GetTrackCount();

	if(track_count<=0)
	{
		RETAILMSG(DEBUG_MEDIA,(_T("*****CPanelImageLibrary GetNextValue no item....\r\n")));
		if(m_type==MEDIA_LIBRARY_EXPLORER)
		{
			//current folder has no file:
			//get from library:
			track_count=m_boxLibrary.GetCount();
			if(track_count>0)
			{
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

			if(m_boxExplorer.MoveToIndex(m_iExplorerFolderCount+m_iExplorerPlayIndex))
			{
				return m_boxExplorer.GetCurFocusItemValue();
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			m_boxLibrary.MoveToLast(TRUE,FALSE);
			return m_boxLibrary.GetCurFocusItemValue();
		}
	}
}

void CPanelImageLibrary::EnableRefresh()
{
	m_bNeedRefreshLib = TRUE;

}
void CPanelImageLibrary::SelectDevice(UINT idDevice)
{
	if(m_idCurDeviceID == idDevice)
		return;

	RETAILMSG(1,(_T("MSG:[CPanelImageLibrary::SelectDevice]: %d\r\n"),idDevice));

	//reset playfile
	m_strCurPlayFile = NULL;

	m_idCurDeviceID = idDevice;

	SendImageCommand(IMAGE_CMD_SELECT_DEVICE,m_idCurDeviceID);

	//move to current device:
	m_boxDevice.MoveToIndex(m_boxDevice.SearchID(m_idCurDeviceID));

	RefreshLibrary();

	if( m_idCurDeviceID == MEDIA_TYPE_USB1 ) // mass storages
	{
		WCHAR str[MAX_PATH];
		swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_USB1);
		m_strCurRoot=str;
	}
	else if( m_idCurDeviceID == MEDIA_TYPE_USB2 ) // mass storages
	{
		WCHAR str[MAX_PATH];
		swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_USB2);
		m_strCurRoot=str;
	}
	else if( m_idCurDeviceID == MEDIA_TYPE_SD1 ) // mass storages
	{
		WCHAR str[MAX_PATH];
		swprintf_s(str,MAX_PATH,_T("\\%s\\"),DEV_NAME_MEDIACARD);
		m_strCurRoot=str;
	}
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
}
void CPanelImageLibrary::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam>MEDIA_TYPE_NUMBER-1)
		return;

	if(m_idValidSrcList[wParam] == lParam)
		return;

	m_idValidSrcList[wParam] = lParam;

	m_boxDevice.ClearList();

	CListBoxItemGL *pItem;

	if(m_idValidSrcList[MEDIA_TYPE_SD1])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus,14,0);
		//if (m_idCurDeviceID == MEDIA_TYPE_SD1)
		//{
		//	pItem->SetIcon2(&m_iconArrow,NULL,9,0);
		//}
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD));
		pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_SD1);
	}
	if(m_idValidSrcList[MEDIA_TYPE_USB1])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus,14,0);
		//if (m_idCurDeviceID == MEDIA_TYPE_USB1)
		//{
		//	pItem->SetIcon2(&m_iconArrow,NULL,9,0);
		//}
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB));
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_USB1);
	}
	if(m_idValidSrcList[MEDIA_TYPE_USB2])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus,14,0);
		//if (m_idCurDeviceID == MEDIA_TYPE_USB2)
		//{
		//	pItem->SetIcon2(&m_iconArrow,NULL,9,0);
		//}
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB));
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus);
#endif
		m_boxDevice.AddItem(pItem,MEDIA_TYPE_USB2);
	}


	if(lParam == 0)
	{
		if(m_idCurDeviceID == wParam)
			m_idCurDeviceID=0;
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

	//RETAILMSG(DEBUG_IMAGE,(_T("CPanelMediaLibrary OnAvailableSourceChanged id:%d:%d\r\n"),wParam,lParam));

	//auto select device:
	if(m_idCurDeviceID == 0)
	{
		if(m_boxDevice.GetCount()>0)
			SelectDevice(m_boxDevice.GetCurFocusItemID());
	}
	else
	{
		//move to current device:
		m_boxDevice.MoveToIndex(m_boxDevice.SearchID(m_idCurDeviceID));
	}
}
void CPanelImageLibrary::SetBrowseType(MEDIA_LIBRARY_TYPE type)
{
	RETAILMSG(DEBUG_IMAGE,(_T("CPanelImageLibrary SetBrowseType id:%d\r\n"),type));

	m_type=type;

	m_dwSetShowTypeTick = GetTickCount();

	m_boxLibrary.Show(type==MEDIA_LIBRARY_TRACKS);
	m_boxExplorer.Show(type==MEDIA_LIBRARY_EXPLORER);
	m_boxSearched.Show(type==MEDIA_LIBRARY_SEARCH_RESULT);

	if(m_type==MEDIA_LIBRARY_TRACKS)
	{
#if LIB_BROWSE_BTN_DISPLAY_REVERSE
		m_btnBrowseMode.SetIcon(&m_iconBrowseFolders);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
#else
		m_btnBrowseMode.SetIcon(&m_iconBrowseFiles);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
#endif
	}
	else if(m_type==MEDIA_LIBRARY_SEARCH_RESULT)
	{
		m_btnBrowseMode.SetIcon(&m_iconBrowseFiles);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
	}
	else
	{
#if LIB_BROWSE_BTN_DISPLAY_REVERSE
		m_btnBrowseMode.SetIcon(&m_iconBrowseFiles);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_TRACK));
#else
		m_btnBrowseMode.SetIcon(&m_iconBrowseFolders);
		m_btnBrowseMode.SetCaption(GetResourceString(IDS_MEDIA_BROWSE_FOLDER));
#endif	
	}

// 	m_btnCatalogAudio.Enable(bMassStorage);
// 	m_btnCatalogVideo.Enable(bMassStorage && m_bEnableVideo);
// 	m_btnCatalogImage.Enable(bMassStorage);
// 	// 	m_btnCatalogArtist.Enable(bMassStorage);
// 	m_btnSearch.Enable(bMassStorage);
// 	m_btnBrowseMode.Enable(bMassStorage);

	// 	m_btnExplorer.SetStatus(type==MEDIA_LIBRARY_EXPLORER?BS_FOCUS:BS_UP);
	// 	m_btnTracks.SetStatus(type==MEDIA_LIBRARY_TRACKS?BS_FOCUS:BS_UP);

	if(type==MEDIA_LIBRARY_EXPLORER )
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
int CPanelImageLibrary::CreateSearchResultList(const wchar_t *SearchString,CListBoxGL *plistboxIn,CListBoxGL *plistboxOut)
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
