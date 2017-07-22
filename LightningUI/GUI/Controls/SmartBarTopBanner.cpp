// CSmartBarTopBanner.cpp : implementation of the CSmartBarTopBanner class
//
#include "SmartBarTopBanner.h"

extern HWND					g_hUIWnd;

CSmartBarTopBanner::CSmartBarTopBanner()
{
	// TODO: add construction code here


	m_pDDSBtnGreenUp = NULL ; // 
	m_pDDSBtnGreenDown = NULL ; // 
	m_pDDSTitleCarPlay = NULL;

	m_bBtnDropClicked = FALSE;
	m_idNotificationUI = 0;
}

CSmartBarTopBanner::~CSmartBarTopBanner()
{

}
BOOL CSmartBarTopBanner::Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns)
{
	CSmartBar::Initialize(pSkinManager,id,hWnd,hIns,0,0,g_iScreenWidth,120,TRUE/*FALSE*/,_T("_SMARTBAR_TOPBANNER_"));

//	LoadRes(L"\\Windows\\skin_topbanner.skn");
 	LoadRes(L"\\ResidentFlash\\LightningUI\\Skins\\skin_topbanner.skn");
// 	LoadRes(L"\\GPS CARD\\skin_topbanner.skn");

	return TRUE;
}
BOOL CSmartBarTopBanner::LoadRes(LPCTSTR lpFileName)
{
	FILE								*file = NULL;
	int								fileSize = 0;
	unsigned char				*localBuffer;
	unsigned char				*cursor;
	SKINHEADER				sh;

	if(!lpFileName)
		return FALSE;

	DWORD dwtick=GetTickCount();
	RETAILMSG(DEBUG_SMARTBAR,(_T("Begin to load res file....%s\r\n"),lpFileName));

	if ( 0!=_wfopen_s(&file, lpFileName,_T("rb")) )
	{
		RETAILMSG(DEBUG_SMARTBAR,(_T("Skin file not found %s.\r\n"),lpFileName));
		return FALSE;
	}

	// read file header
	fread(&sh, 1, sizeof(sh), file);

	/**** Computer texture data size ****/
	fseek (file , 0 , SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);


	/**** Read data from file ****/
	localBuffer = new unsigned char[fileSize+1];
	fread(localBuffer, fileSize, 1, file);
	localBuffer[fileSize]='\0';//terminate.

	cursor = localBuffer+sizeof(sh);
	//para data to surface:   
	int i=sh.iCount;
	UINT ID;
	INT depth,cx,cy;
	while(i>0)
	{
		ID=GetInt(cursor);
		depth=GetInt(cursor);
		cx=GetInt(cursor);
		cy=GetInt(cursor);

		switch (ID)
		{

		case SB_RES_BTN_GREEN_UP:
			FillSurface(&m_pDDSBtnGreenUp,cursor,cx,cy,depth);
			m_siBtnDrop.cx=cx;
			m_siBtnDrop.cy=cy;
			break;
		case SB_RES_BTN_GREEN_DOWN:
			FillSurface(&m_pDDSBtnGreenDown,cursor,cx,cy,depth);
			break;
		case SB_RES_BTN_TITLE_CARPLAY:
			FillSurface(&m_pDDSTitleCarPlay,cursor,cx,cy,depth);
			m_siBtnTitle.cx=cx;
			m_siBtnTitle.cy=cy;
			break;
		}

		cursor+=cx*cy*depth;
		i--;
	}

	RETAILMSG(DEBUG_SMARTBAR,(_T("End to load res....%d ms\r\n"),GetTickCount()-dwtick));

	delete [] localBuffer;

	fclose(file);

	return TRUE;
}
VOID CSmartBarTopBanner::ReleaseRes()
{

	if (m_pDDSBtnGreenUp != NULL)
	{
		m_pDDSBtnGreenUp->Release();
		m_pDDSBtnGreenUp = NULL;
	}
	if (m_pDDSBtnGreenDown != NULL)
	{
		m_pDDSBtnGreenDown->Release();
		m_pDDSBtnGreenDown = NULL;
	}

}

void  CSmartBarTopBanner::OnTouchDown(POINT *pt)
{
	//pt->y-=(g_iScreenHeight/2-m_siBtnDrop.cy/2);
	RETAILMSG(1/*DEBUG_SMARTBAR*/, (_T("**CSmartBarTopBanner::OnTouchDown :%d,%d\r\n"),pt->x,pt->y));


	m_bBtnDropClicked = pt->y < m_iOverlayHeight;

	if(m_bBtnDropClicked)
	{
		m_bNeedUpdateFrame = TRUE;
		m_bClickActivate = TRUE;
	}

}
void  CSmartBarTopBanner::OnTouchUp(POINT *pt)
{
	m_bNeedUpdateFrame = TRUE;

	RETAILMSG(1/*DEBUG_SMARTBAR*/, (_T("**CSmartBarTopBanner::OnTouchUp :%d,%d\r\n"),pt->x,pt->y));

	if(/*m_bBtnDropClicked && */pt->y < m_iOverlayHeight && m_idNotificationUI)
	{
		SwitchPage(GET_PANEL_CLASS_ID(m_idNotificationUI),GET_PANEL_PAGE_ID(m_idNotificationUI),PAGE_SWITCH_ANI_NONE);
	}
	Show(FALSE);
	m_bBtnDropClicked = FALSE;
	m_bClickActivate = FALSE;
}
void  CSmartBarTopBanner::OnTouchMove(POINT *pt)
{

}
void CSmartBarTopBanner::SetTitle(const wchar_t *pstrTitle)
{
	m_strTitle = pstrTitle;
}
void CSmartBarTopBanner::SetNotificationUI(UINT idPanel)
{
	m_idNotificationUI = idPanel;
}

void CSmartBarTopBanner::Render()
{
	//test:
	if(m_pDDSOverlay)
	{
		if(m_pDDSBack)
		{
			int title_px=( m_iOverlayWidth-m_siBtnTitle.cx )/ 2;
			int title_py=( m_iOverlayHeight-m_siBtnTitle.cy )/ 2;
			RECT rcTitle = {title_px, title_py, title_px+m_siBtnTitle.cx, title_py+m_siBtnTitle.cy};

			if(m_bBtnDropClicked)
			{
				if(m_pDDSBtnGreenDown)
					m_pDDSBack->AlphaBlt(NULL, m_pDDSBtnGreenDown,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
			}
			else
			{
				if(m_pDDSBtnGreenUp)
					m_pDDSBack->AlphaBlt(NULL, m_pDDSBtnGreenUp,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
			}

// 			if(m_pDDSTitleCarPlay)
// 				m_pDDSBack->AlphaBlt(&rcTitle, m_pDDSTitleCarPlay,NULL, DDBLT_WAITNOTBUSY, NULL);
			RECT rc={0,0, m_iOverlayWidth, m_iOverlayHeight};
			if(m_strTitle.String())
				DrawStringGDI(m_pDDSBack,m_strTitle.String(),&rc,DT_CENTER|DT_VCENTER,32,RGB(255,255,255));

			//
			m_pDDSOverlay->AlphaBlt(NULL, m_pDDSBack,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
		}

		//m_pDDSOverlay->Flip(NULL,DDFLIP_WAITNOTBUSY);
	}
}
