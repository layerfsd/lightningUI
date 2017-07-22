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
// phoebook_call_log_db_enum.h : Declaration of the CPhonebookCallLogDBEnum

#ifndef __PHONEBOOK_CALL_LOG_DB_ENUM_H_
#define __PHONEBOOK_CALL_LOG_DB_ENUM_H_

#include "phonebook_common.h"
#include "phonebook_db.h"
#include "phonebook_call_record.h"
#include "phonebook_error_codes.h"


class CPhonebookCallLogDB;
class CPhonebookCallRecord;

/////////////////////////////////////////////////////////////////////////////
// CPhonebookCallLogDBEnum
class CPhonebookCallLogDBEnum
{
public:
    // Constructor
    CPhonebookCallLogDBEnum();
    ~CPhonebookCallLogDBEnum();

public:
    // IPhonebookCallLogDBEnum
    STDMETHOD (Next)(unsigned long celt, 
                    CPhonebookCallRecord** rgCallRecord, 
                    unsigned long FAR* pceltFetched);

    STDMETHOD (Skip)(unsigned long celt);
    STDMETHOD (Reset)();

public:
    // Public non-interface methods
    HRESULT Init(CPhonebookDB *pDB, CPhonebookCallLogDB *pCallLogDB);

private:
    CPhonebookDB            *m_pDB;
    CPhonebookCallLogDB     *m_pCallLogDB;
    BOOL                    m_fInit;
};

#endif //__PHONEBOOK_CALL_LOG_DB_ENUM_H_
