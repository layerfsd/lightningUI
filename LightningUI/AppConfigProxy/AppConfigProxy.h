#ifndef _APPCONFIG_PROXY_H_
#define _APPCONFIG_PROXY_H_
#include "../FantasySDK/inc/ProConfig.h"
#include "../Include/oemsector.h"

//
//	#define OEMCONFIGFILENAME   "oem.cfg"
//	#define VERSIONIDLENGTH     32
//
//	#ifndef __ARMCC_VERSION             // ! ARM Developer Suite
//	#define __packed
//	#pragma pack(1)
//	#endif
//
//	__packed
//
//	// The following struct is used for the OS config.
//	typedef struct _OEMSECTOR
//	{
//	    BYTE    VersionID[VERSIONIDLENGTH];         //+32   (32)
//	    BYTE    UpdateOEMSector;                    //+1    (33)
//	    BYTE    UpdateEboot;                        //+1    (34)
//	    BYTE    UpdateNK;                           //+1    (35)
//	    BYTE    UpdateLogo;                         //+1    (36)
//	    BYTE    EraseFlsah;                         //+1    (37)
//	    BYTE    UpdateType;                         //+1    (38)
//	    BYTE    VersionType;                        //+1    (39)
//	    BYTE    TraceType;                          //+1    (40)
//	    BYTE    TouchType;                          //+1    (41)
//	    BYTE    GPSType;                            //+1    (42)
//	    BYTE    PowerStatus;                        //+1    (43)
//	    BYTE    RVChannel;                          //+1    (44)
//	    BYTE    LCDGamma;                           //+1    (45)
//	    BYTE    rsved[209];                         //+209  (254)
//	}OEMSECTOR,*POEMSECTOR;
//
//	// The following enum define the config options of os.
//	typedef enum
//	{
//	    // Flag Disable/Enable
//	    OEMCONFIG_FLAG_DISABLE = 0,
//	    OEMCONFIG_FLAG_ENABLE,
//	    // Update Flag
//	    OEMCONFIG_UPD_SUCCESS,
//	    OEMCONFIG_UPD_FAIL,
//	    // Update Type
//	    OEMCONFIG_UPD_TYPE_NORMAL = 0,
//	    OEMCONFIG_UPD_TYPE_SOTRE,
//	    // Version Type
//	    OEMCONFIG_VERSION_TYPE_RELEASE = 0,
//	    OEMCONFIG_VERSION_TYPE_DEBUG,
//	    // Trace Type
//	    OEMCONFIG_TRACE_TYPE_ALL_TRACE = 0,
//	    OEMCONFIG_TRACE_TYPE_FILTER_TRACE,
//	    OEMCONFIG_TRACE_TYPE_NO_TRACE,
//	    // Touch Type
//	    OEMCONFIG_TOUCH_TYPE_ONCHIP = 0,
//	    OEMCONFIG_TOUCH_TYPE_CAPACITANCE,
//	    OEMCONFIG_TOUCH_TYPE_IR,
//	    OEMCONFIG_TOUCH_TYPE_OTHER,
//	    // GPS Type
//	    OEMCONFIG_GPS_TYPE_CSR = 0,
//	    OEMCONFIG_GPS_TYPE_OTHER,
//	    // Power Type
//	    OEMCONFIG_POWER_STATUS_NORMAL = 0,
//	    OEMCONFIG_POWER_STATUS_SLEEP,
//	    // RV Channel
//	    OEMCONFIG_RV_CHANNEL_CVBS_NTSC = 0,
//	    OEMCONFIG_RV_CHANNEL_CVBS_PAL,
//	    OEMCONFIG_RV_CHANNEL_ARGB,
//	    // LCD Gamma
//	    OEMCONFIG_LCD_GAMMA_DEFAULT = 130,
//	}OEMCONFIG;
//
//
//	#ifndef __ARMCC_VERSION
//	#pragma pack()
//	#endif

class CAppConfigProxy : public CProConfig
{
public:
	CAppConfigProxy(void);
	~CAppConfigProxy(void);

	BOOL SetOemSector(void);
    BOOL GetOemSector(void);
	OEMSECTOR *GetOemSectorParam();

    BOOL SaveAppConfig();

    //CProConfig *m_pConfig;
private:
    HANDLE m_hOPD;
    BOOL   m_bOPDReady;
	OEMSECTOR m_OemSectorParam;
};

#endif
