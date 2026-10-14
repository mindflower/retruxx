#pragma once
#include <math/vector.h>
#include <math/quaternion.h>
#include <renderer/i_renderer.h>
#include <retruxx/common.h>

#include <cstdint>

namespace m3d
{
    // The records below marked as file data are read from and written to SAM/GAM model
    // files byte for byte: fixed-width fields, explicit packing and a size check each.

    // File data.
    struct Index3
    {
        uint16_t I[3];
        uint16_t const& operator[](unsigned int i) const;
        uint16_t& operator[](unsigned int i);
    };

    static_assert(sizeof(Index3) == 0x0006);

    struct DCollisionData
    {
        retruxx::vector<CVector, retruxx::allocator<CVector>> Points;
        retruxx::vector<m3d::Index3, retruxx::allocator<m3d::Index3>> Triangles;
    };

    enum DRAFT_GeomType : int32_t
    {
        BOX = 0x0,
        SPHERE = 0x1,
        CYLINDER = 0x2,
    };

    enum DRAFT_MeshType : int32_t
    {
        SKINED_MESH = 0x2,
        TRI_MESH = 0x1,
        LOAD_POINT = 0x3,
        STATIC_MESH = 0x4,
    };

    enum DRAFT_Change : int32_t
    {
        NEW_PARENT = 0x0,
        NOT_VISIBLE = 0x1,
    };

    // The DRAFT_* records are the packed layouts of the SAM file.
#pragma pack(push, 1)
    struct DRAFT_GeometryHeader
    {
        DRAFT_MeshType Type;
        int16_t MaterialIndex;
        uint32_t VertexCount;
        uint32_t TriangleCount;
        int16_t ParentBone;
        uint32_t NumberOfVertexComponents;
    };
#pragma pack(pop)

    static_assert(sizeof(DRAFT_GeometryHeader) == 0x0014);

#pragma pack(push, 4)
    struct DRAFT_Header
    {
        uint32_t NumberOfMeshes;
        uint32_t NumberOfMaterials;
        uint32_t NumberOfBones;
        uint32_t NumberOfAnimations;
    };

    struct DRAFT_CollisionHeader
    {
        uint32_t PointCount;
        uint32_t TriangleCount;
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
        m3d::DRAFT_GeomType Type;
        CVector Translation;
        Quaternion Rotation;
        m3d::DRAFT_GeomSize Sizes;
    };

    struct DRAFT_HierGeom
    {
        m3d::DRAFT_Geom geom;
        uint32_t parentBone;
    };

    struct DRAFT_HierarchyChange
    {
        m3d::DRAFT_Change Type;
        int16_t Index;
        int16_t NewParent;
    };

    struct DRAFT_BoneBounds
    {
        uint32_t BoneIndex;
        CVector MinRot;
        CVector MaxRot;

        DRAFT_BoneBounds()
        {
            this->BoneIndex = 0;
            this->MinRot.x = -6.2831855f;
            this->MinRot.y = -6.2831855f;
            this->MinRot.z = -6.2831855f;
            this->MaxRot.x = 6.2831855f;
            this->MaxRot.y = 6.2831855f;
            this->MaxRot.z = 6.2831855f;
        }
    };

#pragma pack(pop)

    static_assert(sizeof(DRAFT_Header) == 0x0010);
    static_assert(sizeof(DRAFT_CollisionHeader) == 0x0008);
    static_assert(sizeof(DRAFT_Geom) == 0x002c);
    static_assert(sizeof(DRAFT_HierGeom) == 0x0030);
    static_assert(sizeof(DRAFT_HierarchyChange) == 0x0008);
    static_assert(sizeof(DRAFT_BoneBounds) == 0x001c);

#pragma pack(push, 1)
    struct DRAFT_Bone
    {
        char Name[40];
        int16_t ParentIndex;
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
        uint32_t FramesNumber;
        uint32_t FPS_Number;
        int32_t NextAnimation;
        uint32_t NumberOfChanges;
    };

    struct DRAFT_Influence
    {
        int16_t BoneIndex;
        float Weight;
    };
#pragma pack(pop)

    static_assert(sizeof(DRAFT_Bone) == 0x0052);
    static_assert(sizeof(DRAFT_AnimationHeader) == 0x0029);
    static_assert(sizeof(DRAFT_Influence) == 0x0006);

#pragma pack(push, 4)
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
#pragma pack(pop)

    static_assert(sizeof(DRAFT_Transform) == 0x0028);

    enum VERTEX_COMPONENT : int32_t
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

#pragma pack(push, 4)
    struct DRAFT_VertexComponent
    {
        VERTEX_COMPONENT Type;
        int32_t Size;
        DRAFT_VertexComponent(VERTEX_COMPONENT t, int s);
        DRAFT_VertexComponent();
        bool operator==(DRAFT_VertexComponent const& A);
    };
#pragma pack(pop)

    static_assert(sizeof(DRAFT_VertexComponent) == 0x0008);

    struct DInfluence
    {
        short BoneIndex;
        float Weight;
        bool operator==(DInfluence const&) const;
    };

    struct DShader
    {
        retruxx::string Name;
        m3d::rend::IEffect* Handle = nullptr;
    };

    enum DRAFT_TextureType : int32_t
    {
        DIFFUSE = 0x0,
        BUMP = 0x1,
        LIGHTMAP = 0x2,
        CUBEMAP = 0x3,
        DETAIL = 0x4,
    };

#pragma pack(push, 4)
    struct DRAFT_TextureInfo
    {
        char FileName[40];
        uint32_t UVSet;
        m3d::DRAFT_TextureType Type;
    };

    static_assert(sizeof(DRAFT_TextureInfo) == 0x0030);

    struct DRAFT_MaterialHeader
    {
        m3d::rend::Material material;
        uint32_t TextureLayersNumber;
        uint32_t ShaderStringLength;
    };
#pragma pack(pop)

    static_assert(sizeof(DRAFT_MaterialHeader) == 0x004c);

    struct DTextureInfo
    {
        m3d::DRAFT_TextureType Type;
        unsigned int UV_Set;
        std::basic_string<char, std::char_traits<char>, std::allocator<char>> FileName;
        m3d::rend::TexHandle Handle;
        bool operator<(m3d::DTextureInfo const& A);
    };

    struct DSurfaceMaterial
    {
        m3d::rend::Material material;
        m3d::DShader Shader;
        retruxx::vector<m3d::DTextureInfo, retruxx::allocator<m3d::DTextureInfo>> Textures;
    };

    struct DMesh
    {
        m3d::DRAFT_GeometryHeader Header;
        m3d::rend::VertexType VertType;
        unsigned int VertTypeSize;
        retruxx::vector<m3d::DRAFT_VertexComponent> VertexComponentHeaders;
        retruxx::vector<void*> VerticesComponents;
        retruxx::vector<m3d::Index3> Triangles;
    };

    struct DAnimation
    {
        m3d::DRAFT_AnimationHeader Info;
        retruxx::vector<m3d::DRAFT_HierarchyChange> HierarchyChanges;
        retruxx::vector<retruxx::vector<m3d::DRAFT_Transform>> AnimationKeys;
    };
}  // namespace m3d

// The SAM (draft model) helpers, in the global namespace as in the shipped build.
bool operator==(m3d::DRAFT_Transform const& A, m3d::DRAFT_Transform const& B);
bool VertCompPresent(retruxx::vector<m3d::DRAFT_VertexComponent>& VC, m3d::VERTEX_COMPONENT c);
unsigned int CompOff(retruxx::vector<m3d::DRAFT_VertexComponent>& VC, m3d::VERTEX_COMPONENT Cmp);
bool DefineVertexType(
    retruxx::vector<m3d::DRAFT_VertexComponent>& VC,
    m3d::rend::VertexType& VertType,
    unsigned int& VertTypeSize);
void BinormalToTangentW(m3d::DMesh& Mh);
bool IsStaticTriMesh(
    m3d::DMesh const& TriMesh,
    retruxx::vector<m3d::DAnimation> const& Animations,
    retruxx::vector<m3d::DRAFT_Bone> const& Bones);
