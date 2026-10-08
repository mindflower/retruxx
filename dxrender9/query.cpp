// Ported from the original dxrender9/query.cpp: the IDirect3DQuery9 wrapper handed out by
// CDevice::NewQuery and the CDevice bookkeeping of the queries it created.
#include "query.h"

#include <algorithm>
#include <utility>

#include <core/kernel.h>

#include "device.h"
#include "log.h"

// Set by CDevice::CDevice (the PDB records no data symbol for it in query.obj).
CDevice* Query::m_dev = 0;

namespace
{
    // HTA's i_renderer_query.h keeps QueryReturnValue::Type and the result union private, while
    // The original's Query names the enumerators and writes the union members directly. The values are the
    // PDB's (Bool = 1, Dword = 2, Uint64 = 3, ...); the union starts at its VCACHE member, so that
    // member's address is the address of every member.
    typedef decltype(std::declval<QueryReturnValue const&>().GetType()) RetValType;

    RetValType const RV_NotValid = static_cast<RetValType>(0);
    RetValType const RV_Bool = static_cast<RetValType>(1);
    RetValType const RV_Dword = static_cast<RetValType>(2);
    RetValType const RV_Uint64 = static_cast<RetValType>(3);
    RetValType const RV_BandWidthTimings = static_cast<RetValType>(4);
    RetValType const RV_CacheUtilization = static_cast<RetValType>(5);
    RetValType const RV_InterfaceTimings = static_cast<RetValType>(6);
    RetValType const RV_PipelineTimings = static_cast<RetValType>(7);
    RetValType const RV_StageTimings = static_cast<RetValType>(8);
    RetValType const RV_VCache = static_cast<RetValType>(9);

    void* RetValData(QueryReturnValue& rv)
    {
        return const_cast<VCACHE*>(&rv.GetVCash());
    }
}  // namespace

// orig 0x6431f0 query.cpp:25
IQuery* CDevice::NewQuery(IQuery::Type type)
{
    Query* newQuery = new Query(type);

    m_queries.push_back(newQuery);

    newQuery->AddRef();

    bool supported = false;
    switch (type)
    {
    case IQuery::QUERY_VCACHE:
        supported = m_featureSupported[FEATURE_QUERY_VCACHE];
        break;
    case IQuery::QUERY_EVENT:
        supported = m_featureSupported[FEATURE_QUERY_EVENT];
        break;
    case IQuery::QUERY_OCCLUSION:
        supported = m_featureSupported[FEATURE_QUERY_OCCLUSION];
        break;
    case IQuery::QUERY_TIMESTAMP:
        supported = m_featureSupported[FEATURE_QUERY_TIMESTAMP];
        break;
    case IQuery::QUERY_TIMESTAMPDISJOINT:
        supported = m_featureSupported[FEATURE_QUERY_TIMESTAMPDISJOINT];
        break;
    case IQuery::QUERY_TIMESTAMPFREQ:
        supported = m_featureSupported[FEATURE_QUERY_TIMESTAMPFREQ];
        break;
    case IQuery::QUERY_PIPELINETIMINGS:
        supported = m_featureSupported[FEATURE_QUERY_PIPELINETIMINGS];
        break;
    case IQuery::QUERY_INTERFACETIMINGS:
        supported = m_featureSupported[FEATURE_QUERY_INTERFACETIMINGS];
        break;
    case IQuery::QUERY_VERTEXTIMINGS:
        supported = m_featureSupported[FEATURE_QUERY_VERTEXTIMINGS];
        break;
    case IQuery::QUERY_BANDWIDTHTIMINGS:
        supported = m_featureSupported[FEATURE_QUERY_BANDWIDTHTIMINGS];
        break;
    case IQuery::QUERY_CACHEUTILIZATION:
        supported = m_featureSupported[FEATURE_QUERY_CACHEUTILIZATION];
        break;
    default:
        break;
    }

    if (supported)
    {
        // original: assert-style macro, Kernel::SysError("newQuery->CreateQuery()", ".\\query.cpp", 66)
        // when CreateQuery fails.
        if (!newQuery->CreateQuery())
        {
            g_kernel->SysError(CStr(".\\query.cpp:") + CStr(66), "newQuery->CreateQuery()");
        }
    }
    else
    {
        newQuery->m_state = IQuery::QUERY_NOT_SUPPORT;
    }
    return newQuery;
}

// orig 0x642a10 query.cpp:74
int Query::CreateQuery()
{
    m_state = QUERY_SIGNALED;
    return m_dev->m_pd3dDevice->CreateQuery(static_cast<D3DQUERYTYPE>(m_type), &m_query) == D3D_OK;
}

// orig 0x642a40 query.cpp:82
void Query::ReleaseQuery()
{
    if (m_query)
    {
        m_query->Release();
    }
}

// orig 0x642dc0 query.cpp:90
Query::Query(Type _type) :
    m_type(_type)
{
    m_query = 0;
    m_state = QUERY_ERROR;
    m_retVal.SetType(RV_NotValid);
}

// orig 0x642a90 query.cpp:99
void Query::Begin()
{
    switch (m_type)
    {
    case QUERY_VCACHE:
    case QUERY_EVENT:
    case QUERY_TIMESTAMP:
    case QUERY_TIMESTAMPFREQ:
        break;
    default:
        m_query->Issue(D3DISSUE_BEGIN);
        break;
    }
}

// orig 0x642ad0 query.cpp:118
void Query::End()
{
    m_query->Issue(D3DISSUE_END);
}

// orig 0x642ae0 query.cpp:128
IQuery::State Query::GetState()
{
    if (m_state == QUERY_NOT_SUPPORT)
    {
        return QUERY_NOT_SUPPORT;
    }

    HRESULT hr = m_query->GetData(0, 0, 0);
    switch (hr)
    {
    case S_OK:
        m_state = QUERY_SIGNALED;
        break;
    case S_FALSE:
        m_state = QUERY_ISSUED;
        break;
    default:
        m_state = QUERY_ERROR;
        break;
    }
    return m_state;
}

// orig 0x642b30 query.cpp:149
QueryReturnValue const& Query::GetData()
{
    State state = GetState();
    if (state == QUERY_ERROR)
    {
        return m_retVal;
    }

    switch (m_type)
    {
    case QUERY_VCACHE:
        m_retVal.SetType(RV_VCache);
        while (m_query->GetData(RetValData(m_retVal), sizeof(VCACHE), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_EVENT:
        m_retVal.SetType(RV_Bool);
        while (m_query->GetData(0, 0, D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_OCCLUSION:
        m_retVal.SetType(RV_Dword);
        while (m_query->GetData(RetValData(m_retVal), sizeof(DWORD), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_TIMESTAMP:
        m_retVal.SetType(RV_Uint64);
        while (m_query->GetData(RetValData(m_retVal), sizeof(UINT64), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_TIMESTAMPDISJOINT:
        m_retVal.SetType(RV_Bool);
        while (m_query->GetData(0, 0, D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_TIMESTAMPFREQ:
        m_retVal.SetType(RV_Uint64);
        while (m_query->GetData(RetValData(m_retVal), sizeof(UINT64), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_PIPELINETIMINGS:
        m_retVal.SetType(RV_PipelineTimings);
        while (m_query->GetData(RetValData(m_retVal), sizeof(PIPELINETIMINGS), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_INTERFACETIMINGS:
        m_retVal.SetType(RV_InterfaceTimings);
        while (m_query->GetData(RetValData(m_retVal), sizeof(INTERFACETIMINGS), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_VERTEXTIMINGS:
        m_retVal.SetType(RV_StageTimings);
        while (m_query->GetData(RetValData(m_retVal), sizeof(STAGETIMINGS), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_BANDWIDTHTIMINGS:
        m_retVal.SetType(RV_BandWidthTimings);
        while (m_query->GetData(RetValData(m_retVal), sizeof(BANDWIDTHTIMINGS), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    case QUERY_CACHEUTILIZATION:
        m_retVal.SetType(RV_CacheUtilization);
        while (m_query->GetData(RetValData(m_retVal), sizeof(CACHEUTILIZATION), D3DGETDATA_FLUSH) == S_FALSE)
        {
        }
        break;
    default:
        break;
    }
    return m_retVal;
}

// orig 0x642d20 query.cpp:215
bool Query::IsValid() const
{
    return m_query != 0;
}

// orig 0x643180 query.cpp:222
Query::~Query()
{
    ReleaseQuery();
    m_dev->OnQueryDestructor(this);
}

// orig 0x643010 query.cpp:232
void CDevice::rstQueryPrepareFor()
{
    for (std::list<IQuery*>::iterator it = m_queries.begin(); it != m_queries.end(); ++it)
    {
        static_cast<Query*>(*it)->ReleaseQuery();
    }
}

// orig 0x643040 query.cpp:240
void CDevice::rstQueryRestoreAfter()
{
    for (std::list<IQuery*>::iterator it = m_queries.begin(); it != m_queries.end(); ++it)
    {
        if ((*it)->IsValid())
        {
            static_cast<Query*>(*it)->CreateQuery();
        }
    }
}

// orig 0x6430a0 query.cpp:249
void CDevice::OnQueryDestructor(IQuery* query)
{
    std::list<IQuery*>::iterator it = std::find(m_queries.begin(), m_queries.end(), query);
    if (it != m_queries.end())
    {
        m_queries.erase(it);
    }
}

// orig 0x6430f0 query.cpp:258
void CDevice::ReleaseQueries()
{
    for (std::list<IQuery*>::iterator it = m_queries.begin(); it != m_queries.end(); ++it)
    {
        g_kernel->KernelLog("Warning: Query with type %i is not released (refs = %d)", (*it)->GetType(),
                            (*it)->GetRefCount());
    }
}
