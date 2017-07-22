/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           bluetooth_hid_parser.h

DESCRIPTION:    Header for Bluetooth HID parser API.

REVISION:       $Revision: #2 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HID_PARSER_H
#define _BLUETOOTH_HID_PARSER_H


#include <windows.h>
#include "bluetooth_hid_screen_def.h"
#include "bluetooth_hid_pointer_velocity.h"


#ifdef __cplusplus
extern "C" {
#endif

/* HID feature. */
#define HID_MOUSE_FEATURE               (0x00000001)
#define HID_TOUCH_SCREEN_FEATURE        (0x00000002)
#define HID_CONSUMER_CONTROL_FEATURE    (0x00000004)
#define HID_BLUETOOTH_FEATURE           (0x80000000)

#define IS_MOUSE(m)                     ((m) & HID_MOUSE_FEATURE)
#define IS_TOUCH_SCREEN(m)              ((m) & HID_TOUCH_SCREEN_FEATURE)
#define IS_CONSUMER_CONTROL(m)          ((m) & HID_CONSUMER_CONTROL_FEATURE)


/* HID parser API. */
BOOL WINAPI HidParserInit();
void WINAPI HidParserDeinit();

BOOL WINAPI HidParserGetSerivceRecord(PBYTE* serviceRecord, DWORD* serviceRecordLength);
BOOL WINAPI HidParserGetReportDescriptor(PBYTE* reportDescriptor, DWORD* reportDescriptorLength);
BOOL WINAPI HidParserGetReportDescriptorExt(DWORD hidFeatureMask, PBYTE* reportDescriptor, DWORD* reportDescriptorLength);

BOOL WINAPI HidParserSetRemoteResolution(ScreenResolutionT* psr);
BOOL WINAPI HidParserSetRemoteDirection(ScreenDirectionT sd);
BOOL WINAPI HidParserSetPointerSpeed(SHORT pointerSpeed);

BOOL WINAPI HidParserSetParameters(VelocityControlParameters* parameters);
BOOL WINAPI HidParserGetParameters(VelocityControlParameters* parameters);
BOOL WINAPI HidParserAnalyzeVelocity(UINT msg, UINT flags, LONG remotePointX, LONG remotePointY, PointerVelocity* velocity);

BOOL WINAPI HidParserInitFeatureReport(PCHAR* reportBuf, ULONG* reportSize);
BOOL WINAPI HidParserInitZeroPointInputReport(PCHAR* reportBuf, ULONG* reportSize);

BOOL WINAPI HidParserInitInputReport(UINT msg, UINT flags, LONG remotePointX, LONG remotePointY, PCHAR* reportBuf, ULONG* reportSize);

BOOL WINAPI HidParserInitVirtualKeyReport(BYTE vk, DWORD flags, PCHAR* reportBuf, ULONG* reportSize);

void WINAPI HidParserDumpReport(PCHAR reportBuf, ULONG reportSize);

DWORD WINAPI HidParserGetFeatureMask();


#ifdef __cplusplus
}
#endif

#endif
