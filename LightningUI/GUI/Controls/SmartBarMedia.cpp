// CSmartBarMedia.cpp : implementation of the CSmartBarMedia class
//
#include "SmartBarMedia.h"
#if CVTE_EN_KEY_PROXY
#include "../Key/CKeyHandler.h"
#endif

CSmartBarMedia::CSmartBarMedia()
{
	// TODO: add construction code here

	m_pDDSBackground = NULL;
	m_pDDSBar = NULL;
	m_pDDSSymbos = NULL;

	m_pDDSIconUSB = NULL;
	m_pDDSIconSD = NULL;
	m_pDDSIconBT = NULL;
	m_pDDSIconRadio = NULL;
	m_pDDSIconiPod = NULL;
	m_pDDSIconCarPlay = NULL;

    m_pDDSBtnPlayUp = NULL;
	m_pDDSBtnPlayDown = NULL;
	m_pDDSBtnPauseUp = NULL;
	m_pDDSBtnPauseDown = NULL;
	m_pDDSBtnPreUp = NULL;
	m_pDDSBtnPreDown = NULL;
	m_pDDSBtnNextUp = NULL;
	m_pDDSBtnNextDown = NULL;

	m_bBtnPlayClicked = FALSE;
	m_bBtnPreClicked = FALSE;
	m_bBtnNextClicked = FALSE;

	m_idType = SB_MEDIATYPE_NULL;
	m_bPlayingFlag = FALSE;

	m_bPressed = FALSE;
}

CSmartBarMedia::~CSmartBarMedia()
{

}

BOOL CSmartBarMedia::Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns)
{
	int cx=250;
	int cy=g_iScreenHeight;

	CSmartBar::Initialize(pSkinManager,id,hWnd,hIns,0, 0,cx,cy,TRUE/*FALSE*/,_T("_SMARTBAR_MEDIA_"),0,500,cx,100);

 	LoadRes(L"\\ResidentFlash\\LightningUI\\Skins\\skin_media.skn");
//  LoadRes(L"\\ResidentFlash\\skin_media.skn");

//	LoadRes(L"\\Gps Card\\skin_media.skn");

	//
	m_strNowPlayingTitle=_T("0/0");
	m_strTrackName=_T("98.80");

	return TRUE;
}
BOOL CSmartBarMedia::LoadRes(LPCTSTR lpFileName)
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
			//volume
		case SB_RES_BTN_PLAY_UP:
			FillSurface(&m_pDDSBtnPlayUp,cursor,cx,cy,depth);
			m_siButton.cx=cx;
			m_siButton.cy=cy;
			break;
		case SB_RES_BTN_PLAY_DOWN:
			FillSurface(&m_pDDSBtnPlayDown,cursor,cx,cy,depth);
			break;

		case SB_RES_BTN_PAUSE_UP:
			FillSurface(&m_pDDSBtnPauseUp,cursor,cx,cy,depth);
			break;
		case SB_RES_BTN_PAUSE_DOWN:
			FillSurface(&m_pDDSBtnPauseDown,cursor,cx,cy,depth);
			break;

		case SB_RES_BTN_PRE_UP:
			FillSurface(&m_pDDSBtnPreUp,cursor,cx,cy,depth);
			break;
		case SB_RES_BTN_PRE_DOWN:
			FillSurface(&m_pDDSBtnPreDown,cursor,cx,cy,depth);
			break;

		case SB_RES_BTN_NEXT_UP:
			FillSurface(&m_pDDSBtnNextUp,cursor,cx,cy,depth);
			break;
		case SB_RES_BTN_NEXT_DOWN:
			FillSurface(&m_pDDSBtnNextDown,cursor,cx,cy,depth);
			break;

		case SB_RES_BACKGROUND:
			FillSurface(&m_pDDSBackground,cursor,cx,cy,depth);
			break;
		case SB_RES_BAR:
			FillSurface(&m_pDDSBar,cursor,cx,cy,depth);
			break;
		case SB_RES_ICON_BT:
			FillSurface(&m_pDDSIconBT,cursor,cx,cy,depth);
			m_siIcon.cx=cx;
			m_siIcon.cy=cy;
			break;

		case SB_RES_ICON_RADIO:
			FillSurface(&m_pDDSIconRadio,cursor,cx,cy,depth);
			break;
		case SB_RES_ICON_USB:
			FillSurface(&m_pDDSIconUSB,cursor,cx,cy,depth);
			break;
		case SB_RES_ICON_SD:
			FillSurface(&m_pDDSIconSD,cursor,cx,cy,depth);
			break;
		case SB_RES_ICON_IPOD:
			FillSurface(&m_pDDSIconiPod,cursor,cx,cy,depth);
			break;
		case SB_RES_ICON_CARPLAY:
			FillSurface(&m_pDDSIconCarPlay,cursor,cx,cy,depth);
			break;

		case SB_RES_SYMBOS:
			FillSurface(&m_pDDSSymbos,cursor,cx,cy,depth);
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
void CSmartBarMedia::DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si)
{
// 	if(!m_pDDSSymbos || !pSurface)
// 	{
// 		if(si)
// 		{
// 			si->cx=0;
// 			si->cy=0;
// 		}
// 		return;
// 	}

	//if pSurface = NULL, get char size only


	int px=0;
	int py=0;
	int cx=0;
	int cy=0;

	RECT rcDst ;
	RECT rcSrc ;

	switch (c)
	{
	case '1':
		px=0;		py=0;		cx=22;		cy=40;
		break;
	case '2':
		px=22;		py=0;		cx=22;		cy=40;
		break;
	case '3':
		px=44;		py=0;		cx=22;		cy=40;
		break;
	case '4':
		px=66;		py=0;		cx=22;		cy=40;
		break;
	case '5':
		px=88;		py=0;		cx=22;		cy=40;
		break;
	case '6':
		px=110;		py=0;		cx=22;		cy=40;
		break;
	case '7':
		px=132;		py=0;		cx=22;		cy=40;
		break;
	case '8':
		px=154;		py=0;		cx=22;		cy=40;
		break;
	case '9':
		px=176;		py=0;		cx=22;		cy=40;
		break;
	case '0':
		px=198;		py=0;		cx=22;		cy=40;
		break;
	case '*':
		px=220;		py=0;		cx=18;		cy=40;
		break;
	case '#':
		px=238;		py=0;		cx=26;		cy=40;
		break;
	case '+':
		px=265;		py=0;		cx=24;		cy=40;
		break;
	case '.':
		px=291;		py=0;		cx=9;		cy=40;
		break;
	case '/':
		px=300;		py=0;		cx=18;		cy=40;
		break;
	case ':':
		px=318;		py=0;		cx=8;		cy=40;
		break;
	case '-':
		px=326;		py=0;		cx=17;		cy=40;
		break;
	default:
		if(si)
		{
			si->cx=0;
			si->cy=0;
		}
		return;
		break;
	}

	rcSrc.left=px;
	rcSrc.top=py;
	rcSrc.right=px+cx;
	rcSrc.bottom=py+cy;

	rcDst.left=x;
	rcDst.top=y;
	rcDst.right=x+cx;
	rcDst.bottom=y+cy;

	if(m_pDDSSymbos && pSurface)
	{
		pSurface->AlphaBlt(&rcDst, m_pDDSSymbos,&rcSrc, DDBLT_WAITNOTBUSY, NULL);
	}

	if(si)
	{
		si->cx=cx;
		si->cy=cy;
	}
}
VOID CSmartBarMedia::ReleaseRes()
{
	ReleaseSurface(m_pDDSBackground);
	ReleaseSurface(m_pDDSBar);
	ReleaseSurface(m_pDDSSymbos);

	ReleaseSurface(m_pDDSIconUSB);
	ReleaseSurface(m_pDDSIconSD);
	ReleaseSurface(m_pDDSIconBT);
	ReleaseSurface(m_pDDSIconRadio);
	ReleaseSurface(m_pDDSIconiPod);
	ReleaseSurface(m_pDDSIconCarPlay);

	ReleaseSurface(m_pDDSBtnPlayUp);
	ReleaseSurface(m_pDDSBtnPlayDown);
	ReleaseSurface(m_pDDSBtnPauseUp);
	ReleaseSurface(m_pDDSBtnPauseDown);
	ReleaseSurface(m_pDDSBtnPreUp);
	ReleaseSurface(m_pDDSBtnPreDown);
	ReleaseSurface(m_pDDSBtnNextUp);
	ReleaseSurface(m_pDDSBtnNextDown);

}

void CSmartBarMedia::Render()
{
	//test:
	if(m_pDDSOverlay)
	{
		if(m_pDDSBack)
		{
			if(m_pDDSBackground)
			{
				m_pDDSBack->AlphaBlt(NULL, m_pDDSBackground,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
			}

// 			//bar:
// 			RECT rcBar = {0, m_iOverlayHeight-100, m_iOverlayWidth, m_iOverlayHeight};
// 			if(m_pDDSBar)
// 			{
// 				m_pDDSBack->AlphaBlt(&rcBar, m_pDDSBar,NULL, DDBLT_WAITNOTBUSY, NULL);
// 			}

			{
				//Icon:
				int iconpx=( m_iOverlayWidth-m_siIcon.cx)/2;
				int iconpy=( m_iOverlayHeight-m_siIcon.cy)/2+20;

				RECT rcIcon = {iconpx, iconpy, iconpx+m_siIcon.cx, iconpy+m_siIcon.cy};

				if(m_idType == SB_MEDIATYPE_A2DP)
				{
					if(m_pDDSIconBT)
						m_pDDSBack->AlphaBlt(&rcIcon, m_pDDSIconBT,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else if(m_idType == SB_MEDIATYPE_RADIO)
				{
					if(m_pDDSIconRadio)
						m_pDDSBack->AlphaBlt(&rcIcon, m_pDDSIconRadio,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else if(m_idType == SB_MEDIATYPE_IPOD)
				{
					if(m_pDDSIconiPod)
						m_pDDSBack->AlphaBlt(&rcIcon, m_pDDSIconiPod,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else if(m_idType == SB_MEDIATYPE_CARPLAY)
				{
					if(m_pDDSIconCarPlay)
						m_pDDSBack->AlphaBlt(&rcIcon, m_pDDSIconCarPlay,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else if(m_idType == SB_MEDIATYPE_SD)
				{
					if(m_pDDSIconSD)
						m_pDDSBack->AlphaBlt(&rcIcon, m_pDDSIconSD,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else if(m_idType == SB_MEDIATYPE_USB)
				{
					if(m_pDDSIconUSB)
						m_pDDSBack->AlphaBlt(&rcIcon, m_pDDSIconUSB,NULL, DDBLT_WAITNOTBUSY, NULL);
				}

				//button:

				//play btn:
				int btn_play_px=( m_iOverlayWidth-m_siButton.cx)/2;
				int btn_play_py=( m_iOverlayHeight-70-m_siButton.cy/2);
				RECT rcBtn_play = {btn_play_px, btn_play_py, btn_play_px+m_siButton.cx, btn_play_py+m_siButton.cy};
				if(m_bBtnPlayClicked)
				{
					if(m_bPlayingFlag)
					{
						if(m_pDDSBtnPauseDown)
							m_pDDSBack->AlphaBlt(&rcBtn_play, m_pDDSBtnPauseDown,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
					else
					{
						if(m_pDDSBtnPlayDown)
							m_pDDSBack->AlphaBlt(&rcBtn_play, m_pDDSBtnPlayDown,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
				}
				else
				{
					if(m_bPlayingFlag)
					{
						if(m_pDDSBtnPauseUp)
							m_pDDSBack->AlphaBlt(&rcBtn_play, m_pDDSBtnPauseUp,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
					else
					{
						if(m_pDDSBtnPlayUp)
							m_pDDSBack->AlphaBlt(&rcBtn_play, m_pDDSBtnPlayUp,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
				}

				//previous btn:
				int btn_previous_px=( m_iOverlayWidth-m_siButton.cx)/2-80;
				int btn_previous_py=( m_iOverlayHeight-70-m_siButton.cy/2);
				RECT rcBtn_previous = {btn_previous_px, btn_previous_py, btn_previous_px+m_siButton.cx, btn_previous_py+m_siButton.cy};
				if(m_bBtnPreClicked)
				{
					if(m_pDDSBtnPreDown)
						m_pDDSBack->AlphaBlt(&rcBtn_previous, m_pDDSBtnPreDown,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else
				{
					if(m_pDDSBtnPreUp)
						m_pDDSBack->AlphaBlt(&rcBtn_previous, m_pDDSBtnPreUp,NULL, DDBLT_WAITNOTBUSY, NULL);
				}

				//next btn:
				int btn_next_px=( m_iOverlayWidth-m_siButton.cx)/2+80;
				int btn_next_py=( m_iOverlayHeight-70-m_siButton.cy/2);
				RECT rcBtn_next = {btn_next_px, btn_next_py, btn_next_px+m_siButton.cx, btn_next_py+m_siButton.cy};
				if(m_bBtnNextClicked)
				{
					if(m_pDDSBtnNextDown)
						m_pDDSBack->AlphaBlt(&rcBtn_next, m_pDDSBtnNextDown,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else
				{
					if(m_pDDSBtnNextUp)
						m_pDDSBack->AlphaBlt(&rcBtn_next, m_pDDSBtnNextUp,NULL, DDBLT_WAITNOTBUSY, NULL);
				}

				//draw number:
				if(m_strNowPlayingTitle.String())
				{
					RECT rc={10,40, m_iOverlayWidth-10,100};
					DrawStringGDI(m_pDDSBack,m_strNowPlayingTitle.String(),&rc,DT_CENTER|DT_WORD_ELLIPSIS,48);
				}

				if(m_strTrackName.String())
				{
					RECT rc={30,116, m_iOverlayWidth-30,216};
					DrawStringGDI(m_pDDSBack,m_strTrackName.String(),&rc,DT_CENTER/*|DT_WORDBREAK*/|DT_WORD_ELLIPSIS,(m_idType==SB_MEDIATYPE_RADIO)?54:36);
				}
			}

			//
			m_pDDSOverlay->AlphaBlt(NULL, m_pDDSBack,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
		}

		//m_pDDSOverlay->Flip(NULL,DDFLIP_WAITNOTBUSY);
	}
}
void CSmartBarMedia::OnTouchDown(POINT *pt)
{
	pt->y-=380;
	m_ptLastPress=*pt;

	int offset=30;

	int btn_play_px=( m_iOverlayWidth-m_siButton.cx)/2;
	int btn_play_py=( m_iOverlayHeight-100);
	RECT rcBtn_play = {btn_play_px-offset, btn_play_py-offset, btn_play_px+m_siButton.cx+offset, btn_play_py+m_siButton.cy+offset};
	m_bBtnPlayClicked = PtInRect(&rcBtn_play,*pt);

	int btn_previous_px=( m_iOverlayWidth-m_siButton.cx)/2-120;
	int btn_previous_py=( m_iOverlayHeight-100);
	RECT rcBtn_previous = {btn_previous_px-offset, btn_previous_py-offset, btn_previous_px+m_siButton.cx+offset, btn_previous_py+m_siButton.cy+offset};
	m_bBtnPreClicked = PtInRect(&rcBtn_previous,*pt);

	int btn_next_px=( m_iOverlayWidth-m_siButton.cx)/2+120;
	int btn_next_py=( m_iOverlayHeight-100);
	RECT rcBtn_next = {btn_next_px-offset, btn_next_py-offset, btn_next_px+m_siButton.cx+offset, btn_next_py+m_siButton.cy+offset};
	m_bBtnNextClicked = PtInRect(&rcBtn_next,*pt);


	if(m_bBtnPlayClicked || m_bBtnPreClicked || m_bBtnNextClicked)
	{
		m_dwLastShowTick = GetTickCount();
		m_bPressed = TRUE;
		m_bNeedUpdateFrame = TRUE;
	}

#if CVTE_EN_KEY_PROXY
	if(m_bBtnPlayClicked)
	{
		CKeysHandler::GetInstance()->KeyDown(m_bPlayingFlag?KEY_MEDIA_PAUSE:KEY_MEDIA_PLAY);
	}
	else if(m_bBtnPreClicked)
	{
		CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_PREVIOUS);
	}	
	else if(m_bBtnNextClicked)
	{
		CKeysHandler::GetInstance()->KeyDown(KEY_MEDIA_NEXT);
	}
#endif	
}
void CSmartBarMedia::OnTouchUp(POINT *pt)
{
#if CVTE_EN_KEY_PROXY
	pt->y-=380;
	int offset=30;

	if(m_bBtnPlayClicked)
	{
		int btn_play_px=( m_iOverlayWidth-m_siButton.cx)/2;
		int btn_play_py=( m_iOverlayHeight-100);
		RECT rcBtn_play = {btn_play_px-offset, btn_play_py-offset, btn_play_px+m_siButton.cx+offset, btn_play_py+m_siButton.cy+offset};
		CKeysHandler::GetInstance()->KeyUp(m_bPlayingFlag?KEY_MEDIA_PAUSE:KEY_MEDIA_PLAY, !PtInRect(&rcBtn_play,*pt) );
	}
	else if(m_bBtnPreClicked)
	{
		int btn_previous_px=( m_iOverlayWidth-m_siButton.cx)/2-120;
		int btn_previous_py=( m_iOverlayHeight-100);
		RECT rcBtn_previous = {btn_previous_px-offset, btn_previous_py-offset, btn_previous_px+m_siButton.cx+offset, btn_previous_py+m_siButton.cy+offset};
		CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_PREVIOUS, !PtInRect(&rcBtn_previous,*pt));
	}	
	else if(m_bBtnNextClicked)
	{
		int btn_next_px=( m_iOverlayWidth-m_siButton.cx)/2+120;
		int btn_next_py=( m_iOverlayHeight-100);
		RECT rcBtn_next = {btn_next_px-offset, btn_next_py-offset, btn_next_px+m_siButton.cx+offset, btn_next_py+m_siButton.cy+offset};
		CKeysHandler::GetInstance()->KeyUp(KEY_MEDIA_NEXT, !PtInRect(&rcBtn_next,*pt));
	}
#else

	if(m_bBtnPlayClicked)
	{
		if(m_hMainWnd)
		{
			::PostMessage(m_hMainWnd,WM_MEDIA_KEY,m_bPlayingFlag?KEY_MEDIA_PAUSE:KEY_MEDIA_PLAY,NULL);
		}
	}
	else if(m_bBtnPreClicked)
	{
		if(m_hMainWnd)
		{
			::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_PREVIOUS,NULL);
		}
	}
	else if(m_bBtnNextClicked)
	{
		if(m_hMainWnd)
		{
			::PostMessage(m_hMainWnd,WM_MEDIA_KEY,KEY_MEDIA_NEXT,NULL);
		}
	}
#endif
	m_bBtnPlayClicked=FALSE;
	m_bBtnPreClicked=FALSE;
	m_bBtnNextClicked=FALSE;

	m_bNeedUpdateFrame = TRUE;
}
void CSmartBarMedia::OnTouchMove(POINT *pt)
{
	if(m_bBtnPlayClicked || m_bBtnPreClicked || m_bBtnNextClicked)
	{
		m_dwLastShowTick = GetTickCount();
	}
}
