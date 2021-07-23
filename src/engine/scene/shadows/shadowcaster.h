#pragma once

namespace m3d
{
    class ShadowVolume
    {
    public:
        ~ShadowVolume();
        void Render(struct CMatrix const &,unsigned int) const ;
        void Invalidate();
        unsigned int BuildShadowVolume(struct CVector const &,struct CVector *);
        ShadowVolume();
        void Initialize(AnimatedModel::Mesh const &,unsigned short *);
    protected:
    private:
        void InsertEdge(unsigned int &,unsigned short,unsigned short);
        bool m_bIsLocked;
        unsigned int m_numVerts;
        unsigned int m_numFaces;
        unsigned int m_numEdges;
        unsigned int m_numVolumeVerts;
        CVector *m_volume;
        CVector *m_verts;
        unsigned __int16 *m_tris;
        CVector *m_normals;
        unsigned __int16 *m_edges;
        AnimatedModel::Mesh *m_mesh;
    };
}
