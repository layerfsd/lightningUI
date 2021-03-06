#include "PanelBluetoothTalking.h"
#include "../../resource.h"

CPanelBluetoothTalking::CPanelBluetoothTalking(void)
{
	m_bOnDTMF = FALSE;
// 	m_dwCallStatus=HF_STATE_IDLE;

	m_pSmartBarPhone = NULL;

	m_dwCallCount=0;
	m_dwCurCallIndex=-1;
	m_bEnableAutoSelCall = TRUE;

	m_dwCallTime1=0;
	m_dwCallTime2=0;

	m_bTalking1 = FALSE;
	m_bTalking2 = FALSE;

	m_ciCallInfo1 = NULL;
	m_ciCallInfo2 = NULL;

	m_dwCallStatus = 0;
	m_dwEnterTick = 0;
	m_bNeedAutoShowSmartBar = TRUE;
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
	if(m_bNeedAutoShowSmartBar && 
		(m_dwCallStatus==HF_STATE_OUTGOING_CALL || m_dwCallStatus==HF_STATE_TALKING || m_dwCallStatus==HF_STATE_INCOMING_CALL)
		)
	{
		if(m_pSmartBarPhone && !m_pSmartBarPhone->IsVisible())
		{
			m_pSmartBarPhone->Show(TRUE,INFINITE);
		}
	}
}

void CPanelBluetoothTalking::Render()
{
	static CTexture *pTexBar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
	static CTexture *pTexBar2=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_056):NULL;
	COLORGL cr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};

	if(!BeginRender())return;

// 	COLORGL cr={0.05,0.05,0.05,1};
// 	DrawRect(0,0,g_iScreenWidth,g_iScreenHeight,&cr);

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

	DrawRect(pTexBar,0,(-g_iClientHeight/2+65),g_iClientWidth-4,134-4,2,&cr,TRUE);
	DrawRect(pTexBar2,0,(g_iClientHeight/2-52),g_iClientWidth-4,104-4,2,&cr,TRUE);

	m_btnCall1.SetAlpha(m_fCurAlpha);
	m_btnCall1.Render();
	m_btnCall2.SetAlpha(m_fCurAlpha);
	m_btnCall2.Render();

	m_btnKeyboard.SetAlpha(m_fCurAlpha);
	m_btnKeyboard.Render();
	m_btnACH.SetAlpha(m_fCurAlpha);
	m_btnACH.Render();

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
		CM_String *pstrArea = ((m_dwCurCallIndex==0) ? &m_strMobileArea1 : &m_strMobileArea2);
		CM_String *pstrType = ((m_dwCurCallIndex==0) ? &m_strMobileType1 : &m_strMobileType2);

		if(m_pTextGL)
		{
			m_pTextGL->SetAlpha(m_fCurAlpha);
			m_pTextGL->SetLuminance(m_fCurLuminance);

			//status:
			if(pstrCallStatus->String())
			{
				m_pTextGL->SetColor(&cr,FALSE);
				m_pTextGL->SetString(pstrCallStatus->String());
				m_pTextGL->SetCharScale(1.2f,1.2f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-GLfloat(si.cx/2),(g_iClientHeight/2-32));
				m_pTextGL->Render();
			}

			//area and type:
			if(pstrArea->String())
			{
				m_pTextGL->SetColor(&cr_b,FALSE);
				m_pTextGL->SetString(pstrArea->String());
				m_pTextGL->SetCharScale(0.80f,0.80f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-GLfloat(si.cx)/2/*-40*/,130);
				m_pTextGL->Render();
			}
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
				m_pTextGL->SetColor(&cr,FALSE);
				m_pTextGL->SetString(pstrName);
				m_pTextGL->SetCharScale(1.2f,1.2f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-GLfloat(si.cx/2),90);
				m_pTextGL->Render();
			}

			//number:
			if(pstrNumber)
			{
				m_pTextGL->SetColor(&cr,FALSE);
				m_pTextGL->SetString(pstrNumber);
				m_pTextGL->SetCharScale(1.0f,1.0f);
				m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-GLfloat(si.cx/2),30);
				m_pTextGL->Render();
			}
			//call time
			if(*pbTalking && pbtnHangup->IsVisible() && *pdwCallTime!=0)
			{
				int isec=(GetTickCount()-*pdwCallTime)/1000;
				wchar_t str[16];
				swprintf_s(str,16,_T("%02d:%02d"),isec/60,isec%60);

				m_pTextGL->SetColor(&cr,FALSE);
				m_pTextGL->SetStyle(STYLE_ART);
				m_pTextGL->SetString(str);
				m_pTextGL->SetCharScale(1.0f,1.0f);
				m_pTextGL->GetExtension(&si,STYLE_ART);
				m_pTextGL->SetPos(-GLfloat(si.cx/2),-24);
				m_pTextGL->Render();
				m_pTextGL->SetStyle(STYLE_NORMAL);//reset
			}
		}
	}

	EndRender();
}
void CPanelBluetoothTalking::SetAudioDir(UINT idDir)
{
	if(idDir == HFM_AD_IN_DEVICE)
		m_btnACH.SetIcon(&m_iconACHBT,&m_iconACHBTFocus);
	else
		m_btnACH.SetIcon(&m_iconACHPhone,&m_iconACHPhoneFocus);

}
void CPanelBluetoothTalking::SetCallStatus(DWORD dwCallStatus)
{
	if(m_dwCallStatus != dwCallStatus)
	{
		RETAILMSG(DEBUG_BT,(_T("@@@@@@@ CPanelBluetoothTalking::SetCallStatus dwCallStatus:%d\r\n"),dwCallStatus));

		m_dwCallStatus = dwCallStatus;

		//m_bEnableQuit = (dwCallStatus==HF_STATE_IDLE || dwCallStatus==HF_STATE_TERMINATING);

		/*
		if(GET_PANEL_CLASS_ID(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI) == UI_CLASS_NAVI)
		{
			//star ring:
			::PostMessage(m_hMainWnd,WM_MUTE_AUDIO_CMD,(1<<24)+(0<<16)+_ARM_MUTE_ALL,(1<<16)+(100));
			StartRingTone();
		}
		else
		{
			m_bBeginPlayRingTone = (dwCallStatus == HF_STATE_INCOMING_CALL);
		}

		//end:
		if(dwCallStatus != HF_STATE_INCOMING_CALL)
		{
			EndRingTone();
		}
		*/
	}
}

BOOL CPanelBluetoothTalking::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);

	//m_bEnableQuit = FALSE;

	m_strCallStatus1=_T("status1");
	m_strName1=_T("NAME1");
	m_strNumber1=_T("NUMBER1");
	m_strCallStatus2=_T("status2");
	m_strName2=_T("NAME2");
	m_strNumber2=_T("NUMBER2");

	SIZE si;
	VIEW_STATE vs;

	m_tagBkBlur.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_100));
	si=m_tagBkBlur.GetTexture()->GetImageSize();
	m_tagBkBlur.SetScale(GLfloat(g_iClientWidth)/si.cx,GLfloat(g_iClientHeight)/si.cy);
	vs=m_tagBkBlur.GetCurViewState();
	vs.fRotateX=-180;
	m_tagBkBlur.SetIniViewState(&vs);

	m_tagBkOrigin.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_101));
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


	COLORGL cr_gray={0.7,0.7,0.7,1.0};

	int margin_x=0;
// 	int cx=(g_iClientWidth-margin_x*2)/4;
	int cx=(g_iClientWidth-margin_x*2)/3;
	int cy=120;

	SIZE sibtn1={cx*1-4,cy-4};
	SIZE sibtn2={cx*1-4,cy-4};
	SIZE sibtn3={200,90};

	//
	wchar_t caption[32];

	m_btnCall1.Initialize(IDC_BT_BTN_SWITCH_CALL1,this,pSkinManager->GetTexture(TEXID_512),NULL,
		pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnCall1.SetSize(&sibtn3);
	m_btnCall1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnCall1.SetPos(-g_iClientWidth/2+10+100,g_iScreenHeight/2-10-50);

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
	m_btnCall2.SetPos(g_iClientWidth/2-10-100,g_iScreenHeight/2-10-50);

	swprintf_s(caption,32,_T("%s 2"),GetResourceString(IDS_CALL));
	m_btnCall2.SetCaption(caption);

	m_btnCall2.LinkTextGL(pTextGL);
	m_btnCall2.SetCaptionPos(BTN_CAPTION_CENTER);

	//
	m_btnKeyboard.Initialize(IDC_BT_BTN_KEYBOARD,this,m_pSkinManager->GetTexture(TEXID_531),NULL,
		m_pSkinManager->GetTexture(TEXID_532),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnKeyboard.SetSize(&sibtn1);
	m_btnKeyboard.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnKeyboard.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+60);
	m_btnKeyboard.SetIcon(&m_iconKeyboard,&m_iconKeyboardFocus);

	m_btnACH.Initialize(IDC_BT_BTN_ACH,this,m_pSkinManager->GetTexture(TEXID_531),NULL,
		m_pSkinManager->GetTexture(TEXID_532),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnACH.SetSize(&sibtn1);
	m_btnACH.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnACH.SetPos(/*-g_iScreenWidth/2+margin_x+cx/2*/0,-g_iClientHeight/2+60);
	m_btnACH.SetIcon(&m_iconACHBT,&m_iconACHBTFocus);


	m_btnHang1.Initialize(IDC_BT_BTN_HANG1,this,m_pSkinManager->GetTexture(TEXID_533),NULL,
		m_pSkinManager->GetTexture(TEXID_534),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHang1.SetSize(&sibtn2);
	m_btnHang1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnHang1.SetPos(g_iScreenWidth/2-margin_x-cx/2,-g_iClientHeight/2+60);
	m_btnHang1.SetIcon(&m_iconHang);

	m_btnAnswer1.Initialize(IDC_BT_BTN_ANSWER1,this,m_pSkinManager->GetTexture(TEXID_516),NULL,
		m_pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAnswer1.SetSize(&sibtn2);
	m_btnAnswer1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnAnswer1.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+60);
	m_btnAnswer1.SetIcon(&m_iconAnswer);

	m_btnHang2.Initialize(IDC_BT_BTN_HANG2,this,m_pSkinManager->GetTexture(TEXID_533),NULL,
		m_pSkinManager->GetTexture(TEXID_534),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnHang2.SetSize(&sibtn2);
	m_btnHang2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnHang2.SetPos(g_iScreenWidth/2-margin_x-cx/2,-g_iClientHeight/2+60);
	m_btnHang2.SetIcon(&m_iconHang);

	m_btnAnswer2.Initialize(IDC_BT_BTN_ANSWER2,this,m_pSkinManager->GetTexture(TEXID_516),NULL,
		m_pSkinManager->GetTexture(TEXID_517),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnAnswer2.SetSize(&sibtn2);
	m_btnAnswer2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnAnswer2.SetPos(-g_iScreenWidth/2+margin_x+cx/2,-g_iClientHeight/2+60);
	m_btnAnswer2.SetIcon(&m_iconAnswer);

	//keyboard:
	COLORGL cr_black={0.0,0.0,0.0,1.0};

	CTexture *pUp=pSkinManager->GetTexture(TEXID_514);
	CTexture *pDown=pSkinManager->GetTexture(TEXID_515);

	//int key_margin=10;
	int cy_key=(g_iClientHeight-238)/4;
	int cx_key=cy_key*2;
// 	int cx_key=160;
// 	int cy_key=80;

	SIZE si_key={cx_key-4,cy_key-4};
	GLfloat px=-cx_key;
	GLfloat py=cy_key*3/2+15;

// 	SIZE si_dial={cx_key*3-4,cy_key-4};

	SIZE siInfo2={cx_key*5/2,cy_key-4};

	m_tagDialString.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagDialString.SetSize(&siInfo2);
	//m_tagDialString.SetCaption(_T("13870204516888888"));
	m_tagDialString.LinkTextGL(pTextGL);
	m_tagDialString.SetPos(0,g_iClientHeight/2-60);
	m_tagDialString.SetCaptionHeight(40);
	m_tagDialString.EnableDrawBack(FALSE);
	m_tagDialString.SetCaptionPos(BTN_CAPTION_CENTER);
// 	m_tagDialString.SetCaptionColor(&crb);

	//1-3
	m_btnNum1.Initialize(IDC_BT_BTN_KEY_1,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum1.SetCaption(L"1");
	m_btnNum1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum1.SetCaptionColor(&cr_black);
	m_btnNum1.SetCaptionHeight(40);
	m_btnNum1.LinkTextGL(pTextGL);
	m_btnNum1.SetSize(&si_key);
	m_btnNum1.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum1.SetPos(px,py);

	m_btnNum2.Initialize(IDC_BT_BTN_KEY_2,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum2.SetCaption(L"2");
	m_btnNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum2.SetCaptionColor(&cr_black);
	m_btnNum2.SetCaptionHeight(40);
	m_btnNum2.LinkTextGL(pTextGL);
	m_btnNum2.SetSize(&si_key);
	m_btnNum2.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum2.SetPos(px+cx_key,py);

	m_btnNum3.Initialize(IDC_BT_BTN_KEY_3,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum3.SetCaption(L"3");
	m_btnNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum3.SetCaptionColor(&cr_black);
	m_btnNum3.SetCaptionHeight(40);
	m_btnNum3.LinkTextGL(pTextGL);
	m_btnNum3.SetSize(&si_key);
	m_btnNum3.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum3.SetPos(px+cx_key*2,py);

	//4-6:
	m_btnNum4.Initialize(IDC_BT_BTN_KEY_4,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum4.SetCaption(L"4");
	m_btnNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum4.SetCaptionColor(&cr_black);
	m_btnNum4.SetCaptionHeight(40);
	m_btnNum4.LinkTextGL(pTextGL);
	m_btnNum4.SetSize(&si_key);
	m_btnNum4.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum4.SetPos(px,py-cy_key);

	m_btnNum5.Initialize(IDC_BT_BTN_KEY_5,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum5.SetCaption(L"5");
	m_btnNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum5.SetCaptionColor(&cr_black);
	m_btnNum5.SetCaptionHeight(40);
	m_btnNum5.LinkTextGL(pTextGL);
	m_btnNum5.SetSize(&si_key);
	m_btnNum5.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum5.SetPos(px+cx_key,py-cy_key);

	m_btnNum6.Initialize(IDC_BT_BTN_KEY_6,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum6.SetCaption(L"6");
	m_btnNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum6.SetCaptionColor(&cr_black);
	m_btnNum6.SetCaptionHeight(40);
	m_btnNum6.LinkTextGL(pTextGL);
	m_btnNum6.SetSize(&si_key);
	m_btnNum6.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum6.SetPos(px+cx_key*2,py-cy_key);


	//7-9:
	m_btnNum7.Initialize(IDC_BT_BTN_KEY_7,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum7.SetCaption(L"7");
	m_btnNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum7.SetCaptionColor(&cr_black);
	m_btnNum7.SetCaptionHeight(40);
	m_btnNum7.LinkTextGL(pTextGL);
	m_btnNum7.SetSize(&si_key);
	m_btnNum7.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum7.SetPos(px,py-cy_key*2);

	m_btnNum8.Initialize(IDC_BT_BTN_KEY_8,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum8.SetCaption(L"8");
	m_btnNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum8.SetCaptionColor(&cr_black);
	m_btnNum8.SetCaptionHeight(40);
	m_btnNum8.LinkTextGL(pTextGL);
	m_btnNum8.SetSize(&si_key);
	m_btnNum8.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum8.SetPos(px+cx_key,py-cy_key*2);

	m_btnNum9.Initialize(IDC_BT_BTN_KEY_9,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum9.SetCaption(L"9");
	m_btnNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum9.SetCaptionColor(&cr_black);
	m_btnNum9.SetCaptionHeight(40);
	m_btnNum9.LinkTextGL(pTextGL);
	m_btnNum9.SetSize(&si_key);
	m_btnNum9.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum9.SetPos(px+cx_key*2,py-cy_key*2);

	//*0#:
	m_btnStar.Initialize(IDC_BT_BTN_KEY_STAR,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnStar.SetCaption(L"*");
	m_btnStar.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnStar.SetCaptionColor(&cr_black);
	m_btnStar.SetCaptionHeight(40);
	m_btnStar.LinkTextGL(pTextGL);
	m_btnStar.SetSize(&si_key);
	m_btnStar.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnStar.SetPos(px,py-cy_key*3);

	m_btnNum0.Initialize(IDC_BT_BTN_KEY_0,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum0.SetCaption(L"0");
	m_btnNum0.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum0.SetCaptionColor(&cr_black);
	m_btnNum0.SetCaptionHeight(40);
	m_btnNum0.LinkTextGL(pTextGL);
	m_btnNum0.SetSize(&si_key);
	m_btnNum0.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnNum0.SetPos(px+cx_key,py-cy_key*3);

	m_btnWell.Initialize(IDC_BT_BTN_KEY_WELL,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnWell.SetCaption(L"#");
	m_btnWell.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnWell.SetCaptionColor(&cr_black);
	m_btnWell.SetCaptionHeight(40);
	m_btnWell.LinkTextGL(pTextGL);
	m_btnWell.SetSize(&si_key);
	m_btnWell.SetNotifyFunc(&CPanelBluetoothTalking::OnNotify,this);
	m_btnWell.SetPos(px+cx_key*2,py-cy_key*3);

	//

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
		{
			SelectCallIndex(0);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_SWITCH_CALL2:
		{
			SelectCallIndex(1);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_HANG1:
		{
			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_HANGUP1);
			//Sleep(200);
			QuitPanel();
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_HANG2:
		{
			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_HANGUP2);
			//Sleep(200);
			QuitPanel();
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
			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_ANSWER2);
			PlaySoundEffect(0);
		}
		break;
	case IDC_BT_BTN_ACH:
		{
			SendBluetoothCommand(BT_CMD_HFP_CONTROL,HFP_CONTROL_ACH);
			PlaySoundEffect(0);
		}
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
		int maxlen=g_iClientWidth*11/800;
		if(m_strDialString.Size()<=maxlen)
		{
			m_tagDialString.SetCaption(m_strDialString.String());
		}
		else
		{
			m_tagDialString.SetCaption(m_strDialString.String()+m_strDialString.Size()-maxlen);
		}
	}

	g_BTModule.pApi->apiHF.SendDTMF(c);

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
/*
void CPanelBluetoothTalking::OnPhoneCallState(WPARAM wParam, LPARAM lParam)
{
	if(wParam)//active
	{

	}
	else
	{
		SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),m_iPreviousPanelParam,FALSE);
	}

	//

	m_dwCallStatus=lParam;

	int margin_x=0;
	int cx=(g_iClientWidth-margin_x*2)/4;

	m_btnAnswer.Show(m_dwCallStatus == HF_STATE_INCOMING_CALL);
	m_btnACH.Show(m_dwCallStatus != HF_STATE_INCOMING_CALL);
	m_btnKeyboard.Show(m_dwCallStatus != HF_STATE_INCOMING_CALL);
	m_btnHang.Show(TRUE);

	if(m_dwCallStatus == HF_STATE_INCOMING_CALL)
	{
		m_btnAnswer.SetPos(-cx,-g_iClientHeight/2+60);
		m_btnHang.SetPos(cx,-g_iClientHeight/2+60);
	}
	else
	{
		m_btnHang.SetPos(0,-g_iClientHeight/2+60);
	}
}
*/

void CPanelBluetoothTalking::SelectCallIndex(DWORD dwIndex)
{
	RETAILMSG(1/*DEBUG_BT*/,(_T("SelectCallIndex dwIndex:%d\r\n"),dwIndex));

	if(dwIndex>1)
		return;

	m_bEnableAutoSelCall = FALSE;

	m_dwCurCallIndex=dwIndex;

	m_btnCall1.SetStatus(dwIndex==0?BS_FOCUS:BS_UP);
	m_btnCall2.SetStatus(dwIndex==1?BS_FOCUS:BS_UP);

	m_btnAnswer1.EnableNotify(dwIndex == 0);
	m_btnHang1.EnableNotify(dwIndex == 0);

	m_btnAnswer2.EnableNotify(dwIndex == 1);
	m_btnHang2.EnableNotify(dwIndex == 1);

	//
	const CallInfoT *pci=(dwIndex == 0)?m_ciCallInfo1:m_ciCallInfo2;
	CButtonGL *pbtnAnswer = ((dwIndex==0) ? &m_btnAnswer1 : &m_btnAnswer2);
	CButtonGL *pbtnHangup = ((dwIndex==0) ? &m_btnHang1 : &m_btnHang2);

	CM_String *pstrCallStatus = ((dwIndex==0) ? &m_strCallStatus1 : &m_strCallStatus2);
	CM_String *pstrName = ((dwIndex==0) ? &m_strName1 : &m_strName2);
	CM_String *pstrNumber = ((dwIndex==0) ? &m_strNumber1 : &m_strNumber2);

	CM_String *pstrArea = ((dwIndex==0) ? &m_strMobileArea1 : &m_strMobileArea2);
	CM_String *pstrType = ((dwIndex==0) ? &m_strMobileType1 : &m_strMobileType2);

	BOOL *pbTalking = ((dwIndex==0) ? &m_bTalking1 : &m_bTalking2);
	DWORD *pdwCallTime = ((dwIndex==0) ? &m_dwCallTime1 : &m_dwCallTime2);

	*pbTalking=FALSE;

	if(pci)
	{
		//m_btnACH.Enable(pci->status==HFM_CALL_ACTIVE || pci->status==HFM_CALL_HELD);
		//m_btnACH.Show(pci->status!=HFM_INCOMING_CALL && pci->status!=HFM_CALL_WAITING);

		m_btnKeyboard.Enable(pci->status==HFM_CALL_ACTIVE || pci->status==HFM_CALL_HELD);
		m_btnKeyboard.Show(pci->status!=HFM_INCOMING_CALL && pci->status!=HFM_CALL_WAITING);

		pbtnAnswer->Show(pci->status==HFM_INCOMING_CALL || pci->status==HFM_CALL_WAITING);
		pbtnHangup->Show(TRUE);

// 		if(pci->status==HFM_INCOMING_CALL || pci->status==HFM_CALL_WAITING)
// 		{
// 			int margin_x=0;
// 			int cx=(g_iClientWidth-margin_x*2)/4;
// 			pbtnAnswer->SetPos(-cx,-g_iClientHeight/2+60);
// 			pbtnHangup->SetPos(cx,-g_iClientHeight/2+60);
// 		}
// 		else
// 		{
// 			pbtnHangup->SetPos(0,-g_iClientHeight/2+60);
// 		}

		//status:
		if(pci->status==HFM_CALL_ACTIVE)
			*pstrCallStatus = GetResourceString(IDS_CS_CONNECTED);
		else if(pci->status==HFM_CALL_HELD)
			*pstrCallStatus = GetResourceString(IDS_CS_HOLD);
		else if(pci->status==HFM_OUTGOING_CALL || pci->status==HFM_OUTGOING_CALL_ALERT)
			*pstrCallStatus = GetResourceString(IDS_CS_DIALING);
		else if(pci->status==HFM_INCOMING_CALL)
			*pstrCallStatus = GetResourceString(IDS_CS_INCOMING_CALL);
		else if(pci->status==HFM_CALL_WAITING)
			*pstrCallStatus = GetResourceString(IDS_CS_WAITING_CALL);

		//name and number
		*pstrName = pci->name;
		*pstrNumber = pci->phoneNumbers;

		if(m_MobileData.Query(pci->phoneNumbers))
		{
			*pstrArea = m_MobileData.GetArea();
			*pstrType = m_MobileData.GetType();
		}
		else
		{
			*pstrArea = NULL;
			*pstrType = NULL;
		}

		//
		*pbTalking=(pci->status==HFM_CALL_ACTIVE);

// 		//call time
// 		if(pci->status==HFM_CALL_ACTIVE)
// 			*pdwCallTime=GetTickCount();
	}
	else
	{

	}
}
void CPanelBluetoothTalking::OnHFStateChange(SnmConnectionState status)
{
	if(SNM_CONNECTION_STATE_IDLE == status)
	{
		if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
		{
			SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,0/*m_iPreviousPanelParam*/);
		}
	}

}
void CPanelBluetoothTalking::UpdateCallInfo(const CallInfoT *callInfo,DWORD dwIndex,DWORD dwTotalCalls)
{
	if (callInfo)
	{
		BOOL bValid=FALSE;

		if(callInfo->phoneNumbers && wcslen(callInfo->phoneNumbers)>0)
		{
			bValid = TRUE;
		}

		if(bValid)
		{
			//update name:
			if(callInfo->name && callInfo->name[0])
			{
				//has valid name
				
			}
			else
			{
				RETAILMSG(DEBUG_BT,(_T("<%s>::need serach phonebook for name %s\r\n"),_T(__FUNCTION__),callInfo->phoneNumbers));

				Contact** contactList = NULL;
				DWORD contactCount = 0;
				BOOL bSearched=FALSE;
				DWORD dwRet;

				dwRet = g_BTModule.pApi->apiPB.SearchContact(PACM_SEARCH_ATTRIBUTE_NUMBER,(wchar_t*)callInfo->phoneNumbers,contactList,contactCount);

				if (dwRet == ERROR_SUCCESS)
				{
					if (contactList && contactCount)
					{
						//search at least one item:
						bSearched = TRUE;
					}
				}

				wcscpy_s((wchar_t*)callInfo->name,COUNTOF(callInfo->name), bSearched?contactList[0]->GetKey():GetResourceString(IDS_STRANGER));
			}
		}
		else
		{
			//
			wcscpy_s((wchar_t*)callInfo->name,COUNTOF(callInfo->name), GetResourceString(IDS_BT_STREAMING_AUDIO));
			wcscpy_s((wchar_t*)callInfo->phoneNumbers,COUNTOF(callInfo->phoneNumbers), GetResourceString(IDS_BT_STREAMING_AUDIO_TERMINATE));
		}

		//
		if(dwIndex == 0)
		{
			m_ciCallInfo1=callInfo;

			//call time
			if(m_ciCallInfo1->status==HFM_CALL_ACTIVE && m_dwCallTime1==0)
			{
				m_dwCallTime1=GetTickCount();
			}
		}
		if(dwIndex == 1)
		{
			m_ciCallInfo2=callInfo;

			//call time
			if(m_ciCallInfo2->status==HFM_CALL_ACTIVE && m_dwCallTime2==0)
			{
				m_dwCallTime2=GetTickCount();
			}
		}

		m_dwCallCount = dwTotalCalls;
		m_btnCall1.Show(dwTotalCalls>1);
		m_btnCall2.Show(dwTotalCalls>1);

		//update title:
		CM_String *pstrCallStatus = ((dwIndex==0) ? &m_strCallStatus1 : &m_strCallStatus2);
		if(callInfo->status==HFM_CALL_ACTIVE)
			*pstrCallStatus = GetResourceString(IDS_CS_CONNECTED);
		else if(callInfo->status==HFM_CALL_HELD)
			*pstrCallStatus = GetResourceString(IDS_CS_HOLD);
		else if(callInfo->status==HFM_OUTGOING_CALL || callInfo->status==HFM_OUTGOING_CALL_ALERT)
			*pstrCallStatus = GetResourceString(IDS_CS_DIALING);
		else if(callInfo->status==HFM_INCOMING_CALL)
			*pstrCallStatus = GetResourceString(IDS_CS_INCOMING_CALL);
		else if(callInfo->status==HFM_CALL_WAITING)
			*pstrCallStatus = GetResourceString(IDS_CS_WAITING_CALL);

		//auto select call index:
		if(dwTotalCalls > 1 )
		{
			if(m_bEnableAutoSelCall)
			{
				if(dwIndex==1 && callInfo->status==HFM_CALL_WAITING)//second waiting call
				{
					SelectCallIndex(dwIndex);
				}	
				else if(callInfo->status==HFM_INCOMING_CALL)//incoming call
				{
					SelectCallIndex(dwIndex);
				}
				else
				{
					SelectCallIndex(m_dwCurCallIndex);
				}
			}
			else
			{
				SelectCallIndex(m_dwCurCallIndex);
			}
		}
		else if(dwTotalCalls == 1 )
		{
			//if(m_dwCurCallIndex != 0)
			{
				SelectCallIndex(0);
			}
			m_bEnableAutoSelCall = TRUE;
		}	
	}
}

void CPanelBluetoothTalking::QuitPanel()
{
	//m_bEnableQuit = TRUE;
	m_bNeedAutoShowSmartBar = FALSE;
	if(m_pSkinManager->GetConfig()->GetCurParam()->idCurUI == MAKE_PANEL_ID(UI_CLASS_BT,PAGE_BT_TALKING))
	{
		//SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE,m_iPreviousPanelParam);
	}
}
void CPanelBluetoothTalking::ClearCallInfo()
{
	//m_bEnableQuit = TRUE;

	m_dwCurCallIndex = 0;//-1;
	m_dwCallCount = 0;

	m_bTalking1=FALSE;
	m_bTalking2=FALSE;

	m_dwCallTime1=0;
	m_dwCallTime2=0;

	m_strCallStatus1 = _T("");
	m_strName1 = _T("");
	m_strNumber1 = _T("");
	m_strCallStatus2 = _T("");
	m_strName2 = _T("");
	m_strNumber2 = _T("");

	/*
	m_btnAnswer1.Show(FALSE);
	m_btnHang1.Show(FALSE);

	m_btnAnswer2.Show(FALSE);
	m_btnHang2.Show(FALSE);

	m_btnACH.Show(FALSE);
	m_btnKeyboard.Show(FALSE);
	*/

	m_btnAnswer1.Show(FALSE);

	//m_btnHang1.SetPos(0,-g_iClientHeight/2+60);
	m_btnHang1.Show(TRUE);

	m_btnAnswer2.Show(FALSE);
	m_btnHang2.Show(FALSE);

	//m_btnACH.Enable(FALSE);
	m_btnKeyboard.Enable(FALSE);
	m_btnACH.Show(TRUE);
	m_btnKeyboard.Show(TRUE);


	//
	m_btnCall1.Show(FALSE);
	m_btnCall2.Show(FALSE);

	m_bEnableAutoSelCall = TRUE;
}


