/******************************************************************************************************

                Cambridge Silicon Radio Ltd, 2014

                All rights reserved

FILE:           bluetooth_did.h

DESCRIPTION:    Device Identification API for OEM's

REVISION:       $Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_DID_H
#define _BLUETOOTH_DID_H

#include <ws2bth.h>
#include "bluetooth_module_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BLUETOOTH_DID_IMPLEMENTATION
#ifdef WPP_WIN32_BUILD
#define BLUETOOTH_DID_API extern
#else
#define BLUETOOTH_DID_API __declspec(dllexport)
#endif
#else
#define BLUETOOTH_DID_API __declspec(dllimport)
#endif


// Vendor id source
// 0x0001 means the bluetooth vendor id is registered
// 0x0002 means the usb vendor id is registered
#define VENDOR_ID_SOURCE_BLUETOOTH	0x0001
#define VENDOR_ID_SOURCE_USB		0x0002

// More Bluetooth vendor ids on bluetooth sig
// https://www.bluetooth.org/en-us/specification/assigned-numbers/company-identifiers
#define SAMSUNG_BLUETOOTH_VENDOR_ID	0x0075			// Company id for Samsung Electronics Co. Ltd.

// More Usb vendor ids on usb org
// http://www.linux-usb.org/usb.ids
#define APPLE_USB_VENDOR_ID    		0x05AC                  // Company id for Apple, Inc.

// Apple product id
#define APPLE_PRODUCT_ID_IPHONE5S_5C_5	0x12a8                  // iPhone 5/5s/5c
#define APPLE_PRODUCT_ID_IPHONE4	0x1297			// iPhone 4
#define APPLE_PRODUCT_ID_IPHONE4_CDMA	0x129c			// iPhone 4 (CDMA)
#define APPLE_PRODUCT_ID_IPHONE4S	0x12a0  		// iPhone 4s



typedef enum
{
    DIDM_CALLBACK_DIRECT,/*Only this method supported currently*/
    DIDM_CALLBACK_MESSAGE
} DidmCallbackModeT;

typedef struct
{
    WORD   specificationId;
    WORD   vendorId;
    WORD   productId;
    WORD   version;
    BOOL   primaryRecord;
    WORD   vendorIdSource;
} DiServiceRecordV13;


/* DID READ Indication callback */
typedef void (WINAPI* DidmCbReadServiceRecordIndT)(DWORD result, DiServiceRecordV13* diServiceRecord);

/* DID READ Confirmation callback */
typedef void (WINAPI* DidmCbReadServiceRecordCfmT)(DWORD result);


typedef struct
{
    DidmCallbackModeT		  mode;
    DidmCbReadServiceRecordIndT   didmCbReadServiceRecordInd;
    DidmCbReadServiceRecordCfmT   didmCbReadServiceRecordCfm;
} DidmRegisterInfoT;

DWORD BLUETOOTH_DID_API DidRegister(DidmRegisterInfoT* registerInfo);
DWORD BLUETOOTH_DID_API	DidUnregister();
DWORD BLUETOOTH_DID_API DidReadDiServiceRecord(BT_ADDR bluetoothAddress);
DWORD BLUETOOTH_DID_API DidCancelReadDiServiceRecord();

#ifdef __cplusplus
}
#endif

#endif
