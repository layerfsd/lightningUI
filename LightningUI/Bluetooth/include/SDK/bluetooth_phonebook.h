/******************************************************************************************************

				CSR plc 2014
 	 			
				All rights reserved

FILE:			bluetooth_phonebook.h

DESCRIPTION:	Bluetooth phonebook

REVISION:		$Revision: #1 $
        
*******************************************************************************************************/
#ifndef _BLUETOOTH_PHONEBOOK_H
#define _BLUETOOTH_PHONEBOOK_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNDER_CE
typedef unsigned __int64 BT_ADDR;
#endif

#include "bluetooth_module_api.h"
#include "csr_bt_vcard.h"

#ifdef BLUETOOTH_PHONEBOOK_IMPLEMENTATION
#define BLUETOOTH_PHONEBOOK_API __declspec(dllexport)
#else
#define BLUETOOTH_PHONEBOOK_API __declspec(dllimport)
#endif

#define FILENAME_PHONEBOOK							TEXT("pb.vcf")
#define FILENAME_INCOMING_CALLS_HISTORY				TEXT("ich.vcf")
#define FILENAME_OUTGOING_CALLS_HISTORY				TEXT("och.vcf")
#define FILENAME_MISSED_CALLS_HISTORY				TEXT("mch.vcf")
#define FILENAME_COMBINED_CALLS_HISTORY				TEXT("cch.vcf")

#define TELECOM_PHONEBOOK							TEXT("telecom/pb.vcf")
#define TELECOM_PHONEBOOK_FOLDER					TEXT("telecom/pb/")
#define TELECOM_INCOMING_CALLS_HISTORY				TEXT("telecom/ich.vcf")
#define TELECOM_INCOMING_CALLS_HISTORY_FOLDER		TEXT("telecom/ich/")
#define TELECOM_OUTGOING_CALLS_HISTORY				TEXT("telecom/och.vcf")
#define TELECOM_OUTGOING_CALLS_HISTORY_FOLDER		TEXT("telecom/och/")
#define TELECOM_MISSED_CALLS_HISTORY				TEXT("telecom/mch.vcf")
#define TELECOM_MISSED_CALLS_HISTORY_FOLDER			TEXT("telecom/mch/")
#define TELECOM_COMBINED_CALLS_HISTORY				TEXT("telecom/cch.vcf")
#define TELECOM_COMBINED_CALLS_HISTORY_FOLDER		TEXT("telecom/cch/")

#define SIM_TELECOM_PHONEBOOK						TEXT("SIM1/telecom/pb.vcf")
#define SIM_TELECOM_PHONEBOOK_FOLDER				TEXT("SIM1/telecom/pb/")
#define SIM_TELECOM_INCOMING_CALLS_HISTORY			TEXT("SIM1/telecom/ich.vcf")
#define SIM_TELECOM_INCOMING_CALLS_HISTORY_FOLDER	TEXT("SIM1/telecom/ich/")
#define SIM_TELECOM_OUTGOING_CALLS_HISTORY			TEXT("SIM1/telecom/och.vcf")
#define SIM_TELECOM_OUTGOING_CALLS_HISTORY_FOLDER	TEXT("SIM1/telecom/och/")
#define SIM_TELECOM_MISSED_CALLS_HISTORY			TEXT("SIM1/telecom/mch.vcf")
#define SIM_TELECOM_MISSED_CALLS_HISTORY_FOLDER		TEXT("SIM1/telecom/mch/")
#define SIM_TELECOM_COMBINED_CALLS_HISTORY			TEXT("SIM1/telecom/cch.vcf")
#define SIM_TELECOM_COMBINED_CALLS_HISTORY_FOLDER	TEXT("SIM1/telecom/cch/")

typedef unsigned char* PhonebookDataHandleT;

typedef void (WINAPI* PacmCbCancelCompleteT) (LPVOID userdata, DWORD result);
typedef void (WINAPI* PacmCbPullPhonebookCompleteT) (LPVOID userdata, DWORD result, PhonebookDataHandleT dataHandle, DWORD phonebookSize, DWORD newMissedCall);
typedef void (WINAPI* PacmCbGetVcardListCompleteT) (LPVOID userdata, DWORD result, PhonebookDataHandleT dataHandle, DWORD phonebookSize, DWORD newMissedCall);
typedef void (WINAPI* PacmCbGetVcardEntryCompleteT) (LPVOID userdata, DWORD result, PhonebookDataHandleT dataHandle);

typedef enum
{
	PACM_SOURCE_PHONE			= 0x00,
	PACM_SOURCE_SIM				= 0x01,
} PacmSource;

typedef enum
{
	PACM_FORMAT_VCARD_2_1		= 0x00,
	PACM_FORMAT_VCARD_3_0		= 0x01,
} PacmFormat;

typedef enum
{
	PACM_DATATYPE_PHONEBOOK		= 0x00,
	PACM_DATATYPE_VCARD_LIST	= 0x01,
	PACM_DATATYPE_VCARD_ENTRY	= 0x02,
} PacmDatatype;

typedef enum
{
	PACM_ORDER_INDEXED			= 0x00,
	PACM_ORDER_ALPHABETICAL		= 0x01,
	PACM_ORDER_PHONETICAL    	= 0x02,
} PacmOrder;

typedef enum
{
	PACM_SEARCH_ATTRIBUTE_NAME		= 0x00,
	PACM_SEARCH_ATTRIBUTE_NUMBER	= 0x01,
	PACM_SEARCH_ATTRIBUTE_SOUND		= 0x02,
} PacmSearchAttribute;

#define VCARD_FILTER_GROUP0_VERSION                            (0X01)
#define VCARD_FILTER_GROUP0_FORMATTED_NAME                     (0X02)
#define VCARD_FILTER_GROUP0_STRUCTURED_PRESENTATION_OF_NAME    (0X04)
#define VCARD_FILTER_GROUP0_ASSOCIATED_IMAGE_OR_PHOTO          (0X08)
#define VCARD_FILTER_GROUP0_BIRTHDAY                           (0X10)
#define VCARD_FILTER_GROUP0_DELIVERY_ADDRESS                   (0X20)
#define VCARD_FILTER_GROUP0_DELIVERY                           (0X40)
#define VCARD_FILTER_GROUP0_TELEPHONE_NUMBER                   (0X80)

#define VCARD_FILTER_GROUP1_ELECTORNIC_MAIL_ADDRESS            (0X01)
#define VCARD_FILTER_GROUP1_ELECTRONIC_MAIL                    (0X02)
#define VCARD_FILTER_GROUP1_TIMEZONE                           (0X04)
#define VCARD_FILTER_GROUP1_GEOGRAPHIC_POSITION                (0X08)
#define VCARD_FILTER_GROUP1_JOB_TITLE                          (0X10)
#define VCARD_FILTER_GROUP1_ROLE_WITHIN_THE_ORGANIZATION       (0X20)
#define VCARD_FILTER_GROUP1_ORGANIZATION_LOGO                  (0X40)
#define VCARD_FILTER_GROUP1_VCARD_OF_PERSON_REPRESENTING       (0X80)

#define VCARD_FILTER_GROUP2_NAME_OF_ORGANIZATION               (0X01)
#define VCARD_FILTER_GROUP2_COMMENTS                           (0X02)
#define VCARD_FILTER_GROUP2_REVISION                           (0X04)
#define VCARD_FILTER_GROUP2_PRONUNCATION_OF_NAME               (0X08)
#define VCARD_FILTER_GROUP2_UNIFORM_RESOURCE_LOCATOR           (0X10)
#define VCARD_FILTER_GROUP2_UNIQUE_ID                          (0X20)
#define VCARD_FILTER_GROUP2_PUBLIC_ENCRYPTION_KEY              (0X40)
#define VCARD_FILTER_GROUP2_NICKNAME                           (0X80)

#define VCARD_FILTER_GROUP3_CATEGORIES                         (0X01)
#define VCARD_FILTER_GROUP3_PRODUCT_ID                         (0X02)
#define VCARD_FILTER_GROUP3_CLASS_INFORMATION                  (0X04)
#define VCARD_FILTER_GROUP3_SORT_STRING                        (0X08)
#define VCARD_FILTER_GROUP3_TIME_STAMP                         (0X10)

#define VCARD_FILTER_GROUP4_PROPRITARY_FILTER                  (0x80)

typedef union
{
	unsigned char	rawFilter[8];
	struct
	{
		// First byte
		unsigned char	version:1;
		unsigned char   formattedName:1;
		unsigned char   structuredPresentationOfName:1;
		unsigned char   associatedImageOrPhoto:1;
		unsigned char   birthDay:1;
		unsigned char   deliveryAddress:1;
		unsigned char   delivery:1;
		unsigned char   telephoneNumber:1;
			
		// Second byte
		unsigned char   electornicMailAddress:1;
		unsigned char   electronicMail:1;
		unsigned char   timeZone:1;
		unsigned char   geographicPosition:1;
		unsigned char   jobTitle:1;
		unsigned char   roleWithinTheOrganization:1;
		unsigned char   organizationLogo:1;
		unsigned char   vCardOfPersonRepresenting:1;
			
		// Third byte
		unsigned char   nameOfOrganization:1;
		unsigned char   comments:1;
		unsigned char   revision:1;
		unsigned char   pronuncationOfName:1;
		unsigned char   uniformResourceLocator:1;
		unsigned char   uniqueID:1;
		unsigned char   publicEncryptionKey:1;
		unsigned char   nickname:1;
			
		// Forth byte
		unsigned char   categories:1;
		unsigned char   productID:1;
		unsigned char   classInformation:1;
		unsigned char   sortString:1;
		unsigned char   timeStamp:1;
		unsigned char   reserved1:3;
		
		// Fifth byte
		unsigned char   reserved2:7;
		unsigned char	propritaryFilter:1;
		
		// Sixth byte
		unsigned char	reserved3;
		
		// Seventh byte
		unsigned char	reserved4;

		// Eight byte
		unsigned char	reserved5;
	} fields;
} PacmFilter;

typedef struct
{
	DWORD							size;
	LPVOID							userdata;
	BluetoothStatusChange			bluetoothStatusChangeCb;
	PacmCbPullPhonebookCompleteT	pacmCbPullPhonebookComplete;
	PacmCbGetVcardListCompleteT		pacmCbGetVcardListComplete;
	PacmCbGetVcardEntryCompleteT	pacmCbGetVcardEntryComplete;
	PacmCbCancelCompleteT			pacmCbCancelComplete;
} PacmRegisterInfo;

typedef struct
{
	const TCHAR*	filename;
	const TCHAR*	lastName;
	const TCHAR*	firstName;
	const TCHAR*	middleName;
	const TCHAR*	prefix;
	const TCHAR*	suffix;
} PacmVcardListEntry;

// Transfer functions
DWORD BLUETOOTH_PHONEBOOK_API PacmCancel();
DWORD BLUETOOTH_PHONEBOOK_API PacmRegister(PacmRegisterInfo* pacmRegisterInfo);
DWORD BLUETOOTH_PHONEBOOK_API PacmConnect(BT_ADDR bluetoothAddress);
DWORD BLUETOOTH_PHONEBOOK_API PacmDisconnect();
DWORD BLUETOOTH_PHONEBOOK_API PacmPullPhonebook(BT_ADDR bluetoothAddress, TCHAR* filename, PacmSource source, PacmFormat format, PacmFilter filter, unsigned short maximumListCount, unsigned short listStartOffset);
DWORD BLUETOOTH_PHONEBOOK_API PacmGetVcardList(BT_ADDR bluetoothAddress, TCHAR* filename, PacmOrder order, TCHAR* searchFilter, PacmSearchAttribute searchAttribute, unsigned short maximumListCount, unsigned short listStartOffset);
DWORD BLUETOOTH_PHONEBOOK_API PacmGetVcardEntry(BT_ADDR bluetoothAddress, TCHAR* filename, PacmFormat format, PacmFilter filter);
DWORD BLUETOOTH_PHONEBOOK_API PacmUnregister();

// Data manipulation functions
DWORD BLUETOOTH_PHONEBOOK_API PacmDecodeVcardByHandle(PhonebookDataHandleT phonebookDataHandle, CsrBtVcardData** vcardData);
DWORD BLUETOOTH_PHONEBOOK_API PacmDecodeVcardByBuffer(LPVOID buffer, DWORD bufferSize, PhonebookDataHandleT* phonebookDataHandle, CsrBtVcardData** vcardData);
DWORD BLUETOOTH_PHONEBOOK_API PacmDecodeVcardList(PhonebookDataHandleT phonebookDataHandle, DWORD* vcardListEntries, PacmVcardListEntry** list);
DWORD BLUETOOTH_PHONEBOOK_API PacmGetDataPointer(PhonebookDataHandleT phonebookDataHandle, LPVOID* dataPointer);
DWORD BLUETOOTH_PHONEBOOK_API PacmGetDataSize(PhonebookDataHandleT phonebookDataHandle, DWORD* size);
DWORD BLUETOOTH_PHONEBOOK_API PacmGetDataType(PhonebookDataHandleT phonebookDataHandle, PacmDatatype* dataType);
void  BLUETOOTH_PHONEBOOK_API PacmCloseHandle(PhonebookDataHandleT phonebookDataHandle);

#ifdef __cplusplus
}
#endif

#endif
