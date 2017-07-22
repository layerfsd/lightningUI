/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           bluetooth_hid_screen_def.h

DESCRIPTION:    Basic definition for Bluetooth HID screen.

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_HID_SCREEN_DEF_H
#define _BLUETOOTH_HID_SCREEN_DEF_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    SCREEN_DIR_LANDSCAPE = 0,
    SCREEN_DIR_PORTRAIT
} ScreenDirectionT;

typedef struct
{
    LONG width;             /* Touch screen width in pixel, such as 1024. */
    LONG height;            /* Touch screen height in pixel, such as 768. */
} ScreenResolutionT;


#ifdef __cplusplus
}
#endif

#endif