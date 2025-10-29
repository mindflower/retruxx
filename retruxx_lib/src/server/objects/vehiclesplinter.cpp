#include "vehiclesplinter.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleSplinter)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleSplinter);

    Obj* VehicleSplinterPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleSplinter::VehicleSplinter(VehicleSplinterPrototypeInfo const& prototype) : DummyObject(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleSplinter::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DummyObject);
    }

    VehicleSplinterPrototypeInfo const* VehicleSplinter::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleSplinter::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleSplinter::~VehicleSplinter()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleSplinter::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleSplinter::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
