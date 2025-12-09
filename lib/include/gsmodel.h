#pragma once
#include <core/stringm3d.h>
#include <math/aabb.h>
#include <renderer/i_renderer_vertex.h>
#include <draftstructures.h>

struct CMatrix;

namespace m3d
{
    struct ModelInfo
    {
        /* 0x0000 */ unsigned int m_numVertices;
        /* 0x0004 */ unsigned int m_numFaces;
        /* 0x0008 */ char m_alpha;
    }; /* size: 0x000c */

    struct CollisionDataHeader
    {
        /* 0x0000 */ unsigned int numVertices;
        /* 0x0004 */ unsigned int numFaces;
    }; /* size: 0x0008 */

    struct CollidingVertex
    {
        /* 0x0000 */ float x;
        /* 0x0004 */ float y;
        /* 0x0008 */ float z;
    }; /* size: 0x000c */

    struct LPoint
    {
        /* 0x0000 */ char name[30];
        /* 0x001e */ char Padding_306[2];
        /* 0x0020 */ float x;
        /* 0x0024 */ float y;
        /* 0x0028 */ float z;
    }; /* size: 0x002c */


    class CGSModel
    {
        friend class StaticModelsServer;
    private:
        m3d::ModelInfo m_header;
        m3d::DSurfaceMaterial m_SurfaceMaterial;
        /* 0x0080 */ m3d::rend::VertexType m_VertType;
        /* 0x0084 */ unsigned int m_VertTypeSize;
        /* 0x0088 */ unsigned short* m_drawIndices;
        /* 0x008c */ int m_idxOffset;
        /* 0x0090 */ int m_vertsOffset;
        /* 0x0094 */ void* m_drawVerts;
        m3d::CollisionDataHeader m_col_header;
        /* 0x00a0 */ m3d::CollidingVertex* m_col_verts;
        /* 0x00a4 */ unsigned short* m_col_idx;
        /* 0x00a8 */ m3d::LPoint* m_loadPoints;
        /* 0x00ac */ int m_numLoadPoints;

    public:
        Aabb m_box;
        /* 0x00c8 */ int m_numDrawVerts;
        /* 0x00cc */ int m_numDrawIndices;
        /* 0x00d0 */ int m_numvbbank;
        /* 0x00d4 */ int m_numibbank;

    private:
        void CalculateBBox();
        void renderMesh();
        void renderBillboard(const CMatrix&, float);
        /* 0x00d8 */ unsigned short* m_triIndices;
        /* 0x00dc */ void* m_verts;
        int Load(const CStr& fname);
        int Load();

    public:
        CGSModel(const m3d::CGSModel&);
        CGSModel();
        ~CGSModel();
        int Prepare2Draw(const CStr& dirToTextures, int vertOffset, int index);
        int Prepare2Draw2(void* destVerts, void* destIndices);
        int ChangeTexture(const CStr& name, unsigned int TexNum);
        int Save(const CStr& model_name);
        void Release();
        int Render(const CMatrix& mat, float opacity);
        int RenderNormals(const CMatrix& matT, float scale);
        void CalculateNormals(int mode);
        void FlipFaces();
        void ChangeScale(float s);
        void RestoreDeviceObjects();
        void InvalidateDeviceObjects();
        m3d::ModelInfo& GetHeader();
        const m3d::LPoint* GetLPoints() const;
        int GetNumLPoints() const;
        int getNVerts();
        int getNTriIdxs();
        const void* GetVerts() const;
        int GetVertsOffset();
        const unsigned short* GetTris() const;
        const m3d::rend::TexHandle GetTex(unsigned int) const;
        unsigned int GetVertTypeSize();
    }; /* size: 0x00e0 */
}
