/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/windot11_ce6.h#1 $ */
//-----------------------------------------------------------------------------
/// \brief      WINDOT11 structs from WEC7
//-----------------------------------------------------------------------------
/// \file

#ifndef __WINDOT11_CE6_H__
#define __WINDOT11_CE6_H__

#pragma once

#include <bldver.h>

#if (CE_MAJOR_VER < 7 )

#define NDIS_OBJECT_TYPE_DEFAULT                            0x80    // used when object type is implicit in the API call

#define NWF_MANDATORY_OID       (0x01U)
#define NWF_OPTIONAL_OID        (0x02U)

#define NWF_OPERATIONAL_OID     (0x01U)
#define NWF_STATISTICS_OID      (0x02U) 

#define NWF_DEFINE_OID(Seq,o,m)     ((0x0E000000U) | ((o) << 16) | ((m) << 8) | (Seq))

typedef UCHAR DOT11_MAC_ADDRESS[6];
typedef DOT11_MAC_ADDRESS  * PDOT11_MAC_ADDRESS;

typedef UCHAR DOT11_COUNTRY_OR_REGION_STRING[3];
typedef DOT11_COUNTRY_OR_REGION_STRING * PDOT11_COUNTRY_OR_REGION_STRING;

typedef struct _NDIS_OBJECT_HEADER
{
    UCHAR   Type;
    UCHAR   Revision;
    USHORT  Size;
} NDIS_OBJECT_HEADER, *PNDIS_OBJECT_HEADER;

#define DOT11_SSID_MAX_LENGTH 32
typedef struct _DOT11_SSID {
  ULONG uSSIDLength;
  UCHAR ucSSID[DOT11_SSID_MAX_LENGTH];
} DOT11_SSID, *PDOT11_SSID;

typedef union DOT11_BSS_ENTRY_PHY_SPECIFIC_INFO {
  ULONG  uChCenterFrequency;
  struct {
    ULONG uHopPattern;
    ULONG uHopSet;
    ULONG uDwellTime;
  } FHSS;
} DOT11_BSS_ENTRY_PHY_SPECIFIC_INFO, *PDOT11_BSS_ENTRY_PHY_SPECIFIC_INFO;

typedef enum _DOT11_BSS_TYPE {
  dot11_BSS_type_infrastructure   = 1,
  dot11_BSS_type_independent      = 2,
  dot11_BSS_type_any              = 3 
} DOT11_BSS_TYPE, *PDOT11_BSS_TYPE;

#endif 

#endif __WINDOT11_CE6_H__

