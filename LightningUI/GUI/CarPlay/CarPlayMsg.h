#pragma once
//to register and get user message
#define GET_USER_MESSAGE_ID(name) ::RegisterWindowMessage(name)

#define UWM_CARPLAY_EXIT   _T("UWM_CARPLAY_EXIT-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_DISCONNECT _T("UWM_CARPLAY_DISCONNECT-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_CONNECTING _T("UWM_CARPLAY_CONNECTING-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_CONNECTED _T("UWM_CARPLAY_CONNECTED-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_CONNECT_FAIL _T("UWM_CARPLAY_CONNECT_FAIL-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_START _T("UWM_CARPLAY_START-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_STOP _T("UWM_CARPLAY_STOP-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_HAND_CLIENT   _T("UWM_CARPLAY_HAND_CLIENT-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_HAND_MAIN   _T("UWM_CARPLAY_HAND_MAIN-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_MODE_CHANGED   _T("UWM_CARPLAY_MODE_CHANGED-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_CHANGE_MODE   _T("UWM_CARPLAY_CHANGE_MODE-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_ALT_AUDIO_MSG   _T("UWM_CARPLAY_ALT_AUDIO_MSG-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_DUCK_AUDIO   _T("UWM_CARPLAY_DUCK_AUDIO-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_MAIN_AUDIO_MSG   _T("UWM_CARPLAY_MAIN_AUDIO_MSG-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_SET_DRIVE_TYPE   _T("UWM_CARPLAY_SET_DRIVE_TYPE-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_GET_DRIVE_TYPE   _T("UWM_CARPLAY_GET_DRIVE_TYPE-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_BUTTON_KEY   _T("UWM_CARPLAY_BUTTON_KEY-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_PHONE_KEY   _T("UWM_CARPLAY_PHONE_KEY-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_SET_LIMITED_UI   _T("UWM_CARPLAY_SET_LIMITED_UI-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_SCREEN_SHOW   _T("UWM_CARPLAY_SCREEN_SHOW-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_REQUEST_SIRI   _T("UWM_CARPLAY_REQUEST_SIRI-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_REQUEST_UI   _T("UWM_CARPLAY_REQUEST_UI-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_SET_SYSTEM_POWER_STATE   _T("UWM_CARPLAY_SET_SYSTEM_POWER_STATE-{4346438F-0480-3723-VF26-5RF24YM5639X}")
#define UWM_CARPLAY_AUDIO_DUCK   _T("UWM_CARPLAY_AUDIO_DUCK-{4346438F-0480-3723-VF26-5RF24YM5639X}")


const UINT ID_UWM_CARPLAY_EXIT = GET_USER_MESSAGE_ID(UWM_CARPLAY_EXIT);
const UINT ID_UWM_CARPLAY_DISCONNECT = GET_USER_MESSAGE_ID(UWM_CARPLAY_DISCONNECT);
const UINT ID_UWM_CARPLAY_CONNECTING = GET_USER_MESSAGE_ID(UWM_CARPLAY_CONNECTING);
const UINT ID_UWM_CARPLAY_CONNECTED = GET_USER_MESSAGE_ID(UWM_CARPLAY_CONNECTED);
const UINT ID_UWM_CARPLAY_CONNECT_FAIL = GET_USER_MESSAGE_ID(UWM_CARPLAY_CONNECT_FAIL);
const UINT ID_UWM_CARPLAY_START = GET_USER_MESSAGE_ID(UWM_CARPLAY_START);
const UINT ID_UWM_CARPLAY_STOP = GET_USER_MESSAGE_ID(UWM_CARPLAY_STOP);
const UINT ID_UWM_CARPLAY_HAND_CLIENT = GET_USER_MESSAGE_ID(UWM_CARPLAY_HAND_CLIENT);
const UINT ID_UWM_CARPLAY_HAND_MAIN = GET_USER_MESSAGE_ID(UWM_CARPLAY_HAND_MAIN);
const UINT ID_UWM_CARPLAY_MODE_CHANGED = GET_USER_MESSAGE_ID(UWM_CARPLAY_MODE_CHANGED);
const UINT ID_UWM_CARPLAY_CHANGE_MODE = GET_USER_MESSAGE_ID(UWM_CARPLAY_CHANGE_MODE);
const UINT ID_UWM_CARPLAY_ALT_AUDIO_MSG = GET_USER_MESSAGE_ID(UWM_CARPLAY_ALT_AUDIO_MSG);
const UINT ID_UWM_CARPLAY_DUCK_AUDIO = GET_USER_MESSAGE_ID(UWM_CARPLAY_DUCK_AUDIO);
const UINT ID_UWM_CARPLAY_MAIN_AUDIO_MSG = GET_USER_MESSAGE_ID(UWM_CARPLAY_MAIN_AUDIO_MSG);
const UINT ID_UWM_CARPLAY_SET_DRIVE_TYPE = GET_USER_MESSAGE_ID(UWM_CARPLAY_SET_DRIVE_TYPE);
const UINT ID_UWM_CARPLAY_GET_DRIVE_TYPE = GET_USER_MESSAGE_ID(UWM_CARPLAY_GET_DRIVE_TYPE);
const UINT ID_UWM_CARPLAY_BUTTON_KEY = GET_USER_MESSAGE_ID(UWM_CARPLAY_BUTTON_KEY);
const UINT ID_UWM_CARPLAY_PHONE_KEY = GET_USER_MESSAGE_ID(UWM_CARPLAY_PHONE_KEY);
const UINT ID_UWM_CARPLAY_SET_LIMITED_UI = GET_USER_MESSAGE_ID(UWM_CARPLAY_SET_LIMITED_UI);
const UINT ID_UWM_CARPLAY_SCREEN_SHOW = GET_USER_MESSAGE_ID(UWM_CARPLAY_SCREEN_SHOW);
const UINT ID_UWM_CARPLAY_REQUEST_SIRI = GET_USER_MESSAGE_ID(UWM_CARPLAY_REQUEST_SIRI);
const UINT ID_UWM_CARPLAY_REQUEST_UI = GET_USER_MESSAGE_ID(UWM_CARPLAY_REQUEST_UI);
const UINT ID_UWM_CARPLAY_SET_SYSTEM_POWER_STATE = GET_USER_MESSAGE_ID(UWM_CARPLAY_SET_SYSTEM_POWER_STATE);
const UINT ID_UWM_CARPLAY_AUDIO_DUCK = GET_USER_MESSAGE_ID(UWM_CARPLAY_AUDIO_DUCK);

