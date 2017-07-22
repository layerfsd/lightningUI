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
	TRANSMITTER_POSITION_FRONT_LEFT = 0, //左前轮
	TRANSMITTER_POSITION_FRONT_RIGHT, //右前轮
	TRANSMITTER_POSITION_BACK_LEFT, //左后轮
	TRANSMITTER_POSITION_BACK_RIGHT, //右后轮
	TRANSMITTER_POSITION_BACKUP,	//备胎
	TRANSMITTER_POSITION_ALL = 0xFE, //所有位置
	TRANSMITTER_POSITION_UNKNOWN = 0xFF //未知位置
} TpmsTransmitterPositionT; //当前只定义了轿车的，卡车需要扩充此定义

typedef enum TpmsBatteryLevelT
{
	TPMS_BATTERY_LEVEL_HIGH = 0, //高电量
	TPMS_BATTERY_LEVEL_MEDIUM, //中电量
	TPMS_BATTERY_LEVEL_LOW, //低电量
	TPMS_BATTERY_LEVEL_UNKOWN = 0xFF //未知电量
} TpmsBatteryLevelT;

typedef enum TpmsAlarmT
{
	TPMS_ALARM_NONE = 0x00, //无警报
	TPMS_ALARM_TEMPERATURE_HIGH = 0x01, //胎温高警报
	TPMS_ALARM_PRESSURE_HIGH = 0x02, //胎压高警报
	TPMS_ALARM_PRESSURE_LOW = 0x04, //胎压低警报
	TPMS_ALARM_PRESSURE_FAST_LEAK = 0x08, //快漏气警报
	TPMS_ALARM_PRESSURE_SLOW_LEAK = 0x10, //慢漏气警报
	TPMS_ALARM_PRESSURE_UNBALANCE = 0x20, //左右胎压不均衡警报
	TPMS_ALARM_BATTERY_LOW = 0x40, //电量低警报
	TPMS_ALARM_NO_DATA = 0x80, //警报无胎压数据
	TPMS_ALARM_NOT_BOUND = 0x100 //此位置没有绑定发送器
} TpmsAlarmT;

typedef enum TpmsErrorT
{
	TPMS_ERROR_SUCCESS = 0, //命令执行成功
	TPMS_ERROR_INVALID_PARAMETER, //命令参数不正确
	TPMS_ERROR_INVALID_OPERATION, //在当前状态下无效的命令
	TPMS_ERROR_OPERATION_NOT_SUPPORTED, //操作不支持，比如外置不支持交换轮胎和手动绑定，内置不支持自动绑定
	TPMS_ERROR_BT_CONNECT_FAILED, //蓝牙连接失败
	TPMS_ERROR_BT_COMMUNICATION_FAILED, //蓝牙通信失败
	TPMS_ERROR_NOT_ENOUGH_MEMORY, // 内存分配失败	
	TPMS_ERROR_BLE_GATT, // GATT Driver操作返回错误
	
	TPMS_ERROR_UNKNOWN = 0xFF //未知错误
} TpmsErrorT;

typedef enum TpmsReceiverStatusT
{
	TPMS_RECEIVER_STATUS_STANDBY = 0, // 初始化状态
	TPMS_RECEIVER_STATUS_RECEIVING, //正常接收胎压数据
	TPMS_RECEIVER_STATUS_BINDING_TRANSMITTER, //绑定发送器
	TPMS_RECEIVER_STATUS_BINDING_SMARTPHONE, //绑定手机，safeview上没有这个状态
} TpmsReceiverStatusT;

#pragma pack(push)
#pragma pack(1)

typedef struct TpmsDataT
{
	zm_u8 position; //轮胎位置，原类型为TpmsTransmitterPositionT
	zm_u8 battery; //电池电量，原类型为TpmsBatteryLevelT
	zm_s16 temperature; //温度，单位：摄氏度
	zm_u16 pressure; //胎压，单位：0.1psi
	zm_u16 alarm; //警报，原类型为TpmsAlarmT
} TpmsDataT, *PTpmsDataT;

typedef struct TpmsReceiverSettingsT
{
	zm_s16 alarmTemperatureMax; //报警温度，单位：摄氏度
	zm_u16 alarmPressureMin; //报警低压，单位：0.1psi
	zm_u16 alarmPressureMax; //报警高压，单位：0.1psi
} TpmsReceiverSettingsT, *PTpmsReceiverSettingsT;

typedef struct ZmVersionT
{
	zm_u16 major;//主版本号
	zm_u16 minor; //次版本号
	zm_u16 build; //编译号
	zm_u32 revision;//SVN版本号
} ZmVersionT, *PZmVersionT;

typedef struct TpmsSystemInfoT
{
	zm_u8 transmitter_count; // 发送器的个数,目前固定为5个
	zm_u8 transmitter_type;	// 发送器类型,原型为TpmsSubtypeT
} TpmsSystemInfoT, *PTpmsSystemInfoT;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif //__BLUE_TPMS_DEF_H__
