/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           hid_monitor.h

DESCRIPTION:    Header for HID monitor.

REVISION:       $Revision: #3 $

*******************************************************************************************************/
#ifndef _HID_MONITOR_H
#define _HID_MONITOR_H

#include "bluetooth_hid_monitor.h"
#include "hid_monitor_util.h"
#include "hid_monitor_thread.h"
#include "hid_host_info_thread.h"


/* +++  HID for HDMI +++ */
/* 1. Definition of Bluetooth named event which signal into HDMI. */
#define BT_EVENT_NAME                   TEXT("/bluetooth/event/prim")

/* Primitive definition for Bluetooth event. */
#define BT_HID_SCREEN_CALIBRATE_REQ     ((WORD) 0x8000)
#define BT_HFP_CALL_STATUS_IND          ((WORD) 0x8001)
#define BT_HID_STATUS_IND               ((WORD) 0x8002)

/* HFP call status */
#define HFP_CALL_INACTIVE               0
#define HFP_CALL_ACTIVE                 1

/* HID touch screen direction. */
#define HID_SCREEN_DIR_LANDSCAPE        0
#define HID_SCREEN_DIR_PORTAIT          1

/* HID status definition for the synchonization between BT HID and HDMI. */
#define HID_STATUS_IDLE                 0
#define HID_STATUS_CONNECTED            1
#define HID_STATUS_ACTIVE               2

#define BT_EVENT_DATA(data, prim)       MAKELONG(data, prim)
#define BT_PRIM(d)                      HIWORD(d)
#define BT_DATA(d)                      LOWORD(d)

/* 2. Definition of HDMI named event which signal into BT HID. */
#define HDMI_EVENT_NAME                 TEXT("/hdmi/event/prim")

/* Primitive definition for HDMI event. */
#define HDMI_STATUS_IND                 ((WORD) 0x8000)
#define HDMI_SCREEN_CALIBRATE_IND       ((WORD) 0x8001)
#define HDMI_CONTROL_BAR_STATUS         ((WORD) 0x8002)

/* HDMI status */
#define HDMI_IDLE                       0x0
#define HDMI_CONNECTED                  0x1
#define HDMI_ACTIVE                     0x2

/* Screen calibrate result. */
#define SCREEN_CALIBRATE_SUCCESS        0
#define SCREEN_CALIBRATE_FAIL           1

/* Control bar status. */
#define CONTROL_POS_CHANGE              0x0

#define HDMI_EVENT_DATA(data, prim)     MAKELONG(data, prim)
#define HDMI_PRIM(d)                    HIWORD(d)
#define HDMI_DATA(d)                    LOWORD(d)

#define SCREEN_CALIBRATE_DATA(res, dir) MAKEWORD(res, dir)
#define SCREEN_CALIBRATE_RESULT(d)      LOBYTE(d)
#define SCREEN_CALIBRATE_DIR(d)         HIBYTE(d)
/* ---  HID for HDMI --- */


class CHidMonitor
{
    public:
        CHidMonitor();
        virtual ~CHidMonitor();

        BOOL Init(HidMonitorRegisterInfoT* registerInfo);
        void Deinit();

        DWORD SendReport(HiddReportLenT reportLen, HiddReportT report);
        DWORD EnableLowPowerMode(BOOL enable);

        BOOL IsCallActive();
        void SendCallStatus();

        DWORD A2dpConnect(BT_ADDR bluetoothAddress);
        DWORD A2dpDisconnect();
        BOOL A2dpIsConnected();

        DWORD AvrcpConnect(BT_ADDR bluetoothAddress);
        DWORD AvrcpDisconnect();
        BOOL AvrcpIsConnected();
        HRESULT AvrcpPlay();
        HRESULT AvrcpPause();
        HRESULT AvrcpNext();
        HRESULT AvrcpPrevious();

        void NotifyCallActive(BOOL fCallActive);

        void SetConnectionStatus(HidStateT hidState, BT_ADDR bluetoothAddress);

        void NotifyHostInfoEvent(DWORD eventData);

        BOOL IsHidHost(BT_ADDR bluetoothAddress);

        BOOL GetHidHostInfo(BT_ADDR bluetoothAddress, HidHostInfo* pHostInfo);

        BOOL ClearHidHostInfo(BT_ADDR bluetoothAddress);

        HidMonitorRegisterInfoT* GetRegisterInfo()
        {
            return &m_registerInfo;
        }

        BT_ADDR GetBluetoothAddress()
        {
            return m_bluetoothAddress;
        }

        CHidMonitorThread* GetMonitorThread()
        {
            return m_pMonitorThread;
        }

        CHidHostInfoThread* GetHostInfoThread()
        {
            return m_pHostInfoThread;
        }

        void InitVirtualKeyRect();

        void NotifyHidConnected(BOOL fConnected);

        DWORD HandleTouchMessage(UINT msg, UINT flags, LONG x, LONG y);

    public:
        static void NotifyBluetoothEvent(WORD data, WORD prim);
        static void SendHIDCalibrateReq(WORD screenDir);
        static void SendHIDStatus(HidStateT hidState, BOOL fScreenCalibrated = FALSE);
        static void SendHFPCallStatus(BOOL fCallActive);

        static void DumpReport(PCHAR reportBuf, ULONG reportSize);

    private:
        HidMonitorRegisterInfoT m_registerInfo;
        CHidMonitorThread* m_pMonitorThread;
        CHidHostInfoThread* m_pHostInfoThread;
        BT_ADDR m_bluetoothAddress;
};


#endif /* _HID_MONITOR_H */
