#include <decals.h>
#include <m3dapp.h>
#include <algorithm>
#include <client.h>
#include <landscape.h>
#include <world.h>
#include <renderer/i_renderer.h>
#include <core/kernel.h>
#include <core/log.h>
#include <math/matrix.h>
#include <core/scoped_ptr.h>
#include <server/objects/physicbodies/geoms/ray.h>

namespace m3d
{
    rend::IHlslShader* DecalsList::m_decalsVs = nullptr;
    rend::IHlslShader* DecalsList::m_decalsPs = nullptr;
    CMatrix DecalsList::m_identityMatrix;

    // ---------------------------------------------------------------- Decal
    // A decal is built by clipping the mesh it is stamped on against the six
    // planes of the decal's box, then giving every surviving vertex a texture
    // coordinate from its position within that box.

    void Decal::Init(DecalSource const& source, GeometryInfo const& toPutOn)
    {
        // RVA 0x8CD3E0
        decalCenter.x = source.center.x;
        decalCenter.y = source.center.y;
        decalCenter.z = source.center.z;
        decalNormal = source.normal;

        // The third axis of the decal's frame.
        CVector binormal;
        binormal.x = source.normal.y * source.tangent.z - source.normal.z * source.tangent.y;
        binormal.y = source.normal.z * source.tangent.x - source.normal.x * source.tangent.z;
        binormal.z = source.normal.x * source.tangent.y - source.normal.y * source.tangent.x;

        float const d = source.tangent.y * source.center.y + source.tangent.z * source.center.z +
            source.center.x * source.tangent.x;
        leftPlane.x = source.tangent.x;
        leftPlane.y = source.tangent.y;
        leftPlane.z = source.tangent.z;
        leftPlane.w = source.width * 0.5f - d;

        rightPlane.x = -source.tangent.x;
        rightPlane.y = -source.tangent.y;
        rightPlane.z = -source.tangent.z;
        rightPlane.w = source.width * 0.5f + d;

        float const b = source.center.z * binormal.z + source.center.x * binormal.x + source.center.y * binormal.y;
        bottomPlane.x = binormal.x;
        bottomPlane.y = binormal.y;
        bottomPlane.z = binormal.z;
        bottomPlane.w = source.height * 0.5f - b;

        topPlane.x = -binormal.x;
        topPlane.y = -binormal.y;
        topPlane.z = -binormal.z;
        topPlane.w = source.height * 0.5f + b;

        float const n = source.normal.y * source.center.y + source.normal.z * source.center.z +
            source.normal.x * source.center.x;
        frontPlane.x = -source.normal.x;
        frontPlane.y = -source.normal.y;
        frontPlane.z = -source.normal.z;
        frontPlane.w = source.depth + n;

        backPlane.x = source.normal.x;
        backPlane.y = source.normal.y;
        backPlane.z = source.normal.z;
        backPlane.w = source.depth - n;

        decalVertexCount = 0;
        decalTriangleCount = 0;
        ClipMesh(toPutOn);

        // Texture coordinates come from where each vertex sits in the decal's
        // own frame, with the centre at the middle of the texture.
        float const invWidth = 1.0f / source.width;
        float const invHeight = 1.0f / source.height;
        for (int i = 0; i < decalVertexCount; ++i)
        {
            float const dx = vertexArray[i].x - source.center.x;
            float const dy = vertexArray[i].y - source.center.y;
            float const dz = vertexArray[i].z - source.center.z;
            texcoordArray[i].x =
                (source.tangent.x * dx + source.tangent.z * dz + source.tangent.y * dy) * invWidth + 0.5f;
            texcoordArray[i].y = (dz * binormal.z + dy * binormal.y + dx * binormal.x) * invHeight + 0.5f;
        }
    }

    void Decal::ClipMesh(GeometryInfo const& mesh)
    {
        // RVA 0x8CCD00 - NOTE: numIndices is really the triangle count here,
        // not a number of indices.
        CVector newVertex[9];

        for (unsigned i = 0; i < mesh.numIndices; ++i)
        {
            auto const& tri = mesh.indices[i];
            auto const* p0 = reinterpret_cast<float const*>(mesh.vertices + mesh.vertexStride * tri.index[0]);
            auto const* p1 = reinterpret_cast<float const*>(mesh.vertices + mesh.vertexStride * tri.index[1]);
            auto const* p2 = reinterpret_cast<float const*>(mesh.vertices + mesh.vertexStride * tri.index[2]);

            CVector e1(p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]);
            CVector e2(p2[0] - p0[0], p2[1] - p0[1], p2[2] - p0[2]);

            // Unnormalised face normal; the triangle is only worth clipping if
            // it faces the decal within about 75 degrees.
            CVector faceNormal;
            faceNormal.x = e1.y * e2.z - e1.z * e2.y;
            faceNormal.y = e1.z * e2.x - e1.x * e2.z;
            faceNormal.z = e1.x * e2.y - e1.y * e2.x;

            float const facing =
                faceNormal.z * decalNormal.z + faceNormal.y * decalNormal.y + faceNormal.x * decalNormal.x;
            float const len = sqrt(
                faceNormal.z * faceNormal.z + faceNormal.y * faceNormal.y + faceNormal.x * faceNormal.x);
            if (facing <= len * 0.25f)
            {
                continue;
            }

            newVertex[0] = CVector(p0[0], p0[1], p0[2]);
            newVertex[1] = CVector(p1[0], p1[1], p1[2]);
            newVertex[2] = CVector(p2[0], p2[1], p2[2]);

            long const count = ClipPolygon(3, newVertex, newVertex);
            if (count)
            {
                // The vertex or triangle arrays are full; nothing more will fit.
                if (!AddPolygon(count, newVertex))
                {
                    break;
                }
            }
        }
    }

    long Decal::ClipPolygon(long vertexCount, CVector const* vertex, CVector* newVertex) const
    {
        // RVA 0x8CC930 - the six planes in turn, ping-ponging between the
        // caller's buffer and a temporary. Note the caller may pass the same
        // buffer for input and output.
        CVector tempVertex[9];

        long count = ClipPolygonAgainstPlane(leftPlane, vertexCount, vertex, tempVertex);
        if (!count)
        {
            return count;
        }
        count = ClipPolygonAgainstPlane(rightPlane, count, tempVertex, newVertex);
        if (!count)
        {
            return count;
        }
        count = ClipPolygonAgainstPlane(bottomPlane, count, newVertex, tempVertex);
        if (!count)
        {
            return count;
        }
        count = ClipPolygonAgainstPlane(topPlane, count, tempVertex, newVertex);
        if (!count)
        {
            return count;
        }
        count = ClipPolygonAgainstPlane(backPlane, count, newVertex, tempVertex);
        if (!count)
        {
            return count;
        }
        return ClipPolygonAgainstPlane(frontPlane, count, tempVertex, newVertex);
    }

    long Decal::ClipPolygonAgainstPlane(
        CVector4 const& plane,
        long vertexCount,
        CVector const* vertex,
        CVector* newVertex)
    {
        // RVA 0x8CBD70 - Sutherland-Hodgman against a single plane.
        // NOTE: the classification array is a fixed 12 entries with no bound
        // check; nothing here ever clips more than 9 vertices.
        bool negative[12];
        long negativeCount = 0;
        for (long i = 0; i < vertexCount; ++i)
        {
            negative[i] = (vertex[i].x * plane.x + vertex[i].y * plane.y + vertex[i].z * plane.z + plane.w) < 0.0f;
            negativeCount += negative[i];
        }

        // Wholly on the far side of the plane.
        if (negativeCount == vertexCount)
        {
            return 0;
        }

        long count = 0;
        for (long a = 0; a < vertexCount; ++a)
        {
            long const b = a ? a - 1 : vertexCount - 1;
            if (negative[a])
            {
                // Leaving the half-space: keep only the crossing point.
                if (!negative[b])
                {
                    CVector const& v1 = vertex[b];
                    CVector const& v2 = vertex[a];
                    float const t = (v1.z * plane.z + v1.y * plane.y + plane.x * v1.x + plane.w) /
                        ((v1.z - v2.z) * plane.z + (v1.y - v2.y) * plane.y + (v1.x - v2.x) * plane.x);
                    newVertex->x = v1.x * (1.0f - t) + v2.x * t;
                    newVertex->y = v1.y * (1.0f - t) + v2.y * t;
                    newVertex->z = v1.z * (1.0f - t) + v2.z * t;
                    ++newVertex;
                    ++count;
                }
            }
            else
            {
                // Entering the half-space: the crossing point, then the vertex.
                if (negative[b])
                {
                    CVector const& v1 = vertex[a];
                    CVector const& v2 = vertex[b];
                    float const t = (v1.z * plane.z + v1.y * plane.y + plane.x * v1.x + plane.w) /
                        ((v1.y - v2.y) * plane.y + (v1.z - v2.z) * plane.z + (v1.x - v2.x) * plane.x);
                    newVertex->x = v1.x * (1.0f - t) + v2.x * t;
                    newVertex->y = v1.y * (1.0f - t) + v2.y * t;
                    newVertex->z = v1.z * (1.0f - t) + v2.z * t;
                    ++newVertex;
                    ++count;
                }
                newVertex->x = vertex[a].x;
                newVertex->y = vertex[a].y;
                newVertex->z = vertex[a].z;
                ++newVertex;
                ++count;
            }
        }
        return count;
    }

    bool Decal::AddPolygon(long vertexCount, CVector const* vertex)
    {
        // RVA 0x8CB6A0 - the clipped polygon becomes a triangle fan.
        unsigned short const firstVertex = decalVertexCount;
        if (firstVertex + vertexCount >= 100)
        {
            return false;
        }

        unsigned int const firstTriangle = decalTriangleCount;
        decalTriangleCount = firstTriangle + vertexCount - 2;
        for (long i = 2; i < vertexCount; ++i)
        {
            auto& tri = triangleArray[firstTriangle + i - 2];
            tri.index[0] = firstVertex;
            tri.index[1] = firstVertex + i - 1;
            tri.index[2] = firstVertex + i;
        }

        for (long i = 0; i < vertexCount; ++i)
        {
            vertexArray[firstVertex + i].x = vertex[i].x;
            vertexArray[firstVertex + i].y = vertex[i].y;
            vertexArray[firstVertex + i].z = vertex[i].z;
        }

        decalVertexCount = static_cast<unsigned short>(firstVertex + vertexCount);
        return true;
    }

    // ------------------------------------------------------------ DecalsList

    void DecalsList::ReleaseShaders()
    {
        // RVA 0x8CB740
        if (m_decalsVs)
        {
            m_decalsVs->Release();
            m_decalsVs = nullptr;
        }
        if (m_decalsPs)
        {
            m_decalsPs->Release();
            m_decalsPs = nullptr;
        }
    }

    DecalsList::~DecalsList()
    {
        // RVA 0x8CEF80 - the vertex and index pool fields are the only things
        // that are not cleaned up by the members' own destructors.
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
        M3D_RENDERER->ReleaseVbPoolField(m_VbPoolField);
    }

    unsigned DecalsList::GetNumDecals()
    {
        // RVA 0x6CFF30
        return m_decals.size();
    }

    // The two pools every decal list shares; see the constructor.
    unsigned const IB_POOL_SIZE = 2000;
    unsigned const VB_POOL_SIZE = 1000;

    namespace
    {
        // Scratch copies of the pools, so decals can be compacted in place.
        unsigned short indexHelper[2000];
        rend::VertexXYZT1I vertexHelper[1000];
    }  // namespace

    void DecalsList::RecalcDecalsForMesh(GeometryInfo const& newGeometry)
    {
        // RVA 0x8CF560 - a part's mesh was swapped (e.g. for a damaged model): decals on the old mesh are projected
        // onto the new one and every decal is packed again from the start of the pools.
        unsigned short* const buffIndices = static_cast<unsigned short*>(M3D_RENDERER->LockIbPoolField(m_IbPoolField));
        memcpy(indexHelper, buffIndices, 2 * m_numIds);
        auto* const buffVertices = static_cast<rend::VertexXYZT1I*>(M3D_RENDERER->LockVbPoolField(m_VbPoolField));
        memcpy(vertexHelper, buffVertices, sizeof(rend::VertexXYZT1I) * m_numVerts);

        unsigned ibOffset = 0;
        unsigned short vbOffset = 0;
        m_transforms.resize(0);
        m_invBindtransforms.resize(0);
        CMatrix* const transform = newGeometry.transform ? newGeometry.transform : &m_identityMatrix;
        short const newMatId = GetMatrixId(transform);

        for (auto it = m_decals.begin(); it != m_decals.end();)
        {
            DecalInfo& decal = *it;
            if (decal.mesh == newGeometry.oldMesh)
            {
                // Cast a two unit ray back along the decal's normal onto the new mesh.
                static scoped_ptr<ai::Ray> ray(ai::Ray::CreateObject(nullptr, 2.0f, nullptr));
                dGeomSetPosition(
                    ray->GetGeomId(),
                    decal.source.center.x - decal.source.normal.x,
                    decal.source.center.y - decal.source.normal.y,
                    decal.source.center.z - decal.source.normal.z);
                ray->SetDirection(decal.source.normal);

                dContactGeom meshContacts[3];
                int const numContacts = dCollide(
                    ray->GetGeomId(), static_cast<ai::Geom*>(newGeometry.mesh)->GetGeomId(), 3, meshContacts,
                    sizeof(dContactGeom));
                if (!numContacts)
                {
                    it = m_decals.erase(it);
                    continue;
                }

                // The contact closest to the old centre wins.
                CVector pos(meshContacts[0].pos[0], meshContacts[0].pos[1], meshContacts[0].pos[2]);
                float invLen = static_cast<float>(
                    1.0 / sqrt(
                              static_cast<double>(meshContacts[0].normal[1]) * meshContacts[0].normal[1] +
                              static_cast<double>(meshContacts[0].normal[2]) * meshContacts[0].normal[2] +
                              static_cast<double>(meshContacts[0].normal[0]) * meshContacts[0].normal[0] +
                              0.00000011920929f));
                CVector normal(
                    invLen * meshContacts[0].normal[0], meshContacts[0].normal[1] * invLen,
                    meshContacts[0].normal[2] * invLen);
                for (int i = 1; i < numContacts; ++i)
                {
                    dContactGeom const& contact = meshContacts[i];
                    float const cdx = contact.pos[0] - decal.source.center.x;
                    float const cdz = contact.pos[2] - decal.source.center.z;
                    float const cdy = contact.pos[1] - decal.source.center.y;
                    float const contactDistSq = cdx * cdx + cdz * cdz + cdy * cdy;
                    double const dx = pos.x - decal.source.center.x;
                    double const dy = pos.y - decal.source.center.y;
                    double const dz = pos.z - decal.source.center.z;
                    if (sqrt(dx * dx + dz * dz + dy * dy) > sqrt(contactDistSq))
                    {
                        pos = CVector(contact.pos[0], contact.pos[1], contact.pos[2]);
                        invLen = static_cast<float>(
                            1.0 / sqrt(
                                      static_cast<double>(contact.normal[0]) * contact.normal[0] +
                                      static_cast<double>(contact.normal[2]) * contact.normal[2] +
                                      static_cast<double>(contact.normal[1]) * contact.normal[1] +
                                      0.00000011920929f));
                        normal = CVector(invLen * contact.normal[0], contact.normal[1] * invLen, contact.normal[2] * invLen);
                    }
                }

                // Tangent: normal x (normal + up), or the x axis when that degenerates.
                CVector helper(
                    normal.z * normal.y - (normal.y + 1.0f) * normal.z,
                    normal.z * normal.x - normal.z * normal.x,
                    (normal.y + 1.0f) * normal.x - normal.y * normal.x);
                if (sqrt(
                        static_cast<double>(helper.x) * helper.x + static_cast<double>(helper.z) * helper.z +
                        static_cast<double>(helper.y) * helper.y) < 0.0000099999997f)
                {
                    helper = CVector(1.0f, 0.0f, 0.0f);
                }
                float const invTangentLen = static_cast<float>(
                    1.0 / sqrt(
                              static_cast<double>(helper.x) * helper.x + static_cast<double>(helper.z) * helper.z +
                              static_cast<double>(helper.y) * helper.y + 0.00000011920929f));

                decal.source.center = pos;
                decal.source.normal = normal;
                decal.source.tangent = CVector(invTangentLen * helper.x, helper.y * invTangentLen, helper.z * invTangentLen);
                m_workDecal.Init(decal.source, newGeometry);
                decal.mesh = newGeometry.mesh;
                if (!m_workDecal.decalVertexCount || !m_workDecal.decalTriangleCount)
                {
                    it = m_decals.erase(it);
                    continue;
                }
                if (ibOffset + 3 * m_workDecal.decalTriangleCount > IB_POOL_SIZE ||
                    vbOffset + static_cast<unsigned>(m_workDecal.decalVertexCount) > VB_POOL_SIZE)
                {
                    // Out of room: this decal and everything after it are dropped.
                    m_decals.erase(it, m_decals.end());
                    break;
                }

                decal.ibOffset = ibOffset;
                decal.vbOffset = vbOffset;
                decal.sizeInIb = 3 * m_workDecal.decalTriangleCount;
                decal.sizeInVb = m_workDecal.decalVertexCount;
                decal.mat = transform;
                for (unsigned t = 0; t < m_workDecal.decalTriangleCount; ++t)
                {
                    buffIndices[ibOffset + 3 * t] = vbOffset + m_workDecal.triangleArray[t].index[0];
                    buffIndices[ibOffset + 3 * t + 1] = vbOffset + m_workDecal.triangleArray[t].index[1];
                    buffIndices[ibOffset + 3 * t + 2] = vbOffset + m_workDecal.triangleArray[t].index[2];
                }
                ibOffset += 3 * m_workDecal.decalTriangleCount;
                for (unsigned v = 0; v < m_workDecal.decalVertexCount; ++v)
                {
                    rend::VertexXYZT1I& vertex = buffVertices[vbOffset + v];
                    vertex.x = m_workDecal.vertexArray[v].x;
                    vertex.y = m_workDecal.vertexArray[v].y;
                    vertex.z = m_workDecal.vertexArray[v].z;
                    vertex.tu = m_workDecal.texcoordArray[v].x;
                    vertex.tv = m_workDecal.texcoordArray[v].y;
                    vertex.i1 = newMatId;
                }
                vbOffset += m_workDecal.decalVertexCount;
                ++it;
            }
            else
            {
                if (ibOffset + decal.sizeInIb > IB_POOL_SIZE || vbOffset + static_cast<unsigned>(decal.sizeInVb) > VB_POOL_SIZE)
                {
                    m_decals.erase(it, m_decals.end());
                    break;
                }
                // Other decals are copied down from the scratch copies, rebased onto the new offsets.
                short const matId = GetMatrixId(decal.mat);
                for (unsigned i = 0; i < decal.sizeInIb; ++i)
                {
                    buffIndices[ibOffset + i] = vbOffset + (indexHelper[i + decal.ibOffset] - decal.vbOffset);
                }
                for (unsigned v = 0; v < decal.sizeInVb; ++v)
                {
                    rend::VertexXYZT1I const& from = vertexHelper[v + decal.vbOffset];
                    rend::VertexXYZT1I& to = buffVertices[vbOffset + v];
                    to.x = from.x;
                    to.y = from.y;
                    to.z = from.z;
                    to.tu = from.tu;
                    to.tv = from.tv;
                    to.i1 = matId;
                }
                decal.ibOffset = ibOffset;
                decal.vbOffset = vbOffset;
                ibOffset += decal.sizeInIb;
                vbOffset += decal.sizeInVb;
                ++it;
            }
        }

        m_vbStart = vbOffset;
        m_numVerts = vbOffset;
        m_ibStart = ibOffset;
        m_numIds = ibOffset;
        M3D_RENDERER->UnlockVbPoolField(m_VbPoolField);
        M3D_RENDERER->UnlockIbPoolField(m_IbPoolField);
    }

    DecalsList::DecalsList()
    {
        m_numVerts = 0;
        m_numIds = 0;
        m_vbStart = 0;
        m_ibStart = 0;
        m_IbPoolField = M3D_RENDERER->AddIbPoolField(IB_POOL_SIZE);
        m_VbPoolField = M3D_RENDERER->AddVbPoolField(rend::VERTEX_XYZT1I, VB_POOL_SIZE);
    }

    DecalInfo const& DecalsList::GetDecal(int i)
    {
        // RVA 0x6D3A90 - NOTE: the index is not range checked.
        return m_decals[i];
    }

    void DecalsList::Render(rend::TexHandle tex)
    {
        // RVA 0x8CD7C0 - draws every decal in this list in one call. The decals
        // share a vertex and index pool, so all that is needed per frame is the
        // set of bone transforms they hang off.

        // Each transform costs four vertex shader constants and ten are spoken
        // for, so a mesh with too many bones simply does not get decals.
        unsigned const maxTransforms =
            static_cast<unsigned>((M3D_RENDERER->GetMaxVertexShaderConst() - 10) / 4);
        if (m_transforms.size() > maxTransforms)
        {
            M3D_LOG_INFO("Decals: too much transforms for shader");
            return;
        }

        m_decalsVs->Apply();
        m_decalsPs->Apply();

        // Decals are scaled a hair outwards so they do not fight with the
        // surface they are stamped on.
        CMatrix sc;
        memset(&sc, 0, sizeof(sc));
        sc._11 = 1.012f;
        sc._22 = 1.012f;
        sc._33 = 1.012f;
        sc._44 = 1.0f;

        CMatrix const wv = sc * M3D_RENDERER->MatGet();
        CMatrix const wvp = wv * M3D_RENDERER->MatGetProj();

        // Fog is applied in the shader, so it needs the range packed as
        // (end, 1 / (end - start), start).
        float fogStart = 0.0f;
        float fogEnd = 0.0f;
        m3d::pClient->GetWorld().GetLandscape().GetFogStartAndEnd(fogStart, fogEnd);
        float const fogFactor = m3d::pClient->GetWorld().GetWeatherManager().GetFogReduceFactorFromWeather();

        CVector fogTerm;
        fogTerm.x = fogFactor * fogEnd;
        fogTerm.y = 1.0f / (fogFactor * fogEnd - fogFactor * fogStart);
        fogTerm.z = fogFactor * fogStart;
        m_decalsVs->SetVector3(m_decalsVs->GetParamHandleByName("g_FogTerm"), fogTerm);

        // One combined matrix per bone: undo the bind pose, follow the bone,
        // then go all the way to clip space.
        CMatrix matrices[100];
        for (unsigned i = 0; i < m_transforms.size(); ++i)
        {
            matrices[i] = (m_invBindtransforms[i] * (*m_transforms[i])) * wvp;
        }
        m_decalsVs->SetMatrixArray(
            m_decalsVs->GetParamHandleByName("matrices"), matrices, m_transforms.size());

        M3D_RENDERER->SetTexture(0, tex, -1.0);
        M3D_RENDERER->SetToStream0(m_VbPoolField);
        M3D_RENDERER->SetIndices(m_IbPoolField, m_VbPoolField.RealOffset);

        // The shipped code has these four pushes and their matching pops
        // inlined; they are exactly overlayStart / overlayStop.
        overlayStart();
        M3D_RENDERER->DrawIndexedPrimitiveShader(
            rend::M3DPT_TRIANGLELIST, 0, m_numVerts, m_IbPoolField.RealOffset, m_numIds / 3);
        overlayStop();
    }

    void DecalsList::AddDecal(DecalSource const& source, GeometryInfo const& toPutOn)
    {
        // RVA 0x8CF0B0 - clips the decal against the mesh, finds room for it in
        // the shared vertex and index pools (evicting the oldest decals if need
        // be) and writes the geometry straight into them.

        // Decals landing almost on top of the last one are dropped, so a burst
        // of hits in one spot does not fill the pool with overlapping copies.
        float const smallestSide = (source.width > source.height) ? source.height : source.width;
        float const dx = source.center.x - m_workDecal.decalCenter.x;
        float const dy = source.center.y - m_workDecal.decalCenter.y;
        float const dz = source.center.z - m_workDecal.decalCenter.z;
        if (smallestSide * 0.2f > sqrt(dz * dz + dy * dy + dx * dx))
        {
            return;
        }

        m_workDecal.Init(source, toPutOn);
        if (!m_workDecal.decalVertexCount || !m_workDecal.decalTriangleCount)
        {
            return;
        }

        unsigned const vertCount = m_workDecal.decalVertexCount;
        unsigned const idCount = 3 * m_workDecal.decalTriangleCount;

        // Both pools are ring buffers: m_vbStart / m_ibStart are the write head
        // and the oldest decal's offsets are the tail.
        unsigned ibAvail = 0;
        unsigned vbAvail = 0;
        for (;;)
        {
            if (m_decals.empty())
            {
                m_ibStart = 0;
                m_vbStart = 0;
                m_numVerts = vertCount;
                m_numIds = idCount;
                break;
            }

            bool wrapped = false;
            if (m_ibStart > m_decals.front().ibOffset && m_vbStart > m_decals.front().vbOffset)
            {
                // The head is past the tail, so the free run reaches the end of
                // the pools.
                ibAvail = IB_POOL_SIZE - m_ibStart;
                vbAvail = VB_POOL_SIZE - m_vbStart;
                if (ibAvail >= idCount && vbAvail >= vertCount)
                {
                    m_numVerts = m_vbStart + vertCount;
                    m_numIds = m_ibStart + idCount;
                }
                else
                {
                    // Not enough room left at the end: start over from zero and
                    // let the next pass measure against the oldest decal.
                    m_ibStart = 0;
                    m_vbStart = 0;
                }
                wrapped = true;
            }

            if (!wrapped)
            {
                // The head is behind the tail, so only the gap between them is
                // free.
                ibAvail = m_decals.front().ibOffset - m_ibStart;
                vbAvail = m_decals.front().vbOffset - m_vbStart;
                if (ibAvail < idCount || vbAvail < vertCount)
                {
                    m_decals.pop_front();

                    // NOTE: the shipped code reads the front unconditionally
                    // here, even when that pop emptied the list. The guard is
                    // ours; everything else matches.
                    if (!m_decals.empty() && m_decals.front().ibOffset == 0)
                    {
                        m_numVerts = m_vbStart;
                        m_numIds = m_ibStart;
                    }
                }
            }

            // NOTE: this re-test deliberately uses the room measured above
            // rather than re-measuring, so the wrap case always goes round once
            // more before the decal is placed.
            if (ibAvail >= idCount && vbAvail >= vertCount)
            {
                break;
            }
        }

        // A decal on a skinned mesh follows its bone; everything else uses the
        // shared identity so the shader can treat both alike.
        CMatrix* transform = toPutOn.transform;
        if (!transform)
        {
            transform = &m_identityMatrix;
        }
        short const matrixId = GetMatrixId(transform);

        DecalInfo decalInfo;
        decalInfo.source = source;
        decalInfo.mesh = toPutOn.mesh;
        decalInfo.vbOffset = m_vbStart;
        decalInfo.ibOffset = m_ibStart;
        decalInfo.sizeInVb = m_workDecal.decalVertexCount;
        decalInfo.sizeInIb = idCount;
        decalInfo.mat = transform;
        m_decals.push_back(decalInfo);

        auto* indices = static_cast<unsigned short*>(M3D_RENDERER->LockIbPoolField(m_IbPoolField));
        for (unsigned i = 0; i < m_workDecal.decalTriangleCount; ++i)
        {
            auto const& tri = m_workDecal.triangleArray[i];
            indices[m_ibStart + 3 * i + 0] = static_cast<unsigned short>(m_vbStart + tri.index[0]);
            indices[m_ibStart + 3 * i + 1] = static_cast<unsigned short>(m_vbStart + tri.index[1]);
            indices[m_ibStart + 3 * i + 2] = static_cast<unsigned short>(m_vbStart + tri.index[2]);
        }
        m_ibStart += idCount;

        // Everything between this decal and the next one is filled with
        // degenerate triangles so the whole range can be drawn in one call.
        // NOTE: m_ibStart itself is not advanced past the padding.
        for (unsigned pad = m_ibStart; !m_decals.empty() && pad < m_decals.front().ibOffset; ++pad)
        {
            indices[pad] = 0;
        }
        M3D_RENDERER->UnlockIbPoolField(m_IbPoolField);

        // VERTEX_XYZT1I: position, texture coordinate, and the matrix index
        // packed into the low half of the last float.
        auto* verts = static_cast<float*>(M3D_RENDERER->LockVbPoolField(m_VbPoolField));
        for (unsigned i = 0; i < m_workDecal.decalVertexCount; ++i)
        {
            float* v = verts + 6 * (m_vbStart + i);
            v[0] = m_workDecal.vertexArray[i].x;
            v[1] = m_workDecal.vertexArray[i].y;
            v[2] = m_workDecal.vertexArray[i].z;
            v[3] = m_workDecal.texcoordArray[i].x;
            v[4] = m_workDecal.texcoordArray[i].y;
            *reinterpret_cast<unsigned short*>(&v[5]) = static_cast<unsigned short>(matrixId);
        }
        m_vbStart += m_workDecal.decalVertexCount;
        M3D_RENDERER->UnlockVbPoolField(m_VbPoolField);
    }

    void DecalsList::LoadShaders()
    {
        if (m_decalsVs)
        {
            m_decalsVs->Release();
            m_decalsVs = nullptr;
        }
        m_decalsVs = Application::g_pApp->m_renderer->NewHlslShader("data/shaders/decals.vs", "DecalsVS", rend::IHlslShader::VS_1_1);
        if (!m_decalsVs)
        {
            SYS_ERROR("m_decalsVs");
        }

        if (m_decalsPs)
        {
            m_decalsPs->Release();
            m_decalsPs = nullptr;
        }
        m_decalsPs = Application::g_pApp->m_renderer->NewHlslShader("data/shaders/decals.ps", "DecalsPS", rend::IHlslShader::PS_1_1);
        if (!m_decalsPs)
        {
            SYS_ERROR("m_decalsPs");
        }

        memset(&m_identityMatrix, 0, sizeof(m_identityMatrix));
        m_identityMatrix._44 = 1.0;
        m_identityMatrix._33 = 1.0;
        m_identityMatrix._22 = 1.0;
        m_identityMatrix._11 = 1.0;
    }

    short DecalsList::GetMatrixId(CMatrix* transform)
    {
        // RVA 0x8CF020 - decals on skinned meshes follow a bone, and the shader
        // is handed the bones as a small array, so each distinct transform gets
        // an index. Its inverse is kept alongside to undo the bind pose.
        auto const it = std::find(m_transforms.begin(), m_transforms.end(), transform);
        if (it != m_transforms.end())
        {
            return static_cast<short>(it - m_transforms.begin());
        }

        m_transforms.push_back(transform);
        m_invBindtransforms.push_back(transform->getInverse());

        // NOTE: the -1 is unreachable, the push_back above just filled the list.
        if (m_transforms.empty())
        {
            return -1;
        }
        return static_cast<short>(m_transforms.size() - 1);
    }
}
