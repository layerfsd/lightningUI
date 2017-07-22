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

#ifndef IAPAUTHENTICATIONLISTENER_H
#define IAPAUTHENTICATIONLISTENER_H

#include "iAuthentication.h"
#include "Utils.h"

using namespace iAP;

class IapAuthenticationListener : public AuthenticationListener
{
public:
	IapAuthenticationListener(HWND hNotifyWnd,LPARAM lparamUser);
	~IapAuthenticationListener();
	virtual void OnAuthenticationFinished(BOOL status);

private:
	HWND  m_hNotifyWnd;
	LPARAM m_lparamUser;
};


#endif