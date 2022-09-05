#include "vehiclerecollection.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRecollection)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRecollection);

    ai::Obj* VehicleRecollectionPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRecollectionPrototypeInfo::VehicleRecollectionPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRecollectionPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRecollection::ReollectionItem::ReollectionItem(ReollectionItem const&)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRecollection::ReollectionItem::ReollectionItem(CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::ReollectionItem::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::ReollectionItem::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::Clear()
    {
        throw std::logic_error("Not implemented");
    }

    ai::VehicleRecollectionPrototypeInfo const* VehicleRecollection::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRecollection::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRecollection::VehicleRecollection(ai::VehicleRecollectionPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleRecollection::GetRecollectionPosition(float) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRecollection::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* VehicleRecollection::GetVehicle() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::SetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRecollection::~VehicleRecollection()
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRecollection::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRecollection::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRecollection::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}
