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
// poom.h : Declaration of the Poom_t which is used for phonebook contact.

#pragma once

#ifndef __POOM_H__
#define __POOM_H__

#include <vector>
#include <pimstore.h>
#include "phonebook_common.h"
#include "phonebook_contact.h"

using std::vector;

#undef CHK
#define CHK(hr)         { if (FAILED(hr)) break; }


typedef vector<Contact*>  ContactSet;


class Poom_t 
{
public:
    Poom_t();
    Poom_t(
        IPOutlookApp*  cpOutlookApp
        );
    ~Poom_t();

    HRESULT
    Initialize(
        void
        ); 

    void
    Uninitialize(
        void
        );

    HRESULT
    AddContact(
        ContactInfo* pContactInfo
    );

    HRESULT
    AddContact(
        Contact* pContact,
        BOOL& isNewContact,
        BOOL alwaysAdd
    );

    HRESULT
    RemoveContact(
        LPTSTR key
    );

    HRESULT
    RemoveAllContacts(
    );

    HRESULT
    UpdateContact(
        Contact* pContact
    );

    Contact* 
    GetContact(
        LPTSTR key
    );

    HRESULT 
    GetContact(
        Contact** &ppContact,
        DWORD &totalCount
    );

    HRESULT 
    EnumContact(
        Contact** &ppContact,
        DWORD &totalCount
    );

    HRESULT
    SearchName(
        LPTSTR key,
        Contact** &contactList,
        DWORD &contactCount
    );

    HRESULT
    SearchTelNum(
        LPTSTR key,
        Contact** &contactList,
        DWORD &contactCount
    );

    BOOL Flush();

private:
    BOOL GetVolGuid(CEGUID& ceguid);
    BOOL FlushVol();

    HRESULT
    GetContactInformation(
        BSTR                         key,
        LPTSTR                       searchTemplates[],
        DWORD                        searchTemplatesCount,
        __deref_out_opt IContact**   ppContactInfo
        );

    HRESULT
    GetContact(
        LPTSTR key,
        IContact** ppContactInfo
    );

    HRESULT
    GetContact(
        IContact* pContactInfo,
        Contact** ppContact
    );

    HRESULT
    GetContact(
        LPTSTR key,
        IContact** ppContactInfo,
        Contact** ppContact
    );

    HRESULT
    SaveContact(
        IContact* pContact,
        ContactInfo* pContactInfo
    );

    HRESULT
    SearchContact(
        LPTSTR key,
        Contact** &contactList,
        DWORD &contactCount
    );

    HRESULT
    SearchContactDatabase(
        LPTSTR searchTemplate,
        Contact** &contactList,
        DWORD &contactCount
    );

    HRESULT 
    EnumContact(
        IPOutlookItemCollection* pItems,
        Contact** &ppContact,
        DWORD &totalCount
    );

    BOOL IsContactSetEmpty();
    void AddContactSet(Contact** contactList, DWORD contactCount);
    /* "TRUE" indicating that the contact can be added into database, "FALSE" for updating the same contact. */
    BOOL AddContactSet(Contact* pContact);
    void RemoveContactSet(LPTSTR key);
    void RemoveContactSet();
    void SearchContactSet(LPTSTR key, ContactSet& contactSet);
    BOOL ExistInContactSet(Contact* pContact);

    static bool CompareContact(Contact* pContact, Contact* pContact2);
    static void SortContactSet(ContactSet& contactSet);
    static void DumpContactSet(ContactSet& contactSet);

    static BOOL ValidStr(BSTR str);

private:
    static LPTSTR                       sc_SearchPhoneNumberTemplates[];
    static LPTSTR                       sc_SearchNameTemplates[];
    
    CComPtr<IPOutlookApp>               m_cpOutlookApp;

    CEGUID                              m_ceguid;

    /* Contact collection in which every contact only has the field of key. This is mainly for contact search. */
    ContactSet                         m_contactSet;
}; 

#endif /* __POOM_H__ */



