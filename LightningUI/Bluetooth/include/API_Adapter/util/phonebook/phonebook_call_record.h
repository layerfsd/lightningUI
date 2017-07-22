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
// PhonebookCallRecord.h : Declaration of the CPhonebookCallRecord

#ifndef __PHONEBOOK_CALL_RECORD_H_
#define __PHONEBOOK_CALL_RECORD_H_

#include "phonebook_common.h"
#include "phonebook_error_codes.h"


#define MAX_TCHAR                   512

#define RECORD_ID_CALL_TYPE         0x0
#define RECORD_ID_NUMBER            0x1
#define RECORD_ID_NAME              0x2
#define RECORD_ID_NUMBER_TYPE       0x3
#define RECORD_ID_START_TIME        0x4
#define RECORD_ID_END_TIME          0x5
#define RECORD_ID_TIME_STAMP        0x6


/////////////////////////////////////////////////////////////////////////////
// CPhonebookCallRecord
class CPhonebookCallRecord
{
public:
    // Construction/destruction
    CPhonebookCallRecord();
    virtual ~CPhonebookCallRecord();

public:
    //Get/set call type
    STDMETHOD (get_CallType)(PhonebookCallType *pctType);
    STDMETHOD (put_CallType)(PhonebookCallType ctType);

    //Calculate the duration
    STDMETHOD (get_Duration)(SYSTEMTIME *pdate);

    //Get/set call start/end time
    STDMETHOD (get_EndTime)(SYSTEMTIME *pdate);
    STDMETHOD (put_EndTime)(SYSTEMTIME date);
    STDMETHOD (get_StartTime)(SYSTEMTIME *pdate);
    STDMETHOD (put_StartTime)(SYSTEMTIME date);

    //Get/set number/name/number type
    STDMETHOD (get_Number)(LPTSTR* number);
    STDMETHOD (put_Number)(LPTSTR number);

    //Get/set name
    STDMETHOD (get_Name)(LPTSTR* name);
    STDMETHOD (put_Name)(LPTSTR name);

    //Get/set number type
    STDMETHOD (get_NumberType)(LPTSTR* numberType);
    STDMETHOD (put_NumberType)(LPTSTR numberType);

    //Write/Remove call record from/into DB
    STDMETHOD (Commit)();
    STDMETHOD (DeleteFromDB)();


    HRESULT put_TimeStamp(FILETIME& ft);

public:
    VOID        SetDB(CPhonebookDB   *pdb)  { m_pDBOwner = pdb; }
    VOID        SetOID(CEOID    ceoid) { m_ceoid = ceoid; }

    static LPTSTR   DuplicateString(LPTSTR src, LPTSTR* dst);
    static void     FreeString(LPTSTR str);

private:
    //The internal DB we belong to
    CPhonebookDB        *m_pDBOwner;

    //Our OID in the DB (if we have been written)
    CEOID               m_ceoid;

    //What is our call type
    PhonebookCallType   m_ctType;

    //Contact name
    LPTSTR              m_szName;

    //Call number
    LPTSTR              m_szNumber;

    //Number type
    LPTSTR              m_szNumberType;

    //Start and end time
    SYSTEMTIME          m_StartTime;
    SYSTEMTIME          m_EndTime;

    FILETIME            m_TimeStamp;
};

#endif //__PHONEBOOK_CALL_RECORD_H_
