#ifndef	__CARPLAYAPI_h__
#define	__CARPLAYAPI_h__

#ifdef	__cplusplus
	extern "C" {
#endif

/*
CarplayAPI version numbers use (roughly) a Semantic Versioning (http://semver.org/) scheme,
in which version numbers consist of three parts: MAJOR.MINOR.PATCH
*/

#define CarplayAPIVersionString			CarplayAPIProjectString##" "##CarplayAPIVersionNumber
#define CarplayAPIProjectString			"CarPlayAPI"
#define CarplayAPIVersionNumber			"0.2.15"

#ifndef CARPLAYAPI_INTERNAL
typedef int OSStatus;

typedef unsigned int AirPlaySpeechMode;
#define kAirPlaySpeechMode_NotApplicable			0
#define kAirPlaySpeechMode_None						-1 // No speech-related states are active.
#define kAirPlaySpeechMode_Speaking					1  // Device is speaking to the user.
#define kAirPlaySpeechMode_Recognizing				2  // Device is recording audio to recognize speech from the user.

typedef unsigned int AirPlayTriState;
#define kAirPlayTriState_NotApplicable		0
#define kAirPlayTriState_False				-1
#define kAirPlayTriState_True				1

typedef unsigned int AirPlayResourceID;
#define kAirPlayResourceID_NotApplicable			0
#define kAirPlayResourceID_MainScreen				1 // Main screen.
#define kAirPlayResourceID_MainAudio				2 // Input and output audio stream for Siri, telephony, media.

typedef unsigned int AirPlaySiriAction;
#define kAirPlaySiriAction_NotApplicable				0
#define kAirPlaySiriAction_Prewarm						1 // Pre-warm Siri.
#define kAirPlaySiriAction_ButtonDown					2 // Button down.
#define kAirPlaySiriAction_ButtonUp						3 // Button up.

typedef unsigned int AirPlayTransferType;
#define kAirPlayTransferType_NotApplicable				0
#define kAirPlayTransferType_Take						1 // Transfer ownership permanently.
#define kAirPlayTransferType_Untake						2 // Release permanent ownership. 
#define kAirPlayTransferType_Borrow						3 // Transfer ownership temporarily.
#define kAirPlayTransferType_Unborrow					4 // Release temporary ownership.

typedef unsigned int AirPlayTransferPriority;
#define kAirPlayTransferPriority_NotApplicable				0
#define kAirPlayTransferPriority_NiceToHave					100 // Transfer succeeds only if constraint is <= Anytime.
#define kAirPlayTransferPriority_UserInitiated				500 // Transfer succeeds only if constraint is <= UserInitiated.

typedef unsigned int AirPlayConstraint;
#define kAirPlayConstraint_NotApplicable			0
#define kAirPlayConstraint_Anytime					100  // Resource may be taken/borrowed at any time.
#define kAirPlayConstraint_UserInitiated			500  // Resource may be taken/borrowed if user initiated.
#define kAirPlayConstraint_Never					1000 // Resource may never be taken/borrowed.

typedef unsigned int AirPlayEntity;
#define kAirPlayEntity_NotApplicable			0
#define kAirPlayEntity_Controller				1
#define kAirPlayEntity_Accessory				2

typedef struct
{
	AirPlayEntity			entity;
	AirPlaySpeechMode		mode;
	
}	AirPlaySpeechState;

typedef struct
{
	AirPlayEntity			screen;		// Owner of the screen.
	AirPlayEntity			mainAudio;	// Owner of main audio.
	AirPlayEntity			phoneCall;	// Owner of phone call.
	AirPlaySpeechState		speech;		// Owner of speech and its mode.
	AirPlayEntity			turnByTurn;	// Owner of navigation.
	
}	AirPlayModeState;

#define AirPlayModeStateInit( PTR )		memset( (PTR), 0, sizeof( *(PTR) ) )

typedef struct
{
	AirPlayTransferType			type;
	AirPlayTransferPriority		priority;
	AirPlayConstraint			takeConstraint;				// If "take", constraint for peer to take. Missing otherwise.
	AirPlayConstraint			borrowOrUnborrowConstraint;	// If "take", constraint for peer to borrow. Missing otherwise.
															// If "borrow", constraint for peer to unborrow. Missing otherwise.
}	AirPlayResourceChange;

typedef struct
{
	AirPlayResourceChange		screen;
	AirPlayResourceChange		mainAudio;
	AirPlaySpeechMode			speech;
	AirPlayTriState				phoneCall;
	AirPlayTriState				turnByTurn;
	
}	AirPlayModeChanges;

#define AirPlayModeChangesInit( PTR )	memset( (PTR), 0, sizeof( *(PTR) ) )

#include <winsock2.h>
#include <ws2tcpip.h>

typedef union
{
	struct sockaddr				sa;
	struct sockaddr_in			v4;
	#if( defined( AF_INET6 ) )
		struct sockaddr_in6		v6;
	#endif
	
}	sockaddr_ip;

typedef unsigned long long	AirPlayAudioFormat;
#define kAirPlayAudioFormat_Invalid						0
#define kAirPlayAudioFormat_Reserved1					( 1 << 0 )	// 0x00000001
#define kAirPlayAudioFormat_Reserved2					( 1 << 1 )	// 0x00000002
#define kAirPlayAudioFormat_PCM_8KHz_16Bit_Mono			( 1 << 2 )	// 0x00000004
#define kAirPlayAudioFormat_PCM_8KHz_16Bit_Stereo		( 1 << 3 )	// 0x00000008
#define kAirPlayAudioFormat_PCM_16KHz_16Bit_Mono		( 1 << 4 )	// 0x00000010
#define kAirPlayAudioFormat_PCM_16KHz_16Bit_Stereo		( 1 << 5 )	// 0x00000020
#define kAirPlayAudioFormat_PCM_24KHz_16Bit_Mono		( 1 << 6 )	// 0x00000040
#define kAirPlayAudioFormat_PCM_24KHz_16Bit_Stereo		( 1 << 7 )	// 0x00000080
#define kAirPlayAudioFormat_PCM_32KHz_16Bit_Mono		( 1 << 8 )	// 0x00000100
#define kAirPlayAudioFormat_PCM_32KHz_16Bit_Stereo		( 1 << 9 )	// 0x00000200
#define kAirPlayAudioFormat_PCM_44KHz_16Bit_Mono		( 1 << 10 )	// 0x00000400
#define kAirPlayAudioFormat_PCM_44KHz_16Bit_Stereo		( 1 << 11 )	// 0x00000800
#define kAirPlayAudioFormat_PCM_44KHz_24Bit_Mono		( 1 << 12 )	// 0x00001000
#define kAirPlayAudioFormat_PCM_44KHz_24Bit_Stereo		( 1 << 13 )	// 0x00002000
#define kAirPlayAudioFormat_PCM_48KHz_16Bit_Mono		( 1 << 14 )	// 0x00004000
#define kAirPlayAudioFormat_PCM_48KHz_16Bit_Stereo		( 1 << 15 )	// 0x00008000
#define kAirPlayAudioFormat_PCM_48KHz_24Bit_Mono		( 1 << 16 )	// 0x00010000
#define kAirPlayAudioFormat_PCM_48KHz_24Bit_Stereo		( 1 << 17 )	// 0x00020000
#define kAirPlayAudioFormat_ALAC_44KHz_16Bit_Stereo		( 1 << 18 )	// 0x00040000
#define kAirPlayAudioFormat_ALAC_44KHz_24Bit_Stereo		( 1 << 19 )	// 0x00080000
#define kAirPlayAudioFormat_ALAC_48KHz_16Bit_Stereo		( 1 << 20 )	// 0x00100000
#define kAirPlayAudioFormat_ALAC_48KHz_24Bit_Stereo		( 1 << 21 )	// 0x00200000
#define kAirPlayAudioFormat_AAC_LC_44KHz_Stereo			( 1 << 22 )	// 0x00400000
#define kAirPlayAudioFormat_AAC_LC_48KHz_Stereo			( 1 << 23 )	// 0x00800000
#define kAirPlayAudioFormat_AAC_ELD_44KHz_Stereo		( 1 << 24 )	// 0x01000000
#define kAirPlayAudioFormat_AAC_ELD_48KHz_Stereo		( 1 << 25 )	// 0x02000000
#define kAirPlayAudioFormat_AAC_ELD_16KHz_Mono			( 1 << 26 ) // 0x04000000
#define kAirPlayAudioFormat_AAC_ELD_24KHz_Mono			( 1 << 27 ) // 0x08000000
#define kAirPlayAudioFormat_OPUS_16KHz_Mono				( 1 << 28 ) // 0x10000000
#define kAirPlayAudioFormat_OPUS_24KHz_Mono				( 1 << 29 ) // 0x20000000
#define kAirPlayAudioFormat_OPUS_48KHz_Mono				( 1 << 30 ) // 0x40000000

#define kNoErr						0		//! No error occurred.
#define kInProgressErr				1		//! Operation in progress.

// Generic error codes are in the range -6700 to -6779.

#define kGenericErrorBase			-6700	//! Starting error code for all generic errors.

#define kUnknownErr					-6700	//! Unknown error occurred.
#define kOptionErr					-6701	//! Option was not acceptable.
#define kSelectorErr				-6702	//! Selector passed in is invalid or unknown.
#define kExecutionStateErr			-6703	//! Call made in the wrong execution state (e.g. called at interrupt time).
#define kPathErr					-6704	//! Path is invalid, too long, or otherwise not usable.
#define kParamErr					-6705	//! Parameter is incorrect, missing, or not appropriate.
#define kUserRequiredErr			-6706	//! User interaction is required.
#define kCommandErr					-6707	//! Command invalid or not supported.
#define kIDErr						-6708	//! Unknown, invalid, or inappropriate identifier.
#define kStateErr					-6709	//! Not in appropriate state to perform operation.
#define kRangeErr					-6710	//! Index is out of range or not valid.
#define kRequestErr					-6711	//! Request was improperly formed or not appropriate.
#define kResponseErr				-6712	//! Response was incorrect or out of sequence.
#define kChecksumErr				-6713	//! Checksum does not match the actual data.
#define kNotHandledErr				-6714	//! Operation was not handled (or not handled completely).
#define kVersionErr					-6715	//! Version is not correct or not compatible.
#define kSignatureErr				-6716	//! Signature did not match what was expected.
#define kFormatErr					-6717	//! Unknown, invalid, or inappropriate file/data format.
#define kNotInitializedErr			-6718	//! Action request before needed services were initialized.
#define kAlreadyInitializedErr		-6719	//! Attempt made to initialize when already initialized.
#define kNotInUseErr				-6720	//! Object not in use (e.g. cannot abort if not already in use).
#define kAlreadyInUseErr			-6721	//! Object is in use (e.g. cannot reuse active param blocks).
#define kTimeoutErr					-6722	//! Timeout occurred.
#define kCanceledErr				-6723	//! Operation canceled (successful cancel).
#define kAlreadyCanceledErr			-6724	//! Operation has already been canceled.
#define kCannotCancelErr			-6725	//! Operation could not be canceled (maybe already done or invalid).
#define kDeletedErr					-6726	//! Object has already been deleted.
#define kNotFoundErr				-6727	//! Something was not found.
#define kNoMemoryErr				-6728	//! Not enough memory was available to perform the operation.
#define kNoResourcesErr				-6729	//! Resources unavailable to perform the operation.
#define kDuplicateErr				-6730	//! Duplicate found or something is a duplicate.
#define kImmutableErr				-6731	//! Entity is not changeable.
#define kUnsupportedDataErr			-6732	//! Data is unknown or not supported.
#define kIntegrityErr				-6733	//! Data is corrupt.
#define kIncompatibleErr			-6734	//! Data is not compatible or it is in an incompatible format.
#define kUnsupportedErr				-6735	//! Feature or option is not supported.
#define kUnexpectedErr				-6736	//! Error occurred that was not expected.
#define kValueErr					-6737	//! Value is not appropriate.
#define kNotReadableErr				-6738	//! Could not read or reading is not allowed.
#define kNotWritableErr				-6739	//! Could not write or writing is not allowed.
#define	kBadReferenceErr			-6740	//! An invalid or inappropriate reference was specified.
#define	kFlagErr					-6741	//! An invalid, inappropriate, or unsupported flag was specified.
#define	kMalformedErr				-6742	//! Something was not formed correctly.
#define	kSizeErr					-6743	//! Size was too big, too small, or not appropriate.
#define	kNameErr					-6744	//! Name was not correct, allowed, or appropriate.
#define	kNotPreparedErr				-6745	//! Device or service is not ready.
#define	kReadErr					-6746	//! Could not read.
#define	kWriteErr					-6747	//! Could not write.
#define	kMismatchErr				-6748	//! Something does not match.
#define	kDateErr					-6749	//! Date is invalid or out-of-range.
#define	kUnderrunErr				-6750	//! Less data than expected.
#define	kOverrunErr					-6751	//! More data than expected.
#define	kEndingErr					-6752	//! Connection, session, or something is ending.
#define	kConnectionErr				-6753	//! Connection failed or could not be established.
#define	kAuthenticationErr			-6754	//! Authentication failed or is not supported.
#define	kOpenErr					-6755	//! Could not open file, pipe, device, etc.
#define	kTypeErr					-6756	//! Incorrect or incompatible type (e.g. file, data, etc.).
#define	kSkipErr					-6757	//! Items should be or was skipped.
#define	kNoAckErr					-6758	//! No acknowledge.
#define	kCollisionErr				-6759	//! Collision occurred (e.g. two on bus at same time).
#define	kBackoffErr					-6760	//! Backoff in progress and operation intentionally failed.
#define	kAddressErr					-6761	//! Bad address or no acknowledge of address.
#define	kInternalErr				-6762	//! An error internal to the implementation occurred.
#define	kNoSpaceErr					-6763	//! Not enough space to perform operation.
#define	kCountErr					-6764	//! Count is incorrect.
#define	kEndOfDataErr				-6765	//! Reached the end of the data (e.g. recv returned 0).
#define	kWouldBlockErr				-6766	//! Would need to block to continue (e.g. non-blocking read/write).
#define	kLookErr					-6767	//! Special case that needs to be looked at (e.g. interleaved data).
#define	kSecurityRequiredErr		-6768	//! Security is required for the operation (e.g. must use encryption).
#define	kOrderErr					-6769	//! Order is incorrect.
#define	kUpgradeErr					-6770	//! Must upgrade.
#define kAsyncNoErr					-6771	//! Async operation successfully started and is now in progress.
#define kDeprecatedErr				-6772	//! Operation or data is deprecated.
#define kPermissionErr				-6773	//! Permission denied.

#define kGenericErrorEnd			-6779	//! Last generic error code (inclusive)

#endif

#pragma pack(push)

#pragma pack(1)

typedef struct
{
	unsigned char PlayPause:1;
	unsigned char ScanNextTrack:1;
	unsigned char ScanPreviousTrack:1;
	unsigned char Play:1;
	unsigned char Pause:1;
	unsigned char ACHome:1;
	unsigned char ACBack:1;
	unsigned char NoUsed:1;
}BitFieldButtonHIDReport_t;

typedef union
{
	unsigned char ucReport;
	BitFieldButtonHIDReport_t unReprot;
}ButtonHIDReport_t;

typedef struct
{
	unsigned int HookSwitch:1;
	unsigned int Flash:1;
	unsigned int Drop:1;
	unsigned int PhoneMute:1;
	unsigned int PhoneKey0:1;
	unsigned int PhoneKey1:1;
	unsigned int PhoneKey2:1;
	unsigned int PhoneKey3:1;
	unsigned int PhoneKey4:1;
	unsigned int PhoneKey5:1;
	unsigned int PhoneKey6:1;
	unsigned int PhoneKey7:1;
	unsigned int PhoneKey8:1;
	unsigned int PhoneKey9:1;
	unsigned int PhoneKeyStar:1;
	unsigned int PhoneKeyPound:1;
	unsigned int Delete:1;
}BitFieldPhoneKeyHIDReport_t;

typedef union
{
	unsigned int uiReport;
	BitFieldPhoneKeyHIDReport_t unReprot;
}PhoneKeyHIDReport_t;

typedef struct
{
    unsigned char GPGGA:1;
    unsigned char GPRMC:1;
    unsigned char GPGSV:1;
    unsigned char PASCD:1;
    unsigned char PAGCD:1;
    unsigned char PAACD:1;
    unsigned char GPHDT:1;
	unsigned char Reserve:1;
} BitFieldLocationInfo_t;

typedef union
{
	unsigned char ucInfo;
	BitFieldLocationInfo_t unInfo;
}LocationInfo_t;

typedef struct
{
	double		timeOffset;	// Time offset in seconds from the reference time of this sentence.
	double		speed;		// Vehicle speed in meters/second for this sample.
}	PASCDSample_t;

#define kNMEATransmissionState_Drive		'D'
#define kNMEATransmissionState_Neutral		'N'
#define kNMEATransmissionState_Park			'P'
#define kNMEATransmissionState_Reverse		'R'
#define kNMEATransmissionState_Unknown		'U'

typedef struct
{
	double			timestamp;					// Reference time in seconds. May roll over.
	char			transmissionState;			// State of transmission. See kNMEATransmissionState_*.
	BOOL			slipDetect;					// 1 = wheel slippage detected. 0 = no slip was detected.
	int				sampleCount;				// Number of sensor samples parsed in this sentence. 1-50.
	PASCDSample_t	samples[50];				// Sensor samples.
}	PASCDData_t;

typedef struct
{
    unsigned char softKeyboard:1;
    unsigned char softPhoneKeypad:1;
    unsigned char nonMusicLists:1;
    unsigned char musicLists:1;
    unsigned char japanMaps:1;
	unsigned char Reserve:3;
} BitFieldLimitedUIElement_t;

#pragma pack(pop)

typedef enum
{
	eCarplayAPIUSBPort1 = 1,
	eCarplayAPIUSBPort2 = 2
}CarplayAPIUSBPort;

typedef enum
{
	eCarplayAPITestModeDisable = 0,
	eCarplayAPITestModeJState = 1,
	eCarplayAPITestModeKState = 2,
	eCarplayAPITestModeSE0NAK = 3,
	eCarplayAPITestModePacket = 4,
	eCarplayAPITestModeForceHS = 5,
	eCarplayAPITestModeForceFS = 6,
	eCarplayAPITestModeForceLS = 7
}CarplayAPITestModeType;

typedef enum
{
	eAudioType_Main_Default = 0,
	eAudioType_Main_Alert,
	eAudioType_Main_Media,
	eAudioType_Main_Telephony,
	eAudioType_Main_SpeechRecognition,
	eAudioType_Main_Compatibility,
	eAudioType_Alt_Default,
	eAudioType_Alt_Compatibility,
	eAudioType_MainHigh_Media,
	eAudioType_Input,
	eAudioType_MaxNum
}AudioType;

typedef enum
{
	kIAP2BatteryChargingStateDisabled                             = 0,
	kIAP2BatteryChargingStateCharging                             = 1,
	kIAP2BatteryChargingStateCharged                              = 2,
}IAP2BatteryChargingStateEnums;

typedef enum
{
	eCarplayAPIIconType_120_120 = 0,
	eCarplayAPIIconType_180_180,
	eCarplayAPIIconType_256_256,
	eCarplayAPIIconType_MaxNum
}CarplayAPIIconType;

typedef enum
{
	eAPIEngineTypes_Gasoline = 0,
	eAPIEngineTypes_Diesel,
	eAPIEngineTypes_Electric,
	eAPIEngineTypes_CNG,
}CarplayAPIEngineTypes_IAP2;

typedef void
	( *ControlStreamsCallback_f )(
	PVOID context,
	BOOL inControlStreams);

typedef void
	( *UpdateStreamsCallback_f )(
	PVOID context,
	BOOL inMainAudioStreams,
	BOOL inAltAudioStreams,
	BOOL inMainScreenStreams );

typedef void
( *ConnectStateCallback_f )(
	 PVOID context,
	 BOOL inConnect );

typedef void
( *IAP2ConnectStateCallback_f )(
	 PVOID context,
	 BOOL inConnect );

typedef void
	( *ModesChangedCallback_f )(
	PVOID context,
	const AirPlayModeState * inState);

typedef void
	( *RequestUICallback_f )(
	PVOID context,
	const char * inURL);

typedef void
	( *DuckAudioCallback_f )(
	PVOID context,
	double inDurationSecs,
	double inVolume);

typedef void
	( *UnduckAudioCallback_f )(
	PVOID context,
	double inDurationSecs);

typedef void
	( *DisableBluetoothCallback_f )(
	PVOID context,
	const unsigned char mac[ 6 ]);

typedef void
	( *StartLocationInformationCallback_f )(
	PVOID context,
	const LocationInfo_t* inLocationInfo);

typedef void
	( *StopLocationInformationCallback_f )(
	PVOID context);

typedef void
	( *IsExternalChargerConnectedCallback_f )(
	PVOID context,
	BOOL inConnected);

typedef void
	( *BatteryChargingStateCallback_f )(
	PVOID context,
	IAP2BatteryChargingStateEnums inBatteryChargingState);

typedef void
	( *BatteryChargeLevelCallback_f )(
	PVOID context,
	unsigned short inBatteryChargeLevel);

typedef void
	( *TelephonyAudioFormatCallback_f )(
	PVOID context,
	AirPlayAudioFormat inFormat);

typedef void
	( *GetRightHandCallback_f )(
	PVOID context,
	BOOL* outRightHand);

typedef void
	( *GetEnhancedRequestCarUICallback_f )(
	PVOID context,
	BOOL* outEnhancedRequestCarUI);

typedef void
	( *GetInitialModesCallback_f )(
	PVOID context,
	AirPlayModeChanges * outInitialModes );

typedef void
	( *GetBluetoothIDsCallback_f )(
	PVOID context,
	unsigned char ** outMAC,
	size_t  inMaxMACCount,
	size_t* outMACCount );

typedef void
	( *DeviceInformationUpdateCallback_f )(
	PVOID context,
	LPCWSTR inDeviceName );

typedef void
	( *DeviceTimeUpdateCallback_f )(
	PVOID context,
	unsigned long long inSecondsSinceReferenceDate,
	short inTimeZoneOffsetMinutes,
	char inDaylightSavingsOffsetMinutes );

typedef void
	( *DeviceUUIDUpdateCallback_f )(
	PVOID context,
	LPCWSTR inUUID );

typedef BOOL
	( *DeviceSetMacCallback_f )(
	PVOID context,
	unsigned char* inMacData,
	size_t inMacDataLen);

typedef OSStatus ( *I2COpen_f ) ( void );
typedef OSStatus ( *I2CClose_f ) ( void );
typedef OSStatus ( *I2CWrite_f ) ( UINT32 ulRegAddr, UINT8* pRegValBuf, UINT uiValBufLen );
typedef OSStatus ( *I2CRead_f ) ( UINT32 ulRegAddr, UINT8* pRegValBuf, UINT uiValBufLen );

typedef OSStatus ( *StoreCa_f ) ( const void * buf, size_t bufsize );

typedef void ( *IperfReport_f ) ( PVOID context, TCHAR * report );

typedef struct __CARPLAY_INIT_PARAM
{
	const wchar_t * ServicePath;
	const wchar_t * ClientPath;
	UINT AudioTypeDeviceIDList[eAudioType_MaxNum];
	ControlStreamsCallback_f ControlStreamsCallback;
	PVOID ControlStreamsContext;
	UpdateStreamsCallback_f UpdateStreamsCallback;
	PVOID UpdateStreamsContext;
	ConnectStateCallback_f ConnectStateCallback;
	PVOID ConnectStateContext;
	IAP2ConnectStateCallback_f IAP2ConnectStateCallback;
	PVOID IAP2ConnectStateContext;
	ModesChangedCallback_f ModesChangedCallback;
	PVOID ModesChangedContext;
	RequestUICallback_f RequestUICallback;
	PVOID RequestUIContext;
	DuckAudioCallback_f DuckAudioCallback;
	PVOID DuckAudioContext;
	UnduckAudioCallback_f UnduckAudioCallback;
	PVOID UnduckAudioContext;
	DisableBluetoothCallback_f DisableBluetoothCallback;
	PVOID DisableBluetoothContext;
	StartLocationInformationCallback_f StartLocationInformationCallback;
	PVOID StartLocationInformationContext;
	StopLocationInformationCallback_f StopLocationInformationCallback;
	PVOID StopLocationInformationContext;
	IsExternalChargerConnectedCallback_f IsExternalChargerConnectedCallback;
	PVOID IsExternalChargerConnectedContext;
	BatteryChargingStateCallback_f BatteryChargingStateCallback;
	PVOID BatteryChargingStateContext;
	BatteryChargeLevelCallback_f BatteryChargeLevelCallback;
	PVOID BatteryChargeLevelContext;
	TelephonyAudioFormatCallback_f TelephonyAudioFormatCallback;
	PVOID TelephonyAudioFormatContext;
	TelephonyAudioFormatCallback_f SpeechAudioFormatCallback;
	PVOID SpeechAudioFormatContext;
	GetRightHandCallback_f GetRightHandCallback;
	PVOID GetRightHandContext;
	GetEnhancedRequestCarUICallback_f GetEnhancedRequestCarUICallback;
	PVOID GetEnhancedRequestCarUIContext;
	GetInitialModesCallback_f GetInitialModesCallback;
	PVOID GetInitialModesContext;
	GetBluetoothIDsCallback_f GetBluetoothIDsCallback;
	PVOID GetBluetoothIDsContext;
	DeviceInformationUpdateCallback_f DeviceInformationUpdateCallback;
	PVOID DeviceInformationUpdateContext;
	DeviceTimeUpdateCallback_f DeviceTimeUpdateCallback;
	PVOID DeviceTimeUpdateContext;
	DeviceUUIDUpdateCallback_f DeviceUUIDUpdateCallback;
	PVOID DeviceUUIDUpdateContext;
	DeviceSetMacCallback_f DeviceSetMacCallback;
	PVOID DeviceSetMacContext;
	
	I2COpen_f I2COpenCallback;
	I2CClose_f I2CCloseCallback;
	I2CWrite_f I2CWriteCallback;
	I2CRead_f I2CReadCallback;

	IperfReport_f IperfReportCallback;
	PVOID IperfReportContext;
	IperfReport_f IperfResultsCallback;
	PVOID IperfResultsContext;

	int VideoWidth;
	int VideoHeight;
	int PhysicalWidth;
	int PhysicalHeight;
	
	unsigned int GPSUARTPort;
	LocationInfo_t LocationInfoEnable;
	const char * FirmwareRevision;
	const char * HardwareRevision;
	CarplayAPIEngineTypes_IAP2 VehicleEngineType;
	const wchar_t * VehicleDisplayName;
	LPCWSTR* languages;
	unsigned short languagenum;
	unsigned short curlanguageindex;
	const char * Manufacturer;
	const char * Model;
	BOOL HiFiTouch;
	BOOL IconVisible;
	const char * prioriconpath;
	const char * iconpath[eCarplayAPIIconType_MaxNum];
	BOOL iconprerendered[eCarplayAPIIconType_MaxNum];
	const char * iconlabel;
	BitFieldLimitedUIElement_t LimitedUIElement;
}CARPLAY_INIT_PARAM, *PCARPLAY_INIT_PARAM;

#define CarplayInitParamInit( PTR )	memset( (PTR), 0, sizeof( *(PTR) ) )

OSStatus CarplayAPIGetVersion(char* versionstr, size_t sizeInBytes);

BOOL CarplayAPIInit( const CARPLAY_INIT_PARAM * param );
BOOL CarplayAPIDeinit( void );
OSStatus CarplayAPIRestart( void );

OSStatus CarplayAPIShowWindow(BOOL inShow);

OSStatus CarplayAPIChangeModes(
	const AirPlayModeChanges *					inChanges, 
	const char*									inReason);

#define CarplayAPIChangePhoneCall( ON_PHONE, REASON ) \
	CarplayAPIChangeAppState( kAirPlaySpeechMode_NotApplicable, \
		(ON_PHONE) ? kAirPlayTriState_True : kAirPlayTriState_False, kAirPlayTriState_NotApplicable, \
		(REASON))

#define CarplayAPIChangeSpeechMode( SPEECH_MODE, REASON ) \
	CarplayAPIChangeAppState( (SPEECH_MODE), kAirPlayTriState_NotApplicable, \
		kAirPlayTriState_NotApplicable, (REASON) )

#define CarplayAPIChangeTurnByTurn( TURN_BY_TURN, REASON ) \
	CarplayAPIChangeAppState( kAirPlaySpeechMode_NotApplicable, kAirPlayTriState_NotApplicable, \
		(TURN_BY_TURN) ? kAirPlayTriState_True : kAirPlayTriState_False, (REASON) )

OSStatus CarplayAPIChangeAppState(
	AirPlaySpeechMode							inSpeechMode,
	AirPlayTriState								inPhoneCall, 
	AirPlayTriState								inTurnByTurn,
	const char*									inReason);

// Screen

#define CarplayAPITakeScreen( PRIORITY, TAKE_CONSTRAINT, BORROW_CONSTRAINT, REASON ) \
	CarplayAPIChangeResourceMode( kAirPlayResourceID_MainScreen, kAirPlayTransferType_Take, (PRIORITY), \
		(TAKE_CONSTRAINT), (BORROW_CONSTRAINT), (REASON) )

#define CarplayAPIUntakeScreen( REASON ) \
	CarplayAPIChangeResourceMode( kAirPlayResourceID_MainScreen, kAirPlayTransferType_Untake, \
		kAirPlayTransferPriority_NotApplicable, kAirPlayConstraint_NotApplicable, kAirPlayConstraint_NotApplicable, \
		(REASON) )

#define CarplayAPIBorrowScreen( PRIORITY, UNBORROW_CONSTRAINT, REASON ) \
	CarplayAPIChangeResourceMode( kAirPlayResourceID_MainScreen, kAirPlayTransferType_Borrow, (PRIORITY), \
		kAirPlayConstraint_NotApplicable, (UNBORROW_CONSTRAINT), (REASON) )

#define CarplayAPIUnborrowScreen( REASON ) \
	CarplayAPIChangeResourceMode( kAirPlayResourceID_MainScreen, kAirPlayTransferType_Unborrow, \
		kAirPlayTransferPriority_NotApplicable, kAirPlayConstraint_NotApplicable, kAirPlayConstraint_NotApplicable, \
		(REASON) )

// MainAudio

#define CarplayAPITakeMainAudio( PRIORITY, TAKE_CONSTRAINT, BORROW_CONSTRAINT, REASON ) \
	CarplayAPIChangeResourceMode( kAirPlayResourceID_MainAudio, kAirPlayTransferType_Take, (PRIORITY), \
		(TAKE_CONSTRAINT), (BORROW_CONSTRAINT), (REASON) )

#define CarplayAPIUntakeMainAudio( REASON ) \
	CarplayAPIChangeResourceMode( kAirPlayResourceID_MainAudio, kAirPlayTransferType_Untake, \
		kAirPlayTransferPriority_NotApplicable, kAirPlayConstraint_NotApplicable, kAirPlayConstraint_NotApplicable, \
		(REASON) )

#define CarplayAPIBorrowMainAudio( PRIORITY, UNBORROW_CONSTRAINT, REASON ) \
	CarplayAPIChangeResourceMode( kAirPlayResourceID_MainAudio, kAirPlayTransferType_Borrow, (PRIORITY), \
		kAirPlayConstraint_NotApplicable, (UNBORROW_CONSTRAINT), (REASON) )

#define CarplayAPIUnborrowMainAudio( REASON ) \
	CarplayAPIChangeResourceMode( kAirPlayResourceID_MainAudio, kAirPlayTransferType_Unborrow, \
		kAirPlayTransferPriority_NotApplicable, kAirPlayConstraint_NotApplicable, kAirPlayConstraint_NotApplicable, \
		(REASON) )

OSStatus CarplayAPIChangeResourceMode(
	AirPlayResourceID							inResourceID, 
	AirPlayTransferType							inType, 
	AirPlayTransferPriority						inPriority, 
	AirPlayConstraint							inTakeConstraint, 
	AirPlayConstraint							inBorrowOrUnborrowConstraint, 
	const char*									inReason);

OSStatus CarplayAPIDuckAudio(double inDurationSecs, double inVolume);
OSStatus CarplayAPIUnduckAudio(double inDurationSecs);
OSStatus CarplayAPIButtonKey(ButtonHIDReport_t inReport);
OSStatus CarplayAPIPhoneKey(PhoneKeyHIDReport_t inReport);
OSStatus CarplayAPILimitedUI(BOOL inLimitUI);
OSStatus CarplayAPIRequestSiri(AirPlaySiriAction inAction);
OSStatus CarplayAPIRequestUI(const char* inURL);
OSStatus CarplayAPIForceKeyFrame();
OSStatus CarplayAPITestMode(BOOL inTestMode);
OSStatus CarplayAPISetNightMode(BOOL inNightMode);

OSStatus CarplayAPISwitchHost(CarplayAPIUSBPort Port);
OSStatus CarplayAPISwitchDevice(CarplayAPIUSBPort Port);
OSStatus CarplayAPISetTestMode(CarplayAPIUSBPort Port, CarplayAPITestModeType TestType);

OSStatus CarplayAPIGetDeviceID(wchar_t* deviceid, size_t sizeInWords);
OSStatus CarplayAPIIsActivated(BOOL* active);
OSStatus CarplayAPIInstallCA(const void * buf, size_t bufsize);
OSStatus CarplayAPISetStoreCaCallBack(StoreCa_f inFunc);
OSStatus CarplayAPIWaitServerInit(DWORD dwMilliseconds, BOOL* outServerInit);
OSStatus CarplayAPISingleTouch( BOOL inTouch, unsigned short inX, unsigned short inY );
OSStatus CarplayAPISendKnob( BOOL inButton, BOOL inHome, BOOL inBack, char inX, char inY, char inWheel );
OSStatus CarplayAPISendPASCD( const PASCDData_t * inData  );
OSStatus CarplayAPISendNMEA( char ** inSentences, unsigned int inCount );
OSStatus CarplayAPIIperfStart( int argc, LPCWSTR *argv, void** outContext );
OSStatus CarplayAPIIperfStop( void* inContext );
OSStatus CarplayAPIGetSessionAddress(char* outAddress, size_t sizeInBytes);
OSStatus CarplayAPIGetServerAddress(char* outAddress, size_t sizeInBytes);
OSStatus CarplayAPIStringToSockAddr( const char *inStr, void *outSA, size_t inSASize, size_t *outSASize );
OSStatus CarplayAPIStartH264Test( const char* inAVCCPath, const char* inH264Path );
OSStatus CarplayAPITakeGstResource( BOOL bTake );
OSStatus CarplayAPIConnectTester( void *inBuf, size_t inMaxLen );

#ifdef	__cplusplus
	}
#endif

#endif
