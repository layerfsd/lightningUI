/******************************************************************************************************

                CSR plc 2014

				All rights reserved

FILE:			bluetooth_tpms.h

DESCRIPTION:	TPMS API for Application

REVISION:		$Revision: #1 $

*******************************************************************************************************/
#ifndef _BLUETOOTH_TPMS_H
#define _BLUETOOTH_TPMS_H

#ifdef __cplusplus
extern "C" {
#endif

#define BLUETOOTH_TPMS_IMPLEMENTATION


#ifdef BLUETOOTH_TPMS_IMPLEMENTATION
#ifdef WPP_WIN32_BUILD
#define BLUETOOTH_TPMS_API extern
#else
#define BLUETOOTH_TPMS_API __declspec(dllexport)
#endif
#else
#define BLUETOOTH_TPMS_API __declspec(dllimport)
#endif


typedef unsigned short			TpmsPrimT;
typedef unsigned short			TpmsResultCode;
typedef unsigned short			TpmsReasonCode;


/*bitMask for setting*/
#define TPMS_THRESHOLD_PRESSURE_HIGH	(0x0001)
#define TPMS_THRESHOLD_PRESSURE_LOW		(0x0002)
#define TPMS_THRESHOLD_TEMP_HIGH		(0x0004)
#define TPMS_THRESHOLD_TEMP_LOW			(0x0008)

/*bitMask for getting or indication/notification*/
#define TPMS_PRESSURE_DATA				(0x0010)
#define TPMS_TEMP_DATA					(0x0020)
#define TPMS_BATTERY_LEVEL_DATA			(0x0040)
#define TPMS_RSSI_DATA					(0x0080)

/*TPMS calibrate command*/
typedef enum
{
    TPMS_CALI_0_PRESS,
	TPMS_CALI_FULL_PRESS
}TpmsCaliCmd;

/*TPMS connect status*/
typedef enum
{
    TPMS_IDLE,
    TPMS_CONNECTED,
} TpmsConnStatusT;

typedef enum
{
    TPMS_CM_DIRECT,/*Only this method supported currently*/
    TPMS_CM_MESSAGE
} TpmsCallbackModeT;


typedef unsigned short TpmsThresholdTypeT;
typedef unsigned short TpmsTireDataTypeT;


/*Used for transmit tpms data*/
typedef struct
{
    double pressure;
    signed char temperature;
    unsigned char battery_level;
    signed char rssi;
} TpmsDataStructure;

typedef struct
{
    TpmsTireDataTypeT bitMask;

    double pressure;
    signed char temperature;
    unsigned char battery_level;
    signed char rssi;
} TpmsTireDataT;

/*Used for threshold data*/
typedef struct
{
	double high_pressure;
	double low_pressure;
	signed char high_temperature;/*means high temperature*/
	/*seems we need not set low temperature threshold*/
}TpmsThresholdDataStructure;

/*Call back function pointer define*/
typedef void (* TpmsCbCmdGeneralCfmT)(TpmsResultCode resultCode);
typedef void (* TpmsCbConnStatusT)(BT_ADDR bluetoothAddress, TpmsConnStatusT status,TpmsResultCode resultCode);
typedef void (* TpmsCbTireDataStatusT)(BT_ADDR bluetoothAddress, TpmsTireDataTypeT bitMask, TpmsDataStructure *value);
typedef void (* TpmsCbSearchDeviceNotificationT)(BT_ADDR bluetoothAddress, signed char rssi, unsigned char nameLen, unsigned char *name);
typedef void (* TpmsCbThresholdIndicationT)(BT_ADDR bluetoothAddress,TpmsThresholdTypeT bitMask,TpmsThresholdDataStructure *value);

/*TPMS transmitter PR TEST use only*/
typedef void (* TpmsCbCalibrateResultT)(BT_ADDR bluetoothAddress,unsigned short volt);

/*Call back function structure define*/
typedef struct
{
    TpmsCallbackModeT callbackMode;
	TpmsCbCmdGeneralCfmT tpmsCmdGeneralCfm;/*the confirms of application command ever sent*/
    TpmsCbConnStatusT tpmsConnStatusInd;/*conn,disconn indications*/
    TpmsCbTireDataStatusT tpmsTireDataInd;/*tire pressure,temperature,rssi,battery indication*/
    TpmsCbSearchDeviceNotificationT tpmsSearchDeviceNotification;
	TpmsCbThresholdIndicationT tpmsThresholdIndication;
	TpmsCbCalibrateResultT tpmsCalibrateResult;
} TpmsRegisterInfoT;

DWORD BLUETOOTH_TPMS_API TpmsRegister(TpmsRegisterInfoT *registerInfo);
DWORD BLUETOOTH_TPMS_API TpmsUnregister();

DWORD BLUETOOTH_TPMS_API BleTpmsSearchDevice();/*after this command was sent, application should stop search device before next action*/
DWORD BLUETOOTH_TPMS_API BleTpmsStopSearchDevice();/*do not send other command in search scenario except this one*/
DWORD BLUETOOTH_TPMS_API BleTpmsAddWhiteList(unsigned short addressCount,BT_ADDR * bluetoothAddress);/*if application want to connect multi device, first add these device to whitelist and then connect*/
DWORD BLUETOOTH_TPMS_API BleTpmsClearWhiteList();/*do not operate white list in searching/connecting scenario*/
DWORD BLUETOOTH_TPMS_API BleTpmsConnect(BT_ADDR bluetoothAddress);/*if the address was set to 0, stack would connect the device which stored at white list */
DWORD BLUETOOTH_TPMS_API BleTpmsDisconnect(BT_ADDR bluetoothAddress);
DWORD BLUETOOTH_TPMS_API BleTpmsGetThreshold(BT_ADDR bluetoothAddress, TpmsThresholdTypeT bitMask);
DWORD BLUETOOTH_TPMS_API BleTpmsSetThreshold(BT_ADDR bluetoothAddress, TpmsThresholdTypeT bitMask, TpmsThresholdDataStructure *value);
DWORD BLUETOOTH_TPMS_API BleTpmsGetTireData(BT_ADDR bluetoothAddress, TpmsTireDataTypeT bitMask);

/*TPMS transmitter PR use only, applicaiton should not call this function in notmal case*/
DWORD BLUETOOTH_TPMS_API BleTpmsCalibrate(BT_ADDR bluetoothAddress, TpmsCaliCmd cmd);


#ifdef __cplusplus
}
#endif

#endif/*_BLUETOOTH_TPMS_H*/
