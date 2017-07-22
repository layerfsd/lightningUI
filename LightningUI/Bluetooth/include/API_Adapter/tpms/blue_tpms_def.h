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
*  @file blue_tpms_def.h
*
*/

#ifndef __BLUE_TPMS_DEF_H__
#define __BLUE_TPMS_DEF_H__

#include "zm_data_type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define BAR_TO_PSI_RATIO (14.503774389728)

typedef enum ZmBlueTypeT
{
	ZM_BLUE_NONE = 0,
	ZM_BLUE_TPMS,
} ZmBlueTypeT;

typedef enum TpmsSubtypeT
{
	TPMS_NONE = 0,
	TPMS_EXTERNAL_XMIT,
	TPMS_INTERNAL_XMIT,
	TPMS_RECEIVER,
	TPMS_RELAY
} TpmsSubtypeT;

typedef enum TpmsTransmitterPositionT
{
	TRANSMITTER_POSITION_FRONT_LEFT = 0, //��ǰ��
	TRANSMITTER_POSITION_FRONT_RIGHT, //��ǰ��
	TRANSMITTER_POSITION_BACK_LEFT, //�����
	TRANSMITTER_POSITION_BACK_RIGHT, //�Һ���
	TRANSMITTER_POSITION_BACKUP,	//��̥
	TRANSMITTER_POSITION_ALL = 0xFE, //����λ��
	TRANSMITTER_POSITION_UNKNOWN = 0xFF //δ֪λ��
} TpmsTransmitterPositionT; //��ǰֻ�����˽γ��ģ�������Ҫ����˶���

typedef enum TpmsBatteryLevelT
{
	TPMS_BATTERY_LEVEL_HIGH = 0, //�ߵ���
	TPMS_BATTERY_LEVEL_MEDIUM, //�е���
	TPMS_BATTERY_LEVEL_LOW, //�͵���
	TPMS_BATTERY_LEVEL_UNKOWN = 0xFF //δ֪����
} TpmsBatteryLevelT;

typedef enum TpmsAlarmT
{
	TPMS_ALARM_NONE = 0x00, //�޾���
	TPMS_ALARM_TEMPERATURE_HIGH = 0x01, //̥�¸߾���
	TPMS_ALARM_PRESSURE_HIGH = 0x02, //̥ѹ�߾���
	TPMS_ALARM_PRESSURE_LOW = 0x04, //̥ѹ�;���
	TPMS_ALARM_PRESSURE_FAST_LEAK = 0x08, //��©������
	TPMS_ALARM_PRESSURE_SLOW_LEAK = 0x10, //��©������
	TPMS_ALARM_PRESSURE_UNBALANCE = 0x20, //����̥ѹ�����⾯��
	TPMS_ALARM_BATTERY_LOW = 0x40, //�����;���
	TPMS_ALARM_NO_DATA = 0x80, //������̥ѹ����
	TPMS_ALARM_NOT_BOUND = 0x100 //��λ��û�а󶨷�����
} TpmsAlarmT;

typedef enum TpmsErrorT
{
	TPMS_ERROR_SUCCESS = 0, //����ִ�гɹ�
	TPMS_ERROR_INVALID_PARAMETER, //�����������ȷ
	TPMS_ERROR_INVALID_OPERATION, //�ڵ�ǰ״̬����Ч������
	TPMS_ERROR_OPERATION_NOT_SUPPORTED, //������֧�֣��������ò�֧�ֽ�����̥���ֶ��󶨣����ò�֧���Զ���
	TPMS_ERROR_BT_CONNECT_FAILED, //��������ʧ��
	TPMS_ERROR_BT_COMMUNICATION_FAILED, //����ͨ��ʧ��
	TPMS_ERROR_NOT_ENOUGH_MEMORY, // �ڴ����ʧ��	
	TPMS_ERROR_BLE_GATT, // GATT Driver�������ش���
	
	TPMS_ERROR_UNKNOWN = 0xFF //δ֪����
} TpmsErrorT;

typedef enum TpmsReceiverStatusT
{
	TPMS_RECEIVER_STATUS_STANDBY = 0, // ��ʼ��״̬
	TPMS_RECEIVER_STATUS_RECEIVING, //��������̥ѹ����
	TPMS_RECEIVER_STATUS_BINDING_TRANSMITTER, //�󶨷�����
	TPMS_RECEIVER_STATUS_BINDING_SMARTPHONE, //���ֻ���safeview��û�����״̬
} TpmsReceiverStatusT;

#pragma pack(push)
#pragma pack(1)

typedef struct TpmsDataT
{
	zm_u8 position; //��̥λ�ã�ԭ����ΪTpmsTransmitterPositionT
	zm_u8 battery; //��ص�����ԭ����ΪTpmsBatteryLevelT
	zm_s16 temperature; //�¶ȣ���λ�����϶�
	zm_u16 pressure; //̥ѹ����λ��0.1psi
	zm_u16 alarm; //������ԭ����ΪTpmsAlarmT
} TpmsDataT, *PTpmsDataT;

typedef struct TpmsReceiverSettingsT
{
	zm_s16 alarmTemperatureMax; //�����¶ȣ���λ�����϶�
	zm_u16 alarmPressureMin; //������ѹ����λ��0.1psi
	zm_u16 alarmPressureMax; //������ѹ����λ��0.1psi
} TpmsReceiverSettingsT, *PTpmsReceiverSettingsT;

typedef struct ZmVersionT
{
	zm_u16 major;//���汾��
	zm_u16 minor; //�ΰ汾��
	zm_u16 build; //�����
	zm_u32 revision;//SVN�汾��
} ZmVersionT, *PZmVersionT;

typedef struct TpmsSystemInfoT
{
	zm_u8 transmitter_count; // �������ĸ���,Ŀǰ�̶�Ϊ5��
	zm_u8 transmitter_type;	// ����������,ԭ��ΪTpmsSubtypeT
} TpmsSystemInfoT, *PTpmsSystemInfoT;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif //__BLUE_TPMS_DEF_H__
