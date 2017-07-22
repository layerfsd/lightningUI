/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkHresult.h#1 $ */
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
/// \brief      HRESULT values and macros.  
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2009-12-10
//-----------------------------------------------------------------------------
/// \file

#ifndef _QLX_HRESULT_H_
#define _QLX_HRESULT_H_
#pragma once

#include <winerror.h>   // ERROR_*
#include <winbase.h>    // GetLastError

#include <WdkVersion.h>

#define FACILITY_USERBASE           40
#define FACILITY_QLX                (FACILITY_USERBASE)
#ifndef FACILITY_RAS
// Use the value from Win32 Desktop
#define FACILITY_RAS                83
#endif
#define FACILITY_WINSOCK            (FACILITY_USERBASE+2)
#define FACILITY_TAPI_LINE          (FACILITY_USERBASE+3)
#define FACILITY_CM                 (FACILITY_USERBASE+4)   // Connection Manager API

#define E_QLX_HRESULT(x)            ((HRESULT) (((x) & 0x0000ffff) | (FACILITY_QLX << 16) | 0x80000000))
#define S_QLX_HRESULT(x)            ((HRESULT) (((x) & 0x0000ffff) | (FACILITY_QLX << 16) | 0x00000000))

#define HRESULT_FROM_RASERR(x)      ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_RAS << 16) | 0x80000000))
#define HRESULT_FROM_WSAERR(x)      ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WINSOCK << 16) | 0x80000000))
#define HRESULT_FROM_LINEERR(x)     ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_TAPI_LINE << 16) | 0x80000000))
#define HRESULT_FROM_CMERR(x)       ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_CM << 16) | 0x80000000))

// QLX Specific errors
//
#define E_QLX_UTF8_INVALID_CHAR     E_QLX_HRESULT(0x002)    ///< utf parse encountered an invalid sequence
#define E_QLX_EXCEPTION             E_QLX_HRESULT(0x003)    ///< an exception occurred.
#define E_QLX_INVALID_CHANNEL       E_QLX_HRESULT(0x005)    ///< AP channel selection was invalid
#define E_QLX_INVALID_SECURITY      E_QLX_HRESULT(0x006)    ///< AP security options are not compatible
#define E_QLX_INVALID_CIPHER        E_QLX_HRESULT(0x007)    ///< AP security options are not compatible
#define E_QLX_INVALID_WEP_IDX       E_QLX_HRESULT(0x008)    ///< AP security options are not compatible
#define E_QLX_INVALID_WEP_KEY       E_QLX_HRESULT(0x009)    ///< AP security options are not compatible
#define E_QLX_INVALID_WPA_KEY       E_QLX_HRESULT(0x00A)    ///< AP security options are not compatible
#define E_QLX_REG_MISSING           E_QLX_HRESULT(0x00B)    ///< A required registry setting was missing.
#define E_QLX_INVALID_LICENCE       E_QLX_HRESULT(0x00C)    ///< Invalid or missing license key.
#define E_QLX_NOT_RANDOM            E_QLX_HRESULT(0x00D)    ///< The generated value is not random.
#define E_QLX_ADAPTER_NOT_READY     E_QLX_HRESULT(0x00e)    ///< The specified adapter is not ready/loaded.
#define E_QLX_POWER_MGMT_NOT_ACTIVE E_QLX_HRESULT(0x00f)    ///< The power management queue is not active.

#define E_QLX_WPS_OVERLAP           E_QLX_HRESULT(0x100)    ///< WPS session overlap detected

//web ext parser hr's
#define E_QLX_PARSE_TAG_SHORT       E_QLX_HRESULT(0x400)    ///< the tag "VAR.XYZ.ABC" did not contain enough tokens
#define E_QLX_UNKNOWN_VAR           E_QLX_HRESULT(0x401)    ///< the tag "VAR.XYZ.ABC" went unhandled in a module
#define E_QLX_UNKNOWN_ACTION        E_QLX_HRESULT(0x402)    ///< the first token in a tag is called the "action" 
#define E_QLX_PARSE_STRING_ID       E_QLX_HRESULT(0x403)    ///< when trying to parse a RES.STR.0123 the third param must be numeric
#define E_QLX_WHILE_NOT_CLOSED      E_QLX_HRESULT(0x404)    ///< when trying to parse a RES.STR.0123 the third param must be numeric
#define E_QLX_IF_NOT_CLOSED         E_QLX_HRESULT(0x405)    ///< when trying to parse a RES.STR.0123 the third param must be numeric
#define E_QLX_PARSE_CONDITION_SHORT E_QLX_HRESULT(0x406)    ///< if and while statements must have something in them

#define E_QLX_UNKNOWN_SECURITY      E_QLX_HRESULT(0x504)    ///< AP module error

#define E_QLX_P2P_NOT_IN_GROUP_FORMATION        E_QLX_HRESULT(0x510)    ///< Wi-Fi Direct: Not in group formation
#define E_QLX_P2P_UNEXPECTED_PEER_ADDRESS       E_QLX_HRESULT(0x511)    ///< Wi-Fi Direct: Group formation, unexpected peer address
#define E_QLX_P2P_GROUP_FORMATION_NOT_LOCAL     E_QLX_HRESULT(0x512)    ///< Wi-Fi Direct: Expecting locally initiated group formation commands
#define E_QLX_P2P_GROUP_FORMATION_NOT_REMOTE    E_QLX_HRESULT(0x513)    ///< Wi-Fi Direct: Expecting remotely initiated group formation commands
#define E_QLX_P2P_MISSING_PEER_IES              E_QLX_HRESULT(0x514)    ///< Wi-Fi Direct: Action frame is missing some WPS or P2P IEs.
#define E_QLX_P2P_UNEXPECTED_SENT_INDICATION    E_QLX_HRESULT(0x515)    ///< Wi-Fi Direct: The driver indicated a message was sent that was not requested to be sent.
#define E_QLX_P2P_SEND_PENDING                  E_QLX_HRESULT(0x516)    ///< Wi-Fi Direct: The driver has not completed a send operation.
#define E_QLX_P2P_WRONG_ADAPTER                 E_QLX_HRESULT(0x517)    ///< Wi-Fi Direct: The STA adapter is not active
#define E_QLX_P2P_INVALID_STATE_CHANGE          E_QLX_HRESULT(0x518)    ///< Wi-Fi Direct: The state change is not permitted
#define E_QLX_P2P_NOT_DISCOVERING               E_QLX_HRESULT(0x519)    ///< Wi-Fi Direct: Not in discovery mode
#define E_QLX_P2P_PEER_FAILED_TO_RESPOND        E_QLX_HRESULT(0x51a)    ///< Wi-Fi Direct: Peer failed to respond 
#define E_QLX_P2P_WPS_FAILED                    E_QLX_HRESULT(0x51b)    ///< Wi-Fi Direct: WPS failed
#define E_QLX_P2P_DEVICE_NOT_IN_SCAN_LIST       E_QLX_HRESULT(0x51c)    ///< Wi-Fi Direct: The device is not in the scan list
#define E_QLX_P2P_WPS_NOT_SUPPORTED             E_QLX_HRESULT(0x51d)    ///< Wi-Fi Direct: Unsupported WPS authentication/encryption mode
#define E_QLX_P2P_UNEXPECTED_ACTION_FRAME       E_QLX_HRESULT(0x51e)    ///< Wi-Fi Direct: The action frame was not expected
#define E_QLX_P2P_PROVISION_DISCOVERY_FAILED    E_QLX_HRESULT(0x51f)    ///< Wi-Fi Direct: The provision response did not match the request
#define E_QLX_P2P_INCOMPATIBLE_PARAMETERS       E_QLX_HRESULT(0x520)    ///< Wi-Fi Direct: Incompatable parameters
#define E_QLX_P2P_USER_REJECTED                 E_QLX_HRESULT(0x521)    ///< Wi-Fi Direct: User rejected
#define E_QLX_P2P_BOTH_INTENT_15                E_QLX_HRESULT(0x522)    ///< Wi-Fi Direct: Both intent 15
#define E_QLX_P2P_UNKNOWN_GROUP                 E_QLX_HRESULT(0x523)    ///< Wi-Fi Direct: Unknown group ID
#define E_QLX_NO_COMMON_CHANNELS                E_QLX_HRESULT(0x524)    ///< Wi-Fi Direct: No common channels
#define E_QLX_P2P_PROTOCOL_ERROR                E_QLX_HRESULT(0x525)    ///< Wi-Fi Direct: Protocol error
#define E_QLX_P2P_REQUEST_NOT_ACCEPTED          E_QLX_HRESULT(0x526)    ///< Wi-Fi Direct: Request not accepted
#define E_QLX_P2P_LIMIT_REACHED                 E_QLX_HRESULT(0x527)    ///< Wi-Fi Direct: Limit reached
#define E_QLX_P2P_INVALID_PARAMETER             E_QLX_HRESULT(0x528)    ///< Wi-Fi Direct: Invalid P2P parameter
#define E_QLX_P2P_INCOMPATIBLE_CONFIG_METHODS   E_QLX_HRESULT(0x529)    ///< Wi-Fi Direct: Incompatible config methods
#define E_QLX_P2P_CLIENT_CONNECT_FAILED         E_QLX_HRESULT(0x52a)    ///< Wi-Fi Direct: The client connection failed
#define E_QLX_P2P_CLIENT_DISCONNECTED           E_QLX_HRESULT(0x52b)    ///< Wi-Fi Direct: The client connection has ended
#define E_QLX_P2P_RESPONSE_NOT_ACCEPTED         E_QLX_HRESULT(0x52c)    ///< Wi-Fi Direct: Response not accepted
#define E_QLX_P2P_UNKNOWN_STATUS                E_QLX_HRESULT(0x52d)    ///< Wi-Fi Direct: Peer device sent an unknown status
#define E_QLX_P2P_ALREADY_CONNECTING            E_QLX_HRESULT(0x52e)    ///< Wi-Fi Direct: Already connecting to a peer

#define S_QLX_REBOOT                S_QLX_HRESULT(1)        ///< success, please reboot
#define S_QLX_RESTART_HTTPD         S_QLX_HRESULT(2)        ///< success, please refresh the httpd server
#define S_QLX_RESTART_NETWORK       S_QLX_HRESULT(3)        ///< success, please restart all the network adapters.

// WIN32 Errors as HRESULTs
//
// 0x0000 to 0x00FF 
#define E_INVALID_FUNCTION          HRESULT_FROM_WIN32(ERROR_INVALID_FUNCTION)          ///< 0x 8007 0001
#define E_FILE_NOT_FOUND            HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)            ///< 0x 8007 0002
#define E_PATH_NOT_FOUND            HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND)            ///< 0x 8007 0003
#define E_ACCESS_DENIED             HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED)             ///< 0x 8007 0005
#define E_INVALID_HANDLE            HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE)            ///< 0x 8007 0006
#define E_INVALID_ACCESS            HRESULT_FROM_WIN32(ERROR_INVALID_ACCESS)            ///< 0x 8007 000C
#define E_INVALID_DATA              HRESULT_FROM_WIN32(ERROR_INVALID_DATA)              ///< 0x 8007 000D
#define E_NOT_SAME_DEVICE           HRESULT_FROM_WIN32(ERROR_NOT_SAME_DEVICE)           ///< 0x 8007 0011
#define E_NOT_READY                 HRESULT_FROM_WIN32(ERROR_NOT_READY)                 ///< 0x 8007 0015
#define E_BAD_COMMAND               HRESULT_FROM_WIN32(ERROR_BAD_COMMAND)               ///< 0x 8007 0016
#define E_HANDLE_EOF                HRESULT_FROM_WIN32(ERROR_HANDLE_EOF)                ///< 0x 8007 0026
#define E_WRITE_FAULT               HRESULT_FROM_WIN32(ERROR_WRITE_FAULT)               ///< 0x 8007 001D
#define E_READ_FAULT                HRESULT_FROM_WIN32(ERROR_READ_FAULT)                ///< 0x 8007 001E
#define E_GEN_FAILURE               HRESULT_FROM_WIN32(ERROR_GEN_FAILURE)               ///< 0x 8007 001F
#define E_NOT_SUPPORTED             HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED)             ///< 0x 8007 0032
#define E_DUP_NAME                  HRESULT_FROM_WIN32(ERROR_DUP_NAME)                  ///< 0x 8007 0034
#define E_BAD_NETPATH               HRESULT_FROM_WIN32(ERROR_BAD_NETPATH)               ///< 0x 8007 0035
#define E_NETWORK_BUSY              HRESULT_FROM_WIN32(ERROR_NETWORK_BUSY)              ///< 0x 8007 0036
#define E_DEV_NOT_EXIST             HRESULT_FROM_WIN32(ERROR_DEV_NOT_EXIST)             ///< 0x 8007 0037
#define E_BAD_NET_RESP              HRESULT_FROM_WIN32(ERROR_BAD_NET_RESP)              ///< 0x 8007 003A
#define E_UNEXP_NET_ERR             HRESULT_FROM_WIN32(ERROR_UNEXP_NET_ERR)             ///< 0x 8007 003B 
#define E_BAD_DEV_TYPE              HRESULT_FROM_WIN32(ERROR_BAD_DEV_TYPE)              ///< 0x 8007 0044
#define E_BAD_NET_NAME              HRESULT_FROM_WIN32(ERROR_BAD_NET_NAME)              ///< 0x 8007 0043
#define E_ALREADY_ASSIGNED          HRESULT_FROM_WIN32(ERROR_ALREADY_ASSIGNED)          ///< 0x 8007 0055
#define E_INVALID_PARAMETER         HRESULT_FROM_WIN32(ERROR_INVALID_PARAMETER)         ///< 0x 8007 0057 :: E_INVALIDARG 
#define E_INVALID_PASSWORD          HRESULT_FROM_WIN32(ERROR_INVALID_PASSWORD)          ///< 0x 8007 0065
#define E_OPEN_FAILED               HRESULT_FROM_WIN32(ERROR_OPEN_FAILED)               ///< 0x 8007 006E
#define E_DISK_FULL                 HRESULT_FROM_WIN32(ERROR_DISK_FULL)                 ///< 0x 8007 0070
#define E_INSUFFICIENT_BUFFER       HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)       ///< 0x 8007 007A
#define E_INVALID_NAME              HRESULT_FROM_WIN32(ERROR_INVALID_NAME)              ///< 0x 8007 007B
#define E_MOD_NOT_FOUND             HRESULT_FROM_WIN32(ERROR_MOD_NOT_FOUND)             ///< 0x 8007 007E
#define E_PROC_NOT_FOUND            HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND)            ///< 0x 8007 007F
#define E_BAD_ARGUMENTS             HRESULT_FROM_WIN32(ERROR_BAD_ARGUMENTS)             ///< 0x 8007 00A0
#define E_BAD_PATHNAME              HRESULT_FROM_WIN32(ERROR_BAD_PATHNAME)              ///< 0x 8007 00a1
#define E_BUSY                      HRESULT_FROM_WIN32(ERROR_BUSY)                      ///< 0x 8007 00AA
#define E_ALREADY_EXISTS            HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS)            ///< 0x 8007 00B7
#define E_BAD_EXE_FORMAT            HRESULT_FROM_WIN32(ERROR_BAD_EXE_FORMAT)            ///< 0x 8007 00c1
#define E_LOCKED                    HRESULT_FROM_WIN32(ERROR_LOCKED)                    ///< 0x 8007 00D4
#define E_MORE_DATA                 HRESULT_FROM_WIN32(ERROR_MORE_DATA)                 ///< 0x 8007 00EA
#define E_NO_DATA                   HRESULT_FROM_WIN32(ERROR_NO_DATA)                   ///< 0x 8007 00E8

// 0x0100 to 0x01FF 
#define E_WAIT_TIMEOUT              HRESULT_FROM_WIN32(WAIT_TIMEOUT)                    ///< 0x 8007 0102
#define E_NO_MORE_ITEMS             HRESULT_FROM_WIN32(ERROR_NO_MORE_ITEMS)             ///< 0x 8007 0103
#define E_INVALID_ADDRESS           HRESULT_FROM_WIN32(ERROR_INVALID_ADDRESS)
// 0x0200 to 0x02FF 
// 0x0300 to 0x03FF 
#define E_INVALID_MESSAGE           HRESULT_FROM_WIN32(ERROR_INVALID_MESSAGE)           ///< 0x 8007 03EA
#define E_BADKEY                    HRESULT_FROM_WIN32(ERROR_BADKEY)                    ///< 0x 8007 03F2
// 0x0400 to 0x04FF 
#define E_SERVICE_ALREADY_RUNNING   HRESULT_FROM_WIN32(ERROR_SERVICE_ALREADY_RUNNING)   ///< 0x 8007 0420
#define E_INVALID_SERVICE_ACCOUNT   HRESULT_FROM_WIN32(ERROR_INVALID_SERVICE_ACCOUNT)   ///< 0x 8007 0421 (invalid driver licence)
#define E_SERVICE_DISABLED          HRESULT_FROM_WIN32(ERROR_SERVICE_DISABLED)          ///< 0x 8007 0422
#define E_SERVICE_NOT_ACTIVE        HRESULT_FROM_WIN32(ERROR_SERVICE_NOT_ACTIVE)        ///< 0x 8007 0426
#define E_EXCEPTION_IN_SERVICE      HRESULT_FROM_WIN32(ERROR_EXCEPTION_IN_SERVICE)      ///< 0x 8007 0428
#define E_SERVICE_EXISTS            HRESULT_FROM_WIN32(ERROR_SERVICE_EXISTS)            ///< 0x 8007 0431
#define E_DLL_NOT_FOUND             HRESULT_FROM_WIN32(ERROR_DLL_NOT_FOUND)             ///< 0x 8007 0485
#define E_DEVICE_NOT_CONNECTED      HRESULT_FROM_WIN32(ERROR_DEVICE_NOT_CONNECTED)      ///< 0x 8007 048f
#define E_NOT_FOUND                 HRESULT_FROM_WIN32(ERROR_NOT_FOUND)                 ///< 0x 8007 0490
#define E_BAD_DEVICE                HRESULT_FROM_WIN32(ERROR_BAD_DEVICE)                ///< 0x 8007 04B0
#define E_DEVICE_ALREADY_REMEMBERED HRESULT_FROM_WIN32(ERROR_DEVICE_ALREADY_REMEMBERED) ///< 0x 8007 04B2
#define E_NO_NET_OR_BAD_PATH        HRESULT_FROM_WIN32(ERROR_NO_NET_OR_BAD_PATH)        ///< 0x 8007 04B3
#define E_BAD_PROFILE               HRESULT_FROM_WIN32(ERROR_BAD_PROFILE)               ///< 0x 8007 04B6
#define E_INVALID_DOMAINNAME        HRESULT_FROM_WIN32(ERROR_INVALID_DOMAINNAME)        ///< 0X 8007 04BC
#define E_INVALID_PASSWORDNAME      HRESULT_FROM_WIN32(ERROR_INVALID_PASSWORDNAME)      ///< 0x 8007 04c0 
#define E_REMOTE_SESSION_LIMIT_EXCEEDED HRESULT_FROM_WIN32(ERROR_REMOTE_SESSION_LIMIT_EXCEEDED) ///< 0x 8007 04C4
#define E_NO_NETWORK                HRESULT_FROM_WIN32(ERROR_NO_NETWORK)                ///< 0x 8007 04C6
#define E_CANCELLED                 HRESULT_FROM_WIN32(ERROR_CANCELLED)                 ///< 0x 8007 04C7
#define E_GRACEFUL_DISCONNECT       HRESULT_FROM_WIN32(ERROR_GRACEFUL_DISCONNECT)       ///< 0x 8007 04CA
#define E_CONNECTION_INVALID        HRESULT_FROM_WIN32(ERROR_CONNECTION_INVALID)        ///< 0x 8007 04CD
#define E_CONNECTION_ACTIVE         HRESULT_FROM_WIN32(ERROR_CONNECTION_ACTIVE)         ///< 0x 8007 04CE
#define E_CONNECTION_ABORTED        HRESULT_FROM_WIN32(ERROR_CONNECTION_ABORTED)        ///< 0x 8007 04D4
#define E_RETRY                     HRESULT_FROM_WIN32(ERROR_RETRY)                     ///< 0x 8007 04D5
#define E_SERVICE_NOT_FOUND         HRESULT_FROM_WIN32(ERROR_SERVICE_NOT_FOUND)         ///< 0x 8007 04DB
#define E_NOT_AUTHENTICATED         HRESULT_FROM_WIN32(ERROR_NOT_AUTHENTICATED)         ///< 0x 8007 04DC
#define E_ALREADY_INITIALIZED       HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED)       ///< 0x 8007 04DF
#define E_ALREADY_REGISTERED        HRESULT_FROM_WIN32(ERROR_ALREADY_REGISTERED)        ///< 0x 8007 04DA
#define E_NO_MORE_DEVICES           HRESULT_FROM_WIN32(ERROR_NO_MORE_DEVICES)           ///< 0x 8007 04E0
// 0x0500 to 0x05FF 
#define E_ALLOTTED_SPACE_EXCEEDED   HRESULT_FROM_WIN32(ERROR_ALLOTTED_SPACE_EXCEEDED)   ///< 0x 8007 0540
#define E_INTERNAL_ERROR            HRESULT_FROM_WIN32(ERROR_INTERNAL_ERROR)            ///< 0x 8007 054F
#define E_INVALID_INDEX             HRESULT_FROM_WIN32(ERROR_INVALID_INDEX)             ///< 0x 8007 0585
#define E_NO_SYSTEM_RESOURCES       HRESULT_FROM_WIN32(ERROR_NO_SYSTEM_RESOURCES)       ///< 0x 8007 05AA
#define E_TIMEOUT                   HRESULT_FROM_WIN32(ERROR_TIMEOUT)                   ///< 0x 8007 05B4
// 0x0600 to 0x06FF 
#define E_UNKNOWN_PROPERTY          HRESULT_FROM_WIN32(ERROR_UNKNOWN_PROPERTY)          ///< 0x 8007 0648
#define E_INVALID_HANDLE_STATE      HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE_STATE)      ///< 0x 8007 0649
#define E_BAD_CONFIGURATION         HRESULT_FROM_WIN32(ERROR_BAD_CONFIGURATION)         ///< 0x 8007 064a
#define E_DEVICE_REMOVED            HRESULT_FROM_WIN32(ERROR_DEVICE_REMOVED)            ///< 0x 8007 0651
#define E_DATATYPE_MISMATCH         HRESULT_FROM_WIN32(ERROR_DATATYPE_MISMATCH)         ///< 0x 8007 065d
#define E_UNSUPPORTED_TYPE          HRESULT_FROM_WIN32(ERROR_UNSUPPORTED_TYPE)          ///< 0x 8007 065e
#define E_CREATE_FAILED             HRESULT_FROM_WIN32(ERROR_CREATE_FAILED)             ///< 0x 8007 065f
#define E_VERSION                   HRESULT_FROM_WIN32(ERROR_PRODUCT_VERSION)           ///< 0x 8007 0666
#define E_INVALID_USER_BUFFER       HRESULT_FROM_WIN32(ERROR_INVALID_USER_BUFFER)       ///< 0x 8007 06F8
// 0x0700 to 0x07FF 
#define E_INVALID_DATATYPE          HRESULT_FROM_WIN32(ERROR_INVALID_DATATYPE)          ///< 0x 8007 070c
// 0x0800 to 0x08FF 
#define E_NOT_CONNECTED             HRESULT_FROM_WIN32(ERROR_NOT_CONNECTED)             ///< 0x 8007 08ca
// 0x0900 to 0x09FF 
// 0x1000 to 0x1FFF 
#define E_RESOURCE_DISABLED         HRESULT_FROM_WIN32(ERROR_RESOURCE_DISABLED)             ///< 0x 8007 10D5
#define E_DEVICE_NOT_AVAILABLE      HRESULT_FROM_WIN32(ERROR_DEVICE_NOT_AVAILABLE)          ///< 0x 8007 10DF
#define E_MESSAGE_EXCEEDS_MAX_SIZE  HRESULT_FROM_WIN32(ERROR_MESSAGE_EXCEEDS_MAX_SIZE)      ///< 0x 8007 10F0
#define E_INVALID_STATE             HRESULT_FROM_WIN32(ERROR_INVALID_STATE)                 ///< 0x 8007 139F
#define E_MODEM_RESPONSE_ERROR      HRESULT_FROM_WIN32(ERROR_CTX_MODEM_RESPONSE_ERROR)      ///< 0x 8007 1B63
#define E_MODEM_RESPONSE_TIMEOUT    HRESULT_FROM_WIN32(ERROR_CTX_MODEM_RESPONSE_TIMEOUT)    ///< 0x 8007 1B64
#define E_MODEM_RESPONSE_NO_CARRIER HRESULT_FROM_WIN32(ERROR_CTX_MODEM_RESPONSE_NO_CARRIER) ///< 0x 8007 1B65
#define E_MODEM_RESPONSE_BUSY       HRESULT_FROM_WIN32(ERROR_CTX_MODEM_RESPONSE_BUSY)       ///< 0x 8007 1B67

#define E_LASTWIN32ERROR            ((GetLastError()==ERROR_SUCCESS) ? S_OK : HRESULT_FROM_WIN32(GetLastError()))
#define E_LASTWINSOCKERR            HRESULT_FROM_WSAERR(WSAGetLastError())

/// \brief Returns the WIN32 error code for an HRESULT, if possible. Returns ERROR_SUCCESS or ERROR_GEN_FAILURE otherwise
__inline DWORD WIN32ERR_FROM_HRESULT(HRESULT hr)
{
    DWORD dwErr = ERROR_GEN_FAILURE;
    WORD wFacility = HIWORD(hr) & 0x7ff;

    if(SUCCEEDED(hr))
    {
        dwErr = ERROR_SUCCESS;
    }
    else if(wFacility == FACILITY_WIN32)
    {
        dwErr = LOWORD(hr);
    }
    else
    {
        switch(hr)
        {
            case E_NOTIMPL:
                dwErr = ERROR_NOT_SUPPORTED;
                break;
            case E_OUTOFMEMORY:
                dwErr = ERROR_OUTOFMEMORY;
                break;
            case E_INVALIDARG:
                dwErr = ERROR_INVALID_PARAMETER;
                break;
            case E_NOINTERFACE:
                dwErr = ERROR_NOT_SUPPORTED;
                break;
            case E_POINTER:
                dwErr = ERROR_NOACCESS;
                break;
            case E_HANDLE:
                dwErr = ERROR_INVALID_HANDLE;
                break;
            case E_ABORT:
                dwErr = ERROR_CANCELLED;
                break;
            case E_ACCESSDENIED:
                dwErr = ERROR_ACCESS_DENIED;
                break;
            case E_FAIL:
                dwErr = ERROR_GEN_FAILURE;
                break;
            default:
                // keep as an hresult, don't loose information
                dwErr = hr;
                break;
                
        }
    }

    return dwErr;
}


#endif // _QLX_HRESULT_H_
