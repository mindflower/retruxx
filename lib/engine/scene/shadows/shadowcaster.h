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
        void Initialize(m3d::AnimatedModel::Mesh const& mesh, unsigned short* edgesBuf);
        unsigned int BuildShadowVolume(CVector const& lightDir, CVector* volumeVertexPool);
        void Render(CMatrix const& objToWorld, unsigned int color) const;
        unsigned int GetNumVerts() const;
        CVector const* GetVerts() const;
        void Invalidate();

    private:
        bool m_bIsLocked;
        unsigned int m_numVerts;
        unsigned int m_numFaces;
        unsigned int m_numEdges;
        unsigned int m_numVolumeVerts;
        CVector* m_volume;
        CVector* m_verts;
        unsigned short* m_tris;
        CVector* m_normals;
        unsigned short* m_edges;
        m3d::AnimatedModel::Mesh* m_mesh;
        void InsertEdge(unsigned int& numEdges, unsigned short v0, unsigned short v1);
    };
}  // namespace m3d
