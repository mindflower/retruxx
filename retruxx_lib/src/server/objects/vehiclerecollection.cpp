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
        return new VehicleRecollection(*this);
    }

    VehicleRecollectionPrototypeInfo::VehicleRecollectionPrototypeInfo() = default;

    bool VehicleRecollectionPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    VehicleRecollection::ReollectionItem::ReollectionItem(CVector const& _pos, float _time)
    {
        pos = _pos;
        time = _time;
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
        const float currentTime = theObjects->GetGameTimeDiff();
        const float removalThreshold = currentTime - (theGlobProp.m_gameTimeMult * 3.0f);

        if (!m_recollectionItems.empty() && m_recollectionItems.back().time > currentTime)
        {
            m_recollectionItems.clear();
        }

        // Remove old entries that exceed the time threshold
        while (!m_recollectionItems.empty())
        {
            const auto& oldestItem = m_recollectionItems.front();
            if (removalThreshold <= oldestItem.time)
            {
                break;
            }

            // Remove the oldest item
            m_recollectionItems.erase(m_recollectionItems.begin());
        }

        // Check if we should add a new recollection entry
        const bool shouldAddNewEntry =
            m_recollectionItems.empty() || (currentTime > (m_recollectionItems.back().time + theGlobProp.m_gameTimeMult * 0.3f));

        if (shouldAddNewEntry)
        {
            if (m_vehicleId >= 0)
            {
                // Get the object record for the vehicle
                auto* vehicleObj = RT_DYNCAST(theObjects->GetEntityByObjId(m_vehicleId), Vehicle);
                if (vehicleObj != nullptr)
                {
                    // Create new recollection item with current position
                    ReollectionItem newItem(vehicleObj->GetGeometricCenter(), currentTime);
                    m_recollectionItems.push_back(std::move(newItem));
                    return;
                }
            }

            // If we get here, the vehicle is no longer valid - remove this recollection
            Remove();
        }
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

    VehicleRecollection::VehicleRecollection(ai::VehicleRecollectionPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_vehicleId = -1;
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

    void VehicleRecollection::SetVehicle(Vehicle const* vehicle)
    {
        m_vehicleId = vehicle->GetId();
    }

    VehicleRecollection::~VehicleRecollection() = default;

    void VehicleRecollection::_InternalCreateVisualPart()
    {
        m_recollectionItems.clear();
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
