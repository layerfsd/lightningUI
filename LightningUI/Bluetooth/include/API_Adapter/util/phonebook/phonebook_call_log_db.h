//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
// phonebook_call_log_db.h : Declaration of the CPhonebookCallLogDB

#ifndef __PHONEBOOK_CALL_LOG_DB_H
#define __PHONEBOOK_CALL_LOG_DB_H

#include "phonebook_common.h"
#include "phonebook_db.h"
#include "phonebook_call_record.h"
#include "phonebook_call_log_db_enum.h"
#include "phonebook_error_codes.h"


class CPhonebookCallLogDBEnum;
class CPhonebookCallRecord;

//First and last are for validating the range or acceptable 
//value for a voipcalltype
const PhonebookCallType CT_FIRST = e_ctInvalid;
const PhonebookCallType CT_LAST  = e_ctMissed;

#define CALL_LOG_IN_NAME            TEXT("Phonebook Incoming Call Log DB")
#define CALL_LOG_OUT_NAME           TEXT("Phonebook Outgoing Call Log DB")
#define CALL_LOG_MISSED_NAME        TEXT("Phonebook Missed Call Log DB")
#define CALL_LOG_DB_NUMBER_PROPS    7

const PHONEBOOK_PROPID  CALL_LOG_CALL_TYPE      = MAKELONG(CEVT_I4      , 0x0001);
const PHONEBOOK_PROPID  CALL_LOG_NUMBER         = MAKELONG(CEVT_LPWSTR  , 0x0002);
const PHONEBOOK_PROPID  CALL_LOG_NAME           = MAKELONG(CEVT_LPWSTR  , 0x0003);
const PHONEBOOK_PROPID  CALL_LOG_NUMBER_TYPE    = MAKELONG(CEVT_LPWSTR  , 0x0004);
const PHONEBOOK_PROPID  CALL_LOG_START_TIME     = MAKELONG(CEVT_FILETIME, 0x0005);
const PHONEBOOK_PROPID  CALL_LOG_END_TIME       = MAKELONG(CEVT_FILETIME, 0x0006);
const PHONEBOOK_PROPID  PHONEBOOK_TIME_STAMP    = MAKELONG(CEVT_FILETIME, 0x0007);


/////////////////////////////////////////////////////////////////////////////
// CPhonebookCallLogDB
class CPhonebookCallLogDB
{
public:
    CPhonebookCallLogDB();
    ~CPhonebookCallLogDB();

    STDMETHOD (Init)(LPTSTR szURI);
    STDMETHOD (CreateRecord)(PhonebookCallType ct, CPhonebookCallRecord **ppRecord);
    STDMETHOD (put_Capacity)(int cRecords);
    STDMETHOD (get_Capacity)(int *cRecords);

    HRESULT GetEnumerator(CPhonebookCallLogDBEnum **ppEnum);
    HRESULT FillRecord(CPhonebookCallRecord *pRecord, CEPROPVAL *pvals, int cNumProps);
    HRESULT InternalCreateRecord(PhonebookCallType ct, CPhonebookCallRecord **ppRecord);

    BOOL Flush();

private:
    CPhonebookDB*   m_pCallLogDB;
    int             m_Capacity;
    BOOL            m_fInit;
};


#endif //__PHONEBOOK_CALL_LOG_DB_H
