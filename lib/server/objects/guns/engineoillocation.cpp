#include "engineoillocation.h"

#include "core/kernel.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(EngineOilLocation)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(EngineOilLocation);

    EngineOilLocationPrototypeInfo::EngineOilLocationPrototypeInfo() = default;

    Obj* EngineOilLocationPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x846AA0
        return new EngineOilLocation(*this);
    }

    // RVA 0x846A10
    EngineOilLocation::~EngineOilLocation() = default;

    EngineOilLocation::EngineOilLocation(EngineOilLocationPrototypeInfo const& prototypeInfo) : TemporaryLocation(prototypeInfo)
    {
        // RVA 0x8469F0
    }

    m3d::Object* EngineOilLocation::Clone()
    {
        // RVA 0x846B10
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* __fastcall EngineOilLocation::CreateObject()
    {
        // RVA 0x846CD0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* __fastcall EngineOilLocation::GetBaseClass()
    {
        return RT_CLASS_LOCAL(TemporaryLocation);
    }

    m3d::Class* EngineOilLocation::GetClass() const
    {
        // RVA 0x8469D0
        return RT_CLASS_LOCAL(EngineOilLocation);
    }

    EngineOilLocationPrototypeInfo const* EngineOilLocation::GetPrototypeInfo() const
    {
        // RVA 0x846EE0 - NOTE: the prototype is cast without a type check.
        return static_cast<EngineOilLocationPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void EngineOilLocation::OnObjectIn(Obj* object)
    {
        // RVA 0x846A20 - NOTE: TemporaryLocation::OnObjectIn is not called.
        if (object->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            static_cast<Vehicle*>(object)->IncOnOilMode();
        }
    }

    void EngineOilLocation::OnObjectOut(Obj* object)
    {
        // RVA 0x846A40 - NOTE: TemporaryLocation::OnObjectOut is not called.
        if (object->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            static_cast<Vehicle*>(object)->DecOnOilMode();
        }
    }
}  // namespace ai
