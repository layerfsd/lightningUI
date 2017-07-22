#include "PanelSettingDisplay.h"
#include "../../resource.h"
#include <windev.h>
#include "CVTE_IoCtl.h"
#include "../VIP/Decoder.h"
#include <pwingdi.h>
#include <Escape.h>
#include "../Backlight/Backlight.h"

int g_nLedHeight;
int g_nLcdHeight;
int g_nPanelHeight;
int g_nResetHeight;

int EscapeCallCenter( int nEscape,
					 int cbInput,
					 LPCSTR lpszInData,
					 int cbOutput,
					 LPSTR lpszOutData)
{
	HDC hdc = NULL;
	int ret = 1;

	hdc = CreateCompatibleDC(NULL);

	if(!hdc)
	{
		ret = -1;
		goto EXIT;
	}

	switch( nEscape )
	{
	case OEMESC_ENABLE_CURSOR:
	case OEMESC_DISABLE_CURSOR:
	case OEMESC_SET_TOP_LAYER:
	case OEMESC_GET_TOP_LAYER:
	case OEMESC_GET_LAYER_PARAMS:
	case OEMESC_SET_LAYER_PARAMS:
	case OEMESC_PRINT_REGISTER:
	case OEMESC_GET_PIXEL_CLOCK:
	case OEMESC_SET_PIXEL_CLOCK:
	case OEMESC_GET_USER_VIRTBASE:
	case OEMESC_REL_USER_VIRTBASE:
	case OEMESC_GET_LAYER_ORDER:
	case OEMESC_SET_LAYER_ORDER:
	case OEMESC_GET_VPP_PARAMS:
	case OEMESC_CAPTURE_STREAM_START:
	case OEMESC_CAPTURE_STREAM_STOP:
	case OEMESC_CAPTURE_STREAM_QUERY:
	case OEMESC_SET_GAMMA_RAMP:

		break;

	default:
		ret = 0;
		goto EXIT;
	}

	// query if token supported
	if(((ret = ExtEscape(hdc, QUERYESCSUPPORT, 0, (LPCSTR)&nEscape, 0, NULL)) <= 0))
	{
		goto EXIT;
	}

	//RETAILMSG(DEBUG_SETTING,(_T("APP:MSG:Setting: EscapeCallCenter nEscape[0x%x] \r\n"),nEscape));
	ret = ExtEscape(hdc, nEscape, cbInput,  lpszInData, cbOutput, lpszOutData);

EXIT:
	if(hdc)
	{
		DeleteDC(hdc);
	}

	return ret;
}

void SetLCDGamma(DOUBLE guiLcdGamma)
{
	int retEscape;
	UINT16 *pui16GammaRamp;
	int loop;
	BYTE nTocGamma = (BYTE)(guiLcdGamma*100);

	//RETAILMSG(DEBUG_SETTING,(_T("######set gamma %d\r\n"),int(guiLcdGamma*100)));

	pui16GammaRamp = (UINT16 *)malloc(sizeof(UINT16)*256*3);
	if (pui16GammaRamp)
	{
		memset (pui16GammaRamp, 0, sizeof(UINT16));
		for(loop = 0; loop < 256; loop ++)
		{
			UINT16 val = (UINT16)min( 255, max( 255*pow(loop/255.0, guiLcdGamma) + 0.5, 0));
			*(pui16GammaRamp+loop) = val;
			*(pui16GammaRamp+loop+256) = val;
			*(pui16GammaRamp+loop+512) = val;
		}
		retEscape = EscapeCallCenter(OEMESC_SET_GAMMA_RAMP, sizeof(UINT16)*256*3, (LPCSTR)pui16GammaRamp, 0, NULL);
		if (retEscape <= 0)
		{
			//fprintf(gLogFile, "OEMESC_SET_GAMMA_RAMP failed. \r\n");
		}
		/*
		else
		{
			HANDLE handle = CreateFile(TEXT("OPD1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, 0);

			if(handle == NULL || handle == INVALID_HANDLE_VALUE)
			{
				RETAILMSG(DEBUG_SETTING, (TEXT("APP:ERROR:Setting: [SetLCDGamma] CreateFile[OPD1] Failed. err=%d\r\n"), GetLastError()));
			}
			else
			{
				RETAILMSG(DEBUG_SETTING, (TEXT("APP:ERROR:Setting: [SetLCDGamma] nTocGamma=%d\r\n"), nTocGamma));

				if(!DeviceIoControl(handle, OEMCONFIG_IOCTL_SET_LCD_GAMMA, &nTocGamma, sizeof(nTocGamma),	NULL, 0, NULL, NULL))
				{
					RETAILMSG(DEBUG_SETTING, (TEXT("APP:ERROR:Setting: [SetLCDGamma] DeviceIoControl Failed. err=%d\r\n"), GetLastError()));
				}
				CloseHandle(handle);
			}
		}
		*/
		free(pui16GammaRamp);
		pui16GammaRamp = NULL;
	}
}

CPanelSettingDisplay::CPanelSettingDisplay(void)
{
	m_bColorPaletteShown = TRUE;
	m_bNightMode = FALSE;
}
CPanelSettingDisplay::~CPanelSettingDisplay(void)
{
}

BOOL CPanelSettingDisplay::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingDisplay::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingDisplay::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);
}

void CPanelSettingDisplay::Render()
{
	if(!BeginRender())return;

	COLORGL cr_b={0.35,0.35,0.35,1.0};


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();

	//Screen Title
	m_tagTitleScreen.SetAlpha(m_fCurAlpha);
	m_tagTitleScreen.Render();

	//Screen panel
	m_tagPanelScreen.SetAlpha(m_fCurAlpha);
	m_tagPanelScreen.Render();

#if (CVTE_DEF_UI_STYLE != CVTE_UI_STYLE_ALDINET)
	//Blur
	m_tagTitleBlurEffect.SetAlpha(m_fCurAlpha);
	m_tagTitleBlurEffect.Render();
	m_sliBlurEffect.SetAlpha(m_fCurAlpha);
	m_sliBlurEffect.Render();
#endif

	//Gamma:
	m_iconLCDGamma.SetAlpha(m_fCurAlpha);
	m_iconLCDGamma.Render();
	m_tagTitleGamma.SetAlpha(m_fCurAlpha);
	m_tagTitleGamma.Render();
	m_sliGamma.SetAlpha(m_fCurAlpha);
	m_sliGamma.Render();

	//LCD Brightness day mode 
	m_iconLCDDaymode.SetAlpha(m_fCurAlpha);
	m_iconLCDDaymode.Render();
	m_tagTitleLCDBrightness.SetAlpha(m_fCurAlpha);
	m_tagTitleLCDBrightness.Render();
	m_sliLCDBrightness.SetAlpha(m_fCurAlpha);
	m_sliLCDBrightness.Render();

	//LCD Brightness night mode 
	m_iconLCDNightmode.SetAlpha(m_fCurAlpha);
	m_iconLCDNightmode.Render();
	m_tagTitleLCDBrightnessnight.SetAlpha(m_fCurAlpha);
	m_tagTitleLCDBrightnessnight.Render();
	m_sliLCDBrightnessnight.SetAlpha(m_fCurAlpha);
	m_sliLCDBrightnessnight.Render();

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
	//LED Title
	m_tagTitleLED.SetAlpha(m_fCurAlpha);
	m_tagTitleLED.Render();

	//Panel
	m_tagPanelLight.SetAlpha(m_fCurAlpha);
	m_tagPanelLight.Render();
	m_tagTitlePanelLight.SetAlpha(m_fCurAlpha);
	m_tagTitlePanelLight.Render();


	m_btnLEDColor1.SetAlpha(m_fCurAlpha);
	m_btnLEDColor1.Render();
	m_btnLEDColor2.SetAlpha(m_fCurAlpha);
	m_btnLEDColor2.Render();
	m_btnLEDColor3.SetAlpha(m_fCurAlpha);
	m_btnLEDColor3.Render();
	m_btnLEDColor4.SetAlpha(m_fCurAlpha);
	m_btnLEDColor4.Render();
	m_btnLEDColor5.SetAlpha(m_fCurAlpha);
	m_btnLEDColor5.Render();

	m_btnLEDColorUser.SetAlpha(m_fCurAlpha);
	m_btnLEDColorUser.Render();

	m_iconLEDColor.SetAlpha(m_fCurAlpha);
	m_iconLEDColor.Render();
	m_tagTitleLEDColor.SetAlpha(m_fCurAlpha);
	m_tagTitleLEDColor.Render();
	m_tagLEDColor.SetAlpha(m_fCurAlpha);
	m_tagLEDColor.Render();
	m_sliLEDColor.SetAlpha(m_fCurAlpha);
	m_sliLEDColor.Render();

	m_tagTitleLEDBrightness.SetAlpha(m_fCurAlpha);
	m_tagTitleLEDBrightness.Render();
	m_tagLEDBrightness.SetAlpha(m_fCurAlpha);
	m_tagLEDBrightness.Render();
	m_sliLEDBrightness.SetAlpha(m_fCurAlpha);
	m_sliLEDBrightness.Render();

	m_iconLEDDaymode.SetAlpha(m_fCurAlpha);
	m_iconLEDDaymode.Render();
	m_tagTitleLEDBrightnessnight.SetAlpha(m_fCurAlpha);
	m_tagTitleLEDBrightnessnight.Render();
	m_tagLEDBrightnessnight.SetAlpha(m_fCurAlpha);
	m_tagLEDBrightnessnight.Render();
	m_sliLEDBrightnessnight.SetAlpha(m_fCurAlpha);
	m_sliLEDBrightnessnight.Render();

	m_iconLEDNightmode.SetAlpha(m_fCurAlpha);
	m_iconLEDNightmode.Render();
	m_tagTitleLedAnimationEffect.SetAlpha(m_fCurAlpha);
	m_tagTitleLedAnimationEffect.Render();
	m_sliLedAnimationEffect.SetAlpha(m_fCurAlpha);
	m_sliLedAnimationEffect.Render();

#endif

	//Reset title
	m_tagTitleReset.SetAlpha(m_fCurAlpha);
	m_tagTitleReset.Render();

	//Reset:
	m_btnReset.SetAlpha(m_fCurAlpha);
	m_btnReset.Render();

#else
	//Screen Title
	m_tagTitleScreen.SetAlpha(m_fCurAlpha);
	m_tagTitleScreen.Render();

	//Screen panel
	m_tagPanelScreen.SetAlpha(m_fCurAlpha);
	m_tagPanelScreen.Render();

	//Blur
	m_tagTitleBlurEffect.SetAlpha(m_fCurAlpha);
	m_tagTitleBlurEffect.Render();
	m_sliBlurEffect.SetAlpha(m_fCurAlpha);
	m_sliBlurEffect.Render();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	//lamp control:
	m_tagTitleLampControl.SetAlpha(m_fCurAlpha);
	m_tagTitleLampControl.Render();
	m_sliLampControl.SetAlpha(m_fCurAlpha);
	m_sliLampControl.Render();
#endif

	DrawRect(0,g_iClientHeight/2-40-60-LIST_BOX_TAG_HEIGHT/2,g_iClientWidth-LIST_BOX_ITEM_WIDTH-50,1,&cr_b);

	//Gamma:
	m_iconLCDGamma.SetAlpha(m_fCurAlpha);
	m_iconLCDGamma.Render();
	m_tagTitleGamma.SetAlpha(m_fCurAlpha);
	m_tagTitleGamma.Render();
	m_sliGamma.SetAlpha(m_fCurAlpha);
	m_sliGamma.Render();

	DrawRect(0,g_iClientHeight/2-40-60-LIST_BOX_TAG_HEIGHT-LIST_BOX_TAG_HEIGHT/2,g_iClientWidth-LIST_BOX_ITEM_WIDTH-50,1,&cr_b);

	//LCD Brightness day mode 
	m_iconLCDDaymode.SetAlpha(m_fCurAlpha);
	m_iconLCDDaymode.Render();
	m_tagTitleLCDBrightness.SetAlpha(m_fCurAlpha);
	m_tagTitleLCDBrightness.Render();
	m_sliLCDBrightness.SetAlpha(m_fCurAlpha);
	m_sliLCDBrightness.Render();

	DrawRect(0,g_iClientHeight/2-40-60-LIST_BOX_TAG_HEIGHT*2-LIST_BOX_TAG_HEIGHT/2,g_iClientWidth-LIST_BOX_ITEM_WIDTH-50,1,&cr_b);

	//LCD Brightness night mode 
	m_iconLCDNightmode.SetAlpha(m_fCurAlpha);
	m_iconLCDNightmode.Render();
	m_tagTitleLCDBrightnessnight.SetAlpha(m_fCurAlpha);
	m_tagTitleLCDBrightnessnight.Render();
	m_sliLCDBrightnessnight.SetAlpha(m_fCurAlpha);
	m_sliLCDBrightnessnight.Render();

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	DrawRect(0,g_iClientHeight/2-40-60-LIST_BOX_TAG_HEIGHT*3-LIST_BOX_TAG_HEIGHT/2,g_iClientWidth-LIST_BOX_ITEM_WIDTH-50,1,&cr_b);
#endif

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
	//LED Title
	m_tagTitleLED.SetAlpha(m_fCurAlpha);
	m_tagTitleLED.Render();

	//Panel
    m_tagPanelLight.SetAlpha(m_fCurAlpha);
    m_tagPanelLight.Render();
    m_tagTitlePanelLight.SetAlpha(m_fCurAlpha);
    m_tagTitlePanelLight.Render();
//     m_tagTitlePanelLight2.SetAlpha(m_fCurAlpha);
//     m_tagTitlePanelLight2.Render();

//	    m_btnLEDColorCustom.SetAlpha(m_fCurAlpha);
//	    m_btnLEDColorCustom.Render();

    //if(m_bColorPaletteShown)
    {
		DrawRect(0,g_iClientHeight/2-40-g_nLcdHeight-60-LIST_BOX_TAG_HEIGHT/2,g_iClientWidth-LIST_BOX_ITEM_WIDTH-50,1,&cr_b);

        m_btnLEDColor1.SetAlpha(m_fCurAlpha);
        m_btnLEDColor1.Render();
        m_btnLEDColor2.SetAlpha(m_fCurAlpha);
        m_btnLEDColor2.Render();
        m_btnLEDColor3.SetAlpha(m_fCurAlpha);
        m_btnLEDColor3.Render();
        m_btnLEDColor4.SetAlpha(m_fCurAlpha);
        m_btnLEDColor4.Render();
        m_btnLEDColor5.SetAlpha(m_fCurAlpha);
        m_btnLEDColor5.Render();

        m_btnLEDColorUser.SetAlpha(m_fCurAlpha);
        m_btnLEDColorUser.Render();

		DrawRect(0,g_iClientHeight/2-40-g_nLcdHeight-60-LIST_BOX_TAG_HEIGHT-LIST_BOX_TAG_HEIGHT/2,g_iClientWidth-LIST_BOX_ITEM_WIDTH-60,1,&cr_b);

		m_iconLEDColor.SetAlpha(m_fCurAlpha);
		m_iconLEDColor.Render();
        m_tagTitleLEDColor.SetAlpha(m_fCurAlpha);
        m_tagTitleLEDColor.Render();
        m_tagLEDColor.SetAlpha(m_fCurAlpha);
        m_tagLEDColor.Render();
        m_sliLEDColor.SetAlpha(m_fCurAlpha);
        m_sliLEDColor.Render();
        //m_labLEDCurrentColor.SetAlpha(m_fCurAlpha);
        //m_labLEDCurrentColor.Render();

		DrawRect(0,g_iClientHeight/2-40-g_nLcdHeight-60-LIST_BOX_TAG_HEIGHT*2-LIST_BOX_TAG_HEIGHT/2,g_iClientWidth-LIST_BOX_ITEM_WIDTH-60,1,&cr_b);

        m_tagTitleLEDBrightness.SetAlpha(m_fCurAlpha);
        m_tagTitleLEDBrightness.Render();
        m_tagLEDBrightness.SetAlpha(m_fCurAlpha);
        m_tagLEDBrightness.Render();
        m_sliLEDBrightness.SetAlpha(m_fCurAlpha);
        m_sliLEDBrightness.Render();

		DrawRect(0,g_iClientHeight/2-40-g_nLcdHeight-60-LIST_BOX_TAG_HEIGHT*3-LIST_BOX_TAG_HEIGHT/2,g_iClientWidth-LIST_BOX_ITEM_WIDTH-60,1,&cr_b);

		m_iconLEDDaymode.SetAlpha(m_fCurAlpha);
		m_iconLEDDaymode.Render();
		m_tagTitleLEDBrightnessnight.SetAlpha(m_fCurAlpha);
		m_tagTitleLEDBrightnessnight.Render();
		m_tagLEDBrightnessnight.SetAlpha(m_fCurAlpha);
		m_tagLEDBrightnessnight.Render();
		m_sliLEDBrightnessnight.SetAlpha(m_fCurAlpha);
		m_sliLEDBrightnessnight.Render();

		m_iconLEDNightmode.SetAlpha(m_fCurAlpha);
		m_iconLEDNightmode.Render();
		m_tagTitleLedAnimationEffect.SetAlpha(m_fCurAlpha);
        m_tagTitleLedAnimationEffect.Render();
		m_sliLedAnimationEffect.SetAlpha(m_fCurAlpha);
        m_sliLedAnimationEffect.Render();
    }
#endif

	//Reset title
	m_tagTitleReset.SetAlpha(m_fCurAlpha);
	m_tagTitleReset.Render();

	//Reset:
	m_btnReset.SetAlpha(m_fCurAlpha);
	m_btnReset.Render();
#endif

    EndRender();
}

BOOL CPanelSettingDisplay::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);
    //RETAILMSG(1, (TEXT("##############  CPanelSettingDisplay::Initialize  ################\r\n ")));
	const int box_size_x=LIST_BOX_ITEM_WIDTH;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)

	g_nLedHeight = LIST_BOX_TAG_HEIGHT*5+60;
	g_nLcdHeight = LIST_BOX_TAG_HEIGHT*4+60;	
	g_nResetHeight = LIST_BOX_TAG_HEIGHT+60;
	g_nPanelHeight = g_nLcdHeight+g_nLedHeight+g_nResetHeight+20;

	if(g_nPanelHeight<=g_iClientHeight)
		g_nPanelHeight = g_iClientHeight;

	SetSize(g_iClientWidth-box_size_x,g_nPanelHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};


	int margin_x=12;
	int margin_y = 6;
	int cx = g_iScreenWidth-LIST_BOX_ITEM_WIDTH;
	int caption_h = 22;
	int caption2_h = 22;
	int value_Font = 20;
	int gridHeight = 80;
	int margin_left = 40;
	int space = 80;
	int tag_h = space;
	int captionMargin = 22;

	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
	{
		caption_h = 20;
		caption2_h = 16;
	}

	GLfloat py=g_iClientHeight/2-40;
	SLIDERSTRUCTURE sli;

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(cx, g_iClientHeight*4);
	m_tagBk.SetPos(0, -g_iClientHeight);

	//Screen title:
	m_tagTitleScreen.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleScreen.SetSize(cx,gridHeight);
	m_tagTitleScreen.SetCaption(GetResourceString(IDS_SETTING_DISPLAY_TITLE_LCD));
	m_tagTitleScreen.LinkTextGL(pTextGL);
	m_tagTitleScreen.SetPos(0,py);
	m_tagTitleScreen.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleScreen.SetCaptionColor(&cr_gray, &cr_gray);

	/*
	py-=space;

	//Blur
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

	m_tagTitleBlurEffect.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleBlurEffect.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleBlurEffect.SetCaption(GetResourceString(IDS_SETTING_BLUR_EFFECT));
	m_tagTitleBlurEffect.LinkTextGL(pTextGL);
	m_tagTitleBlurEffect.SetPos(margin_left/2, py);
	m_tagTitleBlurEffect.SetCaptionHeight(caption_h);
	m_tagTitleBlurEffect.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleBlurEffect.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliBlurEffect.Initialize(IDC_SETTING_SLI_BLUR_EFFECT,this,&sli,pSkinManager);
	m_sliBlurEffect.SetRange(0,10);
	m_sliBlurEffect.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect?10:0);
	m_sliBlurEffect.SetPos(m_siCurSize.cx/2-70,py);
	m_sliBlurEffect.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliBlurEffect.LinkTextGL(pTextGL);
	m_sliBlurEffect.SetCaptionColor(&cr_gray);
	m_sliBlurEffect.ShowValue(FALSE);
	*/

	py-=tag_h;

	
	int xNewPosLeft = (-g_iClientWidth + box_size_x)/2;
	int xnewPosRight = (g_iClientWidth - box_size_x)/2;
	//Gamma:
	//LCD Gamma icon
	m_iconLCDGamma.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_682));
	m_iconLCDGamma.SetPos(xNewPosLeft + margin_left + captionMargin, py);
	m_iconLCDGamma.SetShowScale(0.65f);

	//Title & silder
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_tagTitleGamma.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleGamma.SetNotifyFunc(CPanelSettingDisplay::OnNotify, this);
	m_tagTitleGamma.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleGamma.SetCaption(GetResourceString(IDS_SETTING_LCD_GAMMA));
	m_tagTitleGamma.LinkTextGL(pTextGL);
	m_tagTitleGamma.SetPos(margin_left/2,py);
	m_tagTitleGamma.SetCaptionPos(BTN_CAPTION_LEFT, margin_left + captionMargin);
	m_tagTitleGamma.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliGamma.Initialize(IDC_SETTING_SLI_LCD_GAMMA,this,&sli,pSkinManager);
	m_sliGamma.SetRange(CVTE_DEF_LCD_GAMMA_MIN,CVTE_DEF_LCD_GAMMA_MAX);
	m_sliGamma.SetValue(0);
	m_sliGamma.SetPos(m_siCurSize.cx/2-126,py);
	m_sliGamma.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliGamma.LinkTextGL(pTextGL);
	m_sliGamma.SetCaptionColor(&cr_dark);
	m_sliGamma.ShowValue(TRUE);
	m_sliGamma.EnableMoveNotify(TRUE);
	m_sliGamma.EnbaleClickOnlyThumb(TRUE);

	py-=space;

	//LCD Brightness day mode

	//day mode icon
	m_iconLCDDaymode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_098));
	m_iconLCDDaymode.SetPos(xNewPosLeft + margin_left + captionMargin, py);
	m_iconLCDDaymode.SetShowScale(0.65f);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	//title
	m_tagTitleLCDBrightness.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleLCDBrightness.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLCDBrightness.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_BRIGHTNESS));
	m_tagTitleLCDBrightness.LinkTextGL(pTextGL);
	m_tagTitleLCDBrightness.SetPos(margin_left/2,py);
	m_tagTitleLCDBrightness.SetCaptionHeight(caption_h);
	m_tagTitleLCDBrightness.SetCaptionPos(BTN_CAPTION_LEFT, margin_left + captionMargin);
	m_tagTitleLCDBrightness.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliLCDBrightness.Initialize(IDC_SETTING_SLI_LCD_BRIGHTNESS,this,&sli,pSkinManager);
	m_sliLCDBrightness.SetRange(CVTE_DEF_LCD_BRIGHTNESS_SILDER_MIN,CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX);
	m_sliLCDBrightness.SetValue(10);
	m_sliLCDBrightness.SetPos(m_siCurSize.cx/2-126,py);
	m_sliLCDBrightness.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliLCDBrightness.LinkTextGL(pTextGL);
	m_sliLCDBrightness.SetCaptionColor(&cr_dark);
	m_sliLCDBrightness.ShowValue(TRUE);
	m_sliLCDBrightness.EnableMoveNotify(TRUE);
	m_sliLCDBrightness.EnbaleClickOnlyThumb(TRUE);

	py-=space;

	//LCD Brightness night mode 

	//night mode icon
	m_iconLCDNightmode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_099));
	m_iconLCDNightmode.SetPos(xNewPosLeft + margin_left + captionMargin, py);
	m_iconLCDNightmode.SetShowScale(0.65f);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_tagTitleLCDBrightnessnight.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleLCDBrightnessnight.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLCDBrightnessnight.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_BRIGHTNESS));
	m_tagTitleLCDBrightnessnight.LinkTextGL(pTextGL);
	m_tagTitleLCDBrightnessnight.SetPos(margin_left/2,py);
	m_tagTitleLCDBrightnessnight.SetCaptionHeight(caption_h);
	m_tagTitleLCDBrightnessnight.SetCaptionPos(BTN_CAPTION_LEFT, margin_left + captionMargin);
	m_tagTitleLCDBrightnessnight.SetCaptionColor(&cr_gray, &cr_gray);

	m_sliLCDBrightnessnight.Initialize(IDC_SETTING_SLI_LCD_BRIGHTNESS_NIGHT,this,&sli,pSkinManager);
	m_sliLCDBrightnessnight.SetRange(CVTE_DEF_LCD_BRIGHTNESS_SILDER_MIN,CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX);
	m_sliLCDBrightnessnight.SetValue(10);
	m_sliLCDBrightnessnight.SetPos(m_siCurSize.cx/2-126,py);
	m_sliLCDBrightnessnight.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliLCDBrightnessnight.LinkTextGL(pTextGL);
	m_sliLCDBrightnessnight.SetCaptionColor(&cr_dark);
	m_sliLCDBrightnessnight.ShowValue(TRUE);
	m_sliLCDBrightnessnight.EnableMoveNotify(TRUE);

	py-=space;

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
	//LED Title
	m_tagTitleLED.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleLED.SetSize(cx,gridHeight);
	m_tagTitleLED.SetCaption(_T("LED"));
	m_tagTitleLED.LinkTextGL(pTextGL);
	m_tagTitleLED.SetPos(0,py);
	m_tagTitleLED.SetCaptionHeight(caption_h);
	m_tagTitleLED.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitleLED.SetCaptionColor(&cr_gray, &cr_gray);

	py-=space;

	m_tagTitlePanelLight.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitlePanelLight.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitlePanelLight.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT));
	m_tagTitlePanelLight.LinkTextGL(pTextGL);
	m_tagTitlePanelLight.SetPos(margin_left/2,py);
	m_tagTitlePanelLight.SetCaptionHeight(caption_h);
	m_tagTitlePanelLight.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
	m_tagTitlePanelLight.SetCaptionColor(&cr_gray, &cr_gray);


	int x_space = 55;
	m_btnLEDColor1.Initialize(IDC_SETTING_BTN_LED_COLOR1,this,m_pSkinManager->GetTexture(TEXID_671),NULL,
		m_pSkinManager->GetTexture(TEXID_672),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor1.SetSize(64,64);
	m_btnLEDColor1.SetPos(xnewPosRight - x_space * 6,py);
	m_btnLEDColor1.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	//m_btnLEDColor1.Show(FALSE);

	m_btnLEDColor2.Initialize(IDC_SETTING_BTN_LED_COLOR2,this,m_pSkinManager->GetTexture(TEXID_673),NULL,
		m_pSkinManager->GetTexture(TEXID_674),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor2.SetSize(64,64);
	m_btnLEDColor2.SetPos(xnewPosRight - x_space* 5,py);
	m_btnLEDColor2.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	//m_btnLEDColor2.Show(FALSE);

	m_btnLEDColor3.Initialize(IDC_SETTING_BTN_LED_COLOR3,this,m_pSkinManager->GetTexture(TEXID_675),NULL,
		m_pSkinManager->GetTexture(TEXID_676),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor3.SetSize(64,64);
	m_btnLEDColor3.SetPos(xnewPosRight - x_space * 4,py);
	m_btnLEDColor3.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	//m_btnLEDColor3.Show(FALSE);

	m_btnLEDColor4.Initialize(IDC_SETTING_BTN_LED_COLOR4,this,m_pSkinManager->GetTexture(TEXID_677),NULL,
		m_pSkinManager->GetTexture(TEXID_678),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor4.SetSize(64,64);
	m_btnLEDColor4.SetPos(xnewPosRight - x_space * 3,py);
	m_btnLEDColor4.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	//m_btnLEDColor4.Show(FALSE);

	m_btnLEDColor5.Initialize(IDC_SETTING_BTN_LED_COLOR5,this,m_pSkinManager->GetTexture(TEXID_679),NULL,
		m_pSkinManager->GetTexture(TEXID_680),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor5.SetSize(64,64);
	m_btnLEDColor5.SetPos(xnewPosRight - x_space * 2,py);
	m_btnLEDColor5.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	//m_btnLEDColor5.Show(FALSE);

	m_btnLEDColorUser.Initialize(IDC_SETTING_BTN_LED_COLOR_USER,this,m_pSkinManager->GetTexture(TEXID_698),NULL,
		m_pSkinManager->GetTexture(TEXID_699),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColorUser.SetSize(64,64);
	m_btnLEDColorUser.SetPos(xnewPosRight - x_space,py);
	m_btnLEDColorUser.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	//m_btnLEDColorUser.Show(FALSE);

	py-=tag_h;

	//Led Color Title
	m_tagTitleLEDColor.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_289),NULL,
		NULL,NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagTitleLEDColor.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLEDColor.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_COLOR));
	m_tagTitleLEDColor.LinkTextGL(pTextGL);
	m_tagTitleLEDColor.SetPos(margin_left/2,py);
	m_tagTitleLEDColor.SetCaptionHeight(caption_h);
	m_tagTitleLEDColor.SetCaptionPos(BTN_CAPTION_LEFT, margin_left + captionMargin);
	m_tagTitleLEDColor.SetCaptionColor(&cr_gray, &cr_gray);

	//Led Color
	//led color icon
	m_iconLEDColor.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_681));
	m_iconLEDColor.SetPos(xNewPosLeft + margin_left + captionMargin, py);
	m_iconLEDColor.SetShowScale(0.65f);

	GLfloat texCoord_sLEDColor[]={
		4.0/512.0, 4.0/128.0,
		4.0/512.0, 29.0/128.0,
		328.0/512.0, 4.0/128.0,
		328.0/512.0, 29.0/128.0};

		m_tagLEDColor.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_683),texCoord_sLEDColor);
		m_tagLEDColor.SetPos(xnewPosRight - 200, py);


		//Led Color Silder
		SLIDERSTRUCTURE sliColor;
		memset(&sliColor,0,sizeof(sliColor));
		sliColor.size.cx=300;
		sliColor.size.cy=32;
		sliColor.bHasBtns=FALSE;
		sliColor.bFilled=FALSE;
		sliColor.bFilledBoth=FALSE;
		sliColor.bNoBorder = TRUE;
		sliColor.idBackTex=NULL;
		sliColor.idChannelTex=NULL;
		sliColor.idThumbTex=TEXID_640;
		m_sliLEDColor.Initialize(IDC_SETTING_SLI_LED_COLOR,this,&sliColor,pSkinManager);
		m_sliLEDColor.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
		m_sliLEDColor.SetRange(0,300);
		m_sliLEDColor.SetValue(0);
		m_sliLEDColor.SetPos(xnewPosRight - 200,py);
		m_sliLEDColor.EnableMoveNotify(TRUE);
		m_sliLEDColor.ShowValue(TRUE);
		m_sliLEDColor.EnbaleClickOnlyThumb(TRUE);
		//m_sliLEDColor.Show(FALSE);

		py-=tag_h;

		//Led Brightness day mode Title
		//day mode icon
		m_iconLEDDaymode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_098));
		m_iconLEDDaymode.SetPos(xNewPosLeft + margin_left + captionMargin, py);
		m_iconLEDDaymode.SetShowScale(0.65f);

		m_tagTitleLEDBrightness.Initialize(0,this,
			m_pSkinManager->GetTexture(TEXID_289),NULL,
			NULL,NULL,
			NULL,NULL,
			NULL,NULL);
		m_tagTitleLEDBrightness.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
		m_tagTitleLEDBrightness.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_BRIGHTNESS));
		m_tagTitleLEDBrightness.LinkTextGL(pTextGL);
		m_tagTitleLEDBrightness.SetPos(margin_left/2,py);
		m_tagTitleLEDBrightness.SetCaptionHeight(caption_h);
		m_tagTitleLEDBrightness.SetCaptionPos(BTN_CAPTION_LEFT,margin_left + captionMargin);
		m_tagTitleLEDBrightness.SetCaptionColor(&cr_gray, & cr_gray);

		//Led Brightness day mode 
		GLfloat texCoord_sLEDBrightDay[]={
			4.0/512.0, 34.0/128.0,
			4.0/512.0, 59.0/128.0,
			328.0/512.0, 34.0/128.0,
			328.0/512.0, 59.0/128.0};

			m_tagLEDBrightness.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_683),texCoord_sLEDBrightDay);
			m_tagLEDBrightness.SetPos(xnewPosRight - 200,py);
			//m_tagLEDBrightness.Show(FALSE);

			//Led Brightness day mode Silder
			SLIDERSTRUCTURE sliBrightness;
			memset(&sliBrightness,0,sizeof(sliBrightness));
			sliBrightness.size.cx=300;//520/*776*/;
			sliBrightness.size.cy=32;
			sliBrightness.bHasBtns=FALSE;
			sliBrightness.bFilled=FALSE;
			sliBrightness.bFilledBoth=FALSE;
			sliBrightness.bNoBorder = TRUE;
			sliBrightness.idBackTex=NULL;
			sliBrightness.idChannelTex=NULL;
			sliBrightness.idThumbTex=TEXID_640;
			m_sliLEDBrightness.Initialize(IDC_SETTING_SLI_LED_BRIGHTNESS,this,&sliBrightness,pSkinManager);
			m_sliLEDBrightness.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
			m_sliLEDBrightness.SetRange(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MIN,CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX);
			m_sliLEDBrightness.SetValue(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX);
			m_sliLEDBrightness.SetPos(xnewPosRight - 200, py);
			m_sliLEDBrightness.EnableMoveNotify(TRUE);
			m_sliLEDBrightness.ShowValue(TRUE);
			m_sliLEDBrightness.EnbaleClickOnlyThumb(TRUE);

			py-=tag_h;

			//Led Brightness night mode Title
			//Night mode icon
			m_iconLEDNightmode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_099));
			m_iconLEDNightmode.SetPos(xNewPosLeft + margin_left + captionMargin, py);
			m_iconLEDNightmode.SetShowScale(0.65f);

			m_tagTitleLEDBrightnessnight.Initialize(0,this,
				m_pSkinManager->GetTexture(TEXID_289),NULL,
				NULL,NULL,
				NULL,NULL,
				NULL,NULL);
			m_tagTitleLEDBrightnessnight.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
			m_tagTitleLEDBrightnessnight.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_BRIGHTNESS));
			m_tagTitleLEDBrightnessnight.LinkTextGL(pTextGL);
			m_tagTitleLEDBrightnessnight.SetPos(margin_left/2,py);
			m_tagTitleLEDBrightnessnight.SetCaptionHeight(caption_h);
			m_tagTitleLEDBrightnessnight.SetCaptionPos(BTN_CAPTION_LEFT, margin_left + captionMargin);
			m_tagTitleLEDBrightnessnight.SetCaptionColor(&cr_gray, &cr_gray);

			//Led Brightness night mode
			GLfloat texCoord_sLEDBrightNight[]={
				4.0/512.0, 63.0/128.0,
				4.0/512.0, 88.0/128.0,
				328.0/512.0, 63.0/128.0,
				328.0/512.0, 88.0/128.0};

				m_tagLEDBrightnessnight.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_683),texCoord_sLEDBrightNight);
				m_tagLEDBrightnessnight.SetPos(xnewPosRight - 200,py);
				//m_tagLEDBrightnessnight.Show(FALSE);

				//Led Brightness night mode Silder
				memset(&sliBrightness,0,sizeof(sliBrightness));
				sliBrightness.size.cx=300;//520/*776*/;
				sliBrightness.size.cy=32;
				sliBrightness.bHasBtns=FALSE;
				sliBrightness.bFilled=FALSE;
				sliBrightness.bFilledBoth=FALSE;
				sliBrightness.bNoBorder = TRUE;
				sliBrightness.idBackTex=NULL;
				sliBrightness.idChannelTex=NULL;
				sliBrightness.idThumbTex=TEXID_640;

				m_sliLEDBrightnessnight.Initialize(IDC_SETTING_SLI_LED_BRIGHTNESS_NIGHT,this,&sliBrightness,pSkinManager);
				m_sliLEDBrightnessnight.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
				m_sliLEDBrightnessnight.SetRange(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MIN,CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX);
				m_sliLEDBrightnessnight.SetValue(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX);
				m_sliLEDBrightnessnight.SetPos(xnewPosRight - 200, py);
				m_sliLEDBrightnessnight.EnableMoveNotify(TRUE);
				m_sliLEDBrightnessnight.ShowValue(TRUE);
				m_sliLEDBrightnessnight.EnbaleClickOnlyThumb(TRUE);

				py-=tag_h;

				//LED animation effects
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

				m_tagTitleLedAnimationEffect.Initialize(0,this,
					m_pSkinManager->GetTexture(TEXID_289),NULL,
					NULL,NULL,
					NULL,NULL,
					NULL,NULL);
				m_tagTitleLedAnimationEffect.SetSize((g_iClientWidth-box_size_x-margin_x*2), tag_h);
				m_tagTitleLedAnimationEffect.SetCaption(GetResourceString(IDS_SETTING_LED_ANIM_EFFECT));
				m_tagTitleLedAnimationEffect.LinkTextGL(pTextGL);
				m_tagTitleLedAnimationEffect.SetPos(margin_left/2, py);
				m_tagTitleLedAnimationEffect.SetCaptionHeight(caption_h);
				m_tagTitleLedAnimationEffect.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
				m_tagTitleLedAnimationEffect.SetCaptionColor(&cr_gray, &cr_gray);

				m_sliLedAnimationEffect.Initialize(IDC_SETTING_SLI_LED_ANIMATION_EFFECT,this,&sli,pSkinManager);
				m_sliLedAnimationEffect.SetRange(0,10);
				m_sliLedAnimationEffect.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bLedAnimEffect?10:0);
				m_sliLedAnimationEffect.SetPos(m_siCurSize.cx/2-70,py);
				m_sliLedAnimationEffect.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
				m_sliLedAnimationEffect.LinkTextGL(pTextGL);
				m_sliLedAnimationEffect.SetCaptionColor(&cr_gray);
				m_sliLedAnimationEffect.ShowValue(FALSE);

				py-=tag_h;
#endif

				m_tagTitleReset.Initialize(0,this,
					m_pSkinManager->GetTexture(TEXID_289),NULL,
					m_pSkinManager->GetTexture(TEXID_289),NULL,
					NULL,NULL,
					NULL,NULL);
				m_tagTitleReset.SetSize(cx,gridHeight);
				m_tagTitleReset.SetCaption(GetResourceString(IDS_SETTING_UI_DEFAULT));
				m_tagTitleReset.LinkTextGL(pTextGL);
				m_tagTitleReset.SetPos(0,py);
				m_tagTitleReset.SetCaptionHeight(caption2_h);
				m_tagTitleReset.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
				m_tagTitleReset.SetCaptionColor(&cr_gray, &cr_gray);

				py-=tag_h;

				//Reset to Default
				m_btnReset.Initialize(IDC_SETTING_SLI_DISPLAY_RESET,this,
					m_pSkinManager->GetTexture(TEXID_289),NULL,
					m_pSkinManager->GetTexture(TEXID_241),NULL,
					NULL,NULL,
					NULL,NULL);
				m_btnReset.SetSize(cx - margin_left, tag_h);
				m_btnReset.SetCaption(GetResourceString(IDS_RESET_DEFAULT));
				m_btnReset.LinkTextGL(pTextGL);
				m_btnReset.SetPos(margin_x/2, py);
				m_btnReset.SetCaptionHeight(caption_h);
				m_btnReset.SetCaptionPos(BTN_CAPTION_LEFT, captionMargin);
				m_btnReset.SetCaptionColor(&cr_gray, &cr_gray);
				m_btnReset.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);

				/////////////////////////////////////////////////////////////////////////

				if(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault == 0)
				{
					m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault = 130;
				}

				//set value:
				if(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaCurrent != 0)
				{
					SetGamma((DOUBLE)(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaCurrent)/100.0);
					m_sliGamma.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaCurrent-50);
				}
				else
				{
					SetGamma((DOUBLE)(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault)/100.0);
					m_sliGamma.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault-50);
				}

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
				if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent==0)
				{
					m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessDefault;
				}

				FreshLedSilder();
				EnableLedAnimEffect(m_pSkinManager->GetConfig()->GetCurParam()->bLedAnimEffect);
#endif


#else
	//large size:
#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
	g_nLedHeight = LIST_BOX_TAG_HEIGHT*5+60;
#else
	g_nLedHeight = 0;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	g_nLcdHeight = LIST_BOX_TAG_HEIGHT*5+60;
#else
	g_nLcdHeight = LIST_BOX_TAG_HEIGHT*4+60;
#endif	
	g_nResetHeight = LIST_BOX_TAG_HEIGHT+60;
	g_nPanelHeight = g_nLcdHeight+g_nLedHeight+g_nResetHeight+20;

	if(g_nPanelHeight<=g_iClientHeight)
		g_nPanelHeight = g_iClientHeight;

	SetSize(g_iClientWidth-box_size_x,g_nPanelHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
    COLORGL cr_gray2={0.35,0.35,0.35,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={0.0f/256.0f,144.0f/256.0f,255.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	int margin_x=12;
	int cx=(g_iClientWidth-box_size_x-margin_x*2)/2;
	//int cy=cx;
    int tag_h = LIST_BOX_TAG_HEIGHT;

    int caption_h = 22;
    int caption2_h = 18;
    int caption3_h = 14;

    if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
    {
        caption_h = 20;
        caption2_h = 16;
        caption3_h = 14;
    }

	GLfloat py=g_iClientHeight/2-40;
	SLIDERSTRUCTURE sli;
	//Screen title:
	m_tagTitleScreen.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleScreen.SetSize(cx,40);
	m_tagTitleScreen.SetCaption(GetResourceString(IDS_SETTING_DISPLAY_TITLE_LCD));
	m_tagTitleScreen.LinkTextGL(pTextGL);
	m_tagTitleScreen.SetPos(-cx/2-10 +12,py);
	m_tagTitleScreen.SetCaptionHeight(20);
	m_tagTitleScreen.EnableDrawBack(FALSE);
	m_tagTitleScreen.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleScreen.SetCaptionColor(&cr_dark);

	py-=60;

	//Screen panel
	m_tagPanelScreen.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_tagPanelScreen.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h*5);
	m_tagPanelScreen.SetPos(0,py-LIST_BOX_TAG_HEIGHT-LIST_BOX_TAG_HEIGHT);
#else
	m_tagPanelScreen.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h*4);
	m_tagPanelScreen.SetPos(0,py-LIST_BOX_TAG_HEIGHT-LIST_BOX_TAG_HEIGHT/2);
#endif
	

	//Blur
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

	m_tagTitleBlurEffect.Initialize(0,this,NULL/*m_pSkinManager->GetTexture(TEXID_606)*/,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleBlurEffect.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleBlurEffect.SetCaption(GetResourceString(IDS_SETTING_BLUR_EFFECT));
	m_tagTitleBlurEffect.LinkTextGL(pTextGL);
	m_tagTitleBlurEffect.SetPos(0,py);
	m_tagTitleBlurEffect.SetCaptionHeight(caption_h);
	m_tagTitleBlurEffect.EnableDrawBack(FALSE);
	m_tagTitleBlurEffect.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleBlurEffect.SetCaptionColor(&cr_gray);

	m_sliBlurEffect.Initialize(IDC_SETTING_SLI_BLUR_EFFECT,this,&sli,pSkinManager);
	m_sliBlurEffect.SetRange(0,10);
	m_sliBlurEffect.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect?10:0);
	m_sliBlurEffect.SetPos(m_siCurSize.cx/2-70,py);
	m_sliBlurEffect.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliBlurEffect.LinkTextGL(pTextGL);
	m_sliBlurEffect.SetCaptionColor(&cr_gray);
	m_sliBlurEffect.ShowValue(FALSE);

	py-=tag_h;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	//lamp control:
	m_tagTitleLampControl.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
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
	m_sliLampControl.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);

	py-=tag_h;
#endif

	//Gamma:
	//LCD Gamma icon
	m_iconLCDGamma.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_682));
	m_iconLCDGamma.SetPos(-cx+30,py);
	m_iconLCDGamma.SetShowScale(0.65f);

	//Title & silder
	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;


	m_tagTitleGamma.Initialize(0,this,NULL/*m_pSkinManager->GetTexture(TEXID_606)*/,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleGamma.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleGamma.SetCaption(GetResourceString(IDS_SETTING_LCD_GAMMA));
	m_tagTitleGamma.LinkTextGL(pTextGL);
	m_tagTitleGamma.SetPos(40,py);
	m_tagTitleGamma.SetCaptionHeight(caption_h);
	m_tagTitleGamma.EnableDrawBack(FALSE);
	m_tagTitleGamma.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleGamma.SetCaptionColor(&cr_gray);

	m_sliGamma.Initialize(IDC_SETTING_SLI_LCD_GAMMA,this,&sli,pSkinManager);
	m_sliGamma.SetRange(CVTE_DEF_LCD_GAMMA_MIN,CVTE_DEF_LCD_GAMMA_MAX);
	m_sliGamma.SetValue(0);
	m_sliGamma.SetPos(m_siCurSize.cx/2-144,py);
	m_sliGamma.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliGamma.LinkTextGL(pTextGL);
	m_sliGamma.SetCaptionColor(&cr_gray);
	m_sliGamma.ShowValue(TRUE);
	m_sliGamma.EnableMoveNotify(TRUE);
	m_sliGamma.EnbaleClickOnlyThumb(TRUE);

    py-=tag_h;

	//LCD Brightness day mode

	//day mode icon
	m_iconLCDDaymode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_098));
	m_iconLCDDaymode.SetPos(-cx+30,py);
	m_iconLCDDaymode.SetShowScale(0.65f);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	//title
	const wchar_t *pstrDay = GetResourceString(IDS_DAY);
	const wchar_t *pstrNight = GetResourceString(IDS_NIGHT);

	m_tagTitleLCDBrightness.Initialize(0,this,NULL/*m_pSkinManager->GetTexture(TEXID_606)*/,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLCDBrightness.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLCDBrightness.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_BRIGHTNESS));
	m_tagTitleLCDBrightness.SetSubCaption(pstrDay);
	m_tagTitleLCDBrightness.LinkTextGL(pTextGL);
	m_tagTitleLCDBrightness.SetPos(40,py);
	m_tagTitleLCDBrightness.SetCaptionHeight(caption_h);
	m_tagTitleLCDBrightness.EnableDrawBack(FALSE);
	m_tagTitleLCDBrightness.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h,pstrDay?10:0);
	m_tagTitleLCDBrightness.SetCaptionColor(&cr_gray);
	m_tagTitleLCDBrightness.SetSubCaptionPos(BTN_CAPTION_LEFT,caption2_h,4);

	m_sliLCDBrightness.Initialize(IDC_SETTING_SLI_LCD_BRIGHTNESS,this,&sli,pSkinManager);
	m_sliLCDBrightness.SetRange(CVTE_DEF_LCD_BRIGHTNESS_SILDER_MIN,CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX);
	m_sliLCDBrightness.SetValue(10);
	m_sliLCDBrightness.SetPos(m_siCurSize.cx/2-144,py);
	m_sliLCDBrightness.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliLCDBrightness.LinkTextGL(pTextGL);
	m_sliLCDBrightness.SetCaptionColor(&cr_gray);
	m_sliLCDBrightness.ShowValue(TRUE);
	m_sliLCDBrightness.EnableMoveNotify(TRUE);
	m_sliLCDBrightness.EnbaleClickOnlyThumb(TRUE);

    py-=tag_h;

	//LCD Brightness night mode 

	//night mode icon
	m_iconLCDNightmode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_099));
	m_iconLCDNightmode.SetPos(-cx+30,py);
	m_iconLCDNightmode.SetShowScale(0.65f);

	memset(&sli,0,sizeof(sli));
	sli.size.cx=SLI_SETTING_WIDTH;
	sli.size.cy=SLI_SETTING_THICKNESS;
	sli.bHasBtns=FALSE;
	sli.bFilled=FALSE;
	sli.bFilledBoth=FALSE;
	sli.idBackTex=TEXID_668;
	sli.idChannelTex=TEXID_667;
	sli.idThumbTex=TEXID_640;

	m_tagTitleLCDBrightnessnight.Initialize(0,this,NULL/*m_pSkinManager->GetTexture(TEXID_606)*/,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLCDBrightnessnight.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLCDBrightnessnight.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_BRIGHTNESS));	
	m_tagTitleLCDBrightnessnight.SetSubCaption(pstrNight);
	m_tagTitleLCDBrightnessnight.LinkTextGL(pTextGL);
	m_tagTitleLCDBrightnessnight.SetPos(40,py);
	m_tagTitleLCDBrightnessnight.SetCaptionHeight(caption_h);
	m_tagTitleLCDBrightnessnight.EnableDrawBack(FALSE);
	m_tagTitleLCDBrightnessnight.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h,pstrNight?10:0);
	m_tagTitleLCDBrightnessnight.SetCaptionColor(&cr_gray);
	m_tagTitleLCDBrightnessnight.SetSubCaptionPos(BTN_CAPTION_LEFT,caption2_h,4);

	m_sliLCDBrightnessnight.Initialize(IDC_SETTING_SLI_LCD_BRIGHTNESS_NIGHT,this,&sli,pSkinManager);
	m_sliLCDBrightnessnight.SetRange(CVTE_DEF_LCD_BRIGHTNESS_SILDER_MIN,CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX);
	m_sliLCDBrightnessnight.SetValue(10);
	m_sliLCDBrightnessnight.SetPos(m_siCurSize.cx/2-144,py);
	m_sliLCDBrightnessnight.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliLCDBrightnessnight.LinkTextGL(pTextGL);
	m_sliLCDBrightnessnight.SetCaptionColor(&cr_gray);
	m_sliLCDBrightnessnight.ShowValue(TRUE);
	m_sliLCDBrightnessnight.EnableMoveNotify(TRUE);
	//m_sliLCDBrightnessnight.Enable(FALSE);

	py-=tag_h;

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
	//LED Title
	m_tagTitleLED.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLED.SetSize(cx,40);
	if(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage == LANGUAGE_RUSSIAN)
		m_tagTitleLED.SetCaption(_T("§¬§ß§à§á§Ü§Ú"));
	else
		m_tagTitleLED.SetCaption(_T("LED"));
	m_tagTitleLED.LinkTextGL(pTextGL);
	m_tagTitleLED.SetPos(-cx/2-10 +12,py);
	m_tagTitleLED.SetCaptionHeight(20);
	m_tagTitleLED.EnableDrawBack(FALSE);
	m_tagTitleLED.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleLED.SetCaptionColor(&cr_dark);

	py-=60;

	//LED panel light
    m_tagPanelLight.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPanelLight.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h*5);
	//m_tagPanelLight.SetPos(0,py);
    m_tagPanelLight.SetPos(0,py-LIST_BOX_TAG_HEIGHT*2);

    m_tagTitlePanelLight.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitlePanelLight.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitlePanelLight.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT));
	m_tagTitlePanelLight.LinkTextGL(pTextGL);
	m_tagTitlePanelLight.SetPos(0,py/*+12*/);
	m_tagTitlePanelLight.SetCaptionHeight(caption_h);
	m_tagTitlePanelLight.EnableDrawBack(FALSE);
	m_tagTitlePanelLight.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitlePanelLight.SetCaptionColor(&cr_dark);

//     m_tagTitlePanelLight2.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
// 	m_tagTitlePanelLight2.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
// 	m_tagTitlePanelLight2.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_DISC));
// 	m_tagTitlePanelLight2.LinkTextGL(pTextGL);
// 	m_tagTitlePanelLight2.SetPos(0,py-16);
// 
//     switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
//     {
//         case LANGUAGE_SPANISH:
//             m_tagTitlePanelLight2.SetCaptionHeight(caption3_h-2);
//             break;
//         case LANGUAGE_ITALIAN:
//             m_tagTitlePanelLight2.SetCaptionHeight(caption3_h-2);
//             break;
//         default:
//             m_tagTitlePanelLight2.SetCaptionHeight(caption3_h);
//             break;
//     }
// 
// 
// 	m_tagTitlePanelLight2.EnableDrawBack(FALSE);
// 	m_tagTitlePanelLight2.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
// 	m_tagTitlePanelLight2.SetCaptionColor(&cr_gray2);

//	    m_btnLEDColorCustom.Initialize(IDC_SETTING_BTN_LED_COLOR_CUSTOM,this,m_pSkinManager->GetTexture(TEXID_681),NULL,
//			m_pSkinManager->GetTexture(TEXID_682),NULL,NULL,NULL,NULL,NULL);
//		m_btnLEDColorCustom.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
//		m_btnLEDColorCustom.SetPos(cx-64/2,py);
//	    m_btnLEDColorCustom.SetSize(64,64);
//		m_btnLEDColorCustom.EnableClickAni(FALSE);
//
//	    py-=tag_h;

    m_btnLEDColor1.Initialize(IDC_SETTING_BTN_LED_COLOR1,this,m_pSkinManager->GetTexture(TEXID_671),NULL,
		m_pSkinManager->GetTexture(TEXID_672),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor1.SetSize(64,64);
	m_btnLEDColor1.SetPos(cx-64/2-54*5,py);
	m_btnLEDColor1.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
    //m_btnLEDColor1.Show(FALSE);

    m_btnLEDColor2.Initialize(IDC_SETTING_BTN_LED_COLOR2,this,m_pSkinManager->GetTexture(TEXID_673),NULL,
		m_pSkinManager->GetTexture(TEXID_674),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor2.SetSize(64,64);
	m_btnLEDColor2.SetPos(cx-64/2-54*4,py);
	m_btnLEDColor2.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
    //m_btnLEDColor2.Show(FALSE);

    m_btnLEDColor3.Initialize(IDC_SETTING_BTN_LED_COLOR3,this,m_pSkinManager->GetTexture(TEXID_675),NULL,
		m_pSkinManager->GetTexture(TEXID_676),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor3.SetSize(64,64);
	m_btnLEDColor3.SetPos(cx-64/2-54*3,py);
	m_btnLEDColor3.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
    //m_btnLEDColor3.Show(FALSE);

    m_btnLEDColor4.Initialize(IDC_SETTING_BTN_LED_COLOR4,this,m_pSkinManager->GetTexture(TEXID_677),NULL,
		m_pSkinManager->GetTexture(TEXID_678),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor4.SetSize(64,64);
	m_btnLEDColor4.SetPos(cx-64/2-54*2,py);
	m_btnLEDColor4.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
    //m_btnLEDColor4.Show(FALSE);

    m_btnLEDColor5.Initialize(IDC_SETTING_BTN_LED_COLOR5,this,m_pSkinManager->GetTexture(TEXID_679),NULL,
		m_pSkinManager->GetTexture(TEXID_680),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColor5.SetSize(64,64);
	m_btnLEDColor5.SetPos(cx-64/2-54*1,py);
	m_btnLEDColor5.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
    //m_btnLEDColor5.Show(FALSE);

    m_btnLEDColorUser.Initialize(IDC_SETTING_BTN_LED_COLOR_USER,this,m_pSkinManager->GetTexture(TEXID_698),NULL,
		m_pSkinManager->GetTexture(TEXID_699),NULL,NULL,NULL,NULL,NULL);
	m_btnLEDColorUser.SetSize(64,64);
	m_btnLEDColorUser.SetPos(cx-64/2,py);
	m_btnLEDColorUser.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
    //m_btnLEDColorUser.Show(FALSE);

    py-=tag_h;

    //Led Color Title
    m_tagTitleLEDColor.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLEDColor.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLEDColor.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_COLOR));
	m_tagTitleLEDColor.LinkTextGL(pTextGL);
	m_tagTitleLEDColor.SetPos(40,py);
    m_tagTitleLEDColor.SetCaptionHeight(caption_h);
	m_tagTitleLEDColor.EnableDrawBack(FALSE);
	m_tagTitleLEDColor.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleLEDColor.SetCaptionColor(&cr_dark);

    //Led Color
	//led color icon
	m_iconLEDColor.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_681));
	m_iconLEDColor.SetPos(-cx+30,py);
	m_iconLEDColor.SetShowScale(0.65f);

	GLfloat texCoord_sLEDColor[]={
		4.0/512.0, 4.0/128.0,
		4.0/512.0, 29.0/128.0,
		328.0/512.0, 4.0/128.0,
		328.0/512.0, 29.0/128.0};

	m_tagLEDColor.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_683),texCoord_sLEDColor);
	m_tagLEDColor.SetPos(cx-64/2-54*2-40,py);


    //Led Color Silder
	SLIDERSTRUCTURE sliColor;
	memset(&sliColor,0,sizeof(sliColor));
	sliColor.size.cx=300;	//314;//520/*776*/;
	sliColor.size.cy=32;
	sliColor.bHasBtns=FALSE;
	sliColor.bFilled=FALSE;
	sliColor.bFilledBoth=FALSE;
    sliColor.bNoBorder = TRUE;
	sliColor.idBackTex=NULL;
	sliColor.idChannelTex=NULL;
	sliColor.idThumbTex=TEXID_640;
	m_sliLEDColor.Initialize(IDC_SETTING_SLI_LED_COLOR,this,&sliColor,pSkinManager);
	m_sliLEDColor.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliLEDColor.SetRange(0,300);
	m_sliLEDColor.SetValue(0);
	m_sliLEDColor.SetPos(cx-64/2-54*2-40,py);
	m_sliLEDColor.EnableMoveNotify(TRUE);
	m_sliLEDColor.ShowValue(TRUE);
    m_sliLEDColor.EnbaleClickOnlyThumb(TRUE);
    //m_sliLEDColor.Show(FALSE);

    py-=tag_h;

    //Led Brightness day mode Title
	//day mode icon
	m_iconLEDDaymode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_098));
	m_iconLEDDaymode.SetPos(-cx+30,py);
	m_iconLEDDaymode.SetShowScale(0.65f);

    m_tagTitleLEDBrightness.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLEDBrightness.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLEDBrightness.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_BRIGHTNESS));
	m_tagTitleLEDBrightness.SetSubCaption(pstrDay);
	m_tagTitleLEDBrightness.LinkTextGL(pTextGL);
	m_tagTitleLEDBrightness.SetPos(40,py);
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_GREEK:
		m_tagTitleLEDBrightness.SetCaptionHeight(caption2_h);
		break;
	default:
		m_tagTitleLEDBrightness.SetCaptionHeight(caption_h);
		break;
	}	
	m_tagTitleLEDBrightness.EnableDrawBack(FALSE);
	m_tagTitleLEDBrightness.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h, pstrDay?10:0);
	m_tagTitleLEDBrightness.SetCaptionColor(&cr_dark);
	m_tagTitleLEDBrightness.SetSubCaptionPos(BTN_CAPTION_LEFT,caption2_h,4);

    //Led Brightness day mode 
	GLfloat texCoord_sLEDBrightDay[]={
		4.0/512.0, 34.0/128.0,
		4.0/512.0, 59.0/128.0,
		328.0/512.0, 34.0/128.0,
		328.0/512.0, 59.0/128.0};

	m_tagLEDBrightness.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_683),texCoord_sLEDBrightDay);
	m_tagLEDBrightness.SetPos(cx-64/2-54*2-40,py);
    //m_tagLEDBrightness.Show(FALSE);

    //Led Brightness day mode Silder
	SLIDERSTRUCTURE sliBrightness;
	memset(&sliBrightness,0,sizeof(sliBrightness));
	sliBrightness.size.cx=300;//520/*776*/;
	sliBrightness.size.cy=32;
	sliBrightness.bHasBtns=FALSE;
	sliBrightness.bFilled=FALSE;
	sliBrightness.bFilledBoth=FALSE;
    sliBrightness.bNoBorder = TRUE;
	sliBrightness.idBackTex=NULL;
	sliBrightness.idChannelTex=NULL;
	sliBrightness.idThumbTex=TEXID_640;
	m_sliLEDBrightness.Initialize(IDC_SETTING_SLI_LED_BRIGHTNESS,this,&sliBrightness,pSkinManager);
	m_sliLEDBrightness.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliLEDBrightness.SetRange(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MIN,CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX);
	m_sliLEDBrightness.SetValue(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX);
	m_sliLEDBrightness.SetPos(cx-64/2-54*2-40,py);
	m_sliLEDBrightness.EnableMoveNotify(TRUE);
	m_sliLEDBrightness.ShowValue(TRUE);
    m_sliLEDBrightness.EnbaleClickOnlyThumb(TRUE);

    py-=tag_h;

	//Led Brightness night mode Title
	//Night mode icon
	m_iconLEDNightmode.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_099));
	m_iconLEDNightmode.SetPos(-cx+30,py);
	m_iconLEDNightmode.SetShowScale(0.65f);

	m_tagTitleLEDBrightnessnight.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLEDBrightnessnight.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLEDBrightnessnight.SetCaption(GetResourceString(IDS_SETTING_PANEL_LIGHT_BRIGHTNESS));
	m_tagTitleLEDBrightnessnight.SetSubCaption(pstrNight);
	m_tagTitleLEDBrightnessnight.LinkTextGL(pTextGL);
	m_tagTitleLEDBrightnessnight.SetPos(40,py);
	switch(m_pSkinManager->GetConfig()->GetCurParam()->idLanguage)
	{
	case LANGUAGE_GREEK:
		m_tagTitleLEDBrightnessnight.SetCaptionHeight(caption2_h);
		break;
	default:
		m_tagTitleLEDBrightnessnight.SetCaptionHeight(caption_h);
		break;
	}

	m_tagTitleLEDBrightnessnight.EnableDrawBack(FALSE);
	m_tagTitleLEDBrightnessnight.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h,pstrNight?10:0);
	m_tagTitleLEDBrightnessnight.SetCaptionColor(&cr_dark);
	m_tagTitleLEDBrightnessnight.SetSubCaptionPos(BTN_CAPTION_LEFT,caption2_h,4);

	//Led Brightness night mode
	GLfloat texCoord_sLEDBrightNight[]={
		4.0/512.0, 63.0/128.0,
		4.0/512.0, 88.0/128.0,
		328.0/512.0, 63.0/128.0,
		328.0/512.0, 88.0/128.0};

	m_tagLEDBrightnessnight.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_683),texCoord_sLEDBrightNight);
	m_tagLEDBrightnessnight.SetPos(cx-64/2-54*2-40,py);
	//m_tagLEDBrightnessnight.Show(FALSE);

	//Led Brightness night mode Silder
	memset(&sliBrightness,0,sizeof(sliBrightness));
	sliBrightness.size.cx=300;//520/*776*/;
	sliBrightness.size.cy=32;
	sliBrightness.bHasBtns=FALSE;
	sliBrightness.bFilled=FALSE;
	sliBrightness.bFilledBoth=FALSE;
	sliBrightness.bNoBorder = TRUE;
	sliBrightness.idBackTex=NULL;
	sliBrightness.idChannelTex=NULL;
	sliBrightness.idThumbTex=TEXID_640;

	m_sliLEDBrightnessnight.Initialize(IDC_SETTING_SLI_LED_BRIGHTNESS_NIGHT,this,&sliBrightness,pSkinManager);
	m_sliLEDBrightnessnight.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliLEDBrightnessnight.SetRange(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MIN,CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX);
	m_sliLEDBrightnessnight.SetValue(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX);
	m_sliLEDBrightnessnight.SetPos(cx-64/2-54*2-40,py);
	m_sliLEDBrightnessnight.EnableMoveNotify(TRUE);
	m_sliLEDBrightnessnight.ShowValue(TRUE);
	m_sliLEDBrightnessnight.EnbaleClickOnlyThumb(TRUE);
	//m_sliLEDBrightnessnight.Enable(FALSE);

	py-=tag_h;

	//LED animation effects
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

	m_tagTitleLedAnimationEffect.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLedAnimationEffect.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLedAnimationEffect.SetCaption(GetResourceString(IDS_SETTING_LED_ANIM_EFFECT));
	m_tagTitleLedAnimationEffect.LinkTextGL(pTextGL);
	m_tagTitleLedAnimationEffect.SetPos(0,py);
	m_tagTitleLedAnimationEffect.SetCaptionHeight(caption_h);
	m_tagTitleLedAnimationEffect.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_tagTitleLedAnimationEffect.SetCaptionColor(&cr_gray);
	m_tagTitleLedAnimationEffect.EnableDrawBack(FALSE);

	m_sliLedAnimationEffect.Initialize(IDC_SETTING_SLI_LED_ANIMATION_EFFECT,this,&sli,pSkinManager);
	m_sliLedAnimationEffect.SetRange(0,10);
	m_sliLedAnimationEffect.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->bLedAnimEffect?10:0);
	m_sliLedAnimationEffect.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLedAnimationEffect.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);
	m_sliLedAnimationEffect.LinkTextGL(pTextGL);
	m_sliLedAnimationEffect.SetCaptionColor(&cr_gray);
	m_sliLedAnimationEffect.ShowValue(FALSE);

	py-=tag_h;
#endif

	m_tagTitleReset.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleReset.SetSize(cx,40);
	m_tagTitleReset.SetCaption(GetResourceString(IDS_SETTING_UI_DEFAULT));
	m_tagTitleReset.LinkTextGL(pTextGL);
	m_tagTitleReset.SetPos(-cx/2-10 +12,py);
	m_tagTitleReset.SetCaptionHeight(20);
	m_tagTitleReset.EnableDrawBack(FALSE);
	m_tagTitleReset.SetCaptionPos(BTN_CAPTION_LEFT);
	m_tagTitleReset.SetCaptionColor(&cr_dark);

	py-=60;

    //Reset to Default
	m_btnReset.Initialize(IDC_SETTING_SLI_DISPLAY_RESET,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
		m_pSkinManager->GetTexture(TEXID_607),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnReset.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_btnReset.SetCaption(GetResourceString(IDS_RESET_DEFAULT));
	m_btnReset.LinkTextGL(pTextGL);
	m_btnReset.SetPos(0,py);
	m_btnReset.SetCaptionHeight(caption_h);
	m_btnReset.SetCaptionPos(BTN_CAPTION_LEFT,caption2_h);
	m_btnReset.SetCaptionColor(&cr_gray);
	m_btnReset.SetNotifyFunc(&CPanelSettingDisplay::OnNotify,this);

    /////////////////////////////////////////////////////////////////////////

	if(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault == 0)
	{
		m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault = 130;
	}

	//set value:
	if(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaCurrent != 0)
	{
		SetGamma((DOUBLE)(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaCurrent)/100.0);
		m_sliGamma.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaCurrent-50);
	}
	else
	{
		SetGamma((DOUBLE)(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault)/100.0);
		m_sliGamma.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault-50);
	}

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
    if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent==0)
    {
        m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessDefault;
    }

    FreshLedSilder();
	EnableLedAnimEffect(m_pSkinManager->GetConfig()->GetCurParam()->bLedAnimEffect);
#endif

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET
	int iPanelHeight = g_iClientHeight/2 - py + tag_h;

	if(iPanelHeight < g_iClientHeight)
		iPanelHeight = g_iClientHeight;

	SetSize(g_iClientWidth-box_size_x,iPanelHeight);

	return TRUE;
}


void CALLBACK CPanelSettingDisplay::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingDisplay *panel=(CPanelSettingDisplay*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingDisplay::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: idControl[%d] wPara[%d]\r\n"), TEXT(__FUNCTION__),idControl,wPara));
	switch (idControl)
	{
	case IDC_SETTING_SLI_BLUR_EFFECT:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->bBlurEffect = (wPara?1:0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_LAMP_CONTROL:
		{
			if(m_pCommander)
			{
				m_pCommander->SendCmd(_ARM_LAMP_CONTROL_KEY,wPara?1:0);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_LCD_GAMMA:
		{
			SetGamma(0.5+((DOUBLE)(wPara))/100.0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_LCD_BRIGHTNESS:
		{
            if(wPara >= CVTE_DEF_LCD_BRIGHTNESS_SILDER_MIN && wPara <= CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX)
			{
                SetLCDDayModeBrightness(wPara+CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_LCD_BRIGHTNESS_NIGHT:
		{
			if(wPara >= CVTE_DEF_LCD_BRIGHTNESS_SILDER_MIN && wPara <= CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX)
			{
				SetLCDNightModeBrightness(wPara+CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START);
			}
			PlaySoundEffect(0);
		}
		break;
#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
	case IDC_SETTING_SLI_LED_BRIGHTNESS:
		{
            //RETAILMSG(1, (TEXT("APP:MSG: sliLedBrightness[%d]\r\n"),wPara));
            if(wPara >= CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MIN && wPara <= CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX)
			{
                SetLedDayModeBrightness(wPara + CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START);
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_SETTING_SLI_LED_BRIGHTNESS_NIGHT:
		{
			//RETAILMSG(1, (TEXT("APP:MSG: sliLedBrightness[%d]\r\n"),wPara));
			if(wPara >= CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MIN && wPara <= CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX)
			{
				SetLedNightModeBrightness(wPara + CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START);
			}
			PlaySoundEffect(0);
		}
		break;
    case IDC_SETTING_BTN_LED_COLOR1:
        {
            m_btnLEDColor1.SetShowScale(1.2f);
            m_btnLEDColor2.SetShowScale(1.0f);
            m_btnLEDColor3.SetShowScale(1.0f);
            m_btnLEDColor4.SetShowScale(1.0f);
            m_btnLEDColor5.SetShowScale(1.0f);
            m_btnLEDColorUser.SetShowScale(1.0f);

			SetLedHue(0);
			FreshLedSilder();
            PlaySoundEffect(0);
        }
        break;
    case IDC_SETTING_BTN_LED_COLOR2:
        {
            m_btnLEDColor1.SetShowScale(1.0f);
            m_btnLEDColor2.SetShowScale(1.2f);
            m_btnLEDColor3.SetShowScale(1.0f);
            m_btnLEDColor4.SetShowScale(1.0f);
            m_btnLEDColor5.SetShowScale(1.0f);
            m_btnLEDColorUser.SetShowScale(1.0f);

			SetLedHue(51);
			FreshLedSilder();
            PlaySoundEffect(0);
        }
        break;
    case IDC_SETTING_BTN_LED_COLOR3:
        {
            m_btnLEDColor1.SetShowScale(1.0f);
            m_btnLEDColor2.SetShowScale(1.0f);
            m_btnLEDColor3.SetShowScale(1.2f);
            m_btnLEDColor4.SetShowScale(1.0f);
            m_btnLEDColor5.SetShowScale(1.0f);
            m_btnLEDColorUser.SetShowScale(1.0f);

			SetLedHue(113);
			FreshLedSilder();
            PlaySoundEffect(0);
        }
        break;
    case IDC_SETTING_BTN_LED_COLOR4:
        {
            m_btnLEDColor1.SetShowScale(1.0f);
            m_btnLEDColor2.SetShowScale(1.0f);
            m_btnLEDColor3.SetShowScale(1.0f);
            m_btnLEDColor4.SetShowScale(1.2f);
            m_btnLEDColor5.SetShowScale(1.0f);
            m_btnLEDColorUser.SetShowScale(1.0f);

			SetLedHue(226);
			FreshLedSilder();
            PlaySoundEffect(0);
        }
        break;
    case IDC_SETTING_BTN_LED_COLOR5:
        {
            m_btnLEDColor1.SetShowScale(1.0f);
            m_btnLEDColor2.SetShowScale(1.0f);
            m_btnLEDColor3.SetShowScale(1.0f);
            m_btnLEDColor4.SetShowScale(1.0f);
            m_btnLEDColor5.SetShowScale(1.2f);
            m_btnLEDColorUser.SetShowScale(1.0f);

			SetLedHue(300);
			FreshLedSilder();
            PlaySoundEffect(0);
        }
        break;
    case IDC_SETTING_BTN_LED_COLOR_USER:
        {
            m_btnLEDColor1.SetShowScale(1.0f);
            m_btnLEDColor2.SetShowScale(1.0f);
            m_btnLEDColor3.SetShowScale(1.0f);
            m_btnLEDColor4.SetShowScale(1.0f);
            m_btnLEDColor5.SetShowScale(1.0f);
            m_btnLEDColorUser.SetShowScale(1.2f);

            LEDHSB ledcolor;
			LEDRGB ledrgb;
            memset(&ledcolor, 0, sizeof(ledcolor));
		    ledcolor.Hue = 0;
		    ledcolor.Saturation = 0.0f;
		    ledcolor.Brightness = m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f;

		    m_pLED->SetLedHSB(&ledcolor);
    		m_pLED->RefreshLED();
		    m_pLED->GetLedRGB(&ledrgb);

		    m_pSkinManager->GetConfig()->GetCurParam()->iLEDRedCurrent = ledrgb.Red;
		    m_pSkinManager->GetConfig()->GetCurParam()->iLEDGreenCurrent = ledrgb.Green;
		    m_pSkinManager->GetConfig()->GetCurParam()->iLEDBlueCurrent = ledrgb.Blue;

            FreshLedSilder();

            PlaySoundEffect(0);
        }
        break;
    case IDC_SETTING_SLI_LED_COLOR:
        {
            SetLedHue((float)wPara);

            m_btnLEDColor1.SetShowScale(1.0f);
            m_btnLEDColor2.SetShowScale(1.0f);
            m_btnLEDColor3.SetShowScale(1.0f);
            m_btnLEDColor4.SetShowScale(1.0f);
            m_btnLEDColor5.SetShowScale(1.0f);
            m_btnLEDColorUser.SetShowScale(1.0f);

            PlaySoundEffect(0);
        }
        break;
#if 0
    case IDC_SETTING_BTN_LED_COLOR_CUSTOM:
        {
            RETAILMSG(1, (TEXT("APP:MSG: IDC_SETTING_BTN_LED_COLOR_CUSTOM\r\n")));
            if(!m_bColorPaletteShown)
            {
                m_btnLEDColor1.SetShowScale(1.0f);
                m_btnLEDColor2.SetShowScale(1.0f);
                m_btnLEDColor3.SetShowScale(1.0f);
                m_btnLEDColor4.SetShowScale(1.0f);
                m_btnLEDColor5.SetShowScale(1.0f);
                m_btnLEDColorUser.SetShowScale(1.0f);
                m_btnLEDColorCustom.SetShowScale(1.2f);

                m_bColorPaletteShown = TRUE;
                m_tagPanelLight.SetSize((g_iClientWidth-LIST_BOX_ITEM_WIDTH-12*2),LIST_BOX_TAG_HEIGHT*4);
                m_tagPanelLight.SetPosIni(0,g_iClientHeight/2-50-LIST_BOX_TAG_HEIGHT-LIST_BOX_TAG_HEIGHT-LIST_BOX_TAG_HEIGHT/2);
                m_btnReset.SetPosIni(0,g_iClientHeight/2-50-LIST_BOX_TAG_HEIGHT*5);
                //m_labLEDCurrentColor.Show(TRUE);

                m_btnLEDColor1.Show(TRUE);
                m_btnLEDColor2.Show(TRUE);
                m_btnLEDColor3.Show(TRUE);
                m_btnLEDColor4.Show(TRUE);
                m_btnLEDColor5.Show(TRUE);
                m_btnLEDColorUser.Show(TRUE);

                m_tagLEDColor.Show(TRUE);
                m_sliLEDColor.Show(TRUE);
                m_sliLEDBrightness.Show(TRUE);
                m_tagLEDBrightness.Show(TRUE);
            }
            else
            {
                m_btnLEDColor1.SetShowScale(1.0f);
                m_btnLEDColor2.SetShowScale(1.0f);
                m_btnLEDColor3.SetShowScale(1.0f);
                m_btnLEDColor4.SetShowScale(1.0f);
                m_btnLEDColor5.SetShowScale(1.0f);
                m_btnLEDColorUser.SetShowScale(1.0f);
                m_btnLEDColorCustom.SetShowScale(1.0f);

                m_btnLEDColor1.Show(FALSE);
                m_btnLEDColor2.Show(FALSE);
                m_btnLEDColor3.Show(FALSE);
                m_btnLEDColor4.Show(FALSE);
                m_btnLEDColor5.Show(FALSE);
                m_btnLEDColorUser.Show(FALSE);

                m_bColorPaletteShown = FALSE;
                m_tagPanelLight.SetSize((g_iClientWidth-LIST_BOX_ITEM_WIDTH-12*2),LIST_BOX_TAG_HEIGHT);
                m_tagPanelLight.SetPosIni(0,g_iClientHeight/2-50-LIST_BOX_TAG_HEIGHT);
                m_btnReset.SetPosIni(0,g_iClientHeight/2-50-LIST_BOX_TAG_HEIGHT*2);
                //m_labLEDCurrentColor.Show(FALSE);
                m_tagLEDColor.Show(FALSE);
                m_sliLEDColor.Show(FALSE);
                m_sliLEDBrightness.Show(FALSE);
                m_tagLEDBrightness.Show(FALSE);
            }
            PlaySoundEffect(0);
        }
        break;
#endif
	case IDC_SETTING_SLI_LED_ANIMATION_EFFECT:
		{
			EnableLedAnimEffect(wPara?TRUE:FALSE);
			PlaySoundEffect(0);
		}
		break;
#endif
	case IDC_SETTING_SLI_DISPLAY_RESET:
		{
			m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault = CVTE_REG_GAMMA_DEFAULT;
			SetGamma((DOUBLE)(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault)/100.0);
			m_sliGamma.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaDefault-50);

			SetLCDDayModeBrightness(CVTE_REG_LCD_DAYMODE_BRIGHTNESS_DEFAULT);
			SetLCDNightModeBrightness(CVTE_REG_LCD_NIGHTMODE_BRIGHTNESS_DEFAULT);
			FreshLCDBrightnessSilder();
#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
            //LEDRGB ledcolor;
            //memset(&ledcolor, 0, sizeof(ledcolor));
            //ledcolor.Red = 255;
            //ledcolor.Green = 255;
            //ledcolor.Blue = 255;
            //////RETAILMSG(1, (TEXT("APP:MSG: sliLedColor RGB[%d,%d,%d]\r\n"),ledcolor.Red,ledcolor.Green,ledcolor.Blue));

			m_pSkinManager->GetConfig()->GetCurParam()->bLedAnimEffect = CVTE_REG_LED_ANIM_EFFECT_DEFAULT;
			EnableLedAnimEffect(CVTE_REG_LED_ANIM_EFFECT_DEFAULT);
			m_sliLedAnimationEffect.SetValue(CVTE_REG_LED_ANIM_EFFECT_DEFAULT?10:0);

			SetLedDayModeBrightness(CVTE_REG_LED_DAYMODE_BRIGHTNESS_DEFAULT);
			SetLedNightModeBrightness(CVTE_REG_LED_NIGHTMODE_BRIGHTNESS_DEFAULT);

			//LEDHSB ledcolor;
			LEDRGB ledrgb;
			memset(&ledrgb, 0, sizeof(ledrgb));
			ledrgb.Red = CVTE_REG_LED_R_DEFAULT;
			ledrgb.Green = CVTE_REG_LED_G_DEFAULT;
			ledrgb.Blue = CVTE_REG_LED_B_DEFAULT;

			//memset(&ledcolor, 0, sizeof(ledcolor));
			//ledcolor.Hue = 0;
			//ledcolor.Saturation = 0.0f;
			//ledcolor.Brightness = m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f;

			m_pLED->SetLedRGB(&ledrgb);
			m_pLED->SetLedBrightness((float)m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f);
			m_pLED->RefreshLED();
			m_pLED->GetLedRGB(&ledrgb);

			m_pSkinManager->GetConfig()->GetCurParam()->iLEDRedCurrent = ledrgb.Red;
			m_pSkinManager->GetConfig()->GetCurParam()->iLEDGreenCurrent = ledrgb.Green;
			m_pSkinManager->GetConfig()->GetCurParam()->iLEDBlueCurrent = ledrgb.Blue;

            FreshLedSilder();

            m_btnLEDColor1.SetShowScale(1.0f);
            m_btnLEDColor2.SetShowScale(1.0f);
            m_btnLEDColor3.SetShowScale(1.0f);
            m_btnLEDColor4.SetShowScale(1.0f);
            m_btnLEDColor5.SetShowScale(1.0f);
            m_btnLEDColorUser.SetShowScale(1.0f);
#endif
			PlaySoundEffect(0);
		}
		break;
	default:
		break;
	}
}

void CPanelSettingDisplay::OnMCUCmd(WPARAM wParam, LPARAM lParam)
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
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		//lamp:
		m_sliLampControl.SetValue(pCommander->GetItemData(MCU_LIGHT_CONTROL)?10:0);
#endif
	}
	else if(dwType == 1)//single command
	{
		switch (dwID)
		{
		}
	}
}

void CPanelSettingDisplay::SetGamma(DOUBLE iSet)
{
	m_pSkinManager->GetConfig()->GetCurParam()->iLCDGammaCurrent = UINT( iSet*100);

	SetLCDGamma(iSet);
}

#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
void CPanelSettingDisplay::SetLedRGB(LEDRGB * pLedColor)
{
    m_pSkinManager->GetConfig()->GetCurParam()->iLEDRedCurrent = pLedColor->Red;
    m_pSkinManager->GetConfig()->GetCurParam()->iLEDGreenCurrent = pLedColor->Green;
    m_pSkinManager->GetConfig()->GetCurParam()->iLEDBlueCurrent = pLedColor->Blue;

    if(m_pLED == INVALID_HANDLE_VALUE || m_pLED == NULL)
    {
        RETAILMSG(1, (TEXT("ERR:[%s]: m_pLED is invalid.\r\n"), TEXT(__FUNCTION__)));
        return;
    }
    m_pLED->SetLedRGB(pLedColor);
    m_pLED->RefreshLED();
}

void CPanelSettingDisplay::SetLedHue(float fHue)
{
    LEDRGB ledrgb;
    LEDHSB ledhsb;
    memset(&ledrgb, 0, sizeof(ledrgb));
    memset(&ledhsb, 0, sizeof(ledhsb));

    if(m_pLED == INVALID_HANDLE_VALUE || m_pLED == NULL)
    {
        RETAILMSG(1, (TEXT("ERR:[%s]: m_pLED is invalid.\r\n"), TEXT(__FUNCTION__)));
        return;
    }
    ledhsb.Hue = fHue;
    ledhsb.Saturation = 1.0f;
    ledhsb.Brightness = m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f;

    m_pLED->SetLedHSB(&ledhsb);
    m_pLED->RefreshLED();
    m_pLED->GetLedRGB(&ledrgb);

    m_pSkinManager->GetConfig()->GetCurParam()->iLEDRedCurrent = ledrgb.Red;
    m_pSkinManager->GetConfig()->GetCurParam()->iLEDGreenCurrent = ledrgb.Green;
    m_pSkinManager->GetConfig()->GetCurParam()->iLEDBlueCurrent = ledrgb.Blue;
}

void CPanelSettingDisplay::SetLedDayModeBrightness(UINT nSet)
{
	UINT nBk;
	if(nSet>CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END)
		nBk = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END;
	else if(nSet<CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START)
		nBk = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START;
	else
		nBk = nSet;

	m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness = nBk;
	//RETAILMSG(1, (TEXT("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness));

    if(m_pLED == INVALID_HANDLE_VALUE || m_pLED == NULL)
    {
        RETAILMSG(1, (TEXT("ERR:[%s]: m_pLED is invalid.\r\n"), TEXT(__FUNCTION__)));
        return;
    }
	if(!m_bNightMode)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]: SetLedBrightness[%d]\r\n"), TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness));
		m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness;
		m_pLED->SetLedBrightness((float)m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f);
		m_pLED->RefreshLED();
	}
}

void CPanelSettingDisplay::SetLedNightModeBrightness(UINT nSet)
{
	UINT nBk;
	if(nSet>CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END)
		nBk = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END;
	else if(nSet<CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START)
		nBk = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START;
	else
		nBk = nSet;

	m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness = nSet;

	//RETAILMSG(1, (TEXT("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness));

	if(m_pLED == INVALID_HANDLE_VALUE || m_pLED == NULL)
	{
		RETAILMSG(1, (TEXT("ERR:[%s]: m_pLED is invalid.\r\n"), TEXT(__FUNCTION__)));
		return;
	}

	if(m_bNightMode)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]: SetLedBrightness[%d]\r\n"), TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness));
		m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent = m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness;
		m_pLED->SetLedBrightness((float)m_pSkinManager->GetConfig()->GetCurParam()->iLEDBrightnessCurrent/100.0f);
		m_pLED->RefreshLED();
	}
}

void CPanelSettingDisplay::FreshLedSilder()
{
    if(m_pLED)
    {
        LEDHSB ledhsb;
		LEDRGB ledrgb;
		UINT nLEDDaymodeBrightness;
		UINT nLEDNightmodeBrightness;

        memset(&ledhsb, 0, sizeof(ledhsb));
        memset(&ledrgb, 0, sizeof(ledrgb));

        m_pLED->GetLedHSB(&ledhsb);
        m_pLED->GetLedRGB(&ledrgb);

        m_sliLEDColor.SetValue((INT)ledhsb.Hue);

		//printf("MSG:FreshLedSilder: HSB[%f,%f,%f] nLEDCurrentBrightness:%d \r\n",ledhsb.Hue, ledhsb.Saturation, ledhsb.Brightness,nLEDCurrentBrightness);

#if 1
		//printf("MSG:[FreshLedSilder]: iLEDDayModeBrightness[%d] iLEDNightModeBrightness[%d]\r\n",
		//	m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness,
		//	m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness);

		if(m_bNightMode)
		{
			//DayMode led brightness
			if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness <= CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START;
				m_sliLEDBrightness.SetValue(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MIN);
			}
			else if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness >= CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END;
				m_sliLEDBrightness.SetValue(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX);
			}
			else
			{
				m_sliLEDBrightness.SetValue((INT)(m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness-CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START));	
			}
			
			//NightMode led brightness
			if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness <= CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START;
				m_sliLEDBrightnessnight.SetValue(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MIN);
			}
			else if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness >= CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END;
				m_sliLEDBrightnessnight.SetValue(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX);
			}
			else
			{
				m_sliLEDBrightnessnight.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness-CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START);
			}
			
		}
		else
		{
			//DayMode led brightness
			if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness <= CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START;
				m_sliLEDBrightness.SetValue(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MIN);
			}
			else if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness >= CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness = CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END;
				m_sliLEDBrightness.SetValue(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX);
			}
			else
			{
				m_sliLEDBrightness.SetValue((INT)(m_pSkinManager->GetConfig()->GetCurParam()->iLEDDayModeBrightness-CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START));	
			}

			//NightMode led brightness
			if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness <= CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START;
				m_sliLEDBrightnessnight.SetValue(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MIN);
			}
			else if(m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness >= CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END)
			{
				m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness = CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END;
				m_sliLEDBrightnessnight.SetValue(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX);
			}
			else
			{
				m_sliLEDBrightnessnight.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLEDNightModeBrightness-CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START);
			}
		}
#endif
		m_pSkinManager->GetConfig()->GetCurParam()->iLEDRedCurrent = ledrgb.Red;
		m_pSkinManager->GetConfig()->GetCurParam()->iLEDGreenCurrent = ledrgb.Green;
		m_pSkinManager->GetConfig()->GetCurParam()->iLEDBlueCurrent = ledrgb.Blue;
    }
}

void CPanelSettingDisplay::EnableLedAnimEffect(BOOL bEnable)
{
	if(m_pLED)
	{
		m_pSkinManager->GetConfig()->GetCurParam()->bLedAnimEffect = (bEnable);
		m_pLED->EnableAnimShade(bEnable,TRUE);
		if(bEnable)
		{
			m_btnLEDColor1.Enable(FALSE);
			m_btnLEDColor2.Enable(FALSE);
			m_btnLEDColor3.Enable(FALSE);
			m_btnLEDColor4.Enable(FALSE);
			m_btnLEDColor5.Enable(FALSE);
			m_btnLEDColorUser.Enable(FALSE);
			//m_sliLEDColor.Enable(FALSE);
		}
		else
		{
			m_btnLEDColor1.Enable(TRUE);
			m_btnLEDColor2.Enable(TRUE);
			m_btnLEDColor3.Enable(TRUE);
			m_btnLEDColor4.Enable(TRUE);
			m_btnLEDColor5.Enable(TRUE);
			m_btnLEDColorUser.Enable(TRUE);
			//m_sliLEDColor.Enable(TRUE);
			FreshLedSilder();
		}
	}
}
#endif

void CPanelSettingDisplay::FreshLCDBrightnessSilder()
{
	RETAILMSG(0,(TEXT("MSG:[%s]: iLCDDayModeBrightness[%d] iLCDNightModeBrightness[%d]\r\n"),
		TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness,m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness));
	//if(m_bNightMode)
	//{
		//Day mode 
		if(m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness >= CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END)
			m_sliLCDBrightness.SetValue(CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX);
		else
			m_sliLCDBrightness.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness-CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START);

		//Night mode
		if(m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness >= CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END)
			m_sliLCDBrightnessnight.SetValue(CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX);
		else
			m_sliLCDBrightnessnight.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness-CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START);
	//}
	//else
	//{
	//	//Day mode 
	//	if(m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness >= CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END)
	//		m_sliLCDBrightness.SetValue(CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX);
	//	else
	//		m_sliLCDBrightness.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness-CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START);

	//	//Night mode
	//	if(m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness >= CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END)
	//		m_sliLCDBrightnessnight.SetValue(CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX);
	//	else
	//		m_sliLCDBrightnessnight.SetValue(m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness-CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START);
	//}

}

void CPanelSettingDisplay::SetLCDDayModeBrightness(UINT nBrightness)
{
	UINT nBk;
	if(nBrightness>=CVTE_DEF_LCD_BRIGHTNESS_RANGE_END)
		nBk = CVTE_DEF_LCD_BRIGHTNESS_MAX;
	else if(nBrightness<CVGE_DEF_LCD_BRIGHTNESS_RANGE_START)
		nBk = CVGE_DEF_LCD_BRIGHTNESS_RANGE_START;
	else
		nBk = nBrightness;

	m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness = nBk;

	//RETAILMSG(1, (TEXT("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iLCDDayModeBrightness));

	if(!m_bNightMode)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]: BKL_SetBackLight[%d]\r\n"), TEXT(__FUNCTION__),nBk));
		m_pSkinManager->GetConfig()->GetCurParam()->dwBacklight = nBk;
		BKL_SetBackLight(nBk,TRUE);
	}
}

void CPanelSettingDisplay::SetLCDNightModeBrightness(UINT nBrightness)
{
	UINT nBk;
	if(nBrightness>=CVTE_DEF_LCD_BRIGHTNESS_RANGE_END)
		nBk = CVTE_DEF_LCD_BRIGHTNESS_MAX;
	else if(nBrightness<CVGE_DEF_LCD_BRIGHTNESS_RANGE_START)
		nBk = CVGE_DEF_LCD_BRIGHTNESS_RANGE_START;
	else
		nBk = nBrightness;

	m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness = nBk;

	//RETAILMSG(1, (TEXT("MSG:[%s]: %d\r\n"), TEXT(__FUNCTION__),m_pSkinManager->GetConfig()->GetCurParam()->iLCDNightModeBrightness));

	if(m_bNightMode)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]: BKL_SetBackLight[%d]\r\n"), TEXT(__FUNCTION__),nBk));
		m_pSkinManager->GetConfig()->GetCurParam()->dwBacklight = nBk;
		BKL_SetBackLight(nBk,TRUE);
	}
}


void CPanelSettingDisplay::OnHeadlightOn(BOOL bOn, UINT nBk)
{
	m_bNightMode = bOn;

#if 0
	if(bOn)
	{

		m_sliLCDBrightness.Enable(FALSE);
		m_sliLCDBrightnessnight.Enable(TRUE);
#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
		m_sliLEDBrightness.Enable(FALSE);
		m_sliLEDBrightnessnight.Enable(TRUE);
#endif
	}
	else
	{
		m_sliLCDBrightness.Enable(TRUE);
		m_sliLCDBrightnessnight.Enable(FALSE);
#if (CVTE_EN_LED && (CUSTOMER_ID != CUSTOMER_DAIICHI_FIAT_DOBLO) && (CUSTOMER_ID != CUSTOMER_DS701P_DAIICHI_FIAT_DOBLO))
		m_sliLEDBrightness.Enable(TRUE);
		m_sliLEDBrightnessnight.Enable(FALSE);
#endif

	}
#endif
}
