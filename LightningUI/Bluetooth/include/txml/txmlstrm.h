
#ifndef __TXMLSTRM_H__
#define __TXMLSTRM_H__

#pragma once

class TXmlInStream
{
public:
    virtual int Read(char *psz, int bytes) = 0;
    virtual TCHAR Peek() = 0;
    virtual TCHAR Read() = 0;
    virtual bool EndOfStream() = 0;
};

class TXmlOutStream
{
public:
    virtual bool Write(LPCTSTR psz, int bytes = -1) = 0;
    virtual bool Write(TCHAR ch) = 0;
};


class TXmlInTextStream : public TXmlInStream
{
public:
    TXmlInTextStream(LPCTSTR text, int bytes = -1)
    {
        TXMLASSERT(text != NULL);

        m_pszTextPtr = text;
        m_pszReadPtr = text;
        m_nTextBytes = bytes == -1 ? _tcslen(text) : bytes;

        TXMLASSERT(m_nTextBytes > 0);
    }

    virtual int Read(LPTSTR psz, int bytes)
    {
        TXMLASSERT(psz != NULL);
        TXMLASSERT(bytes > 0);
        TXMLASSERT(!EndOfStream());

        bytes = min(bytes, m_pszReadPtr - m_pszTextPtr);
        memcpy(psz, m_pszReadPtr, bytes);
        m_pszReadPtr += bytes;

        return bytes;
    }
    virtual TCHAR Peek()
    {
        TXMLASSERT(!EndOfStream());

        return *m_pszReadPtr;
    }
    virtual TCHAR Read()
    {
        TXMLASSERT(!EndOfStream());

        return *m_pszReadPtr++;
    }
    virtual bool EndOfStream()
    {
        return m_pszReadPtr > m_pszTextPtr + m_nTextBytes;
    }

protected:
    LPCTSTR	m_pszTextPtr;
    LPCTSTR	m_pszReadPtr;
    int			m_nTextBytes;
};

class TXmlOutTextStream : public TXmlOutStream
{
public:
    TXmlOutTextStream(int initbytes = 8192)
    {
        m_pszBufferPtr = (LPTSTR)malloc(initbytes);
        m_pszWritePtr = m_pszBufferPtr;
        TXMLASSERT(m_pszBufferPtr != NULL);

        m_nAllocBytes = initbytes;
    }

    ~TXmlOutTextStream()
    {
        free(m_pszBufferPtr);
    }

    LPCTSTR GetStringPtr() const
    {
        return m_pszBufferPtr;
    }
    int GetLength() const
    {
        return m_pszWritePtr - m_pszBufferPtr;
    }

    virtual bool Write(LPCTSTR psz, int bytes = -1)
    {
        if (bytes == -1)
            bytes = _tcslen(psz);

        if (bytes == 0)
            return true;

        if (! GrowBuffer(bytes + 1))
            return false;

        memcpy(m_pszWritePtr, psz, bytes);
        m_pszWritePtr += bytes;
        *m_pszWritePtr = '\0';

        return true;
    }
    virtual bool Write(TCHAR ch)
    {
        return Write(&ch, 1);
    }

    bool GrowBuffer(int bytes)
    {
        int nBufferSize = GetLength();
        int nNewSize = bytes + nBufferSize;

        if (nNewSize > m_nAllocBytes)
        {
            nNewSize = (nNewSize / 8192 + 1) * 8192;
            m_pszBufferPtr = (LPTSTR)realloc(m_pszBufferPtr, nNewSize);
            TXMLASSERT(m_pszBufferPtr != NULL);
            m_pszWritePtr = m_pszBufferPtr + nBufferSize;
            m_nAllocBytes = nNewSize;
        }

        return m_pszBufferPtr != NULL;
    }

protected:
    LPTSTR	m_pszBufferPtr;
    LPTSTR	m_pszWritePtr;
    int		m_nAllocBytes;
};

#endif

