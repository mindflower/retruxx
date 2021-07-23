#pragma once

class CBaseRenderer :  public CBaseFilter
{
public:
    void TimerCallback();
    virtual void OnRenderEnd(struct IMediaSample *);
    long NotifyEndOfStream();
    virtual long Receive(struct IMediaSample *);
    virtual void OnWaitStart();
    void WaitForReceiveToComplete();
    int IsStreaming();
    virtual long OnStopStreaming();
    virtual long WaitForRenderTime();
    int OnDisplayChange();
    void ResetEndOfStreamTimer();
    virtual long BeginFlush();
    int IsEndOfStreamDelivered();
    virtual long EndFlush();
    virtual long CompleteConnect(struct IPin *);
    virtual long GetSampleTimes(struct IMediaSample *,__int64 *,__int64 *);
    void NotReady();
    virtual void OnWaitEnd();
    virtual long Render(struct IMediaSample *);
    virtual class CBasePin * GetPin(int);
    virtual struct IMediaSample * GetCurrentSample();
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
    void SetRepaintStatus(int);
    virtual long OnStartStreaming();
    virtual long Inactive();
    virtual long CompleteStateChange(enum _FilterState);
    void Ready();
    void SendRepaint();
    CBaseRenderer(struct _GUID const &,char *,struct IUnknown *,long *);
    virtual long SendEndOfStream();
    virtual long StartStreaming();
    virtual long GetMediaPositionInterface(struct _GUID const &,void * *);
    virtual long CancelNotification();
    virtual long __stdcall Stop();
    virtual long __stdcall Run(__int64);
    void SendNotifyWindow(struct IPin *,struct HWND__ *);
    virtual long BreakConnect();
    virtual long PrepareReceive(struct IMediaSample *);
    virtual long ShouldDrawSampleNow(struct IMediaSample *,__int64 *,__int64 *);
    virtual long ResetEndOfStream();
    virtual long SetMediaType(class CMediaType const *);
    virtual long __stdcall FindPin(unsigned short const *,struct IPin * *);
    void SetAbortSignal(int);
    virtual long __stdcall GetState(unsigned long,enum _FilterState *);
    virtual long StopStreaming();
    virtual ~CBaseRenderer();
    int IsEndOfStream();
    virtual void OnRenderStart(struct IMediaSample *);
    virtual long EndOfStream();
    virtual int HaveCurrentSample();
    virtual long SourceThreadCanWait(int);
    virtual int ScheduleSample(struct IMediaSample *);
    virtual int GetPinCount();
    enum _FilterState GetRealState();
    void SignalTimerFired();
    virtual void OnReceiveFirstSample(struct IMediaSample *);
    virtual long Active();
    virtual long __stdcall Pause();
    virtual long ClearPendingSample();
    virtual void PrepareRender();
protected:
private:
    ResetEndOfStreamTimer();
    CRendererPosPassThru *m_pPosition;
    CAMEvent m_RenderEvent;
    CAMEvent m_ThreadSignal;
    CAMEvent m_evComplete;
    int m_bAbort;
    int m_bStreaming;
    unsigned int m_dwAdvise;
    IMediaSample *m_pMediaSample;
    int m_bEOS;
    int m_bEOSDelivered;
    CRendererInputPin *m_pInputPin;
    CCritSec m_InterfaceLock;
    CCritSec m_RendererLock;
    IQualityControl *m_pQSink;
    int m_bRepaintStatus;
    volatile int m_bInReceive;
    __int64 m_SignalTime;
    unsigned int m_EndOfStreamTimer;
    CCritSec m_ObjectCreationLock;
};

class CRendererInputPin :  public CBaseInputPin
{
public:
    virtual long Active();
    virtual long __stdcall EndOfStream();
    struct IMemAllocator * Allocator() const ;
    virtual long BreakConnect();
    virtual long SetMediaType(class CMediaType const *);
    virtual long CheckMediaType(class CMediaType const *);
    virtual long __stdcall QueryId(unsigned short * *);
    virtual long Inactive();
    virtual long CompleteConnect(struct IPin *);
    virtual long __stdcall BeginFlush();
    virtual long __stdcall EndFlush();
    virtual long __stdcall Receive(struct IMediaSample *);
    CRendererInputPin(class CBaseRenderer *,long *,unsigned short const *);
protected:
private:
    CBaseRenderer *m_pRenderer;
};

class CBaseVideoRenderer :  public CBaseRenderer, IQualProp, IQualityControl
{
public:
    virtual long __stdcall get_DevSyncOffset(int *);
    virtual void OnDirectRender(struct IMediaSample *);
    virtual long __stdcall get_AvgSyncOffset(int *);
    virtual void OnWaitStart();
    virtual long SendQuality(__int64,__int64);
    CBaseVideoRenderer(struct _GUID const &,char *,struct IUnknown *,long *);
    virtual long __stdcall NonDelegatingQueryInterface(struct _GUID const &,void * *);
    virtual unsigned long __stdcall AddRef();
    virtual long __stdcall get_FramesDrawn(int *);
    void ThrottleWait();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall get_AvgFrameRate(int *);
    virtual int ScheduleSample(struct IMediaSample *);
    virtual long __stdcall JoinFilterGraph(struct IFilterGraph *,unsigned short const *);
    virtual void RecordFrameLateness(int,int);
    virtual ~CBaseVideoRenderer();
    long GetStdDev(int,int *,__int64,__int64);
    virtual long __stdcall get_Jitter(int *);
    virtual long __stdcall get_FramesDroppedInRenderer(int *);
    virtual long ShouldDrawSampleNow(struct IMediaSample *,__int64 *,__int64 *);
    virtual long __stdcall Notify(struct IBaseFilter *,struct tagQuality);
    virtual long __stdcall QueryInterface(struct _GUID const &,void * *);
    virtual void OnWaitEnd();
    virtual long OnStartStreaming();
    virtual long OnStopStreaming();
    virtual void OnRenderStart(struct IMediaSample *);
    virtual long ResetStreamingTimes();
    virtual void OnRenderEnd(struct IMediaSample *);
    virtual long __stdcall SetSink(struct IQualityControl *);
    void PreparePerformanceData(int,int);
protected:
private:
    int m_nNormal;
    int m_bSupplierHandlingQuality;
    int m_trThrottle;
    int m_trRenderAvg;
    int m_trRenderLast;
    int m_tRenderStart;
    int m_trEarliness;
    int m_trTarget;
    int m_trWaitAvg;
    int m_trFrameAvg;
    int m_trDuration;
    __int64 m_trRememberStampForPerf;
    int m_cFramesDropped;
    int m_cFramesDrawn;
    __int64 m_iTotAcc;
    __int64 m_iSumSqAcc;
    __int64 m_trLastDraw;
    __int64 m_iSumSqFrameTime;
    __int64 m_iSumFrameTime;
    int m_trLate;
    int m_trFrame;
    int m_tStreamingStart;
};
