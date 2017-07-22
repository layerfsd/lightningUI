#include "PanelSettingDAB.h"
#include "../../resource.h"
#include "../../DAB/Inc/DABRadioLib.h"
#include "../../DAB/Adapter/DABAdapter.h"


CPanelSettingDAB::CPanelSettingDAB(void)
{
    
}
CPanelSettingDAB::~CPanelSettingDAB(void)
{

}

BOOL CPanelSettingDAB::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelSettingDAB::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingDAB::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

}

void CPanelSettingDAB::Render()
{
	if(!BeginRender())return;

	m_tagTitleServiceLink.SetAlpha(m_fCurAlpha);
	m_tagTitleServiceLink.Render();
	m_sliServiceLink.SetAlpha(m_fCurAlpha);
	m_sliServiceLink.Render();

	m_tagTitleTA.SetAlpha(m_fCurAlpha);
	m_tagTitleTA.Render();
	m_sliTA.SetAlpha(m_fCurAlpha);
	m_sliTA.Render();

	m_tagTitleLBand.SetAlpha(m_fCurAlpha);
	m_tagTitleLBand.Render();
	m_sliLBand.SetAlpha(m_fCurAlpha);
	m_sliLBand.Render();

	EndRender();
}

BOOL CPanelSettingDAB::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	const   int box_size_x=LIST_BOX_ITEM_WIDTH;
	const   int margin_x=12;
	const   int tag_h = LIST_BOX_TAG_HEIGHT;

	COLORGL cr_w={1.0,1.0,1.0,1.0};
	COLORGL cr_gray={0.25,0.25,0.25,1.0};
	COLORGL cr_dark={0.2,0.2,0.2,1.0};
	COLORGL cr_green={89.0f/256.0f,175.0f/256.0f,97.0f/256.0f,1.0};
	COLORGL cr_b={0.0,0.0,0.0,1.0};

	SetSize(g_iClientWidth-box_size_x,g_iClientHeight);

	//enable scrolling vertical:
	EnableScrollingY(TRUE);

	SLIDERSTRUCTURE sli;

	//int margin_x=20;
	int cx=(g_iClientWidth-240-margin_x*2)/2-10;
	int cy=cx;

	GLfloat py=g_iClientHeight/2-60;

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

	m_sliServiceLink.Initialize(IDC_DAB_SLI_ENABLE_SERVICELINK,this,&sli,pSkinManager);
	m_sliServiceLink.SetRange(0,10);
	m_sliServiceLink.SetValue(FALSE);
	m_sliServiceLink.SetPos(m_siCurSize.cx/2-70,py);
	m_sliServiceLink.SetNotifyFunc(&CPanelSettingDAB::OnNotify,this);

	m_tagTitleServiceLink.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleServiceLink.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleServiceLink.SetCaption(_T("Service Link"));
	m_tagTitleServiceLink.LinkTextGL(pTextGL);
	m_tagTitleServiceLink.SetPos(0,py);
	m_tagTitleServiceLink.SetCaptionHeight(24);
	m_tagTitleServiceLink.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleServiceLink.SetCaptionColor(&cr_gray);

	py -= tag_h;

	m_sliLBand.Initialize(IDC_DAB_SLI_ENABLE_LBAND,this,&sli,pSkinManager);
	m_sliLBand.SetRange(0,10);
	m_sliLBand.SetValue(FALSE);
	m_sliLBand.SetPos(m_siCurSize.cx/2-70,py);
	m_sliLBand.SetNotifyFunc(&CPanelSettingDAB::OnNotify,this);

	m_tagTitleLBand.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleLBand.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleLBand.SetCaption(_T("LBAND"));
	m_tagTitleLBand.LinkTextGL(pTextGL);
	m_tagTitleLBand.SetPos(0,py);
	m_tagTitleLBand.SetCaptionHeight(24);
	m_tagTitleLBand.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleLBand.SetCaptionColor(&cr_gray);

	py -= tag_h;

	m_sliTA.Initialize(IDC_DAB_SLI_ENABLE_TA,this,&sli,pSkinManager);
	m_sliTA.SetRange(0,10);
	m_sliTA.SetValue(FALSE);
	m_sliTA.SetPos(m_siCurSize.cx/2-70,py);
	m_sliTA.SetNotifyFunc(&CPanelSettingDAB::OnNotify,this);

	m_tagTitleTA.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_606),NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	m_tagTitleTA.SetSize((g_iClientWidth-box_size_x-margin_x*2),tag_h);
	m_tagTitleTA.SetCaption(_T("TA"));
	m_tagTitleTA.LinkTextGL(pTextGL);
	m_tagTitleTA.SetPos(0,py);
	m_tagTitleTA.SetCaptionHeight(24);
	m_tagTitleTA.SetCaptionPos(BTN_CAPTION_LEFT,20);
	m_tagTitleTA.SetCaptionColor(&cr_gray);

	return TRUE;
}

void CPanelSettingDAB::OnDabServiceStartReady(WPARAM wParam, LPARAM lParam)
{
	RETAILMSG(1,(_T("===OnDabServiceStartReady: %d===\r\n"),wParam));

	if(wParam)
	{
		byte bON;
		DABApi_GetTASetting(&bON);
		m_sliTA.SetValue(bON?10:0);

		DABApi_GetSrvLinkSetting(&bON);
		m_sliServiceLink.SetValue(bON?10:0);

		DABApi_GetLBandSetting(&bON);
		m_sliLBand.SetValue(bON?10:0);
	}
}
void CALLBACK CPanelSettingDAB::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	CPanelSettingDAB *panel=(CPanelSettingDAB*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelSettingDAB::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_DAB_SLI_ENABLE_TA:
		{
			byte bON;
			if(DABApi_GetTASetting(&bON))
			{
				if(wPara != bON)
				{
					DABApi_SetTASetting();
				}

				DABApi_GetTASetting(&bON);
				RETAILMSG(1,(_T("===DAB set TA=%d\r\n"),bON));
			}
			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_SLI_ENABLE_SERVICELINK:
		{
			DABApi_ServiceLinkOnOff(wPara?TRUE:FALSE);

			byte ServiceLink;
			DABApi_GetSrvLinkSetting(&ServiceLink);
			RETAILMSG(1,(_T("===DAB set ServiceLink=%d\r\n"),ServiceLink));

			PlaySoundEffect(0);
		}
		break;
	case IDC_DAB_SLI_ENABLE_LBAND:
		{
			byte bON;
			if(DABApi_GetLBandSetting(&bON))
			{
				if(wPara != bON)
				{
					DABApi_SetLBandSetting();
				}

				DABApi_GetLBandSetting(&bON);
				RETAILMSG(1,(_T("===DAB set LBand=%d\r\n"),bON));
			}
			PlaySoundEffect(0);
		}
		break;
	}
}

