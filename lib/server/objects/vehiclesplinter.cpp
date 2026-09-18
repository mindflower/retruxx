#include "vehiclesplinter.h"

#include "base/prototypemanager.h"
#include <core/kernel.h>

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleSplinter)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleSplinter);

    Obj* VehicleSplinterPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x851C90
        return new VehicleSplinter(*this);
    }

    VehicleSplinter::VehicleSplinter(VehicleSplinterPrototypeInfo const& prototype) : DummyObject(prototype)
    {
        // RVA 0x851C60 - a plain DummyObject; unlike ParticleSplinter it stays in its own space.
    }

    m3d::Class* VehicleSplinter::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DummyObject);
    }

    VehicleSplinterPrototypeInfo const* VehicleSplinter::GetPrototypeInfo() const
    {
        // RVA 0x852080
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), VehicleSplinterPrototypeInfo const);
    }

    m3d::Class* VehicleSplinter::GetClass() const
    {
        // RVA 0x851C50
        return RT_CLASS_LOCAL(VehicleSplinter);
    }

    VehicleSplinter::~VehicleSplinter() = default;

    m3d::Object* VehicleSplinter::Clone()
    {
        // RVA 0x851D00
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* VehicleSplinter::CreateObject()
    {
        // RVA 0x851EC0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}
