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
    IBaseFilter* g_pReader = nullptr;
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

    //HRESULT AddKeyProvider(IGraphBuilder* graph)
    //{
    //    CKeyProvider prov;
    //    prov.AddRef();
    //    IObjectWithSite* objectWithSite = nullptr;
    //    auto hr = graph->QueryInterface(IID_IObjectWithSite, reinterpret_cast<LPVOID*>(&objectWithSite));
    //    if (FAILED(hr))
    //    {
    //        return hr;
    //    }
    //    hr = objectWithSite->SetSite(&prov);
    //    objectWithSite->Release();
    //    return hr;
    //}

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

//CKeyProvider::CKeyProvider() : m_cRef(0)
//{
//}
//
//// IUnknown methods
//ULONG CKeyProvider::AddRef()
//{
//    return InterlockedIncrement(&m_cRef);
//}
//
//ULONG CKeyProvider::Release()
//{
//    ASSERT(m_cRef > 0);
//
//    ULONG lCount = InterlockedDecrement(&m_cRef);
//    if (m_cRef == 0)
//    {
//        delete this;
//        return (ULONG)0;
//    }
//    return (ULONG)lCount;
//}
//
//// We only support IUnknown and IServiceProvider.
//HRESULT CKeyProvider::QueryInterface(REFIID riid, void** ppv)
//{
//    if (!ppv) return E_POINTER;
//
//    if (riid == IID_IUnknown)
//    {
//        *ppv = (void*) static_cast<IUnknown*>(this);
//        AddRef();
//        return S_OK;
//    }
//    if (riid == IID_IServiceProvider)
//    {
//        *ppv = (void*) static_cast<IServiceProvider*>(this);
//        AddRef();
//        return S_OK;
//    }
//
//    return E_NOINTERFACE;
//}
//
//STDMETHODIMP CKeyProvider::QueryService(REFIID siid, REFIID riid, void** ppv)
//{
//    if (!ppv) return E_POINTER;
//
//    if (siid == __uuidof(IWMReader) && riid == IID_IUnknown)
//    {
//        IUnknown* punkCert;
//        HRESULT hr = WMCreateCertificate(&punkCert);
//        if (SUCCEEDED(hr))
//        {
//            *ppv = (void*)punkCert;
//        }
//        return hr;
//    }
//    return E_NOINTERFACE;
//}

unsigned int g_Tex = 0;
m3d::rend::TexHandle g_pTexture;

namespace m3d
{
    CTextureRenderer* g_pRenderer = nullptr;

    long CTextureRenderer::SetMediaType(CMediaType const* pmt)
    {
        const auto* header = (const VIDEOINFOHEADER*)pmt->pbFormat;
        m_lVidWidth = header->bmiHeader.biWidth;
        m_lVidHeight = header->bmiHeader.biHeight;

        auto v4 = 2;
        auto v5 = 2;
        for (m_lVidPitch = 4 * m_lVidWidth; v5 < m_lVidWidth; v5 *= 2);

        if (m_lVidHeight > 2)
        {
            do
                v4 *= 2;
            while (v4 < m_lVidHeight);
        }
        g_pTexture = M3D_APP->m_renderer->AddDynamicTexture("$TexMedia", v5, v4, 5);
        M3D_APP->m_renderer->SetTextureParameter(g_pTexture, rend::TM_WRAP_S, 3);
        M3D_APP->m_renderer->SetTextureParameter(g_pTexture, rend::TM_WRAP_T, 3);
        return 0;
    }

    long CTextureRenderer::CheckMediaType(CMediaType const* pmt)
    {
        auto result = 0;
        if (!pmt)
            return 0x80004003;
        if (memcmp(&pmt->formattype, &FORMAT_VideoInfo, 0x10u))
            return 0x80070057;
        if (memcmp(pmt, &MEDIATYPE_Video, 0x10u) || memcmp(&pmt->subtype, &MEDIASUBTYPE_RGB32, 0x10u))
            result = 0x8004022A;
        return result;
    }

    void CTextureRenderer::GetVideoDims(int& w, int& h)
    {
        w = m_lVidWidth;
        h = m_lVidHeight;
    }

    long CTextureRenderer::DoRenderSample(IMediaSample* pSample)
    {
        if (::WaitForSingleObject(g_Event2, 0x1E))
        {
            return 0;
        }

        unsigned char* pBmpBuffer;
        pSample->GetPointer(&pBmpBuffer);

        int xsize = 0;
        int ysize = 0;
        M3D_APP->m_renderer->GetDims(g_pTexture, xsize, ysize);

        RETRUXX_NOT_IMPLEMENTED;

        M3D_APP->m_renderer->UnlockTexture(g_pTexture);
        SetEvent(g_Event1);
        return 0;
    }

    CTextureRenderer::CTextureRenderer(IUnknown* pUnk, long* phr) : CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer), NULL, pUnk, phr)
    {
        m_bUseDynamicTextures = false;
        g_Tex = 0;
        if (phr)
        {
            *phr = 0;
        }
    }

    CTextureRenderer::~CTextureRenderer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CTextureRenderer::GetTextureDims(int&, int&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    int mVideoPlayer::Play(char const* filename)
    {
        auto hr = ::CoInitialize(NULL);
        if (FAILED(hr))
        {
            return 0;
        }
        hr = ::CoCreateInstance(
            CLSID_FilterGraph,
            NULL,
            CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
            IID_IGraphBuilder,
            reinterpret_cast<LPVOID*>(&g_pGB)
        );
        if (FAILED(hr))
        {
            return -2147467259;
        }
        auto* renderer = new CTextureRenderer(NULL, &hr);
        if (SUCCEEDED(hr) && renderer != nullptr)
        {
            g_pRenderer = renderer;
            hr = g_pGB->AddFilter(g_pRenderer, L"TEXTURERENDERER");
            if (FAILED(hr))
            {
                M3D_LOG_INFO("Could not add renderer filter to graph! hr = " + CStr(hr));
                return 0;
            }
            hr = CreateFilter(CLSID_WMAsfReader, &g_pReader);
            if (FAILED(hr))
            {
                M3D_LOG_INFO("Failed to create WMAsfReader filter!  hr = " + CStr(hr));
                return 0;
            }
            hr = g_pGB->AddFilter(g_pReader, L"ASF Reader");
            if (FAILED(hr))
            {
                M3D_LOG_INFO("Failed to add ASF reader filter to graph!  hr = " + CStr(hr));
                return 0;
            }
            //hr = AddKeyProvider(g_pGB);
            //if (FAILED(hr))
            //{
            //    LOG("Could not create the key provider!  hr = " + std::to_string(hr), LOG_INFO);
            //    return 0;
            //}
            hr = g_pReader->QueryInterface(IID_IFileSourceFilter, reinterpret_cast<LPVOID*>(&g_pFileSource));
            if (FAILED(hr))
            {
                M3D_LOG_INFO("FAILED(hr=" + CStr(hr) + ") in g_pReader->QueryInterface( IID_IFileSourceFilter, (void**)&g_pFileSource )");
                return 0;
            }
            wchar_t wFileName[MAX_PATH + 1] = { 0 };
            ::MultiByteToWideChar(0, 0, filename, -1, wFileName, MAX_PATH);
            hr = g_pFileSource->Load(wFileName, nullptr);   //TODO: check this
            if (FAILED(hr))
            {
                M3D_LOG_INFO(CStr("Could not load specified video file: ") + filename);
                Release();
                return 0;
            }
            hr = RenderOutputPins(g_pGB, g_pReader);
            if (FAILED(hr))
            {
                M3D_LOG_INFO("FAILED(hr=" + CStr(hr) + ") in RenderOutputPins( g_pGB, g_pReader )");
                return 0;    
            }
            g_pReader->Release();
            hr = g_pGB->QueryInterface(IID_IMediaControl, reinterpret_cast<LPVOID*>(&g_pMC));
            if (FAILED(hr))
            {
                M3D_LOG_INFO("FAILED(hr=" + CStr(hr) + ") in g_pGB->QueryInterface( IID_IMediaControl, (void**)&g_pMC)");
                return 0;
            }
            hr = g_pGB->QueryInterface(IID_IMediaEventEx, reinterpret_cast<LPVOID*>(&g_pME));
            if (FAILED(hr))
            {
                M3D_LOG_INFO("FAILED(hr=" + CStr(hr) + ") in g_pGB->QueryInterface( IID_IMediaEventEx, (void**)&g_pME)");
                return 0;
            }
            hr = g_pGB->QueryInterface(IID_IMediaPosition, reinterpret_cast<LPVOID*>(&g_pMP));
            if (FAILED(hr))
            {
                M3D_LOG_INFO("FAILED(hr=" + CStr(hr) + ") in g_pGB->QueryInterface( IID_IMediaPosition, (void**)&g_pMP)");
                return 0;
            }
            g_Event1 = ::CreateEventA(0, 0, 0, 0);
            g_Event2 = ::CreateEventA(0, 0, 1, 0);

            int volume = -10000;
            if (g_Kernel->GetEngineCfg().m_snd_Enable.GetB())
            {
                auto cfgVolume = g_Kernel->GetEngineCfg().m_snd_2dVolume.GetI();
                if (cfgVolume > 0)
                {
                    //TODO: clamp and other
                    volume = cfgVolume;
                }
            }
            SetAudioVolume(volume);
            hr = g_pMC->Run();
            if (SUCCEEDED(hr))
            {
                m_texRend = g_pRenderer;
                return 1;
            }
            M3D_LOG_INFO("Could not run the DirectShow graph! hr = " + CStr(hr));
            return 0;
        }
        M3D_LOG_INFO("Could not create texture renderer object! hr = " + CStr(hr));
        return 0;
    }

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
        Application::g_pApp->m_renderer->BeginScene();
        Application::g_pApp->m_renderer->ClearViewport(rend::M3DCLEAR_CZ, 0);   //TODO ClearViewport second arg
        Application::g_pApp->m_renderer->PushBlend(rend::BM_NONE);
        Application::g_pApp->m_renderer->PushZbState(rend::ZB_DISABLE);
        Application::g_pApp->m_renderer->PushLighting(false);
        Application::g_pApp->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
        Application::g_pApp->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
        Application::g_pApp->m_renderer->SetTexture(0, g_pTexture, -1.0);

        int tH = 0, tW = 0;
        Application::g_pApp->m_renderer->GetDims(g_pTexture, tW, tH);
        int sH = 0, sW = 0;
        m_texRend->GetVideoDims(sW, sH);
        Application::g_pApp->PutSprite2Rel(0.0, 0.0, 0.0, 0.0, 1024.0, 768.0, sW / tW, sH / tW, static_cast<unsigned>(-1));

        Application::g_pApp->m_renderer->PopLighting();
        Application::g_pApp->m_renderer->PopZbState();
        Application::g_pApp->m_renderer->PopBlend();
        Application::g_pApp->m_renderer->EndScene();
        Application::g_pApp->m_renderer->PresentScene();
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
