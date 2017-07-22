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
#include "IapBrowserListener.h"
#include "../GUI/Common/message_map.h"

IapBrowserListener::IapBrowserListener(HWND hNotifyWnd,LPARAM lparamUser) : 
m_hNotifyWnd(hNotifyWnd),
m_lparamUser(lparamUser),
record_number_read(0),
record_number_total(0)
{
	LOG(LOG_FUNC,__FUNCTION__);
}

IapBrowserListener::~IapBrowserListener()
{
	LOG(LOG_FUNC,__FUNCTION__);
}

void IapBrowserListener::onRecordReady(UINT32 index, std::string record)
{
	LOG(LOG_FUNC,"IapBrowserListener::onRecordReady (index = %d)",index);
	record_number_read += 1;
	//mIapPlayer->notifyRecordReady(index, QString::fromUtf8(record.c_str()));
	ItemInfoRecord item={index,record};
	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_RECORDREADY_RECORD,LPARAM(&item));

	if (record_number_read == record_number_total)
	{
		//mIapPlayer->sendSignal(ReadFinished, 0, 0, "");
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_READFINISH,NULL);
	}
}

void IapBrowserListener::onRecordReady(UINT32 index, iAP::RootRecordID recordId)
{
	LOG(LOG_FUNC,"IapBrowserListener::onRecordReady RootRecordID (index = %d,%d)",index,recordId);
	record_number_read += 1;
	//mIapPlayer->notifyRecordReady(index, recordId);
	ItemInfoRoot item={index,recordId};
	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_RECORDREADY_ROOT,LPARAM(&item));

	if (record_number_read == record_number_total)
	{
		//mIapPlayer->sendSignal(ReadFinished, 0, 0, "");
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_READFINISH,NULL);
	}
}

void IapBrowserListener::onRecordReady(UINT32 index, iAP::TopLevelRecordID recordId)
{
	LOG(LOG_FUNC,"IapBrowserListener::onRecordReady TopLevelRecordID (index = %d,%d)",index,recordId);
	record_number_read += 1;
	//mIapPlayer->notifyRecordReady(index, recordId);
	ItemInfoTopLevel item={index,recordId};
	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_RECORDREADY_TOPLEVEL,LPARAM(&item));

	if (record_number_read == record_number_total)
	{
		//mIapPlayer->sendSignal(ReadFinished, 0, 0, "");
		::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_READFINISH,NULL);
	}
}

void IapBrowserListener::onOpenFinished(UINT32 numberofRecords)
{
	LOG(LOG_FUNC,"IapBrowserListener::onOpenFinished(numberofRecords = %d)",numberofRecords);
	record_number_read = 0;
	record_number_total = numberofRecords;
	//mIapPlayer->sendSignal(OpenFinished, numberofRecords, 0, "");

// 	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_OPENFINISH,numberofRecords);
	::PostMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_OPENFINISH,numberofRecords);

}

void IapBrowserListener::onExtendedUIModeReady(BOOL isExtendedUIModeAvailable)
{
	LOG(LOG_FUNC,"%s %d",__FUNCTION__,isExtendedUIModeAvailable);
	//mIapPlayer->sendSignal(ExtendedUIModeReady, 0, isExtendedUIModeAvailable, "");

// 	::SendMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_EXTENDEDUIMODEREADY,isExtendedUIModeAvailable);
	::PostMessage(m_hNotifyWnd,WM_IPOD_MESSAGE,IPOD_EXTENDEDUIMODEREADY,isExtendedUIModeAvailable);

}

void IapBrowserListener::onApplicationLaunched(BOOL isApplicationLaunched)
{
	RETAILMSG(1,(_T("###<%s> isApplicationLaunched=%d\r\n"),_T(__FUNCTION__),isApplicationLaunched));

}