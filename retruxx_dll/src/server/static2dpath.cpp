#include "static2dpath.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, Push)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, Remove)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetX)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetY)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetCount)
{
    throw std::logic_error("Not implemented");
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

    void Static2DPath::Remove(int)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Static2DPath::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Static2DPath::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    m3d::Object* Static2DPath::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void Static2DPath::Push(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    int Static2DPath::GetCount() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Static2DPath::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    float Static2DPath::GetX(int) const
    {
        throw std::logic_error("Not implemented");
    }

    float Static2DPath::GetY(int) const
    {
        throw std::logic_error("Not implemented");
    }

    Static2DPath::~Static2DPath()
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<CVector, std::allocator<CVector>> Static2DPath::asVector()
    {
        throw std::logic_error("Not implemented");
    }

    Static2DPath::Static2DPath()
    {
        throw std::logic_error("Not implemented");
    }

    Static2DPath::Static2DPath(Static2DPath const&)
    {
        throw std::logic_error("Not implemented");
    }
}
