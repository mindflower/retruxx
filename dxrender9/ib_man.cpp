// Index buffers: the m_ibs table, streaming locks and the pooled index buffer (the original
// dxrender9/ib_man.cpp).
#include <algorithm>
#include <list>

#include "device.h"
#include "log.h"

// orig 0x643490 ib_man.cpp:28
int CDevice::ReferenceIb(IbHandle const* id)
{
    if (!IsIbValid(*id))
    {
        return 0;
    }
    return ++m_ibs[IbId(*id)].m_refs;
}

// orig 0x6434e0 ib_man.cpp:38
int CDevice::ReleaseIb(IbHandle& id)
{
    if (!IsIbValid(id))
    {
        return 0;
    }
    CIndexBuffer& ib = m_ibs[IbId(id)];
    if (--ib.m_refs <= 0)
    {
        if (ib.m_ib)
        {
            ib.m_ib->Release();
            ib.m_ib = 0;
        }
        ib.m_ib = 0;
        id.SetInvalid();
    }
    m_isDevMemStatsValid = false;
    return ib.m_refs;
}

// orig 0x643560 ib_man.cpp:61
void CDevice::SetIndices(IbHandle const& id, int ofs)
{
    if (IsIbValid(id))
    {
        setIndices(m_ibs[IbId(id)].m_ib, ofs);
    }
}

// orig 0x6435b0 ib_man.cpp:71
void* CDevice::LockIbStreaming(IbHandle const* id, int sz, int* ofs, int* discarded)
{
    if (!IsIbValid(*id))
    {
        return 0;
    }
    CIndexBuffer& ib = m_ibs[IbId(*id)];

    *ofs = ib.m_curPos;
    ib.m_curPos += sz;
    if (ib.m_curPos >= (int)(ib.m_desc.Size / 2))
    {
        ib.m_curPos = sz;
        *ofs = 0;
    }

    void* data = LockIb(*id, sz, *ofs, *ofs ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
    if (discarded)
    {
        *discarded = (*ofs == 0);
    }
    return data;
}

// orig 0x643650 ib_man.cpp:105
void* CDevice::LockIb(IbHandle const& id, int sz, int ofs, unsigned int flags)
{
    if (!IsIbValid(id))
    {
        return 0;
    }
    CIndexBuffer& ib = m_ibs[IbId(id)];

    if (ib.m_desc.Pool == D3DPOOL_DEFAULT)
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
        sz = ib.m_desc.Size / 2;
        ofs = 0;
    }

    void* data;
    if (FAILED(ib.m_ib->Lock(ofs * 2, sz * 2, &data, flags)))
    {
        return 0;
    }
    ib.m_locked = 1;
    return data;
}

// orig 0x6436f0 ib_man.cpp:150
void CDevice::UnlockIb(IbHandle const& id)
{
    if (IsIbValid(id))
    {
        m_ibs[IbId(id)].m_ib->Unlock();
        m_ibs[IbId(id)].m_locked = 0;
    }
}

// orig 0x643410 ib_man.cpp:168
IbHandle CDevice::AddIbStreaming(int sz)
{
    return AddIb(sz, true);
}

// orig 0x643b20 ib_man.cpp:175
IbHandle CDevice::AddIb(int indexcount, bool dyn)
{
    unsigned int i;
    for (i = 0; i < m_ibs.size(); i++)
    {
        if (m_ibs[i].m_ib == 0)
        {
            break;
        }
    }
    if (i == m_ibs.size())
    {
        m_ibs.push_back(CIndexBuffer());
    }

    CIndexBuffer& ib = m_ibs[i];

    ib.m_desc.Format = D3DFMT_INDEX16;
    ib.m_desc.Size = indexcount * 2;
    ib.m_desc.Type = D3DRTYPE_INDEXBUFFER;
    if (dyn)
    {
        ib.m_desc.Pool = D3DPOOL_DEFAULT;
        ib.m_desc.Usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
        m_pd3dDevice->EvictManagedResources();
    }
    else
    {
        ib.m_desc.Pool = D3DPOOL_MANAGED;
        ib.m_desc.Usage = D3DUSAGE_WRITEONLY;
    }

    HRESULT hr = m_pd3dDevice->CreateIndexBuffer(ib.m_desc.Size, ib.m_desc.Usage, ib.m_desc.Format, ib.m_desc.Pool,
                                                 &m_ibs[i].m_ib, 0);
    if (FAILED(hr))
    {
        m_ibs[i].m_ib = 0;
        IbHandle invalid;
        invalid.SetInvalid();
        return invalid;
    }

    m_ibs[i].m_curPos = 0;
    m_ibs[i].m_refs = 1;
    m_isDevMemStatsValid = false;

    IbHandle handle;
    ((InternalHandle<IbHandle>&)handle).SetId((int)i);
    return handle;
}

// orig 0x643740 ib_man.cpp:234
void CDevice::rstIbPrepareFor()
{
    for (unsigned int i = 0; i < m_ibs.size(); i++)
    {
        if (m_ibs[i].m_ib)
        {
            if (m_ibs[i].m_desc.Pool == D3DPOOL_DEFAULT)
            {
                m_ibs[i].m_ib->Release();
                m_ibs[i].m_curPos = 0;
                m_ibs[i].m_locked = 0;
            }
        }
    }
}

// orig 0x6437b0 ib_man.cpp:259
void CDevice::rstIbRestoreAfter()
{
    int kf = 0;

    for (unsigned int i = 0; i < m_ibs.size(); i++)
    {
        if (m_ibs[i].m_ib)
        {
            if (m_ibs[i].m_desc.Pool == D3DPOOL_DEFAULT)
            {
                HRESULT hr = m_pd3dDevice->CreateIndexBuffer(m_ibs[i].m_desc.Size, m_ibs[i].m_desc.Usage,
                                                             m_ibs[i].m_desc.Format, m_ibs[i].m_desc.Pool,
                                                             &m_ibs[i].m_ib, 0);
                if (FAILED(hr))
                {
                    m_ibs[i].m_ib = 0;
                    kf++;
                }
            }
        }
    }

    if (kf)
    {
        LogMsg(CStr("IBMan failed to restore ") + CStr(kf) + CStr(" ibs"));
    }
}

// orig 0x643c70 ib_man.cpp:299
IbPoolField CDevice::AddIbPoolField(unsigned int Size)
{
    if (Size > m_IbPoolSize)
    {
        LogMsg(" [ AddIbPoolField ] : Asked size is too big!!! ");
        IbPoolField BadField;
        BadField.Ib.SetInvalid();
        return BadField;
    }

    if (m_IbPoolBuffers.empty())
    {
        m_IbPoolBuffers.push_back(IbHandle());
        m_IbPoolBuffers.back() = AddIb(m_IbPoolSize, false);

        m_IbPoolFields.push_back(PoolFieldInfo());
        m_IbPoolFields.back().Offset = 0;
        m_IbPoolFields.back().Size = 0;

        m_IbPoolFields.push_back(PoolFieldInfo());
        m_IbPoolFields.back().Offset = m_IbPoolSize;
        m_IbPoolFields.back().Size = 0;
    }

    std::list<PoolFieldInfo>::iterator LastIt = m_IbPoolFields.end();
    --LastIt;

    for (std::list<PoolFieldInfo>::iterator it = m_IbPoolFields.begin(); it != LastIt; ++it)
    {
        std::list<PoolFieldInfo>::iterator NextIt = it;
        ++NextIt;

        if (NextIt->Offset - it->Offset - it->Size >= Size)
        {
            unsigned int NewOffset = it->Offset + it->Size;

            unsigned int BufferIndex = NewOffset / m_IbPoolSize;

            PoolFieldInfo NewFieldInfo;
            NewFieldInfo.Size = Size;

            NewFieldInfo.Offset = NewOffset;
            m_IbPoolFields.insert(NextIt, NewFieldInfo);

            IbPoolField NewField;
            NewField.Offset = NewOffset;
            NewField.Size = Size;
            NewField.RealOffset = NewOffset % m_IbPoolSize;
            NewField.Ib = m_IbPoolBuffers[BufferIndex];
            return NewField;
        }
    }

    m_IbPoolBuffers.push_back(IbHandle());
    m_IbPoolBuffers.back() = AddIb(m_IbPoolSize, false);

    PoolFieldInfo NewRightBorder;
    NewRightBorder.Offset = LastIt->Offset + m_IbPoolSize;
    NewRightBorder.Size = 0;
    m_IbPoolFields.push_back(NewRightBorder);

    return AddIbPoolField(Size);
}

// orig 0x6439e0 ib_man.cpp:365
void CDevice::ReleaseIbPoolField(IbPoolField& PoolField)
{
    PoolFieldInfo FieldInfo;
    FieldInfo.Size = PoolField.Size;
    FieldInfo.Offset = PoolField.Offset;

    std::list<PoolFieldInfo>::iterator it = std::find(m_IbPoolFields.begin(), m_IbPoolFields.end(), FieldInfo);

    if (it != m_IbPoolFields.end())
    {
        m_IbPoolFields.erase(it);
        PoolField.Ib.SetInvalid();

        std::list<PoolFieldInfo>::iterator LastIt = m_IbPoolFields.end();
        --LastIt;

        std::list<PoolFieldInfo>::iterator PrevIt = LastIt;
        --PrevIt;

        if (LastIt->Offset - PrevIt->Offset - PrevIt->Size >= m_IbPoolSize && m_IbPoolFields.size() > 2)
        {
            ReleaseIb(m_IbPoolBuffers[m_IbPoolBuffers.size() - 1]);

            m_IbPoolBuffers.erase(m_IbPoolBuffers.end() - 1);
            m_IbPoolFields.erase(LastIt);
        }
    }
    else
    {
        LogMsg("[ReleaseIbPoolField]: Cant find specified field in index buffer");
    }
}

// orig 0x643430 ib_man.cpp:403
void* CDevice::LockIbPoolField(IbPoolField const& PoolField)
{
    return LockIb(PoolField.Ib, PoolField.Size, PoolField.RealOffset, 0);
}

// orig 0x643450 ib_man.cpp:410
void CDevice::UnlockIbPoolField(IbPoolField const& PoolField)
{
    UnlockIb(PoolField.Ib);
}

// orig 0x643470 ib_man.cpp:417
void CDevice::SetIndices(IbPoolField const& PoolField, int VertOffset)
{
    SetIndices(PoolField.Ib, VertOffset);
}
