#pragma once
#include <core/stringm3d.h>
#include <math/aabb.h>
#include <renderer/i_renderer_vertex.h>

class CMatrix;

namespace m3d
{
    struct ModelInfo
    {
        unsigned int m_numVertices;
        unsigned int m_numFaces;
        char m_alpha;
    };

    class CGSModel
    {
    public:
        void CalculateNormals(int);
        void FlipFaces();
        int Load(CStr const &);
        int Prepare2Draw(CStr const &,int,int);
        int RenderNormals(CMatrix const &,float);
        ~CGSModel();
        int Render(CMatrix const &,float);
        ModelInfo & GetHeader();
        void ChangeScale(float);
        CGSModel();
        int ChangeTexture(CStr const &,unsigned int);
        void Release();
        int Save(CStr const &);
        int Prepare2Draw2(void *,void *);

    private:
        void CalculateBBox();
        void renderMesh();
        ModelInfo m_header;
        DSurfaceMaterial m_SurfaceMaterial;
        rend::VertexType m_VertType;
        unsigned int m_VertTypeSize;
        unsigned __int16 *m_drawIndices;
        int m_idxOffset;
        int m_vertsOffset;
        void *m_drawVerts;
        CollisionDataHeader m_col_header;
        CollidingVertex *m_col_verts;
        unsigned __int16 *m_col_idx;
        LPoint *m_loadPoints;
        int m_numLoadPoints;
        Aabb m_box;
        int m_numDrawVerts;
        int m_numDrawIndices;
        int m_numvbbank;
        int m_numibbank;
        unsigned __int16 *m_triIndices;
        void *m_verts;
    };
}
