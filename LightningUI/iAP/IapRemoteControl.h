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

#include "iRemoteControl.h"

using namespace iAP;

class IapRemoteControlListener : public RemoteControlListener
{
	public:
		IapRemoteControlListener(HWND hNotifyWnd,LPARAM lparamUser);
		~IapRemoteControlListener();

		/**
		Notify iPod HID report
		*/
		virtual void onNotifyAccessoryRegisterHID(BOOL succeed, UINT8 index);
		/**
		Notify iPod HID report
		*/
		virtual void onNotifyAccessoryUnregisterHID(BOOL succeed);
		/**
		Notify iPod HID report
		*/
		virtual void onNotifyiPodHIDReport(HIDReport* report);
		/**
		Notify iPod Voice over parameter
		*/
		virtual void onNotifyVoiceOverparameter(UINT8 volume, UINT8 speakingrate);
		/**
		Notify iPod Voice over item property, reserved
		*/
		virtual void onNotifyVoiceOverItemProperty(UINT8 cmd);
		/**
		Notify iPod Voice over feature switch status
		*/
		virtual void onNotifyVoiceOverStatus(BOOL enable);

private:
	HWND m_hNotifyWnd;
	LPARAM m_lparamUser;

};

