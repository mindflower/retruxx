#include "shadowcaster.h"

#include <m3dapp.h>
#include <math/matrix.h>
#include <math/vector.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    ShadowVolume::ShadowVolume()
    {
        // RVA 0x954780
        m_bIsLocked = false;
        m_numEdges = 0;
    }

    ShadowVolume::~ShadowVolume()
    {
        // RVA 0x954790
    }

    void ShadowVolume::Initialize(const AnimatedModel::Mesh& mesh, unsigned short* edgesBuf)
    {
        // RVA 0x9547A0 - the volume works on the welded copy of the mesh, sharing one edge buffer with every other
        // volume.
        m_mesh = const_cast<AnimatedModel::Mesh*>(&mesh);
        m_numFaces = mesh.m_numFacesWelded;
        m_numVerts = mesh.m_numVertsWelded;
        m_tris = mesh.m_trisWelded;
        m_verts = mesh.m_vertsWelded;
        m_normals = mesh.m_faceNormals;
        m_edges = edgesBuf;
    }

    void ShadowVolume::Invalidate()
    {
        // RVA 0x9547E0
    }

    void ShadowVolume::InsertEdge(unsigned int& numEdges, unsigned short v0, unsigned short v1)
    {
        // RVA 0x9546E0 - an edge shared by two lit faces cancels out, leaving only the silhouette. A removed edge is
        // replaced by the last one.
        for (unsigned int i = 0; i < numEdges; ++i)
        {
            if ((m_edges[2 * i] == v0 && m_edges[2 * i + 1] == v1) || (m_edges[2 * i] == v1 && m_edges[2 * i + 1] == v0))
            {
                if (numEdges > 1)
                {
                    m_edges[2 * i] = m_edges[2 * numEdges - 2];
                    m_edges[2 * i + 1] = m_edges[2 * numEdges - 1];
                }
                --numEdges;
                return;
            }
        }
        m_edges[2 * numEdges] = v0;
        m_edges[2 * numEdges + 1] = v1;
        ++numEdges;
    }

    unsigned int ShadowVolume::BuildShadowVolume(const CVector& lightDir, CVector* volumeVertexPool)
    {
        // RVA 0x954810 - every silhouette edge becomes a quad (two triangles) stretched 25 units along the light.
        m_volume = volumeVertexPool;
        m_numVolumeVerts = 0;
        unsigned int numEdges = 0;
        for (unsigned int nFace = 0; nFace < m_numFaces; ++nFace)
        {
            CVector const& n = m_normals[nFace];
            if (n.y * lightDir.y + n.z * lightDir.z + lightDir.x * n.x > 0.0f)
            {
                unsigned short const* const tri = &m_tris[3 * nFace];
                InsertEdge(numEdges, tri[0], tri[1]);
                InsertEdge(numEdges, tri[1], tri[2]);
                InsertEdge(numEdges, tri[2], tri[0]);
            }
        }

        float const ex = lightDir.x * 25.0f;
        float const ey = lightDir.y * 25.0f;
        float const ez = lightDir.z * 25.0f;
        for (unsigned int i = 0; i < numEdges; ++i)
        {
            CVector const& v1 = m_verts[m_edges[2 * i]];
            CVector const& v2 = m_verts[m_edges[2 * i + 1]];
            CVector const v3(v1.x + ex, v1.y + ey, v1.z + ez);
            CVector const v4(v2.x + ex, v2.y + ey, v2.z + ez);
            m_volume[m_numVolumeVerts++] = v1;
            m_volume[m_numVolumeVerts++] = v4;
            m_volume[m_numVolumeVerts++] = v2;
            m_volume[m_numVolumeVerts++] = v1;
            m_volume[m_numVolumeVerts++] = v3;
            m_volume[m_numVolumeVerts++] = v4;
        }
        return m_numVolumeVerts;
    }

    void ShadowVolume::Render(const CMatrix& objToWorld, unsigned int) const
    {
        // RVA 0x955160 - NOTE: the colour argument is ignored; only the stencil counts matter.
        if (!m_numVolumeVerts)
        {
            return;
        }
        auto* const renderer = M3D_RENDERER;
        rend::VbHandle const vb = renderer->GetVbStreaming(rend::VERTEX_XYZ);
        int vofs = 0;
        auto* const dest = static_cast<CVector*>(renderer->LockVbStreaming(vb, m_numVolumeVerts, vofs, nullptr));
        for (unsigned int i = 0; i < m_numVolumeVerts; ++i)
        {
            dest[i] = m_volume[i];
        }
        renderer->UnlockVb(vb);
        renderer->SetToStream0(vb);
        renderer->MatSetWorld(objToWorld);
        renderer->DrawPrimitive(rend::M3DPT_TRIANGLELIST, vofs, m_numVolumeVerts / 3);
    }
}
