/******************************************************************************************************

				CSR plc 2007

                All rights reserved

FILE:           bluetooth_notification.h

DESCRIPTION:    Notification API for Application



*******************************************************************************************************/
#ifndef _BLUETOOTH_NOTIFICATION_H
#define _BLUETOOTH_NOTIFICATION_H

#include <winsock2.h>

#ifndef __cplusplus
extern "C" {
#endif

                                                                     
//  id of post message to notify about a change                      
#define BLUETOOTH_NOTIFICATION_SIGNAL_WINDOW_TEXT                    TEXT("a146d2cd-fa5f-423b-8257-9b70984a8a79")
                                                                     
// General notify ID                                                 
#define BLUETOOTH_NOTIFY_GENERAL_STATE_ID                            0x00000000
#define BLUETOOTH_NOTIFY_GENERAL_ADDRESS_ID                          0x00000001
#define BLUETOOTH_NOTIFY_GENERAL_NAME_MAX_ID                         0x00000002
                                                                     
// General notify text                                               
#define BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_IDLE                    TEXT("Idle")
#define BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTING              TEXT("Connecting")
#define BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_CONNECTED               TEXT("Connected")
#define BLUETOOTH_NOTIFY_GENERAL_STATE_VALUE_DISCONNECTING           TEXT("Disconnecting")
                                                                     
#define BLUETOOTH_NOTIFY_MODULE_ID_MASK                              0xFFFF0000
#define	BLUETOOTH_NOTIFY_NAME_ID_MASK                                0x0000FFFF
                                                                     
// Handsfree notify ID                                               
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE                        0x00000002
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_LINE_IDENTIFICATION          0x00000003
#define BLUETOOTH_NOTIFY_HANDSFREE_SERVICE                           0x00000004
#define BLUETOOTH_NOTIFY_HANDSFREE_CONNECTION_TYPE                   0x00000005
#define BLUETOOTH_NOTIFY_HANDSFREE_BATTERY_LEVEL                     0x00000006
#define BLUETOOTH_NOTIFY_HANDSFREE_PHONE_SIGNAL_STRENGTH             0x00000007
#define BLUETOOTH_NOTIFY_HANDSFREE_SPEAKER_VOLUME                    0x00000008
#define BLUETOOTH_NOTIFY_HANDSFREE_MIC_VOLUME                        0x00000009
#define BLUETOOTH_NOTIFY_HANDSFREE_THREE_WAY_FEATURE_SUPPORT         0x0000000A
#define BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS                       0x0000000B
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_WAITING_NOTIFICATION         0x0000000C
#define BLUETOOTH_NOTIFY_HANDSFREE_SPECIAL_1_SPEAKER_GAIN            0x0000000D
#define BLUETOOTH_NOTIFY_HANDSFREE_SPECIAL_2_MIC_GAIN                0x0000000E
/* [SfWE] Update to add full notification for HFM. */
#define BLUETOOTH_NOTIFY_HANDSFREE_NAME_ID_MAX                       0x0000000F




// Handsfree notify ID
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_STATE_TEXT                   TEXT("Call state")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_LINE_IDENTIFICATION_TEXT     TEXT("Clip")
#define BLUETOOTH_NOTIFY_HANDSFREE_SERVICE_TEXT                      TEXT("Service")
#define BLUETOOTH_NOTIFY_HANDSFREE_CONNECTION_TYPE_TEXT              TEXT("Connection type")
#define BLUETOOTH_NOTIFY_HANDSFREE_BATTERY_LEVEL_TEXT                TEXT("Battery level")
#define BLUETOOTH_NOTIFY_HANDSFREE_PHONE_SIGNAL_STRENGTH_TEXT        TEXT("Signal strength")
#define BLUETOOTH_NOTIFY_HANDSFREE_SPEAKER_VOLUME_TEXT               TEXT("Speaker volume")
#define BLUETOOTH_NOTIFY_HANDSFREE_MIC_VOLUME_TEXT                   TEXT("Mic volume")
#define BLUETOOTH_NOTIFY_HANDSFREE_THREE_WAY_FEATURE_SUPPORT_TEXT    TEXT("Three way feature")
#define BLUETOOTH_NOTIFY_HANDSFREE_HELD_STATUS_TEXT                  TEXT("Held status")
#define BLUETOOTH_NOTIFY_HANDSFREE_CALL_WAITING_NOTIFICATION_TEXT    TEXT("Call waiting notification")
#define BLUETOOTH_NOTIFY_HANDSFREE_SPECIAL_1_SPEAKER_GAIN_TEXT       TEXT("Special 1")
#define BLUETOOTH_NOTIFY_HANDSFREE_SPECIAL_2_MIC_GAIN_TEXT           TEXT("Special 2")



// File Tranfer Notify ID
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_STATE                      0x00000002
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_FILE_NAME                  0x00000003
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_FILE_SIZE                  0x00000004
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_BYTES_TRANSFERRED          0x00000005
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_AMP                        0x00000006
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_CURRENT_THROUGHPUT         0x00000007
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_AVERAGE_THROUGHPUT         0x00000008
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_NAME_ID_MAX                0x00000009

// File Tranfer Notify Text
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_STATE_TEXT                 TEXT("TransferringState")
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_FILE_NAME_TEXT             TEXT("FileName")
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_FILE_SIZE_TEXT             TEXT("FileSize")
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_BYTES_TRANSFERRED_TEXT     TEXT("Transferred")
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_USING_AMP_TEXT            TEXT("AMP")  
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_CURRENT_THROUGHPUT_TEXT     TEXT("CurThruput") /* Current Thourghput */
#define BLUETOOTH_NOTIFY_FILE_TRANSFERING_AVERAGE_THROUGHPUT_TEXT     TEXT("AvgThruput") /* Average Throughput */

// Print Manager notify ID
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_START					     0x00000010
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_PRINTER_STATE			     0x00000010
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_PRINTER_REASON			     0x00000011
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_JOB_STATE				     0x00000012
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_RECONNECT_TIME			     0x00000013
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_END						     0x00000013
                                                                     
// Print Manager notify text                                         
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_PRINTER_STATE_TEXT		     TEXT("PrinterState")
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_PRINTER_REASON_TEXT		     TEXT("PrinterReason")
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_JOB_STATE_TEXT			     TEXT("JobState")
#define BLUETOOTH_NOTIFY_PRINT_MANAGER_RECONNECT_TIME_TEXT		     TEXT("ReconnectTime")
                                                                     
// Module ID                                                         
/* [SfWE] Replace previous definition which isn't used any more due to module ID's limitation. */
//#define BLUETOOTH_MODULE_AUDIO_SOURCE                              0x00010000
#define BLUETOOTH_MODULE_VERIFICATION                                0x00010000
#define BLUETOOTH_MODULE_FILE_TRANSFER_CLIENT                        0x00020000
//#define BLUETOOTH_MODULE_FILE_TRANSFER_SERVER                      0x00040000
#define BLUETOOTH_MODULE_OPP_SERVER                                  0x00040000
#define BLUETOOTH_MODULE_HANDSFREE                                   0x00080000
#define BLUETOOTH_MODULE_GAP                                         0x00100000
//#define BLUETOOTH_MODULE_IMAGE_CLIENT                              0x00100000
#define BLUETOOTH_MODULE_IMAGE_SERVER                                0x00200000
#define BLUETOOTH_MODULE_COEX                                        0x00400000
//#define BLUETOOTH_MODULE_PRINTER_SENDER                            0x00400000
#define BLUETOOTH_MODULE_A2DP_SINK                                   0x00800000
#define BLUETOOTH_MODULE_AVRCP_CT                                    0x01000000
#define BLUETOOTH_MODULE_PHONEBOOK_CLIENT						     0x02000000
#define BLUETOOTH_MODULE_HID_DEVICE                                  0x04000000
#define BLUETOOTH_MODULE_DUN_CLIENT                                  0x08000000
#define BLUETOOTH_MODULE_MAP_CLIENT                                  0x10000000
#define BLUETOOTH_MODULE_MSPXYM                                      0x20000000
#define BLUETOOTH_MODULE_SPP                                         0x40000000
#define BLUETOOTH_MODULE_TPMS_CLIENT                                 0x80000000
#define BLUETOOTH_MODULE_DID										 0x00000000


// Module text                                                       
#define BLUETOOTH_MODULE_AUDIO_SOURCE_TEXT                           AV_MODULE
#define BLUETOOTH_MODULE_FILE_TRANSFER_CLIENT_TEXT                   FTC_MODULE 
#define BLUETOOTH_MODULE_FILE_TRANSFER_SERVER_TEXT                   FTS_MODULE 
#define BLUETOOTH_MODULE_HANDSFREE_TEXT                              HF_MODULE 
#define BLUETOOTH_MODULE_IMAGE_CLIENT_TEXT                           BIPC_MODULE
#define BLUETOOTH_MODULE_IMAGE_SERVER_TEXT                           BIPS_MODULE
#define BLUETOOTH_MODULE_PRINTER_SENDER_TEXT                         BPP_MODULE
#define BLUETOOTH_MODULE_SIM_ACCESS_CLIENT_TEXT                      SAPC_MODULE
#define BLUETOOTH_MODULE_SIM_ACCESS_SERVER_TEXT                      SAPS_MODULE
#define BLUETOOTH_MODULE_PHONEBOOK_CLIENT_TEXT                       PAC_MODULE
#define BLUETOOTH_MODULE_HID_DEVICE_TEXT                             HIDD_MODULE
#define BLUETOOTH_MODULE_DUN_CLIENT_TEXT                             DUNC_DRIVER 
#define BLUETOOTH_MODULE_PBAP_SERVER_TEXT                            PAS_MODULE
#define BLUETOOTH_MODULE_TPMS_CLIENT_TEXT							 TPMS_MODULE
#define BLUETOOTH_MODULE_PBAP_SERVER_TEXT                            PAS_MODULE
#define BLUETOOTH_MODULE_MSPXYM_TEXT                                 MSPXYM_MODULE
#define BLUETOOTH_MODULE_A2DP_SNK_TEXT                               A2DP_MODULE
#define BLUETOOTH_MODULE_AVRCP_CT_TEXT                               AVRCP_MODULE
#define BLUETOOTH_MODULE_MAP_CLIENT_TEXT                             MAPC_MODULE
#define BLUETOOTH_MODULE_SPP_TEXT                                    SPP_MODULE
#define BLUETOOTH_MODULE_OPP_SERVER_TEXT                             OPPS_MODULE 
#define BLUETOOTH_MODULE_COEX_TEXT                                   COEX_MODULE 
#define BLUETOOTH_MODULE_GAP_TEXT                                    GAP_MODULE 
#define BLUETOOTH_MODULE_DID_TEXT									 DID_MODULE

typedef BYTE            BluetoothHWErrorCodeT;

#define	 HCI_HW_NO_FAULT	                                    (BluetoothHWErrorCodeT)(0x00)                                       
#define	 HCI_HW_FAULT_MYSTERY	                                (BluetoothHWErrorCodeT)(0x01)                                       
#define	 HCI_HW_FAULT_NO_BDADDR	                                (BluetoothHWErrorCodeT)(0x02)                                       
#define	 HCI_HW_FAULT_NO_BDCOUNTRYCODE	                        (BluetoothHWErrorCodeT)(0x03)                                       
#define	 HCI_HW_FAULT_NO_BDCOD	                                (BluetoothHWErrorCodeT)(0x04)                                       
#define	 HCI_HW_FAULT_PS_FULL	                                (BluetoothHWErrorCodeT)(0x05)                                       
#define	 HCI_HW_FAULT_RSM_EVENT	                                (BluetoothHWErrorCodeT)(0x06)                                       
#define	 HCI_HW_FAULT_INTERRUPT_BLOCK	                        (BluetoothHWErrorCodeT)(0x07)                                       
#define	 HCI_HW_FAULT_BUF_UNGET	                                (BluetoothHWErrorCodeT)(0x08)                                       
#define	 HCI_HW_FAULT_DUFF_ACLID	                            (BluetoothHWErrorCodeT)(0x09)                                      
#define	 HCI_HW_FAULT_HOSTIO_CHKSUM_ERROR	                    (BluetoothHWErrorCodeT)(0x0b)                                       
#define	 HCI_HW_FAULT_HOSTIO_INTERNAL_ERROR	                    (BluetoothHWErrorCodeT)(0x0c)                                       
#define	 HCI_HW_FAULT_HOSTIO_INVALID_ACK	                    (BluetoothHWErrorCodeT)(0x0d)                                       
#define	 HCI_HW_FAULT_HOSTIO_DISCARDED_HCI_DATA	                (BluetoothHWErrorCodeT)(0x0f)                                       
#define	 HCI_HW_FAULT_HOSTIO_CONVERT_FAILURE	                (BluetoothHWErrorCodeT)(0x10)                                       
#define	 HCI_HW_FAULT_USB_BUFFER_ERROR	                        (BluetoothHWErrorCodeT)(0x11)                                       
#define	 HCI_HW_FAULT_SYNTH	                                    (BluetoothHWErrorCodeT)(0x12)                                       
#define	 HCI_HW_FAULT_HOSTIO_WRITE_UNHANDLED	                (BluetoothHWErrorCodeT)(0x13)                                       
#define	 HCI_HW_FAULT_HOSTIO_WRITE_FAILED	                    (BluetoothHWErrorCodeT)(0x14)                                       
#define	 HCI_HW_FAULT_HOSTIO_WRITE_DISABLED	                    (BluetoothHWErrorCodeT)(0x15)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_UNHANDLED	                    (BluetoothHWErrorCodeT)(0x16)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_DISCARDED	                    (BluetoothHWErrorCodeT)(0x17)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_BCSPLE	                (BluetoothHWErrorCodeT)(0x18)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_BCCMD	                (BluetoothHWErrorCodeT)(0x19)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_HQ	                    (BluetoothHWErrorCodeT)(0x1a)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_DM	                    (BluetoothHWErrorCodeT)(0x1b)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_HCI	                    (BluetoothHWErrorCodeT)(0x1c)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_L2CAP	                (BluetoothHWErrorCodeT)(0x1d)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_RFCOMM	                (BluetoothHWErrorCodeT)(0x1e)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_SDD	                    (BluetoothHWErrorCodeT)(0x1f)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_DEBUG	                (BluetoothHWErrorCodeT)(0x20)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_DFU	                    (BluetoothHWErrorCodeT)(0x21)                                       
#define	 HCI_HW_FAULT_HOSTIO_READ_CONV_VM	                    (BluetoothHWErrorCodeT)(0x22)                                       
#define	 HCI_HW_FAULT_H4_RX_BAD_PDU	                            (BluetoothHWErrorCodeT)(0x23)                                       
#define	 HCI_HW_FAULT_HOSTIO_INVALID_HCI_HANDLE	                (BluetoothHWErrorCodeT)(0x26)                                       
#define	 HCI_HW_FAULT_LC_INBOUND	                            (BluetoothHWErrorCodeT)(0x27)                                       
#define	 HCI_HW_FAULT_LC_INBOUND_PP_LMP_LOST	                (BluetoothHWErrorCodeT)(0x28)                                       
#define	 HCI_HW_FAULT_LC_INBOUND_BC_DATA_LOST	                (BluetoothHWErrorCodeT)(0x29)                                       
#define	 HCI_HW_FAULT_LC_INBOUND_BC_LMP_LOST	                (BluetoothHWErrorCodeT)(0x2a)                                       
#define	 HCI_HW_FAULT_LC_RESET_FAILED	                        (BluetoothHWErrorCodeT)(0x2b)                                       
#define	 HCI_HW_FAULT_LC_NO_LMP_BUFFER	                        (BluetoothHWErrorCodeT)(0x2c)                                       
#define	 HCI_HW_FAULT_LC_NO_LMP_BUFFER_SPACE	                (BluetoothHWErrorCodeT)(0x2d)                                       
#define	 HCI_HW_FAULT_LC_BG_MSG_ID	                            (BluetoothHWErrorCodeT)(0x2e)                                       
#define	 HCI_HW_FAULT_RADIOIO_ACL_BUFFER_CORRUPT	            (BluetoothHWErrorCodeT)(0x2f)                                       
#define	 HCI_HW_FAULT_DUFF_PSBC_DATA	                        (BluetoothHWErrorCodeT)(0x30)                                       
#define	 HCI_HW_FAULT_I2C_EEPROM_UNCONFIGURED	                (BluetoothHWErrorCodeT)(0x31)                                       
#define	 HCI_HW_FAULT_MATHS_ERROR	                            (BluetoothHWErrorCodeT)(0x32)                                       
#define	 HCI_HW_FAULT_FLASH_NOT_RECOGNISED	                    (BluetoothHWErrorCodeT)(0x33)                                       
#define	 HCI_HW_FAULT_UNEXPECTED_ACL_SHUTDOWN	                (BluetoothHWErrorCodeT)(0x34)                                       
#define	 HCI_HW_FAULT_INVALID_PATCH	                            (BluetoothHWErrorCodeT)(0x35)                                       
#define	 HCI_HW_FAULT_COEX_MISSCONFIGURED	                    (BluetoothHWErrorCodeT)(0x36)                                       
#define	 HCI_HW_FAULT_ALREADY_BAGSIED	                        (BluetoothHWErrorCodeT)(0x37)                                       
#define	 HCI_HW_FAULT_BAD_BC_DEST_ADDRESS	                    (BluetoothHWErrorCodeT)(0x38)                                       
#define	 HCI_HW_FAULT_IN_USE_PCM_BUFFER_DESTROYED	            (BluetoothHWErrorCodeT)(0x39)                                       
#define	 HCI_HW_FAULT_ODD_SIZE_PMALLOC_POOL	                    (BluetoothHWErrorCodeT)(0x3a)                                       
#define	 HCI_HW_FAULT_FM_AUDIO_ROUTING	                        (BluetoothHWErrorCodeT)(0x3b)                                       
#define	 HCI_HW_FAULT_EXT_SLOW_CLOCK_CAL_FAILED	                (BluetoothHWErrorCodeT)(0x3c)                                       
#define	 HCI_HW_FAULT_LC_FG_WATCHDOG_BARKED	                    (BluetoothHWErrorCodeT)(0x3d)                                       
#define	 HCI_HW_FAULT_PSFL_CORRUPT_OR_MISSING	                (BluetoothHWErrorCodeT)(0x3e)                                       
#define	 HCI_HW_FAULT_ADC_TENBIT_TIMEOUT	                    (BluetoothHWErrorCodeT)(0x3f)                                       
#define	 HCI_HW_FAULT_INVALID_SDIO_SERVICE_ID	                (BluetoothHWErrorCodeT)(0x40)                                       
#define	 HCI_HW_FAULT_HCI_INVALID_REASON_CODE	                (BluetoothHWErrorCodeT)(0x41)                                       
#define	 HCI_HW_FAULT_HAL_FM_RELINQUISH_ADC2_UNBALANCED	        (BluetoothHWErrorCodeT)(0x42)                                       
#define	 HCI_HW_FAULT_USB_STRING_DESCRIPTORS_M	                (BluetoothHWErrorCodeT)(0x43)                                       
#define	 HCI_HW_FAULT_LC_LMP_IN_INVALID_PACKET	                (BluetoothHWErrorCodeT)(0x44)                                       
#define	 HCI_HW_FAULT_FREQ_COUNTER_IN_USE	                    (BluetoothHWErrorCodeT)(0x45)                                       
#define	 HCI_HW_FAULT_SLOW_CLOCK_TOO_SLOW	                    (BluetoothHWErrorCodeT)(0x46)                                       
#define	 HCI_HW_FAULT_HAL_FM_LO	                                (BluetoothHWErrorCodeT)(0x47)                                       
#define	 HCI_HW_FAULT_FM_API_COMMITTED	                        (BluetoothHWErrorCodeT)(0x48)                                       
#define	 HCI_HW_FAULT_HAL_FM_TX	                                (BluetoothHWErrorCodeT)(0x49)                                       
#define	 HCI_HW_FAULT_WRONG_ANA_HARDWARE_VERSION	            (BluetoothHWErrorCodeT)(0x4a)                                       
#define	 HCI_HW_FAULT_ADC_RUNNING	                            (BluetoothHWErrorCodeT)(0x4b)                                       
#define	 HCI_HW_FAULT_LM_SPURIOUS_TIMEOUT	                    (BluetoothHWErrorCodeT)(0x4c)                                       
#define	 HCI_HW_FAULT_NO_REFERENCE_CLOCK	                    (BluetoothHWErrorCodeT)(0x4d)                                       
#define	 HCI_HW_FAULT_SUSPEND_RESOLVE_ENABLES_REF_CNT	        (BluetoothHWErrorCodeT)(0x4e)                                       
#define	 HCI_HW_FAULT_LPC_PMEM_WRITABLE	                        (BluetoothHWErrorCodeT)(0x4f)                                       
#define	 HCI_HW_FAULT_FASTPIPE_INVALID_ID	                    (BluetoothHWErrorCodeT)(0x50)                                       
#define	 HCI_HW_FAULT_ADC_STOLEN	                            (BluetoothHWErrorCodeT)(0x51)                                       
#define	 HCI_HW_FAULT_BLE_INVALID_CHANNEL_MAP_RECEIVED	        (BluetoothHWErrorCodeT)(0x52)                                       
#define	 HCI_HW_FAULT_BLE_UNBALANCED_RADIO_SETUP	            (BluetoothHWErrorCodeT)(0x53)                                       
#define	 HCI_HW_FAULT_BLE_INCORRECT_RADIO_PARAMS	            (BluetoothHWErrorCodeT)(0x54)                                       
#define	 HCI_HW_FAULT_HAL_FMTX_ANT_UNCALIBRATED	                (BluetoothHWErrorCodeT)(0x55)                                       
#define	 HCI_HW_FAULT_WRONG_BT_POWERTABLE_FOUND	                (BluetoothHWErrorCodeT)(0x56)                                       
#define	 HCI_HW_FAULT_REFLESS_ESCO_COUNT_NEGATIVE	            (BluetoothHWErrorCodeT)(0x57)                                       
#define	 HCI_HW_FAULT_HOSTIO_FROZEN_HANDLES	                    (BluetoothHWErrorCodeT)(0x58)                                       
#define	 HCI_HW_FAULT_PCM_DEFAULT_AUDIO_ROUTING	                (BluetoothHWErrorCodeT)(0x59)                                       
#define	 HCI_HW_FAULT_CLOCK_SWITCHING_MISCONFIGURED	            (BluetoothHWErrorCodeT)(0x5a)                                       
#define	 HCI_HW_FAULT_PERIODIC_TRIM_OVERRUN	                    (BluetoothHWErrorCodeT)(0x5b)                                       
#define	 HCI_HW_FAULT_RDF_SAMPLER_OVERFLOW	                    (BluetoothHWErrorCodeT)(0x5d)                                       
#define	 HCI_HW_FAULT_PMALLOC_BASE_INSUFFICIENT_SPACE	        (BluetoothHWErrorCodeT)(0x5e)                                       
#define	 HCI_HW_FAULT_PMALLOC_ACL_INSUFFICIENT_SPACE	        (BluetoothHWErrorCodeT)(0x5f)                                       
#define	 HCI_HW_FAULT_PMALLOC_SCO_INSUFFICIENT_SPACE	        (BluetoothHWErrorCodeT)(0x60)                                       
#define	 HCI_HW_FAULT_PMALLOC_APP_INSUFFICIENT_SPACE	        (BluetoothHWErrorCodeT)(0x61)                                       
#define	 HCI_HW_FAULT_PMALLOC_INVALID_PSKEY_LENGTH	            (BluetoothHWErrorCodeT)(0x62)                                       
#define	 HCI_HW_FAULT_PMALLOC_INVALID_POOL_ENTR	                (BluetoothHWErrorCodeT)(0x63)                                       
#define	 HCI_HW_FAULT_PMALLOC_ODD_POOL_SIZE	                    (BluetoothHWErrorCodeT)(0x64)                                       
#define	 HCI_HW_FAULT_INVALID_TEMPERATURE	                    (BluetoothHWErrorCodeT)(0x65)                                       
#define	 HCI_HW_FAULT_ANA_REGULATOR_VOLTAGE	                    (BluetoothHWErrorCodeT)(0x66)                                       
#define	 HCI_HW_FAULT_GPS_BUFFER	                            (BluetoothHWErrorCodeT)(0x67)                                       
#define	 HCI_HW_FAULT_GPS_VCO_CALIBRATION	                    (BluetoothHWErrorCodeT)(0x68)                                       
#define	 HCI_HW_FAULT_KALIMBA_UNSUPPORTED_MSG	                (BluetoothHWErrorCodeT)(0x69)                                       
#define	 HCI_HW_FAULT_PCM_INVAL	                                (BluetoothHWErrorCodeT)(0x6a)                                       
#define	 HCI_HW_FAULT_INVALID_PATCH_BUNDLE	                    (BluetoothHWErrorCodeT)(0x6b)                                       
#define	 HCI_HW_FAULT_BLE_INVALID_CONN_REQ_PARAM_RECEIVED	    (BluetoothHWErrorCodeT)(0x6c)                                       
#define	 HCI_HW_FAULT_HCI_ACL_PACKET_NO_PAYLOAD	                (BluetoothHWErrorCodeT)(0x6d)                                       
#define	 HCI_HW_FAULT_SLOW_CLOCK_INT_DIV_FREF_NO_XTAL	        (BluetoothHWErrorCodeT)(0x6e)                                       
#define	 HCI_HW_FAULT_DSPMANAGER_COMPONENT_IN_DSP_BAD_MESSAGE	(BluetoothHWErrorCodeT)(0x76)                                       
#define	 HCI_HW_FAULT_DSPMANAGER_INTERNAL_RESET	                (BluetoothHWErrorCodeT)(0x79)                                       
#define	 HCI_HW_FAULT_H4_LINK_SYNC_FAILURE	                    (BluetoothHWErrorCodeT)(0xfe)                                       

// maps both 0x0a and 0x09 to HCI_HW_FAULT_DUFF_ACLID
#define Byte2HWErrorCode(err)  (((err) & 0xff) == 0x0a ? HCI_HW_FAULT_DUFF_ACLID : ((err) & 0xff))

// Macro to create the full registry path
#define BLUETOOTH_NOTIFY(module)									 (CSR_SNM_REGISTRY_ROOT module)

// Default registry root key
#define BLUETOOTH_NOTIFY_ROOT                                        (HKEY_LOCAL_MACHINE)

//  Typedefs
typedef DWORD           BluetoothNotifyNameIdT;
typedef DWORD           BluetoothNotifyModuleIdT;
typedef DWORD           BluetoothNotifyIdentifierT;


// Notification functions
DWORD WINAPI BluetoothNotificationResolveString(BluetoothNotifyIdentifierT identifier, TCHAR** str);
DWORD WINAPI BluetoothNotificationResolveValue(BluetoothNotifyIdentifierT identifier, DWORD* value);

// Util functions
DWORD WINAPI BluetoothSetDefaultObexUserIdPassword(TCHAR *userid, TCHAR* password);

#ifndef __cplusplus
}
#endif

#endif
