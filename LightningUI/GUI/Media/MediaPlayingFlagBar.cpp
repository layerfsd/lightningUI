#include "MediaPlayingFlagBar.h"
#include "../../Config/customer.h"

CMediaPlayingFlagBar::CMediaPlayingFlagBar(void)
{
	m_iBarCount = 5;

	for (int i=0;i<5;i++)
	{
		m_iValueCur[0]=0;
		m_iValueSet[0]=0;
	}
	m_bValueChangeReady = TRUE;
	m_bClicked = FALSE;
}

CMediaPlayingFlagBar::~CMediaPlayingFlagBar(void)
{
}

BOOL CMediaPlayingFlagBar::Initialize(UINT id,CControlGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL)
{
	m_pSkinManager=pSkinManager;
	m_ID=id;
	m_pTextGL=pTextGL;
	SetParent(pParent);

	//
	SetSize(52,36,FALSE);

	return TRUE;
}

void CMediaPlayingFlagBar::Render(void)
{
	if(!BeginRender())return;


	if(m_bEnableRender && IsVisible() )
	{
		static DWORD s_last_value=0;

// 		static COLORGL cr={176.0f/255.0f,110.0f/255.0f,142.0f/255.0f,1.0};
// 		static COLORGL cr_down={219.0f/255.0f,54.0f/255.0f,116.0f/255.0f,1.0};
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
		static COLORGL cr={0.0f/255.0f,122.0f/255.0f,223.0f/255.0f,1.0};
#else
		static COLORGL cr={208.0f/255.0f,55.0f/255.0f,61.0f/255.0f,1.0};
#endif
		static COLORGL cr_down={255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,1.0};

		if(GetTickCount()-s_last_value>500)
		{
			s_last_value=GetTickCount();

			srand(GetTickCount());

			for (int i=0;i<5;i++)
			{
				m_iValueSet[i]=(rand()%m_siCurSize.cy);
			}
		}

		int cx=5;

		for (int i=0;i<5;i++)
		{
			if(abs(m_iValueCur[i]-m_iValueSet[i])>5)
			{
				m_iValueCur[i]+=(m_iValueSet[i]-m_iValueCur[i])/5;
			}
			else
			{
				m_iValueCur[i]=m_iValueSet[i];
			}
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
			DrawRect(-15+6*i,-m_siCurSize.cy/2+m_iValueCur[i]/2,cx,m_iValueCur[i],m_bClicked?&cr_down:&m_crCurColor);
#else
			DrawRect(-16+7*i,-m_siCurSize.cy/2+m_iValueCur[i]/2,cx,m_iValueCur[i],m_bClicked?&cr_down:&m_crCurColor);
#endif
		}
	}
	EndRender();
}
BOOL CMediaPlayingFlagBar::IsReady(void)
{
	return CControlGL::IsReady() /*&& m_bValueChangeReady*/;
}

void CMediaPlayingFlagBar::OnTouchDown(POINT *pt)
{
	m_ptLastPress = *pt;

	if(IsPtnOnObject(pt))
	{
// 		SetShowScale(1.2f);
		m_bClicked = TRUE;
	}
}
void CMediaPlayingFlagBar::OnTouchUp(POINT *pt)
{
	if(IsPtnOnObject(pt) && IsPtnOnObject(&m_ptLastPress))
	{
		if(m_lpNotifyFunc)
		{
			m_lpNotifyFunc(m_ID,NOTIFY_SHORT_CLICK,0,NULL,m_lparam);
		}
	}

// 	if(m_bClicked)
// 	{
// 		SetShowScale(1.0f);
// 	}

	m_bClicked = FALSE;
}
void CMediaPlayingFlagBar::OnTouchMove(POINT *pt)
{

}

BOOL CMediaPlayingFlagBar::IsPtnOnObject(POINT *pt)
{
	RECT rc=m_rcScreenRect;

	rc.left-=20;
	rc.right+=20;
	rc.top-=30;
	rc.bottom+=30;

	return PtInRect(&rc,*pt) /*&& IsVisible()*/ && IsEnabled();
}