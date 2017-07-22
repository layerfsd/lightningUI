/* SVN $Id: WdkRandom.hpp 7483 2012-10-31 19:14:01Z rorym $ */
//-----------------------------------------------------------------------------
//
//  Copyright (c) Qualnetics Corporation. All rights reserved.
//  Contact Qualnetics Corporation for a copy of the license agreement.
//
//-----------------------------------------------------------------------------
//
/// $Author: rorym $  
/// $Revision: 7483 $  
/// $Date: 2012-10-31 12:14:01 -0700 (Wed, 31 Oct 2012) $  
//
//-----------------------------------------------------------------------------
/// \brief      Random Number Generator.  
//-----------------------------------------------------------------------------

#ifndef _WDK_RANDOM_HPP_
#define _WDK_RANDOM_HPP_

#include <wincrypt.h>

#include <WdkVersion.h>
#include <WdkEhm.h>

class WdkRandom 
{
public:
    WdkRandom( void )
        : m_hProv(NULL)
        , m_pBuf(NULL)
        , m_dwBufSize(0)
        , m_dwAllocChunkSize(256)
    {
    }

    ~WdkRandom( void )
    {
        if (m_hProv)
        {
            CryptReleaseContext(m_hProv, 0);
        }
        Dealloc();
    }

    HRESULT GetDword( __out PDWORD pRand )
    {
        return GetBytes((PBYTE) pRand, sizeof(*pRand));
    }
    
    HRESULT GetWord( __out PWORD pRand )
    {
        return GetBytes((PBYTE) pRand, sizeof(*pRand));
    }
    
    HRESULT GetByte( __out PBYTE pRand )
    {
        return GetBytes(pRand, 1);
    }

    HRESULT GetByte( __out PBYTE pRand, BYTE bMax )
    {
        HRESULT hr = S_OK;

        DWORD dwRand = 0;
        
        CPP(pRand);
        
        hr = GetBytes((PBYTE)&dwRand, sizeof(dwRand));
        CHR(hr);
        
        if(bMax == 0xff)
        {
            hr = (BYTE)(dwRand & 0xff);
        }
        else
        {
            *pRand = (BYTE)(dwRand % (bMax+1));
        }

    Error:
        return hr;
    }
    
    HRESULT GetLong( __out PLONG pRand )
    {
        return GetBytes((PBYTE) pRand, sizeof(*pRand));
    }
    
    HRESULT GetInt( __out PINT pRand )
    {
        return GetBytes((PBYTE) pRand, sizeof(*pRand));
    }
    
    HRESULT GetUchar( __out PUCHAR pRand )
    {
        return GetBytes((PBYTE) pRand, sizeof(*pRand));
    }

    HRESULT GetBool( __out PBOOL pRand )
    {
        HRESULT hr;
        BYTE b = 0;

        CPP(pRand);
        
        hr = GetByte(&b);
        CHR(hr);

        *pRand = BOOLIFY(b & 0x1);
        
    Error:
        return hr;
    }
    
    HRESULT GetBytes( __out_ecount(cbRand) PBYTE pRand, DWORD cbRand )
    {
        HRESULT hr = S_OK;
        BOOL fCrypt;
        CPP(pRand);

        hr = EnsureInit(cbRand * 2);
        CHR(hr);
        
        fCrypt = CryptGenRandom(m_hProv, cbRand, (BYTE *)m_pBuf);
        CBREx(fCrypt, CryptHresult());

        memcpy(pRand, m_pBuf, cbRand);

    Error:
        return hr;
    }

private:

    HRESULT EnsureInit( DWORD dwBufSize )
    {
        HRESULT hr = S_OK;
        BOOL fSuccess;

        if(NULL == m_hProv)
        {
            // Get the handle to the default provider.
            fSuccess = CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, 0);
            if (!fSuccess)
            {
                hr = CryptHresult();
                if (hr !=  NTE_BAD_KEYSET)
                {
                    CHR(hr);
                }
                fSuccess = CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
                CBREx(fSuccess, CryptHresult());
            }
        }

        if(dwBufSize > m_dwBufSize)
        {
            hr = EnsureAlloc(dwBufSize);
            CHR(hr);

            m_dwBufSize = dwBufSize;

            // fill with a random seed
            CeGenRandom(m_dwBufSize, (PBYTE)m_pBuf);
        }
        
    Error:
        return hr;
    }

    HRESULT CryptHresult()
    {
        HRESULT hr = S_OK;
        DWORD gle = GetLastError();
        if(FAILED(gle))
        {
            hr = (HRESULT)gle;
        }
        else if (gle != ERROR_SUCCESS)
        {
            hr = HRESULT_FROM_WIN32(gle);
        }
        return hr;
    }

    VOID Dealloc( VOID )
    {
        LOCALFREE(m_pBuf);
        m_dwBufSize = 0;
    }

    HRESULT EnsureAlloc( DWORD dwSize )
    {
        HRESULT hr = S_OK;
        HLOCAL hAlloc = NULL;
        DWORD dwAlloc;

        if(0 == dwSize)
        {
            // It is not likely the caller intended for a zero length buffer.
            // Assert in debug builds and return S_FALSE since a null buffer 
            // has zero length, so it is technically ok.
            VBR(FALSE);
            hr = S_FALSE;
            goto Done;
        }

        // ensure a minimum size for the total buffer size
        dwAlloc = MAX(dwSize, m_dwAllocChunkSize);
        
        if( m_dwBufSize < dwAlloc || !m_pBuf )
        {
            // alloc in reasonable sized chunks to avoid heap thrash
            if(dwAlloc % m_dwAllocChunkSize)
            {
                dwAlloc = ((dwAlloc / m_dwAllocChunkSize) + 1) * m_dwAllocChunkSize;
                CBRAEx(dwAlloc >= dwSize, E_INVALID_PARAMETER); // buffer too big?
            }

            hAlloc = LocalAlloc(LMEM_FIXED, dwAlloc);
            CPR(hAlloc);

            // dealloc old buffer now that we have a new one
            LOCALFREE(m_pBuf);
            m_pBuf = (PVOID)hAlloc;
            m_dwBufSize = dwAlloc;

            hAlloc = NULL;
        }
        else
        {
            // No Change
            hr = S_FALSE;
        }

    Done:
    Error:
        
        LOCALFREE(hAlloc);
        
        return hr;
    }
    
    HCRYPTPROV          m_hProv;

    PVOID               m_pBuf;
    DWORD               m_dwBufSize;
    DWORD               m_dwAllocChunkSize;
    
};

#endif // _WDK_RANDOM_HPP_

