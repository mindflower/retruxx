#include <level.h>
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Level, GetLandSize)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Level, GetLevelName)
{
    throw std::logic_error("Not implemented");
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Level)
        RT_CLASS_EXPORT(Level, m3d::METHOD, GetLandSize, "", "", "")
        RT_CLASS_EXPORT(Level, m3d::METHOD, GetLevelName, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Level);

    Class* Level::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    char const* Level::GetLevelName() const
    {
        throw std::logic_error("Not implemented");
    }

    int Level::GetLandSize() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* Level::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    int Level::Save(CStr const&, CCamera const&)
    {
        throw std::logic_error("Not implemented");
    }

    Level::~Level()
    {
        throw std::logic_error("Not implemented");
    }

    Object* Level::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    CStr Level::GetFullPathNameA(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Level::New(CCamera&, int)
    {
        throw std::logic_error("Not implemented");
    }

    Class* Level::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int Level::Load(CStr const&, CCamera&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    Level::Level(Level const&)
    {
        throw std::logic_error("Not implemented");
    }

    Level::Level()
    {
        throw std::logic_error("Not implemented");
    }
}
