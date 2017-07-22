/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           hid_monitor_api.h

DESCRIPTION:    Header for HID monitor API.

REVISION:       $Revision: #2 $

*******************************************************************************************************/
#ifndef _HID_MONITOR_API_H
#define _HID_MONITOR_API_H

#include "bluetooth_hid_monitor.h"
#include "hid_monitor.h"


class CHidMonitorAPI
{
    public:
        CHidMonitorAPI();
        virtual ~CHidMonitorAPI();

        BOOL Init(HidMonitorRegisterInfoT* registerInfo);
        void Deinit();

        BOOL SetConnectionStatus(HidStateT hidState, BT_ADDR bluetoothAddress);

        BOOL IsHidHost(BT_ADDR bluetoothAddress);

        BOOL GetHidHostInfo(BT_ADDR bluetoothAddress, HidHostInfo* pHostInfo);

        BOOL ClearHidHostInfo(BT_ADDR bluetoothAddress);

        BOOL NotifyCallActive(BOOL fCallActive);

        /* [DebugOnly] */
        DWORD HandleTouchMessage(UINT msg, UINT flags, LONG x, LONG y);        

    private:
        CHidMonitor* m_hidMonitor;
};

#endif /* _HID_MONITOR_API_H */
