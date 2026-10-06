#pragma once
// Ported from the original dxrender9/query.h: the IDirect3DQuery9 wrapper handed out by
// CDevice::NewQuery.
#include <d3d9.h>

#include <renderer/i_renderer.h>

class CDevice;

class Query : public m3d::rend::IQuery
{
public:
    /* 0x0008 */ Type m_type;
    /* 0x000c */ State m_state;
    /* 0x0010 */ IDirect3DQuery9* m_query;
    /* 0x0018 */ m3d::rend::QueryReturnValue m_retVal;

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
}; /* size: 0x0038 */
