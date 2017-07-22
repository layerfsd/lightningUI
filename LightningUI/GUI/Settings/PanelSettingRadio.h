#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"
#include "../Radio/RadioCommon.h"

class CPanelSettingRadio :
	public CPanelGL
{
public:
	CPanelSettingRadio(void);
	~CPanelSettingRadio(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void UpdateLocal(BOOL bON);
	void UpdateRegionDisplay(DWORD dwRegion);
	void UpdateFMStopLevel(DWORD dwLevel);
	void UpdateAMStopLevel(DWORD dwLevel);
	void UpdateAFStopLevel(DWORD dwLevel);

	void SetLocal(BOOL bON);
	/*void SetRegion(DWORD dwRegion);*/
	void SetFMStopLevel(DWORD dwLevel);
	void SetAMStopLevel(DWORD dwLevel);
	void SetAFStopLevel(DWORD dwLevel);
	void ResetStopLevel();
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	void SetRDS(BOOL bON);
	void SetTA(BOOL bON);
	void SetAF(BOOL bON);
	void SetPTY(BOOL bON);

	void UpdateRDS(BOOL bON);
	void UpdateTA(BOOL bON);
	void UpdateAF(BOOL bON);
	void UpdatePTY(BOOL bON);
	void UpdatePTYid(ProgramType id);
#endif
private:
    BOOL        m_bRegionSpread;

// 	CButtonGL	m_tagTitleLocal;
// 	CSliderGL		m_sliLocal;

	CButtonGL	m_tagTitleStopLevel;
	CButtonGL	m_tagTitleStopLevelFM;
	CButtonGL	m_tagTitleStopLevelAM;
	CButtonGL	m_tagTitleStopLevelAF;
	CSliderGL	m_sliStopLevelFM;
	CSliderGL	m_sliStopLevelAM;
	CSliderGL	m_sliStopLevelAF;
	CButtonGL	m_btnResetStopLevel;

	//region
	CButtonGL	m_btnRadioRegion;
	CButtonGL	m_tagTitleRegion;

	//rds
#if (CVTE_EN_RDS || CVTE_EN_RBDS)
	CButtonGL	m_tagTitleRDS;
	CButtonGL	m_tagTitleRDSSwitch;
	CSliderGL   m_sliRDSSwitch;

	CButtonGL	m_tagTitleAF;
	CSliderGL   m_sliAF;
	CButtonGL   m_tagTitleTA;
	CSliderGL   m_sliTA;

	CButtonGL	m_btnPTY;
	CSliderGL   m_sliPTY;
#endif

#if CVTE_EN_DAB
	CButtonGL	m_tagTitleDAB;

	CButtonGL	m_tagTitleServiceLink;
	CSliderGL   m_sliServiceLink;

	CButtonGL	m_tagTitleLBand;
	CSliderGL   m_sliLBand;

	CButtonGL	m_tagDABTitleTA;
	CSliderGL   m_sliDABTA;

	CButtonGL	m_tagDABAntennaPower;
	CSliderGL   m_sliDABAntennaPower;

#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif
//	    int         m_iFMStopLevel;
//	    int         m_iAMStopLevel;
};

