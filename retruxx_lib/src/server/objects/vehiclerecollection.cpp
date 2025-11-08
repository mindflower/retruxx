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
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRecollectionPrototypeInfo::VehicleRecollectionPrototypeInfo()
    {
    }

    bool VehicleRecollectionPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    VehicleRecollection::ReollectionItem::ReollectionItem(ReollectionItem const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRecollection::ReollectionItem::ReollectionItem(CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::ReollectionItem::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::ReollectionItem::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::Clear()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::VehicleRecollectionPrototypeInfo const* VehicleRecollection::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRecollection::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRecollection::VehicleRecollection(ai::VehicleRecollectionPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleRecollection::GetRecollectionPosition(float time) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRecollection::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    Vehicle* VehicleRecollection::GetVehicle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::SetVehicle(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRecollection::~VehicleRecollection()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRecollection::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRecollection::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRecollection::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
