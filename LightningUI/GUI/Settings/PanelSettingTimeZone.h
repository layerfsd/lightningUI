#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"

#define MAX_TIME_ZONE_CNT		150

class CPanelSettingTimeZone :
	public CPanelGL
{
public:
	CPanelSettingTimeZone(void);
	~CPanelSettingTimeZone(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);



private:
	void TimeZoneChanged();

	CButtonGL       m_btnOK;
	CButtonGL       m_btnCancel;

	CLabelGL	    m_iconSetting;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL		m_iconSettingFocus;
#endif

	CListBoxGL		m_boxTimeZone;

	TIME_ZONE_INFORMATION m_tziCurrent, m_tziSelected;
	int m_iTimeZoneCurIndex;
};

