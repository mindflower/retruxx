#include "static2dpath.h"

#include <stdexcept>

#include <core/kernel.h>
#include "server/utils.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, Push)
{
    // RVA 0x7F9F60
    auto path = (ai::Static2DPath*)context->asObject(0, "Static2DPath");
    const float y = context->asFloat(2);
    const float x = context->asFloat(1);
    path->Push(x, y);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, Remove)
{
    // RVA 0x7F9E80
    auto path = (ai::Static2DPath*)context->asObject(0, "Static2DPath");
    path->Remove(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetX)
{
    // RVA 0x7F9540
    auto path = (ai::Static2DPath*)context->asObject(0, "Static2DPath");
    context->pushFloat(path->GetX(context->asInt(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetY)
{
    // RVA 0x7F9580
    auto path = (ai::Static2DPath*)context->asObject(0, "Static2DPath");
    context->pushFloat(path->GetY(context->asInt(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetCount)
{
    // RVA 0x7F8C20
    auto path = (ai::Static2DPath*)context->asObject(0, "Static2DPath");
    context->pushInt(path->GetCount());
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Static2DPath)
        RT_CLASS_EXPORT(Static2DPath, m3d::METHOD, Push, "", "", "")
        RT_CLASS_EXPORT(Static2DPath, m3d::METHOD, Remove, "", "", "")
        RT_CLASS_EXPORT(Static2DPath, m3d::METHOD, GetX, "", "", "")
        RT_CLASS_EXPORT(Static2DPath, m3d::METHOD, GetY, "", "", "")
        RT_CLASS_EXPORT(Static2DPath, m3d::METHOD, GetCount, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Static2DPath);

    // RVA 0x7F9A00
    void Static2DPath::Remove(int index)
    {
        if (index < 0)
        {
            SYS_ERROR("index >= 0");
        }
        if ((uint32_t)index >= m_Points.size())
        {
            SYS_ERROR("(uint )index < m_Points.size()");
        }
        m_Points.erase(m_Points.begin() + index);
    }

    // RVA 0x7F9EB0
    m3d::Object* Static2DPath::CreateObject()
    {
        return new Static2DPath();
    }

    m3d::Class* Static2DPath::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    // RVA 0x7F9E20
    m3d::Object* Static2DPath::Clone()
    {
        return new Static2DPath(*this);
    }

    // RVA 0x7F9F30
    void Static2DPath::Push(float x, float y)
    {
        m_Points.push_back(PointBase<float>(x, y));
    }

    // RVA 0x7F8A00
    int Static2DPath::GetCount() const
    {
        return static_cast<int>(m_Points.size());
    }

    // RVA 0x7F87F0
    m3d::Class* Static2DPath::GetClass() const
    {
        return RT_CLASS_LOCAL(Static2DPath);
    }

    // RVA 0x7F8DC0
    float Static2DPath::GetX(int index) const
    {
        if (index < 0)
        {
            SYS_ERROR("index >= 0");
        }
        if ((uint32_t)index >= m_Points.size())
        {
            SYS_ERROR("(uint )index < m_Points.size()");
        }
        return m_Points[index].x;
    }

    // RVA 0x7F9150
    float Static2DPath::GetY(int index) const
    {
        if (index < 0)
        {
            SYS_ERROR("index >= 0");
        }
        if ((uint32_t)index >= m_Points.size())
        {
            SYS_ERROR("(uint )index < m_Points.size()");
        }
        return m_Points[index].y;
    }

    // RVA 0x7F99A0
    Static2DPath::~Static2DPath()
    {
    }

    // RVA 0x7F9FB0
    std::vector<CVector, std::allocator<CVector>> Static2DPath::asVector()
    {
        std::vector<CVector> result;
        for (int i = 0; i < GetCount(); ++i)
        {
            const CVector2 pos(m_Points[i].x, m_Points[i].y);
            result.push_back(GetGroundPos(pos, true));
        }
        return result;
    }

    // RVA 0x7F9940
    Static2DPath::Static2DPath()
    {
    }

    // RVA 0x7F9960
    Static2DPath::Static2DPath(Static2DPath const&)
    {
        // NOTE: the original always asserts; paths cannot be cloned, so Clone never returns.
        SYS_ERROR("0");
    }
}
