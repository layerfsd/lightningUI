/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2013

                All rights reserved

FILE:           bt_hid_screen_serialize.h

DESCRIPTION:    Header for the utility to serialize/de-serialize the primitive of Bluetooth HID screen.

REVISION:       $Revision: #1 $

*******************************************************************************************************/

#ifndef _BT_HID_SCREEN_SERIALIZE_H
#define _BT_HID_SCREEN_SERIALIZE_H

#include <windows.h>
#include "bluetooth_hid_screen_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t HidGetScreenResolutionReqSizeof(void *msg);
BYTE* HidGetScreenResolutionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
extern void *HidGetScreenResolutionReqDes(CsrUint8 *buffer, CsrUint16 length);
extern void HidGetScreenResolutionReqSerFree(void *msg);

#ifdef __cplusplus
}
#endif

#endif
