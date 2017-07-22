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
#include "iAuthentication.h"

class NullAuthentication :
	public iAuthentication
{
public:

	NullAuthentication(void)
	{
	}

	virtual ~NullAuthentication(void)
	{
	}
	/**
	Start authentication process with the Apple device. After it is called, client should wait for AuthenticationListener::OnAuthenticationFinished() is called, or timeout.
	*/
	virtual void startAuthentication(){};
	/**
	Register AuthenticationListener implementation by client.
	*/
	virtual void registerListener( AuthenticationListener* listener){};
	/**
	Set Accessory info to iPod device, should to be called before startAuthentication
	@param accessoryName Accessory name, in UTF8 format, 64bytes at most
	@param accFirmwareVer Accessory firmware version
	@param accHardwareVer Accessory hardware version
	@param accManufacturer Accessory manufacturer name, in UTF8 format, 64bytes at most
	@param accModelNum Accessory model number, in UTF8 format, 64bytes at most
	*/
	virtual void setAccessoryInfo(std::string& accessoryName, AccessoryVer& accFirmwareVer, AccessoryVer& accHardwareVer, std::string& accManufacturer, std::string& accModelNum){};

};
