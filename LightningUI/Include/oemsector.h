/******************************************************************************
 CVTE Limited

 Copyright (c) 2014 CVTE company.

 Author Wu haoheng 2014

 All rights reserved.

******************************************************************************/

#ifndef _OEMSECTOR_H
#define _OEMSECTOR_H

#define OEMCONFIGFILENAME   "oem.cfg"
#define VERSIONIDLENGTH     32

#ifndef __ARMCC_VERSION             // ! ARM Developer Suite
#define __packed
#pragma pack(1)
#endif

typedef struct _STORE_INFOR
{
    DWORD cookie : 8;
    DWORD sectorvalid : 1;
    DWORD datavalid : 1;
}STOREINFOR;

typedef struct _STORE_SECTOR
{
    /* 0000 */
    STOREINFOR  Infor;   // Store sector information.
    /* 0004 */
    DWORD		dwSector;	// Starting sector of the image segment.
    /* 0008 */
    DWORD		dwLength;	// Image length of this segment, in contigious sectors.
    /* 00012 */
} STORE_SECTOR, *PSTORE_SECTOR;

__packed

////
//  Update strcut
////
//	typedef struct
//	{
//	    BYTE UpdateOEMSector:2;         //0:Disable     1:Enable
//	                                    //2:Update successfully 3:Update failed
//	    BYTE UpdateEboot:2;
//
//	    BYTE UpdateLogo:2;
//
//	    BYTE UpdateNK:2;
//
//	    BYTE UpdateUI:2;
//
//	    BYTE EraseFlsah:2;
//	}BitFieldUpdate_t;
//
//	typedef union
//	{
//	    UINT16 nUpdate;
//	    BitFieldUpdate_t sUpdate_t;
//	}Update_t;

////
// Function enable/disable option
////
typedef struct
{
    BYTE MouseCursor:1;
    BYTE TouchAdapter:1;
    BYTE DynamicLogo:1;
}BitFieldFunction_t;


typedef union
{
    BYTE nFunctionOption;
    BitFieldFunction_t sFunctionOption_t;
}FunctionOption_t;

// The following struct is used for the OS config.
typedef struct _OEMSECTOR
{
    BYTE    VersionID[VERSIONIDLENGTH];         //+32   (32)
    BYTE    UpdateOEMSector;                    //+1    (33)
    BYTE    UpdateEboot;                        //+1    (34)
    BYTE    UpdateNK;                           //+1    (35)
    BYTE    UpdateLogo;                         //+1    (36)
    BYTE    EraseFlsah;                         //+1    (37)
    BYTE    UpdateType;                         //+1    (38)
    BYTE    VersionType;                        //+1    (39)
    BYTE    TraceType;                          //+1    (40)
    BYTE    TouchType;                          //+1    (41)
    BYTE    GPSType;                            //+1    (42)
    BYTE    PowerStatus;                        //+1    (43)
    BYTE    RVChannel;                          //+1    (44)
    BYTE    LCDGamma;                           //+1    (45)
    BYTE    ColorSignal;                        //+1    (46)
    STORE_SECTOR   SGEbootInfo;                 //+12    (58)
    STORE_SECTOR   SGLogoInfo;                  //+12    (70)
    STORE_SECTOR   SGImageInfo;                 //+12    (82)
    BYTE    UpdateUI;                           //+1     (83)
	FunctionOption_t FunctionOption_t;          //+1    (84)
	BYTE    Backlight;                          //+1     (85)
    BYTE    rsved[169];                         //+169  (254)
}OEMSECTOR,*POEMSECTOR;

// The following enum define the config options of os.
typedef enum
{
    // Flag Disable/Enable
    OEMCONFIG_FLAG_DISABLE = 0, //0
    OEMCONFIG_FLAG_ENABLE,      //1
    // Update Flag
    OEMCONFIG_UPD_SUCCESS,      //2
    OEMCONFIG_UPD_FAIL,         //3
    // Update Type
    OEMCONFIG_UPD_TYPE_NORMAL = 0,
    OEMCONFIG_UPD_TYPE_STORE,
    // Version Type
    OEMCONFIG_VERSION_TYPE_RELEASE = 0,
    OEMCONFIG_VERSION_TYPE_DEBUG,
    // Trace Type
    OEMCONFIG_TRACE_TYPE_ALL_TRACE = 0,
    OEMCONFIG_TRACE_TYPE_FILTER_TRACE,
    OEMCONFIG_TRACE_TYPE_NO_TRACE,
    // Touch Type
    OEMCONFIG_TOUCH_TYPE_ONCHIP = 0,
    OEMCONFIG_TOUCH_TYPE_CAPACITANCE,
    OEMCONFIG_TOUCH_TYPE_IR,
    OEMCONFIG_TOUCH_TYPE_OTHER,
    // GPS Type
    OEMCONFIG_GPS_TYPE_CSR = 0,
    OEMCONFIG_GPS_TYPE_OTHER,
    // Power Type
    OEMCONFIG_POWER_STATUS_NORMAL = 0,
    OEMCONFIG_POWER_STATUS_IDLE,
    OEMCONFIG_POWER_STATUS_SLEEP,
    // RV Channel
    OEMCONFIG_RV_CHANNEL_CMMB = 0,
    OEMCONFIG_RV_CHANNEL_CVBS,
    OEMCONFIG_RV_CHANNEL_DVD,
    // RV Color Single
    OEMCONFIG_COLORSIGNAL_NTSM_MJ = 0,
    OEMCONFIG_COLORSIGNAL_NTSC_443,
    OEMCONFIG_COLORSIGNAL_PAL_M,
    OEMCONFIG_COLORSIGNAL_PAL_60,
    OEMCONFIG_COLORSIGNAL_PAL_BGHID,
    OEMCONFIG_COLORSIGNAL_SECAM,
    OEMCONFIG_COLORSIGNAL_PAL_COMBINATION_N,
    OEMCONFIG_COLORSIGNAL_SECAM_525,
    // LCD Gamma
    OEMCONFIG_LCD_GAMMA_DEFAULT = 130,
    // Backlight
    OEMCONFIG_LCD_BACKLIGHT_DEFAULT = 100,
}OEMCONFIG;


#ifndef __ARMCC_VERSION
#pragma pack()
#endif

#endif //_OEMSECTOR_H

