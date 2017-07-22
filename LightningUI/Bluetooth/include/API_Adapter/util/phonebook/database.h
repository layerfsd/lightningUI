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
#pragma once

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <windows.h>
#include "phonebook_common.h"
#include "phonebook_call_log_db.h"


class Database_t 
{
public:
    Database_t(); 
    ~Database_t(); 

    BOOL Init();
    void Deinit();

    void EnumReset();

    HRESULT WriteCallLog(CallRecord* pCallRecord);
    HRESULT WriteCallLog(PhonebookCallType callType, LPTSTR number, LPTSTR name, LPTSTR numberType, SYSTEMTIME& startTime, SYSTEMTIME& endTime);

    HRESULT ReadCallLog(CallRecord* pCallRecord); 
    HRESULT ReadCallLog(PhonebookCallType* callType, LPTSTR* number, LPTSTR* name, LPTSTR* numberType, SYSTEMTIME* startTime, SYSTEMTIME* endTime, SYSTEMTIME* duration = NULL);

    HRESULT RemoveCallLog(SYSTEMTIME* tartTime);
    HRESULT RemoveCallLog(FILETIME* startTime);
    HRESULT RemoveAllCallLog();

    void FreeString(LPTSTR str);

    BOOL Flush();

private:
    void GetCallLogDatabaseName(TCHAR* szName, size_t cch); 

    CPhonebookCallLogDB*        m_pCallLogDB;
    CPhonebookCallLogDBEnum*    m_pCallLogDBEnum;
    int                         m_callLogDBCapacity;
}; 

#endif /* __DATABASE_H__ */



