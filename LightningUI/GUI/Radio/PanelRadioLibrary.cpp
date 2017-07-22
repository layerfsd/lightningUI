#include "PanelRadioLibrary.h"
#include "../../resource.h"

//0:Europe:				fm 87.5-108.0MHz, am 522-1620KHz
//1:USA:					fm 87.5-107.9MHz, am 530-1710KHz
//2:EasternEurope:	fm 65.00-74.0MHz, am 522-1620KHz
//3:Japan:					fm 76.0-90.0MHz, am 522-1629KHz
//4:Korea:					fm 87.5-108.0MHz, am 531-1719KHz
//5:Row:					fm 87.5-108.0MHz, am 522-1620KHz

extern const wchar_t* g_strInstallPath;

CPanelRadioLibrary::CPanelRadioLibrary(void)
{
	m_type = RADIO_LIBRARY_NULL;

	m_RegionId = RADIO_REGION_Europe;
	m_BandId = RADIO_BAND_ID_INVALID;
	m_RegionIdtoSet = RADIO_REGION_Europe;

	m_iCurStoreIndex = 0;
	m_idCurBOXMENU	 = 0;
	m_dwCurFreq = 0;
	m_bFavListChanged = FALSE;

	for(int i=0;i<RADIO_REGION_Max;i++)
	{
		m_bAMListChanged[i]=0;
		m_bFMListChanged[i]=0;

		memset(&m_infoAM,0,sizeof(RadioStationInfo));
		memset(&m_infoFM,0,sizeof(RadioStationInfo));
	}

// 	m_bRDSOn = FALSE;
// 	m_bAFOn = FALSE;
// 	m_bTAOn = FALSE;
	m_bSearching = FALSE;
	m_bAFSearching = FALSE;
	m_bResetFreq = FALSE;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_iconoffset = 64;
#endif
}
CPanelRadioLibrary::~CPanelRadioLibrary(void)
{

}

BOOL CPanelRadioLibrary::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelRadioLibrary::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

	if(iParam)
	{
		SetShowType((RADIO_LIBRARY_TYPE)iParam);
	}
	else
	{
		SetShowType(m_type);
	}
	/*
	switch (m_type)
	{
	case RADIO_LIBRARY_FM:
		if(m_boxFM.GetCount() == 0 && m_boxFavorite.GetCount() != 0)
		{
			SetShowType(RADIO_LIBRARY_FAVORITE);
		}
		else
		{
			if(!m_boxFM.MoveToIndex(m_boxFM.SearchID(m_dwCurFreq)))
			{
				m_boxFM.KillFocus();
			}
		}
		break;
	case RADIO_LIBRARY_AM:
		if(m_boxAM.GetCount() == 0 && m_boxFavorite.GetCount() != 0)
		{
			SetShowType(RADIO_LIBRARY_FAVORITE);
		}
		else
		{
			if(!m_boxAM.MoveToIndex(m_boxAM.SearchID(m_dwCurFreq)))
			{
				m_boxAM.KillFocus();
			}
		}
		break;
	case RADIO_LIBRARY_FAVORITE:
		if(!m_boxFavorite.MoveToIndex(m_boxFavorite.SearchID(m_dwCurFreq)))
		{
			m_boxFavorite.KillFocus();
		}
		break;
	}
	*/
}

void CPanelRadioLibrary::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);
}
void CPanelRadioLibrary::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_RIGHT);
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
void CPanelRadioLibrary::Render()
{
	if(!BeginRender())return;

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

	m_boxLists.SetAlpha(m_fCurAlpha);
	m_boxLists.Render();


	m_btnNowPlaying.SetAlpha(m_fCurAlpha);
	m_btnNowPlaying.Render();

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	if(m_type==RADIO_LIBRARY_FM)
	{
		m_boxFM.SetAlpha(m_fCurAlpha);
		m_boxFM.Render();

		m_tagNote.Show(m_boxFM.GetCount()==0);
		m_tagNote.SetAlpha(m_fCurAlpha);
		m_tagNote.Render();
	}
	else if(m_type==RADIO_LIBRARY_AM)
	{
		m_boxAM.SetAlpha(m_fCurAlpha);
		m_boxAM.Render();

		m_tagNote.Show(m_boxAM.GetCount()==0);
		m_tagNote.SetAlpha(m_fCurAlpha);
		m_tagNote.Render();
	}
	else if(m_type==RADIO_LIBRARY_FAVORITE)
	{
		m_boxFavorite.SetAlpha(m_fCurAlpha);
		m_boxFavorite.Render();

		m_tagNote.Show(m_boxFavorite.GetCount()==0);
		m_tagNote.SetAlpha(m_fCurAlpha);
		m_tagNote.Render();
	}

	EndRender();
}

BOOL CPanelRadioLibrary::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);

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


// 	COLORGL cr_gray={177.0f/256.0f,131.0f/256.0f,107.0f/256.0f,1.0};
	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_highlight={1.0f,1.0f,1.0f,1.0};
#else
	COLORGL cr_highlight={255.0f/255.0f,63.0f/255.0f,63.0f/255.0f,1.0};
#endif

	COLORGL crb={0.9,0.9,0.9,1.0};
	SIZE siInfo={600,40};
	int cx=LIST_BOX_ITEM_WIDTH;
	int cy=LIST_BOX_ITEM_HEIGHT;
	m_tagNote.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagNote.SetSize(&siInfo);
	m_tagNote.SetCaption(GetResourceString(IDS_NO_LISTITEM));
	m_tagNote.LinkTextGL(pTextGL);
	m_tagNote.SetPos(cx/2,/*g_iClientHeight/2-100*/0);
	m_tagNote.SetCaptionHeight(32);
	m_tagNote.EnableDrawBack(FALSE);
	m_tagNote.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagNote.SetCaptionColor(&crb);


	int ispace=0;

	LISTBOXSTRUCTURE box;

	memset(&box,0,sizeof(box));

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_iconFM.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1409));
	m_iconFMFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1410));

	m_iconAM.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1405));
	m_iconAMFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1406));

	m_iconFavorite.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1403));
	m_iconFavoriteFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1404));

	m_iconFavoriteList.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1413));
	m_iconFavoriteListFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1407));

	m_iconFreq.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1411));
	m_iconFreqFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1412));

	m_iconDelete.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1401));
	m_iconDelete.SetShowScale(1.2f);
	m_iconDeleteFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1402));
	m_iconDeleteFocus.SetShowScale(1.2f);


	SIZE sibtn={cx,cy};

	int count=(g_iClientHeight-cy*2)/cy;
	if((g_iClientHeight-cy*2)%cy > (cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight-cy*2)/count/2*2;
	box.siClient.cx = cx;	
	box.siClient.cy = (box.iSpacing * count);
	box.siBorder.cx = cx;
	box.siBorder.cy = g_iClientHeight-cy*2;
	box.idTexIndexBase=TEXID_289; //068
	box.idTexIndexFocus=TEXID_242;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_285;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxLists.Initialize(IDC_RADIO_BOX_MENU,this,&box,pSkinManager);
	m_boxLists.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_boxLists.SetPos(-g_iClientWidth/2+cx/2,0);
	m_boxLists.LinkTextGL(pTextGL);
	m_boxLists.Show(TRUE,FALSE);
	m_boxLists.SetAlpha(1.0f);
	m_boxLists.SetCaptioniHeight(18);
	m_boxLists.SetCaptionColor(&cr_gray,&cr_w);

	m_iconArrowOn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_083));
	m_iconArrowOn.SetShowScale(0.8f);
	m_iconArrowOff.Initialize(0,NULL, m_pSkinManager->GetTexture(TEXID_068));
	m_iconArrowOff.SetShowScale(0.8f);

	//m_iconAMArrowOn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_083));
	//m_iconAMArrowOn.SetShowScale(0.8f);
	//m_iconAMArrowOff.Initialize(0,NULL, m_pSkinManager->GetTexture(TEXID_068));
	//m_iconAMArrowOff.SetShowScale(0.8f);

	//m_iconFavArrowOn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_083));
	//m_iconFavArrowOn.SetShowScale(0.8f);
	//m_iconFavArrowOff.Initialize(0,NULL, m_pSkinManager->GetTexture(TEXID_068));
	//m_iconFavArrowOff.SetShowScale(0.8f);

	int iconMarginR = -14;
	CListBoxItemGL *pItem;
	pItem = new CListBoxItemGL(_T("FM"), NULL, NULL, 34, 0);
	pItem->SetIcon(&m_iconFM, &m_iconFMFocus, 14, 0);
	pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginR, 0);
	m_boxLists.AddItem(pItem,FM_SELECTED);
	pItem = new CListBoxItemGL(_T("AM"), NULL, NULL, 34, 0);
	pItem->SetIcon(&m_iconAM, &m_iconAMFocus, 14, 0);
	pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginR, 0);
	m_boxLists.AddItem(pItem,AM_SELECTED);
	pItem = new CListBoxItemGL(GetResourceString(IDS_FAVORITE), NULL, NULL, 34, 0);
	pItem->SetIcon(&m_iconFavoriteList, &m_iconFavoriteListFocus, 14, 0);
	pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginR, 0);
	m_boxLists.AddItem(pItem,FAV_SELECTED);


	memset(&box,0,sizeof(box));
	//find param:
	box.iSpacing = g_iClientHeight/6;
	box.siClient.cx = g_iClientWidth-cx;
	box.siClient.cy = g_iClientHeight;	
	box.siBorder.cx = g_iClientWidth-cx;	
	box.siBorder.cy = g_iClientHeight;
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
	box.bNoBorder=TRUE;	
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=FALSE;
	box.pxSlider=-8;
	box.fIndexBaseOffset = 0;

	//FM:
	m_boxFM.Initialize(IDC_RADIO_BOX_FM,this,&box,pSkinManager);
	m_boxFM.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_boxFM.SetPos(cx/2, 0);
	m_boxFM.LinkTextGL(pTextGL);
	m_boxFM.Show(TRUE,FALSE);
	m_boxFM.SetAlpha(1.0f);
	m_boxFM.SetCaptioniHeight(22);
	m_boxFM.SetCaptionColor(&cr_gray,&cr_highlight);

	//AM:
	m_boxAM.Initialize(IDC_RADIO_BOX_AM,this,&box,pSkinManager);
	m_boxAM.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_boxAM.SetPos(cx/2, 0);
	m_boxAM.LinkTextGL(pTextGL);
	m_boxAM.Show(TRUE,FALSE);
	m_boxAM.SetAlpha(1.0f);
	m_boxAM.SetCaptioniHeight(22);
	m_boxAM.SetCaptionColor(&cr_gray,&cr_highlight);

	//Fav:
	m_boxFavorite.Initialize(IDC_RADIO_BOX_FAVORITE,this,&box,pSkinManager);
	m_boxFavorite.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_boxFavorite.SetPos(cx/2, 0);
	m_boxFavorite.LinkTextGL(pTextGL);
	m_boxFavorite.Show(TRUE,FALSE);
	m_boxFavorite.SetAlpha(1.0f);
	m_boxFavorite.SetCaptioniHeight(22);
	m_boxFavorite.SetCaptionColor(&cr_gray,&cr_highlight);
	
	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_204));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));
	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_280));
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_279));

	m_btnHome.Initialize(IDC_RADIO_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_288),NULL,
		m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2+ispace/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.SetIconPos(-cx/2+35,0);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,68);
	m_btnHome.SetCaptionColor(&cr_w,&cr_gray);
	m_btnHome.SetCaptionHeight(20);


	m_btnNowPlaying.Initialize(IDC_RADIO_BTN_NOWPLAYING,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
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

	m_iconFM.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_437));
	m_iconFM.SetShowScale(0.8f);
	m_iconAM.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_439));
	m_iconAM.SetShowScale(0.8f);
	m_iconFavorite.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_435));
	m_iconFavorite.SetShowScale(0.8f);

	m_iconFreq.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_444));
	m_iconFreq.SetShowScale(0.8f);
	m_iconDelete.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_446));
	m_iconDelete.SetShowScale(0.8f);

	m_iconFMFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_438));
	m_iconFMFocus.SetShowScale(0.8f);
	m_iconAMFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_440));
	m_iconAMFocus.SetShowScale(0.8f);
	m_iconFavoriteFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_436));
	m_iconFavoriteFocus.SetShowScale(0.8f);
	m_iconFreqFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_445));
	m_iconFreqFocus.SetShowScale(0.8f);
	m_iconDeleteFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_447));
	m_iconDeleteFocus.SetShowScale(0.8f);

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

	box.idTexIndexBase=TEXID_451;
	box.idTexIndexFocus=TEXID_068;//TEXID_066;//
	box.idTexBase=TEXID_451;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;//FALSE;//
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxLists.Initialize(IDC_RADIO_BOX_MENU,this,&box,pSkinManager);
	m_boxLists.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_boxLists.SetPos(-g_iClientWidth/2+cx/2,0);
	m_boxLists.LinkTextGL(pTextGL);
	m_boxLists.Show(TRUE,FALSE);
	m_boxLists.SetAlpha(1.0f);

	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_RUSSIAN:
	case LANGUAGE_GREEK:
		m_boxLists.SetCaptioniHeight(20);
		break;
	default:
		m_boxLists.SetCaptioniHeight(24);
		break;
	}

	m_boxLists.SetCaptionColor(&cr_gray,&cr_w);

	CListBoxItemGL *pItem;
	pItem=new CListBoxItemGL(_T("FM")/*GetResourceString(IDS_DEVICE_SD)*/);
	pItem->SetIcon(&m_iconFM,&m_iconFMFocus);
	m_boxLists.AddItem(pItem);
	pItem=new CListBoxItemGL(_T("AM")/*GetResourceString(IDS_DEVICE_BT)*/);
	pItem->SetIcon(&m_iconAM,&m_iconAMFocus);
	m_boxLists.AddItem(pItem);
	pItem=new CListBoxItemGL(GetResourceString(IDS_FAVORITE));
	pItem->SetIcon(&m_iconFavorite,&m_iconFavoriteFocus);
	//pItem->SetIcon2(&m_iconLibTracks);
	m_boxLists.AddItem(pItem);

	pItem=new CListBoxItemGL(_T(" ")/*GetResourceString(IDS_DEVICE_USB)*/);
	//pItem->SetIcon(&m_iconFavorite,&m_iconFavoriteFocus);
	//pItem->SetIcon2(&m_iconLibTracks);
	pItem->EnableClick(FALSE);
	m_boxLists.AddItem(pItem);

	//list:
	memset(&box,0,sizeof(box));

	//find param:
	count=(g_iClientHeight)/cy;	//100
	if((g_iClientHeight)%cy > (cy/2))
		count+=1;

	box.iSpacing=(g_iClientHeight)/count/2*2;

	box.siClient.cx=g_iClientWidth-cx;
	box.siClient.cy=(box.iSpacing * count);
	box.siBorder.cx=g_iClientWidth-cx;
	box.siBorder.cy=g_iClientHeight;//(box.iSpacing * count);


	box.idTexIndexBase=TEXID_453;
	box.idTexIndexFocus=TEXID_453;
	box.idTexBase=TEXID_066;
	box.idTexSliThumb=TEXID_063;
	box.bNoSlider=FALSE;
	box.bNoBorder=TRUE;//FALSE;//
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;
	box.bQuickSearchBoard=FALSE;//TRUE;
	box.pxSlider=-20;
	box.fIndexBaseOffset=60;    //74;

	//FM:
	m_boxFM.Initialize(IDC_RADIO_BOX_FM,this,&box,pSkinManager);
	m_boxFM.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_boxFM.SetMarginHor(60);
	m_boxFM.SetPos(cx/2,0);
	m_boxFM.LinkTextGL(pTextGL);
	m_boxFM.Show(TRUE,FALSE);
	m_boxFM.SetAlpha(1.0f);
	m_boxFM.SetCaptioniHeight(26,19,14);
	m_boxFM.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxFM.SetSortType(ITEM_SORT_BY_ID);


	//AM
	m_boxAM.Initialize(IDC_RADIO_BOX_AM,this,&box,pSkinManager);
	m_boxAM.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_boxAM.SetMarginHor(60);
	m_boxAM.SetPos(cx/2,0);
	m_boxAM.LinkTextGL(pTextGL);
	m_boxAM.Show(FALSE,FALSE);
	m_boxAM.SetAlpha(1.0f);
	m_boxAM.SetCaptioniHeight(26,19,14);
	m_boxAM.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxAM.SetSortType(ITEM_SORT_BY_ID);

	//Fav:
	m_boxFavorite.Initialize(IDC_RADIO_BOX_FAVORITE,this,&box,pSkinManager);
	m_boxFavorite.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_boxFavorite.SetMarginHor(60);
	m_boxFavorite.SetPos(cx/2,0);
	m_boxFavorite.LinkTextGL(pTextGL);
	m_boxFavorite.Show(FALSE,FALSE);
	m_boxFavorite.SetAlpha(1.0f);
	m_boxFavorite.SetCaptioniHeight(26,19,14);
	m_boxFavorite.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxFavorite.SetSortType(ITEM_SORT_BY_ID);

// 	for (int i=0;i<1;i++)
// 	{
// 		pItem=new CListBoxItemGL(_T("98.00MHz"),NULL,_T("FM"));
// 		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
// 		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
// 		m_boxFavorite.AddItem(pItem);
// 	}


// 	//setshowtype
// 	SetShowType(RADIO_LIBRARY_FM);

// 	//test
// 	AddFM(87800000,_T("Test Station"));
// 	AddFM(98800000,_T("Test Station"));
//
// 	AddAM(531000,_T("Test Station"));
// 	AddAM(1620000,_T("Test Station"));

	SIZE sibtn={cx,cy+ispace};

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_441));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_442));
	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_433));
    m_iconNowPlayingUp.SetShowScale(0.8f);
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_434));
    m_iconNowPlayingDown.SetShowScale(0.8f);

	m_btnHome.Initialize(IDC_RADIO_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_451),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2+ispace/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_w);
	m_btnHome.SetCaptionHeight(22);
	m_btnHome.SetIconPos(-cx/2+31,0);
	m_btnHome.EnableClickAni(FALSE);

	m_btnNowPlaying.Initialize(IDC_RADIO_BTN_NOWPLAYING,this,m_pSkinManager->GetTexture(TEXID_451),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPanelRadioLibrary::OnNotify,this);
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
	m_btnNowPlaying.EnableClickAni(FALSE);

#endif //if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	//

	GetFavData();
	GetRegionPresetData(RADIO_REGION_Europe);
	GetRegionPresetData(RADIO_REGION_EasternEurope);


	return TRUE;
}


void CALLBACK CPanelRadioLibrary::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelRadioLibrary *panel=(CPanelRadioLibrary*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelRadioLibrary::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(1, (TEXT("CPanelRadioMain::HandleControlMSG idControl[%d], wMsg[%d], wPara[%d].\r\n"),idControl, wMsg, wPara));
	switch (idControl)
	{
	case IDC_RADIO_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BTN_NOWPLAYING:
		{
			SwitchPage(UI_CLASS_RADIO,PAGE_RADIO_MAIN,PAGE_SWITCH_ANI_RIGHT);
			PlaySoundEffect(0);
		}
		break;
	case IDC_RADIO_BOX_MENU:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
				{

				}
				break;
			case NOTIFY_SHORT_CLICK:
				{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
					
					//switch type
					if(index == 0)//fm
					{	
						SetShowType(RADIO_LIBRARY_FM);
						SendRadioCommand(RADIO_CMD_SELECT_BAND,0);
						PlaySoundEffect(0);
					}
					else if(index == 1)//am
					{
						SetShowType(RADIO_LIBRARY_AM);
						SendRadioCommand(RADIO_CMD_SELECT_BAND,1);
						PlaySoundEffect(0);
					}
					else if(index == 2)//fav
					{
						SetShowType(RADIO_LIBRARY_FAVORITE);
						PlaySoundEffect(0);
					}

#else
					//switch type
					if(index == 0)//fm
					{
						SetShowType(RADIO_LIBRARY_FM);
						SendRadioCommand(RADIO_CMD_SELECT_BAND,0);
						PlaySoundEffect(0);
					}
					else if(index == 1)//am
					{
						SetShowType(RADIO_LIBRARY_AM);
						SendRadioCommand(RADIO_CMD_SELECT_BAND,1);
						PlaySoundEffect(0);
					}
					else if(index == 2)//fav
					{
						SetShowType(RADIO_LIBRARY_FAVORITE);
						PlaySoundEffect(0);
					}
#endif	//(CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				}
				break;
			}
		}
		break;
	case IDC_RADIO_BOX_FM:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;

			RETAILMSG(DEBUG_RADIO,(_T("###idControl:%d, wMsg:%d, index:%d, id_item:%d\r\n"),idControl,wMsg,index,id_item));
			if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				if(m_pCommander && id_item!=-1)
				{
					if(m_BandId!=RADIO_BAND_ID_FM)
					{
						SendRadioCommand(RADIO_CMD_SELECT_BAND,0);
					}

					SendRadioCommand(RADIO_CMD_SET_FREQ,id_item);

					PlaySoundEffect(0);
				}
			}
			else if(wMsg == NOTIFY_CONTROL)
			{
// 				if(IsFavorite(id_item))
// 				{
// 					DeleteFavorite(id_item);
// 					m_boxFM.SetIndexIcon2(id_item,&m_iconFavorite,&m_iconFavoriteFocus);
// 				}
// 				else
// 				{
// 					AddFavorite(id_item,RADIO_BAND_ID_FM);
// 					m_boxFM.SetIndexIcon2(id_item,&m_iconFavoriteFocus,&m_iconFavorite);
// 				}
				SendRadioCommand(RADIO_CMD_TOGGLE_FAVORITE,id_item);
				PlaySoundEffect(0);
			}
		}
		break;
	case IDC_RADIO_BOX_AM:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;

			RETAILMSG(DEBUG_RADIO,(_T("###idControl:%d, wMsg:%d, index:%d, id_item:%d\r\n"),idControl,wMsg,index,id_item));
			if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				if(m_pCommander && id_item!=-1)
				{
					if(m_BandId!=RADIO_BAND_ID_AM)
					{
						SendRadioCommand(RADIO_CMD_SELECT_BAND,1);
					}

					SendRadioCommand(RADIO_CMD_SET_FREQ,id_item);

					PlaySoundEffect(0);
				}
			}
			else if(wMsg == NOTIFY_CONTROL)
			{
// 				if(IsFavorite(id_item))
// 				{
// 					DeleteFavorite(id_item);
// 					m_boxAM.SetIndexIcon2(id_item,&m_iconFavorite,&m_iconFavoriteFocus);
// 				}
// 				else
// 				{
// 					AddFavorite(id_item,RADIO_BAND_ID_AM);
// 					m_boxAM.SetIndexIcon2(id_item,&m_iconFavoriteFocus,&m_iconFavorite);
// 				}
				SendRadioCommand(RADIO_CMD_TOGGLE_FAVORITE,id_item);

				PlaySoundEffect(0);
			}
		}
		break;
	case IDC_RADIO_BOX_FAVORITE:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;

			RETAILMSG(DEBUG_RADIO,(_T("###idControl:%d, wMsg:%d, index:%d, id_item:%d\r\n"),idControl,wMsg,index,id_item));

			if(wMsg == NOTIFY_CONTROL)
			{
				RETAILMSG(DEBUG_RADIO,(_T("###delete:%d\r\n"),index));

				//DeleteFavorite(id_item);
				SendRadioCommand(RADIO_CMD_DELETE_FAVORITE,id_item);

				PlaySoundEffect(0);
			}
			else if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				if (lstrcmpi(m_boxFavorite.GetCurFocusItemSubName(), _T("AM")) == 0)//AM
				{
					RadioRegionId region=RadioRegionId(m_boxFavorite.GetCurFocusItemUserParam());
					if(m_RegionId!=region)
					{
						SendRadioCommand(RADIO_CMD_SET_REGION,region);
					}

					if(m_BandId!=RADIO_BAND_ID_AM)
					{
						SendRadioCommand(RADIO_CMD_SELECT_BAND,1);
					}

					SendRadioCommand(RADIO_CMD_SET_FREQ,id_item);
					PlaySoundEffect(0);
				}
				else if (lstrcmpi(m_boxFavorite.GetCurFocusItemSubName(), _T("FM")) == 0)//FM
				{
					RadioRegionId region=RadioRegionId(m_boxFavorite.GetCurFocusItemUserParam());
					if(m_RegionId!=region)
					{
						SendRadioCommand(RADIO_CMD_SET_REGION,region);
					}

					if(m_BandId!=RADIO_BAND_ID_FM)
					{
						SendRadioCommand(RADIO_CMD_SELECT_BAND,0);
					}

					SendRadioCommand(RADIO_CMD_SET_FREQ,id_item);

					PlaySoundEffect(0);
				}
			}
		}
		break;
	}
}
void CPanelRadioLibrary::SetShowType(RADIO_LIBRARY_TYPE type)
{
// 	if(m_type==type)
// 		return;

	m_type=type;

	RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioLibrary::SetShowType :%d ###\r\n"),type));

	m_boxFM.Show(m_type==RADIO_LIBRARY_FM);
	m_boxAM.Show(m_type==RADIO_LIBRARY_AM);
	m_boxFavorite.Show(m_type==RADIO_LIBRARY_FAVORITE);

	if(type==RADIO_LIBRARY_FM)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		ArrowChange(FM_SELECTED);
#endif

		m_boxLists.MoveToIndex(0);
		if(!m_boxFM.MoveToIndex(m_boxFM.SearchID(m_dwCurFreq)))
		{
			m_boxFM.KillFocus();
		}
	}
	else if(type==RADIO_LIBRARY_AM)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		ArrowChange(AM_SELECTED);
#endif
		m_boxLists.MoveToIndex(1);
		if(!m_boxAM.MoveToIndex(m_boxAM.SearchID(m_dwCurFreq)))
		{
			m_boxAM.KillFocus();
		}
	}
	else if(type==RADIO_LIBRARY_FAVORITE)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		ArrowChange(FAV_SELECTED);
#endif
		m_boxLists.MoveToIndex(2);
		if(!m_boxFavorite.MoveToIndex(m_boxFavorite.SearchID(m_dwCurFreq)))
		{
			m_boxFavorite.KillFocus();
		}
	}
}

void CPanelRadioLibrary::ClearList(RadioBandId band)
{
	RETAILMSG(DEBUG_RADIO,(_T("***********CPanelRadioLibrary ClearList, band:%d*************\r\n"),band));

	int index = m_RegionId;

	if(index >= RADIO_REGION_Max)
		return;

	if(band==RADIO_BAND_ID_AM)
	{
		m_boxAM.ClearList();

		m_listAM[index].Head();
		while (!m_listAM[index].End())
		{
			delete m_listAM[index].Get();
			m_listAM[index].Next();
		}
		m_listAM[index].Clear();
	}
	else if(band==RADIO_BAND_ID_FM)
	{
		m_boxFM.ClearList();

		m_listFM[index].Head();
		while (!m_listFM[index].End())
		{
			delete m_listFM[index].Get();
			m_listFM[index].Next();
		}
		m_listFM[index].Clear();
	}
	else
	{
		m_boxAM.ClearList();
		m_boxFM.ClearList();

		m_listFM[index].Head();
		while (!m_listFM[index].End())
		{
			delete m_listFM[index].Get();
			m_listFM[index].Next();
		}
		m_listFM[index].Clear();

		m_listAM[index].Head();
		while (!m_listAM[index].End())
		{
			delete m_listAM[index].Get();
			m_listAM[index].Next();
		}
		m_listAM[index].Clear();
	}
}
void CPanelRadioLibrary::SetRadioRegion(RadioRegionId idRegion,BOOL bResetFreq)
{
	m_RegionIdtoSet = idRegion;
	m_bResetFreq = bResetFreq;
}
BOOL CPanelRadioLibrary::OnUpdateRadioPlayInfo()
{
	RETAILMSG(DEBUG_RADIO,(_T("#### CPanelRadioLibrary::OnUpdateRadioPlayInfo\r\n")));

	switch (m_type)
	{
	case RADIO_LIBRARY_FM:
		if(!m_boxFM.MoveToIndex(m_boxFM.SearchID(m_dwCurFreq)))
		{
			m_boxFM.KillFocus();
		}
		break;
	case RADIO_LIBRARY_AM:
		if(!m_boxAM.MoveToIndex(m_boxAM.SearchID(m_dwCurFreq)))
		{
			m_boxAM.KillFocus();
		}
		break;
	case RADIO_LIBRARY_FAVORITE:
		if(!m_boxFavorite.MoveToIndex(m_boxFavorite.SearchID(m_dwCurFreq)))
		{
			m_boxFavorite.KillFocus();
		}
		break;
	}

	return TRUE;
}
void CPanelRadioLibrary::AddFM(DWORD dwFreq)
{
	RETAILMSG(DEBUG_RADIO,(_T("CPanelRadioLibrary AddFM:%d\r\n"),dwFreq));

	if(m_RegionId == m_RegionIdtoSet)
	{
		CListBoxItemGL *pItem;
		wchar_t str[32];

		if(dwFreq==0 || m_boxFM.SearchID(dwFreq) != -1)
		{
			return;
		}

		swprintf_s(str,32,_T("%d.%02d %s"),dwFreq/1000000,dwFreq/10000-dwFreq/1000000*100,GetResourceString(IDS_MHZ));
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(str,NULL,NULL,10,0);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus,3,0);
		pItem->SetUserParam(m_RegionId);
		if (IsFavorite(dwFreq))
		{
			pItem->SetIcon2(&m_iconFavoriteFocus,&m_iconFavorite,m_iconoffset,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconFavorite,&m_iconFavoriteFocus,m_iconoffset,0);
		}
#else
		wchar_t str_sub[32];
		swprintf_s(str_sub,32,_T("FM (%d)"),m_boxFM.GetCount()+1);
		pItem=new CListBoxItemGL(str,NULL,str_sub);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
		pItem->SetUserParam(m_RegionId);
		if (IsFavorite(dwFreq))
		{
			pItem->SetIcon2(&m_iconFavoriteFocus,&m_iconFavorite);
		}
		else
		{
			pItem->SetIcon2(&m_iconFavorite,&m_iconFavoriteFocus);
		}
#endif
		//更新FM显示列表
		m_boxFM.AddItemByOrder(pItem,dwFreq);
	}

	//添加FM显示列表m_listFM，关机的时候会保存这个列表，开机的时候回读取
	int index=m_RegionIdtoSet;
	if(index < RADIO_REGION_Max && m_listFM[index].SearchID(dwFreq) == FALSE)
	{
		RadioStationInfo *info=new RadioStationInfo();
		memset(info,0,sizeof(RadioStationInfo));
		info->dwBand=RADIO_BAND_ID_FM;
		info->dwRegion=m_RegionIdtoSet;
		info->dwFreq=dwFreq;
		m_listFM[index].Add(info,dwFreq);
		m_bFMListChanged[index]=TRUE;
	}
}
void CPanelRadioLibrary::AddAM(DWORD dwFreq)
{
	RETAILMSG(DEBUG_RADIO,(_T("CPanelRadioLibrary AddAM:%d\r\n"),dwFreq));

	if(m_RegionId == m_RegionIdtoSet)
	{
		CListBoxItemGL *pItem;
		wchar_t str[32];

		if(dwFreq==0 || m_boxAM.SearchID(dwFreq) != -1)
		{
			return;
		}

		swprintf_s(str,32,_T("%d%02d %s"),dwFreq/100000,dwFreq/1000-dwFreq/100000*100,GetResourceString(IDS_KHZ));

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(str,NULL,NULL,10,0);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus,3,0);
		pItem->SetUserParam(m_RegionId);
		if (IsFavorite(dwFreq))
		{
			pItem->SetIcon2(&m_iconFavoriteFocus,&m_iconFavorite,m_iconoffset,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconFavorite,&m_iconFavoriteFocus,m_iconoffset,0);
		}
#else
		wchar_t str_sub[32];
		swprintf_s(str_sub,32,_T("AM (%d)"),m_boxAM.GetCount()+1);
		pItem=new CListBoxItemGL(str,NULL,str_sub);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
		pItem->SetUserParam(m_RegionId);
		if (IsFavorite(dwFreq))
		{
			pItem->SetIcon2(&m_iconFavoriteFocus,&m_iconFavorite);
		}
		else
		{
			pItem->SetIcon2(&m_iconFavorite,&m_iconFavoriteFocus);
		}
#endif

		m_boxAM.AddItemByOrder(pItem,dwFreq);
	}

	//
	int index=m_RegionIdtoSet;
	if(index < RADIO_REGION_Max && m_listAM[index].SearchID(dwFreq) == FALSE)
	{
		RadioStationInfo *info=new RadioStationInfo();
		memset(info,0,sizeof(RadioStationInfo));
		info->dwBand=RADIO_BAND_ID_AM;
		info->dwRegion=m_RegionIdtoSet;
		info->dwFreq=dwFreq;
		m_listAM[index].Add(info,dwFreq);
		m_bAMListChanged[index]=TRUE;
	}
}
void CPanelRadioLibrary::UpdateListPS()
{
	wchar_t str[32];

	if(m_BandId == RADIO_BAND_ID_FM)
	{
		swprintf_s(str,32,_T("%d.%02d MHz (%s)"),m_dwCurFreq/1000000,m_dwCurFreq/10000-m_dwCurFreq/1000000*100,m_strPS.String());
		int index=m_boxFM.SearchID(m_dwCurFreq);
		if(index!=-1)
		{
			m_boxFM.SetItemName(m_dwCurFreq,str);
		}

		index=m_boxFavorite.SearchID(m_dwCurFreq);
		if(index!=-1)
		{
			m_boxFavorite.SetItemName(m_dwCurFreq,str);
			m_bFavListChanged=TRUE;
		}

		//preset list:
		if(m_RegionId < RADIO_REGION_Max)
		{
			if(m_listFM[m_RegionId].SearchID(m_dwCurFreq) != FALSE)
			{
				m_bFMListChanged[m_RegionId] = TRUE;
				wcscpy_s(m_listFM[m_RegionId].Get()->name,32,str);
			}
		}
	}
	/*
	else if(m_BandId == RADIO_BAND_ID_AM)
	{
		swprintf_s(str,32,_T("%d%02d KHz (%s)"),m_dwCurFreq/100000,m_dwCurFreq/1000-m_dwCurFreq/100000*100,m_strPS.String());
		int index=m_boxAM.SearchID(m_dwCurFreq);
		if(index!=-1)
		{
			m_boxAM.SetItemName(m_dwCurFreq,str);
		}

		index=m_boxFavorite.SearchID(m_dwCurFreq);
		if(index!=-1)
		{
			m_boxFavorite.SetItemName(m_dwCurFreq,str);
			m_bFavListChanged=TRUE;
		}

		//preset list:
		if(m_RegionId < RADIO_REGION_Max)
		{
			if(m_listAM[m_RegionId].SearchID(m_dwCurFreq) != FALSE)
			{
			m_bAMListChanged[m_RegionId] = TRUE;
			wcscpy_s(m_listAM[m_RegionId].Get()->name,32,str);
			}
		}
	}
	*/
}
void CPanelRadioLibrary::AddFavorite(DWORD dwFreq,RadioBandId idBand,RadioRegionId idRegion,const wchar_t *pStrName)
{

	CListBoxItemGL *pItem;
	wchar_t str[32];

	if(dwFreq==0 || m_boxFavorite.SearchID(dwFreq) != -1)
	{
		return;
	}

	if(idBand == RADIO_BAND_ID_FM)
	{
		if(pStrName && wcslen(pStrName))
		{
			swprintf_s(str,32,_T("%s"),pStrName);
		}
		else
		{
			swprintf_s(str,32,_T("%d.%02d %s"),dwFreq/1000000,dwFreq/10000-dwFreq/1000000*100,GetResourceString(IDS_MHZ));
		}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(str,NULL,_T("FM"), 10, 0);
		pItem->SetCaptioniOffset(10,0,10);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus,3,0);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus,m_iconoffset,0);
#else
		pItem = new CListBoxItemGL(str,NULL,_T("FM"));
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
#endif
		pItem->SetUserParam(idRegion);
		pItem->ShowValue(FALSE);
		m_boxFavorite.AddItemByOrder(pItem,dwFreq);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_boxFM.SetIndexIcon2(dwFreq,&m_iconFavoriteFocus,&m_iconFavorite,m_iconoffset,0);
#else
		m_boxFM.SetIndexIcon2(dwFreq,&m_iconFavoriteFocus,&m_iconFavorite);
#endif

		SendRadioCommand(RADIO_CMD_QUERY_FAVORITE_STATUS,m_dwCurFreq);
		m_bFavListChanged = TRUE;
	}
	else if(idBand == RADIO_BAND_ID_AM)
	{
		if(pStrName && wcslen(pStrName))
		{
			swprintf_s(str,32,_T("%s"),pStrName);
		}
		else
		{
			swprintf_s(str,32,_T("%d%02d %s"),dwFreq/100000,dwFreq/1000-dwFreq/100000*100,GetResourceString(IDS_KHZ));
		}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(str,NULL,_T("AM"), 10, 0);
		pItem->SetCaptioniOffset(10,0,10);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus,3,0);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus,m_iconoffset,0);
#else
		pItem = new CListBoxItemGL(str,NULL,_T("AM"));
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
#endif
		pItem->SetUserParam(idRegion);
		pItem->ShowValue(FALSE);
		m_boxFavorite.AddItemByOrder(pItem,dwFreq);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_boxAM.SetIndexIcon2(dwFreq,&m_iconFavoriteFocus,&m_iconFavorite,m_iconoffset,0);
#else
		m_boxAM.SetIndexIcon2(dwFreq,&m_iconFavoriteFocus,&m_iconFavorite);
#endif

		SendRadioCommand(RADIO_CMD_QUERY_FAVORITE_STATUS,m_dwCurFreq);
		m_bFavListChanged = TRUE;
	}
}
void CPanelRadioLibrary::DeleteFavorite(DWORD dwFreq)
{
	m_boxFavorite.DeleteItem(dwFreq);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_boxFM.SetIndexIcon2(dwFreq,&m_iconFavorite,&m_iconFavoriteFocus,m_iconoffset,0);
	m_boxAM.SetIndexIcon2(dwFreq,&m_iconFavorite,&m_iconFavoriteFocus,m_iconoffset,0);
#else
	m_boxFM.SetIndexIcon2(dwFreq,&m_iconFavorite,&m_iconFavoriteFocus);
	m_boxAM.SetIndexIcon2(dwFreq,&m_iconFavorite,&m_iconFavoriteFocus);
#endif

	SendRadioCommand(RADIO_CMD_QUERY_FAVORITE_STATUS,m_dwCurFreq);
	m_bFavListChanged = TRUE;

	//
	if(!m_boxFavorite.MoveToIndex(m_boxFavorite.SearchID(m_dwCurFreq)))
	{
		m_boxFavorite.KillFocus();
	}
}
int CPanelRadioLibrary::GetFMListCnt()
{
	return m_boxFM.GetCount();

}
int CPanelRadioLibrary::GetAMListCnt()
{
	return m_boxAM.GetCount();

}
int CPanelRadioLibrary::GetFavListCnt()
{
	return m_boxFavorite.GetCount();

}
int CPanelRadioLibrary::GetFMStorageIndex(DWORD dwFreq)
{
	return m_boxFM.SearchID(dwFreq);

}
int CPanelRadioLibrary::GetAMStorageIndex(DWORD dwFreq)
{
	return m_boxAM.SearchID(dwFreq);

}
DWORD CPanelRadioLibrary::GetNextFM()
{
	int index=m_boxFM.SearchID(m_dwCurFreq);

	if(index==-1 || m_boxFM.GetCount()<=1)
	{
		
		return -1;
	}

	if(m_boxFM.MoveToIndex(index))
	{
		m_boxFM.MoveToNext();
		return m_boxFM.GetCurFocusItemID();
	}
	else
	{
		return -1;
	}
}
DWORD CPanelRadioLibrary::GetPrevFM()
{
	int index=m_boxFM.SearchID(m_dwCurFreq);

	if(index==-1 || m_boxFM.GetCount()<=1)
		return -1;

	if(m_boxFM.MoveToIndex(index))
	{
		m_boxFM.MoveToLast();
		return m_boxFM.GetCurFocusItemID();

	}
	else
	{
		return -1;
	}
}
DWORD CPanelRadioLibrary::GetNextAM()
{
	int index=m_boxAM.SearchID(m_dwCurFreq);

	if(index==-1 || m_boxAM.GetCount()<=1)
		return -1;

	if(m_boxAM.MoveToIndex(index))
	{
		m_boxAM.MoveToNext();
		return m_boxAM.GetCurFocusItemID();
	}
	else
	{
		return -1;
	}
}
DWORD CPanelRadioLibrary::GetPrevAM()
{
	int index=m_boxAM.SearchID(m_dwCurFreq);

	if(index==-1 || m_boxAM.GetCount()<=1)
		return -1;

	if(m_boxAM.MoveToIndex(index))
	{
		m_boxAM.MoveToLast();
		return m_boxAM.GetCurFocusItemID();
	}
	else
	{
		return -1;
	}
}

BOOL CPanelRadioLibrary::IsFavorite(DWORD dwFreq)
{
	return m_boxFavorite.SearchID(dwFreq) != -1;
}
void CPanelRadioLibrary::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

	 	//RETAILMSG(DEBUG_RADIO/*DEBUG_RADIO*/,(_T("MSG:RADIO:RadioLibrary::OnMCUCmd:     type:%d,ID:%x,commander:%x\r\n"),dwType,dwID,lParam));

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}

	if(dwType == 0)//package
	{

		RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioLibrary: update data package:%x\r\n"),dwID));

		//region:
		m_RegionId = RadioRegionId(pCommander->GetItemData(MCU_LOCAL_AREA));

		//reset region
		UINT idRegionStored=m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion;
		if(idRegionStored == RADIO_REGION_Euro_EastEuro)
		{
			if(m_RegionId != RADIO_REGION_Europe && m_RegionId != RADIO_REGION_EasternEurope)
			{
				RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain: Reset radio region.%d\r\n"),RADIO_REGION_Europe));
				m_pCommander->SendCmd(_ARM_REGION_SEL_KEY,RADIO_REGION_Europe);
				m_RegionId = RADIO_REGION_Europe;
			}
		}
		else if(idRegionStored == RADIO_REGION_EastEuro_Euro)
		{
			if(m_RegionId != RADIO_REGION_Europe && m_RegionId != RADIO_REGION_EasternEurope)
			{
				RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain: Reset radio region.%d\r\n"),RADIO_REGION_EasternEurope));
				m_pCommander->SendCmd(_ARM_REGION_SEL_KEY,RADIO_REGION_EasternEurope);
				m_RegionId = RADIO_REGION_EasternEurope;
			}
		}
		else if(idRegionStored < RADIO_REGION_Max)
		{
			if(m_RegionId != idRegionStored)
			{
				RETAILMSG(1, (TEXT("MSG:RADIO:RadioMain: Reset radio region.%d\r\n"),idRegionStored));
				m_pCommander->SendCmd(_ARM_REGION_SEL_KEY,idRegionStored);
				m_RegionId = (RadioRegionId)idRegionStored;
			}
		}
		m_RegionIdtoSet = m_RegionId;


		//band:
		int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
		BYTE high=pCommander->GetItemData(MCU_CUR_FREQU_STORE_H);
		BYTE low=pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);

		if(iband==0)//FM
		{
			m_BandId = RADIO_BAND_ID_FM;
			SetShowType(RADIO_LIBRARY_FM);
		}
		else//AM
		{
			m_BandId = RADIO_BAND_ID_AM;
			SetShowType(RADIO_LIBRARY_AM);
		}
		if(iband==0)
		{
			m_dwCurFreq = ((high*100+low)*10000);
            //RETAILMSG(1, (TEXT("MSG:RADIO:RadioLibrary: [FM] Package msg m_dwCurFreq=%d\r\n"),m_dwCurFreq));
			if(m_RegionId < RADIO_REGION_Max)
			{
				m_infoFM[m_RegionId].dwRegion=m_RegionId;
				m_infoFM[m_RegionId].dwBand=RADIO_BAND_ID_FM;
				m_infoFM[m_RegionId].dwFreq=m_dwCurFreq;
			}
		}
		else
		{
			m_dwCurFreq = ((high*100+low)*1000);
            //RETAILMSG(1, (TEXT("MSG:RADIO:RadioLibrary: [AM] Package msg m_dwCurFreq=%d\r\n"),m_dwCurFreq));

			if(m_RegionId < RADIO_REGION_Max)
			{
				m_infoAM[m_RegionId].dwRegion=m_RegionId;
				m_infoAM[m_RegionId].dwBand=RADIO_BAND_ID_AM;
				m_infoAM[m_RegionId].dwFreq=m_dwCurFreq;
			}
		}

		/*
		m_bRDSOn = pCommander->GetItemData(MCU_RDS_STATE);
		m_bTAOn = pCommander->GetItemData(MCU_RDS_TA);
		m_bAFOn = pCommander->GetItemData(MCU_RDS_AF);
		*/

		//
		ReloadList(m_RegionId,FALSE);
	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
		//case MCU_CUR_FREQU_STORE_H:
		case MCU_CUR_FREQU_STORE_L:
			if(!m_bAFSearching)
			{
				int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
				BYTE high=pCommander->GetItemData(MCU_CUR_FREQU_STORE_H);
				BYTE low=pCommander->GetItemData(MCU_CUR_FREQU_STORE_L);
//	                RETAILMSG(1, (TEXT("MSG:RADIO:RadioLibrary: #MCU_CUR_FREQU_STORE_L#    iband[%d]\r\n"),pCommander->GetItemData(MCU_CUR_BAND_STORE)));
//	                RETAILMSG(1, (TEXT("MSG:RADIO:RadioLibrary: #MCU_CUR_FREQU_STORE_L#    high[%d]\r\n"),pCommander->GetItemData(MCU_CUR_FREQU_STORE_H)));
//	                RETAILMSG(1, (TEXT("MSG:RADIO:RadioLibrary: #MCU_CUR_FREQU_STORE_L#    low[%d]\r\n"),pCommander->GetItemData(MCU_CUR_FREQU_STORE_L)));
				if(iband==0)
				{
					m_dwCurFreq = ((high*100+low)*10000);
                    //RETAILMSG(1, (TEXT("MSG:RADIO:RadioLibrary: [FM] Single msg m_dwCurFreq=%d\r\n"),m_dwCurFreq));

					if(m_RegionId == m_RegionIdtoSet && m_RegionId < RADIO_REGION_Max)
					{
						m_infoFM[m_RegionId].dwRegion=m_RegionId;
						m_infoFM[m_RegionId].dwBand=RADIO_BAND_ID_FM;
						m_infoFM[m_RegionId].dwFreq=m_dwCurFreq;
					}
				}
				else
				{
					m_dwCurFreq = ((high*100+low)*1000);
                    //RETAILMSG(1, (TEXT("MSG:RADIO:RadioLibrary: [AM] Single msg m_dwCurFreq=%d\r\n"),m_dwCurFreq));

					if(m_RegionId == m_RegionIdtoSet && m_RegionId < RADIO_REGION_Max)
					{
						m_infoAM[m_RegionId].dwRegion=m_RegionId;
						m_infoAM[m_RegionId].dwBand=RADIO_BAND_ID_AM;
						m_infoAM[m_RegionId].dwFreq=m_dwCurFreq;
					}
				}

//	                RETAILMSG(1, (TEXT("MSG:RADIO:RadioLibrary: #MCU_CUR_FREQU_STORE_L#    m_dwCurFreq[%d]\r\n"),m_dwCurFreq));
			}
			break;
		case MCU_CUR_BAND_STORE:
			{
				int iband=pCommander->GetItemData(MCU_CUR_BAND_STORE);
//			        RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioLibrary: #MCU_CUR_BAND_STORE# iband[%d]\r\n"),iband));
				if(iband==0)//FM
				{
					if(m_type!=RADIO_LIBRARY_FAVORITE)
						SetShowType(RADIO_LIBRARY_FM);
					m_BandId = RADIO_BAND_ID_FM;
				}
				else//AM
				{
					if(m_type!=RADIO_LIBRARY_FAVORITE)
						SetShowType(RADIO_LIBRARY_AM);
					m_BandId = RADIO_BAND_ID_AM;
				}
			}
			break;
			/*
		case MCU_RDS_STATE:
			{
				m_bRDSOn = pCommander->GetItemData(MCU_RDS_STATE);
			}
			break;
		case MCU_RDS_TA:
			{
				m_bTAOn = pCommander->GetItemData(MCU_RDS_TA);
			}
			break;
		case MCU_RDS_AF:
			{
				m_bAFOn = pCommander->GetItemData(MCU_RDS_AF);
			}
			break;
			*/
		case MCU_SYS_INFO_TITLE:
			{
				BYTE info=pCommander->GetItemData(MCU_SYS_INFO_TITLE);
				m_bSearching=(info==3||info==4);
			}
		case MCU_LOCAL_AREA:
			{
				RadioRegionId region=RadioRegionId(pCommander->GetItemData(MCU_LOCAL_AREA));
//			        RETAILMSG(DEBUG_RADIO,(_T("MSG:RADIO:RadioLibrary: #MCU_LOCAL_AREA# region[%d] m_RegionId[%d]\r\n"),region,m_RegionId));
				if(m_RegionId != region)
				{
					m_RegionId = region;
					//ClearList(RADIO_BAND_ID_INVALID);
					if(m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_Euro_EastEuro || 
						m_pSkinManager->GetConfig()->GetCurParam()->idRadioRegion == RADIO_REGION_EastEuro_Euro)
					{
						//if(m_BandId == RADIO_BAND_ID_FM)
						{
							ReloadList(m_RegionId,m_bResetFreq);
						}
					}
					else
					{
						ClearList(RADIO_BAND_ID_INVALID);
					}

					/*
					//auto search:
					if(m_bRDSOn
						&&(m_bTAOn || m_bAFOn)
						&& (m_RegionId==RADIO_REGION_Europe || m_RegionId==RADIO_REGION_EasternEurope))
					{
						SendRadioCommand(RADIO_CMD_SEARCH,ARM_ADD_OPERATION);
					}
					*/
				}
			}
			break;
		case MCU_CUR_CHANNEL_LEVEL:
			{

			}
			break;
		case MCU_FREQ_STORE_INDEX:
			{
				m_iCurStoreIndex = (pCommander->GetItemData(MCU_FREQ_STORE_INDEX));
			}
			break;
		//case MCU_FREQ_STORE_H:
		case MCU_FREQ_STORE_L:
			{
				BYTE high=pCommander->GetItemData(MCU_FREQ_STORE_H);
				BYTE low=pCommander->GetItemData(MCU_FREQ_STORE_L);

				//add frequency:
// 				if(m_iCurStoreIndex<=99)//fm
// 				{
// 					AddFM((high*100+low)*10000);
// 				}
// 				else//am
// 				{
// 					AddAM((high*100+low)*1000);
// 				}
				if(high>=5 && high<=17)//am
				{
					AddAM((high*100+low)*1000);
				}
				else if(high>=60 && high<=108)//fm
				{
					AddFM((high*100+low)*10000);
				}
			}
			break;
// 		case MCU_RDS_PS:
// 			{
// 				if(pCommander->GetRDSData()->bufPS[0])
// 				{
// 					wchar_t ps[16]=_T("");
// 					GBKTOUnicode((char *)pCommander->GetRDSData()->bufPS,9,ps,16);
// 					m_strPS=ps;
//
// 					//
// 					UpdateListPS();
// 				}
// 				else
// 				{
// 					if(m_strPS.String())
// 					{
// 						m_strPS=NULL;
// 					}
// 				}
// 			}
// 			break;
		}
	}
}
void CPanelRadioLibrary::SetPS(const wchar_t* ps)
{
	if(ps)
	{
		m_strPS=ps;
		UpdateListPS();
	}
	else
	{
		if(m_strPS.String())
		{
			m_strPS=NULL;
		}
	}
}
void CPanelRadioLibrary::ReloadList(RadioRegionId idRegion,BOOL bResetFreq)
{
//		RETAILMSG(1,(_T("MSG:RADIO:RadioLibrary:ReloadList idRegion[%d] bResetFreq[%d]\r\n"),idRegion,bResetFreq));

	CListBoxItemGL *pItem;
	wchar_t str[32];
	wchar_t str_sub[32];
	DWORD dwFreq;

	int index=(idRegion);

	if(index >= RADIO_REGION_Max)
		return;

	//if(m_listAM[index].GetCount())
	{
		m_boxAM.ClearList();
	}
	//if(m_listFM[index].GetCount())
	{
		m_boxFM.ClearList();
	}

	m_listAM[index].Head();
	while (!m_listAM[index].End())
	{
		dwFreq = m_listAM[index].Get()->dwFreq;
		if(wcslen(m_listAM[index].Get()->name))
			swprintf_s(str,32,_T("%s"),m_listAM[index].Get()->name);
		else
 			swprintf_s(str,32,_T("%d%02d %s"),dwFreq/100000,dwFreq/1000-dwFreq/100000*100,GetResourceString(IDS_KHZ));

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(str,NULL,NULL,10,0);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus,3,0);
		pItem->SetUserParam(m_RegionId);
		if (IsFavorite(dwFreq))
		{
			pItem->SetIcon2(&m_iconFavoriteFocus,&m_iconFavorite,m_iconoffset,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconFavorite,&m_iconFavoriteFocus,m_iconoffset,0);
		}
#else
		swprintf_s(str_sub,32,_T("AM (%d)"),m_boxAM.GetCount()+1);
		pItem=new CListBoxItemGL(str,NULL,str_sub);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
		if (IsFavorite(dwFreq))
		{
			pItem->SetIcon2(&m_iconFavoriteFocus,&m_iconFavorite);
		}
		else
		{
			pItem->SetIcon2(&m_iconFavorite,&m_iconFavoriteFocus);
		}
#endif
		m_boxAM.AddItemByOrder(pItem,dwFreq);

		m_listAM[index].Next();
	}

	m_listFM[index].Head();
	while (!m_listFM[index].End())
	{
		dwFreq = m_listFM[index].Get()->dwFreq;
		if(wcslen(m_listFM[index].Get()->name))
			swprintf_s(str,32,_T("%s"),m_listFM[index].Get()->name);
		else
			swprintf_s(str,32,_T("%d.%02d %s"),dwFreq/1000000,dwFreq/10000-dwFreq/1000000*100,GetResourceString(IDS_MHZ));

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(str,NULL,NULL,10,0);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus,3,0);
		pItem->SetUserParam(m_RegionId);
		if (IsFavorite(dwFreq))
		{
			pItem->SetIcon2(&m_iconFavoriteFocus,&m_iconFavorite,m_iconoffset,0);
		}
		else
		{
			pItem->SetIcon2(&m_iconFavorite,&m_iconFavoriteFocus,m_iconoffset,0);
		}
#else
		swprintf_s(str_sub,32,_T("FM (%d)"),m_boxFM.GetCount()+1);
		pItem=new CListBoxItemGL(str,NULL,str_sub);
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
		if (IsFavorite(dwFreq))
		{
			pItem->SetIcon2(&m_iconFavoriteFocus,&m_iconFavorite);
		}
		else
		{
			pItem->SetIcon2(&m_iconFavorite,&m_iconFavoriteFocus);
		}
#endif
		m_boxFM.AddItemByOrder(pItem,dwFreq);

		m_listFM[index].Next();
	}

	if(bResetFreq)
	{
		//set ini freq:
		if(m_BandId == RADIO_BAND_ID_FM)
		{
			DWORD freq=(m_RegionId < RADIO_REGION_Max)?m_infoFM[m_RegionId].dwFreq:0;
			if(m_boxFM.SearchID(freq) == -1)//not exist
			{
				CListBoxItemGL *pitem=m_boxFM.m_listItems.GetItemByIndex(0);
				if(pitem)
				{
					freq=pitem->GetID();
				}
			}
			if(freq!=-1 && freq!=0)
			{
//					RETAILMSG(1,(_T("MSG:RADIO:RadioLibrary:ReloadList(FM) ResetFreq:%d*************\n"),freq));

				SendRadioCommand(RADIO_CMD_SET_FREQ,freq);
			}
		}
		else if(m_BandId == RADIO_BAND_ID_AM)
		{
			DWORD freq=(m_RegionId < RADIO_REGION_Max)?m_infoAM[m_RegionId].dwFreq:0;
			if(m_boxAM.SearchID(freq) == -1)//not exist
			{
				CListBoxItemGL *pitem=m_boxAM.m_listItems.GetItemByIndex(0);
				if(pitem)
				{
					freq=pitem->GetID();
				}
			}
			if(freq!=-1 && freq!=0)
			{
//					RETAILMSG(1,(_T("MSG:RADIO:RadioLibrary:ReloadList(AM) ResetFreq :%d*************\n"),freq));

				SendRadioCommand(RADIO_CMD_SET_FREQ,freq);
			}
		}
	}
}

BOOL CPanelRadioLibrary::SaveFavData()
{
	FILE	*file = NULL;

	if(!m_bFavListChanged)
		return FALSE;

	m_bFavListChanged = FALSE;

	CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("Radio\\"));

	CreateDirectory(strPath.String(),NULL);

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\RadioFav.dat"));

	DWORD dwStart=GetTickCount();

	RETAILMSG(DEBUG_RADIO,(_T("MSG:[%s]: Begin Save radio fav data.%s\r\n"),TEXT(__FUNCTION__),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"wb") )
	{
		return FALSE;
	}

	//
	RadioStationInfo info;
	memset(&info,0,sizeof(RadioStationInfo));
	info.dwSize=sizeof(RadioStationInfo);

	m_boxFavorite.m_listItems.Head();
	while (!m_boxFavorite.m_listItems.End())
	{
		info.dwFreq = m_boxFavorite.m_listItems.Get()->GetID();
		wcscpy_s(info.name,MAX_RADIO_STA_NAME,m_boxFavorite.m_listItems.Get()->GetItemName());

		if (lstrcmpi(m_boxFavorite.m_listItems.Get()->GetItemSubName(), _T("AM")) == 0)//AM
		{
			info.dwBand=1;
		}
		else
		{
			info.dwBand=0;
		}

		info.dwRegion = RadioRegionId(m_boxFavorite.m_listItems.Get()->GetUserParam());

		//
		fwrite(&info,info.dwSize,1,file);

		m_boxFavorite.m_listItems.Next();
	}

	fclose(file);

	RETAILMSG(DEBUG_RADIO,(_T("MSG:[%s]: End Save radio fav data,  take %d ms\r\n"),TEXT(__FUNCTION__),GetTickCount()-dwStart));

	return TRUE;
}
BOOL CPanelRadioLibrary::GetRegionPresetData(RadioRegionId idRegion)
{
	if(idRegion >= RADIO_REGION_Max)
		return FALSE;

	FILE					*file = NULL;
	int					fileSize = 0;
	BYTE				*localBuffer;
	BYTE				*cursor;

	DWORD dwStart=GetTickCount();
	wchar_t tmp[32];
	swprintf_s(tmp, 32, _T("%d"),idRegion);
	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\RadioPreset_"))+CM_String(tmp)+CM_String(_T(".dat"));

	RETAILMSG(DEBUG_RADIO,(_T("\n*************Begin GetRegionPresetData.%s*************\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"rb") )
	{
		RETAILMSG(DEBUG_RADIO,(_T("ERROR: radio data file not found,%s\n"), strFile.String()));
		return  FALSE;
	}

	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	/**** Read data from file ****/
	localBuffer = new BYTE[fileSize];
	if(!localBuffer)
	{
		fclose(file);
		return FALSE;
	}

	fread(localBuffer, fileSize, 1, file);
	cursor = localBuffer;

	fclose(file);

	//para data:
	int count=0;
	RadioStationInfo *info=NULL;
	int siInfo=sizeof(RadioStationInfo);

	while ((cursor-localBuffer)<=fileSize-siInfo)
	{
		//点击进入显示的FM
		if(count==0)//current FM
		{
			memcpy(&m_infoFM,cursor,siInfo);
		}
		else if(count==1)//current AM
		{
			memcpy(&m_infoAM,cursor,siInfo);
		}
		else
		{
			info = new RadioStationInfo;
			memcpy(info,cursor,siInfo);

			RETAILMSG(DEBUG_RADIO,(_T("**GetRegionPresetData: dwBand=%d,  idRegion=%d, freq=%d******\n"),info->dwBand,idRegion,info->dwFreq));

			if(info->dwBand==RADIO_BAND_ID_FM)
			{
				m_listFM[idRegion].Add(info,info->dwFreq);
			}
			else
			{
				m_listAM[idRegion].Add(info,info->dwFreq);
			}
		}

		cursor+=siInfo;
		count++;
	}


	delete[] localBuffer;

	RETAILMSG(DEBUG_RADIO,(_T("\n*************End GetRegionPresetData, total count:%d,  take %d ms*************\n"),count,GetTickCount()-dwStart));

	return TRUE;
}
BOOL CPanelRadioLibrary::SaveRegionPresetData(RadioRegionId idRegion)
{
	FILE	*file = NULL;

	if(idRegion >= RADIO_REGION_Max)
		return FALSE;

	if(!m_bFMListChanged[idRegion] && !m_bAMListChanged[idRegion])
		return FALSE;

	m_bFMListChanged[idRegion] = FALSE;
	m_bAMListChanged[idRegion] = FALSE;

	CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("Radio\\"));

	CreateDirectory(strPath.String(),NULL);

	wchar_t tmp[32];
	swprintf_s(tmp, 32, _T("%d"),idRegion);
	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\RadioPreset_"))+CM_String(tmp)+CM_String(_T(".dat"));

	DWORD dwStart=GetTickCount();

	RETAILMSG(DEBUG_RADIO,(_T("\n*************Begin SaveRegionPresetData .%s*************\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"wb") )
	{
		return FALSE;
	}

	//save current:
	fwrite(&m_infoFM[idRegion],sizeof(RadioStationInfo),1,file);
	fwrite(&m_infoAM[idRegion],sizeof(RadioStationInfo),1,file);


	//FM:
	m_listFM[idRegion].Head();
	while (!m_listFM[idRegion].End())
	{
		fwrite(m_listFM[idRegion].Get(),sizeof(RadioStationInfo),1,file);
		m_listFM[idRegion].Next();
	}
	//AM:
	m_listAM[idRegion].Head();
	while (!m_listAM[idRegion].End())
	{
		fwrite(m_listAM[idRegion].Get(),sizeof(RadioStationInfo),1,file);
		m_listAM[idRegion].Next();
	}

	fclose(file);

	RETAILMSG(DEBUG_RADIO,(_T("\n*************End SaveRegionPresetData,  take %d ms*************\n"),GetTickCount()-dwStart));

	return TRUE;
}
BOOL CPanelRadioLibrary::GetFavData()
{
	FILE					*file = NULL;
	int					fileSize = 0;
	BYTE				*localBuffer;
	BYTE				*cursor;

	DWORD dwStart=GetTickCount();

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\RadioFav.dat"));

	RETAILMSG(DEBUG_RADIO,(_T("\n*************Begin GetFavData.%s*************\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"rb") )
	{
		RETAILMSG(DEBUG_RADIO,(_T("ERROR: radio data file not found,%s\n"), strFile.String()));
		return  FALSE;
	}

	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	/**** Read data from file ****/
	localBuffer = new BYTE[fileSize];
	if(!localBuffer)
	{
		fclose(file);
		return FALSE;
	}

	fread(localBuffer, fileSize, 1, file);
	cursor = localBuffer;

	fclose(file);

	//para data:
	int count=0;
	RadioStationInfo info;
	int siInfo=sizeof(RadioStationInfo);

	while ((cursor-localBuffer)<=fileSize-siInfo)
	{
		memcpy(&info,cursor,siInfo);
		//
		AddFavorite(info.dwFreq,RadioBandId(info.dwBand),RadioRegionId(info.dwRegion),info.name);

		cursor+=siInfo;
		count++;
	}


	delete[] localBuffer;

	RETAILMSG(DEBUG_RADIO,(_T("\n*************End GetFavData, total count:%d,  take %d ms*************\n"),count,GetTickCount()-dwStart));

	//reset
	m_bFavListChanged = FALSE;

	return TRUE;
}
void CPanelRadioLibrary::OnSystemPowerStatePrepare(UINT idState)
{
	CPanelGL::OnSystemPowerStatePrepare(idState);

	switch (idState)
	{
	case SYS_POWER_OFF:
	//case SYS_POWER_SOFT_RESET:
		{
			SaveFavData();
			SaveRegionPresetData(RADIO_REGION_Europe);
			SaveRegionPresetData(RADIO_REGION_EasternEurope);
		}
		break;
	}
}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
void CPanelRadioLibrary::ArrowChange(Item_Selected index)
{
	int iconMarginR = -14;

	CListBoxItemGL* pItem;
	m_boxLists.m_listItems.Head();
	while( (pItem = m_boxLists.m_listItems.Get()) != NULL )
	{
		if ((Item_Selected)m_boxLists.m_listItems.GetID() == index)
		{
			pItem->SetIcon2(&m_iconArrowOn, &m_iconArrowOn, iconMarginR , 0);
		}
		else
		{
			pItem->SetIcon2(&m_iconArrowOff, &m_iconArrowOff, iconMarginR , 0);
		}
		m_boxLists.m_listItems.Next();
	}
}
#endif