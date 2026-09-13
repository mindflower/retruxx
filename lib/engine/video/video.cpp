#include "video.h"
#include <config.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include "Wmsdk.h"
#include <stdexcept>
#include <core/log.h>

namespace 
{
    IMediaEvent* g_pME = nullptr;
    IGraphBuilder* g_pGB = nullptr;
    IMediaControl* g_pMC = nullptr;
    IMediaPosition * g_pMP = nullptr;
    IFileSourceFilter* g_pFileSource = nullptr;
    HANDLE g_Event1 = NULL;
    HANDLE g_Event2 = NULL;

    HRESULT CreateFilter(const IID& clsid, IBaseFilter** filter)
    {
        auto hr = ::CoCreateInstance(
            clsid, 
            0,
            CLSCTX_INPROC_SERVER,
            IID_IBaseFilter,
            reinterpret_cast<LPVOID*>(filter)
        );
        if (FAILED(hr))
        {
            M3D_LOG_INFO("CreateFilter: Failed to create filter!  hr = " + CStr(hr));
        }
        return hr;
    }

    // The only key provider there is. AddKeyProvider takes a reference on it
    // that is never given back, so its count never drops to zero.
    CKeyProvider prov;

    HRESULT AddKeyProvider(IGraphBuilder* pGraph)
    {
        // RVA 0x6A3FD0
        prov.AddRef();

        IObjectWithSite* pObjectWithSite = nullptr;
        HRESULT hr = pGraph->QueryInterface(IID_IObjectWithSite, reinterpret_cast<void**>(&pObjectWithSite));
        if (FAILED(hr))
        {
            return hr;
        }

        hr = pObjectWithSite->SetSite(&prov);
        pObjectWithSite->Release();
        return hr;
    }

    HRESULT RenderOutputPins(IGraphBuilder* pGB, IBaseFilter* pFilter)
    {
        //TODO: check correctness
        IEnumPins* pEnumPin = NULL;
        IPin* pConnectedPin = NULL, * pPin = NULL;
        PIN_DIRECTION PinDirection;
        ULONG     ulFetched;

        // Enumerate all pins on the filter
        auto hr = pFilter->EnumPins(&pEnumPin);

        if (SUCCEEDED(hr))
        {
            // Step through every pin, looking for the output pins
            while ((hr = pEnumPin->Next(1L, &pPin, &ulFetched)) == S_OK)
            {
                // Is this pin connected?  We're not interested in connected pins.
                hr = pPin->ConnectedTo(&pConnectedPin);
                if (pConnectedPin)
                {
                    pConnectedPin->Release();
                    pConnectedPin = NULL;
                }

                // If this pin is not connected, render it.
                if (hr == VFW_E_NOT_CONNECTED)
                {
                    hr = pPin->QueryDirection(&PinDirection);
                    if (hr == S_OK && PinDirection == PINDIR_OUTPUT)
                    {
                        hr = pGB->Render(pPin);
                        if (FAILED(hr))
                        {
                            M3D_LOG_INFO("FAILED(hr=" + CStr(hr) + ") in pGB->Render");
                        }
                    }
                }
                pPin->Release();

                // If there was an error, stop enumerating
                if (FAILED(hr))
                    break;
            }
        }
        else
        { 
            M3D_LOG_INFO("FAILED(hr=" + CStr(hr) + ") in EnumPins");
        }

        // Release pin enumerator
        pEnumPin->Release();
        return hr;
    }

    HRESULT SetAudioVolume(int volume)
    {
        if (g_pGB == nullptr)
        {
            return 0;
        }
        IBasicAudio* pBA = nullptr;
        auto hr = g_pGB->QueryInterface(IID_IBasicAudio, reinterpret_cast<LPVOID*>(&pBA));
        if (FAILED(hr))
        {
            return 0;
        }
        long lVolume = 0;
        hr = pBA->get_Volume(&lVolume);
        if (hr == -2147467263)
        {
            pBA->Release();
            return hr;
        }
        if (SUCCEEDED(hr))
        {
            hr = pBA->put_Volume(volume);
        }
        pBA->Release();
        return hr;
    }
}

CKeyProvider::CKeyProvider()
{
    // RVA 0x6A4B40
    m_cRef = 0;
}

ULONG CKeyProvider::AddRef()
{
    // RVA 0x6A3F30 - NOTE: returns the count read back after the increment
    // rather than InterlockedIncrement's own result.
    InterlockedIncrement(&m_cRef);
    return m_cRef;
}

ULONG CKeyProvider::Release()
{
    // RVA 0x6A3F50
    LONG const count = InterlockedDecrement(&m_cRef);
    if (count == 0)
    {
        // NOTE: frees the object as if it were heap allocated, but the only
        // instance is the file-level `prov`. Its count never reaches zero (see
        // AddKeyProvider), so this never runs.
        delete this;
    }
    return count;
}

HRESULT CKeyProvider::QueryInterface(const _GUID& riid, void** ppv)
{
    // RVA 0x6A3F80 - NOTE: unlike the SDK sample this is based on, ppv is not
    // checked for null.
    if (!IsEqualGUID(riid, IID_IServiceProvider) && !IsEqualGUID(riid, IID_IUnknown))
    {
        return E_NOINTERFACE;
    }
    *ppv = this;
    AddRef();
    return S_OK;
}

HRESULT CKeyProvider::QueryService(const _GUID& siid, const _GUID& riid, void** ppv)
{
    // RVA 0x6A4B50 - the ASF reader asks for its certificate under IWMReader's
    // IID; nothing else is served.
    if (!IsEqualGUID(siid, __uuidof(IWMReader)) || !IsEqualGUID(riid, IID_IUnknown))
    {
        return E_NOINTERFACE;
    }

    // The shipped code answers with WMCreateCertificate(&punk) and hands the
    // certificate back through ppv. NOTE: that function came from the static
    // wmstub.lib of the discontinued Windows Media Format 9 SDK; neither the
    // current Windows SDK nor wmvcore.dll provides it, so it cannot be called
    // here. The request is failed the same way the shipped code fails when
    // WMCreateCertificate itself fails. The provider is still installed on the
    // graph, so this only matters if the ASF reader actually asks for one.
    HRESULT const hr = E_NOTIMPL;
    M3D_LOG_INFO("CKeyProvider::QueryService failed to create certificate!  hr = " + CStr::format_("%x", hr));
    return hr;
}

// NOTE: only CTextureRenderer's constructor and destructor touch this in the
// shipped build, and neither allocates it, so it is always null.
unsigned int* g_Tex = nullptr;
m3d::rend::TexHandle g_pTexture;

namespace m3d
{
    CTextureRenderer* g_pRenderer = nullptr;

    HRESULT CTextureRenderer::SetMediaType(CMediaType const* pmt)
    {
        // RVA 0x6A44B0 - called once the graph has agreed a format, so this is
        // where the texture the frames are copied into gets created.
        auto const* header = reinterpret_cast<VIDEOINFOHEADER const*>(pmt->pbFormat);
        m_lVidWidth = header->bmiHeader.biWidth;
        // A negative height marks a top-down bitmap; only the magnitude is kept,
        // and DoRenderSample always treats the frame as bottom-up.
        m_lVidHeight = abs(header->bmiHeader.biHeight);
        m_lVidPitch = 4 * m_lVidWidth;

        // The texture is the smallest power of two that holds the frame, and
        // never smaller than 2 on either side.
        int texWidth = 2;
        while (texWidth < m_lVidWidth)
        {
            texWidth *= 2;
        }
        int texHeight = 2;
        while (texHeight < m_lVidHeight)
        {
            texHeight *= 2;
        }

        g_pTexture = M3D_RENDERER->AddDynamicTexture("$TexMedia", texWidth, texHeight, rend::TM_DTF_RGBA8888_VIDEOFRAME);
        M3D_RENDERER->SetTextureParameter(g_pTexture, rend::TM_WRAP_S, 3);
        M3D_RENDERER->SetTextureParameter(g_pTexture, rend::TM_WRAP_T, 3);
        return S_OK;
    }

    HRESULT CTextureRenderer::CheckMediaType(CMediaType const* pmt)
    {
        // RVA 0x6A3D90 - only uncompressed 32-bit RGB video is accepted, so the
        // graph has to insert a colour converter in front of this filter.
        if (!pmt)
        {
            return E_POINTER;
        }
        if (memcmp(&pmt->formattype, &FORMAT_VideoInfo, sizeof(GUID)))
        {
            return E_INVALIDARG;
        }
        if (memcmp(&pmt->majortype, &MEDIATYPE_Video, sizeof(GUID)))
        {
            return VFW_E_TYPE_NOT_ACCEPTED;
        }
        if (memcmp(&pmt->subtype, &MEDIASUBTYPE_RGB32, sizeof(GUID)))
        {
            return VFW_E_TYPE_NOT_ACCEPTED;
        }
        return S_OK;
    }

    void CTextureRenderer::GetVideoDims(int& w, int& h)
    {
        // RVA 0x6A3C30
        w = m_lVidWidth;
        h = m_lVidHeight;
    }

    HRESULT CTextureRenderer::DoRenderSample(IMediaSample* pSample)
    {
        // RVA 0x6A3E00 - runs on the DirectShow streaming thread. g_Event2 is
        // set while the game wants frames and g_Event1 tells mVideoPlayer::Update
        // that a new one has been copied in; if the game isn't ready within 30ms
        // the frame is simply dropped.
        if (::WaitForSingleObject(g_Event2, 30) != WAIT_OBJECT_0)
        {
            return S_OK;
        }

        BYTE* pBmpBuffer = nullptr;
        pSample->GetPointer(&pBmpBuffer);

        int texWidth = 0;
        int texHeight = 0;
        M3D_RENDERER->GetDims(g_pTexture, texWidth, texHeight);
        int const texRowBytes = texWidth * 4;

        int lockedPitch = 0;
        auto* dst = static_cast<BYTE*>(
            M3D_RENDERER->LockTexture(g_pTexture, rend::TM_DTF_RGBA8888_VIDEOFRAME, lockedPitch, 0));

        // DIBs are stored bottom-up, so the rows are copied starting from the
        // last one to turn the image the right way up.
        // NOTE: the destination advances by the texture's width rather than by
        // the pitch LockTexture reports, which is ignored. The two agree for the
        // power-of-two RGBA textures made in SetMediaType.
        BYTE const* src = pBmpBuffer + 4 * m_lVidWidth * (m_lVidHeight - 1);
        for (unsigned row = 0; row < static_cast<unsigned>(m_lVidHeight); ++row)
        {
            memcpy(dst, src, m_lVidPitch);
            src -= m_lVidPitch;
            dst += 4 * (texRowBytes / 4);
        }

        M3D_RENDERER->UnlockTexture(g_pTexture);
        ::SetEvent(g_Event1);
        return S_OK;
    }

    CTextureRenderer::CTextureRenderer(IUnknown* pUnk, HRESULT* phr) :
        CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer), NULL, pUnk, phr)
    {
        // RVA 0x6A3C50 - NOTE: this overwrites whatever the base class stored in
        // *phr, so a failure inside CBaseVideoRenderer is reported as success.
        m_bUseDynamicTextures = FALSE;
        g_Tex = nullptr;
        if (phr)
        {
            *phr = S_OK;
        }
    }

    CTextureRenderer::~CTextureRenderer()
    {
        // RVA 0x6A3D20
        M3D_RENDERER->ReleaseTexture(g_pTexture);
        if (g_Tex)
        {
            delete[] g_Tex;
        }
        g_Tex = nullptr;
    }

    void CTextureRenderer::GetTextureDims(int& w, int& h)
    {
        // RVA 0x6A3C00 - the texture's size, which is the frame size rounded up
        // to powers of two.
        M3D_RENDERER->GetDims(g_pTexture, w, h);
    }

    mVideoPlayer::~mVideoPlayer()
    {
        if (IsVideoPlaing())
        {
            Stop();
        }
    }

    int mVideoPlayer::Init()
    {
        return 1;
    }

    // The shipped build checks most of Play's calls with a macro that evaluates
    // its argument a second time to put the failing hr in the log, so a call
    // that fails is made twice. Kept as is.
#define VIDEO_JIF(expr)                                                                   \
    hr = (expr);                                                                          \
    if (FAILED(hr))                                                                       \
    {                                                                                     \
        M3D_LOG_INFO("FAILED(hr=" + CStr::format_("%x", (expr)) + ") in " #expr "\n");    \
        return 0;                                                                         \
    }

    int mVideoPlayer::Play(char const* filename)
    {
        // RVA 0x6A4C80 - builds WM ASF reader -> decoders -> CTextureRenderer and
        // starts it running. NOTE: apart from a file that fails to load, every
        // failure returns with whatever was built so far left in place.
        HRESULT hr = S_OK;
        if (FAILED(::CoInitialize(nullptr)))
        {
            return 0;
        }

        hr = ::CoCreateInstance(
            CLSID_FilterGraph,
            nullptr,
            CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
            IID_IGraphBuilder,
            reinterpret_cast<void**>(&g_pGB));
        if (FAILED(hr))
        {
            // NOTE: E_FAIL is non-zero, so a caller testing the result as a bool
            // takes this for success.
            return E_FAIL;
        }

        auto* renderer = new CTextureRenderer(nullptr, &hr);
        if (FAILED(hr) || !renderer)
        {
            M3D_LOG_INFO("Could not create texture renderer object! hr = " + CStr::format_("%x", hr));
            return 0;
        }

        g_pRenderer = renderer;
        hr = g_pGB->AddFilter(renderer, L"TEXTURERENDERER");
        if (FAILED(hr))
        {
            M3D_LOG_INFO("Could not add renderer filter to graph! hr = " + CStr::format_("%x", hr));
            return 0;
        }

        IBaseFilter* g_pReader = nullptr;
        hr = CreateFilter(CLSID_WMAsfReader, &g_pReader);
        if (FAILED(hr))
        {
            M3D_LOG_INFO("Failed to create WMAsfReader filter!  hr = " + CStr::format_("%x", hr));
            return 0;
        }

        hr = g_pGB->AddFilter(g_pReader, L"ASF Reader");
        if (FAILED(hr))
        {
            M3D_LOG_INFO("Failed to add ASF reader filter to graph!  hr = " + CStr::format_("%x", hr));
            return 0;
        }

        hr = AddKeyProvider(g_pGB);
        if (FAILED(hr))
        {
            M3D_LOG_INFO("Could not create the key provider!");
            return 0;
        }

        VIDEO_JIF(g_pReader->QueryInterface( IID_IFileSourceFilter, (void**)&g_pFileSource ))

        // NOTE: the conversion result is not checked, so a name too long for
        // the buffer reaches Load unterminated.
        WCHAR wFileName[MAX_PATH];
        ::MultiByteToWideChar(CP_ACP, 0, filename, -1, wFileName, MAX_PATH);

        hr = g_pFileSource->Load(wFileName, nullptr);
        if (FAILED(hr))
        {
            M3D_LOG_INFO(
                "Could not load specified video file: " + CStr(filename) + " (hr = " + CStr::format_("%x", hr) + ")");
            Release();
            return 0;
        }

        VIDEO_JIF(RenderOutputPins( g_pGB, g_pReader ))

        // The graph holds its own reference now.
        g_pReader->Release();

        VIDEO_JIF(g_pGB->QueryInterface( IID_IMediaControl, (void**)&g_pMC))
        VIDEO_JIF(g_pGB->QueryInterface( IID_IMediaEventEx, (void**)&g_pME))
        VIDEO_JIF(g_pGB->QueryInterface( IID_IMediaPosition, (void**)&g_pMP))

        // g_Event1 is signalled by DoRenderSample when a frame has been copied;
        // g_Event2 starts signalled and gates whether frames are copied at all.
        g_Event1 = ::CreateEventA(nullptr, FALSE, FALSE, nullptr);
        g_Event2 = ::CreateEventA(nullptr, FALSE, TRUE, nullptr);

        // Volume is in hundredths of a decibel: full volume is 0, each halving
        // of the setting takes off another 10dB, and it bottoms out at -100dB.
        int volume = -10000;
        if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
        {
            int const vol2d = M3D_ENGINE_CFG.m_snd_2dVolume.GetI();
            if (vol2d > 0)
            {
                double attenuation = log(vol2d * 0.01) / log(2.0);
                if (attenuation < -10.0)
                {
                    attenuation = -10.0;
                }
                if (attenuation > 0.0)
                {
                    attenuation = 0.0;
                }
                volume = static_cast<int>(attenuation * 1000.0);
            }
        }
        SetAudioVolume(volume);

        hr = g_pMC->Run();
        if (FAILED(hr))
        {
            M3D_LOG_INFO("Could not run the DirectShow graph! hr = " + CStr::format_("%x", hr));
            return 0;
        }

        m_texRend = renderer;
        return 1;
    }

#undef VIDEO_JIF

    int mVideoPlayer::Stop()
    {
        auto hr = g_pMC->Stop();
        if (SUCCEEDED(hr))
        {
            Release();
            return 1;
        }
        M3D_LOG_INFO("FAILED(hr=" + CStr(hr) + ") in g_pMC->Stop()");
        return 0;
    }

    int mVideoPlayer::Update()
    {
        if (::WaitForSingleObject(g_Event1, 30))
        {
            return 0;
        }
        M3D_RENDERER->BeginScene();
        M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, 0xFF000000);
        M3D_RENDERER->PushBlend(rend::BM_NONE);
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
        M3D_RENDERER->PushLighting(false);
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
        M3D_RENDERER->SetTexture(0, g_pTexture, -1.0);

        int tH = 0, tW = 0;
        M3D_RENDERER->GetDims(g_pTexture, tW, tH);
        
        int sH = 0, sW = 0;
        m_texRend->GetVideoDims(sW, sH);
        
        M3D_APP->PutSprite2Rel(0.0, 0.0, 0.0, 0.0, 1024.0, 768.0, static_cast<float>(sW) / static_cast<float>(tW), static_cast<float>(sH) / static_cast<float>(tH), 0xFFFFFFFF);

        M3D_RENDERER->PopLighting();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->EndScene();
        M3D_RENDERER->PresentScene();

        ::SetEvent(g_Event2);
        return 1;
    }

    int mVideoPlayer::Release()
    {
        if (g_pFileSource != nullptr)
        {
            g_pFileSource->Release();
            g_pFileSource = nullptr;
        }
        if (g_pME != nullptr)
        {
            g_pME->Release();
            g_pME = nullptr;
        }
        if (g_pMC != nullptr)
        {
            g_pMC->Release();
            g_pMC = nullptr;
        }
        if (g_pMP != nullptr)
        {
            g_pMP->Release();
            g_pMP = nullptr;
        }
        if (g_pGB != nullptr)
        {
            g_pGB->Release();
            g_pGB = nullptr;
        }
        ::CloseHandle(g_Event2);
        g_Event2 = NULL;
        ::CloseHandle(g_Event1);
        g_Event1 = NULL;
        ::CoUninitialize();
        return 1;
    }

    int mVideoPlayer::IsVideoPlaing()
    {
        if (g_pME == nullptr)
        {
            return 0;
        }

        long lEventCode = 0;
        LONG_PTR lParam1 = 0;
        LONG_PTR lParam2 = 0;
        if (g_pME->GetEvent(&lEventCode, &lParam1, &lParam2, 0) < 0)
        {
            return 1;
        }
        if (lEventCode == 1)
        {
            Release();
            return 0;
        }
        g_pME->FreeEventParams(lEventCode, lParam1, lParam2);
        return 1;
    }

    int mVideoPlayer::IsVideoLoaded()
    {
        return g_pGB != nullptr;
    }
}
