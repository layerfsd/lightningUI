#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../VIP/CVideoWindow.h"
#include "../../Communication/CommanderEx.h"

#if CVTE_EN_AUX
#define DEBUG_AUX		1

enum SelectMode {
	SelectMode_AUX = 0,
	SelectMode_BT,
	SelectMode_DVD,
	SelectMode_USB
};
class CPanelAUX :
	public CPanelGL
{
public:
	CPanelAUX(void);
	~CPanelAUX(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void LinkVideoWindow(CVideoWindow *pVideoWindow){m_pVideoWindow = pVideoWindow;}

	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif
    void OnSpeechStart();
    void OnSpeechTerminate();
    void OnPhoneCallState(WPARAM wParam, LPARAM lParam);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);
	static void CALLBACK OnVideoWndGesture(WPARAM wParam, LPARAM lParam,LPVOID lpContext);

private:

	CVideoWindow				*m_pVideoWindow;

	CLabelGL			m_tagBackEq;

	CLabelGL			m_tagBkBlur;
	CLabelGL			m_tagBkOrigin;

	CButtonGL		m_btnHome;

	CButtonGL		m_tagTitle;
	CButtonGL		m_tagTips;
	CButtonGL		m_tagRight;
	CButtonGL		m_tagLeft;
	CButtonGL		m_tagAUX_B;
	CButtonGL		m_tagBT;
	CButtonGL		m_tagDVD;
	CButtonGL		m_tagUSB;
	
#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_BOSSAUDIOUI)

	CLabelGL		m_tagAUX;
	CLabelGL		m_tagAUXBox;
	CLabelGL		m_tagAUXTitle;	

	//CButtonGL       m_tagAux1;
	//CButtonGL       m_tagAux2;
	//CButtonGL       m_tagAux3;

#else

	CLabelGL			m_iconAUX;
	CLabelGL			m_iconAUX2;
#endif

	DWORD			m_dwEnterTick;

    	BOOL            m_bPhoneCallActive;
	BOOL			m_bSpeechOn;
	
	SelectMode   m_BeSelect;
	BOOL            m_BeShow;
};

#endif
