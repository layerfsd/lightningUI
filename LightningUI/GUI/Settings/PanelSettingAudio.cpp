#include "PanelSettingAudio.h"
#include "../../resource.h"
extern BOOL						g_bKeyBeep;

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
typedef struct 
{
	UINT iBass;
	UINT iMid;
	UINT iTreb;
}EQParam;

static EQParam g_EQParam[] = {
	//EQ_NONE
	{7, 7, 7},
	//EQ_POP
	{4, 7, 5},
	//EQ_CLASSICAL
	{9, 7, 9},
	//EQ_ROCK
	{11, 7, 9},
	//EQ_JAZZ
	{9, 7, 6},
	//EQ_USER
	{7, 7, 7},
};
#endif

static int g_iPanelHeight;
CPanelSettingAudio *g_pSettingAudio = NULL; 

CPanelSettingAudio::CPanelSettingAudio(void)
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_bEQLock = FALSE;
#endif
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	m_pAudioCodec = NULL;
#endif
}
CPanelSettingAudio::~CPanelSettingAudio(void)
{
	m_iCreatePageSizeY = 0;
}

BOOL CPanelSettingAudio::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingAudio::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingAudio::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingAudio::Render()
{
	static CTexture *pTexBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_606):NULL;
	//static CTexture *pTexTitle=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_066):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();
#endif

    //KeyBeep
	m_tagTitleKeyBeep.SetAlpha(m_fCurAlpha);
	m_tagTitleKeyBeep.Render();
	m_sliKeyBeep.SetAlpha(m_fCurAlpha);
	m_sliKeyBeep.Render();

	//loudness:
	m_tagTitleLoudness.SetAlpha(m_fCurAlpha);
	m_tagTitleLoudness.Render();
	m_sliLoudness.SetAlpha(m_fCurAlpha);
	m_sliLoudness.Render();

#if CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT
	//Rear vol control:
	m_tagTitleRearVolControl.SetAlpha(m_fCurAlpha);
	m_tagTitleRearVolControl.Render();
	m_sliRearVolControl.SetAlpha(m_fCurAlpha);
	m_sliRearVolControl.Render();
#endif

#if CVTE_EN_AUDIO_SUBWOOFER
	//subwoofer:
	m_tagTitleSubwoofer.SetAlpha(m_fCurAlpha);
	m_tagTitleSubwoofer.Render();
	m_sliSubwoofer.SetAlpha(m_fCurAlpha);
	m_sliSubwoofer.Render();
#endif

#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
	m_btnSubwooferFreq.SetAlpha(m_fCurAlpha);
	m_btnSubwooferFreq.Render();
	m_tagValueSubwooferFreq.SetAlpha(m_fCurAlpha);
	m_tagValueSubwooferFreq.Render();
#endif

#if 0
	//volume adaption:
	m_tagTitleVolumeAdaption.SetAlpha(m_fCurAlpha);
	m_tagTitleVolumeAdaption.Render();
	m_sliVolumeAdaption.SetAlpha(m_fCurAlpha);
	m_sliVolumeAdaption.Render();
#endif

#if (CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL)
	m_tagTitleInternalAMP.SetAlpha(m_fCurAlpha);
	m_tagTitleInternalAMP.Render();
	m_sliInternalAMP.SetAlpha(m_fCurAlpha);
	m_sliInternalAMP.Render();
#endif

	//volume:
	m_tagTitleVolume.SetAlpha(m_fCurAlpha);
	m_tagTitleVolume.Render();

	m_tagTitleVolumeMultiMedia.SetAlpha(m_fCurAlpha);
	m_tagTitleVolumeMultiMedia.Render();
	m_sliVolumeMultiMedia.SetAlpha(m_fCurAlpha);
	m_sliVolumeMultiMedia.Render();

	m_tagTitleVolumePhone.SetAlpha(m_fCurAlpha);
	m_tagTitleVolumePhone.Render();
	m_sliVolumePhone.SetAlpha(m_fCurAlpha);
	m_sliVolumePhone.Render();

#if (CVTE_EN_NAVI || CVTE_EN_CARPLAY)
	m_tagTitleVolumeNavi.SetAlpha(m_fCurAlpha);
	m_tagTitleVolumeNavi.Render();
	m_sliVolumeNavi.SetAlpha(m_fCurAlpha);
	m_sliVolumeNavi.Render();
#endif

	//audio effect:
	m_tagAudioEffectBack.SetAlpha(m_fCurAlpha);
	m_tagAudioEffectBack.Render();
	m_tagTitleAudioEffect.SetAlpha(m_fCurAlpha);
	m_tagTitleAudioEffect.Render();

	m_btnEQClassic.SetAlpha(m_fCurAlpha);
	m_btnEQClassic.Render();
	m_btnEQRock.SetAlpha(m_fCurAlpha);
	m_btnEQRock.Render();
	m_btnEQPop.SetAlpha(m_fCurAlpha);
	m_btnEQPop.Render();
	m_btnEQJazz.SetAlpha(m_fCurAlpha);
	m_btnEQJazz.Render();
	m_btnEQUser.SetAlpha(m_fCurAlpha);
	m_btnEQUser.Render();

	//balance
	m_tagTitleBalance.SetAlpha(m_fCurAlpha);
	m_tagTitleBalance.Render();
	m_tagBalanceBack.SetAlpha(m_fCurAlpha);
	m_tagBalanceBack.Render();
	m_tagValueBalance.SetAlpha(m_fCurAlpha);
	m_tagValueBalance.Render();

	m_barBalance.SetAlpha(m_fCurAlpha);
	m_barBalance.Render();

	//EQ
	m_tagTitleEqualizer.SetAlpha(m_fCurAlpha);
	m_tagTitleEqualizer.Render();
	m_tagEqualizerBack.SetAlpha(m_fCurAlpha);
	m_tagEqualizerBack.Render();


	m_tagTitleHigh.SetAlpha(m_fCurAlpha);
	m_tagTitleHigh.Render();
	m_tagTitleMid.SetAlpha(m_fCurAlpha);
	m_tagTitleMid.Render();
	m_tagTitleLow.SetAlpha(m_fCurAlpha);
	m_tagTitleLow.Render();

	m_tagValueHigh.SetAlpha(m_fCurAlpha);
	m_tagValueHigh.Render();
	m_tagValueMid.SetAlpha(m_fCurAlpha);
	m_tagValueMid.Render();
	m_tagValueLow.SetAlpha(m_fCurAlpha);
	m_tagValueLow.Render();

	m_sliLow.SetAlpha(m_fCurAlpha);
	m_sliLow.Render();
	m_sliMid.SetAlpha(m_fCurAlpha);
	m_sliMid.Render();
	m_sliHigh.SetAlpha(m_fCurAlpha);
	m_sliHigh.Render();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_tagControlEQ.SetAlpha(m_fCurAlpha);
	m_tagControlEQ.Render();
	m_sliEQBalFadConctrol.SetAlpha(m_fCurAlpha);
	m_sliEQBalFadConctrol.Render();
#if 0
	m_tagEQLock.SetAlpha(m_fCurAlpha);
	m_tagEQLock.Render();
	m_btnLock.SetAlpha(m_fCurAlpha);
	m_btnLock.Render();
#endif
#endif

	EndRender();
}

BOOL CPanelSettingAudio::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	g_pSettingAudio = this;
	const int box_size_x=LIST_BOX_ITEM_WIDTH;

	//large size:
	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x=12;
	int margin_y = 6;
	int cx = g_iScreenWidth-LIST_BOX_ITEM_WIDTH;
	int cy = 280;
	int caption_h = 22;
	int caption2_h = 22;
	int value_Font = 20;
	int gridHeight = 80;
	int margin_left = 40;
	int space = 80;
	int captionMargin = 22;

	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
	{
		caption_h = 20;
		caption2_h = 16;
	}

	GLfloat py = g_iClientHeight/2-40;

	SIZE siTitle={cx,40};

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

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(cx, g_iClientHeight*4);
	m_tagBk.SetPos(0, -g_iClientHeight);

	//key beep:
	m_tagTitleKeyBeep.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleKeyBeep.SetSize(cx, gridHeight);
	m_tagTitleKeyBeep.SetCaption(GetResourceString(IDS_SETTING_KEY_BEEP));
	m_tagTitleKeyBeep.LinkTextGL(pTextGL);
	m_tagTitleKeyBeep.SetPos(0,py);
	m_tagTitleKeyBeep.SetCaptionHeight(caption_h);
	m_tagTitleKeyBeep.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleKeyBeep.SetCaptionColor(&cr_gray);

	m_sliKeyBeep.Initialize(IDC_SETTING_SLI_KEY_BEEP,this,&sli,pSkinManager);
	m_sliKeyBeep.SetRange(0,10);
	m_sliKeyBeep.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep?10:0);
	m_sliKeyBeep.SetPos(m_siCurSize.cx/2-70,py);
	m_sliKeyBeep.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);


	py-=space;

	//loudness:
	m_tagTitleLoudness.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleLoudness.SetSize(cx, gridHeight);
	m_tagTitleLoudness.SetCaption(GetResourceString(IDS_SETTING_LOUDNESS));
	m_tagTitleLoudness.LinkTextGL(pTextGL);
	m_tagTitleLoudness.SetPos(0,py);
	m_tagTitleLoudness.SetCaptionHeight(caption_h);
	m_tagTitleLoudness.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleLoudness.SetCaptionColor(&cr_gray);

	m_sliLoudness.Initialize(IDC_SETTING_SLI_LOUDNESS,this,&sli,pSkinManager);
	m_sliLoudness.SetRange(0,10);
	m_sliLoudness.SetValue(0);
	m_sliLoudness.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLoudness.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);



#if CVTE_EN_AUDIO_SUBWOOFER
	py-=space;

	//subwoofer:
	m_tagTitleSubwoofer.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleSubwoofer.SetSize(cx, gridHeight);
	m_tagTitleSubwoofer.SetCaption(GetResourceString(IDS_SETTING_SUBWOOFER));
	m_tagTitleSubwoofer.LinkTextGL(pTextGL);
	m_tagTitleSubwoofer.SetPos(0,py);
	m_tagTitleSubwoofer.SetCaptionHeight(caption_h);
	m_tagTitleSubwoofer.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleSubwoofer.SetCaptionColor(&cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_sliSubwoofer.Initialize(IDC_SETTING_SLI_SUBWOOFER,this,&sli,pSkinManager);
	m_sliSubwoofer.SetRange(0,14);
	m_sliSubwoofer.SetValue(0);
	m_sliSubwoofer.SetPos(m_siCurSize.cx/2-126,py);
	m_sliSubwoofer.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliSubwoofer.LinkTextGL(pTextGL);
	m_sliSubwoofer.SetCaptionColor(&cr_dark);
	m_sliSubwoofer.ShowValue(TRUE);
	m_sliSubwoofer.EnableMoveNotify(TRUE);
	m_sliSubwoofer.EnbaleClickOnlyThumb(TRUE);

#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
	py-=space;

	m_btnSubwooferFreq.Initialize(IDC_SETTING_BTN_SUBWOOFERFREQ,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		m_pSkinManager->GetTexture(TEXID_241),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSubwooferFreq.SetSize(cx, gridHeight);
	m_btnSubwooferFreq.SetCaption(GetResourceString(IDS_SETTING_AUDIO_SUBWOOFER_FREQ));
	m_btnSubwooferFreq.LinkTextGL(pTextGL);
	m_btnSubwooferFreq.SetPos(0,py);
	m_btnSubwooferFreq.SetCaptionHeight(caption_h);
	m_btnSubwooferFreq.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_btnSubwooferFreq.SetCaptionColor(&cr_gray,&cr_gray);
	m_btnSubwooferFreq.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);


	m_tagValueSubwooferFreq.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueSubwooferFreq.SetSize(cx,40);
	m_tagValueSubwooferFreq.SetCaption(_T("120HZ"));
	m_tagValueSubwooferFreq.LinkTextGL(pTextGL);
	m_tagValueSubwooferFreq.SetPos(0,py);
	m_tagValueSubwooferFreq.SetCaptionHeight(caption_h);
	m_tagValueSubwooferFreq.EnableDrawBack(FALSE);
	m_tagValueSubwooferFreq.SetCaptionPos(BTN_CAPTION_RIGHT,-20);
	m_tagValueSubwooferFreq.SetCaptionColor(&cr_gray);
#endif

#endif
	py-=space;

	//volume:
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_tagTitleVolume.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleVolume.SetSize(cx, gridHeight);
	m_tagTitleVolume.SetCaption(GetResourceString(IDS_SETTING_VOLUME));
	m_tagTitleVolume.LinkTextGL(pTextGL);
	m_tagTitleVolume.SetPos(0,py);
	m_tagTitleVolume.SetCaptionHeight(caption_h);
	m_tagTitleVolume.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleVolume.SetCaptionColor(&cr_gray,&cr_gray);

	py-=space;

	//volume multimedia:
	m_tagTitleVolumeMultiMedia.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleVolumeMultiMedia.SetSize(cx - margin_left, gridHeight);
	m_tagTitleVolumeMultiMedia.SetCaption(GetResourceString(IDS_TITLE_MEDIA));
	m_tagTitleVolumeMultiMedia.LinkTextGL(pTextGL);
	m_tagTitleVolumeMultiMedia.SetPos(margin_left/2, py);
	m_tagTitleVolumeMultiMedia.SetCaptionHeight(caption_h);
	m_tagTitleVolumeMultiMedia.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleVolumeMultiMedia.SetCaptionColor(&cr_gray);

	m_sliVolumeMultiMedia.Initialize(IDC_SETTING_SLI_VOLUME_MULTIMEDIA,this,&sli,pSkinManager);
	m_sliVolumeMultiMedia.SetRange(0,32);
	m_sliVolumeMultiMedia.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia);
	m_sliVolumeMultiMedia.SetPos(m_siCurSize.cx/2-126,py);
	m_sliVolumeMultiMedia.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumeMultiMedia.LinkTextGL(pTextGL);
	m_sliVolumeMultiMedia.SetCaptionColor(&cr_dark);
	m_sliVolumeMultiMedia.ShowValue(TRUE);
	m_sliVolumeMultiMedia.EnableMoveNotify(TRUE);
	m_sliVolumeMultiMedia.EnbaleClickOnlyThumb(TRUE);

	py-=space;

	//volume phone:
	m_tagTitleVolumePhone.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleVolumePhone.SetSize(cx - margin_left, gridHeight);
	m_tagTitleVolumePhone.SetCaption(GetResourceString(IDS_TITLE_PHONE));
	m_tagTitleVolumePhone.LinkTextGL(pTextGL);
	m_tagTitleVolumePhone.SetPos(margin_left/2,py);
	m_tagTitleVolumePhone.SetCaptionHeight(caption_h);
	m_tagTitleVolumePhone.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleVolumePhone.SetCaptionColor(&cr_gray);

	m_sliVolumePhone.Initialize(IDC_SETTING_SLI_VOLUME_PHONE,this,&sli,pSkinManager);
	m_sliVolumePhone.SetRange(0,32);
	m_sliVolumePhone.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolBluetooth);
	m_sliVolumePhone.SetPos(m_siCurSize.cx/2-126,py);
	m_sliVolumePhone.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumePhone.LinkTextGL(pTextGL);
	m_sliVolumePhone.SetCaptionColor(&cr_dark);
	m_sliVolumePhone.ShowValue(TRUE);
	m_sliVolumePhone.EnableMoveNotify(TRUE);
	m_sliVolumePhone.EnbaleClickOnlyThumb(TRUE);

#if (CVTE_EN_NAVI || CVTE_EN_CARPLAY)
	py-=space;

	//volume navi:
	m_tagTitleVolumeNavi.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleVolumeNavi.SetSize(cx - margin_left, gridHeight);
	m_tagTitleVolumeNavi.SetCaption(GetResourceString(IDS_SETTING_VOL_NAVI_SPEECH));
	m_tagTitleVolumeNavi.LinkTextGL(pTextGL);
	m_tagTitleVolumeNavi.SetPos(margin_left/2,py);
	m_tagTitleVolumeNavi.SetCaptionHeight(caption_h);
	m_tagTitleVolumeNavi.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleVolumeNavi.SetCaptionColor(&cr_gray);

	m_sliVolumeNavi.Initialize(IDC_SETTING_SLI_VOLUME_NAVI,this,&sli,pSkinManager);
	m_sliVolumeNavi.SetRange(0,32);
	m_sliVolumeNavi.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS);
	m_sliVolumeNavi.SetPos(m_siCurSize.cx/2-126,py);
	m_sliVolumeNavi.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumeNavi.LinkTextGL(pTextGL);
	m_sliVolumeNavi.SetCaptionColor(&cr_dark);
	m_sliVolumeNavi.ShowValue(TRUE);
	m_sliVolumeNavi.EnableMoveNotify(TRUE);
	m_sliVolumeNavi.EnbaleClickOnlyThumb(TRUE);
#endif
	//
	py-=space;

	//audio effect:
	m_tagTitleAudioEffect.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAudioEffect.SetSize(&siTitle);
	m_tagTitleAudioEffect.SetCaption(GetResourceString(IDS_SETTING_AUDIO_EFFECT));
	m_tagTitleAudioEffect.LinkTextGL(pTextGL);
	m_tagTitleAudioEffect.SetPos(0,py);
	m_tagTitleAudioEffect.SetCaptionHeight(caption_h);
	m_tagTitleAudioEffect.EnableDrawBack(FALSE);
	m_tagTitleAudioEffect.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleAudioEffect.SetCaptionColor(&cr_gray);

	py = py - space + 20;

	int ix=(g_iClientWidth-box_size_x-margin_x*2)/5;

	m_tagAudioEffectBack.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagAudioEffectBack.SetSize(cx, 100);
	m_tagAudioEffectBack.SetPos(0,py);

	m_btnEQClassic.Initialize(IDC_SETTING_BTN_EQ_CLASSIC,this,m_pSkinManager->GetTexture(TEXID_650),NULL,
		m_pSkinManager->GetTexture(TEXID_651),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQClassic.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQClassic.SetPos(-ix*2,py+10);
	m_btnEQClassic.LinkTextGL(pTextGL);
	m_btnEQClassic.SetCaption(GetResourceString(IDS_SETTING_CLASSIC));
	m_btnEQClassic.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQClassic.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQClassic.SetCaptionHeight(18);
	m_btnEQClassic.SetTouchRate(2.0f,2.0f);
	m_btnEQClassic.EnableClickAni(TRUE);

	m_btnEQPop.Initialize(IDC_SETTING_BTN_EQ_POP,this,m_pSkinManager->GetTexture(TEXID_656),NULL,
		m_pSkinManager->GetTexture(TEXID_657),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQPop.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQPop.SetPos(-ix,py+10);
	m_btnEQPop.LinkTextGL(pTextGL);
	m_btnEQPop.SetCaption(GetResourceString(IDS_SETTING_POP));
	m_btnEQPop.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQPop.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQPop.SetCaptionHeight(18);
	m_btnEQPop.SetTouchRate(2.0f,2.0f);
	m_btnEQPop.EnableClickAni(TRUE);

	m_btnEQRock.Initialize(IDC_SETTING_BTN_EQ_ROCK,this,m_pSkinManager->GetTexture(TEXID_658),NULL,
		m_pSkinManager->GetTexture(TEXID_659),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQRock.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQRock.SetPos(0,py+10);
	m_btnEQRock.LinkTextGL(pTextGL);
	m_btnEQRock.SetCaption(GetResourceString(IDS_SETTING_ROCK));
	m_btnEQRock.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQRock.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQRock.SetCaptionHeight(18);
	m_btnEQRock.SetTouchRate(2.0f,2.0f);
	m_btnEQRock.EnableClickAni(TRUE);

	m_btnEQJazz.Initialize(IDC_SETTING_BTN_EQ_JAZZ,this,m_pSkinManager->GetTexture(TEXID_654),NULL,
		m_pSkinManager->GetTexture(TEXID_655),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQJazz.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQJazz.SetPos(ix,py+10);
	m_btnEQJazz.LinkTextGL(pTextGL);
	m_btnEQJazz.SetCaption(GetResourceString(IDS_SETTING_JAZZ));
	m_btnEQJazz.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQJazz.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQJazz.SetCaptionHeight(18);
	m_btnEQJazz.SetTouchRate(2.0f,2.0f);
	m_btnEQJazz.EnableClickAni(TRUE);

	m_btnEQUser.Initialize(IDC_SETTING_BTN_EQ_USER,this,m_pSkinManager->GetTexture(TEXID_652),NULL,
		m_pSkinManager->GetTexture(TEXID_653),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQUser.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQUser.SetPos(ix*2,py+10);
	m_btnEQUser.LinkTextGL(pTextGL);
	m_btnEQUser.SetCaption(GetResourceString(IDS_SETTING_USER));
	m_btnEQUser.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQUser.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQUser.SetCaptionHeight(18);
	m_btnEQUser.SetTouchRate(2.0f,2.0f);
	m_btnEQUser.EnableClickAni(TRUE);

	py-=space;
	//equalize:
	m_tagTitleEqualizer.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleEqualizer.SetSize(cx, gridHeight);
	m_tagTitleEqualizer.SetCaption(GetResourceString(IDS_SETTING_EQ));
	m_tagTitleEqualizer.LinkTextGL(pTextGL);
	m_tagTitleEqualizer.SetPos(20,py);
	m_tagTitleEqualizer.SetCaptionHeight(20);
	m_tagTitleEqualizer.EnableDrawBack(FALSE);
	m_tagTitleEqualizer.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleEqualizer.SetCaptionColor(&cr_gray);

	////bal/fad:
	m_tagTitleBalance.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleBalance.SetSize(&siTitle);
	m_tagTitleBalance.SetCaption(GetResourceString(IDS_SETTING_BAL_FAD));
	m_tagTitleBalance.LinkTextGL(pTextGL);
	m_tagTitleBalance.SetPos(cx/2-10,py);
	m_tagTitleBalance.SetCaptionHeight(20);
	m_tagTitleBalance.EnableDrawBack(FALSE);
	m_tagTitleBalance.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleBalance.SetCaptionColor(&cr_gray);

	m_tagValueBalance.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueBalance.SetSize(80,40);
	m_tagValueBalance.SetCaption(_T("0/0"));
	m_tagValueBalance.LinkTextGL(pTextGL);
	m_tagValueBalance.SetPos(cx/2-80,py);
	m_tagValueBalance.SetCaptionHeight(18);
	m_tagValueBalance.EnableDrawBack(FALSE);
	m_tagValueBalance.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueBalance.SetCaptionColor(&cr_gray);

	py-=space;

	m_tagEqualizerBack.Initialize(0,this,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagEqualizerBack.SetSize(cx/2, cy);
	m_tagEqualizerBack.SetPos(cx/4, py - 10);
	m_tagEqualizerBack.EnableDrawBack(FALSE);

	py-=space;
	m_tagBalanceBack.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_670),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBalanceBack.SetSize(cx/2, cy);
	m_tagBalanceBack.SetPos(cx/4-20, py);

	int xPos = cx/7;

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_THICKNESS;
	sli.size.cy=cy-space-35;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_662;
	sli.idChannelTex=TEXID_661;
	sli.idThumbTex=TEXID_640;
	sli.dwType=ST_VER_NORMAL;
	sli.dwType=ST_VER_NORMAL;

	m_sliLow.Initialize(IDC_SETTING_SLI_BASS,this,&sli,pSkinManager);
	m_sliLow.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliLow.SetRange(0,14);
	m_sliLow.SetValue(7);
	m_sliLow.SetPos(-3*xPos, py);
	m_sliLow.SetAutoBackFill(TRUE);
	m_sliLow.EnableMoveNotify(TRUE);
	m_sliLow.EnbaleClickOnlyThumb(TRUE);

	m_sliMid.Initialize(IDC_SETTING_SLI_MID,this,&sli,pSkinManager);
	m_sliMid.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliMid.SetRange(0,14);
	m_sliMid.SetValue(7);
	m_sliMid.SetPos(-2*xPos, py);
	m_sliMid.SetAutoBackFill(TRUE);
	m_sliMid.EnableMoveNotify(TRUE);
	m_sliMid.EnbaleClickOnlyThumb(TRUE);

	m_sliHigh.Initialize(IDC_SETTING_SLI_TREBLE,this,&sli,pSkinManager);
	m_sliHigh.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliHigh.SetRange(0,14);
	m_sliHigh.SetValue(7);
	m_sliHigh.SetPos(-xPos, py);
	m_sliHigh.SetAutoBackFill(TRUE);
	m_sliHigh.EnableMoveNotify(TRUE);
	m_sliHigh.EnbaleClickOnlyThumb(TRUE);

	m_tagTitleLow.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLow.SetSize(&siTitle);
	m_tagTitleLow.SetCaption(GetResourceString(IDS_SETTING_LOW));
	m_tagTitleLow.LinkTextGL(pTextGL);
	m_tagTitleLow.SetPos(-3*xPos, py-cy/2+16);
	m_tagTitleLow.SetCaptionHeight(18);
	m_tagTitleLow.EnableDrawBack(FALSE);
	m_tagTitleLow.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleLow.SetCaptionColor(&cr_gray);

	m_tagTitleMid.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleMid.SetSize(&siTitle);
	m_tagTitleMid.SetCaption(GetResourceString(IDS_SETTING_MID));
	m_tagTitleMid.LinkTextGL(pTextGL);
	m_tagTitleMid.SetPos(-2*xPos, py-cy/2+16);
	m_tagTitleMid.SetCaptionHeight(18);
	m_tagTitleMid.EnableDrawBack(FALSE);
	m_tagTitleMid.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleMid.SetCaptionColor(&cr_gray);

	m_tagTitleHigh.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleHigh.SetSize(&siTitle);
	m_tagTitleHigh.SetCaption(GetResourceString(IDS_SETTING_HIGH));
	m_tagTitleHigh.LinkTextGL(pTextGL);
	m_tagTitleHigh.SetPos(-xPos, py-cy/2+16);
	m_tagTitleHigh.SetCaptionHeight(18);
	m_tagTitleHigh.EnableDrawBack(FALSE);
	m_tagTitleHigh.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleHigh.SetCaptionColor(&cr_gray);


	m_tagValueLow.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueLow.SetSize(&siTitle);
	m_tagValueLow.SetCaption(_T("0"));
	m_tagValueLow.LinkTextGL(pTextGL);
	m_tagValueLow.SetPos(-3*xPos,py+122);
	m_tagValueLow.SetCaptionHeight(16);
	m_tagValueLow.EnableDrawBack(FALSE);
	m_tagValueLow.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueLow.SetCaptionColor(&cr_gray);

	m_tagValueMid.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueMid.SetSize(&siTitle);
	m_tagValueMid.SetCaption(_T("0"));
	m_tagValueMid.LinkTextGL(pTextGL);
	m_tagValueMid.SetPos(-2*xPos,py+122);
	m_tagValueMid.SetCaptionHeight(16);
	m_tagValueMid.EnableDrawBack(FALSE);
	m_tagValueMid.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueMid.SetCaptionColor(&cr_gray);

	m_tagValueHigh.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueHigh.SetSize(&siTitle);
	m_tagValueHigh.SetCaption(_T("0"));
	m_tagValueHigh.LinkTextGL(pTextGL);
	m_tagValueHigh.SetPos(-xPos,py+122);
	m_tagValueHigh.SetCaptionHeight(16);
	m_tagValueHigh.EnableDrawBack(FALSE);
	m_tagValueHigh.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueHigh.SetCaptionColor(&cr_gray);


	//bal/fad:
	m_barBalance.Initialize(IDC_SETTING_BAR_AUDIO_BALANCE,this,pSkinManager,pTextGL);
	m_barBalance.SetSize(cx/2, cy);
	m_barBalance.SetMargin(space);
	m_barBalance.SetPos(cx/4-20,py); 
	m_barBalance.SetRange(-7,-7,7,7);
	m_barBalance.SetNotifyFunc(CPanelSettingAudio::OnNotify,this);

	m_iCreatePageSizeY = m_siCurSize.cy;

	py-=cy/2;

#else //CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={0.0f/256.0f,144.0f/256.0f,255.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x=12;
	int cx=(g_iClientWidth-box_size_x-margin_x*2)/2;
	int cy=cx;
    int tag_h = LIST_BOX_TAG_HEIGHT;
    int caption_h = 22;
    int caption2_h = 18;

    if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
    {
        caption_h = 20;
        caption2_h = 16;
    }

	GLfloat py=g_iClientHeight/2-50;

	SIZE siTitle={cx,40};

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

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_tagControlEQ.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagControlEQ.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagControlEQ.SetCaption(GetResourceString(IDS_SETTING_EQ));
	m_tagControlEQ.LinkTextGL(pTextGL);
	m_tagControlEQ.SetPos(0,py);
	m_tagControlEQ.SetCaptionHeight(caption_h);
	m_tagControlEQ.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagControlEQ.SetCaptionColor(&cr_gray);

	m_sliEQBalFadConctrol.Initialize(IDC_SETTING_SLI_EQBALFAD_SWITCH,this,&sli,pSkinManager);
	m_sliEQBalFadConctrol.SetRange(0,10);
	m_sliEQBalFadConctrol.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bEQSwitch?10:0);
	m_sliEQBalFadConctrol.SetPos(m_siCurSize.cx/2-70,py);
	m_sliEQBalFadConctrol.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

	py-=tag_h;
	
#if 0
	m_tagEQLock.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagEQLock.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_SPANISH:
		m_tagEQLock.SetCaption(_T("Cerradura ecualizador"));
		break;
	default:
		m_tagEQLock.SetCaption(_T("Equalizer Lock"));
		break;
	}
	m_tagEQLock.LinkTextGL(pTextGL);
	m_tagEQLock.SetPos(0,py);
	m_tagEQLock.SetCaptionHeight(caption_h);
	m_tagEQLock.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagEQLock.SetCaptionColor(&cr_gray);

	//Lock
	m_iconLock.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_829));
	m_iconUnLock.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_830));

	m_btnLock.Initialize(IDC_SETTING_SLI_EQ_LOCK,this,NULL,NULL,
		NULL,NULL,NULL,NULL,NULL,NULL);
	m_btnLock.SetTouchRate(2.0f,1.5f);
	m_btnLock.SetShowScale(0.65f);
	m_btnLock.EnableDrawBack(FALSE);
	m_btnLock.SetIcon(&m_iconUnLock, &m_iconLock);
	m_btnLock.SetPos((g_iClientWidth-box_size_x-margin_x*2)/2-60,py);
	m_btnLock.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

	py-=tag_h;

#endif

	//equalize:
	m_tagTitleEqualizer.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleEqualizer.SetSize(&siTitle);
	m_tagTitleEqualizer.SetCaption(GetResourceString(IDS_SETTING_EQ));
	m_tagTitleEqualizer.LinkTextGL(pTextGL);
	m_tagTitleEqualizer.SetPos(-cx/2-10 +12,py);
	m_tagTitleEqualizer.SetCaptionHeight(20);
	m_tagTitleEqualizer.EnableDrawBack(FALSE);
	m_tagTitleEqualizer.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleEqualizer.SetCaptionColor(&cr_dark);

	//bal/fad:
	m_tagTitleBalance.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleBalance.SetSize(&siTitle);
	m_tagTitleBalance.SetCaption(GetResourceString(IDS_SETTING_BAL_FAD));
	m_tagTitleBalance.LinkTextGL(pTextGL);
	m_tagTitleBalance.SetPos(cx/2-10,py);
	m_tagTitleBalance.SetCaptionHeight(20);
	m_tagTitleBalance.EnableDrawBack(FALSE);
	m_tagTitleBalance.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleBalance.SetCaptionColor(&cr_dark);

	m_tagValueBalance.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueBalance.SetSize(&siTitle);
	m_tagValueBalance.SetCaption(_T("0/0"));
	m_tagValueBalance.LinkTextGL(pTextGL);
	m_tagValueBalance.SetPos(cx/2+10 -12,py);
	m_tagValueBalance.SetCaptionHeight(18);
	m_tagValueBalance.EnableDrawBack(FALSE);
	m_tagValueBalance.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueBalance.SetCaptionColor(&cr_dark);

	py-=(20+cy/2);

	m_tagEqualizerBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagEqualizerBack.SetSize(cx,cy);
	m_tagEqualizerBack.SetPos(-cx/2,py);

	m_tagBalanceBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_670),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagBalanceBack.SetSize(cx,cy);
	m_tagBalanceBack.SetPos(cx/2,py);
	
	//slider
	//SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_THICKNESS;
	sli.size.cy=cy-tag_h-40;//256;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_662;
	sli.idChannelTex=TEXID_661;
	sli.idThumbTex=TEXID_640;
	sli.dwType=ST_VER_NORMAL;

	m_sliLow.Initialize(IDC_SETTING_SLI_BASS,this,&sli,pSkinManager);
	m_sliLow.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliLow.SetRange(0,14);
	m_sliLow.SetValue(7);
	m_sliLow.SetPos(-cx/2-cx/3,py);
	m_sliLow.SetAutoBackFill(TRUE);
	m_sliLow.EnableMoveNotify(TRUE);
	//m_sliLow.LinkTextGL(pTextGL);
	//m_sliLow.SetCaptionColor(&cr_gray);
	//m_sliLow.ShowValue(TRUE);
	m_sliLow.EnbaleClickOnlyThumb(TRUE);

	m_sliMid.Initialize(IDC_SETTING_SLI_MID,this,&sli,pSkinManager);
	m_sliMid.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliMid.SetRange(0,14);
	m_sliMid.SetValue(7);
	m_sliMid.SetPos(-cx/2,py);
	m_sliMid.SetAutoBackFill(TRUE);
	m_sliMid.EnableMoveNotify(TRUE);
	//m_sliMid.LinkTextGL(pTextGL);
	//m_sliMid.SetCaptionColor(&cr_gray);
	//m_sliMid.ShowValue(TRUE);
	m_sliMid.EnbaleClickOnlyThumb(TRUE);

	m_sliHigh.Initialize(IDC_SETTING_SLI_TREBLE,this,&sli,pSkinManager);
	m_sliHigh.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliHigh.SetRange(0,14);
	m_sliHigh.SetValue(7);
	m_sliHigh.SetPos(-cx/2+cx/3,py);
	m_sliHigh.SetAutoBackFill(TRUE);
	m_sliHigh.EnableMoveNotify(TRUE);
	//m_sliHigh.LinkTextGL(pTextGL);
	//m_sliHigh.SetCaptionColor(&cr_gray);
	//m_sliHigh.ShowValue(TRUE);
	m_sliHigh.EnbaleClickOnlyThumb(TRUE);

	m_tagTitleLow.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLow.SetSize(&siTitle);
	m_tagTitleLow.SetCaption(GetResourceString(IDS_SETTING_LOW));
	m_tagTitleLow.LinkTextGL(pTextGL);
	m_tagTitleLow.SetPos(-cx/2-cx/3,py-cy/2+16);
	m_tagTitleLow.SetCaptionHeight(18);
	m_tagTitleLow.EnableDrawBack(FALSE);
	m_tagTitleLow.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleLow.SetCaptionColor(&cr_gray);

	m_tagTitleMid.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleMid.SetSize(&siTitle);
	m_tagTitleMid.SetCaption(GetResourceString(IDS_SETTING_MID));
	m_tagTitleMid.LinkTextGL(pTextGL);
	m_tagTitleMid.SetPos(-cx/2,py-cy/2+16);
	m_tagTitleMid.SetCaptionHeight(18);
	m_tagTitleMid.EnableDrawBack(FALSE);
	m_tagTitleMid.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleMid.SetCaptionColor(&cr_gray);

	m_tagTitleHigh.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleHigh.SetSize(&siTitle);
	m_tagTitleHigh.SetCaption(GetResourceString(IDS_SETTING_HIGH));
	m_tagTitleHigh.LinkTextGL(pTextGL);
	m_tagTitleHigh.SetPos(-cx/2+cx/3,py-cy/2+16);
	m_tagTitleHigh.SetCaptionHeight(18);
	m_tagTitleHigh.EnableDrawBack(FALSE);
	m_tagTitleHigh.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleHigh.SetCaptionColor(&cr_gray);


	m_tagValueLow.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueLow.SetSize(&siTitle);
	m_tagValueLow.SetCaption(_T("0"));
	m_tagValueLow.LinkTextGL(pTextGL);
	m_tagValueLow.SetPos(-cx/2-cx/3,py+122);
	m_tagValueLow.SetCaptionHeight(16);
	m_tagValueLow.EnableDrawBack(FALSE);
	m_tagValueLow.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueLow.SetCaptionColor(&cr_gray);

	m_tagValueMid.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueMid.SetSize(&siTitle);
	m_tagValueMid.SetCaption(_T("0"));
	m_tagValueMid.LinkTextGL(pTextGL);
	m_tagValueMid.SetPos(-cx/2,py+122);
	m_tagValueMid.SetCaptionHeight(16);
	m_tagValueMid.EnableDrawBack(FALSE);
	m_tagValueMid.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueMid.SetCaptionColor(&cr_gray);

	m_tagValueHigh.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueHigh.SetSize(&siTitle);
	m_tagValueHigh.SetCaption(_T("0"));
	m_tagValueHigh.LinkTextGL(pTextGL);
	m_tagValueHigh.SetPos(-cx/2+cx/3,py+122);
	m_tagValueHigh.SetCaptionHeight(16);
	m_tagValueHigh.EnableDrawBack(FALSE);
	m_tagValueHigh.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueHigh.SetCaptionColor(&cr_gray);


	//bal/fad:
	m_barBalance.Initialize(IDC_SETTING_BAR_AUDIO_BALANCE,this,pSkinManager,pTextGL);
	m_barBalance.SetSize(/*360*/cx+10,/*360*/cy+10);
	m_barBalance.SetMargin(tag_h);
	m_barBalance.SetPos(cx/2,py);
	m_barBalance.SetRange(-7,-7,7,7);
	m_barBalance.SetNotifyFunc(CPanelSettingAudio::OnNotify,this);

	m_iCreatePageSizeY = m_siCurSize.cy;

	SetEQLocked(!m_pSkinManager->GetConfig()->GetCurParam()->bEQSwitch);

	SetEQSwitch(m_pSkinManager->GetConfig()->GetCurParam()->bEQSwitch);

	py-=(20+cy/2+20);

	//audio effect:
	m_tagTitleAudioEffect.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAudioEffect.SetSize(&siTitle);
// 	m_tagTitleAudioEffect.SetCaption(GetResourceString(IDS_SETTING_AUDIO_EFFECT));
	m_tagTitleAudioEffect.SetCaption(_T("Preset EQ"));
	m_tagTitleAudioEffect.LinkTextGL(pTextGL);
	m_tagTitleAudioEffect.SetPos(-cx/2-10+12,py);
	m_tagTitleAudioEffect.SetCaptionHeight(20);
	m_tagTitleAudioEffect.EnableDrawBack(FALSE);
	m_tagTitleAudioEffect.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleAudioEffect.SetCaptionColor(&cr_dark);

	py-=70;

	int ix=(g_iClientWidth-box_size_x-margin_x*2)/5;

	m_tagAudioEffectBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAudioEffectBack.SetSize((g_iClientWidth-box_size_x-margin_x*2),100);
	m_tagAudioEffectBack.SetPos(0,py);

	m_btnEQClassic.Initialize(IDC_SETTING_BTN_EQ_CLASSIC,this,m_pSkinManager->GetTexture(TEXID_650),NULL,
		m_pSkinManager->GetTexture(TEXID_651),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQClassic.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQClassic.SetPos(-ix*2,py+10);
	m_btnEQClassic.LinkTextGL(pTextGL);
	m_btnEQClassic.SetCaption(GetResourceString(IDS_SETTING_CLASSIC));
	m_btnEQClassic.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQClassic.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQClassic.SetCaptionHeight(18);
	m_btnEQClassic.SetTouchRate(2.0f,2.0f);
	m_btnEQClassic.EnableClickAni(TRUE);

	m_btnEQPop.Initialize(IDC_SETTING_BTN_EQ_POP,this,m_pSkinManager->GetTexture(TEXID_656),NULL,
		m_pSkinManager->GetTexture(TEXID_657),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQPop.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQPop.SetPos(-ix,py+10);
	m_btnEQPop.LinkTextGL(pTextGL);
	m_btnEQPop.SetCaption(GetResourceString(IDS_SETTING_POP));
	m_btnEQPop.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQPop.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQPop.SetCaptionHeight(18);
	m_btnEQPop.SetTouchRate(2.0f,2.0f);
	m_btnEQPop.EnableClickAni(TRUE);

	m_btnEQRock.Initialize(IDC_SETTING_BTN_EQ_ROCK,this,m_pSkinManager->GetTexture(TEXID_658),NULL,
		m_pSkinManager->GetTexture(TEXID_659),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQRock.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQRock.SetPos(0,py+10);
	m_btnEQRock.LinkTextGL(pTextGL);
	m_btnEQRock.SetCaption(GetResourceString(IDS_SETTING_ROCK));
	m_btnEQRock.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQRock.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQRock.SetCaptionHeight(18);
	m_btnEQRock.SetTouchRate(2.0f,2.0f);
	m_btnEQRock.EnableClickAni(TRUE);

	m_btnEQJazz.Initialize(IDC_SETTING_BTN_EQ_JAZZ,this,m_pSkinManager->GetTexture(TEXID_654),NULL,
		m_pSkinManager->GetTexture(TEXID_655),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQJazz.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQJazz.SetPos(ix,py+10);
	m_btnEQJazz.LinkTextGL(pTextGL);
	m_btnEQJazz.SetCaption(GetResourceString(IDS_SETTING_JAZZ));
	m_btnEQJazz.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQJazz.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQJazz.SetCaptionHeight(18);
	m_btnEQJazz.SetTouchRate(2.0f,2.0f);
	m_btnEQJazz.EnableClickAni(TRUE);

	m_btnEQUser.Initialize(IDC_SETTING_BTN_EQ_USER,this,m_pSkinManager->GetTexture(TEXID_652),NULL,
		m_pSkinManager->GetTexture(TEXID_653),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQUser.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQUser.SetPos(ix*2,py+10);
	m_btnEQUser.LinkTextGL(pTextGL);
	m_btnEQUser.SetCaption(GetResourceString(IDS_SETTING_USER));
	m_btnEQUser.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQUser.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQUser.SetCaptionHeight(18);
	m_btnEQUser.SetTouchRate(2.0f,2.0f);
	m_btnEQUser.EnableClickAni(TRUE);

	py-=86;

	//key beep:
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

	m_tagTitleKeyBeep.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleKeyBeep.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleKeyBeep.SetCaption(GetResourceString(IDS_SETTING_KEY_BEEP));
	m_tagTitleKeyBeep.LinkTextGL(pTextGL);
	m_tagTitleKeyBeep.SetPos(0,py);
	m_tagTitleKeyBeep.SetCaptionHeight(caption_h);
	m_tagTitleKeyBeep.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleKeyBeep.SetCaptionColor(&cr_gray);

	m_sliKeyBeep.Initialize(IDC_SETTING_SLI_KEY_BEEP,this,&sli,pSkinManager);
	m_sliKeyBeep.SetRange(0,10);
	m_sliKeyBeep.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep?10:0);
	m_sliKeyBeep.SetPos(m_siCurSize.cx/2-70,py);
	m_sliKeyBeep.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

	#if (CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL)
	py-=tag_h;

	m_tagTitleInternalAMP.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleInternalAMP.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleInternalAMP.SetCaption(_T("Internal Amp"));
	m_tagTitleInternalAMP.LinkTextGL(pTextGL);
	m_tagTitleInternalAMP.SetPos(0,py);
	m_tagTitleInternalAMP.SetCaptionHeight(caption_h);
	m_tagTitleInternalAMP.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleInternalAMP.SetCaptionColor(&cr_gray);

	m_sliInternalAMP.Initialize(IDC_SETTING_SLI_DISABLE_INTERNAL_AMP,this,&sli,pSkinManager);
	m_sliInternalAMP.SetRange(0,10);
	m_sliInternalAMP.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep?10:0);
	m_sliInternalAMP.SetPos(m_siCurSize.cx/2-70,py);
	m_sliInternalAMP.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

	#endif

	py-=tag_h;

	/*
	//mute:
	m_tagTitleMute.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleMute.SetSize((g_iClientWidth-box_size_x-margin_x*2),80);
	m_tagTitleMute.SetCaption(GetResourceString(IDS_SETTING_MUTE));
	m_tagTitleMute.LinkTextGL(pTextGL);
	m_tagTitleMute.SetPos(0,py);
	m_tagTitleMute.SetCaptionHeight(caption_h);
	m_tagTitleMute.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleMute.SetCaptionColor(&cr_gray);

	m_sliMute.Initialize(IDC_SETTING_SLI_MUTE,this,&sli,pSkinManager);
	m_sliMute.SetRange(0,10);
	m_sliMute.SetValue(0);
	m_sliMute.SetPos(m_siCurSize.cx/2-70,py);
	m_sliMute.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

	py-=85;
	*/

	//loudness:
	m_tagTitleLoudness.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLoudness.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLoudness.SetCaption(GetResourceString(IDS_SETTING_LOUDNESS));
	m_tagTitleLoudness.LinkTextGL(pTextGL);
	m_tagTitleLoudness.SetPos(0,py);
	m_tagTitleLoudness.SetCaptionHeight(caption_h);
	m_tagTitleLoudness.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleLoudness.SetCaptionColor(&cr_gray);

	m_sliLoudness.Initialize(IDC_SETTING_SLI_LOUDNESS,this,&sli,pSkinManager);
	m_sliLoudness.SetRange(0,10);
	m_sliLoudness.SetValue(0);
	m_sliLoudness.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLoudness.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

	#if CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT
	py-=tag_h;

	//rear vol control:
	m_tagTitleRearVolControl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRearVolControl.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleRearVolControl.SetCaption(GetResourceString(IDS_SETTING_ENABLE_REAR_VOL_CONTROL));
	m_tagTitleRearVolControl.LinkTextGL(pTextGL);
	m_tagTitleRearVolControl.SetPos(0,py);

    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
		case LANGUAGE_RUSSIAN:
	        m_tagTitleRearVolControl.SetCaptionHeight(caption_h-2);
			break;
		default:
			m_tagTitleRearVolControl.SetCaptionHeight(caption_h);
			break;
    }
	m_tagTitleRearVolControl.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleRearVolControl.SetCaptionColor(&cr_gray);

	m_sliRearVolControl.Initialize(IDC_SETTING_SLI_REAR_VOL_CONTROL,this,&sli,pSkinManager);
	m_sliRearVolControl.SetRange(0,10);
	m_sliRearVolControl.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bRearVolControl?10:0);
	m_sliRearVolControl.SetPos(m_siCurSize.cx/2-70,py);
	m_sliRearVolControl.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

	#endif


	#if CVTE_EN_AUDIO_SUBWOOFER
	py-=tag_h;

	//subwoofer:
	m_tagTitleSubwoofer.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleSubwoofer.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleSubwoofer.SetCaption(GetResourceString(IDS_SETTING_SUBWOOFER));
	m_tagTitleSubwoofer.LinkTextGL(pTextGL);
	m_tagTitleSubwoofer.SetPos(0,py);
	m_tagTitleSubwoofer.SetCaptionHeight(caption_h);
	m_tagTitleSubwoofer.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleSubwoofer.SetCaptionColor(&cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_sliSubwoofer.Initialize(IDC_SETTING_SLI_SUBWOOFER,this,&sli,pSkinManager);
	m_sliSubwoofer.SetRange(0,14);
	m_sliSubwoofer.SetValue(0);
    m_sliSubwoofer.SetPos(m_siCurSize.cx/2-144,py);
	m_sliSubwoofer.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliSubwoofer.LinkTextGL(pTextGL);
	m_sliSubwoofer.SetCaptionColor(&cr_gray);
	m_sliSubwoofer.ShowValue(TRUE);
	m_sliSubwoofer.EnableMoveNotify(TRUE);
	m_sliSubwoofer.EnbaleClickOnlyThumb(TRUE);

	#endif

	#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
	py-=tag_h;

	m_btnSubwooferFreq.Initialize(IDC_SETTING_BTN_SUBWOOFERFREQ,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSubwooferFreq.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnSubwooferFreq.SetCaption(GetResourceString(IDS_SETTING_AUDIO_SUBWOOFER_FREQ));
	m_btnSubwooferFreq.LinkTextGL(pTextGL);
	m_btnSubwooferFreq.SetPos(0,py);
	m_btnSubwooferFreq.SetCaptionHeight(caption_h);
	m_btnSubwooferFreq.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnSubwooferFreq.SetCaptionColor(&cr_gray);
	m_btnSubwooferFreq.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);


	m_tagValueSubwooferFreq.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueSubwooferFreq.SetSize(cx,40);
	m_tagValueSubwooferFreq.SetCaption(_T("120HZ"));
	m_tagValueSubwooferFreq.LinkTextGL(pTextGL);
	m_tagValueSubwooferFreq.SetPos(cx/2+10 -20,py);
	m_tagValueSubwooferFreq.SetCaptionHeight(caption_h);
	m_tagValueSubwooferFreq.EnableDrawBack(FALSE);
	m_tagValueSubwooferFreq.SetCaptionPos(BTN_CAPTION_RIGHT,-20);
	m_tagValueSubwooferFreq.SetCaptionColor(&cr_gray);
	#endif

	//
	py-=70;

	//volume:
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;
	
	m_tagTitleVolume.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolume.SetSize(cx,40);
	m_tagTitleVolume.SetCaption(GetResourceString(IDS_SETTING_VOLUME));
	m_tagTitleVolume.LinkTextGL(pTextGL);
	m_tagTitleVolume.SetPos(-cx/2-10 +12,py);
	m_tagTitleVolume.SetCaptionHeight(20);
	m_tagTitleVolume.EnableDrawBack(FALSE);
	m_tagTitleVolume.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleVolume.SetCaptionColor(&cr_dark);

	py-=60;

	//volume multimedia:
	m_tagTitleVolumeMultiMedia.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolumeMultiMedia.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleVolumeMultiMedia.SetCaption(/*GetResourceString(IDS_TITLE_MEDIA)*/_T("Media Player"));
	m_tagTitleVolumeMultiMedia.LinkTextGL(pTextGL);
	m_tagTitleVolumeMultiMedia.SetPos(0,py);
	m_tagTitleVolumeMultiMedia.SetCaptionHeight(caption_h);
	m_tagTitleVolumeMultiMedia.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleVolumeMultiMedia.SetCaptionColor(&cr_gray);

	m_sliVolumeMultiMedia.Initialize(IDC_SETTING_SLI_VOLUME_MULTIMEDIA,this,&sli,pSkinManager);
	m_sliVolumeMultiMedia.SetRange(0,32);
	m_sliVolumeMultiMedia.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia);
	m_sliVolumeMultiMedia.SetPos(m_siCurSize.cx/2-144,py);
	m_sliVolumeMultiMedia.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumeMultiMedia.LinkTextGL(pTextGL);
	m_sliVolumeMultiMedia.SetCaptionColor(&cr_gray);
	m_sliVolumeMultiMedia.ShowValue(TRUE);
	m_sliVolumeMultiMedia.EnableMoveNotify(TRUE);
	m_sliVolumeMultiMedia.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	//volume phone:
	m_tagTitleVolumePhone.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolumePhone.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleVolumePhone.SetCaption(/*GetResourceString(IDS_TITLE_PHONE)*/_T("Bluetooth"));
	m_tagTitleVolumePhone.LinkTextGL(pTextGL);
	m_tagTitleVolumePhone.SetPos(0,py);
	m_tagTitleVolumePhone.SetCaptionHeight(caption_h);
	m_tagTitleVolumePhone.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleVolumePhone.SetCaptionColor(&cr_gray);

	m_sliVolumePhone.Initialize(IDC_SETTING_SLI_VOLUME_PHONE,this,&sli,pSkinManager);
	m_sliVolumePhone.SetRange(0,32);
	m_sliVolumePhone.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolBluetooth);
	m_sliVolumePhone.SetPos(m_siCurSize.cx/2-144,py);
	m_sliVolumePhone.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumePhone.LinkTextGL(pTextGL);
	m_sliVolumePhone.SetCaptionColor(&cr_gray);
	m_sliVolumePhone.ShowValue(TRUE);
	m_sliVolumePhone.EnableMoveNotify(TRUE);
	m_sliVolumePhone.EnbaleClickOnlyThumb(TRUE);

	#if (CVTE_EN_NAVI || CVTE_EN_CARPLAY)
	py-=tag_h;

	//volume navi:
	m_tagTitleVolumeNavi.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolumeNavi.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleVolumeNavi.SetCaption(/*GetResourceString(IDS_SETTING_VOL_NAVI_SPEECH)*/_T("CarPlay"));
	m_tagTitleVolumeNavi.LinkTextGL(pTextGL);
	m_tagTitleVolumeNavi.SetPos(0,py);

//	    if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
//		    m_tagTitleVolumeNavi.SetCaptionHeight(caption_h);
//	    else
	    m_tagTitleVolumeNavi.SetCaptionHeight(caption_h);
	m_tagTitleVolumeNavi.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleVolumeNavi.SetCaptionColor(&cr_gray);

	m_sliVolumeNavi.Initialize(IDC_SETTING_SLI_VOLUME_NAVI,this,&sli,pSkinManager);
	m_sliVolumeNavi.SetRange(0,32);
	m_sliVolumeNavi.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS);
	m_sliVolumeNavi.SetPos(m_siCurSize.cx/2-144,py);
	m_sliVolumeNavi.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumeNavi.LinkTextGL(pTextGL);
	m_sliVolumeNavi.SetCaptionColor(&cr_gray);
	m_sliVolumeNavi.ShowValue(TRUE);
	m_sliVolumeNavi.EnableMoveNotify(TRUE);
	m_sliVolumeNavi.EnbaleClickOnlyThumb(TRUE);
	#endif

	py-=30;
#else

	//key beep:
	m_tagTitleKeyBeep.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleKeyBeep.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleKeyBeep.SetCaption(GetResourceString(IDS_SETTING_KEY_BEEP));
	m_tagTitleKeyBeep.LinkTextGL(pTextGL);
	m_tagTitleKeyBeep.SetPos(0,py);
	m_tagTitleKeyBeep.SetCaptionHeight(caption_h);
	m_tagTitleKeyBeep.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleKeyBeep.SetCaptionColor(&cr_gray);

	m_sliKeyBeep.Initialize(IDC_SETTING_SLI_KEY_BEEP,this,&sli,pSkinManager);
	m_sliKeyBeep.SetRange(0,10);
	m_sliKeyBeep.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep?10:0);
	m_sliKeyBeep.SetPos(m_siCurSize.cx/2-70,py);
	m_sliKeyBeep.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

#if (CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL)
	py-=tag_h;

	m_tagTitleInternalAMP.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleInternalAMP.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleInternalAMP.SetCaption(_T("Use Internal AMP"));
	m_tagTitleInternalAMP.LinkTextGL(pTextGL);
	m_tagTitleInternalAMP.SetPos(0,py);
	m_tagTitleInternalAMP.SetCaptionHeight(caption_h);
	m_tagTitleInternalAMP.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleInternalAMP.SetCaptionColor(&cr_gray);

	m_sliInternalAMP.Initialize(IDC_SETTING_SLI_DISABLE_INTERNAL_AMP,this,&sli,pSkinManager);
	m_sliInternalAMP.SetRange(0,10);
	m_sliInternalAMP.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep?10:0);
	m_sliInternalAMP.SetPos(m_siCurSize.cx/2-70,py);
	m_sliInternalAMP.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

#endif

	py-=tag_h;

	/*
	//mute:
	m_tagTitleMute.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleMute.SetSize((g_iClientWidth-box_size_x-margin_x*2),80);
	m_tagTitleMute.SetCaption(GetResourceString(IDS_SETTING_MUTE));
	m_tagTitleMute.LinkTextGL(pTextGL);
	m_tagTitleMute.SetPos(0,py);
	m_tagTitleMute.SetCaptionHeight(caption_h);
	m_tagTitleMute.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleMute.SetCaptionColor(&cr_gray);

	m_sliMute.Initialize(IDC_SETTING_SLI_MUTE,this,&sli,pSkinManager);
	m_sliMute.SetRange(0,10);
	m_sliMute.SetValue(0);
	m_sliMute.SetPos(m_siCurSize.cx/2-70,py);
	m_sliMute.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

	py-=85;
	*/

	//loudness:
	m_tagTitleLoudness.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLoudness.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLoudness.SetCaption(GetResourceString(IDS_SETTING_LOUDNESS));
	m_tagTitleLoudness.LinkTextGL(pTextGL);
	m_tagTitleLoudness.SetPos(0,py);
	m_tagTitleLoudness.SetCaptionHeight(caption_h);
	m_tagTitleLoudness.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleLoudness.SetCaptionColor(&cr_gray);

	m_sliLoudness.Initialize(IDC_SETTING_SLI_LOUDNESS,this,&sli,pSkinManager);
	m_sliLoudness.SetRange(0,10);
	m_sliLoudness.SetValue(0);
	m_sliLoudness.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLoudness.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

#if CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT
	py-=tag_h;

	//rear vol control:
	m_tagTitleRearVolControl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleRearVolControl.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleRearVolControl.SetCaption(GetResourceString(IDS_SETTING_ENABLE_REAR_VOL_CONTROL));
	m_tagTitleRearVolControl.LinkTextGL(pTextGL);
	m_tagTitleRearVolControl.SetPos(0,py);

    switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
    {
		case LANGUAGE_RUSSIAN:
	        m_tagTitleRearVolControl.SetCaptionHeight(caption_h-2);
			break;
		default:
			m_tagTitleRearVolControl.SetCaptionHeight(caption_h);
			break;
    }
	m_tagTitleRearVolControl.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleRearVolControl.SetCaptionColor(&cr_gray);

	m_sliRearVolControl.Initialize(IDC_SETTING_SLI_REAR_VOL_CONTROL,this,&sli,pSkinManager);
	m_sliRearVolControl.SetRange(0,10);
	m_sliRearVolControl.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bRearVolControl?10:0);
	m_sliRearVolControl.SetPos(m_siCurSize.cx/2-70,py);
	m_sliRearVolControl.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);

#endif


#if CVTE_EN_AUDIO_SUBWOOFER
	py-=tag_h;

	//subwoofer:
	m_tagTitleSubwoofer.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleSubwoofer.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleSubwoofer.SetCaption(GetResourceString(IDS_SETTING_SUBWOOFER));
	m_tagTitleSubwoofer.LinkTextGL(pTextGL);
	m_tagTitleSubwoofer.SetPos(0,py);
	m_tagTitleSubwoofer.SetCaptionHeight(caption_h);
	m_tagTitleSubwoofer.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleSubwoofer.SetCaptionColor(&cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_sliSubwoofer.Initialize(IDC_SETTING_SLI_SUBWOOFER,this,&sli,pSkinManager);
	m_sliSubwoofer.SetRange(0,14);
	m_sliSubwoofer.SetValue(0);
    m_sliSubwoofer.SetPos(m_siCurSize.cx/2-144,py);
	m_sliSubwoofer.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliSubwoofer.LinkTextGL(pTextGL);
	m_sliSubwoofer.SetCaptionColor(&cr_gray);
	m_sliSubwoofer.ShowValue(TRUE);
	m_sliSubwoofer.EnableMoveNotify(TRUE);
	m_sliSubwoofer.EnbaleClickOnlyThumb(TRUE);

#endif

#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
	py-=tag_h;

	m_btnSubwooferFreq.Initialize(IDC_SETTING_BTN_SUBWOOFERFREQ,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSubwooferFreq.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnSubwooferFreq.SetCaption(GetResourceString(IDS_SETTING_AUDIO_SUBWOOFER_FREQ));
	m_btnSubwooferFreq.LinkTextGL(pTextGL);
	m_btnSubwooferFreq.SetPos(0,py);
	m_btnSubwooferFreq.SetCaptionHeight(caption_h);
	m_btnSubwooferFreq.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_btnSubwooferFreq.SetCaptionColor(&cr_gray);
	m_btnSubwooferFreq.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);


	m_tagValueSubwooferFreq.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueSubwooferFreq.SetSize(cx,40);
	m_tagValueSubwooferFreq.SetCaption(_T("120HZ"));
	m_tagValueSubwooferFreq.LinkTextGL(pTextGL);
	m_tagValueSubwooferFreq.SetPos(cx/2+10 -20,py);
	m_tagValueSubwooferFreq.SetCaptionHeight(caption_h);
	m_tagValueSubwooferFreq.EnableDrawBack(FALSE);
	m_tagValueSubwooferFreq.SetCaptionPos(BTN_CAPTION_RIGHT,-20);
	m_tagValueSubwooferFreq.SetCaptionColor(&cr_gray);



// 	m_btnSubwooferFreq.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_btnSubwooferFreq.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
// 	m_btnSubwooferFreq.SetCaption(GetResourceString(L"Subwoofer Freq"));
// 	m_btnSubwooferFreq.LinkTextGL(pTextGL);
// 	m_btnSubwooferFreq.SetPos(0,py);
// 	m_btnSubwooferFreq.SetCaptionHeight(caption_h);
// 	m_btnSubwooferFreq.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
// 	m_btnSubwooferFreq.SetCaptionColor(&cr_gray);
#endif

#if 0
	py-=tag_h;

	//volume adaption:
	m_tagTitleVolumeAdaption.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolumeAdaption.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleVolumeAdaption.SetCaption(GetResourceString(IDS_SETTING_VOLUME_VELOCITY_ADJUST));
	m_tagTitleVolumeAdaption.LinkTextGL(pTextGL);
	m_tagTitleVolumeAdaption.SetPos(0,py);
	m_tagTitleVolumeAdaption.SetCaptionHeight(caption_h);
	m_tagTitleVolumeAdaption.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleVolumeAdaption.SetCaptionColor(&cr_gray);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_sliVolumeAdaption.Initialize(IDC_SETTING_SLI_VOLUME_ADAPTION,this,&sli,pSkinManager);
	m_sliVolumeAdaption.SetRange(0,10);
	m_sliVolumeAdaption.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolumeAdjustRate);
	m_sliVolumeAdaption.SetPos(m_siCurSize.cx/2-144,py);
	m_sliVolumeAdaption.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumeAdaption.LinkTextGL(pTextGL);
	m_sliVolumeAdaption.SetCaptionColor(&cr_gray);
	m_sliVolumeAdaption.ShowValue(TRUE);
	m_sliVolumeAdaption.EnableMoveNotify(TRUE);
	m_sliVolumeAdaption.EnbaleClickOnlyThumb(TRUE);
#endif

	//
	py-=70;

	//volume:
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;
	
	m_tagTitleVolume.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolume.SetSize(cx,40);
	m_tagTitleVolume.SetCaption(GetResourceString(IDS_SETTING_VOLUME));
	m_tagTitleVolume.LinkTextGL(pTextGL);
	m_tagTitleVolume.SetPos(-cx/2-10 +12,py);
	m_tagTitleVolume.SetCaptionHeight(20);
	m_tagTitleVolume.EnableDrawBack(FALSE);
	m_tagTitleVolume.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleVolume.SetCaptionColor(&cr_dark);

	py-=60;

	//volume multimedia:
	m_tagTitleVolumeMultiMedia.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolumeMultiMedia.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleVolumeMultiMedia.SetCaption(GetResourceString(IDS_TITLE_MEDIA));
	m_tagTitleVolumeMultiMedia.LinkTextGL(pTextGL);
	m_tagTitleVolumeMultiMedia.SetPos(0,py);
	m_tagTitleVolumeMultiMedia.SetCaptionHeight(caption_h);
	m_tagTitleVolumeMultiMedia.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleVolumeMultiMedia.SetCaptionColor(&cr_gray);

	m_sliVolumeMultiMedia.Initialize(IDC_SETTING_SLI_VOLUME_MULTIMEDIA,this,&sli,pSkinManager);
	m_sliVolumeMultiMedia.SetRange(0,32);
	m_sliVolumeMultiMedia.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia);
	m_sliVolumeMultiMedia.SetPos(m_siCurSize.cx/2-144,py);
	m_sliVolumeMultiMedia.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumeMultiMedia.LinkTextGL(pTextGL);
	m_sliVolumeMultiMedia.SetCaptionColor(&cr_gray);
	m_sliVolumeMultiMedia.ShowValue(TRUE);
	m_sliVolumeMultiMedia.EnableMoveNotify(TRUE);
	m_sliVolumeMultiMedia.EnbaleClickOnlyThumb(TRUE);

	py-=tag_h;

	//volume phone:
	m_tagTitleVolumePhone.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolumePhone.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleVolumePhone.SetCaption(GetResourceString(IDS_TITLE_PHONE));
	m_tagTitleVolumePhone.LinkTextGL(pTextGL);
	m_tagTitleVolumePhone.SetPos(0,py);
	m_tagTitleVolumePhone.SetCaptionHeight(caption_h);
	m_tagTitleVolumePhone.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleVolumePhone.SetCaptionColor(&cr_gray);

	m_sliVolumePhone.Initialize(IDC_SETTING_SLI_VOLUME_PHONE,this,&sli,pSkinManager);
	m_sliVolumePhone.SetRange(0,32);
	m_sliVolumePhone.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolBluetooth);
	m_sliVolumePhone.SetPos(m_siCurSize.cx/2-144,py);
	m_sliVolumePhone.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumePhone.LinkTextGL(pTextGL);
	m_sliVolumePhone.SetCaptionColor(&cr_gray);
	m_sliVolumePhone.ShowValue(TRUE);
	m_sliVolumePhone.EnableMoveNotify(TRUE);
	m_sliVolumePhone.EnbaleClickOnlyThumb(TRUE);

#if (CVTE_EN_NAVI || CVTE_EN_CARPLAY)
	py-=tag_h;

	//volume navi:
	m_tagTitleVolumeNavi.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleVolumeNavi.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleVolumeNavi.SetCaption(GetResourceString(IDS_SETTING_VOL_NAVI_SPEECH));
	m_tagTitleVolumeNavi.LinkTextGL(pTextGL);
	m_tagTitleVolumeNavi.SetPos(0,py);

//	    if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
//		    m_tagTitleVolumeNavi.SetCaptionHeight(caption_h);
//	    else
	    m_tagTitleVolumeNavi.SetCaptionHeight(caption_h);
	m_tagTitleVolumeNavi.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleVolumeNavi.SetCaptionColor(&cr_gray);

	m_sliVolumeNavi.Initialize(IDC_SETTING_SLI_VOLUME_NAVI,this,&sli,pSkinManager);
	m_sliVolumeNavi.SetRange(0,32);
	m_sliVolumeNavi.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS);
	m_sliVolumeNavi.SetPos(m_siCurSize.cx/2-144,py);
	m_sliVolumeNavi.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliVolumeNavi.LinkTextGL(pTextGL);
	m_sliVolumeNavi.SetCaptionColor(&cr_gray);
	m_sliVolumeNavi.ShowValue(TRUE);
	m_sliVolumeNavi.EnableMoveNotify(TRUE);
	m_sliVolumeNavi.EnbaleClickOnlyThumb(TRUE);
#endif
	//
	py-=70;

	//audio effect:
	m_tagTitleAudioEffect.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleAudioEffect.SetSize(&siTitle);
	m_tagTitleAudioEffect.SetCaption(GetResourceString(IDS_SETTING_AUDIO_EFFECT));
	m_tagTitleAudioEffect.LinkTextGL(pTextGL);
	m_tagTitleAudioEffect.SetPos(-cx/2-10+12,py);
	m_tagTitleAudioEffect.SetCaptionHeight(20);
	m_tagTitleAudioEffect.EnableDrawBack(FALSE);
	m_tagTitleAudioEffect.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleAudioEffect.SetCaptionColor(&cr_dark);

	py-=70;

	int ix=(g_iClientWidth-box_size_x-margin_x*2)/5;

	m_tagAudioEffectBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagAudioEffectBack.SetSize((g_iClientWidth-box_size_x-margin_x*2),100);
	m_tagAudioEffectBack.SetPos(0,py);

	m_btnEQClassic.Initialize(IDC_SETTING_BTN_EQ_CLASSIC,this,m_pSkinManager->GetTexture(TEXID_650),NULL,
		m_pSkinManager->GetTexture(TEXID_651),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQClassic.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQClassic.SetPos(-ix*2,py+10);
	m_btnEQClassic.LinkTextGL(pTextGL);
	m_btnEQClassic.SetCaption(GetResourceString(IDS_SETTING_CLASSIC));
	m_btnEQClassic.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQClassic.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQClassic.SetCaptionHeight(18);
	m_btnEQClassic.SetTouchRate(2.0f,2.0f);
	m_btnEQClassic.EnableClickAni(TRUE);

	m_btnEQPop.Initialize(IDC_SETTING_BTN_EQ_POP,this,m_pSkinManager->GetTexture(TEXID_656),NULL,
		m_pSkinManager->GetTexture(TEXID_657),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQPop.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQPop.SetPos(-ix,py+10);
	m_btnEQPop.LinkTextGL(pTextGL);
	m_btnEQPop.SetCaption(GetResourceString(IDS_SETTING_POP));
	m_btnEQPop.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQPop.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQPop.SetCaptionHeight(18);
	m_btnEQPop.SetTouchRate(2.0f,2.0f);
	m_btnEQPop.EnableClickAni(TRUE);

	m_btnEQRock.Initialize(IDC_SETTING_BTN_EQ_ROCK,this,m_pSkinManager->GetTexture(TEXID_658),NULL,
		m_pSkinManager->GetTexture(TEXID_659),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQRock.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQRock.SetPos(0,py+10);
	m_btnEQRock.LinkTextGL(pTextGL);
	m_btnEQRock.SetCaption(GetResourceString(IDS_SETTING_ROCK));
	m_btnEQRock.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQRock.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQRock.SetCaptionHeight(18);
	m_btnEQRock.SetTouchRate(2.0f,2.0f);
	m_btnEQRock.EnableClickAni(TRUE);

	m_btnEQJazz.Initialize(IDC_SETTING_BTN_EQ_JAZZ,this,m_pSkinManager->GetTexture(TEXID_654),NULL,
		m_pSkinManager->GetTexture(TEXID_655),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQJazz.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQJazz.SetPos(ix,py+10);
	m_btnEQJazz.LinkTextGL(pTextGL);
	m_btnEQJazz.SetCaption(GetResourceString(IDS_SETTING_JAZZ));
	m_btnEQJazz.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQJazz.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQJazz.SetCaptionHeight(18);
	m_btnEQJazz.SetTouchRate(2.0f,2.0f);
	m_btnEQJazz.EnableClickAni(TRUE);

	m_btnEQUser.Initialize(IDC_SETTING_BTN_EQ_USER,this,m_pSkinManager->GetTexture(TEXID_652),NULL,
		m_pSkinManager->GetTexture(TEXID_653),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnEQUser.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_btnEQUser.SetPos(ix*2,py+10);
	m_btnEQUser.LinkTextGL(pTextGL);
	m_btnEQUser.SetCaption(GetResourceString(IDS_SETTING_USER));
	m_btnEQUser.SetCaptionPos(BTN_CAPTION_CENTER,0,-30);
	m_btnEQUser.SetCaptionColor(&cr_gray,&cr_green);
	m_btnEQUser.SetCaptionHeight(18);
	m_btnEQUser.SetTouchRate(2.0f,2.0f);
	m_btnEQUser.EnableClickAni(TRUE);

	py-=tag_h;
	//equalize:
	m_tagTitleEqualizer.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleEqualizer.SetSize(&siTitle);
	m_tagTitleEqualizer.SetCaption(GetResourceString(IDS_SETTING_EQ));
	m_tagTitleEqualizer.LinkTextGL(pTextGL);
	m_tagTitleEqualizer.SetPos(-cx/2-10 +12,py);
	m_tagTitleEqualizer.SetCaptionHeight(20);
	m_tagTitleEqualizer.EnableDrawBack(FALSE);
	m_tagTitleEqualizer.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleEqualizer.SetCaptionColor(&cr_dark);

	//bal/fad:
	m_tagTitleBalance.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleBalance.SetSize(&siTitle);
	m_tagTitleBalance.SetCaption(GetResourceString(IDS_SETTING_BAL_FAD));
	m_tagTitleBalance.LinkTextGL(pTextGL);
	m_tagTitleBalance.SetPos(cx/2-10,py);
	m_tagTitleBalance.SetCaptionHeight(20);
	m_tagTitleBalance.EnableDrawBack(FALSE);
	m_tagTitleBalance.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleBalance.SetCaptionColor(&cr_dark);

	m_tagValueBalance.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueBalance.SetSize(&siTitle);
	m_tagValueBalance.SetCaption(_T("0/0"));
	m_tagValueBalance.LinkTextGL(pTextGL);
	m_tagValueBalance.SetPos(cx/2+10 -12,py);
	m_tagValueBalance.SetCaptionHeight(18);
	m_tagValueBalance.EnableDrawBack(FALSE);
	m_tagValueBalance.SetCaptionPos(BTN_CAPTION_RIGHT);
	m_tagValueBalance.SetCaptionColor(&cr_dark);

	py-=(20+cy/2);

	m_tagEqualizerBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagEqualizerBack.SetSize(cx,cy);
	m_tagEqualizerBack.SetPos(-cx/2,py);

	m_tagBalanceBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_670),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagBalanceBack.SetSize(cx,cy);
	m_tagBalanceBack.SetPos(cx/2,py);

	//slider
	//SLIDERSTRUCTURE sli;
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_THICKNESS;
	sli.size.cy=cy-tag_h;//256;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_662;
	sli.idChannelTex=TEXID_661;
	sli.idThumbTex=TEXID_640;
	sli.dwType=ST_VER_NORMAL;

	m_sliLow.Initialize(IDC_SETTING_SLI_BASS,this,&sli,pSkinManager);
	m_sliLow.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliLow.SetRange(0,14);
	m_sliLow.SetValue(7);
	m_sliLow.SetPos(-cx/2-cx/3,py);
	m_sliLow.SetAutoBackFill(TRUE);
	m_sliLow.EnableMoveNotify(TRUE);
	//m_sliLow.LinkTextGL(pTextGL);
	//m_sliLow.SetCaptionColor(&cr_gray);
	//m_sliLow.ShowValue(TRUE);
	m_sliLow.EnbaleClickOnlyThumb(TRUE);

	m_sliMid.Initialize(IDC_SETTING_SLI_MID,this,&sli,pSkinManager);
	m_sliMid.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliMid.SetRange(0,14);
	m_sliMid.SetValue(7);
	m_sliMid.SetPos(-cx/2,py);
	m_sliMid.SetAutoBackFill(TRUE);
	m_sliMid.EnableMoveNotify(TRUE);
	//m_sliMid.LinkTextGL(pTextGL);
	//m_sliMid.SetCaptionColor(&cr_gray);
	//m_sliMid.ShowValue(TRUE);
	m_sliMid.EnbaleClickOnlyThumb(TRUE);

	m_sliHigh.Initialize(IDC_SETTING_SLI_TREBLE,this,&sli,pSkinManager);
	m_sliHigh.SetNotifyFunc(&CPanelSettingAudio::OnNotify,this);
	m_sliHigh.SetRange(0,14);
	m_sliHigh.SetValue(7);
	m_sliHigh.SetPos(-cx/2+cx/3,py);
	m_sliHigh.SetAutoBackFill(TRUE);
	m_sliHigh.EnableMoveNotify(TRUE);
	//m_sliHigh.LinkTextGL(pTextGL);
	//m_sliHigh.SetCaptionColor(&cr_gray);
	//m_sliHigh.ShowValue(TRUE);
	m_sliHigh.EnbaleClickOnlyThumb(TRUE);

	m_tagTitleLow.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLow.SetSize(&siTitle);
	m_tagTitleLow.SetCaption(GetResourceString(IDS_SETTING_LOW));
	m_tagTitleLow.LinkTextGL(pTextGL);
	m_tagTitleLow.SetPos(-cx/2-cx/3,py-cy/2+16);
	m_tagTitleLow.SetCaptionHeight(18);
	m_tagTitleLow.EnableDrawBack(FALSE);
	m_tagTitleLow.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleLow.SetCaptionColor(&cr_gray);

	m_tagTitleMid.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleMid.SetSize(&siTitle);
	m_tagTitleMid.SetCaption(GetResourceString(IDS_SETTING_MID));
	m_tagTitleMid.LinkTextGL(pTextGL);
	m_tagTitleMid.SetPos(-cx/2,py-cy/2+16);
	m_tagTitleMid.SetCaptionHeight(18);
	m_tagTitleMid.EnableDrawBack(FALSE);
	m_tagTitleMid.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleMid.SetCaptionColor(&cr_gray);

	m_tagTitleHigh.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleHigh.SetSize(&siTitle);
	m_tagTitleHigh.SetCaption(GetResourceString(IDS_SETTING_HIGH));
	m_tagTitleHigh.LinkTextGL(pTextGL);
	m_tagTitleHigh.SetPos(-cx/2+cx/3,py-cy/2+16);
	m_tagTitleHigh.SetCaptionHeight(18);
	m_tagTitleHigh.EnableDrawBack(FALSE);
	m_tagTitleHigh.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagTitleHigh.SetCaptionColor(&cr_gray);


	m_tagValueLow.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueLow.SetSize(&siTitle);
	m_tagValueLow.SetCaption(_T("0"));
	m_tagValueLow.LinkTextGL(pTextGL);
	m_tagValueLow.SetPos(-cx/2-cx/3,py+122);
	m_tagValueLow.SetCaptionHeight(16);
	m_tagValueLow.EnableDrawBack(FALSE);
	m_tagValueLow.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueLow.SetCaptionColor(&cr_gray);

	m_tagValueMid.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueMid.SetSize(&siTitle);
	m_tagValueMid.SetCaption(_T("0"));
	m_tagValueMid.LinkTextGL(pTextGL);
	m_tagValueMid.SetPos(-cx/2,py+122);
	m_tagValueMid.SetCaptionHeight(16);
	m_tagValueMid.EnableDrawBack(FALSE);
	m_tagValueMid.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueMid.SetCaptionColor(&cr_gray);

	m_tagValueHigh.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagValueHigh.SetSize(&siTitle);
	m_tagValueHigh.SetCaption(_T("0"));
	m_tagValueHigh.LinkTextGL(pTextGL);
	m_tagValueHigh.SetPos(-cx/2+cx/3,py+122);
	m_tagValueHigh.SetCaptionHeight(16);
	m_tagValueHigh.EnableDrawBack(FALSE);
	m_tagValueHigh.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagValueHigh.SetCaptionColor(&cr_gray);


	//bal/fad:
	m_barBalance.Initialize(IDC_SETTING_BAR_AUDIO_BALANCE,this,pSkinManager,pTextGL);
	m_barBalance.SetSize(/*360*/cx+10,/*360*/cy+10);
	m_barBalance.SetMargin(tag_h);
	m_barBalance.SetPos(cx/2,py);
	m_barBalance.SetRange(-7,-7,7,7);
	m_barBalance.SetNotifyFunc(CPanelSettingAudio::OnNotify,this);

	m_iCreatePageSizeY = m_siCurSize.cy;

	py-=cy/2;
#endif

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	g_iPanelHeight = g_iClientHeight/2 - py + 20;

	if(g_iPanelHeight < g_iClientHeight)
		g_iPanelHeight = g_iClientHeight;

	SetSize(g_iClientWidth-box_size_x,g_iPanelHeight);

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
	SetTimer(m_hMainWnd, TIMER_RESTORE_AUDIO_EQ_SETTING, 1000, CPanelSettingAudio::OnTimerProc);
#endif
	return TRUE;
}


void CALLBACK CPanelSettingAudio::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	//RETAILMSG(DEBUG_SETTING,(_T("CPanelSettingAudio OnNotify id:%d,msg:%d,para:0x%x,lpUserPara:%x\r\n"),idControl,wMsg,wPara,lpUserPara));

	CPanelSettingAudio *panel=(CPanelSettingAudio*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingAudio::OnMCUCmd(WPARAM wParam, LPARAM lParam)
{
	DWORD dwType=HIWORD(wParam);
	DWORD dwID=LOWORD(wParam);

	CCommanderEx *pCommander = (CCommanderEx*)lParam;
	if(!pCommander)
	{
		return;
	}


	if(dwType == 0)//package
	{
#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
		UpdateAudioPara(AUDIO_SET_SUBWOOFER_FREQ,pCommander->GetItemData(MCU_AUDIO_SUBWOOFER_FREQ));
#endif
		UpdateAudioPara(AUDIO_SET_MUTE,pCommander->GetItemData(MCU_MUTE_MODE));
		UpdateAudioPara(AUDIO_SET_LOUDNESS,pCommander->GetItemData(MCU_AUDIO_LOUD));
		UpdateAudioPara(AUDIO_SET_VOLUME,pCommander->GetItemData(MCU_AUDIO_VOLUME));
		UpdateAudioPara(AUDIO_SET_SUBWOOFER,pCommander->GetItemData(MCU_AUDIO_SUBWOOFER));

#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
		UpdateAudioPara(AUDIO_SET_TREBLE,pCommander->GetItemData(MCU_AUDIO_TREBLE));
		UpdateAudioPara(AUDIO_SET_BASS,pCommander->GetItemData(MCU_AUDIO_BASS));
		UpdateAudioPara(AUDIO_SET_MID,pCommander->GetItemData(MCU_AUDIO_MID));
		UpdateAudioPara(AUDIO_SET_EQ,pCommander->GetItemData(MCU_AUDIO_EQ));
#endif
		
		UpdateAudioPara(AUDIO_SET_BALANCE,pCommander->GetItemData(MCU_AUDIO_BALANCE));
		UpdateAudioPara(AUDIO_SET_FAD,pCommander->GetItemData(MCU_AUDIO_FAD));	
		UpdateAudioPara(AUDIO_SET_INTERNAL_AMP,pCommander->GetItemData(MCU_AUDIO_INTERNAL_AMP));
	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
		case MCU_MUTE_MODE:
			UpdateAudioPara(AUDIO_SET_MUTE,pCommander->GetItemData(MCU_MUTE_MODE));
			break;
		case MCU_AUDIO_LOUD:
			UpdateAudioPara(AUDIO_SET_LOUDNESS,pCommander->GetItemData(MCU_AUDIO_LOUD));
			break;
		case MCU_AUDIO_SUBWOOFER:
			UpdateAudioPara(AUDIO_SET_SUBWOOFER,pCommander->GetItemData(MCU_AUDIO_SUBWOOFER));
			break;
		case MCU_AUDIO_VOLUME:
			UpdateAudioPara(AUDIO_SET_VOLUME,pCommander->GetItemData(MCU_AUDIO_VOLUME));
			break;
#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
		case MCU_AUDIO_BASS:
			UpdateAudioPara(AUDIO_SET_BASS,pCommander->GetItemData(MCU_AUDIO_BASS));
			break;
		case MCU_AUDIO_TREBLE:
			UpdateAudioPara(AUDIO_SET_TREBLE,pCommander->GetItemData(MCU_AUDIO_TREBLE));
			break;
		case MCU_AUDIO_MID:
			UpdateAudioPara(AUDIO_SET_MID,pCommander->GetItemData(MCU_AUDIO_MID));
			break;
		case MCU_AUDIO_BALANCE:
			UpdateAudioPara(AUDIO_SET_BALANCE,pCommander->GetItemData(MCU_AUDIO_BALANCE));
			break;
		case MCU_AUDIO_FAD:
			UpdateAudioPara(AUDIO_SET_FAD,pCommander->GetItemData(MCU_AUDIO_FAD));
			break;
		case MCU_AUDIO_EQ:
			UpdateAudioPara(AUDIO_SET_EQ,pCommander->GetItemData(MCU_AUDIO_EQ));
			break;
#endif
#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
		case MCU_AUDIO_SUBWOOFER_FREQ:
			UpdateAudioPara(AUDIO_SET_SUBWOOFER_FREQ,pCommander->GetItemData(MCU_AUDIO_SUBWOOFER_FREQ));
			break;
#endif
		case MCU_AUDIO_INTERNAL_AMP:
			UpdateAudioPara(AUDIO_SET_INTERNAL_AMP,pCommander->GetItemData(MCU_AUDIO_INTERNAL_AMP));
		default:
			break;

		}
	}
}
void CPanelSettingAudio::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_SLI_KEY_BEEP:
    	ConfigKeyBeep(wPara?1:0);
        PlaySoundEffect(0);
		break;
	case IDC_SETTING_SLI_MUTE:
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_MUTE_ALL,wPara?1:0);
		}
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_SLI_LOUDNESS:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			m_pAudioCodec->SetLoudness(wPara);
		}
#else
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_LOUDNESS_KEY,wPara?1:0);
		}
#endif
		PlaySoundEffect(0);
		break;
#if  (CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL)
	case IDC_SETTING_SLI_DISABLE_INTERNAL_AMP:
		if(m_pCommander)
		{
			//RETAILMSG(1,(_T("IDC_SETTING_SLI_DISABLE_INTERNAL_AMP: %d\r\n"),wPara?1:0));
			if(wPara)
			{
				m_pCommander->SendCmd(_ARM_DISABLE_INTERNAL_AMP,0);
				m_pCommander->SendCmd(_ARM_MUTE_ALL,0);
			}
			else
			{
				//m_pCommander->SendCmd(_ARM_MUTE_ALL,1);
				m_pCommander->SendCmd(_ARM_DISABLE_INTERNAL_AMP,1);
			}
			//PlaySoundEffect(0);
		}
		break;
#endif
	case IDC_SETTING_SLI_REAR_VOL_CONTROL:
		ConfigRearVolControl(wPara?1:0);
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_SLI_SUBWOOFER:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			m_pAudioCodec->SetSubwoofer(wPara);
		}
#else
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_SUBWOOFER_KEY,wPara);
		}
#endif
		PlaySoundEffect(0);
		break;
#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
	case IDC_SETTING_BTN_SUBWOOFERFREQ:
		if(m_pCommander)
		{
			switch(m_pSkinManager->GetConfig()->GetCurParam()->iSubWooferFreqCurrent)
			{
			case SUBWOOFER_FREQ_80HZ:
				wPara = SUBWOOFER_FREQ_120HZ;
				break;
			case SUBWOOFER_FREQ_120HZ:
				wPara = SUBWOOFER_FREQ_160HZ;
				break;
			case SUBWOOFER_FREQ_160HZ:
				wPara = SUBWOOFER_FREQ_80HZ;
				break;
			default:
				break;
			}
			m_pSkinManager->GetConfig()->GetCurParam()->iSubWooferFreqCurrent = wPara;
			UpdateSubWooferFreq((UINT)wPara);
			m_pCommander->SendCmd(_ARM_AUDIO_SUBWOOFER_FREQ,wPara);
		}
		PlaySoundEffect(0);
		break;
#endif
	case IDC_SETTING_SLI_BASS:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			if (m_pAudioCodec->SetBass(wPara))
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQLow = wPara;
				g_EQParam[EQ_USER].iBass = wPara;
				g_EQParam[EQ_USER].iMid = g_EQParam[m_pSkinManager->GetConfig()->GetCurParam()->iEQType].iMid;
				g_EQParam[EQ_USER].iTreb = g_EQParam[m_pSkinManager->GetConfig()->GetCurParam()->iEQType].iTreb;
				UpdateAudioPara(AUDIO_SET_BASS,wPara);
				UpdateAudioPara(AUDIO_SET_EQ,EQ_USER);
			}
		}
#else
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_BASS_KEY,wPara);
		}
#endif
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_SLI_MID:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			if (m_pAudioCodec->SetMid(wPara))
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQMid = wPara;
				g_EQParam[EQ_USER].iBass = g_EQParam[m_pSkinManager->GetConfig()->GetCurParam()->iEQType].iBass;
				g_EQParam[EQ_USER].iMid = wPara;
				g_EQParam[EQ_USER].iTreb = g_EQParam[m_pSkinManager->GetConfig()->GetCurParam()->iEQType].iTreb;
				UpdateAudioPara(AUDIO_SET_MID,wPara);
				UpdateAudioPara(AUDIO_SET_EQ,EQ_USER);
			}
		}
#else
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_MID_KEY,wPara);
		}
#endif
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_SLI_TREBLE:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			if (m_pAudioCodec->SetTreble(wPara))
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQTreb = wPara;
				g_EQParam[EQ_USER].iBass = g_EQParam[m_pSkinManager->GetConfig()->GetCurParam()->iEQType].iBass;
				g_EQParam[EQ_USER].iMid = g_EQParam[m_pSkinManager->GetConfig()->GetCurParam()->iEQType].iMid;
				g_EQParam[EQ_USER].iTreb = wPara;
				UpdateAudioPara(AUDIO_SET_TREBLE,wPara);
				UpdateAudioPara(AUDIO_SET_EQ,EQ_USER);
			}
		}
#else
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_TREBLE_KEY,wPara);
		}
#endif
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_BAR_AUDIO_BALANCE:
		if (wMsg==NOTIFY_VALUE_CHANGE)
		{
			int bal=short(HIWORD(wPara));
			int fad=short(LOWORD(wPara));

			//RETAILMSG(DEBUG_SETTING,(_T("===bal:%d,fad:%d\r\n"),bal,fad));
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
			if (m_pAudioCodec)
			{
				if (m_pAudioCodec->SetBalFad(bal+7,fad+7))
				{
					UpdateAudioPara(AUDIO_SET_BALANCE,bal);
					UpdateAudioPara(AUDIO_SET_FAD,fad);
				}
			}
#endif
			if(m_pCommander)
			{
				m_pCommander->SendCmd(_ARM_BALANCE_KEY,bal+7);
				m_pCommander->SendCmd(_ARM_FAD_KEY,fad+7);
			}

			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_BTN_EQ_CLASSIC:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_CLASSICAL].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_CLASSICAL].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_CLASSICAL].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_CLASSICAL].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_CLASSICAL].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_CLASSICAL].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_CLASSICAL);
		}
#else
		if(m_pCommander)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->iEQType = m_btnEQClassic.IsOnFocus()?EQ_NONE:EQ_CLASSICAL;
			m_pCommander->SendCmd(_ARM_EQ_KEY,m_pSkinManager->GetConfig()->GetCurParam()->iEQType);
		}
#endif
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_BTN_EQ_POP:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_POP].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_POP].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_POP].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_POP].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_POP].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_POP].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_POP);
		}
#else
		if(m_pCommander)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->iEQType = m_btnEQPop.IsOnFocus()?EQ_NONE:EQ_POP;
			m_pCommander->SendCmd(_ARM_EQ_KEY,m_pSkinManager->GetConfig()->GetCurParam()->iEQType);
		}
#endif
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_BTN_EQ_ROCK:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_ROCK].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_ROCK].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_ROCK].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_ROCK].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_ROCK].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_ROCK].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_ROCK);
		}
#else
		if(m_pCommander)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->iEQType = m_btnEQRock.IsOnFocus()?EQ_NONE:EQ_ROCK;
			m_pCommander->SendCmd(_ARM_EQ_KEY,m_pSkinManager->GetConfig()->GetCurParam()->iEQType);
		}
#endif
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_BTN_EQ_JAZZ:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_JAZZ].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_JAZZ].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_JAZZ].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_JAZZ].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_JAZZ].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_JAZZ].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_JAZZ);
		}
#else
		if(m_pCommander)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->iEQType = m_btnEQJazz.IsOnFocus()?EQ_NONE:EQ_JAZZ;
			m_pCommander->SendCmd(_ARM_EQ_KEY,m_pSkinManager->GetConfig()->GetCurParam()->iEQType);
		}
#endif
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_BTN_EQ_USER:
#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
		if (m_pAudioCodec)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->iEQType = m_btnEQUser.IsOnFocus()?EQ_NONE:EQ_USER;
			RETAILMSG(1, (TEXT("MSG:[%s]: iEQType :%d \r\n"),TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iEQType ));
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_USER].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_USER].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_USER].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_USER].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_USER].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_USER].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_USER);
		}
#else
		if(m_pCommander)
		{
			m_pSkinManager->GetConfig()->GetCurParam()->iEQType = m_btnEQUser.IsOnFocus()?EQ_NONE:EQ_USER;
			m_pCommander->SendCmd(_ARM_EQ_KEY,m_pSkinManager->GetConfig()->GetCurParam()->iEQType);
		}
#endif
		PlaySoundEffect(0);
		break;
#if 0
	case IDC_SETTING_SLI_VOLUME_ADAPTION:
		{
			if(m_pSkinManager)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->dwVolumeAdjustRate = wPara;
			}
			PlaySoundEffect(0);
		}
		break;
#endif
	case IDC_SETTING_SLI_VOLUME_MULTIMEDIA:
		{
			if(::SendMessage(m_hMainWnd,WM_VOLUME_CONFIG_MSG,0,wPara))
			{
				m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia = wPara;
			}
			else
			{
				//reset:
				m_sliVolumeMultiMedia.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolMedia);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_VOLUME_NAVI:
		{
			if(::SendMessage(m_hMainWnd,WM_VOLUME_CONFIG_MSG,2,wPara))
			{
				m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS = wPara;
			}
			else
			{
				//reset:
				m_sliVolumeNavi.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolGPS);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_VOLUME_PHONE:
		{
			if(::SendMessage(m_hMainWnd,WM_VOLUME_CONFIG_MSG,1,wPara))
			{
				m_pSkinManager->GetConfig()->GetCurParam()->dwVolBluetooth = wPara;
			}
			else
			{
				//reset:
				m_sliVolumePhone.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->dwVolBluetooth);
			}
			PlaySoundEffect(0);
		}
		break;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	case IDC_SETTING_SLI_EQBALFAD_SWITCH:
		{
			if(m_pSkinManager->GetConfig()->GetCurParam()->bEQSwitch)
			{
				SetEQLocked(TRUE);
				SetEQSwitch(FALSE);
			}
			else
			{
				SetEQLocked(FALSE);
				SetEQSwitch(TRUE);
			}
		}
		break;
	case IDC_SETTING_SLI_EQ_LOCK:
		{
			if(m_bEQLock)
				SetEQLocked(FALSE);
			else
				SetEQLocked(TRUE);		
		}
		break;
#endif
	default:
		break;
	}
}

void CPanelSettingAudio::UpdateAudioPara(AUDIO_SET_ITEM item,INT iSet)
{
	wchar_t str[16];
    //RETAILMSG(1, (TEXT("APP:MSG:[CPanelSettingAudio::UpdateAudioPara] item[%d] iSet[%d].\r\n"),item, iSet));
	switch (item)
	{
	case AUDIO_SET_MUTE:
		//m_sliMute.SetValue(iSet?10:0);
		break;
	case AUDIO_SET_LOUDNESS:
		m_sliLoudness.SetValue(iSet?10:0);
		break;
	case AUDIO_SET_VOLUME:
		break;
	case AUDIO_SET_BASS:
#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
		if(!m_sliLow.IsActived())
#endif
			m_sliLow.SetValue(iSet);
		swprintf_s(str,16,_T("%d"),iSet-7);
		m_tagValueLow.SetCaption(str);
		break;
	case AUDIO_SET_SUBWOOFER:
		if(!m_sliSubwoofer.IsActived())
			m_sliSubwoofer.SetValue(iSet);
		break;
	case AUDIO_SET_TREBLE:
#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
		if(!m_sliHigh.IsActived())
#endif
			m_sliHigh.SetValue(iSet);
		swprintf_s(str,16,_T("%d"),(iSet-7));
		//RETAILMSG(1, (TEXT("MSG:[%s]: iSet(%d) m_tagValueHigh.SetCaption(%s).\r\n"),TEXT(__FUNCTION__),iSet,str));
		m_tagValueHigh.SetCaption(str);
		break;
	case AUDIO_SET_MID:
#if (CVTE_DEF_AUDIO_TYPE != CVTE_AUDIO_TYPE_AK7738)
		if(!m_sliMid.IsActived())
#endif
			m_sliMid.SetValue(iSet);
		swprintf_s(str,16,_T("%d"),iSet-7);
		m_tagValueMid.SetCaption(str);
		break;
	case AUDIO_SET_BALANCE:
		{
			if(!m_barBalance.IsActived())
				m_barBalance.SetValueX(iSet-7);
			wchar_t str[16];
			swprintf_s(str,16,_T("%d/%d"),m_barBalance.GetValueX(),m_barBalance.GetValueY());
			m_tagValueBalance.SetCaption(str);
		}
		break;
	case AUDIO_SET_FAD:
		{
			if(!m_barBalance.IsActived())
				m_barBalance.SetValueY(iSet-7);
			wchar_t str[16];
			swprintf_s(str,16,_T("%d/%d"),m_barBalance.GetValueX(),m_barBalance.GetValueY());
			m_tagValueBalance.SetCaption(str);
		}
		break;
	case AUDIO_SET_EQ:
		{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
			if (m_pSkinManager->GetConfig()->GetCurParam()->bEQSwitch)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iEQType = iSet;
				m_btnEQClassic.SetStatus(iSet==EQ_CLASSICAL?BS_FOCUS:BS_UP);
				m_btnEQPop.SetStatus(iSet==EQ_POP?BS_FOCUS:BS_UP);
				m_btnEQRock.SetStatus(iSet==EQ_ROCK?BS_FOCUS:BS_UP);
				m_btnEQJazz.SetStatus(iSet==EQ_JAZZ?BS_FOCUS:BS_UP);
				m_btnEQUser.SetStatus(iSet==EQ_USER?BS_FOCUS:BS_UP);
			}
			else
			{
				m_btnEQClassic.SetStatus(BS_UP);
				m_btnEQPop.SetStatus(BS_UP);
				m_btnEQRock.SetStatus(BS_UP);
				m_btnEQJazz.SetStatus(BS_UP);
				m_btnEQUser.SetStatus(BS_UP);			
			}
#else
			m_pSkinManager->GetConfig()->GetCurParam()->iEQType = iSet;
			RETAILMSG(1, (TEXT("MSG:[%s]: iEQType :%d \r\n"),TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iEQType ));

			m_btnEQClassic.SetStatus(iSet==EQ_CLASSICAL?BS_FOCUS:BS_UP);
			m_btnEQPop.SetStatus(iSet==EQ_POP?BS_FOCUS:BS_UP);
			m_btnEQRock.SetStatus(iSet==EQ_ROCK?BS_FOCUS:BS_UP);
			m_btnEQJazz.SetStatus(iSet==EQ_JAZZ?BS_FOCUS:BS_UP);
			m_btnEQUser.SetStatus(iSet==EQ_USER?BS_FOCUS:BS_UP);
#endif
		}
		break;
#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
	case AUDIO_SET_SUBWOOFER_FREQ:
		{
			UpdateSubWooferFreq(iSet);
			m_pSkinManager->GetConfig()->GetCurParam()->iSubWooferFreqCurrent = iSet;
		}
		break;
#endif
#if  (CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL)
	case AUDIO_SET_INTERNAL_AMP:
		{
			m_sliInternalAMP.SetValue(iSet?0:10);
		}
		break;
#endif
	default:
		break;
	}
}

void CPanelSettingAudio::OnVolumeConfigMsg(WPARAM wParam, LPARAM lParam)
{
    //RETAILMSG(1, (TEXT("CPanelSettingAudio:[OnVolumeConfigMsg]: :%d :%d\r\n"),wParam,lParam));
	switch (wParam)
	{
	case 0://media
		{
			m_sliVolumeMultiMedia.SetValue(lParam);
		}
		break;
	case 1://phone
		{
			m_sliVolumePhone.SetValue(lParam);
		}
		break;
	case 2://navi
		{
			m_sliVolumeNavi.SetValue(lParam);
		}
		break;
	default:
		break;
	}
}

void CPanelSettingAudio::ConfigKeyBeep(BOOL bOn)
{
	m_pSkinManager->GetConfig()->GetCurParam()->bKeyBeep = bOn;
	g_bKeyBeep = bOn;
}

void CPanelSettingAudio::ConfigRearVolControl(BOOL bOn)
{
	m_pSkinManager->GetConfig()->GetCurParam()->bRearVolControl = bOn;
}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
void CPanelSettingAudio::SetEQSwitch(BOOL bOn)
{
	//int iTagHight_SliEqualizer = LIST_BOX_TAG_HEIGHT;

	m_pSkinManager->GetConfig()->GetCurParam()->bEQSwitch = bOn;

	m_sliEQBalFadConctrol.SetValue(bOn?10:0);
	if (bOn)
	{
		//m_btnLock.Enable(TRUE);
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_EQ_KEY,m_pSkinManager->GetConfig()->GetCurParam()->iEQType);
		}
	}
	else
	{
		if(m_pCommander)
		{
			m_pCommander->SendCmd(_ARM_EQ_KEY,EQ_NONE);
		}
		//m_btnLock.Enable(FALSE);
	}

}

void CPanelSettingAudio::SetEQLocked(BOOL bOn)
{
	//int iTagHight_SliEqualizer = LIST_BOX_TAG_HEIGHT;

	m_bEQLock = bOn;

	if (bOn)
	{
		//m_btnLock.SetIcon(&m_iconLock,&m_iconUnLock);

		m_sliHigh.Enable(FALSE);
		m_sliMid.Enable(FALSE);
		m_sliLow.Enable(FALSE);
		//m_barBalance.Enable(FALSE);
		m_btnEQClassic.Enable(FALSE);
		m_btnEQJazz.Enable(FALSE);
		m_btnEQPop.Enable(FALSE);
		m_btnEQRock.Enable(FALSE);
		m_btnEQUser.Enable(FALSE);
	}
	else
	{
		//m_btnLock.SetIcon(&m_iconUnLock,&m_iconLock);

		m_sliHigh.Enable(TRUE);
		m_sliMid.Enable(TRUE);
		m_sliLow.Enable(TRUE);
		//m_barBalance.Enable(TRUE);
		m_btnEQClassic.Enable(TRUE);
		m_btnEQJazz.Enable(TRUE);
		m_btnEQPop.Enable(TRUE);
		m_btnEQRock.Enable(TRUE);
		m_btnEQUser.Enable(TRUE);
	}

}
#endif

#if CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
void CPanelSettingAudio::UpdateSubWooferFreq(UINT iSet)
{
	if (SUBWOOFER_FREQ_80HZ == iSet)
	{
		m_tagValueSubwooferFreq.SetCaption(L"80Hz");
	}
	else if (SUBWOOFER_FREQ_120HZ == iSet)
	{
		m_tagValueSubwooferFreq.SetCaption(L"120Hz");
	}
	else if (SUBWOOFER_FREQ_160HZ == iSet)
	{
		m_tagValueSubwooferFreq.SetCaption(L"160Hz");
	}
}
#endif

#if (CVTE_DEF_AUDIO_TYPE == CVTE_AUDIO_TYPE_AK7738)
void CPanelSettingAudio::LinkAudioCodec(CAudioCodec *pAudioCodec)
{
	m_pAudioCodec = pAudioCodec;

}

void CPanelSettingAudio::OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	switch (idEvent)
	{
	case TIMER_RESTORE_AUDIO_EQ_SETTING:
		{
			if (g_pSettingAudio->RestoreEQSetting())
			KillTimer(g_pSettingAudio->m_hMainWnd,TIMER_RESTORE_AUDIO_EQ_SETTING);
		}
		break;
	default:
		break;
	}
}

BOOL CPanelSettingAudio::RestoreEQSetting()
{
	if (m_pAudioCodec == NULL || m_pSkinManager == NULL || m_pCommander == NULL)
	{
		return FALSE;
	}

	if (!m_pAudioCodec->IsCodecIni())
	{
		return FALSE;
	}
	RETAILMSG(1, (TEXT("MSG:[%s]: iEQType :%d \r\n"),TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iEQType ));
	RETAILMSG(1, (TEXT("MSG:[%s]: iAudioEQLow :%d \r\n"),TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQLow ));
	RETAILMSG(1, (TEXT("MSG:[%s]: iAudioEQMid :%d \r\n"),TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQMid ));
	RETAILMSG(1, (TEXT("MSG:[%s]: iAudioEQTreb :%d \r\n"),TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQTreb ));

	m_pAudioCodec->SetBalFad(m_pCommander->GetItemData(MCU_AUDIO_BALANCE),m_pCommander->GetItemData(MCU_AUDIO_FAD));

	switch (m_pSkinManager->GetConfig()->GetCurParam()->iEQType)
	{
	case EQ_CLASSICAL:
		{
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_CLASSICAL].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_CLASSICAL].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_CLASSICAL].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_CLASSICAL].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_CLASSICAL].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_CLASSICAL].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_CLASSICAL);
		}
		break;
	case EQ_POP:
		{
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_POP].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_POP].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_POP].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_POP].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_POP].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_POP].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_POP);
		}
		break;
	case EQ_ROCK:
		{
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_ROCK].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_ROCK].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_ROCK].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_ROCK].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_ROCK].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_ROCK].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_ROCK);
		}
		break;
	case EQ_JAZZ:
		{
			if (m_pAudioCodec->SetBass(g_EQParam[EQ_JAZZ].iBass))
			{
				UpdateAudioPara(AUDIO_SET_BASS,g_EQParam[EQ_JAZZ].iBass);
			}
			if (m_pAudioCodec->SetMid(g_EQParam[EQ_JAZZ].iMid))
			{
				UpdateAudioPara(AUDIO_SET_MID,g_EQParam[EQ_JAZZ].iMid);
			}
			if (m_pAudioCodec->SetTreble(g_EQParam[EQ_JAZZ].iTreb))
			{
				UpdateAudioPara(AUDIO_SET_TREBLE,g_EQParam[EQ_JAZZ].iTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_JAZZ);
		}
		break;
	case EQ_USER:
		{
			if (m_pSkinManager)
			{
				g_EQParam[EQ_USER].iBass = m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQLow;
				g_EQParam[EQ_USER].iMid = m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQMid;
				g_EQParam[EQ_USER].iTreb = m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQTreb;

				m_pAudioCodec->SetBass(m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQLow);
				m_pAudioCodec->SetMid(m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQMid);
				m_pAudioCodec->SetTreble(m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQTreb);

				UpdateAudioPara(AUDIO_SET_BASS,m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQLow);
				UpdateAudioPara(AUDIO_SET_MID,m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQMid);
				UpdateAudioPara(AUDIO_SET_TREBLE,m_pSkinManager->GetConfig()->GetCurParam()->iAudioEQTreb);
			}
			UpdateAudioPara(AUDIO_SET_EQ,EQ_USER);
		}
		break;
	default:
		break;
	}
	return TRUE;
}
#endif