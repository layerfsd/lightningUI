/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Wifi/SDK/INC/WdkWmm.h#1 $ */
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
/// \brief      WMM related structs
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKWMM_H_
#define _WDKWMM_H_

#include <WdkVersion.h>

// ------------------------------
// OID_AP_WMM_PARMS

#pragma pack(push, 2)


/// \brief structure containing the WMM qos settings for a specific level
typedef struct _tAcParamRec
{
    unsigned  aci_aifsn : 4;    ///< Arbirtary Inter-frame Space number (1-15) (WMM AP Defaults: BK:7 BE:3 VI:1 VO:1)
    unsigned acm : 1;           ///< Admission Control Mandatory flag, not supported
    unsigned aci : 2;           ///< aci reference, shouldn't be changed
    unsigned reserved : 1;      ///< Reserved
    unsigned ecw_min : 4;       ///< ECW Min value (0-15) as defined in the MMM spec. 
    unsigned ecw_max : 4;       ///< ECW Max Value (0-15) as defined in the MMM spec.
    unsigned  txop_limit : 16;  ///< Transmit Ops.  Maximum time a device can transmit after obtaining an opportunity (in increments of 32microseconds).  As an example WMM AP Default is 94 for video in 802.11a/g
} tAcParamRec;



/// \brief structure containing the WMM qos settings used by the AP
typedef struct _AP_WMM_PARMS
{
    tAcParamRec ac_be;            ///< WMM settings for Best Effort
    tAcParamRec ac_bk;            ///< WMM settings for Background
    tAcParamRec ac_vi;            ///< WMM settings for Video
    tAcParamRec ac_vo;            ///< WMM settings for Voice
} AP_WMM_PARMS, *PAP_WMM_PARMS;

                                                     
#pragma pack(pop)

#endif _WDKWMM_H_
