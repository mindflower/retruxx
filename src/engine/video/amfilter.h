#pragma once

namespace CBaseAllocator
{
    class CSampleList
    {
    public:
        void Add(class CMediaSample *);
        CSampleList();
        void Remove(class CMediaSample *);
        class CMediaSample * RemoveHead();
        int GetCount() const ;
    protected:
    private:
        CMediaSample *m_List;
        int m_nOnList;
    };
}

class CBaseAllocator :  public CUnknown, IMemAllocatorCallbackTemp, CCritSec
{
public:
    void NotifySample();
    virtual long __stdcall Decommit();
    virtual ~CBaseAllocator();
    CBaseAllocator(char *,struct IUnknown *,long *,int,int);
    virtual long __stdcall GetBuffer(struct IMediaSample * *,__int64 *,__int64 *,unsigned long);
    virtual long __stdcall SetProperties(struct _AllocatorProperties *,struct _AllocatorProperties *);
    virtual long __stdcall GetProperties(struct _AllocatorProperties *);
    void SetWaiting();
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
    virtual long __stdcall SetNotify(struct IMemAllocatorNotifyCallbackTemp *);
    virtual unsigned long __stdcall Release();
    virtual long __stdcall ReleaseBuffer(struct IMediaSample *);
    virtual unsigned long __stdcall AddRef();
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
    virtual long __stdcall GetFreeCount(long *);
    virtual long __stdcall Commit();
protected:
    virtual long Alloc();
private:
    static class CMediaSample * & __fastcall NextSample(class CMediaSample *);
    CBaseAllocator::CSampleList m_lFree;
    void *m_hSem;
    int m_lWaiting;
    int m_lCount;
    int m_lAllocated;
    int m_lSize;
    int m_lAlignment;
    int m_lPrefix;
    int m_bChanged;
    int m_bCommitted;
    int m_bDecommitInProgress;
    IMemAllocatorNotifyCallbackTemp *m_pNotify;
    int m_fEnableReleaseCallback;
};

class CBaseFilter :  public CUnknown, IBaseFilter, IAMovieSetup
{
public:
    virtual long __stdcall GetClassID(struct _GUID *);
    virtual long __stdcall Unregister();
    virtual long __stdcall Stop();
    virtual long __stdcall Run(__int64);
    virtual long __stdcall FindPin(unsigned short const *,struct IPin * *);
    CBaseFilter(char *,struct IUnknown *,class CCritSec *,struct _GUID const &,long *);
    CBaseFilter(char const *,struct IUnknown *,class CCritSec *,struct _GUID const &);
    virtual long __stdcall Register();
    virtual long __stdcall QueryFilterInfo(struct _FilterInfo *);
    virtual long __stdcall JoinFilterGraph(struct IFilterGraph *,unsigned short const *);
    virtual long GetPinVersion();
    virtual long __stdcall GetState(unsigned long,enum _FilterState *);
    long NotifyEvent(long,long,long);
    long ReconnectPin(struct IPin *,struct _AMMediaType const *);
    virtual long __stdcall EnumPins(struct IEnumPins * *);
    virtual long __stdcall SetSyncSource(struct IReferenceClock *);
    virtual long __stdcall QueryVendorInfo(unsigned short * *);
    void IncrementPinVersion();
    virtual unsigned long __stdcall Release();
    virtual unsigned long __stdcall AddRef();
    virtual long __stdcall Pause();
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
    virtual struct _AMOVIESETUP_FILTER * GetSetupData();
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
    virtual long __stdcall GetSyncSource(struct IReferenceClock * *);
    virtual long StreamTime(class CRefTime &);
    virtual ~CBaseFilter();
protected:
private:
    _FilterState m_State;
    IReferenceClock *m_pClock;
    CRefTime m_tStart;
    _GUID m_clsid;
    CCritSec *m_pLock;
    unsigned __int16 *m_pName;
    IFilterGraph *m_pGraph;
    IMediaEventSink *m_pSink;
    int m_PinVersion;
};

class CEnumMediaTypes :  public IEnumMediaTypes
{
public:
    virtual long __stdcall Clone(struct IEnumMediaTypes * *);
    virtual long __stdcall Skip(unsigned long);
    virtual long __stdcall Reset();
    virtual long __stdcall Next(unsigned long,struct _AMMediaType * *,unsigned long *);
    virtual ~CEnumMediaTypes();
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    CEnumMediaTypes(class CBasePin *,class CEnumMediaTypes *);
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
protected:
private:
    int AreWeOutOfSync();
    int m_Position;
    CBasePin *m_pPin;
    int m_Version;
    int m_cRef;
};

class CEnumPins :  public IEnumPins
{
public:
    virtual long __stdcall Reset();
    CEnumPins(class CBaseFilter *,class CEnumPins *);
    virtual unsigned long __stdcall AddRef();
    virtual long __stdcall Skip(unsigned long);
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
    virtual long __stdcall Next(unsigned long,struct IPin * *,unsigned long *);
    virtual ~CEnumPins();
    virtual long __stdcall Clone(struct IEnumPins * *);
    virtual unsigned long __stdcall Release();
protected:
private:
    int AreWeOutOfSync();
    long __stdcall Refresh();
    int m_Position;
    int m_PinCount;
    CBaseFilter *m_pFilter;
    int m_Version;
    int m_cRef;
    CGenericList<CBasePin> m_PinCache;
};

class CBasePin :  public CUnknown, IPin, IQualityControl
{
public:
    virtual ~CBasePin();
    virtual long CheckConnect(struct IPin *);
    CBasePin(char *,class CBaseFilter *,class CCritSec *,long *,unsigned short const *,enum _PinDirection);
    virtual long Run(__int64);
    virtual long __stdcall QueryId(unsigned short * *);
    int IsConnected();
    virtual long __stdcall Notify(struct IBaseFilter *,struct tagQuality);
    unsigned short * Name();
    virtual long Active();
    virtual long __stdcall QueryAccept(struct _AMMediaType const *);
    virtual long __stdcall Disconnect();
    virtual long CompleteConnect(struct IPin *);
    virtual long GetMediaType(int,class CMediaType *);
    virtual long __stdcall SetSink(struct IQualityControl *);
    virtual long __stdcall ConnectedTo(struct IPin * *);
    virtual long __stdcall EnumMediaTypes(struct IEnumMediaTypes * *);
    virtual long __stdcall QueryInternalConnections(struct IPin * *,unsigned long *);
    virtual long __stdcall EndOfStream();
    virtual long SetMediaType(class CMediaType const *);
    virtual unsigned long __stdcall NonDelegatingRelease();
    virtual long __stdcall NewSegment(__int64,__int64,double);
    virtual long BreakConnect();
    virtual long __stdcall ReceiveConnection(struct IPin *,struct _AMMediaType const *);
    virtual long __stdcall ConnectionMediaType(struct _AMMediaType *);
    bool CanReconnectWhenActive();
    virtual long __stdcall QueryDirection(enum _PinDirection *);
    struct IPin * GetConnected();
    virtual long __stdcall Connect(struct IPin *,struct _AMMediaType const *);
    virtual long Inactive();
    int IsStopped();
    virtual unsigned long __stdcall NonDelegatingAddRef();
    void IncrementTypeVersion();
    void SetReconnectWhenActive(bool);
    virtual long GetMediaTypeVersion();
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
    virtual long __stdcall QueryPinInfo(struct _PinInfo *);
protected:
    long TryMediaTypes(struct IPin *,class CMediaType const *,struct IEnumMediaTypes *);
    long __stdcall DisconnectInternal();
    void DisplayPinInfo(struct IPin *);
    long AgreeMediaType(struct IPin *,class CMediaType const *);
    long AttemptConnection(struct IPin *,class CMediaType const *);
    void DisplayTypeInfo(struct IPin *,class CMediaType const *);
private:
    unsigned __int16 *m_pName;
    IPin *m_Connected;
    _PinDirection m_dir;
    CCritSec *m_pLock;
    bool m_bRunTimeError;
    bool m_bCanReconnectWhenActive;
    bool m_bTryMyTypesFirst;
    CBaseFilter *m_pFilter;
    IQualityControl *m_pQSink;
    int m_TypeVersion;
    CMediaType m_mt;
    CRefTime m_tStart;
    CRefTime m_tStop;
    long double m_dRate;
};

class CBaseInputPin :  public CBasePin, IMemInputPin
{
public:
    long PassNotify(struct tagQuality &);
    virtual long __stdcall NotifyAllocator(struct IMemAllocator *,int);
    int IsFlushing();
    struct tagAM_SAMPLE2_PROPERTIES * SampleProps();
    virtual long __stdcall GetAllocator(struct IMemAllocator * *);
    virtual long BreakConnect();
    virtual long __stdcall GetAllocatorRequirements(struct _AllocatorProperties *);
    virtual unsigned long __stdcall AddRef();
    CBaseInputPin(char *,class CBaseFilter *,class CCritSec *,long *,unsigned short const *);
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
    virtual long CheckStreaming();
    virtual long __stdcall Notify(struct IBaseFilter *,struct tagQuality);
    virtual long __stdcall Receive(struct IMediaSample *);
    virtual long __stdcall EndFlush();
    virtual long __stdcall BeginFlush();
    virtual ~CBaseInputPin();
    virtual long __stdcall ReceiveMultiple(struct IMediaSample * *,long,long *);
    virtual long Inactive();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall ReceiveCanBlock();
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
protected:
private:
    IMemAllocator *m_pAllocator;
    unsigned __int8 m_bReadOnly;
    unsigned __int8 m_bFlushing;
    tagAM_SAMPLE2_PROPERTIES m_SampleProps;
};

class CDynamicOutputPin :  public CBaseOutputPin, IPinFlowControl
{
public:
    virtual void StopUsingOutputPin();
    virtual long __stdcall Block(unsigned long,void *);
    long ChangeOutputFormat(struct _AMMediaType const *,__int64,__int64,double);
    virtual long StartUsingOutputPin();
    long DynamicReconnect(class CMediaType const *);
    virtual long DeliverBeginFlush();
    virtual long Active();
    virtual unsigned long __stdcall AddRef();
    long ChangeMediaType(class CMediaType const *);
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
    virtual long __stdcall Disconnect();
    virtual bool StreamingThreadUsingOutputPin();
    virtual ~CDynamicOutputPin();
    CDynamicOutputPin(char *,class CBaseFilter *,class CCritSec *,long *,unsigned short const *);
    virtual long DeliverEndFlush();
    virtual long CompleteConnect(struct IPin *);
    void SetConfigInfo(struct IGraphConfig *,void *);
    virtual long Inactive();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
protected:
    long AsynchronousBlockOutputPin(void *);
    static long __fastcall WaitEvent(void *);
    long SynchronousBlockOutputPin();
    void BlockOutputPin();
    long UnblockOutputPin();
private:
    long ChangeMediaTypeHelper(class CMediaType const *);
    long Initialize();
    CCritSec m_BlockStateLock;
    void *m_hUnblockOutputPinEvent;
    void *m_hNotifyCallerPinBlockedEvent;
    CDynamicOutputPin::BLOCK_STATE m_BlockState;
    unsigned int m_dwBlockCallerThreadID;
    unsigned int m_dwNumOutstandingOutputPinUsers;
    void *m_hStopEvent;
    IGraphConfig *m_pGraphConfig;
    int m_bPinUsesReadOnlyAllocator;
};
