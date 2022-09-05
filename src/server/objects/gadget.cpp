#include "gadget.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Gadget)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Gadget);

    GadgetPrototypeInfo::GadgetApplicationInfo::GadgetApplicationInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool GadgetPrototypeInfo::ModificationInfo::ApplyToObj(ai::Obj*, bool) const
    {
        throw std::logic_error("Not implemented");
    }

    GadgetPrototypeInfo::ModificationInfo::ModificationInfo(CStr const&, ai::GadgetPrototypeInfo const*)
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* GadgetPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool GadgetPrototypeInfo::ApplyToVp(ai::VehiclePart*, bool) const
    {
        throw std::logic_error("Not implemented");
    }

    GadgetPrototypeInfo::GadgetPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    int GadgetPrototypeInfo::GetSkinNum() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& GadgetPrototypeInfo::GetModelName() const
    {
        throw std::logic_error("Not implemented");
    }

    bool GadgetPrototypeInfo::ApplyToVehicle(ai::Vehicle*, bool) const
    {
        throw std::logic_error("Not implemented");
    }

    bool GadgetPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<GadgetPrototypeInfo::ModificationInfo, std::allocator<GadgetPrototypeInfo::ModificationInfo>> const&
    GadgetPrototypeInfo::GetModifications() const
    {
        throw std::logic_error("Not implemented");
    }

    int Gadget::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    int Gadget::GetSlotNum() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gadget::SetSlotNum(int)
    {
        throw std::logic_error("Not implemented");
    }

    GadgetPrototypeInfo const* Gadget::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    Gadget::Gadget(GadgetPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Gadget::ApplyToVehicle(Vehicle*, bool) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gadget::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Gadget::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    bool Gadget::ApplyToVp(VehiclePart*, bool) const
    {
        throw std::logic_error("Not implemented");
    }

    void Gadget::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Gadget::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Gadget::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Gadget::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void Gadget::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void Gadget::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gadget::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Gadget::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    Gadget::~Gadget()
    {
        throw std::logic_error("Not implemented");
    }

    void Gadget::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Gadget::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Gadget::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}
