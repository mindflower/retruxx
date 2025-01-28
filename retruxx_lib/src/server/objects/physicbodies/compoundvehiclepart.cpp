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

    CompoundVehiclePart::~CompoundVehiclePart()
    {
        throw std::logic_error("Not implemented");
    }

    CompoundVehiclePart::CompoundVehiclePart(const CompoundVehiclePartPrototypeInfo& prototype) : VehiclePart(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CompoundVehiclePart::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* __fastcall CompoundVehiclePart::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* __fastcall CompoundVehiclePart::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    m3d::Class* CompoundVehiclePart::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::CompoundVehiclePartPrototypeInfo* CompoundVehiclePart::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CompoundVehiclePart::TVehiclePart::TVehiclePart()
    {
        throw std::logic_error("Not implemented");
    }

    void CompoundVehiclePart::SetVisible()
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::SetInvisible()
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::SetBelong(int newBelong)
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::Remove()
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::CreateChildren()
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::SetOwner(ai::PhysicObj* owner)
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::RelinkToSpace(dxSpace* newSpace)
    {
        throw std::logic_error("Not implemented");
    }
    float CompoundVehiclePart::GetMass() const
    {
        throw std::logic_error("Not implemented");
    }
    float CompoundVehiclePart::GetDurability() const
    {
        throw std::logic_error("Not implemented");
    }
    float CompoundVehiclePart::GetMaxDurability() const
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::SetDurability(float durability)
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::RegenerateDurability(float elapsedTime)
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::SetDurabilityRegeneration(float value)
    {
        throw std::logic_error("Not implemented");
    }
    unsigned int CompoundVehiclePart::GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }
    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::begin() const
    {
        throw std::logic_error("Not implemented");
        return VehiclePartsMap::const_iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::begin()
    {
        throw std::logic_error("Not implemented");
        return VehiclePartsMap::iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::end() const
    {
        throw std::logic_error("Not implemented");
        return VehiclePartsMap::const_iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::end()
    {
        throw std::logic_error("Not implemented");
        return VehiclePartsMap::iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::find(const CStr&) const
    {
        throw std::logic_error("Not implemented");
        return VehiclePartsMap::const_iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::find(const CStr& s)
    {
        throw std::logic_error("Not implemented");
        return VehiclePartsMap::iterator();
    }
    void CompoundVehiclePart::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::AddChild(ai::Obj* pObj)
    {
        throw std::logic_error("Not implemented");
    }
    bool CompoundVehiclePart::RemoveChild(ai::Obj* pObj)
    {
        throw std::logic_error("Not implemented");
        return false;
    }
    bool CompoundVehiclePart::CanChildBeAdded(m3d::Class* pClass) const
    {
        throw std::logic_error("Not implemented");
        return false;
    }
    void CompoundVehiclePart::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }
    void CompoundVehiclePart::ClearSavedStatus()
    {
        throw std::logic_error("Not implemented");
    }
}
