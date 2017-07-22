/***************************************************************************
 *                                                                         *
 *       Shanghai Zongmu Technology Co. Ltd.                               *
 *                                                                         *
 *    Copyright (c) 2013 by Shanghai Zongmu Technology Co. Ltd.            *
 *    All rights reserved.                                                 *
 *                                                                         *
 *    This Software is protected by People's Republic of China copyright   *
 *    laws and international treaties.  You may not reverse engineer,      *
 *    decompile or disassemble this Software.                              *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains Shanghai Zongmu Technology Co. Ltd.'s         *
 *    confidential and proprietary information. UNAUTHORIZED COPYING,      *
 *    USE, DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE *
 *    IS PROHIBITED AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not  *
 *    copy this Software without Shanghai Zongmu Technology Co. Ltd.'s     *
 *    express written permission.   Use of any portion of the contents of  *
 *    this Software is subject to and restricted by your written agreement *
 *    with Shanghai Zongmu Technology Co. Ltd.                             *
 *                                                                         *
 ***************************************************************************/
/**
*  @file blue_tpms_sdk.h
*
*/

#ifndef __BLUE_TPMS_SDK_H__
#define __BLUE_TPMS_SDK_H__

#include <windows.h>
#include "blue_tpms_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define ZONGMU_BLUETOOTH_API __declspec(dllexport)

#define BarToPsi(dBar) ((double)((dBar) * BAR_TO_PSI_RATIO))
#define PsiToBar(dPsi) ((double)((dPsi) / BAR_TO_PSI_RATIO))

typedef void (*TpmsCbSystemInfoT)(TpmsErrorT enError, PTpmsSystemInfoT pSystemInfo);
typedef void (*TpmsCbTransmitterDataT)(TpmsErrorT enError, PTpmsDataT pData);
typedef void (*TpmsCbReceiverStatusT)(TpmsErrorT enError, TpmsReceiverStatusT enStatus);
typedef void (*TpmsCbSmartPhoneConnectStatusT)(TpmsErrorT enError, BOOL bConnected);
typedef void (*TpmsCbStartBindingTransmitterT)(TpmsErrorT enError);
typedef void (*TpmsCbStopBindingTransmitterT)(TpmsErrorT enError);
typedef void (*TpmsCbTransmitterFoundT)();
typedef void (*TpmsCbTransmitterBoundT)(TpmsErrorT enError, TpmsTransmitterPositionT enPosition);
typedef void (*TpmsCbStartBindingSmartPhoneT)(TpmsErrorT enError);
typedef void (*TpmsCbStopBindingSmartPhoneT)(TpmsErrorT enError);
typedef void (*TpmsCbSmartPhoneBoundT)(TpmsErrorT enError);
typedef void (*TpmsCbSwapTransmitterPositionT)(TpmsErrorT enError);
typedef void (*TpmsCbReceiverSettingsT)(TpmsErrorT enError, PTpmsReceiverSettingsT pSettings);
typedef void (*TpmsCbApplyReceiverSettingsT)(TpmsErrorT enError);
typedef void (*TpmsCbResetReceiverToFactoryT)(TpmsErrorT enError);
typedef void (*TpmsCbReceiverVersionT)(TpmsErrorT enError, PZmVersionT pVersion);

typedef struct TpmsRegisterInfoT
{
	TpmsCbSystemInfoT cbSystemInfo;
	TpmsCbTransmitterDataT cbTransmitterData;
	TpmsCbReceiverStatusT cbReceiverStatus;
	TpmsCbSmartPhoneConnectStatusT cbSmartPhoneConnectStatus;
	TpmsCbStartBindingTransmitterT cbStartBindingTransmitter;
	TpmsCbStopBindingTransmitterT cbStopBindingTransmitter;
	TpmsCbTransmitterFoundT cbTransmitterFound;
	TpmsCbTransmitterBoundT cbTransmitterBound;
	TpmsCbStartBindingSmartPhoneT cbStartBindingSmartPhone;
	TpmsCbStopBindingSmartPhoneT cbStopBindingSmartPhone;
	TpmsCbSmartPhoneBoundT cbSmartPhoneBound;
	TpmsCbSwapTransmitterPositionT cbSwapTransmitterPosition;
	TpmsCbReceiverSettingsT cbReceiverSettings;
	TpmsCbApplyReceiverSettingsT cbApplyReceiverSettings;
	TpmsCbResetReceiverToFactoryT cbResetReceiverToFactory;
	TpmsCbReceiverVersionT cbReceiverVersion;
} TpmsRegisterInfoT, *PTpmsRegisterInfoT;

BOOL ZONGMU_BLUETOOTH_API TpmsRegister(PTpmsRegisterInfoT pRegisterInfo);

BOOL ZONGMU_BLUETOOTH_API TpmsUnregister();

BOOL ZONGMU_BLUETOOTH_API TpmsGetSystemInfo();

BOOL ZONGMU_BLUETOOTH_API TpmsGetTransmitterData(TpmsTransmitterPositionT enPosition);

BOOL ZONGMU_BLUETOOTH_API TpmsGetReceiverStatus();

BOOL ZONGMU_BLUETOOTH_API TpmsGetSmartPhoneConnectStatus();

BOOL ZONGMU_BLUETOOTH_API TpmsStartBindingTransmitter(BOOL bAutoBinding);

BOOL ZONGMU_BLUETOOTH_API TpmsStopBindingTransmitter(BOOL bSaveResult);

BOOL ZONGMU_BLUETOOTH_API TpmsStartBindingSmartPhone();

BOOL ZONGMU_BLUETOOTH_API TpmsStopBindingSmartPhone();

BOOL ZONGMU_BLUETOOTH_API TpmsBindTransmitterToPosition(TpmsTransmitterPositionT enPosition);

BOOL ZONGMU_BLUETOOTH_API TpmsSwapTransmitterPosition(TpmsTransmitterPositionT enPosition1, TpmsTransmitterPositionT enPosition2);

BOOL ZONGMU_BLUETOOTH_API TpmsGetReceiverSettings();

BOOL ZONGMU_BLUETOOTH_API TpmsApplyReceiverSettings(PTpmsReceiverSettingsT pSettings);

BOOL ZONGMU_BLUETOOTH_API TpmsResetReceiverToFactory();

BOOL ZONGMU_BLUETOOTH_API TpmsGetReceiverVersion();

/*
BOOL ZONGMU_BLUETOOTH_API TpmsOn();

BOOL ZONGMU_BLUETOOTH_API TpmsOff();
*/

#ifdef __cplusplus
}
#endif

#endif
