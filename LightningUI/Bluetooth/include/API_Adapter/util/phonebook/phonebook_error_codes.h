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
#ifndef __PHONEBOOK_ERROR_CODES_H__
#define __PHONEBOOK_ERROR_CODES_H__

// Phonebook error codes
enum
{
//API validation failures:

    // The IPhonebookCallLogDBEnum or IPhonebookCallerInfoDBEnum pointer
    // that was used to make this call is out of date.  Before
    // further calls will succeed, the enumerator must be reset.
    PHONEBOOK_E_ENUMOUTOFDATE        = 0x80cc0000,

    // The interface was not initialized (usually though a call
    // to Init()) before being used.
    PHONEBOOK_E_NOTINITIALIZED       = 0x80cc0001,

    // The IPhonebookCallRecord or IPhonebookCallerInfoRecord pointer is
    // not in any database and therefore is not removable.
    PHONEBOOK_E_RECORDNOTINDB        = 0x80cc0002,

    // The interface was already initialized and cannot be initialized
    // again until it has been uninitialized.
    PHONEBOOK_E_ALREADYINITIALIZED   = 0x80cc0003,

    //There is no caller info DB or no call log DB. The manager will still
    //work, but the DB funtionality is lost.
    PHONEBOOK_E_NODB                 = 0x80cc0004,

    //The time has not been set
    PHONEBOOK_E_TIMENOTSET           = 0x80cc0008,
};

#endif //__PHONEBOOK_ERROR_CODES_H__
