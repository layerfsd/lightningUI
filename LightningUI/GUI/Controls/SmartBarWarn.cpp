// CSmartBarWarn.cpp : implementation of the CSmartBarWarn class
//
#include "SmartBarWarn.h"


CSmartBarWarn::CSmartBarWarn()
{
	// TODO: add construction code here


	m_pDDSBtnAgreeUp = NULL ; // 
	m_pDDSBtnAgreeDown = NULL ; // 
	m_pDDSLabelMainPage = NULL;

	m_bBtnDropClicked = FALSE;
	m_idNotificationUI = 0;

	m_btnAgreePx = 0;
	m_btnAgreePy = 0;
}

CSmartBarWarn::~CSmartBarWarn()
{

}
BOOL CSmartBarWarn::Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns)
{
	CSmartBar::Initialize(pSkinManager,id,hWnd,hIns,0,0,g_iScreenWidth,g_iScreenHeight,TRUE/*FALSE*/,_T("_SMARTBAR_WARN_"));

//	LoadRes(L"\\Windows\\skin_topbanner.skn");
 	LoadRes(L"\\ResidentFlash\\LightningUI\\Skins\\skin_warn.skn");
// 	LoadRes(L"\\GPS CARD\\skin_topbanner.skn");

	return TRUE;
}
BOOL CSmartBarWarn::LoadRes(LPCTSTR lpFileName)
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
		case SB_RES_LABEL_MAIN_PAGE:
			FillSurface(&m_pDDSLabelMainPage,cursor,cx,cy,depth);
			m_siLabelMainPage.cx=cx;
			m_siLabelMainPage.cy=cy;
			break;
		case SB_RES_BTN_AGREE_UP:
			FillSurface(&m_pDDSBtnAgreeUp,cursor,cx,cy,depth);
			m_siBtnAgree.cx=cx;
			m_siBtnAgree.cy=cy;
			m_btnAgreePx= (m_iOverlayWidth - m_siBtnAgree.cx)/2;
			m_btnAgreePy= (m_iOverlayHeight - m_siBtnAgree.cy - 5);
			break;
		case SB_RES_BTN_AGREE_DOWN:
			FillSurface(&m_pDDSBtnAgreeDown,cursor,cx,cy,depth);
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
VOID CSmartBarWarn::ReleaseRes()
{

	if (m_pDDSBtnAgreeUp != NULL)
	{
		m_pDDSBtnAgreeUp->Release();
		m_pDDSBtnAgreeUp = NULL;
	}
	if (m_pDDSBtnAgreeDown != NULL)
	{
		m_pDDSBtnAgreeDown->Release();
		m_pDDSBtnAgreeDown = NULL;
	}
	if (m_pDDSLabelMainPage != NULL)
	{
		m_pDDSLabelMainPage->Release();
		m_pDDSLabelMainPage = NULL;
	}	
}

void  CSmartBarWarn::OnTouchDown(POINT *pt)
{
	RETAILMSG(DEBUG_SMARTBAR, (_T("**CSmartBarWarn::OnTouchDown :%d,%d\r\n"),pt->x,pt->y));

	RECT rcBtnAgree = {m_btnAgreePx-10, m_btnAgreePy-10, m_btnAgreePx+m_siBtnAgree.cx+20, m_btnAgreePy+m_siBtnAgree.cy+20};
	m_bBtnDropClicked = PtInRect(&rcBtnAgree,*pt);

	if(m_bBtnDropClicked)
	{
		m_bNeedUpdateFrame = TRUE;
		m_bClickActivate = TRUE;
	}

}
void  CSmartBarWarn::OnTouchUp(POINT *pt)
{
	m_bNeedUpdateFrame = TRUE;

	RETAILMSG(DEBUG_SMARTBAR, (_T("**CSmartBarWarn::OnTouchUp :%d,%d\r\n"),pt->x,pt->y));

	if (m_bBtnDropClicked)
	{
		Show(FALSE);
		if (m_hMainWnd)
		{
			::PostMessage(m_hMainWnd,WM_CMD_PLAY_SOUND_EFFECT,0,NULL);
		}
	}
	m_bBtnDropClicked = FALSE;
	m_bClickActivate = FALSE;
}
void  CSmartBarWarn::OnTouchMove(POINT *pt)
{

}

void CSmartBarWarn::Render()
{
	//test:
	if(m_pDDSOverlay)
	{
		if(m_pDDSBack)
		{
 			if(m_pDDSLabelMainPage)
			{
				//RECT rcMainPage = {0, 0, m_siLabelMainPage.cx, m_siLabelMainPage.cy};
 				m_pDDSBack->AlphaBlt(/*&rcMainPage*/NULL, m_pDDSLabelMainPage,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
			}

			//btn:
			RECT rcBtnAgree = {m_btnAgreePx, m_btnAgreePy, m_btnAgreePx+m_siBtnAgree.cx, m_btnAgreePy+m_siBtnAgree.cy};

			if(m_bBtnDropClicked)
			{
				if(m_pDDSBtnAgreeDown)
					m_pDDSBack->AlphaBlt(&rcBtnAgree, m_pDDSBtnAgreeDown,NULL, DDBLT_WAITNOTBUSY, NULL);
			}
			else
			{
				if(m_pDDSBtnAgreeUp)
					m_pDDSBack->AlphaBlt(&rcBtnAgree, m_pDDSBtnAgreeUp,NULL, DDBLT_WAITNOTBUSY, NULL);
			}

			//
			m_pDDSOverlay->AlphaBlt(NULL, m_pDDSBack,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
		}
	}
}
