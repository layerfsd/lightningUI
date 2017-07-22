#include "PanelSetMAC.h"
#include "../../CarPlay/CarplayAdapter.h"
#include "../../resource.h"

#if CVTE_EN_CPMAC
CPanelSetMAC::CPanelSetMAC(void)
{

}

CPanelSetMAC::~CPanelSetMAC(void)
{
}

BOOL CPanelSetMAC::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSetMAC::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	SetAlpha(0.1f);
	SetAlpha(1.0f,TRUE);

	RETAILMSG(1, (TEXT("MSG:[%s] \r\n"),TEXT(__FUNCTION__)));

}

void CPanelSetMAC::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
// 	SetAlpha(1.0f);
	SetAlpha(0.0f,FALSE);


}
void CPanelSetMAC::Render()
{
	static DWORD s_dwlastUpdateTimeTick=0;
	static SYSTEMTIME sys_time;
	static COLORGL cr_w={1,1,1,1};
	static COLORGL cr_gray={0.7,0.7,0.7,1};
	static COLORGL cr_b={0.38,0.74,0.89,1};

	if(!BeginRender())return;

	//DrawRect(0,0,800,480,&crb);
	
	if(m_pTextGL)
	{
		SIZE si;

		m_pTextGL->SetCharScale(1.0f,1.0f);//font height point=2.0*32=64
		m_pTextGL->SetColor(&cr_w);
		m_pTextGL->SetAlpha(m_fCurAlpha);
		m_pTextGL->SetLuminance(m_fCurLuminance);

		//static:
		m_pTextGL->SetString(_T("Please Set MAC Address"));
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(GLfloat(-si.cx/2),200);
		m_pTextGL->SetColor(&cr_b);
		m_pTextGL->Render();

		//static:
		m_pTextGL->SetString(m_macstr);
		m_pTextGL->GetExtension(&si);
		m_pTextGL->SetPos(GLfloat(-si.cx/2),100);
		m_pTextGL->SetColor(&cr_b);
		m_pTextGL->Render();

	}

	m_btnNum0.SetAlpha(m_fCurAlpha);
	m_btnNum0.Render();

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

	m_btnNumA.SetAlpha(m_fCurAlpha);
	m_btnNumA.Render();
	m_btnNumB.SetAlpha(m_fCurAlpha);
	m_btnNumB.Render();
	m_btnNumC.SetAlpha(m_fCurAlpha);
	m_btnNumC.Render();

	m_btnNumD.SetAlpha(m_fCurAlpha);
	m_btnNumD.Render();
	m_btnNumE.SetAlpha(m_fCurAlpha);
	m_btnNumE.Render();
	m_btnNumF.SetAlpha(m_fCurAlpha);
	m_btnNumF.Render();

	m_btnDel.SetAlpha(m_fCurAlpha);
	m_btnDel.Render();
	m_btnOK.SetAlpha(m_fCurAlpha);
	m_btnOK.Render();

	EndRender();

}

BOOL CPanelSetMAC::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	//keyboard:
	COLORGL cr_key={1.0,1.0,1.0,1.0};
	COLORGL cr_key_down={0.0,0.0,0.0,1.0};

	CTexture *pUp=pSkinManager->GetTexture(TEXID_521);
	CTexture *pDown=pSkinManager->GetTexture(TEXID_522);

	int cx_key=(g_iClientWidth-35)/9;
	int cy_key=55;

	SIZE si_key={cx_key-4,cy_key-4};
	GLfloat px=0;
	GLfloat py=-50;

	m_btnNum0.Initialize(IDC_SETTING_BTN_KEY_0,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum0.SetCaption(L"0");
	m_btnNum0.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum0.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum0.SetCaptionHeight(53);
	m_btnNum0.LinkTextGL(pTextGL);
	m_btnNum0.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum0.SetPos(px+cx_key*-4,py);
	m_btnNum0.SetShowScale(0.64f);

	m_btnNum1.Initialize(IDC_SETTING_BTN_KEY_1,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum1.SetCaption(L"1");
	m_btnNum1.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum1.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum1.SetCaptionHeight(53);
	m_btnNum1.LinkTextGL(pTextGL);
	m_btnNum1.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum1.SetPos(px+cx_key*-3,py);
	m_btnNum1.SetShowScale(0.64f);

	m_btnNum2.Initialize(IDC_SETTING_BTN_KEY_2,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum2.SetCaption(L"2");
	m_btnNum2.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum2.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum2.SetCaptionHeight(53);
	m_btnNum2.LinkTextGL(pTextGL);
	m_btnNum2.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum2.SetPos(px+cx_key*-2,py);
	m_btnNum2.SetShowScale(0.64f);

	m_btnNum3.Initialize(IDC_SETTING_BTN_KEY_3,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum3.SetCaption(L"3");
	m_btnNum3.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum3.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum3.SetCaptionHeight(53);
	m_btnNum3.LinkTextGL(pTextGL);
	m_btnNum3.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum3.SetPos(px+cx_key*-1,py);
	m_btnNum3.SetShowScale(0.64f);

	m_btnNum4.Initialize(IDC_SETTING_BTN_KEY_4,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum4.SetCaption(L"4");
	m_btnNum4.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum4.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum4.SetCaptionHeight(53);
	m_btnNum4.LinkTextGL(pTextGL);
	m_btnNum4.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum4.SetPos(px+cx_key*0,py);
	m_btnNum4.SetShowScale(0.64f);

	m_btnNum5.Initialize(IDC_SETTING_BTN_KEY_5,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum5.SetCaption(L"5");
	m_btnNum5.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum5.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum5.SetCaptionHeight(53);
	m_btnNum5.LinkTextGL(pTextGL);
	m_btnNum5.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum5.SetPos(px+cx_key*1,py);
	m_btnNum5.SetShowScale(0.64f);

	m_btnNum6.Initialize(IDC_SETTING_BTN_KEY_6,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum6.SetCaption(L"6");
	m_btnNum6.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum6.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum6.SetCaptionHeight(53);
	m_btnNum6.LinkTextGL(pTextGL);
	m_btnNum6.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum6.SetPos(px+cx_key*2,py);
	m_btnNum6.SetShowScale(0.64f);

	m_btnNum7.Initialize(IDC_SETTING_BTN_KEY_7,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum7.SetCaption(L"7");
	m_btnNum7.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum7.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum7.SetCaptionHeight(53);
	m_btnNum7.LinkTextGL(pTextGL);
	m_btnNum7.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum7.SetPos(px+cx_key*3,py);
	m_btnNum7.SetShowScale(0.64f);

	m_btnNum8.Initialize(IDC_SETTING_BTN_KEY_8,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum8.SetCaption(L"8");
	m_btnNum8.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum8.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum8.SetCaptionHeight(53);
	m_btnNum8.LinkTextGL(pTextGL);
	m_btnNum8.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum8.SetPos(px+cx_key*4,py);
	m_btnNum8.SetShowScale(0.64f);

	m_btnNum9.Initialize(IDC_SETTING_BTN_KEY_9,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNum9.SetCaption(L"9");
	m_btnNum9.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNum9.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNum9.SetCaptionHeight(53);
	m_btnNum9.LinkTextGL(pTextGL);
	m_btnNum9.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNum9.SetPos(px+cx_key*-4,py-cy_key*2);
	m_btnNum9.SetShowScale(0.64f);

	m_btnNumA.Initialize(IDC_SETTING_BTN_KEY_A,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumA.SetCaption(L"A");
	m_btnNumA.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumA.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumA.SetCaptionHeight(53);
	m_btnNumA.LinkTextGL(pTextGL);
	m_btnNumA.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNumA.SetPos(px+cx_key*-3,py-cy_key*2);
	m_btnNumA.SetShowScale(0.64f);

	m_btnNumB.Initialize(IDC_SETTING_BTN_KEY_B,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumB.SetCaption(L"B");
	m_btnNumB.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumB.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumB.SetCaptionHeight(53);
	m_btnNumB.LinkTextGL(pTextGL);
	m_btnNumB.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNumB.SetPos(px+cx_key*-2,py-cy_key*2);
	m_btnNumB.SetShowScale(0.64f);

	m_btnNumC.Initialize(IDC_SETTING_BTN_KEY_C,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumC.SetCaption(L"C");
	m_btnNumC.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumC.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumC.SetCaptionHeight(53);
	m_btnNumC.LinkTextGL(pTextGL);
	m_btnNumC.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNumC.SetPos(px+cx_key*-1,py-cy_key*2);
	m_btnNumC.SetShowScale(0.64f);

	m_btnNumD.Initialize(IDC_SETTING_BTN_KEY_D,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumD.SetCaption(L"D");
	m_btnNumD.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumD.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumD.SetCaptionHeight(53);
	m_btnNumD.LinkTextGL(pTextGL);
	m_btnNumD.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNumD.SetPos(px+cx_key*0,py-cy_key*2);
	m_btnNumD.SetShowScale(0.64f);

	m_btnNumE.Initialize(IDC_SETTING_BTN_KEY_E,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumE.SetCaption(L"E");
	m_btnNumE.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumE.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumE.SetCaptionHeight(53);
	m_btnNumE.LinkTextGL(pTextGL);
	m_btnNumE.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNumE.SetPos(px+cx_key*1,py-cy_key*2);
	m_btnNumE.SetShowScale(0.64f);

	m_btnNumF.Initialize(IDC_SETTING_BTN_KEY_F,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnNumF.SetCaption(L"F");
	m_btnNumF.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnNumF.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnNumF.SetCaptionHeight(53);
	m_btnNumF.LinkTextGL(pTextGL);
	m_btnNumF.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnNumF.SetPos(px+cx_key*2,py-cy_key*2);
	m_btnNumF.SetShowScale(0.64f);

	m_btnDel.Initialize(IDC_SETTING_BTN_KEY_DEL,this,pUp,NULL,
	pDown,NULL,
	NULL,NULL,
	NULL,NULL);
	m_btnDel.SetCaption(L"¡û");
	m_btnDel.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnDel.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnDel.SetCaptionHeight(53);
	m_btnDel.LinkTextGL(pTextGL);
	m_btnDel.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnDel.SetPos(px+cx_key*3,py-cy_key*2);
	m_btnDel.SetShowScale(0.64f);
	m_btnDel.EnableHoldNotify(TRUE);

	m_btnOK.Initialize(IDC_SETTING_BTN_RUN_TESTS,this,pUp,NULL,
		pDown,NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnOK.SetCaption(L"OK");
	m_btnOK.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnOK.SetCaptionColor(&cr_key,&cr_key_down);
	m_btnOK.SetCaptionHeight(53);
	m_btnOK.LinkTextGL(pTextGL);
	m_btnOK.SetNotifyFunc(&CPanelSetMAC::OnNotify,this);
	m_btnOK.SetPos(px+cx_key*4,py-cy_key*2);
	m_btnOK.SetShowScale(0.64f);

	m_bShowStatusBar = FALSE;

	return TRUE;
}

void CALLBACK CPanelSetMAC::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSetMAC *panel=(CPanelSetMAC*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}

void CPanelSetMAC::AppendMACChar(wchar_t wch)
{
	if (m_macstr.GetLength() >= 12)
	{
		m_macstr.Delete(0, 1);
	}
	m_macstr += wch;
}

void CPanelSetMAC::DeleteMACChar()
{
	m_macstr.Delete(m_macstr.GetLength()-1,1);
}

void CPanelSetMAC::SetMAC()
{
	char macstr[32] = {0};
	if (m_macstr.GetLength() >= 12)
	{
		int ret = WideCharToMultiByte( CP_ACP, 0, m_macstr, -1, macstr, _countof(macstr), NULL, NULL);
		if (ret > 0)
		{
			CCarplayAdapter::GetInstance()->DeviceSetMac(macstr);
		}
	}
}

void CPanelSetMAC::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
		case IDC_SETTING_BTN_KEY_0:
			{
				AppendMACChar(_T('0'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_1:
			{
				AppendMACChar(_T('1'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_2:
			{
				AppendMACChar(_T('2'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_3:
			{
				AppendMACChar(_T('3'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_4:
			{
				AppendMACChar(_T('4'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_5:
			{
				AppendMACChar(_T('5'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_6:
			{
				AppendMACChar(_T('6'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_7:
			{
				AppendMACChar(_T('7'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_8:
			{
				AppendMACChar(_T('8'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_9:
			{
				AppendMACChar(_T('9'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_A:
			{
				AppendMACChar(_T('A'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_B:
			{
				AppendMACChar(_T('B'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_C:
			{
				AppendMACChar(_T('C'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_D:
			{
				AppendMACChar(_T('D'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_E:
			{
				AppendMACChar(_T('E'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_F:
			{
				AppendMACChar(_T('F'));
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_KEY_DEL:
			{
				DeleteMACChar();
				PlaySoundEffect(0);
			}
			break;
		case IDC_SETTING_BTN_RUN_TESTS:
			{
				SetMAC();
				PlaySoundEffect(0);
			}
			break;
	}
}

#endif
