// CRearOverlay.cpp : implementation of the CRearOverlay class
//
#include "CRearOverlay.h"

extern HWND					g_hUIWnd;

CRearOverlay::CRearOverlay()
{
	// TODO: add construction code here

	m_pDDSRearStaticSafety = NULL ; //
	m_bBtnHomeClicked = FALSE;
}

CRearOverlay::~CRearOverlay()
{

}
BOOL CRearOverlay::Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns)
{
	int cx=g_iScreenWidth;
	int cy=70;
	CSmartBar::Initialize(pSkinManager,id,hWnd,hIns,0,g_iScreenHeight-cy,cx,cy,TRUE/*FALSE*/,_T("_REAR_OVERLAY_"));
	RETAILMSG(DEBUG_REAR_OVERLAY,(_T("MSG:[%s]:\r\n"), TEXT(__FUNCTION__)));

	LoadRes(L"\\ResidentFlash\\LightningUI\\Skins\\Skin_RearOverlay.skn");
	return TRUE;
}
BOOL CRearOverlay::LoadRes(LPCTSTR lpFileName)
{
	FILE								*file = NULL;
	int								fileSize = 0;
	unsigned char				*localBuffer;
	unsigned char				*cursor;
	SKINHEADER				sh;

	if(!lpFileName)
		return FALSE;

	DWORD dwtick=GetTickCount();
	RETAILMSG(DEBUG_REAR_OVERLAY,(_T("MSG:[%s]:Begin to load res file....%s\r\n"), TEXT(__FUNCTION__),lpFileName));

	if ( 0!=_wfopen_s(&file, lpFileName,_T("rb")) )
	{
		RETAILMSG(DEBUG_REAR_OVERLAY,(_T("ERR:[%s]:Skin file not found %s.\r\n"), TEXT(__FUNCTION__),lpFileName));
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
	RETAILMSG(DEBUG_REAR_OVERLAY,(_T("MSG:[%s]: iCount[%d]\r\n"),TEXT(__FUNCTION__),i));
	while(i>0)
	{
		ID=GetInt(cursor);
		depth=GetInt(cursor);
		cx=GetInt(cursor);
		cy=GetInt(cursor);

		switch (ID)
		{
		case SB_RES_REAR_STATIC_SAFETY:
			FillSurface(&m_pDDSRearStaticSafety,cursor,cx,cy,depth);
			m_siRearStaticSafety.cx=cx;
			m_siRearStaticSafety.cy=cy;
	        RETAILMSG(DEBUG_REAR_OVERLAY,(_T("MSG:[%s]:m_siRearStaticSafety.cx[%d] m_siRearStaticSafety.cy[%d]\r\n"),TEXT(__FUNCTION__),m_siRearStaticSafety.cx,m_siRearStaticSafety.cy));
			break;
		case SB_RES_REAR_HOME:
			FillSurface(&m_pDDSBtnHome,cursor,cx,cy,depth);
			m_siBtnHome.cx=cx;
			m_siBtnHome.cy=cy;
	        RETAILMSG(DEBUG_REAR_OVERLAY,(_T("MSG:[%s]:m_pDDSBtnHome.cx[%d] m_pDDSBtnHome.cy[%d]\r\n"),TEXT(__FUNCTION__),m_siBtnHome.cx,m_siBtnHome.cy));
			break;
		case SB_RES_REAR_HOME_FOCUS:
			FillSurface(&m_pDDSBtnHomeFocus,cursor,cx,cy,depth);
	        RETAILMSG(DEBUG_REAR_OVERLAY,(_T("MSG:[%s]:m_pDDSBtnHomeFocus.cx[%d] m_pDDSBtnHomeFocus.cy[%d]\r\n"),TEXT(__FUNCTION__),m_siBtnHome.cx,m_siBtnHome.cy));
			break;

		}

		cursor+=cx*cy*depth;
		i--;
	}

	RETAILMSG(DEBUG_REAR_OVERLAY,(_T("MSG:[%s]:End to load res....%d ms\r\n"), TEXT(__FUNCTION__),GetTickCount()-dwtick));

	delete [] localBuffer;

	fclose(file);

	return TRUE;
}
VOID CRearOverlay::ReleaseRes()
{

	if (m_pDDSRearStaticSafety != NULL)
	{
		m_pDDSRearStaticSafety->Release();
		m_pDDSRearStaticSafety = NULL;
	}

	if (m_pDDSBtnHome != NULL)
	{
		m_pDDSBtnHome->Release();
		m_pDDSBtnHome = NULL;
	}

	if (m_pDDSBtnHomeFocus != NULL)
	{
		m_pDDSBtnHomeFocus->Release();
		m_pDDSBtnHomeFocus = NULL;
	}
}

void  CRearOverlay::OnTouchDown(POINT *pt)
{
	//RETAILMSG(1, (TEXT("MSG:CRearOverlay::OnTouchDown:%d,%d\r\n"),pt->x, pt->y));
	pt->y -= 410;

	int btn_home_px=20;
	int btn_home_py=( m_iOverlayHeight-m_siBtnHome.cy)/2;
	RECT rcBtn_home = {btn_home_px, btn_home_py, btn_home_px+m_siBtnHome.cx, btn_home_py+m_siBtnHome.cy};
	m_bBtnHomeClicked = PtInRect(&rcBtn_home,*pt);

	if(m_bBtnHomeClicked)
	{
		//RETAILMSG(1, (TEXT("MSG:CRearOverlay::OnTouchDown:\r\n")));
		m_dwLastShowTick = GetTickCount();
		m_bPressed = TRUE;
		m_bNeedUpdateFrame = TRUE;
	}
}
void  CRearOverlay::OnTouchUp(POINT *pt)
{
	//RETAILMSG(1, (TEXT("MSG:CRearOverlay::OnTouchUp:%d,%d \r\n"),pt->x, pt->y));

	if(m_bBtnHomeClicked)
	{
		if(m_hMainWnd)
		{
			::PostMessage(m_hMainWnd,WM_REARVIEW_KEY,KEY_REARVIEW_HOME,NULL);
		}
	}

	m_bBtnHomeClicked=FALSE;
	m_bNeedUpdateFrame = TRUE;
}
void  CRearOverlay::OnTouchMove(POINT *pt)
{
	//RETAILMSG(1, (TEXT("MSG:CRearOverlay::OnTouchMove:%d,%d \r\n"),pt->x, pt->y));

	if(m_bBtnHomeClicked)
	{
		m_dwLastShowTick = GetTickCount();
	}
}

void CRearOverlay::Render()
{
	//test:
	if(m_pDDSOverlay)
	{
		if(m_pDDSBack)
		{
			int RearStaticSafteyPx = 0;
			int RearStaticSafteyPy = ( m_iOverlayHeight-m_siRearStaticSafety.cy)/2;
			RECT rcRearStaticSaftey = {RearStaticSafteyPx, RearStaticSafteyPy, RearStaticSafteyPx+m_siRearStaticSafety.cx, RearStaticSafteyPy+m_siRearStaticSafety.cy};
			if(m_pDDSRearStaticSafety)
			{
				m_pDDSBack->AlphaBlt(&rcRearStaticSaftey, m_pDDSRearStaticSafety,NULL, DDBLT_WAITNOTBUSY, NULL);
			}

			//Home btn:
			int btn_home_px= 20;
			int btn_home_py=( m_iOverlayHeight-m_siBtnHome.cy)/2;
			RECT rcBtn_home = {btn_home_px, btn_home_py, btn_home_px+m_siBtnHome.cx, btn_home_py+m_siBtnHome.cy};
			if(m_bBtnHomeClicked)
			{
				if(m_pDDSBtnHomeFocus)
					m_pDDSBack->AlphaBlt(&rcBtn_home, m_pDDSBtnHomeFocus,NULL, DDBLT_WAITNOTBUSY, NULL);
			}
			else
			{
				if(m_pDDSBtnHome)
					m_pDDSBack->AlphaBlt(&rcBtn_home, m_pDDSBtnHome,NULL, DDBLT_WAITNOTBUSY, NULL);
			}

			//
			m_pDDSOverlay->AlphaBlt(NULL, m_pDDSBack,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
		}
		//m_pDDSOverlay->Flip(NULL,DDFLIP_WAITNOTBUSY);
	}
}
