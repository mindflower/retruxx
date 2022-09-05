#include "turboaccelerationpusher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(TurboAccelerationPusher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(TurboAccelerationPusher);

    TurboAccelerationPusherPrototypeInfo::TurboAccelerationPusherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool TurboAccelerationPusherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* TurboAccelerationPusherPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    TurboAccelerationPusher::TurboAccelerationPusher(TurboAccelerationPusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TurboAccelerationPusher::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TurboAccelerationPusher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    TurboAccelerationPusherPrototypeInfo const* TurboAccelerationPusher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void TurboAccelerationPusher::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    TurboAccelerationPusher::~TurboAccelerationPusher()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TurboAccelerationPusher::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    bool TurboAccelerationPusher::isLookAtPoint(CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TurboAccelerationPusher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
