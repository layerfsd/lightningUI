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
//DB.h
#ifndef _PHONEBOOK_DB_H_
#define _PHONEBOOK_DB_H_

#include <windows.h>
#include <winbase.h>
#include "phonebook_common.h"


#define PHONEBOOK_DB_VOL_NAME   TEXT("phonebook.vol")
// FIXME: Currently mount phonebook database in HIVEREG partition
//#define PHONEBOOK_DB_VOL_NAME   TEXT("\\HIVEREG\\Documents and Settings\\phonebook.vol")
//#define PHONEBOOK_DB_VOL_NAME   TEXT("\\StaticStore\\phonebook.vol")

//Anything being added into the DB will be of type PHONEBOOK_PROPID
typedef long PHONEBOOK_PROPID;

class CPhonebookDB
{
public:
    ~CPhonebookDB();

    //Constructor - must call Init() to use the object
    CPhonebookDB(
        DWORD dwNumProps
        );
    
    //Initializes the member variables and opens the CE DB
    HRESULT Initialize(
        const WCHAR* pName,
        SORTORDERSPECEX* pSort
        );

    //Adds a new record and time stamps
    HRESULT AddNewRecord(
        CEPROPVAL *pVals,
        CEOID*  pOut
        );

    //Deletes a record with this ceoid
    BOOL DeleteRecord(CEOID);

    //Overrides the properties of the record with this ceoid with new props
    BOOL EditRecord(CEOID ceoid, CEPROPVAL *propvalNew, int cProps);

    //Finds a record by cepropval
    CEOID   FindRecord(CEPROPVAL* pVal);

    //Reads the current record that has been seeked to
    BOOL ReadCurrentRecord(CEPROPVAL **ppcpvFill, int *pcSize, CEOID *pceoid);

    //Seeks to cElt'th element from the current seek pointer
    BOOL   SeekToNext(int cElts);

    //Sets the seek pointer back to 0
    BOOL   ResetSeek();

    //Accessor Functions
    int    GetCapacity();
    BOOL   Resize(int cElts);

    //Flush phonebook into storage.
    BOOL   Flush();

private:
    //
    // Private functions
    //
    HRESULT Open(
        SORTORDERSPECEX* pSort
        );

    BOOL RemoveOldest();

    BOOL GetVolumeName(WCHAR* szVolName, DWORD length);

    //
    // Member variables
    //
    
    //Is the DB initialized?
    BOOL            m_Initialized;

    //Handle to this DB
    HANDLE          m_Database;


    //Max Number of entries in the DB
    int             m_MaxNumEntries;

    //DB Name
    WCHAR           m_pName[CEDB_MAXDBASENAMELEN];

    //Number of entries in the DB
    int             m_CurrentNumEntries;

    //GUID of the DB
    CEGUID          m_guidDB;

    //OID of the DB
    CEOID           m_oidDB;

    //Number of properties in a record
    int             m_NumProps;

    //Where we are currently seeked to in the DB
    int             m_CurrentSeek;

    //The underlying mutex handle(lock) used for locking and unlocking the DB
    HANDLE m_Lock;
};

#endif
