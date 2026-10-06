// The vertex buffer manager: native vertex buffers, the streaming buffers, the vertex buffer
// pools and the vertex declarations (the original dxrender9/vb_man.cpp).
#include <algorithm>
#include <cstring>
#include <list>
#include <map>
#include <utility>
#include <vector>

#include <core/kernel.h>

#include "device.h"
#include "log.h"

// The original writes SAFE_RELEASE(x); the renderer has no shared header for it.
#define SAFE_RELEASE(x) \
    if (x)              \
    {                   \
        (x)->Release(); \
        (x) = 0;        \
    }

// The original asserts with M3D_ASSERT(cond), which reaches Kernel::SysError(assertion, file, line). The
// kernel the executable hands over is Hard Truck Apocalypse's, whose SysError(whence, descr) is
// what kernel.h's SYS_ERROR builds; kernel.h's own M3D_ASSERT goes through m3d::g_Kernel, which
// the DLL does not link, so the same expansion is spelled out here through g_kernel.
#define VB_MAN_ASSERT(cond) \
    if (!(cond))            \
    g_kernel->SysError((__FILE__ ":") + CStr(__LINE__), (#cond))

namespace
{
    // The original's InvalidHandle<VbHandle>() (i_renderer_handles.h:91): the handle with index -1. The
    // engine's InvalidHandle<> here is an empty class without the conversion.
    VbHandle InvalidVbHandle()
    {
        VbHandle h;
        h.SetInvalid();
        return h;
    }
}

// orig 0x634c40 vb_man.cpp:34
int CDevice::ReferenceVb(VbHandle const* id)
{
    if (IsVbValid(*id))
    {
        return ++m_vbs[VbId(*id)].m_refs;
    }
    return 0;
}

// orig 0x634c90 vb_man.cpp:48
int CDevice::ReleaseVb(VbHandle& id)
{
    if (!IsVbValid(id))
    {
        return 0;
    }

    CVertexBuffer& vb = m_vbs[VbId(id)];
    if (--vb.m_refs <= 0)
    {
        SAFE_RELEASE(vb.m_vb);
        vb.m_vb = 0;

        id.SetInvalid();
    }

    vb.m_lockedAtPresent = -1;

    m_isDevMemStatsValid = false;

    return vb.m_refs;
}

// orig 0x6344a0 vb_man.cpp:78
void CDevice::SetToStream0(VbHandle const& id)
{
    SetToStream(0, id);
}

// orig 0x634d10 vb_man.cpp:84
void CDevice::SetToStream(int stream, VbHandle const& id)
{
    if (IsVbValid(id))
    {
        CVertexBuffer& vb = m_vbs[VbId(id)];
        if (vb.m_vertexDecl)
        {
            setVertexDeclaration(vb.m_vertexDecl);
        }
        setStreamSource(stream, vb.m_vb, vb.m_vertSz);
    }
}

// orig 0x6353e0 vb_man.cpp:105
void CDevice::SetToStreams01(VbHandle const& id0, VbHandle const& id1)
{
    if (IsVbValid(id0) && IsVbValid(id1))
    {
        CVertexBuffer& vb0 = m_vbs[VbId(id0)];
        CVertexBuffer& vb1 = m_vbs[VbId(id1)];

        std::pair<IDirect3DVertexDeclaration9*, IDirect3DVertexDeclaration9*> key(vb0.m_vertexDecl, vb1.m_vertexDecl);
        std::map<std::pair<IDirect3DVertexDeclaration9*, IDirect3DVertexDeclaration9*>, IDirect3DVertexDeclaration9*>::iterator it =
            m_combinedVD.find(key);

        if (it != m_combinedVD.end() && it->second)
        {
            setVertexDeclaration(it->second);
        }

        setStreamSource(0, vb0.m_vb, vb0.m_vertSz);
        setStreamSource(1, vb1.m_vb, vb1.m_vertSz);
    }
}

// orig 0x634d70 vb_man.cpp:136
void* CDevice::LockVbStreaming(VbHandle const& id, int sz, int& ofs, int* discarded)
{
    if (!IsVbValid(id))
    {
        return 0;
    }

    CVertexBuffer& vb = m_vbs[VbId(id)];

    int maxVerts = vb.m_desc.Size / vb.m_vertSz;

    ofs = vb.m_curPos;
    vb.m_curPos += sz;
    if (vb.m_curPos >= maxVerts)
    {
        vb.m_curPos = sz;
        ofs = 0;
    }

    void* ptr = LockVb(id, sz, ofs, ofs ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

    if (discarded)
    {
        *discarded = ofs == 0;
    }

    return ptr;
}

// orig 0x634e10 vb_man.cpp:211
void* CDevice::LockVb(VbHandle const& id, int sz, int ofs, unsigned int flags)
{
    if (!IsVbValid(id))
    {
        return 0;
    }

    CVertexBuffer& vb = m_vbs[VbId(id)];

    if (vb.m_desc.Pool == D3DPOOL_DEFAULT)
    {
        if (flags == 0)
        {
            flags = D3DLOCK_DISCARD;
        }
    }
    else
    {
        flags &= ~(D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE);
    }
    flags |= D3DLOCK_NOSYSLOCK;

    if (sz == 0)
    {
        sz = vb.m_desc.Size / vb.m_vertSz;
        ofs = 0;
    }

    void* buff = 0;
    HRESULT hr = vb.m_vb->Lock(ofs * vb.m_vertSz, sz * vb.m_vertSz, &buff, flags);
    if (FAILED(hr))
    {
        return 0;
    }

    vb.m_locked = 1;

    return buff;
}

// orig 0x634ec0 vb_man.cpp:288
void CDevice::UnlockVb(VbHandle const& id)
{
    if (IsVbValid(id))
    {
        m_vbs[VbId(id)].m_vb->Unlock();
        m_vbs[VbId(id)].m_locked = 0;
    }
}

// orig 0x6344c0 vb_man.cpp:320
void CDevice::GetVertexInfo(VertexType tvert, unsigned int& fvf, int& sizeofvert, IDirect3DVertexDeclaration9*& decl)
{
    IDirect3DVertexDeclaration9* vd = 0;

    switch (tvert)
    {
    case VERTEX_XYZCT1:
        vd = m_vdXYZCT1;
        fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
        sizeofvert = 24;
        break;

    case VERTEX_XYZT1:
        vd = m_vdXYZT1;
        fvf = D3DFVF_XYZ | D3DFVF_TEX1;
        sizeofvert = 20;
        break;

    case VERTEX_XYZNT1:
        vd = m_vdXYZNT1;
        fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
        sizeofvert = 32;
        break;

    case VERTEX_XYZNT2:
        vd = m_vdXYZNT2;
        fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
        sizeofvert = 40;
        break;

    case VERTEX_XYZNT3:
        vd = m_vdXYZNT3;
        fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3;
        sizeofvert = 48;
        break;

    case VERTEX_XYZNC:
        vd = m_vdXYZNC;
        fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
        sizeofvert = 28;
        break;

    case VERTEX_XYZC:
        vd = m_vdXYZC;
        fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
        sizeofvert = 16;
        break;

    case VERTEX_XYZWCT1:
        vd = m_vdXYZWCT1;
        fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
        sizeofvert = 28;
        break;

    case VERTEX_XYZWC:
        vd = m_vdXYZWC;
        fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
        sizeofvert = 20;
        break;

    case VERTEX_XYZNCT1:
        vd = m_vdXYZNCT1;
        fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
        sizeofvert = 36;
        break;

    case VERTEX_XYZNCT2:
        vd = m_vdXYZNCT2;
        fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2;
        sizeofvert = 44;
        break;

    case VERTEX_XYZCT2:
        vd = m_vdXYZCT2;
        fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2;
        sizeofvert = 32;
        break;

    case VERTEX_XYZNCT1_UV2_S1:
        vd = m_vdXYZNCT1_UV2_S1;
        fvf = 0;
        sizeofvert = 8;
        break;

    case VERTEX_STREAM_UV_S1:
        // No declaration: the tail below leaves decl 0.
        sizeofvert = 8;
        fvf = D3DFVF_TEX1;
        break;

    case VERTEX_XYZCT1_UVW:
        vd = m_vdXYZCT1_UVW;
        sizeofvert = 28;
        fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
        break;

    case VERTEX_XYZCT2_UVW:
        vd = m_vdXYZCT2_UVW;
        sizeofvert = 36;
        fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE3(0);
        break;

    case VERTEX_XYZNT1T:
        vd = m_vdXYZNT1T;
        fvf = 0;
        sizeofvert = 48;
        break;

    case VERTEX_XYZNCT1T:
        vd = m_vdXYZNCT1T;
        fvf = 0;
        sizeofvert = 52;
        break;

    case VERTEX_XYZ:
        vd = m_vdXYZ;
        fvf = 0;
        sizeofvert = 12;
        break;

    case VERTEX_WATERTEST:
        vd = m_vdWaterTest;
        fvf = 0;
        sizeofvert = 8;
        break;

    case VERTEX_GRASSTEST:
        vd = m_vdGrassTest;
        fvf = 0;
        sizeofvert = 24;
        break;

    case VERTEX_IMPOSTORTEST:
        vd = m_vdImpostorTest;
        fvf = 0;
        sizeofvert = 20;
        break;

    case VERTEX_YNI:
        vd = m_vdYNI;
        fvf = 0;
        sizeofvert = 12;
        break;

    case VERTEX_XYZT1I:
        vd = m_vdXYZT1I;
        fvf = 0;
        sizeofvert = 24;
        break;

    // The 25th vertex type of the original (the per-instance id stream); HTA's VertexType ends at
    // VERTEX_XYZT1I (0x17), so it has no name here.
    case (VertexType)0x18:
        vd = m_vdInstanceId;
        fvf = 0;
        sizeofvert = 4;
        break;

    default:
        decl = 0;
        return;
    }

    if (!vd)
    {
        decl = 0;
        return;
    }
    decl = vd;
}

// orig 0x634be0 vb_man.cpp:494
VbHandle CDevice::AddVbStreaming(VertexType tv, int sz, unsigned int flags)
{
    return AddVb(tv, sz, "Streaming", flags | 0x200);
}

// orig 0x636110 vb_man.cpp:501
VbHandle CDevice::AddVb(VertexType tvert, int vertcount, CStr const& vbName, unsigned int flags)
{
    unsigned int i;
    for (i = 0; i < m_vbs.size(); i++)
    {
        if (!m_vbs[i].m_vb)
        {
            break;
        }
    }
    if (i == m_vbs.size())
    {
        m_vbs.push_back(CVertexBuffer());
    }

    D3DVERTEXBUFFER_DESC& desc = m_vbs[i].m_desc;

    unsigned int fvf;
    int sizeofvert;
    IDirect3DVertexDeclaration9* vdecl;
    GetVertexInfo(tvert, fvf, sizeofvert, vdecl);

    memset(&desc, 0, sizeof(desc));
    desc.FVF = fvf;
    desc.Format = D3DFMT_VERTEXDATA;
    desc.Type = D3DRTYPE_VERTEXBUFFER;
    desc.Size = sizeofvert * vertcount;
    m_vbs[i].m_vertSz = sizeofvert;

    // Bit 0x200 of the flags: a dynamic (streaming) buffer in the default pool.
    if (flags & 0x200)
    {
        desc.Pool = D3DPOOL_DEFAULT;
        desc.Usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
        m_pd3dDevice->EvictManagedResources();
    }
    else
    {
        desc.Pool = D3DPOOL_MANAGED;
        desc.Usage = D3DUSAGE_WRITEONLY;
    }

    HRESULT hr = m_pd3dDevice->CreateVertexBuffer(desc.Size, desc.Usage, desc.FVF, desc.Pool, &m_vbs[i].m_vb, 0);

    if (FAILED(hr))
    {
        m_vbs[i].m_vb = 0;
        return InvalidVbHandle();
    }

    m_vbs[i].m_curPos = 0;
    m_vbs[i].m_refs = 1;
    m_vbs[i].m_vertexDecl = vdecl;
    m_vbs[i].m_vbName = vbName;

    m_isDevMemStatsValid = false;

    InternalHandle<VbHandle> res;
    res.SetId(i);
    return res;
}

// orig 0x634f10 vb_man.cpp:576
void CDevice::rstVbPrepareFor()
{
    for (unsigned int i = 0; i < m_vbs.size(); i++)
    {
        if (m_vbs[i].m_vb && m_vbs[i].m_desc.Pool == D3DPOOL_DEFAULT)
        {
            m_vbs[i].m_vb->Release();
            m_vbs[i].m_curPos = 0;
            m_vbs[i].m_locked = 0;
        }
    }
}

// orig 0x634f70 vb_man.cpp:601
void CDevice::rstVbRestoreAfter()
{
    int kf = 0;

    for (unsigned int i = 0; i < m_vbs.size(); i++)
    {
        if (!m_vbs[i].m_vb)
        {
            continue;
        }

        if (m_vbs[i].m_desc.Pool != D3DPOOL_DEFAULT)
        {
            continue;
        }

        HRESULT hr = m_pd3dDevice->CreateVertexBuffer(m_vbs[i].m_desc.Size, m_vbs[i].m_desc.Usage, m_vbs[i].m_desc.FVF,
                                                      m_vbs[i].m_desc.Pool, &m_vbs[i].m_vb, 0);

        if (FAILED(hr))
        {
            m_vbs[i].m_vb = 0;
            kf++;
        }
    }

    if (kf != 0)
    {
        LogMsg(CStr("VBMan failed to restore ") + CStr(kf) + " vbs");
    }
}

// orig 0x6362e0 vb_man.cpp:643
VbPoolField CDevice::AddVbPoolField(VertexType Type, unsigned int Size)
{
    if (Size > m_VbPoolSize)
    {
        LogMsg(" [ AddVbPoolField ] : Asked size is too big!!! ");
        // Vb is -1 from Handle's constructor; the other fields stay uninitialised, as in 1.5.
        VbPoolField BadField;
        return BadField;
    }

    std::vector<VertexType>::iterator TypeIter = std::find(m_VbPoolTypes.begin(), m_VbPoolTypes.end(), Type);

    unsigned int PoolIndex;
    if (TypeIter == m_VbPoolTypes.end())
    {
        m_VbPoolTypes.push_back(Type);

        m_VbPoolBuffers.push_back(std::vector<VbHandle>());

        m_VbPoolBuffers.back().push_back(InvalidVbHandle());
        m_VbPoolBuffers.back().back() = AddVb(Type, m_VbPoolSize, "Pool", 0);

        m_VbPoolFields.push_back(std::list<PoolFieldInfo>());

        m_VbPoolFields.back().push_back(PoolFieldInfo());
        m_VbPoolFields.back().back().Offset = 0;
        m_VbPoolFields.back().back().Size = 0;

        m_VbPoolFields.back().push_back(PoolFieldInfo());
        m_VbPoolFields.back().back().Offset = m_VbPoolSize;
        m_VbPoolFields.back().back().Size = 0;

        PoolIndex = m_VbPoolTypes.size() - 1;
    }
    else
    {
        PoolIndex = TypeIter - m_VbPoolTypes.begin();
    }

    std::list<PoolFieldInfo>& Fields = m_VbPoolFields[PoolIndex];
    std::list<PoolFieldInfo>::iterator End = Fields.end();
    End--;

    for (std::list<PoolFieldInfo>::iterator It = Fields.begin(); It != End; It++)
    {
        std::list<PoolFieldInfo>::iterator Next = It;
        Next++;

        if (Next->Offset - It->Offset - It->Size >= Size)
        {
            PoolFieldInfo NewFieldInfo;
            NewFieldInfo.Offset = It->Offset + It->Size;

            unsigned int VbIndex = NewFieldInfo.Offset / m_VbPoolSize;

            VbPoolField NewField;
            NewField.Offset = NewFieldInfo.Offset;
            NewField.RealOffset = NewFieldInfo.Offset % m_VbPoolSize;
            NewField.Vb = m_VbPoolBuffers[PoolIndex][VbIndex];
            NewField.Size = Size;
            NewFieldInfo.Size = Size;

            Fields.insert(Next, NewFieldInfo);
            NewField.VertType = Type;
            return NewField;
        }
    }

    m_VbPoolBuffers[PoolIndex].push_back(InvalidVbHandle());
    m_VbPoolBuffers[PoolIndex].back() = AddVb(Type, m_VbPoolSize, "Pool", 0);

    PoolFieldInfo NewRightBorder;
    NewRightBorder.Offset = End->Offset + m_VbPoolSize;
    NewRightBorder.Size = 0;
    Fields.push_back(NewRightBorder);

    return AddVbPoolField(Type, Size);
}

// orig 0x6354c0 vb_man.cpp:724
void CDevice::ReleaseVbPoolField(VbPoolField& PoolField)
{
    std::vector<VertexType>::iterator TypeIter = std::find(m_VbPoolTypes.begin(), m_VbPoolTypes.end(), PoolField.VertType);
    if (TypeIter == m_VbPoolTypes.end())
    {
        LogMsg("[ReleaseVbPoolField]: Cant find specified vertex buffer");
        return;
    }

    PoolFieldInfo FieldInfo;
    FieldInfo.Size = PoolField.Size;
    FieldInfo.Offset = PoolField.Offset;
    unsigned int PoolIndex = TypeIter - m_VbPoolTypes.begin();

    std::list<PoolFieldInfo>::iterator FieldIter =
        std::find(m_VbPoolFields[PoolIndex].begin(), m_VbPoolFields[PoolIndex].end(), FieldInfo);

    if (FieldIter != m_VbPoolFields[PoolIndex].end())
    {
        m_VbPoolFields[PoolIndex].erase(FieldIter);
        PoolField.Vb = InvalidVbHandle();

        // The last pool buffer goes when nothing but its right border is left in it.
        std::list<PoolFieldInfo>& Fields = m_VbPoolFields[PoolIndex];
        std::list<PoolFieldInfo>::iterator Last = Fields.end();
        Last--;
        std::list<PoolFieldInfo>::iterator Prev = Last;
        Prev--;

        if (Last->Offset - Prev->Offset - Prev->Size >= m_VbPoolSize && Fields.size() > 2)
        {
            ReleaseVb(m_VbPoolBuffers[PoolIndex].back());

            m_VbPoolBuffers[PoolIndex].pop_back();
            Fields.erase(Last);
        }
    }
    else
    {
        LogMsg("[ReleaseVbPoolField]: Cant find specified field in vertex buffer");
    }
}

// orig 0x634820 vb_man.cpp:771
void* CDevice::LockVbPoolField(VbPoolField const& PoolField)
{
    return LockVb(PoolField.Vb, PoolField.Size, PoolField.RealOffset, 0);
}

// orig 0x634840 vb_man.cpp:778
void CDevice::UnlockVbPoolField(VbPoolField const& PoolField)
{
    UnlockVb(PoolField.Vb);
}

// orig 0x634860 vb_man.cpp:785
void CDevice::SetToStream(int Stream, VbPoolField const& PoolField)
{
    SetToStream(Stream, PoolField.Vb);
}

// orig 0x634880 vb_man.cpp:794
void CDevice::SetToStream0(VbPoolField const& PoolField)
{
    SetToStream(0, PoolField.Vb);
}

// orig 0x6356b0 vb_man.cpp:802
void CDevice::CreateCombinedVertexDeclaration(IDirect3DVertexDeclaration9* a, IDirect3DVertexDeclaration9* b)
{
    D3DVERTEXELEMENT9 aElems[64];
    UINT aNumElems;
    HRESULT hr = a->GetDeclaration(aElems, &aNumElems);
    VB_MAN_ASSERT(hr == D3D_OK);

    D3DVERTEXELEMENT9 bElems[64];
    UINT bNumElems;
    hr = b->GetDeclaration(bElems, &bNumElems);
    VB_MAN_ASSERT(hr == D3D_OK);

    std::vector<D3DVERTEXELEMENT9> combined;
    for (unsigned int j = 0; j < aNumElems - 1; j++)
    {
        combined.push_back(aElems[j]);
    }
    for (unsigned int j = 0; j < bNumElems - 1; j++)
    {
        combined.push_back(bElems[j]);
    }
    D3DVERTEXELEMENT9 endTag = D3DDECL_END();
    combined.push_back(endTag);

    IDirect3DVertexDeclaration9* combinedVD;
    m_pd3dDevice->CreateVertexDeclaration(&combined[0], &combinedVD);

    m_combinedVD[std::make_pair(a, b)] = combinedVD;
}

// orig 0x635970 vb_man.cpp:839
void CDevice::InitVertexDeclarations()
{
    // The element tables, as laid out in the original image (.rdata 0x7e4ccc-0x7e4fec).
    static const D3DVERTEXELEMENT9 ddXYZCT1[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZCT1, &m_vdXYZCT1);

    static const D3DVERTEXELEMENT9 ddXYZT1[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZT1, &m_vdXYZT1);

    static const D3DVERTEXELEMENT9 ddXYZNT1[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNT1, &m_vdXYZNT1);

    static const D3DVERTEXELEMENT9 ddXYZNT2[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNT2, &m_vdXYZNT2);

    static const D3DVERTEXELEMENT9 ddXYZNT3[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        {0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNT3, &m_vdXYZNT3);

    static const D3DVERTEXELEMENT9 ddXYZN[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZN, &m_vdXYZN);

    static const D3DVERTEXELEMENT9 ddXYZ[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZ, &m_vdXYZ);

    static const D3DVERTEXELEMENT9 ddXYZW[] = {
        {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZW, &m_vdXYZW);

    static const D3DVERTEXELEMENT9 ddXYZC[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZC, &m_vdXYZC);

    static const D3DVERTEXELEMENT9 ddXYZNC[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNC, &m_vdXYZNC);

    static const D3DVERTEXELEMENT9 ddXYZWCT1[] = {
        {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
        {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZWCT1, &m_vdXYZWCT1);

    static const D3DVERTEXELEMENT9 ddXYZWC[] = {
        {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
        {0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZWC, &m_vdXYZWC);

    static const D3DVERTEXELEMENT9 ddXYZNCT1[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNCT1, &m_vdXYZNCT1);

    static const D3DVERTEXELEMENT9 ddXYZNCT2[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNCT2, &m_vdXYZNCT2);

    static const D3DVERTEXELEMENT9 ddXYZCT2[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZCT2, &m_vdXYZCT2);

    static const D3DVERTEXELEMENT9 ddXYZNCT1T[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNCT1T, &m_vdXYZNCT1T);

    static const D3DVERTEXELEMENT9 ddXYZNT1T[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNT1T, &m_vdXYZNT1T);

    static const D3DVERTEXELEMENT9 ddXYZCT1_UVW[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZCT1_UVW, &m_vdXYZCT1_UVW);

    static const D3DVERTEXELEMENT9 ddXYZCT2_UVW[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
        {0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZCT2_UVW, &m_vdXYZCT2_UVW);

    static const D3DVERTEXELEMENT9 ddXYZNCT1_UV2_S1[] = {
        {0, 0, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 4, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {1, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZNCT1_UV2_S1, &m_vdXYZNCT1_UV2_S1);

    static const D3DVERTEXELEMENT9 ddWaterTest[] = {
        {0, 0, D3DDECLTYPE_SHORT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddWaterTest, &m_vdWaterTest);

    static const D3DVERTEXELEMENT9 ddGrassTest[] = {
        {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddGrassTest, &m_vdGrassTest);

    static const D3DVERTEXELEMENT9 ddImpostorTest[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddImpostorTest, &m_vdImpostorTest);

    static const D3DVERTEXELEMENT9 ddYNI[] = {
        {0, 0, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 4, D3DDECLTYPE_SHORT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddYNI, &m_vdYNI);

    static const D3DVERTEXELEMENT9 ddXYZT1I[] = {
        {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 20, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddXYZT1I, &m_vdXYZT1I);

    static const D3DVERTEXELEMENT9 ddInstanceId[] = {
        {1, 0, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
        D3DDECL_END()};
    m_pd3dDevice->CreateVertexDeclaration(ddInstanceId, &m_vdInstanceId);

    m_vdXYZW4NCT1 = 0;
    m_vdXYZW4TNCT1 = 0;

    CreateCombinedVertexDeclaration(m_vdXYZCT1, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZT1, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZNT1, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZNT2, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZNT3, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZN, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZ, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZW, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZC, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZNC, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZWCT1, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZWC, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZNCT1, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZNCT2, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZCT2, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZNCT1T, m_vdInstanceId);
    CreateCombinedVertexDeclaration(m_vdXYZNT1T, m_vdInstanceId);

    // The global streaming buffers: 128 KB of vertices each (the counts are 0x20000 / vertex size).
    m_vbXyz = AddVb(VERTEX_XYZ, 0x2aaa, "GlobalStreaming", 0x200);
    m_vbXyzc = AddVb(VERTEX_XYZC, 0x2000, "GlobalStreaming", 0x200);
    m_vbXyznc = AddVb(VERTEX_XYZNC, 0x1249, "GlobalStreaming", 0x200);
    m_vbXyzct1 = AddVb(VERTEX_XYZCT1, 0x1555, "GlobalStreaming", 0x200);
    m_vbXyznt1 = AddVb(VERTEX_XYZNT1, 0x1000, "GlobalStreaming", 0x200);
    m_vbXyznt2 = AddVb(VERTEX_XYZNT2, 0xccc, "GlobalStreaming", 0x200);
    m_vbXyznt3 = AddVb(VERTEX_XYZNT3, 0xaaa, "GlobalStreaming", 0x200);
    m_vbXyznct2 = AddVb(VERTEX_XYZNCT2, 0xba2, "GlobalStreaming", 0x200);
    m_vbXyznct1 = AddVb(VERTEX_XYZNCT1, 0xe38, "GlobalStreaming", 0x200);
    m_vbXyznt1t = AddVb(VERTEX_XYZNT1T, 0xaaa, "GlobalStreaming", 0x200);
    m_vbXyznct1t = AddVb(VERTEX_XYZNCT1T, 0x9d8, "GlobalStreaming", 0x200);
    m_vbXyzwct1 = AddVb(VERTEX_XYZWCT1, 0x1249, "GlobalStreaming", 0x200);
}

// orig 0x635190 vb_man.cpp:1189
VbHandle CDevice::GetVbStreaming(VertexType VertType)
{
    switch (VertType)
    {
    case VERTEX_XYZ:
        return m_vbXyz;
    case VERTEX_XYZC:
        return m_vbXyzc;
    case VERTEX_XYZNC:
        return m_vbXyznc;
    case VERTEX_XYZCT1:
        return m_vbXyzct1;
    case VERTEX_XYZNT1:
        return m_vbXyznt1;
    case VERTEX_XYZNT2:
        return m_vbXyznt2;
    case VERTEX_XYZNT3:
        return m_vbXyznt3;
    case VERTEX_XYZNCT2:
        return m_vbXyznct2;
    case VERTEX_XYZNCT1:
        return m_vbXyznct1;
    case VERTEX_XYZNT1T:
        return m_vbXyznt1t;
    case VERTEX_XYZNCT1T:
        return m_vbXyznct1t;
    case VERTEX_XYZWCT1:
        return m_vbXyzwct1;
    default:
        LogMsg(CStr("Unsupported vertex type in GetVbStreaming: ") + CStr((int)VertType));
        return InvalidVbHandle();
    }
}

// orig 0x6348a0 vb_man.cpp:1240
void CDevice::DoneVertexDeclarations()
{
    ReleaseVb(m_vbXyz);
    ReleaseVb(m_vbXyzc);
    ReleaseVb(m_vbXyznc);
    ReleaseVb(m_vbXyzct1);
    ReleaseVb(m_vbXyznt1);
    ReleaseVb(m_vbXyznt2);
    ReleaseVb(m_vbXyznt3);
    ReleaseVb(m_vbXyznct2);
    ReleaseVb(m_vbXyznct1);
    ReleaseVb(m_vbXyzwct1);
    ReleaseVb(m_vbXyznt1t);
    ReleaseVb(m_vbXyznct1t);

    SAFE_RELEASE(m_vdXYZCT1);
    SAFE_RELEASE(m_vdXYZT1);
    SAFE_RELEASE(m_vdXYZNT1);
    SAFE_RELEASE(m_vdXYZNT2);
    SAFE_RELEASE(m_vdXYZNT3);
    SAFE_RELEASE(m_vdXYZN);
    SAFE_RELEASE(m_vdXYZ);
    SAFE_RELEASE(m_vdXYZW);
    SAFE_RELEASE(m_vdXYZC);
    SAFE_RELEASE(m_vdXYZNC);
    SAFE_RELEASE(m_vdXYZWCT1);
    SAFE_RELEASE(m_vdXYZWC);
    SAFE_RELEASE(m_vdXYZNCT1);
    SAFE_RELEASE(m_vdXYZNCT2);
    SAFE_RELEASE(m_vdXYZCT2);
    SAFE_RELEASE(m_vdXYZNCT1T);
    SAFE_RELEASE(m_vdXYZNT1T);
    SAFE_RELEASE(m_vdXYZW4NCT1);
    SAFE_RELEASE(m_vdXYZW4TNCT1);
    SAFE_RELEASE(m_vdXYZCT1_UVW);
    SAFE_RELEASE(m_vdXYZCT2_UVW);
    SAFE_RELEASE(m_vdXYZNCT1_UV2_S1);
    SAFE_RELEASE(m_vdWaterTest);
    SAFE_RELEASE(m_vdGrassTest);
    SAFE_RELEASE(m_vdImpostorTest);
    SAFE_RELEASE(m_vdYNI);
    SAFE_RELEASE(m_vdXYZT1I);
    SAFE_RELEASE(m_vdInstanceId);
}
