#include "static2dpath.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, Push)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, Remove)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetX)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetY)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Static2DPath, GetCount)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Static2DPath::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Static2DPath::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    m3d::Object* Static2DPath::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Static2DPath::Push(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Static2DPath::GetCount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Static2DPath::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Static2DPath::GetX(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Static2DPath::GetY(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Static2DPath::~Static2DPath()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    std::vector<CVector, std::allocator<CVector>> Static2DPath::asVector()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Static2DPath::Static2DPath()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Static2DPath::Static2DPath(Static2DPath const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
