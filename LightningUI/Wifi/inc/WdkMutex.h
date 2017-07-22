/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkMutex.h#1 $ */
//-----------------------------------------------------------------------------
//
//  Copyright (c) Qualnetics Corporation. All rights reserved.
//  Contact Qualnetics Corporation for a copy of the license agreement.
//
//-----------------------------------------------------------------------------
//
/// $Author: xg01 $  
/// $Revision: #1 $  
/// $Date: 2013/07/11 $  
//
//-----------------------------------------------------------------------------
/// \brief      SmartMutex class
//-----------------------------------------------------------------------------
/// \file

#ifndef _WDKMUTEX_H_
#define _WDKMUTEX_H_
#pragma once

#include <winbase.h>

#include <WdkVersion.h>
#include <WdkEhm.h>

/// \brief Mutex class, use with care.
class SmartMutex
{
public:
    /// \brief Create a SmartMutex object.
    SmartMutex( LPCWSTR pwzName )
        : m_dwCount(0)
    {
        m_hMutex = CreateMutex(NULL, FALSE, pwzName);
    }

    ~SmartMutex(void)
    {
        VBR(m_dwCount <= 1);
        while(m_dwCount)
        {
            Release();
        }
        
        CLOSEHANDLE( m_hMutex );
    }

    BOOL Validate(void) { return BOOLIFY(m_hMutex); }
        
    /// \brief Try to take the mutex.
    HRESULT TryTake( DWORD dwTimeoutMs )
    {
        HRESULT hr = S_OK;
        
        CBREx(m_hMutex, E_INVALID_HANDLE);
        
        DWORD dwWait = WaitForSingleObject(m_hMutex, dwTimeoutMs);

        switch(dwWait)
        {
            case WAIT_OBJECT_0:
                break;
            case WAIT_TIMEOUT:
                CHR(E_TIMEOUT);
            default:
                CHR(E_LASTWIN32ERROR);


        }
        
        ++m_dwCount;

    Error:
        return S_OK;
    }
    
    /// \brief Take the mutex
    HRESULT Take(void)
    {
        return TryTake(INFINITE);
    }

    /// \brief Release the Mutex
    DWORD Release(void)
    {
        VBR(m_dwCount > 0);

        if(m_dwCount)
        {
            --m_dwCount;
            ReleaseMutex( m_hMutex );
        }

        return m_dwCount;
    }

private:
    HANDLE              m_hMutex;
    DWORD               m_dwCount;
    
};

#endif // _WDKMUTEX_H_

