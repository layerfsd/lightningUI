/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WdkDualModeWifiApi.h#1 $ */
//-----------------------------------------------------------------------------
//
//  Copyright (c) Qualnetics Corporation. All rights reserved.
//  Contact Qualnetics Corporation for a copy of the license agreement.
//
//-----------------------------------------------------------------------------
//
/// $Author: xg01 $  
/// $Revision: #1 $  
/// $Date: 2013/07/11 $  
//
//-----------------------------------------------------------------------------
/// \brief      WDK Dual Mode Wifi APIs 
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDK_DUALMODEWIFI_API_H_
#define _WDK_DUALMODEWIFI_API_H_
#pragma once

#include <WdkVersion.h>
#include <WifiDKDef.h>
#include <WdkVersion.h>
#include <WdkWps.h>
#include <WdkWmm.h>
#include <WdkWifiStaApi.h>

/// \brief enumeration used to identify client and ap radios
typedef enum _WDK_WIFIADAPTER
{
    WDK_WIFI_NONE   = 0,                ///< NULL Adapter
    WDK_WIFI_AP     = 1,                ///< AP Adapter
    WDK_WIFI_STA    = 2,                ///< STA Adapter
} WDK_WIFIADAPTER, *PWDK_WIFIADAPTER;
#define WDK_WIFI_CLIENT    WDK_WIFI_STA

/// \brief enumeration used to identify supported cipher modes
typedef enum _DMW_CIPHER
{
    Cipher_Disabled     =0x00000000,    ///< Cipher is disabled
    Cipher_WEP64        =0x00000002,    ///< use 64bit WEP Cipher, valid only with WPA_Mode_Disabled
    Cipher_WEP128       =0x00000006,    ///< use 128bit WEP Cipher, valid only with WPA_Mode_Disabled
    Cipher_TKIP         =0x00000010,    ///< use TKIP, NOT valid with WPA_Mode_Disabled
    Cipher_AES          =0x00000020,    ///< use AES, NOT valid with WPA_Mode_Disabled
    Cipher_TKIP_AES     =0x00000030     ///< use TKIP or AES, NOT valid with WPA_Mode_Disabled
} DMW_CIPHER, *PDMW_CIPHER;

/// \brief enumeration used to identify supported wpa encryption modes
typedef enum _DMW_WPA_MODE
{
    WPA_Mode_Disabled   =0x00000000,    ///< disable all WPA modes
    WPA_Mode_WPA        =0x00000001,    ///< enable only WPA
    WPA_Mode_WPA2       =0x00000002,    ///< enable only WPA2
    WPA_Mode_BOTH       =0x00000003,    ///< enable both WPA and WPA2
} DMW_WPA_MODE, *PDMW_WPA_MODE;

/// \brief enumeration used to identify supported wpa encryption modes
typedef enum _DMW_WAPI_MODE
{
    WAPI_Mode_Disabled      =0x00000000,    ///< disable all WPA modes
    WAPI_Mode_PreKey        =0x00000001,    ///< enable pre-shared key mode
    WAPI_Mode_Certificates  =0x00000002,    ///< enable Certificates key mode
} DMW_WAPI_MODE, *PDMW_WAPI_MODE;

/// \brief structure to wrap a cipher/encryption mode pair
typedef struct _DMW_SECURITY_PAIR
{
    DMW_WPA_MODE eWpaMode;  ///< WPA mode
    DMW_CIPHER eCipher;     ///< Cipher mode
}DMW_SECURITY_PAIR, *PDMW_SECURITY_PAIR;    ///< Pointer to _DMW_SECURITY_PAIR_



#define MAX_SSID            32      ///< maximum number of characters allowed in an SSID
#define MAX_WEP_KEYS        104     ///< maximum size of the complete wep key array (4x26)
#define MAX_WEP_KEY_64      10      ///< size of an individual 64bit wep key
#define MAX_WEP_KEY_128     26      ///< size of an individual 128bit wep key
#define COUNT_WEP_KEYS      4       ///< count of wep keys supported in the structure
#define MIN_WPA_PHRASE      8       ///< minimum size of a WPA passphrase allowed into the api
#define MAX_WPA_PHRASE      64      ///< maximum size of a WPA passpharse allowed into the api
#define MIN_WAPI_PHRASE     8       ///< minimum size of a WAPI passphrase allowed into the api
#define MAX_WAPI_PHRASE     64      ///< maximum size of a WAPI passphrase allowed into the api
#define MAX_COUNTRY_CODE    2       ///< size of the country code member

/// \brief structure containing the summary of the DMW settings
typedef struct _DMW_AP_SETTINGS_
{
    DWORD dwSize;               ///< set to sizeof(_DMW_AP_SETTINGS_) before calling get/set
    CHAR szSSID[MAX_SSID+1];    ///< ssid value
    DWORD dwBeaconPeriod;       ///< beacon period in kusec counts
    DWORD dwConfigReserved;     ///< set to zero
    DWORD dwChannelId;          ///< channel to use 1-14 or in use
    DMW_CIPHER eCipher;         ///< cipher to use or in use
    DMW_WPA_MODE eWpaMode;      ///< encyption mode to use or in use for WPA/WPA2
    DMW_WAPI_MODE eWapiMode;    ///< encyption mode to use or in use for WAPI
    DWORD dwDefaultKeyId;       ///< default wep key 1-4
    CHAR szWepKey[COUNT_WEP_KEYS][MAX_WEP_KEY_128+1];   ///< wep key data (write only)
    CHAR szWpaKey[MAX_WPA_PHRASE+1];                    ///< wpa passphrase data (write only)
    CHAR szWapiKey[MAX_WAPI_PHRASE+1];                  ///< wapi passphrase data (write only)
    BOOL fHideSSID;                                     ///< AP is hidden
    DWORD dwAuthTimeout;                                ///< minutes allowed for auth to complete
    DWORD dwAssocTimeout;                               ///< minutes allowed for assoc to complete

} DMW_AP_SETTINGS, *PDMW_AP_SETTINGS;   ///< Pointer to _DMW_AP_SETTINGS_

/// \brief enumeration used to identify wifi adapter status
typedef enum _WDK_WIFISTATUS
{
    WDK_WIFISTATUS_Unknown                  = -1,   ///< status is unknown
    WDK_WIFISTATUS_Okay                     = 0,    ///< status is okay
    
    WDK_WIFISTATUS_LoaderDisabled           = 1,    ///< the wifi driver loader is disabled
    WDK_WIFISTATUS_DriverUnloaded           = 2,    ///< the driver instance is not loaded
    WDK_WIFISTATUS_DriverLoading            = 3,    ///< the wifi driver is loading
    WDK_WIFISTATUS_DriverDisabled           = 4,    ///< the wifi driver is disabled or unloading
    
    WDK_WIFISTATUS_DriverNoResp             = 5,    ///< the wifi driver is not responding
    WDK_WIFISTATUS_DriverInternalFailure    = 6,    ///< the wifi driver reported an internal failure
    WDK_WIFISTATUS_DriverFirmwareFailure    = 7,    ///< the wifi driver reported a firmware failure
    WDK_WIFISTATUS_DriverLicenseFailure     = 8,    ///< the wifi driver reported a license failure
    
    WDK_WIFISTATUS_NdisuioNotLoaded         = 9,    ///< NDISUIO is not active
    
    WDK_WIFISTATUS_OutOfMemory              = 10,   ///< low memory condition
    
    WDK_WIFISTATUS_BadUserInput             = 11,   ///< the request is invalid
    WDK_WIFISTATUS_PowerOff                 = 12,   ///< The instance is powered off
} WDK_WIFISTATUS;

/// \brief stucture containing the elements to setup a channel scan
typedef struct _DMW_CHANNEL_SCAN_PARAMS
{
    WORD wMask;         ///< a bit mask of allowed channels to scan
    WORD wDwell;        ///< the amount of 100 usec time periods to spend scanning each channel
} DMW_CHANNEL_SCAN_PARAMS, *PDMW_CHANNEL_SCAN_PARAMS;  ///< Pointer to _DMW_CHANNEL_SCAN_PARAMS_

/// \brief structure containing the elements describing a scan result
typedef struct _DMW_AP_SCAN_INFO
{
    WORD    ChannelId;          ///< the id of the channel that this structure describes (1-14)
    signed short    AvgLevel;   ///< the average level signal detected on the channel
    signed short    PeakLevel;  ///< the peak level of signal detected on the channel
    WORD    Activity;           ///< TBD
} DMW_AP_SCAN_INFO, *PDMW_AP_SCAN_INFO;     ///< Pointer to _DMW_AP_SCAN_INFO_

/// \brief structure containing the elements describing an associated client
typedef struct _DMW_AP_ASSOCIATION_INFO_STRUCT 
{
    WORD    reserved;           ///< reserved for future use
    WORD    id;                 ///< an internal id for the associated client
    BYTE    mac_addr[MAC_LEN];  ///< the 6 byte hardware address of the associated client
    SHORT   rssi;               ///< RSSI (dBm)
} DMW_AP_ASSOCIATION_INFO, *PDMW_AP_ASSOCIATION_INFO;       ///< Pointer to _DMW_AP_ASSOCIATION_INFO_STRUCT


/// \brief structure containing elements describing a supported channel
typedef struct _DMW_CHAN_INFO
{
    BYTE        chan;       ///< Channel
    BYTE        band;       ///< Band (e.g. BAND_2_4GHZ)
    WORD        freq;       ///< Frequency
    BYTE        chanWdth;   ///< Channel width
    signed char txPwr;      ///< Trasnsmit power
    BYTE        txPwrMit;   ///< Reserved
    BYTE        flags;      ///< Reserved
}DMW_CHAN_INFO, *PDMW_CHAN_INFO;

#define BAND_2_4GHZ                     0x01    ///< 2.4 GHz Band
#define BAND_5GHZ                       0x02    ///< 5.0 GHz Band

// Startup status byte field enumerated values:
#define DMWSTATUS_START_MEMORY_ALLOC_FAILED         3       ///< Out of memory
#define DMWSTATUS_START_INIT_RADIO_CARD_FAILED      5       ///< Radio hardware failed to initialize
#define DMWSTATUS_START_START_RADIO_CARD_FAILED     7       ///< Radio hardware failed to start
#define DMWSTATUS_START_MGMT_START_FAILED           8       ///< Management routines failed to start
#define DMWSTATUS_START_MGMT_INIT_FAILED            10      ///< Manangement routines failed to initialize
#define DMWSTATUS_START_UPDATE_MAC_ADDRESS_FAILED   14      ///< Failure to set MAC address to radio
#define DMWSTATUS_START_RADIO_SWITCHED_OFF          19      ///< Status when instance is disabled

// Security key status byte field enumerated values:
#define DMWSTATUS_SEC_SECURITY_KEY_INVALID          2       ///< Serial key string not valid
#define DMWSTATUS_SEC_VERSION_FAILURE               3       ///< Serial key version not supported by this driver
#define DMWSTATUS_SEC_EXPIRED_FAILURE               5       ///< Unused (no eval keys)
#define DMWSTATUS_SEC_STA_ONLY_FAILURE              6       ///< Station use only
#define DMWSTATUS_SEC_LENGTH_FAILURE                7       ///< length of serial key is invalid (should not happen)
#define DMWSTATUS_SEC_KEY_IS_FOR_EVAL_VERSION       10      ///< Unused (no eval keys)
#define DMWSTATUS_SEC_INVALID_ADDRESS_RANGE         11      ///< Range in serial key is invalid (should not happen)
#define DMWSTATUS_SEC_MATCH_ADDR_FAILURE            64      ///< Match to MAC address failed

// Bits definition in failure byte:
#define DMWSTATUS_FAIL_RADIO_FAIL                   0x01    ///< This fail status bit will be set when:
                                                            ///< - A persistent failure (10 seconds) occurs in starting a scan or setting a channel.
                                                            ///< - Transmitter is stuck. Packets are queued but no completes received in 10 seconds.
#define DMWSTATUS_FAIL_DRIVER_FAIL                  0x02    ///< Not yet implemented.
#define DMWSTATUS_FAIL_SERIAL_NUM_FAIL              0x04    ///< The serial number is invalid or it doesn’t match the radio’s OUI of the MAC address.
#define DMWSTATUS_FAIL_OUT_OF_BUFFERS               0x08    ///< Out of buffers condition

/// \brief SDIO interface driver type
typedef enum _UFSDIO_TYPE
{
    UFSDIO_UNKNOWN = 0,     ///< Unknown or invalid type
    UFSDIO_GENERIC = 1,     ///< Generic ufsdio
    UFSDIO_MONOLITHIC = 2,  ///< Monolithic ufsdio
} UFSDIO_TYPE, *PUFSDIO_TYPE;

/// \brief Wi-Fi driver information types
typedef enum _DMW_INFO_STR
{
    DMW_INFO_VENDOR                      = 0,   ///< Vendor name
    DMW_INFO_VERSION                     = 1,   ///< Version
    DMW_INFO_DATE                        = 2,   ///< Build date
    DMW_INFO_SDIO_INTERFACE              = 3,   ///< SDIO driver version
    DMW_INFO_FIRMWARE                    = 4,   ///< Firmware version
    DMW_INFO_HARDWARE                    = 5,   ///< Hardware version
} DMW_INFO_STR;

#define AP_INFO_STR_SIZE 128

#define MAX_WPS_MANUFACTURER_LEN_SZ     65
#define MAX_WPS_MODEL_NAME_LEN_SZ       33
#define MAX_WPS_MODEL_NUMBER_LEN_SZ     33
#define MAX_WPS_SERIAL_NUMBER_LEN_SZ    33
#define MAX_WPS_DEVICE_NAME_LEN_SZ      33

/// \brief WPS Beacon parameters
typedef struct _DMW_AP_WPS_BEACON_PARAMS
{
    BYTE  uuid[16];             ///< Unique, permanent 16 byte code
    BYTE  bCategory;            ///< Device Category
    BYTE  bSubcategory;         ///< Device Sub-category
    WCHAR wzManufacturer[MAX_WPS_MANUFACTURER_LEN_SZ];  ///< Manufacturer
    WCHAR wzModelName[MAX_WPS_MODEL_NAME_LEN_SZ];       ///< Model name
    WCHAR wzModelNumber[MAX_WPS_MODEL_NUMBER_LEN_SZ];   ///< Model number
    WCHAR wzSerialNumber[MAX_WPS_SERIAL_NUMBER_LEN_SZ]; ///< Serial number
    WCHAR wzDeviceName[MAX_WPS_DEVICE_NAME_LEN_SZ];     ///< Device name
    
} DMW_AP_WPS_BEACON_PARAMS, *PDMW_AP_WPS_BEACON_PARAMS;

/// \brief Gets the loaded state of the wifi driver (ndcufmp).
///
/// Traverses the active drivers registry entries searching for the driver instance.
/// This API currently supports only the monolithic ufsdio version.
///
/// \param pfLoaded pointer to a BOOL value to receive the loaded state
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_SUPPORTED: The generic ufsdio sdio interface is in use.
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetIsDriverLoaded( PBOOL pfLoaded );

/// \brief Loads or unloads the wifi driver (ndcufmp).
///
/// This API currently supports only the monolithic ufsdio version.
///
/// \param fLoad a BOOL value indicating the the action to take
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_SUPPORTED: The generic ufsdio sdio interface is in use.
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_LoadDriver( BOOL fLoad );

/// \brief Initializes the WDK_DMW_* api
///
/// Initializes the internal storage for the Api to perform quick query tasks, you may
/// call this multiple times but each successful call must be matched by a call to
/// WDK_DMW_CleanupApi to properly clean up resources after the final matched call.
///
/// An optional timeout may be configured in the registry. If the timeout is enabled, then
/// the API will block until the wifi adapters are loaded or the timeout is reached. If
/// the timeout is enabled, and this function returns E_QLX_ADAPTER_NOT_READY, then
/// WDK_DMW_CleanupApi should not be called.
///
/// The registry settings for the timeout are 
/// "EnableInitApiTimeoutMs" to enable the timeout, and InitApiTimeoutMs to set the
/// timeout value in milliseconds.
///
/// \code
/// [HKEY_LOCAL_MACHINE\Software\Qualnetics\WdkDualModeWifiApi]
///     "EnableInitApiTimeoutMs" = dword:1
///     "InitApiTimeoutMs" = dword:1388
/// \endcode
///
/// \return HRESULT
///     - S_OK: Success
///     - E_QLX_ADAPTER_NOT_READY: The adapter is not ready or fully loaded.
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_InitApi();

/// \brief cleans up after the WDK_DMW_*, WDK_AP_*, and WDK_STA_* API calls.
///
/// Cleans up internal storage that was created in WDK_DMW_InitApi.  Each successful
/// call to WDK_DMW_LoadDriver must have a matched CleanupApi call to properly free 
/// any resources created.
///
/// This function should not be called if WDK_DMW_LoadDriver was unsuccessful.
///
WifiDKCore_API void WDK_DMW_CleanupApi();

/// \brief Wi-Fi driver modes
typedef enum _WDK_WIFI_MODE
{
    WDK_WIFI_MODE_NONE   = 0,                ///< None (disabled)
    WDK_WIFI_MODE_AP     = 1,                ///< AP
    WDK_WIFI_MODE_STA    = 2,                ///< STA
    WDK_WIFI_MODE_WFD    = 3,                ///< WFD
} WDK_WIFI_MODE, *PWDK_WIFI_MODE;

/// \brief Sets the active wifi mode.
///
/// If WFD mode is selected, the STA auto connect service (WZC on CE6, Native Wi-Fi Service on WEC7)
/// will be disabled for the STA adapter using the WIFI_STA_SetEnableAutoConnect function. 
/// The auto connect service is not re-enabled when STA mode is selected.
///
/// \param mode mode to set
///
/// \return HRESULT
///     - S_OK: Success, or the specified mode is currently active.
///     - E_SERVICE_NOT_ACTIVE: The WFD service is not running.
///     - E_SERVICE_DISABLED: The WFD service in not enabled.
///     - E_*: Other Error Conditions.
///
WifiDKCore_API HRESULT WDK_DMW_SetMode( WDK_WIFI_MODE mode );

/// \brief Gets the active wifi mode.
///
/// \param pMode pointer to a WDK_WIFI_MODE value
///
/// \return HRESULT
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetMode( __out PWDK_WIFI_MODE pMode );


/// \brief Gets the persistent state of the ndis instance for each radio.
///
/// Reads the registry setting that controls if the ndis instance for the specified radio type
/// is created at boot time. When disabled the adapter can not be used nor can settings
/// be adjusted.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pfEnabled pointer to a BOOL value indicating the requested Adapter state
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetAdapterEnabled(WDK_WIFIADAPTER eAdapter, __out PBOOL pfEnabled);

/// \brief Gets the ready state of the ndis instance for each radio.
///
/// Queries the media state for the adapter. The adapter is considered ready
/// if the media state is in a known state.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pfReady pointer to a BOOL value indicating the requested Adapter state
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetAdapterReady(WDK_WIFIADAPTER eAdapter, __out PBOOL pfReady);

/// \brief Gets the NDIS Adapter name for the specified adapter.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pwzName buffer to contain the name of the adapter instance
/// \param cchName length in characters of the buffer
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetAdapterName( WDK_WIFIADAPTER eAdapter, __out LPWSTR pwzName, DWORD cchName);

/// \brief Gets the adapter load state.
///
/// \param adapter adapter to check.
/// \param dwTimeoutMs Maximum time to wait for the adapter to load, in milliseconds.
///
/// \return HRESULT
///     - S_OK: Success, adapter is loaded.
///     - E_QLX_ADAPTER_NOT_READY: The adapter is not ready or fully loaded.
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_WaitForAdapterReady( WDK_WIFIADAPTER adapter, DWORD dwTimeoutMs );

/// \brief Gets the active Adapter instance.
///
/// \param pAdapter pointer to an enum value indicating which of the adapter instances to target
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetActiveAdapter( __out PWDK_WIFIADAPTER pAdapter );

/// \brief Gets the persistent value used for the mac address for the adapter
///
/// Reads the registry setting that controls the MAC software setting for the adapter.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pwzAddress LPWSTR buffer for the hardware address
/// \param dwAddressSize DWORD length of the buffer
/// \param pwzSerial LPWSTR optional buffer for the serial number or file name
/// \param dwSerialSize DWORD length of the serial buffer
/// \param pfSerialIsFileName PBOOL optionl buffer which return value indicates if the serial buffer contains a serial number or file name.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetHardwareAddress(WDK_WIFIADAPTER eAdapter, __out LPWSTR pwzAddress, DWORD dwAddressSize, __out_opt LPWSTR pwzSerial, DWORD dwSerialSize, __out_opt PBOOL pfSerialIsFileName);

/// \brief Sets the persistent value used for the mac address for the adapter
///
/// Writes the registry setting that controls the MAC software setting for the adapter.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pwzAddress pointer to a WCHAR array containing a zero terminated value to be used as the new MAC address. 
///                  If null or blank, the current address will be deleted and the default address in the radio will be used
/// \param pwzSerialNum pointer to a WCHAR array containing a zero terminated value to be used as the new serial number or file
/// \param fSerialIsFileName BOOL value indicating whether the serial num value contains a valid serial number or a file name
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_SetHardwareAddress(WDK_WIFIADAPTER eAdapter, __in_opt LPCWSTR pwzAddress, __in LPCWSTR pwzSerialNum, BOOL fSerialIsFileName);

/// \brief Gets the current in-use mac address for the adapter
///
/// Either pwzAddress or bAddr must be non-null.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pwzAddress LPWSTR buffer for the hardware address
/// \param dwAddressSize DWORD length of the buffer
/// \param bAddr BYTE array for the address in bytes
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetCurrentHardwareAddress(WDK_WIFIADAPTER eAdapter, __out_opt LPWSTR pwzAddress, DWORD dwAddressSize, __out_opt BYTE bAddr[MAC_LEN]);

/// \brief Gets the current country code for the adapter.
///
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pwzCountryCode LPWSTR buffer to hold the country code
/// \param dwCountryCode DWORD length of the buffer
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetCountryCode(WDK_WIFIADAPTER eAdapter, __out LPWSTR pwzCountryCode, DWORD dwCountryCode);

/// \brief Sets the country code for the adapter.
///
/// Supported two character country codes:\n
/*!
AL  Albania\n
DZ  Algeria\n
AR  Argentina\n
AM  Armenia\n
AW  Aruba\n
AU  Australia\n
AT  Austria\n
AZ  Azerbaijan\n
BH  Bahrain\n
BD  Bangladesh\n
BB  Barbados\n
BY  Belarus\n
BE  Belgium\n
BZ  Belize\n
BO  Bolivia, Plurinational State of\n
BA  Bosnia and Herzegovina\n
BR  Brazil\n
BN  Brunei Darussalam\n
BG  Bulgaria\n
KH  Cambodia\n
CA  Canada\n
CL  Chile\n
CN  China\n
CO  Colombia\n
CR  Costa Rica\n
HR  Croatia\n
CY  Cyprus\n
CZ  Czech Republic\n
DK  Denmark\n
DO  Dominican Republic\n
EC  Ecuador\n
EG  Egypt\n
SV  El Salvador\n
EE  Estonia\n
FI  Finland\n
FR  France\n
GE  Georgia\n
DE  Germany\n
GR  Greece\n
GL  Greenland\n
GD  Grenada\n
GU  Guam\n
GT  Guatemala\n
HT  Haiti\n
HN  Honduras\n
HK  Hong Kong\n
HU  Hungary\n
IS  Iceland\n
IN  India\n
ID  Indonesia\n
IR  Iran, Islamic Republic of\n
IE  Ireland\n
IL  Israel\n
IT  Italy\n
JM  Jamaica\n
JP  Japan\n
JO  Jordan\n
KZ  Kazakhstan\n
KP  Korea, Democratic People's Republic of\n
KR  Korea, Republic of\n
KW  Kuwait\n
LV  Latvia\n
LB  Lebanon\n
LI  Liechtenstein\n
LT  Lithuania\n
LU  Luxembourg\n
MO  Macao\n
MK  Macedonia, the former Yugoslav Republic of\n
MY  Malaysia\n
MT  Malta\n
MX  Mexico\n
MC  Monaco\n
MA  Morocco\n
NP  Nepal\n
NL  Netherlands\n
NZ  New Zealand\n
NO  Norway\n
OM  Oman\n
PK  Pakistan\n
PA  Panama\n
PG  Papua New Guinea\n
PE  Peru\n
PH  Philippines\n
PL  Poland\n
PT  Portugal\n
PR  Puerto Rico\n
QA  Qatar\n
RO  Romania\n
RU  Russian Federation\n
SA  Saudi Arabia\n
SC  Seychelles\n
SL  Sierra Leone\n
SG  Singapore\n
SK  Slovakia\n
SI  Slovenia\n
ZA  South Africa\n
ES  Spain\n
LK  Sri Lanka\n
SE  Sweden\n
CH  Switzerland\n
SY  Syrian Arab Republic\n
TW Taiwan, Province of China\n
TH  Thailand\n
TT  Trinidad and Tobago\n
TN  Tunisia\n
TR  Turkey\n
UA  Ukraine\n
AE  United Arab Emirates\n
GB  United Kingdom\n
US  United States\n
UY  Uruguay\n
UZ  Uzbekistan\n
VE  Venezuela, Bolivarian Republic of\n
VN  Viet Nam\n
YE  Yemen\n
ZW  Zimbabwe\n
 */
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param wzCountryCode LPWSTR buffer containing the country code
/// \param fPersist BOOL if TRUE, the new country code will persist across reboots
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_SetCountryCode(WDK_WIFIADAPTER eAdapter, __in LPCWSTR wzCountryCode, BOOL fPersist);

/// \brief Gets an info string from the Wi-Fi driver
///
/// \param wzString buffer to fill with string, size should be AP_INFO_STR_SIZE
/// \param cchString  size of incoming buffer
/// \param DmwAppInfoStr DMW_INFO_STR enum value for the desired string
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetInfoString(__out_opt LPWSTR wzString, __in_opt DWORD cchString, __in_opt DMW_INFO_STR DmwAppInfoStr );

/// \brief Gets the current default radio enabled state of a radio instance.
///
/// Gets the current default radio enabled state for the specified adapter. The default radio enabled state is
/// set by the WDK_DMW_SetPowerState API. If the default radio state is enabled, the adapter will be available
/// when the device boots.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pfEnabled pointer to a BOOL value to receive the radio state
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetDefaultRadioEnabled(WDK_WIFIADAPTER eAdapter, __out PBOOL pfEnabled);

/// \brief Gets the IP address of the adapter
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pszIPAddress LPWSTR to fill with found address
/// \param cchIPAddress  size of passed in buffer, should be at least 16 characters
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetIPAddress(WDK_WIFIADAPTER eAdapter, __out LPWSTR pszIPAddress, DWORD cchIPAddress);



/// \brief Queries the driver for the current supported channels list.
///
/// Use this function to determine the channels that the current locale supports.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param  ppChannelInfo a pointer to an array of DWM_CHAN_INFO structs.  This member will 
///         be created for you and you will need to delete[] it after you are done with the information in the
///         array.
/// \param  pdwChannelInfoCount pointer to a DWORD to receive the number of elements in the pChannelInfo
///         array that was created.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetSupportedChannelList(WDK_WIFIADAPTER eAdapter, __out PDMW_CHAN_INFO *ppChannelInfo, __out PDWORD pdwChannelInfoCount);

/// \brief Gets current status of WMM support
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pfEnabled -pointer to a BOOL that gets the state of WMM QoS
/// \param pfPSEnabled - pointer to BOOL that gets the state of WMM Power Save
///
/// \return HRESULT
///
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetEnableWMM( WDK_WIFIADAPTER eAdapter, __out PBOOL pfEnabled, __out PBOOL pfPSEnabled );


/// \brief Sets the current status of WMM support
///
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param fEnable - BOOL to enable/disable WMM QoS
/// \param fPSEnable - BOOL to enable/disable WMM Power Save
/// \param fPersist - if true, persists changes to the registry for on-boot defaults.
///
/// \return HRESULT
///
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_SetEnableWMM( WDK_WIFIADAPTER eAdapter, BOOL fEnable, BOOL fPSEnable, BOOL fPersist);



/// \brief debug levels
typedef enum _WDK_DMW_DEBUG_LEVEL
{
    WDK_DMW_DEBUG_Off       = 0,    ///< Off
    WDK_DMW_DEBUG_Error     = 1,    ///< Error messages
    WDK_DMW_DEBUG_Warn      = 2,    ///< Warning messages
    WDK_DMW_DEBUG_Event     = 3,    ///< Event messages
    WDK_DMW_DEBUG_Info      = 4,    ///< Informational messages
    WDK_DMW_DEBUG_Trace     = 5,    ///< Trace messages
    WDK_DMW_DEBUG_Loud      = 6,    ///< Verbose messages
    WDK_DMW_DEBUG_VeryLoud  = 7,    ///< Extremely verbase messages
} WDK_DMW_DEBUG_LEVEL, *PWDK_DMW_DEBUG_LEVEL;

/// \brief bit flags for setting specific debug level zones
typedef struct _WDK_DMW_DEBUG_DEBUGLEVEL_ZONES
{
    unsigned level      :4; ///< 0x 0000 00ff
    unsigned host       :1; ///< 0x 0000 0100
    unsigned radio      :1; ///< 0x 0000 0200
    unsigned core       :1; ///< 0x 0000 0400
    unsigned local      :1; ///< 0x 0000 0800
    unsigned leds       :1; ///< 0x 0000 1000
    unsigned buffers    :1; ///< 0x 0000 2000
    unsigned log        :1; ///< 0x 0000 4000
    unsigned bt         :1; ///< 0x 0000 8000
    unsigned web        :1; ///< 0x 0001 0000
    unsigned mgmt       :1; ///< 0x 0002 0000
} WDK_DMW_DEBUG_DEBUGLEVEL_ZONES, *PWDK_DMW_DEBUG_DEBUGLEVEL_ZONES;

/// \brief union of a DWORD and a debug level struture
typedef union _WDK_DMW_DEBUG_DEBUGLEVEL
{
    WDK_DMW_DEBUG_DEBUGLEVEL_ZONES zone;    ///< debug level zones
    DWORD dw;                               ///< DWORD value
} WDK_DMW_DEBUG_DEBUGLEVEL, *PWDK_DMW_DEBUG_DEBUGLEVEL;

/// \brief bit flags for setting specific print mask zones
typedef struct _WDK_DMW_DEBUG_PRINTMASK_ZONES
{
    unsigned scanning       :1; ///< 0x 0000 0001
    unsigned assoc          :1; ///< 0x 0000 0002
    unsigned ap_mgmt        :1; ///< 0x 0000 0004
    unsigned learning       :1; ///< 0x 0000 0008
    unsigned crypto_mcast   :1; ///< 0x 0000 0010
    unsigned crypto         :1; ///< 0x 0000 0020
    unsigned addr_tbl       :1; ///< 0x 0000 0040
    unsigned gen_rx         :1; ///< 0x 0000 0080
    unsigned reserved_1     :1; ///< Reserved
    unsigned scan_table     :1; ///< 0x 0000 0200
    unsigned scan_table_v   :1; ///< 0x 0000 0400
    unsigned ndc_mib        :1; ///< 0x 0000 0800
    unsigned reserved_2     :19; ///< Reserved
    unsigned error          :1; ///< 0x 8000 0000
} WDK_DMW_DEBUG_PRINTMASK_ZONES, *PWDK_DMW_DEBUG_PRINTMASK_ZONES;

/// \brief union of a DWORD and a Printmask structure
typedef union _WDK_DMW_DEBUG_PRINTMASK
{
    WDK_DMW_DEBUG_PRINTMASK_ZONES zone; ///< print mask zones
    DWORD dw;                           ///< DWORD value
} WDK_DMW_DEBUG_PRINTMASK, *PWDK_DMW_DEBUG_PRINTMASK;

/// \brief struct containing debug level zones and print mask zones
typedef struct _WDK_DMW_DEBUG_PARAMS
{
    WDK_DMW_DEBUG_DEBUGLEVEL DbgLevel;  ///< Debug Level settings
    WDK_DMW_DEBUG_PRINTMASK PrintMask;  ///< Debug PrintMask settings
} WDK_DMW_DEBUG_PARAMS, *PWDK_DMW_DEBUG_PARAMS;

/// \brief Gets the debug zones for the wifi driver.
///
/// \param pDbg struct to receive the debug zone settings
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetDebugZones( __out PWDK_DMW_DEBUG_PARAMS pDbg );

/// \brief Sets the debug zones for the wifi driver.
/// 
/// \param pDbg struct contianing the new debug zone settings
///
/// \return HRESULT
///     - S_QLX_REBOOT: Success, reboot required to apply changes.
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_SetDebugZones( __in PWDK_DMW_DEBUG_PARAMS pDbg );

/// \brief strut to contain information about BlueCore PS keys
typedef struct _AP_PSKEY_INFO_STRUCT
{
    USHORT  keynum;         ///< PS key index
    USHORT  keylen;         ///< PS key length
    UCHAR   pskey[128];     ///< PS key data
} AP_PSKEY_INFO_STRUCT, *PAP_PSKEY_INFO_STRUCT;

/// \brief Gets a range of BlueCore PS keys from the EEPROM using the specified adapter.
///
/// The adapter must be active to retrieve the key values. The same key values will
/// be retrieved from both adapters.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param usKeyFirst index of the first keynum to retrieve
/// \param pInfo buffer to hold the PS key information
/// \param pcInfo in: element count of the buffer, out: number of keys retreived
///
/// \return HRESULT
///     - S_OK: Success.
///     - E_NOT_FOUND: No keys were found, or the adapter is not active.
///     - E_*: Other Error Conditions.
///
WifiDKCore_API HRESULT WDK_DMW_GetEepromPsKey( WDK_WIFIADAPTER eAdapter, USHORT usKeyFirst, __out_ecount(cInfo) PAP_PSKEY_INFO_STRUCT pInfo, __inout USHORT *pcInfo );

/// \brief Gets the current power state of an adapter instance.
///
/// Gets the current power and radio enabled state for the specified adapter.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pfOn pointer to a BOOL value to receive the adapter power state (on or off)
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetPowerState( WDK_WIFIADAPTER eAdapter, __out BOOL * pfOn);

/// \brief Sets the current power state of the radio instance.
///
/// Sets the current power and radio enabled state for the specified adapter (on or off). 
/// Also sets the default, boot time enabled state for the adapter.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param fOn TRUE to enable the adapter power, FALSE to disable
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_SetPowerState( WDK_WIFIADAPTER eAdapter, BOOL fOn );

/// \brief Gets the current 802.11n enabled state
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pfEnabled - pointer to BOOL to receive the current 802.11n state.
///
/// \return HRESULT
///
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_Get11nEnabled( WDK_WIFIADAPTER eAdapter, __out PBOOL pfEnabled );

/// \brief Sets the current 802.11n enabled state
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param fEnabled - BOOL value indicating the desired 802.11n enabled state.
///
/// \return HRESULT
///
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_Set11nEnabled( WDK_WIFIADAPTER eAdapter, BOOL fEnabled );

/// \brief WPS modes used by the AP instance
typedef enum _DMW_WPS_MODE
{
    DMW_WPS_MODE_PIN = 0x01,    ///< WPS PIN mode
    DMW_WPS_MODE_PBC = 0x02,    ///< WPS push button control mode
} DMW_WPS_MODE;
#define DMW_WPS_MODE_ALL    (DMW_WPS_MODE_PIN | DMW_WPS_MODE_PBC)

/// \brief WPS Settings
typedef struct _DMW_WPS_SETTINGS
{
    BOOL    fEnable;    ///< WPS Enabled state
    DWORD   dwModes;    ///< DMW_WPS_MODE flags (AP Only)
} DMW_WPS_SETTINGS, *PDMW_WPS_SETTINGS;


/// \brief Gets the current WPS enabled state
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pSettings - pointer to a DMW_WPS_SETTINGS structure to receive the current WPS settings.
///
/// \return HRESULT
///
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_GetWPSSettings( WDK_WIFIADAPTER eAdapter, __out PDMW_WPS_SETTINGS pSettings );

/// \brief Sets the current WPS enabled state
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pSettings - pointer to a DMW_WPS_SETTINGS structure with the desired settings.
///
/// \return HRESULT
///
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_BAD_CONFIGURATION: In AP mode, this value is returned if WEP is currently enabled.
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_SetWPSSettings( WDK_WIFIADAPTER eAdapter, PDMW_WPS_SETTINGS pSettings );

/// \brief Opens a waitable handle for WPS connect events
///
/// Use CloseHandle to close the handle returned by this function.
/// When a successful WPS connection occurs, then the this event handle will be triggered.
/// The key parameters should then be obtained using the WDK_DMW_WPSGetApKey function.
///
/// \return 
///   - Non-NULL: Valid, waitable, handle.
///   - NULL: Failure, use GetLastError to retreive the error code.
///
WifiDKCore_API HANDLE WDK_DMW_OpenWPSHandle( VOID );

/// \brief Connect to the specified AP/STA using WPS
///
/// AP connection attempts must be disabled while a WPS connection attempt is in progress.
///  - In CE6.0 the WZC service must be disabled. 
///  - In WEC7, the Native Wi-Fi service must be disabled.
///
/// If fPBC is FALSE, the pszPin parameter is ignored.
///
/// The PIN must be 4 or 8 digits. If 8 digits, the PIN checksum should be pre-validated
/// using the WDK_WPS_ValidateWpsPin function.
///
/// The connect state is active for two (2) minutes.
///
/// In AP mode if this function is called while a WPS connect operation is in progress, the
/// two minute timer will be reset.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param bAddress STA Only: AP MAC address.
/// \param pszPin PIN to use for the WPS connection
/// \param fPBC BOOL indicating whether or not to use the pushbutton control WPS method.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_WPSConnect( WDK_WIFIADAPTER eAdapter, __in BYTE bAddress[MAC_LEN], __in_opt LPCSTR pszPin, BOOL fPBC );

/// \brief Cancels an in progress WPS connect action started by WDK_DMW_WPSConnect.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_WPSCancel( WDK_WIFIADAPTER eAdapter );

/// \brief Retrieve the key obtained through the most recent call to WDK_DMW_WPSConnect
///
/// This function is valid for the STA adapter only.
///
/// When the key parameters are successfully obtained, subsequent calls to this function will return E_NOT_FOUND.
///
/// \param eAdapter enum value indicating which of the adapter instances to target
/// \param pKeyParams Buffer to hold the key parameters return by the WPS connect operation
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_NOT_FOUND: There is no WPS key parameters available
///     - E_QLX_WPS_OVERLAP: WPS session overlap detected
///     - E_NOT_AUTHENTICATED: WPS failure
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_DMW_WPSGetApKey( WDK_WIFIADAPTER eAdapter, __out PWDK_WPS_KEY_PARAMS pKeyParams );

/// \brief Gets the maximum number of client associations for the AP
///
/// \param pulMaxAssoc - pointer to ULONG to receive the value
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_GetMaxAssoc( __out ULONG *pulMaxAssoc);

/// \brief Gets the settings used at boot time for the AP instance.
///
/// Get the current state of the AP instance that is used when booting.  This
/// state may not reflect the current operating state if the settings have be changed.
///
/// \param pSettings struct to receive the AP settings read from the registry
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_GetDefaultSettings(__out PDMW_AP_SETTINGS pSettings);

/// \brief Sets the settings used at boot time for the AP instance.
///
/// Sets the state of the AP instance that is used when booting.
///
/// \param pSettings struct containing the AP settings to write to the registry
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_SetDefaultSettings(__in PDMW_AP_SETTINGS pSettings);

/// \brief Gets the settings from the AP instance
///
/// Get the current state of the AP instance.
///
/// \param pSettings struct to receive the current AP settings
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_QLX_INVALID_SECURITY: Security setting invalid
///     - E_QLX_INVALID_CHANNEL: Invalid AP channel
///     - E_QLX_INVALID_CIPHER: Invalid cipher
///     - E_QLX_INVALID_WEP_IDX: Invalid WEP key index
///     - E_QLX_INVALID_WEP_KEY: Invalid WEP key
///     - E_QLX_INVALID_WPA_KEY: Invalid WPA key
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_GetCurrentSettings(__out PDMW_AP_SETTINGS pSettings);

/// \brief Sets the current state of the AP instance
///
/// Sets the current state of the AP instance.  You must also call SetDefaultSettings
/// if you would like the settings in pSettings, persisted through the next boot.
///
/// After the settings are applied, any associated stations will be required to reassociate.
///
/// \param pSettings struct containing the AP settings to write to the registry
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_QLX_INVALID_SECURITY: Security setting invalid
///     - E_QLX_INVALID_CHANNEL: Invalid AP channel
///     - E_QLX_INVALID_CIPHER: Invalid cipher
///     - E_QLX_INVALID_WEP_IDX: Invalid WEP key index
///     - E_QLX_INVALID_WEP_KEY: Invalid WEP key
///     - E_QLX_INVALID_WPA_KEY: Invalid WPA key
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_SetCurrentSettings(__in PDMW_AP_SETTINGS pSettings);

/// \brief Changes the AP instance to a new channel 1-14
///
/// This function changes the channel of the AP instance without altering any
/// of the other AP settings.
///
/// \param wChannelId WORD containing the new channel id to move to.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_ChangeChannel(WORD wChannelId);

/// \brief Performs a passive channel scan and returns the results.
///
/// Use this function to perform a passive channel scan on the selected
/// channels, and return the results into a newly allocated PDWM_AP_SCAN_INFO
/// array to you.
///
/// \param  wChannelMask WORD containing a bit mask of channels to scan, low bit is channel 1
/// \param  wDwell WORD containing the count of 100 micro second time periods to listen for
///         on each channel.  This is roughly equiv to 1 ms.  A resonable value would be 100-300.
/// \param  ppScanInfo a pointer to an array of DMW_AP_SCAN_INFO structs.  This member will be created
///         for you and you will need to delete[] it after you are done with the information in the
///         array.
/// \param  pdwScanInfoCount pointer to a DWORD to receive the number of elements in the pScanInfo
///         array that was created.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_PerformChannelScan(WORD wChannelMask, WORD wDwell, __out PDMW_AP_SCAN_INFO *ppScanInfo, __out PDWORD pdwScanInfoCount);

/// \brief Queries the driver for the current association list
///
/// Use this function to determine if any clients are currently associated or are associating
/// with the AP.
///
/// \param  ppAssociationInfo a pointer to an array of DMW_AP_ASSOCIATION_INFO structs.  This member will 
///         be created for you and you will need to delete[] it after you are done with the information in the
///         array.
/// \param  pdwAssociationInfoCount pointer to a DWORD to receive the number of elements in the pAssociationInfo
///         array that was created.
///
/// \return HRESULT
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_GetAssociationList(__out PDMW_AP_ASSOCIATION_INFO *ppAssociationInfo, __out PDWORD pdwAssociationInfoCount);

/// \brief Gets the current WMM Configuration parameters
///
/// \param pWMMParams - pointer to AP_WMM_PARAMS struct to receive info
///
/// \return HRESULT
///
///     - S_OK: Success
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_GetWMMParams(__out PAP_WMM_PARMS pWMMParams);

/// \brief Sets the current AP WMM configuration parameters
///
/// \param pWMMParams - pointer to AP_WMM_PARAMS struct to set info
/// \param fPersist - if true, persists changes to the registry for on-boot defaults.
///
/// \return HRESULT
///
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_SetWMMParams(__in PAP_WMM_PARMS pWMMParams, BOOL fPersist);

/// \brief Gets the AP WPS beacon parameters from the registry
///
/// The AP adapter instance does not have to be active for this function to succeed.
///
/// \param pParams - pointer to DMW_AP_WPS_BEACON_PARAMS struct to receive info
///
/// \return HRESULT
///
///     - S_OK: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_GetWPSBeaconParams(__out PDMW_AP_WPS_BEACON_PARAMS pParams);

/// \brief Sets the AP WPS beacon parameters in the registry
///
/// The AP adapter instance does not have to be active for this function to succeed.
/// 
/// The new beacon parameters will not take effect until the the next time the AP adapter is
/// loaded or the device is rebooted.
///
/// \param pParams - pointer to DMW_AP_WPS_BEACON_PARAMS struct to set info
///
/// \return HRESULT
///
///     - S_QLX_REBOOT: Success
///     - E_NOT_READY: The WDK_DMW_InitApi function was not called, or failed. 
///     - E_*: Other Error Conditions
///
WifiDKCore_API HRESULT WDK_AP_SetWPSBeaconParams(__in PDMW_AP_WPS_BEACON_PARAMS pParams);

#endif // _WDK_DUALMODEWIFI_API_H_

