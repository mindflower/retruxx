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
        int Stop();
        int IsVideoPlaing();
        int Update();
        mVideoPlayer();
        int Release();
        int IsVideoLoaded();
        int Init();
        int Play(char const*);

    private:
        HRESULT hr;
        m3d::CTextureRenderer* m_texRend;
    };
}
