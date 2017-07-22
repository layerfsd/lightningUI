// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ANDROIDAUTOAPI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ANDROIDAUTOAPI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef ANDROID_AUTO_API_H
#define ANDROID_AUTO_API_H

#include <xstring>

typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16  int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

using namespace std;

#define	Stringify( X )				# X
#define	StringifyExpansion( X )		Stringify( X )

#define AndroidAutoAPIMajorVersion			1
#define AndroidAutoAPIMinorVersion			12
#define AndroidAutoAPIRevisionVersion		3
#define AndroidAutoAPIBuildVersion			1

#define AndroidAutoAPIVersionNumber			AndroidAutoAPIMajorVersion,\
AndroidAutoAPIMinorVersion,AndroidAutoAPIRevisionVersion,AndroidAutoAPIBuildVersion

#define AndroidAutoAPIVersionNumberStr		StringifyExpansion(AndroidAutoAPIMajorVersion.\
AndroidAutoAPIMinorVersion.AndroidAutoAPIRevisionVersion)

#define AndroidAutoAPIBuildVersionStr		StringifyExpansion(AndroidAutoAPIBuildVersion)

#ifdef ANDROIDAUTOAPI_EXPORTS
#define ANDROIDAUTOAPI_API __declspec(dllexport)
#else
#define ANDROIDAUTOAPI_API __declspec(dllimport)
#endif

namespace AndroidAutoAPI
{

enum AudioFocusRequestType {
	AUDIO_FOCUS_GAIN = 1,
	AUDIO_FOCUS_GAIN_TRANSIENT = 2,
	AUDIO_FOCUS_GAIN_TRANSIENT_MAY_DUCK = 3,
	AUDIO_FOCUS_RELEASE = 4
};

enum AudioFocusStateType {
	AUDIO_FOCUS_STATE_GAIN = 1,
	AUDIO_FOCUS_STATE_GAIN_TRANSIENT = 2,
	AUDIO_FOCUS_STATE_LOSS = 3,
	AUDIO_FOCUS_STATE_LOSS_TRANSIENT_CAN_DUCK = 4,
	AUDIO_FOCUS_STATE_LOSS_TRANSIENT = 5,
	AUDIO_FOCUS_STATE_GAIN_MEDIA_ONLY = 6,
	AUDIO_FOCUS_STATE_GAIN_TRANSIENT_GUIDANCE_ONLY = 7
};

enum VideoFocusMode {
	VIDEO_FOCUS_PROJECTED = 1,
	VIDEO_FOCUS_NATIVE = 2,
	VIDEO_FOCUS_NATIVE_TRANSIENT = 3
};

enum NavFocusType {
	NAV_FOCUS_NATIVE = 1,
	NAV_FOCUS_PROJECTED = 2
};

enum BluetoothPairingMethod {
  BLUETOOTH_PAIRING_OOB = 1,
  BLUETOOTH_PAIRING_NUMERIC_COMPARISON = 2,
  BLUETOOTH_PAIRING_PASSKEY_ENTRY = 3,
  BLUETOOTH_PAIRING_PIN = 4
};

enum KeyCode {
	KEYCODE_UNKNOWN = 0,
	KEYCODE_SOFT_LEFT = 1,
	KEYCODE_SOFT_RIGHT = 2,
	KEYCODE_HOME = 3,
	KEYCODE_BACK = 4,
	KEYCODE_CALL = 5,
	KEYCODE_ENDCALL = 6,
	KEYCODE_0 = 7,
	KEYCODE_1 = 8,
	KEYCODE_2 = 9,
	KEYCODE_3 = 10,
	KEYCODE_4 = 11,
	KEYCODE_5 = 12,
	KEYCODE_6 = 13,
	KEYCODE_7 = 14,
	KEYCODE_8 = 15,
	KEYCODE_9 = 16,
	KEYCODE_STAR = 17,
	KEYCODE_POUND = 18,
	KEYCODE_DPAD_UP = 19,
	KEYCODE_DPAD_DOWN = 20,
	KEYCODE_DPAD_LEFT = 21,
	KEYCODE_DPAD_RIGHT = 22,
	KEYCODE_DPAD_CENTER = 23,
	KEYCODE_VOLUME_UP = 24,
	KEYCODE_VOLUME_DOWN = 25,
	KEYCODE_POWER = 26,
	KEYCODE_CAMERA = 27,
	KEYCODE_CLEAR = 28,
	KEYCODE_A = 29,
	KEYCODE_B = 30,
	KEYCODE_C = 31,
	KEYCODE_D = 32,
	KEYCODE_E = 33,
	KEYCODE_F = 34,
	KEYCODE_G = 35,
	KEYCODE_H = 36,
	KEYCODE_I = 37,
	KEYCODE_J = 38,
	KEYCODE_K = 39,
	KEYCODE_L = 40,
	KEYCODE_M = 41,
	KEYCODE_N = 42,
	KEYCODE_O = 43,
	KEYCODE_P = 44,
	KEYCODE_Q = 45,
	KEYCODE_R = 46,
	KEYCODE_S = 47,
	KEYCODE_T = 48,
	KEYCODE_U = 49,
	KEYCODE_V = 50,
	KEYCODE_W = 51,
	KEYCODE_X = 52,
	KEYCODE_Y = 53,
	KEYCODE_Z = 54,
	KEYCODE_COMMA = 55,
	KEYCODE_PERIOD = 56,
	KEYCODE_ALT_LEFT = 57,
	KEYCODE_ALT_RIGHT = 58,
	KEYCODE_SHIFT_LEFT = 59,
	KEYCODE_SHIFT_RIGHT = 60,
	KEYCODE_TAB = 61,
	KEYCODE_SPACE = 62,
	KEYCODE_SYM = 63,
	KEYCODE_EXPLORER = 64,
	KEYCODE_ENVELOPE = 65,
	KEYCODE_ENTER = 66,
	KEYCODE_DEL = 67,
	KEYCODE_GRAVE = 68,
	KEYCODE_MINUS = 69,
	KEYCODE_EQUALS = 70,
	KEYCODE_LEFT_BRACKET = 71,
	KEYCODE_RIGHT_BRACKET = 72,
	KEYCODE_BACKSLASH = 73,
	KEYCODE_SEMICOLON = 74,
	KEYCODE_APOSTROPHE = 75,
	KEYCODE_SLASH = 76,
	KEYCODE_AT = 77,
	KEYCODE_NUM = 78,
	KEYCODE_HEADSETHOOK = 79,
	KEYCODE_FOCUS = 80,
	KEYCODE_PLUS = 81,
	KEYCODE_MENU = 82,
	KEYCODE_NOTIFICATION = 83,
	KEYCODE_SEARCH = 84,
	KEYCODE_MEDIA_PLAY_PAUSE = 85,
	KEYCODE_MEDIA_STOP = 86,
	KEYCODE_MEDIA_NEXT = 87,
	KEYCODE_MEDIA_PREVIOUS = 88,
	KEYCODE_MEDIA_REWIND = 89,
	KEYCODE_MEDIA_FAST_FORWARD = 90,
	KEYCODE_MUTE = 91,
	KEYCODE_PAGE_UP = 92,
	KEYCODE_PAGE_DOWN = 93,
	KEYCODE_PICTSYMBOLS = 94,
	KEYCODE_SWITCH_CHARSET = 95,
	KEYCODE_BUTTON_A = 96,
	KEYCODE_BUTTON_B = 97,
	KEYCODE_BUTTON_C = 98,
	KEYCODE_BUTTON_X = 99,
	KEYCODE_BUTTON_Y = 100,
	KEYCODE_BUTTON_Z = 101,
	KEYCODE_BUTTON_L1 = 102,
	KEYCODE_BUTTON_R1 = 103,
	KEYCODE_BUTTON_L2 = 104,
	KEYCODE_BUTTON_R2 = 105,
	KEYCODE_BUTTON_THUMBL = 106,
	KEYCODE_BUTTON_THUMBR = 107,
	KEYCODE_BUTTON_START = 108,
	KEYCODE_BUTTON_SELECT = 109,
	KEYCODE_BUTTON_MODE = 110,
	KEYCODE_ESCAPE = 111,
	KEYCODE_FORWARD_DEL = 112,
	KEYCODE_CTRL_LEFT = 113,
	KEYCODE_CTRL_RIGHT = 114,
	KEYCODE_CAPS_LOCK = 115,
	KEYCODE_SCROLL_LOCK = 116,
	KEYCODE_META_LEFT = 117,
	KEYCODE_META_RIGHT = 118,
	KEYCODE_FUNCTION = 119,
	KEYCODE_SYSRQ = 120,
	KEYCODE_BREAK = 121,
	KEYCODE_MOVE_HOME = 122,
	KEYCODE_MOVE_END = 123,
	KEYCODE_INSERT = 124,
	KEYCODE_FORWARD = 125,
	KEYCODE_MEDIA_PLAY = 126,
	KEYCODE_MEDIA_PAUSE = 127,
	KEYCODE_MEDIA_CLOSE = 128,
	KEYCODE_MEDIA_EJECT = 129,
	KEYCODE_MEDIA_RECORD = 130,
	KEYCODE_F1 = 131,
	KEYCODE_F2 = 132,
	KEYCODE_F3 = 133,
	KEYCODE_F4 = 134,
	KEYCODE_F5 = 135,
	KEYCODE_F6 = 136,
	KEYCODE_F7 = 137,
	KEYCODE_F8 = 138,
	KEYCODE_F9 = 139,
	KEYCODE_F10 = 140,
	KEYCODE_F11 = 141,
	KEYCODE_F12 = 142,
	KEYCODE_NUM_LOCK = 143,
	KEYCODE_NUMPAD_0 = 144,
	KEYCODE_NUMPAD_1 = 145,
	KEYCODE_NUMPAD_2 = 146,
	KEYCODE_NUMPAD_3 = 147,
	KEYCODE_NUMPAD_4 = 148,
	KEYCODE_NUMPAD_5 = 149,
	KEYCODE_NUMPAD_6 = 150,
	KEYCODE_NUMPAD_7 = 151,
	KEYCODE_NUMPAD_8 = 152,
	KEYCODE_NUMPAD_9 = 153,
	KEYCODE_NUMPAD_DIVIDE = 154,
	KEYCODE_NUMPAD_MULTIPLY = 155,
	KEYCODE_NUMPAD_SUBTRACT = 156,
	KEYCODE_NUMPAD_ADD = 157,
	KEYCODE_NUMPAD_DOT = 158,
	KEYCODE_NUMPAD_COMMA = 159,
	KEYCODE_NUMPAD_ENTER = 160,
	KEYCODE_NUMPAD_EQUALS = 161,
	KEYCODE_NUMPAD_LEFT_PAREN = 162,
	KEYCODE_NUMPAD_RIGHT_PAREN = 163,
	KEYCODE_VOLUME_MUTE = 164,
	KEYCODE_INFO = 165,
	KEYCODE_CHANNEL_UP = 166,
	KEYCODE_CHANNEL_DOWN = 167,
	KEYCODE_ZOOM_IN = 168,
	KEYCODE_ZOOM_OUT = 169,
	KEYCODE_TV = 170,
	KEYCODE_WINDOW = 171,
	KEYCODE_GUIDE = 172,
	KEYCODE_DVR = 173,
	KEYCODE_BOOKMARK = 174,
	KEYCODE_CAPTIONS = 175,
	KEYCODE_SETTINGS = 176,
	KEYCODE_TV_POWER = 177,
	KEYCODE_TV_INPUT = 178,
	KEYCODE_STB_POWER = 179,
	KEYCODE_STB_INPUT = 180,
	KEYCODE_AVR_POWER = 181,
	KEYCODE_AVR_INPUT = 182,
	KEYCODE_PROG_RED = 183,
	KEYCODE_PROG_GREEN = 184,
	KEYCODE_PROG_YELLOW = 185,
	KEYCODE_PROG_BLUE = 186,
	KEYCODE_APP_SWITCH = 187,
	KEYCODE_BUTTON_1 = 188,
	KEYCODE_BUTTON_2 = 189,
	KEYCODE_BUTTON_3 = 190,
	KEYCODE_BUTTON_4 = 191,
	KEYCODE_BUTTON_5 = 192,
	KEYCODE_BUTTON_6 = 193,
	KEYCODE_BUTTON_7 = 194,
	KEYCODE_BUTTON_8 = 195,
	KEYCODE_BUTTON_9 = 196,
	KEYCODE_BUTTON_10 = 197,
	KEYCODE_BUTTON_11 = 198,
	KEYCODE_BUTTON_12 = 199,
	KEYCODE_BUTTON_13 = 200,
	KEYCODE_BUTTON_14 = 201,
	KEYCODE_BUTTON_15 = 202,
	KEYCODE_BUTTON_16 = 203,
	KEYCODE_LANGUAGE_SWITCH = 204,
	KEYCODE_MANNER_MODE = 205,
	KEYCODE_3D_MODE = 206,
	KEYCODE_CONTACTS = 207,
	KEYCODE_CALENDAR = 208,
	KEYCODE_MUSIC = 209,
	KEYCODE_CALCULATOR = 210,
	KEYCODE_ZENKAKU_HANKAKU = 211,
	KEYCODE_EISU = 212,
	KEYCODE_MUHENKAN = 213,
	KEYCODE_HENKAN = 214,
	KEYCODE_KATAKANA_HIRAGANA = 215,
	KEYCODE_YEN = 216,
	KEYCODE_RO = 217,
	KEYCODE_KANA = 218,
	KEYCODE_ASSIST = 219,
	KEYCODE_BRIGHTNESS_DOWN = 220,
	KEYCODE_BRIGHTNESS_UP = 221,
	KEYCODE_MEDIA_AUDIO_TRACK = 222,
	KEYCODE_SLEEP = 223,
	KEYCODE_WAKEUP = 224,
	KEYCODE_PAIRING = 225,
	KEYCODE_MEDIA_TOP_MENU = 226,
	KEYCODE_11 = 227,
	KEYCODE_12 = 228,
	KEYCODE_LAST_CHANNEL = 229,
	KEYCODE_TV_DATA_SERVICE = 230,
	KEYCODE_VOICE_ASSIST = 231,
	KEYCODE_TV_RADIO_SERVICE = 232,
	KEYCODE_TV_TELETEXT = 233,
	KEYCODE_TV_NUMBER_ENTRY = 234,
	KEYCODE_TV_TERRESTRIAL_ANALOG = 235,
	KEYCODE_TV_TERRESTRIAL_DIGITAL = 236,
	KEYCODE_TV_SATELLITE = 237,
	KEYCODE_TV_SATELLITE_BS = 238,
	KEYCODE_TV_SATELLITE_CS = 239,
	KEYCODE_TV_SATELLITE_SERVICE = 240,
	KEYCODE_TV_NETWORK = 241,
	KEYCODE_TV_ANTENNA_CABLE = 242,
	KEYCODE_TV_INPUT_HDMI_1 = 243,
	KEYCODE_TV_INPUT_HDMI_2 = 244,
	KEYCODE_TV_INPUT_HDMI_3 = 245,
	KEYCODE_TV_INPUT_HDMI_4 = 246,
	KEYCODE_TV_INPUT_COMPOSITE_1 = 247,
	KEYCODE_TV_INPUT_COMPOSITE_2 = 248,
	KEYCODE_TV_INPUT_COMPONENT_1 = 249,
	KEYCODE_TV_INPUT_COMPONENT_2 = 250,
	KEYCODE_TV_INPUT_VGA_1 = 251,
	KEYCODE_TV_AUDIO_DESCRIPTION = 252,
	KEYCODE_TV_AUDIO_DESCRIPTION_MIX_UP = 253,
	KEYCODE_TV_AUDIO_DESCRIPTION_MIX_DOWN = 254,
	KEYCODE_TV_ZOOM_MODE = 255,
	KEYCODE_TV_CONTENTS_MENU = 256,
	KEYCODE_TV_MEDIA_CONTEXT_MENU = 257,
	KEYCODE_TV_TIMER_PROGRAMMING = 258,
	KEYCODE_HELP = 259,
	KEYCODE_NAVIGATE_PREVIOUS = 260,
	KEYCODE_NAVIGATE_NEXT = 261,
	KEYCODE_NAVIGATE_IN = 262,
	KEYCODE_NAVIGATE_OUT = 263,
	KEYCODE_DPAD_UP_LEFT = 268,
	KEYCODE_DPAD_DOWN_LEFT = 269,
	KEYCODE_DPAD_UP_RIGHT = 270,
	KEYCODE_DPAD_DOWN_RIGHT = 271,
	KEYCODE_SENTINEL = 65535,
	KEYCODE_ROTARY_CONTROLLER = 65536,
	KEYCODE_MEDIA = 65537,
	KEYCODE_NAVIGATION = 65538,
	KEYCODE_RADIO = 65539,
	KEYCODE_TEL = 65540,
	KEYCODE_PRIMARY_BUTTON = 65541,
	KEYCODE_SECONDARY_BUTTON = 65542,
	KEYCODE_TERTIARY_BUTTON = 65543
};

enum SensorType {
	SENSOR_LOCATION = 1,
	SENSOR_COMPASS = 2,
	SENSOR_SPEED = 3,
	SENSOR_RPM = 4,
	SENSOR_ODOMETER = 5,
	SENSOR_FUEL = 6,
	SENSOR_PARKING_BRAKE = 7,
	SENSOR_GEAR = 8,
	SENSOR_OBDII_DIAGNOSTIC_CODE = 9,
	SENSOR_NIGHT_MODE = 10,
	SENSOR_ENVIRONMENT_DATA = 11,
	SENSOR_HVAC_DATA = 12,
	SENSOR_DRIVING_STATUS_DATA = 13,
	SENSOR_DEAD_RECKONING_DATA = 14,
	SENSOR_PASSENGER_DATA = 15,
	SENSOR_DOOR_DATA = 16,
	SENSOR_LIGHT_DATA = 17,
	SENSOR_TIRE_PRESSURE_DATA = 18,
	SENSOR_ACCELEROMETER_DATA = 19,
	SENSOR_GYROSCOPE_DATA = 20,
	SENSOR_GPS_SATELLITE_DATA = 21
};

enum SensorErrorType {
	SENSOR_OK = 1,
	SENSOR_ERROR_TRANSIENT = 2,
	SENSOR_ERROR_PERMANENT = 3
};

enum Gear {
	GEAR_NEUTRAL = 0,
	GEAR_1 = 1,
	GEAR_2 = 2,
	GEAR_3 = 3,
	GEAR_4 = 4,
	GEAR_5 = 5,
	GEAR_6 = 6,
	GEAR_7 = 7,
	GEAR_8 = 8,
	GEAR_9 = 9,
	GEAR_10 = 10,
	GEAR_DRIVE = 100,
	GEAR_PARK = 101,
	GEAR_REVERSE = 102
};

enum DrivingStatus {
	DRIVE_STATUS_UNRESTRICTED = 0,
	DRIVE_STATUS_NO_VIDEO = 1,
	DRIVE_STATUS_NO_KEYBOARD_INPUT = 2,
	DRIVE_STATUS_NO_VOICE_INPUT = 4,
	DRIVE_STATUS_NO_CONFIG = 8,
	DRIVE_STATUS_LIMIT_MESSAGE_LEN = 16
};

enum DriverPosition {
  DRIVER_POSITION_LEFT = 0,
  DRIVER_POSITION_RIGHT = 1,
  DRIVER_POSITION_CENTER = 2,
  DRIVER_POSITION_UNKNOWN = 3
};

enum AudioStreamType {
  AUDIO_STREAM_GUIDANCE = 1,
  AUDIO_STREAM_SYSTEM_AUDIO = 2,
  AUDIO_STREAM_MEDIA = 3,
  AUDIO_STREAM_TELEPHONY = 4
};

enum VoiceSessionStatus {
	VOICE_SESSION_START = 1,
	VOICE_SESSION_END = 2
};

enum VideoFrameRateType {
	VIDEO_FPS_60 = 1,
	VIDEO_FPS_30 = 2
};

enum HeadLightState {
	HEAD_LIGHT_STATE_OFF = 1,
	HEAD_LIGHT_STATE_ON = 2,
	HEAD_LIGHT_STATE_HIGH = 3
};

enum TurnIndicatorState {
	TURN_INDICATOR_NONE = 1,
	TURN_INDICATOR_LEFT = 2,
	TURN_INDICATOR_RIGHT = 3
};

class CAndroidAutoAPIIMPL;

class ANDROIDAUTOAPI_API IAndroidAutoAPICallback {
public:
	virtual void connectCallback(bool connect) = 0;
	virtual void audioFocusRequestCallback(AudioFocusRequestType request) = 0;
	virtual void videoFocusCallback(VideoFocusMode focus, int reason) = 0;
	virtual void naviFocusCallback(NavFocusType type) = 0;
	virtual void onPairingRequest(const string& phoneAddr, BluetoothPairingMethod pairingMethod) = 0;
	virtual void microphoneRequestCallback(bool open, bool ancEnabled, bool ecEnabled, int maxUnacked) = 0;
	virtual void voiceSessionNotificationCallback(VoiceSessionStatus status) = 0;
	virtual void audioPlaybackStartCallback(AudioStreamType type) = 0;
	virtual void audioPlaybackStopCallback(AudioStreamType type) = 0;
	virtual void vedioPlaybackStartCallback() = 0;
	virtual void vedioPlaybackStopCallback() = 0;
};

// This class is exported from the AndroidAutoAPI.dll
class ANDROIDAUTOAPI_API CAndroidAutoAPI {
public:
	CAndroidAutoAPI(void);
	~CAndroidAutoAPI();

	void setIdentityInfo(const string& make, const string& model, const string& year,
		const string& id);
	void setHeadUnitInfo(const string& huMake, const string& huModel,
		const string& huSwBuild, const string& huSwVersion);
	void setDriverPosition(DriverPosition position);

	//KeyCodes
	void addKeycode(KeyCode keycode);
	void addKeycode(KeyCode* keycodearray, size_t arraycount);
	void clearKeycodes();

	//Audio IDs
	void addAudioIDs(AudioStreamType type, int id);
	void clearAudioIDs();
	void setAudioSourceID(int id);

	void setBluetoothAddress(const string& btaddr);

	//Video Parameters
	void setVideoParameters(int32_t nativewidth, int32_t nativeheight, int32_t density, int32_t aspectratio_e4, VideoFrameRateType fps);

	//GPS
	void setGPSPort(uint32_t port);

	//DeviceID
	BOOL InstallCA(const void * buf, size_t bufsize);
	BOOL GetDeviceID(wchar_t* deviceid, size_t sizeInWords);
	BOOL IsActivated();

	BOOL init(IAndroidAutoAPICallback* callback, const wchar_t * clientpath);
	void setAudioFocus(AudioFocusStateType focusState, bool unsolicited);
	void setVideoFocus(VideoFocusMode focusMode, bool unsolicited);
	void setNaviFocus(NavFocusType type);
	void requestDelayedPairing();
	void onReadyForPairing(bool alreadyPaired);
	void sendAuthData(const string& authData);
	void onKeyEvent(KeyCode keycode, bool isDown);
	void setVolume(AudioStreamType type, uint32_t vol);
	void setMicGain(double dbGain);

	//Sensor
	void registerSensor(SensorType type);
	void reportSensorError(SensorType type, SensorErrorType errorType);
	void reportLocationData(uint64_t timestamp, int32_t latitudeE7, int32_t longitudeE7,
		bool hasAccuracy, int32_t accuracyE3, bool hasAltitude, int32_t altitudeE2,
		bool hasSpeed, int32_t speedE3, bool hasBearing, int32_t bearingE6);
	void reportCompassData(int32_t bearingE6);
	void reportCompassData3D(int32_t bearingE6, bool hasPitch, int32_t pitchE6,
		bool hasRoll, int32_t rollE6);
	void reportSpeedData(int32_t speedE3, bool hasCruiseEngaged, bool cruiseEngaged,
		bool hasCruiseSetSpeed, int32_t cruiseSetSpeed);
	void reportRpmData(int32_t rpmE3);
	void reportOdometerData(int32_t kmsE1, bool hasTripKms, int32_t tripKmsE1);
	void reportFuelData(bool hasFuelRemainingPercent, int32_t fuelRemainingPercent,
		bool hasRangeKm, int32_t rangeKm, bool hasLowFuelWarning, bool lowFuelWarning);
	void reportParkingBrakeData(bool engaged);
	void reportGearData(Gear gear);
	void reportDiagnosticsData(uint8_t *data, size_t len);
	void reportNightModeData(bool night_mode);
	void reportEnvironmentData(bool hasTemperature, int32_t temperatureE3,
		bool hasPressure, int32_t pressureE3, bool hasRain, int32_t rain);
	void reportHvacData(bool hasTargetTemperature, int32_t targetTemperatureE3,
		bool hasCurrentTemperature, int32_t currentTemperatureE3);
	void reportDrivingStatusData(int32_t drivingStatus);
	void reportDeadReckoningData(bool hasSteeringAngle, int32_t steeringAngleE1,
		const int32_t* wheelSpeedE3, int32_t wheelSpeedEntries);
	void reportPassengerData(bool passengerPresent);
	void reportDoorData(bool hasHoodOpen, bool hoodOpen,
		bool hasTrunkOpen, bool trunkOpen, const bool* doorOpen, int32_t doorOpenEntries);
	void reportLightData(bool hasHeadLightState, HeadLightState headLightState,
		bool hasTurnIndicatorState, TurnIndicatorState turnIndicatorState,
		bool hasHazardLightsOn, bool hazardLightsOn);
	void reportTirePressureData(const int32_t* tirePressuresE2, int32_t numEntries);
	void reportAccelerometerData(bool hasAccelerationX, int32_t xAccelerationE3,
		bool hasAccelerationY, int32_t yAccelerationE3, bool hasAccelerationZ,
		int32_t zAccelerationE3);
	void reportGyroscopeData(bool hasRotationSpeedX, int32_t xRotationSpeedE3,
		bool hasRotationSpeedY, int32_t yRotationSpeedE3, bool hasRotationSpeedZ,
		int32_t zRotationSpeedE3);
	void reportGpsSatelliteData(int32_t numberInUse, bool hasNumberInView, int32_t numberInView,
		const int32_t* prns, const int32_t* snrsE3, const bool* usedInFix,
		const int32_t* azimuthsE3, const int32_t* elevationsE3);
private:
	CAndroidAutoAPIIMPL * pImpl;
};

}

#endif //ANDROID_AUTO_API_H
