/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company PrimaII BSP/CSP

 Copyright (c) 2010 - 2011  Cambridge Silicon Radio Limited, a CSR plc group
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

#include "../stdafx.h"
#include "IapRemoteControl.h"
#include "../GUI/Common/message_map.h"

IapRemoteControlListener::IapRemoteControlListener(HWND hNotifyWnd,LPARAM lparamUser) : 
m_hNotifyWnd(hNotifyWnd),
m_lparamUser(lparamUser)
{

}

IapRemoteControlListener::~IapRemoteControlListener()
{

}

void IapRemoteControlListener::onNotifyAccessoryRegisterHID(BOOL succeed, UINT8 index)
{
	RETAILMSG(1,(_T("###<%s> succeed=%d, index=%d\r\n"),_T(__FUNCTION__),succeed,index));

}
	
void IapRemoteControlListener::onNotifyAccessoryUnregisterHID(BOOL succeed)
{
	RETAILMSG(1,(_T("###<%s> succeed=%d\r\n"),_T(__FUNCTION__),succeed));

}
	
void IapRemoteControlListener::onNotifyiPodHIDReport(HIDReport* report)
{
	RETAILMSG(1,(_T("###<%s> report=%d\r\n"),_T(__FUNCTION__),report));

}
	
void IapRemoteControlListener::onNotifyVoiceOverparameter(UINT8 volume, UINT8 speakingrate)
{
	RETAILMSG(1,(_T("###<%s> volume=%d,speakingrate=%d\r\n"),_T(__FUNCTION__),volume,speakingrate));

}
	
void IapRemoteControlListener::onNotifyVoiceOverItemProperty(UINT8 cmd)
{
	RETAILMSG(1,(_T("###<%s> cmd=%d\r\n"),_T(__FUNCTION__),cmd));

}
	
void IapRemoteControlListener::onNotifyVoiceOverStatus(BOOL enable)
{
	RETAILMSG(1,(_T("###<%s> enable=%d\r\n"),_T(__FUNCTION__),enable));

}