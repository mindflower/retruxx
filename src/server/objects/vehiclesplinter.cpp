#include "vehiclesplinter.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleSplinter)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleSplinter);

    Obj* VehicleSplinterPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleSplinter::VehicleSplinter(VehicleSplinterPrototypeInfo const& prototype) : DummyObject(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleSplinter::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    VehicleSplinterPrototypeInfo const* VehicleSplinter::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleSplinter::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleSplinter::~VehicleSplinter()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleSplinter::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleSplinter::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
