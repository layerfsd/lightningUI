//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//

#pragma once

#include <windows.h>
#include <vector.hxx>
#include <list.hxx>
#include <lmcons.h>
#include "ehm.h"
#include "Wlanapi.h"
#include "CmNet.h"
#include "cmcsp.h"
#include "cmcspwifi.h"
#include "iptypes.h"
#include "iphlpapi.h"
#include "Eapcfg.h"
#include <wincrypt.h>

 #include "EthernetLan.h"
#include "../Common/common.h"

// DEBUGZONEs
#define ZONE_FATAL        DEBUGZONE(0)
#define ZONE_ERROR        DEBUGZONE(1)
#define ZONE_WARN         DEBUGZONE(2)
#define ZONE_INFO         DEBUGZONE(3)
#define ZONE_TRACE        DEBUGZONE(4)
#define ZONE_VERBOSE      DEBUGZONE(5)

#define IPADDRESS_STRING_MAXLENGTH 16

// Number of previous signal strengths to consider for an average
#define SIGNAL_STRENGTH_AVERAGE_COUNT 4
// Signal strengths bar
#define SIGNAL_STRENGTH_BAR 20
// Max Signal strengths bars
#define MAX_SIGNAL_STRENGTH_BARS 4

// Timeout for the configuration change complete
#define WIFI_WAIT_FOR_CONFIG_CHANGE 5000

// Timeout for the thread to be started
#define WIFI_WAIT_FOR_THREAD_START 1000

// Indices array (the way sorting works, there are 16 categories as listed below:
//  {Known/Unknown}, {High, Low, Very low, no} Quality, {Open, Secure}
#define SORTING_INDICES_COUNT 16

// Key length standard for specific security type
#define WIFI_KEYLENGTH_WEP_CHAR1 5
#define WIFI_KEYLENGTH_WEP_CHAR2 13
#define WIFI_KEYLENGTH_WEP_HEX1 10
#define WIFI_KEYLENGTH_WEP_HEX2 26
#define WIFI_KEYLENGTH_WPAPSK_MINCHAR 8
#define WIFI_KEYLENGTH_WPAPSK_MAXCHAR 63
#define WIFI_KEYLENGTH_WPAPSK_HEX 64

// WiFi UX defaults
#define WIFI_UX_KNOWN_NETWORK               0x00000001
#define WIFI_UX_UNKNOWN_NETWORK             0x00000002
#define WIFI_UX_SIGNAL_QUALITY_VERY_HIGH    0x00000004
#define WIFI_UX_SIGNAL_QUALITY_HIGH         0x00000008
#define WIFI_UX_SIGNAL_QUALITY_LOW          0x00000010
#define WIFI_UX_SIGNAL_QUALITY_VERY_LOW     0x00000020
#define WIFI_UX_SIGNAL_QUALITY_NONE         0x00000040
#define WIFI_UX_SECURITY_ENABLED            0x00000080
#define WIFI_UX_SECURITY_DISABLED           0x00000100

// Wi-Fi Maximum list length
#define WIFI_UX_MAX_LIST_LENGTH         0xFFFF

// Connecting timeout
#define WIFI_TIMEOUT_CONNECTING 40000

// Scan timeout
#define WIFI_TIMEOUT_SCANNING 5000 // The msdn suggested scan timeout is 4 seconds. We make it to 5 seconds for sure. 
                                   // Here is the suggestion from msdn: 
                                   // "The application should then wait to receive the wlan_notification_acm_scan_complete notification or timeout after 4 seconds"

// Scan interval
#define WIFI_INTERVAL_SCANNING 30000

// Default value to index mapping
static DWORD DefaultToIndex[SORTING_INDICES_COUNT+1] = {
    // Default Value
    {(WIFI_UX_KNOWN_NETWORK       | WIFI_UX_SIGNAL_QUALITY_VERY_HIGH          | WIFI_UX_SECURITY_DISABLED)},
    {(WIFI_UX_KNOWN_NETWORK       | WIFI_UX_SIGNAL_QUALITY_HIGH               | WIFI_UX_SECURITY_DISABLED)},
    {(WIFI_UX_KNOWN_NETWORK       | WIFI_UX_SIGNAL_QUALITY_LOW                | WIFI_UX_SECURITY_DISABLED)},
    {(WIFI_UX_KNOWN_NETWORK       | WIFI_UX_SIGNAL_QUALITY_VERY_LOW           | WIFI_UX_SECURITY_DISABLED)},
    {(WIFI_UX_KNOWN_NETWORK       | WIFI_UX_SIGNAL_QUALITY_VERY_HIGH          | WIFI_UX_SECURITY_ENABLED)},
    {(WIFI_UX_KNOWN_NETWORK       | WIFI_UX_SIGNAL_QUALITY_HIGH               | WIFI_UX_SECURITY_ENABLED)},
    {(WIFI_UX_KNOWN_NETWORK       | WIFI_UX_SIGNAL_QUALITY_LOW                | WIFI_UX_SECURITY_ENABLED)},
    {(WIFI_UX_KNOWN_NETWORK       | WIFI_UX_SIGNAL_QUALITY_VERY_LOW           | WIFI_UX_SECURITY_ENABLED)},
    {(WIFI_UX_UNKNOWN_NETWORK     | WIFI_UX_SIGNAL_QUALITY_VERY_HIGH          | WIFI_UX_SECURITY_DISABLED)},
    {(WIFI_UX_UNKNOWN_NETWORK     | WIFI_UX_SIGNAL_QUALITY_HIGH               | WIFI_UX_SECURITY_DISABLED)},
    {(WIFI_UX_UNKNOWN_NETWORK     | WIFI_UX_SIGNAL_QUALITY_LOW                | WIFI_UX_SECURITY_DISABLED)},
    {(WIFI_UX_UNKNOWN_NETWORK     | WIFI_UX_SIGNAL_QUALITY_VERY_LOW           | WIFI_UX_SECURITY_DISABLED)},
    {(WIFI_UX_UNKNOWN_NETWORK     | WIFI_UX_SIGNAL_QUALITY_VERY_HIGH          | WIFI_UX_SECURITY_ENABLED)},
    {(WIFI_UX_UNKNOWN_NETWORK     | WIFI_UX_SIGNAL_QUALITY_HIGH               | WIFI_UX_SECURITY_ENABLED)},
    {(WIFI_UX_UNKNOWN_NETWORK     | WIFI_UX_SIGNAL_QUALITY_LOW                | WIFI_UX_SECURITY_ENABLED)},
    {(WIFI_UX_UNKNOWN_NETWORK     | WIFI_UX_SIGNAL_QUALITY_VERY_LOW           | WIFI_UX_SECURITY_ENABLED)},
    {(0xFFFFFFFF)}
};

#define SAFE_DELETE(x){if(x){delete x;x=NULL;}}
#define CLEAN_BSTR(x){if(x){::SysFreeString(x);x=NULL;}}

TCHAR const g_cszWiFiDefaultWEPKey[] = TEXT("*****");
TCHAR const g_cszWiFiDefaultWPAKey[] = TEXT("********");
TCHAR const g_cszWiFiPowerRegKeyPath[] = TEXT("SYSTEM\\CurrentControlSet\\Control\\Power\\State\\WiFiDevice");

typedef enum ADVANCED_SPINNER_TYPE
{
    AST_EAPTYPE,
    AST_KEYINDEX,
    AST_COUNT
};

typedef enum WIFI_KEYTYPE
{
    WIFI_KT_UNKNOWN = 0,
    WIFI_KT_KNOWN,
    WIFI_KT_NONE,
    WIFI_KT_PASSPHRASE_WEP,     // Can be 5, 13, or 29 ASCII characters OR 10, 26, or 58 hex characters
    WIFI_KT_PASSPHRASE_WPA,     // Can be 8 - 63 ASCII characters or 64 hex characters
    WIFI_KT_DOMAINUSERPASSWORD,
    WIFI_KT_COUNT
};


#define NWCTL_MAX_WEPK_MATERIAL   1024
//#define NWCTL_WEPK_PRESENT        0x0001  // specifies whether the configuration includes or not a WEP key
//#define NWCTL_WEPK_PASSPHRASE     0x0002  // the Key material is passPhrase type(Not networkKey type).
//#define NWCTL_WEPK_HEXTEXT        0x0004  // the Key material is in hex format, stored as text such as a string L"ac72f"
//#define NWCTL_ONEX_ENABLED        0x0010  // for this configuration 802.1X should be enabled
//#define NWCTL_BROADCAST_SSID      0x4000

enum WIFI_NETWORKDATAFLAGS
{
    WIFI_NDF_VALID_AVAILABLE        = 0x0001,
    WIFI_NDF_VALID_EXISTINGCONFIG   = 0x0002,
    WIFI_NDF_VALID_AUTOCONNECT      = 0x0004,
    WIFI_NDF_VALID_HIDDEN           = 0x0008,
    WIFI_NDF_VALID_ONEXENABLED      = 0x0010,
    WIFI_NDF_VALID_AUTHALGO         = 0x0020,
    WIFI_NDF_VALID_BSSTYPE          = 0x0040,
    WIFI_NDF_VALID_CIPHALGO         = 0x0080,
    WIFI_NDF_VALID_SSID             = 0x0100,   // If both SSID and SZSSID are specified, only SSID will be used.
    WIFI_NDF_VALID_SZSSID           = 0x0200,   // If at least one is set then ssid and szSsid will both be set to match
    WIFI_NDF_VALID_GUIDINTERFACE    = 0x0400,
    WIFI_NDF_VALID_PASSPHRASE       = 0x0800,
    WIFI_NDF_VALID_ENCRYPTEDKEY     = 0x1000,
    WIFI_NDF_VALID_KEY              = 0x2000,
    WIFI_NDF_VALID_EAPTYPE          = 0x4000
};

// Thesd not real WIFI_NETWORKDATAFLAGs in the sense that they shouldn't ever be set in dwFlags
#define WIFI_NDF_INVALID_ONEXENABLED  0x40000000
#define WIFI_NDF_INVALID_PASSPHRASE   0x80000000

typedef struct _WIFI_NETWORKDATA
{
    BOOL fAvailable;
    BOOL fExistingConfig;
    BOOL fAutoConnect;
    BOOL fHidden;
    BOOL fOneXEnabled;
    DOT11_AUTH_ALGORITHM authalgo;
    DOT11_BSS_TYPE bsstype;
    DOT11_CIPHER_ALGORITHM ciphalgo;
    DOT11_SSID ssid;
    DWORD dwFlags;
    GUID guidInterface;
    TCHAR szSsid[DOT11_SSID_MAX_LENGTH + 1];    // We provide this so callers don't have to keep on converting the ssid
    TCHAR szPassphrase[PWLEN + 1];
    TCHAR szUsername[UNLEN + 1];
    TCHAR szDomain[DNLEN + 1];
    ULONG iKey;
    EAP_METHOD_TYPE eapmethodtype;
    DWORD dwCredBlobLength;
    BYTE    *pbCredBlob;                   // Pointer to provider specific credential blob
    DWORD   dwAuthDataLen;                
    BYTE    *pbAuthData;                   // Pointer to provider specific config blob
} WIFI_NETWORKDATA;

typedef struct _NETWORK_ADPT_IPINFO {
    WCHAR    szAdapterName[MAX_PATH];
    BOOL    fUseDHCP;
       WCHAR   pszIpAddr[IPADDRESS_STRING_MAXLENGTH];
       WCHAR   pszSubnetMask[IPADDRESS_STRING_MAXLENGTH];
       WCHAR   pszGateway[IPADDRESS_STRING_MAXLENGTH];
       WCHAR   pszDNSAddr[IPADDRESS_STRING_MAXLENGTH];
       WCHAR   pszDNSAltAddr[IPADDRESS_STRING_MAXLENGTH];
       WCHAR   pszWINSAddr[IPADDRESS_STRING_MAXLENGTH];
       WCHAR   pszWINSAltAddr[IPADDRESS_STRING_MAXLENGTH];
} NETWORK_ADPT_IPINFO, *PNETWORK_ADPT_IPINFO;

class WiFiListSyncBlk
{
    public:
        HRESULT GetSyncSig(HANDLE hMutex)
        {
            HRESULT hr = S_OK;

            CBR(NULL!=hMutex);
            m_hMutex = hMutex;

            CBR(WAIT_TIMEOUT!=WaitForSingleObject(m_hMutex, WIFI_WAIT_FOR_CONFIG_CHANGE));

        Error:
            return hr;
        }

        ~WiFiListSyncBlk()
        {
            if (m_hMutex)
            {
                ReleaseMutex(m_hMutex);
            }
        }

    private:
        HANDLE m_hMutex;
};

#define WIFILIST_SYNCBLK(cs) \
    WiFiListSyncBlk listGuard; \
    CHR(listGuard.GetSyncSig(cs))

class WiFiUXLock
{
    public:
        void Lock(void) { EnterCriticalSection(&m_cs); }
        void Unlock(void) { LeaveCriticalSection(&m_cs); }    
        BOOL IsLocked(void) { return (m_cs.OwnerThread == (HANDLE)GetCurrentThreadId()); }

    protected:
        CRITICAL_SECTION m_cs;
};

/// <summary>
/// NetworkItem class - contains information about a single network and defines
/// actions (connect and edit) that can be applied to the network
/// </summary>
class WiFiNetworkItem : public WiFiUXLock
{
    public:
        WiFiNetworkItem();
        WiFiNetworkItem(__in PWLAN_AVAILABLE_NETWORK pWiFiNetworkdata, HANDLE hWlanClient, GUID &guidInterface, CM_SESSION_HANDLE hCMSession);
        ~WiFiNetworkItem();
        VOID Initialize(PWLAN_AVAILABLE_NETWORK pWiFiNetworkdata, BOOL fOverWrite);
        VOID InitializeCMsession(CM_SESSION_HANDLE hCMSession);

        // Methods
        bool Connect(BOOL bConfig=TRUE);
        bool Connect(PCWSTR pszPasskey, BOOL fHidden);
        bool Connect(PCWSTR pszUsername, PCWSTR pszPassword, BOOL fHidden);

		HRESULT ConnectWiFiNetwork();

		bool Disconnect();

        HRESULT GetNetworkNameText(LPTSTR* ppWiFiNetworkName);
        BOOL IsAuthenticated();
        BOOL Is8021xNetwork();
        WIFI_NETWORKDATA* GetNetworkConfigData();
        DWORD GetNetworkSignalStrength();
        HRESULT SetNetworkSignalStrength(DWORD dwSignalQuality);
        bool IsKnownNetwork();
        int GetActualNetworkType();
        PWLAN_AVAILABLE_NETWORK GetStackNetworkData();
        HRESULT GetIpProperties(PNETWORK_ADPT_IPINFO pIpPropInfo);
        HRESULT GetAdptIpProperties(PNETWORK_ADPT_IPINFO pIpPropInfo);

        CM_CONNECTION_STATE GetConnectionState();
        HRESULT UpdateConnectionState();
        VOID SetConnectionState(CM_CONNECTION_STATE cmcsNetwork);
        BOOL IsConnected(DWORD dwTimeOut);
        VOID ConfigureInternalWiFiNetworkData(BOOL fOverWrite);
        HRESULT ConfigureWiFiNetworkProfile(
            BOOL* pfSecTypeOneX, 
            BSTR pszKey, 
            BYTE bEapType, 
            LPCTSTR pszCertName, 
            BOOL* pfValidateServer
        );
        HRESULT UpdateInternalWiFiNetworkData(
            BOOL* pfSecTypeOneX, 
            BSTR pszKey, 
            BYTE bEapType, 
            LPCTSTR pszCertName, 
            BOOL* pfValidateServer
        );
        VOID RemoveConfiguration();
        VOID SetAvailable();
        VOID GetSzSSID(TCHAR *pszSsid);
        BOOL IsConfigDeleteable();
        VOID NotifyFailure();

    protected:

        // Member variables
        UINT m_cRef;
        PWLAN_AVAILABLE_NETWORK m_pWiFiNetworkdata;

    private:
        VOID InitializeWlanClientAndInterface();
//         HRESULT ConnectWiFiNetwork();

        static DWORD s_CMAcquireConnectionThread(WiFiNetworkItem *pNetworkItem);
        HRESULT CMAcquireConnectionThread();
        DWORD WaitForAvailability();
        HRESULT SetUsernameAndDomain(TCHAR *pszUNDN);

        WIFI_NETWORKDATA m_InternalWiFiNetworkData;
        CM_CONNECTION_CONFIG *m_pConfig;
        CM_CONNECTION_STATE m_CMConnectionState;
        CM_CONNECTION_HANDLE m_hConnection;
        HANDLE m_hEvent_Connected;
        HANDLE m_hEvent_FailedConnect;
        HANDLE m_hWlanClient;
        BOOL m_fSharedWlanClient;
        BOOL m_fguidInterfaceSet;
        GUID m_guidInterface;
        CM_SESSION_HANDLE m_hCMSession;
        BOOL m_fConnecting;
        HANDLE m_hAvailability;

        // Signal strength statistics related stuff...
        DWORD m_dwSignalStrengthReadingsSum;
        DWORD m_dwSignalStrengthReadingArrayIndex;
        DWORD m_dwPastSignalStrengthReadings[SIGNAL_STRENGTH_AVERAGE_COUNT];
};

class WiFiNetworkList : public WiFiUXLock
{
    public:
        WiFiNetworkList(HWND hwndNotify,WLAN_NOTIFICATION_CALLBACK lpfnCallback=NULL,PVOID pContext=NULL);
        ~WiFiNetworkList();
        static void s_WlanNotificationCallback(PWLAN_NOTIFICATION_DATA pNotifData, PVOID pContext);
        BOOL StartScan();
        BOOL IsScanComplete(DWORD dwTimeOut);
        HRESULT SetWiFiListUiDoneUpdating(BOOL fUIDoneUpdating);
        DWORD GetInitialCount();
        HRESULT GetWiFiNetworkItem(DWORD dwIndex, WiFiNetworkItem** ppWiFiNetworkItem);
        HRESULT SetWiFiNetUiCallbackWindowHandle(HANDLE* phWnd);
        HRESULT ClearWiFiNetUiCallbackWindowHandle();
		HRESULT GetWiFiNetworkItem(TCHAR *pszSSID, WiFiNetworkItem** ppWiFiNetworkItem);

        static DWORD WINAPI ListenForCMNotificationsThread(LPVOID lpv);
        void ListenForCMNotifications(void);
        HRESULT InsertNewEntry(WiFiNetworkItem* pWiFiNetworkItem, BOOL fForceInsert);
        HRESULT DeleteEntryConfigByName(LPCTSTR pszNetworkName);
        HRESULT RemoveEntry(WiFiNetworkItem** ppWiFiNetworkItem);

		WIFI_STATUS GetWifiStatus(){return m_statusWiFi;}

    private:
        void OnScanComplete();
        void UpdateWiFiNetworkList();
        void StartWiFiListener();
        void StopWiFiListener();
        void StartNotificationThread();
        void StopNotificationThread(void);
        void RegisterForCMNotifications();
        void UnregisterFromCMNotifications();
        HRESULT HandleWlanCallbackNotification(PWLAN_NOTIFICATION_DATA pNotifData);
        void StartNotificationListener();
        void StopNotificationListener();
        void InitializeWlanClientAndInterface();
        void UninitializeWlanClientAndInterface();
        DWORD GetAndUpdateIndices(WiFiNetworkItem *pNetworkItem);
        VOID GetFromAndToIndexForFlag(DWORD dwFlag, DWORD *pdwFrom, DWORD *pdwTo);
        VOID IncrementIndicesBetween(DWORD dwFromIndex, DWORD dwToIndex);
        VOID RemoveStaleEntries();
        VOID AddNewEntries();
        VOID SortNetworkList();
        HRESULT AppendCmConnections();

        DWORD HandleCMNotification();
        HRESULT GetNotification(CM_NOTIFICATION **ppNotif);
        BOOL ShouldShowNetwork(WiFiNetworkItem *pNetworkItem);
        VOID OnConnecting(DWORD dwItemIndex);
        VOID OnConnected(DWORD dwItemIndex, CM_RESULT cmResult);
        VOID OnDisconnected(DWORD dwItemIndex, CM_RESULT cmResult);
        VOID OnDisconnecting(DWORD dwItemIndex, CM_RESULT cmResult);
        DWORD FindNetworkItemByName(TCHAR *pszConnection);
        HRESULT GetConnectionDetails(CM_NOTIFICATION *pNotif, CM_CONNECTION_DETAILS **ppConnDetails);
        void ClearNetworkListInternal(BOOL fShutting);

    private:
        HANDLE m_hMutex;
		HWND	m_hwndNotify;
        ce::vector<WiFiNetworkItem*> m_rgNetworkItems;
        ce::vector<WiFiNetworkItem*> m_rgNetworkItemsOnReset;
        HANDLE m_hWlanClient;
        DWORD m_dwScanCounter; // counter of StartScan-OnScanComplete pair
        HANDLE m_hEvent_ScanComplete;
		WLAN_NOTIFICATION_CALLBACK m_lpfnCallback;
		PVOID m_lpfnCallbackContext;
        PWLAN_AVAILABLE_NETWORK_LIST m_pAvailableNetworkList;
        GUID m_guidInterface;
        BOOL m_fguidInterfaceSet;
        DWORD m_dwPrevWlanRegisterNotifType;
        CM_SESSION_HANDLE m_hCMSession;
        // Suite of indices for sorting
        DWORD m_dwNetworkIndicesArray[SORTING_INDICES_COUNT];
        BOOL m_fWiFiListenerAlreadyStarted;
        BOOL m_fNotificationThreadAlreadyStarted;
        HANDLE m_hNotificationThread;
        HANDLE m_hEvent_CMnotification;
        HANDLE m_hEvent_NotificationThreadReady;
        HANDLE m_hEvent_NotificationThreadExit;
        CM_NOTIFICATIONS_LISTENER_HANDLE m_hCMListener;
        // UI update complete flag
        BOOL m_fUIDoneUpdating;
		WIFI_STATUS	m_statusWiFi;
};

#define MAX_DISPLAY_CERTS 20

// Utils functions
HRESULT GetInterfaceInfo(__in HANDLE hClient, __deref_out PWLAN_INTERFACE_INFO pInterfaceInfo);
HRESULT SetAdptIpProperties(PNETWORK_ADPT_IPINFO pIpPropInfo);
HRESULT GetWiFiPowerState(BOOL *pWiFiOn);
HRESULT SetWiFiPowerState(BOOL fWiFiOn, WiFiNetworkList* pWiFiList);
HRESULT GetAvailableNetworkList(__in HANDLE hClient, __deref_out PWLAN_AVAILABLE_NETWORK_LIST *ppAvailableNetworkList, __out GUID *pguidInterface);
HRESULT GetAllCertNames(LPTSTR* ppszAllCertNamesArray, DWORD* pCntCertNames);
HRESULT GetCertName(PCCERT_CONTEXT pUserCert, LPTSTR* ppszCertName, DWORD* pCertNameLen);
HRESULT GetCertByName(LPCTSTR pszCertName, PCCERT_CONTEXT* ppUserCert);
HRESULT InsertAuthDataByCertName(__in WIFI_NETWORKDATA *pWiFiNetworkData, __in LPCTSTR pszCertName, __in BOOL* pfValidateServer);
HRESULT GetCertNameFromAuthData(
    __in WIFI_NETWORKDATA *pWiFiNetworkData, 
    __out LPTSTR* ppszCertName, 
    __out BOOL* pfValidateServer
);
DWORD GetValidateServerCertCheckFromEAP(DWORD dwEapType);
HRESULT SsidToString(__in DOT11_SSID *pSsid, __out_ecount(cchSsid) TCHAR *pszSsid, UINT cchSsid);
HRESULT StringToSsid(__in_ecount(DOT11_SSID_MAX_LENGTH+1) TCHAR *pszSsid, __out DOT11_SSID *pssid);
HRESULT SyncProfile(WIFI_NETWORKDATA *pNetworkData, BOOL fUpdateXmlDirection, BSTR* pstrXml);
HRESULT GetWiFiConfigByConnectionName(LPCTSTR pConnectionName, WIFI_NETWORKDATA *pWiFiData);
CM_RESULT CmUpdateConnectionConfigByName(__in const WCHAR* const pszConnection,
    CM_CONFIG_OPTION cmco,
    __in_bcount(cbConfig) CM_CONNECTION_CONFIG* pConfig,
    DWORD cbConfig);
HRESULT AllocAndGetXmlFromConnectionConfig(
    const CM_CONNECTION_CONFIG *pConfig,
    LPWSTR *pszXml);
HRESULT ConfigureWiFiNetwork(__inout WIFI_NETWORKDATA *pWiFiNetworkData);
BOOL IsOneXNetwork(__in const WIFI_NETWORKDATA *pNetworkData);
WIFI_KEYTYPE GetKeyType(__in const WIFI_NETWORKDATA *pNetworkData);
HRESULT CheckWepKey(__in const PWLAN_AVAILABLE_NETWORK pNetworkData, LPCTSTR pszKey, DWORD dwKeyLength);
HRESULT CheckWepKey(__in WIFI_NETWORKDATA* pNetworkData, LPCTSTR pszKey, DWORD dwKeyLength);

