// CSmartBarPhone.cpp : implementation of the CSmartBarPhone class
//
#include "SmartBarPhone.h"
#if CVTE_EN_KEY_PROXY
#include "../Key/CKeyHandler.h"
#endif

extern HWND					g_hUIWnd;

CSmartBarPhone::CSmartBarPhone()
{
	// TODO: add construction code here

	m_pDDSBackground = NULL;

	//talking:
	m_pDDSSymbos = NULL ; //
	m_pDDSBtnAnswerUp = NULL ; //
	m_pDDSBtnAnswerDown = NULL ; //
	m_pDDSBtnHangUp = NULL ; //
	m_pDDSBtnHangDown = NULL ; //
	m_pDDSBtnACHUp = NULL ; //
	m_pDDSBtnACHDown = NULL ; //
	m_pDDSBtnACHPhoneUp = NULL ; //
	m_pDDSBtnACHPhoneDown = NULL ; //

	m_bBtnAnswerClicked = FALSE;
	m_bBtnHangClicked = FALSE;
	m_bBtnACHClicked = FALSE;


	m_idType = SB_NULL;
	m_idAudioDir = 1;
}

CSmartBarPhone::~CSmartBarPhone()
{

}
void CSmartBarPhone::SetAudioDir(UINT idAudioDir)
{
	m_idAudioDir=idAudioDir;
	m_bNeedUpdateFrame = TRUE;
}
void CSmartBarPhone::SetType(SMARTBAR_PHONE_TYPE type)
{
	RETAILMSG(DEBUG_SMARTBAR, (TEXT("---CSmartBarPhone SetType: %d--- \r\n"),type));

	m_idType = type;
	m_bNeedUpdateFrame = TRUE;
}
void CSmartBarPhone::SetPhoneNumber(const wchar_t*pstrNumber)
{
	RETAILMSG(DEBUG_SMARTBAR, (TEXT("---CSmartBarPhone SetPhoneNumber: %s--- \r\n"),pstrNumber));

	m_strPhoneNumber = pstrNumber;

	m_bNeedUpdateFrame = TRUE;
}

BOOL CSmartBarPhone::Initialize(CSkinManagerGL *pSkinManager,UINT id,HWND hWnd,HINSTANCE hIns)
{
	CSmartBar::Initialize(pSkinManager,id,hWnd,hIns,0,g_iScreenHeight-100,g_iScreenWidth,100,TRUE/*FALSE*/,_T("_SMARTBAR_PHONE_"));

	LoadRes(L"\\ResidentFlash\\LightningUI\\Skins\\Skin_phone.skn");
// 	LoadRes(L"\\ResidentFlash\\Skin_phone.skn");
// 	LoadRes(L"\\MEDIA CARD\\Skin_phone.skn");

	return TRUE;
}
BOOL CSmartBarPhone::LoadRes(LPCTSTR lpFileName)
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
		case SB_RES_BACKGROUND:
			FillSurface(&m_pDDSBackground,cursor,cx,cy,depth);
			break;
		case SB_RES_SYMBOS:
			FillSurface(&m_pDDSSymbos,cursor,cx,cy,depth);
			break;

			//talking
		case SB_RES_BTN_ANSWER_UP:
			FillSurface(&m_pDDSBtnAnswerUp,cursor,cx,cy,depth);
			m_siBtnAnswer.cx=cx;
			m_siBtnAnswer.cy=cy;
			break;
		case SB_RES_BTN_ANSWER_DOWN:
			FillSurface(&m_pDDSBtnAnswerDown,cursor,cx,cy,depth);
			break;
		case SB_RES_BTN_HANG_UP:
			FillSurface(&m_pDDSBtnHangUp,cursor,cx,cy,depth);
			m_siBtnHang.cx=cx;
			m_siBtnHang.cy=cy;
			break;
		case SB_RES_BTN_HANG_DOWN:
			FillSurface(&m_pDDSBtnHangDown,cursor,cx,cy,depth);
			break;
		case SB_RES_BTN_ACH_UP:
			FillSurface(&m_pDDSBtnACHUp,cursor,cx,cy,depth);
			m_siBtnACH.cx=cx;
			m_siBtnACH.cy=cy;
			break;
		case SB_RES_BTN_ACH_DOWN:
			FillSurface(&m_pDDSBtnACHDown,cursor,cx,cy,depth);
			break;
		case SB_RES_BTN_ACH_PHONE_UP:
			FillSurface(&m_pDDSBtnACHPhoneUp,cursor,cx,cy,depth);
			break;
		case SB_RES_BTN_ACH_PHONE_DOWN:
			FillSurface(&m_pDDSBtnACHPhoneDown,cursor,cx,cy,depth);
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
void CSmartBarPhone::DrawChar(LPDIRECTDRAWSURFACE pSurface,wchar_t c,int x,int y,SIZE *si)
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
VOID CSmartBarPhone::ReleaseRes()
{
	if (m_pDDSBackground != NULL)
	{
		m_pDDSBackground->Release();
		m_pDDSBackground = NULL;
	}


	//talking:
	if (m_pDDSBtnAnswerUp != NULL)
	{
		m_pDDSBtnAnswerUp->Release();
		m_pDDSBtnAnswerUp = NULL;
	}
	if (m_pDDSBtnAnswerDown != NULL)
	{
		m_pDDSBtnAnswerDown->Release();
		m_pDDSBtnAnswerDown = NULL;
	}

	if (m_pDDSBtnHangUp != NULL)
	{
		m_pDDSBtnHangUp->Release();
		m_pDDSBtnHangUp = NULL;
	}
	if (m_pDDSBtnHangDown != NULL)
	{
		m_pDDSBtnHangDown->Release();
		m_pDDSBtnHangDown = NULL;
	}

	if (m_pDDSBtnACHUp != NULL)
	{
		m_pDDSBtnACHUp->Release();
		m_pDDSBtnACHUp = NULL;
	}
	if (m_pDDSBtnACHDown != NULL)
	{
		m_pDDSBtnACHDown->Release();
		m_pDDSBtnACHDown = NULL;
	}

	if (m_pDDSBtnACHPhoneUp != NULL)
	{
		m_pDDSBtnACHPhoneUp->Release();
		m_pDDSBtnACHPhoneUp = NULL;
	}
	if (m_pDDSBtnACHPhoneDown != NULL)
	{
		m_pDDSBtnACHPhoneDown->Release();
		m_pDDSBtnACHPhoneDown = NULL;
	}

	if (m_pDDSSymbos != NULL)
	{
		m_pDDSSymbos->Release();
		m_pDDSSymbos = NULL;
	}

}

void  CSmartBarPhone::OnTouchDown(POINT *pt)
{
	pt->y-=380;
	RETAILMSG(DEBUG_SMARTBAR, (_T("**CSmartBarPhone::OnTouchDown :%d,%d\r\n"),pt->x,pt->y));

	if(m_idType == SB_INCOMING)
	{
		int btn_answer_px=0;
		int btn_answer_py=( m_iOverlayHeight-m_siBtnAnswer.cy)/2;
		RECT rcAnswer = {btn_answer_px, btn_answer_py, btn_answer_px+m_siBtnAnswer.cx, btn_answer_py+m_siBtnAnswer.cy};

		int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
		int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
		RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};

		m_bBtnAnswerClicked = PtInRect(&rcAnswer,*pt);
		m_bBtnHangClicked = PtInRect(&rcHang,*pt);

		if(m_bBtnAnswerClicked || m_bBtnHangClicked)
		{
			m_bNeedUpdateFrame = TRUE;
		}
	}
	else if(m_idType == SB_OUTGOING)
	{
		int btn_ach_px=0;
		int btn_ach_py=( m_iOverlayHeight-m_siBtnACH.cy)/2;
		RECT rcAch = {btn_ach_px, btn_ach_py, btn_ach_px+m_siBtnACH.cx, btn_ach_py+m_siBtnACH.cy};

		int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
		int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
		RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};

		m_bBtnACHClicked = PtInRect(&rcAch,*pt);
		m_bBtnHangClicked = PtInRect(&rcHang,*pt);

		if(m_bBtnACHClicked || m_bBtnHangClicked)
		{
			m_bNeedUpdateFrame = TRUE;
		}
	}
	else if(m_idType == SB_TALKING)
	{
		int btn_ach_px=0;
		int btn_ach_py=( m_iOverlayHeight-m_siBtnACH.cy)/2;
		RECT rcAch = {btn_ach_px, btn_ach_py, btn_ach_px+m_siBtnACH.cx, btn_ach_py+m_siBtnACH.cy};

		int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
		int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
		RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};

		m_bBtnACHClicked = PtInRect(&rcAch,*pt);
		m_bBtnHangClicked = PtInRect(&rcHang,*pt);

		if(m_bBtnACHClicked || m_bBtnHangClicked)
		{
			m_bNeedUpdateFrame = TRUE;
		}
	}
#if CVTE_EN_KEY_PROXY
	if(m_bBtnAnswerClicked)
	{
		CKeysHandler::GetInstance()->KeyDown(KEY_PHONE_ANSWER);
	}
	else if(m_bBtnACHClicked)
	{
		CKeysHandler::GetInstance()->KeyDown(KEY_PHONE_ACH);
	}	
	else if(m_bBtnHangClicked)
	{
		CKeysHandler::GetInstance()->KeyDown(KEY_PHONE_HANG);
	}
#endif	
}
void  CSmartBarPhone::OnTouchUp(POINT *pt)
{
#if CVTE_EN_KEY_PROXY
	pt->y-=380;

	RETAILMSG(DEBUG_SMARTBAR, (_T("**CSmartBarPhone::OnTouchUp :%d,%d\r\n"),pt->x,pt->y));

	m_bNeedUpdateFrame = TRUE;

	if(m_idType == SB_INCOMING)
	{
		if(m_bBtnAnswerClicked)
		{
			int btn_answer_px=0;
			int btn_answer_py=( m_iOverlayHeight-m_siBtnAnswer.cy)/2;
			RECT rcAnswer = {btn_answer_px, btn_answer_py, btn_answer_px+m_siBtnAnswer.cx, btn_answer_py+m_siBtnAnswer.cy};
			CKeysHandler::GetInstance()->KeyUp(KEY_PHONE_ANSWER, !PtInRect(&rcAnswer,*pt));
		}
		if(m_bBtnHangClicked)
		{
			int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
			int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
			RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};
			CKeysHandler::GetInstance()->KeyUp(KEY_PHONE_HANG, !PtInRect(&rcHang,*pt));
		}

		m_bBtnAnswerClicked = FALSE;
		m_bBtnHangClicked = FALSE;
	}
	else if(m_idType == SB_OUTGOING)
	{
		if(m_bBtnACHClicked)
		{
			int btn_ach_px=0;
			int btn_ach_py=( m_iOverlayHeight-m_siBtnACH.cy)/2;
			RECT rcAch = {btn_ach_px, btn_ach_py, btn_ach_px+m_siBtnACH.cx, btn_ach_py+m_siBtnACH.cy};
			CKeysHandler::GetInstance()->KeyUp(KEY_PHONE_ACH, !PtInRect(&rcAch,*pt));
		}
		if(m_bBtnHangClicked)
		{
			int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
			int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
			RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};
			CKeysHandler::GetInstance()->KeyUp(KEY_PHONE_HANG, !PtInRect(&rcHang,*pt));
		}

		m_bBtnACHClicked = FALSE;
		m_bBtnHangClicked = FALSE;
	}
	else if(m_idType == SB_TALKING)
	{
		if(m_bBtnACHClicked)
		{
			int btn_ach_px=0;
			int btn_ach_py=( m_iOverlayHeight-m_siBtnACH.cy)/2;
			RECT rcAch = {btn_ach_px, btn_ach_py, btn_ach_px+m_siBtnACH.cx, btn_ach_py+m_siBtnACH.cy};
			CKeysHandler::GetInstance()->KeyUp(KEY_PHONE_ACH, !PtInRect(&rcAch,*pt));
		}
		if(m_bBtnHangClicked)
		{
			int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
			int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
			RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};
			CKeysHandler::GetInstance()->KeyUp(KEY_PHONE_HANG, !PtInRect(&rcHang,*pt));
		}

		m_bBtnACHClicked = FALSE;
		m_bBtnHangClicked = FALSE;
	}
#else
	m_bNeedUpdateFrame = TRUE;

	if(m_idType == SB_INCOMING)
	{
		if(m_bBtnAnswerClicked)
		{
			::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_ANSWER,NULL);
		}
		if(m_bBtnHangClicked)
		{
			::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
			//Show(FALSE);
		}

		m_bBtnAnswerClicked = FALSE;
		m_bBtnHangClicked = FALSE;
	}
	else if(m_idType == SB_OUTGOING)
	{
		if(m_bBtnACHClicked)
		{
			::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_ACH,NULL);
		}
		if(m_bBtnHangClicked)
		{
			::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
			//Show(FALSE);
		}

		m_bBtnACHClicked = FALSE;
		m_bBtnHangClicked = FALSE;
	}
	else if(m_idType == SB_TALKING)
	{
		if(m_bBtnACHClicked)
		{
			::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_ACH,NULL);
		}
		if(m_bBtnHangClicked)
		{
			::PostMessage(g_hUIWnd,WM_PHONE_KEY,KEY_PHONE_HANG,NULL);
			//Show(FALSE);
		}

		m_bBtnACHClicked = FALSE;
		m_bBtnHangClicked = FALSE;
	}
#endif	
}
void  CSmartBarPhone::OnTouchMove(POINT *pt)
{

}

void CSmartBarPhone::Render()
{
	//test:
	if(m_pDDSOverlay)
	{
		if(m_pDDSBack)
		{
			if(m_pDDSBackground)
				m_pDDSBack->AlphaBlt(NULL, m_pDDSBackground,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);

			if(m_idType == SB_INCOMING)
			{
				//incoming:
				int btn_answer_px=0;
				int btn_answer_py=( m_iOverlayHeight-m_siBtnAnswer.cy)/2;
				RECT rcAnswer = {btn_answer_px, btn_answer_py, btn_answer_px+m_siBtnAnswer.cx, btn_answer_py+m_siBtnAnswer.cy};

				int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
				int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
				RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};

				if(m_bBtnAnswerClicked)
				{
					if(m_pDDSBtnAnswerDown)
						m_pDDSBack->AlphaBlt(&rcAnswer, m_pDDSBtnAnswerDown,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else
				{
					if(m_pDDSBtnAnswerUp)
						m_pDDSBack->AlphaBlt(&rcAnswer, m_pDDSBtnAnswerUp,NULL, DDBLT_WAITNOTBUSY, NULL);
				}

				if(m_bBtnHangClicked)
				{
					if(m_pDDSBtnHangDown)
						m_pDDSBack->AlphaBlt(&rcHang, m_pDDSBtnHangDown,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else
				{
					if(m_pDDSBtnHangUp)
						m_pDDSBack->AlphaBlt(&rcHang, m_pDDSBtnHangUp,NULL, DDBLT_WAITNOTBUSY, NULL);
				}

				//
				RECT rc={0,0, m_iOverlayWidth, m_iOverlayHeight};
				DrawStringGDI(m_pDDSBack,m_strPhoneNumber.String(),&rc,DT_CENTER|DT_VCENTER);
			}
			else if(m_idType == SB_OUTGOING)
			{
				//outgoing:
				int btn_ach_px=0;
				int btn_ach_py=( m_iOverlayHeight-m_siBtnACH.cy)/2;
				RECT rcAch = {btn_ach_px, btn_ach_py, btn_ach_px+m_siBtnACH.cx, btn_ach_py+m_siBtnACH.cy};

				int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
				int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
				RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};

				if(m_bBtnACHClicked)
				{
					if(m_idAudioDir!=0)//BT
					{
						if(m_pDDSBtnACHDown)
							m_pDDSBack->AlphaBlt(&rcAch, m_pDDSBtnACHDown,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
					else
					{
						if(m_pDDSBtnACHPhoneDown)
							m_pDDSBack->AlphaBlt(&rcAch, m_pDDSBtnACHPhoneDown,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
				}
				else
				{
					if(m_idAudioDir!=0)//BT
					{
						if(m_pDDSBtnACHUp)
							m_pDDSBack->AlphaBlt(&rcAch, m_pDDSBtnACHUp,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
					else
					{
						if(m_pDDSBtnACHPhoneUp)
							m_pDDSBack->AlphaBlt(&rcAch, m_pDDSBtnACHPhoneUp,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
				}

				if(m_bBtnHangClicked)
				{
					if(m_pDDSBtnHangDown)
						m_pDDSBack->AlphaBlt(&rcHang, m_pDDSBtnHangDown,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else
				{
					if(m_pDDSBtnHangUp)
						m_pDDSBack->AlphaBlt(&rcHang, m_pDDSBtnHangUp,NULL, DDBLT_WAITNOTBUSY, NULL);
				}

				//
				RECT rc={0,0, m_iOverlayWidth, m_iOverlayHeight};
				DrawStringGDI(m_pDDSBack,m_strPhoneNumber.String(),&rc,DT_CENTER|DT_VCENTER);

			}
			else if(m_idType == SB_TALKING)
			{
				//talking:
				int btn_ach_px=0;
				int btn_ach_py=( m_iOverlayHeight-m_siBtnACH.cy)/2;
				RECT rcAch = {btn_ach_px, btn_ach_py, btn_ach_px+m_siBtnACH.cx, btn_ach_py+m_siBtnACH.cy};

				int btn_hang_px= m_iOverlayWidth-m_siBtnHang.cx;
				int btn_hang_py=( m_iOverlayHeight-m_siBtnHang.cy)/2;
				RECT rcHang = {btn_hang_px, btn_hang_py, btn_hang_px+m_siBtnHang.cx, btn_hang_py+m_siBtnHang.cy};

				if(m_bBtnACHClicked)
				{
					if(m_idAudioDir!=0)//BT
					{
						if(m_pDDSBtnACHDown)
							m_pDDSBack->AlphaBlt(&rcAch, m_pDDSBtnACHDown,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
					else
					{
						if(m_pDDSBtnACHPhoneDown)
							m_pDDSBack->AlphaBlt(&rcAch, m_pDDSBtnACHPhoneDown,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
				}
				else
				{
					if(m_idAudioDir!=0)//BT
					{
						if(m_pDDSBtnACHUp)
							m_pDDSBack->AlphaBlt(&rcAch, m_pDDSBtnACHUp,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
					else
					{
						if(m_pDDSBtnACHPhoneUp)
							m_pDDSBack->AlphaBlt(&rcAch, m_pDDSBtnACHPhoneUp,NULL, DDBLT_WAITNOTBUSY, NULL);
					}
				}

				if(m_bBtnHangClicked)
				{
					if(m_pDDSBtnHangDown)
						m_pDDSBack->AlphaBlt(&rcHang, m_pDDSBtnHangDown,NULL, DDBLT_WAITNOTBUSY, NULL);
				}
				else
				{
					if(m_pDDSBtnHangUp)
						m_pDDSBack->AlphaBlt(&rcHang, m_pDDSBtnHangUp,NULL, DDBLT_WAITNOTBUSY, NULL);
				}

				//
				RECT rc={0,0, m_iOverlayWidth, m_iOverlayHeight};
				DrawStringGDI(m_pDDSBack,m_strPhoneNumber.String(),&rc,DT_CENTER|DT_VCENTER);

			}
			//
			m_pDDSOverlay->AlphaBlt(NULL, m_pDDSBack,NULL, DDBLT_WAITNOTBUSY|DDABLT_NOBLEND, NULL);
		}

		//m_pDDSOverlay->Flip(NULL,DDFLIP_WAITNOTBUSY);
	}
}
