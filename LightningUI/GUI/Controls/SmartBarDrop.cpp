// CSmartBarDrop.cpp : implementation of the CSmartBarDrop class
//
#include "SmartBarDrop.h"

extern HWND					g_hUIWnd;

CSmartBarDrop::CSmartBarDrop()
{
	// TODO: add construction code here


	m_pDDSBtnDropUp = NULL ; //
	m_pDDSBtnDropDown = NULL ; //

	m_bBtnDropClicked = FALSE;
    m_LastTouchDownTick = 0;
}

CSmartBarDrop::~CSmartBarDrop()
{

}
BOOL CSmartBarDrop::Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns)
{
	CSmartBar::Initialize(pSkinManager,id,hWnd,hIns,0,g_iScreenHeight/2+64,36,80,TRUE/*FALSE*/,_T("_SMARTBAR_PHONE_"));

	LoadRes(L"\\ResidentFlash\\LightningUI\\Skins\\Skin_drop.skn");
// 	LoadRes(L"\\ResidentFlash\\Skin_drop.skn");
// 	LoadRes(L"\\MEDIA CARD\\Skin_drop.skn");

	return TRUE;
}
BOOL CSmartBarDrop::LoadRes(LPCTSTR lpFileName)
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

		case SB_RES_BTN_DROP_UP:
			FillSurface(&m_pDDSBtnDropUp,cursor,cx,cy,depth);
			m_siBtnDrop.cx=cx;
			m_siBtnDrop.cy=cy;
	        RETAILMSG(DEBUG_SMARTBAR,(_T("m_siBtnDrop.cx[%d] m_siBtnDrop.cy[%d]\r\n"),m_siBtnDrop.cx,m_siBtnDrop.cy));
			break;
		case SB_RES_BTN_DROP_DOWN:
			FillSurface(&m_pDDSBtnDropDown,cursor,cx,cy,depth);
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
VOID CSmartBarDrop::ReleaseRes()
{

	if (m_pDDSBtnDropUp != NULL)
	{
		m_pDDSBtnDropUp->Release();
		m_pDDSBtnDropUp = NULL;
	}
	if (m_pDDSBtnDropDown != NULL)
	{
		m_pDDSBtnDropDown->Release();
		m_pDDSBtnDropDown = NULL;
	}

}

void  CSmartBarDrop::OnTouchDown(POINT *pt)
{
    //RETAILMSG(DEBUG_SMARTBAR, (_T("MSG:CSmartBarDrop::OnTouchDown : %d,%d m_siBtnDrop.cx[%d] m_siBtnDrop.cy[%d] m_iOverlayHeight[%d]\r\n"),pt->x,pt->y,m_siBtnDrop.cx,m_siBtnDrop.cy,m_iOverlayHeight));

	pt->y-=(g_iScreenHeight/2+64);       //304
    //static int offset = 10;
	int btn_drop_px=0;                                      //0
	int btn_drop_py=( m_iOverlayHeight-m_siBtnDrop.cy)/2;   //0
	RECT rcDrop = {btn_drop_px, btn_drop_py, btn_drop_px+m_siBtnDrop.cx, btn_drop_py+m_siBtnDrop.cy};

	m_bBtnDropClicked = PtInRect(&rcDrop,*pt);

    //RETAILMSG(DEBUG_SMARTBAR, (_T("MSG:CSmartBarDrop::OnTouchDown : point:%d,%d, aera:%d,%d, click:%d\r\n"),pt->x,pt->y,btn_drop_px+m_siBtnDrop.cx+offset,btn_drop_py+m_siBtnDrop.cy,m_bBtnDropClicked));
//	    m_LastPoint.x = pt->x;
//	    m_LastPoint.y = pt->y;
	if(m_bBtnDropClicked)
	{
		m_bNeedUpdateFrame = TRUE;
        m_LastTouchDownTick = GetTickCount();
	}

}
void  CSmartBarDrop::OnTouchUp(POINT *pt)
{
//	    pt->y-=(g_iScreenHeight/2-m_siBtnDrop.cy/2);

//		RETAILMSG(DEBUG_SMARTBAR, (_T("MSG:CSmartBarDrop::OnTouchUp :%d,%d\r\n"),pt->x,pt->y));
	m_bNeedUpdateFrame = TRUE;

//	    m_LastPoint.x = 0;
//	    m_LastPoint.y = 0;

	if(m_bBtnDropClicked && (GetTickCount()-m_LastTouchDownTick)<1000)
	{
	    RETAILMSG(DEBUG_SMARTBAR, (_T("MAG:CSmartBarDrop::OnTouchUp :Activate smartbar\r\n")));
		::PostMessage(g_hUIWnd,WM_SMARTBAR_CONTROL,SMART_BAR_MEDIA,1);
// 		::PostMessage(g_hUIWnd,WM_SMARTBAR_CONTROL,SMART_BAR_DROP,0);
	}

	m_bBtnDropClicked = FALSE;

}
void  CSmartBarDrop::OnTouchMove(POINT *pt)
{
//	RETAILMSG(DEBUG_SMARTBAR, (_T("**CSmartBarDrop::OnTouchMove :%d,%d \r\n"),pt->x,pt->y));

//	    pt->y-=(g_iScreenHeight/2-m_siBtnDrop.cy/2);
//		RETAILMSG(DEBUG_SMARTBAR, (_T("**CSmartBarDrop::OnTouchMove :%d,%d\r\n"),pt->x,pt->y));
//		m_bNeedUpdateFrame = TRUE;
//
//	    if(pt->x - m_LastPoint.x > 70 && m_bBtnDropClicked)
//	    {
//		    RETAILMSG(DEBUG_SMARTBAR, (_T("**CSmartBarDrop::OnTouchMove :Activate smartbar\r\n")));
//			::PostMessage(g_hUIWnd,WM_SMARTBAR_CONTROL,SMART_BAR_MEDIA,1);
//	    }
//		m_bBtnDropClicked = FALSE;
}

void CSmartBarDrop::Render()
{
	//test:
	if(m_pDDSOverlay)
	{
		if(m_pDDSBack)
		{
			int btn_drop_px=0;
			int btn_drop_py=( m_iOverlayHeight-m_siBtnDrop.cy)/2;   //0
			RECT rcDrop = {btn_drop_px, btn_drop_py, btn_drop_px+m_siBtnDrop.cx, btn_drop_py+m_siBtnDrop.cy};

			if(m_bBtnDropClicked)
			{
				if(m_pDDSBtnDropDown)
					m_pDDSBack->AlphaBlt(&rcDrop, m_pDDSBtnDropDown,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
			}
			else
			{
				if(m_pDDSBtnDropUp)
					m_pDDSBack->AlphaBlt(&rcDrop, m_pDDSBtnDropUp,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
			}

			//
			m_pDDSOverlay->AlphaBlt(NULL, m_pDDSBack,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
		}

		//m_pDDSOverlay->Flip(NULL,DDFLIP_WAITNOTBUSY);
	}
}
