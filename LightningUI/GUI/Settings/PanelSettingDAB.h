#pragma once
#include "../Common/ui_common.h"
#include "SettingCommon.h"

class CPanelSettingDAB :
	public CPanelGL
{
public:
	CPanelSettingDAB(void);
	~CPanelSettingDAB(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnDabServiceStartReady(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	CButtonGL	m_tagTitleServiceLink;
	CSliderGL   m_sliServiceLink;

	CButtonGL	m_tagTitleTA;
	CSliderGL   m_sliTA;

	CButtonGL	m_tagTitleLBand;
	CSliderGL   m_sliLBand;

};

