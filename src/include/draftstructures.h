#pragma once
#include <string>
#include <math/vector.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    struct Index3
    {
        unsigned __int16 I[3];

    public:
        unsigned short & operator[](unsigned int);
        unsigned short const & operator[](unsigned int) const ;
    };

    class DRAFT_BoneBounds
    {
    public:
        DRAFT_BoneBounds(DRAFT_BoneBounds const&);

    private:
        unsigned int BoneIndex;
        CVector MinRot;
        CVector MaxRot;
    };

    enum VERTEX_COMPONENT
    {
        COORDINATE = 0x0,
        NORMAL = 0x1,
        COLOR = 0x2,
        TEXTURE1 = 0x3,
        TEXTURE2 = 0x4,
        TEXTURE3 = 0x5,
        TANGENT = 0x14,
        BINORMAL = 0x15,
        INFLUENCES = 0x16,
    };

    class DRAFT_VertexComponent
    {
    public:
        bool operator==(DRAFT_VertexComponent const&);
        DRAFT_VertexComponent(VERTEX_COMPONENT, int);
        DRAFT_VertexComponent();

    private:
        VERTEX_COMPONENT Type;
        int Size;
    };

    class DShader
    {
    public:
        DShader(DShader const&);

    private:
        std::string Name;
        rend::IEffect* Handle;
    };

    enum DRAFT_TextureType
    {
        DIFFUSE = 0x0,
        BUMP = 0x1,
        LIGHTMAP = 0x2,
        CUBEMAP = 0x3,
        DETAIL = 0x4,
    };

    class DTextureInfo
    {
    public:
        bool operator<(DTextureInfo const&);

    private:
        DRAFT_TextureType Type;
        unsigned int UV_Set;
        std::string FileName;
        rend::TexHandle Handle;
    };
}

