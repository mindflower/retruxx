#pragma once
#include <Streams.h>
#include <Windows.h>

class CMediaType;
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

// Hands the Windows Media ASF reader the certificate it insists on before it
// will open a file. The reader finds it by asking the filter graph's site.
class CKeyProvider : public IServiceProvider
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const _GUID& riid, void** ppv) override;
    virtual ULONG STDMETHODCALLTYPE AddRef() override;
    virtual ULONG STDMETHODCALLTYPE Release() override;
    CKeyProvider();
    virtual HRESULT STDMETHODCALLTYPE QueryService(const _GUID& siid, const _GUID& riid, void** ppv) override;

private:
    volatile long m_cRef;
};

namespace m3d
{
    // Layout and offsets as in the shipped build. There is deliberately no
    // sizeof check: CBaseVideoRenderer comes from the vendored DirectShow SDK
    // base classes, which are 0x20 bytes larger than the 2005 ones, so here the
    // object is 0x190 bytes with the members starting at 0x180. Nothing reads
    // them by offset from outside this class, so the shift is harmless.
    class CTextureRenderer : public CBaseVideoRenderer
    {
    public:
        CTextureRenderer(IUnknown* pUnk, HRESULT* phr);
        virtual ~CTextureRenderer() override;
        virtual HRESULT CheckMediaType(const CMediaType* pmt) override;
        virtual HRESULT SetMediaType(const CMediaType* pmt) override;
        virtual HRESULT DoRenderSample(IMediaSample* pSample) override;
        int m_bUseDynamicTextures;
        long m_lVidWidth;
        long m_lVidHeight;
        long m_lVidPitch;
        void GetTextureDims(int& w, int& h);
        void GetVideoDims(int& w, int& h);
    };



    class mVideoPlayer
    {
    public:
        ~mVideoPlayer();
        int Init();
        int Play(char const* filename);
        int Stop();
        int Update();
        int Release();
        int IsVideoPlaing();
        int IsVideoLoaded();

    private:
        HRESULT hr = NULL;
        CTextureRenderer* m_texRend = nullptr;
    };
}
