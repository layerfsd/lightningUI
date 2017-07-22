/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2013

               All rights reserved and confidential information of CSR

FILE:          pinyin.h

DESCRIPTION:   Header for the utility to get pinyin for simplified Chinese.

REVISION:      $Revision: #1 $

NOTE:          Only support to get pinyin in the 1st level of simplified
               Chinese library, defined in GBK/GB2312.
****************************************************************************/
#pragma once

#include <windows.h>


/* Get pinyin from Unicode string. */
LPSTR GetPinYin(LPCWSTR unicodeStr);

/* Get pinyin & first letters(consonant) from Unicode string. */
BOOL GetPinYin(LPCWSTR unicodeStr, LPSTR& pinyin, LPSTR& firstLetters);

void FreePinYin(LPSTR pinyin);

LPSTR CopyPinYin(LPSTR& dst, LPSTR src);