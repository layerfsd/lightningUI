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
#include "utils.h"

namespace iAP{

	class RemoteControlListener;

	class HIDDescriptor{
	public:
		UINT16 vid;
		UINT16 pid;
		UINT8 countrycode;
		std::vector<UINT8> descriptor;
	};

	class HIDReport{
	public:
		UINT16 lenth;
		std::vector<UINT8> report;
	};

	typedef enum _VoiceOverType{
		ID_VOLUME				= 0x01,
		ID_SPEAKINGRATE			= 0x10
	}VoiceOverType;

	class iRemoteControl{
	public:
		iRemoteControl();
		virtual ~iRemoteControl();

		/**
		init remote control.
		*/
		virtual void init()=0;
		/**
		deinit remote control.
		*/
		virtual void deinit()=0;
		/**
		Accessory register a HID device on iDevice.
		*/
		virtual void AccessoryRegistorHIDdevice(HIDDescriptor* descriptorVec)=0;
		/**
		Accessory route the HID report to iDevice.
		*/
		virtual void AccessorySendHIDReport(UINT8 index, HIDReport* report)=0;
		/**
		Accessory Unregister the HID device.
		*/
		virtual void AccessoryDegisterHID(UINT8 index)=0;
		/**
		Accessory operate Voice over.
		*/
		virtual void AccessorySendVoiceOverOperation()=0;
		/**
		Accessory inquire Voice over parameter.
		*/
		virtual void AccessoryGetVoiceOverParameter(UINT8* volumeValue, UINT8* speakingrateValue)=0;
		/**
		Accessory set Voice over parameter.
		*/
		virtual void AccessorySetVoiceOverParameter(UINT8 mask, UINT8 volume, UINT8 speakingrate)=0;
		/**
		Accessory set Voice over context.
		*/
		virtual void AccessorySetVoiceOverContext()=0;
		/**
		Register listener for notification returned by iPod
		@param listener listener to be used by passing return value.
		*/
		virtual void registerListener( RemoteControlListener* listener)=0;
	};

	class RemoteControlListener{
	public:
		RemoteControlListener(){};
		virtual ~RemoteControlListener(){};

		/**
		Notify iPod HID report
		*/
		virtual void onNotifyAccessoryRegisterHID(BOOL succeed, UINT8 index)=0;
		/**
		Notify iPod HID report
		*/
		virtual void onNotifyAccessoryUnregisterHID(BOOL succeed)=0;
		/**
		Notify iPod HID report
		*/
		virtual void onNotifyiPodHIDReport(HIDReport* report)=0;
		/**
		Notify iPod Voice over parameter
		*/
		virtual void onNotifyVoiceOverparameter(UINT8 volume, UINT8 speakingrate)=0;
		/**
		Notify iPod Voice over item property, reserved
		*/
		virtual void onNotifyVoiceOverItemProperty(UINT8 cmd)=0;
		/**
		Notify iPod Voice over feature switch status
		*/
		virtual void onNotifyVoiceOverStatus(BOOL enable)=0;
	};

}