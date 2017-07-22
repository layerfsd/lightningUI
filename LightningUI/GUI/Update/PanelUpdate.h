#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "../../Communication/CmdSender_McuUpdate.h"
#include "../Settings/SettingCommon.h"

#define DEBUG_UPDATE		1

class CPanelUpdate :
	public CPanelGL
{
public:
	CPanelUpdate(void);
	~CPanelUpdate(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnBack();

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);
	void OnMcuUpdateACK(INT ack);
	void OnMcuUpdateMsg(WPARAM wParam, LPARAM lParam);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	BOOL IsUpdateFileExists(LPCTSTR   lpszFileName) ;
	void StartMcuUpdate();
	static DWORD UpdateMcuThreadPro(LPVOID lpParam);
	void UpdateMcuProc();

	BOOL RequestUpdate();
	BOOL CancelUpdate();
	BOOL PushPack(BYTE *pBuff,INT iLen,INT iSendDelay=0);

private:

	HANDLE		m_hACKEvent;
	CmdSender_McuUpdate	*m_pCmdSender;
	HANDLE		m_hMCUReadyEvent;

	CM_String m_strNotification;
	CM_String m_strTitle;

	CSliderGL		m_sliProgress;

	BOOL			m_bUpdating;

	BOOL			m_bMCUReadyToUpdate;
};
