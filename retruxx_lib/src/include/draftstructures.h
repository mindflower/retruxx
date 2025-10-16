#pragma once
#include <math/vector.h>
#include <math/quaternion.h>
#include <renderer/i_renderer.h>
#include <thirdparty/containers.h>

namespace m3d
{
    struct Index3
    {
        unsigned __int16 I[3];

    public:
        unsigned short & operator[](unsigned int);
        unsigned short const & operator[](unsigned int) const ;
    };

    struct DCollisionData
    {
        /* 0x0000 */ retruxx::vector<CVector, retruxx::allocator<CVector> > Points;
        /* 0x0010 */ retruxx::vector<m3d::Index3, retruxx::allocator<m3d::Index3> > Triangles;
    }; /* size: 0x0020 */

    static_assert(sizeof(DCollisionData) == 0x0020);

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

    struct DRAFT_Geom
    {
        /* 0x0000 */ m3d::DRAFT_GeomType Type;
        CVector Translation;
        Quaternion Rotation;
        m3d::DRAFT_GeomSize Sizes;
    }; /* size: 0x002c */

    struct DRAFT_HierGeom
    {
        /* 0x0000 */ m3d::DRAFT_Geom geom;
        /* 0x002c */ unsigned int parentBone;
    }; /* size: 0x0030 */

    struct DRAFT_HierarchyChange
    {
        /* 0x0000 */ m3d::DRAFT_Change Type;
        /* 0x0004 */ short Index;
        /* 0x0006 */ short NewParent;
    }; /* size: 0x0008 */

    struct DRAFT_BoneBounds
    {
        /* 0x0000 */ unsigned int BoneIndex;
        CVector MinRot;
        CVector MaxRot;

        DRAFT_BoneBounds()
        {
            this->BoneIndex = 0;
            this->MinRot.x = -6.2831855;
            this->MinRot.y = -6.2831855;
            this->MinRot.z = -6.2831855;
            this->MaxRot.x = 6.2831855;
            this->MaxRot.y = 6.2831855;
            this->MaxRot.z = 6.2831855;
        }
    }; /* size: 0x001c */

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

    struct DShader
    {
        /* 0x0000 */ retruxx::string Name;
        /* 0x001c */ m3d::rend::IEffect* Handle = nullptr;
    }; /* size: 0x0020 */

    static_assert(sizeof(DShader) == 0x0020);

    enum DRAFT_TextureType
    {
        DIFFUSE = 0x0,
        BUMP = 0x1,
        LIGHTMAP = 0x2,
        CUBEMAP = 0x3,
        DETAIL = 0x4,
    };

    struct DTextureInfo
    {
        /* 0x0000 */ m3d::DRAFT_TextureType Type;
        /* 0x0004 */ unsigned int UV_Set;
        std::basic_string<char, std::char_traits<char>, std::allocator<char> > FileName;
        m3d::rend::TexHandle Handle;
        bool operator<(const m3d::DTextureInfo& A);
    }; /* size: 0x0028 */

    struct DSurfaceMaterial
    {
        m3d::rend::Material material;
        m3d::DShader Shader;
        retruxx::vector<m3d::DTextureInfo, retruxx::allocator<m3d::DTextureInfo> > Textures;
    }; /* size: 0x0074 */

    class DMesh
    {
    private:
        m3d::DRAFT_GeometryHeader Header;
        m3d::rend::VertexType VertType;
        unsigned int VertTypeSize;
        retruxx::vector<m3d::DRAFT_VertexComponent> VertexComponentHeaders;
        retruxx::vector<void*> VerticesComponents;
        retruxx::vector<m3d::Index3> Triangles;
    };

    class DAnimation
    {
    private:
        m3d::DRAFT_AnimationHeader Info;
        retruxx::vector<m3d::DRAFT_HierarchyChange> HierarchyChanges;
        retruxx::vector<retruxx::vector<m3d::DRAFT_Transform>> AnimationKeys;
    };
}

