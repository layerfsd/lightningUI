#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"


class CPanelSettingLanguage :
	public CPanelGL
{
public:
	CPanelSettingLanguage(void);
	~CPanelSettingLanguage(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);



private:

	CButtonGL       m_btnOK;
	CButtonGL       m_btnCancel;

	CLabelGL	    m_iconSetting;
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CLabelGL		m_iconSettingFocus;
#endif

	CListBoxGL		m_boxLanguage;

	UINT				m_idLanguageToSet;

	CTextGL			*m_pTextGL;

};

