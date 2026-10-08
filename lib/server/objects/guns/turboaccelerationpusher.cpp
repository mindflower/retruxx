#include "turboaccelerationpusher.h"

#include <math/matrix.h>
#include <stdexcept>

#include "core/kernel.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(TurboAccelerationPusher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(TurboAccelerationPusher);

    TurboAccelerationPusherPrototypeInfo::TurboAccelerationPusherPrototypeInfo()
    {
        m_AccelerationValue = 1.0f;
        m_AccelerationTime = 0.0f;
    }

    bool TurboAccelerationPusherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_AccelerationValue, xmlNode, "AccelerationValue");
            m3d::SafeFloatAttrib(m_AccelerationTime, xmlNode, "AccelerationTime");
        }
        return result;
    }

    Obj* TurboAccelerationPusherPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x846540
        return new TurboAccelerationPusher(*this);
    }

    TurboAccelerationPusher::TurboAccelerationPusher(TurboAccelerationPusherPrototypeInfo const& prototype) : Gun(prototype)
    {
        // RVA 0x846420
    }

    m3d::Class* TurboAccelerationPusher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    m3d::Class* TurboAccelerationPusher::GetClass() const
    {
        // RVA 0x846410
        return RT_CLASS_LOCAL(TurboAccelerationPusher);
    }

    TurboAccelerationPusherPrototypeInfo const* TurboAccelerationPusher::GetPrototypeInfo() const
    {
        // RVA 0x846930
        // NOTE: the prototype is cast without a type check.
        return static_cast<TurboAccelerationPusherPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void TurboAccelerationPusher::_LaunchShells()
    {
        // RVA 0x846460
        // Firing it gives the vehicle it is mounted on a turbo boost.
        Gun::_LaunchShells();
        TurboAccelerationPusherPrototypeInfo const* prototype = GetPrototypeInfo();
        Obj* obj = this;
        while (!obj->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            obj = static_cast<Obj*>(obj->GetParent());
            if (!obj)
            {
                return;
            }
        }
        Vehicle* vehicle = static_cast<Vehicle*>(obj);
        vehicle->SetTurboThrottleTime(prototype->m_AccelerationTime);
        vehicle->SetTurboThrottleValue(prototype->m_AccelerationValue);
    }

    // RVA 0x846440
    TurboAccelerationPusher::~TurboAccelerationPusher() = default;

    m3d::Object* TurboAccelerationPusher::Clone()
    {
        // RVA 0x8465B0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    bool TurboAccelerationPusher::isLookAtPoint(CVector const&, float) const
    {
        // RVA 0x846450
        // It has nothing to aim, so it always counts as aimed.
        return true;
    }

    m3d::Object* TurboAccelerationPusher::CreateObject()
    {
        // RVA 0x846770
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai
