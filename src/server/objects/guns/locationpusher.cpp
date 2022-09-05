#include "locationpusher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(LocationPusher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(LocationPusher);

    LocationPusherPrototypeInfo::LocationPusherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* LocationPusherPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* LocationPusher::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    LocationPusher::LocationPusher(LocationPusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    LocationPusherPrototypeInfo const* LocationPusher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* LocationPusher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    LocationPusher::~LocationPusher()
    {
        throw std::logic_error("Not implemented");
    }

    void LocationPusher::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* LocationPusher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* LocationPusher::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    bool LocationPusher::isLookAtPoint(CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }
}
