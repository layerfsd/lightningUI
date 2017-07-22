#include "RadarBar.h"

CRadarBar::CRadarBar(void)
{
	for (int i=0;i<RADAR_COUNT;i++)
	{
		m_iRadarSingal[i]=0;
	}
}

CRadarBar::~CRadarBar(void)
{
}
BOOL CRadarBar::IsReady()
{
	return CControlGL::IsReady();

}
void CRadarBar::SetRadarSingal(INT index,INT iSingal)
{
	COLORGL cr_r={1.0,0.0,0.0,1.0};
	COLORGL cr_y={1.0,1.0,0.0,1.0};
	COLORGL cr_g={0.0,1.0,0.0,1.0};
	COLORGL *cr=&cr_g;

	VIEW_STATE vs;

	if(index<0 && index>=RADAR_COUNT)
	{
		return;
	}

	if(iSingal<0)
		iSingal=0;
	else if(iSingal>10)
		iSingal=10;

	if(iSingal<=2)
		cr=&cr_r;
	else if(iSingal<=4)
		cr=&cr_y;
	else
		cr=&cr_g;

	m_iRadarSingal[index]=iSingal;

	if(iSingal==0)
	{
		m_tagRadarBar[index].Show(FALSE);
		return;
	}
	else
	{
		m_tagRadarBar[index].Show(TRUE);
	}


// 	GLfloat radius=122.0f+60.0f*(iSingal/10.0f);
// 	GLfloat ang_start=15.0f;
// 	GLfloat ang_step=10.0f;
	GLfloat radius=250.0f+110.0f*(iSingal/10.0f);
	GLfloat ang_start=18.0f;
	GLfloat ang_step=12.0f;

	vs=m_tagRadarBar[index].GetCurViewState();

	if(index>=0 && index<RADAR_COUNT/2)
	{
		vs.fRotateZ=ang_start-ang_step*index;
		vs.fTransX=0-radius*sin(vs.fRotateZ*PI/180.0f);
		vs.fTransY=0+radius*cos(vs.fRotateZ*PI/180.0f);	
		vs.fScaleX=radius/180.0f;
	}
	else if(index>=RADAR_COUNT/2 && index<RADAR_COUNT)
	{
		vs.fRotateZ=-ang_start+ang_step*(index-RADAR_COUNT/2);
		vs.fTransX=0-radius*sin(-vs.fRotateZ*PI/180.0f);
		vs.fTransY=0-radius*cos(-vs.fRotateZ*PI/180.0f);	
		vs.fScaleX=radius/180.0f;
	}
	else
	{
		return;
	}

	m_tagRadarBar[index].SetLightColor(cr);
	m_tagRadarBar[index].SetIniViewState(&vs);
	m_tagRadarBar[index].Show(iSingal!=0);

}
void CRadarBar::Render()
{
// 	static CTexture *pTexVideoBack=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_400):NULL;
// 	static CTexture *pTexToolbar=m_pSkinManager?m_pSkinManager->GetTexture(TEXID_055):NULL;
// 	static COLORGL cr={1.0,1.0,1.0,1.0};

	if(!BeginRender())return;

	COLORGL cr={1.0,1.0,1.0,m_fCurAlpha};


	//radar:
	m_tagBarBack.Render();
	m_tagBarBack2.Render();

	m_tagRadarBack.SetAlpha(m_fCurAlpha);
	m_tagRadarBack.Render();
	for (int i=0;i<8;i++)
	{
		m_tagRadarBar[i].SetAlpha(m_fCurAlpha);
		m_tagRadarBar[i].Render();
	}


	EndRender();

}
BOOL CRadarBar::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CControlGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

// 	COLORGL cr={0.0,1.0,0.0,1.0};
// 	COLORGL cr_r={1.0,0.0,0.0,1.0};
// 	wchar_t caption[8];
// 	SIZE si={400,160};

	m_tagBarBack.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_342));
	m_tagBarBack.SetScale(3.0,2.6);
	m_tagBarBack.SetPos(0,300);

	VIEW_STATE vs;
	m_tagBarBack2.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_342));

	vs=m_tagBarBack2.GetCurViewState();
	vs.fRotateZ=-180;
	m_tagBarBack2.SetIniViewState(&vs);
	m_tagBarBack2.SetScale(3.0,2.6);
	m_tagBarBack2.SetPos(0,-300);

	//m_tagRadarBack.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_340));
	GLfloat texCoord_s01002[]={	
		784.0/1024.0, 0.0,	
		784.0/1024.0, 1.0,	
		1.0, 0.0,	
		1.0, 1.0};
	m_tagRadarBack.Initialize(0,this,m_pSkinManager->GetTexture(TEXID_362),texCoord_s01002);

	for (int i=0;i<RADAR_COUNT;i++)
	{
		m_tagRadarBar[i].Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_341));
		m_tagRadarBar[i].EnableLight(TRUE);

		SetRadarSingal(i,0);
	}

	return TRUE;
}


void CRadarBar::OnTouchDown(POINT *pt)
{
	
}
void CRadarBar::OnTouchUp(POINT *pt)
{
	
}
void CRadarBar::OnTouchMove(POINT *pt)
{

}
void CRadarBar::OnGesture(GESTUREINFO *pgi)
{

}

