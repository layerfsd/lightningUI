#pragma once
#include "../Common/ui_common.h"
#include "../Settings/SettingCommon.h"

#define DEBUG_DEV_MGR						1

enum DEV_TYPE
{
	DEV_NULL,
	DEV_USBDISK,
	DEV_SDMEDIACARD,
	DEV_SDGPSCARD,
	DEV_IAP,
	DEV_PHONELINK,
	DEV_MHL,
	DEV_USBDISK2,
	DEV_DECK,
	DEV_EASYCONNECTED,
};

class CPanelDevConnectMgr :
	public CPanelGL
{
public:
	CPanelDevConnectMgr(void);
	~CPanelDevConnectMgr(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnTouchDown(POINT *pt);
	void OnTouchUp(POINT *pt);
	void OnTouchMove(POINT *pt);


	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);

protected:
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

protected:

private:
	BOOL IsDeviceReady(DEV_TYPE device);
	BOOL IsFileExists(LPCTSTR   lpszFileName);

private:
	CButtonGL		m_tagTitle;

	DWORD			m_dwEnterTick;

	BOOL				m_bInsert;
	DEV_TYPE			m_idDeviceType;
	BOOL				m_bChecked;

	CLabelGL			m_iconUSB;
	CLabelGL			m_iconSD;
	CLabelGL			m_iconIAP;

	CLabelGL			m_iconLoading;

};
