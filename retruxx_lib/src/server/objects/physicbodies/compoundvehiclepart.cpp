#include "compoundvehiclepart.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompoundVehiclePart)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompoundVehiclePart);

    CompoundVehiclePartPrototypeInfo::CompoundVehiclePartPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompoundVehiclePartPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundVehiclePartPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompoundVehiclePart::TVehiclePart::TVehiclePart(ai::VehiclePart*, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompoundVehiclePart::~CompoundVehiclePart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompoundVehiclePart::CompoundVehiclePart(const CompoundVehiclePartPrototypeInfo& prototype) : VehiclePart(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CompoundVehiclePart::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* __fastcall CompoundVehiclePart::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* __fastcall CompoundVehiclePart::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    m3d::Class* CompoundVehiclePart::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::CompoundVehiclePartPrototypeInfo* CompoundVehiclePart::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompoundVehiclePart::TVehiclePart::TVehiclePart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompoundVehiclePart::SetVisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::SetInvisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::SetBelong(int newBelong)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::SetOwner(ai::PhysicObj* owner)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::RelinkToSpace(dxSpace* newSpace)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    float CompoundVehiclePart::GetMass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    float CompoundVehiclePart::GetDurability() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    float CompoundVehiclePart::GetMaxDurability() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::SetDurability(float durability)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::RegenerateDurability(float elapsedTime)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::SetDurabilityRegeneration(float value)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    unsigned int CompoundVehiclePart::GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::begin() const
    {
        RETRUXX_NOT_IMPLEMENTED;
        return VehiclePartsMap::const_iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::begin()
    {
        RETRUXX_NOT_IMPLEMENTED;
        return VehiclePartsMap::iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::end() const
    {
        RETRUXX_NOT_IMPLEMENTED;
        return VehiclePartsMap::const_iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::end()
    {
        RETRUXX_NOT_IMPLEMENTED;
        return VehiclePartsMap::iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::const_iterator CompoundVehiclePart::find(const CStr&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
        return VehiclePartsMap::const_iterator();
    }
    CompoundVehiclePart::VehiclePartsMap::iterator CompoundVehiclePart::find(const CStr& s)
    {
        RETRUXX_NOT_IMPLEMENTED;
        return VehiclePartsMap::iterator();
    }
    void CompoundVehiclePart::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::AddChild(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    bool CompoundVehiclePart::RemoveChild(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
        return false;
    }
    bool CompoundVehiclePart::CanChildBeAdded(m3d::Class* pClass) const
    {
        RETRUXX_NOT_IMPLEMENTED;
        return false;
    }
    void CompoundVehiclePart::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
    void CompoundVehiclePart::ClearSavedStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
