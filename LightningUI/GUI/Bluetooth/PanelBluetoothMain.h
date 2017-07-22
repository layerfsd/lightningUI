#pragma once
#include "BluetoothCommon.h"

#include "../Common/ui_common.h"
#include "../../Communication/CommanderEx.h"


class CPanelBluetoothMain :
	public CPanelGL
{
public:
	CPanelBluetoothMain(void);
	~CPanelBluetoothMain(void);

	BOOL IsReady();
	void OnEnterPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);
	void OnQuitPanel(UINT iParam=0,UINT idAni=0,LPVOID lpAniPara=0);

	void OnGestureMsg(UINT idGesture);

	void Render();
	BOOL Initialize(UINT id,CPanelGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL,HWND hWnd,MODULEENTRY entry);

	void OnPairedDeviceListChanged();

	void OnPhoneBookDownloadFinish(const CContactList *pclist);
	void OnPhoneBookDownloadFailed();

	void OnA2dpConnected(BOOL bConnected);

	void OnHFStateChange(SnmConnectionState status);

	void UpdateCallHistory();

	void OnDeviceSearchFinished();

	void OnPairFailed();

	void OnDeviceConnectFinish(WPARAM wParam);

	void Disconnect();

	void DownloadPhonebook();
	void CancelDownloadPhonebook();
	void SetContactDownloadedCount(DWORD dwCount){m_dwContactDownloadedCount = dwCount;}

	void SetDeviceName(const wchar_t *pName){m_strDeviceName=pName;}

	CONTACT_STATUS GetPhonebookStatus();

	void OnBeginConnect();
	void OnBeginPair();
	void SetAutoConnectingFlag(BOOL bConnecting){m_bAutoConnecting = bConnecting;m_boxDeviceList.Enable(!m_bAutoConnecting && !m_bManualConnecting);}

	void OnDisconnectAllFinish();

private:
	static DWORD ConnectThreadPro(LPVOID lpParam);
	static DWORD PairThreadPro(LPVOID lpParam);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);

	void ShowContactDetail(BOOL bShow,const wchar_t* szName=NULL);

private:
	void SetShowType(BT_HOMEPAGE_TYPE type);
	void SetDialString(const wchar_t* pString);
	void Input(wchar_t c);
	void ClearInput(int i);


	void UpdateDeviceList();

	INT UpdateDetails(Contact *pContact);
	INT GetContactDetail(const wchar_t* szName);
	void QueryPairedDevice();

	void UnpairDevice(BTDEV *pbtd);
	void ConnectDevice(BTDEV *pbtd);
	void DisconnectDevice(BTDEV *pbtd);

	void QueryCallHistory();
	void AddRecord(CALLREC *pcr);

	void PhoneDial(const wchar_t *pstrDialNumber);

	void SearchDevice();
	void CancelSearchDevice();
	void QuerySearchedDevice();
	void PairDevice(BTDEV *pbtd);

	void RefreshContactList();

private:

	BT_HOMEPAGE_TYPE	m_type;

	CONTACT_STATUS m_statusContact;

	SnmConnectionState m_hfStatus;

	DWORD		m_dwContactDownloadedCount;
	DWORD		m_dwContactDownloadTick;

	CM_String		m_strDeviceName;

	BTDEV			*m_pCurSellectedDevice;
	BTDEV			*m_pCurConnectedDevice;
	BOOL			m_bManualConnecting;
	BOOL			m_bAutoConnecting;
	BOOL			m_bManualDisconnecting;

	CLabelGL		m_iconHomeUp;
	CLabelGL		m_iconHomeDown;
	CButtonGL		m_btnHome;

	CLabelGL		m_iconSearchDeviceUp;
	CLabelGL		m_iconSearchDeviceDown;
	CButtonGL		m_btnSearchDevice;

	UINT				m_idPickDeviceFor;
// 	CLabelGL		m_iconPairedDeviceUp;
// 	CLabelGL		m_iconPairedDeviceDown;
// 	CButtonGL		m_btnPairedDevice;

	CLabelGL		m_iconBT;
	CLabelGL		m_iconSearching;

	CLabelGL		m_iconTypeHome;
	CLabelGL		m_iconTypeMobile;
	CLabelGL		m_iconTypeWork;

	CButtonGL	m_tagNote;

	CLabelGL			m_iconOut;
	CLabelGL			m_iconIn;
	CLabelGL			m_iconMissed;

	CLabelGL			m_iconDevice;
	CLabelGL			m_iconHistory;
	CLabelGL			m_iconContact;
	CLabelGL			m_iconDelete;

	CLabelGL			m_iconDeviceFocus;
	CLabelGL			m_iconHistoryFocus;
	CLabelGL			m_iconContactFocus;
	CLabelGL			m_iconDeleteFocus;

	CListBoxGL		m_boxLists;

	CListBoxGL		m_boxHistory;
	CListBoxGL		m_boxContact;
	CListBoxGL		m_boxContactDetail;

// 	CListBoxGL		m_boxDevicePaired;
// 	CListBoxGL		m_boxDeviceSearched;
	CListBoxGL		m_boxDeviceList;

	DWORD			m_dwShowContactDetailTick;

	BOOL				m_bDeviceSearching;
	BOOL				m_bShowSearchedDevice;
	BOOL				m_bNeedRefreshPhonebook;

	const CContactList		*m_pContactList;

	//
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
	CButtonGL	m_btnStar;
	CButtonGL	m_btnWell;

	CLabelGL		m_iconSearchContactUp;
	CButtonGL	m_btnSearchContact;

	CLabelGL		m_iconDial;
	CButtonGL	m_btnDial;
	CButtonGL	m_tagKeyboardBk;
	CButtonGL	m_btnDelete;
	CM_String		m_strDialString;
	CButtonGL	m_tagDialString;
	CM_String		m_strRedialString;

	CButtonGL	m_btnDownloadPhonebook;
	CButtonGL	m_btnCancelDownloadPhonebook;

};

