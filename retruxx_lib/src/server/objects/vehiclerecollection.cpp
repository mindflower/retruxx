#include "vehiclerecollection.h"
#include "base/objcontainer.h"
#include <stdexcept>
#include <algorithm>
#include "base/globalproperties.h"
#include "vehicle.h"

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
        //const auto timeDiff = GetObjects()->GetGameTimeDiff();
        //const auto mult = GetGlobProp().m_gameTimeMult * 3.0;
        //
        //
        //if (!m_recollectionItems.empty() && m_recollectionItems.back().time > timeDiff + mult)
        //{
        //    m_recollectionItems.clear();
        //}
        //
        //bool addNew = true;
        //if (!m_recollectionItems.empty())
        //{
        //    const auto& lastItem = m_recollectionItems.back();
        //    if (timeDiff - mult <= lastItem.time && lastItem.time >= timeDiff + mult)
        //    {
        //        addNew = false;
        //    }
        //}
        //
        //
        //if (addNew)
        //{
        //    if (auto* vehicle = dynamic_cast<Vehicle*>(GetObjects()->GetEntityByObjId(m_vehicleId)))
        //    {
        //        m_recollectionItems.push_back(ReollectionItem{ vehicle->GetPosition(), timeDiff });
        //    }
        //    else
        //    {
        //        Remove();
        //    }
        //}
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

    CVector VehicleRecollection::GetRecollectionPosition(float time) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRecollection::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
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
