#include "PanelDABLibrary.h"
#include "../../resource.h"

#if CVTE_EN_DAB

#define BOXLIST_TYPE_ENSEMBLE_SERVICE				0
#define BOXLIST_TYPE_PTY_LIST						1
#define BOXLIST_TYPE_PRESET_LIST					2


CPanelDABLibrary::CPanelDABLibrary(void)
{
	m_dwListCnt = 0;
	m_bPlayable = FALSE;
	m_iBrowserLevel = 0;
	m_idCurDeviceID = 0;
	m_nQueryMode = ListType_AllService;
	m_nCurPTYType = PtyList_AllType;
	m_nDABLibBoxType = DAB_LIBRARY_SERVICE;
	m_bPresetListChanged = FALSE;
	m_pCurPlayingList = NULL;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	int strMarginR = 34;
	m_pServiceItem = new CListBoxItemGL(_T("Service"),NULL,NULL ,strMarginR,0);
	m_pPtyItem	= new CListBoxItemGL(_T("PTY"),NULL,NULL ,strMarginR,0);
	m_pEnsembleItem = new CListBoxItemGL(_T("ENsemble"),NULL,NULL ,strMarginR,0);
#endif
}
CPanelDABLibrary::~CPanelDABLibrary(void)
{

}

BOOL CPanelDABLibrary::IsReady()
{
	switch (m_nDABLibBoxType)
	{
	case DAB_LIBRARY_SERVICE:
		return CPanelGL::IsReady() && m_boxServiceList.IsReady();
		break;
	case DAB_LIBRARY_PTY_LIST:
		return CPanelGL::IsReady() && m_boxPTYList.IsReady() && (m_boxPTYServiceList.IsVisible()?m_boxPTYServiceList.IsReady():TRUE);
		break;
	case DAB_LIBRARY_ENSEMBLE_LIST:
		return CPanelGL::IsReady() && m_boxEnsembleList.IsReady() && (m_boxEnsembleServiceList.IsVisible()?m_boxEnsembleServiceList.IsReady():TRUE);
		break;
	default:
		return CPanelGL::IsReady();
		break;
	}
}

void CPanelDABLibrary::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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
		SetShowType((DAB_LIBRARY_TYPE)iParam);
	}
	else
	{
		SetShowType((DAB_LIBRARY_TYPE)m_nDABLibBoxType);
	}
//		if(m_boxServiceList.GetCount() <= 0)
//		{
//			UpdateServiceList();
//		}

//		BYTE nPreset;
//		DABApi_GetCurrentPreset(&nPreset);
//		RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: nPreset[%d]\r\n"),TEXT(__FUNCTION__),nPreset));
}

void CPanelDABLibrary::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);
}
void CPanelDABLibrary::OnGestureMsg(UINT idGesture)
{
	CPanelGL::OnGestureMsg(idGesture);

	switch (idGesture)
	{
	case GEST_DUAL_TOUCH_PAN_EAST:
		break;
	case GEST_DUAL_TOUCH_PAN_WEST:
		break;
	}
}
void CPanelDABLibrary::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);

}
void CPanelDABLibrary::OnTouchUp(POINT *pt)
{

	if(m_nDABLibBoxType == DAB_LIBRARY_PTY_LIST)
	{
		if(pt->x < LIST_BOX_ITEM_WIDTH +140 && m_boxPTYServiceList.IsVisible() && !m_boxPTYServiceList.IsPtnOnObject(pt))
		{
			ShowPTYServiceDetail(FALSE);
		}
	}
	else if(m_nDABLibBoxType == DAB_LIBRARY_ENSEMBLE_LIST)
	{
		if(pt->x < LIST_BOX_ITEM_WIDTH +140 && m_boxEnsembleServiceList.IsVisible() && !m_boxEnsembleServiceList.IsPtnOnObject(pt))
		{
			ShowEnsembleServiceDetail(FALSE);
		}
	}

	CPanelGL::OnTouchUp(pt);

}
void CPanelDABLibrary::OnTouchMove(POINT *pt)
{
	CPanelGL::OnTouchMove(pt);

}

void CPanelDABLibrary::Render()
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

	m_DABboxList.SetAlpha(m_fCurAlpha);
	m_DABboxList.Render();

	m_btnNowPlaying.SetAlpha(m_fCurAlpha);
	m_btnNowPlaying.Render();

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	switch (m_nDABLibBoxType)
	{
	case DAB_LIBRARY_SERVICE:
		m_boxServiceList.SetAlpha(m_fCurAlpha);
		m_boxServiceList.Render();

		m_btnHome.EnableNotify(!m_boxServiceList.IsOnQuickSearchMode());
		m_DABboxList.EnableNotify(!m_boxServiceList.IsOnQuickSearchMode());
		break;
	case DAB_LIBRARY_PTY_LIST:
		m_boxPTYList.SetAlpha(m_fCurAlpha);
		m_boxPTYList.Render();
		m_boxPTYServiceList.SetAlpha(m_fCurAlpha);
		m_boxPTYServiceList.Render();

		m_btnHome.EnableNotify(TRUE);
		m_DABboxList.EnableNotify(TRUE);
		break;
	case DAB_LIBRARY_ENSEMBLE_LIST:
		m_boxEnsembleList.SetAlpha(m_fCurAlpha);
		m_boxEnsembleList.Render();

		m_boxEnsembleServiceList.SetAlpha(m_fCurAlpha);
		m_boxEnsembleServiceList.Render();

		m_btnHome.EnableNotify(!m_boxEnsembleList.IsOnQuickSearchMode() && !m_boxEnsembleServiceList.IsOnQuickSearchMode());
		m_DABboxList.EnableNotify(!m_boxEnsembleList.IsOnQuickSearchMode() && !m_boxEnsembleServiceList.IsOnQuickSearchMode());
		break;
	}

	EndRender();
}

BOOL CPanelDABLibrary::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
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


	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_highlight={1.0f,1.0f,1.0f,1.0};
#else
	COLORGL cr_highlight={255.0f/255.0f,63.0f/255.0f,63.0f/255.0f,1.0};
#endif 

	int cx=LIST_BOX_ITEM_WIDTH;     //234    //240;
	int cy=LIST_BOX_ITEM_HEIGHT;    //100
	int itemh=LIST_BOX_TAG_HEIGHT;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	int count=(g_iClientHeight-cy*2)/cy;
	if((g_iClientHeight-cy*2)%cy > (cy/2))
		count+=1;

	LISTBOXSTRUCTURE box;
	memset(&box,0,sizeof(box));
	box.iSpacing=(g_iClientHeight-cy*2)/count/2*2;
	box.siClient.cx = cx;	
	box.siClient.cy = (box.iSpacing * count);
	box.siBorder.cx = cx;
	box.siBorder.cy = g_iClientHeight-cy*2;
	box.idTexIndexBase=TEXID_068;
	box.idTexIndexFocus=TEXID_240;
	box.idTexBase=TEXID_285;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_DABboxList.Initialize(IDC_DAB_LIB_BOX_LIST,this,&box,pSkinManager);
	m_DABboxList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_DABboxList.SetPos(-g_iClientWidth/2+cx/2,0);
	m_DABboxList.LinkTextGL(pTextGL);
	m_DABboxList.Show(TRUE,FALSE);
	m_DABboxList.SetAlpha(1.0f);
	m_DABboxList.SetCaptioniHeight(18);
	m_DABboxList.SetCaptionColor(&cr_gray,&cr_w);

	
	m_iconService.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1411));
	m_iconServiceFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1412));

	m_iconEnsemble.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1411));
	m_iconEnsembleFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1412));

	m_iconPTY.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1411));
	m_iconPTYFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1412));

	m_ServiceArrowOn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_083));
	m_ServiceArrowOn.SetShowScale(0.8f);
	m_ServiceArrowOff.Initialize(0,NULL, m_pSkinManager->GetTexture(TEXID_068));
	m_ServiceArrowOff.SetShowScale(0.8f);

	m_EnsembleArrowOn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_083));
	m_EnsembleArrowOn.SetShowScale(0.8f);
	m_EnsembleArrowOff.Initialize(0,NULL, m_pSkinManager->GetTexture(TEXID_068));
	m_EnsembleArrowOff.SetShowScale(0.8f);

	m_PtyArrowOn.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_083));
	m_PtyArrowOn.SetShowScale(0.8f);
	m_PtyArrowOff.Initialize(0,NULL, m_pSkinManager->GetTexture(TEXID_068));
	m_PtyArrowOff.SetShowScale(0.8f);

	int iconMarginR = 24;
	CListBoxItemGL *pItem;
	m_pServiceItem->SetIcon(&m_iconService, &m_iconServiceFocus, iconMarginR, 0);
	m_pServiceItem->SetIcon2(&m_ServiceArrowOn, &m_ServiceArrowOn);
	m_DABboxList.AddItem(m_pServiceItem, DAB_LIBRARY_SERVICE);
	m_pPtyItem->SetIcon(&m_iconPTY, &m_iconPTY, iconMarginR, 0);
	m_pPtyItem->SetIcon2(&m_PtyArrowOff, &m_PtyArrowOff);
	m_DABboxList.AddItem(m_pPtyItem,DAB_LIBRARY_PTY_LIST);
	m_pEnsembleItem->SetIcon(&m_iconEnsemble, &m_iconEnsembleFocus, iconMarginR, 0);
	m_pEnsembleItem->SetIcon2(&m_EnsembleArrowOff, &m_EnsembleArrowOff);
	m_DABboxList.AddItem(m_pEnsembleItem,DAB_LIBRARY_ENSEMBLE_LIST);
	pItem = new CListBoxItemGL(_T(""));
	pItem->EnableClick(FALSE);
	m_DABboxList.AddItem(pItem);


	memset(&box,0,sizeof(box));
	//find param:
	box.iSpacing = g_iClientHeight/6;
	box.siClient.cx = g_iClientWidth-cx;
	box.siClient.cy = g_iClientHeight;	
	box.siBorder.cx = g_iClientWidth-cx;	
	box.siBorder.cy = g_iClientHeight;
	box.idTexIndexBase=TEXID_282;
	box.idTexIndexFocus=TEXID_242;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_285;
	box.idTexBtnUp=TEXID_282;
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

	//Service list:
	m_boxServiceList.Initialize(IDC_DAB_LIB_SERVICE_ITEM,this,&box,pSkinManager);
	m_boxServiceList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxServiceList.SetPos(cx/2,0);
	m_boxServiceList.LinkTextGL(pTextGL);
	m_boxServiceList.Show(TRUE,FALSE);
	m_boxServiceList.SetAlpha(1.0f);
	m_boxServiceList.SetCaptioniHeight(26,19,14);
	m_boxServiceList.SetCaptionColor(&cr_gray,&cr_highlight);

	//Ensemble List:
	m_boxEnsembleList.Initialize(IDC_DAB_LIB_ENSEMBLE_ITEM,this,&box,pSkinManager);
	m_boxEnsembleList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxEnsembleList.SetPos(cx/2,0);
	m_boxEnsembleList.LinkTextGL(pTextGL);
	m_boxEnsembleList.Show(TRUE,FALSE);
	m_boxEnsembleList.SetAlpha(1.0f);
	m_boxEnsembleList.SetCaptioniHeight(26,19,14);
	m_boxEnsembleList.SetCaptionColor(&cr_gray,&cr_highlight);
	//PTY list
	m_boxPTYList.Initialize(IDC_DAB_LIB_PTY_LIST_ITEM,this,&box,pSkinManager);
	m_boxPTYList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxPTYList.SetPos(cx/2,0);
	m_boxPTYList.LinkTextGL(pTextGL);
	m_boxPTYList.Show(FALSE,FALSE);
	m_boxPTYList.SetAlpha(1.0f);
	m_boxPTYList.SetCaptioniHeight(26,19,14);
	m_boxPTYList.SetCaptionColor(&cr_gray,&cr_highlight);

	//initialize ProgramType list:
	for (int i=0;i<PT_Count;i++)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(ProgramTypeToString(ProgramType(i)), NULL, NULL, 50, 0);
		pItem->SetIcon(&m_iconPTY,&m_iconPTYFocus, 24, 0);
#else
		pItem=new CListBoxItemGL(ProgramTypeToString(ProgramType(i)));
		pItem->SetIcon(&m_iconPTY,&m_iconPTYFocus);
#endif
		m_boxPTYList.AddItem(pItem,i);
	}

	//pty service:
	//reset size:
	box.siClient.cx=g_iClientWidth-cx - 160;
	box.siBorder.cx=g_iClientWidth-cx- 160;
	box.idTexBase=TEXID_066;
	box.bNoBorder=FALSE;//

	//pty service list
	m_boxPTYServiceList.Initialize(IDC_DAB_LIB_PTY_SERVICE_LIST_ITEM,this,&box,pSkinManager);
	m_boxPTYServiceList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxPTYServiceList.SetMarginHor(60);
	m_boxPTYServiceList.SetPos(cx/2 + 80,0);
	m_boxPTYServiceList.LinkTextGL(pTextGL);
	m_boxPTYServiceList.Show(FALSE,FALSE);
	m_boxPTYServiceList.SetAlpha(1.0f);
	m_boxPTYServiceList.SetCaptioniHeight(26,19,14);
	m_boxPTYServiceList.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxPTYServiceList.SetAniSpeed(7);

	//test
	for (int i=0;i<16;i++)
	{
		pItem=new CListBoxItemGL(_T("PTY Service test"));
		pItem->SetIcon(&m_iconPTY,&m_iconPTYFocus);
		m_boxPTYServiceList.AddItemByOrder(pItem,i);
	}

	//EnsembleServiceList
	m_boxEnsembleServiceList.Initialize(IDC_DAB_LIB_ENSEMBLE_SERVICE_LIST_ITEM,this,&box,pSkinManager);
	m_boxEnsembleServiceList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxEnsembleServiceList.SetMarginHor(60);
	m_boxEnsembleServiceList.SetPos(cx/2 + 80,0);
	m_boxEnsembleServiceList.LinkTextGL(pTextGL);
	m_boxEnsembleServiceList.Show(FALSE,FALSE);
	m_boxEnsembleServiceList.SetAlpha(1.0f);
	m_boxEnsembleServiceList.SetCaptioniHeight(26,19,14);
	m_boxEnsembleServiceList.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxEnsembleServiceList.SetAniSpeed(7);




	SIZE sibtn={cx,cy};

	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_280));
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_279));

	m_btnNowPlaying.Initialize(IDC_DAB_LIB_BTN_NOWPLAYING,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_btnNowPlaying.SetPos(-g_iClientWidth/2+cx/2,g_iClientHeight/2-cy/2);
	m_btnNowPlaying.SetIcon(&m_iconNowPlayingUp,&m_iconNowPlayingDown);
	m_btnNowPlaying.SetIconPos(-cx/2+14+16,0);
	m_btnNowPlaying.LinkTextGL(pTextGL);
	m_btnNowPlaying.SetCaption(GetResourceString(IDS_TITLE_NOWPLAYING));
	m_btnNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT,58);
	m_btnNowPlaying.SetCaptionColor(&cr_gray,&cr_w);


	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_204));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));

	m_btnHome.Initialize(IDC_DAB_LIB_BTN_HOME ,this,
		m_pSkinManager->GetTexture(TEXID_288),NULL,
		m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.SetIconPos(-cx/2+35,0);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,68);
	m_btnHome.SetCaptionColor(&cr_w,&cr_gray);
	m_btnHome.SetCaptionHeight(20);

#else
	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_441));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_442));
	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_433));
	m_iconNowPlayingUp.SetShowScale(0.8f);
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_434));
	m_iconNowPlayingDown.SetShowScale(0.8f);
	m_iconReturn.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_012));
	m_iconReturn.SetShowScale(0.6f);
	//m_iconChangeQueryMode.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_445));

	m_iconService.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_444));
	m_iconServiceFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_445));

	m_iconEnsemble.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_444));
	m_iconEnsembleFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_445));

	m_iconPTY.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_444));
	m_iconPTYFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_445));

	int ispace=0;

	LISTBOXSTRUCTURE box;

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

	box.idTexIndexBase=TEXID_451;
	box.idTexIndexFocus=TEXID_068;//TEXID_066;//
	box.idTexBase=TEXID_451;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;//FALSE;//
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_DABboxList.Initialize(IDC_DAB_LIB_BOX_LIST,this,&box,pSkinManager);
	m_DABboxList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_DABboxList.SetPos(-g_iClientWidth/2+cx/2,0);
	m_DABboxList.LinkTextGL(pTextGL);
	m_DABboxList.Show(TRUE,FALSE);
	m_DABboxList.SetAlpha(1.0f);
	m_DABboxList.SetCaptioniHeight(22);
	m_DABboxList.SetCaptionColor(&cr_gray,&cr_w);

	CListBoxItemGL *pItem;
	pItem=new CListBoxItemGL(_T("Service"));
	pItem->SetIcon(&m_iconService,&m_iconServiceFocus);
	m_DABboxList.AddItem(pItem,DAB_LIBRARY_SERVICE);

	pItem=new CListBoxItemGL(_T("PTY"));
	pItem->SetIcon(&m_iconPTY,&m_iconPTYFocus);
	m_DABboxList.AddItem(pItem,DAB_LIBRARY_PTY_LIST);

	pItem=new CListBoxItemGL(_T("Ensemble"));
	pItem->SetIcon(&m_iconEnsemble,&m_iconEnsembleFocus);
	m_DABboxList.AddItem(pItem,DAB_LIBRARY_ENSEMBLE_LIST);

	//		pItem=new CListBoxItemGL(_T("PTY list"));
	//		pItem->SetIcon(&m_iconEnsemble,&m_iconEnsembleFocus);
	//		m_DABboxList.AddItem(pItem,1);
	pItem=new CListBoxItemGL(_T(" ")/*GetResourceString(IDS_DEVICE_USB)*/);
	pItem->EnableClick(FALSE);
	m_DABboxList.AddItem(pItem);

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
	//box.bQuickSearchBoard=TRUE;
	box.pxSlider=-20;
	box.fIndexBaseOffset=60;    //74;

	//Service list:
	m_boxServiceList.Initialize(IDC_DAB_LIB_SERVICE_ITEM,this,&box,pSkinManager);
	m_boxServiceList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxServiceList.SetMarginHor(60);
	m_boxServiceList.SetPos(cx/2,0);
	m_boxServiceList.LinkTextGL(pTextGL);
	m_boxServiceList.Show(TRUE,FALSE);
	m_boxServiceList.SetAlpha(1.0f);
	m_boxServiceList.SetCaptioniHeight(26,19,14);
	m_boxServiceList.SetCaptionColor(&cr_gray,&cr_highlight);

	//Ensemble List:
	m_boxEnsembleList.Initialize(IDC_DAB_LIB_ENSEMBLE_ITEM,this,&box,pSkinManager);
	m_boxEnsembleList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxEnsembleList.SetMarginHor(60);
	m_boxEnsembleList.SetPos(cx/2,0);
	m_boxEnsembleList.LinkTextGL(pTextGL);
	m_boxEnsembleList.Show(TRUE,FALSE);
	m_boxEnsembleList.SetAlpha(1.0f);
	m_boxEnsembleList.SetCaptioniHeight(26,19,14);
	m_boxEnsembleList.SetCaptionColor(&cr_gray,&cr_highlight);
	//PTY list
	m_boxPTYList.Initialize(IDC_DAB_LIB_PTY_LIST_ITEM,this,&box,pSkinManager);
	m_boxPTYList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxPTYList.SetMarginHor(60);
	m_boxPTYList.SetPos(cx/2,0);
	m_boxPTYList.LinkTextGL(pTextGL);
	m_boxPTYList.Show(FALSE,FALSE);
	m_boxPTYList.SetAlpha(1.0f);
	m_boxPTYList.SetCaptioniHeight(26,19,14);
	m_boxPTYList.SetCaptionColor(&cr_gray,&cr_highlight);

	//initialize ProgramType list:
	for (int i=0;i<PT_Count;i++)
	{
		pItem=new CListBoxItemGL(ProgramTypeToString(ProgramType(i)));
		pItem->SetIcon(&m_iconPTY,&m_iconPTYFocus);
		m_boxPTYList.AddItem(pItem,i);
	}

	//pty service:
	//reset size:
	box.siClient.cx=g_iClientWidth-cx - 160;
	box.siBorder.cx=g_iClientWidth-cx- 160;
	box.idTexBase=TEXID_066;
	box.bNoBorder=FALSE;//

	//pty service list
	m_boxPTYServiceList.Initialize(IDC_DAB_LIB_PTY_SERVICE_LIST_ITEM,this,&box,pSkinManager);
	m_boxPTYServiceList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxPTYServiceList.SetMarginHor(60);
	m_boxPTYServiceList.SetPos(cx/2 + 80,0);
	m_boxPTYServiceList.LinkTextGL(pTextGL);
	m_boxPTYServiceList.Show(FALSE,FALSE);
	m_boxPTYServiceList.SetAlpha(1.0f);
	m_boxPTYServiceList.SetCaptioniHeight(26,19,14);
	m_boxPTYServiceList.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxPTYServiceList.SetAniSpeed(7);

	//test
	for (int i=0;i<16;i++)
	{
		pItem=new CListBoxItemGL(_T("PTY Service test"));
		pItem->SetIcon(&m_iconPTY,&m_iconPTYFocus);
		m_boxPTYServiceList.AddItemByOrder(pItem,i);
	}

	//EnsembleServiceList
	m_boxEnsembleServiceList.Initialize(IDC_DAB_LIB_ENSEMBLE_SERVICE_LIST_ITEM,this,&box,pSkinManager);
	m_boxEnsembleServiceList.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_boxEnsembleServiceList.SetMarginHor(60);
	m_boxEnsembleServiceList.SetPos(cx/2 + 80,0);
	m_boxEnsembleServiceList.LinkTextGL(pTextGL);
	m_boxEnsembleServiceList.Show(FALSE,FALSE);
	m_boxEnsembleServiceList.SetAlpha(1.0f);
	m_boxEnsembleServiceList.SetCaptioniHeight(26,19,14);
	m_boxEnsembleServiceList.SetCaptionColor(&cr_gray,&cr_highlight);
	m_boxEnsembleServiceList.SetAniSpeed(7);

	SIZE sibtn={cx,cy+ispace};
	SIZE sibtn2={cx/2,cy+ispace};

	m_btnHome.Initialize(IDC_DAB_LIB_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_251),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2+ispace/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_w);
	m_btnHome.SetCaptionHeight(22);
	m_btnHome.SetIconPos(-cx/2+31,0);
	m_btnHome.EnableClickAni(FALSE);

#if 0

	//		m_btnChangeQueryMode.Initialize(IDC_DAB_LIB_BTN_CHANGE_QUERYMODE,this,m_pSkinManager->GetTexture(TEXID_451),NULL,
	//			m_pSkinManager->GetTexture(TEXID_068),NULL,
	//			NULL,NULL,
	//			NULL,NULL);
	//		m_btnChangeQueryMode.SetSize(&sibtn2);
	//		m_btnChangeQueryMode.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	//		m_btnChangeQueryMode.SetPos(g_iClientWidth/2-cx/4-cx/2,g_iClientHeight/2-cy/2-ispace/2);
	//		m_btnChangeQueryMode.SetIcon(&m_iconChangeQueryMode);
	//		m_btnChangeQueryMode.EnableClickAni(FALSE);

	m_btnReturn.Initialize(IDC_DAB_LIB_BTN_RETURN,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnReturn.SetSize(&sibtn2);
	m_btnReturn.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_btnReturn.SetPos(g_iClientWidth/2-cx/4,g_iClientHeight/2-cy/2-ispace/2);
	m_btnReturn.SetIcon(&m_iconReturn/*,&m_iconNowPlayingDown*/);
	m_btnReturn.EnableClickAni(FALSE);

	//SIZE siTitle={g_iClientWidth-cx*2,cy};
	SIZE siTitle={g_iClientWidth-cx-cx/2,cy};
	m_btnTitle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTitle.SetSize(&siTitle);
	m_btnTitle.SetPos(cx/4+1,g_iClientHeight/2-cy/2-ispace/2);
	//m_btnTitle.SetPos(0,g_iClientHeight/2-cy/2-ispace/2);
	m_btnTitle.LinkTextGL(pTextGL);
	m_btnTitle.SetCaption(_T("All service"));
	m_btnTitle.SetCaptionPos(BTN_CAPTION_LEFT,32);
	m_btnTitle.SetCaptionColor(&cr_gray,&cr_w);
	m_btnTitle.SetCaptionHeight(22);

#endif

	m_btnNowPlaying.Initialize(IDC_DAB_LIB_BTN_NOWPLAYING,this,m_pSkinManager->GetTexture(TEXID_251),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPanelDABLibrary::OnNotify,this);
	m_btnNowPlaying.SetPos(-g_iClientWidth/2+cx/2,g_iClientHeight/2-cy/2-ispace/2);
	m_btnNowPlaying.SetIcon(&m_iconNowPlayingUp,&m_iconNowPlayingDown);
	m_btnNowPlaying.LinkTextGL(pTextGL);
	m_btnNowPlaying.SetCaption(GetResourceString(IDS_TITLE_NOWPLAYING));
	m_btnNowPlaying.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnNowPlaying.SetCaptionColor(&cr_gray,&cr_w);

	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_SPANISH:
		m_btnNowPlaying.SetCaptionHeight(17);
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

	//UpdatePTYList();

	//

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	//set ini type:
	SetShowType(DAB_LIBRARY_SERVICE);

	m_pCurPlayingList = &m_boxServiceList;

	return TRUE;
}


void CALLBACK CPanelDABLibrary::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelDABLibrary *panel=(CPanelDABLibrary*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelDABLibrary::NotifyDabServiceReady(BOOL bReady)
{

}
void CPanelDABLibrary::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	RETAILMSG(DEBUG_DAB, (TEXT("MSG:%s: idControl[%d] wMsg[%X] wPara[%d]\r\n"), TEXT(__FUNCTION__), idControl, wMsg, wPara));

	switch (idControl)
	{
	case IDC_DAB_LIB_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_LIB_BTN_NOWPLAYING:
		{
			SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_RIGHT);
			PlaySoundEffect(0);
		}
		break;
		/*
	case IDC_DAB_LIB_BTN_RETURN:
		{
			switch(m_nDABLibBoxType)
			{
				case DAB_LIBRARY_SERVICE:
					{
						ReturnPrevClassService();
						UpdateServiceList();
						UpdateQueryTitle();
					}
					break;
				case DAB_LIBRARY_PTY_LIST:
					{
						m_nCurPTYType = PtyList_AllType;
						UpdatePTYListTitle();
					}
					break;
			}

//				if(m_iBrowserLevel<=1)
//					SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_RIGHT);
			PlaySoundEffect(0);
		}
		break;
		*/
		/*
	case IDC_DAB_LIB_BTN_CHANGE_QUERYMODE:
		{
			CycleClassService();
			UpdateServiceList();
			UpdateQueryTitle();
			PlaySoundEffect(0);
		}
		break;
		*/
	case IDC_DAB_LIB_BOX_LIST:
		{
			int index=wPara;
			UINT iditem=*(UINT*)lpPara;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
					SetShowType((DAB_LIBRARY_TYPE)iditem);

					if(index == DAB_LIBRARY_SERVICE)
					{	
						ArrowChange(DAB_LIBRARY_SERVICE);
					}
					else if(index == DAB_LIBRARY_PTY_LIST) 
					{
						ArrowChange(DAB_LIBRARY_PTY_LIST);
					}
					else if(index == DAB_LIBRARY_ENSEMBLE_LIST)
					{
						ArrowChange(DAB_LIBRARY_ENSEMBLE_LIST);
					}
					PlaySoundEffect(0);
				}
				break;
			}
#else
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
					SetShowType((DAB_LIBRARY_TYPE)iditem);
					PlaySoundEffect(0);
				}
				break;
			}
#endif		
		}
		break;
	case IDC_DAB_LIB_SERVICE_ITEM:
		{
			int index=wPara;
			UINT iditem=*(UINT*)lpPara;
			if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				PlayItemAllService(iditem);
				SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_RIGHT);
				PlaySoundEffect(0);
			}
		}
		break;
	case IDC_DAB_LIB_ENSEMBLE_ITEM:
		{
			int index=wPara;
			UINT iditem=*(UINT*)lpPara;
			if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				ShowEnsembleServiceDetail(TRUE,iditem);
				PlaySoundEffect(0);
			}
		}
		break;
	case IDC_DAB_LIB_ENSEMBLE_SERVICE_LIST_ITEM:
		{
			int index=wPara;
			UINT iditem=*(UINT*)lpPara;
			if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				PlayItemSubService(iditem);
				SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_RIGHT);
				PlaySoundEffect(0);
			}
		}
		break;
	case IDC_DAB_LIB_PTY_SERVICE_LIST_ITEM:
		{
			int index=wPara;
			UINT iditem=*(UINT*)lpPara;
			if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				if(iditem != -1)
				{
					PlayItemAllService(iditem);
					SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_RIGHT);
				}
				else
				{
					ShowPTYServiceDetail(FALSE);
				}
				PlaySoundEffect(0);
			}
		}
		break;
	case IDC_DAB_LIB_PTY_LIST_ITEM:
		{
			int index=wPara;
			UINT iditem=*(UINT*)lpPara;
			if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				/*
				m_nCurPTYType = index;
				if(m_nCurPTYType == PtyList_AllType)
				{
					UpdatePTYListTitle();
				}
				else
				{
					RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: MoveToIndex[%d]\r\n"),index));
					if(m_nCurPTYType != PtyList_AllType || m_nCurPTYType != PtyList_None)
					{
						DABApi_PTYSeekProc(m_nCurPTYType);
					}
					m_boxPTYList.MoveToIndex(m_boxPTYList.SearchID(index));
					UpdatePTYListTitle();
					//PlayItem(index);
				}
				*/

				//DABApi_PTYSeekProc(iditem);
				ShowPTYServiceDetail(TRUE,iditem);
				//SwitchPage(UI_CLASS_DAB,PAGE_DAB_MAIN,PAGE_SWITCH_ANI_RIGHT);
				PlaySoundEffect(0);
			}
			PlaySoundEffect(0);
		}
		break;
	}
}

void CPanelDABLibrary::UpdateQueryMode(BYTE nQueryMode)
{
	m_nQueryMode = nQueryMode;
}

const wchar_t* CPanelDABLibrary::GetIndexValue(INT index, BOOL bMoveIndex)
{
	if(bMoveIndex)
	{
		if(m_boxServiceList.MoveToIndex(index,0,FALSE))
			return m_boxServiceList.GetCurFocusItemValue();
		else
			return NULL;
	}
	else
	{
		return m_boxServiceList.GetIndexValue(index);
	}
}

const wchar_t* CPanelDABLibrary::GetCurValue()
{
	return m_boxServiceList.GetCurFocusItemValue();
}

int CPanelDABLibrary::GetCurIndex()
{
	return m_boxServiceList.GetCurIndex();
}
int CPanelDABLibrary::GetTotalServiceCnt()
{
	return m_boxServiceList.GetCount();
}
const wchar_t* CPanelDABLibrary::GetNextValue()
{
	m_boxServiceList.MoveToNext(TRUE,FALSE);
	return m_boxServiceList.GetCurFocusItemValue();
}
const wchar_t* CPanelDABLibrary::GetPreviousValue()
{
	m_boxServiceList.MoveToLast(TRUE,FALSE);
	return m_boxServiceList.GetCurFocusItemValue();
}
BOOL CPanelDABLibrary::PlayItemSubService(UINT idItem)
{
	BOOL bRet=FALSE;
	RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]:  idItem[%d]\r\n"),TEXT(__FUNCTION__), idItem));
	bRet = DABApi_ServiceDirectEntryProc(ListType_SubService, idItem);
	m_boxEnsembleServiceList.MoveToIndex(m_boxEnsembleServiceList.SearchID(idItem));
	return bRet;
}
BOOL CPanelDABLibrary::PlayItemAllService(UINT idItem)
{
	BOOL bRet = FALSE;
// 	if(m_idCurDeviceID == idItem)
// 	{
// 		RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: m_idCurDeviceID == idItem = %d\r\n"),TEXT(__FUNCTION__),idItem));
// 		return FALSE;
// 	}
// 	m_idCurDeviceID = idItem;

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]:  idItem[%d]\r\n"),TEXT(__FUNCTION__), idItem));
	bRet = DABApi_ServiceDirectEntryProc(ListType_AllService, idItem);
	m_boxServiceList.MoveToIndex(m_boxServiceList.SearchID(idItem));
	return bRet;
}

//	//搜索模式定义
//	typedef enum _dab_search_mode
//	{
//		AllService_Search, //在所有Service 中操作
//		PresetService_Search, //在所有Preset 中操作
//		CurrentService_Search, //在当前Service 中操作
//		Manual_Search //在Station 中操作
//	};

//	typedef enum _dab_list_type
//	{
//		ListType_AllService, //所有Service 列表
//		ListType_Ensembel, //所有Ensemble 列表
//		ListType_SubService, //指定Ensemble 下的Service 列表
//		ListType_PresetService, //当前波段的Preset 列表
//		ListType_PresetShortService, //当前波段的Preset short label --- add 20141008
//		ListType_CurrentService //当前Ensemble 下的Service 列表
//	};
BOOL CPanelDABLibrary::PlayPrevious()
{
	/*
	FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode(&m_nSearchMode));
	FUNC_CHECK_BOOL(DABApi_ChanUpProc(m_nSearchMode));

	Sleep(5);
	if(DABApi_GetFlag(DAB_ViewFlag_PlayInfoUpdate))
	{
		int iTotal;
		CM_String m_strService;
		CM_String m_strServiceCompare;

		if(!DABApi_SetFlag(DAB_ViewFlag_PlayInfoUpdate, 0))
			return FALSE;
		if(!DABApi_GetTotalOfListInfo(m_nQueryMode, 0, &iTotal))
			return FALSE;

		m_strService = DABApi_GetCurrChanInfo(ChnInfo_ServiceLabel);
		for(int i=0;i<iTotal;i++)
		{
			m_strServiceCompare = m_boxServiceList.GetIndexName(i);
			if(lstrcmpi(m_strServiceCompare.String(),m_strService.String()) == 0)
			{
				RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: MoveToIndex[%d]\r\n"),TEXT(__FUNCTION__), i));
				m_boxServiceList.MoveToIndex(i);
			}
		}
		RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: Done\r\n"),TEXT(__FUNCTION__)));

		return TRUE;
	}
*/
   	return FALSE;

//	return TRUE;
}

BOOL CPanelDABLibrary::PlayNext()
{
	/*
	FUNC_CHECK_BOOL(DABApi_GetCurrentSearchMode(&m_nSearchMode));
	FUNC_CHECK_BOOL(DABApi_ChanDownProc(m_nSearchMode));

	Sleep(5);
	if(DABApi_GetFlag(DAB_ViewFlag_PlayInfoUpdate))
	{
		int iTotal;
		CM_String m_strService;
		CM_String m_strServiceCompare;

		if(!DABApi_SetFlag(DAB_ViewFlag_PlayInfoUpdate, 0))
			return FALSE;
		if(!DABApi_GetTotalOfListInfo(m_nQueryMode, 0, &iTotal))
			return FALSE;

		m_strService = DABApi_GetCurrChanInfo(ChnInfo_ServiceLabel);
		for(int i=0;i<iTotal;i++)
		{
			m_strServiceCompare = m_boxServiceList.GetIndexName(i);
			if(lstrcmpi(m_strServiceCompare.String(),m_strService.String()) == 0)
			{
				RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: MoveToIndex[%d]\r\n"),TEXT(__FUNCTION__),i));
				m_boxServiceList.MoveToIndex(i);
			}
		}
		RETAILMSG(DEBUG_DAB, (TEXT("MSG:[%s]: Done\r\n"),TEXT(__FUNCTION__)));
		return TRUE;
	}
	*/
   return FALSE;

//	return TRUE;
}
//
//void CPanelDABLibrary::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
//{
//	//auto select device:
//	if(m_idCurDeviceID == 0)
//	{
//		if(m_boxServiceList.GetCount()>0)
//			PlayItem(m_boxServiceList.GetCurFocusItemID());
//	}
//	else
//	{
//		//move to current device:
//		m_boxServiceList.MoveToIndex(m_boxServiceList.SearchID(m_idCurDeviceID));
//	}
//}


void CPanelDABLibrary::EnterNextClassService()
{
	m_nQueryMode = ListType_CurrentService;
//		switch(m_nQueryMode)
//		{
//			case ListType_AllService:
//				m_nQueryMode = ListType_Ensembel;
//				break;
//			case ListType_Ensembel:
//				m_nQueryMode = ListType_SubService;
//				break;
//			case ListType_SubService:
//				m_nQueryMode = ListType_PresetService;
//				break;
//			case ListType_PresetService:
//				m_nQueryMode = ListType_CurrentService;
//				break;
//			case ListType_CurrentService:
//				m_nQueryMode = ListType_CurrentService;
//				break;
//			default:
//				m_nQueryMode = ListType_AllService	;
//				break;
//		}
}

void CPanelDABLibrary::ReturnPrevClassService()
{
	m_nQueryMode = ListType_Ensembel;
//		switch(m_nQueryMode)
//		{
//			case ListType_AllService:
//				m_nQueryMode = ListType_AllService;
//				break;
//			case ListType_Ensembel:
//				m_nQueryMode = ListType_AllService;
//				break;
//			case ListType_SubService:
//				m_nQueryMode = ListType_Ensembel;
//				break;
//			case ListType_PresetService:
//				m_nQueryMode = ListType_SubService;
//				break;
//			case ListType_CurrentService:
//				m_nQueryMode = ListType_PresetService;
//				break;
//			default:
//				m_nQueryMode = ListType_AllService;
//				break;
//		}
}

void CPanelDABLibrary::CycleClassService()
{
	switch(m_nQueryMode)
	{
		case ListType_AllService:
			m_nQueryMode = ListType_Ensembel;
			break;
		case ListType_Ensembel:
			m_nQueryMode = ListType_SubService;
			break;
		case ListType_SubService:
			m_nQueryMode = ListType_PresetService;
			break;
		case ListType_PresetService:
			m_nQueryMode = ListType_CurrentService;
			break;
		case ListType_CurrentService:
			m_nQueryMode = ListType_AllService;
			break;
		default:
			m_nQueryMode = ListType_AllService;
			break;
	}
}

void CPanelDABLibrary::ClearServiceBoxList()
{
	m_boxServiceList.ClearList();
}

void CPanelDABLibrary::ClearEnsembleBoxList()
{
	m_boxEnsembleList.ClearList();
}
void CPanelDABLibrary::UpdateSubServiceList(int idxEnsemble)
{
	int iTotal;
	CListBoxItemGL *pItem;
	const byte list_type=ListType_SubService;

	DABApi_GetTotalOfListInfo(list_type, idxEnsemble, &iTotal);

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:%s: idxEnsemble=%d, iTotal[%d]\r\n"), TEXT(__FUNCTION__),idxEnsemble,iTotal));
	m_boxEnsembleServiceList.ClearList();

	if(iTotal > 0)
	{
		for(int i=0;i<iTotal;i++)
		{
			pItem=new CListBoxItemGL(DABApi_GetNameOfListInfo(list_type, i ));
			pItem->SetIcon(&m_iconService,&m_iconServiceFocus);
			m_boxEnsembleServiceList.AddItem(pItem,i);
		}

		//move to now playing index:
		if(m_strCurrentService.String())
			m_boxEnsembleServiceList.MoveToIndex(m_boxEnsembleServiceList.SearchName(m_strCurrentService.String()));
	}
}
void CPanelDABLibrary::UpdateEnsembleList()
{
	int iTotal;
	CListBoxItemGL *pItem;
	const byte list_type=ListType_Ensembel;

	DABApi_GetTotalOfListInfo(list_type, 0, &iTotal);

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:%s:  iTotal[%d]\r\n"), TEXT(__FUNCTION__),iTotal));
	ClearEnsembleBoxList();

	if(iTotal > 0)
	{
		for(int i=0;i<iTotal;i++)
		{
			pItem=new CListBoxItemGL(DABApi_GetNameOfListInfo(list_type, i ));
			pItem->SetIcon(&m_iconService,&m_iconServiceFocus);
			m_boxEnsembleList.AddItem(pItem,i);
		}
	}
}
void CPanelDABLibrary::UpdatePtyServiceList(int iPty)
{
	int iTotal;
	CListBoxItemGL *pItem;
	const byte list_type=ListType_AllService;

	DABApi_GetTotalOfListInfo(list_type, 0, &iTotal);

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:%s: [%d]\r\n"), TEXT(__FUNCTION__),iPty));
	m_boxPTYServiceList.ClearList();

	if(iTotal > 0)
	{
		int cnt=0;
		for(int i=0;i<iTotal;i++)
		{
			byte pty;
			wchar_t *name = DABApi_GetNameOfListInfo(list_type, i , &pty);
			if(pty == iPty)
			{
				cnt++;
				pItem=new CListBoxItemGL(name);
				pItem->SetIcon(&m_iconService,&m_iconServiceFocus);
				m_boxPTYServiceList.AddItem(pItem,i);
				RETAILMSG(DEBUG_DAB, (TEXT("MSG:%s:  idx[%d],%s, pty=%d(%s)\r\n"), TEXT(__FUNCTION__),i,name,pty,ProgramTypeToString(ProgramType(pty))));
			}
		}

		if(cnt != 0)
		{
			//move to now playing index:
			if(m_strCurrentService.String())
				m_boxPTYServiceList.MoveToIndex(m_boxPTYServiceList.SearchName(m_strCurrentService.String()));
		}
		else
		{
			pItem=new CListBoxItemGL(GetResourceString(IDS_NO_LISTITEM));
			pItem->SetIcon(&m_iconService,&m_iconServiceFocus);
			m_boxPTYServiceList.AddItem(pItem,-1);
		}
	}
}
void CPanelDABLibrary::UpdateAllServiceList()
{
	int iTotal;
	CListBoxItemGL *pItem;
	const byte list_type=ListType_AllService;

	DABApi_GetTotalOfListInfo(list_type, 0, &iTotal);

	RETAILMSG(DEBUG_DAB, (TEXT("MSG:%s:  iTotal[%d]\r\n"), TEXT(__FUNCTION__),iTotal));
	ClearServiceBoxList();

	if(iTotal > 0)
	{
		for(int i=0;i<iTotal;i++)
		{
			byte pty;
			wchar_t *name = DABApi_GetNameOfListInfo(list_type, i , &pty);
			pItem=new CListBoxItemGL(name);
			pItem->SetIcon(&m_iconService,&m_iconServiceFocus);
			m_boxServiceList.AddItem(pItem,i);
			//RETAILMSG(DEBUG_DAB, (TEXT("MSG:%s:  idx[%d],%s, pty=%d(%s)\r\n"), TEXT(__FUNCTION__),i,name,pty,ProgramTypeToString(ProgramType(pty))));
		}

		//move to now playing index:
		if(m_strCurrentService.String())
			m_boxServiceList.MoveToIndex(m_boxServiceList.SearchName(m_strCurrentService.String()));
	}
}

void CPanelDABLibrary::UpdateQueryTitle()
{
	switch(m_nQueryMode)
	{
		case ListType_AllService:
			m_btnTitle.SetCaption(_T("All service"));
			break;
		case ListType_Ensembel:
			m_btnTitle.SetCaption(_T("All ensemble"));
			break;
		case ListType_SubService:
			m_btnTitle.SetCaption(_T("Sub service"));
			break;
		case ListType_PresetService:
			m_btnTitle.SetCaption(_T("Preset service"));
			break;
		case ListType_CurrentService:
			m_btnTitle.SetCaption(_T("Current service"));
			break;
	}
}

void CPanelDABLibrary::UpdatePTYListTitle()
{
	switch(m_nCurPTYType)
	{
		case PtyList_AllType:
		case PtyList_None:
			m_btnTitle.SetCaption(_T("All PTY"));
			break;
		case PtyList_News:
			m_btnTitle.SetCaption(_T("News"));
			break;
		case PtyList_Affairs:
			m_btnTitle.SetCaption(_T("Affairs"));
			break;
		case PtyList_Information:
			m_btnTitle.SetCaption(_T("Information"));
			break;
		case PtyList_Sport:
			m_btnTitle.SetCaption(_T("Sport"));
			break;
		case PtyList_Education:
			m_btnTitle.SetCaption(_T("Education"));
			break;
		case PtyList_Drama:
			m_btnTitle.SetCaption(_T("Drama"));
			break;
		case PtyList_Culture:
			m_btnTitle.SetCaption(_T("Culture"));
			break;
		case PtyList_Science:
			m_btnTitle.SetCaption(_T("Science"));
			break;
		case PtyList_Varied:
			m_btnTitle.SetCaption(_T("Varied"));
			break;
		case PtyList_PopMusic:
			m_btnTitle.SetCaption(_T("PopMusic"));
			break;
		case PtyList_RockMusic:
			m_btnTitle.SetCaption(_T("RockMusic"));
			break;
		case PtyList_EasyMusic:
			m_btnTitle.SetCaption(_T("EasyMusic"));
			break;
		case PtyList_LightClassical:
			m_btnTitle.SetCaption(_T("LightClassical"));
			break;
		case PtyList_ClassicalMusic:
			m_btnTitle.SetCaption(_T("ClassicalMusic"));
			break;
		case PtyList_OtherMusic:
			m_btnTitle.SetCaption(_T("OtherMusic"));
			break;
		case PtyList_Weather:
			m_btnTitle.SetCaption(_T("Weather"));
			break;
		case PtyList_Finance:
			m_btnTitle.SetCaption(_T("Finance"));
			break;
		case PtyList_Children:
			m_btnTitle.SetCaption(_T("Children's"));
			break;
		case PtyList_Factual:
			m_btnTitle.SetCaption(_T("Factual"));
			break;
		case PtyList_Religion:
			m_btnTitle.SetCaption(_T("Religion"));
			break;
		case PtyList_PhoneIn:
			m_btnTitle.SetCaption(_T("PhoneIn"));
			break;
		case PtyList_Travel:
			m_btnTitle.SetCaption(_T("Travel"));
			break;
		case PtyList_Leisure:
			m_btnTitle.SetCaption(_T("Leisure"));
			break;
		case PtyList_JazzAndBlues:
			m_btnTitle.SetCaption(_T("JazzAndBlues"));
			break;
		case PtyList_CountryMusic:
			m_btnTitle.SetCaption(_T("CountryMusic"));
			break;
		case PtyList_NationalMusic:
			m_btnTitle.SetCaption(_T("NationalMusic"));
			break;
		case PtyList_OldiesMusic:
			m_btnTitle.SetCaption(_T("OldiesMusic"));
			break;
		case PtyList_FolkMusic:
			m_btnTitle.SetCaption(_T("FolkMusic"));
			break;
		case PtyList_Documentary:
			m_btnTitle.SetCaption(_T("Documentary"));
			break;
		default:
			m_btnTitle.SetCaption(_T(""));
			break;
	}
}
void CPanelDABLibrary::UpdateScanStatus(BOOL bScaning)
{

}
bool CPanelDABLibrary::UpdateCurChannelInfo()
{
	wchar_t *name = DABApi_GetCurrChanInfo( ChnInfo_ServiceLabel );
	if(name && wcslen(name))
		m_strCurrentService = name;
	else
		m_strCurrentService = NULL;

	if(m_strCurrentService.String())
		m_boxServiceList.MoveToIndex(m_boxServiceList.SearchName(m_strCurrentService.String()));

	return (m_strCurrentService.String() != NULL) ;
}
void CPanelDABLibrary::ShowEnsembleServiceDetail(BOOL bShow,int idxEnsemble)
{
	m_boxEnsembleServiceList.ShowQuickSearchMode(FALSE);
	m_boxEnsembleServiceList.MoveToIndex(m_boxEnsembleServiceList.SearchID(idxEnsemble));
	m_boxEnsembleServiceList.Show(bShow);
	m_boxEnsembleList.EnableNotify(!bShow);
	m_boxEnsembleList.SetLuminance(bShow?0.6f:1.0f);

	if(bShow)
	{
		m_boxEnsembleServiceList.SetPos(g_iScreenWidth - LIST_BOX_ITEM_WIDTH/2 - 80,0);
		m_boxEnsembleServiceList.SetPos(LIST_BOX_ITEM_WIDTH/2 + 80,0,0,TRUE);
	}

	//update service:
	if(bShow)
	{
		DABApi_ServiceDirectEntryProc(ListType_Ensembel,idxEnsemble);
		UpdateSubServiceList(idxEnsemble);
	}
}
void CPanelDABLibrary::ShowPTYServiceDetail(BOOL bShow,int iPty)
{
	m_boxPTYServiceList.ShowQuickSearchMode(FALSE);
	m_boxPTYServiceList.MoveToIndex(m_boxPTYServiceList.GetCurIndex());
	m_boxPTYServiceList.Show(bShow);
	m_boxPTYList.EnableNotify(!bShow);
	m_boxPTYList.SetLuminance(bShow?0.6f:1.0f);

	if(bShow)
	{
		m_boxPTYServiceList.SetPos(g_iScreenWidth - LIST_BOX_ITEM_WIDTH/2 - 80,0);
		m_boxPTYServiceList.SetPos(LIST_BOX_ITEM_WIDTH/2 + 80,0,0,TRUE);

		UpdatePtyServiceList(iPty);
	}
}
void CPanelDABLibrary::SetShowType(DAB_LIBRARY_TYPE type)
{
// 	if(m_nDABLibBoxType==type)
// 		return;

	m_nDABLibBoxType=type;

	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: %d \r\n"), TEXT(__FUNCTION__), type));

	m_boxServiceList.Show(m_nDABLibBoxType==DAB_LIBRARY_SERVICE);
	m_boxPTYList.Show(m_nDABLibBoxType==DAB_LIBRARY_PTY_LIST);
	m_boxEnsembleList.Show(m_nDABLibBoxType==DAB_LIBRARY_ENSEMBLE_LIST);

	//reset:
	m_boxServiceList.ShowQuickSearchMode(FALSE);
	m_boxPTYList.ShowQuickSearchMode(FALSE);
	ShowPTYServiceDetail(FALSE);
	ShowEnsembleServiceDetail(FALSE);
	//

	if(type==DAB_LIBRARY_SERVICE)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)	
		m_DABboxList.MoveToIndex(DAB_LIBRARY_SERVICE); 
#else
		m_DABboxList.MoveToIndex(0);
#endif
		UpdateAllServiceList();
	}
	else if(type==DAB_LIBRARY_PTY_LIST)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)		
		m_DABboxList.MoveToIndex(DAB_LIBRARY_PTY_LIST);
#else
		m_DABboxList.MoveToIndex(1);
#endif

	}
	else if(type==DAB_LIBRARY_ENSEMBLE_LIST)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)	
		m_DABboxList.MoveToIndex(DAB_LIBRARY_ENSEMBLE_LIST);
#else
		m_DABboxList.MoveToIndex(2);
#endif
		UpdateEnsembleList();
	}
}

#if 0
BOOL CPanelDABLibrary::SavePresetData()
{
	FILE	*file = NULL;

	if(!m_bPresetListChanged)
		return FALSE;

	m_bPresetListChanged = FALSE;

	CM_String strPath=CM_String(g_strInstallPath)+CM_String(_T("Radio\\"));

	CreateDirectory(strPath.String(),NULL);

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\DABPreset.dat"));

	DWORD dwStart=GetTickCount();

	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: ++ %s\r\n"),TEXT(__FUNCTION__), strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"wb") )
	{
		return FALSE;
	}

	//
	DABServiceInfo info;
	memset(&info,0,sizeof(DABServiceInfo));
	info.dwSize=sizeof(DABServiceInfo);

	m_boxServiceList.m_listItems.Head();
	while (!m_boxServiceList.m_listItems.End())
	{
		info.dwFreq = m_boxServiceList.m_listItems.Get()->GetID();
		wcscpy_s(info.servicelabel, LONG_LABEL_LENGTH+1, m_boxServiceList.m_listItems.Get()->GetItemName());
		wcscpy_s(info.ensemblelabel, LONG_LABEL_LENGTH+1, m_boxServiceList.m_listItems.Get()->GetItemSubName())

		//
		fwrite(&info,info.dwSize,1,file);

		m_boxServiceList.m_listItems.Next();
	}

	fclose(file);

	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: --\n"),TEXT(__FUNCTION__)));

	return TRUE;
}

BOOL CPanelDABLibrary::GetPresetData()
{
	FILE					*file = NULL;
	int					fileSize = 0;
	BYTE				*localBuffer;
	BYTE				*cursor;

	DWORD dwStart=GetTickCount();

	CM_String strFile=CM_String(g_strInstallPath)+CM_String(_T("Radio\\DABPreset.dat"));

	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: ++ %s\r\n"),strFile.String()));

	if ( 0!=_wfopen_s(&file, strFile.String(), L"rb") )
	{
		RETAILMSG(DEBUG_DAB,(_T("ERR:[%s]: DAB preset data file not found,%s\n"), TEXT(__FUNCTION__), strFile.String()));
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
	DABServiceInfo info;
	int siInfo=sizeof(DABServiceInfo);

	while ((cursor-localBuffer)<=fileSize-siInfo)
	{
		memcpy(&info,cursor,siInfo);
		//
		AddPreset(info.servicelabel,info.ensemblelabel,dwFreq);

		cursor+=siInfo;
		count++;
	}


	delete[] localBuffer;

	RETAILMSG(DEBUG_DAB,(_T("MSG:[%s]: GetPresetData total count:%d, take %d ms\n"),count,GetTickCount()-dwStart));

	//reset
	m_bPresetListChanged = FALSE;

	return TRUE;
}

void CPanelDABLibrary::AddPreset(const wchar_t *pServicelabel,const wchar_t *pEnsemblelabel,UINT )
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
			swprintf_s(str,32,_T("%d.%02d MHz"),dwFreq/1000000,dwFreq/10000-dwFreq/1000000*100);
		}
		pItem=new CListBoxItemGL(str,RegionToString(idRegion),_T("FM"));
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
		pItem->ShowValue(FALSE);
		m_boxFavorite.AddItemByOrder(pItem,dwFreq);

		m_boxFM.SetIndexIcon2(dwFreq,&m_iconFavoriteFocus,&m_iconFavorite);

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
			swprintf_s(str,32,_T("%d%02d KHz"),dwFreq/100000,dwFreq/1000-dwFreq/100000*100);
		}

		pItem=new CListBoxItemGL(str,RegionToString(idRegion),_T("AM"));
		pItem->SetIcon(&m_iconFreq,&m_iconFreqFocus);
		pItem->SetIcon2(&m_iconDelete,&m_iconDeleteFocus);
		pItem->ShowValue(FALSE);
		m_boxFavorite.AddItemByOrder(pItem,dwFreq);

		m_boxAM.SetIndexIcon2(dwFreq,&m_iconFavoriteFocus,&m_iconFavorite);

		SendRadioCommand(RADIO_CMD_QUERY_FAVORITE_STATUS,m_dwCurFreq);
		m_bFavListChanged = TRUE;
	}
}

#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
void CPanelDABLibrary::ArrowChange(UINT index)
{
	int iconMarginR = 24;

	m_pServiceItem->SetIcon(&m_iconService, &m_iconServiceFocus, iconMarginR, 0);
	m_pEnsembleItem->SetIcon(&m_iconEnsemble, &m_iconEnsembleFocus, iconMarginR, 0);
	m_pPtyItem->SetIcon(&m_iconPTY, &m_iconPTYFocus, iconMarginR, 0);

	switch(index)
	{
	case DAB_LIBRARY_SERVICE:
		{	
			m_pServiceItem->SetIcon2(&m_ServiceArrowOn, &m_ServiceArrowOn, -14, 0);
			m_pEnsembleItem->SetIcon2(&m_EnsembleArrowOff, &m_EnsembleArrowOff, -14, 0);
			m_pPtyItem->SetIcon2(&m_PtyArrowOff, &m_PtyArrowOff, -14, 0);
		}
		break;
	case DAB_LIBRARY_ENSEMBLE_LIST:
		{
			m_pServiceItem->SetIcon2(&m_ServiceArrowOff, &m_ServiceArrowOff, -14, 0);
			m_pEnsembleItem->SetIcon2(&m_EnsembleArrowOn, &m_EnsembleArrowOn, -14, 0);
			m_pPtyItem->SetIcon2(&m_PtyArrowOff, &m_PtyArrowOff, -14, 0);
		}
		break;
	case DAB_LIBRARY_PTY_LIST:
		{
			m_pServiceItem->SetIcon2(&m_ServiceArrowOff, &m_ServiceArrowOff, -14, 0);
			m_pEnsembleItem->SetIcon2(&m_EnsembleArrowOff, &m_EnsembleArrowOff, -14, 0);
			m_pPtyItem->SetIcon2(&m_PtyArrowOn, &m_PtyArrowOn, -14, 0);
		}
		break;
	default:
		break;
	}
}
#endif



#endif //CVTE_EN_DAB
