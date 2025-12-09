#include "gadget.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Gadget)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Gadget);

    GadgetPrototypeInfo::GadgetApplicationInfo::GadgetApplicationInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GadgetPrototypeInfo::ModificationInfo::ApplyToObj(ai::Obj*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GadgetPrototypeInfo::ModificationInfo::ModificationInfo(CStr const&, ai::GadgetPrototypeInfo const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* GadgetPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GadgetPrototypeInfo::ApplyToVp(ai::VehiclePart*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GadgetPrototypeInfo::GadgetPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int GadgetPrototypeInfo::GetSkinNum() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& GadgetPrototypeInfo::GetModelName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GadgetPrototypeInfo::ApplyToVehicle(ai::Vehicle*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GadgetPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<GadgetPrototypeInfo::ModificationInfo, retruxx::allocator<GadgetPrototypeInfo::ModificationInfo>> const&
    GadgetPrototypeInfo::GetModifications() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gadget::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gadget::GetSlotNum() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gadget::SetSlotNum(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GadgetPrototypeInfo const* Gadget::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gadget::Gadget(GadgetPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gadget::ApplyToVehicle(Vehicle*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gadget::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gadget::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    bool Gadget::ApplyToVp(VehiclePart*, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gadget::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Gadget::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Gadget::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gadget::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gadget::Registration()
    {
        m_propertiesMap["SlotNum"] = 41;
    }

    void Gadget::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gadget::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gadget::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gadget::~Gadget()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gadget::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Gadget::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Gadget::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
