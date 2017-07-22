#pragma once
#include "../../StdAfx.h"
#include "api_base.h"

#include <winsock2.h>
#include "svsutil.hxx"
#include <bt_api.h>
#include <bthapi.h>
#include <bt_buffer.h>
#include <bt_ddi.h>
#include <pm.h>
#include <cmthread.h>
#include "bluetooth_gap.h"


//#define DYNAMIC_WPP_LIB
#define WPP_LIB_NAME                            TEXT("wpplib")
#define BTP_UI_EXT_DLL_NAME                     TEXT("btp_ui_ext.dll")
#define WPP_LIB_PATH                            TEXT("SOFTWARE\\CSR\\") TEXT("Applications\\") TEXT("WPPAPP")
#define WPP_PB_ATTRIBUTE_FILTER                 TEXT("filter")

#define BTH_SC_DB_KEY                           TEXT("comm\\security\\bluetooth")
#define BT_LINK_KEY_RECORD_FORMAT               TEXT("Record%04x%02x%06x")                      

#define DUNNETWORK_NAME_STRING_MAXLEN 30

#define PIN_REQUEST_STORE_TIMEOUT   10000
// [SfWE] SSP confirmation timeout is limited as 29 sec by default.
#define PIN_REQUEST_PROC_TIMEOUT    30000

// [SfWE] Bluetooth profile's connection timeout in millisecond.
#define BT_CONNECTION_TIMEOUT       30000

#define THREAD_EXIT_TIMEOUT         15000

#define BT_ENABLE_THREAD_DELAY      1000

#define BT_POWER_DEVICE             L"BTT0:"

//bt enum

#define BTENUM_CHANNEL_NOT_USED     0xfc
#define BTENUM_MAX_NAME             248
#define BTENUM_MAX_PORT_NAME        16


/* +++ Macro for auto-connect SPP. +++ */
#define AUTO_CONNECT_SPP_DATA(sppIndex, reason)     MAKELONG(sppIndex, reason)

#define AUTO_CONNECT_SPP_INDEX_IN_EVENT(data)       LOWORD(data)
#define AUTO_CONNECT_SPP_REASON_IN_EVENT(data)      HIWORD(data)
/* --- Macro for auto-connect SPP. --- */


enum
{
    BTENUM_DEVICE_MODEM = 0,
    BTENUM_DEVICE_HANDSFREE_AG,
    BTENUM_DEVICE_PHONEBOOK_PSE,
    BTENUM_DEVICE_TPMS,
    BTENUM_DEVICE_MAP_MSE,
    BTENUM_DEVICE_A2DP,
    BTENUM_DEVICE_AVRCP,
    BTENUM_DEVICE_SPP,
    BTENUM_DEVICE_HID,

    BTENUM_CLASSES,
};

enum BTH_PAIR_RESPONSE_TYPE
{
    BTH_PAIR_ACCEPT = 0,    /* User accept pairing. */
    BTH_PAIR_REFUSE,        /* User reject pairing. */
    BTH_PAIR_TERMINATED,    /* Pairing is terminated by remote side. */
};

/* +++ GAP API function pointer +++ */
typedef DWORD (WINAPI *PFN_GapRegister)(GapRegisterInfoT* registerInfo);
typedef DWORD (WINAPI *PFN_GapUnregister)();
typedef DWORD (WINAPI *PFN_GapSetLocalName)(BluetoothDeviceName deviceName);
typedef DWORD (WINAPI *PFN_GapEnableUnderTestMode)();
typedef DWORD (WINAPI *PFN_GapGetDeviceRecord)(BT_ADDR deviceAddr, BluetoothDeviceRecord* deviceRecord);
typedef DWORD (WINAPI *PFN_GapEnumDeviceRecord)(BluetoothDeviceRecord* deviceRecordList, DWORD recordCount, DWORD* actualCount);
/* --- GAP API function pointer --- */

static LPWSTR gszKeyNames[BTENUM_CLASSES + 1] =
{
    TEXT("DUN"),
    TEXT("HFP"),
    TEXT("PBAP"),
    TEXT("TPMS"),
    TEXT("MAP"),
    TEXT("A2DP"),
    TEXT("AVRCP"),
    TEXT("SPP"),
    TEXT("HID"),
    TEXT("ALL")
};


extern BOOL GetBA(WCHAR *pp, BT_ADDR *pba);

extern BOOL GetConnectedDeviceOne(DWORD dwService = BTENUM_DEVICE_HANDSFREE_AG );

struct BTSERVICE
{
    BTSERVICE *pNext;
    int         iDeviceClass;   //0-9, -1 for "UNKNOWN"
    GUID        service_id;     //service GUID
    unsigned char   ucChannel;  //RFCOMM channel for every service
    int             imtu;
    unsigned char   *psdp;
    unsigned int    csdp;
    unsigned int fActive   : 1;     // currently active
    unsigned int fAuth     : 1;     // connection must be authenticated
    unsigned int fEncrypt  : 1;     // connection must be encrypted
    int tpms_tire;                      // tpms tire matching

    TCHAR       szDUNNetworkName[DUNNETWORK_NAME_STRING_MAXLEN];    //dun network configuration name
    DWORD       dwHidDevClass;  //just for HID device
    WCHAR       szPortName[BTENUM_MAX_PORT_NAME];   //just for modem,like BSP2:
    HANDLE      hDevHandle;     //modem have a non-zero handle in registry

    int BthEnumGetClassDefaults(int iClass, BTSERVICE *pbts);
    BTSERVICE(int iclass)
    {
        HEAP_STATISTICS_CLASS_ALLOC(BTSERVICE)

        memset(this, 0, sizeof(*this));
        iDeviceClass = iclass;
        ucChannel = BTENUM_CHANNEL_NOT_USED;
        BthEnumGetClassDefaults(iclass, this);
    }
    BTSERVICE(BTSERVICE *pbts)
    {
        HEAP_STATISTICS_CLASS_ALLOC(BTSERVICE)

        *this = *pbts;
        psdp = (unsigned char *)LocalAlloc(LMEM_FIXED, csdp);

        if (psdp)
        {
            memcpy(psdp, pbts->psdp, csdp);
        }
    }
    BTSERVICE()
    {
        HEAP_STATISTICS_CLASS_ALLOC(BTSERVICE)

        memset(this, 0, sizeof(*this));
    }
    ~BTSERVICE()
    {
        HEAP_STATISTICS_CLASS_FREE(BTSERVICE)

        if (psdp)
            LocalFree(psdp);
    }
};

struct BTDEV
{
    BTDEV       *pNext;
    WCHAR       szDeviceName[BTENUM_MAX_NAME + 1];
    BT_ADDR     b;          // Address of the item
    unsigned int fKey  : 1;     // have key
    unsigned int fPin  : 1;     // have pin
    unsigned int fActive  : 1;  // connected
    BTSERVICE *pServiceList;
    CRITICAL_SECTION m_cs;

    BTDEV(BT_ADDR bt)
    {
        HEAP_STATISTICS_CLASS_ALLOC(BTDEV)

        memset(this, 0, sizeof(*this));
        b = bt;
        InitializeCriticalSection(&m_cs);
    }

    BTDEV()
    {
        HEAP_STATISTICS_CLASS_ALLOC(BTDEV)

        memset(this, 0, sizeof(*this));
        InitializeCriticalSection(&m_cs);
    }

    ~BTDEV(void)
    {
        HEAP_STATISTICS_CLASS_FREE(BTDEV)

        FreeService();
        DeleteCriticalSection(&m_cs);
    }

    
    void Lock() //const
    {
        EnterCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }
    void Unlock() //const
    {
        LeaveCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }
    

    //NOTE: default use the parameter pointer directly, don't free it in caller
    BTSERVICE *AddService(BTSERVICE *pbts, BOOL bNew = FALSE)
    {
        if (bNew)
        {
            BTSERVICE *pTemp = new BTSERVICE(pbts);

            if (pTemp == NULL)
            {
                BTDBG(ZONE_ERROR, (TEXT("BTDEV::AddService ERROR \r\n")));
                return NULL;
            }

            Lock();
            pTemp->pNext = pServiceList;
            pServiceList = pTemp;
            Unlock();
            return pTemp;
        }
        else
        {
            Lock();
            pbts->pNext = pServiceList;
            pServiceList = pbts;
            Unlock();
            return pbts;
        }
    }

    BTSERVICE *FindService(int iDeviceClass)
    {
        BTSERVICE *pbts;

        Lock();

        for (pbts = pServiceList; pbts != NULL; pbts = pbts->pNext)
        {
            if (pbts->iDeviceClass == iDeviceClass)
                break;
        }

        Unlock();

        return pbts;
    }

    void FreeService()
    {
        BTSERVICE *pTemp;

        Lock();

        if (pServiceList == NULL)
        {
            Unlock();
            return;
        }

        for (pTemp = pServiceList; pTemp != NULL; pTemp = pServiceList)
        {
            pServiceList = pServiceList->pNext;
            delete pTemp;
        }

        Unlock();
    }

    int GetServiceClass(USHORT serviceId);
};


struct CBTDevList
{
    BTDEV *m_pbtdList;
    DWORD m_dwCntList;
    CRITICAL_SECTION m_cs;

    CBTDevList(): m_pbtdList(NULL), m_dwCntList(0)
    {
        HEAP_STATISTICS_CLASS_ALLOC(CBTDevList)

        InitializeCriticalSection(&m_cs);
    }
    ~CBTDevList()
    {
        HEAP_STATISTICS_CLASS_FREE(CBTDevList)

        Free();
        DeleteCriticalSection(&m_cs);
    }
    void Lock() const
    {
        EnterCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }
    void Unlock() const
    {
        LeaveCriticalSection((LPCRITICAL_SECTION)&m_cs);
    }

    //NOTE:default use the parameter pointer directly, don't free it in caller
    BTDEV *Add(BTDEV *pb, BOOL bNew = FALSE);
    BOOL Remove(BTDEV *pb, BOOL bDelete = TRUE);
    void Free();
    BTDEV *Find(BTDEV *pb) const;
    BTDEV *Find(BT_ADDR b) const;   //FindTrustedDevice
};


class CApi_SearchPair :
    public CApi_base
{
public:
    CApi_SearchPair(void): m_hev(NULL), m_hLookup(NULL), m_hThreadSearch(NULL), m_hThreadConnectAll(NULL), 
        m_hThreadPair(NULL), m_hThreadBTN(NULL), m_hRequestBTN(NULL), m_bSearchExit(FALSE), m_dwSearchUuids(0), 
        m_pAutoConnectThread(NULL), m_dwPinCode(0), m_pEnableBluetoothThread(NULL),
        m_pfnGapRegister(NULL), m_pfnGapUnregister(NULL), m_pfnGapSetLocalName(NULL),
        m_pfnGapEnableUnderTestMode(NULL), m_pfnGapGetDeviceRecord(NULL), m_pfnGapEnumDeviceRecord(NULL),
        m_pAutoConnectSppThread(NULL)
    {
        ZeroMemory(m_searchUUIDs, sizeof(m_searchUUIDs));
    }

    ~CApi_SearchPair(void)
    {
    }

    virtual BOOL Initialize();
    virtual void DeInitialize();

    BOOL InitFuncPtr();
    void DumpFuncPtr();

    //ce stack event notifications
    HANDLE m_hMsgQ;
    HANDLE m_hRequestBTN;
    HANDLE m_hThreadBTN;

    //BTNotify thread
    static DWORD BTNotify_thread(CApi_SearchPair *pApiSearchPair)
    {
        return pApiSearchPair->Thread_run_BTNotify();
    }
    DWORD Thread_run_BTNotify();

    //BT mode
    int SetMode(LPTSTR lpsz);
    int GetMode(LPTSTR lpsz);

    //BT Power
    DWORD IsBTPowerOff(DWORD *pIsOff);
    BOOL SetBTOn();
    BOOL SetBTOff();
    void EnableBluetooth(BOOL enable = TRUE);

    void BthEnumInit(void); //read registry, initialize global variables:mtu/auth/encrypt[] of all classes including "UNKNOWN"
    BOOL BthEnumDevices();  //enumerate all devices under HLM\\software\\microsoft\\bluetooth\\device\\<classes>\\ including "UNKNOWN"(class=-1) and add to m_btdPairedList
    BOOL BthEnumDeviceRecord();//enumerate device record stored in BT stack.
    BOOL BthGetSecurityInfo();//  get the list of authenticated devices which we paired with but did not discover by enumerate,and also fill auth info to paired devices
    BOOL BthEnumUpdate(BTDEV *bt);//add device to key:HLM\\software\\microsoft\\bluetooth\\device\\<class>\\<bt_addr_string>, if class =-1, add to "UNKNOWN"
    BOOL BthEnumRemove(BTDEV *bt);//remove device from  key:HLM\\software\\microsoft\\bluetooth\\device\\<class>\\<bt_addr_string>
    BOOL BthClearSecurityInfo();    //revoke all pin and key
    BOOL BthWriteLocalName(LPTSTR szBluetoothName); //write Bluetooth local name through standard HCI command of "HCI_Change_Local_Name (0x0c13)"

    //Search
    HANDLE m_hLookup;
    HANDLE m_hThreadSearch;
    BOOL m_bSearchExit;
    const CBTDevList *m_pbtdSearchList;

    BOOL SearchDevice();
    BOOL SearchAllServices(BTDEV *pbtd);
    void SearchAllServices();
    void SearchDeviceCancel();
    static DWORD Search_device_thread(CApi_SearchPair *pApiSearchPair)
    {
        return pApiSearchPair->Thread_run_Search_device();
    }
    DWORD Thread_run_Search_device();

    //Pair
    HANDLE m_hThreadPair;
    BTDEV *m_pbtdPairing;

    DWORD Pair(BTDEV *pbtd);
    void PairCancel();
    static DWORD Pair_thread(CApi_SearchPair *pApiSearchPair)
    {
        return pApiSearchPair->Thread_run_Pair();
    }
    DWORD Thread_run_Pair();

    //Get paired device
    BOOL GetPairedDevice();
    BOOL DelPairedDevice(BTDEV *pbt);
    const CBTDevList *m_pbtdPairedList;

    //Pair request
    HANDLE m_hev;           //the event for pin request
    HANDLE m_hevPairResp;   //the event for user response
    DWORD m_dwPinCode;      //SSP comparison pin code

    DWORD GetPinCode()              { return m_dwPinCode; }
    void SetPinCode(DWORD pinCode)  { m_dwPinCode = pinCode; }
    BOOL ValidSspPinCode()          { return ValidSspPinCode(GetPinCode()); }
    BOOL ValidSspPinCode(DWORD pinCode) { return pinCode != 0 && pinCode != (DWORD) -1 ? TRUE : FALSE; }

    int AnswerPair(BT_ADDR *pba);
    int RefusePair(BT_ADDR *pba, BOOL local = TRUE);

    BOOL ValidLinkKey(BT_LINK_KEY_EVENT *plke);

    static DWORD Get_remot_pin_request_thread(CApi_SearchPair *pApiSearchPair)
    {
        return pApiSearchPair->Thread_run_Get_remot_pin_request();
    }
    DWORD Thread_run_Get_remot_pin_request();

    //Connect/disconnect to device
    DWORD ConnectDevice(BTDEV *pbtd);
    DWORD DisconnectDevice(BTDEV *pbtd);
    void ConncectCancel(BTDEV *pbtd);   //cancel connect  when timeout or user cancel
    BOOL AddPairedDevice(BTDEV *pbtd);  //add device to pairedList 
    BOOL UpdateDeviceName(BTDEV *pbtd); //update remote device name
    
    //Init remote Bluetooth service to retrieve according to local SDP.
    SdpQueryUuid m_searchUUIDs[MAX_UUIDS_IN_QUERY];     //UUIDs to retrieve
    DWORD m_dwSearchUuids;                              //Total number of UUIDs to retrieve

    BOOL InitSearchService();
    void ShowErrorString(int iErr);
    
    //Init page timeout used in searching device.
    void InitPageTimeout();

    //One connect all
    HANDLE m_hThreadConnectAll;
    BOOL ConnectDeviceAll(BTDEV *pbtd);
    BTDEV* m_pbtdConnectingAll;
    static DWORD Connect_device_all_thread(CApi_SearchPair *pApiSearchPair)
    {
        return pApiSearchPair->Thread_run_Connect_device_all();
    }
    DWORD Thread_run_Connect_device_all();

    void ConnectDeivceAllCancel();
    DWORD DisconnectDeviceAll(BTDEV *pbtd);

    void StartAutoConnect(BT_ADDR bluetoothAddress = 0, UCHAR reason = 0);
    void SetAutoConnectFlag();
    void StopAutoConnect();
    /* +++ GAP API +++ */
    DWORD Register(GapRegisterInfoT* registerInfo)
    {
        CHECK_FUNC_PTR(m_pfnGapRegister);
        return m_pfnGapRegister(registerInfo);
    }
    DWORD Unregister()
    {
        CHECK_FUNC_PTR(m_pfnGapUnregister);
        return m_pfnGapUnregister();
    }
    DWORD SetLocalName(BluetoothDeviceName deviceName)
    {
        CHECK_FUNC_PTR(m_pfnGapSetLocalName);
        return m_pfnGapSetLocalName(deviceName);
    }
    DWORD EnableUnderTestMode()
    {
        CHECK_FUNC_PTR(m_pfnGapEnableUnderTestMode);
        return m_pfnGapEnableUnderTestMode();
    }
    DWORD GetDeviceRecord(BT_ADDR deviceAddr, BluetoothDeviceRecord* deviceRecord)
    {
        CHECK_FUNC_PTR(m_pfnGapGetDeviceRecord);
        return m_pfnGapGetDeviceRecord(deviceAddr, deviceRecord);
    }
    DWORD EnumDeviceRecord(BluetoothDeviceRecord* deviceRecordList, DWORD recordCount, DWORD* actualCount)
    {
        CHECK_FUNC_PTR(m_pfnGapEnumDeviceRecord);
        return m_pfnGapEnumDeviceRecord(deviceRecordList, recordCount, actualCount);
    }
    /* --- GAP API --- */

    BOOL HasValidLinkKey(BT_ADDR btAddr);

    DWORD EnterActiveMode(BT_ADDR* pba);
    static DWORD WINAPI EnterActiveModeThread(LPVOID lpParameter);

private:
    PFN_GapRegister                m_pfnGapRegister;
    PFN_GapUnregister              m_pfnGapUnregister;
    PFN_GapSetLocalName            m_pfnGapSetLocalName;
    PFN_GapEnableUnderTestMode     m_pfnGapEnableUnderTestMode; // TEST ONLY
    PFN_GapGetDeviceRecord         m_pfnGapGetDeviceRecord;
    PFN_GapEnumDeviceRecord        m_pfnGapEnumDeviceRecord;

    //Thread to auto-connect Bluetooth profile.
    class CAutoConnectThread;
    friend CAutoConnectThread;
    class CAutoConnectThread : public CMiniThread
    {
        public:
            CAutoConnectThread(CApi_SearchPair* apiSearchPair);
            virtual ~CAutoConnectThread();
    
            virtual DWORD ThreadRun();

            void Notify(BT_ADDR bluetoothAddress, UCHAR reason);

            void SetConnectFlag();

            static BOOL IsConnected(int serviceType);

            void StopAutoConnect();
        private:
            DWORD GetSelectedDevice(BTDEV* &pbtd);

            BOOL NeedConnect(BT_ADDR bluetoothAddress);

            BOOL IsConnected();
            BOOL IsConnected(BTDEV* pbtd);

            BOOL IsIdle(int serviceType);

            BOOL SupportService(BTDEV* pbtd, int serviceType);

            BOOL ValidAddress(BT_ADDR bluetoothAddress);
            BOOL SameAddress(BT_ADDR btAddr, BT_ADDR btAddr2) { return btAddr == btAddr2 ? TRUE : FALSE; }

        private:
            CApi_SearchPair* m_apiSearchPair;
            HANDLE m_hAutoConnectEvent;
            BT_ADDR m_bluetoothAddress;     /* Bluetooth address which need to perform auto-connect. */
            BOOL m_fAutoConnectNeeded;      /* Flag indicating whether the thread need to auto-connect. */
    };

    CAutoConnectThread* m_pAutoConnectThread;

    //Thread to auto connect SPP.
    class CAutoConnectSppThread : public CMiniThread
    {
        public:
            CAutoConnectSppThread(CApi_SearchPair* apiSearchPair);
            virtual ~CAutoConnectSppThread();
    
            virtual DWORD ThreadRun();

            void NotifyConnectSppforTpms(UCHAR reason);

            void SetConnectSppFlag();

            void StopAutoConnectSpp();

            BOOL IsSppforTpmsConnected();

        private:
            void NotifyConnectSpp(BYTE sppIndex, UCHAR reason);

            const CBTDevList* GetPairedList();

            BOOL IsSppforTpms(BT_ADDR bluetoothAddress);
            BOOL IsSppforTpms(BTDEV* btd);
            BOOL IsSppforTpmsService();

            DWORD ConnectSppDevice(BYTE sppIndex, BT_ADDR bluetoothAddress);

            BOOL IsSppConnected(BYTE sppIndex);

        private:
            CApi_SearchPair* m_apiSearchPair;
            HANDLE m_hAutoConnectSppEvent;
            BOOL m_fAutoConnectSppNeeded;    /* Flag indicating whether the thread need to auto-connect. */
    };

    CAutoConnectSppThread* m_pAutoConnectSppThread;

    //Thread to enable/disable Bluetooth.
    class CEnableBluetoothThread;
    friend CEnableBluetoothThread;
    class CEnableBluetoothThread : public CMiniThread
    {
        public:
            CEnableBluetoothThread(CApi_SearchPair* apiSearchPair, BOOL enable = TRUE);
            virtual ~CEnableBluetoothThread();
    
            virtual DWORD ThreadRun();

        private:
            void Enable();
            void Disable();

            BOOL IsConnected(int serviceType);

        private:
            CApi_SearchPair* m_apiSearchPair;
            BOOL m_enable;
    };

    CEnableBluetoothThread* m_pEnableBluetoothThread;
};
