/* SVN $Id: //SoCSW/connectivity/UniFi/CE6/Rel/Built/RoadiHub_7.8.2_P4/WDK/Common/SDK/INC/WdkCritSec.h#1 $ */
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
/// \brief      Critical section enter/leave. Reduces potential lock convoys
/// \remarks 
///             - Original Author :  Rory McLeod
///             - Original Date   :  2008-06-09
//-----------------------------------------------------------------------------
/// \file

#ifndef _QLX_CS_H_
#define _QLX_CS_H_
#pragma once

#include <winbase.h>

#include <WdkVersion.h>
#include <WdkEhm.h>

/// \brief Enter Critical Section with lock convoy protection
#define ENTERCS( /* LPCRITICAL_SECTION */ pCS) { \
    int retries = 0; \
    const int maxRetries = 3; \
    while(!TryEnterCriticalSection(pCS)) { \
        if( retries++ < maxRetries ) { \
            Sleep(0); \
            continue; } \
        EnterCriticalSection(pCS); \
        break; } }

// \brief Leave Critical Section
#define LEAVECS(/* LPCRITICAL_SECTION */ pCS) LeaveCriticalSection(pCS)


/// \brief CS Protected memcpy
#define CS_MEMCPY(_pdest, _psrc, _size, _pcs) { ENTERCS(_pcs); memcpy(_pdest, _psrc, _size); LEAVECS(_pcs); }

/// \brief Critical Section class, use with care.
class SmartCritSec
{
public:
    /// \brief Create a SmartCritSec object.
    SmartCritSec(void) :
        m_pcs(NULL),
        m_dwCount(0)
        {
            m_pcs = &m_cs;
            InitializeCriticalSection( m_pcs );
        }

    /// \brief Create a SmartCritSec object using an existing CS.
    SmartCritSec(LPCRITICAL_SECTION pcs) :
        m_pcs(NULL),
        m_dwCount(0)
        {
            memset(&m_cs, 0, sizeof(m_cs));
            m_pcs = pcs;
        }

    ~SmartCritSec(void)
        {
            CRITICAL_SECTION csNull = {0};
            
            VBR(m_dwCount <= 1);
            while(m_dwCount)
            {
                Leave();
            }
            
            if(memcmp(&m_cs, &csNull, sizeof(csNull)))
                DeleteCriticalSection( m_pcs );
        }

    /// \brief Try to enter the CS. Returns TRUE if entered.
    BOOL TryEnter( void )
        {
            BOOL bRet = TryEnterCriticalSection(m_pcs);

            if(bRet)
                ++m_dwCount;

            return bRet;
        }
    
    /// \brief Enter the CS
    DWORD Enter(void)
        {
            ENTERCS( m_pcs );
            return ++m_dwCount;
        }

    /// \brief Leave the CS
    DWORD Leave(void)
        {
            VBR(m_dwCount > 0);

            if(m_dwCount)
            {
                --m_dwCount;
                LEAVECS( m_pcs );
            }

            return m_dwCount;
        }

    /// \brief Returns TRUE if the current thread is the owner
    BOOL IsCurrentThreadOwner(void)
        {
#ifdef UNDER_CE
            return ( m_pcs->OwnerThread == (HANDLE) GetCurrentThreadId() );
#else
            return ( m_pcs->OwningThread == (HANDLE) GetCurrentThreadId() );
#endif
        }

    /// \brief Returns a pointer to the CS
    LPCRITICAL_SECTION GetCS() 
        { 
            return m_pcs; 
        }
    
private:
    CRITICAL_SECTION    m_cs;
    LPCRITICAL_SECTION  m_pcs;
    DWORD               m_dwCount;
    
};

#endif // _QLX_CS_H_
