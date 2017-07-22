#ifndef TPMS_API_H
#define TPMS_API_H

#include "tpms\blue_tpms_sdk.h"

#define TPMS_TRANSMITTER_COUNT 5
#define TPMS_TEMPERATUR_RANGE_MIN 50
#define TPMS_TEMPERATUR_RANGE_MAX 90
#define TPMS_PRESSURE_RANGE_MIN 10
#define TPMS_PRESSURE_RANGE_MAX 40
#define TPMS_ALARM_DISMISS_TIMEOUT 120000

typedef enum BT_OPERATION
{
	BT_OPERATION_CURRENT,
	BT_OPERATION_INIT,
	BT_OPERATION_TPMS_ALARM,
	BT_OPERATION_BIND,
	BT_OPERATION_START_BINDING_TRANSMITTER,
	BT_OPERATION_STOP_BINDING_TRANSMITTER,
	BT_OPERATION_TRANSMITTER_FOUND,
	BT_OPERATION_BIND_TRANSMITTER_TO_POSITION,
	BT_OPERATION_SWAP_TRANSMITTER,
	BT_OPERATION_BIND_SMARTPHONE,
	BT_OPERATION_APPLY_SETTINGS,
} BT_OPERATION;

typedef struct stTpmsWarningData
{
	stTpmsWarningData(int _nPostion, int _nAlarm)
	{
		nPosition = (TpmsTransmitterPositionT)_nPostion;
		nAlarm = (TpmsAlarmT)_nAlarm;
	}

	TpmsTransmitterPositionT nPosition;		//Tyre
	TpmsAlarmT				 nAlarm;		//alarm
} TpmsWarningDataT;

class CApi_TPMS	: public CApi_base
{
public:
    CApi_TPMS(void);
    /* Define the virtual de-constructor to free SfWE SDK. Otherwise memory leak occurs. */
    virtual ~CApi_TPMS(void);

    virtual BOOL Initialize();
    virtual void DeInitialize();

	void SetDefaultAlarmValue();

protected:
	static void tpmsSystemInfoReceived(TpmsErrorT enError, PTpmsSystemInfoT pInfo);

	static void tpmsTransmitterDataReceived(TpmsErrorT enError, PTpmsDataT pData);

	static void tpmsReceiverStatusReceived(TpmsErrorT enError, TpmsReceiverStatusT enStatus);

	static void tpmsSmartPhoneConnectStatusReceived(TpmsErrorT enError, BOOL bConnected);

	static void tpmsStartBindingTransmitterReceived(TpmsErrorT enError);

	static void tpmsStopBindingTransmitterReceived(TpmsErrorT enError);

	static void tpmsTransmitterFound();

	static void tpmsTransmitterBound(TpmsErrorT enError, TpmsTransmitterPositionT enPosition);

	static void tpmsStartBindingSmartPhoneReceived(TpmsErrorT enError);

	static void tpmsStopBindingSmartPhoneReceived(TpmsErrorT enError);

	static void tpmsSmartPhoneBound(TpmsErrorT enError);

	static void tpmsTransmitterPositionSwapped(TpmsErrorT enError);

	static void tpmsReceiverSettingsReceived(TpmsErrorT enError, PTpmsReceiverSettingsT pSettings);

	static void tpmsReceiverSettingsApplied(TpmsErrorT enError);

	static void tpmsReceiverResetToFactory(TpmsErrorT enError);

	static void tpmsReceiverVersionReceived(TpmsErrorT enError, PZmVersionT pVersion);

};

#endif // TPMS_API_H
