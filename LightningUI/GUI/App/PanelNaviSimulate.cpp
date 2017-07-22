#include "PanelNaviSimulate.h"
#include "../../resource.h"
#include "../Media/MediaCommon.h"

CPanelNaviSimulate::CPanelNaviSimulate(void)
{
	m_bPlayingGuidanceVoice = FALSE;
	m_timerVoiceNotification = 0;
	m_timerSimGuidanceVoice = 0;
}
CPanelNaviSimulate::~CPanelNaviSimulate(void)
{
}
BOOL CPanelNaviSimulate::IsReady()
{
	return CPanelGL::IsReady();

}
void CPanelNaviSimulate::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	SetAlpha(0.1f);
	SetAlpha(1.0f,TRUE);

	::PostMessage(m_hMainWnd,WM_NAVI_MSG,NAVI_START_MSG,1);

}
void CPanelNaviSimulate::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
// 	SetAlpha(1.0f);
	SetAlpha(0.0f,FALSE);

}
void CPanelNaviSimulate::Render()
{
	//static COLORGL crb={0.03125,0.03125,0.03125,1.0};

	if(!BeginRender())return;

	m_btnSimGuidanceVoice.SetAlpha(m_fCurAlpha);
	m_btnSimGuidanceVoice.Render();

	m_btnPlayNotificationAllowDuck.SetAlpha(m_fCurAlpha);
	m_btnPlayNotificationAllowDuck.Render();

	m_btnPlayNotificationNotAllowDuck.SetAlpha(m_fCurAlpha);
	m_btnPlayNotificationNotAllowDuck.Render();

	EndRender();

}
BOOL CPanelNaviSimulate::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	m_bShowStatusBar = FALSE;

	int cx_box=(g_iScreenWidth);

	SIZE si0={cx_box-40,100};
	GLfloat py=g_iClientHeight/2-70;

	m_btnSimGuidanceVoice.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_512),NULL,
		m_pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnSimGuidanceVoice.SetCaption(_T("Start Simulating Guidance Voice"));
	m_btnSimGuidanceVoice.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnSimGuidanceVoice.SetCaptionHeight(28);
	m_btnSimGuidanceVoice.LinkTextGL(pTextGL);
	m_btnSimGuidanceVoice.SetSize(&si0);
	m_btnSimGuidanceVoice.SetNotifyFunc(& CPanelNaviSimulate::OnNotify,this);
	m_btnSimGuidanceVoice.SetPos(0,py);

	py-=110;

	m_btnPlayNotificationAllowDuck.Initialize(1,this,m_pSkinManager->GetTexture(TEXID_512),NULL,
		m_pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayNotificationAllowDuck.SetCaption(_T("Play Notification Allow Ducking"));
	m_btnPlayNotificationAllowDuck.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnPlayNotificationAllowDuck.SetCaptionHeight(28);
	m_btnPlayNotificationAllowDuck.LinkTextGL(pTextGL);
	m_btnPlayNotificationAllowDuck.SetSize(&si0);
	m_btnPlayNotificationAllowDuck.SetNotifyFunc(& CPanelNaviSimulate::OnNotify,this);
	m_btnPlayNotificationAllowDuck.SetPos(0,py);

	py-=110;

	m_btnPlayNotificationNotAllowDuck.Initialize(2,this,m_pSkinManager->GetTexture(TEXID_512),NULL,
		m_pSkinManager->GetTexture(TEXID_513),NULL,
		NULL,NULL,
		NULL,NULL);
	m_btnPlayNotificationNotAllowDuck.SetCaption(_T("Play Notification Not Allow Ducking"));
	m_btnPlayNotificationNotAllowDuck.SetCaptionPos(BTN_CAPTION_CENTER);
	m_btnPlayNotificationNotAllowDuck.SetCaptionHeight(28);
	m_btnPlayNotificationNotAllowDuck.LinkTextGL(pTextGL);
	m_btnPlayNotificationNotAllowDuck.SetSize(&si0);
	m_btnPlayNotificationNotAllowDuck.SetNotifyFunc(& CPanelNaviSimulate::OnNotify,this);
	m_btnPlayNotificationNotAllowDuck.SetPos(0,py);

	return TRUE;
}

void CALLBACK CPanelNaviSimulate::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelNaviSimulate *panel=(CPanelNaviSimulate*)lpUserPara;

	if(!panel)
	{
		return;
	}

	panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
}
void CPanelNaviSimulate::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case 0:
		if(m_bPlayingGuidanceVoice)
		{
			//sndPlaySound(NULL,0);
			::SendMessage(m_hMainWnd,WM_CMD_RINGTONE,0,0);
			m_bPlayingGuidanceVoice = FALSE;
			m_btnSimGuidanceVoice.SetCaption(_T("Start Simulating Guidance Voice"));
			m_btnSimGuidanceVoice.SetStatus(BS_UP);
			if(m_timerSimGuidanceVoice != 0)
			{
				timeKillEvent(m_timerSimGuidanceVoice);
				m_timerSimGuidanceVoice = 0;
			}
		}
		else
		{
			SendMediaCommand(MEDIA_CMD_SET_RINGTONE_FILE,(LPARAM)_T("\\ResidentFlash\\Media\\navi_guidance.wav"));
			//::SendMessage(m_hMainWnd,WM_CMD_RINGTONE,(1<<16)+0,(0<<16)+0xFFFF);//ring, no loop, max volume
			//if(sndPlaySound(_T("\\ResidentFlash\\Media\\navi_guidance.wav"),SND_ASYNC|SND_LOOP))
			{
				m_bPlayingGuidanceVoice = TRUE;
				m_btnSimGuidanceVoice.SetStatus(BS_FOCUS);
				m_btnSimGuidanceVoice.SetCaption(_T("Stop Simulating Guidance Voice"));
			}
			if(m_timerSimGuidanceVoice == 0)
			{
				m_timerSimGuidanceVoice = timeSetEvent( 3000, 1, SimGuidanceVoiceTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
			}
		}
		break;
	case 1:
		if(m_bPlayingGuidanceVoice)
		{
			//sndPlaySound(NULL,0);
			::SendMessage(m_hMainWnd,WM_CMD_RINGTONE,0,0);
			m_bPlayingGuidanceVoice = FALSE;
			m_btnSimGuidanceVoice.SetCaption(_T("Start Simulating Guidance Voice"));
			m_btnSimGuidanceVoice.SetStatus(BS_UP);
			if(m_timerSimGuidanceVoice != 0)
			{
				timeKillEvent(m_timerSimGuidanceVoice);
				m_timerSimGuidanceVoice = 0;
			}
		}
		if(m_timerVoiceNotification == 0)
		{
			if(sndPlaySound(_T("\\ResidentFlash\\Media\\navi_guidance3.wav"),SND_ASYNC))
			{
				::PostMessage(m_hMainWnd,WM_NAVI_MSG,NAVI_GUIDANCE_MSG,(1<<16)+1);
				m_timerVoiceNotification = timeSetEvent( 2000, 1, QuitVoiceNotificationTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
			}
		}
		break;
	case 2:
		if(m_bPlayingGuidanceVoice)
		{
			//sndPlaySound(NULL,0);
			::SendMessage(m_hMainWnd,WM_CMD_RINGTONE,0,0);
			m_bPlayingGuidanceVoice = FALSE;
			m_btnSimGuidanceVoice.SetCaption(_T("Start Simulating Guidance Voice"));
			m_btnSimGuidanceVoice.SetStatus(BS_UP);
			if(m_timerSimGuidanceVoice != 0)
			{
				timeKillEvent(m_timerSimGuidanceVoice);
				m_timerSimGuidanceVoice = 0;
			}
		}
		if(m_timerVoiceNotification == 0)
		{
			if(sndPlaySound(_T("\\ResidentFlash\\Media\\navi_guidance2.wav"),SND_ASYNC))
			{
				::PostMessage(m_hMainWnd,WM_NAVI_MSG,NAVI_GUIDANCE_MSG,1);
				m_timerVoiceNotification = timeSetEvent( 2000, 1, QuitVoiceNotificationTimerProc, (DWORD)this, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
			}
		}
		break;
	}
}

void CALLBACK CPanelNaviSimulate::QuitVoiceNotificationTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelNaviSimulate* panel = (CPanelNaviSimulate*)dwUser;
	if ( panel != NULL )
	{
		::PostMessage(panel->m_hMainWnd,WM_NAVI_MSG,NAVI_GUIDANCE_MSG,0);
		timeKillEvent(panel->m_timerVoiceNotification);
		panel->m_timerVoiceNotification = 0;
	}
}
void CALLBACK CPanelNaviSimulate::SimGuidanceVoiceTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CPanelNaviSimulate* panel = (CPanelNaviSimulate*)dwUser;
	if ( panel != NULL )
	{
		::SendMessage(panel->m_hMainWnd,WM_CMD_RINGTONE,(1<<16)+0,(0<<16)+0xFFFF);//ring, no loop, max volume
		timeKillEvent(panel->m_timerSimGuidanceVoice);
		if(panel->m_bPlayingGuidanceVoice)
		{
			panel->m_timerSimGuidanceVoice = timeSetEvent( 10000, 1, SimGuidanceVoiceTimerProc, (DWORD)panel, TIME_ONESHOT | TIME_CALLBACK_FUNCTION );
		}
	}
}
void CPanelNaviSimulate::OnAndroidAutoMsg(WPARAM wParam, LPARAM lParam)
{
	if(wParam == ANDROIDAUTO_NAVI_STATUS)
	{
		if(lParam)
		{
			//terminate native guidance:
			if(m_bPlayingGuidanceVoice)
			{
				//sndPlaySound(NULL,0);
				::SendMessage(m_hMainWnd,WM_CMD_RINGTONE,0,0);
				m_bPlayingGuidanceVoice = FALSE;
				m_btnSimGuidanceVoice.SetCaption(_T("Start Simulating Guidance Voice"));
				m_btnSimGuidanceVoice.SetStatus(BS_UP);
			}
		}
	}
}