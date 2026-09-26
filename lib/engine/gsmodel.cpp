#include <gsmodel.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <file/tagged.h>
#include <cstring>

namespace m3d
{
    namespace
    {
        // Header of an untagged ("GSSM") model file.
        struct GsmHeader
        {
            /* 0x0000 */ char m_id[4];
            /* 0x0004 */ int m_version;
            /* 0x0008 */ unsigned int m_numVertices;
            /* 0x000c */ unsigned int m_numFaces;
            /* 0x0010 */ char m_alpha;
            /* 0x0011 */ char m_texname[128];
        }; /* size: 0x0094 */
        static_assert(sizeof(GsmHeader) == 0x94);

        // The engine winds triangles the other way round from the files.
        void FlipWinding(unsigned short* indices, unsigned int numFaces)
        {
            for (unsigned int i = 0; i < numFaces; ++i)
            {
                unsigned short const first = indices[3 * i];
                indices[3 * i] = indices[3 * i + 2];
                indices[3 * i + 2] = first;
            }
        }
    }

    int CGSModel::Load(const CStr& fname)
    {
        // RVA 0x8F9D80 - reads a tagged "GSM" file or, failing that, an untagged "GSSM" one, then loads the texture
        // named in the header from the model's directory. Vertices are 32 bytes (position, normal, one uv set).
        fs::auxTaggedFile file;
        GsmHeader Header;
        char* formatTitle;
        fs::auxTaggedFile::eError const openResult = file.Open(fname.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC);
        if (openResult == fs::auxTaggedFile::BAD_FORMAT)
        {
            scoped_ptr<fs::FileStream> stream(g_Kernel->GetFileServer().CreateFileStream());
            if (!stream->Open(fname.c_str(), fs::IStream::OPEN_READ))
            {
                M3D_LOG_INFO(CStr("Cannot read file: ") + fname);
                return 0;
            }
            stream->ReadBytes(&Header, sizeof(Header));
            if (Header.m_version != 3 && Header.m_version != 2)
            {
                M3D_LOG_INFO(CStr("GSSM Load (") + fname + CStr("): unsupported version (") + CStr(Header.m_version)
                             + CStr("), wants (") + CStr(3) + CStr(")"));
                return 0;
            }
            m_header.m_numVertices = Header.m_numVertices;
            m_header.m_numFaces = Header.m_numFaces;
            m_header.m_alpha = Header.m_alpha;
            m_verts = g_Kernel->g_mar.AllocMem(32 * m_header.m_numVertices, nullptr, 0);
            stream->ReadBytes(m_verts, 32 * m_header.m_numVertices);
            m_triIndices = static_cast<unsigned short*>(g_Kernel->g_mar.AllocMem(6 * m_header.m_numFaces, nullptr, 0));
            stream->ReadBytes(m_triIndices, 6 * m_header.m_numFaces);
            FlipWinding(m_triIndices, m_header.m_numFaces);
            stream->Close();
            if (Header.m_version == 2)
            {
                // Version 2 files store inverted normals.
                float* normal = reinterpret_cast<float*>(static_cast<char*>(m_verts) + 12);
                for (unsigned int i = 0; i < m_header.m_numVertices; ++i, normal += 8)
                {
                    normal[0] = 0.0f - normal[0];
                    normal[1] = 0.0f - normal[1];
                    normal[2] = 0.0f - normal[2];
                }
            }
        }
        else
        {
            if (openResult != fs::auxTaggedFile::SUCCESS)
            {
                return 0;
            }
            file.getFormatTitle(&formatTitle);
            if (strcmp(formatTitle, "GSM"))
            {
                M3D_LOG_INFO(fname + CStr(" is not tagged GSM"));
                file.Close();
                return 0;
            }
            unsigned int formatVersion;
            file.getFormatVersion(formatVersion);
            // NOTE: a version 0 file loads nothing and ends up reported as a sick model below.
            if (formatVersion)
            {
                file.getChunkDataCopy(1u, &Header);
                m_header.m_numVertices = Header.m_numVertices;
                m_header.m_numFaces = Header.m_numFaces;
                m_header.m_alpha = Header.m_alpha;
                m_verts = g_Kernel->g_mar.AllocMem(32 * Header.m_numVertices, nullptr, 0);
                file.getChunkDataCopy(2u, m_verts);
                // Tagged files are Z-up: swap Y and Z of the positions and the normals.
                unsigned int* v = reinterpret_cast<unsigned int*>(m_verts);
                for (unsigned int i = 0; i < m_header.m_numVertices; ++i, v += 8)
                {
                    unsigned int const py = v[1];
                    v[1] = v[2];
                    v[2] = py;
                    unsigned int const ny = v[4];
                    v[4] = v[5];
                    v[5] = ny;
                }
                m_triIndices = static_cast<unsigned short*>(g_Kernel->g_mar.AllocMem(6 * m_header.m_numFaces, nullptr, 0));
                file.getChunkDataCopy(3u, m_triIndices);
                FlipWinding(m_triIndices, m_header.m_numFaces);
            }
            if (formatVersion >= 2 && file.isChunkPresent(4u))
            {
                void* data;
                file.getChunkData(4u, &data);
                int const numLoadPoints = *static_cast<int*>(data);
                m_loadPoints = static_cast<LPoint*>(g_Kernel->g_mar.AllocMem(sizeof(LPoint) * numLoadPoints, nullptr, 0));
                m_numLoadPoints = numLoadPoints;
                memcpy(m_loadPoints, static_cast<char*>(data) + 4, sizeof(LPoint) * numLoadPoints);
            }
            if (formatVersion >= 3 && file.isChunkPresent(5u))
            {
                file.getChunkDataCopy(5u, &m_col_header);
                if (m_col_header.numFaces && m_col_header.numVertices)
                {
                    M3D_LOG_INFO(CStr("Collision data for ") + fname + CStr(": ") + CStr(m_col_header.numFaces)
                                 + CStr(" faces, ") + CStr(m_col_header.numVertices) + CStr(" vertices"));
                    m_col_verts = static_cast<CollidingVertex*>(
                        g_Kernel->g_mar.AllocMem(sizeof(CollidingVertex) * m_col_header.numVertices, nullptr, 0));
                    file.getChunkDataCopy(6u, m_col_verts);
                    m_col_idx = static_cast<unsigned short*>(g_Kernel->g_mar.AllocMem(6 * m_col_header.numFaces, nullptr, 0));
                    file.getChunkDataCopy(7u, m_col_idx);
                    for (unsigned int i = 0; i < m_col_header.numVertices; ++i)
                    {
                        float const y = m_col_verts[i].y;
                        m_col_verts[i].y = m_col_verts[i].z;
                        m_col_verts[i].z = y;
                    }
                    FlipWinding(m_col_idx, m_col_header.numFaces);
                }
                else
                {
                    M3D_LOG_INFO(CStr("Collision header present, but no actual data found"));
                }
            }
            file.Close();
        }

        m_VertType = rend::VERTEX_XYZNT1;
        m_VertTypeSize = 32;
        memset(&m_SurfaceMaterial.material, 0, sizeof(m_SurfaceMaterial.material));
        m_SurfaceMaterial.material.m_diffuse.r = 1.0f;
        m_SurfaceMaterial.material.m_ambient.r = 1.0f;
        m_SurfaceMaterial.material.m_diffuse.g = 1.0f;
        m_SurfaceMaterial.material.m_ambient.g = 1.0f;
        m_SurfaceMaterial.material.m_diffuse.b = 1.0f;
        m_SurfaceMaterial.material.m_ambient.b = 1.0f;
        m_SurfaceMaterial.material.m_diffuse.a = 1.0f;
        m_SurfaceMaterial.material.m_ambient.a = 1.0f;
        m_SurfaceMaterial.Textures.push_back(DTextureInfo());
        DTextureInfo& texture = m_SurfaceMaterial.Textures.back();
        texture.FileName = Header.m_texname;
        texture.Type = DIFFUSE;
        texture.UV_Set = 0;
        CStr texName = DirectoryFromFileName(fname) + CStr("\\") + CStr(Header.m_texname);
        UnifyFileName(texName);
        texture.Handle = M3D_RENDERER->AddTexture(texName, 2);
        CalculateBBox();
        if (!m_triIndices || !m_header.m_numFaces || !m_verts || !m_header.m_numVertices)
        {
            // NOTE: there is no separator before "numVerts=".
            M3D_LOG_INFO(CStr("ERROR GSModel: file ") + fname + CStr(" contains sick model: numTris=")
                         + CStr(m_header.m_numFaces) + CStr("numVerts=") + CStr(m_header.m_numVertices));
            return 0;
        }
        return 1;
    }

    CGSModel::CGSModel()
    {
        this->m_triIndices = 0;
        this->m_verts = 0;
        this->m_drawIndices = 0;
        this->m_drawVerts = 0;
        this->m_numDrawIndices = 0;
        this->m_numDrawVerts = 0;
        this->m_col_header.numFaces = 0;
        this->m_col_header.numVertices = 0;
        this->m_col_verts = 0;
        this->m_col_idx = 0;
        this->m_loadPoints = 0;
        this->m_numLoadPoints = 0;
    }

    CGSModel::~CGSModel()
    {
        Release();
    }

    void CGSModel::Release()
    {
        // RVA 0x8F9900 - the texture entries themselves are kept.
        for (DTextureInfo& texture : m_SurfaceMaterial.Textures)
        {
            M3D_RENDERER->ReleaseTexture(texture.Handle);
        }
        if (m_triIndices)
        {
            g_Kernel->g_mar.FreeMem(m_triIndices, nullptr, 0);
        }
        m_triIndices = nullptr;
        if (m_verts)
        {
            g_Kernel->g_mar.FreeMem(m_verts, nullptr, 0);
        }
        m_verts = nullptr;
        if (m_col_verts)
        {
            g_Kernel->g_mar.FreeMem(m_col_verts, nullptr, 0);
        }
        m_col_verts = nullptr;
        if (m_col_idx)
        {
            g_Kernel->g_mar.FreeMem(m_col_idx, nullptr, 0);
        }
        m_col_idx = nullptr;
        if (m_loadPoints)
        {
            g_Kernel->g_mar.FreeMem(m_loadPoints, nullptr, 0);
        }
        m_loadPoints = nullptr;
    }

    void CGSModel::CalculateBBox()
    {
        // RVA 0x8F8E50
        m_box.m_box[0] = 10000.0f;
        m_box.m_box[3] = -10000.0f;
        m_box.m_box[1] = 10000.0f;
        m_box.m_box[4] = -10000.0f;
        m_box.m_box[2] = 10000.0f;
        m_box.m_box[5] = -10000.0f;
        char const* vert = static_cast<char const*>(m_verts);
        for (unsigned int i = 0; i < m_header.m_numVertices; ++i, vert += m_VertTypeSize)
        {
            float const* const pos = reinterpret_cast<float const*>(vert);
            for (int k = 0; k < 3; ++k)
            {
                if (m_box.m_box[k] > pos[k])
                {
                    m_box.m_box[k] = pos[k];
                }
                if (pos[k] > m_box.m_box[k + 3])
                {
                    m_box.m_box[k + 3] = pos[k];
                }
            }
        }
    }

    int CGSModel::Prepare2Draw(const CStr& dirToTextures, int vertOffset, int index)
    {
        // RVA 0x8F9460 - draws straight from the loaded arrays, rebasing the indices onto the shared buffer.
        m_drawVerts = m_verts;
        m_numDrawVerts = m_header.m_numVertices;
        m_numDrawIndices = 3 * m_header.m_numFaces;
        m_drawIndices = m_triIndices;
        // NOTE: left over from a vertex optimisation step that is gone: both ratios are always 1.
        float const vertsRatio = static_cast<float>(static_cast<double>(static_cast<int>(m_header.m_numVertices))
                                                    / static_cast<double>(m_header.m_numVertices));
        float const idxRatio = static_cast<float>(static_cast<double>(m_numDrawIndices)
                                                  / static_cast<double>(static_cast<unsigned int>(m_numDrawIndices)));
        if (vertsRatio < 0.75f)
        {
            M3D_LOG_INFO(CStr("BAD MODELLING? : model ") + dirToTextures + CStr(" vr = ") + CStr(vertsRatio)
                         + CStr(" ir = ") + CStr(idxRatio));
        }
        for (int i = 0; i < m_numDrawIndices; ++i)
        {
            m_drawIndices[i] += vertOffset;
        }
        m_vertsOffset = vertOffset;
        m_idxOffset = index;
        return 1;
    }

    int CGSModel::Prepare2Draw2(void* destVerts, void* destIndices)
    {
        // RVA 0x8F8D90
        if (!destVerts)
        {
            return -1;
        }
        memcpy(destVerts, m_drawVerts, m_VertTypeSize * m_numDrawVerts);
        memcpy(destIndices, m_drawIndices, sizeof(unsigned short) * m_numDrawIndices);
        return 1;
    }

    int CGSModel::Render(const CMatrix& mat, float opacity)
    {
        // RVA 0x8F99F0 - opacity is not used.
        M3D_RENDERER->SetTexture(0, m_SurfaceMaterial.Textures[0].Handle, -1.0);
        M3D_RENDERER->MatPush(mat);
        M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLELIST, m_vertsOffset, m_numDrawVerts, m_idxOffset,
                                           m_numDrawIndices / 3);
        M3D_RENDERER->MatPop(true);
        return 1;
    }
}
