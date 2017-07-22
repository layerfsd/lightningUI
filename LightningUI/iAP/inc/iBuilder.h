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

namespace iAP{

class iAuthentication;
class AuthenticationListener;
class iBrowser;
class BrowserListener;
class iPlayer;
class PlayerListener;
class iOSCommunication;
class iOSCommunicationListener;
class iRemoteControl;
class RemoteControlListener;
class TransportMode;

class iBuilder{
public:
	iBuilder();
	virtual ~iBuilder();
	/**
	Factory method to create instance of iBuilder implementation. 
	*/
	static iBuilder* create();
	/**
	Configure I2C device name and attribute.
	@param portName Device name of I2C. e.g. _T("I2C1:").
	@param readAddr Read address of the device.
	@param writeAddr Write address of the device.
	@param freq I2C clock frequency.
	*/
	virtual void configI2CDevice(LPCWSTR portName, UINT16 readAddr, UINT16 writeAddr, UINT32 freq)=0;
	/**
	Configure USB device name. Don't call if connecting with UART.
	@param portName Device name of USB. e.g. _T("LNG1:").
	*/
	virtual void configUsbDevice(LPCTSTR portName)=0;
	/**
	Configure UART device name. Don't call if connecting with USB.
	@param portName Device name of UART. e.g. _T("COM1:").
	*/
	virtual void configUartDevice(LPCTSTR portName,UINT baudRate,UINT dataBits,UINT stopBits,UINT parity)=0;
	/**
	Construct the iAP middleware. I2C, USB (or UART) device should be configured in advance.
	@param authenticationListener Instance of AuthenticationListener implementation by client.
	@param browsingListener Instance of BrowserListener implementation by client.
	@param playerListener Instance of PlayerListener implementation by client.
	@return Success of construction.
	*/
	virtual BOOL construct(AuthenticationListener* authenticationListener, BrowserListener* browsingListener, PlayerListener* playerListener, iOSCommunicationListener* OSCommunicationer, RemoteControlListener* remoteControlListener)=0;
	/**
	Destruct the middleware. After it is called, client should delete instance of iBuilder in order to complete to destruction process.
	*/
	virtual void destruct()=0;
	/**
	Detect if the device is plugged out.
	*/
	virtual BOOL plugOutDetection()=0;
	/**
	Get the instance of iAuthentication service. It should be called after the middleware is constructed.
	*/
	virtual iAuthentication* getAuthentication()=0;
	/**
	Get the instance of iBrowser service. It should be called after the middleware is constructed.
	*/
	virtual iBrowser* getBrowser()=0;
	/**
	Get the instance of iPlayer service. It should be called after the middleware is constructed.
	*/
	virtual iPlayer* getPlayer()=0;
	/**
	Get the instance of iOSCommunication service. It should be called after the middleware is constructed.
	*/
	virtual iOSCommunication* getiOSCommunication()=0;
	/**
	Get the instance of iRemoteControl service. It should be called after the middleware is constructed.
	*/
	virtual iRemoteControl* getRemoteControl()=0;
};

}//namespace iAP