#pragma once

#include "Api_SearchPair.h"
#include "Api_HF.h"
#include "Api_DUN.h"
#include "Api_SystemEvent.h"
#include "Api_IPC.h"
#include "Api_TPMS.h"
#include "Api_PB.h"
#include "Api_MAP.h"
#include "Api_A2dp.h"
#include "Api_Avrcp.h"
#include "Api_SPP.h"
#include "Api_Opps.h"
#include "Api_HID.h"
#include "Api_DID.h"

class CAPI
{
public:
    CAPI(void);
    ~CAPI(void);

    BOOL Initialize();
    void DeInitialize();
    void PowerDown();
    void PowerUp();

    CApi_SearchPair apiSearchPair;
    CApi_HF apiHF;
    CApi_DUN apiDUN;
    CApi_SystemEvent apiSystemEvent;
    CApi_IPC apiIPC;
#ifdef ENABLE_TPMS
    CApi_TPMS apiTPMS;
#endif
	CApi_PB apiPB;
	CApi_MAP apiMAP;
	CApi_A2dp apiA2dp;
	CApi_Avrcp apiAvrcp;
	CApi_SPP apiSPP;
	CApi_Opps apiOpps;
	CApi_HID apiHID;
	CApi_Did apiDID;
};
