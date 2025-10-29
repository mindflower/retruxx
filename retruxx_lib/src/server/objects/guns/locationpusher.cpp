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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* LocationPusherPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* LocationPusher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    LocationPusher::LocationPusher(LocationPusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    LocationPusherPrototypeInfo const* LocationPusher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* LocationPusher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    LocationPusher::~LocationPusher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void LocationPusher::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* LocationPusher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* LocationPusher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool LocationPusher::isLookAtPoint(CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
