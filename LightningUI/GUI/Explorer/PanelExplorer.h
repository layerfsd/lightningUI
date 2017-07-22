#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"


#define DEBUG_EXPLORER		0


class CPanelExplorer :
	public CPanelGL
{
public:
	CPanelExplorer(void);
	~CPanelExplorer(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void SetNotification(EXPLORERMSG *pMsg);

	#if CVTE_EN_KEY_PROXY
	BOOL OnKey(UINT idKey,UINT idStatus);
	#else
	BOOL OnKey(UINT idKey);
	#endif

public:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

protected:

private:
	CExplorerGL	m_Explorer;

	UINT m_idCaller;
	NOTIFYPROC m_pCallbackFunc;
	LPVOID m_lpCallbackParam;
	CM_String m_strTitle;
	CM_String m_strPath;
	CM_String m_strFilter;

	//toolbar:
// 	CLabelGL			m_iconHomeUp;
// 	CLabelGL			m_iconHomeDown;
// 	CLabelGL			m_iconReturnUp;
// 	CLabelGL			m_iconReturnDown;
//
 	CButtonGL		m_btnHome;

};
