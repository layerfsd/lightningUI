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

#ifndef IAPBROWSERLISTENER_H
#define IAPBROWSERLISTENER_H

#include "iBrowser.h"
#include "Utils.h"

using namespace iAP;

struct ItemInfoTopLevel
{
	UINT32 index;
	TopLevelRecordID recordId;
};

struct ItemInfoRoot
{
	UINT32 index;
	RootRecordID recordId;
};

struct ItemInfoRecord
{
	UINT32 index;
	std::string record;
};

class IapBrowserListener : public BrowserListener
{
public:
	IapBrowserListener(HWND hNotifyWnd,LPARAM lparamUser);
	~IapBrowserListener();
	virtual void onRecordReady(UINT32 index, std::string record);
	virtual void onRecordReady(UINT32 index, RootRecordID recordId);
	virtual void onRecordReady(UINT32 index, TopLevelRecordID recordId);
	virtual void onOpenFinished(UINT32 numberofRecords);
	virtual void onExtendedUIModeReady(BOOL isExtendedUIModeAvailable);
	/**
	Notify GUI the application status
	@param isApplicationLaunched TRUE or FALSE
	*/
	virtual void onApplicationLaunched(BOOL isApplicationLaunched);

private:
	HWND m_hNotifyWnd;
	LPARAM m_lparamUser;

	UINT32 record_number_read;
	UINT32 record_number_total;
};

#endif