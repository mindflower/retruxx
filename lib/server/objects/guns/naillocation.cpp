#include "naillocation.h"

#include <stdexcept>

#include "core/kernel.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(NailLocation)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(NailLocation);

    NailLocationPrototypeInfo::NailLocationPrototypeInfo() = default;

    ai::Obj* NailLocationPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x864790
        return new NailLocation(*this);
    }

    m3d::Object* NailLocation::Clone()
    {
        // RVA 0x864800
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* NailLocation::CreateObject()
    {
        // RVA 0x8649C0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* NailLocation::GetBaseClass()
    {
        // RVA 0x864700
        return RT_CLASS_LOCAL(TemporaryLocation);
    }

    m3d::Class* NailLocation::GetClass() const
    {
        // RVA 0x864710
        return RT_CLASS_LOCAL(NailLocation);
    }

    void NailLocation::OnObjectIn(ai::Obj* object)
    {
        // RVA 0x864750
        // Nails stop the vehicles that drive in, until they leave again.
        if (object->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            static_cast<Vehicle*>(object)->IncStoppageMode();
        }
    }

    void NailLocation::OnObjectOut(ai::Obj* object)
    {
        // RVA 0x864770
        if (object->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            static_cast<Vehicle*>(object)->DecStoppageMode();
        }
    }

    // RVA 0x864740
    NailLocation::~NailLocation() = default;

    NailLocation::NailLocation(ai::NailLocationPrototypeInfo const& prototypeInfo) : TemporaryLocation(prototypeInfo)
    {
        // RVA 0x864720
    }

    ai::NailLocationPrototypeInfo const* NailLocation::GetPrototypeInfo() const
    {
        // RVA 0x864BD0
        // NOTE: the prototype is cast without a type check.
        return static_cast<NailLocationPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }
}  // namespace ai
