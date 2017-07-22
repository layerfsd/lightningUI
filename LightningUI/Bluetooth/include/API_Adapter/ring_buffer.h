//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft shared
// source or premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license agreement,
// you are not authorized to use this source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the SOURCE.RTF on your install media or the root of your tools installation.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//

#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

#include <windows.h>
#include <list.hxx>
#include <block_allocator.hxx>
#include "critical_section.h"
//#include "bluetooth_debug.h"


#define RING_BUFFER_SIZE              0x10000   /* 64KB */
#define RING_LIST_NODE_ALLOC          ce::singleton_allocator< ce::fixed_block_allocator<10>, sizeof(RING_NODE*) >

class RING_NODE 
{
    public:
        BYTE *m_pReadBuffer;
        UINT m_uiReadyToRead;

        UINT m_uiWriteRemaining;
        BYTE *m_pWriteBuffer;

        UINT m_uiBlockSize;
        VOID *m_pHead;
};

typedef ce::list<RING_NODE*> RingNodeList;

class RingBuffer 
{
    public:
        RingBuffer(DWORD bufSize = RING_BUFFER_SIZE, DWORD maxRingNode = 3);
        ~RingBuffer();

        HRESULT Purge();
        HRESULT Read(BYTE *pDest, UINT uiRequested, UINT *puiReturned);
        HRESULT Write(const BYTE *pSrc, UINT uiSize, UINT *puiWritten);

        UINT BytesReadyToRead();

    private:
        //
        // Write to the beginning, read from the end
        //    (front() = newest from wire)
        //    (last()  = oldest data)
        RingNodeList m_BufferList;
        DWORD m_dwBufferSize;
        DWORD m_dwMaxRingNode;
        
        UINT m_uiReadyToRead;
        CRITICAL_SECTION m_myLock;
};

#endif
