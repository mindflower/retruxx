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
    /* 0x0004 */ volatile long m_cRef;
}; /* size: 0x0008 */

static_assert(sizeof(CKeyProvider) == 0x0008);

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
        virtual ~CTextureRenderer() override /* 0x00 */;
        virtual HRESULT CheckMediaType(const CMediaType* pmt) override /* 0x00 */;
        virtual HRESULT SetMediaType(const CMediaType* pmt) override /* 0x00 */;
        virtual HRESULT DoRenderSample(IMediaSample* pSample) override /* 0x00 */;
        /* 0x0160 */ int m_bUseDynamicTextures;
        /* 0x0164 */ long m_lVidWidth;
        /* 0x0168 */ long m_lVidHeight;
        /* 0x016c */ long m_lVidPitch;
        void GetTextureDims(int& w, int& h);
        void GetVideoDims(int& w, int& h);
    }; /* size: 0x0170 */



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
