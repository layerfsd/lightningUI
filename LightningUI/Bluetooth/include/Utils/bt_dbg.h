
#ifndef _BT_DBG_H_
#define _BT_DBG_H_

#include <windows.h>


#ifdef __cplusplus
extern "C"
{
#endif

/* "BLUESTAR" defines the specific part relative with SfWE, instead of legacy WPP. */
#define BLUESTAR

/* +++ Macro to enable/disable Bluetooth profiles. */
#define ENABLE_HFP
#define ENABLE_PBAP
//#define ENABLE_TPMS
//#define ENABLE_MAP
#define ENABLE_A2DP
#define ENABLE_AVRCP
//#define ENABLE_SPP
//#define ENABLE_OPPS
//#define ENABLE_DUN
//#define ENABLE_HID
//#define ENABLE_DID
/* --- Macro to enable/disable Bluetooth profiles. */

#ifdef ENABLE_HFP
/* Macro to enable HFP audio transfer into local device when call is active. */
#define ENABLE_HFP_AUDIO_TRANSFER

/* Macro to enable HFP query operator. */
#define ENABLE_HFP_QUERY_OPERATOR

/* Macro to enable HFP voice recognition. */
#define ENABLE_HFP_VOICE_RECOGNITION

/* Macro to enable HFP specific with Apple iPhone which support some specific AT command. */
#define ENABLE_HFP_FOR_IPHONEx

#ifdef ENABLE_HFP_FOR_IPHONE
/* Macro to enable iPhone Siri EFM(Eys Free Mode). */
#define ENABLE_IPHONE_SIRI_EFM
#endif

/* Marco to enable new APIs for Siri EFM */
//#define ENABLE_API_IPHONE_SIRI_EFM

#ifdef ENABLE_API_IPHONE_SIRI_EFM
#define ENABLE_APL_DEBUG (1)
#define DIAL_CASE_NUMBER_XAPL   TEXT("*#1001")
#define DIAL_CASE_NUMBER_EFM1   TEXT("*#1002")
#define DIAL_CASE_NUMBER_EFM0   TEXT("*#1003")
#define DIAL_CASE_NUMBER_SIRI   TEXT("*#1004")
#define DIAL_CASE_NUMBER_BVRA1  TEXT("*#1005")
#define DIAL_CASE_NUMBER_BVRA0  TEXT("*#1006")
#endif

/* Macro to enable the checking whether HFP call is idle. If defined, we apply
   the limitation that HFP call is always of higher priority, especially when
   PBAP is downloading or A2DP is streaming. */
#define ENABLE_HFP_CHECK_CALL_IDLE
#endif

#ifdef ENABLE_PBAP
/* Macro to store phonebook into WinCE database. */
#define STORE_PHONEBOOK_IN_DB

/* Macro to store call record into WinCE database. */
#define STORE_CALL_RECORD_IN_DB

/* Macro to enable dumping phonebook. */
#define DUMP_PHONEBOOK
#endif

#ifdef ENABLE_HID
/* Enable HID to control HDMI output, in which SPP1 is used implicitely. */
#define ENABLE_HID_FOR_HDMI

/* Enable AVRCP(CT) to control medial player for HDMI. If not defined, HID consumer control is used instead. */
//#define ENABLE_AVRCP_FOR_HDMI

/* Enable A2DP(SNK) to receive audio transferred from remote phone. If not defined(default), HDMI audio is used instead. */
//#define ENABLE_A2DP_FOR_HDMI

/* [TestOnly] Remove later!!! Test HID for Miracast, in which LG Nexus4 is used by default. 
   Note:
   (1) Currently, BT App is verified fully so as to support HID for HDMI generically, not limited with the phone.
   (2) If the macro is defined, BT App is enabled to test only HID for Miracast with Nexus4.
   (3) The final version of BT App will be unified to support both HDMI and Miracast concurrently.
*/
//#define TEST_ONLY_HID_FOR_MIRACAST
#endif

#ifdef ENABLE_SPP
/* Macro to configure SPP. 
  (1) 3 SPP(SPP0/1/2) can be supported since SfWE1.1.13, while 2 SPP can only be supported in previous SfWE version.
  (2) The prerequisite to use 3 SPP is to make sure that SfWE SPP driver is configured properly so that 
      WinCE6.0 system can expose 3 "COM".
  (3) The default configuration of SPP lies in
      - SPP0: When SPP is enabled, SPP0 is activated for generic usage which can also be fitted with Apple iAP/iAP2.
      - SPP1: When HID for HDMI is enabled, SPP1 is activated implcitely.
      - SPP2: When SPP for TPMS is enabled, SPP2 is activated.
*/

/* Enable SPP for TPMS, in which SPP2 is used by default. */
//#define ENABLE_SPP_FOR_TPMS

#define ENABLE_GED_DID_INFO_ON_SPP_CONNECTED
#endif

/* +++ Macro to enable/disable auto connect Bluetooth profiles. */
#define ENABLE_AUTO_CONNECT

#ifdef ENABLE_AUTO_CONNECT
#define AUTO_CONNECT_HF
#define AUTO_CONNECT_A2DP
#define AUTO_CONNECT_AVRCP
//#define AUTO_CONNECT_MAP
//#define AUTO_CONNECT_SPP
//#define AUTO_CONNECT_HID
#endif
/* --- Macro to enable/disable auto connect Bluetooth profiles. */

/* +++ Macro relative with BT power. */
/* Flag indicating whether to set BT power on/off when BT is turned on/off. 
   - If defined:     Awlays power BT on/off. (Default)
   - If NOT defined: Mainly set Bluetooth scan mode enabled/disabled.
*/
#define ENABLE_SET_BT_POWER
/* --- Macro relative with BT power. */

/* Macro relative with BSP RAM-based registry. By default, the macro isn't defined because BSP use HIVE-based registry instead. */
//#define BSP_USE_RAM_BASED_REGISTRY

#undef  ZONE_INIT
#undef  ZONE_DEINIT
#undef  ZONE_POWERUP
#undef  ZONE_POWERDOWN
#undef  ZONE_MMI
#undef  ZONE_LOGIC
#undef  ZONE_API
#undef  ZONE_DATAPOOL
#undef  ZONE_THREAD
#undef  ZONE_WARN
#undef  ZONE_ERROR

    void DebugInitialize();
    void DebugUninitialize();
    void DebugOut(TCHAR *lpszFmt, ...);
	void TpmsLog(TCHAR *lpszFmt, ...);

#define DEBUG
#ifdef DEBUG

#define  ZONE_INIT			DEBUGZONE(0)
#define  ZONE_DEINIT	    DEBUGZONE(1)
#define  ZONE_POWERUP		DEBUGZONE(2)
#define  ZONE_POWERDOWN		DEBUGZONE(3)
#define  ZONE_MMI			DEBUGZONE(4)
#define  ZONE_LOGIC			DEBUGZONE(5)
#define  ZONE_API			DEBUGZONE(6)
#define  ZONE_DATAPOOL		DEBUGZONE(7)
#define  ZONE_THREAD		DEBUGZONE(8)
#define  ZONE_WARN	        DEBUGZONE(9)
#define  ZONE_ERROR	        DEBUGZONE(10)
#define  ZONE_DEBUGONLY		1
#define  ZONE_DUMP          0


#ifdef DECLARE_DBGPARAM

#define DBG_BIT_INIT		(1<<0)
#define DBG_BIT_DEINIT		(1<<1)
#define DBG_BIT_POWERUP		(1<<2)
#define DBG_BIT_POWERDOWN	(1<<3)
#define DBG_BIT_MMI			(1<<4)
#define DBG_BIT_LOGIC		(1<<5)
#define DBG_BIT_API			(1<<6)
#define DBG_BIT_DATAPOOL    (1<<7)
#define DBG_BIT_THREAD      (1<<8)
#define DBG_BIT_WARN        (1<<9)
#define DBG_BIT_ERROR       (1<<10)
#define DBG_BIT_BIT11		(1<<11)
#define DBG_BIT_BIT12		(1<<12)
#define DBG_BIT_BIT13		(1<<13)
#define DBG_BIT_BIT14		(1<<14)
#define DBG_BIT_BIT15		(1<<15)

    DBGPARAM dpCurSettings =
    {
        TEXT("BT_APP"),
        {
            TEXT("Init"), TEXT("DeInit"), TEXT("PowerUp"), TEXT("PowerDown"),
            TEXT("MMI"), TEXT("Logic"), TEXT("API"), TEXT("DataPool"),
            TEXT("Thread"), TEXT("Warning"), TEXT("Error"), TEXT("BIT12"),
            TEXT("BIT13"), TEXT("BIT14"), TEXT("BIT15"), TEXT("BIT16")
        },
        DBG_BIT_INIT | DBG_BIT_DEINIT | DBG_BIT_POWERUP  | DBG_BIT_POWERDOWN  | DBG_BIT_MMI  |
        DBG_BIT_LOGIC  | DBG_BIT_API  | DBG_BIT_DATAPOOL  | DBG_BIT_THREAD  | DBG_BIT_WARN  | DBG_BIT_ERROR
    };

#else
    extern DBGPARAM dpCurSettings;
#endif//DECLARE_DBGPARAM

#define BTDBG(cond,printf_exp) ((void)((cond)?(DebugOut printf_exp),1:0))
#define TPMSLOG(cond,printf_exp) ((void)((cond)?(TpmsLog printf_exp),1:0))

#else	//!DEBUG

#define  ZONE_INIT			0
#define  ZONE_DEINIT	    0
#define  ZONE_POWERUP		0
#define  ZONE_POWERDOWN		0
#define  ZONE_MMI			0
#define  ZONE_LOGIC			0
#define  ZONE_API			0
#define  ZONE_DATAPOOL		0
#define  ZONE_THREAD		1
#define  ZONE_WARN	        1
#define  ZONE_ERROR	        1
#define  ZONE_DEBUGONLY	    0
#define  ZONE_DUMP          0

#define BTDBG(cond,printf_exp) ((void)((cond)?(DebugOut printf_exp),1:0))
#define TPMSLOG(cond,printf_exp) ((void)((cond)?(TpmsLog printf_exp),1:0))

#endif // DEBUG

#define PTS_ENABLE				0
#if(PTS_ENABLE)
	#define PTS_AVRCP			1
#else
	#define PTS_AVRCP			0
#endif

#ifdef __cplusplus
}
#endif

#endif _BT_DBG_H_
