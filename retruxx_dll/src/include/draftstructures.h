#pragma once
#include <string>
#include <vector>
#include <math/vector.h>
#include <math/quaternion.h>
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
    class DCollisionData
    {
    private:
        std::vector<CVector> Points;
        std::vector<Index3> Triangles;
    };

    enum DRAFT_GeomType
    {
        BOX = 0x0,
        SPHERE = 0x1,
        CYLINDER = 0x2,
    };

    enum DRAFT_MeshType
    {
        SKINED_MESH = 0x2,
        TRI_MESH = 0x1,
        LOAD_POINT = 0x3,
        STATIC_MESH = 0x4,
    };

    enum DRAFT_Change
    {
        NEW_PARENT = 0x0,
        NOT_VISIBLE = 0x1,
    };

    struct DRAFT_GeometryHeader
    {
        DRAFT_MeshType Type;
        __int16 MaterialIndex;
        unsigned int VertexCount;
        unsigned int TriangleCount;
        __int16 ParentBone;
        unsigned int NumberOfVertexComponents;
    };

    struct DRAFT_BoxSizes
    {
        float x;
        float y;
        float z;
    };

    struct DRAFT_CylinderSizes
    {
        float r;
        float y;
    };

    union DRAFT_GeomSize
    {
        DRAFT_BoxSizes BoxSizes;
        float SphereRadius;
        DRAFT_CylinderSizes CylinderSizes;
    };

    class DRAFT_Geom
    {
    private:
        DRAFT_GeomType Type;
        CVector Translation;
        Quaternion Rotation;
        m3d::DRAFT_GeomSize Sizes;
    };

    class DRAFT_HierGeom
    {
    private:
        DRAFT_Geom geom;
        unsigned int parentBone;
    };

    struct DRAFT_HierarchyChange
    {
        DRAFT_Change Type;
        __int16 Index;
        __int16 NewParent;
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

    struct DRAFT_Bone
    {
        char Name[40];
        __int16 ParentIndex;
        float Tx;
        float Ty;
        float Tz;
        float Rx;
        float Ry;
        float Rz;
        float Rw;
        float Sx;
        float Sy;
        float Sz;
    };

    struct DRAFT_AnimationHeader
    {
        char Name[25];
        unsigned int FramesNumber;
        unsigned int FPS_Number;
        int NextAnimation;
        unsigned int NumberOfChanges;
    };

    struct DRAFT_Transform
    {
        float Tx;
        float Ty;
        float Tz;
        float Rx;
        float Ry;
        float Rz;
        float Rw;
        float Sx;
        float Sy;
        float Sz;
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

    class DSurfaceMaterial
    {
    private:
        rend::Material material;
        DShader Shader;
        std::vector<DTextureInfo> Textures;
    };

    class DMesh
    {
    private:
        m3d::DRAFT_GeometryHeader Header;
        m3d::rend::VertexType VertType;
        unsigned int VertTypeSize;
        std::vector<m3d::DRAFT_VertexComponent> VertexComponentHeaders;
        std::vector<void*> VerticesComponents;
        std::vector<m3d::Index3> Triangles;
    };

    class DAnimation
    {
    private:
        m3d::DRAFT_AnimationHeader Info;
        std::vector<m3d::DRAFT_HierarchyChange> HierarchyChanges;
        std::vector<std::vector<m3d::DRAFT_Transform>> AnimationKeys;
    };
}

