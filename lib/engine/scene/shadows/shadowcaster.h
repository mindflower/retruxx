#pragma once
#include <skelmodel.h>

struct CMatrix;
struct CVector;

namespace m3d
{
    // The stencil shadow volume of one mesh: the silhouette edges as seen from the light, extruded away from it.
    class ShadowVolume
    {
    public:
        ShadowVolume();
        ~ShadowVolume();
        void Initialize(const m3d::AnimatedModel::Mesh& mesh, unsigned short* edgesBuf);
        unsigned int BuildShadowVolume(const CVector& lightDir, CVector* volumeVertexPool);
        void Render(const CMatrix& objToWorld, unsigned int color) const;
        unsigned int GetNumVerts() const;
        const CVector* GetVerts() const;
        void Invalidate();

    private:
        /* 0x0000 */ bool m_bIsLocked;
        /* 0x0001 */ char Padding_123[3];
        /* 0x0004 */ unsigned int m_numVerts;
        /* 0x0008 */ unsigned int m_numFaces;
        /* 0x000c */ unsigned int m_numEdges;
        /* 0x0010 */ unsigned int m_numVolumeVerts;
        /* 0x0014 */ CVector* m_volume;
        /* 0x0018 */ CVector* m_verts;
        /* 0x001c */ unsigned short* m_tris;
        /* 0x0020 */ CVector* m_normals;
        /* 0x0024 */ unsigned short* m_edges;
        /* 0x0028 */ m3d::AnimatedModel::Mesh* m_mesh;
        void InsertEdge(unsigned int& numEdges, unsigned short v0, unsigned short v1);
    }; /* size: 0x002c */

    static_assert(sizeof(ShadowVolume) == 0x002c);
}
