#pragma once
#include <math/vector.h>
#include <math/quaternion.h>
#include <renderer/i_renderer.h>
#include <retruxx/common.h>

namespace m3d
{
    struct Index3
    {
        /* 0x0000 */ unsigned short I[3];
        const unsigned short& operator[](unsigned int i) const;
        unsigned short& operator[](unsigned int i);
    }; /* size: 0x0006 */

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

    // The DRAFT_* records are the packed layouts of the SAM file.
#pragma pack(push, 1)
    struct DRAFT_GeometryHeader
    {
        /* 0x0000 */ DRAFT_MeshType Type;
        /* 0x0004 */ __int16 MaterialIndex;
        /* 0x0006 */ unsigned int VertexCount;
        /* 0x000a */ unsigned int TriangleCount;
        /* 0x000e */ __int16 ParentBone;
        /* 0x0010 */ unsigned int NumberOfVertexComponents;
    }; /* size: 0x0014 */
#pragma pack(pop)

    static_assert(sizeof(DRAFT_GeometryHeader) == 0x0014);

    struct DRAFT_Header
    {
        /* 0x0000 */ unsigned int NumberOfMeshes;
        /* 0x0004 */ unsigned int NumberOfMaterials;
        /* 0x0008 */ unsigned int NumberOfBones;
        /* 0x000c */ unsigned int NumberOfAnimations;
    }; /* size: 0x0010 */

    struct DRAFT_CollisionHeader
    {
        /* 0x0000 */ unsigned int PointCount;
        /* 0x0004 */ unsigned int TriangleCount;
    }; /* size: 0x0008 */

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

#pragma pack(push, 1)
    struct DRAFT_Bone
    {
        /* 0x0000 */ char Name[40];
        /* 0x0028 */ __int16 ParentIndex;
        /* 0x002a */ float Tx;
        /* 0x002e */ float Ty;
        /* 0x0032 */ float Tz;
        /* 0x0036 */ float Rx;
        /* 0x003a */ float Ry;
        /* 0x003e */ float Rz;
        /* 0x0042 */ float Rw;
        /* 0x0046 */ float Sx;
        /* 0x004a */ float Sy;
        /* 0x004e */ float Sz;
    }; /* size: 0x0052 */

    struct DRAFT_AnimationHeader
    {
        /* 0x0000 */ char Name[25];
        /* 0x0019 */ unsigned int FramesNumber;
        /* 0x001d */ unsigned int FPS_Number;
        /* 0x0021 */ int NextAnimation;
        /* 0x0025 */ unsigned int NumberOfChanges;
    }; /* size: 0x0029 */

    struct DRAFT_Influence
    {
        /* 0x0000 */ short BoneIndex;
        /* 0x0002 */ float Weight;
    }; /* size: 0x0006 */
#pragma pack(pop)

    static_assert(sizeof(DRAFT_Bone) == 0x0052);
    static_assert(sizeof(DRAFT_AnimationHeader) == 0x0029);
    static_assert(sizeof(DRAFT_Influence) == 0x0006);

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

    struct DRAFT_VertexComponent
    {
        /* 0x0000 */ VERTEX_COMPONENT Type;
        /* 0x0004 */ int Size;
        DRAFT_VertexComponent(VERTEX_COMPONENT t, int s);
        DRAFT_VertexComponent();
        bool operator==(DRAFT_VertexComponent const& A);
    }; /* size: 0x0008 */

    struct DInfluence
    {
        /* 0x0000 */ short BoneIndex;
        /* 0x0004 */ float Weight;
        bool operator==(const DInfluence&) const;
    }; /* size: 0x0008 */

    static_assert(sizeof(DInfluence) == 0x0008);

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

    struct DRAFT_TextureInfo
    {
        /* 0x0000 */ char FileName[40];
        /* 0x0028 */ unsigned int UVSet;
        /* 0x002c */ m3d::DRAFT_TextureType Type;
    }; /* size: 0x0030 */

    static_assert(sizeof(DRAFT_TextureInfo) == 0x0030);

    struct DRAFT_MaterialHeader
    {
        /* 0x0000 */ m3d::rend::Material material;
        /* 0x0044 */ unsigned int TextureLayersNumber;
        /* 0x0048 */ unsigned int ShaderStringLength;
    }; /* size: 0x004c */

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

    struct DMesh
    {
        /* 0x0000 */ m3d::DRAFT_GeometryHeader Header;
        /* 0x0014 */ m3d::rend::VertexType VertType;
        /* 0x0018 */ unsigned int VertTypeSize;
        /* 0x001c */ retruxx::vector<m3d::DRAFT_VertexComponent> VertexComponentHeaders;
        /* 0x002c */ retruxx::vector<void*> VerticesComponents;
        /* 0x003c */ retruxx::vector<m3d::Index3> Triangles;
    }; /* size: 0x004c */

    struct DAnimation
    {
        /* 0x0000 */ m3d::DRAFT_AnimationHeader Info;
        /* 0x0029 */ char Padding_201[3];
        /* 0x002c */ retruxx::vector<m3d::DRAFT_HierarchyChange> HierarchyChanges;
        /* 0x003c */ retruxx::vector<retruxx::vector<m3d::DRAFT_Transform>> AnimationKeys;
    }; /* size: 0x004c */
}

// The SAM (draft model) helpers, in the global namespace as in the shipped build.
bool operator==(m3d::DRAFT_Transform const& A, m3d::DRAFT_Transform const& B);
bool VertCompPresent(retruxx::vector<m3d::DRAFT_VertexComponent>& VC, m3d::VERTEX_COMPONENT c);
unsigned int CompOff(retruxx::vector<m3d::DRAFT_VertexComponent>& VC, m3d::VERTEX_COMPONENT Cmp);
bool DefineVertexType(
    retruxx::vector<m3d::DRAFT_VertexComponent>& VC, m3d::rend::VertexType& VertType, unsigned int& VertTypeSize);
void BinormalToTangentW(m3d::DMesh& Mh);
bool IsStaticTriMesh(m3d::DMesh const& TriMesh, retruxx::vector<m3d::DAnimation> const& Animations,
    retruxx::vector<m3d::DRAFT_Bone> const& Bones);
