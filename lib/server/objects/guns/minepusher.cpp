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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* MinePusherPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MinePusher::MinePusher(MinePusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* MinePusher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MinePusherPrototypeInfo const* MinePusher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* MinePusher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    MinePusher::~MinePusher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MinePusher::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool MinePusher::isLookAtPoint(CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* MinePusher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* MinePusher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
