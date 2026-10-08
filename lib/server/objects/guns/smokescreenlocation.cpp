#include "smokescreenlocation.h"

#include <stdexcept>

#include "core/kernel.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SmokeScreenLocation)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SmokeScreenLocation);

    SmokeScreenLocationPrototypeInfo::SmokeScreenLocationPrototypeInfo() = default;

    ai::Obj* SmokeScreenLocationPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x864290
        return new SmokeScreenLocation(*this);
    }

    m3d::Object* SmokeScreenLocation::Clone()
    {
        // RVA 0x864300
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* SmokeScreenLocation::CreateObject()
    {
        // RVA 0x8644C0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* SmokeScreenLocation::GetBaseClass()
    {
        // RVA 0x864200
        return RT_CLASS_LOCAL(TemporaryLocation);
    }

    m3d::Class* SmokeScreenLocation::GetClass() const
    {
        // RVA 0x864210
        return RT_CLASS_LOCAL(SmokeScreenLocation);
    }

    void SmokeScreenLocation::OnObjectIn(ai::Obj* object)
    {
        // RVA 0x864250
        // Vehicles inside the smoke are hidden, until they leave it again.
        if (object->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            static_cast<Vehicle*>(object)->IncInSmokeScreenMode();
        }
    }

    void SmokeScreenLocation::OnObjectOut(ai::Obj* object)
    {
        // RVA 0x864270
        if (object->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            static_cast<Vehicle*>(object)->DecInSmokeScreenMode();
        }
    }

    // RVA 0x864240
    SmokeScreenLocation::~SmokeScreenLocation() = default;

    SmokeScreenLocation::SmokeScreenLocation(ai::SmokeScreenLocationPrototypeInfo const& prototypeInfo) : TemporaryLocation(prototypeInfo)
    {
        // RVA 0x864220
    }

    ai::SmokeScreenLocationPrototypeInfo const* SmokeScreenLocation::GetPrototypeInfo() const
    {
        // RVA 0x8646D0
        // NOTE: the prototype is cast without a type check.
        return static_cast<SmokeScreenLocationPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }
}  // namespace ai
