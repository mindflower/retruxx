// Ported from the original dxrender9/mesh.cpp: ID3DXMesh-backed meshes and the geometry
// optimizers built on them.
#include <cstring>

#include <d3dx9.h>

#include "device.h"
#include "log.h"

// orig 0x64d420 mesh.cpp:29
bool compareVector(CVector const& a, CVector const& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

// orig 0x64d5b0 mesh.cpp:37
MeshHandle CDevice::AddMesh(VertexType type, void* verts, int numVerts, unsigned short* tris, int numTris,
                            SubmeshInfo* subs, int numSubs, int* remap)
{
    unsigned int fvf;
    int vertSize;
    IDirect3DVertexDeclaration9* vdecl;
    GetVertexInfo(type, fvf, vertSize, vdecl);

    for (int i1 = 0; i1 < numTris; i1++)
    {
        if (tris[i1 * 3] > numVerts ||
            tris[i1 * 3 + 1] > numVerts ||
            tris[i1 * 3 + 2] > numVerts)
        {
            LogMsg(CStr("ERROR: Incoming mesh have index outta vertices in tri # ") + CStr(i1) + ": (" +
                   CStr((int)tris[i1 * 3]) + "," + CStr((int)tris[i1 * 3 + 1]) + "," + CStr((int)tris[i1 * 3 + 2]) +
                   "), while maximum vertex index is " + CStr(numVerts));
            return MeshHandle();
        }
    }

    unsigned int i;
    for (i = 0; i < m_meshes.size(); i++)
    {
        if (m_meshes[i].m_mesh == 0)
        {
            break;
        }
    }
    if (i == m_meshes.size())
    {
        m_meshes.push_back(CMesh());
    }

    CMesh& m = m_meshes[i];
    InternalHandle<MeshHandle> newHandle;
    newHandle.SetId(i);

    m.m_numTris = numTris;
    m.m_indices = new unsigned short[numTris * 3];
    memcpy(m.m_indices, tris, numTris * 3 * sizeof(unsigned short));

    m.m_numVerts = numVerts;
    m.m_verts = new unsigned char[vertSize * numVerts];
    memcpy(m.m_verts, verts, vertSize * numVerts);
    m.m_vertexType = type;

    HRESULT hr = D3DXCreateMeshFVF(m.m_numTris, m.m_numVerts, D3DXMESH_MANAGED, fvf, m_pd3dDevice, &m.m_mesh);
    if (FAILED(hr))
    {
        LogMsg(CStr("ERROR: cannot create mesh, error: ") + getD3dErrorStr(hr));
        ReleaseMesh(newHandle);
        return MeshHandle();
    }

    unsigned char* meshIb;
    hr = m.m_mesh->LockIndexBuffer(D3DLOCK_NOSYSLOCK, (void**)&meshIb);
    if (FAILED(hr))
    {
        LogMsg(CStr("ERROR: cannot lock ib for mesh, error: ") + getD3dErrorStr(hr));
        ReleaseMesh(newHandle);
        return MeshHandle();
    }
    memcpy(meshIb, m.m_indices, m.m_numTris * 3 * sizeof(unsigned short));
    m.m_mesh->UnlockIndexBuffer();

    unsigned char* meshVb;
    hr = m.m_mesh->LockVertexBuffer(D3DLOCK_NOSYSLOCK, (void**)&meshVb);
    if (FAILED(hr))
    {
        LogMsg(CStr("ERROR: cannot lock vb for mesh, error: ") + getD3dErrorStr(hr));
        ReleaseMesh(newHandle);
        return MeshHandle();
    }
    memcpy(meshVb, m.m_verts, m.m_numVerts * vertSize);
    m.m_mesh->UnlockVertexBuffer();

    DWORD* adj = new DWORD[m.m_numTris * 3];
    hr = m.m_mesh->GenerateAdjacency(0.0f, adj);
    if (FAILED(hr))
    {
        LogMsg(CStr("ERROR: cannot generate adj, error: ") + getD3dErrorStr(hr));
        if (adj)
        {
            delete[] adj;
        }
        ReleaseMesh(newHandle);
        return MeshHandle();
    }

    ID3DXBuffer* buf = 0;
    hr = m.m_mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, adj, adj, 0, &buf);
    if (FAILED(hr))
    {
        LogMsg(CStr("ERROR: could not optimize mesh, error: ") + getD3dErrorStr(hr));
        if (adj)
        {
            delete[] adj;
        }
        ReleaseMesh(newHandle);
        return MeshHandle();
    }

    DWORD remapSize = buf->GetBufferSize();
    void* remapData = buf->GetBufferPointer();
    memcpy(remap, remapData, remapSize);
    if (buf)
    {
        buf->Release();
        buf = 0;
    }

    if (adj)
    {
        delete[] adj;
    }

    return newHandle;
}

// orig 0x64d480 mesh.cpp:231
int CDevice::ReleaseMesh(MeshHandle& id)
{
    if (!IsMeshValid(id))
    {
        return 0;
    }

    CMesh& m = m_meshes[MeshId(id)];
    if (--m.m_refs <= 0)
    {
        if (m.m_mesh)
        {
            m.m_mesh->Release();
            m.m_mesh = 0;
        }
        if (m.m_pmesh)
        {
            m.m_pmesh->Release();
            m.m_pmesh = 0;
        }
        if (m.m_verts)
        {
            delete[] static_cast<unsigned char*>(m.m_verts);
        }
        m.m_verts = 0;
        if (m.m_indices)
        {
            delete[] m.m_indices;
        }
        m.m_indices = 0;
        id.SetInvalid();
    }
    return m.m_refs;
}

// orig 0x64d540 mesh.cpp:258
void CDevice::RenderMesh(MeshHandle const& id, float lod)
{
    if (!IsMeshValid(id))
    {
        return;
    }

    ActuateStates(true);
    CMesh& m = m_meshes[MeshId(id)];
    m.m_mesh->DrawSubset(0);
    m_stats.polyCount += m.m_numTris;
}

// orig 0x64d460 mesh.cpp:289
void CDevice::StartRenderMeshes()
{
}

// orig 0x64d470 mesh.cpp:295
void CDevice::FinishRenderMeshes()
{
}

// orig 0x64e530 mesh.cpp:309
int CDevice::OptimizeGeometryToSingleStrip(VertexType vt, void* vertsIn, int numVertsIn, unsigned short* indicesIn,
                                           int numTrisIn, int zeroBaseIndex, void** vertsOut, int* numVertsOut,
                                           int** remap, unsigned short** singleStripOut,
                                           int* numSingleStripOutIndices)
{
    *numSingleStripOutIndices = 0;
    *singleStripOut = 0;
    *numVertsOut = 0;
    *vertsOut = 0;

    *remap = new int[numVertsIn];

    MeshHandle mh = AddMesh(vt, vertsIn, numVertsIn, indicesIn, numTrisIn, 0, 0, *remap);

    if (!mh.IsValid())
    {
        if (*remap)
        {
            delete[] *remap;
        }
        *remap = 0;
        ReleaseMesh(mh);
        return 0;
    }

    CMesh& m = m_meshes[MeshId(mh)];

    IDirect3DIndexBuffer9* ib;
    DWORD numIdxs;
    HRESULT hr = D3DXConvertMeshSubsetToSingleStrip(m.m_mesh, 0, D3DXMESH_MANAGED, &ib, &numIdxs);

    if (FAILED(hr))
    {
        LogMsg("ERROR: cannot convert mesh to single strip");
        ReleaseMesh(mh);
        return 0;
    }

    unsigned char* data;
    if (SUCCEEDED(ib->Lock(0, 0, (void**)&data, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY)))
    {
        *numSingleStripOutIndices = numIdxs;
        *singleStripOut = new unsigned short[numIdxs];
        memcpy(*singleStripOut, data, numIdxs * sizeof(unsigned short));
        ib->Unlock();
    }

    ib->Release();

    IDirect3DVertexBuffer9* vb;
    D3DVERTEXBUFFER_DESC dd;
    m.m_mesh->GetVertexBuffer(&vb);
    vb->GetDesc(&dd);
    if (vb)
    {
        vb->Release();
        vb = 0;
    }

    unsigned char* meshVb;
    hr = m.m_mesh->LockVertexBuffer(D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY, (void**)&meshVb);
    if (FAILED(hr))
    {
        LogMsg(CStr("ERROR: cannot lock vb for mesh, error: ") + getD3dErrorStr(hr));
        ReleaseMesh(mh);
        return 0;
    }

    unsigned int fvf;
    int vertSize;
    IDirect3DVertexDeclaration9* vdecl;
    GetVertexInfo(vt, fvf, vertSize, vdecl);

    *numVertsOut = dd.Size / vertSize;

    *vertsOut = new unsigned char[dd.Size];
    memcpy(*vertsOut, meshVb, dd.Size);
    m.m_mesh->UnlockVertexBuffer();

    ReleaseMesh(mh);

    return 1;
}

// orig 0x64e880 mesh.cpp:438
int CDevice::OptimizeGeometryToTriList(VertexType vt, void* vertsIn, int numVertsIn, unsigned short* indicesIn,
                                       int numTrisIn, int zeroBaseIndex, void** vertsOut, int* numVertsOut,
                                       int** remap, unsigned short** trisIndices, int* numTrisIndices)
{
    *numTrisIndices = 0;
    *trisIndices = 0;
    *numVertsOut = 0;
    *vertsOut = 0;

    *remap = new int[numVertsIn];

    MeshHandle mh = AddMesh(vt, vertsIn, numVertsIn, indicesIn, numTrisIn, 0, 0, *remap);

    if (!mh.IsValid())
    {
        if (*remap)
        {
            delete[] *remap;
        }
        *remap = 0;
        ReleaseMesh(mh);
        return 0;
    }

    CMesh& m = m_meshes[MeshId(mh)];

    unsigned char* data;
    if (SUCCEEDED(m.m_mesh->LockIndexBuffer(D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY, (void**)&data)))
    {
        DWORD numFaces = m.m_mesh->GetNumFaces();
        *numTrisIndices = numFaces * 3;
        *trisIndices = new unsigned short[numFaces * 3];
        memcpy(*trisIndices, data, numFaces * 3 * sizeof(unsigned short));
        m.m_mesh->UnlockIndexBuffer();
    }

    IDirect3DVertexBuffer9* vb;
    D3DVERTEXBUFFER_DESC dd;
    m.m_mesh->GetVertexBuffer(&vb);
    vb->GetDesc(&dd);
    if (vb)
    {
        vb->Release();
        vb = 0;
    }

    unsigned char* meshVb;
    HRESULT hr = m.m_mesh->LockVertexBuffer(D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY, (void**)&meshVb);
    if (FAILED(hr))
    {
        LogMsg(CStr("ERROR: cannot lock vb for mesh, error: ") + getD3dErrorStr(hr));
        ReleaseMesh(mh);
        return 0;
    }

    unsigned int fvf;
    int vertSize;
    IDirect3DVertexDeclaration9* vdecl;
    GetVertexInfo(vt, fvf, vertSize, vdecl);

    *numVertsOut = dd.Size / vertSize;

    *vertsOut = new unsigned char[dd.Size];
    memcpy(*vertsOut, meshVb, dd.Size);
    m.m_mesh->UnlockVertexBuffer();

    ReleaseMesh(mh);

    return 1;
}
