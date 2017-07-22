#pragma once
#include <pwinuser.h>
#include <cmthread.h>
#include <queue>
//#include "afxwin.h"
#include "api_base.h"
#include "bluetooth_hid_device.h"


typedef void (WINAPI* HiddT) (void);

typedef DWORD (WINAPI* HiddSendReportT)(HiddReportLenT reportLen, HiddReportT report);
typedef DWORD (WINAPI* HiddSendRegisterT)(HiddDeviceTypeT deviceType, HiddRegisterT*  registerCallbacks);
typedef DWORD (WINAPI* HiddUnregisterT)(void);
typedef DWORD (WINAPI* HiddPostReportT)(HiddReportLenT reportLen, HiddReportT report);
typedef DWORD (WINAPI* HiddGetMouseCooT)(CsrUint8* X,CsrUint8* Y,CsrUint8* scroll);
typedef DWORD (WINAPI* HiddUnPlugT)();
typedef DWORD (WINAPI* HiddSetHostT)(BT_ADDR bluetoothAddress);

typedef DWORD (WINAPI* HiddRegisterExFuncT)(HiddServiceRecordLenT serviceRecordLen, HiddServiceRecordT serviceRecord, HiddRegisterT* registerCallbacks);
typedef DWORD (WINAPI* HiddEnableLowPowerModeT)(BOOL enable);

class CApi_HID  : public CApi_base
{
public:
    CApi_HID(void);
    ~CApi_HID(void);

    virtual BOOL Initialize();
    virtual void DeInitialize();

    BOOL HidModuleInit();
    void HidModuleDeinit();

    DWORD Connect(BT_ADDR bluetoothAddress);
    DWORD Disconnect();
    DWORD MakeDiscoverable();

    DWORD HidRegisterEx(HiddServiceRecordLenT serviceRecordLen, HiddServiceRecordT serviceRecord, HiddRegisterT* registerCallbacks);
    DWORD HidRegister(HiddDeviceTypeT deviceType, HiddRegisterT*  registerCallbacks);
    DWORD HidUnregister();
    DWORD HidModuleSend(HiddReportLenT reportLen, HiddReportT report);
    DWORD HidModulePost(HiddReportLenT reportLen, HiddReportT report);
    DWORD HidUnPlug();
    DWORD HidSetHost(BT_ADDR bluetoothAddress);
    DWORD HidEnableLowPowerMode(BOOL enable);

    DWORD HandleTouchMessage(HWND hWnd, UINT message, UINT flags, LONG x, LONG y);
    DWORD HandleTouchMessage(UINT message, UINT flags, LONG x, LONG y);
    DWORD HandleTouchMessage(UINT message, UINT flags);

    WORD GetConnectionStatus();
    void SetConnectionStatus(HidStateT hidState);

    DWORD GetFeatureMask()
    {
        return m_hidFeature;
    }

    BT_ADDR GetBluetoothAddress()
    {
        return m_bluetoothAddress;
    }

    void UpdateHidState(HidStateT hidState, BT_ADDR btAddress);

    BOOL IsHidHost(BT_ADDR bluetoothAddress);

    static void NotifyCallActive(BOOL fCallActive);

private:
    CRITICAL_SECTION cs;
    HINSTANCE  m_hDll;

    HidStateT m_hidState;
    DWORD m_hidFeature;
    BT_ADDR m_bluetoothAddress;
};

