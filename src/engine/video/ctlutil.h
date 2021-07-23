#pragma once

class COARefTime :  public CRefTime
{
public:
    class COARefTime & operator=(__int64 const &);
    class COARefTime & operator=(double const &);
    COARefTime(class CRefTime);
    COARefTime();
    COARefTime(double);
    operator double();
    operator __int64();
protected:
private:
};

class CPosPassThru :  public IMediaSeeking, CMediaPosition
{
public:
    virtual long __stdcall GetAvailable(__int64 *,__int64 *);
    virtual long __stdcall SetPositions(__int64 *,unsigned long,__int64 *,unsigned long);
    CPosPassThru(char const *,struct IUnknown *,long *,struct IPin *);
    virtual long __stdcall CanSeekForward(long *);
    virtual long __stdcall GetPositions(__int64 *,__int64 *);
    virtual long __stdcall ConvertTimeFormat(__int64 *,struct _GUID const *,__int64,struct _GUID const *);
    virtual long __stdcall CheckCapabilities(unsigned long *);
    virtual long __stdcall IsFormatSupported(struct _GUID const *);
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
    virtual unsigned long __stdcall AddRef();
    virtual long __stdcall put_CurrentPosition(double);
    virtual unsigned long __stdcall Release();
    virtual long __stdcall SetRate(double);
    virtual long __stdcall IsUsingTimeFormat(struct _GUID const *);
    virtual long __stdcall GetDuration(__int64 *);
    virtual long __stdcall GetStopPosition(__int64 *);
    virtual long __stdcall get_Duration(double *);
    virtual long __stdcall SetTimeFormat(struct _GUID const *);
    virtual long __stdcall get_CurrentPosition(double *);
    virtual long __stdcall CanSeekBackward(long *);
    virtual long __stdcall GetRate(double *);
    virtual long __stdcall QueryPreferredFormat(struct _GUID *);
    virtual long __stdcall GetCurrentPosition(__int64 *);
    virtual long __stdcall get_Rate(double *);
    virtual long __stdcall get_PrerollTime(double *);
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
    virtual long __stdcall put_Rate(double);
    virtual long __stdcall put_PrerollTime(double);
    virtual long __stdcall GetPreroll(__int64 *);
    virtual long __stdcall put_StopTime(double);
    virtual long __stdcall GetCapabilities(unsigned long *);
    virtual long __stdcall get_StopTime(double *);
    virtual long __stdcall GetTimeFormatA(struct _GUID *);
    virtual long GetMediaTime(__int64 *,__int64 *);
protected:
private:
    long GetSeekingLongLong(long (__stdcall IMediaSeeking::*)(__int64 *),__int64 *);
    long GetPeer(struct IMediaPosition * *);
    long GetPeerSeeking(struct IMediaSeeking * *);
    IPin *m_pPin;
};

class CCmdQueue
{
public:
    virtual long GetDueCommand(class CDeferredCommand * *,long);
    virtual long Insert(class CDeferredCommand *);
    int CheckTime(class CRefTime,int);
    virtual long Remove(class CDeferredCommand *);
    virtual long Run(__int64);
    virtual long SetSyncSource(struct IReferenceClock *);
    CCmdQueue();
    virtual long EndRun();
    virtual ~CCmdQueue();
    virtual long New(class CDeferredCommand * *,struct IUnknown *,double,struct _GUID *,long,short,long,struct tagVARIANT *,struct tagVARIANT *,short *,int);
    virtual long GetCommandDueFor(__int64,class CDeferredCommand * *);
protected:
    void SetTimeAdvise();
private:
    CCmdQueue_vtbl *__vftable /*VFT*/;
    CCritSec m_Lock;
    CGenericList<CDeferredCommand> m_listPresentation;
    CGenericList<CDeferredCommand> m_listStream;
    CAMEvent m_evDue;
    unsigned int m_dwAdvise;
    CRefTime m_tCurrentAdvise;
    IReferenceClock *m_pClock;
    int m_bRunning;
    CRefTime m_StreamTimeOffset;
};

class CDeferredCommand :  public CUnknown, IDeferredCommand
{
public:
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
    class CRefTime GetTime();
    virtual long __stdcall Confidence(long *);
    CDeferredCommand(class CCmdQueue *,struct IUnknown *,long *,struct IUnknown *,double,struct _GUID *,long,short,long,struct tagVARIANT *,struct tagVARIANT *,short *,int);
    virtual long __stdcall Cancel();
    struct _GUID const & GetIID();
    struct tagVARIANT * GetResult();
    struct tagDISPPARAMS * GetParams();
    int IsStreamTime();
    long Invoke();
    virtual long __stdcall Postpone(double);
    virtual unsigned long __stdcall Release();
    short GetFlags();
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
    virtual unsigned long __stdcall AddRef();
    long GetMethod();
    virtual long __stdcall GetHResult(long *);
protected:
private:
    CCmdQueue *m_pQueue;
    IUnknown *m_pUnk;
    __int64 m_time;
    _GUID *m_iid;
    int m_dispidMethod;
    __int16 m_wFlags;
    tagVARIANT *m_pvarResult;
    int m_bStream;
    CDispParams m_DispParams;
    int m_DispId;
    CBaseDispatch m_Dispatch;
    HRESULT m_hrResult;
};

class CBaseDispatch
{
public:
    long __stdcall GetTypeInfo(struct _GUID const &,unsigned int,unsigned long,struct ITypeInfo * *);
    CBaseDispatch();
    long __stdcall GetTypeInfoCount(unsigned int *);
    long __stdcall GetIDsOfNames(struct _GUID const &,unsigned short * *,unsigned int,unsigned long,long *);
    ~CBaseDispatch();
protected:
private:
    ITypeInfo *m_pti;
};
