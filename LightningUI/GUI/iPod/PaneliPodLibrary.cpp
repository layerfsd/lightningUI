#include "PaneliPodLibrary.h"
#include "../../resource.h"
#include "../Media/MediaCommon.h"


CPaneliPodLibrary::CPaneliPodLibrary(void)
{
	m_dwListCnt = 0;
	m_bPlayable = FALSE;
	m_iBrowserLevel = 0;

	for (int i=0;i<MEDIA_TYPE_NUMBER;i++)
	{
		m_idValidSrcList[i]=0;
	}
}
CPaneliPodLibrary::~CPaneliPodLibrary(void)
{

}

BOOL CPaneliPodLibrary::IsReady()
{
	return CPanelGL::IsReady();

}

void CPaneliPodLibrary::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
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

	//move to ipod index:
	if(!m_boxMenu.MoveToIndex(m_boxMenu.SearchID(MEDIA_TYPE_IPOD)))
	{
	}
}

void CPaneliPodLibrary::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);
}
void CPaneliPodLibrary::OnGestureMsg(UINT idGesture)
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
void CPaneliPodLibrary::Render()
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

	m_boxMenu.SetAlpha(m_fCurAlpha);
	m_boxMenu.Render();

	m_btnNowPlaying.SetAlpha(m_fCurAlpha);
	m_btnNowPlaying.Render();

	m_btnHome.SetAlpha(m_fCurAlpha);
	m_btnHome.Render();

	m_btnTitle.SetAlpha(m_fCurAlpha);
	m_btnTitle.Render();

	m_boxItems.SetAlpha(m_fCurAlpha);
	m_boxItems.Render();

	m_btnReturn.SetAlpha(m_fCurAlpha);
	m_btnReturn.Render();

	EndRender();
}

BOOL CPaneliPodLibrary::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);

	SIZE si;
	VIEW_STATE vs;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
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
	m_iconArrow.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_277));
	m_iconTrack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_246));
	m_iconArtist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_247));
	m_iconAlbum.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_287));
	m_iconGenre.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_280));
	m_iconPlaylist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_215));

	//m_iconTrack.SetShowScale(0.8f);
	//m_iconArtist.SetShowScale(0.8f);
	//m_iconAlbum.SetShowScale(0.8f);
	//m_iconGenre.SetShowScale(0.8f);
	//m_iconPlaylist.SetShowScale(0.8f);

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_204));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_205));

	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_280));
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_279));

	m_iconReturn.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_255));
	m_iconReturnFocus.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_254));


	//
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

	COLORGL cr_gray={128.0f/256.0f,129.0f/256.0f,132.0f/256.0f,1.0};
	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
	COLORGL cr_highlight={1.0f,1.0f,1.0f,1.0};

	int cx=LIST_BOX_ITEM_WIDTH;     //234    //240;
	int cy=LIST_BOX_ITEM_HEIGHT;    //100

	int ispace=0;

	LISTBOXSTRUCTURE box;

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
	box.idTexIndexBase=TEXID_289;//TEXID_068;//
	box.idTexIndexFocus=TEXID_242;//TEXID_251;//
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_285;
	box.bNoSlider=TRUE;
	box.bNoBorder=TRUE;	//TRUE;
	box.bGlow=FALSE;
	box.bNoHorSeperator=TRUE;
	box.idAlignType=LIST_ALIGN_LEFT;

	m_boxMenu.Initialize(IDC_IPODCORE_BOX_MENU,this,&box,pSkinManager);
	m_boxMenu.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
	m_boxMenu.SetPos(-g_iClientWidth/2+cx/2,0);
	m_boxMenu.LinkTextGL(pTextGL);
	m_boxMenu.Show(TRUE,FALSE);
	m_boxMenu.SetAlpha(1.0f);
	m_boxMenu.SetCaptioniHeight(18);
	m_boxMenu.SetCaptionColor(&cr_gray,&cr_w);

	//list:
	memset(&box,0,sizeof(box));
	//find param:
	count = 5;
	box.iSpacing=(g_iClientHeight - cy)/5;
	box.siClient.cx = g_iClientWidth-cx;	//g_iClientWidth-cx;
	box.siClient.cy = g_iClientHeight - cy;	//(box.iSpacing * count);
	box.siBorder.cx = g_iClientWidth-cx;	//g_iClientWidth-cx;
	box.siBorder.cy = g_iClientHeight - cy;	//g_iClientHeight-cy;//(box.iSpacing * count);
	box.idTexIndexBase=TEXID_282;
	box.idTexIndexFocus=TEXID_242;//TEXID_361;
	box.idTexIndexClick=TEXID_241;
	box.idTexBase=TEXID_285;
	box.idTexBtnUp=TEXID_282;
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

	//list:
	m_boxItems.Initialize(IDC_IPODCORE_BOX_LIBRARY,this,&box,pSkinManager);
	m_boxItems.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
	m_boxItems.SetPos(cx/2,-cy/2);
	m_boxItems.LinkTextGL(pTextGL);
	m_boxItems.Show(TRUE,FALSE);
	m_boxItems.SetAlpha(1.0f);
	m_boxItems.SetCaptioniHeight(22); //32,24,18
	m_boxItems.SetCaptionColor(&cr_gray,&cr_highlight);

	SIZE siTitle={g_iClientWidth-cx,cy};	//{g_iClientWidth-cx-cx/2,cy};
	m_btnTitle.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		m_pSkinManager->GetTexture(TEXID_283),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTitle.SetSize(&siTitle);
	m_btnTitle.SetPos(g_iClientWidth/2-siTitle.cx/2,g_iClientHeight/2-siTitle.cy/2);
	m_btnTitle.LinkTextGL(pTextGL);
	m_btnTitle.SetCaption(_T("iPod"));
	m_btnTitle.SetCaptionPos(BTN_CAPTION_LEFT,32);
	m_btnTitle.SetCaptionColor(&cr_gray,&cr_w);
	m_btnTitle.SetCaptionHeight(22);
	m_btnTitle.EnableClickAni(FALSE);

	SIZE sibtn2={122,78};
	//
	m_btnReturn.Initialize(IDC_IPODCORE_BTN_RETURN,this,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnReturn.SetSize(&sibtn2);
	m_btnReturn.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
	//m_btnReturn.SetPos(cx/2+(g_iClientWidth-cx)/5*2,g_iClientHeight/2-cy/2);
	m_btnReturn.SetPos(g_iClientWidth/2-sibtn2.cx/2-10,g_iClientHeight/2-cy/2-ispace/2);
	m_btnReturn.SetIcon(&m_iconReturn,&m_iconReturnFocus);
	m_btnReturn.SetIconPos(-sibtn2.cx/2+32,0);
	m_btnReturn.LinkTextGL(pTextGL);
	m_btnReturn.SetCaption(GetResourceString(IDS_RETURN));
	m_btnReturn.SetCaptionPos(BTN_CAPTION_LEFT,-sibtn2.cx/2+119,0);
	m_btnReturn.SetCaptionColor(&cr_gray,&cr_w);
	m_btnReturn.SetCaptionHeight(16);
	m_btnReturn.EnableClickAni(FALSE);
	m_btnReturn.EnableDrawBack(FALSE);

	m_strBrowseTitle[0]=_T("iPod");
	m_strBrowseTitle[1]=_T("iPod");


	SIZE sibtn={cx,cy};
	m_btnHome.Initialize(IDC_IPODCORE_BTN_HOME,this,
		m_pSkinManager->GetTexture(TEXID_288),NULL,
		m_pSkinManager->GetTexture(TEXID_242),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL);
	//m_btnHome.SetShowScale(0.9f);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2+ispace/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.SetIconPos(-cx/2+35,0);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,68);
	m_btnHome.SetCaptionColor(&cr_w,&cr_gray);
	m_btnHome.SetCaptionHeight(20);


	m_btnNowPlaying.Initialize(IDC_IPODCORE_BTN_NOWPLAYING,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnNowPlaying.SetShowScale(0.9f);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
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
	m_iconTrack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_283));
	m_iconArtist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_282));
	m_iconAlbum.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_281));
	m_iconGenre.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_206));
	m_iconPlaylist.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_401));

	m_iconTrack.SetShowScale(0.8f);
	m_iconArtist.SetShowScale(0.8f);
	m_iconAlbum.SetShowScale(0.8f);
	m_iconGenre.SetShowScale(0.8f);
	m_iconPlaylist.SetShowScale(0.8f);

	m_iconReturn.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_012));
    m_iconReturn.SetShowScale(0.6f);

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


// 	COLORGL cr_gray={177.0f/256.0f,131.0f/256.0f,107.0f/256.0f,1.0};
	COLORGL cr_w={1.0f,1.0f,1.0f,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_highlight={255.0f/255.0f,63.0f/255.0f,63.0f/255.0f,1.0};

	int cx=LIST_BOX_ITEM_WIDTH;     //234    //240;
	int cy=LIST_BOX_ITEM_HEIGHT;    //100

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

	m_boxMenu.Initialize(IDC_IPODCORE_BOX_MENU,this,&box,pSkinManager);
	m_boxMenu.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
	m_boxMenu.SetPos(-g_iClientWidth/2+cx/2,0);
	m_boxMenu.LinkTextGL(pTextGL);
	m_boxMenu.Show(TRUE,FALSE);
	m_boxMenu.SetAlpha(1.0f);
	m_boxMenu.SetCaptioniHeight(24);
	m_boxMenu.SetCaptionColor(&cr_gray,&cr_w);

// 	CListBoxItemGL *pItem;
// 	pItem=new CListBoxItemGL(GetResourceString(IDS_RETURN));
// 	pItem->SetIcon(&m_iconReturn);
// 	m_boxMenu.AddItem(pItem);
// 	pItem=new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
// 	pItem->SetIcon(&m_iconAlbum);
// 	m_boxMenu.AddItem(pItem);
// 	pItem=new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
// 	pItem->SetIcon(&m_iconArtist);
// 	m_boxMenu.AddItem(pItem);


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
	box.fIndexBaseOffset=60;

	//list:
	m_boxItems.Initialize(IDC_IPODCORE_BOX_LIBRARY,this,&box,pSkinManager);
	m_boxItems.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
	m_boxItems.SetMarginHor(38);
	m_boxItems.SetPos(cx/2,-cy/2);
	m_boxItems.LinkTextGL(pTextGL);
	m_boxItems.Show(TRUE,FALSE);
	m_boxItems.SetAlpha(1.0f);
	m_boxItems.SetCaptioniHeight(22); //32,24,18
	m_boxItems.SetCaptionColor(&cr_gray,&cr_highlight);


	SIZE sibtn={cx,cy+ispace};
	SIZE sibtn2={cx/2,cy+ispace};

	m_iconHomeUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_441));
	m_iconHomeDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_442));
	m_iconNowPlayingUp.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_433));
    m_iconNowPlayingUp.SetShowScale(0.8f);
	m_iconNowPlayingDown.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_434));
    m_iconNowPlayingDown.SetShowScale(0.8f);

	m_btnHome.Initialize(IDC_IPODCORE_BTN_HOME,this,m_pSkinManager->GetTexture(TEXID_451),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHome.SetSize(&sibtn);
	m_btnHome.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
	m_btnHome.SetPos(-g_iClientWidth/2+cx/2,-g_iClientHeight/2+cy/2+ispace/2);
	m_btnHome.SetIcon(&m_iconHomeUp,&m_iconHomeDown);
	m_btnHome.LinkTextGL(pTextGL);
	m_btnHome.SetCaption(GetResourceString(IDS_TITLE_HOME));
	m_btnHome.SetCaptionPos(BTN_CAPTION_LEFT,63);
	m_btnHome.SetCaptionColor(&cr_gray,&cr_w);
	m_btnHome.SetCaptionHeight(22);
	m_btnHome.SetIconPos(-cx/2+31,0);
	m_btnHome.EnableClickAni(FALSE);

	m_btnReturn.Initialize(IDC_IPODCORE_BTN_RETURN,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnReturn.SetSize(&sibtn2);
	m_btnReturn.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
	m_btnReturn.SetPos(g_iClientWidth/2-cx/4,g_iClientHeight/2-cy/2-ispace/2);
	m_btnReturn.SetIcon(&m_iconReturn/*,&m_iconNowPlayingDown*/);
	//m_btnReturn.LinkTextGL(pTextGL);
	//m_btnReturn.SetCaption(GetResourceString(IDS_RETURN));
	//m_btnReturn.SetCaptionPos(BTN_CAPTION_LEFT,88);
	//m_btnReturn.SetCaptionColor(&cr_gray,&cr_w);
	//m_btnReturn.SetCaptionHeight(28);
	//m_btnReturn.SetIconPos(-cx/2+46,0);
	//m_btnReturn.SetIconScale(1.4f,1.4f);
	m_btnReturn.EnableClickAni(FALSE);

	SIZE siTitle={g_iClientWidth-cx-cx/2,cy};
	m_btnTitle.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_252),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnTitle.SetSize(&siTitle);
	m_btnTitle.SetPos(cx/4+1,g_iClientHeight/2-cy/2-ispace/2);
	m_btnTitle.LinkTextGL(pTextGL);
	m_btnTitle.SetCaption(_T("iPod"));
	m_btnTitle.SetCaptionPos(BTN_CAPTION_LEFT,32);
	m_btnTitle.SetCaptionColor(&cr_gray,&cr_w);
	m_btnTitle.SetCaptionHeight(22);

	m_strBrowseTitle[0]=_T("iPod");
	m_strBrowseTitle[1]=_T("iPod");

	m_btnNowPlaying.Initialize(IDC_IPODCORE_BTN_NOWPLAYING,this,m_pSkinManager->GetTexture(TEXID_451),NULL,
		m_pSkinManager->GetTexture(TEXID_068),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNowPlaying.SetSize(&sibtn);
	m_btnNowPlaying.SetNotifyFunc(&CPaneliPodLibrary::OnNotify,this);
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
#endif
	return TRUE;
}


void CALLBACK CPaneliPodLibrary::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPaneliPodLibrary *panel=(CPaneliPodLibrary*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPaneliPodLibrary::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_IPODCORE_BTN_HOME:
		{
			SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER);
			PlaySoundEffect(0);
		}
		break;
	case IDC_IPODCORE_BTN_NOWPLAYING:
		{
			SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_RIGHT);
			PlaySoundEffect(0);
		}
		break;
	case IDC_IPODCORE_BTN_RETURN:
		{
			if(m_iBrowserLevel<=1)
				SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_RIGHT);
			else
				SendiPodCommand(IPOD_CMD_BROWSEUP,0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_IPODCORE_BOX_MENU:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;
			switch (wMsg)
			{
			case NOTIFY_DOUBLE_CLICK:
			case NOTIFY_SHORT_CLICK:
				{
					RETAILMSG(1,(_T("###ipod list select device:%d\r\n"),id_item));

					//SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,id_item);
					if(id_item==MEDIA_TYPE_A2DP)
					{
						SwitchPage(UI_CLASS_BT,PAGE_BT_A2DP,PAGE_SWITCH_ANI_RIGHT);
						SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,id_item);
					}
					else if(id_item!=MEDIA_TYPE_IPOD)
					{
						SwitchPage(UI_CLASS_MEDIA,PAGE_MEDIA_MAIN,PAGE_SWITCH_ANI_RIGHT);
						SendMediaCommand(MEDIA_CMD_SELECT_DEVICE,id_item);
					}

					PlaySoundEffect(0);
				}
				break;
			}
		}
		break;
	case IDC_IPODCORE_BOX_LIBRARY:
		{
			int index=wPara;
			UINT id_item=*(UINT*)lpPara;

			RETAILMSG(1,(_T("###idControl:%d, wMsg:%d, index:%d, id_item:%d, current level:%d\r\n"),idControl,wMsg,index,id_item,m_iBrowserLevel));
			if(wMsg == NOTIFY_SHORT_CLICK || wMsg == NOTIFY_DOUBLE_CLICK)
			{
				if(!m_bPlayable)
				{
					m_strBrowseTitle[m_iBrowserLevel]=m_boxItems.GetCurFocusItemName();
					m_btnTitle.SetCaption(m_strBrowseTitle[m_iBrowserLevel].String(),TRUE);
				}
				else
				{
					SwitchPage(UI_CLASS_IPOD,PAGE_IPOD_MAIN,PAGE_SWITCH_ANI_RIGHT);
				}
				SendiPodCommand(IPOD_CMD_SELECT_INDEX,index);
				PlaySoundEffect(0);
			}
		}
		break;
	}
}

void CPaneliPodLibrary::onRecordReady(UINT32 index, std::string record)
{
	wchar_t temp[MAX_STRING];
	UTF8TOUnicode((char*)record.c_str(),-1,temp,MAX_STRING);

	CListBoxItemGL *pItem;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	pItem = new CListBoxItemGL(temp,NULL,NULL,30,0);
	if(m_iBrowserLevel == 2)
	{
		if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_PLAYLISTS)) == 0)
			pItem->SetIcon(&m_iconPlaylist,NULL,14,0);
		else if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_ARTISTS)) == 0)
			pItem->SetIcon(&m_iconArtist,NULL,14,0);
		else if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_ALBUMS)) == 0)
			pItem->SetIcon(&m_iconAlbum,NULL,14,0);
		else if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_GENRES)) == 0)
			pItem->SetIcon(&m_iconGenre,NULL,14,0);
		else if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_COMPOSERS)) == 0)
			pItem->SetIcon(&m_iconArtist,NULL,14,0);
		else
			pItem->SetIcon(&m_iconTrack,NULL,14,0);
	}
	else
	{
		pItem->SetIcon(&m_iconTrack,NULL,14,0);
	}
#else
	pItem = new CListBoxItemGL(temp);
	if(m_iBrowserLevel == 2)
	{
		if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_PLAYLISTS)) == 0)
			pItem->SetIcon(&m_iconPlaylist);
		else if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_ARTISTS)) == 0)
			pItem->SetIcon(&m_iconArtist);
		else if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_ALBUMS)) == 0)
			pItem->SetIcon(&m_iconAlbum);
		else if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_GENRES)) == 0)
			pItem->SetIcon(&m_iconGenre);
		else if(lstrcmpi(m_btnTitle.GetCaption(), GetResourceString(IDS_LIBRARY_MENU_COMPOSERS)) == 0)
			pItem->SetIcon(&m_iconArtist);
		else
			pItem->SetIcon(&m_iconTrack);
	}
	else
	{
		pItem->SetIcon(&m_iconTrack);
	}
#endif
	m_boxItems.AddItem(pItem,index);

}
void CPaneliPodLibrary::onOpenFinish(DWORD cnt,BOOL bPlayable)
{
	m_dwListCnt = cnt;
	m_bPlayable = bPlayable;
}
void CPaneliPodLibrary::onRecordReady(UINT32 index, RootRecordID recordId)
{
	/*
	CListBoxItemGL *pItem;

	switch(recordId)
	{
	case ROOT_RECORD_ID_AUDIO:
		pItem = new CListBoxItemGL(_T("Audio"));
		m_boxLibrary.AddItem(pItem,index);
		break;
	case ROOT_RECORD_ID_VIDEO:
		pItem = new CListBoxItemGL(_T("Video"));
		m_boxLibrary.AddItem(pItem,index);
		break;
	case ROOT_RECORD_ID_TOTAL:
		pItem = new CListBoxItemGL(_T("Total"));
		m_boxLibrary.AddItem(pItem,index);
		break;
	default:
		break;
	}
	*/
}
void CPaneliPodLibrary::SetBrowserLevel(int iLevel)
{
	if(iLevel!=m_iBrowserLevel)
	{
		RETAILMSG(1,(_T("###iCPaneliPodLibrary::SetBrowserLevel : %d, current:%d\r\n"),iLevel,m_iBrowserLevel));

		if(iLevel<m_iBrowserLevel)
		{
			if(iLevel>0)
				m_btnTitle.SetCaption(m_strBrowseTitle[iLevel-1].String(),TRUE);
			else if(iLevel==0)
				m_btnTitle.SetCaption(_T("iPod"),TRUE);
		}

		m_iBrowserLevel = iLevel;
	}
}
void CPaneliPodLibrary::OnAvailableSourceChanged(WPARAM wParam, LPARAM lParam)
{
	if(wParam>MEDIA_TYPE_NUMBER-1)
		return;

	if(m_idValidSrcList[wParam] == lParam)
		return;

	m_idValidSrcList[wParam] = lParam;

	//refresh device list:
	m_boxMenu.ClearList();

	CListBoxItemGL *pItem;

	if(m_idValidSrcList[MEDIA_TYPE_SD1])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus,14,0);
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_SD));
		pItem->SetIcon(&m_iconDevSD,&m_iconDevSDFocus);
#endif
		m_boxMenu.AddItem(pItem,MEDIA_TYPE_SD1);
	}
	if(m_idValidSrcList[MEDIA_TYPE_USB1])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus,14,0);
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB));
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus);
#endif
		m_boxMenu.AddItem(pItem,MEDIA_TYPE_USB1);
	}
	if(m_idValidSrcList[MEDIA_TYPE_USB2])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus,14,0);
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_USB));
		pItem->SetIcon(&m_iconDevUSB,&m_iconDevUSBFocus);
#endif
		m_boxMenu.AddItem(pItem,MEDIA_TYPE_USB2);
	}
	if(m_idValidSrcList[MEDIA_TYPE_A2DP])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_BT),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDevBT,&m_iconDevBTFocus,14,0);
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_BT));
		pItem->SetIcon(&m_iconDevBT,&m_iconDevBTFocus);
#endif
		m_boxMenu.AddItem(pItem,MEDIA_TYPE_A2DP);
	}
	if(m_idValidSrcList[MEDIA_TYPE_IPOD])
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_IPOD),NULL,NULL,28,0);
		pItem->SetIcon(&m_iconDeviPod,&m_iconDeviPodFocus,14,0);
		//pItem->SetIcon2(&m_iconArrow,NULL,9,0);
#else
		pItem=new CListBoxItemGL(GetResourceString(IDS_DEVICE_IPOD));
		pItem->SetIcon(&m_iconDeviPod,&m_iconDeviPodFocus);
#endif
		m_boxMenu.AddItem(pItem,MEDIA_TYPE_IPOD);
	}

	//add blank item:
	if(m_boxMenu.GetCount()<4)
	{
		int iblank=4-m_boxMenu.GetCount();

		for (int i=0;i<iblank;i++)
		{
			pItem=new CListBoxItemGL(_T(" "));
			pItem->EnableClick(FALSE);
			m_boxMenu.AddItem(pItem,0);
		}
	}

	//move to ipod index:
	if(!m_boxMenu.MoveToIndex(m_boxMenu.SearchID(MEDIA_TYPE_IPOD)))
	{
	}
}
void CPaneliPodLibrary::onRecordReady(UINT32 index, TopLevelRecordID recordId)
{
	CListBoxItemGL *pItem;

	switch(recordId)
	{
	case TOP_LEVEL_RECORD_ID_PLAYLIST:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_PLAYLISTS),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconPlaylist,NULL,14,0);
#else
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_PLAYLISTS));
		pItem->SetIcon(&m_iconPlaylist);
#endif
		m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_ARTIST:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_ARTISTS),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconArtist,NULL,14,0);
#else
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_ARTISTS));
		pItem->SetIcon(&m_iconArtist);
#endif
		m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_ALBUM:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_ALBUMS),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconAlbum,NULL,14,0);
#else
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_ALBUMS));
		pItem->SetIcon(&m_iconAlbum);
#endif
		m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_GENRE:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_GENRES),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconGenre,NULL,14,0);
#else
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_GENRES));
		pItem->SetIcon(&m_iconGenre);
#endif
		m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_TRACK:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_TRACKS),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconTrack,NULL,14,0);
#else
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_TRACKS));
		pItem->SetIcon(&m_iconTrack);
#endif
		m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_COMPOSER:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_COMPOSERS),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconArtist,NULL,14,0);
#else
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_COMPOSERS));
		pItem->SetIcon(&m_iconArtist);
#endif
		m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_AUDIOBOOK:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_AUDIOBOOKS),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconTrack,NULL,14,0);
#else
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_AUDIOBOOKS));
		pItem->SetIcon(&m_iconTrack);
#endif
		m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_PODCAST:
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_PODCASTS),NULL,NULL,30,0);
		pItem->SetIcon(&m_iconTrack,NULL,14,0);
#else
		pItem = new CListBoxItemGL(GetResourceString(IDS_LIBRARY_MENU_PODCASTS));
		pItem->SetIcon(&m_iconTrack);
#endif
		m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_TOTAL:
		//pItem = new CListBoxItemGL(_T("Total"));
		//pItem->SetIcon(&m_iconTrack);
		//m_boxItems.AddItem(pItem,index);
		break;
	case TOP_LEVEL_RECORD_ID_INVALID:
		//pItem = new CListBoxItemGL(_T("Invalid"));
		//m_boxItems.AddItem(pItem,index);
		break;
	default:
		break;
	}
}
