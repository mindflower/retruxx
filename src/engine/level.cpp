#include <level.h>
#include <stdexcept>

namespace m3d
{
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
