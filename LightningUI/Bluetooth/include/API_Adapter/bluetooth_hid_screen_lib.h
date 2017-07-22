/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           bluetooth_hid_screen_lib.h

DESCRIPTION:    Utility lib for Bluetooth HID screen

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HID_SCREEN_LIB_H
#define _BLUETOOTH_HID_SCREEN_LIB_H

#include "bluetooth_hid_screen_prim.h"
#include "bluetooth_hid_screen_def.h"
#include "bluetooth_spp.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Downstream primitive. */
DWORD HidSendMessage(void* prim, DWORD primSize);

DWORD HidGetScreenResolutionReqSend();
DWORD HidGetScreenDirectionReqSend();
DWORD HidGetPointerMovementUnitReqSend();
DWORD HidStartScreenCalibrationReqSend();
DWORD HidStopScreenCalibrationReqSend();
DWORD HidShowCalibrationScreenReqSend(HidScreenDirectionT screenDir, COLORREF color);
DWORD HidGetVelocityControlParametersReqSend();
DWORD HidNotifyHdmiStatusIndSend(HdmiStatusT hdmiStatus);

/* Upstream primitive. */
void HidRecvMessageHandler(PBYTE buf, DWORD bufSize);
DWORD HidParseMessage(PBYTE buf, DWORD bufSize, void** prim, DWORD* primSize);

void HidSppForHidStateChangedHandler(DWORD connectionIndex, BT_ADDR bluetoothAddress, SppStateT state, DWORD result);

#ifdef __cplusplus
}
#endif

#endif /* _BLUETOOTH_HID_SCREEN_LIB_H */
