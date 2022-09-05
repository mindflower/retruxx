#include "compoundvehiclepart.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompoundVehiclePart)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompoundVehiclePart);

    CompoundVehiclePartPrototypeInfo::CompoundVehiclePartPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void CompoundVehiclePartPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundVehiclePartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    CompoundVehiclePart::TVehiclePart::TVehiclePart(ai::VehiclePart*, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    CompoundVehiclePart::CompoundVehiclePart(const CompoundVehiclePartPrototypeInfo& prototype) : VehiclePart(prototype)
    {
        throw std::logic_error("Not implemented");
    }
}
