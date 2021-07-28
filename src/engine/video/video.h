#pragma once
#include <Streams.h>
#include <Windows.h>

class CMediaType;

namespace m3d
{
    class CTextureRenderer : public CBaseVideoRenderer
    {
    public:
        virtual long SetMediaType(CMediaType const*);
        virtual long CheckMediaType(CMediaType const*);
        void GetVideoDims(int&, int&);
        virtual long DoRenderSample(IMediaSample*);
        CTextureRenderer(IUnknown*, long*);
        virtual ~CTextureRenderer();
        void GetTextureDims(int&, int&);

    private:
        int m_bUseDynamicTextures;
        int m_lVidWidth;
        int m_lVidHeight;
        int m_lVidPitch;
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
