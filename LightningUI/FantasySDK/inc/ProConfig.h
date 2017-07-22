#pragma once
#include "Util.h"
#include "Reg.hxx"


#define TIME_MODE_12H		1
#define TIME_MODE_24H		2

#define FONT_YAHEI			1
#define FONT_SONGTI			2
#define FONT_LISHU			3
#define FONT_KAITI				4
#define FONT_HEITI				5
#define FONT_HUPO			6
#define FONT_HANGKAI		7
#define FONT_XINWEI			8

#define FONT_YAHEI_W		11
#define FONT_SONGTI_W		12
#define FONT_LISHU_W		13
#define FONT_KAITI_W			14
#define FONT_HEITI_W				15
#define FONT_HUPO_W			16
#define FONT_HANGKAI_W		17
#define FONT_XINWEI_W			18

#define UI_LANG_ENGLISH			0
#define UI_LANG_CHN_TRA			2
#define UI_LANG_CHN_SIN			3
#define UI_LANG_JPN					4
#define UI_LANG_ARABIC				5
#define UI_LANG_DUTCH				6
#define UI_LANG_FRANCH			7
#define UI_LANG_GERMAN			8
#define UI_LANG_HEBREW			9
#define UI_LANG_ITALIAN			10
#define UI_LANG_KOREAN			11
#define UI_LANG_PORTUGUESE	12
#define UI_LANG_RUSSIAN			13
#define UI_LANG_SPANISH			14
#define UI_LANG_THAI					15
#define UI_LANG_TURKISH			16

#define ARGTYPE_INT						1
#define ARGTYPE_BOOL					2
#define ARGTYPE_DWORD				3
#define ARGTYPE_FLOAT					4
#define ARGTYPE_STRING					5

#define RK_CLOCK                TEXT("Software\\Microsoft\\Clock")
#define RV_INDST                TEXT("HomeDST")                        // are we currently in DST
#define RV_AUTODST              TEXT("AutoDST")
#define RV_DSTUI                TEXT("ShowDSTUI")
#define RV_TIMEZONES            TEXT("Time Zones")
#define RV_DISPLAY              TEXT("Display")
#define RV_TZI                  TEXT("TZI")
#define RV_DLT                  TEXT("Dlt")
#define RV_STD                  TEXT("Std")
#define FILETIME_TO_MINUTES ((__int64)600000000L)

typedef struct tagTZREG {
	LONG    Bias;
	LONG    StandardBias;
	LONG    DaylightBias;
	SYSTEMTIME StandardDate;
	SYSTEMTIME DaylightDate;
} TZREG;

typedef struct tagConfigParam
{
	//system:
    //0-English,2-Chinese tra,3-Chinese simple,4-Eorean,5-German,
    //6-Spanish,7-Portuguese,8-Italian,9-French,10-Japaness,11-Russian
	UINT				idLanguage;
	UINT				idFont;
	UINT				idTimeMode;
	UINT				idCurSRC;
	UINT				idCurUI;
	UINT				idPreSRC;
	UINT				idPreUI;
	UINT				idCarPlayLastUI;
	UINT				idCarPlayLastSRC;
	UINT				idCurSRCPara;
	CM_String		strLogoName;
	CM_String		strLogoName2;
	CM_String		strTexBackgroundName;
	CM_String		strNaviPath;
	CM_String		strOwnerName;
	CM_String		strSkinFileName;

	//display:
	GLfloat			fBackLuminance;
	GLfloat			fUITransparency;
	BOOL			bShowWallpaper;

	//media info:
	CM_String		strCurMediaDevice;
	CM_String		strCurMediaFile;
	UINT				iCurMediaPos;

	UINT				iMediaType;
	UINT				idMediaCategory;
	UINT				idMediaPlayMode;
	UINT				idMediaLibraryType;

	//audio media:
	CM_String		strAudioFileSD1;
	UINT				iAudioPosSD1;
	CM_String		strAudioFileSD2;
	UINT				iAudioPosSD2;
	CM_String		strAudioFileUSB1;
	UINT				iAudioPosUSB1;
	CM_String		strAudioFileUSB2;
	UINT				iAudioPosUSB2;
	CM_String		strAudioFileResident;
	UINT				iAudioPosResident;

	//video media:
	CM_String		strVideoFileSD1;
	UINT				iVideoPosSD1;
	CM_String		strVideoFileSD2;
	UINT				iVideoPosSD2;
	CM_String		strVideoFileUSB1;
	UINT				iVideoPosUSB1;
	CM_String		strVideoFileUSB2;
	UINT				iVideoPosUSB2;
	CM_String		strVideoFileResident;
	UINT				iVideoPosResident;

	//video param:
	INT				iVideoContrast;
	INT				iVideoBrightness;
	INT				iVideoHUE;
	INT				iVideoSaturation;

	//3G:
	BOOL			b3GAutoDial;

	UINT				iScreenSaverTimeOut;

	//bluetooth:
	BOOL			bBluetoothPowerOn;
	BOOL			bBluetoothAutoConnect;
	BOOL			bContactAutoDownload;
	BOOL			bCallAutoAnswer;
	BOOL			bIncomingRing;
	BOOL			bVcardFirstNameFirst;

	//Navi:
	UINT				idNaviSpeechConfig;

	//STW
	BYTE				dwKeyVolt[16];

	//widget list:
	BYTE				idUIList[32];

	//DISK
	BOOL			bDiskOSDFlag;
	DWORD		dwDiskVideoShapeConfig;
	DWORD		dwDiskTVSystemConfig;
	DWORD		dwDiskRegion;

	BOOL			bAAPAutoLaunch;

	//can bus status:
	BOOL			bHasCanBus;
	DWORD           dwECONLevel;

	//auto gps time:
	BOOL			bTimeAutoAdjust;

	//auto adjust daylight saving time
	BOOL			bDSTAutoAdjust;

	//backlight:
	DWORD		dwBacklight;

	//idle screen:
	DWORD		dwIdleScreen;

	//
	DWORD		dwVolumeAdjustRate;

	//volume:
	DWORD		dwVolMedia;
	DWORD		dwVolGPS;
	DWORD		dwVolBluetooth;

	//camera type
	DWORD		dwCameraType;
    UINT        iRearStandard;
	UINT        iDTVStandard;
	UINT        iDVDStandard;

	//mcu update flag:
	BOOL			bMCUSoftwareUpdateFlag;

	//auto start navigation:
	BOOL			bAutoStartNavi;

	//software update flag:
	DWORD		dwSoftwareUpdateFlag;

	//key beep:
	BOOL		bKeyBeep;
	//Reversing volume control 
	BOOL		bRearVolControl;

	//LCD Settings:
	UINT		iLEDRedDefault;
	UINT		iLEDGreenDefault;
	UINT		iLEDBlueDefault;
	UINT		iLEDBrightnessDefault;
    UINT		iLCDGammaDefault;

	UINT		iLEDRedCurrent;
	UINT		iLEDGreenCurrent;
	UINT		iLEDBlueCurrent;
    UINT		iLEDBrightnessCurrent;
	UINT		iLCDGammaCurrent;

	//
	BOOL	bBlurEffect;
    BOOL    bLedAnimEffect;
	//
	DWORD dwAndroidLinkType;

	//
	DWORD dwDriveType;

	//
	int	iTimeZone;

	//Radio:
	DWORD   dwCurFreq;
	DWORD	idRadioRegion;
	DWORD   dwStopLevelAF;
	BOOL	bRadioRDS;
	BOOL	bRadioAF;
	BOOL	bRadioTA;

    //DAB
    BOOL    bDABEnable;
	BOOL    bDABAntPowerOn;
	BOOL	bDABServiceLink;
	BOOL	bDABLBAND;
	BOOL	bDABTA;

    //Factory
    BOOL    bActiveFactory;
    BOOL    bSnapshotEnable;

	UINT    iSubWooferFreqCurrent;

	UINT    iPowerStatusBeforeACCOff;

	//Night mode
	BOOL		bNightModeActivate;
	UINT		iLCDNightModeBrightness;
	UINT		iLCDDayModeBrightness;
	UINT		iLEDNightModeBrightness;
	UINT		iLEDDayModeBrightness;

	//Audio EQ
	BOOL		bEQSwitch;
	UINT		iEQType;

	//DCP/CDP 
	BOOL		bDCPControl;
	//rearview delay:
	UINT			dwRearViewDelay;
	//power off delay:
	UINT			dwPowerOffDelay;

	UINT		iAudioEQLow;
	UINT		iAudioEQMid;
	UINT		iAudioEQTreb;

}ConfigParam,*LPConfigParam;

class CProConfig
{
public:
	CProConfig(void);
	~CProConfig(void);

	BOOL SaveConfig(void);
	void	 RestoreFactorySetting();
	ConfigParam *GetCurParam();

protected:
	BOOL GetArg(LPCTSTR lpStrBuff,LPCTSTR lpStrArg,UINT idType,LPVOID pArg);
	BOOL GetConfigsFromFile(LPCTSTR lpConfigFile);
	BOOL SaveConfigsToFile(LPCTSTR lpConfigFile);
	BOOL GetConfigFromReg();
	BOOL SaveConfigsToReg(void);
	//BOOL RestoreTimeZone(void);
	//BOOL UseDST(TIME_ZONE_INFORMATION &tziCurrent, SYSTEMTIME &stCurrent);
	
protected:
	ConfigParam m_curParam;
	ConfigParam m_oldParam;

};
