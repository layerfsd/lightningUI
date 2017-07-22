/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company iAP Application

 Copyright (c) 2012  Cambridge Silicon Radio Limited, a CSR plc group
 company.

 All rights reserved.

 This Software is protected by United Kingdom copyright laws and international
 treaties.  You may not reverse engineer, decompile or disassemble this
 Software.

 WARNING:
 This Software contains Cambridge Silicon Radio Limited's confidential and
 proprietary information. UNAUTHORIZED COPYING, USE, MODIFICATION,
 DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL, REPACKAGING, REASSEMBLING
 OR DISCLOSURE OF THE WHOLE OR ANY PART OF THE SOFTWARE IS PROHIBITED AND MAY
 RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this Software without
 Cambridge Silicon Radio Limited's express written permission.   Use of any
 portion of the contents of this Software is subject to and restricted by your
 signed written agreement with Cambridge Silicon Radio Limited.
******************************************************************************/
#include "iOSCommunication.h"

using namespace iAP;

class NullOSCommunication :
	public iOSCommunication
{
public:

	NullOSCommunication(void)
	{
	}

	virtual ~NullOSCommunication(void)
	{
	}
	/**
	Accessory transfer data to iPod
	*/
	virtual void sendData(UINT8* pcDatabuffer, UINT32 buffersize){};
	/**
	Register listener for notification returned by iPod
	@param listener listener to be used by passing return value.
	*/
	virtual void registerListener( iOSCommunicationListener* listener){};

};
