#include "BluetoothCommon.h"
#include "../../resource.h"

const wchar_t* DialStateToString(DIAL_STATE state)
{
	switch (state)
	{
	case DIAL_STATE_TALKING:
		return GetResourceString(IDS_CS_CONNECTED);
		break;
	case DIAL_STATE_HOLDING:
		return GetResourceString(IDS_CS_HOLD);
		break;
	case DIAL_STATE_CALLOUT:
		return GetResourceString(IDS_CS_DIALING);
		break;
	case DIAL_STATE_INCOMING:
		return GetResourceString(IDS_CS_INCOMING_CALL);
		break;
	case DIAL_STATE_WAITING:
		return GetResourceString(IDS_CS_WAITING_CALL);
		break;
	default:
		return NULL;
		break;
	}
}