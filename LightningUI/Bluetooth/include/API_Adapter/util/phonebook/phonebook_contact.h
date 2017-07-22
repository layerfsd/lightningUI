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
// PhonebookContact.h : Declaration of Contact entry in phonebook.

#ifndef __PHONEBOOK_CONTACT_H_
#define __PHONEBOOK_CONTACT_H_

#include <auto_xxx.hxx>
#include "phonebook_common.h"
#include <atlstr.h>

#define MAX_CONTACT_STR_LEN     1022


/////////////////////////////////////////////////////////////////////////////
// Contact
class Contact
{
public:
    // Construction/destruction
    Contact();
    Contact(BOOL bDetail);
    Contact(Contact* pContact);
    Contact(Contact* pContact, BOOL bCopyDetailInfo);
    virtual ~Contact();

public:
    void SetContactInfo(ContactInfo* pContactInfo);
    void UpdateContactInfo(ContactInfo* pContactInfo);

    void SetFileName(LPCTSTR fileName);

    // Set key using internal ContactInfo initialized, which is mainly as 'name'.
    BOOL SetKey();
    void SetKey(LPTSTR key);

    void SetName(LPTSTR name);
    void SetMobileTel(LPTSTR mobileTel);
    void SetHomeTel(LPTSTR homeTel);
    void SetWorkTel(LPTSTR workTel);

    void SetHomeAddr(LPTSTR homeAddr);
    void SetWorkAddr(LPTSTR workAddr);

    void SetCompany(LPTSTR company);

    void SetEmail(LPTSTR email);

    void SetUrl(LPTSTR url);

    void SetNotes(LPTSTR notes);

    void SetDetailFlag(BOOL detail = TRUE) { m_bDetail = detail; }

    BOOL GetDetailFlag()    { return m_bDetail; }

    LPTSTR GetFileName()    { return m_fileName; }

    LPTSTR GetKey()         { return m_key; }

    LPTSTR GetName()        { return m_pContactInfo ? m_pContactInfo->name : NULL; }
    LPTSTR GetMobileTel()   { return m_pContactInfo ? m_pContactInfo->mobileTel : NULL; }
    LPTSTR GetHomeTel()     { return m_pContactInfo ? m_pContactInfo->homeTel : NULL; }
    LPTSTR GetWorkTel()     { return m_pContactInfo ? m_pContactInfo->workTel : NULL; }

    LPTSTR GetHomeAddr()    { return m_pContactInfo ? m_pContactInfo->homeAddr : NULL; }
    LPTSTR GetWorkAddr()    { return m_pContactInfo ? m_pContactInfo->workAddr : NULL; }

    LPTSTR GetCompany()     { return m_pContactInfo ? m_pContactInfo->company : NULL; }

    LPTSTR GetEmail()       { return m_pContactInfo ? m_pContactInfo->email : NULL; }

    LPTSTR GetUrl()         { return m_pContactInfo ? m_pContactInfo->homePage : NULL; }

    LPTSTR GetNotes()       { return m_pContactInfo ? m_pContactInfo->notes : NULL; }

    ContactInfo* GetContactInfo()   { return m_pContactInfo; }

    void FreeContactInfo();

    LPTSTR GetTelNum();

    LPSTR GetSortKey()      { return m_sortKey; }

    BOOL SameKey(LPTSTR key);
    BOOL HasKey(LPTSTR key);

    void Dump();

    static LPTSTR GetFullName(TCHAR* fullName, size_t cchName, LPCTSTR firstName, LPCTSTR middleName, LPCTSTR lastName);

    static LPTSTR GetFullAddr(TCHAR* fullAddr, size_t cchAddr, LPCTSTR addr, LPCTSTR city, LPCTSTR state, LPCTSTR zip, LPCTSTR country);

    static BOOL ValidStr(LPCTSTR str);

    static LPTSTR DupStr(LPCTSTR src, size_t& cch);
    static LPTSTR DupStr(LPCTSTR src);
    static void FreeStr(LPCTSTR str);

    static void SetCodePage(UINT codePage)  { m_codePage = codePage; }

    static char* Convert2MultiByteString(wchar_t* unicodeStr, UINT codePage = CP_ACP);
    static void FreeMultiByteString(char* multiByteStr);
    static char* CopyMultiByteString(char* &dst, char* src);

    bool operator==(Contact& otherContact) const;
    bool operator!=(Contact& otherContact) const;
    bool operator>(Contact& otherContact) const;
    bool operator<(Contact& otherContact) const;

private:
    void MergeTel(LPTSTR& tel, LPTSTR& telSrc, ContactInfo* pciDst, ContactInfo* pciSrc);
    BOOL MergeTel(LPTSTR& tel, LPTSTR& telDst, LPTSTR& telSrc);

    BOOL Init(BOOL bDetail = TRUE);
    void Copy(Contact* pContact, BOOL bCopyDetailInfo);

    void SetSortKey(LPTSTR key);
    void SetSortKey(LPSTR sortKey, LPSTR sortKey2);
    void FreeSortKey();

    /* Search contact list according to pinyin. Be noted that the function is only valid for Simplified Chinese GBK.
       Result: "TRUE" for found, "FALSE" for not found. */
    BOOL SearchPinYin(LPSTR pinyin, LPSTR firstLetters, LPTSTR searchKey);

    /* Find out whether "searchKey" is in name, while "TRUE" for found, "FALSE" for not found. */
    BOOL SearchName(LPTSTR name, LPTSTR searchKey);

    BOOL SearchName(CString& strFullName, CString& strFirstLetters, LPTSTR searchStr);

    static ContactInfo* CopyContactInfo(ContactInfo* pDst, ContactInfo* pSrc);
    static void FreeContactInfo(ContactInfo* pContactInfo);

public:
    Contact* m_pNext;

private:
    /* Current ANSI code-page identifier for the system. */
    static UINT m_codePage;

    ContactInfo*  m_pContactInfo;

    /* Key/sort of contact for find/search, which is mainly as 'ContactName'. */
    LPTSTR m_key;

    LPTSTR m_fileName;  

    BOOL m_bDetail;

    /* 1st sort key for contact in the string of char. */
    LPSTR m_sortKey;

    /* 2nd sort key for contact in the string of char. */
    LPSTR m_sortKey2;
};

#endif //__PHONEBOOK_CONTACT_H_
