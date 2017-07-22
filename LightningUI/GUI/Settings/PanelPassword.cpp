#include "PanelPassword.h"
#include "../../resource.h"

CPanelPassword * g_pCPanelPassword = NULL;

const int caption_h = 30;
const int caption2_h = 30;
const int box_size_x=LIST_BOX_ITEM_WIDTH;
const int margin_x=12;
const int tag_h = LIST_BOX_TAG_HEIGHT;
const int cx=g_iClientWidth-box_size_x-margin_x*2;

CPanelPassword::CPanelPassword(void)
{
	m_idCaller=0;
	m_pCallbackFunc=NULL;
	m_lpCallbackParam=NULL;
	m_type=0;

	m_dwLastNotificationTick=0;
	m_dwNotificationShowTime=5000;
	m_idNotificationIcon=0;
	m_bShowing=FALSE;

	m_bAutoHide=FALSE;
	m_bUnLock = FALSE;
	m_strPassword = _T("0000");
// 	m_idLastUIClass=UI_MAINMENU;
}

CPanelPassword::~CPanelPassword(void)
{
}

BOOL CPanelPassword::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelPassword::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelPassword::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(0.1f);
	SetAlpha(1.0f,TRUE);


	m_dwLastNotificationTick=GetTickCount();

	if(m_bUnLock)
	{
		if(m_pCallbackFunc)
		{
			m_pCallbackFunc(m_idCaller,NB_OK,0,NULL,m_lpCallbackParam);
		}
	}
}

void CPanelPassword::Render()
{
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;

	if(!BeginRender())return;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();

	if(!m_bUnLock)
	{
		COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};
		COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
		COLORGL cr_gray2={0.35,0.35,0.35,1.0};
		COLORGL cr_dark={0.0,0.0,0.0,1.0};

		DrawRect(0,g_iClientHeight/2-80, g_iClientWidth-LIST_BOX_ITEM_WIDTH - 10, 1, &cr_gray);

		//device status
		if(m_pTextGL)
		{
			if(m_strTitle.String())
			{
				m_pTextGL->SetString(m_strTitle.String());
				m_pTextGL->SetColor(&cr_gray);
				m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-6);
				m_pTextGL->SetAlpha(m_fCurAlpha);
				m_pTextGL->SetLuminance(m_fCurLuminance);
				m_pTextGL->Render();
			}

			if(m_strNotification.String())
			{
				m_pTextGL->SetString(m_strNotification.String());
				m_pTextGL->SetCharScale(0.4f,0.4f);
				m_pTextGL->SetColor(&cr_gray);
				m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-36-8);
				m_pTextGL->SetAlpha(m_fCurAlpha);
				m_pTextGL->SetLuminance(m_fCurLuminance);
				m_pTextGL->Render();
			}
		}

		m_btnReturn.SetAlpha(m_fCurAlpha);
		m_btnReturn.Render();

		m_tagPasswordTitle.SetAlpha(m_fCurAlpha);
		m_tagPasswordTitle.Render();

		for (DWORD i = 0; i<_countof(m_tagNumber); i++)
		{
			m_tagNumber[i].SetAlpha(m_fCurAlpha);
			m_tagNumber[i].Render();
		}

		for (DWORD i = 0; i<_countof(m_btnNumber); i++)
		{
			m_btnNumber[i].SetAlpha(m_fCurAlpha);
			m_btnNumber[i].Render();
		}

	}
#else
	if(!m_bUnLock)
	{
		COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};
		COLORGL cr_gray={0.25,0.25,0.25,1.0};
		COLORGL cr_gray2={0.35,0.35,0.35,1.0};
		COLORGL cr_dark={0.0,0.0,0.0,1.0};

		//		m_tagBkBlur.Render();
		//		m_tagBkBlur.SetAlpha(m_fCurAlpha);
		//

		DrawRect(0,g_iClientHeight/2-70,g_iClientWidth-LIST_BOX_ITEM_WIDTH-50,1,&cr_gray);

		//device status

		if(m_pTextGL)
		{


			if(m_strTitle.String())
			{
				m_pTextGL->SetString(m_strTitle.String());
				m_pTextGL->SetCharScale(0.8f,0.8f);
				m_pTextGL->SetColor(&cr_dark);
				//m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-6);
				//m_pTextGL->SetCharScale(1.0f,1.0f);
				//m_pTextGL->GetExtension(&si);
				//m_pTextGL->SetSize(g_iClientWidth/2, caption_h);
				//m_pTextGL->SetPos(0,g_iClientHeight/2-25);
				m_pTextGL->SetAlpha(m_fCurAlpha);
				m_pTextGL->SetLuminance(m_fCurLuminance);
				m_pTextGL->Render();
			}

			if(m_strNotification.String())
			{
				m_pTextGL->SetString(m_strNotification.String());
				m_pTextGL->SetCharScale(0.4f,0.4f);
				m_pTextGL->SetColor(&cr_gray2);
				//m_pTextGL->GetExtension(&si);
				m_pTextGL->SetPos(-cx/2+10,g_iClientHeight/2-36-8);
				//m_pTextGL->SetCharScale(0.8f,0.8f);
				//m_pTextGL->GetExtension(&si);
				//m_pTextGL->SetSize(g_iClientWidth/2, caption2_h);
				//m_pTextGL->SetPos(0/*-cx/2*/,g_iClientHeight/2-50);
				m_pTextGL->SetAlpha(m_fCurAlpha);
				m_pTextGL->SetLuminance(m_fCurLuminance);
				m_pTextGL->Render();
			}
		}

		m_btnReturn.SetAlpha(m_fCurAlpha);
		m_btnReturn.Render();

		m_tagPasswordTitle.SetAlpha(m_fCurAlpha);
		m_tagPasswordTitle.Render();

		for (DWORD i = 0; i<_countof(m_tagNumber); i++)
		{
			m_tagNumber[i].SetAlpha(m_fCurAlpha);
			m_tagNumber[i].Render();
		}

		for (DWORD i = 0; i<_countof(m_btnNumber); i++)
		{
			m_btnNumber[i].SetAlpha(m_fCurAlpha);
			m_btnNumber[i].Render();
		}

	}
#endif

	EndRender();

}
BOOL CPanelPassword::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	COLORGL cr_gray={190.0f/256.0f,190.0f/256.0f,190.0f/256.0f,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_b={0.0,161.0f/256.0f,228.0/256.0f,1.0};
	COLORGL cr_dark={0.0,0.0,0.0,1.0};

	m_tagBk.Initialize(0,this,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		m_pSkinManager->GetTexture(TEXID_065),NULL,
		NULL,NULL,
		NULL,NULL);
	m_tagBk.SetSize(g_iClientWidth, g_iClientHeight);
	m_tagBk.SetPos(LIST_BOX_ITEM_WIDTH/2, 0);

	g_pCPanelPassword= this;

	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);
	EnableScrollingY(FALSE);

	//Return
	m_btnReturn.Initialize(IDC_SETTING_PASSWORD_RETURN,this,
		m_pSkinManager->GetTexture(TEXID_012),NULL,
		m_pSkinManager->GetTexture(TEXID_012),NULL,NULL,NULL,NULL,NULL);
	m_btnReturn.SetShowScale(0.9f);
	m_btnReturn.SetTouchRate(2.0f, 1.5f);
	m_btnReturn.SetPos(cx/2-40,g_iClientHeight/2-40);
	m_btnReturn.SetNotifyFunc(&CPanelPassword::OnNotify,this);

	//Password
	m_tagPasswordTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPasswordTitle.SetSize(cx,60);
	m_tagPasswordTitle.SetCaption(GetResourceString(IDS_SETTING_ENTER_PASSWORD));
	m_tagPasswordTitle.LinkTextGL(pTextGL);
	m_tagPasswordTitle.SetPos(0,100);
	m_tagPasswordTitle.SetCaptionHeight(caption_h);
	m_tagPasswordTitle.EnableDrawBack(FALSE);
	m_tagPasswordTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagPasswordTitle.SetCaptionColor(&cr_gray);

	wchar_t string[4];
	for (DWORD i = 0; i<_countof(m_tagNumber); i++)
	{
		m_tagNumber[i].Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
			NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagNumber[i].SetSize(70,70);
		m_tagNumber[i].SetCaption(_T(""));
		m_tagNumber[i].SetCaptionColor(&cr_dark, &cr_dark);
		m_tagNumber[i].SetCaptionHeight(caption2_h);
		m_tagNumber[i].SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagNumber[i].LinkTextGL(pTextGL);
	}
	m_tagNumber[0].SetPos(-200+80*1,10);
	m_tagNumber[1].SetPos(-200+80*2,10);
	m_tagNumber[2].SetPos(-200+80*3,10);
	m_tagNumber[3].SetPos(-200+80*4,10);

	for (DWORD i = 0; i<_countof(m_btnNumber); i++)
	{
		m_btnNumber[i].Initialize(IDC_SETTING_PASSWORD_NUMBER_0+i,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
			m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
		swprintf_s(string, _countof(string), L"%u", i);
		m_btnNumber[i].SetCaption(string);
		m_btnNumber[i].SetCaptionPos(BTN_CAPTION_CENTER);
		m_btnNumber[i].SetCaptionColor(&cr_dark,&cr_dark);
		m_btnNumber[i].SetCaptionHeight(caption_h);
		m_btnNumber[i].LinkTextGL(pTextGL);
		m_btnNumber[i].SetSize(80,80);
		m_btnNumber[i].SetNotifyFunc(&CPanelPassword::OnNotify,this);
	}

	int btnnumx = -270;//-cx/2+40;
	int btnnumy = -80;
	int xOffset = 90;
	int yOffset = 90;
	m_btnNumber[1].SetPos(btnnumx+xOffset*1, btnnumy);
	m_btnNumber[2].SetPos(btnnumx+xOffset*2, btnnumy);
	m_btnNumber[3].SetPos(btnnumx+xOffset*3, btnnumy);
	m_btnNumber[4].SetPos(btnnumx+xOffset*4, btnnumy);
	m_btnNumber[5].SetPos(btnnumx+xOffset*5, btnnumy);
	m_btnNumber[6].SetPos(btnnumx+xOffset*1, btnnumy-yOffset);
	m_btnNumber[7].SetPos(btnnumx+xOffset*2, btnnumy-yOffset);
	m_btnNumber[8].SetPos(btnnumx+xOffset*3, btnnumy-yOffset);
	m_btnNumber[9].SetPos(btnnumx+xOffset*4, btnnumy-yOffset);
	m_btnNumber[0].SetPos(btnnumx+xOffset*5, btnnumy-yOffset);

#else
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_b={0.0,161.0f/256.0f,228.0/256.0f,1.0};
	COLORGL cr_dark={0.0,0.0,0.0,1.0};

	g_pCPanelPassword= this;

	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);
	EnableScrollingY(FALSE);

	//Return
	m_btnReturn.Initialize(IDC_SETTING_PASSWORD_RETURN,this,m_pSkinManager->GetTexture(TEXID_095),NULL,
		m_pSkinManager->GetTexture(TEXID_012),NULL,NULL,NULL,NULL,NULL);
	m_btnReturn.SetShowScale(0.9f);
	m_btnReturn.SetTouchRate(2.0f, 1.5f);
	m_btnReturn.SetPos(cx/2-40,g_iClientHeight/2-40);
	m_btnReturn.SetNotifyFunc(&CPanelPassword::OnNotify,this);

	//Password
	m_tagPasswordTitle.Initialize(0,this,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagPasswordTitle.SetSize(cx,60);
	m_tagPasswordTitle.SetCaption(GetResourceString(IDS_SETTING_ENTER_PASSWORD));
	m_tagPasswordTitle.LinkTextGL(pTextGL);
	m_tagPasswordTitle.SetPos(0,100);
	m_tagPasswordTitle.SetCaptionHeight(caption_h);
	m_tagPasswordTitle.EnableDrawBack(FALSE);
	m_tagPasswordTitle.SetCaptionPos(BTN_CAPTION_CENTER);
	m_tagPasswordTitle.SetCaptionColor(&cr_dark);

	wchar_t string[4];
	for (DWORD i = 0; i<_countof(m_tagNumber); i++)
	{
		m_tagNumber[i].Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,
			NULL,NULL,NULL,NULL,NULL,NULL);
		m_tagNumber[i].SetSize(70,70);
		m_tagNumber[i].SetCaption(_T(""));
		m_tagNumber[i].SetCaptionColor(&cr_gray);
		m_tagNumber[i].SetCaptionHeight(caption2_h);
		m_tagNumber[i].SetCaptionPos(BTN_CAPTION_CENTER);
		m_tagNumber[i].LinkTextGL(pTextGL);
	}
	m_tagNumber[0].SetPos(-200+80*1,10);
	m_tagNumber[1].SetPos(-200+80*2,10);
	m_tagNumber[2].SetPos(-200+80*3,10);
	m_tagNumber[3].SetPos(-200+80*4,10);

	for (DWORD i = 0; i<_countof(m_btnNumber); i++)
	{
		m_btnNumber[i].Initialize(IDC_SETTING_PASSWORD_NUMBER_0+i,this,m_pSkinManager->GetTexture(TEXID_609),NULL,
			m_pSkinManager->GetTexture(TEXID_608),NULL,NULL,NULL,NULL,NULL);
		swprintf_s(string, _countof(string), L"%u", i);
		m_btnNumber[i].SetCaption(string);
		m_btnNumber[i].SetCaptionPos(BTN_CAPTION_CENTER);
		m_btnNumber[i].SetCaptionColor(&cr_gray,&cr_b);
		m_btnNumber[i].SetCaptionHeight(caption_h);
		m_btnNumber[i].LinkTextGL(pTextGL);
		m_btnNumber[i].SetSize(80,80);
		m_btnNumber[i].SetNotifyFunc(&CPanelPassword::OnNotify,this);
	}

	int btnnumx = -270;//-cx/2+40;
	int btnnumy = -80;
	int xOffset = 90;
	int yOffset = 90;
	m_btnNumber[1].SetPos(btnnumx+xOffset*1, btnnumy);
	m_btnNumber[2].SetPos(btnnumx+xOffset*2, btnnumy);
	m_btnNumber[3].SetPos(btnnumx+xOffset*3, btnnumy);
	m_btnNumber[4].SetPos(btnnumx+xOffset*4, btnnumy);
	m_btnNumber[5].SetPos(btnnumx+xOffset*5, btnnumy);
	m_btnNumber[6].SetPos(btnnumx+xOffset*1, btnnumy-yOffset);
	m_btnNumber[7].SetPos(btnnumx+xOffset*2, btnnumy-yOffset);
	m_btnNumber[8].SetPos(btnnumx+xOffset*3, btnnumy-yOffset);
	m_btnNumber[9].SetPos(btnnumx+xOffset*4, btnnumy-yOffset);
	m_btnNumber[0].SetPos(btnnumx+xOffset*5, btnnumy-yOffset);

#endif //CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET

	return TRUE;
}


void CPanelPassword::OnTouchDown(POINT *pt)
{
	CPanelGL::OnTouchDown(pt);
}
void CPanelPassword::OnTouchUp(POINT *pt)
{
	CPanelGL::OnTouchUp(pt);
}
void CPanelPassword::OnTouchMove(POINT *pt)
{

}
void CPanelPassword::Hide()
{
	m_bShowing=FALSE;
	//quit
// 	::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,m_idLastPanel,NULL);
// 	::PostMessage(m_hMainWnd,WM_PANEL_CHANGE,m_idLastPanel,m_iLastPanelParam);
	OnBack();
}

void CALLBACK CPanelPassword::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelPassword *panel=(CPanelPassword*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelPassword::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_SETTING_PASSWORD_RETURN://cancel
		OnBack();
		PlaySoundEffect(0);
		break;
	case IDC_SETTING_PASSWORD_NUMBER_0:
		Input('0');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_1:
		Input('1');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_2:
		Input('2');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_3:
		Input('3');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_4:
		Input('4');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_5:
		Input('5');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_6:
		Input('6');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_7:
		Input('7');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_8:
		Input('8');
		break;
	case IDC_SETTING_PASSWORD_NUMBER_9:
		Input('9');
		break;
	default:
		break;
	}
}
void CPanelPassword::SetNotification(NBMSG *pMsg)
{
	//RETAILMSG(1, (TEXT("MSG:[SetNotification] \r\n")));
	if(!pMsg)
		return;

	m_idCaller=pMsg->idCaller;
	m_pCallbackFunc=pMsg->pCallbackFunc;
	m_lpCallbackParam=pMsg->lpCallbackParam;
	m_type=pMsg->type;

	m_dwNotificationShowTime=pMsg->dwShowTime;
	m_idNotificationIcon=pMsg->idIcon;

	m_strNotification = pMsg->pstrNotification;
	m_strTitle=pMsg->pstrTitle;
//		m_strNotification = _T("Advance Setting");
//		m_strTitle=_T("Advance");

	m_bAutoHide=pMsg->bAutoHide;

	m_dwLastNotificationTick=GetTickCount();
	m_bShowing = TRUE;
//
//		RETAILMSG(1, (TEXT("m_idCaller:[%d]\r\n"),m_idCaller));
//		RETAILMSG(1, (TEXT("m_pCallbackFunc:[%X]\r\n"),m_pCallbackFunc));
//		RETAILMSG(1, (TEXT("m_lpCallbackParam:[%d]\r\n"),m_lpCallbackParam));
//		RETAILMSG(1, (TEXT("m_type:[%d]\r\n"),m_type));
//		RETAILMSG(1, (TEXT("m_strNotification:[%S]\r\n"),m_strNotification.String()));
//		RETAILMSG(1, (TEXT("m_strTitle:[%S]\r\n"),m_strTitle.String()));

}

void CPanelPassword::SetCmd(WPARAM wParam, LPARAM lParam)
{
	UINT nCmd = (UINT)wParam;
	switch (nCmd)
	{
	case 1: //Lock
		LockPasswordBox(TRUE);
		break;
	case 2:	//UnLock
		LockPasswordBox(FALSE);
		break;
	case 3:
		SetPassword((LPCTSTR)lParam);
		break;
	}
}

void CPanelPassword::OnBack()
{
	m_bShowing = FALSE;
	//SwitchPage(GET_PANEL_CLASS_ID(m_idPreviousPanel),GET_PANEL_PAGE_ID(m_idPreviousPanel),PAGE_SWITCH_ANI_NONE);
	ClearInput(-1);

	SwitchPage(UI_CLASS_SETTING,PAGE_SETTINGS_WRAPPER,PAGE_SWITCH_ANI_NONE,PAGE_SETTINGS_GENERAL);
}

void CPanelPassword::Input(wchar_t c)
{
	wchar_t str[8];
	swprintf_s(str,8,_T("%c"),c);

	m_strInput += str;

	RETAILMSG(1, (TEXT("MSG:[%s]: [%s]\r\n"), TEXT(__FUNCTION__),m_strInput.String()));

	if(m_strInput.Size())
	{
		if(m_strInput.Size()<=PASSWORD_MAX_LENGTH)
		{
			for(int i=0;i<m_strInput.Size();i++)
			{
				if(!m_btagNumberInput[i])
				{
					//m_tagNumber[i].SetCaption(_T("*"));
					m_tagNumber[i].SetCaption(str);
					m_btagNumberInput[i] = TRUE;
				}
			}

			if(m_strInput.Size()==PASSWORD_MAX_LENGTH)
			{
				if(!VerifyPassword(m_strInput.String()))
				{
					SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_PASSWORD_CLEAR,400,OnTimerProc);
				}
				else
				{
					m_bUnLock = TRUE;
					SetTimer(m_hMainWnd,TIMER_SETTING_ADVANCE_PASSWORD_VERIFY,400,OnTimerProc);

					RETAILMSG(1, (TEXT("MSG:[%s]: Verify successfully.\r\n"), TEXT(__FUNCTION__)));
				}
			}
		}
		else
		{
			//m_tagDialString.SetCaption(m_strInput.String()+m_strInput.Size()-maxlen);
		}
	}
}
void CPanelPassword::ClearInput(int iNumChar)
{
	if(m_strInput.Size()>0)
	{
		if(iNumChar<=0)
		{
			m_strInput = NULL;
			for(int i=0;i<PASSWORD_MAX_LENGTH;i++)
			{
				m_btagNumberInput[i] = FALSE;
				m_tagNumber[i].SetCaption(_T(""));
			}
		}
		else
		{
			int iClearNum = min(iNumChar,PASSWORD_MAX_LENGTH);
			m_strInput -= iClearNum;
			for(int i=1;i<=iClearNum;i++)
			{
				m_btagNumberInput[PASSWORD_MAX_LENGTH-i] = FALSE;
				m_tagNumber[PASSWORD_MAX_LENGTH-i].SetCaption(_T(""));
			}
		}
	}
}


BOOL CPanelPassword::VerifyPassword(LPCTSTR lpPassword)
{
	BOOL bRet = FALSE;
	if (lpPassword == NULL)
	{
		return bRet;
	}
    if (wcslen(lpPassword) < PASSWORD_MAX_LENGTH)
    {
        return bRet;
    }
	if(lstrcmpi(/*_T("0521")*/m_strPassword.String(), lpPassword) == 0)
	{
		m_bVerifyRight = TRUE;
		bRet = TRUE;
	}

	return bRet;
}

void CPanelPassword::SetPassword(LPCTSTR lpPassword)
{
	m_strPassword = lpPassword;
}

void CPanelPassword::OnTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	switch(idEvent)
	{
	case TIMER_SETTING_ADVANCE_PASSWORD_CLEAR:
		{
			g_pCPanelPassword->ClearInput(-1);
			KillTimer(g_pCPanelPassword->m_hMainWnd,TIMER_SETTING_ADVANCE_PASSWORD_CLEAR);
		}
		break;
	case TIMER_SETTING_ADVANCE_PASSWORD_VERIFY:
		{
			if(g_pCPanelPassword->m_pCallbackFunc)
			{
				g_pCPanelPassword->m_pCallbackFunc(g_pCPanelPassword->m_idCaller,NB_OK,0,NULL,g_pCPanelPassword->m_lpCallbackParam);
			}
			else
			{
				RETAILMSG(1, (TEXT("ERR:m_pCallbackFunc is NULL!!!!\r\n")));
			}
			g_pCPanelPassword->ClearInput(-1);
			KillTimer(g_pCPanelPassword->m_hMainWnd,TIMER_SETTING_ADVANCE_PASSWORD_VERIFY);
		}
		break;
	default:
		break;
	}
}

BOOL CPanelPassword::IsActivate()
{
	return m_bUnLock;
}

void CPanelPassword::LockPasswordBox(BOOL bLock)
{
	m_bUnLock = !bLock;
}