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
#pragma once

#include "../../stdafx.h"
#include "string"
#include "vector"
//#include "utils.h"

namespace iAP{

class iOSCommunicationListener;

class iOSCommunication{
public:
	iOSCommunication();
	virtual ~iOSCommunication();

	/**
	Accessory transfer data to iPod
	*/
	virtual void sendData(UINT8* pcDatabuffer, UINT32 buffersize)=0;
	/**
	Register listener for notification returned by iPod
	@param listener listener to be used by passing return value.
	*/
	virtual void registerListener( iOSCommunicationListener* listener)=0;
};

class iOSCommunicationListener{
public:
	iOSCommunicationListener(){};
	virtual ~iOSCommunicationListener(){};

	/**
	Notify iPod open a data transfer session
	*/
	virtual void onNotifyOpenDataSession(UINT16 sessionID)=0;
	/**
	Notify iPod close a data transfer session
	*/
	virtual void onNotifyCloseDataSession(UINT16 sessionID)=0;
	/**
	Notify iPod transferred a data
	*/
	virtual void onNotifyiPodDataTransfer(UINT8* iPodData, UINT16 sessionID)=0;
	/**
	Notify iPod notify
	*/
	virtual void onNotifyiPodNotify(UINT16 sessionID)=0;
	/**
	Notify for require the number of EAProtocol supported by application
	*/
	virtual void onNotifyRetEAProtocolNum(UINT8* num)=0;
	/**
	Notify for require the string for the index EAProtocol
	*/
	virtual void onNotifyRetEAProtocolTokensString(UINT8 indexProtocol, std::string& eaProtocolTokenString)=0;
	/**
	Notify for require the string for the index 
	*/
	virtual BOOL onNotifyRetBundleSeedIDTokenString( std::string& bundleSeedIDTokenString)=0;
};

}