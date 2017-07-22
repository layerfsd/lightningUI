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
#include "IapAuthenticationListener.h"
#include "../GUI/Common/message_map.h"

IapAuthenticationListener::IapAuthenticationListener(HWND hNotifyWnd,LPARAM lparamUser) : 
m_hNotifyWnd(hNotifyWnd),
m_lparamUser(lparamUser)
{
	LOG(LOG_FUNC,__FUNCTION__);
}

IapAuthenticationListener::~IapAuthenticationListener()
{
	LOG(LOG_FUNC,__FUNCTION__);
}

void IapAuthenticationListener::OnAuthenticationFinished(BOOL status)
{
// 	LOG(LOG_FUNC,__FUNCTION__);
	LOG(LOG_FUNC,"%s %d",__FUNCTION__,status);

	//mIapPlayer->sendSignal(AuthenticationFinished, 0, status, "");
	::PostMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_AUTHENTICATION,status);
}