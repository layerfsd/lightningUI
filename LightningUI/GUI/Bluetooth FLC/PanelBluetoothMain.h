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

	//notify:
	void ClearDeviceList();
	void AddPairedDevice(strDeviceInfo *dev);
	void AddInquiredDevice(strDeviceInfo *dev);
	void ClearHistoryList();
	void AddCallLog(strCallLog *log,BOOL bAdjustNameOrder);
	void ClearContactList();
	void AddContact(ContactInfo *contact);
	CONTACT_STATUS GetContactStatus();
	void OnUpdatePBFinish(int cnt);
	void OnUpdateCallHistoryFinish(int cnt);
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	void OnUpdateCallLogFinish(DWORD cnt);
	BOOL IsUserDownloadCallLogPermit();
#endif
	void OnIniStatus(BOOL bSucc);
	void OnNotifyPairResult(PAIR_RESULT result);

	void OnNotifyHfpConnected(BOOL bConnected);
	void OnNotifyA2dpConnected(BOOL bConnected);
	void OnNotifyDeviceInquiryFinish();
	void OnNotifyStartSyncContact();
	void OnNotifySetupInI(strSetupInI *ptIni);

	int CreateSearchResultList(const wchar_t *SearchString);
	void SetStoredContactStatus(BOOL bAvailable);

protected:
	static void CALLBACK OnNotify(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara,LPVOID lpUserPara);
	void HandleControlMSG(DWORD idControl,DWORD wMsg,DWORD wPara,LPVOID lpPara);


private:
	void SetShowType(BT_HOMEPAGE_TYPE type);
	void SetDialString(const wchar_t* pString);
	void Input(wchar_t c);
	void ClearInput(int i);
	void ShowContactDetail(BOOL bShow,ContactInfo* contact);
	void DeleteStoredInfo(strDeviceInfo *dev);

private:

	BT_HOMEPAGE_TYPE	m_type;

	CONTACT_STATUS m_statusContact;

	ConnectionStatus	m_hfStatus;
	BOOL		m_bContactManualDownload;
#if CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
	CONTACT_STATUS m_statusCallLog;
	BOOL        m_bCallLogManualDownload;
	BOOL		m_bUserDownloadCallLogPermit;
	DWORD   m_dwCallLogDownloadTick;
	CButtonGL	m_btnDownloadCallLog;
	CButtonGL	m_btnCancelDownloadCallLog;
#endif

	BOOL		m_bIniStatus;
	BOOL		m_dwIniTick;
	//

	BOOL		m_bConnected;

	BOOL		m_bDeviceSearching;

	DWORD	m_dwContactDownloadTick;
	DWORD	m_dwContactDownloadedCount;

	//controls:
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
	CLabelGL		m_iconTypeDefault;

	CButtonGL	m_tagNote;

	CLabelGL			m_iconOut;
	CLabelGL			m_iconIn;
	CLabelGL			m_iconMissed;

	CLabelGL			m_iconDevice;
	CLabelGL			m_iconHistory;
	CLabelGL			m_iconContact;
	CLabelGL			m_iconKeyboard;
	CLabelGL			m_iconDelete;

	CLabelGL			m_iconDeviceFocus;
	CLabelGL			m_iconHistoryFocus;
	CLabelGL			m_iconContactFocus;
	CLabelGL			m_iconKeyboardFocus;
	CLabelGL			m_iconDeleteFocus;

	CListBoxGL		m_boxLists;

	CListBoxGL		m_boxHistory;
	CListBoxGL		m_boxContact;
	CListBoxGL		m_boxContactDetail;

	CListBoxGL      m_boxContanctSearch;
	BOOL            m_bContanctSearching;

// 	CListBoxGL		m_boxDevicePaired;
// 	CListBoxGL		m_boxDeviceSearched;
	CListBoxGL		m_boxDeviceList;

	DWORD			m_dwShowContactDetailTick;

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

	CButtonGL	m_tagNum0;
	CButtonGL   m_tagNum2;
	CButtonGL   m_tagNum3;
	CButtonGL   m_tagNum4;
	CButtonGL   m_tagNum5;
	CButtonGL   m_tagNum6;
	CButtonGL   m_tagNum7;
	CButtonGL   m_tagNum8;
	CButtonGL   m_tagNum9;

#if (CVTE_DEF_UI_STYLE == CVTE_UI_STYLE_ALDINET)
	CButtonGL	m_tagBk;

	CLabelGL			m_iconOutFocus;
	CLabelGL			m_iconInFocus;
	CLabelGL			m_iconMissedFocus;

	CLabelGL			m_iconArrowOn;
	CLabelGL			m_iconArrowOff;

	CButtonGL	m_tagLine1;
	CButtonGL	m_tagLine2;
	CButtonGL	m_tagLine3;
	CButtonGL	m_tagLine4;
	CButtonGL	m_tagLine5;
	CButtonGL	m_tagLine6;
	CButtonGL	m_tagLine7;
	CButtonGL	m_tagLine8;
	CButtonGL	m_tagLine9;
#endif

	CLabelGL	m_iconSearchContactUp;
	CButtonGL	m_btnSearchContact;
#if BT_PHONEBOOK_AUTOSTORE
	CButtonGL	m_btnSyncContact;
#endif

	CLabelGL		m_iconDial;
	CButtonGL	m_btnDial;
	CButtonGL	m_tagKeyboardBk;
	CButtonGL	m_btnDelete;
	CM_String		m_strDialString;
	CButtonGL	m_tagDialString;
	CM_String		m_strRedialString;

	CButtonGL	m_btnDownloadPhonebook;
	CButtonGL	m_btnCancelDownloadPhonebook;

	BOOL			m_bDownloadingCalllog;
	BOOL			m_bStoredContactAvailable;
	BOOL			m_bSyncingData;
};

