
#ifndef __TXMLSTL_H__
#define __TXMLSTL_H__

#pragma once
#include <windows.h>
#include <tchar.h>
#pragma warning(disable:4002)//add by Yang to hide warning C4002: too many actual parameters for macro 'TXMLASSERT'

typedef long LONG_PTR;
typedef unsigned long ULONG_PTR;
typedef ULONG_PTR DWORD_PTR;

struct TXmlStringData
{
    long nRefs;     // reference count
    int nDataLength;
    int nAllocLength;
    // TCHAR data[nAllocLength]

    TCHAR *data()
    {
        return (TCHAR *)(this + 1);
    }
};

// Globals

// For an empty string, m_pchData will point here
// (note: avoids special case of checking for NULL m_pchData)
// empty string data (and locked)
_declspec(selectany) int _txml_rgInitData[] = { -1, 0, 0, 0 };
_declspec(selectany) TXmlStringData *_txml_tmpDataNil = (TXmlStringData *) &_txml_rgInitData;
_declspec(selectany) LPCTSTR _txml_tmpPchNil = (LPCTSTR)(((BYTE *)&_txml_rgInitData) + sizeof(TXmlStringData));

class TXmlString
{
public:
// Constructors
    TXmlString();
    TXmlString(const TXmlString &stringSrc);
    TXmlString(TCHAR ch, int nRepeat = 1);
    TXmlString(LPCSTR lpsz);
    TXmlString(LPCWSTR lpsz);
    TXmlString(LPCSTR lpch, int nLength);
    TXmlString(LPCWSTR lpch, int nLength);
    TXmlString(const unsigned char *psz);

// Attributes & Operations
    // as an array of characters
    int GetLength() const;
    BOOL IsEmpty() const;
    void Empty();                       // free up the data

    TCHAR GetAt(int nIndex) const;      // 0 based
    TCHAR operator[](int nIndex) const; // same as GetAt
    void SetAt(int nIndex, TCHAR ch);
    operator LPCTSTR() const;           // as a C string

    // overloaded assignment
    const TXmlString &operator=(const TXmlString &stringSrc);
    const TXmlString &operator=(TCHAR ch);
#ifdef _UNICODE
    const TXmlString &operator=(char ch);
#endif
    const TXmlString &operator=(LPCSTR lpsz);
    const TXmlString &operator=(LPCWSTR lpsz);
    const TXmlString &operator=(const unsigned char *psz);

    // string concatenation
    const TXmlString &operator+=(const TXmlString &string);
    const TXmlString &operator+=(TCHAR ch);
#ifdef _UNICODE
    const TXmlString &operator+=(char ch);
#endif
    const TXmlString &operator+=(LPCTSTR lpsz);

    friend TXmlString __stdcall operator+(const TXmlString &string1, const TXmlString &string2);
    friend TXmlString __stdcall operator+(const TXmlString &string, TCHAR ch);
    friend TXmlString __stdcall operator+(TCHAR ch, const TXmlString &string);
#ifdef _UNICODE
    friend TXmlString __stdcall operator+(const TXmlString &string, char ch);
    friend TXmlString __stdcall operator+(char ch, const TXmlString &string);
#endif
    friend TXmlString __stdcall operator+(const TXmlString &string, LPCTSTR lpsz);
    friend TXmlString __stdcall operator+(LPCTSTR lpsz, const TXmlString &string);

    // string comparison
    int Compare(LPCTSTR lpsz) const;         // straight character
    int CompareNoCase(LPCTSTR lpsz) const;   // ignore case
    int Collate(LPCTSTR lpsz) const;         // NLS aware
    int CollateNoCase(LPCTSTR lpsz) const;   // ignore case

    // simple sub-string extraction
    TXmlString Mid(int nFirst, int nCount) const;
    TXmlString Mid(int nFirst) const;
    TXmlString Left(int nCount) const;
    TXmlString Right(int nCount) const;

    TXmlString SpanIncluding(LPCTSTR lpszCharSet) const;
    TXmlString SpanExcluding(LPCTSTR lpszCharSet) const;

    // upper/lower/reverse conversion
    void MakeUpper();
    void MakeLower();
    void MakeReverse();

    // trimming whitespace (either side)
    void TrimRight();
    void TrimLeft();

    // remove continuous occurrences of chTarget starting from right
    void TrimRight(TCHAR chTarget);
    // remove continuous occcurrences of characters in passed string,
    // starting from right
    void TrimRight(LPCTSTR lpszTargets);
    // remove continuous occurrences of chTarget starting from left
    void TrimLeft(TCHAR chTarget);
    // remove continuous occcurrences of characters in
    // passed string, starting from left
    void TrimLeft(LPCTSTR lpszTargets);

    // advanced manipulation
    // replace occurrences of chOld with chNew
    int Replace(TCHAR chOld, TCHAR chNew);
    // replace occurrences of substring lpszOld with lpszNew;
    // empty lpszNew removes instances of lpszOld
    int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
    // remove occurrences of chRemove
    int Remove(TCHAR chRemove);
    // insert character at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, TCHAR ch);
    // insert substring at zero-based index; concatenates
    // if index is past end of string
    int Insert(int nIndex, LPCTSTR pstr);
    // delete nCount characters starting at zero-based index
    int Delete(int nIndex, int nCount = 1);

    // searching (return starting index, or -1 if not found)
    // look for a single character match
    int Find(TCHAR ch) const;                     // like "C" strchr
    int ReverseFind(TCHAR ch) const;
    int Find(TCHAR ch, int nStart) const;         // starting at index
    int FindOneOf(LPCTSTR lpszCharSet) const;

    // look for a specific sub-string
    int Find(LPCTSTR lpszSub) const;        // like "C" strstr
    int Find(LPCTSTR lpszSub, int nStart) const;  // starting at index

    // Concatentation for non strings
    const TXmlString &Append(int n)
    {
        TCHAR szBuffer[10];
        wsprintf(szBuffer, _T("%d"), n);
        ConcatInPlace(SafeStrlen(szBuffer), szBuffer);
        return *this;
    }

    // Access to string implementation buffer as "C" character array
    LPTSTR GetBuffer(int nMinBufLength);
    void ReleaseBuffer(int nNewLength = -1);
    LPTSTR GetBufferSetLength(int nNewLength);
    void FreeExtra();

    // Use LockBuffer/UnlockBuffer to turn refcounting off
    LPTSTR LockBuffer();
    void UnlockBuffer();

// Implementation
public:
    ~TXmlString();
    int GetAllocLength() const;

    static BOOL __stdcall _IsValidString(LPCWSTR lpsz, int nLength = -1)
    {
        if (lpsz == NULL)
            return FALSE;

        return TRUE;//!::IsBadStringPtrW(lpsz, nLength);
    }

    static BOOL __stdcall _IsValidString(LPCSTR lpsz, int nLength = -1)
    {
        if (lpsz == NULL)
            return FALSE;

        return TRUE;//!::IsBadStringPtrA(lpsz, nLength);
    }
    LPTSTR m_pchData;   // pointer to ref counted string data
    // implementation helpers
    TXmlStringData *GetData() const;
    static int PASCAL SafeStrlen(LPCTSTR lpsz);
    BOOL ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
protected:

    void Init();
    void AllocCopy(TXmlString &dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
    BOOL AllocBuffer(int nLen);
    void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
    void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
    void CopyBeforeWrite();
    BOOL AllocBeforeWrite(int nLen);
    void Release();
    static void PASCAL Release(TXmlStringData *pData);

    static const TXmlString &__stdcall _GetEmptyString()
    {
        return *(TXmlString *)&_txml_tmpPchNil;
    }

// TXmlString conversion helpers
    static int __cdecl _wcstombsz(char *mbstr, const wchar_t *wcstr, size_t count)
    {
        if (count == 0 && mbstr != NULL)
            return 0;

        int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
        TXMLASSERT(mbstr == NULL || result <= (int)count);

        if (result > 0)
            mbstr[result - 1] = 0;

        return result;
    }

    static int __cdecl _mbstowcsz(wchar_t *wcstr, const char *mbstr, size_t count)
    {
        if (count == 0 && wcstr != NULL)
            return 0;

        int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1, wcstr, (int)count);
        TXMLASSERT(wcstr == NULL || result <= (int)count);

        if (result > 0)
            wcstr[result - 1] = 0;

        return result;
    }

// Helpers to avoid CRT startup code
#ifdef _ATL_MIN_CRT
    static TCHAR *_cstrchr(const TCHAR *p, TCHAR ch)
    {
        //strchr for '\0' should succeed
        while (*p != 0)
        {
            if (*p == ch)
                break;

            p = ::CharNext(p);
        }

        return (TCHAR *)((*p == ch) ? p : NULL);
    }
    static TCHAR *_cstrchr_db(const TCHAR *p, TCHAR ch1, TCHAR ch2)
    {
        const TCHAR *lpsz = NULL;

        while (*p != 0)
        {
            if (*p == ch1 && *(p + 1) == ch2)
            {
                lpsz = p;
                break;
            }

            p = ::CharNext(p);
        }

        return (TCHAR *)lpsz;
    }
    static TCHAR *_cstrrchr(const TCHAR *p, TCHAR ch)
    {
        const TCHAR *lpsz = NULL;

        while (*p != 0)
        {
            if (*p == ch)
                lpsz = p;

            p = ::CharNext(p);
        }

        return (TCHAR *)lpsz;
    }
    static TCHAR *_cstrrev(TCHAR *pStr)
    {
        // Optimize NULL, zero-length, and single-char case.
        if ((pStr == NULL) || (pStr[0] == '\0') || (pStr[1] == '\0'))
            return pStr;

        TCHAR *p = pStr;

        while (p[1] != 0)
        {
            TCHAR *pNext = ::CharNext(p);

            if (pNext > p + 1)
            {
                TCHAR p1 = *(TCHAR *)p;
                *(TCHAR *)p = *(TCHAR *)(p + 1);
                *(TCHAR *)(p + 1) = p1;
            }

            p = pNext;
        }

        TCHAR *q = pStr;

        while (q < p)
        {
            TCHAR t = *q;
            *q = *p;
            *p = t;
            q++;
            p--;
        }

        return (TCHAR *)pStr;
    }
    static TCHAR *_cstrstr(const TCHAR *pStr, const TCHAR *pCharSet)
    {
        int nLen = _tcslen(pCharSet);

        if (nLen == 0)
            return (TCHAR *)pStr;

        const TCHAR *pRet = NULL;
        const TCHAR *pCur = pStr;

        while ((pStr = _cstrchr(pCur, *pCharSet)) != NULL)
        {
            if (memcmp(pCur, pCharSet, nLen * sizeof(TCHAR)) == 0)
            {
                pRet = pCur;
                break;
            }

            pCur = ::CharNext(pCur);
        }

        return (TCHAR *) pRet;
    }
    static int _cstrspn(const TCHAR *pStr, const TCHAR *pCharSet)
    {
        int nRet = 0;
        TCHAR *p = (TCHAR *)pStr;

        while (*p != 0)
        {
            TCHAR *pNext = ::CharNext(p);

            if (pNext > p + 1)
            {
                if (_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
                    break;

                nRet += 2;
            }
            else
            {
                if (_cstrchr(pCharSet, *p) == NULL)
                    break;

                nRet++;
            }

            p = pNext;
        }

        return nRet;
    }
    static int _cstrcspn(const TCHAR *pStr, const TCHAR *pCharSet)
    {
        int nRet = 0;
        TCHAR *p = (TCHAR *)pStr;

        while (*p != 0)
        {
            TCHAR *pNext = ::CharNext(p);

            if (pNext > p + 1)
            {
                if (_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
                    break;

                nRet += 2;
            }
            else
            {
                if (_cstrchr(pCharSet, *p) != NULL)
                    break;

                nRet++;
            }

            p = pNext;
        }

        return nRet;
    }
    static TCHAR *_cstrpbrk(const TCHAR *p, const TCHAR *lpszCharSet)
    {
        while (*p != 0)
        {
            if (_cstrchr(lpszCharSet, *p) != NULL)
            {
                return (TCHAR *)p;
                break;
            }

            p = ::CharNext(p);
        }

        return NULL;
    }
    static int _cstrisdigit(TCHAR ch)
    {
        WORD type;
        GetStringTypeEx(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
        return (type & C1_DIGIT) == C1_DIGIT;
    }
    static int _cstrisspace(TCHAR ch)
    {
        WORD type;
        GetStringTypeEx(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
        return (type & C1_SPACE) == C1_SPACE;
    }
    static int _cstrcmp(const TCHAR *pstrOne, const TCHAR *pstrOther)
    {
        return lstrcmp(pstrOne, pstrOther);
    }
    static int _cstrcmpi(const TCHAR *pstrOne, const TCHAR *pstrOther)
    {
        return lstrcmpi(pstrOne, pstrOther);
    }
    static int _cstrcoll(const TCHAR *pstrOne, const TCHAR *pstrOther)
    {
        int nRet = CompareString(GetThreadLocale(), 0, pstrOne, -1, pstrOther, -1);
        TXMLASSERT(nRet != 0);
        return nRet - 2;  // Convert to strcmp convention.  This really is documented.
    }
    static int _cstrcolli(const TCHAR *pstrOne, const TCHAR *pstrOther)
    {
        int nRet = CompareString(GetThreadLocale(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
        TXMLASSERT(nRet != 0);
        return nRet - 2;  // Convert to strcmp convention.  This really is documented.
    }
    static int _cstrtoi(const TCHAR *nptr)
    {
        int c;              /* current char */
        int total;          /* current total */
        int sign;           /* if '-', then negative, otherwise positive */

        while (_cstrisspace(*nptr))
            ++nptr;

        c = (int)(_TUCHAR) * nptr++;
        sign = c;           /* save sign indication */

        if (c == _T('-') || c == _T('+'))
            c = (int)(_TUCHAR) * nptr++;  /* skip sign */

        total = 0;

        while (_cstrisdigit((TCHAR)c))
        {
            total = 10 * total + (c - '0');     /* accumulate digit */
            c = (int)(_TUCHAR) * nptr++;  /* get next char */
        }

        if (sign == '-')
            return -total;
        else
            return total;   /* return result, negated if necessary */
    }
#else //!_ATL_MIN_CRT
    static TCHAR *_cstrchr(const TCHAR *p, TCHAR ch)
    {
        return (TCHAR *)_tcschr(p, ch);
    }
    static TCHAR *_cstrrchr(const TCHAR *p, TCHAR ch)
    {
        return (TCHAR *)_tcsrchr(p, ch);
    }
    static TCHAR *_cstrrev(TCHAR *pStr)
    {
        return _tcsrev(pStr);
    }
    static TCHAR *_cstrstr(const TCHAR *pStr, const TCHAR *pCharSet)
    {
        return (TCHAR *)_tcsstr(pStr, pCharSet);
    }
    static int _cstrspn(const TCHAR *pStr, const TCHAR *pCharSet)
    {
        return (int)_tcsspn(pStr, pCharSet);
    }
    static int _cstrcspn(const TCHAR *pStr, const TCHAR *pCharSet)
    {
        return (int)_tcscspn(pStr, pCharSet);
    }
    static TCHAR *_cstrpbrk(const TCHAR *p, const TCHAR *lpszCharSet)
    {
        return (TCHAR *)_tcspbrk(p, lpszCharSet);
    }
    static int _cstrisdigit(TCHAR ch)
    {
        return _istdigit(ch);
    }
    static int _cstrisspace(TCHAR ch)
    {
        return _istspace(ch);
    }
    static int _cstrcmp(const TCHAR *pstrOne, const TCHAR *pstrOther)
    {
        return _tcscmp(pstrOne, pstrOther);
    }
    static int _cstrcmpi(const TCHAR *pstrOne, const TCHAR *pstrOther)
    {
        return _tcsicmp(pstrOne, pstrOther);
    }
    static int _cstrtoi(const TCHAR *nptr)
    {
        return _ttoi(nptr);
    }
#endif //!_ATL_MIN_CRT
};

// Compare helpers
bool __stdcall operator==(const TXmlString &s1, const TXmlString &s2);
bool __stdcall operator==(const TXmlString &s1, LPCTSTR s2);
bool __stdcall operator==(LPCTSTR s1, const TXmlString &s2);
bool __stdcall operator!=(const TXmlString &s1, const TXmlString &s2);
bool __stdcall operator!=(const TXmlString &s1, LPCTSTR s2);
bool __stdcall operator!=(LPCTSTR s1, const TXmlString &s2);
bool __stdcall operator<(const TXmlString &s1, const TXmlString &s2);
bool __stdcall operator<(const TXmlString &s1, LPCTSTR s2);
bool __stdcall operator<(LPCTSTR s1, const TXmlString &s2);
bool __stdcall operator>(const TXmlString &s1, const TXmlString &s2);
bool __stdcall operator>(const TXmlString &s1, LPCTSTR s2);
bool __stdcall operator>(LPCTSTR s1, const TXmlString &s2);
bool __stdcall operator<=(const TXmlString &s1, const TXmlString &s2);
bool __stdcall operator<=(const TXmlString &s1, LPCTSTR s2);
bool __stdcall operator<=(LPCTSTR s1, const TXmlString &s2);
bool __stdcall operator>=(const TXmlString &s1, const TXmlString &s2);
bool __stdcall operator>=(const TXmlString &s1, LPCTSTR s2);
bool __stdcall operator>=(LPCTSTR s1, const TXmlString &s2);


/////////////////////////////////////////////////////////////////////////////
// TXmlString Implementation

inline TXmlStringData *TXmlString::GetData() const
{
    TXMLASSERT(m_pchData != NULL);
    return ((TXmlStringData *)m_pchData) - 1;
}
inline void TXmlString::Init()
{
    m_pchData = _GetEmptyString().m_pchData;
}
inline TXmlString::TXmlString(const unsigned char *lpsz)
{
    Init();
    *this = (LPCSTR)lpsz;
}
inline const TXmlString &TXmlString::operator=(const unsigned char *lpsz)
{
    *this = (LPCSTR)lpsz;
    return *this;
}
#ifdef _UNICODE
inline const TXmlString &TXmlString::operator+=(char ch)
{
    *this += (TCHAR)ch;
    return *this;
}
inline const TXmlString &TXmlString::operator=(char ch)
{
    *this = (TCHAR)ch;
    return *this;
}
inline TXmlString __stdcall operator+(const TXmlString &string, char ch)
{
    return string + (TCHAR)ch;
}
inline TXmlString __stdcall operator+(char ch, const TXmlString &string)
{
    return (TCHAR)ch + string;
}
#endif

inline int TXmlString::GetLength() const
{
    return GetData()->nDataLength;
}
inline int TXmlString::GetAllocLength() const
{
    return GetData()->nAllocLength;
}
inline BOOL TXmlString::IsEmpty() const
{
    return GetData()->nDataLength == 0;
}
inline TXmlString::operator LPCTSTR() const
{
    return m_pchData;
}
inline int PASCAL TXmlString::SafeStrlen(LPCTSTR lpsz)
{
    return (lpsz == NULL) ? 0 : _tcslen(lpsz);
}

// TXmlString support (windows specific)
inline int TXmlString::Compare(LPCTSTR lpsz) const
{
    return _cstrcmp(m_pchData, lpsz);    // MBCS/Unicode aware
}
inline int TXmlString::CompareNoCase(LPCTSTR lpsz) const
{
    return _cstrcmpi(m_pchData, lpsz);    // MBCS/Unicode aware
}
// TXmlString::Collate is often slower than Compare but is MBSC/Unicode
//  aware as well as locale-sensitive with respect to sort order.
#if 0
inline int TXmlString::Collate(LPCTSTR lpsz) const
{
    return _cstrcoll(m_pchData, lpsz);    // locale sensitive
}
inline int TXmlString::CollateNoCase(LPCTSTR lpsz) const
{
    return _cstrcolli(m_pchData, lpsz);    // locale sensitive
}
#endif
inline TCHAR TXmlString::GetAt(int nIndex) const
{
    TXMLASSERT(nIndex >= 0);
    TXMLASSERT(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
}
inline TCHAR TXmlString::operator[](int nIndex) const
{
    // same as GetAt
    TXMLASSERT(nIndex >= 0);
    TXMLASSERT(nIndex < GetData()->nDataLength);
    return m_pchData[nIndex];
}
inline bool __stdcall operator==(const TXmlString &s1, const TXmlString &s2)
{
    return s1.Compare(s2) == 0;
}
inline bool __stdcall operator==(const TXmlString &s1, LPCTSTR s2)
{
    return s1.Compare(s2) == 0;
}
inline bool __stdcall operator==(LPCTSTR s1, const TXmlString &s2)
{
    return s2.Compare(s1) == 0;
}
inline bool __stdcall operator!=(const TXmlString &s1, const TXmlString &s2)
{
    return s1.Compare(s2) != 0;
}
inline bool __stdcall operator!=(const TXmlString &s1, LPCTSTR s2)
{
    return s1.Compare(s2) != 0;
}
inline bool __stdcall operator!=(LPCTSTR s1, const TXmlString &s2)
{
    return s2.Compare(s1) != 0;
}
inline bool __stdcall operator<(const TXmlString &s1, const TXmlString &s2)
{
    return s1.Compare(s2) < 0;
}
inline bool __stdcall operator<(const TXmlString &s1, LPCTSTR s2)
{
    return s1.Compare(s2) < 0;
}
inline bool __stdcall operator<(LPCTSTR s1, const TXmlString &s2)
{
    return s2.Compare(s1) > 0;
}
inline bool __stdcall operator>(const TXmlString &s1, const TXmlString &s2)
{
    return s1.Compare(s2) > 0;
}
inline bool __stdcall operator>(const TXmlString &s1, LPCTSTR s2)
{
    return s1.Compare(s2) > 0;
}
inline bool __stdcall operator>(LPCTSTR s1, const TXmlString &s2)
{
    return s2.Compare(s1) < 0;
}
inline bool __stdcall operator<=(const TXmlString &s1, const TXmlString &s2)
{
    return s1.Compare(s2) <= 0;
}
inline bool __stdcall operator<=(const TXmlString &s1, LPCTSTR s2)
{
    return s1.Compare(s2) <= 0;
}
inline bool __stdcall operator<=(LPCTSTR s1, const TXmlString &s2)
{
    return s2.Compare(s1) >= 0;
}
inline bool __stdcall operator>=(const TXmlString &s1, const TXmlString &s2)
{
    return s1.Compare(s2) >= 0;
}
inline bool __stdcall operator>=(const TXmlString &s1, LPCTSTR s2)
{
    return s1.Compare(s2) >= 0;
}
inline bool __stdcall operator>=(LPCTSTR s1, const TXmlString &s2)
{
    return s2.Compare(s1) <= 0;
}

inline TXmlString::TXmlString()
{
    Init();
}

inline TXmlString::TXmlString(const TXmlString &stringSrc)
{
    TXMLASSERT(stringSrc.GetData()->nRefs != 0);

    if (stringSrc.GetData()->nRefs >= 0)
    {
        TXMLASSERT(stringSrc.GetData() != _txml_tmpDataNil);
        m_pchData = stringSrc.m_pchData;
        InterlockedIncrement(&GetData()->nRefs);
    }
    else
    {
        Init();
        *this = stringSrc.m_pchData;
    }
}

inline BOOL TXmlString::AllocBuffer(int nLen)
// always allocate one extra character for '\0' termination
// assumes [optimistically] that data length will equal allocation length
{
    TXMLASSERT(nLen >= 0);
    TXMLASSERT(nLen <= 0x7fffffff);    // max size (enough room for 1 extra)

    if (nLen == 0)
    {
        Init();
    }
    else
    {
        TXmlStringData *pData = NULL;
        pData = (TXmlStringData *)new BYTE[sizeof(TXmlStringData) + (nLen + 1) * sizeof(TCHAR)];

        if (pData == NULL)
            return FALSE;

        pData->nRefs = 1;
        pData->data()[nLen] = '\0';
        pData->nDataLength = nLen;
        pData->nAllocLength = nLen;
        m_pchData = pData->data();
    }

    return TRUE;
}

inline void TXmlString::Release()
{
    if (GetData() != _txml_tmpDataNil)
    {
        TXMLASSERT(GetData()->nRefs != 0);

        if (InterlockedDecrement(&GetData()->nRefs) <= 0)
            delete[](BYTE *)GetData();

        Init();
    }
}

inline void PASCAL TXmlString::Release(TXmlStringData *pData)
{
    if (pData != _txml_tmpDataNil)
    {
        TXMLASSERT(pData->nRefs != 0);

        if (InterlockedDecrement(&pData->nRefs) <= 0)
            delete[](BYTE *)pData;
    }
}

inline void TXmlString::Empty()
{
    if (GetData()->nDataLength == 0)
        return;

    if (GetData()->nRefs >= 0)
        Release();
    else
        *this = _T("");

    TXMLASSERT(GetData()->nDataLength == 0);
    TXMLASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
}

inline void TXmlString::CopyBeforeWrite()
{
    if (GetData()->nRefs > 1)
    {
        TXmlStringData *pData = GetData();
        Release();

        if (AllocBuffer(pData->nDataLength))
            memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(TCHAR));
    }

    TXMLASSERT(GetData()->nRefs <= 1);
}

inline BOOL TXmlString::AllocBeforeWrite(int nLen)
{
    BOOL bRet = TRUE;

    if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
    {
        Release();
        bRet = AllocBuffer(nLen);
    }

    TXMLASSERT(GetData()->nRefs <= 1);
    return bRet;
}

inline TXmlString::~TXmlString()
//  free any attached data
{
    if (GetData() != _txml_tmpDataNil)
    {
        if (InterlockedDecrement(&GetData()->nRefs) <= 0)
            delete[](BYTE *)GetData();
    }
}

inline void TXmlString::AllocCopy(TXmlString &dest, int nCopyLen, int nCopyIndex,
                                  int nExtraLen) const
{
    // will clone the data attached to this string
    // allocating 'nExtraLen' characters
    // Places results in uninitialized string 'dest'
    // Will copy the part or all of original data to start of new string

    int nNewLen = nCopyLen + nExtraLen;

    if (nNewLen == 0)
    {
        dest.Init();
    }
    else
    {
        if (dest.AllocBuffer(nNewLen))
            memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen * sizeof(TCHAR));
    }
}

inline TXmlString::TXmlString(LPCTSTR lpsz)
{
    Init();
    int nLen = SafeStrlen(lpsz);

    if (nLen != 0)
    {
        if (AllocBuffer(nLen))
            memcpy(m_pchData, lpsz, nLen * sizeof(TCHAR));
    }
}

#ifdef _UNICODE
inline TXmlString::TXmlString(LPCSTR lpsz)
{
    Init();
    int nSrcLen = 0;

    if (lpsz)
    {
        LPCSTR p = lpsz;

        while (*p++)++nSrcLen;
    }

    if (nSrcLen != 0)
    {
        if (AllocBuffer(nSrcLen))
        {
            _mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
            ReleaseBuffer();
        }
    }
}
#else //_UNICODE
inline TXmlString::TXmlString(LPCWSTR lpsz)
{
    Init();
    int nSrcLen = (lpsz != NULL) ? (int)wcslen(lpsz) : 0;

    if (nSrcLen != 0)
    {
        if (AllocBuffer(nSrcLen * 2))
        {
            _wcstombsz(m_pchData, lpsz, (nSrcLen * 2) + 1);
            ReleaseBuffer();
        }
    }
}
#endif //!_UNICODE

// Assignment operators
//  All assign a new value to the string
//      (a) first see if the buffer is big enough
//      (b) if enough room, copy on top of old buffer, set size and type
//      (c) otherwise free old string data, and create a new one
//
//  All routines return the new string (but as a 'const TXmlString&' so that
//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
//

inline void TXmlString::AssignCopy(int nSrcLen, LPCTSTR lpszSrcData)
{
    if (AllocBeforeWrite(nSrcLen))
    {
        memcpy(m_pchData, lpszSrcData, nSrcLen * sizeof(TCHAR));
        GetData()->nDataLength = nSrcLen;
        m_pchData[nSrcLen] = '\0';
    }
}

inline const TXmlString &TXmlString::operator=(const TXmlString &stringSrc)
{
    if (m_pchData != stringSrc.m_pchData)
    {
        if ((GetData()->nRefs < 0 && GetData() != _txml_tmpDataNil) || stringSrc.GetData()->nRefs < 0)
        {
            // actual copy necessary since one of the strings is locked
            AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
        }
        else
        {
            // can just copy references around
            Release();
            TXMLASSERT(stringSrc.GetData() != _txml_tmpDataNil);
            m_pchData = stringSrc.m_pchData;
            InterlockedIncrement(&GetData()->nRefs);
        }
    }

    return *this;
}

inline const TXmlString &TXmlString::operator=(LPCTSTR lpsz)
{
    TXMLASSERT(lpsz == NULL || _IsValidString(lpsz));
    AssignCopy(SafeStrlen(lpsz), lpsz);
    return *this;
}

#ifdef _UNICODE
inline const TXmlString &TXmlString::operator=(LPCSTR lpsz)
{
    int nSrcLen = 0;

    if (lpsz) //add by Yang
    {
        LPCSTR p = lpsz;

        while (*p++)nSrcLen++;

		if (AllocBeforeWrite(nSrcLen))
		{
			_mbstowcsz(m_pchData, lpsz, nSrcLen + 1);
			ReleaseBuffer();
		}		
    }
		
    return *this;
}
#else //!_UNICODE
inline const TXmlString &TXmlString::operator=(LPCWSTR lpsz)
{
    int nSrcLen = (lpsz != NULL) ? (int)wcslen(lpsz) : 0;

    if (AllocBeforeWrite(nSrcLen * 2))
    {
        _wcstombsz(m_pchData, lpsz, (nSrcLen * 2) + 1);
        ReleaseBuffer();
    }

    return *this;
}
#endif  //!_UNICODE

// Concatenation
// NOTE: "operator+" is done as friend functions for simplicity
//      There are three variants:
//          TXmlString + TXmlString
// and for ? = TCHAR, LPCTSTR
//          TXmlString + ?
//          ? + TXmlString

inline BOOL TXmlString::ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data,
                                   int nSrc2Len, LPCTSTR lpszSrc2Data)
{
    // -- master concatenation routine
    // Concatenate two sources
    // -- assume that 'this' is a new TXmlString object

    BOOL bRet = TRUE;
    int nNewLen = nSrc1Len + nSrc2Len;

    if (nNewLen != 0)
    {
        bRet = AllocBuffer(nNewLen);

        if (bRet)
        {
            memcpy(m_pchData, lpszSrc1Data, nSrc1Len * sizeof(TCHAR));
            memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(TCHAR));
        }
    }

    return bRet;
}

inline TXmlString __stdcall operator+(const TXmlString &string1, const TXmlString &string2)
{
    TXmlString s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, string2.GetData()->nDataLength, string2.m_pchData);
    return s;
}

inline TXmlString __stdcall operator+(const TXmlString &string, LPCTSTR lpsz)
{
    TXMLASSERT(lpsz == NULL || TXmlString::_IsValidString(lpsz));
    TXmlString s;
    s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData, TXmlString::SafeStrlen(lpsz), lpsz);
    return s;
}

inline TXmlString __stdcall operator+(LPCTSTR lpsz, const TXmlString &string)
{
    TXMLASSERT(lpsz == NULL || TXmlString::_IsValidString(lpsz));
    TXmlString s;
    s.ConcatCopy(TXmlString::SafeStrlen(lpsz), lpsz, string.GetData()->nDataLength, string.m_pchData);
    return s;
}

inline void TXmlString::ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData)
{
    //  -- the main routine for += operators

    // concatenating an empty string is a no-op!
    if (nSrcLen == 0)
        return;

    // if the buffer is too small, or we have a width mis-match, just
    //   allocate a new buffer (slow but sure)
    if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
    {
        // we have to grow the buffer, use the ConcatCopy routine
        TXmlStringData *pOldData = GetData();

        if (ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData))
        {
            TXMLASSERT(pOldData != NULL);
            TXmlString::Release(pOldData);
        }
    }
    else
    {
        // fast concatenation when buffer big enough
        memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(TCHAR));
        GetData()->nDataLength += nSrcLen;
        TXMLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);
        m_pchData[GetData()->nDataLength] = '\0';
    }
}

inline const TXmlString &TXmlString::operator+=(LPCTSTR lpsz)
{
    TXMLASSERT(lpsz == NULL || _IsValidString(lpsz));
    ConcatInPlace(SafeStrlen(lpsz), lpsz);
    return *this;
}

inline const TXmlString &TXmlString::operator+=(TCHAR ch)
{
    ConcatInPlace(1, &ch);
    return *this;
}

inline const TXmlString &TXmlString::operator+=(const TXmlString &string)
{
    ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
    return *this;
}

inline LPTSTR TXmlString::GetBuffer(int nMinBufLength)
{
    TXMLASSERT(nMinBufLength >= 0);

    if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
    {
        // we have to grow the buffer
        TXmlStringData *pOldData = GetData();
        int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it

        if (nMinBufLength < nOldLen)
            nMinBufLength = nOldLen;

        if (!AllocBuffer(nMinBufLength))
            return NULL;

        memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(TCHAR));
        GetData()->nDataLength = nOldLen;
        TXmlString::Release(pOldData);
    }

    TXMLASSERT(GetData()->nRefs <= 1);

    // return a pointer to the character storage for this string
    TXMLASSERT(m_pchData != NULL);
    return m_pchData;
}

inline void TXmlString::ReleaseBuffer(int nNewLength)
{
    CopyBeforeWrite();  // just in case GetBuffer was not called

    if (nNewLength == -1)
        nNewLength = _tcslen(m_pchData); // zero terminated

    TXMLASSERT(nNewLength <= GetData()->nAllocLength);
    GetData()->nDataLength = nNewLength;
    m_pchData[nNewLength] = '\0';
}

inline LPTSTR TXmlString::GetBufferSetLength(int nNewLength)
{
    TXMLASSERT(nNewLength >= 0);

    if (GetBuffer(nNewLength) == NULL)
        return NULL;

    GetData()->nDataLength = nNewLength;
    m_pchData[nNewLength] = '\0';
    return m_pchData;
}

inline void TXmlString::FreeExtra()
{
    TXMLASSERT(GetData()->nDataLength <= GetData()->nAllocLength);

    if (GetData()->nDataLength != GetData()->nAllocLength)
    {
        TXmlStringData *pOldData = GetData();

        if (AllocBuffer(GetData()->nDataLength))
        {
            memcpy(m_pchData, pOldData->data(), pOldData->nDataLength * sizeof(TCHAR));
            TXMLASSERT(m_pchData[GetData()->nDataLength] == '\0');
            TXmlString::Release(pOldData);
        }
    }

    TXMLASSERT(GetData() != NULL);
}

inline LPTSTR TXmlString::LockBuffer()
{
    LPTSTR lpsz = GetBuffer(0);

    if (lpsz != NULL)
        GetData()->nRefs = -1;

    return lpsz;
}

inline void TXmlString::UnlockBuffer()
{
    TXMLASSERT(GetData()->nRefs == -1);

    if (GetData() != _txml_tmpDataNil)
        GetData()->nRefs = 1;
}

inline int TXmlString::Find(TCHAR ch) const
{
    return Find(ch, 0);
}

inline int TXmlString::Find(TCHAR ch, int nStart) const
{
    int nLength = GetData()->nDataLength;

    if (nStart >= nLength)
        return -1;

    // find first single character
    LPTSTR lpsz = _cstrchr(m_pchData + nStart, (_TUCHAR)ch);

    // return -1 if not found and index otherwise
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

inline int TXmlString::FindOneOf(LPCTSTR lpszCharSet) const
{
    TXMLASSERT(_IsValidString(lpszCharSet));
    LPTSTR lpsz = _cstrpbrk(m_pchData, lpszCharSet);
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

inline void TXmlString::MakeUpper()
{
    CopyBeforeWrite();
    CharUpper(m_pchData);
}

inline void TXmlString::MakeLower()
{
    CopyBeforeWrite();
    CharLower(m_pchData);
}

inline void TXmlString::MakeReverse()
{
    CopyBeforeWrite();
    _cstrrev(m_pchData);
}

inline void TXmlString::SetAt(int nIndex, TCHAR ch)
{
    TXMLASSERT(nIndex >= 0);
    TXMLASSERT(nIndex < GetData()->nDataLength);

    CopyBeforeWrite();
    m_pchData[nIndex] = ch;
}

inline TXmlString::TXmlString(TCHAR ch, int nLength)
{
    TXMLASSERT(!_istlead(ch));    // can't create a lead byte string
    Init();

    if (nLength >= 1)
    {
        if (AllocBuffer(nLength))
        {
#ifdef _UNICODE

            for (int i = 0; i < nLength; i++)
                m_pchData[i] = ch;

#else
            memset(m_pchData, ch, nLength);
#endif
        }
    }
}

inline TXmlString::TXmlString(LPCTSTR lpch, int nLength)
{
    Init();

    if (nLength != 0)
    {
        if (AllocBuffer(nLength))
            memcpy(m_pchData, lpch, nLength * sizeof(TCHAR));
    }
}

#ifdef _UNICODE
inline TXmlString::TXmlString(LPCSTR lpsz, int nLength)
{
    Init();

    if (nLength != 0)
    {
        if (AllocBuffer(nLength))
        {
            int n = ::MultiByteToWideChar(CP_ACP, 0, lpsz, nLength, m_pchData, nLength + 1);
            ReleaseBuffer((n >= 0) ? n : -1);
        }
    }
}
#else //_UNICODE
inline TXmlString::TXmlString(LPCWSTR lpsz, int nLength)
{
    Init();

    if (nLength != 0)
    {
        if (AllocBuffer(nLength * 2))
        {
            int n = ::WideCharToMultiByte(CP_ACP, 0, lpsz, nLength, m_pchData, (nLength * 2) + 1, NULL, NULL);
            ReleaseBuffer((n >= 0) ? n : -1);
        }
    }
}
#endif //!_UNICODE

inline const TXmlString &TXmlString::operator=(TCHAR ch)
{
    TXMLASSERT(!_istlead(ch));    // can't set single lead byte
    AssignCopy(1, &ch);
    return *this;
}

inline TXmlString __stdcall operator+(const TXmlString &string1, TCHAR ch)
{
    TXmlString s;
    s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, 1, &ch);
    return s;
}

inline TXmlString __stdcall operator+(TCHAR ch, const TXmlString &string)
{
    TXmlString s;
    s.ConcatCopy(1, &ch, string.GetData()->nDataLength, string.m_pchData);
    return s;
}

inline TXmlString TXmlString::Mid(int nFirst) const
{
    return Mid(nFirst, GetData()->nDataLength - nFirst);
}

inline TXmlString TXmlString::Mid(int nFirst, int nCount) const
{
    // out-of-bounds requests return sensible things
    if (nFirst < 0)
        nFirst = 0;

    if (nCount < 0)
        nCount = 0;

    if (nFirst + nCount > GetData()->nDataLength)
        nCount = GetData()->nDataLength - nFirst;

    if (nFirst > GetData()->nDataLength)
        nCount = 0;

    TXmlString dest;
    AllocCopy(dest, nCount, nFirst, 0);
    return dest;
}

inline TXmlString TXmlString::Right(int nCount) const
{
    if (nCount < 0)
        nCount = 0;
    else if (nCount > GetData()->nDataLength)
        nCount = GetData()->nDataLength;

    TXmlString dest;
    AllocCopy(dest, nCount, GetData()->nDataLength - nCount, 0);
    return dest;
}

inline TXmlString TXmlString::Left(int nCount) const
{
    if (nCount < 0)
        nCount = 0;
    else if (nCount > GetData()->nDataLength)
        nCount = GetData()->nDataLength;

    TXmlString dest;
    AllocCopy(dest, nCount, 0, 0);
    return dest;
}

// strspn equivalent
inline TXmlString TXmlString::SpanIncluding(LPCTSTR lpszCharSet) const
{
    TXMLASSERT(_IsValidString(lpszCharSet));
    return Left(_cstrspn(m_pchData, lpszCharSet));
}

// strcspn equivalent
inline TXmlString TXmlString::SpanExcluding(LPCTSTR lpszCharSet) const
{
    TXMLASSERT(_IsValidString(lpszCharSet));
    return Left(_cstrcspn(m_pchData, lpszCharSet));
}

inline int TXmlString::ReverseFind(TCHAR ch) const
{
    // find last single character
    LPTSTR lpsz = _cstrrchr(m_pchData, (_TUCHAR)ch);

    // return -1 if not found, distance from beginning otherwise
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

// find a sub-string (like strstr)
inline int TXmlString::Find(LPCTSTR lpszSub) const
{
    return Find(lpszSub, 0);
}

inline int TXmlString::Find(LPCTSTR lpszSub, int nStart) const
{
    TXMLASSERT(_IsValidString(lpszSub));

    int nLength = GetData()->nDataLength;

    if (nStart > nLength)
        return -1;

    // find first matching substring
    LPTSTR lpsz = _cstrstr(m_pchData + nStart, lpszSub);

    // return -1 for not found, distance from beginning otherwise
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

inline void TXmlString::TrimRight()
{
    CopyBeforeWrite();

    // find beginning of trailing spaces by starting at beginning (DBCS aware)
    LPTSTR lpsz = m_pchData;
    LPTSTR lpszLast = NULL;

    while (*lpsz != '\0')
    {
        if (_cstrisspace(*lpsz))
        {
            if (lpszLast == NULL)
                lpszLast = lpsz;
        }
        else
        {
            lpszLast = NULL;
        }

        lpsz = ::CharNext(lpsz);
    }

    if (lpszLast != NULL)
    {
        // truncate at trailing space start
        *lpszLast = '\0';
        GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
    }
}

inline void TXmlString::TrimLeft()
{
    CopyBeforeWrite();

    // find first non-space character
    LPCTSTR lpsz = m_pchData;

    while (_cstrisspace(*lpsz))
        lpsz = ::CharNext(lpsz);

    // fix up data and length
    int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
    memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(TCHAR));
    GetData()->nDataLength = nDataLength;
}

inline void TXmlString::TrimRight(LPCTSTR lpszTargetList)
{
    // find beginning of trailing matches
    // by starting at beginning (DBCS aware)

    CopyBeforeWrite();
    LPTSTR lpsz = m_pchData;
    LPTSTR lpszLast = NULL;

    while (*lpsz != '\0')
    {
        if (_cstrchr(lpszTargetList, *lpsz) != NULL)
        {
            if (lpszLast == NULL)
                lpszLast = lpsz;
        }
        else
            lpszLast = NULL;

        lpsz = ::CharNext(lpsz);
    }

    if (lpszLast != NULL)
    {
        // truncate at left-most matching character
        *lpszLast = '\0';
        GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
    }
}

inline void TXmlString::TrimRight(TCHAR chTarget)
{
    // find beginning of trailing matches
    // by starting at beginning (DBCS aware)

    CopyBeforeWrite();
    LPTSTR lpsz = m_pchData;
    LPTSTR lpszLast = NULL;

    while (*lpsz != '\0')
    {
        if (*lpsz == chTarget)
        {
            if (lpszLast == NULL)
                lpszLast = lpsz;
        }
        else
            lpszLast = NULL;

        lpsz = ::CharNext(lpsz);
    }

    if (lpszLast != NULL)
    {
        // truncate at left-most matching character
        *lpszLast = '\0';
        GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pchData);
    }
}

inline void TXmlString::TrimLeft(LPCTSTR lpszTargets)
{
    // if we're not trimming anything, we're not doing any work
    if (SafeStrlen(lpszTargets) == 0)
        return;

    CopyBeforeWrite();
    LPCTSTR lpsz = m_pchData;

    while (*lpsz != '\0')
    {
        if (_cstrchr(lpszTargets, *lpsz) == NULL)
            break;

        lpsz = ::CharNext(lpsz);
    }

    if (lpsz != m_pchData)
    {
        // fix up data and length
        int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
        memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(TCHAR));
        GetData()->nDataLength = nDataLength;
    }
}

inline void TXmlString::TrimLeft(TCHAR chTarget)
{
    // find first non-matching character

    CopyBeforeWrite();
    LPCTSTR lpsz = m_pchData;

    while (chTarget == *lpsz)
        lpsz = ::CharNext(lpsz);

    if (lpsz != m_pchData)
    {
        // fix up data and length
        int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pchData);
        memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(TCHAR));
        GetData()->nDataLength = nDataLength;
    }
}

inline int TXmlString::Delete(int nIndex, int nCount /* = 1 */)
{
    if (nIndex < 0)
        nIndex = 0;

    int nNewLength = GetData()->nDataLength;

    if (nCount > 0 && nIndex < nNewLength)
    {
        CopyBeforeWrite();
        int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;

        memmove(m_pchData + nIndex, m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
        GetData()->nDataLength = nNewLength - nCount;
    }

    return nNewLength;
}

inline int TXmlString::Insert(int nIndex, TCHAR ch)
{
    CopyBeforeWrite();

    if (nIndex < 0)
        nIndex = 0;

    int nNewLength = GetData()->nDataLength;

    if (nIndex > nNewLength)
        nIndex = nNewLength;

    nNewLength++;

    if (GetData()->nAllocLength < nNewLength)
    {
        TXmlStringData *pOldData = GetData();
        LPTSTR pstr = m_pchData;

        if (!AllocBuffer(nNewLength))
            return -1;

        memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(TCHAR));
        TXmlString::Release(pOldData);
    }

    // move existing bytes down
    memmove(m_pchData + nIndex + 1, m_pchData + nIndex, (nNewLength - nIndex) * sizeof(TCHAR));
    m_pchData[nIndex] = ch;
    GetData()->nDataLength = nNewLength;

    return nNewLength;
}

inline int TXmlString::Insert(int nIndex, LPCTSTR pstr)
{
    if (nIndex < 0)
        nIndex = 0;

    int nInsertLength = SafeStrlen(pstr);
    int nNewLength = GetData()->nDataLength;

    if (nInsertLength > 0)
    {
        CopyBeforeWrite();

        if (nIndex > nNewLength)
            nIndex = nNewLength;

        nNewLength += nInsertLength;

        if (GetData()->nAllocLength < nNewLength)
        {
            TXmlStringData *pOldData = GetData();
            LPTSTR pstr = m_pchData;

            if (!AllocBuffer(nNewLength))
                return -1;

            memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(TCHAR));
            TXmlString::Release(pOldData);
        }

        // move existing bytes down
        memmove(m_pchData + nIndex + nInsertLength, m_pchData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(TCHAR));
        memcpy(m_pchData + nIndex, pstr, nInsertLength * sizeof(TCHAR));
        GetData()->nDataLength = nNewLength;
    }

    return nNewLength;
}

inline int TXmlString::Replace(TCHAR chOld, TCHAR chNew)
{
    int nCount = 0;

    // short-circuit the nop case
    if (chOld != chNew)
    {
        // otherwise modify each character that matches in the string
        CopyBeforeWrite();
        LPTSTR psz = m_pchData;
        LPTSTR pszEnd = psz + GetData()->nDataLength;

        while (psz < pszEnd)
        {
            // replace instances of the specified character only
            if (*psz == chOld)
            {
                *psz = chNew;
                nCount++;
            }

            psz = ::CharNext(psz);
        }
    }

    return nCount;
}

inline int TXmlString::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
    // can't have empty or NULL lpszOld

    int nSourceLen = SafeStrlen(lpszOld);

    if (nSourceLen == 0)
        return 0;

    int nReplacementLen = SafeStrlen(lpszNew);

    // loop once to figure out the size of the result string
    int nCount = 0;
    LPTSTR lpszStart = m_pchData;
    LPTSTR lpszEnd = m_pchData + GetData()->nDataLength;
    LPTSTR lpszTarget;

    while (lpszStart < lpszEnd)
    {
        while ((lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
        {
            nCount++;
            lpszStart = lpszTarget + nSourceLen;
        }

        lpszStart += _tcslen(lpszStart) + 1;
    }

    // if any changes were made, make them
    if (nCount > 0)
    {
        CopyBeforeWrite();

        // if the buffer is too small, just
        //   allocate a new buffer (slow but sure)
        int nOldLength = GetData()->nDataLength;
        int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;

        if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
        {
            TXmlStringData *pOldData = GetData();
            LPTSTR pstr = m_pchData;

            if (!AllocBuffer(nNewLength))
                return -1;

            memcpy(m_pchData, pstr, pOldData->nDataLength * sizeof(TCHAR));
            TXmlString::Release(pOldData);
        }

        // else, we just do it in-place
        lpszStart = m_pchData;
        lpszEnd = m_pchData + GetData()->nDataLength;

        // loop again to actually do the work
        while (lpszStart < lpszEnd)
        {
            while ((lpszTarget = _cstrstr(lpszStart, lpszOld)) != NULL)
            {
                int nBalance = nOldLength - ((int)(DWORD_PTR)(lpszTarget - m_pchData) + nSourceLen);
                memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance * sizeof(TCHAR));
                memcpy(lpszTarget, lpszNew, nReplacementLen * sizeof(TCHAR));
                lpszStart = lpszTarget + nReplacementLen;
                lpszStart[nBalance] = '\0';
                nOldLength += (nReplacementLen - nSourceLen);
            }

            lpszStart += _tcslen(lpszStart) + 1;
        }

        TXMLASSERT(m_pchData[nNewLength] == '\0');
        GetData()->nDataLength = nNewLength;
    }

    return nCount;
}

inline int TXmlString::Remove(TCHAR chRemove)
{
    CopyBeforeWrite();

    LPTSTR pstrSource = m_pchData;
    LPTSTR pstrDest = m_pchData;
    LPTSTR pstrEnd = m_pchData + GetData()->nDataLength;

    while (pstrSource < pstrEnd)
    {
        if (*pstrSource != chRemove)
        {
            *pstrDest = *pstrSource;
            pstrDest = ::CharNext(pstrDest);
        }

        pstrSource = ::CharNext(pstrSource);
    }

    *pstrDest = '\0';
    int nCount = (int)(DWORD_PTR)(pstrSource - pstrDest);
    GetData()->nDataLength -= nCount;

    return nCount;
}

#endif

