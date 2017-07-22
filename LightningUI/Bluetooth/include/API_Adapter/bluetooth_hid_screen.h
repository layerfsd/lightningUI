/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           bluetooth_hid_screen.h

DESCRIPTION:    Bluetooth HID screen.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HID_SCREEN_H
#define _BLUETOOTH_HID_SCREEN_H

#include <windows.h>
#include <winuserm.h>
#include "bluetooth_hid_screen_def.h"
#include "bluetooth_hid_screen_prim.h"


#ifdef __cplusplus
extern "C" {
#endif


#define HID_SCREEN_LOGIC_MIN                ((SHORT) 0x8001)        /* -32767 */
#define HID_SCREEN_LOGIC_MAX                ((SHORT) 0x7fff)        /* 32767  */

/* Default resolution for local touch screen (HID device). */
#define DEFAULT_LOCAL_SCREEN_WIDTH          800
#define DEFAULT_LOCAL_SCREEN_HEIGHT         480
#define DEFAULT_LOCAL_SCREEN_DIRECTION      SCREEN_DIR_LANDSCAPE

/* Default resolution for remote touch screen (HID host). */
#define DEFAULT_REMOTE_SCREEN_WIDTH         1280    //1920
#define DEFAULT_REMOTE_SCREEN_HEIGHT        720     //1080
#define DEFAULT_REMOTE_SCREEN_DIRECTION     SCREEN_DIR_LANDSCAPE  //SCREEN_DIR_PORTRAIT

#define IS_SCREEN_LANDSCAPE(sd)             ((sd) == SCREEN_DIR_LANDSCAPE)
#define IS_SCREEN_PORTRAIT(sd)              ((sd) == SCREEN_DIR_PORTRAIT)

#define IS_BOUNDARY_POINT(x, y)             (((x) == HID_SCREEN_LOGIC_MAX) || ((y) == HID_SCREEN_LOGIC_MAX))

/* Extended virtual key definition. */
#define VK_NULL                             0x0
#define VK_CLOSE                            VK_APP1
#define VK_DOCK                             VK_APP2

#define VALID_VK(b)                         ((b) != VK_NULL)

#define IS_VK_HOME(vk)                      ((vk) == VK_HOME)
#define IS_VK_MENU(vk)                      ((vk) == VK_MENU)
#define IS_VK_BACK(vk)                      ((vk) == VK_BACK)

#define IS_VK_OF_GENERIC_CONTROL(vk)        (IS_VK_HOME(vk) || IS_VK_MENU(vk) || IS_VK_BACK(vk))

#define IS_VK_CLOSE(vk)                     ((vk) == VK_CLOSE)
#define IS_VK_DOCK(vk)                      ((vk) == VK_DOCK)

#define IS_EXTENDED_VK(vk)                  (IS_VK_CLOSE(vk) || IS_VK_DOCK(vk))

#define IS_VK_PLAY(vk)                      ((vk) == VK_PLAY)
#define IS_VK_PAUSE(vk)                     ((vk) == VK_PAUSE)
#define IS_VK_MEDIA_NEXT_TRACK(vk)          ((vk) == VK_MEDIA_NEXT_TRACK)
#define IS_VK_MEDIA_PREV_TRACK(vk)          ((vk) == VK_MEDIA_PREV_TRACK)
#define IS_VK_MEDIA_STOP(vk)                ((vk) == VK_MEDIA_STOP)
#define IS_VK_MEDIA_PLAY_PAUSE(vk)          ((vk) == VK_MEDIA_PLAY_PAUSE)
#define IS_VK_VOLUME_UP(vk)                 ((vk) == VK_VOLUME_UP)
#define IS_VK_VOLUME_DOWN(vk)               ((vk) == VK_VOLUME_DOWN)

#define IS_VK_OF_MEDIA_CONTROL(vk)          (IS_VK_PLAY(vk) || IS_VK_PAUSE(vk) || IS_VK_MEDIA_NEXT_TRACK(vk) || IS_VK_MEDIA_PREV_TRACK(vk) || \
                                             IS_VK_MEDIA_STOP(vk) || IS_VK_MEDIA_PLAY_PAUSE(vk) || IS_VK_VOLUME_UP(vk) || IS_VK_VOLUME_DOWN(vk))

#define HDMI_REG                            TEXT("Software\\CSR\\SmartPhoneLink\\HDMI")
#define HDMI_WINDOW_HANDLE                  TEXT("HWND")
#define OUTPUT_RECT_FORMAT                  TEXT("%d, %d, %d, %d")                  /* left, top, right, bottom */

/* Virtual key for generic control. */
#define VK_BACK_OUTPUT_RECT                 TEXT("VK_BACK_OutputRect")
#define VK_HOME_OUTPUT_RECT                 TEXT("VK_HOME_OutputRect")
#define VK_MENU_OUTPUT_RECT                 TEXT("VK_MENU_OutputRect")

/* Virtual key for media player control. */
#define VK_PLAY_OUTPUT_RECT                 TEXT("VK_PLAY_OutputRect")
#define VK_PAUSE_OUTPUT_RECT                TEXT("VK_PAUSE_OutputRect")
#define VK_MEDIA_NEXT_TRACK_OUTPUT_RECT     TEXT("VK_MEDIA_NEXT_TRACK_OutputRect")
#define VK_MEDIA_PREV_TRACK_OUTPUT_RECT     TEXT("VK_MEDIA_PREV_TRACK_OutputRect")
#define VK_MEDIA_STOP_OUTPUT_RECT           TEXT("VK_MEDIA_STOP_OutputRect")
#define VK_MEDIA_PLAY_PAUSE_OUTPUT_RECT     TEXT("VK_MEDIA_PLAY_PAUSE_OutputRect")
#define VK_VOLUME_UP_OUTPUT_RECT            TEXT("VK_VOLUME_UP_OutputRect")
#define VK_VOLUME_DOWN_OUTPUT_RECT          TEXT("VK_VOLUME_DOWN_OutputRect")

/* Extended virtual key. */
#define VK_CLOSE_OUTPUT_RECT                TEXT("CLOSE_OutputRect")
#define VK_DOCK_OUTPUT_RECT                 TEXT("DOCK_OutputRect")

#define HDMI_LANDSCAPE_OUTPUT_RECT          TEXT("HDMI_LandscapeRect")
#define HDMI_PORTRAIT_OUTPUT_RECT           TEXT("HDMI_PortraitRect")


typedef struct
{
    ScreenResolutionT localResolution;
    ScreenDirectionT localDirection;  

    ScreenResolutionT remoteResolution;
    ScreenDirectionT remoteDirection;

    /* 0: output rectangle of local screen when remote screen is in 'landscape', 1: remote screen in 'portrait'. */
    RECT outputRect[2];
} HidScreenInfoT;

typedef struct
{
    BYTE vk;                /* Virtual key, such as 'VK_HOME'. */
    RECT rect;              /* Boudary rectangle for virtual key. */
} VirtualKeyRectT;


/* HID screen API. */
BOOL WINAPI HidScreenInit();
void WINAPI HidScreenDeinit();

BOOL WINAPI HidScreenCheckValid(HidScreenInfoT* phs);
BOOL WINAPI HidScreenVerify(HidScreenInfoT* phs);

BOOL WINAPI HidScreenSetInfo(HidScreenInfoT* phs);
BOOL WINAPI HidScreenGetInfo(HidScreenInfoT* phs);
BOOL WINAPI HidScreenGetDefaultInfo(HidScreenInfoT* phs);

BOOL WINAPI HidScreenGetLocalResolution(ScreenResolutionT* psr);
BOOL WINAPI HidScreenGetLocalDirection(ScreenDirectionT* psd);

BOOL WINAPI HidScreenSetRemoteResolution(ScreenResolutionT* psr);
BOOL WINAPI HidScreenSetRemoteDirection(ScreenDirectionT sd);

BOOL WINAPI HidScreenSetOutputRect(ScreenDirectionT sd, RECT* rect);

BOOL WINAPI HidScreenIsValidPoint(POINT* localPoint);
BOOL WINAPI HidScreenConvertLocal2RemotePoint(POINT* localPoint, POINT* remotePoint);

BOOL WINAPI HidScreenSetVirtualKeyRect(VirtualKeyRectT* vkRect, BYTE vkCount);
BOOL WINAPI HidScreenGetVirtualKeyRect(VirtualKeyRectT** vkRect, BYTE* vkCount);
BOOL WINAPI HidScreenGetDefaultVirtualKeyRect(VirtualKeyRectT** vkRect, BYTE* vkCount);

BOOL WINAPI HidScreenGetVirtualKey(LONG x, LONG y, BYTE& vk);

LPCTSTR WINAPI HidScreenGetVirtualKeyString(BYTE vk);

LPCTSTR WINAPI HidScreenGetDirectionString(ScreenDirectionT sd);

DWORD WINAPI HidScreenMapResult(HidScreenResultT hsr);


#ifdef __cplusplus
}
#endif

#endif
