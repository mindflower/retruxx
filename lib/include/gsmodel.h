#pragma once
#include <cstdint>
#include <core/stringm3d.h>
#include <math/aabb.h>
#include <renderer/i_renderer_vertex.h>
#include <draftstructures.h>

struct CMatrix;

namespace m3d
{
    struct ModelInfo
    {
        unsigned int m_numVertices;
        unsigned int m_numFaces;
        char m_alpha;
    };

    // Collision chunk records of GSM files, copied byte for byte.
#pragma pack(push, 4)
    struct CollisionDataHeader
    {
        uint32_t numVertices;
        uint32_t numFaces;
    };

    struct CollidingVertex
    {
        float x;
        float y;
        float z;
    };
#pragma pack(pop)

    static_assert(sizeof(CollisionDataHeader) == 0x0008);
    static_assert(sizeof(CollidingVertex) == 0x000c);

    // A load point as stored in GSM files, copied byte for byte.
#pragma pack(push, 4)
    struct LPoint
    {
        char name[30];
        float x;
        float y;
        float z;
    };
#pragma pack(pop)

    static_assert(sizeof(LPoint) == 0x002c);

    class CGSModel
    {
        friend class StaticModelsServer;

    private:
        m3d::ModelInfo m_header;
        m3d::DSurfaceMaterial m_SurfaceMaterial;
        m3d::rend::VertexType m_VertType;
        unsigned int m_VertTypeSize;
        unsigned short* m_drawIndices;
        int m_idxOffset;
        int m_vertsOffset;
        void* m_drawVerts;
        m3d::CollisionDataHeader m_col_header;
        m3d::CollidingVertex* m_col_verts;
        unsigned short* m_col_idx;
        m3d::LPoint* m_loadPoints;
        int m_numLoadPoints;

    public:
        Aabb m_box;
        int m_numDrawVerts;
        int m_numDrawIndices;
        int m_numvbbank;
        int m_numibbank;

    private:
        void CalculateBBox();
        void renderMesh();
        void renderBillboard(CMatrix const&, float);
        unsigned short* m_triIndices;
        void* m_verts;
        int Load(CStr const& fname);
        int Load();

    public:
        CGSModel(m3d::CGSModel const&);
        CGSModel();
        ~CGSModel();
        int Prepare2Draw(CStr const& dirToTextures, int vertOffset, int index);
        int Prepare2Draw2(void* destVerts, void* destIndices);
        int ChangeTexture(CStr const& name, unsigned int TexNum);
        int Save(CStr const& model_name);
        void Release();
        int Render(CMatrix const& mat, float opacity);
        int RenderNormals(CMatrix const& matT, float scale);
        void CalculateNormals(int mode);
        void FlipFaces();
        void ChangeScale(float s);
        void RestoreDeviceObjects();
        void InvalidateDeviceObjects();
        m3d::ModelInfo& GetHeader();
        m3d::LPoint const* GetLPoints() const;
        int GetNumLPoints() const;
        int getNVerts();
        int getNTriIdxs();
        void const* GetVerts() const;
        int GetVertsOffset();
        unsigned short const* GetTris() const;
        m3d::rend::TexHandle const GetTex(unsigned int) const;
        unsigned int GetVertTypeSize();
    };
}  // namespace m3d
