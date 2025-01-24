#include "gadget.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Gadget)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Gadget);

    GadgetPrototypeInfo::GadgetApplicationInfo::GadgetApplicationInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool GadgetPrototypeInfo::ModificationInfo::ApplyToObj(ai::Obj*, bool) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    GadgetPrototypeInfo::ModificationInfo::ModificationInfo(CStr const&, ai::GadgetPrototypeInfo const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ai::Obj* GadgetPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool GadgetPrototypeInfo::ApplyToVp(ai::VehiclePart*, bool) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    GadgetPrototypeInfo::GadgetPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int GadgetPrototypeInfo::GetSkinNum() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr const& GadgetPrototypeInfo::GetModelName() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool GadgetPrototypeInfo::ApplyToVehicle(ai::Vehicle*, bool) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool GadgetPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::vector<GadgetPrototypeInfo::ModificationInfo, retruxx::allocator<GadgetPrototypeInfo::ModificationInfo>> const&
    GadgetPrototypeInfo::GetModifications() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Gadget::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Gadget::GetSlotNum() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gadget::SetSlotNum(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    GadgetPrototypeInfo const* Gadget::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Gadget::Gadget(GadgetPrototypeInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gadget::ApplyToVehicle(Vehicle*, bool) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gadget::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Gadget::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    bool Gadget::ApplyToVp(VehiclePart*, bool) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gadget::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Gadget::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Gadget::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Gadget::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gadget::Registration()
    {
        m_propertiesMap["SlotNum"] = 41;
    }

    void Gadget::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gadget::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Gadget::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Gadget::~Gadget()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Gadget::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Gadget::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Gadget::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }
}
