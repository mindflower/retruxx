#pragma once

class CAutoLock
{
public:
    CAutoLock(class CCritSec *);
    ~CAutoLock();
protected:
private:
    CCritSec *m_pLock;
};

class CCritSec
{
public:
    void Unlock();
    CCritSec();
    void Lock();
    ~CCritSec();
protected:
private:
    _RTL_CRITICAL_SECTION m_CritSec;
};

class QzCComPtr<IPinConnection>
{
public:
protected:
private:
    QzCComPtr<IPinConnection>();
    operator&();
    operator IPinConnection *();
    ~QzCComPtr<IPinConnection>();
    operator->();
    IPinConnection *p;
};

class CAMThread
{
public:
    CAMThread();
    void Close();
    unsigned long GetRequest();
    void Reply(unsigned long);
    static unsigned long __stdcall InitialThreadProc(void *);
    int ThreadExists() const ;
    int CheckRequest(unsigned long *);
    unsigned long CallWorker(unsigned long);
    static long __fastcall CoInitializeHelper();
    virtual ~CAMThread();
    int Create();
protected:
private:
    CAMThread_vtbl *__vftable /*VFT*/;
    CAMEvent m_EventSend;
    CAMEvent m_EventComplete;
    unsigned int m_dwParam;
    unsigned int m_dwReturnVal;
    void *m_hThread;
    CCritSec m_AccessLock;
    CCritSec m_WorkerLock;
};

class CAMEvent
{
public:
    ~CAMEvent();
    CAMEvent(int);
    operator void *() const ;
    int Check();
    int Wait(unsigned long);
    void Set();
    void Reset();
protected:
private:
    void *m_hEvent;
};
