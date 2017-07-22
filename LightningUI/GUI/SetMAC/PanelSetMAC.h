#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include <afxwin.h>

class CPanelSetMAC :
	public CPanelGL
{
public:
	CPanelSetMAC(void);
	~CPanelSetMAC(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

protected:

private:
	void AppendMACChar(wchar_t wch);
	void DeleteMACChar();
	void SetMAC();

	CButtonGL	m_btnNum0;
	CButtonGL	m_btnNum1;
	CButtonGL	m_btnNum2;
	CButtonGL	m_btnNum3;
	CButtonGL	m_btnNum4;
	CButtonGL	m_btnNum5;
	CButtonGL	m_btnNum6;
	CButtonGL	m_btnNum7;
	CButtonGL	m_btnNum8;
	CButtonGL	m_btnNum9;
	CButtonGL	m_btnNumA;
	CButtonGL	m_btnNumB;
	CButtonGL	m_btnNumC;
	CButtonGL	m_btnNumD;
	CButtonGL	m_btnNumE;
	CButtonGL	m_btnNumF;
	CButtonGL	m_btnDel;
	CButtonGL	m_btnOK;
	
	CAtlStringW	m_macstr;
};
