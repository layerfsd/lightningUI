/******************************************************************************************************

  				Cambridge Silicon Radio Ltd, 2014
 	 			
				All rights reserved

FILE:			bluetooth_module_api.h

DESCRIPTION:	...

REVISION:		$Revision: #1 $
        
*******************************************************************************************************/
#ifndef _BLUETOOTH_MODULE_API_H
#define _BLUETOOTH_MODULE_API_H

// Used in bluetooth status callbacks
#define BLUETOOTH_STATUS_ON				0x0001
#define BLUETOOTH_STATUS_OFF			0x0002

#define API_CALLBACK_MODEL_DIRECT		(0x0001)
#define API_CALLBACK_MODEL_MESSAGE		(0x0002)

typedef enum
{
	MODULE_API_CALLBACK_MODEL_DIRECT		= API_CALLBACK_MODEL_DIRECT,
	MODULE_API_CALLBACK_MODEL_MESSAGE		= API_CALLBACK_MODEL_MESSAGE,
} ModuleApiCallbackModel;

typedef enum
{
	MODULE_API_BLUETOOTH_STATUS_ON		= BLUETOOTH_STATUS_ON,
	MODULE_API_BLUETOOTH_STATUS_OFF		= BLUETOOTH_STATUS_OFF,
} ModuleApiBluetoothStatus;

typedef void (WINAPI* BluetoothStatusChange) (LPVOID userdata, ModuleApiBluetoothStatus status);

#endif
