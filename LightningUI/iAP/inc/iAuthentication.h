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
#include   <string> 
using   namespace   std; 
namespace iAP{

class AuthenticationListener;

typedef struct _AccessoryVer{
	UINT8 accMajorVerNum;
	UINT8 accMinorVerNum;
	UINT8 accRevisionVerNum;
}AccessoryVer;


class iAuthentication
{
public:
	iAuthentication(void);
	virtual ~iAuthentication(void);
	/**
	Start authentication process with the Apple device. After it is called, client should wait for AuthenticationListener::OnAuthenticationFinished() is called, or timeout.
	*/
	virtual void startAuthentication()=0;
	/**
	Register AuthenticationListener implementation by client.
	*/
	virtual void registerListener( AuthenticationListener* listener)=0;
	/**
	Set Accessory info to iPod device, should to be called before startAuthentication
	@param accessoryName Accessory name, in UTF8 format, 64bytes at most
	@param accFirmwareVer Accessory firmware version
	@param accHardwareVer Accessory hardware version
	@param accManufacturer Accessory manufacturer name, in UTF8 format, 64bytes at most
	@param accModelNum Accessory model number, in UTF8 format, 64bytes at most
	*/
	virtual void setAccessoryInfo(std::string& accessoryName, AccessoryVer& accFirmwareVer, AccessoryVer& accHardwareVer, std::string& accManufacturer, std::string& accModelNum)=0;
};

class AuthenticationListener{
public:
	AuthenticationListener(void){};
	virtual ~AuthenticationListener(void){};
	/**
	Notify the result of authentication process. If succeeds, client can continue to work with other services to operate the Apple device. If fails, client should retry or notify the user.
	@param status TRUE means authentication success, FALSE means failure.
	*/
	virtual void OnAuthenticationFinished(BOOL status)=0;
};

}//namespace iAP