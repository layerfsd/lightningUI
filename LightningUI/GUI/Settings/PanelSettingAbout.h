#pragma once
#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"
#include "SettingCommon.h"
#include "QRBarGL.h"
#include "../../AppConfigProxy/AppConfigProxy.h"

class CPanelSettingAbout :
	public CPanelGL
{
public:
	CPanelSettingAbout(void);
	~CPanelSettingAbout(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnMCUCmd(WPARAM wParam, LPARAM lParam);
    void LinkAppConfig(CAppConfigProxy *phAppConfig){m_pAppConfig = phAppConfig;}
	void OnDeviceChanged();
#if CVTE_EN_CPMAC
	void OnMACChanged();
#endif

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

private:
	void RequestUpdateMCU(BOOL bUpdate);
	void OnRequestUpdateMCUSucceed();
	void OnRequestUpdateMCUFailed();
	void GenerateQR();
	BOOL IsFileExists(LPCTSTR   lpszFileName);
	void PrepareUpdateOS();
	BOOL StoreFile( const wchar_t * path, const void * buf, size_t bufsize );

private:
	void SetNcmMacAdress();

private:
	CAppConfigProxy     *m_pAppConfig;

	CM_String		m_strOSVersion;
	CM_String		m_strAPPVersion;
	CM_String		m_strMCUVersion;
	CM_String		m_strHardwareVersion;
	CM_String       m_strHardwareID;
#if CVTE_EN_CPMAC
	CM_String       m_strCarPlayMAC;
#endif

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;
#endif

	CButtonGL	m_tagTitleOSVersion;
	CButtonGL	m_tagValueOSVersion;

	CButtonGL	m_tagTitleAppVersion;
	CButtonGL	m_tagValueAppVersion;

	CButtonGL	m_tagTitleMCUVersion;
	CButtonGL	m_tagValueMCUVersion;
	CButtonGL	m_btnUpdateNK;
	CButtonGL	m_btnUpdateMCU;
	
	//CButtonGL   m_btnUpdate;
	//CLabelGL    m_iconUpdate;

	CButtonGL	m_tagTitleHardwareVersion;
	CButtonGL	m_tagValueHardwareVersion;

	CButtonGL	m_tagTitleHardwareID;
	CButtonGL	m_tagValueHardwareID;

#if CVTE_EN_CPMAC
	CButtonGL	m_tagTitleCarPlayMAC;
	CButtonGL	m_tagValueCarPlayMAC;
#endif

	DWORD		m_dwMCUVersion;

	DWORD		m_dwUpdateMcuRequestFlag;
	BOOL			m_bOnRequestMcuUpdate;

	CButtonGL	m_tagQRBarBk;

    CButtonGL   m_btnFactoryActivate;
    CButtonGL   m_btnFactoryMode;

	CQRBarGL		m_QRBar;
	BOOL			m_bQRGenerated;
    DWORD       m_dwClickUpCount;
    DWORD       m_dwLastTouchTick;
    BOOL        m_FactoryActivate;
};

