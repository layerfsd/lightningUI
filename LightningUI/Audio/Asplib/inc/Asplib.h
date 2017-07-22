#ifndef	__ASPLIB_H__
#define	__ASPLIB_H__

#include <windows.h>
#include "AspType.h"

#ifdef	__cplusplus
extern "C" {
#endif

HASP AspInit(ASP_DESC *param);
void AspDeinit(HASP hAsp);
BOOL AspChangeMode(HASP hAsp,AspMode mode);

#ifdef	__cplusplus
}
#endif

#endif