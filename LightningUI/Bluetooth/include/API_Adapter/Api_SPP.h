#pragma once
#include "api_base.h"
#include "bluetooth_spp.h"
#include <pkfuncs.h>


#define SPP_STATE_IDLE                  TEXT("Idle")
#define SPP_STATE_CONNECTING            TEXT("Connecting")
#define SPP_STATE_CONNECTED             TEXT("Connected")
#define SPP_STATE_DISCONNECTING         TEXT("Disconnecting")
#define SPP_STATE_TRANSFER              TEXT("Transfer")
#define SPP_STATE_TRANSFER_COMPLETED    TEXT("TransferCompleted")
#define SPP_STATE_TRANSFER_FAIL         TEXT("TransferFail")


#define INVALID_SPP_INDEX               0xFF

#define BLUETOOTH_SPP_DRIVER_REG        TEXT("Drivers\\BuiltIn\\BchsSppDrv")

/* +++ Macro of SPP for TPMS. +++ */
#define BLUETOOTH_SPP_DEVICE_NAME               TEXT("SPP")
#define BLUETOOTH_SPP_SERVICE_NAME              TEXT("SPP Service")

#define BLUETOOTH_SPP_FOR_TPMS_DEVICE_NAME      TEXT("SPP for TPMS")
#define BLUETOOTH_SPP_FOR_TPMS_SERVICE_NAME     TEXT("SPP Service for TPMS")
/* --- Macro of SPP for TPMS. --- */


class CApi_SPP  : public CApi_base
{
private:
    HANDLE m_hConnectionEvent;
    HANDLE m_hSppforTpmsConnectionEvent;

public:
    CApi_SPP(void);
    ~CApi_SPP(void);

    HANDLE m_hSpp;
    SppServiceName m_localServiceName;
    SppConnectInfo m_sppConnectInfo;

    DWORD m_sppIndex;
    CString m_szSendFileName;
    CString m_szReceiveFileName;

    DWORD m_sppIndexforTpms;

    virtual BOOL Initialize();
    virtual void DeInitialize();

    BOOL SppModuleInit(DWORD connectionIndex, LPVOID pfnStateCallback);
    void SppModuleDeinit(DWORD connectionIndex);

    DWORD SppModuleConnect(DWORD connectionIndex, BT_ADDR bluetoothAddress);
    DWORD SppModuleConnect(DWORD connectionIndex, SppConnectInfo *connectInfo);
    DWORD SppModuleDisconnect(DWORD connectionIndex);

    DWORD SppModuleSend(DWORD connectionIndex, LPCTSTR fileName);
    DWORD SppModuleCancelSend(DWORD connectionIndex);

    DWORD SppModuleReceive(DWORD connectionIndex, LPCTSTR fileName);
    DWORD SppModuleCancelReceive(DWORD connectionIndex);

    /* Override CApi_base. */
    WORD GetConnectionStatus();
	void SetConnectionStatus(SppStateT status);

    SppStateT GetConnectionStatus(DWORD connectionIndex);
    void SetConnectionStatus(DWORD connectionIndex, SppStateT status);

    BT_ADDR GetBluetoothAddress(DWORD connectionIndex);

    DWORD WaitConnected(DWORD timeout = INFINITE);
    void NotifyConnected(BOOL isConnected = TRUE);

    DWORD WaitSppforTpmsConnected(DWORD timeout = INFINITE);
    void NotifySppforTpmsState(DWORD connectionIndex, BT_ADDR bluetoothAddress, SppStateT state, DWORD result);

	void GetDeviceDidInfoOnSppConnected();
};