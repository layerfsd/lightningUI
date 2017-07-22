#ifndef HOOK_H
#define HOOK_H
#include "FLC_BLUETOOTH_SDK.h"
#ifdef __cplusplus
extern "C" {
#endif
#ifndef FLC_BLUETOOTH_API
#define FLC_BLUETOOTH_API __declspec(dllexport)
#endif
typedef void (*MouseProc)(int x, int y, Hid_Mouse_Control ctr, bool bIs);
FLC_BLUETOOTH_API bool FLC_InstallMouseHook(MouseProc MouseCallback);
FLC_BLUETOOTH_API bool FLC_RemoveMouseHook();

#ifdef __cplusplus
}
#endif

#endif
