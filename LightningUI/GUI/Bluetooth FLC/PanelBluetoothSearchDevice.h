#pragma once
#include "BluetoothCommon.h"

#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"


class CPanelBluetoothSearchDevice :
	public CPanelGL
{
public:
	CPanelBluetoothSearchDevice(void);
	~CPanelBluetoothSearchDevice(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);


protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);


private:
	CLabelGL			m_tagBk;

};

