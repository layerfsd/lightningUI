#include "PanelBluetoothTalking.h"
#include "../../resource.h"

CPanelBluetoothTalking::CPanelBluetoothTalking(void)
{
	m_bOnDTMF = FALSE;

	m_pSmartBarPhone = NULL;

	m_dwCallCount=0;
	m_dwCurCallIndex=-1;
	m_bEnableAutoSelCall = TRUE;

	m_dwCallTime1=0;
	m_dwCallTime2=0;

	m_iCallState1=DIAL_STATE_HANGUP;
	m_iCallState2=DIAL_STATE_HANGUP;

	m_bTalking1 = FALSE;
	m_bTalking2 = FALSE;

	m_dwEnterTick = 0;
	m_bNeedAutoShowSmartBar = TRUE;
	m_bAudioInHandfree = TRUE;
	m_bCarPlayConnected = FALSE;
}
CPanelBluetoothTalking::~CPanelBluetoothTalking(void)
{

}

BOOL CPanelBluetoothTalking::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelBluetoothTalking::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	RETAILMSG(DEBUG_BT,(_T("@@@@@@@ CPanelBluetoothTalking::OnEnterPanel iParam:%d\r\n"),iParam));
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

// 	MakeDefaultAni(ANIM_ROTATE_TOP_CLOCK_IN);

// 	//Switch audio SRC:
// 	SwitchAudioSRC(SRC_AUDIO_ARM);


	//test:
// 	if(m_MobileData.Query(_T("+8613570204516")))
// 	{
// 		m_strMobileArea2=m_MobileData.GetArea();
// 		m_strMobileType2=m_MobileData.GetType();
// 	}
// 	if(m_MobileData.Query(_T("15914207900")))
// 	{
// 		m_strMobileArea1=m_MobileData.GetArea();
// 		m_strMobileType1=m_MobileData.GetType();
// 	}

	//
	//m_bEnableQuit = FALSE;
	m_dwEnterTick = GetTickCount();

	m_bNeedAutoShowSmartBar = TRUE;
	//
	if(m_pSmartBarPhone && m_pSmartBarPhone->IsVisible())
	{
		m_pSmartBarPhone->Show(FALSE);
	}

	//
	if(m_dwCallCount == 0)
	{
		SwitchPage(UI_CLASS_HOME,PAGE_HOME_WRAPPER,PAGE_SWITCH_ANI_NONE);
	}
}

void CPanelBluetoothTalking::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	//default
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);


	SetDTMFMode(FALSE);

	//
	if(m_bNeedAutoShowSmartBar && m_dwCallCount>0 && !m_bCarPlayConnected)
	{
		if(m_pSmartBarPhone && !m_pSmartBarPhone->IsVisible())
		{
			m_pSmartBarPhone->Show(TRUE,INFINITE);
		}
	}
}

void CPanelBluetoothTalking::Render()
{
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};
#if( CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET )
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_065):NULL;
#else
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
	static CTexture *pTexBar2=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_056):NULL;
#endif

	if(!BeginRender())return;


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

#if( CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET )
	DrawRect(pTexBar,0,(-g_iClientHeight+400)/2,g_iClientWidth-4,400,2,&cr,TRUE);
#else
	DrawRect(pTexBar,0,(-g_iClientHeight/2+62),g_iClientWidth-4,134-4,2,&cr,TRUE);
	DrawRect(pTexBar2,0,(g_iClientHeight/2-40),g_iClientWidth-4,80-4,2,&cr,TRUE);
#endif 
	m_btnCall1.SetAlpha(m_fCurAlpha);
	m_btnCall1.Render();
	m_btnCall2.SetAlpha(m_fCurAlpha);
	m_btnCall2.Render();

	m_btnKeyboard.SetAlpha(m_fCurAlpha);
	m_btnKeyboard.Render();
	m_btnACH.SetAlpha(m_fCurAlpha);
	m_btnACH.Render();
	m_btnMic.SetAlpha(m_fCurAlpha);
	m_btnMic.Render();

	CButtonGL *pbtnAnswer = ((m_dwCurCallIndex==0) ? &m_btnAnswer1 : &m_btnAnswer2);
	CButtonGL *pbtnHangup = ((m_dwCurCallIndex==0) ? &m_btnHang1 : &m_btnHang2);

	pbtnAnswer->SetAlpha(m_fCurAlpha);
	pbtnAnswer->Render();
	pbtnHangup->SetAlpha(m_fCurAlpha);
	pbtnHangup->Render();


	if(m_bOnDTMF)
	{
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


		m_tagDialString.SetAlpha(m_fCurAlpha);
		m_tagDialString.Render();

	}
	else
	{
		SIZE si;
		COLORGL cr={1,1,1,1};
		COLORGL cr_b={0.8,0.8,0.8,1};
		GLfloat yoffset = 20.0;

		CM_String *pstrCallStatus = ((m_dwCurCallIndex==0) ? &m_strCallStatus1 : &m_strCallStatus2);

		const wchar_t *pstrName = NULL;//= ((m_dwCurCallIndex==0) ? &m_strName1 : &m_strName2);
		const wchar_t *pstrNumber = NULL;//= ((m_dwCurCallIndex==0) ? &m_strNumber1 : &m_strNumber2);

		if(m_dwCallCount==0 && GetTickCount()-m_dwEnterTick>1000)
		{
			pstrName = GetResourceString(IDS_BT_STREAMING_AUDIO);
			pstrNumber = GetResourceString(IDS_BT_STREAMING_AUDIO_TERMINATE);
		}
		else
		{
			pstrName = ((m_dwCurCallIndex==0) ? m_strName1.String() : m_strName2.String());
			pstrNumber = ((m_dwCurCallIndex==0) ? m_strNumber1.String() : m_strNumber2.String());
		}

		BOOL *pbTalking = ((m_dwCurCallIndex==0) ? &m_bTalking1 : &m_bTalking2);
		DWORD *pdwCallTime = ((m_dwCurCallIndex==0) ? &m_dwCallTime1 : &m_dwCallTime2);
//	    modified by wuhaoheng
#if SHOW_PHONCALL_AREA_TYPE
		CM_String *pstrArea = ((m_dwCurCallIndex==0) ? &m_strMobileArea1 : &m_strMobileArea2);
		CM_String *pstrType = ((m_dwCurCallIndex==0) ? &m_strMobileType1 : &m_strMobileType2);
#endif
		if(m_pTextGL)
		{
			m_pTextGL->SetAlpha(m_fCurAlpha);
			m_pTextGL->SetLuminance(m_fCurLuminance);

			//status:
			if(pstrCallStatus->String())
			{
				m_pTextGL->SetColor(&cr,FALSE);
				m_pTextGL->SetString(pstrCallStatus->String());
				m_pTextGL->SetCharScale(1.0f,1.0f);
				m_pTextGL->GetExtension(&si);
				#if( CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET )
				m_pTextGL->SetPos(-GLfloat(si.cx/2),(g_iClientHeight/2-18));
				#else
				m_pTextGL->SetPos(-GLfloat(si.cx/2),(g_iClientHeight/2-22));
				#endif
				m_pTextGL->Render();
			}

			//area and type:
//	        Do not show area and type.
//	        modified by wuhaoheng
#if SHOW_PHONCALL_AREA_TYPE
			if(pstrArea->String())
			{
				m_pTextGL->SetColor(&cr_b,FALSE);
				m_pTextGL->SetString(pstrArea->String());
				m_pTextGL->SetCharScale(0.60f,0.60f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-GLfloat(si.cx)/2/*-40*/,165);
				m_pTextGL->Render();
				yoffset = 0.0;
			}
#endif
			/*
			if(pstrType->String())
			{
				m_pTextGL->SetColor(&cr_b,FALSE);
				m_pTextGL->SetString(pstrType->String());
				m_pTextGL->SetCharScale(0.85f,0.85f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(40,(g_iClientHeight/2-80));
				m_pTextGL->Render();
			}
			*/

			//Name:
			if(pstrName)
			{
				m_pTextGL->EnableAutoEllipsis(TRUE,g_iScreenWidth-60);
				m_pTextGL->SetColor(&cr,FALSE);
				m_pTextGL->SetString(pstrName);
				m_pTextGL->SetCharScale(1.0f,1.0f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-GLfloat(si.cx/2),110+yoffset);
				m_pTextGL->Render();
				m_pTextGL->EnableAutoEllipsis(FALSE);
			}

			//number:
			if(pstrNumber)
			{
				m_pTextGL->SetColor(&cr,FALSE);
				m_pTextGL->SetString(pstrNumber);
				m_pTextGL->SetCharScale(1.0f,1.0f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-GLfloat(si.cx/2),45+yoffset);
				m_pTextGL->Render();
			}
			//call time
			if(*pbTalking && pbtnHangup->IsVisible() && *pdwCallTime!=0)
			{
				int isec=(GetTickCount()-*pdwCallTime)/1000;
				wchar_t str[16];
				swprintf_s(str,16,_T("%02d:%02d"),isec/60,isec%60);

				m_pTextGL->SetColor(&cr,FALSE);
				//m_pTextGL->SetStyle(STYLE_ART);
				m_pTextGL->SetString(str);
				m_pTextGL->SetCharScale(1.5f,1.5f);
				m_pTextGL->GetExtension(&si,STYLE_ART);
				m_pTextGL->SetPos(-GLfloat(si.cx/2),-20+yoffset);
				m_pTextGL->Render();
				m_pTextGL->SetStyle(STYLE_NORMAL);//reset
			}
		}
	}

	EndRender();
}
void CPanelBluetoothTalking::SetAudioDir(UINT idDir)
{
	if(idDir == 1)
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_btnACH.SetCaption(GetResourceString(IDS_HANDS_FREE));
#endif
		m_btnACH.SetIcon(&m_iconACHBT);
		m_btnMic.Enable(TRUE);
		m_bAudioInHandfree = TRUE;
	}
	else
	{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		m_btnACH.SetCaption(GetResourceString(IDS_PRIVACY));
#endif
		m_btnACH.SetIcon(&m_iconACHPhone);
		m_btnMic.Enable(FALSE);
		m_bAudioInHandfree = FALSE;
	}

}
void CPanelBluetoothTalking::SetMicMute(BOOL bMute)
{
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_MuteString = GetResourceString(IDS_SETTING_MUTE);
	m_MuteString += _T(":");
	m_MuteString += bMute?GetResourceString(IDS_ON):GetResourceString(IDS_OFF);
	m_btnMic.SetCaption(m_MuteString.String());
	m_btnMic.SetIcon(bMute?&m_iconMicOff:&m_iconMic);
#else
	m_btnMic.SetStatus(bMute?BS_FOCUS:BS_UP);
#endif
}
BOOL CPanelBluetoothTalking::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);


#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	GLfloat pix_ratio=GLfloat(CVTE_DEF_SCREEN_PHYSICALWIDTH)/GLfloat(CVTE_DEF_SCREEN_PHYSICALHEIGHT)*GLfloat(g_iScreenHeight)/GLfloat(g_iScreenWidth);

	m_iconKeyboard.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1110));
	m_iconKeyboard.SetScale(1.0f,1.0f*pix_ratio);
	
	m_iconHang.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1109));
	m_iconHang.SetScale(1.0f,1.0f*pix_ratio);
	
	m_iconAnswer.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1108));
	m_iconAnswer.SetScale(1.0f,1.0f*pix_ratio);

	m_iconACHPhone.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1107));
	m_iconACHPhone.SetScale(1.0f,1.0f*pix_ratio);
	
	m_iconACHBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1106));
	m_iconACHBT.SetScale(1.0f,1.0f*pix_ratio);
	
	m_iconMic.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1111));
	m_iconMic.SetScale(1.0f,1.0f*pix_ratio);

	m_iconMicOff.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_1112));
	m_iconMicOff.SetScale(1.0f,1.0f*pix_ratio);

	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};

	int margin_x=10;
	int cx=(g_iClientWidth-margin_x*2)/3;
	int cy=120;
	int yoffset=70;
	int captionoffset=65;
	int iconoffset=20;
	int fontsize=16;

	SIZE sibtn1={cx*1-4,cy-4};
	SIZE sibtn2={cx*1-4,cy-4};
	SIZE sibtn3={128,128};
	wchar_t caption[32];

	//
	m_btnCall1.Initialize(IDC_BT_BTN_SWITCH_CALL1,this,pSkinManager->GetTexture(TEXID_1115),NULL,
		pSkinManager->GetTexture(TEXID_1116),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCall1.SetSize(&sibtn3);
	m_btnCall1.SetScale(0.8f,0.8f*pix_ratio);
	m_btnCall1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnCall1.SetPos(-g_iClientWidth/2+10+100,g_iScreenHeight/2-sibtn3.cy/2-85);
	swprintf_s(caption,32,_T("%s 1"),GetResourceString(IDS_CALL));
	m_btnCall1.SetCaption(caption);

	m_btnCall1.LinkTextGL(pTextGL);
	m_btnCall1.SetCaptionPos(BTN_CAPTION_CENTER);

	m_btnCall2.Initialize(IDC_BT_BTN_SWITCH_CALL2,this,pSkinManager->GetTexture(TEXID_1115),NULL,
		pSkinManager->GetTexture(TEXID_1116),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCall2.SetSize(&sibtn3);
	m_btnCall2.SetScale(0.8f,0.8f*pix_ratio);
	m_btnCall2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnCall2.SetPos(g_iClientWidth/2-10-100,g_iScreenHeight/2-sibtn3.cy/2-85);

	swprintf_s(caption,32,_T("%s 2"),GetResourceString(IDS_CALL));
	m_btnCall2.SetCaption(caption);

	m_btnCall2.LinkTextGL(pTextGL);
	m_btnCall2.SetCaptionPos(BTN_CAPTION_CENTER);

	m_btnKeyboard.Initialize(IDC_BT_BTN_KEYBOARD,this,
		m_pSkinManager->GetTexture(TEXID_1113),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnKeyboard.LinkTextGL(pTextGL);
	m_btnKeyboard.SetSize(&sibtn3);
	m_btnKeyboard.SetScale(0.8f,0.8f*pix_ratio);
	m_btnKeyboard.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnKeyboard.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+yoffset);
	m_btnKeyboard.SetIcon(&m_iconKeyboard);
	m_btnKeyboard.SetIconPos(0,iconoffset);
	m_btnKeyboard.SetCaptionHeight(fontsize);
	m_btnKeyboard.SetCaptionPos(BTN_CAPTION_BOTTOM,0,captionoffset);
	m_btnKeyboard.SetCaptionColor(&cr_gray);
	m_btnKeyboard.SetCaption(GetResourceString(IDS_KEYPAD));

	m_btnACH.Initialize(IDC_BT_BTN_ACH,this,
		m_pSkinManager->GetTexture(TEXID_1113),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnACH.LinkTextGL(pTextGL);
	m_btnACH.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnACH.SetSize(&sibtn3);
	m_btnACH.SetScale(0.8f,0.8f*pix_ratio);
	m_btnACH.SetPos((-g_iScreenWidth/2+margin_x+cx/2) + (g_iScreenWidth-2*margin_x-cx)/3,-g_iClientHeight/2+yoffset);
	m_btnACH.SetIcon(&m_iconACHBT);
	m_btnACH.SetIconPos(0,iconoffset);
	m_btnACH.SetCaptionHeight(fontsize);
	m_btnACH.SetCaptionPos(BTN_CAPTION_BOTTOM,0,captionoffset);
	m_btnACH.SetCaptionColor(&cr_gray);
	m_btnACH.SetCaption(GetResourceString(IDS_HANDS_FREE));

	m_btnMic.Initialize(IDC_BT_BTN_MIC_MUTE,this,
		m_pSkinManager->GetTexture(TEXID_1113),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnMic.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnMic.SetSize(&sibtn3);
	m_btnMic.SetScale(0.8f,0.8f*pix_ratio);
	m_btnMic.SetPos((g_iScreenWidth/2-margin_x-cx/2)-(g_iScreenWidth-2*margin_x-cx)/3,-g_iClientHeight/2+yoffset);
	m_btnMic.SetIcon(&m_iconMic);
	m_btnMic.SetIconPos(0,iconoffset);
	m_btnMic.LinkTextGL(pTextGL);
	m_btnMic.SetCaptionHeight(fontsize);
	m_btnMic.SetCaptionPos(BTN_CAPTION_BOTTOM,0,captionoffset);
	m_btnMic.SetCaptionColor(&cr_gray);
	m_MuteString = GetResourceString(IDS_SETTING_MUTE);
	m_MuteString += _T(":");
	m_MuteString += GetResourceString(IDS_OFF);
	m_btnMic.SetCaption(m_MuteString.String());

	m_btnHang1.Initialize(IDC_BT_BTN_HANG1,this,
		m_pSkinManager->GetTexture(TEXID_1117),NULL,
		m_pSkinManager->GetTexture(TEXID_1118),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHang1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnHang1.SetSize(&sibtn3);
	m_btnHang1.SetScale(0.8f,0.8f*pix_ratio);
	m_btnHang1.SetPos(g_iScreenWidth/2-margin_x-cx/2,-g_iClientHeight/2+yoffset);
	m_btnHang1.SetIcon(&m_iconHang);
	m_btnHang1.SetIconPos(0,iconoffset);
	m_btnHang1.LinkTextGL(pTextGL);
	m_btnHang1.SetCaptionHeight(fontsize);
	m_btnHang1.SetCaptionPos(BTN_CAPTION_BOTTOM,0,captionoffset);
	m_btnHang1.SetCaptionColor(&cr_gray);
	m_btnHang1.SetCaption(GetResourceString(IDS_HANGUP));

	m_btnAnswer1.Initialize(IDC_BT_BTN_ANSWER1,this,
		m_pSkinManager->GetTexture(TEXID_1115),NULL,
		m_pSkinManager->GetTexture(TEXID_1116),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAnswer1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnAnswer1.SetSize(&sibtn3);
	m_btnAnswer1.SetScale(0.8f,0.8f*pix_ratio);
	m_btnAnswer1.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+yoffset);
	m_btnAnswer1.SetIcon(&m_iconAnswer);
	m_btnAnswer1.SetIconPos(0,iconoffset);
	m_btnAnswer1.LinkTextGL(pTextGL);
	m_btnAnswer1.SetCaptionHeight(fontsize);
	m_btnAnswer1.SetCaptionPos(BTN_CAPTION_BOTTOM,0,captionoffset);
	m_btnAnswer1.SetCaptionColor(&cr_gray);
	m_btnAnswer1.SetCaption(GetResourceString(IDS_ANSWER));

	m_btnHang2.Initialize(IDC_BT_BTN_HANG2,this,
		m_pSkinManager->GetTexture(TEXID_1117),NULL,
		m_pSkinManager->GetTexture(TEXID_1118),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHang2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnHang2.SetSize(&sibtn3);
	m_btnHang2.SetScale(0.8f,0.8f*pix_ratio);
	m_btnHang2.SetPos(g_iScreenWidth/2-margin_x-cx/2,-g_iClientHeight/2+yoffset);
	m_btnHang2.SetIcon(&m_iconHang);
	m_btnHang2.SetIconPos(0,iconoffset);
	m_btnHang2.LinkTextGL(pTextGL);
	m_btnHang2.SetCaptionHeight(fontsize);
	m_btnHang2.SetCaptionPos(BTN_CAPTION_BOTTOM,0,captionoffset);
	m_btnHang2.SetCaptionColor(&cr_gray);
	m_btnHang2.SetCaption(GetResourceString(IDS_HANGUP));

	m_btnAnswer2.Initialize(IDC_BT_BTN_ANSWER2,this,
		m_pSkinManager->GetTexture(TEXID_1115),NULL,
		m_pSkinManager->GetTexture(TEXID_1116),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAnswer2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnAnswer2.SetSize(&sibtn3);
	m_btnAnswer2.SetScale(0.8f,0.8f*pix_ratio);
	m_btnAnswer2.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+yoffset);
	m_btnAnswer2.SetIcon(&m_iconAnswer);
	m_btnAnswer2.SetIconPos(0,iconoffset);
	m_btnAnswer2.LinkTextGL(pTextGL);
	m_btnAnswer2.SetCaptionHeight(fontsize);
	m_btnAnswer2.SetCaptionPos(BTN_CAPTION_BOTTOM,0,captionoffset);
	m_btnAnswer2.SetCaptionColor(&cr_gray);
	m_btnAnswer2.SetCaption(GetResourceString(IDS_ANSWER));

	//keyboard:
	COLORGL cr_key={1.0,1.0,1.0,1.0};
	COLORGL cr_key_down={0.0,0.0,0.0,1.0};

	CTexture *pUp=pSkinManager->GetTexture(TEXID_521);
	CTexture *pDown=pSkinManager->GetTexture(TEXID_522);

	//int key_margin=10;
	int cy_key=(g_iClientHeight-238)/3;
	int cx_key=cy_key*3/2;
	// 	int cx_key=160;
	// 	int cy_key=80;

	SIZE si_key={cx_key-4,cy_key-4};
	GLfloat px=-cx_key-cx_key/2;
	GLfloat py=cy_key+30;
	GLfloat scalesize=0.5f;

	SIZE siInfo2={cx_key*5/2,cy_key-4};

	m_tagDialString.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDialString.SetSize(&siInfo2);
	m_tagDialString.LinkTextGL(pTextGL);
	m_tagDialString.SetPos(0,g_iClientHeight/2-40);
	m_tagDialString.SetCaptionHeight(32);
	m_tagDialString.EnableDrawBack(FALSE);
	m_tagDialString.SetCaptionPos(BTN_CAPTION_CENTER);

	//1-4
	m_btnNum1.Initialize(IDC_BT_BTN_KEY_1,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum1.SetCaption(L"1");
	m_btnNum1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum1.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum1.SetCaptionHeight(53);
	m_btnNum1.LinkTextGL(pTextGL);
	m_btnNum1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum1.SetPos(px,py);
	m_btnNum1.SetScale(scalesize,scalesize*pix_ratio);

	m_btnNum2.Initialize(IDC_BT_BTN_KEY_2,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum2.SetCaption(L"2");
	m_btnNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum2.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum2.SetCaptionHeight(53);
	m_btnNum2.LinkTextGL(pTextGL);
	m_btnNum2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum2.SetPos(px+cx_key,py);
	m_btnNum2.SetScale(scalesize,scalesize*pix_ratio);

	m_btnNum3.Initialize(IDC_BT_BTN_KEY_3,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum3.SetCaption(L"3");
	m_btnNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum3.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum3.SetCaptionHeight(53);
	m_btnNum3.LinkTextGL(pTextGL);
	m_btnNum3.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum3.SetPos(px+cx_key*2,py);
	m_btnNum3.SetScale(scalesize,scalesize*pix_ratio);

	m_btnNum4.Initialize(IDC_BT_BTN_KEY_4,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum4.SetCaption(L"4");
	m_btnNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum4.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum4.SetCaptionHeight(53);
	m_btnNum4.LinkTextGL(pTextGL);
	m_btnNum4.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum4.SetPos(px+cx_key*3,py);
	m_btnNum4.SetScale(scalesize,scalesize*pix_ratio);

	//5-8:
	m_btnNum5.Initialize(IDC_BT_BTN_KEY_5,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum5.SetCaption(L"5");
	m_btnNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum5.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum5.SetCaptionHeight(53);
	m_btnNum5.LinkTextGL(pTextGL);
	m_btnNum5.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum5.SetPos(px,py-cy_key);
	m_btnNum5.SetScale(scalesize,scalesize*pix_ratio);

	m_btnNum6.Initialize(IDC_BT_BTN_KEY_6,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum6.SetCaption(L"6");
	m_btnNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum6.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum6.SetCaptionHeight(53);
	m_btnNum6.LinkTextGL(pTextGL);
	m_btnNum6.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum6.SetPos(px+cx_key,py-cy_key);
	m_btnNum6.SetScale(scalesize,scalesize*pix_ratio);

	m_btnNum7.Initialize(IDC_BT_BTN_KEY_7,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum7.SetCaption(L"7");
	m_btnNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum7.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum7.SetCaptionHeight(53);
	m_btnNum7.LinkTextGL(pTextGL);
	m_btnNum7.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum7.SetPos(px+cx_key*2,py-cy_key);
	m_btnNum7.SetScale(scalesize,scalesize*pix_ratio);

	m_btnNum8.Initialize(IDC_BT_BTN_KEY_8,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum8.SetCaption(L"8");
	m_btnNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum8.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum8.SetCaptionHeight(53);
	m_btnNum8.LinkTextGL(pTextGL);
	m_btnNum8.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum8.SetPos(px+cx_key*3,py-cy_key);
	m_btnNum8.SetScale(scalesize,scalesize*pix_ratio);

	//9-#:
	m_btnNum9.Initialize(IDC_BT_BTN_KEY_9,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum9.SetCaption(L"9");
	m_btnNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum9.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum9.SetCaptionHeight(53);
	m_btnNum9.LinkTextGL(pTextGL);
	m_btnNum9.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum9.SetPos(px,py-cy_key*2);
	m_btnNum9.SetScale(scalesize,scalesize*pix_ratio);

	//*0#:
	m_btnStar.Initialize(IDC_BT_BTN_KEY_STAR,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnStar.SetCaption(L"*");
	m_btnStar.SetCaptionPos(BTN_CAPTION_CENTER,0,-6);
	m_btnStar.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnStar.SetCaptionHeight(53);
	m_btnStar.LinkTextGL(pTextGL);
	m_btnStar.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnStar.SetPos(px+cx_key,py-cy_key*2);
	m_btnStar.SetScale(scalesize,scalesize*pix_ratio);

	m_btnNum0.Initialize(IDC_BT_BTN_KEY_0,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum0.SetCaption(L"0");
	m_btnNum0.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum0.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum0.SetCaptionHeight(53);
	m_btnNum0.LinkTextGL(pTextGL);
	m_btnNum0.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum0.SetPos(px+cx_key*2,py-cy_key*2);
	m_btnNum0.SetScale(scalesize,scalesize*pix_ratio);

	m_btnWell.Initialize(IDC_BT_BTN_KEY_WELL,this,
		m_pSkinManager->GetTexture(TEXID_1119),NULL,
		m_pSkinManager->GetTexture(TEXID_1114),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnWell.SetCaption(L"#");
	m_btnWell.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnWell.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnWell.SetCaptionHeight(53);
	m_btnWell.LinkTextGL(pTextGL);
	m_btnWell.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnWell.SetPos(px+cx_key*3,py-cy_key*2);
	m_btnWell.SetScale(scalesize,scalesize*pix_ratio);

#else
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

	//list:
	m_iconKeyboard.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_535));
	m_iconHang.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_539));
	m_iconAnswer.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_518));

	m_iconKeyboardFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_536));


	m_iconACHPhone.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_537));
	m_iconACHPhoneFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_538));
	m_iconACHBT.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_542));
	m_iconACHBTFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_543));
	m_iconMic.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_554));
	m_iconMicFocus.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_555));


	COLORGL cr_gray={0.7,0.7,0.7,1.0};

	int margin_x=10;
	// 	int cx=(g_iClientWidth-margin_x*2)/4;
	int cx=(g_iClientWidth-margin_x*2)/3;
	int cy=120;

	SIZE sibtn1={cx*1-4,cy-4};
	SIZE sibtn2={cx*1-4,cy-4};
	SIZE sibtn3={160,72};

	//
	wchar_t caption[32];

	m_btnCall1.Initialize(IDC_BT_BTN_SWITCH_CALL1,this,pSkinManager->GetTexture(TEXID_512),NULL,
		pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCall1.SetSize(&sibtn3);
	m_btnCall1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnCall1.SetPos(-g_iClientWidth/2+10+100,g_iScreenHeight/2-sibtn3.cy/2-4);

	swprintf_s(caption,32,_T("%s 1"),GetResourceString(IDS_CALL));
	m_btnCall1.SetCaption(caption);

	m_btnCall1.LinkTextGL(pTextGL);
	m_btnCall1.SetCaptionPos(BTN_CAPTION_CENTER);

	m_btnCall2.Initialize(IDC_BT_BTN_SWITCH_CALL2,this,pSkinManager->GetTexture(TEXID_512),NULL,
		pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCall2.SetSize(&sibtn3);
	m_btnCall2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnCall2.SetPos(g_iClientWidth/2-10-100,g_iScreenHeight/2-sibtn3.cy/2-4);

	swprintf_s(caption,32,_T("%s 2"),GetResourceString(IDS_CALL));
	m_btnCall2.SetCaption(caption);

	m_btnCall2.LinkTextGL(pTextGL);
	m_btnCall2.SetCaptionPos(BTN_CAPTION_CENTER);

	//
	m_btnKeyboard.Initialize(IDC_BT_BTN_KEYBOARD,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnKeyboard.SetSize(&sibtn1);
	m_btnKeyboard.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnKeyboard.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+60);
	m_btnKeyboard.SetIcon(&m_iconKeyboard,&m_iconKeyboardFocus);
	m_btnKeyboard.SetIconScale(1.1f,1.1f);
	m_btnKeyboard.SetShowScale(0.9f);

	m_btnACH.Initialize(IDC_BT_BTN_ACH,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnACH.SetSize(&sibtn1);
	m_btnACH.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnACH.SetPos((-g_iScreenWidth/2+margin_x+cx/2) + (g_iScreenWidth-2*margin_x-cx)/3,-g_iClientHeight/2+60);
	m_btnACH.SetIcon(&m_iconACHBT,&m_iconACHBTFocus);
	m_btnACH.SetIconScale(1.1f,1.1f);
	m_btnACH.SetShowScale(0.9f);

	m_btnMic.Initialize(IDC_BT_BTN_MIC_MUTE,this,m_pSkinManager->GetTexture(TEXID_521),NULL,
		m_pSkinManager->GetTexture(TEXID_522),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnMic.SetSize(&sibtn1);
	m_btnMic.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnMic.SetPos((g_iScreenWidth/2-margin_x-cx/2)-(g_iScreenWidth-2*margin_x-cx)/3,-g_iClientHeight/2+60);
	m_btnMic.SetIcon(&m_iconMic,&m_iconMicFocus);
	m_btnMic.SetIconScale(1.1f,1.1f);
	m_btnMic.SetShowScale(0.9f);

	m_btnHang1.Initialize(IDC_BT_BTN_HANG1,this,m_pSkinManager->GetTexture(TEXID_533),NULL,
		m_pSkinManager->GetTexture(TEXID_534),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnHang1.SetSize(&sibtn2);
	m_btnHang1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnHang1.SetPos(g_iScreenWidth/2-margin_x-cx/2,-g_iClientHeight/2+60);
	m_btnHang1.SetIcon(&m_iconHang);
	m_btnHang1.SetIconScale(1.1f,1.1f);
	m_btnHang1.SetShowScale(0.9f);

	m_btnAnswer1.Initialize(IDC_BT_BTN_ANSWER1,this,m_pSkinManager->GetTexture(TEXID_516),NULL,
		m_pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnAnswer1.SetSize(&sibtn2);
	m_btnAnswer1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnAnswer1.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+60);
	m_btnAnswer1.SetIcon(&m_iconAnswer);
	m_btnAnswer1.SetIconScale(1.1f,1.1f);
	m_btnAnswer1.SetShowScale(0.9f);

	m_btnHang2.Initialize(IDC_BT_BTN_HANG2,this,m_pSkinManager->GetTexture(TEXID_533),NULL,
		m_pSkinManager->GetTexture(TEXID_534),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnHang2.SetSize(&sibtn2);
	m_btnHang2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnHang2.SetPos(g_iScreenWidth/2-margin_x-cx/2,-g_iClientHeight/2+60);
	m_btnHang2.SetIcon(&m_iconHang);
	m_btnHang2.SetIconScale(1.1f,1.1f);
	m_btnHang2.SetShowScale(0.9f);

	m_btnAnswer2.Initialize(IDC_BT_BTN_ANSWER2,this,m_pSkinManager->GetTexture(TEXID_516),NULL,
		m_pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	//m_btnAnswer2.SetSize(&sibtn2);
	m_btnAnswer2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnAnswer2.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+60);
	m_btnAnswer2.SetIcon(&m_iconAnswer);
	m_btnAnswer2.SetIconScale(1.1f,1.1f);
	m_btnAnswer2.SetShowScale(0.9f);

	//keyboard:
	COLORGL cr_key={1.0,1.0,1.0,1.0};
	COLORGL cr_key_down={0.0,0.0,0.0,1.0};

	CTexture *pUp=pSkinManager->GetTexture(TEXID_521);
	CTexture *pDown=pSkinManager->GetTexture(TEXID_522);

	//int key_margin=10;
	int cy_key=(g_iClientHeight-238)/3;
	int cx_key=cy_key*3/2;
	// 	int cx_key=160;
	// 	int cy_key=80;

	SIZE si_key={cx_key-4,cy_key-4};
	GLfloat px=-cx_key-cx_key/2;
	GLfloat py=cy_key+20;

	// 	SIZE si_dial={cx_key*3-4,cy_key-4};

	SIZE siInfo2={cx_key*5/2,cy_key-4};

	m_tagDialString.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDialString.SetSize(&siInfo2);
	//m_tagDialString.SetCaption(_T("13870204516888888"));
	m_tagDialString.LinkTextGL(pTextGL);
	m_tagDialString.SetPos(0,g_iClientHeight/2-60);
	m_tagDialString.SetCaptionHeight(32);
	m_tagDialString.EnableDrawBack(FALSE);
	m_tagDialString.SetCaptionPos(BTN_CAPTION_CENTER);
	// 	m_tagDialString.SetCaptionColor(&crb);

	//1-4
	m_btnNum1.Initialize(IDC_BT_BTN_KEY_1,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum1.SetCaption(L"1");
	m_btnNum1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum1.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum1.SetCaptionHeight(53);
	m_btnNum1.LinkTextGL(pTextGL);
	//m_btnNum1.SetSize(&si_key);
	m_btnNum1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum1.SetPos(px,py);
	m_btnNum1.SetShowScale(0.64f);

	m_btnNum2.Initialize(IDC_BT_BTN_KEY_2,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum2.SetCaption(L"2");
	m_btnNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum2.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum2.SetCaptionHeight(53);
	m_btnNum2.LinkTextGL(pTextGL);
	//m_btnNum2.SetSize(&si_key);
	m_btnNum2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum2.SetPos(px+cx_key,py);
	m_btnNum2.SetShowScale(0.64f);

	m_btnNum3.Initialize(IDC_BT_BTN_KEY_3,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum3.SetCaption(L"3");
	m_btnNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum3.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum3.SetCaptionHeight(53);
	m_btnNum3.LinkTextGL(pTextGL);
	//m_btnNum3.SetSize(&si_key);
	m_btnNum3.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum3.SetPos(px+cx_key*2,py);
	m_btnNum3.SetShowScale(0.64f);

	m_btnNum4.Initialize(IDC_BT_BTN_KEY_4,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum4.SetCaption(L"4");
	m_btnNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum4.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum4.SetCaptionHeight(53);
	m_btnNum4.LinkTextGL(pTextGL);
	//m_btnNum4.SetSize(&si_key);
	m_btnNum4.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum4.SetPos(px+cx_key*3,py);
	m_btnNum4.SetShowScale(0.64f);

	//5-8:
	m_btnNum5.Initialize(IDC_BT_BTN_KEY_5,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum5.SetCaption(L"5");
	m_btnNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum5.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum5.SetCaptionHeight(53);
	m_btnNum5.LinkTextGL(pTextGL);
	//m_btnNum5.SetSize(&si_key);
	m_btnNum5.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum5.SetPos(px,py-cy_key);
	m_btnNum5.SetShowScale(0.64f);

	m_btnNum6.Initialize(IDC_BT_BTN_KEY_6,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum6.SetCaption(L"6");
	m_btnNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum6.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum6.SetCaptionHeight(53);
	m_btnNum6.LinkTextGL(pTextGL);
	//m_btnNum6.SetSize(&si_key);
	m_btnNum6.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum6.SetPos(px+cx_key,py-cy_key);
	m_btnNum6.SetShowScale(0.64f);

	m_btnNum7.Initialize(IDC_BT_BTN_KEY_7,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum7.SetCaption(L"7");
	m_btnNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum7.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum7.SetCaptionHeight(53);
	m_btnNum7.LinkTextGL(pTextGL);
	//m_btnNum7.SetSize(&si_key);
	m_btnNum7.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum7.SetPos(px+cx_key*2,py-cy_key);
	m_btnNum7.SetShowScale(0.64f);

	m_btnNum8.Initialize(IDC_BT_BTN_KEY_8,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum8.SetCaption(L"8");
	m_btnNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum8.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum8.SetCaptionHeight(53);
	m_btnNum8.LinkTextGL(pTextGL);
	//m_btnNum8.SetSize(&si_key);
	m_btnNum8.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum8.SetPos(px+cx_key*3,py-cy_key);
	m_btnNum8.SetShowScale(0.64f);

	//9-#:
	m_btnNum9.Initialize(IDC_BT_BTN_KEY_9,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum9.SetCaption(L"9");
	m_btnNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum9.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum9.SetCaptionHeight(53);
	m_btnNum9.LinkTextGL(pTextGL);
	//m_btnNum9.SetSize(&si_key);
	m_btnNum9.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum9.SetPos(px,py-cy_key*2);
	m_btnNum9.SetShowScale(0.64f);

	//*0#:
	m_btnStar.Initialize(IDC_BT_BTN_KEY_STAR,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnStar.SetCaption(L"*");
	m_btnStar.SetCaptionPos(BTN_CAPTION_CENTER,0,-6);
	m_btnStar.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnStar.SetCaptionHeight(53);
	m_btnStar.LinkTextGL(pTextGL);
	//m_btnStar.SetSize(&si_key);
	m_btnStar.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnStar.SetPos(px+cx_key,py-cy_key*2);
	m_btnStar.SetShowScale(0.64f);

	m_btnNum0.Initialize(IDC_BT_BTN_KEY_0,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum0.SetCaption(L"0");
	m_btnNum0.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum0.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum0.SetCaptionHeight(53);
	m_btnNum0.LinkTextGL(pTextGL);
	//m_btnNum0.SetSize(&si_key);
	m_btnNum0.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum0.SetPos(px+cx_key*2,py-cy_key*2);
	m_btnNum0.SetShowScale(0.64f);

	m_btnWell.Initialize(IDC_BT_BTN_KEY_WELL,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnWell.SetCaption(L"#");
	m_btnWell.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnWell.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnWell.SetCaptionHeight(53);
	m_btnWell.LinkTextGL(pTextGL);
	//m_btnWell.SetSize(&si_key);
	m_btnWell.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnWell.SetPos(px+cx_key*3,py-cy_key*2);
	m_btnWell.SetShowScale(0.64f);
#endif	//CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	SetDTMFMode(FALSE);

	return TRUE;
}
void CALLBACK CPanelBluetoothTalking::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_BT,(_T("CPanelBluetoothTalking OnNotify id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara,lpUserPara));

	CPanelBluetoothTalking *panel=(CPanelBluetoothTalking*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelBluetoothTalking::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_BT_BTN_KEY_0:
		Input('0');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_1:
		Input('1');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_2:
		Input('2');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_3:
		Input('3');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_4:
		Input('4');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_5:
		Input('5');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_6:
		Input('6');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_7:
		Input('7');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_8:
		Input('8');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_9:
		Input('9');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_STAR:
		Input('*');
		PlaySoundEffect(0);
		break;
	case IDC_BT_BTN_KEY_WELL:
		Input('#');
		PlaySoundEffect(0);
		break;

	case IDC_BT_BTN_KEYBOARD:
		{
			SetDTMFMode(!m_bOnDTMF);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_SWITCH_CALL1:
		if(m_dwCurCallIndex!=0)
		{
			if(m_iCallState2==DIAL_STATE_TALKING)
				SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_SWITCHCALLS);

			SelectCallIndex(0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_SWITCH_CALL2:
		if(m_dwCurCallIndex!=1)
		{
			if(m_iCallState1==DIAL_STATE_TALKING)
				SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_SWITCHCALLS);

			SelectCallIndex(1);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_HANG1:
		{
			if(m_dwCallCount>1)
			{
				SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_END1STCALL);
			}
			else
			{
				SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_HANGUP1);
			}

			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_HANG2:
		{
			if(m_iCallState2==DIAL_STATE_INCOMING || m_iCallState2==DIAL_STATE_WAITING)
			{
				SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_ENDINCOMINGCALL);
			}
			else
			{
				SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_HANGUP2);
			}

			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_ANSWER1:
		{
			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_ANSWER1);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_ANSWER2:
		{
// 			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_HOLD1STCALL);
// 			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_ANSWER2);
			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_SWITCHCALLS);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_ACH:
		{
			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_ACH);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_MIC_MUTE:
		{
			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_MIC_MUTE);
			PlaySoundEffect(0);
		}
		break;
	default:
		break;
	}
}


void CPanelBluetoothTalking::Input(wchar_t c)
{
	wchar_t str[8];
	swprintf_s(str,8,_T("%c"),c);

	m_strDialString += str;
	if(m_strDialString.Size())
	{
		UINT maxlen=g_iClientWidth*11/800;
		if(m_strDialString.Size()<=maxlen)
		{
			m_tagDialString.SetCaption(m_strDialString.String());
		}
		else
		{
			m_tagDialString.SetCaption(m_strDialString.String()+m_strDialString.Size()-maxlen);
		}
	}

	BT_DTMF(c);
}
void CPanelBluetoothTalking::SetDTMFMode(BOOL bDTMF)
{
	m_bOnDTMF = bDTMF;

	if(!bDTMF)
	{
		m_strDialString = NULL;
		m_tagDialString.SetCaption(NULL);
	}

	m_btnKeyboard.SetStatus(bDTMF?BS_FOCUS:BS_UP);

	m_btnNum1.Show(bDTMF);
	m_btnNum2.Show(bDTMF);
	m_btnNum3.Show(bDTMF);

	m_btnNum4.Show(bDTMF);
	m_btnNum5.Show(bDTMF);
	m_btnNum6.Show(bDTMF);

	m_btnNum7.Show(bDTMF);
	m_btnNum8.Show(bDTMF);
	m_btnNum9.Show(bDTMF);

	m_btnStar.Show(bDTMF);
	m_btnNum0.Show(bDTMF);
	m_btnWell.Show(bDTMF);


	m_tagDialString.Show(bDTMF);
}
void CPanelBluetoothTalking::OnNotifyCallCount(DWORD dwCount)
{
	m_dwCallCount = dwCount;

	m_btnCall1.Show(dwCount>1);
	m_btnCall2.Show(dwCount>1);

	//clear:
	if(dwCount==0)
	{
		m_dwCurCallIndex=0;
		m_bOnDTMF = FALSE;

		m_strName1=NULL;
		m_strNumber1=NULL;
		m_strCallStatus1=NULL;
		m_strMobileArea1=NULL;
		m_strMobileType1=NULL;
		m_bTalking1=FALSE;
		m_dwCallTime1=0;
		m_iCallState1=DIAL_STATE_HANGUP;

		m_strName2=NULL;
		m_strNumber2=NULL;
		m_strCallStatus2=NULL;
		m_strMobileArea2=NULL;
		m_strMobileType2=NULL;
		m_bTalking2=FALSE;
		m_dwCallTime2=0;
		m_iCallState2=DIAL_STATE_HANGUP;

		//quit
		if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
		{
			SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,m_iPreviousPanelParam);
		}
	}
}
void CPanelBluetoothTalking::OnNotifyCallStatus(strCallStatus* callStatus)
{
	if(callStatus)
	{
		//call info:
		CM_String *pName=(callStatus->iCallIndex == 0)?&m_strName1:&m_strName2;
		CM_String *pNum=(callStatus->iCallIndex == 0)?&m_strNumber1:&m_strNumber2;
		CM_String *pStatus=(callStatus->iCallIndex == 0)?&m_strCallStatus1:&m_strCallStatus2;

#if SHOW_PHONCALL_AREA_TYPE
		CM_String *pMobileArea=(callStatus->iCallIndex == 0)?&m_strMobileArea1:&m_strMobileArea2;
		CM_String *pMobileType=(callStatus->iCallIndex == 0)?&m_strMobileType1:&m_strMobileType2;
#endif
		BOOL *pTalking=(callStatus->iCallIndex == 0)?&m_bTalking1:&m_bTalking2;
		DWORD *pCallTime=(callStatus->iCallIndex == 0)?&m_dwCallTime1:&m_dwCallTime2;
		DIAL_STATE *pCallState=(callStatus->iCallIndex == 0)?&m_iCallState1:&m_iCallState2;

		if ((wcslen(callStatus->wcCallerName) == 0) || (wcscmp(callStatus->wcCallerName,callStatus->wcCallerNum) == 0))
		{
			*pName = GetResourceString(IDS_STRANGER);
		}
		else
		{
			if(m_pSkinManager->GetConfig()->GetCurParam()->bVcardFirstNameFirst)
			{
				//swap family name and first name:
				wchar_t temp[64];
				wchar_t *p=temp;
				int family_len=0;
				int total_len=wcslen(callStatus->wcCallerName);
				//cut tail space
				int ispace_tail=0;
				int ispace_mid=0;
				if(total_len>1)
				{
					wchar_t *p=callStatus->wcCallerName+total_len-1;
					while (*p == ' ')
					{
						ispace_tail++;	*p = 0;		p--;
					}
				}
				total_len-=ispace_tail;
				wcscpy_s(temp,64,callStatus->wcCallerName);
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
				if(family_len)
					*pName = temp+family_len+ispace_mid;
				else
					*pName = temp;
			}
			else
			{
				*pName = callStatus->wcCallerName;
			}
		}
		*pNum = callStatus->wcCallerNum;
		*pStatus = DialStateToString(callStatus->iCallState);
		*pTalking=(callStatus->iCallState==DIAL_STATE_TALKING);
		*pCallState=callStatus->iCallState;

		if(callStatus->iCallState==DIAL_STATE_TALKING && *pCallTime==0)
		{
			*pCallTime = GetTickCount();
		}

#if SHOW_PHONCALL_AREA_TYPE
		if((callStatus->iCallState!=DIAL_STATE_HANGUP) && (!pMobileArea->String() || !pMobileType->String()))
		{
			if(m_MobileData.Query(pNum->String()))
			{
				*pMobileArea=m_MobileData.GetArea();
				*pMobileType=m_MobileData.GetType();
			}
		}
#endif
		//UI:
		if(m_dwCallCount>1)//2 calls
		{
			if(m_bEnableAutoSelCall)
			{
				if(callStatus->iCallIndex>0 && callStatus->iCallState==DIAL_STATE_WAITING)
					SelectCallIndex(callStatus->iCallIndex);
				else if(callStatus->iCallState==DIAL_STATE_INCOMING)
					SelectCallIndex(callStatus->iCallIndex);
				else
					SelectCallIndex(m_dwCurCallIndex);
			}
			else
			{
				SelectCallIndex(m_dwCurCallIndex);
			}
		}
		else if(m_dwCallCount==1)//1 call
		{
			//auto select call index:
			if(m_iCallState2!=DIAL_STATE_HANGUP)
				SelectCallIndex(1);
			else
				SelectCallIndex(0);

			m_bEnableAutoSelCall = TRUE;
		}
	}
}
void CPanelBluetoothTalking::SelectCallIndex(DWORD dwIndex)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("SelectCallIndex dwIndex:%d\r\n"),dwIndex));

	CButtonGL *pbtnAnswer = ((dwIndex==0) ? &m_btnAnswer1 : &m_btnAnswer2);
	CButtonGL *pbtnHangup = ((dwIndex==0) ? &m_btnHang1 : &m_btnHang2);
	DIAL_STATE *pCallState = ((dwIndex==0)?&m_iCallState1:&m_iCallState2);

	m_dwCurCallIndex = dwIndex;
	m_bEnableAutoSelCall = FALSE;

	m_btnCall1.SetStatus(dwIndex==0?BS_FOCUS:BS_UP);
	m_btnCall2.SetStatus(dwIndex==1?BS_FOCUS:BS_UP);

	m_btnAnswer1.EnableNotify(dwIndex == 0);
	m_btnHang1.EnableNotify(dwIndex == 0);

	m_btnAnswer2.EnableNotify(dwIndex == 1);
	m_btnHang2.EnableNotify(dwIndex == 1);


	m_btnKeyboard.Enable(*pCallState==DIAL_STATE_TALKING/* || *pCallState==DIAL_STATE_HOLDING*/);
	m_btnKeyboard.Show(*pCallState!=DIAL_STATE_INCOMING && *pCallState!=DIAL_STATE_WAITING);

	m_btnACH.Show(*pCallState!=DIAL_STATE_INCOMING);
	m_btnMic.Show(*pCallState!=DIAL_STATE_INCOMING);

	pbtnAnswer->Show(*pCallState==DIAL_STATE_INCOMING || *pCallState==DIAL_STATE_WAITING);
	pbtnHangup->Show(TRUE);
}
