#include "minepusher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MinePusher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MinePusher);

    MinePusherPrototypeInfo::MinePusherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* MinePusherPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    MinePusher::MinePusher(MinePusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* MinePusher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    MinePusherPrototypeInfo const* MinePusher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* MinePusher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    MinePusher::~MinePusher()
    {
        throw std::logic_error("Not implemented");
    }

    void MinePusher::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    bool MinePusher::isLookAtPoint(CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* MinePusher::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* MinePusher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
