#include "PanelVehicleInfoRadar.h"
#include "../../resource.h"



//CPanelVehicleInfoRadar
CPanelVehicleInfoRadar::CPanelVehicleInfoRadar(void)
{

}
CPanelVehicleInfoRadar::~CPanelVehicleInfoRadar(void)
{

}

BOOL CPanelVehicleInfoRadar::IsReady()
{
	return CControlGL::IsReady();
}
void CPanelVehicleInfoRadar::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	//make animation:
	if(idAni != ANIM_NONE)
	{
		SetAlpha(0.1f);
		SetAlpha(1.0f,TRUE);
	}
	else
	{
		SetAlpha(1.0f);
	}
}
void CPanelVehicleInfoRadar::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}
void CPanelVehicleInfoRadar::Render()
{

	if(!BeginRender())return;

	m_barRadar.SetAlpha(m_fCurAlpha);
	m_barRadar.Render();

	EndRender();

}
void CPanelVehicleInfoRadar::SetInfoRadar(INT id,INT iSignal)
{
	m_barRadar.SetRadarSingal(id,iSignal);
}
BOOL CPanelVehicleInfoRadar::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	VIEW_STATE vs;
	m_barRadar.Initialize(0,this,pSkinManager,pTextGL,hWnd,entry);
	vs=m_barRadar.GetCurViewState();
	vs.fRotateZ=90;
	m_barRadar.SetIniViewState(&vs);

	m_barRadar.SetPos(0,0);
	m_barRadar.SetShowScale(1.0);
	for (int i=0;i<8;i++)
	{
		m_barRadar.SetRadarSingal(i,/*i+3*/0);
	}

	return TRUE;
}


