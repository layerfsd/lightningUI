/****************************************************************************

                CSR plc 2012

                All rights reserved

FILE:           phonebook_common.h

DESCRIPTION:    Phonebook common definition

REVISION:       $Revision: #1 $

****************************************************************************/

#pragma once

#ifndef __PHONEBOOK_COMMON__
#define __PHONEBOOK_COMMON__

#include <windows.h>


#ifdef DEBUG_PHONEBOOK
#define ZONE_PB_INIT            1
#define ZONE_PB_INFO            1
#define ZONE_PB_ERROR           1
#define ZONE_PB_WARN            1
#define ZONE_PB_DUMP            1
#else
#define ZONE_PB_INIT            0
#define ZONE_PB_INFO            0
#define ZONE_PB_ERROR           0
#define ZONE_PB_WARN            0
#define ZONE_PB_DUMP            0
#endif


/* Default volume name which contain contact database in WinCE. */
#define CE_DATABASE_DEFAULT_VOLUME_NAME     TEXT("default.vol")

/* Code page for simplified Chinese GBK. */
#define CP_SIMPLIFIED_CHINESE_GBK           936

/* Chinese charater defined in Unicode. */
#define IS_CHINESE_CHAR(wc)                 ((wc) >= 0x4E00 && (wc) <= 0x9FA5)

/* FirstName | MiddleName | LastName */
#define PHONEBOOK_CONTACT_NAME_FORMAT       TEXT("%s%s%s")

/* Addr | City | State | Zip | Country */
#define PHONEBOOK_CONTACT_ADDRESS_FORMAT    TEXT("%s %s %s %s %s")

/* E.g "LastName MiddleName FirstName", or "LastName,MiddleName,FirstName". */
#define PHONEBOOK_FORMATTED_NAME_DELIMITER  TEXT(" ,")

#define CALL_NUMBER_TYPE_WORK_TEL           TEXT("w")
#define CALL_NUMBER_TYPE_MOBILE_TEL         TEXT("m")
#define CALL_NUMBER_TYPE_HOME_TEL           TEXT("h")
#define CALL_NUMBER_TYPE_PAGER              TEXT("pgr")
#define CALL_NUMBER_TYPE_CAR_TEL            TEXT("car")
#define CALL_NUMBER_TYPE_WORK2_TEL          TEXT("w2")
#define CALL_NUMBER_TYPE_HOME2_TEL          TEXT("h2")
#define CALL_NUMBER_TYPE_RADIO_TEL          TEXT("rdo")
#define CALL_NUMBER_TYPE_ASSISTANT_TEL      TEXT("ast")


/* Phonebook call type. */
typedef enum
{
    e_ctInvalid = 0,
    e_ctIncoming,
    e_ctOutgoing, 
    e_ctMissed,
} PhonebookCallType;

/* Phonebook call record. */
typedef struct
{
    PhonebookCallType   callType;
    LPTSTR              number;
    LPTSTR              name;
    LPTSTR              numberType;
    SYSTEMTIME          startTime;
    SYSTEMTIME          endTime;
} CallRecord;

/* Phonebook contact information. */
typedef struct
{
    LPTSTR              suffix;
    LPTSTR              firstName;

    LPTSTR              workTel;
    LPTSTR              homeTel;
    LPTSTR              lastName;
    LPTSTR              company;
    LPTSTR              jobTitle;
    LPTSTR              department;
    LPTSTR              officeLoc;
    LPTSTR              mobileTel;
    LPTSTR              radioTel;
    LPTSTR              carTel;

    LPTSTR              workFax;
    LPTSTR              homeFax;
    LPTSTR              home2Tel;

    SYSTEMTIME*         birthday;
    LPTSTR              assistant;
    SYSTEMTIME*         anniversary;
    LPTSTR              assistantTel;

    LPTSTR              children;
    LPTSTR              work2Tel;
    LPTSTR              homePage;
    LPTSTR              pager;
    LPTSTR              spouse;

    LPTSTR              name;       /* firstName, lastName */
    LPTSTR              title;
    LPTSTR              middle;
    LPTSTR              homeAddr;
    LPTSTR              homeCity;
    LPTSTR              homeState;
    LPTSTR              homeZip;
    LPTSTR              homeCountry;
    LPTSTR              workAddr;
    LPTSTR              workCity;
    LPTSTR              workState;
    LPTSTR              workZip;
    LPTSTR              workCountry;
    LPTSTR              otherAddr;
    LPTSTR              otherCity;
    LPTSTR              otherState;
    LPTSTR              otherZip;
    LPTSTR              otherCountry;
    LPTSTR              email;
    LPTSTR              email2;
    LPTSTR              email3;

    LPTSTR              notes;
} ContactInfo;

#endif /* __PHONEBOOK_COMMON__ */
