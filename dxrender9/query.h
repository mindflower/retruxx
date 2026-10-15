#pragma once
// Ported from the original dxrender9/query.h: the IDirect3DQuery9 wrapper handed out by
// CDevice::NewQuery.
#include <d3d9.h>

#include <renderer/i_renderer.h>

class CDevice;

class Query : public m3d::rend::IQuery
{
public:
    Type m_type;
    State m_state;
    IDirect3DQuery9* m_query;
    m3d::rend::QueryReturnValue m_retVal;

    static CDevice* m_dev;

    Query(Type type);
    virtual ~Query();

    int CreateQuery();
    void ReleaseQuery();

    // IRenderResource
    bool IsValid() const override;

    // IQuery
    // orig 0x642df0 query.h:28
    Type GetType() const override
    {
        return m_type;
    }
    void Begin() override;
    void End() override;
    State GetState() override;
    m3d::rend::QueryReturnValue const& GetData() override;
};
