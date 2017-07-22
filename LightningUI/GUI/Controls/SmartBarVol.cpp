// CSmartBarVol.cpp : implementation of the CSmartBarVol class
//
#include "SmartBarVol.h"

#define SMARTBAR_VOL_OVERLAY_WIDTH	256
#define SMARTBAR_VOL_OVERLAY_HEIGHT	256

CSmartBarVol::CSmartBarVol()
{
	// TODO: add construction code here

	m_pDDSBackground = NULL;
	m_pDDSSymbos = NULL;

	//volume:
	m_pDDSIconMute = NULL;
	m_pDDSSliBase=NULL;
	m_pDDSSliActive=NULL;
	m_pDDSIconMedia=NULL;
	m_pDDSIconSpeech=NULL;
	m_pDDSIconPhone=NULL;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	m_pDDSIconEQClassic=NULL;
	m_pDDSIconEQPop=NULL;
	m_pDDSIconEQRock=NULL;
	m_pDDSIconEQJazz=NULL;
	m_pDDSIconEQUser=NULL;
	m_pDDSIconEQNone=NULL;

	m_pDDSIconEQClassicFocus=NULL;
	m_pDDSIconEQPopFocus=NULL;
	m_pDDSIconEQRockFocus=NULL;
	m_pDDSIconEQJazzFocus=NULL;
	m_pDDSIconEQUserFocus=NULL;
	m_pDDSIconEQNoneFocus=NULL;

	m_bBtnEQClicked = FALSE;
#endif

	m_dwVolumeCur =15;
	m_dwVolumeMax =32;

	m_bMute = FALSE;
	m_iVolumeChannel = SB_VOLUME_CHANNEL_MEDIA;

	m_idEQType = 0;

	m_iOverlayPx = 0;
	m_iOverlayPy = 0;
}

CSmartBarVol::~CSmartBarVol()
{

}
void CSmartBarVol::SetMute(BOOL bMute)
{
	m_bMute = bMute;
	m_bNeedUpdateFrame = TRUE;
}
void CSmartBarVol::SetVolume(DWORD dwVolume,UINT iVolumeChannel)
{
	if(dwVolume>m_dwVolumeMax)
	{
		dwVolume = m_dwVolumeMax;
	}

	m_dwVolumeCur = dwVolume;
	m_bNeedUpdateFrame = TRUE;
	
	//Do not set m_bMute here, it only modified by SetMute function.
	//m_bMute = (m_dwVolumeCur==0);
	m_iVolumeChannel = iVolumeChannel;

	//auto hide:
	if(!m_bMute && m_bVisible && m_dwShowTimeOut==INFINITE)
	{
		RETAILMSG(DEBUG_SMARTBAR, (TEXT("---Reset Vol bar show time to 4000--- \r\n")));
		Show(TRUE,4000);
	}
}
BOOL CSmartBarVol::Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns)
{
	m_iOverlayPx = g_iScreenWidth/2-SMARTBAR_VOL_OVERLAY_WIDTH/2;
	m_iOverlayPy = g_iScreenHeight/2-SMARTBAR_VOL_OVERLAY_HEIGHT/2;

	CSmartBar::Initialize(pSkinManager,id,hWnd,hIns,m_iOverlayPx, m_iOverlayPy,SMARTBAR_VOL_OVERLAY_WIDTH,SMARTBAR_VOL_OVERLAY_HEIGHT,TRUE,_T("_SMARTBAR_VOLUME_"));

	LoadRes(L"\\ResidentFlash\\LightningUI\\Skins\\skin_vol.skn");
// 	LoadRes(L"\\ResidentFlash\\skin_vol.skn");


	return TRUE;
}
BOOL CSmartBarVol::LoadRes(LPCTSTR lpFileName)
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
	RETAILMSG(DEBUG_SMARTBAR,(_T("load %d res file\r\n"),i));
	while(i>0)
	{
		ID=GetInt(cursor);
		depth=GetInt(cursor);
		cx=GetInt(cursor);
		cy=GetInt(cursor);
		//RETAILMSG(DEBUG_SMARTBAR,(_T("load ID%d \r\n"),ID));
		switch (ID)
		{
			//volume
		case SB_RES_SLI_VOL_BASE:
			FillSurface(&m_pDDSSliBase,cursor,cx,cy,depth);
			m_siSliBase.cx=cx;
			m_siSliBase.cy=cy;
			break;
		case SB_RES_SLI_VOL_ACTIVE:
			FillSurface(&m_pDDSSliActive,cursor,cx,cy,depth);
			break;
		case SB_RES_BACKGROUND:
			FillSurface(&m_pDDSBackground,cursor,cx,cy,depth);
			m_siSliBackground.cx = cx;
			m_siSliBackground.cy = cy;
			break;
		case SB_RES_ICON_MUTE:
			FillSurface(&m_pDDSIconMute,cursor,cx,cy,depth);
			m_siIconVolume.cx=cx;
			m_siIconVolume.cy=cy;
			break;
		case SB_RES_SYMBOS:
			FillSurface(&m_pDDSSymbos,cursor,cx,cy,depth);
			break;

		case SB_RES_SPEECH:
			FillSurface(&m_pDDSIconSpeech,cursor,cx,cy,depth);
			m_siIconVolumeChannel.cx=cx;
			m_siIconVolumeChannel.cy=cy;
			break;
		case SB_RES_MEDIA:
			FillSurface(&m_pDDSIconMedia,cursor,cx,cy,depth);
			break;
		case SB_RES_PHONE:
			FillSurface(&m_pDDSIconPhone,cursor,cx,cy,depth);
			break;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
		case SB_RES_EQ_CLASSIC:
			m_siEQType.cx=cx;
			m_siEQType.cy=cy;
			m_iconEQPx = (m_iOverlayWidth-m_siEQType.cx)-10;
			m_iconEQPy = 44;
			FillSurface(&m_pDDSIconEQClassic,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_POP:
			FillSurface(&m_pDDSIconEQPop,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_ROCK:
			FillSurface(&m_pDDSIconEQRock,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_JAZZ:
			FillSurface(&m_pDDSIconEQJazz,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_NONE:
			FillSurface(&m_pDDSIconEQNone,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_USER:
			FillSurface(&m_pDDSIconEQUser,cursor,cx,cy,depth);
			break;

		case SB_RES_EQ_CLASSIC_FOCUS:
			FillSurface(&m_pDDSIconEQClassicFocus,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_POP_FOCUS:
			FillSurface(&m_pDDSIconEQPopFocus,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_ROCK_FOCUS:
			FillSurface(&m_pDDSIconEQRockFocus,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_JAZZ_FOCUS:
			FillSurface(&m_pDDSIconEQJazzFocus,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_NONE_FOCUS:
			FillSurface(&m_pDDSIconEQNoneFocus,cursor,cx,cy,depth);
			break;
		case SB_RES_EQ_USER_FOCUS:
			FillSurface(&m_pDDSIconEQUserFocus,cursor,cx,cy,depth);
			break;
#endif

		}

		cursor+=cx*cy*depth;
		i--;
	}

	RETAILMSG(DEBUG_SMARTBAR,(_T("End to load res....%d ms\r\n"),GetTickCount()-dwtick));

	delete [] localBuffer;

	fclose(file);

	return TRUE;
}
void CSmartBarVol::DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si)
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
		px=0;		py=0;		cx=17;		cy=30;
		break;
	case '2':
		px=17;		py=0;		cx=17;		cy=30;
		break;
	case '3':
		px=34;		py=0;		cx=17;		cy=30;
		break;
	case '4':
		px=51;		py=0;		cx=17;		cy=30;
		break;
	case '5':
		px=68;		py=0;		cx=17;		cy=30;
		break;
	case '6':
		px=85;		py=0;		cx=17;		cy=30;
		break;
	case '7':
		px=102;		py=0;		cx=17;		cy=30;
		break;
	case '8':
		px=119;		py=0;		cx=17;		cy=30;
		break;
	case '9':
		px=136;		py=0;		cx=17;		cy=30;
		break;
	case '0':
		px=153;		py=0;		cx=17;		cy=30;
		break;
	case '*':
		px=170;		py=0;		cx=14;		cy=30;
		break;
	case '#':
		px=184;		py=0;		cx=21;		cy=30;
		break;
	case '+':
		px=205;		py=0;		cx=20;		cy=30;
		break;
	case '.':
		px=225;		py=0;		cx=7;		cy=30;
		break;
	case '/':
		px=232;		py=0;		cx=14;		cy=30;
		break;
	case ':':
		px=246;		py=0;		cx=7;		cy=30;
		break;
	case '-':
		px=252;		py=0;		cx=6;		cy=30;
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
VOID CSmartBarVol::ReleaseRes()
{
	if (m_pDDSBackground != NULL)
	{
		m_pDDSBackground->Release();
		m_pDDSBackground = NULL;
	}

	if (m_pDDSIconMute != NULL)
	{
		m_pDDSIconMute->Release();
		m_pDDSIconMute = NULL;
	}

	if (m_pDDSSliBase != NULL)
	{
		m_pDDSSliBase->Release();
		m_pDDSSliBase = NULL;
	}

	if (m_pDDSSliActive != NULL)
	{
		m_pDDSSliActive->Release();
		m_pDDSSliActive = NULL;
	}

	if (m_pDDSSymbos != NULL)
	{
		m_pDDSSymbos->Release();
		m_pDDSSymbos = NULL;
	}
	
	if (m_pDDSIconSpeech != NULL)
	{
		m_pDDSIconSpeech->Release();
		m_pDDSIconSpeech = NULL;
	}	
	
	if (m_pDDSIconMedia != NULL)
	{
		m_pDDSIconMedia->Release();
		m_pDDSIconMedia = NULL;
	}
	
	if (m_pDDSIconPhone != NULL)
	{
		m_pDDSIconPhone->Release();
		m_pDDSIconPhone = NULL;
	}		

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	if (m_pDDSIconEQClassic != NULL)
	{
		m_pDDSIconEQClassic->Release();
		m_pDDSIconEQClassic = NULL;
	}

	if (m_pDDSIconEQPop != NULL)
	{
		m_pDDSIconEQPop->Release();
		m_pDDSIconEQPop = NULL;
	}

	if (m_pDDSIconEQRock != NULL)
	{
		m_pDDSIconEQRock->Release();
		m_pDDSIconEQRock = NULL;
	}

	if (m_pDDSIconEQJazz != NULL)
	{
		m_pDDSIconEQJazz->Release();
		m_pDDSIconEQJazz = NULL;
	}

	if (m_pDDSIconEQUser != NULL)
	{
		m_pDDSIconEQUser->Release();
		m_pDDSIconEQUser = NULL;
	}

	if (m_pDDSIconEQNone != NULL)
	{
		m_pDDSIconEQNone->Release();
		m_pDDSIconEQNone = NULL;
	}

	////

	if (m_pDDSIconEQClassicFocus != NULL)
	{
		m_pDDSIconEQClassicFocus->Release();
		m_pDDSIconEQClassicFocus = NULL;
	}

	if (m_pDDSIconEQPopFocus != NULL)
	{
		m_pDDSIconEQPopFocus->Release();
		m_pDDSIconEQPopFocus = NULL;
	}

	if (m_pDDSIconEQRockFocus != NULL)
	{
		m_pDDSIconEQRockFocus->Release();
		m_pDDSIconEQRockFocus = NULL;
	}

	if (m_pDDSIconEQJazzFocus != NULL)
	{
		m_pDDSIconEQJazzFocus->Release();
		m_pDDSIconEQJazzFocus = NULL;
	}

	if (m_pDDSIconEQUserFocus != NULL)
	{
		m_pDDSIconEQUserFocus->Release();
		m_pDDSIconEQUserFocus = NULL;
	}

	if (m_pDDSIconEQNoneFocus != NULL)
	{
		m_pDDSIconEQNoneFocus->Release();
		m_pDDSIconEQNoneFocus = NULL;
	}
#endif
}

BOOL CSmartBarVol::Show(BOOL bShow,UINT iTimeOut)
{
#if (CUSTOMER_ID == CUSTOMER_CVTE_CERTIFICATION)
	UINT iTime = iTimeOut;
#else
	UINT iTime = ((m_bMute&&bShow)?INFINITE:iTimeOut);
#endif
	return CSmartBar::Show(bShow,iTime);
}
void CSmartBarVol::Render()
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


			{
				//volume slider:
				int slipx=( m_iOverlayWidth-m_siSliBase.cx)/2;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				int slipy= m_iOverlayHeight-m_siSliBase.cy-62;
#else
				int slipy= m_iOverlayHeight-m_siSliBase.cy-56;
#endif

				RECT rcBase = {slipx, slipy, slipx+m_siSliBase.cx, slipy+m_siSliBase.cy};

				RECT rcActive1 = {slipx, slipy, slipx+(m_dwVolumeCur*m_siSliBase.cx)/m_dwVolumeMax, slipy+m_siSliBase.cy};
				RECT rcActive2 = {0, 0, (m_dwVolumeCur*m_siSliBase.cx)/m_dwVolumeMax, m_siSliBase.cy};

				int thumpx=slipx+(m_dwVolumeCur*m_siSliBase.cx)/m_dwVolumeMax-m_siSliThumb.cx/2;
				int thumpy=( m_iOverlayHeight-m_siSliThumb.cy)/2;

				RECT rcThumb = {thumpx, thumpy, thumpx+m_siSliThumb.cx, thumpy+m_siSliThumb.cy};

				int iconpx=( m_iOverlayWidth-m_siIconVolume.cx)/2;
				#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				RECT rcIconVolmune = {iconpx, 110, iconpx+m_siIconVolume.cx, 110+m_siIconVolume.cy};
				#else
				RECT rcIconVolmune = {iconpx, 100, iconpx+m_siIconVolume.cx, 100+m_siIconVolume.cy};
				#endif


				int iconVolumeChannelPx = (m_iOverlayWidth-m_siIconVolumeChannel.cx)/2;
				int iconVolumeChannelPy = (m_iOverlayHeight-m_siIconVolumeChannel.cy)/2;
				RECT rcIconVolmuneChannel = {iconVolumeChannelPx, iconVolumeChannelPy, iconVolumeChannelPx+m_siIconVolumeChannel.cx, iconVolumeChannelPy+m_siIconVolumeChannel.cy};

				//slider:
				if(m_pDDSSliBase)
					m_pDDSBack->AlphaBlt(&rcBase, m_pDDSSliBase,NULL, DDBLT_WAITNOTBUSY, NULL);
				if(m_pDDSSliActive)
					m_pDDSBack->AlphaBlt(&rcActive1, m_pDDSSliActive,&rcActive2, DDBLT_WAITNOTBUSY, NULL);
// 				if(m_pDDSSliThumb)
// 					m_pDDSBack->AlphaBlt(&rcThumb, m_pDDSSliThumb,NULL, DDBLT_WAITNOTBUSY, NULL);

				switch(m_iVolumeChannel)
				{
					case SB_VOLUME_CHANNEL_MEDIA:
						if(m_pDDSIconMedia)
						{
							m_pDDSBack->AlphaBlt(&rcIconVolmuneChannel, m_pDDSIconMedia,NULL, DDBLT_WAITNOTBUSY, NULL);
						}
						break;
					case SB_VOLUME_CHANNEL_SPEECH:
						if(m_pDDSIconSpeech)
						{
							m_pDDSBack->AlphaBlt(&rcIconVolmuneChannel, m_pDDSIconSpeech,NULL, DDBLT_WAITNOTBUSY, NULL);
						}
						break;
					case SB_VOLUME_CHANNEL_PHONE:
						if(m_pDDSIconPhone)
						{
							m_pDDSBack->AlphaBlt(&rcIconVolmuneChannel, m_pDDSIconPhone,NULL, DDBLT_WAITNOTBUSY, NULL);
						}
						break;
					default:
						if(m_pDDSIconMedia)
						{
							m_pDDSBack->AlphaBlt(&rcIconVolmuneChannel, m_pDDSIconMedia,NULL, DDBLT_WAITNOTBUSY, NULL);
						}
						break;
				}

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				RECT rcIconEQ = {m_iconEQPx, m_iconEQPy, m_iconEQPx+m_siEQType.cx, m_iconEQPy+m_siEQType.cy};

				if(m_bBtnEQClicked)
				{
					switch(m_idEQType)
					{
					case EQ_NONE:
						if(m_pDDSIconEQNoneFocus)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQNoneFocus,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_CLASSICAL:
						if(m_pDDSIconEQClassicFocus)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQClassicFocus,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_POP:
						if(m_pDDSIconEQPopFocus)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQPopFocus,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_ROCK:
						if(m_pDDSIconEQRockFocus)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQRockFocus,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_JAZZ:
						if(m_pDDSIconEQJazzFocus)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQJazzFocus,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_USER:
						if(m_pDDSIconEQUserFocus)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQUserFocus,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					}
				}
				else
				{
					switch(m_idEQType)
					{
					case EQ_NONE:
						if(m_pDDSIconEQNone)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQNone,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_CLASSICAL:
						if(m_pDDSIconEQClassic)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQClassic,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_POP:
						if(m_pDDSIconEQPop)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQPop,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_ROCK:
						if(m_pDDSIconEQRock)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQRock,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_JAZZ:
						if(m_pDDSIconEQJazz)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQJazz,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					case EQ_USER:
						if(m_pDDSIconEQUser)
							m_pDDSBack->AlphaBlt(&rcIconEQ, m_pDDSIconEQUser,NULL, DDBLT_WAITNOTBUSY, NULL);
						break;
					}
				}
#endif

				//icon:
				//Do not show mute icon even the current volume is zero.
				if(m_bMute/* || m_dwVolumeCur==0*/)
				{
					if(m_pDDSIconMute)
					{
						m_pDDSBack->AlphaBlt(&rcIconVolmune, m_pDDSIconMute,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
				}
// 				else
// 				{
// 					if(m_pDDSIconVolume)
// 						m_pDDSBack->AlphaBlt(&rcIconVolmune, m_pDDSIconVolume,NULL, DDBLT_WAITNOTBUSY, NULL);
// 				}

				//draw number:
				wchar_t str[8];
				swprintf_s(str,8,_T("%d"),m_dwVolumeCur);
				//SIZE si;
				//GetStringExtensionArt(str,&si);
				//DrawStringArt(m_pDDSBack,str,( m_iOverlayWidth-si.cx)/2,56);
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				RECT rc={0,45, m_iOverlayWidth, 45+60};
#else
				RECT rc={0,40, m_iOverlayWidth, 40+60};
#endif
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
				DrawStringGDI(m_pDDSBack,str,&rc,DT_CENTER|DT_VCENTER,26,RGB(255,255,255));
#elif (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
				DrawStringGDI(m_pDDSBack,str,&rc,DT_CENTER|DT_VCENTER,26,RGB(0,123,222));
#else
				DrawStringGDI(m_pDDSBack,str,&rc,DT_CENTER|DT_VCENTER,26,RGB(40,40,40));
#endif
			}


			//
			m_pDDSOverlay->AlphaBlt(NULL, m_pDDSBack,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
		}

		//m_pDDSOverlay->Flip(NULL,DDFLIP_WAITNOTBUSY);
	}

}
void CSmartBarVol::SetEQType(UINT idType)
{
	m_idEQType = idType;
}

void  CSmartBarVol::OnTouchDown(POINT *pt)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: %d,%d\r\n"),TEXT(__FUNCTION__),pt->x, pt->y));
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)	
	static RECT rcBtn_EQ = {g_iScreenWidth/2-256/2+m_iconEQPx, g_iScreenHeight/2-256/2+m_iconEQPy-10, g_iScreenWidth/2-256/2+m_iconEQPx+m_siEQType.cx, g_iScreenHeight/2-256/2+m_iconEQPy+m_siEQType.cy};
	m_bBtnEQClicked = PtInRect(&rcBtn_EQ,*pt);

	if(m_bBtnEQClicked)
	{
		//RETAILMSG(1, (TEXT("MSG:[%s]:m_bBtnEQClicked \r\n"),TEXT(__FUNCTION__)));
		m_bNeedUpdateFrame = TRUE;
	}
#endif
}

void  CSmartBarVol::OnTouchUp(POINT *pt)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: %d,%d \r\n"),TEXT(__FUNCTION__),pt->x, pt->y));
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)
	if(m_bBtnEQClicked)
	{
		RETAILMSG(1, (TEXT("MSG:[%s]:m_bBtnEQClicked \r\n"),TEXT(__FUNCTION__)));
		if(m_hMainWnd)
		{
			::PostMessage(m_hMainWnd,WM_SMARTBARVOL_EQ,NULL,NULL);
		}
	}

	m_bBtnEQClicked=FALSE;
	m_bNeedUpdateFrame = TRUE;
#endif
}

void  CSmartBarVol::OnTouchMove(POINT *pt)
{
	//RETAILMSG(1, (TEXT("MSG:[%s]: %d,%d \r\n"),TEXT(__FUNCTION__),pt->x, pt->y));

	//if(m_bBtnHomeClicked)
	//{
	//	m_dwLastShowTick = GetTickCount();
	//}
}

BOOL   CSmartBarVol::IsTouchOnObject(POINT *pt)
{
	static RECT rcBtnObject = {m_iOverlayPx, m_iOverlayPy, m_iOverlayPx+m_siSliBackground.cx, m_iOverlayPy+m_siSliBackground.cy};
	return PtInRect(&rcBtnObject,*pt);
}