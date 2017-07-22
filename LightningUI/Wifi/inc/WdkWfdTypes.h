/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WdkWfdTypes.h#1 $ */
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
/// \brief      WFD Related Constants and Structures
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKWFDTYPES_H_
#define _WDKWFDTYPES_H_

// system
#include <windef.h>
#include <windot11_wfd.h>

// sdk
#include <WdkVersion.h>
#include <WdkWps.h>

#define MAX_WFD_DEVICE_NAME_LEN         32
#define MAX_WFD_DEVICE_NAME_LEN_SZ      (MAX_WFD_DEVICE_NAME_LEN+1) 

#define MAX_WFD_GO_INTENT               15

/// \brief Wi-Fi Direct service state
typedef enum _WDK_WFD_STATE
{
    WDK_WFD_STATE_Disabled = 0,     ///< Disabled, no substates
    WDK_WFD_STATE_Idle,             ///< Idle/Listening, no substates
    WDK_WFD_STATE_Discovery,        ///< Discovering, no substates
    WDK_WFD_STATE_GroupFormation,   ///< Group formation, substate == WDK_WFD_GROUP_FORMATION_PHASE
    WDK_WFD_STATE_Connect,          ///< Connecting to the group, substate = WDK_WFD_CONNECT_PHASE

    NUM_WFD_STATES,                 ///< Count of states, NOT A STATE
} WDK_WFD_STATE, *PWDK_WFD_STATE;

/// \brief WFD group formation sub-states
typedef enum  _WDK_WFD_GROUP_FORMATION_PHASE
{
    WDK_WFD_GROUP_FORMATION_PHASE_Unknown = 0,  ///< Unknown
    WDK_WFD_GROUP_FORMATION_PHASE_Waiting,      ///< Waiting for a response
    WDK_WFD_GROUP_FORMATION_PHASE_RemoteRequest,///< A remote device has initiated group formation
    WDK_WFD_GROUP_FORMATION_PHASE_LocalRequest, ///< This device has initiated group formation
    WDK_WFD_GROUP_FORMATION_PHASE_StartClient,  ///< Client operation is starting
    WDK_WFD_GROUP_FORMATION_PHASE_StartGO,      ///< GO operation is starting 

    NUM_GROUP_FORMATION_PHASES,             ///< Count of phases, not a state
} WDK_WFD_GROUP_FORMATION_PHASE, *PWDK_WFD_GROUP_FORMATION_PHASE;

/// \brief WFD group formation sub-states
typedef enum  _WDK_WFD_CONNECT_PHASE
{
    WDK_WFD_CONNECT_PHASE_Unknown           = 0,    ///< Unknown
    WDK_WFD_CONNECT_PHASE_ClientWPS         = 100,  ///< CLient WPS connect
    WDK_WFD_CONNECT_PHASE_ClientProfile,            ///< Client profile setup
    WDK_WFD_CONNECT_PHASE_ClientConnecting,         ///< Client started
    WDK_WFD_CONNECT_PHASE_ClientConnected,          ///< Client connected
    WDK_WFD_CONNECT_PHASE_StartGO           = 200,  ///< GO starting
} WDK_WFD_CONNECT_PHASE, *PWDK_WFD_CONNECT_PHASE;

/// \brief Wi-Fi Direct primary device types
typedef enum _WFD_PRIMARY_DEVICE_TYPE
{
    WFD_PRIMARY_DEVICE_COMPUTER     = 1,
    WFD_PRIMARY_DEVICE_INPUT        = 2,
    WFD_PRIMARY_DEVICE_PRINTER      = 3,
    WFD_PRIMARY_DEVICE_CAMERA       = 4,
    WFD_PRIMARY_DEVICE_STORAGE      = 5,
    WFD_PRIMARY_DEVICE_NETWORK      = 6,
    WFD_PRIMARY_DEVICE_DISPLAY      = 7,
    WFD_PRIMARY_DEVICE_MULTIMEDIA   = 8,
    WFD_PRIMARY_DEVICE_GAMING       = 9,
    WFD_PRIMARY_DEVICE_TELEPHONE    = 10,
    WFD_PRIMARY_DEVICE_AUDIO        = 11,
    WFD_PRIMARY_DEVICE_OTHER        = 255,
} WFD_PRIMARY_DEVICE_TYPE, *PWFD_PRIMARY_DEVICE_TYPE;

/// \brief Wi-Fi Direct device roles
typedef enum _WFD_ROLE_TYPE {
    WFD_ROLE_TYPE_NONE          = 0x00,
    WFD_ROLE_TYPE_DEVICE        = 0x01,
    WFD_ROLE_TYPE_GROUP_OWNER   = 0x02,
    WFD_ROLE_TYPE_CLIENT        = 0x04,
    WFD_ROLE_TYPE_MAX           = 0x05
} WFD_ROLE_TYPE,  *PWFD_ROLE_TYPE;

/// \brief WFD device information structure
typedef struct _WDK_WFD_DEVICE_INFO
{
    DWORD               dwSize;             ///< Size of this structure
    BYTE                mac[MAC_LEN];       ///< MAC address
    WORD                wConfigMethods;     ///< Configuration methods
    BOOL                fSecondary;         ///< Indicates the secondary device type is in use
    WDK_WPS_DEVICE_TYPE primary;            ///< Primary device type
    WDK_WPS_DEVICE_TYPE secondary;          ///< Secondary device type
    WCHAR               wzName[DOT11_WPS_DEVICE_NAME_MAX_LENGTH+1];    ///< Device name, plus terminator
} WDK_WFD_DEVICE_INFO, *PWDK_WFD_DEVICE_INFO;

typedef struct _WFD_DEVICE_CAP_FIELDS
{
    unsigned service_discovery      : 1;
    unsigned client_discoverability : 1;
    unsigned concurrent_operation   : 1;
    unsigned infrastructure_managed : 1;
    unsigned device_limit           : 1;
    unsigned invitations            : 1;
    unsigned reserved               : 2;
} WFD_DEVICE_CAP_FIELDS, *PWFD_DEVICE_CAP_FIELDS;

typedef struct _WFD_GROUP_CAP_FIELDS
{
    unsigned group_owner            : 1;
    unsigned persistent             : 1;
    unsigned group_limit            : 1;
    unsigned intra_bss_distribution : 1;
    unsigned cross_connection       : 1;
    unsigned persistent_reconnect   : 1;
    unsigned group_formation        : 1;
    unsigned reserved               : 1;
} WFD_GROUP_CAP_FIELDS, *PWFD_GROUP_CAP_FIELDS;

///< WFD capabilities
typedef struct _WFD_CAPABILITIES
{
    union
    {
        BYTE byte;
        WFD_DEVICE_CAP_FIELDS field;
    } device;
    union
    {
        BYTE byte;
        WFD_GROUP_CAP_FIELDS field;
    } group;
} WFD_CAPABILITIES, *PWFD_CAPABILITIES;

///< Discovered device informaion
typedef struct _WDK_WFD_DISCOVERD_DEVICE
{
    WDK_WFD_DEVICE_INFO devInfo;    ///< Device information
    WFD_CAPABILITIES capabilities;  ///< WFD capabilities
    LONG    lRssi;                  ///< RSSI (signal strength)
} WDK_WFD_DISCOVERED_DEVICE, *PWDK_WFD_DISCOVERED_DEVICE;

///\brief WFD Device categories
typedef enum _WDK_WFD_DEVICE_CATEGORY
{
    WDK_WFD_DEVICE_CAT_COMPUTER     = 1,    ///< Computer
    WDK_WFD_DEVICE_CAT_INPUT        = 2,    ///< Input device
    WDK_WFD_DEVICE_CAT_PRINTER      = 3,    ///< Printer, scanner, copier, fax
    WDK_WFD_DEVICE_CAT_CAMERA       = 4,    ///< Camera
    WDK_WFD_DEVICE_CAT_STORAGE      = 5,    ///< Storage device
    WDK_WFD_DEVICE_CAT_NETWORK      = 6,    ///< Network infrastructure
    WDK_WFD_DEVICE_CAT_DISPLAY      = 7,    ///< Display
    WDK_WFD_DEVICE_CAT_MULTIMEDIA   = 8,    ///< Multimedia device
    WDK_WFD_DEVICE_CAT_GAMING       = 9,    ///< Gaming device
    WDK_WFD_DEVICE_CAT_PHONE        = 10,   ///< Phone/Tablet
    WDK_WFD_DEVICE_CAT_AUDIO        = 11,   ///< Audio device
    WDK_WFD_DEVICE_CAT_OTHER        = 255,  ///< Other
} WDK_WFD_DEVICE_CATEGORY, *PWDK_WFD_DEVICE_CATEGORY;

///\brief Subcategories for the WDK_WFD_DEVICE_CAT_PHONE category 
typedef enum _WDK_WFD_DEVICE_SUBCAT_PHONE
{
    WDK_WFD_DEVICE_SUBCAT_PHONE_WINMOB              = 1,    ///< Windows Mobile
    WDK_WFD_DEVICE_SUBCAT_PHONE_SINGLE              = 2,    ///< Single mode
    WDK_WFD_DEVICE_SUBCAT_PHONE_DUAL                = 3,    ///< Dual mode
    WDK_WFD_DEVICE_SUBCAT_PHONE_SMART               = 4,    ///< Single mode smartphone
    WDK_WFD_DEVICE_SUBCAT_PHONE_SMART_DUALMODE      = 5,    ///< Dual mode smartphone
    WDK_WFD_DEVICE_SUBCAT_PHONE_OTHER               = 255,  ///< Other
} WDK_WFD_DEVICE_SUBCAT_PHONE, *PWDK_WFD_DEVICE_SUBCAT_PHONE;

/// \brief Listen state discoverabability
typedef enum _WDK_WFD_DISCOVERABILITY
{
    WDK_WFD_DISCOVERABILITY_DISABLED    = DOT11_WFD_DEVICE_NOT_DISCOVERABLE,    ///< Discoverability disabled
    WDK_WFD_DISCOVERABILITY_AUTO        = DOT11_WFD_DEVICE_AUTO_AVAILABILITY,   ///< Automatic discoverability
    WDK_WFD_DISCOVERABILITY_HIGH        = DOT11_WFD_DEVICE_HIGH_AVAILABILITY,   ///< High discoverability
} WDK_WFD_DISCOVERABILITY, *PWDK_WFD_DISCOVERABILITY;

typedef struct WDK_WFD_DEVICE_CAPABILITIES  
{
    DWORD dwSize;                       ///< struture size
    BOOL fClientDiscoverability;        ///< Client discoverability
    BOOL fListenStateDiscoverability;   ///< Listen state discoverability
} WDK_WFD_DEVICE_CAPABILITIES, * PWDK_WFD_DEVICE_CAPABILITIES;

typedef struct _WDK_WFD_GROUP_OWNER_CAPABILITIES 
{
    DWORD dwSize;                   ///< structure size
    BOOL fPersistentGroup;          ///< persistent group supported
    BOOL fIntraBSSDistribution;     ///< Intra BSS distribution
    BOOL fCrossConnection;          ///< Cross Connection
    BOOL fPersistentReconnect;      ///< Persistent reconnect
} WDK_WFD_GROUP_OWNER_CAPABILITIES, *PWDK_WFD_GROUP_OWNER_CAPABILITIES;

typedef struct _WDK_WFD_GROUP_ID 
{
    DWORD dwSize;                   ///< structure size
    WCHAR wzSsid[MAX_SSID_SZ_LEN];  ///< SSID
} WDK_WFD_GROUP_ID, * PWDK_WFD_GROUP_ID;

/// \brief WDF configuration method
typedef enum _WDK_WFD_CONFIG_METHOD
{
    WDK_WFD_CONFIG_PIN          = 0, ///< PIN 
    WDK_WFD_CONFIG_PUSHBUTTON   = 1, ///< Pushbutton control
} WDK_WFD_CONFIG_METHOD;

/// \brief Callback indicating a remote pairing request has been made, return TRUE to accept, FALSE to deny.
typedef BOOL (*PFN_OnPairingRequest)( __in const PWDK_WFD_DEVICE_INFO pDevInfo );

/// \brief Callback indicating the state of the Wi-Fi Direct service has changed
typedef VOID (*PFN_OnWfdStateChange)( WDK_WFD_STATE state, DWORD dwSubstate, HRESULT hr );

/// \brief The driver has indicated a device discovery event has occured
typedef VOID (*PFD_pfnOnDeviceDiscovery)( DWORD cDevices );

/// \brief Callbacks used by the WiFi Direct service. 
typedef struct _WDK_WFD_CALLBACKS
{
    PFN_OnPairingRequest        pfnOnPairingRequest;    ///< A Remote pairing request has been made, return TRUE to accept, FALSE to deny.
    PFN_OnWfdStateChange        pfnOnStateChange;       ///< The state of the Wi-Fi Direct service has changed
    PFD_pfnOnDeviceDiscovery    pfnOnDeviceDiscovery;   ///< The driver has indicated a device discovery event has occured
} WDK_WFD_CALLBACKS, *PWDK_WFD_CALLBACKS;


#endif //_WDKWFDTYPES_H_

