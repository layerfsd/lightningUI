#include "PanelBluetoothSearchDevice.h"
#include "../../resource.h"

CPanelBluetoothSearchDevice::CPanelBluetoothSearchDevice(void)
{

}
CPanelBluetoothSearchDevice::~CPanelBluetoothSearchDevice(void)
{

}

BOOL CPanelBluetoothSearchDevice::IsReady()
{
	return CPanelGL::IsReady();

}

void CPanelBluetoothSearchDevice::OnEnterPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	CPanelGL::OnEnterPanel(iParam,idAni,lpAniPara);

	RETAILMSG(DEBUG_BT,(_T("@@@@@@@ CPanelBluetoothSearchDevice::OnEnterPanel iParam:%d\r\n"),iParam));

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

// 	MakeDefaultAni(ANIM_ROTATE_TOP_CLOCK_IN);

}

void CPanelBluetoothSearchDevice::OnQuitPanel(UINT iParam,UINT idAni,LPVOID lpAniPara)
{
	//default
	CPanelGL::OnQuitPanel(iParam,idAni,lpAniPara);

	//make animation:
	SetAlpha(1.0f);
	SetAlpha(0.0f,TRUE);

}

void CPanelBluetoothSearchDevice::Render()
{
	if(!BeginRender())return;

	m_tagBk.SetAlpha(m_fCurAlpha);
	m_tagBk.Render();



	EndRender();
}

BOOL CPanelBluetoothSearchDevice::Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry)
{
	CPanelGL::Initialize(id,pParent,pSkinManager,pTextGL,hWnd,entry);

	ShowBackground(FALSE);


	m_tagBk.Initialize(0,NULL,m_pSkinManager->GetTexture(TEXID_541));
	m_tagBk.SetShowScale(g_iScreenWidth/256.0f);


	return TRUE;
}
void CALLBACK CPanelBluetoothSearchDevice::OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara)
{
	RETAILMSG(DEBUG_BT,(_T("CPanelBluetoothSearchDevice OnNotify id:%d,msg:%d,para:%d,lpUserPara:%x\r\n"),idControl,wMsg,wPara,lpUserPara));

	CPanelBluetoothSearchDevice *panel=(CPanelBluetoothSearchDevice*)lpUserPara;

	if(panel)
	{
		panel->HandleControlMSG(idControl,wMsg,wPara,lpPara);
	}
}
void CPanelBluetoothSearchDevice::HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara)
{
	switch (idControl)
	{
	case IDC_BT_BTN_KEY_0:
		break;
	}
}

