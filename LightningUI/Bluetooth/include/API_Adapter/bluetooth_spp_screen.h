/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           bluetooth_spp_screen.h

DESCRIPTION:    Bluetooth SPP screen.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_SPP_SCREEN_H
#define _BLUETOOTH_SPP_SCREEN_H

#include <windows.h>
#include "bluetooth_spp.h"
#include "bluetooth_hid_screen_prim.h"
#include "bluetooth_hid_pointer_velocity.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void (WINAPI *SppScreenCbMessageHandlerT)(PBYTE prim, DWORD primSize);
typedef void (WINAPI *SppScreenCbStateHandlerT)(SppStateT state, DWORD result);

typedef struct
{
    SppScreenCbMessageHandlerT          messageHandler;
	SppScreenCbStateHandlerT			stateHandler;
} SppScreenRegisterInfoT;


/* SPP screen API proto-type. */
typedef DWORD (WINAPI *PFN_SppScreenInit)(DWORD sppIndex, SppScreenRegisterInfoT* registerInfo);
typedef DWORD (WINAPI *PFN_SppScreenDeinit)();
typedef DWORD (WINAPI *PFN_SppScreenConnect)(BT_ADDR bluetoothAddress);
typedef DWORD (WINAPI *PFN_SppScreenDisconnect)();
typedef DWORD (WINAPI *PFN_SppScreenSendData)(LPCVOID lpBuffer, DWORD bufferSize);

typedef DWORD (WINAPI *PFN_SppScreenInitInfo)();

typedef DWORD (WINAPI *PFN_SppScreenGetResolution)(ScreenResolutionT* screenResolution);
typedef DWORD (WINAPI *PFN_SppScreenGetDirection)(ScreenDirectionT* screenDirection);
typedef DWORD (WINAPI *PFN_SppScreenGetPointerMovementUnit)(UINT* pixelUnit);

typedef DWORD (WINAPI *PFN_SppScreenStartCalibration)();
typedef DWORD (WINAPI *PFN_SppScreenStopCalibration)();
typedef DWORD (WINAPI *PFN_SppScreenShowCalibration)(HidScreenDirectionT screenDir, COLORREF color);

typedef DWORD (WINAPI *PFN_SppScreenGetVelocityControlParameters)(VelocityControlParameters* parameters);

typedef DWORD (WINAPI *PFN_SppScreenMessageHandler)(PBYTE prim, DWORD primSize);


/* SPP screen API. */
DWORD WINAPI SppScreenInit(DWORD sppIndex, SppScreenRegisterInfoT* registerInfo);
DWORD WINAPI SppScreenDeinit();
DWORD WINAPI SppScreenConnect(BT_ADDR bluetoothAddress, DWORD timeout);
DWORD WINAPI SppScreenDisconnect();
DWORD WINAPI SppScreenSendData(LPCVOID lpBuffer, DWORD bufferSize);

DWORD WINAPI SppScreenInitInfo();

DWORD WINAPI SppScreenGetResolution(ScreenResolutionT* screenResolution);
DWORD WINAPI SppScreenGetDirection(ScreenDirectionT* screenDirection);
DWORD WINAPI SppScreenGetPointerMovementUnit(SHORT* pixelUnit);

DWORD WINAPI SppScreenStartCalibration();
DWORD WINAPI SppScreenStopCalibration();
DWORD WINAPI SppScreenShowCalibration(HidScreenDirectionT screenDir, COLORREF color);

DWORD WINAPI SppScreenGetVelocityControlParameters(VelocityControlParameters* parameters);

DWORD WINAPI SppScreenNotifyHdmiStatus(HdmiStatusT hdmiStatus);

DWORD WINAPI SppScreenMessageHandler(PBYTE prim, DWORD primSize);
DWORD SppScreenStateHandler(DWORD connectionIndex, BT_ADDR bluetoothAddress, SppStateT state, DWORD result);

#ifdef __cplusplus
}
#endif

#endif
