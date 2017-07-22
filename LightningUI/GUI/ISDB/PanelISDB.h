#pragma once
#include <FantasyLib.h>
#include "../Common/ui_common.h"
#include "../../VIP/CVideoWindow.h"
#include "../../Communication/CommanderEx.h"

#if CVTE_EN_ISDB

#define DEBUG_ISDB 1

#define ISDB_LIST_BOX_ITEM_WIDTH   250
#define ISDB_LIST_BOX_ITEM_HEIGHT  80

enum ISDB_LIST_TYPE
{
	ISDB_LIST_NULL,
	ISDB_LIST_001,
	ISDB_LIST_002,
	ISDB_LIST_003,
	ISDB_LIST_004,
	ISDB_LIST_005,
	ISDB_LIST_006,
	ISDB_LIST_007,
	ISDB_LIST_008,
	ISDB_LIST_009,
	ISDB_LIST_010,
};

class CPanelISDB :
	public CPanelGL
{
public:
	CPanelISDB(void);
	~CPanelISDB(void);
	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();

	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	CButtonGL		m_btnHome;
	CLabelGL			m_tagDAB;
	CListBoxGL		m_boxMenu;
	CButtonGL                m_tagTitle;
	CTextGL	                m_pTextGL;
	ISDB_LIST_TYPE       m_BeSelect;
	DWORD                    m_isdbSignal;
	CSliderGL			m_sliISDBSignal;
};
	
#endif